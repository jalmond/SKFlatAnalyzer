SKFlat.py -a SkimTree_SSNonIso -l DATADilep/2016_data.txt --nmax 150 --skim SkimTree_Dilepton -n 100 -y 2016 &
SKFlat.py -a SkimTree_SSNonIso -l DATADilep/2017_data.txt --nmax 150 --skim SkimTree_Dilepton -n 100 -y 2017 &
SKFlat.py -a SkimTree_SSNonIso -l DATADilep/2018_data.txt --nmax 150 --skim SkimTree_Dilepton -n 100 -y 2018 &
SKFlat.py -a SkimTree_SSNonIso -l DATADilep/2016_singledata.txt --nmax 150 --userflags isSingleMu -n 100 -y 2016 &
SKFlat.py -a SkimTree_SSNonIso -l DATADilep/2017_singledata.txt --nmax 150 --userflags isSingleMu -n 100 -y 2017 &
SKFlat.py -a SkimTree_SSNonIso -l DATADilep/2018_singledata.txt --nmax 150 --userflags isSingleMu -n 100 -y 2018                                                       
SKFlat.py -a SkimTree_SSNonIso -l MCDilep/MC_2016.txt -n 100  -y 2016 --nmax 150 &
SKFlat.py -a SkimTree_SSNonIso -l MCDilep/MC_2017.txt -n 100  -y 2017 --nmax 150  &
SKFlat.py -a SkimTree_SSNonIso -l MCDilep/MC_2018.txt -n 100  -y 2018 --nmax 150 &
SKFlat.py -a SkimTree_SSNonIso -l MCDilep/MC_2016big.txt -n 400  -y 2016 --nmax 200 &
SKFlat.py -a SkimTree_SSNonIso -l MCDilep/MC_2017big.txt -n 400  -y 2017 --nmax 200 &
SKFlat.py -a SkimTree_SSNonIso -l MCDilep/MC_2018big.txt -n 400  -y 2018 --nmax 200
