analyzer=HNL_LeptonIDSF
rundir=HNL_LeptonIDSF
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=800
skim=' '
declare  -a era_list=("2018" "2016postVFP" "2016preVFP" "2017")


if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n 100  --nmax 700   -e ${i}  --skim SkimTree_DileptonBDT&
	SKFlat.py -a $analyzer  -l   $mcpath/MC.txt  -n 400  --nmax 700  -e ${i}  --skim SkimTree_DileptonBDT
    done
fi


