analyzer=HNL_SignalRegion_Plotter
rundir=HNL_SignalRegion_Plotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=5
njobs_sig_syst=20
njobs_data=100
nmax=400
skim=' '

SKFlat.py -a $analyzer  -i DYTypeI_DF_M600_private   -n 20 --nmax 600  -e 2018  --skim SkimTree_HNMultiLepBDT  --userflags RunSyst&

if [[ $1 == "SYST" ]]; then
    
    ##### SET Era here 
    declare  -a era_list=("2016preVFP")
    nmax=500

    for i in "${era_list[@]}"
    do
	
	SKFlat.py -a $analyzer  -l $sigpath/Private/SSWW.txt  -n $njobs_sig_syst  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunSyst&
        SKFlat.py -a $analyzer  -l $sigpath/Private/DY.txt    -n $njobs_sig_syst  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunSyst&
        SKFlat.py -a $analyzer  -l $sigpath/Private/VBF.txt   -n $njobs_sig_syst  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunSyst&
	
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunSyst,RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunSyst,RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunSyst,RunFake   &

        SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunSyst,RunPrompt &
        SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS2.txt            -n 500       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunSyst,RunPrompt &
        SKFlat.py -a $analyzer  -l $mcpath/Conv/Conv.txt                 -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunSyst,RunConv &
        SKFlat.py -a $analyzer  -l $mcpath/Conv/ConvWG.txt                 -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags RunSyst,RunConv &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 200    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags RunSyst,RunCF

    done
fi

