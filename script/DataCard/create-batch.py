# Place it at /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter
# python create-batch.py -l RunList.txt --Asymptotic[--Full][--Q*]
# RunList.txt contains paths of results from text2workspace.py e.g. /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/card_2017_MuMu_M500_HNL_UL.root

import os, sys
import commands as cmd
import argparse
import datetime

parser = argparse.ArgumentParser(description='option')
parser.add_argument('-n', dest='Name', default="")
parser.add_argument('-l', dest='RunList')
parser.add_argument('--Full', action='store_true')
parser.add_argument('--Q1', action='store_true')
parser.add_argument('--Q2', action='store_true')
parser.add_argument('--Q3', action='store_true')
parser.add_argument('--Q4', action='store_true')
parser.add_argument('--Q5', action='store_true')
parser.add_argument('--Asymptotic', action='store_true')
args = parser.parse_args()

pwd = os.getcwd()
CMSSW_BASE = os.environ['CMSSW_BASE']
SCRAM_ARCH = os.environ['SCRAM_ARCH']

failure, result = cmd.getstatusoutput('combine --help')
if failure:
  print "[!!ERROR!!] cannot run combine."
  print "Please set proper cmsenv first."
  print "Exiting ..."
  sys.exit(1)

cards = open(args.RunList).readlines()
NCARD = len(cards)
if args.Name == "":
  args.Name = args.RunList.split('_')[-1].replace('.txt','')

os.system('mkdir -p Batch/'+args.Name)
with open('Batch/submit_skeleton.sh','w') as skel:
  skel.write("universe = vanilla\n")
  skel.write("getenv   = True\n")
  skel.write("should_transfer_files = YES\n")
  skel.write("when_to_transfer_output = ON_EXIT\n")
  skel.write("request_memory = 12000\n")

