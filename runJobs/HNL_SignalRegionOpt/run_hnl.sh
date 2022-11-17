analyzer=HNL_SignalRegionOpt
rundir=HNL_SignalRegionOpt
sigpath=${SKFlat_WD}/runJobs/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/${analyzer}/DATA/
njobs=40
njobs_sig=20
njobs_data=20
nmax=300
skim=' '

declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=("2017")


if [[ $1 == "DATA" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep &
        SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &

    done

fi

if [[ $1 == "DY300" ]]; then

    SKFlat.py -a $analyzer  -i DYTypeI_DF_M300_private  -n 20  --nmax ${nmax}   -e 2017 &

fi


if [[ $1 == "Big" ]]; then

    declare  -a era_list=( "2018")

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i ZZTo4L_powheg  -n 200  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep   &
	SKFlat.py -a $analyzer  -i TTLL_powheg  -n 200  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep   --userflags RunCF&

    done
fi


if [[ $1 == "SIG" ]]; then
 

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "ZZ" ]]; then


    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i ZZTo4L_powheg  -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep &

    done

fi



if [[ $1 == "" ]]; then


    for i in "${era_list[@]}"
    do
	
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep &
        SKFlat.py -a $analyzer  -l $mcpath/CF.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunCF &
        SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunConv & 
        SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n $njobs_sig  --nmax ${nmax}   -e ${i} &

    done

fi
