analyzer=SkimTree_FakeEventSkimBDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
njobs=600
njobs_data=600
nmax=350
skim=' '
declare  -a era_list=("2018" "2017"  "2016preVFP" "2016postVFP")

declare  -a era_list=(  "2016preVFP" "2016postVFP")

if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
	
	#SKFlat.py -a $analyzer  -l  $mcpath/QCD_${i}.txt    -n 20  --nmax ${nmax}   -e ${i}  --skim SkimTree_FakeEventSkimBDT&
        #SKFlat.py -a $analyzer  -l  $mcpath/HaneolList.txt  -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_FakeEventSkimBDT&
	SKFlat.py -a $analyzer  -l  $mcpath/MC.txt          -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_FakeEventSkimBDT&

    done

fi

