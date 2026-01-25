#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: run_strategy_benchmark_3d.sh [--repeats N] [--out-csv PATH]
EOF
}

match_reps=5
results_csv_override=""
half_time_timeout_sec=420
progress_interval_sec="${PROGRESS_INTERVAL_SEC:-60}"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --repeats)
      match_reps="$2"
      shift 2
      ;;
    --out-csv)
      results_csv_override="$2"
      shift 2
      ;;
    --half-time-timeout-sec)
      half_time_timeout_sec="$2"
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
env_root="$(cd "${script_dir}/.." && pwd)"
repo_root="$(cd "${env_root}/../.." && pwd)"

fcp_dir="${env_root}/FCPCodebase"
venv_activate="${fcp_dir}/.venv/bin/activate"
launcher="${env_root}/scripts/run_rcssserver3d_and_RoboViz.sh"
log_dir="${env_root}/strategy_benchmark_logs_3d"
results_csv="${log_dir}/strategy_benchmark_results_3d.csv"
if [[ -n "${results_csv_override}" ]]; then
  results_csv="${results_csv_override}"
fi
roboviz_disable="${ROBOVIZ_DISABLE:-1}"
roboviz_log_dir="${log_dir}/match_logs"
parser="${repo_root}/scripts/utils/parse_roboviz_log.py"
half_eps="0.5"

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
    echo "[BENCH] trainer_send_failed=${name} raw=${payload}"
    return 1
  fi
  echo "[BENCH] trainer_sent=${name} raw=${payload}"
}

parse_log_field() {
  local logfile="$1"
  local key="$2"
  python3 "${parser}" "${logfile}" 2>/dev/null | awk -F= -v k="${key}" '$1==k {print $2; exit}'
}

get_log_snapshot() {
  local logfile="$1"
  python3 "${parser}" "${logfile}" 2>/dev/null | awk -F= '
    $1=="latest_time"{t=$2}
    $1=="latest_half"{h=$2}
    $1=="latest_play_mode"{p=$2}
    $1=="latest_score_left"{sl=$2}
    $1=="latest_score_right"{sr=$2}
    END{
      if (t=="") t="NOT_FOUND";
      if (h=="") h="NOT_FOUND";
      if (p=="") p="NOT_FOUND";
      if (sl=="") sl="NOT_FOUND";
      if (sr=="") sr="NOT_FOUND";
      print t "|" h "|" p "|" sl "|" sr
    }'
}

wait_for_logfile() {
  local launcher_log="$1"
  local timeout=30
  local waited=0
  while [ "$waited" -lt "$timeout" ]; do
    if [ -f "${launcher_log}" ]; then
      local line
      line="$(grep -m1 -E 'Recording to new logfile:' "${launcher_log}" || true)"
      if [ -n "${line}" ]; then
        echo "${line}" | sed -n 's/.*Recording to new logfile: //p' | tr -d '\r'
        return 0
      fi
    fi
    sleep 0.5
    waited=$((waited + 1))
  done
  return 1
}

format_elapsed() {
  local total="$1"
  printf "%02d:%02d" $((total / 60)) $((total % 60))
}

wait_for_half_time() {
  local pair_id="$1"
  local logfile="$2"
  local match_start_epoch="$3"
  local deadline=$((SECONDS + half_time_timeout_sec))
  local last_progress_bucket=-1
  local last_sim_time=""
  local diag_checked=0
  while [ "$SECONDS" -lt "$deadline" ]; do
    if ! server_alive; then
      return 1
    fi
    local snapshot
    snapshot="$(get_log_snapshot "${logfile}")"
    local sim_time sim_half sim_mode sim_left sim_right
    IFS='|' read -r sim_time sim_half sim_mode sim_left sim_right <<< "${snapshot}"
    local now_epoch wall_elapsed progress_bucket wall_fmt sim_display
    now_epoch="$(date +%s)"
    wall_elapsed=$((now_epoch - match_start_epoch))
    progress_bucket=$((wall_elapsed / progress_interval_sec))
    if [ "${diag_checked}" -eq 0 ] && [ "${wall_elapsed}" -ge 20 ]; then
      diag_checked=1
      local time_token time_value
      if command -v rg >/dev/null 2>&1; then
        time_token="$(rg -o "\\(time [0-9]+(\\.[0-9]+)?\\)" "${logfile}" | tail -n 1 || true)"
      else
        time_token="$(grep -Eo "\\(time [0-9]+(\\.[0-9]+)?\\)" "${logfile}" | tail -n 1 || true)"
      fi
      time_value="${time_token#(time }"
      time_value="${time_value%)}"
      if [ -z "${time_value}" ]; then
        echo "[BENCH] sim_time not advancing in log; using wall_elapsed"
      else
        if ! awk "BEGIN {exit !(${time_value} > 0)}"; then
          echo "[BENCH] sim_time not advancing in log; using wall_elapsed"
        fi
      fi
    fi
    if [ "${progress_bucket}" -ne "${last_progress_bucket}" ] || [ "${sim_time}" != "${last_sim_time}" ]; then
      wall_fmt="$(format_elapsed "${wall_elapsed}")"
      sim_display="${sim_time}"
      if [ -z "${sim_display}" ] || [ "${sim_display}" = "NOT_FOUND" ]; then
        sim_display="NA"
      fi
      echo "[MATCH PROGRESS] pair_id=${pair_id} wall_elapsed=${wall_fmt} sim_time=${sim_display} half=${sim_half} play_mode=${sim_mode}"
      last_progress_bucket="${progress_bucket}"
      last_sim_time="${sim_time}"
    fi
    if [ "${sim_half}" = "2" ]; then
      return 0
    fi
    sleep 1
  done
  return 2
}

