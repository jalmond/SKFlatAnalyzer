#include "TriggerNormCheck.h"

void TriggerNormCheck::initializeAnalyzer(){

}

void TriggerNormCheck::executeEvent(){


  AnalyzerParameter param;

  param.Electron_Tight_ID = "passMediumID";
  param.Electron_ID_SF_Key = "passMediumID";

  param.Muon_Tight_ID = "POGTightWithTightIso";
  param.Muon_ID_SF_Key = "NUM_TightID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";

  if(!IsDATA || (IsDATA && DataStream=="SingleMuon")){

    param.Name = "SingleMuon";
    Muon_TrigWithPtRange.PtValues          = {25,            30,           55,      999999};
    Muon_TrigWithPtRange.Triggers          = {"HLT_Mu20_v", "HLT_Mu27_v", "HLT_Mu50_v"};
    Muon_TrigWithPtRange.IsDATA = IsDATA;
    Muon_TrigWithPtRange.DataStream = DataStream;
    Muon_TrigWithPtRange.TriggerSafePtCuts = {25,            30,           55};
    Muon_TrigWithPtRange.Validate();

    executeEventFromParameter(param);

  }

  if(!IsDATA || (IsDATA && DataStream=="DoubleMuon")){

    param.Name = "DoubleMuon";
    Muon_TrigWithPtRange.PtValues          = {10,        20,        999999};
    Muon_TrigWithPtRange.Triggers          = {"HLT_Mu8_v", "HLT_Mu17_v"};
    Muon_TrigWithPtRange.IsDATA = IsDATA;
    Muon_TrigWithPtRange.DataStream = DataStream;
    Muon_TrigWithPtRange.TriggerSafePtCuts = {10,        20};
    Muon_TrigWithPtRange.Validate();

    executeEventFromParameter(param);

  }

  if(!IsDATA || (IsDATA && DataStream=="SingleElectron")){

    param.Name = "SingleElectron";
    Electron_TrigWithPtRange.PtValues          = {10,                                        15,                                             25,                                     999999};
    Electron_TrigWithPtRange.Triggers          = {"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"};
    Electron_TrigWithPtRange.IsDATA = IsDATA;
    Electron_TrigWithPtRange.DataStream = DataStream;
    Electron_TrigWithPtRange.TriggerSafePtCuts = {10,                                        15,                                             25};
    Electron_TrigWithPtRange.Validate();

    executeEventFromParameter(param);

  }

  if(!IsDATA || (IsDATA && DataStream=="SinglePhoton")){

    param.Name = "SinglePhoton";
    Electron_TrigWithPtRange.PtValues          = {28,                35,            55,               80,              100,              140,               170,                200,              250,           999999};
    Electron_TrigWithPtRange.Triggers          = {"HLT_Photon25_v", "HLT_Photon33_v", "HLT_Photon50_v", "HLT_Photon75_v", "HLT_Photon90_v", "HLT_Photon120_v", "HLT_Photon150_v", "HLT_Photon175_v", "HLT_Photon200_v"};
    Electron_TrigWithPtRange.TriggerSafePtCuts = {28,                35,            55,               80,              100,              140,               170,                200,              250};
    Electron_TrigWithPtRange.IsDATA = IsDATA;
    Electron_TrigWithPtRange.DataStream = DataStream;
    Electron_TrigWithPtRange.Validate();

    executeEventFromParameter(param);

  }

}

