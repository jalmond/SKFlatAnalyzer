#include <iostream>
#include "common.h"

// This is a script to draw fake closure related plots.
// Why don't use the plotter in HNDiLepWorkspace? That script is pretty heavy and highly depends on pre-defined data path and MC path. FakeClosure output paths don't fit that code structure...
// How to run this code?
// root -b -l -q makePlots_FakeClosure.C

TString workdir = "/data6/Users/jihkim/SKFlatOutput/";
TString SKFlatVersion = "Run2UltraLegacy_v3";
TString skim = "SkimTree_HNMultiLepBDT";
TString flagObs = "RunFakeClosureObs__";
TString flagPred = "RunFakeClosurePred__";
TString analyzer = "HNL_SignalRegionPlotter";
TString file_path = "";
//vector<TString> year = {"2016", "2017", "2018"};
vector<TString> year = {"2017"};
//vector<TString> luminosity = {"35.9", "41.5", "59.7"};
vector<TString> luminosity = {"41.5"};

const int YearNumber = year.size(); // number of the years
int MCNumber = 0;
int BundleNumber = 0;
int maxBinNumber_total = 0, maxBinNumber_temp = 0;
double minRange = 0., maxRange = 0., binContent = 0., binError = 0., binError_Stat = 0., binError_Syst = 0.;
double max_Data = 0., max_Background = 0., max_Hist = 0.;

void FixOverflows(TH1D *hist, int maxBin, int maxBin_total);

