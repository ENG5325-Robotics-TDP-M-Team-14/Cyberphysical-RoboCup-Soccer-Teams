import asyncio
import ast
import logging
import os
from pathlib import Path
import psycopg2 as pg
from nubia import command, argument
import numpy as np
import sys
import tensorflow as tf
from termcolor import cprint
import time
from typing import Any, Dict, List, Optional, Tuple
from .utils import listcsvs, SessionLoggerAdapter

@command("v1-data", help='Commands for data preparation of the v1 dataset.')
class DataCLI:

    @command("update-match-playertypes-at-postgres", help="Update the existing playertypes postgres table using playertypes CSV tables.")
    @argument("indir", aliases=['i'], type=Path, description="Path to look for CSVs.")
    @argument("hostname", aliases=['hn'], type=str, description="Postgres host.")
    @argument("password", aliases=['pw'], type=str, description="Postgres user password.")
    @argument("port", aliases=['p'], type=int, description="Postgres port.")
    @argument("user", aliases=['u'], type=str, description="Postgres user.")
    @argument("dbname", aliases=['db'], type=str, description="Postgres database name.")
    @argument("schema", aliases=['sc'], type=str, description="Postgres database destination schema name.")
    def update_all_matches_playertypes_at_postgres(self, indir: Path, hostname: str, password: str, port: int=5432, user: str='postgres', dbname: str='postgres', schema: str='data') -> int:
        """
            Update the existing playertypes postgres table using all playertypes CSV tables in a given folder.
            Returns an error code (Unix style).
        """
        from tasks.v1.data import update_match_playertypes_at_postgres
        cprint(f"Input dir: {indir}")
        cprint(f"Host: {hostname}")
        cprint(f"Password: {password}")
        cprint(f"Port: {port}")
        cprint(f"Username: {user}")
        cprint(f"DB Name: {dbname}")
        csvpaths, compressedcsvpaths = listcsvs(indir)
        cprint(f"Found {len(csvpaths)} CSV files")
        cprint(f"Found {len(compressedcsvpaths)} GZ-compressed CSV files")
        async def tasks():
            # Filter for player types tables
            filtered_csvpaths = list(filter(lambda csvpath: csvpath.name.endswith('playertypes.csv'), csvpaths)) +\
                list(filter(lambda csvpath: csvpath.name.endswith('playertypes.csv.gz'), compressedcsvpaths))
            connection = pg.connect(f"host={hostname} port={port} dbname={dbname} user={user} password={password}")
            connection.set_isolation_level(1)
            try:
                asyncjobs = list(map(lambda filepath: update_match_playertypes_at_postgres(filepath, connection, schema), filtered_csvpaths))
                await asyncio.gather(*asyncjobs)
            finally:
                connection.close()
        asyncio.run(tasks())
        return 0

    @command("copy-all-matches-contents-to-postgres", help="Copy all matches' contents spreaded into multiple tables of a specific schema of a postgresql database.")
    @argument("indir", aliases=['i'], type=Path, description="Path to look for CSVs.")
    @argument("hostname", aliases=['hn'], type=str, description="Postgres host.")
    @argument("password", aliases=['pw'], type=str, description="Postgres user password.")
    @argument("port", aliases=['p'], type=int, description="Postgres port.")
    @argument("user", aliases=['u'], type=str, description="Postgres user.")
    @argument("dbname", aliases=['db'], type=str, description="Postgres database name.")
    @argument("schema", aliases=['sc'], type=str, description="Postgres database destination schema name.")
    def copy_all_matches_contents_to_postgres(self, indir: Path, hostname: str, password: str, schema: str, port: int=5432, user: str='postgres', dbname: str='postgres') -> int:
        """
            Copy all data in a folder to a postgres database.
            The data is spreaded into multiple (pre-defined) tables of a specific schema.
            Returns an error code (Unix style).
        """
        from tasks.v1.data import copy_match_contents_to_postgres
        cprint(f"Input dir: {indir}")
        cprint(f"Host: {hostname}")
        cprint(f"Password: {password}")
        cprint(f"Port: {port}")
        cprint(f"DB Schema: {schema}")
        cprint(f"Username: {user}")
        cprint(f"DB Name: {dbname}")
        csvpaths, compressedcsvpaths = listcsvs(indir)
        cprint(f"Found {len(csvpaths)} CSV files")
        cprint(f"Found {len(compressedcsvpaths)} GZ-compressed CSV files")
        async def tasks():
            # Group files by soccer match
            grouped_filestems: Dict[str, List[Path]] = {}
            for path in [*csvpaths, *compressedcsvpaths]:
                filestem = path.stem.split('.')[0]
                if filestem not in grouped_filestems:
                    grouped_filestems[filestem] = []
                grouped_filestems[filestem].append(path)
            connection = pg.connect(f"host={hostname} port={port} dbname={dbname} user={user} password={password}")
            connection.set_isolation_level(1)
            try:
                asyncjobs = list(map(lambda grouped_filepaths: copy_match_contents_to_postgres(grouped_filepaths, connection, schema), grouped_filestems.values()))
                await asyncio.gather(*asyncjobs)
            finally:
                connection.close()
        asyncio.run(tasks())
        return 0
    
    @command("normalize-raw-features", aliases=['normalize'], help="Normalized extracted raw features for use in v1.0.x experiments from each CSV and save it in a new CSV.")
    @argument("compress", aliases=['c'], type=bool, description="Whether to compress the generated CSV.")
    @argument("indir", aliases=['i'], type=Path, description="Path to look for CSVs.")
    @argument("outdir", aliases=['o'], type=Path, description="Path to save the generated CSVs.")
    def normalize_raw_features(self, indir: Path, compress: bool=True, outdir: Path=Path(os.getcwd())) -> int:
        """
            Normalizes previously extracted raw features. Each feature has its own normalization bounds.
        """
        from tasks.v1.data import normalize_raw_features
        os.getcwd()
        cprint(f"Compress? {compress}")
        cprint(f"Input dir: {indir}")
        cprint(f"Output dir: {outdir}")
        if not outdir.exists():
            try:
                os.makedirs(outdir)
            except OSError as err:
                cprint(err)
                return 1
        csvpaths, compressedcsvpaths = listcsvs(indir)
        cprint(f"Found {len(csvpaths)} CSV files")
        cprint(f"Found {len(compressedcsvpaths)} GZ-compressed CSV files")
        async def tasks():
            asyncjobs = list(map(lambda filepath: normalize_raw_features(filepath, compress, outdir), csvpaths)) +\
                        list(map(lambda filepath: normalize_raw_features(filepath, compress, outdir), compressedcsvpaths))
            await asyncio.gather(*asyncjobs)
        asyncio.run(tasks())
        return 0

    @command("extract-raw-features", aliases=['extract'], help="Extract the useful data for use in v1.0.x experiments from each CSV and save it in a new CSV.")
    @argument("compress", aliases=['c'], type=bool, description="Whether to compress the generated CSV.")
    @argument("indir", aliases=['i'], type=Path, description="Path to look for CSVs.")
    @argument("outdir", aliases=['o'], type=Path, description="Path to save the generated CSVs.")
    def extract_raw_features(self, indir: Path, compress: bool=True, outdir: Path=Path(os.getcwd())) -> int:
        """
            Extracts a subset of raw features (table columns) from the canonical dataset that are useful for v1.0.x experiments
            Returns an error code (Unix style).
        """
        from tasks.v1.data import extract_raw_features
        cprint(f"Compress? {compress}")
        cprint(f"Input dir: {indir}")
        cprint(f"Output dir: {outdir}")
        if not outdir.exists():
            try:
                os.makedirs(outdir)
            except OSError as err:
                cprint(err)
                return 1
        csvpaths, compressedcsvpaths = listcsvs(indir)
        cprint(f"Found {len(csvpaths)} CSV files")
        cprint(f"Found {len(compressedcsvpaths)} GZ-compressed CSV files")
        async def tasks():
            asyncjobs = list(map(lambda filepath: extract_raw_features(filepath, compress, outdir), csvpaths)) +\
                        list(map(lambda filepath: extract_raw_features(filepath, compress, outdir), compressedcsvpaths))
            await asyncio.gather(*asyncjobs)
        asyncio.run(tasks())
        return 0
    
    @command("copy-all-matches-metadata-to-postgres", aliases=['postgres'], help="Copy Matches' metadata to a postgresql database's 'public.matches' table.")
    @argument("indir", aliases=['i'], type=Path, description="Path to look for CSVs.")
    @argument("hostname", aliases=['hn'], type=str, description="Postgres host.")
    @argument("password", aliases=['pw'], type=str, description="Postgres user password.")
    @argument("port", aliases=['p'], type=int, description="Postgres port.")
    @argument("user", aliases=['u'], type=str, description="Postgres user.")
    @argument("dbname", aliases=['db'], type=str, description="Postgres database name.")
    def copy_all_matches_metadata_to_postgres(self, indir: Path, hostname: str, password: str, port: int=5432, user: str='postgres', dbname: str='postgres') -> int:
        """
            Copy all data in a folder to a postgres database.
            All the data is dumped into a specific table of a specific schema.
            Returns an error code (Unix style).
        """
        from tasks.v1.data import copy_match_metadata_to_postgres
        cprint(f"Input dir: {indir}")
        cprint(f"Host: {hostname}")
        cprint(f"Password: {password}")
        cprint(f"Port: {port}")
        cprint(f"Username: {user}")
        cprint(f"DB Name: {dbname}")
        csvpaths, compressedcsvpaths = listcsvs(indir)
        cprint(f"Found {len(csvpaths)} CSV files")
        cprint(f"Found {len(compressedcsvpaths)} GZ-compressed CSV files")
        async def tasks():
            connection = pg.connect(f"host={hostname} port={port} dbname={dbname} user={user} password={password}")
            try:
                match_filepaths = list(filter(lambda filepath: filepath.name.endswith('.match.csv'), csvpaths)) +\
                        list(filter(lambda filepath: filepath.name.endswith('.match.csv.gz'), compressedcsvpaths))
                asyncjobs = list(map(lambda match_filepath: copy_match_metadata_to_postgres(match_filepath, connection), match_filepaths))
                await asyncio.gather(*asyncjobs)
            finally:
                connection.close()
        asyncio.run(tasks())
        return 0


