analyzer=SkimTree_DileptonBDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/SampleLists/Bkg/
datapath=${SKFlat_WD}/SkimScripts/SampleLists/Data/
njobs=600
njobs_data=600
nmax=400
declare  -a era_list=("2018" "2017"  "2016preVFP" "2016postVFP")
declare  -a era_list=("2016preVFP")
if [[ $1 == "All" ]]; then

    for i in "${era_list[@]}"
    do
        #SKFlat.py -a $analyzer  -l $datapath/DATA_Dilepton${i}.txt       -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT &
	#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_SingleLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT &
	#SKFlat.py -a $analyzer  -l $mcpath/CFEventMC_BDTSkim.txt           -n 500            --nmax ${nmax}   -e ${i}  &
	SKFlat.py -a $analyzer  -i DYJets     -n 500            --nmax ${nmax}   -e ${i}  &
    done
fi

