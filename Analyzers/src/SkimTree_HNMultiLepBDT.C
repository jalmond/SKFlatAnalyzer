#include "SkimTree_HNMultiLepBDT.h"

void SkimTree_HNMultiLepBDT::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNMultiLepBDT::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  SKWeight=0;
  electron_ptratio = 0;
  electron_ptrel  = 0;
  electron_cj_bjetdisc = 0;
  electron_mva_cf = 0;
  electron_mva_conv = 0;
  electron_mva_fake = 0 ;

  muon_mva_conv = 0;
  muon_ptratio = 0;
  muon_ptrel  = 0;
  muon_cj_bjetdisc = 0;

  newtree->Branch("electron_ptrel",&electron_ptrel);
  newtree->Branch("electron_ptratio",&electron_ptratio);
  newtree->Branch("electron_cj_bjetdisc",&electron_cj_bjetdisc);
  newtree->Branch("electron_mva_cf",&electron_mva_cf);
  newtree->Branch("electron_mva_conv",&electron_mva_conv);
  newtree->Branch("electron_mva_fake",&electron_mva_fake);

  newtree->Branch("muon_ptrel",&muon_ptrel);
  newtree->Branch("muon_ptratio",&muon_ptratio);
  newtree->Branch("muon_cj_bjetdisc",&muon_cj_bjetdisc);
  newtree->Branch("muon_mva_conv",&muon_mva_conv);

  newtree->Branch("SKWeight", &SKWeight, 'SKWeight/F');
  
    

  triggers_dimu.clear();

  triggers.clear();

  if(GetEraShort()=="2016a"){
    triggers_dimu = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
    };
    
    triggers_emu = {
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v",      
      "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v",
    };
  }

  if(GetEraShort()=="2016b"){
    triggers_dimu = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
    };

    triggers_emu = {
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"
    };
  }

  if(DataYear==2016){

    triggers_di_el = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
    };

    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
      "HLT_IsoMu24_v",
      "HLT_IsoTkMu24_v",
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
      "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
      "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",              // H
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v",     // B-G
      "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",    // B-G
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",  // H
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",  // H
      "HLT_Ele27_WPTight_Gsf_v",
      "HLT_Ele25_eta2p1_WPTight_Gsf_v" ,
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_Photon175_v",
      "HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v",
      "HLT_Photon175_v",
      "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v",
      "HLT_Photon175_v",
      "HLT_DoubleEle33_CaloIdL_MW_v",
      "HLT_DoubleEle37_Ele27_CaloIdL_GsfTrkIdVL_v",
      "HLT_Photon175_v",
      "HLT_DoublePhoton60_v"   
    };
  }
  else if(DataYear==2017){
    triggers_dimu = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v"

    };
    triggers_di_el = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"
    };

    triggers_emu = {
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
    };
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",                  // B
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
      "HLT_IsoMu27_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",      // C-F
      "HLT_Ele32_WPTight_Gsf_v",
      "HLT_Ele32_WPTight_Gsf_L1DoubleEG_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_Photon200_v",
      "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v",
      "HLT_Photon200_v",
      "HLT_DoubleEle33_CaloIdL_MW_v",
      "HLT_Ele35_WPTight_Gsf_v",
      "HLT_Ele32_WPTight_Gsf_v", 
      "HLT_Ele32_WPTight_Gsf_L1DoubleEG_v",
      "HLT_Photon200_v",
      "HLT_DoublePhoton70_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",

    };
  }
  else if(DataYear==2018){
    triggers_dimu = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v"
    };
    
    triggers_di_el = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"
    };
    triggers_emu = {			 
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"
    };


    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
      "HLT_IsoMu24_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Ele32_WPTight_Gsf_v",
      "HLT_DoubleEle25_CaloIdL_MW_v" ,
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v",
      "HLT_Photon200_v", 
      "HLT_DoublePhoton70_v"
    };
  }
  else{
    cout << "[SkimTree_HNMultiLepBDT::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_HNMultiLepBDT::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_HNMultiLepBDT::initializeAnalyzer]   " << triggers.at(i) << endl;
  }



  //SetupIDMVAReader(false);
  //SetupIDMVAReader(true);
}

void SkimTree_HNMultiLepBDT::executeEvent(){

  Event ev;

  //=============================
  //==== If survived, fill tree
  //=============================

  electron_ptrel->clear();
  electron_ptratio->clear();
  electron_cj_bjetdisc->clear();
  electron_mva_cf->clear();
  electron_mva_conv->clear();
  electron_mva_fake->clear();

  muon_mva_conv->clear();
  muon_ptrel->clear();
  muon_ptratio->clear();
  muon_cj_bjetdisc->clear();


  SKWeight=MCweight(true,true);
  
  std::vector<Muon>     AllmuonColl     = GetAllMuons();
  std::vector<Electron> AllelectronColl = GetAllElectrons();


  std::vector<Jet>    AK4_JetAllColl = GetJets("NoID", 10., 5.0);
  
  for(auto i: AllmuonColl){
    muon_mva_conv->push_back(GetBDTScoreMuon(i,AnalyzerCore::Conv,  "BDTG"));
    muon_ptratio->push_back(JetLeptonPtRatioLepAware(i,false));
    muon_ptrel->push_back(JetLeptonPtRelLepAware(i,true));
    
    float  JetDiscCJ = -999;
    
    int IdxMatchJet=-1;
    float mindR1=999.;
    
    for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
      float dR1=i.DeltaR(AK4_JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
    }
    if(IdxMatchJet!=-1)    JetDiscCJ = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
    else JetDiscCJ=0.;
    muon_cj_bjetdisc->push_back(JetDiscCJ);
  }
  
  for(auto i: AllelectronColl){
    electron_mva_fake->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTG"));
    electron_mva_cf->push_back(GetBDTScoreEl(i,AnalyzerCore::CF,  "BDTG"));
    electron_mva_conv->push_back(GetBDTScoreEl(i,AnalyzerCore::Conv,  "BDTG"));
    electron_ptratio->push_back(JetLeptonPtRatioLepAware(i,false));
    electron_ptrel->push_back(JetLeptonPtRelLepAware(i,true));
    
    float  JetDiscCJ = -999;

    int IdxMatchJet=-1;
    float mindR1=999.;

    for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
      float dR1=i.DeltaR(AK4_JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
    }
    if(IdxMatchJet!=-1)    JetDiscCJ = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
    else JetDiscCJ=0.;
    electron_cj_bjetdisc->push_back(JetDiscCJ);


  }


  newtree->Fill();

}

void SkimTree_HNMultiLepBDT::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNMultiLepBDT::SkimTree_HNMultiLepBDT(){

  newtree = NULL;

}

SkimTree_HNMultiLepBDT::~SkimTree_HNMultiLepBDT(){

}

void SkimTree_HNMultiLepBDT::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
