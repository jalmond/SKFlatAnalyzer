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



void HNL_RegionDefinitions::RunAllSignalRegions(HNL_LeptonCore::ChargeType qq, 
                        std::vector<Electron> electronsInitial, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Tau> TauColl, 
                        std::vector<Jet> JetLooseColl,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, 
                        Event ev,   Particle METv, AnalyzerParameter param, int nElForRunCF,   float weight_ll){



  std::vector<Electron> electrons;
  if(RunCF) {
    /// Add code to smear individual electron for CF Bkg                                                                                                                                                                                                                                   
    for(unsigned int i=0; i<electronsInitial.size(); i++){
      Electron this_electron = electronsInitial.at(i);
      double ElEnergyShift = 1;

      if(int(i)==nElForRunCF) ElEnergyShift = GetShiftCFEl(this_electron, param.Electron_Tight_ID);

      this_electron.SetPtEtaPhiM( electronsInitial.at(i).Pt()*ElEnergyShift, electronsInitial.at(i).Eta(), electronsInitial.at(i).Phi(), electronsInitial.at(i).M() );
      electrons.push_back( this_electron);
    }
  }
  else  electrons = electronsInitial;

  vector<HNL_LeptonCore::Channel> channels = {GetChannelENum(param.Channel)};

  if(GetChannelENum(param.Channel) == HNL_LeptonCore::NONE){
    cout << "CHANNEL NOT SET" << endl;
    exit(EXIT_FAILURE);
  }

  HNL_LeptonCore::SearchRegion CutFlow_Region = SignalRegion;

  for(auto dilep_channel : channels){
    
    double  weight_channel = weight_ll;
    // DYTypeI, VBFTypeI EMu or MuE xsec should be a half of that of EE or MuMu.
    if ( ((MCSample.Contains("DYTypeI")||MCSample.Contains("VBFTypeI")) && MCSample.Contains("private")) && (dilep_channel == EMu) ) weight_channel *= 0.5;

    //// Select CHannel used for Signals to check if signal is EE/MM/Emu using gen info
    if(MCSample.Contains("Type")&& !SelectChannel(dilep_channel)) continue;
  
    FillHist( param.Name+"/RunAllSignalRegions_NOCUT_ALLCHANNEL",  1,  weight_ll, 2,0,2);

    TString channel_string = GetChannelString(dilep_channel);
    param.CutFlowDir = "CutFlow";
    param.Name = param.Name  + "/"+channel_string;

    std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(muons,     electrons,     param);
    std::vector<Lepton *> LepsV       = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
   
    FillCutflow(CutFlow_Region, weight_ll, "NoCut", param);

    if(!PassGenMatchFilter(LepsT,param)) continue;
    FillCutflow(CutFlow_Region, weight_ll, "GENMatched",param);
    
    if(!PassHEMVeto(LepsV)) continue;
    FillCutflow(CutFlow_Region, weight_ll, "HEMVeto", param);

    if(!PassMETFilter()) return;

    FillCutflow(CutFlow_Region, weight_ll, "METFilter",param);

    if(!ConversionSplitting(LepsT,RunConv,2)) continue;

    FillCutflow(CutFlow_Region, weight_ll, "ConvFilter",param);

    FillCutflow(HNL_LeptonCore::ChannelDepInc, weight_channel, GetChannelString(dilep_channel) +"_NoCut",param);
    

    if(! CheckLeptonFlavourForChannel(dilep_channel, LepsT))  continue;
    FillCutflow(CutFlow_Region, weight_channel, "LeptonFlavour",param);
    
    if(LepsT.size() ==2)  FillCutflow(HNL_LeptonCore::ChannelDepDilep, weight_channel, GetChannelString(dilep_channel) +"_Dilep",param);
    
    if(RunCF){
      if(dilep_channel == MuMu) continue;
      
      if(IsData && SameCharge(LepsT)) continue;
      if(!IsData && !SameCharge(LepsT)) continue;
      
      if(IsData)weight_channel = GetCFWeightElectron(LepsT, param,nElForRunCF,true);
      if(IsData)FillWeightHist(param.Name+"/CFWeight",weight_channel);

    }
    else{
      if(!SameCharge(LepsT)) continue;
    }
 
    if(RunFake&& IsData){
      if(_jentry%nLog==0){
    cout << "Running Fakes... " <<  GetFakeWeight(LepsT, param) << "  " << GetFakeWeight(LepsT, param) << endl;
      }

      weight_channel = GetFakeWeight(LepsT, param);
      FillFakeWeightHist(param.Name+"/FakeWeight", LepsT,param, weight_channel);
    }

    if (!PassTriggerSelection(dilep_channel, ev, LepsT,param.TriggerSelection)) continue;
    
    EvalTrigWeight(dilep_channel, muons,electrons,param, ev,weight_channel);

    FillCutflow(CutFlow_Region, weight_channel, "Trigger",param);


    /// RunMainRegionCode runs SR1/SR2/SR3
    
    if(isnan(weight_channel)) {
      cout << "Weight for event is set to Nan..." << endl;
      exit(EXIT_FAILURE);
    }
    
    if(HasFlag("RunCR")) RunMainRegionCode(false,dilep_channel,Inclusive, LepsT, LepsV, TauColl,JetLooseColl, JetColl,  VBF_JetColl, AK8_JetColl, B_JetColl,ev, METv ,param, weight_channel);
    else     RunMainRegionCode(true,dilep_channel,Inclusive, LepsT, LepsV, TauColl, JetLooseColl,JetColl, VBF_JetColl, AK8_JetColl, B_JetColl,ev, METv ,param, weight_channel);
  }
}

