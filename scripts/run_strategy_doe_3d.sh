#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/run_strategy_doe_3d.sh [--repeats N] [--out-csv PATH]
EOF
}

repeats=5
out_csv=""

while [[ $# -gt 0 ]]; do
  case "$1" in
    --repeats)
      repeats="$2"
      shift 2
      ;;
    --out-csv)
      out_csv="$2"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown argument: $1"
      usage
      exit 1
      ;;
  esac
done

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"
env_root="${repo_root}/environment/3d-environment"
fcp_dir="${env_root}/FCPCodebase"
venv_activate="${fcp_dir}/.venv/bin/activate"
launcher="${env_root}/scripts/run_rcssserver3d_and_RoboViz.sh"
log_dir="${env_root}/strategy_benchmark_logs_3d/match_logs"
parser="${repo_root}/scripts/utils/parse_roboviz_log.py"

if [[ -z "${out_csv}" ]]; then
  out_csv="${env_root}/strategy_benchmark_logs_3d/strategy_benchmark_results.csv"
fi

mkdir -p "$(dirname "${out_csv}")"
mkdir -p "${log_dir}"

expected_header="pair_id,left_team,right_team,left_goals,right_goals,timestamp,status"
if [[ ! -f "${out_csv}" ]]; then
  echo "${expected_header}" > "${out_csv}"
fi

use_setsid=0
if command -v setsid >/dev/null 2>&1; then
  use_setsid=1
fi

last_pid=""
start_cmd() {
  if [ "$use_setsid" -eq 1 ]; then
    setsid "$@" &
  else
    "$@" &
  fi
  last_pid=$!
}

terminate_pid() {
  local pid="$1"
  if [ -z "$pid" ]; then
    return
  fi
  if [ "$use_setsid" -eq 1 ]; then
    kill -TERM -- "-${pid}" 2>/dev/null || true
  else
    kill -TERM "$pid" 2>/dev/null || true
    if command -v pkill >/dev/null 2>&1; then
      pkill -TERM -P "$pid" 2>/dev/null || true
    fi
  fi
}

kill_pid() {
  local pid="$1"
  if [ -z "$pid" ]; then
    return
  fi
  if [ "$use_setsid" -eq 1 ]; then
    kill -KILL -- "-${pid}" 2>/dev/null || true
  else
    kill -KILL "$pid" 2>/dev/null || true
    if command -v pkill >/dev/null 2>&1; then
      pkill -KILL -P "$pid" 2>/dev/null || true
    fi
  fi
}

wait_for_port() {
  local port="$1"
  if ! command -v ss >/dev/null 2>&1; then
    return 0
  fi
  local tries=40
  while [ "$tries" -gt 0 ]; do
    if ss -lnt 2>/dev/null | grep -q ":${port}"; then
      return 0
    fi
    sleep 0.25
    tries=$((tries - 1))
  done
  return 1
}

send_trainer_cmd() {
  local name="$1"
  local payload="$2"
  if ! python3 - "$payload" <<'PY'
import socket, struct, sys
host = "127.0.0.1"
port = 3200
msg = sys.argv[1]
data = msg.encode("ascii")
hdr = struct.pack("!I", len(data))
with socket.create_connection((host, port), timeout=2) as s:
    s.sendall(hdr + data)
PY
  then
    echo "[DOE] trainer_send_failed=${name} raw=${payload}"
    return 1
  fi
  echo "[DOE] trainer_sent=${name} raw=${payload}"
}

latest_log_since() {
  local since_epoch="$1"
  local latest=""
  latest="$(find "${log_dir}" -maxdepth 1 -type f -name 'roboviz_log_*.log' -printf '%T@ %p\n' 2>/dev/null | sort -nr | head -n 1 | awk '{print $2}')"
  if [ -z "${latest}" ]; then
    return 1
  fi
  local mtime
  mtime="$(stat -c %Y "${latest}" 2>/dev/null || echo 0)"
  if [ "${mtime}" -lt "${since_epoch}" ]; then
    return 1
  fi
  echo "${latest}"
}

parse_log_field() {
  local logfile="$1"
  local key="$2"
  python3 "${parser}" "${logfile}" 2>/dev/null | awk -F= -v k="${key}" '$1==k {print $2; exit}'
}

launcher_pid=""
agent_pids=()

