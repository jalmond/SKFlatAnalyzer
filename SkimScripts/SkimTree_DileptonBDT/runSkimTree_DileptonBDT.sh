analyzer=SkimTree_DileptonBDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
sigpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/
njobs=600
njobs_data=600
nmax=600
skim=' '
declare  -a era_list=("2018" "2017"  "2016preVFP" "2016postVFP")

if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_Dilepton  &
	SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_Dilepton  &
	#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}EMu.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}   &
	SKFlat.py -a $analyzer  -l  $mcpath/MC2.txt  -n 1000  --nmax ${nmax}   -e ${i}  --skim SkimTree_Dilepton &
	#SKFlat.py -a $analyzer  -i TTLJ_powheg  -n 1000  --nmax ${nmax}   -e ${i}   
	
    done

fi

