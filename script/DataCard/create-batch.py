# python create-batch.py -l RunList.txt

import os
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

cards = open(args.RunList).readlines()
NCARD = len(cards)

#JobStartTime = datetime.datetime.now()
#timestamp =  JobStartTime.strftime('%Y_%m_%d_%H%M%S')
#
#JobName = timestamp
#if args.Name!="":
#  JobName += "__"+args.Name
#  if args.Q1:
#    JobName += "__025"
#  if args.Q2:
#    JobName += "__160"
#  if args.Q3:
#    JobName += "__500"
#  if args.Q4:
#    JobName += "__840"
#  if args.Q5:
#    JobName += "__975"
#    
#print '#### Job directory :'
#print 'cd '+JobName
#print 'condor_submit submit.sh'
#print 'cd ..'
#
#FullPathJobDir = pwd+'/'+JobName+'/'
#os.system('mkdir -p '+JobName+'/output/')
#
##### first write the executable
#
#executable_filename = 'Combine_'+JobName+'.sh'
#executable = open(pwd+'/'+JobName+'/'+executable_filename,'w')
#print>>executable,'''#!/bin/bash
#SECTION=`printf $1`
#WORKDIR=`pwd`
##### Don't make root history
#export ROOT_HIST=0
##### use cvmfs for root ####
#export CMS_PATH=/cvmfs/cms.cern.ch
#source $CMS_PATH/cmsset_default.sh
#export SCRAM_ARCH={0}
#cd {1}/src/
#echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
#echo "@@@@ cmsswrel = "$cmsswrel
#echo "@@@@ scram..."
#eval `scramv1 runtime -sh`
#cd -
#source {2}/run_${{SECTION}}.sh
#'''.format(SCRAM_ARCH,CMSSW_BASE,pwd+'/'+JobName)
#executable.close()
#
##### Write jds
#
#jdsfile = open(pwd+'/'+JobName+'/submit.sh','w')
#print>>jdsfile,'''executable = {1}
#universe   = vanilla
#arguments  = $(Process)
#log = condor.log
#getenv     = True
#should_transfer_files = YES
#when_to_transfer_output = ON_EXIT
#output = job_$(Process).log
#error = job_$(Process).err
#request_memory = 12000
##concurrency_limits = n150.jihunk
##transfer_output_files = higgsCombineTest.AsymptoticLimits.mH120.root
##transfer_output_remaps = "higgsCombineTest.AsymptoticLimits.mH120.root = output/hists_$(Process).root"
#queue {0}
#'''.format(str(NCARD), executable_filename)
##if args.Full:
#jdsfile.close()

#### write job sh file



os.system('mkdir -p Batch')
with open('Batch/submit_skeleton.sh','w') as skel:
  skel.write("universe = vanilla\n")
  skel.write("getenv   = True\n")
  skel.write("should_transfer_files = YES\n")
  skel.write("when_to_transfer_output = ON_EXIT\n")
  skel.write("request_memory = 12000\n")
  #skel.write("queue\n")

