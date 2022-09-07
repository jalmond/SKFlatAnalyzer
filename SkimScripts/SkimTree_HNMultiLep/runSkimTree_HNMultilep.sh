analyzer=SkimTree_HNMultiLep
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
njobs=100
njobs_data=100
nmax=100
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=( "2016postVFP" "2016preVFP" "2018")

if [[ $1 == "DATA" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_TMP.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} &
        #SKFlat.py -a $analyzer  -l $datapath/DATA_l_${i}.txt   -n ${njobs_data}  --nmax ${nmax}    -e ${i} 
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
        SKFlat.py -a $analyzer  -l $mcpath/${i}/MC.txt  -n ${njobs}  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "WJet" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i WJets_MG  -n ${njobs}  --nmax ${nmax}   -e ${i} &
    done

fi
