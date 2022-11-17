#ifndef SkimTree_HNElectronOpt_h
#define SkimTree_HNElectronOpt_h

#include "HNL_LeptonCore.h"

class SkimTree_HNElectronOpt : public HNL_LeptonCore {


 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_HNElectronOpt();
  ~SkimTree_HNElectronOpt();

  TTree *newtree;

  bool isSingleMu;

  vector<TString> triggers;
  vector<TString> triggers_dimu;
  vector<TString> triggers_di_el;
  vector<TString> triggers_emu;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;

};



#endif
