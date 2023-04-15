#ifndef HNL_LeptonCore_h
#define HNL_LeptonCore_h

#include "TRandom.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"

#include "AnalyzerCore.h"

class HNL_LeptonCore : public AnalyzerCore {


 public:

  enum TheRunEra{
    y2016B,y2016C,y2016D,y2016E,y2016F,y2016G,y2016H,
    y2017B,y2017C,y2017D,y2017E,y2017F,
    y2018A,y2018B,y2018C,y2018D,
    y2016MC,
    y2017MC,
    y2018MC,
    yUL2016B,yUL2016C,yUL2016D,yUL2016E,yUL2016F,yUL2016Flate,yUL2016G,yUL2016H,
    yUL2017B,yUL2017C,yUL2017D,yUL2017E,yUL2017F,
    yUL2018A,yUL2018B,yUL2018C,yUL2018D,
    yUL2016MCAPV,
    yUL2016MCnonAPV,
    yUL2017MC,
  yUL2018MC
  };


  enum ChargeType
  {
    Inclusive=0,
    Plus=1,
    Minus=-1,
  };

  enum Channel
  {
    E=0,
    EE=1,
    EEE=2,
    EEEE=3,
    Mu=10,
    MuMu=11,
    MuMuMu=12,
    MuMuMuMu=13,
    EMu=15,
    EMuL=16,
    EMuLL=17,
    LL=18

  };
  enum SearchRegion
  {
    ChannelDepPresel,
    ChannelDepSR1,
    ChannelDepSR2, 
    ChannelDepSR3,
    ChannelDepFAILSR3,
    ChannelDepDilep,
    ChannelDepTrigger,
    ChannelDepInc,
    ChannelDepIncQ,
    MuonSR,
    ElectronSR,
    ElectronMuonSR,
    MuonSRQQ,
    ElectronSRQQ,
    ElectronMuonSRQQ,
    MuonSRBDT,
    ElectronSRBDT,
    ElectronMuonSRBDT,
    MuonSRBDTQQ,
    ElectronSRBDTQQ,
    ElectronMuonSRBDTQQ,
    SR1,
    SR2,
    SR3,
    SR3BDT,
    SR4,
    Presel,
    PreselSS,
    PreselOS,
    SR,
    sigmm,
    sigee,
    sigem,
    sigmm_17028,
    sigee_17028,
    sigem_17028,
    SR1Tau,
    SR2Tau,
    SR3Tau,  
    ChannelDepCR1,
    ChannelDepCR2,
    ChannelDepCR3,
    CR

  };

  enum LeptonType
  {
    MuonLep=100,
    ElectronLep=101,
    TauLep=102
  };

  HNL_LeptonCore();
  ~HNL_LeptonCore();


  //========== MAIN ANALYZER FUNCTIONS


  // ----- SETUP ANALYZER
  AnalyzerParameter InitialiseHNLParameters( TString analysis_tag, vector<vector<TString> >  param_vec);
  AnalyzerParameter InitialiseHNLParameter(TString s_setup="", TString tag="");  

  
  // ------ Analysis Obj