agent_pids=()
launcher_pid=""
launcher_log=""
cleanup_done=0

cleanup_match() {
  set +e
  for pid in "${agent_pids[@]}"; do
    terminate_pid "$pid"
  done
  terminate_pid "$launcher_pid"

  sleep 1

  for pid in "${agent_pids[@]}"; do
    kill_pid "$pid"
  done
  kill_pid "$launcher_pid"
  launcher_log=""
}

cleanup_all() {
  if [ "$cleanup_done" -eq 1 ]; then
    return
  fi
  cleanup_done=1
  cleanup_match
}

trap cleanup_all EXIT
trap 'cleanup_all; exit 130' INT TERM

kill_stale_processes() {
  if ! command -v pgrep >/dev/null 2>&1; then
    return
  fi
  local patterns=("rcssserver3d" "RoboViz" "Run_Player.py")
  local found=0
  for pat in "${patterns[@]}"; do
    if pgrep -f "$pat" >/dev/null 2>&1; then
      echo "[BENCH] stopping stale processes: ${pat}"
      pkill -TERM -f "$pat" >/dev/null 2>&1 || true
      found=1
    fi
  done
  if [ "$found" -eq 1 ]; then
    sleep 1
    for pat in "${patterns[@]}"; do
      pkill -KILL -f "$pat" >/dev/null 2>&1 || true
    done
  fi
}

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
  launcher_log="$(mktemp)"
  start_cmd bash -c "env ROBOVIZ_DISABLE=\"${roboviz_disable}\" \"${launcher}\" >\"${launcher_log}\" 2>&1"
  launcher_pid=$last_pid
  if ! wait_for_port 3200; then
    echo "[BENCH] warning: monitor port 3200 not detected; proceeding"
  else
    echo "[BENCH] monitor port 3200 open"
  fi
}

kill_stale_processes

mkdir -p "${log_dir}"
mkdir -p "${roboviz_log_dir}"
expected_header="pair_id,left_team,right_team,left_goals,right_goals,timestamp,status"
if [[ ! -f "${results_csv}" ]]; then
  echo "${expected_header}" > "${results_csv}"
else
  current_header="$(head -n 1 "${results_csv}")"
  if [[ "${current_header}" != "${expected_header}" ]]; then
    tmp_csv="$(mktemp)"
    echo "${expected_header}" > "${tmp_csv}"
    if tail -n +2 "${results_csv}" | grep -q .; then
      while IFS= read -r line; do
        [[ -z "${line}" ]] && continue
        echo "${line},unknown" >> "${tmp_csv}"
      done < <(tail -n +2 "${results_csv}")
    fi
    mv "${tmp_csv}" "${results_csv}"
  fi
fi

base_left="BASIC"
opponents=(NOISE DEFLOCK HIPRESS DIRECT AGGRO)
match_total=$(( ${#opponents[@]} * 2 * match_reps ))
match_index=0

for opponent in "${opponents[@]}"; do
  for side in 0 1; do
    for rep in $(seq 1 "${match_reps}"); do
      if [[ "${side}" -eq 0 ]]; then
        left="${base_left}"
        right="${opponent}"
      else
        left="${opponent}"
        right="${base_left}"
      fi

      match_index=$((match_index + 1))
      pair_id="${left}_${right}"
      match_start_epoch="$(date +%s)"
      echo "[SIM RESET] restarting server+roboviz for next match"
      echo "[MATCH START] pair_id=${pair_id} index=${match_index}/${match_total}"

      cleanup_match
      kill_stale_processes
      start_launcher

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

      log_file=""
      if [[ "${roboviz_disable}" == "1" ]]; then
        echo "[BENCH] warning: ROBOVIZ_DISABLE=1; cannot capture logfile"
      else
        log_file="$(wait_for_logfile "${launcher_log}" || true)"
      fi

      if [ -z "${log_file}" ]; then
        status="parse_error"
        echo "[BENCH] warning: logfile not detected; marking parse_error"
      else
        echo "[LOG] pair_id=${pair_id} logfile=${log_file}"
        sleep 1
        send_trainer_cmd "drop_ball" "(dropBall)" || true
        status="ok"
      fi

      match_failed=0
      left_goals="NA"
      right_goals="NA"
      if [[ "${status}" == "ok" ]]; then
        if wait_for_half_time "${pair_id}" "${log_file}" "${match_start_epoch}"; then
          sleep 1
          snapshot="$(get_log_snapshot "${log_file}")"
          IFS='|' read -r _ _ _ sim_left sim_right <<< "${snapshot}"
          if [[ "${sim_left}" != "NOT_FOUND" && "${sim_right}" != "NOT_FOUND" ]]; then
            left_goals="${sim_left}"
            right_goals="${sim_right}"
          else
            status="parse_error"
          fi
        else
          wait_status=$?
          if [ "${wait_status}" -eq 1 ]; then
            status="server_dead"
          elif [ "${wait_status}" -eq 2 ]; then
            status="timeout"
          else
            status="parse_error"
          fi
        fi
      fi
      timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
      echo "${pair_id},${left},${right},${left_goals},${right_goals},${timestamp},${status}" >> "${results_csv}"
      echo "[MATCH END] pair_id=${pair_id}"

      cleanup_match
      sleep 2
    done
  done
done
