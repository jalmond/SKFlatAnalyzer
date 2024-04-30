analyzer=HNL_ControlRegion_Plotter
rundir=HNL_ControlRegion_Plotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=2
njobs_data=200
nmax=500
skim=' '

if [[ $1 == "" ]]; then

    declare  -a era_list=("2017" "2018" "2016postVFP" "2016preVFP")
    declare  -a era_list=("2016preVFP")

    for i in "${era_list[@]}"
    do

#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt      -n 100   --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &
#SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep,RunPrompt &
#SKFlat.py -a $analyzer  -l $mcpath/Conv.txt                 -n 10        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep,RunConv&

#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt     -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT    --userflags SSMultiLep,RunCF &

#SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunFake  &
#SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100      --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &

#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt      -n 100   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR &
#SKFlat.py -a $analyzer  -l $mcpath/PromptOS.txt             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR &
#SKFlat.py -a $analyzer  -i WJets_MG             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR &
#SKFlat.py -a $analyzer  -l $mcpath/Conv.txt             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR &
#SKFlat.py -a $analyzer  -i SingleTop_sch_Lep                -n 20        --nmax ${nmax}   -e ${i}  --userflags OS_VR &
#SKFlat.py -a $analyzer  -i SingleTop_tch_antitop_Incl       -n 20        --nmax ${nmax}   -e ${i}  --userflags OS_VR &
#SKFlat.py -a $analyzer  -i SingleTop_tch_top_Incl           -n 20        --nmax ${nmax}   -e ${i}  --userflags OS_VR &
#SKFlat.py -a $analyzer  -i SingleTop_tW_antitop_NoFullyHad  -n 20        --nmax ${nmax}   -e ${i}  --userflags OS_VR &
#SKFlat.py -a $analyzer  -i SingleTop_tW_top_NoFullyHad      -n 20        --nmax ${nmax}   -e ${i}  --userflags OS_VR &

#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR,RunFake &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR,RunFake &
#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt      -n 100   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR,RunFake &
#SKFlat.py -a $analyzer  -l $mcpath/PromptOS.txt             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags OS_VR,RunFake &

SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags SS_CR &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags SS_CR &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt      -n 100   --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags SS_CR &
SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags SS_CR,RunPrompt &
SKFlat.py -a $analyzer  -l $mcpath/Conv.txt                 -n 10        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags SS_CR,RunConv &

SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SS_CR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SS_CR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt     -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SS_CR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT    --userflags SS_CR,RunCF &

SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt      -n 100   --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR &
SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR,RunPrompt &
SKFlat.py -a $analyzer  -l $mcpath/Conv.txt                 -n 10        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR,RunConv&

SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt     -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags LLL_VR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT    --userflags LLL_VR,RunCF &

SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt      -n 100   --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR &
SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR,RunPrompt &
SKFlat.py -a $analyzer  -l $mcpath/Conv.txt                 -n 10        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR,RunConv&

SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt     -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags VBF_CR,RunFake   &
SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT    --userflags VBF_CR,RunCF &

    done
    
fi
