# Place this at CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter

import os
import commands as cmd
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('dirNames',nargs='+')
args = parser.parse_args()

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

input_path = os.getcwd()

#tags = ["_sronly"]
tags = ["_syst"]

for dirName in args.dirNames:
  cards = cmd.getoutput('ls '+dirName+' | grep sr3_inv').replace('_sr3_inv.txt','').split('\n')
  dirName = dirName.replace('/','')
  with open("RunList_"+dirName+".txt",'w') as f:
    for card in cards:
      for tag in tags:
        this = input_path+"/"+dirName+"/"+card+tag+".root\n"
        f.write(this)
