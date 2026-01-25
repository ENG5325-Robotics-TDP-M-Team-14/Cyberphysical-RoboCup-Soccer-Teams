#!/usr/bin/env python3
import argparse
import re
import sys


TOKEN_RE = re.compile(
    r"\((?P<tag>RuleHalfTime|play_modes|time|half|score_left|score_right|play_mode)\s+([^)]+)\)"
)


def _line_excerpt(buffer, start_idx, end_idx, max_len=200):
    line_start = buffer.rfind("\n", 0, start_idx)
    line_end = buffer.find("\n", end_idx)
    if line_start == -1:
        line_start = 0
    else:
        line_start += 1
    if line_end == -1:
        line_end = len(buffer)
    line = " ".join(buffer[line_start:line_end].strip().split())
    if len(line) > max_len:
        return line[:max_len] + "..."
    return line


def parse_log(path):
    rule_half_time = None
    play_modes = None
    start_time = None
    last_time = None
    last_half = None

    score_left = None
    score_right = None
    score_line_num = None
    score_line_text = None

    half_time_line_num = None
    half_time_line_text = None
    half_time_value = None

    goal_events = []
    goal_event_set = set()

    buffer = ""
    line_base = 1
    chunk_size = 1024 * 1024

    def add_goal_event(time_val, side, line_num):
        key = (time_val, side, line_num)
        if key in goal_event_set:
            return
        goal_event_set.add(key)
        goal_events.append({"time": time_val, "side": side, "line": line_num})

    with open(path, "r", errors="ignore") as f:
        while True:
            chunk = f.read(chunk_size)
            if not chunk:
                break
            buffer += chunk

            last_match_end = 0
            last_pos = 0
            line_cursor = line_base

            for match in TOKEN_RE.finditer(buffer):
                line_cursor += buffer[last_pos:match.start()].count("\n")
                line_num = line_cursor
                last_pos = match.start()
                last_match_end = match.end()

                tag = match.group("tag")
                value = match.group(2).strip()
                excerpt = _line_excerpt(buffer, match.start(), match.end())

                if tag == "RuleHalfTime" and rule_half_time is None:
                    try:
                        rule_half_time = float(value)
                    except ValueError:
                        pass
                elif tag == "play_modes" and play_modes is None:
                    play_modes = value.split()
                elif tag == "time":
                    try:
                        last_time = float(value)
                        if start_time is None:
                            start_time = last_time
                    except ValueError:
                        pass
                elif tag == "half":
                    try:
                        last_half = int(value)
                        if half_time_line_num is None and last_half == 2:
                            half_time_line_num = line_num
                            half_time_line_text = excerpt
                            half_time_value = last_time
                    except ValueError:
                        pass
                elif tag == "score_left":
                    try:
                        new_left = int(value)
                        if score_left is not None and new_left > score_left:
                            add_goal_event(last_time, "Left", line_num)
                        score_left = new_left
                        score_line_num = line_num
                        score_line_text = excerpt
                    except ValueError:
                        pass
                elif tag == "score_right":
                    try:
                        new_right = int(value)
                        if score_right is not None and new_right > score_right:
                            add_goal_event(last_time, "Right", line_num)
                        score_right = new_right
                        score_line_num = line_num
                        score_line_text = excerpt
                    except ValueError:
                        pass
                elif tag == "play_mode":
                    try:
                        idx = int(value)
                    except ValueError:
                        continue
                    if play_modes and 0 <= idx < len(play_modes):
                        mode = play_modes[idx]
                        if mode == "Goal_Left":
                            add_goal_event(last_time, "Left", line_num)
                        elif mode == "Goal_Right":
                            add_goal_event(last_time, "Right", line_num)

                if half_time_line_num is None and rule_half_time is not None:
                    if last_time is not None and last_time >= rule_half_time and last_half == 1:
                        half_time_line_num = line_num
                        half_time_line_text = excerpt
                        half_time_value = last_time

            if last_match_end > 0:
                dropped = buffer[:last_match_end]
                line_base += dropped.count("\n")
                buffer = buffer[last_match_end:]
            elif len(buffer) > 4096:
                drop = len(buffer) - 4096
                line_base += buffer[:drop].count("\n")
                buffer = buffer[drop:]

    return {
        "start_time": start_time,
        "half_time_value": half_time_value,
        "half_time_line_num": half_time_line_num,
        "half_time_line_text": half_time_line_text,
        "goal_events": goal_events,
        "score_left": score_left,
        "score_right": score_right,
        "score_line_num": score_line_num,
        "score_line_text": score_line_text,
    }


def main():
    parser = argparse.ArgumentParser(description="Parse a RoboViz logfile for match events.")
    parser.add_argument("logfile", help="Path to a RoboViz log file")
    args = parser.parse_args()

    data = parse_log(args.logfile)

    start_time = data["start_time"]
    half_time_value = data["half_time_value"]
    half_time_line_num = data["half_time_line_num"]
    half_time_line_text = data["half_time_line_text"]
    score_left = data["score_left"]
    score_right = data["score_right"]
    score_line_num = data["score_line_num"]
    score_line_text = data["score_line_text"]
    goal_events = data["goal_events"]

    print(f"start_time={start_time if start_time is not None else 'NOT_FOUND'}")
    if half_time_line_num is not None:
        print(f"half_time_line={half_time_line_num}:{half_time_line_text}")
        print(f"half_time_time={half_time_value}")
    else:
        print("half_time_line=NOT_FOUND")
        print("half_time_time=NOT_FOUND")

    print(f"goal_event_count={len(goal_events)}")
    for ev in goal_events:
        ev_time = ev["time"] if ev["time"] is not None else "UNKNOWN"
        print(f"goal_event={ev_time},{ev['side']}")

    if score_left is not None and score_right is not None:
        print(f"score_latest={score_left},{score_right}")
        print(f"score_line={score_line_num}:{score_line_text}")
    else:
        print("score_latest=NOT_FOUND")
        print("score_line=NOT_FOUND")

    summary_score = (
        f"{score_left},{score_right}" if score_left is not None and score_right is not None else "NOT_FOUND"
    )
    summary_half = half_time_value if half_time_line_num is not None else "NOT_FOUND"
    print(f"summary: start_time={start_time} half_time={summary_half} goals={len(goal_events)} score={summary_score}")

    if half_time_line_num is not None and score_left is not None and score_right is not None:
        return 0
    return 2


if __name__ == "__main__":
    sys.exit(main())
