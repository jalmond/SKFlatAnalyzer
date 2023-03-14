#include "AnalyzerParameter.h"

void AnalyzerParameter::Clear(){

  Name = "Default";
  DefName = "Default";

  MCCorrrectionIgnoreNoHist = false;
  DEBUG = false;

  WriteOutVerbose = 0;
  SRConfig="";
  Electron_Tight_ID = "Default";
  Electron_Loose_ID = "Default";
  Electron_Veto_ID = "Default";
  Electron_ID_SF_Key = "Default";
  Electron_Trigger_SF_Key = "Default";
  Electron_Trigger_NameForSF = "Default";

  Electron_FR_ID = "Default";
  Electron_FR_Key = "Default";
  Electron_PR_Key = "Default";
  
  
  Electron_CF_ID = "Default";
  Electron_CF_Key = "Default";
  Electron_Tight_RelIso = 999.;
  Electron_Loose_RelIso = 999.;
  Electron_Veto_RelIso = 999.;
  Electron_UseMini = false;
  Electron_UsePtCone = false;
  Electron_MinPt = 10.;


  
  Tau_Tight_ID = "Default";
  Tau_Loose_ID = "Default";
  Tau_Veto_ID = "Default";

  Muon_Tight_ID = "Default";
  Muon_Loose_ID = "Default";
  Muon_Veto_ID = "Default";
  Muon_RECO_SF_Key = "Default";
  Muon_Trigger_SF_Key = "Default";
  Muon_Trigger_NameForSF = "Default";
  Muon_ID_SF_Key = "Default";
  Muon_ISO_SF_Key = "Default";
  Muon_Trigger_SF_Key = "Default";
  Muon_FR_ID = "Default";
  Muon_FR_Key = "Default";
  Muon_PR_Key = "Default";
  Muon_CF_ID = "Default";
  Muon_CF_Key = "Default";
  Muon_Tight_RelIso = 999.;
  Muon_Loose_RelIso = 999.;
  Muon_Veto_RelIso = 999.;
  Muon_UseMini = false;
  Muon_UsePtCone = false;
  Muon_UseTuneP = false;
  Muon_MinPt = 10.;

  SystDir_PU=0;
  SystDir_BTag="central";
  BJet_Method="2a";
  ElFakeMethod="MC";
  MuFakeMethod="MC";
  CFMethod="MC";
  ConvMethod="MC";
  JetPUID="Default";
  Jet_ID = "Default";
  FatJet_ID = "Default";

  syst_ = Central;

}

AnalyzerParameter::AnalyzerParameter(){

  Name = "Default";
  DefName = "Default";

  MCCorrrectionIgnoreNoHist = false;
  DEBUG = false;

  WriteOutVerbose = 0;
  SRConfig="";

  Electron_Tight_ID = "Default";
  Electron_Loose_ID = "Default";
  Electron_Veto_ID = "Default";
  Electron_ID_SF_Key = "Default";
  Electron_Trigger_NameForSF = "Default";
  Electron_Trigger_SF_Key = "Default";


  Tau_Tight_ID = "Default";
  Tau_Loose_ID = "Default";
  Tau_Veto_ID = "Default";

  Muon_Tight_ID = "Default";
  Muon_Loose_ID = "Default";
  Muon_Veto_ID = "Default";
  Muon_RECO_SF_Key = "Default";
  Muon_ID_SF_Key = "Default";
  Muon_ISO_SF_Key = "Default";
  Muon_Trigger_SF_Key = "Default";
  Muon_Trigger_NameForSF = "Default";
    


  SystDir_PU=0;
  SystDir_BTag="Default";
  BJet_Method="Default";
  ElFakeMethod="MC";
  MuFakeMethod="MC";
  CFMethod="MC";
  ConvMethod="MC";

  JetPUID="Default";

  Jet_ID = "Default";
  FatJet_ID = "Default";

  syst_ = Central;

}

