analyzer=HNL_ConvStudy
rundir=HNL_ConvStudy
sigpath=${SKFlat_WD}/runJobs/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/${analyzer}/DATA/
njobs=100
njobs_data=100
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

declare  -a era_list=("2018")


if [[ $1 == "WG" ]]; then
    

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l  $mcpath/WG.txt  -n 20  --nmax 10   -e ${i}  --skim SkimTree_HNMultiLep &

    done

fi

