#ifndef CalcFakeRate_h
#define CalcFakeRate_h

#include "AnalyzerCore.h"
#include "TriggerWithPtRange.h"

class CalcFakeRate : public AnalyzerCore {

public:

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  void FillFakeRatePlots(TString name, TString frtype, Lepton *lep, bool IsTight, double weight);

  TriggerWithPtRange Muon_TrigWithPtRange, Electron_TrigWithPtRange;

  vector<double> Electron_FakeRatePtBinnings, Muon_FakeRatePtBinnings;

  vector<Electron> AllElectrons;
  vector<Muon>  AllMuons;

  double weight_Prefire;

  bool PassTrigger(Event ev, TString trg);

};

#endif

