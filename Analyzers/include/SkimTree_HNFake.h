#ifndef SkimTree_HNFake_h
#define SkimTree_HNFake_h

#include "HNL_LeptonCore.h"


class SkimTree_HNFake : public HNL_LeptonCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_HNFake();
  ~SkimTree_HNFake();

  TTree *newtree;

  float vSKWeight;
  vector<float>* velectron_ptratio;
  vector<float>* velectron_ptrel;
  vector<float>* velectron_cj_bjetdisc;
  vector<float>* velectron_cj_flavour;

  vector<float>* velectron_mva_cf;
  vector<float>* velectron_mva_conv;
  vector<float>* velectron_mva_fake;

  vector<float>* vmuon_mva_fake;
  vector<float>* vmuon_ptrel;
  vector<float>* vmuon_ptratio;
  vector<float>* vmuon_cj_bjetdisc;
  vector<float>* vmuon_cj_flavour;



  vector<TString> triggers;
  void WriteHist();

};



#endif