TString AnalyzerParameter::GetSystType(){

  if(syst_==Syst::Central){
    return "Central";
  }
  else if(syst_==Syst::JetResUp){
    return "JetResUp";
  }
  else if(syst_==Syst::JetResDown){
    return "JetResDown";
  }
  else if(syst_==Syst::JetEnUp){
    return "JetEnUp";
  }
  else if(syst_==Syst::JetEnDown){
    return "JetEnDown";
  }
  else if(syst_==Syst::PrefireUp){
    return "PrefireUp";
  }
  else if(syst_==Syst::PrefireDown){
    return "PrefireDown";
  }
  else if(syst_==Syst::PUUp){
    return "PUUp";
  }
  else if(syst_==Syst::PUDown){
    return "PUDown";
  }

  else if(syst_==Syst::JetPUIDUp){
    return "JetPUIDUp";
  }
  else if(syst_==Syst::JetPUIDDown){
    return "JetPUIDDown";
  }
  else if(syst_==Syst::JetMassUp){
    return "JetMassUp";
  }
  else if(syst_==Syst::JetMassDown){
    return "JetMassDown";
  }
  else if(syst_==Syst::JetMassSmearUp){
    return "JetMassSmearUp";
  }
  else if(syst_==Syst::JetMassSmearDown){
    return "JetMassSmearDown";
  }
  else if(syst_==Syst::MuonRecoSFUp){
    return "MuonRecoSFUp";
  }
  else if(syst_==Syst::MuonRecoSFDown){
    return "MuonRecoSFDown";
  }
  
  else if(syst_==Syst::MuonEnUp){
    return "MuonEnUp";
  }
  else if(syst_==Syst::MuonEnDown){
    return "MuonEnDown";
  }
  
  else if(syst_==Syst::MuonIDSFUp){
    return "MuonIDSFUp";
  }
  else if(syst_==Syst::MuonIDSFDown){
    return "MuonIDSFDown";
  }
  else if(syst_==Syst::MuonISOSFUp){

    return "MuonISOSFUp";
 }
  else if(syst_==Syst::MuonISOSFDown){
    return "MuonISOSFDown";
  }
  else if(syst_==Syst::MuonTriggerSFUp){
    return "MuonTriggerSFUp";
  }
  else if(syst_==Syst::MuonTriggerSFDown){
    return "MuonTriggerSFDown";
  }
  else if(syst_==Syst::ElectronRecoSFUp){
    return "ElectronRecoSFUp";
  }
  else if(syst_==Syst::ElectronRecoSFDown){
    return "ElectronRecoSFDown";
  }
  else if(syst_==Syst::ElectronResUp){
    return "ElectronResUp";
  }
  else if(syst_==Syst::ElectronResDown){
    return "ElectronResDown";
  }
  else if(syst_==Syst::ElectronEnUp){
    return "ElectronEnUp";
  }
  else if(syst_==Syst::ElectronEnDown){
    return "ElectronEnDown";
  }
  else if(syst_==Syst::ElectronIDSFUp){
    return "ElectronIDSFUp";
  }
  else if(syst_==Syst::ElectronIDSFDown){
    return "ElectronIDSFDown";
  }
  else if(syst_==Syst::ElectronTriggerSFUp){
    return "ElectronTriggerSFUp";
  }
  else if(syst_==Syst::ElectronTriggerSFDown){
    return "ElectronTriggerSFDown";
  }
  else if(syst_==Syst::BTagSFHTagUp){
    return "BTagSFHTagUp";
  }
  else if(syst_==Syst::BTagSFHTagDown){
    return "BTagSFHTagDown";
  }
  else if(syst_==Syst::BTagSFLTagUp){
    return "BTagSFLTagUp";
  }
  else if(syst_==Syst::BTagSFLTagDown){
    return "BTagSFLTagDown";
  }
  else if(syst_==Syst::METUnclUp){
    return "METUnclUp";
  }
  else if(syst_==Syst::METUnclDown){
    return "METUnclDown";
  }
  else if(syst_==Syst::CFUp){
    return "CFUp";
  }
  else if(syst_==Syst::CFDown){
    return "CFDown";
  }
  else if(syst_==Syst::FRUp){
    return "FRUp";
  }
  else if(syst_==Syst::FRDown){
    return "FRDown";
  }


  else{
    cout << "[AnalyzerParameter::GetSystType] Wrong Syst" << endl;
    exit(ENODATA);
    return "ERROR";
  }

}

AnalyzerParameter::~AnalyzerParameter(){
}
