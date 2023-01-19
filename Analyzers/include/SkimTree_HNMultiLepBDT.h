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


  float SKWeight;
  vector<float>* electron_ptratio;
  vector<float>* electron_ptrel;
  vector<float>* electron_cj_bjetdisc;
  vector<float>* electron_mva_cf;
  vector<float>* electron_mva_conv;
  vector<float>* electron_mva_fake;

  vector<float>* muon_mva_conv;
  vector<float>* muon_ptrel;
  vector<float>* muon_ptratio;
  vector<float>* muon_cj_bjetdisc;


  bool isSingleMu;

  vector<TString> triggers;
  vector<TString> triggers_dimu;
  vector<TString> triggers_di_el;
  vector<TString> triggers_emu;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;

};



#endif
