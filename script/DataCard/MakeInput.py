# Place it in SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter

import os, sys
import commands as cmd
import argparse
from ROOT import *

parser = argparse.ArgumentParser(description='script for creating input root file.',formatter_class=argparse.RawTextHelpFormatter)
parser.add_argument('--CR', action='store_true', help='Make HNL_ControlRegion_Plotter input (default : HNL_SignalRegion_Plotter)')
parser.add_argument('--Syst', action='store_true', help='Add systematics')
args = parser.parse_args()


#eras = ["2016preVFP", "2016postVFP", "2017", "2018"]
#eras = ["2016","2017","2018"]
eras = ["2017"]
#eras = ["2018"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M1000","M10000"]
masses = ["M85","M90","M95","M100","M125","M150","M200","M250","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M85","M90"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]
#channels = ["MuMu"]
HistChannelMap = {'MuMu':'Muon', 'EE':'Electron', 'EMu':'ElectronMuon'}
RegionToHistPrefixMap = {}
RegionToHistInfixMap = {}
RegionToHistSuffixMap = {}
regions = ["sr_inv","cf_cr","ww_cr","zg_cr","wz_cr","wzewk_cr","zz_cr"] # for CRs
regions.append("") # for SR
#regions = ["sr_inv"] # for CRs
## Ugly region maps ##
RegionToCRFlagMap = {}
RegionToCRFlagMap['sr_inv'] = "SS_CR__"
RegionToCRFlagMap['cf_cr']  = "SS_CR__"
RegionToCRFlagMap['ww_cr']  = "VBF_CR__"
RegionToCRFlagMap['zg_cr']  = "LLL_VR__"
RegionToCRFlagMap['wz_cr']  = "LLL_VR__"
RegionToCRFlagMap['wzewk_cr'] = "LLL_VR__"
RegionToCRFlagMap['zz_cr']  = "LLL_VR__"
RegionToCRFlagMap[''] = ""
RegionToChannelMap = {}
RegionToChannelMap['sr_inv'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
RegionToChannelMap['cf_cr']  = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
RegionToChannelMap['ww_cr']  = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
RegionToChannelMap['zg_cr']  = {'MuMu':'MuMuMu', 'EE':'EEE', 'EMu':'EMuL'}
RegionToChannelMap['wz_cr']  = {'MuMu':'MuMuMu', 'EE':'EEE', 'EMu':'EMuL'}
RegionToChannelMap['wzewk_cr']  = {'MuMu':'MuMuMu', 'EE':'EEE', 'EMu':'EMuL'}
RegionToChannelMap['zz_cr']  = {'MuMu':'MuMuMuMu', 'EE':'EEEE', 'EMu':'EMuLL'}
RegionToChannelMap[''] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
RegionToHistSuffixMap = {}
RegionToHistSuffixMap['sr_inv'] = {'MuMu':'LimitBins/Muon', 'EE':'LimitBins/Electron', 'EMu':'LimitBins/ElectronMuon'} # Will add 'SR' or 'CR' below
RegionToHistSuffixMap['cf_cr']  = {'MuMu':'LimitShape_CF/Binned', 'EE':'LimitShape_CF/Binned', 'EMu':'LimitShape_CF/Binned'} # Binned -> Binned later
RegionToHistSuffixMap['ww_cr']  = {'MuMu':'LimitShape_WW/Binned', 'EE':'LimitShape_WW/Binned', 'EMu':'LimitShape_WW/Binned'} # Binned -> Binned later
RegionToHistSuffixMap['zg_cr']  = {'MuMu':'LimitShape_ZG/Binned', 'EE':'LimitShape_ZG/Binned', 'EMu':'LimitShape_ZG/Binned'} # Binned -> Binned later
RegionToHistSuffixMap['wz_cr']  = {'MuMu':'LimitShape_WZ/Binned', 'EE':'LimitShape_WZ/Binned', 'EMu':'LimitShape_WZ/Binned'} # Binned -> Binned later
RegionToHistSuffixMap['wzewk_cr']  = {'MuMu':'LimitShape_WZEWK/Binned', 'EE':'LimitShape_WZEWK/Binned', 'EMu':'LimitShape_WZEWK/Binned'} # Binned -> Binned later
RegionToHistSuffixMap['zz_cr']  = {'MuMu':'LimitShape_ZZ/Binned', 'EE':'LimitShape_ZZ/Binned', 'EMu':'LimitShape_ZZ/Binned'} # Binned -> Binned later
RegionToHistSuffixMap[''] = {'MuMu':'LimitBins/Muon', 'EE':'LimitBins/Electron', 'EMu':'LimitBins/ElectronMuon'} # Will add 'SR' or 'CR' below

#tags = ["HNL_ULID","HNTightV2"] # HNLParameter Name
tags = ["HNL_ULID"] # HNLParameter Name
#outputTag = "240501_1704_" # tag the output directory name as you wish
outputTag = "rateParam_" # tag the output directory name as you wish

# Skim
DataSkim = "_SkimTree_HNMultiLepBDT_"
FakeSkim = "_SkimTree_HNMultiLepBDT_"
#CFSkim = "_SkimTree_HNMultiLepBDT_" #FIXME MC CF
CFSkim = "_SkimTree_DileptonBDT_" #FIXME Data CF
ConvSkim = "_SkimTree_HNMultiLepBDT_"
MCSkim = "_SkimTree_HNMultiLepBDT_"
SignalSkim = "_SkimTree_HNMultiLepBDT_"

# This will do necessary hadd for you.
MergeData   = False
MergeFake   = False  # RunFake
MergeCF     = False  # RunCF
MergeConv   = False  # RunConv
MergeMC     = False  # RunPrompt
MergeSignal = False
#MergeDYVBF = True
#MergeSSWW  = True

if args.CR:
  Blinded = False # Blinded --> the total background will be used as data_obs
  CRflags = ["SS_CR__","VBF_CR__","LLL_VR__"]
  #CRflags = ["LLL_VR__"]
  Analyzer = "HNL_ControlRegion_Plotter"
  for channel in RegionToHistSuffixMap['sr_inv'].keys():
    RegionToHistSuffixMap['sr_inv'][channel] += "CR"
else:
  Blinded = True # Blinded --> the total background will be used as data_obs
  CRflags = [""]
  Analyzer = "HNL_SignalRegion_Plotter"
  for channel in RegionToHistSuffixMap[''].keys():
    RegionToHistSuffixMap[''][channel] += "SR"

## ChargeSplit has been deprecated due to insignificant improvement. Just legacy ##
ChargeSplit = False
if ChargeSplit:
  ChargeSplit = "ChargeSplit"
else:
  ChargeSplit = ""

InputPath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/"+Analyzer+"/"

##### Start merging #####
MergeList = {}
MergeList['Conv_others'] = ["TG","TTG","WZG","WWG","WGToLNuG","WGJJToLNu"] #FIXME time to time
MergeList['ZG_norm']     = ["DYJets_MG","ZGToLLG"] #FIXME add this after applying ConversionSpltting
MergeList['ZZ_norm']     = ["ZZTo4L_powheg","GluGluToZZto4e","GluGluToZZto4mu","GluGluToZZto2e2mu"] #FIXME time to time
MergeList['WW_norm']     = ["WpWp_QCD","WpWp_EWK"] #FIXME time to time
MergeList['Prompt']      = [
                            #VVV
                            'WWW','WWZ','WZZ','ZZZ',
                            #SingleTop
                            'SingleTop_sch_Lep','SingleTop_tch_antitop_Incl','SingleTop_tch_top_Incl','SingleTop_tW_antitop_NoFullyHad','SingleTop_tW_top_NoFullyHad',
                            #ttV
                            'ttWToLNu','ttZToLLNuNu','ttZToQQ_ll', #'ttWToQQ' : no entry
                            #TTXX
                            'TTTT','TTZZ',
                            #tZq
                            'tZq',
                            #Higgs
                            'ttHToNonbb','tHq','VHToNonbb',
                            #VBFHiggs
                            'VBFHToTauTau_M125','VBFHToWWTo2L2Nu','VBF_HToZZTo4L',
                            #ggH
                            'GluGluHToWWTo2L2Nu','GluGluHToZZTo4L', #'GluGluHToTauTau_M125' : no entry
                            #minor WWs
                            'WWTo2L2Nu_DS','WWTo2L2Nu_powheg',
                           ] #FIXME time to time

if MergeData:

  if Blinded:
    print "[MergeData] Data blinded. skipping..."
  else:
    print "[MergeData] Data unblinded. merging..."
    for era in eras:
      for CRflag in CRflags:
        OutFile=InputPath + era + "/" + CRflag + "/DATA/"+Analyzer+DataSkim+"DATA.root"
        if os.path.exists(OutFile):
          os.system("rm " + OutFile)
        os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/" + CRflag + "/DATA/*")

if MergeFake:

  for era in eras:
    for CRflag in CRflags:
      OutFile=InputPath + era + "/" + CRflag + "RunFake__/DATA/"+Analyzer+FakeSkim+"Fake.root"
      if os.path.exists(OutFile):
        os.system("rm " + OutFile)
      os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/" + CRflag + "RunFake__/DATA/*")

if MergeCF:

  for era in eras:
    for CRflag in CRflags:
      OutFile=InputPath + era + "/" + CRflag + "RunCF__/DATA/"+Analyzer+CFSkim+"CF.root" #FIXME Data CF
      #OutFile=InputPath + era + "/" + CRflag + "RunCF__/"+Analyzer+CFSkim+"CF.root" #FIXME MC CF
      if os.path.exists(OutFile):
        os.system("rm " + OutFile)
      os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/" + CRflag + "RunCF__/DATA/*") #FIXME Data CF
      #os.system("hadd " + OutFile + " " + InputPath + "/"+era+"/" + CRflag + "RunCF__/*") #FIXME MC CF

if MergeConv:

  for era in eras:
    for CRflag in CRflags:
      for OutProc in MergeList.keys():
        OutFile=InputPath + era + "/" + CRflag + "RunConv__/"+Analyzer+ConvSkim+OutProc+".root"
        if os.path.exists(OutFile):
          os.system("rm " + OutFile)
        #print "hadd " + OutFile + " " + ' '.join([OutFile.split(OutProc+".root")[0]+ThisProc+".root" for ThisProc in MergeList[OutProc]])
        if "does not exist" in cmd.getoutput("hadd " + OutFile + " " + ' '.join([OutFile.split(OutProc+".root")[0]+ThisProc+".root" for ThisProc in MergeList[OutProc]])):
          os.system("rm " + OutFile)

if MergeMC:

  for era in eras:
    for CRflag in CRflags:
      for OutProc in MergeList.keys():
        OutFile=InputPath + era + "/" + CRflag + "RunPrompt__/"+Analyzer+MCSkim+OutProc+".root"
        if os.path.exists(OutFile):
          os.system("rm " + OutFile)
        if "does not exist" in cmd.getoutput("hadd " + OutFile + " " + ' '.join([OutFile.split(OutProc+".root")[0]+ThisProc+".root" for ThisProc in MergeList[OutProc]])):
          os.system("rm " + OutFile)

if MergeSignal:

  if args.CR:
    print "##### This is CR setting."
    print "##### Skipping signal merging ..."
    pass
  else:
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

##### Main job starts #####
for tag in tags:
  for era in eras:
    for region in regions: # ...and even each region to control!!
      if (not args.CR) and (not region == ''): continue
      elif (args.CR) and (region == ''): continue
      OutputPath = InputPath+'/LimitExtraction/'+outputTag+tag+'/'
      os.system('mkdir -p '+OutputPath + era + '/' + region)
  
      f_path_data        = InputPath + era + "/" + RegionToCRFlagMap[region] + "/DATA/"+Analyzer+DataSkim+"DATA.root"
      f_path_fake        = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunFake__/DATA/"+Analyzer+FakeSkim+"Fake.root"
      f_path_cf          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunCF__/DATA/"+Analyzer+CFSkim+"CF.root"
      #f_path_cf          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunCF__/"+Analyzer+CFSkim+"CF.root" #FIXME MC CF
      f_path_zg          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunConv__/"+Analyzer+ConvSkim+"ZG_norm.root" #FIXME to ZG_norm later
      f_path_conv        = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunConv__/"+Analyzer+ConvSkim+"Conv_others.root"
      f_path_wz          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunPrompt__/"+Analyzer+MCSkim+"WZTo3LNu_amcatnlo.root"
      f_path_zz          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunPrompt__/"+Analyzer+MCSkim+"ZZ_norm.root"
      f_path_ww          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunPrompt__/"+Analyzer+MCSkim+"WW_norm.root"
      f_path_wzewk       = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunPrompt__/"+Analyzer+MCSkim+"WZ_EWK.root"
      f_path_prompt      = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunPrompt__/"+Analyzer+MCSkim+"Prompt.root"
      
      if not Blinded: f_data        = TFile.Open(f_path_data)
      f_fake        = TFile.Open(f_path_fake)
      f_cf          = TFile.Open(f_path_cf)
      f_zg          = TFile.Open(f_path_zg)
      f_conv        = TFile.Open(f_path_conv)
      f_wz          = TFile.Open(f_path_wz)
      f_zz          = TFile.Open(f_path_zz)
      f_ww          = TFile.Open(f_path_ww)
      f_wzewk       = TFile.Open(f_path_wzewk)
      f_prompt      = TFile.Open(f_path_prompt)

      for mass in masses: # iterate for each mass ...
        for channel in channels: # ...and each channel

          if (region == "sr_inv" or region == "") and int(mass.replace("M","")) <= 500:
            LimitDir = "LimitExtractionBDT"
            InputHistMass = mass+"/"
            if args.CR:
              if not 'BDT' in RegionToHistSuffixMap[region][channel]:
                RegionToHistSuffixMap[region][channel] += 'BDT' # FIXME make sure SR and CR share the same naming convention
              else: pass
          else:
            LimitDir = "LimitExtraction"
            InputHistMass = ""
            RegionToHistSuffixMap[region][channel] = RegionToHistSuffixMap[region][channel].replace('BDT','') # FIXME make sure SR and CR share the same naming convention
   

          # Set channel dependent scaler first
          DYVBFscaler = 0.01 # Set the signalDYVBF scaler
          if "EMu" in channel:
            SSWWscaler = 4.*DYVBFscaler*DYVBFscaler # Set the EMu signalSSWW scaler
          else:
            SSWWscaler = DYVBFscaler*DYVBFscaler # Set the signalSSWW scaler

          print "input_hist :", LimitDir+"/"+tag+"/"+RegionToChannelMap[region][channel]+"/"+InputHistMass+RegionToHistSuffixMap[region][channel]
          input_hist = LimitDir+"/"+tag+"/"+RegionToChannelMap[region][channel]+"/"+InputHistMass+RegionToHistSuffixMap[region][channel]
          
          print "##### Initiating",mass,channel,"..."
          if not Blinded: h_data        = f_data.Get(input_hist)
          h_fake        = f_fake.Get(input_hist)
          h_cf          = f_cf.Get(input_hist)
          h_zg          = f_zg.Get(input_hist)
          h_conv        = f_conv.Get(input_hist)
          h_wz          = f_wz.Get(input_hist)
          h_zz          = f_zz.Get(input_hist)
          h_ww          = f_ww.Get(input_hist)
          h_wzewk       = f_wzewk.Get(input_hist)
          h_prompt      = f_prompt.Get(input_hist)
          print "##### histo done."
 
          # Make list of [file path, histogram, histo name]
          input_list = [
                        [f_path_fake, h_fake, "fake"],
                        [f_path_cf, h_cf, "cf"],
                        [f_path_zg, h_zg, "zg"],
                        [f_path_conv, h_conv, "conv"],
                        [f_path_wz, h_wz, "wz"],
                        [f_path_zz, h_zz, "zz"],
                        [f_path_ww, h_ww, "ww"],
                        [f_path_wzewk, h_wzewk, "wzewk"],
                        [f_path_prompt, h_prompt, "prompt"],
                       ]
          
          if Blinded:
            print "##### This analysis is blinded."
            print "##### Creating pseudo data..."
            print "Adding prompt..."
            h_data = h_prompt.Clone()
          
            bkg_list = [ #bkg except prompt (already added above)
                        [f_path_fake, h_fake, "fake"],
                        [f_path_cf, h_cf, "cf"],
                        [f_path_zg, h_zg, "zg"],
                        [f_path_conv, h_conv, "conv"],
                        [f_path_wz, h_wz, "wz"],
                        [f_path_zz, h_zz, "zz"],
                        [f_path_ww, h_ww, "ww"],
                        [f_path_wzewk, h_wzewk, "wzewk"],
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
          
            input_list.append(["fake_data_path", h_data, "data_obs"]) # fake data = total bkg. There is no hist path of it.
          else:
            input_list.append([f_path_data, h_data, "data_obs"])
          print "##### Data done."
  
          # Now list has bkg, (pseudo) data. Finally let's add signals
          if args.CR:
            print "##### This is CR setting."
            print "##### Skipping signal ..."
          else:
            f_path_signalDYVBF = InputPath + era + "/"+Analyzer+"_signalDYVBF_"+mass+".root"
            f_path_signalSSWW  = InputPath + era + "/"+Analyzer+"_signalSSWW_"+mass+".root"
  
            f_signalDYVBF = TFile.Open(f_path_signalDYVBF)
            f_signalSSWW  = TFile.Open(f_path_signalSSWW)
  
            #if int(mass.replace("M","")) <= 100: DYVBFscaler = 0.001 # if you want to use HybridNew without additional options, see https://cms-talk.web.cern.ch/t/too-large-error-with-hybridnew/32844
            try:
              h_signalDYVBF = f_signalDYVBF.Get(input_hist)
            except ReferenceError:
              print("[!!WARNING!!] There is no signal file "+f_path_signalDYVBF+".")
              print "Skipping..."
            else:
              h_signalDYVBF.Scale(DYVBFscaler) # Scaling the signal due to Combine fitting
              input_list.append([f_path_signalDYVBF, h_signalDYVBF, "signalDYVBF"])
              print "Scaled signalDYVBF :", h_signalDYVBF.Integral()
  
            try:
              h_signalSSWW  = f_signalSSWW.Get(input_hist)
            except ReferenceError:
              print("[!!WARNING!!] There is no signal file "+f_path_signalSSWW+".")
              print "Skipping..."
            else:
              h_signalSSWW.Scale(SSWWscaler) # To be consistent when calculating mixing limits
              print "Scaled signalSSWW :", h_signalSSWW.Integral()
              input_list.append([f_path_signalSSWW, h_signalSSWW, "signalSSWW"])
  
            print "##### Signal done."
  
  
          if args.Syst:
            if args.CR:
              print "##### This is CR setting."
              print "##### Skipping systematics ..."
            else:
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
                           "PrefireUp","PrefireDown",
                           "PUUp","PUDown"
                           #"CFUp","CFDown",
                           #"FRUp","FRDown",
                          ]
  
              Nproc = len(input_list) # The number of processes = the length of the input list before adding systematics
              for this_syst in syst_list: # Define new input_hist with each syst name
                input_hist = LimitDir+"/Syst_"+this_syst+tag+"/"+channel+"/"+mass+"/LimitBins/"+HistChannelMap[channel]+Region
  
                for i in range(Nproc):
                  if not "fake_data_path" in input_list[i][0]: # There is no file like "fake_data_path" ...
                    f_syst = TFile.Open(input_list[i][0]) # Get each process's file
                    h_syst = f_syst.Get(input_hist)
                    try: h_syst.SetDirectory(0) # Store h_syst in memory so that it cannot be deleted during the iteration
                    except AttributeError: continue
                    name_syst = input_list[i][2]+"_"+this_syst # Define syst histo name
                    if "DYVBF" in input_list[i][2]: # Scale the syst variated signals
                      h_syst.Scale(DYVBFscaler)
                    elif "SSWW" in input_list[i][2]:
                      h_syst.Scale(SSWWscaler)
                    print "Appending "+name_syst+"..."
                    input_list.append([input_list[i][0], h_syst, name_syst]) # Append each syst histogram while iterating
  
              print "##### Systematics done."
  
  
          print "##### Now creating a limit input root file..."
          outfile = TFile.Open(OutputPath+era+"/"+region+"/"+mass+"_"+channel+"_card_input.root","RECREATE")
          
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
          print OutputPath+era+"/"+region+"/"+mass+"_"+channel+"_card_input.root has been created."
