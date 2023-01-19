#include "HNL_SignalRegionPlotter.h"

void HNL_SignalRegionPlotter::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  cout << "SetupMVAReader " << endl;
  SetupMVAReader();
}


void HNL_SignalRegionPlotter::executeEvent(){
  
  if((_jentry==0)){ 
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer
    TriggerPrintOut(GetEvent());
  }
  
  if(!IsData)  gens = GetGens();

  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNL","_UL");
  RunULAnalysis(param_signal);

  if(!IsData) RunSyst=true;
  if(RunSyst){
    TString param_signal_name = param_signal.Name;
    vector<AnalyzerParameter::Syst> SystList;// = GetSystList("Initial");

    for(auto isyst : SystList){
      param_signal.syst_ = AnalyzerParameter::Syst(isyst);
      
      param_signal.Name = "Syst_"+param_signal.GetSystType()+param_signal_name;
      param_signal.DefName = "Syst_"+param_signal.GetSystType()+param_signal_name;
      RunULAnalysis(param_signal);
    }
  }    


  return ;
}

void HNL_SignalRegionPlotter::RunULAnalysis(AnalyzerParameter param){

  if(run_Debug) cout << "HNL_SignalRegionPlotter::executeEvent " << endl;

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  
  // HL ID
  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5); 
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);

  TString el_ID = (RunFake) ?  param.Electron_FR_ID : param.Electron_Tight_ID ;
  TString mu_ID = (RunFake) ?  param.Muon_FR_ID :  param.Muon_Tight_ID ;

  double Min_Muon_Pt     = (RunFake) ? 3. : 5.;
  double Min_Electron_Pt = (RunFake) ? 7. : 10.;

  std::vector<Muon>       MuonCollTInit = GetMuons    ( param,mu_ID, Min_Muon_Pt, 2.4, RunFake);
  std::vector<Electron>   ElectronCollTInit = GetElectrons( param,el_ID, Min_Electron_Pt, 2.5, RunFake)  ;

  std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( MuonCollTInit,gens,param);
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,gens,param);

  std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

  std::vector<Tau>        TauColl        = GetTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

  // Creat Lepton vector to have lepton blind codes 

  Particle METv = GetvMET("PuppiT1xyCorr",param); // returns MET with systematic correction

  
  std::vector<FatJet> fatjets_tmp                 = GetFatJets(param, param.FatJet_ID, 200., 5.);
  
  std::vector<FatJet> AK8_JetColl                  = SelectAK8Jets(fatjets_tmp, 200., 5., true,  1., false, -999, false, 0., 20000., ElectronCollV, MuonCollV);
  

  // AK4 JET                                                                                                                                                                              
  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);
  
  std::vector<Jet> JetCollLoose                    = SelectAK4Jets(jets_tmp,     15., 4.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, AK8_JetColl);
  
  TString PUIDWP="";
  std::vector<Jet> JetColl                           = SelectAK4Jets(jets_tmp,     20., 2.7, true,  0.4,0.8, PUIDWP,   ElectronCollV,MuonCollV, AK8_JetColl);
  std::vector<Jet> VBF_JetColl                       = SelectAK4Jets(jets_tmp,     30., 4.7, true,  0.4,0.8, PUIDWP,  ElectronCollV,MuonCollV, AK8_JetColl);    // High ETa jets                 
  std::vector<Jet> BJetColltmp                       = SelectAK4Jets(jets_tmp,     20., 2.4, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, AK8_JetColl);
  
  //double PJet_PUID_weight = GetJetPileupIDSF(JetColl, PUIDWP, param);
  //    weight*= PJet_PUID_weight;
  //FillWeightHist("PJet_PUID_weight_" ,PJet_PUID_weight);
  
  
  // select B jets                                                                                                                                                                        
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  
  // Get BJets  and EV weight to corr BTag Eff                                                                                                                       
  std::vector<Jet> BJetColl    = SelectBJets(param, BJetColltmp, param_jets);
  double sf_btag               = GetBJetSF(param, BJetColltmp, param_jets);
  if(!IsData )weight*= sf_btag;
  RunAllSignalRegions(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV,  TauColl,JetCollLoose, JetColl,VBF_JetColl,AK8_JetColl, BJetColl, ev,METv, param, weight);
  


}



 


HNL_SignalRegionPlotter::HNL_SignalRegionPlotter(){

  cout << "HNL_SignalRegionPlotter::HNL_SignalRegionPlotter  TMVA::Tools::Instance() " << endl;
  TMVA::Tools::Instance();
  cout << "Create Reader class " << endl;
  MVAReader = new TMVA::Reader();
  
  
}
 
HNL_SignalRegionPlotter::~HNL_SignalRegionPlotter(){

  delete MVAReader;

}