void   HNL_RegionDefinitions::RunMainRegionCode(bool IsSR,HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType qq, std::vector<Lepton *> LepsT,std::vector<Lepton *> LepsV, vector<Tau> TauColl,  std::vector<Jet> JetLooseColl, std::vector<Jet> JetColl,       std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param,  float weight_reg){
  

  TString channel_string = GetChannelString(channel);

  HNL_LeptonCore::SearchRegion LimitRegions,LimitRegionsBDT,LimitRegionR1,LimitRegionR2,LimitRegionR3,LimitRegionsBDTR3;
  if(IsSR){
  
    LimitRegions    = HNL_LeptonCore::MuonSR;
    LimitRegionR1  = HNL_LeptonCore::MuonSR1;
    LimitRegionR2  = HNL_LeptonCore::MuonSR2;    
    LimitRegionR3  = HNL_LeptonCore::MuonSR3;    
    LimitRegionsBDT = HNL_LeptonCore::MuonSRBDT;
    LimitRegionsBDTR3 = HNL_LeptonCore::MuonSR3BDT;
    
    if (channel == EE) {
      LimitRegions =HNL_LeptonCore::ElectronSR;
      LimitRegionR1  = HNL_LeptonCore::ElectronSR1;
      LimitRegionR2  = HNL_LeptonCore::ElectronSR2;
      LimitRegionR3  = HNL_LeptonCore::ElectronSR3;
      LimitRegionsBDT = HNL_LeptonCore::ElectronSRBDT;
      LimitRegionsBDTR3 = HNL_LeptonCore::ElectronSR3BDT;
    }
    if (channel == EMu) {
      LimitRegions =HNL_LeptonCore::ElectronMuonSR;
      LimitRegionR1  = HNL_LeptonCore::ElectronMuonSR1;
      LimitRegionR2  = HNL_LeptonCore::ElectronMuonSR2;
      LimitRegionR3  = HNL_LeptonCore::ElectronMuonSR3;
      LimitRegionsBDT = HNL_LeptonCore::ElectronMuonSRBDT;
      LimitRegionsBDTR3 = HNL_LeptonCore::ElectronMuonSR3BDT;
    }
  }
  else{
    
    LimitRegions    = HNL_LeptonCore::MuonCR;
    LimitRegionR1  = HNL_LeptonCore::MuonCR1;
    LimitRegionR2  = HNL_LeptonCore::MuonCR2;
    LimitRegionR3  = HNL_LeptonCore::MuonCR3;
    LimitRegionsBDT = HNL_LeptonCore::MuonCRBDT;
    LimitRegionsBDTR3 = HNL_LeptonCore::MuonCR3BDT;

    if (channel == EE) {
      LimitRegions =HNL_LeptonCore::ElectronCR;
      LimitRegionR1  = HNL_LeptonCore::ElectronCR1;
      LimitRegionR2  = HNL_LeptonCore::ElectronCR2;
      LimitRegionR3  = HNL_LeptonCore::ElectronCR3;
      LimitRegionsBDT = HNL_LeptonCore::ElectronCRBDT;
      LimitRegionsBDTR3 = HNL_LeptonCore::ElectronCR3BDT;
    }
    if (channel == EMu) {
      LimitRegions =HNL_LeptonCore::ElectronMuonCR;
      LimitRegionR1  = HNL_LeptonCore::ElectronMuonCR1;
      LimitRegionR2  = HNL_LeptonCore::ElectronMuonCR2;
      LimitRegionR3  = HNL_LeptonCore::ElectronMuonCR3;
      LimitRegionsBDT = HNL_LeptonCore::ElectronMuonCRBDT;
      LimitRegionsBDTR3 = HNL_LeptonCore::ElectronMuonCR3BDT;
    }
  }
  
  if(!PassPreselection(IsSR,channel,Inclusive, 
               LepsT, LepsV, TauColl, 
               JetColl, VBF_JetColl, AK8_JetColl, B_JetColl,
               ev, METv ,param, weight_reg)) return;
  

  if(AK8_JetColl.size() > 0) {
    
    TString RegionBin= RunSignalRegionAK8String (IsSR,channel,qq, LepsT, LepsV, TauColl, 
                         JetColl, AK8_JetColl,B_JetColl,
                         ev, METv ,param,weight_reg) ;
    
    if(RegionBin != "false") {

      /// Region 1+2+3
      FillLimitInput(LimitRegions, weight_reg,   RegionBin,  "LimitExtraction/"+param.Name);

      if(IsSR) Fill_RegionPlots(param,"AllSR" , TauColl, 
                JetColl, AK8_JetColl, LepsT, 
                METv, nPV, weight_reg);

      if(IsSR) FillCutflow(HNL_LeptonCore::ChannelDepSR1, weight_reg, channel_string +"_SR1",param);
      else FillCutflow(HNL_LeptonCore::ChannelDepCR1, weight_reg, channel_string +"_CR1",param);
      
      //// Region1 only limit
      FillLimitInput(LimitRegionR1, weight_reg,   RegionBin,  "LimitExtraction/"+param.Name);
      
      for(unsigned int im=0; im<MNStrList.size(); im++){
        //// Low Mass BDT Binned R1+2+3 only limit 
        if(RegionBin != "false") FillLimitInput(LimitRegionsBDT, weight_reg, RegionBin,"LimitExtractionBDT/"+param.Name+"/M"+MNStrList[im]);
      }
    }
    return;
  }
  else{

    //// Fail AK8 Req
    TString RegionBin = RunSignalRegionWWString(IsSR, channel,qq, LepsT, LepsV,  TauColl, JetLooseColl, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param,  weight_reg);
    
    if(RegionBin != "false") {
      
      if(IsSR) Fill_RegionPlots(param,"AllSR" , TauColl, JetColl, AK8_JetColl, LepsT,  METv, nPV, weight_reg);

      if(IsSR)FillCutflow(HNL_LeptonCore::ChannelDepSR2, weight_reg, channel_string +"_SR2",param);
      else FillCutflow(HNL_LeptonCore::ChannelDepCR2, weight_reg, channel_string +"_CR2",param);
      
      /// Region 1+2+3
      FillLimitInput(LimitRegions, weight_reg, RegionBin,"LimitExtraction/"+param.Name);

      /// Region 2 only Limit
      FillLimitInput(LimitRegionR2, weight_reg, RegionBin,  "LimitExtraction/"+param.Name);
      
      //// Low Mass BDT Binned R1+2+3 only limit
      for(unsigned int im=0; im<MNStrList.size(); im++) FillLimitInput(LimitRegionsBDT, weight_reg, RegionBin,"LimitExtractionBDT/"+param.Name+"/M"+MNStrList[im]);
    
    }
    else{

      //// Fail VBF Req
      for(auto imapHP :FinalBDTHyperParamMap){
        TString RegBDT = RunSignalRegionAK4StringBDT(IsSR,imapHP.first , imapHP.second.first, imapHP.second.second, channel,qq, LepsT, JetColl,  B_JetColl, ev, METv ,param,weight_reg);

        if(RegBDT != "false"){
          //// Low Mass BDT Binned R1+2+3 only limit input
          FillLimitInput(LimitRegionsBDT, weight_reg, RegBDT,"LimitExtractionBDT/"+param.Name+"/M"+imapHP.first);
          /// R3 LowMass BDt Only limit input
          FillLimitInput(LimitRegionsBDTR3, weight_reg, RegBDT,"LimitExtractionBDT/"+param.Name+"/M"+imapHP.first);
        }
      }
      
      RegionBin  = RunSignalRegionAK4String (IsSR,channel,qq, LepsT, LepsV, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param,weight_reg);
      if(RegionBin != "false") {
        if(IsSR) Fill_RegionPlots(param,"AllSR" , TauColl, JetColl, AK8_JetColl, LepsT,  METv, nPV, weight_reg);
        if(IsSR) FillCutflow(HNL_LeptonCore::ChannelDepSR3HM, weight_reg, channel_string +"_SR3",param);
        else FillCutflow(HNL_LeptonCore::ChannelDepCR3HM, weight_reg, channel_string +"_CR3",param);
        
        //// Binned R1+2+3 only limit input
        FillLimitInput(LimitRegions, weight_reg,   RegionBin,"LimitExtraction/"+param.Name);
        ///  R3 HighMass only limit input
        FillLimitInput(LimitRegionR3, weight_reg, RegionBin,  "LimitExtraction/"+param.Name);
      }
      else{
        if(IsSR){
          if(JetColl.size() == 0)Fill_RegionPlots(param,"FailZeroJet" ,  TauColl, JetColl, AK8_JetColl, LepsT,  METv, nPV, weight_reg);
          if(JetColl.size() == 1)Fill_RegionPlots(param,"Fail1Jet" ,  TauColl, JetColl, AK8_JetColl, LepsT,  METv, nPV, weight_reg);
          if(JetColl.size() > 1) Fill_RegionPlots(param,"FailDiJet" ,  JetColl, AK8_JetColl, LepsT,  METv, nPV, weight_reg);
          
          if(JetColl.size() == 0) FillCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_reg, GetChannelString(channel) +"_SR3Fail_0j",param);
          else if(JetColl.size() == 1)FillCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_reg, GetChannelString(channel) +"_SR3Fail_1j",param);
          else FillCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_reg, GetChannelString(channel) +"_SR3Fail_2j",param);
        }
      }
    }
  }

  return;
}




