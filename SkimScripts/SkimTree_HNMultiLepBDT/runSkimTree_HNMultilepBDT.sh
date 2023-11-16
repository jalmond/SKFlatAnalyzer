analyzer=SkimTree_HNMultiLepBDT
qrundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
sigpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/
njobs=600
njobs_data=100
nmax=500
skim=' '

declare  -a era_list=("2016postVFP" "2016preVFP"  "2017" "2018")

if [[ $1 == "W" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i WJets_Sherpa  -n 4000  --nmax 500   -e ${i} --skim  SkimTree_HNMultiLep &
    done
fi

if [[ $1 == "SL" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_mu_${i}.txt   -n ${njobs_data}  --nmax 500    -e ${i} --skim  SkimTree_HNMultiLep &
    done
fi


if [[ $1 == "DATA" ]]; then

    declare  -a era_list=("2016preVFP" "2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt     -n ${njobs_data}  --nmax ${nmax}    -e ${i} --skim  SkimTree_HNMultiLep &
        SKFlat.py -a $analyzer  -l $datapath/DATA_l_${i}.txt   -n ${njobs_data}  --nmax ${nmax}    -e ${i} --skim  SkimTree_HNMultiLep &
    done
fi

if [[ $1 == "Signal" ]]; then

    declare  -a era_list=("2016preVFP" "2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l ${sigpath}/DYAdditionalBDT.txt  -n 20  --nmax ${nmax}   -e ${i} &
        SKFlat.py -a $analyzer  -l ${sigpath}/DYOfficial.txt  -n 20  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l ${sigpath}/VBFOfficial.txt  -n 20  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l ${sigpath}/SSWWOfficial.txt  -n 20  --nmax ${nmax}   -e ${i} &
    done
fi

if [[ $1 == "MC1" ]]; then

    declare  -a era_list=("2016preVFP" "2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l ${mcpath}/MCXSmall.txt -n 2  --nmax ${nmax}   -e ${i}   --skim  SkimTree_HNMultiLepBDT&
        SKFlat.py -a $analyzer  -l ${mcpath}/MCSmall.txt  -n 5  --nmax ${nmax}   -e ${i}   --skim  SkimTree_HNMultiLepBDT&
    done
fi

if [[ $1 == "MC2" ]]; then

    declare  -a era_list=("2016preVFP" "2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l ${mcpath}/MCMedium.txt -n 20 --nmax ${nmax}   -e ${i}  --skim  SkimTree_HNMultiLepBDT&
        SKFlat.py -a $analyzer  -l ${mcpath}/${i}/QCD.txt -n 2  --nmax ${nmax}   -e ${i}  --skim  SkimTree_HNMultiLepBDT&
    done
fi


if [[ $1 == "MC3" ]]; then

    declare  -a era_list=("2016preVFP" "2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l ${mcpath}/MCLarge.txt   -n 50   --nmax ${nmax}   -e ${i}   --skim  SkimTree_HNMultiLep &
        SKFlat.py -a $analyzer  -l ${mcpath}/MCXLarge.txt  -n 200  --nmax ${nmax}   -e ${i}   --skim  SkimTree_HNMultiLep &
    done
fi


if [[ $1 == "" ]]; then

    declare  -a era_list=("2016preVFP" "2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt     -n ${njobs_data}  --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l $datapath/DATA_l_${i}.txt   -n ${njobs_data}  --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l ${mcpath}/MCXSmall.txt      -n 2              --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l ${mcpath}/MCSmall.txt       -n 5              --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l ${mcpath}/MCMedium.txt      -n 20             --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l ${mcpath}/MCLarge.txt       -n 50             --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l ${mcpath}/MCXLarge.txt      -n 200            --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l ${mcpath}/${i}/QCD.txt      -n 2              --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
        SKFlat.py -a $analyzer  -l ${sigpath}/Signal.txt       -n 10             --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLep&
    done
fi
