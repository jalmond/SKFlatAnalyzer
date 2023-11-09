#include "HNL_RegionDefinitions.h"

void HNL_RegionDefinitions::RunAllControlRegions(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,   std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, vector<TString> CRs, float weight_ll ){
  
  vector<HNL_LeptonCore::Channel> channels = {GetChannelENum(param.Channel)};
 
  if(GetChannelENum(param.Channel) == HNL_LeptonCore::NONE){
    cout << "CHANNEL NOT SET" << endl;
    exit(EXIT_FAILURE);
  }

  if(run_Debug) cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;  
  int nlog(0);
  //  if(run_Debug) {cout << "RunAllControlRegions ["<< nlog<< "] : Start Loop " << endl; nlog++;}

  
  for(unsigned int ic = 0; ic < channels.size(); ic++){
    
    if(run_Debug) cout << "HNL_RegionDefinitions::RunAllControlRegions [" << GetChannelString(channels[ic])<<" ]" << endl;
    
    HNL_LeptonCore::Channel dilep_channel   = channels[ic];
    HNL_LeptonCore::Channel trilep_channel  = GetTriLeptonChannel(dilep_channel);
    HNL_LeptonCore::Channel fourlep_channel = GetQuadLeptonChannel(dilep_channel);

    // Change Name of param to include Flavour channel                                                                                                                                                           
    HNL_LeptonCore::SearchRegion CutFlow_Region = ControlRegion;

    TString channel_string = GetChannelString(dilep_channel);
    param.Channel    =  channel_string;
    param.CutFlowDir = "CutFlow";
    
    param.Name = channel_string + "/" + param.Name ;

    std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(muons,     electrons,     param);
    std::vector<Lepton *> LepsV       = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
    
    FillCutflow(CutFlow_Region, weight_ll, "NoCut", param);
    
    if(!PassHEMVeto(LepsV)) continue;
    FillCutflow(CutFlow_Region, weight_ll, "HEMVeto", param);

    if(!PassMETFilter()) return;

    FillCutflow(CutFlow_Region, weight_ll, "METFilter",param);

    if(run_Debug) {cout <<"RunAllControlRegions ["<< nlog<< "] : Pass METFilters" << endl;nlog++;}
    
    /// Filters events based on COnv/CF/Fake/Prompt
    
    if(! (HasFlag("OS_VR") && RunFake)){
      if(!PassGenMatchFilter(LepsT,param)) return;
    }
    
    FillCutflow(CutFlow_Region, weight_ll, "GENMatched",param);

    if(run_Debug) cout << "HNL_RegionDefinitions::RunAllControlRegions [" << GetChannelString(channels[ic])<<" ]" << endl;
    
    //// Make Trilep/4 lep param
    AnalyzerParameter paramTrilep  = param;
    AnalyzerParameter paramQuadlep = param;
    paramTrilep.Channel  = GetChannelString(trilep_channel);
    paramTrilep.Name     = GetChannelString(trilep_channel)  + "/" + param.Name ;
    paramQuadlep.Channel = GetChannelString(fourlep_channel);
    paramQuadlep.Name    = GetChannelString(fourlep_channel) + "/" + param.Name ;
    
    double weight_channel = weight_ll;
   
    TString label    = param.Name;
   
    if(! (CheckLeptonFlavourForChannel(dilep_channel, LepsT) 
	  || CheckLeptonFlavourForChannel(trilep_channel, LepsT) 
	  || CheckLeptonFlavourForChannel(fourlep_channel, LepsT))) continue;


    FillCutflow(CutFlow_Region, weight_channel, "LeptonFlavour",param);

    if(run_Debug) {cout <<"RunAllControlRegions ["<< nlog<< "] pass Lep Flavour" << endl;nlog++;}

    if(dilep_channel == EMu || dilep_channel == MuE){
      if(param.TriggerSelection == "POGSgLep"){
	/// For Single Lep its on OR of two PD so need to add veto logic to not double ocunt in data
	if ( PassMultiDatasetTriggerSelection(dilep_channel, ev, LepsT, "POGSgMu", "POGSglEl")) continue;
      }
      else{
	if (!PassTriggerSelection(dilep_channel, ev, LepsT,param.TriggerSelection)) continue;
      }
    }
    else{
      
      if (!PassTriggerSelection(dilep_channel, ev, LepsT,param.TriggerSelection)) continue;
    }

    EvalTrigWeight(dilep_channel, muons,electrons,param, weight_channel);

    FillCutflow(CutFlow_Region, weight_channel, "Trigger",param);

    double weight_OS = weight_channel;

    /// For OS Fakes use SS TT events - VV , but RunFake uses LL so need to apply Tight ID 
    bool PassTight = true;
    if(RunFake){
      if(HasFlag("OS_VR") && (dilep_channel==EE || dilep_channel==MuMu) ){
	
	if(dilep_channel==EE   && !SameCharge(electrons))   continue;
	if(dilep_channel==MuMu && !SameCharge(muons))       continue;
	
	for(auto ilep : muons) {
	  if(!ilep.PassID(param.Muon_Tight_ID))  PassTight=false;
	}
	for(auto ilep : electrons) {
	  if(!ilep.PassID(param.Electron_Tight_ID))PassTight=false;
	}
	
	if(!IsData) weight_OS *=-1;
	else        weight_OS =1;
      }
      
      else {
	if(IsData){
	  weight_channel = GetFakeWeight(LepsT, param );
	  
	  //int nT = 0;
	  //for(auto ilep : muons) {
	  //  if(ilep.PassID(param.Muon_Tight_ID)) nT++;
	  // }
	  //cout << param.Name << " weight_channel = " << weight_channel << " nT = " << nT << endl;
	  
	  if(HasFlag("OS_VR")) weight_OS = weight_channel;
	  if(LepsT.size()==2)FillFakeWeightHist(param.Name+"_2L/FakeWeight", LepsT,param, weight_channel);
	  if(LepsT.size()==3)FillFakeWeightHist(param.Name+"_3L/FakeWeight", LepsT,param, weight_channel);
	  if(LepsT.size()==4)FillFakeWeightHist(param.Name+"_4L/FakeWeight", LepsT,param, weight_channel);
	  if(run_Debug) {cout <<"RunAllControlRegions ["<< nlog<< "] Fake Weight = " << weight_channel << endl;nlog++;}
	}
      }
    }

    if(RunCR("BDTCheck",CRs)){
      double BDTweight_channel = weight_channel;
      if( B_JetColl.size() > 0)  return;
      if (!PassTriggerSelection(dilep_channel, ev, LepsT,"POG")) return;
      if(LepsV.size() ==2){
	Particle  ll = (*LepsV[0]);
	ll+= (*LepsV[1]);
	if(!IsData && ll.Charge() != 0) return;
	if(IsData  && ll.Charge() != 0) BDTweight_channel = -1;
	if (fabs(ll.M()-M_Z) <  10) {
	  for(auto ilep : electrons) {
	    if(!IsData && !ilep.IsPrompt()) continue;
	    if(!IsData && ilep.LeptonIsCF()) continue;
	    /// dont subtract SS for high pt 
	    if(IsData  && ll.Charge() != 0 && ilep.Pt() > 100) continue;
	    TString CFMVA = "";
	    TString ConvMVA = "";
	    TString FakeMVA = "";
	    if( FindHEMElectron (ilep)) FillBDTHists(ilep,"HEM",BDTweight_channel);
	    if( FindHEMElectron (ilep))  continue;
	    TString ptstring = "Pt1";
	    if(ilep.Pt() < 30 ) ptstring = "Pt1";
	    else if(ilep.Pt() < 150 ) ptstring = "Pt2";
	    else ptstring = "Pt3";
	    TString etastring = ilep.sEtaRegion();
	    FillBDTHists(ilep,ptstring+"_"+etastring,BDTweight_channel);
	  }
	}
      }      
      return;
    }
    

    vector<TString> passed;
    if(HasFlag("OS_VR")){
      //// OS L+L-
      
      FillCutflow(CutFlow_Region, weight_OS, "OS_VR",param);
      
      if(PassTight && FillZAK8CRPlots  (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_OS)) passed.push_back("ZAK8_CR");
      if(PassTight && FillTopAK8CRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_OS)) passed.push_back("TopAK8_CR");
      if(PassTight && FillZCRPlots     (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_OS)) passed.push_back("Z_CR");
      if(PassTight && FillTopCRPlots   (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_OS)) passed.push_back("Top_CR");
      if(PassTight && FillTopCR2Plots (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_OS))  passed.push_back("Top_CR2");
      
    }
      
    if(HasFlag("LLL_VR")){
	
      FillCutflow(CutFlow_Region, weight_channel, "VV_VR",param);
      
      // LLL / LLLL 
      if(FillZZCRPlots (fourlep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramQuadlep, weight_channel)) passed.push_back("ZZ_CR");
      if(FillZZ2CRPlots(fourlep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramQuadlep, weight_channel)) passed.push_back("ZZLoose_CR"); 
      if(FillWZCRPlots (trilep_channel,  LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel))  passed.push_back("WZ_CR");

      if(ConversionSplitting(LepsT,RunConv,3)){
	FillCutflow(CutFlow_Region, weight_channel, "VG_VR",param);
	if(FillWGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WG_CR");
	if(FillZGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("ZG_CR");
      }
      if(FillWZ2CRPlots      (trilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("WZ2_CR");
      if(FillWZBCRPlots      (trilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel)) passed.push_back("WZB_CR");
      if(FillZ_ElNPCRPlots   (trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel))     passed.push_back("ZNPEl_CR");
      if(FillZ_MuonNPCRPlots (trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel))     passed.push_back("ZNPMu_CR");
      if(FillTopNPCRPlots    (trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTrilep, weight_channel))     passed.push_back("TopNP_CR");
    }
  
    
    if(HasFlag("SS_CR") || HasFlag("VBF_CR")){

      if(HasFlag("SS_CR"))  FillCutflow(CutFlow_Region, weight_channel, "SS_CR",param);
      if(HasFlag("VBF_CR")) FillCutflow(CutFlow_Region, weight_channel, "VBF_CR",param);
      
      if(RunCF){    
	if(LepsT.size() == 2){
	  if(dilep_channel == MuMu)       continue;
	  if(IsData && SameCharge(LepsT)) continue;
	  if(IsData){
	    weight_channel = GetCFWeightElectron(LepsT, param);
	    FillWeightHist(param.ChannelDir()+"/CFWeight",weight_channel);
	  }
	  if(!IsData && !SameCharge(LepsT)) continue;
	}
	else continue;
      }
      else if(LepsT.size() == 2){
	if(!SameCharge(LepsT)) continue;
      }
      
    }
    
    if(HasFlag("VBF_CR")){
      if(FillWWCR1Plots  (dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel))      passed.push_back("WpWp_CR1");
      if(FillWWCR2Plots  (dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel))      passed.push_back("WpWp_CR2");
      if(FillWWCRNPPlots (dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel))      passed.push_back("WpWp_CRNP");
      if(FillWWCRNP2Plots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel))      passed.push_back("WpWp_CRNP2");
      if(FillWWCRNP3Plots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel))      passed.push_back("WpWp_CRNP3");
    }
    if(HasFlag("SS_CR")){
      FillHighMassSR3BDTCRPlots(dilep_channel, LepsT, LepsV, JetAllColl,  JetColl,VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel);
      if(FillSSPreselectionPlots(dilep_channel,    LepsT, LepsV, JetColl,     AK8_JetColl, B_JetColl, ev, METv, param, weight_channel))	passed.push_back("SSPresel");
      if(FillHighMassSR1CRPlots(dilep_channel, LepsT, LepsV, JetColl,     AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR1_CR");
      if(FillHighMassSR2CRPlots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR2_CR");
      if(FillHighMassSR3CRPlots(dilep_channel, LepsT, LepsV, JetColl,     AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR3_CR");
      if(FillHighMass1JetCRPlots(dilep_channel, LepsT, LepsV, JetColl,    AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMass1Jet_CR");
      if(FillHighMassBJetCRPlots(dilep_channel, LepsT, LepsV, JetColl,    AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassBJet_CR");
      if(FillHighMassNPCRPlots(dilep_channel, LepsT, LepsV, JetColl,      AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassNP_CR");
    }
        
    TString ControlLabel = "";
    vector<TString> cutlabels = {};
    
    if(HasFlag("OS_VR")){
      cutlabels.push_back("ZAK8_CR");
      cutlabels.push_back("TopAK8_CR");
      cutlabels.push_back("Z_CR");
      cutlabels.push_back("Top_CR");
      cutlabels.push_back("Top_CR2");
      ControlLabel+="OS_VR";
    }
    
    if(HasFlag("LLL_VR")) {
      cutlabels.push_back("ZZ_CR");
      cutlabels.push_back("ZZOffshell_CR");
      cutlabels.push_back("WZ_CR");
      cutlabels.push_back("WG_CR");
      cutlabels.push_back("ZG_CR");
      cutlabels.push_back("WZ2_CR");
      cutlabels.push_back("WZB_CR");
      cutlabels.push_back("ZNPEl_CR");
      cutlabels.push_back("ZNPMu_CR");
      cutlabels.push_back("TopNP_CR");
      ControlLabel+="LLL";
    }

    if(HasFlag("SS_CR")) {
      cutlabels.push_back("SSPresel");
      cutlabels.push_back("HighMassSR1_CR");
      cutlabels.push_back("HighMassSR2_CR");
      cutlabels.push_back("HighMassSR3_CR");
      cutlabels.push_back("HighMass1Jet_CR");
      cutlabels.push_back("HighMassBJet_CR");
      cutlabels.push_back("HighMassNP_CR");
      ControlLabel+="SS";
    }

    if(HasFlag("VBF_CR")){
      cutlabels.push_back("WpWp_CR1");
      cutlabels.push_back("WpWp_CR2");
      cutlabels.push_back("WpWp_CRNP");
      cutlabels.push_back("WpWp_CRNP2");
      cutlabels.push_back("WpWp_CRNP3");
      ControlLabel+="SSVBF";
    }
    
    //    FillCutflow(CutFlow_Region, weight_channel, "NoCut",param);

    for(auto ip : passed) FillCutflow(param.CutFlowDirChannel(), ControlLabel+"_SelectedControlRegions", weight_channel, cutlabels,ip);
    for(auto ip : passed) FillCutflow(param.CutFlowDirIncChannel(), ControlLabel+"_SelectedControlRegions", weight_channel, cutlabels,ip);
    for(auto ip : passed) FillCutflow(HNL_LeptonCore::CR, weight_channel, ip,param);
    
    for(unsigned int ipass =0; ipass < passed.size();ipass++){
      //void AnalyzerCore::FillTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){                                                                                 
      for(unsigned int ipass2=ipass+1; ipass2 < passed.size(); ipass2++){
	FillTypeCutflow(channel_string+"CR_Correlation"+param.Name, weight_channel, cutlabels, passed[ipass], passed[ipass2]);
      }
    }
  }
  return;
}



bool HNL_RegionDefinitions::FillTopCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  int NB_JetColl = B_JetColl.size();
  Particle  ll   = (*leps[0]) + (*leps[1]);

  if (!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (METv.Pt() < 50)        return false;
  if (NB_JetColl == 0)       return false;
  if (JetColl.size() < 4)    return false;
  if (channel != EMu){
    if (ll.M()  < 110)       return false;
  }

  if(run_Debug){
    cout << "HNL_TopAK8_TwoLepton_CR " << event << endl;
    for(auto ilep: leps) cout << "HNL_TopAK8_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  if(RunFake) {
    if(SameCharge(leps)) Fill_RegionPlots(param,"HNL_OS_Top_TwoLepton_CR" ,  JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  }
  else {
    if(!SameCharge(leps)) Fill_RegionPlots(param,"HNL_OS_Top_TwoLepton_CR" ,  JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  }
  return true;
}



bool HNL_RegionDefinitions::FillTopCR2Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){
  
  int NB_JetColl = B_JetColl.size();
  Particle  ll   = (*leps[0]) + (*leps[1]);
  
  if (!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (METv.Pt()  < 40)       return false;
  if (NB_JetColl < 2)        return false;
  if (JetColl.size() < 4)    return false;

  if (channel != EMu){
    if (ll.M()  < 110)       return false;
  }
  
  if(run_Debug){
    cout << "HNL_TopAK8_TwoLepton_CR " << event << endl;
    for(auto ilep: leps) cout << "HNL_TopAK8_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
    //PrintGen(gens);                                                                                                                                                                                              
  }
    
  if(RunFake) {
    if(SameCharge(leps)) Fill_RegionPlots(param,"HNL_OS_Top2b_TwoLepton_CR" ,  JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  }
  else {
    if(!SameCharge(leps)) Fill_RegionPlots(param,"HNL_OS_Top2b_TwoLepton_CR" ,  JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  }
  return true;
}


bool HNL_RegionDefinitions::FillTopAK8CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if (!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (METv.Pt() < 50) return false;

  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl == 0) return false;
  if(AK8_JetColl.size() == 0) return false;

  Particle  ll   = (*leps[0]) + (*leps[1]);
  if (channel != EMu){
    if (ll.M()  < 110)      return false;
  }
  
  if(run_Debug){
    cout << "HNL_TopAK8_TwoLepton_CR " << event << endl;
    for(auto ilep: leps) cout << "HNL_TopAK8_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
    //PrintGen(gens);                                                                                                                                                                                                                                    
  }
  if(RunFake) {
    if(SameCharge(leps)) Fill_RegionPlots(param,"HNL_SS_TopAK8_TwoLepton_CR" ,  JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  }
  else{
    if(!SameCharge(leps))Fill_RegionPlots(param,"HNL_OS_TopAK8_TwoLepton_CR" ,  JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  }
  return true;
}


bool HNL_RegionDefinitions::FillTopNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if(!HasOSSFPair(leps)) return false;
  if(fabs(GetMassBestZ(leps,true) -M_Z) < 15)  return false;

  Particle lll = (*leps[0]) + (*leps[1])+ (*leps[2]);
  bool InZmass_lll_Window = (fabs(lll.M() - M_Z) < 15);
  if(InZmass_lll_Window) return false;
  
  if(B_JetColl.size() == 0) return false;

  if(HasLowMassOSSF(leps,M_CUT_LL))  return false;

  Fill_RegionPlots(param,"HNL_TopNP_ThreeLepton_CR" ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitions::FillZ_MuonNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 30.;
  double mtcut  = 30.;

  if(METv.Pt() > metcut) return false;
  if(!ZmassOSSFWindowCheck(leps,M_ZWINDOW_CR)) return false;
  
  if(GetIndexNonBestZ(leps,M_ZWINDOW_CR) < 0) return false;
  if(M_T((*leps[GetIndexNonBestZ(leps,M_ZWINDOW_CR)]), METv) > mtcut) return false;
  
  Lepton* noZLep = leps[GetIndexNonBestZ(leps,M_ZWINDOW_CR)];
  if(noZLep->LeptonFlavour() != Lepton::MUON) return false;

  if(run_Debug){
    cout << "HNL_ZNP_ThreeLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZNP_ThreeLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(param,"HNL_ZNPMu_ThreeLepton_CR" ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::FillZ_ElNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 30.;
  double mtcut  = 30.;


  if(METv.Pt() > metcut) return false;
  if(!ZmassOSSFWindowCheck(leps,15)) return false;

  if(GetIndexNonBestZ(leps,M_ZWINDOW_CR) < 0) return false;
  if(M_T((*leps[GetIndexNonBestZ(leps,M_ZWINDOW_CR)]), METv) > mtcut) return false;

  Lepton* noZLep = leps[GetIndexNonBestZ(leps,M_ZWINDOW_CR)];
  if(noZLep->LeptonFlavour() == Lepton::MUON) return false;

  if(run_Debug){
    cout << "HNL_ZNP_ThreeLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZNP_ThreeLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(param,"HNL_ZNPEl_ThreeLepton_CR" ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::FillZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
 

  if (leps_veto.size() != 2)        return false;
  
  int NB_JetColl = B_JetColl.size();
  Particle  ll = (*leps[0]) + (*leps[1]);

  if(NB_JetColl > 0)                return false;
  
  if (fabs(ll.M()-M_Z) > M_ZWINDOW) return false;

  if(run_Debug){
    cout << "HNL_ZAK8_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZAK8_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  if(RunFake){
    if(SameCharge(leps))Fill_RegionPlots(param,"HNL_OS_Z_TwoLepton_CR" ,  JetColl , AK8_JetColl,  leps,  METv, nPV, w);
  }
  else if(!SameCharge(leps)){
    Fill_RegionPlots(param,"HNL_OS_Z_TwoLepton_CR" ,  JetColl , AK8_JetColl,  leps,  METv, nPV, w);
  }

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

  if (fabs(ll.M()-M_Z) > M_ZWINDOW) return false;

  if(run_Debug){
    cout << "HNL_ZAK8_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZAK8_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  if(RunFake){
    if(SameCharge(leps))Fill_RegionPlots(param,"HNL_SS_ZAK8_TwoLepton_CR" ,  JetColl , AK8_JetColl,  leps,  METv, nPV, w);
  }
  else{
    if(!SameCharge(leps))Fill_RegionPlots(param,"HNL_OS_ZAK8_TwoLepton_CR" ,  JetColl , AK8_JetColl,  leps,  METv, nPV, w);
  }
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
  if ( ll.M() < M_CUT_LL) return false;
  //  if ( METv.Pt() < 30.) return false;
  if (NB_JetColl>0) return false;
  
  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max(leps[0]->Eta()  - Av_JetEta , (leps[1])->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;

  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi >  2.) return false;

  if(run_Debug){
    cout << "HNL_WpWp_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWp_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }
  Fill_RegionPlots(param,"HNL_WpWp_TwoLepton_CR1" ,  jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitions::FillWWCR2Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){

  // This CRis same as AN2021_008 Table 10                                                                                                                                                                         
  // Same as SR but High MET

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(leps_veto.size() != 2) return false;

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
  if ( ll.M() < M_CUT_LL) return false;

  double met2_st = GetMET2ST(leps, jets_eta5, AK8_JetColl, METv);
  if(met2_st > 15) return false;


  if (NB_JetColl>0) return false;

  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max(leps[0]->Eta()  - Av_JetEta , (leps[1])->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;


  if(run_Debug){
    cout << "HNL_WpWp_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWp_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }
  Fill_RegionPlots(param,"HNL_WpWp_TwoLepton_CR2" ,  jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

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
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;



  if(run_Debug){
    cout << "HNL_WpWpNP_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(param,"HNL_WpWpNP_TwoLepton_CR" ,  jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);
  
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
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;



  if(run_Debug){
    cout << "HNL_WpWpNP2_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP2_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(param,"HNL_WpWpNP2_TwoLepton_CR" ,  jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

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

  if ( ll.M() < M_CUT_LL) return false;
  if (NB_JetColl>0) return false;
  if ((j1+j2).M() < 150. || (j1+j2).M() > 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;

  if(run_Debug){
    cout << "HNL_WpWpNP3_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP3_TwoLepton_CR Type " <<  ilep->LeptonGenType() << endl;
  }

  Fill_RegionPlots(param,"HNL_WpWpNP3_TwoLepton_CR" ,  jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::FillSSPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_CUT_LL)) return false;
  if(ll.M() < 20) return false;

  Fill_RegionPlots(param,"HNL_SSPresel_TwoLepton"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  if(JetColl.size() == 0)   Fill_RegionPlots(param,"HNL_SSPresel_0J_TwoLepton"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  else if(JetColl.size() == 1)   Fill_RegionPlots(param,"HNL_SSPresel_1J_TwoLepton"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  else    Fill_RegionPlots(param,"HNL_SSPresel_2J_TwoLepton"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::RunCR(TString CRName , vector<TString> CRs) {
  
  if (std::find(CRs.begin(), CRs.end(), CRName) != CRs.end())    return true;
  else return false;
  
}


bool HNL_RegionDefinitions::FillSSVBFPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Jet> VBF_JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;



  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;
  if(ll.M() < M_CUT_LL) return false;


  if(VBF_JetColl.size() < 2) return false;
  Fill_RegionPlots(param,"HNL_SSVBFPresel_TwoLepton"  ,  VBF_JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitions::FillHighMassSR1CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;


  int NB_JetColl = B_JetColl.size();


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;
  if(ll.M() < M_CUT_LL) return false;

  double met2_st = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
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

  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();
  FillHist( "ControlSR1/"+param.Name+"/N1Mass_Central",  MN1,  w, 6, ml1jbins, "Reco M_{l1jj}");


  FillCutflow(HNL_LeptonCore::ChannelDepCR1, w, GetChannelString(channel) +"_CR1", param);

  Fill_RegionPlots(param,"HNL_HighMassSR1_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitions::FillHighMass1JetCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if (channel!=EE ) return false;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;



  Particle ll =  (*leps[0]) + (*leps[1]);
  // Kepp only Z peak events with many CF
  if (fabs(ll.M()-M_Z) > M_ZWINDOW_VETO) return false;
  
  if(JetColl.size() != 1) return false;
  Fill_RegionPlots(param,"HNL_HighMass1Jet_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitions::FillHighMassBJetCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){



  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) > M_ZWINDOW_VETO)) return false;
  if(ll.M() < M_CUT_LL) return false;
  int NB_JetColl = B_JetColl.size();

  if(NB_JetColl  < 1) return false;
  Fill_RegionPlots(param,"HNL_HighMassBJet_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitions::FillHighMassNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  Particle ll =  (*leps[0]) + (*leps[1]);
  if(ll.M() < M_CUT_LL) return false;
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (leps_veto.size() != 2) return false;

  if(JetColl.size() > 0) return false;

  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;

  if(leps[0]->DeltaPhi(*leps[1]) < 2.5) return false;

  /*  if(param.FakeRateMethod == "Standard" && param.FakeRateParam == "MotherJetPt") {
    if(leps[0]->PassLepID() && leps[1]->PassLepID())  cout << " TT FillHighMassNPCRPlots " << param.Name << "   weight = " << w << endl;
    if(leps[0]->PassLepID() && !leps[1]->PassLepID())  cout << " TL FillHighMassNPCRPlots " << param.Name << "   weight = " << w << endl;
    if(!leps[0]->PassLepID() && leps[1]->PassLepID())  cout << " LT FillHighMassNPCRPlots " << param.Name << "   weight = " << w << endl;
    if(!leps[0]->PassLepID() && !leps[1]->PassLepID())  cout << " LL FillHighMassNPCRPlots " << param.Name << "   weight = " << w << endl;
    }*/
  
  Fill_RegionPlots(param,"HNL_HighMassNP_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  
  return true;

}



bool HNL_RegionDefinitions::FillHighMassSR3BDTCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetAllColl, std::vector<Jet> JetColl, std::vector<Jet> JetVBFColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){
  
  if (!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;
  
  int NB_JetColl = B_JetColl.size();
  
  Particle ll=  *leps.at(0) + *leps.at(1);
  
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;

  if(ll.M() < M_CUT_LL) return false;

  double met2_st = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  if(AK8_JetColl.size() > 0) return false;

  std::vector<Tau>        TauColl        = SelectTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

  
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
  
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;

  if(ll.M() < M_CUT_LL) return false;


  double met2_st = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  double LowerMassSR3WmassCut = 50.;
  double UpperMassSR3WmassCut = 150.;

  if(AK8_JetColl.size() > 0) return false;

  if(JetColl.size() == 0 && leps[1]->Pt() > 80.) FillHist( "ControlSR3/"+param.Name+"/SignalBins",   0.5, w, 16, 0, 16., "Signalbins");
  if(JetColl.size() == 1 && leps[1]->Pt() > 80.) FillHist( "ControlSR3/"+param.Name+"/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");

  Fill_RegionPlots(param,"HNL_HighMassSR3_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  if (JetColl.size() < 2  && leps[1]->Pt() > 80.) {
    Fill_RegionPlots(param,"HNL_HighMassSR3LowJet_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
    return true;
  }

  if (JetColl.size() < 2) return false;

  if(!(GetRecoObjMass("HNL_SR3", JetColl, AK8_JetColl,leps) < UpperMassSR3WmassCut && GetRecoObjMass("HNL_SR3", JetColl, AK8_JetColl,leps) > LowerMassSR3WmassCut)) return false;

  Fill_RegionPlots(param,"HNL_HighMassSR3_2J_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=1; enne<JetColl.size(); enne++) {

      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();
      if(emme == enne) continue;

      if ( fabs(dijetmass_tmp- M_W) < fabs(dijetmass-M_W) ) {
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
  FillCutflow(HNL_LeptonCore::ChannelDepCR3, w, GetChannelString(channel) +"_CR3", param);

  return true;
  
}



bool HNL_RegionDefinitions::FillHighMassSR2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;

  int NB_JetColl = B_JetColl.size();

  Particle ll    =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-M_Z) < M_ZWINDOW_VETO)) return false;
  if (ll.M() < M_CUT_LL) return false;


  double met2_st = GetMET2ST(leps, JetColl, AK8_JetColl, METv);
  bool PassHMMet = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  if(PassVBF(JetColl,leps,450)) {
    double HT(0.);
    for(UInt_t emme=0; emme<JetColl.size(); emme++){
      HT += JetColl[emme].Pt();
    }
    if (HT/leps[0]->Pt() > 2)  return false;
    
    Fill_RegionPlots(param,"HNL_HighMassCR2_TwoLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
    
  
    double HTLT[2] = { 0., 2};
    FillHist( "ControlSR2/"+param.Name+"/HT_LT1_Central",  HT/leps[0]->Pt(),  w, 1, HTLT, "Reco HT/LT1");
    FillCutflow(HNL_LeptonCore::ChannelDepCR2, w, GetChannelString(channel) +"_CR2",param);
  
    return true;
  }
  return false;
}


bool HNL_RegionDefinitions::FillWZ2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(!HasOSSFPair(leps)) return false;

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
  if(GetIndexNonBestZ(leps,M_ZWINDOW_CR) < 0) return false;
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

  Fill_RegionPlots(param,"HNL_WZ2_ThreeLepton_CR" ,  jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);
  OutCutFlow("HNL_WZ2_ThreeLepton_CR",w);

  return true;

}

bool HNL_RegionDefinitions::FillWZBCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(!HasOSSFPair(leps)) return false;

  if(jets_eta5.size() < 2) return false;

  double metcut = 30.;
  double trilep_masscut=105.;

  std::vector<Jet> JetColl   = SelectAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
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

  if(GetIndexNonBestZ(leps,M_ZWINDOW_CR) < 0) return false;
  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;

  Jet j1 = jets_eta5[ijet1] ;
  Jet j2 = jets_eta5[ijet2];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( METv.Pt() < metcut) return false;
  if (NB_JetColl==0) return false;
  if ((j1+j2).M() < 500.) return false;
  if (maxDiJetDeta<2.5) return false;

  if(!ZmassOSSFWindowCheck(leps,M_ZWINDOW_CR)) return false;


  double Av_JetEta= 0.5*(jets_eta5[ijet1].Eta()+ jets_eta5[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  double zeppenfeld2 = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[2]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  if(zeppenfeld2 > zeppenfeld) zeppenfeld=zeppenfeld2;

  if (zeppenfeld > 1.00) return false;


  Fill_RegionPlots(param,"HNL_WZB_ThreeLepton_CR" ,  jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

  OutCutFlow("HNL_WZB_ThreeLepton_CR",w);

  return true;

}


bool HNL_RegionDefinitions::FillZZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  int sumQ = leps[0]->Charge() + leps[1]->Charge() +leps[2]->Charge() +leps[3]->Charge() ;
  if(sumQ != 0) return false;

  int NB_JetColl=B_JetColl.size();

  if(NB_JetColl  > 0 ) return false;
  bool z_cr_pass(false);

  Particle Z1Cand;
  Particle Z2Cand;

  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      if(IsOSSF(leps[iel], leps[iel2])){
	Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
        int zel1(-9), zel2(-9);
        if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = (*leps[2]) + (*leps[3]);}
        if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = (*leps[1]) + (*leps[3]);}
        if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = (*leps[1]) + (*leps[2]);}
        if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = (*leps[0]) + (*leps[3]);}
        if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = (*leps[0]) + (*leps[2]);}
        if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = (*leps[0]) + (*leps[1]);}
	
	if(IsOSSF(leps[zel1], leps[zel2])){
          if(fabs(Z1Cand.M() - M_Z) < M_ZWINDOW_CR){
	    if(fabs(Z2Cand.M() - M_Z) < M_ZWINDOW_CR){
	      z_cr_pass=true;
	    }
          }
        }
      }
    }
  }
  if(!z_cr_pass)  return false;
  Fill_RegionPlots(param,"HNL_ZZ_FourLepton_CR" ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  OutCutFlow("HNL_ZZ_FourLepton_CR",w);

  return true;
}


bool HNL_RegionDefinitions::FillZZ2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  int sumQ = leps[0]->Charge() + leps[1]->Charge() +leps[2]->Charge() +leps[3]->Charge() ;
  if(sumQ != 0) return false;

  if(B_JetColl.size()  > 0 ) return false;

  //// Select Best Z candidate indices
  pair<int,int> BestZCand = LeptonPairBestZCand(leps);
  if(BestZCand.first == -1) return false;

  Particle Z1Cand =  (*leps[BestZCand.first]) + (*leps[BestZCand.second]);
  vector<int> Z2ind;
  for(unsigned int iel =0; iel < leps.size() ; iel++){
    if(int(iel) == BestZCand.first) continue;
    if(int(iel) == BestZCand.second) continue;
    Z2ind.push_back(iel);
  }
  if(Z2ind.size() != 2) return false;
  if(!IsOSSF(leps[Z2ind[0]],leps[Z2ind[1]])) return false;

  Particle Z2Cand = (*leps[Z2ind[0]]) + (*leps[Z2ind[1]]);
  if(Z2Cand.M() < M_CUT_LL) return false;

  Fill_RegionPlots(param,"HNL_ZZLoose_FourLepton_CR" ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  OutCutFlow("HNL_ZZLoose_FourLepton_CR",w);

  return true;
}


bool HNL_RegionDefinitions::FillZGCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  //if(MCSample.Contains("DY")) return false;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(!HasOSSFPair(leps)) return false;

  double metcut       = 50.;
  int NB_JetColl      = B_JetColl.size();

  if(!ZmasslllWindowCheck(leps,M_ZWINDOW_CR)) return false;
  
  if(HasLowMassOSSF(leps,M_CUT_LL))       return false;

  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_3",   1,  w, 14, 0, 14., "FillZGCRPlots");

  if(ZmassOSSFWindowCheck(leps,M_ZWINDOW_CR)) return false;

  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_5",   1,  w, 14, 0, 14., "FillZGCRPlots");

  if(NB_JetColl > 0)     return false;

  if(METv.Pt() > metcut) return false;

  Fill_RegionPlots(param,"HNL_ZG_ThreeLepton_CR" ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  OutCutFlow("HNL_ZG_ThreeLepton_CR",w);

  return true;

}



bool HNL_RegionDefinitions::FillWGCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(!HasOSSFPair(leps))    return false;
  if(HasLowMassMeson(leps)) return false;

  double metcut = 30.;
  double mt_cut = 30.;

  int NB_JetColl=B_JetColl.size();

  double MassMinOSSF = GetMassMinOSSF(leps);
  if(MassMinOSSF > 4)  return false;
  
  Particle lll = (*leps[0]) + (*leps[1])+ (*leps[2]);
  double MT_lll = M_T(METv,lll);

  if(MT_lll <= mt_cut)  return false;
  if(NB_JetColl > 0)    return false;
  if(METv.Pt() < metcut)return false;

  Fill_RegionPlots(param,"HNL_WG_ThreeLepton_CR"  ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  OutCutFlow("HNL_WG_ThreeLepton_CR",w);

  return true;
}


bool HNL_RegionDefinitions::FillWZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(leps_veto.size() != 3) return false;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if(!HasOSSFPair(leps)) return false;

  double metcut  = 50.;
  double mtcut   = 20.;
  double trilep_masscut=105.;

  int NB_JetColl=B_JetColl.size();
    
  int sum_ch(0);
  for(auto ilep : leps) sum_ch += ilep->Charge();

  if(fabs(sum_ch) != 1) return false;
  
  if(METv.Pt() < metcut) return false;
  
  if(NB_JetColl > 0) return false;
  
  if(GetIndexNonBestZ(leps,M_ZWINDOW_CR) < 0) return false;
  
  if(M_T((*leps[GetIndexNonBestZ(leps,M_ZWINDOW_CR)]), METv)  < mtcut) return false;

  if(HasLowMassOSSF(leps,M_CUT_LL)) return false;

  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;
  
  OutCutFlow("HNL_WZ_ThreeLepton_CR",w);
  Fill_RegionPlots(param,"HNL_WZ_ThreeLepton_CR" ,  JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  
  return true;
}


/*                                                                                                                                                                                                                 CR Functions                                                                                                                                                                                                     -----------------------------------                                                                                                                                                                              * RunAllControlRegions Run ALL following CRs                                                                                                                                                                     ** RunElectronChannelCR                                                                                                                                                                                          ** RunMuonChannelCR                                                                                                                                                                                              ===============================================================                                                                                                                                                  1- FillTopCRPlots                                                                                                                                                                                              %% 2 leptons (OS/SS) + MET > 50 + Nb > 0 +                                                                                                                                                                         %% -validate B tagging eff + SF                                                                                                                                                                                  ===============================================================                                                                                                                                                  2- FillZNPCRPlots                                                                                                                                                                                                %% 3Lep +  MET < 30 + MZ  + MTnonZlep < 30                                                                                                                                                                       %% Validate Fakes                                                                                                                                                                                                ===============================================================y                                                                                                                                                 3- FillZCRPlots                                                                                                                                                                                                %% 2 Lep + MET < 30 + Nb(0) + NAK8(>0) + MZ                                                                                                                                                                        %% Check in SR1 like region with low met and 0 b jet                                                                                                                                                             ===============================================================                                                                                                                                                  4- FillWWCR1Plots                                                                                                                                                                                              %% SSlep +  VBFJ(2) + MJJ(>500) + JPt(30) + Nb(0) JJEta(>2.5) + Zepp(<0.75) + MZ + llDphi(<2.)                                                                                                                   %% - region frmo Peking, SR w/Reverse dphi cut                                                                                                                                                                     ===============================================================                                                                                                                                                  5- FillWWCR2Plots                                                                                                                                                                                              %% SSlep +  VBFJ(2) + MJJ(>500) + JPt(30) + Nb(0) JJEta(>2.5) + Zepp(<0.75) + MZ + MET2ST(>15)                                                                                                                     %% - SR2 BUT with HIgh met                                                                                                                                                                                       ===============================================================                                                                                                                                                  6- FillWWCRNPPlots                                                                                                                                                                                             %% SSlep +  VBFJ(2) + MET > 30 + Nb(>0) + JJEta(>2.5) + MZ +  MJJ(>500) same as AN2020_045 Table 15 Nonprompt                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
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

