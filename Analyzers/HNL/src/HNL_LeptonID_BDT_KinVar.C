#include "HNL_LeptonID_BDT_KinVar.h"

void HNL_LeptonID_BDT_KinVar::initializeAnalyzer(){

  SeperateFakes = HasFlag("SeperateFake");
  SeperateConv = HasFlag("SeperateConv");
  SeperateCF = HasFlag("SeperateCF");
  SeperatePrompt = HasFlag("SeperatePrompt");
  
  HNL_LeptonCore::initializeAnalyzer();

  InitializeTreeVars();

  SetupTrees(tree_mm,tree_ee);
  //  SetupTrees(tree_mm_OB,tree_ee_OB);
  //SetupTrees(tree_mm_EC,tree_ee_EC);
  

}
void HNL_LeptonID_BDT_KinVar::SetupTrees(TTree* tree_mm, TTree* tree_ee){


  tree_mm->Branch("Pt", &Pt, "Pt/F"); 
  tree_ee->Branch("Pt", &Pt, "Pt/F");
  tree_mm->Branch("Eta", &Eta, "Eta/F");
  tree_ee->Branch("Eta", &Eta, "Eta/F");
  tree_ee->Branch("PileUp", &PileUp, "PileUp/F");
  tree_mm->Branch("PileUp", &PileUp, "PileUp/F");

  tree_mm->Branch("MiniIsoChHad", &MiniIsoChHad, "MiniIsoChHad/F"); 
  tree_ee->Branch("MiniIsoChHad", &MiniIsoChHad, "MiniIsoChHad/F");
  tree_mm->Branch("MiniIsoNHad", &MiniIsoNHad, "MiniIsoNHad/F"); 
  tree_ee->Branch("MiniIsoNHad", &MiniIsoNHad, "MiniIsoNHad/F");
  tree_mm->Branch("MiniIsoPhHad", &MiniIsoPhHad, "MiniIsoPhHad/F");  
  tree_ee->Branch("MiniIsoPhHad", &MiniIsoPhHad, "MiniIsoPhHad/F");

  tree_ee->Branch("RelMiniIsoCh", &RelMiniIsoCh, "RelMiniIsoCh/F");
  tree_ee->Branch("RelMiniIsoN", &RelMiniIsoN, "RelMiniIsoN/F");
  tree_mm->Branch("RelMiniIsoCh", &RelMiniIsoCh, "RelMiniIsoCh/F");
  tree_mm->Branch("RelMiniIsoN", &RelMiniIsoN, "RelMiniIsoN/F");


  tree_mm->Branch("IsoChHad", &IsoChHad, "IsoChHad/F"); 
  tree_ee->Branch("IsoChHad", &IsoChHad, "IsoChHad/F");
  tree_mm->Branch("IsoNHad", &IsoNHad, "IsoNHad/F"); 
  tree_ee->Branch("IsoNHad", &IsoNHad, "IsoNHad/F");
  tree_mm->Branch("IsoPhHad", &IsoPhHad, "IsoPhHad/F"); 
  tree_ee->Branch("IsoPhHad", &IsoPhHad, "IsoPhHad/F");


  tree_mm->Branch("Dxy", &Dxy, "Dxy/F"); 
  tree_ee->Branch("Dxy", &Dxy, "Dxy/F");
  tree_mm->Branch("RelDxy", &RelDxy, "RelDxy/F");
  tree_ee->Branch("RelDxy", &RelDxy, "RelDxy/F");
  tree_mm->Branch("DxySig", &DxySig, "DxySig/F"); 
  tree_ee->Branch("DxySig", &DxySig, "DxySig/F");
  tree_mm->Branch("Dz", &Dz, "Dz/F"); 
  tree_ee->Branch("Dz", &Dz, "Dz/F");
  tree_mm->Branch("RelDz", &RelDz, "RelDz/F");
  tree_ee->Branch("RelDz", &RelDz, "RelDz/F");
  tree_mm->Branch("DzSig", &DzSig, "DzSig/F"); 
  tree_ee->Branch("DzSig", &DzSig, "DzSig/F");
  tree_mm->Branch("RelIso", &RelIso, "RelIso/F"); 
  tree_ee->Branch("RelIso", &RelIso, "RelIso/F");
  tree_mm->Branch("IP3D", &IP3D, "IP3D/F"); 
  tree_ee->Branch("IP3D", &IP3D, "IP3D/F");
  tree_mm->Branch("RelIP3D", &RelIP3D, "RelIP3D/F");
  tree_ee->Branch("RelIP3D", &RelIP3D, "RelIP3D/F");

  tree_mm->Branch("MVA", &MVA, "MVA/F"); 
  tree_ee->Branch("MVA", &MVA, "MVA/F");
  tree_mm->Branch("RelMVA", &RelMVA, "RelMVA/F");
  tree_ee->Branch("RelMVA", &RelMVA, "RelMVA/F");

  tree_ee->Branch("MVAIso", &MVAIso, "MVAIso/F");
  tree_ee->Branch("RelMVAIso", &RelMVAIso, "RelMVAIso/F");

  tree_mm->Branch("Chi2", &Chi2, "Chi2/F");  
  tree_mm->Branch("Validhits", &Validhits, "Validhits/I");  

  tree_mm->Branch("Matched_stations", &Matched_stations, "Matched_stations/I");
  tree_mm->Branch("Pixel_hits", &Pixel_hits, "Pixel_hits/I");
  tree_mm->Branch("Minireliso", &Minireliso, "Minireliso/F");
  tree_mm->Branch("Pixel_hits", &Pixel_hits, "Pixel_hits/I");
  tree_mm->Branch("Tracker_layers", &Tracker_layers, "Tracker_layers/I");
  tree_mm->Branch("POGTight", &POGTight, "POGTight/I");
  tree_mm->Branch("POGMedium", &POGMedium, "POGMedium/I");
  tree_mm->Branch("HNTightID", &HNTightID, "HNTightID/I");
  tree_mm->Branch("MuonSetSegmentCompatibility", &MuonSetSegmentCompatibility, "MuonSetSegmentCompatibility/F");
  tree_ee->Branch("POGTight", &POGTight, "POGTight/I");
  tree_ee->Branch("POGMedium", &POGMedium, "POGMedium/I");
  tree_ee->Branch("HNTightID", &HNTightID, "HNTightID/I");
  tree_mm->Branch("PtRatio", &PtRatio, "PtRatio/F");         tree_ee->Branch("PtRatio", &PtRatio, "PtRatio/F");         
  tree_mm->Branch("PtRatioCorr", &PtRatioCorr, "PtRatioCorr/F");         tree_ee->Branch("PtRatioCorr", &PtRatioCorr, "PtRatioCorr/F");         
  tree_mm->Branch("PtRatioNoLep", &PtRatioNoLep, "PtRatioNoLep/F");         tree_ee->Branch("PtRatioNoLep", &PtRatioNoLep, "PtRatioNoLep/F");         
  tree_mm->Branch("PtRel", &PtRel, "PtRel/F");         tree_ee->Branch("PtRel", &PtRel, "PtRel/F");         
  tree_mm->Branch("MassDrop", &MassDrop, "MassDrop/F");         tree_ee->Branch("MassDrop", &MassDrop, "MassDrop/F");         
  tree_mm->Branch("MassDropNoLep", &MassDropNoLep, "MassDropNoLep/F");         tree_ee->Branch("MassDropNoLep", &MassDropNoLep, "MassDropNoLep/F");         
  tree_mm->Branch("PtRelCorr", &PtRelCorr, "PtRelCorr/F");         tree_ee->Branch("PtRelCorr", &PtRelCorr, "PtRelCorr/F");         
  tree_mm->Branch("PtRelWithLep", &PtRelWithLep, "PtRelWithLep/F");         tree_ee->Branch("PtRelWithLep", &PtRelWithLep, "PtRelWithLep/F");         
  tree_mm->Branch("CEMFracCJ", &CEMFracCJ, "CEMFracCJ/F");   tree_ee->Branch("CEMFracCJ", &CEMFracCJ, "CEMFracCJ/F");   
  tree_mm->Branch("NEMFracCJ", &NEMFracCJ, "NEMFracCJ/F");   tree_ee->Branch("NEMFracCJ", &NEMFracCJ, "NEMFracCJ/F");   
  tree_mm->Branch("CHFracCJ", &CHFracCJ, "CHFracCJ/F");      tree_ee->Branch("CHFracCJ", &CHFracCJ, "CHFracCJ/F");      
  tree_mm->Branch("NHFracCJ", &NHFracCJ, "NHFracCJ/F");      tree_ee->Branch("NHFracCJ", &NHFracCJ, "NHFracCJ/F");      
  tree_mm->Branch("MuFracCJ", &MuFracCJ, "MuFracCJ/F");      tree_ee->Branch("MuFracCJ", &MuFracCJ, "MuFracCJ/F");      
  tree_mm->Branch("JetDiscCJ", &JetDiscCJ, "JetDiscCJ/F");   tree_ee->Branch("JetDiscCJ", &JetDiscCJ, "JetDiscCJ/F");   

  tree_mm->Branch("JetNTrk", &JetNTrk, "JetNTrk/F");   tree_ee->Branch("JetNTrk", &JetNTrk, "JetNTrk/F");
  tree_mm->Branch("JetNMVATrk", &JetNMVATrk, "JetNMVATrk/F");   tree_ee->Branch("JetNMVATrk", &JetNMVATrk, "JetNMVATrk/F");
  tree_mm->Branch("PileupJetId", &PileupJetId, "PileupJetId/F");   tree_ee->Branch("PileupJetId", &PileupJetId, "PileupJetId/F");

  // Electron Specific variables
  tree_ee->Branch("MissingHits", &MissingHits, "MissingHits/I");
  tree_ee->Branch("Full5x5_sigmaIetaIeta", &Full5x5_sigmaIetaIeta, "Full5x5_sigmaIetaIeta/F");
  tree_ee->Branch("sigmaIetaIeta", &sigmaIetaIeta, "sigmaIetaIeta/F");
  tree_ee->Branch("dEtaSeed", &dEtaSeed, "dEtaSeed/F");
  tree_ee->Branch("dPhiIn", &dPhiIn, "dPhiIn/F");
  tree_ee->Branch("dEtaIn", &dEtaIn, "dEtaIn/F");
  tree_ee->Branch("HoverE", &HoverE, "HoverE/F");
  tree_ee->Branch("Rho", &Rho, "Rho/F");
  tree_ee->Branch("TrkIso", &TrkIso, "TrkIso/F");
  tree_ee->Branch("isEcalDriven", &isEcalDriven, "isEcalDriven/I");
  tree_ee->Branch("EoverP", &EoverP, "EoverP/I");
  tree_ee->Branch("FBrem", &FBrem, "FBrem/I");
  tree_ee->Branch("InvEminusInvP", &InvEminusInvP, "InvEminusInvP/F");
  tree_ee->Branch("PassConversionVeto", &PassConversionVeto, "PassConversionVeto/I");
  tree_ee->Branch("ecalPFClusterIso", &ecalPFClusterIso, "ecalPFClusterIso/F");
  tree_ee->Branch("hcalPFClusterIso", &hcalPFClusterIso, "hcalPFClusterIso/F");
  tree_ee->Branch("IsGsfCtfScPixChargeConsistent", &IsGsfCtfScPixChargeConsistent, "IsGsfCtfScPixChargeConsistent/I");
  tree_ee->Branch("IsGsfScPixChargeConsistent", &IsGsfScPixChargeConsistent, "IsGsfScPixChargeConsistent/I");
  tree_ee->Branch("IsGsfCtfChargeConsistent", &IsGsfCtfChargeConsistent, "IsGsfCtfChargeConsistent/I");
  
  tree_ee->Branch("EtaWidth",&EtaWidth,"EtaWidth/F");
  tree_ee->Branch("PhiWidth",&PhiWidth,"PhiWidth/F");
  tree_ee->Branch("e2x5OverE5x5",&e2x5OverE5x5,"e2x5OverE5x5/F");
  tree_ee->Branch("e1x5OverE5x5",&e1x5OverE5x5,"e1x5OverE5x5/F");
  tree_ee->Branch("e15",&e15,"e15/F");
  tree_ee->Branch("e25",&e25,"e25/F");
  tree_ee->Branch("e55",&e55,"e55/F");
  tree_ee->Branch("dr03EcalRecHitSumEt",&dr03EcalRecHitSumEt,"dr03EcalRecHitSumEt/F");
  tree_ee->Branch("dr03HcalDepth1TowerSumEt",&dr03HcalDepth1TowerSumEt,"dr03HcalDepth1TowerSumEt/F");
  tree_ee->Branch("dr03HcalTowerSumEt",&dr03HcalTowerSumEt,"dr03HcalTowerSumEt/F");
  tree_ee->Branch("dr03TkSumPt",&dr03TkSumPt,"dr03TkSumPt/F");
  tree_ee->Branch("R9",&R9,"R9/F");


  
  // Event weight
  tree_mm->Branch("w_tot", &w_tot, "w_tot/F"); 
  tree_ee->Branch("w_tot", &w_tot, "w_tot/F");                     
  


  outfile->cd();

}


