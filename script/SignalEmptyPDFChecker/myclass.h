//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb  3 14:11:25 2020 by ROOT version 6.12/07
// from TTree SKFlat/SKFlat
// found on file: /gv0/DATA/SKFlat/Run2Legacy_v4/2018/PrivateMC/WR_MG_LO_FastSim/WRtoNLtoLLJJ_WR4000_N2000/2020_01_30_215526/SKFlatNtuple_2018_MC_47.root
//////////////////////////////////////////////////////////

#ifndef myclass_h
#define myclass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "vector"
#include "vector"

class myclass {
public :
   TChain          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Bool_t          IsData;
   Int_t           nTotal;
   Int_t           run;
   ULong64_t       event;
   Int_t           lumi;
   Double_t        Rho;
   Int_t           nPV;
   Bool_t          Flag_goodVertices;
   Bool_t          Flag_globalTightHalo2016Filter;
   Bool_t          Flag_globalSuperTightHalo2016Filter;
   Bool_t          Flag_HBHENoiseFilter;
   Bool_t          Flag_HBHENoiseIsoFilter;
   Bool_t          Flag_EcalDeadCellTriggerPrimitiveFilter;
   Bool_t          Flag_BadPFMuonFilter;
   Bool_t          Flag_BadChargedCandidateFilter;
   Bool_t          Flag_eeBadScFilter;
   Bool_t          Flag_ecalBadCalibReducedMINIAODFilter;
   Int_t           PVtrackSize;
   Double_t        PVchi2;
   Double_t        PVndof;
   Double_t        PVnormalizedChi2;
   Double_t        vertex_X;
   Double_t        vertex_Y;
   Double_t        vertex_Z;
   vector<string>  *HLT_TriggerName;
   vector<double>  *jet_pt;
   vector<double>  *jet_eta;
   vector<double>  *jet_phi;
   vector<double>  *jet_charge;
   vector<double>  *jet_area;
   vector<int>     *jet_partonFlavour;
   vector<int>     *jet_hadronFlavour;
   vector<double>  *jet_CSVv2;
   vector<double>  *jet_DeepCSV;
   vector<double>  *jet_CvsL;
   vector<double>  *jet_CvsB;
   vector<double>  *jet_DeepFlavour_b;
   vector<double>  *jet_DeepFlavour_bb;
   vector<double>  *jet_DeepFlavour_lepb;
   vector<double>  *jet_DeepFlavour_c;
   vector<double>  *jet_DeepFlavour_uds;
   vector<double>  *jet_DeepFlavour_g;
   vector<double>  *jet_DeepCvsL;
   vector<double>  *jet_DeepCvsB;
   vector<double>  *jet_chargedHadronEnergyFraction;
   vector<double>  *jet_neutralHadronEnergyFraction;
   vector<double>  *jet_neutralEmEnergyFraction;
   vector<double>  *jet_chargedEmEnergyFraction;
   vector<double>  *jet_muonEnergyFraction;
   vector<int>     *jet_chargedMultiplicity;
   vector<int>     *jet_neutralMultiplicity;
   vector<bool>    *jet_tightJetID;
   vector<bool>    *jet_tightLepVetoJetID;
   vector<int>     *jet_partonPdgId;
   vector<int>     *jet_vtxNtracks;
   vector<double>  *jet_m;
   vector<double>  *jet_energy;
   vector<double>  *jet_PileupJetId;
   vector<double>  *jet_shiftedEnUp;
   vector<double>  *jet_shiftedEnDown;
   vector<double>  *jet_smearedRes;
   vector<double>  *jet_smearedResUp;
   vector<double>  *jet_smearedResDown;
   vector<double>  *jet_JECL1FastJet;
   vector<double>  *jet_JECFull;
   vector<double>  *fatjet_pt;
   vector<double>  *fatjet_eta;
   vector<double>  *fatjet_phi;
   vector<double>  *fatjet_charge;
   vector<double>  *fatjet_area;
   vector<int>     *fatjet_partonFlavour;
   vector<int>     *fatjet_hadronFlavour;
   vector<double>  *fatjet_CSVv2;
   vector<double>  *fatjet_DeepCSV;
   vector<double>  *fatjet_DeepFlavour_b;
   vector<double>  *fatjet_DeepFlavour_bb;
   vector<double>  *fatjet_DeepFlavour_lepb;
   vector<double>  *fatjet_DeepFlavour_c;
   vector<double>  *fatjet_DeepFlavour_uds;
   vector<double>  *fatjet_DeepFlavour_g;
   vector<double>  *fatjet_CvsL;
   vector<double>  *fatjet_CvsB;
   vector<double>  *fatjet_DeepCvsL;
   vector<double>  *fatjet_DeepCvsB;
   vector<bool>    *fatjet_tightJetID;
   vector<bool>    *fatjet_tightLepVetoJetID;
   vector<int>     *fatjet_partonPdgId;
   vector<int>     *fatjet_vtxNtracks;
   vector<double>  *fatjet_m;
   vector<double>  *fatjet_energy;
   vector<double>  *fatjet_puppi_tau1;
   vector<double>  *fatjet_puppi_tau2;
   vector<double>  *fatjet_puppi_tau3;
   vector<double>  *fatjet_puppi_tau4;
   vector<double>  *fatjet_softdropmass;
   vector<double>  *fatjet_chargedHadronEnergyFraction;
   vector<double>  *fatjet_neutralHadronEnergyFraction;
   vector<double>  *fatjet_neutralEmEnergyFraction;
   vector<double>  *fatjet_chargedEmEnergyFraction;
   vector<double>  *fatjet_muonEnergyFraction;
   vector<int>     *fatjet_chargedMultiplicity;
   vector<int>     *fatjet_neutralMultiplicity;
   vector<double>  *fatjet_shiftedEnUp;
   vector<double>  *fatjet_shiftedEnDown;
   vector<double>  *fatjet_smearedRes;
   vector<double>  *fatjet_smearedResUp;
   vector<double>  *fatjet_smearedResDown;
   vector<double>  *fatjet_LSF;
   vector<double>  *fatjet_LSFlep_PID;
   vector<double>  *fatjet_LSFlep_Pt;
   vector<double>  *fatjet_LSFlep_Eta;
   vector<double>  *fatjet_LSFlep_Phi;
   vector<double>  *electron_MVAIso;
   vector<double>  *electron_MVANoIso;
   vector<double>  *electron_Energy;
   vector<double>  *electron_Energy_Scale_Up;
   vector<double>  *electron_Energy_Scale_Down;
   vector<double>  *electron_Energy_Smear_Up;
   vector<double>  *electron_Energy_Smear_Down;
   vector<double>  *electron_eta;
   vector<double>  *electron_phi;
   vector<int>     *electron_charge;
   vector<double>  *electron_gsfpt;
   vector<double>  *electron_gsfEta;
   vector<double>  *electron_gsfPhi;
   vector<int>     *electron_gsfCharge;
   vector<double>  *electron_scEta;
   vector<double>  *electron_scPhi;
   vector<double>  *electron_etaWidth;
   vector<double>  *electron_phiWidth;
   vector<double>  *electron_dEtaIn;
   vector<double>  *electron_dEtaInSeed;
   vector<double>  *electron_dPhiIn;
   vector<double>  *electron_sigmaIEtaIEta;
   vector<double>  *electron_Full5x5_SigmaIEtaIEta;
   vector<double>  *electron_e2x5OverE5x5;
   vector<double>  *electron_e1x5OverE5x5;
   vector<double>  *electron_HoverE;
   vector<double>  *electron_fbrem;
   vector<double>  *electron_eOverP;
   vector<double>  *electron_InvEminusInvP;
   vector<double>  *electron_dxyVTX;
   vector<double>  *electron_dxyerrVTX;
   vector<double>  *electron_dzVTX;
   vector<double>  *electron_dzerrVTX;
   vector<double>  *electron_3DIPVTX;
   vector<double>  *electron_3DIPerrVTX;
   vector<double>  *electron_dxy;
   vector<double>  *electron_sigdxy;
   vector<double>  *electron_dz;
   vector<double>  *electron_dxyBS;
   vector<double>  *electron_dzBS;
   vector<double>  *electron_chIso03;
   vector<double>  *electron_nhIso03;
   vector<double>  *electron_phIso03;
   vector<double>  *electron_puChIso03;
   vector<bool>    *electron_passConversionVeto;
   vector<bool>    *electron_isGsfCtfScPixChargeConsistent;
   vector<bool>    *electron_isGsfScPixChargeConsistent;
   vector<bool>    *electron_isGsfCtfChargeConsistent;
   vector<int>     *electron_mHits;
   vector<int>     *electron_ecalDriven;
   vector<double>  *electron_r9;
   vector<double>  *electron_scEnergy;
   vector<double>  *electron_scPreEnergy;
   vector<double>  *electron_scRawEnergy;
   vector<double>  *electron_scEt;
   vector<double>  *electron_E15;
   vector<double>  *electron_E25;
   vector<double>  *electron_E55;
   vector<double>  *electron_RelPFIso_dBeta;
   vector<double>  *electron_RelPFIso_Rho;
   vector<unsigned int> *electron_IDBit;
   vector<int>     *electron_IDCutBit;
   vector<double>  *electron_EnergyUnCorr;
   vector<double>  *electron_chMiniIso;
   vector<double>  *electron_nhMiniIso;
   vector<double>  *electron_phMiniIso;
   vector<double>  *electron_puChMiniIso;
   vector<double>  *electron_trackIso;
   vector<double>  *electron_dr03EcalRecHitSumEt;
   vector<double>  *electron_dr03HcalDepth1TowerSumEt;
   vector<double>  *electron_dr03HcalTowerSumEt;
   vector<double>  *electron_dr03TkSumPt;
   vector<double>  *electron_ecalPFClusterIso;
   vector<double>  *electron_hcalPFClusterIso;
   vector<double>  *muon_PfChargedHadronIsoR04;
   vector<double>  *muon_PfNeutralHadronIsoR04;
   vector<double>  *muon_PfGammaIsoR04;
   vector<double>  *muon_PFSumPUIsoR04;
   vector<double>  *muon_PfChargedHadronIsoR03;
   vector<double>  *muon_PfNeutralHadronIsoR03;
   vector<double>  *muon_PfGammaIsoR03;
   vector<double>  *muon_PFSumPUIsoR03;
   vector<unsigned int> *muon_TypeBit;
   vector<unsigned int> *muon_IDBit;
   vector<bool>    *muon_ishighpt;
   vector<double>  *muon_dB;
   vector<double>  *muon_phi;
   vector<double>  *muon_eta;
   vector<double>  *muon_pt;
   vector<double>  *muon_mass;
   vector<double>  *muon_trkiso;
   vector<double>  *muon_hcaliso;
   vector<double>  *muon_ecaliso;
   vector<double>  *muon_trkisoR05;
   vector<double>  *muon_hcalisoR05;
   vector<double>  *muon_ecalisoR05;
   vector<int>     *muon_charge;
   vector<int>     *muon_nChambers;
   vector<int>     *muon_matchedstations;
   vector<int>     *muon_stationMask;
   vector<int>     *muon_nSegments;
   vector<double>  *muon_normchi;
   vector<int>     *muon_validhits;
   vector<int>     *muon_trackerHits;
   vector<int>     *muon_pixelHits;
   vector<int>     *muon_validmuonhits;
   vector<int>     *muon_trackerLayers;
   vector<double>  *muon_qoverp;
   vector<double>  *muon_theta;
   vector<double>  *muon_lambda;
   vector<double>  *muon_dxy;
   vector<double>  *muon_d0;
   vector<double>  *muon_dsz;
   vector<double>  *muon_dz;
   vector<double>  *muon_dxyBS;
   vector<double>  *muon_dzBS;
   vector<double>  *muon_dszBS;
   vector<double>  *muon_dxyVTX;
   vector<double>  *muon_dxyerrVTX;
   vector<double>  *muon_dzVTX;
   vector<double>  *muon_dzerrVTX;
   vector<double>  *muon_3DIPVTX;
   vector<double>  *muon_3DIPerrVTX;
   vector<double>  *muon_dszVTX;
   vector<double>  *muon_vx;
   vector<double>  *muon_vy;
   vector<double>  *muon_vz;
   vector<double>  *muon_Best_pt;
   vector<double>  *muon_Best_ptError;
   vector<double>  *muon_Best_eta;
   vector<double>  *muon_Best_phi;
   vector<double>  *muon_Inner_pt;
   vector<double>  *muon_Inner_ptError;
   vector<double>  *muon_Inner_eta;
   vector<double>  *muon_Inner_phi;
   vector<double>  *muon_Outer_pt;
   vector<double>  *muon_Outer_ptError;
   vector<double>  *muon_Outer_eta;
   vector<double>  *muon_Outer_phi;
   vector<double>  *muon_GLB_pt;
   vector<double>  *muon_GLB_ptError;
   vector<double>  *muon_GLB_eta;
   vector<double>  *muon_GLB_phi;
   vector<double>  *muon_TuneP_pt;
   vector<double>  *muon_TuneP_ptError;
   vector<double>  *muon_TuneP_eta;
   vector<double>  *muon_TuneP_phi;
   vector<double>  *muon_TuneP_charge;
   vector<double>  *muon_roch_sf;
   vector<double>  *muon_roch_sf_up;
   vector<double>  *muon_PfChargedHadronMiniIso;
   vector<double>  *muon_PfNeutralHadronMiniIso;
   vector<double>  *muon_PfGammaMiniIso;
   vector<double>  *muon_PFSumPUMiniIso;
   vector<double>  *muon_MVA;
   vector<double>  *muon_lowptMVA;
   vector<double>  *muon_softMVA;
   vector<double>  *muon_jetPtRatio;
   vector<double>  *muon_jetPtRel;
   vector<int>     *muon_simType;
   vector<int>     *muon_simExtType;
   vector<int>     *muon_simFlavour;
   vector<int>     *muon_simHeaviestMotherFlavour;
   vector<int>     *muon_simPdgId;
   vector<int>     *muon_simMotherPdgId;
   vector<double>  *muon_simMatchQuality;
   vector<double>  *LHE_Px;
   vector<double>  *LHE_Py;
   vector<double>  *LHE_Pz;
   vector<double>  *LHE_E;
   vector<int>     *LHE_Status;
   vector<int>     *LHE_ID;
   vector<double>  *PDFWeights_Scale;
   vector<double>  *PDFWeights_Error;
   vector<double>  *PDFWeights_AlphaS;
   vector<double>  *gen_phi;
   vector<double>  *gen_eta;
   vector<double>  *gen_pt;
   vector<double>  *gen_mass;
   vector<double>  *gen_charge;
   vector<int>     *gen_mother_index;
   vector<int>     *gen_status;
   vector<int>     *gen_PID;
   vector<int>     *gen_isPrompt;
   vector<int>     *gen_isPromptFinalState;
   vector<int>     *gen_isTauDecayProduct;
   vector<int>     *gen_isPromptTauDecayProduct;
   vector<int>     *gen_isDirectPromptTauDecayProductFinalState;
   vector<int>     *gen_isHardProcess;
   vector<int>     *gen_isLastCopy;
   vector<int>     *gen_isLastCopyBeforeFSR;
   vector<int>     *gen_isPromptDecayed;
   vector<int>     *gen_isDecayedLeptonHadron;
   vector<int>     *gen_fromHardProcessBeforeFSR;
   vector<int>     *gen_fromHardProcessDecayed;
   vector<int>     *gen_fromHardProcessFinalState;
   vector<int>     *gen_isMostlyLikePythia6Status3;
   Double_t        gen_weight;
   Double_t        genWeight_Q;
   Double_t        genWeight_X1;
   Double_t        genWeight_X2;
   Int_t           genWeight_id1;
   Int_t           genWeight_id2;
   Double_t        genWeight_alphaQCD;
   Double_t        genWeight_alphaQED;
   vector<double>  *photon_Energy;
   vector<double>  *photon_EnergyUnCorr;
   vector<double>  *photon_eta;
   vector<double>  *photon_phi;
   vector<double>  *photon_scEta;
   vector<double>  *photon_scPhi;
   vector<double>  *photon_HoverE;
   vector<int>     *photon_hasPixelSeed;
   vector<double>  *photon_Full5x5_SigmaIEtaIEta;
   vector<double>  *photon_ChIso;
   vector<double>  *photon_NhIso;
   vector<double>  *photon_PhIso;
   vector<double>  *photon_ChIsoWithEA;
   vector<double>  *photon_NhIsoWithEA;
   vector<double>  *photon_PhIsoWithEA;
   vector<bool>    *photon_passMVAID_WP80;
   vector<bool>    *photon_passMVAID_WP90;
   vector<bool>    *photon_passLooseID;
   vector<bool>    *photon_passMediumID;
   vector<bool>    *photon_passTightID;
   Int_t           nPileUp;
   Double_t        pfMET_pt;
   Double_t        pfMET_phi;
   Double_t        pfMET_SumEt;
   Double_t        pfMET_Type1_pt;
   Double_t        pfMET_Type1_phi;
   Double_t        pfMET_Type1_SumEt;
   Double_t        pfMET_Type1_PhiCor_pt;
   Double_t        pfMET_Type1_PhiCor_phi;
   Double_t        pfMET_Type1_PhiCor_SumEt;
   vector<double>  *pfMET_pt_shifts;
   vector<double>  *pfMET_phi_shifts;
   vector<double>  *pfMET_SumEt_shifts;
   vector<double>  *pfMET_Type1_pt_shifts;
   vector<double>  *pfMET_Type1_phi_shifts;
   vector<double>  *pfMET_Type1_SumEt_shifts;
   vector<double>  *pfMET_Type1_PhiCor_pt_shifts;
   vector<double>  *pfMET_Type1_PhiCor_phi_shifts;
   vector<double>  *pfMET_Type1_PhiCor_SumEt_shifts;

