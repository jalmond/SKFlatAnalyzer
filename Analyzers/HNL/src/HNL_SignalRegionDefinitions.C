#include "HNL_RegionDefinitions.h"


/*


  Analysis REGION Functions 
  -----------------------------------                                                                                                                   a) RunPreslection
  b) PassVBFInitial : Use this IF using shape for SR2 
  c) PassVBF

  Presel
  ----------------------------
  - PassPreselection

  SR Functions
  -----------------------------------
  * RunAllSignalRegions Runs All SR files
  1- RunSignalRegionAK8 (SR1) OR RunSignalRegionAK8String To run RunSignalRegionAK8 and return Limit input string
  2- RunSignalRegionWW  (SR2) OR RunSignalRegionWWString To run RunSignalRegionWW and return Limit input string  
  3- RunSignalRegionAK4 (SR3) OR RunSignalRegionAK4String To run RunSignalRegionAK4  and return Limit input string  
  4- RunSignalRegionAK4StringBDT Run SR3 BDT
  

 */



void HNL_RegionDefinitions::RunSR3BDT(HNL_LeptonCore::ChargeType qq, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,  std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){
  
  /// Function plots BDT for all Setup in MNStrList[im], NCutList[ic], NTreeList[] 
  // The string returned can be ignored, since this is for use in Limit plot, but function make plot in LimitInputSR3BDT/param.Name
  
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu, EMu};
  std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
  std::vector<Lepton *> leps       = MakeLeptonPointerVector(muons,electrons,param);
  for(auto dilep_channel : channels){
    
    for(unsigned int im=0; im<MNStrList.size(); im++){
      for(unsigned int ic=0; ic<NCutList.size(); ic++){
	for(unsigned int it=0; it<NTreeList.size(); it++){
	  RunSignalRegionAK4StringBDT(true,MNStrList[im], NCutList[ic], NTreeList[it], dilep_channel,qq, leps, JetColl,B_JetColl, ev, METv ,param,"", weight_ll);
	  
	}
      }
    }
  }
  

  return;

}


