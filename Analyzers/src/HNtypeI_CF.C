#include "HNtypeI_CF.h"

void HNtypeI_CF::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  HEM1516 = HasFlag("HEM1516");


  rand_ = new TRandom3(1234);


  cout << "[HNtypeI_CF::initializeAnalyzer()] RunFake = " << RunFake << endl;
  cout << "[HNtypeI_CF::initializeAnalyzer()] RunCF = " << RunCF << endl;
  cout << "[HNtypeI_CF::initializeAnalyzer()] RunSyst = " << RunSyst << endl;
  cout << "[HNtypeI_CFr::initializeAnalyzer()] PromptLeptonOnly = " << PromptLeptonOnly << endl;


  MuonVetoIDs      = {"HNVeto2016", "POGLoose"};


  HighPtMuonTriggers.clear();
  MuonTriggers.clear();
  MuonTriggersH.clear();
  ElectronTriggers.clear();
  SingleElectronTriggers.clear();
  EMuTriggers.clear();
  EMuTriggersH.clear();
  
  if(DataYear==2016){                                                                   // Lumi values for trigger weight (/pb)
    ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");          // 35918.219492947

    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }
  else if(DataYear==2017){
    ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");

    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }
  else if(DataYear==2018){
    ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
    
    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }

  //==== b tagging
  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  mcCorr->SetJetTaggingParameters(jtps);


  //==== PUReweight
  if(!IsDATA){
    TString datapath = getenv("DATA_DIR");

    TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_"+TString::Itoa(DataYear,10)+".root";
    TString PUhname = "PUReweight_"+TString::Itoa(DataYear,10);
    //==== Only for 2016 FastSim, use different one.
    if(DataYear==2016 && IsFastSim){
      PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
    }

    cout << "[HNtypeI_CF::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNtypeI_CF::initializeAnalyzer()] PUhname = " << PUhname << endl;
    cout << "[HNtypeI_CF::initializeAnalyzer()] Signal = " << Signal << endl;
    cout << "[HNtypeI_CF::initializeAnalyzer()] SignalOS = " << SignalOS << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void HNtypeI_CF::executeEvent(){

  
  //************************************************
  //-----------------------------------------------
  //-----------------------------------------------
  // Initial checks/functions that run before analyzer 
  //----------------------------------------------- 
  //-----------------------------------------------     
  //************************************************                                                                                                                            

  
  //==========================
  //==== Gen for genmatching
  //==========================

  if(!IsData)gens = GetGens();

  
  //************************************************///
  //-----------------------------------------------////
  //-----------------------------------------------////
  // Prefire weight
  //-----------------------------------------------////
  //-----------------------------------------------////
  //************************************************///   

  weight_Prefire      = GetPrefireWeight( 0);
  weight_Prefire_Up   = GetPrefireWeight(+1);
  weight_Prefire_Down = GetPrefireWeight(-1);

  //************************************************///
  //-----------------------------------------------////
  //-----------------------------------------------////
  // NVTX setup
  //-----------------------------------------------////
  //-----------------------------------------------////
  //************************************************///
  N_VTX = nPV;
  if(!IsDATA){
    //==== nPileUp starts from 0
    N_VTX = nPileUp+1;
    //==== but histogram is as a function of nPileUp not nVTX
    int nPileUpForBin = nPileUp;
    if(DataYear==2018) nPileUpForBin = max(5, nPileUpForBin);
    int bin_pu = hist_PUReweight->FindBin(nPileUpForBin);
    weight_PU = hist_PUReweight->GetBinContent(bin_pu);

    weight_PU_Up = hist_PUReweight_Up->GetBinContent(bin_pu);
    weight_PU_Down = hist_PUReweight_Down->GetBinContent(bin_pu);

  }


  //************************************************/// 
  // setup vector of all objects
  //************************************************///
  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllJets = GetAllJets();
  AllFatJets = puppiCorr->Correct(GetAllFatJets());

  
  //************************************************///
  // make signal specifc plots
  //************************************************///
  if(Signal)MakeSignalPlots();


  //========================
  //==== AnalyzerParameter
  //========================

  //==== Central

  AnalyzerParameter param;


  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ELIDs;
  vector<TString> channel;
  vector<TString>  mu_loose_id;
  vector<TString>  el_loose_id;

  ELIDs.push_back(make_pair("passPOGTight_TTrig_HNTC","HNVeto2016"));       el_loose_id.push_back("HNLooseV2");
  ELIDs.push_back(make_pair("passMVAID_noIso_WP90V16","HNVeto2016"));  el_loose_id.push_back("passMVAID_noIso_WP90LooseV2");                               \

  ELIDs.push_back(make_pair("passPOGTightv1_TTrig_HNTC","HNVeto2016"));  el_loose_id.push_back("HNLoose");
  ELIDs.push_back(make_pair("passPOGTightv2_TTrig_HNTC","HNVeto2016"));  el_loose_id.push_back("HNLoose");
  ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_4","HNVeto2016"));  el_loose_id.push_back("HNLooseV3");
  ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip4_4","HNVeto2016"));  el_loose_id.push_back("HNLooseV4");


  /*
  ELIDs.push_back(make_pair("HEEPv7", "HNVeto"));
  el_loose_id.push_back("HEEPLoose");

  ELIDs.push_back(make_pair("HEEP2018", "HNVeto"));
  el_loose_id.push_back("HEEPLoose");

  ELIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
  el_loose_id.push_back("HNLoose2016");
  
  ELIDs.push_back(make_pair("HNTightV1", "HNVeto"));
  el_loose_id.push_back("HNLoose");
  
  ELIDs.push_back(make_pair("HNTightV2", "HNVeto"));
  el_loose_id.push_back("HNLoose");
  
  ELIDs.push_back(make_pair("HNTightV3", "HNVeto"));
  el_loose_id.push_back("HNLoose");
  
  ELIDs.push_back(make_pair("HNTightV4", "HNVeto"));
  el_loose_id.push_back("HNLoose");

  ELIDs.push_back(make_pair("HNMediumV1", "HNVeto"));
  el_loose_id.push_back("HNLoose");
  
  ELIDs.push_back(make_pair("HNMediumV2", "HNVeto"));
  el_loose_id.push_back("HNLoose");
  
  ELIDs.push_back(make_pair("HNMediumV3", "HNVeto"));
  el_loose_id.push_back("HNLoose");
  
  
  
  // TIGHT POG + no CC in low pt
  ELIDs.push_back(make_pair("passTightID_nocc", "HNVeto"));
  el_loose_id.push_back("passLooseID_nocc");
  
  // POG + IP + CC
  ELIDs.push_back(make_pair("passTightID", "HNVeto"));
  el_loose_id.push_back("passLooseID");
  
  ELIDs.push_back(make_pair("passMediumID", "HNVeto"));
  el_loose_id.push_back("passLooseID");
  
  ELIDs.push_back(make_pair("passMVAID_noIso_WP80","HNVeto2016"));
  el_loose_id.push_back("passMVAID_noIso_WP80Loose");
  
  ELIDs.push_back(make_pair("passMVAID_noIso_WP90","HNVeto2016"));
  el_loose_id.push_back("passMVAID_noIso_WP90Loose");
  
  ELIDs.push_back(make_pair("passMVAID_iso_WP80","HNVeto2016"));
  el_loose_id.push_back("passMVAID_iso_WP80Loose");
  
  ELIDs.push_back(make_pair("passMVAID_iso_WP90","HNVeto2016"));
  el_loose_id.push_back("passMVAID_iso_WP90Loose");
  */
  
  for (auto i: ELIDs) { 
    channel.push_back("EE");
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");
  }
  for(unsigned int it_id=0; it_id<ELIDs.size(); it_id++){
    
    //************************************************///
    // setup leptton veto/loose/tight 
    //************************************************///

    TString MuonTightID      = MuIDs[it_id].first;
    TString MuonVetoID      = MuIDs[it_id].second;
    TString MuonLooseID  =  mu_loose_id[it_id];
    TString ElectronTightID = ELIDs[it_id].first;
    TString ElectronLooseID = el_loose_id[it_id];
    TString ElectronVetoID  = ELIDs[it_id].second;

    TString FakeRateID =  ElectronLooseID;
    if(channel[it_id] == "MuMu") FakeRateID = MuonLooseID;
    param.Clear();


    TString Tight_ID = ElectronTightID;
    if(channel[it_id] == "MuMu") Tight_ID = MuonTightID;

  
    param.syst_ = AnalyzerParameter::Central;
    param.Name = "HNtypeI_CF_"+ElectronTightID;
    param.MCCorrrectionIgnoreNoHist = false;

    
    //==== Muon ID
    param.Muon_Tight_ID        = MuonTightID;
    param.Muon_Loose_ID        = MuonLooseID;
    param.Muon_Veto_ID        = MuonVetoID;
    param.Muon_FR_ID          = MuonLooseID;     // ID name in histmap_Electron.txt
    param.Muon_FR_Key         = MuonTightID+"_AwayJetPt40";  // histname                                                                              

    param.Muon_MinPt = 10.;
    param.Muon_UsePtCone      = true;

    //==== Electron ID
    param.Electron_Tight_ID       = ElectronTightID;
    param.Electron_Loose_ID       = ElectronLooseID;
    param.Electron_Veto_ID        = ElectronVetoID;
    param.Electron_FR_ID          = ElectronLooseID;     // ID name in histmap_Electron.txt
    param.Electron_FR_Key         = ElectronTightID+"_AwayJetPt40";  // histname
    param.Electron_MinPt = 15.;
    //param.Electron_ID_SF_Key      = "passTightID";
    param.Electron_ID_SF_Key      = "";
    param.Electron_Trigger_SF_Key = "";
    param.Electron_UsePtCone      = true;

    //==== Jet ID
    param.Jet_ID    = "HNTight";
    param.FatJet_ID = "HNTight";


    //************************************************///
    //
    // have list of IP cuts to modify ID
    //
    //************************************************///

    //************************************************///
    // change param name to link to IP/CC cuts
    //************************************************///
    param.Name = "HNtypeI_CF_"+channel[it_id] + "_" +Tight_ID;
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  } 
}

void HNtypeI_CF::executeEventFromParameter(AnalyzerParameter param, TString El_ID, TString channel){
  

  bool ee = true;
 

  // setup event level objects
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight = 1.;
  //************************************************///
  
  //  if(Signal) weight_norm_1invpb=1.;
  
  //************************************************///
  // MC weights initial settings
  // -- set to full trigger luminosity for data taking year
  // -- apply MC gen weights
  // -- apply prefire weight
  //************************************************///
  if(!IsDATA){
    
    
    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();

    //==== Apply Prefire
    weight *= weight_Prefire;

  }

  //************************************************///  
  //==== No Cut cutflow entry, get nevents in sample
  //************************************************///   


  //bool PassSingleElectronTrig = ev.PassTrigger(SingleElectronTriggers);


  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon>     this_AllMuons = AllMuons;
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  //************************************************///   
  // setup systematic variables
  //************************************************///   
  int SystDir_ElectronIDSF(0);
  int SystDir_ElectronRecoSF(0);

  //************************************************///   
  //==== PU reweight for MC
  //************************************************///   

  double weight_pu_up = weight;
  double weight_pu_down = weight;
  if(!IsDATA){

    double this_pureweight = 1.;
    double this_pureweight_up = 1.;
    double this_pureweight_down = 1.;
    this_pureweight = weight_PU;
    this_pureweight = weight_PU_Down;
    this_pureweight = weight_PU_Up;

    weight *= this_pureweight;
    weight_pu_up *= this_pureweight_up;
    weight_pu_down *= this_pureweight_down;
  }
  
  //************************************************///   
  // 
  // setup analysis level lepton vectors
  //
  //************************************************///   
  
  TString elID =  param.Electron_Tight_ID;
  TString muID =  param.Muon_Tight_ID;

  /// Get collecton of temporary leptons
  std::vector<Electron>  myelectrons = SelectElectrons(this_AllElectrons, elID, 10., 2.5);
  std::vector<Muon>     mymuons     = SelectMuons    (this_AllMuons,   muID, 10., 2.4);


  std::vector<Electron> tmpveto_electrons = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon> tmpveto_muons= SelectMuons(this_AllMuons, param.Muon_Veto_ID, 5., 2.4);

  std::vector<Muon>     veto_muons     = MuonPromptOnly(tmpveto_muons, gens);
  std::vector<Electron>     veto_electrons = ElectronPromptOnly(tmpveto_electrons, gens);

 
  std::sort(myelectrons.begin(), myelectrons.end(), PtComparing);
  std::sort(mymuons.begin(), mymuons.end(), PtComparing);
  std::sort(veto_muons.begin(), veto_muons.end(), PtComparing);
  std::sort(veto_electrons.begin(),veto_electrons.end(), PtComparing);

  bool PassDoubleElectronTrig = ev.PassTrigger(ElectronTriggers);
  if(!(PassDoubleElectronTrig)) return;

  for(unsigned int iel=0; iel< myelectrons.size(); iel++){

    double ptbins[10] = { 10., 15.,20.,30.,40.,50.,  60., 200.,400., 2000.};
    double etabins2[5] = { 0.,0.8,  1.479, 2.,  2.5};
    
    FillHist(("el_pt_eta"+param.Electron_Tight_ID).Data(), myelectrons[iel].Pt(), fabs(myelectrons[iel].Eta()),  weight, 9, ptbins, 4 , etabins2);
    if(IsCF(myelectrons[iel],gens))       FillHist(("el_cf_pt_eta"+param.Electron_Tight_ID).Data(), myelectrons[iel].Pt(), fabs(myelectrons[iel].Eta()),  weight, 9, ptbins, 4 , etabins2);

    if(fabs(myelectrons[iel].Eta()) < 1.5 ) {
      FillHist( "el_barrel_pt"+param.Electron_Tight_ID, myelectrons[iel].Pt(), weight, 9 ,ptbins,"pt");
      if(IsCF(myelectrons[iel],gens))   FillHist( "el_cf_barrel_pt"+param.Electron_Tight_ID, myelectrons[iel].Pt(), weight, 9 ,ptbins,"pt");
    }
    else{
      FillHist( "el_endcap_pt"+param.Electron_Tight_ID, myelectrons[iel].Pt(), weight, 9 ,ptbins,"pt");
      if(IsCF(myelectrons[iel], gens))      FillHist( "el_cf_endcap_pt"+param.Electron_Tight_ID, myelectrons[iel].Pt(), weight, 9 ,ptbins,"pt");

    }

  }
  return;

}



bool HNtypeI_CF::PassHNID(TString TightID, Electron el){

  
  bool pass=true;
  if(!el.PassID(TightID) ) pass=false;
  
  if (TightID == "passTightID" || TightID == "passMediumID" ||   TightID == "passMVAID_iso_WP90" ||   TightID == "passMVAID_iso_WP80" ){
    if (! (el.IsGsfCtfScPixChargeConsistent() &&   el.Pass_TriggerEmulation()))  pass=false;
	
    if( fabs(el.scEta()) <= 1.479 ){
      if( (fabs(el.dXY())<0.05 && fabs(el.dZ())<0.1) )  pass=false;
    }
    else{
      if( !(fabs(el.dXY())<0.1 && fabs(el.dZ())<0.2) )  pass=false;
    }
  }// ID

  if (TightID == "passTightID_nocc" ){
    if (! el.Pass_TriggerEmulation())  pass=false;
    
    if( fabs(el.scEta()) <= 1.479 ){
      if( (fabs(el.dXY())<0.05 && fabs(el.dZ())<0.1) )  pass=false;
    }
    else{
      if( !(fabs(el.dXY())<0.1 && fabs(el.dZ())<0.2) )  pass=false;
    }
  }// ID     
  if (TightID == "passTightID_noccb" ){
    if (! el.Pass_TriggerEmulation())  pass=false;

    if( fabs(el.scEta()) <= 1.479 ){
      if( (fabs(el.dXY())<0.05 && fabs(el.dZ())<0.1) )  pass=false;
    }
    else{
      if (!el.IsGsfCtfScPixChargeConsistent())  pass=false;
      if( !(fabs(el.dXY())<0.1 && fabs(el.dZ())<0.2) )  pass=false;
    }
  }// ID  
  return pass;



}

void HNtypeI_CF::RunMM(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){
  
  TString label = param.Name;
  TString channel_s="_MuMu";
  Particle METv = ev.GetMETVector();
  int i_channel=1;

                                                                                                                                                                                  

  FillEventCutflow(0,i_channel,w, "NoCut",param.Name);

  FillCutFlow(true, "CutFlow", "NoCut_"+param.Name, w);
  FillCutFlow(true, "CutFlow", "NoCut_no_weight_"+param.Name, 1.);

  FillEventCutflow(0,i_channel,w, "METFilter",param.Name);

  FillCutFlow(true, "CutFlow", "METFilter_"+param.Name, w);


  bool PassDoubleMuonTrig = ev.PassTrigger(MuonTriggers);
  bool PassHighPtMuonTrig = ev.PassTrigger(HighPtMuonTriggers);
  // if singlemuon dataset and passes double mu trigger veto
  if(isSingleMu && PassDoubleMuonTrig)  return;
  if(isSingleMu && !PassHighPtMuonTrig) return;
  // if MC OR double and Mu50
  if(!IsData && !(PassDoubleMuonTrig|| PassHighPtMuonTrig)) return;
  
  // if Data and doublemuon dataset use double triggers only
  if(IsData &&!isSingleMu && !(PassDoubleMuonTrig)) return;


  if(!IsData){
    if(DataYear==2016){

      double dimu_trig_weight=0.;
      if(ev.PassTrigger(HighPtMuonTriggers)|| ev.PassTrigger(MuonTriggers)) dimu_trig_weight += 27267.591;
      if(ev.PassTrigger(HighPtMuonTriggers)||ev.PassTrigger(MuonTriggersH)) dimu_trig_weight += 8650.628;

      w *= dimu_trig_weight / ev.GetTriggerLumi("Full");
    }
  }
  
  FillEventCutflow(0,i_channel,w, "Trigger_mu",param.Name);

  FillHist( "MMNObj/nelectrons"+label+channel_s, electrons.size() ,w, 5, 0., 5,"N_{electron}");
  FillHist( "MMNObj/nmuons"+label  +channel_s  , muons.size()   ,w, 5, 0., 5,"N_{muon}");
  FillHist( "MMNObj/nelectrons_veto"+label+channel_s, electrons_veto.size() ,w, 5, 0., 5,"N_{electron}");


  //************************************************///
  // ZZ CR plots                                                                                                                                                                                               
  //************************************************///                                                                                                                                                                                                                                                                                                                                                                         
  int NBJets=GetNBJets();


  //************************************************///                                                                                                                                                    
  //  prompt lepton CRs
  //************************************************///                                                                                                                                                                                      
  FillZZCRPlots(i_channel, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, w);
  FillWZCRPlots(i_channel, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, w);

  // Dimuon selection 
  
  if(muons.size()!=2)  return;
  if(muons[0].Pt() < 20.) return;
  if(muons[1].Pt() < 10.) return;  

  if(RunFake) w*= GetFakeWeightMuon(muons, param.Muon_Tight_ID);

  // setup sting to separate SS/OS
  TString charge_s= (muons[0].Charge() ==  muons[1].Charge() )  ?  "same_sign" : "opposite_sign";
  int charge_i = (muons[0].Charge() ==  muons[1].Charge() )  ?  1 : -1;

  if(SignalOS) charge_s= "opposite_sign";

  FillEventCutflow(charge_i,i_channel, w, "Dilep_mu_"+charge_s,param.Name);
  
  // lepton veto
  if(muons_veto.size()!=2) return;
  if(electrons_veto.size() > 0)  return;
  FillEventCutflow(charge_i,i_channel,w, "Lep_veto_mu_"+charge_s,param.Name);


  //************************************************///                                                                                                                                                      
  Particle llCand = muons.at(0) + muons.at(1);

  //************************************************///                                                                                                                          
  
  if (llCand.M() < 10.) return;
  FillEventCutflow(charge_i,i_channel,w, "Dimu_mass_"+charge_s,param.Name);

  //************************************************///                                                                                                                                                                                                                                                                                                                                  
  TString signal_region1 = (charge_s=="same_sign")  ?  "SR1" : "SR3";
  TString signal_region2 = (charge_s=="same_sign")  ?  "SR2" : "SR4";

  
  int njets = jets.size() + fatjets.size();
  if(njets == 0 ) {
    FillRegionPlots(i_channel,true,"zerojet"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    return;
  }


  FillEventCutflow(charge_i,i_channel,w, "Jet_req_"+charge_s,param.Name);
  

  if(llCand.M() < 101. && llCand.M() > 81.)   FillRegionPlots(i_channel,true,"DYCR"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
  
  TString ev_type="";
  if(RunFake) {
    bool el1_tight=muons[0].PassID(param.Muon_Tight_ID);
    bool el2_tight=muons[1].PassID(param.Muon_Tight_ID);
    if(el1_tight&&el2_tight)ev_type = "TT_";
    if(el1_tight&&!el2_tight)ev_type = "TL_";
    if(!el1_tight&&el2_tight)ev_type = "LT_";
    if(!el1_tight&&!el2_tight)ev_type = "LL_";
    FillRegionPlots(i_channel,true,"presel"+channel_s+"_"+ev_type+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
  }
  
  FillRegionPlots(i_channel,true,"presel"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
  
  ////////////////////////////////////////
  ////////////////////////////////////////
  // SR selections                        
  ////////////////////////////////////////
  ////////////////////////////////////////                                                                                                                                                               
  
  double ST = GetST(electrons, muons, jets, fatjets, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  
  if(met2_st < 15)          FillEventCutflow(charge_i,i_channel,w, "MET_"+charge_s,param.Name);
  
  
  // require low MET and 0 bjets
  if(met2_st < 15 && NBJets == 0 ){
    FillEventCutflow(charge_i,i_channel,w, "BJet_"+charge_s,param.Name);
    
    if(njets == 1 && fatjets.size() == 0) {
      FillRegionPlots(i_channel,true,"singlejet"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    }
    
    // SR1/3 with 0 AK8 jets
    if( jets.size() > 1 && fatjets.size() == 0) {
      
      // SR1/3  MM                                                                   
      FillEventCutflow(charge_i,i_channel,w, "HighMassJet_"+charge_s,param.Name); \
      
      
      // make MJJ mass cut in W wondow
      if(GetMass(signal_region1, jets, fatjets) < 150. && GetMass(signal_region1, jets, fatjets) > 30.){
	
	FillEventCutflow(charge_i,i_channel,w, "HighMassSR_"+charge_s,param.Name);          
	FillRegionPlots(i_channel,true,signal_region1+"_Highmass" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	
	FillAllSignalRegion1(i_channel, signal_region1, IsDATA,  charge_s, label, jets, fatjets,  electrons, muons,  METv, nPV ,w );
	
	FillEventCutflow(charge_i,i_channel,w, "Highmass_"+signal_region1,param.Name);
	
	
	
	
      } // mass req
      
      
    } // jet req for SR2/4
    else if(fatjets.size() > 0) {
      // SR2/4  MM

      
      FillEventCutflow(charge_i,i_channel,w, "HighMassJet_"+charge_s,param.Name);             
      // make mass window cut on fatjet
      if(GetMass(signal_region2,jets, fatjets) <150. &&GetMass(signal_region2,jets, fatjets) > 50.){


	// fill general high mass plots
	FillRegionPlots(i_channel,true,signal_region2 + "_Highmass", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);

	// fill all mass binned plots 
	FillAllSignalRegion2(i_channel, signal_region2, IsDATA,  charge_s, label, jets, fatjets,  electrons, muons,  METv, nPV  ,w);
	FillEventCutflow(charge_i,i_channel,w, "Highmass_"+signal_region2,param.Name);
      } // end of mass req
    }// 0 AK8
  } // end met/nbjet                                                                                       
  else{
    // SR1/3 with 0 AK8 jets                                                                                                    
    if( jets.size() > 1 && fatjets.size() == 0) FillRegionPlots(i_channel,true,signal_region1 + "_HighmassCR", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    
    if(fatjets.size() > 0 )FillRegionPlots(i_channel,true,signal_region2 + "_HighmassCR", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    

  } // end else of jet req
  return;
  
} /// end function                      




void HNtypeI_CF::FillAllSignalRegion1(int i_channel, TString signal_region1, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,   Particle _MET,int _npv  , double w     ){

  vector<pair<int, pair<TString, double> > >  systs = GetSyst(i_channel, isdata);
 
  for(unsigned int isys =0; isys < systs.size(); isys++){

    if (i_channel == 0){

      // EE SR1/3
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 + "_highmass","", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.);


      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 25., 15., 50.,120., 120., 50., 110., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn125", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 30., 25., 50.,120., 140., 90., 140., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 55., 40., 50.,120., 220.,160., 225.,  6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 70., 60., 50.,120., 310.,220., 270.,  6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 80., 60., 50.,120., 370.,235., 335.,  6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 100., 65., 50.,120., 450.,335., 450.,  6.);


      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 65., 50.,120., 560.,400., 555., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 690., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 966., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1130., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1300., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1490., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1490., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1600., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);

    }
    if (i_channel == 1){
      
      /// MM plots
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 + "_highmass","", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.);
      
      // CC limits                                                                                                                                                                                                                                                                                                                                             
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn100", label, jets,  fatjets ,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 25., 15., 50.,120., 110., 55., 115., 9.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn125", label, jets,  fatjets ,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 25., 25., 50.,120., 140., 85., 140., 7.);      
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 50., 40., 50.,120., 250.,160., 215.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 85., 45., 50.,120., 310.,215., 270.,  7.);      
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 100., 50., 50.,120., 370.,225., 340.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 110., 60., 50.,120., 490.,296., 490.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 60., 50.,120., 610.,370., 550., 7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 680.,370., 630.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 885.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 890.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1225.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1230.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 1245., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1690.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1890.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1940.,  7.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2220., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2220., 6.);
      FillSigRegionPlots1(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2220., 6.);

    } // MM
    
  }// end of syst
}

void HNtypeI_CF::FillAllSignalRegion2(int i_channel, TString signal_region2, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons, Particle _MET,int _npv  , double w){
  

  // get loop of systs + one nominal                                                                                                                                                                                                                                                                                                                         
  vector<pair<int, pair<TString, double> > >  systs = GetSyst(i_channel, isdata);
  for(unsigned int isys =0; isys < systs.size(); isys++){

    // fill an inclusive high mass selection                                                                                                                                                                                                                                                                                                                 
    FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 + "_highmass","", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w,  9999.,  9999.,  9999.,  9999.,  9999.,  9999.,  9999.);


    if(i_channel == 0){
      // EE SR2/4
       FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 25.,15., 40.,130., 90., 220., 15.);
       FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn125", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 60.,15., 40.,130., 123., 145., 15.);////////////////
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 20., 40., 130., 173., 220., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 25., 40., 130., 220., 305., 15.);//////////////
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 30., 40., 130., 270, 330., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 35., 40., 130., 330., 440., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 120., 35., 40., 130., 440., 565., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 120., 0., 40., 130., 565., 675., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 635., 775., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 740., 1005., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 865., 1030., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 890., 1185., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1035., 1395., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1085., 1460., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1140., 1590., 15.);///////////
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1245., 1700., 15.);///////
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1300., 1800., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1300., 1800., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1300., 1800., 15.);

    }
    if(i_channel == 1){
      // fill mass binned selections                                                                                                                                                                                                                                                                                                                           
      // MM SR2/4
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 25.,  15., 40.,130., 98., 145., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn125", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 60.,  15., 40.,130., 110., 150., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 20., 40., 130., 175., 235., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 25., 40., 130., 226., 280., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 20., 40., 130., 280., 340., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 65., 40., 130., 340., 445., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 65., 40., 130., 445., 560., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 560., 685., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 635., 825., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 755., 960., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 840., 1055., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 900., 1205., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 990., 1250., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1035., 1430., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1110., 1595., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1285., 1700., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1330., 1800., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1330., 1800., 15.);
      FillSigRegionPlots2(i_channel,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1330., 1800., 15.);
    }
  } // end of syst                   
    

}
vector<pair<int, pair<TString, double> > >  HNtypeI_CF::GetSyst(int i_channel, bool isdata){

  vector<pair<int, pair<TString, double> > >   systs;
  systs.push_back(make_pair(0,make_pair("",0.)));

  if(!isdata){
    systs.push_back(make_pair(1,make_pair("JESup",0.9)));
    systs.push_back(make_pair(-1,make_pair("JESdown",0.9)));
    systs.push_back(make_pair(1,make_pair("JERup",0.92)));
    systs.push_back(make_pair(-1,make_pair("JERdown",0.92)));
    systs.push_back(make_pair(1,make_pair("MUIDup",0.97)));
    systs.push_back(make_pair(-1,make_pair("MUIDdown",0.97)));
  }

  return systs;
}

