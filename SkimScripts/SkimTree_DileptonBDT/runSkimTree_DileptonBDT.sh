analyzer=SkimTree_DileptonBDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
sigpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/
njobs=600
njobs_data=600
nmax=400
skim=' '

declare  -a era_list=("2018" "2017"  "2016preVFP" "2016postVFP")

if [[ $1 == "More" ]]; then

    declare  -a era_list=( "2016preVFP" "2016postVFP")
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $mcpath/MC_Additional.txt   --nmax 300           -e ${i} &
      done

fi


if [[ $1 == "All" ]]; then

    declare  -a era_list=( "2017")
    for i in "${era_list[@]}"
    
    do
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt               -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT &
	#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_EMu.txt           -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT &
	#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_SingleLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT &
	#SKFlat.py -a $analyzer  -l $mcpath/MC.txt                        -n 500            --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT &
	#SKFlat.py -a $analyzer  -i WGToLNuG                               -n 800            --nmax ${nmax}   -e ${i}   &
	#SKFlat.py -a $analyzer  -i ZGToLLG                                -n 800            --nmax ${nmax}   -e ${i}   &
	SKFlat.py -a $analyzer  -i WWTo2L2Nu_powheg                       -n 800            --nmax ${nmax}   -e ${i}   &
	SKFlat.py -a $analyzer  -i WWZ                       -n 800            --nmax ${nmax}   -e ${i}   &
	SKFlat.py -a $analyzer  -i ZZZ                       -n 800            --nmax ${nmax}   -e ${i}   &
	SKFlat.py -a $analyzer  -i WWW                       -n 800            --nmax ${nmax}   -e ${i}   &
	
    done
fi