void TriggerNormCheck::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  std::vector<Electron> electrons = GetElectrons(param.Electron_Tight_ID, 10., 2.5);
  std::vector<Muon>         muons = GetMuons(param.Muon_Tight_ID, 10., 2.4);

  double sf_electrons = 1.;
  double sf_muons = 1.;

  if(!IsDATA){
    for(unsigned int i=0; i<electrons.size(); i++){
      double this_recosf = mcCorr->ElectronReco_SF(electrons.at(i).scEta(),electrons.at(i).Pt());
      double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, electrons.at(i).scEta(), electrons.at(i).Pt());
      sf_electrons *= this_recosf*this_idsf;
    }
    for(unsigned int i=0; i<muons.size(); i++){
      double this_pt = muons.at(i).MiniAODPt();
      double this_eta = muons.at(i).Eta();

      double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  this_eta, this_pt);
      double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, this_eta, this_pt);

      sf_muons *= this_idsf*this_isosf;
    }
  }

  bool RunElectronSingleLeptonNormPlots = false;
  bool RunMuonSingleLeptonNormPlots = false;
  if(param.Name=="SingleMuon" || param.Name=="DoubleMuon") RunMuonSingleLeptonNormPlots = true;
  if(param.Name=="SinglePhoton" || param.Name=="SingleElectron") RunElectronSingleLeptonNormPlots = true;

  for(int it_lep=0; it_lep<2; it_lep++){

    TString LepFlav;
    TriggerWithPtRange this_twpr;
    bool OneLeptonEvent;
    bool TwoLeptonEvent;
    bool DrawThis;
    vector<Lepton> leptons;
    double this_sf;

    if(it_lep==0){
      LepFlav = "Electron";
      this_twpr = Electron_TrigWithPtRange;
      OneLeptonEvent = (electrons.size()==1) && (muons.size()==0);
      TwoLeptonEvent = (electrons.size()==2) && (muons.size()==0);
      DrawThis = RunElectronSingleLeptonNormPlots;
      for(unsigned int i=0; i<electrons.size(); i++){
        leptons.push_back( electrons.at(i) );
      }
      this_sf = sf_electrons;
    }
    else{
      LepFlav = "Muon";
      this_twpr = Muon_TrigWithPtRange;
      OneLeptonEvent = (electrons.size()==0) && (muons.size()==1);
      TwoLeptonEvent = (electrons.size()==0) && (muons.size()==2);
      DrawThis = RunMuonSingleLeptonNormPlots;
      for(unsigned int i=0; i<muons.size(); i++){
        leptons.push_back( muons.at(i) );
      }
      this_sf = sf_muons;
    }

    if(DrawThis){

      for(unsigned int i=0; i<this_twpr.Triggers.size(); i++){
        TString this_trigger = this_twpr.Triggers.at(i);

        if(PassTrigger(ev, this_trigger)){
          //==== TODO Add SFs
          double weight = 1.;
          if(!IsDATA){
            weight *= weight_norm_1invpb * ev.GetTriggerLumi(this_trigger) * ev.MCweight() * this_sf;
          }

          if(OneLeptonEvent){
            double thisMT = MT( METv, leptons.at(0) );

            if( (METv.Pt() > 40.) && (leptons.at(0).Pt() > this_twpr.TriggerSafePtCuts.at(i) ) && (leptons.at(0).Pt() > 20.) ){
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"Lepton_0_Pt", leptons.at(0).Pt(), weight, 1000, 0., 1000.);
            }

            if( (METv.Pt() > 40.) && (thisMT > 50) && (leptons.at(0).Pt() > this_twpr.TriggerSafePtCuts.at(i) ) && (leptons.at(0).Pt() > 20.) ){
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"Lepton_0_Pt", leptons.at(0).Pt(), weight, 1000, 0., 1000.);
            }

          }
          if(TwoLeptonEvent){
            double dilepmass = (leptons.at(0)+leptons.at(1)).M();
            bool OnZEvent = IsOnZ( dilepmass, 15. );
            if( OnZEvent && (leptons.at(0).Pt() > this_twpr.TriggerSafePtCuts.at(i) ) && (leptons.at(1).Pt() > 20.) ){
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Z_Mass", dilepmass, weight, 40, 70., 110.);
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_0_Pt", leptons.at(0).Pt(), weight, 1000, 0., 1000.);
              JSFillHist(LepFlav+"_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_1_Pt", leptons.at(1).Pt(), weight, 1000, 0., 1000.);
            }
          }


        }

      } // END Loop Trigger


    }


  }


}

TriggerNormCheck::TriggerNormCheck(){

}

TriggerNormCheck::~TriggerNormCheck(){

}

bool TriggerNormCheck::PassTrigger(Event ev, TString trg){

  if(DataYear==2016 && trg=="HLT_Mu50_v"){
    return ev.PassTrigger("HLT_Mu50_v") || ev.PassTrigger("HLT_TkMu50_v");
  }
  else{
    return ev.PassTrigger(trg);
  }

}



