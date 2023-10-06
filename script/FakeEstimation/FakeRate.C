void GetFR2D(TFile *outfile, TString year, TString tag, TString channel, TString ID){ // tag : LF, HF (when to use flavor-dep fake rate method), or "" for No flavor separation, or TriLep, ...
  TString fake_data = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/DATA/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_Muon.root";
  TString fake_W    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_WJets_MG.root";
  TString fake_Z    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_DYJets.root";
  TString fake_TT   = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_TTLJ_powheg.root";
  //TString fake_W    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_WJets_MG.root";
  //TString fake_Z    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_DYJets.root";
  //TString fake_TT   = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_TTLJ_powheg.root";
  //TString fake_QCD  = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_FakeEventSkimBDT_QCD_MuEnriched.root";
  
  TFile *f_fake_data = new TFile(fake_data);
  TFile *f_fake_W    = new TFile(fake_W);
  TFile *f_fake_Z    = new TFile(fake_Z);
  TFile *f_fake_TT   = new TFile(fake_TT);
  //TFile *f_fake_QCD  = new TFile(fake_QCD);

  TString nametag = (tag!="") ? tag+"_" : "";

  TString histname_loose = nametag+"Fake_Loose_"+channel+"_"+ID+"_LFvsHF_40_pt_eta";
  TString histname_tight = nametag+"Fake_Tight_"+channel+"_"+ID+"_LFvsHF_40_pt_eta";

  TH2D *h_loose_data = (TH2D*)f_fake_data->Get(histname_loose);
  TH2D *h_loose_W    = (TH2D*)f_fake_W   ->Get(histname_loose);
  TH2D *h_loose_Z    = (TH2D*)f_fake_Z   ->Get(histname_loose);
  TH2D *h_loose_TT   = (TH2D*)f_fake_TT  ->Get(histname_loose);
  TH2D *h_tight_data = (TH2D*)f_fake_data->Get(histname_tight);
  TH2D *h_tight_W    = (TH2D*)f_fake_W   ->Get(histname_tight);
  TH2D *h_tight_Z    = (TH2D*)f_fake_Z   ->Get(histname_tight);
  TH2D *h_tight_TT   = (TH2D*)f_fake_TT  ->Get(histname_tight);
  //TH2D *h_loose_QCD_2D_Uncorr = (TH2D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr_eta");
  //TH2D *h_tight_QCD_2D_Uncorr = (TH2D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr_eta");
  //TH2D *h_loose_QCD_2D_Corr   = (TH2D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD_eta");
  //TH2D *h_tight_QCD_2D_Corr   = (TH2D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD_eta");
  //TH1D *h_loose_QCD_1D_Uncorr = (TH1D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr");
  //TH1D *h_tight_QCD_1D_Uncorr = (TH1D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr");
  //TH1D *h_loose_QCD_1D_Corr   = (TH1D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD");
  //TH1D *h_tight_QCD_1D_Corr   = (TH1D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD");

  h_loose_data->Add(h_loose_W,-1.);
  h_loose_data->Add(h_loose_Z,-1.);
  h_loose_data->Add(h_loose_TT,-1.);
  h_tight_data->Add(h_tight_W,-1.);
  h_tight_data->Add(h_tight_Z,-1.);
  h_tight_data->Add(h_tight_TT,-1.);

  h_tight_data->Divide(h_loose_data);
  h_tight_data->SetName(ID+"_"+nametag+"pt_eta_AwayJetPt40");

  //h_tight_QCD_2D_Uncorr->Divide(h_loose_QCD_2D_Uncorr);
  //h_tight_QCD_2D_Uncorr->SetName("HNL_ULID_2017_PtPartonUncorr_eta_TriLepQCD");
  //h_tight_QCD_2D_Corr  ->Divide(h_loose_QCD_2D_Corr);
  //h_tight_QCD_2D_Corr  ->SetName("HNL_ULID_2017_PtPartonQCD_eta_TriLepQCD");
  //h_tight_QCD_1D_Uncorr->Divide(h_loose_QCD_1D_Uncorr);
  //h_tight_QCD_1D_Uncorr->SetName("HNL_ULID_2017_PtPartonUncorr_TriLepQCD");
  //h_tight_QCD_1D_Corr  ->Divide(h_loose_QCD_1D_Corr);
  //h_tight_QCD_1D_Corr  ->SetName("HNL_ULID_2017_PtPartonQCD_TriLepQCD");

  outfile->cd();
  h_tight_data->Write();
  //h_tight_QCD_2D_Uncorr->Write();
  //h_tight_QCD_2D_Corr->Write();
  //h_tight_QCD_1D_Uncorr->Write();
  //h_tight_QCD_1D_Corr->Write();
}

