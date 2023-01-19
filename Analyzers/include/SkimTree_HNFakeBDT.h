#ifndef SkimTree_HNFakeBDT_h
#define SkimTree_HNFakeBDT_h

#include "HNL_LeptonCore.h"


class SkimTree_HNFakeBDT : public HNL_LeptonCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_HNFakeBDT();
  ~SkimTree_HNFakeBDT();

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

  vector<TString> triggers;
  void WriteHist();

};



#endif
