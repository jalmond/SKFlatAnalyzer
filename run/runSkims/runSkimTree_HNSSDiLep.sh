#SKFlat.py -a SkimTree_SSHN -l DATADilep/2016_data.txt --nmax 100 --skim SkimTree_SSNonIso -n 150 -y 2016
#SKFlat.py -a SkimTree_SSHN -l DATADilep/2017_data.txt --nmax 200 --skim SkimTree_SSNonIso -n 150 -y 2017
#SKFlat.py -a SkimTree_SSHN -l DATADilep/2018_data.txt --nmax 200 --skim SkimTree_SSNonIso -n 200 -y 2018
#SKFlat.py -a SkimTree_SSHN -l DATADilep/2016_singledata.txt --nmax 200 --skim SkimTree_SSNonIso --userflags isSingleMu -n 50 -y 2016
#SKFlat.py -a SkimTree_SSHN -l DATADilep/2017_singledata.txt --nmax 200 --skim SkimTree_SSNonIso --userflags isSingleMu -n 50 -y 2017
#SKFlat.py -a SkimTree_SSHN -l DATADilep/2018_singledata.txt --nmax 200 --skim SkimTree_SSNonIso  --userflags isSingleMu -n 50 -y 2018                                                       
SKFlat.py -a SkimTree_SSHN -l MCDilep/MC_2016.txt  -n 1000  -y 2016 --skim SkimTree_SSNonIso  --nmax 200 &
#SKFlat.py -a SkimTree_SSHN -l MCDilep/MC_2016big.txt  -n 1000  -y 2016 --skim SkimTree_SSNonIso  --nmax 200 &
SKFlat.py -a SkimTree_SSHN -l MCDilep/MC_2017.txt -n 1000  -y 2017 --skim SkimTree_SSNonIso  --nmax 200 &
#SKFlat.py -a SkimTree_SSHN -l MCDilep/MC_2017big.txt -n 1000  -y 2017 --skim SkimTree_SSNonIso  --nmax 200 &
SKFlat.py -a SkimTree_SSHN -l MCDilep/MC_2018.txt -n 1000  -y 2018 --skim SkimTree_SSNonIso  --nmax 200
#SKFlat.py -a SkimTree_SSHN -l MCDilep/MC_2018big.txt -n 1000  -y 2018 --skim SkimTree_SSNonIso  --nmax 200
