#include "FakeRateHN.h"

void FakeRateHN::initializeAnalyzer(){

  //================
  //==== Userflags
  //================
  
  isEE = HasFlag("isEE");
  isMM = HasFlag("isMM");
  isSingleMu = HasFlag("isSingleMu");


  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  HEM1516 = HasFlag("HEM1516");


  rand_ = new TRandom3(1234);


  cout << "[FakeRateHN::initializeAnalyzer()] RunSyst = " << RunSyst << endl;


  MuonTriggers.clear();
  MuonTriggersH.clear();
  ElectronTriggers.clear();
  SingleElectronTriggers.clear();
  EMuTriggers.clear();
  EMuTriggersH.clear();
  

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

    cout << "[FakeRateHN::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[FakeRateHN::initializeAnalyzer()] PUhname = " << PUhname << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void FakeRateHN::executeEvent(){
  
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

  
  AnalyzerParameter param;


  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ELIDs;
  vector<TString> channel;
  vector<TString>  mu_loose_id;
  vector<TString>  el_loose_id;
  if(isEE||!IsDATA){


    ELIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));    el_loose_id.push_back("HNLoose2016");                                                                                                     
    ELIDs.push_back(make_pair("HN2016", "HNVeto2016"));    el_loose_id.push_back("HN2016_Loose");                                                                                                     
    ELIDs.push_back(make_pair("HN2017", "HNVeto2016"));    el_loose_id.push_back("HN2017_Loose");                                                                                                     
    ELIDs.push_back(make_pair("HN2018", "HNVeto2016"));    el_loose_id.push_back("HN2018_Loose");                                                                                                     

    ELIDs.push_back(make_pair("HN2016RelaxedIP", "HNVeto2016"));    el_loose_id.push_back("HN2016RelaxedIP_Loose");
    ELIDs.push_back(make_pair("HN2017RelaxedIP", "HNVeto2016"));    el_loose_id.push_back("HN2017RelaxedIP_Loose");
    ELIDs.push_back(make_pair("HN2018RelaxedIP", "HNVeto2016"));    el_loose_id.push_back("HN2018RelaxedIP_Loose");
    

    //HN2016

    /*
    // =========  HEEP IDS  ================================ //                                                                                               
    ELIDs.push_back(make_pair("HEEPv7",     "HNVeto"));       el_loose_id.push_back("HEEPLoose");
    ELIDs.push_back(make_pair("HEEP2018",   "HNVeto"));       el_loose_id.push_back("HEEPLoose");
    ELIDs.push_back(make_pair("HNHEEPv7",   "HNVeto"));       el_loose_id.push_back("HNHEEPLoose");
    ELIDs.push_back(make_pair("HNHEEP2018", "HNVeto"));       el_loose_id.push_back("HNHEEPLoose");
    // =========  EXO-17-028 IDS  ================================ //                                                                                         
    ELIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));                el_loose_id.push_back("HNLoose2016");
    ELIDs.push_back(make_pair("HNTight2016Update", "HNVeto2016"));          el_loose_id.push_back("HNLoose2016Update");
    ELIDs.push_back(make_pair("HNTight2016NOIP", "HNVeto2016"));            el_loose_id.push_back("HNLoose2016NOIP");
    ELIDs.push_back(make_pair("HNTight2016UpdateNOIP", "HNVeto2016"));      el_loose_id.push_back("HNLoose2016UpdateNOIP");

    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HNTight_dxy02_05_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_05_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip3_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip3_3", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_05_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_5");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_05_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_05_dz01_ip4_5");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip3_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip3_4");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip3_3NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip3_3");
    ELIDs.push_back(make_pair("HNTight_dxy02_05_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_05_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNTight_dxy02_05_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_05_dz01_ip4_5");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_02_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_02_dz01_ip4_5");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip3_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_02_dz01_ip3_4");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip3_3NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_02_dz01_ip3_3");
    ELIDs.push_back(make_pair("HNTight_dxy05_10_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip999");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_05_dz01_ip999");
    ELIDs.push_back(make_pair("HNTight_dxy02_05_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_05_dz01_ip999");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy02_02_dz01_ip999");


    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_05_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_05_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip3_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip3_3", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip3_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip3_3", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_05_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_05_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip4_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip4_5", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip3_4", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip3_3", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy05_05_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_05_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip999", "HNVeto"));    el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy05_05_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_05_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy05_05_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_05_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_05_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_05_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip4_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip4_5NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip3_4NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip3_3NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy05_10_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy05_05_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_05_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");
    ELIDs.push_back(make_pair("HNMedium_dxy02_02_dz01_ip999NOIP", "HNVeto"));    el_loose_id.push_back("HNLoose_dxy05_10_dz01_ip4_4");



    ELIDs.push_back(make_pair("passPOGTight", "HNVeto"));              el_loose_id.push_back("passLooseID_nocc");
    ELIDs.push_back(make_pair("passPOGTight_HNTC_NOPC", "HNVeto"));    el_loose_id.push_back("passLooseID_nocc");
    ELIDs.push_back(make_pair("passPOGTight_TC", "HNVeto"));           el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_IP4", "HNVeto"));          el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TC_IP4", "HNVeto"));       el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_HNTC", "HNVeto"));         el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_HNTC_IP4", "HNVeto"));     el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TTrig", "HNVeto"));        el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TTrig_TC", "HNVeto"));     el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TTrig_IP4", "HNVeto"));    el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TTrig_HNTC", "HNVeto"));   el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TTrig_HNTC_IP4", "HNVeto"));    el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TTrig_HNTC_IP45", "HNVeto"));   el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGTight_TTrig_HNTC_IP5", "HNVeto"));    el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGMedium", "HNVeto"));           el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGMedium_TC", "HNVeto"));        el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGMedium_TC_IP4", "HNVeto"));    el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGMedium_HNTC", "HNVeto"));      el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passPOGMedium_HNTC_IP4", "HNVeto"));  el_loose_id.push_back("passLooseID");


    // POG M + IP + tight charge                                                                                                                              
    ELIDs.push_back(make_pair("passMediumID", "HNVeto"));    el_loose_id.push_back("passLooseID");
    ELIDs.push_back(make_pair("passTightID", "HNVeto"));    el_loose_id.push_back("passLooseID");

    // POG MVA noISo + POG IP + tight charge                                                                                                                  
    ELIDs.push_back(make_pair("passMVAID_noIso_WP80","HNVeto2016"));
    el_loose_id.push_back("passMVAID_noIso_WP80Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V2","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V3","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V4","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V5","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V6","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V7","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V8","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V9","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V10","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V11","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V12","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V13","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V14","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V15","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V16","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V17","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V18","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V19","HNVeto2016"));     el_loose_id.push_back("passMVAID_noIso_WP90Loose");

    ELIDs.push_back(make_pair("passMVAID_iso_WP80","HNVeto2016"));        el_loose_id.push_back("passMVAID_iso_WP80Loose");

    ELIDs.push_back(make_pair("passMVAID_iso_WP90","HNVeto2016"));        el_loose_id.push_back("passMVAID_iso_WP90Loose");

    ELIDs.push_back(make_pair("passPOGMVAID_noIso_WP80","HNVeto2016"));    el_loose_id.push_back("passMVAID_noIso_WP80Loose");

    ELIDs.push_back(make_pair("passPOGMVAID_noIso_WP90","HNVeto2016"));    el_loose_id.push_back("passMVAID_noIso_WP90Loose");

    // POG MVA Iso + POG IP + tight charge                                                                                                                    
    ELIDs.push_back(make_pair("passPOGMVAID_iso_WP80","HNVeto2016"));    el_loose_id.push_back("passMVAID_iso_WP80Loose");

    ELIDs.push_back(make_pair("passPOGMVAID_iso_WP90","HNVeto2016"));    el_loose_id.push_back("passMVAID_iso_WP90Loose");


    
    ELIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTightV1", "HNVeto"));   el_loose_id.push_back("HNLoose");

    ELIDs.push_back(make_pair("HEEPv7", "HNVeto"));    el_loose_id.push_back("HEEPLoose");
    ELIDs.push_back(make_pair("HEEP2018", "HNVeto"));    el_loose_id.push_back("HEEPLoose");

    ELIDs.push_back(make_pair("HNTightV2", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTightV3", "HNVeto"));    el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTightV4", "HNVeto"));    el_loose_id.push_back("HNLooseV2");

    ELIDs.push_back(make_pair("HNMediumV1", "HNVeto"));    el_loose_id.push_back("HNLoose");


    ELIDs.push_back(make_pair("HNMediumV2", "HNVeto"));    el_loose_id.push_back("HNLoose");
    
    
    ELIDs.push_back(make_pair("HNMediumV3", "HNVeto"));    el_loose_id.push_back("HNLoose");


    // TIGHT POG + no CC in low pt                                                       
    ELIDs.push_back(make_pair("passTightID_nocc", "HNVeto"));    el_loose_id.push_back("passLooseID_nocc");

    // POG + IP + CC                                                                     
    ELIDs.push_back(make_pair("passTightID", "HNVeto"));    el_loose_id.push_back("passLooseID");

    ELIDs.push_back(make_pair("passMediumID", "HNVeto"));    el_loose_id.push_back("passLooseID");
    

    ELIDs.push_back(make_pair("passMVAID_noIso_WP80","HNVeto2016"));    el_loose_id.push_back("passMVAID_noIso_WP80Loose");

    ELIDs.push_back(make_pair("passMVAID_noIso_WP90","HNVeto2016"));    el_loose_id.push_back("passMVAID_noIso_WP90Loose");

    ELIDs.push_back(make_pair("passMVAID_iso_WP80","HNVeto2016"));    el_loose_id.push_back("passMVAID_iso_WP80Loose");

    ELIDs.push_back(make_pair("passMVAID_iso_WP90","HNVeto2016"));    el_loose_id.push_back("passMVAID_iso_WP90Loose");

    
    ELIDs.push_back(make_pair("passPOGTight_TTrig_HNTC","HNVeto2016"));       el_loose_id.push_back("HNLooseV2"); 
    
    ELIDs.push_back(make_pair("passMVAID_noIso_WP90V16","HNVeto2016"));  el_loose_id.push_back("passMVAID_noIso_WP90LooseV2");                             
    
    ELIDs.push_back(make_pair("passPOGTightv1_TTrig_HNTC","HNVeto2016"));  el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("passPOGTightv2_TTrig_HNTC","HNVeto2016"));  el_loose_id.push_back("HNLoose");
    ELIDs.push_back(make_pair("HNTight_dxy02_02_dz01_ip4_4","HNVeto2016"));  el_loose_id.push_back("HNLooseV3");
    ELIDs.push_back(make_pair("HNTight_dxy05_05_dz01_ip4_4","HNVeto2016"));  el_loose_id.push_back("HNLooseV4");


    */


    ELIDs.push_back(make_pair("HNTightV1","HNVeto2016"));  el_loose_id.push_back("HNLooseV5");
    for (auto i: ELIDs) {
      channel.push_back("EE");
      MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
      mu_loose_id.push_back("HNLoose2016");
    }
  }
  if(isSingleMu || isMM||!IsDATA){
    
    
    //MuIDs.push_back(make_pair("HNTightPFIsoVeryVeryTight","HNVeto2016"));       mu_loose_id.push_back("HNLooseV1");

    //    MuIDs.push_back(make_pair("HNTight_Iso07_dxy_02_ip_3","HNVeto2016"));       mu_loose_id.push_back("HNLooseV1");
    
        /*
    MuIDs.push_back(make_pair("POGTightPFIsoVeryTight","HNVeto2016"));
    mu_loose_id.push_back("HNLooseV1");
    
    
    MuIDs.push_back(make_pair("POGHighPtMixTight", "HNVeto2016"));
    mu_loose_id.push_back("POGHighPtMixLoose"); // ---> FIX FR PT BIN                    

    MuIDs.push_back(make_pair("POGTightPFIsoVeryVeryTight","HNVeto2016"));
    mu_loose_id.push_back("HNLooseV1");

    MuIDs.push_back(make_pair("POGTightPFIsoTight","HNVeto2016"));
    mu_loose_id.push_back("HNLooseV1");


    MuIDs.push_back(make_pair("POGTightStandardPFIsoVeryVeryTight","HNVeto2016"));
    mu_loose_id.push_back("HNLooseStandard");                                         

    MuIDs.push_back(make_pair("POGTightStandardPFIsoTight","HNVeto2016"));
    mu_loose_id.push_back("HNLooseStandard");  


    MuIDs.push_back(make_pair("POGTightPFIsoMedium","HNVeto2016"));
    mu_loose_id.push_back("HNLooseV1");

    MuIDs.push_back(make_pair("POGTightPFIsoLoose","HNVeto2016"));
    mu_loose_id.push_back("HNLooseV1");

    MuIDs.push_back(make_pair("HNTightV1", "HNVeto2016"));
    mu_loose_id.push_back("HNLooseV1");

    MuIDs.push_back(make_pair("HNTightV2", "HNVeto2016"));
    mu_loose_id.push_back("HNLooseV1");


    MuIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
    mu_loose_id.push_back("HNLoose2016");

    MuIDs.push_back(make_pair("POGHighPtTight", "HNVeto2016"));
    mu_loose_id.push_back("POGHighPtLoose");
    */
    int iel= MuIDs.size() - channel.size();
    for (int i =0; i < iel; i++){
      channel.push_back("MuMu");
      el_loose_id.push_back("HNLoose2016");
      ELIDs.push_back(make_pair("HNVeto2016", "HNVeto2016"));
    }



  }
  
  if(IsData){
    if(isSingleMu){
      if(DataYear!=2016){
	Event ev = GetEvent();
	if(!ev.PassTrigger("HLT_Mu3_PFJet40_v")) return;
      } 
      else return;
    }
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
    param.Name = "FakeRateHN_"+ElectronTightID;
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
    param.Name = "FakeRateHN_"+channel[it_id] + "_" +Tight_ID;
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  } 
}

