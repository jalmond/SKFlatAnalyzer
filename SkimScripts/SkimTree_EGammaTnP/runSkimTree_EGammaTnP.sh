analyzer=SkimTree_EGammaTnP
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/

declare  -a era_list=("2017" "2016preVFP" "2016postVFP" "2018")

if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do

	SKFlat.py -a $analyzer  -i DYJetsToEE_MiNNLO                     -n 6000  --nmax 400   -e ${i}  --skim SkimTree_DileptonBDT &
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_SingleLepton.txt  -n 6000  --nmax 400   -e ${i}  --skim SkimTree_DileptonBDT &

    done
fi
