#include "HNL_FakeRate.h"

void HNL_FakeRate::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

  TString AnalyzerPath=getenv("SKFlat_WD"), SKFlatV = getenv("SKFlatV");
  TString FilePath="/data/"+SKFlatV+"/"+GetEra()+"/FakeRate/NvtxSF/";
  NvtxSFFile = new TFile(AnalyzerPath+FilePath+"FakeRateNVtx13TeV_"+GetEra()+".root");

}

void HNL_FakeRate::executeEvent(){

  Event ev = GetEvent();

  AnalyzerParameter param;

  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ELIDs;
  vector<HNL_LeptonCore::Channel> channel;
  vector<TString> paramnames;
  
  TString           era="2016";
  if(DataYear==2017)era="2017";
  if(DataYear==2018)era="2018";
  
  // Loose ID has no loose IP
  // HNTight_17028 :  PassMVA(0.9, 0.825, 0.5)) && fabs(dXY())<0.01 && fabs(dZ())<0.04 && fabs(IP3D()/IP3Derr())<4. RelIso()<0.08
  //                  PassConversionVeto() IsGsfCtfScPixChargeConsistent Pass_TriggerEmulation
  // HNLoose_17028 : Pass_HNLoose2016(0.6, 0.2, 0.1, 10.) && MVA(-0.1, 0.1, -0.1) PassConversionVeto() IsGsfCtfScPixChargeConsistent Pass_TriggerEmulation

  /*ELIDs.push_back(make_pair("HNTight_17028", "HNLoose_17028")); 
  ELIDs.push_back(make_pair("HNTightV2",    "HNLooseV4")); 
  ELIDs.push_back(make_pair("passPOGTight", "HNLoosePOG"));
  ELIDs.push_back(make_pair("passPOGMedium","HNLoosePOG"));
  ELIDs.push_back(make_pair("passMVAID_noIso_WP80",   "passMVAID_noIso_WP80Loose"));
  ELIDs.push_back(make_pair("passMVAID_noIso_WP90",   "passMVAID_noIso_WP90Loose"));
  ELIDs.push_back(make_pair("HN2016POG",   "HNLoosePOG"));
  ELIDs.push_back(make_pair("HN2016POGCC",   "HNLoosePOG"));
  ELIDs.push_back(make_pair("HN2016MVA",   "HNLooseMVA"));
  ELIDs.push_back(make_pair("HN2016MVA2",   "HNLooseMVA"));
  ELIDs.push_back(make_pair("HN2016MVA2CC",   "HNLooseMVA"));
ç
  paramnames.push_back("HNTight_17028"  );
  paramnames.push_back("HNTightV2"      );
  paramnames.push_back("passPOGTight"  );
  paramnames.push_back("passPOGMedium"  );
  paramnames.push_back("passMVAID_noIso_WP80"  );
  paramnames.push_back("passMVAID_noIso_WP90"  );
  paramnames.push_back("HN2016POG"  );
  paramnames.push_back("HN2016POGCC"  );
  paramnames.push_back("HN2016MVA"  );
  paramnames.push_back("HN2016MVA2"  );
  paramnames.push_back("HN2016MVA2CC"  );*/

  bool ElectronOpt=true;
  if (ElectronOpt){
    vector<TString> vTrig = {"LooseTrig_","TightTrig_"};                                                                                 
    vector<TString> vConv = {"","ConvBEC_"};
    vector<TString> vCC = {"CCBEC_"};
    vector<TString> vMVAB;
    vector<TString> vMVAEC;
    for(unsigned int imva=0 ; imva < 70 ; imva++){
      double mva_d= -0.5 + double(imva)*0.02;
      std::string mvaS= std::to_string(mva_d);
      vMVAB.push_back("MVAB"+mvaS+"_");
    }
    for(unsigned int imva=0 ; imva < 85 ; imva++){
      double mva_d= -0.8 + imva*0.02;
      std::string mvaS= std::to_string(mva_d);
      vMVAEC.push_back("MVAEC"+mvaS+"_");
    }
    
    vector<TString> OptElectronsIDs;
    for(auto iTrig : vTrig){
      for(auto iConv : vConv){
	for(auto iCC : vCC){
	  for(auto iMVAB : vMVAB) OptElectronsIDs.push_back(iTrig+iConv+iCC+iMVAB+"MVAEC-0.8_ISOB0.2_ISOEC0.2_DXYB1EC1");
	  for(auto iMVAEC : vMVAEC) OptElectronsIDs.push_back(iTrig+iConv+iCC+iMVAEC+"MVAB-0.5_ISOB0.2_ISOEC0.2_DXYB1EC1");
	  OptElectronsIDs.push_back(iTrig+iConv+iCC+"POGT_ISOB0.2_ISOEC0.2_DXYB1EC1");
	  OptElectronsIDs.push_back(iTrig+iConv+iCC+"POGM_ISOB0.2_ISOEC0.2_DXYB1EC1");
	  OptElectronsIDs.push_back(iTrig+iConv+iCC+"POGTNoIso_ISOB0.2_ISOEC0.2_DXYB1EC1");
	  OptElectronsIDs.push_back(iTrig+iConv+iCC+"POGMNoIso_ISOB0.2_ISOEC0.2_DXYB1EC1");
	}
      }
    }
    
    for (auto i: OptElectronsIDs) {
      paramnames.push_back("El_"+i);     
      ELIDs.push_back(make_pair("ElOpt_"+i, "ElOptLoose_"+i));
    }
    for (auto i: ELIDs) {
      channel.push_back(EE);
      MuIDs.push_back(make_pair("HNVeto2016","HNLoose_17028"));
    }
    
    if( paramnames.size() != ELIDs.size()) return;
  }

  

  // MUON IDS
  /*MuIDs.push_back(make_pair("HNTightPFIsoMedium","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTightPFIsoTight","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTightPFIsoVeryTight","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTightPFIsoVeryVeryTight","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTight_17028","HNLoose_17028"));
  MuIDs.push_back(make_pair("HNTightV2","HNLooseV1"));
  MuIDs.push_back(make_pair("HNLooseMVA","HNLooseMVA"));
  
  paramnames.push_back("POGTightWithMediumIso");
  paramnames.push_back("POGTightWithTightIso");
  paramnames.push_back("POGTightWithVeryTightIso");
  paramnames.push_back("POGTightWithVeryVeryTightIso");
  paramnames.push_back("HNTight_17028");
  paramnames.push_back("HNTightV2");
  paramnames.push_back("HNMVA");*/


  bool MuonOpt=true;
  if (MuonOpt){
    vector<TString> vMVAB;
    vector<TString> vMVAEC;
    for(unsigned int imva=0 ; imva < 65 ; imva++){
      double mva_d= -0.4 + double(imva)*0.02;
      std::string mvaS= std::to_string(mva_d);
      vMVAB.push_back("MVAB"+mvaS+"_");
    }
    for(unsigned int imva=0 ; imva < 80 ; imva++){
      double mva_d= -0.7 + imva*0.02;
      std::string mvaS= std::to_string(mva_d);
      vMVAEC.push_back("MVAEC"+mvaS+"_");
    }

    vector<TString> OptMuonsIDs;
    for(auto iMVAB : vMVAB) OptMuonsIDs.push_back(iMVAB+"MVAEC-0.7_ISOB0.2_ISOEC0.2_DXYB1EC1");
    for(auto iMVAEC : vMVAEC) OptMuonsIDs.push_back(iMVAEC+"MVAB-0.4_ISOB0.2_ISOEC0.2_DXYB1EC1");
    OptMuonsIDs.push_back("POGT_ISOB0.2_ISOEC0.2_DXYB1EC1");
    OptMuonsIDs.push_back("POGM_ISOB0.2_ISOEC0.2_DXYB1EC1");
    
    for (auto i: OptMuonsIDs){
      paramnames.push_back("Mu_"+i);
      MuIDs.push_back(make_pair("MuOpt_"+i, "MuOptLoose_"+i));
    }

  }
  
  int iel= MuIDs.size() - channel.size();
  for (int i =0; i < iel; i++){
    channel.push_back(MuMu);
    ELIDs.push_back(make_pair("HNVeto2016","HNLoose_17028"));
  }

  if( paramnames.size() != ELIDs.size()) return;


  for(unsigned int it_id=0; it_id<ELIDs.size(); it_id++){
    
    //************************************************///
    // setup leptton veto/loose/tight 
    //************************************************///

    TString MuonTightID      = MuIDs[it_id].first;
    TString MuonLooseID      = MuIDs[it_id].second;
    TString ElectronTightID  = ELIDs[it_id].first;
    TString ElectronLooseID  = ELIDs[it_id].second;

    TString FakeRateID       = (channel[it_id] == MuMu) ? MuonLooseID :  ElectronLooseID;
    TString Tight_ID         = (channel[it_id] == MuMu) ? MuonTightID :  ElectronTightID;
    
    param.Clear();
  
    param.syst_ = AnalyzerParameter::Central;
    param.Name = paramnames[it_id];
    param.MCCorrrectionIgnoreNoHist = false;

   
    //==== Muon IDparam.Muon_Loose_ID
    param.Muon_Tight_ID        = MuonTightID;
    param.Muon_Loose_ID        = MuonLooseID;

    //==== Electron ID
    param.Electron_Tight_ID       = ElectronTightID;
    param.Electron_Loose_ID       = ElectronLooseID;

    //==== Jet ID
    param.Jet_ID    = "tight";

    //    param.WriteOutVerbose = -1;  Run All IDs and make All Region plots
    //    param.WriteOutVerbose = -2;  Run All IDs and make only FR
    //    param.WriteOutVerbose = -3;  Run All IDs and make only FR + PR
    //     param.WriteOutVerbose = 1;  Run Main ID and make All Region plots
    //     param.WriteOutVerbose = 2;  Run Main ID and make  only FR 
    //     param.WriteOutVerbose = 3;  Run Main ID and make  only FR + PR
    //     param.WriteOutVerbose = 0;  makes NVertx plots
    param.WriteOutVerbose= -2; // 0 means only make FR  1 means FR+PR  2 means SR+PR + CR plots  3 means makes NVertx plots
   
    
    if(param.WriteOutVerbose >=0){
      if (channel[it_id]==MuMu && MuonTightID != "HNTightV2") continue;
      if (channel[it_id]==EE   && ElectronTightID != "HNTightV2") continue;
    }
    
    param.Name = GetChannelString(channel[it_id]) + "_"+ paramnames[it_id];
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  } 
}

