#include "HNL_SignalRegionPlotter.h"

void HNL_SignalRegionPlotter::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  
  SetupIDMVAReaderDefault(); /// Not needed for BDT skim
  SetupEventMVAReader();                                                                                                                                                                                                                            
}


void HNL_SignalRegionPlotter::executeEvent(){

  FillTimer("START_EV");
  
  if((_jentry==0)){ 
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer
    TriggerPrintOut(GetEvent());
  }
  
  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("MVAUL","_UL");
  if(RunFakeClosurePred)     param_signal.Muon_FR_Key = "PtPartonQCD_eta_TriLepQCD"; //JH
  else if(RunFakeClosureObs) param_signal.FakeMethod = "MC"; //JH
  RunULAnalysis(param_signal);

  //if(!IsData) RunSyst=true;
  if(!IsData) RunSyst=false; //FIXME later
  if(RunSyst){
    TString param_signal_name = param_signal.Name;
    //vector<AnalyzerParameter::Syst> SystList;// = GetSystList("Initial");
    vector<AnalyzerParameter::Syst> SystList = GetSystList("Initial");
    //vector<AnalyzerParameter::Syst> SystList = GetSystList("All");

    for(auto isyst : SystList){
      param_signal.syst_ = AnalyzerParameter::Syst(isyst);
      
      param_signal.Name = "Syst_"+param_signal.GetSystType()+param_signal_name;
      param_signal.DefName = "Syst_"+param_signal.GetSystType()+param_signal_name;
      RunULAnalysis(param_signal);
    }
  }    

  FillTimer("END_EV");

  return ;
}

void HNL_SignalRegionPlotter::RunULAnalysis(AnalyzerParameter param){

  if(run_Debug) cout << "HNL_SignalRegionPlotter::executeEvent " << endl;

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  ///// MERGE WJet samples for more stats                                                                                                                                      
  //if(MCSample.Contains("WJet")){
  //  vector<TString> vec = {"WJet"};
  //  double merge_weight = MergeMultiMC( vec, "" );
  //  weight*= merge_weight;
  // }

  /// Merge DY samples for more stats                                                                                                                                        
  //if(MCSample.Contains("DYJets_MG")){
  //  vector<TString> vec = {"DYMG"};
  //  double merge_weight = MergeMultiMC( vec, "" );
  //  weight*= merge_weight;
  // }


  
  // veto leptons
  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5); 
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);

  TString el_ID = (RunFake) ?  param.Electron_FR_ID : param.Electron_Tight_ID ;
  TString mu_ID = (RunFake) ?  param.Muon_FR_ID :  param.Muon_Tight_ID ;

  double Min_Muon_Pt     = (RunFake) ? 3. : 5.;
  double Min_Electron_Pt = (RunFake) ? 7. : 10.;

  std::vector<Muon>       MuonCollTInit = GetMuons    ( param,mu_ID, Min_Muon_Pt, 2.4, false);
  std::vector<Electron>   ElectronCollTInit = GetElectrons( param,el_ID, Min_Electron_Pt, 2.5, false)  ;
 
  std::vector<Muon>       MuonCollT;
  std::vector<Electron>   ElectronCollT;
  if(RunFakeClosure){
    MuonCollT     = MuonCollTInit;
    ElectronCollT  =  ElectronCollTInit; //JH : for closure test, pretend the MCs are data
  }
  else{
    MuonCollT     = GetLepCollByRunType    ( MuonCollTInit ,param);
    ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,param);
  }

  //cout << "Number of MuonCollT = " << MuonCollT.size() << " number of electrons = " << ElectronCollT.size() << endl;

  std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);
  std::vector<Tau>        TauColl        = GetTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets("HNL",param);
  std::vector<Jet> AK4_JetAllColl                 = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet> JetColl                        = GetHNLJets("Tight",param);
  std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param);
  std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param);
  std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param);
  //  std::vector<Jet> BJetCollSR1                    = GetHNLJets("BJetT",param);
 
  Particle METv = GetvMET("PuppiT1xyULCorr",param); // returns MET with systematic correction; run this after all object selection done; NOTE that VBF jet is used here

  // select B jets                                                                                                                                                                        
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  double sf_btag               = GetBJetSF(param, JetColl, param_jets);
  
  JetTagging::Parameters param_jetsT = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets);
  double sf_btagSR1               = GetBJetSF(param, JetColl, param_jetsT);
  //cout << "sf_btag : "    << sf_btag    << endl;
  //cout << "sf_btagSR1 : " << sf_btagSR1 << endl; //JH

  if(!IsData){
    weight = weight*sf_btag;
    FillWeightHist(param.Name+"/BtagSF",sf_btag);
  }

  FillTimer("START_SR");

  RunAllSignalRegions(Inclusive,
          ElectronCollT,ElectronCollV,MuonCollT,MuonCollV,  TauColl,
          JetCollLoose, AK4_JetAllColl, JetColl,VBF_JetColl,AK8_JetColl, BJetColl,BJetColl, 
          ev,METv, param, weight);

  FillTimer("END_SR");


}



 


HNL_SignalRegionPlotter::HNL_SignalRegionPlotter(){

  cout << "HNL_SignalRegionPlotter::HNL_SignalRegionPlotter  TMVA::Tools::Instance() " << endl;
  TMVA::Tools::Instance();
  cout << "Create Reader class " << endl;
  MVAReaderMM = new TMVA::Reader();
  MVAReaderEE = new TMVA::Reader();
  MVAReaderEM = new TMVA::Reader();
  
}
 
HNL_SignalRegionPlotter::~HNL_SignalRegionPlotter(){

  delete MVAReaderMM;
  delete MVAReaderEE;
  delete MVAReaderEM;

}




