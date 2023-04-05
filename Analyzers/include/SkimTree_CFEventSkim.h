#ifndef SkimTree_CFEventSkim_h
#define SkimTree_CFEventSkim_h

#include "AnalyzerCore.h"

class SkimTree_CFEventSkim : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_CFEventSkim();
  ~SkimTree_CFEventSkim();

  TTree *newtree;

  void WriteHist();

};



#endif
