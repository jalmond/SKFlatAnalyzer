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
  vector<HNL_LeptonCore::Channel> ChannelsToRun = {MuMu,EE,EMu };
  
  vector<TString> RegionsToPlot = {"SSMultiLep","Dilepton"}; 
  
  ///// Run command 

  vector<TString> CRToRun;
  if(HasFlag("Dilepton"))        CRToRun = {"OS_VR","SS_CR","VBF_CR","LLL_VR"};
  else if(HasFlag("OS_VR"))      CRToRun = {"OS_VR"}; //JH
  else if(HasFlag("SS_CR"))      CRToRun = {"SS_CR"}; //JH
  else if(HasFlag("VBF_CR"))     CRToRun = {"VBF_CR"}; //JH
  else if(HasFlag("LLL_VR"))     CRToRun = {"LLL_VR"}; //JH
  else if(HasFlag("SSMultiLep")) CRToRun = {"SS_CR","VBF_CR","LLL_VR"};
  //else CRToRun = {"SS_CR"}; //JH

  for (auto id: LepIDs){
    for(auto channel : ChannelsToRun){
      AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(id,channel);
     
      for(auto iCR : CRToRun) RunControlRegions(param_signal , {iCR} );
    }
  }
  return;
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

  std::vector<Muon>       MuonTightColl     = SelectMuons    ( param,Muon_ID,     Min_Muon_Pt,     2.4,weight); 
  std::vector<Electron>   ElectronTightColl = SelectElectrons( param,Electron_ID, Min_Electron_Pt, 2.5,weight);

  //// Change this so now Truth matching does not remove Leptons but in Definition code the GenFIlter removes events 
  //  std::vector<Muon>       MuonTightColl      =  GetLepCollByRunType    (MuonTightCollInit,    param);  
  //std::vector<Electron>   ElectronTightColl  =  GetLepCollByRunType    (ElectronTightCollInit,param);


  Particle METv = GetvMET("PuppiT1xyULCorr",param); // returns MET with systematic correction                                                                      

  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets(param.AK8JetColl,param);
  std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);
  std::vector<Jet>    AK4_VBF_JetColl             = GetHNLJets(param.AK4VBFJetColl,  param);
  std::vector<Jet>    AK4_JetAllColl              = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet>    AK4_JetCollLoose            = GetHNLJets("Loose",     param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param);
  
  EvalJetWeight(AK4_JetColl, AK8_JetColl, weight, param);

  if(CRs.size() == 0) return;
  
  vector<int> RunEl ;
  if(RunCF) RunEl =  {0,1} ;
  else RunEl = {-1};

  for(auto ir : RunEl){
    RunAllControlRegions(ElectronTightColl,ElectronVetoColl,MuonTightColl,MuonVetoColl, 
			 AK4_JetAllColl, AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl, 
			 ev,METv, param, CRs,ir,weight);
  }

}




HNL_ControlRegion_Plotter::HNL_ControlRegion_Plotter(){

  SetupEvMVA();
}
 
HNL_ControlRegion_Plotter::~HNL_ControlRegion_Plotter(){

  DeleteEvMVA();
}