void HNL_LeptonID_BDT_KinVar::executeEvent(){

  if(MCSample.Contains("TTLJ") && SeperatePrompt) {
    double r = ((double) rand() / (RAND_MAX));
    if(r > 0.12) return;
  }

  AnalyzerParameter param_bdt = HNL_LeptonCore::InitialiseHNLParameter("BDTTop", "");
  
  Event ev = GetEvent();

  gens = GetGens();

  double weight =SetupWeight(ev,param_bdt);
  vector<TString> MCMergeList = {"DY","WG"};

  
  if(!SeperateCF) weight *= MergeMultiMC(MCMergeList,"CombineAll");
  
  if(MCSample.Contains("Type")) weight = 1.;

  if(weight == 0) return;

  FillHist("CutFlow", 0, weight,  20, 0., 20.);
 
  if(!PassMETFilter()) return;

  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};
  
  for(auto dilep_channel : channels){

    std::vector<Muon>       MuonCollTAll     =  GetMuons    ( param_bdt,"MVAID", 10., 2.4, RunFake);
    std::vector<Electron>   ElectronCollTAll =  GetElectrons( param_bdt,"TopMVAID", 10., 2.5, RunFake);
 
    bool HasHEM(false);
    for(auto i: ElectronCollTAll) {
      if(FindHEMElectron(i)) HasHEM=true;
    }
    if(HasHEM) continue;
    
    if(dilep_channel == EE)  MuonCollTAll.clear();
    if(dilep_channel == MuMu) ElectronCollTAll.clear();

    std::vector<Electron>   ElectronCollT;
    std::vector<Muon>       MuonCollT;

    vector<Gen> gen_lep= GetGenLepronsSignal();
    if(IsSignal()){

      for(auto iel: ElectronCollTAll){
        bool matched_lep=false;
        for(auto igen : gen_lep){
          if(iel.DeltaR(igen) < 0.2) matched_lep=true;
        }
        if(matched_lep) ElectronCollT.push_back(iel);
      }

      for(auto iel: MuonCollTAll){
        bool matched_lep=false;
        for(auto igen :gen_lep){
          if(iel.DeltaR(igen) <0.2) matched_lep=true;
        }
        if(matched_lep)MuonCollT.push_back(iel);
      }

    }
    else{
      ElectronCollT = ElectronCollTAll;
      MuonCollT=MuonCollTAll;
    }

    


    //    TString OptionEl="CFHFakeNHConv";
    TString OptionEl="";
    TString OptionMu="";
    if(SeperateFakes) {
      OptionEl="HFake";
      OptionMu="HFake";
    }
    if(SeperateConv) {
      OptionEl = "NHConv";
      OptionMu="NHConv";
    }
    if(SeperateCF) {
      OptionEl = "CF";
      if(dilep_channel==MuMu) continue;
    }
    if(SeperatePrompt) {
      OptionEl="PromptNoCF";
      OptionMu="Prompt";
      weight=1.;
    }


    /// Select SS dilepotn events, [assing trigger and MVALoose ID
    std::vector<Lepton *> LepsAll  = (dilep_channel==EE) ? MakeLeptonPointerVector(ElectronCollT) : MakeLeptonPointerVector(MuonCollT);
    
    //if (!PassTriggerSelection(dilep_channel, ev, LepsAll, "Dilep")) continue;


    std::vector<Electron>   ElectronCollTSkim  = (IsSignal()) ? ElectronCollT :  SkimLepColl(ElectronCollT, gens, param_bdt, OptionEl);
    std::vector<Muon>       MuonCollTSkim      = (IsSignal()) ? MuonCollT :  SkimLepColl(MuonCollT, gens, param_bdt, OptionMu);


    std::vector<Lepton *> LepsT  = MakeLeptonPointerVector(MuonCollTSkim,ElectronCollTSkim);

    if(SameCharge(LepsAll)) {
      
      for(auto ilep : LepsT){
	int  lepType = GetLeptonType_JH(*ilep, gens);

	if(ilep->LeptonFlavour() == Lepton::ELECTRON)       FillHist( "LepType/SSElectron", lepType ,weight, 14., -7., 7);
	else  FillHist( "LepType/SSMuon", lepType, weight, 14., -7., 7);
      }
    }

    for(auto ilep : LepsT) {
      int  lepType = GetLeptonType_JH(*ilep, gens);
      double neg_w = (weight > 0) ? weight : 0.;
      if(ilep->LeptonFlavour() == Lepton::ELECTRON)       FillHist( "LepType/Electron", lepType ,neg_w, 14., -7., 7);
      else  FillHist( "LepType/Muon", lepType, neg_w, 14., -7., 7);
      
    }

    if(GetRatio) continue;

    //if(!CheckLeptonFlavourForChannel(dilep_channel,LepsAll)) return;

    std::vector<Jet>    AK4_JetAllColl = GetJets("NoID", 10., 5.0);

    if(weight < 0) continue;

    MakeTreeSS2L(dilep_channel, LepsT,MuonCollTSkim,ElectronCollTSkim, AK4_JetAllColl,  weight, "");

  }
  
}

