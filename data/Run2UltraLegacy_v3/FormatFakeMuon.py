import os 

print ("Moving 2016a FR files")
MV2016a="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/FR_Mu_2016a_* 2016preVFP/FakeRate/DataFR/MuFR/"
MV2016b="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/FR_Mu_2016b_* 2016postVFP/FakeRate/DataFR/MuFR/"
MV2017 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/FR_Mu_2017_*  2017/FakeRate/DataFR/MuFR/"
MV2018 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/FR_Mu_2018_*  2018/FakeRate/DataFR/MuFR/"

MVPr2016a="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/PR_Mu_2016a* 2016preVFP/FakeRate/DataFR/MuFR/"
MVPr2016b="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/PR_Mu_2016b* 2016postVFP/FakeRate/DataFR/MuFR/"
MVPr2017 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/PR_Mu_2017* 2017/FakeRate/DataFR/MuFR/"
MVPr2018 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/Muon/PR_Mu_2018* 2018/FakeRate/DataFR/MuFR/"

os.system(MV2016a)
#os.system(MVPr2016a)
os.system("ls -lth 2016preVFP/FakeRate/DataFR/MuFR/")

os.system(MV2016b)
#os.system(MVPr2016b)
os.system("ls -lth 2016postVFP/FakeRate/DataFR/MuFR/")

os.system(MV2017)
#os.system(MVPr2017)
os.system("ls -lth 2017/FakeRate/DataFR/MuFR/")

os.system(MV2018)
#os.system(MVPr2018)
os.system("ls -lth 2018/FakeRate/DataFR/MuFR/")


