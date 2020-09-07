#include "Riostream.h"
#include "TGraph.h"
#include <vector>


#include <string.h>
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TKey.h"
#include <iostream>
#include <TStyle.h>
#include "TCanvas.h"
#include "TLegend.h"

#include "TString.h"

void setTDRStyle();
void GetSignalEfficiencyPerMassBin(TString s_channel = "Schannel"){ 


  // Set Plotting style
  setTDRStyle();
  gStyle->SetPalette(1);
    
  vector <TString> masses;
  if (s_channel.Contains("Schannel")){
    masses.push_back("100");
    masses.push_back("125");
    masses.push_back("200");
    masses.push_back("250");
    masses.push_back("300");
    masses.push_back("400");
    masses.push_back("500");
    masses.push_back("600");
    masses.push_back("700");
    masses.push_back("800");
    masses.push_back("900");
    masses.push_back("1000");
    masses.push_back("1100");
    masses.push_back("1200");
    masses.push_back("1300");
    masses.push_back("1400");
    masses.push_back("1700");

  }
  else if (s_channel.Contains("Tchannel")){
    
    masses.push_back("500");
    masses.push_back("600");
    masses.push_back("700");
    masses.push_back("800");
    masses.push_back("900");
    masses.push_back("1000");
    masses.push_back("1100");
    masses.push_back("1200");
    masses.push_back("1300");
    masses.push_back("1400");
    masses.push_back("1500");
    masses.push_back("1700");
  }
  else{
    masses.push_back("100");
    masses.push_back("125");
    masses.push_back("200");
    masses.push_back("250");
    masses.push_back("300");
    masses.push_back("400");
    masses.push_back("500");
    masses.push_back("600");
    masses.push_back("700");
    masses.push_back("800");
    masses.push_back("900");
    masses.push_back("1000");
    masses.push_back("1100");
    masses.push_back("1200");
    masses.push_back("1300");
    masses.push_back("1400");
    masses.push_back("1700");

  }
  vector <TString> samples= masses;
  
  vector<TString> SR;
  SR.push_back("SR1");
  SR.push_back("SR2");
  SR.push_back("SR3");
  SR.push_back("SR4");
  vector <pair<TString,TString > > channel;
  channel.push_back(make_pair("MupMup","MumMum"));
  channel.push_back(make_pair("EpEp","EmEm"));
  vector<TString> muIDs;
  muIDs.push_back("HNTightV1");
  muIDs.push_back("HNTightV2");
  muIDs.push_back("POGTightWithTightIso");
  
  vector<TString> elIDs;
  elIDs.push_back("passTightID_nocc");
  elIDs.push_back("HNTight2016");
  elIDs.push_back("passMVAID_iso_WP80");
  elIDs.push_back("passMVAID_iso_WP90");
  elIDs.push_back("passMediumID");
  elIDs.push_back("passTightID");
  TString outfile = "HNEfficiencies_massbinned"+s_channel+".root";
  TFile* fout = new TFile(outfile.Data(),"RECREATE");




  for(unsigned int j = 0 ; j < SR.size(); ++j){
    for(unsigned int k = 0 ; k < channel.size(); ++k){
      
      vector<TString> IDs;
      if ( channel[k].first.Contains("Ep") ) IDs = elIDs;
      else IDs =   muIDs;
      
      for(unsigned int l = 0 ; l < IDs.size(); l++){
	vector<double> effs;
	TString ID = IDs[l];
	TString _sr = SR[j];
	TString sign = "_same_sign";
	TString _sign = "Signal__";
	if(_sr == "SR3") { _sr = "SR1"; sign = "_opposite_sign"; _sign = "SignalOS__"; }
	if(_sr == "SR4") { _sr = "SR2"; sign= "_opposite_sign"; _sign = "SignalOS__";}


	TString channel_lab = "MuMu";
	if ( channel[k].first.Contains("Ep")) channel_lab = "EE";

	TH1D* this_hist = new TH1D(_sr+"_"+channel_lab +"_massbinned_"+sign +"_nevent_HNtypeI_JA_"+channel_lab+"_" + ID , "", masses.size(), 0, masses.size());
	TH1D* this_histmm = new TH1D(_sr+"_"+channel_lab +"mm_massbinned_"+sign +"_nevent_HNtypeI_JA_"+channel_lab+"_" + ID , "", masses.size(), 0, masses.size());
	TH1D* this_histpp = new TH1D(_sr+"_"+channel_lab +"pp_massbinned_"+sign +"_nevent_HNtypeI_JA_"+channel_lab+"_" + ID , "", masses.size(), 0, masses.size());
	for(unsigned int i = 0 ; i < masses.size(); ++i){
	  
	  TString im = samples.at(i);
	  this_hist->GetXaxis()->SetBinLabel(i+1,im);
	}
	for(unsigned int i = 0 ; i < masses.size(); ++i){

          TString im = samples.at(i);

	  TFile * filemm = new TFile(("/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/HNtypeI_JA/2016/"+_sign+"/HNtypeI_JA_HN_"+s_channel+"_"+channel[k].first+"_"+im+"_nlo.root").Data());
	  TFile * filepp = new TFile(("/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/HNtypeI_JA/2016/"+_sign+"/HNtypeI_JA_HN_"+s_channel+"_"+channel[k].first+"_"+im+"_nlo.root").Data());
	  
	  bool mm_ok=true;
	  bool pp_ok=true;
	  if(filemm->IsZombie())mm_ok=false;
	  if(filepp->IsZombie()) pp_ok=false;
	  if(!filemm) mm_ok=false;

	  if(!filepp) pp_ok=false;

	  
	  TString hist = _sr + "_"+channel_lab + "_mn"+im + sign+"/"+_sr+"_"+channel_lab +"_mn"+im+sign +"_nevent_HNtypeI_JA_"+channel_lab+"_" + ID +"_";
	  if(!(TH1F*)filemm->Get(hist)) mm_ok=false;
	  if(!(TH1F*)filepp->Get(hist)) pp_ok=false;
	  cout  << "Channel " << channel_lab << " SR = " << _sr << " Charge = " << _sign << "  ID " << ID << "  Mass = " << masses.at(i) << endl;
	  
	  
	  if(mm_ok && pp_ok){
	  TH1* hnsig =   (TH1F*)filemm->Get(("CutFlow/NoCut_HNtypeI_JA_"+channel_lab + "_"+ ID));
	  float nsig = float(hnsig->Integral());
	  TH1*  hpass = (TH1F*)filemm->Get(hist);
	  TH1* hnsig2 =   (TH1F*)filepp->Get(("CutFlow/NoCut_HNtypeI_JA_"+channel_lab + "_"+ ID));
	  
	  float nsig2= float(hnsig2->Integral());
	  TH1*  hpass2 = (TH1F*)filepp->Get(hist);
	  
	  cout  << " \n ------- " << endl;
	  cout << "-- acceptance = " << hpass2->Integral()/nsig2 << endl;
	  cout << "++ acceptance = " << hpass->Integral()/nsig << endl;
	  hpass->Add(hpass2,1.);
	  double err ;
	  hpass->IntegralAndError(1, hpass->GetNbinsX()+1, err    , "");
	  cout << "Average = " << hpass->Integral()/(nsig+nsig2) << "  %err = " << 100*err/(hpass->Integral())<< endl;; 
	  this_hist->Fill(im, hpass->Integral()/(nsig+nsig2));
	  }
	  else  if(mm_ok ){
	    TH1* hnsig =   (TH1F*)filemm->Get(("CutFlow/NoCut_HNtypeI_JA_"+channel_lab + "_"+ ID));
	    float nsig = float(hnsig->Integral());
	    TH1*  hpass = (TH1F*)filemm->Get(hist);
	    this_hist->Fill(im, hpass->Integral()/(nsig));

	  }
	  else if(pp_ok ){
            TH1* hnsig =   (TH1F*)filepp->Get(("CutFlow/NoCut_HNtypeI_JA_"+channel_lab + "_"+ ID));
            float nsig = float(hnsig->Integral());
            TH1*  hpass = (TH1F*)filepp->Get(hist);
            this_hist->Fill(im, hpass->Integral()/(nsig));

          }
	  else this_hist->Fill(im, 0.);

	  if(mm_ok ){
            TH1* hnsig =   (TH1F*)filemm->Get(("CutFlow/NoCut_HNtypeI_JA_"+channel_lab + "_"+ ID));
            float nsig = float(hnsig->Integral());
            TH1*  hpass = (TH1F*)filemm->Get(hist);
            this_histmm->Fill(im, hpass->Integral()/(nsig));

          }
	  else this_histmm->Fill(im, 0.);

	  if(pp_ok ){
            TH1* hnsig =   (TH1F*)filepp->Get(("CutFlow/NoCut_HNtypeI_JA_"+channel_lab + "_"+ ID));
            float nsig = float(hnsig->Integral());
            TH1*  hpass = (TH1F*)filepp->Get(hist);
            this_histpp->Fill(im, hpass->Integral()/(nsig));

          }
	  else this_histpp->Fill(im, 0.);

	  filemm->Close();
          filepp->Close();
	}
	fout->cd();
	this_hist->Write();
	this_histmm->Write();
	this_histpp->Write();
	
	TCanvas* c1 = new TCanvas((_sr+"_"+channel_lab +"_highmass"+sign +"_nevents_HNtypeI_JA_"+channel_lab+"_" + ID), _sr+"_"+channel_lab +"_highmass"+sign +"_nevents_HNtypeI_JA_"+channel_lab+"_" + ID, 1600, 1200);
	this_hist->Draw("colz textE");
	c1->SaveAs((_sr+"_"+channel_lab +"_highmass"+sign +"_nevents_HNtypeI_JA_"+channel_lab+"_" + ID+"_massbinned.pdf"));
      }
      
    }
  }
  fout->Close();
}

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);


  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);


  // For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);
  tdrStyle->SetEndErrorSize(2);
  //  tdrStyle->SetErrorMarker(20);
  //  tdrStyle->SetErrorX(0.);

  tdrStyle->SetMarkerStyle(20);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);

  // tdrStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.12);
  tdrStyle->SetPadLeftMargin(0.12);
  tdrStyle->SetPadRightMargin(0.1);

  // For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.4);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset


  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.4);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  // Postscript options:
  tdrStyle->SetPaperSize(20.,20.);


  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();

}
