import os,ROOT

cwd = os.getcwd()

filename = 'samples_WR_2018.txt'
Year = "2018"

SKFlat_WD = os.environ['SKFlat_WD']
SKFlatV = os.environ['SKFlatV']
DATA_DIR = SKFlat_WD+'/data/'+SKFlatV+'/'

PDs = open(filename).readlines()

#PDs = ['WRtoNLtoLLJJ_WR4000_N2000']

#### jds
out_jds = open('jobdir/submit.jds','w')
print>>out_jds,'''executable = CheckEmptyPDFFiles.sh
universe   = vanilla
arguments  = $(Process)
log = condor.log
getenv     = True
output = job_$(Process).log
error = job_$(Process).err
queue {0}
'''.format(len(PDs))
out_jds.close()

#### executalbe
out_exe = open('jobdir/CheckEmptyPDFFiles.sh','w')
print>>out_exe,'''
#!/bin/bash
SECTION=`printf $1`
WORKDIR=`pwd`

#### make sure use C locale
export LC_ALL=C

#### Don't make root history
export ROOT_HIST=0

#### use cvmfs for root ####
export CMS_PATH=/cvmfs/cms.cern.ch
source $CMS_PATH/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc700
export cmsswrel=cmssw-patch/CMSSW_10_4_0_patch1
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/src
echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
echo "@@@@ cmsswrel = "$cmsswrel
echo "@@@@ scram..."
eval `scramv1 runtime -sh`
cd -
source /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/external/$SCRAM_ARCH/bin/thisroot.sh

source {0}/run_${{SECTION}}.sh

'''.format(cwd+'/jobdir')

#### make run_XXX.C

for i_PD in range(0,len(PDs)):

  PD = PDs[i_PD]

  if "#" in PD:
    continue

  PD = PD.strip('\n')
  PD = PD.replace(':','_')
  Files = open(DATA_DIR+'/'+Year+'/Sample/ForSNU/'+PD+'.txt')

  #print '#### Checking '+PD

  out_run = open(cwd+'/jobdir/run_'+str(i_PD)+'.sh','w')

  for File in Files:

    filepath_access = File.strip('\n')

    cmd = 'root -l -b -q "/data6/Users/jskim/SKFlatAnalyzer/script/SignalEmptyPDFChecker/run.C(\\\"%s\\\")"\n'%filepath_access
    out_run.write(cmd)
  out_run.close()
