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
  5- RunSignalRegionTrilepton  (REMNANT)
  

  CR Functions
  -----------------------------------
  * RunAllControlRegions Run ALL following CRs
  ** RunElectronChannelCR
  ** RunMuonChannelCR
  ===============================================================
  1- FillTopCRPlots
  %% 2 leptons (OS/SS) + MET > 50 + Nb > 0 + 
  %% -validate B tagging eff + SF
  ===============================================================  
  2- FillZNPCRPlots
  %% 3Lep +  MET < 30 + MZ  + MTnonZlep < 30
  %% Validate Fakes
  ===============================================================y
  3- FillZCRPlots
  %% 2 Lep + MET < 30 + Nb(0) + NAK8(>0) + MZ 
  %% Check in SR1 like region with low met and 0 b jet
  ===============================================================
  4- FillWWCR1Plots
  %% SSlep +  VBFJ(2) + MJJ(>500) + JPt(30) + Nb(0) JJEta(>2.5) + Zepp(<0.75) + MZ + llDphi(<2.) 
  %% - region frmo Peking, SR w/Reverse dphi cut
  ===============================================================
  5- FillWWCR2Plots
  %% SSlep +  VBFJ(2) + MJJ(>500) + JPt(30) + Nb(0) JJEta(>2.5) + Zepp(<0.75) + MZ + MET2ST(>15)
  %% - SR2 BUT with HIgh met
  ===============================================================
  6- FillWWCRNPPlots
  %% SSlep +  VBFJ(2) + MET > 30 + Nb(>0) + JJEta(>2.5) + MZ +  MJJ(>500) same as AN2020_045 Table 15 Nonprompt 
  %%  SR2 BUT with HIgh met and bjet
  ===============================================================
  7- FillWWCRNP2Plots
  %%  SSlep +  VBFJ(2) + Nb(>0) + JJEta(>2.5) + MZ +  MJJ(>500) 
  %%  SR2 BUT with  bjet  
  ===============================================================
  8- FillWWCRNP3Plots
  %% SSlep +  VBFJ(2) + Nb(0) + JJEta(>2.5) + MZ +  MJJ(150-500)
  %% Loook in SR2 MJJ sideband
  ===============================================================
  9- FillOSPreselectionPlots
  %% OS2l 
  %% Validate lepton ID
  ===============================================================
  10- FillSSPreselectionPlots
  %% - SS2l + mll(>20) + Nj(>1) +MZ
  ===============================================================
  11- FillSSVBFPreselectionPlots
  %% - SS2l + mll(>20) + Nvbfj(>1) +MZ 
  ===============================================================
  12- FillHighMassSR1CRPlots
  %% FillSSPreselectionPlots + MET2ST > 15||Nb(>0)
  ===============================================================
  13- FillHighMass1JetCRPlots
  %% SS2l + Nj(1) + MZpeak 
  ===============================================================
  14- FillHighMassBJetCRPlots
  %% SS2l + NBj(1)+ Mll(>10)
  ===============================================================
  15- FillHighMassNPCRPlots
  %% SS2l + Nj(0)+ Mll(>10) + llDphi(> 2.5)                                                                                                                                                                                                                                    ===============================================================
  16- FillHighMassSR3BDTCRPlots
  %% 
  ==============================================================
  17- FillHighMassSR3CRPlots
  %% CR3
  ===============================================================
  18- FillHighMassSR2CRPlots
  %% CR2
  ===============================================================
  19- FillWZ2CRPlots
  %% 
  %% 
  ===============================================================
  20- FillWZBCRPlots
 %% 
 %% 
  ===============================================================
  21- FillZZCRPlots
 %% 
 %% 
  ===============================================================
  22- FillZZ2CRPlots
 %% 
 %% 
  ===============================================================
  23- FillZGCRPlots
 %% 
 %% 
  ===============================================================
  24- FillWGCRPlots
 %% 
 %% 
  ===============================================================
  25- FillWZCRPlots
 %% 
 %% 
  ===============================================================

  BDT Functions
  -----------------------------------
  1- RunSR3BDT  (used to make plots of mass dep BDT scores for all weight files input)


 */



