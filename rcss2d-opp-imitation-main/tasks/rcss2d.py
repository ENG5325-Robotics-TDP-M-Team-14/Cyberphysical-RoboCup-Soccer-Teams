from abc import ABC, abstractmethod
from enum import Enum
import numpy as np
from typing import NamedTuple, Tuple

class FieldSide(Enum):
    """ Identifies a side of the field. """
    LEFT    = 'l'
    RIGHT   = 'r'

    def __str__(self) -> str:
        return self.value
    
    @staticmethod
    def from_str(side: str) -> 'FieldSide':
        side_lowercase = side.lower()
        if side_lowercase == 'l' or side_lowercase == 'left':
            return FieldSide.LEFT
        elif side_lowercase == 'r' or side_lowercase == 'right':
            return FieldSide.RIGHT
        raise ValueError(f'Specified field side {side} is invalid.')

class UniformNumber(Enum):
    """ Identifies a uniform number.
        I know it looks quite dumb but this is called strong-typing.
    """
    ONE     = '1'
    TWO     = '2'
    THREE   = '3'
    FOUR    = '4'
    FIVE    = '5'
    SIX     = '6'
    SEVEN   = '7'
    EIGHT   = '8'
    NINE    = '9'
    TEN     = '10'
    ELEVEN  = '11'

    def __str__(self) -> str:
        return self.value
    
    def __int__(self) -> int:
        return int(self.value)
    
    def __float__(self) -> float:
        return float(self.value)
    
    @staticmethod
    def from_int(number: int) -> 'UniformNumber':
        # I know the below looks stupid but this prevents memory allocations
        # Feel free to change it for a static memory allocation solution
        if number == 1:
            return UniformNumber.ONE
        elif number == 2:
            return UniformNumber.TWO
        elif number == 3:
            return UniformNumber.THREE
        elif number == 4:
            return UniformNumber.FOUR
        elif number == 5:
            return UniformNumber.FIVE
        elif number == 6:
            return UniformNumber.SIX
        elif number == 7:
            return UniformNumber.SEVEN
        elif number == 8:
            return UniformNumber.EIGHT
        elif number == 9:
            return UniformNumber.NINE
        elif number == 10:
            return UniformNumber.TEN
        elif number == 11:
            return UniformNumber.ELEVEN
        raise ValueError(f'Specified uniform number {number} is out of bounds.')

class RCSSServerParamsV16:
    """
        Stores the default server parameters of RCSS2D.
    """
    BALL_DECAY              = 0.97
    BALL_RAND               = 0.05
    BALL_SPEED_MAX          = 3
    DASH_POWER_RATE         = 0.006
    EFFORT_INIT             = 1.0   # Also known as Effort Max
    EFFORT_MIN              = 0.6
    EXTRA_STAMINA           = 50
    INERTIA_MOMENT          = 5.0
    KICK_RAND               = 0.1
    KICKABLE_MARGIN         = 0.7
    PLAYER_DECAY            = 0.4
    PLAYER_RAND             = 0.1
    PLAYER_SPEED_MAX        = 1.05
    PITCH_LENGTH            = 105
    PITCH_WIDTH             = 68
    STAMINA_INC_MAX         = 45
    STAMINA_MAX             = 8000
    STAMINA_CAPACITY        = 130600
    
    # NOTE: Many params missing, check out the server.conf rcssserver file for a complete list

class RCSSPlayerParamsV16:
    """
        Stores the default heteroplayer parameters of RCSS2D.
    """
    DASH_POWER_RATE_DELTA_MIN       = -0.0012
    DASH_POWER_RATE_DELTA_MAX       =  0.0008
    EFFORT_MAX_DELTA_FACTOR         = -0.004
    EFFORT_MIN_DELTA_FACTOR         = -0.004
    EXTRA_STAMINA_DELTA_MIN         = 0.0
    EXTRA_STAMINA_DELTA_MAX         = 50.0
    INERTIA_MOMENT_DELTA_FACTOR     = 25.0
    KICK_RAND_DELTA_FACTOR          = 1.0
    KICKABLE_MARGIN_DELTA_MIN       = -0.1
    KICKABLE_MARGIN_DELTA_MAX       =  0.1
    PLAYER_DECAY_DELTA_MIN          = -0.1
    PLAYER_DECAY_DELTA_MAX          =  0.1
    STAMINA_INC_MAX_DELTA_FACTOR    = -6000.0
    
    # NOTE: Many params missing, check out the player.conf rcssserver file for a complete list

