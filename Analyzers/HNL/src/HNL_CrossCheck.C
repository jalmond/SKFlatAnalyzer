#include "HNL_CrossCheck.h"

void HNL_CrossCheck::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  SetupEventMVAReader();

  nLog = 100000;
}


void HNL_CrossCheck::executeEvent(){

  if(!(_jentry%10000)) run_Debug=true;
  else run_Debug=false;

  ///// LIST IDs to run
  vector<TString> LepIDs = {"HNL_ULID"};//,"TopHN", "DefaultPOGTight"};
  if(strcmp(std::getenv("USER"),"jalmond")==0) LepIDs = {"HNL_ULID"};//,"HNTightV2" ,"TopHN"};//, "POGTight"};
  if(RunFakeTF) LepIDs = {"HNL_ULID"};
  
  //// List Channels to run
  vector<HNL_LeptonCore::Channel> ChannelsToRun = {EMu ,MuMu,EE};
  
  vector<TString> RegionsToPlot = {"SSMultiLep","Dilepton"}; 
  
  ///// Run command 

  vector<TString> CRToRun;
  if(HasFlag("Dilepton"))        CRToRun = {"OS_VR","SS_CR","VBF_CR","LLL_VR"};
  else if(HasFlag("SSMultiLep")) CRToRun = {"SS_CR","VBF_CR","LLL_VR"};
  else CRToRun = {"SS_CR"};

  for (auto id: LepIDs){
    for(auto channel : ChannelsToRun){
      if(channel != MuMu  && id =="TopHN") continue;

      AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(id,channel);
      if(channel == EMu) param_signal.CFMethod   = "MC";

      for(auto iCR : CRToRun) RunControlRegions(param_signal , {iCR} );
    }
  }
  return;
}

