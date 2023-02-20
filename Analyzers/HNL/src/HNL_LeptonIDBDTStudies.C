#include "HNL_LeptonIDBDTStudies.h"

void HNL_LeptonIDBDTStudies::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();

}

double HNL_LeptonIDBDTStudies::GetCorrCloseJetDisc(bool ismuon, double jetdisc){

  if(IsData) return jetdisc;
  double SF=1;
  if(!ismuon){
    if (DataEra=="2016postVFP"){
      if(jetdisc < 0.02)  SF= 1.11111 ; 
      else if(jetdisc < 0.04)  SF= 1.08108 ; 
      else if(jetdisc < 0.06)  SF= 1.07143 ; 
      else if(jetdisc < 0.08)  SF= 1.05263 ; 
      else if(jetdisc < 0.1)  SF= 1.04167 ; 
      else if(jetdisc < 0.12)  SF= 1.04348 ; 
      else if(jetdisc < 0.14)  SF= 1.04478 ; 
      else if(jetdisc < 0.16)  SF= 1.05263 ; 
      else if(jetdisc < 0.18)  SF= 1.04651 ; 
      else if(jetdisc < 0.2)  SF= 1.04712 ; 
      else if(jetdisc < 0.22)  SF= 1.04265 ; 
      else if(jetdisc < 0.24)  SF= 1.03896 ; 
      else if(jetdisc < 0.26)  SF= 1.02362 ; 
      else if(jetdisc < 0.28)  SF= 1.0219 ; 
      else if(jetdisc < 0.3)  SF= 1.02389 ; 
      else if(jetdisc < 0.32)  SF= 1.00629 ; 
      else  SF= 1 ; 
    }
    if (DataEra=="2016preVFP"){
      if(jetdisc < 0.02)  SF= 1.11111 ; 
      else if(jetdisc < 0.04)  SF= 1.05263 ; 
      else if(jetdisc < 0.06)  SF= 1.07143 ; 
      else if(jetdisc < 0.08)  SF= 1.03896 ; 
      else if(jetdisc < 0.1)  SF= 1.04167 ; 
      else if(jetdisc < 0.12)  SF= 1.03448 ; 
      else if(jetdisc < 0.14)  SF= 1.03704 ; 
      else if(jetdisc < 0.16)  SF= 1.02564 ; 
      else if(jetdisc < 0.18)  SF= 1.01695 ; 
      else if(jetdisc < 0.2)  SF= 1.01523 ; 
      else if(jetdisc < 0.22)  SF= 1.01852 ; 
      else if(jetdisc < 0.24)  SF= 1.01695 ; 
      else if(jetdisc < 0.26)  SF= 1.00775 ; 
      else SF=1;
    }

    if (DataEra=="2017"){
      if(jetdisc < 0.02)  SF= 1.17647 ; 
      else if(jetdisc < 0.04)  SF= 1.14286 ; 
      else if(jetdisc < 0.06)  SF= 1.11111 ; 
      else if(jetdisc < 0.08)  SF= 1.09589 ; 
      else if(jetdisc < 0.1)  SF= 1.08696 ; 
      else if(jetdisc < 0.12)  SF= 1.08108 ; 
      else if(jetdisc < 0.14)  SF= 1.07692 ; 
      else if(jetdisc < 0.16)  SF= 1.08108 ; 
      else if(jetdisc < 0.18)  SF= 1.09091 ; 
      else if(jetdisc < 0.2)  SF= 1.08696 ; 
      else if(jetdisc < 0.22)  SF= 1.08374 ; 
      else if(jetdisc < 0.24)  SF= 1.07623 ; 
      else if(jetdisc < 0.26)  SF= 1.05691 ; 
      else if(jetdisc < 0.28)  SF= 1.04089 ; 
      else if(jetdisc < 0.3)  SF= 1.02389 ; 
      else if(jetdisc < 0.32)  SF= 1.01587 ; 
      else if(jetdisc < 0.34)  SF= 1.00592 ; 
      else  SF= 1 ; 
    }
    if(DataEra=="2018"){
      if(jetdisc < 0.02)  SF= 1.17647 ; 
      else if(jetdisc < 0.04)  SF= 1.14286 ; 
      else if(jetdisc < 0.06)  SF= 1.11111 ; 
      else if(jetdisc < 0.08)  SF= 1.09589 ; 
      else if(jetdisc < 0.1)  SF= 1.08696 ; 
      else if(jetdisc < 0.12)  SF= 1.09091 ; 
      else if(jetdisc < 0.14)  SF= 1.08527 ; 
      else if(jetdisc < 0.16)  SF= 1.09589 ; 
      else if(jetdisc < 0.18)  SF= 1.11801 ; 
      else if(jetdisc < 0.2)  SF= 1.1236 ; 
      else if(jetdisc < 0.22)  SF= 1.12245 ; 
      else if(jetdisc < 0.24)  SF= 1.10599 ; 
      else if(jetdisc < 0.26)  SF= 1.09244 ; 
      else if(jetdisc < 0.28)  SF= 1.08527 ; 
      else if(jetdisc < 0.3)  SF= 1.07527 ; 
      else if(jetdisc < 0.32)  SF= 1.07023 ; 
      else if(jetdisc < 0.34)  SF= 1.0625 ; 
      else if(jetdisc < 0.36)  SF= 1.04956 ; 
      else if(jetdisc < 0.38)  SF= 1.0411 ; 
      else if(jetdisc < 0.4)  SF= 1.03627 ; 
      else if(jetdisc < 0.42)  SF= 1.03194 ; 
      else if(jetdisc < 0.44)  SF= 1.03044 ; 
      else if(jetdisc < 0.46)  SF= 1.0245 ; 
      else if(jetdisc < 0.48)  SF= 1.02564 ; 
      else if(jetdisc < 0.5)  SF= 1.02669 ; 
      else if(jetdisc < 0.52)  SF= 1.02564 ; 
      else if(jetdisc < 0.54)  SF= 1.03053 ; 
      else if(jetdisc < 0.56)  SF= 1.02377 ; 
      else if(jetdisc < 0.58)  SF= 1.01933 ; 
      else if(jetdisc < 0.6)  SF= 1.01351 ; 
      else SF=1;

    }
    return jetdisc*SF;
  }
  else {

    if (DataEra=="2016postVFP"){
      if(jetdisc < 0.02)  SF= 1.11111 ; 
      else if(jetdisc < 0.04)  SF= 1.11111 ; 
      else if(jetdisc < 0.06)  SF= 1.11111 ; 
      else if(jetdisc < 0.08)  SF= 1.03896 ; 
      else  SF= 1 ; 
    }
    if (DataEra=="2016preVFP"){
      if(jetdisc < 0.02)  SF= 1.17647 ; 
      else if(jetdisc < 0.04)  SF= 1.25 ; 
      else if(jetdisc < 0.06)  SF= 1.30435 ; 
      else if(jetdisc < 0.08)  SF= 1.29032 ; 
      else if(jetdisc < 0.1)  SF= 1.23457 ; 
      else if(jetdisc < 0.12)  SF= 1.21212 ; 
      else if(jetdisc < 0.14)  SF= 1.21739 ; 
      else if(jetdisc < 0.16)  SF= 1.21212 ; 
      else if(jetdisc < 0.18)  SF= 1.23288 ; 
      else if(jetdisc < 0.2)  SF= 1.25 ; 
      else if(jetdisc < 0.22)  SF= 1.25 ; 
      else if(jetdisc < 0.24)  SF= 1.21827 ; 
      else if(jetdisc < 0.26)  SF= 1.19816 ; 
      else if(jetdisc < 0.28)  SF= 1.15226 ; 
      else if(jetdisc < 0.3)  SF= 1.13636 ; 
      else if(jetdisc < 0.32)  SF= 1.12281 ; 
      else if(jetdisc < 0.34)  SF= 1.10032 ; 
      else if(jetdisc < 0.36)  SF= 1.08108 ; 
      else if(jetdisc < 0.38)  SF= 1.05263 ; 
      else if(jetdisc < 0.4)  SF= 1.04712 ; 
      else if(jetdisc < 0.42)  SF= 1.03194 ; 
      else if(jetdisc < 0.44)  SF= 1.00457 ; 
      else SF= 1 ;
    }
    if (DataEra=="2017"){
      if(jetdisc < 0.02)  SF= 1.17647 ; 
      else if(jetdisc < 0.04)  SF= 1.17647 ; 
      else if(jetdisc < 0.06)  SF= 1.2 ; 
      else if(jetdisc < 0.08)  SF= 1.17647 ; 
      else if(jetdisc < 0.1)  SF= 1.13636 ; 
      else if(jetdisc < 0.12)  SF= 1.15385 ; 
      else if(jetdisc < 0.14)  SF= 1.16667 ; 
      else if(jetdisc < 0.16)  SF= 1.18519 ; 
      else if(jetdisc < 0.18)  SF= 1.19205 ; 
      else if(jetdisc < 0.2)  SF= 1.18343 ; 
      else if(jetdisc < 0.22)  SF= 1.1828 ; 
      else if(jetdisc < 0.24)  SF= 1.17647 ; 
      else if(jetdisc < 0.26)  SF= 1.15044 ; 
      else if(jetdisc < 0.28)  SF= 1.1336 ; 
      else if(jetdisc < 0.3)  SF= 1.1194 ; 
      else if(jetdisc < 0.32)  SF= 1.10345 ; 
      else if(jetdisc < 0.34)  SF= 1.08626 ; 
      else if(jetdisc < 0.36)  SF= 1.07784 ; 
      else if(jetdisc < 0.38)  SF= 1.07042 ; 
      else if(jetdisc < 0.4)  SF= 1.06383 ; 
      else if(jetdisc < 0.42)  SF= 1.06329 ; 
      else if(jetdisc < 0.44)  SF= 1.05012 ; 
      else if(jetdisc < 0.46)  SF= 1.04072 ; 
      else if(jetdisc < 0.48)  SF= 1.03672 ; 
      else if(jetdisc < 0.5)  SF= 1.03093 ; 
      else if(jetdisc < 0.52)  SF= 1.0297 ; 
      else if(jetdisc < 0.54)  SF= 1.02857 ; 
      else if(jetdisc < 0.56)  SF= 1.0219 ; 
      else if(jetdisc < 0.58)  SF= 1.01222 ; 
      else if(jetdisc < 0.6)  SF= 1.01351 ; 
      else if(jetdisc < 0.62)  SF= 1.01473 ; 
      else if(jetdisc < 0.64)  SF= 1.01587 ; 
      else if(jetdisc < 0.66)  SF= 1.01852 ; 
      else if(jetdisc < 0.68)  SF= 1.01341 ; 
      else if(jetdisc < 0.7)  SF= 1.01892 ; 
      else if(jetdisc < 0.72)  SF= 1.00418 ; 
      else  SF= 1.00543 ; 

    }
    if (DataEra=="2018"){

      if(jetdisc < 0.02)  SF= 1.17647 ; 
      else if(jetdisc < 0.04)  SF= 1.25 ; 
      else if(jetdisc < 0.06)  SF= 1.30435 ; 
      else if(jetdisc < 0.08)  SF= 1.26984 ; 
      else if(jetdisc < 0.1)  SF= 1.20482 ; 
      else if(jetdisc < 0.12)  SF= 1.21212 ; 
      else if(jetdisc < 0.14)  SF= 1.21739 ; 
      else if(jetdisc < 0.16)  SF= 1.21212 ; 
      else if(jetdisc < 0.18)  SF= 1.21622 ; 
      else if(jetdisc < 0.2)  SF= 1.23457 ; 
      else if(jetdisc < 0.22)  SF= 1.23596 ; 
      else if(jetdisc < 0.24)  SF= 1.22449 ; 
      else if(jetdisc < 0.26)  SF= 1.18182 ; 
      else if(jetdisc < 0.28)  SF= 1.15702 ; 
      else if(jetdisc < 0.3)  SF= 1.12782 ; 
      else if(jetdisc < 0.32)  SF= 1.10345 ; 
      else if(jetdisc < 0.34)  SF= 1.0828 ; 
      else if(jetdisc < 0.36)  SF= 1.06825 ; 
      else if(jetdisc < 0.38)  SF= 1.0585 ; 
      else if(jetdisc < 0.4)  SF= 1.04987 ; 
      else if(jetdisc < 0.42)  SF= 1.05528 ; 
      else if(jetdisc < 0.44)  SF= 1.04019 ; 
      else if(jetdisc < 0.46)  SF= 1.04072 ; 
      else if(jetdisc < 0.48)  SF= 1.03004 ; 
      else if(jetdisc < 0.5)  SF= 1.02881 ; 
      else if(jetdisc < 0.52)  SF= 1.03175 ; 
      else if(jetdisc < 0.54)  SF= 1.03053 ; 
      else if(jetdisc < 0.56)  SF= 1.03131 ; 
      else if(jetdisc < 0.58)  SF= 1.03943 ; 
      else if(jetdisc < 0.6)  SF= 1.0453 ; 
      else if(jetdisc < 0.62)  SF= 1.04202 ; 
      else if(jetdisc < 0.64)  SF= 1.04575 ; 
      else if(jetdisc < 0.66)  SF= 1.04265 ; 
      else if(jetdisc < 0.68)  SF= 1.03501 ; 
      else if(jetdisc < 0.7)  SF= 1.03397 ; 
      else if(jetdisc < 0.72)  SF= 1.033 ; 
      else if(jetdisc < 0.74)  SF= 1.02351 ; 
      else SF=1;
    }
    return jetdisc*SF;

  }

  return jetdisc;

}

