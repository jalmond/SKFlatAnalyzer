#ifndef HNWRAnalyzer_h
#define HNtypeI_JA_h

#include "AnalyzerCore.h"

class HNtypeI_JA : public AnalyzerCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param, TString El_ID, TString tightID, TString cc, double dx_b, double dx_e, double dz_b, double dz_e,  double iso_b, double iso_e);
				 
  
  void executeEvent();

  HNtypeI_JA();
  ~HNtypeI_JA();

  bool RunFake, RunCF;
  bool RunSyst;
  bool Signal;
  bool PromptLeptonOnly;

  bool HEM1516;
  bool BeforeRun319077;


  // Trigger
  vector<TString> MuonTriggers;
  vector<TString> MuonTriggersH;
  vector<TString> ElectronTriggers;
  vector<TString> SingleElectronTriggers;
  vector<TString> EMuTriggers;
  vector<TString> EMuTriggersH;
  vector<TString> Mu8Ele23Triggers;
  vector<TString> Mu23Ele12Triggers;

  // Lepton ID
  vector<TString> MuonVetoIDs;
  vector<TString> MuonLooseIDs;
  vector<TString> MuonTightIDs;
  vector<TString> ElectronVetoIDs;
  vector<TString> ElectronLooseIDs;
  // Fake rate file
  vector<TString> FakeRateIDs;
  vector<TString> ElectronTightIDs;



  // Lepton pT cut
  double MuonPtCut1;
  double MuonPtCut2;
  double ElectronPtCut1;
  double ElectronPtCut2;
  double EMuPtCut1;
  double EMuPtCut2;

  vector<Gen> gens;

  vector<Electron> AllElectrons;
  vector<Muon> AllMuons, AllTunePMuons;
  vector<Jet> AllJets;
  vector<FatJet> AllFatJets;

  double weight_Prefire, weight_Prefire_Up, weight_Prefire_Down;

  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;

  //==== DYPtReweight
  bool ApplyDYPtReweight;
  double ZPtReweight, ZPtReweight_Up, ZPtReweight_Down;

  //==== My tool
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR=0.4);
  bool LeptonPassID(Lepton &lepton, TString ID);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon *>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron *>& electrons, double TightIso=-999, bool UseMini=false);

  double MT(Particle a, Particle b);
  void FillRegionPlots(bool plotCR,TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w);
  void FillSigRegionPlots1(TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11);
  void FillSigRegionPlots2(TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7);


  void MakeSignalPlots();

  int SignalLeptonChannel;

  //==== pileup
  int N_VTX;
  TH1D *hist_PUReweight;
  TH1D *hist_PUReweight_Up;
  TH1D *hist_PUReweight_Down;
  double weight_PU, weight_PU_Up, weight_PU_Down;

  void FillWeightHist(TString label, double _weight);
  void FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight);

  //==== LSF SF
  double LSFSF(int lepflav, int dir);

};



#endif