bool  HNL_RegionDefinitions::PassPreselection(bool ApplyForSR,HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType qq, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto, vector<Tau> TauColl,  std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param,  float w){

  // ONLY CODE SS PRESLECTION 
  
  // APPLY PT CUTS
  /*
    ee channel 25/10
    emu   25/5
    mm    20/5
    
    
   */
  
  if(run_Debug) cout << "HNL_RegionDefinitions::PassPreselection " << GetChannelString(channel) <<  endl;

  FillCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_MultiTrigger",param);
  FillCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_Trigger",param);

  if(!PassHEMVeto(leps)) return false;

  // Make sure events contain 2 leps
  if (leps_veto.size() != 2) return false;
  
  // CUT ON MASS OF LL PAIR

  Particle ll =  (*leps[0]) + (*leps[1]);
  
  // VETO Z PEAK IN EE CHANNEL
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;

  if(ll.M() < M_CUT_LL) return false; // TO_CHECK: IS 20 BEST OPTION

  if(ApplyForSR)Fill_RegionPlots(param,"Preselection" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);
  FillCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_Presel",param);
  
  return true;
}




bool  HNL_RegionDefinitions::RunSignalRegionAK8(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, float w){


  TString SR1String = RunSignalRegionAK8String(ApplyForSR,channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, w);
  
  if(SR1String == "false") return false;
  else return true;
  
}


