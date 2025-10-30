#!/bin/bash
################
## Extract RCG log information into CSV tables using rcg2csv.
## 
## This will generate tables for all *.rcg.gz files in the same folder as this script.
##  
################

DIR=`cd $(dirname $0) && pwd`;
cd $DIR;

set -e
count=0
num_files=`ls *.rcg.gz | wc -l`
for filename in `ls *.rcg.gz`; do
    cleanname=${filename%%.*};
    rcg2csv -m -mo $cleanname.match.csv -sp -spo $cleanname.serverparams.csv -pp -ppo $cleanname.playerparams.csv -pt -pto $cleanname.playertypes.csv $filename;
    ((++count));
    echo "Extracted CSVs from ${cleanname}" \($count/$num_files\) \(`date +%H:%M:%S`\);
done

echo "Done!"