void FakeRateHN::executeEventFromParameter(AnalyzerParameter param, TString El_ID, TString channel){
  

  bool ee = true;
  if (channel == "MuMu") ee = false;

  //************************************************///
  // plot CR species cuts for All plots to be made
  //************************************************///

 
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
    weight *= weight_norm_1invpb*ev.MCweight();
  }

  //************************************************///  
  //==== No Cut cutflow entry, get nevents in sample
  //************************************************///   

  FillEventCutflow(0,0,weight, "NoCut",param.Name);
  FillEventCutflow(0,1,weight, "NoCut",param.Name);

  //************************************************///   
  //==== Event selecitions : MET Filters
  //************************************************///   

  if(!PassMETFilter()) return;
  FillEventCutflow(0,0,weight, "METFilter",param.Name);
  FillEventCutflow(0,1,weight, "METFilter",param.Name);


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

  //************************************************///   
  //==== PU reweight for MC
  //************************************************///   

  double weight_nopu=weight;
  if(!IsDATA){

    double this_pureweight = 1.;
    this_pureweight = weight_PU;

    weight *= this_pureweight;
  }
  
  //************************************************///   
  // 
  // setup analysis level lepton vectors
  //
  //************************************************///   
  
  std::vector<Electron> tight_electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 9.5, 2.5);
  std::vector<Muon>     tight_muons         = SelectMuons    (this_AllMuons,    param.Muon_Tight_ID, 5., 2.4);


  std::vector<Electron> loose_electrons = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 9.5, 2.5);
  std::vector<Muon>     loose_muons         = SelectMuons    (this_AllMuons,    param.Muon_Loose_ID, 5., 2.4);

 
  std::sort(tight_electrons.begin(), tight_electrons.end(), PtComparing);
  std::sort(tight_muons.begin(), tight_muons.end(), PtComparing);
  std::sort(loose_muons.begin(), loose_muons.end(), PtComparing);
  std::sort(loose_electrons.begin(),loose_electrons.end(), PtComparing);
  

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
    
    for(unsigned int iel=0 ; iel < loose_electrons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_electrons[iel]) < 0.4) jetok = false;
    }
    for(unsigned int iel=0 ; iel < loose_muons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_muons[iel]) < 0.4) jetok = false;
    }
    if(jetok) jets.push_back(jets_tmp[ijet]);
  }
  std::sort(jets.begin(), jets.end(), PtComparing);


  if(ee) RunE(tight_electrons, loose_electrons,tight_muons,loose_muons, jets, fatjets, ev,  param, weight);
  if(!ee) RunM(tight_electrons, loose_electrons,tight_muons,loose_muons,  jets, fatjets,ev, param, weight);
  
}



