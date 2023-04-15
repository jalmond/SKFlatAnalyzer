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



  void RunULAnalysis(AnalyzerParameter param, vector<Electron> el, vector<Muon> mu, std::vector<Tau> taus, std::vector<Jet> AllJetColl ,  std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1,Event ev);


};



#endif