void HNL_RegionDefinitions::RunAllSignalRegions(HNL_LeptonCore::ChargeType qq, 
						std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Tau> TauColl, 
						std::vector<Jet> JetCollLoose, std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, 
						Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){

  vector<HNL_LeptonCore::Channel> channels = {GetChannelENum(param.Channel)};

  for(auto dilep_channel : channels){
    
    
    if(run_Debug) cout << "HNL_RegionDefinitions::RunAllSignalRegions " << GetChannelString(dilep_channel) <<  endl;

    double  weight_channel = weight_ll;

    //// Select CHannel used for Signals to check if signal is EE/MM/Emu using gen info
    if(MCSample.Contains("Type")&& !SelectChannel(dilep_channel)) continue;
  
    std::vector<Lepton *> leps       = MakeLeptonPointerVector(muons,electrons,param);
    
    
    if(!PassGenMatchFilter(leps,param)) continue;
    
    if(!ConversionSplitting(leps,RunConv,2)) continue;

    // Make channel speciific AnalyzerParameter                                                                                                              
    AnalyzerParameter param_channel = param;

    FillCutflow(HNL_LeptonCore::ChannelDepInc, weight_channel, GetChannelString(dilep_channel) +"_NoCut",param_channel);

    if(!PassMETFilter()) return;

    if(MCSample.Contains("Type")){
      vector<Gen> SigGens = GetGenLepronsSignal();
      if (SigGens.size()==2){

	if(SigGens[0].PID() > 0 && SigGens[1].PID()  > 0 )     FillCutflow(HNL_LeptonCore::ChannelDepIncQ, weight_channel, GetChannelString(dilep_channel) +"_MMQ_NoCut",param_channel);
	else if(SigGens[0].PID()  < 0 && SigGens[1].PID()  < 0 )     FillCutflow(HNL_LeptonCore::ChannelDepIncQ, weight_channel, GetChannelString(dilep_channel) +"_PPQ_NoCut",param_channel);
	
      }
    }
    
    
    TString channel_string = GetChannelString(dilep_channel);
    param_channel.Name =  channel_string + "_Channel/" + param_channel.DefName; 
    
    std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(muons_veto,electrons_veto,param_channel);
    
    if(leps.size() ==2)  FillCutflow(HNL_LeptonCore::ChannelDepDilep, weight_channel, GetChannelString(dilep_channel) +"_Dilep",param_channel);
    

    if(RunCF){
      if(dilep_channel == MuMu) continue;

      if(IsData && SameCharge(leps)) continue;
      if(!IsData && !SameCharge(leps)) continue;
      
      if(IsData)weight_channel = GetCFWeightElectron(leps, param_channel);
      //if(IsData)weight_channel = GetCFWeightElectron(leps, param_channel, true, 0); // data-driven charge flip
      if(IsData)FillWeightHist(param_channel.Name+"/CFWeight",weight_channel);

    }
    else{
      if(!SameCharge(leps)) continue;
    }
 
    if(RunFake&& IsData){
      if(_jentry%nLog==0){
	cout << "Running Fakes... " <<  GetFakeWeight(leps, param_channel) << "  " << GetFakeWeight(leps, param_channel) << endl;
      }

      weight_channel = GetFakeWeight(leps, param_channel);
      FillFakeWeightHist(param_channel.Name+"/FakeWeight", leps,param_channel, weight_channel);
    }

    // Run PRESEL + SR1-3                                                                                                                                                           
    //bool run_ORTrigger = false;

    
    HNL_LeptonCore::SearchRegion LimitRegions = HNL_LeptonCore::MuonSR;
    if (dilep_channel == EE) LimitRegions =HNL_LeptonCore::ElectronSR;
    if (dilep_channel == EMu) LimitRegions =HNL_LeptonCore::ElectronMuonSR;
    if (dilep_channel == MuE) LimitRegions =HNL_LeptonCore::ElectronMuonSR;
    
    HNL_LeptonCore::SearchRegion LimitRegionsQ = HNL_LeptonCore::MuonSRQQ;
    if (dilep_channel == EE) LimitRegionsQ =HNL_LeptonCore::ElectronSRQQ;
    if (dilep_channel == EMu) LimitRegionsQ =HNL_LeptonCore::ElectronMuonSRQQ;
    if (dilep_channel == MuE) LimitRegionsQ =HNL_LeptonCore::ElectronMuonSRQQ;

    HNL_LeptonCore::SearchRegion LimitRegionsBDT = HNL_LeptonCore::MuonSRBDT;
    if (dilep_channel == EE) LimitRegionsBDT =HNL_LeptonCore::ElectronSRBDT;
    if (dilep_channel == EMu) LimitRegionsBDT =HNL_LeptonCore::ElectronMuonSRBDT;
    if (dilep_channel == MuE) LimitRegionsBDT =HNL_LeptonCore::ElectronMuonSRBDT;

    /*HNL_LeptonCore::SearchRegion LimitRegionsBDTQ = HNL_LeptonCore::MuonSRBDTQQ;
    if (dilep_channel == EE) LimitRegionsBDTQ =HNL_LeptonCore::ElectronSRBDTQQ;
    if (dilep_channel == EMu) LimitRegionsBDTQ =HNL_LeptonCore::ElectronMuonSRBDTQQ;
    */
    
    if (!PassTriggerSelection(dilep_channel, ev, leps,param.TriggerSelection)) continue;
    EvalTrigWeight(dilep_channel, muons,electrons,param, weight_channel);
       

    if(!PassPreselection(dilep_channel,qq, leps, leps_veto, TauColl, JetColl, VBF_JetColl, AK8_JetColl, B_JetColl,ev, METv ,param_channel,"", weight_channel)) continue;

    TString  lep_charge =  (leps[0]->Charge() < 0)  ? "QM" :  "QP";
    
    if(AK8_JetColl.size() > 0) {

      TString SRbin= RunSignalRegionAK8String (dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl,B_JetColl,ev, METv ,param_channel,"", weight_channel) ;

      if(SRbin != "false") FillLimitInput(LimitRegions, weight_channel,   SRbin,  "LimitInput/"+param_channel.Name);
      if(SRbin != "false") FillLimitInput(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param_channel.Name);
      
      for(unsigned int im=0; im<MNStrList.size(); im++){
	if(SRbin != "false") FillLimitInput(LimitRegionsBDT, weight_channel, SRbin,"LimitInputBDT/"+param_channel.Name+"/M"+MNStrList[im]);
      }
    }
    else{
      
      TString SRbin = RunSignalRegionWWString( dilep_channel,qq, leps, leps_veto,  TauColl, JetCollLoose, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param_channel,  "", weight_channel);
            

      if(SRbin != "false"){
	FillLimitInput(LimitRegions, weight_channel, SRbin,"LimitInput/"+param_channel.Name);
	FillLimitInput(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param_channel.Name);

	for(unsigned int im=0; im<MNStrList.size(); im++){
	  if(SRbin != "false") FillLimitInput(LimitRegionsBDT, weight_channel, SRbin,"LimitInputBDT/"+param_channel.Name+"/M"+MNStrList[im]);
	}

      }
      else{

	for(auto imapHP :FinalBDTHyperParamMap){
	  TString SRBDT = RunSignalRegionAK4StringBDT(true,imapHP.first , imapHP.second.first, imapHP.second.second, dilep_channel,qq, leps, JetColl,  B_JetColl, ev, METv ,param_channel,"", weight_channel);
	  if(SRBDT != "false") FillLimitInput(LimitRegionsBDT, weight_channel, SRBDT,"LimitInputBDT/"+param_channel.Name+"/M"+imapHP.first);
	}

	SRbin  = RunSignalRegionAK4String (dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);
	


	if(SRbin != "false"){
	  FillLimitInput(LimitRegions, weight_channel, SRbin,"LimitInput/"+param_channel.Name);
	  FillLimitInput(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param_channel.Name);

	}
	else{
	  if(JetColl.size() == 0)Fill_RegionPlots(param,"FailZeroJet" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, weight_channel);
	  if(JetColl.size() == 1)Fill_RegionPlots(param,"Fail1Jet" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, weight_channel);
	  if(JetColl.size() > 1)Fill_RegionPlots(param,"FailDiJet" ,  JetColl, AK8_JetColl, leps,  METv, nPV, weight_channel);
	  
	  
	  if(JetColl.size() == 0) FillCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_0j",param_channel);
	  else if(JetColl.size() == 1)FillCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_1j",param_channel);
	  else FillCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_2j",param_channel);
	  
	}
      }
    }
  }
  return;
}




