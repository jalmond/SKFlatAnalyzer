analyzer=HNL_FakeRate
rundir=HNL_FakeRate
mcpath=${SKFlat_WD}/runJobs/${rundir}/mc_lists/
datapath=${SKFlat_WD}/runJobs/${rundir}/data_lists/
njobs=400
njobs_data=100
nmax=350
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=("2018")




if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -l $mcpath/MC2.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFake&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFake&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFake

    done

fi



if [[ $1 == "El" ]]; then
    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFake&
	
    done

fi