void HNL_LeptonIDBDTStudies::executeEvent(){
  
  if((_jentry==0)){ 
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer
    TriggerPrintOut(GetEvent());
  }
  
  if(!IsData)  gens = GetGens();

  TString ID = "POG";

  AnalyzerParameter param  = HNL_LeptonCore::InitialiseHNLParameter(ID,"");
  
  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);
    
  std::vector<Electron>   ElectronCollProbe = GetElectrons("HNL_ULID_Baseline", 10., 2.5); 
  std::vector<Electron>   ElectronColl = GetElectrons("passPOGTight", 10., 2.5); 

  std::vector<Muon>       MuonCollProbe     = GetMuons    ("HNL_ULID_Baseline", 10., 2.4);
  std::vector<Muon>       MuonColl     = GetMuons    ("POGTightWithTightIso", 10., 2.4);


  if(!PassMETFilter()) return;

  
  if(HasFlag("JetDisc"))MakeJetDiscPlots("LooseMuE",param,MuMu, ElectronCollProbe, MuonColl, weight);
  if(HasFlag("JetDisc"))MakeJetDiscPlots("TightMuE",param,MuMu, ElectronColl, MuonColl, weight);

  if(HasFlag("JetDisc"))MakeJetDiscPlots("LooseEMu",param,EE, ElectronColl, MuonCollProbe, weight);
  if(HasFlag("JetDisc"))MakeJetDiscPlots("TightEMu",param,EE, ElectronColl, MuonColl, weight);

  HNL_LeptonCore::Channel dilep_channel= EE;
  if(MuonCollProbe.size() > 0) return;
  if(HasFlag("BDT"))MakeBDTPlots(param,dilep_channel, ElectronCollProbe, param.Electron_Tight_ID, weight);
  
  return;
  
}

