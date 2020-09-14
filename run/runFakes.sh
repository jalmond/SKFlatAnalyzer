if [[ "$1"  == "DATA"  ||  SkimTree_SSNonIso"$1"  == "data" ]];
then
    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2016
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2018
    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_mm.txt --skim SkimTree_HNFake -n 100  --userflags isMM --nmax 150  -y 2016
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_mm.txt --skim SkimTree_HNFake -n 100  --userflags isMM --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_m.txt  -n 100  --userflags isSingleMu --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_mm.txt --skim SkimTree_HNFake -n 100  --userflags isMM --nmax 150  -y 2018
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_m.txt -n 100  --userflags isSingleMu --nmax 150  -y 2018
elif [[ "$1"  == "MC"  ||  "$1" == "mc" ]];
then
    SKFlat.py -a FakeRateHN  -l lists/MC_2016_SkimTree_NonIso.txt --skim SkimTree_NonIso -n 100  --nmax 150  -y 2016
    SKFlat.py -a FakeRateHN  -l lists/MC_2017_SkimTree_NonIso.txt --skim SkimTree_NonIso -n 100  --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l lists/MC_2018_SkimTree_NonIso.txt --skim  -n 100  --nmax 150  -y 2018

else
    
    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2016
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2018
    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_mm.txt --skim SkimTree_HNFake -n 100  --userflags isMM --nmax 150  -y 2016
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_mm.txt --skim SkimTree_HNFake -n 100  --userflags isMM --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_mm.txt --skim SkimTree_HNFake -n 100  --userflags isMM --nmax 150  -y 2018

    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_m.txt  -n 100  --userflags isSingleMu --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_m.txt -n 100  --userflags isSingleMu --nmax 150  -y 2018

    SKFlat.py -a FakeRateHN  -l lists/MC_2016_SkimTree_NonIso.txt --skim SkimTree_NonIso -n 100  --nmax 150  -y 2016
    SKFlat.py -a FakeRateHN  -l lists/MC_2017_SkimTree_NonIso.txt --skim SkimTree_NonIso -n 100  --nmax 150  -y 2017
    SKFlat.py -a FakeRateHN  -l lists/MC_2018_SkimTree_NonIso.txt --skim SkimTree_NonIso -n 100  --nmax 150  -y 2018


fi

