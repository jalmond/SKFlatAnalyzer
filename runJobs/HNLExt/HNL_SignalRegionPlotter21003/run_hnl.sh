analyzer=HNL_SignalRegionPlotter21003
rundir=HNL_SignalRegionPlotter21003
sigpath=${SKFlat_WD}/runJobs/HNLExt/${analyzer}/Signals/
njobs=30
njobs_sig=2
njobs_data=100
nmax=250
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")


if [[ $1 == "SSWW2000" ]]; then

    SKFlat.py -a $analyzer  -i SSWWTypeI_SF_M2000_private  -n 10  --nmax ${nmax}   -e 2018 &

fi



if [[ $1 == "SSWW" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/${i}/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
    done

fi



if [[ $1 == "" ]]; then


    for i in "${era_list[@]}"
    do
	
	SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i}   &
        SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i}  &
	
    done

fi
