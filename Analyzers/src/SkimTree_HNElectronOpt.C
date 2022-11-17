#include "SkimTree_HNElectronOpt.h"

void SkimTree_HNElectronOpt::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNElectronOpt::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  triggers_dimu.clear();

  triggers.clear();

  if(DataYear==2016){

    triggers_di_el = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
    };
  }
  else if(DataYear==2017){

    triggers_di_el = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"
    };

  }
  else if(DataYear==2018){
    
    triggers_di_el = {
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"
    };
  }
  else{
    cout << "[SkimTree_HNElectronOpt::initializeAnalyzer] DataYear is wrong : " << DataYear << endl;
  }

  cout << "[SkimTree_HNElectronOpt::initializeAnalyzer] triggers to skim = " << endl;
  for(unsigned int i=0; i<triggers.size(); i++){
    cout << "[SkimTree_HNElectronOpt::initializeAnalyzer]   " << triggers.at(i) << endl;
  }

}

void SkimTree_HNElectronOpt::executeEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  //==== Skim 1 ) trigger
  if(! (ev.PassTrigger(triggers_di_el)) ) return;

  std::vector<Muon>     MuonColl     = GetMuons("HNLoosest", 5., 2.4);
  std::vector<Electron> ElectronColl = GetElectrons("HNLoosest", 10., 2.5);
 
  std::sort(MuonColl.begin(), MuonColl.end(), PtComparing);
  std::sort(ElectronColl.begin(), ElectronColl.end(), PtComparing);
  
  int NEl  = ElectronColl.size();
  int NMu  = MuonColl.size();

  if(NMu > 0)  return;
  if(NEl !=2)  return;

  AnalyzerParameter param = HNL_LeptonCore::InitialiseHNLParameter("HNLOpt","_UL");  
  
  std::vector<FatJet> fatjets_tmp                 = GetFatJets(param, param.FatJet_ID, 200., 2.7);

  std::vector<FatJet> AK8_JetColl                  = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1., false, -999, true, 0., 20000., ElectronColl, MuonColl);


  if(AK8_JetColl.size() > 0 && ElectronColl[0].Pt() < 100) return;
  

  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);

  std::vector<Jet> JetColl                    = GetAK4Jets(jets_tmp,     20., 4.7, true,  0.4,0.8, "",   ElectronColl,MuonColl, AK8_JetColl);
  std::vector<Jet> BJetColltmp                = GetAK4Jets(jets_tmp,     20., 2.4, true,  0.4,0.8, "",   ElectronColl,MuonColl, AK8_JetColl);
  
  if(JetColl.size() < 2  && ElectronColl[0].Pt() < 80) return;

  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);

  std::vector<Jet> BJetColl    = GetBJets(param, BJetColltmp, param_jets);

  if(BJetColl.size() > 0) return;
  
  std::vector<Lepton *> leps       = MakeLeptonPointerVector(MuonColl,ElectronColl,param);

  Particle ll =  (*leps[0]) + (*leps[1]);
  if(! SameCharge(leps)) return;
  if ((fabs(ll.M()-90.) < 15)) return;
  
  if (ll.M() < 10) return ;

  //=============================
  //==== If survived, fill tree
  //=============================

  newtree->Fill();

}

void SkimTree_HNElectronOpt::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNElectronOpt::SkimTree_HNElectronOpt(){

  newtree = NULL;

}

SkimTree_HNElectronOpt::~SkimTree_HNElectronOpt(){

}

void SkimTree_HNElectronOpt::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
