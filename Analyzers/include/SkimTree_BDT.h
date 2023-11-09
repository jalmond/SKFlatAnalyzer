#ifndef SkimTree_BDT_h
#define SkimTree_BDT_h

#include "HNL_LeptonCore.h"

class SkimTree_BDT : public HNL_LeptonCore {

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
