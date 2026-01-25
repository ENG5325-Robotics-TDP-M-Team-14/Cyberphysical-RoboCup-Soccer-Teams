#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"

env_root="${repo_root}/environment/3d-environment"
fcp_dir="${env_root}/FCPCodebase"
venv_activate="${fcp_dir}/.venv/bin/activate"
launcher="${env_root}/scripts/run_rcssserver3d_and_RoboViz.sh"
log_dir="${repo_root}/benchmark_logs_3d"
results_csv="${log_dir}/strategy_benchmark_results_3d.csv"

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
  python3 - "$payload" <<'PY'
import socket, struct, sys
host = "127.0.0.1"
port = 3200
msg = sys.argv[1]
data = msg.encode("ascii")
hdr = struct.pack("!I", len(data))
with socket.create_connection((host, port), timeout=2) as s:
    s.sendall(hdr + data)
PY
  echo "[SMOKE] trainer_sent=${name} raw=${payload}"
}

get_match_score() {
  python3 - <<'PY'
import re
import socket
import struct
import time

host = "127.0.0.1"
port = 3200
deadline = time.time() + 2.0
score_left = None
score_right = None

def read_exact(sock, n):
    data = b""
    while len(data) < n:
        chunk = sock.recv(n - len(data))
        if not chunk:
            return None
        data += chunk
    return data

try:
    with socket.create_connection((host, port), timeout=2) as s:
        s.settimeout(0.5)
        msg = b"(reqfullstate)"
        hdr = struct.pack("!I", len(msg))
        s.sendall(hdr + msg)
        while time.time() < deadline:
            hdr = read_exact(s, 4)
            if not hdr:
                break
            length = struct.unpack("!I", hdr)[0]
            if length <= 0:
                break
            body = read_exact(s, length)
            if not body:
                break
            text = body.decode("utf-8", errors="ignore")
            for m in re.finditer(r"\\(score_left\\s+(-?\\d+)\\)", text):
                score_left = int(m.group(2))
            for m in re.finditer(r"\\(score_right\\s+(-?\\d+)\\)", text):
                score_right = int(m.group(2))
            if score_left is not None and score_right is not None:
                break
except Exception:
    pass

if score_left is None or score_right is None:
    print("0,0")
else:
    print(f"{score_left},{score_right}")
PY
}

agent_pids=()
launcher_pid=""

cleanup() {
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
}

trap cleanup EXIT INT TERM

mkdir -p "${log_dir}"
if [[ ! -f "${results_csv}" ]]; then
  echo "pair_id,left_team,right_team,left_goals,right_goals,timestamp" > "${results_csv}"
fi

start_cmd "$launcher"
launcher_pid=$last_pid

if ! wait_for_port 3200; then
  echo "[SMOKE] warning: monitor port 3200 not detected; proceeding"
else
  echo "[SMOKE] monitor port 3200 open"
fi

strategies=(BASIC NOISE DEFLOCK HIPRESS DIRECT AGGRO)
match_total=$(( ${#strategies[@]} * ${#strategies[@]} ))
match_index=0

for left in "${strategies[@]}"; do
  for right in "${strategies[@]}"; do
    match_index=$((match_index + 1))
    pair_id="${left}_${right}"
    echo "[MATCH START] pair_id=${pair_id} index=${match_index}/${match_total}"

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

    sleep 1
    send_trainer_cmd "drop_ball" "(dropBall)"

    sleep 300
    score_line="$(get_match_score || true)"
    if [[ -z "${score_line}" ]]; then
      score_line="0,0"
    fi
    left_goals="${score_line%,*}"
    right_goals="${score_line#*,}"
    timestamp="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"
    echo "${pair_id},${left},${right},${left_goals},${right_goals},${timestamp}" >> "${results_csv}"
    echo "[MATCH END] pair_id=${pair_id}"

    for pid in "${agent_pids[@]}"; do
      terminate_pid "$pid"
    done
    sleep 1
    for pid in "${agent_pids[@]}"; do
      kill_pid "$pid"
    done
    agent_pids=()
  done
done