int HNtypeI_CF::GetNBJets(){

  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jets   = SelectJets(this_AllJets,"HNTight", 20., 2.5);

  int NBJets=0;

  for(unsigned int i=0; i<jets.size(); i++){

    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium,                                                    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  return NBJets;
}

void HNtypeI_CF::RunEE(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){


  TString channel_s="_EE";
  int i_channel=0;
  TString label = param.Name;
  Particle METv = ev.GetMETVector();
  
  FillEventCutflow(0,i_channel,w, "NoCut",param.Name);

  FillCutFlow(true, "CutFlow", "NoCut_"+param.Name, w);
  FillCutFlow(true, "CutFlow", "NoCut_no_weight_"+param.Name, 1.);


  if(!PassMETFilter()) return;
  FillEventCutflow(0,i_channel,w, "METFilter",param.Name);
  
  FillCutFlow(true, "CutFlow", "METFilter_"+param.Name, w);


  
  bool PassDoubleElectronTrig = ev.PassTrigger(ElectronTriggers);  
  if(!(PassDoubleElectronTrig)) return;

 
  /// 0 = SS+OS filling
  FillEventCutflow(0,i_channel,w, "Trigger_el",param.Name);
  

  int NBJets=GetNBJets();



  FillHist( "NObj/nelectrons"+label+channel_s, electrons.size() ,w, 5, 0., 5,"N_{electron}");
  FillHist( "NObj/nmuons"+label+channel_s    , muons.size()   ,w, 5, 0., 5,"N_{muon}");
  FillHist( "NObj/nelectrons_veto"+label+channel_s, electrons_veto.size() ,w, 5, 0., 5,"N_{electron}");
  

  //************************************************///   
  // Fill Prompt CR plots
  //************************************************///   
  FillZZCRPlots(i_channel, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, w);
  FillWZCRPlots(i_channel, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, w);


  //************************************************///    
  //************************************************///   
  //************************************************///   
  // Dilepton event selection 
  //************************************************///   
  //************************************************///   
  //************************************************///   
  
  if(electrons.size()!=2)  return;
  if(electrons[0].Pt() < 25.) return;
  if(electrons[1].Pt() < 15.) return;

  if(RunFake) w*= GetFakeWeightElectron(electrons,param.Electron_Tight_ID);

  //************************************************/// 
  // charge_s is string to label ++ vs +-           
  //************************************************/// 
  
  TString charge_s= (electrons[0].Charge() ==  electrons[1].Charge() )  ?  "same_sign" : "opposite_sign";

  if(SignalOS) charge_s= "opposite_sign";

  
  int charge_i = (electrons[0].Charge() ==  electrons[1].Charge() )  ?  1 : -1;
  FillEventCutflow(charge_i,i_channel,w, "Dilep_el_"+charge_s,param.Name);

  if(electrons_veto.size()!=2) return;
  if(muons_veto.size() > 0)  return;
  FillEventCutflow(charge_i,i_channel,w, "Lep_veto_el_"+charge_s,param.Name);

  /*
  if(charge_s=="same_sign"){
    
    if(param.Electron_Tight_ID == "HNTight2016") {
      FilAllElectronPlots("SS"+param.Name, param.Electron_Veto_ID, electrons_veto,w);
    }
    FilAllElectronPlots("SS"+param.Name, param.Electron_Tight_ID, electrons,w);
  }
  */
  //************************************************///   
  // create proxy for Z/W candidates
  //************************************************///   
  Particle llCand = electrons.at(0) + electrons.at(1);
  
  if(llCand.M() < 10) return;
  FillEventCutflow(charge_i,i_channel,w, "Diel_mass_"+charge_s,param.Name);

  TString signal_region1 = (charge_s=="same_sign")  ?  "SR1" : "SR3";
  TString signal_region2 = (charge_s=="same_sign")  ?  "SR2" : "SR4";


  //************************************************///   
  // select Z peak
  //************************************************///   
  int njets = jets.size() + fatjets.size();
  if(njets == 0 ) {
    FillRegionPlots(i_channel,true,"zerojet"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    return;
  }


  FillEventCutflow(charge_i,i_channel,w, "Jet_req_"+charge_s,param.Name);

  if(llCand.M() < 101. && llCand.M() > 81.)   FillRegionPlots(i_channel,true,"DYCR"+channel_s+"_" +charge_s, label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
  
  else {

    FillEventCutflow(charge_i,i_channel,w, "noZ_"+charge_s,param.Name);

    TString ev_type="";
    if(RunFake) {
      bool el1_tight=PassHNID(param.Electron_Tight_ID, electrons[0]);
      bool el2_tight=PassHNID(param.Electron_Tight_ID, electrons[1]);
      if(el1_tight&&el2_tight)ev_type = "TT_";
      if(el1_tight&&!el2_tight)ev_type = "TL_";
      if(!el1_tight&&el2_tight)ev_type = "LT_";
      if(!el1_tight&&!el2_tight)ev_type = "LL_";
      FillRegionPlots(i_channel,true,"presel"+channel_s+"_"+ev_type+"_"+charge_s, label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    }

    FillRegionPlots(i_channel,true,"presel"+channel_s+"_"+charge_s, label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    
    
    //************************************************///   
    // SR selections
    //************************************************///   
    double ST = GetST(electrons, muons, jets, fatjets, ev);
    double met2_st = pow(METv.Pt(),2.)/ ST;


    if(met2_st < 15)FillEventCutflow(charge_i,i_channel,w, "MET_"+charge_s,param.Name);


    // MET + Bjet req
    if(met2_st < 15 && NBJets == 0 ){

      FillEventCutflow(charge_i,i_channel,w, "BJet_"+charge_s,param.Name);
      
      if(njets == 1 && fatjets.size() == 0) {
	FillRegionPlots(i_channel,true,"singlejet"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
      }
      // Jet Req
      if( jets.size() > 1 && fatjets.size() == 0) {  
        FillEventCutflow(charge_i,i_channel,w, "HighMassJet_"+charge_s,param.Name);

	if(GetMass(signal_region1, jets, fatjets) < 150. && GetMass(signal_region1, jets, fatjets) > 50.){
	  FillEventCutflow(charge_i,i_channel,w, "HighMassSR_"+charge_s,param.Name);
	  

	  //// SR1 
	  FillRegionPlots(i_channel,true,signal_region1 + "_Highmass", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	  FillAllSignalRegion1(i_channel, signal_region1, IsDATA,  charge_s, label, jets, fatjets,  electrons, muons,  METv, nPV ,w );
	  FillEventCutflow(charge_i,i_channel,w, "Highmass_"+signal_region1,param.Name);

	} // mass req        
	
      } //jet req
      
     // MET BJet
      else if(fatjets.size() > 0) {
        FillEventCutflow(charge_i,i_channel,w, "HighMassJet_"+charge_s,param.Name);
	
	if(GetMass(signal_region2,jets, fatjets) <130. &&GetMass(signal_region2,jets, fatjets) > 40.){
	  
	  FillEventCutflow(charge_i,i_channel,w, "HighMassSR_"+charge_s,param.Name);
	  
	  // fill all mass binned plots                                                                                                                                                                                                               
	  //// SR2                                                                                                                                                                                                                                
	  FillRegionPlots(i_channel,true,signal_region2 + "_Highmass", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	  FillAllSignalRegion2(i_channel, signal_region2, IsDATA,  charge_s, label, jets, fatjets,  electrons, muons,  METv,  nPV ,w );
	  FillEventCutflow(charge_i,i_channel,w, "Highmass_"+signal_region2,param.Name);
	}// J mass
      } // 0 AK8
    } // end met/nbjet
    else{

      if(fatjets.size() > 0)FillRegionPlots(i_channel,true,signal_region2 +"_HighmassCR", label, jets,  fatjets, electrons, muons,  METv, nPV, w);
      else if(jets.size()>1) FillRegionPlots(i_channel,true,signal_region1 +"_HighmassCR", label, jets,  fatjets, electrons, muons,  METv, nPV, w);
			   
      
		      

    }
  } // end else of jet req



} /// end function RunEE

double HNtypeI_CF::GetFakeRateEl(double eta, double pt, TString id){

  return fakeEst->GetElectronFakeRate(id,"AwayJetPt40",eta, pt, 0);


  if(DataYear==2016){
    if(fabs(eta) < 0.8){
      if(pt < 15.) return 0.049;
      if(pt < 23.) return 0.054;
      if(pt < 35.) return 0.071;
      if(pt < 40.) return 0.074;
      if(pt < 60.) return 0.108;
      if(pt < 60.) return 0.200;
      else return  0.200;
    }
    else  if(fabs(eta) < 1.479){
      if(pt < 15.) return 0.154;
      if(pt < 23.) return 0.094;
      if(pt < 35.) return 0.091;
      if(pt < 40.) return 0.143;
      if(pt < 60.) return 0.118;
      if(pt < 60.) return 0.171;
      else return  0.171;
    }
    else if(fabs(eta) < 2.){
      if(pt < 15.) return 0.221;
      if(pt < 23.) return 0.152;
      if(pt < 35.) return 0.140;
      if(pt < 40.) return 0.161;
      if(pt < 60.) return 0.176;
      if(pt < 60.) return 0.223;
      else return  0.228;
    }
    else{
      if(pt < 15.) return 0.180;
      if(pt < 23.) return 0.113 ;
      if(pt < 35.) return 0.144 ;
      if(pt < 40.) return 0.130;
      if(pt < 60.) return 0.183 ;
      if(pt < 60.) return 0.184 ;
      else return  0.194;
      
    }
  }
  if(DataYear==2017){
    if(fabs(eta) < 0.8){
      if(pt < 15.) return 0.065;
      if(pt < 23.) return 0.068;
      if(pt < 35.) return 0.081;
      if(pt < 40.) return 0.100;
      if(pt < 60.) return 0.142 ;
      if(pt < 60.) return 0.229 ;
      else return  0.469;
    }
    else  if(fabs(eta) < 1.479){
      if(pt < 15.) return 0.141;
      if(pt < 23.) return 0.143;
      if(pt < 35.) return 0.120;
      if(pt < 40.) return 0.113;
      if(pt < 60.) return 0.150;
      if(pt < 60.) return 0.228;
      else return  0.37;
    }
    else if(fabs(eta) < 2.){
      if(pt < 15.) return 0.277;
      if(pt < 23.) return 0.182 ;
      if(pt < 30.) return 0.208;
      if(pt < 35.) return 0.204;
      if(pt < 40.) return 0.2001;
      if(pt < 60.) return 0.227;
      else return  0.358;
    }
    else{
      if(pt < 15.) return 0.369;
      if(pt < 23.) return 0.211;
      if(pt < 30.) return 0.181;
      if(pt < 35.) return 0.213;
      if(pt < 40.) return 0.148;
      if(pt < 60.) return 0.248;
      else return  0.289;
      
    }
  }

  if(DataYear==2018){
    if(fabs(eta) < 0.8){
      if(pt < 15.) return 0.038;
      if(pt < 23.) return 0.064;
      if(pt < 30.) return 0.085;
      if(pt < 35.) return 0.098;
      if(pt < 40.) return 0.078;
      if(pt < 60.) return 0.200;
      else return  0.449;
    }
    else  if(fabs(eta) < 1.479){
      if(pt < 15.) return 0.144;
      if(pt < 23.) return 0.112;
      if(pt < 30.) return 0.111;
      if(pt < 35.) return 0.133;
      if(pt < 40.) return 0.137;
      if(pt < 60.) return 0.211;
      else return  0.366;
    }
    else if(fabs(eta) < 2.){
      if(pt < 15.) return 0.28;
      if(pt < 23.) return 0.183;
      if(pt < 30.) return 0.173;
      if(pt < 35.) return 0.195;
      if(pt < 40.) return 0.183;
      if(pt < 60.) return 0.238;
      else return  0.342;
    }
    else{
      if(pt < 15.) return 0.28;
      if(pt < 23.) return  0.201;
      if(pt < 30.) return 0.205;
      if(pt < 35.) return 0.235;
      if(pt < 40.) return 0.206;
      if(pt < 60.) return 0.236;
      else return  0.290;
      
    }
  }

  return 1.;


  return 1.;

}
double HNtypeI_CF::GetFakeRateM(double eta, double pt, TString id){


  return fakeEst->GetMuonFakeRate(id, "AwayJetPt40",eta, pt, 0);

  ///double ptbinscone[8] = { 10., 15.,20.,30.,40.,50.,  60., 200.};
  // double ptbins[9] = { 5., 10., 15.,20.,30.,45.,60.,100., 200.};  
  //double etabins2[5] = { 0.,0.8,  1.479, 2.,  2.5};

  if(DataYear == 2018){
    if(fabs(eta) < 0.8){
      if(pt < 15.) return 0.119;
      if(pt < 20.) return 0.102;
      if(pt < 30.) return 0.092;
      if(pt < 40.) return 0.095;
      if(pt < 50.) return 0.0937;
      if(pt < 60.) return 0.132;
      else return  0.33;
    }
    else  if(fabs(eta) < 1.479){
      if(pt < 15.) return 0.160;
      if(pt < 20.) return 0.117;
      if(pt < 30.) return 0.097;
      if(pt < 40.) return 0.114;
      if(pt < 50.) return 0.153;
      if(pt < 60.) return 0.243;
      else return  0.37;
    }
    else if(fabs(eta) < 2.){
      if(pt < 15.) return 0.158;
      if(pt < 20.) return 0.150;
      if(pt < 30.) return 0.122;
      if(pt < 40.) return 0.163;
      if(pt < 50.) return 0.161;
      if(pt < 60.) return 0.189;
      else return  0.39;
    }
    else{
      if(pt < 15.) return 0.172;
      if(pt < 20.) return 0.167;
      if(pt < 30.) return 0.160;
      if(pt < 40.) return 0.193;
      if(pt < 50.) return 0.223;
      if(pt < 60.) return 0.299;
      else return  0.38;
      
    }
  }	
  if(DataYear == 2017){
    if(fabs(eta) < 0.8){
      if(pt < 15.) return 0.111;
      if(pt < 20.) return 0.0896;
      if(pt < 30.) return 0.0755;
      if(pt < 40.) return 0.0087;
      if(pt < 50.) return 0.103;
      if(pt < 60.) return 0.158;
      else return  0.33;
    }
    else  if(fabs(eta) < 1.479){
      if(pt < 15.) return 0.104;
      if(pt < 20.) return 0.120;
      if(pt < 30.) return 0.087;
      if(pt < 40.) return 0.109;
      if(pt < 50.) return 0.132;
      if(pt < 60.) return 0.216;
      else return  0.35;
    }
    else if(fabs(eta) < 2.){
      if(pt < 15.) return 0.163;
      if(pt < 20.) return 0.146;
      if(pt < 30.) return 0.134;
      if(pt < 40.) return 0.144;
      if(pt < 50.) return 0.130;
      if(pt < 60.) return 0.295;
      else return  0.394;
    }
    else{
      if(pt < 15.) return 0.168;
      if(pt < 20.) return 0.163;
      if(pt < 30.) return 0.160;
      if(pt < 40.) return 0.186;
      if(pt < 50.) return 0.146;
      if(pt < 60.) return 0.242;
      else return  0.426;

    }
  }

  if(DataYear == 2016){
    if(fabs(eta) < 0.8){
      if(pt < 15.) return 0.091;
      if(pt < 20.) return 0.077;
      if(pt < 30.) return 0.0578;
      if(pt < 40.) return 0.0700;
      if(pt < 50.) return 0.0990;
      if(pt < 60.) return 0.137;
      else return  0.322;
    }
    else  if(fabs(eta) < 1.479){
      if(pt < 15.) return 0.100;
      if(pt < 20.) return 0.0882;
      if(pt < 30.) return 0.0658;
      if(pt < 40.) return 0.0814;
      if(pt < 50.) return 0.1179;
      if(pt < 60.) return 0.167;
      else return  0.322;
    }
    else if(fabs(eta) < 2.){
      if(pt < 15.) return 0.128;
      if(pt < 20.) return 0.127;
      if(pt < 30.) return 0.110;
      if(pt < 40.) return 0.110;
      if(pt < 50.) return 0.124;
      if(pt < 60.) return 0.200;
      else return  0.394;
    }
    else{
      if(pt < 15.) return 0.154;
      if(pt < 20.) return 0.138;
      if(pt < 30.) return 0.122;
      if(pt < 40.) return 0.150;
      if(pt < 50.) return 0.186;
      if(pt < 60.) return 0.229;
      else return  0.361;

    }
  }
	    
  return 1.;

}

double HNtypeI_CF::CalculateDiLepWeight( double fr1, double fr2, bool mu1Tight, bool mu2Tight){

  
  // Function calculates event weight given r/f of two muons in the event
  double alpha = 1./((1.- fr1)*(1.- fr2));

  // Initialise weight
  double  w_mm=-999.;
    
  // Terms for RF and FR events from MM
  double termTT = 0.;
  double termTL = alpha*(fr2*(1.-fr1));
  double termLT = alpha*(fr1*(1.-fr2));
  double termLL = -2.*alpha*(fr2*(fr1));
  
  
  // Term for FF events
  termTT += 0.;
  termTL += 0.;
  termLT += 0.;
  termLL += alpha*(fr1*fr2);

  
  // decide which type of event we have so to return correct weight
  bool isLL = (!mu1Tight&&!mu2Tight);
  bool isLT = (!mu1Tight&&mu2Tight);
  bool isTL = (mu1Tight && !mu2Tight);
  bool isTT = (mu1Tight && mu2Tight);

  if(isLL)w_mm = termLL;
  if(isTL)w_mm = termTL;
  if(isLT)w_mm = termLT;
  if(isTT)w_mm=  termTT;  



  return w_mm;   
}

double HNtypeI_CF:: GetFakeWeightElectron(std::vector<Electron> electrons , TString tight_id){

  if(electrons.size() != 2) return 0.;
  
  TString histtag=tight_id;
  //if(tight_id == "passTightID") histtag="passTightID_noccb";
  float _el1_eta=fabs(electrons.at(0).Eta());
  float _el2_eta=fabs(electrons.at(1).Eta());

  double fr1 = GetFakeRateEl(_el1_eta, electrons.at(0).Pt(), histtag);
  double fr2 = GetFakeRateEl(_el2_eta, electrons.at(1).Pt(), histtag);

  bool isTight1 = electrons[0].PassID(tight_id);
  bool isTight2 =  electrons[1].PassID(tight_id);
  return CalculateDiLepWeight(fr1, fr2, isTight1, isTight2);
  
}

double HNtypeI_CF:: GetFakeWeightMuon(std::vector<Muon> muons , TString tight_id){

  if(muons.size() != 2) return 0.;

  TString histtag1=tight_id;
  TString histtag2=tight_id;

  if(tight_id == "POGHighPtMixTight") {
    if(muons[0].Pt() < 60) histtag1 == "POGTightPFIsoVeryTight";
    if(muons[1].Pt() < 60) histtag2 == "POGTightPFIsoVeryTight";
  }
  if(tight_id == "POGHighPtTight") {
    if(muons[0].Pt() < 60) return 0.;
    if(muons[1].Pt() < 60) return 0.;
  }
  /*
    HNTightV1_AwayJetPt40;1
    KEY: TH2DPOGTightPFIsoVeryTight_AwayJetPt40;1
    KEY: TH2DPOGHighPtMixTight_AwayJetPt40;1
   */

  float _mu1_eta=fabs(muons.at(0).Eta());
  float _mu2_eta=fabs(muons.at(1).Eta());

  double fr1 = GetFakeRateM(_mu1_eta, muons.at(0).Pt(), histtag1);
  double fr2 = GetFakeRateM(_mu2_eta, muons.at(1).Pt(), histtag2);

  
  bool isTight1 = muons[0].PassID(tight_id);
  bool isTight2 =  muons[1].PassID(tight_id);
  return CalculateDiLepWeight(fr1, fr2, isTight1, isTight2);

}



HNtypeI_CF::HNtypeI_CF(){
  
  RunFake = false;

}
 
 HNtypeI_CF::~HNtypeI_CF(){
   delete rand_;

}

void HNtypeI_CF::FilAllMuonPlots(TString label , TString cut,  std::vector<Muon> muons, float w){

  for(unsigned int i=0; i <  muons.size(); i++){

    TString mu_lab="muon1";
    if(i==1) mu_lab="muon2";
    if(i==2) mu_lab="muon3";
    FillHist( cut+ "/pt_mu_"+mu_lab+label , muons.at(i).Pt() , w, 500, 0., 1000., "muon p_{T} GeV");
    FillHist( cut+ "/dxy_mu_"+mu_lab+label , muons.at(i).dXY() , w, 500, -1., 1., "dXY");
    FillHist( cut+ "/dz_mu_"+mu_lab+label , muons.at(i).dZ() , w, 500, -1., 1., "dZ");
    FillHist( cut+ "/reliso_mu_"+mu_lab+label , muons.at(i).RelIso() , w, 50, 0., 1., "R_{ISO} GeV");
    FillHist( cut+ "/eta_"+mu_lab+ label  , muons.at(i).Eta() , w, 60, -3., 3.,"muon #eta");
    FillHist( cut+ "/IP3D_"+mu_lab+ label  , muons.at(i).IP3D(), w, 20, -10., 10., "IP3D");
    FillHist( cut+ "/IP3Derr_"+mu_lab+ label  , muons.at(i).IP3Derr(), w, 20, -10., 10., "IP3Derr");
  }
}
void HNtypeI_CF::FilAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w){

  FillHist( cut+ "/nelectrons"+label , size(els) , w, 5, 0., 5., "n_{el}");

  for(unsigned int i=0; i < els.size(); i++){
    TString el_lab="el1";
    if(i==1) el_lab="el2";
    if(i==2) el_lab="el3";
    FillHist( cut+ "/pt_vetoel_"+label , els.at(i).Pt() , w, 500, 0., 1000., "el p_{T} GeV");
    FillHist( cut+ "/dxy_vetoel_"+label , els.at(i).dXY() , w, 500, -1., 1., "dXY");
    FillHist( cut+ "/dz_vetoel_"+label , els.at(i).dZ() , w, 500, -1., 1., "dZ");
    FillHist( cut+ "/reliso_vetoel_"+label , els.at(i).RelIso() , w, 50, 0., 1., "R_{ISO} GeV");
    FillHist( cut+ "/eta_"+ label  , els.at(i).Eta() , w, 60, -3., 3.,"el #eta");
    FillHist( cut+ "/cc_vetoel", els[i].IsGsfCtfScPixChargeConsistent()  , w, 2, 0., 2.,"Charge Cons.");
    FillHist( cut+ "/convveto_vetoel", els[i].PassConversionVeto()  , w, 2, 0., 2.,"Conv. veto.");
    FillHist( cut+ "/mva_vetoel_"+label , els.at(i).MVAIso() , w, 100, -1., 1., "el mva");
    FillHist( cut+ "/mvanoniso_vetoel_"+label , els.at(i).MVANoIso() , w, 100, -1., 1., "el mva nonIso");
    FillHist( cut+ "/Full5x5_sigmaIetaIeta_"+label , els.at(i).Full5x5_sigmaIetaIeta() , w, 1000, 0., 0.1, "");
    FillHist( cut+ "/dEtaSeed_"+label , els.at(i).dEtaSeed() , w, 1000, -0.1, 0.1, "");
    FillHist( cut+ "/dPhiIn_"+label , els.at(i).dPhiIn() , w, 1000, -.1, 0.1, "");
    FillHist( cut+ "/HoverE_"+label , els.at(i).HoverE() , w, 100, 0., 0.5, "");
    FillHist( cut+ "/InvEminusInvP_"+label , els.at(i).InvEminusInvP() , w, 100, 0., 0.2, "");
    FillHist( cut+ "/e2x5OverE5x5_"+label , els.at(i).e2x5OverE5x5() , w, 100, -1., 1., "");
    FillHist( cut+ "/e1x5OverE5x5_"+label , els.at(i).e1x5OverE5x5() , w, 100, -1., 1., "");
    FillHist( cut+ "/TrkIso_"+label , els.at(i).TrkIso() , w, 100, 0., 1., "");
    FillHist( cut+ "/dr03EcalRecHitSumEt_"+label , els.at(i).dr03EcalRecHitSumEt() , w, 100, 0., 1., "");
    FillHist( cut+ "/dr03HcalDepth1TowerSumEt_"+label , els.at(i).dr03HcalDepth1TowerSumEt() , w, 100, 0., 1., "");
    FillHist( cut+ "/dr03HcalTowerSumEt_"+label , els.at(i).dr03HcalTowerSumEt() , w, 100, 0., 1., "");
    FillHist( cut+ "/dr03TkSumPt_"+label , els.at(i).dr03TkSumPt() , w, 100, 0., 1., "");
    FillHist( cut+ "/ecalPFClusterIso_"+label , els.at(i).ecalPFClusterIso() , w, 100, 0., 1., "");
    FillHist( cut+ "/hcalPFClusterIso_"+label , els.at(i).hcalPFClusterIso() , w, 100, 0., 1., "");
    FillHist( cut+ "/isEcalDriven_"+label , els.at(i).isEcalDriven() , w, 100, 0., 1., "");

  }
  return;

}



void HNtypeI_CF::FillZZCRPlots(int i_channel, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){

  Particle METv = ev.GetMETVector();


  TString channel_s= (i_channel==0) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (i_channel==0) ?   25. : 20.;
  double lep2_ptcut= (i_channel==0) ?   15. : 15.;

  int NBJets=GetNBJets();

  if(i_channel == 1){
    if(muons.size() ==4 && electrons_veto.size() == 0 && muons_veto.size()==4){
      if(muons[0].Pt() > lep1_ptcut && muons[3].Pt()  > lep2_ptcut) {
	if(NBJets ==0){
	  bool m_llos_l10(false), z_cr_pass(false);
	  Particle Z1Cand;
	  Particle Z2Cand;
	  
	  for(unsigned int iel =0; iel < muons.size()-1 ; iel++){
	    for(unsigned int iel2 =iel+1; iel2 < muons.size() ; iel2++){
	      if(iel== iel2) continue;
	      Z1Cand = muons.at(iel) + muons.at(iel2);
	      if(muons.at(iel).Charge() != muons.at(iel2).Charge()){
		if(Z1Cand.M() < 10) m_llos_l10=true;
	      }
	      int zel1(-9), zel2(-9);
	      if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = muons.at(2) + muons.at(3);}
	      if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = muons.at(1) + muons.at(3);}
	      if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = muons.at(1) + muons.at(2);}
	      if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = muons.at(0) + muons.at(3);}
	      if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = muons.at(0) + muons.at(2);}
	      if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = muons.at(0) + muons.at(1);}
	      
	      if(muons.at(iel).Charge() != muons.at(iel2).Charge()){
		if(muons.at(zel1).Charge() != muons.at(zel2).Charge()){
		  if(fabs(Z1Cand.M() - 90.1) < 15.){
		    if(fabs(Z2Cand.M() - 90.1) < 15.){
		      z_cr_pass=true;
		    }
		  }
		}
	      }
	      
	    }
	  }
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(i_channel,true,"ZZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(i_channel,true,"ZZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	}
      }
    }// end ZZ                                                                                                                                                                                                                                  
  }// mm channel 
  else {
    // ee channel 
    if(electrons.size() ==4 && muons_veto.size() == 0 && electrons_veto.size()==4){
      if(electrons[0].Pt() > lep1_ptcut && electrons[3].Pt()  > lep2_ptcut) {
	if(NBJets ==0){
	  bool m_llos_l10(false), z_cr_pass(false);
	  Particle Z1Cand;
	  Particle Z2Cand;

	  for(unsigned int iel =0; iel < electrons.size()-1 ; iel++){
	    for(unsigned int iel2 =iel+1; iel2 < electrons.size() ; iel2++){
	      if(iel== iel2) continue;
	      Z1Cand = electrons.at(iel) + electrons.at(iel2);
	      if(electrons.at(iel).Charge() != electrons.at(iel2).Charge()){
		if(Z1Cand.M() < 10) m_llos_l10=true;
	      }
	      int zel1(-9), zel2(-9);
	      if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = electrons.at(2) + electrons.at(3);}
	      if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = electrons.at(1) + electrons.at(3);}
	      if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = electrons.at(1) + electrons.at(2);}
	      if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = electrons.at(0) + electrons.at(3);}
	      if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = electrons.at(0) + electrons.at(2);}
	      if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = electrons.at(0) + electrons.at(1);}

	      if(electrons.at(iel).Charge() != electrons.at(iel2).Charge()){
		if(electrons.at(zel1).Charge() != electrons.at(zel2).Charge()){
		  if(fabs(Z1Cand.M() - 90.1) < 15.){
		    if(fabs(Z2Cand.M() - 90.1) < 15.){
		      z_cr_pass=true;
		    }
		  }
		}
	      }

	    }
	  }
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(i_channel,true,"ZZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(i_channel,true,"ZZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	}
      }
    }// end ZZ            
  } // end of ee channel 
  return;
}