bool  HNL_RegionDefinitions::PassPreselection(HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType qq, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto, vector<Tau> TauColl,  std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString TriggerOption ,  float w){

  // ONLY CODE SS PRESLECTION 
  
  // APPLY PT CUTS
  /*
    ee channel 25/10
    emu   25/5
    mm    20/5
    
    
   */
  
  if(run_Debug) cout << "HNL_RegionDefinitions::PassPreselection " << GetChannelString(channel) <<  endl;

  // Make sure events contain 2 leps                                                                                                       
  // Make sure correct leptons are used                                                                                                                                      
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  //  FillCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_MultiTrigger",param);
  
  //FillCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_Trigger",param);


  if(!PassHEMVeto(leps)) return false;

  // Make sure events contain 2 leps
  if (leps_veto.size() != 2) return false;
  
  // CUT ON MASS OF LL PAIR

  Particle ll =  (*leps[0]) + (*leps[1]);
  
  // VETO Z PEAK IN EE CHANNEL
  if (channel==EE  && (fabs(ll.M()-M_Z) < 10)) return false;

  if(ll.M() < 10) return false; // TO_CHECK: IS 20 BEST OPTION

  // REMOVE 0 Jet EVENTS
  //int njets     = JetColl.size() + AK8_JetColl.size() + VBF_JetColl.size();
   
  //  Fill_RegionPlots(param,"Preselection" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);
  FillCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_Presel",param);
  
  return true;
}




