# rcss2d-opp-imitation

Research on the use of Deep Imitation Learning to obtain models of opponent behavior in the RoboCup Soccer Simulation 2D League by the Laboratory of Autonomous Computational Systems, Aeronautics Instute of Technology, Brazil.

This has only been tested on OS X, but should work on Linux with few adjustments.

## Preparation

This project uses [PyPoetry](https://python-poetry.org/) to manage python versions, dependencies and virtual environments.

There is a dependency on [psycopg2](https://www.psycopg.org/docs/), a [Postgres](https://www.postgresql.org/) adapter. _psycopg2_ requires Postgres's libraries, so they should be there before setting things up.

Install dependencies by issuing `poetry install` at the root folder.

To reproduce training, you'll need to [download the training and validation/test datasets](https://1drv.ms/u/s!AvytUx8S4DbMhrcVv6n09UNvfMpKQw?e=mlJNq2) or prepare them on your own.

To reproduce results, you'll need to [download the trained model](https://1drv.ms/u/s!AvytUx8S4DbMhroa7sK82xuQs_XIzg?e=xAh4cv) or use a compatible one trained on your own.

If you use the pipeline stages on this repository to prepare a dataset on your own, keep the convention of CSV naming:
`<FILENAME>.{dash,kick,tackle,turn,match,playertypes}.csv[.gz]`. Check the [example data](./tests/data/) for compressed and uncompressed tables.

You probably may also want to check [rcg2csv](https://github.com/FelipeCoimbra/librcsc) and [rcl2csv](https://gitlab.com/FelipeCoimbra/rcl2csv/) projects to extract CSV tables from log data. Check out the `logs-to-tables/` folder for usage.

## Reproducing data preparation and training interactively

Start a shell wrapped by Poetry's virtual environment:

```
poetry shell
```

Start the CLI through:

```
python cli.py
```

There you can find multiple commands (routines) regarding multiple experiments and their help information.
These routines range from extracting/transforming/cleaning the data to training a model.

### Examples

Output description for a command set.
```
help v1-data
```

Extract useful information from raw CSV tables generated from `*.rcg` and `*.rcl` logs (see [rcg2csv](https://github.com/FelipeCoimbra/librcsc) and [rcl2csv](https://gitlab.com/FelipeCoimbra/rcl2csv/)).
```
v1-data extract-raw-features indir=./datadir/ compress=False outdir=./outdir/
```

Train a Feedforward Neural Network to output action types and parameters. 
```
v1-train v1-0-x patch=2 training=./training_dataset.csv.gz test-and-validation=./test_and_val_dataset.csv.gz
```

## Reproducing data preparation and training programmatically

The `cli.py` tool may also be used programmatically, for example:

```bash
python cli.py -v -s v1-train v1-0-x \
    --patch 2 \ # REQUIRED: Experiment patch number
    --training ./training.csv.gz \ # REQUIRED: Path to training csv table
    --test-and-validation ./test_and_val.csv.gz \ # REQUIRED: Path to test and validation csv table
    --seed 7047888462262876369 \
    --num-checkpoints 2 \
    --input-arch none \
    --hidden-arch [512,256,128] \
    --hidden-activation relu \
    --optimizer adam \
    --learning-rate 0.001 \
    --batch-size 4096 \
    --epochs 2 \
    --steps-per-epoch 300 \
    --validation-steps 200 \
    --tensorboard-suffix 'test-turnclean-hidden_512_256_128-hidden_act_relu-lrate0001-adam-batch4096-epochs200-stepsperepoch300-valsteps200' ;
```

## Datasets

| Version  | Description |
|----------|-------------|
|   v1     | 1400 matches total of Helios2019 against ITAndroids2020, RoboCIn2020, Titans2020, Futvasf2020, MT2019, Yushan2019, and CYRUS2019 teams with Synchronous mode and Fullstate turned on for both sides. 200 matches against each team, with Helios2019 always on the right field side. |
| v1-extra | 60 matches total of Helios2019 against FCPGPR2019, FRAUnited2019, HFutEngine2019, Hillstone2019, Razi2019, and Rione2019 with Synchronous mode and Fullstate mode turned on for both sides. 10 matches against each team, with Helios2019 always on the right field side. |

[The datasets are available here](https://1drv.ms/u/s!AvytUx8S4DbMhrcVv6n09UNvfMpKQw?e=mlJNq2).
A README file in the root directory has further notes on the available data.

## Experiments

| Version | Description |
|---------|-------------|
| v1.0.0  | Feedforward NN with single output layer (softmax for action classification). Arbitrary hidden layer configuration. Choices between RMSProp, Adagrad and Adam optimizers. |
| v1.0.1  | Add action parameter regression output layer. Checkpoint saving of best model per output. Classification metrics per player action. |
| v1.0.2  | Support for input feature selection. Checkpoint saving of best model per player action. |

Our best result reported is a v1.0.2 model which [is available here](https://1drv.ms/u/s!AvytUx8S4DbMhroa7sK82xuQs_XIzg?e=xAh4cv).

A thorough description of the model's design, training, and results are found in 
>[_COIMBRA, Felipe Vieira. Opponent Modeling by Imitation in Robot Soccer. 2021. 130f. Final paper (Undergraduation study) – Instituto Tecnológico de Aeronáutica, São José dos Campos_](http://www.bdita.bibl.ita.br/TGsDigitais/78003.pdf).

## Versioning

We try to adhere to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) just like [TensorFlow 2 does](tensorflow.org/datasets/datasets_versioning) when versioning both Datasets and Training code.

- A training *A.B.C* will always use datasets of version *A.x.y*. A change in *A* means a change in the semantics of the data, its underlying source distribution (an example could be turning off Fullstate mode of RCSS2D matches).
- The number *B* means a different training dataset that comes from the same raw pool of data. Changing or adding columns to tables likely should change *B* because it usually means a pipeline break. We have a command (vA-B-x) for every (MAJOR A, MINOR B) pair.
- The number *C* denotes a patch number. The pipeline is not broken at new *C* numbers but it still may require some few tweaks in code.
Therefore, commands receive the patch as an argument to choose the right module to pull underneath.

## Teams

|    League    | Teams | Download Link |
|--------------|-------|-------------------------|
| LARC 2020 | ITAndroids2020, RoboCIn2020, Titans2020, Futvasf2020 | [https://bitbucket.org/larc_cbr_2d_simulation/larc-cbr-2020-simulation-2d/src/master/](https://bitbucket.org/larc_cbr_2d_simulation/larc-cbr-2020-simulation-2d/src/master/)
| RoboCup 2019 | CYRUS2019, FCPGPR2019, FRAUnited2019, HFutEngine2019, Hillstone2019, MT2019, Razi2019, Rione2019, Yushan2019 | [https://archive.robocup.info/Soccer/Simulation/2D/binaries/RoboCup/2019/Elimination/](https://archive.robocup.info/Soccer/Simulation/2D/binaries/RoboCup/2019/Elimination/) |


## Postgres Cluster

The repo also contains scripts for setting up a PostgreSQL cluster that can be used to manage v1 data using SQL.
New dataset major versions might require different DB schemas.

To set it up, execute the following scripts **in order** in a Postgres 14 cluster (you may use `psql` for this).
You'll need to modify the scripts by choosing a schema name before using them.

1. `db/setup_pg_v1.sql`
2. `db/setup_pg_v1_data.sql`
3. `db/pg_v1_auxiliary.sql`
4. `db/pg_v1_optimizations.sql`

Data can be ingested to the Postgres cluster with the CLI command
```console
python cli.py v1-data copy-all-matches-metadata-to-postgres --hostname=localhost --user=postgres --password="<your password>" --indir="<directory with only match CSVs>"
python cli.py v1-data copy-all-matches-contents-to-postgres --hostname=localhost --user=postgres --password="<your password>" --indir="<directory with all CSVs>"
```

To generate a datasets for training/validation/testing from the PostgreSQL database, use the `db/gen_dataset_indarch.sql` script to output data to STDOUT and then pipe it into a file. For example:
```console
psql --file=./db/gen_dataset_indarch.sql | pv | gzip > dataset.csv.gz
```
here, the `pv` command gives us feedback on the throughput and total outputted data.
