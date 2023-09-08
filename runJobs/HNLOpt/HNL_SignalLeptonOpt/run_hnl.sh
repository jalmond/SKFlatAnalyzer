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
nmax=300


if [[ $1 == "" ]]; then
    
    SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private  -n 1  --nmax 10   -e 2018 --skim SkimTree_HNMultiLepBDT  --userflags 'RunEE,ELID_Full,Scan1' &
    
fi


if [[ $1 == "ELECTRON_Full_Scan" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP"  "2017" "2018")

    for i in "${era_list[@]}"
    do

        Flag='RunEE,ELID_FullScan,ScanEC'
        source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}
        Flag='RunEE,ELID_FullScan,ScanBB'
        source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}

    done
fi

if [[ $1 == "ELECTRON_POG" ]]; then
    
    declare  -a era_list=("2016postVFP" "2016preVFP"  "2017" "2018")
    
    for i in "${era_list[@]}"
    do
	
        Flag='RunEE,ELID_POGScan'
        source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}
        source ${runPATH}/run_hnl.sh ElectronDATA ${Flag} ${i}


    done
fi

if [[ $1 == "ELECTRON_POG_DATA" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP"  "2017" "2018")

    for i in "${era_list[@]}"
    do
	
        Flag='RunEE,ELID_POGScan'
        source ${runPATH}/run_hnl.sh ElectronDATA ${Flag} ${i}


    done
fi


#############################################################################                                                                                                                                                               
#############################################################################                                                                                                                                                               
#############################################################################     




if [[ $1 == "Electron" ]]; then

    # If no inut then this is ran                                                                                                                        
    source ${runPATH}/run_hnl.sh SignalsID ${3} ${2} &
    
    CFFlag=${2}',RunCF'
    source ${runPATH}/run_hnl.sh CF ${3} ${CFFlag}  &
    ConvFlag=${2}',RunConv'
    source ${runPATH}/run_hnl.sh Conv ${3} ${ConvFlag}  &
   
    FakeFlag=${2}',RunFake'
    source ${runPATH}/run_hnl.sh Fake  ${3} ${FakeFlag}  &
    PromptFlag=${2}',RunPrompt'
    source ${runPATH}/run_hnl.sh Prompt  ${3} ${PromptFlag}  


fi

if [[ $1 == "ElectronDATA" ]]; then

    # If no inut then this is ran                                                                                                                                               
    source ${runPATH}/run_hnl.sh DATA ${3} ${2} &

fi



if [[ $1 == "DATA" ]]; then

    SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${2}  --skim SkimTree_HNMultiLepBDT  --userflags ${3} &                                                        
fi

if [[ $1 == "CF" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/CF.txt  -n $njobs  --nmax ${nmax}   -e ${2} --skim SkimTree_HNMultiLepBDT  --userflags ${3} &
    #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} --userflags RunCF  &                                 

fi


if [[ $1 == "Fake" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/FakeOptAll.txt     -n $njobs       --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/FakeOptLarge.txt  -n $nLargejobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/FakeOptXLarge.txt -n 100          --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &

fi

if [[ $1 == "Prompt" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/MCOpt.txt  -n $njobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/MCOptLarge.txt  -n $nLargejobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT 

fi


if [[ $1 == "SignalsID" ]]; then

    SKFlat.py -a $analyzer  -l $sigpath/DYOptSmall.txt    -n 25  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/SSWWOptSmall.txt  -n 25  --nmax ${nmax}  -e ${2}  --userflags ${3}  --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/VBFOptSmall.txt   -n 25  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&

fi


if [[ $1 == "Conv" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n 5  --nmax ${nmax}   -e ${2} --skim SkimTree_HNMultiLepBDT  --userflags ${3}
fi



