from ROOT import *
import os, argparse

parser = argparse.ArgumentParser(description='script for checking FOM',formatter_class=argparse.RawTextHelpFormatter)
parser.add_argument('--Opt', action='store_true', help='Optimization setup')
args = parser.parse_args()

if args.Opt:
  ## file setting ##
  #eras = ["2016preVFP", "2016postVFP", "2017", "2018"]
  eras = ["Run2"]
  #eras = ["2018"]
  ID = "HNL_ULID"
  #channel = "MuMu"
  channel = "EE"
  #region = "InitialSR2"
  region = "PassSR2"

  os.system('rm CompareFOM_Opt.txt')
  for era in eras:
    ## row setting ##
    pathlist = []
    pathlist.append("PlaceHolder")
    pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/RunFake__/DATA/HNL_SignalRegion_Plotter_SkimTree_HNMultiLepBDT_Fake.root")
    if not "Mu" in channel:
      pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/RunCF__/DATA/HNL_SignalRegion_Plotter_SkimTree_DileptonBDT_CF.root")
    pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/RunConv__/HNL_SignalRegion_Plotter_SkimTree_HNMultiLepBDT_Conv_inc.root")
    pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/RunPrompt__/HNL_SignalRegion_Plotter_SkimTree_HNMultiLepBDT_Prompt_inc.root")
    pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/HNL_SignalRegion_Plotter_signalDYVBF_M1000.root")
    pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/HNL_SignalRegion_Plotter_signalSSWW_M1000.root")
    pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/HNL_SignalRegion_Plotter_signalSSWW_M10000.root")
    #pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/HNL_SignalRegion_Plotter_SkimTree_HNMultiLepBDT_SSWWTypeI_SF_M1000_private.root")
    #pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/"+era+"/HNL_SignalRegion_Plotter_SkimTree_HNMultiLepBDT_SSWWTypeI_SF_M10000_private.root")

    rowNamelist = []
    rowNamelist.append("Selection")
    rowNamelist.append("fake")
    if not "Mu" in channel:
      rowNamelist.append("cf")
    rowNamelist.append("conv")
    rowNamelist.append("prompt")
    rowNamelist.append("DYVBF_M1000")
    rowNamelist.append("SSWW_M1000")
    rowNamelist.append("SSWW_M10000")

    ## column setting ##
    test_sel1 = []
    test_sel1.append([region+"/"+ID+"/"+channel+"/VBF/Lead_DEta_jet1_jet2","gt",2.5]) # hist, gt(lt), value(xxx)
    test_sel1.append([region+"/"+ID+"/"+channel+"/VBF/Lead_DEta_jet1_jet2","gt",3]) # hist, gt(lt), value(xxx)
    test_sel1.append([region+"/"+ID+"/"+channel+"/VBF/Lead_DEta_jet1_jet2","gt",3.5]) # hist, gt(lt), value(xxx)
    test_sel1.append([region+"/"+ID+"/"+channel+"/VBF/Lead_DEta_jet1_jet2","gt",4]) # hist, gt(lt), value(xxx)
    test_sel1.append([region+"/"+ID+"/"+channel+"/VBF/Lead_DEta_jet1_jet2","gt",4.5]) # hist, gt(lt), value(xxx)
    test_sel2 = []
    test_sel2.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_jet1_jet2","gt",2.5]) # hist, gt(lt), value(xxx)
    test_sel2.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_jet1_jet2","gt",3]) # hist, gt(lt), value(xxx)
    test_sel2.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_jet1_jet2","gt",3.5]) # hist, gt(lt), value(xxx)
    test_sel2.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_jet1_jet2","gt",4]) # hist, gt(lt), value(xxx)
    test_sel2.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_jet1_jet2","gt",4.5]) # hist, gt(lt), value(xxx)
    test_sel3 = []
    test_sel3.append([region+"/"+ID+"/"+channel+"/VBF/Lead_MJJ","gt",650])   # hist, gt(lt), value(xxx)
    test_sel3.append([region+"/"+ID+"/"+channel+"/VBF/Lead_MJJ","gt",700])   # hist, gt(lt), value(xxx)
    test_sel3.append([region+"/"+ID+"/"+channel+"/VBF/Lead_MJJ","gt",750])   # hist, gt(lt), value(xxx)
    test_sel3.append([region+"/"+ID+"/"+channel+"/VBF/Lead_MJJ","gt",800])   # hist, gt(lt), value(xxx)
    test_sel3.append([region+"/"+ID+"/"+channel+"/VBF/Lead_MJJ","gt",850])   # hist, gt(lt), value(xxx)
    test_sel4 = []
    test_sel4.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEtaJets_MJJ","gt",650])   # hist, gt(lt), value(xxx)
    test_sel4.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEtaJets_MJJ","gt",700])   # hist, gt(lt), value(xxx)
    test_sel4.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEtaJets_MJJ","gt",750])   # hist, gt(lt), value(xxx)
    test_sel4.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEtaJets_MJJ","gt",800])   # hist, gt(lt), value(xxx)
    test_sel4.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEtaJets_MJJ","gt",850])   # hist, gt(lt), value(xxx)
    test_sel5 = []
    test_sel5.append([region+"/"+ID+"/"+channel+"/VBF/Lead_zeppenfeld","lt",0.8])   # hist, gt(lt), value(xxx)
    test_sel5.append([region+"/"+ID+"/"+channel+"/VBF/Lead_zeppenfeld","lt",0.75])   # hist, gt(lt), value(xxx)
    test_sel5.append([region+"/"+ID+"/"+channel+"/VBF/Lead_zeppenfeld","lt",0.7])   # hist, gt(lt), value(xxx)
    test_sel5.append([region+"/"+ID+"/"+channel+"/VBF/Lead_zeppenfeld","lt",0.65])   # hist, gt(lt), value(xxx)
    test_sel5.append([region+"/"+ID+"/"+channel+"/VBF/Lead_zeppenfeld","lt",0.6])   # hist, gt(lt), value(xxx)
    test_sel6 = []
    test_sel6.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_Jets_zeppenfeld","lt",0.8])   # hist, gt(lt), value(xxx)
    test_sel6.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_Jets_zeppenfeld","lt",0.75])   # hist, gt(lt), value(xxx)
    test_sel6.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_Jets_zeppenfeld","lt",0.7])   # hist, gt(lt), value(xxx)
    test_sel6.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_Jets_zeppenfeld","lt",0.65])   # hist, gt(lt), value(xxx)
    test_sel6.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_Jets_zeppenfeld","lt",0.6])   # hist, gt(lt), value(xxx)
    #test_sel.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_jet1_jet2","gt",2.5])   # hist, gt(lt), value(xxx)
    #test_sel.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEtaJets_MJJ","gt",750])   # hist, gt(lt), value(xxx)
    #test_sel.append([region+"/"+ID+"/"+channel+"/VBF/MaxDEta_Jets_zeppenfeld","lt",0.75])   # hist, gt(lt), value(xxx)
    test_sel = []
    test_sel.append(test_sel1)
    test_sel.append(test_sel2)
    test_sel.append(test_sel3)
    test_sel.append(test_sel4)
    test_sel.append(test_sel5)
    test_sel.append(test_sel6)

    colNamelist1 = []
    colNamelist1.append("Lead_dEta > 2.5")
    colNamelist1.append("Lead_dEta > 3")
    colNamelist1.append("Lead_dEta > 3.5")
    colNamelist1.append("Lead_dEta > 4")
    colNamelist1.append("Lead_dEta > 4.5")
    colNamelist2 = []
    colNamelist2.append("Max_dEta > 2.5")
    colNamelist2.append("Max_dEta > 3")
    colNamelist2.append("Max_dEta > 3.5")
    colNamelist2.append("Max_dEta > 4")
    colNamelist2.append("Max_dEta > 4.5")
    colNamelist3 = []
    colNamelist3.append("Lead_Mjj > 650 GeV")
    colNamelist3.append("Lead_Mjj > 700 GeV")
    colNamelist3.append("Lead_Mjj > 750 GeV")
    colNamelist3.append("Lead_Mjj > 800 GeV")
    colNamelist3.append("Lead_Mjj > 850 GeV")
    colNamelist4 = []
    colNamelist4.append("MaxDEta_Mjj > 650 GeV")
    colNamelist4.append("MaxDEta_Mjj > 700 GeV")
    colNamelist4.append("MaxDEta_Mjj > 750 GeV")
    colNamelist4.append("MaxDEta_Mjj > 800 GeV")
    colNamelist4.append("MaxDEta_Mjj > 850 GeV")
    colNamelist5 = []
    colNamelist5.append("Lead_zep < 0.8")
    colNamelist5.append("Lead_zep < 0.75")
    colNamelist5.append("Lead_zep < 0.7")
    colNamelist5.append("Lead_zep < 0.65")
    colNamelist5.append("Lead_zep < 0.6")
    colNamelist6 = []
    colNamelist6.append("MaxDEta_zep < 0.8")
    colNamelist6.append("MaxDEta_zep < 0.75")
    colNamelist6.append("MaxDEta_zep < 0.7")
    colNamelist6.append("MaxDEta_zep < 0.65")
    colNamelist6.append("MaxDEta_zep < 0.6")
    #colNamelist.append("Max_dEta > 2.5")
    #colNamelist.append("Max_dEta_Mjj > 750 GeV")
    #colNamelist.append("Max_dEta_zep < 0.75")
    colNamelist = []
    colNamelist.append(colNamelist1)
    colNamelist.append(colNamelist2)
    colNamelist.append(colNamelist3)
    colNamelist.append(colNamelist4)
    colNamelist.append(colNamelist5)
    colNamelist.append(colNamelist6)
    
    with open("CompareFOM_Opt.txt", 'a') as f:
      f.write(era+' '+region+' '+ID+' '+channel+' (if bkg <=0 --> 1.00e-03)\n')
      f.write('='*150+'\n')

      for k in range(len(test_sel)): # iterate for multiple variables
        bkg_survive = [0.] * len(test_sel[k])
        bkg_total = [0.] * len(test_sel[k])
        for i in range(len(pathlist)): # write each row (file)
          this_file    = TFile.Open(pathlist[i])
          #print this_file
          f.write('%-14s'%rowNamelist[i])

          sig_survive = [0.] * len(test_sel[k])
          sig_total = [0.] * len(test_sel[k])
          for j in range(len(test_sel[k])): # write each column (hist)
            if i==0:
              f.write('%-23s'%colNamelist[k][j])
            else:
              this_hist    = this_file.Get(test_sel[k][j][0])
              #print this_hist
              this_Nbin    = this_hist.GetNbinsX()
              this_LowEdge = this_hist.GetXaxis().GetBinLowEdge(1)
              this_width   = this_hist.GetXaxis().GetBinWidth(1)
              this_cutbin = int(((test_sel[k][j][2] - this_LowEdge)/this_width)+1)
              #print this_cutbin,this_Nbin
              this_total = this_hist.Integral() + this_hist.GetBinContent(0) + this_hist.GetBinContent(this_Nbin+1) # should include under/overflow contents
              if this_total <= 0.:
                print "[WARNING] In file",this_file
                print "[WARNING] and hist",this_hist
                print "[WARNING] Negative/Zero events :",this_total
                print "[WARNING] Converting to 0.001 ..."
                this_total = 0.001
              if test_sel[k][j][1] == "gt":
                this_survive = this_hist.Integral(this_cutbin,this_Nbin+1)
              elif test_sel[k][j][1] == "lt":
                this_survive = this_hist.Integral(0,this_cutbin-1)
              #print this_survive
              #print this_total
              this_eff = (this_survive / this_total) * 100.

              if not("DY" in rowNamelist[i] or "VBF" in rowNamelist[i] or "SSWW" in rowNamelist[i]): # bkg loop
                bkg_survive[j] += this_survive
                bkg_total[j] += this_total
              else: # signal
                sig_survive[j] += this_survive
                sig_total[j] += this_total

              #f.write('%-20s (%s'%(format(this_survive,'.3e'),format(this_eff,'.3e'))+'%)')
              f.write('%-8s'%format(this_survive,'.3f') + '(%-4s'%format(this_eff,'.1f') + '%)        ')

          if rowNamelist[i]=="prompt": # after filling the prompt
            f.write('\n')
            f.write('%-14s'%"bkg")
            for j in range(len(test_sel[k])): # write each column (hist)
              this_bkg_eff = (bkg_survive[j] / bkg_total[j]) * 100.
              f.write('%-8s'%format(bkg_survive[j],'.3f') + '(%-4s'%format(this_bkg_eff,'.1f') + '%)        ')

          #print bkg_survive
          #print sig_survive
          #print bkg_total
          #print sig_total
          if "DY" in rowNamelist[i] or "VBF" in rowNamelist[i] or "SSWW" in rowNamelist[i]: # after filling each signal
            f.write('\n')
            f.write('%-14s'%"FOM")
            for j in range(len(test_sel[k])): # write each column (hist)
              if "SSWW" in rowNamelist[i]:
                this_FOM = sqrt(2.*((sqrt(sig_survive[j])+bkg_survive[j])*log(1+(sqrt(sig_survive[j])/(bkg_survive[j])))-sqrt(sig_survive[j])))
              else:
                this_FOM = sqrt(2.*((sig_survive[j]+bkg_survive[j])*log(1+(sig_survive[j]/(bkg_survive[j])))-sig_survive[j]))
              f.write('%-23s'%format(this_FOM,'.2f'))

          f.write('\n')
        f.write('='*150+'\n')

