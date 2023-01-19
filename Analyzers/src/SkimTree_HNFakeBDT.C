#include "SkimTree_HNFakeBDT.h"

void SkimTree_HNFakeBDT::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNFakeBDT::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
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



  triggers.clear();


  if(DataYear==2016){

    if(IsDATA){
      
      if (this->DataStream == "DoubleMuon"){    
	
	triggers = {
	  "HLT_Mu3_PFJet40_v",                             // DoubleMuon
	  "HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon
	  "HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon
	};
      } // // 2016 DMu DATA    
      
      if (this->DataStream == "DoubleEG"){
        triggers = {
	  "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // DoubleEG
	  "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // DoubleEG
	  "HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // DoubleEG
          //"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",        // DoubleEG
	  "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // DoubleEG
	};
      } // 2016EGMu DATA    
    } // 2016 DATA

    else{


      triggers = {
	"HLT_Mu50_v",
        "HLT_Mu3_PFJet40_v",                             // DoubleMuon
        "HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon
        "HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon
        "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // DoubleEG
        "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // DoubleEG
        "HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // DoubleEG
        //"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",        // DoubleEG
        "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // DoubleEG
      };

    } // 2016 MC
    
  } // 2016


  else if(DataYear==2017){
    

    if(IsDATA){
      
      if (this->DataStream == "SingleMuon"){
	triggers = { "HLT_Mu3_PFJet40_v","HLT_Mu50_v"};
	
      } // 2017 DATA SMu

      if (this->DataStream == "SingleElectron"){
	
	triggers = {
	  "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // SingleElectron
	  "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // SingleElectron
	  //"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",        // SingleElectron
	  "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // SingleElectron
	};
      }// 2017 DATA SEl       

      if (this->DataStream == "DoubleMuon"){

	triggers = {
          "HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon                                                                
          "HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon                                                                
        };

      } // 2017 DATA DMu       
    } // 2017
    
    else {
      
      
      triggers = {
	"HLT_Mu50_v",
	"HLT_Mu3_PFJet40_v",                             // SingleMuon
	"HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon
	"HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon
	"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // SingleElectron
	"HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // SingleElectron
	//"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",        // SingleElectron
	"HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // SingleElectron
      };
      cout << "Filling 2017 trigger " << endl;

    } // 2017 MC
    
  }
  else if(DataYear==2018){
    if(IsDATA){
   
      if (this->DataStream == "SingleMuon"){
      
	triggers = {"HLT_Mu3_PFJet40_v","HLT_Mu50_v"};
	
      }
      if (this->DataStream == "EGamma"){

        triggers = {
          "HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v", 
          "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",
          //"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",      
          "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v" 
        };
      }
      if (this->DataStream == "DoubleMuon"){
	triggers = {
	  "HLT_Mu8_TrkIsoVVL_v", 
	  "HLT_Mu17_TrkIsoVVL_v",
	};
	
      }
    } // 2018 DATA
    else {
      

      triggers = {
        "HLT_Mu50_v",
	"HLT_Mu3_PFJet40_v",                             // SingleMuon
	"HLT_Mu8_TrkIsoVVL_v",                           // DoubleMuon
	"HLT_Mu17_TrkIsoVVL_v",                          // DoubleMuon
	"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v",     // EGamma
	"HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v",    // EGamma
	//"HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v",        // EGamma
	"HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"     // EGamma
      };

    }

  }
  else{
    cout<<"[SkimTree_HNFakeBDT::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }


  cout << "[SkimTree_HNFakeBDT::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_HNFakeBDT::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

}

void SkimTree_HNFakeBDT::executeEvent(){

  Event ev;

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

  return;





}

void SkimTree_HNFakeBDT::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNFakeBDT::SkimTree_HNFakeBDT(){
  newtree=NULL;
}

SkimTree_HNFakeBDT::~SkimTree_HNFakeBDT(){

}

void SkimTree_HNFakeBDT::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
