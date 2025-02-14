analyzer=HNL_SignalRegion_Plotter
rundir=HNL_SignalRegion_Plotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=5
njobs_sig_syst=20
njobs_data=100
nmax=700
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")


if [[ $1 == "ScanSystematic" ]]; then

    SKFlat.py -a $analyzer  -i   DYTypeI_DF_M500_private  -n 1  --nmax 1   -e 2017 --skim SkimTree_HNMultiLepBDT   --userflags RunSyst,ScanSystematic &

fi

if [[ $1 == "DY" ]]; then

    declare  -a era_list=("2018")
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/PrivateCutFlow/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags  TestSignal &
        SKFlat.py -a $analyzer  -l $sigpath/PrivateCutFlow/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags TestSignal &
        SKFlat.py -a $analyzer  -l $sigpath/PrivateCutFlow/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags TestSignal &

    done
fi

if [[ $1 == "HighPtVeto" ]]; then

    declare  -a era_list=("2018")
    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -l $sigpath/Private/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags Remove_HighPtEC &
        SKFlat.py -a $analyzer  -l $sigpath/Private/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags Remove_HighPtEC &
        SKFlat.py -a $analyzer  -l $sigpath/Private/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags Remove_HighPtEC &

	#SKFlat.py -a $analyzer  -l $sigpath/Private/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags  Keep_HighPtEC &
        #SKFlat.py -a $analyzer  -l $sigpath/Private/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags Keep_HighPtEC &
        #SKFlat.py -a $analyzer  -l $sigpath/Private/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags Keep_HighPtEC &

    done

fi

if [[ $1 == "SIG" ]]; then

    declare  -a era_list=("2018")
    for i in "${era_list[@]}"
    do
	
	SKFlat.py -a $analyzer  -l $sigpath/Private/SSWW.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Private/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Private/VBF.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
        SKFlat.py -a $analyzer  -l $sigpath/Private/Weinberg.txt   -n 40  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Official/SSWWOfficial.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -l $sigpath/Official/DYOfficial.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &
        SKFlat.py -a $analyzer  -l $sigpath/Official/VBFOfficial.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  &

    done
    
fi

