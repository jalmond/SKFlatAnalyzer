from ROOT import *
import os

era = "2017"
ID = "HNL_ULID"
card_name = "M1700_EE"
signal_name = "signalDYVBF" #"signalSSWW"

pathlist = []
taglist = []
pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter_PR43/LimitInputs/240501_1704_"+ID+"/"+era+"/"+card_name+"_card_input.root")
taglist.append("PR43")
pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter_PR44/LimitInputs/240504_PR44_"+ID+"/"+era+"/"+card_name+"_card_input.root")
taglist.append("PR44")
pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/LimitInputs/240505_PR45_"+ID+"/"+era+"/"+card_name+"_card_input.root")
taglist.append("PR45")

final_taglist = []
siglist = []
bkglist = []
fakelist = []
cflist = []
convlist = []
promptlist = []
FOMlist = []
for i in range(len(pathlist)):
  this_file = TFile.Open(pathlist[i])
  this_tag  = []
  this_sig  = []
  this_bkg  = []
  this_fake = []
  this_cf   = []
  this_conv = []
  this_prompt = []
  this_FOM  = []
  for j in range(this_file.Get("data_obs").GetNbinsX()):
    this_tag.append(taglist[i]+' ('+this_file.Get("data_obs").GetXaxis().GetBinLabel(j+1)+')')
    this_sig.append(this_file.Get(signal_name).GetBinContent(j+1))
    this_bkg.append(this_file.Get("data_obs").GetBinContent(j+1))
    this_fake.append(this_file.Get("fake").GetBinContent(j+1))
    this_cf.append(this_file.Get("cf").GetBinContent(j+1))
    this_conv.append(this_file.Get("conv").GetBinContent(j+1))
    this_prompt.append(this_file.Get("prompt").GetBinContent(j+1))
    if this_bkg[j]<=0: this_bkg[j] = 0.001
    this_FOM.append( sqrt(2.*((this_sig[j]+this_bkg[j])*log(1+(this_sig[j]/(this_bkg[j])))-this_sig[j])) )
  final_taglist.append(this_tag)
  siglist.append(this_sig)
  bkglist.append(this_bkg)
  fakelist.append(this_fake)
  cflist.append(this_cf)
  convlist.append(this_conv)
  promptlist.append(this_prompt)
  FOMlist.append(this_FOM)

for i in range(len(pathlist)):
  this_file = TFile.Open(pathlist[i])
  maxNbin = 0
  maxNbin = max(maxNbin, this_file.Get("data_obs").GetNbinsX())
#print maxNbin

names = []
contents = []
tot_sig    = []
tot_bkg    = []
tot_fake   = []
tot_cf     = []
tot_conv   = []
tot_prompt = []
tot_FOM    = []
for j in range(len(taglist)):
  tot_sig   .append([0.])
  tot_bkg   .append([0.])
  tot_fake  .append([0.])
  tot_cf    .append([0.])
  tot_conv  .append([0.])
  tot_prompt.append([0.])
  tot_FOM   .append([0.])

for i in range(maxNbin+1):
  this_names = []
  if(i<maxNbin): this_names.append('bin'+str(i+1))
  else: this_names.append('Total')
  this_names.append('sig')
  this_names.append('bkg')
  this_names.append('fake')
  this_names.append('cf')
  this_names.append('conv')
  this_names.append('prompt')
  this_names.append('FOM')
  names.append(this_names)
  this_contents = []
  this_final_taglist = []
  this_siglist = []
  this_bkglist = []
  this_fakelist = []
  this_cflist = []
  this_convlist = []
  this_promptlist = []
  this_FOMlist = []
  if(i<maxNbin):
    for j in range(len(taglist)):
      try:
        this_final_taglist.append(final_taglist[j][i])
        this_siglist.append(siglist[j][i])
        this_bkglist.append(bkglist[j][i])
        this_fakelist.append(fakelist[j][i])
        this_cflist.append(cflist[j][i])
        this_convlist.append(convlist[j][i])
        this_promptlist.append(promptlist[j][i])
        this_FOMlist.append(FOMlist[j][i])
        tot_sig[j]   [0] += siglist[j][i]
        tot_bkg[j]   [0] += bkglist[j][i]
        tot_fake[j]  [0] += fakelist[j][i]
        tot_cf[j]    [0] += cflist[j][i]
        tot_conv[j]  [0] += convlist[j][i]
        tot_prompt[j][0] += promptlist[j][i]
        tot_FOM[j]   [0] += FOMlist[j][i]
      except IndexError:
        this_final_taglist.append(taglist[j]+' (N/A)')
        this_siglist.append(0)
        this_bkglist.append(0)
        this_fakelist.append(0)
        this_cflist.append(0)
        this_convlist.append(0)
        this_promptlist.append(0)
        this_FOMlist.append(0)
    this_contents.append(this_final_taglist)
    this_contents.append(this_siglist)
    this_contents.append(this_bkglist)
    this_contents.append(this_fakelist)
    this_contents.append(this_cflist)
    this_contents.append(this_convlist)
    this_contents.append(this_promptlist)
    this_contents.append(this_FOMlist)
    contents.append(this_contents)
  else:
    final_tot_sig    = []
    final_tot_bkg    = []
    final_tot_fake   = []
    final_tot_cf     = []
    final_tot_conv   = []
    final_tot_prompt = []
    final_tot_FOM    = []
    for j in range(len(taglist)):
      final_tot_sig   .append(tot_sig[j][0])
      final_tot_bkg   .append(tot_bkg[j][0])
      final_tot_fake  .append(tot_fake[j][0])
      final_tot_cf    .append(tot_cf[j][0])
      final_tot_conv  .append(tot_conv[j][0])
      final_tot_prompt.append(tot_prompt[j][0])
      final_tot_FOM   .append(tot_FOM[j][0])
    this_contents.append(taglist)
    this_contents.append(final_tot_sig)
    this_contents.append(final_tot_bkg)
    this_contents.append(final_tot_fake)
    this_contents.append(final_tot_cf)
    this_contents.append(final_tot_conv)
    this_contents.append(final_tot_prompt)
    this_contents.append(final_tot_FOM)
    contents.append(this_contents)

with open("CompareFOM.txt", 'w') as f:
  f.write(era+' '+ID+' '+card_name+' '+signal_name+' (if bkg <=0 --> 1.00e-03)\n')
  for i in range(maxNbin+1):
    f.write('=========================================================================\n')
    for c in range(len(names[0])):
      f.write('%-10s'%names[i][c])
      for j in range(len(taglist)):
        #print contents[i][c][j]
        if type(contents[i][c][j]) == str:
          f.write('%-21s'%contents[i][c][j])
        else:
          f.write('%-21s'%format(contents[i][c][j],'.2e'))
      f.write('\n')
 
