#ifndef SkimTree_HNLTriggerSF_h
#define SkimTree_HNLTriggerSF_h

#include "HNL_LeptonCore.h"

class SkimTree_HNLTriggerSF : public HNL_LeptonCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_HNLTriggerSF();
  ~SkimTree_HNLTriggerSF();

  TTree *newtree;

  void WriteHist();


};



#endif
