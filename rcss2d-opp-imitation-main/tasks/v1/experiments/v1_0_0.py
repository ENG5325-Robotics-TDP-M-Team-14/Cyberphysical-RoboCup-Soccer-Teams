import numpy as np
import pickle
import tensorflow as tf
import tensorflow.keras as keras
from tensorflow.keras import layers
from typing import OrderedDict, Tuple

from .v1_0_x import (
    ALL_FEATURE_COLUMNS, 
    OUTPUT_COLUMNS,
    COMMAND_TYPES,
    TrainingOptions,
    LearningRateFineSchedule,
    LearningRateInverseSchedule,
    correct_vel_normalizations,
    CommandMetrics
)


INPUT_DIMENSION = len(ALL_FEATURE_COLUMNS)
OUTPUT_DIMENSION = len(COMMAND_TYPES)

def train(options: TrainingOptions) -> None:
    logger = options.logger

    logger.info('Next: Create dataset definitions.')

    column_defaults = {
        **{feature_col: np.NaN for feature_col in ALL_FEATURE_COLUMNS},
        OUTPUT_COLUMNS[0]: str('nop'),    # Means "No Operation"
    }
    
    trainingset = tf.data.experimental.make_csv_dataset(
        str(options.training_datasetpath.resolve()),
        select_columns=ALL_FEATURE_COLUMNS + OUTPUT_COLUMNS[:1],
        batch_size=options.batch_size,
        column_defaults=column_defaults.values(),
        compression_type='GZIP',
        shuffle=True,
        shuffle_buffer_size=80000
    )
    validationset = tf.data.experimental.make_csv_dataset(
        str(options.test_and_validation_datasetpath.resolve()),
        select_columns=ALL_FEATURE_COLUMNS + OUTPUT_COLUMNS[:1],
        batch_size=options.batch_size,
        column_defaults=column_defaults.values(),
        compression_type='GZIP',
        shuffle=False,  # No need to shuffle the validation!
    ).take(1515186) # That's half the size of the test & val dataset (~2.5% of total rows)
    validationset = validationset.take(options.validation_steps) # We limit the number of evaluations cause we can't support all this computation

    logger.info('Create dataset definition done!')
    logger.info('Next: Create dataset ingestion pipeline')

    trainingset = build_ingestion_pipeline(trainingset, options)
    validationset = build_ingestion_pipeline(validationset, options)

    logger.info('Create dataset ingestion pipeline done!')
    logger.info('Next: Create Neural Network')

    inputs = keras.Input(shape=(INPUT_DIMENSION,), dtype=np.float32, name='input')
    x = layers.Normalization()(inputs)
    for hidden_size in options.hidden_arch:
        x = layers.Dense(units=hidden_size, activation=options.hidden_activation)(x)
    classification_output = layers.Dense(units=OUTPUT_DIMENSION, activation='softmax')(x)

    model = keras.Model(inputs=inputs, outputs=classification_output, name="helios_player_command_prediction")

    learning_rate = options.learning_rate
    # if options.lrate_scheduling is not None:
    #     if options.lrate_scheduling == 'inverse-time-decay' or options.lrate_scheduling == 'inverse-discretetime-decay':
    #         learning_rate = keras.optimizers.schedules.InverseTimeDecay(
    #             initial_learning_rate=options.learning_rate,
    #             decay_steps=options.lrate_decay_step,
    #             decay_rate=options.lrate_decay,
    #             staircase=options.lrate_scheduling == 'inverse-discretetime-decay'
    #         )
    #     elif options.lrate_scheduling == 'fine':
    #         if len(options.lrate_fineschedule) == 0: 
    #             logger.warn("Fine-tuned learning rate schedule selected but not schedule provided")
    #         learning_rate = LearningRateFineSchedule(options.learning_rate, options.lrate_fineschedule)
    #     else:
    #         raise NotImplementedError(f"Specified learning rate scheduling {options.lrate_scheduling} not supported.")
    
    optimizer = None
    if options.optimizer == 'rmsprop':
        optimizer = keras.optimizers.RMSprop(
            learning_rate=learning_rate,
            rho=options.rho,
            momentum=options.momentum,
            epsilon=options.epsilon
        )
    elif options.optimizer == 'adagrad':
        optimizer = keras.optimizers.Adagrad(
            learning_rate=learning_rate,
            initial_accumulator_value=options.initial_accumulator_value,
            epsilon=options.epsilon
        )
    elif options.optimizer == 'adam':
        optimizer = keras.optimizers.Adam(
            learning_rate=learning_rate,
            beta_1=options.beta1,
            beta_2=options.beta2,
            epsilon=options.epsilon
        )
    else:
        raise NotImplementedError(f"Specified optimizer {options.optimizer} not supported.")

    model.compile(
        optimizer=optimizer,
        loss=keras.losses.CategoricalCrossentropy(),
        metrics=[
            # keras.metrics.TruePositives(name='tp'),
            # keras.metrics.FalsePositives(name='fp'),
            # keras.metrics.TrueNegatives(name='tn'),
            # keras.metrics.FalseNegatives(name='fn'),
            keras.metrics.CategoricalAccuracy(name='acc'),
            # keras.metrics.Precision(name='precision'),
            # keras.metrics.Recall(name='recall'),
            # keras.metrics.AUC(name='auc'),
            # keras.metrics.AUC(name='prc', curve='PR'), # precision-recall curve
            CommandMetrics('dash'),
            CommandMetrics('turn'),
            CommandMetrics('kick'),
            CommandMetrics('tackle')
        ]
    )

    model.summary(print_fn=options.logger.info)


    logger.info('Create Neural Network done!')
    logger.info(f'Next: Create callbacks.')

    tensorboard_callback = tf.keras.callbacks.TensorBoard(
        log_dir=str(options.session_homepath.resolve()), 
        histogram_freq=1
    )

    bestmodel_filepath = options.session_homepath / ('-'.join(['modelbest',options.tensorboard_suffix])+".hdf5")
    checkpointbest = tf.keras.callbacks.ModelCheckpoint(
        filepath=str(bestmodel_filepath.resolve()),
        save_weights_only=True,
        monitor='val_acc',
        mode='max',
        save_best_only=True
    )

    commandbest_checkpoints = []
    for command in COMMAND_TYPES:
        command = command.decode('utf8') # It's a bytes object because tensorflow saves it that way
        commandbest_filepath = options.session_homepath / ('-'.join([f'{command}best',options.tensorboard_suffix])+".hdf5")
        commandbest_checkpoints.append(tf.keras.callbacks.ModelCheckpoint(
            filepath=str(commandbest_filepath.resolve()),
            save_weights_only=True,
            monitor=f'val_{command}_acc',
            mode='max',
            save_best_only=True
        ))

    model_per_epoch_filepath = options.session_homepath / ('-'.join(['model{epoch}',options.tensorboard_suffix])+".hdf5")
    checkpoint = tf.keras.callbacks.ModelCheckpoint(
        filepath=str(model_per_epoch_filepath.resolve()),
        save_weights_only=True,
        save_freq=options.steps_per_epoch * (options.epochs // 4) # Save only 4 check points, corresponding to 25%/50%/75%/100% of epochs
    )


    logger.info(f'Create callbacks done!')
    logger.info(f'Next: Execute training.')
    

    fit_history = model.fit(
        x=trainingset,
        validation_data=validationset.cache(),#validationset.take(options.validation_steps).cache(),
        # validation_steps=options.validation_steps, # Redundant
        steps_per_epoch=options.steps_per_epoch,
        epochs=options.epochs,
        callbacks=[
            tensorboard_callback,
            checkpointbest,
            *commandbest_checkpoints,
            checkpoint
        ]
    )

    logger.info('Execute training done!')
    logger.info('Next: Save training history and model result')
    
    with open(str((options.session_homepath / ('-'.join(['fithistory', options.tensorboard_suffix+".pkl"]))).resolve()), 'wb') as file:
        pickle.dump(fit_history.history, file)

    logger.info('Save training history and model result done!')

def concat_features_and_split_input_output(tensor_dict: OrderedDict[str, tf.Tensor], options: TrainingOptions) -> Tuple[tf.Tensor]:
    """
        Concatenates features into a single input tensor and separates it from the output tensor.
        Also turns the output tensor from a (1,batch_size) tensor with strings into a (batch_size,output_dim) 1-hot tensor.
    """
    batch_size = tensor_dict['playercommand_type'].shape[0]

    nn_input = tf.concat(
        [ tf.reshape(tensor_dict[feature], (batch_size, 1)) for feature in ALL_FEATURE_COLUMNS ],
        name='make_nn_input',
        axis=1
    ) # (training_batch,) -> (training_batch, 1) -> (training_batch, input_dimension)

    nn_output = tf.reshape(tensor_dict['playercommand_type'], (batch_size, 1)) # (training_batch,) -> (training_batch, 1)
    nn_output = tf.concat(
        [ tf.equal(nn_output, command_type, name='make_1hot_outputs') for command_type in COMMAND_TYPES], # Generates a Tensor<shape=(training_batch, 1)> for each command string
        axis=1,
        name='make_nn_ouput'
    ) # Concatenates the generated 1-hot tensors of each command string into a Tensor<shape=(training_batch, num_command_types)>
    nn_output = tf.cast(nn_output, np.float32, name='cast_1hot_to_float')

    return nn_input, nn_output

def build_ingestion_pipeline(dataset: tf.data.Dataset, options:TrainingOptions) -> tf.data.Dataset:
    return dataset.map(
        #
        # 1. Fix the bad domain normalization for velocities that we've made when preparing the v1 dataset.
        #
        # The input is a {str: Tensor<shape=(1,training_batch)>} dictionary
        lambda input: correct_vel_normalizations(input, options=options)
        # The output is a {str: Tensor<shape=(1,training_batch)>} dictionary
    ).map(
        #
        # 2. Stack features together to make a single Input tensor and split input from output
        #
        # The input is a dataset in the form of a {str: Tensor<shape=(1,training_batch)>} dictionary
        lambda input: concat_features_and_split_input_output(input, options=options)
        # The output is a (Tensor<shape=(training_batch,input_dim)>, Tensor<shape=(training_batch,num_command_types)>) tuple
        #
    )
