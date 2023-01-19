#ifndef HNL_SignalLeptonOpt_h
#define HNL_SignalLeptonOpt_h

#include "HNL_RegionDefinitionsOpt.h"

class HNL_SignalLeptonOpt : public HNL_RegionDefinitionsOpt {

 public:

  bool RunHighPt,RunEE;

  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalLeptonOpt();
  ~HNL_SignalLeptonOpt();



  void RunULAnalysis(AnalyzerParameter param, vector<Electron> el, vector<Muon> mu);


};



#endif
