analyzer=HNL_ControlRegion_Plotter
rundir=HNL_ControlRegion_Plotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=2
njobs_data=200
nmax=800
skim=' '


if [[ $1 == "Resubmit" ]]; then

    declare  -a era_list=("2018")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 300    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags SSMultiLep,RunCF
    done
fi

if [[ $1 == "Main" ]]; then

    declare  -a era_list=("2017" "2016postVFP" "2016preVFP" "2018")

    for i in "${era_list[@]}"
    do
        #### Data                                                                                                                                                                             	
	SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &
	SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep 
	
	#### Prompt
	SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
	SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS2.txt            -n 200       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
	
	### Conv
	SKFlat.py -a $analyzer  -l $mcpath/Conv/ConvWG.txt          -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags SSMultiLep,RunConv&
	SKFlat.py -a $analyzer  -l $mcpath/Conv/Conv.txt          -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags SSMultiLep,RunConv&

	### Fakes
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &
	SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   
	
	### CF
	SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags SSMultiLep,RunCF 

	
    done
    
fi


if [[ $1 == "Plots" ]]; then

    declare  -a era_list=("2017" "2016postVFP" "2016preVFP" "2018")

    for i in "${era_list[@]}"
    do
        #### Data                                                                                                                                                                                                                                                             
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep,Plots &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep,Plots &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep,Plots

        #### Prompt                                                                                                                                                                                                                                                           
        SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,Plots,RunPrompt &
        SKFlat.py -a $analyzer  -l $mcpath/Prompt/PromptSS2.txt            -n 200       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,Plots,RunPrompt &

        ### Conv                                                                                                                                                                                                                                                              
        SKFlat.py -a $analyzer  -l $mcpath/Conv/ConvWG.txt          -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT --userflags SSMultiLep,Plots,RunConv&
        SKFlat.py -a $analyzer  -l $mcpath/Conv/Conv.txt          -n 50        --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags SSMultiLep,Plots,RunConv&

        ### Fakes                                                                                                                                                                                                                                                             
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,Plots,RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,Plots,RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,Plots,RunFake

        ### CF                                                                                                                                                                                                                                                                
        SKFlat.py -a $analyzer  -l $datapath/DL/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags SSMultiLep,Plots,RunCF


    done

fi
