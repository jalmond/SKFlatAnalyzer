#ifndef SkimTree_ConvEventSkim_h
#define SkimTree_ConvEventSkim_h

#include "AnalyzerCore.h"

class SkimTree_ConvEventSkim : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_ConvEventSkim();
  ~SkimTree_ConvEventSkim();

  TTree *newtree;

  void WriteHist();

};



#endif