void HNL_LeptonIDBDTStudies::MakeJetDiscPlots(TString label, AnalyzerParameter param,HNL_LeptonCore::Channel dilep_channel, vector<Electron> ElectronColl ,vector<Muon>       MuonColl, double weight_ll){

  TString channel_string = GetChannelString(dilep_channel) ;

  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(MuonColl, ElectronColl);

  if(label.Contains("Tight"))  weight_ll*=GetLeptonSFEventWeight(LeptonColl,param);
  
  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);
  std::vector<Lepton *>   LeptonCollV      = MakeLeptonPointerVector(MuonCollV, ElectronCollV);


  //// Select TT -> emu for Top sample
  
  Event ev = GetEvent();
  if(LeptonColl.size() != 2) return;
  if(HasFlag("SS")){
    if(!SameCharge(LeptonColl)) return;
  }
  else {
    if(SameCharge(LeptonColl)) return;
  }

  Particle METv = GetvMET("PuppiT1xyCorr"); // reyturns MET with systematic correction                                                                                                 

  std::vector<Jet>      JetColl     = GetHNLJets("Tight", param);
  std::vector<Jet> BJetColl  = GetHNLJets("BJetM", param);                                                                                                                

  if(HasFlag("SS")){
    if(BJetColl.size() == 0) return;
  }

  double metcut = (HasFlag("SS")) ? 30. : 40;
  

  if(ElectronColl.size() == 1 && MuonColl.size()   == 1){
    if(ElectronCollV.size() == 1 && MuonCollV.size()   == 1){
      if((JetColl.size() > 3 && METv.Pt() > metcut )){
	if(dilep_channel==MuMu){
	  if(PassTriggerSelection(dilep_channel, ev, LeptonColl,"Lep")){

	    if(MuonColl[0].Pt() > 20) {
	      
	      vector<Jet> JetAllColl = GetAllJets();
	      TString cut = "TopJets_TightMu";
	      
	      FillHist(cut +"/MET_"+channel_string, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 200, 0., 200);
	      if(JetColl.size() < 6) FillHist(cut +"/NJet_"+channel_string, JetColl.size(), weight_ll, 8, 0., 8);
	      else FillHist(cut +"/NJet_"+channel_string, 6., weight_ll, 8, 0., 8);

	      FillHist(cut +"/MuonPt_"+channel_string+"_"+label, MuonColl[0].Pt(), weight_ll, 40, 0., 200);
	      FillHist(cut +"/ElectronPt_"+channel_string+"_"+label, ElectronColl[0].Pt(), weight_ll, 40, 0., 200);
	      
              FillHist(cut +"/MET_"+channel_string+"_"+label, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 200, 0., 200);
              if(JetColl.size() < 6) FillHist(cut +"/NJet_"+channel_string+"_"+label, JetColl.size(), weight_ll, 8, 0., 8);
              else FillHist(cut +"/NJet_"+channel_string+"_"+label, 6., weight_ll, 8, 0., 8);

	      for(auto ilep: LeptonColl){
		
		if(ilep->Pt() > 25) FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_gt25PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
		if(ilep->Pt() > 25)     FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_gt25PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel2_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,false) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel3_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,false) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel4_"+label   , JetLeptonPtRelLepAware(*ilep,true,true,false) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel5_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,true) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel6_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,true) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel7"+label   , JetLeptonPtRelLepAware(*ilep,true,true,true) , weight_ll, 200,0,200);
		FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel8"+label   , JetLeptonPtRelLepAware(*ilep,false,false,true) , weight_ll, 200,0,200);

                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio2_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,false) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio3_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,false) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio4_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,false) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio5_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,true) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio6_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,true) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio7_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,true) , weight_ll, 200,0,2);
		FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio8_"+label   , JetLeptonPtRatioLepAware(*ilep,false,false,true) , weight_ll, 200,0,2);
		
		if(ilep->IsPrompt( GetLeptonType_JH(*ilep, gens))){
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel2_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,false) , weight_ll, 200,0,200);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel3_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,false) , weight_ll, 200,0,200);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel4_"+label   , JetLeptonPtRelLepAware(*ilep,true,true,false) , weight_ll, 200,0,200);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel5_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,true) , weight_ll, 200,0,200);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel6_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,true) , weight_ll, 200,0,200);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel7"+label   , JetLeptonPtRelLepAware(*ilep,true,true,true) , weight_ll, 200,0,200);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel8"+label   , JetLeptonPtRelLepAware(*ilep,false,false,true) , weight_ll, 200,0,200);

		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio2_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,false) , weight_ll, 200,0,2);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio3_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,false) , weight_ll, 200,0,2);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio4_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,false) , weight_ll, 200,0,2);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio5_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,true) , weight_ll, 200,0,2);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio6_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,true) , weight_ll, 200,0,2);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio7_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,true) , weight_ll, 200,0,2);
		  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio8_"+label   , JetLeptonPtRatioLepAware(*ilep,false,false,true) , weight_ll, 200,0,2);

		}

              }

	      for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
		
		bool closelep(false);
		TString lepflav="";
		TString ptlabel="";
		bool isprompt(true);

		for(auto ilep : LeptonCollV) {
		  if(ilep->DeltaR(JetAllColl.at(ij)) < 0.4) {
		    closelep=true;
		    lepflav=ilep->GetFlavour();
		    if(ilep->Pt() < 20) ptlabel="lt20_";
		    else if(ilep->Pt() < 35)ptlabel="lt35_";
		    else ptlabel="gt35_";
		    if(!ilep->IsPrompt( GetLeptonType_JH(*ilep, gens))) isprompt=false;
		  }
		}
		
		if(!closelep){
		  if(!JetAllColl.at(ij).PassID("tight")) continue;
		  if(JetAllColl.at(ij).Pt() < 20) continue;
		}
		
		FillHist( cut+ "/JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		
		if(closelep) {

		  double mc_cj_disc_corr  = GetCorrCloseJetDisc(true, JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet)) ;
		  
		  FillHist( cut+ "/CJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  FillHist( cut+ "/CJ_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  
		  FillHist( cut+ "/ModCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , mc_cj_disc_corr , weight_ll, 1000,0,1);
		  FillHist( cut+ "/ModCJ_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , mc_cj_disc_corr , weight_ll, 1000,0,1);

		  if(isprompt){
		    FillHist( cut+ "/CJ_PromptLep_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		    FillHist( cut+ "/CJ_PromptLep_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  }
		  else{
		    FillHist( cut+ "/CJ_NonPromptLep_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
                    FillHist( cut+ "/CJ_NonPromptLep_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  }
		}
		else {
		  FillHist( cut+ "/AJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  if(JetAllColl.at(ij).Pt() > 30) FillHist( cut+ "/AJ_JetDisc_DeepJet30_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		}
		
		if(JetAllColl.at(ij).PassID("tight")) {
		  if(closelep)    FillHist( cut+ "/TightCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  else {
		    FillHist( cut+ "/TightAJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		    if(JetAllColl.at(ij).Pt() > 30) FillHist( cut+ "/TightAJ_JetDisc_DeepJet30_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  }
		}
	      }
	      
	      //FillElectronPlots("ElectronMuon","Top",ElectronColl,weight_ll);
	      //if(BJetColl.size() > 1)FillElectronPlots("ElectronMuon","BTop",ElectronColl,weight_ll);
	    }
	  }
	}
	else if(dilep_channel==EE){

	  if(PassTriggerSelection(dilep_channel, ev, LeptonColl,"Lep")){
	    
	    if(ElectronColl[0].Pt() > 20) {
	      
	      vector<Jet> JetAllColl = GetAllJets();
	      TString cut = "TopJets_TightEl";

	      FillHist(cut +"/MET_"+channel_string, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 200, 0., 200);
	      if(JetColl.size() < 6) FillHist(cut +"/NJet_"+channel_string, JetColl.size(), weight_ll, 8, 0., 8);
	      else FillHist(cut +"/NJet_"+channel_string, 6., weight_ll, 8, 0., 8);

	      FillHist(cut +"/MuonPt_"+channel_string+"_"+label, MuonColl[0].Pt(), weight_ll, 40, 0., 200);
	      FillHist(cut +"/ElectronPt_"+channel_string+"_"+label, ElectronColl[0].Pt(), weight_ll, 40, 0., 200);
	      FillHist(cut +"/MET_"+channel_string+"_"+label, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 200, 0., 200);
              if(JetColl.size() < 6) FillHist(cut +"/NJet_"+channel_string+"_"+label, JetColl.size(), weight_ll, 8, 0., 8);
              else FillHist(cut +"/NJet_"+channel_string+"_"+label, 6., weight_ll, 8, 0., 8);


	      for(auto ilep: LeptonColl){
		if(ilep->Pt() > 25) FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_gt25PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
		if(ilep->Pt() > 25)     FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_gt25PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel2_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,false) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel3_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,false) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel4_"+label   , JetLeptonPtRelLepAware(*ilep,true,true,false) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel5_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,true) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel6_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,true) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel7"+label   , JetLeptonPtRelLepAware(*ilep,true,true,true) , weight_ll, 200,0,200);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel8"+label   , JetLeptonPtRelLepAware(*ilep,false,false,true) , weight_ll, 200,0,200);

                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio2_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,false) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio3_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,false) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio4_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,false) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio5_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,true) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio6_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,true) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio7_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,true) , weight_ll, 200,0,2);
                FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio8_"+label   , JetLeptonPtRatioLepAware(*ilep,false,false,true) , weight_ll, 200,0,2);

		if(ilep->IsPrompt( GetLeptonType_JH(*ilep, gens))){
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel2_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,false) , weight_ll, 200,0,200);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel3_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,false) , weight_ll, 200,0,200);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel4_"+label   , JetLeptonPtRelLepAware(*ilep,true,true,false) , weight_ll, 200,0,200);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel5_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,true) , weight_ll, 200,0,200);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel6_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,true) , weight_ll, 200,0,200);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel7"+label   , JetLeptonPtRelLepAware(*ilep,true,true,true) , weight_ll, 200,0,200);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel8"+label   , JetLeptonPtRelLepAware(*ilep,false,false,true) , weight_ll, 200,0,200);

                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio2_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,false) , weight_ll, 200,0,2);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio3_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,false) , weight_ll, 200,0,2);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio4_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,false) , weight_ll, 200,0,2);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio5_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,true) , weight_ll, 200,0,2);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio6_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,true) , weight_ll, 200,0,2);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio7_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,true) , weight_ll, 200,0,2);
                  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio8_"+label   , JetLeptonPtRatioLepAware(*ilep,false,false,true) , weight_ll, 200,0,2);

                }


	      }

	      for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
		
		bool closelep(false);
		TString lepflav="";
		TString ptlabel="";
		bool isprompt=true;
		for(auto ilep : LeptonCollV) {
		  if(ilep->DeltaR(JetAllColl.at(ij)) < 0.4) {
		    closelep=true;
		    lepflav=ilep->GetFlavour();
		    if(ilep->Pt() < 20)ptlabel="lt20_";
                    else if(ilep->Pt() < 35)ptlabel="lt35_";
                    else ptlabel="gt35_";
                    if(!ilep->IsPrompt( GetLeptonType_JH(*ilep, gens))) isprompt=false;

		  }
		}
		
		if(!closelep){
		  if(!JetAllColl.at(ij).PassID("tight")) continue;
		  if(JetAllColl.at(ij).Pt() < 20) continue;
		}
		
		FillHist( cut+ "/JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		
		if(closelep) {
                  double mc_cj_disc_corr  = GetCorrCloseJetDisc(false, JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet)) ;

		  FillHist( cut+ "/CJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  FillHist( cut+ "/CJ_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  
                  FillHist( cut+ "/ModCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , mc_cj_disc_corr , weight_ll, 1000,0,1);

                  FillHist( cut+ "/ModCJ_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , mc_cj_disc_corr , weight_ll, 1000,0,1);


		  if(isprompt){
                    FillHist( cut+ "/CJ_PromptLep_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
                    FillHist( cut+ "/CJ_PromptLep_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
                  }
		  else{
                    FillHist( cut+ "/CJ_NonPromptLep_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
                    FillHist( cut+ "/CJ_NonPromptLep_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
                  }

		}
		else {
		  FillHist( cut+ "/AJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
                  if(JetAllColl.at(ij).Pt() > 30) FillHist( cut+ "/AJ_JetDisc_DeepJet30_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		}
		
		if(JetAllColl.at(ij).PassID("tight")) {
		  if(closelep)    FillHist( cut+ "/TightCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  else FillHist( cut+ "/TightAJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		}
		if(BJetColl.size() > 1){
		  FillHist( cut+ "BJetEv/JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  
		  if(closelep)    FillHist( cut+ "BJetEv/CJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  else FillHist( cut+ "BJetEv/AJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  
		  if(JetAllColl.at(ij).PassID("tight")) {
		    if(closelep)    FillHist( cut+ "BJetEv/TightCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		    else FillHist( cut+ "BJetEv/TightAJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
		  }
		  
		}
	      }
	    }
	  }
	}
      }
    }
  }
  else{
  

    if(!PassTriggerSelection(dilep_channel, ev, LeptonColl,"Lep")) return;

    if(dilep_channel==EE && ElectronCollV.size() != 2) return;
    if(dilep_channel==EE &&  MuonCollV.size() > 0) return;
    if(dilep_channel==MuMu && MuonCollV.size() != 2) return;
    if(dilep_channel==MuMu && ElectronCollV.size()  > 0) return;
       
    if(dilep_channel==EE && ElectronColl.size() != 2) return;
    if(dilep_channel==EE &&  MuonColl.size() > 0) return;
    if(dilep_channel==MuMu && MuonColl.size() != 2) return;
    if(dilep_channel==MuMu && ElectronColl.size()  > 0) return;

    if(LeptonColl.size() != 2) return;

    if(dilep_channel==EE && ElectronCollV[0].Pt() < 35) return;
    if(dilep_channel==MuMu && MuonCollV[0].Pt() < 35) return;
    if(SameCharge(LeptonColl)) return;

    // Z peak 
    

    if(fabs(GetLLMass(LeptonColl)- 90.1) < 10 && METv.Pt() < 30) {
     
      vector<Jet> JetAllColl = GetAllJets();
      TString cut = "ZJets_"+channel_string;

      if(dilep_channel==MuMu)FillHist(cut +"/MuonPt_"+channel_string+"_"+label, MuonColl[1].Pt(), weight_ll, 40, 0., 200);
      if(dilep_channel==EE)  FillHist(cut +"/ElectronPt_"+channel_string+"_"+label, ElectronColl[1].Pt(), weight_ll, 40, 0., 200);

      FillHist(cut +"/MET_"+channel_string, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 200, 0., 200);
      if(JetColl.size() < 6) FillHist(cut +"/NJet_"+channel_string, JetColl.size(), weight_ll, 8, 0., 8);
      else FillHist(cut +"/NJet_"+channel_string, 6., weight_ll, 8, 0., 8);


      FillHist(cut +"/MET_"+channel_string+"_"+label, GetvMET("PuppiT1xyCorr").Pt(), weight_ll, 200, 0., 200);
      if(JetColl.size() < 6) FillHist(cut +"/NJet_"+channel_string+"_"+label, JetColl.size(), weight_ll, 8, 0., 8);
      else FillHist(cut +"/NJet_"+channel_string+"_"+label, 6., weight_ll, 8, 0., 8);


      for(unsigned int ij=0; ij<JetAllColl.size(); ij++){

	bool closelep(false);
	TString lepflav="";
	TString ptlabel="";
	bool isprompt=true;
	for(auto ilep : LeptonCollV) {
	  if(ilep->DeltaR(JetAllColl.at(ij)) < 0.4) {
	    closelep=true;
	    lepflav=ilep->GetFlavour();
	    if(ilep->Pt() < 20)ptlabel="lt20_";
	    else if(ilep->Pt() < 35)ptlabel="lt35_";
	    else ptlabel="gt35_";
	    if(!ilep->IsPrompt( GetLeptonType_JH(*ilep, gens))) isprompt=false;
	  }
	}
	
	if(!closelep){
	  if(!JetAllColl.at(ij).PassID("tight")) continue;
	  if(JetAllColl.at(ij).Pt() < 20) continue;
	}

	FillHist( cut+ "/JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);

	if(closelep)    {
	  double mc_cj_disc_corr  = (lepflav=="Electron") ? GetCorrCloseJetDisc(false, JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet)):  GetCorrCloseJetDisc(true, JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet)) ;

	  FillHist( cut+ "/CJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	  FillHist( cut+ "/CJ_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	  FillHist( cut+ "/ModCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , mc_cj_disc_corr , weight_ll, 1000,0,1);
	  FillHist( cut+ "/ModCJ_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , mc_cj_disc_corr , weight_ll, 1000,0,1);

	  if(isprompt){
	    FillHist( cut+ "/CJ_PromptLep_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	    FillHist( cut+ "/CJ_PromptLep_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	  }
	  else{
	    FillHist( cut+ "/CJ_NonPromptLep_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	    FillHist( cut+ "/CJ_NonPromptLep_JetDisc_DeepJet_"+ptlabel+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	  }

	}
	else FillHist( cut+ "/AJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);

	if(JetAllColl.at(ij).PassID("tight")) {
	  if(closelep)    FillHist( cut+ "/TightCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	  else FillHist( cut+ "/TightAJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	}
	if(BJetColl.size() > 1){
	  FillHist( cut+ "BJetEv/JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);

	  if(closelep)    FillHist( cut+ "BJetEv/CJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	  else FillHist( cut+ "BJetEv/AJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);

	  if(JetAllColl.at(ij).PassID("tight")) {
	    if(closelep)    FillHist( cut+ "BJetEv/TightCJ_JetDisc_DeepJet_"+lepflav+"_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	    else FillHist( cut+ "BJetEv/TightAJ_JetDisc_DeepJet_"+label   , JetAllColl.at(ij).GetTaggerResult(JetTagging::DeepJet) , weight_ll, 1000,0,1);
	  }
	  
	}
      }
      
      for(auto ilep : LeptonColl){

	if(ilep->Pt() > 25) FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_gt25PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
	if(ilep->Pt() > 25) 	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_gt25PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
	
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel2_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,false) , weight_ll, 200,0,200);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel3_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,false) , weight_ll, 200,0,200);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel4_"+label   , JetLeptonPtRelLepAware(*ilep,true,true,false) , weight_ll, 200,0,200);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel5_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,true) , weight_ll, 200,0,200);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel6_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,true) , weight_ll, 200,0,200);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel7"+label   , JetLeptonPtRelLepAware(*ilep,true,true,true) , weight_ll, 200,0,200);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRel8"+label   , JetLeptonPtRelLepAware(*ilep,false,false,true) , weight_ll, 200,0,200);

	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);

	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio2_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,false) , weight_ll, 200,0,2);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio3_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,false) , weight_ll, 200,0,2);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio4_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,false) , weight_ll, 200,0,2);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio5_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,true) , weight_ll, 200,0,2);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio6_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,true) , weight_ll, 200,0,2);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio7_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,true) , weight_ll, 200,0,2);
	FillHist( cut+ "/"+ilep->GetFlavour() + "Jet_PtRatio8_"+label   , JetLeptonPtRatioLepAware(*ilep,false,false,true) , weight_ll, 200,0,2);

	if(ilep->IsPrompt( GetLeptonType_JH(*ilep, gens))){
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel_"+label   , JetLeptonPtRelLepAware(*ilep) , weight_ll, 200,0,200);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel2_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,false) , weight_ll, 200,0,200);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel3_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,false) , weight_ll, 200,0,200);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel4_"+label   , JetLeptonPtRelLepAware(*ilep,true,true,false) , weight_ll, 200,0,200);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel5_"+label   , JetLeptonPtRelLepAware(*ilep,false,true,true) , weight_ll, 200,0,200);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel6_"+label   , JetLeptonPtRelLepAware(*ilep,true,false,true) , weight_ll, 200,0,200);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel7"+label   , JetLeptonPtRelLepAware(*ilep,true,true,true) , weight_ll, 200,0,200);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRel8"+label   , JetLeptonPtRelLepAware(*ilep,false,false,true) , weight_ll, 200,0,200);

	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio_"+label   , JetLeptonPtRatioLepAware(*ilep) , weight_ll, 200,0,2);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio2_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,false) , weight_ll, 200,0,2);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio3_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,false) , weight_ll, 200,0,2);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio4_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,false) , weight_ll, 200,0,2);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio5_"+label   , JetLeptonPtRatioLepAware(*ilep,false,true,true) , weight_ll, 200,0,2);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio6_"+label   , JetLeptonPtRatioLepAware(*ilep,true,false,true) , weight_ll, 200,0,2);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio7_"+label   , JetLeptonPtRatioLepAware(*ilep,true,true,true) , weight_ll, 200,0,2);
	  FillHist( cut+ "/"+ilep->GetFlavour() + "PromptLep_Jet_PtRatio8_"+label   , JetLeptonPtRatioLepAware(*ilep,false,false,true) , weight_ll, 200,0,2);

	}

      }
    }
  }
  
  return;

}


