#ifndef AnalyzerCore_h
#define AnalyzerCore_h

#include "TLorentzVector.h"
#include "TString.h"
#include "TMath.h"
#include "TH3.h"
#include <sstream>      
#include <ctime>

#include "SKFlatNtuple.h"
#include "Event.h"
#include "Particle.h"
#include "Gen.h"
#include "LHE.h"
#include "Lepton.h"
#include "Muon.h"
#include "Electron.h"
#include "Tau.h"
#include "Photon.h"
#include "JetTaggingParameters.h"
#include "Jet.h"
#include "FatJet.h"

#include "AnalyzerParameter.h"
#include "MCCorrection.h"
#include "PuppiSoftdropMassCorr.h"
#include "FakeBackgroundEstimator.h"
#include "CFBackgroundEstimator.h"
#include "GeneralizedEndpoint.h"
#include "GEScaleSyst.h"
#include "PDFReweight.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"


#define M_Z 91.1876
#define M_W 80.379

class AnalyzerCore: public SKFlatNtuple {

public:

  AnalyzerCore();
  ~AnalyzerCore();

  virtual void initializeAnalyzer(){

  };

  virtual void executeEvent(){

  };

  //==================
  //==== Get objects
  //==================

  //==== When GetAllMuons, we apply Rochester correciton
  //==== Then, Pt orderding can be changed
  //==== So, in the analysis code, do e.g.,
  // vector<Muon> muons = GetMuons(~~~);
  // std::sort(muons.begin(), muons.end(), PtComparing);
  //==== ** Recommend you to do the same for other objects (Electron, Jet, FatJet, ...) **
  inline static bool PtComparing(const Particle& p1, const Particle& p2){ return (p1.Pt() > p2.Pt()); }
  inline static bool PtComparingPtr(Particle* p1, Particle* p2){ return (p1->Pt() > p2->Pt()); }

  Event GetEvent();

  // MC weight functions                                                                                                                         
  double GetLeptonSFEventWeight   (std::vector<Lepton *> leps,AnalyzerParameter param );
  double GetMuonSFEventWeight    (std::vector<Muon> muons,AnalyzerParameter param );
  double GetElectronSFEventWeight(std::vector<Electron> electrons, AnalyzerParameter param );

  // FR functions                                                                                                                                
  double GetFakeWeightMuon(std::vector<Muon> muons , AnalyzerParameter param);
  double GetFakeWeightMuon(std::vector<Muon> muons , std::vector<TString> vtrig, AnalyzerParameter param);
  double GetFakeWeightElectron(std::vector<Electron> electrons , vector<TString> trigs, AnalyzerParameter param);
  double GetFakeWeightElectron(std::vector<Electron> electrons , AnalyzerParameter param);
  double GetFakeRateEl(double eta, double pt, AnalyzerParameter param);
  double GetFakeRateM(double eta, double pt, AnalyzerParameter param);
  double GetFakeWeight(std::vector<Lepton *> leps,  AnalyzerParameter param, bool apply_r);

  double GetCFWeightElectron(std::vector<Electron> electrons ,  AnalyzerParameter param);
  double GetCFWeightElectron(vector<double> el_pt, vector<double> el_eta ,  AnalyzerParameter param);
  double GetCFWeightElectron(std::vector<Lepton* > leps ,  AnalyzerParameter param);
  double GetCFrates(TString id, double pt, double eta); //JH
  double GetCFWeightElectron(vector<Lepton *> lepptrs, AnalyzerParameter param, bool applySF, int syst); //JH
  std::vector<Electron> GetAllElectrons();
  std::vector<Electron> GetElectrons(TString id, double ptmin, double fetamax, bool vetoHEM = false);
  std::vector<Electron> GetElectrons(AnalyzerParameter param, TString id, double ptmin, double fetamax ,bool Run_Fake=false, bool vetoHEM=false);

  std::vector<Electron> GetElectrons(AnalyzerParameter param  ,bool Run_Fake=false, bool vetoHEM = false);
  std::vector<Muon> GetAllMuons();
  std::vector<Muon> GetMuons(TString id, double ptmin, double fetamax);
  std::vector<Muon> GetMuons(AnalyzerParameter param, TString id, double ptmin, double fetamax,bool Run_Fake=false);
  std::vector<Muon> GetMuons(AnalyzerParameter param,bool Run_Fake=false);

