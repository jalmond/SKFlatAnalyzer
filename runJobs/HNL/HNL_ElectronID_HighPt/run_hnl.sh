analyzer=HNL_ElectronID_HighPt
rundir=HNL_ElectronID_HighPt
sigpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=5
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

declare  -a era_list=("2017")

if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
	SKFlat.py -a $analyzer  -l $sigpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
        SKFlat.py -a $analyzer  -l $sigpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
	SKFlat.py -a $analyzer  -i TTLJ_powheg  -n $njobs  --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT &
        SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT &
	
    done

fi

