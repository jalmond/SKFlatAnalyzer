analyzer=SkimTree_HNElectronOpt
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
sigpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/

njobs=1000
njobs_data=250
nmax=250
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=("2017")

if [[ $1 == "DATA" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt   -n ${njobs_data}  --nmax ${nmax}    -e ${i} 
    done
    
fi

if [[ $1 == "MC" ]]; then
    for i in "${era_list[@]}"
    do
        #SKFlat.py -a $analyzer  -l $mcpath/WJet.txt  -n ${njobs}  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -i  WGToLNuG  -n ${njobs}  --nmax ${nmax}   -e ${i} &
    done

fi



if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt   -n ${njobs_data}  --nmax ${nmax}  --skim SkimTree_HNMultiLep   -e ${i}&
        SKFlat.py -a $analyzer  -l $mcpath/${i}/MC.txt  -n ${njobs}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep &
	
    done

fi

if [[ $1 == "Sig" ]]; then
    for i in "${era_list[@]}"
    do


        SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n 1000  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n 1000  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n 1000   --nmax ${nmax}   -e ${i} &

    done

fi
