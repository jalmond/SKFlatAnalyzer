analyzer=HNL_LeptonIDBDTStudies
rundir=HNL_LeptonIDBDTStudies
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=800
skim=' '
declare  -a era_list=("2018" "2016postVFP" "2016preVFP" "2017")

if [[ $1 == "TEST" ]]; then
    
    declare  -a era_list=("2018")
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i VBFTypeI_DF_M500_private  -n 2  --nmax 2  -e ${i} 
    done
fi



if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n 100  --nmax 700   -e ${i}  --skim SkimTree_DileptonBDT&
	SKFlat.py -a $analyzer  -l   $mcpath/MC.txt  -n 100  --nmax 700  -e ${i}  --skim SkimTree_DileptonBDT&
	SKFlat.py -a $analyzer  -l   $mcpath/MC2.txt  -n 400  --nmax 700  -e ${i}  &
    done
fi


