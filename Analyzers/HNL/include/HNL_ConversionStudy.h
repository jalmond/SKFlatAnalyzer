#ifndef HNL_ConversionStudy_h
#define HNL_ConversionStudy_h

#include "HNL_RegionDefinitions.h"

class HNL_ConversionStudy : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_ConversionStudy();
  ~HNL_ConversionStudy();




};



#endif
