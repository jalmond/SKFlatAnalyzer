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

  if(GetChannelENum(param.Channel) == HNL_LeptonCore::NONE){
    cout << "CHANNEL NOT SET" << endl;
    exit(EXIT_FAILURE);
  }


  for(auto dilep_channel : channels){
    
    // Make channel speciific AnalyzerParameter                                                                                                                             
    double  weight_channel = weight_ll;

    //// Select CHannel used for Signals to check if signal is EE/MM/Emu using gen info
    if(MCSample.Contains("Type")&& !SelectChannel(dilep_channel)) continue;
  
 
    std::vector<Lepton *> leps       = MakeLeptonPointerVector(muons,     electrons,     param);
    std::vector<Lepton *> leps_veto       = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
   
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
    param_channel.Name =  channel_string + "/" + param_channel.DefName; 


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

      TString SRbin= RunSignalRegionAK8String (true,dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl,B_JetColl,ev, METv ,param_channel,"", weight_channel) ;

      if(SRbin != "false") FillLimitInput(LimitRegions, weight_channel,   SRbin,  "LimitInput/"+param_channel.Name);
      if(SRbin != "false") FillLimitInput(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param_channel.Name);
      
      for(unsigned int im=0; im<MNStrList.size(); im++){
	if(SRbin != "false") FillLimitInput(LimitRegionsBDT, weight_channel, SRbin,"LimitInputBDT/"+param_channel.Name+"/M"+MNStrList[im]);
      }
    }
    else{
      
      TString SRbin = RunSignalRegionWWString(true, dilep_channel,qq, leps, leps_veto,  TauColl, JetCollLoose, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param_channel,  "", weight_channel);
            

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

	SRbin  = RunSignalRegionAK4String (true,dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);
	


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




bool  HNL_RegionDefinitions::RunSignalRegionAK8(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  TString SR1String = RunSignalRegionAK8String(ApplyForSR,channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  
  if(SR1String == "false") return false;
  else return true;
  
}


/// Return TString for Limit bin

TString HNL_RegionDefinitions::RunSignalRegionAK8String(bool ApplyForSR, 
							HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , 
							std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl,
							std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, 
							Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){
   
  TString RegionTag                = ApplyForSR ? "SR1" : "CR1";
  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR1 : HNL_LeptonCore::CR1;

  double met_cut     = 10.;
  double met2_st     = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;

  double LowerMassSR1WmassCut = 450.;

  bool HasTightTau=false;
  for(auto itau: TauColl){
    bool HasTightTau_n=true;
    for (auto ilep : leps)        if (ilep->DeltaR(itau) < 0.4) HasTightTau_n=false;
    for (auto ijet : AK8_JetColl) if (ijet.DeltaR(itau) < 1.)   HasTightTau_n=false;
    for (auto ijet : JetColl)     if (ijet.DeltaR(itau) < 0.5)  HasTightTau_n=false;
    if(HasTightTau_n) HasTightTau=true;
  }
  
  if(ApplyForSR) FillCutflow(Reg, w, RegionTag+"_Init",param);
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";  
  if (leps_veto.size() != 2) return "false";
  
  
  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";
  
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_lep_charge",param);
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_lep_pt",param);
  if(AK8_JetColl.size() == 0) return "false";

  
  Particle ll =  (*leps[0]) + (*leps[1]);
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_dilep_mass",param);

  if(HasTightTau) return "false";
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_tauveto",param);
    
  if(ApplyForSR)   Fill_RegionPlots(param,"Inclusive"+RegionTag , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);

  if(PassHMMet&&ApplyForSR)FillCutflow(Reg, w, RegionTag+"_MET",param);
  
  if(PassRegionReq &&ApplyForSR) FillCutflow(Reg, w, RegionTag+"_bveto",param);    
  
  if(!PassRegionReq) return "false";
  if(ApplyForSR)FillCutflow(HNL_LeptonCore::ChannelDepSR1, w, GetChannelString(channel) +"_"+RegionTag,param);
 
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

  Particle N1cand = AK8_JetColl[m] + *leps[0];
  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();

  TString R1Bin=RegionTag+"_MNbin1";

  if(MN1 > 225){
    if(leps[0]->Pt() < pt_cut)  return RegionTag+"_MNbin1";
    FillCutflow(Reg, w, RegionTag+"_LepPt",  param.Name);
  }
  else   if(MN1 > 150){
    if(leps[0]->Pt() < pt_cut-50)  return RegionTag+"_MNbin1";
    FillCutflow(Reg, w, RegionTag+"_LepPt",param.Name);
  }

  int nSRbins=8;
  double ml1jbins[nSRbins] = { 0., 500, 600.,700.,850, 1000.,1400, 2000.};

     
  if(ApplyForSR)FillHist( param.Name+"/LimitShape_"+RegionTag+"/PreCuts_N1Mass_Central",  MN1,  w, nSRbins-1, ml1jbins, "Reco M_{l1jj}");
  
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_masscuts",param);
  
  if(ApplyForSR)FillHist( param.Name+"/LimitShape_"+RegionTag+"/N1Mass_Central",  MN1,  w, 4, ml1jbins, "Reco M_{l1jj}");
  
  if(ApplyForSR)Fill_RegionPlots(param,"Final"+RegionTag ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);
  
  for(int ibin=1; ibin < nSRbins; ibin++){
    if(MN1 < ml1jbins[ibin]) return RegionTag+"_MNbin"+to_string(ibin+1);
  }
  
  return "true";

}





bool  HNL_RegionDefinitions::RunSignalRegionWW(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){

  TString SR2String = RunSignalRegionWWString(ApplyForSR,channel, qq, leps, leps_veto, TauColl,JetCollLoose, JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR2String == "false") return false;
  else return true;
  
}



TString HNL_RegionDefinitions::RunSignalRegionWWString(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  TString RegionTag                = ApplyForSR ? "SR2" : "CR2";
  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR2 : HNL_LeptonCore::CR2;

  double met_cut     = 10.;
  double met2_st     = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;


  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus  && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  if(ApplyForSR)  FillCutflow(Reg,w, RegionTag+"_lep_charge",param);
  
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_lep_pt",param);

  bool use_leadjets=true;
  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi < 2.) return "false";
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_DPhi",param);
  
  if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return "false";
  if(JetColl.size() < 2) return "false";
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_DiJet",param);

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
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_DiJetEta",param);

  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  if(ApplyForSR ) FillCutflow(Reg, w, RegionTag+"_DiJetMass",param);
  if(JJ.M() < 750) return "false";
  
  double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  
  if(zeppenfeld > 0.75) return "false";
  
  if(!PassVBF(JetColl,leps,750., true)) return "false";
  
  Fill_RegionPlots(param,"Inclusive"+RegionTag ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);
  
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_VBF",param);
  
  double HT(0.);
  for(auto ij : JetColl) HT += ij.Pt();
  

  if(ApplyForSR)Fill_RegionPlots(param,"Pass"+RegionTag ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);


  double htltcut = 1.;
  
  if(PassHMMet     && ApplyForSR) FillCutflow(Reg, w, RegionTag+"_met",param);
  if(PassBJetMVeto && ApplyForSR) FillCutflow(Reg, w, RegionTag+"_bveto",param);

  if(PassRegionReq){
    if (HT/leps[0]->Pt() < htltcut){
      
      if(ApplyForSR)      FillCutflow(Reg, w, RegionTag+"_ht_lt1",param);
      double PTBin[5] = { 0.,50, 100., 120., 200};
      double L2Pt = (leps[1]->Pt() > 200) ? 199. : leps[1]->Pt();
      if(ApplyForSR)FillHist( "LimitSR2/"+param.Name+"/PT2_LT1_Central",  L2Pt,  w, 4, PTBin, "Reco PTLep2");
      
      
      if (L2Pt > 120)      return RegionTag+"_HTLTbin1";
      return RegionTag+"_HTLTbin2";
    }
  }
  return "false";
}



bool  HNL_RegionDefinitions::RunSignalRegionAK4(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){



  TString SR3String = RunSignalRegionAK4String(ApplyForSR,channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR3String == "false") return false;
  else return true;

}




TString HNL_RegionDefinitions::RunSignalRegionAK4StringBDT(bool ApplyForSR, TString mN, TString NCut, TString NTree, HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> LepTColl, std::vector<Jet> JetColl, std::vector<Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR3BDT : HNL_LeptonCore::CR3BDT;

  double met_cut     = 15.;
  vector<FatJet> AK8_JetColl;
  double met2_st     = GetMET2ST(LepTColl, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;


  TString RegionTag                = ApplyForSR ? "SR3" : "CR3";

  if(!CheckLeptonFlavourForChannel(channel, LepTColl)) return "false";
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_lep_pt",param);

  if (LepTColl.size() != 2) return "false";

  if(qq==Plus && LepTColl[0]->Charge() < 0) return "false";
  if(qq==Minus && LepTColl[0]->Charge() > 0) return "false";

  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_lep_charge",param);

  Particle ll =  (*LepTColl[0]) + (*LepTColl[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < 10)) return "false";

  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_dilep_mass",param);

  float MVAvalue = EvaluateEventMVA(mN, NCut,NTree, channel,  LepTColl,ev, METv,param);
  //cout << "Summary of BDT " << mN << endl;
  //cout << "Predetermined value = " << MVAvalue << " " <<  ev.HNL_MVA_Event(GetChannelString(channel)+"_"+mN) << endl;
  
  if(ApplyForSR)FillHist(param.Name+"/LimitShape_SR3BDT/SignalBins_M"+mN+"_NCut"+NCut+"_NTree"+NTree, MVAvalue, w, 40, -1., 1.);

  std::vector<FatJet> FatJetColl;

  if(!PassRegionReq)  return "false";
  
  if(MVAvalue< 0.0)        return RegionTag+"_BDTbin1";
  else if(MVAvalue< 0.10)  return RegionTag+"_BDTbin2";
  else if(MVAvalue< 0.125) return RegionTag+"_BDTbin3";
  else if(MVAvalue< 0.15)  return RegionTag+"_BDTbin4";
  else if(MVAvalue< 0.175) return RegionTag+"_BDTbin5";
  else if(MVAvalue< 0.2)   return RegionTag+"_BDTbin6";
  else if(MVAvalue< 0.225) return RegionTag+"_BDTbin7";
  else  return RegionTag+"_BDTbin8";

  return "true";
}


TString HNL_RegionDefinitions::RunSignalRegionAK4String(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR3 : HNL_LeptonCore::CR3;

  double met_cut     = 15.;
  double met2_st     = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;

  TString RegionTag  = ApplyForSR ? "SR3" : "CR3";
      
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";

  if(ApplyForSR) FillCutflow(Reg, w, RegionTag+"_lep_pt",param);
  if(ApplyForSR) Fill_RegionPlots(param,"InclusiveSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus  && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  if(ApplyForSR) FillCutflow(Reg, w, RegionTag+"_lep_charge",param);

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < 10)) {
    if(ApplyForSR)Fill_RegionPlots(param,RegionTag+"ZPeak" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
    return "false";
  }

  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_dilep_mass",param);

  if(ApplyForSR&&PassRegionReq){
    if(JetColl.size()==0)  Fill_RegionPlots(param,"ZeroJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
    if(JetColl.size()==1)  Fill_RegionPlots(param,"OneJetSR3" ,  TauColl,JetColl, AK8_JetColl, leps,  METv, nPV, w);
  }
  
  
  if(PassRegionReq && JetColl.size() == 0 && leps[1]->Pt() > 140.) {
    if(ApplyForSR)FillHist( param.Name+"/LimitShape_SR3/SignalBins",   0.5, w, 16, 0, 16.,  "Signalbins");
    if(ApplyForSR)FillHist( param.Name+"/LimitShape_SR3/SignalBinsQ",   0.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_0JetBinHPT",param);

    return RegionTag+"_bin1";
  }
  if(PassRegionReq && JetColl.size() == 1 && leps[1]->Pt() > 140.) {
    if(ApplyForSR)FillHist( param.Name+"/LimitShape_SR3/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");
    if(ApplyForSR)FillHist( param.Name+"/LimitShape_SR3/SignalBinsQ",   1.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_1JetBinHPT",param);
    return RegionTag+"_bin2";
  }
  
  if(JetColl.size() < 2)   return "false";
  
  if(ApplyForSR)FillCutflow(Reg, w, RegionTag+"_dijet",param);

                                                           
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_Wmass",param);

  if(JetColl[0].Pt() < 25.)  return "false";
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_J1Pt",param);
   
 
  if(PassHMMet&& ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_MET",param);
  if(PassRegionReq && ApplyForSR)  {
    Fill_RegionPlots(param,"DiJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
    FillCutflow(Reg, w, RegionTag+"_bveto",param);
  }

  if(!PassRegionReq) return "false";
 
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

  if(Wcand.M() < 400) return RegionTag+"_bin3";
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_W1Mass",param);
  if(leps[1]->Pt() < 20) return RegionTag+"_bin3";

  if(leps[0]->Pt() < pt_bin2) return RegionTag+"_bin3";
  if(ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_LepPt",param);

  if(met2_st < 2){
    if(N1cand.M() < 500.)   return RegionTag+"_bin4";
    if(N1cand.M() < 750.)   return RegionTag+"_bin5";
    return RegionTag+"_bin6";
  }
  else   if(met2_st < 7){
    if(N1cand.M() < 500.)   return RegionTag+"_bin7";
    if(N1cand.M() < 750.)   return RegionTag+"_bin8";
    return RegionTag+"_bin9";
  }
  else{
    if(N1cand.M() < 750.)   return RegionTag+"_bin10";
  }
  return RegionTag+"_bin11";

  
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



