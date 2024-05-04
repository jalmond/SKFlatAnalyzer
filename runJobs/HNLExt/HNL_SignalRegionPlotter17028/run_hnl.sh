analyzer=HNL_SignalRegionPlotter17028
rundir=HNL_SignalRegionPlotter17028
sigpath=${SKFlat_WD}/runJobs/HNLExt/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/HNLExt/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/HNLExt/${analyzer}/DATA/
njobs=30
njobs_data=100
nmax=350
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

if [[ $1 == "DY" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/${i}/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT&
    done

fi

if [[ $1 == "VBF" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i VBFTypeI_DF_M400_private   -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
    done

fi

if [[ $1 == "M1000" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i DYTypeI_DF_M1000_private  -n 10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
        SKFlat.py -a $analyzer  -i VBFTypeI_DF_M1000_private  -n 10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&

    done

fi

if [[ $1 == "DY300" ]]; then

    SKFlat.py -a $analyzer  -i DYTypeI_DF_M300_private  -n 1  --nmax ${nmax}   -e 2018 --skim SkimTree_HNMultiLepBDT&

fi



if [[ $1 == "SSWW" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/${i}/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
    done

fi



if [[ $1 == "DATA" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT & 
    done
    
fi

if [[ $1 == "FAKE" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunFake  &
    done

fi


if [[ $1 == "MC" ]]; then
    
    declare  -a era_list=("2016postVFP")

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $mcpath/${i}/MC.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim  SkimTree_HNMultiLepBDT &

    done

fi

if [[ $1 == "" ]]; then

    declare  -a era_list=("2018")

    for i in "${era_list[@]}"
    do
	
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT 
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags RunFake  
        #SKFlat.py -a $analyzer  -l $datapath/DATAEl_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT --userflags RunCF
	
	#SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT 
	#SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
	#SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
	SKFlat.py -a $analyzer  -l $sigpath/DYRerun.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
        #SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT
	#SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunConv &                                                                                                                         

    done

fi
