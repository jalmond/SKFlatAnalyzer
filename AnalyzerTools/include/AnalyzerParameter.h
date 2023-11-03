#ifndef AnalyzerParameter_h
#define AnalyzerParameter_h

#include "TString.h"
#include <iostream>

using namespace std;

class AnalyzerParameter{

public:

  TString Name;
  TString DefName;

  bool MCCorrrectionIgnoreNoHist,DEBUG;

  TString Electron_Tight_ID, Electron_Loose_ID, Electron_Veto_ID;
  TString Electron_ID_SF_Key, Electron_Trigger_SF_Key,Electron_Trigger_NameForSF;
  TString Electron_FR_ID, Electron_FR_Key, Electron_PR_Key;
  TString Electron_CF_ID, Electron_CF_Key;
  double Electron_Tight_RelIso, Electron_Loose_RelIso, Electron_Veto_RelIso;
  bool Electron_UseMini, Electron_UsePtCone;
  double Electron_MinPt, Electron_MaxEta ;
  double Electron_LFcut; //JH

  TString Muon_Tight_ID, Muon_Loose_ID, Muon_Veto_ID;
  TString Muon_RECO_SF_Key, Muon_ID_SF_Key, Muon_ISO_SF_Key, Muon_Trigger_SF_Key,Muon_Trigger_NameForSF;
  TString Muon_FR_ID, Muon_FR_Key, Muon_PR_Key;
  TString Muon_CF_ID, Muon_CF_Key;
  double Muon_Tight_RelIso, Muon_Loose_RelIso, Muon_Veto_RelIso;
  bool Muon_UseMini, Muon_UsePtCone, Muon_UseTuneP;
  double Muon_MinPt, Muon_MaxEta ;
  double Muon_LFcut; //JH

  TString JetPUID;
  double Jet_MinPt, Jet_MaxEta ;
  double FatJet_MinPt, FatJet_MaxEta ;

  TString Tau_Tight_ID, Tau_Loose_ID, Tau_Veto_ID;
  
  TString SRConfig;

  int SystDir_PU,WriteOutVerbose;

  std::string SystDir_BTag,  BJet_Method,   FakeMethod,CFMethod,  ConvMethod;
;
;


  TString Jet_ID, FatJet_ID;

  enum Syst{
    Central,
    JetResUp, JetResDown,
    JetEnUp, JetEnDown,
    
    JetMassUp,JetMassDown,
    JetMassSmearUp,JetMassSmearDown,
    MuonRecoSFUp,MuonRecoSFDown,
    MuonEnUp,MuonEnDown,
    MuonIDSFUp,MuonIDSFDown,
    MuonISOSFUp,MuonISOSFDown,
    MuonTriggerSFUp,MuonTriggerSFDown,
    ElectronRecoSFUp,ElectronRecoSFDown,
    ElectronResUp,ElectronResDown,
    ElectronEnUp,ElectronEnDown,
    ElectronIDSFUp,ElectronIDSFDown,
    ElectronTriggerSFUp,ElectronTriggerSFDown,
    BTagSFHTagUp,BTagSFHTagDown,
    BTagSFLTagUp,BTagSFLTagDown,
    METUnclUp,METUnclDown,
    CFUp,CFDown,
    FRUp,FRDown,
    PrefireUp,PrefireDown,
    PUUp,PUDown,
    JetPUIDUp, JetPUIDDown,
    NSyst
  };
  Syst syst_;
  TString GetSystType();

  void Clear();

  AnalyzerParameter();
  ~AnalyzerParameter();

};

#endif