void GetFR1D(TFile *outfile, TString year, TString tag, TString channel, TString ID, TString var){ // tag : LF, HF (when using flavor-dep fake rate method), or TriLep
  TString fake_data = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/DATA/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_Muon.root";
  TString fake_W    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_WJets_MG.root";
  TString fake_Z    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_DYJets.root";
  TString fake_TT   = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_TTLJ_powheg.root";
  
  TFile *f_fake_data = new TFile(fake_data);
  TFile *f_fake_W    = new TFile(fake_W);
  TFile *f_fake_Z    = new TFile(fake_Z);
  TFile *f_fake_TT   = new TFile(fake_TT);
 
  TString nametag = (tag!="") ? tag+"_" : "";

  TString histname_loose = nametag+"Fake_Loose_"+channel+"_"+ID+"_LFvsHF_40_"+var;
  TString histname_tight = nametag+"Fake_Tight_"+channel+"_"+ID+"_LFvsHF_40_"+var;

  //TString histname_loose = "Fake_Loose_"+channel+"_"+ID+"_LFvsHF_40_"+var;
  //TString histname_tight = "Fake_Tight_"+channel+"_"+ID+"_LFvsHF_40_"+var;
  //if(tag=="LF"||tag=="HF"){
  //  histname_loose = tag+"_"+histname_loose;
  //  histname_tight = tag+"_"+histname_tight;
  //}

  TH1D *h_loose_data_1D = (TH1D*)f_fake_data->Get(histname_loose);
  TH1D *h_loose_W_1D    = (TH1D*)f_fake_W   ->Get(histname_loose);
  TH1D *h_loose_Z_1D    = (TH1D*)f_fake_Z   ->Get(histname_loose);
  TH1D *h_loose_TT_1D   = (TH1D*)f_fake_TT  ->Get(histname_loose);
  TH1D *h_tight_data_1D = (TH1D*)f_fake_data->Get(histname_tight);
  TH1D *h_tight_W_1D    = (TH1D*)f_fake_W   ->Get(histname_tight);
  TH1D *h_tight_Z_1D    = (TH1D*)f_fake_Z   ->Get(histname_tight);
  TH1D *h_tight_TT_1D   = (TH1D*)f_fake_TT  ->Get(histname_tight);

  // Show the stacked histo
  TCanvas *c1 = new TCanvas(nametag+"loose_"+ID+"_"+var,"",800,800);
  TCanvas *c2 = new TCanvas(nametag+"tight_"+ID+"_"+var,"",800,800);

  // Loose
  c1->cd();

  h_loose_data_1D->SetMarkerStyle(20);
  h_loose_data_1D->SetMarkerColor(kBlack);

  TH1D *h_loose_MC_1D = (TH1D*)h_loose_W_1D->Clone();
  h_loose_MC_1D->Add(h_loose_Z_1D);
  h_loose_MC_1D->Add(h_loose_TT_1D);
  h_loose_MC_1D->SetMarkerSize(0);
  h_loose_MC_1D->SetLineWidth(0);
  h_loose_MC_1D->SetFillStyle(3144);
  h_loose_MC_1D->SetFillColor(kBlack);

  THStack *hs_loose = new THStack(tag+"_hs_loose", "");
  h_loose_TT_1D->SetLineWidth(0);
  h_loose_TT_1D->SetFillColor(kRed);
  hs_loose->Add(h_loose_TT_1D);
  h_loose_Z_1D->SetLineWidth(0);
  h_loose_Z_1D->SetFillColor(kYellow);
  hs_loose->Add(h_loose_Z_1D);
  h_loose_W_1D->SetLineWidth(0);
  h_loose_W_1D->SetFillColor(kGreen);
  hs_loose->Add(h_loose_W_1D);

  TH1 *loose_sum_MC = (TH1*)hs_loose->GetStack()->Last(); // https://root-forum.cern.ch/t/thstack-and-getbincontent/18967
  double max_loose_data = h_loose_data_1D->GetBinContent(h_loose_data_1D->GetMaximumBin());
  double max_loose_MC   = loose_sum_MC->GetBinContent(loose_sum_MC->GetMaximumBin());
  double max_loose_tot  = std::max(max_loose_data, max_loose_MC);
  hs_loose->SetMaximum(max_loose_tot*1.1);

  hs_loose->Draw("hist");

  hs_loose->SetTitle(nametag+"Loose_"+ID);
  hs_loose->GetXaxis()->SetLabelOffset(0.001);
  hs_loose->GetXaxis()->SetLabelSize(0.04);
  hs_loose->GetXaxis()->SetTitleOffset(1);
  hs_loose->GetXaxis()->SetTitleSize(0.04);
  hs_loose->GetXaxis()->SetTitle(var);
  hs_loose->GetYaxis()->SetLabelSize(0.04);
  hs_loose->GetYaxis()->SetTitleOffset(1.25);
  hs_loose->GetYaxis()->SetTitle("Nevents");

  h_loose_MC_1D->Draw("e2 same");

  h_loose_data_1D->Draw("ep same");
  if(var=="pt") gPad->SetLogx();
  gPad->SetLogy();

  TLegend *lg_loose = new TLegend(0.6, 0.67, 0.8, 0.87);
  lg_loose->AddEntry(h_loose_data_1D,"Data","lpe"); // show line, polymarker, error bar
  lg_loose->AddEntry(h_loose_W_1D,"Wjets","f"); // show fill
  lg_loose->AddEntry(h_loose_Z_1D,"DY","f");
  lg_loose->AddEntry(h_loose_TT_1D,"t#bar{t}");
  lg_loose->SetBorderSize(0);
  lg_loose->Draw("same");

  // Tight
  c2->cd();

  h_tight_data_1D->SetMarkerStyle(20);
  h_tight_data_1D->SetMarkerColor(kBlack);

  TH1D *h_tight_MC_1D = (TH1D*)h_tight_W_1D->Clone();
  h_tight_MC_1D->Add(h_tight_Z_1D);
  h_tight_MC_1D->Add(h_tight_TT_1D);
  h_tight_MC_1D->SetMarkerSize(0);
  h_tight_MC_1D->SetLineWidth(0);
  h_tight_MC_1D->SetFillStyle(3144);
  h_tight_MC_1D->SetFillColor(kBlack);

  THStack *hs_tight = new THStack(tag+"_hs_tight", "");
  h_tight_TT_1D->SetLineWidth(0);
  h_tight_TT_1D->SetFillColor(kRed);
  hs_tight->Add(h_tight_TT_1D);
  h_tight_Z_1D->SetLineWidth(0);
  h_tight_Z_1D->SetFillColor(kYellow);
  hs_tight->Add(h_tight_Z_1D);
  h_tight_W_1D->SetLineWidth(0);
  h_tight_W_1D->SetFillColor(kGreen);
  hs_tight->Add(h_tight_W_1D);

  TH1 *tight_sum_MC = (TH1*)hs_tight->GetStack()->Last(); // https://root-forum.cern.ch/t/thstack-and-getbincontent/18967
  double max_tight_data = h_tight_data_1D->GetBinContent(h_tight_data_1D->GetMaximumBin());
  double max_tight_MC   = tight_sum_MC->GetBinContent(tight_sum_MC->GetMaximumBin());
  double max_tight_tot  = std::max(max_tight_data, max_tight_MC);
  hs_tight->SetMaximum(max_tight_tot*1.1);

  hs_tight->Draw("hist");

  hs_tight->SetTitle(nametag+"Tight_"+ID);
  hs_tight->GetXaxis()->SetLabelOffset(0.001);
  hs_tight->GetXaxis()->SetLabelSize(0.04);
  hs_tight->GetXaxis()->SetTitleOffset(1);
  hs_tight->GetXaxis()->SetTitleSize(0.04);
  hs_tight->GetXaxis()->SetTitle(var);
  hs_tight->GetYaxis()->SetLabelSize(0.04);
  hs_tight->GetYaxis()->SetTitleOffset(1.25);
  hs_tight->GetYaxis()->SetTitle("Nevents");

  h_tight_MC_1D->Draw("e2 same");

  h_tight_data_1D->Draw("ep same");
  if(var=="pt") gPad->SetLogx();
  gPad->SetLogy();

  TLegend *lg_tight = new TLegend(0.6, 0.67, 0.8, 0.87);
  lg_tight->AddEntry(h_tight_data_1D,"Data","lpe"); // show line, polymarker, error bar
  lg_tight->AddEntry(h_tight_W_1D,"Wjets","f"); // show fill
  lg_tight->AddEntry(h_tight_Z_1D,"DY","f");
  lg_tight->AddEntry(h_tight_TT_1D,"t#bar{t}");
  lg_tight->SetBorderSize(0);
  lg_tight->Draw("same");

  // Now get the FR after prompt subtraction
  TH1D *h_loose_data_sub_1D = (TH1D*)h_loose_data_1D->Clone();
  TH1D *h_tight_data_sub_1D = (TH1D*)h_tight_data_1D->Clone();
  h_loose_data_sub_1D->Add(h_loose_W_1D,-1.);
  h_loose_data_sub_1D->Add(h_loose_Z_1D,-1.);
  h_loose_data_sub_1D->Add(h_loose_TT_1D,-1.);
  h_tight_data_sub_1D->Add(h_tight_W_1D,-1.);
  h_tight_data_sub_1D->Add(h_tight_Z_1D,-1.);
  h_tight_data_sub_1D->Add(h_tight_TT_1D,-1.);
  //for(int i=1; i<=h_loose_data_1D->GetNbinsX(); i++){
  //  cout << i << "th bin loose : " << h_loose_data_sub_1D->GetBinContent(i) << endl;
  //  cout << i << "th bin tight : " << h_tight_data_sub_1D->GetBinContent(i) << endl;
  //}

  TH1D *h_FR_1D = (TH1D*)h_tight_data_sub_1D->Clone();
  h_FR_1D->Divide(h_loose_data_sub_1D);
  h_FR_1D->SetName(ID+"_"+nametag+var+"_AwayJetPt40");
  outfile->cd();
  c1->Write();
  c2->Write();
  h_FR_1D->Write();

}