bool FakeRateHN::PassHNID(TString TightID, Electron el){

  
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
  return pass;



}

void FakeRateHN::RunM(std::vector<Electron> tight_el, std::vector<Electron> loose_el, std::vector<Muon> tight_mu, std::vector<Muon> loose_mu, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){
  
  TString label = param.Name;
  TString channel_s="_MuMu";
  Particle METv = ev.GetMETVector();

  if(loose_mu.size() != 1) return;
    
  if(isSingleMu) {
    if (IsData && DataYear!=2016){
      if(!ev.PassTrigger("HLT_Mu3_PFJet40_v")) return;
    }
  }
  
  // remove if muon                                                                                                                          
  if(loose_el.size() > 0) return;


  double isocut = GetIsoFromID("MuMu", param.Muon_Tight_ID,loose_mu[0].Eta(), loose_mu[0].Pt());
  GetFakeRateAndPromptRates(loose_mu,label, tight_mu,w,isocut);

  TString triggerslist_3="HLT_Mu3_PFJet40_v";                                                                                                                                  
  TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";                                                                                                                                  
  TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";        
  float prescale_trigger = GetPrescale(loose_mu,   ev.PassTrigger(triggerslist_3),ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_17));                     

  if(prescale_trigger == 0.) return;
  w*=prescale_trigger;

  bool truth_match= false;
  if(!IsData) {

    /// ONLY INCLUDE ELECTRONS FROM W/Z                                                                           
    gens = GetGens();
    if(GetLeptonType(loose_mu.at(0), gens) > 0 ) truth_match=true;

  }
  else truth_match=true;



  if(jets.size() > 0){
    if(jets.at(0).Pt() > 40.){
      FillRegionPlots("SingleLooseMuJet_"+param.Muon_Tight_ID , label, jets,  fatjets,  loose_el,loose_mu,  METv, w);

      if(truth_match)       FillRegionPlots("SingleLooseMuJet_matched_"+param.Muon_Tight_ID , label, jets,  fatjets, loose_el,  loose_mu,  METv, w);
      if(tight_mu.size()==1) {
        FillRegionPlots("SingleTightMuJet_"+param.Muon_Tight_ID , label, jets,  fatjets,  loose_el,loose_mu,  METv, w);
        if(truth_match) FillRegionPlots("SingleTightMuJet_matched_"+param.Electron_Tight_ID , label, jets,  fatjets, loose_el, loose_mu,  METv, w);
      }
    }
  }

  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int imu = 0; imu < tight_mu.size();  imu++){
    METdphi = TVector2::Phi_mpi_pi((tight_mu.at(imu).Phi()- METv.Phi()));
    MT = sqrt(2.* tight_mu.at(imu).Et()*METv.Pt() * (1 - cos( METdphi)));
  }
  if(METv.Pt()  > 30 && (60. < MT)  &&(MT < 100.) &&truth_match){
    if(jets.size() > 0 && tight_mu.size()==1){
      if(jets[0].Pt() > 40){
        if(truth_match) FillRegionPlots("SingleTightMu_promptCR_"+param.Muon_Tight_ID , label, jets,  fatjets,  loose_el,loose_mu, METv, w);

      }
    }
  }




  return;

} /// end function                      





