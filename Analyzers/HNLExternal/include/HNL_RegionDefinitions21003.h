#ifndef HNL_RegionDefinitions21003_h
#define HNL_RegionDefinitions21003_h

#include "TRandom.h"

#include "HNL_LeptonCore.h"

class HNL_RegionDefinitions21003 : public HNL_LeptonCore {

 public:

  HNL_RegionDefinitions21003();
  ~HNL_RegionDefinitions21003();

  // analyis functions to run SR

			
  bool RunSignalRegionWW(HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType q, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto,  std::vector<Tau> Taus, std::vector<Jet> loose_jets, std::vector<Jet> jets,  std::vector<Jet> bjets, Event ev,  Particle METv, AnalyzerParameter param,   float w);


  bool PassVBF(vector<Jet>  jets,std::vector<Lepton *> leps1, double mjj_cut,bool use_leadjet=false);


};



#endif