   // List of branches
   TBranch        *b_IsData;   //!
   TBranch        *b_nTotal;   //!
   TBranch        *b_runNum;   //!
   TBranch        *b_evtNum;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_Rho;   //!
   TBranch        *b_nPV;   //!
   TBranch        *b_Flag_goodVertices;   //!
   TBranch        *b_Flag_globalTightHalo2016Filter;   //!
   TBranch        *b_Flag_globalSuperTightHalo2016Filter;   //!
   TBranch        *b_Flag_HBHENoiseFilter;   //!
   TBranch        *b_Flag_HBHENoiseIsoFilter;   //!
   TBranch        *b_Flag_EcalDeadCellTriggerPrimitiveFilter;   //!
   TBranch        *b_Flag_BadPFMuonFilter;   //!
   TBranch        *b_Flag_BadChargedCandidateFilter;   //!
   TBranch        *b_Flag_eeBadScFilter;   //!
   TBranch        *b_Flag_ecalBadCalibReducedMINIAODFilter;   //!
   TBranch        *b_PVtrackSize;   //!
   TBranch        *b_PVchi2;   //!
   TBranch        *b_PVndof;   //!
   TBranch        *b_PVnormalizedChi2;   //!
   TBranch        *b_PVx;   //!
   TBranch        *b_PVy;   //!
   TBranch        *b_PVz;   //!
   TBranch        *b_HLT_TriggerName;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_charge;   //!
   TBranch        *b_jet_area;   //!
   TBranch        *b_jet_partonFlavour;   //!
   TBranch        *b_jet_hadronFlavour;   //!
   TBranch        *b_jet_CSVv2;   //!
   TBranch        *b_jet_DeepCSV;   //!
   TBranch        *b_jet_CvsL;   //!
   TBranch        *b_jet_CvsB;   //!
   TBranch        *b_jet_DeepFlavour_b;   //!
   TBranch        *b_jet_DeepFlavour_bb;   //!
   TBranch        *b_jet_DeepFlavour_lepb;   //!
   TBranch        *b_jet_DeepFlavour_c;   //!
   TBranch        *b_jet_DeepFlavour_uds;   //!
   TBranch        *b_jet_DeepFlavour_g;   //!
   TBranch        *b_jet_DeepCvsL;   //!
   TBranch        *b_jet_DeepCvsB;   //!
   TBranch        *b_jet_chargedHadronEnergyFraction;   //!
   TBranch        *b_jet_neutralHadronEnergyFraction;   //!
   TBranch        *b_jet_neutralEmEnergyFraction;   //!
   TBranch        *b_jet_chargedEmEnergyFraction;   //!
   TBranch        *b_jet_muonEnergyFraction;   //!
   TBranch        *b_jet_chargedMultiplicity;   //!
   TBranch        *b_jet_neutralMultiplicity;   //!
   TBranch        *b_jet_tightJetID;   //!
   TBranch        *b_jet_tightLepVetoJetID;   //!
   TBranch        *b_jet_partonPdgId;   //!
   TBranch        *b_jet_vtxNtracks;   //!
   TBranch        *b_jet_m;   //!
   TBranch        *b_jet_energy;   //!
   TBranch        *b_jet_PileupJetId;   //!
   TBranch        *b_jet_shiftedEnUp;   //!
   TBranch        *b_jet_shiftedEnDown;   //!
   TBranch        *b_jet_smearedRes;   //!
   TBranch        *b_jet_smearedResUp;   //!
   TBranch        *b_jet_smearedResDown;   //!
   TBranch        *b_jet_JECL1FastJet;   //!
   TBranch        *b_jet_JECFull;   //!
   TBranch        *b_fatjet_pt;   //!
   TBranch        *b_fatjet_eta;   //!
   TBranch        *b_fatjet_phi;   //!
   TBranch        *b_fatjet_charge;   //!
   TBranch        *b_fatjet_area;   //!
   TBranch        *b_fatjet_partonFlavour;   //!
   TBranch        *b_fatjet_hadronFlavour;   //!
   TBranch        *b_fatjet_CSVv2;   //!
   TBranch        *b_fatjet_DeepCSV;   //!
   TBranch        *b_fatjet_DeepFlavour_b;   //!
   TBranch        *b_fatjet_DeepFlavour_bb;   //!
   TBranch        *b_fatjet_DeepFlavour_lepb;   //!
   TBranch        *b_fatjet_DeepFlavour_c;   //!
   TBranch        *b_fatjet_DeepFlavour_uds;   //!
   TBranch        *b_fatjet_DeepFlavour_g;   //!
   TBranch        *b_fatjet_CvsL;   //!
   TBranch        *b_fatjet_CvsB;   //!
   TBranch        *b_fatjet_DeepCvsL;   //!
   TBranch        *b_fatjet_DeepCvsB;   //!
   TBranch        *b_fatjet_tightJetID;   //!
   TBranch        *b_fatjet_tightLepVetoJetID;   //!
   TBranch        *b_fatjet_partonPdgId;   //!
   TBranch        *b_fatjet_vtxNtracks;   //!
   TBranch        *b_fatjet_m;   //!
   TBranch        *b_fatjet_energy;   //!
   TBranch        *b_fatjet_puppi_tau1;   //!
   TBranch        *b_fatjet_puppi_tau2;   //!
   TBranch        *b_fatjet_puppi_tau3;   //!
   TBranch        *b_fatjet_puppi_tau4;   //!
   TBranch        *b_fatjet_softdropmass;   //!
   TBranch        *b_fatjet_chargedHadronEnergyFraction;   //!
   TBranch        *b_fatjet_neutralHadronEnergyFraction;   //!
   TBranch        *b_fatjet_neutralEmEnergyFraction;   //!
   TBranch        *b_fatjet_chargedEmEnergyFraction;   //!
   TBranch        *b_fatjet_muonEnergyFraction;   //!
   TBranch        *b_fatjet_chargedMultiplicity;   //!
   TBranch        *b_fatjet_neutralMultiplicity;   //!
   TBranch        *b_fatjet_shiftedEnUp;   //!
   TBranch        *b_fatjet_shiftedEnDown;   //!
   TBranch        *b_fatjet_smearedRes;   //!
   TBranch        *b_fatjet_smearedResUp;   //!
   TBranch        *b_fatjet_smearedResDown;   //!
   TBranch        *b_fatjet_LSF;   //!
   TBranch        *b_fatjet_LSFlep_PID;   //!
   TBranch        *b_fatjet_LSFlep_Pt;   //!
   TBranch        *b_fatjet_LSFlep_Eta;   //!
   TBranch        *b_fatjet_LSFlep_Phi;   //!
   TBranch        *b_electron_MVAIso;   //!
   TBranch        *b_electron_MVANoIso;   //!
   TBranch        *b_electron_Energy;   //!
   TBranch        *b_electron_Energy_Scale_Up;   //!
   TBranch        *b_electron_Energy_Scale_Down;   //!
   TBranch        *b_electron_Energy_Smear_Up;   //!
   TBranch        *b_electron_Energy_Smear_Down;   //!
   TBranch        *b_electron_eta;   //!
   TBranch        *b_electron_phi;   //!
   TBranch        *b_electron_charge;   //!
   TBranch        *b_electron_gsfpt;   //!
   TBranch        *b_electron_gsfEta;   //!
   TBranch        *b_electron_gsfPhi;   //!
   TBranch        *b_electron_gsfCharge;   //!
   TBranch        *b_electron_scEta;   //!
   TBranch        *b_electron_scPhi;   //!
   TBranch        *b_electron_etaWidth;   //!
   TBranch        *b_electron_phiWidth;   //!
   TBranch        *b_electron_dEtaIn;   //!
   TBranch        *b_electron_dEtaInSeed;   //!
   TBranch        *b_electron_dPhiIn;   //!
   TBranch        *b_electron_sigmaIEtaIEta;   //!
   TBranch        *b_electron_Full5x5_SigmaIEtaIEta;   //!
   TBranch        *b_electron_e2x5OverE5x5;   //!
   TBranch        *b_electron_e1x5OverE5x5;   //!
   TBranch        *b_electron_HoverE;   //!
   TBranch        *b_electron_fbrem;   //!
   TBranch        *b_electron_eOverP;   //!
   TBranch        *b_electron_InvEminusInvP;   //!
   TBranch        *b_electron_dxyVTX;   //!
   TBranch        *b_electron_dxyerrVTX;   //!
   TBranch        *b_electron_dzVTX;   //!
   TBranch        *b_electron_dzerrVTX;   //!
   TBranch        *b_electron_3DIPVTX;   //!
   TBranch        *b_electron_3DIPerrVTX;   //!
   TBranch        *b_electron_dxy;   //!
   TBranch        *b_electron_sigdxy;   //!
   TBranch        *b_electron_dz;   //!
   TBranch        *b_electron_dxyBS;   //!
   TBranch        *b_electron_dzBS;   //!
   TBranch        *b_electron_chIso03;   //!
   TBranch        *b_electron_nhIso03;   //!
   TBranch        *b_electron_phIso03;   //!
   TBranch        *b_electron_puChIso03;   //!
   TBranch        *b_electron_passConversionVeto;   //!
   TBranch        *b_electron_isGsfCtfScPixChargeConsistent;   //!
   TBranch        *b_electron_isGsfScPixChargeConsistent;   //!
   TBranch        *b_electron_isGsfCtfChargeConsistent;   //!
   TBranch        *b_electron_mHits;   //!
   TBranch        *b_electron_ecalDriven;   //!
   TBranch        *b_electron_r9;   //!
   TBranch        *b_electron_scEnergy;   //!
   TBranch        *b_electron_scPreEnergy;   //!
   TBranch        *b_electron_scRawEnergy;   //!
   TBranch        *b_electron_scEt;   //!
   TBranch        *b_electron_E15;   //!
   TBranch        *b_electron_E25;   //!
   TBranch        *b_electron_E55;   //!
   TBranch        *b_electron_RelPFIso_dBeta;   //!
   TBranch        *b_electron_RelPFIso_Rho;   //!
   TBranch        *b_electron_IDBit;   //!
   TBranch        *b_electron_IDCutBit;   //!
   TBranch        *b_electron_EnergyUnCorr;   //!
   TBranch        *b_electron_chMiniIso;   //!
   TBranch        *b_electron_nhMiniIso;   //!
   TBranch        *b_electron_phMiniIso;   //!
   TBranch        *b_electron_puChMiniIso;   //!
   TBranch        *b_electron_trackIso;   //!
   TBranch        *b_electron_dr03EcalRecHitSumEt;   //!
   TBranch        *b_electron_dr03HcalDepth1TowerSumEt;   //!
   TBranch        *b_electron_dr03HcalTowerSumEt;   //!
   TBranch        *b_electron_dr03TkSumPt;   //!
   TBranch        *b_electron_ecalPFClusterIso;   //!
   TBranch        *b_electron_hcalPFClusterIso;   //!
   TBranch        *b_muon_PfChargedHadronIsoR04;   //!
   TBranch        *b_muon_PfNeutralHadronIsoR04;   //!
   TBranch        *b_muon_PfGammaIsoR04;   //!
   TBranch        *b_muon_PFSumPUIsoR04;   //!
   TBranch        *b_muon_PfChargedHadronIsoR03;   //!
   TBranch        *b_muon_PfNeutralHadronIsoR03;   //!
   TBranch        *b_muon_PfGammaIsoR03;   //!
   TBranch        *b_muon_PFSumPUIsoR03;   //!
   TBranch        *b_muon_TypeBit;   //!
   TBranch        *b_muon_IDBit;   //!
   TBranch        *b_muon_ishighpt;   //!
   TBranch        *b_muon_dB;   //!
   TBranch        *b_muon_phi;   //!
   TBranch        *b_muon_eta;   //!
   TBranch        *b_muon_pt;   //!
   TBranch        *b_muon_mass;   //!
   TBranch        *b_muon_trkiso;   //!
   TBranch        *b_muon_hcaliso;   //!
   TBranch        *b_muon_ecaliso;   //!
   TBranch        *b_muon_trkisoR05;   //!
   TBranch        *b_muon_hcalisoR05;   //!
   TBranch        *b_muon_ecalisoR05;   //!
   TBranch        *b_muon_charge;   //!
   TBranch        *b_muon_nChambers;   //!
   TBranch        *b_muon_matchedstations;   //!
   TBranch        *b_muon_stationMask;   //!
   TBranch        *b_muon_nSegments;   //!
   TBranch        *b_muon_normchi;   //!
   TBranch        *b_muon_validhits;   //!
   TBranch        *b_muon_trackerHits;   //!
   TBranch        *b_muon_pixelHits;   //!
   TBranch        *b_muon_validmuonhits;   //!
   TBranch        *b_muon_trackerLayers;   //!
   TBranch        *b_muon_qoverp;   //!
   TBranch        *b_muon_theta;   //!
   TBranch        *b_muon_lambda;   //!
   TBranch        *b_muon_dxy;   //!
   TBranch        *b_muon_d0;   //!
   TBranch        *b_muon_dsz;   //!
   TBranch        *b_muon_dz;   //!
   TBranch        *b_muon_dxyBS;   //!
   TBranch        *b_muon_dzBS;   //!
   TBranch        *b_muon_dszBS;   //!
   TBranch        *b_muon_dxyVTX;   //!
   TBranch        *b_muon_dxyerrVTX;   //!
   TBranch        *b_muon_dzVTX;   //!
   TBranch        *b_muon_dzerrVTX;   //!
   TBranch        *b_muon_3DIPVTX;   //!
   TBranch        *b_muon_3DIPerrVTX;   //!
   TBranch        *b_muon_dszVTX;   //!
   TBranch        *b_muon_vx;   //!
   TBranch        *b_muon_vy;   //!
   TBranch        *b_muon_vz;   //!
   TBranch        *b_muon_Best_pt;   //!
   TBranch        *b_muon_Best_ptError;   //!
   TBranch        *b_muon_Best_eta;   //!
   TBranch        *b_muon_Best_phi;   //!
   TBranch        *b_muon_Inner_pt;   //!
   TBranch        *b_muon_Inner_ptError;   //!
   TBranch        *b_muon_Inner_eta;   //!
   TBranch        *b_muon_Inner_phi;   //!
   TBranch        *b_muon_Outer_pt;   //!
   TBranch        *b_muon_Outer_ptError;   //!
   TBranch        *b_muon_Outer_eta;   //!
   TBranch        *b_muon_Outer_phi;   //!
   TBranch        *b_muon_GLB_pt;   //!
   TBranch        *b_muon_GLB_ptError;   //!
   TBranch        *b_muon_GLB_eta;   //!
   TBranch        *b_muon_GLB_phi;   //!
   TBranch        *b_muon_TuneP_pt;   //!
   TBranch        *b_muon_TuneP_ptError;   //!
   TBranch        *b_muon_TuneP_eta;   //!
   TBranch        *b_muon_TuneP_phi;   //!
   TBranch        *b_muon_TuneP_charge;   //!
   TBranch        *b_muon_roch_sf;   //!
   TBranch        *b_muon_roch_sf_up;   //!
   TBranch        *b_muon_PfChargedHadronMiniIso;   //!
   TBranch        *b_muon_PfNeutralHadronMiniIso;   //!
   TBranch        *b_muon_PfGammaMiniIso;   //!
   TBranch        *b_muon_PFSumPUMiniIso;   //!
   TBranch        *b_muon_MVA;   //!
   TBranch        *b_muon_lowptMVA;   //!
   TBranch        *b_muon_softMVA;   //!
   TBranch        *b_muon_jetPtRatio;   //!
   TBranch        *b_muon_jetPtRel;   //!
   TBranch        *b_muon_simType;   //!
   TBranch        *b_muon_simExtType;   //!
   TBranch        *b_muon_simFlavour;   //!
   TBranch        *b_muon_simHeaviestMotherFlavour;   //!
   TBranch        *b_muon_simPdgId;   //!
   TBranch        *b_muon_simMotherPdgId;   //!
   TBranch        *b_muon_simMatchQuality;   //!
   TBranch        *b_LHE_Px;   //!
   TBranch        *b_LHE_Py;   //!
   TBranch        *b_LHE_Pz;   //!
   TBranch        *b_LHE_E;   //!
   TBranch        *b_LHE_Status;   //!
   TBranch        *b_LHE_ID;   //!
   TBranch        *b_PDFWeights_Scale;   //!
   TBranch        *b_PDFWeights_Error;   //!
   TBranch        *b_PDFWeights_AlphaS;   //!
   TBranch        *b_gen_phi;   //!
   TBranch        *b_gen_eta;   //!
   TBranch        *b_gen_pt;   //!
   TBranch        *b_gen_mass;   //!
   TBranch        *b_gen_charge;   //!
   TBranch        *b_gen_mother_index;   //!
   TBranch        *b_gen_status;   //!
   TBranch        *b_gen_PID;   //!
   TBranch        *b_gen_isPrompt;   //!
   TBranch        *b_gen_isPromptFinalState;   //!
   TBranch        *b_gen_isTauDecayProduct;   //!
   TBranch        *b_gen_isPromptTauDecayProduct;   //!
   TBranch        *b_gen_isDirectPromptTauDecayProductFinalState;   //!
   TBranch        *b_gen_isHardProcess;   //!
   TBranch        *b_gen_isLastCopy;   //!
   TBranch        *b_gen_isLastCopyBeforeFSR;   //!
   TBranch        *b_gen_isPromptDecayed;   //!
   TBranch        *b_gen_isDecayedLeptonHadron;   //!
   TBranch        *b_gen_fromHardProcessBeforeFSR;   //!
   TBranch        *b_gen_fromHardProcessDecayed;   //!
   TBranch        *b_gen_fromHardProcessFinalState;   //!
   TBranch        *b_gen_isMostlyLikePythia6Status3;   //!
   TBranch        *b_gen_weight;   //!
   TBranch        *b_genWeight_Q;   //!
   TBranch        *b_genWeight_X1;   //!
   TBranch        *b_genWeight_X2;   //!
   TBranch        *b_genWeight_id1;   //!
   TBranch        *b_genWeight_id2;   //!
   TBranch        *b_genWeight_alphaQCD;   //!
   TBranch        *b_genWeight_alphaQED;   //!
   TBranch        *b_photon_Energy;   //!
   TBranch        *b_photon_EnergyUnCorr;   //!
   TBranch        *b_photon_eta;   //!
   TBranch        *b_photon_phi;   //!
   TBranch        *b_photon_scEta;   //!
   TBranch        *b_photon_scPhi;   //!
   TBranch        *b_photon_HoverE;   //!
   TBranch        *b_photon_hasPixelSeed;   //!
   TBranch        *b_photon_Full5x5_SigmaIEtaIEta;   //!
   TBranch        *b_photon_ChIso;   //!
   TBranch        *b_photon_NhIso;   //!
   TBranch        *b_photon_PhIso;   //!
   TBranch        *b_photon_ChIsoWithEA;   //!
   TBranch        *b_photon_NhIsoWithEA;   //!
   TBranch        *b_photon_PhIsoWithEA;   //!
   TBranch        *b_photon_passMVAID_WP80;   //!
   TBranch        *b_photon_passMVAID_WP90;   //!
   TBranch        *b_photon_passLooseID;   //!
   TBranch        *b_photon_passMediumID;   //!
   TBranch        *b_photon_passTightID;   //!
   TBranch        *b_nPileUp;   //!
   TBranch        *b_pfMET_pt;   //!
   TBranch        *b_pfMET_phi;   //!
   TBranch        *b_pfMET_SumEt;   //!
   TBranch        *b_pfMET_Type1_pt;   //!
   TBranch        *b_pfMET_Type1_phi;   //!
   TBranch        *b_pfMET_Type1_SumEt;   //!
   TBranch        *b_pfMET_Type1_PhiCor_pt;   //!
   TBranch        *b_pfMET_Type1_PhiCor_phi;   //!
   TBranch        *b_pfMET_Type1_PhiCor_SumEt;   //!
   TBranch        *b_pfMET_pt_shifts;   //!
   TBranch        *b_pfMET_phi_shifts;   //!
   TBranch        *b_pfMET_SumEt_shifts;   //!
   TBranch        *b_pfMET_Type1_pt_shifts;   //!
   TBranch        *b_pfMET_Type1_phi_shifts;   //!
   TBranch        *b_pfMET_Type1_SumEt_shifts;   //!
   TBranch        *b_pfMET_Type1_PhiCor_pt_shifts;   //!
   TBranch        *b_pfMET_Type1_PhiCor_phi_shifts;   //!
   TBranch        *b_pfMET_Type1_PhiCor_SumEt_shifts;   //!

