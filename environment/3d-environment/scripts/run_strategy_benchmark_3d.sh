#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: run_strategy_benchmark_3d.sh [--repeats N] [--out-csv PATH] [--half-time-timeout-sec N] [--pairs STRAT_A,STRAT_B] [--unums N1,N2,...]
EOF
}

if [[ "${BENCH_INHIBIT_ACTIVE:-0}" != "1" ]] && [[ "${BENCH_NO_INHIBIT:-0}" != "1" ]]; then
  if command -v systemd-inhibit >/dev/null 2>&1; then
    exec systemd-inhibit --what=idle:sleep --why="3D benchmark" env BENCH_INHIBIT_ACTIVE=1 "$0" "$@"
  fi
fi

if [[ "${BENCH_PG_ACTIVE:-0}" != "1" ]]; then
  if command -v setsid >/dev/null 2>&1; then
    setsid env BENCH_PG_ACTIVE=1 "$0" "$@" &
    worker_pid=$!
    trap 'kill -TERM -- "-${worker_pid}" 2>/dev/null || true; wait "${worker_pid}" 2>/dev/null || true; exit 130' INT TERM
    wait "${worker_pid}"
    exit $?
  fi
fi

match_reps=5
results_csv_override=""
half_time_timeout_sec=420
pairs_override=""
unums_override=""
match_wall_timeout_sec="${MATCH_WALL_TIMEOUT_SEC:-1800}"
progress_interval_sec="${PROGRESS_INTERVAL_SEC:-60}"
current_pair_id=""
current_left=""
current_right=""
current_match_id=""
current_row_written=1
bench_pgid=""
cpp_checked=0

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
    --pairs)
      pairs_override="$2"
      shift 2
      ;;
    --unums)
      unums_override="$2"
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
roboviz_disable="${ROBOVIZ_DISABLE:-0}"
roboviz_log_dir="${log_dir}/match_logs"
parser="${repo_root}/scripts/utils/parse_roboviz_log.py"
half_eps="0.5"

use_setsid=0

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

wait_for_port_close() {
  local port="$1"
  if ! command -v ss >/dev/null 2>&1; then
    return 0
  fi
  local tries=40
  while [ "$tries" -gt 0 ]; do
    if ! ss -lnt 2>/dev/null | grep -q ":${port}"; then
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
  python3 "${parser}" "${logfile}" 2>/dev/null | awk -F= -v k="${key}" '$1==k {print $2; exit}' || true
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
    }' || true
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
  local wall_deadline=$((match_start_epoch + match_wall_timeout_sec))
  local last_progress_wall=-1
  local last_sim_time=""
  local last_sim_half=""
  local last_sim_mode=""
  local diag_checked=0
  while [ "$SECONDS" -lt "$deadline" ]; do
    if ! server_alive; then
      return 1
    fi
    if ! agents_alive; then
      return 4
    fi
    local snapshot
    snapshot="$(get_log_snapshot "${logfile}")"
    local sim_time sim_half sim_mode sim_left sim_right
    IFS='|' read -r sim_time sim_half sim_mode sim_left sim_right <<< "${snapshot}"
    local now_epoch wall_elapsed wall_fmt sim_display should_print
    now_epoch="$(date +%s)"
    if [ "${now_epoch}" -ge "${wall_deadline}" ]; then
      return 3
    fi
    wall_elapsed=$((now_epoch - match_start_epoch))
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
    should_print=0
    if [ "${last_progress_wall}" -lt 0 ]; then
      last_progress_wall="${wall_elapsed}"
      last_sim_time="${sim_time}"
      last_sim_half="${sim_half}"
      last_sim_mode="${sim_mode}"
    elif [ $((wall_elapsed - last_progress_wall)) -ge "${progress_interval_sec}" ]; then
      should_print=1
    elif [ "${sim_half}" != "${last_sim_half}" ] || [ "${sim_mode}" != "${last_sim_mode}" ]; then
      should_print=1
    fi
    if [ "${should_print}" -eq 1 ]; then
      wall_fmt="$(format_elapsed "${wall_elapsed}")"
      sim_display="${sim_time}"
      if [ -z "${sim_display}" ] || [ "${sim_display}" = "NOT_FOUND" ]; then
        sim_display="NA"
      fi
      echo "[MATCH PROGRESS] pair_id=${pair_id} wall_elapsed=${wall_fmt} sim_time=${sim_display} half=${sim_half} play_mode=${sim_mode}"
      last_progress_wall="${wall_elapsed}"
      last_sim_time="${sim_time}"
      last_sim_half="${sim_half}"
      last_sim_mode="${sim_mode}"
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
robo_pid=""
server_pid=""
cleanup_done=0

