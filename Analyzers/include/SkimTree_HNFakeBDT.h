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



  vector<TString> triggers;
  void WriteHist();

};



#endif
