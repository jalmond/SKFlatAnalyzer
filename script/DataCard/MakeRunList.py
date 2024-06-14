# Place this at CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter

import commands as cmd

#mylist = cmd.getoutput("ls /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter/*.root | grep HNL_UL.root")
#with open("RunList.txt",'w') as f:
#  f.write(mylist)
#channels = ["MuMu","EE"]
#for channel in channels:
#  mylist = cmd.getoutput("ls /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter/exo17028_CombinedYears_noSSWW/"+channel).split('\n')
#  mylist = '\n'.join(["/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter/exo17028_CombinedYears_noSSWW/"+channel+"/"+s for s in mylist])
#  with open("RunList_exo17028_noSSWW.txt",'a') as f:
#    f.write(mylist)
#    f.write('\n')

#years = ["2016","2017","2018"]
#years = ["Run2"]
#years = ["2016preVFP","2016postVFP","2017","2018","Run2"]
years = ["2018"]
#masses = ["M5000","M7500","M10000","M15000","M20000"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M85","M90","M95","M100","M125","M150","M200","M250","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]
masses = ["M100","M1000","M10000"]
#masses = ["M500"]
#channels = ["MuMu","EE"]
#channels = ["EMu"]
channels = ["MuMu","EE","EMu"]
input_path = "/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter/"
#InputWPs = ["240422_HNL_ULID"]
#InputWPs = ["240501_1704_HNL_ULID","240501_1704_HNTightV2"]
#InputWPs = ["240504_PR44_HNL_ULID"]
#InputWPs = ["240505_PR45_HNL_ULID"]
#InputWPs = ["240505_PR46_HNL_ULID","240505_PR46_HNTightV2"]
#InputWPs = ["rateParam_HNL_ULID"]
#InputWPs = ["PR48_rateParam_HNL_ULID"]
#InputWPs = ["PR51_HNL_ULID"]
InputWPs = ["PR51_rescale_HNL_ULID"]
#OutputWP = "_NOsr2inv_NOcr1Norm_FixCF"
#OutputWP = "_NOsr2inv_IncNorm"
#OutputWP = "_AltStat"
#OutputWP = "_NoStat"
OutputWP = ""
#tags = ["_sronly"]
tags = ["_syst"]

for InputWP in InputWPs:
  with open("RunList_"+InputWP+OutputWP+".txt",'w') as f:
    for year in years:
      for channel in channels:
        for mass in masses:
          for tag in tags:
            card = input_path+InputWP+OutputWP+"/card_"+year+"_"+channel+"_"+mass+tag+".root\n"
            f.write(card)
