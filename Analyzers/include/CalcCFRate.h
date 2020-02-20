#ifndef CalcCFRate_h
#define CalcCFRate_h

#include "AnalyzerCore.h"

class CalcCFRate : public AnalyzerCore {

public:

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  void FillCFRatePlots(TString name, Lepton *lep, bool IsCF, double weight);

  vector<double> Electron_FakeRatePtBinnings, Muon_FakeRatePtBinnings;

  vector<Electron> AllElectrons;
  vector<Muon>  AllMuons;

};



#endif

