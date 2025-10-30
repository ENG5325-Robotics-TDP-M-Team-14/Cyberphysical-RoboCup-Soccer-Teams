from enum import Enum
from logging import LoggerAdapter
import numpy as np
from pathlib import Path
import tensorflow as tf
from typing import List, NamedTuple, Optional, OrderedDict, Tuple, Union

from tasks.rcss2d import FieldSide, RCSSServerParamsV16 as SP, UniformNumber, RCSSPlayerParamsV16 as PP

"""
    Features Columns:
        ball_x                                  : float32
        ball_y                                  : float32
        ball_vx                                 : float32
        ball_vy                                 : float32
        {<side><unum>, self}_x                  : float32
        {<side><unum>, self}_y                  : float32
        {<side><unum>, self}_body               : float32
        {<side><unum>, self}_vx                 : float32
        {<side><unum>, self}_vy                 : float32
        {<side><unum>, self}_dash_power_rate    : float32
        {<side><unum>, self}_effort_min         : float32
        {<side><unum>, self}_effort_max         : float32
        {<side><unum>, self}_extra_stamina      : float32
        {<side><unum>, self}_inertia_moment     : float32
        {<side><unum>, self}_kick_rand          : float32
        {<side><unum>, self}_kickable_margin    : float32
        {<side><unum>, self}_player_decay       : float32
    where <side> and <unum> are a player's side (l or r) and uniform number (1 to 11)

    Outputs Columns:
        playercommand_type
        dash_power
        dash_direction
        turn_moment
        kick_power
        kick_direction
        tackle_direction
"""
POSITION_FEATURES = ['x','y']
POSE_FEATURES = POSITION_FEATURES + ['body']
VEL_FEATURES = ['vx','vy']
HETEROPARAM_FEATURES = [
    'dash_power_rate',
    'effort_min',
    'effort_max',
    'extra_stamina',
    'inertia_moment',
    'kick_rand',
    'kickable_margin',
    'player_decay'
]
ALL_BALL_FEATURES = [
    f"ball_{feature}" for feature in POSITION_FEATURES + VEL_FEATURES
]
ALL_PLAYER_FEATURES = [
    f"{side}{unum}_{feature}" for side in ('l', 'r') for unum in range(1,12) for feature in POSE_FEATURES + VEL_FEATURES + HETEROPARAM_FEATURES
]
ALL_SELF_FEATURES = [
    f"self_{feature}" for feature in POSE_FEATURES + VEL_FEATURES + HETEROPARAM_FEATURES
]
ALL_FEATURE_COLUMNS = [
    *ALL_BALL_FEATURES,
    *ALL_PLAYER_FEATURES,
    *ALL_SELF_FEATURES
]
CLASSIFICATION_OUTPUT_COLUMNS = [
    'playercommand_type'
]
REGRESSION_OUTPUT_COLUMNS = [
    'dash_power',
    'dash_direction',
    'turn_moment',
    'kick_power',
    'kick_direction',
    'tackle_direction'
]
OUTPUT_COLUMNS = [
    *CLASSIFICATION_OUTPUT_COLUMNS,
    *REGRESSION_OUTPUT_COLUMNS
]
COMMAND_TYPES = [
    b'dash',
    b'turn',
    b'kick',
    b'tackle'
]

class TrainingOptions(NamedTuple):

    #
    # General options
    #
    logger:                         LoggerAdapter
    session_homepath:               Path
    tensorboard_suffix:             str
    num_checkpoints:                int
    #
    # Dataset options
    #
    training_datasetpath:           Path
    test_and_validation_datasetpath:Path
    batch_size:                     int     # The number of data points that makes a mini-batch for backpropagation.
    #
    #  Architecture options
    #
    input_arch:                     str
    hidden_arch:                    List[int]
    hidden_activation:              str
    regression_activation:          str
    # initial_weights:        Optional[Path]=None
    # input_layer_bias:       Optional[np.ndarray]=None
    # output_layer_bias:      Optional[np.ndarray]=None
    #
    # Training options
    #
    optimizer:                  str
    learning_rate:              float # All optimizers
    lrate_scheduling:           Optional[str] # All optimizers
    lrate_decay:                float # All optimizers
    lrate_decay_step:           float # All optimizers
    lrate_fineschedule:         List[Tuple[int, float]] # All optimizers
    rho:                        float # RMSProp
    momentum:                   float # RMSProp
    epsilon:                    float # Adagrad, RMSProp, Adam
    initial_accumulator_value:  float # Adagrad
    beta1:                      float # Adam
    beta2:                      float # Adam
    epochs:                     int
    steps_per_epoch:            int
    validation_steps:           int

# class LearningRateFineSchedule(tf.keras.optimizers.schedules.LearningRateSchedule):

#   def __init__(self, initial_learning_rate: float, schedule: List[Tuple[int, float]]) -> None:
#     self.learning_rate = initial_learning_rate
#     self.schedule = schedule
#     print(self.schedule)
#     print(self.schedule[0])
#     self.pointer = 0

