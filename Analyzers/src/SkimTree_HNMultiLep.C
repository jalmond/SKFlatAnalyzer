#include "SkimTree_HNMultiLep.h"

void SkimTree_HNMultiLep::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNMultiLep::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);


  vSKWeight=0;
  velectron_ptratio = 0;
  velectron_ptrel  = 0;
  velectron_cj_bjetdisc = 0;
  velectron_cj_flavour = 0;
  velectron_mva_cf = 0;
  velectron_mva_conv = 0;
  velectron_mva_fake = 0 ;

  vmuon_mva_fake = 0;
  vmuon_ptratio = 0;
  vmuon_ptrel  = 0;
  vmuon_cj_bjetdisc = 0;
  vmuon_cj_flavour = 0;

  newtree->Branch("electron_ptrel",&velectron_ptrel);
  newtree->Branch("electron_ptratio",&velectron_ptratio);
  newtree->Branch("electron_cj_bjetdisc",&velectron_cj_bjetdisc);
  newtree->Branch("electron_mva_cf",&velectron_mva_cf);
  newtree->Branch("electron_mva_conv",&velectron_mva_conv);
  newtree->Branch("electron_mva_fake",&velectron_mva_fake);
  newtree->Branch("electron_cj_flavour",&velectron_cj_flavour);

  newtree->Branch("muon_ptrel",&vmuon_ptrel);
  newtree->Branch("muon_ptratio",&vmuon_ptratio);
  newtree->Branch("muon_cj_bjetdisc",&vmuon_cj_bjetdisc);
  newtree->Branch("muon_mva_fake",&vmuon_mva_fake);
  newtree->Branch("muon_cj_flavour",&vmuon_cj_flavour);

  newtree->Branch("SKWeight", &vSKWeight);

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
    cout << "[SkimTree_HNMultiLep::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_HNMultiLep::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_HNMultiLep::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

}

