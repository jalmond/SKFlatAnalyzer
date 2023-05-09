analyzer=SkimTree_HNL_SS_ElMu
rundir=runSkims
mcpath=${SKFlat_WD}/SkimScripts/${analyzer}/Signals/
njobs=20
nmax=300
skim=' '
declare  -a era_list=("2016preVFP" "2016postVFP" "2017" "2018") 

for i in "${era_list[@]}"
do
    SKFlat.py -a $analyzer  -l $mcpath/SSWW.txt  -n ${njobs}  --nmax ${nmax}   -e ${i} -o /gv0/Users/jalmond/MC/  --skim SkimTree_HNMultiLepBDT& 
    SKFlat.py -a $analyzer  -l $mcpath/DY.txt  -n ${njobs}  --nmax ${nmax}   -e ${i} -o /gv0/Users/jalmond/MC/  --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $mcpath/VBF.txt  -n ${njobs}  --nmax ${nmax}   -e ${i} -o /gv0/Users/jalmond/MC/  --skim SkimTree_HNMultiLepBDT&
 
done
