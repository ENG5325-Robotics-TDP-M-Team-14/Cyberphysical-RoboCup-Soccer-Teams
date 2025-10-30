from nubia import Nubia, Options
import sys

import tasks.v1.cli

if __name__ == "__main__":
    shell = Nubia(
        name="rcss2d_deep_imitation",
        command_pkgs=[tasks.v1.cli],
        options=Options(
            persistent_history=True, auto_execute_single_suggestions=False
        ),
    )
    sys.exit(shell.run())
