#include "HNL_LeptonCore.h"


void HNL_LeptonCore::FillCutflowDef(TString cutflow_dirname,TString cutflow_histname, double weight, vector<TString> bin_lables, TString fill_label){

  char end_str = string(cutflow_dirname).back();

  if ( end_str  == '/') {
    cout << "[HNL_LeptonCore::FillCutflowDef ] ERROR in assiging Hist name, remove / from end " << endl;
    cout << cutflow_dirname << endl;
    exit(EXIT_FAILURE);

  }
  TH1D *this_hist = GetHist1D(cutflow_dirname+"/"+cutflow_histname);

  if( !this_hist ){
    TString cf_name="FillEventCutflow";

    if(cutflow_histname.Contains("SR")) cf_name="LimitBins";
    if(cutflow_histname.Contains("SR_Cut")) cf_name="SignalCutFlow";

    if(cutflow_histname.Contains("Limit")) cf_name="LimitBins";
    if(!cutflow_dirname.Contains("ChannelCutFlow"))  cf_name = cutflow_dirname + "/"+cf_name;
    else cf_name = cutflow_dirname;

    cout << "cf_name+cutflow_histname = " << cf_name+"/"+cutflow_histname << endl;
    this_hist = new TH1D(cf_name+"/"+cutflow_histname, "", bin_lables.size(), 0, bin_lables.size());
    for (unsigned int i=0 ; i < bin_lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,bin_lables[i]);
    this_hist->SetDirectory(NULL);

    maphist_TH1D[cutflow_dirname+"/"+cutflow_histname] = this_hist;
  }
  this_hist->Fill(fill_label, weight);

}


void HNL_LeptonCore::FillCutflow(TString analysis_dir_name,TString histname, double weight, vector<TString> lables, TString label){
  FillCutflowDef(analysis_dir_name,histname, weight,lables, label);
  FillCutflowDef(analysis_dir_name,histname+"_unweighted", 1,lables, label);
}


void HNL_LeptonCore::FillCutflow(AnalyzerParameter param,TString histname, double weight, vector<TString> lables, TString label){
  
  if(param.Channel!="Default")FillCutflowDef(param.CutFlowDirIncChannel(),histname, weight,lables, label);
  FillCutflowDef(param.CutFlowDirChannel(),   histname, weight,lables, label);

  //FillCutflowDef(param.CutFlowDirChannel(),   histname+"_unweighted",1,lables, label);
  //if(param.Channel!="Default")FillCutflowDef(param.CutFlowDirIncChannel(),histname+"_unweighted", 1,lables, label);

}

void HNL_LeptonCore::FillLimitInput(HNL_LeptonCore::SearchRegion sr, double event_weight, TString label,  TString hist_path){

  vector<TString> lables = GetLabelsFromRegion(sr);
  TString histname = GetCutFlowNameFromRegion(sr);

  FillCutflowDef(hist_path,   histname, event_weight,lables, label);
  //FillCutflowDef(hist_path,   histname+"_unweighted", 1,lables, label);

}
void HNL_LeptonCore::FillCutflow(HNL_LeptonCore::SearchRegion sr, double event_weight, TString label,  TString hist_path){

  vector<TString> lables = GetLabelsFromRegion(sr);
  TString histname = GetCutFlowNameFromRegion(sr);

  FillCutflowDef(hist_path,   histname, event_weight,lables, label);
  //FillCutflowDef(hist_path,   histname+"_unweighted", 1,lables, label);

}


void HNL_LeptonCore::FillCutflow(HNL_LeptonCore::SearchRegion sr, double event_weight, TString label,  AnalyzerParameter param){
  
  TString OutputDir= param.CutFlowDirChannel();
  
  vector<TString> lables = GetLabelsFromRegion(sr);
  TString histname = GetCutFlowNameFromRegion(sr);
  FillCutflowDef(OutputDir,   histname, event_weight,lables, label);
  if(param.Channel!="Default") FillCutflowDef(param.CutFlowDirIncChannel(),   histname, event_weight,lables, label);

  //  FillCutflowDef(OutputDir,   histname+"_unweighted", 1,lables, label);
  //  if(param.Channel!="Default") FillCutflowDef(param.CutFlowDirIncChannel(),   histname+"_unweighted", 1,lables, label);


  return;
}


