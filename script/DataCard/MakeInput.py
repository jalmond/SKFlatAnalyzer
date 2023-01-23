# Place it in /data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter

import os, sys
import commands as cmd
from ROOT import *

eras = ["2016preVFP", "2016postVFP", "2017", "2018"]
eras = ["2016","2017","2018"]
eras = ["2016"]
masses = ["M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000"]
masses = ["M5000","M7500","M10000","M15000","M20000"]
masses = ["M100","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]
channels = ["MuMu","EE","EMu"]

InputPath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/Before_Merge_230119/HNL_SignalRegionPlotter/"
OutputPath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/Before_Merge_230119/HNL_SignalRegionPlotter/InputForCombine/"
Analyzer = "HNL_SignalRegionPlotter"

MergeData   = False
MergeFake   = False
MergeCF     = False
MergeConv   = False
MergeMC     = False
MergeSignal = False
#MergeDYVBF = True
#MergeSSWW  = True

Blinded = True

if MergeData:

  for era in eras:
    OutFile=OutputPath + era + "/DATA/"+Analyzer+"_SkimTree_HNMultiLep_DATA.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + "  " + InputPath + "/"+era+"/DATA/*")

if MergeFake:

  for era in eras:
    OutFile=OutputPath + era + "/RunFake__/DATA/"+Analyzer+"_SkimTree_HNMultiLep_Fake.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + "  " + InputPath + "/"+era+"/RunFake__/DATA/*")

if MergeCF:

  for era in eras:
    OutFile=OutputPath + era + "/RunCF__/DATA/"+Analyzer+"_SkimTree_Dilepton_CF.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + "  " + InputPath + "/"+era+"//RunCF__/DATA/*")

if MergeConv:

  for era in eras:
    OutFile=OutputPath + era + "/RunConv__/"+Analyzer+"_SkimTree_HNMultiLep_Conv.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + "  " + InputPath + "/"+era+"/RunConv__/*")

if MergeMC:

  for era in eras:
    OutFile=OutputPath + era + "/"+Analyzer+"_SkimTree_HNMultiLep_MC.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + "  " + InputPath + "/"+era+"/*HNMu*")

if MergeSignal:

  for era in eras:
    for mass in masses:
      OutFileDYVBF=InputPath + era + "/"+Analyzer+"_signalDYVBF_"+mass+".root"
      OutFileSSWW =InputPath + era + "/"+Analyzer+"_signalSSWW_"+mass+".root"
      if int(mass.replace("M","")) < 500: # DY only
        os.system("hadd -f " + OutFileDYVBF + "  " + InputPath+"/"+era+"/*DYTypeI*"+mass+"*.root")
      elif 500 <= int(mass.replace("M","")) and int(mass.replace("M","")) <= 3000: # DY+VBF+SSWW
        os.system("hadd -f " + OutFileDYVBF + "  " + InputPath+"/"+era+"/*DYTypeI*"+mass+"*.root" + " " + InputPath+"/"+era+"/*VBFTypeI*"+mass+"*.root")
        os.system("hadd -f " + OutFileSSWW  + "  " + InputPath+"/"+era+"/*SSWWTypeI*"+mass+"*.root")
      elif 3000 < int(mass.replace("M","")): # SSWW only
        os.system("hadd -f " + OutFileSSWW  + "  " + InputPath+"/"+era+"/*SSWWTypeI*"+mass+"*.root")


for era in eras:
  os.system('mkdir -p '+OutputPath + era)

  f_path_data        = InputPath + era + "/DATA/"+Analyzer+"_SkimTree_HNMultiLep_DATA.root"
  f_path_fake        = InputPath + era + "/RunFake__/DATA/"+Analyzer+"_SkimTree_HNMultiLep_Fake.root"
  f_path_cf          = InputPath + era + "/RunCF__/DATA/"+Analyzer+"_SkimTree_Dilepton_CF.root"
  f_path_conv        = InputPath + era + "/RunConv__/"+Analyzer+"_SkimTree_HNMultiLep_Conv.root"
  f_path_prompt      = InputPath + era + "/"+Analyzer+"_SkimTree_HNMultiLep_MC.root"
  
  if not Blinded: f_data        = TFile.Open(f_path_data)
  f_fake        = TFile.Open(f_path_fake)
  f_cf          = TFile.Open(f_path_cf)
  f_conv        = TFile.Open(f_path_conv)
  f_prompt      = TFile.Open(f_path_prompt)
  
  for mass in masses: #iterate for signals
    for channel in channels:
      if int(mass.replace("M","")) <= 500:
        if channel == "MuMu": input_hist =  "LimitInputBDT/HNL_UL/"+str(int(mass.replace("M","")))+"/MuonSR" #path to the input histogram
        elif channel == "EE": input_hist =  "LimitInputBDT/HNL_UL/"+str(int(mass.replace("M","")))+"/ElectronSR" #path to the input histogram
        elif channel == "EMu": input_hist = "LimitInputBDT/HNL_UL/"+str(int(mass.replace("M","")))+"/ElectronMuonSR" #path to the input histogram
      elif int(mass.replace("M","")) > 500:
        if channel == "MuMu": input_hist =  "LimitInput/HNL_UL/MuonSR" #path to the input histogram
        elif channel == "EE": input_hist =  "LimitInput/HNL_UL/ElectronSR" #path to the input histogram
        elif channel == "EMu": input_hist = "LimitInput/HNL_UL/ElectronMuonSR" #path to the input histogram
      
      if not Blinded: h_data        = f_data.Get(input_hist)
      h_fake        = f_fake.Get(input_hist)
      h_cf          = f_cf.Get(input_hist)
      h_conv        = f_conv.Get(input_hist)
      h_prompt      = f_prompt.Get(input_hist)
      
      # Make list of [file path, histogram, histo name]
      input_list = [
                    [f_path_fake, h_fake, "fake"],
                    [f_path_cf, h_cf, "cf"],
                    [f_path_conv, h_conv, "conv"],
                    [f_path_prompt, h_prompt, "prompt"],
                   ]
      
      if Blinded:
        print "##### This analysis is blinded."
        print "##### Creating fake data..."
        print "Adding prompt..."
        h_data = h_prompt.Clone()
      
        bkg_list = [
                      [f_path_fake, h_fake, "fake"],
                      [f_path_cf, h_cf, "cf"],
                      [f_path_conv, h_conv, "conv"],
                     ]
      
        total_number = 0 # to cross check
        total_number += h_prompt.GetEntries()
      
        for bkg in bkg_list:
          try:
            bkg[1].GetEntries()
          except AttributeError:
            print("[!!WARNING!!] There is no hist named "+input_hist+" in "+bkg[0])
            print "Skipping "+bkg[2]+"..."
            continue
          print "Adding "+bkg[2]+"..."
          h_data.Add(bkg[1]) # Add each bkg while iterating
          total_number += bkg[1].GetEntries()
      
        #print h_data.GetEntries(), total_number
        if h_data.GetEntries() == total_number: pass
        else:
          print "[!!ERROR!!] Cross check failed. Exiting..."
          sys.exit()
      
        input_list.append(["fake_data_path", h_data, "data_obs"])
      else:
        input_list.append([f_path_data, h_data, "data_obs"])

      # Now list has bkg, (pseudo) data. Finally let's add signals

      f_path_signalDYVBF = InputPath + era + "/"+Analyzer+"_signalDYVBF_"+mass+".root"
      f_path_signalSSWW  = InputPath + era + "/"+Analyzer+"_signalSSWW_"+mass+".root"

      f_signalDYVBF = TFile.Open(f_path_signalDYVBF)
      f_signalSSWW  = TFile.Open(f_path_signalSSWW)

      scaler = 0.01 # Set the signal scaler
      #if int(mass.replace("M","")) < 1000: scaler = 0.01
      #else: scaler = 0.1
      try:
        h_signalDYVBF = f_signalDYVBF.Get(input_hist)
      except ReferenceError:
        print("[!!WARNING!!] There is no signal file "+f_path_signalDYVBF+".")
        print "Skipping..."
      else:
        h_signalDYVBF.Scale(scaler) # Scaling the signal due to Combine fitting
        input_list.append([f_path_signalDYVBF, h_signalDYVBF, "signalDYVBF"])
        print "Scaled signalDYVBF :", h_signalDYVBF.Integral()

      try:
        h_signalSSWW  = f_signalSSWW.Get(input_hist)
      except ReferenceError:
        print("[!!WARNING!!] There is no signal file "+f_path_signalDYVBF+".")
        print "Skipping..."
      else:
        h_signalSSWW.Scale(scaler*scaler) # To be consistent in calculating mixing limits
        print "Scaled signalSSWW :", h_signalSSWW.Integral()
        input_list.append([f_path_signalSSWW, h_signalSSWW, "signalSSWW"])


      print "##### Now creating an output root file..."
      outfile = TFile.Open(OutputPath+era+"/"+mass+"_"+channel+"_card_input.root","RECREATE")
      
      outfile.cd() # Move into it
      for item in input_list:
        try:
          item[1].SetName(item[2])
        except AttributeError:
          print("[!!WARNING!!] There is no hist named "+input_hist+" in "+item[0])
          print "Skipping "+item[2]+"..."
          continue
        print "Writing "+item[2]+"..."
        item[1].Write() # Write each histogram while iterating
      
      outfile.Close()
