#ifndef SkimTree_FakeEventSkimBDT_h
#define SkimTree_FakeEventSkimBDT_h

#include "HNL_LeptonCore.h"

class SkimTree_FakeEventSkimBDT : public HNL_LeptonCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_FakeEventSkimBDT();
  ~SkimTree_FakeEventSkimBDT();

  TTree *newtree;

  void WriteHist();

};



#endif