int FakeRateHN::GetNBJets(){



  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jets   = SelectJets(this_AllJets,"HNTight", 20., 2.5);

  int NBJets=0;

  for(unsigned int i=0; i<jets.size(); i++){

    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium,                                                    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  return NBJets;
}

void FakeRateHN::RunE(std::vector<Electron> tight_el, std::vector<Electron> loose_el, std::vector<Muon> tight_mu, std::vector<Muon> loose_mu, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){


  TString channel_s="_EE";
  int i_channel=0;
  TString label = param.Name;
  Particle METv = ev.GetMETVector();
  
  if(loose_el.size() != 1) return;
 
  FillHist(("Electron" + param.Name + "_EHIso").Data(), loose_el[0].dr03EcalRecHitSumEt()+loose_el[0].dr03HcalDepth1TowerSumEt(),  1., 1200, -2., 10.,"");
  FillHist(("Electron" + param.Name + "_hovere").Data(), loose_el[0].HoverE(),  1., 2200, -2., 20.,"");
  FillHist(("Electron" + param.Name + "_trkiso").Data(), loose_el[0].TrkIso(),  1., 200, 0., 100.,"");
  FillHist(("Electron" + param.Name + "_dxy").Data(), abs(loose_el[0].dXY()),  1., 1000, 0., 1.,"");

  if(tight_el.size()==1){
    FillHist(("ElectronTight" + param.Name + "_EHIso").Data(), loose_el[0].dr03EcalRecHitSumEt()+loose_el[0].dr03HcalDepth1TowerSumEt(),  1., 1200, -2., 10.,"");
    FillHist(("ElectronTight" + param.Name + "_hovere").Data(), loose_el[0].HoverE(),  1., 2200, -2., 20.,"");
    FillHist(("ElectronTight" + param.Name + "_trkiso").Data(), loose_el[0].TrkIso(),  1., 200, 0., 100.,"");
    FillHist(("ElectronTight" + param.Name + "_dxy").Data(), abs(loose_el[0].dXY()),  1., 1000, 0., 1.,"");


  }


  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                                     
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_33="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"; /// 

  // remove if muon 
  if(loose_mu.size() > 0) return;

  double isocut = GetIsoFromID("EE", param.Electron_Tight_ID,loose_el[0].Eta(), loose_el[0].Pt());
  
  GetFakeRateAndPromptRates(loose_el,label, tight_el,isocut,w,param.Electron_Tight_ID);


  float prescale_trigger =  GetPrescale(loose_el,  ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_12), ev.PassTrigger(triggerslist_18), ev.PassTrigger(triggerslist_23), ev.PassTrigger(triggerslist_33));                                                                                                                               
																					       
  if(prescale_trigger == 0.) return;
  w*=prescale_trigger;
  
  bool truth_match= false;
  if(!IsData) {

    /// ONLY INCLUDE ELECTRONS FROM W/Z                                                                                                      
    gens = GetGens();
    if(GetLeptonType(loose_el.at(0), gens) > 0 ) truth_match=true;

    
  }
  else truth_match=true;
  
  
  
  if(jets.size() > 0){
    if(jets.at(0).Pt() > 40.){
      FillRegionPlots("SingleLooseElJet_"+param.Electron_Tight_ID , label, jets,  fatjets,  loose_el, loose_mu,  METv, w);
     
      if(truth_match)       FillRegionPlots("SingleLooseElJet_matched_"+param.Electron_Tight_ID , label, jets,  fatjets, loose_el, loose_mu,  METv, w);
      if(tight_el.size()==1) {
	FillRegionPlots("SingleTightElJet_"+param.Electron_Tight_ID , label, jets,  fatjets,  loose_el, loose_mu,  METv, w);
	if(truth_match) FillRegionPlots("SingleTightElJet_matched_"+param.Electron_Tight_ID , label, jets,  fatjets,  loose_el, loose_mu , METv, w);
      }
    }
  }
  
  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int iel = 0; iel < tight_el.size();  iel++){
    METdphi = TVector2::Phi_mpi_pi((tight_el.at(iel).Phi()- METv.Phi()));
    MT = sqrt(2.* tight_el.at(iel).Et()*METv.Pt() * (1 - cos( METdphi)));
  }
  if(METv.Pt()  > 30 && (60. < MT)  &&(MT < 100.) &&truth_match){
    if(jets.size() > 0 && tight_el.size()==1){
      if(jets[0].Pt() > 40){
	if(truth_match) FillRegionPlots("SingleTightEl_promptCR_"+param.Electron_Tight_ID , label, jets,  fatjets,  loose_el, loose_mu, METv, w);
	
      }
    }
  }

} /// end function RunE



void FakeRateHN::GetFakeRateAndPromptRates(std::vector<Muon> muonLooseColl, TString mutag, std::vector<Muon> muonTightColl, float w, float isocut){


  //  TString triggerslist_3="HLT_Mu3_PFJet40_v";
  //TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
  //TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

  if(muonLooseColl.size()<1) return;
  
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  
  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jetColl   = SelectJets(this_AllJets, "HNTight", 20., 2.5);
  std::vector<Jet>    jetCollTight ;
  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    bool jetok=true;

    for(unsigned int iel=0 ; iel < muonLooseColl.size(); iel++){
      if(jetColl[ijet].DeltaR(muonLooseColl[iel]) < 0.4) jetok = false;
    }
    if(jetok) jetCollTight.push_back(jetColl[ijet]);
  }
  std::sort(jetCollTight.begin(), jetCollTight.end(), PtComparing);


  float prescale_trigger =  1.;//GetPrescale(muonLooseColl,   ev.PassTrigger(triggerslist_3),ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_17));

  if(muonLooseColl.size()==1)MakeFakeRatePlots("", mutag, muonTightColl ,muonLooseColl,  jetCollTight, jetColl,  prescale_trigger, w, isocut, METv);
  return;
}

