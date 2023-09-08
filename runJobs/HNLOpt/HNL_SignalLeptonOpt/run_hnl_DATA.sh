### SETUP RUN OPTIONS
analyzer=HNL_SignalLeptonOpt
rundir=HNLOpt/HNL_SignalLeptonOpt

# PATHS                                                                                                                                                    
runPATH=${SKFlat_WD}/runJobs/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

#SKIM                                                                                                                                                     
skim=' '

# JOB CONFIG                                                                                                                                               
njobs=25
nLargejobs=50
njobs_sig=30
njobs_data=10
nmax=350



#SKFlat.py -a $analyzer  -l $datapath/2018_DiLepton_EE.txt -n ${njobs_data}  --nmax ${nmax}   -e 2018 --userflags RunEE  --skim  SkimTree_HNMultiLepBDT
SKFlat.py -a $analyzer  -l $datapath/2018_DiLepton_MuMu.txt  -n ${njobs_data}  --nmax ${nmax}  -e 2018   --skim SkimTree_HNMultiLepBDT

