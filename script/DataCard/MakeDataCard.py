# Make Datacards
# Place this at CombineTool/CMSSW_10_2_13/src/<your working directory>
# You need to place card_skeletons already

import os, sys, argparse

parser = argparse.ArgumentParser(description='script for creating or merging data cards.',formatter_class=argparse.RawTextHelpFormatter)
parser.add_argument('--CR', action='store_true', help='Make datacards named sr with HNL_SignalRegion_Plotter and sr_inv with HNL_ControlRegion_Plotter input. (Default : SR only)')
parser.add_argument('--Syst', action='store_true', help='Add systematics into the datacards')
parser.add_argument('--Combine', choices=['CR','SR','Era'], help='CR --> Merge CR and SR datacards in one era,\nEra --> Merge pre-processed (CR+SR) over the Run2,\nSR --> Merge SR only datacards over the Run2')
args = parser.parse_args()

pwd = os.getcwd()

#####################################################
#
# args.CR --> sr, sr_inv connected via rateParam
# else --> sr only, bkg norm uncert. treated by lnN
# args.syst --> postpone
#
#####################################################

eras = ["2016","2017","2018"]
eras = ["2017"]
#eras = ["2016preVFP","2016postVFP","2018"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
masses = ["M100","M1000","M10000"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M85","M90","M95","M100","M125","M150","M200","M250","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]

#SRpath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter_PR43/LimitInputs/"
#SRpath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/LimitInputs/"
#CRpath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_ControlRegion_Plotter/LimitInputs/"
SRpath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/LimitExtraction/"
CRpath = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_ControlRegion_Plotter/LimitExtraction/"

#myWPs = ["240422_HNL_ULID"]
#myWPs = ["240501_1704_HNL_ULID","240501_1704_HNTightV2"]
#myWPs = ["240504_PR44_HNL_ULID"]
#myWPs = ["240505_PR45_HNL_ULID"]
#myWPs = ["240505_PR46_HNL_ULID","240505_PR46_HNTightV2"]
myWPs = ["rateParam_HNL_ULID"]

################################################################################################################################################

def CardSetting(isCR, era, channel, mass):

  with open("/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter/card_skeleton.txt",'r') as f: # your workspace
    lines = f.readlines()

  # skip the lepton SF syst for now ...
  #for i in range(len(lines)):
  #  if channel == "MuMu":
  #    if "Muon" in lines[i]: lines[i] = lines_orig[i]
  #    if "Electron" in lines[i]: lines[i] = lines[i].replace("1","-")
  #  elif channel == "EE":
  #    if "Muon" in lines[i]: lines[i] = lines[i].replace("1","-")
  #    if "Electron" in lines[i]: lines[i] = lines_orig[i]
  #  elif channel == "EMu":
  #    if "Muon" in lines[i]: lines[i] = lines_orig[i]
  #    if "Electron" in lines[i]: lines[i] = lines_orig[i]

  # wz_cr setting
  #lines_wz_cr = lines[:]
  #lines_wz_cr[4] = "shapes * *  "+CRpath+WP+"/"+era+"/wz/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
  #if "Mu" in channel: lines_wz_cr[17] = "rate                       -1      -1    0     -1    0            0\n"  # no cf
  #else: lines_wz_cr[17] = "rate                      -1      -1    -1    -1     0            0\n"                  # no signal FIXME will add signal in CRs
  #lines_wz_cr[20] = "" # remove lnN bkg normalization uncert.
  #for i in range(len(lines_wz_cr)):
  #  lines_wz_cr[i] = lines_wz_cr[i].replace('bin1','wz_cr')
  #for i in range(22,40):
  #  lines_wz_cr[i] = "" # remove unnecessary syst sources.

  # sr_inv setting
  lines_sr_inv = lines[:]
  #lines_sr_inv[4] = "shapes * *  "+CRpath+WP+"/"+era+"/SS_CR__/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
  lines_sr_inv[4] = "shapes * *  "+CRpath+WP+"/"+era+"/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
  if "Mu" in channel: lines_sr_inv[17] = "rate                       -1      -1    0     -1    0            0\n"  # no cf
  else: lines_sr_inv[17] = "rate                      -1      -1    -1    -1     0            0\n"                  # no signal FIXME will add signal in CRs
  lines_sr_inv[20] = "" # remove lnN bkg normalization uncert.
  for i in range(len(lines_sr_inv)):
    lines_sr_inv[i] = lines_sr_inv[i].replace('bin1','sr_inv')
  for i in range(22,40):
    lines_sr_inv[i] = "" # remove unnecessary syst sources.

  # sr setting
  lines[4] = "shapes * *  "+SRpath+WP+"/"+era+"/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
  if int(mass.replace("M","")) < 500:
    if "Mu" in channel: lines[17] = "rate                       -1      -1    0     -1    -1           0\n"  # no cf
    else: lines[17] = "rate                       -1      -1    -1    -1    -1           0\n"                  # no SSWW
  if 500 <= int(mass.replace("M","")) and int(mass.replace("M","")) <= 3000:
    if "Mu" in channel: lines[17] = "rate                       -1      -1    0     -1    -1           -1\n"
    else: lines[17] = "rate                       -1      -1    -1    -1    -1           -1\n"
  elif 3000 < int(mass.replace("M","")):
    if "Mu" in channel: lines[17] = "rate                       -1      -1    0     -1    0            -1\n"
    else: lines[17] = "rate                       -1      -1    -1    -1    0            -1\n"                  # no DYVBF
  for i in range(len(lines)):
    lines[i] = lines[i].replace('bin1','sr')
  if not args.Syst:
    for i in range(22,40):
      lines[i] = "" # remove unnecessary syst sources.

  lines_sr = lines[:]
  lines_sr[20] = "" # remove lnN bkg normalization uncert.

  lines_sronly = lines[:]
  lines_sronly[-2] = "" # remove PromptNorm rateParam
  lines_sronly[-1] = "" # remove ConvNorm rateParam

  if isCR:
    return (lines_sr, lines_sr_inv)
  else:
    return lines_sronly
  
