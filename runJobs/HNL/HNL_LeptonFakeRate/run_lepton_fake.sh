analyzer=HNL_LeptonFakeRate
rundir=HNL_LeptonFakeRate
mcpath=${SKFlat_WD}/runJobs/HNL/${rundir}/mc_lists/
datapath=${SKFlat_WD}/runJobs/HNL/${rundir}/data_lists/
njobs=400
njobs_data=100
nmax=600
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a joblist=("CheckProfile"  "RatesMuon" "RatesMuon2" "RatesEl" "RatesEl2")

for i in "${era_list[@]}"

do
    
    if [[ $1 == "CheckProfile" ]]; then
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags CheckProfile&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n  ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags CheckProfileEE&
    fi

    if [[ $1 == "Rates" ]]; then
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt              -n ${njobs}       --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRates&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRates&
	SKFlat.py -a $analyzer  -l $mcpath/MC.txt              -n ${njobs}       --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesEE&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesEE&
    fi

    if [[ $1 == "RatesMuon" ]]; then
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt              -n ${njobs}       --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRates&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRates&
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_MuMu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_DileptonBDT --userflags RunRates&
    fi
    if [[ $1 == "RatesMuon2" ]]; then
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt              -n ${njobs}       --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesNonSNU&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesNonSNU&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_MuMu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_DileptonBDT --userflags RunRatesNonSNU&
    fi
    if [[ $1 == "RatesEl" ]]; then
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt              -n ${njobs}       --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesEE&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesEE&
        #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_DileptonBDT --userflags RunRatesEE&
    fi
    if [[ $1 == "RatesEl2" ]]; then
	SKFlat.py -a $analyzer  -l $mcpath/MC.txt              -n ${njobs}       --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesNonSNU_EE&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags RunRatesNonSNU_EE&
        SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_DileptonBDT --userflags RunRatesNonSNU_EE&
    fi
    if [[ $1 == "MakeRegionPlots" ]]; then
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt              -n ${njobs}       --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags MakeRegionPlots&
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags MakeRegionPlots&                                              
        SKFlat.py -a $analyzer  -l ${mcpath}/QCD_${i}.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags MakeRegionPlots&                                                     
    fi

done
echo "List of Job Options are:"
for i in "${joblist[@]}"

do
    echo $i
done

