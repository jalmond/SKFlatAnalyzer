analyzer=HNL_Signal_Efficiency
rundir=HNL_Signal_Efficiency
sigpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/

njobs=5
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

declare  -a era_list=("2017")
if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
        SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
        SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT&
	SKFlat.py -a $analyzer  -l ${mcpath}/Conv.txt    -n 100  --nmax 400  -e ${i}  --skim SkimTree_ConvEventSkim&
        SKFlat.py -a $analyzer  -l ${mcpath}/Fake.txt    -n 100  --nmax 400  -e ${i}  --skim SkimTree_FakeEventSkimBDT&
	SKFlat.py -a $analyzer  -i TTLL_powheg    -n 100  --nmax 400  -e ${i}  --skim SkimTree_HNMultiLepBDT &
	SKFlat.py -a $analyzer  -i DYJets         -n 100  --nmax 400  -e ${i}  --skim SkimTree_HNMultiLepBDT &

    done
    
fi