for i in range(0,NCARD):

  card = cards[i].strip('\n')
  shortcard = card.split('/')[-1].replace(".root","").replace("card_","")

  os.system('mkdir -p Batch/full_CLs/'+shortcard+'/output/')
  os.system('cp Batch/submit_skeleton.sh Batch/full_CLs/'+shortcard+'/submit.sh')
  os.system('mkdir -p Batch/Asymptotic/'+shortcard+'/output/')
  os.system('cp Batch/submit_skeleton.sh Batch/Asymptotic/'+shortcard+'/submit.sh')

  if args.Full or args.Q1:
    with open("Batch/full_CLs/"+shortcard+"/run_full_CLs_Q1.sh",'w') as runfile:
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.025 -T 1000\n")
    with open("Batch/full_CLs/"+shortcard+"/submit_skeleton.sh",'a') as submitfile:
      submitfile.write("log = "+shortcard+"_Q1.log")
      submitfile.write("output = "+shortcard+"_Q1.out")
      submitfile.write("error = "+shortcard+"_Q1.err")
      #submitfile.write("transfer_output_files = higgsCombineTest.AsymptoticLimits.mH120.root")
      #submitfile.write("transfer_output_remaps = \"higgsCombineTest.AsymptoticLimits.mH120.root = output/hists_$(Process).root\"")
  if args.Full or args.Q2:
    with open("Batch/full_CLs/"+shortcard+"/run_full_CLs_Q2.sh",'w') as runfile:
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.160 -T 1000\n")
  if args.Full or args.Q3:
    with open("Batch/full_CLs/"+shortcard+"/run_full_CLs_Q3.sh",'w') as runfile:
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.500 -T 1000\n")
  if args.Full or args.Q4:
    with open("Batch/full_CLs/"+shortcard+"/run_full_CLs_Q4.sh",'w') as runfile:
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.840 -T 1000\n")
  if args.Full or args.Q5:
    with open("Batch/full_CLs/"+shortcard+"/run_full_CLs_Q5.sh",'w') as runfile:
      runfile.write("combine -M HybridNew --LHCmode LHC-limits "+card+" -n "+shortcard+" --saveHybridResult --expectedFromGrid 0.975 -T 1000\n")

  if args.Asymptotic:
    with open("Batch/Asymptotic/"+shortcard+"/run_Asymptotic.sh",'w') as runfile:
      runfile.write("#!/bin/bash\n")
      runfile.write("combine -M AsymptoticLimits "+card+" --run blind\n")
    with open("Batch/Asymptotic/"+shortcard+"/submit.sh",'a') as submitfile:
      submitfile.write("executable = run_Asymptotic.sh\n")
      submitfile.write("log = "+shortcard+"_Asymptotic.log\n")
      submitfile.write("output = "+shortcard+"_Asymptotic.out\n")
      submitfile.write("error = "+shortcard+"_Asymptotic.err\n")
      submitfile.write("transfer_output_files = higgsCombineTest.AsymptoticLimits.mH120.root\n")
      submitfile.write("transfer_output_remaps = \"higgsCombineTest.AsymptoticLimits.mH120.root = output/"+shortcard+"_Asymptotic.root\"\n")
      submitfile.write("queue\n")
    os.chdir('Batch/Asymptotic/'+shortcard)
    os.system('condor_submit submit.sh -batch-name test')
    os.chdir(pwd)


















#  runfile = open(pwd+'/'+JobName+'/run_'+str(i)+'.sh','w')
#
#  os.system('mkdir -p Batch_output/'+shortcard)
#
#  runfile.write('echo "Input datacard '+shortcard+'"\n')
#  
#  _quant = "0.500"
#  #0.025 0.160 0.500 0.840 0.975
#  if args.Q1:
#    _quant = "0.025"
#  if args.Q2:
#    _quant = "0.160"
#  if args.Q3:
#    _quant = "0.500"
#  if args.Q4:
#    _quant = "0.840"
#  if args.Q5:
#    _quant = "0.975"
#
#    
#  #### Full CLs
#  if args.Full:
#    print>>runfile,'''for quant in {2}
#do
#  echo "#### quant = "$quant
#  #combine -M HybridNew --frequentist --testStat LHC {0} -n {1}_exp$quant --expectedFromGrid $quant -T 100
#  #combine -M HybridNew --testStat LHC {0} -n {1}_exp$quant --expectedFromGrid $quant -T 1000 
#  combine -M HybridNew --LHCmode LHC-limits {0} -n {1} --saveHybridResult --expectedFromGrid $quant -T 1000 
#done
#'''.format(card,shortcard, _quant)
#
#  #### AymptoticLimits
#  else:
#    runfile.write('combine -M AsymptoticLimits '+card+' --run blind \n')
#    #runfile.write('combine -M AsymptoticLimits '+card+'\n')
#
#
#  runfile.close()
#
