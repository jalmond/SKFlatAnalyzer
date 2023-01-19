#ifndef SkimTree_HNMultiLep_h
#define SkimTree_HNMultiLep_h

#include "AnalyzerCore.h"

class SkimTree_HNMultiLep : public AnalyzerCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_HNMultiLep();
  ~SkimTree_HNMultiLep();

  TTree *newtree;

  float vSKWeight;
  vector<float>* velectron_ptratio;
  vector<float>* velectron_ptrel;
  vector<float>* velectron_cj_bjetdisc;
  vector<float>* velectron_mva_cf;
  vector<float>* velectron_mva_conv;
  vector<float>* velectron_mva_fake;

  vector<float>* vmuon_mva_conv;
  vector<float>* vmuon_ptrel;
  vector<float>* vmuon_ptratio;
  vector<float>* vmuon_cj_bjetdisc;

  bool isSingleMu;

  vector<TString> triggers;
  vector<TString> triggers_dimu;
  vector<TString> triggers_di_el;
  vector<TString> triggers_emu;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;

};



#endif
