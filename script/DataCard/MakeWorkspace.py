# convert data cards into work space root files
# run this at /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/myWP

import commands as cmd

#cards = cmd.getoutput("ls *HNL_UL.txt").split('\n')
#card_names = cmd.getoutput("ls *.txt").replace(".txt","").split('\n')
#
#for i in range(len(cards)):
#  print cmd.getoutput("text2workspace.py -P HiggsAnalysis.CombinedLimit.MyModel:myModel "+cards[i]+" -o "+card_names[i]+".root")

years = ["2016","2017","2018"]
years = ["Run2"]
years = ["2017"]
#years = ["2016preVFP","2016postVFP","2018","Run2"]
masses = ["M5000","M7500","M10000","M15000","M20000"]
masses = ["M100","M1000","M10000"]
masses = ["M90","M100","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]
masses = ["M500"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]

for year in years:
  for channel in channels:
    for mass in masses:
      card = "card_"+year+"_"+channel+"_"+mass+".txt"
      card_name = card.replace(".txt","")
      log = cmd.getoutput("text2workspace.py -P HiggsAnalysis.CombinedLimit.MyModel:myModel "+card+" -o "+card_name+".root")
      print log
      with open(card_name+".log",'w') as f:
        f.write(log)
