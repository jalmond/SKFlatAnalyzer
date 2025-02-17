#include "HNL_ControlRegion_Plotter.h"

void HNL_ControlRegion_Plotter::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();


  if(IsDATA){
    bool run_ee_bdt=false;
    bool run_mm_bdt=false;
    bool run_em_bdt=false;

    if (this->DataStream == "DoubleMuon") run_mm_bdt=true;
    if (this->DataStream == "SingleMuon") run_mm_bdt=true;
    if (this->DataStream == "DoubleEG") run_ee_bdt=true;
    if (this->DataStream == "SingleElectron") run_ee_bdt=true;
    if (this->DataStream == "EGamma") run_ee_bdt=true;
    if (this->DataStream == "MuonEG") run_em_bdt=true;

    SetupEventMVAReader("V2",run_ee_bdt,run_mm_bdt,run_em_bdt);

  }
  else SetupEventMVAReader("V2");


  RunTopID = HasFlag("RunHNTop");
  RunPOGID = HasFlag("RunPOG");
  RunHighPtID = HasFlag("RunHighPt");
  RunPekingID = HasFlag("RunPeking");

  nLog = 100000;
}


void HNL_ControlRegion_Plotter::executeEvent(){

  if(!(_jentry%10000)) run_Debug=true;
  else run_Debug=false;

  vector<TString> LepIDs = {"HNL_ULIDv2"};

  /// Set ID by flag
  if(RunTopID) LepIDs = {"TopHN"};
  if(RunPOGID) LepIDs = {"POGTight"};
  if(RunHighPtID) LepIDs = {"HighPt"};
  if(RunPekingID) LepIDs = {"Peking"};

  //  if(strcmp(std::getenv("USER"),"jalmond")==0) LepIDs = {"HNL_ULID","POGTight","TopHN","HNTightV2","MVAPOG"};//,"HNTightV2","POGTight","TopHN","HighPt"};                               

  vector<HNL_LeptonCore::Channel> ChannelsToRun = {};
  if(RunEE)   ChannelsToRun.push_back(EE);
  if(RunMuMu) ChannelsToRun.push_back(MuMu);
  if(RunEMu)  ChannelsToRun.push_back(EMu);
  if(ChannelsToRun.size() == 0)ChannelsToRun = {EE,MuMu,EMu};

  ///// Run command 

  vector<TString> CRToRun;
  if(HasFlag("OS"))  {
    LepIDs = {"HNL_ULIDv2"};
    CRToRun = {"OS_VR"};
    ChannelsToRun = {MuMu};
  }
  else if(HasFlag("Dilepton"))        CRToRun = {"OS_VR","SS_CR","VBF_CR","LLL_VR"};
  else if(HasFlag("SSMultiLep")) CRToRun = {"SS_CR","VBF_CR"};
  else if(HasFlag("LLL")) CRToRun = {"LLL_VR"};
  else CRToRun = {"SS_CR"};


  if(IsDATA){
    if (this->DataStream == "DoubleMuon") ChannelsToRun = {MuMu};
    if (this->DataStream == "SingleMuon") ChannelsToRun = {MuMu};
    if (this->DataStream == "DoubleEG") ChannelsToRun = {EE};
    if (this->DataStream == "SingleElectron") ChannelsToRun = {EE};
    if (this->DataStream == "EGamma") ChannelsToRun = {EE};
    if (this->DataStream == "MuonEG") ChannelsToRun = {EMu};
  }



  for (auto id: LepIDs){

    for(auto channel : ChannelsToRun){
      if(channel != MuMu  && id =="TopHN") continue;

      AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(id,channel);
      if(channel == EMu) param_signal.CFMethod   = "MC";

      param_signal.PlottingVerbose = 0;

      for(auto iCR : CRToRun) {
	RunControlRegions(param_signal , {iCR} );

	TString param_name = param_signal.Name;

	TString SystString = "";
	//	if(HasFlag("OS")) SystString = "Muon";
	SystString=GetChannelString(channel);


	if(HasFlag("RunSyst")){
	  /// Some code to remove unnecessary Syst runs                                                                                                                                                              
	  if(!PassMETFilter()) return;
  
	  Event ev = GetEvent();

	  if(channel==EE){
	    if(!ev.PassTrigger(TrigList_HNL_DblEG)) continue;
	    std::vector<Muon>       MuonCollV     = SelectMuons    (param_signal,param_signal.Muon_Veto_ID,     5., 2.4);  
	    if(MuonCollV.size() > 0) continue;
	  }
	  if(channel==MuMu){
            if(!ev.PassTrigger(TrigList_HNL_DblMu)) continue;
	    std::vector<Electron>   ElectronCollV = SelectElectrons(param_signal,param_signal.Electron_Veto_ID, 10., 2.5);	    
	    if(ElectronCollV.size() >0) continue;
	  }
	  if(channel==EMu){
	    if(!(ev.PassTrigger(TrigList_HNL_MuEG) || ev.PassTrigger(TrigList_HNL_EGMu) )) continue;
	  }
	}
	

	for(auto isyst : GetSystList(SystString)){
	  bool runJob = UpdateParamBySyst(id,param_signal,AnalyzerParameter::Syst(isyst),param_name);
	  if(runJob)         RunControlRegions(param_signal , {iCR} );
	}
      }
    }
  }
  return;
}