double HNtypeI_CF::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}

double HNtypeI_CF::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  Particle METv = ev.GetMETVector();

  _st += METv.Pt();

  return _st;
}
void HNtypeI_CF::FillWZCRPlots(int i_channel, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){


  Particle METv = ev.GetMETVector();
  
  TString channel_s= (i_channel==0) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (i_channel==0) ?   25. : 20.;
  double lep2_ptcut= (i_channel==0) ?   15. : 15.;
  double metcut = 50.;
  double mtcut = 20.;
  double trilep_masscut=105.;

  int NBJets=GetNBJets();

  if(i_channel == 1){
    // muon channel   require 3 muons and 0 electrons
    if(muons.size() ==3 && electrons_veto.size() == 0 && muons_veto.size()==3){
      // pass pt cuts
      if(muons[0].Pt() > lep1_ptcut && muons[1].Pt()  > lep2_ptcut) {
	// met cut
	if(METv.Pt() > metcut){
	  // 0 b jets
	  if(NBJets ==0){

	    
	    bool m_llos_l10(false);
	    // ind pair of leptons that are from Z
	    Particle ZCand;
	    int iel_Z1(0), iel_Z2(0), iel_W(0);
	    float mass_diff_tmp=9999999.;

	    for(unsigned int iel =0; iel < muons.size()-1 ; iel++){
	      for(unsigned int iel2 =iel+1; iel2 < muons.size() ; iel2++){
		if(iel== iel2) continue;
		Particle llCand = muons.at(iel) + muons.at(iel2);
		if(muons.at(iel).Charge() != muons.at(iel2).Charge()){
		  if(llCand.M() < 10) m_llos_l10=true;
		  if(fabs(llCand.M() - 90.1) < mass_diff_tmp){
		    mass_diff_tmp = fabs(llCand.M() - 90.1) ;
		    iel_Z1=iel;
		    iel_Z2=iel2;
		    ZCand = muons.at(iel) + muons.at(iel2);
		  }
		}
	      }
	    }

	    // if no mass < 10 GeV
	    if(!m_llos_l10){
	      
	      // specify W lpton
	      if((iel_Z1 + iel_Z2)==3) iel_W=0;
	      if((iel_Z1 + iel_Z2)==2) iel_W=1;
	      if((iel_Z1 + iel_Z2)==1) iel_W=2;
	      
	      // require OS mass in Z window
	      if(mass_diff_tmp < 10.){
		if(MT(METv, muons.at(iel_W)) > mtcut){
		  if((muons.at(0) + muons.at(1) + muons.at(2)).M()  > trilep_masscut){
		    // fill channel specific plots
		    FillRegionPlots(i_channel,true,"WZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
		    // fill plot for all channels
		    FillRegionPlots(i_channel,true,"WZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
		  }
		}
	      }
	    }
	  }
	}
      }
    }// WZ                             
  }// mm channel
  else {
    // ee channel 

    if(electrons.size() ==3 && muons_veto.size() == 0 && electrons_veto.size()==3){
      if(electrons[0].Pt() > lep1_ptcut && electrons[1].Pt()  > lep2_ptcut) {
	if(METv.Pt() > metcut){
	  if(NBJets ==0){
	    bool m_llos_l10(false);
	    Particle ZCand;
	    int iel_Z1(0), iel_Z2(0), iel_W(0);
	    float mass_diff_tmp=999999.;
	    for(unsigned int iel =0; iel < electrons.size()-1 ; iel++){
	      for(unsigned int iel2 =iel+1; iel2 < electrons.size() ; iel2++){
		if(iel== iel2) continue;
		Particle llCand = electrons.at(iel) + electrons.at(iel2);
		if(electrons.at(iel).Charge() != electrons.at(iel2).Charge()){
		  if(llCand.M() < 10) m_llos_l10=true;
		  if(fabs(llCand.M() - 90.1) < mass_diff_tmp){
		    mass_diff_tmp = fabs(llCand.M() - 90.1) ;
		    iel_Z1=iel;
		    iel_Z2=iel2;
		    ZCand = electrons.at(iel) + electrons.at(iel2);
		  }
		}
	      }
	    }
	    if(!m_llos_l10){

	      if((iel_Z1 + iel_Z2)==3) iel_W=0;
	      if((iel_Z1 + iel_Z2)==2) iel_W=1;
	      if((iel_Z1 + iel_Z2)==1) iel_W=2;


	      if(mass_diff_tmp < 10.){
		if(MT(METv, electrons.at(iel_W)) > mtcut){
		  if((electrons.at(0) + electrons.at(1) + electrons.at(2)).M()  > trilep_masscut){
		    FillRegionPlots(i_channel,true,"WZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
		    FillRegionPlots(i_channel,true,"WZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
		  }
		}
	      }
	    }
	  }
	}
      }
    }// WZ                                                                                                                                                                          
  }// end ee channel 

}

void HNtypeI_CF::MakeSignalPlots(){
  bool mupt_pass=true;
  int mother_nu(0);
  unsigned int ind_w(0);
  TString  mu_ch="";
  if(Signal){
    gens = GetGens();
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      vector<int> history = TrackGenSelfHistory(gen, gens);

      if(gens.at(gen.MotherIndex()).PID() == 9900012){
        if(gen.PID() == 13) mu_ch="minus";
        if(gen.PID() == -13) mu_ch="plus";

      }

      if(fabs(gen.PID()) == 24 && gens.at(gen.MotherIndex()).PID() == 9900012){
        ind_w= i;
      }

      if(gen.PID() == 9900012 &&  gen.Status() == 22) {
        mother_nu= gen.MotherIndex();
      }
    }
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      vector<int> history = TrackGenSelfHistory(gen, gens);


      if(int(gen.MotherIndex()) == ind_w && gen.Status() == 23) {
        FillHist( ("SignalGen/" + mu_ch+"jet_w_pt").Data(), gens.at(i).Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"jet_w_eta", gens.at(i).Eta(),  1., 60, -3., 3,"");
        FillHist( "SignalGen/" + mu_ch+"w_dijet_mass", gens.at(gen.MotherIndex()).M(),  1., 200, 0., 200.,"");
      }
      if(gen.PID() == 9900012){
        FillHist( "SignalGen/" + mu_ch+"Neutrino_pt", gens.at(i).Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"Neutrino_mass", gens.at(i).M(), 1., 200, 0., 2000.,"");
        FillHist( "SignalGen/" + mu_ch+"Neutrino_eta", gens.at(i).Eta(), 1., 60, -3., 3,"");
      }
      if(fabs(gen.PID()) != 13) continue;
      //if(gen.PID() != 13) continue;                                                                                                                                           


      if(gens.at(gen.MotherIndex()).PID() == 9900012) {
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_Neutrino_pt", gen.Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_Neutrino_eta", gen.Eta(),  1., 60, -3., 3,"");

        for(unsigned int j=2; j<gens.size(); j++){
          Gen gen2 = gens.at(j);
          vector<int> history = TrackGenSelfHistory(gen2, gens);
          if(j==ind_w){
            FillHist( "SignalGen/" + mu_ch+"Lep_mu_Neutrino_dr_W_Neutrino", gen2.DeltaR(gen), 1., 100, 0., 10.,"");
            FillHist( "SignalGen/" + mu_ch+"Mass_mu_Neutrino__W_Neutrino", (gen2 + gen).M(), 1., 150, 0., 1500.,"");
          }
	}

        if(gen.Pt() < 50.) mupt_pass = false;
      }
      else if(gen.MotherIndex() == mother_nu){
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_W_pt", gen.Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_W_eta", gen.Eta(),  1., 60, -3., 3,"");
        if(gen.Pt() < 50.) mupt_pass = false;

      }
    }

    bool elpt_pass=true;
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      vector<int> history = TrackGenSelfHistory(gen, gens);

      if(fabs(gen.PID()) != 11) continue;

      if(gens.at(gen.MotherIndex()).PID() == 9900012) {
        FillHist( "SignalGen/" + mu_ch+"Lep_el_Neutrino_pt", gen.Pt(), 1., 200, 0., 400.,"");
        if(gen.Pt() < 35.) elpt_pass = false;
      }
      else if(gen.MotherIndex() == mother_nu){

        FillHist( "SignalGen/" + mu_ch+"Lep_el_W_pt", gen.Pt(), 1., 200, 0., 400., "");

        if(gen.Pt() < 35.) elpt_pass = false;

      }
    }

    if(mupt_pass)       FillHist( "SignalGen/" + mu_ch+"Lep_muon_50gev_pt", 1. ,1., 2, 0., 2,"");
    if(elpt_pass)       FillHist( "SignalGen/" + mu_ch+"Lep_electron_35gev_pt", 1., 1., 2, 0., 2.,"");
  }



}
double  HNtypeI_CF::GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets){
  
  if (type=="SR1"|| type=="SR3"){
    
    float dijetmass_tmp=9999.;
    float dijetmass=99990000.;
    int m=-999;
    int n=-999;
    double ST(0.);
    for(UInt_t emme=0; emme<jets.size(); emme++){
      ST += jets[emme].Pt();
      for(UInt_t enne=1; enne<jets.size(); enne++) {
	
	dijetmass_tmp = (jets[emme]+jets[enne]).M();
	if(emme == enne) continue;
	
	if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
	  dijetmass = dijetmass_tmp;
	  m = emme;
	  n = enne;
	}
      }
    }
    Particle Wcand = jets[m] + jets[n];
    return Wcand.M();
  }
  
  if (type=="SR2"|| type=="SR4"){
    float dijetmass_tmp=999.;
    float dijetmass=9990000;
    int m=-999;
    for(UInt_t emme=0; emme<fatjets.size(); emme++){
      dijetmass_tmp= fatjets[emme].SDMass();
      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
	dijetmass = dijetmass_tmp;
	m = emme;
      }
    }
    return fatjets[m].SDMass();
  }
  return -9999.;
}