void HNL_LeptonID_BDT_KinVar::MakeTreeSS2L(HNL_LeptonCore::Channel lep_channel,vector<Lepton *>  LepTColl, 
				   vector<Muon> Muons, 
				   vector<Electron> Electrons,
				   vector<Jet>& JetAllColl,
				   float weight, TString Label)
{
  
  InitializeTreeVars();
  
  int imu(0), iel(0);

  for(auto lep : LepTColl){
    
    bool ismuon = (lep->LeptonFlavour() == Lepton::MUON);

    float weight_lep = weight;

    //if(SeperateFakes)weight_lep *= ScaleLepToSS("Fake",ismuon, GetLeptonType_JH(*lep, gens));
    //if(SeperateConv)weight_lep *= ScaleLepToSS("Conv", ismuon, GetLeptonType_JH(*lep, gens));
    
    
    Pt    = (lep->Pt() > 500. ) ? 499 :  lep->Pt();
    Eta   = fabs(lep->Eta());
    PileUp = nPileUp;
    MiniIsoChHad = lep->MiniIsoChHad();
    MiniIsoNHad = lep->MiniIsoNHad();
    MiniIsoPhHad = lep->MiniIsoPhHad();
    IsoChHad = lep->IsoChHad();
    IsoNHad = lep->IsoNHad();
    IsoPhHad = lep->IsoPhHad();


    Dxy   = lep->LogdXY();
    RelDxy   = lep->LogdXY()/lep->Pt();

    DxySig   = lep->LogdXYSig();
    Dz   = lep->LogdZ();
    RelDz   = lep->LogdZ()/lep->Pt();
    DzSig   = lep->LogdZSig();

    IP3D    = lep->SIP3D();
    RelIP3D    = lep->SIP3D()/lep->Pt();

    RelIso    =  lep->RelIso();
    
    Minireliso   = lep->MiniRelIso();
    RelMiniIsoCh = lep->MiniRelIsoCharged();
    RelMiniIsoN = lep->MiniRelIsoNeutral();
    
    JetNTrk = lep->JetNTracks();
    JetNMVATrk = lep->JetNTracksMVA();
    if(lep->LeptonFlavour() == Lepton::ELECTRON) {
      
      MVA    = Electrons[iel].MVANoIsoResponse();

      MVAIso    = Electrons[iel].MVAIsoResponse();

      RelMVA    = Electrons[iel].MVANoIsoResponse()/lep->Pt();

      RelMVAIso    = Electrons[iel].MVAIsoResponse()/lep->Pt();
														   
      POGTight = Electrons[iel].Pass_CutBasedTightNoIso() ?  1: 0;
    
      POGMedium = Electrons[iel].Pass_CutBasedMediumNoIso()?  1: 0;
      HNTightID = Electrons[iel].PassID("HNTightV2") ?  1: 0;
      
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
      dr03EcalRecHitSumEt= Electrons[iel].dr03EcalRecHitSumEt()/Electrons[iel].UncorrPt();
      dr03HcalDepth1TowerSumEt= Electrons[iel].dr03HcalDepth1TowerSumEt()/Electrons[iel].UncorrPt();
      dr03HcalTowerSumEt= Electrons[iel].dr03HcalTowerSumEt()/Electrons[iel].UncorrPt();
      dr03TkSumPt= Electrons[iel].dr03TkSumPt()/Electrons[iel].UncorrPt();
      R9= Electrons[iel].R9();
      HoverE  = Electrons[iel].HoverE();
      Rho     = Electrons[iel].Rho();

      TrkIso  = Electrons[iel].TrkIso()/Electrons[iel].UncorrPt();
      
      InvEminusInvP = fabs(Electrons[iel].InvEminusInvP());
      if(isinf(InvEminusInvP)) InvEminusInvP = 0.;
      
      ecalPFClusterIso = Electrons[iel].ecalPFClusterIso()/Electrons[iel].UncorrPt();

      hcalPFClusterIso = Electrons[iel].hcalPFClusterIso()/Electrons[iel].UncorrPt();

      isEcalDriven = Electrons[iel].isEcalDriven();
      
      EoverP = log(Electrons[iel].EOverP());
      FBrem = Electrons[iel].FBrem();
      PassConversionVeto  = (Electrons[iel].PassConversionVeto()) ? 1 : 0;
      
      IsGsfCtfScPixChargeConsistent  = (Electrons[iel].IsGsfCtfScPixChargeConsistent()) ? 1 : 0;
      IsGsfScPixChargeConsistent  = (Electrons[iel].IsGsfScPixChargeConsistent()) ? 1 : 0;
      IsGsfCtfChargeConsistent  = (Electrons[iel].IsGsfCtfChargeConsistent()) ? 1 : 0;
      
    }
    else{
      
      MVA    = Muons[imu].MVA();
      
      RelMVA    = Muons[imu].MVA()/Muons[imu].Pt();

      POGTight = Muons[imu].isPOGTight() ?  1: 0;
      
      POGMedium = Muons[imu].isPOGMedium() ?  1: 0;

      HNTightID = Muons[imu].PassID("HNTightV2") ?  1: 0;

      MuonSetSegmentCompatibility   = Muons[imu].MuonSetSegmentCompatibility();

      Chi2 = Muons[imu].Chi2();
      
      Validhits = Muons[imu].ValidMuonHits();
      
      Matched_stations = Muons[imu].MatchedStations();
      
      Pixel_hits  = Muons[imu].PixelHits();
      
      Tracker_layers = Muons[imu].TrackerLayers();
      
    }

    //==== Vars for non-prompt lepton bkg
    int IdxMatchJet=-1;
    float mindR1=999.;
    
    for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
      float dR1=lep->DeltaR(JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
    }
  
    if(IdxMatchJet!=-1){
      
      if(lep->LeptonFlavour() == Lepton::ELECTRON){
	PtRatio=JetLeptonPtRatioLepAware(Electrons[iel],false);
        PtRatioNoLep=JetLeptonPtRatioLepAware(Electrons[iel],true);
	PtRelWithLep=JetLeptonPtRelLepAware( Electrons[iel],false);
        PtRel=JetLeptonPtRelLepAware(Electrons[iel],true);
        MassDropNoLep=JetLeptonMassDropLepAware(Electrons[iel],true);
        MassDrop=JetLeptonMassDropLepAware(Electrons[iel],false);
	
	PtRatioCorr=JetLeptonPtRatioLepAware(Electrons[iel],false,true);
        PtRelCorr=JetLeptonPtRelLepAware(Electrons[iel],true,true);
      }
      else{
	PtRatio=JetLeptonPtRatioLepAware(Muons[imu],false);
	PtRatioNoLep=JetLeptonPtRatioLepAware(Muons[imu],true);
	PtRelWithLep=JetLeptonPtRelLepAware( Muons[imu],false);
	PtRel=JetLeptonPtRelLepAware(Muons[imu],true);
	MassDropNoLep=JetLeptonMassDropLepAware(Muons[imu],true);
	MassDrop=JetLeptonMassDropLepAware(Muons[imu],false);
	PtRatioCorr=JetLeptonPtRatioLepAware(Muons[imu],false,true);
        PtRelCorr=JetLeptonPtRelLepAware(Muons[imu],true,true);
      }
      CEMFracCJ = JetAllColl.at(IdxMatchJet).ChargedEmEnergyFraction();
      NEMFracCJ = JetAllColl.at(IdxMatchJet).NeutralEmEnergyFraction();
      CHFracCJ  = JetAllColl.at(IdxMatchJet).ChargedHadEnergyFraction();
      NHFracCJ  = JetAllColl.at(IdxMatchJet).NeutralHadEnergyFraction();
      MuFracCJ  = JetAllColl.at(IdxMatchJet).MuonEnergyFraction();
      JetDiscCJ = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
      PileupJetId   = JetAllColl.at(IdxMatchJet).PileupJetId();
    }
    else{
      PtRatio = min(1/(1.+lep->RelIso()), 1.5);
      PtRatioNoLep  = min(1/(1.+lep->RelIso()),1.5);
      PtRatioCorr  = min(1/(1.+lep->RelIso()),1.5);
      MassDropNoLep=0, MassDrop=0,PtRel=0, PtRelWithLep=0, PtRelCorr=0;
      
      CEMFracCJ=0, NEMFracCJ=0., CHFracCJ=0., NHFracCJ=0., MuFracCJ=0., JetDiscCJ=0.,PileupJetId=-1;
      
    }
    

    w_tot     = !IsDATA? weight_lep: 1.;

    if(lep->LeptonFlavour() == Lepton::MUON) {
      tree_mm->Fill();
      //   if(EtaRegion==1)tree_mm_IB->Fill();
      //if(EtaRegion==2)tree_mm_OB->Fill();
      //if(EtaRegion==3)tree_mm_EC->Fill();
      
    }

    if(lep->LeptonFlavour() == Lepton::ELECTRON) {
      tree_ee->Fill();
      //if(EtaRegion==1)tree_ee_IB->Fill();
      //if(EtaRegion==2)tree_ee_OB->Fill();
      //if(EtaRegion==3)tree_ee_EC->Fill();
    }

    if(lep->LeptonFlavour() == Lepton::ELECTRON) iel++;
    else imu++;

  }
  
}



