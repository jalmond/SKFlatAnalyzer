#include "SkimTree_NonIso.h"

void SkimTree_NonIso::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_NonIso::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  if( MCSample.Contains("DY") ){
    cout << "[SkimTree_NonIso::initializeAnalyzer()] This is DY sample, so saving gen_* and LHE_* for Z-pt reweighting" << endl;
  }
  else{
    cout << "[SkimTree_NonIso::initializeAnalyzer()] Throwing away gen_* and LHE_*" << endl;
    if(!IsDATA){
      newtree->SetBranchStatus("gen_*",0);
      newtree->SetBranchStatus("LHE_*",0);
      newtree->SetBranchStatus("gen_weight",1); // for MCweight()
    }
  }

  isSingleMu = HasFlag("isSingleMu");


  triggers.clear();
  if(DataYear==2016){
    if(IsData){
      if(isSingleMu) triggers = { "HLT_Mu50_v"};
      triggers = {
	"HLT_Mu3_PFJet40_v",                             // DoubleMuon
	"HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon
	"HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon
	"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // DoubleEG
	"HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // DoubleEG
	"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",          // DoubleEG
	"HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // DoubleEG
      };
    }
    else{
      triggers = {
        "HLT_Mu3_PFJet40_v",                             // DoubleMuon                                                                         
	"HLT_Mu50_v",

        "HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon                                                                         
        "HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon                                                                         
        "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // DoubleEG                                                                           
        "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // DoubleEG                                                                           
        "HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",          // DoubleEG                                                                           
        "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // DoubleEG                                                                           
      };

    }
  }
  else if(DataYear==2017){
    if(IsData){
      if(isSingleMu) triggers = { "HLT_Mu3_PFJet40_v","HLT_Mu50_v"};
      else{
	triggers = {
	  "HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon
	  "HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon
	  "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // SingleElectron
	  "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // SingleElectron
	  "HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",          // SingleElectron
	  "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // SingleElectron
	};
      }
    }
    else {
      triggers = {
	"HLT_Mu50_v",
	"HLT_Mu3_PFJet40_v",
	"HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon                                                                                                          
	"HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon                                                                                                          
	"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // SingleElectron                                                                                                      
	"HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // SingleElectron                                                                                                      
	"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",          // SingleElectron                                                                                                      
	"HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // SingleElectron                                                                                                      
      };

    }
  }
  else if(DataYear==2018){
    if(IsData){
      if(isSingleMu) triggers = {"HLT_Mu3_PFJet40_v","HLT_Mu50_v"};
      else {
	triggers = {
	  "HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon
	  "HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon
	  "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // EGamma
	  "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // EGamma
	  "HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",          // EGamma
	  "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // EGamma
	};
      }
    }
    else{
      triggers = {
	"HLT_Mu3_PFJet40_v",
	"HLT_Mu50_v",
	"HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon                                                                                                          
	"HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon                                                                                                          
	"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // EGamma                                                                                                              
	"HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // EGamma                                                                                                              
	"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",          // EGamma                                                                                                              
	"HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // EGamma                                                                                                              
      };

    }
  }
  else{
    cout << "[SkimTree_NonIso::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_NonIso::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_NonIso::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

  //==== WR (Z') : lepton pt > 53 (65) GeV
  LeptonPtCut = 40.;
  //==== WR (Z') : jet pt > 40 GeV
  AK4JetPtCut = 30.;
  //==== WR (Z') : Jet pt > 200 (300) GeV
  AK8JetPtCut = 170.;

  cout << "[SkimTree_NonIso::initializeAnalyzer] LeptonPtCut = " << LeptonPtCut << endl;
  cout << "[SkimTree_NonIso::initializeAnalyzer] AK4JetPtCut = " << AK4JetPtCut << endl;
  cout << "[SkimTree_NonIso::initializeAnalyzer] AK8JetPtCut = " << AK8JetPtCut << endl;

}

void SkimTree_NonIso::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  //==== Skim 1 ) trigger
  if(! (ev.PassTrigger(triggers)) ) return;

  //==== Skim 2) at least two leptons (e or mu) with pt > "LeptonPtCut"

  vector<Muon> allmuons = GetMuons("HNLoosest", 5., 2.4);
  vector<Electron> allel = GetElectrons("HNLoosest", 8., 2.4);

  int NLep = allmuons.size() + allel.size();
  
  vector<Jet> alljet = GetJets("HNTight", 30., 2.7);
  
  bool dphi_lj(false);
  for(unsigned int imu=0; imu < allmuons.size(); imu++){
    for(unsigned int ij=0; ij <alljet.size(); ij++){
      float dphi =fabs(TVector2::Phi_mpi_pi(allmuons[imu].Phi()- alljet.at(ij).Phi()));
      if(dphi > 2.5) dphi_lj=true;
    }
  }
  
  for(unsigned int iel=0; iel <allel.size(); iel++){
    for(unsigned int ij=0; ij <alljet.size(); ij++){
      float dphi =fabs(TVector2::Phi_mpi_pi(allel[iel].Phi()- alljet.at(ij).Phi()));
      if(dphi >2.5) dphi_lj=true;
    }
  }
  


  if( NLep == 0 ) return;
  if( !dphi_lj ) return;

  //=============================
  //==== If survived, fill tree
  //=============================

  newtree->Fill();

}

void SkimTree_NonIso::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_NonIso::SkimTree_NonIso(){

  newtree = NULL;

}

SkimTree_NonIso::~SkimTree_NonIso(){

}

void SkimTree_NonIso::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}


