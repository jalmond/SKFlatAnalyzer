#include "HNL_ControlRegionPlotter.h"

void HNL_ControlRegionPlotter::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  //SetupIDMVAReaderDefault(); /// Not needed for BDT skim                                                                                                                                                                                                                  
  //SetupEventMVAReader();

}


void HNL_ControlRegionPlotter::executeEvent(){


  if(!(_jentry%10000)) run_Debug=true;
  else run_Debug=false;

  Event ev = GetEvent();

  vector<TString> LooseIDs = {"HNL_ULID_Baseline",    "HNL_ULID_FO",   "HNLooseV1"};
  vector<TString> TightIDs = {"HNL_ULID_2017_Looser", "HNL_ULID_2017", "HNTightV2"};
  std::map<int, TString> LFcutmap;
  LFcutmap[1] = "0";
  LFcutmap[2] = "0p8";

  for(int i=0; i<LooseIDs.size(); i++){

    if(TightIDs.at(i).Contains("HNTight")){

      AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("MVAUL","_PtCone");
      //redefine FR ID, FR key, Tight IDs + ptcone setting
      param_signal.Name           = param_signal.Name+"_"+TightIDs.at(i);
      param_signal.DefName        = param_signal.DefName+"_"+TightIDs.at(i);
      param_signal.Muon_FR_ID     = LooseIDs.at(i);
      param_signal.Muon_FR_Key    = "ptcone_eta_AwayJetPt40";
      param_signal.Muon_UsePtCone = true;
      param_signal.Muon_Tight_ID  = TightIDs.at(i);
      //RunControlRegions(param_signal , {"CR_OS_Z","CR_OS_ZAk8","CR_OS_Top","CR_OS_TopAk8","CR_WZ","CR_ZZ"});
      RunControlRegions(param_signal , {"CR_WZ","CR_ZZ"}); // JH : OS later

    }

    for(int doSep = 0; doSep < 3; doSep++){

      AnalyzerParameter param_signal = doSep ? HNL_LeptonCore::InitialiseHNLParameter("MVAUL","_LFvsHF_cut"+LFcutmap[doSep]) : HNL_LeptonCore::InitialiseHNLParameter("MVAUL","");
      //redefine FR ID, FR key, Tight IDs
      param_signal.Name          = param_signal.Name+"_"+TightIDs.at(i);
      param_signal.DefName       = param_signal.DefName+"_"+TightIDs.at(i);
      param_signal.Muon_FR_ID    = LooseIDs.at(i);
      param_signal.Muon_FR_Key   = "pt_eta_AwayJetPt40";
      param_signal.Muon_Tight_ID = TightIDs.at(i);
      //RunControlRegions(param_signal , {"CR_OS_Z","CR_OS_ZAk8","CR_OS_Top","CR_OS_TopAk8","CR_WZ","CR_ZZ"});
      RunControlRegions(param_signal , {"CR_WZ","CR_ZZ"}); // JH : OS later

    }
  }

/*
  vector<TString> IDs = {};//"HNL_ULID_Baseline";
  
  TString param_signal_name = param_signal.Name;
  for (auto id: IDs){
    param_signal.Electron_Tight_ID = id;
    param_signal.Muon_Tight_ID = id;
    param_signal.Name = param_signal_name + id;
    param_signal.DefName = param_signal_name + id;
    param_signal.Electron_ID_SF_Key = "Default";
    param_signal.Muon_FR_ID = "HNL_ULID_Baseline";
    param_signal.Electron_FR_ID = "HNL_ULID_Baseline";
    param_signal.Muon_FR_Key  ="ptcone_eta_AwayJetPt40";
    param_signal.Electron_FR_Key  = "ptcone_eta_AwayJetPt40";
   
    RunControlRegions(param_signal , {"BDTCheck"});
    
    return;
  }
  
  vector<TString> ELIDs = {"HNL_ULID_Run2","passMVAID_noIso_WP90","HNTightV2"};
  for (auto id: ELIDs){
    AnalyzerParameter param_signal2 = HNL_LeptonCore::InitialiseHNLParameter("HNL");
    param_signal2.Name = param_signal_name + id;
    param_signal2.FakeMethod = "MC";
    param_signal2.CFMethod   = "MC";
    param_signal2.ConvMethod = "MC";

    param_signal2.Electron_Tight_ID = id;
    if(id=="HNL_ULID_Run2")param_signal2.Electron_ID_SF_Key = "passHNL_ULID_Run2";
    else if(id=="HNTightV2")param_signal2.Electron_ID_SF_Key = "passHNTightV2";
    else  param_signal2.Electron_ID_SF_Key = "Default";
    
    param_signal2.Muon_ID_SF_Key = "NUM_HNL_ULID_"+GetYearString();
    param_signal2.Muon_Tight_ID  = "HNL_ULID_"+GetYearString();
    param_signal2.Muon_RECO_SF_Key = "MuonRecoSF";
    RunControlRegions(param_signal2 , {"CR_OS_Z","CR_OS_Top","CR_WZ"});
  }
  
  return;
*/

}

