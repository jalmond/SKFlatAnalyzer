#ifndef HNL_SignalLeptonOpt_h
#define HNL_SignalLeptonOpt_h

#include "HNL_RegionDefinitionsOpt.h"

class HNL_SignalLeptonOpt : public HNL_RegionDefinitionsOpt {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalLeptonOpt();
  ~HNL_SignalLeptonOpt();



  void RunULAnalysis(AnalyzerParameter param);


};



#endif
