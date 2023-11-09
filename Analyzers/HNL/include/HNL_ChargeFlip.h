#ifndef HNL_ChargeFlip_h
#define HNL_ChargeFlip_h

#include "HNL_RegionDefinitions.h"

class HNL_ChargeFlip : public HNL_RegionDefinitions {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  HNL_ChargeFlip();
  ~HNL_ChargeFlip();

};



#endif
