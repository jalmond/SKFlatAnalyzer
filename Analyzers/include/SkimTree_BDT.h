#ifndef SkimTree_BDT_h
#define SkimTree_BDT_h

#include "AnalyzerCore.h"

class SkimTree_BDT : public AnalyzerCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_BDT();
  ~SkimTree_BDT();

  TTree *newtree;

  void WriteHist();


};



#endif
