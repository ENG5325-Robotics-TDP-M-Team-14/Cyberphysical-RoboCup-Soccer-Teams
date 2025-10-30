#!/bin/bash
################
## Extract RCL log information into CSV tables using gzcat and rcl2csv.
## 
## This will generate tables for all *.rcg.gz files in the same folder as this script.
##  
################
DIR=`cd $(dirname $0) && pwd`;
cd $DIR;

set -e
count=0
num_files=`ls *.rcl.gz | wc -l`
for filename in `ls *.rcl.gz`; do
    cleanname=${filename%%.*};
    echo "Extracting CSV from ${cleanname} ...";
    gzcat $filename | rcl2csv $DIR $cleanname;
    ((++count));
    echo "Extracted CSVs from ${cleanname}" \($count/$num_files\) \(`date +%H:%M:%S`\);
done

echo "Done!"
