analyzer=HNL_LeptonIDSF
rundir=HNL_LeptonIDSF
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=400
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_Dilepton&
    done

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l   $mcpath/MC.txt  -n 400  --nmax ${nmax}   -e ${i}  --skim SkimTree_Dilepton  &
    done
fi
