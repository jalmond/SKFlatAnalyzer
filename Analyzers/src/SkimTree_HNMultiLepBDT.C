#include "SkimTree_HNMultiLepBDT.h"

void SkimTree_HNMultiLepBDT::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNMultiLepBDT::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  SetupIDMVAReaderDefault();
  InitialiseLeptonBDTSKFlat();


  ///// ELECTRON BRANCHES                                                                                                                                                                                                                
  if(!fChain->GetBranch("electron_mva_cf_v2")){
    newtree->Branch("electron_mva_cf_v2",     &velectron_mva_cf_v2);
    newtree->Branch("electron_mva_cf_ed_v2",  &velectron_mva_cf_ed_v2);
    newtree->Branch("electron_mva_conv_v2",   &velectron_mva_conv_v2);
    newtree->Branch("electron_mva_conv_ed_v2",&velectron_mva_conv_ed_v2);

    newtree->Branch("electron_mva_fake_v4",   &velectron_mva_fake_v4);
    newtree->Branch("electron_mva_fakeHF_v4", &velectron_mva_fakeHF_v4);
    newtree->Branch("electron_mva_fakeHFB_v4",&velectron_mva_fakeHFB_v4);
    newtree->Branch("electron_mva_fakeHFC_v4",&velectron_mva_fakeHFC_v4);
    newtree->Branch("electron_mva_fakeLF_v4", &velectron_mva_fakeLF_v4);
    newtree->Branch("electron_mva_fakeTop_v4",&velectron_mva_fakeTop_v4);

    newtree->Branch("electron_mva_fake_ed_v4",   &velectron_mva_fake_ed_v4);
    newtree->Branch("electron_mva_fakeHF_ed_v4", &velectron_mva_fakeHF_ed_v4);
    newtree->Branch("electron_mva_fakeHFB_ed_v4",&velectron_mva_fakeHFB_ed_v4);
    newtree->Branch("electron_mva_fakeHFC_ed_v4",&velectron_mva_fakeHFC_ed_v4);
    newtree->Branch("electron_mva_fakeLF_ed_v4", &velectron_mva_fakeLF_ed_v4);
    newtree->Branch("electron_mva_fakeTop_ed_v4",&velectron_mva_fakeTop_ed_v4);
    newtree->Branch("electron_ptrel",      &velectron_ptrel);
    newtree->Branch("electron_ptratio",    &velectron_ptratio);
    newtree->Branch("electron_lepton_type",&velectron_lepton_type);
    newtree->Branch("electron_is_cf",      &velectron_is_cf);
  }

  newtree->Branch("electron_mva_fake_ed_v5",&velectron_mva_fake_ed_v5);
  newtree->Branch("electron_mva_fakeHFB_v5",&velectron_mva_fakeHFB_v5);
  newtree->Branch("electron_mva_fakeHFC_v5",&velectron_mva_fakeHFC_v5);
  newtree->Branch("electron_mva_fakeLF_v5", &velectron_mva_fakeLF_v5);
  newtree->Branch("electron_mva_conv_ed_v5",&velectron_mva_conv_ed_v5);
  newtree->Branch("electron_mva_cf_ed_v5",  &velectron_mva_cf_ed_v5 );
  newtree->Branch("electron_mva_cf_ed_v5pt",&velectron_mva_cf_ed_v5pt );

  newtree->Branch("electron_mva_fake_QCD_LFvsHF_v5",  &velectron_mva_fake_QCD_LFvsHF_v5);
  newtree->Branch("electron_mva_fake_QCD_HFBvsHFC_v5",&velectron_mva_fake_QCD_HFBvsHFC_v5);
  newtree->Branch("electron_mva_fake_QCD_LF1_v5",     &velectron_mva_fake_QCD_LF1_v5);
  newtree->Branch("electron_mva_fake_QCD_LF2_v5",     &velectron_mva_fake_QCD_LF2_v5);

  newtree->Branch("electron_v2_cj_bjetdisc",&velectron_v2_cj_bjetdisc);
  newtree->Branch("electron_v2_cj_cvsbjetdisc",&velectron_v2_cj_cvsbjetdisc);
  newtree->Branch("electron_v2_cj_cvsljetdisc",&velectron_v2_cj_cvsljetdisc);
  newtree->Branch("electron_v2_cj_flavour", &velectron_v2_cj_flavour);

  //// MUON BRANCH                                                                                                                                                                                                                                                                                                                                                          

  if(!fChain->GetBranch("muon_mva_fake_ed_v4")){
    newtree->Branch("muon_mva_fake_v4",   &vmuon_mva_fake_v4);
    newtree->Branch("muon_mva_fake_ed_v4",&vmuon_mva_fake_ed_v4);
    newtree->Branch("muon_ptrel",         &vmuon_ptrel);
    newtree->Branch("muon_ptratio",       &vmuon_ptratio);
    newtree->Branch("muon_lepton_type",   &vmuon_lepton_type);
    newtree->Branch("muon_is_cf",         &vmuon_is_cf);
    newtree->Branch("SKWeight", &vSKWeight);
  }

  newtree->Branch("muon_mva_fake_QCD_LFvsHF_v5",  &vmuon_mva_fake_QCD_LFvsHF_v5);
  newtree->Branch("muon_mva_fake_QCD_HFBvsHFC_v5",&vmuon_mva_fake_QCD_HFBvsHFC_v5);
  newtree->Branch("muon_mva_fake_QCD_LF1_v5",     &vmuon_mva_fake_QCD_LF1_v5);
  newtree->Branch("muon_mva_fake_QCD_LF2_v5",     &vmuon_mva_fake_QCD_LF2_v5);

  newtree->Branch("muon_v2_cj_bjetdisc",   &vmuon_v2_cj_bjetdisc);
  newtree->Branch("muon_v2_cj_cvsbjetdisc",&vmuon_v2_cj_cvsbjetdisc);
  newtree->Branch("muon_v2_cj_cvsljetdisc"   ,&vmuon_v2_cj_cvsljetdisc);
  newtree->Branch("muon_v2_cj_flavour",    &vmuon_v2_cj_flavour);



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

}

void SkimTree_HNMultiLepBDT::executeEvent(){


  Event ev;
  ev.SetTrigger(*HLT_TriggerName);
  if(!fChain->GetBranch("electron_mva_cf_v2"))  ResetLeptonBDTSKFlat();
  ResetLeptonBDTSKFlatV5();

  vSKWeight=MCweight(true,true);

  if(!fChain->GetBranch("electron_mva_cf_v2"))SetupLeptonBDTSKFlat();

  SetupLeptonBDTSKFlatV5();

  
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
