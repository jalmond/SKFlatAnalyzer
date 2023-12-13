# Place it in SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter

import os, sys
import commands as cmd
from ROOT import *

eras = ["2016preVFP", "2016postVFP", "2017", "2018"]
eras = ["2016","2017","2018"]
eras = ["2017"]
#eras = ["2018"]
masses = ["M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000"]
masses = ["M5000","M7500","M10000","M15000","M20000"]
masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]
masses = ["M500"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]
tags = ["HNL_ULID","HNTightV2"] # HNLParameter Name
outputTag = "231227_KCMS_WS" # tag the output as you wish

InputPath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter/"
Analyzer = "HNL_SignalRegionPlotter"

# Skim
DataSkim = ""
FakeSkim = "_SkimTree_HNMultiLepBDT_"
#CFSkim = "_SkimTree_HNMultiLepBDT_" #FIXME MC CF
CFSkim = "_SkimTree_DileptonBDT_" #FIXME Data CF
ConvSkim = "_SkimTree_HNMultiLepBDT_"
MCSkim = "_SkimTree_HNMultiLepBDT_"
SignalSkim = "_SkimTree_HNMultiLepBDT_"

# This will do necessary hadd for you.
MergeData   = False
MergeFake   = True  # RunFake
MergeCF     = False  # RunCF
MergeConv   = True  # RunConv
MergeMC     = True  # RunPrompt
MergeSignal = True
#MergeDYVBF = True
#MergeSSWW  = True

Blinded = True # Blinded --> the total background will be used as data_obs
AddSyst = False
ChargeSplit = False
if ChargeSplit:
  ChargeSplit = "ChargeSplit"
else:
  ChargeSplit = ""

if MergeData:

  for era in eras:
    OutFile=InputPath + era + "/DATA/"+Analyzer+DataSkim+"DATA.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/DATA/*")

if MergeFake:

  for era in eras:
    OutFile=InputPath + era + "/RunFake__/DATA/"+Analyzer+FakeSkim+"Fake.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/RunFake__/DATA/*")

