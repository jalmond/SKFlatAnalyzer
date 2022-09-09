analyzer=HNL_FakeRate
rundir=HNL_FakeRate
mcpath=${SKFlat_WD}/runJobs/${rundir}/mc_lists/
datapath=${SKFlat_WD}/runJobs/${rundir}/data_lists/
njobs=200
njobs_data=100
nmax=250
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

if [[ $1 == "DY" ]]; then
    
    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -i DYJets   -n 200  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFake &

    done


fi


if [[ $1 == "Top" ]]; then

    SKFlat.py -a $analyzer  -i TTLJ_powheg    -n 200  --nmax ${nmax}  -e 2018 --skim SkimTree_HNFake &

fi



if [[ $1 == "NVTX" ]]; then
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i DYJets   -n 200  --nmax ${nmax}  -e ${i}  &
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}  &
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} &

    done

fi

if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do

        SKFlat.py -a $analyzer  -l $mcpath/MC_${i}.txt  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFake &

        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFake &

        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFake &

    done

fi