bool  HNL_RegionDefinitions::RunSignalRegionAK8(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  TString SR1String = RunSignalRegionAK8String(channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  
  if(SR1String == "false") return false;
  else return true;
  
}


/// Return TString for Limit bin

TString HNL_RegionDefinitions::RunSignalRegionAK8String(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){
  
  
  bool HasTightTau=false;
  for(auto itau: TauColl){
    bool HasTightTau_n=true;
    for (auto ilep : leps)        if (ilep->DeltaR(itau) < 0.4) HasTightTau_n=false;
    for (auto ijet : AK8_JetColl) if (ijet.DeltaR(itau) < 1.)   HasTightTau_n=false;
    for (auto ijet : JetColl)     if (ijet.DeltaR(itau) < 0.5)  HasTightTau_n=false;
    if(HasTightTau_n) HasTightTau=true;
  }
  
  FillCutflow(HNL_LeptonCore::SR1, w, "SR1_Init",param);
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";  
  if (leps_veto.size() != 2) return "false";
  
  
  
  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";
  
  FillCutflow(HNL_LeptonCore::SR1, w, "SR1_lep_charge",param);
  FillCutflow(HNL_LeptonCore::SR1,w, "SR1_lep_pt",param);
  if(AK8_JetColl.size() == 0) return "false";

  
  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < 10)) return "false";
  
  FillCutflow(HNL_LeptonCore::SR1, w, "SR1_dilep_mass",param);

  if(HasTightTau) return "false";
  FillCutflow(HNL_LeptonCore::SR1, w, "SR1_tauveto",param);


  double met2_st     = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = (met2_st < 10);
  bool PassBJetMVeto = (B_JetColl.size()==0);
  double LowerMassSR1WmassCut = 450.;
  
  TString signal_region1 = "HNL_SR1";
  
  Fill_RegionPlots(param,"InclusiveSR1" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);

  if(PassHMMet)FillCutflow(HNL_LeptonCore::SR1, w, "SR1_MET",param);

  if(!(PassHMMet&&PassBJetMVeto)) return "false";
  
  FillCutflow(HNL_LeptonCore::SR1, w, "SR1_bveto",param);    

  FillCutflow(HNL_LeptonCore::ChannelDepSR1, w, GetChannelString(channel) +"_SR1",param);
 
  double pt_cut = 100.;
  if(DataEra=="2018")  pt_cut = 200.; 

  //Fill Limit plot
 
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  
  int m=-999;
  for(UInt_t emme=0; emme<AK8_JetColl.size(); emme++){
    dijetmass_tmp = AK8_JetColl[emme].SDMass();
    if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
      dijetmass = dijetmass_tmp;
      m = emme;
    }
  }
  
  Particle Wcand = AK8_JetColl[m] + *leps[0]+*leps[1];
  if(Wcand.M()  < LowerMassSR1WmassCut ) return "false";
  //  if(GetRecoObjMass(signal_region1,JetColl, AK8_JetColl,  leps) < LowerMassSR1WmassCut ) return "false";
  //  FillCutflow(HNL_LeptonCore::SR1, w, "SR1_Wmass",param);

  Particle N1cand = AK8_JetColl[m] + *leps[0];
  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();
  if(MN1 > 225){
    if(leps[0]->Pt() < pt_cut)  return "SR1_MNbin1";
    FillCutflow(HNL_LeptonCore::SR1, w, "SR1_LepPt",  param.Name);
  }
  else   if(MN1 > 150){
    if(leps[0]->Pt() < pt_cut-50)  return "SR1_MNbin1";
    FillCutflow(HNL_LeptonCore::SR1, w, "SR1_LepPt",param.Name);
  }

  int nSRbins=8;
  double ml1jbins[nSRbins] = { 0., 500, 600.,700.,850, 1000.,1400, 2000.};

     
  FillHist( param.Name+"/LimitShape_SR1/PreCuts_N1Mass_Central",  MN1,  w, nSRbins-1, ml1jbins, "Reco M_{l1jj}");
  
  FillCutflow(HNL_LeptonCore::SR1, w, "SR1_masscuts",param);
      
  FillHist( param.Name+"/LimitShape_SR1/N1Mass_Central",  MN1,  w, 4, ml1jbins, "Reco M_{l1jj}");
  
  Fill_RegionPlots(param,"FinalSR1" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);
  
  for(int ibin=1; ibin < nSRbins; ibin++){
    if(MN1 < ml1jbins[ibin]) return "SR1_MNbin"+to_string(ibin+1);
  }
  
  return "true";

}





