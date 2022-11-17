#include "HNL_LepIDKinVar.h"

void HNL_LepIDKinVar::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

  InitializeTreeVars();
  
  tree_mm->Branch("Pt", &Pt, "Pt/F"); 
  tree_ee->Branch("Pt", &Pt, "Pt/F");
  tree_mm->Branch("Eta", &Eta, "Eta/F");
  tree_ee->Branch("Eta", &Eta, "Eta/F");

  tree_mm->Branch("MiniIsoChHad", &MiniIsoChHad, "MiniIsoChHad/F"); 
  tree_ee->Branch("MiniIsoChHad", &MiniIsoChHad, "MiniIsoChHad/F");
  tree_mm->Branch("MiniIsoNHad", &MiniIsoNHad, "MiniIsoNHad/F"); 
  tree_ee->Branch("MiniIsoNHad", &MiniIsoNHad, "MiniIsoNHad/F");
  tree_mm->Branch("MiniIsoPhHad", &MiniIsoPhHad, "MiniIsoPhHad/F");  
  tree_ee->Branch("MiniIsoPhHad", &MiniIsoPhHad, "MiniIsoPhHad/F");

  tree_mm->Branch("IsoChHad", &IsoChHad, "IsoChHad/F"); 
  tree_ee->Branch("IsoChHad", &IsoChHad, "IsoChHad/F");
  tree_mm->Branch("IsoNHad", &IsoNHad, "IsoNHad/F"); 
  tree_ee->Branch("IsoNHad", &IsoNHad, "IsoNHad/F");
  tree_mm->Branch("IsoPhHad", &IsoPhHad, "IsoPhHad/F"); 
  tree_ee->Branch("IsoPhHad", &IsoPhHad, "IsoPhHad/F");


  tree_mm->Branch("Dxy", &Dxy, "Dxy/F"); 
  tree_ee->Branch("Dxy", &Dxy, "Dxy/F");
  tree_mm->Branch("DxySig", &DxySig, "DxySig/F"); 
  tree_ee->Branch("DxySig", &DxySig, "DxySig/F");
  tree_mm->Branch("Dz", &Dz, "Dz/F"); 
  tree_ee->Branch("Dz", &Dz, "Dz/F");
  tree_mm->Branch("DzSig", &DzSig, "DzSig/F"); 
  tree_ee->Branch("DzSig", &DzSig, "DzSig/F");
  tree_mm->Branch("RelIso", &RelIso, "RelIso/F"); 
  tree_ee->Branch("RelIso", &RelIso, "RelIso/F");
  tree_mm->Branch("IP3D", &IP3D, "IP3D/F"); 
  tree_ee->Branch("IP3D", &IP3D, "IP3D/F");
  tree_mm->Branch("MVA", &MVA, "MVA/F"); 
  tree_ee->Branch("MVA", &MVA, "MVA/F");
  tree_ee->Branch("MVAIso", &MVAIso, "MVAIso/F");

  tree_mm->Branch("Chi2", &Chi2, "Chi2/F");  
  tree_mm->Branch("Validhits", &Validhits, "Validhits/I");  

  tree_mm->Branch("Matched_stations", &Matched_stations, "Matched_stations/I");

  tree_mm->Branch("Pixel_hits", &Pixel_hits, "Pixel_hits/I");

  tree_mm->Branch("Minireliso", &Minireliso, "Minireliso/F");
  
  tree_mm->Branch("Pixel_hits", &Pixel_hits, "Pixel_hits/I");

  tree_mm->Branch("Tracker_layers", &Tracker_layers, "Tracker_layers/I");

  tree_mm->Branch("POGTight", &POGTight, "POGTight/I");
  tree_mm->Branch("POGMedium", &POGMedium, "POGMedium/I");

  tree_ee->Branch("POGTight", &POGTight, "POGTight/I");
  tree_ee->Branch("POGMedium", &POGMedium, "POGMedium/I");


  tree_mm->Branch("PtRatio", &PtRatio, "PtRatio/F");         tree_ee->Branch("PtRatio", &PtRatio, "PtRatio/F");         
  tree_mm->Branch("PtRatioAJ", &PtRatioAJ, "PtRatioAJ/F");   tree_ee->Branch("PtRatioAJ", &PtRatioAJ, "PtRatioAJ/F");   
  tree_mm->Branch("PtRelv0", &PtRelv0, "PtRelv0/F");         tree_ee->Branch("PtRelv0", &PtRelv0, "PtRelv0/F");         
  tree_mm->Branch("PtRelv1", &PtRelv1, "PtRelv1/F");         tree_ee->Branch("PtRelv1", &PtRelv1, "PtRelv1/F");         
  tree_mm->Branch("CEMFracAJ", &CEMFracAJ, "CEMFracAJ/F");   tree_ee->Branch("CEMFracAJ", &CEMFracAJ, "CEMFracAJ/F");   
  tree_mm->Branch("NEMFracAJ", &NEMFracAJ, "NEMFracAJ/F");   tree_ee->Branch("NEMFracAJ", &NEMFracAJ, "NEMFracAJ/F");   
  tree_mm->Branch("CHFracAJ", &CHFracAJ, "CHFracAJ/F");      tree_ee->Branch("CHFracAJ", &CHFracAJ, "CHFracAJ/F");      
  tree_mm->Branch("NHFracAJ", &NHFracAJ, "NHFracAJ/F");      tree_ee->Branch("NHFracAJ", &NHFracAJ, "NHFracAJ/F");      
  tree_mm->Branch("MuFracAJ", &MuFracAJ, "MuFracAJ/F");      tree_ee->Branch("MuFracAJ", &MuFracAJ, "MuFracAJ/F");      
  tree_mm->Branch("JetDiscAJ", &JetDiscAJ, "JetDiscAJ/F");   tree_ee->Branch("JetDiscAJ", &JetDiscAJ, "JetDiscAJ/F");   
  tree_mm->Branch("CEMFracCJ", &CEMFracCJ, "CEMFracCJ/F");   tree_ee->Branch("CEMFracCJ", &CEMFracCJ, "CEMFracCJ/F");   
  tree_mm->Branch("NEMFracCJ", &NEMFracCJ, "NEMFracCJ/F");   tree_ee->Branch("NEMFracCJ", &NEMFracCJ, "NEMFracCJ/F");   
  tree_mm->Branch("CHFracCJ", &CHFracCJ, "CHFracCJ/F");      tree_ee->Branch("CHFracCJ", &CHFracCJ, "CHFracCJ/F");      
  tree_mm->Branch("NHFracCJ", &NHFracCJ, "NHFracCJ/F");      tree_ee->Branch("NHFracCJ", &NHFracCJ, "NHFracCJ/F");      
  tree_mm->Branch("MuFracCJ", &MuFracCJ, "MuFracCJ/F");      tree_ee->Branch("MuFracCJ", &MuFracCJ, "MuFracCJ/F");      
  tree_mm->Branch("JetDiscCJ", &JetDiscCJ, "JetDiscCJ/F");   tree_ee->Branch("JetDiscCJ", &JetDiscCJ, "JetDiscCJ/F");   

  tree_mm->Branch("JetNTrk", &JetNTrk, "JetNTrk/F");   tree_ee->Branch("JetNTrk", &JetNTrk, "JetNTrk/F");
  tree_mm->Branch("PileupJetId", &PileupJetId, "PileupJetId/F");   tree_ee->Branch("PileupJetId", &PileupJetId, "PileupJetId/F");


  tree_ee->Branch("MissingHits", &MissingHits, "MissingHits/I");
  tree_ee->Branch("Full5x5_sigmaIetaIeta", &Full5x5_sigmaIetaIeta, "Full5x5_sigmaIetaIeta/F");
  tree_ee->Branch("sigmaIetaIeta", &sigmaIetaIeta, "sigmaIetaIeta/F");
  
  tree_ee->Branch("dEtaSeed", &dEtaSeed, "dEtaSeed/F");
  
  tree_ee->Branch("dPhiIn", &dPhiIn, "dPhiIn/F");
  tree_ee->Branch("dEtaIn", &dEtaIn, "dEtaIn/F");
  
  tree_ee->Branch("HoverE", &HoverE, "HoverE/F");

  tree_ee->Branch("TrkIso", &TrkIso, "TrkIso/F");

  tree_ee->Branch("isEcalDriven", &isEcalDriven, "isEcalDriven/I");

  tree_ee->Branch("InvEminusInvP", &InvEminusInvP, "InvEminusInvP/F");

  tree_ee->Branch("PassConversionVeto", &PassConversionVeto, "PassConversionVeto/I");

  tree_ee->Branch("ecalPFClusterIso", &ecalPFClusterIso, "ecalPFClusterIso/F");

  tree_ee->Branch("hcalPFClusterIso", &hcalPFClusterIso, "hcalPFClusterIso/F");

  tree_ee->Branch("IsGsfCtfScPixChargeConsistent", &IsGsfCtfScPixChargeConsistent, "IsGsfCtfScPixChargeConsistent/I");
  tree_ee->Branch("IsGsfScPixChargeConsistent", &IsGsfScPixChargeConsistent, "IsGsfScPixChargeConsistent/I");
  tree_ee->Branch("IsGsfCtfChargeConsistent", &IsGsfCtfChargeConsistent, "IsGsfCtfChargeConsistent/I");

  tree_mm->Branch("w_tot", &w_tot, "w_tot/F");                     tree_ee->Branch("w_tot", &w_tot, "w_tot/F");                     

  outfile->cd();

}