agents_alive() {
  for pid in "${agent_pids[@]}"; do
    if ! kill -0 "${pid}" 2>/dev/null; then
      return 1
    fi
  done
  return 0
}

cleanup_match() {
  set +e
  for pid in "${agent_pids[@]}"; do
    terminate_pid "$pid"
  done
  terminate_pid "$launcher_pid"
  terminate_pid "$robo_pid"
  terminate_pid "$server_pid"

  sleep 1

  for pid in "${agent_pids[@]}"; do
    kill_pid "$pid"
  done
  kill_pid "$launcher_pid"
  kill_pid "$robo_pid"
  kill_pid "$server_pid"
  if [ -n "${launcher_pid}" ]; then
    for _ in $(seq 1 20); do
      if ! kill -0 "${launcher_pid}" 2>/dev/null; then
        break
      fi
      sleep 0.1
    done
  fi
  if [ -n "${robo_pid}" ]; then
    for _ in $(seq 1 20); do
      if ! kill -0 "${robo_pid}" 2>/dev/null; then
        break
      fi
      sleep 0.1
    done
  fi
  if [[ "${BENCH_KILL_STALE:-0}" == "1" ]] && command -v pkill >/dev/null 2>&1; then
    pkill -TERM -f "RoboViz" 2>/dev/null || true
    pkill -TERM -f "rcssserver3d" 2>/dev/null || true
    sleep 0.5
    pkill -KILL -f "RoboViz" 2>/dev/null || true
    pkill -KILL -f "rcssserver3d" 2>/dev/null || true
  fi
  launcher_log=""
  robo_pid=""
  server_pid=""
}

cleanup_all() {
  if [ "$cleanup_done" -eq 1 ]; then
    return
  fi
  cleanup_done=1
  trap - EXIT INT TERM
  if [ "${current_row_written}" -eq 0 ] && [ -n "${current_pair_id}" ] && [ -f "${results_csv}" ]; then
    timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
    echo "${current_match_id},${current_pair_id},${current_left},${current_right},NA,NA,${timestamp},error,unknown" >> "${results_csv}"
    current_row_written=1
    current_pair_id=""
  fi
  if [ -n "${bench_pgid}" ]; then
    kill -TERM -- "-${bench_pgid}" 2>/dev/null || true
    sleep 1
    kill -KILL -- "-${bench_pgid}" 2>/dev/null || true
  else
    cleanup_match
  fi
}

trap cleanup_all EXIT
trap 'cleanup_all; exit 130' INT TERM

kill_stale_processes() {
  if [[ "${BENCH_KILL_STALE:-1}" != "1" ]]; then
    return
  fi
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
  robo_pid=""
  server_pid=""
  start_cmd bash -c "env ROBOVIZ_DISABLE=\"${roboviz_disable}\" \"${launcher}\" >\"${launcher_log}\" 2>&1"
  launcher_pid=$last_pid
  if ! wait_for_port 3200; then
    echo "[BENCH] warning: monitor port 3200 not detected; proceeding"
  else
    echo "[BENCH] monitor port 3200 open"
  fi
  if [ -f "${launcher_log}" ]; then
    local line
    for _ in $(seq 1 20); do
      line="$(grep -m1 -E 'RoboViz PID:' "${launcher_log}" || true)"
      if [ -n "${line}" ]; then
        robo_pid="$(echo "${line}" | sed -n 's/.*RoboViz PID: \([0-9]\+\) | Server PID: \([0-9]\+\).*/\1/p')"
        server_pid="$(echo "${line}" | sed -n 's/.*RoboViz PID: \([0-9]\+\) | Server PID: \([0-9]\+\).*/\2/p')"
        break
      fi
      sleep 0.2
    done
  fi
}

