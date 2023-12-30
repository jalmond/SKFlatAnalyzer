#!/bin/bash

era=$1
isdata=$2
sample=$3
skim=$4
flag=$5

#echo -e ".L test.C\n test(\"$era\",$isdata,\"$sample\",\"$skim\",\"$flag\");" | root -l 2>&1 | tee "$3".log

# HNL_LeptonFakeRate
#echo -e ".L test.C\n test(\"2017\",false,\"QCD_Mu_300\",\"FakeEventSkimBDT\",\"FakeRateTruth\");" | root -l 2>&1 | tee "QCD_Mu_300".log
#echo -e ".L test.C\n test(\"2017\",false,\"QCD_Mu_300\",\"FakeEventSkimBDT\",\"\");" | root -l 2>&1 | tee "QCD_Mu_300".log
#echo -e ".L test.C\n test(\"2017\",false,\"TTLJ\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "TTLJ".log
#echo -e ".L test.C\n test(\"2017\",false,\"DYJets\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "DYJets".log
#echo -e ".L test.C\n test(\"2017\",false,\"WJets\",\"HNFakeBDT\",\"\");" | root -l 2>&1 | tee "WJets".log

# HNL_SignalRegionPlotter
#echo -e ".L test.C\n test(\"2017\",true,\"DoubleMuon\",\"HNMultiLepBDT\",\"RunFake\");" | root -l 2>&1 | tee "DoubleMuon_RunFake".log
#echo -e ".L test.C\n test(\"2017\",true,\"DoubleEG\",\"HNMultiLepBDT\",\"RunFake\");" | root -l 2>&1 | tee "DoubleEG_RunFake".log
#echo -e ".L test.C\n test(\"2017\",true,\"MuonEG\",\"HNMultiLepBDT\",\"RunFake\");" | root -l 2>&1 | tee "MuonEG_RunFake".log
#echo -e ".L test.C\n test(\"2017\",false,\"DYJets\",\"HNMultiLepBDT\",\"RunCF\");" | root -l 2>&1 | tee "DYJets_RunCF".log
#echo -e ".L test.C\n test(\"2017\",true,\"DoubleEG\",\"DileptonBDT\",\"RunCF\");" | root -l 2>&1 | tee "DoubleEG_RunCF".log
echo -e ".L test.C\n test(\"2017\",false,\"DYTypeI_DF_M1000\",\"HNMultiLepBDT\",\"\");" | root -l 2>&1 | tee "DYTypeI_DF_M1000".log
echo -e ".L test.C\n test(\"2017\",false,\"VBFTypeI_DF_M1000\",\"HNMultiLepBDT\",\"\");" | root -l 2>&1 | tee "VBFTypeI_DF_M1000".log
#echo -e ".L test.C\n test(\"2017\",false,\"SSWWTypeI_DF_M1000\",\"HNMultiLepBDT\",\"\");" | root -l 2>&1 | tee "SSWWTypeI_DF_M1000".log
#echo -e ".L test.C\n test(\"2017\",false,\"SSWWTypeI_SF_M1000\",\"HNMultiLepBDT\",\"\");" | root -l 2>&1 | tee "SSWWTypeI_SF_M1000".log

# HNL_SignalRegionPlotter (fake closure)
#echo -e ".L test.C\n test(\"2017\",false,\"TTLJ\",\"HNMultiLepBDT\",\"RunFakeClosurePred\");" | root -l 2>&1 | tee "TTLJ_ClosPred".log
#echo -e ".L test.C\n test(\"2017\",false,\"TTLJ\",\"HNMultiLepBDT\",\"RunFakeClosureObs\");" | root -l 2>&1 | tee "TTLJ_ClosObs".log

# HNL_ControlRegionPlotter
#echo -e ".L test.C\n test(\"2017\",true,\"DoubleMuon\",\"HNMultiLepBDT\",\"RunFake\");" | root -l 2>&1 | tee "DoubleMuon_RunFake".log
#echo -e ".L test.C\n test(\"2017\",true,\"DoubleMuon\",\"HNMultiLepBDT\",\"SS_CR\");" | root -l 2>&1 | tee "DoubleMuon_SS_CR".log
