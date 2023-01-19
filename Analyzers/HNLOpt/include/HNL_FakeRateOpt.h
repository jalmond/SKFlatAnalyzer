#ifndef HNL_FakeRateOpt_h
#define HNL_FakeRateOpt_h

#include "HNL_LeptonCore.h"


class HNL_FakeRateOpt  : public HNL_LeptonCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param, TString El_ID, HNL_LeptonCore::Channel channel);
  void RunE(std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets,AnalyzerParameter param,  float w);
  void RunM(std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, AnalyzerParameter param,  float w);

  
  void executeEvent();

  HNL_FakeRateOpt();
  ~HNL_FakeRateOpt();


  void FillRegionPlots( TString plot_dir, TString region,    std::vector<Jet> jets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met,  double w);

  double ApplyNvtxReweight(int Nvtx, TString Key);
  void MakeNVertexDistPrescaledTrig(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev,std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight);				  
				    
  void GetFakeRates(std::vector<Lepton *> leps,std::vector<bool> blepsT,  AnalyzerParameter param,TString tightlabel,  std::vector<Jet> jets,  TString tag,float event_weight, float isocut);
					   
  void GetFakeRateAndPromptRates(AnalyzerParameter param, std::vector<Lepton *> leps,std::vector<bool> blepsT, std::vector<Jet>    jetCollTight, TString label, float event_weight, float isocut);

  
  void MakeDiLepPlots(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight);
				    
    
  float GetPrescale( std::vector<Lepton *> leps);

  void MakeFakeRatePlots(TString label, TString mutag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT, std::vector<Jet> jets,  float event_weight, float isocut, Particle MET);
			 
  void MakePromptRatePlots(TString label, TString mutag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> lepsT, std::vector<Jet> jets,float event_weight, float isocut, Particle MET);
			   
  
  bool UseEvent(std::vector<Lepton *> leps,   std::vector< Jet> jets, float awayjetcut, Particle MET, float wt);


  //==== pileup

  void FillWeightHist(TString label, double _weight);
  void FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight);
  void FillEventCutflow(int charge_s,int version,float wt,TString cut,    TString label);

  TFile* NvtxSFFile;
  map< TString, TH1D* > maphist_NvtxSF;



};



#endif

