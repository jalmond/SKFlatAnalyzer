#include "HNL_LeptonID_BDT_KinVar.h"

void HNL_LeptonID_BDT_KinVar::initializeAnalyzer(){

  SeperateFakes = HasFlag("SeperateFake");
  SeperateConv = HasFlag("SeperateConv");
  SeperateCF = HasFlag("SeperateCF");
  SeperatePrompt = HasFlag("SeperatePrompt");

  GetRatio = HasFlag("GetRatio");
  HNL_LeptonCore::initializeAnalyzer();

  InitializeIDTreeVars();

  SetupTrees(tree_mm,tree_ee);
  

}
void HNL_LeptonID_BDT_KinVar::SetupTrees(TTree* tree_mm, TTree* tree_ee){


  tree_mm->Branch("Pt", &Pt, "Pt/F"); 
  tree_ee->Branch("Pt", &Pt, "Pt/F");
  tree_mm->Branch("PtBinned", &PtBinned, "PtBinned/F");
  tree_ee->Branch("PtBinned", &PtBinned, "PtBinned/F");
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
  tree_mm->Branch("Validhits", &Validhits, "Validhits/F");  

  tree_mm->Branch("Matched_stations", &Matched_stations, "Matched_stations/F");
  tree_mm->Branch("Pixel_hits", &Pixel_hits, "Pixel_hits/F");
  tree_mm->Branch("Minireliso", &Minireliso, "Minireliso/F");
  tree_mm->Branch("Tracker_layers", &Tracker_layers, "Tracker_layers/F");
  tree_mm->Branch("POGTight", &POGTight, "POGTight/F");
  tree_mm->Branch("POGMedium", &POGMedium, "POGMedium/F");
  tree_mm->Branch("HNTightID", &HNTightID, "HNTightID/F");

  tree_ee->Branch("POGTight", &POGTight, "POGTight/F");
  tree_ee->Branch("POGMedium", &POGMedium, "POGMedium/F");
  tree_ee->Branch("HNTightID", &HNTightID, "HNTightID/F");
  tree_mm->Branch("PtRatio", &PtRatio, "PtRatio/F");         tree_ee->Branch("PtRatio", &PtRatio, "PtRatio/F");         
  tree_mm->Branch("PtRel", &PtRel, "PtRel/F");         tree_ee->Branch("PtRel", &PtRel, "PtRel/F");         
  tree_mm->Branch("MassDrop", &MassDrop, "MassDrop/F");         tree_ee->Branch("MassDrop", &MassDrop, "MassDrop/F");         

  tree_mm->Branch("CEMFracCJ", &CEMFracCJ, "CEMFracCJ/F");   tree_ee->Branch("CEMFracCJ", &CEMFracCJ, "CEMFracCJ/F");   
  tree_mm->Branch("NEMFracCJ", &NEMFracCJ, "NEMFracCJ/F");   tree_ee->Branch("NEMFracCJ", &NEMFracCJ, "NEMFracCJ/F");   
  tree_mm->Branch("CHFracCJ", &CHFracCJ, "CHFracCJ/F");      tree_ee->Branch("CHFracCJ", &CHFracCJ, "CHFracCJ/F");      
  tree_mm->Branch("NHFracCJ", &NHFracCJ, "NHFracCJ/F");      tree_ee->Branch("NHFracCJ", &NHFracCJ, "NHFracCJ/F");      
  tree_mm->Branch("MuFracCJ", &MuFracCJ, "MuFracCJ/F");      tree_ee->Branch("MuFracCJ", &MuFracCJ, "MuFracCJ/F");      
  tree_mm->Branch("JetDiscCJ", &JetDiscCJ, "JetDiscCJ/F");   tree_ee->Branch("JetDiscCJ", &JetDiscCJ, "JetDiscCJ/F");   


  // Electron Specific variables
  tree_ee->Branch("MissingHits", &MissingHits, "MissingHits/F");
  tree_ee->Branch("Full5x5_sigmaIetaIeta", &Full5x5_sigmaIetaIeta, "Full5x5_sigmaIetaIeta/F");
  tree_ee->Branch("dEtaSeed",    &dEtaSeed, "dEtaSeed/F");
  tree_ee->Branch("dPhiIn", &dPhiIn, "dPhiIn/F");
  tree_ee->Branch("dEtaIn", &dEtaIn, "dEtaIn/F");
  tree_ee->Branch("HoverE", &HoverE, "HoverE/F");
  tree_ee->Branch("TrkIso", &TrkIso, "TrkIso/F");
  tree_ee->Branch("isEcalDriven", &isEcalDriven, "isEcalDriven/F");
  tree_ee->Branch("EoverP", &EoverP, "EoverP/F");
  tree_ee->Branch("FBrem", &FBrem, "FBrem/F");
  tree_ee->Branch("InvEminusInvP", &InvEminusInvP, "InvEminusInvP/F");
  tree_ee->Branch("PassConversionVeto", &PassConversionVeto, "PassConversionVeto/F");
  tree_ee->Branch("ecalPFClusterIso", &ecalPFClusterIso, "ecalPFClusterIso/F");
  tree_ee->Branch("hcalPFClusterIso", &hcalPFClusterIso, "hcalPFClusterIso/F");
  tree_ee->Branch("IsGsfCtfScPixChargeConsistent", &IsGsfCtfScPixChargeConsistent, "IsGsfCtfScPixChargeConsistent/F");
  tree_ee->Branch("IsGsfScPixChargeConsistent", &IsGsfScPixChargeConsistent, "IsGsfScPixChargeConsistent/F");
  tree_ee->Branch("IsGsfCtfChargeConsistent", &IsGsfCtfChargeConsistent, "IsGsfCtfChargeConsistent/F");
  
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
  tree_mm->Branch("w_id_tot", &w_id_tot, "w_id_tot/F"); 
  tree_ee->Branch("w_id_tot", &w_id_tot, "w_id_tot/F");                     

  outfile->cd();

}


