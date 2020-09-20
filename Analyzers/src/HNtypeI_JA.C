#include "HNtypeI_JA.h"

void HNtypeI_JA::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  Signal = HasFlag("Signal");
  SignalOS = HasFlag("SignalOS");
  if(SignalOS) Signal=SignalOS;
  PromptLeptonOnly = HasFlag("PromptLeptonOnly");
  
  isEE = HasFlag("isEE");
  isMM = HasFlag("isMM");


  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  HEM1516 = HasFlag("HEM1516");


  rand_ = new TRandom3(1234);


  cout << "[HNtypeI_JA::initializeAnalyzer()] RunFake = " << RunFake << endl;
  cout << "[HNtypeI_JA::initializeAnalyzer()] RunCF = " << RunCF << endl;
  cout << "[HNtypeI_JA::initializeAnalyzer()] RunSyst = " << RunSyst << endl;
  cout << "[HNtypeI_JAr::initializeAnalyzer()] PromptLeptonOnly = " << PromptLeptonOnly << endl;


  MuonVetoIDs      = {"HNVeto2016", "POGLoose"};

  
  MuonTriggers.clear();
  MuonTriggersH.clear();
  ElectronTriggers.clear();
  SingleElectronTriggers.clear();
  EMuTriggers.clear();
  EMuTriggersH.clear();
  
  if(DataYear==2016){                                                                   // Lumi values for trigger weight (/pb)
    if (isMM || !IsDATA){
      MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v");                       // 27267.591112919
      MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v");                     // 27267.591112919
      MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v");                    // 35918.219492947
      MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v");                  // 35918.219492947
      MuonTriggersH.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v");                   // 35918.219492947
      MuonTriggersH.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v");                 // 35918.219492947
    } 

    // Electron trigger adopted from AN2019_031

    if(isEE || !IsDATA){
      ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");          // 35918.219492947
      //SingleElectronTriggers.push_back("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v");
      SingleElectronTriggers.push_back("HLT_Ele27_WPTight_Gsf_v");
      SingleElectronTriggers.push_back("HLT_Ele115_CaloIdVT_GsfTrkIdT_v");
    }
    //EMuTriggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v");          // 27267.591112919
    //EMuTriggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v");         // 27267.591112919
    //EMuTriggersH.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");      // 8650.628380028
    //EMuTriggersH.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");     // 8650.628380028

    // These are needed for applying lepton pT cuts 
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v");
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }
  else if(DataYear==2017){
    if(isMM || !IsDATA)    MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v");
    if(isEE || ! IsDATA)ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
    //    EMuTriggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //    EMuTriggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    // These are needed for applying lepton pT cuts 
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }
  else if(DataYear==2018){
    if(isMM|| !IsDATA)MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v");
    if(isEE|| !IsDATA)ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
    //EMuTriggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //EMuTriggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    // These are needed for applying lepton pT cuts 
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

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
    //==== Only for 2016 FastSim, use different one..
    if(DataYear==2016 && IsFastSim){
      PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
    }

    cout << "[HNtypeI_JA::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] PUhname = " << PUhname << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] Signal = " << Signal << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] SignalOS = " << SignalOS << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void HNtypeI_JA::executeEvent(){

  
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


  //PrintGen(gens);

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
  vector<TString>  loose_id;
  if(isEE||!IsDATA){
    channel.push_back("EE");
    channel.push_back("EE");
    channel.push_back("EE");
    channel.push_back("EE");
    channel.push_back("EE");
    channel.push_back("EE");
    channel.push_back("EE");
 
    ELIDs.push_back(make_pair("passMediumID", "HNVeto"));
    ELIDs.push_back(make_pair("passTightID", "HNVeto"));
    ELIDs.push_back(make_pair("passMVAID_iso_WP90", "HNVeto2016"));
    ELIDs.push_back(make_pair("passMVAID_iso_WP80", "HNVeto2016"));
    ELIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
    ELIDs.push_back(make_pair("passTightID_nocc", "HNVeto"));
    ELIDs.push_back(make_pair("passTightID_noccb", "HNVeto"));

    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
  }
  if(isMM||!IsDATA){
    channel.push_back("MuMu");
    channel.push_back("MuMu");
    channel.push_back("MuMu");
    channel.push_back("MuMu");
    channel.push_back("MuMu");
    channel.push_back("MuMu");
    channel.push_back("MuMu");
   
    ELIDs.push_back(make_pair("HNVeto", "HNVeto"));
    ELIDs.push_back(make_pair("HNVeto", "HNVeto"));
    ELIDs.push_back(make_pair("HNVeto", "HNVeto"));
    ELIDs.push_back(make_pair("HNVeto", "HNVeto"));
    ELIDs.push_back(make_pair("HNVeto", "HNVeto"));
    ELIDs.push_back(make_pair("HNVeto", "HNVeto"));
    ELIDs.push_back(make_pair("HNVeto", "HNVeto"));

    MuIDs.push_back(make_pair("POGTightPFIsoVeryVeryTight","HNVeto2016"));
    MuIDs.push_back(make_pair("POGTightPFIsoVeryTight","HNVeto2016"));
    MuIDs.push_back(make_pair("POGTightPFIsoTight","HNVeto2016"));
    MuIDs.push_back(make_pair("POGTightPFIsoMedium","HNVeto2016"));
    MuIDs.push_back(make_pair("POGTightPFIsoLoose","HNVeto2016"));
    MuIDs.push_back(make_pair("HNTightV1", "HNVeto2016"));
    MuIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));

    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
  }

  for(unsigned int it_id=0; it_id<ELIDs.size(); it_id++){
    
    //************************************************///
    // setup leptton veto/loose/tight 
    //************************************************///

    TString MuonTightID      = MuIDs[it_id].first;
    TString MuonVetoID      = MuIDs[it_id].second;
    TString MuonLooseID  =  loose_id[it_id];
    TString ElectronTightID = ELIDs[it_id].first;
    TString ElectronLooseID = loose_id[it_id];
    TString ElectronVetoID  = ELIDs[it_id].second;

    TString FakeRateID =  ElectronLooseID;
    if(channel[it_id] == "MuMu") FakeRateID = MuonLooseID;
    param.Clear();


    TString Tight_ID = ElectronTightID;
    if(channel[it_id] == "MuMu") Tight_ID = MuonTightID;

  
    param.syst_ = AnalyzerParameter::Central;
    param.Name = "HNtypeI_JA_"+ElectronTightID;
    param.MCCorrrectionIgnoreNoHist = false;

    
    //==== Muon ID
    param.Muon_Tight_ID        = MuonTightID;
    param.Muon_Loose_ID        = MuonLooseID;
    param.Muon_Veto_ID        = MuonVetoID;
    param.Muon_ID_SF_Key      = "";
    param.Muon_ISO_SF_Key     = "";
    param.Muon_Trigger_SF_Key = "";
    param.Muon_MinPt = 10.;
    param.Muon_UsePtCone      = true;

    //==== Electron ID
    param.Electron_Tight_ID       = ElectronTightID;
    param.Electron_Loose_ID       = ElectronLooseID;
    param.Electron_Veto_ID        = ElectronVetoID;
    param.Electron_FR_ID          = FakeRateID;     // ID name in histmap_Electron.txt
    param.Electron_FR_Key         = "AwayJetPt40";  // histname
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
    param.Name = "HNtypeI_JA_"+channel[it_id] + "_" +Tight_ID;
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  } 
}

