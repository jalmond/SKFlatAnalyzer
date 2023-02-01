#ifndef HNL_LeptonIDBDTStudies_h
#define HNL_LeptonIDBDTStudies_h

#include "HNL_RegionDefinitions.h"

class HNL_LeptonIDBDTStudies : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_LeptonIDBDTStudies();
  ~HNL_LeptonIDBDTStudies();

  void MakeBDTPlots(AnalyzerParameter param,HNL_LeptonCore::Channel dilep_channel, vector<Electron> ElectronColl, TString ID, double weight_ll);
};



#endif