  std::vector<Tau> GetAllTaus();
  std::vector<Tau> GetTaus(vector<Lepton*> leps, TString id, double ptmin, double fetamax);
  std::vector<Tau> GetTaus(TString id, double ptmin, double fetamax);

  std::vector<Photon> GetAllPhotons();
  std::vector<Photon> GetPhotons(TString id, double ptmin, double fetamax);


  double GetIsoFromID(TString type_lep, TString id, double eta, double pt);

  bool PassID(std::vector<Electron> electrons, TString ID);

  //==== If TightIso is set, it calculate ptcone
  //==== If UseMini is true, Lepton::RelIso() returns MiniRelIso
  std::vector<Lepton *> MakeLeptonPointerVector(const std::vector<Muon>& muons,const std::vector<Electron>& electrons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(const std::vector<Muon>& muons,const std::vector<Electron>& electrons, AnalyzerParameter param, double TightIso=-999, bool UseMini=false);

  std::vector<Lepton *> MakeLeptonPointerVector(const std::vector<Muon>& muons, double TightIso=-999, bool UseMini=false);
  std::vector<Lepton *> MakeLeptonPointerVector(const std::vector<Electron>& electrons, double TightIso=-999, bool UseMini=false);

  std::vector<Jet> GetAllJets();
  std::vector<Jet> GetJets(AnalyzerParameter param);
  std::vector<Jet> GetJets(AnalyzerParameter param,TString ID, double ptmin, double fetamax);
  std::vector<Jet> GetJets(TString ID, double ptmin, double fetamax);

  double GetJetPileupIDSF(vector<Jet> jets , TString WP, AnalyzerParameter param);

  std::vector<FatJet> GetAllFatJets();
  std::vector<FatJet> GetFatJets(AnalyzerParameter param);
  std::vector<FatJet> GetFatJets(AnalyzerParameter param,TString ID, double ptmin, double fetamax);
  std::vector<FatJet> GetFatJets(TString id, double ptmin, double fetamax);

  std::vector<Gen> GetGens();
  std::vector<LHE> GetLHEs();

  bool ConversionSplitting(std::vector<Lepton *> leps,const std::vector<Gen>& gens);
  bool ConversionVeto(std::vector<Lepton *> leps,const std::vector<Gen>& gens);
  bool IsCF(Electron el, std::vector<Gen> gens);


  //===================================================
  //==== Get objects METHOD 2
  //==== Get AllObject in the begining, and apply cut
  //==================================================+

  std::vector<Electron> SelectElectrons(const std::vector<Electron>& electrons, TString id, double ptmin, double fetamax, bool vetoHEM = false);
  std::vector<Electron> SelectElectrons(const std::vector<Electron>& electrons, TString id, double ptmin, double fetamax, bool cc, double dx_b, double dx_e,double dz_b,double dz_e, double iso_b, double iso_e);


  std::vector<Muon> UseTunePMuon(const std::vector<Muon>& muons);
  std::vector<Muon> SelectMuons(const std::vector<Muon>& muons, TString id, double ptmin, double fetamax);

  std::vector<Tau> SelectTaus(const std::vector<Tau>& taus, TString id, double ptmin, double fetamax);


  std::vector<Jet> SelectJets(const std::vector<Jet>& jets, TString id, double ptmin, double fetamax);

  std::vector<FatJet> SelectFatJets(const std::vector<FatJet>& jets, TString id, double ptmin, double fetamax);

  //==== BJets                                                                                                                                   
  pair<int,double> GetNBJets(vector<Jet>       jets,  TString WP="Medium", TString method="2a");
  pair<int,double> GetNBJets(vector<Jet>       jets,  AnalyzerParameter param, TString WP="Medium");
  //pair<int,double> GetNBJets(TString ID,              TString WP="Medium", TString method="2a");
  //pair<int,double> GetNBJets(AnalyzerParameter param, TString WP="Medium");
  int GetNBJets2a(TString ID, TString WP="Medium");
  int GetNBJets2a( vector<Jet> jets, TString WP="Medium");
  int GetNBJets2a( vector<Jet> jets, AnalyzerParameter param,TString WP="Medium");

  //===== AK8 Jet Syst + weights                                                                                                                 

  double GetEventFatJetSF(vector<FatJet> fatjets, TString label, int dir);
  double GetFatJetSF(FatJet fatjet, TString tag,  int dir);


  //===== Detailed jet selection                                                                                                                 
  vector<Jet>   GetAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean,   TString pu_tag,std::vector<Lepton *> leps_veto,  vector<FatJet> fatjets);