bool  HNL_RegionDefinitions::RunSignalRegionWW(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){

  TString SR2String = RunSignalRegionWWString(channel, qq, leps, leps_veto, TauColl,JetCollLoose, JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR2String == "false") return false;
  else return true;
  
}



TString HNL_RegionDefinitions::RunSignalRegionWWString(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  //  FillCutflow(HNL_LeptonCore::SR2,w, "SR2_lep_charge",param);
  
  //  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_lep_pt",param);

  bool use_leadjets=true;
  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi < 2.) return "false";
  //  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DPhi",param);
  
  if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return "false";
  if(JetColl.size() < 2) return "false";
  //  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJet",param);

  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < JetColl.size()-1; ij++){
    for(unsigned int ij2 = ij+1; ij2 < JetColl.size(); ij2++){

      double deta = fabs(JetColl[ij].Eta() - JetColl[ij2].Eta());
      if(deta > maxDiJetDeta) {
	maxDiJetDeta=deta;
      	ijet1=ij;
	ijet2=ij2;
      }
    }
  }
  if(use_leadjets){ijet1=0;ijet2=1;}

  if(maxDiJetDeta < 2.5) return "false";
  //  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetEta",param);

  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  //  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetMass",param);
  if(JJ.M() < 750) return "false";

  double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;

  if(zeppenfeld > 0.75) return "false";

  if(!PassVBF(JetColl,leps,750., true)) return "false";

  Fill_RegionPlots(param,"InclusiveSR2" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);

  //  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_VBF",param);

  bool PassBJetMVeto = (B_JetColl.size()==0);

  double HT(0.);
  for(auto ij : JetColl) HT += ij.Pt();
  

  Fill_RegionPlots(param,"PassSR2" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);


  double htltcut = 1.;

  double met2_st = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet    = (met2_st < 10);  ////// An option to add since we have MET in SR1/3
  
  if(PassHMMet) {
    //    FillCutflow(HNL_LeptonCore::SR2, w, "SR2_met",param);

    if (HT/leps[0]->Pt() < htltcut){
      
      //      FillCutflow(HNL_LeptonCore::SR2, w, "SR2_ht_lt1",param);
      
      if(PassBJetMVeto){
	
	//	FillCutflow(HNL_LeptonCore::SR2, w, "SR2_bveto",param);
	

	double PTBin[5] = { 0.,50, 100., 120., 200};
	double L2Pt = (leps[1]->Pt() > 200) ? 199. : leps[1]->Pt();
	if(param.WriteOutVerbose == 0)FillHist( "LimitSR2/"+param.Name+"/PT2_LT1_Central",  L2Pt,  w, 4, PTBin, "Reco PTLep2");


	if (L2Pt > 120)      return "SR2_HTLTbin1";
	return "SR2_HTLTbin2";

      }
    }
  }
  return "false";
}



bool  HNL_RegionDefinitions::RunSignalRegionAK4(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){



  TString SR3String = RunSignalRegionAK4String(channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR3String == "false") return false;
  else return true;

}




