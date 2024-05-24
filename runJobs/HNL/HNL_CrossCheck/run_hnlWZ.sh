analyzer=HNL_ControlRegion_Plotter
rundir=HNL_ControlRegion_Plotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=2
njobs_data=200
nmax=500
skim=' '

if [[ $1 == "" ]]; then

    declare  -a era_list=("2017" "2018" "2016postVFP" "2016preVFP")
    declare  -a era_list=("2017")
    for i in "${era_list[@]}"
    do
        #### Data                                                                                                                                                                                                                             
	SKFlat.py -a $analyzer  -i WZTo3LNu_mllmin4p0_powheg        -n 200       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
	#SKFlat.py -a $analyzer  -i WJets_MG        -n 50       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunConv &
	#SKFlat.py -a $analyzer  -i WJets_amcatnlo        -n 50       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunConv &
	#SKFlat.py -a $analyzer  -i WZTo3LNu_mllmin0p1_powheg        -n 200       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
	
    done
    
fi