void HNL_ControlRegionPlotter::RunControlRegions(AnalyzerParameter param, vector<TString> CRs){


  if(_jentry==0) PrintParam(param);
  
 
  if(run_Debug) {
    if(RunFake)  cout << "HNL_ControlRegionPlotter::RunControlRegions [RunFake]" << endl;
    else if(RunCF)  cout << "HNL_ControlRegionPlotter::RunControlRegions [RunCF]" << endl;
    else if(RunConv)  cout << "HNL_ControlRegionPlotter::RunControlRegions [RunConv]" << endl;
    else if(RunPromptTLRemoval)  cout << "HNL_ControlRegionPlotter::RunControlRegions [RunFake]" << endl;
    else  cout << "HNL_ControlRegionPlotter::RunControlRegions [Standard]" << endl;
  }

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);
  if(run_Debug) cout << "HNL_ControlRegionPlotter::RunControlRegions Weight = " << weight << endl;
  

  // HL ID                                                                                                                                                   
  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID,     5.,  2.4);

  TString el_ID = (RunFake) ?  param.Electron_FR_ID  : param.Electron_Tight_ID ;
  TString mu_ID = (RunFake) ?  param.Muon_FR_ID      : param.Muon_Tight_ID ;
  if(run_Debug) {
    if(RunFake){
      cout << "param.Muon_FR_ID = "  << param.Muon_FR_ID  << endl;
      cout << "param.Muon_FR_Key = " << param.Muon_FR_Key << endl;
    }
  }

  //// OS CHECK TMP FAKE
  //  el_ID= param.Electron_Tight_ID;
  //mu_ID =param.Muon_Tight_ID;

  double Min_Muon_Pt     = (RunFake) ? 8. : 10.;
  double Min_Electron_Pt = (RunFake) ? 8. : 10.;

  if(run_Debug) {
    cout << "Min_Muon_Pt = " << Min_Muon_Pt << endl;
    cout << "Min_Electron_Pt = " << Min_Electron_Pt << endl;
  }

  // 3 Methods to run MC
  // 1) Fakes/ CF are done by data and Conv done my MC
  // - in this case if MC should remove Fake lep and CF leps
  // - this is done by Adding option 
  // std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( GetMuons    ( param,mu_ID, Min_Muon_Pt, 2.4, RunFake)      ,gens,param,"");
  //std::vector<Electron>   ElectronCollT = GetLepCollByRunType    ( GetElectrons( param,el_ID, Min_Electron_Pt, 2.5, RunFake)  ,gens,param,"");

  std::vector<Muon>       MuonCollTInit = GetMuons        ( param,mu_ID, Min_Muon_Pt,     2.4, false); // false : No replacement with PtCone
  std::vector<Electron>   ElectronCollTInit = GetElectrons( param,el_ID, Min_Electron_Pt, 2.5, false);

  if(run_Debug) {
    cout << "MuonCollTInit size = " << MuonCollTInit.size() << endl;
    cout << "ElectronCollTInit size = " << ElectronCollTInit.size() << endl;
  }
  std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( MuonCollTInit, param); // prompt + conversion, do nothing for data
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,param);

  if(run_Debug) {
    cout << "Number of Tight Muons = " << MuonCollT.size() << endl;
    cout << "Number of Tight Electrons = " << ElectronCollT.size() << endl;
  }

  // Creat Lepton vector to have lepton blind codes                                                                                                          

  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets("HNL",param);
  std::vector<Jet>    AK4_JetAllColl              = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet>    AK4_JetColl                 = GetHNLJets("Tight",     param);
  std::vector<Jet>    AK4_JetCollLoose            = GetHNLJets("Loose",     param);
  std::vector<Jet>    AK4_VBF_JetColl             = GetHNLJets("VBFTight",  param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJetM",     param);
  std::vector<Jet>    AK4_BJetCollSR1             = GetHNLJets("BJetT",     param);

  Particle METv = GetvMET("PuppiT1xyULCorr", param, AK4_VBF_JetColl, AK8_JetColl, MuonCollT, ElectronCollT); // returns MET with systematic correction; run this after all object selection done; NOTE that VBF jet is used here //JH FIXME what's the difference between GetvMETs?????
  if(run_Debug) cout << "PuppiT1xyULCorr = " << METv.Pt() << endl;

  TString PUIDWP="";
  if(PUIDWP != ""){
    double PJet_PUID_weight = GetJetPileupIDSF(AK4_JetColl, PUIDWP, param);
    weight*= PJet_PUID_weight;
    FillWeightHist("PJet_PUID_weight_" ,PJet_PUID_weight);
  }

  // Get BJet collection
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  double sf_btag               = GetBJetSF(param,  AK4_JetColl, param_jets);
  if(!IsData )weight*= sf_btag;

  RunAllControlRegions(ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, AK4_JetAllColl, AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl, ev,METv, param, CRs,weight);
  

}




HNL_ControlRegionPlotter::HNL_ControlRegionPlotter(){


}
 
HNL_ControlRegionPlotter::~HNL_ControlRegionPlotter(){

}
