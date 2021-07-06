#include "HNDilepton.h"

void HNDilepton::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  // dataset
  isSingleMu = HasFlag("isSingleMu");
  isEE = HasFlag("isEE");
  isMM = HasFlag("isMM");
  if(isSingleMu) isMM=HasFlag("isSingleMu");

  for(int i = 0 ; i < 100; i++){
    
    //int i = std::stoi(s);
    //nJob = GetIntFlag();
  }
  // bkg flags 
  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");

  // signal flags 
  Signal = HasFlag("Signal");
  SignalOS = HasFlag("SignalOS");
  if(SignalOS) Signal=SignalOS;
  
  IncludeFakeLepton = HasFlag("IncludeFakeLepton");
  
  /// Other flags
  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  HEM1516 = HasFlag("HEM1516");

  ///// random number for fake systs
  rand_ = new TRandom3(1234);

  cout << "[HNDilepton::initializeAnalyzer()] RunFake = " << RunFake << endl;
  cout << "[HNDilepton::initializeAnalyzer()] RunCF = " << RunCF << endl;
  cout << "[HNDilepton::initializeAnalyzer()] RunSyst = " << RunSyst << endl;
  cout << "[HNDilepton::initializeAnalyzer()] IncludeFakeLepton = " << IncludeFakeLepton << endl;


  // clear trigger lists 
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

    TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PileupRate13TeV_"+TString::Itoa(DataYear,10)+".root";
    TString PUhname = "PUReweight";
    //==== Only for 2016 FastSim, use different one.                                                                              
    if(DataYear==2016 && IsFastSim){
      PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
    }

    cout << "[HNtypeI_JA::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] PUhname = " << PUhname << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] Signal = " << Signal << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] SignalOS = " << SignalOS << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);


  }

}


void HNDilepton::executeEvent(){

  Event ev = GetEvent();
  FillHist( "Nevents", 0.5, weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight(), 1, 0., 1,"");

  //cout << "RunNumber:EventNumber = " << run << ":" << event << endl;
  
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
    //N_VTX = nPileUp+1;                                                                                                          
    //==== but histogram is as a function of nPileUp not nVTX                                                                     
    //int nPileUpForBin = nPileUp;                                                                                                
    //if(DataYear==2018) nPileUpForBin = max(5, nPileUpForBin);                                                                   
    if(N_VTX < 5) N_VTX=5;
    if(N_VTX > 59) N_VTX=59;
    int bin_pu = hist_PUReweight->FindBin(N_VTX);
    weight_PU = hist_PUReweight->GetBinContent(bin_pu);

    //weight_PU_Up = hist_PUReweight_Up->GetBinContent(bin_pu);                                                                   
    //weight_PU_Down = hist_PUReweight_Down->GetBinContent(bin_pu);                                                               

  }


  //************************************************/// 
  // setup vector of all objects
  //************************************************///
  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllJets = GetAllJets();
  AllFatJets = puppiCorr->Correct(GetAllFatJets());
  
  bool run_Debug = false;
  if(run_Debug){
    for(auto i : AllMuons){
      cout << "Muon " << i.Pt() << " " << i.Eta() << " " << i.PassID("HNLoosest")<<  endl;
      cout << "Type " <<  GetLeptonType(i, gens) << endl;

    }
    
    for(auto i : AllElectrons){
      cout << "Electron " << i.Pt() << " " <<i.Eta() << " " << i.PassID("HNLoosest") << " " << i.PassID("HNVeto2016") << " " << i.PassID("HNMediumV1") <<  endl;
    }
  }

  //==== Central

  AnalyzerParameter param;


  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ELIDs;
  vector<TString>  mu_loose_id;  vector<TString>  el_loose_id;
  vector<HNDilepton::Flavour> channel;

  if(isEE||!IsDATA){

    ELIDs.push_back(make_pair("HNVeto2016", "HNVeto2016"));    el_loose_id.push_back("HNLoose2016");

    for (auto i: ELIDs) { 
      channel.push_back(EE);
      MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
      mu_loose_id.push_back("HNLoose2016");
    }
  }
  if(isMM||!IsDATA){


    /*
    // =========  HN POG Mix  ================================ //  
    //  mu1,2 pt > 60  HighPt ID
    // else POGTightPFIsoVeryVeryTight

    //MuIDs.push_back(make_pair("POGHighPtMixTight", "HNVeto2016"));
    //mu_loose_id.push_back("POGHighPtMixLoose"); // ---> FIX FR PT BIN

    
    // =========  HN POG Tight    ================================ //                                                                                                                                           
    // POGTightPFIsoVeryTight : IP < 3. | iso < 0.10 | POGTight
    MuIDs.push_back(make_pair("POGTightPFIsoVeryTight","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("POGTightPFIsoVeryVeryTight","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("POGTightPFIsoTight","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("POGTightPFIsoMedium","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("POGTightPFIsoLoose","HNVeto2016")); mu_loose_id.push_back("HNLooseV1");

    MuIDs.push_back(make_pair("HNTightPFIsoVeryTight","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTightPFIsoVeryVeryTight","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTightPFIsoTight","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTightPFIsoMedium","HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTightPFIsoLoose","HNVeto2016")); mu_loose_id.push_back("HNLooseV1");


    // =========  HN POG Tight IDS  ================================ //                                                                                                                                     
    // POG Tight                                                                                                                                                                                            
    // Pass_HNTight(iso/pt, |dxy|, |dz|,|IP3D|)

    // == HNTightV1
    // Pass_HNTight(0.07, 0.01, 0.1,3.)                    
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_02_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_02_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_02_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_05_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_05_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_05_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_20_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_20_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso10_dxy_20_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");


    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_02_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_02_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_02_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_05_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_05_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_05_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_20_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_20_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso09_dxy_20_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");



    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_02_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_02_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_02_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_05_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_05_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_05_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");

    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_20_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_20_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso08_dxy_20_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");



    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_02_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_02_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_02_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_05_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_05_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_05_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_20_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_20_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_20_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");


    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_02_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_02_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_02_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_05_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_05_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_05_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_20_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_20_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso06_dxy_20_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");


    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_02_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_02_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_02_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_05_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_05_dz_01_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_05_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_05_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_20_ip_3", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_20_ip_4", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");
    MuIDs.push_back(make_pair("HNTight_Iso05_dxy_20_ip_999", "HNVeto2016"));    mu_loose_id.push_back("HNLooseV1");


    // EXO-17-028
    MuIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    // HighPt ID
    MuIDs.push_back(make_pair("POGHighPtTight", "HNVeto2016"));
    mu_loose_id.push_back("POGHighPtLoose");

    int iel= MuIDs.size() - channel.size();
    for (int i =0; i < iel; i++){
      channel.push_back(MuMu);
      el_loose_id.push_back("HNLoose2016");
      ELIDs.push_back(make_pair("HNVeto2016", "HNVeto2016"));
    }
    */
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
    param.Name = "HNDilepton_"+ElectronTightID;
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

    if(channel[it_id]==MuMu)param.Name = "HNDilepton_MuMu_" +Tight_ID;
    if(channel[it_id]==EE)  param.Name = "HNDilepton_EE_" +Tight_ID;
    //    if(channel[it_id]==EMu) param.Name = "HNDilepton_EMU_" +Tight_ID;
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  }
    
}





