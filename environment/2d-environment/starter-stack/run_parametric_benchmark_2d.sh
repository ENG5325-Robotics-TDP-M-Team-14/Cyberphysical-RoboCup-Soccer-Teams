#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: run_parametric_benchmark_2d.sh [options]

Options:
  --parameter PARAM        One of: press_threshold, shoot_range, formation
  --mode MODE              One of: 4v4, 2v2, 1v1 (default: 4v4)
  --repeats N              Replicates per side per level (default: 5)
  --levels CSV             Comma-separated levels for selected parameter
                           press_threshold/shoot_range: low,baseline,high
                           formation: baseline,def,off
  --out-csv PATH           Optional mirror path for final aggregated results CSV
  RCSSSERVER_PORT_BASE     Optional env var for the first benchmark server port
                           (default: 6100; later matches increment from there)
  RCSSSERVER_PORT_STRIDE   Optional env var for the per-match/per-retry port stride
                           (default: 10; keeps player/coach port groups from overlapping)
  -h, --help               Show this help
EOF
}

parameter="press_threshold"
mode="4v4"
repeats=5
levels_override=""
results_csv_mirror=""

MATCH_TIMEOUT_SECONDS="${MATCH_TIMEOUT_SECONDS:-300}"
START_DELAY="${START_DELAY:-2}"
SIDE_DELAY="${SIDE_DELAY:-2}"
HALF_TIME_CYCLES="${HALF_TIME_CYCLES:-150}"
RCSSSERVER_PORT_BASE="${RCSSSERVER_PORT_BASE:-6100}"
RCSSSERVER_PORT_RETRIES="${RCSSSERVER_PORT_RETRIES:-5}"
RCSSSERVER_PORT_STRIDE="${RCSSSERVER_PORT_STRIDE:-10}"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --parameter)
      parameter="$2"
      shift 2
      ;;
    --mode)
      mode="$2"
      shift 2
      ;;
    --repeats)
      repeats="$2"
      shift 2
      ;;
    --levels)
      levels_override="$2"
      shift 2
      ;;
    --out-csv)
      results_csv_mirror="$2"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown argument: $1" >&2
      usage
      exit 1
      ;;
  esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ENV2D_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
AGENT_DIR="${SCRIPT_DIR}/Agent/src"
COMPAT_HELPER="${SCRIPT_DIR}/link_starteragent2d_v2_compat_2d.sh"
SIMULATOR_ID="2d"
BENCHMARK_KIND="parametric"
BASELINE_CONTROLLER="BASIC"

resolve_rcssserver_bin() {
  local candidate
  local status
  for candidate in \
    "${ENV2D_DIR}/rcssserver-19.0.0/build-linux/rcssserver" \
    "${ENV2D_DIR}/rcssserver-19.0.0/build/rcssserver"
  do
    if [[ ! -x "${candidate}" ]]; then
      continue
    fi

    status=0
    "${candidate}" --help >/dev/null 2>&1 || status=$?
    if [[ "${status}" -ne 126 && "${status}" -ne 127 ]]; then
      printf '%s\n' "${candidate}"
      return 0
    fi
  done

  return 1
}

RCSSSERVER_BIN="$(resolve_rcssserver_bin || true)"

agent_binary_runnable() {
  local path="$1"
  local status=0

  if [[ ! -x "${path}" ]]; then
    return 1
  fi

  "${path}" --help >/dev/null 2>&1 || status=$?
  [[ "${status}" -ne 126 && "${status}" -ne 127 ]]
}

require_agent_binary() {
  local name="$1"
  local path="${AGENT_DIR}/${name}"

  if agent_binary_runnable "${path}"; then
    return 0
  fi

  echo "${path} is missing or not runnable on this machine." >&2
  echo "Build StarterAgent2D-V2 and refresh starter-stack links with:" >&2
  echo "  ${COMPAT_HELPER} --force" >&2
  echo "See LINUX_SETUP.md for the supported 2D build path." >&2
  exit 1
}

if [[ ! -x "${RCSSSERVER_BIN}" ]]; then
  echo "rcssserver not found/executable in build or build-linux directories" >&2
  exit 1
fi
if [[ ! -x "${AGENT_DIR}/start-4players.sh" ]]; then
  echo "start-4players.sh not found at ${AGENT_DIR}/start-4players.sh" >&2
  exit 1
fi

require_agent_binary sample_player
require_agent_binary sample_coach