ensure_cpp_modules() {
  if [ "${cpp_checked}" -eq 1 ]; then
    return
  fi
  cpp_checked=1
  echo "[BENCH] checking C++ modules (one-time)..."
  (cd "${fcp_dir}" && source "${venv_activate}" && python - <<'PY'
from scripts.commons.Script import Script
Script.build_cpp_modules(exit_on_build=False)
PY
  )
}

kill_stale_processes

mkdir -p "${log_dir}"
mkdir -p "${roboviz_log_dir}"
expected_header="match_id,pair_id,left_team,right_team,left_goals,right_goals,timestamp,status,error_reason"
if [[ ! -f "${results_csv}" ]]; then
  echo "${expected_header}" > "${results_csv}"
else
  current_header="$(head -n 1 "${results_csv}")"
  if [[ "${current_header}" != "${expected_header}" ]]; then
    tmp_csv="$(mktemp)"
    echo "${expected_header}" > "${tmp_csv}"
    declare -A pair_counts=()
    while IFS=, read -r pair_id left right left_goals right_goals timestamp status error_reason rest; do
      [[ -z "${pair_id}" || "${pair_id}" == "pair_id" ]] && continue
      count="${pair_counts[$pair_id]:-0}"
      count=$((count + 1))
      pair_counts["$pair_id"]="${count}"
      match_id="${pair_id}#${count}"
      if [[ -z "${error_reason}" ]]; then
        error_reason="unknown"
      fi
      echo "${match_id},${pair_id},${left},${right},${left_goals},${right_goals},${timestamp},${status},${error_reason}" >> "${tmp_csv}"
    done < <(tail -n +2 "${results_csv}")
    mv "${tmp_csv}" "${results_csv}"
  fi
fi

base_left="BASIC"
opponents=(NOISE DEFLOCK HIPRESS DIRECT AGGRO)
if [[ -n "${pairs_override}" ]]; then
  IFS=',' read -r pair_left pair_right pair_extra <<< "${pairs_override}"
  if [[ -z "${pair_left}" || -z "${pair_right}" || -n "${pair_extra}" ]]; then
    echo "Invalid --pairs '${pairs_override}'. Expected 'STRAT_A,STRAT_B'."
    exit 1
  fi
  base_left="${pair_left}"
  opponents=("${pair_right}")
fi
player_unums=(1 2 3 4)
if [[ -n "${unums_override}" ]]; then
  IFS=',' read -r -a player_unums <<< "${unums_override}"
  if [[ "${#player_unums[@]}" -eq 0 ]]; then
    echo "Invalid --unums '${unums_override}'. Expected 'N1,N2,...'."
    exit 1
  fi
  declare -A _unums_seen=()
  for u in "${player_unums[@]}"; do
    if [[ ! "${u}" =~ ^[0-9]+$ ]] || [ "${u}" -lt 1 ] || [ "${u}" -gt 11 ]; then
      echo "Invalid --unums '${unums_override}'. Uniforms must be 1-11."
      exit 1
    fi
    if [[ -n "${_unums_seen[$u]:-}" ]]; then
      echo "Invalid --unums '${unums_override}'. Duplicate uniform ${u}."
      exit 1
    fi
    _unums_seen["$u"]=1
  done
