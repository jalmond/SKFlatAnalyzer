if [[ "$1"  == "DATA"  ||  "$1"  == "data" ]];
then
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_ee.txt --skim SkimTree_SSHN -n 100  --userflags isEE --nmax 150  -y 2016
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_ee.txt --skim SkimTree_SSHN -n 100  --userflags isEE --nmax 150  -y 2017
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_ee.txt --skim SkimTree_SSHN -n 100  --userflags isEE --nmax 150  -y 2018
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_mm.txt --skim SkimTree_SSHN -n 100  --userflags isMM --nmax 150  -y 2016
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_mm.txt --skim SkimTree_SSHN -n 100  --userflags isMM --nmax 150  -y 2017
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_mm.txt --skim SkimTree_SSHN -n 100  --userflags isMM --nmax 150  -y 2018
elif [[ "$1"  == "MC"  ||  "$1" == "mc" ]];
then
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2016_SkimTree_SSHN.txt --skim SkimTree_SSNonIso -n 30  --nmax 100  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2017_SkimTree_SSHN.txt --skim SkimTree_SSNonIso -n 30  --nmax 100  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2018_SkimTree_SSHN.txt --skim SkimTree_SSNonIso -n 30  --nmax 100  -y 2018 &
    #SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2016_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 50  --nmax 50  -y 2016 &
    #SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2017_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 50  --nmax 50  -y 2017 &
    #SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2018_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 50  --nmax 50  -y 2018 &

    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2016_SkimTree_SSHNbig.txt --skim SkimTree_SSHN -n 400  --nmax 100  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2017_SkimTree_SSHNbig.txt --skim SkimTree_SSHN -n 400  --nmax 100  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2018_SkimTree_SSHNbig.txt --skim SkimTree_SSHN -n 400  --nmax 100  -y 2018 #                                   

elif [[ "$1"  == "SIG"  ||  "$1" == "sig" ]];
then

    SKFlat.py -a HNtypeI_Dilepton  -l lists/SigEE.txt -n 100   --userflags Signal --nmax 150  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/SigEE.txt -n 100   --userflags Signal --nmax 150  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/SigEE.txt -n 100   --userflags Signal --nmax 150  -y 2018 &

else
    
    
    ####   RUNNING CF 
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE,RunCF --nmax 150  -y 2016    &
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE,RunCF --nmax 150  -y 2017    &
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE,RunCF --nmax 150  -y 2018  &
    
    ## RUNNING FAKE
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_ee.txt --skim SkimTree_SSHN -n 10  --userflags isEE,RunFake --nmax 150  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_ee.txt --skim SkimTree_SSHN -n 10  --userflags isEE,RunFake  --nmax 150  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_ee.txt --skim SkimTree_SSHN -n 10  --userflags isEE,RunFake  --nmax 150  -y 2018 
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_mm.txt --skim SkimTree_SSHN -n 10  --userflags isMM,RunFake  --nmax 150  -y 2016 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_mm.txt --skim SkimTree_SSHN -n 10  --userflags isMM,RunFake  --nmax 150  -y 2017 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_mm.txt --skim SkimTree_SSHN -n 10  --userflags isMM,RunFake --nmax 150  -y 2018 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_m.txt  --skim SkimTree_SSHN -n 10  --userflags isSingleMu,RunFake --nmax 150  -y 2016 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_m.txt  --skim SkimTree_SSHN -n 10  --userflags isSingleMu,RunFake --nmax 150  -y 2017 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_m.txt --skim SkimTree_SSHN -n 10  --userflags isSingleMu,RunFake --nmax 150  -y 2018 &

    #### RUNNING #DATA
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_ee.txt --skim SkimTree_SSHN -n 10  --userflags isEE --nmax 150  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_ee.txt --skim SkimTree_SSHN -n 10  --userflags isEE --nmax 150  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_ee.txt --skim SkimTree_SSHN -n 10  --userflags isEE --nmax 150  -y 2018 

    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_m.txt  --skim SkimTree_SSHN -n 10  --userflags isSingleMu --nmax 150  -y 2016 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_m.txt  --skim SkimTree_SSHN -n 10  --userflags isSingleMu --nmax 150  -y 2017 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_m.txt  --skim SkimTree_SSHN -n 10  --userflags isSingleMu --nmax 150  -y 2018 &

    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_mm.txt --skim SkimTree_SSHN -n 10  --userflags isMM --nmax 150  -y 2016 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_data_mm.txt --skim SkimTree_SSHN -n 10  --userflags isMM --nmax 150  -y 2017 &
    #SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_data_mm.txt --skim SkimTree_SSHN -n 10  --userflags isMM --nmax 150  -y 2018


    ### Running MC
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2016_SkimTree_SSHN.txt --skim SkimTree_SSNonIso -n 30  --nmax 150  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2017_SkimTree_SSHN.txt --skim SkimTree_SSNonIso -n 30  --nmax 150  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2018_SkimTree_SSHN.txt --skim SkimTree_SSNonIso -n 30  --nmax 150  -y 2018 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2016_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 50  --nmax 150  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2017_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 50  --nmax 150  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2018_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 50  --nmax 150  -y 2018 
    
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2016_SkimTree_SSHNbig.txt --skim SkimTree_SSHN -n 400  --nmax 150  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2017_SkimTree_SSHNbig.txt --skim SkimTree_SSHN -n 400  --nmax 150  -y 2017 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/MC_2018_SkimTree_SSHNbig.txt --skim SkimTree_SSHN -n 400  --nmax 150  -y 2018 #

    SKFlat.py -a HNtypeI_Dilepton  -l lists/Sig_2016.txt  -n 2   --userflags Signal --nmax 150  -y 2016 &
    SKFlat.py -a HNtypeI_Dilepton  -l lists/Sig_2016.txt  --userflags SignalOS -n 2  --nmax 150  -y 2016 
    
fi
