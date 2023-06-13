analyzer=HNL_SignalStudies
rundir=HNL_SignalStudies
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Signals/
bkgpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/DATA/
njobs=20
njobs_data=20
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")


if [[ $1 == "Sig" ]]; then


    for i in "${era_list[@]}"
    do
	#SKFlat.py -a $analyzer  -l $mcpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
	#SKFlat.py -a $analyzer  -l $mcpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
	SKFlat.py -a $analyzer  -l $mcpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&


    done

fi


if [[ $1 == "DY100" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private  -n 1  --nmax 1   -e ${i} --skim SkimTree_HNMultiLepBDT &
    done

fi

if [[ $1 == "1000" ]]; then

    SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private  -n 10  --nmax 10   -e 2018 --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -i DYTypeI_DF_M1000_private  -n 10  --nmax 10   -e 2018 --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -i VBFTypeI_DF_M1000_private  -n 10  --nmax 10   -e 2018 --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -i SSWWTypeI_SF_M1000_private  -n 10  --nmax ${nmax}   -e 2018 --skim SkimTree_HNMultiLepBDT&
    

fi



if [[ $1 == "QCD" ]]; then

    declare  -a era_list=("2017")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i TTLJ_powheg   -n 500  --nmax 350  -e ${i}  &
    done

fi


if [[ $1 == "" ]]; then

    declare  -a era_list=("2017")

    for i in "${era_list[@]}"
    do
	
	#SKFlat.py -a $analyzer  -l $mcpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
        #SKFlat.py -a $analyzer  -l $mcpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
        #SKFlat.py -a $analyzer  -l $mcpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	#SKFlat.py -a $analyzer  -i DYJets  -n 200  --nmax 2000   -e ${i} --skim SkimTree_HNMultiLepBDT&
	#SKFlat.py -a $analyzer  -i  ZGToLLG   -n 20  --nmax 400   -e ${i}  --skim SkimTree_HNMultiLepBDT&
	#SKFlat.py -a $analyzer  -i  TTLJ_powheg   -n 600  --nmax 400   -e ${i}  --skim SkimTree_HNMultiLepBDT&
	
    done
fi
