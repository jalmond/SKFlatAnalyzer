#include "HNL_LeptonIDSF.h"

void HNL_LeptonIDSF::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_LeptonIDSF::executeEvent(){
  
  if((_jentry==0)){ 
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer
    TriggerPrintOut(GetEvent());
  }
  
  if(!IsData)  gens = GetGens();

  vector<TString> IDs = {"EXO17028","POGTight","HNL"};
  for(auto id : IDs){
    MeasureElectronIDSF(HNL_LeptonCore::InitialiseHNLParameter(id,""));
    MeasureMuonIDSF(HNL_LeptonCore::InitialiseHNLParameter(id ,""));
  }
  return ;
}

void HNL_LeptonIDSF::MeasureMuonIDSF(AnalyzerParameter param){

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  std::vector<Electron>   ElectronCollProbe = GetElectrons("passPOGTight", 10., 2.5);
  std::vector<Muon>       MuonCollProbe     = GetMuons    ("POGLoose", 10., 2.4);


  std::vector<FatJet>   FatjetColl  = GetFatJets(param, "tight", 200., 5.);

  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 2.5);

  std::vector<Jet> JetColl   = SelectAK4Jets(jets_tmp,     20., 2.5, true,  0.4,0.8,"loose",   ElectronCollProbe,MuonCollProbe, FatjetColl)
    ;
  // select B jets
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);

  std::vector<Jet> BJetColl  = SelectBJets(param, jets_tmp, param_jets);
  
  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(MuonCollProbe,ElectronCollProbe);

  // Chose Typ1 Phi corr MET + smear jets 
  //Particle METUnsmearedv = GetvMET("T1xyCorr"); // reyturns MET with systematic correction
  //Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets_tmp); // smears MET
  Particle METv = GetvMET("PuppiT1xyCorr"); // reyturns MET with systematic correction                                                                                                

  if(METv.Pt() > 40) return;
  if(BJetColl.size() > 0 ) return;
  
  if(!PassMETFilter()) return;

  HNL_LeptonCore::Channel dilep_channel = MuMu;

  if(!CheckLeptonFlavourForChannel(dilep_channel, LeptonColl)) return;
  if (!PassTriggerSelection(dilep_channel, ev, LeptonColl,"Dilep")) return;

  MeasureIDSF(dilep_channel, MuonCollProbe, param.Muon_Tight_ID, weight);
 

}

void HNL_LeptonIDSF::MeasureElectronIDSF(AnalyzerParameter param){

  if(run_Debug) cout << "HNL_LeptonIDSF::executeEvent " << endl;
  
  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);
  
  
  std::vector<Electron>   ElectronCollProbe = GetElectrons("passProbeID", 10., 2.5); 
  std::vector<Muon>       MuonCollProbe     = GetMuons    ("POGTightWithTightIso", 10., 2.4);
  if(!IsData){

    for(auto iel : ElectronCollProbe){

      double this_recosf  = mcCorr->ElectronReco_SF(iel.scEta(), iel.Pt(), 0);
      weight*=this_recosf;
      
    }
  }

  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 2.5);

  std::vector<FatJet>   FatjetColl  = GetFatJets(param, "tight", 200., 5.);

  std::vector<Jet> JetColl                        = SelectAK4Jets(jets_tmp,     20., 2.5, true,  0.4,0.8,"loose",   ElectronCollProbe,MuonCollProbe, FatjetColl);
  // select B jets
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);

  std::vector<Jet> BJetColl                       = SelectBJets(param, jets_tmp, param_jets);
  
  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(MuonCollProbe,ElectronCollProbe);

  // Chose Typ1 Phi corr MET + smear jets 
  Particle METv = GetvMET("PuppiT1xyCorr"); // reyturns MET with systematic correction
  // Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets_tmp); // smears MET
  
  if(METv.Pt() > 40) return;
  if(BJetColl.size() > 0 ) return;
  
  if(!PassMETFilter()) return;

  HNL_LeptonCore::Channel dilep_channel= EE;

  if(!CheckLeptonFlavourForChannel(dilep_channel, LeptonColl)) return; // Check if EE cahnnel has 2 el...
  if(!PassTriggerSelection(dilep_channel, ev, LeptonColl,"Dilep")) return; // check if trigger passes and steam is correct and pt applied

  MeasureIDSF(dilep_channel, ElectronCollProbe, param.Electron_Tight_ID, weight);

  return;
  
}
     

