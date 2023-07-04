analyzer=HNL_LeptonFakeRate
rundir=HNL_LeptonFakeRate
mcpath=${SKFlat_WD}/runJobs/HNL/${rundir}/mc_lists/
datapath=${SKFlat_WD}/runJobs/HNL/${rundir}/data_lists/
njobs=400
njobs_data=100
nmax=400
#declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=("2017")




if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT&
#SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT &
#SKFlat.py -a $analyzer  -l $mcpath/QCD_mu.txt  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT &
#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT&
#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFakeBDT&
    done

fi



if [[ $1 == "El" ]]; then
    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFake&
	
    done

fi