  std::pair<double,double> METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL =false,bool ispuppi=false);
  std::map<TString, double> cfmap;




  /// Jet Functions                                                                                                                                                                                                 
  TString CloseJetFlavour(std::vector<Jet> jetColl, Lepton* lep);
  TString CloseJetFlavour(std::vector<Jet> jetColl, Muon mu);
  TString CloseJetFlavour(std::vector<Jet> jetColl, Electron el);
  TString CloseJetFlavourInt(std::vector<Jet> jetColl, Lepton* lep);
  TString CloseJetFlavourInt(std::vector<Jet> jetColl, Muon mu);
  TString CloseJetFlavourInt(std::vector<Jet> jetColl, Electron el);


  

  //---- BDT Related                                                                                                                                                                                                
  void initializeAnalyzer();
  void SetupEventMVAReader(bool ee=true, bool mm=true, bool emu=true);

  double MergeMultiMC(vector<TString> vec, TString Method);



  // JETS
  std::vector<Jet> GetHNLJets(TString JetType, AnalyzerParameter param);
  std::vector<FatJet> GetHNLAK8Jets(TString JetType, AnalyzerParameter param);
  
  // ------ General 
  bool IsSignal(); //// checks if sample is Signal
  double ScaleLepToSS(TString bkg, bool isMuon, int lepttype);
  void PrintParam(AnalyzerParameter param); 
  void OutCutFlow(TString lab, double w);
  TString QToString(HNL_LeptonCore::ChargeType q);
  bool CheckLeptonFlavourForChannel(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps);
  double PassEventTypeFilter(vector<Lepton *> leps , vector<Gen> gens);
  bool HasLowMassMeson(std::vector<Lepton *> leps);

  double SetupWeight(Event ev, AnalyzerParameter param);
  double GetPtCutTrigger(TString trigname, int nlep, TString flavour);
  bool CheckSRStream(Event ev,HNL_LeptonCore::Channel channel_ID);
  
  TString GetPtBin(bool mu, double pt);
  

  /// ------ SYSTEMATICS
  vector<AnalyzerParameter::Syst> GetSystList(TString SystType);

  /// ------ Corrections
  double HNLZvtxSF(HNL_LeptonCore::Channel ch);
  bool PassHEMVeto(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps);


  /// ---- TRIGGER
  TString Category(Electron el);
  bool PassPtTrigger(Event ev, vector<TString> triglist,std::vector<Lepton *> leps, bool check_pd=true);
  void TriggerPrintOut(Event ev);
  bool PassTriggerSelection(HNL_LeptonCore::Channel channel,Event ev, std::vector<Lepton *> leps, TString selection, bool check_pd=true);
  bool PassMultiTriggerSelection(HNL_LeptonCore::Channel channel,Event ev, std::vector<Lepton *> leps, TString selectionMain, TString selectionOR);
  bool PassTriggerAndCheckStream(bool apply_ptcut,vector<Lepton*> leps, Event ev, vector<TString> triglist, bool check_pd=true);

  TString  DoubleElectronPD();
  TString  SingleElectronPD();
  TString  MuonEGPD();
  TString  DoubleMuonPD();
  TString DoubleToString(double d);


  ///=============== GET/SELECT OBJECT
  vector<Gen> GetGenLepronsSignal();
  bool SelectChannel(HNL_LeptonCore::Channel channel);
  TString GetProcess();
  TString GetChannelString(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType  q=HNL_LeptonCore::ChargeType::Inclusive);
  std::vector<Jet> SelBJets(std::vector<Jet>& jetColl, JetTagging::Parameters jtp);
  Particle GetvMET(TString METType, AnalyzerParameter param);
  Particle GetvMET(TString METType, AnalyzerParameter param, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Muon> muons, std::vector<Electron> electrons); //JH
  Particle GetvMET(TString METType);
  Particle GetSignalObject(TString obj, TString Sig);

  vector<Muon> GetSignalLeptons(const std::vector<Muon>& MuColl, vector<Gen>& TruthColl);
  vector<Electron> GetSignalLeptons(const std::vector<Electron>& ElColl, vector<Gen>& TruthColl);
  vector<Muon> GetLepCollByRunType(const vector<Muon>& MuColl, vector<Gen>& TruthColl, AnalyzerParameter param, TString Option="NoSel");
  vector<Electron> GetLepCollByRunType(const vector<Electron>& ElColl, vector<Gen>& TruthColl, AnalyzerParameter param, TString Option="NoSel");


  //================== KINEMATIC HELPER
  double M_T(Particle a, Particle b);
  double GetLT(std::vector<Lepton *> leps);
  double GetHT( std::vector<Jet> jets, std::vector<FatJet> fatjets);
  double GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev);
  double GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle met);
  double GetST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev);
  double GetST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets, Particle met);

  bool  ZmasslllWindowCheck(std::vector<Lepton *> leps);
  bool  ZmassOSWindowCheck(std::vector<Lepton *> leps);
  bool  ZmassOSSFWindowCheck(std::vector<Lepton *> leps, double mass_diff);

  double GetMassMinOSSF(std::vector<Lepton *> leps);
  double GetMassMinSSSF(std::vector<Lepton *> leps);
  int GetIndexNonMinOSSF(std::vector<Lepton *> leps);
  int GetIndexNonMinSSSF(std::vector<Lepton *> leps);
  int GetIndexNonBestZ(std::vector<Lepton *> leps,double mass_diff);

  double GetLLMass(std::vector<Lepton *> leps);
  double GetLLMass(std::vector<Muon> leps);
  double GetLLMass(std::vector<Electron> leps);

  double GetMassBestZ(std::vector<Lepton *> leps,  bool bestZ);
  double  GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets, vector<Lepton*> leps);

  //================                                                                                                                                                                                        
  //==== Functions                                                                                                                                                                                          
  //================                                                                                                                                                                                        
  TString GetPtLabel(Muon mu);
  TString GetEtaLabel(Muon mu);

  TString GetElType(Electron el, const std::vector<Gen>& gens);
  TString GetElTypeString(Electron el, const std::vector<Gen>& gens);
  TString GetElTypeTString(Electron el, const std::vector<Gen>& gens);
  TString GetLepTypeTString(const Lepton& lep, const std::vector<Gen>& gens);

  //==== LEPTON CHARGE                                                                                                                                                                                           
  bool SameCharge(vector<Electron> els, int ch=0);
  bool SameCharge(vector<Electron> els, vector<Muon> mus,  int ch=0);
  bool SameCharge(vector<Muon> mus, int ch=0);
  bool SameCharge(std::vector<Lepton *> leps, int ch=0);

  /// global var for user flags
  bool RunFake, RunCF,  RunConv, RunSyst,RunPromptTLRemoval,run_ORTrigger;
  bool IsSkimmed;
  bool Signal;
  bool HEM1516 ,BeforeRun319077;
  bool DEBUG;
 
  bool DblEG_Channel;
  bool DblMuon_Channel;
  bool SglMuon_Channel;
  bool SglEG_Channel;
  bool MuonEG_Channel;
  
  bool IsCentral, RunFullAnalysis;
  TRandom3* rand_;
  

  // Trigger vectors used to store year dependant trigger lists
  
  vector<TString> TrigList_HNL_HighPtMu;
  vector<TString> TrigList_HNL_DblMu;
  vector<TString> TrigList_HNL_Mu;
  vector<TString> TrigList_Full_Mu;
  vector<TString> TrigList_POG_Mu;
  
  // EG
  vector<TString> TrigList_HNL_DblEG;
  vector<TString> TrigList_HNL_EG;
  vector<TString> TrigList_POG_EG;
  vector<TString> TrigList_HNL_HighPtEG;

  vector<TString> TrigList_Full_EG;
  
  // MUG
  vector<TString> TrigList_HNL_MuEG;
  vector<TString> TrigList_POG_MuEG;
  vector<TString> TrigList_Full_MuEG;
  
  
  // Lepton ID
  vector<TString> MuonVetoIDs;
  vector<TString> MuonLooseIDs;
  vector<TString> MuonTightIDs;
  vector<TString> ElectronVetoIDs;
  vector<TString> ElectronLooseIDs;
  // Fake rate file
  vector<TString> FakeRateIDs;
  vector<TString> ElectronTightIDs;


  bool run_Debug;
  // Lepton pT cut

  // obj vectors
  vector<Gen> gens;
  vector<FatJet> AllFatJets;

  double weight_Prefire, weight_Prefire_Up, weight_Prefire_Down;


  //==== My tool 

  // LEP PLOTS

  void FillAllMuonPlots(TString label , TString cut,  std::vector<Muon> muons, double w);
  void FillMuonPlots(TString label , TString cut,  std::vector<Muon> muons, double w);
  void FillMuonKinematicPlots(TString label , TString cut,  Muon mu, double w);


  void FillAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, double w);
  void FillElectronPlots(TString label , TString cut,  std::vector<Electron> els, double w);

  void FillElectronKinematicPlots(TString label , TString cut,  Electron el, double w);

  void FillLeptonKinematicPlots(TString label , TString cut,  Lepton lep, double w);



  // === Cut flow                                                                                                                                                                                      
  void FillTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2);
  void FillFullTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2);


  void FillEventCutflowDef(TString dirname, TString histname, double weight, vector<TString> lables, TString label);
  void FillEventCutflowAll(TString dirname, TString histname, double weight, vector<TString> lables, TString label);


  void FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight);
  void FillEventCutflow(HNL_LeptonCore::SearchRegion sr,double wt,TString cut,    TString label, int verbose_level=0);
  void FillEventCutflowSR(TString analysis_dir_name,HNL_LeptonCore::SearchRegion sr, double event_weight, TString label);
  void FillEventCutflowPerMass(TString dirname,HNL_LeptonCore::SearchRegion sr, double event_weight, TString region_name,   TString label);




  //// ===============================  TOOLS =============================== ////                                                                                                                 
		   
  double GetFilterEffType1(TString SigProcess, int mass);
  double GetXsec(TString SigProcess, int mass);
 

  //// ===============================  SR PLOTS =============================== ////

  void Fill_RegionPlots(HNL_LeptonCore::Channel channel, int plotCR,TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps, Particle  met, double nvtx, double w, int verbose_level=0);
  void Fill_RegionPlots(HNL_LeptonCore::Channel channel, int plotCR,TString label_1, TString label_2,  std::vector<Tau> Taus, std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx, double w,int verbose_level=0);
  void Fill_RegionPlots(HNL_LeptonCore::Channel channel,TString label_1, TString label_2,std::vector<Tau> Taus,  std::vector<Jet> jets,  std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx, double w,int verbose_level=0);



  void FillAK8Plots(HNL_LeptonCore::Channel channel, TString label_1, TString label_2,  std::vector<Tau> Taus, std::vector<Jet> jets,  std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx, double w);


  void Fill_SigRegionPlots1(HNL_LeptonCore::Channel channel,TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7);


  void Fill_SigRegionPlots3(HNL_LeptonCore::Channel channel, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps , Particle  met, double nvtx, double w, double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11);


  void Fill_SigRegionPlots4(HNL_LeptonCore::Channel channel,TString label_1,std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx, double w );


  bool  Fill_DefSigRegionPlots3(HNL_LeptonCore::Channel channel, TString label_sr, TString label_mass, TString label_anid,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11);
					       
					       
  void Fill_All_SignalRegion3(HNL_LeptonCore::Channel channel, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps,  Particle _met,int _npv , double w , bool full);
					
  void Fill_All_SignalRegion1(HNL_LeptonCore::Channel channel, TString signal_region, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Lepton *> leps,  Particle _met,int _npv , double w , bool full);


  //// ===============================  GLOBAL VAR =============================== ////


  TH1D *hist_PUReweight;
  TH1D *hist_PUReweight_Up;
  TH1D *hist_PUReweight_Down;

  double weight_PU, weight_PU_Up, weight_PU_Down;


  /// BDT 


  vector<TString> MNStrList, NCutList, NTreeList;
  map<TString, std::pair<TString,TString> > FinalBDTHyperParamMap;
  TMVA::Reader *MVAReader;
  TMVA::Reader *MVAReaderMM, *MVAReaderEE, *MVAReaderEM;

  /// Event BDT var
  void InitializeTreeVars();
  void SetupEventBDTVariables(std::vector<Lepton *> LepTColl,    std::vector<Jet> JetAllColl,std::vector<Jet> JetColl, std::vector<Jet> JetVBFColl, std::vector<Jet> B_JetColl, Event  ev, Particle METv, AnalyzerParameter param);

  double EvaluateEventMVA(TString mN, TString NCut, TString NTree,HNL_LeptonCore::Channel channel ,
			  std::vector<Lepton *> LepTColl,
			  Event  ev, Particle METv, AnalyzerParameter param);



  Float_t ev_bdt_Nj, ev_bdt_Nvbfj, ev_bdt_Nb, ev_bdt_LQ;
  Float_t ev_bdt_Ptl1, ev_bdt_Ptl2, ev_bdt_Ptj1, ev_bdt_Ptj2, ev_bdt_Ptj3, ev_bdt_MET, ev_bdt_HT, ev_bdt_LT, ev_bdt_HTLT, ev_bdt_HTLT1, ev_bdt_HTLT2, ev_bdt_MET2ST, ev_bdt_MET2HT, ev_bdt_Etal1, ev_bdt_Etal2;
  Float_t ev_bdt_PtRatioAJl1, ev_bdt_PtRatioAJl2;
  Float_t ev_bdt_CEMFracAJl1, ev_bdt_CEMFracAJl2, ev_bdt_NEMFracAJl1, ev_bdt_NEMFracAJl2, ev_bdt_CHFracAJl1, ev_bdt_CHFracAJl2, ev_bdt_NHFracAJl1, ev_bdt_NHFracAJl2, ev_bdt_MuFracAJl1, ev_bdt_MuFracAJl2, ev_bdt_JetDiscAJl1, ev_bdt_JetDiscAJl2;
  Float_t ev_bdt_dEtall, ev_bdt_dRll, ev_bdt_dRjj12, ev_bdt_dRjj23, ev_bdt_dRjj13;
  Float_t ev_bdt_dRlj11, ev_bdt_dRlj12, ev_bdt_dRlj13, ev_bdt_dRlj21, ev_bdt_dRlj22, ev_bdt_dRlj23;
  Float_t ev_bdt_dphivl1, ev_bdt_dphivl2, ev_bdt_dphivj1, ev_bdt_dphivj2, ev_bdt_dphivj3;
  Float_t ev_bdt_MSSSF, ev_bdt_Mlj11, ev_bdt_Mlj12, ev_bdt_Mlj13, ev_bdt_Mlj21, ev_bdt_Mlj22, ev_bdt_Mlj23;
  Float_t ev_bdt_MTvl1, ev_bdt_MTvl2, ev_bdt_MTvll, ev_bdt_Mllj1, ev_bdt_Mllj2, ev_bdt_Mllj3, ev_bdt_Mllj4;
  Float_t ev_bdt_Mlljj12, ev_bdt_Mlljj13, ev_bdt_Mlljj14, ev_bdt_Mlljj23, ev_bdt_Mlljj24, ev_bdt_Mlljj34;
  Float_t ev_bdt_Mljj112, ev_bdt_Mljj113, ev_bdt_Mljj114, ev_bdt_Mljj123, ev_bdt_Mljj124, ev_bdt_Mljj134;
  Float_t ev_bdt_Mljj212, ev_bdt_Mljj213, ev_bdt_Mljj214, ev_bdt_Mljj223, ev_bdt_Mljj224, ev_bdt_Mljj234;
  Float_t ev_bdt_Mjj12, ev_bdt_Mjj13, ev_bdt_Mjj14, ev_bdt_Mjj23, ev_bdt_Mjj24, ev_bdt_Mjj34;

  Float_t ev_bdt_PtWj1, ev_bdt_PtWj2;
  Float_t ev_bdt_dRWjj, ev_bdt_dRlW12, ev_bdt_dRlW22;
  Float_t ev_bdt_M_W2_jj, ev_bdt_M_N1_l1jj, ev_bdt_M_N2_l2jj, ev_bdt_M_W1_lljj;

  Float_t w_tot;



};



#endif