void HNL_LeptonIDBDTStudies::MakeBDTPlots(AnalyzerParameter param,HNL_LeptonCore::Channel dilep_channel, vector<Electron> ElectronColl, TString ID, double weight_ll){

  TString channel_string = GetChannelString(dilep_channel) ;

  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(ElectronColl);
  
  if(!IsData){

    vector<Electron> ElectronCollFake     = SkimLepColl(ElectronColl, gens, param, "HFake");
    FillAllElectronPlots("FakeElectron","MC"+channel_string,ElectronCollFake,weight_ll);

    vector<Electron> ElectronCollCF     = SkimLepColl(ElectronColl, gens, param, "CF");
    FillAllElectronPlots("CFElectron","MC"+channel_string,ElectronCollCF,weight_ll);

    vector<Electron> ElectronCollConv     = SkimLepColl(ElectronColl, gens, param, "NHConv");
    FillAllElectronPlots("ConvElectron","MC"+channel_string,ElectronCollConv,weight_ll);

    vector<Electron> ElectronCollPrompt   = SkimLepColl(ElectronColl, gens, param, "PromptNoCF");
    FillAllElectronPlots("PromptElectron","MC"+channel_string,ElectronCollPrompt,weight_ll);
  }
  

  if(!CheckLeptonFlavourForChannel(dilep_channel, LeptonColl)) return; // Check if EE cahnnel has 2 el                            
  Event ev = GetEvent();
  
  if(!PassTriggerSelection(dilep_channel, ev, LeptonColl,"Dilep")) return;

  if(LeptonColl.size() != 2) return;
 
  Particle METv = GetvMET("PuppiT1xyCorr"); // reyturns MET with systematic correction                                            

  std::vector<Jet> BJetColl  = GetHNLJets("BJetM", param);
  // use Z peak                                                                                                                              
  if(fabs(GetLLMass(LeptonColl)- 90.1) < 10 && METv.Pt() < 30&& BJetColl.size()==0) {
    
    if(ElectronColl[0].PassID("passPOGTight") && ElectronColl[1].PassID("passPOGTight") ){
      AnalyzerParameter paramPOG=param;
      paramPOG.Electron_ID_SF_Key = "passTightID";
      double POGSF = GetElectronSFEventWeight(ElectronColl,paramPOG);
      if(!IsData)  POGSF*= 0.95;
      FillHist(channel_string+"/"+ID+"/llmass_"+channel_string, GetLLMass(LeptonColl), weight_ll*POGSF, 70, 50., 120);
      FillHist(channel_string+"/"+ID+"/NVTX_"+channel_string, nPileUp, weight_ll*POGSF, 100, 0., 100);
      FillHist(channel_string+"/"+ID+"/MET_"+channel_string, GetvMET("PuppiT1xyCorr").Pt(), weight_ll*POGSF, 100, 0., 100);
    }
    
    for(auto iel : ElectronColl) {
      if(iel.PassID("HNL_ULID_Baseline")){
	
	if(SameCharge(ElectronColl))FillHist( channel_string+"_SS/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");
	else FillHist( channel_string+"_OS/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");
	
	if(SameCharge(ElectronColl))FillHist( channel_string+"_SS/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");
	else FillHist( channel_string+"_OS/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");
	
	if(SameCharge(ElectronColl))FillHist( channel_string+"_SS/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");
	else FillHist( channel_string+"_OS/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");

	if(iel.PassID("HNL_ULID_Fake")){
	  if(SameCharge(ElectronColl))FillHist( channel_string+"_SS_WithMVAFake/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");
	  else FillHist( channel_string+"_OS_WithMVAFake/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");

	  if(SameCharge(ElectronColl))FillHist( channel_string+"_SS_WithMVAFake/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");
	  else FillHist( channel_string+"_OS_WithMVAFake/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");

	  if(SameCharge(ElectronColl))FillHist( channel_string+"_SS_WithMVAFake/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");
	  else FillHist( channel_string+"_OS_WithMVAFake/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");
  
	}
      }
    }
    

    std::vector<FatJet>   AK8_JetColl = GetHNLAK8Jets("", param);
    std::vector<Jet>      JetColl     = GetHNLJets("Tight", param);

    if(BJetColl.size() > 1) return;
    if(METv.Pt() > 30) return;
    if(!ZmassOSSFWindowCheck(LeptonColl, 10.)) return;

    FillAllElectronPlots("OSElectron","Z",ElectronColl,weight_ll);

    vector<Electron> ElectronCollDATATight;
    vector<Electron> ElectronCollDATAFake;
    for(auto iel : ElectronColl){
      if(iel.IsGsfCtfScPixChargeConsistent() && iel.SIP3D() < 3) ElectronCollDATATight.push_back(iel);
      if( iel.SIP3D() > 5) ElectronCollDATAFake.push_back(iel);
    }
    FillAllElectronPlots("OSElectron","ZTight",ElectronCollDATATight,weight_ll);
    FillAllElectronPlots("OSElectron","ZFake" ,ElectronCollDATAFake,weight_ll);

    
    std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
    std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);
    
    
    std::vector<Tau> TauColl;

    Fill_RegionPlots(dilep_channel, 0, param.Name,"ZPeak" ,  TauColl, JetColl, AK8_JetColl, LeptonColl,  GetvMET("PuppiT1xyCorr"), nPV, weight_ll,param.WriteOutVerbose);
  }

  return;

}



HNL_LeptonIDBDTStudies::HNL_LeptonIDBDTStudies(){


}
 
HNL_LeptonIDBDTStudies::~HNL_LeptonIDBDTStudies(){

}

