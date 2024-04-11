#include "HNL_ControlRegion_Plotter.h"

void HNL_ControlRegion_Plotter::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  SetupEventMVAReader();

  nLog = 100000;
}


void HNL_ControlRegion_Plotter::executeEvent(){

  if(!(_jentry%10000)) run_Debug=true;
  else run_Debug=false;

  ///// LIST IDs to run
  vector<TString> LepIDs = {"HNL_ULID"};//"TopHN","HNL_ULID","HNTightV2"};//,"TopHN", "DefaultPOGTight"};
  //// List Channels to run
  vector<HNL_LeptonCore::Channel> ChannelsToRun = {MuMu,EE,EMu};//,EE,EMu,MuE };
  
  vector<TString> RegionsToPlot = {"SSMultiLep","Dilepton"}; 
  
  ///// Run command 

  vector<TString> CRToRun;
  bool StandardProcess = true;
  //if(HasFlag("Dilepton"))        CRToRun = {"OS_VR","SS_CR","VBF_CR","LLL_VR"};
  if(HasFlag("OS_VR"))             CRToRun = {"OS_VR"}; //JH
  else if(HasFlag("SS_CR"))        CRToRun = {"SS_CR"}; //JH
  else if(HasFlag("VBF_CR"))       CRToRun = {"VBF_CR"}; //JH
  else if(HasFlag("LLL_VR"))       CRToRun = {"LLL_VR"}; //JH
  else if(HasFlag("Dilepton"))     CRToRun = {"OS_VR","SS_CR","VBF_CR","LLL_VR"};
  else if(HasFlag("SSMultiLep")) CRToRun = {"SS_CR","VBF_CR","LLL_VR"};
  else if(HasFlag("TestFakes"))  { StandardProcess=false ; CRToRun = {"SS_CR","VBF_CR","LLL_VR"};}
  //else CRToRun = {"SS_CR"}; //JH

  if(StandardProcess){
    for (auto id: LepIDs){
      for(auto channel : ChannelsToRun){
        AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(id,channel);
        
        if(channel == MuMu && id == "HNL_ULID") param_signal.FakeRateParam  = "Pt";
        if(channel == MuMu && id == "HNL_ULID") param_signal.Muon_FR_ID = "HNL_ULID_FO";
        for(auto iCR : CRToRun) RunControlRegions(param_signal , {iCR} );
      }
    }
    return;
  }

}

