#ifndef HNL_LeptonIDSF_h
#define HNL_LeptonIDSF_h

#include "HNL_RegionDefinitions.h"

class HNL_LeptonIDSF : public HNL_RegionDefinitions {

 public:


  void initializeAnalyzer();
  void executeEvent();

  HNL_LeptonIDSF();
  ~HNL_LeptonIDSF();

  void MeasureElectronIDSF(AnalyzerParameter param);
  void MeasureMuonIDSF(AnalyzerParameter param);
  void MeasureIDSF(HNL_LeptonCore::Channel dilep_channel, vector<Muon > LeptonColl, TString ID, double w);
  void MeasureIDSF(HNL_LeptonCore::Channel dilep_channel, vector<Electron > LeptonColl, TString ID, double w);
  void FilllHistBins(Lepton lep, bool passID,  TString Channel_string,TString _ID, double lep_weight);

};



#endif