else:
  era = "2017"
  ID = "HNL_ULID"
  card_name = "M1700_EE"
  signal_name = "signalDYVBF" #"signalSSWW"
  
  pathlist = []
  taglist = []
  #pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter_PR43/LimitInputs/240501_1704_"+ID+"/"+era+"/"+card_name+"_card_input.root")
  #taglist.append("PR43")
  pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter_PR44/LimitInputs/240504_PR44_"+ID+"/"+era+"/"+card_name+"_card_input.root")
  taglist.append("PR44")
  pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter/LimitInputs/240505_PR46_HNTightV2"+"/"+era+"/"+card_name+"_card_input.root")
  taglist.append("PR45 HNTightV2")
  pathlist.append("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegion_Plotter_PR45_HNL_ULID/LimitInputs/240505_PR45_"+ID+"/"+era+"/"+card_name+"_card_input.root")
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
      this_conv.append(this_file.Get("conv_inc").GetBinContent(j+1))
      this_prompt.append(this_file.Get("prompt_inc").GetBinContent(j+1))
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
      f.write('============='+'='*3*30+'\n')
      for c in range(len(names[0])):
        f.write('%-10s'%names[i][c])
        for j in range(len(taglist)):
          #print contents[i][c][j]
          if type(contents[i][c][j]) == str:
            f.write('%-30s'%contents[i][c][j])
          else:
            f.write('%-30s'%format(contents[i][c][j],'.2e'))
        f.write('\n')
   
