#ifndef HNL_SignalMuonOpt_h
#define HNL_SignalMuonOpt_h

#include "HNL_RegionForLeptonOpt.h"

class HNL_SignalMuonOpt : public HNL_RegionForLeptonOpt {

 public:

  bool RunHighPt,RunEE;

  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalMuonOpt();
  ~HNL_SignalMuonOpt();



  void RunULAnalysis(AnalyzerParameter param, vector<Electron> el, vector<Muon> mu, std::vector<Tau> taus, std::vector<Jet> AllJetColl ,  std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1,Event ev);


};



#endif