void HNL_RegionDefinitions::RunSR3BDT(HNL_LeptonCore::ChargeType qq, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,  std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1, Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){
  
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
            std::vector<Jet> JetCollLoose, std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1, 
            Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){

  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu, EMu};

  for(auto dilep_channel : channels){
    
    //if(electrons.size()>1)cout << "HNL_RegionDefinitions::RunAllSignalRegions " << GetChannelString(dilep_channel) <<  endl;
    //if(electrons.size()>1)cout << "muon size : " << muons.size() << endl;
    //if(electrons.size()>1)cout << "electron size : " << electrons.size() << endl;

    float weight_channel = weight_ll;

    //// Select CHannel used for Signals to check if signal is EE/MM/Emu using gen info
    if(MCSample.Contains("Type")&& !SelectChannel(dilep_channel)) continue;
  
    //cout << "ParamName = " << param.Name << endl;
    std::vector<Lepton *> leps       = MakeLeptonPointerVector(muons,electrons,param);
    //if(electrons.size()>1)cout << "leps defined;" << endl;
    //if(electrons.size()>1)cout << "leps size : " << leps.size() << endl;
    FillHist("Nlep_NoCut", leps.size() , weight_channel, 10., 0., 10); //JH

    if(!IsDATA && dilep_channel != MuMu)  weight_channel*= GetElectronSFEventWeight(electrons, param);
    if(!IsDATA && dilep_channel != EE)    weight_channel*= GetMuonSFEventWeight(muons, param);

    FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, weight_channel, GetChannelString(dilep_channel) +"_NoCut", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

    if(!PassGenMatchFilter(leps,param)) continue;
    FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, weight_channel, GetChannelString(dilep_channel) +"_GenFilter", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

    //if(!ConversionSplitting(leps)) continue; // FIXME JH : This doesn't do anything for now

    // Make channel speciific AnalyzerParameter
    AnalyzerParameter param_channel = param;

    //FillEventCutflow(HNL_LeptonCore::ChannelDepInc, weight_channel, GetChannelString(dilep_channel) +"_NoCut", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

    if(!PassMETFilter()) return;
    FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, weight_channel, GetChannelString(dilep_channel) +"_METFilter", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

    if(MCSample.Contains("Type")){
      vector<Gen> SigGens = GetGenLepronsSignal();
      if (SigGens.size()==2){

        if(SigGens[0].PID() > 0 && SigGens[1].PID()  > 0 )     FillEventCutflow(HNL_LeptonCore::ChannelDepIncQ, weight_channel, GetChannelString(dilep_channel) +"_MMQ_NoCut", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
        else if(SigGens[0].PID()  < 0 && SigGens[1].PID()  < 0 )     FillEventCutflow(HNL_LeptonCore::ChannelDepIncQ, weight_channel, GetChannelString(dilep_channel) +"_PPQ_NoCut", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
  
      }
    }
    
    
    TString channel_string = GetChannelString(dilep_channel);
    param_channel.Name =  channel_string + "_Channel/" + param_channel.DefName; 
    
    std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
    
    //if(leps.size() ==2)  FillEventCutflow(HNL_LeptonCore::ChannelDepDilep, weight_channel, GetChannelString(dilep_channel) +"_Dilep", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH
    if(leps.size() ==2) FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, weight_channel, GetChannelString(dilep_channel) +"_DiLep", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH
    

    if(RunCF){
      if(dilep_channel == MuMu) continue;

      if(IsData && SameCharge(leps)) continue;
      if(!IsData && !SameCharge(leps)) continue;
      
      if(IsData)weight_channel = GetCFWeightElectron(leps, param_channel);
      //if(IsData)weight_channel = GetCFWeightElectron(leps, param_channel, true, 0); // data-driven charge flip
      if(IsData)FillWeightHist(param.Name+"/CFWeight",weight_channel);

    }
    else{
      if(!SameCharge(leps)) continue;
    }
    FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, weight_channel, GetChannelString(dilep_channel) +"_SameCharge", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

    //if(RunFake&& IsData){
    if(RunFake){ //FIXME need this even in MC to perform the closure test
      //weight_channel = GetFakeWeight(leps, param_channel, true);
      weight_channel *= GetFakeWeight(leps, param_channel, false); //FIXME tentatively pr = 1.
      FillWeightHist(param_channel.Name+"/FakeWeight",weight_channel);
    }
    
    // Run PRESEL + SR1-3                                                                                                                                                           
    //bool run_ORTrigger = false;

    
    HNL_LeptonCore::SearchRegion LimitRegions = HNL_LeptonCore::MuonSR;
    if (dilep_channel == EE) LimitRegions =HNL_LeptonCore::ElectronSR;
    if (dilep_channel == EMu) LimitRegions =HNL_LeptonCore::ElectronMuonSR;
    HNL_LeptonCore::SearchRegion LimitRegionsQ = HNL_LeptonCore::MuonSRQQ;
    if (dilep_channel == EE) LimitRegionsQ =HNL_LeptonCore::ElectronSRQQ;
    if (dilep_channel == EMu) LimitRegionsQ =HNL_LeptonCore::ElectronMuonSRQQ;

    HNL_LeptonCore::SearchRegion LimitRegionsBDT = HNL_LeptonCore::MuonSRBDT;
    if (dilep_channel == EE) LimitRegionsBDT =HNL_LeptonCore::ElectronSRBDT;
    if (dilep_channel == EMu) LimitRegionsBDT =HNL_LeptonCore::ElectronMuonSRBDT;

    /*HNL_LeptonCore::SearchRegion LimitRegionsBDTQ = HNL_LeptonCore::MuonSRBDTQQ;
    if (dilep_channel == EE) LimitRegionsBDTQ =HNL_LeptonCore::ElectronSRBDTQQ;
    if (dilep_channel == EMu) LimitRegionsBDTQ =HNL_LeptonCore::ElectronMuonSRBDTQQ;
    */
    

    if(!PassPreselection(dilep_channel,qq, leps, leps_veto, TauColl, JetColl, VBF_JetColl, AK8_JetColl, B_JetColl,ev, METv ,param_channel,"", weight_channel)) continue;
    if(RunFakeClosure) return; //JH : don't need further steps
    //cout << "passed presel;" << endl;
    //cout << "MET : " << METv.Pt() << endl;
    //cout << "AK8 jet size : " << AK8_JetColl.size() << endl; //JH
    TString  lep_charge =  (leps[0]->Charge() < 0)  ? "QM" :  "QP";
    
    if(AK8_JetColl.size() > 0) {

      TString SRbin= RunSignalRegionAK8String (dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl,B_JetCollSR1,ev, METv ,param_channel,"", weight_channel) ;
      //if(SRbin != "false") cout << "passed SR1 : " << SRbin << endl;
      //if(SRbin != "false") cout << "MET : " << METv.Pt() << endl; //JH

      if(SRbin != "false") FillEventCutflow(LimitRegions, weight_channel, SRbin,"LimitInput/"+param.Name);
      if(SRbin != "false") FillEventCutflow(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param.Name);
      
      for(unsigned int im=0; im<MNStrList.size(); im++){
        if(SRbin != "false") FillEventCutflow(LimitRegionsBDT, weight_channel, SRbin,"LimitInputBDT/"+param.Name+"/M"+MNStrList[im]);
      }
    }
    else{

      TString SRbin = RunSignalRegionWWString( dilep_channel,qq, leps, leps_veto,  TauColl, JetCollLoose, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param_channel,  "", weight_channel);       

      if(SRbin != "false"){
        FillEventCutflow(LimitRegions, weight_channel, SRbin,"LimitInput/"+param.Name);
        FillEventCutflow(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param.Name);
        for(unsigned int im=0; im<MNStrList.size(); im++){
          if(SRbin != "false") FillEventCutflow(LimitRegionsBDT, weight_channel, SRbin,"LimitInputBDT/"+param.Name+"/M"+MNStrList[im]);
        }
      }
      else{
  for(auto imapHP :FinalBDTHyperParamMap){
    TString SRBDT = RunSignalRegionAK4StringBDT(true,imapHP.first , imapHP.second.first, imapHP.second.second, dilep_channel,qq, leps, JetColl,  B_JetColl, ev, METv ,param_channel,"", weight_channel);
    if(SRBDT != "false") FillEventCutflow(LimitRegionsBDT, weight_channel, SRBDT,"LimitInputBDT/"+param.Name+"/M"+imapHP.first);
  }

  SRbin  = RunSignalRegionAK4String (dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);

  if(SRbin != "false"){
    FillEventCutflow(LimitRegions, weight_channel, SRbin,"LimitInput/"+param.Name);
    FillEventCutflow(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param.Name);

  }
  else{
    if(JetColl.size() == 0)Fill_RegionPlots(dilep_channel, 0, param_channel.Name,"FailZeroJet" ,  TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, weight_channel,param.WriteOutVerbose);
    if(JetColl.size() == 1)Fill_RegionPlots(dilep_channel, 0, param_channel.Name,"Fail1Jet" ,  TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, weight_channel,param.WriteOutVerbose);
    if(JetColl.size() > 1)Fill_RegionPlots(dilep_channel, 0, param_channel.Name,"FailDiJet" ,  JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, weight_channel,param.WriteOutVerbose);
    
    if(JetColl.size() == 0) FillEventCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_0j", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
    else if(JetColl.size() == 1)FillEventCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_1j", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
    else FillEventCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_2j", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
    
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
    
    THIS IS APPLIED IN PassTriggerSelection
    
   */
  
  if(run_Debug) cout << "HNL_RegionDefinitions::PassPreselection " << GetChannelString(channel) <<  endl;

  // Make sure events contain 2 leps                                                                                                       
  // Make sure correct leptons are used                                                                                                                                      
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_FlavorFilter", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

  if (! (  PassMultiTriggerSelection(channel, ev, leps,"Dilep", "HighPt") || PassMultiTriggerSelection(channel, ev, leps,"Dilep", "Lep"))) return false;
  
  //FillEventCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_MultiTrigger", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_MultiTrigger", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH
  
  if (!PassTriggerSelection(channel, ev, leps,"Dilep")) return false;
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_Trigger", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH
  
  //w *= HNLZvtxSF(channel); //JH
  //FillEventCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_Trigger", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH


  if(run_Debug) cout << "HNL_RegionDefinitions::PassTriggerSelection " << GetChannelString(channel) <<  endl;

  
  if(!PassHEMVeto(channel, leps)) return false;
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_HEM", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH


  // Make sure events contain 2 leps
  if (leps_veto.size() != 2) return false;
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_lepveto", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

  
  // CUT ON MASS OF LL PAIR

  Particle ll =  (*leps[0]) + (*leps[1]);
  
  // VETO Z PEAK IN EE CHANNEL
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;

  if(ll.M() < 10) return false; // TO_CHECK: IS 20 BEST OPTION
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_Mll10", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose); //JH

  // REMOVE 0 Jet EVENTS
  //int njets     = JetColl.size() + AK8_JetColl.size() + VBF_JetColl.size();
 
  if(RunFake){ leps = LeptonUsePtParton(leps); } //JH

  Fill_RegionPlots(channel, 0, param.Name,"Preselection" , TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose);
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_Presel", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
 
  //cout << "end presel;" << endl; //JH
  //cout << "AK8 jet size : " << AK8_JetColl.size() << endl; //JH

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
    for (auto ilep : leps) if (ilep->DeltaR(itau) < 0.4) HasTightTau_n=false;
    for (auto ijet : AK8_JetColl) if (ijet.DeltaR(itau) < 1.)  HasTightTau_n=false;
    for (auto ijet : JetColl) if (ijet.DeltaR(itau) < 0.5)  HasTightTau_n=false;
    if(HasTightTau_n) HasTightTau=true;
  }
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_Init",param.Name,param.WriteOutVerbose);
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";  
  if (leps_veto.size() != 2) return "false";

  
  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_lep_charge",param.Name,param.WriteOutVerbose);

  FillEventCutflow(HNL_LeptonCore::SR1,w, "SR1_lep_pt",param.Name,param.WriteOutVerbose);
  if(AK8_JetColl.size() == 0) return "false";

  
  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return "false";
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_dilep_mass",param.Name,param.WriteOutVerbose);

  double ST = GetST(leps, JetColl, AK8_JetColl, METv);
  double HT = GetHT(JetColl, AK8_JetColl);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 15);
  bool PassBJetMVeto = (B_JetColl.size()==0);
  double LowerMassSR1WmassCut = 500.;
  
  TString signal_region1 = "HNL_SR1";
  
  //double FatJetTau21_SF = GetEventFatJetSF(AK8_JetColl,"ak8_type1", 0);
  //FillWeightHist(param.Name+"/fatjet_ak8_type1_sf_"+param.Name,FatJetTau21_SF);
  //if(!IsData)w*=  FatJetTau21_SF;
  Fill_RegionPlots(channel, 0, param.Name,"InclusiveSR1" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);

  if(PassHMMet)FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_MET",param.Name,param.WriteOutVerbose);

  if(GetMass(signal_region1,JetColl, AK8_JetColl,  leps) < LowerMassSR1WmassCut ) return "false";
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_Wmass",param.Name,param.WriteOutVerbose); 
  
  if(!(PassHMMet&&PassBJetMVeto)) return "false";
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_bveto",param.Name,param.WriteOutVerbose);    

  FillEventCutflow(HNL_LeptonCore::ChannelDepSR1, w, GetChannelString(channel) +"_SR1", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
  
  if(leps[0]->Pt() < 100) return "SR1_MNbin1";
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_LepPt",param.Name,param.WriteOutVerbose);

 
  double dPhil2_l = fabs(TVector2::Phi_mpi_pi( ( (AK8_JetColl[0] + *leps[1]).Phi() - (leps[0]->Phi() ))));
  double dRl2_l2_cut = 2.;
  if(dPhil2_l < dRl2_l2_cut) return "SR1_MNbin1";
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_DphiN_Wlep",param.Name,param.WriteOutVerbose);
  
  if(HasTightTau) return "false";
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_TauVeto",param.Name,param.WriteOutVerbose);

  if( ( HT/leps[0]->Pt()) > 4.) return "SR1_MNbin1";

  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_HTPt",param.Name,param.WriteOutVerbose);


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
  
  
  Particle N1cand = AK8_JetColl[m] + *leps[0];
  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();
      
  int nSRbins=5;
  double ml1jbins[nSRbins] = { 0., 500., 750., 1000., 2000.};
  
      
  FillHist( param.Name+"/LimitShape_SR1/PreCuts_N1Mass_Central",  MN1,  w, nSRbins-1, ml1jbins, "Reco M_{l1jj}");
  
  TString SR1Bin2="SR1_MNbin2";
  
  if(channel==MuMu){
    if(MN1 > 225){
      if(leps[0]->Pt() < 140)   return SR1Bin2;
    }
    else   if(MN1 > 150){
      if(leps[0]->Pt() < 70)   return SR1Bin2;
    }
  }
  if(channel==EE || channel==EMu){
    if(MN1 > 635){
      if(leps[0]->Pt() < 140)   return SR1Bin2;
    }
    else   if(MN1 > 550){
      if(leps[0]->Pt() < 120)   return SR1Bin2;
    }
    else if(MN1 > 440){
      if(leps[0]->Pt() < 120)   return SR1Bin2;
    }
    else   if(MN1 > 270){
      if(leps[0]->Pt() < 100)   return SR1Bin2;
    }
  }
      
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_masscuts",param.Name,param.WriteOutVerbose);
      
  FillHist( param.Name+"/LimitShape_SR1/N1Mass_Central",  MN1,  w, 4, ml1jbins, "Reco M_{l1jj}");
  
  Fill_RegionPlots(channel, 0, param.Name,"FinalSR1" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
  
  for(int ibin=1; ibin < nSRbins; ibin++){
    if(MN1 < ml1jbins[ibin]) return "SR1_MNbin"+to_string(ibin+2);
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

  FillEventCutflow(HNL_LeptonCore::SR2,w, "SR2_lep_charge",param.Name,param.WriteOutVerbose);
  
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_lep_pt",param.Name,param.WriteOutVerbose); //JH : FIXME redundant;

  bool use_leadjets=true;
  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi < 2.) return "false";
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_DPhi",param.Name,param.WriteOutVerbose);
  
  if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return "false"; //JH : FIXME why 20? 10 was already applied in Presel;
  if(JetColl.size() < 2) return "false";
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

  if(maxDiJetDeta < 2.5) return "false";
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetEta",param.Name,param.WriteOutVerbose);

  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  if(JJ.M() < 750) return "false";
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetMass",param.Name,param.WriteOutVerbose);

  double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta; //JH : FIXME is it correct to use maxDiJetDeta?

  if(zeppenfeld > 0.75) return "false";

  //if(!PassVBF(JetColl,leps,750., true)) return "false"; //JH : FIXME redundant;

  Fill_RegionPlots(channel, 0, param.Name,"InclusiveSR2" ,  TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose);

  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_VBF",param.Name,param.WriteOutVerbose);

  bool PassBJetMVeto = (B_JetColl.size()==0);

  double HT(0.);
  for(auto ij : JetCollLoose) HT += ij.Pt();
  

  //Fill_RegionPlots(channel, 0, param.Name,"PassSR2" ,  TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose); //JH : FIXME redundant;


  double htltcut = 2.;

  double ST = GetST( leps, JetColl, AK8_JetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 12);  ////// An option to add since we have MET in SR1/3
  
  if(PassHMMet) {
    FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_met",param.Name,param.WriteOutVerbose);

    if (HT/leps[0]->Pt() < htltcut){
      
      FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_ht_lt1",param.Name,param.WriteOutVerbose);
      
      if(PassBJetMVeto){
  
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_bveto",param.Name,param.WriteOutVerbose);
  
  double HTLT[3] = { 0.,1., 2};
  double QHTLT[5] = { -2.,-1.,0.,1., 2};
  FillHist( param.Name+"/LimitShape_SR2/HT_LT1_Central",  HT/leps[0]->Pt(),  w, 2, HTLT, "Reco HT/LT1");
  FillHist( param.Name+"/LimitShape_SR2/Q_HT_LT1_Central",  (leps[0]->Charge()*HT)/leps[0]->Pt(),  w, 4 , QHTLT, "Reco HT/LT1 * Q");
  
  FillEventCutflow(HNL_LeptonCore::ChannelDepSR2, w, GetChannelString(channel) +"_SR2", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);
  
  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_Final",param.Name,param.WriteOutVerbose);
  
  
  if (HT/leps[0]->Pt() < 1)      return "SR2_HTLTbin1";
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
  if(isSR)FillEventCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_lep_pt",param.Name,param.WriteOutVerbose);

  if (LepTColl.size() != 2) return "false";

  if(qq==Plus && LepTColl[0]->Charge() < 0) return "false";
  if(qq==Minus && LepTColl[0]->Charge() > 0) return "false";

  if(isSR)FillEventCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_lep_charge",param.Name,param.WriteOutVerbose);

  Particle ll =  (*LepTColl[0]) + (*LepTColl[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return "false";

  if(isSR)  FillEventCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_dilep_mass",param.Name,param.WriteOutVerbose);

  float MVAvalue = EvaluateEventMVA(mN, NCut,NTree, channel,  LepTColl,ev, METv,param);
  //cout << "Summary of BDT " << mN << endl;
  //cout << "Predetermined value = " << MVAvalue << " " <<  ev.HNL_MVA_Event(GetChannelString(channel)+"_"+mN) << endl;
  
  if(isSR)FillHist(param.Name+"/LimitShape_SR3BDT/SignalBins_M"+mN+"_NCut"+NCut+"_NTree"+NTree, MVAvalue, w, 40, -1., 1.);

  std::vector<FatJet> FatJetColl;
  double ST = GetST( LepTColl, JetColl, FatJetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 15);
  
  TString LabelPrefix = (isSR) ? "SR3" : "CR3";
  

  if(!PassHMMet) return "false";
  if(B_JetColl.size() > 0)    return "false";
  if(MVAvalue< 0.0) return LabelPrefix+"_BDTbin1";
  else if(MVAvalue< 0.10) return LabelPrefix+"_BDTbin2";
  else if(MVAvalue< 0.15) return LabelPrefix+"_BDTbin3";
  else if(MVAvalue< 0.2) return LabelPrefix+"_BDTbin4";
  else if(MVAvalue< 0.25) return LabelPrefix+"_BDTbin5";
  else if(MVAvalue< 0.3) return LabelPrefix+"_BDTbin6";
  else if(MVAvalue< 0.35) return LabelPrefix+"_BDTbin7";
  else  return LabelPrefix+"_BDTbin8";

  return "true";
}


TString HNL_RegionDefinitions::RunSignalRegionAK4String(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){

  double bin = 0;
                                        
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_pt",param.Name,param.WriteOutVerbose);

  Fill_RegionPlots(channel, 0, param.Name,"InclusiveSR3" ,TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose);

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_charge",param.Name,param.WriteOutVerbose);

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) {
    Fill_RegionPlots(channel, 0, param.Name,"SR3ZPeak" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
    return "false";
  }

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_dilep_mass",param.Name,param.WriteOutVerbose);

  double ST = GetST(leps, JetColl, AK8_JetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 15);

  double LowerMassSR3WmassCut = 50.;   double UpperMassSR3WmassCut = 120.;
  
  int  NB_JetColl      =  B_JetColl.size();

  if(NB_JetColl==0 && PassHMMet){
    if(JetColl.size()==0)  Fill_RegionPlots(channel, 0, param.Name, "ZeroJetSR3", TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose);
    if(JetColl.size()==1)  Fill_RegionPlots(channel, 0, param.Name, "OneJetSR3", TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose);
  }

  
  if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 0 && leps[1]->Pt() > 140.) {
    FillHist( param.Name+"/LimitShape_SR3/SignalBins",   0.5, w, 16, 0, 16.,  "Signalbins");
    FillHist( param.Name+"/LimitShape_SR3/SignalBinsQ",   0.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    bin=1;
    FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_0JetBinHPT",param.Name,param.WriteOutVerbose);

   return "SR3_bin1";
  }
  if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 1 && leps[1]->Pt() > 140.) {
    FillHist( param.Name+"/LimitShape_SR3/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");
    FillHist( param.Name+"/LimitShape_SR3/SignalBinsQ",   1.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    bin=2;
    FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_1JetBinHPT",param.Name,param.WriteOutVerbose);
    return "SR3_bin2";
  }
  
  if(JetColl.size() < 2)   return "false";

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_dijet",param.Name,param.WriteOutVerbose);

  //if(!(GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) < UpperMassSR3WmassCut && GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) > LowerMassSR3WmassCut)) {
  //  return "false";
 // }
                                                           
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_Wmass",param.Name,param.WriteOutVerbose);

  if(JetColl[0].Pt() < 25.)  return "false";
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_J1Pt",param.Name,param.WriteOutVerbose);
   
  if(!PassHMMet)   return "false";
  
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_MET",param.Name,param.WriteOutVerbose);


  if(NB_JetColl>  0)   return "false";
  
  Fill_RegionPlots(channel, 0, param.Name,"DiJetSR3" ,TauColl, JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose);

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_bveto",param.Name,param.WriteOutVerbose);

  Fill_RegionPlots(channel, 0, param.Name,"DiJetSR3" ,TauColl,  JetColl, AK8_JetColl, B_JetColl, leps, leps_veto, METv, nPV, w,param.WriteOutVerbose);
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
  double LT = leps[0]->Pt() + leps[1]->Pt();
  //double dRl2JJ = leps[1]->DeltaR(JetColl[m] + JetColl[n]);

  TString sbin="";

  if(LT > 150){
    if(N1cand.M() > 350.) {
      if(Wcand.M() > 600)  {
  sbin="3";
  bin=3;
      }
      else  {
  sbin="4";
  bin=4;
      }
    }
    else {
      sbin="5";
      bin=5;
    }
  }
  else {
    if(N1cand.M() > 250.) {
      sbin="6";
      bin=6;
    }  
    else {
      sbin="7";
      bin=7;
    }
  }

  bin = sbin.Atof()-0.5; //JH : to be consistent with bin 1, 2
  FillHist( param.Name+"/LimitShape_SR3/SignalBins",   bin, w, 16, 0, 16., "Signalbins");
  double qbin = bin*double(leps[0]->Charge());
  FillHist( param.Name+"/LimitShape_SR3/SignalBinsQ",   qbin, w, 32, -16, 16, "Signalbins");
  
  TString signal_region = "HNL_SR3";
  
  FillEventCutflow(HNL_LeptonCore::ChannelDepSR3, w, GetChannelString(channel) +"_SR3", "ChannelCutFlow/"+param.DefName,param.WriteOutVerbose);

  return "SR3_bin"+sbin;
  
}



bool HNL_RegionDefinitions::RunSignalRegionTrilepton(HNL_LeptonCore::Channel channel,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Jet> JetColl ,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 3) return false;
  
  FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep",param.Name);
  
  // If running fake bkg now set weight to Fake weight
  //if(RunFake)   w= GetFakeWeightMuon(leps, param);
  if(RunFake&&IsData)   FillWeightHist(param.Name+"/FakeWeight_"+param.Name,w);

  int n_veto_lep  = leps_veto.size();
  
  bool pass_sr1(true);

  if(n_veto_lep > 3) pass_sr1=false;

  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_veto",param.Name);

 
 
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_chargereq",param.Name);

  int  NB_JetColl      = B_JetColl.size();

  if (NB_JetColl > 0)pass_sr1=false;
  
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_bjet",param.Name);
  
  if(ZmasslllWindowCheck(leps)) pass_sr1=false;
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_Zmlll",param.Name);
  if(ZmassOSWindowCheck(leps)) pass_sr1=false;
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_Zmll_os",param.Name);

  
  if(pass_sr1)  {
    // For Cut flow comparison
    //PrintEvent(param,"SR4_"+PostLabel,w);    
    // For event cut flow
    FillEventCutflow(HNL_LeptonCore::SR4, w, "SR4_lll_mu",param.Name);
    // Make histograms generically
    //    Fill_RegionPlots(channel_lll,true,"HNL_SR4_lll" , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
    // make limit inout plots
    //Fill_SigRegionPlots4(channel_lll, param.Name, JetColl,  AK8_JetColl, leps,  METv, nPV, w);

    FillEventCutflow(SR,w, "SR4Pass",param.Name);

  }

  if(!pass_sr1)     FillEventCutflow(SR,w, "SR4Fail",param.Name);

  return pass_sr1;
 
  
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





void HNL_RegionDefinitions::RunAllControlRegions(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,   std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param,  float weight_ll ){
  
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu, EMu};
  vector<HNL_LeptonCore::Channel> channels_lll = {EEE,MuMuMu, EMuL};
  vector<HNL_LeptonCore::Channel> channels_llll = {EEEE,MuMuMuMu, EMuLL};

  if(!PassMETFilter()) return;
 
  //cout << "----------------   RunAllControlRegions [Tight]  ----------------------------------------------------------------------------------------------------------------" << endl;
  std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(muons,electrons,param);
  //cout << "----------------   RunAllControlRegions  [Veto ]----------------------------------------------------------------------------------------------------------------" << endl;
  std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);

  

  ///  Select events based on NConv and if running Run_Conv
  FillHist("CR_BeforeConvCut", 1. , weight_ll, 2., 0.,  2.);

  
  if(!PassGenMatchFilter(LepsT,param)) return;
  
  if(!IsData && RunPromptTLRemoval){
    weight_ll = -1*weight_ll* GetFakeWeight(LepsT, param , false);
  }


  for(unsigned int ic = 0; ic < channels.size(); ic++){

    if(run_Debug) cout << "----------------------------------------" << endl;
    if(run_Debug) cout << "HNL_RegionDefinitions::RunAllControlRegions [" << GetChannelString(channels[ic])<<" ]" << endl;
    

    HNL_LeptonCore::Channel dilep_channel = channels[ic];
    HNL_LeptonCore::Channel trilep_channel = channels_lll[ic];
    HNL_LeptonCore::Channel fourlep_channel = channels_llll[ic];


    // Change Name of param to include Flavour channel
    TString channel_string = GetChannelString(dilep_channel);
    param.Name = channel_string + "/" + param.DefName ;

    //// Make Trilep/4 lep param
    AnalyzerParameter paramTrilep = param;
    AnalyzerParameter paramQuadlep = param;
    paramTrilep.Name  = GetChannelString(trilep_channel) + "/" + param.DefName ;
    paramQuadlep.Name = GetChannelString(fourlep_channel) + "/" + param.DefName ;

    float weight_channel = weight_ll;

    // Weight Lepton with SF corr
    if(!IsDATA && dilep_channel != MuMu)  weight_channel*= GetElectronSFEventWeight(electrons, param);
    if(!IsDATA && dilep_channel != EE)    weight_channel*= GetMuonSFEventWeight(muons, param);
    
    TString label    = param.Name;
    
    if(run_Debug){
      cout << "CheckLeptonFlavourForChannel(dilep_channel, LepsT) = " <<CheckLeptonFlavourForChannel(dilep_channel, LepsT) << endl;
      cout << "CheckLeptonFlavourForChannel(trilep_channel, LepsT) = " <<CheckLeptonFlavourForChannel(trilep_channel, LepsT) << endl;
      cout << "CheckLeptonFlavourForChannel(fourlep_channel, LepsT) = " <<CheckLeptonFlavourForChannel(fourlep_channel, LepsT) << endl;
      
    }
    
    if(! (CheckLeptonFlavourForChannel(dilep_channel, LepsT) || CheckLeptonFlavourForChannel(trilep_channel, LepsT) || CheckLeptonFlavourForChannel(fourlep_channel, LepsT))) continue;
    
    if(SameCharge(LepsT)){
      int LepType1= LepsT[0]->LeptonGenType();
      int LepType2= LepsT[1]->LeptonGenType();
      FillHist("ControlRegionLeptonType/"+param.Name + "/"+GetChannelString(channels[ic]) , LepType1, LepType2,  weight_channel, 14,-7., 7.,14, -7.,7.);
    }
   
    /// This removes overlap in streams
    if (!PassTriggerSelection(dilep_channel, ev, LepsT,"Dilep")) continue;
    if(run_Debug)cout << param.Name << " " << " passes trigger " << endl;

    
    weight_channel *= HNLZvtxSF(dilep_channel);
 

    if(RunFake&&IsData){
      weight_channel = GetFakeWeight(LepsT, param , false);
      FillWeightHist(param.Name+"/FakeWeight",weight_channel);
    }

    vector<TString> passed;
    Fill_RegionPlots(dilep_channel, 0,"Inclusive" , param.Name,  VBF_JetColl,  AK8_JetColl, B_JetColl,  LepsT, LepsV, METv, nPV, weight_channel);
    
    // OS CR
    //if(FillZAK8CRPlots  (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("ZAK8_CR");
    //if(FillTopAK8CRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("TopAK8_CR"); //JH
    if(FillZCRPlots  (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("Z_CR");
    if(FillZCRPlots_Nolepveto  (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("Z_CR");
    if(FillTopCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("Top_CR"); //JH
    if(FillTopCRPlots_Nolepveto(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("Top_CR"); //JH
    if(FillTopCRPlots_2jet(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("Top_CR"); //JH
    
    if(run_Debug) cout << "FillTopCRPlots " << endl;

    if(RunCF){    
      if(LepsT.size() == 2){

  if(dilep_channel == MuMu) continue;
  
  if(IsData && SameCharge(LepsT)) continue;
  if(!IsData && !SameCharge(LepsT)) continue;
  
  if(IsData)weight_channel = GetCFWeightElectron(LepsT, param);
  //if(IsData)weight_channel = GetCFWeightElectron(leps, param_channel, true, 0); // data-driven charge flip
  if(IsData)FillWeightHist(param.Name+"/CFWeight",weight_channel);
      }
      else continue;
    }
    else if(LepsT.size() == 2){
      if(!SameCharge(LepsT)) continue;
    }
    
    
    if(FillSSPreselectionPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("SSPresel");
    if(FillSSVBFPreselectionPlots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("SSVBFPresel");

    //VBF CR                                                                                                                                                                      
    //if(FillWWCR1Plots  (dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CR1");
    //if(FillWWCR2Plots  (dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CR2");
    //if(FillWWCRNPPlots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CRNP");
    //if(FillWWCRNP2Plots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CRNP2");
    //if(FillWWCRNP3Plots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CRNP3");
    //if(FillWZ2CRPlots (trilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("WZ2_CR");
    //if(FillWZBCRPlots (trilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("WZB_CR");
    

    // LLL+                                                                                                                                 
                                                                     
    //if(FillZZCRPlots(  fourlep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramQuadlep, weight_channel)) passed.push_back("ZZ_CR");
    //if(FillZZ2CRPlots( fourlep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramQuadlep, weight_channel)) passed.push_back("ZZLoose_CR");
    //if(FillWZCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("WZ_CR");
    //if(FillZNPCRPlots(trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("ZNP_CR");

    if(run_Debug)cout << "Conv split "  <<  ConversionVeto(LepsT,All_Gens) << " " << ConversionSplitting(LepsT) << endl;
    
    // Treat conversions using GENT MEthod
    //if(ConversionVeto(LepsT,All_Gens)){
    //  if(FillWGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("WG_CR");
    //  if(FillZGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("ZG_CR");
    //}
    // Treat using pt cit method
    //if(ConversionSplitting(LepsT)){
    //  
    //  AnalyzerParameter paramTMP=param;
    //  paramTMP.Name=param.Name+"_ConvMethodPt";
    //  if(FillWGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTMP, weight_channel)) passed.push_back("WG_Method2_CR");
    //  if(FillZGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTMP, weight_channel)) passed.push_back("ZG_Method2_CR");

    //}
    
    
    // 17-028 +CR                                                                                                                                                                                                  
    // SR1 : MET Inv. || BVeto Inv.
    //if(FillHighMassSR1CRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR1_CR");

    // SR2 : MET Inv. || BVeto Inv.
    //if(FillHighMassSR2CRPlots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR2_CR");

    // SR3 : MET Inv. || BVeto Inv.
    //if(FillHighMassSR3CRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR3_CR");
  
    //FillHighMassSR3BDTCRPlots(dilep_channel, LepsT, LepsV, JetAllColl, JetColl,VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel);


    //if(FillHighMass1JetCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMass1Jet_CR");

    //if(FillHighMassBJetCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassBJet_CR");
    
    // 0 Jet : 2 SS lep dphi > 2.5
    //if(FillHighMassNPCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassNP_CR");
    
    

    vector<TString> cutlabels = { "TopAK8_CR","ZAK8_CR","WpWp_CR1","WpWp_CR2","WpWp_CRNP","WpWp_CRNP2","ZZ_CR","ZZLoose_CR","ZG_CR","WG_CR","WZ_CR","WG_Method2_CR","ZG_Method2_CR","WZ2_CR","WZB_CR","HighMassSR1_CR","HighMassSR2_CR","HighMassSR3_CR","HighMass1Jet_CR","HighMassBJet_CR","HighMassNP_CR","ZNP_CR","SSPresel","SSVBFPresel"};
    
    for(auto ip : passed) FillEventCutflow(HNL_LeptonCore::CR, weight_channel, ip, "ChannelCutflow/"+param.Name);
    
    for(unsigned int ipass =0; ipass < passed.size();ipass++){
      //void AnalyzerCore::FillTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){                                                                                 
      for(unsigned int ipass2=ipass+1; ipass2 < passed.size(); ipass2++){
  FillTypeCutflow(channel_string+"CR_Correlation"+param.Name, weight_channel, cutlabels, passed[ipass], passed[ipass2]);
      }
    }
  }
  return;
}




bool HNL_RegionDefinitions::FillTopAK8CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (leps_veto.size() != 2) return false;


  if(METv.Pt() < 50) return false;
  
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl == 0) return false;
  if(AK8_JetColl.size() == 0) return false;

  if(run_Debug){
    cout << "HNL_TopAK8_TwoLepton_CR " << event << endl;
    for(auto ilep: leps) cout << "HNL_TopAK8_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
    //PrintGen(gens);                                                                                                                                                                                              
  }
  if(SameCharge(leps)) Fill_RegionPlots(channel,1,"HNL_SS_TopAK8_TwoLepton_CR" , param.Name, JetColl  ,AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);
  else Fill_RegionPlots(channel,1,"HNL_OS_TopAK8_TwoLepton_CR" , param.Name, JetColl  ,AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
  return true;
}

bool HNL_RegionDefinitions::FillTopCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(SameCharge(leps)) return false;
  if (leps_veto.size() != 2) return false;

  Particle  ll = (*leps[0]) + (*leps[1]);
  if (ll.M() < 55) return false;
  if (fabs(ll.M()-90.) < 15) return false;
  
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl < 2) return false;

  Fill_RegionPlots(channel,1,"HNL_OS_Top_TwoLepton_CR" , param.Name, JetColl  ,AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
  return true;
}

bool HNL_RegionDefinitions::FillTopCRPlots_Nolepveto(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(SameCharge(leps)) return false;

  Particle  ll = (*leps[0]) + (*leps[1]);
  if (ll.M() < 55) return false;
  if (fabs(ll.M()-90.) < 15) return false;
  
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl < 2) return false;

  Fill_RegionPlots(channel,1,"HNL_OS_Top_TwoLepton_CR_Nolepveto" , param.Name, JetColl  ,AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
  return true;
}

bool HNL_RegionDefinitions::FillTopCRPlots_2jet(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(SameCharge(leps)) return false;

  Particle  ll = (*leps[0]) + (*leps[1]);
  if (ll.M() < 55) return false;
  if (fabs(ll.M()-90.) < 15) return false;
  
  if(JetColl.size() < 2) return false;

  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl < 2) return false;

  Fill_RegionPlots(channel,1,"HNL_OS_Top_TwoLepton_CR_2jet" , param.Name, JetColl  ,AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
  return true;
}


bool HNL_RegionDefinitions::FillZNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 30.;
  double mtcut = 30.;


  if(METv.Pt() > metcut) return false;
  if(GetIndexNonBestZ(leps,15.) < 0) return false;
  if(M_T((*leps[GetIndexNonBestZ(leps,15.)]), METv) > mtcut) return false;

  if(run_Debug){
    cout << "HNL_ZNP_ThreeLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZNP_ThreeLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(channel,1,"HNL_ZNP_ThreeLepton_CR" , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::FillZAK8CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (leps_veto.size() != 2) return false;
  
  
  if(METv.Pt() > 30) return false;
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl > 0) return false;
  if(AK8_JetColl.size() == 0) return false;

  Particle  ll = (*leps[0]) + (*leps[1]);

  if(ll.Charge() != 0) return false;
  if (fabs(ll.M()-90.) > 10) return false;

  if(run_Debug){
    cout << "HNL_ZAK8_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZAK8_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }
  if(SameCharge(leps))Fill_RegionPlots(channel, true,"HNL_SS_ZAK8_TwoLepton_CR" , param.Name, JetColl , AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);
  else Fill_RegionPlots(channel, true,"HNL_OS_ZAK8_TwoLepton_CR" , param.Name, JetColl , AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);
  return true;

}

bool HNL_RegionDefinitions::FillZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(SameCharge(leps)) return false;
  if (leps_veto.size() != 2) return false;
  
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl > 0) return false;

  Particle  ll = (*leps[0]) + (*leps[1]);
  if (ll.M() < 55) return false;

  Fill_RegionPlots(channel, true,"HNL_OS_Z_TwoLepton_CR" , param.Name, JetColl , AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);
  return true;
}

bool HNL_RegionDefinitions::FillZCRPlots_Nolepveto(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(SameCharge(leps)) return false;
  
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl > 0) return false;

  Particle  ll = (*leps[0]) + (*leps[1]);
  if (ll.M() < 55) return false;

  Fill_RegionPlots(channel, true,"HNL_OS_Z_TwoLepton_CR_Nolepveto" , param.Name, JetColl , AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);
  return true;
}


bool HNL_RegionDefinitions::FillWWCR1Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){
  
  // This CRis same as AN2021_008 Table 10
  // Same as SR but Inverse DPhi cut
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;

  int NB_JetColl = B_JetColl.size();
  /***
   //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045  
   //https://arxiv.org/pdf/2005.01173.pdf 
   **/

  
  Particle ll =  *leps[0] + *leps[1];


  // two highest pt jets with    
  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( ll.M() < 20.) return false;
  //  if ( METv.Pt() < 30.) return false;
  if (NB_JetColl>0) return false;
  
  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max(leps[0]->Eta()  - Av_JetEta , (leps[1])->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;

  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi >  2.) return false;

  if(run_Debug){
    cout << "HNL_WpWp_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWp_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }
  Fill_RegionPlots(channel, true,"HNL_WpWp_TwoLepton_CR1" , param.Name, jets_eta5,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitions::FillWWCR2Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){

  // This CRis same as AN2021_008 Table 10                                                                                                                                                                         
  // Same as SR but High MET

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;

  int NB_JetColl = B_JetColl.size();
  /***                                                                                                                                                                                                             
   //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                         
   //https://arxiv.org/pdf/2005.01173.pdf                                                                                                                                                                          
   **/


  Particle ll =  *leps[0] + *leps[1];


  // two highest pt jets with                                                                                                                                                                                      
  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( ll.M() < 20.) return false;

  double ST = GetST(leps, jets_eta5, AK8_JetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  if(met2_st > 15) return false;


  if (NB_JetColl>0) return false;

  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max(leps[0]->Eta()  - Av_JetEta , (leps[1])->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;


  if(run_Debug){
    cout << "HNL_WpWp_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWp_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }
  Fill_RegionPlots(channel, true,"HNL_WpWp_TwoLepton_CR2" , param.Name, jets_eta5,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitions::FillWWCRNPPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  // This is the same as AN2020_045 Table 15 Nonprompt

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;
  


  std::vector<Jet> JetColl                  = SelectAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();
  //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045
  //https://arxiv.org/pdf/2005.01173.pdf 
  
  Particle ll =  *leps[0] + *leps[1];

  
  // two highest pt jets with   
  
  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  
  if ( METv.Pt() < 30.) return false;
  if (NB_JetColl==0) return false;
  if ((j1+j2).M() < 500.) return false;
  
  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max((leps[0])->Eta()  - Av_JetEta , leps[1]->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;



  if(run_Debug){
    cout << "HNL_WpWpNP_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(channel, true,"HNL_WpWpNP_TwoLepton_CR" , param.Name, jets_eta5,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);
  
  return true;

}

bool HNL_RegionDefinitions::FillWWCRNP2Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  // This is the same as AN2020_045 Table 15 Nonprompt                                                                                                                                                             

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;



  std::vector<Jet> JetColl                  = SelectAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();
  //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                          
  //https://arxiv.org/pdf/2005.01173.pdf                                                                                                                                                                           

  Particle ll =  *leps[0] + *leps[1];


  // two highest pt jets with                                                                                                                                                                                      

  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;

  if (NB_JetColl==0) return false;
  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max((leps[0])->Eta()  - Av_JetEta , leps[1]->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;



  if(run_Debug){
    cout << "HNL_WpWpNP2_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP2_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(channel, true,"HNL_WpWpNP2_TwoLepton_CR" , param.Name, jets_eta5,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::FillWWCRNP3Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  // low mjj region

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;

  std::vector<Jet> JetColl                  = SelectAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();

  Particle ll =  *leps[0] + *leps[1];

  // two highest pt jets with                                                                                                                                                                                      
  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;

  if ( ll.M() < 20.) return false;
  if (NB_JetColl>0) return false;
  if ((j1+j2).M() < 150. || (j1+j2).M() > 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;

  if(run_Debug){
    cout << "HNL_WpWpNP3_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP3_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(channel, true,"HNL_WpWpNP3_TwoLepton_CR" , param.Name, jets_eta5,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitions::FillOSPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;
  if(ll.M() < 110) return false;

  //double ST = GetST(leps, jets, AK8_JetColl, ev);
  //  double met2_st = pow(METv.Pt(),2.)/ ST;

  int njet = AK8_JetColl.size();

  if(njet < 1) return false;

  Fill_RegionPlots(channel,1,"HNL_OSPresel_TwoLepton"  , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitions::FillSSPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;
  if(ll.M() < 20) return false;

  //double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  //double met2_st = pow(METv.Pt(),2.)/ ST;

  //int njets = JetColl.size() + AK8_JetColl.size();

  Fill_RegionPlots(channel,1,"HNL_SSPresel_TwoLepton"  , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
  return true;

}



bool HNL_RegionDefinitions::FillSSVBFPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Jet> VBF_JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;



  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;
  if(ll.M() < 20) return false;


  if(VBF_JetColl.size() < 2) return false;
  Fill_RegionPlots(channel,1,"HNL_SSVBFPresel_TwoLepton"  , param.Name, VBF_JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::FillHighMassSR1CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;


  int NB_JetColl = B_JetColl.size();


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;
  if(ll.M() < 20) return false;

  double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 20);


  //double LowerMassSR1WmassCut = 30.;
  //double UpperMassSR1WmassCut = 150.;


  if(PassHMMet && NB_JetColl==0) return false;

  if(run_Debug){
    cout << "HNL_HighMassSR1_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps){
      Gen gen_closest = GetGenMatchedLepton(*ilep, All_Gens);

      cout << "HNL_HighMassSR1_TwoLepton_CR Type " <<  ilep->LeptonGenType() << " " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << " matched gen = " <<  gen_closest.Index() << endl;


    }

    for(auto ilep: leps)  cout << "HNL_HighMassSR1_TwoLepton_CR Type " <<  ilep->LeptonGenType() << " " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << endl;
  }


  if(AK8_JetColl.size()==0)    return false;

  Particle N1cand = AK8_JetColl[0] + *leps[0];

  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};

  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M(); //JH : FIXME this is not desirable
  FillHist( "ControlSR1/"+param.Name+"/N1Mass_Central",  MN1,  w, 6, ml1jbins, "Reco M_{l1jj}");


  FillEventCutflow(HNL_LeptonCore::ChannelDepCR1, w, GetChannelString(channel) +"_CR1", "ChannelCutFlow/"+param.DefName);

  Fill_RegionPlots(channel,1,"HNL_HighMassSR1_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitions::FillHighMass1JetCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if (channel!=EE ) return false;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;



  Particle ll =  (*leps[0]) + (*leps[1]);
  // Kepp only Z peak events with many CF
  if (fabs(ll.M()-90.) > 10) return false;
  
  if(JetColl.size() != 1) return false;
  Fill_RegionPlots(channel,1,"HNL_HighMass1Jet_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);


  return true;

}


bool HNL_RegionDefinitions::FillHighMassBJetCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){



  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) > 10)) return false;
  if(ll.M() < 10) return false;
  int NB_JetColl = B_JetColl.size();

  if(NB_JetColl  < 1) return false;
  Fill_RegionPlots(channel,1,"HNL_HighMassBJet_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitions::FillHighMassNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  Particle ll =  (*leps[0]) + (*leps[1]);
  if(ll.M() < 10) return false;

  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(JetColl.size() > 0) return false;


  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;

  if(leps[0]->DeltaPhi(*leps[1]) < 2.5) return false;

  Fill_RegionPlots(channel,1,"HNL_HighMassNP_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitions::FillHighMassSR3BDTCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> JetVBFColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){
  
  if (!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;

  
  int NB_JetColl = B_JetColl.size();
  
  Particle ll ;//=  leps.at(0) + leps.at(1);
  
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;


  if(ll.M() < 20) return false;

  double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  if(AK8_JetColl.size() > 0) return false;

  std::vector<Tau>        TauColl        = GetTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

  
  for(auto imapHP :FinalBDTHyperParamMap){
    TString SRBDT = RunSignalRegionAK4StringBDT(true,imapHP.first , imapHP.second.first, imapHP.second.second,  channel,Inclusive, leps, JetColl, B_JetColl, ev, METv ,param,"", w);
  }


  return true;
}


bool HNL_RegionDefinitions::FillHighMassSR3CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  int NB_JetColl = B_JetColl.size();

  Particle ll =  (*leps[0]) + (*leps[1]);
  
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;


  if(ll.M() < 20) return false;

  double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  double LowerMassSR3WmassCut = 50.;
  double UpperMassSR3WmassCut = 150.;

  if(AK8_JetColl.size() > 0) return false;


  if(JetColl.size() == 0 && leps[1]->Pt() > 80.) FillHist( "ControlSR3/"+param.Name+"/SignalBins",   0.5, w, 16, 0, 16., "Signalbins");
  if(JetColl.size() == 1 && leps[1]->Pt() > 80.) FillHist( "ControlSR3/"+param.Name+"/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");
  
  if (JetColl.size() < 2 && leps[1]->Pt() > 80.) Fill_RegionPlots(channel,1,"HNL_HighMassSR3b_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  if (JetColl.size() < 2  && leps[1]->Pt() > 80.) {
    Fill_RegionPlots(channel,1,"HNL_HighMassSR3LowJet_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
    return true;
  }
  if (JetColl.size() < 2) return false;

  if(!(GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) < UpperMassSR3WmassCut && GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) > LowerMassSR3WmassCut)) return false;

  Fill_RegionPlots(channel,1,"HNL_HighMassSR3_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=1; enne<JetColl.size(); enne++) {

      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();
      if(emme == enne) continue;

      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
        n = enne;
      }
    }
  }

  Particle N1cand = JetColl[m] + JetColl[n]+ *leps[0];
  Particle N2cand = JetColl[m] + JetColl[n]+ *leps[1];
  
  double bin(0.);
  if (met2_st < 9){
    if(leps[0]->Pt() < 25) {
      if(N2cand.M() < 100.) {
        bin=2.5;
      }
      else{
        bin=3.5;
      }
    }

    else if(leps[1]->Pt() < 60) {
      if(N2cand.M() < 100.) {
        bin=4.5;
      }
      else if(N2cand.M() < 125.) {
        bin=5.5;
      }
      else if(N2cand.M() < 150.) {
        bin=6.5;
      }
      else if(N2cand.M() < 175.) {
        bin=7.5;

              }
      else if(N2cand.M() < 200.) {
        bin=8.5;
      }
      else {
        bin=9.5;
      }
    }
    else{
      if(N1cand.M() < 200) {
        bin=10.5;
      }
      else  if(N1cand.M() < 250.) {
        bin=11.5;
      }
      else if(N1cand.M() < 300.) {
        bin=12.5;
      }
      else if(N1cand.M() < 500.) {
        bin=13.5;
      }
      else if(N1cand.M() < 1000.) {
        bin=14.5;
      }
      else {
        bin = 15.5;
      }
    }
  }

  FillHist( "ControlSR3/"+param.Name+"/SignalBins",   bin, w, 16, 0, 16., "Signalbins");
  FillEventCutflow(HNL_LeptonCore::ChannelDepCR3, w, GetChannelString(channel) +"_CR3", "ChannelCutFlow/"+param.DefName);

  return true;
  
}



bool HNL_RegionDefinitions::FillHighMassSR2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;


  int NB_JetColl = B_JetColl.size();
  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;
  if(ll.M() < 20) return false;

  double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  if(PassVBF(JetColl,leps,450)) {
    
  
    double HT(0.);
    for(UInt_t emme=0; emme<JetColl.size(); emme++){
      HT += JetColl[emme].Pt();
    }
    if (HT/leps[0]->Pt() > 2)  return false;
    
    Fill_RegionPlots(channel,1,"HNL_HighMassCR2_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
    
  
    double HTLT[2] = { 0., 2};
    FillHist( "ControlSR2/"+param.Name+"/HT_LT1_Central",  HT/leps[0]->Pt(),  w, 1, HTLT, "Reco HT/LT1");
    FillEventCutflow(HNL_LeptonCore::ChannelDepCR2, w, GetChannelString(channel) +"_CR2", "ChannelCutFlow/"+param.DefName);
  
    return true;
  }
  return false;
}


bool HNL_RegionDefinitions::FillWZ2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if(jets_eta5.size() < 2) return false;

  double metcut = 30.;
  double trilep_masscut=105.;


  std::vector<Jet> JetColl                  = SelectAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();

  ////file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                        
  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < jets_eta5.size(); ij++){
    for(unsigned int ij2 = ij+1; ij2 < jets_eta5.size(); ij2++){
      double deta = fabs(jets_eta5[ij].Eta() - jets_eta5[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }
  if(GetIndexNonBestZ(leps,15.) < 0) return false;
  if(GetMassMinOSSF(leps)  < 20.) return false;
  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;


  if(jets_eta5.size() < 2) return false;
  Jet j1 = jets_eta5[ijet1] ;
  Jet j2 = jets_eta5[ijet2];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( METv.Pt() < metcut) return false;
  if (NB_JetColl>0) return false;
  if ((j1+j2).M() < 500.) return false;
  if (maxDiJetDeta<2.5) return false;

  double Av_JetEta= 0.5*(jets_eta5[ijet1].Eta()+ jets_eta5[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  double zeppenfeld2 = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[2]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  if(zeppenfeld2 > zeppenfeld) zeppenfeld=zeppenfeld2;

  if (zeppenfeld > 1.00) return false;

  Fill_RegionPlots(channel, 1,"HNL_WZ2_ThreeLepton_CR" , param.Name, jets_eta5,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);


  return true;

}

bool HNL_RegionDefinitions::FillWZBCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if(jets_eta5.size() < 2) return false;

  double metcut = 30.;
  double trilep_masscut=105.;


  std::vector<Jet> JetColl                  = SelectAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();
  //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                          


  if(jets_eta5.size() < 2) return false;
  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < jets_eta5.size(); ij++){
    for(unsigned int ij2 = ij+1; ij2 < jets_eta5.size(); ij2++){
      double deta = fabs(jets_eta5[ij].Eta() - jets_eta5[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }

  if(GetIndexNonBestZ(leps,15.) < 0) return false;
  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;

  Jet j1 = jets_eta5[ijet1] ;
  Jet j2 = jets_eta5[ijet2];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( METv.Pt() < metcut) return false;
  if (NB_JetColl==0) return false;
  if ((j1+j2).M() < 500.) return false;
  if (maxDiJetDeta<2.5) return false;

  double Av_JetEta= 0.5*(jets_eta5[ijet1].Eta()+ jets_eta5[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  double zeppenfeld2 = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[2]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  if(zeppenfeld2 > zeppenfeld) zeppenfeld=zeppenfeld2;

  if (zeppenfeld > 1.00) return false;


  Fill_RegionPlots(channel, 1,"HNL_WZB_ThreeLepton_CR" , param.Name, jets_eta5,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);


  return true;

}


bool HNL_RegionDefinitions::FillZZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  int NB_JetColl=B_JetColl.size();


  if(NB_JetColl  > 0 ) return false;
  bool m_llos_l10(false), z_cr_pass(false);

  Particle Z1Cand;
  Particle Z2Cand;


  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
      if(leps[iel]->Charge() != leps[iel2]->Charge()){
        if(Z1Cand.M() < 10) m_llos_l10=true;

        int zel1(-9), zel2(-9);
        if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = (*leps[2]) + (*leps[3]);}
        if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = (*leps[1]) + (*leps[3]);}
        if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = (*leps[1]) + (*leps[2]);}
        if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = (*leps[0]) + (*leps[3]);}
        if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = (*leps[0]) + (*leps[2]);}
        if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = (*leps[0]) + (*leps[1]);}

        if(leps[zel1]->Charge() != leps[zel2]->Charge()){
          if(fabs(Z1Cand.M() - 90.1) < 10.){
      if(fabs(Z2Cand.M() - 90.1) < 10.){
        z_cr_pass=true;
      }
          }
        }
      }
    }
  }
  if(m_llos_l10 || !z_cr_pass)  return false;
  Fill_RegionPlots(channel, 1,"HNL_ZZ_FourLepton_CR" , param.Name, JetColl,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitions::FillZZ2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){



  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  int NB_JetColl=B_JetColl.size();


  if(NB_JetColl  > 0 ) return false;
  bool m_llos_l10(false), z_cr_pass(false);

  Particle Z1Cand;

  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
      if(leps[iel]->Charge() != leps[iel2]->Charge()){
        if(Z1Cand.M() < 12) m_llos_l10=true;

        if(fabs(Z1Cand.M() - 90.1) < 10.){
          z_cr_pass=true;
        }
      }
    }
  }



  if(m_llos_l10 || !z_cr_pass)  return false;
  Fill_RegionPlots(channel, 1,"HNL_ZZLoose_FourLepton_CR" , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitions::FillZGCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  //if(MCSample.Contains("DY")) return false;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 50.;
  int NB_JetColl=B_JetColl.size();

  FillHist( "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_1",   1,  w, 14, 0, 14., "FillZGCRPlots");

  FillHist( "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_MLLL",   ((*leps[0]) + (*leps[1])+ (*leps[2])).M(),  w, 200, 0, 4000., "FillZGCRPlots");
  
  Particle lll = (*leps[0]) + (*leps[1])+ (*leps[2]);
  bool passZmass_lll_Window = (fabs(lll.M() - 90.1) < 10.);
  if(!passZmass_lll_Window) return false;

  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_2",   1,  w, 14, 0, 14., "FillZGCRPlots");
  Particle ll1 = *leps[0] + *leps[1];
  Particle ll2 = *leps[0] + *leps[2];
  Particle ll3 = *leps[1] + *leps[2];

  bool RemoveLowOSSFMass=false;
  bool OSSFMInZPeak=false;
  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle OSSFll = *leps[i] + *leps[j];
      FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_OSSSM",   OSSFll.M(),  w, 200, 0, 500., "FillZGCRPlots");
      if(fabs(OSSFll.M() - 90.1) < 10) OSSFMInZPeak=true;
      if(OSSFll.M() < 10) RemoveLowOSSFMass=true;

    }
  }
  
  if(RemoveLowOSSFMass) return false;
  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_3",   1,  w, 14, 0, 14., "FillZGCRPlots");

  if(OSSFMInZPeak) return false;
  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_4",   1,  w, 14, 0, 14., "FillZGCRPlots");

  if(ZmassOSSFWindowCheck(leps,15.)) return false;

  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_5",   1,  w, 14, 0, 14., "FillZGCRPlots");
  if(NB_JetColl > 0) return false;

  if(METv.Pt() > metcut) return false;

      
  if(run_Debug){
    cout << "HNL_ZG_ThreeLepton_CR " << param.Name << " " << event  << endl;

    //for(auto ilep: leps){
    // Gen gen_closest = GetGenMatchedLepton(*ilep, All_Gens);
    //  Gen gen_photon_closest = GetGenMatchedPhoton(*ilep, All_Gens);
      //int NearPhotonType = GetGenPhotonType(gen_photon_closest,All_Gens);

    //      FillHist("Photon_pt_HNL_ZG_ThreeLepton_CR",gen_photon_closest.Pt() , w, 100., 0.,  200.);
    // }

    //PrintGen(All_Gens);
  }

  Fill_RegionPlots(channel,1,"HNL_ZG_ThreeLepton_CR" , param.Name, JetColl,  AK8_JetColl,  B_JetColl, leps, leps_veto, METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitions::FillWGCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if(HasLowMassMeson(leps)) return false;

  double metcut = 30.;
  double mt_cut = 30.;
  int NB_JetColl=B_JetColl.size();

  Particle ll1 = (*leps[0]) + (*leps[1]);
  Particle ll2 = (*leps[0]) + (*leps[2]);
  Particle ll3 = (*leps[1]) + (*leps[2]);
  bool passlos_ll_mass=false;
  if(ll1.Charge() == 0 && (ll1.M() < 4.)) passlos_ll_mass=true;
  if(ll2.Charge() == 0 && (ll2.M() < 4.)) passlos_ll_mass=true;
  if(ll3.Charge() == 0 && (ll3.M() < 4.)) passlos_ll_mass=true;
  if(!passlos_ll_mass) return false;
  Particle lll = (*leps[0]) + (*leps[1])+ (*leps[2]);
  double MT_lll = M_T(METv,lll);

  if(MT_lll <= mt_cut) return false;
  if(NB_JetColl > 0)return false;
  if(METv.Pt() < metcut)return false;

  if(run_Debug){
    cout << "HNL_WG_ThreeLepton_CR " << param.Name << " " << event  << endl;

    PrintGen(All_Gens);
  }
  Fill_RegionPlots(channel,1,"HNL_WG_ThreeLepton_CR"  , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitions::FillWZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  //  cout << "FillWZCRPlots " << param.Name << " " << leps.size() << endl;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 50.;
  double mtcut = 20.;
  double trilep_masscut=105.;


  int NB_JetColl=B_JetColl.size();
  

  if(METv.Pt() < metcut) return false;
  
  if(NB_JetColl > 0) return false;
  
  if(GetIndexNonBestZ(leps,15.) < 0) return false;

  if(M_T((*leps[GetIndexNonBestZ(leps,15.)]), METv)  < mtcut) return false;

  if(GetMassMinOSSF(leps)  < 10.) return false;

  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;
  
  Fill_RegionPlots(channel,1,"HNL_WZ_ThreeLepton_CR" , param.Name, JetColl,  AK8_JetColl, B_JetColl,  leps, leps_veto, METv, nPV, w);
  
  return true;
}