void GetMCTruthFRratio(TString sample, TString year, TString channel, TString ID, TString var, TFile* outfile){ // W, Z, TT

  TString sample_full;
  if(sample=="W") sample_full = "WJets_MG";
  else if(sample=="Z") sample_full = "DYJets";
  else if(sample=="TT") sample_full = "TTLJ_powheg";
  TString fake_MC = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_"+sample_full+".root";
  TFile *f_fake_MC = new TFile(fake_MC);

  TH1D *h_loose_fakeHF = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeHF_"+var);
  TH1D *h_tight_fakeHF = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeHF_"+var);
  TH1D *h_loose_fakeLF = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeLF_"+var);
  TH1D *h_tight_fakeLF = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeLF_"+var);

  h_tight_fakeHF->Divide(h_tight_fakeHF,h_loose_fakeHF,1,1,"B"); // unweighted, Binomial error
  h_tight_fakeLF->Divide(h_tight_fakeLF,h_loose_fakeLF,1,1,"B");

  TCanvas *c1  = new TCanvas("FakeRateRatioHoverL_"+sample+"_"+channel+"_"+year+"_"+ID+"_"+var,"",800,800);

  c1->cd();

  c1->SetRightMargin(0.05);
  c1->SetLeftMargin(0.14);

  TH1D *h_FRratio = (TH1D*)h_tight_fakeHF->Clone();
  h_FRratio->Divide(h_tight_fakeLF);
  h_FRratio->SetName("FakeRateRatioHoverL_"+sample+"_"+channel+"_"+year+"_"+ID+"_"+var);
  TString var_latex;
  if(var=="PtPartonUncorr") var_latex = "p_{T}^{parton, uncorr}";
  else if(var=="PtPartonQCD") var_latex = "p_{T}^{parton, corr}";
  h_FRratio->GetXaxis()->SetTitle(var_latex+" (GeV)");
  h_FRratio->GetXaxis()->SetLabelOffset(0);
  h_FRratio->GetYaxis()->SetTitle("#frac{FR_{HF}}{FR_{LF}}");
  h_FRratio->SetLineColor(kRed);
  h_FRratio->SetStats(0);
  h_FRratio->Draw();
  gPad->SetLogx();

  TLegend *lg_FRratio = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg_FRratio->AddEntry(h_FRratio,ID,"lpe"); // show line, polymarker, error bar
  lg_FRratio->SetBorderSize(0);
  lg_FRratio->Draw("same");

  TText *t = new TText(.95,.91,sample_full+" ("+year+")");
  t->SetNDC(); // set this or the text will show at x, y w.r.t x, y-axis of the histogram (not pad)
  t->SetTextSize(0.04);
  t->SetTextAlign(31);
  t->Draw();

  // calculate deviation from 1
  double dev(0.);
  for(unsigned int i=1; i<=10; i++){
    dev+=pow(h_FRratio->GetBinContent(i)-1.,2);
  }
  dev = sqrt(dev);
  TString dev_t = Form("%.3f",dev);

  TText *t2 = new TText(.94,.71,"dev from 1 : "+dev_t);
  t2->SetNDC(); // set this or the text will show at x, y w.r.t x, y-axis of the histogram (not pad)
  t2->SetTextSize(0.04);
  t2->SetTextFont(42);
  t2->SetTextAlign(31);
  t2->Draw();

  // create root file
  outfile->cd();
  c1->Write();

}

