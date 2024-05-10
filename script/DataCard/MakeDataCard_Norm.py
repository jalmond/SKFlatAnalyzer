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
#masses = ["M100","M1000","M10000"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
masses = ["M85","M90","M95","M100","M125","M150","M200","M250","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]

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

  with open("/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter/card_skeleton_Norm.txt",'r') as f: # your workspace
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

  regions = ["sr_inv","cf_cr","ww_cr","zg_cr","wz_cr","wzewk_cr","zz_cr"]
  lines_cr = {}
  # cr norm setting
  for region in regions:
    this_lines_cr = lines[:]
    this_lines_cr[4] = "shapes * *  "+CRpath+WP+"/"+era+"/"+region+"/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
    #### !! crazy hard-coded process rate !! #### FIXME
    # fake   cf     zg     conv   wz     zz     ww     wzewk  prompt  signalDYVBF  signalSSWW
    # 1      2      3      4      5      6      7      8      9       -1           0
    if region == "sr_inv":
      if "Mu" in channel:
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     -1     -1     -1      0            0\n"  # no signal
      else:
        this_lines_cr[17] = "rate                       -1     -1     -1     -1     -1     -1     -1     -1     -1      0            0\n"  # no signal
    elif region == "cf_cr":
      if "Mu" in channel: continue
      else:
        this_lines_cr[17] = "rate                       -1     -1     -1     -1     -1     -1     -1     -1     -1      0            0\n"  # no signal
    elif region == "ww_cr":
      if channel=="EE":
        this_lines_cr[17] = "rate                       -1     -1     -1     -1     -1     -1     -1     -1     -1      0            0\n"  # no signal
      if channel=="EMu":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     -1     -1     -1      0            0\n"  # no cf, signal
      elif channel=="MuMu":
        this_lines_cr[17] = "rate                       -1     0      0      -1     -1     -1     -1     -1     -1      0            0\n"  # no cf, zg, signal
    elif region == "zg_cr":
      if channel=="EE":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     0      -1     -1      0            0\n"  # no cf, ww, signal
      if channel=="EMu":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     0      -1     -1      0            0\n"  # no cf, ww, signal
      elif channel=="MuMu":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     0      -1     -1      0            0\n"  # no cf, ww, signal
    elif region == "wz_cr":
      if channel=="EE":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     0      -1     -1      0            0\n"  # no signal
      if channel=="EMu":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     0      -1     -1      0            0\n"  # no cf, signal
      elif channel=="MuMu":
        this_lines_cr[17] = "rate                       -1     0      0      -1     -1     -1     0      -1     -1      0            0\n"  # no cf, zg, signal
    elif region == "wzewk_cr":
      if channel=="EE":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     0      -1     -1      0            0\n"  # no signal
      if channel=="EMu":
        this_lines_cr[17] = "rate                       -1     0      -1     -1     -1     -1     0      -1     -1      0            0\n"  # no cf, signal
      elif channel=="MuMu":
        this_lines_cr[17] = "rate                       -1     0      -1     0      -1     -1     -1     -1     -1      0            0\n"  # no cf, zg, signal
    elif region == "zz_cr":
      if channel=="EE":
        this_lines_cr[17] = "rate                       -1     0      0      -1     0      -1     0      0      -1      0            0\n"  # no signal
      if channel=="EMu":
        this_lines_cr[17] = "rate                       -1     0      0      -1     0      -1     0      0      -1      0            0\n"  # no signal
      elif channel=="MuMu":
        this_lines_cr[17] = "rate                       -1     0      0      0      0      -1     0      0      -1      0            0\n"  # no signal
    if "Mu" in channel:
      this_lines_cr[41] = ""  # remove cf norm
      this_lines_cr[48] = ""  # remove cf norm
    for i in range(len(this_lines_cr)):
      this_lines_cr[i] = this_lines_cr[i].replace('bin1',region)
    for i in range(22,40):
      this_lines_cr[i] = "" # remove unnecessary syst sources.
    for i in range(41,55):
      if (not region in lines[i]):
        this_lines_cr[i] = "" # remove not corresponding rateParams

    lines_cr[region] = this_lines_cr

  # sr setting
  lines[4] = "shapes * *  "+SRpath+WP+"/"+era+"/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
  if int(mass.replace("M","")) < 500:
    if "Mu" in channel: lines[17] = "rate                       -1     0      -1     -1     -1     -1     -1     -1     -1      -1           0\n"  # no cf
    else: lines[17] = "rate                       -1     -1     -1     -1     -1     -1     -1     -1     -1      -1           0\n"  # no SSWW
  if 500 <= int(mass.replace("M","")) and int(mass.replace("M","")) <= 3000:
    if "Mu" in channel: lines[17] = "rate                       -1     0      -1     -1     -1     -1     -1     -1     -1      -1           -1\n"
    else: lines[17] = "rate                       -1     -1     -1     -1     -1     -1     -1     -1     -1      -1           -1\n"
  elif 3000 < int(mass.replace("M","")):
    if "Mu" in channel: lines[17] = "rate                       -1     0      -1     -1     -1     -1     -1     -1     -1      0            -1\n"
    else: lines[17] = "rate                       -1     -1     -1     -1     -1     -1     -1     -1     -1      0            -1\n"                  # no DYVBF
  for i in range(len(lines)):
    lines[i] = lines[i].replace('bin1','sr')
  if not args.Syst:
    for i in range(22,40):
      lines[i] = "" # remove unnecessary syst sources.
  for i in range(41,55):
    if (not "sr" in lines[i]) or ("inv" in lines[i]):
      lines[i] = "" # remove cr rateParams
  if "Mu" in channel:
    lines[41] = ""  # remove cf norm
    lines[48] = ""  # remove cf norm

  lines_sr = lines[:]

  lines_sronly = lines[:]
  for i in range(41,55):
    lines_sronly[i] = "" # remove all rateParams

  if isCR:
    return (lines_sr, lines_cr)
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
        for region in this_card[1].keys():
          with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_"+region+".txt",'w') as f:
            for line in this_card[1][region]:
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
          if "Mu" in channel: Add_cf_cr = ""
          else: Add_cf_cr = "cf_cr=card_"+era+"_"+channel+"_"+mass+"_cf_cr.txt"
          os.system("combineCards.py sr=card_"+era+"_"+channel+"_"+mass+"_sr"+systTag+".txt \
                                     sr_inv=card_"+era+"_"+channel+"_"+mass+"_sr_inv.txt "\
                                     +Add_cf_cr+" \
                                     ww_cr=card_"+era+"_"+channel+"_"+mass+"_ww_cr.txt \
                                     zg_cr=card_"+era+"_"+channel+"_"+mass+"_zg_cr.txt \
                                     wz_cr=card_"+era+"_"+channel+"_"+mass+"_wz_cr.txt \
                                     wzewk_cr=card_"+era+"_"+channel+"_"+mass+"_wzewk_cr.txt \
                                     zz_cr=card_"+era+"_"+channel+"_"+mass+"_zz_cr.txt \
                                     > card_"+era+"_"+channel+"_"+mass+systTag+".txt")
      elif args.Combine == "SR":
        os.system("combineCards.py year16a=card_2016preVFP_"+channel+"_"+mass+"_sronly"+systTag+".txt year16b=card_2016postVFP_"+channel+"_"+mass+"_sronly"+systTag+".txt year17=card_2017_"+channel+"_"+mass+"_sronly"+systTag+".txt year18=card_2018_"+channel+"_"+mass+"_sronly"+systTag+".txt > card_Run2_"+channel+"_"+mass+"_sronly"+systTag+".txt")
      else:
        os.system("combineCards.py year16a=card_2016preVFP_"+channel+"_"+mass+systTag+".txt year16b=card_2016postVFP_"+channel+"_"+mass+systTag+".txt year17=card_2017_"+channel+"_"+mass+systTag+".txt year18=card_2018_"+channel+"_"+mass+systTag+".txt > card_Run2_"+channel+"_"+mass+systTag+".txt")
    os.system('echo \'Done.\'')
    os.chdir(pwd)
