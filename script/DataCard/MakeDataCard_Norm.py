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

#eras = ["2016","2017","2018"]
#eras = ["2017"]
eras = ["2018"]
#eras = ["2016preVFP","2016postVFP","2017","2018"]
channels = ["MuMu","EE","EMu"]
#channels = ["MuMu","EE"]
#channels = ["MuMu"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
masses = ["M100","M1000","M10000"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M85","M90","M95","M100","M125","M150","M200","M250","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M1000"]

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
#myWPs = ["rateParam_HNL_ULID_PR46"]
#myWPs = ["PR48_rateParam_HNL_ULID"]
myWPs = ["PR51_HNL_ULID"]

################################################################################################################################################

def CardSetting(isCR, era, channel, mass):

  with open("/data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegion_Plotter/card_skeleton_Norm.txt",'r') as f: # your workspace
    lines = f.readlines()

  regions_cr = ["sr1_inv","sr2_inv","sr3_inv","cf_cr","ww_cr","wz_cr1","wz_cr2","wz_cr3","zg_cr1","zg_cr3","zz_cr1","zz_cr3"]
  lines_cr = {}
  # cr norm setting
  for region in regions_cr:
    this_lines_cr = lines[:]
    this_lines_cr[4] = "shapes * *  "+CRpath+WP+"/"+era+"/"+region+"/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
    if (region == "cf_cr" and "Mu" in channel): continue
    this_lines_cr[17] = "rate                       -1     -1     -1     -1            -1     -1     -1     -1             0            0\n"  # no signal
    for i in range(len(this_lines_cr)):
      this_lines_cr[i] = this_lines_cr[i].replace('bin1',region)
    for i in range(22,40):
      this_lines_cr[i] = "" # remove unnecessary syst sources.

    # handle norm constraints
    for i in range(41,59):
      if "Norm" in this_lines_cr[i]:
        this_lines_cr[i] = this_lines_cr[i].replace('Norm','Norm'+era) # era dependent norm constraint
      if (not region in this_lines_cr[i]):
        this_lines_cr[i] = "" # remove not corresponding rateParams

    lines_cr[region] = this_lines_cr

  # sr setting
  regions_sr = ["sr1","sr2","sr3"]
  lines_sr = {}
  lines_sronly = {}

  for region in regions_sr:
    this_lines_sr = lines[:]
    this_lines_sr[4] = "shapes * *  "+SRpath+WP+"/"+era+"/"+region+"/"+mass+"_"+channel+"_card_input.root $PROCESS $PROCESS_$SYSTEMATIC\n"
    if int(mass.replace("M","")) < 500:
      if "Mu" in channel: this_lines_sr[17] = "rate                       -1     0      -1     -1            -1     -1     -1     -1             -1           0\n"  # no cf, SSWW
      else: this_lines_sr[17] = "rate                       -1     -1     -1     -1            -1     -1     -1     -1             -1           0\n"  # no SSWW
    if 500 <= int(mass.replace("M","")) and int(mass.replace("M","")) <= 3000:
      if "Mu" in channel: this_lines_sr[17] = "rate                       -1     0      -1     -1            -1     -1     -1     -1             -1           -1\n" # no cf
      else: this_lines_sr[17] = "rate                       -1     -1     -1     -1            -1     -1     -1     -1             -1           -1\n"
    elif 3000 < int(mass.replace("M","")):
      if "Mu" in channel: this_lines_sr[17] = "rate                       -1     0      -1     -1            -1     -1     -1     -1             0            -1\n" # no cf, DYVBF
      else: this_lines_sr[17] = "rate                       -1     -1     -1     -1            -1     -1     -1     -1             0            -1\n" # no DYVBF
    for i in range(len(this_lines_sr)):
      this_lines_sr[i] = this_lines_sr[i].replace('bin1',region)
    if not args.Syst:
      for i in range(22,40):
        this_lines_sr[i] = "" # remove unnecessary syst sources.

    # handle norm constraints
    for i in range(41,59):
      if "Norm" in this_lines_sr[i]:
        this_lines_sr[i] = this_lines_sr[i].replace('Norm','Norm'+era) # era dependent norm constraint
      if (not region in this_lines_sr[i]):
        this_lines_sr[i] = "" # remove not corresponding rateParams

    lines_sr[region] = this_lines_sr

    this_lines_sronly = this_lines_sr[:]
    for i in range(41,59):
      this_lines_sronly[i] = "" # remove all rateParams
    lines_sronly[region] = this_lines_sronly

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
        for region in this_card[0].keys():
          with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_"+region+systTag+".txt",'w') as f:
            for line in this_card[0][region]:
              f.write(line)
        for region in this_card[1].keys():
          with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_"+region+".txt",'w') as f:
            for line in this_card[1][region]:
              f.write(line)
      else:
        for region in this_card.keys():
          with open(WP+"/card_"+era+"_"+channel+"_"+mass+"_sronly_"+region+systTag+".txt",'w') as f:
            for line in this_card[region]:
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
          os.system("combineCards.py \
                                     sr1=card_"+era+"_"+channel+"_"+mass+"_sr1"+systTag+".txt \
                                     sr2=card_"+era+"_"+channel+"_"+mass+"_sr2"+systTag+".txt \
                                     sr3=card_"+era+"_"+channel+"_"+mass+"_sr3"+systTag+".txt \
                                     sr1_inv=card_"+era+"_"+channel+"_"+mass+"_sr1_inv.txt \
                                     sr2_inv=card_"+era+"_"+channel+"_"+mass+"_sr2_inv.txt \
                                     sr3_inv=card_"+era+"_"+channel+"_"+mass+"_sr3_inv.txt "\
                                     #sr=card_"+era+"_"+channel+"_"+mass+"_sr"+systTag+".txt \
                                     #sr_inv=card_"+era+"_"+channel+"_"+mass+"_sr_inv.txt "\
                                     +Add_cf_cr+" \
                                     ww_cr=card_"+era+"_"+channel+"_"+mass+"_ww_cr.txt \
                                     zg_cr1=card_"+era+"_"+channel+"_"+mass+"_zg_cr1.txt \
                                     zg_cr3=card_"+era+"_"+channel+"_"+mass+"_zg_cr3.txt \
                                     wz_cr1=card_"+era+"_"+channel+"_"+mass+"_wz_cr1.txt \
                                     wz_cr2=card_"+era+"_"+channel+"_"+mass+"_wz_cr2.txt \
                                     wz_cr3=card_"+era+"_"+channel+"_"+mass+"_wz_cr3.txt \
                                     zz_cr1=card_"+era+"_"+channel+"_"+mass+"_zz_cr1.txt \
                                     zz_cr3=card_"+era+"_"+channel+"_"+mass+"_zz_cr3.txt \
                                     > card_"+era+"_"+channel+"_"+mass+systTag+".txt")
      elif args.Combine == "SR":
        os.system("combineCards.py year16a=card_2016preVFP_"+channel+"_"+mass+"_sronly"+systTag+".txt year16b=card_2016postVFP_"+channel+"_"+mass+"_sronly"+systTag+".txt year17=card_2017_"+channel+"_"+mass+"_sronly"+systTag+".txt year18=card_2018_"+channel+"_"+mass+"_sronly"+systTag+".txt > card_Run2_"+channel+"_"+mass+"_sronly"+systTag+".txt")
      else:
        os.system("combineCards.py year16a=card_2016preVFP_"+channel+"_"+mass+systTag+".txt year16b=card_2016postVFP_"+channel+"_"+mass+systTag+".txt year17=card_2017_"+channel+"_"+mass+systTag+".txt year18=card_2018_"+channel+"_"+mass+systTag+".txt > card_Run2_"+channel+"_"+mass+systTag+".txt")
    os.system('echo \'Done.\'')
    os.chdir(pwd)
