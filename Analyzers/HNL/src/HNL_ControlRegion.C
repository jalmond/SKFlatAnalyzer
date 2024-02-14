#include "HNL_ControlRegion.h"

void HNL_ControlRegion::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  SetupEventMVAReader();

  nLog = 100000;
}


void HNL_ControlRegion::executeEvent(){

  if(!(_jentry%10000)) run_Debug=true;
  else run_Debug=false;

  ///// LIST IDs to run
  vector<TString> LepIDs = {"HNL_ULID"};//"TopHN","HNL_ULID","HNTightV2"};//,"TopHN", "DefaultPOGTight"};
  //// List Channels to run
  vector<HNL_LeptonCore::Channel> ChannelsToRun = {MuMu,EE};//,EMu,MuE };
  
  vector<TString> RegionsToPlot = {"SSMultiLep","Dilepton"}; 
  
  ///// Run command 

  vector<TString> CRToRun;
  bool StandardProcess = true;
  if(HasFlag("Dilepton"))        CRToRun = {"OS_VR","SS_CR","VBF_CR","LLL_VR"};
  else if(HasFlag("SSMultiLep")) CRToRun = {"SS_CR","VBF_CR","LLL_VR"};
  else if(HasFlag("ScanFakes"))  { StandardProcess=false ; CRToRun = {"SS_CR"};}
  else CRToRun = {"SS_CR"};

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

  if(HasFlag("ScanFakes")){
    
    for (auto id: LepIDs){
      for(auto channel : ChannelsToRun){
	      
	vector<TString> AJetPt = {"AJ30","AJ40"};
	
	if(id=="HNL_ULID"){
	  
	  if(channel == MuMu){
	    
	    vector<TString> MuFakeIDs = {"HNL_ULID_FO_v1_"+GetYearString(),"HNL_ULID_FO_v2_"+GetYearString(),"HNL_ULID_FO_v3_"+GetYearString(),"HNL_ULID_FO_v4_"+GetYearString(),"HNL_ULID_FO_v5_"+GetYearString()};
	    
	    vector<TString> FakeTag   = {"HNL_ULID_FO_v1","HNL_ULID_FO_v2","HNL_ULID_FO_v3","HNL_ULID_FO_v4","HNL_ULID_FO_v5"};
	    vector<TString> FakeParam = {"Pt","PtParton","PtCorr"};
	    vector<TString> FakeMethod= {"Standard"};                                                              
	    
	    for(unsigned int i= 0 ; i < FakeTag.size(); i++){
	      for(unsigned int j= 0 ; j < FakeParam.size(); j++){
		for(unsigned int k= 0 ; k < FakeMethod.size(); k++){
		  for(auto iaj : AJetPt){
		    if(FakeMethod[k] == "BDTFlavour" && FakeTag[i] !=  "HNL_ULID_FO") continue;
		    AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(id,channel);
		    param_signal.PlottingVerbose = -1;
		    param_signal.FakeRateMethod = FakeMethod[k]; 
		    param_signal.FakeRateParam  = FakeParam[j]; 
		    param_signal.Muon_FR_ID     = MuFakeIDs[i] ; 
		    param_signal.k.Muon_PR      = "pt_eta_"+FakeTag[i]+"_PR_cent";
		    param_signal.k.Muon_FR      = FakeTag[i]+"_"+iaj;
		    param_signal.ApplyPR        = false;
		    param_signal.DefName        = param_signal.DefName+"_"+FakeTag[i]+"_"+param_signal.FakeRateName()+"_"+iaj;
		    param_signal.Name           = param_signal.DefName+"_"+FakeTag[i]+"_"+param_signal.FakeRateName()+"_"+iaj;
		    
		    RunControlRegions(param_signal , CRToRun );
		  }
		}
	      }
	    }
	  }
	  else{
	    
	    
	    vector<TString> ElFakeIDs = {"HNL_ULID_FO_v1_"+GetYearString(),"HNL_ULID_FO_v2_"+GetYearString(),"HNL_ULID_FO_v3_"+GetYearString(),"HNL_ULID_FO_v4_"+GetYearString(),"HNL_ULID_FO_v5_"+GetYearString()};

            vector<TString> FakeTag   = {"HNL_ULID_FO_v1","HNL_ULID_FO_v2","HNL_ULID_FO_v3","HNL_ULID_FO_v4","HNL_ULID_FO_v5"};
            vector<TString> FakeParam = {"Pt","PtParton","PtCorr"};
            vector<TString> FakeMethod= {"Standard"};
	    
	    
	    for(unsigned int i= 0 ; i < FakeTag.size(); i++){
	      for(unsigned int j= 0 ; j < FakeParam.size(); j++){
		for(unsigned int k= 0 ; k < FakeMethod.size(); k++){
		  if(FakeMethod[k] == "BDTFlavour" && FakeTag[i] !=  "HNL_ULID_FO_"+GetYearString()) continue;
		  for(auto iaj : AJetPt){
		    
		    AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(id,channel);
		    if(FakeTag[i] == "HNL_ULLID_FOv3") param_signal.Electron_Tight_ID = "HNL_ULLID";
		    param_signal.PlottingVerbose = -1;
		    param_signal.FakeRateMethod = FakeMethod[k];
		    param_signal.FakeRateParam  = FakeParam[j];
		    param_signal.Electron_FR_ID     = ElFakeIDs[i] ;
		    param_signal.k.Electron_PR      = "pt_eta_"+FakeTag[i]+"_PR_cent";
		    param_signal.k.Electron_FR      = FakeTag[i]+"_"+iaj;
		    param_signal.ApplyPR        = false;
		    param_signal.Name           = param_signal.DefName+"_"+FakeTag[i]+"_"+param_signal.FakeRateName()+"_"+iaj;
		    param_signal.DefName           = param_signal.DefName+"_"+FakeTag[i]+"_"+param_signal.FakeRateName()+"_"+iaj;
		    RunControlRegions(param_signal , CRToRun );
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  /// For BDT plots 

  if(HasFlag("BDTCheck")) {
    AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("SignalStudy");
    vector<TString> IDs = {"HNL_ULID_Baseline"};
    TString param_signal_name = param_signal.Name;
    for (auto id: IDs){
      param_signal.Electron_Tight_ID = id;
      param_signal.Muon_Tight_ID = id;
      param_signal.Name = param_signal_name + id;
      param_signal.DefName = param_signal_name + id;
      param_signal.k.Electron_ID_SF = "Default";
      param_signal.Muon_FR_ID = "HNL_ULID_Baseline";
      param_signal.Electron_FR_ID = "HNL_ULID_Baseline";
      RunControlRegions(param_signal , {"BDTCheck"});

      return;
    }
  }

  return;
  
}

void HNL_ControlRegion::RunControlRegions(AnalyzerParameter param, vector<TString> CRs){


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

  std::vector<Muon>       MuonTightColl = MuonTightCollInit;//;      =  GetLepCollByRunType    (MuonTightCollInit,    param);  
  std::vector<Electron>   ElectronTightColl = ElectronTightCollInit;//;  =  GetLepCollByRunType    (ElectronTightCollInit,param);
 

  Particle METv = GetvMET("PuppiT1xyULCorr",param); // returns MET with systematic correction                                                                      

  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets(param.AK8JetColl,param);
  std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);
  std::vector<Jet>    AK4_VBF_JetColl             = GetHNLJets(param.AK4VBFJetColl,  param);
  std::vector<Jet>    AK4_JetAllColl              = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet>    AK4_JetCollLoose            = GetHNLJets("Loose",     param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param);
  
  EvalJetWeight(AK4_JetColl, AK8_JetColl, weight, param);

  bool DrawBasicPlotsL (true);
  bool DrawBasicPlotsT (true);
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
      FillHist("SimplePlots/Tight_TriMuon", 1 ,  1, 200, 0 ,5 );
      double MassMinOSSF = GetMassMinOSSF(MakeLeptonPointerVector(MuonTightColl));
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("SimplePlots/Tight_TriMuon_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("SimplePlots/Tight_TriMuon1_Pt",  MuonVetoColl[0].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/Tight_TriMuon2_Pt",  MuonVetoColl[1].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/Tight_TriMuon3_Pt",  MuonVetoColl[2].Pt() ,  1, 50, 0 ,200 );
      PassTriLep=true;
    }
    else   if(ElectronTightColl.size() == 3) {
      double MassMinOSSF = GetMassMinOSSF(MakeLeptonPointerVector(ElectronTightColl));
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("SimplePlots/Tight_TriEl", 1 ,  1, 200, 0 ,5 );
      FillHist("SimplePlots/Tight_TriEl_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("SimplePlots/Tight_TriEl1_Pt",  ElectronVetoColl[0].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/Tight_TriEl2_Pt",  ElectronVetoColl[1].Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/Tight_TriEl3_Pt",  ElectronVetoColl[2].Pt() ,  1, 50, 0 ,200 );
      PassTriLep=true;

    }
    else if(LepsAll.size() == 3){
      double MassMinOSSF = GetMassMinOSSF(LepsAll);
      if(MassMinOSSF > 50) MassMinOSSF = 49;
      FillHist("SimplePlots/Tight_TriEMu", 1 ,  1, 200, 0 ,5 );
      FillHist("SimplePlots/Tight_TriEMu_MinOSSF", MassMinOSSF ,  1, 50, 0 ,50 );
      FillHist("SimplePlots/Tight_TriEMu1_Pt",  LepsAll[0]->Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/Tight_TriEMu2_Pt",  LepsAll[1]->Pt() ,  1, 50, 0 ,200 );
      FillHist("SimplePlots/Tight_TriEMu3_Pt",  LepsAll[2]->Pt() ,  1, 50, 0 ,200 );
      //      PassTriLep=true;

    }
  }

  if(CRs.size() == 0) return;
  
  
  RunAllControlRegions(ElectronTightColl,ElectronVetoColl,MuonTightColl,MuonVetoColl, 
		       AK4_JetAllColl, AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl, 
		       ev,METv, param, CRs,weight);
  

}




HNL_ControlRegion::HNL_ControlRegion(){

  SetupEvMVA();
}
 
HNL_ControlRegion::~HNL_ControlRegion(){

  DeleteEvMVA();
}
