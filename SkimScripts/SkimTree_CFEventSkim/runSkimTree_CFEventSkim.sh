analyzer=SkimTree_CFEventSkim
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Bkg/
nmax=300
skim=' '
declare  -a era_list=("2018" "2017"  "2016preVFP" "2016postVFP")
if [[ $1 == "" ]]; then
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l  $mcpath/MC.txt  -n 1000  --nmax ${nmax}   -e ${i}  &
    done

fi

