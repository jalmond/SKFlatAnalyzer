analyzer=HNL_ControlRegionPlotter
rundir=HNL_ControlRegionPlotter
sigpath=${SKFlat_WD}/runJobs/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/${analyzer}/DATA/
njobs=30
njobs_data=100
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=( "2018")

if [[ $1 == "WZ" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i WZTo3LNu_mllmin4p0_powheg  -n $njobs  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep &
    done

fi

if [[ $1 == "VBF" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/${i}/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
    done

fi

if [[ $1 == "VBF1500" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i VBFTypeI_SF_ll_M1500  -n 1  --nmax ${nmax}   -e ${i} &
    done

fi


if [[ $1 == "SSWW" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/${i}/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
    done

fi



if [[ $1 == "DATA" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep & 
    done
    
fi

if [[ $1 == "FAKE" ]]; then

    SKFlat.py -a $analyzer  -l $datapath/DATA_2018.txt  -n ${njobs_data}  --nmax ${nmax}   -e 2018  --skim SkimTree_HNMultiLep --userflags RunFake  &


fi


if [[ $1 == "XG" ]]; then
    
    declare  -a era_list=("2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i WGToLNuG   -n  10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunConv &
	SKFlat.py -a $analyzer  -i ZGToLLG   -n  10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunConv &
	SKFlat.py -a $analyzer  -i WGToLNuG   -n  10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep  &
        SKFlat.py -a $analyzer  -i ZGToLLG   -n  10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep &
	SKFlat.py -a $analyzer  -i DYJets   -n  10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunConv &
        SKFlat.py -a $analyzer  -i DYJets   -n  10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep &

    done

fi


if [[ $1 == "CF" ]]; then


    SKFlat.py -a $analyzer  -l $mcpath/CF.txt  -n $njobs_data  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunCF


fi


if [[ $1 == "" ]]; then


    for i in "${era_list[@]}"
    do
SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep  &
SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep --userflags RunFake  &
SKFlat.py -a $analyzer  -l $datapath/DATA_DoubleEG_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_Dilepton --userflags RunCF  &
SKFlat.py -a $analyzer  -l $datapath/DATA_MuonEG_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --userflags RunCF  &
SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep &
SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunConv&
SKFlat.py -a $analyzer  -l $mcpath/PromptRemoval.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunPromptTLRemoval&
#SKFlat.py -a $analyzer  -l $mcpath/CF.txt  -n $njobs_data  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLep --userflags RunCF &

    done

fi
