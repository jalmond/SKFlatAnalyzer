#include "HNL_ControlRegionPlotter.h"

void HNL_ControlRegionPlotter::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_ControlRegionPlotter::executeEvent(){


  //if(!(_jentry%1000)) run_Debug=true;
  //else run_Debug=false;

  Event ev = GetEvent();

  if(!IsData)  gens = GetGens();

  //AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("MVAUL","_UL");
  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNL","_UL");

  RunControlRegions(param_signal);

  //if(!IsData) RunSyst=true;
  if(RunSyst){
    TString param_signal_name = param_signal.Name;
    vector<AnalyzerParameter::Syst> SystList = GetSystList("Initial");
    
    for(auto isyst : SystList){
      param_signal.syst_ = AnalyzerParameter::Syst(isyst);

      param_signal.Name = "Syst_"+param_signal.GetSystType()+param_signal_name;
      param_signal.DefName = "Syst_"+param_signal.GetSystType()+param_signal_name;

      RunControlRegions(param_signal);
    }
  }


}

void HNL_ControlRegionPlotter::RunControlRegions(AnalyzerParameter param){


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
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);

  TString el_ID = (RunFake||RunPromptTLRemoval) ?  param.Electron_FR_ID : param.Electron_Tight_ID ;
  TString mu_ID = (RunFake||RunPromptTLRemoval) ?  param.Muon_FR_ID :  param.Muon_Tight_ID ;

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

  std::vector<Muon>       MuonCollTInit = GetMuons    ( param,mu_ID, Min_Muon_Pt, 2.4, false);
  std::vector<Electron>   ElectronCollTInit = GetElectrons( param,el_ID, Min_Electron_Pt, 2.5, false)  ;

  //cout << "MuonCollTInit size = " << MuonCollTInit.size() << endl;
  //cout << "ElectronCollTInit size = " << ElectronCollTInit.size() << endl;

  std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( MuonCollTInit,gens,param,"");  
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,gens,param,"");


  if(run_Debug) {
    cout << "Number of Tight Muons = " << MuonCollT.size() << endl;
    cout << "Number of Tight Electrons = " << ElectronCollT.size() << endl;
  }

  // Creat Lepton vector to have lepton blind codes                                                                                                          

  //std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(MuonCollT,ElectronCollT,param);
  //std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV,param);

  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets("HNL",param);
  std::vector<Jet>    AK4_JetAllColl              = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet>    AK4_JetColl                 = GetHNLJets("Tight",param);
  std::vector<Jet>    AK4_JetCollLoose            = GetHNLJets("Loose",param);
  std::vector<Jet>    AK4_VBF_JetColl             = GetHNLJets("VBFTight",param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJetM",param);
  std::vector<Jet>    AK4_BJetCollSR1             = GetHNLJets("BJetT",param);

  Particle METv = GetvMET("PuppiT1xyCorr", param, AK4_VBF_JetColl, AK8_JetColl, MuonCollT, ElectronCollT); // returns MET with systematic correction; run this after all object selection done; NOTE that VBF jet is used here

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
  
  RunAllControlRegions(ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, AK4_JetAllColl, AK4_JetColl,AK4_VBF_JetColl,AK8_JetColl, AK4_BJetColl, ev,METv, param, weight);

  

}




HNL_ControlRegionPlotter::HNL_ControlRegionPlotter(){


}
 
HNL_ControlRegionPlotter::~HNL_ControlRegionPlotter(){

}
