def get_home_pos(formation_id: str, unum: int) -> tuple[float, float]:
    """
    Return the home position for a formation and uniform number.
    Positions are defined for unums 1-4; any unum > 4 reuses the last position.
    """
    formations = {
        "BASELINE": (
            (-14, 0),
            (-9, -5),
            (-9, 0),
            (-9, 5),
        ),
        "DEF_121": (
            (-14, 0),
            (-11, -4),
            (-8, 0),
            (-11, 4),
        ),
        "OFF_112": (
            (-14, 0),
            (-6, -4),
            (-9, 0),
            (-6, 4),
        ),
    }

    if formation_id not in formations:
        valid = ", ".join(sorted(formations.keys()))
        raise ValueError(f"Unknown formation_id '{formation_id}'. Valid ids: {valid}")

    positions = formations[formation_id]
    clamped_unum = max(1, min(unum, len(positions)))
    return positions[clamped_unum - 1]
