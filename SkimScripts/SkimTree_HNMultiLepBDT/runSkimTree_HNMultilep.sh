analyzer=SkimTree_HNMultiLepBDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
sigpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/
njobs=400
njobs_data=100
nmax=250
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

if [[ $1 == "DATA" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep  &
        SKFlat.py -a $analyzer  -l $datapath/DATA_l_${i}.txt   -n ${njobs_data}  --nmax ${nmax}   --skim SkimTree_HNMultiLep  -e ${i} 
    done
    
fi

if [[ $1 == "DATAEMu" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}EMu.txt  -n ${njobs_data}  --nmax 100   -e ${i} &

    done

fi
if [[ $1 == "MC" ]]; then
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l  $mcpath/MC.txt  -n 500  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep &
		
    done

fi

if [[ $1 == "SIG" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l  $sigpath/SSWW.txt  -n 500  --nmax 350   -e ${i}  &
        SKFlat.py -a $analyzer  -l  $sigpath/DY.txt  -n 500  --nmax 350   -e ${i}  &
        SKFlat.py -a $analyzer  -l  $sigpath/VBF.txt  -n 500  --nmax 350   -e ${i}  &

    done

fi


if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -l  $mcpath/MC.txt  -n 500  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep 
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep  &
	SKFlat.py -a $analyzer  -l $datapath/DATA_l_${i}.txt   -n ${njobs_data}  --nmax ${nmax}   --skim SkimTree_HNMultiLep  -e ${i}
	
        SKFlat.py -a $analyzer  -l  $sigpath/SSWW.txt  -n 50  --nmax 250   -e ${i}  
        SKFlat.py -a $analyzer  -l  $sigpath/DY.txt  -n 50  --nmax 250   -e ${i}  
        SKFlat.py -a $analyzer  -l  $sigpath/VBF.txt  -n 50  --nmax 250   -e ${i}  

    done

fi
