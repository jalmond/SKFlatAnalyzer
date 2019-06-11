#ifndef HNWRAnalyzer_h
#define HNWRAnalyzer_h

#include "AnalyzerCore.h"
#include "GenFinderForHNWRSignal.h"
#include "GenFinderForDY.h"

class HNWRAnalyzer : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  HNWRAnalyzer();
  ~HNWRAnalyzer();

  bool RunFake, RunCF;
  bool RunSyst;
  bool PromptLeptonOnly;
  bool NoTrigger;

  vector<Gen> gens;

  vector<Electron> AllElectrons;
  vector<Muon> AllMuons, AllTunePMuons;
  vector<Jet> AllJets;
  vector<FatJet> AllFatJets;

  double weight_Prefire;

  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;

  //==== DYPtReweight
  bool ApplyDYPtReweight;
  TH2D *hist_DYPtReweight_2D;
  TH1D *hist_DYPtReweight_Electron_Resolved;
  TH1D *hist_DYPtReweight_Electron_Boosted;
  TH1D *hist_DYPtReweight_Muon_Resolved;
  TH1D *hist_DYPtReweight_Muon_Boosted;

  double GetDYPtReweight(double zpt, int flav, int region);
  double GetDYPtReweight(double zmass, double zpt);

  //==== My tool
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR=0.4);
  bool LeptonPassID(Lepton &lepton, TString ID);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon *>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron *>& electrons, double TightIso=-999, bool UseMini=false);

  GenFinderForHNWRSignal *genFinderSig;
  GenFinderForDY *genFinderDY;

  //==== pileup
  int N_VTX;
  TH1D *hist_PUReweight;
  double weight_PU;

};



#endif