void HNtypeI_JA::executeEventFromParameter(AnalyzerParameter param, TString El_ID, TString channel){
  
  
  bool ee = true;
  if (channel == "MuMu") ee = false;

  //************************************************///
  // plot CR species cuts for All plots to be made
  //************************************************///

  //************************************************///
  // IsCentral true means nominal analysis settings
  //************************************************///
  bool IsCentral = !( param.Name.Contains("Syst_") );
 
  //************************************************///
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
    
    
    if(ee)weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();
    else{
      double dimu_trig_weight=0.;
      if(ev.PassTrigger(MuonTriggers)) dimu_trig_weight += 27267.591;
      if(ev.PassTrigger(MuonTriggersH)) dimu_trig_weight += 8650.628;
      weight *= weight_norm_1invpb*dimu_trig_weight*ev.MCweight();

    }
    //==== Apply Prefire
    weight *= weight_Prefire;
    FillWeightHist("prefire_"+param.Name, weight_Prefire);

  }

  //************************************************///  
  //==== No Cut cutflow entry, get nevents in sample
  //************************************************///   

  FillEventCutflow(0,0,weight, "NoCut",param.Name);
  FillEventCutflow(0,1,weight, "NoCut",param.Name);
  
  FillCutFlow(IsCentral, "CutFlow", "NoCut_"+param.Name, weight);
  FillCutFlow(IsCentral, "CutFlow", "NoCut_no_weight_"+param.Name, 1.);

  //************************************************///   
  //==== Event selecitions : MET Filters
  //************************************************///   

  if(!PassMETFilter()) return;
  FillEventCutflow(0,0,weight, "METFilter",param.Name);
  FillEventCutflow(0,1,weight, "METFilter",param.Name);

  FillCutFlow(IsCentral, "CutFlow", "METFilter_"+param.Name, weight);
  


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

  double weight_nopu=weight;
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
  
  std::vector<Electron> myelectrons_tmp = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon>     mymuons     = SelectMuons    (this_AllMuons,    param.Muon_Tight_ID, param.Muon_MinPt, 2.4);

  std::vector<Electron> myelectrons;
  if (param.Electron_Tight_ID == "passTightID" || param.Electron_Tight_ID == "passMediumID" ||   param.Electron_Tight_ID == "passMVAID_iso_WP90" ||   param.Electron_Tight_ID == "passMVAID_iso_WP80" ){
    for(unsigned int i = 0 ; i < myelectrons_tmp.size(); i++){
      if( !( myelectrons_tmp[i].IsGsfCtfScPixChargeConsistent() &&   myelectrons_tmp[i].Pass_TriggerEmulation())) continue;

      if( fabs(myelectrons_tmp[i].scEta()) <= 1.479 ){
        if( (fabs(myelectrons_tmp[i].dXY())<0.05 && fabs(myelectrons_tmp[i].dZ())<0.1) ) myelectrons.push_back(myelectrons_tmp[i]);
      }
      else{
        if( (fabs(myelectrons_tmp[i].dXY())<0.1 && fabs(myelectrons_tmp[i].dZ())<0.2) )  myelectrons.push_back(myelectrons_tmp[i]);
      }
      
    }
  }
  else{
    for(unsigned int i = 0 ; i < myelectrons_tmp.size(); i++){
      myelectrons.push_back(myelectrons_tmp[i]);
    }
  }


  if(RunFake&& ee)  myelectrons = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID , param.Electron_MinPt, 2.5);
  if(RunFake&& !ee) mymuons  = SelectMuons(this_AllMuons, param.Muon_Loose_ID, param.Muon_MinPt, 2.4);
  std::vector<Electron> veto_electrons = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon> veto_muons= SelectMuons(this_AllMuons, param.Muon_Veto_ID, 5., 2.4);

  if(ee){
    if(param.Electron_Tight_ID == "HNTight2016") {
      FilAllElectronPlots(param.Name, param.Electron_Veto_ID,veto_electrons,weight);
    }
    FilAllElectronPlots(param.Name, param.Electron_Tight_ID, myelectrons,weight);
  }
  else {
    FilAllMuonPlots(param.Name,"muons",  mymuons, weight );
    FilAllMuonPlots(param.Name,"vetomuons",  veto_muons, weight );
  }

  
 
  std::sort(myelectrons.begin(), myelectrons.end(), PtComparing);
  std::sort(mymuons.begin(), mymuons.end(), PtComparing);
  std::sort(veto_muons.begin(), veto_muons.end(), PtComparing);
  std::sort(veto_electrons.begin(),veto_electrons.end(), PtComparing);
  

  //************************************************///   
  //==== Jets vectors
  // - jet ID is Tight POG https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2016
  // - fat jet ID is: https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging
  // - ------ Tight POG
  // - ------ tau21 < 0.75   /// this needs checking as values changed 
  // - ------ 40. < SDmass < 130 GeV
  //************************************************///   

  std::vector<FatJet>   fatjets         = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 2.5);
  std::sort(fatjets.begin(), fatjets.end(), PtComparing);

  std::vector<Jet>      jets_tmp   = SelectJets(this_AllJets, param.Jet_ID, 20., 2.5);

  std::vector<Jet>      jets;
  for(unsigned int ijet =0; ijet < jets_tmp.size(); ijet++){
    bool jetok=true;
    
    for(unsigned int iel=0 ; iel < veto_electrons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(veto_electrons[iel]) < 0.4) jetok = false;
    }
    for(unsigned int iel=0 ; iel < veto_muons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(veto_muons[iel]) < 0.4) jetok = false;
    }
    if(jetok) jets.push_back(jets_tmp[ijet]);
  }
  std::sort(jets.begin(), jets.end(), PtComparing);


  //************************************************///
  // jet up b jet variable
  // --- use medium Deep CVS
  //************************************************///
  int NBJets=GetNBJets();

  
  //************************************************///
  //@@@@ Apply lepton scale factors
  //************************************************///

  if(!IsDATA){

    //==== lepton scale factors here

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;
    if(ee){
      for(unsigned int i=0; i<myelectrons.size(); i++){
	
	TString Electron_ID_SF_Key      =  El_ID;
	
	double this_recosf = mcCorr->ElectronReco_SF(myelectrons.at(i).scEta(),myelectrons.at(i).Pt(), SystDir_ElectronRecoSF);
	double this_idsf = 1.;
	if(Electron_ID_SF_Key != "HNTight2016" && !Electron_ID_SF_Key.Contains("nocc"))  mcCorr->ElectronID_SF(Electron_ID_SF_Key, myelectrons.at(i).scEta(), myelectrons.at(i).Pt(), SystDir_ElectronIDSF);
	weight *= this_recosf*this_idsf;
	FillWeightHist("el_reco_sf_"+param.Name, this_recosf);
	FillWeightHist("el_id_sf_"+param.Name, this_idsf);
	
      }
    }
    else{

    }
  }

  //************************************************///   
  // select trigger
  //************************************************///   

  if(ee) RunEE(myelectrons, veto_electrons,mymuons,veto_muons, jets, fatjets, ev,  param, weight);
  else  RunMM(myelectrons, veto_electrons,mymuons,veto_muons,  jets, fatjets,ev, param, weight);
  
}



