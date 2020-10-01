SKFlat.py -a HNtypeI_Dilepton  -i WZ_pythia --skim SkimTree_SSNonIso -n 100  --nmax 150  -y 2016

SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_ee.txt --skim SkimTree_SSNonIso -n 150  --userflags isEE,RunFake --nmax 150  -y 2016
SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_data_mm.txt --skim SkimTree_SSNonIso -n 150  --userflags isMM,RunFake  --nmax 150  -y 2016

SKFlat.py -a HNtypeI_Dilepton  -l lists/Sig_2016.txt  -n 100   --userflags Signal --nmax 150  -y 2016

