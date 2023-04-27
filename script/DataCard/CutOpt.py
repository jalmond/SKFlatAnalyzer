from ROOT import *
import math

def FoM(sig, bkg):
  return math.sqrt( 2.*((sig+bkg)*log(1+(sig/bkg)) - sig) )

def ScanSR3(mass, channel):
  f_DYVBF = TFile.Open("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_before230415Merge/2017/HNL_SignalRegionPlotter_signalDYVBF_M"+mass+".root")
  f_SSWW  = TFile.Open("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_before230415Merge/2017/HNL_SignalRegionPlotter_signalSSWW_M"+mass+".root")
  f_MC    = TFile.Open("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_before230415Merge/2017/HNL_SignalRegionPlotter_SkimTree_HNMultiLepBDT_MC.root")
  f_fake  = TFile.Open("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_before230415Merge/2017/RunFake__/DATA/HNL_SignalRegionPlotter_SkimTree_HNMultiLepBDT_Fake.root")
  f_CF    = TFile.Open("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_before230415Merge/2017/RunCF__/HNL_SignalRegionPlotter_SkimTree_HNMultiLepBDT_CF.root")
  f_conv  = TFile.Open("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_before230415Merge/2017/RunConv__/HNL_SignalRegionPlotter_SkimTree_HNMultiLepBDT_Conv.root")
  
  h1_path = channel+"/MVAUL_UL/RegionPlots_ZeroJetSR3/Lep_2_pt"
  h2_path = channel+"/MVAUL_UL/RegionPlots_OneJetSR3/Lep_2_pt"

  try:
    h1_DYVBF = f_DYVBF.Get(h1_path)
  except ReferenceError:
    pass
  h1_SSWW = f_SSWW.Get(h1_path)
  h1_MC = f_MC.Get(h1_path)
  h1_fake = f_fake.Get(h1_path)
  if "E" in channel:
    h1_CF = f_CF.Get(h1_path)
  h1_conv = f_conv.Get(h1_path)
  h1_bkg = h1_MC.Clone()
  h1_bkg.Add(h1_fake)
  if "E" in channel:
    h1_bkg.Add(h1_CF)
  h1_bkg.Add(h1_conv)
  
  try:
    h2_DYVBF = f_DYVBF.Get(h2_path)
  except ReferenceError:
    pass
  h2_SSWW = f_SSWW.Get(h2_path)
  h2_MC = f_MC.Get(h2_path)
  h2_fake = f_fake.Get(h2_path)
  if "E" in channel:
    h2_CF = f_CF.Get(h2_path)
  h2_conv = f_conv.Get(h2_path)
  h2_bkg = h2_MC.Clone()
  h2_bkg.Add(h2_fake)
  if "E" in channel:
    h2_bkg.Add(h2_CF)
  h2_bkg.Add(h2_conv)
 
  print "================================================================================="
  print "mass : "+mass+" GeV, channel : "+channel

  try:
    tot_sig_1 = h1_DYVBF.Integral(1,1000)+h1_SSWW.Integral(1,1000)
    tot_sig_2 = h2_DYVBF.Integral(1,1000)+h2_SSWW.Integral(1,1000)
  except UnboundLocalError:
    tot_sig_1 = h1_SSWW.Integral(1,1000)
    tot_sig_2 = h2_SSWW.Integral(1,1000)

  for i in range(31):
    try:
      this_sig_1 = h1_DYVBF.Integral(i+1,1000)+h1_SSWW.Integral(i+1,1000)
      this_sig_2 = h2_DYVBF.Integral(i+1,1000)+h2_SSWW.Integral(i+1,1000)
    except UnboundLocalError:
      this_sig_1 = h1_SSWW.Integral(i+1,1000)
      this_sig_2 = h2_SSWW.Integral(i+1,1000)
    this_bkg_1 = h1_bkg.Integral(i+1,1000)
    this_bkg_2 = h2_bkg.Integral(i+1,1000)
    if this_bkg_1 <= 0. or this_bkg_2 <= 0.:
      print "cut on", 20*(i), "GeV : zero bkg!! skipping..."
    else:
      print "cut on", 20*(i), "GeV : FoM", FoM(this_sig_1,this_bkg_1)+FoM(this_sig_2,this_bkg_2), ", sig. eff.", 100*(this_sig_1+this_sig_2)/(tot_sig_1+tot_sig_2), "%, bkg. rej.", 100-100*(this_bkg_1+this_bkg_2)/(h1_bkg.Integral(1,1000)+h2_bkg.Integral(1,1000)), "%, Nbkg", this_bkg_1+this_bkg_2

ScanSR3("10000","MuMu")
ScanSR3("10000","EE")
ScanSR3("10000","EMu")
ScanSR3("1000","MuMu")
