analyzer=HNL_SignalEfficiency
rundir=HNL_SignalEfficiency
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Signals/

njobs=5
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

declare  -a era_list=("2017")
if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $mcpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $mcpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $mcpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i}
    done
    
    SKFlat.py -a $analyzer  -i TTLJ_powheg  -n 300  --nmax ${nmax}   -e ${i}
    

fi


if [[ $1 == "Fake" ]]; then

    SKFlat.py -a $analyzer  -l ${SKFlat_WD}/runJobs/SampleLists/Bkg/FakeMuon.txt  -n 300  --nmax ${nmax}   -e 2017 &
    

fi