normalize_level() {
  printf '%s' "$1" | tr '[:upper:]' '[:lower:]'
}

level_to_team() {
  local level
  level="$(normalize_level "$1")"

  case "${parameter}" in
    press_threshold)
      case "${level}" in
        low) echo "PRESLOW" ;;
        baseline) echo "PRESBASE" ;;
        high) echo "PRESHIGH" ;;
        *) return 1 ;;
      esac
      ;;
    shoot_range)
      case "${level}" in
        low) echo "SHOTLOW" ;;
        baseline) echo "SHOTBASE" ;;
        high) echo "SHOTHIGH" ;;
        *) return 1 ;;
      esac
      ;;
    formation)
      case "${level}" in
        baseline) echo "FORMBASE" ;;
        def|def_121) echo "FORMDEF" ;;
        off|off_112) echo "FORMOFF" ;;
        *) return 1 ;;
      esac
      ;;
    *)
      return 1
      ;;
  esac
}

level_to_value() {
  local level
  level="$(normalize_level "$1")"

  case "${parameter}" in
    press_threshold)
      case "${level}" in
        low) echo "2" ;;
        baseline) echo "3" ;;
        high) echo "4" ;;
        *) return 1 ;;
      esac
      ;;
    shoot_range)
      case "${level}" in
        low) echo "20.0" ;;
        baseline) echo "25.0" ;;
        high) echo "30.0" ;;
        *) return 1 ;;
      esac
      ;;
    formation)
      case "${level}" in
        baseline) echo "BASELINE" ;;
        def|def_121) echo "DEF_121" ;;
        off|off_112) echo "OFF_112" ;;
        *) return 1 ;;
      esac
      ;;
    *)
      return 1
      ;;
  esac
}

next_run_index() {
  local root="$1"
  local max_idx=0
  local d
  mkdir -p "${root}"
  for d in "${root}"/run_*; do
    [[ -d "${d}" ]] || continue
    local bn idx
    bn="$(basename "${d}")"
    idx="${bn#run_}"
    if [[ "${idx}" =~ ^[0-9]{4}$ ]]; then
      if (( 10#${idx} > max_idx )); then
        max_idx=$((10#${idx}))
      fi
    fi
  done
  printf "%04d" $((max_idx + 1))
}

case "${parameter}" in
  press_threshold|shoot_range|formation)
    ;;
  *)
    echo "Invalid --parameter '${parameter}' (expected: press_threshold, shoot_range, formation)" >&2
    exit 1
    ;;
esac

declare -a levels=()
if [[ -n "${levels_override}" ]]; then
  IFS=',' read -r -a levels <<< "${levels_override}"
else
  case "${parameter}" in
    press_threshold|shoot_range)
      levels=(low baseline high)
      ;;
    formation)
      levels=(baseline def off)
      ;;
  esac
fi

for lvl in "${levels[@]}"; do
  if ! level_to_team "${lvl}" >/dev/null; then
    echo "Invalid level '${lvl}' for parameter '${parameter}'" >&2
    exit 1
  fi
  if ! level_to_value "${lvl}" >/dev/null; then
    echo "Could not map level '${lvl}' to concrete value for parameter '${parameter}'" >&2
    exit 1
  fi
done

declare -a TEAM_ARGS=()
case "${mode}" in
  4v4)
    TEAM_ARGS=(-n 4)
    ;;
  2v2)
    # 2v2 uses goalkeeper + one field player (player 2) per side.
    TEAM_ARGS=(-n 2)
    ;;
  1v1)
    # 1v1 is a no-goalie duel using only field player 2 per side.
    TEAM_ARGS=(-n 2 -u 2 -C)
    ;;
  *)
    echo "Invalid --mode '${mode}' (expected: 4v4, 2v2 or 1v1)" >&2
    exit 1
    ;;
esac

OUTPUT_ROOT="${SCRIPT_DIR}/benchmark_outputs/${SIMULATOR_ID}/${BENCHMARK_KIND}/mode_${mode}/baseline_${BASELINE_CONTROLLER}/parameter_${parameter}"
RUN_INDEX="$(next_run_index "${OUTPUT_ROOT}")"
RUN_ID="run_${RUN_INDEX}"
RUN_DIR="${OUTPUT_ROOT}/${RUN_ID}"
MATCH_LOG_ROOT="${RUN_DIR}/match_logs"
SERVER_LOG_DIR="${RUN_DIR}/server_logs"
RESULTS_CSV="${RUN_DIR}/results.csv"
LEVELS_CSV="${RUN_DIR}/levels.csv"
RUN_METADATA_JSON="${RUN_DIR}/run_metadata.json"
METRICS_CATALOG_CSV="${RUN_DIR}/metrics_catalog.csv"
BEHAVIOURAL_SCAFFOLD_CSV="${RUN_DIR}/behavioural_metrics_scaffold.csv"

