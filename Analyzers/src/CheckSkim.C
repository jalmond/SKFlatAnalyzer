#include "CheckSkim.h"

void CheckSkim::initializeAnalyzer(){

  triggers.clear();
  if(DataYear==2016){
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
      "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v",     // B-G                                                                                                                                             
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v",    // B-G                                                                                                                                             
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",  // H                                                                                                                                               
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"  // H                                                                                                                                               
    };
  }
  else if(DataYear==2017){
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"
    };
  }
  else if(DataYear==2018){
    triggers = {
      "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v",
      "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v",
      "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v",
      "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"
    };

  }
}

void CheckSkim::executeEvent(){


  Event ev;
  ev.SetTrigger(*HLT_TriggerName);

  //==== Skim 1 ) trigger                                                                                                                                                                                        
  if(! (ev.PassTrigger(triggers)) ) return;



  std::vector<Muon>     muonPreColl     = GetMuons("HNLoosest", 8., 2.4);
  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 8., 2.5);

  std::sort(muonPreColl.begin(), muonPreColl.end(), PtComparing);
  std::sort(electronPreColl.begin(), electronPreColl.end(), PtComparing);

  int NEl  = electronPreColl.size();
  int NMu  = muonPreColl.size();
  int NLep = NEl+NMu;

  if(NLep!=2) return;
  if(NEl > 0 && electronPreColl[0].Pt() < 25.) return;
  if(NMu > 0 && muonPreColl[0].Pt() < 20.) return;

  double MCweight = 1.;
  //  if(!IsDATA) MCweight = gen_weight>0 ? 1. : -1. ;

  FillHist("sumW", 0, MCweight, 1, 0., 1.);
  
}
