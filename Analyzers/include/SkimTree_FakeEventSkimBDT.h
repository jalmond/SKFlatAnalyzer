#ifndef SkimTree_FakeEventSkimBDT_h
#define SkimTree_FakeEventSkimBDT_h

#include "AnalyzerCore.h"

class SkimTree_FakeEventSkimBDT : public AnalyzerCore {

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