void HNL_ControlRegion_Plotter::RunControlRegions(AnalyzerParameter param, vector<TString> CRs){


  //  if(_jentry==0) param.PrintParameters();
  run_Debug = (_jentry%nLog==0);

  if(run_Debug) cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  
  Event ev = GetEvent();

  /// SetupWeight applies w_GenNorm=1., w_BR=1., w_PU  w_Pref  
  double weight =SetupWeight(ev,param);
  
  // HL ID                                                                                                                                                   
  std::vector<Electron>   ElectronVetoColl = GetElectrons(param.Electron_Veto_ID, 10.,  2.5);
  std::vector<Muon>       MuonVetoColl     = GetMuons    (param.Muon_Veto_ID,     5.,  2.4);

  /// IF ruunning fake then use FR_ID not Tight
  TString Electron_ID = SetLeptonID("Electron",param);
  TString Muon_ID     = SetLeptonID("Muon", param);

  double Min_Muon_Pt     = RunFake ? 5  : 10.;
  double Min_Electron_Pt = RunFake ? 10 : 15;

  std::vector<Muon>       MuonTightCollInit     = SelectMuons    ( param,Muon_ID,     Min_Muon_Pt,     2.4,weight); 
  std::vector<Electron>   ElectronTightCollInit = SelectElectrons( param,Electron_ID, Min_Electron_Pt, 2.5,weight);

  std::vector<Muon>       MuonTightColl      =  GetLepCollByRunType    (MuonTightCollInit,    param);  
  std::vector<Electron>   ElectronTightColl  =  GetLepCollByRunType    (ElectronTightCollInit,param);


  Particle METv = GetvMET("PuppiT1xyULCorr",param); // returns MET with systematic correction                                                                      

  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets(param.AK8JetColl,param);
  std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);
  std::vector<Jet>    AK4_VBF_JetColl             = GetHNLJets(param.AK4VBFJetColl,  param);
  std::vector<Jet>    AK4_JetAllColl              = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet>    AK4_JetCollLoose            = GetHNLJets("Loose",     param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param);
  
  EvalJetWeight(AK4_JetColl, AK8_JetColl, weight, param);

  bool DrawBasicPlotsL (false);
  bool DrawBasicPlotsT (false); //JH : FIXME later
  if(DrawBasicPlotsL){
    std::vector<Lepton *> LepsAll       = MakeLeptonPointerVector(MuonVetoColl,ElectronVetoColl,param);
    
    if(MuonVetoColl.size() == 3)     {
      FillHist("SimplePlots/TriMuon", 1 ,  1, 200, 0 ,5 );
      double MassMinOSSF = GetMassMinOSSF(MakeLeptonPointerVector(MuonVetoColl));
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("SimplePlots/TriMuon_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("SimplePlots/TriMuon1_Pt",  MuonVetoColl[0].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/TriMuon2_Pt",  MuonVetoColl[1].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/TriMuon3_Pt",  MuonVetoColl[2].Pt() ,  1, 50, 0 ,200 );
    }
    else   if(ElectronVetoColl.size() == 3) {
      double MassMinOSSF = GetMassMinOSSF(MakeLeptonPointerVector(ElectronVetoColl));
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("SimplePlots/TriEl", 1 ,  1, 200, 0 ,5 );
      FillHist("SimplePlots/TriEl_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("SimplePlots/TriEl1_Pt",  ElectronVetoColl[0].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/TriEl2_Pt",  ElectronVetoColl[1].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/TriEl3_Pt",  ElectronVetoColl[2].Pt() ,  1, 50, 0 ,200 );
    }
    else if(LepsAll.size() == 3){
      double MassMinOSSF = GetMassMinOSSF(LepsAll);
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("SimplePlots/TriEMu", 1 ,  1, 200, 0 ,5 );
      FillHist("SimplePlots/TriEMu_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("SimplePlots/TriEMu1_Pt",  LepsAll[0]->Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/TriEMu2_Pt",  LepsAll[1]->Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/TriEMu3_Pt",  LepsAll[2]->Pt() ,  1, 50, 0 ,200 );
    }
  }

  bool PassTriLep(false);
  if(DrawBasicPlotsT){
    std::vector<Lepton *> LepsAll       = MakeLeptonPointerVector(MuonTightColl,ElectronTightColl);

    if(MuonTightColl.size() == 3)     {
      FillHist("Tight_TriMuon", 1 ,  1, 200, 0 ,5 );
      double MassMinOSSF = GetMassMinOSSF(MakeLeptonPointerVector(MuonTightColl));
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("Tight_TriMuon_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("Tight_TriMuon1_Pt",  MuonVetoColl[0].Pt() ,  1, 50, 0 ,200 );
      FillHist("Tight_TriMuon2_Pt",  MuonVetoColl[1].Pt() ,  1, 50, 0 ,200 );
      FillHist("Tight_TriMuon3_Pt",  MuonVetoColl[2].Pt() ,  1, 50, 0 ,200 );
      PassTriLep=true;
    }
    else   if(ElectronTightColl.size() == 3) {
      double MassMinOSSF = GetMassMinOSSF(MakeLeptonPointerVector(ElectronTightColl));
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("Tight_TriEl", 1 ,  1, 200, 0 ,5 );
      FillHist("Tight_TriEl_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("Tight_TriEl1_Pt",  ElectronVetoColl[0].Pt() ,  1, 50, 0 ,200 );
      FillHist("Tight_TriEl2_Pt",  ElectronVetoColl[1].Pt() ,  1, 50, 0 ,200 );
      FillHist("Tight_TriEl3_Pt",  ElectronVetoColl[2].Pt() ,  1, 50, 0 ,200 );
      PassTriLep=true;

    }
    else if(LepsAll.size() == 3){
      double MassMinOSSF = GetMassMinOSSF(LepsAll);
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("Tight_TriEMu", 1 ,  1, 200, 0 ,5 );
      FillHist("Tight_TriEMu_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("Tight_TriEMu1_Pt",  LepsAll[0]->Pt() ,  1, 50, 0 ,200 );
      FillHist("Tight_TriEMu2_Pt",  LepsAll[1]->Pt() ,  1, 50, 0 ,200 );
      FillHist("Tight_TriEMu3_Pt",  LepsAll[2]->Pt() ,  1, 50, 0 ,200 );
      //      PassTriLep=true;

    }
  }

  if(CRs.size() == 0) return;
  
  //if(RunFake) {
  //  vector<Muon> MuonTightCollMedium;
  //  for(auto imu : MuonTightColl) if( imu.PassID("POGMedium")) MuonTightCollMedium.push_back(imu);

  //  std::vector<Lepton *> LepsAll       = MakeLeptonPointerVector(MuonTightColl,ElectronTightColl,param);
  //  std::vector<Lepton *> LepsAll2       = MakeLeptonPointerVector(MuonTightCollMedium,ElectronTightColl,param);
  //  if(SameCharge(LepsAll)) {
  //    if(LepsAll[0]->Pt() > 20 && LepsAll[1]->Pt() > 10){
  //      FillFakeHistograms(param, "SSFakePlots" ,LepsAll, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //      if(AK4_JetColl.size()==0)       FillFakeHistograms(param, "SS1JetFakePlots" ,  LepsAll, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //      if(AK4_BJetColl.size()==0)       FillFakeHistograms(param, "SS0BJetFakePlots" ,  LepsAll, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //      else  FillFakeHistograms(param, "SSBJetFakePlots" ,  LepsAll, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //    }
  //  }
  //  if(SameCharge(LepsAll2)) {
  //    if(LepsAll2[0]->Pt() > 20 && LepsAll2[1]->Pt() > 10){
  //      FillFakeHistograms(param, "SS2FakePlots" ,LepsAll2, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //      if(AK4_JetColl.size()==0)       FillFakeHistograms(param, "SS21JetFakePlots" ,  LepsAll2, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //      if(AK4_BJetColl.size()==0)       FillFakeHistograms(param, "SS20BJetFakePlots" ,  LepsAll2, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //      else  FillFakeHistograms(param, "SS2BJetFakePlots" ,  LepsAll2, AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,   1);
  //    }
  //  }
  //  
  //  return;
  //}


  RunAllControlRegions(ElectronTightColl,ElectronVetoColl,MuonTightColl,MuonVetoColl, 
           AK4_JetAllColl, AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl, 
           ev,METv, param, CRs,weight);
  

}




HNL_ControlRegion_Plotter::HNL_ControlRegion_Plotter(){

  SetupEvMVA();
}
 
HNL_ControlRegion_Plotter::~HNL_ControlRegion_Plotter(){

  DeleteEvMVA();
}