void HNL_LeptonCore::FillFullTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D("FillEventFullType/"+histname, "", lables.size(), 0, lables.size(),  lables.size(), 0, lables.size());
    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetYaxis()->SetBinLabel(i+1,lables[i]);

    maphist_TH2D[histname] = this_hist;
  }

  this_hist->Fill(label1, label2, fabs(weight));

}


void HNL_LeptonCore::FillTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D("CR2DCutflow/"+histname, "", lables.size(), 0, lables.size(),  lables.size(), 0, lables.size());
    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetYaxis()->SetBinLabel(i+1,lables[i]);

    maphist_TH2D[histname] = this_hist;
  }

  this_hist->Fill(label1, label2, fabs(weight));

}


void HNL_LeptonCore::FillCutFlow(bool IsCentral, TString suffix, TString histname, double ev_weight){

  if(IsCentral)   FillHist(suffix+"/"+histname, 0., ev_weight, 1, 0., 1.);

  return ;
}



TString HNL_LeptonCore::GetCutFlowNameFromRegion(HNL_LeptonCore::SearchRegion sr){
  
  TString EVHistName = "";
  if(sr==WZCR)   EVHistName= "WZCR";
  if(sr==WZBCR)   EVHistName= "WZBCR";
  if(sr==ZZCR)   EVHistName= "ZZCR";
  if(sr==ZZCR2)  EVHistName= "ZZCR2";
  if(sr==WGCR)   EVHistName= "WGCR";
  if(sr==ZGCR)   EVHistName= "ZGCR";
  if(sr==WZBCR)   EVHistName= "WZBCR";
  if(sr==WZCR2)   EVHistName= "WZCR2";
  if(sr==HMCR1)   EVHistName= "HMCR1";
  if(sr==HMCR2)   EVHistName= "HMCR2";
  if(sr==HMCR3)   EVHistName= "HMCR3";
  if(sr==HMNPCR)   EVHistName= "HMNPCR";
  if(sr==HMBCR)   EVHistName= "HMBCR";
  if(sr==HM1JCR)   EVHistName= "HM1JCR";
  if(sr==PreselVBF)   EVHistName= "PreselVBF";
  if(sr==Presel)   EVHistName= "Presel";
  if(sr==WWNP1CR)   EVHistName= "WWNP1CR";
  if(sr==WWNP2CR)   EVHistName= "WWNP2CR";
  if(sr==WWNP3CR)   EVHistName= "WWNP3CR";
  if(sr==WWCR1)   EVHistName= "WWCR1";
  if(sr==WWCR2)   EVHistName= "WWCR2";
  if(sr==ZAK8CR)   EVHistName= "ZAK8CR";
  if(sr==ZCR)   EVHistName= "ZCR";
  if(sr==ZNPElCR)   EVHistName= "ZNPElCR";
  if(sr==ZNPMuCR)   EVHistName= "ZNPMuCR";
  if(sr==TopCR)   EVHistName= "TopCR";
  if(sr==TopNPCR)   EVHistName= "TopNPCR";
  if(sr==TopAK8NPCR)   EVHistName= "TopAK8NPCR";
  if(sr==TopNPCR2)   EVHistName= "TopNPCR2";


  /////

  if(sr==SR1)    EVHistName= "Cutflow_SR1";
  if(sr==SR2)    EVHistName= "Cutflow_SR2";
  if(sr==SR3Fail)EVHistName= "Cutflow_SR3Fail";
  if(sr==SR3) EVHistName= "Cutflow_SR3";
  if(sr==MuonSRSummary)  EVHistName= "Cutflow_Summary_Muon";
  if(sr==SR3BDT) EVHistName= "Cutflow_SR3BDT";
  if(sr==PreselSS || sr==Presel ) EVHistName = "SS_Presel";
  if(sr==PreselOS || sr==Presel ) EVHistName ="OS_Presel";
  if(sr==ChannelDepInc )  EVHistName ="ChannelDependant_Inclusive";
  if(sr==ChannelDepIncQ ) EVHistName ="ChannelDependantQQ_Inclusive";
  if(sr==ChannelDepDilep)EVHistName ="ChannelDependantDilep";
  if(sr==ChannelDepTrigger) EVHistName ="ChannelDependantTrigger";
  if(sr==ChannelDepPresel)  EVHistName ="ChannelDependantPresel";
  if(sr==ChannelDepSR1)   EVHistName ="ChannelDependantSR1";
  if(sr==ChannelDepSR2)   EVHistName ="ChannelDependantSR2";
  if(sr==ChannelDepSR3)   EVHistName ="ChannelDependantSR3";
  if(sr==ChannelDepFAILSR3) EVHistName ="ChannelDependantSR3Fail";
  if(sr==SR) EVHistName ="SR_Summary";
  if(sr==ChannelDepCR1)   EVHistName ="ChannelDependantCR1";
  if(sr==ChannelDepCR2)   EVHistName ="ChannelDependantCR2";
  if(sr==ChannelDepCR3)   EVHistName ="ChannelDependantCR3";
  if(sr == ControlRegion) EVHistName = "ValidationRegionFlow";
  if(sr==MuonSRQQ )       EVHistName ="MuonChargeSplitSR";
  if(sr==ElectronSRQQ)    EVHistName ="ElectronChargeSplitSR";
  if(sr==ElectronMuonSRQQ)EVHistName ="ElectronMuonChargeSplitSR";
  if(sr==MuonCR         || sr==MuonCRBDT         )         EVHistName ="MuonCR";
  if(sr==MuonSR         || sr==MuonSRBDT         || sr==MuonSROpt         || sr==MuonSRBDTOpt)         EVHistName ="MuonSR";
  if(sr==ElectronSR     || sr==ElectronSRBDT     || sr==ElectronSROpt     || sr==ElectronSRBDTOpt)     EVHistName ="ElectronSR";
  if(sr==ElectronCR     || sr==ElectronCRBDT     )     EVHistName ="ElectronCR";
  if(sr==ElectronMuonSR || sr==ElectronMuonSRBDT || sr==ElectronMuonSROpt || sr==ElectronMuonSRBDTOpt) EVHistName ="ElectronMuonSR";
  if(sr==ElectronMuonCR || sr==ElectronMuonCRBDT ) EVHistName ="ElectronMuonCR";
  if(sr==CR)    EVHistName ="ControlRegions";
  if(sr==sigmm) EVHistName ="SR_CutFlow";
  if(sr==sigee) EVHistName ="SR_CutFlow";
  if(sr==sigem) EVHistName ="SR_CutFlow";
  if(sr==sigmm_17028) EVHistName ="SR_CutFlow_17028";
  if(sr==sigee_17028) EVHistName ="SR_CutFlow_17028";

  return EVHistName;
}