  double  GetBJetSF(AnalyzerParameter param,vector<Jet> jets, JetTagging::Parameters jtp);
  vector<Jet>   GetBJets(AnalyzerParameter param, vector<Jet> jets, JetTagging::Parameters jtp);
  vector<Jet>   GetLJets(AnalyzerParameter param, vector<Jet> jets, JetTagging::Parameters jtp);


  vector<Jet>  GetAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean, TString pu_tag, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, vector<FatJet> fatjets);
  
  vector<FatJet> GetAK8Jetsv2(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut,double WQCDTagger,  vector<Electron>  veto_electrons, vector<Muon>  veto_muons);

  vector<FatJet> GetAK8Jets(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut,   vector<Electron>  veto_electrons, vector<Muon>  veto_muons);

  //==================
  //==== Systematics
  //==================

  std::vector<Electron> ScaleElectrons(const std::vector<Electron>& electrons, int sys);
  std::vector<Electron> SmearElectrons(const std::vector<Electron>& electrons, int sys);

  std::vector<Muon> ScaleMuons(const std::vector<Muon>& muons, int sys);

  std::vector<Jet> ScaleJets(const std::vector<Jet>& jets, int sys);
  std::vector<Jet> SmearJets(const std::vector<Jet>& jets, int sys);

  std::vector<FatJet> ScaleFatJets(const std::vector<FatJet>& jets, int sys);
  std::vector<FatJet> SmearFatJets(const std::vector<FatJet>& jets, int sys);
  std::vector<FatJet> ScaleSDMassFatJets(const std::vector<FatJet>& jets, int sys);
  std::vector<FatJet> SmearSDMassFatJets(const std::vector<FatJet>& jets, int sys);

  //====================
  //==== Event Filters
  //====================

  bool PassMETFilter();

  //============
  //==== Tools
  //============

  //===== Estimators

  MCCorrection *mcCorr=NULL;
  PuppiSoftdropMassCorr *puppiCorr=NULL;
  FakeBackgroundEstimator *fakeEst=NULL;
  CFBackgroundEstimator *cfEst=NULL;
  void initializeAnalyzerTools();

  //==== MCweight
  double MCweight(bool usesign=true, bool norm_1invpb=true) const;

  //==== Kfactors
  float GetKFactor();


  //==== Prefire
  double GetPrefireWeight(int sys);

  //==== PU Reweight
  double GetPileUpWeight(int N_pileup, int syst);

  //==== Muon GeneralizedEngpoint momentum scaling
  GeneralizedEndpoint *muonGE=NULL;
  GEScaleSyst *muonGEScaleSyst=NULL;

  //==== Using new PDF set
  PDFReweight *pdfReweight=NULL;
  double GetPDFWeight(LHAPDF::PDF* pdf_);
  //==== NewCentral/ProdCentral
  double GetPDFReweight();
  //==== NewErrorSet/ProdCentral
  double GetPDFReweight(int member);

  //================
  //==== Functions
  //================
 
  bool IsOnZ(double m, double width);
  double MT(TLorentzVector a, TLorentzVector b);
  double MT2(TLorentzVector a, TLorentzVector b, Particle METv, double METgap);
  double projectedMET(TLorentzVector a, TLorentzVector b, Particle METv);
  bool HasFlag(TString flag);

  

  std::vector<Muon> MuonWithoutGap(const std::vector<Muon>& muons);
  std::vector<Muon> MuonPromptOnly(const std::vector<Muon>& muons, const std::vector<Gen>& gens,AnalyzerParameter param);
  std::vector<Muon> MuonPromptOnly(const std::vector<Muon>& muons, const std::vector<Gen>& gens);
  TString PromptStatus(Muon mu, const std::vector<Gen>& gens);
  
  std::vector<Muon> MuonUsePtCone(const std::vector<Muon>& muons);
  Muon MuonUsePtCone(const Muon& muon);
  Particle UpdateMET(const Particle& METv, const std::vector<Muon>& muons);
  Particle UpdateMETSmearedJet(const Particle& METv, const std::vector<Jet>& jets);

  std::vector<Muon> MuonApplyPtCut(const std::vector<Muon>& muons, double ptcut);
  std::vector<Electron> ElectronPromptOnly(const std::vector<Electron>& electrons, const std::vector<Gen>& gens,AnalyzerParameter param);
  std::vector<Electron> ElectronPromptOnly(const std::vector<Electron>& electrons, const std::vector<Gen>& gens);
  std::vector<Electron> ElectronUsePtCone(const std::vector<Electron>& electrons);
  Electron ElectronUsePtCone(const Electron& electron);
  std::vector<Electron> ElectronApplyPtCut(const std::vector<Electron>& electrons, double ptcut);
  std::vector<Jet> JetsAwayFromFatJet(const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets, double mindr=1.0);
  std::vector<Jet> JetsVetoLeptonInside(const std::vector<Jet>& jets, const std::vector<Electron>& els, const std::vector<Muon>& mus, double dR=0.4);
  std::vector<FatJet> FatJetsVetoLeptonInside(const std::vector<FatJet>& jets, const std::vector<Electron>& els, const std::vector<Muon>& mus, double dR=0.8);
  std::vector<Jet> JetsAwayFromPhoton(const std::vector<Jet>& jets, const std::vector<Photon>& photons, double mindr);
  Particle AddFatJetAndLepton(const FatJet& fatjet, const Lepton& lep);



  //--------- JA                                                                                                                                 

  void PrintEvent(AnalyzerParameter param,TString selection,double w);
  void FillEventComparisonFile(AnalyzerParameter param, TString label,string time, double w);


  //==== GenMatching

  void PrintGen(const std::vector<Gen>& gens);
  static Gen GetGenMatchedLepton(const Lepton& lep, const std::vector<Gen>& gens);
  static Gen GetGenMatchedPhoton(const Lepton& lep, const std::vector<Gen>& gens);
  static vector<int> TrackGenSelfHistory(const Gen& me, const std::vector<Gen>& gens);
  bool IsFromHadron(const Gen& me, const std::vector<Gen>& gens);
  int GetLeptonType(const Lepton& lep, const std::vector<Gen>& gens);
  int GetLeptonType_Public(int TruthIdx, const std::vector<Gen>& TruthColl);
  int GetGenPhotonType(const Gen& genph, const std::vector<Gen>& gens);
  static bool IsFinalPhotonSt23_Public(const std::vector<Gen>& TruthColl);
  int  GetPrElType_InSameSCRange_Public(int TruthIdx, const std::vector<Gen>& TruthColl);

  int  GenMatchedIdx(const Lepton& Lep, std::vector<Gen>& truthColl);
  int  GetNearPhotonIdx(const Lepton& Lep, std::vector<Gen>& TruthColl);
  int  FirstNonSelfMotherIdx(int TruthIdx, std::vector<Gen>& TruthColl);
  int  LastSelfMotherIdx(int TruthIdx,std::vector<Gen>& TruthColl);
  bool HasHadronicAncestor(int TruthIdx, std::vector<Gen>& TruthColl);
  bool IsFinalPhotonSt23(std::vector<Gen>& TruthColl);
  int  GetPrElType_InSameSCRange(int TruthIdx, std::vector<Gen>& TruthColl, TString Option="");

  bool IsSignalPID(int pid);

  

  // HEM code
  bool FindHEMElectron(Electron electron);

  //==== Plotting

  std::map< TString, TH1D* > maphist_TH1D;
  std::map< TString, TH2D* > maphist_TH2D;
  std::map< TString, TH3D* > maphist_TH3D;

  TH1D* GetHist1D(TString histname);
  TH2D* GetHist2D(TString histname);
  TH3D* GetHist3D(TString histname);

  // === Ev weights                                                                                                                              
  void FillWeightHist(TString label, double _weight);

  // === HIST settings/filling                        

  void FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max, TString label="");
  void FillHist(TString histname, double value, double weight, int n_bin, double *xbins, TString label="");

  void FillHist(TString histname,
                double value_x, double value_y,
                double weight,
                int n_binx, double x_min, double x_max,
                int n_biny, double y_min, double y_max);
  void FillHist(TString histname,
                double value_x, double value_y,
                double weight,
                int n_binx, double *xbins,
                int n_biny, double *ybins);
  void FillHist(TString histname,
		double value_x, double value_y, double value_z,
		double weight,
		int n_binx, double x_min, double x_max,
		int n_biny, double y_min, double y_max,
		int n_binz, double z_min, double z_max);
  void FillHist(TString histname,
		double value_x, double value_y, double value_z,
		double weight,
		int n_binx, const double *xbins,
		int n_biny, const double *ybins,
		int n_binz, const double *zbins);

  //==== JSFillHist : 1D
  std::map< TString, std::map<TString, TH1D*> > JSmaphist_TH1D;
  TH1D* JSGetHist1D(TString suffix, TString histname);
  void JSFillHist(TString suffix, TString histname, double value, double weight, int n_bin, double x_min, double x_max);
  //==== JSFillHist : 2D
  std::map< TString, std::map<TString, TH2D*> > JSmaphist_TH2D;
  TH2D* JSGetHist2D(TString suffix, TString histname);
  void JSFillHist(TString suffix, TString histname,
                  double value_x, double value_y,
                  double weight,
                  int n_binx, double x_min, double x_max,
                  int n_biny, double y_min, double y_max);
  void JSFillHist(TString suffix, TString histname,
                  double value_x, double value_y,
                  double weight,
                  int n_binx, double *xbins,
                  int n_biny, double *ybins);

  virtual void WriteHist();

  //==== Quick Plotters
  void FillLeptonPlots(std::vector<Lepton *> leps, TString this_region, double weight);
  void FillJetPlots(std::vector<Jet> jets, std::vector<FatJet> fatjets, TString this_region, double weight);

  //==== Output rootfile
  void SwitchToTempDir();
  TFile *outfile=NULL;
  void SetOutfilePath(TString outname);


  // JA Added functions 

  vector<Muon> SkimLepColl(const vector<Muon>& MuColl, vector<Gen>& TruthColl, AnalyzerParameter param, TString Option);
  vector<Electron> SkimLepColl(const vector<Electron>& ElColl, vector<Gen>& TruthColl, AnalyzerParameter param,TString Option);
  vector<Electron> SkimLepColl(const vector<Electron>& ElColl,TString Option);
  vector<Muon> SkimLepColl(const vector<Muon>& MuColl,  TString Option);
  vector<Jet> SkimJetColl(const vector<Jet>& JetColl, vector<Gen>& TruthColl, AnalyzerParameter param,TString Option);
  bool HasEWLepInJet(Jet Jet, vector<Gen>& TruthColl, TString Option);


  // JH functions 

  // HEM code                                                                                                                                                

  bool IsHEMIssueRun();
  bool IsHEMIssueReg(Particle& Particle);
  bool IsHEMCRReg(Particle& Particle, TString Option);

  int  GetPartonType_JH(int TruthIdx, std::vector<Gen>& TruthColl);
  int  GetLeptonType_JH(int TruthIdx, std::vector<Gen>& TruthColl);
  int  GetLeptonType_JH(const Lepton& Lep, std::vector<Gen>& TruthColl);
  int  GetPhotonType_JH(int PhotonIdx, std::vector<Gen>& TruthColl);
  int  GetFakeLepSrcType(const Lepton& Lep, vector<Jet>& JetColl);



  string run_timestamp;

};



#endif

