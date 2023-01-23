# Place it at /data6/Users/jihkim/HNDiLeptonWorskspace/Limits/ReadLimits/Shape
# python ReadLimitFromTree.py --Full[--Asymptotic]

from ROOT import *
import os, argparse

parser = argparse.ArgumentParser(description='option')
parser.add_argument('--Asymptotic', action='store_true')
parser.add_argument('--Full', action='store_true')
args = parser.parse_args()

workdir = "/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Batch/"

years = ["2016","2017","2018"]
years = ["Run2"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]
masses = ["100","200","300","400","500","600","700","800","900","1000","1100","1200","1300","1500","1700","2000","2500","3000"]
masses = ["100","200","300","400","500","600","700","800","900","1000","1100","1200","1300","1500","1700","2000","2500","3000","5000","7500","10000","15000","20000"]
#masses = ["100","1000","10000"]
IDs = ["HNL_UL"]
tags = [""] #["_DYVBF"]
myWPs = ["Workshop", "InputForCombine"]
#myWPs = ["InputForCombine"]
myWPs = ["Workshop"]

for WP in myWPs:
  this_workdir = workdir+WP
  os.system("mkdir -p out/"+WP)
  for year, channel, ID, tag in [[year, channel, ID, tag] for year in years for channel in channels for ID in IDs for tag in tags]:
    
    if args.Asymptotic:
      with open("out/"+WP+"/"+year+"_"+channel+"_"+ID+tag+"_"+"Asym_limit.txt", 'w') as f:
  
        for mass in masses:
          this_name = year+"_"+channel+"_M"+mass+"_"+ID+tag
          path = this_workdir+"/Asymptotic/"+this_name+"/output/"+this_name+"_Asymptotic.root"
  
          f_Asym = TFile.Open(path)
          tree_Asym = f_Asym.Get("limit")
  
          tree_Asym.GetEntry(2) # substitute for obs. limit for now
          #f.write(mass+"\t"+str(round(tree_Asym.limit,3))+"\t")
          #f.write(mass+"\t"+str(round(tree_Asym.limit/2.,3))+"\t") # For estimating full Run2
          if 500 <= int(mass) and int(mass) <= 1500:
            f.write(mass+"\t"+str(round((tree_Asym.limit/(2.-0.0005*int(mass))),3))+"\t") # For estimating bkg reduction in SR1 (5-->0.x)
          else:
            f.write(mass+"\t"+str(round(tree_Asym.limit,3))+"\t")
  
          for i in range(5): # expected limits
            tree_Asym.GetEntry(i)
            #f.write(str(round(tree_Asym.limit,3))+"\t")
            #f.write(str(round(tree_Asym.limit/2.,3))+"\t") # For estimating full Run2
            if 500 <= int(mass) and int(mass) <= 1500:
              f.write(str(round((tree_Asym.limit/(2.-0.0005*int(mass))),3))+"\t") # For estimating full Run2
            else:
              f.write(str(round(tree_Asym.limit,3))+"\t")
          f.write("\n")
  
    if args.Full:
      with open("out/"+WP+"/"+year+"_"+channel+"_"+ID+tag+"_"+"Full_limit.txt", 'w') as f:
  
        for mass in masses:
          this_name = year+"_"+channel+"_M"+mass+"_"+ID+tag
          paths = [
                  this_workdir+"/full_CLs/"+this_name+"/output/"+this_name+"_Q1.root",
                  this_workdir+"/full_CLs/"+this_name+"/output/"+this_name+"_Q2.root",
                  this_workdir+"/full_CLs/"+this_name+"/output/"+this_name+"_Q3.root",
                  this_workdir+"/full_CLs/"+this_name+"/output/"+this_name+"_Q4.root",
                  this_workdir+"/full_CLs/"+this_name+"/output/"+this_name+"_Q5.root",
                  ]
  
          f_Q1 = TFile.Open(paths[0])
          f_Q2 = TFile.Open(paths[1])
          f_Q3 = TFile.Open(paths[2])
          f_Q4 = TFile.Open(paths[3])
          f_Q5 = TFile.Open(paths[4])
  
          tree_Q1 = f_Q1.Get("limit")
          tree_Q2 = f_Q2.Get("limit")
          tree_Q3 = f_Q3.Get("limit")
          tree_Q4 = f_Q4.Get("limit")
          tree_Q5 = f_Q5.Get("limit")
          
          tree_Q1.GetEntry(0)
          tree_Q2.GetEntry(0)
          tree_Q3.GetEntry(0)
          tree_Q4.GetEntry(0)
          tree_Q5.GetEntry(0)
          
          f.write(mass+"\t"+str(round(tree_Q3.limit,3))+"\t"+str(round(tree_Q1.limit,3))+"\t"+str(round(tree_Q2.limit,3))+"\t"+str(round(tree_Q3.limit,3))+"\t"+str(round(tree_Q4.limit,3))+"\t"+str(round(tree_Q5.limit,3))+"\n")