/// Return TString for Limit bin

TString HNL_RegionDefinitions::RunSignalRegionAK8String(bool ApplyForSR, 
                            HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , 
                            std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl,
                            std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, 
                            Event ev, Particle METv, AnalyzerParameter param,  float w){
   
  TString RegionTag                = ApplyForSR ? "SR1" : "CR1";
  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR1 : HNL_LeptonCore::CR1;

  double met_cut     = ApplyForSR ? 10. : 12;
  double met2_st     = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;

  double LowerMassSR1WmassCut = 450.;

   FillCutflow(Reg, w, RegionTag+"_Init",param);

  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";
   FillCutflow(Reg, w, RegionTag+"_lep_charge",param);

  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";  
  if (leps_veto.size() != 2) return "false";

  if(leps[1]->Pt() < 15 ) return "false";

  FillCutflow(Reg, w, RegionTag+"_lep_pt",param);
  
  Particle ll =  (*leps[0]) + (*leps[1]);
  if(ll.M() < M_CUT_LL) return false;

  FillCutflow(Reg, w, RegionTag+"_dilep_mass",param);
    
  if(AK8_JetColl.size() != 1)  return "false";
  FillCutflow(Reg, w, RegionTag+"_1AK8",param);
 
  if(ApplyForSR) Fill_RegionPlots(param,"Inclusive"+RegionTag , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);

  if(PassHMMet&&ApplyForSR)FillCutflow(Reg, w, RegionTag+"_MET",param);
  
  if(PassRegionReq &&ApplyForSR) FillCutflow(Reg, w, RegionTag+"_bveto",param);    

  if(!PassRegionReq) return "false";
  if(!ApplyForSR) {
    FillCutflow(Reg, w, RegionTag+"_MET",param);
    FillCutflow(Reg, w, RegionTag+"_bveto",param);
  }
  double pt_cut = 100.;
  if(DataEra=="2018")  pt_cut = 200.; 

  //Fill Limit plot
 
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  
  int m=-999;
  for(UInt_t emme=0; emme<AK8_JetColl.size(); emme++){
    dijetmass_tmp = AK8_JetColl[emme].SDMass();
    if ( fabs(dijetmass_tmp-M_W) < fabs(dijetmass-M_W) ) {
      dijetmass = dijetmass_tmp;
      m = emme;
    }
  }

  //// Fill Plots before All SR cuts for better stats 
  if(ApplyForSR) Fill_RegionPlots(param,"Pass"+RegionTag ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);
  
  Particle Wcand = AK8_JetColl[m] + *leps[0]+*leps[1];
  if(Wcand.M()  < LowerMassSR1WmassCut ) return "false";

  FillCutflow(Reg, w, RegionTag+"_Wmass",param);

  Particle N1cand = AK8_JetColl[m] + *leps[0];
  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();

  TString R1Bin=RegionTag+"_MNbin1";

  int nSRbins=6;
  double ml1jbins[nSRbins] = { 0., 500, 750., 1000.,1500, 2000.};

  FillHist(  "LimitExtraction/"+ param.Name+"/LimitShape_"+RegionTag+"/N1Mass_Central",  MN1,  w, nSRbins-1, ml1jbins, "Reco M_{l1jj}");

  if(MN1 > 225){
    if(leps[0]->Pt() < pt_cut)  return RegionTag+"_MNbin1";
  }
  else   if(MN1 > 150){
    if(leps[0]->Pt() < pt_cut-50)  return RegionTag+"_MNbin1";
  }
  
  for(int ibin=1; ibin < nSRbins; ibin++){
    if(MN1 < ml1jbins[ibin]) return RegionTag+"_MNbin"+to_string(ibin+1);
  }
  
  return "true";

}