class FeatureNormalizer(ABC):
    """ Base class for table column normalizers. """

    @abstractmethod
    def inplace(self, values: np.ndarray) -> None:
        """
            Normalizes a numpy array representing a table column in-place.
            
            :values: numpy array to be normalized in-place
        """
        raise NotImplementedError
    
    @abstractmethod
    def normalize(self, values: np.ndarray) -> np.ndarray:
        """
            Normalizes a numpy array representing a table column.
            
            :values: numpy array to be normalized.
            Returns numpy array with normalized values.
        """
        raise NotImplementedError

##
## NOTE: Normalizers below only operate with default rcssserver v16 parameters.
##
class XCoordNormalizerV16(FeatureNormalizer):
    """ Normalize global X coordinates.
        X coordinates can be any real number, but players typically stay in [-half_pitch_length, +half_pitch_length].
        We normalize coordinates to a tight [-1, 1] range, clipping away positioning off the field.
    """

    def inplace(self, values: np.ndarray) -> None:
        values /= (RCSSServerParamsV16.PITCH_LENGTH / 2)
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return (values / (RCSSServerParamsV16.PITCH_LENGTH / 2)).clip(-1, 1)

class YCoordNormalizerV16(FeatureNormalizer):
    """ Normalize global Y coordinates.
        Y coordinates can be any real number, but players typically stay in [-half_pitch_width, +half_pitch_width].
        We normalize coordinates to a tight [-1, 1] range, clipping away positioning off the field.
    """

    def inplace(self, values: np.ndarray) -> None:
        values /= (RCSSServerParamsV16.PITCH_WIDTH / 2)
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return (values / (RCSSServerParamsV16.PITCH_WIDTH / 2)).clip(-1, 1)

class PlayerSpeedNormalizerV16(FeatureNormalizer):
    """ Normalize global player velocity coordinate.
        Velocity coordinates are in [-player_speed_max, +player_speed_max].
        We normalize them to a tight [-1, 1] range.
    """

    def inplace(self, values: np.ndarray) -> None:
        values /= RCSSServerParamsV16.PLAYER_SPEED_MAX
        values.clip(-1, 1, out=values)

    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return (values / RCSSServerParamsV16.PLAYER_SPEED_MAX).clip(-1, 1)

class AngleDegreeNormalizerV16(FeatureNormalizer):
    """ Normalize angles in degrees.
        Angle is in [-180, +180].
        We normalize it to a tight [-1, 1] range.
    """
    def inplace(self, values: np.ndarray) -> None:
        values /= 180.0
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return (values / 180.0).clip(-1, 1)

class PlayerStaminaNormalizerV16(FeatureNormalizer):
    """ Normalize player stamina.
        Stamina is in the range [0, stamina_max].
        We normalize it to a tight [-1, 1] range.
    """

    def inplace(self, values: np.ndarray) -> None:
        values -= RCSSServerParamsV16.STAMINA_MAX / 2
        values /= RCSSServerParamsV16.STAMINA_MAX / 2
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return ((2*values - RCSSServerParamsV16.STAMINA_MAX) / RCSSServerParamsV16.STAMINA_MAX).clip(-1, 1)

class PlayerStaminaResNormalizerV16(FeatureNormalizer):
    """ Normalize stamina reserve.
        Stamina reserve is in the range [0, stamina_capacity].
        We normalize it to a tight [-1, 1] range.
    """

    def inplace(self, values: np.ndarray) -> None:
        values -= RCSSServerParamsV16.STAMINA_CAPACITY / 2
        values /= RCSSServerParamsV16.STAMINA_CAPACITY / 2
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return ((2*values - RCSSServerParamsV16.STAMINA_CAPACITY)/ RCSSServerParamsV16.STAMINA_CAPACITY).clip(-1, 1)

