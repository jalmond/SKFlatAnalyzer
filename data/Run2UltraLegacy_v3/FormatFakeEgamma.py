import os 

print ("Moving 2016a FR files")
MV2016a="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/FR_EGamma_2016a_* 2016preVFP/FakeRate/DataFR/ElFR/"
MV2016b="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/FR_EGamma_2016b_* 2016postVFP/FakeRate/DataFR/ElFR/"
MV2017 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/FR_EGamma_2017_*  2017/FakeRate/DataFR/ElFR/"
MV2018 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/FR_EGamma_2018_*  2018/FakeRate/DataFR/ElFR/"

MVPr2016a="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/PR_EGamma_2016a_* 2016preVFP/FakeRate/DataFR/ElFR/"
MVPr2016b="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/PR_EGamma_2016b_* 2016postVFP/FakeRate/DataFR/ElFR/"
MVPr2017 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/PR_EGamma_2017_* 2017/FakeRate/DataFR/ElFR/"
MVPr2018 ="mv /data6/Users/jalmond/2020/HNDiLeptonWorskspace/src/HNL_Lepton_Fakes/CreateInput/EGamma/PR_EGamma_2018_* 2018/FakeRate/DataFR/ElFR/"

os.system(MV2016a)
#os.system(MVPr2016a)
os.system("ls -lth 2016preVFP/FakeRate/DataFR/ElFR/")

os.system(MV2016b)
#os.system(MVPr2016b)
os.system("ls -lth 2016postVFP/FakeRate/DataFR/ElFR/")

os.system(MV2017)
#os.system(MVPr2017)
os.system("ls -lth 2017/FakeRate/DataFR/ElFR/")

os.system(MV2018)
#os.system(MVPr2018)
os.system("ls -lth 2018/FakeRate/DataFR/ElFR/")


