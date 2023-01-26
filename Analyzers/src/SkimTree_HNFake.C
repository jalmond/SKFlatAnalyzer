#include "SkimTree_HNFake.h"

void SkimTree_HNFake::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNFake::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  //// ADD NEW VARIABLES NOT IN SKFLAT
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
    cout<<"[SkimTree_HNFake::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }


  cout << "[SkimTree_HNFake::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_HNFake::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

}

void SkimTree_HNFake::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);


  if(!(ev.PassTrigger(triggers))) return;


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
  std::vector<Jet>      AK4_JetAllColl = GetJets("NoID", 10., 5.0);
  
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
      JetDiscCJ=0.;
      JetFlavourCJ=-999;
    }
    vmuon_cj_bjetdisc->push_back(JetDiscCJ);
    vmuon_cj_flavour->push_back(JetFlavourCJ);
  }

  for(auto i: AllelectronColl){
    velectron_mva_fake->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTG"));
    velectron_mva_cf->push_back(GetBDTScoreEl(i,AnalyzerCore::CF,  "BDTG"));
    velectron_mva_conv->push_back(GetBDTScoreEl(i,AnalyzerCore::Conv,  "BDTG"));
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
      JetDiscCJ=0.;
      JetDiscCJ=-999;
      
    }
    velectron_cj_bjetdisc->push_back(JetDiscCJ);
    velectron_cj_flavour->push_back(JetFlavourCJ);
    
  }

  

  //// NOW Apply skim
  

  double mu_pt = (IsData) ? 4: 25;
  double el_pt = (IsData) ? 8: 25;

  Particle METv = GetvMET("PuppiT1xyULCorr");
  if(METv.Pt() > 40.) return;

  vector<Muon> allmuons = GetMuons("HNLoosest", mu_pt, 2.4);
  vector<Electron> allel = GetElectrons("HNLoosest", el_pt, 2.5);

  
  int NLep = allmuons.size() + allel.size();
  
  if( NLep == 0 ) return;

  vector<Jet> alljet = GetJets("tight", 30., 2.7);
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

  if( !dphi_lj ) return;
  

  newtree->Fill();
  return;





}

void SkimTree_HNFake::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNFake::SkimTree_HNFake(){
  newtree=NULL;
}

SkimTree_HNFake::~SkimTree_HNFake(){

}

void SkimTree_HNFake::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
