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


if [[ $1 == "" ]]; then

    declare  -a era_list=("2017" )

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i DYTypeI_DF_M600_private    -n  10   --nmax 600   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags 'HF_Bin1,FullEta,MuID_NP' &

    done
fi




#############################################################################                                                                                                                                                               
#############################################################################                                                                                                                                                               
#############################################################################     



if [[ $1 == "Muon_POG" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
    for i in "${era_list[@]}"
    do
        Flag='Muon_POG'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}
    done
fi




if [[ $1 == "MUON_SEL" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
    for i in "${era_list[@]}"
    do
        Flag='Mu_SelectionOptSR1'
        source ${runPATH}/run_hnl_muon.sh Muon ${Flag} ${i}
	Flag='Mu_SelectionOptSR2'
        source ${runPATH}/run_hnl_muon.sh Muon ${Flag} ${i}
        Flag='Mu_SelectionOptSR3'
        source ${runPATH}/run_hnl_muon.sh Muon ${Flag} ${i}
    done

fi



if [[ $1 == "NP_MUON_HF" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
    declare  -a era_list=("2017")
    for i in "${era_list[@]}"
    do
        Flag='HF_Bin1,FullEta,MuID_NP'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}
    done
fi

if [[ $1 == "NP_MUON_HF_Bin2" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

        Flag='HF_Bin2,FullEta,MuID_NP'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}

    done

fi


if [[ $1 == "NP_MUON_HF_Bin3" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

        Flag='HF_Bin3,FullEta,MuID_NP,SIP'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}
	Flag='HF_Bin3,FullEta,MuID_NP,DXY'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}
	Flag='HF_Bin3,FullEta,MuID_NP,DZ'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}

	
    done

fi



if [[ $1 == "NP_MUON_LF_Bin1" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

        Flag='LF_Bin1,MuID_NP,BB'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}
	Flag='LF_Bin1,MuID_NP,EC'
        source ${runPATH}/run_hnl_muon.sh MuonAll ${Flag} ${i}

    done

fi






#############################################################################
#############################################################################
#############################################################################



if [[ $1 == "MuonAll" ]]; then


    source ${runPATH}/run_hnl_muon.sh SignalsID ${3} ${2}

    FakeFlag=${2}',RunFake'
    source ${runPATH}/run_hnl_muon.sh FakeAll  ${3} ${FakeFlag}
    PromptFlag=${2}',RunPrompt'
    source ${runPATH}/run_hnl_muon.sh Prompt  ${3} ${PromptFlag}
    ConvFlag=${2}',RunConv'
    source ${runPATH}/run_hnl_muon.sh Conv ${3} ${ConvFlag}

fi



if [[ $1 == "Muon" ]]; then
    
   
    source ${runPATH}/run_hnl_muon.sh Signals ${3} ${2} 
    
    FakeFlag=${2}',RunFake'
    source ${runPATH}/run_hnl_muon.sh Fake  ${3} ${FakeFlag}  
    PromptFlag=${2}',RunPrompt'
    source ${runPATH}/run_hnl_muon.sh Prompt  ${3} ${PromptFlag}  
    ConvFlag=${2}',RunConv'
    source ${runPATH}/run_hnl_muon.sh Conv ${3} ${ConvFlag} 




fi


if [[ $1 == "Fake" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/FakeOpt.txt       -n $njobs       --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/FakeOptLarge.txt  -n $nLargejobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/FakeOptXLarge.txt -n 100          --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &

fi


if [[ $1 == "FakeAll" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/FakeOptAll.txt     -n $njobs       --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/FakeOptLarge.txt  -n $nLargejobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/FakeOptXLarge.txt -n 100          --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &

fi

if [[ $1 == "Prompt" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/MCOpt.txt  -n $njobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/MCOptLarge.txt  -n $nLargejobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &

fi



if [[ $1 == "SignalsID" ]]; then

    SKFlat.py -a $analyzer  -l $sigpath/DYIDOpt.txt    -n $njobs_sig  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/SSWWOpt.txt  -n $njobs_sig  --nmax ${nmax}  -e ${2}  --userflags ${3}  --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/VBFOpt.txt   -n $njobs_sig  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&

fi


if [[ $1 == "Signals" ]]; then

    SKFlat.py -a $analyzer  -l $sigpath/SSWWOpt.txt  -n $njobs_sig  --nmax ${nmax}  -e ${2}  --userflags ${3}  --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/DYOpt.txt    -n $njobs_sig  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/VBFOpt.txt   -n $njobs_sig  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&

fi


if [[ $1 == "Conv" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n 5  --nmax ${nmax}   -e ${2} --skim SkimTree_HNMultiLepBDT  --userflags ${3} 
fi



