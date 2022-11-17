#ifndef HNL_LepIDKinVar_h
#define HNL_LepIDKinVar_h

#include "HNL_RegionDefinitions.h"

class HNL_LepIDKinVar : public HNL_RegionDefinitions {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool SS2l, TriLep, TetraLep; 
  bool DblMu, DblEG, MuEG;
  bool SystRun;
  vector<TString> TrigList_DblMu, TrigList_DblEG, TrigList_MuEG;

  void MakeTreeSS2L(HNL_LeptonCore::Channel, vector<Lepton *>  LepTColl,
		    vector<Muon>Muons,
		    vector<Electron> Electrons,
                    vector<Jet>& JetAllColl,
		    float weight, TString Label);

  TTree *tree_mm;
  TTree *tree_ee;

  void WriteHist();
  void InitializeTreeVars();

  Float_t Pt,  Eta;
  Float_t PtRatio, PtRatioAJ, PtRelv0, PtRelv1;
  Float_t CEMFracAJ, NEMFracAJ, CHFracAJ, NHFracAJ,MuFracAJ, JetDiscAJ;
  Float_t CEMFracCJ, NEMFracCJ, CHFracCJ, NHFracCJ, MuFracCJ, JetDiscCJ;
  Float_t Dxy,Dz,DxySig, DzSig, RelIso,IP3D,MVA,MVAIso,Chi2, Minireliso,Full5x5_sigmaIetaIeta,dEtaSeed,dPhiIn,HoverE,TrkIso,InvEminusInvP,ecalPFClusterIso,hcalPFClusterIso;
  Float_t R9,dr03TkSumPt,dr03HcalTowerSumEt,dr03HcalDepth1TowerSumEt,dr03EcalRecHitSumEt, e2x5OverE5x5,e1x5OverE5x5,e15,e25,e55,EtaWidth,PhiWidth,dEtaIn,sigmaIetaIeta, MiniIsoChHad,MiniIsoNHad,MiniIsoPhHad,IsoChHad,IsoNHad,IsoPhHad;

  Int_t isEcalDriven,Pixel_hits,JetNTrk,PileupJetId;
  Int_t Validhits,Matched_stations,Tracker_layers,MissingHits;
  Int_t POGTight, POGMedium;
  
  Int_t PassConversionVeto,IsGsfCtfScPixChargeConsistent, IsGsfScPixChargeConsistent, IsGsfCtfChargeConsistent;

  Float_t w_tot;

  HNL_LepIDKinVar();
  ~HNL_LepIDKinVar();

};



#endif
