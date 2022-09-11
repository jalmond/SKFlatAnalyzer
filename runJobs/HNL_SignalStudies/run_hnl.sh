analyzer=HNL_SignalStudies
rundir=HNL_SignalStudies
mcpath=${SKFlat_WD}/runJobs/${analyzer}/Signals/
bkgpath=${SKFlat_WD}/runJobs/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/${analyzer}/DATA/
njobs=20
njobs_data=200
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=("2017")

if [[ $1 == "VBF1500" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i VBFTypeI_SF_ll_M1500  -n 1  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "SSWW1000" ]]; then

    SKFlat.py -a $analyzer  -i SSWWTypeI_SF_M1000_private  -n 10  --nmax ${nmax}   -e 2018 &
    

fi



if [[ $1 == "SSWW" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $mcpath/${i}/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $mcpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l $mcpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l $mcpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $bkgpath/MC.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep &
	SKFlat.py -a $analyzer  -l $bkgpath/QCD.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep
    done

fi


if [[ $1 == "Q" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $mcpath/DYsample.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $mcpath/VBFsample.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $mcpath/SSWWsample.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $bkgpath/MC.txt  -n 400   --nmax ${nmax}   -e ${i}  &
        SKFlat.py -a $analyzer  -l $bkgpath/QCD.txt  -n 100  --nmax ${nmax}   -e ${i} 
    done

fi



if [[ $1 == "MC" ]]; then

    declare  -a era_list=("2018")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $bkgpath/MC.txt  -n 200  --nmax 300   -e ${i} &
    done
    
fi
