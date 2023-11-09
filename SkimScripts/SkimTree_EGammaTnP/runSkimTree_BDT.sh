analyzer=SkimTree_BDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
sigpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/
njobs=600
njobs_data=600
nmax=100
skim=' '


declare  -a era_list=("2017" )
# "2016preVFP" "2016postVFP" "2018")

if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i TTLJ_powheg  -n 6000  --nmax 300           -e ${i}  
	SKFlat.py -a $analyzer  -i TTLL_powheg  -n 6000  --nmax 300           -e ${i}  
	SKFlat.py -a $analyzer  -i DYJets_MG    -n 6000  --nmax 300           -e ${i}  
        SKFlat.py -a $analyzer  -i DYJets       -n 6000  --nmax 300           -e ${i}

    done
fi