bool  HNL_RegionDefinitions::RunSignalRegionWW(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl,  std::vector<Jet> JetLooseColl,  std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param,   float w){

  TString SR2String = RunSignalRegionWWString(ApplyForSR,channel, qq, leps, leps_veto, TauColl,JetLooseColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, w);
  if(SR2String == "false") return false;
  else return true;
  
}



TString HNL_RegionDefinitions::RunSignalRegionWWString(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl,  std::vector<Jet> JetLooseColl,  std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param,  float w){


  TString RegionTag                = ApplyForSR ? "SR2" : "CR2";
  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR2 : HNL_LeptonCore::CR2;

  double met_cut     = ApplyForSR ? 10. : 12;

  double met2_st     = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus  && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillCutflow(Reg, w, RegionTag+"_lep_charge",param);

  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";

  if(leps[1]->Pt() < 15.) return "false";
  FillCutflow(Reg, w, RegionTag+"_lep_pt",param);

  bool use_leadjets=true;
  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi < 2.) return "false";
  FillCutflow(Reg, w, RegionTag+"_DPhi",param);
  
  if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return "false";
  FillCutflow(Reg, w, RegionTag+"_LLMass",param);

  if(JetColl.size() < 2) return "false";
  FillCutflow(Reg, w, RegionTag+"_DiJet",param);

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
  FillCutflow(Reg, w, RegionTag+"_DiJetEta",param);

  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  FillCutflow(Reg, w, RegionTag+"_DiJetMass",param);
  if(JJ.M() < 750) return "false";
  
  double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  
  if(zeppenfeld > 0.75) return "false";
  
  if(!PassVBF(JetColl,leps,750., true)) return "false";
  
  if(ApplyForSR) Fill_RegionPlots(param,"Inclusive"+RegionTag ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);
  
  FillCutflow(Reg, w, RegionTag+"_VBF",param);
  
  double HT(0.);
  for(auto ij : JetLooseColl) HT += ij.Pt();
  
  double htltcut = 1.;
  
  if(PassHMMet     && ApplyForSR) FillCutflow(Reg, w, RegionTag+"_met",param);

  if(PassRegionReq){

    if(!ApplyForSR &&PassHMMet) FillCutflow(Reg, w, RegionTag+"_met",param);
    FillCutflow(Reg, w, RegionTag+"_bveto",param);

    double HTBin[6] = { 0.,1, 2., 3., 4,5};
    double HTPT = (HT/leps[0]->Pt() > 5)  ? 5. : HT/leps[0]->Pt() ;

    FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag + "/HT_PT_Central",  HTPT,  w, 5, HTBin, "Reco H_{T}/P_{T}^{lep1}");

    double PTBin[5] = { 0.,50, 100., 120., 200};
    double L2Pt = (leps[1]->Pt() > 200) ? 199. : leps[1]->Pt();

    FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag + "/PT2_LT1_Central",  L2Pt,  w, 4, PTBin, "Reco P_{T}^{Lep2}");

    if (HT/leps[0]->Pt() < htltcut){
      
      if(ApplyForSR)Fill_RegionPlots(param,"Pass"+RegionTag ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w);      
      
      FillCutflow(Reg, w, RegionTag+"_ht_lt1",param);
      
      if (L2Pt > 120)      return RegionTag+"_HTLTbin1";
      return RegionTag+"_HTLTbin2";
    }
  }
  return "false";
}