#   def __call__(self, step: int) -> float:
#     def update_lrate():
#         self.learning_rate = self.schedule[self.pointer][1]
#         self.pointer += 1
#         return self.learning_rate
#     def return_lrate():
#         return self.learning_rate
#     return tf.cond(tf.logical_and(tf.less(self.pointer, len(self.schedule)), tf.equal(step, self.schedule[self.pointer][0])), true_fn=update_lrate, false_fn=return_lrate)

# class LearningRateInverseSchedule(tf.keras.optimizers.schedules.LearningRateSchedule):

#   def __init__(self, initial_learning_rate: float) -> None:
#     self.initial_learning_rate = initial_learning_rate

#   def __call__(self, step: int) -> float:
#     return self.initial_learning_rate / (1 + step)


def correct_vel_normalizations(tensor_dict: OrderedDict[str, tf.Tensor], options: Optional[TrainingOptions]=None) -> OrderedDict[str, tf.Tensor]:
    """
        Corrects the velocity domain normalization of the v1 dataset.
            The v1 dataset has the velocities domain-normalized by their max speed, but we also have a friction effect in the values
            we get from the rcssserver. We divide the columns by the appropriate decay (ball decay for balls and the hetero param decay for players)
        
    """
    class DecayObjectType(Enum):
        BALL = 'ball'
        PLAYER = 'player'
        SELF = 'self'
        def __str__(self) -> str:
            return self.value
    
    def get_decay(decay_type: DecayObjectType, player: Optional[Tuple[FieldSide, UniformNumber]]=None) -> Union[float, tf.Tensor]:
        """
            Ball has a standard scalar decay.
            Players have a specific scalar decay depending on what Heterogeneous Type they are assigned to, so we return a tensor of decays.
            TODO: Is this really right or should i use the default player decay for all players and self.
        """
        if decay_type == DecayObjectType.BALL:
            return SP.BALL_DECAY
        elif decay_type == DecayObjectType.PLAYER:
            if player is None:
                # options.logger.warn(f"Asked for a player decay but no player specified. Using default RCSS2Dv16 Player Decay.")
                return SP.PLAYER_DECAY
            feature_name = f"{player[0]}{player[1]}_player_decay"
            if feature_name in tensor_dict:
                return tensor_dict[feature_name]
            # options.logger.warn(f"Could not find decay feature {feature_name}. Using default RCSS2Dv16 Player Decay.")
            return SP.PLAYER_DECAY
        elif decay_type == DecayObjectType.SELF:
            feature_name = 'self_player_decay'
            if feature_name in tensor_dict:
                return tensor_dict[feature_name]
            # options.logger.warn(f"Could not find decay feature {feature_name}. Using default RCSS2Dv16 Player Decay.")
            return SP.PLAYER_DECAY
        # options.logger.critical(f"There's no decay parameter for decay type {decay_type}")
        raise NotImplementedError(f"There's no decay parameter for decay type {decay_type}")
    
    def denormalize_decay(decay: tf.Tensor) -> tf.Tensor:
        return SP.PLAYER_DECAY + ((PP.PLAYER_DECAY_DELTA_MAX - PP.PLAYER_DECAY_DELTA_MIN)/2) * decay
    ##
    # Ball, if present.
    ##
    ball_decay = get_decay(DecayObjectType.BALL)
    if 'ball_vx' in tensor_dict:
        tensor_dict['ball_vx'] /= (1 + SP.BALL_RAND) * ball_decay
    if 'ball_vy' in tensor_dict:
        tensor_dict['ball_vy'] /= (1 + SP.BALL_RAND) * ball_decay
    ##
    # Players, when present.
    ##
    for side in (FieldSide.LEFT, FieldSide.RIGHT):
        for unum in range(1,12):
            vx_str = f'{side}{unum}_vx'
            vy_str = f'{side}{unum}_vy'
            player_decays = get_decay(DecayObjectType.PLAYER, player=(side, UniformNumber.from_int(unum)))
            player_decays = denormalize_decay(player_decays)
            if vx_str in tensor_dict:
                tensor_dict[vx_str] /=  (1 + SP.PLAYER_RAND) * player_decays
            if vy_str in tensor_dict:
                tensor_dict[vy_str] /= (1 + SP.PLAYER_RAND) * player_decays
    ##
    # Self, if present.
    ##
    if 'self_vx' in tensor_dict:
        self_decays = get_decay(DecayObjectType.SELF)
        self_decays = denormalize_decay(self_decays) # Denormalize it
        tensor_dict['self_vx'] /= (1 + SP.PLAYER_RAND) * self_decays
    if 'self_vy' in tensor_dict:
        tensor_dict['self_vy'] /= (1 + SP.PLAYER_RAND) * self_decays
    
    return tensor_dict

