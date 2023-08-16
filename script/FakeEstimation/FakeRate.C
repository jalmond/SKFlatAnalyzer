void GetFR2D(TFile *outfile){
  TString fake_data = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/DATA/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_FakeRate.root";
  TString fake_W    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_WJets_MG.root";
  TString fake_Z    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_DYJets.root";
  TString fake_TT   = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_TTLJ_powheg.root";
  TString fake_QCD  = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_FakeEventSkimBDT_QCD_MuEnriched.root";
  
  //TFile *f_fake_data = new TFile(fake_data);
  TFile *f_fake_W    = new TFile(fake_W);
  TFile *f_fake_Z    = new TFile(fake_Z);
  TFile *f_fake_TT   = new TFile(fake_TT);
  TFile *f_fake_QCD  = new TFile(fake_QCD);
  
  //TH2D *h_loose_data = (TH2D*)f_fake_data->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  //TH2D *h_loose_W    = (TH2D*)f_fake_W   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  //TH2D *h_loose_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  //TH2D *h_loose_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  //TH2D *h_tight_data = (TH2D*)f_fake_data->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  //TH2D *h_tight_W    = (TH2D*)f_fake_W   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  //TH2D *h_tight_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  //TH2D *h_tight_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_loose_QCD_2D_Uncorr = (TH2D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr_eta");
  TH2D *h_tight_QCD_2D_Uncorr = (TH2D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr_eta");
  TH2D *h_loose_QCD_2D_Corr   = (TH2D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD_eta");
  TH2D *h_tight_QCD_2D_Corr   = (TH2D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD_eta");
  TH1D *h_loose_QCD_1D_Uncorr = (TH1D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr");
  TH1D *h_tight_QCD_1D_Uncorr = (TH1D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonUncorr");
  TH1D *h_loose_QCD_1D_Corr   = (TH1D*)f_fake_QCD->Get("Fake_Loose_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD");
  TH1D *h_tight_QCD_1D_Corr   = (TH1D*)f_fake_QCD->Get("Fake_Tight_MuMu_HNL_ULID_2017_TriLep_40_PtPartonQCD");

  //h_loose_data->Add(h_loose_W,-1.);
  //h_loose_data->Add(h_loose_Z,-1.);
  //h_loose_data->Add(h_loose_TT,-1.);
  //h_tight_data->Add(h_tight_W,-1.);
  //h_tight_data->Add(h_tight_Z,-1.);
  //h_tight_data->Add(h_tight_TT,-1.);
  //h_tight_data->Divide(h_loose_data);
  //h_tight_data->SetName("Fake_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");

  h_tight_QCD_2D_Uncorr->Divide(h_loose_QCD_2D_Uncorr);
  h_tight_QCD_2D_Uncorr->SetName("HNL_ULID_2017_PtPartonUncorr_eta_TriLepQCD");
  h_tight_QCD_2D_Corr  ->Divide(h_loose_QCD_2D_Corr);
  h_tight_QCD_2D_Corr  ->SetName("HNL_ULID_2017_PtPartonQCD_eta_TriLepQCD");
  h_tight_QCD_1D_Uncorr->Divide(h_loose_QCD_1D_Uncorr);
  h_tight_QCD_1D_Uncorr->SetName("HNL_ULID_2017_PtPartonUncorr_TriLepQCD");
  h_tight_QCD_1D_Corr  ->Divide(h_loose_QCD_1D_Corr);
  h_tight_QCD_1D_Corr  ->SetName("HNL_ULID_2017_PtPartonQCD_TriLepQCD");

  outfile->cd();
  //h_tight_data->Write();
  h_tight_QCD_2D_Uncorr->Write();
  h_tight_QCD_2D_Corr->Write();
  h_tight_QCD_1D_Uncorr->Write();
  h_tight_QCD_1D_Corr->Write();
}

void GetFR1D(TFile *outfile){
  TString fake_data = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/DATA/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_FakeRate.root";
  TString fake_W    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_WJets_MG.root";
  TString fake_Z    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_DYJets.root";
  TString fake_TT   = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_TTLJ_powheg.root";
  
  TFile *f_fake_data = new TFile(fake_data);
  TFile *f_fake_W    = new TFile(fake_W);
  TFile *f_fake_Z    = new TFile(fake_Z);
  TFile *f_fake_TT   = new TFile(fake_TT);
  
  TH2D *h_loose_data = (TH2D*)f_fake_data->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_loose_W    = (TH2D*)f_fake_W   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_loose_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_loose_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_tight_data = (TH2D*)f_fake_data->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_tight_W    = (TH2D*)f_fake_W   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_tight_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");
  TH2D *h_tight_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr_eta");

  TH1D *h_loose_data_1D = h_loose_data->ProjectionX("Fake_Loose_Data_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");
  TH1D *h_loose_W_1D    = h_loose_W   ->ProjectionX("Fake_Loose_W_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");
  TH1D *h_loose_Z_1D    = h_loose_Z   ->ProjectionX("Fake_Loose_Z_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");
  TH1D *h_loose_TT_1D   = h_loose_TT  ->ProjectionX("Fake_Loose_TT_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");
  TH1D *h_tight_data_1D = h_tight_data->ProjectionX("Fake_Tight_Data_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");
  TH1D *h_tight_W_1D    = h_tight_W   ->ProjectionX("Fake_Tight_W_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");
  TH1D *h_tight_Z_1D    = h_tight_Z   ->ProjectionX("Fake_Tight_Z_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");
  TH1D *h_tight_TT_1D   = h_tight_TT  ->ProjectionX("Fake_Tight_TT_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr",0,-1,"e");

  // Show the stacked histo
  TCanvas *c1 = new TCanvas("loose_PtPartonUncorr","",800,800);
  TCanvas *c2 = new TCanvas("tight_PtPartonUncorr","",800,800);

  // Loose PtPartonUncorr
  c1->cd();

  h_loose_data_1D->SetMarkerStyle(20);
  h_loose_data_1D->SetMarkerColor(kBlack);

  THStack *hs_loose = new THStack("hs_loose", "");
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
  h_loose_data_1D->Draw("same");
  gPad->SetLogx();
  gPad->SetLogy();

  TLegend *lg_loose = new TLegend(0.6, 0.67, 0.8, 0.87);
  lg_loose->AddEntry(h_loose_data_1D,"Data","lpe"); // show line, polymarker, error bar
  lg_loose->AddEntry(h_loose_W_1D,"Wjets","f"); // show fill
  lg_loose->AddEntry(h_loose_Z_1D,"DY","f");
  lg_loose->AddEntry(h_loose_TT_1D,"t#bar{t}");
  lg_loose->SetBorderSize(0);
  lg_loose->Draw("same");

  // Tight PtPartonUncorr
  c2->cd();

  h_tight_data_1D->SetMarkerStyle(20);
  h_tight_data_1D->SetMarkerColor(kBlack);

  THStack *hs_tight = new THStack("hs_tight", "");
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
  h_tight_data_1D->Draw("same");
  gPad->SetLogx();
  gPad->SetLogy();

  TLegend *lg_tight = new TLegend(0.6, 0.67, 0.8, 0.87);
  lg_tight->AddEntry(h_tight_data_1D,"Data","lpe"); // show line, polymarker, error bar
  lg_tight->AddEntry(h_tight_W_1D,"Wjets","f"); // show fill
  lg_tight->AddEntry(h_tight_Z_1D,"DY","f");
  lg_tight->AddEntry(h_tight_TT_1D,"t#bar{t}");
  lg_tight->SetBorderSize(0);
  lg_tight->Draw("same");

  // Now get the FR after prompt subtraction
  h_loose_data_1D->Add(h_loose_W_1D,-1.);
  h_loose_data_1D->Add(h_loose_Z_1D,-1.);
  h_loose_data_1D->Add(h_loose_TT_1D,-1.);
  h_tight_data_1D->Add(h_tight_W_1D,-1.);
  h_tight_data_1D->Add(h_tight_Z_1D,-1.);
  h_tight_data_1D->Add(h_tight_TT_1D,-1.);
  TH1D *h_FR_1D = (TH1D*)h_tight_data_1D->Clone();
  h_FR_1D->Divide(h_loose_data_1D);
  h_FR_1D->SetName("Fake_MuMu_HNL_ULID_2017_V1_40_PtPartonUncorr");
  outfile->cd();
  h_FR_1D->Write();
  c1->Write();
  c2->Write();

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

void GetMCTruthFR(TString sample, TString year, TString channel, TString ID, TString leptype, TString var, TFile* outfile){ // W, Z, TT

  TString sample_full;
  if(sample=="W") sample_full = "WJets_MG";
  else if(sample=="Z") sample_full = "DYJets";
  else if(sample=="TT") sample_full = "TTLJ_powheg";
  TString fake_MC = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/FakeRateTruth__/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_"+sample_full+".root";
  TFile *f_fake_MC = new TFile(fake_MC);

  TH1D *h_loose = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_"+leptype+"_"+var);
  TH1D *h_tight = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_"+leptype+"_"+var);

  h_tight->Divide(h_tight,h_loose,1,1,"B"); // unweighted, Binomial error

  TCanvas *c1  = new TCanvas("FakeRate_"+sample+"_"+channel+"_"+year+"_"+ID+"_"+leptype+"_"+var,"",800,800);

  c1->cd();

  c1->SetRightMargin(0.05);
  c1->SetLeftMargin(0.14);

  h_tight->SetName("FakeRate_"+sample+"_"+channel+"_"+year+"_"+ID+"_"+leptype+"_"+var);
  TString var_latex;
  if(var=="PtPartonUncorr") var_latex = "p_{T}^{parton, uncorr}";
  else if(var=="PtPartonQCD") var_latex = "p_{T}^{parton, corr}";
  h_tight->GetXaxis()->SetTitle(var_latex+" (GeV)");
  h_tight->GetXaxis()->SetLabelOffset(0);
  h_tight->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight->SetLineColor(kRed);
  h_tight->SetStats(0);
  h_tight->Draw();
  gPad->SetLogx();

  TLegend *lg = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg->AddEntry(h_tight,ID+" ("+leptype+")","lpe"); // show line, polymarker, error bar
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
  //GetFR1D(outfileQCDFakeRate);
  TFile *outfile = new TFile("FakeRate_Mu_2017_IDopt.root","RECREATE");
  GetMCTruthFRratio("TT","2017","MuMu","TriLep"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","TriLep"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","MVALoose","PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","MVALoose","PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p01"   ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01"   ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p015"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p015"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p02"   ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p02"   ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p025"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p025"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p03"   ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p03"   ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p035"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p035"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Pgt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Pgt0p45"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Plt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Plt0p45"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Pgt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Pgt0p45"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Plt0p45"  ,"PtPartonUncorr",outfile);
  //GetMCTruthFRratio("TT","2017","MuMu","Blt0p01_Plt0p45"  ,"PtPartonQCD"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","TagLF"  ,"PtPartonUncorr"   ,outfile);
  GetMCTruthFRratio("TT","2017","MuMu","TagHF"  ,"PtPartonUncorr"   ,outfile);
  //GetPtPartonSF("2017","MuMu");
}