vector<TString>  HNL_LeptonCore::GetLabelsFromRegion(HNL_LeptonCore::SearchRegion sr){
  
  vector<TString> labels;
  
  int nstep(0);
  if(sr==WGCR)  nstep = 10;
  if(sr==ZGCR)  nstep = 10;
  if(sr==WZCR)  nstep = 11;
  if(sr==ZZCR)  nstep = 5;
  if(sr==ZZCR2)  nstep = 8;
  if(sr==WZBCR)   nstep = 10; 
  if(sr==ZZCR)   nstep = 10;
  if(sr==ZZCR2)  nstep = 10;
  if(sr==WGCR)   nstep = 10;
  if(sr==ZGCR)   nstep = 10;
  if(sr==WZBCR)   nstep = 10;
  if(sr==WZCR2)   nstep = 10;
  if(sr==HMCR1)   nstep = 10;
  if(sr==HMCR2)   nstep = 10;
  if(sr==HMCR3)   nstep = 10;
  if(sr==HMNPCR)   nstep = 10;
  if(sr==HMBCR)   nstep = 10;
  if(sr==HM1JCR)   nstep = 10;
  if(sr==PreselVBF)   nstep = 10;
  if(sr==Presel)   nstep = 10; 
  if(sr==WWNP1CR)   nstep = 10; 
  if(sr==WWNP2CR)   nstep = 10; 
  if(sr==WWNP3CR)   nstep = 10; 
  if(sr==WWCR1)   nstep = 10; 
  if(sr==WWCR2)   nstep = 10;
  if(sr==ZAK8CR)   nstep = 10;
  if(sr==ZCR)   nstep = 10; 
  if(sr==ZNPElCR)   nstep = 10; 
  if(sr==ZNPMuCR)   nstep = 10;
  if(sr==TopNPCR)   nstep = 10; 
  if(sr==TopCR)   nstep = 10; 
  if(sr==TopAK8NPCR)   nstep = 10; 
  if(sr==TopNPCR2)   nstep = 10;

  if(nstep > 0) {
    for(int i=0; i < nstep; i++) labels.push_back("Step"+to_string(i));
    return labels;
  }
  
  
  if(sr==SR1 )   labels = {  "SR1_Init","SR1_lep_charge","SR1_lep_pt","SR1_dilep_mass" , "SR1_tauveto","SR1_1AK8" ,"SR1_MET" ,"SR1_Wmass","SR1_bveto","SR1_Lep2Pt","SR1_PNTagger","SR1_LepPt","SR1_DphiN_Wlep","SR1_TauVeto","SR1_HTPt","SR1_masscuts" };
  if(sr==CR1 )   labels = {  "CR1_Init","CR1_lep_charge","CR1_lep_pt","CR1_dilep_mass" , "CR1_tauveto","CR1_1AK8" ,"CR1_MET" ,"CR1_Wmass","CR1_bveto","CR1_Lep2Pt","CR1_PNTagger","CR1_LepPt","CR1_DphiN_Wlep","CR1_TauVeto","CR1_HTPt","CR1_masscuts" };
  
  if(sr==SR2)  labels = {  "SR2_lep_charge",  "SR2_lep_pt",  "SR2_DPhi",  "SR2_LLMass", "SR2_DiJet", "SR2_DiJetEta", "SR2_DiJetMass","SR2_VBF","SR2_met","SR2_ht_lt1","SR2_bveto",  "SR2_Final"};
  if(sr==CR2)  labels = {  "CR2_lep_charge",  "CR2_lep_pt",  "CR2_DPhi",  "CR2_LLMass", "CR2_DiJet", "CR2_DiJetEta", "CR2_DiJetMass","CR2_VBF","CR2_met","CR2_ht_lt1","CR2_bveto",  "CR2_Final"};
  
  if( sr==SR3Fail) labels = {"SR3_2vl","SR3_mll","SR3_bveto","SR3_met", "SR3_dijet","SR3_0jpt","SR3_Wmass300","SR3_leppt", "SR3_Wmass400", "SR3_Nmass300"};
  
  if( sr==SR3) labels = { "SR3_lep_charge" ,"SR3_lep_pt", "SR3_dilep_mass","SR3_bveto","SR3_MET","SR3_0JetBin", "SR3_1JetBin", "SR3_jet", "SR3_dijet", "SR3_J1Pt", "SR3_W1Mass","SR3_LepPt" };
  if( sr==CR3) labels = { "CR3_lep_charge" ,"CR3_lep_pt", "CR3_dilep_mass","CR3_bveto","CR3_MET","CR3_0JetBin", "CR3_1JetBin", "CR3_jet", "CR3_dijet", "CR3_J1Pt", "CR3_W1Mass","CR3_LepPt" };
  
  if(sr==MuonSRSummary) labels = {"Inclusive", "GenMatch", "CheckLeptonFlavourForChannel","METFilter", "CFCut", "Preselection", "AK8","SigReg1", "SigReg1Fail",  "SigReg2",  "SigReg3", "SigReg3Pass", "SigReg3Fail"};
  
  if( sr==SR3BDT)  labels = {  "SR3_lep_charge" , "SR3_lep_pt",  "SR3_dilep_mass", "SR3_jet", "SR3_dijet","SR3_Wmass",  "SR3_J1Pt", "SR3_MET", "SR3_bveto"};
  
  if(sr==PreselSS || sr==Presel )   labels = {"NoCut", "METFilter", "Trigger", "Dilepton","SS_Dilep" ,"SS_lep_veto", "SS_Dilep_mass", "SS_Presel"};
  
  if(sr==PreselOS || sr==Presel )   labels = {"NoCut", "METFilter", "Trigger", "Dilepton", "OS_Dilep" ,"OS_lep_veto", "OS_Dilep_mass", "OS_Presel"};
  if(sr==ChannelDepInc )  labels = {"MuMu_NoCut","EE_NoCut","EMu_NoCut"};
  
  if(sr==ChannelDepIncQ ) labels = {"MuMu_MMQ_NoCut","MuMu_PPQ_NoCut", "EE_MMQ_NoCut",  "EE_PPQ_NoCut","EMu_MMQ_NoCut", "EMu_PPQ_NoCut"};
  
  if(sr==ChannelDepDilep)labels = {"MuMu_Dilep","EE_Dilep","EMu_Dilep"};
  
  if(sr==ChannelDepTrigger) labels = {"MuMu_Trigger","EE_Trigger","EMu_Trigger", "MuMu_MultiTrigger","EE_MultiTrigger","EMu_MultiTrigger"};
  
  if(sr==ChannelDepPresel) labels = {"MuMu_Presel","EE_Presel","EMu_Presel"};
  
  if(sr==ChannelDepSR1)   labels = {"MuMu_SR1","EE_SR1","EMu_SR1"};
  
  if(sr==ChannelDepSR2) labels = {"MuMu_SR2","EE_SR2","EMu_SR2"};
  
  if(sr==ChannelDepSR3) labels = {"MuMu_SR3","EE_SR3","EMu_SR3"};
  
  if(sr==ChannelDepFAILSR3) labels = {"MuMu_SR3Fail_0j","MuMu_SR3Fail_1j","MuMu_SR3Fail_2j", "EE_SR3Fail_0j","EE_SR3Fail_1j","EE_SR3Fail_2j","EMu_SR3Fail_0j","EMu_SR3Fail_1j","EMu_SR3Fail_2j"};
  
  if(sr==SR)   labels = {"SR1Pass","SR1Fail","SR2Pass","SR2Fail","SR3Pass","SR3Fail","SR4Pass","SR4Fail"  };
  
  if(sr==ChannelDepCR1)      labels = {"MuMu_CR1","EE_CR1","EMu_CR1"};
  if(sr==ChannelDepCR2)      labels = {"MuMu_CR2","EE_CR2","EMu_CR2"};
  if(sr==ChannelDepCR3)      labels = {"MuMu_CR3","EE_CR3","EMu_CR3"};
  if(sr == ControlRegion)    labels = {"NoCut","HEMVeto","METFilter","GENMatched","LeptonFlavour","Trigger","OS_VR","VV_VR","VG_VR","SS_CR","VBF_CR"};
  
  
  vector<TString> SRlabels = {"SR1_MNbin1","SR1_MNbin2","SR1_MNbin3","SR1_MNbin4","SR1_MNbin5", "SR1_MNbin6","SR1_MNbin7","SR1_MNbin8",			      "SR2_HTLTbin1", "SR2_HTLTbin2", 			      "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8", "SR3_bin9","SR3_bin10","SR3_bin11"};


  vector<TString> CRlabels = {"CR1_MNbin1","CR1_MNbin2","CR1_MNbin3","CR1_MNbin4","CR1_MNbin5", "CR1_MNbin6","CR1_MNbin7","CR1_MNbin8",			      "CR2_HTLTbin1", "CR2_HTLTbin2", 			      "CR3_bin1","CR3_bin2","CR3_bin3","CR3_bin4","CR3_bin5","CR3_bin6","CR3_bin7","CR3_bin8", "CR3_bin9","CR3_bin10","CR3_bin11"};

  vector<TString> SRQlabels=  {"QMSR1_MNbin1","QMSR1_MNbin2","QMSR1_MNbin3","QMSR1_MNbin4","QMSR1_MNbin5","QMSR1_MNbin6","QMSR1_MNbin7","QMSR2_HTLTbin1", "QMSR2_HTLTbin2",  "QMSR3_bin1","QMSR3_bin2","QMSR3_bin3","QMSR3_bin4","QMSR3_bin5","QMSR3_bin6","QMSR3_bin7",  "QPSR1_MNbin1","QPSR1_MNbin2","QPSR1_MNbin3","QPSR1_MNbin4","QPSR1_MNbin5","QPSR1_MNbin6","QPSR1_MNbin7","QPSR2_HTLTbin1","QPSR2_HTLTbin2",  "QPSR3_bin1","QPSR3_bin2","QPSR3_bin3","QPSR3_bin4","QPSR3_bin5","QPSR3_bin6","QPSR3_bin7"};

  vector<TString> SRBDTlabels=  {"SR1_MNbin1","SR1_MNbin2","SR1_MNbin3","SR1_MNbin4","SR1_MNbin5","SR1_MNbin6","SR1_MNbin7","SR1_MNbin8",				 "SR2_HTLTbin1", "SR2_HTLTbin2",				 "SR3_BDTbin1","SR3_BDTbin2","SR3_BDTbin3","SR3_BDTbin4","SR3_BDTbin5","SR3_BDTbin6","SR3_BDTbin7","SR3_BDTbin8"};

  vector<TString> CRBDTlabels=  {"CR1_MNbin1","CR1_MNbin2","CR1_MNbin3","CR1_MNbin4","CR1_MNbin5","CR1_MNbin6","CR1_MNbin7","CR1_MNbin8",				 "CR2_HTLTbin1", "CR2_HTLTbin2",				 "CR3_BDTbin1","CR3_BDTbin2","CR3_BDTbin3","CR3_BDTbin4","CR3_BDTbin5","CR3_BDTbin6","CR3_BDTbin7","CR3_BDTbin8"};
  
  
  vector<TString> SRlabelsOpt = {"SR1_MNbin1","SR1_MNbin2","SR1_MNbin3","SR1_MNbin4",
				 "SR2_HTLTbin1", "SR2_HTLTbin2",
				 "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8", "SR3_bin9","SR3_bin10","SR3_bin11"};
  vector<TString> SRBDTlabelsOpt=  {"SR1_MNbin1","SR1_MNbin2","SR1_MNbin3","SR1_MNbin4",
				    "SR2_HTLTbin1", "SR2_HTLTbin2",
				    "SR3_BDTbin1","SR3_BDTbin2","SR3_BDTbin3","SR3_BDTbin4","SR3_BDTbin5","SR3_BDTbin6","SR3_BDTbin7"};
  
  ///// STANDARD ANALYSIS LIMIT LABELS                                                                                                                                                                                                                                                                                                                                                                                                                                                     
  if(sr==MuonSR    || sr==ElectronSR    || sr==ElectronMuonSR)       labels = SRlabels;

  if(sr==MuonCR    || sr==ElectronCR    || sr==ElectronMuonCR)       labels = CRlabels;
  ///// STANDARD ANALYSIS LIMIT LABELS WITH CHARGE SPLIT                                                                                                                                                                                                                              
  
  if(sr==MuonSRQQ  || sr==ElectronSRQQ  || sr==ElectronMuonSRQQ)     labels = SRQlabels;
  ///// STANDARD ANALYSIS LIMIT LABELS FOR BDT MASSES                                                                                                                                                                                                                                                                                                                                                                                                                                      
  if(sr==MuonSRBDT || sr==ElectronSRBDT || sr==ElectronMuonSRBDT)    labels = SRBDTlabels ;
  if(sr==MuonCRBDT || sr==ElectronCRBDT || sr==ElectronMuonCRBDT)    labels = CRBDTlabels ;
  
  //// OPTIMISING LIMIT PLOTS                                                                                                                                                                                                                                                                                                                                                                                                                                                              
  if(sr==MuonSRBDTOpt || sr==ElectronSRBDTOpt || sr==ElectronMuonSRBDTOpt)    labels = SRBDTlabelsOpt ;
  if(sr==MuonSROpt    || sr==ElectronSROpt    || sr==ElectronMuonSROpt)       labels = SRlabelsOpt;

  //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
  if(sr==CR)   labels = { "Z_CR","Top_CR","Top_CR2", "TopAK8_CR","ZAK8_CR",
			  "WpWp_CR1","WpWp_CR2","WpWp_CRNP","WpWp_CRNP2","WpWp_CRNP3",
			  "WZ_CR", "ZZ_CR","ZZOffShell_CR","ZG_CR","WG_CR","WZ2_CR","WZB_CR", "ZNPEl_CR", "ZNPMu_CR", "TopNP_CR",
			  "HighMassSR1_CR","HighMassSR2_CR","HighMassSR3_CR" ,"HighMass1Jet_CR","HighMassBJet_CR","HighMassNP_CR",
			  "SSPresel"};
  
  
  if(sr==sigmm)  labels = {"SSNoCut", "SSGen", "SSGen2", "SSMuMuTrig", "SSMuMuTrig2", "SSMuMuTrig2L", "SSMuMu", "SSMuMu_Pt", "SSMuMu_HEMVeto","SSMuMu_LepVeto", "SSMuMu_LLMass",  "SSMuMu_vTau","SSMuMu_Jet","SSMuMu_BJet", "SSMuMu_DiJet",  "SSMuMu_SR1", "SSMuMu_SR2","SSMuMu_SR3Inclusive","SSMuMu_SR3","SSMuMu_SR4","SSMuMu_SRFail"};
  
  if(sr==sigmm_17028) labels = {"SSNoCut", "SSGen","SSGen2", "SSMuMuTrig", "SSMuMuTrig2","SSMuMuTrig2L" , "SSMuMu","SSMuMu_Pt", "SSMuMu_HEMVeto","SSMuMu_LepVeto", "SSMuMu_LLMass", "SSMuMu_Jet", "SSMuMu_BJet", "SSMuMu_DiJet",  "SSMuMu_SR1","SSMuMu_SR3", "SSMuMu_SRFail"};

  if(sr==sigee) labels = {"SSNoCut", "SSGen", "SSGen2", "SSEETrig", "SSEETrig2", "SSEETrig2L","SSEE", "SSEE_Pt","SSEE_LepVeto", "SSEE_HEMVeto","SSEE_LLMass",  "SSEE_vTau","SSEE_Jet","SSEE_BJet", "SSEE_DiJet",  "SSEE_SR1","SSEE_SR2","SSEE_SR3Inclusive","SSEE_SR3","SSEE_SR4", "SSEE_SRFail"};
  
  if(sr==sigee_17028) labels = {"SSNoCut", "SSGen","SSGen2", "SSEETrig", "SSEETrig2", "SSEETrig2L", "SSEE","SSEE_Pt", "SSEE_HEMVeto", "SSEE_LepVeto", "SSEE_LLMass", "SSEE_Jet", "SSEE_BJet", "SSEE_DiJet",  "SSEE_SR1","SSEE_SR3","SSEE_SRFail"};
    
  if(sr==sigem)   labels = {"SSNoCut", "SSGen", "SSGen2", "SSEMuTrig", "SSEMuTrig2","SSEMuTrig2L","SSEMu", "SSEMu_Pt","SSEMu_LepVeto", "SSEMu_LLMass",  "SSEMu_vTau","SSEMu_Jet","SSEMu_BJet", "SSEMu_DiJet",  "SSEMu_SR1","SSEMu_SR1Fail", "SSEMu_SR2","SSEMu_SR3","SSEMu_SR4","SSEMu_SR5","SSEMu_SR3Fail"};
  
  if(sr==sigem_17028)  labels = {"SSNoCut", "SSGen","SSGen2", "SSEMuTrig", "SSEMuTrig2", "SSEMuTrig2L", "SSEMu","SSEMu_Pt", "SSEMu_LepVeto", "SSEMu_LLMass", "SSEMu_Jet", "SSEMu_BJet", "SSEMu_DiJet",  "SSEMu_SR1","SSEMu_SR1Fail","SSEMu_SR3","SSEMu_SR3Fail"};
  
  return labels;
  
}