   myclass();
   virtual ~myclass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init();
   virtual bool     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual void SetTreeName(TString tname){
     fChain = new TChain(tname);
   }

   virtual void AddFile(TString filename){
     fChain->Add(filename);
   }

};

#endif

#ifdef myclass_cxx
myclass::myclass()
{

}

myclass::~myclass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t myclass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t myclass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void myclass::Init()
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   HLT_TriggerName = 0;
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_charge = 0;
   jet_area = 0;
   jet_partonFlavour = 0;
   jet_hadronFlavour = 0;
   jet_CSVv2 = 0;
   jet_DeepCSV = 0;
   jet_CvsL = 0;
   jet_CvsB = 0;
   jet_DeepFlavour_b = 0;
   jet_DeepFlavour_bb = 0;
   jet_DeepFlavour_lepb = 0;
   jet_DeepFlavour_c = 0;
   jet_DeepFlavour_uds = 0;
   jet_DeepFlavour_g = 0;
   jet_DeepCvsL = 0;
   jet_DeepCvsB = 0;
   jet_chargedHadronEnergyFraction = 0;
   jet_neutralHadronEnergyFraction = 0;
   jet_neutralEmEnergyFraction = 0;
   jet_chargedEmEnergyFraction = 0;
   jet_muonEnergyFraction = 0;
   jet_chargedMultiplicity = 0;
   jet_neutralMultiplicity = 0;
   jet_tightJetID = 0;
   jet_tightLepVetoJetID = 0;
   jet_partonPdgId = 0;
   jet_vtxNtracks = 0;
   jet_m = 0;
   jet_energy = 0;
   jet_PileupJetId = 0;
   jet_shiftedEnUp = 0;
   jet_shiftedEnDown = 0;
   jet_smearedRes = 0;
   jet_smearedResUp = 0;
   jet_smearedResDown = 0;
   jet_JECL1FastJet = 0;
   jet_JECFull = 0;
   fatjet_pt = 0;
   fatjet_eta = 0;
   fatjet_phi = 0;
   fatjet_charge = 0;
   fatjet_area = 0;
   fatjet_partonFlavour = 0;
   fatjet_hadronFlavour = 0;
   fatjet_CSVv2 = 0;
   fatjet_DeepCSV = 0;
   fatjet_DeepFlavour_b = 0;
   fatjet_DeepFlavour_bb = 0;
   fatjet_DeepFlavour_lepb = 0;
   fatjet_DeepFlavour_c = 0;
   fatjet_DeepFlavour_uds = 0;
   fatjet_DeepFlavour_g = 0;
   fatjet_CvsL = 0;
   fatjet_CvsB = 0;
   fatjet_DeepCvsL = 0;
   fatjet_DeepCvsB = 0;
   fatjet_tightJetID = 0;
   fatjet_tightLepVetoJetID = 0;
   fatjet_partonPdgId = 0;
   fatjet_vtxNtracks = 0;
   fatjet_m = 0;
   fatjet_energy = 0;
   fatjet_puppi_tau1 = 0;
   fatjet_puppi_tau2 = 0;
   fatjet_puppi_tau3 = 0;
   fatjet_puppi_tau4 = 0;
   fatjet_softdropmass = 0;
   fatjet_chargedHadronEnergyFraction = 0;
   fatjet_neutralHadronEnergyFraction = 0;
   fatjet_neutralEmEnergyFraction = 0;
   fatjet_chargedEmEnergyFraction = 0;
   fatjet_muonEnergyFraction = 0;
   fatjet_chargedMultiplicity = 0;
   fatjet_neutralMultiplicity = 0;
   fatjet_shiftedEnUp = 0;
   fatjet_shiftedEnDown = 0;
   fatjet_smearedRes = 0;
   fatjet_smearedResUp = 0;
   fatjet_smearedResDown = 0;
   fatjet_LSF = 0;
   fatjet_LSFlep_PID = 0;
   fatjet_LSFlep_Pt = 0;
   fatjet_LSFlep_Eta = 0;
   fatjet_LSFlep_Phi = 0;
   electron_MVAIso = 0;
   electron_MVANoIso = 0;
   electron_Energy = 0;
   electron_Energy_Scale_Up = 0;
   electron_Energy_Scale_Down = 0;
   electron_Energy_Smear_Up = 0;
   electron_Energy_Smear_Down = 0;
   electron_eta = 0;
   electron_phi = 0;
   electron_charge = 0;
   electron_gsfpt = 0;
   electron_gsfEta = 0;
   electron_gsfPhi = 0;
   electron_gsfCharge = 0;
   electron_scEta = 0;
   electron_scPhi = 0;
   electron_etaWidth = 0;
   electron_phiWidth = 0;
   electron_dEtaIn = 0;
   electron_dEtaInSeed = 0;
   electron_dPhiIn = 0;
   electron_sigmaIEtaIEta = 0;
   electron_Full5x5_SigmaIEtaIEta = 0;
   electron_e2x5OverE5x5 = 0;
   electron_e1x5OverE5x5 = 0;
   electron_HoverE = 0;
   electron_fbrem = 0;
   electron_eOverP = 0;
   electron_InvEminusInvP = 0;
   electron_dxyVTX = 0;
   electron_dxyerrVTX = 0;
   electron_dzVTX = 0;
   electron_dzerrVTX = 0;
   electron_3DIPVTX = 0;
   electron_3DIPerrVTX = 0;
   electron_dxy = 0;
   electron_sigdxy = 0;
   electron_dz = 0;
   electron_dxyBS = 0;
   electron_dzBS = 0;
   electron_chIso03 = 0;
   electron_nhIso03 = 0;
   electron_phIso03 = 0;
   electron_puChIso03 = 0;
   electron_passConversionVeto = 0;
   electron_isGsfCtfScPixChargeConsistent = 0;
   electron_isGsfScPixChargeConsistent = 0;
   electron_isGsfCtfChargeConsistent = 0;
   electron_mHits = 0;
   electron_ecalDriven = 0;
   electron_r9 = 0;
   electron_scEnergy = 0;
   electron_scPreEnergy = 0;
   electron_scRawEnergy = 0;
   electron_scEt = 0;
   electron_E15 = 0;
   electron_E25 = 0;
   electron_E55 = 0;
   electron_RelPFIso_dBeta = 0;
   electron_RelPFIso_Rho = 0;
   electron_IDBit = 0;
   electron_IDCutBit = 0;
   electron_EnergyUnCorr = 0;
   electron_chMiniIso = 0;
   electron_nhMiniIso = 0;
   electron_phMiniIso = 0;
   electron_puChMiniIso = 0;
   electron_trackIso = 0;
   electron_dr03EcalRecHitSumEt = 0;
   electron_dr03HcalDepth1TowerSumEt = 0;
   electron_dr03HcalTowerSumEt = 0;
   electron_dr03TkSumPt = 0;
   electron_ecalPFClusterIso = 0;
   electron_hcalPFClusterIso = 0;
   muon_PfChargedHadronIsoR04 = 0;
   muon_PfNeutralHadronIsoR04 = 0;
   muon_PfGammaIsoR04 = 0;
   muon_PFSumPUIsoR04 = 0;
   muon_PfChargedHadronIsoR03 = 0;
   muon_PfNeutralHadronIsoR03 = 0;
   muon_PfGammaIsoR03 = 0;
   muon_PFSumPUIsoR03 = 0;
   muon_TypeBit = 0;
   muon_IDBit = 0;
   muon_ishighpt = 0;
   muon_dB = 0;
   muon_phi = 0;
   muon_eta = 0;
   muon_pt = 0;
   muon_mass = 0;
   muon_trkiso = 0;
   muon_hcaliso = 0;
   muon_ecaliso = 0;
   muon_trkisoR05 = 0;
   muon_hcalisoR05 = 0;
   muon_ecalisoR05 = 0;
   muon_charge = 0;
   muon_nChambers = 0;
   muon_matchedstations = 0;
   muon_stationMask = 0;
   muon_nSegments = 0;
   muon_normchi = 0;
   muon_validhits = 0;
   muon_trackerHits = 0;
   muon_pixelHits = 0;
   muon_validmuonhits = 0;
   muon_trackerLayers = 0;
   muon_qoverp = 0;
   muon_theta = 0;
   muon_lambda = 0;
   muon_dxy = 0;
   muon_d0 = 0;
   muon_dsz = 0;
   muon_dz = 0;
   muon_dxyBS = 0;
   muon_dzBS = 0;
   muon_dszBS = 0;
   muon_dxyVTX = 0;
   muon_dxyerrVTX = 0;
   muon_dzVTX = 0;
   muon_dzerrVTX = 0;
   muon_3DIPVTX = 0;
   muon_3DIPerrVTX = 0;
   muon_dszVTX = 0;
   muon_vx = 0;
   muon_vy = 0;
   muon_vz = 0;
   muon_Best_pt = 0;
   muon_Best_ptError = 0;
   muon_Best_eta = 0;
   muon_Best_phi = 0;
   muon_Inner_pt = 0;
   muon_Inner_ptError = 0;
   muon_Inner_eta = 0;
   muon_Inner_phi = 0;
   muon_Outer_pt = 0;
   muon_Outer_ptError = 0;
   muon_Outer_eta = 0;
   muon_Outer_phi = 0;
   muon_GLB_pt = 0;
   muon_GLB_ptError = 0;
   muon_GLB_eta = 0;
   muon_GLB_phi = 0;
   muon_TuneP_pt = 0;
   muon_TuneP_ptError = 0;
   muon_TuneP_eta = 0;
   muon_TuneP_phi = 0;
   muon_TuneP_charge = 0;
   muon_roch_sf = 0;
   muon_roch_sf_up = 0;
   muon_PfChargedHadronMiniIso = 0;
   muon_PfNeutralHadronMiniIso = 0;
   muon_PfGammaMiniIso = 0;
   muon_PFSumPUMiniIso = 0;
   muon_MVA = 0;
   muon_lowptMVA = 0;
   muon_softMVA = 0;
   muon_jetPtRatio = 0;
   muon_jetPtRel = 0;
   muon_simType = 0;
   muon_simExtType = 0;
   muon_simFlavour = 0;
   muon_simHeaviestMotherFlavour = 0;
   muon_simPdgId = 0;
   muon_simMotherPdgId = 0;
   muon_simMatchQuality = 0;
   LHE_Px = 0;
   LHE_Py = 0;
   LHE_Pz = 0;
   LHE_E = 0;
   LHE_Status = 0;
   LHE_ID = 0;
   PDFWeights_Scale = 0;
   PDFWeights_Error = 0;
   PDFWeights_AlphaS = 0;
   gen_phi = 0;
   gen_eta = 0;
   gen_pt = 0;
   gen_mass = 0;
   gen_charge = 0;
   gen_mother_index = 0;
   gen_status = 0;
   gen_PID = 0;
   gen_isPrompt = 0;
   gen_isPromptFinalState = 0;
   gen_isTauDecayProduct = 0;
   gen_isPromptTauDecayProduct = 0;
   gen_isDirectPromptTauDecayProductFinalState = 0;
   gen_isHardProcess = 0;
   gen_isLastCopy = 0;
   gen_isLastCopyBeforeFSR = 0;
   gen_isPromptDecayed = 0;
   gen_isDecayedLeptonHadron = 0;
   gen_fromHardProcessBeforeFSR = 0;
   gen_fromHardProcessDecayed = 0;
   gen_fromHardProcessFinalState = 0;
   gen_isMostlyLikePythia6Status3 = 0;
   photon_Energy = 0;
   photon_EnergyUnCorr = 0;
   photon_eta = 0;
   photon_phi = 0;
   photon_scEta = 0;
   photon_scPhi = 0;
   photon_HoverE = 0;
   photon_hasPixelSeed = 0;
   photon_Full5x5_SigmaIEtaIEta = 0;
   photon_ChIso = 0;
   photon_NhIso = 0;
   photon_PhIso = 0;
   photon_ChIsoWithEA = 0;
   photon_NhIsoWithEA = 0;
   photon_PhIsoWithEA = 0;
   photon_passMVAID_WP80 = 0;
   photon_passMVAID_WP90 = 0;
   photon_passLooseID = 0;
   photon_passMediumID = 0;
   photon_passTightID = 0;
   pfMET_pt_shifts = 0;
   pfMET_phi_shifts = 0;
   pfMET_SumEt_shifts = 0;
   pfMET_Type1_pt_shifts = 0;
   pfMET_Type1_phi_shifts = 0;
   pfMET_Type1_SumEt_shifts = 0;
   pfMET_Type1_PhiCor_pt_shifts = 0;
   pfMET_Type1_PhiCor_phi_shifts = 0;
   pfMET_Type1_PhiCor_SumEt_shifts = 0;

   fChain->SetBranchAddress("IsData", &IsData, &b_IsData);
   fChain->SetBranchAddress("nTotal", &nTotal, &b_nTotal);
   fChain->SetBranchAddress("run", &run, &b_runNum);
   fChain->SetBranchAddress("event", &event, &b_evtNum);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumiBlock);
   fChain->SetBranchAddress("Rho", &Rho, &b_Rho);
   fChain->SetBranchAddress("nPV", &nPV, &b_nPV);
   fChain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices, &b_Flag_goodVertices);
   fChain->SetBranchAddress("Flag_globalTightHalo2016Filter", &Flag_globalTightHalo2016Filter, &b_Flag_globalTightHalo2016Filter);
   fChain->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter, &b_Flag_globalSuperTightHalo2016Filter);
   fChain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter, &b_Flag_HBHENoiseFilter);
   fChain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter, &b_Flag_HBHENoiseIsoFilter);
   fChain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, &b_Flag_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter, &b_Flag_BadPFMuonFilter);
   fChain->SetBranchAddress("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter, &b_Flag_BadChargedCandidateFilter);
   fChain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter, &b_Flag_eeBadScFilter);
   fChain->SetBranchAddress("Flag_ecalBadCalibReducedMINIAODFilter", &Flag_ecalBadCalibReducedMINIAODFilter, &b_Flag_ecalBadCalibReducedMINIAODFilter);
   fChain->SetBranchAddress("PVtrackSize", &PVtrackSize, &b_PVtrackSize);
   fChain->SetBranchAddress("PVchi2", &PVchi2, &b_PVchi2);
   fChain->SetBranchAddress("PVndof", &PVndof, &b_PVndof);
   fChain->SetBranchAddress("PVnormalizedChi2", &PVnormalizedChi2, &b_PVnormalizedChi2);
   fChain->SetBranchAddress("vertex_X", &vertex_X, &b_PVx);
   fChain->SetBranchAddress("vertex_Y", &vertex_Y, &b_PVy);
   fChain->SetBranchAddress("vertex_Z", &vertex_Z, &b_PVz);
   fChain->SetBranchAddress("HLT_TriggerName", &HLT_TriggerName, &b_HLT_TriggerName);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_charge", &jet_charge, &b_jet_charge);
   fChain->SetBranchAddress("jet_area", &jet_area, &b_jet_area);
   fChain->SetBranchAddress("jet_partonFlavour", &jet_partonFlavour, &b_jet_partonFlavour);
   fChain->SetBranchAddress("jet_hadronFlavour", &jet_hadronFlavour, &b_jet_hadronFlavour);
   fChain->SetBranchAddress("jet_CSVv2", &jet_CSVv2, &b_jet_CSVv2);
   fChain->SetBranchAddress("jet_DeepCSV", &jet_DeepCSV, &b_jet_DeepCSV);
   fChain->SetBranchAddress("jet_CvsL", &jet_CvsL, &b_jet_CvsL);
   fChain->SetBranchAddress("jet_CvsB", &jet_CvsB, &b_jet_CvsB);
   fChain->SetBranchAddress("jet_DeepFlavour_b", &jet_DeepFlavour_b, &b_jet_DeepFlavour_b);
   fChain->SetBranchAddress("jet_DeepFlavour_bb", &jet_DeepFlavour_bb, &b_jet_DeepFlavour_bb);
   fChain->SetBranchAddress("jet_DeepFlavour_lepb", &jet_DeepFlavour_lepb, &b_jet_DeepFlavour_lepb);
   fChain->SetBranchAddress("jet_DeepFlavour_c", &jet_DeepFlavour_c, &b_jet_DeepFlavour_c);
   fChain->SetBranchAddress("jet_DeepFlavour_uds", &jet_DeepFlavour_uds, &b_jet_DeepFlavour_uds);
   fChain->SetBranchAddress("jet_DeepFlavour_g", &jet_DeepFlavour_g, &b_jet_DeepFlavour_g);
   fChain->SetBranchAddress("jet_DeepCvsL", &jet_DeepCvsL, &b_jet_DeepCvsL);
   fChain->SetBranchAddress("jet_DeepCvsB", &jet_DeepCvsB, &b_jet_DeepCvsB);
   fChain->SetBranchAddress("jet_chargedHadronEnergyFraction", &jet_chargedHadronEnergyFraction, &b_jet_chargedHadronEnergyFraction);
   fChain->SetBranchAddress("jet_neutralHadronEnergyFraction", &jet_neutralHadronEnergyFraction, &b_jet_neutralHadronEnergyFraction);
   fChain->SetBranchAddress("jet_neutralEmEnergyFraction", &jet_neutralEmEnergyFraction, &b_jet_neutralEmEnergyFraction);
   fChain->SetBranchAddress("jet_chargedEmEnergyFraction", &jet_chargedEmEnergyFraction, &b_jet_chargedEmEnergyFraction);
   fChain->SetBranchAddress("jet_muonEnergyFraction", &jet_muonEnergyFraction, &b_jet_muonEnergyFraction);
   fChain->SetBranchAddress("jet_chargedMultiplicity", &jet_chargedMultiplicity, &b_jet_chargedMultiplicity);
   fChain->SetBranchAddress("jet_neutralMultiplicity", &jet_neutralMultiplicity, &b_jet_neutralMultiplicity);
   fChain->SetBranchAddress("jet_tightJetID", &jet_tightJetID, &b_jet_tightJetID);
   fChain->SetBranchAddress("jet_tightLepVetoJetID", &jet_tightLepVetoJetID, &b_jet_tightLepVetoJetID);
   fChain->SetBranchAddress("jet_partonPdgId", &jet_partonPdgId, &b_jet_partonPdgId);
   fChain->SetBranchAddress("jet_vtxNtracks", &jet_vtxNtracks, &b_jet_vtxNtracks);
   fChain->SetBranchAddress("jet_m", &jet_m, &b_jet_m);
   fChain->SetBranchAddress("jet_energy", &jet_energy, &b_jet_energy);
   fChain->SetBranchAddress("jet_PileupJetId", &jet_PileupJetId, &b_jet_PileupJetId);
   fChain->SetBranchAddress("jet_shiftedEnUp", &jet_shiftedEnUp, &b_jet_shiftedEnUp);
   fChain->SetBranchAddress("jet_shiftedEnDown", &jet_shiftedEnDown, &b_jet_shiftedEnDown);
   fChain->SetBranchAddress("jet_smearedRes", &jet_smearedRes, &b_jet_smearedRes);
   fChain->SetBranchAddress("jet_smearedResUp", &jet_smearedResUp, &b_jet_smearedResUp);
   fChain->SetBranchAddress("jet_smearedResDown", &jet_smearedResDown, &b_jet_smearedResDown);
   fChain->SetBranchAddress("jet_JECL1FastJet", &jet_JECL1FastJet, &b_jet_JECL1FastJet);
   fChain->SetBranchAddress("jet_JECFull", &jet_JECFull, &b_jet_JECFull);
   fChain->SetBranchAddress("fatjet_pt", &fatjet_pt, &b_fatjet_pt);
   fChain->SetBranchAddress("fatjet_eta", &fatjet_eta, &b_fatjet_eta);
   fChain->SetBranchAddress("fatjet_phi", &fatjet_phi, &b_fatjet_phi);
   fChain->SetBranchAddress("fatjet_charge", &fatjet_charge, &b_fatjet_charge);
   fChain->SetBranchAddress("fatjet_area", &fatjet_area, &b_fatjet_area);
   fChain->SetBranchAddress("fatjet_partonFlavour", &fatjet_partonFlavour, &b_fatjet_partonFlavour);
   fChain->SetBranchAddress("fatjet_hadronFlavour", &fatjet_hadronFlavour, &b_fatjet_hadronFlavour);
   fChain->SetBranchAddress("fatjet_CSVv2", &fatjet_CSVv2, &b_fatjet_CSVv2);
   fChain->SetBranchAddress("fatjet_DeepCSV", &fatjet_DeepCSV, &b_fatjet_DeepCSV);
   fChain->SetBranchAddress("fatjet_DeepFlavour_b", &fatjet_DeepFlavour_b, &b_fatjet_DeepFlavour_b);
   fChain->SetBranchAddress("fatjet_DeepFlavour_bb", &fatjet_DeepFlavour_bb, &b_fatjet_DeepFlavour_bb);
   fChain->SetBranchAddress("fatjet_DeepFlavour_lepb", &fatjet_DeepFlavour_lepb, &b_fatjet_DeepFlavour_lepb);
   fChain->SetBranchAddress("fatjet_DeepFlavour_c", &fatjet_DeepFlavour_c, &b_fatjet_DeepFlavour_c);
   fChain->SetBranchAddress("fatjet_DeepFlavour_uds", &fatjet_DeepFlavour_uds, &b_fatjet_DeepFlavour_uds);
   fChain->SetBranchAddress("fatjet_DeepFlavour_g", &fatjet_DeepFlavour_g, &b_fatjet_DeepFlavour_g);
   fChain->SetBranchAddress("fatjet_CvsL", &fatjet_CvsL, &b_fatjet_CvsL);
   fChain->SetBranchAddress("fatjet_CvsB", &fatjet_CvsB, &b_fatjet_CvsB);
   fChain->SetBranchAddress("fatjet_DeepCvsL", &fatjet_DeepCvsL, &b_fatjet_DeepCvsL);
   fChain->SetBranchAddress("fatjet_DeepCvsB", &fatjet_DeepCvsB, &b_fatjet_DeepCvsB);
   fChain->SetBranchAddress("fatjet_tightJetID", &fatjet_tightJetID, &b_fatjet_tightJetID);
   fChain->SetBranchAddress("fatjet_tightLepVetoJetID", &fatjet_tightLepVetoJetID, &b_fatjet_tightLepVetoJetID);
   fChain->SetBranchAddress("fatjet_partonPdgId", &fatjet_partonPdgId, &b_fatjet_partonPdgId);
   fChain->SetBranchAddress("fatjet_vtxNtracks", &fatjet_vtxNtracks, &b_fatjet_vtxNtracks);
   fChain->SetBranchAddress("fatjet_m", &fatjet_m, &b_fatjet_m);
   fChain->SetBranchAddress("fatjet_energy", &fatjet_energy, &b_fatjet_energy);
   fChain->SetBranchAddress("fatjet_puppi_tau1", &fatjet_puppi_tau1, &b_fatjet_puppi_tau1);
   fChain->SetBranchAddress("fatjet_puppi_tau2", &fatjet_puppi_tau2, &b_fatjet_puppi_tau2);
   fChain->SetBranchAddress("fatjet_puppi_tau3", &fatjet_puppi_tau3, &b_fatjet_puppi_tau3);
   fChain->SetBranchAddress("fatjet_puppi_tau4", &fatjet_puppi_tau4, &b_fatjet_puppi_tau4);
   fChain->SetBranchAddress("fatjet_softdropmass", &fatjet_softdropmass, &b_fatjet_softdropmass);
   fChain->SetBranchAddress("fatjet_chargedHadronEnergyFraction", &fatjet_chargedHadronEnergyFraction, &b_fatjet_chargedHadronEnergyFraction);
   fChain->SetBranchAddress("fatjet_neutralHadronEnergyFraction", &fatjet_neutralHadronEnergyFraction, &b_fatjet_neutralHadronEnergyFraction);
   fChain->SetBranchAddress("fatjet_neutralEmEnergyFraction", &fatjet_neutralEmEnergyFraction, &b_fatjet_neutralEmEnergyFraction);
   fChain->SetBranchAddress("fatjet_chargedEmEnergyFraction", &fatjet_chargedEmEnergyFraction, &b_fatjet_chargedEmEnergyFraction);
   fChain->SetBranchAddress("fatjet_muonEnergyFraction", &fatjet_muonEnergyFraction, &b_fatjet_muonEnergyFraction);
   fChain->SetBranchAddress("fatjet_chargedMultiplicity", &fatjet_chargedMultiplicity, &b_fatjet_chargedMultiplicity);
   fChain->SetBranchAddress("fatjet_neutralMultiplicity", &fatjet_neutralMultiplicity, &b_fatjet_neutralMultiplicity);
   fChain->SetBranchAddress("fatjet_shiftedEnUp", &fatjet_shiftedEnUp, &b_fatjet_shiftedEnUp);
   fChain->SetBranchAddress("fatjet_shiftedEnDown", &fatjet_shiftedEnDown, &b_fatjet_shiftedEnDown);
   fChain->SetBranchAddress("fatjet_smearedRes", &fatjet_smearedRes, &b_fatjet_smearedRes);
   fChain->SetBranchAddress("fatjet_smearedResUp", &fatjet_smearedResUp, &b_fatjet_smearedResUp);
   fChain->SetBranchAddress("fatjet_smearedResDown", &fatjet_smearedResDown, &b_fatjet_smearedResDown);
   fChain->SetBranchAddress("fatjet_LSF", &fatjet_LSF, &b_fatjet_LSF);
   fChain->SetBranchAddress("fatjet_LSFlep_PID", &fatjet_LSFlep_PID, &b_fatjet_LSFlep_PID);
   fChain->SetBranchAddress("fatjet_LSFlep_Pt", &fatjet_LSFlep_Pt, &b_fatjet_LSFlep_Pt);
   fChain->SetBranchAddress("fatjet_LSFlep_Eta", &fatjet_LSFlep_Eta, &b_fatjet_LSFlep_Eta);
   fChain->SetBranchAddress("fatjet_LSFlep_Phi", &fatjet_LSFlep_Phi, &b_fatjet_LSFlep_Phi);
   fChain->SetBranchAddress("electron_MVAIso", &electron_MVAIso, &b_electron_MVAIso);
   fChain->SetBranchAddress("electron_MVANoIso", &electron_MVANoIso, &b_electron_MVANoIso);
   fChain->SetBranchAddress("electron_Energy", &electron_Energy, &b_electron_Energy);
   fChain->SetBranchAddress("electron_Energy_Scale_Up", &electron_Energy_Scale_Up, &b_electron_Energy_Scale_Up);
   fChain->SetBranchAddress("electron_Energy_Scale_Down", &electron_Energy_Scale_Down, &b_electron_Energy_Scale_Down);
   fChain->SetBranchAddress("electron_Energy_Smear_Up", &electron_Energy_Smear_Up, &b_electron_Energy_Smear_Up);
   fChain->SetBranchAddress("electron_Energy_Smear_Down", &electron_Energy_Smear_Down, &b_electron_Energy_Smear_Down);
   fChain->SetBranchAddress("electron_eta", &electron_eta, &b_electron_eta);
   fChain->SetBranchAddress("electron_phi", &electron_phi, &b_electron_phi);
   fChain->SetBranchAddress("electron_charge", &electron_charge, &b_electron_charge);
   fChain->SetBranchAddress("electron_gsfpt", &electron_gsfpt, &b_electron_gsfpt);
   fChain->SetBranchAddress("electron_gsfEta", &electron_gsfEta, &b_electron_gsfEta);
   fChain->SetBranchAddress("electron_gsfPhi", &electron_gsfPhi, &b_electron_gsfPhi);
   fChain->SetBranchAddress("electron_gsfCharge", &electron_gsfCharge, &b_electron_gsfCharge);
   fChain->SetBranchAddress("electron_scEta", &electron_scEta, &b_electron_scEta);
   fChain->SetBranchAddress("electron_scPhi", &electron_scPhi, &b_electron_scPhi);
   fChain->SetBranchAddress("electron_etaWidth", &electron_etaWidth, &b_electron_etaWidth);
   fChain->SetBranchAddress("electron_phiWidth", &electron_phiWidth, &b_electron_phiWidth);
   fChain->SetBranchAddress("electron_dEtaIn", &electron_dEtaIn, &b_electron_dEtaIn);
   fChain->SetBranchAddress("electron_dEtaInSeed", &electron_dEtaInSeed, &b_electron_dEtaInSeed);
   fChain->SetBranchAddress("electron_dPhiIn", &electron_dPhiIn, &b_electron_dPhiIn);
   fChain->SetBranchAddress("electron_sigmaIEtaIEta", &electron_sigmaIEtaIEta, &b_electron_sigmaIEtaIEta);
   fChain->SetBranchAddress("electron_Full5x5_SigmaIEtaIEta", &electron_Full5x5_SigmaIEtaIEta, &b_electron_Full5x5_SigmaIEtaIEta);
   fChain->SetBranchAddress("electron_e2x5OverE5x5", &electron_e2x5OverE5x5, &b_electron_e2x5OverE5x5);
   fChain->SetBranchAddress("electron_e1x5OverE5x5", &electron_e1x5OverE5x5, &b_electron_e1x5OverE5x5);
   fChain->SetBranchAddress("electron_HoverE", &electron_HoverE, &b_electron_HoverE);
   fChain->SetBranchAddress("electron_fbrem", &electron_fbrem, &b_electron_fbrem);
   fChain->SetBranchAddress("electron_eOverP", &electron_eOverP, &b_electron_eOverP);
   fChain->SetBranchAddress("electron_InvEminusInvP", &electron_InvEminusInvP, &b_electron_InvEminusInvP);
   fChain->SetBranchAddress("electron_dxyVTX", &electron_dxyVTX, &b_electron_dxyVTX);
   fChain->SetBranchAddress("electron_dxyerrVTX", &electron_dxyerrVTX, &b_electron_dxyerrVTX);
   fChain->SetBranchAddress("electron_dzVTX", &electron_dzVTX, &b_electron_dzVTX);
   fChain->SetBranchAddress("electron_dzerrVTX", &electron_dzerrVTX, &b_electron_dzerrVTX);
   fChain->SetBranchAddress("electron_3DIPVTX", &electron_3DIPVTX, &b_electron_3DIPVTX);
   fChain->SetBranchAddress("electron_3DIPerrVTX", &electron_3DIPerrVTX, &b_electron_3DIPerrVTX);
   fChain->SetBranchAddress("electron_dxy", &electron_dxy, &b_electron_dxy);
   fChain->SetBranchAddress("electron_sigdxy", &electron_sigdxy, &b_electron_sigdxy);
   fChain->SetBranchAddress("electron_dz", &electron_dz, &b_electron_dz);
   fChain->SetBranchAddress("electron_dxyBS", &electron_dxyBS, &b_electron_dxyBS);
   fChain->SetBranchAddress("electron_dzBS", &electron_dzBS, &b_electron_dzBS);
   fChain->SetBranchAddress("electron_chIso03", &electron_chIso03, &b_electron_chIso03);
   fChain->SetBranchAddress("electron_nhIso03", &electron_nhIso03, &b_electron_nhIso03);
   fChain->SetBranchAddress("electron_phIso03", &electron_phIso03, &b_electron_phIso03);
   fChain->SetBranchAddress("electron_puChIso03", &electron_puChIso03, &b_electron_puChIso03);
   fChain->SetBranchAddress("electron_passConversionVeto", &electron_passConversionVeto, &b_electron_passConversionVeto);
   fChain->SetBranchAddress("electron_isGsfCtfScPixChargeConsistent", &electron_isGsfCtfScPixChargeConsistent, &b_electron_isGsfCtfScPixChargeConsistent);
   fChain->SetBranchAddress("electron_isGsfScPixChargeConsistent", &electron_isGsfScPixChargeConsistent, &b_electron_isGsfScPixChargeConsistent);
   fChain->SetBranchAddress("electron_isGsfCtfChargeConsistent", &electron_isGsfCtfChargeConsistent, &b_electron_isGsfCtfChargeConsistent);
   fChain->SetBranchAddress("electron_mHits", &electron_mHits, &b_electron_mHits);
   fChain->SetBranchAddress("electron_ecalDriven", &electron_ecalDriven, &b_electron_ecalDriven);
   fChain->SetBranchAddress("electron_r9", &electron_r9, &b_electron_r9);
   fChain->SetBranchAddress("electron_scEnergy", &electron_scEnergy, &b_electron_scEnergy);
   fChain->SetBranchAddress("electron_scPreEnergy", &electron_scPreEnergy, &b_electron_scPreEnergy);
   fChain->SetBranchAddress("electron_scRawEnergy", &electron_scRawEnergy, &b_electron_scRawEnergy);
   fChain->SetBranchAddress("electron_scEt", &electron_scEt, &b_electron_scEt);
   fChain->SetBranchAddress("electron_E15", &electron_E15, &b_electron_E15);
   fChain->SetBranchAddress("electron_E25", &electron_E25, &b_electron_E25);
   fChain->SetBranchAddress("electron_E55", &electron_E55, &b_electron_E55);
   fChain->SetBranchAddress("electron_RelPFIso_dBeta", &electron_RelPFIso_dBeta, &b_electron_RelPFIso_dBeta);
   fChain->SetBranchAddress("electron_RelPFIso_Rho", &electron_RelPFIso_Rho, &b_electron_RelPFIso_Rho);
   fChain->SetBranchAddress("electron_IDBit", &electron_IDBit, &b_electron_IDBit);
   fChain->SetBranchAddress("electron_IDCutBit", &electron_IDCutBit, &b_electron_IDCutBit);
   fChain->SetBranchAddress("electron_EnergyUnCorr", &electron_EnergyUnCorr, &b_electron_EnergyUnCorr);
   fChain->SetBranchAddress("electron_chMiniIso", &electron_chMiniIso, &b_electron_chMiniIso);
   fChain->SetBranchAddress("electron_nhMiniIso", &electron_nhMiniIso, &b_electron_nhMiniIso);
   fChain->SetBranchAddress("electron_phMiniIso", &electron_phMiniIso, &b_electron_phMiniIso);
   fChain->SetBranchAddress("electron_puChMiniIso", &electron_puChMiniIso, &b_electron_puChMiniIso);
   fChain->SetBranchAddress("electron_trackIso", &electron_trackIso, &b_electron_trackIso);
   fChain->SetBranchAddress("electron_dr03EcalRecHitSumEt", &electron_dr03EcalRecHitSumEt, &b_electron_dr03EcalRecHitSumEt);
   fChain->SetBranchAddress("electron_dr03HcalDepth1TowerSumEt", &electron_dr03HcalDepth1TowerSumEt, &b_electron_dr03HcalDepth1TowerSumEt);
   fChain->SetBranchAddress("electron_dr03HcalTowerSumEt", &electron_dr03HcalTowerSumEt, &b_electron_dr03HcalTowerSumEt);
   fChain->SetBranchAddress("electron_dr03TkSumPt", &electron_dr03TkSumPt, &b_electron_dr03TkSumPt);
   fChain->SetBranchAddress("electron_ecalPFClusterIso", &electron_ecalPFClusterIso, &b_electron_ecalPFClusterIso);
   fChain->SetBranchAddress("electron_hcalPFClusterIso", &electron_hcalPFClusterIso, &b_electron_hcalPFClusterIso);
   fChain->SetBranchAddress("muon_PfChargedHadronIsoR04", &muon_PfChargedHadronIsoR04, &b_muon_PfChargedHadronIsoR04);
   fChain->SetBranchAddress("muon_PfNeutralHadronIsoR04", &muon_PfNeutralHadronIsoR04, &b_muon_PfNeutralHadronIsoR04);
   fChain->SetBranchAddress("muon_PfGammaIsoR04", &muon_PfGammaIsoR04, &b_muon_PfGammaIsoR04);
   fChain->SetBranchAddress("muon_PFSumPUIsoR04", &muon_PFSumPUIsoR04, &b_muon_PFSumPUIsoR04);
   fChain->SetBranchAddress("muon_PfChargedHadronIsoR03", &muon_PfChargedHadronIsoR03, &b_muon_PfChargedHadronIsoR03);
   fChain->SetBranchAddress("muon_PfNeutralHadronIsoR03", &muon_PfNeutralHadronIsoR03, &b_muon_PfNeutralHadronIsoR03);
   fChain->SetBranchAddress("muon_PfGammaIsoR03", &muon_PfGammaIsoR03, &b_muon_PfGammaIsoR03);
   fChain->SetBranchAddress("muon_PFSumPUIsoR03", &muon_PFSumPUIsoR03, &b_muon_PFSumPUIsoR03);
   fChain->SetBranchAddress("muon_TypeBit", &muon_TypeBit, &b_muon_TypeBit);
   fChain->SetBranchAddress("muon_IDBit", &muon_IDBit, &b_muon_IDBit);
   fChain->SetBranchAddress("muon_ishighpt", &muon_ishighpt, &b_muon_ishighpt);
   fChain->SetBranchAddress("muon_dB", &muon_dB, &b_muon_dB);
   fChain->SetBranchAddress("muon_phi", &muon_phi, &b_muon_phi);
   fChain->SetBranchAddress("muon_eta", &muon_eta, &b_muon_eta);
   fChain->SetBranchAddress("muon_pt", &muon_pt, &b_muon_pt);
   fChain->SetBranchAddress("muon_mass", &muon_mass, &b_muon_mass);
   fChain->SetBranchAddress("muon_trkiso", &muon_trkiso, &b_muon_trkiso);
   fChain->SetBranchAddress("muon_hcaliso", &muon_hcaliso, &b_muon_hcaliso);
   fChain->SetBranchAddress("muon_ecaliso", &muon_ecaliso, &b_muon_ecaliso);
   fChain->SetBranchAddress("muon_trkisoR05", &muon_trkisoR05, &b_muon_trkisoR05);
   fChain->SetBranchAddress("muon_hcalisoR05", &muon_hcalisoR05, &b_muon_hcalisoR05);
   fChain->SetBranchAddress("muon_ecalisoR05", &muon_ecalisoR05, &b_muon_ecalisoR05);
   fChain->SetBranchAddress("muon_charge", &muon_charge, &b_muon_charge);
   fChain->SetBranchAddress("muon_nChambers", &muon_nChambers, &b_muon_nChambers);
   fChain->SetBranchAddress("muon_matchedstations", &muon_matchedstations, &b_muon_matchedstations);
   fChain->SetBranchAddress("muon_stationMask", &muon_stationMask, &b_muon_stationMask);
   fChain->SetBranchAddress("muon_nSegments", &muon_nSegments, &b_muon_nSegments);
   fChain->SetBranchAddress("muon_normchi", &muon_normchi, &b_muon_normchi);
   fChain->SetBranchAddress("muon_validhits", &muon_validhits, &b_muon_validhits);
   fChain->SetBranchAddress("muon_trackerHits", &muon_trackerHits, &b_muon_trackerHits);
   fChain->SetBranchAddress("muon_pixelHits", &muon_pixelHits, &b_muon_pixelHits);
   fChain->SetBranchAddress("muon_validmuonhits", &muon_validmuonhits, &b_muon_validmuonhits);
   fChain->SetBranchAddress("muon_trackerLayers", &muon_trackerLayers, &b_muon_trackerLayers);
   fChain->SetBranchAddress("muon_qoverp", &muon_qoverp, &b_muon_qoverp);
   fChain->SetBranchAddress("muon_theta", &muon_theta, &b_muon_theta);
   fChain->SetBranchAddress("muon_lambda", &muon_lambda, &b_muon_lambda);
   fChain->SetBranchAddress("muon_dxy", &muon_dxy, &b_muon_dxy);
   fChain->SetBranchAddress("muon_d0", &muon_d0, &b_muon_d0);
   fChain->SetBranchAddress("muon_dsz", &muon_dsz, &b_muon_dsz);
   fChain->SetBranchAddress("muon_dz", &muon_dz, &b_muon_dz);
   fChain->SetBranchAddress("muon_dxyBS", &muon_dxyBS, &b_muon_dxyBS);
   fChain->SetBranchAddress("muon_dzBS", &muon_dzBS, &b_muon_dzBS);
   fChain->SetBranchAddress("muon_dszBS", &muon_dszBS, &b_muon_dszBS);
   fChain->SetBranchAddress("muon_dxyVTX", &muon_dxyVTX, &b_muon_dxyVTX);
   fChain->SetBranchAddress("muon_dxyerrVTX", &muon_dxyerrVTX, &b_muon_dxyerrVTX);
   fChain->SetBranchAddress("muon_dzVTX", &muon_dzVTX, &b_muon_dzVTX);
   fChain->SetBranchAddress("muon_dzerrVTX", &muon_dzerrVTX, &b_muon_dzerrVTX);
   fChain->SetBranchAddress("muon_3DIPVTX", &muon_3DIPVTX, &b_muon_3DIPVTX);
   fChain->SetBranchAddress("muon_3DIPerrVTX", &muon_3DIPerrVTX, &b_muon_3DIPerrVTX);
   fChain->SetBranchAddress("muon_dszVTX", &muon_dszVTX, &b_muon_dszVTX);
   fChain->SetBranchAddress("muon_vx", &muon_vx, &b_muon_vx);
   fChain->SetBranchAddress("muon_vy", &muon_vy, &b_muon_vy);
   fChain->SetBranchAddress("muon_vz", &muon_vz, &b_muon_vz);
   fChain->SetBranchAddress("muon_Best_pt", &muon_Best_pt, &b_muon_Best_pt);
   fChain->SetBranchAddress("muon_Best_ptError", &muon_Best_ptError, &b_muon_Best_ptError);
   fChain->SetBranchAddress("muon_Best_eta", &muon_Best_eta, &b_muon_Best_eta);
   fChain->SetBranchAddress("muon_Best_phi", &muon_Best_phi, &b_muon_Best_phi);
   fChain->SetBranchAddress("muon_Inner_pt", &muon_Inner_pt, &b_muon_Inner_pt);
   fChain->SetBranchAddress("muon_Inner_ptError", &muon_Inner_ptError, &b_muon_Inner_ptError);
   fChain->SetBranchAddress("muon_Inner_eta", &muon_Inner_eta, &b_muon_Inner_eta);
   fChain->SetBranchAddress("muon_Inner_phi", &muon_Inner_phi, &b_muon_Inner_phi);
   fChain->SetBranchAddress("muon_Outer_pt", &muon_Outer_pt, &b_muon_Outer_pt);
   fChain->SetBranchAddress("muon_Outer_ptError", &muon_Outer_ptError, &b_muon_Outer_ptError);
   fChain->SetBranchAddress("muon_Outer_eta", &muon_Outer_eta, &b_muon_Outer_eta);
   fChain->SetBranchAddress("muon_Outer_phi", &muon_Outer_phi, &b_muon_Outer_phi);
   fChain->SetBranchAddress("muon_GLB_pt", &muon_GLB_pt, &b_muon_GLB_pt);
   fChain->SetBranchAddress("muon_GLB_ptError", &muon_GLB_ptError, &b_muon_GLB_ptError);
   fChain->SetBranchAddress("muon_GLB_eta", &muon_GLB_eta, &b_muon_GLB_eta);
   fChain->SetBranchAddress("muon_GLB_phi", &muon_GLB_phi, &b_muon_GLB_phi);
   fChain->SetBranchAddress("muon_TuneP_pt", &muon_TuneP_pt, &b_muon_TuneP_pt);
   fChain->SetBranchAddress("muon_TuneP_ptError", &muon_TuneP_ptError, &b_muon_TuneP_ptError);
   fChain->SetBranchAddress("muon_TuneP_eta", &muon_TuneP_eta, &b_muon_TuneP_eta);
   fChain->SetBranchAddress("muon_TuneP_phi", &muon_TuneP_phi, &b_muon_TuneP_phi);
   fChain->SetBranchAddress("muon_TuneP_charge", &muon_TuneP_charge, &b_muon_TuneP_charge);
   fChain->SetBranchAddress("muon_roch_sf", &muon_roch_sf, &b_muon_roch_sf);
   fChain->SetBranchAddress("muon_roch_sf_up", &muon_roch_sf_up, &b_muon_roch_sf_up);
   fChain->SetBranchAddress("muon_PfChargedHadronMiniIso", &muon_PfChargedHadronMiniIso, &b_muon_PfChargedHadronMiniIso);
   fChain->SetBranchAddress("muon_PfNeutralHadronMiniIso", &muon_PfNeutralHadronMiniIso, &b_muon_PfNeutralHadronMiniIso);
   fChain->SetBranchAddress("muon_PfGammaMiniIso", &muon_PfGammaMiniIso, &b_muon_PfGammaMiniIso);
   fChain->SetBranchAddress("muon_PFSumPUMiniIso", &muon_PFSumPUMiniIso, &b_muon_PFSumPUMiniIso);
   fChain->SetBranchAddress("muon_MVA", &muon_MVA, &b_muon_MVA);
   fChain->SetBranchAddress("muon_lowptMVA", &muon_lowptMVA, &b_muon_lowptMVA);
   fChain->SetBranchAddress("muon_softMVA", &muon_softMVA, &b_muon_softMVA);
   fChain->SetBranchAddress("muon_jetPtRatio", &muon_jetPtRatio, &b_muon_jetPtRatio);
   fChain->SetBranchAddress("muon_jetPtRel", &muon_jetPtRel, &b_muon_jetPtRel);
   fChain->SetBranchAddress("muon_simType", &muon_simType, &b_muon_simType);
   fChain->SetBranchAddress("muon_simExtType", &muon_simExtType, &b_muon_simExtType);
   fChain->SetBranchAddress("muon_simFlavour", &muon_simFlavour, &b_muon_simFlavour);
   fChain->SetBranchAddress("muon_simHeaviestMotherFlavour", &muon_simHeaviestMotherFlavour, &b_muon_simHeaviestMotherFlavour);
   fChain->SetBranchAddress("muon_simPdgId", &muon_simPdgId, &b_muon_simPdgId);
   fChain->SetBranchAddress("muon_simMotherPdgId", &muon_simMotherPdgId, &b_muon_simMotherPdgId);
   fChain->SetBranchAddress("muon_simMatchQuality", &muon_simMatchQuality, &b_muon_simMatchQuality);
   fChain->SetBranchAddress("LHE_Px", &LHE_Px, &b_LHE_Px);
   fChain->SetBranchAddress("LHE_Py", &LHE_Py, &b_LHE_Py);
   fChain->SetBranchAddress("LHE_Pz", &LHE_Pz, &b_LHE_Pz);
   fChain->SetBranchAddress("LHE_E", &LHE_E, &b_LHE_E);
   fChain->SetBranchAddress("LHE_Status", &LHE_Status, &b_LHE_Status);
   fChain->SetBranchAddress("LHE_ID", &LHE_ID, &b_LHE_ID);
   fChain->SetBranchAddress("PDFWeights_Scale", &PDFWeights_Scale, &b_PDFWeights_Scale);
   fChain->SetBranchAddress("PDFWeights_Error", &PDFWeights_Error, &b_PDFWeights_Error);
   fChain->SetBranchAddress("PDFWeights_AlphaS", &PDFWeights_AlphaS, &b_PDFWeights_AlphaS);
   fChain->SetBranchAddress("gen_phi", &gen_phi, &b_gen_phi);
   fChain->SetBranchAddress("gen_eta", &gen_eta, &b_gen_eta);
   fChain->SetBranchAddress("gen_pt", &gen_pt, &b_gen_pt);
   fChain->SetBranchAddress("gen_mass", &gen_mass, &b_gen_mass);
   fChain->SetBranchAddress("gen_charge", &gen_charge, &b_gen_charge);
   fChain->SetBranchAddress("gen_mother_index", &gen_mother_index, &b_gen_mother_index);
   fChain->SetBranchAddress("gen_status", &gen_status, &b_gen_status);
   fChain->SetBranchAddress("gen_PID", &gen_PID, &b_gen_PID);
   fChain->SetBranchAddress("gen_isPrompt", &gen_isPrompt, &b_gen_isPrompt);
   fChain->SetBranchAddress("gen_isPromptFinalState", &gen_isPromptFinalState, &b_gen_isPromptFinalState);
   fChain->SetBranchAddress("gen_isTauDecayProduct", &gen_isTauDecayProduct, &b_gen_isTauDecayProduct);
   fChain->SetBranchAddress("gen_isPromptTauDecayProduct", &gen_isPromptTauDecayProduct, &b_gen_isPromptTauDecayProduct);
   fChain->SetBranchAddress("gen_isDirectPromptTauDecayProductFinalState", &gen_isDirectPromptTauDecayProductFinalState, &b_gen_isDirectPromptTauDecayProductFinalState);
   fChain->SetBranchAddress("gen_isHardProcess", &gen_isHardProcess, &b_gen_isHardProcess);
   fChain->SetBranchAddress("gen_isLastCopy", &gen_isLastCopy, &b_gen_isLastCopy);
   fChain->SetBranchAddress("gen_isLastCopyBeforeFSR", &gen_isLastCopyBeforeFSR, &b_gen_isLastCopyBeforeFSR);
   fChain->SetBranchAddress("gen_isPromptDecayed", &gen_isPromptDecayed, &b_gen_isPromptDecayed);
   fChain->SetBranchAddress("gen_isDecayedLeptonHadron", &gen_isDecayedLeptonHadron, &b_gen_isDecayedLeptonHadron);
   fChain->SetBranchAddress("gen_fromHardProcessBeforeFSR", &gen_fromHardProcessBeforeFSR, &b_gen_fromHardProcessBeforeFSR);
   fChain->SetBranchAddress("gen_fromHardProcessDecayed", &gen_fromHardProcessDecayed, &b_gen_fromHardProcessDecayed);
   fChain->SetBranchAddress("gen_fromHardProcessFinalState", &gen_fromHardProcessFinalState, &b_gen_fromHardProcessFinalState);
   fChain->SetBranchAddress("gen_isMostlyLikePythia6Status3", &gen_isMostlyLikePythia6Status3, &b_gen_isMostlyLikePythia6Status3);
   fChain->SetBranchAddress("gen_weight", &gen_weight, &b_gen_weight);
   fChain->SetBranchAddress("genWeight_Q", &genWeight_Q, &b_genWeight_Q);
   fChain->SetBranchAddress("genWeight_X1", &genWeight_X1, &b_genWeight_X1);
   fChain->SetBranchAddress("genWeight_X2", &genWeight_X2, &b_genWeight_X2);
   fChain->SetBranchAddress("genWeight_id1", &genWeight_id1, &b_genWeight_id1);
   fChain->SetBranchAddress("genWeight_id2", &genWeight_id2, &b_genWeight_id2);
   fChain->SetBranchAddress("genWeight_alphaQCD", &genWeight_alphaQCD, &b_genWeight_alphaQCD);
   fChain->SetBranchAddress("genWeight_alphaQED", &genWeight_alphaQED, &b_genWeight_alphaQED);
   fChain->SetBranchAddress("photon_Energy", &photon_Energy, &b_photon_Energy);
   fChain->SetBranchAddress("photon_EnergyUnCorr", &photon_EnergyUnCorr, &b_photon_EnergyUnCorr);
   fChain->SetBranchAddress("photon_eta", &photon_eta, &b_photon_eta);
   fChain->SetBranchAddress("photon_phi", &photon_phi, &b_photon_phi);
   fChain->SetBranchAddress("photon_scEta", &photon_scEta, &b_photon_scEta);
   fChain->SetBranchAddress("photon_scPhi", &photon_scPhi, &b_photon_scPhi);
   fChain->SetBranchAddress("photon_HoverE", &photon_HoverE, &b_photon_HoverE);
   fChain->SetBranchAddress("photon_hasPixelSeed", &photon_hasPixelSeed, &b_photon_hasPixelSeed);
   fChain->SetBranchAddress("photon_Full5x5_SigmaIEtaIEta", &photon_Full5x5_SigmaIEtaIEta, &b_photon_Full5x5_SigmaIEtaIEta);
   fChain->SetBranchAddress("photon_ChIso", &photon_ChIso, &b_photon_ChIso);
   fChain->SetBranchAddress("photon_NhIso", &photon_NhIso, &b_photon_NhIso);
   fChain->SetBranchAddress("photon_PhIso", &photon_PhIso, &b_photon_PhIso);
   fChain->SetBranchAddress("photon_ChIsoWithEA", &photon_ChIsoWithEA, &b_photon_ChIsoWithEA);
   fChain->SetBranchAddress("photon_NhIsoWithEA", &photon_NhIsoWithEA, &b_photon_NhIsoWithEA);
   fChain->SetBranchAddress("photon_PhIsoWithEA", &photon_PhIsoWithEA, &b_photon_PhIsoWithEA);
   fChain->SetBranchAddress("photon_passMVAID_WP80", &photon_passMVAID_WP80, &b_photon_passMVAID_WP80);
   fChain->SetBranchAddress("photon_passMVAID_WP90", &photon_passMVAID_WP90, &b_photon_passMVAID_WP90);
   fChain->SetBranchAddress("photon_passLooseID", &photon_passLooseID, &b_photon_passLooseID);
   fChain->SetBranchAddress("photon_passMediumID", &photon_passMediumID, &b_photon_passMediumID);
   fChain->SetBranchAddress("photon_passTightID", &photon_passTightID, &b_photon_passTightID);
   fChain->SetBranchAddress("nPileUp", &nPileUp, &b_nPileUp);
   fChain->SetBranchAddress("pfMET_pt", &pfMET_pt, &b_pfMET_pt);
   fChain->SetBranchAddress("pfMET_phi", &pfMET_phi, &b_pfMET_phi);
   fChain->SetBranchAddress("pfMET_SumEt", &pfMET_SumEt, &b_pfMET_SumEt);
   fChain->SetBranchAddress("pfMET_Type1_pt", &pfMET_Type1_pt, &b_pfMET_Type1_pt);
   fChain->SetBranchAddress("pfMET_Type1_phi", &pfMET_Type1_phi, &b_pfMET_Type1_phi);
   fChain->SetBranchAddress("pfMET_Type1_SumEt", &pfMET_Type1_SumEt, &b_pfMET_Type1_SumEt);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_pt", &pfMET_Type1_PhiCor_pt, &b_pfMET_Type1_PhiCor_pt);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_phi", &pfMET_Type1_PhiCor_phi, &b_pfMET_Type1_PhiCor_phi);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_SumEt", &pfMET_Type1_PhiCor_SumEt, &b_pfMET_Type1_PhiCor_SumEt);
   fChain->SetBranchAddress("pfMET_pt_shifts", &pfMET_pt_shifts, &b_pfMET_pt_shifts);
   fChain->SetBranchAddress("pfMET_phi_shifts", &pfMET_phi_shifts, &b_pfMET_phi_shifts);
   fChain->SetBranchAddress("pfMET_SumEt_shifts", &pfMET_SumEt_shifts, &b_pfMET_SumEt_shifts);
   fChain->SetBranchAddress("pfMET_Type1_pt_shifts", &pfMET_Type1_pt_shifts, &b_pfMET_Type1_pt_shifts);
   fChain->SetBranchAddress("pfMET_Type1_phi_shifts", &pfMET_Type1_phi_shifts, &b_pfMET_Type1_phi_shifts);
   fChain->SetBranchAddress("pfMET_Type1_SumEt_shifts", &pfMET_Type1_SumEt_shifts, &b_pfMET_Type1_SumEt_shifts);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_pt_shifts", &pfMET_Type1_PhiCor_pt_shifts, &b_pfMET_Type1_PhiCor_pt_shifts);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_phi_shifts", &pfMET_Type1_PhiCor_phi_shifts, &b_pfMET_Type1_PhiCor_phi_shifts);
   fChain->SetBranchAddress("pfMET_Type1_PhiCor_SumEt_shifts", &pfMET_Type1_PhiCor_SumEt_shifts, &b_pfMET_Type1_PhiCor_SumEt_shifts);
   Notify();
}

Bool_t myclass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void myclass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t myclass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef myclass_cxx
