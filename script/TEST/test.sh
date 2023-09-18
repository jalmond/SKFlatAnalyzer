#!/bin/bash

era=$1
isdata=$2
sample=$3
skim=$4
flag=$5

#echo -e ".L test.C\n test(\"$era\",$isdata,\"$sample\",\"$skim\",\"$flag\");" | root -l 2>&1 | tee "$3".log

# HNL_LeptonFakeRate
#echo -e ".L test.C\n test(\"2017\",false,\"QCD_Mu_300\",\"FakeEventSkimBDT\",\"FakeRateTruth\");" | root -l 2>&1 | tee "QCD_Mu_300".log
echo -e ".L test.C\n test(\"2017\",false,\"QCD_Mu_300\",\"FakeEventSkimBDT\",\"\");" | root -l 2>&1 | tee "QCD_Mu_300".log
#echo -e ".L test.C\n test(\"2017\",false,\"TTLJ\",\"HNFakeBDT\",\"FakeRateTruth\");" | root -l 2>&1 | tee "TTLJ".log
#echo -e ".L test.C\n test(\"2017\",false,\"DYJets\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "DYJets".log
#echo -e ".L test.C\n test(\"2017\",false,\"WJets\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "WJets".log

# HNL_SignalRegionPlotter (fake closure)
#echo -e ".L test.C\n test(\"2017\",false,\"TTLJ\",\"HNMultiLepBDT\",\"RunFakeClosurePred\");" | root -l 2>&1 | tee "TTLJ_ClosPred".log
#echo -e ".L test.C\n test(\"2017\",false,\"TTLJ\",\"HNMultiLepBDT\",\"RunFakeClosureObs\");" | root -l 2>&1 | tee "TTLJ_ClosObs".log