void HNL_LeptonID_BDT_KinVar::executeEventFromParameter(AnalyzerParameter param){

  
}


void HNL_LeptonID_BDT_KinVar::InitializeTreeVars(){

  Pt=-1, Eta=-1;
  PtRatioNoLep=-1.; PtRatio=-1,  PtRel=-1, PtRelWithLep=-1, PtRatioCorr=-1, PtRelCorr=-1, MassDrop=-1,MassDropNoLep=-1;
  CEMFracCJ=-1, NEMFracCJ=-1, CHFracCJ=-1, NHFracCJ=-1, MuFracCJ=-1, JetDiscCJ=-1,JetNTrk =-1,PileupJetId=-1,JetNMVATrk=-1;

  PileUp = -1;
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
  RelMiniIsoCh=-1.;
  RelMiniIsoN=-1.;
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
  Rho=-1.;
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
  HNTightID = -1;
  isEcalDriven= -1;
  EoverP=-1;
  FBrem=-1;
  PassConversionVeto=-1 ;
  IsGsfCtfScPixChargeConsistent =-1;
  IsGsfScPixChargeConsistent =-1;
  IsGsfCtfChargeConsistent =-1;
  
  R9=-1;
  
  MuonSetSegmentCompatibility = -1;

  w_tot=-1;

}



