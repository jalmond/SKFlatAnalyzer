#ifndef HNL_SignalRegionPlotter17028_h
#define HNL_SignalRegionPlotter17028_h

#include "HNL_RegionDefinitions17028.h"

class HNL_SignalRegionPlotter17028 : public HNL_RegionDefinitions17028 {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalRegionPlotter17028();
  ~HNL_SignalRegionPlotter17028();



  void RunULAnalysis(AnalyzerParameter param);
  void RunEXO17028Analysis(AnalyzerParameter param);

  ///void RunMuonChannel(std::vector<Lepton *> LepsT,std::vector<Lepton *> LepsV,std::vector<Tau> TauColl,  std::vector<Jet> JetColl, std::vector<Jet> VBFJetColl,  std::vector<FatJet> FatjetColl,  std::vector<Jet> bjets, Event Ev, AnalyzerParameter param,  float _weight)   ;


};



#endif
