#ifndef HNL_RegionDefinitions17028_h
#define HNL_RegionDefinitions17028_h

#include "TRandom.h"

#include "HNL_LeptonCore.h"

class HNL_RegionDefinitions17028 : public HNL_LeptonCore {

 public:

  HNL_RegionDefinitions17028();
  ~HNL_RegionDefinitions17028();

  // analyis functions to run SR
  
			
  void   RunEXO17028SignalRegions(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet>  fatjets, std::vector<Jet> bjets, Event ev,  Particle METv, AnalyzerParameter param,  float w);


  /// PRESEL 
  bool  PassPreselection(HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType q, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto,  std::vector<Jet> jets, std::vector<FatJet>  fatjets, std::vector<Jet> bjets, Event ev, Particle METv, AnalyzerParameter param, TString channel_string ,  float w);


  //// SIGNAL REGION DEFINITIONS

  bool  RunSignalRegionAK8(HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType q, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto, std::vector<Jet> jets, std::vector<FatJet>  fatjets, std::vector<Jet> bjets, Event ev,  Particle METv, AnalyzerParameter param, TString channel_string ,  float w);
  bool  RunSignalRegionAK4(HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType q, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto, std::vector<Jet> jets, std::vector<FatJet>  fatjets, std::vector<Jet> bjets, Event ev,  Particle METv, AnalyzerParameter param, TString channel_string ,  float w);

  
  void Fill_All_SignalRegion1(HNL_LeptonCore::Channel channel, TString signal_region3, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps,   Particle _MET,int _npv  , double w  );

  void Fill_All_SignalRegion2(HNL_LeptonCore::Channel channel, TString signal_region1, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps, Particle _MET,int _npv  , double w);

  void Fill_SigRegionPlots2(HNL_LeptonCore::Channel channel,TString label_sr, TString label_mass, TString label_anid,  std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7);

  void   Fill_SigRegionPlots1(HNL_LeptonCore::Channel channel, TString label_sr, TString label_mass, TString label_anid,  std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps, Particle  met , double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11);




};



#endif

