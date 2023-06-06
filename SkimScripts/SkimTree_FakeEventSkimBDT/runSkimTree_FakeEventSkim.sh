analyzer=SkimTree_FakeEventSkimBDT
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
njobs=600
njobs_data=600
nmax=350
skim=' '
declare  -a era_list=("2018" "2017"  "2016preVFP" "2016postVFP")
declare  -a era_list=( "2016preVFP")
if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do

	SKFlat.py -a $analyzer  -l  $mcpath/QCD_${i}.txt  -n 600  --nmax ${nmax}   -e ${i}  --skim SkimTree_FakeEventSkim&
	SKFlat.py -a $analyzer  -l  $mcpath/MC.txt  -n 600  --nmax ${nmax}   -e ${i}  --skim SkimTree_FakeEventSkim&

    done

fi