void HNL_ControlRegion_Plotter::RunControlRegions(AnalyzerParameter param, vector<TString> CRs){

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

  double Min_FakeMuon_Pt     =  5;
  double Min_FakeElectron_Pt =  10 ;
  std::vector<Muon>       MuonTightColl_Init     = SelectMuons    ( param,Muon_ID,     Min_FakeMuon_Pt,     2.4,weight); 
  std::vector<Electron>   ElectronTightColl_Init = SelectElectrons( param,Electron_ID, Min_FakeElectron_Pt, 2.5,weight);

  //// Apply Full Pt cut after pt corrected in fakes  
  double Min_Muon_Pt     =  10.;
  double Min_Electron_Pt =  15;
  std::vector<Muon>       MuonTightColl  = SelectMuons(MuonTightColl_Init,Muon_ID,     Min_Muon_Pt,     2.4);
  std::vector<Electron>   ElectronTightColl = SelectElectrons(ElectronTightColl_Init,Electron_ID, Min_Electron_Pt, 2.5);



  //// Change this so now Truth matching does not remove Leptons but in Definition code the GenFIlter removes events 
  //  std::vector<Muon>       MuonTightColl      =  GetLepCollByRunType    (MuonTightCollInit,    param);  
  //std::vector<Electron>   ElectronTightColl  =  GetLepCollByRunType    (ElectronTightCollInit,param);


  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets(param.AK8JetColl,param);
  std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);
  std::vector<Jet>    AK4_VBF_JetColl             = GetHNLJets(param.AK4VBFJetColl,  param);
  std::vector<Jet>    AK4_JetAllColl              = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet>    AK4_JetCollLoose            = GetHNLJets("Loose",     param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param);
  
  EvalJetWeight(AK4_JetColl,AK4_VBF_JetColl, AK8_JetColl, weight, param);

  Particle METv = GetvMET("PuppiT1xyULCorr", param, AK4_VBF_JetColl, AK8_JetColl, MuonTightColl,ElectronTightColl);

  if(CRs.size() == 0) return;
  
  vector<int> RunEl ;
  if(RunCF) RunEl =  {0,1} ;
  else RunEl = {-1};


  if(HasFlag("OS"))  {
    
    
    if(!MCSample.Contains("DYJets_Pt")){

      /// Run MiNNLO only
      RunAllControlRegions(ElectronTightColl,ElectronVetoColl,MuonTightColl,MuonVetoColl,
			   AK4_JetCollLoose,AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl,
			   ev,METv, param, CRs,-1,weight);
      
    }
    
    
    
    param.Name=param.Name+"_PtBinnedDY";
    param.DefName=param.DefName+"_PtBinnedDY";

    
    if(MCSample == "DYJetsToMuMu_MiNNLO" || MCSample.Contains("DYJets_Pt") ) {

      if(MuonTightColl.size()==2 && !(SameCharge(MuonTightColl))){
	Lepton l1(MuonTightColl[0]);
	Lepton l2(MuonTightColl[1]);

	int Idx1_Closest = GenMatchedIdx(l1, All_Gens);
	int Idx2_Closest = GenMatchedIdx(l2, All_Gens);
	if(Idx1_Closest >=0 && Idx2_Closest >= 0){

	  if (std::abs(All_Gens[Idx1_Closest].PID()) != 13 || std::abs(All_Gens[Idx2_Closest].PID()) != 13) return;

	  Particle Z = All_Gens[Idx1_Closest] + All_Gens[Idx2_Closest];

	  if(MCSample == "DYJetsToMuMu_MiNNLO"){
	    if(Z.Pt() > 150) return;
	  }
	  else if(Z.Pt() <= 150) return;
	}
	else return;
      } // DY loop                                                                                                                                                                                              
    }


    RunAllControlRegions(ElectronTightColl,ElectronVetoColl,MuonTightColl,MuonVetoColl,
			 AK4_JetCollLoose,AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl,
			 ev,METv, param, CRs,-1,weight);

  }
  else{
    
    for(auto ir : RunEl){
      RunAllControlRegions(ElectronTightColl,ElectronVetoColl,MuonTightColl,MuonVetoColl, 
			   AK4_JetCollLoose,AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl, 
			   ev,METv, param, CRs,ir,weight);
    }
  }
}




HNL_ControlRegion_Plotter::HNL_ControlRegion_Plotter(){

  SetupEvMVA();
}
 
HNL_ControlRegion_Plotter::~HNL_ControlRegion_Plotter(){

  DeleteEvMVA();
}