void HNL_FakeRate::executeEventFromParameter(AnalyzerParameter param, TString El_ID, HNL_LeptonCore::Channel channel){

  
  //cout << "executeEventFromParameter " << GetChannelString(channel) << " " << El_ID  << endl;

  //************************************************///
  // setup event level objects
  Event ev = GetEvent();

  double weight = SetupWeight(ev, param) / ev.GetTriggerLumi("Full");
  if(IsData) weight = 1;
  
  if(!PassMETFilter()) return;

  
  std::vector<Electron> loose_electrons     = GetElectrons( param,param.Electron_Loose_ID, 9.5, 2.5,false) ;
  std::vector<Muon>     loose_muons         = GetMuons    ( param,param.Muon_Loose_ID, 5, 2.4, false);
  
  std::vector<Jet> jets_tmp     = GetJets   ( param, "tight", 30., 2.7);
  std::vector<Jet> jets; 
  for(unsigned int ijet =0; ijet < jets_tmp.size(); ijet++){
    bool jetok=true;
    
    for(unsigned int iel=0 ; iel < loose_electrons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_electrons[iel]) < 0.4) jetok = false;
    }
    for(unsigned int iel=0 ; iel < loose_muons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_muons[iel]) < 0.4) jetok = false;
    }
    if(jetok) jets.push_back(jets_tmp[ijet]);
  }

  if(channel==EE) {

    RunE(loose_electrons,loose_muons, jets,  param, weight);
    
  }
  if(channel==MuMu){
    RunM(loose_electrons,loose_muons,  jets, param, weight);
  }
}