void HNL_LepIDKinVar::executeEvent(){
  
  AnalyzerParameter param_bdt = HNL_LeptonCore::InitialiseHNLParameter("BDT", "");
  
  Event ev = GetEvent();
  double weight =SetupWeight(ev,param_bdt);
  
  if (MCSample.Contains("Type1")) {
    weight*=1/xsec;
  }

  FillHist("CutFlow", 0., weight, 20, 0., 20.);
  
  if(!PassMETFilter()) return;

  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};
  
  for(auto dilep_channel : channels){

    std::vector<Muon>       MuonCollT     =  GetMuons    ( param_bdt,param_bdt.Muon_Veto_ID, 10., 2.4, RunFake);
    std::vector<Electron>   ElectronCollT =  GetElectrons( param_bdt,param_bdt.Electron_Veto_ID, 10., 2.5, RunFake);

 
    if(dilep_channel == EE) MuonCollT.clear();
    if(dilep_channel == MuMu) ElectronCollT.clear();

    TString OptionEl="CFHFakeNHConv";
    TString OptionMu="HFakeNHConv";
    gens = GetGens();

    std::vector<Electron>   ElectronCollTSkim  = (MCSample.Contains("TypeI")) ? ElectronCollT :  SkimLepColl(ElectronCollT, gens, param_bdt, OptionEl);
    std::vector<Muon>       MuonCollTSkim      = (MCSample.Contains("TypeI")) ? MuonCollT :  SkimLepColl(MuonCollT, gens, param_bdt, OptionMu);
   



    std::vector<Lepton *> LepsT  = MakeLeptonPointerVector(MuonCollTSkim,ElectronCollTSkim);
    

    if (!PassTriggerSelection(dilep_channel, ev, LepsT, "Dilep")) continue;

    std::vector<Jet>    AK4_JetAllColl = GetJets("NoID", 10., 3.0);


    JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
    

    std::vector<Electron>   ElectronCollV = GetElectrons(param_bdt.Electron_Veto_ID, 10., 2.5);
    std::vector<Muon>       MuonCollV     = GetMuons    (param_bdt.Muon_Veto_ID, 5., 2.4);
    std::vector<FatJet> fatjets_tmp    = GetFatJets("tight", 200., 5);

    std::vector<FatJet> AK8_JetColl    = GetAK8Jets(fatjets_tmp, 200., 2.7, true, 1., false, -999, false, 40., 130., ElectronCollV, MuonCollV);

    std::vector<Jet>    jets_tmp       = GetJets("tight", 15., 5);
    std::vector<Jet> bjets_tmp                  = GetAK4Jets(jets_tmp,     20., 2.4, true, 0.4, 0.8, "", ElectronCollV, MuonCollV, AK8_JetColl);

    std::vector<Jet> BJetColl         = GetBJets(param_bdt, bjets_tmp, param_jets);
    double sf_btag                    = GetBJetSF(param_bdt, bjets_tmp, param_jets);
    if(!IsData )weight*= sf_btag;
    if(BJetColl.size() > 0) return;
    Particle METv = GetvMET("PuppiT1xyULCorr",param_bdt);
    if(METv.Pt() > 70.) return;

    MakeTreeSS2L(dilep_channel, LepsT,MuonCollTSkim,ElectronCollTSkim, AK4_JetAllColl,  weight, "");

  }
  
}

