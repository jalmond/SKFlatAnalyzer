if [[ "$1"  == "DATA"  ||  "$1"  == "data" ]];
then
    SKFlat.py -a HNtypeI_JA  -l DataJA/2016_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE --nmax 150  -y 2016
    SKFlat.py -a HNtypeI_JA  -l DataJA/2017_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE --nmax 150  -y 2017
    SKFlat.py -a HNtypeI_JA  -l DataJA/2018_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE --nmax 150  -y 2018
    SKFlat.py -a HNtypeI_JA  -l DataJA/2016_data_mm.txt --skim SkimTree_SSNonIso -n 100  --userflags isMM --nmax 150  -y 2016
    SKFlat.py -a HNtypeI_JA  -l DataJA/2017_data_mm.txt --skim SkimTree_SSNonIso -n 100  --userflags isMM --nmax 150  -y 2017
    SKFlat.py -a HNtypeI_JA  -l DataJA/2018_data_mm.txt --skim SkimTree_SSNonIso -n 100  --userflags isMM --nmax 150  -y 2018
elif [[ "$1"  == "MC"  ||  "$1" == "mc" ]];
then
    SKFlat.py -a HNtypeI_JA  -l lists/MC_2016_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 100  --nmax 150  -y 2016
    SKFlat.py -a HNtypeI_JA  -l lists/MC_2017_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 100  --nmax 150  -y 2017
    SKFlat.py -a HNtypeI_JA  -l lists/MC_2018_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 100  --nmax 150  -y 2018
elif [[ "$1"  == "SIG"  ||  "$1" == "sig" ]];
then

    SKFlat.py -a HNtypeI_JA  -l MCJA/Sig2016.txt  -n 100   --userflags Signal --nmax 150  -y 2016
    SKFlat.py -a HNtypeI_JA  -l MCJA/Sig2016.txt  --userflags SignalOS -n 100  --nmax 150  -y 2016
else
    
    #SKFlat.py -a HNtypeI_JA  -l DataJA/2016_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE --nmax 150  -y 2016
    #SKFlat.py -a HNtypeI_JA  -l DataJA/2017_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE --nmax 150  -y 2017
    #SKFlat.py -a HNtypeI_JA  -l DataJA/2018_data_ee.txt --skim SkimTree_SSNonIso -n 100  --userflags isEE --nmax 150  -y 2018
    #SKFlat.py -a HNtypeI_JA  -l DataJA/2016_data_mm.txt --skim SkimTree_SSNonIso -n 100  --userflags isMM --nmax 150  -y 2016
    #SKFlat.py -a HNtypeI_JA  -l DataJA/2017_data_mm.txt --skim SkimTree_SSNonIso -n 100  --userflags isMM --nmax 150  -y 2017
    #SKFlat.py -a HNtypeI_JA  -l DataJA/2018_data_mm.txt --skim SkimTree_SSNonIso -n 100  --userflags isMM --nmax 150  -y 2018
    SKFlat.py -a HNtypeI_JA  -l lists/MC_2016_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 100  --nmax 150  -y 2016
    SKFlat.py -a HNtypeI_JA  -l lists/MC_2017_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 100  --nmax 150  -y 2017
    SKFlat.py -a HNtypeI_JA  -l lists/MC_2018_SkimTree_SSNonIso.txt --skim SkimTree_SSNonIso -n 100  --nmax 150  -y 2018
    SKFlat.py -a HNtypeI_JA  -l MCJA/Sig2016.txt  -n 100   --userflags Signal --nmax 150  -y 2016
    SKFlat.py -a HNtypeI_JA  -l MCJA/Sig2016.txt  --userflags SignalOS -n 100  --nmax 150  -y 2016

fi

