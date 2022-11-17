analyzer=HNL_SignalLeptonOpt
rundir=HNL_SignalLeptonOpt
sigpath=${SKFlat_WD}/runJobs/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/${analyzer}/DATA/
njobs=20
njobs_sig=2
njobs_data=2
nmax=300
skim=' '

declare  -a era_list=("2016preVFP" "2016postVFP" "2017" "2018")


if [[ $1 == "DATA" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep &
    done
    
fi

if [[ $1 == "DY100" ]]; then

    SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private  -n 10  --nmax ${nmax}   -e 2017 &

fi


if [[ $1 == "SIG" ]]; then
 

    SKFlat.py -a $analyzer  -l $sigpath/2017/SSWW.txt  -n $njobs_sig  --nmax ${nmax}   -e 2017 &
    SKFlat.py -a $analyzer  -l $sigpath/2017/DY.txt  -n $njobs_sig  --nmax ${nmax}   -e 2017 &
    SKFlat.py -a $analyzer  -l $sigpath/2017/VBF.txt  -n $njobs_sig  --nmax ${nmax}   -e 2017 &
   

fi

if [[ $1 == "DY" ]]; then


    SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private  -n 1  --nmax ${nmax}   -e 2017   &


fi



if [[ $1 == "" ]]; then

    declare  -a era_list=("2017")

    for i in "${era_list[@]}"
    do
	
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep &
        SKFlat.py -a $analyzer  -l $mcpath/CF.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunCF &
        SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunConv & 

    done

fi

