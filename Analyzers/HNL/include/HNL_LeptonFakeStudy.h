#ifndef HNL_LeptonFakeStudy_h
#define HNL_LeptonFakeStudy_h

#include "HNL_LeptonCore.h"


class HNL_LeptonFakeStudy  : public HNL_LeptonCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param, TString El_ID, HNL_LeptonCore::Channel channel);

  
  void executeEvent();

  HNL_LeptonFakeStudy();
  ~HNL_LeptonFakeStudy();

  
  bool UseEvent(std::vector<Lepton *> leps,   std::vector< Jet> jets, float awayjetcut, Particle MET, float wt);




};



#endif

