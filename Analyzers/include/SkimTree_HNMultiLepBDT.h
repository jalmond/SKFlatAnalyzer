#ifndef SkimTree_HNMultiLepBDT_h
#define SkimTree_HNMultiLepBDT_h

#include "AnalyzerCore.h"

class SkimTree_HNMultiLepBDT : public AnalyzerCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_HNMultiLepBDT();
  ~SkimTree_HNMultiLepBDT();

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