mkdir -p "${RUN_DIR}" "${MATCH_LOG_ROOT}" "${SERVER_LOG_DIR}"

echo "parameter_level,parameter_value,variant_controller" > "${LEVELS_CSV}"
for lvl in "${levels[@]}"; do
  normalized_level="$(normalize_level "${lvl}")"
  echo "${normalized_level},$(level_to_value "${normalized_level}"),$(level_to_team "${normalized_level}")" >> "${LEVELS_CSV}"
done

cat > "${METRICS_CATALOG_CSV}" <<'EOF'
metric_name,status,scope,source,notes
left_goals,available,match,rcssserver text log score line,Final left score from server log parsing
right_goals,available,match,rcssserver text log score line,Final right score from server log parsing
match_status,available,match,parametric runner,ok/timeout/error
error_reason,available,match,parametric runner,timeout/parse_fail/empty
timestamp_utc,available,match,parametric runner,Row timestamp in UTC
wall_time_sec,available,match,parametric runner,Wall-clock duration measured in shell
shot_distance_m,scaffolded,match,placeholder,Requires event-level kick telemetry
shot_location_x,scaffolded,match,placeholder,Requires event-level kick telemetry
shot_location_y,scaffolded,match,placeholder,Requires event-level kick telemetry
press_initiation_distance_m,scaffolded,match,placeholder,Requires press trigger instrumentation
time_to_shot_s,scaffolded,match,placeholder,Requires shot event timing instrumentation
time_to_engagement_s,scaffolded,match,placeholder,Requires engagement event timing instrumentation
spatial_occupancy_json,scaffolded,match,placeholder,Requires positional time-series logs
trajectory_path_length_m,scaffolded,match,placeholder,Requires positional time-series logs
trajectory_curvature_mean,scaffolded,match,placeholder,Requires positional time-series logs
EOF

echo "run_id,benchmark_type,simulator,mode,baseline_controller,parameter_name,parameter_level,parameter_value,match_id,pair_id,left_team,right_team,left_goals,right_goals,timestamp,status,error_reason,wall_time_sec" > "${RESULTS_CSV}"
echo "run_id,match_id,simulator,mode,baseline_controller,parameter_name,parameter_level,parameter_value,shot_distance_m,shot_location_x,shot_location_y,press_initiation_distance_m,time_to_shot_s,time_to_engagement_s,spatial_occupancy_json,trajectory_path_length_m,trajectory_curvature_mean,availability_shot_distance,availability_press_distance,availability_time_to_shot,availability_time_to_engagement,availability_spatial_occupancy,availability_trajectory,notes" > "${BEHAVIOURAL_SCAFFOLD_CSV}"

export RUN_METADATA_JSON LEVELS_CSV RUN_ID BENCHMARK_KIND SIMULATOR_ID mode BASELINE_CONTROLLER parameter OUTPUT_ROOT RUN_DIR RCSSSERVER_PORT_BASE RCSSSERVER_PORT_STRIDE
python3 - <<'PY'
import csv
import json
import os
from datetime import datetime, timezone

levels = []
with open(os.environ["LEVELS_CSV"], newline="", encoding="utf-8") as f:
    for row in csv.DictReader(f):
        levels.append(row)

metadata = {
    "run_id": os.environ["RUN_ID"],
    "benchmark_type": os.environ["BENCHMARK_KIND"],
    "simulator": os.environ["SIMULATOR_ID"],
    "mode": os.environ["mode"],
    "baseline_controller": os.environ["BASELINE_CONTROLLER"],
    "parameter_name": os.environ["parameter"],
    "levels": levels,
    "output_root": os.environ["OUTPUT_ROOT"],
    "run_dir": os.environ["RUN_DIR"],
    "server_port_base": int(os.environ["RCSSSERVER_PORT_BASE"]),
    "server_port_stride": int(os.environ["RCSSSERVER_PORT_STRIDE"]),
    "created_at_utc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
    "notes": {
        "behavioural_metrics": "Scaffolded placeholders are emitted; requested behavioural metrics are not computed from current logs yet."
    },
}

