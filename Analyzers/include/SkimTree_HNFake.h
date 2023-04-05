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



  vector<TString> triggers;
  void WriteHist();

};



#endif