void HNDilepton::executeEventFromParameter(AnalyzerParameter param, TString Tight_ID, HNDilepton::Flavour channel){
  
  // make default hist

  if(RunFake){
    if( _jentry==0 ){

      vector<TString> id_names={"HNTightV1", "HNTight2016","HNTight2016Update","HNVeto2016"};    
      for(unsigned int i4=0; i4 < 4; i4++){//                                                                                                                                                 
	for(unsigned int i5=0; i5 < 5; i5++){
	  for(unsigned int i6=0; i6 < 5; i6++){
	    for(unsigned int i7=0; i7 < 3; i7++){
	      
	      double dz = 0.04 + 0.02*double(i4);           std::string _dz= std::to_string(dz);
	      double dxy_b = 0.01 + 0.01*double(i5);           std::string dxyb= std::to_string(dxy_b);
	      double dxy_e = 0.01 + 0.01*double(i6);           std::string dxye= std::to_string(dxy_e);
	      double ip = 3 + 1.*double(i7);           std::string _ip= std::to_string(ip);
	      
	      for(unsigned int j=0; j < 13.; j++){
		
		double mva_1 = 0.65;          std::string mva1= std::to_string(mva_1);
		double mva_2 = 0.61 + 0.02*double(j);          std::string mva2= std::to_string(mva_2);
		double mva_3 = 0.89;           std::string mva3= std::to_string(mva_3);
		for(unsigned int i2=0; i2 < 7; i2++){
		  for(unsigned int i3=0; i3 < 5; i3++){
		    double iso_b = 0.07 + 0.005*double(i2);           std::string isob= std::to_string(iso_b);
		    double iso_e = 0.05 + 0.01*double(i3);           std::string isoe= std::to_string(iso_e);
		    if(j<11) id_names.push_back("mva1_" + mva1 +"_mva2_"+mva2+"_mva3_"+mva3+"_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_b_"+isob+"_iso_e_"+isoe);
		    if(j==11) id_names.push_back("pogmedium_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_b_"+isob+"_iso_e_"+isoe);
		    if(j==12) id_names.push_back("pogtight_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_b_"+isob+"_iso_e_"+isoe);
		  }
		}
		if(j<11)  id_names.push_back("mva1_" + mva1 +"_mva2_"+mva2+"_mva3_"+mva3+"_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_pogmedium");
		if(j<11)  id_names.push_back("mva1_" + mva1 +"_mva2_"+mva2+"_mva3_"+mva3+"_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_pogtight");
		if(j==11) id_names.push_back("mediumpog_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_pogmedium");
		if(j==12) id_names.push_back("tightpog_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_pogtight");
	      }
	    }
	  } // mva1                                                                                                                                                                       
	}
      }
      
      AnalyzerCore::FillEventCutflow("Fullopt_testhist_events", 1., id_names, "HNTightV1");                                                                                                           
    }
    return;
  }

  bool     IsEE      =  (channel == EE);
  TString  channel_s =  (IsEE)  ? "EE" : "MuMu";
  
  Event ev = GetEvent();
  double weight = 1.;
  //************************************************///
  //************************************************///
  // MC weights initial settings
  // -- set to full trigger luminosity for data taking year
  // -- apply MC gen weights
  // -- apply prefire weight
  if(!IsDATA){
    
    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();
    weight *= weight_Prefire;
  }
  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon>     this_AllMuons     = AllMuons;
  vector<Jet>      this_AllJets      = AllJets;
  vector<FatJet>   this_AllFatJets   = AllFatJets;

  if(!IsDATA){

    double this_pureweight       = 1.;
    this_pureweight = weight_PU;

    weight *= this_pureweight;
  }
  
  //************************************************///   
  // 
  // setup analysis level lepton vectors
  //
  //************************************************///   
  
  TString elID =  param.Electron_Tight_ID;
  TString muID =  param.Muon_Tight_ID;

  /// Get collecton of temporary leptons
  double ptel=15.;

  std::vector<Electron>  tmp_myelectrons = SelectElectrons(this_AllElectrons, elID, ptel, 2.5);
  
  double ptmuon = 10.;
  std::vector<Muon>      tmp_mymuons     = SelectMuons    (this_AllMuons,     muID, ptmuon, 2.4);


  /// declare main collection of leptons
  std::vector<Muon>         mymuons    ;
  std::vector<Electron>     myelectrons ;
  
  std::sort(tmp_mymuons.begin(), tmp_mymuons.end(), PtComparing);

  if(Signal||IncludeFakeLepton){
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


  std::vector<FatJet>   fatjets_tmp  = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 5.);
  std::vector<Jet>      jets_tmp     = SelectJets(this_AllJets, param.Jet_ID, 20., 2.7);


  std::sort(fatjets_tmp.begin(), fatjets_tmp.end(), PtComparing);

  map<TString,   std::vector<FatJet> > fatjet_map;
  fatjet_map["ak8_type1"]                      = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1., true, -999, true, 40., 130., veto_electrons, veto_muons);
  std::vector<FatJet> fatjets                  = GetAK8Jets(fatjets_tmp, 200., 2.7, true,  1., true, -999, true, 40., 130., veto_electrons, veto_muons);
  std::sort(jets_tmp.begin(), jets_tmp.end(), PtComparing);
  
  map<TString,   std::vector<Jet> > jet_map;

  std::vector<Jet> jets      = GetAK4Jets(jets_tmp, 20., 2.7, true,  0.4,  0.8,"", veto_electrons, veto_muons,fatjets);
  jet_map["ak4_type1"]                   = GetAK4Jets(jets_tmp, 20., 2.7, true,  0.4,  0.8,"", veto_electrons, veto_muons,fatjets);
  

  //************************************************///   
  // select trigger
  //************************************************///   
  
  if(IsEE){
    
    if(veto_electrons.size() != 2) return;
    if(veto_electrons[0].Charge() !=  veto_electrons[1].Charge() ) return;
    RunElectron("AllElectrons", this_AllElectrons,veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);

    vector<TString> id_types = {"Medium","Tight"};
    bool runID=true;
    if(runID){

      int n_tot = 143704;

      vector<TString> id_names2;
      vector<TString> el_ids_ = {"HNTightV1", "HNTight2016","HNTight2016Update","HNVeto2016"};
      
      float n_it=0.1;
      for(auto _el_id : el_ids_)  
	{
	  RunElectron(n_it,n_tot, _el_id,id_names2,"FullOpt_StandardIDs",SelectElectrons(this_AllElectrons, _el_id, 10., 2.5),veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	  n_it = n_it+1.;
	}

      // loop over IP first
      for(unsigned int i4=0; i4 < 4; i4++){//
	for(unsigned int i5=0; i5 < 5; i5++){
	  for(unsigned int i6=0; i6 < 5; i6++){
	    for(unsigned int i7=0; i7 < 3; i7++){
	      double dz = 0.04 + 0.02*double(i4);           std::string _dz= std::to_string(dz);
	      double dxy_b = 0.01 + 0.01*double(i5);           std::string dxyb= std::to_string(dxy_b);
	      double dxy_e = 0.01 + 0.01*double(i6);           std::string dxye= std::to_string(dxy_e);
	      double ip = 3 + 1.*double(i7);           std::string _ip= std::to_string(ip);
	      
	      // then loop over ID ; mva + pog  ; 11 mva + 2 pog
	      for(unsigned int j=0; j < 13.; j++){
	
		double mva_1 = 0.65 ;          std::string mva1= std::to_string(mva_1);
		double mva_3 = 0.89;           std::string mva3= std::to_string(mva_3);
		double mva_2 = 0.61 + 0.02*double(j);          std::string mva2= std::to_string(mva_2); // only loop over OB
		
		// loop over iso/mini iso range i2/i3 
		int id_version = 0; // mva
		if(j == 11) id_version = 1; //pog medium 
		if(j == 12) id_version = 2; //pog tight 

		for(unsigned int i2=0; i2 < 7; i2++){
		  for(unsigned int i3=0; i3 < 5; i3++){//                                                                                                                                                                         			
		    double iso_b = 0.07 + 0.005*double(i2);           std::string isob= std::to_string(iso_b);
		    double iso_e = 0.05 + 0.01*double(i3);           std::string isoe= std::to_string(iso_e);
		    if(id_version==0){
		      param.Name = "mva1_" + mva1 +"_mva2_"+mva2+"_mva3_"+mva3+"_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_b_"+isob+"_iso_e_"+isoe;

		      vector<Electron> els,els2;
		      for(auto el : myelectrons){
		
			if(!el.PassMVA(mva_1,mva_2,mva_3)) continue;
			if(el.passIDHN(4,dxy_b, dxy_e, dz, dz, ip,ip, -999., -999., iso_b, iso_e))  els.push_back(el);
			if(el.passIDHN(3,dxy_b, dxy_e, dz, dz, ip,ip, iso_b, iso_e, -999., -999.))                         els2.push_back(el);
		      }
		      RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_Miniiso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight); 
		      RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_iso", els2, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight); 
		    }
		    else if(id_version==2){
		      param.Name = "pogtight_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_b_"+isob+"_iso_e_"+isoe;
		      vector<Electron> els,els2;
		      for(auto el : myelectrons){
			if(el.passIDHN(1,dxy_b, dxy_e, dz, dz, ip,ip, -999., -999., iso_b, iso_e))  els.push_back(el);
			if(el.passIDHN(1,dxy_b, dxy_e, dz, dz, ip,ip, iso_b, iso_e, -999., -999.))els2.push_back(el);
		      }
		      RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_Miniiso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		      RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_iso", els2, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    }
		    else if(id_version==1){
		      param.Name = "pogmedium_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_b_"+isob+"_iso_e_"+isoe;
		      vector<Electron> els,els2;
		      
		      for(auto el : myelectrons){
			if(el.passIDHN(2,dxy_b, dxy_e, dz, dz, ip,ip, -999., -999., iso_b, iso_e)) els.push_back(el);
			if(el.passIDHN(2,dxy_b, dxy_e, dz, dz, ip,ip, iso_b, iso_e, -999., -999.))els2.push_back(el);
		      }		      
		      RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_Miniiso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		      RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_iso", els2, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    }
		    // n_it increased to fill next bin
		    n_it = n_it +1.;
		  }// iso b                                          
		} //iso endcap
		    
		// now test with POG IDs 
		if(id_version==0){
		  if(1){
		    vector<Electron> els;
		    for(auto el : myelectrons){
		      if(!el.passIDHN(3,dxy_b, dxy_e, dz, dz, ip,ip, 999., 999.,999., 999.)) continue;
		      if(!el.PassMVA(mva_1,mva_2,mva_3)) continue;
		      els.push_back(el);
		    }
		    
		    RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_iso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_Miniiso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    n_it = n_it +1.;
		    
		  }
		  // Tight POG Iso
		  if(1){
		    param.Name = "mva1_" + mva1 +"_mva2_"+mva2+"_mva3_"+mva3+"_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_pogtight";
		    
		    vector<Electron> els;
		    
		    for(auto el : myelectrons){
		      if(!el.passIDHN(3,dxy_b, dxy_e, dz, dz, ip,ip, -999., -999.,-999., -999.)) continue;
		      if(!el.PassMVA(mva_1,mva_2,mva_3)) continue;
		      els.push_back(el);
		    }
		    
		    RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_iso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_Miniiso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    n_it = n_it +1.;

		  }
		} // mva ID
		
                if(id_version==2){
		  // tight POG Iso                                                                                                                                                                                                                                   
		  if(1){
		    param.Name = "tightog_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_pogtight";
		    
		    vector<Electron> els;
		    
		    for(auto el : myelectrons){
		      if(!el.passIDHN(1,dxy_b, dxy_e, dz, dz, ip,ip, -999., -999.,-999., -999.)) continue;
		      els.push_back(el);
		    }
		    
		    RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_iso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_Miniiso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		    n_it = n_it +1.;
		    
		  }
		} // mva ID                          
		
                if(id_version==1){
		  // Medium POG Iso                                                                                                                                                                                                                                  
		  
		  param.Name = "mediumpog_dxy_b_"+dxyb+"_dxy_e_"+dxye+"_dz_"+_dz+"_ip_"+_ip+"_iso_pogmedium";
		  
		  vector<Electron> els;
		  
		  for(auto el : myelectrons){
		    if(!el.passIDHN(2,dxy_b, dxy_e, dz, dz, ip,ip, -999., -999.,-999., -999.)) continue;
		    els.push_back(el);
		  }
		  
		  RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_iso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		  RunElectron (n_it,n_tot,param.Name, id_names2,"FullOpt_Miniiso", els, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		  n_it = n_it +1.;
		  
		} // pog ID            
		
	      } // mva + pog
		  
	    }
	  }
	}
      }
    }

    return;

    goto fullopt;
    
    for(auto id_type : id_types){

      // run POG but vary 
      // IP  3 - 7
      // ISO 0.05 - 1
      // dxy 0.01 - 0.1
      // dz 0.02 - 0.1
      //      if(!el.passIDHN(4,0.01, 0.01, 0.05,0.04, 4.9,4.9, -999., -999., iso_b, 0.05)) continue;

      //-----------------------------------------------
      // VERSION 1 POG * IP
      //-----------------------------------------------
      
      int i_id_type = (id_type=="Medium") ? 2 : 1;
      double n_ip_it=70.;
      for(unsigned int k=0; k < n_ip_it+1; k++){
        double var = 3. + (10 - 3.)*double(k)/n_ip_it;
	std::string svar= std::to_string(var);
	
	vector<Electron> els_hnB;
	vector<Electron> els_hntightB;

        vector<Electron> els_hnE;
        vector<Electron> els_hntightE;
	for(auto el : myelectrons){
	  if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2,var,4., -999., -999., -999., -999.)) els_hnB.push_back(el);
	  if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2, 4.,var, -999., -999., -999., -999.)) els_hnE.push_back(el);
	  if(el.passIDHN(i_id_type,0.01, 0.01, 0.05,0.05, var,4.9, -999., -999., 0.08, 0.08)) els_hntightB.push_back(el);
	  if(el.passIDHN(i_id_type,0.01, 0.01, 0.05,0.05, 4.9, var, -999., -999., 0.08, 0.08)) els_hntightE.push_back(el);
	}

	param.Name = "HNDilepton_EE_HN"+id_type+"_ipB"  + svar;	RunElectron(param.Name,els_hnB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	param.Name = "HNDilepton_EE_HN"+id_type+"_ipE"  + svar;	RunElectron(param.Name,els_hnE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HNTight"+id_type+"_ipB"  + svar;	RunElectron(param.Name,els_hntightB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HNTight"+id_type+"_ipE"  + svar;	RunElectron(param.Name,els_hntightE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
      }
 
      //-----------------------------------------------                                                                                                                                                             
      // VERSION 2 POG * DXY                                                                                                                                                                                         
      //-----------------------------------------------       

      double n_dxy_it=99.;
      for(unsigned int k=0; k < n_dxy_it+1; k++){
        double var = 0.01 + (0.1 - 0.01)*double(k)/n_dxy_it;
	std::string svar= std::to_string(var);

        vector<Electron> els_hnB;
        vector<Electron> els_hntightB;

        vector<Electron> els_hnE;
        vector<Electron> els_hntightE;
        for(auto el : myelectrons){
          if(el.passIDHN(i_id_type,var, 0.1, 0.1,0.2,5.,4., -999., -999., -999., -999.)) els_hnB.push_back(el);
          if(el.passIDHN(i_id_type,0.05, var, 0.1,0.2, 4.,4., -999., -999., -999., -999.)) els_hnE.push_back(el);
          if(el.passIDHN(i_id_type,var, 0.01, 0.05,0.05, 4.9,4.9, -999., -999., 0.08, 0.08)) els_hntightB.push_back(el);
          if(el.passIDHN(i_id_type,0.01, var, 0.05,0.05, 4.9, 4.9, -999., -999., 0.08, 0.08)) els_hntightE.push_back(el);
        }

        param.Name = "HNDilepton_EE_HN"+id_type+"_dxyB"  + svar; RunElectron(param.Name,els_hnB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HN"+id_type+"_dxyE"  + svar; RunElectron(param.Name,els_hnE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	param.Name = "HNDilepton_EE_HNTight"+id_type+"_dxyB"  + svar;    RunElectron(param.Name,els_hntightB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	param.Name = "HNDilepton_EE_HNTight"+id_type+"_dxyE"  + svar;    RunElectron(param.Name,els_hntightE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
      }
      
      double n_dz_it=99.;
      for(unsigned int k=0; k < n_dz_it+1; k++){
        double var = 0.02 + (0.2 - 0.02)*double(k)/n_dz_it;
	std::string svar= std::to_string(var);

        vector<Electron> els_hnB;
        vector<Electron> els_hntightB;

        vector<Electron> els_hnE;
        vector<Electron> els_hntightE;
        for(auto el : myelectrons){
          if(el.passIDHN(i_id_type,0.05, var, 0.1,0.2,5.,4., -999., -999., -999., -999.)) els_hnB.push_back(el);
          if(el.passIDHN(i_id_type,0.05, 0.1, var,0.2, 4.,4., -999., -999., -999., -999.)) els_hnE.push_back(el);
          if(el.passIDHN(i_id_type,0.01, 0.01, var,0.05, 4.9,4.9, -999., -999., 0.08, 0.08)) els_hntightB.push_back(el);
          if(el.passIDHN(i_id_type,0.01, 0.01, 0.05,var, 4.9, 4.9, -999., -999., 0.08, 0.08)) els_hntightE.push_back(el);
        }

        param.Name = "HNDilepton_EE_HN"+id_type+"_dzB"  + svar; RunElectron(param.Name,els_hnB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HN"+id_type+"_dzE"  + svar; RunElectron(param.Name,els_hnE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HNTight"+id_type+"_dzB"  + svar;    RunElectron(param.Name,els_hntightB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HNTight"+id_type+"_dzE"  + svar;    RunElectron(param.Name,els_hntightE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
      }


      
      double n_iso_it=20.;
      for(unsigned int k=0; k < n_iso_it+1; k++){
        double var = 0.05 + (0.1 - 0.05)*double(k)/n_iso_it;
	std::string svar= std::to_string(var);

        vector<Electron> els_hnB;
        vector<Electron> els_hntightB;
        vector<Electron> els_hnE;
        vector<Electron> els_hntightE;

        vector<Electron> els_hnMB;
        vector<Electron> els_hnMtightB;
        vector<Electron> els_hnME;
        vector<Electron> els_hnMtightE;
        for(auto el : myelectrons){
          if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2,4,4., var ,-999., -999., -999.)) els_hnB.push_back(el);
          if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2, 4.,4., -999., var, -999., -999.)) els_hnE.push_back(el);
          if(el.passIDHN(i_id_type,0.01, 0.01, 0.05,0.05, 4.9,4.9, var, 0.08, -999.,-999.)) els_hntightB.push_back(el);
          if(el.passIDHN(i_id_type,0.01, 0.01, 0.05,0.05, 4.9, 4.9, 0.08 ,var,-999.,-999. )) els_hntightE.push_back(el);
	  
	  if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2,4,4., -999. ,-999., var, -999.)) els_hnMB.push_back(el);
          if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., -999., var)) els_hnME.push_back(el);
          if(el.passIDHN(i_id_type,0.01, 0.01, 0.05,0.05, 4.9,4.9,  -999.,-999., var, 0.08)) els_hnMtightB.push_back(el);
	  if(el.passIDHN(i_id_type,0.01, 0.01, 0.05,0.05, 4.9, 4.9,  -999.,-999.,0.08,var)) els_hnMtightE.push_back(el);

        }
      
	if(k==0){
	  vector<Electron> els_hnEpog;
	  vector<Electron> els_hnBpog;
	  for(auto el : myelectrons){
	    if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2,4,4., -999., -999., -999., -999.)) els_hnBpog.push_back(el);
	    if(el.passIDHN(i_id_type,0.05, 0.1, 0.1,0.2, 4.,4., -999.,-999., -999., -999.)) els_hnEpog.push_back(el);
	  }
	  param.Name = "HNDilepton_EE_HN"+id_type+"_isoB" ;	  RunElectron(param.Name,els_hnBpog, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	  param.Name = "HNDilepton_EE_HN"+id_type+"_isoE";	  RunElectron(param.Name,els_hnEpog, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
          param.Name = "HNDilepton_EE_HN"+id_type+"_misoB" ;       RunElectron(param.Name,els_hnBpog, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
          param.Name = "HNDilepton_EE_HN"+id_type+"_misoE";        RunElectron(param.Name,els_hnEpog, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	}
      
	param.Name = "HNDilepton_EE_HN"+id_type+"_isoB"  + svar;	RunElectron(param.Name,els_hnB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	param.Name = "HNDilepton_EE_HN"+id_type+"_isoE"  + svar;        RunElectron(param.Name,els_hnE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HNTight"+id_type+"_isoB"  + svar;        RunElectron(param.Name,els_hntightB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HNTight"+id_type+"_isoE"  + svar;        RunElectron(param.Name,els_hntightE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);

	param.Name = "HNDilepton_EE_HN"+id_type+"_misoB"  + svar;        RunElectron(param.Name,els_hnMB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HN"+id_type+"_misoE"  + svar;        RunElectron(param.Name,els_hnME, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
        param.Name = "HNDilepton_EE_HNTight"+id_type+"_misoB"  + svar;        RunElectron(param.Name,els_hnMtightB, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	param.Name = "HNDilepton_EE_HNTight"+id_type+"_misoE"  + svar;        RunElectron(param.Name,els_hnMtightE, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);

      } 
    }
    if(1){
      
      for(unsigned int i=0; i < 21; i++){
	for(unsigned int j=0; j < 21; j++){
	  for(unsigned int k=0; k < 16; k++){
	    double mva_1 = 0.7 + 0.01*double(i);          std::string mva1= std::to_string(mva_1);
	    double mva_2 = 0.7 + 0.01*double(j);          std::string mva2= std::to_string(mva_2);
	    double mva_3 = 0.5 + 0.02*double(k);          std::string mva3= std::to_string(mva_3);

	    vector<Electron> els_mva;
	    vector<Electron> els_tight_mva;
	    vector<Electron> els_tight_miso_mva;
	    for(auto el : myelectrons){
	      if(el.passIDHN(3,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999.,  -999., -999.) && el.PassMVA(mva_1,mva_2,mva_3))  els_mva.push_back(el);
	      if(el.passIDHN(3,0.01, 0.01, 0.05,0.05, 4.9,4.9, 0.08, 0.08, -999.,-999.) && el.PassMVA(mva_1,mva_2,mva_3))   els_tight_mva.push_back(el);
	      if(el.passIDHN(4,0.01, 0.01, 0.05,0.05, 4.9,4.9,  -999.,-999.,0.08, 0.08) && el.PassMVA(mva_1,mva_2,mva_3))   els_tight_miso_mva.push_back(el);
	    }
	    param.Name = "HNDilepton_EE_HN_mva" + mva1 + "_mva"+mva2+"_mva"+mva3; RunElectron(param.Name,els_mva, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	    param.Name = "HNDilepton_EE_HNTight_mva" + mva1 + "_mva"+mva2+"_mva"+mva3; RunElectron(param.Name,els_tight_mva, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	    param.Name = "HNDilepton_EE_HNTight_miso_mva" + mva1 + "_mva"+mva2+"_mva"+mva3; RunElectron(param.Name,els_tight_miso_mva, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
	  }
	}
      }
    }
  fullopt:

    if(1){
      int n_it = 0;
      int n_tot = 10*10*10*3*5*3*5*4;
      vector<TString> idnames;
      vector<TString> idnames_miso;
      cout << "_jentry = " << _jentry << endl;
      if(_jentry==0) {
	for(unsigned int i=0; i < 10; i++){
	  for(unsigned int j=0; j < 10; j++){
	    for(unsigned int k=0; k < 10; k++){
	      for(unsigned int i2=0; i2 < 3; i2++){
		for(unsigned int i3=0; i3 < 5; i3++){
		  for(unsigned int i4=0; i4 < 3; i4++){
		    for(unsigned int i5=0; i5 < 5; i5++){
		      for(unsigned int i6=0; i6 < 4; i6++){
			
			double mva_1 = 0.7 + 0.02*double(i);          std::string mva1= std::to_string(mva_1);
			double mva_2 = 0.7 + 0.02*double(j);          std::string mva2= std::to_string(mva_2);
			double mva_3 = 0.5 + 0.025*double(k);           std::string mva3= std::to_string(mva_3);

			double iso_b = 0.07 + 0.01*double(i2);           std::string isob= std::to_string(iso_b);
			double iso_e = 0.05 + 0.01*double(i3);           std::string isoe= std::to_string(iso_e);
			double dz_b = 0.04 + 0.02*double(i4);           std::string dzb= std::to_string(dz_b);
			double dz_e = 0.04 + 0.02*double(i4);           std::string dze= std::to_string(dz_e);
			double dxy_b = 0.01 + 0.01*double(i5);           std::string dxyb= std::to_string(dxy_b);
			double dxy_e = 0.01 + 0.01*double(i5);           std::string dxye= std::to_string(dxy_e);

			double ip = 3 + 1.*double(i2);           std::string _ip= std::to_string(ip);

			n_it++;
			//cout << "Iteration " << n_it  << " / " << n_tot << endl;                                                                                                                       

			param.Name = "HNDilepton_EE_HNMini_mva" + mva1 +"_mva"+mva2+"_mva"+mva3+"_miniiso_b"+isob+ "_miniiso_e"+isoe+"_dz_b"+dzb+"_dz_e"+dze+"_dxy_b"+dxyb+"_dxy_e"+dxye+"_ip"+_ip;
			idnames_miso.push_back(param.Name);			

			param.Name = "HNDilepton_EE_HN_mva" + mva1 +"_mva"+mva2+"_mva"+mva3+"_iso_b"+isob+ "_iso_e"+isoe+"_dz_b"+dzb+"_dz_e"+dze+"_dxy_b"+dxyb+"_dxy_e"+dxye+"_ip"+_ip;
			idnames.push_back(param.Name);

		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
      n_it=0;
      for(unsigned int i=0; i < 10; i++){
        for(unsigned int j=0; j < 10; j++){
          for(unsigned int k=0; k < 10; k++){
	    for(unsigned int i2=0; i2 < 3; i2++){
	      for(unsigned int i3=0; i3 < 5; i3++){
		for(unsigned int i4=0; i4 < 3; i4++){
		  for(unsigned int i5=0; i5 < 5; i5++){
		    for(unsigned int i6=0; i6 < 4; i6++){
		      n_it++;
		      cout << "Run Iteration " << n_it  << " / " << n_tot << endl;
		      double mva_1 = 0.7 + 0.02*double(i);          std::string mva1= std::to_string(mva_1);
		      double mva_2 = 0.7 + 0.02*double(j);          std::string mva2= std::to_string(mva_2);
		      double mva_3 = 0.5 + 0.025*double(k);           std::string mva3= std::to_string(mva_3);
		      
		      double iso_b = 0.07 + 0.01*double(i2);           std::string isob= std::to_string(iso_b);
		      double iso_e = 0.05 + 0.01*double(i3);           std::string isoe= std::to_string(iso_e);
		      double dz_b = 0.04 + 0.02*double(i4);           std::string dzb= std::to_string(dz_b);
		      double dz_e = 0.04 + 0.02*double(i4);           std::string dze= std::to_string(dz_e);
		      double dxy_b = 0.01 + 0.01*double(i5);           std::string dxyb= std::to_string(dxy_b);
		      double dxy_e = 0.01 + 0.01*double(i5);           std::string dxye= std::to_string(dxy_e);

		      double ip = 3 + 1.*double(i2);           std::string _ip= std::to_string(ip);

		      param.Name = "HNDilepton_EE_HNMini_mva" + mva1 +"_mva"+mva2+"_mva"+mva3+"_miniiso_b"+isob+ "_miniiso_e"+isoe+"_dz_b"+dzb+"_dz_e"+dze+"_dxy_b"+dxyb+"_dxy_e"+dxye+"_ip"+_ip;
		      vector<Electron> els;
		      
		      for(auto el : myelectrons){
			if(!el.passIDHN(4,dxy_b, dxy_e, dz_b, dz_e, ip,ip, -999., -999., iso_b, iso_e)) continue;
			if(!el.PassMVA(mva_1,mva_2,mva_3)) continue;
			els.push_back(el);
		      }
		      return;
		      //RunElectron(param.Name,idnames_miso, param.Name,els,veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);

		      param.Name = "HNDilepton_EE_HN_mva" + mva1 +"_mva"+mva2+"_mva"+mva3+"_iso_b"+isob+ "_iso_e"+isoe+"_dz_b"+dzb+"_dz_e"+dze+"_dxy_b"+dxyb+"_dxy_e"+dxye+"_ip"+_ip;

                      vector<Electron> els2;
		      
                      for(auto el : myelectrons){
                        if(!el.passIDHN(3,dxy_b, dxy_e, dz_b, dz_e, ip,ip, iso_b, iso_e, -999., -999.)) continue;
                        if(!el.PassMVA(mva_1,mva_2,mva_3)) continue;
                        els2.push_back(el);
                      }
                      //RunElectron(param.Name, idnames, param.Name,els2,veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
		      
		    }// iso b
		  }
		}
	      }// iso_e   
	    }
	  }
	}
      }
     
 
    }
  }
  return;
  if(!IsEE)  {
    //    PassMM(-1,myelectrons, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
    PassMM(0,myelectrons, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
    PassMM(1,myelectrons, veto_electrons,mymuons,veto_muons,  jet_map, fatjet_map, ev,  param, weight);
  }
  

}




bool HNDilepton::PassMM(int ptbin, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_mm ){
  

  if(ptbin==0) param.Name = param.Name+"_LowPt";
  if(ptbin==1) param.Name = param.Name+"_HighPt";
  HNDilepton::Flavour  flavour  = MuMu;

  if(ptbin==0 && param.Name.Contains("POGHighPtTight")) return false;


  if(muons.size()==2){
    if(ptbin==0){
      if(muons[1].Pt() > 60) return false;
    }
    if(ptbin==1){
      
      if(muons[1].Pt() < 60) return false;
    }
  }
  
  //************************************************///                                                                                                                                                     
  //@@@@ Apply lepton scale factors                                                                                                                                                                         
  //************************************************///                                                                                                                                                     


  /// Get FatJet collection  'ak8_type1'
  std::map<TString, std::vector<FatJet> >::iterator map_fatjet_it = map_fatjets.find("ak8_type1");

  if(map_fatjet_it == map_fatjets.end() ) return false;
  std::vector<FatJet> fatjets = map_fatjet_it->second;


  std::map<TString, std::vector<Jet> >::iterator map_jet_it = map_jets.find("ak4_type1");
  if(map_jet_it == map_jets.end() )return false;
  std::vector<Jet> jets = map_jet_it->second;

  TString label    = param.Name;
  TString channel_s="MuMu";


  bool debug=false;
  if(debug)cout << label << endl;

  /// Get MET l corrected for jet smearing in MC
  Particle METunsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METunsmearedv, jets);
  
  
  if(!PassMETFilter()) return false;
  FillEventCutflow(AllCharge,flavour,weight_mm, "METFilter",param.Name);

  // Apply trigger
  bool PassDoubleMuonTrig = ev.PassTrigger(MuonTriggers);
  bool PassHighPtMuonTrig = ev.PassTrigger(HighPtMuonTriggers);
  // if singlemuon dataset and passes double mu trigger veto


  if(isSingleMu && PassDoubleMuonTrig)  return false;
  if(isSingleMu && !PassHighPtMuonTrig) return false;
  // if MC OR double and Mu50
  if(!IsData && !(PassDoubleMuonTrig|| PassHighPtMuonTrig)) return false;
  
  // Dimuon selection   
  if(muons.size()  != 2 ) return false ;
  if(muons[0].Pt() < 20.) return false;
  if(muons[1].Pt() < 10.) return false;  


  if(muons_veto.size()!=2) return false;
  if(electrons_veto.size() > 0)  return false;

  //************************************************///                                                                                                                                                      
  Particle llCand = muons.at(0) + muons.at(1);

  //************************************************///                                                                                                                          
  
  if (llCand.M() < 10.) return false;
  

  int njets = jets.size() + fatjets.size();
  
  if(njets == 0.) return false;

  bool inZpeak = (llCand.M() < 101. && llCand.M() > 81.) ;

  if(flavour == EE && inZpeak ) return false;

  return true;

}

void HNDilepton::RunHighMassSR(HNDilepton::ChargeType charge_i, TString channel_s,HNDilepton::Flavour flavour, TString charge_s,Particle llCand, TString label, vector<Jet> jets, vector<FatJet> fatjets,  vector<Electron> electrons , vector<Muon> muons, Particle METv, int mPV, double weight_hm, Event ev, AnalyzerParameter param, bool FullAnalysis){

  TString signal_region1 = (charge_s=="same_sign")  ?  "SR1" : "SR3";
  TString signal_region2 = (charge_s=="same_sign")  ?  "SR2" : "SR4";

  /// MAKES EXTENSIVE LIST OF PLOTS
  bool DEBUG(false);


  int njets = jets.size() + fatjets.size();

  
  /// High Mass Region removes 0 jet binsxs

  bool inZpeak = (llCand.M() < 101. && llCand.M() > 81.) ;

  if(flavour == EE && inZpeak ) return;

  
  double ST = GetST(electrons, muons, jets, fatjets, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;

  if(FullAnalysis){
    TString ev_type="";
    if(RunFake) {
      bool lep1_tight(false);
      bool lep2_tight(false);
      if(flavour==MuMu){
	lep1_tight=muons[0].PassID(param.Muon_Tight_ID);
	lep2_tight=muons[1].PassID(param.Muon_Tight_ID);
      }
      if(flavour==EE){
        lep1_tight=electrons[0].PassID(param.Electron_Tight_ID);
        lep2_tight=electrons[1].PassID(param.Electron_Tight_ID);
      } 
      if(lep1_tight&&lep2_tight)ev_type = "TT_";
      if(lep1_tight&&!lep2_tight)ev_type = "TL_";
      if(!lep1_tight&&lep2_tight)ev_type = "LT_";
      if(!lep1_tight&&!lep2_tight)ev_type = "LL_";
    }
  }
  
  
  
  if(FullAnalysis)FillRegionPlots(flavour,true,"presel"+channel_s+"_"+ charge_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, weight_hm);
  
  return;
} /// end function                      




void HNDilepton::FillAllSignalRegion1(HNDilepton::Flavour flavour, TString signal_region1, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons,   Particle _MET,int _npv  , double w    , bool FullAnalysis ){

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

  return;
}

void HNDilepton::FillAllSignalRegion2(HNDilepton::Flavour flavour, TString signal_region2, bool isdata, TString charge_s, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Muon> muons, Particle _MET,int _npv  , double w, bool FullAnalysis){
  
  return;
}



void  HNDilepton::FillObjectSizes(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ll, TString  channel_s){

  TString label = param.Name;

  bool debug=false;
  if(debug){
    cout << "HNDilepton::FillObjectSizes::INFO " << endl;
    cout << "HNDilepton::FillObjectSizes::INFO  #electrons    = "<<  electrons.size() << endl;
    cout << "HNDilepton::FillObjectSizes::INFO  #muon         = "<<  muons.size() << endl;
    cout << "HNDilepton::FillObjectSizes::INFO  #electrons (veto) = "<<  electrons_veto.size() << endl;
    cout << "HNDilepton::FillObjectSizes::INFO  #muon      (veto) = "<<  muons_veto.size() << endl;
  }
  FillHist( "NObj/n_electrons_"+label+channel_s, electrons.size() ,weight_ll, 5, 0., 5,"N_{electron}");
  FillHist( "NObj/n_muons_"+label+channel_s,muons.size() ,weight_ll, 5, 0., 5,"N_{muons}");
  FillHist( "NObj/n_electrons_veto_"+label+channel_s, electrons_veto.size() ,weight_ll, 5, 0., 5,"N_{electron}");
  FillHist( "NObj/n_muons_veto_"+label+channel_s , muons_veto.size() ,weight_ll, 5, 0., 5,"N_{muon}");

  for(auto fatjet : map_fatjets){
    FillHist( "NObj/n_ak8jet_"+fatjet.first+"_"+label+channel_s, fatjet.second.size() ,weight_ll, 5, 0., 5,"N_{AK8}");
    if(debug)cout << "HNDilepton::FillObjectSizes::INFO " << fatjet.first << " = " << fatjet.second.size() << endl;
  }
  for(auto jet : map_jets){
    FillHist( "NObj/n_ak4jet_"+jet.first+"_"+label+channel_s, jet.second.size() ,weight_ll, 10, 0., 10,"N_{AK4}");
    if(debug)cout << "HNDilepton::FillObjectSizes::INFO " << jet.first << " = " << jet.second.size() << endl;
  }

  return;
}


void HNDilepton::MakeAK4JetPlots(Flavour channel, AnalyzerParameter param,map<TString, vector<Jet> > jet_map, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, double w){
  
  for(map<TString,   std::vector<Jet> >::iterator mit = jet_map.begin(); mit != jet_map.end(); mit++){

    for(unsigned int ijet =0; ijet < mit->second.size(); ijet++){

      if(channel==EE){
	for(auto vel : veto_electrons){
	  FillHist( "AK4Plots/deltaR_j_lep"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(vel),w, 50, 0., 5,"#DeltaR(l,AK4)");
	}
      }
      if(channel==MuMu){
        for(auto vel : veto_muons){
          FillHist( "AK4Plots/deltaR_j_lep"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(vel),w, 50, 0., 5,"#DeltaR(l,AK4)");
        }
      } 

    }
    
  }
  return;
}

void HNDilepton::MakeAK8JetPlots(Flavour channel, AnalyzerParameter param,map<TString, vector<FatJet> > fatjet_map, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, double w){

  for(map<TString,   std::vector<FatJet> >::iterator mit = fatjet_map.begin(); mit != fatjet_map.end(); mit++){
    
    for(unsigned int ijet =0; ijet < mit->second.size(); ijet++){
      
      if(channel==EE){
	for(auto vel : veto_electrons){
	  
	  FillHist( "AK8Plots/deltaR_fj_lep"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(vel) ,w, 50, 0., 5,"#DeltaR(l,AK8)");
	}
	
	FillHist( "AK8Plots/AK8Jet_pt_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].Pt() , w, 100, 0., 1000., "AK8 jet p_{T} (GeV)");
	FillHist( "AK8Plots/AK8Jet_eta_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].Eta() , w, 100, -5., 5., "AK8 jet #eta");
	FillHist( "AK8Plots/AK8Jet_mass_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].SDMass(),w, 40, 0., 200., "AK8 SDMass");
	FillHist( "AK8Plots/AK8Jet_tau21_"+param.Electron_Tight_ID+"_"+mit->first, mit->second[ijet].PuppiTau21(),w, 20, 0., 1., "AK8 #tau_{2}/#tau_{1}");
      }
      if(channel==MuMu){
	for(auto vem : veto_muons){
	  
	  FillHist( "AK8Plots/deltaR_fj_lep"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].DeltaR(vem) ,w, 50, 0., 5,"#DeltaR(l,AK8)");
	}
	FillHist( "AK8Plots/AK8Jet_pt_"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].Pt() , w, 100, 0., 1000., "AK8 jet p_{T} (GeV)");
        FillHist( "AK8Plots/AK8Jet_eta_"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].Eta() , w, 100, -5., 5., "AK8 jet #eta");
        FillHist( "AK8Plots/AK8Jet_mass_"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].SDMass(),w, 40, 0., 200., "AK8 SDMass");
        FillHist( "AK8Plots/AK8Jet_tau21_"+param.Muon_Tight_ID+"_"+mit->first, mit->second[ijet].PuppiTau21(),w, 20, 0., 1., "AK8 #tau_{2}/#tau_{1}");

      }
    }// map
  }
  return;
}



vector<Jet>   HNDilepton::GetAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean, TString pu_tag, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, vector<FatJet> fatjets){

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


vector<FatJet>   HNDilepton::GetAK8Jets(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut, vector<Electron>  veto_electrons, vector<Muon>  veto_muons){

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

vector<pair<int, pair<TString, double> > >  HNDilepton::GetSyst(HNDilepton::Flavour flavour, bool isdata){

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


int HNDilepton::GetNBJets(TString ID, TString WP ){

  vector<Jet> this_AllJets = AllJets;
  std::vector<Jet>      jets   = SelectJets(this_AllJets,ID, 20., 2.4);

  int NBJets=0;

  JetTagging::WP jwp = JetTagging::Medium;
  if  (WP == "Loose")  jwp = JetTagging::Loose;
  else if  (WP == "Medium")  jwp = JetTagging::Medium;
  else  if  (WP == "Tight")  jwp = JetTagging::Tight;
  else return -1;
  for(unsigned int i=0; i<jets.size(); i++){
    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    jwp, 
						    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  return NBJets;
}



void HNDilepton::FillElectronEfficiency(TString tag, std::vector<Electron> electrons, double w){

  double ptbins[13] = { 15.,20.,25.,30.,35.,40.,50., 60., 80., 100.,200.,500., 2000.};

  for (auto el: electrons){

    FillHist( "Efficiency/"+tag+"_pt_el", el.Pt(),w, 12, ptbins, "el p_{T} GeV");
    FillHist( "Efficiency/"+tag+"_pt_el", el.Eta(),w, 50, -2.5,2.5, "el #eta");

    /*if(tag == "AllElectrons"){
      FillHist( "Efficiency/"+tag+"_pt_el", el.Pt(),w, 12, ptbins, "el p_{T} GeV");
      FillHist( "Efficiency/"+tag+"_pt_el", el.Eta(),w, 50, -2.5,2.5, "el #eta");
    }
    else{
      if (el.PassID(tag)){
	FillHist( "Efficiency/"+tag+"_pt_el", el.Pt(),w, 12, ptbins, "el p_{T} GeV");
	FillHist( "Efficiency/"+tag+"_pt_el", el.Eta(),w, 50, -2.5,2.5, "el #eta");
      }
      }*/

  }    
    return;
}

void HNDilepton::FillElectronPresel(TString tag, double w, int bins){
  
  //if(bins>0)FillHist("PreselEEYields/"+tag+"_events", 0.1,w,2 ,0,2 ,"el #eta");
  if(bins==2)   FillHist("PreselEEYields/"+tag+"_SR1_events", 1.1,w,2 ,0,2, "el #eta");
  if(bins==3)   FillHist("PreselEEYields/"+tag+"_SR2_events", 1.1,w,2 ,0,2, "el #eta");

}

void HNDilepton::FillElectronPresel(double iid, int nvec,TString id,vector<TString> idnames,TString tag, double w, int bins){

  //if(bins>0)FillHist("PreselEEYields/"+tag+"_events", 0.1,w,2 ,0,2 ,"el #eta");                                                                                                  

  //if(_jentry==0) AnalyzerCore::FillEventCutflow("Fullopt_SR1_events", w, idnames, "TEST");
  //if(_jentry==0) AnalyzerCore::FillEventCutflow("Fullopt_SR2_events", w, idnames, "TEST");

  //if(bins==2)     AnalyzerCore::FillEventCutflow("Fullopt_SR1_events", w, idnames, id);
  // if(bins==3)     AnalyzerCore::FillEventCutflow("Fullopt_SR2_events", w, idnames, id);

  if(!Signal){
    if(bins==2)   FillHist("PreselEEYields/"+tag+"_SR1_events", iid,w,nvec ,0,double(nvec), "el #eta");
    if(bins==3)   FillHist("PreselEEYields/"+tag+"_SR2_events", iid,w,nvec ,0,double(nvec), "el #eta");
  }
  if(bins==4)   FillHist("PreselEEYields/"+tag+"_SR3_events", iid,w,nvec ,0,double(nvec), "el #eta");
  if(bins==5)   FillHist("PreselEEYields/"+tag+"_SR4_events", iid,w,nvec ,0,double(nvec), "el #eta");


}


void HNDilepton::RunElectron(double iid, int nvec,TString id,vector<TString> idnames, TString tag, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ee){

  //  FillElectronEfficiency(tag ,electrons,weight_ee);                                                                                                                            
  FillElectronPresel(iid,nvec,id,idnames,tag+"_LowPt" ,weight_ee, PassEE(0,electrons, electrons_veto,muons, muons_veto,  map_jets, map_fatjets, ev,  param, weight_ee));
  FillElectronPresel(iid,nvec,id,idnames,tag+"_HighPt" ,weight_ee , PassEE(1,electrons, electrons_veto,muons, muons_veto,  map_jets, map_fatjets, ev,  param, weight_ee));
  // FillElectronPresel(tag ,weight_ee, PassEE(-1,electrons, electrons_veto,muons, muons_veto,  map_jets, map_fatjets, ev,  param, weight_ee));                                    

  return;
}


void HNDilepton::RunElectron(TString tag, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ee){


  //  FillElectronEfficiency(tag ,electrons,weight_ee);
  FillElectronPresel(tag+"_LowPt" ,weight_ee, PassEE(0,electrons, electrons_veto,muons, muons_veto,  map_jets, map_fatjets, ev,  param, weight_ee));
  FillElectronPresel(tag+"_HighPt" ,weight_ee , PassEE(1,electrons, electrons_veto,muons, muons_veto,  map_jets, map_fatjets, ev,  param, weight_ee));
  // FillElectronPresel(tag ,weight_ee, PassEE(-1,electrons, electrons_veto,muons, muons_veto,  map_jets, map_fatjets, ev,  param, weight_ee));
  
}

int HNDilepton::PassEE(int ptbin, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::map<TString, std::vector<Jet> > map_jets, std::map<TString, std::vector<FatJet> > map_fatjets,  Event ev, AnalyzerParameter param,  float weight_ee){
  

  if(ptbin==0) param.Name = param.Name+"_LowPt";
  if(ptbin==1) param.Name = param.Name+"_HighPt";
  HNDilepton::Flavour flavour=EE;

  if(electrons.size()!=2) return 0;
  if(param.Name.Contains("HEEP")) {
    if(electrons[1].Pt() < 40) return 0;
  }
  if(ptbin==0 && param.Name.Contains("HEEP")) return 0;
  if(ptbin==0){
    if(electrons[1].Pt() > 40) return 0;
  }
  if(ptbin>=1){
    if(electrons[1].Pt() < 40) return 0;
  }
  //************************************************///                                                                                                                                                     
  //@@@@ Apply lepton scale factors                                                                                                                                                                         
  //************************************************///                                                                                                                                                     


  std::map<TString, std::vector<FatJet> >::iterator map_fatjet_it = map_fatjets.find("ak8_type1");
  if(map_fatjet_it == map_fatjets.end() ) {
    for(auto mit : map_fatjets) cout << "ak8_type1 " << " in " << mit.first << endl;
    return 0;
  }
  std::vector<FatJet> fatjets = map_fatjet_it->second;

  std::map<TString, std::vector<Jet> >::iterator map_jet_it = map_jets.find("ak4_type1");
  if(map_jet_it == map_jets.end() ){
    for(auto mit : map_jets) cout << "ak4_type1 " << " in " << mit.first << endl;
    return 0;
  }
  std::vector<Jet> jets = map_jet_it->second;

  TString channel_s="_EE";

  TString label = param.Name;

  Particle METv = ev.GetMETVector();
  //Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);
 

  if(!PassMETFilter()) return 0;

  bool PassDoubleElectronTrig = ev.PassTrigger(ElectronTriggers);  
  if(!(PassDoubleElectronTrig)) return 0;
  //************************************************///    
  //************************************************///   
  //************************************************///   
  // Dilepton event selection 
  //************************************************///   
  //************************************************///   
  //************************************************///   
  //void HNDilepton::FilAllElectronPlots(TString label , TString cut,  std::vector<Electron> els, float w){
  
  //  FilAllElectronPlots(param.Name,"El",electrons_veto, fabs(weight_ee));
  if(electrons.size()!=2)  return 0;
  if(electrons[0].Pt() < 25.) return 0 ;
  if(electrons[1].Pt() < 15.) return 0;

  
  TString charge_s= (electrons[0].Charge() ==  electrons[1].Charge() )  ?  "same_sign" : "opposite_sign";
  HNDilepton::ChargeType charge_i = (electrons[0].Charge() ==  electrons[1].Charge() )  ?  SS : OS;

  if(charge_s=="opposite_sign") return 0;
  if(SignalOS){ charge_s= "opposite_sign"; charge_i=OS;}

  if(electrons_veto.size()!=2) return 0;
  if(muons_veto.size() > 0)  return 0;

  //************************************************///   
  // create proxy for Z/W candidates
  //************************************************///   
  Particle llCand = electrons.at(0) + electrons.at(1);
  
  if(llCand.M() < 10) return 0;

  int njets = jets.size() + fatjets.size();
  bool inZpeak = (llCand.M() < 101. && llCand.M() > 81.) ;

  if(flavour == EE && inZpeak ) return 0;
  int NBJets=GetNBJets("HNTight","Medium");

  double ST = GetST(electrons, muons, jets, fatjets, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  
  if(NBJets > 0 || met2_st > 10. )  {
    if(fatjets.size() > 0 ) return 3;
    if(jets.size() > 0) return 2;
  }
  else {
    if(fatjets.size() > 0 ) return 5;
    if(jets.size() > 0) return 4;
  }
  return 1;


}

double HNDilepton::GetEventFatJetSF(vector<FatJet> fatjets, TString label, int dir){
  
  double FatJetTau21_SF(1);
  for (auto ifj : fatjets){
    FatJetTau21_SF*=GetFatJetSF(ifj, label,dir);
  }
  return FatJetTau21_SF;

}

double HNDilepton::GetFatJetSF(FatJet fatjet, TString tag,  int dir){

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

double HNDilepton::GetMuonMCWeight(std::vector<Muon> muons, TString Tight_ID,AnalyzerParameter param ){

  double this_weight(1.);
  if(!IsDATA){

    /*vector<TString> SF_list = {"passMediumID","passTightID"};

    for (auto mu: mymuons){
      TString Muon_ID_SF_Key      =  Tight_ID;
      bool apply_ID_SF=false;
      //POGTightPFIsoLoose                                                                                                                                                                                                                  
      if(Muon_ID_SF_Key.Contains("HNTightV")) Muon_ID_SF_Key = "passTightID";
      if(Muon_ID_SF_Key.Contains("HNMediumV")) Muon_ID_SF_Key = "passMediumID";
      if(Muon_ID_SF_Key.Contains("passTightID")) Muon_ID_SF_Key = "passTightID";
      for(auto i : SF_list){
	if(i==Muon_ID_SF_Key) apply_ID_SF=true;
      }
    */

    
    for (auto mu: muons){    
      TString Muon_ID_SF_Key      =  Tight_ID;                                                                                                                                                             
      
      double MiniAODP = sqrt( mu.MiniAODPt() * mu.MiniAODPt() + mu.Pz() * mu.Pz() );
      
      double this_recosf =  1.;
      double this_idsf   =  1.;
      double this_isosf  =  1.;
      
      if(Muon_ID_SF_Key == "POGHighPtTight") {
	this_recosf = mcCorr->MuonReco_SF("HighPtMuonRecoSF", mu.Eta(), MiniAODP, 0);
	this_idsf  = mcCorr->MuonID_SF ("NUM_HighPtID_DEN_genTracks" ,  mu.Eta(), mu.MiniAODPt(),  0);
	this_isosf = mcCorr->MuonISO_SF("NUM_LooseRelTkIso_DEN_HighPtIDandIPCut", mu.Eta(), mu.MiniAODPt(),  0);
      }
      if(Muon_ID_SF_Key.Contains("POGTight")){
	this_idsf  = mcCorr->MuonID_SF ("NUM_TightID_DEN_genTracks" ,  mu.Eta(), mu.MiniAODPt(),  0);
	
	if(Muon_ID_SF_Key.Contains("PFIsoVery") || Muon_ID_SF_Key.Contains("PFIsoTight") || Muon_ID_SF_Key.Contains("PFIsoMedium")){
	  this_isosf = mcCorr->MuonISO_SF("NUM_TightRelIso_DEN_TightIDandIPCut", mu.Eta(), mu.MiniAODPt(),  0);
	}
      }
      if(Muon_ID_SF_Key.Contains("HNTight")){
	this_idsf  = mcCorr->MuonID_SF ("NUM_TightID_DEN_genTracks" ,  mu.Eta(), mu.MiniAODPt(),  0);
	this_isosf = mcCorr->MuonISO_SF("NUM_TightRelIso_DEN_TightIDandIPCut", mu.Eta(), mu.MiniAODPt(),  0);
	
      }
      FillWeightHist("IDMuWeight_"+param.Name,this_idsf);
      FillWeightHist("ISOMuWeight_"+param.Name,this_isosf);
      this_weight *= this_recosf*this_idsf*this_isosf;

    }// end of muon loop
  }// end of MC req.
 
  return this_weight;
  
}


double HNDilepton::GetElectronMCWeight(std::vector<Electron> electrons, TString Tight_ID,AnalyzerParameter param ){
  
  double this_weight(1.);
  if(!IsDATA){

    //==== lepton scale factors here                                                                                                                                                                                                          

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

    vector<TString> SF_list = {"passMediumID","passTightID","passMVAID_iso_WP80","passMVAID_iso_WP90"};
    for (auto el: electrons){
      
      TString Electron_ID_SF_Key      =  Tight_ID;
      bool apply_ID_SF=false;
      
      
      if(Electron_ID_SF_Key.Contains("HNTightV")) Electron_ID_SF_Key = "passTightID";
      if(Electron_ID_SF_Key.Contains("HNMediumV")) Electron_ID_SF_Key = "passMediumID";
      if(Electron_ID_SF_Key.Contains("passTightID")) Electron_ID_SF_Key = "passTightID";
      
      for(auto i : SF_list){
	if(i==Electron_ID_SF_Key) apply_ID_SF=true;
      }
      
      int SystDir_ElectronIDSF(0);                                                                                                                                                                            
      int SystDir_ElectronRecoSF(0);         
      double this_recosf  = mcCorr->ElectronReco_SF(el.scEta(),el.Pt(), SystDir_ElectronRecoSF);
      double this_idsf    = (apply_ID_SF) ? mcCorr->ElectronID_SF(Electron_ID_SF_Key, el.scEta(), el.Pt(), SystDir_ElectronIDSF) :  1.;
      this_weight *= this_recosf*this_idsf;
      FillWeightHist("el_reco_sf_"+param.Name, this_recosf);
      FillWeightHist("el_id_sf_"+param.Name, this_idsf);
      
    }
  }
  return this_weight;


}

double HNDilepton::GetFakeRateEl(double eta, double pt, TString id){
  
  double scale=1.;
  
  if(id=="passMVAID_iso_WP80") scale=0.9;
  if(id=="passMVAID_iso_WP90") scale=0.9;
  
  return fakeEst->GetElectronFakeRate(id,"AwayJetPt40",eta, pt, 0)*scale;

  return 1.;

}
double HNDilepton::GetFakeRateM(double eta, double pt, TString id){

  //IDMu     = POGTightPFIsoVeryTight,HNTight2016,POGHighPtMixTight,POGTightPFIsoVeryVeryTight,POGTightPFIsoTight,POGTightPFIsoMedium,POGTightPFIsoLoose,HNTightV1,HNTightV2,POGHighPtTight


  double scale=1.;
  
  if(id=="HNTightV1") scale=0.9;
  if(id=="HNTightV2") scale=0.9;
  if(id=="POGTightPFIsoLoose") scale=0.7;
  if(id=="POGTightPFIsoMedium") scale=0.7;
  if(id=="POGTightPFIsoTight") scale=0.8;
  if(id=="POGTightPFIsoVeryTight") scale=0.9;
  if(id=="HNTight2016") scale=0.9;

  return fakeEst->GetMuonFakeRate(id, "AwayJetPt40",eta, pt, 0)*scale;

  return 1.;

}

double HNDilepton::CalculateDiLepWeight( double fr1, double fr2, bool mu1Tight, bool mu2Tight){

  
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




double HNDilepton::GetCFWeightElectron(std::vector<Electron> electrons , TString tight_id){
  //double CFBackgroundEstimator::GetElectronCFRate(TString ID, TString key, double eta, double pt, int sys){

  TString tight_id_tmp = tight_id;
  double el1_cf_rate =   cfEst->GetElectronCFRate(tight_id_tmp,"HNCF",(electrons[0].scEta()), electrons[0].Pt(), 0);
  double el2_cf_rate =   cfEst->GetElectronCFRate(tight_id_tmp,"HNCF",(electrons[1].scEta()), electrons[1].Pt(), 0);

  if((electrons[0].scEta()) < 1.5) el1_cf_rate *= 0.8;
  else el1_cf_rate *= 0.9;
  if((electrons[1].scEta()) < 1.5) el2_cf_rate *= 0.8;
  else el2_cf_rate *= 0.9;

  double cf_weight = (el1_cf_rate / (1.-el1_cf_rate))  + (el2_cf_rate/(1.-el2_cf_rate));
  return cf_weight;
}
double HNDilepton:: GetFakeWeightElectron(std::vector<Electron> electrons , TString tight_id){

  if(electrons.size() == 2){
    
    TString histtag=tight_id;
    //if(tight_id == "passTightID") histtag="passTightID_noccb";
    float _el1_eta=fabs(electrons.at(0).scEta());
    float _el2_eta=fabs(electrons.at(1).scEta());
    
    
    TString postfix = "";
    if(tight_id.Contains("HEEP")) postfix = "_pt";
    double fr1 = GetFakeRateEl(_el1_eta, electrons.at(0).Pt(), histtag+"_pt");
    double fr2 = GetFakeRateEl(_el2_eta, electrons.at(1).Pt(), histtag+postfix);
    
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

  double this_weight=-1.;
  TString histtag=tight_id;
  
  vector<double> FRs;
  
  
  for(auto iel : electrons){
    if(tight_id.Contains("HEEP") && iel.Pt() < 40) return 0.;
    bool isTight = iel.PassID(histtag);
    if(isTight) continue;
    TString postfix= (FRs.size()==0) ? "_pt" : "";
    double fr = GetFakeRateEl(fabs(iel.Eta()), iel.Pt(), histtag+postfix);
    this_weight*= -1.*fr/(1-fr);
    FRs.push_back(fr);
  }

  if(FRs.size()==0){
    return 0;
  }
  else{
    return this_weight;
  }

}

double HNDilepton:: GetFakeWeightMuon(std::vector<Muon> muons , TString tight_id){

  if(muons.size() == 2) {
    
    TString histtag1=tight_id;
    TString histtag2=tight_id;
    
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
    
    double fr1 = GetFakeRateM(_mu1_eta, muons.at(0).Pt(), histtag1+"_pt");
    double fr2 = GetFakeRateM(_mu2_eta, muons.at(1).Pt(), histtag2);
    
    
    bool isTight1 = muons[0].PassID(histtag1);
    bool isTight2 =  muons[1].PassID(histtag2);
    return CalculateDiLepWeight(fr1, fr2, isTight1, isTight2);
  
  }
  double this_weight=-1.;
  bool lowpt(false);
  for(auto imu : muons){
    if(imu.Pt() < 60.) lowpt=true;
  }
  TString histtag=tight_id;
  vector<double> FRs;

  for(auto imu : muons){
    if(tight_id == "POGHighPtTight" && imu.Pt() < 60) return 0.;
    bool isTight = imu.PassID(histtag);
    if(isTight) continue;
    TString postfix= (FRs.size()==0) ? "_pt" : "";
    double fr = GetFakeRateM(fabs(imu.Eta()), imu.Pt(), histtag);
    this_weight*= -1.*fr/(1-fr);
    FRs.push_back(fr);

  }

  if(FRs.size()==0){
    return 0;
  }
  else{
    return this_weight;
  }
  
}





HNDilepton::HNDilepton(){
  
  RunFake = false;

}
 
 HNDilepton::~HNDilepton(){
   delete rand_;

}

void HNDilepton::FilAllMuonPlots(TString label , TString cut,  std::vector<Muon> muons, float w){

  for(unsigned int i=0; i <  muons.size(); i++){

    TString mu_lab="muon1";
    if(i==1) mu_lab="muon2";
    if(i==2) mu_lab="muon3";
    FillHist( cut+ "/pt_mu_"+mu_lab+label , muons.at(i).Pt() , w, 500, 0., 1000., "muon p_{T} GeV");
    FillHist( cut+ "/dxy_mu_"+mu_lab+label , muons.at(i).dXY() , w, 500, -0.2, 0.2, "dXY");
    FillHist( cut+ "/dz_mu_"+mu_lab+label , muons.at(i).dZ() , w, 500, -0.5, 0.5, "dZ");
    FillHist( cut+ "/reliso_mu_"+mu_lab+label , muons.at(i).RelIso() , w, 50, 0., 1., "R_{ISO} GeV");
    FillHist( cut+ "/eta_"+mu_lab+ label  , muons.at(i).Eta() , w, 60, -3., 3.,"muon #eta");
    FillHist( cut+ "/IP3D_"+mu_lab+ label  , muons.at(i).IP3D()/muons.at(i).IP3Derr(), w, 400, -20., 20., "IP3D");
  }
}
void HNDilepton::FilAllElectronPlots(TString _label , TString cut,  std::vector<Electron> els, float w){

  for(unsigned int i=0; i < els.size(); i++){
    TString el_lab="el1";
    if(i==1) el_lab="el2";
    if(i==2) el_lab="el3";

    TString label = _label;
    if(!els[i].IsGsfCtfScPixChargeConsistent()){
      FillHist( cut+ "/pt_noncc_vetoel_"+el_lab+label , els.at(i).Pt() , w, 500, 0., 1000., "el p_{T} GeV");
    }

    if( fabs( els.at(i).scEta()) <= 0.8 )label="_barrel1"+label;
    else if( fabs( els.at(i).scEta()) <= 1.479 )label="_barrel2"+label;
    else label="_endcap"+label;

    if(!IsData){
      int type1=GetLeptonType(els.at(i), gens);
      if(type1 <=0) label += "_nonprompt";
      else label += "_prompt";
    }

    FillHist( cut+ "/IP3D_"+ label  ,  els.at(i).Pt(),  els.at(i).IP3D()/els.at(i).IP3Derr(),  w, 100, 0., 500,400, -10., 10.);
    FillHist( cut+ "/DXY"+ label    ,  els.at(i).Pt(), els.at(i).dXY(), w, 100, 0., 500,400, -0.1, 0.1);
    FillHist( cut+ "/reliso_vetoel_"+label , els.at(i).RelIso() , w, 50, 0., 0.6, "R_{ISO} GeV");
    FillHist( cut+ "/iso_vetoel_"+label ,  els.at(i).Pt(), els.at(i).RelIso()*els.at(i).Pt(), w,100, 0., 500, 50, 0., 10.);
    FillHist( cut+ "/miniisopt_vetoel_"+label ,  els.at(i).Pt(),els.at(i).MiniRelIso()*els.at(i).Pt(), w, 100, 0., 500,50, 0., 10.);
    FillHist( cut+ "/miniiso_vetoel_"+label , els.at(i).MiniRelIso() , w, 50, 0., 0.6);
    FillHist( cut+ "/mva_pt_vetoel_"+label ,  els.at(i).Pt(),els.at(i).MVANoIso(), w, 100, 0., 500, 200, -1., 1.);
    FillHist( cut+ "/mva1_vetoel_"+label , els.at(i).MVANoIso(), w, 50, -1., 1.);
    FillHist( cut+ "/Full5x5_sigmaIetaIeta_"+label , els.at(i).Full5x5_sigmaIetaIeta() , w, 100, 0., 0.04, "");
    FillHist( cut+ "/dEtaSeed_"+label , els.at(i).dEtaSeed() , w, 1000, -0.02, 0.02, "");
    FillHist( cut+ "/dPhiIn_"+label , els.at(i).dPhiIn() , w, 1000, -.05, 0.05, "");
    FillHist( cut+ "/HoverE_"+label , els.at(i).HoverE() , w, 100, 0., 0.15, "");
    FillHist( cut+ "/InvEminusInvP_"+label , els.at(i).InvEminusInvP() , w, 100, 0., 0.1, "");
    
  }
  return;
  
}

void HNDilepton::MakeGeneralPlots(vector<Muon> mymuons, vector<Electron> myelectrons, map<TString, vector<FatJet> > fatjet_map, AnalyzerParameter param ,double weight){

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


void HNDilepton::FillZZCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){

  Particle METv = ev.GetMETVector();
  //Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);


  TString channel_s= (flavour==EE) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (flavour==EE) ?   25. : 20.;
  double lep2_ptcut= (flavour==EE) ?   20. : 15.;
  //  double lep3_ptcut= (flavour==EE) ?   20. : 15.;


  int NBJets=GetNBJets("HNTight","Medium");

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

double HNDilepton::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}

double HNDilepton::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){
  
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


void HNDilepton::FillZGCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){

  Particle METv = ev.GetMETVector();
  //Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  int NBJets=GetNBJets("HNTight","Medium");

  TString channel_s= (flavour==EE) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (flavour==EE) ?   25. : 20.;
  double lep2_ptcut= (flavour==EE) ?   20. : 15.;
  double lep3_ptcut= (flavour==EE) ?   20. : 15.;
  //  double lep4_ptcut= (flavour==EE) ?   10. : 10.;
  double metcut = 50.;

  if(flavour == MuMu){
    // muon channel   require 3 muons and 0 electrons                                                                                                                             
    if(muons.size() ==3 && electrons_veto.size() == 0 && muons_veto.size()==3){
      // pass pt cuts                                                                                                                                                             
      if(muons[0].Pt() > lep1_ptcut && muons[1].Pt()  > lep2_ptcut&& muons[2].Pt() > lep3_ptcut ) {

	Particle lll = muons[0] + muons[1]+ muons[2];
	bool passZmass_lll_Window = (fabs(lll.M() - 90.1) < 15.);
	if(passZmass_lll_Window){
	  
	  Particle ll1 = muons[0] + muons[1];
	  Particle ll2 = muons[0] + muons[2];
	  Particle ll3 = muons[1] + muons[2];
	  bool passZmass_os_Window=true;
	  if(ll1.Charge() == 0 && (fabs(ll1.M() - 90.1) < 15.)) passZmass_os_Window=false;
	  if(ll2.Charge() == 0 && (fabs(ll2.M() - 90.1) < 15.)) passZmass_os_Window=false;
	  if(ll3.Charge() == 0 && (fabs(ll3.M() - 90.1) < 15.)) passZmass_os_Window=false;

	  if(NBJets == 0){
	    if(METv.Pt() < metcut){
	      if(passZmass_os_Window){
		FillRegionPlots(flavour,true,"ZG_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
		// fill plot for all channels                                                                                                                                                           
		FillRegionPlots(flavour,true,"ZG_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
	      }
	    }
	  }
	}
      }
    }
  }
  if(flavour == EE){
    // muon channel   require 3 electrons and 0 electrons                                                                                                                                                       
    if(electrons.size() ==3 && muons_veto.size() == 0 && electrons_veto.size()==3){
      // pass pt cuts                                                                                                                                                                                       
      if(electrons[0].Pt() > lep1_ptcut && electrons[1].Pt()  > lep2_ptcut && electrons[2].Pt()  > lep3_ptcut  ) {

        Particle lll = electrons[0] + electrons[1]+ electrons[2];
        bool passZmass_lll_Window = (fabs(lll.M() - 90.1) < 15.);
        if(passZmass_lll_Window){

          Particle ll1 = electrons[0] + electrons[1];
          Particle ll2 = electrons[0] + electrons[2];
          Particle ll3 = electrons[1] + electrons[2];
          bool passZmass_os_Window=true;
          if(ll1.Charge() == 0 && (fabs(ll1.M() - 90.1) < 15.)) passZmass_os_Window=false;
          if(ll2.Charge() == 0 && (fabs(ll2.M() - 90.1) < 15.)) passZmass_os_Window=false;
          if(ll3.Charge() == 0 && (fabs(ll3.M() - 90.1) < 15.)) passZmass_os_Window=false;

          if(NBJets == 0){
            if(METv.Pt() < metcut){
              if(passZmass_os_Window){
                FillRegionPlots(flavour,true,"ZG_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
                // fill plot for all channels                                                                                                                                                               
                FillRegionPlots(flavour,true,"ZG_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
              }
            }
          }
        }
      }
    }
  }
  
  return;

}
void HNDilepton::FillWGCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){

  Particle METv = ev.GetMETVector();
  //Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  TString channel_s= (flavour==EE) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (flavour==EE) ?   25. : 20.;
  double lep2_ptcut= (flavour==EE) ?   20. : 15.;
  double lep3_ptcut= (flavour==EE) ?   20. : 15.;
  double metcut = 50.;
  double mt_cut = 30.;
  int NBJets=GetNBJets("HNTight","Medium");

  if(flavour == MuMu){
    // muon channel   require 3 muons and 0 electrons                                                                                                                                                                                                                                         
    if(muons.size() ==3 && electrons_veto.size() == 0 && muons_veto.size()==3){
      // pass pt cuts                                                                                                                                                                                                                                                                         
      if(muons[0].Pt() > lep1_ptcut && muons[1].Pt()  > lep2_ptcut && muons[2].Pt()  > lep3_ptcut) {

	Particle ll1 = muons[0] + muons[1];
	Particle ll2 = muons[0] + muons[2];
	Particle ll3 = muons[1] + muons[2];
	bool passlos_ll_mass=false;
	if(ll1.Charge() == 0 && (ll1.M() < 4.)) passlos_ll_mass=true;
	if(ll2.Charge() == 0 && (ll2.M() < 4.)) passlos_ll_mass=true;
	if(ll3.Charge() == 0 && (ll3.M() < 4.)) passlos_ll_mass=true;

	Particle lll = muons[0]+muons[1]+muons[2];
	double MT_lll = MT(METv,lll);
	if(MT_lll > mt_cut){
          if(NBJets == 0){
            if(METv.Pt() < metcut){
              if(passlos_ll_mass){
                FillRegionPlots(flavour,true,"WG_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
                // fill plot for all channels                                                                                                                                                                                                                                                 
                FillRegionPlots(flavour,true,"WG_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
              }
            }
          }
        }
      }
    }
  }

  if(flavour == EE){
    // muon channel   require 3 muons and 0 electrons                                                                                                                                                      
                                                                                                                                                                                                            
    if(electrons.size() ==3 && muons_veto.size() == 0 && electrons_veto.size()==3){
      // pass pt cuts                                                                                                                                                                                      
                                                                                                                                                                                                            
      if(electrons[0].Pt() > lep1_ptcut && electrons[1].Pt()  > lep2_ptcut && electrons[2].Pt()  > lep3_ptcut) {

	Particle ll1 = electrons[0] + electrons[1];
        Particle ll2 = electrons[0] + electrons[2];
        Particle ll3 = electrons[1] + electrons[2];
        bool passlos_ll_mass=false;
        if(ll1.Charge() == 0 && (ll1.M() < 4.)) passlos_ll_mass=true;
	if(ll2.Charge() == 0 && (ll2.M() < 4.)) passlos_ll_mass=true;
	if(ll3.Charge() == 0 && (ll3.M() < 4.)) passlos_ll_mass=true;

	Particle lll = electrons[0]+electrons[1]+electrons[2];
        double MT_lll =MT(METv,lll);
	if(MT_lll > mt_cut){
          if(NBJets == 0){
            if(METv.Pt() < metcut){
              if(passlos_ll_mass){
                FillRegionPlots(flavour,true,"WG_cr"+channel_s , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
                // fill plot for all channels                                                                                                                                                                                                                            
                FillRegionPlots(flavour,true,"WG_cr" , label, jets,  fatjets,  electrons, muons,  METv, nPV, w);
              }
            }
          }
        }
      }
    }
  }


  return;
}

void HNDilepton::FillWZCRPlots(HNDilepton::Flavour flavour, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, TString label, float w){


  Particle METv = ev.GetMETVector();
  //Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  TString channel_s= (flavour==EE) ?  "_EE" : "_MuMu";

  double lep1_ptcut= (flavour==EE) ?   25. : 20.;
  double lep2_ptcut= (flavour==EE) ?   20. : 15.;
  double lep3_ptcut= (flavour==EE) ?   20. : 15.;
  double metcut = 50.;
  double mtcut = 20.;
  double trilep_masscut=105.;

  int NBJets=GetNBJets("HNTight","Medium");
  if(jets.size()==0) return;
  if(flavour == MuMu){
    // muon channel   require 3 muons and 0 electrons
    if(muons.size() ==3 && electrons_veto.size() == 0 && muons_veto.size()==3){
      // pass pt cuts
      if(muons[0].Pt() > lep1_ptcut && muons[1].Pt()  > lep2_ptcut && muons[2].Pt()  > lep3_ptcut) {
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
      if(electrons[0].Pt() > lep1_ptcut && electrons[1].Pt()  > lep2_ptcut && electrons[2].Pt()  > lep3_ptcut) {
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

void HNDilepton::MakeSignalPlots(){
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
double  HNDilepton::GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets){
  
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

void HNDilepton::FillSigRegionPlots1(HNDilepton::Flavour channel, int systdir, TString syst, double sysval, TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11){

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
    
    HNDilepton::Region cutflow_index = MuMu_SR1;
    if(channel == EE )cutflow_index = EE_SR1;
    HNDilepton::ChargeType charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  SS : OS;
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
    if(met2st < 5. && dRW_lep2 < 2.){
      if(N2cand.M() < 100.) bin=0;
      else if(N2cand.M() < 125.) bin=1;
      else if(N2cand.M() < 150.) bin=2;
      else if(N2cand.M() < 175.) bin=3;
      else if(N2cand.M() < 200.) bin=4;
      else bin=5;
    }
    else{
      if(N2cand.M() < 100.) bin=6;
      else if(N2cand.M() < 125.) bin=7;
      else if(N2cand.M() < 150.) bin=8;
      else if(N2cand.M() < 175.) bin=9;
      else if(N2cand.M() < 200.) bin=10;
      else bin=5;
    }
  }
  else{
    
    if(N1cand.M() < 200)  bin=11;
    else  if(N1cand.M() < 250.) bin=12;
    else if(N1cand.M() < 300.) bin=13;
    else if(N1cand.M() < 500.) bin=14;
    else if(N1cand.M() < 700.) bin=15;
    else if(N1cand.M() < 1000.) bin=16;
    else if(N1cand.M() < 1250.) bin=17;
    else if(N1cand.M() < 1500.) bin=18;
    else if(N1cand.M() < 2000.) bin=19;
    else bin=20;
    
  }
  

  if(!IsData){
    double rand_n =  rand_->Uniform(1.);
    
    
    if (rand_n > sysval && systdir==-1) return;
    if (rand_n > sysval && systdir==1) w*=(2); 
  }
  FillHist( label_1+ "/"+ label_1 +label_2+  "_signalbin_"  + label_3+"_"+syst,  bin,  w, 20, 0.,20., "SRs");
  
	    


  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double ml2jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double mlljbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};

  FillHist( label_1+ "/"+ label_1+label_2 +  "_reco_mlljj_"  + label_3+"_"+syst,  W1cand.M(),  w, 6,mlljbins, "Reco M_{lljj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_reco_ml1jj_"  + label_3+"_"+syst,  N1cand.M(),  w, 6,ml1jbins , "Reco M_{l1jj}");
  FillHist( label_1+ "/"+ label_1 +label_2 +  "_reco_ml2jj_"  + label_3+"_"+syst,  N2cand.M(),  w, 6, ml2jbins, "Reco M_{l2jj}");
  FillHist( label_1+ "/"+ label_1 +label_2+  "_njets_" + label_3+"_"+syst, jets.size() , w, 10, 0., 10., "N_{jets}");


}

void HNDilepton::FillSigRegionPlots2(HNDilepton::Flavour channel, int systdir, TString syst, double sysval,TString label_1, TString label_2, TString label_3,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7){

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

    HNDilepton::Region  cutflow_index = MuMu_SR2;
    if(channel == EE )cutflow_index = EE_SR2;
    HNDilepton::ChargeType charge_i = (lep1.Charge() ==  lep2.Charge() )  ?  SS : OS;

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
void HNDilepton::FillRegionPlots(HNDilepton::Flavour channel, bool plotCR, TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w){
  

  bool debug(false);
  if(debug){
    if(channel == EE ) cout << "FillRegionPlotsEE: " << label_1 << " " << label_2 << " plotCR = " << plotCR << " nel = " << els.size()  <<  endl;
    if(channel == MuMu ) cout << "FillRegionPlotsMuMu: " << label_1 << " " << label_2 << " plotCR = " << plotCR << " nel = " << els.size()  <<  endl; 
    else cout << "FillRegionPlots: " << label_1 << " " << label_2 << " plotCR = " << plotCR << " nel = " << els.size()  <<  endl;
  }

  Particle lep1, lep2,lep3,lep4;
  if(channel == EE   && els.size() < 2) return;
  if(channel == MuMu && mus.size() < 2) return;
  if(channel == EE   ){ lep1 = els[0]; lep2 = els[1];}
  if(channel == MuMu ){ lep1 = mus[0]; lep2 = mus[1];}
  bool threelep = (els.size() == 3) || (mus.size() == 3);
  bool fourlep = (els.size() == 4) || (mus.size() == 4);
  if(els.size() == 3) lep3 = els[2];
  if(els.size() == 4) lep4 = els[3];
  if(mus.size() == 3) lep3 = mus[2];
  if(mus.size() == 4) lep4 = mus[3];
  
  int NBJets=GetNBJets("HNTight","Medium");
  if(NBJets>0) return;

  double ST = GetST(els, mus, jets, fatjets, met);
  double met2_st = pow(met.Pt(),2.)/ ST;
  
  if(met2_st > 15) return;

  if(fatjets.size()>0){
    FillHist( label_1+ "/"+ label_1 +  "_SR2_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");
    //    FillHist( label_1+ "/"+ label_1 +  "_SR2_events_" + label_2, 0.5, w, 1, 0., 1., "N");
  }
  else{
    if(jets.size() > 0)FillHist( label_1+ "/"+ label_1 +  "_SR1_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");
    //    FillHist( label_1+ "/"+ label_1 +  "_SR1_events_" + label_2, 0.5, w, 1, 0., 1., "N");

  }
  vector<Muon>     this_AllMuons     = AllMuons;
  std::vector<Muon>      veto_muons_tmp2     = SelectMuons    (this_AllMuons,  "POGHighPtTight"   , 50., 2.4);
  //
  //FillHist( label_1+ "/"+ label_1 +  "_nhighptmuons_" + label_2, veto_muons_tmp2.size(), w, 10, 0., 10., "N(Highpt) muons");
 
  return;


  if(!plotCR) return;

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

  FillHist( label_1+ "/"+ label_1 +  "_lep1_pt_" + label_2, lep1.Pt()  , w, 400, 0., 2000.,"l_{1} p_{T} GeV");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_pt_" + label_2, lep2.Pt()  , w, 200, 0., 1000.,"1_{2} p_{T} GeV");
  if(threelep) {
    FillHist( label_1+ "/"+ label_1 +  "_lep3_pt_" + label_2, lep3.Pt()  , w, 200, 0., 1000.,"l_{3} p_{T} GeV");
    FillHist( label_1+ "/"+ label_1 +  "_lep3_eta_" + label_2, lep3.Eta()  , w, 30, -3., 3.,"l_{3} #eta");
  }
  if(fourlep) {
    FillHist( label_1+ "/"+ label_1 +  "_lep4_pt_" + label_2, lep4.Pt()  , w, 200, 0., 1000.,"l_{4} p_{T} GeV");
    FillHist( label_1+ "/"+ label_1 +  "_lep4_eta_" + label_2, lep4.Eta()  , w, 30, -3., 3.,"l_{4} #eta");
  }
  FillHist( label_1+ "/"+ label_1 +  "_lep_sumpt_" + label_2, lep1.Pt()+lep2.Pt()  , w, 100, 0., 1000.,"p_{T} GeV");

  
  FillHist( label_1+ "/"+ label_1 +  "_lep1_eta_" + label_2, lep1.Eta()  , w, 30, -3., 3,"l_{1} #eta");
  FillHist( label_1+ "/"+ label_1 +  "_lep2_eta_" + label_2, lep2.Eta()  , w, 30, -3., 3.,"l_{2} #eta");

  
  FillHist( label_1+ "/"+ label_1 + "_nPV_"+ label_2,  nvtx , w, 60, 0., 60.);
  FillHist( label_1+ "/"+ label_1 +  "_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");
  
  FillHist( label_1+ "/"+ label_1 +  "_st_" + label_2, ST  , w, 200, 0., 2000.,"ST GeV");
  FillHist( label_1+ "/"+ label_1 +  "_met2_st_" + label_2, met2_st  , w, 60, 0., 30.,"MET2/ST GeV");

  FillHist( label_1+ "/"+ label_1 +  "_met_" + label_2, met.Pt()  , w, 200, 0., 400.,"MET GeV");

  Event ev = GetEvent();
  Particle METunsmearedv = ev.GetMETVector();
  
  FillHist( label_1+ "/"+ label_1 +  "_met_uncorr_" + label_2, METunsmearedv.Pt()  , w, 200, 0., 400.,"MET GeV");
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

double HNDilepton::MT(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
  
}


std::vector<Jet> HNDilepton::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

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


void HNDilepton::FillEventCutflow(HNDilepton::ChargeType charge_i, HNDilepton::Flavour cf_flavour,float wt, TString cut,   TString label){
  
  return;
  // cf_flavour lables what hist to fill
  // charge_i :+1 means SS, -1 means OS ; 0 means OS/SS
  
  // DiEl EXO17-028                                                                                                                                                              
  if(cf_flavour == EE){
    TString charge_ = (charge_i == SS) ? "same_sign" : "opposite_sign";
    vector<TString> labels_ss = {"NoCut", "METFilter", "Trigger_el", 
				 "Dilep_el_same_sign",
				 "Lep_veto_el_same_sign", 
				 "Diel_mass_same_sign",
				 "Jet_req_same_sign",
				 "noZ_same_sign",
				 "MET_same_sign",
				 "BJet_same_sign",
				 "HighMassJet_same_sign",
				 "HighMassSR_same_sign", 
				 "Highmass_SR1",
				 "Highmass_SR2",
				 "Highmass_BJL_SR1", 
				 "Highmass_BJL_SR2",
				 "Highmass_BJT_SR1", 
				 "Highmass_BJT_SR2"};

    vector<TString> labels_os = {"NoCut", "METFilter", "Trigger_el", 
				 "Dilep_el_opposite_sign",
				 "Lep_veto_el_opposite_sign",
				 "Diel_mass_opposite_sign",
				 "Jet_req_opposite_sign", 
				 "noZ_opposite_sign",
				 "MET_opposite_sign",
				 "BJet_opposite_sign",
				 "HighMassJet_opposite_sign",
				 "HighMassSR_opposite_sign" ,
				 "Highmass_SR3",
				 "Highmass_SR4",
				 "Highmass_BJL_SR3",
				 "Highmass_BJL_SR4",
				 "Highmass_BJT_SR3",
				 "Highmass_BJT_SR4"};
    if(charge_i == SS)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_"+charge_, wt, labels_ss, cut);
    else if(charge_i == OS)    AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_"+charge_, wt, labels_os, cut);
    else {
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_same_sign", wt, labels_ss, cut);
      AnalyzerCore::FillEventCutflow(label+"exo_17_028_diel_opposite_sign", wt, labels_os, cut);
    }
    return;
  }

  // DiMu EXO17-028                                                                                                                                                                                                                                                                                           
  if(cf_flavour == MuMu){
    TString charge_ = (charge_i == SS) ? "same_sign" : "opposite_sign";
    vector<TString> labels_ss = {"NoCut",
				 "METFilter",
				 "Trigger_mu", 
				 "Dilep_mu_same_sign",
				 "Lep_veto_mu_same_sign", 
				 "Dimu_mass_same_sign",
				 "Jet_req_same_sign",
				 "MET_same_sign",
				 "BJet_same_sign",
				 "HighMassJet_same_sign",
				 "HighMassSR_same_sign",
				 "Highmass_SR1", 
				 "Highmass_SR2",
				 "Highmass_BJL_SR1",
				 "Highmass_BJL_SR2",
				 "Highmass_BJT_SR1",
				 "Highmass_BJT_SR2"};
    
    vector<TString> labels_os = {"NoCut",
				 "METFilter",
				 "Trigger_mu",
				 "Dilep_mu_opposite_sign",
				 "Lep_veto_mu_opposite_sign",
				 "Dimu_mass_opposite_sign",
				 "Jet_req_opposite_sign",
				 "MET_opposite_sign",
				 "BJet_opposite_sign",
				 "HighMassJet_opposite_sign",
				 "HighMassSR_opposite_sign",
				 "Highmass_SR3",
				 "Highmass_SR4",
				 "Highmass_BJL_SR3",
				 "Highmass_BJL_SR4",
				 "Highmass_BJT_SR3",
				 "Highmass_BJT_SR4"};

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
void HNDilepton::FillEventCutflow(HNDilepton::ChargeType charge_i,Region cf,float wt, TString cut,   TString label){                                                            
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

void HNDilepton::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, -5., 5,"ev weight");
}
void HNDilepton::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

