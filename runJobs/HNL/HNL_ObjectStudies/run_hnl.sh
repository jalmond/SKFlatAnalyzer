analyzer=HNL_ObjectStudies
rundir=HNL_ObjectStudies
mcpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Signals/
bkgpath=${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/
datapath=${SKFlat_WD}/runJobs/HNL/${analyzer}/DATA/
njobs=20
njobs_data=20
nmax=300
skim=' '
declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
declare  -a era_list=("2017")



if [[ $1 == "Sig" ]]; then


    for i in "${era_list[@]}"
    do
	SKFlat.py -a $analyzer  -l $mcpath/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l $mcpath/DY.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
	SKFlat.py -a $analyzer  -l $mcpath/VBF.txt  -n $njobs  --nmax ${nmax}   -e ${i} &


    done

fi


if [[ $1 == "VBF1500" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -i VBFTypeI_SF_ll_M1500  -n 1  --nmax 1000   -e ${i} &
    done

fi

if [[ $1 == "SSWW1000" ]]; then

    SKFlat.py -a $analyzer  -i SSWWTypeI_SF_M1000_private  -n 10  --nmax ${nmax}   -e 2018 &
    

fi



if [[ $1 == "SSWW" ]]; then

    for i in "${era_list[@]}"
    do
        SKFlat.py -a $analyzer  -l $mcpath/${i}/SSWW.txt  -n $njobs  --nmax ${nmax}   -e ${i} &
    done

fi


if [[ $1 == "" ]]; then

    declare  -a era_list=("2016preVFP")

    for i in "${era_list[@]}"
    do
	
        #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLep  &
	#SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private  -n 20  --nmax 1000   -e ${i} &
        #SKFlat.py -a $analyzer  -i DYTypeI_DF_M250_private  -n 20  --nmax 1000   -e ${i} &
        #SKFlat.py -a $analyzer  -i DYTypeI_DF_M500_private  -n 20  --nmax 1000   -e ${i} &
        #SKFlat.py -a $analyzer  -i DYTypeI_DF_M1000_private  -n 20  --nmax 1000   -e ${i} &
        #SKFlat.py -a $analyzer  -i DYJets  -n 200  --nmax 2000   -e ${i} &
	
	#SKFlat.py -a $analyzer  -i  WWW   -n 2  --nmax 2   -e ${i}   --skim SkimTree_HNMultiLepBDT &
	#SKFlat.py -a $analyzer  -i  WWW   -n 2  --nmax 2   -e ${i}   --skim SkimTree_HNMultiLep &
	SKFlat.py -a $analyzer  -l ${bkgpath}/Fake.txt   -n 400  --nmax 600   -e ${i}  --userflags FakeLepton &
	#SKFlat.py -a $analyzer  -i WJets_MG   -n 50  --nmax 600   -e ${i}  --userflags LeptonTypes &
	
    done
fi
