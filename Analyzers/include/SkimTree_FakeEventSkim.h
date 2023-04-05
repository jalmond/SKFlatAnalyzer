#ifndef SkimTree_FakeEventSkim_h
#define SkimTree_FakeEventSkim_h

#include "AnalyzerCore.h"

class SkimTree_FakeEventSkim : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_FakeEventSkim();
  ~SkimTree_FakeEventSkim();

  TTree *newtree;

  void WriteHist();

};



#endif
