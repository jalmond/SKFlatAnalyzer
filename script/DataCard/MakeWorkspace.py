# convert data cards into work space root files
# run this at /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace

import commands as cmd

cards = cmd.getoutput("ls *.txt").split('\n')
card_names = cmd.getoutput("ls *.txt").replace(".txt","").split('\n')

for i in range(len(cards)):
  print cmd.getoutput("text2workspace.py -P HiggsAnalysis.CombinedLimit.MyModel:myModel "+cards[i]+" -o "+card_names[i]+".root")