@command("v1-train", help='Commands for training models for experiments')
class TrainCLI:
    
    def _new_logger(self, logfilepath: Path) -> logging.Logger:
        logger: logging.Logger = logging.getLogger('train')
        log_formatter = logging.Formatter("[%(asctime)s] [%(levelname)8s] [%(name)4s]::%(funcName)s(), line %(lineno)s: %(message)s")
        for handler in logger.handlers:
            handler.setFormatter(log_formatter) ## TODO: This won't work and i dont know why
        file_handler = logging.FileHandler(filename=logfilepath.resolve(), mode='w')
        file_handler.setLevel(logging.INFO)
        file_handler.setFormatter(log_formatter)
        logger.addHandler(file_handler)
        return logger
    
    def _make_logger_and_outdir(self, outdir: Path) -> Tuple[logging.Logger, Path]:
        if not outdir.exists():
            error_msg = f'Output directory path {outdir} does not exist. Aborting...'
            print(error_msg)
            raise ValueError(error_msg)
        elif not outdir.is_dir():
            error_msg = f'Output directory path {outdir} does not point to a directory. Aborting...'
            print(error_msg)
            raise ValueError(error_msg)
        
        logging_homepath = outdir / Path(time.strftime("%Y%m%d-%H%M%S-train-v1.0.0", time.localtime()))
        logging_homepath.mkdir(parents=True, exist_ok=True)
        logfilepath = logging_homepath / 'execution.log'
        logger = self._new_logger(logfilepath)
        logger.info(f'Logger started. Logs are saved to: {logfilepath.resolve()}')
        return logger, logging_homepath

    
    @command("v1-0-x", help='Feedforward Neural Network training for action type prediction and action parameter regression')
    @argument('training', type=Path, description="The path to the training dataset file.")
    @argument('test_and_validation', type=Path, description="The path to the test and validation dataset file.")
    @argument('nsessions', type=int, description="Number of training sessions to execute.")
    @argument('outdir', type=Path, description="The root folder where to store training logs, tensorboard logs and trained models.")
    @argument('seed', type=int, description="Seed to be used for random number generation during training. If none is specified, a new one is generated")
    @argument('tensorboard_suffix', type=str, description="Suffix to append at the end of the tensorboard log")
    @argument('num_checkpoints', type=int, description="Number of model checkpoints to be saved during training (these will be spread uniformly throughout training)")
    @argument('batch_size', type=int, description="The number of data points used at each step of backpropagation.")
    @argument('input_arch', type=str, description="The input layer especification one of {'full', 'ablation1', 'ablation2', 'none'} ")
    @argument('hidden_arch', type=str, description="The hidden layer architecture in the form of a python list, i.e. [512,256,128]")
    @argument('hidden_activation', type=str, description="Activation function to use for in the hidden layer")
    @argument('regression_activation', type=str, description="Activation function to use for in the regression output layer")
    @argument('optimizer', type=str, description="The optimization algorithm.")
    @argument('learning_rate', type=float, description="The learning rate of the gradient optimization.")
    @argument('lrate_scheduling', type=str, description="The scheduling strategy to apply at the learning rate.")
    @argument('lrate_decay', type=float, description="The decay when using a learning rate decay strategy.")
    @argument('lrate_decay_step', type=float, description="The rate of update when using a learning rate decay strategy. When opting for discrete updates, the update happens every this number of steps.")
    @argument('lrate_fineschedule', type=str, description="The list of exact epochs to update the learning rate and the value to be assigned when using a fine-tuned learning rate decay strategy.")
    @argument('rho', type=float, description="The exponential discount factor on gradient accumulation for adaptive optimizers (when applicable).")
    @argument('momentum', type=float, description="The momentum weight for first order optimizers (when applicable). This is the classical momentum, not the Nesterov one.")
    @argument('epsilon', type=float, description="The epsilon used for numerically stabilizing the scaling of gradients with their accumulated inverse square roots (AdaGrad, RMSProp, Adam).")
    @argument('initial_accumulator_value', type=float, description="AdaGrad's initial value for the gradient square accumulator.")
    @argument('beta1', type=float, description="Adam's 1st moment decay parameter.")
    @argument('beta2', type=float, description="Adam's 2nd moment decay parameter.")
    @argument('epochs', type=int, description="Number of training epochs to execute in a session. Set to a negative number to loop indefinitely.")
    @argument('steps_per_epoch', type=int, description="Number of batches that define an epoch of training. This is necessary as we can't transverse the whole dataset.")
    @argument('validation_steps', type=int, description="Number of batches that define an epoch of training.")
    def v1_0_x(self,
        patch: int,
        training: Path, 
        test_and_validation: Path,
        nsessions: int=1, 
        outdir: Path=Path(os.getcwd()) / Path('logs'),
        seed: int=-1,
        tensorboard_suffix: str='',
        num_checkpoints: int=20,
        batch_size: int=256,
        input_arch: str='none',
        hidden_arch: str='[512,256,128]', 
        hidden_activation: str='relu',
        regression_activation: str='tanh',
        optimizer: str='adam',
        learning_rate: float=0.001,
        lrate_scheduling: str=None,
        lrate_decay: float=1,
        lrate_decay_step: float=1,
        lrate_fineschedule: str='[]',
        rho: float=0.9,
        momentum: float=0.0,
        epsilon: float=1e-7,
        initial_accumulator_value: float=0.1,
        beta1: float=0.9,
        beta2: float=0.999,
        epochs: int=np.inf,
        steps_per_epoch: int=300,
        validation_steps: int=200
    ):
        
        logger, logging_homepath = self._make_logger_and_outdir(outdir)
        
        if nsessions < 1:
            logger.warn(f'Invalid nsessions={nsessions}. Falling back to 1 training session only.')
            nsessions = 1

        logger.info(f"Initializing training version 1.0.{patch}")

        ##
        # CLI Options confirmation
        ##
        logger.info(f'General Options')
        logger.info(f"NumberOfSessions={nsessions}")
        logger.info(f"OutDir={outdir}")
        logger.info(f"Seed={seed if nsessions == 1 else 'Many'}")
        logger.info(f"TensorboardSuffix={tensorboard_suffix}")
        logger.info(f"NumberOfCheckpoints={num_checkpoints}")
        
        logger.info(f'Dataset Options')
        logger.info(f'TrainingDataset={training}')
        logger.info(f'TestAndValidationDataset={test_and_validation}')
        logger.info(f'BatchSize={batch_size}')
        
        logger.info(f'Architecture Options')
        logger.info(f'InputArchitecture={input_arch}')
        logger.info(f'HiddenArchitecture={hidden_arch}')
        hidden_arch_unwrapped = ast.literal_eval(hidden_arch)
        logger.info(f'HiddenActivation={hidden_activation}')
        logger.info(f'RegressionActivation={regression_activation}')
        
        logger.info(f'Training Options')
        logger.info(f'Optimizer={optimizer}')
        logger.info(f'LearningRate={learning_rate}')
        logger.info(f'LearningRateScheduling={lrate_scheduling}')
        logger.info(f'LearningRateDecay={lrate_decay}')
        logger.info(f'LearningRateDecayStep={lrate_decay_step}')
        logger.info(f'LearningRateFineSchedule={lrate_fineschedule}')
        lrate_fineschedule_unwrapped = ast.literal_eval(lrate_fineschedule)
        logger.info(f'Rho={rho}')
        logger.info(f'Momentum={momentum}')
        logger.info(f'Epsilon={epsilon}')
        logger.info(f'InitialAccumulatorValue={initial_accumulator_value}')
        logger.info(f'Beta1={beta1}')
        logger.info(f'Beta2={beta2}')
        logger.info(f'NumberOfEpochs={epochs}')
        logger.info(f'StepsPerEpoch={steps_per_epoch}')
        logger.info(f'ValidationStepsPerEpoch={validation_steps}')
        
        from tasks.v1.experiments.v1_0_x import TrainingOptions
        if patch == 0:
            from tasks.v1.experiments.v1_0_0 import train
        elif patch == 1:
            from tasks.v1.experiments.v1_0_1 import train
        elif patch == 2:
            from tasks.v1.experiments.v1_0_2 import train
        else:
            raise NotImplementedError(f"Patch version {patch} does not exist.")

        try:
            ##
            # Run training sessions
            ##
            for session in range(1, nsessions+1):
                ##
                # Pre-session setup
                ##
                ## Session home folder creation
                session_homepath = logging_homepath / f'session{session}'
                try:
                    session_homepath.mkdir(parents=True, exist_ok=False)
                except FileExistsError as excpt:
                    logger.exception(excpt)
                    logger.info(f"Skipping session {session}")
                    continue
                ## Random seed setting
                if seed < 0 or session > 1:
                    seed = np.random.randint(sys.maxsize)
                logger.info(f'Setting TensorFlow random seed to {seed}')
                tf.random.set_seed(seed)
                #
                ## Now just pack everything and train!
                #
                logger.info(f'Starting session #{session}')
                train(TrainingOptions(
                    # General stuff
                    session_homepath=session_homepath,
                    logger=SessionLoggerAdapter(logger,{},session),
                    tensorboard_suffix=tensorboard_suffix,
                    num_checkpoints=num_checkpoints,
                    # Dataset params
                    training_datasetpath=training,
                    test_and_validation_datasetpath=test_and_validation,
                    batch_size=batch_size,
                    # Architecture params
                    input_arch=input_arch,
                    hidden_arch=hidden_arch_unwrapped,
                    hidden_activation=hidden_activation,
                    regression_activation=regression_activation,
                    # Training params
                    optimizer=optimizer,
                    learning_rate=learning_rate,
                    lrate_scheduling=lrate_scheduling,
                    lrate_decay=lrate_decay,
                    lrate_decay_step=lrate_decay_step,
                    lrate_fineschedule=lrate_fineschedule_unwrapped,
                    rho=rho,
                    momentum=momentum,
                    epsilon=epsilon,
                    initial_accumulator_value=initial_accumulator_value,
                    beta1=beta1,
                    beta2=beta2,
                    epochs=epochs,
                    steps_per_epoch=steps_per_epoch,
                    validation_steps=validation_steps
                ))
                logger.info(f'Ended session #{session}')
        finally:
            logging.shutdown()
        return 0
