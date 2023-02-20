analyzer=HNL_LeptonIDBDTStudies
rundir=HNL_LeptonIDBDTStudies
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=800
skim=' SkimTree_DileptonBDT'
declare  -a era_list=("2018" "2016postVFP" "2016preVFP" "2017")

if [[ $1 == "TEST" ]]; then
    
    declare  -a era_list=("2018")
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i WW_pythia  -n 100  --nmax 600  -e ${i}    --userflags JetDisc &
    done
fi



if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton_Mu.txt  -n 200  --nmax 700   -e ${i}   --userflags JetDisc --skim $skim&
	SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton_E.txt  -n 200  --nmax 700   -e ${i}   --userflags JetDiss --skim $skim&
	SKFlat.py -a $analyzer  -l   $mcpath/MC.txt  -n 200  --nmax 700  -e ${i}  --skim $skim --userflags JetDisc&

    done
fi


