if [[ "$1"  == "DATA"  ||  SkimTree_SSNonIso"$1"  == "data" ]];
then
    
    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2016 &                              
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2017 &                              
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2018 &                              
    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2018 &
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_m.txt  -n 10  --skim SkimTree_HNFake --userflags isSingleMu --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_m.txt -n 10  --skim SkimTree_HNFake --userflags isSingleMu --nmax 150  -y 2018


elif [[ "$1"  == "DATAMUON"  ||  "$1" == "mc" ]];
then
        SKFlat.py -a FakeRateHN  -l DataFake/2016_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2018 &
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_m.txt  -n 10  --skim SkimTree_HNFake --userflags isSingleMu --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_m.txt -n 10  --skim SkimTree_HNFake --userflags isSingleMu --nmax 150  -y 2018


elif [[ "$1"  == "DATAEG"  ||  "$1" == "mc" ]];
then

    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2018 &


elif [[ "$1"  == "MC"  ||  "$1" == "mc" ]];
then

    SKFlat.py -a FakeRateHN  -l lists/MC_2016.txt -n 1000  --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2017.txt -n 1000  --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2018.txt -n 1000  --nmax 150  -y 2018 &

    SKFlat.py -a FakeRateHN  -l lists/MC_2016_SkimTree_HNFake.txt --skim SkimTree_HNFake -n 200  --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2017_SkimTree_HNFake.txt --skim SkimTree_HNFake -n 200  --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2018_SkimTree_HNFake.txt --skim SkimTree_HNFake -n 200  --nmax 150  -y 2018


else
    
#
#    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2016 &
#    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2017 &
#    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_ee.txt --skim SkimTree_HNFake -n 100  --userflags isEE --nmax 150  -y 2018 &
    SKFlat.py -a FakeRateHN  -l DataFake/2016_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_mm.txt --skim SkimTree_HNFake -n 10  --userflags isMM --nmax 150  -y 2018 &
    SKFlat.py -a FakeRateHN  -l DataFake/2017_data_m.txt  -n 10  --skim SkimTree_HNFake --userflags isSingleMu --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l DataFake/2018_data_m.txt -n 10  --skim SkimTree_HNFake --userflags isSingleMu --nmax 150  -y 2018

    SKFlat.py -a FakeRateHN  -l lists/MC_2016.txt -n 1000  --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2017.txt -n 1000  --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2018.txt -n 1000  --nmax 150  -y 2018 &

    SKFlat.py -a FakeRateHN  -l lists/MC_2016_SkimTree_HNFake.txt --skim SkimTree_HNFake -n 200  --nmax 150  -y 2016 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2017_SkimTree_HNFake.txt --skim SkimTree_HNFake -n 200  --nmax 150  -y 2017 &
    SKFlat.py -a FakeRateHN  -l lists/MC_2018_SkimTree_HNFake.txt --skim SkimTree_HNFake -n 200  --nmax 150  -y 2018


fi