if MergeCF:

  for era in eras:
    OutFile=InputPath + era + "/RunCF__/DATA/"+Analyzer+CFSkim+"CF.root" #FIXME Data CF
    #OutFile=InputPath + era + "/RunCF__/"+Analyzer+CFSkim+"CF.root" #FIXME MC CF
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/RunCF__/DATA/*") #FIXME Data CF
    #os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/RunCF__/*") #FIXME MC CF

if MergeConv:

  for era in eras:
    OutFile=InputPath + era + "/RunConv__/"+Analyzer+ConvSkim+"Conv.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/RunConv__/*")

if MergeMC:

  for era in eras:
    #OutFile=InputPath + era + "/"+Analyzer+MCSkim+"MC.root"
    OutFile=InputPath + era + "/RunPrompt__/"+Analyzer+MCSkim+"MC.root"
    if os.path.exists(OutFile):
      os.system("rm " + OutFile)
    MClist = cmd.getoutput('ls '+InputPath+era+'/RunPrompt__/ | grep -v private | grep -v Run').replace("\n"," ").replace("HNL_SignalRegionPlotter",InputPath+era+"/RunPrompt__/HNL_SignalRegionPlotter") # remove signals, directories
    os.system("hadd " + OutFile + " " + MClist)

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

for tag in tags:
  for era in eras:
    OutputPath = InputPath+'/LimitInputs/'+outputTag+'_'+tag+'/'
    os.system('mkdir -p '+OutputPath + era)
  
    f_path_data        = InputPath + era + "/DATA/"+Analyzer+DataSkim+"DATA.root"
    f_path_fake        = InputPath + era + "/RunFake__/DATA/"+Analyzer+FakeSkim+"Fake.root"
    f_path_cf          = InputPath + era + "/RunCF__/DATA/"+Analyzer+CFSkim+"CF.root"
    #f_path_cf          = InputPath + era + "/RunCF__/"+Analyzer+CFSkim+"CF.root" #FIXME MC CF
    f_path_conv        = InputPath + era + "/RunConv__/"+Analyzer+ConvSkim+"Conv.root"
    f_path_prompt      = InputPath + era + "/RunPrompt__/"+Analyzer+MCSkim+"MC.root"
    
    if not Blinded: f_data        = TFile.Open(f_path_data)
    f_fake        = TFile.Open(f_path_fake)
    f_cf          = TFile.Open(f_path_cf)
    f_conv        = TFile.Open(f_path_conv)
    f_prompt      = TFile.Open(f_path_prompt)
    
    for mass in masses: #iterate for signals
      for channel in channels:
        if int(mass.replace("M","")) <= 500:
          if channel == "MuMu": input_hist =  "LimitInputBDT/"+channel+"_Channel/"+tag+"/"+mass+"/LimitBins/MuonSR" #path to the input histogram
          elif channel == "EE": input_hist =  "LimitInputBDT/"+channel+"_Channel/"+tag+"/"+mass+"/LimitBins/ElectronSR" #path to the input histogram
          elif channel == "EMu": input_hist = "LimitInputBDT/"+channel+"_Channel/"+tag+"/"+mass+"/LimitBins/ElectronMuonSR" #path to the input histogram
        elif int(mass.replace("M","")) > 500:
          if channel == "MuMu": input_hist =  "LimitInput/"+channel+"_Channel/"+tag+"/LimitBins/Muon"+ChargeSplit+"SR" #path to the input histogram
          elif channel == "EE": input_hist =  "LimitInput/"+channel+"_Channel/"+tag+"/LimitBins/Electron"+ChargeSplit+"SR" #path to the input histogram
          elif channel == "EMu": input_hist = "LimitInput/"+channel+"_Channel/"+tag+"/LimitBins/ElectronMuon"+ChargeSplit+"SR" #path to the input histogram
        
        print "##### Initiating",mass,channel,"..."
        if not Blinded: h_data        = f_data.Get(input_hist)
        h_fake        = f_fake.Get(input_hist)
        h_cf          = f_cf.Get(input_hist)
        h_conv        = f_conv.Get(input_hist)
        h_prompt      = f_prompt.Get(input_hist)
        print "##### histo done."
        
        # Make list of [file path, histogram, histo name]
        input_list = [
                      [f_path_fake, h_fake, "fake"],
                      [f_path_cf, h_cf, "cf"],
                      [f_path_conv, h_conv, "conv"],
                      [f_path_prompt, h_prompt, "prompt"],
                     ]
        
        if Blinded:
          print "##### This analysis is blinded."
          print "##### Creating pseudo data..."
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
        print "##### Data done."
  
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
          print("[!!WARNING!!] There is no signal file "+f_path_signalSSWW+".")
          print "Skipping..."
        else:
          h_signalSSWW.Scale(scaler*scaler) # To be consistent when calculating mixing limits
          print "Scaled signalSSWW :", h_signalSSWW.Integral()
          input_list.append([f_path_signalSSWW, h_signalSSWW, "signalSSWW"])
  
        print "##### Signal done."
  
  
        if AddSyst:
          print "##### Systematics activated."
          syst_list = [
                       "JetResUp","JetResDown",
                       "JetEnUp","JetEnDown",
                       "JetMassUp","JetMassDown",
                       "JetMassSmearUp","JetMassSmearDown",
                       "MuonRecoSFUp","MuonRecoSFDown",
                       "MuonEnUp","MuonEnDown",
                       "MuonIDSFUp","MuonIDSFDown",
                       "MuonISOSFUp","MuonISOSFDown",
                       "ElectronRecoSFUp","ElectronRecoSFDown",
                       "ElectronResUp","ElectronResDown",
                       "ElectronEnUp","ElectronEnDown",
                       "ElectronIDSFUp","ElectronIDSFDown",
                       "ElectronTriggerSFUp","ElectronTriggerSFDown",
                       "BTagSFHTagUp","BTagSFHTagDown",
                       "BTagSFLTagUp","BTagSFLTagDown",
                       "METUnclUp","METUnclDown",
                       "CFUp","CFDown",
                       "FRUp","FRDown",
                       "PrefireUp","PrefireDown",
                       "PUUp","PUDown"
                      ]
  
          Nproc = len(input_list) # The number of processes = the length of the input list before adding systematics
          for this_syst in syst_list: # Define new input_hist with each syst name
            if int(mass.replace("M","")) <= 500:
              if channel == "MuMu": input_hist =  "LimitInputBDT/"+"Syst_"+this_syst+"MVAUL_UL/"+mass+"/FillEventCutflow/MuonSR"
              elif channel == "EE": input_hist =  "LimitInputBDT/"+"Syst_"+this_syst+"MVAUL_UL/"+mass+"/FillEventCutflow/ElectronSR"
              elif channel == "EMu": input_hist = "LimitInputBDT/"+"Syst_"+this_syst+"MVAUL_UL/"+mass+"/FillEventCutflow/ElectronMuonSR"
            elif int(mass.replace("M","")) > 500:
              if channel == "MuMu": input_hist =  "LimitInput/"+"Syst_"+this_syst+"MVAUL_UL/FillEventCutflow/Muon"+ChargeSplit+"SR"
              elif channel == "EE": input_hist =  "LimitInput/"+"Syst_"+this_syst+"MVAUL_UL/FillEventCutflow/Electron"+ChargeSplit+"SR"
              elif channel == "EMu": input_hist = "LimitInput/"+"Syst_"+this_syst+"MVAUL_UL/FillEventCutflow/ElectronMuon"+ChargeSplit+"SR"
  
            for i in range(Nproc):
              if not "fake_data_path" in input_list[i][0]: # There is no file like "fake_data_path" ...
                f_syst = TFile.Open(input_list[i][0]) # Get each process's file
                h_syst = f_syst.Get(input_hist)
                try: h_syst.SetDirectory(0) # Store h_syst in memory so that it cannot be deleted during the iteration
                except AttributeError: continue
                name_syst = input_list[i][2]+"_"+this_syst # Define syst histo name
                print "Appending "+name_syst+"..."
                input_list.append([input_list[i][0], h_syst, name_syst]) # Append each syst histogram while iterating
  
          print "##### Systematics done."
  
  
        print "##### Now creating a limit input root file..."
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
        print OutputPath+era+"/"+mass+"_"+channel+"_card_input.root has been created."