void HNtypeI_CF::FillSigRegionPlots1(int channel, int systdir, TString syst, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11){

  Particle lep1, lep2;
  if(channel == 0 && els.size() < 2) return;
  if(channel == 1 && mus.size() < 2) return;
  if(channel == 0 ){ lep1 = els[0]; lep2 = els[1];}
  if(channel == 1 ){ lep1 = mus[0]; lep2 = mus[1];}

  
  if(jets.size() < 2) return;
  
  
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  double ST(0.);
  for(UInt_t emme=0; emme<jets.size(); emme++){
    ST += jets[emme].Pt();
    for(UInt_t enne=1; enne<jets.size(); enne++) {

      dijetmass_tmp = (jets[emme]+jets[enne]).M();
      if(emme == enne) continue;

      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
	dijetmass = dijetmass_tmp;
	m = emme;
	n = enne;
      }
    }
  }
  ST += lep1.Pt() + lep2.Pt() + met.Pt();
  double met2 = pow(met.Pt(),2.);
  Particle Wcand = jets[m] + jets[n];
  Particle W1cand = jets[m] + jets[n]+lep1 + lep2;
  Particle N1cand = jets[m] + jets[n]+lep1;
  Particle N2cand = jets[m] + jets[n]+lep2;


  if(!label_1.Contains("highmass")){
    if(jets.size() >= var1) return;
    if(jets[0].Pt() < var2)return;
    if(Wcand.DeltaR(lep2) > var3) return;
    if(lep1.Pt() < var4) return;
    if(lep2.Pt() < var5) return;
    if(Wcand.M() < var6) return;
    if(Wcand.M() > var7) return;
    if(W1cand.M() < var8) return;
    if(N1cand.M() < var9 && N2cand.M()  < var9) return;
    if(N1cand.M()  > var10 && N2cand.M()  > var10) return;
    if(met2/ST > var11) return;

    if(!IsData){
      double rand_n =  rand_->Uniform(1.);
      
      
      if (rand_n > sysval && systdir==-1) return;
      if (rand_n > sysval && systdir==1) w*=(2);
    }
    
    int cutflow_index = 3;
    if(channel == 0 )cutflow_index = 4;
    int charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  1 : -1;

    if(syst=="")FillEventCutflow(charge_i,cutflow_index,w, label_1+label_2,label_1+label_3);

    FillHist( label_1+ "/"+ label_1+label_2 +  "_nevent_"  + label_3+"_"+syst,  1.,  w, 2, 0.,2. );
    return;


  }

  double sum_eta = fabs(lep1.Eta()) + fabs(lep2.Eta());
  int bin = 0;

  float met2st = met2/ST;
  if(jets[0].Pt() < 25.){
    if(met2st < 4.)bin=1;
    else bin=2;
  }
  else{
    if(lep1.Pt() < 150.){
      if(met2st < 4.){
	if(sum_eta < 3.){
	  if(N2cand.M() < 100.) bin=3;
	  else if(N2cand.M() < 125.) bin=4;
	  else if(N2cand.M() < 150.) bin=5;
	  else if(N2cand.M() < 175.) bin=6;
	  else if(N2cand.M() < 200.) bin=7;
	  else if(N2cand.M() < 250.) bin=13;
	  else if(N2cand.M() < 300.) bin=14;
	  else bin=15;

	}
	else{
	  if(N2cand.M() < 100.) bin=8;
          else if(N2cand.M() < 125.) bin=9;
          else if(N2cand.M() < 150.) bin=10;
          else if(N2cand.M() < 175.) bin=11;
          else if(N2cand.M() < 200.) bin=12;
	  else if(N2cand.M() < 250.) bin=13;
	  else if(N2cand.M() < 300.) bin=14;
	  else bin=15;

	}
      }
      else{
	if(N2cand.M() < 100.) bin=16;
	else if(N2cand.M() < 125.) bin=17;
	else if(N2cand.M() < 150.) bin=18;
	else if(N2cand.M() < 175.) bin=19;
	else if(N2cand.M() < 200.) bin=10;
	else if(N2cand.M() < 250.) bin=21;
	else if(N2cand.M() < 300.) bin=22;
	else bin=23;
	
      }
    }
    else{
      
      
      if(N1cand.M() < 300.) bin=24;
      else       if(N1cand.M() < 300.) bin=35;
      else       if(N1cand.M() < 350.) bin=26;
      else       if(N1cand.M() < 400.) bin=27;
      else       if(N1cand.M() < 500.) bin=28;
      else       if(N1cand.M() < 1000.) bin=29;
      else bin=30;
    }
  }

  if(!IsData){
      double rand_n =  rand_->Uniform(1.);
    
    
    if (rand_n > sysval && systdir==-1) return;
    if (rand_n > sysval && systdir==1) w*=(2); 
  }
  FillHist( label_1+ "/"+ label_1 +label_2+  "_signalbin_"  + label_3+"_"+syst,  bin,  w, 30, 0.,30., "SRs");
  
	    


  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double ml2jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double mlljbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};

  FillHist( label_1+ "/"+ label_1+label_2 +  "_reco_mlljj_"  + label_3+"_"+syst,  W1cand.M(),  w, 6,mlljbins, "Reco M_{lljj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml1jj_"  + label_3+"_"+syst,  N1cand.M(),  w, 6,ml1jbins , "Reco M_{l1jj}");
  FillHist( label_1+ "/"+ label_1 +label_2 +  "_reco_ml2jj_"  + label_3+"_"+syst,  N2cand.M(),  w, 6, ml2jbins, "Reco M_{l2jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_njets_" + label_3+"_"+syst, jets.size() , w, 10, 0., 10., "N_{jets}");


}

