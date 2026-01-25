def press_threshold_to_margin(press_threshold: int) -> float:
    if press_threshold == 2:
        return 0.2
    if press_threshold == 3:
        return 0.5
    if press_threshold == 4:
        return 0.8
    raise ValueError("Unknown press_threshold. Valid values: 2, 3, 4")