void GetMCTruthFR(TFile *outfile, TString sample, TString year, TString tag, TString channel, TString ID, TString leptype, TString var){ // W, Z, TT

  TString sample_full;
  if(sample=="W") sample_full = "WJets_MG";
  else if(sample=="Z") sample_full = "DYJets";
  else if(sample=="TT") sample_full = "TTLJ_powheg";
  else if(sample=="QCD") sample_full = "QCD_MuEnriched";
  TString fake_MC = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_FakeEventSkimBDT_"+sample_full+".root";
  TFile *f_fake_MC = new TFile(fake_MC);

  TString histname_loose = "Fake_Loose_"+channel+"_"+ID+"_LFvsHF_NoSel_"+leptype+"_"+var;
  TString histname_tight = "Fake_Tight_"+channel+"_"+ID+"_LFvsHF_NoSel_"+leptype+"_"+var;
  if(tag=="LF"||tag=="HF"){
    histname_loose = tag+"_"+histname_loose;
    histname_tight = tag+"_"+histname_tight;
  }

  TH1D *h_loose = (TH1D*)f_fake_MC->Get(histname_loose);
  TH1D *h_tight = (TH1D*)f_fake_MC->Get(histname_tight);

  h_tight->Divide(h_tight,h_loose,1,1,"B"); // unweighted, Binomial error

  TString histname_FR = sample+"_"+histname_loose.ReplaceAll("Loose_","");
  TCanvas *c1  = new TCanvas(histname_FR,"",900,800);

  c1->cd();

  c1->SetRightMargin(0.05);
  c1->SetLeftMargin(0.14);

  h_tight->SetName(histname_FR);
  std::map<TString, TString> var_latex;
  var_latex["PtPartonUncorr"] = "p_{T}^{parton, uncorr} (GeV)";
  var_latex["PtPartonQCD"]    = "p_{T}^{parton, corr} (GeV)";
  var_latex["pt"]             = "p_{T} (GeV)";
  var_latex["eta"]            = "#eta";
  var_latex["etaFine"]        = "#eta";
  var_latex["dXY"]            = "d_{xy} (cm)";
  var_latex["dZ"]             = "d_{z} (cm)";
  var_latex["SIP3D"]          = "SIP_{3D}";
  var_latex["mva"]            = "MVA_{HFvsPr}";
  var_latex["LFvsHF"]         = "MVA_{LFvsHF}";
  var_latex["reliso"]         = "PFRelIso";
  h_tight->GetXaxis()->SetTitleOffset(1.2);
  h_tight->GetXaxis()->SetTitle(var_latex[var]);
  h_tight->GetXaxis()->SetLabelOffset(0);
  h_tight->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight->SetLineColor(kRed);
  h_tight->SetStats(0);
  h_tight->Draw();
  if(var.Contains("pt")) gPad->SetLogx();

  TLegend *lg = new TLegend(0.6, 0.77, 0.95, 0.88);
  lg->AddEntry(h_tight,"#bf{"+tag+"} "+ID+" ("+leptype+")","lpe"); // show line, polymarker, error bar
  lg->SetBorderSize(0);
  lg->Draw("same");

  TText *t = new TText(.95,.91,sample_full+" ("+year+")");
  t->SetNDC(); // set this or the text will show at x, y w.r.t x, y-axis of the histogram (not pad)
  t->SetTextSize(0.04);
  t->SetTextAlign(31);
  t->Draw();

  // create root file
  outfile->cd();
  c1->Write();

  // store png
  gSystem->Exec("mkdir -p FakeRateTruth/"+channel+"/"+ID+"/"+sample+"/"+var);
  if(tag!="") c1->SaveAs("FakeRateTruth/"+channel+"/"+ID+"/"+sample+"/"+var+"/"+sample+"_"+tag+"_"+ID+"_"+leptype+"_"+var+".png");
  else        c1->SaveAs("FakeRateTruth/"+channel+"/"+ID+"/"+sample+"/"+var+"/"+sample+"_"+"NoSep_"+ID+"_"+leptype+"_"+var+".png");

}

