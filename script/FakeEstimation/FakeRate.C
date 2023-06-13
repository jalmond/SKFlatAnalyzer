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

void GetFR2D(){
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

void FakeRate(){
  GetFR1D();
}
