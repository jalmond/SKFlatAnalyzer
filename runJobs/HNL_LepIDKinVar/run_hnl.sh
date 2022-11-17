analyzer=HNL_LepIDKinVar
rundir=HNL_LepIDKinVar
sigpath=${SKFlat_WD}/runJobs/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/${analyzer}/Bkg/
njobs=250
nmax=250
skim=' --skim SkimTree_HNMultiLep'
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")


declare  -a era_list=("2017")

if [[ $1 == "DY" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n 2  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "VBF" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/${i}/VBF.txt  -n 2  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "SSWW" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/${i}/SSWW.txt  -n 2  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "Top" ]]; then
    
    SKFlat.py -a $analyzer  -i TTLJ_powheg   -n 100  --nmax ${nmax}   -e 2017 ${skim}&

fi

if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n 5  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n 5  --nmax ${nmax}   -e ${i}  &
	SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n 5  --nmax ${nmax}   -e ${i}  &
	SKFlat.py -a $analyzer  -l   $mcpath/${i}/MC.txt  -n 25  --nmax ${nmax}   -e ${i} ${skim}  &
    done

fi
