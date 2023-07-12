void GetFR2D(){
  TString fake_data = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/DATA/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_FakeRate.root";
  TString fake_W    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_WJets_MG.root";
  TString fake_Z    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_DYJets.root";
  TString fake_TT   = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_TTLJ_powheg.root";
  
  TFile *f_fake_data = new TFile(fake_data);
  TFile *f_fake_W    = new TFile(fake_W);
  TFile *f_fake_Z    = new TFile(fake_Z);
  TFile *f_fake_TT   = new TFile(fake_TT);
  
  TH2D *h_loose_data = (TH2D*)f_fake_data->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_loose_W    = (TH2D*)f_fake_W   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_loose_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_loose_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_data = (TH2D*)f_fake_data->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_W    = (TH2D*)f_fake_W   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");

  h_loose_data->Add(h_loose_W,-1.);
  h_loose_data->Add(h_loose_Z,-1.);
  h_loose_data->Add(h_loose_TT,-1.);
  h_tight_data->Add(h_tight_W,-1.);
  h_tight_data->Add(h_tight_Z,-1.);
  h_tight_data->Add(h_tight_TT,-1.);
  h_tight_data->Divide(h_loose_data);
  h_tight_data->SetName("Fake_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TFile *outfile = new TFile("FakeRate2D.root","RECREATE");
  outfile->cd();
  h_tight_data->Write();
}

void GetFR1D(){
  TString fake_data = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/DATA/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_FakeRate.root";
  TString fake_W    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_WJets_MG.root";
  TString fake_Z    = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_DYJets.root";
  TString fake_TT   = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/2017/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_TTLJ_powheg.root";
  
  TFile *f_fake_data = new TFile(fake_data);
  TFile *f_fake_W    = new TFile(fake_W);
  TFile *f_fake_Z    = new TFile(fake_Z);
  TFile *f_fake_TT   = new TFile(fake_TT);
  
  TH2D *h_loose_data = (TH2D*)f_fake_data->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_loose_W    = (TH2D*)f_fake_W   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_loose_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_loose_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Loose_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_data = (TH2D*)f_fake_data->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_W    = (TH2D*)f_fake_W   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_Z    = (TH2D*)f_fake_Z   ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");
  TH2D *h_tight_TT   = (TH2D*)f_fake_TT  ->Get("Fake_Tight_MuMu_HNL_ULID_2017_V1_40_ptratio_eta");

  TH1D *h_loose_data_1D = h_loose_data->ProjectionX("Fake_Loose_Data_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");
  TH1D *h_loose_W_1D    = h_loose_W   ->ProjectionX("Fake_Loose_W_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");
  TH1D *h_loose_Z_1D    = h_loose_Z   ->ProjectionX("Fake_Loose_Z_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");
  TH1D *h_loose_TT_1D   = h_loose_TT  ->ProjectionX("Fake_Loose_TT_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");
  TH1D *h_tight_data_1D = h_tight_data->ProjectionX("Fake_Tight_Data_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");
  TH1D *h_tight_W_1D    = h_tight_W   ->ProjectionX("Fake_Tight_W_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");
  TH1D *h_tight_Z_1D    = h_tight_Z   ->ProjectionX("Fake_Tight_Z_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");
  TH1D *h_tight_TT_1D   = h_tight_TT  ->ProjectionX("Fake_Tight_TT_MuMu_HNL_ULID_2017_V1_40_ptratio",0,-1,"e");

  // Show the stacked histo
  TCanvas *c1 = new TCanvas("loose_ptratio","",800,800);
  TCanvas *c2 = new TCanvas("tight_ptratio","",800,800);

  // Loose ptratio
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

  // Tight ptratio
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
  h_FR_1D->SetName("Fake_MuMu_HNL_ULID_2017_V1_40_ptratio");
  TFile *outfile = new TFile("FakeRate1D.root","RECREATE");
  outfile->cd();
  h_FR_1D->Write();
  c1->Write();
  c2->Write();

}

void GetMCTruthFR(TString year, TString channel, TString ID, TString sample, TFile* outfile){ // W, Z, TT

  TString sample_full;
  if(sample=="W") sample_full = "WJets_MG";
	else if(sample=="Z") sample_full = "DYJets";
	else if(sample=="TT") sample_full = "TTLJ_powheg";
  TString fake_MC = "/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_LeptonFakeRate/"+year+"/HNL_LeptonFakeRate_SkimTree_HNFakeBDT_"+sample_full+".root";
  //TString fake_MC = "hists_TTLJ.root";

  TFile *f_fake_MC = new TFile(fake_MC);

  TH1D *h_loose_prompt = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_prompt_ptratio");
  TH1D *h_loose_tau    = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_tau_ptratio");
  TH1D *h_loose_conv   = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_conv_ptratio");
  cout << "GetEntries of h_loose_conv with " << ID << " in " << sample << ": " << h_loose_conv->GetEntries() << endl;
  TH1D *h_loose_fake   = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fake_ptratio");
  TH1D *h_loose_fakeHF = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeHF_ptratio");
  TH1D *h_loose_fakeLF = (TH1D*)f_fake_MC->Get("Fake_Loose_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeLF_ptratio");
  TH1D *h_tight_prompt = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_prompt_ptratio");
  TH1D *h_tight_tau    = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_tau_ptratio");
  TH1D *h_tight_conv   = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_conv_ptratio");
  TH1D *h_tight_fake   = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fake_ptratio");
  TH1D *h_tight_fakeHF = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeHF_ptratio");
  TH1D *h_tight_fakeLF = (TH1D*)f_fake_MC->Get("Fake_Tight_"+channel+"_HNL_ULID_"+year+"_"+ID+"_NoSel_fakeLF_ptratio");

  // Show the stacked histo
  TCanvas *c1  = new TCanvas("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_prompt","",800,800);
  TCanvas *c2  = new TCanvas("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_tau"   ,"",800,800);
  TCanvas *c3  = new TCanvas("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_conv"  ,"",800,800);
  TCanvas *c4  = new TCanvas("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_fake"  ,"",800,800);
  TCanvas *c5  = new TCanvas("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_fakeHF","",800,800);
  TCanvas *c6  = new TCanvas("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_fakeLF","",800,800);
  TCanvas *c7  = new TCanvas("FakeRateRatioHoverL_"+sample+"_"+channel+"_"+ID+"_"+year,"",800,800);

  // prompt rate
  c1->cd();

  c1->SetRightMargin(0.05);
  c1->SetLeftMargin(0.14);

  h_tight_prompt->Divide(h_tight_prompt,h_loose_prompt,1,1,"B");
  h_tight_prompt->SetName("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_prompt");
  h_tight_prompt->GetXaxis()->SetTitle("p_{T}^{ratio} (GeV)");
  h_tight_prompt->GetXaxis()->SetLabelOffset(0);
  h_tight_prompt->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight_prompt->SetLineColor(kRed);
  h_tight_prompt->SetStats(0);
  h_tight_prompt->Draw();
  gPad->SetLogx();

  TLegend *lg_tight_prompt = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg_tight_prompt->AddEntry(h_tight_prompt,"prompt leptons","lpe"); // show line, polymarker, error bar
  lg_tight_prompt->SetBorderSize(0);
  lg_tight_prompt->Draw("same");

  // tau rate
  c2->cd();

  c2->SetRightMargin(0.05);
  c2->SetLeftMargin(0.14);

  h_tight_tau->Divide(h_tight_tau,h_loose_tau,1,1,"B");
  h_tight_tau->SetName("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_tau");
  h_tight_tau->GetXaxis()->SetTitle("p_{T}^{ratio} (GeV)");
  h_tight_tau->GetXaxis()->SetLabelOffset(0);
  h_tight_tau->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight_tau->SetLineColor(kRed);
  h_tight_tau->SetStats(0);
  h_tight_tau->Draw();
  gPad->SetLogx();

  TLegend *lg_tight_tau = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg_tight_tau->AddEntry(h_tight_tau,"tau leptons","lpe"); // show line, polymarker, error bar
  lg_tight_tau->SetBorderSize(0);
  lg_tight_tau->Draw("same");

  // conv rate
  c3->cd();

  c3->SetRightMargin(0.05);
  c3->SetLeftMargin(0.14);

  h_tight_conv->Divide(h_tight_conv,h_loose_conv,1,1,"B");
  h_tight_conv->SetName("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_conv");
  h_tight_conv->GetXaxis()->SetTitle("p_{T}^{ratio} (GeV)");
  h_tight_conv->GetXaxis()->SetLabelOffset(0);
  h_tight_conv->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight_conv->SetLineColor(kRed);
  h_tight_conv->SetStats(0);
  h_tight_conv->Draw();
  gPad->SetLogx();

  TLegend *lg_tight_conv = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg_tight_conv->AddEntry(h_tight_conv,"conv leptons","lpe"); // show line, polymarker, error bar
  lg_tight_conv->SetBorderSize(0);
  lg_tight_conv->Draw("same");

  // fake rate
  c4->cd();

  c4->SetRightMargin(0.05);
  c4->SetLeftMargin(0.14);

  h_tight_fake->Divide(h_tight_fake,h_loose_fake,1,1,"B");
  h_tight_fake->SetName("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_fake");
  h_tight_fake->GetXaxis()->SetTitle("p_{T}^{ratio} (GeV)");
  h_tight_fake->GetXaxis()->SetLabelOffset(0);
  h_tight_fake->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight_fake->SetLineColor(kRed);
  h_tight_fake->SetStats(0);
  h_tight_fake->Draw();
  gPad->SetLogx();

  TLegend *lg_tight_fake = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg_tight_fake->AddEntry(h_tight_fake,"fake leptons","lpe"); // show line, polymarker, error bar
  lg_tight_fake->SetBorderSize(0);
  lg_tight_fake->Draw("same");

  // fakeHF rate
  c5->cd();

  c5->SetRightMargin(0.05);
  c5->SetLeftMargin(0.14);

  h_tight_fakeHF->Divide(h_tight_fakeHF,h_loose_fakeHF,1,1,"B");
  h_tight_fakeHF->SetName("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_fakeHF");
  h_tight_fakeHF->GetXaxis()->SetTitle("p_{T}^{ratio} (GeV)");
  h_tight_fakeHF->GetXaxis()->SetLabelOffset(0);
  h_tight_fakeHF->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight_fakeHF->SetLineColor(kRed);
  h_tight_fakeHF->SetStats(0);
  h_tight_fakeHF->Draw();
  gPad->SetLogx();

  TLegend *lg_tight_fakeHF = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg_tight_fakeHF->AddEntry(h_tight_fakeHF,"fakeHF leptons","lpe"); // show line, polymarker, error bar
  lg_tight_fakeHF->SetBorderSize(0);
  lg_tight_fakeHF->Draw("same");

  // fakeLF rate
  c6->cd();

  c6->SetRightMargin(0.05);
  c6->SetLeftMargin(0.14);

  h_tight_fakeLF->Divide(h_tight_fakeLF,h_loose_fakeLF,1,1,"B");
  h_tight_fakeLF->SetName("FakeRate_"+sample+"_"+channel+"_"+ID+"_"+year+"_fakeLF");
  h_tight_fakeLF->GetXaxis()->SetTitle("p_{T}^{ratio} (GeV)");
  h_tight_fakeLF->GetXaxis()->SetLabelOffset(0);
  h_tight_fakeLF->GetYaxis()->SetTitle("#frac{Tight}{Loose}");
  h_tight_fakeLF->SetLineColor(kRed);
  h_tight_fakeLF->SetStats(0);
  h_tight_fakeLF->Draw();
  gPad->SetLogx();

  TLegend *lg_tight_fakeLF = new TLegend(0.6, 0.77, 0.87, 0.88);
  lg_tight_fakeLF->AddEntry(h_tight_fakeLF,"fakeLF leptons","lpe"); // show line, polymarker, error bar
  lg_tight_fakeLF->SetBorderSize(0);
  lg_tight_fakeLF->Draw("same");

  // fakeHF rate Ptratio MVAlt0p64
  c7->cd();

  c7->SetRightMargin(0.05);
  c7->SetLeftMargin(0.14);

  TH1D *h_FRratio = (TH1D*)h_tight_fakeHF->Clone();
  h_FRratio->Divide(h_tight_fakeLF);
  h_FRratio->SetName("FakeRateRatioHoverL_"+sample+"_"+channel+"_"+ID+"_"+year);
  h_FRratio->GetXaxis()->SetTitle("p_{T}^{ratio} (GeV)");
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

  // create root file
  outfile->cd();
  c1->Write();
  c2->Write();
  c3->Write();
  c4->Write();
  c5->Write();
  c6->Write();
  c7->Write();

}

void FakeRate(){
  //GetFR1D();
	TFile *outfile = new TFile("FakeRateTruth.root","RECREATE");
  GetMCTruthFR("2017","MuMu","TriLep","TT",outfile);
  GetMCTruthFR("2017","MuMu","MVALoose","TT",outfile);
  GetMCTruthFR("2017","MuMu","TriLep","W",outfile);
  GetMCTruthFR("2017","MuMu","MVALoose","W",outfile);
  GetMCTruthFR("2017","MuMu","TriLep","Z",outfile);
  GetMCTruthFR("2017","MuMu","MVALoose","Z",outfile);
}