HNL_LeptonID_BDT_KinVar::HNL_LeptonID_BDT_KinVar(){


  tree_mm = new TTree("Tree_mm", "Tree_mm");
  tree_ee = new TTree("Tree_ee", "Tree_ee");


  //tree_mm_IB = new TTree("Tree_mm_IB", "Tree_mm_IB");
  //t/r/ee_ee_IB = new TTree("Tree_ee_IB", "Tree_ee_IB");

  //  tree_mm_OB = new TTree("Tree_mm_OB", "Tree_mm_OB");
  //tree_ee_OB = new TTree("Tree_ee_OB", "Tree_ee_OB");


  //t//ree_mm_EC = new TTree("Tree_mm_EC", "Tree_mm_EC");
  //tree_ee_EC = new TTree("Tree_ee_EC", "Tree_ee_EC");


}


HNL_LeptonID_BDT_KinVar::~HNL_LeptonID_BDT_KinVar(){

  delete tree_mm;
  delete tree_ee;


  //delete tree_mm_IB;
  //delete tree_ee_IB;

  // delete tree_mm_OB;
  // delete tree_ee_OB;

  //delete tree_mm_EC;
  //delete tree_ee_EC;
}


void HNL_LeptonID_BDT_KinVar::WriteHist(){

  AnalyzerCore::WriteHist();

  outfile->cd();

  tree_mm->Write();
  tree_ee->Write();

  outfile->cd();

}
