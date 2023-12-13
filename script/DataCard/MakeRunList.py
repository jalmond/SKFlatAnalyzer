# Place this at CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter

import commands as cmd

#mylist = cmd.getoutput("ls /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/*.root | grep HNL_UL.root")
#with open("RunList.txt",'w') as f:
#  f.write(mylist)

years = ["2016","2017","2018"]
years = ["Run2"]
years = ["2017"]
years = ["2016preVFP","2016postVFP","2017","2018","Run2"]
masses = ["M5000","M7500","M10000","M15000","M20000"]
masses = ["M90","M100","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]
#masses = ["M100","M1000","M10000"]
#masses = ["M500"]
channels = ["MuMu","EE","EMu"]
channels = ["MuMu","EE"]
input_path = "/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/"
#myWPs = ["Workshop", "InputForCombine"]
#myWPs = ["InputForCombine"]
#myWPs = ["Workshop"]
#myWPs = ["AddSyst"]
#myWPs = ["KPS23Spr"]
#myWPs = ["KPS23Spr_woBDT"]
#myWPs = ["SR2HT_SR3l2pt"]
#myWPs = ["SR2HT_SR3l2pt_ChargeSplit"]
#myWPs = ["HNL_ULID","HNTightV2"]
myWPs = ["NewOpt_HNL_ULID","NewOpt_HNTightV2"]

for WP in myWPs:
  with open("RunList_"+WP+".txt",'w') as f:
    for year in years:
      for channel in channels:
        for mass in masses:
          card = input_path+WP+"/card_"+year+"_"+channel+"_"+mass+".root\n"
          f.write(card)
