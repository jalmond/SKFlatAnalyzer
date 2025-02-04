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
nmax=700
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")


if [[ $1 == "DY" ]]; then


    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i DYTypeI_DF_M600_private   -n 20 --nmax 600  -e ${i}  --skim SkimTree_HNMultiLepBDT &
	SKFlat.py -a $analyzer  -i VBFTypeI_DF_M600_private   -n 20 --nmax 600  -e ${i}  --skim SkimTree_HNMultiLepBDT &
        SKFlat.py -a $analyzer  -i DYTypeI_DF_M1000_private   -n 20 --nmax 600  -e ${i}  --skim SkimTree_HNMultiLepBDT &
	SKFlat.py -a $analyzer  -i SSWWTypeI_SF_M1000_private   -n 20 --nmax 600  -e ${i}  --skim SkimTree_HNMultiLepBDT &
    done
fi


if [[ $1 == "SIG" ]]; then

    declare  -a era_list=("2018")
    for i in "${era_list[@]}"
    do
	
	SKFlat.py -a $analyzer  -l $sigpath/Private/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Private/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Private/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
        SKFlat.py -a $analyzer  -l $sigpath/Private/Weinberg.txt   -n 40  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Official/SSWWOfficial.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Official/DYOfficial.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
        SKFlat.py -a $analyzer  -l $sigpath/Official/VBFOfficial.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &

    done
    
fi


if [[ $1 == "Main" ]]; then

    declare  -a era_list=("2018")
    #"2016preVFP")

    for i in "${era_list[@]}"
    do
	
        SKFlat.py -a $analyzer  -l $sigpath/Private/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT &
	SKFlat.py -a $analyzer  -l $sigpath/Private/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
        SKFlat.py -a $analyzer  -l $sigpath/Private/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &

	SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunFake   &
	SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunFake   &

	SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS.txt             -n 50        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunPrompt &
        SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS2.txt            -n 200       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunPrompt &
        SKFlat.py -a $analyzer  -l $mcpath/Conv/Conv.txt                 -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunConv & 
        SKFlat.py -a $analyzer  -l $mcpath/Conv/ConvWG.txt                 -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags RunConv & 
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 200    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags RunCF 

	
    done
    
fi




if [[ $1 == "CutFlow" ]]; then

    nmax=750
    declare  -a era_list=("2016preVFP" "2016postVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -l $sigpath/PrivateCutFlow/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT &
        SKFlat.py -a $analyzer  -l $sigpath/PrivateCutFlow/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
        SKFlat.py -a $analyzer  -l $sigpath/PrivateCutFlow/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &

        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags RunFake   &

        #SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 200  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags RunOS   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags RunOS   &
        #SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags RunOS  


	SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS.txt             -n 50        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunPrompt &
        SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS2.txt            -n 200       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunPrompt &
	SKFlat.py -a $analyzer  -l $mcpath/Conv/Conv.txt                 -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunConv &
	SKFlat.py -a $analyzer  -l $mcpath/Conv/ConvWG.txt                 -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags RunConv &
	SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 400    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags RunCF &


    done

fi