void FakeRateHN::GetFakeRateAndPromptRates(std::vector<Electron> electronLooseColl, TString eltag, std::vector<Electron> electronTightColl, float isocut,  float w, TString tightid ){
  
  
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_33="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"; /// --> no 33 trigger
  
  TString triggerslist="HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v";
  
  float id_weight=1.;
  float reco_weight=1.;

  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jetColl   = SelectJets(this_AllJets, "HNTight", 20., 2.5);
  std::vector<Jet>    jetCollTight ;
  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    bool jetok=true;

    for(unsigned int iel=0 ; iel < electronLooseColl.size(); iel++){
      if(jetColl[ijet].DeltaR(electronLooseColl[iel]) < 0.4) jetok = false;
    }
    if(jetok) jetCollTight.push_back(jetColl[ijet]);
  }
  std::sort(jetCollTight.begin(), jetCollTight.end(), PtComparing);
  
  if(electronLooseColl.size()<1) return;
  

  float prescale_trigger =  1.;//GetPrescale(electronLooseColl,  ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_12), ev.PassTrigger(triggerslist_18), ev.PassTrigger( triggerslist_23), ev.PassTrigger(triggerslist_33)); 
  
  

  MakeFakeRatePlots(tightid, eltag, electronTightColl,electronLooseColl,  jetCollTight, jetColl,  prescale_trigger, isocut,w, METv);
  
  
  return;
  
}

float FakeRateHN::GetPrescale( std::vector<Muon> muons,bool pass3, bool pass2, bool pass1 ){

  Event ev = GetEvent();

  float prescale_trigger= 1.;
  if(muons.size() != 1) return 0.;
  if(muons.size() ==1){

    /// 10 - 20  HLT_Mu7
    /// 20 - INF  HLT_Mu17
    
    if(muons.at(0).Pt() >= 20.){
      if(pass1){
	if(IsData) return 1.;
	prescale_trigger = ev.GetTriggerLumi("HLT_Mu17_TrkIsoVVL_v");
      }
      else {
	if(IsData) return 0;
	return 0;
      }
    }
    else  if(muons.at(0).Pt() >= 10.){

      if(pass2){
	if(IsData) return 1.;
        prescale_trigger = ev.GetTriggerLumi("HLT_Mu8_TrkIsoVVL_v") ; //// 20 + GeV bins
      }
      else {
	if(IsData) return 0;
	return 0;
      }
    }
    else  if(muons.at(0).Pt() >= 5.){
      if(pass3){
        if(IsData) return 1.;
        prescale_trigger =  ev.GetTriggerLumi("HLT_Mu3_PFJet40_v") ; //// 20 + GeV bins                                                                         
      }
      else {
        if(IsData) return 0;
	return 0;
      }
    }

  }
  if(prescale_trigger == 0.) return 0.;
  if(IsData) return 1.;
  if(muons.at(0).Pt() < 5.) return 0.;
  
  
  return prescale_trigger;
}


double FakeRateHN::GetPrescale( std::vector<Electron> electrons,bool pass5,  bool pass4, bool pass3, bool pass2, bool pass1 ){
  
  Event ev = GetEvent();

  float prescale_trigger= 1.;
  if(electrons.size() != 1) return 0.;
  if(electrons.size() ==1){

    /// 10 - 15  HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_*
    /// 15 - 20  HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_*
    /// 20 - 25  HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_*
    /// 25 - 35  HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_* 
    /// 35 - inf HLT_Ele33_CaloIdL_TrackIdL_IsoVL_PFJet30_*

    
    if(electrons.at(0).Pt() >= 25.){
      //HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30

      if(pass2){
	if(IsDATA) return 1.;
        prescale_trigger =  ev.GetTriggerLumi("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v") ; //// 20 + GeV bins
      }
      else {
	if(IsDATA) return 0;
	return 0;
	//prescale_trigger =  ev.GetTriggerLumi("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v") * 0.8; 
      }
    }
    else   if(electrons.at(0).Pt() >= 20.){
      //HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30
      if(pass3){
	if(IsDATA) return 1.;
	prescale_trigger = ev.GetTriggerLumi("HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v");
      }
      else {
	if(IsDATA) return 0;
	return 0;
	//prescale_trigger = ev.GetTriggerLumi("HLT_Ele17_CaloIdL_TrackIdL_IsoVL_v")*0.8 ;
      }
    }
    else   if(electrons.at(0).Pt() >= 15.){
      //HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_
      if(pass4){
	if(IsDATA) return 1.;
        prescale_trigger = ev.GetTriggerLumi("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v") ;
      }
      else {
	if(IsDATA) return 0;
	return 0;
        //prescale_trigger = ev.GetTriggerLumi("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v")*0.8 ;
      }
    }
    else   if(electrons.at(0).Pt() >= 9.5){
      //HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_                                                                                                                               
      if(pass5){
        if(IsDATA) return 1.;
        prescale_trigger = ev.GetTriggerLumi("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v") ;
      }
      else {
        if(IsDATA) return 0;
	return 0;
        //prescale_trigger = ev.GetTriggerLumi("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v")*0.8 ;
      }
    }
    else{
      prescale_trigger = 0.;
    }
  }
  if(prescale_trigger == 0.) return 0.;
  if(IsData) return 1.;
  if(electrons.at(0).Pt() < 9.5) return 0.;
  

  
  return prescale_trigger;
}


void FakeRateHN::MakeFakeRatePlots(TString label, TString mutag,   std::vector<Muon> muons_tight, std::vector<Muon> muons,  std::vector<Jet> jets, std::vector<Jet> alljets, float prescale_w, float w, float isocut, Particle MET){


  if(muons.size() != 1 ) return;

  if(prescale_w==0.) return;

  if(!IsData)gens = GetGens();

  bool useevent20 = UseEvent(muons , jets, 20., prescale_w, MET,  w);
  bool useevent30 = UseEvent(muons , jets, 30., prescale_w, MET,w);
  bool useevent40 = UseEvent(muons , jets, 40., prescale_w, MET,w);
  bool useevent60 = UseEvent(muons , jets, 60., prescale_w, MET,w);

  bool truth_match= false;
  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT MUONS                                                                                                    
  if(!IsData) {
    if(muons.size() > 0){
      /// ONLY INCLUDE MUONS FROM W/Z                                                                                                                     
      if(GetLeptonType(muons.at(0), gens) > 0 ) truth_match=true;
    }
    else truth_match = true;
  }
  else truth_match=true;

  label= mutag;
  if(truth_match){
    if(jets.size() >= 1){
      if(useevent20)GetFakeRates(muons, muons_tight,label, jets, alljets,  label+"_20",(prescale_w * w),isocut);
      if(useevent30)GetFakeRates(muons, muons_tight,label, jets, alljets,  label+"_30",(prescale_w * w),isocut);
      if(useevent40)GetFakeRates(muons, muons_tight,label, jets, alljets,  label+"_40",(prescale_w * w),isocut);
      if(useevent60)GetFakeRates(muons, muons_tight, label,jets, alljets,  label+"_60",(prescale_w * w),isocut);
    }
  }
}