void HNL_FakeRate::RunM(std::vector<Electron> loose_el,  std::vector<Muon> loose_mu, std::vector<Jet> jets,   AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleEG") return;
    if(this->DataStream == "SingleElectron") return;
    if(this->DataStream == "EGamma") return;
  }

  TString label    = param.Name;
  TString channel_s="_MuMu";
  Event ev = GetEvent();

  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  if(loose_mu.size() == 0) return;

  
  // remove if muon                                                                                                                          
  if(loose_el.size() > 0) return;


  double isocut = GetIsoFromID("Muon", param.Muon_Tight_ID,loose_mu[0].Eta(), loose_mu[0].Pt());
  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_mu);
  std::vector<bool> blepsT;
  for(auto ilep : loose_mu) {
    blepsT.push_back(ilep.PassID(param.Muon_Tight_ID));
  }

  if(param.WriteOutVerbose == 0) {
    MakeNVertexDistPrescaledTrig(MuMu,param, ev, leps,blepsT,param.Name+channel_s,event_weight);
    return;
  }

  
  if(fabs(param.WriteOutVerbose) > 1 ){
    GetFakeRateAndPromptRates(param, leps,blepsT,jets,param.Name+channel_s,event_weight,isocut);
    return;
  }

  if (param.Muon_Tight_ID != "HNTightV2") return;

  MakeDiLepPlots(MuMu,param, ev, leps,blepsT,param.Name+channel_s,event_weight);

  if(loose_mu.size() != 1) return;
  
  bool has_away_jet=false;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(loose_mu[0].Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      has_away_jet=true;
    }
  }

  /// Make plots to check control of prompt leptons and to plot Loose and Tight Samples
  /// Only run when not running MANY IDs so param.WriteOutVerbose is set to 2

  // prescale_trigger is to to weight of trigger prescalee depedning on the pt
  float prescale_trigger = GetPrescale(leps);
  if(prescale_trigger == 0.) return;
  event_weight*=prescale_trigger;


  bool truth_match= false;
  if(!IsDATA) {
    gens = GetGens();
    if(GetLeptonType_JH(loose_mu.at(0), gens) > 0 ) truth_match=true;
  }
  else truth_match=true;

  if(jets.size() > 0){
    if(jets.at(0).Pt() > 40.){
      
      // Plot for 1  lep + Jet pT > trig pt
      FillRegionPlots("MuMu", "SingleLooseMuJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(has_away_jet)       FillRegionPlots("MuMu","SingleLooseMuAwayJet_"+param.Name, jets,   loose_el,  loose_mu,  METv, event_weight);
      if(blepsT[0]){
        FillRegionPlots("MuMu","SingleTightMuJet_"+param.Name,  jets,  loose_el,loose_mu,  METv, event_weight);
        if(has_away_jet) FillRegionPlots("MuMu","SingleTightMuAwayJet_"+param.Name, jets,  loose_el, loose_mu,  METv, event_weight);
      }
    }
  }

  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int imu = 0; imu < loose_mu.size();  imu++){
    METdphi=  TVector2::Phi_mpi_pi((loose_mu.at(imu).Phi()- METv.Phi()));
    MT = sqrt(2.* loose_mu.at(imu).Et()*METv.Pt() * (1 - cos( METdphi)));
  }
  if((60. < MT)  &&(MT < 100.)){
    if(loose_mu[0].Pt() > 20){
      if(jets.size() > 0 && blepsT[0]){
	if(jets[0].Pt() > 40){
	  
	  FillRegionPlots("MuMu","SingleTightMu_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	  if(has_away_jet)FillRegionPlots("MuMu","SingleTightMuAwayJet_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);

	  bool isPrompt=false;
	  for(unsigned int ij=0; ij < jets.size(); ij++){
	    if(jets.at(ij).Pt() < 40.) continue;
	    float dphi =fabs(TVector2::Phi_mpi_pi(loose_mu[0].Phi()- jets.at(ij).Phi()));
	    if(dphi > 2.5){
	      if((jets.at(ij).Pt() /  loose_mu[0].Pt() ) < 1.) isPrompt = true;
	    }
	  }
	  
	  if (isPrompt) FillRegionPlots("MuMu","SingleTightMu_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	  if (isPrompt&&has_away_jet) FillRegionPlots("MuMu","SingleTightMuAwayJet_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	}
	
      }
    }
  }

  
  bool useevent40 = UseEvent(leps , jets, 40., METv, event_weight);
  if(useevent40){
    if (blepsT[0] && jets.size() >= 1)FillRegionPlots("MuMu","TightMu40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if (jets.size() >= 1)FillRegionPlots("MuMu","LooseMu40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    
    if(truth_match){
      if (blepsT[0] && jets.size() >= 1)FillRegionPlots("MuMu","TightMu40_"+param.Name , jets,  loose_el,loose_mu, METv, event_weight);
      if (jets.size() >= 1)FillRegionPlots("MuMu","LooseMu40_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    }
    
  }
  
  return;

} /// end function                      





void HNL_FakeRate::RunE( std::vector<Electron> loose_el, std::vector<Muon> loose_mu, std::vector<Jet> jets, AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleMuon") return;
    if(this->DataStream == "SingleMuon") return;
  }

  TString channel_s="_EE";
  TString label = param.Name;
  Event ev = GetEvent();

  Particle METv = GetvMET("PuppiT1xyULCorr",param);
  
  if(loose_el.size() == 0) return;
 

  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                                     
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_33="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"; /// 

  // remove if muon 
  if(loose_mu.size() > 0) return;

  double isocut = GetIsoFromID("Electron", param.Electron_Tight_ID,loose_el[0].Eta(), loose_el[0].Pt());
  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_el);
  std::vector<bool> blepsT;
  for(auto ilep : loose_el) {
    blepsT.push_back(ilep.PassID(param.Electron_Tight_ID));
  }

  if(param.WriteOutVerbose == 0) {
    MakeNVertexDistPrescaledTrig(EE,param, ev, leps,blepsT,param.Name+channel_s,event_weight);
    return;
  }

  if(fabs(param.WriteOutVerbose) > 1) {
    GetFakeRateAndPromptRates(param, leps,blepsT,jets,param.Name+channel_s,event_weight,isocut);
    return;
  }


  if (param.Electron_Tight_ID != "HNTightV2") return;

  MakeDiLepPlots(EE,param, ev, leps,blepsT,param.Name+channel_s,event_weight);
  
  if(loose_el.size() != 1) return;

  bool has_away_jet=false;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(loose_el[0].Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      has_away_jet=true;
    }
  } 


  float prescale_trigger =  GetPrescale(leps);
  if(prescale_trigger == 0.) return;
  event_weight*=prescale_trigger;



  bool truth_match= false;
  if(!IsDATA) {
    gens = GetGens();
    if(GetLeptonType(loose_el.at(0), gens) > 0 ) truth_match=true;
  }
  else truth_match=true;

  if(jets.size() > 0){
    if(jets.at(0).Pt() > 40.){
      FillRegionPlots("EE","SingleLooseElJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(has_away_jet)      FillRegionPlots("EE","SingleLooseElAwayJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(blepsT[0]){
        FillRegionPlots("EE","SingleTightElJet_"+param.Name,  jets,  loose_el,loose_mu,  METv, event_weight);
        if(has_away_jet) FillRegionPlots("EE","SingleTightElAwayJet_"+param.Name, jets,  loose_el, loose_mu,  METv, event_weight);
      }
    }
  }

  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int iel = 0; iel < loose_el.size();  iel++){
    METdphi = TVector2::Phi_mpi_pi((loose_el.at(iel).Phi()- METv.Phi()));
    MT = sqrt(2.* loose_el.at(iel).Et()*METv.Pt() * (1 - cos( METdphi)));
  }
  if((60. < MT)  &&(MT < 100.)){
    if(loose_el[0].Pt() > 25){
      if(jets.size() > 0 && blepsT[0]){
	if(jets[0].Pt() > 40){
	  FillRegionPlots("EE","SingleTightEl_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	  if(has_away_jet)	  FillRegionPlots("EE","SingleTightElAwayJet_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);

	  bool isPrompt=false;
          for(unsigned int ij=0; ij < jets.size(); ij++){
	    if(jets.at(ij).Pt() < 40.) continue;
            float dphi =fabs(TVector2::Phi_mpi_pi(loose_el[0].Phi()- jets.at(ij).Phi()));
            if(dphi > 2.5){
              if((jets.at(ij).Pt() /  loose_el[0].Pt() ) < 1.) isPrompt = true;
	    }
	  }
	  
          if (isPrompt ) FillRegionPlots("EE","SingleTightEl_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	  if (isPrompt && has_away_jet) FillRegionPlots("EE","SingleTightElAwayJet_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	}
      }
    }
  }
  


  bool useevent40 = UseEvent(leps , jets, 40., METv,event_weight);
  if(useevent40){
    if (blepsT[0] && jets.size() >= 1)FillRegionPlots("EE","TightEl40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if (jets.size() >= 1)FillRegionPlots("EE","LooseEl40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    
    
    if(truth_match){
      if (blepsT[0] && jets.size() >= 1)FillRegionPlots("EE","TightEl40_"+param.Name , jets,  loose_el,loose_mu, METv, event_weight);
      if (jets.size() >= 1)FillRegionPlots("EE","LooseEl40_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    }
  }
  
  
  return;
  


} /// end function RunE

double HNL_FakeRate::ApplyNvtxReweight(int NPV, TString Key){

  if(IsData) return 1.;
  if(!NvtxSFFile->GetListOfKeys()->Contains(Key)){ printf("[Error] No %s in NvtxSF File.\n", Key.Data()); return -1.; }

  std::map<TString, TH1D*>::iterator mapit = maphist_NvtxSF.find(Key);
  if(mapit == maphist_NvtxSF.end()){ //first usage
    maphist_NvtxSF[Key] = (TH1D*) ((TH1*) NvtxSFFile->Get(Key))->Clone();
  }
  mapit = maphist_NvtxSF.find(Key);
  
  int this_bin = mapit->second->FindBin(NPV);
  double NvtxSF = mapit->second->GetBinContent(this_bin);
  if(NvtxSF>10) NvtxSF=1.;

  return NvtxSF;

}


void HNL_FakeRate::MakeDiLepPlots(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){

  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;

  // now we have 2 Tight leptons                                                                                                                             

  TString plot_dir = GetChannelString(channel);

  // Make Z peak                                                                                                                                            
  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;
  bool PassZMass = (fabs(90. - Z.M()) < 10.) ? true : false;

  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl    = GetBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )event_weight*= sf_btag;

  if(BJetColl.size() > 0) return;

  int nbin_pt    =6;
  double bins_pt[nbin_pt+1] = {0.,5., 10., 20., 30., 50., 200. };

  if(channel == MuMu){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    double NVxt_Mu3 =  ApplyNvtxReweight(nPV,triggerslist_3);
    double NVxt_Mu8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_Mu17 =  ApplyNvtxReweight(nPV,triggerslist_17);

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;


    if(pass_3){
      if(leps[1]->Pt() > 5){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3)*NVxt_Mu3;
	
        if(PassZMass) {
	  for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    
    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8;
	
        if(PassZMass){
	  for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_Mu17;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }
  
  if(channel == EE){

    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;
    double NVxt_El8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
    double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);


    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_El8;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_12){
      if(leps[1]->Pt() > 14){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }

    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_23){
      if(leps[1]->Pt() > 25){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }

  return;


}



void HNL_FakeRate::MakeNVertexDistPrescaledTrig(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){
  

  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;
  
  // now we have 2 Tight leptons
  // Make Z peak 

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;


  // remove b jet 

  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl    = GetBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )event_weight*= sf_btag;

  if(BJetColl.size() > 0) return;

  bool PassZMass = (fabs(90. - Z.M()) < 10.) ? true : false;


  // Now plot Z peak for

  int nbin_npv    =35;
  double bins_npv[nbin_npv+1] = { 0.,5., 10.,12., 14., 16., 18., 20., 22., 24., 26., 28., 30., 32., 34., 36., 38., 40., 42., 44., 46., 48., 50., 52., 54., 56., 58., 60., 62., 64., 66., 68., 70., 75., 80., 100.};

  int nbin_pt    =6;
  double bins_pt[nbin_pt+1] = {0.,5., 10., 20., 30., 50., 200. };

  if(channel == MuMu){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;


    if(pass_3){
      if(leps[1]->Pt() > 5){
	double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3);

	if(PassZMass) {
	  FillHist(( triggerslist_3 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
	FillHist(( triggerslist_3 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_8){
      if(leps[1]->Pt() > 10){
	double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8);

	if(PassZMass){
	  FillHist(( triggerslist_8 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
        FillHist(( triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
	double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17);
	if(PassZMass){
	  FillHist(( triggerslist_17 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	} 
	FillHist(( triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }   
  }
  


  if(channel == EE){

    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;


    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8);
	if(PassZMass){
	  FillHist(( triggerslist_8 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
        FillHist(( triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_12){
      if(leps[1]->Pt() > 14){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12);
	if(PassZMass){
	  FillHist(( triggerslist_12 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
	FillHist(( triggerslist_12 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
	      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17);
	if(PassZMass){
	  FillHist(( triggerslist_17 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
	FillHist(( triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_23){
      if(leps[1]->Pt() > 25){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23);
	if(PassZMass){
	  FillHist(( triggerslist_23 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
        FillHist(( triggerslist_23 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }
  
  return;
  

}
void HNL_FakeRate::GetFakeRateAndPromptRates(AnalyzerParameter param, std::vector<Lepton *> leps,std::vector<bool> blepsT, std::vector<Jet>    jetCollTight, TString label, float event_weight, float isocut){
						      
  
  if (leps.size()<1) return;
  
  Event ev = GetEvent();
  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  
  if(leps.size()==1){
    MakeFakeRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, isocut, METv);
  }

  if(fabs(param.WriteOutVerbose) < 3) return;

  if(leps.size()==2){
    TString dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v";
    if(DataYear==2017) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";

    if(DataYear==2018) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";

    

    if(IsDATA&& !(ev.IsPDForTrigger(dilep_triggerslist, this->DataStream) )) return;
    if(ev.PassTrigger(dilep_triggerslist))    MakePromptRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, isocut, METv);
  }
  return;
}


void HNL_FakeRate::MakePromptRatePlots(TString label, TString tag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT , std::vector<Jet> jets,  float event_weight, float isocut, Particle MET){
  
  int nbin_ptcone=11;
  int nbin_pt    =11;
  int nbin_eta   =4;
  double ptbinscone[nbin_ptcone+1] = { 10.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};
  double ptbins    [nbin_pt    +1] = { 10.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};
  double etabins   [nbin_eta+1   ] = { 0.,0.8,  1.479, 2.,  2.5};

  Event ev = GetEvent();

  if(leps.size() != 2) return;

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;
  if(fabs(90. - Z.M()) > 10.) return;

  if(blepsT[1]){
    // lep [1] is tag
    int ilep=0;
    float lep_pt = leps[ilep]->Pt();
    float lep_pt_corr =  leps[ilep]->CalcPtCone(leps[ilep]->RelIso(), isocut);
    float lep_eta = fabs(leps[ilep]->Eta());
    if(lep_pt_corr > 2000.) lep_pt_corr = 1999.;
    float weight_ptcorr=event_weight;
    TString L_prefix = "Prompt_Loose"+tag ;
    TString T_prefix = "Prompt_Tight"+tag;
    
    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHist((prefix + "_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      if(fabs(90. - Z.M()) < 5){
	FillHist((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
	FillHist((prefix + "_TZ_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      }
    }
  }
  if(blepsT[0]){
    // lep [0] is tag                                                                                                                                                             
    int ilep=1;
    float lep_pt = leps[ilep]->Pt();
    float lep_pt_corr =  leps[ilep]->CalcPtCone(leps[ilep]->RelIso(), isocut);
    float lep_eta = fabs(leps[ilep]->Eta());
    if(lep_pt_corr > 2000.) lep_pt_corr = 1999.;
    float weight_ptcorr=event_weight;
    TString L_prefix = "Prompt_Loose"+tag ;
    TString T_prefix = "Prompt_Tight"+tag;

    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHist((prefix + "_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      if(fabs(90. - Z.M()) < 5){
	FillHist((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
        FillHist((prefix + "_TZ_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      }
    }
  }


  return;
}


float HNL_FakeRate::GetPrescale(std::vector<Lepton *>   leps  ){

  if(leps.size() != 1) return 0.;

  Event ev = GetEvent();
  float prescale_trigger= 1.;
  if(leps[0]->LeptonFlavour() == Lepton::MUON){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;
  
    double NVxt_Mu3 =  ApplyNvtxReweight(nPV,triggerslist_3);
    double NVxt_Mu8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_Mu17 =  ApplyNvtxReweight(nPV,triggerslist_17);

    /// 10 - 20  HLT_Mu7
    /// 20 - INF  HLT_Mu17
    
    if(leps.at(0)->Pt() >= 20.){
      if(pass_17) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_Mu17;
      else return 0;
    }
    else  if(leps.at(0)->Pt() >= 10.){

      if(pass_8)prescale_trigger =  (IsDATA) ? 1. :   ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8 ; //// 20 + GeV bins
      else return 0;
    }
    else  if(leps.at(0)->Pt() >= 5.){
      if(pass_3)prescale_trigger =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3)*NVxt_Mu3 ; //// 20 + GeV bins                                                                             
      else return 0 ;
    }      
    if(prescale_trigger == 0.) return 0.;
    if(leps.at(0)->Pt() < 5.) return 0.;
    
    return prescale_trigger;
  }
  else{
    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";  
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;

    double NVxt_El8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
    double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);

 
    if(leps.at(0)->Pt() >= 25.){
      if(pass_23)prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
      else return 0;
    }
    else   if(leps.at(0)->Pt() >= 20.){
      if(pass_17)prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_El12;
      else return 0;
    }
    else   if(leps.at(0)->Pt() >= 15.){
      if(pass_12) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
      else return 0;
    }
    else   if(leps.at(0)->Pt() >= 9.5){
      if(pass_8) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_El8;
      else return 0;
    }
    else prescale_trigger = 0.;
    if(prescale_trigger == 0.) return 0.;
    if(leps.at(0)->Pt() < 9.5) return 0.;
    return prescale_trigger;

  }
  
  return prescale_trigger;
}
 

void HNL_FakeRate::MakeFakeRatePlots(TString label, TString mutag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT ,  std::vector<Jet> jets,  float event_weight, float isocut, Particle MET){
				
  if(!IsDATA)gens = GetGens();


  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT MUONS                                                                                                                                                  
  bool truth_match= false;

  if(!IsDATA) {
    if(leps.size() > 0){
      if(GetLeptonType(*leps[0], gens) > 0 ) truth_match=true;
    }
  }
  else truth_match=true;
  if(!truth_match) return;

  bool useevent20 = UseEvent(leps , jets, 20.,  MET, event_weight);
  bool useevent30 = UseEvent(leps , jets, 30.,  MET, event_weight);
  bool useevent40 = UseEvent(leps , jets, 40.,  MET, event_weight);
  bool useevent60 = UseEvent(leps , jets, 60.,  MET, event_weight);


  label= mutag;
  if(jets.size() >= 1){
    if(useevent20)GetFakeRates(leps, blepsT, param, label, jets,  label+"_20",(event_weight),isocut);
    if(useevent30)GetFakeRates(leps, blepsT,param, label, jets,   label+"_30",(event_weight),isocut);
    if(useevent40)GetFakeRates(leps, blepsT,param, label, jets,  label+"_40",(event_weight),isocut);
    if(useevent60)GetFakeRates(leps, blepsT,param, label,jets,   label+"_60",(event_weight),isocut);
  }
  return;
}


bool HNL_FakeRate::UseEvent(std::vector<Lepton *> leps ,  std::vector< Jet> jets, float awayjetcut, Particle MET, float wt){


  bool useevent = false;
  if(leps.size() != 1) return false;
  
  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int w = 0; w < leps.size();  w++){
    METdphi = TVector2::Phi_mpi_pi(leps.at(w)->Phi()-MET.Phi());
    MT = sqrt(2.* leps.at(w)->Et()*MET.Pt() * (1 - cos( METdphi)));
    if(( (MET.Pt() < 40) && (MT < 25.)) ) {

      for (unsigned int ielT=0; ielT < leps.size(); ielT++){
        for(unsigned int ij=0; ij < jets.size(); ij++){
          if(jets.at(ij).Pt() < awayjetcut) continue;
          float dphi =fabs(TVector2::Phi_mpi_pi(leps.at(ielT)->Phi()- jets.at(ij).Phi()));
	  if( (jets.at(ij).ChargedEmEnergyFraction()) > 0.65)  continue;

          if(dphi > 2.5){
            if((jets.at(ij).Pt() /  leps.at(ielT)->Pt() ) < 1.2) continue;
            useevent = true;
          }
	}
      }
    }
  }

  return useevent;

}


void HNL_FakeRate::GetFakeRates(std::vector<Lepton *> leps,std::vector<bool> blepsT,  AnalyzerParameter param,TString tightlabel,  std::vector<Jet> jets,  TString tag,float event_weight, float isocut){
					 
  bool IsMuon=(leps[0]->LeptonFlavour() == Lepton::MUON);
  int nbin_ptcone=  IsMuon ? 10 : 8;
  int nbin_pt    = IsMuon ? 10 : 9;
  int nbin_eta   = 4;
  
  double ptbinscone  [nbin_ptcone+1];
  double ptbins      [nbin_pt+1];
  double etabins   [nbin_eta+1   ] =  { 0.,0.8,  1.479, 2.,  2.5};
  if(IsMuon){
    vector<double> vptbinscone = {6.,10., 15.,20.,25.,30.,35.,40.,50., 60.,100. } ;
    vector<double> vptbins =  { 5., 10., 15.,20.,25.,30.,35.,40.,50.,60.,100.} ; 
    std::copy(vptbinscone.begin(), vptbinscone.end(), ptbinscone);
    std::copy(vptbins.begin(), vptbins.end(), ptbins);
  }
  else{
    vector<double> vptbinscone = { 10., 15.,23.,30.,35., 40.,50.,60.,100.};
    vector<double> vptbins     = { 10., 15.,20.,25.,30., 35.,40.,50.,60.,100.};
    std::copy(vptbinscone.begin(), vptbinscone.end(), ptbinscone);
    std::copy(vptbins.begin(), vptbins.end(), ptbins);

  }
  Event ev = GetEvent();

  float lep_pt = leps[0]->Pt();
  float lep_pt_corr =  leps[0]->CalcPtCone(leps[0]->RelIso(), isocut);
  float lep_eta =   fabs(leps[0]->Eta());
  float lep_reliso  = leps[0]->RelIso();
  float lep_ip3d    = fabs(leps[0]->IP3D()/leps[0]->IP3Derr());
  float lep_mva     =  leps[0]->lep_mva();
  float lep_dxy     = fabs(leps[0]->dXY());
  if(lep_pt_corr > 100.) lep_pt_corr = 99.;

  float weight_ptcorr=event_weight;
  float weight_pt=event_weight;
  TString L_prefix = "Fake_Loose"+tag;
  TString T_prefix =  "Fake_Tight"+tag;

  double prescale_lep (1.);

  bool fill_plot=false;

  if(IsMuon){
    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";
    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    if(lep_pt_corr< 5) return;
    else if (lep_pt_corr > 5  && lep_pt_corr < 15){
      if(lep_pt > 5.) {
	if(Mu3PD&&ev.PassTrigger(triggerslist_3)) { fill_plot=true; if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_3);}
	else { fill_plot=false;weight_ptcorr = 0.;	}
      }
    }
    
    else if ( lep_pt_corr < 30){
      if(lep_pt > 10) {

	if(Mu8PD&&ev.PassTrigger(triggerslist_8)) {  fill_plot=true;if(!IsDATA)weight_ptcorr= event_weight * ev.GetTriggerLumi(triggerslist_8); }
	else{ fill_plot=false;weight_ptcorr = 0.;      }
      }
    }
    
    else {
      if(lep_pt > 20) {
	if(Mu17PD && ev.PassTrigger(triggerslist_17)) {  fill_plot=true; if(!IsDATA) weight_ptcorr= event_weight * ev.GetTriggerLumi(triggerslist_17);}
        else{ fill_plot=false;weight_ptcorr = 0.;      }
      }
    }
    
    /// Get precale weiht for normal pt bins
    prescale_lep = GetPrescale(leps);
  } 
  else{
    
    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                    
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";

    bool El8PD=  (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool El12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool El17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool El23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));


    if (lep_pt_corr > 10  && lep_pt_corr < 23){
      if(lep_pt > 10) {
	if(El8PD && ev.PassTrigger(triggerslist_8)) { fill_plot=true;if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_8);}
	else{fill_plot=false;weight_ptcorr = 0.;}
      }
    }
    else if ( lep_pt_corr < 35){
      if(lep_pt > 15) {
	if(El12PD&&ev.PassTrigger(triggerslist_12)) {  fill_plot=true;if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_12);}
	else{ fill_plot=false;weight_ptcorr = 0.;}
      }
    }
    
    else if (lep_pt_corr < 40){
      if(lep_pt > 20) {
	if(El17PD&& ev.PassTrigger(triggerslist_17)) {fill_plot=true; if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_17);}
	else{fill_plot=false;weight_ptcorr = 0.;}
      }
    }
    else{
      if(lep_pt > 25) {
	if(El23PD&&ev.PassTrigger(triggerslist_23)) {fill_plot=true;if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_23);}
	else{fill_plot=false;	  weight_ptcorr = 0.;}
      }// pt 
    }// end ptbin check
    prescale_lep = GetPrescale(leps);
  }


  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);
  std::vector<FatJet> AK8_JetColl;
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
							     

  std::vector<Jet> BJetColl    = GetBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )weight_pt*= sf_btag;

  
  double ptminfix = IsMuon ? 8 : 12;
  double ptmin    = IsMuon ? 10 : 10;

  for(int ilep = 0 ; ilep < 2; ilep++)  {
    TString prefix = (ilep==0) ? L_prefix : T_prefix;
    if((ilep==1) && !(blepsT[0])) continue;
    if(lep_pt > ptmin){
      FillHist((prefix + "_reliso").Data(), lep_reliso, weight_pt*prescale_lep, 50, 0., 1.);
      FillHist((prefix + "_dXY").Data(),    lep_dxy, weight_pt*prescale_lep, 50, 0., 1.);
      FillHist((prefix + "_IP3D").Data(),   lep_ip3d, weight_pt*prescale_lep, 50, 0., 10.);
      FillHist((prefix + "_mva").Data(),    lep_mva, weight_pt*prescale_lep, 50, -1., 1.);
      FillHist((prefix + "_pt_eta").Data(), lep_pt, lep_eta,weight_pt*prescale_lep, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_pt").Data(),     lep_pt, weight_pt*prescale_lep, nbin_pt, ptbins, "p_{T} (GeV)");
      FillHist((prefix + "_eta").Data(),    lep_eta, weight_pt*prescale_lep , nbin_eta, etabins,"#eta");
      if(BJetColl.size()==0){
	FillHist((prefix + "_0BJet_pt").Data(),     lep_pt, weight_pt*prescale_lep, nbin_pt, ptbins, "p_{T} (GeV)");
	FillHist((prefix + "_0BJet_eta").Data(),    lep_eta, weight_pt*prescale_lep , nbin_eta, etabins,"#eta");
      }
      else{
	FillHist((prefix + "_BJet_pt").Data(),     lep_pt, weight_pt*prescale_lep, nbin_pt, ptbins, "p_{T} (GeV)");
        FillHist((prefix + "_BJet_eta").Data(),    lep_eta, weight_pt*prescale_lep , nbin_eta, etabins,"#eta");
      }
    }    
    if(fill_plot) {
      FillHist((prefix + "_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      if(lep_pt > ptminfix) FillHist((prefix + "_ptcone_ptfix_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
    }
  }
  return;
}


HNL_FakeRate::HNL_FakeRate(){
  
}
 
HNL_FakeRate::~HNL_FakeRate(){

  //  delete NvtxSFFile;
  //for(std::map< TString, TH1D* >::iterator mapit = maphist_NvtxSF.begin(); mapit!=maphist_NvtxSF.end(); mapit++){
  //  delete mapit->second;
  // }
  //maphist_NvtxSF.clear();

}




void HNL_FakeRate::FillRegionPlots( TString plot_dir, TString region,  std::vector<Jet> jets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met,  double w){
  
  if((els.size()+ mus.size()) != 1) return;
  Lepton lep1;
  if(els.size()==1) lep1=els[0];
  else lep1 = mus[0];

  int nbin_pt    =11;
  double ptbins    [nbin_pt    +1] = { 0.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};

  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_binned_pt", lep1.Pt()  , w, nbin_pt, ptbins,"p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_pt", lep1.Pt()  , w, 400, 0., 2000.,"p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_eta", lep1.Eta()  , w, 30, -3., 3,"#eta");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_lep_jet_ptrel", lep1.lep_jet_ptrel(), w, 50., 0., 2.,"PtRel");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_lep_jet_ptratio", lep1.lep_jet_ptratio(), w, 50., 0., 2.,"PtRel");

  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Met", met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MetPhi", met.Phi()  , w, 100, -5, 5,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/njets" , jets.size() , w, 5, 0., 5., "N_{jets}");

  float lep_reliso  = lep1.RelIso();
  float lep_minireliso  = lep1.MiniRelIso();
  float lep_ip3d    = fabs(lep1.IP3D()/lep1.IP3Derr());
  float lep_mva     =  lep1.lep_mva();
  float lep_dxy     = fabs(lep1.dXY());

  if(els.size() > 0)   FillHist( plot_dir +  "/RegionPlots_"+ region+ "/NMissingHits", els[0].NMissingHits(), w, 5, 0., 5.);


  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Reliso", lep_reliso, w, 50, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MiniReliso", lep_minireliso, w, 50, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/dXY",    lep_dxy, w, 100, 0., 0.5);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/IP3D",   lep_ip3d, w, 50, 0., 10.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Mva",    lep_mva, w, 50, -1., 1.);



  double lep_jet_ratio= 0.;
  double jet_CEEF = 0.;  double jet_CHEF = 0.;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(lep1.Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      lep_jet_ratio = jets.at(ij).Pt() /  lep1.Pt();
      jet_CEEF = jets.at(ij).ChargedEmEnergyFraction();
      jet_CHEF = jets.at(ij).ChargedHadEnergyFraction();
    }
  }
  
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/JetLepPtRatio" , lep_jet_ratio  , w, 50, 0., 5.,"P^{jet}_{T} / P^{lepton}_{T}");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/AwayJetChargedEMEnergyFraction" , jet_CEEF  , w, 50, 0., 2.,"Jet charged EMF");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/AwayJetChargedHadEnergyFraction" , jet_CHEF  , w, 50, 0., 2.,"Jet charged EMF");
  


  double METdphi = TVector2::Phi_mpi_pi(lep1.Phi()-met.Phi());
  double MT = sqrt(2.* lep1.Et()*met.Pt() * (1 - cos( METdphi)));
  
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Mt" , MT  , w, 40, 0., 200.,"MET GeV");
  
}


void HNL_FakeRate::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){


  return;

}

void HNL_FakeRate::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNL_FakeRate::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

