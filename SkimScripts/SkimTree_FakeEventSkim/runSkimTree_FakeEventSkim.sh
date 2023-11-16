analyzer=SkimTree_FakeEventSkim
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
njobs=600
njobs_data=600
nmax=500
skim=' '
declare  -a era_list=("2018" "2017"  "2016preVFP" "2016postVFP")
if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i WJets_Sherpa -n 4000  --nmax ${nmax}   -e ${i}  &
	#SKFlat.py -a $analyzer  -l  $mcpath/QCD_${i}.txt  -n 400  --nmax ${nmax}   -e ${i}  &
	#SKFlat.py -a $analyzer  -l  $mcpath/MC.txt  -n       400  --nmax ${nmax}   -e ${i}&
	#SKFlat.py -a $analyzer  -l  $mcpath/QCD_BEnriched.txt -n 400  --nmax ${nmax}   -e ${i}&

    done

fi

