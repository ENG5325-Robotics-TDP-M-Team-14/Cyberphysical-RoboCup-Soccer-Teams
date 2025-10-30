import logging
import os
from pathlib import Path
from typing import Any, Dict, List, Tuple

def listcsvs(dataset_dirpath: Path) -> Tuple[List[Path], List[Path]]:
    """ 
        Transverses a dataset directory tree up until depth 1 and creates a list of paths to all 
        files marked as CSV tables (.csv or .csv.gz).

        Returns list with CSVs paths, list with compressed CSVs paths
    """
    dataset_contents = os.listdir(dataset_dirpath)
    csvpaths = []
    compressedcsvpaths = []
    for content in dataset_contents:
        content_path = dataset_dirpath / content
        if os.path.isdir(content_path):
            # Logs organized in folders
            match_dir = content_path
            csvpaths.extend([match_dir / filename for filename in os.listdir(match_dir) if filename.endswith('.csv')])
            compressedcsvpaths.extend([match_dir / filename for filename in os.listdir(match_dir) if filename.endswith('csv.gz')])
        elif content.endswith('.csv'):
            csvpaths.append(content_path)
        elif content.endswith('.csv.gz'):
            compressedcsvpaths.append(content_path)
    return csvpaths, compressedcsvpaths

class SessionLoggerAdapter(logging.LoggerAdapter):
    """ 
        Logger Adapter for training sessions
        Adds session identification to logs.
    """
    def __init__(self, logger: logging.Logger, extra: Dict[str, Any], session_number: int) -> None:
        super().__init__(logger, extra)
        self.session = session_number
    def process(self, msg: str, kwargs) -> Tuple[str, Dict[str, Any]]:
        return f'(At session #{self.session}) ' + str(msg), kwargs
