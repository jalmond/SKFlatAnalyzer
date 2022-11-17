#ifndef HNL_SignalEfficiency_h
#define HNL_SignalEfficiency_h

#include "HNL_RegionDefinitions.h"

class HNL_SignalEfficiency : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalEfficiency();
  ~HNL_SignalEfficiency();



};



#endif