#########################################
#
# MAIN
#
#########################################

if args.Syst:
  systTag = "_syst"
else:
  systTag = ""

for WP in myWPs:
  if not args.Combine:
    os.system("mkdir -p "+WP)
    os.system("ln -s /data6/Users/jihkim/SKFlatAnalyzer/script/DataCard/MakeWorkspace.py "+WP)
    os.system("ln -s /data6/Users/jihkim/SKFlatAnalyzer/script/DataCard/CheckNuisance.py "+WP)

    for era, channel, mass in [(era, channel, mass) for era in eras for channel in channels for mass in masses]:
      this_card = CardSetting(args.CR, era, channel, mass)
      if args.CR:
        with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_sr"+systTag+".txt",'w') as f:
          for line in this_card[0]:
            f.write(line)
        with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_sr_inv.txt",'w') as f:
          for line in this_card[1]:
            f.write(line)
      else:
        with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_sronly"+systTag+".txt",'w') as f:
          for line in this_card:
            f.write(line)

  else:
    os.chdir(WP)
    os.system('echo \'Currently combining cards at...\'')
    os.system('pwd')
    if args.Syst:
      os.system('echo \'Systematics have been added.\'')
    for channel, mass in [(channel, mass) for channel in channels for mass in masses]:
      if args.Combine == "CR":
        for era in eras:
          os.system("combineCards.py sr=card_"+era+"_"+channel+"_"+mass+"_sr"+systTag+".txt sr_inv=card_"+era+"_"+channel+"_"+mass+"_sr_inv.txt > card_"+era+"_"+channel+"_"+mass+systTag+".txt")
      elif args.Combine == "SR":
        os.system("combineCards.py year16a=card_2016preVFP_"+channel+"_"+mass+"_sronly"+systTag+".txt year16b=card_2016postVFP_"+channel+"_"+mass+"_sronly"+systTag+".txt year17=card_2017_"+channel+"_"+mass+"_sronly"+systTag+".txt year18=card_2018_"+channel+"_"+mass+"_sronly"+systTag+".txt > card_Run2_"+channel+"_"+mass+"_sronly"+systTag+".txt")
      else:
        os.system("combineCards.py year16a=card_2016preVFP_"+channel+"_"+mass+systTag+".txt year16b=card_2016postVFP_"+channel+"_"+mass+systTag+".txt year17=card_2017_"+channel+"_"+mass+systTag+".txt year18=card_2018_"+channel+"_"+mass+systTag+".txt > card_Run2_"+channel+"_"+mass+systTag+".txt")
    os.system('echo \'Done.\'')
    os.chdir(pwd)