with open(os.environ["RUN_METADATA_JSON"], "w", encoding="utf-8") as f:
    json.dump(metadata, f, indent=2, sort_keys=True)
PY

SERVER_PID=""
MATCH_INDEX=0

cleanup_match() {
  if [[ -n "${SERVER_PID}" ]]; then
    kill -INT "${SERVER_PID}" >/dev/null 2>&1 || true
    wait "${SERVER_PID}" >/dev/null 2>&1 || true
    SERVER_PID=""
  fi
  killall sample_player sample_coach >/dev/null 2>&1 || true
}

cleanup() {
  cleanup_match
}
trap cleanup EXIT INT TERM

server_error_reason() {
  local server_log="$1"
  if grep -Eq "cannot execute binary file|Exec format error" "${server_log}"; then
    printf '%s' "server_binary_incompatible"
    return 0
  fi
  if grep -q "Error initializing sockets" "${server_log}"; then
    printf '%s' "server_bind_failed"
    return 0
  fi
  if grep -q "Waiting for players to connect" "${server_log}" && ! grep -q "Kick_off_" "${server_log}"; then
    printf '%s' "players_failed_to_connect"
    return 0
  fi
  if [[ ! -s "${server_log}" ]]; then
    printf '%s' "empty_server_log"
    return 0
  fi
  printf '%s' "parse_fail"
}

pid_is_active() {
  local pid="$1"
  local stat=""
  if ! kill -0 "${pid}" >/dev/null 2>&1; then
    return 1
  fi

  stat="$(ps -o stat= -p "${pid}" 2>/dev/null | awk 'NR==1 {print $1}')"
  if [[ -z "${stat}" || "${stat}" == Z* ]]; then
    return 1
  fi

  return 0
}

