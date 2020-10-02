#ifndef HNtypeI_Dilepton_h
#define HNtypeI_Dilepton_h

#include "TRandom.h"

#include "AnalyzerCore.h"

class HNtypeI_Dilepton : public AnalyzerCore {

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


  // analyis functions 
  void RunEE(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets,std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float w);
  void RunMM(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets,std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float w);
			 

  // bkg functions 
  double GetCFWeightElectron(std::vector<Electron> electrons , TString tight_id);
  double CalculateDiLepWeight( double fr1, double fr2, bool mu1Tight, bool mu2Tight);
  double GetFakeRateEl(double eta, double pt, TString id);
  double GetFakeRateM(double eta, double pt, TString id);
  double  GetFakeWeightElectron(std::vector<Electron> electrons , TString tight_id);
  double  GetFakeWeightMuon(std::vector<Muon> muons , TString tight_id);	       
  
  void executeEvent();

  HNtypeI_Dilepton();
  ~HNtypeI_Dilepton();


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


  // Trigger vectors
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
  std::vector<FatJet>   GetAK8Jets(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool applytau21, double tau21_cut , bool apply_mass, double sdmass_lower_cut,  double sdmass_upper_cut, vector<Electron>  veto_electrons, vector<Muon>  veto_muons);

  void  FillObjectSizes(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ll, TString  channel_s);
  
  void MakeGeneralPlots(std::vector<Muon> muons,std::vector<Electron> myelectrons, std::map<TString, std::vector<FatJet> > fatjet_map,AnalyzerParameter param ,double weights);

  double GetEventFatJetSF(std::vector<FatJet> fatjets, TString label, int dir);					  
  double GetFatJetSF(FatJet fatjet, TString tag,  int dir);

  std::vector<Jet>   GetAK4Jets(std::vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , double dr_ak8_clean , TString pustring,  std::vector<Electron>  veto_electrons, std::vector<Muon>  veto_muons, std::vector<FatJet> fatjets);


  void MakeAK8JetPlots(Flavour flavour, AnalyzerParameter param,map<TString, std::vector<FatJet> > fatjet_map, std::vector<Electron>  veto_electrons, std::vector<Muon>  veto_muons, double w);					   
  void MakeAK4JetPlots(Flavour flavour,AnalyzerParameter param,map<TString, std::vector<Jet> > jet_map, std::vector<Electron>  veto_electrons, std::vector<Muon>  veto_muons, double w);					   
  std::vector<Jet> JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR=0.4);
  bool LeptonPassID(Lepton &lepton, TString ID);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Muon *>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(std::vector<Electron *>& electrons, double TightIso=-999, bool UseMini=false);

  /// Helpful tools
  double GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets);
  double MT(Particle a, Particle b);
  
  // Plotter functions
  void FilAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w);
  void FilAllMuonPlots(TString label , TString cut,  std::vector<Muon> mus, float w);


  /// SR functions
  void FillRegionPlots(HNtypeI_Dilepton::Flavour channel, bool plotCR,TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w);

  void FillSigRegionPlots1(HNtypeI_Dilepton::Flavour channel,int sysdir, TString sys, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11);

  void FillSigRegionPlots2(HNtypeI_Dilepton::Flavour channel,int sysdir, TString sys, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7);

  
  void RunHighMassSR(ChargeType charge_i, TString channel_s,HNtypeI_Dilepton::Flavour flavour, TString charge_s,Particle llCand, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> , std::vector<Muon> muons, Particle METv, int mPV, double w, Event ev, AnalyzerParameter param, bool FullAnalysis);
		   

  int GetNBJets(TString ID, TString WP);

  void FillZZCRPlots(HNtypeI_Dilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);

  void FillWZCRPlots(HNtypeI_Dilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);

  void FillZGCRPlots(HNtypeI_Dilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);
  void FillWGCRPlots(HNtypeI_Dilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w);				 


  void FillAllSignalRegion1(HNtypeI_Dilepton::Flavour flavour, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,  Particle _met,int _npv , double w , bool full);
					
  void FillAllSignalRegion2(HNtypeI_Dilepton::Flavour flavour, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,  Particle _met,int _npv , double w , bool full);

			    

  vector<pair<int, pair<TString, double> > >  GetSyst(HNtypeI_Dilepton::Flavour flavour, bool isdata);
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
  void FillEventCutflow(HNtypeI_Dilepton::ChargeType charge_s, HNtypeI_Dilepton::Flavour version,float wt,TString cut,    TString label);
  void FillEventCutflow(HNtypeI_Dilepton::ChargeType charge_s, HNtypeI_Dilepton::Region version,float wt,TString cut,    TString label);

  //==== LSF SF
  double LSFSF(int lepflav, int dir);

};



#endif

