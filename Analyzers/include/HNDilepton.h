#ifndef HNDilepton_h
#define HNDilepton_h

#include "TRandom.h"

#include "AnalyzerCore.h"

class HNDilepton : public AnalyzerCore {

 public:

  enum ChargeType
  {
    AllCharge=0,
    SS=1,
    OS=-1
  };
  enum Flavour
  {
    EE=0,
    MuMu=1
  };
  enum Region
  {
    MuMu_SR1=3,
    EE_SR1=4,
    MuMu_SR2=5,
    EE_SR2=6
  };


  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param, TString El_ID, Flavour channel);

  void executeEvent();

  HNDilepton();
  ~HNDilepton();

  // analyis functions to run SR
  void RunElectron( TString tag, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets,std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float w);
  void RunElectron( double iid, int nvec,TString id, vector<TString> idnames, TString tag, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets,std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float w);
   int PassEE(int ptbin, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets,std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float w);
  bool PassMM(int ptbin, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets,std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float w);
	
  
  // analysis function to run CR
 // function to get MC SF weights
 double GetElectronMCWeight(std::vector<Electron> electrons, TString Tight_ID,AnalyzerParameter param );
 double GetMuonMCWeight    (std::vector<Muon> muons, TString Tight_ID,AnalyzerParameter param );

 /// bkg functions 
 // -- Charge flip
 double GetCFWeightElectron(std::vector<Electron> electrons , TString tight_id);
 // -- Fake
 double CalculateDiLepWeight( double fr1, double fr2, bool mu1Tight, bool mu2Tight);
 double GetFakeRateEl(double eta, double pt, TString id);
 double GetFakeRateM(double eta, double pt, TString id);
 double  GetFakeWeightElectron(std::vector<Electron> electrons , TString tight_id);
 double  GetFakeWeightMuon(std::vector<Muon> muons , TString tight_id);	       
 

  // global var for user flags
  bool RunFake, RunCF;
  bool RunSyst;
  bool isEE;
  bool isMM;
  bool Signal;
  bool SignalOS;
  bool IncludeFakeLepton;
  bool isSingleMu;
  bool HEM1516;
  bool BeforeRun319077;

  TRandom3* rand_;


  // Trigger vectors used to store year dependant trigger lists
  vector<TString> HighPtMuonTriggers;
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

  // obj vectors
  vector<Gen> gens;
  vector<Electron> AllElectrons;
  vector<Muon> AllMuons, AllTunePMuons;
  vector<Jet> AllJets;
  vector<FatJet> AllFatJets;

  double weight_Prefire, weight_Prefire_Up, weight_Prefire_Down;

  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;


  //==== My tool 

  //// ===============================  JETS =============================== ////
  
  /// Function to get Jet colection 
  std::vector<FatJet>   GetAK8Jets(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool applytau21, double tau21_cut , bool apply_mass, double sdmass_lower_cut,  double sdmass_upper_cut, vector<Electron>  veto_electrons, vector<Muon>  veto_muons);
  std::vector<Jet>   GetAK4Jets(std::vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , double dr_ak8_clean , TString pustring,  std::vector<Electron>  veto_electrons, std::vector<Muon>  veto_muons, std::vector<FatJet> fatjets);
  
  // Funtion to get SF for jet ID                                                                                                                                                                                                                                                                                                                                    
  double GetEventFatJetSF(std::vector<FatJet> fatjets, TString label, int dir);
  double GetFatJetSF(FatJet fatjet, TString tag,  int dir);

  /// function to make plots for jet variables
  void MakeAK8JetPlots(Flavour flavour, AnalyzerParameter param,map<TString, std::vector<FatJet> > fatjet_map, std::vector<Electron>  veto_electrons, std::vector<Muon>  veto_muons, double w);
  void MakeAK4JetPlots(Flavour flavour,AnalyzerParameter param,map<TString, std::vector<Jet> > jet_map, std::vector<Electron>  veto_electrons, std::vector<Muon>  veto_muons, double w);
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR=0.4);


  //// ===============================  B-JETS =============================== ////                                                                                                                                                                                                                                                                                    

  int GetNBJets(TString ID, TString WP);

  
  //// ===============================  HISTS =============================== ////                                                                                                                                                                                                                                                                                    

  // Function to fill hist of object sizes
  void FillElectronEfficiency(TString tag, std::vector<Electron> electrons, double w);

  void  FillObjectSizes(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ll, TString  channel_s);
  
  // Function to fill any plot not used in analysis
  void MakeGeneralPlots(std::vector<Muon> muons,std::vector<Electron> myelectrons, std::map<TString, std::vector<FatJet> > fatjet_map,AnalyzerParameter param ,double weights);
  
  // Plotter functions                                                                                                                                                                                                                                                                                                                                               
  void FilAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w);
  void FilAllMuonPlots(TString label , TString cut,  std::vector<Muon> mus, float w);


  void FillWeightHist(TString label, double _weight);
  void FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight);
  void FillEventCutflow(HNDilepton::ChargeType charge_s, HNDilepton::Flavour version,float wt,TString cut,    TString label);
  void FillEventCutflow(HNDilepton::ChargeType charge_s, HNDilepton::Region version,float wt,TString cut,    TString label);





  //// ===============================  LEPTONS =============================== ////                                                                                                                                                                                                                                                                                    


  void FillElectronPresel(TString tag, double w, int bins);
  void FillElectronPresel(double iid,int nvec, TString id,vector<TString> idnames,TString tag, double w, int bins);

  bool LeptonPassID(Lepton &lepton, TString ID);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon *>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron *>& electrons, double TightIso=-999, bool UseMini=false);


  //// ===============================  TOOLS =============================== ////                                                                                                                                                                                                                                                                                    

  /// Helpful tools
  double GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets);
  double MT(Particle a, Particle b);
  


  vector<pair<int, pair<TString, double> > >  GetSyst(HNDilepton::Flavour flavour, bool isdata);
  double GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev);
  double GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle met);


		   

  //// ===============================  CR PLOTS =============================== ////

  void FillZZCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);

  void FillWZCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);

  void FillZGCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);
  void FillWGCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);				 


  //// ===============================  SR PLOTS =============================== ////

  void FillRegionPlots(HNDilepton::Flavour channel, bool plotCR,TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w);

  void FillSigRegionPlots1(HNDilepton::Flavour channel,int sysdir, TString sys, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11);


  void FillSigRegionPlots2(HNDilepton::Flavour channel,int sysdir, TString sys, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7);



  void RunHighMassSR(ChargeType charge_i, TString channel_s,HNDilepton::Flavour flavour, TString charge_s,Particle llCand, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> , std::vector<Muon> muons, Particle METv, int mPV, double w, Event ev, AnalyzerParameter param, bool FullAnalysis);



  void FillAllSignalRegion1(HNDilepton::Flavour flavour, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,  Particle _met,int _npv , double w , bool full);
					
  void FillAllSignalRegion2(HNDilepton::Flavour flavour, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,  Particle _met,int _npv , double w , bool full);

  void MakeSignalPlots();
  

  //// ===============================  GLOBAL VAR =============================== ////


  int SignalLeptonChannel;

  //==== pileup
  int N_VTX;
  TH1D *hist_PUReweight;
  TH1D *hist_PUReweight_Up;
  TH1D *hist_PUReweight_Down;
  double weight_PU, weight_PU_Up, weight_PU_Down;



};



#endif