void HNL_LepIDKinVar::MakeTreeSS2L(HNL_LeptonCore::Channel lep_channel,vector<Lepton *>  LepTColl, 
				   vector<Muon> Muons, 
				   vector<Electron> Electrons,
				   vector<Jet>& JetAllColl,
				   float weight, TString Label)
{
  

  
  // CorrectChannelStream checks EE channel has only EE events
  if(!CheckLeptonFlavourForChannel(lep_channel, LepTColl)) return;

  // Pt cut is set in CheckLeptonFlavourForChannel
  //if(!(LepTColl[0]->Pt()>20 && LepTColl[1]->Pt()>10)) return;

  float Mll = GetLLMass(LepTColl);
  if (lep_channel==EE && (fabs(Mll-90.) < 10.)) return;
  
  InitializeTreeVars();
  
  int imu(0), iel(0);

  for(auto lep : LepTColl){

    Pt    = lep->Pt();
    Eta   = fabs(lep->Eta());

    MiniIsoChHad = lep->MiniIsoChHad();
    MiniIsoNHad = lep->MiniIsoNHad();
    MiniIsoPhHad = lep->MiniIsoPhHad();
    IsoChHad = lep->IsoChHad();
    IsoNHad = lep->IsoNHad();
    IsoPhHad = lep->IsoPhHad();


    Dxy   = lep->LogdXY();
    DxySig   = lep->LogdXYSig();
    Dz   = lep->LogdZ();
    DzSig   = lep->LogdZSig();

    IP3D    = lep->IP3D()/lep->IP3Derr();

    RelIso    =  lep->RelIso();
    
    Minireliso   = lep->MiniRelIso();

    
    if(lep->LeptonFlavour() == Lepton::ELECTRON) {
      
      MVA    = Electrons[iel].MVANoIsoResponse();

      MVAIso    = Electrons[iel].MVAIsoResponse();

      POGTight = Electrons[iel].Pass_CutBasedTightNoIso() ?  1: 0;
    
      POGMedium = Electrons[iel].Pass_CutBasedMediumNoIso()?  1: 0;
      
      
      MissingHits = Electrons[iel].NMissingHits();
    
      Full5x5_sigmaIetaIeta  = Electrons[iel].Full5x5_sigmaIetaIeta();
      sigmaIetaIeta  = Electrons[iel].sigmaIetaIeta();
      
      dEtaSeed  = Electrons[iel].dEtaSeed();

      dPhiIn  = Electrons[iel].dPhiIn();
      dEtaIn  = Electrons[iel].dEtaIn();

      EtaWidth = Electrons[iel].EtaWidth();
      PhiWidth = Electrons[iel].PhiWidth();
      e2x5OverE5x5 = Electrons[iel].e2x5OverE5x5();
      e1x5OverE5x5= Electrons[iel].e1x5OverE5x5();
      e15= Electrons[iel].e15();
      e25= Electrons[iel].e25();
      e55= Electrons[iel].e55();
      dr03EcalRecHitSumEt= Electrons[iel].dr03EcalRecHitSumEt();
      dr03HcalDepth1TowerSumEt= Electrons[iel].dr03HcalDepth1TowerSumEt();
      dr03HcalTowerSumEt= Electrons[iel].dr03HcalTowerSumEt();
      dr03TkSumPt= Electrons[iel].dr03TkSumPt();
      R9= Electrons[iel].R9();
      HoverE  = Electrons[iel].HoverE();
    
      TrkIso  = Electrons[iel].TrkIso();
      
      InvEminusInvP = fabs(Electrons[iel].InvEminusInvP());
      
      ecalPFClusterIso = Electrons[iel].ecalPFClusterIso()/Electrons[iel].UncorrPt();

      hcalPFClusterIso = Electrons[iel].hcalPFClusterIso()/Electrons[iel].UncorrPt();

      isEcalDriven = Electrons[iel].isEcalDriven();
      
      PassConversionVeto  = (Electrons[iel].PassConversionVeto()) ? 1 : 0;
      
      IsGsfCtfScPixChargeConsistent  = (Electrons[iel].IsGsfCtfScPixChargeConsistent()) ? 1 : 0;
      IsGsfScPixChargeConsistent  = (Electrons[iel].IsGsfScPixChargeConsistent()) ? 1 : 0;
      IsGsfCtfChargeConsistent  = (Electrons[iel].IsGsfCtfChargeConsistent()) ? 1 : 0;
      iel++;
      
    }
    else{
      
      MVA    = Muons[imu].MVA();
      
      
      POGTight = Muons[imu].isPOGTight() ?  1: 0;
      
      POGMedium = Muons[imu].isPOGMedium() ?  1: 0;
      
      Chi2 = Muons[imu].Chi2();
      
      Validhits = Muons[imu].ValidMuonHits();
      
      Matched_stations = Muons[imu].MatchedStations();
      
      Pixel_hits  = Muons[imu].PixelHits();
      
      Tracker_layers = Muons[imu].TrackerLayers();
      
      imu++;
    }

    //==== Vars for non-prompt lepton bkg
    int IdxMatchJet=-1, IdxMatchAwayJet=-1;
    float mindR1=999., maxdPhi1=-999.;
    
    for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
      
      float dPhi1 = fabs(TVector2::Phi_mpi_pi(lep->Phi() - JetAllColl.at(ij).Phi()));
      if(dPhi1>maxdPhi1){ maxdPhi1=dPhi1; IdxMatchAwayJet=ij; }
      float dR1=lep->DeltaR(JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
      
    }
  
    if(IdxMatchJet!=-1){

      PtRatio   = lep->Pt()/JetAllColl.at(IdxMatchJet).Pt();
      TLorentzVector JetNoLep1(JetAllColl.at(IdxMatchJet));
      JetNoLep1  -= *lep;
      PtRelv0   = lep->lep_jet_ptrel();
      PtRelv1   = lep->Perp(JetNoLep1.Vect());
      CEMFracCJ = JetAllColl.at(IdxMatchJet).ChargedEmEnergyFraction();
      NEMFracCJ = JetAllColl.at(IdxMatchJet).NeutralEmEnergyFraction();
      CHFracCJ  = JetAllColl.at(IdxMatchJet).ChargedHadEnergyFraction();
      NHFracCJ  = JetAllColl.at(IdxMatchJet).NeutralHadEnergyFraction();
      MuFracCJ  = JetAllColl.at(IdxMatchJet).MuonEnergyFraction();
      JetDiscCJ = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
      JetNTrk   = JetAllColl.at(IdxMatchJet).NVtxTracks();
      PileupJetId   = JetAllColl.at(IdxMatchJet).PileupJetId();
    }
    else{
      
      PtRatio=-1., PtRelv0=-1., PtRelv1=-1., CEMFracCJ=-1., NEMFracCJ=-1., CHFracCJ=-1., NHFracCJ=-1., MuFracCJ=-1., JetDiscCJ=-1.,JetNTrk=-1,PileupJetId=-1;
      
    }
    

    if(IdxMatchAwayJet!=-1){
      
      PtRatioAJ = JetAllColl.at(IdxMatchAwayJet).Pt()/lep->Pt();
      CEMFracAJ = JetAllColl.at(IdxMatchAwayJet).ChargedEmEnergyFraction();
      NEMFracAJ = JetAllColl.at(IdxMatchAwayJet).NeutralEmEnergyFraction();
      CHFracAJ  = JetAllColl.at(IdxMatchAwayJet).ChargedHadEnergyFraction();
      NHFracAJ  = JetAllColl.at(IdxMatchAwayJet).NeutralHadEnergyFraction();
      MuFracAJ  = JetAllColl.at(IdxMatchAwayJet).MuonEnergyFraction();
      JetDiscAJ = JetAllColl.at(IdxMatchAwayJet).GetTaggerResult(JetTagging::DeepJet);
      
    }
    else{
      
      PtRatioAJ=-1., CEMFracAJ=-1., NEMFracAJ=-1., CHFracAJ=-1., NHFracAJ=-1., MuFracAJ=-1., JetDiscAJ=-1.;
      
    }
    
    if(lep->LeptonFlavour() == Lepton::ELECTRON) cout <<  "Fill electron " << endl;
    w_tot     = !IsDATA? weight:-1.;
    if(lep->LeptonFlavour() == Lepton::MUON) tree_mm->Fill();
    if(lep->LeptonFlavour() == Lepton::ELECTRON)   tree_ee->Fill();
  }
  
}



