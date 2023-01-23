# Make Datacards
# Place this at /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace

import os

pwd = os.getcwd()

with open("/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/card_skeleton.txt",'r') as f:
  lines = f.readlines()

#print lines

#input_path = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter/"
input_path = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/Before_Merge_230119/HNL_SignalRegionPlotter/"
eras = ["2016","2017","2018"]
#eras = ["2016"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]
masses = ["M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000"]
masses = ["M5000","M7500","M10000","M15000","M20000"]
masses = ["M100","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]

myWPs = ["Workshop", "InputForCombine"]
myWPs = ["InputForCombine"]
myWPs = ["Workshop"]

doCombine = True

for WP in myWPs:
  #os.system("mkdir -p "+WP)
  #os.system("ln -s /data6/Users/jihkim/SKFlatAnalyzer/script/DataCard/MakeWorkspace.py "+WP)
  #for era in eras:
  #  for channel in channels:
  #    for mass in masses:
  #      lines[4] = "shapes * *  "+input_path+WP+"/"+era+"/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
  #      if int(mass.replace("M","")) < 500:
  #        if channel == "MuMu": lines[17] = "rate          -1      0    0     -1           0\n"
  #        else: lines[17] = "rate          -1      0    -1     -1           0\n"
  #      if 500 <= int(mass.replace("M","")) and int(mass.replace("M","")) <= 3000:
  #        if channel == "MuMu": lines[17] = "rate          -1      0    0     -1           -1\n"
  #        else: lines[17] = "rate          -1      0    -1     -1           -1\n"
  #      elif 3000 < int(mass.replace("M","")):
  #        if channel == "MuMu": lines[17] = "rate          -1      0    0     0           -1\n"
  #        else: lines[17] = "rate          -1      0    -1     0           -1\n"
  #      with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_HNL_UL.txt",'w') as f:
  #        for line in lines:
  #          f.write(line)

  if doCombine:
    os.chdir(WP)
    for channel in channels:
      for mass in masses:
        os.system("combineCards.py year16=card_2016_"+channel+"_"+mass+"_HNL_UL.txt year17=card_2017_"+channel+"_"+mass+"_HNL_UL.txt year18=card_2018_"+channel+"_"+mass+"_HNL_UL.txt > card_Run2_"+channel+"_"+mass+"_HNL_UL.txt")
    os.chdir(pwd)