void makePlots_FakeClosure(){

  // Select what to draw first
  //TString DrawSample = "All";
  //TString DrawSample = "DY";
  //TString DrawSample = "DY_MG";
  TString DrawSample = "TT";
  //TString DrawSample = "WJets";

  // Set MC, Bundle numbers according to the sample type
  if(DrawSample=="All"){
    MCNumber = 5; // DYJets, TTLL, TTLJ, TTJJ, WJets
    BundleNumber = 3; // DY, TT, W
  }
  else if(DrawSample=="DY"){
    MCNumber = 1; // DYJets
    BundleNumber = 1;
  }
  else if(DrawSample=="DY_MG"){
    MCNumber = 1; // DYJets_MG <-- checked DYJets10to50_MG gives sudden peaks due to low stats
    BundleNumber = 1;
  }
  else if(DrawSample=="TT"){
    MCNumber = 3; // TTLL, TTLJ, TTJJ
    BundleNumber = 1;
  }

  // Read line
  string histline;
  ifstream in("histList_FakeClosure.txt");
  // Line loop
  while(getline(in, histline)){
    std::istringstream is(histline);
    TString this_line = histline;
    if(this_line.Contains("#")||this_line=="") continue;
    TString channel, region, variable, IDname, txt_region, output_region, txt_variable;
    int rebin, minBinNumber, maxBinNumber;
    is >> channel;
    is >> region;
    is >> variable;
    is >> IDname;
    is >> rebin;
    is >> minBinNumber;
    is >> maxBinNumber;

    output_region = region;

    // txt_variable
    if(variable.Contains("1_pt")) txt_variable = "p_{T}(l_{1}) (GeV)";
    if(variable.Contains("2_pt")) txt_variable = "p_{T}(l_{2}) (GeV)";
    if(variable.Contains("3_pt")) txt_variable = "p_{T}(l_{3}) (GeV)";
    if(variable.Contains("MT")) txt_variable = "M_{T}(l,#slash{E}_{T}^{miss}) (GeV)";
    if(variable.Contains("MET")) txt_variable = "#slash{E}_{T}^{miss} (GeV)";
    if(variable.Contains("MET2ST")) txt_variable = "(#slash{E}_{T}^{miss})^{2}/S_{T} (GeV)";
    if(variable.Contains("DiLep")) txt_variable = "m(ll) (GeV)";
    if(variable.Contains("TriLep")) txt_variable = "m(lll) (GeV)";
    if(region=="PtParton"&&variable.Contains("1_pt")) txt_variable = "p_{T}^{parton}(l_{1}) (GeV)";
    if(region=="PtParton"&&variable.Contains("2_pt")) txt_variable = "p_{T}^{parton}(l_{2}) (GeV)";
    if(variable.Contains("1_eta")) txt_variable = "#eta(l_{1})";
    if(variable.Contains("2_eta")) txt_variable = "#eta(l_{2})";
    if(variable.Contains("N_AK4Jets")) txt_variable = "N_{j}";

    // Declare variables needed for making plots 
    TFile *f_Data[MCNumber][YearNumber], *f_MC[MCNumber][YearNumber];
    TH1D *h_Data[MCNumber][YearNumber], *h_MC[MCNumber][YearNumber], *h_Temp[YearNumber], *h_Bundle_Data[YearNumber], *h_Bundle_MC[BundleNumber][YearNumber], *h_Total_MC[YearNumber],*h_Error_MC[YearNumber], *h_Ratio[YearNumber];
    EColor color_bundle[BundleNumber];
    TCanvas *c1;
    TPad *c_up, *c_down;
    THStack *hs;
    TLegend *lg, *lg2;

    // Year loop
    for(int it_y=0; it_y<year.size(); it_y++){
      file_path = SKFlatVersion+"/"+analyzer+"/"+year.at(it_y)+"/";

      //=========================================
      //==== Set input ROOT files
      //=========================================

      // DATA (Pred), MC (Obs)
      if(DrawSample=="All"){
        f_Data[0][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_DYJets.root");
        f_Data[1][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_TTJJ_powheg.root");
        f_Data[2][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_TTLL_powheg.root");
        f_Data[3][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_TTLJ_powheg.root");
        f_Data[4][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_WJets_MG.root");
        
        f_MC[0][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_DYJets.root");
        f_MC[1][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_TTJJ_powheg.root");
        f_MC[2][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_TTLL_powheg.root");
        f_MC[3][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_TTLJ_powheg.root");
        f_MC[4][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_WJets_MG.root");
      }
      else if(DrawSample=="DY"){
        f_Data[0][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_DYJets.root");
        f_MC[0][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_DYJets.root");
      }
      else if(DrawSample=="DY_MG"){
        f_Data[0][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_DYJets_MG.root");
        f_MC[0][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_DYJets_MG.root");
      }
      else if(DrawSample=="TT"){
        f_Data[0][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_TTJJ_powheg.root");
        f_Data[1][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_TTLL_powheg.root");
        f_Data[2][it_y] = new TFile(workdir+file_path+flagPred+"/"+analyzer+"_"+skim+"_TTLJ_powheg.root");
        
        f_MC[0][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_TTJJ_powheg.root");
        f_MC[1][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_TTLL_powheg.root");
        f_MC[2][it_y]   = new TFile(workdir+file_path+flagObs+"/"+analyzer+"_"+skim+"_TTLJ_powheg.root");
      }

      //=========================================
      //==== Get histograms
      //=========================================

      TString hist_path = IDname+"/RegionPlots_Preselection";
      // add prefix according to the flavor
      if     (channel=="MuMu") hist_path = "MuMu_Channel/"+hist_path;
      else if(channel=="EE")   hist_path = "EE_Channel/"+hist_path;
      else if(channel=="EMu")  hist_path = "EMu_Channel/"+hist_path; // FIXME later

      if(region=="PtParton") hist_path = hist_path+"_PtParton"; // if region == "PtParton", fakes have been drawn as a function of PtParton, even though the histo name is "Lep_*_Pt".

      // Fake Obs, Pred
      for(int it_mc=0; it_mc<MCNumber; it_mc++){
        h_Data[it_mc][it_y] = (TH1D*)f_Data[it_mc][it_y]->Get(hist_path+"/"+variable);
        h_MC[it_mc][it_y]   = (TH1D*)f_MC[it_mc][it_y]->Get(hist_path+"/"+variable);
      }

      // Set directory and delete the original files
      for(int it_mc=0; it_mc<MCNumber; it_mc++){
        if(h_Data[it_mc][it_y]) h_Data[it_mc][it_y]->SetDirectory(0);
        if(h_MC[it_mc][it_y])   h_MC[it_mc][it_y]->SetDirectory(0);

        f_Data[it_mc][it_y]->Close();
        f_MC[it_mc][it_y]->Close();
      }

      //=========================================
      //==== Make plots
      //=========================================

      // Make am empty histogram to be copied multiple times later
      h_Temp[it_y] = (TH1D*)h_Data[0][it_y]->Clone();
      maxBinNumber_temp = h_Temp[it_y]->GetNbinsX();
      for(int i=0; i<maxBinNumber_temp+2; i++){
        h_Temp[it_y]->SetBinContent(i, 0.);
        h_Temp[it_y]->SetBinError(i, 0.);
      }

      //==== CANVAS
      c1 = new TCanvas("c1", "", 1000, 1000);
      c1->cd();
      setTDRStyle();

      //==== PAD : drawing distribution
      c_up = new TPad("c_up", "", 0, 0.25, 1, 1);
      c_up->SetTopMargin(0.08);
      c_up->SetBottomMargin(0.017);
      c_up->SetLeftMargin(0.14);
      c_up->SetRightMargin(0.04);
      c_up->Draw();
      c_up->cd();

      // Merge Obs
      h_Bundle_Data[it_y] = (TH1D*)h_Temp[it_y]->Clone(); // create data bundle mold
      for(int it_mc=0; it_mc<MCNumber; it_mc++){
        if(h_Data[it_mc][it_y]) h_Bundle_Data[it_y]->Add(h_Data[it_mc][it_y]); // add all
      }
      // Merge Pred
      for(int it_bd=0; it_bd<BundleNumber; it_bd++){
        h_Bundle_MC[it_bd][it_y] = (TH1D*)h_Temp[it_y]->Clone(); // create bundle mold for each process
      }
      if(DrawSample=="All"){
        for(int it_mc=0; it_mc<=0; it_mc++){
          if(h_MC[it_mc][it_y]) h_Bundle_MC[0][it_y]->Add(h_MC[it_mc][it_y]); // add DY
        }
        for(int it_mc=1; it_mc<=3; it_mc++){
          if(h_MC[it_mc][it_y]) h_Bundle_MC[1][it_y]->Add(h_MC[it_mc][it_y]); // add TT
        }
        for(int it_mc=4; it_mc<MCNumber; it_mc++){
          if(h_MC[it_mc][it_y]) h_Bundle_MC[2][it_y]->Add(h_MC[it_mc][it_y]); // add WJets
        }
      }
      else if(DrawSample=="DY"){
        for(int it_mc=0; it_mc<=0; it_mc++){
          if(h_MC[it_mc][it_y]) h_Bundle_MC[0][it_y]->Add(h_MC[it_mc][it_y]); // DY
        }
      }
      else if(DrawSample=="DY_MG"){
        for(int it_mc=0; it_mc<=0; it_mc++){
          if(h_MC[it_mc][it_y]) h_Bundle_MC[0][it_y]->Add(h_MC[it_mc][it_y]); // DY_MG
        }
      }
      else if(DrawSample=="TT"){
        for(int it_mc=0; it_mc<=2; it_mc++){
          if(h_MC[it_mc][it_y]) h_Bundle_MC[0][it_y]->Add(h_MC[it_mc][it_y]); // TT
        }
      }

      // Color
      if(DrawSample=="All"){
        color_bundle[0] = kYellow;            // DY
        color_bundle[1] = kRed;               // TT
        color_bundle[2] = (EColor)(kAzure+8); // WJets
      }
      else if(DrawSample.Contains("DY")){
        color_bundle[0] = kYellow;            // DY(_MG)
      }
      else if(DrawSample=="TT"){
        color_bundle[0] = kRed;               // TT
      }

      // Rebin
      h_Bundle_Data[it_y]->Rebin(rebin);
      for(int it_bd=0; it_bd<BundleNumber; it_bd++){
        h_Bundle_MC[it_bd][it_y]->Rebin(rebin);
      }
      h_Temp[it_y]->Rebin(rebin);

      maxBinNumber_total = h_Bundle_Data[it_y]->GetNbinsX();  // This is needed for adding overflow bins

      // This is needed for drawing a line in the ratio plot
      minRange = h_Bundle_Data[it_y]->GetBinLowEdge(minBinNumber);
      maxRange = h_Bundle_Data[it_y]->GetBinLowEdge(maxBinNumber) + h_Bundle_Data[it_y]->GetBinWidth(maxBinNumber);

      // Fix overflows
      FixOverflows(h_Bundle_Data[it_y], maxBinNumber, maxBinNumber_total);
      for(int it_bd=0; it_bd<BundleNumber; it_bd++){
        FixOverflows(h_Bundle_MC[it_bd][it_y], maxBinNumber, maxBinNumber_total);
      }

      // Stack & Draw MC
      hs = new THStack("hs", "");
      for(int it_bd=0; it_bd<BundleNumber; it_bd++){
        h_Bundle_MC[it_bd][it_y]->SetLineWidth(0);
        h_Bundle_MC[it_bd][it_y]->SetFillColor(color_bundle[it_bd]);
        hs->Add(h_Bundle_MC[it_bd][it_y]);
      }
      hs->Draw("hist");
      hs->SetTitle("");
      hs->GetXaxis()->SetLabelSize(0.);
      //hs->GetYaxis()->SetLabelSize(0.045);
      hs->GetYaxis()->SetLabelSize(0.04);
      if(!( variable.Contains("Eta") )) hs->GetYaxis()->SetTitle("Events / "+TString::Itoa(rebin, 10)+" GeV");
      else hs->GetYaxis()->SetTitle("Events");
      hs->GetYaxis()->SetTitleSize(0.075);
      hs->GetYaxis()->SetTitleOffset(0.8);
      hs->GetXaxis()->SetRange(minBinNumber, maxBinNumber);

      // h_Total_MC : A histogram of all MC for calcalating the total error of all backgrounds
      h_Total_MC[it_y] = (TH1D*)h_Temp[it_y]->Clone();
      for(int it_bd=0; it_bd<BundleNumber; it_bd++){
        if(h_Bundle_MC[it_bd][it_y]) h_Total_MC[it_y]->Add(h_Bundle_MC[it_bd][it_y]); // total sum of all Pred MC
      }

      // Draw MC error
      h_Total_MC[it_y]->SetMarkerSize(0);
      h_Total_MC[it_y]->SetLineWidth(0);
      h_Total_MC[it_y]->SetFillStyle(3144);
      h_Total_MC[it_y]->SetFillColor(kBlack);
      h_Total_MC[it_y]->Draw("e2 same");

      // Draw Data
      h_Bundle_Data[it_y]->SetMarkerStyle(20);
      h_Bundle_Data[it_y]->SetMarkerColor(kBlack);
      h_Bundle_Data[it_y]->Draw("ep same");

      // Set Min or Max of y axis
      max_Data = h_Bundle_Data[it_y]->GetBinContent(h_Bundle_Data[it_y]->GetMaximumBin());
      max_Background = h_Total_MC[it_y]->GetBinContent(h_Total_MC[it_y]->GetMaximumBin());
      max_Hist = std::max(max_Data, max_Background);
      hs->SetMinimum(0);
      hs->SetMaximum(max_Hist+100);

      // Draw the legend
      lg = new TLegend(0.65, 0.55, 0.8, 0.9);
      lg->AddEntry(h_Total_MC[it_y], "Stat. Uncertainty", "f"); // No need for systematic error ...?
      lg->AddEntry(h_Bundle_Data[it_y], "Measured", "lep");
      if(DrawSample=="All"){
        lg->AddEntry(h_Bundle_MC[0][it_y], "DYJets", "f");
        lg->AddEntry(h_Bundle_MC[1][it_y], "t#bar{t}", "f");
        lg->AddEntry(h_Bundle_MC[2][it_y], "WJets", "f");
      }
      else if(DrawSample=="DY"){
        lg->AddEntry(h_Bundle_MC[0][it_y], "DYJets", "f");
      }
      else if(DrawSample=="DY_MG"){
        lg->AddEntry(h_Bundle_MC[0][it_y], "DYJets_MG", "f");
      }
      else if(DrawSample=="TT"){
        lg->AddEntry(h_Bundle_MC[0][it_y], "t#bar{t}", "f");
      }
      lg->SetBorderSize(0);
      lg->SetTextSize(0.03);
      lg->SetFillStyle(1001);
      lg->SetShadowColor(0);
      lg->Draw("same");

      // Add text
      TLatex txt;
      txt.SetNDC();
      txt.SetTextSize(0.05);
      txt.SetTextAlign(32);
      txt.SetTextFont(42);
      txt.DrawLatex(.96,.95, luminosity.at(it_y)+" fb^{-1} (13 TeV)");

      TLatex txt2;
      txt2.SetNDC();
      txt2.SetTextSize(0.06);
      txt2.SetTextAlign(12);
      txt2.SetTextFont(62);
      txt2.DrawLatex(.14, .95, "CMS");

      TLatex txt3;
      txt3.SetNDC();
      txt3.SetTextSize(0.055);
      txt3.SetTextAlign(12);
      txt3.SetTextFont(52);
      txt3.DrawLatex(.24, .94, "Preliminary");

      TLatex txt4;
      txt4.SetNDC();
      txt4.SetTextSize(0.05);
      txt4.SetTextAlign(12);
      txt4.SetTextFont(62);
      txt4.DrawLatex(.19,.84, "Preselection");
 
      c1->cd();

      // PAD : drawing ratio
      c_down = new TPad("c_down", "", 0, 0, 1, 0.25);
      c_down->SetTopMargin(0.03);
      c_down->SetBottomMargin(0.35);
      c_down->SetLeftMargin(0.14);
      c_down->SetRightMargin(0.04);
      //c_down->SetGridx();
      //c_down->SetGridy();
      c_down->Draw();
      c_down->cd();

      // Relative error of the sum of Pred.
      h_Error_MC[it_y] = (TH1D*)h_Temp[it_y]->Clone();
      for(int it_bin = minBinNumber; it_bin < maxBinNumber+1; it_bin++){
        binContent = h_Total_MC[it_y]->GetBinContent(it_bin);
        binError_Stat = h_Total_MC[it_y]->GetBinError(it_bin);
        //binError_Syst = h_Total_MC[it_y]->GetBinContent(it_bin)*0.3;
        binError = sqrt(binError_Stat*binError_Stat + binError_Syst*binError_Syst);
        if(binContent != 0.){
          binError = binError/binContent;
          binError_Stat = binError_Stat/binContent;
        }
        else{
          binError = 0.;
          binError_Stat = 0.;
        }
        h_Error_MC[it_y]->SetBinContent(it_bin, 1.);
        h_Error_MC[it_y]->SetBinError(it_bin, binError_Stat);
        //h_Error_Background2[it_y]->SetBinContent(it_bin, 1.);
        //h_Error_Background2[it_y]->SetBinError(it_bin, binError); // use this later if stat + syst error is necessary
      }
      h_Error_MC[it_y]->SetTitle("");
      h_Error_MC[it_y]->SetStats(0);
      h_Error_MC[it_y]->GetXaxis()->SetTitle(txt_variable);
      h_Error_MC[it_y]->GetYaxis()->SetTitle("#frac{Meas.}{Pred.}");
      h_Error_MC[it_y]->GetXaxis()->SetRange(minBinNumber, maxBinNumber);
      h_Error_MC[it_y]->GetYaxis()->SetRangeUser(0, 2);
      h_Error_MC[it_y]->GetXaxis()->SetLabelSize(0.12);
      h_Error_MC[it_y]->GetYaxis()->SetLabelSize(0.08);
      h_Error_MC[it_y]->GetXaxis()->SetTitleSize(0.16);
      h_Error_MC[it_y]->GetYaxis()->SetTitleSize(0.14);
      h_Error_MC[it_y]->GetXaxis()->SetTitleOffset(0.9);
      h_Error_MC[it_y]->GetYaxis()->SetTitleOffset(0.4);

      h_Error_MC[it_y]->SetMarkerSize(0);
      h_Error_MC[it_y]->SetLineWidth(0);
      h_Error_MC[it_y]->SetFillStyle(3144);
      h_Error_MC[it_y]->SetFillColor(kOrange);
      h_Error_MC[it_y]->Draw("e2"); 

      //h_Error_Background2[it_y]->SetMarkerSize(0);
      //h_Error_Background2[it_y]->SetLineWidth(0);
      //h_Error_Background2[it_y]->SetFillStyle(1001);
      //h_Error_Background2[it_y]->SetFillColor(kCyan);
      //h_Error_Background2[it_y]->Draw("e2 same");
 
      // Meas./Pred. ratio
      h_Ratio[it_y] = (TH1D*)h_Bundle_Data[it_y]->Clone();
      h_Ratio[it_y]->Divide(h_Total_MC[it_y]);
      h_Ratio[it_y]->SetLineColor(1);
      h_Ratio[it_y]->SetMarkerColor(1);
      h_Ratio[it_y]->SetMarkerStyle(20);
      h_Ratio[it_y]->Draw("ep same");

      lg2 = new TLegend(0.75, 0.88, 0.9, 0.95);
      //lg2->SetNColumns(2);
      lg2->AddEntry(h_Error_MC[it_y], "Stat. uncertainty", "f");
      //lg2->AddEntry(h_Error_Background2[it_y], "Stat. + Syst.", "f");
      lg2->SetBorderSize(1);
      lg2->SetTextSize(0.06);
      lg2->SetFillStyle(1001);
      lg2->SetShadowColor(0);
      lg2->Draw("same");

      TLine line(minRange, 1., maxRange, 1.);
      line.SetLineWidth(1);
      line.SetLineColor(2);
      line.Draw();

      c1->cd();

      //=========================================
      //==== Save plots
      //=========================================
      if(DrawSample=="All"){
        gSystem->Exec("mkdir -p plots_FakeClosure/"+channel);
        c1->SaveAs("./plots_FakeClosure/"+channel+"/"+output_region+"_"+IDname+"_"+variable+"_"+year.at(it_y)+".png");
      }
      else{
        gSystem->Exec("mkdir -p plots_FakeClosure/"+channel+"/"+DrawSample);
        c1->SaveAs("./plots_FakeClosure/"+channel+"/"+DrawSample+"/"+output_region+"_"+IDname+"_"+variable+"_"+DrawSample+"_"+year.at(it_y)+".png");
      }

      delete c_up;
      delete c_down;

      c1->Close();

      delete c1;
      delete hs;
      delete lg;
      delete lg2;
      //delete f_Fake[it_y];
      for(int it_mc=0; it_mc<MCNumber; it_mc++){
        delete f_MC[it_mc][it_y];
        delete h_MC[it_mc][it_y];
        delete f_Data[it_mc][it_y];
        delete h_Data[it_mc][it_y];
      }
      delete h_Bundle_Data[it_y];
      delete h_Temp[it_y];
      for(int it_bd=0; it_bd<BundleNumber; it_bd++){
        delete h_Bundle_MC[it_bd][it_y];
      }
      delete h_Total_MC[it_y];
      delete h_Error_MC[it_y];
      delete h_Ratio[it_y];

    }  // year
  }  // histline  
}


void FixOverflows(TH1D *hist, int maxBin, int maxBin_total){ // add all bin contents after the last bin to the last bin; so the last bin will contain the last + after
  double binContent = hist->GetBinContent(maxBin);
  double binError = hist->GetBinError(maxBin)*hist->GetBinError(maxBin);

  for(int i=maxBin+1; i<maxBin_total+2; i++){
    binContent += hist->GetBinContent(i);
    binError += hist->GetBinError(i)*hist->GetBinError(i);
  }

  binError = sqrt(binError);

  hist->SetBinContent(maxBin, binContent);
  hist->SetBinError(maxBin, binError);
}
