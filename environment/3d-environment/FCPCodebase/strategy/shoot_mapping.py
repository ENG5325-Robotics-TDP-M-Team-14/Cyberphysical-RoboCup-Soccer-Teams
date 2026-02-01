def shoot_range_to_goal_shot_dist(shoot_range: float) -> float:
    """
    Map 2D shoot_range into a 3D goal-shot distance gate (ball-to-opponent-goal).

    IMPORTANT: BASIC/NOMINAL (25.0) is treated as the *legacy baseline*:
    - We do NOT gate goal shots for BASIC, to preserve pre-wiring behaviour.
    - Only conservative/aggressive strategies should change behaviour via gating.
    """
    if shoot_range == 20.0:   # CONSERVATIVE
        return 5.0
    if shoot_range == 25.0:   # NOMINAL / BASIC baseline (no gate)
        return float("inf")
    if shoot_range == 30.0:   # AGGRESSIVE
        return 7.0
    raise ValueError("Unknown shoot_range. Valid values: 20.0, 25.0, 30.0")