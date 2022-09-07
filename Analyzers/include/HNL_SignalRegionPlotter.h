#ifndef HNL_SignalRegionPlotter_h
#define HNL_SignalRegionPlotter_h

#include "HNL_RegionDefinitions.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"


class HNL_SignalRegionPlotter : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_SignalRegionPlotter();
  ~HNL_SignalRegionPlotter();



  void RunULAnalysis(AnalyzerParameter param);

  ///void RunMuonChannel(std::vector<Lepton *> LepsT,std::vector<Lepton *> LepsV,std::vector<Tau> TauColl,  std::vector<Jet> JetColl, std::vector<Jet> VBFJetColl,  std::vector<FatJet> FatjetColl,  std::vector<Jet> bjets, Event Ev, AnalyzerParameter param,  float _weight)   ;


};



#endif