bool HNtypeI_JA::PassHNID(TString TightID, Electron el){

  
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

void HNtypeI_JA::RunMM(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){
  
  TString label = param.Name;
  TString channel_s="_MuMu";
  Particle METv = ev.GetMETVector();
  int i_channel=1;



  bool PassDoubleMuonTrig = ev.PassTrigger(MuonTriggers);
  if(!(PassDoubleMuonTrig)) return;

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

  // setup sting to separate SS/OS
  TString charge_s= (muons[0].Charge() ==  muons[1].Charge() )  ?  "same_sign" : "opposite_sign";
  int charge_i = (muons[0].Charge() ==  muons[1].Charge() )  ?  1 : -1;

  if(SignalOS) charge_s= "opposite_sign";
  FillEventCutflow(charge_i,i_channel, w, "Dilep_mu_"+charge_s,param.Name);
  
  // current fix sincewe have no OS signal

  
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
  
  // in EE channel remove Z peak 
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
    // SR1/3 with 0 AK8 jets
    
    if( jets.size() > 1 && fatjets.size() == 0) {
      
      // SR1/3  MM                                                                                                       


      FillEventCutflow(charge_i,i_channel,w, "HighMassJet_"+charge_s,param.Name);                                                                                                                                                                                                      
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
      if(GetMass(signal_region2,jets, fatjets) <130. &&GetMass(signal_region2,jets, fatjets) > 40.){
	FillEventCutflow(charge_i,i_channel,w, "HighMassSR_"+charge_s,param.Name);                                                                                                                                                                                                      
	// fill general high mass plots
	FillRegionPlots(i_channel,true,signal_region2 + "_Highmass", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	
	// fill all mass binned plots 
	FillAllSignalRegion2(i_channel, signal_region2, IsDATA,  charge_s, label, jets, fatjets,  electrons, muons,  METv, nPV  ,w);
	FillEventCutflow(charge_i,i_channel,w, "Highmass_"+signal_region2,param.Name);
      } // end of mass req
    }// 0 AK8
  } // end met/nbjet                                                                                       
  else{

    if( jets.size() > 1 && fatjets.size() == 0) FillRegionPlots(i_channel,true,signal_region1 + "_HighmassCR", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);

    if(fatjets.size() > 0 )FillRegionPlots(i_channel,true,signal_region2 + "_HighmassCR", label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
    
    
  } // end else of jet req
return;

} /// end function                      




void HNtypeI_JA::FillAllSignalRegion1(int i_channel, TString signal_region1, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,   Particle _MET,int _npv  , double w     ){

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

    } // MM
    
  }// end of syst
}

