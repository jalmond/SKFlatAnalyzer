#include "HNL_RegionForLeptonOpt.h"


/*
  
  Mirror of HNL_RegionDefinitions but allows var and cuts to be modified

 */





void HNL_RegionForLeptonOpt::RunAllSignalRegions(HNL_LeptonCore::ChargeType qq,
                                                   std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Tau> TauColl,
                                                   std::vector<Jet> AllJetColl ,  std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl,
                                                   std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1,
                                                   Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){

  

  TString SR1_Binning_Version = "Binning_Version2";
  TString SR2_Binning_Version = "Binning_Version2";
  TString SR3_Binning_Version = "Binning_Version6";


  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};
  
  for(auto dilep_channel : channels){
    
    float weight_channel = weight_ll;

    /// SelectChannel separates OS/SS samples and EE/MM since signales have all processes in sample                                                                                                                                          
    if(MCSample.Contains("Type")&& !SelectChannel(dilep_channel)) continue;

    if (param.Name.Contains("MuOpt") && dilep_channel != MuMu) continue;
    if (param.Name.Contains("ElOpt") && dilep_channel != EE) continue;

    if(run_Debug)cout << "ID Mu  " << param.Muon_Tight_ID << " " << param.Muon_FR_ID << endl;
    if(run_Debug)cout << "ID El  " << param.Electron_Tight_ID << " " << param.Electron_FR_ID << endl;

    std::vector<Lepton *> leps       = MakeLeptonPointerVector(muons,electrons,param);

    TString channel_string = GetChannelString(dilep_channel);

    AnalyzerParameter param_channel = param;

    param_channel.Name =  channel_string + "/" + param_channel.Name;

    FillEventCutflow(HNL_LeptonCore::ChannelDepInc, weight_channel, GetChannelString(dilep_channel) +"_NoCut", "ChannelCutFlow/"+param_channel.Name,param_channel.WriteOutVerbose);//DEf                                                            

    
    HNL_LeptonCore::SearchRegion Regions = HNL_LeptonCore::MuonSRSummary;
    if (dilep_channel == EE)     Regions = HNL_LeptonCore::ElectronSRSummary;
    if (dilep_channel == EMu)    Regions = HNL_LeptonCore::ElectronMuonSRSummary;

    if(param_channel.Name.Contains("HNTightV2")) FillEventCutflow(Regions,  weight_channel, "Inclusive","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);

    if(!PassGenMatchFilter(leps, param_channel)) continue;

    FillEventCutflow(Regions,  weight_channel, "GenMatch","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);

    // CheckLeptonFlavourForChannel checks 2 leptons of type according rto dilep_channel are present in event and passes Trigger looses cuts                                                                                                 
    if(!CheckLeptonFlavourForChannel(dilep_channel, leps)) continue;
    FillEventCutflow(Regions,  weight_channel, "CheckLeptonFlavourForChannel","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);


    /// Weight MC based on corr                                                                                                                                                                                                              
    if(!IsDATA && dilep_channel != MuMu)  weight_channel*= GetElectronSFEventWeight(electrons, param_channel);
    if(!IsDATA && dilep_channel != EE)    weight_channel*= GetMuonSFEventWeight(muons, param_channel);

    if(!PassMETFilter()) return;
    FillEventCutflow(Regions,  weight_channel, "METFilter","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);


    std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);

    if(leps.size() ==2)  FillEventCutflow(HNL_LeptonCore::ChannelDepDilep, weight_channel, GetChannelString(dilep_channel) +"_Dilep", "ChannelCutFlow/"+param_channel.Name,param.WriteOutVerbose);//Def                                      
    
    if(RunCF){
      if(dilep_channel == MuMu) continue;
      if(IsData && SameCharge(leps)) continue;
      if(!IsData && !SameCharge(leps)) continue;
      if(IsData)weight_channel = GetCFWeightElectron(leps, param_channel);
      if(IsData)FillWeightHist(param_channel.Name+"/CFWeight",weight_channel);
    }
    else{
      if(!SameCharge(leps)) continue;
    }

    FillEventCutflow(Regions,  weight_channel, "CFCut","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);

    if(RunFake&& IsData){

      if(run_Debug)cout << "ID2 Mu  " << param_channel.Muon_Tight_ID << " " << param_channel.Muon_FR_ID << endl;
      if(run_Debug)cout << "ID2 El  " << param_channel.Electron_Tight_ID << " " << param_channel.Electron_FR_ID << endl;

      weight_channel = GetFakeWeight(leps, param_channel, false);

      if(run_Debug)cout << "Fake " << weight_channel << endl;
      FillWeightHist(param_channel.Name+"/FakeWeight",weight_channel);
    }

    // Run PRESEL + SR1-3                                                                                                                                                                                                                    
    //bool run_ORTrigger = false;                                                                                                                                                                                                            
    HNL_LeptonCore::SearchRegion LimitRegions = HNL_LeptonCore::MuonSROpt;
    if (dilep_channel == EE) LimitRegions =HNL_LeptonCore::ElectronSROpt;
    if (dilep_channel == EMu) LimitRegions =HNL_LeptonCore::ElectronMuonSROpt;

    HNL_LeptonCore::SearchRegion LimitRegionsBDT = HNL_LeptonCore::MuonSRBDTOpt;
    if (dilep_channel == EE) LimitRegionsBDT =HNL_LeptonCore::ElectronSRBDTOpt;
    if (dilep_channel == EMu) LimitRegionsBDT =HNL_LeptonCore::ElectronMuonSRBDTOpt;

    
    if(!PassPreselection(dilep_channel,qq, leps, leps_veto, TauColl, JetColl, VBF_JetColl, AK8_JetColl, B_JetColl,ev, METv ,param_channel,"", weight_channel)) continue;

    FillEventCutflow(Regions,  weight_channel, "Preselection","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);

    vector<TString> Masses    = {"600", "700","800","1000", "2000" };
    vector<TString> BDTMasses = {"100","400","500"};


    if(AK8_JetColl.size() > 0) {

      FillEventCutflow(Regions,  weight_channel, "AK8","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);

            
      TString SRbin= RunSignalRegionAK8String ("---", dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetCollSR1,ev, METv ,param_channel,SR1_Binning_Version, weight_channel) ;
      
      ////// Fill Limit plot                                                                                                                                                                                                               
      if(SRbin != "false") {
	FillEventCutflow(LimitRegions, weight_channel, SRbin,"LimitInput/"+param_channel.Name);
	for(auto bdt_mass : BDTMasses) FillEventCutflow(LimitRegionsBDT, weight_channel, SRbin,"LimitInputBDT/"+param_channel.Name+"/"+bdt_mass);
      }

      ///// Fill Cutflow                                                                                                                                                                                                                   
      if(SRbin != "false")  FillEventCutflow(Regions,  weight_channel, "SigReg1","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);
      else FillEventCutflow(Regions,  weight_channel, "SigReg1Fail","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);
      
    }
    else{


      TString SR2bin = RunSignalRegionWWString( dilep_channel,qq, leps, leps_veto,  TauColl, JetCollLoose, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param_channel,  SR2_Binning_Version, weight_channel);

      if(SR2bin != "false"){
	FillEventCutflow(LimitRegions, weight_channel, SR2bin,"LimitInput/"+param_channel.Name);
        for(auto bdt_mass : BDTMasses) FillEventCutflow(LimitRegionsBDT, weight_channel, SR2bin,"LimitInputBDT/"+param_channel.Name+"/"+bdt_mass);
        FillEventCutflow(Regions,  weight_channel, "SigReg2","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);
      }
      else{

        FillEventCutflow(Regions,  weight_channel, "SigReg3","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);

        TString SR3bin  = RunSignalRegionAK4String (dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param_channel,SR3_Binning_Version, weight_channel);
        TString SRBDT100 = RunSignalRegionAK4StringBDT("100", dilep_channel, leps,JetColl,B_JetColl, METv , ev);
        TString SRBDT400 = RunSignalRegionAK4StringBDT("400", dilep_channel, leps,JetColl,B_JetColl, METv, ev);
	TString SRBDT500 = RunSignalRegionAK4StringBDT("500", dilep_channel, leps,JetColl,B_JetColl, METv, ev);

        if(SRBDT100 != "false") FillEventCutflow(LimitRegionsBDT, weight_channel, SRBDT100,"LimitInputBDT/"+param_channel.Name+"/100");
        if(SRBDT400 != "false") FillEventCutflow(LimitRegionsBDT, weight_channel, SRBDT400,"LimitInputBDT/"+param_channel.Name+"/400");
	if(SRBDT500 != "false") FillEventCutflow(LimitRegionsBDT, weight_channel, SRBDT500,"LimitInputBDT/"+param_channel.Name+"/500");

        if(SR3bin != "false") {

	  FillEventCutflow(LimitRegions, weight_channel, SR3bin, "LimitInput/"+param_channel.Name);
          FillEventCutflow(Regions,  weight_channel, "SigReg3Pass","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);
        }
        else           FillEventCutflow(Regions,  weight_channel, "SigReg3Fail","Summary/"+param_channel.Name,param_channel.WriteOutVerbose);

      }

    }

  }
  return;
}



bool  HNL_RegionForLeptonOpt::PassPreselection(HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType qq, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto, vector<Tau> TauColl,  std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString TriggerOption ,  float w){

  // ONLY CODE SS PRESLECTION 
  
  // APPLY PT CUTS
  /*
    ee channel 25/10
    emu   25/5
    mm    20/5
    
    THIS IS APPLIED IN PassTriggerSelection
    
  */
  
  if(run_Debug) cout << "HNL_RegionForLeptonOpt::PassPreselection " << GetChannelString(channel) <<  endl;

  // Make sure events contain 2 leps                                                                                                       
  // Make sure correct leptons are used                                                                                                                                      

  if (! (  PassMultiTriggerSelection(channel, ev, leps,"Dilep", "HighPt") || PassMultiTriggerSelection(channel, ev, leps,"Dilep", "Lep"))) return false;
  
  FillEventCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_MultiTrigger", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
  
  if (!PassTriggerSelection(channel, ev, leps,"Dilep")) return false;
  
  
  w *= HNLZvtxSF(channel);

  
  FillEventCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_Trigger", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def


  if(run_Debug) cout << "HNL_RegionForLeptonOpt::PassTriggerSelection " << GetChannelString(channel) <<  endl;

  
  int nel_hem(0);
  if (channel==EE){
    if (DataEra=="2018"){
      for(auto iel : leps){
	if (iel->Eta() < -1.25){
          if((iel->Phi() < -0.82) && (iel->Phi() > -1.62)) nel_hem++;
	}
      }
    }
  }

  if(nel_hem > 0) return false;


  // Make sure events contain 2 leps
  if (leps_veto.size() != 2) return false;

  
  // CUT ON MASS OF LL PAIR

  Particle ll =  (*leps[0]) + (*leps[1]);
  
  // VETO Z PEAK IN EE CHANNEL

  double mz_cut = 10.;
  if (channel==EE  && (fabs(ll.M()-90.) < mz_cut)) return false;

  if(ll.M() < 10) return false; // TO_CHECK: IS 20 BEST OPTION

  Fill_RegionPlots(channel, 0, param.Name,"Preselection" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_Presel", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
  
  return true;
}


bool  HNL_RegionForLeptonOpt::RunSignalRegionAK8(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  TString SR1String = RunSignalRegionAK8String("",channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  
  if(SR1String == "false") return false;
  else return true;
  
}


/// Return TString for Limit bin

TString HNL_RegionForLeptonOpt::RunSignalRegionAK8String(TString mass, HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString Modifier ,  float w){

    
  TString PNMass = param.Name+"_"+mass;
  if (mass == "---") PNMass = param.Name;

  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_Init",PNMass,param.WriteOutVerbose);
  
  if (leps_veto.size() != 2) return "false";
  FillEventCutflow(HNL_LeptonCore::SR1,w, "SR1_lep_pt",PNMass,param.WriteOutVerbose);
  if(AK8_JetColl.size() == 0) return "false";
  
  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::ChannelDepSR1, w, GetChannelString(channel) +"_SR1", "ChannelCutFlow/"+PNMass,param.WriteOutVerbose);//Def                                          

  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_lep_charge",PNMass,param.WriteOutVerbose);
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_dilep_mass",PNMass,param.WriteOutVerbose);

  double ST = GetST(leps, JetColl, AK8_JetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;

  double met_cut = 15.;
    
  bool PassHMMet    = (met2_st < met_cut);
  bool PassBJetMVeto = (B_JetColl.size()==0);

  double LowerMassSR1WmassCut = 450;
  
  TString signal_region1 = "HNL_SR1";
  
  Fill_RegionPlots(channel, 0, PNMass,"InclusiveR1" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
  
  //  double dPhil2_l = fabs(TVector2::Phi_mpi_pi( ( (AK8_JetColl[0] + *leps[1]).Phi() - (leps[0]->Phi() ))));
  
  double pt_cut = 100.;
  if(DataEra=="2018")  pt_cut = 150.;

  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_1AK8",PNMass,param.WriteOutVerbose);

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

  Particle N1cand = AK8_JetColl[m] + *leps[0];
  Particle Wcand = AK8_JetColl[m] + *leps[0]+*leps[1];
  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();
  
  if(!PassHMMet) return "false";
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_MET",PNMass,param.WriteOutVerbose);


  if(Wcand.M()  < LowerMassSR1WmassCut ) return "false";
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_Wmass",PNMass,param.WriteOutVerbose);


  if(!PassBJetMVeto) return "false";
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_bveto",PNMass,param.WriteOutVerbose);
  
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_PNTagger",PNMass,param.WriteOutVerbose);
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_DphiN_Wlep",PNMass,param.WriteOutVerbose);
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_TauVeto",PNMass,param.WriteOutVerbose);
    
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_HTPt",PNMass,param.WriteOutVerbose);
  Fill_RegionPlots(channel, 0, PNMass,"InclusiveSRHM1" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
  
  TString SR1Bin="SR1_MNbin1";
  
  if(MN1 > 225){
    if(leps[0]->Pt() < pt_cut)  return "SR1_MNbin1";
    FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_LepPt",PNMass,param.WriteOutVerbose);
    }
  else   if(MN1 > 150){
    if(leps[0]->Pt() < pt_cut-50)  return "SR1_MNbin1";
    FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_LepPt",PNMass,param.WriteOutVerbose);
  }
  
  
  int nSRbins=4;
  double ml1jbins[nSRbins] = { 0., 500, 750., 2000.};
  for(int ibin=1; ibin < nSRbins; ibin++){
    if(MN1 < ml1jbins[ibin]) return "SR1_MNbin"+to_string(ibin+1);
  }
  cout << "Error in SR1 binning " << Modifier << endl;
  exit(EXIT_FAILURE);
    
  
}



TString HNL_RegionForLeptonOpt::RunSignalRegionAK4StringBDT(TString mN, HNL_LeptonCore::Channel channel, 
							      std::vector<Lepton *> LepTColl,  
							      std::vector<Jet> JetColl,std::vector<Jet> B_JetColl,Particle METv, Event  ev){

  if (LepTColl.size() != 2) return "false";
  Particle ll =  (*LepTColl[0]) + (*LepTColl[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return "false";

  float MVAvalue = ev.HNL_MVA_Event(GetChannelString(channel)+"_"+mN);

  std::vector<FatJet> FatJetColl;
  double ST = GetST( LepTColl, JetColl, FatJetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 15);


  TString LabelPrefix =  "SR3";
  if(!PassHMMet) return "false";
  if(B_JetColl.size() > 0)    return "false";
  
  if(DataYear==2016){
    if(MVAvalue< 0.0) return LabelPrefix+"_BDTbin1";
    else if(MVAvalue< 0.10) return LabelPrefix+"_BDTbin2";
    else if(MVAvalue< 0.125) return LabelPrefix+"_BDTbin3";
    else if(MVAvalue< 0.15) return LabelPrefix+"_BDTbin4";
    else if(MVAvalue< 0.2) return LabelPrefix+"_BDTbin5";
    else if(MVAvalue< 0.225) return LabelPrefix+"_BDTbin6";
    else  return LabelPrefix+"_BDTbin7";

  }
  
  if(DataYear==2017){
    if(MVAvalue< 0.0) return LabelPrefix+"_BDTbin1";
    else if(MVAvalue< 0.10) return LabelPrefix+"_BDTbin2";
    else if(MVAvalue< 0.125) return LabelPrefix+"_BDTbin3";
    else if(MVAvalue< 0.15) return LabelPrefix+"_BDTbin4";
    else if(MVAvalue< 0.2) return LabelPrefix+"_BDTbin5";
    else if(MVAvalue< 0.225) return LabelPrefix+"_BDTbin6";
    else  return LabelPrefix+"_BDTbin7";
  }

  if(DataYear==2018){
    if(MVAvalue< 0.0) return LabelPrefix+"_BDTbin1";
    else if(MVAvalue< 0.10) return LabelPrefix+"_BDTbin2";
    else if(MVAvalue< 0.125) return LabelPrefix+"_BDTbin3";
    else if(MVAvalue< 0.15) return LabelPrefix+"_BDTbin4";
    else if(MVAvalue< 0.2) return LabelPrefix+"_BDTbin5";
    else if(MVAvalue< 0.225) return LabelPrefix+"_BDTbin6";
    else  return LabelPrefix+"_BDTbin7";

  }
  return "true";
}



bool  HNL_RegionForLeptonOpt::RunSignalRegionWW(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,
						  std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, 
						  std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, 
						  Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){
  
  TString SR2String = RunSignalRegionWWString(channel, qq, leps, leps_veto, TauColl,JetCollLoose, JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR2String == "false") return false;
  else return true;
  
}



TString HNL_RegionForLeptonOpt::RunSignalRegionWWString(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,
							  std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, 
							  std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, 
							  Event ev, Particle METv, AnalyzerParameter param, TString Modifier ,  float w){
  
  
  if (leps_veto.size() != 2) return "false";


  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::SR2,w, "SR2_lep_charge",param.Name,param.WriteOutVerbose);

  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_lep_pt",param.Name,param.WriteOutVerbose);

  Fill_RegionPlots(channel, 0, param.Name,"InclusiveSR2" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);


  
  if(!PassVBF(JetColl,leps,750., param, w, true)) return "false";

  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_VBF",param.Name,param.WriteOutVerbose);

  bool PassBJetMVeto = (B_JetColl.size()==0);

  double HT(0.);
  
  for(auto ij : JetColl) HT += ij.Pt();
  
  double htltcut = 1.;
  
  double ST = GetST( leps, JetColl, AK8_JetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;

  double met_cut = 10.;

  bool PassHMMet    = (met2_st < met_cut); 
  
  if(!PassHMMet) return "false";
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_met",param.Name,param.WriteOutVerbose);
  if(!PassBJetMVeto)return"false";
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_bveto",param.Name,param.WriteOutVerbose);    
  if ( (HT/leps[0]->Pt()) > htltcut) return"false";
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_ht_lt1",param.Name,param.WriteOutVerbose);

  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_Final",param.Name,param.WriteOutVerbose);

  
  double PTBin[5] = { 0.,50, 100., 120., 200};
  double L2Pt = (leps[1]->Pt() > 200) ? 199. : leps[1]->Pt();
  if(param.WriteOutVerbose == 0)FillHist( "LimitSR2/"+param.Name+"/PT2_LT1_Central",  L2Pt,  w, 4, PTBin, "Reco PTLep2");
  
  
  if (L2Pt > 120)      return "SR2_HTLTbin1";
  return "SR2_HTLTbin2";
  
  return "false";
}



bool  HNL_RegionForLeptonOpt::RunSignalRegionAK4(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){



  TString SR3String = RunSignalRegionAK4String(channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR3String == "false") return false;
  else return true;

}


TString HNL_RegionForLeptonOpt::RunSignalRegionAK4String(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString Modifier ,  float w){
                                       
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_pt",param.Name,param.WriteOutVerbose);

  if (leps_veto.size() != 2) {
    FillEventCutflow(HNL_LeptonCore::SR3Fail, w, "SR3_2vl",param.Name,param.WriteOutVerbose);
    return "false";
  }

  if(qq==Plus && leps[0]->Charge() < 0)  return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_charge",param.Name,param.WriteOutVerbose);

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) {
    FillEventCutflow(HNL_LeptonCore::SR3Fail, w, "SR3_mll",param.Name,param.WriteOutVerbose);
    return "false";
  }

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_dilep_mass",param.Name,param.WriteOutVerbose);

  double ST      = GetST(leps, JetColl, AK8_JetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;

  double met_cut = 15;
  bool PassHMMet  = (met2_st < met_cut);

  int  NB_JetColl      =  B_JetColl.size();

  double pt_bin1= 140.;

  double pt_bin2= 140.;
  
  
  if(NB_JetColl>  0)  {
    FillEventCutflow(HNL_LeptonCore::SR3Fail, w, "SR3_bveto",param.Name,param.WriteOutVerbose);
    return "false";
  }
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_bveto",param.Name,param.WriteOutVerbose);

  if(!PassHMMet) {
    FillEventCutflow(HNL_LeptonCore::SR3Fail, w, "SR3_met",param.Name,param.WriteOutVerbose);
    return "false";
  }
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_MET",param.Name,param.WriteOutVerbose);  


  //////// SR1  PASS MET and BJet veto

  if(JetColl.size() == 0 && leps[1]->Pt() > pt_bin1) {
    if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBins",   0.5, w, 16, 0, 16.,  "Signalbins");
    if(param.WriteOutVerbose == 0)    FillHist( "LimitSR3/"+param.Name+"/SignalBinsQ",   0.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_0JetBin",param.Name,param.WriteOutVerbose);

    return "SR3_bin1";
  }
  if(JetColl.size() == 1 && leps[1]->Pt() > pt_bin1) {
    if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");
    if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBinsQ",   1.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_1JetBin",param.Name,param.WriteOutVerbose);

    return "SR3_bin2";
  }

  if(JetColl.size() >0)FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_jet",param.Name,param.WriteOutVerbose);

  if(JetColl.size() < 2) {
    FillEventCutflow(HNL_LeptonCore::SR3Fail, w, "SR3_dijet",param.Name,param.WriteOutVerbose);
    return "false";
  }
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_dijet",param.Name,param.WriteOutVerbose);

  if(Modifier == "MassDep_EXO17028")      return "SR3_bin3";

  if(JetColl[0].Pt() < 25.)  {
    FillEventCutflow(HNL_LeptonCore::SR3Fail, w, "SR3_0jpt",param.Name,param.WriteOutVerbose);
    return "false";
  }
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_J1Pt",param.Name,param.WriteOutVerbose);
 
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

  if(Wcand.M() < 400) return "SR3_bin3";
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_W1Mass",param.Name,param.WriteOutVerbose);

  if(leps[1]->Pt() < 20) return "SR3_bin3";
  if(leps[0]->Pt() < pt_bin2) return "SR3_bin3";
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_LepPt",param.Name,param.WriteOutVerbose);

  Fill_RegionPlots(channel, 0, param.Name,"InclusiveSRHM3" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);

  //double PhiComp = fabs(TVector2::Phi_mpi_pi( ( (NCand.Phi() - Wlep.Phi() ))));
  /// < 1.5 ? > 1.5
  //NCand.DeltaR(Wlep)

  FillEventCutflow(HNL_LeptonCore::ChannelDepSR3, w, GetChannelString(channel) +"_SR3", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def          
  

  double Lep_LT = (leps[0]->Pt()+leps[1]->Pt());
  
  if(Lep_LT > 400){
    if(N1cand.M() < 500.)   return "SR3_bin4";
    if(N1cand.M() < 750.)   return "SR3_bin5";
    return "SR3_bin6";
  }
  else{ 
    if(met2_st < 3){
      if(N1cand.M() < 500.)   return "SR3_bin7";
      if(N1cand.M() < 750.)   return "SR3_bin8";
      return "SR3_bin9";
    }
    else{
      if(N1cand.M() < 500.)   return "SR3_bin10";
      return "SR3_bin11";
    }
  }
  
  
  return "false";
  
}





HNL_RegionForLeptonOpt::HNL_RegionForLeptonOpt(){
      


}
 
HNL_RegionForLeptonOpt::~HNL_RegionForLeptonOpt(){

}




bool HNL_RegionForLeptonOpt::PassVBFInitial(vector<Jet>  JetColl){

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

bool HNL_RegionForLeptonOpt::PassVBF(vector<Jet>  JetColl,std::vector<Lepton *> leps, double mjj_cut ,AnalyzerParameter param,double w , bool use_leadjets){

  if(leps.size() != 2) return false;
  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi < 2.) return false;
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_DPhi",param.Name,param.WriteOutVerbose);

  if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return false;
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_LLMass",param.Name,param.WriteOutVerbose);

  if(JetColl.size() < 2) return false;
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJet",param.Name,param.WriteOutVerbose);


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
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetEta",param.Name,param.WriteOutVerbose);

  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  if(JJ.M() < mjj_cut) return false;
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetMass",param.Name,param.WriteOutVerbose);

  double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;

  if(zeppenfeld > 0.75) return false;

  return true;
}






