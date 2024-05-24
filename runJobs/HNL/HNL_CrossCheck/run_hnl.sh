analyzer=HNL_CrossCheck
rundir=HNL_CrossCheck
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=2
njobs_data=200
nmax=600
skim=' '

if [[ $1 == "" ]]; then

    declare  -a era_list=("2017")

    for i in "${era_list[@]}"
    do
        #### Data                                                                                                                                                                             
        SKFlat.py -a $analyzer  -i DYJets_MG          -n 200        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
        SKFlat.py -a $analyzer  -i DYJets          -n 200        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
        SKFlat.py -a $analyzer  -i DYJetsToTauTau_MiNNLO          -n 200        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
        SKFlat.py -a $analyzer  -i DYJetsToEE_MiNNLO          -n 200        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -i DYJetsToMuMu_MiNNLO          -n 200        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -i ZGToLLG          -n 50        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	

    done
    
fi
