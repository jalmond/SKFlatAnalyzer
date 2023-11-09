#include "HNL_SignalRegionPlotter17028.h"

void HNL_SignalRegionPlotter17028::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_SignalRegionPlotter17028::executeEvent(){
  
  if((_jentry==0)){ 
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer
    TriggerPrintOut(GetEvent());
  }
  
  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNL");
  RunEXO17028Analysis(param_signal);

  return ;
}

void HNL_SignalRegionPlotter17028::RunEXO17028Analysis(AnalyzerParameter param){


  if(run_Debug) cout << "HNL_SignalRegionPlotter17028::executeEvent " << endl;

  
  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  
  // HL ID
  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5); 
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);

  TString el_ID = (RunFake) ?  param.Electron_FR_ID : param.Electron_Tight_ID ;
  TString mu_ID = (RunFake) ?  param.Muon_FR_ID :  param.Muon_Tight_ID ;

  double Min_Muon_Pt     = (RunFake) ? 8. : 10.;
  double Min_Electron_Pt = (RunFake) ? 8. : 10.;


  std::vector<Muon>       MuonCollTInit     = SelectMuons    ( param,mu_ID, Min_Muon_Pt, 2.4);
  std::vector<Electron>   ElectronCollTInit = SelectElectrons( param,el_ID, Min_Electron_Pt, 2.5)  ;

  std::vector<Muon>       MuonCollT      = GetLepCollByRunType    ( MuonCollTInit,param);
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,param);

  std::vector<Tau>        mytaus        = GetTaus     (param.Tau_Veto_ID,20., 2.3); 

  // Creat Lepton vector to have lepton blind codes 

  Particle METv = GetMiniAODvMET("T1xyCorr"); // reyturns MET with systematic correction

  
  std::vector<FatJet> fatjets_tmp                 = SelectFatJets(param, param.FatJet_ID, 200., 5.);
  std::vector<FatJet> AK8_JetColl                  = SelectAK8Jets(fatjets_tmp, 200., 5., true,  1., true, -999, true, 60., 130., "",ElectronCollV, MuonCollV);
    
  
  // AK4 JET                                                                                                                                                                              
  std::vector<Jet> jets_tmp     = SelectJets   ( param, param.Jet_ID, 20., 5.);
  
  TString PUIDWP="loose";
  std::vector<Jet> JetColl                           = SelectAK4Jets(jets_tmp,     20., 2.7, true,  0.4,0.8, PUIDWP,   ElectronCollV,MuonCollV, AK8_JetColl);
  std::vector<Jet> BJetColltmp                       = SelectAK4Jets(jets_tmp,     20., 2.4, false,  0.4,0.8, PUIDWP,   ElectronCollV,MuonCollV, AK8_JetColl);

    
  double PJet_PUID_weight = GetJetPileupIDSF(JetColl, PUIDWP, param);
  weight*= PJet_PUID_weight;
  FillWeightHist("PJet_PUID_weight_" ,PJet_PUID_weight);

  // select B jets                                                                                                                                                                        
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  
  // Get BJets  and EV weight to corr BTag Eff                                                                                                                       
  std::vector<Jet> BJetColl    = SelectBJets(param, BJetColltmp, param_jets);
  double sf_btag               = GetBJetSF(param, BJetColltmp, param_jets);
  if(!IsData )weight*= sf_btag;
  RunEXO17028SignalRegions(ElectronCollT,ElectronCollV,MuonCollT,MuonCollV,  JetColl,AK8_JetColl, BJetColl, ev,METv, param, weight);
  
}


 


HNL_SignalRegionPlotter17028::HNL_SignalRegionPlotter17028(){


}
 
HNL_SignalRegionPlotter17028::~HNL_SignalRegionPlotter17028(){

}




