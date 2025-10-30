from logging import LoggerAdapter
import time
from typing import Any, Dict, List
from types import FunctionType

def static_vars(**kwargs: Dict[str,Any]) -> FunctionType:
    """
        A simple decorator function to allow functions to have static variables.
    """
    def decorate(func: FunctionType) -> FunctionType:
        for k in kwargs:
            setattr(func, k, kwargs[k])
        return func
    return decorate

@static_vars(correlationID=0)
def profile(fn: FunctionType) -> FunctionType:
    """
        A simple decorator to time-profile end-to-end execution of functions.
        This enables any functions that receive a LoggerAdapter object in a 'logger' keyword arguments to be time-profiled.
    """
    def profiled_fn(*args: List[Any], **kwargs: Dict[str,Any]):
        if 'logger' not in kwargs:
            # If there's no logger available, skip profiling.
            return fn(*args, **kwargs)
        logger: LoggerAdapter = kwargs['logger']
        logger.info(f"Profile[function = {fn.__name__}][call={profile.correlationID}]: Start")
        start = time.time()
        result = fn(*args, **kwargs)
        logger.info(f"Profile[function={fn.__name__}][call={profile.correlationID}]: Finished. Elapsed {time.time()-start} sec")
        profile.correlationID += 1
        return result
    return profiled_fn
