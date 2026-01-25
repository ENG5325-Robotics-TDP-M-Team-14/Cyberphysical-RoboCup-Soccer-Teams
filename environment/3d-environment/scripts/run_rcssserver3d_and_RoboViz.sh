#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ENV_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
ROBOVIZ_DISABLE="${ROBOVIZ_DISABLE:-0}"
SERVER_PORT=3200

# Filter only known, non-actionable startup noise from BOTH stdout and stderr.
NOISE_RE='OpenGLServer not found|TextureServer|no FPSController|\(sparkgui\.rb\)|redefining Object#method_missing|^\(MonitorServer\) WARNING: SimulationServer not found\.$'

ROBO_VIZ_DIR="$ENV_ROOT/RoboViz"
ROBO_VIZ_BIN="$ROBO_VIZ_DIR/bin/roboviz.sh"

cleanup() {
  echo "[INFO] Cleaning up..."
  # Terminate RoboViz first (GUI), then server
  if [[ -n "${ROBO_PID:-}" ]]; then
    kill -TERM "$ROBO_PID" 2>/dev/null || true
  fi
  if [[ -n "${SERVER_PID:-}" ]]; then
    kill -TERM "$SERVER_PID" 2>/dev/null || true
  fi
  # Give them a moment to exit gracefully
  sleep 0.5
  # Force kill if still running
  if [[ -n "${ROBO_PID:-}" ]]; then
    kill -KILL "$ROBO_PID" 2>/dev/null || true
  fi
  if [[ -n "${SERVER_PID:-}" ]]; then
    kill -KILL "$SERVER_PID" 2>/dev/null || true
  fi
}
trap cleanup INT TERM EXIT

echo "[INFO] Starting rcssserver3d..."
rcssserver3d \
  --script-path /usr/local/share/rcssserver3d/rcssserver3d.rb \
  > >(grep -v -E "$NOISE_RE") \
  2> >(grep -v -E "$NOISE_RE" >&2) &
SERVER_PID=$!

echo "[INFO] Waiting for server to open monitor port ${SERVER_PORT}..."
until ss -lnt | grep -q ":${SERVER_PORT}"; do
  sleep 0.5
done
echo "[INFO] Monitor port is open."

if [[ "${ROBOVIZ_DISABLE}" == "1" ]]; then
  echo "[INFO] RoboViz disabled. Server PID: $SERVER_PID"
  wait "$SERVER_PID"
  exit 0
fi

echo "[INFO] Starting RoboViz..."
(
  cd "$ROBO_VIZ_DIR"
  "$ROBO_VIZ_BIN" --serverPort="${SERVER_PORT}"
) &
ROBO_PID=$!

echo "[INFO] RoboViz PID: $ROBO_PID | Server PID: $SERVER_PID"
echo "[INFO] Close RoboViz window or press Ctrl+C to stop everything."

# When RoboViz exits (window closed), stop the server and exit.
wait "$ROBO_PID"
echo "[INFO] RoboViz exited. Stopping server..."
kill -TERM "$SERVER_PID" 2>/dev/null || true
wait "$SERVER_PID" 2>/dev/null || true
