#ifndef HNL_SignalRegionOpt_h
#define HNL_SignalRegionOpt_h

#include "HNL_RegionDefinitionsOpt.h"

class HNL_SignalRegionOpt : public HNL_RegionDefinitionsOpt {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalRegionOpt();
  ~HNL_SignalRegionOpt();



  void RunULAnalysis(AnalyzerParameter param);


};



#endif
