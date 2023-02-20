analyzer=HNL_LeptonIDSF
rundir=HNL_LeptonIDSF
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=200
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

    declare  -a era_list=("2016postVFP" "2016preVFP")

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n 200  --nmax 100   -e ${i}  --skim SkimTree_DileptonBDT&
	SKFlat.py -a $analyzer  -l   $mcpath/MC.txt  -n 600  --nmax 100  -e ${i}  --skim SkimTree_DileptonBDT&
    done
fi


