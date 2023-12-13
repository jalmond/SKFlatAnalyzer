#include "HNL_ControlRegionPlotter.h"

void HNL_ControlRegionPlotter::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  SetupEventMVAReader();

  nLog = 100000;
}


void HNL_ControlRegionPlotter::executeEvent(){

  if(!(_jentry%10000)) run_Debug=true;
  else run_Debug=false;

  vector<TString> LepIDs = {"HNL_ULID","HNTightV2"};//,"TopHN"};//, "DefaultPOGTight"};

  vector<HNL_LeptonCore::Channel> ChannelsToRun = {EE, MuMu};//,EMu,MuE};

  for (auto id: LepIDs){
    for(auto channel : ChannelsToRun){
     
      ///// Run command
      vector<TString> CRToRun;
      if(HasFlag("OS_VR"))   CRToRun.push_back("OS_VR");
      if(HasFlag("LLL_VR"))  CRToRun.push_back("LLL_VR");
      if(HasFlag("SS_CR"))   CRToRun.push_back("SS_CR");
      if(HasFlag("VBF_CR"))  CRToRun.push_back("VBF_CR");
      
      vector<TString> AJetPt = {"AJ30"};

      if(id=="HNL_ULID"){
  
        if(channel == MuMu){

          
          //vector<TString> MuFakeIDs = {"HNL_ULID_FO","HNL_ULID_FOv2_"+GetYearString()};// "HNL_ULID_FOv3_"+GetYearString(), "HNL_ULID_FOv4_"+GetYearString(),"HNL_ULID_FOv5_"+GetYearString(), "HNL_ULID_FOv6_"+GetYearString(),  "HNL_ULID_FOv7_"+GetYearString()};
          
          //vector<TString> FakeTag   = {"HNL_ULID_FO","HNL_ULID_FOv2"};//, "HNL_ULID_FOv3", "HNL_ULID_FOv4","HNL_ULID_FOv5", "HNL_ULID_FOv6", "HNL_ULID_FOv7"};
          //vector<TString> FakeParam = {"Pt","PtParton"};
          //vector<TString> FakeMethod= {"BDTFlavour","Standard"};
          vector<TString> MuFakeIDs =  {"HNL_ULID_FOv2_"+GetYearString()};
          vector<TString> FakeTag   = {"HNL_ULID_FOv2"};
          vector<TString> FakeParam = {"Pt"};
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
                  //param_signal.Name           = param_signal.DefName+"_"+FakeTag[i]+"_"+param_signal.FakeRateName()+"_"+iaj;
                  
                  RunControlRegions(param_signal , CRToRun );
                }
              }
            }
          }
        }
        else{
    
    
    //vector<TString> ElFakeIDs = {"HNL_ULID_FO_"+GetYearString(),"HNL_ULID_FOv2_"+GetYearString()};//, "HNL_ULID_FOv3_"+GetYearString()};//, "HNL_ULID_FOv4_"+GetYearString(),"HNL_ULID_FOv5_"+GetYearString(), "HNL_ULID_FOv6_"+GetYearString(), "HNL_ULLID_FOv3"};
          //vector<TString> FakeTag   = {"HNL_ULID_FO","HNL_ULID_FOv2"};//, "HNL_ULID_FOv3", "HNL_ULID_FOv4","HNL_ULID_FOv5", "HNL_ULID_FOv6","HNL_ULLID_FOv3"};
          //vector<TString> FakeParam = {"Pt","PtParton"};
          //vector<TString> FakeMethod= {"BDTFlavour","Standard"};

          vector<TString> ElFakeIDs = {"HNL_ULID_FOv2_"+GetYearString()};
          vector<TString> FakeTag   = {"HNL_ULID_FOv2"};
          vector<TString> FakeParam = {"PtParton"}; 
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
                  //param_signal.Name           = param_signal.DefName+"_"+FakeTag[i]+"_"+param_signal.FakeRateName()+"_"+iaj;
                  RunControlRegions(param_signal , CRToRun );
                }
              }
            }
          }
        }
      }
      else {
        AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(id,channel);
        RunControlRegions(param_signal , CRToRun );
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

void HNL_ControlRegionPlotter::RunControlRegions(AnalyzerParameter param, vector<TString> CRs){


  //  if(_jentry==0) param.PrintParameters();
  run_Debug = (_jentry%nLog==0);

  if(run_Debug) cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
  
  Event ev = GetEvent();

  /// SetupWeight applies w_GenNorm=1., w_BR=1., w_PU  w_Pref  
  double weight =SetupWeight(ev,param);
  
  // HL ID                                                                                                                                                   
  std::vector<Electron>   ElectronVetoColl = GetElectrons(param.Electron_Veto_ID, 10.,  2.5);
  std::vector<Muon>       MuonVetoColl     = GetMuons    (param.Muon_Veto_ID,     10.,  2.4);

  /// IF ruunning fake then use FR_ID not Tight
  TString Electron_ID = SetLeptonID("Electron",param);
  TString Muon_ID     = SetLeptonID("Muon", param);

  double Min_Muon_Pt     = RunFake ? 6  : 10.;
  double Min_Electron_Pt = RunFake ? 10 : 15;

  std::vector<Muon>       MuonTightCollInit     = SelectMuons    ( param,Muon_ID,     Min_Muon_Pt,     2.4,weight); 
  std::vector<Electron>   ElectronTightCollInit = SelectElectrons( param,Electron_ID, Min_Electron_Pt, 2.5,weight);

  std::vector<Muon>       MuonTightColl      =  GetLepCollByRunType    (MuonTightCollInit,    param);  
  std::vector<Electron>   ElectronTightColl  =  GetLepCollByRunType    (ElectronTightCollInit,param);

  //  for(auto iel : ElectronTightColl ) cout << "EL " << iel.IsPrompt() << " eta = " << iel.Eta() << " " << weight << endl;

  Particle METv = GetvMET("PuppiT1xyULCorr",param); // returns MET with systematic correction                                                                      

  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets(param.AK8JetColl,param);
  std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);
  std::vector<Jet>    AK4_VBF_JetColl             = GetHNLJets(param.AK4VBFJetColl,  param);
  std::vector<Jet>    AK4_JetAllColl              = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet>    AK4_JetCollLoose            = GetHNLJets("Loose",     param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param);
  
  EvalJetWeight(AK4_JetColl, AK8_JetColl, weight, param);
  
  if(0){
    std::vector<Muon>       MuonTightColl1    = SelectMuons    ( param, "HNL_ULID_FO",     Min_Muon_Pt,     2.4,weight);
    std::vector<Muon>       MuonTightColl2    = SelectMuons    ( param, "HNL_ULID_POGM_FO",     Min_Muon_Pt,     2.4,weight);
    std::vector<Muon>       MuonTightColl3    = SelectMuons    ( param, "HNL_ULID_FOv3_"+GetYearString(),     Min_Muon_Pt,     2.4,weight);
    
    if(param.Channel == "EE"){
      if(ElectronVetoColl.size() == 2 && ElectronTightColl.size() == 2 && MuonTightColl.size() ==0){
  if(SameCharge(ElectronTightColl)) FillFakeHistograms(param, "ElFakeStudies", MakeLeptonPointerVector(MuonTightColl, ElectronTightColl, param),  AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,weight);                                       }
    }
    if(param.Channel == "MuMu"){
      if(MuonVetoColl.size() == 2 && ElectronTightColl.size() == 0 && MuonTightColl.size() ==2){
  if(SameCharge(MuonTightColl)) FillFakeHistograms(param, "MuonFakeStudies", MakeLeptonPointerVector(MuonTightColl, ElectronTightColl, param),  AK4_JetColl,AK8_JetColl, AK4_BJetColl, METv,weight);                                   
      }
    }
  }

  if(CRs.size() == 0) return;

  RunAllControlRegions(ElectronTightColl,ElectronVetoColl,MuonTightColl,MuonVetoColl, 
           AK4_JetAllColl, AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl, 
           ev,METv, param, CRs,weight);
  

}




HNL_ControlRegionPlotter::HNL_ControlRegionPlotter(){

  SetupEvMVA();
}
 
HNL_ControlRegionPlotter::~HNL_ControlRegionPlotter(){

  DeleteEvMVA();
}
