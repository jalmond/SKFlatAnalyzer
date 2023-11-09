analyzer=SkimTree_HNMultiLep
qrundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
sigpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/
njobs=600
njobs_data=100
nmax=350
skim=' '

declare  -a era_list=("2016postVFP" "2016preVFP"  "2017" "2018")


if [[ $1 == "SL" ]]; then
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/DATA_mu_${i}.txt   -n ${njobs_data}  --nmax 500    -e ${i} &
    done
fi


if [[ $1 == "DATA" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}    &
        SKFlat.py -a $analyzer  -l $datapath/DATA_l_${i}.txt   -n ${njobs_data}  --nmax ${nmax}    -e ${i} &
    done

fi
if [[ $1 == "MC" ]]; then
    SKFlat.py -a $analyzer  -l ${mcpath}/2016postVFP/QCD.txt -n 2  --nmax ${nmax}   -e ${i} 

fi


if [[ $1 == "Signal" ]]; then
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l ${sigpath}/Signal.txt  -n 100  --nmax 100   -e ${i}   
	
    done
fi

if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}    &
        SKFlat.py -a $analyzer  -l $datapath/DATA_l_${i}.txt   -n ${njobs_data}  --nmax ${nmax}    -e ${i} &
        SKFlat.py -a $analyzer  -l ${mcpath}/MCXSmall.txt  -n 2  --nmax ${nmax}   -e ${i}  &
        SKFlat.py -a $analyzer  -l ${mcpath}/MCSmall.txt  -n 5  --nmax ${nmax}   -e ${i}  &
        SKFlat.py -a $analyzer  -l ${mcpath}/MCMedium.txt  -n 20  --nmax ${nmax}   -e ${i}  
        SKFlat.py -a $analyzer  -l ${mcpath}/MCLarge.txt  -n 50  --nmax ${nmax}   -e ${i}   &
        SKFlat.py -a $analyzer  -l ${mcpath}/MCXLarge.txt  -n 200  --nmax ${nmax}   -e ${i}   &
        SKFlat.py -a $analyzer  -l ${mcpath}/${i}/QCD.txt -n 2  --nmax ${nmax}   -e ${i} 
    done
fi
