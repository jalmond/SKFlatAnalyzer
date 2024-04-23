1;95;0canalyzer=HNL_Lepton_ChargeFlip
rundir=HNL_Lepton_ChargeFlip
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Data/
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
njobs=250
nmax=400
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

declare  -a era_list=("2017")

if [[ $1 == "Study" ]]; then

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i DYJets           -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftCFStudy&
        SKFlat.py -a $analyzer  -i TTLL_powheg      -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftCFStudy&
    done
fi

if [[ $1 == "Shift" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2018")

    for  i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $mcpath/Shift.txt -n 200      --nmax 400   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergy&
	SKFlat.py -a $analyzer  -l $mcpath/Shift_all.txt -n 200  --nmax 400   -e ${i}  --skim SkimTree_BDT --userflags ShiftEnergy
    done
fi


if [[ $1 == "ShiftZ" ]]; then

    declare  -a era_list=("2017")

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -i DYJets        -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergyZ&
        SKFlat.py -a $analyzer  -i TTLL_powheg      -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergyZ&
        SKFlat.py -a $analyzer  -l $mcpath/DYMG.txt -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergyZ&
        SKFlat.py -a $analyzer  -i WJets_MG         -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergyZ&
        SKFlat.py -a $analyzer  -i TTLJ_powheg      -n 600  --nmax 500   -e ${i}  --skim SkimTree_DileptonBDT --userflags ShiftEnergyZ&

    done
fi



if [[ $1 == "Rates" ]]; then

    declare  -a era_list=("2017")
    for i in "${era_list[@]}"
    do
        #SKFlat.py -a $analyzer -l $mcpath/CFRates.txt   -n 600  --nmax 600   -e ${i}  --skim SkimTree_DileptonBDT --userflags ElCFRates&  
        SKFlat.py -a $analyzer -l $mcpath/CFRates.txt   -n 600  --nmax 800   -e ${i}  --skim SkimTree_DileptonBDT --userflags ElCFRates,RateNoShift&  
	
        #SKFlat.py -a $analyzer  -l $mcpath/Shift_all.txt -n 600  --nmax 600   -e ${i}  --skim SkimTree_BDT --userflags ElCFRates&
        #SKFlat.py -a $analyzer  -l $mcpath/Shift_all.txt -n 600  --nmax 600   -e ${i}  --skim SkimTree_BDT --userflags ElCFRates,RateNoShift&
	
    done
fi

if [[ $1 == "SF" ]]; then
    

    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l runJobs/SampleLists/Bkg/DYMG.txt      -n 500  --nmax 500   -e ${i} --skim SkimTree_DileptonBDT --userflags ScaleFactor&
	SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt  -n 50    --nmax 500       -e ${i}  --skim SkimTree_DileptonBDT --userflag ScaleFactor   &   
        SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt  -n 50    --nmax 500       -e ${i}  --skim SkimTree_DileptonBDT --userflag ScaleFactor,FLATSF   &           
    done
fi
