#ifndef ExampleRun_h
#define ExampleRun_h

#include "HNL_LeptonCore.h"

class ExampleRun : public HNL_LeptonCore {

public:

  void initializeAnalyzer();

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool RunSyst;
  bool RunNewPDF;
  bool RunXSecSyst;

  TString IsoMuTriggerName;
  double TriggerSafePtCut;

  vector<TString> MuonIDs, MuonIDSFKeys;
  vector<Muon> AllMuons;
  vector<Jet> AllJets;

  double weight_Prefire;

  ExampleRun();
  ~ExampleRun();

};



#endif

