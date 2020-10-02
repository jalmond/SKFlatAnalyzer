#include "HNtypeI_Dilepton.h"

void HNtypeI_Dilepton::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  // dataset
  isSingleMu = HasFlag("isSingleMu");
  isEE = HasFlag("isEE");
  isMM = HasFlag("isMM");
  if(isSingleMu) isMM=HasFlag("isSingleMu");


  // bkg flags 
  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");

  // signal flags 
  Signal = HasFlag("Signal");
  SignalOS = HasFlag("SignalOS");
  if(SignalOS) Signal=SignalOS;

  PromptLeptonOnly = HasFlag("PromptLeptonOnly");
  
  /// Other flags
  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  HEM1516 = HasFlag("HEM1516");

  
  ///// random number for fake systs
  rand_ = new TRandom3(1234);


  cout << "[HNtypeI_Dilepton::initializeAnalyzer()] RunFake = " << RunFake << endl;
  cout << "[HNtypeI_Dilepton::initializeAnalyzer()] RunCF = " << RunCF << endl;
  cout << "[HNtypeI_Dilepton::initializeAnalyzer()] RunSyst = " << RunSyst << endl;
  cout << "[HNtypeI_Dileptonr::initializeAnalyzer()] PromptLeptonOnly = " << PromptLeptonOnly << endl;


  MuonVetoIDs      = {"HNVeto2016", "POGLoose"};


  HighPtMuonTriggers.clear();
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
      HighPtMuonTriggers = {"HLT_Mu50_v","HLT_TkMu50_v"};
    } 

    // Electron trigger adopted from AN2019_031

    if(isEE || !IsDATA){
      ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");          // 35918.219492947
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
    if(isEE || ! IsDATA){
      ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
      SingleElectronTriggers.push_back("HLT_Ele27_WPTight_Gsf_v");
      SingleElectronTriggers.push_back("HLT_Ele115_CaloIdVT_GsfTrkIdT_v");

    }
    if(isMM || !IsDATA) HighPtMuonTriggers = {"HLT_Mu50_v"};

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
    if(isEE|| !IsDATA){
      ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
      SingleElectronTriggers.push_back("HLT_Ele27_WPTight_Gsf_v");
      SingleElectronTriggers.push_back("HLT_Ele115_CaloIdVT_GsfTrkIdT_v");

    }
    if(isMM || !IsDATA)HighPtMuonTriggers = {"HLT_Mu50_v"};

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
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Loose, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Tight, JetTagging::incl, JetTagging::comb) );
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

    cout << "[HNtypeI_Dilepton::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNtypeI_Dilepton::initializeAnalyzer()] PUhname = " << PUhname << endl;
    cout << "[HNtypeI_Dilepton::initializeAnalyzer()] Signal = " << Signal << endl;
    cout << "[HNtypeI_Dilepton::initializeAnalyzer()] SignalOS = " << SignalOS << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void HNtypeI_Dilepton::executeEvent(){

  bool run_Debug=false;
  if(run_Debug &&  (event != 497362754)) return;
  
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

  if(run_Debug){
    for(auto i : AllMuons){
      cout << "Muon " << i.Pt() << " " << i.Eta() << " " << i.PassID("HNLoosest")<<  endl;
    }
    
    for(auto i : AllElectrons){
      cout << "Electron " << i.Pt() << " " <<i.Eta() << " " << i.PassID("HNLoosest") << " " << i.PassID("HNVeto2016") << " " << i.PassID("HNMediumV1") <<  endl;
    }
  }
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
  vector<TString>  mu_loose_id;  vector<TString>  el_loose_id;
  vector<HNtypeI_Dilepton::Flavour> channel;

  if(isEE||!IsDATA){
    
    ELIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
    el_loose_id.push_back("HNLoose2016");

    ELIDs.push_back(make_pair("HNTightV1", "HNVeto2016"));
    el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HNTightV2", "HNVeto2016"));
    el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HNTightV3", "HNVeto2016"));
    el_loose_id.push_back("HNLoose");
    

    ELIDs.push_back(make_pair("HNMediumV1", "HNVeto2016"));
    el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HNMediumV2", "HNVeto2016"));
    el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HNMediumV3", "HNVeto2016"));
    el_loose_id.push_back("HNLoose");



    // TIGHT POG + no CC in low pt
    ELIDs.push_back(make_pair("passTightID_nocc", "HNVeto2016"));
    el_loose_id.push_back("passLooseID_nocc");
    
    // POG + IP + CC
    ELIDs.push_back(make_pair("passTightID", "HNVeto2016"));
    el_loose_id.push_back("passLooseID");

    ELIDs.push_back(make_pair("passMediumID", "HNVeto2016"));
    el_loose_id.push_back("passLooseID");

    ELIDs.push_back(make_pair("passMVAID_noIso_WP80","HNVeto2016"));
    el_loose_id.push_back("passMVAID_noIso_WP80Loose");

    ELIDs.push_back(make_pair("passMVAID_noIso_WP90","HNVeto2016"));
    el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    
    ELIDs.push_back(make_pair("passMVAID_iso_WP80","HNVeto2016"));
    el_loose_id.push_back("passMVAID_iso_WP80Loose");

    ELIDs.push_back(make_pair("passMVAID_iso_WP90","HNVeto2016"));
    el_loose_id.push_back("passMVAID_iso_WP90Loose");


    for (auto i: ELIDs) { 
      channel.push_back(EE);
      MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
      mu_loose_id.push_back("HNLoose2016");
    }
  }
  if(isMM||!IsDATA){

    
    MuIDs.push_back(make_pair("POGTightPFIsoVeryTight","HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");
    
    MuIDs.push_back(make_pair("POGHighPtMixTight", "HNVeto2016"));
    mu_loose_id.push_back("POGHighPtMixLoose"); // ---> FIX FR PT BIN

    MuIDs.push_back(make_pair("POGTightPFIsoVeryVeryTight","HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");
      
    MuIDs.push_back(make_pair("POGTightPFIsoTight","HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    MuIDs.push_back(make_pair("POGTightPFIsoMedium","HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    MuIDs.push_back(make_pair("POGTightPFIsoLoose","HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    MuIDs.push_back(make_pair("HNTightV1", "HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    MuIDs.push_back(make_pair("HNTightV2", "HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    MuIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    MuIDs.push_back(make_pair("POGHighPtTight", "HNVeto2016"));
    mu_loose_id.push_back("POGHighPtLoose");

    int iel= MuIDs.size() - channel.size();
    for (int i =0; i < iel; i++){
      channel.push_back(MuMu);
      el_loose_id.push_back("HNLoose2016");
      ELIDs.push_back(make_pair("HNVeto2016", "HNVeto2016"));
    }

  }

  for(unsigned int it_id=0; it_id<ELIDs.size(); it_id++){
    
    //************************************************///
    // setup leptton veto/loose/tight 
    //************************************************///

    TString MuonTightID      = MuIDs[it_id].first;
    TString MuonVetoID       = MuIDs[it_id].second;
    TString MuonLooseID      = mu_loose_id[it_id];
    TString ElectronTightID  = ELIDs[it_id].first;
    TString ElectronLooseID  = el_loose_id[it_id];
    TString ElectronVetoID   = ELIDs[it_id].second;

    TString FakeRateID       = ElectronLooseID;
    if(channel[it_id] == MuMu) FakeRateID = MuonLooseID;
    param.Clear();

    TString Tight_ID         = ElectronTightID;
    if(channel[it_id] == MuMu) Tight_ID = MuonTightID;

  
    param.syst_ = AnalyzerParameter::Central;
    param.Name = "HNtypeI_Dilepton_"+ElectronTightID;
    param.MCCorrrectionIgnoreNoHist = false;

    
    //==== Muon ID
    param.Muon_Tight_ID        = MuonTightID;
    param.Muon_Loose_ID        = MuonLooseID;
    param.Muon_Veto_ID         = MuonVetoID;
    param.Muon_FR_ID           = MuonLooseID;     // ID name in histmap_Electron.txt
    param.Muon_FR_Key          = MuonTightID+"_AwayJetPt40";  // histname                                                                              

    param.Muon_MinPt = 10.;
    param.Muon_UsePtCone       = true;

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
    param.Jet_ID    = "tight";
    param.FatJet_ID = "tight";


    //************************************************///
    //
    // have list of IP cuts to modify ID
    //
    //************************************************///

    //************************************************///
    // change param name to link to IP/CC cuts
    //************************************************///
    if(channel[it_id]==MuMu)param.Name = "HNtypeI_Dilepton_MuMu_" +Tight_ID;
    if(channel[it_id]==EE)  param.Name = "HNtypeI_Dilepton_EE_" +Tight_ID;
    //    if(channel[it_id]==EMu) param.Name = "HNtypeI_Dilepton_EMU_" +Tight_ID;
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  } 
}

void HNtypeI_Dilepton::executeEventFromParameter(AnalyzerParameter param, TString Tight_ID, HNtypeI_Dilepton::Flavour channel){
  
  bool     IsEE      =  (channel == EE);
  TString  channel_s =  (IsEE)  ? "EE" : "MuMu";

  //************************************************///
  // plot CR species cuts for All plots to be made
  //************************************************///
  // Only run over all jet collections for one ID                                                                                 
  bool run_opt_jets(false);
  if (IsEE   && Tight_ID=="HNMediumV1")             run_opt_jets=true;
  if (!IsEE  && Tight_ID=="POGTightPFIsoVeryTight") run_opt_jets=true;

  //************************************************///
  // IsCentral true means nominal analysis settings
  //************************************************///
  //  bool IsCentral = !( param.Name.Contains("Syst_") );
 
  //************************************************///
  // setup event level objects
  Event ev = GetEvent();
  double weight = 1.;
  //************************************************///
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
    FillWeightHist("prefire_"+param.Name, weight_Prefire);
  }

  //************************************************///  
  //==== No Cut cutflow entry, get nevents in sample
  //************************************************///   


  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon>     this_AllMuons     = AllMuons;
  vector<Jet>      this_AllJets      = AllJets;
  vector<FatJet>   this_AllFatJets   = AllFatJets;

  //************************************************///   
  // setup systematic variables
  //************************************************///   
  int SystDir_ElectronIDSF(0);
  int SystDir_ElectronRecoSF(0);

  //************************************************///   
  //==== PU reweight for MC
  //************************************************///   

  double weight_pu_up   = weight;
  double weight_pu_down = weight;
  if(!IsDATA){

    double this_pureweight       = 1.;
    double this_pureweight_up    = 1.;
    double this_pureweight_down  = 1.;
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
  if(RunFake) elID = param.Electron_FR_ID;
  TString muID =  param.Muon_Tight_ID;
  if(RunFake) muID = param.Muon_FR_ID;

  /// Get collecton of temporary leptons
  std::vector<Electron>  tmp_myelectrons = SelectElectrons(this_AllElectrons, elID, 10., 2.5);
  std::vector<Muon>      tmp_mymuons     = SelectMuons    (this_AllMuons,     muID, 10., 2.4);

  /// declare main collection of leptons
  std::vector<Muon>         mymuons    ;
  std::vector<Electron>     myelectrons ;

  /// If running fake need loose collection and change pt to ptcone
  /// if running MC need to truth match

  if(RunCF){
    mymuons     = tmp_mymuons;
    myelectrons = tmp_myelectrons;
  }
  else if(RunFake) {

    for(unsigned int i=0; i<tmp_myelectrons.size(); i++){
      //==== electrons is a const vector. So in this function, we have to copy the elements like below                                                       
      double isocut_el = GetIsoFromID("EE", param.Electron_Tight_ID,tmp_myelectrons.at(i).Eta(), tmp_myelectrons.at(i).Pt());   
      Electron this_electron = tmp_myelectrons.at(i);
      this_electron.SetPtEtaPhiM( tmp_myelectrons.at(i).CalcPtCone(tmp_myelectrons.at(i).RelIso(), isocut_el), tmp_myelectrons.at(i).Eta(), tmp_myelectrons.at(i).Phi(), tmp_myelectrons.at(i).M() );
      myelectrons.push_back( this_electron );
    }
    for(unsigned int i=0; i<tmp_mymuons.size(); i++){
      double isocut_mu = GetIsoFromID("MuMu", param.Muon_Tight_ID,tmp_mymuons.at(i).Eta(), tmp_mymuons.at(i).Pt());
      if(param.Muon_Tight_ID == "POGHighPtMixTight"){
	if( tmp_mymuons.at(i).CalcPtCone(tmp_mymuons.at(i).RelIso(), isocut_mu) < 60.){
	  isocut_mu = GetIsoFromID("MuMu","POGTightPFIsoVeryTight",tmp_mymuons.at(i).Eta(), tmp_mymuons.at(i).Pt());
	}
      }

      Muon this_muon = tmp_mymuons.at(i);
      this_muon.SetPtEtaPhiM( tmp_mymuons.at(i).CalcPtCone(tmp_mymuons.at(i).RelIso(), isocut_mu), tmp_mymuons.at(i).Eta(), tmp_mymuons.at(i).Phi(), tmp_mymuons.at(i).M() );
      mymuons.push_back( this_muon );
    }

  }
  else if(Signal){
    // If signal keep all leptons in case truth matching is bugged 
    mymuons     = tmp_mymuons;
    myelectrons = tmp_myelectrons;
  }
  else{
    /// MC remove fakes
    mymuons     = MuonPromptOnly(tmp_mymuons, gens);
    myelectrons = ElectronPromptOnly(tmp_myelectrons, gens);
  }
  
  // veto leptons use all available
  std::vector<Electron>     veto_electrons     = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>         veto_muons         = SelectMuons(this_AllMuons, param.Muon_Veto_ID, 5., 2.4);

  // pt order collections 
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

  std::vector<FatJet>   fatjets_tmp  = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 5.);
  std::vector<Jet>      jets_tmp     = SelectJets(this_AllJets, param.Jet_ID, 20., 2.7);


  std::sort(fatjets_tmp.begin(), fatjets_tmp.end(), PtComparing);

  map<TString,   std::vector<FatJet> > fatjet_map;
  //fatjet_map["ak8_exo_17_028"]               = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1., true, 0.75, true, 40., 130., veto_electrons, veto_muons);
  fatjet_map["ak8_type1"]                      = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1., true, -999, true, 40., 130., veto_electrons, veto_muons);
  std::vector<FatJet> fatjets                  = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1., true, -999, true, 40., 130., veto_electrons, veto_muons);
  
  if(run_opt_jets){
    /// 
    fatjet_map["ak8_eta2_7_dr1"]               = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1.,  false, -999, false, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_eta2_7_dr08"]              = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  0.8, false, -999, false, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_eta2_7_nocleaning"]        = GetAK8Jets(fatjets_tmp, 200., 2.7, false, 1.,  false, -999, false, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_eta2_5_dr1"]               = GetAK8Jets(fatjets_tmp, 200., 2.5, true,  1.,  false, -999, false, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_eta2_5_dr08"]              = GetAK8Jets(fatjets_tmp, 200., 2.5, true,  0.8, false, -999, false, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_eta2_5_nocleaning"]        = GetAK8Jets(fatjets_tmp, 200., 2.5, false, 1.,  false, -999, false, -999, -999, veto_electrons, veto_muons);
    
    fatjet_map["ak8_tau21_eta2_7_dr1"]               = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1.,  true, -999, true, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_tau21_eta2_7_dr08"]              = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  0.8, true, -999, true, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_tau21_eta2_7_nocleaning"]        = GetAK8Jets(fatjets_tmp, 200., 2.7, false, 1.,  true, -999, true, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_tau21_eta2_5_dr1"]               = GetAK8Jets(fatjets_tmp, 200., 2.5, true,  1.,  true, -999, true, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_tau21_eta2_5_dr08"]              = GetAK8Jets(fatjets_tmp, 200., 2.5, true,  0.8, true, -999, true, -999, -999, veto_electrons, veto_muons);
    fatjet_map["ak8_tau21_eta2_5_nocleaning"]        = GetAK8Jets(fatjets_tmp, 200., 2.5, false, 1.,  true, -999, true, -999, -999, veto_electrons, veto_muons);
  }

  // Fill AK8 plots
  if(IsEE   && SameCharge(myelectrons)) MakeAK8JetPlots(param, fatjet_map, veto_electrons,veto_muons,weight);
  if(!IsEE  && SameCharge(mymuons))     MakeAK8JetPlots(param, fatjet_map, veto_electrons,veto_muons,weight);
  
  MakeGeneralPlots(mymuons, myelectrons,fatjet_map, param, weight);

  /// Get Jet Map  ak4_type1 used in analysis
  std::sort(jets_tmp.begin(), jets_tmp.end(), PtComparing);
  
  map<TString,   std::vector<Jet> > jet_map;
  //jet_map["ak4_exo_17_028"]              = GetAK4Jets(jets_tmp, 20., 2.7, true,  0.4, 0.8, "loose", veto_electrons, veto_muons,fatjets);
  jet_map["ak4_type1"]                   = GetAK4Jets(jets_tmp, 20., 2.7, true,  0.4,  0.8,"", veto_electrons, veto_muons,fatjets);
  if(run_opt_jets){
    jet_map["ak4_eta27_pileup_loose"]                   = GetAK4Jets(jets_tmp, 20., 2.7, true,  0.4,0.8, "loose", veto_electrons, veto_muons,fatjets);
    jet_map["ak4_eta27_pileup_medium"]                  = GetAK4Jets(jets_tmp, 20., 2.7, true,  0.4,0.8, "medium", veto_electrons, veto_muons,fatjets);
    jet_map["ak4_eta27_pileup_tight"]                   = GetAK4Jets(jets_tmp, 20., 2.7, true,  0.4,0.8, "tight", veto_electrons, veto_muons,fatjets);
    jet_map["ak4_eta25_pileup_loose"]                   = GetAK4Jets(jets_tmp, 20., 2.5, true,  0.4,0.8, "loose", veto_electrons, veto_muons,fatjets);
    jet_map["ak4_eta25_pileup_medium"]                  = GetAK4Jets(jets_tmp, 20., 2.5, true,  0.4,0.8, "medium", veto_electrons, veto_muons,fatjets);
    jet_map["ak4_eta25_pileup_tight"]                   = GetAK4Jets(jets_tmp, 20., 2.5, true,  0.4,0.8, "tight", veto_electrons, veto_muons,fatjets);
  }

  // Fill AK4 plots                                                                                                                                           
  if(IsEE   && SameCharge(myelectrons)) MakeAK4JetPlots(param, jet_map, veto_electrons,veto_muons,weight);
  if(!IsEE  && SameCharge(mymuons))     MakeAK4JetPlots(param, jet_map, veto_electrons,veto_muons,weight);
  
  //************************************************///
  //@@@@ Apply lepton scale factors
  //************************************************///

  if(!IsDATA){

    //==== lepton scale factors here

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;
    vector<TString> SF_list = {"passMediumID","passTightID","passMVAID_iso_WP80","passMVAID_iso_WP90"};
    if(IsEE){
      for (auto el: myelectrons){
	
	TString Electron_ID_SF_Key      =  Tight_ID;
	bool apply_ID_SF=false;

	for(auto i : SF_list){
	  if(i==Tight_ID) apply_ID_SF=true;
	}

	double this_recosf  = mcCorr->ElectronReco_SF(el.scEta(),el.Pt(), SystDir_ElectronRecoSF);
	double this_idsf    = (apply_ID_SF) ? mcCorr->ElectronID_SF(Electron_ID_SF_Key, el.scEta(), el.Pt(), SystDir_ElectronIDSF) :  1.;
	weight *= this_recosf*this_idsf;
	FillWeightHist("el_reco_sf_"+param.Name, this_recosf);
	FillWeightHist("el_id_sf_"+param.Name, this_idsf);
	
      }
    }
    else{
      /// Fill MM 
    }
  }

  //************************************************///   
  // select trigger
  //************************************************///   
  
  FillObjectSizes (myelectrons, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight,channel_s);
  if(IsEE)   RunEE(myelectrons, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
  if(!IsEE)  RunMM(myelectrons, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
  

}

void  HNtypeI_Dilepton::FillObjectSizes(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ll, TString  channel_s){
  
  TString label = param.Name;

  bool debug=false;
  if(debug){
    cout << "HNtypeI_Dilepton::FillObjectSizes::INFO " << endl;
    cout << "HNtypeI_Dilepton::FillObjectSizes::INFO  #electrons    = "<<  electrons.size() << endl;
    cout << "HNtypeI_Dilepton::FillObjectSizes::INFO  #muon         = "<<  muons.size() << endl;
    cout << "HNtypeI_Dilepton::FillObjectSizes::INFO  #electrons (veto) = "<<  electrons_veto.size() << endl;
    cout << "HNtypeI_Dilepton::FillObjectSizes::INFO  #muon      (veto) = "<<  muons_veto.size() << endl;
  }
  FillHist( "NObj/n_electrons_"+label+channel_s, electrons.size() ,weight_ll, 5, 0., 5,"N_{electron}");
  FillHist( "NObj/n_muons_"+label+channel_s,muons.size() ,weight_ll, 5, 0., 5,"N_{muons}");
  FillHist( "NObj/n_electrons_veto_"+label+channel_s, electrons_veto.size() ,weight_ll, 5, 0., 5,"N_{electron}");
  FillHist( "NObj/n_muons_veto_"+label+channel_s , muons_veto.size() ,weight_ll, 5, 0., 5,"N_{muon}");
  
  for(auto fatjet : map_fatjets){
    FillHist( "NObj/n_ak8jet_"+fatjet.first+"_"+label+channel_s, fatjet.second.size() ,weight_ll, 5, 0., 5,"N_{AK8}");
    if(debug)cout << "HNtypeI_Dilepton::FillObjectSizes::INFO " << fatjet.first << " = " << fatjet.second.size() << endl;
  }
  for(auto jet : map_jets){
    FillHist( "NObj/n_ak4jet_"+jet.first+"_"+label+channel_s, jet.second.size() ,weight_ll, 10, 0., 10,"N_{AK4}");
    if(debug)cout << "HNtypeI_Dilepton::FillObjectSizes::INFO " << jet.first << " = " << jet.second.size() << endl;
  }
  
}


void HNtypeI_Dilepton::RunMM(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_mm ){
  

  /// Get FatJet collection  'ak8_type1'
  std::map<TString, std::vector<FatJet> >::iterator map_fatjet_it = map_fatjets.find("ak8_type1");

  if(map_fatjet_it == map_fatjets.end() ) return;
  std::vector<FatJet> fatjets = map_fatjet_it->second;


  std::map<TString, std::vector<Jet> >::iterator map_jet_it = map_jets.find("ak4_type1");
  if(map_jet_it == map_jets.end() )return;
  std::vector<Jet> jets = map_jet_it->second;

  TString label    = param.Name;
  TString channel_s="MuMu";
  HNtypeI_Dilepton::Flavour  flavour  = MuMu;

  /// Get MET l corrected for jet smearing in MC
  Particle METunsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METunsmearedv, jets);
  
  
  FillEventCutflow(AllCharge,flavour,weight_mm, "NoCut",param.Name);
  if(!PassMETFilter()) return;
  FillEventCutflow(AllCharge,flavour,weight_mm, "METFilter",param.Name);

  // Apply trigger
  bool PassDoubleMuonTrig = ev.PassTrigger(MuonTriggers);
  bool PassHighPtMuonTrig = ev.PassTrigger(HighPtMuonTriggers);
  // if singlemuon dataset and passes double mu trigger veto
  if(SameCharge(muons)){
    if(PassDoubleMuonTrig)FillHist( "Trigger/muons"+label+channel_s, 1 ,weight_mm, 5, 0., 5,"");
    if(PassHighPtMuonTrig)FillHist( "Trigger/muons"+label+channel_s, 2 ,weight_mm, 5, 0., 5,"");
    if(PassDoubleMuonTrig||PassHighPtMuonTrig)FillHist( "Trigger/muons"+label+channel_s, 3 ,weight_mm, 5, 0., 5,"");
  }

  if(isSingleMu && PassDoubleMuonTrig)  return;
  if(isSingleMu && !PassHighPtMuonTrig) return;
  // if MC OR double and Mu50
  if(!IsData && !(PassDoubleMuonTrig|| PassHighPtMuonTrig)) return;
  
  // if Data and doublemuon dataset use double triggers only
  if(IsData &&!isSingleMu && !(PassDoubleMuonTrig)) return;
  
  /// Fix needed in 2016 muon trigger mc event weights
  if(!IsData && DataYear==2016){

      double dimu_trig_weight=0.;
      if(ev.PassTrigger(HighPtMuonTriggers)|| ev.PassTrigger(MuonTriggers))  dimu_trig_weight += 27267.591;
      if(ev.PassTrigger(HighPtMuonTriggers)|| ev.PassTrigger(MuonTriggersH)) dimu_trig_weight += 8650.628;

      weight_mm *= (dimu_trig_weight / ev.GetTriggerLumi("Full"));
  }
  
  FillEventCutflow(AllCharge,flavour,weight_mm, "Trigger_mu",param.Name);

  //************************************************///                                                                                                                                                    
  //  prompt lepton CRs
  //************************************************///                                                                                                                                                                                      
  FillZZCRPlots(flavour, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, weight_mm);
  FillWZCRPlots(flavour, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, weight_mm);
  //FillZGCRPlots(flavour, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, weight_mm);
  //FillWGCRPlots(flavour, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, weight_mm);

  // Dimuon selection 
  
  if(muons.size()  != 2 ) return;
  if(muons[0].Pt() < 20.) return;
  if(muons[1].Pt() < 10.) return;  

  // If running fake bkg now set weight to Fake weight
  if(RunFake) weight_mm= GetFakeWeightMuon(muons, param.Muon_Tight_ID);

  // setup sting to separate SS/OS
  TString    charge_s = (muons[0].Charge() ==  muons[1].Charge() )  ?  "same_sign" : "opposite_sign";
  HNtypeI_Dilepton::ChargeType charge_i = (muons[0].Charge() ==  muons[1].Charge() )  ?  SS : OS;

  if(SignalOS) { charge_s= "opposite_sign"; charge_i=OS;}

  FillEventCutflow(charge_i,flavour, weight_mm, "Dilep_mu_"+charge_s,param.Name);
  
  // lepton veto
  if(muons_veto.size()!=2) return;
  if(electrons_veto.size() > 0)  return;
  FillEventCutflow(charge_i,flavour,weight_mm, "Lep_veto_mu_"+charge_s,param.Name);


  //************************************************///                                                                                                                                                      
  Particle llCand = muons.at(0) + muons.at(1);

  //************************************************///                                                                                                                          
  
  if (llCand.M() < 10.) return;
  FillEventCutflow(charge_i,flavour,weight_mm, "Dimu_mass_"+charge_s,param.Name);

  
  //  Run High Mass with main jet collections                                                                                                                
  double FatJetTau21_SF = GetEventFatJetSF(fatjets,"ak8_type1", 0);      
  FillWeightHist("fatjet_ak8_type1_sf_"+param.Name,FatJetTau21_SF);
  RunHighMassSR(charge_i,channel_s, flavour, charge_s, llCand, label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_mm*FatJetTau21_SF, ev, param, true);
  
  for(auto map_fatjet_it : map_fatjets){
    if(map_fatjet_it.first == "ak8_type1") continue;

    FatJetTau21_SF = GetEventFatJetSF( map_fatjet_it.second, map_fatjet_it.first, 0);
    FillWeightHist("fatjet"+map_fatjet_it.first+"_sf_"+param.Name,FatJetTau21_SF);
  
    RunHighMassSR(charge_i, channel_s, flavour, charge_s, llCand, label+map_fatjet_it.first, jets,  map_fatjet_it.second,  electrons, muons,  METv, nPV, weight_mm*FatJetTau21_SF,  ev,param,false);
  }
  
  for(auto map_jet_it : map_jets){
    if(map_jet_it.first == "ak4_type1") continue;
    
    RunHighMassSR(charge_i, channel_s, flavour, charge_s, llCand, label+map_jet_it.first, map_jet_it.second,fatjets,  electrons, muons,  METv, nPV, weight_mm,  ev,param,false);
  }

}

void HNtypeI_Dilepton::RunHighMassSR(HNtypeI_Dilepton::ChargeType charge_i, TString channel_s,HNtypeI_Dilepton::Flavour flavour, TString charge_s,Particle llCand, TString label, vector<Jet> jets, vector<FatJet> fatjets,  vector<Electron> electrons , vector<Muon> muons, Particle METv, int mPV, double weight_hm, Event ev, AnalyzerParameter param, bool FullAnalysis){

  TString signal_region1 = (charge_s=="same_sign")  ?  "SR1" : "SR3";
  TString signal_region2 = (charge_s=="same_sign")  ?  "SR2" : "SR4";

  /// forget SR3
  if(signal_region1=="SR3") return;
    

  int njets = jets.size() + fatjets.size();

  
  /// High Mass Region removes 0 jet binsxs

  if(njets == 0 ) {
    if(FullAnalysis)    FillRegionPlots(flavour,true,"zerojet"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
    return;
  }
  if(FullAnalysis)      FillEventCutflow(charge_i,flavour,weight_hm, "Jet_req_"+charge_s,param.Name);
  
  bool inZpeak = (llCand.M() < 101. && llCand.M() > 81.) ;
  if(FullAnalysis &&  inZpeak)   FillRegionPlots(flavour,true,"DYCR"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
  
  if(flavour == EE && inZpeak ) return;
  if(flavour == EE)     FillEventCutflow(charge_i,flavour,weight_hm, "noZ_"+charge_s,param.Name);

  
  double ST = GetST(electrons, muons, jets, fatjets, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;

  if(FullAnalysis){
    TString ev_type="";
    if(RunFake) {
      bool el1_tight=muons[0].PassID(param.Muon_Tight_ID);
      bool el2_tight=muons[1].PassID(param.Muon_Tight_ID);
      if(el1_tight&&el2_tight)ev_type = "TT_";
      if(el1_tight&&!el2_tight)ev_type = "TL_";
      if(!el1_tight&&el2_tight)ev_type = "LT_";
      if(!el1_tight&&!el2_tight)ev_type = "LL_";
      FillRegionPlots(flavour,true,"presel"+channel_s+"_"+ev_type+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
      FillRegionPlots(flavour,true,"presel_no_weight_"+channel_s+"_"+ev_type+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, 1.);
      
    }
  }
  
  if(FullAnalysis)FillRegionPlots(flavour,true,"presel"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);

  ////////////////////////////////////////  
  // SR Variables                                                                                                                                            
  ////////////////////////////////////////      
  int  NBJets       =  GetNBJets("HNTight","medium");
  bool PassHMMet    = (met2_st < 15);
  bool PassBJetVeto = (NBJets==0); 
  double LowerMassSR1WmassCut = 30.;
  double UpperMassSR1WmassCut = 150.;
  double LowerMassSR2WmassCut = 50.;
  double UpperMassSR2WmassCut = 150.;

  ////////////////////////////////////////
  ////////////////////////////////////////
  // SR selections                        
  ////////////////////////////////////////
  ////////////////////////////////////////                                                                                                                   
  if(FullAnalysis && PassHMMet)          FillEventCutflow(charge_i,flavour,weight_hm, "MET_"+charge_s,param.Name);
  
  
  // require low MET and 0 bjets
  if(PassHMMet && PassBJetVeto){

    if(FullAnalysis) FillEventCutflow(charge_i,flavour,weight_hm, "BJet_"+charge_s,param.Name);
    
    if(fatjets.size() == 0){
      // remove low jet pt events
      if(jets[0].Pt() < 25.) return;
    }

    if(njets == 1 && fatjets.size() == 0) {
      if(FullAnalysis) FillRegionPlots(flavour,true,"singlejet_"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
    }


    // SR1/3 with 0 AK8 jets
    if( jets.size() > 1 && fatjets.size() == 0) {
      
      // SR1/3  MM                                                                   
      if(FullAnalysis)FillEventCutflow(charge_i,flavour,weight_hm, "HighMassJet_"+charge_s,param.Name); 
      
      
      // make MJJ mass cut in W wondow
      if(GetMass(signal_region1, jets, fatjets) < UpperMassSR1WmassCut && GetMass(signal_region1, jets, fatjets) > LowerMassSR1WmassCut){
	
	if(FullAnalysis)FillEventCutflow(charge_i,flavour,weight_hm, "HighMassSR_"+charge_s,param.Name);          
	if(FullAnalysis)FillRegionPlots(flavour,true,signal_region1+"_Highmass" , label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
	
	FillAllSignalRegion1(flavour, signal_region1, IsDATA,  charge_s, label, jets, fatjets,  electrons, muons,  METv, nPV ,weight_hm ,FullAnalysis);
	
	if(FullAnalysis)FillEventCutflow(charge_i,flavour,weight_hm, "Highmass_"+signal_region1,param.Name);
		
 	
      } // mass req
      
      
    } // jet req for SR2/4
    else if(fatjets.size() > 0) {
      // SR2/4  MM

      if(FullAnalysis)FillEventCutflow(charge_i,flavour,weight_hm, "HighMassJet_"+charge_s,param.Name);             
	// make mass window cut on fatjet
	if(GetMass(signal_region2,jets, fatjets) < UpperMassSR2WmassCut  &&GetMass(signal_region2,jets, fatjets) > LowerMassSR2WmassCut){
	  
	  
	  // fill general high mass plots
	  if(FullAnalysis)FillRegionPlots(flavour,true,signal_region2 + "_Highmass", label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);

	// fill all mass binned plots 
	  FillAllSignalRegion2(flavour, signal_region2, IsDATA,  charge_s, label, jets, fatjets,  electrons, muons,  METv, nPV  ,weight_hm,FullAnalysis);
	if(FullAnalysis)FillEventCutflow(charge_i,flavour,weight_hm, "Highmass_"+signal_region2,param.Name);
      } // end of mass req
    }// 0 AK8
  } // end met/nbjet                                                                                       
  else{
    // SR1/3 with 0 AK8 jets                                                                                                    
    if(FullAnalysis&& jets.size() > 1 && fatjets.size() == 0) FillRegionPlots(flavour,true,signal_region1 + "_HighmassCR", label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
  
    if(FullAnalysis&&fatjets.size() > 0 )FillRegionPlots(flavour,true,signal_region2 + "_HighmassCR", label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
    
    
  } // end else of jet req
  return;
  
} /// end function                      




void HNtypeI_Dilepton::FillAllSignalRegion1(HNtypeI_Dilepton::Flavour flavour, TString signal_region1, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,   Particle _MET,int _npv  , double w    , bool FullAnalysis ){

  vector<pair<int, pair<TString, double> > >  systs = GetSyst(flavour, isdata);
 
  for(unsigned int isys =0; isys < systs.size(); isys++){

    FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 + "_highmass","", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.,9999.);

    if(!FullAnalysis) continue;
    if (flavour == EE){

      // EE SR1/3

      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 25., 15., 50.,120., 120., 50., 110., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn125", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 30., 25., 50.,120., 140., 90., 140., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 55., 40., 50.,120., 220.,160., 225.,  6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 70., 60., 50.,120., 310.,220., 270.,  6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 80., 60., 50.,120., 370.,235., 335.,  6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 100., 65., 50.,120., 450.,335., 450.,  6.);


      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 65., 50.,120., 560.,400., 555., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 690., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 966., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1130., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1300., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1490., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1490., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1600., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 125., 0., 50.,120., 760.,400., 1930., 6.);

    }
    if (flavour == MuMu){
      
      // CC limits                                                                                                                                                                                                                                                                                                                                             
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn100", label, jets,  fatjets ,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 25., 15., 50.,120., 110., 55., 115., 9.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn125", label, jets,  fatjets ,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 25., 25., 50.,120., 140., 85., 140., 7.);      
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 50., 40., 50.,120., 250.,160., 215.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 85., 45., 50.,120., 310.,215., 270.,  7.);      
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 100., 50., 50.,120., 370.,225., 340.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 999., 110., 60., 50.,120., 490.,296., 490.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second, signal_region1 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 60., 50.,120., 610.,370., 550., 7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 680.,370., 630.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 885.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 890.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1225.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1230.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 1245., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1690.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1890.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 3.1, 110., 0., 50.,120., 800.,370., 1940.,  7.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2220., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2220., 6.);
      FillSigRegionPlots1(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region1 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 4, 25., 9999., 110., 0., 50.,120., 800.,370., 2220., 6.);

    } // MM
    
  }// end of syst
}

void HNtypeI_Dilepton::FillAllSignalRegion2(HNtypeI_Dilepton::Flavour flavour, TString signal_region2, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons, Particle _MET,int _npv  , double w, bool FullAnalysis){
  
  // get loop of systs + one nominal                                                                                                                         

  vector<pair<int, pair<TString, double> > >  systs = GetSyst(flavour, isdata);
  for(unsigned int isys =0; isys < systs.size(); isys++){

    // fill an inclusive high mass selection                                                                                                                                                                                                                                                                                                                 
    FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 + "_highmass","", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w,  9999.,  9999.,  9999.,  9999.,  9999.,  9999.,  9999.);

    if(!FullAnalysis) continue;

    if(flavour == EE){
      // EE SR2/4
       FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 25.,15., 40.,130., 90., 220., 15.);
       FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn125", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 60.,15., 40.,130., 123., 145., 15.);////////////////
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 20., 40., 130., 173., 220., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 25., 40., 130., 220., 305., 15.);//////////////
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 30., 40., 130., 270, 330., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 35., 40., 130., 330., 440., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 120., 35., 40., 130., 440., 565., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 120., 0., 40., 130., 565., 675., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 635., 775., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 740., 1005., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 865., 1030., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 890., 1185., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1035., 1395., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1085., 1460., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1140., 1590., 15.);///////////
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1245., 1700., 15.);///////
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1300., 1800., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1300., 1800., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1300., 1800., 15.);

    }
    if(flavour == MuMu){
      // fill mass binned selections                                                                                                                                                                                                                                                                                                                           
      // MM SR2/4
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 25.,  15., 40.,130., 98., 145., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn125", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 60.,  15., 40.,130., 110., 150., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 100., 20., 40., 130., 175., 235., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn250", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 25., 40., 130., 226., 280., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 20., 40., 130., 280., 340., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 65., 40., 130., 340., 445., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 65., 40., 130., 445., 560., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn600", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 560., 685., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 635., 825., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn800", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 755., 960., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn900", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 840., 1055., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0., 40., 130., 900., 1205., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1100", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 990., 1250., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1200", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1035., 1430., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1300", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1110., 1595., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1400", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1285., 1700., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1500", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1330., 1800., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn1700", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1330., 1800., 15.);
      FillSigRegionPlots2(flavour,systs[isys].first, systs[isys].second.first,  systs[isys].second.second,signal_region2 , "_mn2000", label, jets,  fatjets,  electrons, muons,  _MET, _npv, w, 140., 0.,40., 130., 1330., 1800., 15.);
    }
  } // end of syst                   
    

}

void HNtypeI_Dilepton::MakeAK4JetPlots(AnalyzerParameter param,map<TString, vector<Jet> > jet_map, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, double w){
  
  for(map<TString,   std::vector<Jet> >::iterator mit = jet_map.begin(); mit != jet_map.end(); mit++){

    for(unsigned int ijet =0; ijet < mit->second.size(); ijet++){

      for(auto vel : veto_electrons){
	FillHist( "AK4Plots/deltaR_j_lep"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(vel),w, 50, 0., 5,"#DeltaR(l,AK4)");
      }
    }
  }
  return;
}

void HNtypeI_Dilepton::MakeAK8JetPlots(AnalyzerParameter param,map<TString, vector<FatJet> > fatjet_map, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, double w){

  for(map<TString,   std::vector<FatJet> >::iterator mit = fatjet_map.begin(); mit != fatjet_map.end(); mit++){
    
    for(unsigned int ijet =0; ijet < mit->second.size(); ijet++){
      
      for(auto vel : veto_electrons){
	
       FillHist( "AK8Plots/deltaR_fj_lep"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(vel) ,w, 50, 0., 5,"#DeltaR(l,AK8)");
      }
    
      FillHist( "AK8Plots/AK8Jet_pt_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].Pt() , w, 100, 0., 1000., "AK8 jet p_{T} (GeV)");
      FillHist( "AK8Plots/AK8Jet_eta_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].Eta() , w, 100, -5., 5., "AK8 jet #eta");
      FillHist( "AK8Plots/AK8Jet_mass_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].SDMass(),w, 40, 0., 200., "AK8 SDMass");
      FillHist( "AK8Plots/AK8Jet_mass_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].PuppiTau21(),w, 20, 0., 1., "AK8 #tau_{2}/#tau_{1}");
      
      for(auto vem : veto_muons){

	FillHist( "AK8Plots/deltaR_fj_lep"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(vem) ,w, 50, 0., 5,"#DeltaR(l,AK8)");
      }
    }
  }
  return;
}


vector<Jet>   HNtypeI_Dilepton::GetAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean, TString pu_tag, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, vector<FatJet> fatjets){

  vector<Jet> output_jets;
  for(unsigned int ijet =0; ijet < jets.size(); ijet++){
    bool jetok=true;

    if(fabs(jets[ijet].Eta() > eta_cut)) continue;
    if(jets[ijet].Pt() < pt_cut)continue;

    for(unsigned int iel=0 ; iel < veto_electrons.size(); iel++){
      if(jets[ijet].DeltaR(veto_electrons[iel]) < dr_lep_clean) jetok = false;
    }
    
    for(unsigned int iel=0 ; iel < veto_muons.size(); iel++){
      if(jets[ijet].DeltaR(veto_muons[iel]) < dr_lep_clean) jetok = false;
    }
    for(unsigned int ifjet =0; ifjet < fatjets.size(); ifjet++){
      if(jets[ijet].DeltaR(fatjets[ifjet]) <dr_ak8_clean) jetok = false;
    }
    
    if(!jetok) continue;
    if(pu_tag=="")output_jets.push_back(jets[ijet]);
    else if(jets[ijet].PassPileupMVA(pu_tag)) output_jets.push_back(jets[ijet]);
  }
  return output_jets;
}


vector<FatJet>   HNtypeI_Dilepton::GetAK8Jets(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut, vector<Electron>  veto_electrons, vector<Muon>  veto_muons){

  vector<FatJet> output_fatjets;
  for(unsigned int ijet =0; ijet < fatjets.size(); ijet++){
    
    bool jetok=true;
    
    for(unsigned int iel=0 ; iel < veto_electrons.size(); iel++){
      if(fatjets[ijet].DeltaR(veto_electrons[iel]) < dr_lep_clean) jetok = false;
    }
    
    for(unsigned int iel=0 ; iel < veto_muons.size(); iel++){
      if(fatjets[ijet].DeltaR(veto_muons[iel]) < dr_lep_clean) jetok = false;
    }
    
    double lower_sd_mass_cut=sdmass_lower_cut;
    double upper_sd_mass_cut=sdmass_upper_cut;
    if(sdmass_lower_cut < 0.){
      lower_sd_mass_cut = 40.;
      upper_sd_mass_cut = 130.;
      if(DataYear==2017){
	//https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging                                                                                                                                                
	lower_sd_mass_cut=65.;
	upper_sd_mass_cut=105.;
      }
    }
    // tau21 cut has SF so need to apply SD mass for 2017
    if(apply_tau21) {
      if(DataYear==2017) {
	lower_sd_mass_cut  = 65.; 
	upper_sd_mass_cut  = 105.;
      }
    } 
    
    double tau_21_cut = tau21_cut;
    if(tau21_cut < 0.){
      if(DataYear==2016) tau_21_cut = 0.55;
      if(DataYear==2017) tau_21_cut = 0.45;
      if(DataYear==2018) tau_21_cut = 0.45;
    }
    
    if( fabs(fatjets[ijet].Eta()) > eta_cut)    continue;
    if( fabs(fatjets[ijet].Pt())  < pt_cut)    continue;
    
    if(lepton_cleaning && !jetok)  continue;
    if(apply_tau21 && !fatjets[ijet].PassPuppiTau21(tau_21_cut))  continue;
    if(apply_masscut && !fatjets[ijet].PassSDMassrange(lower_sd_mass_cut,upper_sd_mass_cut)) continue;
    
    output_fatjets.push_back(fatjets[ijet]);
  }
  
  
  return output_fatjets;
}

vector<pair<int, pair<TString, double> > >  HNtypeI_Dilepton::GetSyst(HNtypeI_Dilepton::Flavour flavour, bool isdata){

  vector<pair<int, pair<TString, double> > >   systs;
  systs.push_back(make_pair(0,make_pair("",0.)));

  if(!isdata){
    //systs.push_back(make_pair(1,make_pair("JESup",0.9)));
    //systs.push_back(make_pair(-1,make_pair("JESdown",0.9)));
    //systs.push_back(make_pair(1,make_pair("JERup",0.92)));
    //systs.push_back(make_pair(-1,make_pair("JERdown",0.92)));
    //systs.push_back(make_pair(1,make_pair("MUIDup",0.97)));
    //systs.push_back(make_pair(-1,make_pair("MUIDdown",0.97)));
  }

  return systs;
}


int HNtypeI_Dilepton::GetNBJets(TString ID, TString WP ){

  vector<Jet> this_AllJets = AllJets;
  std::vector<Jet>      jets   = SelectJets(this_AllJets,ID, 20., 2.5);

  int NBJets=0;

  JetTagging::WP jwp = JetTagging::Medium;
  if  (WP == "Loose")  jwp = JetTagging::Loose;
  if  (WP == "Tight")  jwp = JetTagging::Tight;

  for(unsigned int i=0; i<jets.size(); i++){
    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    jwp, 
						    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  return NBJets;
}



void HNtypeI_Dilepton::RunEE(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ee){
  
  std::map<TString, std::vector<FatJet> >::iterator map_fatjet_it = map_fatjets.find("ak8_type1");
  if(map_fatjet_it == map_fatjets.end() )return;
  std::vector<FatJet> fatjets = map_fatjet_it->second;

  std::map<TString, std::vector<Jet> >::iterator map_jet_it = map_jets.find("ak8_type1");
  if(map_jet_it == map_jets.end() )return;
  std::vector<Jet> jets = map_jet_it->second;


  TString channel_s="_EE";
  HNtypeI_Dilepton::Flavour flavour=EE;
  TString label = param.Name;

  Particle METUnsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  
  FillEventCutflow(AllCharge,flavour,weight_ee, "NoCut",param.Name);
  if(!PassMETFilter()) return;
  FillEventCutflow(AllCharge,flavour,weight_ee, "METFilter",param.Name);

  bool PassSingleElectronTrig = ev.PassTrigger(SingleElectronTriggers);
  bool PassDoubleElectronTrig = ev.PassTrigger(ElectronTriggers);  
  if(SameCharge(electrons)){
    if(PassDoubleElectronTrig)FillHist( "Trigger/electrons"+label+channel_s, 1 ,weight_ee, 5, 0., 5,"");
    if(PassSingleElectronTrig)FillHist( "Trigger/electrons"+label+channel_s, 2 ,weight_ee, 5, 0., 5,"");
    if(PassDoubleElectronTrig||PassSingleElectronTrig)FillHist( "Trigger/electrons"+label+channel_s, 3 ,weight_ee, 5, 0., 5,"");
  }

  if(!(PassDoubleElectronTrig)) return;

 
  /// 0 = SS+OS filling
  FillEventCutflow(AllCharge,flavour,weight_ee, "Trigger_el",param.Name);
  
  FillHist( "NObj/nelectrons"+label+channel_s, electrons.size() ,weight_ee, 5, 0., 5,"N_{electron}");
  FillHist( "NObj/nmuons"+label+channel_s    , muons.size()   ,weight_ee, 5, 0., 5,"N_{muon}");
  FillHist( "NObj/nelectrons_veto"+label+channel_s, electrons_veto.size() ,weight_ee, 5, 0., 5,"N_{electron}");
  

  //************************************************///   
  // Fill Prompt CR plots
  //************************************************///   
  FillZZCRPlots(EE, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, weight_ee);
  FillWZCRPlots(EE, electrons, electrons_veto, muons,  muons_veto, jets, fatjets, ev, label, weight_ee);


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

  if(RunFake) {
    FillHist( "FakePlots/El_fakeweight"+label+channel_s    , GetFakeWeightElectron(electrons,param.Electron_Tight_ID)   ,weight_ee, 200, -10., 10,"N_{muon}");
  }
  
  if(RunFake) weight_ee = GetFakeWeightElectron(electrons,param.Electron_Tight_ID);
  if(RunCF)   weight_ee = GetCFWeightElectron(electrons,param.Electron_Tight_ID);
  if(RunCF)   FillHist( "CFPlots/El_cfweight"+label+channel_s    , GetCFWeightElectron(electrons,param.Electron_Tight_ID) ,weight_ee, 1000, 0., 1.);


  //************************************************/// 
  // charge_s is string to label ++ vs +-           
  //************************************************/// 
  
  TString charge_s= (electrons[0].Charge() ==  electrons[1].Charge() )  ?  "same_sign" : "opposite_sign";
  HNtypeI_Dilepton::ChargeType charge_i = (electrons[0].Charge() ==  electrons[1].Charge() )  ?  SS : OS;

  if(RunCF){
    if(charge_s == "opposite_sign") {charge_s = "same_sign"; charge_i=SS;}
    else {  charge_s = "opposite_sign"; }
    charge_i=OS;
  }

  if(SignalOS){ charge_s= "opposite_sign"; charge_i=OS;}

  FillEventCutflow(charge_i,EE,weight_ee, "Dilep_el_"+charge_s,param.Name);

  if(electrons_veto.size()!=2) return;
  if(muons_veto.size() > 0)  return;
  FillEventCutflow(charge_i,EE,weight_ee, "Lep_veto_el_"+charge_s,param.Name);

  //************************************************///   
  // create proxy for Z/W candidates
  //************************************************///   
  Particle llCand = electrons.at(0) + electrons.at(1);
  
  if(llCand.M() < 10) return;
  FillEventCutflow(charge_i,EE,weight_ee, "Diel_mass_"+charge_s,param.Name);


  double FatJetTau21_SF = GetEventFatJetSF(fatjets,"ak8_type1", 0);
  FillWeightHist("fatjet_ak8_type1_sf_"+param.Name,FatJetTau21_SF);

  RunHighMassSR(charge_i,channel_s, EE, charge_s, llCand, label, jets,  fatjets,  electrons, muons,  METv, nPV,weight_ee*FatJetTau21_SF, ev, param,  true);

  for(auto map_fatjet_it : map_fatjets){
    if(map_fatjet_it.first == "ak8_type1") continue;
    FatJetTau21_SF = GetEventFatJetSF(map_fatjet_it.second,map_fatjet_it.first, 0);
    FillWeightHist("fatjet_"+map_fatjet_it.first+"_sf_"+param.Name,FatJetTau21_SF);

    RunHighMassSR(charge_i, channel_s, EE, charge_s, llCand, label+map_fatjet_it.first, jets,  map_fatjet_it.second,  electrons, muons,  METv, nPV, weight_ee, ev, param, false);
  }

  for(auto map_jet_it : map_jets){
    if(map_jet_it.first == "ak4_type1") continue;
    RunHighMassSR(charge_i, channel_s, EE, charge_s, llCand, label+map_jet_it.first,  map_jet_it.second, fatjets,  electrons, muons,  METv, nPV, weight_ee, ev, param, false);
  }
}

double HNtypeI_Dilepton::GetEventFatJetSF(vector<FatJet> fatjets, TString label, int dir){
  
  double FatJetTau21_SF(1);
  for (auto ifj : fatjets){
    FatJetTau21_SF*=GetFatJetSF(ifj, label,dir);
  }
  return FatJetTau21_SF;

}

double HNtypeI_Dilepton::GetFatJetSF(FatJet fatjet, TString tag,  int dir){

  if(IsData) return 1.;
  float fsys = -1;
  if(dir > 0) fsys =1;
  if(dir==0) fsys=0.;

  double loose_sf(1.);
  if(DataYear==2016) loose_sf = 1.03 + fsys*0.14;
  if(DataYear==2017) loose_sf = 0.957 + fsys*0.074;
  if(DataYear==2018) loose_sf = 0.980 + fsys*0.027;

  if(tag=="ak8_type1") return loose_sf;
  if (tag.Contains("_tau21" ))  return loose_sf;

  return 1.;

}


double HNtypeI_Dilepton::GetFakeRateEl(double eta, double pt, TString id){

  return fakeEst->GetElectronFakeRate(id,"AwayJetPt40",eta, pt, 0);

  return 1.;

}
double HNtypeI_Dilepton::GetFakeRateM(double eta, double pt, TString id){


  return fakeEst->GetMuonFakeRate(id, "AwayJetPt40",eta, pt, 0);

  return 1.;

}

double HNtypeI_Dilepton::CalculateDiLepWeight( double fr1, double fr2, bool mu1Tight, bool mu2Tight){

  
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




double HNtypeI_Dilepton::GetCFWeightElectron(std::vector<Electron> electrons , TString tight_id){
  //double CFBackgroundEstimator::GetElectronCFRate(TString ID, TString key, double eta, double pt, int sys){

  double el1_cf_rate =   cfEst->GetElectronCFRate(tight_id,"HNCF",(electrons[0].scEta()), electrons[0].Pt(), 0);
  double el2_cf_rate =   cfEst->GetElectronCFRate(tight_id,"HNCF",(electrons[1].scEta()), electrons[1].Pt(), 0);

  if((electrons[0].scEta()) < 1.5) el1_cf_rate *= 0.8;
  else el1_cf_rate *= 0.9;
  if((electrons[1].scEta()) < 1.5) el2_cf_rate *= 0.8;
  else el2_cf_rate *= 0.9;

  double cf_weight = (el1_cf_rate / (1.-el1_cf_rate))  + (el2_cf_rate/(1.-el2_cf_rate));
  return cf_weight;
}
double HNtypeI_Dilepton:: GetFakeWeightElectron(std::vector<Electron> electrons , TString tight_id){

  if(electrons.size() != 2) return 0.;
  
  TString histtag=tight_id;
  //if(tight_id == "passTightID") histtag="passTightID_noccb";
  float _el1_eta=fabs(electrons.at(0).scEta());
  float _el2_eta=fabs(electrons.at(1).scEta());

  double fr1 = GetFakeRateEl(_el1_eta, electrons.at(0).Pt(), histtag);
  double fr2 = GetFakeRateEl(_el2_eta, electrons.at(1).Pt(), histtag);

  bool isTight1 = electrons[0].PassID(tight_id);
  bool isTight2 =  electrons[1].PassID(tight_id);
  if(!tight_id.Contains("MVA")){
    if(fabs(CalculateDiLepWeight(fr1, fr2, isTight1, isTight2)) > 1.){
      cout << "WARNING " ;
      cout << tight_id << " " ;
      cout << CalculateDiLepWeight(fr1, fr2, isTight1, isTight2) << endl;
      cout << "fr1   fr2   isT1  isT2   "<< endl;
      cout << fr1 << " " << fr2 << " " << isTight1 << " " << isTight2 << endl;
      cout << _el1_eta << " " << electrons.at(0).Pt() << endl;
      cout << _el2_eta << " " << electrons.at(1).Pt() << endl;
    }
  }

  return CalculateDiLepWeight(fr1, fr2, isTight1, isTight2);
  
}

double HNtypeI_Dilepton:: GetFakeWeightMuon(std::vector<Muon> muons , TString tight_id){

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



HNtypeI_Dilepton::HNtypeI_Dilepton(){
  
  RunFake = false;

}
 
 HNtypeI_Dilepton::~HNtypeI_Dilepton(){
   delete rand_;

}

void HNtypeI_Dilepton::FilAllMuonPlots(TString label , TString cut,  std::vector<Muon> muons, float w){

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
void HNtypeI_Dilepton::FilAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w){

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

void HNtypeI_Dilepton::MakeGeneralPlots(vector<Muon> mymuons, vector<Electron> myelectrons, map<TString, vector<FatJet> > fatjet_map, AnalyzerParameter param ,double weight){

  if(SameCharge(myelectrons)){
    int type1=fabs(GetLeptonType(myelectrons.at(0), gens));
    int type2=fabs(GetLeptonType(myelectrons.at(1), gens));

    Particle llCand = myelectrons.at(0) + myelectrons.at(1);
    if(llCand.M() > 101. || llCand.M() < 81.) FillHist( "GeneralPlots/DeltaRLL_"+param.Electron_Tight_ID, myelectrons[0].DeltaR(myelectrons[1]), weight, 50, 0., 5,"#DeltaR(l,l)");
							
    
    FillHist( "GeneralPlots/LeptonType_"+ param.Electron_Tight_ID, type1,type2, weight, 20, -10,10., 20,-10.,10.);

  }
  if(SameCharge(mymuons)){
    FillHist( "GeneralPlots/DeltaRLL_"+param.Muon_Tight_ID, mymuons[0].DeltaR(mymuons[1]), weight, 50, 0., 5,"#DeltaR(l,l)");
  }


  if(SameCharge(myelectrons)){
    
    Particle llCand = myelectrons.at(0) + myelectrons.at(1);
    if(llCand.M() > 101. || llCand.M() < 81.){
      
      for (auto iel: myelectrons) {
	FillHist( "GeneralPlots/el_pt"+param.Electron_Tight_ID, iel.Pt(), weight, 50, 0. , 500.,"pt");
	
	if(fabs(iel.Eta()) < 1.5){
	  FillHist( "GeneralPlots/el_barrel_pt"+param.Electron_Tight_ID, iel.Pt(), weight, 50, 0. , 500.,"pt");
	  if(iel.IsGsfCtfScPixChargeConsistent ())        FillHist( "GeneralPlots/el_barrel_cc_pt"+param.Electron_Tight_ID, iel.Pt(), weight, 50, 0. , 500.,"pt");

	}
	else{
	  FillHist( "GeneralPlots/el_endcap_pt"+param.Electron_Tight_ID, iel.Pt(), weight, 50, 0. , 500.,"pt");
	  if(iel.IsGsfCtfScPixChargeConsistent ())        FillHist( "GeneralPlots/el_endcap_cc_pt"+param.Electron_Tight_ID, iel.Pt(), weight, 50, 0. , 500.,"pt");

	  
	  
	}
	if(iel.IsGsfCtfScPixChargeConsistent ()) FillHist( "GeneralPlots/el_cc_pt"+param.Electron_Tight_ID, iel.Pt(), weight, 50, 0. , 500.,"p\
t");
	
      }
    }
  }


  for(map<TString,   std::vector<FatJet> >::iterator mit = fatjet_map.begin(); mit != fatjet_map.end(); mit++){

    for(unsigned int ijet =0; ijet < mit->second.size(); ijet++){
      for(unsigned int iel=0 ; iel < myelectrons.size(); iel++){
        if(SameCharge(myelectrons))FillHist( "GeneralPlots/deltaR_fj_lep"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(myelectrons[iel]) ,weight, 50, 0., 5,"#DeltaR(l,AK8)");
      }
      for(unsigned int iel=0 ; iel < mymuons.size(); iel++){
        if(SameCharge(mymuons))FillHist( "GeneralPlots/deltaR_fj_lep"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(mymuons[iel]) ,weight, 50, 0., 5,"#DeltaR(l,AK8)");
      }
    }
  }


 
  return;
}


void HNtypeI_Dilepton::FillZZCRPlots(HNtypeI_Dilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){

  Particle METUnsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);


  TString channel_s= (flavour==EE) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (flavour==EE) ?   25. : 20.;
  double lep2_ptcut= (flavour==EE) ?   15. : 15.;


  int NBJets=GetNBJets("HNTight","medium");

  if(flavour == MuMu){
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
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(flavour,true,"ZZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(flavour,true,"ZZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
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
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(flavour,true,"ZZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	  if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(flavour,true,"ZZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	}
      }
    }// end ZZ            
  } // end of ee channel 
  return;
}

double HNtypeI_Dilepton::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}

double HNtypeI_Dilepton::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  Particle METUnsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  _st += METv.Pt();

  return _st;
}
void HNtypeI_Dilepton::FillWZCRPlots(HNtypeI_Dilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){


  Particle METUnsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  TString channel_s= (flavour==EE) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (flavour==EE) ?   25. : 20.;
  double lep2_ptcut= (flavour==EE) ?   15. : 15.;
  double metcut = 50.;
  double mtcut = 20.;
  double trilep_masscut=105.;

  int NBJets=GetNBJets("HNTight","medium");

  if(flavour == MuMu){
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
		    FillRegionPlots(flavour,true,"WZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
		    // fill plot for all channels
		    FillRegionPlots(flavour,true,"WZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
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
		    FillRegionPlots(flavour,true,"WZ_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
		    FillRegionPlots(flavour,true,"WZ_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
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

void HNtypeI_Dilepton::MakeSignalPlots(){
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
double  HNtypeI_Dilepton::GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets){
  
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

void HNtypeI_Dilepton::FillSigRegionPlots1(HNtypeI_Dilepton::Flavour channel, int systdir, TString syst, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11){

  Particle lep1, lep2;
  if(channel == EE && els.size() < 2) return;
  if(channel == MuMu && mus.size() < 2) return;
  if(channel == EE ){ lep1 = els[0]; lep2 = els[1];}
  if(channel == MuMu ){ lep1 = mus[0]; lep2 = mus[1];}

  
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
    
    HNtypeI_Dilepton::Region cutflow_index = MuMu_SR1;
    if(channel == EE )cutflow_index = EE_SR1;
    HNtypeI_Dilepton::ChargeType charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  SS : OS;
    if(RunCF) charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  OS : SS;

    if(syst=="")FillEventCutflow(charge_i,cutflow_index,w, label_1+label_2,label_1+label_3);
    //if(syst=="")FillRegionPlots(channel,true,label_1+label_2+"_All_"+label_3 , label_1, jets,  fatjets,  els, mus,  met, nPV, w);
    

    FillHist( label_1+ "/"+ label_1+label_2 +  "_nevent_"  + label_3+"_"+syst,  1.,  w, 2, 0.,2. );
    return;


  }

  double sum_eta = fabs(lep1.Eta()) + fabs(lep2.Eta());
  int bin = 0;
  double dRW_lep2 = Wcand.DeltaR(lep2);
  float met2st = met2/ST;
  if(lep1.Pt() < 150.){
    if(met2st < 5.){
      if(sum_eta < 2.){
	if(dRW_lep2 < 3.2){
	  if(N2cand.M() < 100.) bin=0;
	  else if(N2cand.M() < 125.) bin=1;
	  else if(N2cand.M() < 150.) bin=2;
	  else if(N2cand.M() < 175.) bin=3;
	  else if(N2cand.M() < 200.) bin=4;
	}
	else{
	  bin=5;
	}
	if(N2cand.M() > 200){
	  if(N2cand.M() < 250.) bin=12;
	  else if(N2cand.M() < 300.) bin=13;
	  else bin=14;
	}
      }
      else{
	if(dRW_lep2 < 3.2){
	if(N2cand.M() < 100.) bin=6;
	else if(N2cand.M() < 125.) bin=7;
	else if(N2cand.M() < 150.) bin=8;
	else if(N2cand.M() < 175.) bin=9;
	else if(N2cand.M() < 200.) bin=10;
	}
	else {
	  bin=11;
	}
	if(N2cand.M() >200){
	  if(N2cand.M() < 250.) bin=12;
	  else if(N2cand.M() < 300.) bin=13;
	  else bin=14;
	}
      }
    }
    else{
      
      if(N2cand.M() < 100.) bin=15;
      else if(N2cand.M() < 125.) bin=16;
      else if(N2cand.M() < 150.) bin=17;
      else if(N2cand.M() < 175.) bin=18;
      else if(N2cand.M() < 200.) bin=19;
      else if(N2cand.M() < 250.) bin=20;
      else if(N2cand.M() < 300.) bin=21;
      else bin=22;
      
    }
  }
  else{
    
    
      if(N1cand.M() < 300.) bin=23;
      else       if(N1cand.M() < 300.) bin=24;
      else       if(N1cand.M() < 350.) bin=25;
      else       if(N1cand.M() < 400.) bin=26;
      else       if(N1cand.M() < 500.) bin=27;
      else       if(N1cand.M() < 1000.) bin=28;
      else bin=29;
  }


  if(!IsData){
      double rand_n =  rand_->Uniform(1.);
    
    
    if (rand_n > sysval && systdir==-1) return;
    if (rand_n > sysval && systdir==1) w*=(2); 
  }
  FillHist( label_1+ "/"+ label_1 +label_2+  "_signalbin_"  + label_3+"_"+syst,  bin,  w, 29, 0.,29., "SRs");
  
	    


  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double ml2jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double mlljbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};

  FillHist( label_1+ "/"+ label_1+label_2 +  "_reco_mlljj_"  + label_3+"_"+syst,  W1cand.M(),  w, 6,mlljbins, "Reco M_{lljj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml1jj_"  + label_3+"_"+syst,  N1cand.M(),  w, 6,ml1jbins , "Reco M_{l1jj}");
  FillHist( label_1+ "/"+ label_1 +label_2 +  "_reco_ml2jj_"  + label_3+"_"+syst,  N2cand.M(),  w, 6, ml2jbins, "Reco M_{l2jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_njets_" + label_3+"_"+syst, jets.size() , w, 10, 0., 10., "N_{jets}");


}

void HNtypeI_Dilepton::FillSigRegionPlots2(HNtypeI_Dilepton::Flavour channel, int systdir, TString syst, double sysval,TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7){

   Particle lep1, lep2;
   if(channel == EE   && els.size() < 2) return;
   if(channel == MuMu && mus.size() < 2) return;
   if(channel == EE   ){ lep1 = els[0]; lep2 = els[1];}
   if(channel == MuMu ){ lep1 = mus[0]; lep2 = mus[1];}

   
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

    HNtypeI_Dilepton::Region  cutflow_index = MuMu_SR2;
    if(channel == EE )cutflow_index = EE_SR2;
    HNtypeI_Dilepton::ChargeType charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  SS : OS;

    if(RunCF) charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  OS : SS;


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
void HNtypeI_Dilepton::FillRegionPlots(HNtypeI_Dilepton::Flavour channel, bool plotCR, TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w){
  
  bool debug(false);
  if(debug){
    if(channel == EE ) cout << "FillRegionPlotsEE: " << label_1 << " " << label_2 << " plotCR = " << plotCR << " nel = " << els.size()  <<  endl;
    if(channel == MuMu ) cout << "FillRegionPlotsMuMu: " << label_1 << " " << label_2 << " plotCR = " << plotCR << " nel = " << els.size()  <<  endl; 
    else cout << "FillRegionPlots: " << label_1 << " " << label_2 << " plotCR = " << plotCR << " nel = " << els.size()  <<  endl;
  }

  Particle lep1, lep2;
  if(channel == EE   && els.size() < 2) return;
  if(channel == MuMu && mus.size() < 2) return;
  if(channel == EE   ){ lep1 = els[0]; lep2 = els[1];}
  if(channel == MuMu ){ lep1 = mus[0]; lep2 = mus[1];}
  
  if(!plotCR) return;

  int NBJets=GetNBJets("HNTight","medium");


  
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
    
  
    Particle WCand  = jets[m]+jets[n];
    Particle N1Cand  = jets[m]+jets[n]+lep1;
    Particle N2Cand  = jets[m]+jets[n]+lep2;
   
    FillHist( label_1+ "/"+ label_1 +  "_reco_ml1jj_ml2jj_"  + label_2,  N1Cand.M(), N2Cand.M(), w, 100, 0., 2000., 100, 0., 2000.);

    FillHist( label_1+ "/"+ label_1 +  "_reco_jj_lep1_dr_"  + label_2,   WCand.DeltaR(lep1),  w, 10, 0, 5, "#DeltaR (W,lep1)");
    FillHist( label_1+ "/"+ label_1 +  "_reco_jj_lep2_dr_"  + label_2,   WCand.DeltaR(lep2),  w, 10, 0, 5, "#DeltaR (W,lep2)");
    FillHist( label_1+ "/"+ label_1 +  "_reco_Wj1_lep2_dr_"  + label_2,   jets[m].DeltaR(lep2),  w, 10, 0, 5, "#DeltaR (Wj1,lep2)");
    FillHist( label_1+ "/"+ label_1 +  "_reco_Wj2_lep2_dr_"  + label_2,   jets[n].DeltaR(lep2),  w, 10, 0, 5, "#DeltaR (Wj2,lep2)");


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

  FillHist( label_1+ "/"+ label_1 +  "_lep1_pt_" + label_2, lep1.Pt()  , w, 400, 0., 2000.,"p_{T} GeV");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_pt_" + label_2, lep2.Pt()  , w, 200, 0., 1000.,"p_{T} GeV");

  FillHist( label_1+ "/"+ label_1 +  "_lep_sumpt_" + label_2, lep1.Pt()+lep2.Pt()  , w, 100, 0., 1000.,"p_{T} GeV");

  
  FillHist( label_1+ "/"+ label_1 +  "_lep1_eta_" + label_2, lep1.Eta()  , w, 30, -3., 3,"#eta");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_eta_" + label_2, lep2.Eta()  , w, 30, -3., 3.,"#eta");

  
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
    if(channel ==EE ){
      if(jets[i].DeltaR(els[0]) < mindRelj1) mindRelj1=jets[i].DeltaR(els[0]);
      if(jets[i].DeltaR(els[0]) < mindRelj2) mindRelj2=jets[i].DeltaR(els[1]);
      
    
      if(i == 0){
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep1_dr_"+ label_2,  jets[i].DeltaR(els[0]) , w, 10, 0., 5., "#Delta R(l1,j)") ;
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep2_dr_"+ label_2,  jets[i].DeltaR(els[1]) , w, 10, 0., 5., "#Delta R(l2,j)"); 
      }
      FillHist( label_1+ "/"+ label_1 + "_jet_lep1_dr_"+ label_2,  jets[i].DeltaR(els[0]) , w, 10, 0., 5., "#Delta R(l1,j)") ;
      FillHist( label_1+ "/"+ label_1 + "_jet_lep2_dr_"+ label_2,  jets[i].DeltaR(els[1]) , w, 10, 0., 5., "#Delta R(l2,j)");

    }
    if(channel == MuMu ){
      if(jets[i].DeltaR(mus[0]) < mindRelj1) mindRmuj1=jets[i].DeltaR(mus[0]);
      if(jets[i].DeltaR(mus[0]) < mindRelj2) mindRmuj2=jets[i].DeltaR(mus[1]);

      if(i == 0){
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep1_dr_"+ label_2,  jets[i].DeltaR(mus[0]) , w, 10, 0., 5., "#Delta R(l1,j)") ;
	FillHist( label_1+ "/"+ label_1 + "_jet1_lep2_dr_"+ label_2,  jets[i].DeltaR(mus[1]) , w, 10, 0., 5., "#Delta R(l2,j)");
      }
      FillHist( label_1+ "/"+ label_1 + "_jet_lep1_dr_"+ label_2,  jets[i].DeltaR(mus[0]) , w, 10, 0., 5., "#Delta R(l1,j)") ;
      FillHist( label_1+ "/"+ label_1 + "_jet_lep2_dr_"+ label_2,  jets[i].DeltaR(mus[1]) , w, 10, 0., 5., "#Delta R(l2,j)");

    }
    FillHist( label_1+"/"+label_1+"_DeltaRJ_MET_" + label_2,  jets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(J,met)");
  }
  if(channel == EE ) FillHist( label_1+ "/"+ label_1 + "_jet_lep1_mindr_"+ label_2, mindRelj1  , w, 10, 0., 5., "#Delta R(l1,j)") ;
  if(channel == EE ) FillHist( label_1+ "/"+ label_1 + "_jet_lep2_mindr_"+ label_2, mindRelj2  , w, 10, 0., 5., "#Delta R(l1,j)") ;
  if(channel == MuMu ) FillHist( label_1+ "/"+ label_1 + "_jet_lep1_mindr_"+ label_2, mindRmuj1  , w, 10, 0., 5., "#Delta R(l1,j)") ;
  if(channel == MuMu ) FillHist( label_1+ "/"+ label_1 + "_jet_lep2_mindr_"+ label_2, mindRmuj2  , w, 10, 0., 5., "#Delta R(l1,j)") ;

  if(channel ==EE ) {
    FillHist( label_1+ "/"+ label_1 + "_sumlep_eta_"+ label_2, (fabs(els[0].Eta())+  fabs(els[1].Eta())) , w, 10, 0., 5.);
    if(fabs(els[0].Eta()) > fabs(els[1].Eta()))    FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(els[0].Eta()) , w, 10, 0., 2.5);
    else  FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(els[1].Eta()) , w, 10, 0., 2.5);
  }
  if(channel == MuMu ) {
    FillHist( label_1+ "/"+ label_1 + "_sumlep_eta_"+ label_2, (fabs(mus[0].Eta())+  fabs(mus[1].Eta())) , w, 10, 0., 5.);
    if(fabs(mus[0].Eta()) > fabs(mus[1].Eta()))    FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(mus[0].Eta()) , w, 10, 0., 2.5);
    else  FillHist( label_1+ "/"+ label_1 + "_maxlep_eta_"+ label_2, fabs(mus[1].Eta()) , w, 10, 0., 2.5);
  }


  for(unsigned int i=0; i < fatjets.size(); i++){
    FillHist( label_1+ "/"+ label_1 + "_fatjet_pt_"+ label_2,  fatjets[i].Pt() , w, 100, 0., 2000., "AK8 Jet p_{T} GeV");
    FillHist( label_1+ "/"+ label_1 + "_fatjet_sdmass_"+ label_2,  fatjets[i].SDMass() , w, 100, 0., 500., "Mass_{softdrop} GeV");
    FillHist( label_1+ "/"+ label_1 + "_fatjet_tau21_"+ label_2,  fatjets[i].PuppiTau2()/ fatjets[i].PuppiTau1() , w, 400, 0., 2., "#tau_{21}");
  
    if(channel == EE ){
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep1_dr_"+ label_2,  fatjets[i].DeltaR(els[0]) , w, 10, 0., 5., "#Delta R(l1,fj)") ;
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep2_dr_"+ label_2,  fatjets[i].DeltaR(els[1]) , w, 10, 0., 5., "#Delta R(l2,fj)");
    }
    if(channel == MuMu ){
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep1_dr_"+ label_2,  fatjets[i].DeltaR(mus[0]) , w, 10, 0., 5., "#Delta R(l1,fj)") ;
      FillHist( label_1+ "/"+ label_1 + "_fatjet_lep2_dr_"+ label_2,  fatjets[i].DeltaR(mus[1]) , w, 10, 0., 5., "#Delta R(l2,fj)");
    }
    FillHist( label_1+"/"+label_1+"_DeltaRFJ_MET_" + label_2,  fatjets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(FJ,met)");

  }

  
  
  if(channel == EE ){
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

double HNtypeI_Dilepton::MT(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
  
}


std::vector<Jet> HNtypeI_Dilepton::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

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


void HNtypeI_Dilepton::FillEventCutflow(HNtypeI_Dilepton::ChargeType charge_i, HNtypeI_Dilepton::Flavour cf,float wt, TString cut,   TString label){

  // cf lables what hist to fill
  // charge_i :+1 means SS, -1 means OS ; 0 means OS/SS
  
  //cf = 0 DiEl EXO17-028 cutflow
  //cf = 1 DiMu EXO17-028 cutflow

  // DiEl EXO17-028                                                                                                                                                              
  
  //  cout << charge_i << " " << cf << " " << cut << " " << label << endl;
  if(cf == EE){
    TString charge_ = (charge_i == SS) ? "same_sign" : "opposite_sign";
    vector<TString> labels_ss = {"NoCut", "METFilter", "Trigger_el", "Dilep_el_same_sign","Lep_veto_el_same_sign", "Diel_mass_same_sign", "Jet_req_same_sign", "noZ_same_sign","MET_same_sign","BJet_same_sign","HighMassJet_same_sign","HighMassSR_same_sign", "Highmass_SR1", "Highmass_SR2"};
    vector<TString> labels_os = {"NoCut", "METFilter", "Trigger_el", "Dilep_el_opposite_sign","Lep_veto_el_opposite_sign","Diel_mass_opposite_sign","Jet_req_opposite_sign", "noZ_opposite_sign","MET_opposite_sign","BJet_opposite_sign","HighMassJet_opposite_sign", "HighMassSR_opposite_sign" ,"Highmass_SR3", "Highmass_SR4"};
    if(charge_i == SS)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_"+charge_, wt, labels_ss, cut);
    else if(charge_i == OS)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_"+charge_, wt, labels_os, cut);
    else {
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_same_sign", wt, labels_ss, cut);
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_opposite_sign", wt, labels_os, cut);
    }
    return;
  }

  // DiMu EXO17-028                                                                                                                                                                                                                                                                                           
  if(cf == MuMu){
    TString charge_ = (charge_i == SS) ? "same_sign" : "opposite_sign";
    vector<TString> labels_ss = {"NoCut", "METFilter", "Trigger_mu", "Dilep_mu_same_sign","Lep_veto_mu_same_sign", "Dimu_mass_same_sign", "Jet_req_same_sign", "MET_same_sign","BJet_same_sign","HighMassJet_same_sign" ,"HighMassSR_same_sign","Highmass_SR1", "Highmass_SR2"};
    vector<TString> labels_os = {"NoCut", "METFilter", "Trigger_mu", "Dilep_mu_opposite_sign","Lep_veto_mu_opposite_sign","Dimu_mass_opposite_sign","Jet_req_opposite_sign", "MET_opposite_sign","BJet_opposite_sign","HighMassJet_opposite_sign","HighMassSR_opposite_sign", "Highmass_SR3", "Highmass_SR4"};
    if(charge_i == SS)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_"+charge_, wt, labels_ss, cut);
    else if(charge_i == OS)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_"+charge_, wt, labels_os, cut);
    else {
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_same_sign", wt, labels_ss, cut);
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_dimu_opposite_sign", wt, labels_os, cut);
    }
    return;
  }

  // DiMu SR1                                                                                                                     
  return;
}        
void HNtypeI_Dilepton::FillEventCutflow(HNtypeI_Dilepton::ChargeType charge_i,Region cf,float wt, TString cut,   TString label){                                                            
  vector<TString> masses = {"100","125","200","250","300","400","500","600","700","800","900","1000","1100","1200","1300","1400","1500","1700","2000"};
  
  if(cf == MuMu_SR1){

    vector<TString> labels;
    
    if(charge_i  == SS ) {
 
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR1_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR1", wt, labels, cut);
    }
    else {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR3_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR3", wt, labels, cut);

    }
  }

  // DiEL SR1/3                                                                                                                                                                                                                                                                                               
  if(cf== EE_SR1){
    vector<TString> labels;

    if(charge_i  == SS ) {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR1_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_EE_SR1", wt, labels, cut);
    }
    else{
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR3_mn"+masses[x] );

      AnalyzerCore::FillEventCutflow(label+"exo_17_028_EE_SR3", wt, labels, cut);

    }
  }
  if(cf== MuMu_SR2){
    vector<TString> labels;
    if(charge_i  == SS ) {
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR2_mn"+masses[x] );

      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR2", wt, labels, cut);
    }
    else{
      for (unsigned int x=0; x < masses.size(); x++) labels.push_back("SR4_mn"+masses[x] );
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_MuMu_SR4", wt, labels, cut);

    }
  }
  if(cf== EE_SR2){
    vector<TString> labels;

    if(charge_i  == SS ) {
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

void HNtypeI_Dilepton::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNtypeI_Dilepton::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

