analyzer=HNL_LeptonFakeStudy
rundir=HNL_LeptonFakeStudy
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/HNL/${rundir}/data_lists/
njobs=40
njobs_data=100
nmax=300
#declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=("2017")

if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
	#SKFlat.py -a $analyzer  -l ${mcpath}/Fake.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT&
	SKFlat.py -a $analyzer  -l ${mcpath}/QCD_${i}.txt  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT&
	#SKFlat.py -a $analyzer  -i DYJets  -n ${njobs}  --nmax ${nmax}  -e ${i}   --skim SkimTree_FakeEventSkimBDT&
    done

fi