bool  HNL_RegionDefinitions::RunSignalRegionAK4(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param,   float w){

  TString SR3String = RunSignalRegionAK4String(ApplyForSR,channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param,w);
  if(SR3String == "false") return false;
  else return true;

}




TString HNL_RegionDefinitions::RunSignalRegionAK4StringBDT(bool ApplyForSR, TString mN, TString NCut, TString NTree, HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> LepTColl, std::vector<Jet> JetColl, std::vector<Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param,  float w){
  
  //// Only fill Cutflow/plots for 1 mass point
  bool FillCutFlow = (mN=="100") ;

  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR3BDT : HNL_LeptonCore::CR3BDT;

  TString BDTLabel = "M"+mN+"_NTrees"+NTree+"_NCuts"+NCut+"_MaxDepth3";
  
  double met_cut     = ApplyForSR ? 15. : 17; /// Apply buffer between SR and CR MET cuts
  vector<FatJet> AK8_JetColl;
  double met2_st     = GetMET2ST(LepTColl, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;

  TString RegionTag                = ApplyForSR ? "SR3" : "CR3";

  if(qq==Plus && LepTColl[0]->Charge() < 0) return "false";
  if(qq==Minus && LepTColl[0]->Charge() > 0) return "false";
  if(FillCutFlow)FillCutflow(Reg, w, RegionTag+"_lep_charge",param);

  if(!CheckLeptonFlavourForChannel(channel, LepTColl)) return "false";
  if(FillCutFlow)FillCutflow(Reg, w, RegionTag+"_lep_pt",param);

  if (LepTColl.size() != 2) return "false";

  Particle ll =  (*LepTColl[0]) + (*LepTColl[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return "false";

  if(FillCutFlow)  FillCutflow(Reg, w, RegionTag+"_dilep_mass",param);

  //float MVAvalue = EvaluateEventMVA(mN, NCut,NTree, channel,  LepTColl,ev, METv,param);
  //cout << "Summary of BDT " << mN << endl;
  //cout << "Predetermined value = " << MVAvalue << " " <<  ev.HNL_MVA_Event(GetChannelString(channel)+"_"+mN) << endl;

  float MVAvalueIncl    = EvaluateEventMVA(mN, "Incl", NCut, NTree, channel, LepTColl, ev, METv, param);
  float MVAvalueFake    = EvaluateEventMVA(mN, "Fake", NCut, NTree, channel, LepTColl, ev, METv, param);
  float MVAvalueNonFake = EvaluateEventMVA(mN, "NonFake", NCut, NTree, channel, LepTColl, ev, METv, param);

  if(FillCutFlow && ApplyForSR && PassHMMet ) FillCutflow(Reg, w, RegionTag+"_MET",param);

  if(!PassRegionReq)  return "false";
  if(FillCutFlow && !ApplyForSR )  FillCutflow(Reg, w, RegionTag+"_MET",param);
  if(FillCutFlow) FillCutflow(Reg, w, RegionTag+"_bveto",param);
  
  FillHist("LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"BDT/MVA1D_Incl_AllJets_"+BDTLabel, MVAvalueIncl, w, 80, -1., 1.);
  if(JetColl.size() < 2){
    FillHist("LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"BDT/MVA1D_Incl_01Jet_"+BDTLabel, MVAvalueIncl, w, 80, -1., 1.);
  }
  else{
    FillHist("LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"BDT/MVA1D_Incl_2Jets_"+BDTLabel, MVAvalueIncl, w, 80, -1., 1.);
  }
  FillHist("LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"BDT/MVA2D_"+BDTLabel, MVAvalueFake, MVAvalueNonFake, w, 80, -1., 1., 80, -1., 1.);

  vector<Tau> TauColl;
  if(FillCutFlow&&ApplyForSR) Fill_RegionPlots(param,"Pass"+RegionTag+"BDT" ,TauColl,  JetColl, AK8_JetColl, LepTColl,  METv, nPV, w);

  if(FillCutFlow){
    if(ApplyForSR)FillCutflow(HNL_LeptonCore::ChannelDepSR3, w, GetChannelString(channel) +"_"+RegionTag,param);
    else FillCutflow(HNL_LeptonCore::ChannelDepCR3, w, GetChannelString(channel) +"_"+RegionTag,param);
  }

  ///////// BDT BINS NEED OPTIMISING

  vector<pair<TString, double > > BDTLimitBinsMu;
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin1" , -0.2));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin2" , 0.));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin3" , 0.025));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin4" , 0.05));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin5" , 0.075));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin6" , 0.1));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin7" , 0.125));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin8" , 0.15));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin9" , 0.175));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin10" , 0.2));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin11" , 0.225));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin12" , 0.25));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin13" , 0.275));
  BDTLimitBinsMu.push_back( make_pair(RegionTag+"_BDTbin14" , 1.));


  vector<pair<TString, double > > BDTLimitBinsEl;
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin1" , -0.2));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin2" , 0.));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin3" , 0.025));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin4" , 0.05));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin5" , 0.075));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin6" , 0.1));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin7" , 0.125));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin8" , 0.15));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin9" , 0.175));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin10" , 0.2));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin11" , 0.225));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin12" , 0.25));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin13" , 0.275));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin13" , 0.3));
  BDTLimitBinsEl.push_back( make_pair(RegionTag+"_BDTbin14" , 1.));

  /// Need to Add EMu Binning as current EMu uses EE
  
  double  bdtbin = 0.5;
  if(channel == MuMu){ 
    for(auto imap : BDTLimitBinsMu){
      if(MVAvalueIncl< imap.second) {
        FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"BDT/MVA_"+BDTLabel+"_Binned",  bdtbin,  w,  BDTLimitBinsMu.size(), 0, double(BDTLimitBinsMu.size()),  RegionTag);
        return imap.first;
      }
      bdtbin=bdtbin+1.;
    }  
  }
  else{
    
    for(auto imap : BDTLimitBinsEl){
      if(MVAvalueIncl< imap.second) {
        FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"BDT/MVA_"+BDTLabel+"_Binned",  bdtbin,  w, BDTLimitBinsEl.size(), 0, double(BDTLimitBinsEl.size()),  RegionTag);
        return imap.first;
      }
      bdtbin=bdtbin+1.;
    }

  }

  return "true";
}


