import argparse
parser = argparse.ArgumentParser(description='option')
parser.add_argument('-e', dest='era', default="NULL")
parser.add_argument('-s', dest='skim', default="NULL")
parser.add_argument('--Full', action='store_true')
args = parser.parse_args()
input_era=args.era
input_skim=args.skim

analyzer=HNL_ControlRegionPlotter
rundir=HNL_ControlRegionPlotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=2
njobs_data=200
nmax=500
skim=' '


Eras = ["2016preVFP", "2016postVFP", "2017", "2018"]
flag_list= ["SS_CR"]

if input_era != "NULL":
    Eras = [input_era]

if input_flag != "NULL":
    flag_list = input_flag


for Era in Eras:

    if RunJob== "WZ":

        for FLAG in flag_list:
            os.system("SKFlat.py -a $analyzer  -i WZTo3LNu_amcatnlo   -n 100 --nmax 600   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags "+FLAG+",RunPrompt  &")
        

    else:

        if FLAG == "OS_VR":
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt       -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunFake  &
            SKFlat.py -a $analyzer  -l $mcpath/VVOS.txt                  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunFake  &     
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &                  
            SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &                  
            SKFlat.py -a $analyzer  -l $mcpath/PromptOS.txt             -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunPrompt &         
            SKFlat.py -a $analyzer  -l $mcpath/ConvOS.txt               -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunConv   &         
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake   &         
            SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake  &
        else:
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100      --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG  &
            #SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt             -n 20       --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunPrompt &
            #SKFlat.py -a $analyzer  -l $mcpath/PromptSS2.txt            -n 100      --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags $FLAG,RunPrompt &
            #SKFlat.py -a $analyzer  -l $mcpath/Conv.txt                 -n 10       --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunConv &
            #SKFlat.py -a $analyzer  -l $mcpath/CF.txt                  -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunCF &
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunFake   &
            #SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags $FLAG,RunFake  &
            #SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100      --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags $FLAG  &


if [[ $1 == "CF" ]]; then

    declare  -a era_list=("2017")
    declare  -a flag_list=( "SS_CR" "VV_VR")

    for i in "${era_list[@]}"
    do
                                                                                                                                                                                                         
        for j in "${flag_list[@]}"
        do
            FLAG=$j
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt   -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags $FLAG,RunCF &
            SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt   -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags $FLAG,RunCF &
	    SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt   -n $njobs   --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags $FLAG,RunCF &
        done
    done

fi




if [[ $1 == "OSEMu" ]]; then

    declare  -a era_list=("2017")
    FLAG="OS_VR"

    for i in "${era_list[@]}"
    do
        #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &
        #SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG  &
        #SKFlat.py -a $analyzer  -l $mcpath/PromptOS.txt             -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunPrompt &
        #SKFlat.py -a $analyzer  -l $mcpath/ConvOS.txt               -n 200    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunConv   &
        #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt      -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake   &
        SKFlat.py -a $analyzer  -l $datapath/${i}_SingleLepton.txt  -n 100    --nmax ${nmax}   -e ${i}  --skim SkimTree_DileptonBDT  --userflags $FLAG,RunFake  &

    done

fi