TString HNL_RegionDefinitions::RunSignalRegionAK4StringBDT(bool isSR, TString mN, TString NCut, TString NTree, HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> LepTColl, std::vector<Jet> JetColl, std::vector<Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  if(!CheckLeptonFlavourForChannel(channel, LepTColl)) return "false";
  //  if(isSR)FillCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_lep_pt",param);

  if (LepTColl.size() != 2) return "false";

  if(qq==Plus && LepTColl[0]->Charge() < 0) return "false";
  if(qq==Minus && LepTColl[0]->Charge() > 0) return "false";

  //  if(isSR)FillCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_lep_charge",param);

  Particle ll =  (*LepTColl[0]) + (*LepTColl[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < 10)) return "false";

  //  if(isSR)  FillCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_dilep_mass",param);

  float MVAvalue = EvaluateEventMVA(mN, NCut,NTree, channel,  LepTColl,ev, METv,param);
  //cout << "Summary of BDT " << mN << endl;
  //cout << "Predetermined value = " << MVAvalue << " " <<  ev.HNL_MVA_Event(GetChannelString(channel)+"_"+mN) << endl;
  
  if(isSR)FillHist(param.Name+"/LimitShape_SR3BDT/SignalBins_M"+mN+"_NCut"+NCut+"_NTree"+NTree, MVAvalue, w, 40, -1., 1.);

  std::vector<FatJet> FatJetColl;
  double met2_st = GetMET2ST(LepTColl, JetColl, FatJetColl, METv);
  bool PassHMMet    = (met2_st < 15);
  
  TString LabelPrefix = (isSR) ? "SR3" : "CR3";
  

  if(!PassHMMet) return "false";
  if(B_JetColl.size() > 0)    return "false";
  if(MVAvalue< 0.0) return LabelPrefix+"_BDTbin1";
  else if(MVAvalue< 0.10) return LabelPrefix+"_BDTbin2";
  else if(MVAvalue< 0.125) return LabelPrefix+"_BDTbin3";
  else if(MVAvalue< 0.15) return LabelPrefix+"_BDTbin4";
  else if(MVAvalue< 0.175) return LabelPrefix+"_BDTbin5";
  else if(MVAvalue< 0.2) return LabelPrefix+"_BDTbin6";
  else if(MVAvalue< 0.225) return LabelPrefix+"_BDTbin7";
  else  return LabelPrefix+"_BDTbin8";

  return "true";
}


