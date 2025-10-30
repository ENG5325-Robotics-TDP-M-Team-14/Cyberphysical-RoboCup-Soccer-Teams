from pathlib import Path
import re
from typing import Optional


class MatchData:

    def __init__(self) -> None:
        self.timestamp:         Optional[str] = None
        self.left_teamname:     Optional[str] = None
        self.left_finalscore:   Optional[int] = None
        self.right_teamname:    Optional[str] = None
        self.right_finalscore:  Optional[int] = None
    
    def __repr__(self) -> str:
        return (
            "MatchData: {timestamp: %s, left_teamname: %s, left_finalscore: %s, right_teamname: %s, right_finalscore: %s}"
            % (self.timestamp, self.left_teamname, self.left_finalscore, self.right_teamname, self.right_finalscore)
        )
    
    @staticmethod
    def from_filepath(filepath: Path) -> 'MatchData':
        filename = filepath.name
        data = MatchData()
        try:
            # Filename format is <timestamp>-<left teamname>_<left final score>-vs-<right final score>_teamname.match.csv...
            capture_results = re.match(rf'^(\d+)-([a-zA-z0-9_\-+]+)_(\d+)-vs-([a-zA-Z0-9_\-+]+)_(\d+)\.*', filename)
            data.timestamp = capture_results[1]
            data.left_teamname = capture_results[2]
            data.left_finalscore = int(capture_results[3])
            data.right_teamname = capture_results[4]
            data.right_finalscore = int(capture_results[5])
        except Exception as excpt:
            print(excpt)
            return ValueError(f"Failed to parse filename {filename}.")
        return data
