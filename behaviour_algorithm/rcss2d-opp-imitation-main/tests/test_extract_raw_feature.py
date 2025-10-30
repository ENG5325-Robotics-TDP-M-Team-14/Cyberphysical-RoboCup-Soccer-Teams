import os
import pandas as pd
from pathlib import Path
import pytest
from termcolor import cprint

from tasks.v1.data import extract_raw_features

HERE = Path(os.path.dirname(os.path.realpath(__file__)))

class TestExtractRawFeatures:

    TESTFILES_DIRPATH = HERE / 'data'
    INPUTNAME_TEMPLATE = 'test.%s.csv'
    INPUTNAME_GZ_TEMPLATE = 'test.%s.csv.gz'

    @pytest.mark.asyncio
    async def test_dash(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'dash'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        assert set(df.columns) == {'running_time', 'stopped_time', 'teamname', 'unum', 'dash_power', 'dash_direction'}
    
    @pytest.mark.asyncio
    async def test_kick(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'kick'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        assert set(df.columns) == {'running_time', 'stopped_time', 'teamname', 'unum', 'kick_power', 'kick_direction'}
    
    @pytest.mark.asyncio
    async def test_turn(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'turn'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        assert set(df.columns) == {'running_time', 'stopped_time', 'teamname', 'unum', 'turn_moment'}
    
    @pytest.mark.asyncio
    async def test_tackle(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'tackle'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        assert set(df.columns) == {'running_time', 'stopped_time', 'teamname', 'unum', 'tackle_direction'}
    
    @pytest.mark.asyncio
    async def test_match(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'match'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        assert set(df.columns) == {
            ' cycle', 
            ' stopped', 
            ' playmode', 
            ' l_name', 
            ' r_name', 
            ' b_x',
            ' b_y',
            ' b_vx',
            ' b_vy',
            *[ f" {side}{i}_{player_feature}"
                for side in ['l', 'r'] 
                    for i in range(1,12) 
                        for player_feature in [
                            't',
                            'goalie',
                            'discarded',
                            'x',
                            'y',
                            'vx',
                            'vy',
                            'body',
                            'stamina',
                            'stamina_cap'
                        ] 
            ]
        }
    
    @pytest.mark.asyncio
    async def test_playertypes(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'playertypes'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        assert set(df.columns) == {
            'id',
            'dash_power_rate',
            'player_decay',
            'inertia_moment',
            'kickable_margin',
            'kick_rand',
            'extra_stamina',
            'effort_min',
            'effort_max'
        }
    
    @pytest.mark.asyncio
    async def test_input_compression(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'playertypes'
        inputfilename_compressed = TestExtractRawFeatures.INPUTNAME_GZ_TEMPLATE % 'playertypes'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename_compressed, False, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        df_input_compressed: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        assert df.equals(df_input_compressed)

    @pytest.mark.asyncio
    async def test_output_compression(self, tmpdir):
        inputfilename = TestExtractRawFeatures.INPUTNAME_TEMPLATE % 'playertypes'
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, False, tmpdir)
        await extract_raw_features(TestExtractRawFeatures.TESTFILES_DIRPATH / inputfilename, True, tmpdir)
        outputfilename = inputfilename
        df: pd.DataFrame = pd.read_csv(tmpdir / outputfilename)
        df_output_compressed: pd.DataFrame = pd.read_csv(tmpdir / (outputfilename + '.gz'))
        assert df.equals(df_output_compressed)
        