void FakeRateHN::MakeFakeRatePlots(TString label, TString eltag,   std::vector<Electron> electrons_tight, std::vector<Electron> electrons,  std::vector<Jet> jets, std::vector<Jet> alljets, float prescale_w, float isocut ,float w, Particle MET){
  
  if(electrons.size() != 1 ) return;
  
  if(prescale_w==0.) return;
  
  if(!IsData)gens = GetGens();

  bool useevent20 = UseEvent(electrons , jets, 20., prescale_w, MET,  w); 
  bool useevent30 = UseEvent(electrons , jets, 30., prescale_w, MET,w); 
  bool useevent40 = UseEvent(electrons , jets, 40., prescale_w, MET,w); 
  bool useevent60 = UseEvent(electrons , jets, 60., prescale_w, MET,w); 

  bool truth_match= false;
  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT ELECTRONS
  if(!IsData) {
    if(electrons.size() > 0){
      /// ONLY INCLUDE ELECTRONS FROM W/Z
      if(GetLeptonType(electrons.at(0), gens) > 0 )  truth_match=true;
    }
    else truth_match = true;
  }
  else truth_match=true;
  
  label= eltag;
  


  if(truth_match){
    if(jets.size() >= 1){

      if(useevent20)GetFakeRates(electrons, electrons_tight,label, jets, alljets,  label+"_20", isocut,(w));
      if(useevent30)GetFakeRates(electrons, electrons_tight,label, jets, alljets,  label+"_30", isocut,(w));
      if(useevent40)GetFakeRates(electrons, electrons_tight,label, jets, alljets,  label+"_40", isocut,(w));
      if(useevent60)GetFakeRates(electrons, electrons_tight, label,jets, alljets,  label+"_60", isocut,(w));
    }
  }
}

bool FakeRateHN::UseEvent(std::vector<Muon> muons,  std::vector< Jet> jets, float awayjetcut, float precale_weight, Particle MET, float wt){
  bool useevent = false;
  if(muons.size() != 1) return false;
  if(precale_weight != 0.){
    /// Z and W veto                                                                                                                                          
    Double_t MT=0;
    Double_t METdphi=0;
    for(unsigned int w = 0; w < muons.size();  w++){
      METdphi = TVector2::Phi_mpi_pi(muons.at(w).Phi()-MET.Phi());
      MT = sqrt(2.* muons.at(w).Et()*MET.Pt() * (1 - cos( METdphi)));

      if(( (MET.Pt() < 20) && (MT < 25.)) ) {

        for (unsigned int ielT=0; ielT < muons.size(); ielT++){
          for(unsigned int ij=0; ij < jets.size(); ij++){
            if(jets.at(ij).Pt() < awayjetcut) continue;
            float dphi =fabs(TVector2::Phi_mpi_pi(muons.at(ielT).Phi()- jets.at(ij).Phi()));
	    if(dphi > 2.5){
	      if((jets.at(ij).Pt() /  muons.at(ielT).Pt() ) < 1.) continue;
	      useevent = true;
	    }
          }
        }
      }
    }
  }
  return useevent;

}
bool FakeRateHN::UseEvent(std::vector<Electron> electrons,  std::vector< Jet> jets, float awayjetcut, float precale_weight, Particle MET, float wt){
  
  bool useevent = false;
  if(electrons.size() != 1) return false;
  if(precale_weight != 0.){
    /// Z and W veto
    Double_t MT=0;
    Double_t METdphi=0;
    for(unsigned int w = 0; w < electrons.size();  w++){
      METdphi = TVector2::Phi_mpi_pi(electrons.at(w).Phi()-MET.Phi());
      MT = sqrt(2.* electrons.at(w).Et()*MET.Pt() * (1 - cos( METdphi)));

      if(( (MET.Pt() < 20) && (MT < 25.)) ) {
	
        for (unsigned int ielT=0; ielT < electrons.size(); ielT++){
          for(unsigned int ij=0; ij < jets.size(); ij++){
            if(jets.at(ij).Pt() < awayjetcut) continue;
            float dphi =fabs(TVector2::Phi_mpi_pi(electrons.at(ielT).Phi()- jets.at(ij).Phi()));
            if( (jets.at(ij).ChargedEmEnergyFraction()) > 0.65)  continue;
	    if(dphi > 2.5) useevent = true;
          }
        }
      }
    }
  }
  return useevent;
}