void HNL_LeptonID_BDT_KinVar::executeEvent(){

  if(MCSample.Contains("TTLJ") && SeperatePrompt) {
    double r = ((double) rand() / (RAND_MAX));
    if(r > 0.12) return;
  }

  AnalyzerParameter param_bdt = HNL_LeptonCore::InitialiseHNLParameter("BDT", "");
  
  Event ev = GetEvent();

  gens = GetGens();

  if(!PassMETFilter()) return;

  double weight =SetupWeight(ev,param_bdt);

  vector<TString> MCMergeList = {"DY","WG"};
  if(SeperateConv || SeperateCF)weight *= MergeMultiMC(MCMergeList,"CombineAll");

  if(weight <= 0) {
    std::vector<Electron>   ElectronCollTMP =  GetElectrons( param_bdt,"MVAID", 10., 2.5, RunFake);
    for(auto ilep : ElectronCollTMP)   FillHist( "fEtaElZeroW", fabs(ilep.Eta()) ,weight, 300., 0., 3);
    ////  Remove -v2 weights for training 
    return;
  }
  
  if(SeperateFakes){
    weight  = GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0); 
    if(MCSample.Contains("TTL")) weight*= 0.25;
  }
  if(SeperatePrompt) {
    weight  = GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  }
  
  if(MCSample.Contains("Type")) weight = GetPrefireWeight(0) * GetPileUpWeight(nPileUp,0);
  
  FillHist("CutFlow", 0, weight,  20, 0., 20.);
  FillHist( "nPV", nPV ,weight, 100., 0., 100.);
 
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};
  
  for(auto dilep_channel : channels){

    std::vector<Muon>       MuonCollTAll     =  GetMuons    ( param_bdt,"MVAID", 10., 2.4, RunFake);
    std::vector<Electron>   ElectronCollTAll =  GetElectrons( param_bdt,"MVAID", 10., 2.5, RunFake);
 
    bool HasHEM(false);
    for(auto i: ElectronCollTAll) {
      if(FindHEMElectron(i)) HasHEM=true;
    }
    if(HasHEM) continue;
    
    FillHist( "nPVHEM", nPV ,weight, 100., 0., 100.);


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

    std::vector<Jet>    AK4_JetAllColl = GetAllJets();

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
  

  int imu(0), iel(0);

  for(auto lep : LepTColl){
    
    InitializeIDTreeVars();
    
    float weight_lep = weight;

    if(SeperateFakes && HasFlag("HF")){
      std::vector<Jet>    AK4_JetAllColl = GetAllJets();

      int JetHadFlavour = -999;
      int IdxMatchJet=-1;
      float mindR1=999.;

      for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
        float dR1=lep->DeltaR(AK4_JetAllColl.at(ij));
        if(dR1>0.4) continue;
        if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
      }
      if(IdxMatchJet!=-1)     JetHadFlavour = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();
      else JetHadFlavour=4;
      
      if (JetHadFlavour < 4)  return;
    }

    if(SeperateFakes && HasFlag("LF")){
      std::vector<Jet>    AK4_JetAllColl = GetAllJets();
      
      int JetHadFlavour = -999;
      int IdxMatchJet=-1;
      float mindR1=999.;

      for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
        float dR1=lep->DeltaR(AK4_JetAllColl.at(ij));
        if(dR1>0.4) continue;
        if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
      }
      if(IdxMatchJet!=-1)     JetHadFlavour = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();
      else JetHadFlavour=0;
      
      if (JetHadFlavour != 0)  return;
    }
    
    if(lep->LeptonFlavour() == Lepton::MUON) FillHist( "nPV_Muon", nPV ,weight, 100., 0., 100.);
    else FillHist( "nPV_Electron", nPV ,weight, 100., 0., 100.);


    SetBDTIDVar(lep);

    if(lep->LeptonFlavour() == Lepton::ELECTRON)  SetBDTIDVariablesElectron(Electrons[iel]);
    else SetBDTIDVariablesMuon(Muons[imu]);

    w_id_tot     = !IsDATA? weight_lep: 1.;

    if(lep->LeptonFlavour() == Lepton::MUON)     tree_mm->Fill();
    if(lep->LeptonFlavour() == Lepton::ELECTRON) tree_ee->Fill();

    if(lep->LeptonFlavour() == Lepton::ELECTRON) iel++;
    else imu++;

  }
  
}



void HNL_LeptonID_BDT_KinVar::executeEventFromParameter(AnalyzerParameter param){

  
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
