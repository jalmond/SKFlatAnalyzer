analyzer=HNL_ControlRegionPlotter
rundir=HNL_ControlRegionPlotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=2
njobs_data=100
nmax=400
skim=' '

declare  -a era_list=("2017")

if [[ $1 == "TEST" ]]; then

    declare  -a era_list=("2017")
    declare  -a flag_list=("SS_CR")

    for i in "${era_list[@]}"
    do
        for j in "${flag_list[@]}"
        do
            FLAG=$j
            SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private   -n 1 --nmax 1   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG  &
	    
        done
    done
fi




if [[ $1 == "Full" ]]; then

    declare  -a era_list=("2017")

    
    for i in "${era_list[@]}"
    do
        
	#### Data
	SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt  -n 40             --nmax 400       -e ${i}  --skim SkimTree_DileptonBDT    &
	##### Prompt
	SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt  -n 20             --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunPrompt &
	#SKFlat.py -a $analyzer  -l $mcpath/PromptOS.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT   --userflags RunPrompt &
	### SIGNALS
	#SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT &
        #SKFlat.py -a $analyzer  -l $sigpath/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
        #SKFlat.py -a $analyzer  -l $sigpath/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
	### Conv
        SKFlat.py -a $analyzer  -l $mcpath/Conv.txt   -n 10      --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunConv &
	### CF
        SKFlat.py -a $analyzer  -l $mcpath/CF.txt     -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunCF &
        #####   FAKES
	SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunFake  &
        #SKFlat.py -a $analyzer  -l $mcpath/VVOS.txt             -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunFake  &



    done

fi

if [[ $1 == "WZ" ]]; then

    declare  -a era_list=("2017")
    declare  -a flag_list=("VV_VR")

    for i in "${era_list[@]}"
    do
        for j in "${flag_list[@]}"
        do
            FLAG=$j
            SKFlat.py -a $analyzer  -i WZTo3LNu_amcatnlo   -n 100 --nmax 600   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunPrompt  &
	    SKFlat.py -a $analyzer  -i WZTo3LNu_amcatnlo   -n 100 --nmax 600   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG &

        done
    done
fi


if [[ $1 == "" ]]; then

    declare  -a era_list=("2017")
    declare  -a flag_list=("OS_VR" "SS_CR" "VV_VR")

    for i in "${era_list[@]}"
    do
        #### Data                                                                                                                                                                                                                             
	for j in "${flag_list[@]}"
	do
            FLAG=$j
	    if [[ $FLAG == "OS_VR" ]]; then
		SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt       -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunFake  &
		SKFlat.py -a $analyzer  -l $mcpath/VVOS.txt                  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunFake  &     
		SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &                  
		SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &                  
		SKFlat.py -a $analyzer  -l $mcpath/PromptOS.txt             -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunPrompt &         
		SKFlat.py -a $analyzer  -l $mcpath/ConvOS.txt               -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunConv   &         
		SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake   &         
		SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake  &
	    else
		SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100      --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG  &
                SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100      --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG  &
		SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt             -n 20       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunPrompt &
		SKFlat.py -a $analyzer  -l $mcpath/Conv.txt                 -n 10       --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunConv &
		SKFlat.py -a $analyzer  -l $mcpath/CF.txt                   -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunCF &
                SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunFake   &
		SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunFake  &
	    fi
	    
	done
    done

fi

if [[ $1 == "CF" ]]; then

    declare  -a era_list=("2017")
    declare  -a flag_list=( "SS_CR" "VV_VR")

    for i in "${era_list[@]}"
    do
                                                                                                                                                                                                         
        for j in "${flag_list[@]}"
        do
            FLAG=$j
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt   -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags $FLAG,RunCF &
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt   -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags $FLAG,RunCF &
	    SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt   -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags $FLAG,RunCF &
        done
    done

fi




if [[ $1 == "OSEMu" ]]; then

    declare  -a era_list=("2017")
    FLAG="OS_VR"

    for i in "${era_list[@]}"
    do
        #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &
        #SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &
        #SKFlat.py -a $analyzer  -l $mcpath/PromptOS.txt             -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunPrompt &
        #SKFlat.py -a $analyzer  -l $mcpath/ConvOS.txt               -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunConv   &
        #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake  &

    done

fi

