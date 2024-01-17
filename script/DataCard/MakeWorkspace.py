# convert data cards into work space root files
# run this at /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/myWP

import commands as cmd

# Use this if you want to convert all datacards in this directory
#cards = cmd.getoutput("ls *HNL_UL.txt").split('\n')
#card_names = cmd.getoutput("ls *.txt").replace(".txt","").split('\n')
#
#for i in range(len(cards)):
#  print cmd.getoutput("text2workspace.py -P HiggsAnalysis.CombinedLimit.MyModel:myModel "+cards[i]+" -o "+card_names[i]+".root")

years = ["2016","2017","2018"]
years = ["Run2"]
years = ["2017"]
#years = ["2016preVFP","2016postVFP","2018","Run2"]
masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]
#masses = ["M100","M200","M500","M600","M1000"]
channels = ["MuMu","EE","EMu"]
tags = ["_sr","_sr_inv","_sronly",""]

masses = ["M100"]
channels = ["EE"]
#tags = ["_sr_syst","_syst",""]
tags = ["_sr"]

for year in years:
  for channel in channels:
    for mass in masses:
      for tag in tags:
        card = "card_"+year+"_"+channel+"_"+mass+tag+".txt"
        card_name = card.replace(".txt","")
        if channel == "EMu":
          log = cmd.getoutput("text2workspace.py -P HiggsAnalysis.CombinedLimit.HNDilepModel:hnDilepModel_EMu "+card+" -o "+card_name+".root")
        else:
          log = cmd.getoutput("text2workspace.py -P HiggsAnalysis.CombinedLimit.HNDilepModel:hnDilepModel "+card+" -o "+card_name+".root")
        print "!!!!!!!!!!!!!!!!!!!!!!!","Processing",card_name,"...","!!!!!!!!!!!!!!!!!!!!!!!"
        print log
        with open(card_name+".log",'w') as f:
          f.write(log)
