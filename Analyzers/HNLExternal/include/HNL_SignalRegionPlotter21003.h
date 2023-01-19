#ifndef HNL_SignalRegionPlotter21003_h
#define HNL_SignalRegionPlotter21003_h

#include "HNL_RegionDefinitions21003.h"

class HNL_SignalRegionPlotter21003 : public HNL_RegionDefinitions21003 {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalRegionPlotter21003();
  ~HNL_SignalRegionPlotter21003();



  void RunULAnalysis(AnalyzerParameter param);


};



#endif