void HNL_LepIDKinVar::executeEventFromParameter(AnalyzerParameter param){

  
}


void HNL_LepIDKinVar::InitializeTreeVars(){

  Pt=-1, Eta=-1;
  PtRatio=-1, PtRatioAJ=-1, PtRelv0=-1, PtRelv1=-1;
  CEMFracAJ=-1,NEMFracAJ=-1, CHFracAJ=-1, NHFracAJ=-1,  MuFracAJ=-1, JetDiscAJ=-1;
  CEMFracCJ=-1, NEMFracCJ=-1, CHFracCJ=-1, NHFracCJ=-1, MuFracCJ=-1, JetDiscCJ=-1,JetNTrk =-1,PileupJetId=-1;

  MiniIsoChHad=-1;
  MiniIsoNHad=-1;
  MiniIsoPhHad=-1;
  IsoChHad=-1;
  IsoNHad=-1;
  IsoPhHad=-1;

  Dxy=-1.;
  DxySig=-1.;
  Dz=-1.;
  DzSig=-1.;
  RelIso=-1.;
  IP3D=-1.;
  MVA=-1.;
  MVAIso=-1.;
  Chi2=-1.;
  Validhits=-1.;
  Matched_stations=-1.;
  Pixel_hits=-1.;
  Minireliso=-1.;
  Pixel_hits=-1.;
  Tracker_layers=-1.;
  MissingHits=-1.;
  Full5x5_sigmaIetaIeta=-1.;
  sigmaIetaIeta=-1.;
  dEtaSeed=-1.;
  dPhiIn=-1.;
  dEtaIn=-1.;
  EtaWidth=-1.;
  PhiWidth=-1.;
  HoverE=-1.;
  TrkIso=-1.;
  e2x5OverE5x5=-1;
  e1x5OverE5x5=-1;
  e15=-1;
  e25=-1;
  e55=-1;
  dr03EcalRecHitSumEt=-1;
  dr03HcalDepth1TowerSumEt=-1;
  dr03HcalTowerSumEt=-1;
  dr03TkSumPt=-1;
  
  InvEminusInvP=-1.;
  ecalPFClusterIso=-1.;
  hcalPFClusterIso=-1.;
  POGTight=-1;
  POGMedium = -1;
  isEcalDriven= -1;
  PassConversionVeto=-1 ;
  IsGsfCtfScPixChargeConsistent =-1;
  IsGsfScPixChargeConsistent =-1;
  IsGsfCtfChargeConsistent =-1;
  
  R9=-1;
  
  w_tot=-1;

}



HNL_LepIDKinVar::HNL_LepIDKinVar(){

  tree_mm = new TTree("Tree_mm", "Tree_mm");
  tree_ee = new TTree("Tree_ee", "Tree_ee");

}


HNL_LepIDKinVar::~HNL_LepIDKinVar(){

  delete tree_mm;
  delete tree_ee;

}


void HNL_LepIDKinVar::WriteHist(){

  AnalyzerCore::WriteHist();

  outfile->cd();
  tree_mm->Write();
  tree_ee->Write();
  outfile->cd();

}
