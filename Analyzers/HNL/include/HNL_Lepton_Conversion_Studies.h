#ifndef HNL_Lepton_Conversion_Studies_h
#define HNL_Lepton_Conversion_Studies_h

#include "HNL_RegionDefinitions.h"

class HNL_Lepton_Conversion_Studies : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_Lepton_Conversion_Studies();
  ~HNL_Lepton_Conversion_Studies();




};



#endif
