#ifndef HNL_ControlRegion_h
#define HNL_ControlRegion_h

#include "HNL_RegionDefinitions.h"

class HNL_ControlRegion : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_ControlRegion();
  ~HNL_ControlRegion();

  void RunControlRegions(AnalyzerParameter param, vector<TString> CRs);

};



#endif
