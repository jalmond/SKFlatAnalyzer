### SETUP RUN OPTIONS
analyzer=HNL_SignalLeptonOpt
rundir=HNLOpt/HNL_SignalLeptonOpt

# PATHS                                                                                                                                                    
runPATH=${SKFlat_WD}/runJobs/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

#SKIM                                                                                                                                                     
skim=' '

# JOB CONFIG                                                                                                                                               
njobs=25
nLargejobs=50
njobs_sig=30
njobs_data=10
nmax=350


if [[ $1 == "" ]]; then

    declare  -a era_list=("2017" )

    for i in "${era_list[@]}"
    do
	
	#SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT &  
	#SKFlat.py -a $analyzer  -l $sigpath/DY.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
	SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private    -n 1   --nmax 1   -e ${i}  --skim SkimTree_HNMultiLepBDT  &
    done
fi


if [[ $1 == "ELECTRON_BASIC" ]]; then

    declare  -a era_list=("2017" )

    for i in "${era_list[@]}"
    do

	Flag='RunEE,ELID'
        source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}


    done
fi

if [[ $1 == "ELECTRON_CF" ]]; then

    declare  -a era_list=("2017" )

    for i in "${era_list[@]}"
    do

        #Flag='RunEE,ELID_CF,BB'
	#source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}

        #Flag2='RunEE,ELID_CF,EC'
	#source ${runPATH}/run_hnl.sh Electron ${Flag2} ${i}

	Flag='RunEE,ELID_CF,FullEta'
	source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}


    done
fi

if [[ $1 == "ELID_NP_CF_HighPt" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do
	
        #Flag='RunEE,ELID_NP_CF,HighPt,BB'
        #source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}
	Flag2='RunEE,ELID_NP_CF,HighPt,EC'
	source ${runPATH}/run_hnl.sh Electron ${Flag2} ${i}

    done
fi

if [[ $1 == "ELID_NP_CF" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

        Flag='RunEE,ELID_NP_CF'                                                                                                                                  
        source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}                                                                                                                 

    done
fi

if [[ $1 == "ELID_CONV" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP"  "2018")

    for i in "${era_list[@]}"
    do

        Flag='RunEE,ELID_Conv'
        source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}

    done
fi
if [[ $1 == "ELID_ISO" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP"  "2018")

    for i in "${era_list[@]}"
    do

        Flag='RunEE,ELID_Iso'
        source ${runPATH}/run_hnl.sh Electron ${Flag} ${i}

    done
fi


#############################################################################                                                                                                                                                               
#############################################################################                                                                                                                                                               
#############################################################################     

if [[ $1 == "NP_MUON_FullEta" ]]; then

    #declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
    declare  -a era_list=("2017")
    for i in "${era_list[@]}"
    do

        Flag='MuID_NP,FullEta'
        source ${runPATH}/run_hnl.sh Muon ${Flag} ${i}

    done

fi

if [[ $1 == "NP_MUON_DXY" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
    for i in "${era_list[@]}"
    do

        Flag='MuID_DXY'
        source ${runPATH}/run_hnl.sh Muon ${Flag} ${i}

    done

fi

if [[ $1 == "NP_MUON" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")
    for i in "${era_list[@]}"
    do

	Flag='MuID_NP,BB'
	source ${runPATH}/run_hnl.sh Muon ${Flag} ${i}

	
	Flag2='MuID_NP,EC'
	source ${runPATH}/run_hnl.sh Muon ${Flag2} ${i}

    done

fi

if [[ $1 == "Mu_Final" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

        Flag='Mu_Final'
        source ${runPATH}/run_hnl.sh Muon ${Flag} ${i}


    done

fi

if [[ $1 == "Mu_comp" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

        Flag='Mu_Comp'
        source ${runPATH}/run_hnl.sh Muon ${Flag} ${i}


    done

fi


if [[ $1 == "MUON_COMP" ]]; then

    declare  -a era_list=("2016postVFP" "2016preVFP" "2017" "2018")

    for i in "${era_list[@]}"
    do

    SKFlat.py -a $analyzer  -l $mcpath/MCOpt.txt  -n 20  --nmax ${nmax}   -e ${i}   --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/MCOptLarge.txt  -n 50  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $sigpath/SSWWOpt.txt  -n $njobs_sig  --nmax ${nmax}  -e ${i}    &
    SKFlat.py -a $analyzer  -l $sigpath/DYOpt.txt    -n $njobs_sig  --nmax ${nmax}   -e ${i}  &
    SKFlat.py -a $analyzer  -l $sigpath/VBFOpt.txt   -n $njobs_sig  --nmax ${nmax}   -e ${i}  &
    SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n 10  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT  --userflags RunConv &

    done

fi





#############################################################################
#############################################################################
#############################################################################




if [[ $1 == "Muon" ]]; then
    
    
    source ${runPATH}/run_hnl.sh MC  ${3} ${2}  
    source ${runPATH}/run_hnl.sh Signals ${3} ${2} 
    
    ConvFlag=${2}',RunConv'

    source ${runPATH}/run_hnl.sh Conv ${3} ${ConvFlag} 
    

fi

if [[ $1 == "Electron" ]]; then

    # If no inut then this is ran                                                                                                                        
    source ${runPATH}/run_hnl.sh Signals ${3} ${2} 
    source ${runPATH}/run_hnl.sh MC  ${3} ${2}
    
    CFFlag=${2}',RunCF'
    source ${runPATH}/run_hnl.sh CF ${3} ${CFFlag}
    ConvFlag=${2}',RunConv'
    source ${runPATH}/run_hnl.sh Conv ${3} ${ConvFlag}
    

fi



if [[ $1 == "MC" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/MCOpt.txt  -n $njobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/MCOptLarge.txt  -n $nLargejobs  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &
    SKFlat.py -a $analyzer  -l $mcpath/MCOptXLarge.txt  -n 100  --nmax 400   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT &

fi

if [[ $1 == "Signals" ]]; then

    SKFlat.py -a $analyzer  -l $sigpath/SSWWOpt.txt  -n $njobs_sig  --nmax ${nmax}  -e ${2}  --userflags ${3}  --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/DYOpt.txt    -n $njobs_sig  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&
    SKFlat.py -a $analyzer  -l $sigpath/VBFOpt.txt   -n $njobs_sig  --nmax ${nmax}   -e ${2}  --userflags ${3} --skim SkimTree_HNMultiLepBDT&

fi


if [[ $1 == "CF" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/CF.txt  -n $njobs  --nmax ${nmax}   -e ${2} --skim SkimTree_HNMultiLepBDT  --userflags ${3} &
    #SKFlat.py -a $analyzer  -l $datapath/DATA_${i}.txt  -n ${njobs_data}  --nmax ${nmax}   -e ${i} --userflags RunCF  &                                 

fi


if [[ $1 == "Conv" ]]; then

    SKFlat.py -a $analyzer  -l $mcpath/Conv.txt  -n 5  --nmax ${nmax}   -e ${2} --skim SkimTree_HNMultiLepBDT  --userflags ${3} &
fi



if [[ $1 == "TMP" ]]; then
    
    Flag='MuID_NP,FullPt,BB'
    
    SKFlat.py -a $analyzer  -i DYTypeI_DF_M100_private  -n 20  --nmax 1000   -e 2017  --userflags $Flag   --skim SkimTree_HNMultiLepBDT&
       

fi