void HNtypeI_CF::FillSigRegionPlots2(int channel, int systdir, TString syst, double sysval,TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7){

   Particle lep1, lep2;
   if(channel == 0 && els.size() < 2) return;
   if(channel == 1 && mus.size() < 2) return;
   if(channel == 0 ){ lep1 = els[0]; lep2 = els[1];}
   if(channel == 1 ){ lep1 = mus[0]; lep2 = mus[1];}

   
  if(fatjets.size() == 0) return;

  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  double ST(0.);
  for(UInt_t emme=0; emme<jets.size(); emme++){
    ST += jets[emme].Pt();
  }
  for(UInt_t emme=0; emme<fatjets.size(); emme++){
    ST += fatjets[emme].Pt();
    dijetmass_tmp = fatjets[emme].SDMass();
    if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
    }
  }
  ST += lep1.Pt() + lep2.Pt() + met.Pt();
  double met2 = met.Pt()*met.Pt();
  Particle Wcand = fatjets[m];
  Particle W1cand = fatjets[m] +lep1 + lep2;
  Particle N1cand = fatjets[m] +lep1;
  Particle N2cand = fatjets[m] +lep2;

  if(!label_1.Contains("highmass")){
    
    if(lep1.Pt() < var1) return;
    if(lep2.Pt() < var2) return;
    if(fatjets[m].SDMass() < var3) return;
    if(fatjets[m].SDMass() > var4) return;
    if(N1cand.M()  < var5 && N2cand.M()  < var5) return;
    if(N1cand.M()  > var6 && N2cand.M()  > var6) return;
    if(met2/ST > var7) return;

    if(!IsData){
      double rand_n =  rand_->Uniform(1.);
      
      
      if (rand_n > sysval && systdir==-1) return;
      if (rand_n > sysval && systdir==1) w*=(2);
    }
    FillHist( label_1+ "/"+ label_1+label_2 +  "_nevent_"  + label_3+"_"+syst,  1.,  w, 2, 0.,2. , "N cut");

    int cutflow_index = 5;
    if(channel == 0 )cutflow_index = 6;
    int charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  1 : -1;

    if(syst=="")FillEventCutflow(charge_i,cutflow_index,w, label_1+label_2,label_1+label_3);

    return;
  }

  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double ml2jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double mlljbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};

  float Nmass2 = (N1cand.M() + N2cand.M())  / 2.;
  float sumpt=  lep2.Pt() + lep1.Pt();


  int bin = 0;
  if(sumpt < 100){
    if( Nmass2 < 100) bin = 0;
    else  bin = 1;
  }
  else   if(sumpt < 200){

    if(Nmass2 < 200) bin = 2;
    else if(Nmass2 < 250) bin = 3;
    else if(Nmass2 < 300) bin = 4;
    else if(Nmass2 < 350) bin = 5;
    else if(Nmass2 < 400) bin = 6;
    else  bin =7;
  }
  else{
    if(Nmass2 < 200) bin = 8;
    else if(Nmass2 < 400) bin = 9;
    else     if(Nmass2 < 500) bin = 9;
    else     if(Nmass2 < 600) bin = 10;
    else     if(Nmass2 < 600) bin = 11;
    else     if(Nmass2 < 800) bin = 12;
    else     if(Nmass2 < 900) bin = 13;
    else     if(Nmass2 < 1000) bin = 14;
    else     if(Nmass2 < 1250) bin = 15;
    else     if(Nmass2 < 1500) bin = 16;
    else     if(Nmass2 < 2000) bin = 17;
    else bin = 18;

  }


  if(!IsData){
    double rand_n =  rand_->Uniform(1.);
    

    if (rand_n > sysval && systdir==-1) return;
    if (rand_n > sysval && systdir==1) w*=(2);
  }

  FillHist( label_1+ "/"+ label_1+label_2 +  "_signalbin_"  + label_3+"_"+syst,  bin,  w, 18, 0.,18., "SRs");
  FillHist( label_1+ "/"+ label_1+label_2 +  "_reco_mllJ_"  + label_3+"_"+syst,  W1cand.M(),  w, 6, mlljbins, "Reco M_{lljj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml1J_"  + label_3+"_"+syst,  N1cand.M(),  w, 6, ml1jbins, "Reco M_{l1jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml2J_"  + label_3+"_"+syst,  N2cand.M(),  w, 6, ml2jbins, "Reco M_{l2jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_njets_" + label_3+"_"+syst, jets.size() , w, 10, 0., 10., "N_{jets}");


  return;


}
void HNtypeI_CF::FillRegionPlots(int channel, bool plotCR, TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w){
  
  //cout << "FillRegionPlots: " << label_1 << " " << label_2 << " plotCR = " << plotCR << " nel = " << els.size()  <<  endl;


  Particle lep1, lep2;
  if(channel == 0 && els.size() < 2) return;
  if(channel == 1 && mus.size() < 2) return;
  if(channel == 0 ){ lep1 = els[0]; lep2 = els[1];}
  if(channel == 1 ){ lep1 = mus[0]; lep2 = mus[1];}
  
  if(!plotCR) return;

  int NBJets=GetNBJets();


  
  Particle llCand = lep1 + lep2;
  Particle llJCand ;
  Particle lljjCand ;

  if(fatjets.size() > 0) llJCand= lep1 + lep2+ fatjets[0];
  if(jets.size() ==1 ) lljjCand += jets[0];
  else   if(jets.size() > 1){

    float dijetmass_tmp=999.;
    float dijetmass=9990000.;
    int m=-999;
    int n=-999;

    for(UInt_t emme=0; emme<jets.size(); emme++){
      for(UInt_t enne=1; enne<jets.size(); enne++) {

        dijetmass_tmp = (jets[emme]+jets[enne]).M();
        if(emme == enne) continue;

        if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
	  dijetmass = dijetmass_tmp;
	  m = emme;
          n = enne;
	}
      }
    }
    lljjCand = lep1 + lep2 + jets[m]+jets[n];
    
  
    FillHist( label_1+ "/"+ label_1 +  "_reco_mjj_"  + label_2,   (jets[m]+jets[n]).M(),  w, 50, 0, 500, "Reco M_{jj}");
    if(els.size()> 0)FillHist( label_1+ "/"+ label_1 +  "_reco_ml1jj_"  + label_2,   (els[0] + jets[m]+jets[n]).M(),  w, 200, 0., 2000., "Reco M_{l1jj}");
    if(els.size()> 1){
      FillHist( label_1+ "/"+ label_1 +  "_reco_ml2jj_"  + label_2,   (els[1] + jets[m]+jets[n]).M(),  w,  200, 0., 2000., "Reco M_{l2jj} ");      
      FillHist( label_1+ "/"+ label_1 +  "_reco_mlljj_"  + label_2,  lljjCand.M(),  w,  200, 0., 2000., "Reco M_{lljj}");
    }
    if(mus.size()> 0)FillHist( label_1+ "/"+ label_1 +  "_reco_ml1jj_"  + label_2,   (mus[0] + jets[m]+jets[n]).M(),  w, 200, 0., 2000., "Reco M_{l1jj}");
    if(mus.size()> 1){
      FillHist( label_1+ "/"+ label_1 +  "_reco_ml2jj_"  + label_2,   (mus[1] + jets[m]+jets[n]).M(),  w,  200, 0., 2000., "Reco M_{l2jj} ");
      FillHist( label_1+ "/"+ label_1 +  "_reco_mlljj_"  + label_2,  lljjCand.M(),  w,  200, 0., 2000., "Reco M_{lljj}");
    }

  }
  if(fatjets.size() > 0)FillHist( label_1+ "/"+ label_1 +  "_reco_mllJ_"  + label_2,  llJCand.M(),  w, 250, 0, 2000, "Reco M_{llJ}");

  FillHist( label_1+ "/"+ label_1 +  "_lep1_pt_" + label_2, lep1.Pt()  , w, 500, 0., 2000.,"p_{T} GeV");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_pt_" + label_2, lep2.Pt()  , w, 200, 0., 1000.,"p_{T} GeV");

  FillHist( label_1+ "/"+ label_1 +  "_lep_sumpt_" + label_2, lep1.Pt()+lep2.Pt()  , w, 100, 0., 1000.,"p_{T} GeV");

  
  FillHist( label_1+ "/"+ label_1 +  "_lep1_eta_" + label_2, lep1.Eta()  , w, 60, -3., 3,"#eta");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_eta_" + label_2, lep2.Eta()  , w, 60, -3., 3.,"#eta");

  
  FillHist( label_1+ "/"+ label_1 + "_nPV_"+ label_2,  nvtx , w, 60, 0., 60.);
  FillHist( label_1+ "/"+ label_1 +  "_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");
  
  double ST = GetST(els, mus, jets, fatjets, met);
  double met2_st = pow(met.Pt(),2.)/ ST;
  FillHist( label_1+ "/"+ label_1 +  "_st_" + label_2, ST  , w, 200, 0., 2000.,"ST GeV");
  FillHist( label_1+ "/"+ label_1 +  "_met2_st_" + label_2, met2_st  , w, 60, 0., 30.,"MET2/ST GeV");

  FillHist( label_1+ "/"+ label_1 +  "_met_" + label_2, met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( label_1+ "/"+ label_1 +  "_nbjets_" + label_2, NBJets , w, 5, 0., 5., "N_{b jets}");
  FillHist( label_1+ "/"+ label_1 +  "_ll_mass_" + label_2,  llCand.M(), w, 200, 0., 1000., "M_{ll} GeV");
  
  float mindRelj1(99999.);
  float mindRelj2(99999.);
  float mindRmuj1(99999.);
  float mindRmuj2(99999.);
  for(unsigned int i=0; i < jets.size(); i++){
    if(i == 0)     FillHist( label_1+ "/"+ label_1 + "_jet1_pt_"+ label_2,  jets[i].Pt() , w, 400, 0., 2000., "AK4 Jet p_{T} GeV");

    FillHist( label_1+ "/"+ label_1 + "_jet_pt_"+ label_2,  jets[i].Pt() , w, 400, 0., 2000., "AK4 Jet p_{T} GeV");
    FillHist( label_1+ "/"+ label_1 + "_jet_eta_"+ label_2,  jets[i].Eta() , w, 100, -5., 5., "AK4 Jet #eta ");
    if(channel == 0 ){
      if(jets[i].DeltaR(els[0]) < mindRelj1) mindRelj1=jets[i].DeltaR(els[0]);
      if(jets[i].DeltaR(els[0]) < mindRelj2) mindRelj2=jets[i].DeltaR(els[1]);
      
    
      if(i == 0){
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep1_dr_"+ label_2,  jets[i].DeltaR(els[0]) , w, 100, 0., 5., "#Delta R(l1,j)") ;
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep2_dr_"+ label_2,  jets[i].DeltaR(els[1]) , w, 100, 0., 5., "#Delta R(l2,j)"); 
      }
      FillHist( label_1+ "/"+ label_1 + "_jet_lep1_dr_"+ label_2,  jets[i].DeltaR(els[0]) , w, 100, 0., 5., "#Delta R(l1,j)") ;
      FillHist( label_1+ "/"+ label_1 + "_jet_lep2_dr_"+ label_2,  jets[i].DeltaR(els[1]) , w, 100, 0., 5., "#Delta R(l2,j)");

    }
    if(channel == 1 ){
      if(jets[i].DeltaR(mus[0]) < mindRelj1) mindRmuj1=jets[i].DeltaR(mus[0]);
      if(jets[i].DeltaR(mus[0]) < mindRelj2) mindRmuj2=jets[i].DeltaR(mus[1]);

      if(i == 0){
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep1_dr_"+ label_2,  jets[i].DeltaR(mus[0]) , w, 100, 0., 5., "#Delta R(l1,j)") ;
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep2_dr_"+ label_2,  jets[i].DeltaR(mus[1]) , w, 100, 0., 5., "#Delta R(l2,j)");
      }
      FillHist( label_1+ "/"+ label_1 + "_jet_lep1_dr_"+ label_2,  jets[i].DeltaR(mus[0]) , w, 100, 0., 5., "#Delta R(l1,j)") ;
      FillHist( label_1+ "/"+ label_1 + "_jet_lep2_dr_"+ label_2,  jets[i].DeltaR(mus[1]) , w, 100, 0., 5., "#Delta R(l2,j)");

    }
    FillHist( label_1+"/"+label_1+"_DeltaRJ_MET_" + label_2,  jets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(J,met)");
  }
  if(channel == 0 ) FillHist( label_1+ "/"+ label_1 + "_jet_lep1_mindr_"+ label_2, mindRelj1  , w, 100, 0., 5., "#Delta R(l1,j)") ;
  if(channel == 0 ) FillHist( label_1+ "/"+ label_1 + "_jet_lep2_mindr_"+ label_2, mindRelj2  , w, 100, 0., 5., "#Delta R(l1,j)") ;
  if(channel == 1 ) FillHist( label_1+ "/"+ label_1 + "_jet_lep1_mindr_"+ label_2, mindRmuj1  , w, 100, 0., 5., "#Delta R(l1,j)") ;
  if(channel == 1 ) FillHist( label_1+ "/"+ label_1 + "_jet_lep2_mindr_"+ label_2, mindRmuj2  , w, 100, 0., 5., "#Delta R(l1,j)") ;

  if(channel == 0 ) {
    FillHist( label_1+ "/"+ label_1 + "_sumlep_eta_"+ label_2, (fabs(els[0].Eta())+  fabs(els[1].Eta())) , w, 100, 0., 5.);
    if(fabs(els[0].Eta()) > fabs(els[1].Eta()))    FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(els[0].Eta()) , w, 100, 0., 2.5);
    else  FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(els[1].Eta()) , w, 100, 0., 2.5);
  }
  if(channel == 1 ) {
    FillHist( label_1+ "/"+ label_1 + "_sumlep_eta_"+ label_2, (fabs(mus[0].Eta())+  fabs(mus[1].Eta())) , w, 100, 0., 5.);
    if(fabs(mus[0].Eta()) > fabs(mus[1].Eta()))    FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(mus[0].Eta()) , w, 100, 0., 2.5);
    else  FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(mus[1].Eta()) , w, 100, 0., 2.5);
  }


  for(unsigned int i=0; i < fatjets.size(); i++){
    FillHist( label_1+ "/"+ label_1 + "_fatjet_pt_"+ label_2,  fatjets[i].Pt() , w, 100, 0., 2000., "AK8 Jet p_{T} GeV");
    FillHist( label_1+ "/"+ label_1 + "_fatjet_sdmass_"+ label_2,  fatjets[i].SDMass() , w, 100, 0., 500., "Mass_{softdrop} GeV");
    FillHist( label_1+ "/"+ label_1 + "_fatjet_tau21_"+ label_2,  fatjets[i].PuppiTau2()/ fatjets[i].PuppiTau1() , w, 400, 0., 2., "#tau_{21}");
  
    if(channel == 0 ){
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep1_dr_"+ label_2,  fatjets[i].DeltaR(els[0]) , w, 100, 0., 5., "#Delta R(l1,fj)") ;
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep2_dr_"+ label_2,  fatjets[i].DeltaR(els[1]) , w, 100, 0., 5., "#Delta R(l2,fj)");
    }
    if(channel == 1 ){
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep1_dr_"+ label_2,  fatjets[i].DeltaR(mus[0]) , w, 100, 0., 5., "#Delta R(l1,fj)") ;
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep2_dr_"+ label_2,  fatjets[i].DeltaR(mus[1]) , w, 100, 0., 5., "#Delta R(l2,fj)");
    }
    FillHist( label_1+"/"+label_1+"_DeltaRFJ_MET_" + label_2,  fatjets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(FJ,met)");

  }

  
  
  if(channel == 0 ){
    FillHist( label_1+"/"+label_1+"_DeltaRl1_MET_" + label_2,  els[0].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(l1,met)");
    FillHist( label_1+"/"+label_1+"_DeltaRl2_MET_" + label_2,  els[1].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(l2,met)");
    FillHist( label_1+"/"+label_1+"_DeltaRLL_"+ label_2, els[0].DeltaR(els[1]) ,w, 50,  0., 5,"#DeltaR(l,l)");
  }
  else{
    FillHist( label_1+"/"+label_1+"_DeltaRl1_MET_" + label_2,  mus[0].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(l1,met)");
    FillHist( label_1+"/"+label_1+"_DeltaRl2_MET_" + label_2,  mus[1].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(l2,met)");

    FillHist( label_1+"/"+label_1+"_DeltaRLL_"+ label_2, mus[0].DeltaR(mus[1]) ,w, 50,  0., 5,"#DeltaR(l,l)");
  }
  
  
  return;
}

double HNtypeI_CF::MT(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
  
}


std::vector<Jet> HNtypeI_CF::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    Jet this_jet = jets.at(i);

    bool HasLeptonInside = false;

    for(unsigned int j=0; j<els.size(); j++){
      if( this_jet.DeltaR( *(els.at(j)) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    for(unsigned int j=0; j<mus.size(); j++){
      if( this_jet.DeltaR( *(mus.at(j)) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    //==== if all fine,
    out.push_back( this_jet );

  }
  return out;

}

bool HNtypeI_CF::LeptonPassID(Lepton &lepton, TString ID){

  if(lepton.LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)&lepton;
    return el->PassID(ID);
  }
  else if(lepton.LeptonFlavour()==Lepton::MUON){
    Muon *mu = (Muon *)&lepton;
    return mu->PassID(ID);
  }
  else{
    cerr << "[HNtypeI_CF::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}


void HNtypeI_CF::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){

  // cf lables what hist to fill
  // charge_i :+1 means SS, -1 means OS ; 0 means OS/SS
  
  //cf = 0 DiEl EXO17-028 cutflow
  //cf = 1 DiMu EXO17-028 cutflow

  // DiEl EXO17-028                                                                                                                                                              
  
  
  if(cf== 0){
    TString charge_ = (charge_i == 1) ? "same_sign" : "opposite_sign";
    vector<TString> labels_ss = {"NoCut", "METFilter", "Trigger_el", "Dilep_el_same_sign","Lep_veto_el_same_sign", "Diel_mass_same_sign", "Jet_req_same_sign", "noZ_same_sign","MET_same_sign","BJet_same_sign","HighMassJet_same_sign","HighMassSR_same_sign", "Highmass_SR1", "Highmass_SR2"};
    vector<TString> labels_os = {"NoCut", "METFilter", "Trigger_el", "Dilep_el_opposite_sign","Lep_veto_el_opposite_sign","Diel_mass_opposite_sign","Jet_req_opposite_sign", "noZ_opposite_sign","MET_opposite_sign","BJet_opposite_sign","HighMassJet_opposite_sign", "HighMassSR_opposite_sign" ,"Highmass_SR3", "Highmass_SR4"};
    if(charge_i == 1)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_"+charge_, wt, labels_ss, cut);
    else if(charge_i == -1)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_"+charge_, wt, labels_os, cut);
    else {
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_same_sign", wt, labels_ss, cut);
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_opposite_sign", wt, labels_os, cut);
    }
    return;
  }

  // DiMu EXO17-028                                                                                                                                                                                                                                                                                           
  if(cf == 1){
    TString charge_ = (charge_i == 1) ? "same_sign" : "opposite_sign";
    vector<TString> labels_ss = {"NoCut", "METFilter", "Trigger_mu", "Dilep_mu_same_sign","Lep_veto_mu_same_sign", "Dimu_mass_same_sign", "Jet_req_same_sign", "MET_same_sign","BJet_same_sign","HighMassJet_same_sign" ,"HighMassSR_same_sign","Highmass_SR1", "Highmass_SR2"};
    vector<TString> labels_os = {"NoCut", "METFilter", "Trigger_mu", "Dilep_mu_opposite_sign","Lep_veto_mu_opposite_sign","Dimu_mass_opposite_sign","Jet_req_opposite_sign", "MET_opposite_sign","BJet_opposite_sign","HighMassJet_opposite_sign","HighMassSR_opposite_sign", "Highmass_SR3", "Highmass_SR4"};
    if(charge_i == 1)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_"+charge_, wt, labels_ss, cut);
    else if(charge_i == -1)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_"+charge_, wt, labels_os, cut);
    else {
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_same_sign", wt, labels_ss, cut);
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_opposite_sign", wt, labels_os, cut);
    }
    return;
  }

  // DiMu SR1                                                                                                                                                                                                                                                                                                 
  vector<TString> masses = {"100","125","200","250","300","400","500","600","700","800","900","1000","1100","1200","1300","1400","1500","1700","2000"};
  
  if(cf== 3){

    vector<TString> labels;
    
    bool _ss = (charge_i  >= 0)  ? true : false;
    if(_ss) {
 
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR1_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR1", wt, labels, cut);
    }
    else {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR3_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR3", wt, labels, cut);

    }
  }

  // DiEL SR1/3                                                                                                                                                                                                                                                                                               
  if(cf== 4){
    vector<TString> labels;

    bool _ss = (charge_i  >= 0) ? true : false;
    if(_ss) {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR1_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_EE_SR1", wt, labels, cut);
    }
    else{
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR3_mn"+masses[x] );

      AnalyzerCore::FillEventCutflow(label+"exo_17_028_EE_SR3", wt, labels, cut);

    }
  }
  if(cf== 5){
    vector<TString> labels;
    bool _ss = (charge_i  >= 0)  ? true : false;
    if(_ss) {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR2_mn"+masses[x] );

      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR2", wt, labels, cut);
    }
    else{
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR4_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR4", wt, labels, cut);

    }
  }
  if(cf== 6){
    vector<TString> labels;

    bool _ss = (charge_i  >= 0)  ? true : false;

    if(_ss) {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR2_mn"+masses[x] );

      AnalyzerCore::FillEventCutflow(label+"exo_17_028_EE_SR2", wt, labels, cut);
    }
    else{
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR4_mn"+masses[x] );

      AnalyzerCore::FillEventCutflow(label+"exo_17_028_EE_SR4", wt, labels, cut);

    }
  }


  return;

}

void HNtypeI_CF::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNtypeI_CF::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

double HNtypeI_CF::LSFSF(int lepflav, int dir){

  //==== lepflav = 0 : electron-jet
  //==== lepflav = 1 : mu-jet

  if(lepflav==0){
    if(dir==0)     return 1.04;
    else if(dir>0) return 1.04+0.03;
    else           return 1.04-0.04;
  }
  else if(lepflav==1){
    if(dir==0)     return 1.01;
    else if(dir>0) return 1.01+0.03;
    else           return 1.01-0.03;
  }
  else{
    cerr << "[HNtypeI_CF::LSFSF] wrong lepflav : " <<  lepflav << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }

}