void GetPtPartonSF(TString year, TString channel){ //FIXME add lepton channel later

  TString fake_MC = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_FakeEventSkimBDT_QCD_MuEnriched.root";
  TFile *f_fake_MC = new TFile(fake_MC);

  vector<TString> IDs = {
                         "MVALoose", "TriLep",
                         "Blt0p01", "Blt0p015", "Blt0p02", "Blt0p025", "Blt0p03", "Blt0p035", "Blt0p04", "Blt0p045", "Blt0p05", "Blt0p055", "Blt0p1", "Blt0p2",
                         "Pgt0p45", "Plt0p45",
                         "TagHF", "TagLF",
                         "Blt0p01_Pgt0p45", "Blt0p01_Plt0p45",
                         "Blt0p01_TagHF", "Blt0p01_TagLF",
                         "Blt0p025_Pgt0p45", "Blt0p025_Plt0p45",
                         "Blt0p025_TagHF", "Blt0p025_TagLF",
                         "Blt0p025_Pgt0p45_TagHF", "Blt0p025_Pgt0p45_TagLF",
                        };

  for(auto ID:IDs){

    TProfile *h_uncorr = (TProfile*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_40_MVA_PtPartonUncorr");
    double bor1 = (h_uncorr->GetBinContent(163) + h_uncorr->GetBinContent(164))/2.;
    double bor2 = (h_uncorr->GetBinContent(165) + h_uncorr->GetBinContent(166))/2.;
    double SF = bor2/bor1;
    TString SF_t = Form("%.3f",SF);
    cout << "PtPartonSFmap[\"HNL_ULID_2017_"+ID+"\"] = "+SF_t+";" << endl;

  }

}

void FakeRate(){
  //TFile *outfileQCDFakeRate = new TFile("FakeRate_Mu_2017_QCD.root","RECREATE");
  //GetFR2D(outfileQCDFakeRate);

  // GetFR2D
  TFile *outfile2D = new TFile("FakeRate_Mu_2017_LFvsHF_2D.root","RECREATE");
  GetFR2D(outfile2D,"2017","LF","MuMu","HNL_ULID_2017");
  GetFR2D(outfile2D,"2017","HF","MuMu","HNL_ULID_2017");
  GetFR2D(outfile2D,"2017",""  ,"MuMu","HNL_ULID_2017");
  GetFR2D(outfile2D,"2017","LF","MuMu","HNTightV2");
  GetFR2D(outfile2D,"2017","HF","MuMu","HNTightV2");
  GetFR2D(outfile2D,"2017",""  ,"MuMu","HNTightV2");

  // GetFR1D
  TFile *outfile1D = new TFile("FakeRate_Mu_2017_LFvsHF_1D_test.root","RECREATE");
  GetFR1D(outfile1D,"2017","LF","MuMu","HNL_ULID_2017","pt");
  GetFR1D(outfile1D,"2017","HF","MuMu","HNL_ULID_2017","pt");
  GetFR1D(outfile1D,"2017",""  ,"MuMu","HNL_ULID_2017","pt");
  GetFR1D(outfile1D,"2017","LF","MuMu","HNTightV2","pt");
  GetFR1D(outfile1D,"2017","HF","MuMu","HNTightV2","pt");
  GetFR1D(outfile1D,"2017",""  ,"MuMu","HNTightV2","pt");

  // GetMCTruthFR
  //TFile *outfile1D = new TFile("FakeRateTruth_Mu_2017_LFvsHF_1D.root","RECREATE");
  //vector<TString> samples  = {"QCD", "W", "Z", "TT"};
  //vector<TString> tags     = {"", "HF", "LF"};
  ////vector<TString> eras     = {"2016preVFP", "2016postVFP", "2017", "2018"};
  ////vector<TString> flavors  = {"MuMu", "EE"};
  //vector<TString> IDs      = {"HNL_ULID_2017", "HNTightV2"};
  //vector<TString> LepTypes = {"fake", "fakeHF", "fakeLF"};
  //vector<TString> vars     = {"pt", "etaFine", "reliso", "dXY", "dZ", "SIP3D", "mva", "LFvsHF"};
  //for(unsigned int i=0; i<samples.size(); i++){
  //  for(unsigned int j=0; j<tags.size(); j++){
  //    for(unsigned int k=0; k<IDs.size(); k++){
  //      for(unsigned int l=0; l<LepTypes.size(); l++){
  //        for(unsigned int m=0; m<vars.size(); m++){
  //          GetMCTruthFR(outfile1D, samples.at(i), "2017", tags.at(j), "MuMu", IDs.at(k), LepTypes.at(l), vars.at(m));
  //        }
  //      }
  //    }
  //  }
  //}

  //GetMCTruthFRratio("TT","2017","MuMu","TriLep"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","TriLep"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","MVALoose","PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","MVALoose","PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01"   ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01"   ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p015"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p015"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p02"   ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p02"   ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p025"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p025"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p03"   ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p03"   ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p035"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p035"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Pgt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Pgt0p45"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Plt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Plt0p45"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Pgt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Pgt0p45"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Plt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Plt0p45"  ,"PtPartonQCD"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","TagLF"  ,"PtPartonUncorr"   ,outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","TagHF"  ,"PtPartonUncorr"   ,outfile);
  //GetPtPartonSF("2017","MuMu");
}