run_match() {
  local level="$1"
  local parameter_value="$2"
  local left_team="$3"
  local right_team="$4"
  local side_idx="$5"
  local rep_idx="$6"
  local match_id="${parameter}_${level}_${mode}_s${side_idx}_r${rep_idx}"
  local pair_id="${left_team}_${right_team}"
  local match_log_dir="${MATCH_LOG_ROOT}/${match_id}"
  local server_log="${SERVER_LOG_DIR}/${match_id}.server.log"
  local server_port=""
  local coach_port=""
  local left_goals="NA"
  local right_goals="NA"
  local status="ok"
  local error_reason=""
  local hit_wall_timeout=0
  local wall_start wall_end wall_time_sec timestamp
  local startup_try startup_port

  MATCH_INDEX=$((MATCH_INDEX + 1))
  mkdir -p "${match_log_dir}"
  cleanup_match

  wall_start="$(date +%s)"
  echo "START ${match_id}: ${left_team} (L) vs ${right_team} (R) @ $(date -u +"%Y-%m-%dT%H:%M:%SZ")"

  for startup_try in $(seq 0 $((RCSSSERVER_PORT_RETRIES - 1))); do
    startup_port=$((RCSSSERVER_PORT_BASE + ((MATCH_INDEX - 1) * RCSSSERVER_PORT_STRIDE) + (startup_try * RCSSSERVER_PORT_STRIDE)))
    coach_port=$((startup_port + 2))
    : > "${server_log}"
    "${RCSSSERVER_BIN}" \
      server::auto_mode=on \
      server::synch_mode=off \
      server::slow_down_factor=1 \
      server::port="${startup_port}" \
      server::olcoach_port="$((startup_port + 1))" \
      server::coach_port="${coach_port}" \
      server::half_time="${HALF_TIME_CYCLES}" \
      server::game_log_dir="${match_log_dir}" \
      server::text_log_dir="${match_log_dir}" \
      > "${server_log}" 2>&1 &
    SERVER_PID=$!

    sleep "${START_DELAY}"
    if kill -0 "${SERVER_PID}" >/dev/null 2>&1; then
      server_port="${startup_port}"
      break
    fi

    wait "${SERVER_PID}" >/dev/null 2>&1 || true
    SERVER_PID=""
    if ! grep -q "Error initializing sockets" "${server_log}"; then
      break
    fi
    echo "WARN ${match_id}: benchmark port ${startup_port} unavailable, retrying" >&2
  done

  if [[ -z "${server_port}" ]]; then
    status="error"
    error_reason="$(server_error_reason "${server_log}")"
    wall_end="$(date +%s)"
    wall_time_sec=$((wall_end - wall_start))
    timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
    echo "${RUN_ID},${BENCHMARK_KIND},${SIMULATOR_ID},${mode},${BASELINE_CONTROLLER},${parameter},${level},${parameter_value},${match_id},${pair_id},${left_team},${right_team},${left_goals},${right_goals},${timestamp},${status},${error_reason},${wall_time_sec}" >> "${RESULTS_CSV}"
    echo "${RUN_ID},${match_id},${SIMULATOR_ID},${mode},${BASELINE_CONTROLLER},${parameter},${level},${parameter_value},,,,,,,,,,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,requires_event_level_telemetry_for_shots_press_and_trajectories" >> "${BEHAVIOURAL_SCAFFOLD_CSV}"
    echo "END   ${match_id}: ${left_team} ${left_goals} - ${right_goals} ${right_team} status=${status} error=${error_reason} wall=${wall_time_sec}s"
    return
  fi

  "${AGENT_DIR}/start-4players.sh" -t "${left_team}" -p "${server_port}" "${TEAM_ARGS[@]}" >/dev/null 2>&1
  sleep "${SIDE_DELAY}"
  "${AGENT_DIR}/start-4players.sh" -t "${right_team}" -p "${server_port}" "${TEAM_ARGS[@]}" >/dev/null 2>&1

  local wait_start
  wait_start="$(date +%s)"
  while pid_is_active "${SERVER_PID}"; do
    if (( $(date +%s) - wait_start > MATCH_TIMEOUT_SECONDS )); then
      hit_wall_timeout=1
      kill -INT "${SERVER_PID}" >/dev/null 2>&1 || true
      break
    fi
    sleep 1
  done
  wait "${SERVER_PID}" >/dev/null 2>&1 || true
  SERVER_PID=""

  wall_end="$(date +%s)"
  wall_time_sec=$((wall_end - wall_start))

  local score_line
  score_line="$(awk '/Score:/{print $2","$4}' "${server_log}" | tail -n 1)"
  if [[ -n "${score_line}" ]]; then
    left_goals="${score_line%,*}"
    right_goals="${score_line#*,}"
    status="ok"
    error_reason=""
  elif [[ "${hit_wall_timeout}" -eq 1 ]]; then
    status="error"
    error_reason="timeout"
  elif [[ "${status}" == "ok" ]]; then
    status="error"
    error_reason="$(server_error_reason "${server_log}")"
  fi

  timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
  echo "${RUN_ID},${BENCHMARK_KIND},${SIMULATOR_ID},${mode},${BASELINE_CONTROLLER},${parameter},${level},${parameter_value},${match_id},${pair_id},${left_team},${right_team},${left_goals},${right_goals},${timestamp},${status},${error_reason},${wall_time_sec}" >> "${RESULTS_CSV}"

  echo "${RUN_ID},${match_id},${SIMULATOR_ID},${mode},${BASELINE_CONTROLLER},${parameter},${level},${parameter_value},,,,,,,,,,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,not_available_from_current_logs,requires_event_level_telemetry_for_shots_press_and_trajectories" >> "${BEHAVIOURAL_SCAFFOLD_CSV}"

  echo "END   ${match_id}: ${left_team} ${left_goals} - ${right_goals} ${right_team} status=${status} port=${server_port} wall=${wall_time_sec}s"
}

for lvl in "${levels[@]}"; do
  normalized_level="$(normalize_level "${lvl}")"
  parameter_value="$(level_to_value "${normalized_level}")"
  variant_team="$(level_to_team "${normalized_level}")"

  for side in 0 1; do
    for rep in $(seq 1 "${repeats}"); do
      if [[ "${side}" -eq 0 ]]; then
        run_match "${normalized_level}" "${parameter_value}" "${BASELINE_CONTROLLER}" "${variant_team}" "${side}" "${rep}"
      else
        run_match "${normalized_level}" "${parameter_value}" "${variant_team}" "${BASELINE_CONTROLLER}" "${side}" "${rep}"
      fi
    done
  done
done

if [[ -n "${results_csv_mirror}" ]]; then
  mkdir -p "$(dirname "${results_csv_mirror}")"
  cp "${RESULTS_CSV}" "${results_csv_mirror}"
fi

echo "Parametric benchmark finished."
echo "Run directory: ${RUN_DIR}"
echo "Results CSV: ${RESULTS_CSV}"
echo "Run metadata: ${RUN_METADATA_JSON}"
if [[ -n "${results_csv_mirror}" ]]; then
  echo "Mirrored results CSV: ${results_csv_mirror}"
fi