void SkimTree_HNMultiLep::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);
  
  
  velectron_ptrel->clear();
  velectron_ptratio->clear();
  velectron_cj_bjetdisc->clear();
  velectron_mva_cf->clear();
  velectron_mva_conv->clear();
  velectron_mva_fake->clear();
  velectron_cj_flavour->clear();

  vmuon_mva_fake->clear();
  vmuon_ptrel->clear();
  vmuon_ptratio->clear();
  vmuon_cj_bjetdisc->clear();
  vmuon_cj_flavour->clear();


  vSKWeight=MCweight(true,true);
  
  std::vector<Muon>     AllmuonColl     = GetAllMuons();
  std::vector<Electron> AllelectronColl = GetAllElectrons();
  std::vector<Jet>    AK4_JetAllColl = GetJets("NoID", 10., 5.0);

  for(auto i: AllmuonColl){

    vmuon_mva_fake->push_back(GetBDTScoreMuon(i,AnalyzerCore::Fake,  "BDTG"));
    vmuon_ptratio->push_back(JetLeptonPtRatioLepAware(i,false));
    vmuon_ptrel->push_back(JetLeptonPtRelLepAware(i,true));

    float  JetDiscCJ = -999;
    int JetFlavourCJ=-999;

    int IdxMatchJet=-1;
    float mindR1=999.;

    for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
      float dR1=i.DeltaR(AK4_JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
    }
    if(IdxMatchJet!=-1) {
      JetDiscCJ = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
      JetFlavourCJ = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();
    }
    else {
      JetDiscCJ=1.5;
      JetFlavourCJ=0;
    }
    vmuon_cj_bjetdisc->push_back(JetDiscCJ);
    vmuon_cj_flavour->push_back(JetFlavourCJ);
  }
  
  for(auto i: AllelectronColl){
    velectron_mva_fake->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv1"));
    velectron_mva_cf->push_back(GetBDTScoreEl(i,AnalyzerCore::CF,  "BDTGv1"));
    velectron_mva_conv->push_back(GetBDTScoreEl(i,AnalyzerCore::Conv,  "BDTGv1"));
    velectron_ptratio->push_back(JetLeptonPtRatioLepAware(i,false));
    velectron_ptrel->push_back(JetLeptonPtRelLepAware(i,true));

    float  JetDiscCJ = -999;
    int  JetFlavourCJ = -999;

    int IdxMatchJet=-1;
    float mindR1=999.;

    for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
      float dR1=i.DeltaR(AK4_JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
    }
    if(IdxMatchJet!=-1) {
      JetDiscCJ = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
      JetFlavourCJ  = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();

    }
    else {
      JetDiscCJ=1.5;
      JetDiscCJ=0;

    }
    velectron_cj_bjetdisc->push_back(JetDiscCJ);
    velectron_cj_flavour->push_back(JetFlavourCJ);

    
    
  }
  
  
  
  if(MCSample.Contains("Type")){
    
    newtree->Fill();
    
    return;
  }
  

  //==== Skim 1 ) trigger
  if(! (ev.PassTrigger(triggers)) ) return;

  if(this->DataStream == "SingleMuon" && (ev.PassTrigger(triggers_dimu))) return;
  if(this->DataStream == "SingleElectron" && (ev.PassTrigger(triggers_di_el))) return;

  std::vector<Muon>     muonPreColl     = GetMuons("HNLoosest", 7., 2.4);
  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 8., 2.5);

  std::sort(muonPreColl.begin(), muonPreColl.end(), PtComparing);
  std::sort(electronPreColl.begin(), electronPreColl.end(), PtComparing);
  
  int NEl  = electronPreColl.size();
  int NMu  = muonPreColl.size();
  int NLep = NEl+NMu;
  bool HasSS2lOR3l = false;
  bool LeadLepPt = false;

  bool HasFatJet(false);
  
  vector<FatJet> allfatjets = puppiCorr->Correct( GetFatJets("tight", 200., 2.7) ); //==== corret SDMass                                                     

  HasFatJet = (allfatjets.size() > 0);

  std::vector<Lepton *> leps;
  for(unsigned int i=0; i<electronPreColl.size(); i++){
    Electron& el = electronPreColl.at(i);
    leps.push_back( &el );
  }
  for(unsigned int i=0; i<muonPreColl.size(); i++){
    Muon& mu = muonPreColl.at(i);
    leps.push_back( &mu );
  }

  if      ( NLep >= 3 ){ 
    HasSS2lOR3l = true; 
    if(NMu==0 ){
      if(electronPreColl.at(0).Pt() > 23.) LeadLepPt = true;
    }  
    else if(NEl==0){
      if(muonPreColl.at(0).Pt() > 17.) LeadLepPt = true;
    }
    else if((electronPreColl.at(0).Pt() > 23.) || (muonPreColl.at(0).Pt() > 17.))LeadLepPt = true;
    
  }
  else if ( NLep == 2 ){
    if(muonPreColl.size()==2){
      if (muonPreColl[0].Charge() == muonPreColl[1].Charge())   HasSS2lOR3l = true;
      else if (HasFatJet) HasSS2lOR3l = true;
    }
    else if(electronPreColl.size()==2){
      
      if( electronPreColl[0].Charge() == electronPreColl[1].Charge() ) HasSS2lOR3l = true;
      else if (HasFatJet) HasSS2lOR3l = true;
    }

    else if(electronPreColl.size() == 1 &&  muonPreColl.size() == 1){
      if( electronPreColl[0].Charge() ==  muonPreColl[0].Charge()) HasSS2lOR3l = true;
      else if (HasFatJet) HasSS2lOR3l = true;
    }
    
    if(NMu==2 && muonPreColl.at(0).Pt()>  17.    ) LeadLepPt = true;
    if(NEl==2 && electronPreColl.at(0).Pt()>23 ) LeadLepPt = true;
    if(NMu==1 && NEl==1 && electronPreColl.at(0).Pt()>23 ) LeadLepPt = true;
    if(NMu==1 && NEl==1 && muonPreColl.at(0).Pt()>23 ) LeadLepPt = true;
  }

  if( !(HasSS2lOR3l && LeadLepPt) ) return;

  //=============================
  //==== If survived, fill tree
  //=============================

  newtree->Fill();

}

void SkimTree_HNMultiLep::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNMultiLep::SkimTree_HNMultiLep(){

  newtree = NULL;

}

SkimTree_HNMultiLep::~SkimTree_HNMultiLep(){

}

void SkimTree_HNMultiLep::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
