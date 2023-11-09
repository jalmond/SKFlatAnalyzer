analyzer=HNL_ChargeFlip
rundir=HNL_ChargeFlip
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=400
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

if [[ $1 == "Shift" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i DYJets           -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergy&
	SKFlat.py -a $analyzer  -i TTLL_powheg      -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergy&
	SKFlat.py -a $analyzer  -l $mcpath/DYMG.txt -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergy&
	SKFlat.py -a $analyzer  -i WJets_MG         -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergy&
	SKFlat.py -a $analyzer  -i TTLJ_powheg      -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergy&
    done
fi


if [[ $1 == "ShiftZ" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i DYJets        -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergyZ&
    done
fi



if [[ $1 == "Rates" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer -l $mcpath/DYMG.txt   -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ElCFRates&  
	SKFlat.py -a $analyzer  -i DYJets            -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ElCFRates&
	SKFlat.py -a $analyzer  -i TTLL_powheg       -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ElCFRates&
	SKFlat.py -a $analyzer  -i TTLJ_powheg       -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ElCFRates&
	SKFlat.py -a $analyzer  -i WJets_MG          -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ElCFRates&
	
    done
fi

if [[ $1 == "SF" ]]; then
    
    declare  -a era_list=("2017")
    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i ZGToLLG       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor&
	SKFlat.py -a $analyzer  -i WGToLNuG       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor&
	SKFlat.py -a $analyzer  -i ZZTo4L_powheg       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor&
	SKFlat.py -a $analyzer  -i WZTo3LNu_amcatnlo       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor&
	SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt  -n 50    --nmax 500       -e ${i}  --skim SkimTree_DileptonBDT --userflag ScaleFactor   &                   
 
	SKFlat.py -a $analyzer  -i ZGToLLG       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor,FLATSF&
        SKFlat.py -a $analyzer  -i WGToLNuG       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor,FLATSF&
        SKFlat.py -a $analyzer  -i ZZTo4L_powheg       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor,FLATSF&
        SKFlat.py -a $analyzer  -i WZTo3LNu_amcatnlo       -n 50  --nmax 500   -e ${i} --skim SkimTree_HNMultiLepBDT --userflags ScaleFactor,FLATSF&
        SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt  -n 50    --nmax 500       -e ${i}  --skim SkimTree_DileptonBDT --userflag ScaleFactor,FLATSF   &           
    done
fi