cleanup_match() {
  for pid in "${agent_pids[@]}"; do
    terminate_pid "$pid"
  done
  terminate_pid "$launcher_pid"
  sleep 1
  for pid in "${agent_pids[@]}"; do
    kill_pid "$pid"
  done
  kill_pid "$launcher_pid"
  agent_pids=()
  launcher_pid=""
}

cleanup_all() {
  set +e
  cleanup_match
}

trap cleanup_all EXIT
trap 'cleanup_all; exit 130' INT TERM

server_alive() {
  if [ -z "${launcher_pid}" ]; then
    return 1
  fi
  if ! kill -0 "${launcher_pid}" 2>/dev/null; then
    return 1
  fi
  if command -v ss >/dev/null 2>&1; then
    if ! ss -lnt 2>/dev/null | grep -q ":3200"; then
      return 1
    fi
  fi
  return 0
}

start_launcher() {
  start_cmd env ROBOVIZ_DISABLE=0 "$launcher"
  launcher_pid=$last_pid
  if ! wait_for_port 3200; then
    echo "[DOE] warning: monitor port 3200 not detected; proceeding"
  else
    echo "[DOE] monitor port 3200 open"
  fi
}

start_agents() {
  local left="$1"
  local right="$2"

  agent_pids=()
  for u in 1 2 3 4; do
    start_cmd bash -c "cd \"${fcp_dir}\" && source \"${venv_activate}\" && python Run_Player.py -t ${left} -u ${u} --strategy ${left}"
    agent_pids+=("$last_pid")
    sleep 0.3
  done
  sleep 0.5
  for u in 1 2 3 4; do
    start_cmd bash -c "cd \"${fcp_dir}\" && source \"${venv_activate}\" && python Run_Player.py -t ${right} -u ${u} --strategy ${right}"
    agent_pids+=("$last_pid")
    sleep 0.3
  done
}

wait_for_half_time() {
  local match_start_epoch="$1"
  local max_wait_sec=420
  local deadline=$((match_start_epoch + max_wait_sec))
  local log_file=""
  while true; do
    if ! server_alive; then
      return 1
    fi
    log_file="$(latest_log_since "${match_start_epoch}")" || true
    if [ -n "${log_file}" ]; then
      local half_time
      half_time="$(parse_log_field "${log_file}" "half_time_time")"
      if [ -n "${half_time}" ] && [ "${half_time}" != "NOT_FOUND" ]; then
        echo "${log_file}"
        return 0
      fi
    fi
    if [ "$(date +%s)" -ge "${deadline}" ]; then
      return 2
    fi
    sleep 1
  done
}

base="BASIC"
opponents=(NOISE DEFLOCK HIPRESS DIRECT AGGRO)
match_total=$(( ${#opponents[@]} * 2 * repeats ))
match_index=0

for opponent in "${opponents[@]}"; do
  for side in 0 1; do
    for rep in $(seq 1 "${repeats}"); do
      if [[ "${side}" -eq 0 ]]; then
        left="${base}"
        right="${opponent}"
      else
        left="${opponent}"
        right="${base}"
      fi

      match_index=$((match_index + 1))
      pair_id="${left}_${right}"
      echo "[MATCH START] pair_id=${pair_id} index=${match_index}/${match_total}"

      cleanup_match

      match_start_epoch="$(date +%s)"
      start_launcher
      start_agents "${left}" "${right}"
      sleep 1
      send_trainer_cmd "drop_ball" "(dropBall)" || true

      status="ok"
      log_file=""
      if log_file="$(wait_for_half_time "${match_start_epoch}")"; then
        sleep 1
      else
        if server_alive; then
          status="parse_error"
        else
          status="server_dead"
        fi
      fi

      left_goals="NA"
      right_goals="NA"
      if [[ "${status}" == "ok" ]]; then
        score_latest="$(parse_log_field "${log_file}" "score_latest")"
        if [[ -n "${score_latest}" && "${score_latest}" != "NOT_FOUND" ]]; then
          left_goals="${score_latest%,*}"
          right_goals="${score_latest#*,}"
        else
          status="parse_error"
        fi
      fi

      timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
      echo "${pair_id},${left},${right},${left_goals},${right_goals},${timestamp},${status}" >> "${out_csv}"
      echo "[MATCH END] pair_id=${pair_id}"

      cleanup_match
    done
  done
done