class BallSpeedNormalizerV16(FeatureNormalizer):
    """ Normalize global ball velocity coordinate.
        Velocity coordinates are in [-ball_speed_max, +ball_speed_max].
        We normalize them to a tight [-1, 1] range.
    """

    def inplace(self, values: np.ndarray) -> None:
        values /= RCSSServerParamsV16.BALL_SPEED_MAX
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return (values / RCSSServerParamsV16.BALL_SPEED_MAX).clip(-1, 1)

class CommandPowerNormalizerV16(FeatureNormalizer):
    """ Normalize command power parameter.
        Power parameters in commands are in [-100, 100].
        We normalize them to a tight [-1, 1].
    """

    def inplace(self, values: np.ndarray) -> None:
        values.clip(-100, 100, out=values)
        values /= 100
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        return values.clip(-100, 100) / 100

class DashPowerRateNormalizerV16(FeatureNormalizer):
    """ Normalize Dash Power Rate heterogeneous parameter.
        Dash Power Rate is in [default_dash_power_rate + new_dash_power_rate_delta_min, default_dash_power_rate + new_dash_power_rate_delta_max].
        The actual delta is a uniform random variable biased around (new_dash_power_rate_delta_min + new_dash_power_rate_delta_max) / 2
        We remove this bias to normalize the parameter to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.DASH_POWER_RATE_DELTA_MIN, 
            RCSSPlayerParamsV16.DASH_POWER_RATE_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.DASH_POWER_RATE + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.DASH_POWER_RATE - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.DASH_POWER_RATE - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)


class StaminaIncMaxNormalizerV16(FeatureNormalizer):
    """ Normalize Stamina Increment Maximum heterogeneous parameter.
        Stamina Increment Maximum is in a range that depends on the Dash Power Rate range.
        The actual delta is a uniform random variable in this range, so it is biased around (range_upper_bound+range_lower_bound)/2
        We remove this bias to normalize the parameter to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.STAMINA_INC_MAX_DELTA_FACTOR * RCSSPlayerParamsV16.DASH_POWER_RATE_DELTA_MIN,
            RCSSPlayerParamsV16.STAMINA_INC_MAX_DELTA_FACTOR * RCSSPlayerParamsV16.DASH_POWER_RATE_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2 
        values -= RCSSServerParamsV16.STAMINA_INC_MAX + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta)/2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.STAMINA_INC_MAX - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.STAMINA_INC_MAX - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)


class PlayerDecayNormalizerV16(FeatureNormalizer):
    """ Normalize Player Decay heterogeneous parameter.
        Player Decay is in [default_player_decay + player_decay_delta_min, default_player_decay + player_decay_delta_max].
        The actual delta is a uniform random variable biased around (player_decay_delta_min + player_decay_delta_max)/2
        We remove this bias to normalize the parameter to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.PLAYER_DECAY_DELTA_MIN, 
            RCSSPlayerParamsV16.PLAYER_DECAY_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.PLAYER_DECAY + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.PLAYER_DECAY - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.PLAYER_DECAY - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)


class InertiaMomentNormalizerV16(FeatureNormalizer):
    """ Normalize Inertia Moment heterogeneous parameter.
        Inertia Moment is in a range that depends on the Player Decay range.
        The actual delta is a uniform random variable in this range, so it is biased around (range_upper_bound+range_lower_bound)/2
        We remove this bias to normalize it to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.INERTIA_MOMENT_DELTA_FACTOR * RCSSPlayerParamsV16.PLAYER_DECAY_DELTA_MIN,
            RCSSPlayerParamsV16.INERTIA_MOMENT_DELTA_FACTOR * RCSSPlayerParamsV16.PLAYER_DECAY_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.INERTIA_MOMENT + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)

    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.INERTIA_MOMENT - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.INERTIA_MOMENT - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)

