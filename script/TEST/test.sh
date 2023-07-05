#!/bin/bash

era=$1
isdata=$2
sample=$3
skim=$4
flag=$5

#echo -e ".L test.C\n test(\"$era\",$isdata,\"$sample\",\"$skim\",\"$flag\");" | root -l 2>&1 | tee "$3".log
echo -e ".L test.C\n test(\"2017\",false,\"TTLJ\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "TTLJ".log
#echo -e ".L test.C\n test(\"2017\",false,\"DYJets\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "DYJets".log
#echo -e ".L test.C\n test(\"2017\",false,\"WJets\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "WJets".log