class CommandMetrics(tf.keras.metrics.Metric):
    """
        This is a Tensorflow Metric class that allows us to collect command-specific metrics during training.
        Check out v1_0_x.py to see an example on how to compile the computation graph with this object.

        This Metric calculates Accucary, Precision and Recall for the specific command asked.
        Note that if the batch has no instance of this command, the calculated precision will be NaN.
    """
    ALL_CMD_METRICS = [
        'acc', 
        'prec',
        'rec',
        'tp_cnt',
        'fp_cnt',
        'tn_cnt',
        'fn_cnt'
    ]
    COMMAND_INDEX = {
        'dash': 0,
        'turn': 1,
        'kick': 2,
        'tackle': 3
    }

    def __init__(self, command: str, *args, **kwargs) -> None:
        super(CommandMetrics, self).__init__(name=f'{command}_acc_prec_rec', *args, **kwargs)
        if command not in self.COMMAND_INDEX: raise ValueError(f'Invalid command {command}')
        self.command = command
        self.cmd_metrics = {}
        for metric in self.ALL_CMD_METRICS[:3]:
            self.cmd_metrics[f"{command}_{metric}"] = self.add_weight(name=f"{command}_{metric}", initializer='zeros')
        for metric in self.ALL_CMD_METRICS[3:]:
            self.cmd_metrics[f"{command}_{metric}"] = self.add_weight(name=f"{command}_{metric}", dtype=np.int32, initializer='zeros')
        self.computed = 0
    
    def update_state(self, ytrue, ypred, sample_weight=None):
        """
            This is called once after every trained batch.
            ytrue: Tensor<shape=(batch,output_size)> with the correct output according to our dataset.
            ypred: Tensor<shape=(batch,output_size)> with the output of the neural network.
        """
        predictions = tf.reshape(tf.argmax(ypred, axis=1), shape=(-1,1)) # Reshape into (<inferred>, 1)
        answers = tf.reshape(tf.argmax(ytrue, axis=1), shape=(-1,1))# Reshape into (<inferred>, 1)
        command_index = self.COMMAND_INDEX[self.command]
        
        true = tf.equal(predictions, answers)
        false = tf.not_equal(predictions, answers)
        positive = tf.reshape(tf.equal(predictions, command_index), shape=(-1,1))
        negative = tf.reshape(tf.not_equal(predictions, command_index), shape=(-1,1))
        
        truepositives = tf.reshape(tf.reduce_sum(tf.cast(tf.logical_and(true, positive), np.int32), axis=0), shape=())
        truenegatives = tf.reshape(tf.reduce_sum(tf.cast(tf.logical_and(true, negative), np.int32), axis=0), shape=())
        falsepositives = tf.reshape(tf.reduce_sum(tf.cast(tf.logical_and(false, positive), np.int32), axis=0), shape=())
        falsenegatives = tf.reshape(tf.reduce_sum(tf.cast(tf.logical_and(false, negative), np.int32), axis=0), shape=())

        # Counts
        self.cmd_metrics[f'{self.command}_tp_cnt'].assign_add(truepositives)
        self.cmd_metrics[f'{self.command}_fp_cnt'].assign_add(falsepositives)
        self.cmd_metrics[f'{self.command}_tn_cnt'].assign_add(truenegatives)
        self.cmd_metrics[f'{self.command}_fn_cnt'].assign_add(falsenegatives)

        truepositives = tf.cast(truepositives, np.float32)
        truenegatives = tf.cast(truenegatives, np.float32)
        falsepositives = tf.cast(falsepositives, np.float32)
        falsenegatives = tf.cast(falsenegatives, np.float32)

        # Accuracy, Precision, Recall
        for metric in self.ALL_CMD_METRICS[:3]:
            self.cmd_metrics[f'{self.command}_{metric}'].assign(self.cmd_metrics[f'{self.command}_{metric}'] * self.computed)
        
        self.cmd_metrics[f'{self.command}_acc'].assign_add(truepositives / (truepositives + truenegatives + falsepositives + falsenegatives))
        self.cmd_metrics[f'{self.command}_prec'].assign_add(truepositives / (truepositives + falsepositives))
        self.cmd_metrics[f'{self.command}_rec'].assign_add(truepositives / (truepositives + falsenegatives))
        
        self.computed += 1
        for metric in self.ALL_CMD_METRICS[:3]:
            self.cmd_metrics[f'{self.command}_{metric}'].assign(self.cmd_metrics[f'{self.command}_{metric}'] / self.computed)

    def result(self):
        return self.cmd_metrics

    def reset_state(self):
        self.computed = 0
        self.cmd_metrics[f'{self.command}_tp_cnt'].assign(0)
        self.cmd_metrics[f'{self.command}_fp_cnt'].assign(0)
        self.cmd_metrics[f'{self.command}_tn_cnt'].assign(0)
        self.cmd_metrics[f'{self.command}_fn_cnt'].assign(0)
        self.cmd_metrics[f'{self.command}_acc'].assign(0.0)
        self.cmd_metrics[f'{self.command}_prec'].assign(0.0)
        self.cmd_metrics[f'{self.command}_rec'].assign(0.0)
