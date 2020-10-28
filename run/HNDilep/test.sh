SKFlat.py -a HNtypeI_Dilepton  -i WZTo3LNu_powheg --skim SkimTree_Dilepton -n 100  --nmax 150  -y 2016&
SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_cfdata_ee.txt --skim SkimTree_Dilepton -n 50  --userflags isEE --nmax 150  -y 2016&
SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2016_cfdata_mm.txt --skim SkimTree_Dilepton -n 50  --userflags isMM  --nmax 150  -y 2016
SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_cfdata_ee.txt --skim SkimTree_Dilepton -n 50  --userflags isEE --nmax 150  -y 2017
SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2017_cfdata_mm.txt --skim SkimTree_Dilepton -n 50  --userflags isMM  --nmax 150  -y 2017

SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_cfdata_ee.txt --skim SkimTree_Dilepton -n 50  --userflags isEE --nmax 150  -y 2018
SKFlat.py -a HNtypeI_Dilepton  -l DataJA/2018_cfdata_mm.txt --skim SkimTree_Dilepton -n 50  --userflags isMM  --nmax 150  -y 2018

#SKFlat.py -a HNtypeI_Dilepton  -l lists/Sig_cf2016.txt  -n 100   --userflags Signal --nmax 150  -y 2016

