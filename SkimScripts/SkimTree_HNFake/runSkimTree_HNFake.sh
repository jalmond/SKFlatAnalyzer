analyzer=SkimTree_HNFake
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
njobs=400
njobs_data=400
nmax=350
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} 
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}
	SKFlat.py -a $analyzer  -l $mcpath/MC2.txt  -n ${njobs}  --nmax ${nmax}   -e ${i} 
		
    done
    
fi
