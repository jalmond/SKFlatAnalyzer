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

if [[ $1 == "" ]]; then

    declare  -a era_list=("2018" "2017")
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $mcpath/MC2.txt   --nmax 400           -e ${i} &
        #SKFlat.py -a $analyzer  -i SingleTop_sch_Lep  --nmax 350           -e ${i}  --skim SkimTree_Dilepton&
        #SKFlat.py -a $analyzer  -i SingleTop_tW_antitop_NoFullyHad  --nmax 350           -e ${i}  --skim SkimTree_Dilepton&
        #SKFlat.py -a $analyzer  -i SingleTop_tW_top_NoFullyHad  --nmax 350           -e ${i}  --skim SkimTree_Dilepton&
    done

fi

if [[ $1 == "Ind" ]]; then
    
    declare  -a era_list=("2018" )
    
     for i in "${era_list[@]}"
     do
	 SKFlat.py -a $analyzer  -l  $datapath/DATA_${i}_ind.txt  -n 500  --nmax 300    -e ${i} --skim SkimTree_Dilepton& 
     done

fi


if [[ $1 == "All" ]]; then

    declare  -a era_list=("2018" "2017" )
    for i in "${era_list[@]}"
    
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}           -e ${i}  --skim SkimTree_DileptonBDT  &
	SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  &
	SKFlat.py -a $analyzer  -l $datapath/DATA_${i}EMu.txt  -n ${njobs_data}  --nmax ${nmax}        -e ${i} --skim SkimTree_DileptonBDT  &
	SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n 2000  --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT &
    done
fi