class KickableMarginNormalizerV16(FeatureNormalizer):
    """ Normalize Kickable Margin heterogeneous parameter.
        Kickable Margin is in [default_kickable_margin + kickable_margin_delta_min, default_kickable_margin + kickable_margin_delta_max].
        The actual delta is a uniform random variable biased around (kickable_margin_delta_min + kickable_margin_delta_max)/2
        We remove this bias to normalize it to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.KICKABLE_MARGIN_DELTA_MIN,
            RCSSPlayerParamsV16.KICKABLE_MARGIN_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.KICKABLE_MARGIN + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.KICKABLE_MARGIN - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.KICKABLE_MARGIN - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)

class KickRandNormalizerV16(FeatureNormalizer):
    """ Normalize Kick Rand heterogeneous parameter.
        Kick Rand is in a range that depends on the Kickable Margin range.
        The actual delta is a uniform random variable in this range, so it is biased around (range_upper_bound+range_lower_bound)/2
        We remove this bias to normalize it to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.KICK_RAND_DELTA_FACTOR * RCSSPlayerParamsV16.KICKABLE_MARGIN_DELTA_MIN,
            RCSSPlayerParamsV16.KICK_RAND_DELTA_FACTOR * RCSSPlayerParamsV16.KICKABLE_MARGIN_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.KICK_RAND + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.KICK_RAND - bounds_middle).clip(-1, 1) 
        return (2*(values - RCSSServerParamsV16.KICK_RAND - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)

class ExtraStaminaNormalizerV16(FeatureNormalizer):
    """ Normalize Extra Stamina heterogeneous parameter.
        Extra Stamina is in [default_extra_stamina + extra_stamina_delta_min, default_extra_stamina + extra_stamina_delta_max].
        The actual delta is a uniform random variable biased around (extra_stamina_delta_min + extra_stamina_delta_max)/2
        We remove this bias to normalize it to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.EXTRA_STAMINA_DELTA_MIN, 
            RCSSPlayerParamsV16.EXTRA_STAMINA_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.EXTRA_STAMINA + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.EXTRA_STAMINA - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.EXTRA_STAMINA - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)


class EffortMinNormalizerV16(FeatureNormalizer):
    """ Normalize Effort Minimum heterogeneous parameter.
        Effort Minimum is in a range that depends on the Extra Stamina range.
        The actual delta is a uniform random variable biased around (range_upper_bound+range_lower_bound)/2
        We remove this bias to normalize it to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.EFFORT_MIN_DELTA_FACTOR * RCSSPlayerParamsV16.EXTRA_STAMINA_DELTA_MIN,
            RCSSPlayerParamsV16.EFFORT_MIN_DELTA_FACTOR * RCSSPlayerParamsV16.EXTRA_STAMINA_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.EFFORT_MIN + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.EFFORT_MIN - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.EFFORT_MIN - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)

class EffortMaxNormalizerV16(FeatureNormalizer):
    """ Normalize Effort Maximum heterogeneous parameter.
        Effort Maximum is in a range that depends on the Extra Stamina range.
        The actual delta is a uniform random variable biased around (range_upper_bound+range_lower_bound)/2
        We remove this bias to normalize it to a tight [-1, 1] range.
    """

    def __get_bound_deltas(self) -> Tuple[float, float]:
        return (
            RCSSPlayerParamsV16.EFFORT_MAX_DELTA_FACTOR * RCSSPlayerParamsV16.EXTRA_STAMINA_DELTA_MIN,
            RCSSPlayerParamsV16.EFFORT_MAX_DELTA_FACTOR * RCSSPlayerParamsV16.EXTRA_STAMINA_DELTA_MAX
        )

    def inplace(self, values: np.ndarray) -> None:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        values -= RCSSServerParamsV16.EFFORT_INIT + bounds_middle # Centers the values around 0
        if np.abs(upper_bound_delta - lower_bound_delta) > 1e-7:
            values /= np.abs(upper_bound_delta - lower_bound_delta) / 2 # Normalize to [-1, 1]
        values.clip(-1, 1, out=values)
    
    def normalize(self, values: np.ndarray) -> np.ndarray:
        lower_bound_delta, upper_bound_delta = self.__get_bound_deltas()
        bounds_middle = (lower_bound_delta + upper_bound_delta) / 2
        if np.abs(upper_bound_delta - lower_bound_delta) < 1e-7:
            return (values - RCSSServerParamsV16.EFFORT_INIT - bounds_middle).clip(-1, 1)
        return (2*(values - RCSSServerParamsV16.EFFORT_INIT - bounds_middle) / np.abs(upper_bound_delta - lower_bound_delta)).clip(-1, 1)
