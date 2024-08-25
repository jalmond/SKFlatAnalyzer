analyzer=SkimTree_HNFakeBDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/SampleLists/Bkg/
datapath=${SKFlat_WD}/SkimScripts/SampleLists/Data/
njobs=400
njobs_data=400
nmax=400
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

if [[ $1 == "" ]]; then
    
    for i in "${era_list[@]}"
    do
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_FakeMu.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} --skim SkimTree_HNFake&
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_FakeEl.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} --skim SkimTree_HNFake&
        SKFlat.py -a $analyzer  -l $mcpath/FakeRateMainMCAlt.txt      -n ${njobs}       --nmax ${nmax}   -e ${i} --skim SkimTree_HNFake& 
	#SKFlat.py -a $analyzer  -l $mcpath/FakeRateMainMC.txt             -n ${njobs}       --nmax ${nmax}   -e ${i} --skim SkimTree_HNFake&
    done
fi
