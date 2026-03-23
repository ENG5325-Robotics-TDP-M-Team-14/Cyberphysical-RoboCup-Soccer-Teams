#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
ENV2D_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)
AGENT_DIR="${SCRIPT_DIR}/Agent/src"
COMPAT_HELPER="${SCRIPT_DIR}/link_starteragent2d_v2_compat_2d.sh"
RESULTS_CSV="${SCRIPT_DIR}/strategy_benchmark_results_2d.csv"
LOG_DIR="${SCRIPT_DIR}/strategy_benchmark_logs_2d"

MATCH_TIMEOUT_SECONDS="${MATCH_TIMEOUT_SECONDS:-300}"
START_DELAY="${START_DELAY:-2}"
SIDE_DELAY="${SIDE_DELAY:-2}"
RCSSSERVER_PORT_BASE="${RCSSSERVER_PORT_BASE:-6000}"
RCSSSERVER_PORT_RETRIES="${RCSSSERVER_PORT_RETRIES:-5}"
RCSSSERVER_PORT_STRIDE="${RCSSSERVER_PORT_STRIDE:-10}"

SERVER_PID=""
MATCH_INDEX=0

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
  echo "See docs/setup/linux.md for the supported 2D build path." >&2
  exit 1
}

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

if [[ ! -x "${RCSSSERVER_BIN}" ]]; then
  echo "rcssserver not found/executable at ${RCSSSERVER_BIN}" >&2
  exit 1
fi

if [[ ! -x "${AGENT_DIR}/start-4players.sh" ]]; then
  echo "start-4players.sh not found at ${AGENT_DIR}/start-4players.sh" >&2
  exit 1
fi

require_agent_binary sample_player
require_agent_binary sample_coach

if [[ ! -f "${RESULTS_CSV}" ]]; then
  echo "pair_id,left_team,right_team,left_goals,right_goals,timestamp" > "${RESULTS_CSV}"
fi

if [[ ! -d "${LOG_DIR}" ]]; then
  mkdir -p "${LOG_DIR}"
fi

run_match() {
  local pair_id="$1"
  local left_team="$2"
  local right_team="$3"
  local side_idx="${4:-}"
  local rep_idx="${5:-}"
  local server_port=""
  local coach_port=""
  local startup_try
  local startup_port
  local meta=""
  if [[ -n "${side_idx}" && -n "${rep_idx}" ]]; then
    meta=" side=${side_idx} rep=${rep_idx}"
  fi
  local log_file
  log_file="$(mktemp)"

  MATCH_INDEX=$((MATCH_INDEX + 1))
  cleanup_match

  echo "START ${pair_id}: ${left_team} (L) vs ${right_team} (R)${meta} @ $(date -u +"%Y-%m-%dT%H:%M:%SZ")"

  for startup_try in $(seq 0 $((RCSSSERVER_PORT_RETRIES - 1))); do
    startup_port=$((RCSSSERVER_PORT_BASE + ((MATCH_INDEX - 1) * RCSSSERVER_PORT_STRIDE) + (startup_try * RCSSSERVER_PORT_STRIDE)))
    coach_port=$((startup_port + 2))
    : > "${log_file}"
    "${RCSSSERVER_BIN}" \
      server::auto_mode=on \
      server::synch_mode=off \
      server::slow_down_factor=1 \
      server::port="${startup_port}" \
      server::olcoach_port="$((startup_port + 1))" \
      server::coach_port="${coach_port}" \
      server::half_time=150 \
      server::game_log_dir="${LOG_DIR}" \
      server::text_log_dir="${LOG_DIR}" \
      > "${log_file}" 2>&1 &
    SERVER_PID=$!

    sleep "${START_DELAY}"
    if kill -0 "${SERVER_PID}" >/dev/null 2>&1; then
      server_port="${startup_port}"
      break
    fi

    wait "${SERVER_PID}" >/dev/null 2>&1 || true
    SERVER_PID=""
    if ! grep -q "Error initializing sockets" "${log_file}"; then
      break
    fi
    echo "WARN ${pair_id}${meta}: benchmark port ${startup_port} unavailable, retrying" >&2
  done

  if [[ -z "${server_port}" ]]; then
    echo "Failed to start rcssserver for ${pair_id}${meta}" >&2
    echo "Server log: ${log_file}" >&2
    exit 1
  fi

  "${AGENT_DIR}/start-4players.sh" -p "${server_port}" -P "${coach_port}" -t "${left_team}" >/dev/null 2>&1
  sleep "${SIDE_DELAY}"
  "${AGENT_DIR}/start-4players.sh" -p "${server_port}" -P "${coach_port}" -t "${right_team}" >/dev/null 2>&1

  local start_ts
  start_ts="$(date +%s)"
  while kill -0 "${SERVER_PID}" >/dev/null 2>&1; do
    if (( $(date +%s) - start_ts > MATCH_TIMEOUT_SECONDS )); then
      echo "Match timeout after ${MATCH_TIMEOUT_SECONDS}s, forcing shutdown" >&2
      kill -INT "${SERVER_PID}" >/dev/null 2>&1 || true
      break
    fi
    sleep 1
  done
  wait "${SERVER_PID}" >/dev/null 2>&1 || true
  SERVER_PID=""

  local score_line
  score_line="$(awk '/Score:/{print $2","$4}' "${log_file}" | tail -n 1)"
  if [[ -z "${score_line}" ]]; then
    echo "Failed to parse score from ${log_file}" >&2
    exit 1
  fi

  local left_goals
  local right_goals
  left_goals="${score_line%,*}"
  right_goals="${score_line#*,}"

  local timestamp
  timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
  echo "${pair_id},${left_team},${right_team},${left_goals},${right_goals},${timestamp}" >> "${RESULTS_CSV}"
  echo "END   ${pair_id}: ${left_team} ${left_goals} - ${right_goals} ${right_team}${meta} @ ${timestamp}"

  rm -f "${log_file}"
}

pairs=(
  "BASIC_NOISE:BASIC:NOISE"
  "BASIC_DEFLOCK:BASIC:DEFLOCK"
  "BASIC_HIPRESS:BASIC:HIPRESS"
  "BASIC_DIRECT:BASIC:DIRECT"
  "BASIC_AGGRO:BASIC:AGGRO"
)

for pair in "${pairs[@]}"; do
  pair_id="${pair%%:*}"
  rest="${pair#*:}"
  left="${rest%%:*}"
  right="${rest#*:}"

  for side in 0 1; do
    for rep in 1 2 3 4 5; do
      if [[ "${side}" -eq 0 ]]; then
        run_match "${pair_id}" "${left}" "${right}" "${side}" "${rep}"
      else
        run_match "${pair_id}" "${right}" "${left}" "${side}" "${rep}"
      fi
    done
  done
done