void HNL_LeptonIDSF::MeasureIDSF(HNL_LeptonCore::Channel dilep_channel, vector<Muon> MuonColl, TString ID, double weight_ll){

  TString channel_string = GetChannelString(dilep_channel) ;
       
  if(MuonColl.size() != 2) return;
  
  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(MuonColl);

  if(!ZmassOSSFWindowCheck(LeptonColl, 10.)) return;

  int nbin_pt    =11;
  int nbin_eta   =4;
  double ptbins    [nbin_pt    +1] = { 10.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};
  double etabins   [nbin_eta+1   ] = { 0.,0.8,  1.479, 2.,  2.5};

  FillHist(channel_string+"/llmass_"+channel_string, GetLLMass(LeptonColl), weight_ll, 70, 50., 120);
  FillHist(channel_string+"/NVTX_"+channel_string, nPileUp, weight_ll, 100, 0., 100);
  FillHist(channel_string+"/MET_"+channel_string, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 100, 0., 100);

  if(MuonColl[0].PassID(ID)) {
    FillHist(channel_string+"/"+ID+"_denom",MuonColl[1].Pt(), fabs(MuonColl[1].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins);
    if(MuonColl[1].PassID(ID))     FillHist(channel_string+"/"+ID+"_num",MuonColl[1].Pt(), fabs(MuonColl[1].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins);

  }
  if(MuonColl[1].PassID(ID)) {
    FillHist(channel_string+"/"+ID+"_denom",MuonColl[0].Pt(), fabs(MuonColl[0].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins);
    if(MuonColl[0].PassID(ID))     FillHist(channel_string+"/"+ID+"_num",MuonColl[0].Pt(), fabs(MuonColl[0].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins);
    
  }
  

  return;

}

void HNL_LeptonIDSF::MeasureIDSF(HNL_LeptonCore::Channel dilep_channel, vector<Electron> ElectronColl, TString ID, double weight_ll){

  TString channel_string = GetChannelString(dilep_channel) ;

  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(ElectronColl);

  if(LeptonColl.size() != 2) return;

  if(!ZmassOSSFWindowCheck(LeptonColl, 10.)) return;

  int nbin_pt    =11;
  int nbin_eta   =4;
  double ptbins    [nbin_pt    +1] = { 10.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};
  double etabins   [nbin_eta+1   ] = { 0.,0.8,  1.479, 2.,  2.5};


  FillHist(channel_string+"/llmass_"+channel_string, GetLLMass(LeptonColl), weight_ll, 70, 50., 120);
  FillHist(channel_string+"/NVTX_"+channel_string, nPileUp, weight_ll, 100, 0., 100);
  FillHist(channel_string+"/MET_"+channel_string, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 100, 0., 100);


  if(ElectronColl[0].PassID(ID)) {
    FillHist(channel_string+"/"+ID+"_denom",ElectronColl[1].Pt(), fabs(ElectronColl[1].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins)    ;
    if(ElectronColl[1].PassID(ID))     FillHist(channel_string+"/"+ID+"_num",ElectronColl[1].Pt(), fabs(ElectronColl[1].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins);

  }
  if(ElectronColl[1].PassID(ID)) {
    FillHist(channel_string+"/"+ID+"_denom",ElectronColl[0].Pt(), fabs(ElectronColl[0].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins)    ;
    if(ElectronColl[0].PassID(ID))     FillHist(channel_string+"/"+ID+"_num",ElectronColl[0].Pt(), fabs(ElectronColl[0].Eta()),  weight_ll, nbin_pt, ptbins, nbin_eta , etabins);

  }


  return;

}


HNL_LeptonIDSF::HNL_LeptonIDSF(){


}
 
HNL_LeptonIDSF::~HNL_LeptonIDSF(){

}