fi
match_total=$(( ${#opponents[@]} * 2 * match_reps ))

declare -A expected_match_ids=()
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
      pair_id="${left}_${right}"
      match_id="${pair_id}#${rep}"
      expected_match_ids["${match_id}"]=1
    done
  done
done

declare -A seen_match_ids=()
duplicate_count=0
unknown_count=0
if [[ -f "${results_csv}" ]]; then
  while IFS=, read -r match_id pair_id _; do
    [[ -z "${match_id}" || "${match_id}" == "match_id" ]] && continue
    if [[ -n "${seen_match_ids[$match_id]:-}" ]]; then
      duplicate_count=$((duplicate_count + 1))
    else
      seen_match_ids["${match_id}"]=1
    fi
    if [[ -z "${expected_match_ids[$match_id]:-}" ]]; then
      unknown_count=$((unknown_count + 1))
    fi
  done < "${results_csv}"
fi
missing_count=0
for match_id in "${!expected_match_ids[@]}"; do
  if [[ -z "${seen_match_ids[$match_id]:-}" ]]; then
    missing_count=$((missing_count + 1))
  fi
done
if [ "${duplicate_count}" -gt 0 ]; then
  echo "[RESUME] warning: ${duplicate_count} duplicate match_id entries found"
fi
if [ "${unknown_count}" -gt 0 ]; then
  echo "[RESUME] warning: ${unknown_count} unexpected match_id entries found"
fi
if [ "${missing_count}" -gt 0 ] && [ "${#seen_match_ids[@]}" -gt 0 ]; then
  echo "[RESUME] warning: ${missing_count} matches missing from CSV; will resume selectively"
fi
match_index=0
bench_pgid="$(ps -o pgid= $$ | tr -d ' ')"

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
      match_id="${pair_id}#${rep}"
      if [[ -n "${seen_match_ids[$match_id]:-}" ]]; then
        continue
      fi
      current_pair_id="${pair_id}"
      current_left="${left}"
      current_right="${right}"
      current_match_id="${match_id}"
      current_row_written=0
      match_start_epoch="$(date +%s)"
      echo "[SIM RESET] restarting server+roboviz for next match"
      echo "[MATCH START] pair_id=${pair_id} index=${match_index}/${match_total}"

      cleanup_match
      if ! wait_for_port_close 3200; then
        echo "[BENCH] warning: monitor port still open after cleanup"
      fi
      kill_stale_processes
      ensure_cpp_modules
      start_launcher

      agent_pids=()
      for u in "${player_unums[@]}"; do
        start_cmd bash -c "cd \"${fcp_dir}\" && source \"${venv_activate}\" && python Run_Player.py -t ${left} -u ${u} --strategy ${left}"
        agent_pids+=("$last_pid")
        sleep 0.3
      done
      sleep 0.5
      for u in "${player_unums[@]}"; do
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

      error_reason=""
      if [ -z "${log_file}" ]; then
        status="error"
        error_reason="roboviz_dead"
        echo "[BENCH] warning: logfile not detected; marking error"
      else
        echo "[LOG] pair_id=${pair_id} logfile=${log_file}"
        sleep 1
        if ! send_trainer_cmd "drop_ball" "(dropBall)"; then
          status="error"
          error_reason="server_dead"
        else
          status="ok"
        fi
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
            status="error"
            error_reason="parser_fail"
          fi
        else
          wait_status=$?
          if [ "${wait_status}" -eq 1 ]; then
            status="error"
            error_reason="server_dead"
          elif [ "${wait_status}" -eq 2 ]; then
            status="timeout"
            error_reason="timeout"
          elif [ "${wait_status}" -eq 3 ]; then
            status="timeout"
            error_reason="timeout"
          elif [ "${wait_status}" -eq 4 ]; then
            status="error"
            error_reason="agent_dead"
          else
            status="error"
            error_reason="unknown"
          fi
        fi
      fi
      if [[ "${status}" == "ok" ]]; then
        error_reason=""
      elif [[ -z "${error_reason}" ]]; then
        error_reason="unknown"
      fi
      timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
      echo "${match_id},${pair_id},${left},${right},${left_goals},${right_goals},${timestamp},${status},${error_reason}" >> "${results_csv}"
      current_row_written=1
      current_pair_id=""
      current_match_id=""
      echo "[MATCH END] pair_id=${pair_id}"

      cleanup_match
      sleep 2
    done
  done
done
