analyzer=HNL_SignalRegionOpt
rundir=HNLOpt/HNL_SignalRegionOpt

# PATHS
runPATH=${SKFlat_WD}/runJobs/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/DATA/

# SKIM
skim=' '

# JOB CONFIG
njobs=40
njobs_sig=5
njobs_data=20
nmax=300

FLAG=opt_sr1

declare  -a era_list_Full=("2016postVFP" "2016preVFP" "2018")
declare  -a era_list_Partial=("2017")


if [[ $1 == "" ]]; then
    
    # If no inut then this is ran

    for i in "${era_list_Partial[@]}"
    do
	source ${runPATH}/run_hnl.sh Prompt  ${i}
        source ${runPATH}/run_hnl.sh NonPrompt ${i}
	source ${runPATH}/run_hnl.sh CF ${i}
	source ${runPATH}/run_hnl.sh Conv ${i}
	source ${runPATH}/run_hnl.sh Signals ${i}
    done

fi


if [[ $1 == "Signals" ]]; then
    
    SKFlat.py -a $analyzer  -l $sigpath/SSWWOpt.txt  -n $njobs_sig  --nmax ${nmax}  -e ${2} --userflags $FLAG &
    SKFlat.py -a $analyzer  -l $sigpath/DYOpt.txt    -n $njobs_sig  --nmax ${nmax}   -e ${2} --userflags $FLAG &
    SKFlat.py -a $analyzer  -l $sigpath/VBFOpt.txt   -n $njobs_sig  --nmax ${nmax}   -e ${2} --userflags $FLAG &
    
fi 

if [[ $1 == "Prompt" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt  -n $njobs  --nmax ${nmax}   -e ${2} --userflags $FLAG --skim SkimTree_HNMultiLep &

fi


if [[ $1 == "NonPrompt" ]]; then

    SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${2} --userflags $FLAG  --skim SkimTree_HNMultiLep --userflags RunFake  &

fi

if [[ $1 == "CF" ]]; then
    
    SKFlat.py -a $analyzer  -l $mcpath/CF.txt  -n $njobs  --nmax ${nmax}   -e ${2} --userflags $FLAG --skim SkimTree_HNMultiLep --userflags RunCF &
    #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} --userflags RunCF  &                                                                                                                                                                                             
fi


if [[ $1 == "Conv" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n $njobs  --nmax ${nmax}   -e ${2} --userflags $FLAG --skim SkimTree_HNMultiLep --userflags RunConv &
fi


if [[ $1 == "TMP" ]]; then
    
    SKFlat.py -a $analyzer  -l $sigpath/SSWWOpt.txt  -n $njobs_sig  --nmax ${nmax}  -e ${2} --userflags $FLAG &

fi
