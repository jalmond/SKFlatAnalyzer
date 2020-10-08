#include "SkimTree_SSHN.h"

void SkimTree_SSHN::initializeAnalyzer(){

  isSingleMu = HasFlag("isSingleMu");

  outfile->cd();
  cout << "[SkimTree_SSHN::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);
  
  //el_type = 0;
  //mu_type = 0;

  //  b_el_type = newtree->Branch("el_type",&el_type,"el_type");
  //b_mu_type = newtree->Branch("mu_type",&mu_type,"mu_type");

  if( MCSample.Contains("HN") ){
  cout << "[SkimTree_SSHN::initializeAnalyzer()] This is HN sample, so saving gen_* and LHE_* for Z-pt reweighting" << endl;
  }
  else{
    cout << "[SkimTree_SSHN::initializeAnalyzer()] Throwing away gen_* and LHE_*" << endl;
    /*if(!IsDATA){
      newtree->SetBranchStatus("gen_*",0);
      newtree->SetBranchStatus("LHE_*",0);
      newtree->SetBranchStatus("gen_weight",1); // for MCweight()
 
      newtree->Branch("el_type", &el_type);
      newtree->Branch("mu_type", &mu_type);
      }*/
  }


  triggers_dimu.clear();

  triggers.clear();
  if(DataYear==2016){
    triggers_dimu = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"
    };
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v",     // B-G
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",    // B-G
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",  // H
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"  // H
    };
  }
  else if(DataYear==2017){
    triggers_dimu = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v",
    };

    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"
    };
  }
  else if(DataYear==2018){
    triggers_dimu = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v",

    };
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"
    };
  }
  else{
    cout << "[SkimTree_SSHN::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_SSHN::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_SSHN::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

}

void SkimTree_SSHN::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  //==== Skim 1 ) trigger
  if(! (ev.PassTrigger(triggers)) ) return;

  if(isSingleMu && (ev.PassTrigger(triggers_dimu))) return;
  
  std::vector<Muon>     muonPreColl     = GetMuons("HNLoosest", 8., 2.4);
  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 8., 2.5);

  std::sort(muonPreColl.begin(), muonPreColl.end(), PtComparing);
  std::sort(electronPreColl.begin(), electronPreColl.end(), PtComparing);
  
  /*
  gens = GetGens();

  for(auto i : GetAllMuons()){
    if(!IsData){
      int type = GetLeptonType(i, gens);
      mu_type->push_back(type);
    }
  }
  for(auto i : GetAllElectrons()){
    if(!IsData){
      int type = GetLeptonType(i, gens);
      if(IsCF(i, gens)) type = 10;
      el_type->push_back(type);
    }
    }*/

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
      if(electronPreColl.at(0).Pt() > 23. && electronPreColl.at(1).Pt() > 12) LeadLepPt = true;
    }  
    else if(NEl==0){
      if(muonPreColl.at(0).Pt() > 17. && muonPreColl.at(1).Pt() > 8) LeadLepPt = true;
    }
    else LeadLepPt = true;
    
  }
  else if ( NLep == 2 ){
    if(SameCharge(muonPreColl) )   HasSS2lOR3l = true;
    else if (HasFatJet) HasSS2lOR3l = true;
    if(SameCharge(electronPreColl))    HasSS2lOR3l = true;
    else if (HasFatJet) HasSS2lOR3l = true;

  }
  
  if(NMu==2 && muonPreColl.at(0).Pt()>  17.    ) LeadLepPt = true;
  if(NEl==2 && electronPreColl.at(0).Pt()>23 && electronPreColl.at(1).Pt()>10) LeadLepPt = true;
  if(NMu==1 && NEl==1 && electronPreColl.at(0).Pt()>23 ) LeadLepPt = true;
  if(NMu==1 && NEl==1 && muonPreColl.at(0).Pt()>23 ) LeadLepPt = true;

  if( !(HasSS2lOR3l && LeadLepPt) ) return;

  //=============================
  //==== If survived, fill tree
  //=============================

  newtree->Fill();

  /*  if(!IsData){
    mu_type->clear();
    el_type->clear();
    }*/
}

void SkimTree_SSHN::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_SSHN::SkimTree_SSHN(){

  newtree = NULL;

}

SkimTree_SSHN::~SkimTree_SSHN(){

}

void SkimTree_SSHN::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}