TString HNL_RegionDefinitions::RunSignalRegionAK4String(bool ApplyForSR,HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param,  float w){

  HNL_LeptonCore::SearchRegion Reg = ApplyForSR ? HNL_LeptonCore::SR3 : HNL_LeptonCore::CR3;

  double met_cut     = ApplyForSR ? 15. : 17;

  double met2_st     = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet     = ApplyForSR ? (met2_st < met_cut) :  (met2_st > met_cut);
  bool PassBJetMVeto = ApplyForSR ? (B_JetColl.size()==0) : (B_JetColl.size()> 0);
  bool PassRegionReq = ApplyForSR ? (PassHMMet && PassBJetMVeto) :  (PassHMMet ||  PassBJetMVeto) ;

  TString RegionTag  = ApplyForSR ? "SR3" : "CR3";
      

  if(qq==Plus  && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";
   FillCutflow(Reg, w, RegionTag+"_lep_charge",param);

  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";

  FillCutflow(Reg, w, RegionTag+"_lep_pt",param);
  if(ApplyForSR) Fill_RegionPlots(param,"InclusiveSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);

  if (leps_veto.size() != 2) return "false";

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) {
    if(ApplyForSR)Fill_RegionPlots(param,RegionTag+"ZPeak" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
    return "false";
  }

  FillCutflow(Reg, w, RegionTag+"_dilep_mass",param);

  if(ApplyForSR&&PassRegionReq){
    if(JetColl.size()==0)  Fill_RegionPlots(param,"ZeroJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
    if(JetColl.size()==1)  Fill_RegionPlots(param,"OneJetSR3" ,  TauColl,JetColl, AK8_JetColl, leps,  METv, nPV, w);
  }
  
  if(PassHMMet&& ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_MET",param);

  if(!PassRegionReq) return "false";
  if(ApplyForSR)     Fill_RegionPlots(param,"DiJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
  if(!ApplyForSR)  FillCutflow(Reg, w, RegionTag+"_MET",param);
  FillCutflow(Reg, w, RegionTag+"_bveto",param);
  
  
  double PTBin[6] = { 0.,50, 100., 120.,200, 400};
  double L2Pt = (leps[1]->Pt() > 400) ? 399. : leps[1]->Pt();

  FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag + "/Pt_Lepton1",  L2Pt,  w, 5, PTBin, "Reco P_{T}^{Lep2}");
  
  double Binvalue=0;
  if((JetColl.size()<2) && (leps[1]->Pt()  < 50)) Binvalue=0.5;
  else   if((JetColl.size()<2) && (leps[1]->Pt()  < 150)) Binvalue=1.5;
  else   if((JetColl.size()<2)) Binvalue=2.5;
  else   {
    Particle llJJ =  *leps[0] + *leps[1]+JetColl[0]+JetColl[1];
    /// For first one these are arbitary                                                                                                                                                                                                                                                                                                                   
    if(llJJ.M() < 200) Binvalue= 3.5; 
    else     if(llJJ.M() < 400) Binvalue= 4.5;
    else Binvalue= 5.5;
  }
  FillHist(  "LimitExtraction/"+ param.Name+"/LimitShape_"+RegionTag+"/Binnned",  Binvalue  ,  w, 6,0,6 ,"CR Binned");

  if(JetColl.size() == 0){
    if(leps[1]->Pt() > 140.) {
      FillCutflow(Reg, w, RegionTag+"_0JetBinHighPT",param);
      FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  0.5,  w, 11, 0,11,  RegionTag+" Bins");
      return RegionTag+"_bin1";
    }
    else   FillCutflow(Reg, w, RegionTag+"_0JetBinLowPT",param);
  }
  if(JetColl.size() == 1){
    if(leps[1]->Pt() > 140.) {
      FillCutflow(Reg, w, RegionTag+"_1JetBinHighPT",param);
      FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  1.5,  w, 11, 0,11,  RegionTag+" Bins");
      return RegionTag+"_bin2";
    }
    else  FillCutflow(Reg, w, RegionTag+"_1JetBinLowPT",param);

  }

  if(ApplyForSR)Fill_RegionPlots(param,"PassSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w);
  
  if(JetColl.size() < 2)   return "false";
  
  FillCutflow(Reg, w, RegionTag+"_dijet",param);
                                                           
  if(JetColl[0].Pt() < 25.)  return "false";
  FillCutflow(Reg, w, RegionTag+"_J1Pt",param);
    
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=1; enne<JetColl.size(); enne++) {

      if(emme == enne) continue;      
      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();

      
      if ( fabs(dijetmass_tmp-M_W) < fabs(dijetmass-M_W) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
        n = enne;
      }
    }
  }

  double pt_bin2= 140.;
  Particle Wcand = JetColl[m] + JetColl[n]+ *leps[0]+ *leps[1];
  Particle N1cand = JetColl[m] + JetColl[n]+ *leps[0];
  Particle N2cand = JetColl[m] + JetColl[n]+ *leps[1];
  //Particle NCand = (leps[1]->Pt() < leps[0]->Pt()) ? N2cand : N1cand;
  // double MN  = (NCand.M() > 2000.) ? 1999. : NCand.M();

  TString LimitBin = "";
 
  if(Wcand.M() < 400) LimitBin=RegionTag+"_bin3";
  FillCutflow(Reg, w, RegionTag+"_W1Mass",param);
  if(leps[1]->Pt() < 20) LimitBin=RegionTag+"_bin3";

  if(leps[0]->Pt() < pt_bin2) LimitBin=RegionTag+"_bin3";
  FillCutflow(Reg, w, RegionTag+"_LepPt",param);

  if(met2_st < 2){
    if(N1cand.M() < 500.)   LimitBin=RegionTag+"_bin4";
    if(N1cand.M() < 750.)   LimitBin=RegionTag+"_bin5";
    LimitBin=RegionTag+"_bin6";
  }
  else   if(met2_st < 7){
    if(N1cand.M() < 500.)   LimitBin=RegionTag+"_bin7";
    if(N1cand.M() < 750.)   LimitBin=RegionTag+"_bin8";
    LimitBin=RegionTag+"_bin9";
  }
  else{
    if(N1cand.M() < 750.)   LimitBin=RegionTag+"_bin10";
  }
  LimitBin=RegionTag+"_bin11";

  if(LimitBin.Contains("_bin3")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  2.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin4")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  3.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin5")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  4.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin6")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  5.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin7")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  6.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin8")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  7.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin9")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins",  8.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin10")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins", 9.5,  w, 11, 0,11,  RegionTag+" Bins");
  if(LimitBin.Contains("_bin11")) FillHist( "LimitExtraction/"+param.Name+"/LimitShape_"+RegionTag+"/RegionBins", 10.5,  w, 11, 0,11,  RegionTag+" Bins");
  
  return LimitBin;
}


HNL_RegionDefinitions::HNL_RegionDefinitions(){
      
}
 
HNL_RegionDefinitions::~HNL_RegionDefinitions(){

  
}


void HNL_RegionDefinitions::RunSR3BDT(HNL_LeptonCore::ChargeType qq, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,  std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){

  /// Function plots BDT for all Setup in MNStrList[im], NCutList[ic], NTreeList[]                                                                                                                                
  // The string returned can be ignored, since this is for use in Limit plot, but function make plot in LimitInputSR3BDT/param.Name                                                                               

  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu, EMu};
  std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
  std::vector<Lepton *> leps       = MakeLeptonPointerVector(muons,electrons,param);
  for(auto dilep_channel : channels){

    for(unsigned int im=0; im<MNStrList.size(); im++){
      for(unsigned int ic=0; ic<NCutList.size(); ic++){
        for(unsigned int it=0; it<NTreeList.size(); it++){
          RunSignalRegionAK4StringBDT(true,MNStrList[im], NCutList[ic], NTreeList[it], dilep_channel,qq, leps, JetColl,B_JetColl, ev, METv ,param,weight_ll);

        }
      }
    }
  }


  return;

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



