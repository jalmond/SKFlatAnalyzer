analyzer=HNL_LeptonIDSF
rundir=HNL_LeptonIDSF
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=200
skim=' '
declare  -a era_list=("2018" "2016postVFP" "2016preVFP" "2017")

if [[ $1 == "TT" ]]; then
    
    declare  -a era_list=("2017")
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i TTLJ_powheg  -n 600  --nmax 200  -e ${i} --skim SkimTree_DileptonBDT 
    done
fi



if [[ $1 == "" ]]; then

    declare  -a era_list=("2018" "2016postVFP" "2016preVFP" "2017")

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt     -n 600  --nmax 400   -e ${i}  --skim SkimTree_DileptonBDT&
	SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt   -n 600  --nmax 400   -e ${i}  --skim SkimTree_DileptonBDT&
	SKFlat.py -a $analyzer  -l   $mcpath/MC.txt                   -n 600  --nmax 400   -e ${i}  --skim SkimTree_DileptonBDT
    done
fi


