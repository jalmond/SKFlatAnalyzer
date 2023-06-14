#!/bin/bash

era=$1
isdata=$2
sample=$3
skim=$4
flag=$5

echo -e ".L test.C\n test(\"$era\",$isdata,\"$sample\",\"$skim\",\"$flag\");" | root -l 2>&1 | tee "$3".log
