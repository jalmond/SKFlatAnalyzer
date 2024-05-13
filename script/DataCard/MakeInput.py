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
#eras = ["2017"]
eras = ["2018"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M1000","M10000"]
masses = ["M85","M90","M95","M100","M125","M150","M200","M250","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M1000"]
#masses = ["M85"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]
#channels = ["MuMu"]
HistChannelMap = {'MuMu':'Muon', 'EE':'Electron', 'EMu':'ElectronMuon'}
## Ugly region maps ##
RegionToCRFlagMap = {}
RegionToChannelMap = {}
RegionToHistSuffixMap = {}

#tags = ["HNL_ULID","HNTightV2"] # HNLParameter Name
tags = ["HNL_ULID"] # HNLParameter Name
#outputTag = "240501_1704_" # tag the output directory name as you wish
#outputTag = "rateParam_" # tag the output directory name as you wish
outputTag = "PR48_rateParam_" # tag the output directory name as you wish

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
  Analyzer = "HNL_ControlRegion_Plotter"

  regions = ["sr_inv","sr1_inv","sr2_inv","sr3_inv","cf_cr","ww_cr","zg_cr","wz_cr","wzewk_cr","zz_cr"] # for CRs
  #regions = ""

  RegionToCRFlagMap['sr_inv'] = "SS_CR__"
  RegionToCRFlagMap['sr1_inv'] = "SS_CR__"
  RegionToCRFlagMap['sr2_inv'] = "SS_CR__"
  RegionToCRFlagMap['sr3_inv'] = "SS_CR__"
  RegionToCRFlagMap['cf_cr']  = "SS_CR__"
  RegionToCRFlagMap['ww_cr']  = "VBF_CR__"
  RegionToCRFlagMap['zg_cr']  = "LLL_VR__"
  RegionToCRFlagMap['wz_cr']  = "LLL_VR__"
  RegionToCRFlagMap['wzewk_cr'] = "LLL_VR__"
  RegionToCRFlagMap['zz_cr']  = "LLL_VR__"

  RegionToChannelMap['sr_inv'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['sr1_inv'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['sr2_inv'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['sr3_inv'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['cf_cr']  = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['ww_cr']  = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['zg_cr']  = {'MuMu':'MuMuMu', 'EE':'EEE', 'EMu':'EMuL'}
  RegionToChannelMap['wz_cr']  = {'MuMu':'MuMuMu', 'EE':'EEE', 'EMu':'EMuL'}
  RegionToChannelMap['wzewk_cr']  = {'MuMu':'MuMuMu', 'EE':'EEE', 'EMu':'EMuL'}
  RegionToChannelMap['zz_cr']  = {'MuMu':'MuMuMuMu', 'EE':'EEEE', 'EMu':'EMuLL'}

  RegionToHistSuffixMap['sr_inv'] = {'MuMu':'LimitBins/MuonCR', 'EE':'LimitBins/ElectronCR', 'EMu':'LimitBins/ElectronMuonCR'}
  RegionToHistSuffixMap['sr1_inv'] = {'MuMu':'LimitBins/MuonCR1', 'EE':'LimitBins/ElectronCR1', 'EMu':'LimitBins/ElectronMuonCR1'}
  RegionToHistSuffixMap['sr2_inv'] = {'MuMu':'LimitBins/MuonCR2', 'EE':'LimitBins/ElectronCR2', 'EMu':'LimitBins/ElectronMuonCR2'}
  RegionToHistSuffixMap['sr3_inv'] = {'MuMu':'LimitBins/MuonCR3', 'EE':'LimitBins/ElectronCR3', 'EMu':'LimitBins/ElectronMuonCR3'}
  RegionToHistSuffixMap['cf_cr']  = {'MuMu':'LimitShape_CF/Binned', 'EE':'LimitShape_CF/Binned', 'EMu':'LimitShape_CF/Binned'}
  RegionToHistSuffixMap['ww_cr']  = {'MuMu':'LimitShape_WW/Binned', 'EE':'LimitShape_WW/Binned', 'EMu':'LimitShape_WW/Binned'}
  RegionToHistSuffixMap['zg_cr']  = {'MuMu':'LimitShape_ZG/Binned', 'EE':'LimitShape_ZG/Binned', 'EMu':'LimitShape_ZG/Binned'}
  RegionToHistSuffixMap['wz_cr']  = {'MuMu':'LimitShape_WZ/Binned', 'EE':'LimitShape_WZ/Binned', 'EMu':'LimitShape_WZ/Binned'}
  RegionToHistSuffixMap['wzewk_cr']  = {'MuMu':'LimitShape_WZEWK/Binned', 'EE':'LimitShape_WZEWK/Binned', 'EMu':'LimitShape_WZEWK/Binned'}
  RegionToHistSuffixMap['zz_cr']  = {'MuMu':'LimitShape_ZZ/Binned', 'EE':'LimitShape_ZZ/Binned', 'EMu':'LimitShape_ZZ/Binned'}

else:
  Blinded = True # Blinded --> the total background will be used as data_obs
  CRflags = [""]
  Analyzer = "HNL_SignalRegion_Plotter"

  regions = ["sr","sr1","sr2","sr3"] # for SRs
  #regions = ["sr1"] # for SRs
  #regions = ""

  RegionToCRFlagMap['sr'] = ""
  RegionToCRFlagMap['sr1'] = ""
  RegionToCRFlagMap['sr2'] = ""
  RegionToCRFlagMap['sr3'] = ""

  RegionToChannelMap['sr'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['sr1'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['sr2'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}
  RegionToChannelMap['sr3'] = {'MuMu':'MuMu', 'EE':'EE', 'EMu':'EMu'}

  RegionToHistSuffixMap['sr'] = {'MuMu':'LimitBins/MuonSR', 'EE':'LimitBins/ElectronSR', 'EMu':'LimitBins/ElectronMuonSR'}
  RegionToHistSuffixMap['sr1'] = {'MuMu':'LimitBins/MuonSR1', 'EE':'LimitBins/ElectronSR1', 'EMu':'LimitBins/ElectronMuonSR1'}
  RegionToHistSuffixMap['sr2'] = {'MuMu':'LimitBins/MuonSR2', 'EE':'LimitBins/ElectronSR2', 'EMu':'LimitBins/ElectronMuonSR2'}
  RegionToHistSuffixMap['sr3'] = {'MuMu':'LimitBins/MuonSR3', 'EE':'LimitBins/ElectronSR3', 'EMu':'LimitBins/ElectronMuonSR3'}

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
                            #SingleTop : 0.1 level events
                            #'SingleTop_sch_Lep','SingleTop_tch_antitop_Incl','SingleTop_tch_top_Incl','SingleTop_tW_antitop_NoFullyHad','SingleTop_tW_top_NoFullyHad',
                            #ttV
                            'ttWToLNu','ttZToLLNuNu', #'ttZToQQ_ll', 'ttWToQQ' : no entry
                            #TTXX
                            'TTTT','TTZZ',
                            #tZq
                            'tZq',
                            #Higgs
                            'ttHToNonbb','tHq','VHToNonbb',
                            #VBFHiggs
                            'VBF_HToZZTo4L', #'VBFHToTauTau_M125', 'VBFHToWWTo2L2Nu', : no entry
                            #ggH
                            'GluGluHToZZTo4L', #'GluGluHToTauTau_M125', 'GluGluHToWWTo2L2Nu', : no entry
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
      print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",region
      OutputPath = InputPath+'/LimitExtraction/'+outputTag+tag+'/'
      os.system('mkdir -p '+OutputPath + era + '/' + region)
  
      f_path_data        = InputPath + era + "/" + RegionToCRFlagMap[region] + "/DATA/"+Analyzer+DataSkim+"DATA.root"
      f_path_fake        = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunFake__/DATA/"+Analyzer+FakeSkim+"Fake.root"
      f_path_cf          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunCF__/DATA/"+Analyzer+CFSkim+"CF.root"
      #f_path_cf          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunCF__/"+Analyzer+CFSkim+"CF.root" #FIXME MC CF
      f_path_zg          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunConv__/"+Analyzer+ConvSkim+"ZG_norm.root" #FIXME to ZG_norm later
      f_path_conv        = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunConv__/"+Analyzer+ConvSkim+"Conv_others.root"
      #f_path_wz          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunPrompt__/"+Analyzer+MCSkim+"WZTo3LNu_amcatnlo.root"
      f_path_wz          = InputPath + era + "/" + RegionToCRFlagMap[region] + "RunPrompt__/"+Analyzer+MCSkim+"WZTo3LNu_mllmin4p0_powheg.root" # This gives better agr
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

          if (region == "sr" or "sr3" in region) and int(mass.replace("M","")) <= 500:
            LimitDir = "LimitExtractionBDT"
            InputHistMass = mass+"/"
            if not 'BDT' in RegionToHistSuffixMap[region][channel]:
              RegionToHistSuffixMap[region][channel] += 'BDT'
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
                print("[!!WARNING!!] There is no hist named "+input_hist+" in "+bkg[0]+" .")
                print "Skipping "+bkg[2]+"..."
                continue
              print "Adding "+bkg[2]+"..."
              h_data.Add(bkg[1]) # Add each bkg while iterating
              total_number += bkg[1].GetEntries()
          
            #print h_data.GetEntries(), total_number
            if h_data.GetEntries() == total_number: pass #NOTE Only valid when blinded.
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
  
            print "opening",f_path_signalDYVBF,"..."
            f_signalDYVBF = TFile.Open(f_path_signalDYVBF)
            print "opening",f_path_signalSSWW,"..."
            f_signalSSWW  = TFile.Open(f_path_signalSSWW)
  
            #if int(mass.replace("M","")) <= 100: DYVBFscaler = 0.001 # if you want to use HybridNew without additional options, see https://cms-talk.web.cern.ch/t/too-large-error-with-hybridnew/32844
            try:
              h_signalDYVBF = f_signalDYVBF.Get(input_hist)
            except ReferenceError:
              print("[!!WARNING!!] There is no signal file "+f_path_signalDYVBF+" .")
              print "Skipping..."
              #print "Making an empty hist..."
              #h_signalDYVBF = h_data.Clone() #NOTE This might be not working after unblinding, when there is no real data.
              #for i in range(h_signalDYVBF.GetNbinsX()):
              #  h_signalDYVBF.SetBinContent(i+1,0)
              #  h_signalDYVBF.SetBinError(i+1,0)
              #input_list.append([f_path_signalDYVBF, h_signalDYVBF, "signalDYVBF"])
              #if h_signalDYVBF.Integral() != 0:
              #  print "[!!ERROR!!] this must be empty. But having :",h_signalDYVBF.Integral()
              #  print "Exiting..."
              #  sys.exit()
            else:
              try:
                h_signalDYVBF.Scale(DYVBFscaler) # Scaling the signal due to Combine fitting
              except AttributeError:
                print("[!!WARNING!!] There is no hist named "+input_hist+" in "+f_path_signalDYVBF+" .")
                print "Skipping..."
                #print "Making an empty hist..."
                #h_signalDYVBF = h_data.Clone() #NOTE This might be not working after unblinding, when there is no real data.
                #for i in range(h_signalDYVBF.GetNbinsX()):
                #  h_signalDYVBF.SetBinContent(i+1,0)
                #  h_signalDYVBF.SetBinError(i+1,0)
                #input_list.append([f_path_signalDYVBF, h_signalDYVBF, "signalDYVBF"])
                #if h_signalDYVBF.Integral() != 0:
                #  print "[!!ERROR!!] this must be empty. But having :",h_signalDYVBF.Integral()
                #  print "Exiting..."
                #  sys.exit()
              else:
                input_list.append([f_path_signalDYVBF, h_signalDYVBF, "signalDYVBF"])
                print "Scaled signalDYVBF :", h_signalDYVBF.Integral()
  
            try:
              h_signalSSWW  = f_signalSSWW.Get(input_hist)
            except ReferenceError:
              print("[!!WARNING!!] There is no signal file "+f_path_signalSSWW+" .")
              print "Skipping..."
              #print "Making an empty hist..."
              #h_signalSSWW = h_data.Clone() #NOTE This might be not working after unblinding, when there is no real data.
              #for i in range(h_signalSSWW.GetNbinsX()):
              #  h_signalSSWW.SetBinContent(i+1,0)
              #  h_signalSSWW.SetBinError(i+1,0)
              #input_list.append([f_path_signalSSWW, h_signalSSWW, "signalSSWW"])
              #if h_signalSSWW.Integral() != 0:
              #  print "[!!ERROR!!] this must be empty. But having :",h_signalSSWW.Integral()
              #  print "Exiting..."
              #  sys.exit()
            else:
              try:
                h_signalSSWW.Scale(SSWWscaler) # To be consistent when calculating mixing limits
              except AttributeError:
                print("[!!WARNING!!] There is no hist named "+input_hist+" in "+f_path_signalSSWW+" .")
                print "Skipping..."
                #print "Making an empty hist..."
                #h_signalSSWW = h_data.Clone() #NOTE This might be not working after unblinding, when there is no real data.
                #for i in range(h_signalSSWW.GetNbinsX()):
                #  h_signalSSWW.SetBinContent(i+1,0)
                #  h_signalSSWW.SetBinError(i+1,0)
                #input_list.append([f_path_signalSSWW, h_signalSSWW, "signalSSWW"])
                #if h_signalSSWW.Integral() != 0:
                #  print "[!!ERROR!!] this must be empty. But having :",h_signalSSWW.Integral()
                #  print "Exiting..."
                #  sys.exit()
              else:
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
                           #"JetMassUp","JetMassDown",
                           #"JetMassSmearUp","JetMassSmearDown",
                           #"MuonRecoSFUp","MuonRecoSFDown",
                           #"MuonEnUp","MuonEnDown",
                           #"MuonIDSFUp","MuonIDSFDown",
                           #"MuonISOSFUp","MuonISOSFDown",
                           #"ElectronRecoSFUp","ElectronRecoSFDown",
                           #"ElectronResUp","ElectronResDown",
                           #"ElectronEnUp","ElectronEnDown",
                           #"ElectronIDSFUp","ElectronIDSFDown",
                           #"ElectronTriggerSFUp","ElectronTriggerSFDown",
                           "BTagSFHTagUp","BTagSFHTagDown",
                           #"BTagSFLTagUp","BTagSFLTagDown",
                           #"METUnclUp","METUnclDown",
                           #"PrefireUp","PrefireDown",
                           #"PUUp","PUDown"
                           #"CFUp","CFDown",
                           #"FRUp","FRDown",
                          ]
  
              Nproc = len(input_list) # The number of processes = the length of the input list before adding systematics
              for this_syst in syst_list: # Define new input_hist with each syst name
                input_hist = LimitDir+"/Syst_"+this_syst+tag+"/"+channel+"/"+InputHistMass+RegionToHistSuffixMap[region][channel]
  
                for i in range(Nproc):
                  if not "fake_data_path" in input_list[i][0]: # There is no file like "fake_data_path" ...
                    f_syst = TFile.Open(input_list[i][0]) # Get each process's file
                    try:
                      h_syst = f_syst.Get(input_hist)
                    except ReferenceError:
                      print("[!!WARNING!!] There is no file "+input_list[i][0]+" .")
                      if "signal" in input_list[i][2]:
                        print "Skipping..."
                      else:
                        print "Making an empty hist..."
                        if args.CR:
                          h_syst = h_prompt.Clone() #FIXME
                        else:
                          h_syst = h_data.Clone() # With CR, data may not be defined for some regions with no entry ...
                        for j in range(h_syst.GetNbinsX()):
                          h_syst.SetBinContent(j+1,0)
                          h_syst.SetBinError(j+1,0)
                        h_syst.SetBinContent(1,0.001) # to avoid Combine complaining for empty hist.
                        h_syst.SetBinError(1,0.0001)
                        #if h_syst.Integral() != 0:
                        #  print "[!!ERROR!!] this must be empty. But having :",h_syst.Integral()
                        #  print "Exiting..."
                        #  sys.exit()
                        h_syst.SetDirectory(0) # Store h_syst in memory so that it cannot be deleted during the iteration
                    else:
                      try:
                        h_syst.SetDirectory(0) # Store h_syst in memory so that it cannot be deleted during the iteration
                      except AttributeError:
                        print("[!!WARNING!!] There is no hist named "+input_hist+" in "+input_list[i][0]+" .")
                        if "signal" in input_list[i][2]:
                          print "Skipping..."
                        else:
                          print "Making an empty hist..."
                          if args.CR:
                            h_syst = h_prompt.Clone() #FIXME
                          else:
                            h_syst = h_data.Clone() # With CR, data may not be defined for some regions with no entry ...
                          for j in range(h_syst.GetNbinsX()):
                            h_syst.SetBinContent(j+1,0)
                            h_syst.SetBinError(j+1,0)
                          h_syst.SetBinContent(1,0.001) # to avoid Combine complaining for empty hist.
                          h_syst.SetBinError(1,0.0001)
                          #if h_syst.Integral() != 0:
                          #  print "[!!ERROR!!] this must be empty. But having :",h_syst.Integral()
                          #  print "Exiting..."
                          #  sys.exit()
                          h_syst.SetDirectory(0) # Store h_syst in memory so that it cannot be deleted during the iteration

                    name_syst = input_list[i][2]+"_"+this_syst # Define syst histo name
                    try:
                      if "DYVBF" in input_list[i][2]: # Scale the syst variated signals
                        h_syst.Scale(DYVBFscaler)
                      elif "SSWW" in input_list[i][2]:
                        h_syst.Scale(SSWWscaler)
                    except AttributeError:
                      print("[!!WARNING!!] There is no hist named "+input_hist+" in "+input_list[i][0]+" .")
                      if "signal" in input_list[i][2]:
                        print "Skipping..."
                        continue
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
              print("[!!WARNING!!] There is no hist corresponding to "+item[2]+" in "+item[0]+" .")
              if "signal" in item[2]:
                print "Skipping..."
                continue
              else:
                print "Making an empty hist..."
                if args.CR:
                  h_missing = h_prompt.Clone() #FIXME
                else:
                  h_missing = h_data.Clone() # With CR, data may not be defined for some regions with no entry ...
                for i in range(h_missing.GetNbinsX()):
                  h_missing.SetBinContent(i+1,0)
                  h_missing.SetBinError(i+1,0)
                h_missing.SetBinContent(1,0.001) # to avoid Combine complaining for empty hist.
                h_missing.SetBinError(1,0.0001)
                #if h_missing.Integral() != 0:
                #  print "[!!ERROR!!] this must be empty. But having :",h_missing.Integral()
                #  print "Exiting..."
                #  sys.exit()
                h_missing.SetName(item[2])
                print "Writing "+item[2]+"..."
                h_missing.Write() # Write empty histogram
                continue

            if item[1].Integral() <=0 : #finally, treat -ve bins
              for i in range(item[1].GetNbinsX()):
                item[1].SetBinContent(i+1,0)
                item[1].SetBinError(i+1,0)
              item[1].SetBinContent(1,0.001) # to avoid Combine complaining for empty hist.
              item[1].SetBinError(1,0.0001)

            print "Writing "+item[2]+"..."
            item[1].Write() # Write each histogram while iterating
          
          outfile.Close()
          print OutputPath+era+"/"+region+"/"+mass+"_"+channel+"_card_input.root has been created."