void FakeRateHN::GetFakeRates(std::vector<Muon> loose_mu, std::vector<Muon> tight_mu, TString tightlabel,  std::vector<Jet> jets,  std::vector<Jet> lljets, TString tag,double w, float isocut){

  double ptbinscone[13] = { 6.,10., 15.,20.,25.,30.,35.,40.,50., 60., 80., 100.,200.};
  double ptbins[13]     = { 5., 10., 15.,20.,25.,30.,35.,40.,50.,60.,80.,100., 200.};
  double etabins[5] = { 0.,0.8,  1.479, 2.,  2.5};

  if(tag.Contains("HighPt")){
    if(loose_mu.size() == 1){
      if(loose_mu.at(0).Pt() < 60.) return;
      //if(loose_mu[0].CalcPtCone(loose_mu[0].TrkIso()/loose_mu[0].TuneP4().Pt(), isocut) < 60.) return;
    }
  }
  Event ev = GetEvent();
  if(loose_mu.size() == 1 && jets.size() >= 1){
    float mu_pt = loose_mu.at(0).Pt();
    float mu_pt_corr =  loose_mu[0].CalcPtCone(loose_mu[0].RelIso(), isocut);
    if(tag.Contains("HighPt")) mu_pt_corr=mu_pt;
    if(mu_pt_corr > 200.) mu_pt_corr = 200.;

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";


    float weight_ptcorr=w;

    bool fill_plot=false;
    if(mu_pt_corr< 5) return;
    else if (mu_pt_corr > 5  && mu_pt_corr < 15){
      if(mu_pt > 5.) {
	
	if(ev.PassTrigger(triggerslist_3)) {
	  fill_plot=true;
	  if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_3);
	}
	else {
	  fill_plot=false;
	  weight_ptcorr = 0.;
	}
	
	if(IsData){
	  if(DataYear!=2016){
	    if(!isSingleMu) {
	      fill_plot=false;
	      weight_ptcorr = 0.;
	    }
	  }
	}
      }
    }


    else if ( mu_pt_corr < 30){
      if(mu_pt > 10) {

	if(ev.PassTrigger(triggerslist_8)) {
	  fill_plot=true;
          if(!IsData)weight_ptcorr= w * ev.GetTriggerLumi(triggerslist_8);
	}
	else{
	  fill_plot=false;
          weight_ptcorr= 0.;
	}
     
	if(IsData){
	  if(DataYear!=2016){
	    if(isSingleMu) {
	      fill_plot=false;
	      weight_ptcorr = 0.;
	    }
	  }
	}

      }

    }

    else {
      if(mu_pt > 20) {
	if(ev.PassTrigger(triggerslist_17)) {
	  fill_plot=true;
	  if(!IsData) weight_ptcorr= w * ev.GetTriggerLumi(triggerslist_17);
	}
	else{
	  fill_plot=false;
          weight_ptcorr= 0.;
	}
	if(IsData){
          if(DataYear!=2016){
            if(isSingleMu) {
              fill_plot=false;
              weight_ptcorr = 0.;
            }
          }
        }
      }
    }
    
    /// Get precale weiht for normal pt bins
    double prescale_mu = GetPrescale(loose_mu,   ev.PassTrigger(triggerslist_3),ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_17));
      
    FillHist(("LooseMu" + tag + "_pt_eta").Data(), mu_pt, fabs(loose_mu.at(0).Eta()),  prescale_mu*w, 12, ptbins, 4 , etabins);
    FillHist(("LooseMu" + tag + "_pt").Data(), mu_pt,   prescale_mu*w, 12, ptbins, "p_{T} (GeV)");
    FillHist(("LooseMu" + tag + "_eta").Data(), mu_pt,   prescale_mu*w, 4, etabins,"#eta");
    if(fill_plot){
      FillHist(("LooseMu" + tag + "_ptcone_eta").Data(), mu_pt_corr, fabs(loose_mu.at(0).Eta()),  weight_ptcorr, 12, ptbinscone, 4 , etabins);
      if(mu_pt > 8.)       FillHist(("LooseMu" + tag + "_ptcone_ptfix_eta").Data(), mu_pt_corr, fabs(loose_mu.at(0).Eta()),  weight_ptcorr, 12, ptbinscone, 4 , etabins);
      if(fabs(loose_mu.at(0).Eta()) < 1.5){
	FillHist(("LooseMu" + tag + "_barrel_ptcone").Data(), mu_pt_corr,  weight_ptcorr, 12,ptbinscone,"p_{T} cone (GeV)");
	if(mu_pt > 8.) FillHist(("LooseMu" + tag + "_barrel_ptcone_ptfix").Data(), mu_pt_corr,  weight_ptcorr, 12,ptbinscone,"p_{T} cone (GeV)");
      }
      else{
	FillHist(("LooseMu" + tag + "_endcap_ptcone").Data(), mu_pt_corr,  weight_ptcorr, 12,ptbinscone,"p_{T} cone (GeV)");
	if(mu_pt > 8.) FillHist(("LooseMu" + tag + "_endcap_ptcone_ptfix").Data(), mu_pt_corr,  weight_ptcorr, 12,ptbinscone,"p_{T} cone (GeV)");

      }
    }
    
    if( tight_mu.size() == 1){
      FillHist(("TightMu" + tag + "_pt_eta").Data(), mu_pt, fabs(tight_mu.at(0).Eta()),  w*prescale_mu, 12, ptbins, 4 , etabins);
      FillHist(("TightMu" + tag + "_pt").Data(), mu_pt,  w*prescale_mu, 8, ptbins, "p_{T} (GeV)");
      FillHist(("TightMu" + tag + "_eta").Data(),fabs(tight_mu.at(0).Eta()),  w*prescale_mu, 4, etabins,"#eta");
      if(fill_plot){
	FillHist(("TightMu" + tag + "_ptcone_eta").Data(), mu_pt_corr, fabs(tight_mu.at(0).Eta()), weight_ptcorr, 12, ptbinscone, 4 , etabins);
	if(mu_pt > 8.) FillHist(("TightMu" + tag + "_ptcone_ptfix_eta").Data(), mu_pt_corr, fabs(tight_mu.at(0).Eta()), weight_ptcorr, 12, ptbinscone, 4 , etabins);
	if(fabs(loose_mu.at(0).Eta()) < 1.5){
	  
	  FillHist(("TightMu" + tag + "_barrel_ptcone").Data(), mu_pt_corr,  weight_ptcorr, 12, ptbinscone,"p_{T} cone (GeV)");
	  if(mu_pt > 8.) FillHist(("TightMu" + tag + "_barrel_ptcone_ptfix").Data(), mu_pt_corr,  weight_ptcorr, 12, ptbinscone,"p_{T} cone (GeV)");
	  
	}
	else{
	  
          FillHist(("TightMu" + tag + "_endcap_ptcone").Data(), mu_pt_corr,  weight_ptcorr, 12, ptbinscone,"p_{T} cone (GeV)");
	  if(mu_pt > 8.) FillHist(("TightMu" + tag + "_endcap_ptcone_ptfix").Data(), mu_pt_corr,  weight_ptcorr, 12, ptbinscone,"p_{T} cone (GeV)");
	}
      }
    }
  }
  return;
}

