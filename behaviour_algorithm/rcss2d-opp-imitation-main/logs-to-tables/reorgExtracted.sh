#!/bin/bash
################
## Group *.rcg.gz, *.rcl.gz and *.csv.gz files per match and reorganize them in folders
## 
## This will generate tables for all *.rcg.gz files in the same folder as this script.
##  
################

DIR=`cd $(dirname $0) && pwd`;
cd $DIR;

mkdir -vp raw/
mkdir -vp completecsv/

count=0
num_files=`ls *.rcg.gz | wc -l`
for filename in `ls *.rcg.gz`; do
    cleanname=${filename%%.*};
    mkdir -vp raw/$cleanname
    mv ${cleanname}.rcg.gz raw/$cleanname/
    mv ${cleanname}.rcl.gz raw/$cleanname/
    mkdir -vp completecsv/$cleanname
    mv ${cleanname}.match.csv completecsv/$cleanname/
    mv ${cleanname}.serverparams.csv completecsv/$cleanname/
    mv ${cleanname}.playerparams.csv completecsv/$cleanname/
    mv ${cleanname}.playertypes.csv completecsv/$cleanname/
    mv ${cleanname}.dash.csv completecsv/$cleanname/
    mv ${cleanname}.turn.csv completecsv/$cleanname/
    mv ${cleanname}.kick.csv completecsv/$cleanname/
    mv ${cleanname}.tackle.csv completecsv/$cleanname/
    ((++count));
    echo "Moved ${cleanname}" \($count/$num_files\) \(`date +%H:%M:%S`\);
done

echo "Done!"
