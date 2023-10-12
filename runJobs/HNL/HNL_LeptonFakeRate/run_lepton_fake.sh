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

#### Measure truth fake rates from MCs, to optimize loose ID (MC) and get PtParton SF (QCD) ####

#TEST#
#SKFlat.py -a $analyzer  -i QCD_Pt-15To20_MuEnriched  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT --userflags FakeRateTruth &
#SKFlat.py -a $analyzer  -i QCD_Pt-120To170_MuEnriched  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT --userflags FakeRateTruth &

#SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT --userflags FakeRateTruth &
#SKFlat.py -a $analyzer  -l $mcpath/QCD_mu.txt  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT --userflags FakeRateTruth &

#### Measure truth fake rates from MCs *in the measurement region* to make use of it for the MC closure test (QCD), to get PtParton SF (QCD) ####

#### TEST ####
#SKFlat.py -a $analyzer  -i QCD_Pt-15To20_MuEnriched  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT & 
#SKFlat.py -a $analyzer  -i QCD_Pt-120To170_MuEnriched  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT &

#SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT &
#SKFlat.py -a $analyzer  -l $mcpath/QCD_mu.txt  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT &

#SKFlat.py -a $analyzer  -i TTLJ_powheg  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT &

#### Measuring the data-driven fake rates (MC : prompt subtraction) ####
#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_mu.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFakeBDT&
#SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n ${njobs_data}  --nmax ${nmax}  -e ${i} --skim SkimTree_HNFakeBDT&
#SKFlat.py -a $analyzer  -l $mcpath/MC.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT &
# But how much the QCD describe the data?
#SKFlat.py -a $analyzer  -l $mcpath/QCD_mu.txt  -n ${njobs}  --nmax ${nmax}  -e ${i} --skim SkimTree_FakeEventSkimBDT & # w/o BDT skim, you can't do LFvsHF separation
# And how much TTLL, TTJJ contribute to the fake measurement region?
#SKFlat.py -a $analyzer  -i TTLL_powheg  -n ${njobs}  --nmax ${nmax}  -e ${i} & # checked this is negligible
#SKFlat.py -a $analyzer  -i TTJJ_powheg  -n ${njobs}  --nmax ${nmax}  -e ${i} & # this is actually fake source, not prompt
# And does HNFakeBDT skim really cover all fake measurement region?
#SKFlat.py -a $analyzer  -i TTLJ_powheg  -n ${njobs}  --nmax ${nmax}  -e ${i} &
SKFlat.py -a $analyzer  -i DYJets  -n ${njobs}  --nmax ${nmax}  -e ${i} &
SKFlat.py -a $analyzer  -i WJets_MG  -n ${njobs}  --nmax ${nmax}  -e ${i} &

    done

fi



if [[ $1 == "Test" ]]; then

    SKFlat.py -a $analyzer  -i DYJets  -n 2  --nmax ${nmax}  -e 2017 --skim SkimTree_HNFakeBDT&

fi