void HNL_CrossCheck::RunControlRegions(AnalyzerParameter param, vector<TString> CRs){


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

  double Min_Muon_Pt     = RunFake ? 5  : 15.;  //// Check 
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
  
  if(_jentry < 2000 ){
    if(RunFake){
      cout << "Running Fakes: Initial check for process name " + param.Name << endl;
      cout << "Muon ID = " << param.Muon_Tight_ID  << " run ID  = " << Muon_ID << endl;
      for(auto ilep : MuonTightColl)cout << "Muon isT=" << ilep.PassID(param.Muon_Tight_ID) << endl;
      
      cout << "Electron ID = " << param.Electron_Tight_ID  << " run ID  = " << Electron_ID << endl;
      for(auto ilep : ElectronTightColl)cout << "Electron isT=" << ilep.PassID(param.Electron_Tight_ID) << endl;
    }
  }


  vector<int> RunEl ;
  if(RunCF) RunEl =  {0,1} ;
  else RunEl = {-1};

  
  HNL_LeptonCore::Channel dilep_channel   =   GetChannelENum(param.Channel);

  if(!PassMETFilter()) return;

  std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(MuonTightColl, ElectronTightColl,     param);
  std::vector<Lepton *> LepsV       = MakeLeptonPointerVector(MuonVetoColl,ElectronVetoColl,param);

  if(!PassHEMVeto(LepsV)) return;  

  //  if (! (CheckLeptonFlavourForChannel(dilep_channel, LepsT))) return;
  //  if (!PassTriggerSelection(dilep_channel, ev, LepsT,param.TriggerSelection)) return;
  ///  EvalTrigWeight(dilep_channel, MuonTightColl, ElectronTightColl,param, ev, weight);

  param.Name = param.Name  + "/"+ GetChannelString(dilep_channel);

  
  //FillHighMassNPCRPlots(dilep_channel, LepsT, LepsV, AK4_JetColl, AK8_JetColl, AK4_BJetColl, ev, METv, param, weight)  ;
  bool RunNP=true;
  if(RunNP){
    if(LepsT.size() < 2) return ;    

    Particle ll =  (*LepsT[0]) + (*LepsT[1]);
    if(ll.M() < M_CUT_NonSR3_LL) return ;

    bool HasConv=false;
    int ConvLepType=0;
    for(auto ilep : LepsT){
      if(ilep->LeptonGenType() > 3) {HasConv = true; ConvLepType = ilep->LeptonGenType();}
      if(ilep->LeptonGenType() < -3) {HasConv = true;  ConvLepType = ilep->LeptonGenType();}
    }
    if(!HasConv) return;
    
    if(LepsT.size() == 3){
      AnalyzerParameter paramTrilep  = param;
      paramTrilep.Channel  = GetChannelString(GetTriLeptonChannel(dilep_channel));

      if(CheckLeptonFlavourForChannel(GetTriLeptonChannel(dilep_channel),LepsT )){
	Fill_RegionPlots(paramTrilep,"HNL_SS_ThreeLep"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
	if(ConvLepType == 6)        Fill_RegionPlots(paramTrilep,"HNL_SS_ThreeLep_6"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
	if(ConvLepType == 5)        Fill_RegionPlots(paramTrilep,"HNL_SS_ThreeLep_5"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
	if(ConvLepType == 4)        Fill_RegionPlots(paramTrilep,"HNL_SS_ThreeLep_4"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
	if(ConvLepType == -4)        Fill_RegionPlots(paramTrilep,"HNL_SS_ThreeLep_M4"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
	if(ConvLepType == -5)        Fill_RegionPlots(paramTrilep,"HNL_SS_ThreeLep_M5"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
	if(ConvLepType == -6)        Fill_RegionPlots(paramTrilep,"HNL_SS_ThreeLep_M6"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
      }
    }

    if(LepsT.size() != 2) return ;
    if(!CheckLeptonFlavourForChannel(dilep_channel,LepsT )) return ;
    if(SameCharge(LepsT)) {
      /* if(LepsT[0]->LeptonGenType() == -5 || LepsT[1]->LeptonGenType() == -5){
	cout << "--------- " << endl;
	cout << MCSample << " Conv Type Lep1 " << LepsT[0]->LeptonGenType()  << " lep 2 " << LepsT[1]->LeptonGenType()  << " Mass = " << ll.M() << endl;
	cout << "Lep1 pt = " << LepsT[0]->PrintInfo() << endl;
	PrintMatchedGen(All_Gens,*LepsT[0]);
	cout << "Lep2 pt = " << LepsT[1]->PrintInfo() << endl;
	PrintMatchedGen(All_Gens,*LepsT[1]);
	}*/
      if(LepsT[0]->LeptonGenType() == 4 || LepsT[1]->LeptonGenType() == 4) Fill_RegionPlots(param,"HNL_SS_ConvP4_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
      if(LepsT[0]->LeptonGenType() == 5 || LepsT[1]->LeptonGenType() == 5) Fill_RegionPlots(param,"HNL_SS_ConvP5_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
      if(LepsT[0]->LeptonGenType() == 6 || LepsT[1]->LeptonGenType() == 6) Fill_RegionPlots(param,"HNL_SS_ConvP6_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
      if(LepsT[0]->LeptonGenType() == -4 || LepsT[1]->LeptonGenType() == -4) Fill_RegionPlots(param,"HNL_SS_ConvM4_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
      if(LepsT[0]->LeptonGenType() == -5 || LepsT[1]->LeptonGenType() == -5) Fill_RegionPlots(param,"HNL_SS_ConvM5_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
      if(LepsT[0]->LeptonGenType() == -6 || LepsT[1]->LeptonGenType() == -6) Fill_RegionPlots(param,"HNL_SS_ConvM6_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);											      
    }
    if(SameCharge(LepsT))Fill_RegionPlots(param,"HNL_SS_HighMassNP_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
    else Fill_RegionPlots(param,"HNLS_OS_HighMassNP_TwoLepton_CR"  ,  AK4_JetColl, AK8_JetColl ,LepsT,  METv, nPV, weight);
    
  }

}




HNL_CrossCheck::HNL_CrossCheck(){

  SetupEvMVA();
}
 
HNL_CrossCheck::~HNL_CrossCheck(){

  DeleteEvMVA();
}