void FakeRateHN::GetFakeRates(std::vector<Electron> loose_el, std::vector<Electron> tight_el, TString tightlabel,  std::vector<Jet> jets,  std::vector<Jet> alljets, TString tag,float isocut, double w){
  

  double ptconebins[11] = { 10., 15.,23.,30.,35., 40.,50.,60.,80.,100.,200.};
  double ptbins[12]     = { 10., 15.,20.,25.,30., 35.,40.,50.,60.,80.,100., 200.};
  double etabins[5] = { 0.,0.8,  1.479, 2.,  2.5};


  /// for most cuts just plot pt_eta
  Event ev = GetEvent();  

  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                    
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";



  if(loose_el.size() != 1) return;
  
  if(tag.Contains("HEEP")){
    if(loose_el.size() == 1){
      if(loose_el.at(0).Pt() < 35.) return;
    }    
  }  

  float el_pt = loose_el.at(0).Pt();
  float el_pt_corr =  loose_el[0].CalcPtCone(loose_el[0].RelIso(), isocut);
  if(el_pt_corr > 200.) el_pt_corr = 200.;

  
  float weight_ptcorr=w;
  bool fill_plot=false;
  
  if (el_pt_corr > 10  && el_pt_corr < 23){
    if(el_pt > 10) {
      if(ev.PassTrigger(triggerslist_8)) {
	fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_8);
      }
      else{
	fill_plot=false;
        weight_ptcorr = 0.;
      }
    }
  }
  else if ( el_pt_corr < 35){
    if(el_pt > 15) {
      if(ev.PassTrigger(triggerslist_12)) {
	fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_12);
      }
      else{
	fill_plot=false;
        weight_ptcorr = 0.;
      }
    }
  }
    
  else if (el_pt_corr < 40){
    if(el_pt > 20) {
      if(ev.PassTrigger(triggerslist_18)) {
	fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_18);
      }

      else{
	fill_plot=false;
        weight_ptcorr = 0.;

      }
    }
  }
  else{
    if(el_pt > 25) {
      if(ev.PassTrigger(triggerslist_23)) {
        fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_23);
      } 

      else{
        fill_plot=false;
        weight_ptcorr = 0.;

      }
    }// pt 
  }// end ptbin check

  
  if(loose_el.size() == 1 && jets.size() >= 1){
    

    // weight for pt binning
    double prescale = GetPrescale(loose_el,  ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_12), ev.PassTrigger(triggerslist_18), ev.PassTrigger( triggerslist_23), ev.PassTrigger(triggerslist_23));

    // PT BINNING 
    if(el_pt > 10.)FillHist(("LooseEl" + tag + "_pt_eta").Data(), el_pt, fabs(loose_el.at(0).Eta()),  w*(prescale), 11, ptbins, 4 , etabins);
    if(el_pt > 10.)FillHist(("LooseEl" + tag + "_pt").Data(), el_pt, w*(prescale), 11, ptbins,"p_{T}  (GeV)");
    if(el_pt > 10.)FillHist(("LooseEl" + tag + "_eta").Data(), fabs(loose_el.at(0).Eta()), w*(prescale), 4 , etabins,"#eta");
    // PT CONE
    if(fill_plot){
      FillHist(("LooseEl" + tag + "_ptcone_eta").Data(), el_pt_corr, fabs(loose_el.at(0).Eta()),  weight_ptcorr, 10, ptconebins, 4 , etabins);
      if(el_pt > 12)FillHist(("LooseEl" + tag + "_ptcone_ptfix_eta").Data(), el_pt_corr, fabs(loose_el.at(0).Eta()),  weight_ptcorr, 10, ptconebins, 4 , etabins);
    }

    if(fabs(loose_el.at(0).Eta()) <1.5){
      if(fill_plot){
	FillHist(("LooseEl" + tag + "_barrel_ptcone").Data(), el_pt_corr, weight_ptcorr,10, ptconebins,"p_{T} cone (GeV)");
	if(el_pt > 12)FillHist(("LooseEl" + tag + "_barrel_ptcone_ptfix").Data(), el_pt_corr, weight_ptcorr,10, ptconebins,"p_{T} cone (GeV)");
      }
    }
    else{
      if(fill_plot){
	FillHist(("LooseEl" + tag + "_endcap_ptcone").Data(), el_pt_corr, weight_ptcorr,10, ptconebins,"p_{T} cone (GeV)");
	if(el_pt > 12)FillHist(("LooseEl" + tag + "_endcap_ptcone_ptfix").Data(), el_pt_corr, weight_ptcorr,10, ptconebins,"p_{T} cone (GeV)");
      }

    }
    
    if( tight_el.size() == 1){
      if(fill_plot){
	FillHist(("TightEl" + tag + "_ptcone_eta").Data(), el_pt_corr, fabs(tight_el.at(0).Eta()),  weight_ptcorr, 10, ptconebins, 4 , etabins);
	if(el_pt > 12)FillHist(("TightEl" + tag + "_ptcone_ptfix_eta").Data(), el_pt_corr, fabs(tight_el.at(0).Eta()),  weight_ptcorr, 10, ptconebins, 4 , etabins);
      }

      if(fabs(loose_el.at(0).Eta()) <1.5){
	if(fill_plot){
	  FillHist(("TightEl" + tag + "_barrel_ptcone").Data(), el_pt_corr,  weight_ptcorr, 10, ptconebins, "p_{T} cone (GeV)");
	  if(el_pt > 12)FillHist(("TightEl" + tag + "_barrel_ptcone_ptfix").Data(), el_pt_corr,  weight_ptcorr, 10, ptconebins, "p_{T} cone (GeV)");
	}
      }
      else{
	if(fill_plot){
	  FillHist(("TightEl" + tag + "_endcap_ptcone").Data(), el_pt_corr,  weight_ptcorr, 10, ptconebins, "p_{T} cone (GeV)");
	  if(el_pt > 12) FillHist(("TightEl" + tag + "_endcap_ptcone_ptfix").Data(), el_pt_corr,  weight_ptcorr, 10, ptconebins, "p_{T} cone (GeV)");
	}

      }
      if(el_pt > 10.)FillHist(("TightEl" + tag + "_pt_eta").Data(), el_pt, fabs(tight_el.at(0).Eta()),  w*prescale, 11, ptbins, 4 , etabins);
      if(el_pt > 10.)     FillHist(("TightEl" + tag + "_pt").Data(), el_pt, w*(prescale), 11, ptbins,"p_{T}  (GeV)");
      if(el_pt > 10.)FillHist(("TightEl" + tag + "_eta").Data(), fabs(tight_el.at(0).Eta()), w*(prescale), 4 , etabins,"#eta");

    }
  }
  
  return;
  
}


FakeRateHN::FakeRateHN(){
  


}
 
 FakeRateHN::~FakeRateHN(){
   delete rand_;

}

void FakeRateHN::FillRegionPlots( TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met,  double w){
  
  if((els.size()+ mus.size()) != 1) return;
  Particle lep1;
  if(els.size()==1) lep1=els[0];
  else lep1 = mus[0];

  FillHist( label_1+ "/"+ label_1 +  "_lep1_pt_" + label_2, lep1.Pt()  , w, 400, 0., 2000.,"p_{T} GeV");

  FillHist( label_1+ "/"+ label_1 +  "_lep1_eta_" + label_2, lep1.Eta()  , w, 30, -3., 3,"#eta");
  
  FillHist( label_1+ "/"+ label_1 +  "_met_" + label_2, met.Pt()  , w, 200, 0., 400.,"MET GeV");

  FillHist( label_1+ "/"+ label_1 +  "_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");

}



double FakeRateHN::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}


double FakeRateHN::MT(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
  
}


std::vector<Jet> FakeRateHN::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

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

bool FakeRateHN::LeptonPassID(Lepton &lepton, TString ID){

  if(lepton.LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)&lepton;
    return el->PassID(ID);
  }
  else if(lepton.LeptonFlavour()==Lepton::MUON){
    Muon *mu = (Muon *)&lepton;
    return mu->PassID(ID);
  }
  else{
    cerr << "[FakeRateHN::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}


void FakeRateHN::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){


  return;

}

void FakeRateHN::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void FakeRateHN::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

double FakeRateHN::LSFSF(int lepflav, int dir){

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
    cerr << "[FakeRateHN::LSFSF] wrong lepflav : " <<  lepflav << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }

}
