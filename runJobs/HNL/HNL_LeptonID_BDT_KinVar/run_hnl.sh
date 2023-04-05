analyzer=HNL_LeptonID_BDT_KinVar
rundir=HNL_LeptonID_BDT_KinVar
sigpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Signals/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=300
nmax=250

declare  -a era_list=("2018" "2016postVFP" "2016preVFP" "2017")

#declare  -a era_list=("2016postVFP" "2016preVFP" "2018")

if [[ $1 == "TT" ]]; then

    declare  -a era_list=("2016postVFP")
    nmax=2

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i  WZ_pythia  -n 300  --nmax ${nmax}   -e ${i}  --userflags SeperateFake &
    
    done
fi



if [[ $1 == "" ]]; then

    nmax=300
    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $sigpath/DYsample.txt  -n 10  --nmax ${nmax}   -e ${i}  &
        SKFlat.py -a $analyzer  -l $sigpath/VBFsample.txt  -n 10  --nmax ${nmax}   -e ${i} & 
        SKFlat.py -a $analyzer  -l $sigpath/SSWWsample.txt  -n 10  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l $mcpath/Prompt.txt      -n 400  --nmax ${nmax}   -e ${i}  --userflags SeperatePrompt &
	#SKFlat.py -a $analyzer  -l   $mcpath/Fake.txt  -n 300  --nmax ${nmax}   -e ${i}  --userflags SeperateFake,HFB  --skim SkimTree_FakeEventSkim&
	#SKFlat.py -a $analyzer  -l   $mcpath/Fake.txt  -n 300  --nmax ${nmax}   -e ${i}  --userflags SeperateFake,HFC --skim SkimTree_FakeEventSkim&
	SKFlat.py -a $analyzer  -l   $mcpath/Fake.txt  -n 300  --nmax ${nmax}   -e ${i}  --userflags SeperateFake,LF --skim SkimTree_FakeEventSkim& 
	SKFlat.py -a $analyzer  -l   $mcpath/Fake.txt  -n 300  --nmax ${nmax}   -e ${i}  --userflags SeperateFake --skim SkimTree_FakeEventSkim&
	#SKFlat.py -a $analyzer  -l   $mcpath/Conv.txt  -n 500  --nmax ${nmax}   -e ${i}  --userflags SeperateConv &
	#SKFlat.py -a $analyzer  -l   $mcpath/CF.txt    -n 1000  --nmax ${nmax}   -e ${i}  --userflags SeperateCF  
    done
fi


