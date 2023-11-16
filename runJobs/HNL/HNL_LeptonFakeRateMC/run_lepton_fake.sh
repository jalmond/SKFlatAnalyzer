analyzer=HNL_LeptonFakeRateMC
rundir=HNL_LeptonFakeRateMC
mcpath=${SKFlat_WD}/runJobs/HNL/${rundir}/mc_lists/
datapath=${SKFlat_WD}/runJobs/HNL/${rundir}/data_lists/
njobs=500
njobs_data=100
nmax=500
declare  -a era_list=("2017")


declare  -a joblist=("MCClosure" "MCFakes" "Standard" "CheckProfile" "CheckProfileEE")

for i in "${era_list[@]}"

do

    if [[ $1 == "MCClosure" ]]; then
        SKFlat.py -a $analyzer  -i WJets_Sherpa               -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNMultiLepBDT --userflags MCClosure,RunFake& 
        #SKFlat.py -a $analyzer  -l $mcpath/MC.txt               -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags MCClosure,RunFake& 
    fi

    if [[ $1 == "MCFakes" ]]; then
	SKFlat.py -a $analyzer  -l ${mcpath}/QCD_${i}_Mu.txt    -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags MCFakes&
	SKFlat.py -a $analyzer  -l ${mcpath}/QCD_BEnriched.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags MCFakes&
        #SKFlat.py -a $analyzer  -l $mcpath/MC.txt               -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags MCFakes&
    fi

    if [[ $1 == "Standard" ]]; then
        SKFlat.py -a $analyzer  -l ${mcpath}/QCD_${i}_Mu.txt    -n ${njobs}   --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags Standard&
        SKFlat.py -a $analyzer  -l ${mcpath}/QCD_BEnriched.txt  -n ${njobs}   --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags Standard&
        #SKFlat.py -a $analyzer  -l $mcpath/MC.txt               -n ${njobs}   --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags Standard&
    fi


    if [[ $1 == "CheckProfile" ]]; then
	SKFlat.py -a $analyzer  -l ${mcpath}/QCD_${i}_Mu.txt    -n ${njobs}   --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags CheckProfile&
	SKFlat.py -a $analyzer  -l ${mcpath}/QCD_BEnriched.txt  -n ${njobs}   --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags CheckProfile&
        SKFlat.py -a $analyzer  -l $mcpath/MC.txt               -n ${njobs}   --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT --userflags CheckProfile&
    fi
    if [[ $1 == "CheckProfileEE" ]]; then
        SKFlat.py -a $analyzer  -l $datapath/DATA_${i}_el.txt  -n 20  --nmax ${nmax}  -e ${i}   --skim SkimTree_HNFakeBDT --userflags CheckProfileEE&
    fi

done

echo "List of Job Options are:"
for i in "${joblist[@]}"

do
    echo $i
done