void HNtypeI_JA::FillAllSignalRegion2(int i_channel, TString signal_region2, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons, Particle _MET,int _npv  , double w){
  

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
    }
  } // end of syst                   
    

}
vector<pair<int, pair<TString, double> > >  HNtypeI_JA::GetSyst(int i_channel, bool isdata){

  vector<pair<int, pair<TString, double> > >   systs;
  systs.push_back(make_pair(0,make_pair("",0.)));

  if(isdata){
    systs.push_back(make_pair(1,make_pair("JESup",0.9)));
    systs.push_back(make_pair(-1,make_pair("JESdown",0.9)));
    systs.push_back(make_pair(1,make_pair("JERup",0.92)));
    systs.push_back(make_pair(-1,make_pair("JERdown",0.92)));
    systs.push_back(make_pair(1,make_pair("MUIDup",0.97)));
    systs.push_back(make_pair(-1,make_pair("MUIDdown",0.97)));
  }

  return systs;
}

int HNtypeI_JA::GetNBJets(){

  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jets   = SelectJets(this_AllJets,"HNTight", 20., 2.5);

  int NBJets=0;

  for(unsigned int i=0; i<jets.size(); i++){

    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium,                                                    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  return NBJets;
}

void HNtypeI_JA::RunEE(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){


  TString channel_s="_EE";
  int i_channel=0;
  TString label = param.Name;
  Particle METv = ev.GetMETVector();
  
  
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



  if(charge_s=="same_sign"){
    
    if(param.Electron_Tight_ID == "HNTight2016") {
      FilAllElectronPlots("SS"+param.Name, param.Electron_Veto_ID, electrons_veto,w);
    }
    FilAllElectronPlots("SS"+param.Name, param.Electron_Tight_ID, electrons,w);
  }
  
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
      // SR1/3 with 0 AK8 jets                                                                                                                                

      // Jet Req
      if( jets.size() > 1 && fatjets.size() == 0) {  

	FillEventCutflow(charge_i,i_channel,w, "HighMassJet_"+charge_s,param.Name);
	if(GetMass(signal_region1, jets, fatjets) < 150. && GetMass(signal_region1, jets, fatjets) > 30.){
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
  } // end else of jet req
} /// end function RunEE

HNtypeI_JA::HNtypeI_JA(){
  
  RunFake = false;

}
 
 HNtypeI_JA::~HNtypeI_JA(){
   delete rand_;

}

void HNtypeI_JA::FilAllMuonPlots(TString label , TString cut,  std::vector<Muon> muons, float w){

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
void HNtypeI_JA::FilAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w){

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



void HNtypeI_JA::FillZZCRPlots(int i_channel, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){

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

double HNtypeI_JA::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}

double HNtypeI_JA::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  Particle METv = ev.GetMETVector();

  _st += METv.Pt();

  return _st;
}
void HNtypeI_JA::FillWZCRPlots(int i_channel, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){


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

void HNtypeI_JA::MakeSignalPlots(){
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
double  HNtypeI_JA::GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets){
  
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

void HNtypeI_JA::FillSigRegionPlots1(int channel, int systdir, TString syst, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11){

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
    double rand_n =  rand_->Uniform(1.);


    if (rand_n > sysval && systdir==-1) return;
    if (rand_n > sysval && systdir==1) w*=(2);

    int cutflow_index = 3;
    if(channel == 0 )cutflow_index = 4;
    int charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  1 : -1;

    FillEventCutflow(charge_i,cutflow_index,w, label_1+label_2,label_1+label_3);


    FillHist( label_1+ "/"+ label_1+label_2 +  "_nevent_"  + label_3+"_"+syst,  1.,  w, 2, 0.,2. );
    return;
    
    
  }

  int bin = 0;
  ///  pt2 < 25.   < 50., < 100.                                                                                                                                                                                 
  // pt < 25                                                                                                                                                                                                     
  //bin 100., 200., 300., 500., 2000.,                                                                                                                                                                           
  
  float Nmass2 = (N1cand.M() + N2cand.M())  / 2.;
  float sumpt=  lep2.Pt() + lep1.Pt();
  float met2st = met2/ST;
  if(lep1.Pt() < 150.){
    if(met2st < 2.5){
      if(N2cand.M() < 100.) bin=1;
      else if(N2cand.M() < 150.) bin=2;
      else if(N2cand.M() < 200.) bin=3;
      else if(N2cand.M() < 250.) bin=4;
      else if(N2cand.M() < 300.) bin=5;
      else bin=6;
    }
    else{
      if(N2cand.M() < 100.) bin=7;
      else if(N2cand.M() < 150.) bin=8;
      else if(N2cand.M() < 200.) bin=9;
      else if(N2cand.M() < 250.) bin=10;
      else if(N2cand.M() < 300.) bin=11;
      else bin=12;
      
    }
  }
  else{
    
    if(met2st < 3.){
      if(N1cand.M() < 300.) bin=13;
      else       if(N1cand.M() < 300.) bin=14;
      else       if(N1cand.M() < 350.) bin=15;
      else       if(N1cand.M() < 400.) bin=16;
      else       if(N1cand.M() < 500.) bin=17;
      else       if(N1cand.M() < 1000.) bin=18;
      else bin=19;
    }
    else{
      if(N1cand.M() < 300.) bin=20;
      else       if(N1cand.M() < 300.) bin=21;
      else       if(N1cand.M() < 350.) bin=22;
      else       if(N1cand.M() < 400.) bin=23;
      else       if(N1cand.M() < 500.) bin=24;
      else       if(N1cand.M() < 1000.) bin=25;
      else bin=26;
      
    }
  }
   
  double rand_n =  rand_->Uniform(1.);

  if (rand_n > sysval && systdir==-1) return;
  if (rand_n > sysval && systdir==1) w*=(2); 
  FillHist( label_1+ "/"+ label_1 +label_2+  "_signalbin_"  + label_3+"_"+syst,  bin,  w, 26, 0.,26., "Reco M_{lljj}");

  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double ml2jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double mlljbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};

  double ml1jfbins[13] = { 0., 50., 100.,150.,200.,250.,300.,400.,500.,750., 1000.,1500., 2000.};
  double ml2jfbins[13] = { 0., 50., 100.,150.,200.,250.,300.,400.,500.,750., 1000.,1500., 2000.};
  double mlljfbins[13] = { 0., 50., 100.,150.,200.,250.,300.,400.,500.,750., 1000.,1500., 2000.};

  FillHist( label_1+ "/"+ label_1+label_2 +  "_reco_mlljj_"  + label_3+"_"+syst,  W1cand.M(),  w, 6,mlljbins, "Reco M_{lljj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml1jj_"  + label_3+"_"+syst,  N1cand.M(),  w, 6,ml1jbins , "Reco M_{l1jj}");
  FillHist( label_1+ "/"+ label_1 +label_2 +  "_reco_ml2jj_"  + label_3+"_"+syst,  N2cand.M(),  w, 6, ml2jbins, "Reco M_{l2jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_njets_" + label_3+"_"+syst, jets.size() , w, 10, 0., 10., "N_{jets}");

  
}

void HNtypeI_JA::FillSigRegionPlots2(int channel, int systdir, TString syst, double sysval,TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7){

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
    double rand_n =  rand_->Uniform(1.);


    if (rand_n > sysval && systdir==-1) return;
    if (rand_n > sysval && systdir==1) w*=(2);
    FillHist( label_1+ "/"+ label_1+label_2 +  "_nevent_"  + label_3+"_"+syst,  1.,  w, 2, 0.,2. , "N cut");

    int cutflow_index = 5;
    if(channel == 0 )cutflow_index = 6;
    int charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  1 : -1;

    FillEventCutflow(charge_i,cutflow_index,w, label_1+label_2,label_1+label_3);

    return;
  }

  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double ml2jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double mlljbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};

  int bin = 0;
  ///  pt2 < 25.   < 50., < 100.
  // pt < 25
  //bin 100., 200., 300., 500., 2000., 
  float Nmass2 = (N1cand.M() + N2cand.M())  / 2.;
  float sumpt=  lep2.Pt() + lep1.Pt();

  if(sumpt < 100){
    if( Nmass2 < 100) bin = 1;
    else  bin = 2;
  }
  else   if(sumpt < 200){

    if(Nmass2 < 200) bin = 3;
    else if(Nmass2 < 250) bin = 4;
    else if(Nmass2 < 300) bin = 5;
    else if(Nmass2 < 350) bin = 6;
    else if(Nmass2 < 400) bin = 7;
    else  bin =8;
  }
  else{
    if(Nmass2 < 400) bin = 9;
    else     if(Nmass2 < 500) bin = 10;
    else     if(Nmass2 < 600) bin = 11;
    else     if(Nmass2 < 800) bin = 12;
    else     if(Nmass2 < 1000) bin = 13;
    else     if(Nmass2 < 1600) bin = 14;
    else bin = 15;
    
  }

  double ml1jfbins[13] = { 0., 50., 100.,150.,200.,250.,300.,400.,500.,750., 1000.,1500., 2000.};
  double ml2jfbins[13] = { 0., 50., 100.,150.,200.,250.,300.,400.,500.,750., 1000.,1500., 2000.};
  double mlljfbins[13] = { 0., 50., 100.,150.,200.,250.,300.,400.,500.,750., 1000.,1500., 2000.};

  double rand_n =  rand_->Uniform(1.);


  if (rand_n > sysval && systdir==-1) return;
  if (rand_n > sysval && systdir==1) w*=(2);

  FillHist( label_1+ "/"+ label_1+label_2 +  "_signalbin_"  + label_3+"_"+syst,  bin,  w, 15, 0.,15. , "Reco M_{lljj}");
  FillHist( label_1+ "/"+ label_1+label_2 +  "_reco_mllJ_"  + label_3+"_"+syst,  W1cand.M(),  w, 6, mlljbins, "Reco M_{lljj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml1J_"  + label_3+"_"+syst,  N1cand.M(),  w, 6, ml1jbins, "Reco M_{l1jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml2J_"  + label_3+"_"+syst,  N2cand.M(),  w, 6, ml2jbins, "Reco M_{l2jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_njets_" + label_3+"_"+syst, jets.size() , w, 10, 0., 10., "N_{jets}");

  return;


}
void HNtypeI_JA::FillRegionPlots(int channel, bool plotCR, TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w){
  
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
  }
  if(fatjets.size() > 0)FillHist( label_1+ "/"+ label_1 +  "_reco_mllJ_"  + label_2,  llJCand.M(),  w, 250, 0, 2000, "Reco M_{llJ}");

  FillHist( label_1+ "/"+ label_1 +  "_lep1_pt_" + label_2, lep1.Pt()  , w, 100, 0., 500.,"p_{T} GeV");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_pt_" + label_2, lep2.Pt()  , w, 50, 0., 250.,"p_{T} GeV");
  FillHist( label_1+ "/"+ label_1 +  "_lep1_eta_" + label_2, lep1.Eta()  , w, 60, -3., 53,"#eta");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_eta_" + label_2, lep2.Eta()  , w, 60, -3., 3.,"#eta");

  
  FillHist( label_1+ "/"+ label_1 + "_nPV_"+ label_2,  nvtx , w, 60, 0., 60.);
  FillHist( label_1+ "/"+ label_1 +  "_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");
  
  double ST = GetST(els, mus, jets, fatjets, met);
  double met2_st = pow(met.Pt(),2.)/ ST;
  FillHist( label_1+ "/"+ label_1 +  "_st_" + label_2, ST  , w, 200, 0., 400.,"ST GeV");
  FillHist( label_1+ "/"+ label_1 +  "_met2_st_" + label_2, met2_st  , w, 60, 0., 30.,"ST GeV");

  FillHist( label_1+ "/"+ label_1 +  "_met_" + label_2, met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( label_1+ "/"+ label_1 +  "_nbjets_" + label_2, NBJets , w, 5, 0., 5., "N_{b jets}");
  FillHist( label_1+ "/"+ label_1 +  "_ll_mass_" + label_2,  llCand.M(), w, 200, 0., 400., "M_{ll} GeV");
  
  
  for(unsigned int i=0; i < fatjets.size(); i++){
    FillHist( label_1+ "/"+ label_1 + "_fatjet_pt_"+ label_2,  fatjets[i].Pt() , w, 60, 0., 300., "AK8 Jet p_{T} GeV");
    FillHist( label_1+ "/"+ label_1 + "_fatjet_sdmass_"+ label_2,  fatjets[i].SDMass() , w, 100, 0., 500., "Mass_{softdrop} GeV");
    FillHist( label_1+ "/"+ label_1 + "_fatjet_tau21_"+ label_2,  fatjets[i].PuppiTau2()/ fatjets[i].PuppiTau1() , w, 400, 0., 2., "#tau_{21}");
  }


  
  return;
}

double HNtypeI_JA::MT(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
  
}


std::vector<Jet> HNtypeI_JA::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

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

bool HNtypeI_JA::LeptonPassID(Lepton &lepton, TString ID){

  if(lepton.LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)&lepton;
    return el->PassID(ID);
  }
  else if(lepton.LeptonFlavour()==Lepton::MUON){
    Muon *mu = (Muon *)&lepton;
    return mu->PassID(ID);
  }
  else{
    cerr << "[HNtypeI_JA::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}


void HNtypeI_JA::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){

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
  vector<TString> masses = {"100","125","200","250","300","400","500","600","700","800","900","1000","1100","1200","1300","1400","1500"};
  if(cf== 3){

    vector<TString> labels;
    
    bool _ss = (charge_i  >= 0)  ? true : false;
    if(_ss) {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR1_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu", wt, labels, cut);
    }
    else {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR1_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu", wt, labels, cut);
      
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
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR1_mn"+masses[x] );

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

void HNtypeI_JA::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNtypeI_JA::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

double HNtypeI_JA::LSFSF(int lepflav, int dir){

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
    cerr << "[HNtypeI_JA::LSFSF] wrong lepflav : " <<  lepflav << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }

}
