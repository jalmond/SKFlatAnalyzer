#include "HNWRAnalyzer.h"

void HNWRAnalyzer::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");

  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  PromptLeptonOnly = HasFlag("PromptLeptonOnly");
  ApplyDYPtReweight = HasFlag("ApplyDYPtReweight");
  RunXsecSyst = HasFlag("RunXsecSyst");
  Signal = HasFlag("Signal");

  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunFake = " << RunFake << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunCF = " << RunCF << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunSyst = " << RunSyst << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] PromptLeptonOnly = " << PromptLeptonOnly << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] ApplyDYPtReweight = " << ApplyDYPtReweight << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] RunXsecSyst = " << RunXsecSyst << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer()] Signal = " << Signal << endl;

  //===============================
  //==== Year-dependent variables
  //===============================

  //==== Triggers

  Triggers_Electron.clear();
  Triggers_Muon.clear();

  if(DataYear==2016){

    Triggers_Electron = {
      "HLT_Ele27_WPTight_Gsf_v",
      "HLT_Photon175_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 30.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2017){

    Triggers_Electron = {
      "HLT_Ele35_WPTight_Gsf_v",
      "HLT_Photon200_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 38.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2018){

    Triggers_Electron = {
      "HLT_Ele32_WPTight_Gsf_v",
      "HLT_Photon200_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 35.;
    TriggerSafePt_Muon = 52.;

  }

  //==== Z-pt rewieght
  ZPtReweight = 1.;
  ZPtReweight_Up = 1.;
  ZPtReweight_Down = 1.;

  //==== b tagging
  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  mcCorr->SetJetTaggingParameters(jtps);

  //==== Signal finder
  genFinderSig = new GenFinderForHNWRSignal();
  SignalLeptonChannel = -1;

  //==== PUReweight
  if(!IsDATA){
    TString datapath = getenv("DATA_DIR");

    TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_"+TString::Itoa(DataYear,10)+".root";
    TString PUhname = "PUReweight_"+TString::Itoa(DataYear,10);
    //==== Only for 2016 FastSim, use different one..
    if(DataYear==2016 && IsFastSim){
      PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
    }

    cout << "[HNWRAnalyzer::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNWRAnalyzer::initializeAnalyzer()] PUhname = " << PUhname << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void HNWRAnalyzer::executeEvent(){

  //==========================
  //==== Gen for genmatching
  //==========================

  gens = GetGens();

  if(ApplyDYPtReweight){
    ZPtReweight = mcCorr->GetOfficialDYReweight(gens);
    ZPtReweight_Up = mcCorr->GetOfficialDYReweight(gens,+1);
    ZPtReweight_Down = mcCorr->GetOfficialDYReweight(gens,-1);
  }

  if(Signal){
    int genNpid = -1;
    for(unsigned int i=2; i<gens.size(); i++){
      int pid = abs( gens.at(i).PID() );
      if( pid==9900012 || pid==9900014 ){
        genNpid = pid;
        FillHist("GEN_N_Eta", gens.at(i).Eta(), 1., 60, -3., 3.);
        FillHist("GEN_N_Pt", gens.at(i).Pt(), 1., 800, 0., 8000.);
        break;
      }
    }
    //==== 0 = electron
    //==== 1 = muon
    if(genNpid==9900012){
      SignalLeptonChannel = 0;
      FillHist("SignalFlavour", 0., 1., 3, -1., 2.);
    }
    else if(genNpid==9900014){
      SignalLeptonChannel = 1;
      FillHist("SignalFlavour", 1., 1., 3, -1., 2.);
    }
    else{
      FillHist("SignalFlavour", -1., 1., 3, -1., 2.);
    }
  }

  //==== Prefire weight

  weight_Prefire      = GetPrefireWeight( 0);
  weight_Prefire_Up   = GetPrefireWeight(+1);
  weight_Prefire_Down = GetPrefireWeight(-1);

  //==== Nvtx

  N_VTX = nPV;
  if(!IsDATA){
    //==== nPileUp starts from 0
    N_VTX = nPileUp+1;
    //==== but histogram is as a function of nPileUp not nVTX
    int bin_pu = hist_PUReweight->FindBin(nPileUp);
    weight_PU = hist_PUReweight->GetBinContent(bin_pu);

    weight_PU_Up = hist_PUReweight_Up->GetBinContent(bin_pu);
    weight_PU_Down = hist_PUReweight_Down->GetBinContent(bin_pu);

  }

  //========================
  //==== AnalyzerParameter
  //========================

  //==== Central

  AnalyzerParameter param;
  param.Clear();

  param.Name = "HNWR";

  param.MCCorrrectionIgnoreNoHist = false;

  param.Electron_Tight_ID = "HNWRTight";
  if(DataYear>=2017 && IsFastSim) param.Electron_Tight_ID = "HNWRTightFastSim";
  param.Electron_Loose_ID = "HNWRLoose";
  param.Electron_ID_SF_Key = "HEEP";
  param.Electron_Trigger_SF_Key = "HEEP";
  param.Electron_FR_ID = "HNWR";
  param.Electron_FR_Key = "AwayJetPt40";
  param.Electron_CF_ID = "HNWRTight";
  param.Electron_CF_Key = "ZToLL";
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 53.;

  param.Muon_Tight_ID = "HNWRTight";
  param.Muon_Loose_ID = "HNWRLoose";
  param.Muon_RECO_SF_Key = "HighPtMuonRecoSF";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  param.Muon_Trigger_SF_Key = "POGHighPtLooseTrkIso";
  param.Muon_FR_ID = "HNWR";
  param.Muon_FR_Key = "AwayJetPt40";
  param.Muon_CF_ID = "HNWRTight";
  param.Muon_CF_Key = "ZToLL";
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_UseTuneP = true;
  param.Muon_MinPt = 53.;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  AllElectrons = GetAllElectrons();
  AllMuons = GetAllMuons();
  AllTunePMuons = UseTunePMuon( AllMuons );
  AllJets = GetAllJets();
  AllFatJets = puppiCorr->Correct( GetAllFatJets() ); // TODO correct SD Mass here, but not sure about the systematics

  param.dRSeparation = 0.4;

  executeEventFromParameter(param);

  if(RunSyst){

    for(int it_syst=1; it_syst<AnalyzerParameter::NSyst; it_syst++){
      param.syst_ = AnalyzerParameter::Syst(it_syst);

      //==== ZPtRwUp and ZPtRwDown are only ran when ApplyDYPtReweight
      //==== If not, skip
      if( (param.syst_==AnalyzerParameter::ZPtRwUp || param.syst_==AnalyzerParameter::ZPtRwDown) && !ApplyDYPtReweight ) continue;

      param.Name = "Syst_"+param.GetSystType()+"_HNWR";

      executeEventFromParameter(param);
    }
  }

}

void HNWRAnalyzer::executeEventFromParameter(AnalyzerParameter param){

  bool IsCentral = !( param.Name.Contains("Syst_") );

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();
  double weight = 1.;
  if(!IsDATA){

    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();

    //==== Apply Prefire
    if(param.syst_ == AnalyzerParameter::PrefireUp) weight *= weight_Prefire_Up;
    else if(param.syst_ == AnalyzerParameter::PrefireDown)  weight *= weight_Prefire_Down;
    else  weight *= weight_Prefire;

    //==== Apply ZPtReweight
    if(ApplyDYPtReweight){

      if(param.syst_ == AnalyzerParameter::ZPtRwUp) weight *= ZPtReweight_Up;
      else if(param.syst_ == AnalyzerParameter::ZPtRwDown) weight *= ZPtReweight_Down;
      else weight *= ZPtReweight;

    }

  }

  //=============
  //==== No Cut
  //=============

  FillCutFlow(IsCentral, "CutFlow", "NoCut_"+param.Name, weight);

  if(RunXsecSyst && param.syst_ == AnalyzerParameter::Central){
    double normweight = 1./sumW/PDFWeights_Scale->at(0);
    for(unsigned int i=0; i<PDFWeights_Scale->size(); i++){
      JSFillHist("XsecSyst_Den", "PDFWeights_Scale_"+TString::Itoa(i,10)+"_XsecSyst_Den", 0., PDFWeights_Scale->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
    }
    for(unsigned int i=0; i<PDFWeights_Error->size(); i++){
      JSFillHist("XsecSyst_Den", "PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Den", 0., PDFWeights_Error->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
    }
    for(unsigned int i=0; i<PDFWeights_AlphaS->size(); i++){
      JSFillHist("XsecSyst_Den", "PDFWeights_AlphaS_"+TString::Itoa(i,10)+"_XsecSyst_Den", 0., PDFWeights_AlphaS->at(i)*ev.MCweight()*normweight, 1, 0., 1.);
    }
  }

  //========================
  //==== Event selecitions
  //========================

  if(!PassMETFilter()) return;

  FillCutFlow(IsCentral, "CutFlow", "METFilter_"+param.Name, weight);

  bool PassSingleElectron = ev.PassTrigger(Triggers_Electron);
  bool PassMu50 = ev.PassTrigger(Triggers_Muon);
  if(IsFastSim){
    PassSingleElectron = true;
    PassMu50 = true;
  }
  //if(!PassSingleElectron && !PassMu50) return;

  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> this_AllTunePMuons = AllTunePMuons;
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  int SystDir_MuonRecoSF(0);
  int SystDir_ElectronRecoSF(0);

  int SystDir_MuonIDSF(0);
  int SystDir_ElectronIDSF(0);

  int SystDir_MuonISOSF(0);

  int SystDir_MuonTriggerSF(0);
  int SystDir_ElectronTriggerSF(0);

  int SystDir_LSFSF(0);
  int SystDir_PU(0);

  if(param.syst_ == AnalyzerParameter::Central){

  }
  else if(param.syst_ == AnalyzerParameter::JetResUp){
    this_AllJets = SmearJets( this_AllJets, +1 );
    this_AllFatJets = SmearFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetResDown){
    this_AllJets = SmearJets( this_AllJets, -1 );
    this_AllFatJets = SmearFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnUp){
    this_AllJets = ScaleJets( this_AllJets, +1 );
    this_AllFatJets = ScaleFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnDown){
    this_AllJets = ScaleJets( this_AllJets, -1 );
    this_AllFatJets = ScaleFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonRecoSFUp){
    SystDir_MuonRecoSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonRecoSFDown){
    SystDir_MuonRecoSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnUp){
    this_AllMuons = ScaleMuons( this_AllMuons, +1 );
    this_AllTunePMuons = ScaleMuons( this_AllTunePMuons , +1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnDown){
    this_AllMuons = ScaleMuons( this_AllMuons, -1 );
    this_AllTunePMuons = ScaleMuons( this_AllTunePMuons , -1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonIDSFUp){
    SystDir_MuonIDSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonIDSFDown){
    SystDir_MuonIDSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonISOSFUp){
    SystDir_MuonISOSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonISOSFDown){
    SystDir_MuonISOSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonTriggerSFUp){
    SystDir_MuonTriggerSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::MuonTriggerSFDown){
    SystDir_MuonTriggerSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronRecoSFUp){
    SystDir_ElectronRecoSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronRecoSFDown){
    SystDir_ElectronRecoSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronResUp){
    this_AllElectrons = SmearElectrons( this_AllElectrons, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronResDown){
    this_AllElectrons = SmearElectrons( this_AllElectrons, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronEnUp){
    this_AllElectrons = ScaleElectrons( this_AllElectrons, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronEnDown){
    this_AllElectrons = ScaleElectrons( this_AllElectrons, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronIDSFUp){
    SystDir_ElectronIDSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronIDSFDown){
    SystDir_ElectronIDSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFUp){
    SystDir_ElectronTriggerSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFDown){
    SystDir_ElectronTriggerSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::LSFSFUp){
    SystDir_LSFSF = +1;
  }
  else if(param.syst_ == AnalyzerParameter::LSFSFDown){
    SystDir_LSFSF = -1;
  }
  else if(param.syst_ == AnalyzerParameter::PUUp){
    SystDir_PU = +1;
  }
  else if(param.syst_ == AnalyzerParameter::PUDown){
    SystDir_PU = -1;
  }
  else if(param.syst_ == AnalyzerParameter::ZPtRwUp){

  }
  else if(param.syst_ == AnalyzerParameter::ZPtRwDown){

  }
  else if(param.syst_ == AnalyzerParameter::PrefireUp){

  }
  else if(param.syst_ == AnalyzerParameter::PrefireDown){

  }
  else{
    cerr << "[HNWRAnalyzer::executeEventFromParameter] Wrong syst : param.syst_ = " << param.syst_ << endl;
    exit(EXIT_FAILURE);
  }

  //==== PU reweight

  if(!IsDATA){

    double this_pureweight = 1.;
    if(SystDir_PU==0) this_pureweight = weight_PU;
    else if(SystDir_PU>0) this_pureweight = weight_PU_Up;
    else this_pureweight = weight_PU_Down;

    weight *= this_pureweight;
  }

  //==============
  //==== Leptons
  //==============

  //==== First, make base lepton vectors
  std::vector<Electron> myelectrons = SelectElectrons(this_AllElectrons, "HNWRLT", param.Electron_MinPt, 2.4);
  std::vector<Muon> mymuons = SelectMuons(this_AllTunePMuons, "HNWRLT", param.Muon_MinPt, 2.4);
  if(PromptLeptonOnly){
    myelectrons = ElectronPromptOnly(myelectrons, gens);
    mymuons = MuonPromptOnly(mymuons, gens);
  }
  std::sort(myelectrons.begin(), myelectrons.end(), PtComparing);
  std::sort(mymuons.begin(), mymuons.end(), PtComparing);

  //==== With this, make Loose and Tight lepton vectors, but use pointer
  std::vector<Electron *> Loose_electrons, Tight_electrons;
  std::vector<Muon *> Loose_muons, Tight_muons;
  std::vector<Lepton *> Tight_leps_el, Tight_leps_mu, Tight_leps;
  std::vector<Lepton *> Loose_leps_el, Loose_leps_mu, Loose_leps;
  for(unsigned int i=0; i<myelectrons.size(); i++){
    Electron& el = myelectrons.at(i);
    if(el.PassID(param.Electron_Loose_ID)){
      Loose_electrons.push_back( &el );
      Loose_leps_el.push_back( &el );
      Loose_leps.push_back( &el );
    }
    if(el.PassID(param.Electron_Tight_ID)){
      Tight_electrons.push_back( &el );
      Tight_leps_el.push_back( &el );
      Tight_leps.push_back( &el );
    }
  }
  for(unsigned int i=0; i<mymuons.size(); i++){
    Muon& mu = mymuons.at(i);
    if(mu.PassID(param.Muon_Loose_ID)){
      Loose_muons.push_back( &mu );
      Loose_leps_mu.push_back( &mu );
      Loose_leps.push_back( &mu );
    }
    if(mu.PassID(param.Muon_Tight_ID)){
      Tight_muons.push_back( &mu );
      Tight_leps_mu.push_back( &mu );
      Tight_leps.push_back( &mu );
    }
  }
  //==== Sorting leptons, not distinguishing flavour
  std::sort(Tight_leps.begin(), Tight_leps.end(), PtComparingPtr);

  //==== # of leptons
  int n_Loose_leptons = Loose_electrons.size()+Loose_muons.size();
  int n_Tight_leptons = Tight_electrons.size()+Tight_muons.size();
  //==== [CUT] : return if no tight lepton
  if(n_Tight_leptons==0) return;

  //==== [CUT] return if lead pt <= 60 GeV
  if(Tight_leps.at(0)->Pt()<=60.) return;

  //===========
  //==== Jets
  //===========

  std::vector<FatJet>   fatjets         = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 2.4);
  std::vector<FatJet>   fatjets_LSF    = SelectFatJets(this_AllFatJets, "HNLSF", 200, 2.4);
  std::sort(fatjets.begin(), fatjets.end(), PtComparing);
  std::sort(fatjets_LSF.begin(), fatjets_LSF.end(), PtComparing);

  std::vector<Jet>      jets         = SelectJets(this_AllJets, param.Jet_ID, 40., 2.4);
  std::sort(jets.begin(), jets.end(), PtComparing);

  int NBJets=0;
  for(unsigned int i=0; i<jets.size(); i++){
    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium,
                                                    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  //==============
  //==== Sum Pts
  //==============

  double HT(0.);
  for(unsigned int i=0; i<this_AllJets.size(); i++){
    HT += this_AllJets.at(i).Pt();
  }

  //===========================
  //==== Lepton scale factors
  //===========================

  double this_trigger_sf = 1.;

  //=====================
  //==== Categorization
  //=====================

  std::map<TString, bool> map_bool_To_Region;

  FatJet HNFatJet;
  Particle WRCand;
  Particle NCand;
  Particle NCand_1, NCand_2;

  //==== leps_for_plot
  //==== - If Resolved, at(0) is Tight Leading, at(1) is Tight Subleading
  //==== - If Boosted, at(0) is Tight Leading, at(1) is Loose inside the AK8jet
  vector<Lepton *> leps_for_plot;
  int NExtraLooseElectron(0),NExtraLooseMuon(0),NExtraLooseLepton(0);
  int NExtraTightElectron(0),NExtraTightMuon(0),NExtraTightLepton(0);

  //==== Check Resolved first

  bool IsResolvedEvent = false;
  bool IsResolved_SR_EE(false), IsResolved_SR_MM(false), IsResolved_SR_EM(false);
  bool IsResolved_LowWRCR_EE(false), IsResolved_LowWRCR_MM(false), IsResolved_LowWRCR_EM(false);
  bool IsResolved_DYCR_EE(false), IsResolved_DYCR_MM(false), IsResolved_DYCR_EM(false);

  if(n_Tight_leptons==2){

    FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+param.Name, weight);

    TString Suffix = "";
    bool tmp_IsEE(false), tmp_IsMM(false), tmp_IsEM(false);
    bool this_triggerpass(false);
    if(      (Tight_electrons.size()==2) && (Tight_muons.size()==0) ){
      tmp_IsEE = true;
      Suffix = "SingleElectron";
      this_triggerpass = PassSingleElectron;

      if(Signal){
        if(SignalLeptonChannel!=0) return;
      }

    }
    else if( (Tight_electrons.size()==0) && (Tight_muons.size()==2) ){
      tmp_IsMM = true;
      Suffix = "SingleMuon";
      this_triggerpass = PassMu50;

      if(Signal){
        if(SignalLeptonChannel!=1) return;
      }

    }
    else if( (Tight_electrons.size()==1) && (Tight_muons.size()==1) ){
      tmp_IsEM = true;
      Suffix = "EMu";
      this_triggerpass = PassMu50;
    }
    FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_"+param.Name, weight);

    if(this_triggerpass){

      FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_PassTrigger_"+param.Name, weight);

      //==== lljj

      if( jets.size()>=2 ){

        FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_TwoAK4Jets_"+param.Name, weight);

        Lepton *LeadLep = Tight_leps.at(0);
        Lepton *SubLeadLep = Tight_leps.at(1);

        bool dRLeadJetLepton    = ( jets.at(0).DeltaR( *LeadLep ) > param.dRSeparation ) && ( jets.at(0).DeltaR( *SubLeadLep ) > param.dRSeparation );
        bool dRSubLeadJetLepton = ( jets.at(1).DeltaR( *LeadLep ) > param.dRSeparation ) && ( jets.at(1).DeltaR( *SubLeadLep ) > param.dRSeparation );
        bool dRTwoLepton = (LeadLep->DeltaR( *SubLeadLep ) > param.dRSeparation);
        bool dRTwoJets = (jets.at(0).DeltaR( jets.at(1) ) > param.dRSeparation);

        if( dRLeadJetLepton && dRSubLeadJetLepton && dRTwoLepton && dRTwoJets ){

          FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_dRSeparation_"+param.Name, weight);

          IsResolvedEvent = true;

          WRCand = *LeadLep+*SubLeadLep+jets.at(0)+jets.at(1);
          NCand = *SubLeadLep+jets.at(0)+jets.at(1);
          NCand_1 = *LeadLep+jets.at(0)+jets.at(1);
          NCand_2 = *SubLeadLep+jets.at(0)+jets.at(1);

          double dilep_mass = (*LeadLep+*SubLeadLep).M();

          bool DiLepMassGT200 = (dilep_mass > 200.);
          bool DiLepMassGT400 = (dilep_mass > 400.);
          bool DiLepMassLT150 = (dilep_mass > 60.) && (dilep_mass < 150.);
          bool WRMassGT800 = ( WRCand.M() > 800. );

          //==== Now IsResolvedEvent is set
          //==== No overlap with Boosted, so we can apply SFs

          //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
          //@@@@ Apply scale factors

          double trigger_sf_SingleElectron = 1.;
          double trigger_sf_SingleMuon = 1.;

          if(!IsDATA){

            mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

            for(unsigned int i=0; i<Tight_electrons.size(); i++){
              double this_recosf = mcCorr->ElectronReco_SF(Tight_electrons.at(i)->scEta(),Tight_electrons.at(i)->Pt(), SystDir_ElectronRecoSF);
              double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, Tight_electrons.at(i)->scEta(), Tight_electrons.at(i)->Pt(), SystDir_ElectronIDSF);
              weight *= this_recosf*this_idsf;
            }
            for(unsigned int i=0; i<Tight_muons.size(); i++){

              double MiniAODP = sqrt( Tight_muons.at(i)->MiniAODPt() * Tight_muons.at(i)->MiniAODPt() + Tight_muons.at(i)->Pz() * Tight_muons.at(i)->Pz() );

              double this_recosf = mcCorr->MuonReco_SF(param.Muon_RECO_SF_Key, Tight_muons.at(i)->Eta(), MiniAODP, SystDir_MuonRecoSF);
              double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  Tight_muons.at(i)->Eta(), Tight_muons.at(i)->MiniAODPt(), SystDir_MuonIDSF);
              double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, Tight_muons.at(i)->Eta(), Tight_muons.at(i)->MiniAODPt(), SystDir_MuonISOSF);

              weight *= this_recosf*this_idsf*this_isosf;
            }

            //==== Trigger SF

            trigger_sf_SingleElectron = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, Tight_electrons, SystDir_ElectronTriggerSF);
            trigger_sf_SingleMuon = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons, SystDir_MuonTriggerSF);

          }
          if(Suffix=="SingleElectron"){
            this_trigger_sf = trigger_sf_SingleElectron;
            weight *= trigger_sf_SingleElectron;
          }
          else{
            trigger_sf_SingleElectron = trigger_sf_SingleMuon;
            weight *= trigger_sf_SingleMuon;
          }
          //@@@@
          //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

          if(DiLepMassGT200){

            FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_mllGT200_"+param.Name, weight);

            if(WRMassGT800){

              FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_mWRGT800_"+param.Name, weight);

              //==== Region Dictionary
              //==== - HNWR_SingleElectron_Resolved_SR : ee Resolved SR [IsResolved_SR_EE]
              //==== - HNWR_SingleMuon_Resolved_SR : mm Resolved SR [IsResolved_SR_MM]
              //==== - HNWR_EMu_Resolved_SR : em Resolved sideband [IsResolved_SR_EM]

              if(DiLepMassGT400){

                FillCutFlow(IsCentral, "CutFlow", "NTightLeptonIsTwo_"+Suffix+"_mWRGT800_mll400_"+param.Name, weight);

                map_bool_To_Region[Suffix+"_Resolved_SR"] = true;
                if(tmp_IsEE) IsResolved_SR_EE = true;
                else if(tmp_IsMM) IsResolved_SR_MM = true;
                else if(tmp_IsEM) IsResolved_SR_EM = true;

/*
                if( (IsResolved_SR_EE||IsResolved_SR_MM) ){
                  cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
                  cout << "Resolved Event" << endl;
                  cout << "----------------------------------------" << endl;
                  cout << "tmp_IsEE = " << tmp_IsEE << endl;
                  cout << "tmp_IsMM = " << tmp_IsMM << endl;
                  cout << "----------------------------------------" << endl;
                  cout << "[RECO]" << endl;
                  cout << "LeadLep : "; LeadLep->Print();
                  cout << "SubLeadLep : "; SubLeadLep->Print();
                  cout << "j1 : "; jets.at(0).Print();
                  cout << "j2 : "; jets.at(1).Print();
                  if(fatjets_LSF.size()>0){
                    cout << "LSF FatJet size = " << fatjets_LSF.size() << endl;
                    cout << "J1 : "; fatjets_LSF.at(0).Print();
                  }
                  cout << "-->" << endl;
                  cout << "(l1+l2+j1+j2) : "; (WRCand).Print();
                  cout << "(l2+j1+j2) : "; (NCand).Print();
                  cout << "dR(j1,j2) = " << jets.at(0).DeltaR(jets.at(1)) << endl;
                  cout << "dR(l2,j1) = " << SubLeadLep->DeltaR(jets.at(0)) << endl;
                  cout << "dR(l2,j2) = " << SubLeadLep->DeltaR(jets.at(1)) << endl;
                  cout << "----------------------------------------" << endl;
                  cout << "[GEN]" << endl;
                  cout << "WR : "; (genFinderSig->WR).Particle::Print();
                  cout << "Primary : "; (genFinderSig->priLep).Particle::Print();
                  cout << "N : "; (genFinderSig->N).Particle::Print();
                  cout << "Secondary : ";  (genFinderSig->secLep).Particle::Print();
                  cout << "jet1 : "; (genFinderSig->jet1).Particle::Print();
                  cout << "jet2 : "; (genFinderSig->jet2).Particle::Print();
                  cout << "-->" << endl;
                  cout << "WR matching : dR = " << WRCand.DeltaR( genFinderSig->WR ) << "; pt_gen = " << (genFinderSig->WR).Pt() << ", pt_reco = " << WRCand.Pt() << endl;
                  cout << "Primary lepton matching : dR = " << LeadLep->DeltaR( genFinderSig->priLep ) << "; pt_gen = " << (genFinderSig->priLep).Pt() << ", pt_reco = " <<LeadLep->Pt() << endl;
                  cout << "Secondary lepton matching : dR = " << SubLeadLep->DeltaR( genFinderSig->secLep ) << "; pt_gen = " << (genFinderSig->secLep).Pt() << ", pt_reco = " <<SubLeadLep->Pt() << endl;
                }
*/
              }

            }
            //==== Validate EMu method here
            else{

              //==== Region Dictionary
              //==== - HNWR_SingleElectron_Resolved_LowWRCR : ee Resolved, but low m(WR) [IsResolved_LowWRCR_EE]
              //==== - HNWR_SingleMuon_Resolved_LowWRCR : mm Resolved, but low m(WR) [IsResolved_LowWRCR_MM]
              //==== - HNWR_EMu_Resolved_LowWRCR : em Resolved, but low m(WR) [IsResolved_LowWRCR_EM]

              map_bool_To_Region[Suffix+"_Resolved_LowWRCR"] = true;
              if(tmp_IsEE) IsResolved_LowWRCR_EE = true;
              else if(tmp_IsMM) IsResolved_LowWRCR_MM = true;
              else if(tmp_IsEM) IsResolved_LowWRCR_EM = true;
            }

          }

          leps_for_plot.push_back( Tight_leps.at(0) );
          leps_for_plot.push_back( Tight_leps.at(1) );

          //==== Region Dictionary
          //==== - HNWR_SingleElectron_Resolved_DYCR : ee Resolved DYCR [IsResolved_DYCR_EE]
          //==== - HNWR_SingleMuon_Resolved_DYCR : mm Resolved DYCR [IsResolved_DYCR_MM]
          //==== - HNWR_EMu_Resolved_DYCR : filled, but NOT USED [IsResolved_DYCR_EM]

          if(DiLepMassLT150){
            map_bool_To_Region[Suffix+"_Resolved_DYCR"] = true;
            if(tmp_IsEE) IsResolved_DYCR_EE = true;
            else if(tmp_IsMM) IsResolved_DYCR_MM = true;
            else if(tmp_IsEM) IsResolved_DYCR_EM = true;

          }

          for(unsigned int i=0; i<Loose_leps.size(); i++){
            if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SubLeadLep){
              //cout << "--> duplicate" << endl;
              continue;
            }
            NExtraLooseLepton++;
            if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraLooseElectron++;
            else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraLooseMuon++;
            else{
              cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in RESOLVED" << endl;
              exit(EXIT_FAILURE);
            }

          }
          for(unsigned int i=0; i<Tight_leps.size(); i++){
            if(Tight_leps.at(i)==LeadLep || Tight_leps.at(i)==SubLeadLep){
              //cout << "--> duplicate" << endl;
              continue;
            }
            NExtraTightLepton++;
            if(Tight_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraTightElectron++;
            else if(Tight_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraTightMuon++;
            else{
              cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra Tight lepton in RESOLVED" << endl;
              exit(EXIT_FAILURE);
            }

          }

/*
          //==== XXX debugging
          if(IsResolved_SR_MM){
            if(NExtraLooseLepton>0){
              cout << "==========================================" << endl;
              cout << "NExtraLooseElectron = " << NExtraLooseElectron << endl;
              cout << "NExtraLooseMuon = " << NExtraLooseMuon << endl;
              //genFinder->Debug=true;
              genFinder->Run(gens);
              genFinder->Print();
              genFinder->PrintGen(gens);
              cout << "#### RECO ####" << endl;
              for(unsigned int i=0; i<Loose_leps.size(); i++){
                if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SubLeadLep){
                  cout << i << " (Duplicated) ";Loose_leps.at(i)->Print();
                  //cout << "--> duplicate" << endl;
                  continue;
                }
                if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON){
                  cout << i << " (Electron) ";Loose_leps.at(i)->Print();
                }
                else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON){
                  cout << i << " (Muon) ";Loose_leps.at(i)->Print();
                }
                else{
                  cout << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in RESOLVED" << endl;
                  exit(EXIT_FAILURE);
                }

              }

            }

          } // END Debugging
*/

        } // END if dR(l,l)>0.4 && dR(j,j)>0.4

      } // END if # of jets >= 2

    } // END Trigger fired

  } // END if # of tight lepton == 2

  bool IsBoostedEvent(false);
  bool IsBoosted_SR_EE(false), IsBoosted_SR_MM(false);
  bool IsBoosted_LowWRCR_EE(false), IsBoosted_LowWRCR_MM(false);
  bool IsBoosted_DYCR_EE(false), IsBoosted_DYCR_MM(false);
  bool IsBoosted_CR_EMJet(false), IsBoosted_CR_MEJet(false);
  bool IsBoosted_LowWRCR_EMJet(false), IsBoosted_LowWRCR_MEJet(false);
  if(!IsResolvedEvent){

    FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+param.Name, weight);

    Lepton *LeadLep = Tight_leps.at(0);
    bool tmp_IsLeadE(false), tmp_IsLeadM(false);
    TString Suffix = "";
    bool this_triggerpass(false);
    std::vector<Electron *> ForSF_electrons;
    std::vector<Muon *> ForSF_muons;
    if(LeadLep->LeptonFlavour()==Lepton::ELECTRON){
      tmp_IsLeadE = true;
      Suffix = "SingleElectron";
      this_triggerpass = PassSingleElectron;

      ForSF_electrons.push_back( Tight_electrons.at(0) );

      if(Signal){
        if(SignalLeptonChannel!=0) return;
      }

    }
    else if(LeadLep->LeptonFlavour()==Lepton::MUON){
      tmp_IsLeadM = true;
      Suffix = "SingleMuon";
      this_triggerpass = PassMu50;

      ForSF_muons.push_back( Tight_muons.at(0) );

      if(Signal){
        if(SignalLeptonChannel!=1) return;
      }

    }
    else{
      cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong flavour : " << LeadLep->LeptonFlavour() << endl;
      exit(EXIT_FAILURE);
    }

    FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_"+param.Name, weight);

    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //@@@@ Apply scale factors

    double trigger_sf_SingleElectron = 1.;
    double trigger_sf_SingleMuon = 1.;

    if(!IsDATA){

      mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

      for(unsigned int i=0; i<ForSF_electrons.size(); i++){
        double this_recosf = mcCorr->ElectronReco_SF(ForSF_electrons.at(i)->scEta(),ForSF_electrons.at(i)->Pt(), SystDir_ElectronIDSF);
        double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, ForSF_electrons.at(i)->scEta(), ForSF_electrons.at(i)->Pt(), SystDir_ElectronIDSF);
        weight *= this_recosf*this_idsf;
      }
      for(unsigned int i=0; i<ForSF_muons.size(); i++){

        double MiniAODP = sqrt( ForSF_muons.at(i)->MiniAODPt() * ForSF_muons.at(i)->MiniAODPt() + ForSF_muons.at(i)->Pz() * ForSF_muons.at(i)->Pz() );

        double this_recosf = mcCorr->MuonReco_SF(param.Muon_RECO_SF_Key, ForSF_muons.at(i)->Eta(), MiniAODP, SystDir_MuonIDSF);
        double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  ForSF_muons.at(i)->Eta(), ForSF_muons.at(i)->MiniAODPt(), SystDir_MuonIDSF);
        double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, ForSF_muons.at(i)->Eta(), ForSF_muons.at(i)->MiniAODPt(), SystDir_MuonIDSF);

        weight *= this_recosf*this_idsf*this_isosf;
      }

      //==== Trigger SF

      trigger_sf_SingleElectron = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, Tight_electrons, SystDir_ElectronTriggerSF);
      trigger_sf_SingleMuon = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, Tight_muons, SystDir_MuonTriggerSF);

    }
    if(Suffix=="SingleElectron"){
      trigger_sf_SingleMuon = trigger_sf_SingleElectron;
      weight *= trigger_sf_SingleElectron;
    }
    else{
      trigger_sf_SingleElectron = trigger_sf_SingleMuon;
      weight *= trigger_sf_SingleMuon;
    }
    //@@@@
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    if(this_triggerpass){

      FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_PassTrigger_"+param.Name, weight);

      leps_for_plot.push_back( Tight_leps.at(0) );

      std::vector<Lepton *> Loose_SF_leps = tmp_IsLeadE ? Loose_leps_el : Loose_leps_mu;
      std::vector<Lepton *> Loose_OF_leps = tmp_IsLeadE ? Loose_leps_mu : Loose_leps_el;;

      //==== Check for DYCR very first time

      bool HasLowMll = false;
      Lepton *LowMllLooseLepton;
      for(unsigned int i=0; i<Loose_SF_leps.size(); i++){
        if(Loose_SF_leps.at(i)==LeadLep){
          //cout << "--> duplicate" << endl;
          continue;
        }
        double dilep_mass  = (*(LeadLep)+*(Loose_SF_leps.at(i))).M();
        if( (dilep_mass > 60.) && (dilep_mass < 150.) ){
          HasLowMll = true;
          LowMllLooseLepton = Loose_SF_leps.at(i);
          leps_for_plot.push_back( Loose_SF_leps.at(i) );
          break;
        }
      }

      if(HasLowMll){

        for(unsigned int i=0; i<fatjets.size(); i++){
          FatJet this_fatjet = fatjets.at(i);
          if( fabs( LeadLep->DeltaPhi(this_fatjet) ) > 2.0 ){

            //==== Now this is the DY sideband we want

            HNFatJet = this_fatjet;
            if( this_fatjet.DeltaR( *LowMllLooseLepton ) < 0.8 ){
              NCand = HNFatJet;
            }
            else{
              NCand = HNFatJet + *(LowMllLooseLepton);
            }
            WRCand = *LeadLep+NCand;

            //==== Region Dictionary
            //==== - HNWR_SingleElectron_Boosted_DYCR : ee Boosted DYCR [IsBoosted_DYCR_EE]
            //==== - HNWR_SingleMuon_Boosted_DYCR : mm Boosted DYCR [IsBoosted_DYCR_MM]

            map_bool_To_Region[Suffix+"_Boosted_DYCR"] = true;
            if(tmp_IsLeadE) IsBoosted_DYCR_EE = true;
            else if(tmp_IsLeadM) IsBoosted_DYCR_MM = true;

            break;
          }
        }
      }      

      //==== If not, look for merged jet
      else{

        FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_NoLowmll_"+param.Name, weight);

        bool HasAwayMergedFatJet = false;
        for(unsigned int i=0; i<fatjets_LSF.size(); i++){
          FatJet this_fatjet = fatjets_LSF.at(i);
          if( fabs( LeadLep->DeltaPhi(this_fatjet) ) > 2.0 ){
            HasAwayMergedFatJet = true;
            HNFatJet = this_fatjet;
            NCand = HNFatJet;
            WRCand = *LeadLep+NCand;
            break;
          }
        }

        if(HasAwayMergedFatJet){

          if(!IsDATA){
            //==== LSF SF
            weight *= LSFSF(SystDir_LSFSF);
          }

          FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_HasMergedJet_"+param.Name, weight);

          bool HasSFLooseLepton = false;
          Lepton *SFLooseLepton;
          bool HasOFLooseLepton = false;
          Lepton *OFLooseLepton;

          for(unsigned int k=0; k<Loose_SF_leps.size(); k++){
            if( LeadLep->DeltaR( *(Loose_SF_leps.at(k)) ) < 0.01 ) continue;
            if( Loose_SF_leps.at(k)->Pt() <= 53. ) continue;
            if( HNFatJet.DeltaR( *(Loose_SF_leps.at(k)) ) < 0.8 ){
              HasSFLooseLepton = true;
              SFLooseLepton = Loose_SF_leps.at(k);
              break;
            }
          }

          for(unsigned int k=0; k<Loose_OF_leps.size(); k++){

            if( LeadLep->DeltaR( *(Loose_OF_leps.at(k)) ) < 0.01 ) continue;
            if( Loose_OF_leps.at(k)->Pt() <= 53. ) continue;
            if( HNFatJet.DeltaR( *(Loose_OF_leps.at(k)) ) < 0.8 ){
              HasOFLooseLepton = true;
              OFLooseLepton = Loose_OF_leps.at(k);
              break;
            }
          }

          //==== Now, veto with additoanl Tight lepton
          for(unsigned int i=0; i<Loose_leps.size(); i++){
            if(Loose_leps.at(i)==LeadLep || Loose_leps.at(i)==SFLooseLepton || Loose_leps.at(i)==OFLooseLepton ){
              //cout << "--> duplicate" << endl;
              continue;
            }
            NExtraLooseLepton++;
            if(Loose_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraLooseElectron++;
            else if(Loose_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraLooseMuon++;
            else{
              cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra loose lepton in BOOSTED" << endl;
              exit(EXIT_FAILURE);
            }

          }
          for(unsigned int i=0; i<Tight_leps.size(); i++){
            if(Tight_leps.at(i)==LeadLep || Tight_leps.at(i)==SFLooseLepton || Tight_leps.at(i)==OFLooseLepton ){
              //cout << "--> duplicate" << endl;
              continue;
            }
            NExtraTightLepton++;
            if(Tight_leps.at(i)->LeptonFlavour()==Lepton::ELECTRON) NExtraTightElectron++;
            else if(Tight_leps.at(i)->LeptonFlavour()==Lepton::MUON) NExtraTightMuon++;
            else{
              cerr << "[HNWRAnalyzer::executeEventFromParameter] wrong lepton flavour while counting extra Tight lepton in BOOSTED" << endl;
              exit(EXIT_FAILURE);
            }

          }

          bool NoExtraTightLepton = (NExtraTightLepton==0);
          bool WRMassGT800 = ( WRCand.M() > 800. );

          if(NoExtraTightLepton){

            FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_NoExtraTight_"+param.Name, weight);

            if(HasSFLooseLepton){

              FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_HasSFLooseLepton_"+param.Name, weight);

              if(!HasOFLooseLepton){

                FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_NoHasOFLooseLepton_"+param.Name, weight);

                leps_for_plot.push_back( SFLooseLepton );

                if( (*LeadLep+*SFLooseLepton).M() > 200 ){

                  FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_mllGT200_"+param.Name, weight);

                  if(WRMassGT800){

                    FillCutFlow(IsCentral, "CutFlow", "NotResolved_"+Suffix+"_mWRGT800_"+param.Name, weight);

                    //==== Region Dictionary
                    //==== - HNWR_SingleElectron_Boosted_SR : ee Boosted SR [IsBoosted_SR_EE]
                    //==== - HNWR_SingleMuon_Boosted_SR : mm Boosted SR [IsBoosted_SR_MM]

                    map_bool_To_Region[Suffix+"_Boosted_SR"] = true;
                    if(tmp_IsLeadE) IsBoosted_SR_EE = true;
                    else if(tmp_IsLeadM) IsBoosted_SR_MM = true;

                  }
                  else{

                    //==== Region Dictionary
                    //==== - HNWR_SingleElectron_Boosted_LowWRCR : ee Boosted, but low m(WR) [IsBoosted_LowWRCR_EE]
                    //==== - HNWR_SingleMuon_Boosted_LowWRCR : mm Boosted, but low m(WR) [IsBoosted_LowWRCR_MM]

                    map_bool_To_Region[Suffix+"_Boosted_LowWRCR"] = true;
                    if(tmp_IsLeadE) IsBoosted_LowWRCR_EE = true;
                    else if(tmp_IsLeadM) IsBoosted_LowWRCR_MM = true;

                  }

                }

              }

            }

            if(!HasSFLooseLepton){

              FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_NoHasSFLooseLepton_"+param.Name, weight);

              if(HasOFLooseLepton){

                FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_HasOFLooseLepton_"+param.Name, weight);

                leps_for_plot.push_back( OFLooseLepton );

                if( (*LeadLep+*OFLooseLepton).M() > 200 ){

                  FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_mllGT200_"+param.Name, weight);

                  if(WRMassGT800){

                    FillCutFlow(IsCentral, "CutFlow", "FSB_"+Suffix+"_mWRGT800_"+param.Name, weight);

                    //==== Region Dictionary
                    //==== - HNWR_SingleElectron_EMu_Boosted_CR : isolated e + mu-AK8jet (ttbar dominant) [IsBoosted_CR_EMJet]
                    //==== - HNWR_SingleMuon_EMu_Boosted_CR : isolated m + e-AK8jet (ttbar dominant) [IsBoosted_CR_MEJet]

                    map_bool_To_Region[Suffix+"_EMu_Boosted_CR"] = true;
                    if(tmp_IsLeadE) IsBoosted_CR_EMJet = true;
                    else if(tmp_IsLeadM) IsBoosted_CR_MEJet = true;

                  }
                  else{

                    //==== Region Dictionary
                    //==== - HNWR_SingleElectron_EMu_Boosted_LowWRCR : isolated e + mu-AK8jet (ttbar dominant), but low m(WR) [IsBoosted_LowWRCR_EMJet]
                    //==== - HNWR_SingleMuon_EMu_Boosted_LowWRCR : isolated m + e-AK8jet (ttbar dominant), but low m(WR) [IsBoosted_LowWRCR_MEJet]

                    map_bool_To_Region[Suffix+"_EMu_Boosted_LowWRCR"] = true;
                    if(tmp_IsLeadE) IsBoosted_LowWRCR_EMJet = true;
                    else if(tmp_IsLeadM) IsBoosted_LowWRCR_MEJet = true;

                  }

                }

              }

            }

          } // END No Extra Tight Lepton

        } // END If has merged jet

      } // END If no m(ll)<150 pair

    } // END If trigger fired

  } // END If not resolved

  if( map_bool_To_Region.size() == 0 ) return;

  //================================================
  //==== Double counting check
  //==== Tight electron and Tight muon near
  //==== HNFatJet
  //==== Tight_electrons, Tight_muons

  if(IsBoosted_SR_EE){
    for(unsigned int i=0; i<Tight_muons.size(); i++){
      if( fabs(Tight_muons.at(i)->DeltaPhi(HNFatJet)) > 2.0){
        JSFillHist((param.Name), "IsBoosted_SR_EE_But_HasTightMuon_"+(param.Name), 0., weight, 1, 0., 1.);
        break;
      }
    }
  }
  if(IsBoosted_SR_MM){
    for(unsigned int i=0; i<Tight_electrons.size(); i++){
      if( fabs(Tight_electrons.at(i)->DeltaPhi(HNFatJet)) > 2.0){
        JSFillHist((param.Name), "IsBoosted_SR_MM_But_HasTightElectron_"+(param.Name), 0., weight, 1, 0., 1.);
        break;

      }
    }
  }
  //================================================

  for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

    TString this_region = it_map->first;
    this_region = (param.Name)+"_"+this_region;

    if(it_map->second){

      if(RunXsecSyst && param.syst_ == AnalyzerParameter::Central){

        double normweight = 1./sumW/PDFWeights_Scale->at(0);
        for(unsigned int i=0; i<PDFWeights_Scale->size(); i++){
          //JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);

          JSFillHist("XsecSyst_Num_"+this_region, "PDFWeights_Scale_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+this_region, WRCand.M(), PDFWeights_Scale->at(i)*ev.MCweight()*normweight, 800, 0., 8000.);
        }
        for(unsigned int i=0; i<PDFWeights_Error->size(); i++){
          //JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);

          JSFillHist("XsecSyst_Num_"+this_region, "PDFWeights_Error_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+this_region, WRCand.M(), PDFWeights_Error->at(i)*ev.MCweight()*normweight, 800, 0., 8000.);
        }
        for(unsigned int i=0; i<PDFWeights_AlphaS->size(); i++){
          //JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);

          JSFillHist("XsecSyst_Num_"+this_region, "PDFWeights_AlphaS_"+TString::Itoa(i,10)+"_XsecSyst_Num_"+this_region, WRCand.M(), PDFWeights_AlphaS->at(i)*ev.MCweight()*normweight, 800, 0., 8000.);
        }

      }


      JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);
      JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 1000., 0., 1000.);

      JSFillHist(this_region, "nPileUp_"+this_region, nPileUp, weight, 200., 0., 200.);
      JSFillHist(this_region, "nPV_"+this_region, nPV, weight, 200., 0., 200.);
      JSFillHist(this_region, "N_VTX_"+this_region, N_VTX, weight, 200., 0., 200.);

      JSFillHist(this_region, "Lepton_Size_"+this_region, leps_for_plot.size(), weight, 10, 0., 10.);

      JSFillHist(this_region, "NExtraLooseElectron_"+this_region, NExtraLooseElectron, weight, 10, 0., 10.);
      JSFillHist(this_region, "NExtraLooseMuon_"+this_region, NExtraLooseMuon, weight, 10, 0., 10.);
      JSFillHist(this_region, "NExtraLooseLepton_"+this_region, NExtraLooseLepton, weight, 10, 0., 10.);

      JSFillHist(this_region, "NExtraTightElectron_"+this_region, NExtraTightElectron, weight, 10, 0., 10.);
      JSFillHist(this_region, "NExtraTightMuon_"+this_region, NExtraTightMuon, weight, 10, 0., 10.);
      JSFillHist(this_region, "NExtraTightLepton_"+this_region, NExtraTightLepton, weight, 10, 0., 10.);

      JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "LSFFatJet_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "FatJet_LSF_Size_"+this_region, fatjets_LSF.size(), weight, 10, 0., 10.);
      JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

      JSFillHist(this_region, "NBJets_"+this_region, NBJets, weight, 10, 0., 10.);
      JSFillHist(this_region, "HT_"+this_region, HT, weight, 4000, 0., 4000.);

      if(this_region.Contains("Boosted")){
        JSFillHist(this_region, "dPhi_lJ_"+this_region, fabs( leps_for_plot.at(0)->DeltaPhi(HNFatJet) ), weight, 40, 0., 4.);

        JSFillHist(this_region, "HNFatJet_Pt_"+this_region, HNFatJet.Pt(), weight, 1000, 0., 1000.);
        JSFillHist(this_region, "HNFatJet_Eta_"+this_region, HNFatJet.Eta(), weight, 60, -3., 3.);
        JSFillHist(this_region, "HNFatJet_Mass_"+this_region, HNFatJet.M(), weight, 3000, 0., 3000.);
        JSFillHist(this_region, "HNFatJet_SDMass_"+this_region, HNFatJet.SDMass(), weight, 3000, 0., 3000.);
        JSFillHist(this_region, "HNFatJet_PuppiTau21_"+this_region, HNFatJet.PuppiTau2()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
        JSFillHist(this_region, "HNFatJet_PuppiTau31_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau1(), weight, 100, 0., 1.);
        JSFillHist(this_region, "HNFatJet_PuppiTau32_"+this_region, HNFatJet.PuppiTau3()/HNFatJet.PuppiTau2(), weight, 100, 0., 1.);
        JSFillHist(this_region, "HNFatJet_LSF_"+this_region, HNFatJet.LSF(), weight, 100, 0., 1.);

      }

      if( leps_for_plot.size()>=2 ){
        JSFillHist(this_region, "ZCand_Mass_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).M(), weight, 2000, 0., 2000.);
        JSFillHist(this_region, "ZCand_Pt_"+this_region, ((*leps_for_plot.at(0))+(*leps_for_plot.at(1))).Pt(), weight, 2000, 0., 2000.);
        JSFillHist(this_region, "dPhi_ll_"+this_region, fabs((*leps_for_plot.at(0)).DeltaPhi(*leps_for_plot.at(1))), weight, 40, 0., 4.);
      }

      JSFillHist(this_region, "WRCand_Mass_"+this_region, WRCand.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "WRCand_Pt_"+this_region, WRCand.Pt(), weight, 300, 0., 3000.);

      JSFillHist(this_region, "NCand_Mass_"+this_region, NCand.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "NCand_Pt_"+this_region, NCand.Pt(), weight, 300, 0., 3000.);
      JSFillHist(this_region, "NCand_1_Mass_"+this_region, NCand_1.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "NCand_1_Pt_"+this_region, NCand_1.Pt(), weight, 300, 0., 3000.);
      JSFillHist(this_region, "NCand_2_Mass_"+this_region, NCand_2.M(), weight, 800, 0., 8000.);
      JSFillHist(this_region, "NCand_2_Pt_"+this_region, NCand_2.Pt(), weight, 300, 0., 3000.);

      if(Signal){
        JSFillHist(this_region, "TriggerEff_"+this_region, 0., this_trigger_sf, 1, 0., 1.);
      }

      FillLeptonPlots(leps_for_plot, this_region, weight);
      FillJetPlots(jets, fatjets_LSF, this_region, weight);

      if(jets.size()>=2){
        JSFillHist(this_region, "dRj1j2_"+this_region, jets.at(0).DeltaR( jets.at(1) ), weight, 60., 0., 6.);
      }

    } // END if(pass Region)

  } // END Loop mapRegionBool

}

HNWRAnalyzer::HNWRAnalyzer(){

  RunFake = false;

}

HNWRAnalyzer::~HNWRAnalyzer(){

}

std::vector<Jet> HNWRAnalyzer::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

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

bool HNWRAnalyzer::LeptonPassID(Lepton &lepton, TString ID){

  if(lepton.LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)&lepton;
    return el->PassID(ID);
  }
  else if(lepton.LeptonFlavour()==Lepton::MUON){
    Muon *mu = (Muon *)&lepton;
    return mu->PassID(ID);
  }
  else{
    cerr << "[HNWRAnalyzer::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}

void HNWRAnalyzer::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    JSFillHist(suffix, histname, 0., weight, 1, 0., 1.);

  }

}

double HNWRAnalyzer::LSFSF(int dir){

  if(dir==0) return 0.87;
  else if(dir>0) return 0.87+0.08;
  else return 0.87-0.07;

}




