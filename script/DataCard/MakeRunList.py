# Place this at /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter

import commands as cmd

#mylist = cmd.getoutput("ls /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/*.root | grep HNL_UL.root")
#with open("RunList.txt",'w') as f:
#  f.write(mylist)

years = ["2016","2017","2018"]
years = ["2017"]
masses = ["M5000","M7500","M10000","M15000","M20000"]
masses = ["M100","M1000","M10000"]
masses = ["M100","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
masses = ["M100","M200","M300","M400","M500"]
channels = ["MuMu","EE","EMu"]
channels = ["MuMu","EE"]
input_path = "/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/"
myWPs = ["Workshop", "InputForCombine"]
#myWPs = ["InputForCombine"]

for WP in myWPs:
  with open("RunList_"+WP+".txt",'w') as f:
    for year in years:
      for channel in channels:
        for mass in masses:
          card = input_path+WP+"/card_"+year+"_"+channel+"_"+mass+"_HNL_UL.root\n"
          f.write(card)