TString HNL_RegionDefinitions::RunSignalRegionAK4String(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){

                                       
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";
  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_pt",param);

  Fill_RegionPlots(param,"InclusiveSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_charge",param);

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < 10)) {
    Fill_RegionPlots(param,"SR3ZPeak" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
    return "false";
  }

  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_dilep_mass",param);
  double met2_st  = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet  = (met2_st < 15);

  int  NB_JetColl      =  B_JetColl.size();

  if(NB_JetColl==0 && PassHMMet){
    if(JetColl.size()==0)  Fill_RegionPlots(param,"ZeroJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
    if(JetColl.size()==1)  Fill_RegionPlots(param,"OneJetSR3" ,  TauColl,JetColl, AK8_JetColl, leps,  METv, nPV, w);
  }

  
  if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 0 && leps[1]->Pt() > 140.) {
    FillHist( param.Name+"/LimitShape_SR3/SignalBins",   0.5, w, 16, 0, 16.,  "Signalbins");
    FillHist( param.Name+"/LimitShape_SR3/SignalBinsQ",   0.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    FillCutflow(HNL_LeptonCore::SR3, w, "SR3_0JetBinHPT",param);

    return "SR3_bin1";
  }
  if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 1 && leps[1]->Pt() > 140.) {
    FillHist( param.Name+"/LimitShape_SR3/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");
    FillHist( param.Name+"/LimitShape_SR3/SignalBinsQ",   1.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    FillCutflow(HNL_LeptonCore::SR3, w, "SR3_1JetBinHPT",param);
    return "SR3_bin2";
  }
  
  if(JetColl.size() < 2)   return "false";

  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_dijet",param);

  //if(!(GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) < UpperMassSR3WmassCut && GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) > LowerMassSR3WmassCut)) {
  //  return "false";
  // }
                                                           
  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_Wmass",param);

  if(JetColl[0].Pt() < 25.)  return "false";
  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_J1Pt",param);
   
  if(!PassHMMet)   return "false";
  
  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_MET",param);


  if(NB_JetColl>  0)   return "false";
  
  Fill_RegionPlots(param,"DiJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);

  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_bveto",param);

  //Fill_RegionPlots      (channel, true, param.Name+"/SR3/" , "", JetColl, AK8_JetColl, leps,  METv, nPV, w);
  //FIll Limit 

 
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=1; enne<JetColl.size(); enne++) {

      if(emme == enne) continue;      
      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();

      
      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
        n = enne;
      }
    }
  }
  
  Particle N1cand = JetColl[m] + JetColl[n]+ *leps[0];
  Particle N2cand = JetColl[m] + JetColl[n]+ *leps[1];
  Particle Wcand = JetColl[m] + JetColl[n]+ *leps[0]+ *leps[1];

  Particle NCand = (leps[1]->Pt() < leps[0]->Pt()) ? N2cand : N1cand;
  Lepton Nlep = (leps[1]->Pt() < 100.) ?  *leps[1] : *leps[0];
  Lepton Wlep = (leps[1]->Pt() > 100.) ? *leps[1] : *leps[0];

  double pt_bin2= 140.;

  if(Wcand.M() < 400) return "SR3_bin3";
  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_W1Mass",param);
  if(leps[1]->Pt() < 20) return "SR3_bin3";

  if(leps[0]->Pt() < pt_bin2) return "SR3_bin3";
  FillCutflow(HNL_LeptonCore::SR3, w, "SR3_LepPt",param);

  if(met2_st < 2){
    if(N1cand.M() < 500.)   return "SR3_bin4";
    if(N1cand.M() < 750.)   return "SR3_bin5";
    return "SR3_bin6";
  }
  else   if(met2_st < 7){
    if(N1cand.M() < 500.)   return "SR3_bin7";
    if(N1cand.M() < 750.)   return "SR3_bin8";
    return "SR3_bin9";
  }
  else{
    if(N1cand.M() < 750.)   return "SR3_bin10";
  }
  return "SR3_bin11";

  
}


HNL_RegionDefinitions::HNL_RegionDefinitions(){
      
}
 
HNL_RegionDefinitions::~HNL_RegionDefinitions(){

  
}




bool HNL_RegionDefinitions::PassVBFInitial(vector<Jet>  JetColl){

  if(JetColl.size() < 2) return false;

  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < JetColl.size()-1; ij++){
    for(unsigned int ij2 = ij+1; ij2 < JetColl.size(); ij2++){

      double deta = fabs(JetColl[ij].Eta() - JetColl[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }

  if(maxDiJetDeta < 2.5) return false;
  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  if(JJ.M() < 450.) return false;

  return true;


}

bool HNL_RegionDefinitions::PassVBF(vector<Jet>  JetColl,std::vector<Lepton *> leps, double mjj_cut ,bool use_leadjets){

  if(leps.size() != 2) return false;
  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi < 2.) return false;
  if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return false;
  if(JetColl.size() < 2) return false;

  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < JetColl.size()-1; ij++){
    for(unsigned int ij2 = ij+1; ij2 < JetColl.size(); ij2++){

      double deta = fabs(JetColl[ij].Eta() - JetColl[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }
  if(use_leadjets){ijet1=0;ijet2=1;}
  
  if(maxDiJetDeta < 2.5) return false;
  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  if(JJ.M() < mjj_cut) return false;
  double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;

  if(zeppenfeld > 0.75) return false;

  return true;
}



