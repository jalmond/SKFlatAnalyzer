#ifndef HNL_LeptonConversionStudy_h
#define HNL_LeptonConversionStudy_h

#include "HNL_RegionDefinitions.h"

class HNL_LeptonConversionStudy : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_LeptonConversionStudy();
  ~HNL_LeptonConversionStudy();




};



#endif
