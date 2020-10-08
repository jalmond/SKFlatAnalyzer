#ifndef FakeRateHN_h
#define FakeRateHN_h

#include "TRandom.h"

#include "AnalyzerCore.h"

class FakeRateHN : public AnalyzerCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param, TString El_ID, TString channel);
  void RunE(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w);
  void RunM(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w);
			 
			 
  
  void executeEvent();

  FakeRateHN();
  ~FakeRateHN();

  bool RunSyst;
  bool isEE;
  bool isMM;
  bool isSingleMu;

  bool HEM1516;
  bool BeforeRun319077;
  TRandom3* rand_;


  void FillRegionPlots( TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met,  double w);
				    
				    

  void GetFakeRates(std::vector<Electron> loose_el, std::vector<Electron> tight_el,TString tightlabel,  std::vector<Jet> jets,std::vector<Jet> alljets, TString tag, float isocut,double w);
  void GetFakeRates(std::vector<Muon> loose_el, std::vector<Muon> tight_el,TString tightlabel,  std::vector<Jet> jets,std::vector<Jet> alljets, TString tag, double w, float isocut);
  
  double GetPrescale( std::vector<Electron> electrons, bool passlowest, bool passlow, bool passhigh, bool highest, bool passshighest);
  bool  UseEvent(std::vector<Electron> electrons,  vector<Jet> jets, float pcut,float precale_weight, Particle MET, float w);
  bool  UseEvent(std::vector<Muon> electrons,  vector<Jet> jets, float pcut,float precale_weight, Particle MET, float w);
  float GetPrescale( std::vector<Muon> muons,bool pass3, bool pass2, bool pass1 );


  void MakeFakeRatePlots(TString label, TString eltag,   std::vector<Electron> tightelectrons,std::vector<Electron> electrons,   std::vector <Jet> jets, std::vector<Jet> alljets,  float precale_w, float isocut, float w, Particle MET);
  void MakeFakeRatePlots(TString label, TString eltag,   std::vector<Muon> tightelectrons,std::vector<Muon> electrons,   std::vector <Jet> jets, std::vector<Jet> alljets,  float precale_w,float w,float isocut,  Particle MET);

  void GetFakeRateAndPromptRates(std::vector<Electron> electrons, TString eltag, std::vector<Electron> tightelectrons, float isocut,float w, TString tightid);
  void GetFakeRateAndPromptRates(std::vector<Muon> electrons, TString eltag, std::vector<Muon> tightelectrons,float w, float isocut);


  // Trigger
  vector<TString> MuonTriggers;
  vector<TString> MuonTriggersH;
  vector<TString> ElectronTriggers;
  vector<TString> SingleElectronTriggers;
  vector<TString> EMuTriggers;
  vector<TString> EMuTriggersH;
  vector<TString> Mu8Ele23Triggers;
  vector<TString> Mu23Ele12Triggers;

  // Lepton ID
  vector<TString> MuonVetoIDs;
  vector<TString> MuonLooseIDs;
  vector<TString> MuonTightIDs;
  vector<TString> ElectronVetoIDs;
  vector<TString> ElectronLooseIDs;
  // Fake rate file
  vector<TString> FakeRateIDs;
  vector<TString> ElectronTightIDs;



  // Lepton pT cut
  double MuonPtCut1;
  double MuonPtCut2;
  double ElectronPtCut1;
  double ElectronPtCut2;
  double EMuPtCut1;
  double EMuPtCut2;

  vector<Gen> gens;

  vector<Electron> AllElectrons;
  vector<Muon> AllMuons, AllTunePMuons;
  vector<Jet> AllJets;
  vector<FatJet> AllFatJets;

  double weight_Prefire, weight_Prefire_Up, weight_Prefire_Down;

  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;

  //==== DYPtReweight
  bool ApplyDYPtReweight;
  double ZPtReweight, ZPtReweight_Up, ZPtReweight_Down;

  //==== My tool
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR=0.4);
  bool LeptonPassID(Lepton &lepton, TString ID);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon *>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron *>& electrons, double TightIso=-999, bool UseMini=false);

  bool PassHNID(TString TightID, Electron el);
  double GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets);
  void FilAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w);
  void FilAllMuonPlots(TString label , TString cut,  std::vector<Muon> mus, float w);
  double MT(Particle a, Particle b);
  void FillRegionPlots(int channel, bool plotCR,TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w);
  void FillSigRegionPlots1(int channel,int sysdir, TString sys, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11);
  void FillSigRegionPlots2(int channel,int sysdir, TString sys, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7);


  int GetNBJets();

  void FillZZCRPlots(int i_channel, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);
  void FillWZCRPlots(int i_channel, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);
				 


  void FillAllSignalRegion1(int i_channel, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,  Particle _met,int _npv , double w );
					
  void FillAllSignalRegion2(int i_channel, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,  Particle _met,int _npv , double w );

			    

  vector<pair<int, pair<TString, double> > >  GetSyst(int i_channel, bool isdata);
  double GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev);
  double GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle met);


  void MakeSignalPlots();

  int SignalLeptonChannel;

  //==== pileup
  int N_VTX;
  TH1D *hist_PUReweight;
  TH1D *hist_PUReweight_Up;
  TH1D *hist_PUReweight_Down;
  double weight_PU, weight_PU_Up, weight_PU_Down;

  void FillWeightHist(TString label, double _weight);
  void FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight);
  void FillEventCutflow(int charge_s,int version,float wt,TString cut,    TString label);

  //==== LSF SF
  double LSFSF(int lepflav, int dir);

};



#endif