for i in range(0,NCARD):

  card = cards[i].strip('\n')
  shortcard = card.split('/')[-1].replace(".root","").replace("card_","")

  os.system('mkdir -p Batch/'+args.Name+'/full_CLs/'+shortcard+'/output/')
  os.system('cp Batch/submit_skeleton.sh Batch/'+args.Name+'/full_CLs/'+shortcard+'/submit_Q1.sh')
  os.system('cp Batch/submit_skeleton.sh Batch/'+args.Name+'/full_CLs/'+shortcard+'/submit_Q2.sh')
  os.system('cp Batch/submit_skeleton.sh Batch/'+args.Name+'/full_CLs/'+shortcard+'/submit_Q3.sh')
  os.system('cp Batch/submit_skeleton.sh Batch/'+args.Name+'/full_CLs/'+shortcard+'/submit_Q4.sh')
  os.system('cp Batch/submit_skeleton.sh Batch/'+args.Name+'/full_CLs/'+shortcard+'/submit_Q5.sh')
  os.system('mkdir -p Batch/'+args.Name+'/Asymptotic/'+shortcard+'/output/')
  os.system('cp Batch/submit_skeleton.sh Batch/'+args.Name+'/Asymptotic/'+shortcard+'/submit.sh')

  if args.Full or args.Q1:
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/run_Q1.sh",'w') as runfile:
      runfile.write("#!/bin/bash\n")
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.025 -T 1000\n")
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/submit_Q1.sh",'a') as submitfile:
      submitfile.write("executable = run_Q1.sh\n")
      submitfile.write("log = "+shortcard+"_Q1.log\n")
      submitfile.write("output = "+shortcard+"_Q1.out\n")
      submitfile.write("error = "+shortcard+"_Q1.err\n")
      submitfile.write("transfer_output_files = higgsCombine"+shortcard+".HybridNew.mH120.quant0.025.root\n")
      submitfile.write("transfer_output_remaps = \"higgsCombine"+shortcard+".HybridNew.mH120.quant0.025.root = output/"+shortcard+"_Q1.root\"\n")
      submitfile.write("queue\n")
    os.chdir('Batch/'+args.Name+'/full_CLs/'+shortcard)
    os.system('condor_submit submit_Q1.sh -batch-name '+shortcard+'_'+args.Name+'_Q1')
    os.chdir(pwd)
  if args.Full or args.Q2:
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/run_Q2.sh",'w') as runfile:
      runfile.write("#!/bin/bash\n")
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.160 -T 1000\n")
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/submit_Q2.sh",'a') as submitfile:
      submitfile.write("executable = run_Q2.sh\n")
      submitfile.write("log = "+shortcard+"_Q2.log\n")
      submitfile.write("output = "+shortcard+"_Q2.out\n")
      submitfile.write("error = "+shortcard+"_Q2.err\n")
      submitfile.write("transfer_output_files = higgsCombine"+shortcard+".HybridNew.mH120.quant0.160.root\n")
      submitfile.write("transfer_output_remaps = \"higgsCombine"+shortcard+".HybridNew.mH120.quant0.160.root = output/"+shortcard+"_Q2.root\"\n")
      submitfile.write("queue\n")
    os.chdir('Batch/'+args.Name+'/full_CLs/'+shortcard)
    os.system('condor_submit submit_Q2.sh -batch-name '+shortcard+'_'+args.Name+'_Q2')
    os.chdir(pwd)
  if args.Full or args.Q2:
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/run_Q3.sh",'w') as runfile:
      runfile.write("#!/bin/bash\n")
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.500 -T 1000\n")
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/submit_Q3.sh",'a') as submitfile:
      submitfile.write("executable = run_Q3.sh\n")
      submitfile.write("log = "+shortcard+"_Q3.log\n")
      submitfile.write("output = "+shortcard+"_Q3.out\n")
      submitfile.write("error = "+shortcard+"_Q3.err\n")
      submitfile.write("transfer_output_files = higgsCombine"+shortcard+".HybridNew.mH120.quant0.500.root\n")
      submitfile.write("transfer_output_remaps = \"higgsCombine"+shortcard+".HybridNew.mH120.quant0.500.root = output/"+shortcard+"_Q3.root\"\n")
      submitfile.write("queue\n")
    os.chdir('Batch/'+args.Name+'/full_CLs/'+shortcard)
    os.system('condor_submit submit_Q3.sh -batch-name '+shortcard+'_'+args.Name+'_Q3')
    os.chdir(pwd)
  if args.Full or args.Q4:
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/run_Q4.sh",'w') as runfile:
      runfile.write("#!/bin/bash\n")
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.840 -T 1000\n")
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/submit_Q4.sh",'a') as submitfile:
      submitfile.write("executable = run_Q4.sh\n")
      submitfile.write("log = "+shortcard+"_Q4.log\n")
      submitfile.write("output = "+shortcard+"_Q4.out\n")
      submitfile.write("error = "+shortcard+"_Q4.err\n")
      submitfile.write("transfer_output_files = higgsCombine"+shortcard+".HybridNew.mH120.quant0.840.root\n")
      submitfile.write("transfer_output_remaps = \"higgsCombine"+shortcard+".HybridNew.mH120.quant0.840.root = output/"+shortcard+"_Q4.root\"\n")
      submitfile.write("queue\n")
    os.chdir('Batch/'+args.Name+'/full_CLs/'+shortcard)
    os.system('condor_submit submit_Q4.sh -batch-name '+shortcard+'_'+args.Name+'_Q4')
    os.chdir(pwd)
  if args.Full or args.Q5:
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/run_Q5.sh",'w') as runfile:
      runfile.write("#!/bin/bash\n")
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.975 -T 1000\n")
    with open("Batch/"+args.Name+"/full_CLs/"+shortcard+"/submit_Q5.sh",'a') as submitfile:
      submitfile.write("executable = run_Q5.sh\n")
      submitfile.write("log = "+shortcard+"_Q5.log\n")
      submitfile.write("output = "+shortcard+"_Q5.out\n")
      submitfile.write("error = "+shortcard+"_Q5.err\n")
      submitfile.write("transfer_output_files = higgsCombine"+shortcard+".HybridNew.mH120.quant0.975.root\n")
      submitfile.write("transfer_output_remaps = \"higgsCombine"+shortcard+".HybridNew.mH120.quant0.975.root = output/"+shortcard+"_Q5.root\"\n")
      submitfile.write("queue\n")
    os.chdir('Batch/'+args.Name+'/full_CLs/'+shortcard)
    os.system('condor_submit submit_Q5.sh -batch-name '+shortcard+'_'+args.Name+'_Q5')
    os.chdir(pwd)

  if args.Asymptotic:
    with open("Batch/"+args.Name+"/Asymptotic/"+shortcard+"/run_Asymptotic.sh",'w') as runfile:
      runfile.write("#!/bin/bash\n")
      runfile.write("combine -M AsymptoticLimits "+card+" --run blind\n")
    with open("Batch/"+args.Name+"/Asymptotic/"+shortcard+"/submit.sh",'a') as submitfile:
      submitfile.write("executable = run_Asymptotic.sh\n")
      submitfile.write("log = "+shortcard+"_Asymptotic.log\n")
      submitfile.write("output = "+shortcard+"_Asymptotic.out\n")
      submitfile.write("error = "+shortcard+"_Asymptotic.err\n")
      submitfile.write("transfer_output_files = higgsCombineTest.AsymptoticLimits.mH120.root\n")
      submitfile.write("transfer_output_remaps = \"higgsCombineTest.AsymptoticLimits.mH120.root = output/"+shortcard+"_Asymptotic.root\"\n")
      submitfile.write("queue\n")
    os.chdir('Batch/'+args.Name+'/Asymptotic/'+shortcard)
    os.system('condor_submit submit.sh -batch-name '+shortcard+'_'+args.Name+'_Asymptotic')
    os.chdir(pwd)
