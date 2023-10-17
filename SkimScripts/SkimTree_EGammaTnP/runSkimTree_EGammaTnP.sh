analyzer=SkimTree_EGammaTnP
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
datapath=${SKFlat_WD}/SkimScripts/${analyzer}/data_lists_multilep/
njobs=2000
njobs_data=2000
nmax=300
skim=' '

declare  -a era_list=("2017" "2016preVFP" "2016postVFP" "2018")

if [[ $1 == "" ]]; then

    for i in "${era_list[@]}"
    do

	SKFlat.py -a $analyzer  -i DYJetsToEE_MiNNLO    -n 6000  --nmax 300           -e ${i}   --skim SkimTree_DileptonBDT &
	#SKFlat.py -a $analyzer  -i DYJets_MG    -n 6000  --nmax 300           -e ${i}   --skim SkimTree_DileptonBDT &
        #SKFlat.py -a $analyzer  -i DYJets       -n 6000  --nmax 300           -e ${i}   --skim SkimTree_DileptonBDT &
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_SingleLepton.txt  -n 6000  --nmax 300   -e ${i}  --skim SkimTree_DileptonBDT &

    done
fi
