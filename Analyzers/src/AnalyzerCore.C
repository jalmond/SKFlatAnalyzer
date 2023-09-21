#include "AnalyzerCore.h"

AnalyzerCore::AnalyzerCore(){

  outfile = NULL;

  mcCorr = new MCCorrection();
  puppiCorr = new PuppiSoftdropMassCorr();
  fakeEst = new FakeBackgroundEstimator();
  cfEst = new CFBackgroundEstimator();
  pdfReweight = new PDFReweight();
  muonGE = new GeneralizedEndpoint();
  muonGEScaleSyst = new GEScaleSyst();

  JECSources = {"AbsoluteStat","AbsoluteScale","AbsoluteFlavMap","AbsoluteMPFBias","Fragmentation","SinglePionECAL","SinglePionHCAL","FlavorQCD","TimePtEta","RelativeJEREC1","RelativeJEREC2","RelativeJERHF","RelativePtBB","RelativePtEC1","RelativePtEC2","RelativePtHF","RelativeBal","RelativeSample","RelativeFSR","RelativeStatFSR","RelativeStatEC","RelativeStatHF","PileUpDataMC","PileUpPtRef","PileUpPtBB","PileUpPtEC1","PileUpPtEC2","PileUpPtHF","FlavorZJet","FlavorPhotonJet","FlavorPureGluon","FlavorPureQuark","FlavorPureCharm","FlavorPureBottom","Total"};

  iSetupLeptonBDTv5=false;
  iSetupLeptonBDTv4=false;
  
  TimeTagMatcher.clear();
  TimerMap.clear();
  TimerMap["LATEST"] = std::clock();
  TimingMap.clear();
  TimingMap["start"] = std::clock();

  //  double time_elapsed_ms = 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC;
  //std::cout << "CPU time used: " << time_elapsed_ms / 1000.0 << " s\n";



  /// TESTBDT couts comparison of branches and Function 
  TESTBDT=false;
    
}

AnalyzerCore::~AnalyzerCore(){

  //=== hist maps
  
  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    delete mapit->second;
  }
  maphist_TH1D.clear();

  for(std::map< TString, TH2D* >::iterator mapit = maphist_TH2D.begin(); mapit!=maphist_TH2D.end(); mapit++){

    //cout << "Deleting TH2D " << mapit->first << endl;
    delete mapit->second;
  }
  maphist_TH2D.clear();

  for(std::map< TString, TH3D* >::iterator mapit = maphist_TH3D.begin(); mapit!=maphist_TH3D.end(); mapit++){
    delete mapit->second;
  }
  maphist_TH3D.clear();
  
  //==== output rootfile

  if(outfile){
    outfile->Close();
    delete outfile;
  }

  //==== Tools

  if(mcCorr) delete mcCorr;
  if(puppiCorr) delete puppiCorr;
  if(fakeEst) delete fakeEst;
  if(cfEst) delete cfEst;
  if(pdfReweight) delete pdfReweight;
  if(muonGE) delete muonGE;
  if(muonGEScaleSyst) delete muonGEScaleSyst;

  AK4CHSJECUncMap.clear();
  AK4PUPPIJECUncMap.clear();
  AK8CHSJECUncMap.clear();
  AK8PUPPIJECUncMap.clear();

  
  if(iSetupLeptonBDTv5){

    ////// NEW MVA 
    delete ElectronIDv5_MVAReader;
    delete ElectronIDv5_CFMVAReader;
    delete ElectronIDv5_FakeMVAReader;
    delete MuonIDv5_FakeMVAReader;

  }
  if(iSetupLeptonBDTv4){
    delete ElectronIDv4_FakeMVAReader;
    delete ElectronIDv4_CFMVAReader;
    delete ElectronIDv4_ConvMVAReader;
    delete MuonIDv4_FakeMVAReader;
  }

}


void  AnalyzerCore::FillTimer(TString inittag){
  
  TString tag = "";
  if (TimeTagMatcher.find(inittag) == TimeTagMatcher.end()) {
    tag = TString(std::to_string(TimeTagMatcher.size())) +"_"+inittag;
    TimeTagMatcher[inittag] = tag;
  }
  else{
    auto itr = TimeTagMatcher.find(inittag);
    tag = itr->second;
  }

  if (TimerMap.find(tag) == TimerMap.end()) {
    auto itr = TimerMap.find("LATEST");
    double last_time = itr->second;
    TimerMap[tag] = (std::clock() - last_time)/ CLOCKS_PER_SEC;
  }
  else{
    auto itr = TimerMap.find("LATEST");
    auto itr2= TimerMap.find(tag);
    TimerMap[tag]= itr2->second + ( (std::clock() - itr->second)/ CLOCKS_PER_SEC);
  }
  TimerMap["LATEST"] =std::clock() ;
  
  if(_jentry==0) return;

  if(_jentry%10000==0){
    vector<TString> TimerLabels;
    for(auto i: TimeTagMatcher) TimerLabels.push_back(i.first);
    for(auto i : TimerMap) {
      if(i.first != "LATEST")    cout << i.first << " processing time = " << i.second << endl;
    }
  }

  return;
}

void  AnalyzerCore::AddTimerStamp(TString tag){
  
  if (TimingMap.find(tag) == TimingMap.end()) TimingMap[tag] = std::clock();
  
  return;

}



bool AnalyzerCore::AnalyserRunsFullBkg(){

  // Flag to setup fakeEst/cfEst/pdfReweight
  vector<TString> AnalyserList = {"HNL_SignalRegionPlotter","HNL_ControlRegionPlotter","HNL_SignalRegionPlotter17028","HNL_SignalRegionPlotter21003","HNL_SignalLeptonOpt", "HNL_SignalRegionOpt", "HNL_SignalStudies","HNL_ObjectStudies", "HNL_LeptonIDSF"};

  if(std::find(AnalyserList.begin(), AnalyserList.end(), Analyzer) != AnalyserList.end()) {
    return true;
  }
  return false;
}


void AnalyzerCore::FillEventComparisonFile(AnalyzerParameter param,TString label,string date, double w){

  //// Make TEX file                                                                                                                             

  ofstream ofile_tex;
  string lqdir = getenv("SKFlat_WD");

  if(!IsData)label = label  +"_" +MCSample;
  else label = label  +"_" +GetEra();

  //label = label + k_sample_name;                                                                                                               
  string compfile = string(getenv("SKFlatEvCompDir")) +  "/"+date + "/"+string(param.Name)+string(label)+".txt";

  cout << "FillEventComparisonFile  " << label << " " << compfile << endl;
  ofile_tex.open(compfile.c_str(),ios::out | ios::app);

  ofile_tex.setf(ios::fixed,ios::floatfield);
  ofile_tex << "[ "<<getenv("USER") << " : " << run << " : " << event << " : " << param.Muon_Tight_ID << " : " << param.Electron_Tight_ID << " :\
 " << w <<   "]" << endl;

  ofile_tex.close();

  return;
}

//==== Attach the historams to ai different direcotry, not outfile
//==== We will write these histograms in WriteHist() to outfile
void AnalyzerCore::SwitchToTempDir(){

  gROOT->cd();
  TDirectory *tempDir = NULL;
  int counter = 0;
  while (!tempDir) {
    //==== First, let's find a directory name that doesn't exist yet
    std::stringstream dirname;
    dirname << "AnalyzerCore" << counter;
    if (gROOT->GetDirectory((dirname.str()).c_str())) {
      ++counter;
      continue;
    }
    //==== Let's try to make this directory
    tempDir = gROOT->mkdir((dirname.str()).c_str());
  }
  tempDir->cd();

}

void AnalyzerCore::SetOutfilePath(TString outname){
  outfile = new TFile(outname,"RECREATE");
};

Event AnalyzerCore::GetEvent(){

  Event ev;
  ev.SetTrigger(*HLT_TriggerName);
  ev.SetMET(pfMET_Type1_pt,pfMET_Type1_phi);
  ev.SetnPV(nPV);
  ev.SetEra(GetEra());

  return ev;

}

double AnalyzerCore::GetJECUncertainty(TString source, TString JetType, double eta, double pt, int sys){

  std::map<TString, std::vector<std::map<double, std::vector<double> > > >::iterator mapit;
  bool NotFound=false;
  if(JetType=="AK4PFchs") {
    mapit = AK4CHSJECUncMap.find(source); 
    if(mapit == AK4CHSJECUncMap.end()) NotFound=true;
  }
  if(JetType=="AK4PFPuppi") {
    mapit = AK4PUPPIJECUncMap.find(source);
    if(mapit == AK4PUPPIJECUncMap.end()) NotFound=true;
  }
  if(JetType=="AK8PFchs"){
    mapit = AK8CHSJECUncMap.find(source);
    if(mapit == AK8CHSJECUncMap.end()) NotFound=true;
  }
  if(JetType=="AK8PFPuppi") {
    mapit = AK8PUPPIJECUncMap.find(source);
    if(mapit == AK8PUPPIJECUncMap.end()) NotFound=true;
  }
  if(NotFound) {cout<< "ERROR, " << source  << " not found in JEC Uncertainty MAP for " << JetType << endl; return -999.;}

  
  double bin_boundary(-999.);

  std::map<double, std::vector<double> > ptmap = mapit->second.at(0);

  std::vector<double> etabins;  
  for(std::map<double, std::vector<double> >::iterator it = ptmap.begin(); it!= ptmap.end(); it++){
    etabins.push_back(it->first);
  }

  
  for(unsigned int i=0; i < etabins.size()-1 ; i++){
    if(eta >= etabins.at(i) && eta < etabins.at(i+1)){  bin_boundary = double(etabins.at(i)) ; break;}
  }

  if(bin_boundary == -999) return 1.;
  
  std::vector<double> ptbins;
  

  for(std::map<double, std::vector<double> >::iterator pit = ptmap.begin();  pit != ptmap.end(); pit++){
    if(double(pit->first) == double(bin_boundary)) {ptbins = pit->second; }
  }
  
  int ptbin(-999);
  if(pt >= ptbins.at(ptbins.size() - 1)) ptbin = ptbins.size() - 1;
  for(unsigned int j = 0 ; j < ptbins.size()-1; j++){
    if( pt >= ptbins.at(j)  && pt < ptbins.at(j+1)) {ptbin=j; break;}
  }
  
  if(ptbin == -999) return 1.;


  std::map<double, std::vector<double> > upmap = mapit->second.at(1); 
  std::map<double, std::vector<double> > downmap = mapit->second.at(2); 
  
  std::map<double, std::vector<double> >::iterator mapit_unc;
  if(sys> 0) mapit_unc =  mapit->second.at(1).find(bin_boundary);
  else mapit_unc =  mapit->second.at(2).find(bin_boundary);
  
  double unc = (sys> 0) ?   1+ mapit_unc->second.at(ptbin) : 1 - mapit_unc->second.at(ptbin);

  return unc;
}

void AnalyzerCore::SetupJECUncertainty(TString source , TString JetType){

 
  string analysisdir = getenv("DATA_DIR");
  
  string file = analysisdir + "/"+string(GetEra()) + "/JEC/Summer19UL16APV_V7_MC_UncertaintySources_AK4PFchs.txt"; 
  if(GetEra() == "2016postVFP") file = analysisdir + "/"+ string(GetEra())+ "/JEC/Summer19UL16_V7_MC_UncertaintySources_AK4PFchs.txt";
  if(GetEra() == "2017") file = analysisdir + "/"+ string(GetEra())+ "/JEC/Summer19UL17_V5_MC_UncertaintySources_AK4PFchs.txt";
  if(GetEra() == "2018") file = analysisdir + "/"+ string(GetEra())+ "/JEC/Summer19UL18_V5_MC_UncertaintySources_AK4PFchs.txt";


  string sline;
  vector<string> SourceLines;
  bool ExtractLine=false;
  int nline(0);
  ifstream jec_file(file.c_str());  
  while(getline(jec_file,sline) ){
    if(ExtractLine){
      if(nline==0){nline++; continue;}
      if(sline.find("[")!=string::npos) break;
      if(nline==1)SourceLines.push_back(sline);
    }
    if(sline.find(source)!=string::npos) { ExtractLine=true;}
  }

  jec_file.close();

  cout << "Setting up JEC uncertainty vector for source ["<<source<< "]." << file << endl;

  std::map<double, std::vector<double> > etaptmap, eta_uncupmap, eta_uncdownmap;
  for(unsigned int ilines =0; ilines <  SourceLines.size(); ilines++){

    string line = SourceLines[ilines];
    std::istringstream is( line );

    std::string s_eta_min, s_eta_max, s_nBins;
    is >> s_eta_min; 
    is >> s_eta_max;  
    is >> s_nBins;

    double  eta_min = stod(s_eta_min);
    double  eta_max = stod(s_eta_max);
    double  nBins = stod(s_nBins);

    bool EtaBinCheck=true;
    if((ilines == SourceLines.size() -1)){
      if( eta_min != 5.0) EtaBinCheck=false;
      if( eta_max != 5.4) EtaBinCheck=false;
    }
    if(!EtaBinCheck){
      cout << "[AnalyzerCore::SetupJECUncertainty] Eta bin set incorrectly ..." << endl;
      exit(EXIT_FAILURE);      
    }
    std::vector<double> ptbin, unc_up, unc_down;
    bool finalbin(false);

    for(int i=0; i < nBins; i++){
      std::string  subString;
      is >> subString;
      double value_from_string =  stod(subString);
      if((i %3) == 0) {ptbin.push_back(value_from_string); if( i == nBins-3) finalbin=true; }
      if((i %3) == 1) {unc_up.push_back(value_from_string);}
      if((i %3) == 2) {unc_down.push_back(value_from_string);}

      // Check Final bin is last bin in txt file for this eta bin
      if((i %3) == 2 && finalbin) {
  string CheckFinalPt; 
  is >> CheckFinalPt;
  if(!CheckFinalPt.empty()) {
    cout << "[AnalyzerCore::SetupJECUncertainty] Last pt bin of  set incorrectly ..." << endl;
    exit(EXIT_FAILURE);
  }
      }
    }
    etaptmap[eta_min] = ptbin;
    eta_uncupmap[eta_min] =  unc_up;
    eta_uncdownmap[eta_min] = unc_down;

    std::vector<double> NULLBin;
    if(ilines ==  SourceLines.size() -1) etaptmap[eta_max] = NULLBin;
        
  }
  
  jec_file.close();

  std::vector<std::map<double, std::vector<double> > > vec_unc;
  vec_unc.push_back(etaptmap);
  vec_unc.push_back(eta_uncupmap);
  vec_unc.push_back(eta_uncdownmap);

  if(JetType=="AK4PFchs") AK4CHSJECUncMap[source] = vec_unc;
  if(JetType=="AK4PFPuppi") AK4PUPPIJECUncMap[source] = vec_unc;
  if(JetType=="AK8PFchs") AK8CHSJECUncMap[source] = vec_unc;
  if(JetType=="AK8PFPuppi") AK8PUPPIJECUncMap[source] = vec_unc;

  return;
  
}



std::vector<Muon> AnalyzerCore::GetAllMuons(){

  std::vector<Muon> out;
  if(!muon_pt) return out;
  for(unsigned int i=0; i<muon_pt->size(); i++){

    Muon mu;

    mu.SetCharge(muon_charge->at(i));
    mu.SetUncorrectedPt(muon_pt->at(i));
    Muon mu_tmp;
    mu_tmp.SetPtEtaPhiM(muon_pt->at(i), muon_eta->at(i), muon_phi->at(i), muon_mass->at(i));
    mu.SetMiniAODPt(muon_pt->at(i));
    mu.SetMiniAODTunePPt(muon_TuneP_pt->at(i));

    double rc = muon_roch_sf->at(i);
    double rc_err = muon_roch_sf_up->at(i)-rc;
    //==== For the Rochester corection, up and down err are the same
    mu.SetMomentumScaleUpDown( muon_pt->at(i) * (rc+rc_err), muon_pt->at(i) * (rc-rc_err)  );
    mu.SetPtEtaPhiM(muon_pt->at(i)*rc, muon_eta->at(i), muon_phi->at(i), muon_mass->at(i));

    //==== TuneP
    //==== Apply scailing later with AnalyzerCore::UseTunePMuon()
    mu.SetTuneP4(muon_TuneP_pt->at(i), muon_TuneP_ptError->at(i), muon_TuneP_eta->at(i), muon_TuneP_phi->at(i), muon_TuneP_charge->at(i));

    mu.SetSoftMVA(muon_softMVA->at(i));
    mu.SetMVA(muon_MVA->at(i));
    mu.SetLepMVA( muon_MVA->at(i));
    
    mu.SetdXY(muon_dxyVTX->at(i), muon_dxyerrVTX->at(i));
    mu.SetdZ(muon_dzVTX->at(i), muon_dzerrVTX->at(i));
    mu.SetIP3D(muon_3DIPVTX->at(i), muon_3DIPerrVTX->at(i));
    mu.SetTypeBit(muon_TypeBit->at(i));
    mu.SetIDBit(muon_IDBit->at(i));
    mu.SetisPOGHighPt(muon_ishighpt->at(i));
    mu.SetPOGMediumHIP(muon_ismedium_hip->at(i),muon_ismedium_nohip->at(i));
    mu.SetChi2(muon_normchi->at(i));
    mu.SetIso(muon_PfChargedHadronIsoR04->at(i),muon_PfNeutralHadronIsoR04->at(i),muon_PfGammaIsoR04->at(i),muon_PFSumPUIsoR04->at(i),muon_trkiso->at(i));
    mu.SetLepIso(muon_PfChargedHadronIsoR04->at(i),muon_PfNeutralHadronIsoR04->at(i),muon_PfGammaIsoR04->at(i));
   
    mu.SetTrackerLayers(muon_trackerLayers->at(i));

    mu.SetMatchedStations(muon_matchedstations->at(i));
    mu.SetPixelHits(muon_pixelHits->at(i));
    mu.SetValidMuonHits(muon_validmuonhits->at(i));

    //==== Should be set after Eta is set

    mu.SetMiniIso(
      muon_PfChargedHadronMiniIso->at(i), 
      muon_PfNeutralHadronMiniIso->at(i), 
      muon_PfGammaMiniIso->at(i), 
      muon_PFSumPUMiniIso->at(i),
      Rho,
      mu.EA()
    );

    mu.SetFilterBits(muon_filterbits->at(i));
    mu.SetPathBits(muon_pathbits->at(i));
    
    
    ////// FILL MVA VARIABLES
    if(fChain->GetBranch("muon_mva_fake_v4"))     mu.SetHNL_FakeLepMVAMuonV4(muon_mva_fake_v4->at(i));
    if(fChain->GetBranch("muon_mva_fake_ed_v4"))  mu.SetHNL_FakeLepMVAMuon_EtaDependantV4(muon_mva_fake_ed_v4->at(i));
    if(fChain->GetBranch("muon_mva_fake_QCD_LFvsHF_v5")) {
      mu.SetHNL_FakeFlavourLepMVA_V5( muon_mva_fake_QCD_LFvsHF_v5->at(i),
              muon_mva_fake_QCD_HFBvsHFC_v5->at(i),
              muon_mva_fake_QCD_LF1_v5->at(i),
              muon_mva_fake_QCD_LF2_v5->at(i));
    }

    if(iSetupLeptonBDTv4) {
      if(!fChain->GetBranch("muon_mva_fake_v4")) mu.SetHNL_FakeLepMVAMuonV4( GetBDTScoreMuon(mu,AnalyzerCore::Fake,  "BDTGv4"));
      if(!fChain->GetBranch("muon_mva_fake_ed_v4")) mu.SetHNL_FakeLepMVAMuon_EtaDependantV4( GetBDTScoreMuon_EtaDependant(mu,AnalyzerCore::Fake,  "BDTGv4"));
    }

    if(iSetupLeptonBDTv5) {
      
      if(!fChain->GetBranch("muon_mva_fake_QCD_LFvsHF_v5")){
  
  mu.SetHNL_FakeFlavourLepMVA_V5( GetBDTScoreMuon(mu,AnalyzerCore::FakeRate,  "BDTGv5_QCD_LFvsHF"),
          GetBDTScoreMuon(mu,AnalyzerCore::FakeRate,  "BDTGv5_QCD_HFBvsHFC"),
          GetBDTScoreMuon(mu,AnalyzerCore::FakeRate,  "BDTGv5_LF1"),
          GetBDTScoreMuon(mu,AnalyzerCore::FakeRate,  "BDTGv5_LF2"));
  
      }
    }
    

    if(fChain->GetBranch("muon_ptrel"))            mu.SetJetPtRel(muon_ptrel->at(i));
    if(fChain->GetBranch("muon_ptratio"))          mu.SetJetPtRatio(muon_ptratio->at(i));
    if(fChain->GetBranch("muon_v2_cj_bjetdisc"))   mu.SetCloseJetBScore(muon_v2_cj_bjetdisc->at(i));
    if(fChain->GetBranch("muon_v2_cj_cvsbjetdisc"))mu.SetCloseJetCvsBScore(muon_v2_cj_cvsbjetdisc->at(i));
    if(fChain->GetBranch("muon_v2_cj_cvsljetdisc"))mu.SetCloseJetCvsLScore(muon_v2_cj_cvsljetdisc->at(i));
    if(fChain->GetBranch("muon_v2_cj_flavour"))    mu.SetCloseJetFlavour(muon_v2_cj_flavour->at(i));
    

    bool FillCloseJetVar=!fChain->GetBranch("muon_v2_cj_cvsljetdisc");
    if(Analyzer=="HNL_LeptonID_BDT_KinVar") FillCloseJetVar=false;

    if(FillCloseJetVar){
      
      std::vector<Jet>    AK4_JetAllColl = All_Jets;

      float  JetDiscCJ = -999;
      float  JetDiscCJ_CvsB = -999;
      float  JetDiscCJ_CvsL = -999;

      int JetHadFlavour = -999;
      int IdxMatchJet=-1;
      float mindR1=999.;

      for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
        float dR1=mu.DeltaR(AK4_JetAllColl.at(ij));
        if(dR1>0.4) continue;
        if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
      }
      if(IdxMatchJet!=-1)    {
  JetDiscCJ     = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
  JetDiscCJ_CvsB     = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsB);
  JetDiscCJ_CvsL     = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsL);

  JetHadFlavour = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();
      }
      else {
  JetDiscCJ=0.1; //// These are low pt ; value chosen to match peak in prompt leptons
  JetDiscCJ_CvsB=0.4;
  JetDiscCJ_CvsL=0.1;
  JetHadFlavour=0;
      }

      mu.SetJetPtRel(JetLeptonPtRelLepAware(mu));
      mu.SetJetPtRatio(JetLeptonPtRatioLepAware(mu));
      mu.SetCloseJetBScore(JetDiscCJ);
      mu.SetCloseJetCvsBScore(JetDiscCJ_CvsB);
      mu.SetCloseJetCvsLScore(JetDiscCJ_CvsL);
      mu.SetCloseJetFlavour(JetHadFlavour);
    }

    if(!fChain->GetBranch("muon_lepton_type") ||  !fChain->GetBranch("muon_is_cf"))  {
      int lep_type = GetLeptonType_JH(mu, All_Gens);
      mu.SetLeptonType(lep_type);
      mu.SetLeptonIsCF(IsCF(mu, All_Gens));

    }
    else {
      if(fChain->GetBranch("muon_lepton_type")) mu.SetLeptonType(muon_lepton_type->at(i));
      if(fChain->GetBranch("muon_is_cf")) mu.SetLeptonIsCF(muon_is_cf->at(i));
    }
    
    out.push_back(mu);

  }

  //if(PtOrderObj) std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


void AnalyzerCore::InitializeElectronIDTreeVars(){

  bdt_id_Pt=-1;
  bdt_id_Eta=-1;
  bdt_id_MiniIsoChHad=-1;
  bdt_id_MiniIsoNHad=-1;
  bdt_id_MiniIsoPhHad=-1;
  bdt_id_IsoChHad=-1;
  bdt_id_IsoNHad=-1 ;
  bdt_id_IsoPhHad=-1;
  bdt_id_RelMiniIsoCh=-1;
  bdt_id_RelMiniIsoN=-1;
  bdt_id_Dxy=-1;
  bdt_id_Dz=-1;
  bdt_id_DxySig=-1;
  bdt_id_DzSig=-1;
  bdt_id_RelIso=-1;
  bdt_id_IP3D=-1;
  bdt_id_PtRatio=-1;
  bdt_id_PtRatioV2=-1;
  bdt_id_PtRatioV3=-1;
  bdt_id_PtRel=-1;
  bdt_id_PtRelV2=-1;
  bdt_id_PtRelV3=-1;
  bdt_id_CEMFracCJ=-1;
  bdt_id_NEMFracCJ=-1;
  bdt_id_CHFracCJ=-1;
  bdt_id_NHFracCJ=-1;
  bdt_id_MuFracCJ=-1;
  bdt_id_JetDiscCJ=-1;
  bdt_id_JetDiscCJCvsB=-1;
  bdt_id_JetDiscCJCvsL=-1;
  bdt_id_MVA=-1;
  bdt_id_MVAIso=-1;
  bdt_id_MVARaw=-1;
  bdt_id_MVAIsoRaw=-1;
  bdt_id_Full5x5_sigmaIetaIeta=-1;
  bdt_id_dEtaSeed=-1;
  bdt_id_dPhiIn=-1;
  bdt_id_HoverE=-1;
  bdt_id_EoverP=-1;
  bdt_id_FBrem=-1;
  bdt_id_Rho=-1;
  bdt_id_R9=-1;
  bdt_id_psEoRraw=-1;

  bdt_id_TrkIso=-1;   //InvEminusInvP=-1;                                                                    
  bdt_id_dr03TkSumPt=-1;
  bdt_id_dr03HcalTowerSumEt=-1;

  bdt_id_e55=-1;
  bdt_id_e2x5OverE5x5=-1;
  bdt_id_e1x5OverE5x5=-1;
  bdt_id_EtaWidth=-1;
  bdt_id_PhiWidth=-1;
  bdt_id_ecalPFClusterIso=-1;
  bdt_id_hcalPFClusterIso=-1;
  bdt_id_MissingHits=-1;
  bdt_id_isEcalDriven=-1;
  bdt_id_PassConversionVeto=-1;
  bdt_id_IsGsfCtfScPixChargeConsistent=-1;
  bdt_id_IsGsfScPixChargeConsistent=-1 ;
  bdt_id_IsGsfCtfChargeConsistent=-1;


  return;
}



void AnalyzerCore::InitializeIDTreeVars(){

  // Initialise all BDT var to default
  // Kinematic
  bdt_id_Fake_Type=-999,bdt_id_Fake_Flavour=-999;
  bdt_id_Pt=-999, bdt_id_Eta=-999, bdt_id_PtBinned=-999,bdt_id_PtBinned2=-999;
  bdt_id_PtRatio=-999,  bdt_id_PtRel=-999; bdt_id_MassDrop=-999;

  bdt_id_PtRatioV2=-999,  bdt_id_PtRelV2=-999;
  bdt_id_PtRatioV3=-999,  bdt_id_PtRelV3=-999;
  /// Pileup
  bdt_id_PileUp = -999; 
  
  // Iso
  bdt_id_MiniIsoChHad=-999;  bdt_id_MiniIsoNHad=-999;  bdt_id_MiniIsoPhHad=-999;  bdt_id_IsoChHad=-999;  bdt_id_IsoNHad=-999;  bdt_id_IsoPhHad=-999;
  bdt_id_Minireliso=-999.;  bdt_id_RelMiniIsoCh=-999.;  bdt_id_RelMiniIsoN=-999.; bdt_id_RelIso=-999.;

  
  // Closejet
  bdt_id_CEMFracCJ=-999, bdt_id_NEMFracCJ=-999, bdt_id_CHFracCJ=-999, bdt_id_NHFracCJ=-999,bdt_id_MuFracCJ=-999;
  
  bdt_id_JetDiscCJ=-999,  bdt_id_JetDiscCJCvsB=-999,  bdt_id_JetDiscCJCvsL=-999;

  /// IP
  bdt_id_Dxy=-999.;  bdt_id_DxySig=-999.;  bdt_id_Dz=-999.;  bdt_id_DzSig=-999., bdt_id_IP3D=-999.; 

  // MVA
  bdt_id_MVA=-999.;  bdt_id_MVAIso=-999.;  bdt_id_Chi2=-999.;
  bdt_id_MVARaw=-999;
  bdt_id_MVAIsoRaw=-999;
  
  // Hits
  bdt_id_Validhits=-999.;  bdt_id_Matched_stations=-999.; 
  bdt_id_Pixel_hits=-999.;  bdt_id_Tracker_layers=-999.;  bdt_id_MissingHits=-999.; 

  // El Shower/SCL
  bdt_id_Full5x5_sigmaIetaIeta=-999.; bdt_id_dEtaSeed=-999.;  bdt_id_dPhiIn=-999.;  bdt_id_dEtaIn=-999.;  bdt_id_EtaWidth=-999.;  bdt_id_PhiWidth=-999.;  bdt_id_HoverE=-999.;
  bdt_id_TrkIso=-999.;  bdt_id_e2x5OverE5x5=-999;  bdt_id_e1x5OverE5x5=-999;  
  bdt_id_e55=-999;  
  bdt_id_dr03HcalTowerSumEt=-999;  bdt_id_dr03TkSumPt=-999;
  bdt_id_InvEminusInvP=-999.;  bdt_id_ecalPFClusterIso=-999.;  bdt_id_hcalPFClusterIso=-999.;  
  
  // IDs
  bdt_id_POGTight=-999;  bdt_id_POGMedium = -999;  bdt_id_HNTightID = -999, bdt_id_POGMVA80ID=-999, bdt_id_POGMVA90ID=-999;  

  // COnv/CF
  bdt_id_isEcalDriven= -999;  bdt_id_EoverP=-999;  bdt_id_FBrem=-999;  bdt_id_PassConversionVeto=-999 ,bdt_id_Rho=-999;
  bdt_id_IsGsfCtfScPixChargeConsistent =-999;  bdt_id_IsGsfScPixChargeConsistent =-999;  bdt_id_IsGsfCtfChargeConsistent =-999;  bdt_id_R9=-999;

  bdt_id_psEoRraw= -999;

  bdt_id_w_id_tot=1;

  return;

}

void AnalyzerCore::PrintBDTVariables(Electron el,TString label){
      
  if(!el.IsPrompt()) return;
  if(!el.IsEC()) return;

  cout << "PrintBDTVariables " << label << endl;
  InitializeIDTreeVars();

  Lepton *lep = (Lepton *)(&el);
  for(auto i : el.MAPBDT()) cout << i.first << "  " << i.second  << endl;
  SetBDTIDVar(lep);
  SetBDTIDVariablesElectron(el);
  PrintBDTInput();
  
}




void AnalyzerCore::FillBDTHists(Muon mu,TString cut, double w, bool SplitFakes){


  map<TString, double> mapBDT = mu.MAPBDT();
  for(auto imap : mapBDT )  FillHist("BDTVariables_"+cut+"/"+ mu.GetFlavour()+ "/"+imap.first, imap.second  , w, 200, -1., 1);

  InitializeIDTreeVars();
  Lepton *lep = (Lepton *)(&mu);
  SetBDTIDVar(lep);
  SetBDTIDVariablesMuon(mu);

  if(SplitFakes){
    TString lep_fake_tag=MatchGenDef(All_Gens, mu);
    vector<TString> Lists = {"pi+","q","K+","__","g","B0","B+","D0","D+"};
    if (std::find(Lists.begin(), Lists.end(), lep_fake_tag) == Lists.end()) return;
    cut=cut+"_"+lep_fake_tag;
  }
    
  FillHist( cut+ "/Lepton_bdt_id_Pt",        bdt_id_Pt, w, 100, 0., 500.,"");
  FillHist( cut+ "/Lepton_bdt_id_Eta",       bdt_id_Eta, w, 60, -3., 3.,"");
  FillHist( cut+ "/Lepton_bdt_id_PtRatio",   bdt_id_PtRatio, w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel",     bdt_id_PtRel, w, 500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatioV2",   bdt_id_PtRatioV2, w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRelV2",     bdt_id_PtRelV2, w, 500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatioV3",   bdt_id_PtRatioV3, w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRelV3",     bdt_id_PtRelV3, w, 500, 0., 500., "");

  FillHist( cut+ "/Lepton_bdt_id_MiniIsoChHad",  bdt_id_MiniIsoChHad, w, 200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_MiniIsoNHad",   bdt_id_MiniIsoNHad, w,  200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_MiniIsoPhHad",  bdt_id_MiniIsoPhHad, w, 200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_IsoChHad",      bdt_id_IsoChHad, w,     200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_IsoNHad",       bdt_id_IsoNHad, w,      200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_IsoPhHad" ,     bdt_id_IsoPhHad, w,     200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_RelMiniIsoCh",  bdt_id_RelMiniIsoCh, w, 200, 0, 0.4);
  FillHist( cut+ "/Lepton_bdt_id_RelMiniIsoN",   bdt_id_RelMiniIsoN, w,  200, 0, 0.4);
  FillHist( cut+ "/Lepton_bdt_id_RelIso",        bdt_id_RelIso, w,       200, 0, 6);
  FillHist( cut+ "/Lepton_bdt_id_Minireliso",    bdt_id_Minireliso, w,   200, 0, 2);

  FillHist( cut+ "/Lepton_bdt_id_CEMFracCJ",  bdt_id_CEMFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_NEMFracCJ",  bdt_id_NEMFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_EMFracCJ",  bdt_id_CEMFracCJ+bdt_id_NEMFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_CHFracCJ",   bdt_id_CHFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_NHFracCJ",   bdt_id_NHFracCJ, w,200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_HFracCJ",   bdt_id_CHFracCJ+bdt_id_NHFracCJ, w,200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_MuFracCJ",   bdt_id_MuFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJ",  bdt_id_JetDiscCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsB",  bdt_id_JetDiscCJCvsB, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsL",  bdt_id_JetDiscCJCvsL, w, 200, 0, 2);

  double JetDiscBins[6] = { 0.,0.1, 0.2, 0.3,  0.75, 1.};

  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJ2",  bdt_id_JetDiscCJ, w, 5, JetDiscBins);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsB2",  bdt_id_JetDiscCJCvsB, w, 5, JetDiscBins);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsL2",  bdt_id_JetDiscCJCvsL, w, 5, JetDiscBins);



  /// IP                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
  FillHist( cut+ "/Lepton_bdt_id_Dxy",     bdt_id_Dxy, w, 200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_DxySig",  bdt_id_DxySig, w, 200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_Dz" ,     bdt_id_Dz, w, 200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_DzSig",   bdt_id_DzSig, w,200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_IP3D",    bdt_id_IP3D, w, 100, 0, 8);

  FillHist( cut+ "/Lepton_bdt_id_MVA",     bdt_id_MVA, w, 220 , -1 ,  10);
  FillHist( cut+ "/Lepton_bdt_id_MVAIso",  bdt_id_MVAIso, w,220 , -1 ,10);

  FillHist( cut+ "/Lepton_bdt_id_MVARaw",     bdt_id_MVARaw, w,   500 , -1 ,1);
  FillHist( cut+ "/Lepton_bdt_id_MVAIsoRaw",  bdt_id_MVAIsoRaw, w,500 , -1 ,1);

  return;

}


void AnalyzerCore::FillBDTHists(Electron el,TString cut, double w, bool SplitFakes){
  

  map<TString, double> mapBDT = el.MAPBDT();
  for(auto imap : mapBDT )  FillHist("BDTVariables_"+cut+"/"+ el.GetFlavour()+ "/"+imap.first, imap.second  , w, 200, -1., 1);


  InitializeIDTreeVars();
  Lepton *lep = (Lepton *)(&el);
  SetBDTIDVar(lep);
  SetBDTIDVariablesElectron(el);

  FillHist( cut+ "/Lepton_bdt_id_Pt",        bdt_id_Pt, w, 100, 0., 500.,"");
  FillHist( cut+ "/Lepton_bdt_id_Eta",       bdt_id_Eta, w, 60, -3., 3.,"");
  FillHist( cut+ "/Lepton_bdt_id_PtBinned",  bdt_id_PtBinned, w, 5, 0,5 );
  FillHist( cut+ "/Lepton_bdt_id_PtRatio",   bdt_id_PtRatio, w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel",     bdt_id_PtRel, w, 500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatioV2",   bdt_id_PtRatioV2, w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRelV2",     bdt_id_PtRelV2, w, 500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatioV3",   bdt_id_PtRatioV3, w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRelV3",     bdt_id_PtRelV3, w, 500, 0., 500., "");


  FillHist( cut+ "/Lepton_bdt_id_PtRatio_CorrJet", JetLeptonPtRatioLepAwareMuon(el,true,false,false) , w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatio_CorrJetMu", JetLeptonPtRatioLepAwareMuon(el,true,true,false) , w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatio_UncorrLep_CorrJet", JetLeptonPtRatioLepAwareMuon(el,true,false,true) , w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatio_UncorrLep_CorrJetMu", JetLeptonPtRatioLepAwareMuon(el,true, true, true) , w,  500, 0., 2., "");
  
  
  FillHist( cut+ "/Lepton_bdt_id_PtRel_CorrJet", JetLeptonPtRelLepAwareMuon(el,true,false,false) , w,  500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel_CorrJetMu", JetLeptonPtRelLepAwareMuon(el,true,true,false) , w,  500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel_UncorrLep_CorrJet", JetLeptonPtRelLepAwareMuon(el,true,false,true) , w,  500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel_UncorrLep_CorrJetMu", JetLeptonPtRelLepAwareMuon(el,true, true, true) , w,  500, 0., 500., "");

  FillHist( cut+ "/Lepton_bdt_id_PtRatio_UnCorrJet", JetLeptonPtRatioLepAwareMuon(el,false,false,false) , w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatio_UnCorrJetMu", JetLeptonPtRatioLepAwareMuon(el,false,true,false) , w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatio_UncorrLep_UnCorrJet", JetLeptonPtRatioLepAwareMuon(el,false,false,true) , w,  500, 0., 2., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRatio_UncorrLep_UnCorrJetMu", JetLeptonPtRatioLepAwareMuon(el,false, true, true) , w,  500, 0., 2., "");


  FillHist( cut+ "/Lepton_bdt_id_PtRel_UnCorrJet", JetLeptonPtRelLepAwareMuon(el,false,false,false) , w,  500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel_UnCorrJetMu", JetLeptonPtRelLepAwareMuon(el,false,true,false) , w,  500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel_UncorrLep_UnCorrJet", JetLeptonPtRelLepAwareMuon(el,false,false,true) , w,  500, 0., 500., "");
  FillHist( cut+ "/Lepton_bdt_id_PtRel_UncorrLep_UnCorrJetMu", JetLeptonPtRelLepAwareMuon(el,false, true, true) , w,  500, 0., 500., "");


  if(1){

    std::vector<Jet> jets = GetAllJets();

    double  mindR=0.4;

    Jet closejet;
    Jet UnCorrJet;
    for(auto jet : jets){
      if(jet.Pt() < 10) continue; //// Make consistent with data cut
      
      if (lep->DeltaR(jet) < mindR) {
  UnCorrJet=jet;
  closejet = GetCorrectedJetCloseToLepton(*lep,jet);
  mindR = lep->DeltaR(jet);
      }
    }
    
    if (mindR == 0.4)  FillHist( cut+ "/Lepton_bdt_id_PtRatio_noLepCorr",  std::min(1./ (1. + el.RelIso()), 1.5) ,  w,  500, 0., 2., "");
    else FillHist( cut+ "/Lepton_bdt_id_PtRatio_noLepCorr",    std::min(el.Pt() / UnCorrJet.Pt(),1.5)  ,  w,  500, 0., 2., "");
        
    
    FillHist( cut+ "/Lepton_bdt_id_CHMultCJ",   closejet.CHMult() , w, 30, 0., 30., "");
    FillHist( cut+ "/Lepton_bdt_id_NMultCJ",   closejet.NMult() , w, 30, 0., 30, "");

    /////////// Now test corrected Energy Fractions
    /// Since Jet Energy is updated the fraction in MC needs updating
    
    //// values stored are those from MiniAOD
    ///https://github.com/cms-sw/cmssw/blob/master/DataFormats/PatCandidates/interface/Jet.h#L399

    
  }

  FillHist( cut+ "/Lepton_bdt_id_MiniIsoChHad",  bdt_id_MiniIsoChHad, w, 200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_MiniIsoNHad",   bdt_id_MiniIsoNHad, w,  200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_MiniIsoPhHad",  bdt_id_MiniIsoPhHad, w, 200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_IsoChHad",      bdt_id_IsoChHad, w,     200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_IsoNHad",       bdt_id_IsoNHad, w,      200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_IsoPhHad" ,     bdt_id_IsoPhHad, w,     200, 0, 100);
  FillHist( cut+ "/Lepton_bdt_id_RelMiniIsoCh",  bdt_id_RelMiniIsoCh, w, 200, 0, 0.4);
  FillHist( cut+ "/Lepton_bdt_id_RelMiniIsoN",   bdt_id_RelMiniIsoN, w,  200, 0, 0.4);
  FillHist( cut+ "/Lepton_bdt_id_RelIso",        bdt_id_RelIso, w,       200, 0, 6);
  FillHist( cut+ "/Lepton_bdt_id_Minireliso",    bdt_id_Minireliso, w,   200, 0, 2);

  FillHist( cut+ "/Lepton_bdt_id_CEMFracCJ",  bdt_id_CEMFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_NEMFracCJ",  bdt_id_NEMFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_EMFracCJ",  bdt_id_CEMFracCJ+bdt_id_NEMFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_CHFracCJ",   bdt_id_CHFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_NHFracCJ",   bdt_id_NHFracCJ, w,200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_HFracCJ",   bdt_id_CHFracCJ+bdt_id_NHFracCJ, w,200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_MuFracCJ",   bdt_id_MuFracCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJ",  bdt_id_JetDiscCJ, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsB",  bdt_id_JetDiscCJCvsB, w, 200, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsL",  bdt_id_JetDiscCJCvsL, w, 200, 0, 2);

  double JetDiscBins[6] = { 0.,0.1, 0.2, 0.3,  0.75, 1.};
  
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJ2",  bdt_id_JetDiscCJ, w, 5, JetDiscBins);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsB2",  bdt_id_JetDiscCJCvsB, w, 5, JetDiscBins);
  FillHist( cut+ "/Lepton_bdt_id_JetDiscCJCvsL2",  bdt_id_JetDiscCJCvsL, w, 5, JetDiscBins);



  /// IP                                                                                                                                                                                                                                                                                                                                                                                                                                 
  FillHist( cut+ "/Lepton_bdt_id_Dxy",     bdt_id_Dxy, w, 200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_DxySig",  bdt_id_DxySig, w, 200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_Dz" ,     bdt_id_Dz, w, 200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_DzSig",   bdt_id_DzSig, w,200 , -30, 10);
  FillHist( cut+ "/Lepton_bdt_id_IP3D",    bdt_id_IP3D, w, 100, 0, 8);

  FillHist( cut+ "/Lepton_bdt_id_MVA",     bdt_id_MVA, w, 220 , -1 ,  10);
  FillHist( cut+ "/Lepton_bdt_id_MVAIso",  bdt_id_MVAIso, w,220 , -1 ,10);

  FillHist( cut+ "/Lepton_bdt_id_MVARaw",     bdt_id_MVARaw, w,   500 , -1 ,1);
  FillHist( cut+ "/Lepton_bdt_id_MVAIsoRaw",  bdt_id_MVAIsoRaw, w,500 , -1 ,1);

  FillHist( cut+ "/Lepton_bdt_id_MissingHits",  bdt_id_MissingHits, w, 5 ,0 ,5);

  // El Shower/SCL                                                                                                                                                                                                                                                                                                                                                                                                                       
  FillHist( cut+ "/Lepton_bdt_id_Full5x5_sigmaIetaIeta",  bdt_id_Full5x5_sigmaIetaIeta, w, 200 , 0 , 0.04);
  FillHist( cut+ "/Lepton_bdt_id_dEtaSeed",  bdt_id_dEtaSeed, w, 200, 0., 0.05);
  FillHist( cut+ "/Lepton_bdt_id_dPhiIn",    bdt_id_dPhiIn, w, 200, 0. , 0.2);
  FillHist( cut+ "/Lepton_bdt_id_dEtaIn",  bdt_id_dEtaIn, w, 200, 0., 0.1);
  FillHist( cut+ "/Lepton_bdt_id_EtaWidth",  bdt_id_EtaWidth, w, 200 , 0 , 1);
  FillHist( cut+ "/Lepton_bdt_id_PhiWidth",  bdt_id_PhiWidth, w, 400, 0, 2);
  FillHist( cut+ "/Lepton_bdt_id_HoverE "  , bdt_id_HoverE, w, 150 , 0 , 1.5);
  FillHist( cut+ "/Lepton_bdt_id_TrkIso",  bdt_id_TrkIso, w, 200, 0, 5);
  FillHist( cut+ "/Lepton_bdt_id_e2x5OverE5x5",  bdt_id_e2x5OverE5x5, w, 220, 0, 1.1);
  FillHist( cut+ "/Lepton_bdt_id_e1x5OverE5x5",  bdt_id_e1x5OverE5x5, w, 350, -1, 2.5);
  FillHist( cut+ "/Lepton_bdt_id_e55",  bdt_id_e55, w, 400, 0 , 8000);
  FillHist( cut+ "/Lepton_bdt_id_dr03TkSumPt",  bdt_id_dr03TkSumPt, w,  250, 0 , 0.25);
  FillHist( cut+ "/Lepton_bdt_id_InvEminusInvP",  bdt_id_InvEminusInvP, w, 200, 0 ,2);
  FillHist( cut+ "/Lepton_bdt_id_ecalPFClusterIso",  bdt_id_ecalPFClusterIso, w,200,0 , 0.5);
  FillHist( cut+ "/Lepton_bdt_id_hcalPFClusterIso",  bdt_id_hcalPFClusterIso, w,200,0 , 0.5);

  // IDs                                                                                                                                                                                                                                                                                                                                                                                                                                 
  FillHist( cut+ "/Lepton_bdt_id_EoverP",  bdt_id_EoverP, w, 200, -10 ,10);
  FillHist( cut+ "/Lepton_bdt_id_FBrem",  bdt_id_FBrem, w,  200, -2 , 2);
  FillHist( cut+ "/Lepton_bdt_id_Rho",  bdt_id_Rho, w,  200, 0 , 20);
  FillHist( cut+ "/Lepton_bdt_id_IsGsfCtfScPixChargeConsistent",  bdt_id_IsGsfCtfScPixChargeConsistent , w, 3 ,0 ,3);
  FillHist( cut+ "/Lepton_bdt_id_IsGsfScPixChargeConsistent",  bdt_id_IsGsfScPixChargeConsistent , w, 3 ,0 ,3);
  FillHist( cut+ "/Lepton_bdt_id_IsGsfCtfChargeConsistent",  bdt_id_IsGsfCtfChargeConsistent , w,3 ,0 ,3);
  FillHist( cut+ "/Lepton_bdt_id_R9",  bdt_id_R9, w, 200, 0., 5);
  FillHist( cut+ "/Lepton_bdt_id_PSEoverRawE", bdt_id_psEoRraw, w, 1000, 0., 1);

  return;

}


void AnalyzerCore::PrintBDTInput(){


  cout << "bdt_id_Pt " << bdt_id_Pt<< endl;
  cout << "bdt_id_PtBinned " << bdt_id_PtBinned<< endl;
  cout << "bdt_id_Eta " << bdt_id_Eta<< endl;
  cout << "bdt_id_PtRatio " << bdt_id_PtRatio<< endl;
  cout << "bdt_id_PtRel " << bdt_id_PtRel<< endl;
  cout << "bdt_id_PileUp " << bdt_id_PileUp << endl;

  cout << "bdt_id_MiniIsoChHad " << bdt_id_MiniIsoChHad<< endl;;  
  cout << "bdt_id_MiniIsoNHad " << bdt_id_MiniIsoNHad<< endl;;  
  cout << "bdt_id_MiniIsoPhHad " << bdt_id_MiniIsoPhHad<< endl;;

  cout << "bdt_id_IsoChHad " << bdt_id_IsoChHad<< endl;;  
  cout << "bdt_id_IsoNHad " << bdt_id_IsoNHad<< endl;;  
  cout << "bdt_id_IsoPhHad" << bdt_id_IsoPhHad<< endl;;
  cout << "bdt_id_Minireliso " << bdt_id_Minireliso<< endl;
  cout << "bdt_id_RelMiniIsoCh " << bdt_id_RelMiniIsoCh<< endl;
  cout << "bdt_id_RelMiniIsoN " << bdt_id_RelMiniIsoN<< endl;
  cout << "bdt_id_RelIso " << bdt_id_RelIso<< endl;;

  cout << "bdt_id_CEMFracCJ " << bdt_id_CEMFracCJ<< endl;
  cout << "bdt_id_NEMFracCJ " << bdt_id_NEMFracCJ<< endl;
  cout << "bdt_id_CHFracCJ " << bdt_id_CHFracCJ<< endl;
  cout << "bdt_id_NHFracCJ " << bdt_id_NHFracCJ<< endl;
  cout << "bdt_id_MuFracCJ " << bdt_id_MuFracCJ<< endl;
  cout << "bdt_id_JetDiscCJ " << bdt_id_JetDiscCJ<< endl;

  /// IP                                                                                                                                                                                                                                    
  cout << "bdt_id_Dxy " << bdt_id_Dxy<< endl; 
  cout << "bdt_id_DxySig " << bdt_id_DxySig<< endl;
  cout << "bdt_id_Dz" << bdt_id_Dz<< endl;
  cout << "bdt_id_DzSig " << bdt_id_DzSig<< endl;
  cout << "bdt_id_IP3D " << bdt_id_IP3D<< endl;

  cout << "bdt_id_MVA " << bdt_id_MVA<< endl;
  cout << "bdt_id_MVAIso " << bdt_id_MVAIso<< endl;

  cout << "bdt_id_MissingHits " << bdt_id_MissingHits<< endl;

  // El Shower/SCL                                                                                                                                                                                                                          
  cout << "bdt_id_Full5x5_sigmaIetaIeta " << bdt_id_Full5x5_sigmaIetaIeta<< endl;
  cout << "bdt_id_dEtaSeed " << bdt_id_dEtaSeed<< endl;
  cout << "bdt_id_dPhiIn " << bdt_id_dPhiIn<< endl;
  cout << "bdt_id_dEtaIn " << bdt_id_dEtaIn<< endl;
  cout << "bdt_id_EtaWidth " << bdt_id_EtaWidth<< endl;  
  cout << "bdt_id_PhiWidth " << bdt_id_PhiWidth<< endl;  
  cout << "bdt_id_HoverE "  << bdt_id_HoverE<< endl;
  cout << "bdt_id_TrkIso " << bdt_id_TrkIso<< endl; 
  cout << "bdt_id_e2x5OverE5x5 " << bdt_id_e2x5OverE5x5<< endl;;  
  cout << "bdt_id_e1x5OverE5x5 " << bdt_id_e1x5OverE5x5<< endl;;  
  cout << "bdt_id_e55 " << bdt_id_e55<< endl;;  

  cout << "bdt_id_dr03HcalTowerSumEt " << bdt_id_dr03HcalTowerSumEt<< endl;;  
  cout << "bdt_id_dr03TkSumPt " << bdt_id_dr03TkSumPt<< endl;;
  cout << "bdt_id_InvEminusInvP " << bdt_id_InvEminusInvP<< endl;  

  cout << "bdt_id_ecalPFClusterIso " << bdt_id_ecalPFClusterIso<< endl; 
  cout << "bdt_id_hcalPFClusterIso " << bdt_id_hcalPFClusterIso<< endl;

  // IDs                                                                                                                                                                                                                                    
  cout << "bdt_id_isEcalDriven " << bdt_id_isEcalDriven << endl;  
  cout << "bdt_id_EoverP " << bdt_id_EoverP<< endl;;  
  cout << "bdt_id_FBrem " << bdt_id_FBrem<< endl;;  
  cout << "bdt_id_PassConversionVeto " << bdt_id_PassConversionVeto<< endl; ;
  cout << "bdt_id_IsGsfCtfScPixChargeConsistent " << bdt_id_IsGsfCtfScPixChargeConsistent << endl;;  
  cout << "bdt_id_IsGsfScPixChargeConsistent " << bdt_id_IsGsfScPixChargeConsistent << endl;;  
  cout << "bdt_id_IsGsfCtfChargeConsistent " << bdt_id_IsGsfCtfChargeConsistent << endl;;  
  cout << "bdt_id_R9 " << bdt_id_R9<< endl;;
  cout << "bdt_id_psEoRraw " << bdt_id_psEoRraw << endl;
  
  return;
 
}




double AnalyzerCore::GetBDTScoreMuon(Muon mu ,BkgType bkg, TString BDTTag){

  InitializeIDTreeVars();

  Lepton *lep = (Lepton *)(&mu);
  SetBDTIDVar(lep);
  SetBDTIDVariablesMuon(mu);


  TString MVATagStr = BDTTag;
  if (bkg == BkgType::Fake) MVATagStr += "_Fake";
  if (bkg == BkgType::FakeRate) MVATagStr += "_Fake";

  //// Version 4 is NoPt Reader
  if(iSetupLeptonBDTv4){
    if(MVATagStr.Contains("v4"))  return  MuonIDv4_FakeMVAReader->EvaluateMVA(MVATagStr);
  }    
  if(iSetupLeptonBDTv5){

    if(MVATagStr.Contains("v5"))  return  MuonIDv5_FakeMVAReader->EvaluateMVA(MVATagStr);
  }

  return -1;  
}

void AnalyzerCore::SetBDTIDVarV1(Lepton*  lep){

  bdt_id_Pt    = (lep->Pt() > 500)  ? 499 : lep->Pt();
  bdt_id_Eta   = fabs(lep->Eta());
  bdt_id_MiniIsoChHad = lep->MiniIsoChHad();
  bdt_id_MiniIsoNHad = lep->MiniIsoNHad();
  bdt_id_MiniIsoPhHad = lep->MiniIsoPhHad();
  bdt_id_IsoChHad = lep->IsoChHad();
  bdt_id_IsoNHad = lep->IsoNHad();
  bdt_id_IsoPhHad = lep->IsoPhHad();
  bdt_id_Dxy   = lep->LogdXY();
  bdt_id_DxySig   = lep->LogdXYSig();
  bdt_id_Dz   = lep->LogdZ();
  bdt_id_DzSig   = lep->LogdZSig();
  bdt_id_IP3D    = lep->SIP3D();
  bdt_id_RelIso    =  lep->RelIso();
  bdt_id_Minireliso   = lep->MiniRelIso();
  bdt_id_RelMiniIsoCh = lep->MiniRelIsoCharged();
  bdt_id_RelMiniIsoN = lep->MiniRelIsoNeutral();

  //==== Vars for non-prompt lepton bkg                                                               

                                                                                                  
  int IdxMatchJet=-1;
  float mindR1=999.;
  std::vector<Jet>   JetAllColl = GetJets("NoID", 10., 5.0);

  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
    float dR1=lep->DeltaR(JetAllColl.at(ij));
    if(dR1>0.4) continue;
    if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
  }

  if(IdxMatchJet!=-1){
    bdt_id_CEMFracCJ = JetAllColl.at(IdxMatchJet).ChargedEmEnergyFraction();
    bdt_id_NEMFracCJ = JetAllColl.at(IdxMatchJet).NeutralEmEnergyFraction();
    bdt_id_CHFracCJ  = JetAllColl.at(IdxMatchJet).ChargedHadEnergyFraction();
    bdt_id_NHFracCJ  = JetAllColl.at(IdxMatchJet).NeutralHadEnergyFraction();
    bdt_id_MuFracCJ  = JetAllColl.at(IdxMatchJet).MuonEnergyFraction();
    bdt_id_JetDiscCJ = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
  }

  else{
    bdt_id_CEMFracCJ=0, bdt_id_NEMFracCJ=0., bdt_id_CHFracCJ=0., bdt_id_NHFracCJ=0., bdt_id_MuFracCJ=0., bdt_id_JetDiscCJ=0.;
  }

  return;
}

void AnalyzerCore::SetBDTIDVarOLD(Lepton*  lep){

  bdt_id_PileUp = nPV;
  
  bdt_id_Pt    = (lep->Pt() > 500)  ? 499 : lep->Pt();
  
  if (lep->Pt() < 50.) bdt_id_PtBinned= 0;
  else     if (lep->Pt() < 100.) bdt_id_PtBinned= 1;
  else     if (lep->Pt() < 200.) bdt_id_PtBinned= 2;
  else bdt_id_PtBinned =3;

  bdt_id_Eta   = fabs(lep->Eta());

  if (lep->MiniIsoChHad() > 150) bdt_id_MiniIsoChHad = 150;
  else   bdt_id_MiniIsoChHad = lep->MiniIsoChHad();

  if (lep->MiniIsoNHad() > 150) bdt_id_MiniIsoNHad = 150.;
  else   bdt_id_MiniIsoNHad = lep->MiniIsoNHad();

  if(lep->MiniIsoPhHad() > 150.) bdt_id_MiniIsoPhHad = 150.;
  else bdt_id_MiniIsoPhHad = lep->MiniIsoPhHad();

  if(lep->IsoChHad() > 150) bdt_id_IsoChHad = 150;
  else bdt_id_IsoChHad = lep->IsoChHad();
  if(lep->IsoNHad()  > 150)  bdt_id_IsoNHad  = 150;
  else   bdt_id_IsoNHad = lep->IsoNHad();
  if(lep->IsoPhHad() > 150) bdt_id_IsoPhHad  = 150; 
  else bdt_id_IsoPhHad = lep->IsoPhHad();

  bdt_id_Dxy      = lep->LogdXY();
  bdt_id_RelDxy   = lep->LogdXY()/lep->Pt();
  bdt_id_DxySig   = lep->LogdXYSig();
  bdt_id_Dz       = lep->LogdZ();
  bdt_id_RelDz    = lep->LogdZ()/lep->Pt();
  bdt_id_DzSig    = lep->LogdZSig();
  bdt_id_IP3D     = lep->SIP3D();
  bdt_id_RelIP3D    = lep->SIP3D()/lep->Pt();
  if (lep->RelIso() > 1)  bdt_id_RelIso = 1;
  else  bdt_id_RelIso     =  lep->RelIso();

  bdt_id_Minireliso   = lep->MiniRelIso();
  bdt_id_RelMiniIsoCh = lep->MiniRelIsoCharged();
  bdt_id_RelMiniIsoN = lep->MiniRelIsoNeutral();

  //==== Vars for non-prompt lepton bkg                                                               

                                                                                                  
  int IdxMatchJet=-1;
  float mindR1=999.;
  std::vector<Jet>   JetAllColl = GetAllJets();

  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
    float dR1=lep->DeltaR(JetAllColl.at(ij));
    if(dR1>0.4) continue;
    if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
  }

  if(IdxMatchJet!=-1){

    bdt_id_CEMFracCJ = JetAllColl.at(IdxMatchJet).ChargedEmEnergyFraction();
    bdt_id_NEMFracCJ = JetAllColl.at(IdxMatchJet).NeutralEmEnergyFraction();
    bdt_id_CHFracCJ  = JetAllColl.at(IdxMatchJet).ChargedHadEnergyFraction();
    bdt_id_NHFracCJ  = JetAllColl.at(IdxMatchJet).NeutralHadEnergyFraction();
    bdt_id_MuFracCJ  = JetAllColl.at(IdxMatchJet).MuonEnergyFraction();
    bdt_id_JetDiscCJ = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
  }

  else{
    bdt_id_CEMFracCJ=1.5, bdt_id_NEMFracCJ=1.5, bdt_id_CHFracCJ=1.5, bdt_id_NHFracCJ=1.5, bdt_id_MuFracCJ=1.5, bdt_id_JetDiscCJ=1.5;
  }

  return;
}





void AnalyzerCore::SetBDTIDVar(Lepton*  lep){

  bdt_id_PileUp = nPV;
  
  bdt_id_Pt    = (lep->Pt() > 500)  ? 499 : lep->Pt();


  if (lep->Pt() < 50.) bdt_id_PtBinned= 0;
  else     if (lep->Pt() < 100.) bdt_id_PtBinned= 1;
  else     if (lep->Pt() < 200.) bdt_id_PtBinned= 2;
  else bdt_id_PtBinned =3;

  if (lep->Pt() < 20.) bdt_id_PtBinned2= 0;
  else     if (lep->Pt() < 30.) bdt_id_PtBinned2= 1;
  else     if (lep->Pt() < 50.) bdt_id_PtBinned2= 2;
  else     if (lep->Pt() < 60.) bdt_id_PtBinned2= 3;
  else     if (lep->Pt() < 70.) bdt_id_PtBinned2= 4;
  else     if (lep->Pt() < 80.) bdt_id_PtBinned2= 5;
  else     if (lep->Pt() < 90.) bdt_id_PtBinned2= 6;
  else     if (lep->Pt() < 100.) bdt_id_PtBinned2= 7;
  else     if (lep->Pt() < 150.) bdt_id_PtBinned2= 8;
  else     if (lep->Pt() < 200.) bdt_id_PtBinned2= 9;
  else bdt_id_PtBinned2 =10;

  bdt_id_Eta   = fabs(lep->Eta());

  /////// ISOLATION 

  /// MINI ISO
  if (lep->MiniIsoChHad() > 20) bdt_id_MiniIsoChHad = 20;
  else   bdt_id_MiniIsoChHad = lep->MiniIsoChHad();
  if (lep->MiniIsoNHad() > 20) bdt_id_MiniIsoNHad = 20.;
  else   bdt_id_MiniIsoNHad = lep->MiniIsoNHad();
  if(lep->MiniIsoPhHad() > 20.) bdt_id_MiniIsoPhHad = 20.;
  else bdt_id_MiniIsoPhHad = lep->MiniIsoPhHad();

  //// pfIsolationVariables
  if(lep->IsoChHad() > 20) bdt_id_IsoChHad = 20;
  else bdt_id_IsoChHad = lep->IsoChHad();
  if(lep->IsoNHad()  > 20)  bdt_id_IsoNHad  = 20;
  else   bdt_id_IsoNHad = lep->IsoNHad();
  if(lep->IsoPhHad() > 20) bdt_id_IsoPhHad  = 20; 
  else bdt_id_IsoPhHad = lep->IsoPhHad();

  bdt_id_Dxy      = lep->LogdXY();
  bdt_id_DxySig   = lep->LogdXYSig();
  bdt_id_Dz       = lep->LogdZ();
  bdt_id_DzSig    = lep->LogdZSig();
  bdt_id_IP3D     = lep->SIP3D();

   
  if (lep->RelIso() > 1)  bdt_id_RelIso = 1;
  else  bdt_id_RelIso     =  lep->RelIso();

  bdt_id_Minireliso   = lep->MiniRelIso();
  bdt_id_RelMiniIsoCh = lep->MiniRelIsoCharged();
  bdt_id_RelMiniIsoN  = lep->MiniRelIsoNeutral();

  //==== Vars for non-prompt lepton bkg                                                               

                                                                                                  
  int IdxMatchJet=-1;
  float mindR1=999.;
  std::vector<Jet>   JetAllColl = GetAllJets();
  
  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
    float dR1=lep->DeltaR(JetAllColl.at(ij));
    if(dR1>0.4) continue;
    if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
  }
  
  if(IdxMatchJet!=-1){

    bdt_id_CEMFracCJ = JetAllColl.at(IdxMatchJet).ChargedEmEnergyFraction();
    bdt_id_NEMFracCJ = JetAllColl.at(IdxMatchJet).NeutralEmEnergyFraction();
    bdt_id_CHFracCJ  = JetAllColl.at(IdxMatchJet).ChargedHadEnergyFraction();
    bdt_id_NHFracCJ  = JetAllColl.at(IdxMatchJet).NeutralHadEnergyFraction();
    bdt_id_MuFracCJ  = JetAllColl.at(IdxMatchJet).MuonEnergyFraction();
    bdt_id_JetDiscCJ = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
    bdt_id_JetDiscCJCvsB = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsB);
    bdt_id_JetDiscCJCvsL = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsL);
  }

  else{
    bdt_id_CEMFracCJ=1., bdt_id_NEMFracCJ=0., bdt_id_CHFracCJ=0., bdt_id_NHFracCJ=0, bdt_id_MuFracCJ=1.5, bdt_id_JetDiscCJ=0.1;
    bdt_id_JetDiscCJCvsB=0.4;
    bdt_id_JetDiscCJCvsL=0.1;
   
  }

  /////// TEST                                                                                                                                                                                            
  return;
}

void AnalyzerCore::SetupIDMVAReaderDefault(bool ForceSetupV4, bool ForceSetupV5){
  
  bool SkimContainsVersion4 = (fChain->GetBranch("electron_mva_fake_ed_v4"));
  bool SkimContainsVersion5 = (fChain->GetBranch("electron_mva_fake_ed_v5"));
  
  bool SetupVersion4 = (!SkimContainsVersion4 || ForceSetupV4);
  bool SetupVersion5 = (!SkimContainsVersion5 || ForceSetupV5);
  
  SetupLeptonBDT(SetupVersion4,SetupVersion5); /// This initialises MVAReaders and sets iSetupLeptonBDTvX bools

  if(SetupVersion4){
    // Muon ID Setup  Books weight files
    if(fChain->GetBranch("muon_mva_fake_v4")){
      cout << "WARNING SetupIDMVAReaderDefault set tp force V4 setup when running on a skimmed sample with V4 variables in ntup" << endl;
    }
    if(fChain->GetBranch("electron_mva_fake_ed_v4")) {
      cout << "WARNING SetupIDMVAReaderDefault set tp force V4 setup when running on a skimmed sample with V4 variables in ntup" << endl;
    }
    SetupIDMVAReaderElectron();
  }

  if(SetupVersion5){
    
    if(fChain->GetBranch("electron_mva_fake_ed_v5"))         cout << "WARNING SetupIDMVAReaderDefault set tp force V5 setup when running on a skimmed sample with V4 variables in ntup" << endl;
    SetupIDMVAReaderElectronUpdate(); 
  }

  if(SetupVersion4 || SetupVersion5) SetupIDMVAReaderMuon();


  return ;
}


void AnalyzerCore::SetupIDMVAReaderMuon(){

  if(! ( iSetupLeptonBDTv4 || iSetupLeptonBDTv5)) return;
    
  InitializeIDTreeVars();

  //// IF branch for var exists in ntuple by default dont setup reader

  // Fake                                                                                                                                                                    
  if(iSetupLeptonBDTv5){
    MuonIDv5_FakeMVAReader->AddVariable("Eta", &bdt_id_Eta);
    MuonIDv5_FakeMVAReader->AddVariable("MiniIsoChHad", &bdt_id_MiniIsoChHad);
    MuonIDv5_FakeMVAReader->AddVariable("MiniIsoPhHad",&bdt_id_MiniIsoPhHad);
    MuonIDv5_FakeMVAReader->AddVariable("MiniIsoNHad",&bdt_id_MiniIsoNHad);
    MuonIDv5_FakeMVAReader->AddVariable("RelMiniIsoCh", &bdt_id_RelMiniIsoCh);
    MuonIDv5_FakeMVAReader->AddVariable("RelMiniIsoN",&bdt_id_RelMiniIsoN);
    MuonIDv5_FakeMVAReader->AddVariable("RelIso",&bdt_id_RelIso);
    MuonIDv5_FakeMVAReader->AddVariable("Dxy",  &bdt_id_Dxy);
    MuonIDv5_FakeMVAReader->AddVariable("DxySig",  &bdt_id_DxySig);
    MuonIDv5_FakeMVAReader->AddVariable("Dz",  &bdt_id_Dz);
    MuonIDv5_FakeMVAReader->AddVariable("DzSig",  &bdt_id_DzSig);
    MuonIDv5_FakeMVAReader->AddVariable("IP3D", &bdt_id_IP3D);
    MuonIDv5_FakeMVAReader->AddVariable("PtRatio",  &bdt_id_PtRatio);
    MuonIDv5_FakeMVAReader->AddVariable("PtRel",  &bdt_id_PtRel);
    MuonIDv5_FakeMVAReader->AddVariable("JetDiscCJ"    ,&bdt_id_JetDiscCJ);
    MuonIDv5_FakeMVAReader->AddVariable("JetDiscCJCvsB",&bdt_id_JetDiscCJCvsB);
    MuonIDv5_FakeMVAReader->AddVariable("JetDiscCJCvsL",&bdt_id_JetDiscCJCvsL);
    MuonIDv5_FakeMVAReader->AddVariable("NEMFracCJ",&bdt_id_NEMFracCJ);
    MuonIDv5_FakeMVAReader->AddVariable("CHFracCJ",&bdt_id_CHFracCJ);
    MuonIDv5_FakeMVAReader->AddVariable("NHFracCJ",&bdt_id_NHFracCJ);
    MuonIDv5_FakeMVAReader->AddVariable("MuFracCJ",&bdt_id_MuFracCJ);
    MuonIDv5_FakeMVAReader->AddVariable("MVA",  &bdt_id_MVA);
    MuonIDv5_FakeMVAReader->AddVariable("Chi2",  &bdt_id_Chi2);
    MuonIDv5_FakeMVAReader->AddVariable("Validhits",  &bdt_id_Validhits);
    MuonIDv5_FakeMVAReader->AddVariable("Matched_stations",  &bdt_id_Matched_stations);
    MuonIDv5_FakeMVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);
  }

  if(iSetupLeptonBDTv4){
    MuonIDv4_FakeMVAReader->AddVariable("Eta", &bdt_id_Eta);
    MuonIDv4_FakeMVAReader->AddVariable("MiniIsoChHad", &bdt_id_MiniIsoChHad);
    MuonIDv4_FakeMVAReader->AddVariable("MiniIsoPhHad",&bdt_id_MiniIsoPhHad);
    MuonIDv4_FakeMVAReader->AddVariable("MiniIsoNHad",&bdt_id_MiniIsoNHad);
    MuonIDv4_FakeMVAReader->AddVariable("RelMiniIsoCh", &bdt_id_RelMiniIsoCh);
    MuonIDv4_FakeMVAReader->AddVariable("RelMiniIsoN",&bdt_id_RelMiniIsoN);
    MuonIDv4_FakeMVAReader->AddVariable("Dxy",  &bdt_id_Dxy);
    MuonIDv4_FakeMVAReader->AddVariable("DxySig",  &bdt_id_DxySig);
    MuonIDv4_FakeMVAReader->AddVariable("Dz",  &bdt_id_Dz);
    MuonIDv4_FakeMVAReader->AddVariable("DzSig",  &bdt_id_DzSig);
    MuonIDv4_FakeMVAReader->AddVariable("RelIso", &bdt_id_RelIso);
    MuonIDv4_FakeMVAReader->AddVariable("IP3D", &bdt_id_IP3D);
    MuonIDv4_FakeMVAReader->AddVariable("PtRatio",  &bdt_id_PtRatio);
    MuonIDv4_FakeMVAReader->AddVariable("PtRel",  &bdt_id_PtRel);
    MuonIDv4_FakeMVAReader->AddVariable("NEMFracCJ",&bdt_id_NEMFracCJ);
    MuonIDv4_FakeMVAReader->AddVariable("CHFracCJ",&bdt_id_CHFracCJ);
    MuonIDv4_FakeMVAReader->AddVariable("JetDiscCJ",&bdt_id_JetDiscCJ);
    MuonIDv4_FakeMVAReader->AddVariable("NHFracCJ",&bdt_id_NHFracCJ);
    MuonIDv4_FakeMVAReader->AddVariable("MuFracCJ",&bdt_id_MuFracCJ);
    MuonIDv4_FakeMVAReader->AddVariable("MVA",  &bdt_id_MVA);
    MuonIDv4_FakeMVAReader->AddVariable("Chi2",  &bdt_id_Chi2);
    MuonIDv4_FakeMVAReader->AddVariable("Validhits",  &bdt_id_Validhits);
    MuonIDv4_FakeMVAReader->AddVariable("Matched_stations",  &bdt_id_Matched_stations);
    MuonIDv4_FakeMVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);
  }

  TString AnalyzerPath=std::getenv("SKFlat_WD");

  TString MVAPathMuonFakeV4="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIMuonFake/Version6/"+GetYearString()+"/dataset/weights/";
  TString MVAPathMuonFakeV5="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIMuonFake/Version12/"+GetYearString()+"/dataset/weights/";

  TString xmlpf = "_TMVAClassification_BDTG.weights.xml";


  TString BDTGv4_Fake = "";
  if(GetYear() == 2016) BDTGv4_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf;
  if(GetYear() == 2017) BDTGv4_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf;
  if(GetYear() == 2018) BDTGv4_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf;

  //----> 0.988444335798: ['0.135', '0.129', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
  //----> 0.983863690644: ['0.72', '0.151', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
  //----> 0.987515887664: ['0.805', '0.28', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']

  TString BDTGv4_BB_Fake = "";
  if(GetYear() == 2016) BDTGv4_BB_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtBB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf;
  if(GetYear() == 2017) BDTGv4_BB_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtBB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf;
  if(GetYear() == 2018) BDTGv4_BB_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtBB_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+xmlpf;

  TString BDTGv4_EC_Fake = "";
  if(GetYear() == 2016) BDTGv4_EC_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtEC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf;
  if(GetYear() == 2017) BDTGv4_EC_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtEC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+xmlpf;
  if(GetYear() == 2018) BDTGv4_EC_Fake = "BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtEC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+xmlpf;

  if(iSetupLeptonBDTv4){
    MuonIDv4_FakeMVAReader->BookMVA("BDTGv4_Fake",    MVAPathMuonFakeV4+BDTGv4_Fake);
    MuonIDv4_FakeMVAReader->BookMVA("BDTGv4_BB_Fake", MVAPathMuonFakeV4+BDTGv4_BB_Fake);
    MuonIDv4_FakeMVAReader->BookMVA("BDTGv4_EC_Fake", MVAPathMuonFakeV4+BDTGv4_EC_Fake);
  }
  // ----> 0.990249335302: ['0.478', '0.195', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtBB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
  // ----> 0.983556816291: ['0.167', '0.551', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtEC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
  //----> 0.988444335798: ['0.135', '0.129', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']

  //----> 0.988732999523: ['0.348', '0.105', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtBB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
  // ----> 0.981577613688: ['0.479', '0.184', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtEC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
  //----> 0.983863690644: ['0.72', '0.151', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']

  // ----> 0.98889260451: ['0.742', '0.172', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtBB_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
  // ----> 0.982613570019: ['0.639', '0.217', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPtEC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT.root']
  //----> 0.987515887664: ['0.805', '0.28', 'BDTG_version6_MuonFakeBkg_LF_TypeI_MuMu_SignalMuonFakeNoPt_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']

  if(iSetupLeptonBDTv5){

    if(GetYear() == 2016){
      //MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LFvsHF_Fake",           MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_LFvsHF_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);  

      //MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_HFBvsHFC_Fake",     MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_HFBvsHFC_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);  

      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_LFvsHF_Fake", 
              MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFvsHF_2016_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);  

      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_HFBvsHFC_Fake", 
              MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_HFBvsHFC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);  

      //----> 0.812412822248: ['0.81', '0.118', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_HFBvsHFC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
      //----> 0.857725962924: ['0.131', '0.191', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_HFBvsHFC_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']

      //----> 0.909569086244: ['0.121', '0.299', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFvsHF_2016_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']

      //----> 0.954268329212: ['0.112', '0.293', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_LFvsHF_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']


      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathMuonFakeV5 +"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother1_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LF2_Fake",MVAPathMuonFakeV5 +"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother2_2016_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_300_BDT"+ xmlpf);


      //----> 0.854619468881: ['0.12', '0.443', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother1_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']

      //----> 0.82287088718: ['0.254', '0.115', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother2_2016_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_300_BDT.root']

    }
    if(GetYear() == 2017){
      ///MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LFvsHF_Fake",                  MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_LFvsHF_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);   

      ///MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_HFBvsHFC_Fake",     MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_HFBvsHFC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);   

      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_LFvsHF_Fake",
              MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFvsHF_2017_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);   

      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_HFBvsHFC_Fake",
              MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_HFBvsHFC_2017_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);   

      //----> 0.819748332215: ['0.386', '0.102', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_HFBvsHFC_2017_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
      //----> 0.872141270213: ['0.436', '0.11', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_HFBvsHFC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
      //----> 0.918010546249: ['0.233', '0.39', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFvsHF_2017_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
      //----> 0.956586987068: ['0.172', '0.135', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_LFvsHF_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathMuonFakeV5 +"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother1_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_100_BDT"+ xmlpf);      
      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LF2_Fake",MVAPathMuonFakeV5 +"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother2_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_100_BDT"+ xmlpf);


      //----> 0.860615816119: ['0.139', '0.885', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother1_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_100_BDT.root']
        

      //----> 0.824113180693: ['0.174', '0.107', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother2_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_100_BDT.root']

    }
   
    if(GetYear() == 2018){
      //MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LFvsHF_Fake",        MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_LFvsHF_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);   
      //MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_HFBvsHFC_Fake",      MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_HFBvsHFC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);   
      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_LFvsHF_Fake",    MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFvsHF_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);   
      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_HFBvsHFC_Fake",  MVAPathMuonFakeV5+"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_HFBvsHFC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);   

      //----> 0.823428671706: ['0.283', '0.125', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_HFBvsHFC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
      //----> 0.874450108543: ['0.813', '0.104', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_HFBvsHFC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_100_BDT.root']
      //----> 0.920109543703: ['0.106', '0.101', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFvsHF_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
      //----> 0.956505115356: ['0.106', '0.673', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_LFvsHF_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']

      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathMuonFakeV5 +"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother1_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+ xmlpf);
      MuonIDv5_FakeMVAReader->BookMVA("BDTGv5_LF2_Fake",MVAPathMuonFakeV5 +"BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother2_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+ xmlpf);


      //----> 0.866496726781: ['0.161', '0.263', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother1_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']

      //----> 0.817553405836: ['0.176', '0.141', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother2_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']

      //----> 0.719608872018: ['0.112', '0.147', 'BDTG_version12_MuonFakeBkg_TypeI_MuMu_SignalMuonFake_QCD_LFMother3_2018_NTrees300_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']

    }
  }

  return;
 
}



void AnalyzerCore::SetupIDMVAReaderElectronUpdate(){
  
  InitializeIDTreeVars();
  

  ////// Version5 has merged to have just one reader for Fake/Conv
  
  /////// Barrel
  ElectronIDv5_MVAReader->AddVariable("Pt",           &bdt_id_Pt);
  ElectronIDv5_MVAReader->AddVariable("Eta",          &bdt_id_Eta);
  ElectronIDv5_MVAReader->AddVariable("MiniIsoChHad", &bdt_id_MiniIsoChHad);
  ElectronIDv5_MVAReader->AddVariable("MiniIsoPhHad", &bdt_id_MiniIsoPhHad);
  ElectronIDv5_MVAReader->AddVariable("MiniIsoNHad",  &bdt_id_MiniIsoNHad);
  ElectronIDv5_MVAReader->AddVariable("RelMiniIsoCh", &bdt_id_RelMiniIsoCh);
  ElectronIDv5_MVAReader->AddVariable("RelMiniIsoN",  &bdt_id_RelMiniIsoN);
  ElectronIDv5_MVAReader->AddVariable("RelIso",       &bdt_id_RelIso);
  if(DataYear == 2016){
    ElectronIDv5_MVAReader->AddVariable("Dxy",        &bdt_id_Dxy);
    ElectronIDv5_MVAReader->AddVariable("Dz",         &bdt_id_Dz);
  }
  else{
    ElectronIDv5_MVAReader->AddVariable("DxySig",     &bdt_id_DxySig);
    ElectronIDv5_MVAReader->AddVariable("DzSig",     &bdt_id_DzSig);
  }
  ElectronIDv5_MVAReader->AddVariable("IP3D",  &bdt_id_IP3D);
  ElectronIDv5_MVAReader->AddVariable("PtRatioV3",    &bdt_id_PtRatioV3);
  ElectronIDv5_MVAReader->AddVariable("PtRelV2",      &bdt_id_PtRelV2);
  ElectronIDv5_MVAReader->AddVariable("JetDiscCJ"    ,&bdt_id_JetDiscCJ);
  ElectronIDv5_MVAReader->AddVariable("JetDiscCJCvsB",&bdt_id_JetDiscCJCvsB);
  ElectronIDv5_MVAReader->AddVariable("JetDiscCJCvsL",&bdt_id_JetDiscCJCvsL);

  ElectronIDv5_MVAReader->AddVariable("MVA",          &bdt_id_MVA);
  ElectronIDv5_MVAReader->AddVariable("MVAIso",       &bdt_id_MVAIso);
  ElectronIDv5_MVAReader->AddVariable("dPhiIn",       &bdt_id_dPhiIn);
  ElectronIDv5_MVAReader->AddVariable("EoverP",       &bdt_id_EoverP);
  ElectronIDv5_MVAReader->AddVariable("FBrem",        &bdt_id_FBrem);
  ElectronIDv5_MVAReader->AddVariable("R9" ,          &bdt_id_R9);
  ElectronIDv5_MVAReader->AddVariable("e55",          &bdt_id_e55);
  ElectronIDv5_MVAReader->AddVariable("PhiWidth",     &bdt_id_PhiWidth);
  ElectronIDv5_MVAReader->AddVariable("EtaWidth",     &bdt_id_EtaWidth);
  ElectronIDv5_MVAReader->AddVariable("MissingHits",  &bdt_id_MissingHits);
  ElectronIDv5_MVAReader->AddVariable("PassConversionVeto",  &bdt_id_PassConversionVeto);
  ElectronIDv5_MVAReader->AddVariable("IsGsfCtfScPixChargeConsistent",  &bdt_id_IsGsfCtfScPixChargeConsistent);
  ElectronIDv5_MVAReader->AddVariable("IsGsfScPixChargeConsistent",  &bdt_id_IsGsfScPixChargeConsistent);
  ElectronIDv5_MVAReader->AddVariable("IsGsfCtfChargeConsistent",  &bdt_id_IsGsfCtfChargeConsistent);
  ElectronIDv5_MVAReader->AddVariable("InvEminusInvP",  &bdt_id_InvEminusInvP);
  ElectronIDv5_MVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);


  /////// Barrel                                                                                                                                                                                                                                                                                                                                                    
  ElectronIDv5_CFMVAReader->AddVariable("PtBinned2",    &bdt_id_PtBinned2);
  ElectronIDv5_CFMVAReader->AddVariable("Eta",          &bdt_id_Eta);
  ElectronIDv5_CFMVAReader->AddVariable("MiniIsoChHad", &bdt_id_MiniIsoChHad);
  ElectronIDv5_CFMVAReader->AddVariable("MiniIsoPhHad", &bdt_id_MiniIsoPhHad);
  ElectronIDv5_CFMVAReader->AddVariable("MiniIsoNHad",  &bdt_id_MiniIsoNHad);
  ElectronIDv5_CFMVAReader->AddVariable("RelMiniIsoCh", &bdt_id_RelMiniIsoCh);
  ElectronIDv5_CFMVAReader->AddVariable("RelMiniIsoN",  &bdt_id_RelMiniIsoN);
  ElectronIDv5_CFMVAReader->AddVariable("RelIso",       &bdt_id_RelIso);
  if(DataYear == 2016){
    ElectronIDv5_CFMVAReader->AddVariable("Dxy",        &bdt_id_Dxy);
    ElectronIDv5_CFMVAReader->AddVariable("Dz",         &bdt_id_Dz);
  }
  else{
    ElectronIDv5_CFMVAReader->AddVariable("DxySig",     &bdt_id_DxySig);
    ElectronIDv5_CFMVAReader->AddVariable("DzSig",     &bdt_id_DzSig);
  }
  ElectronIDv5_CFMVAReader->AddVariable("IP3D",  &bdt_id_IP3D);
  ElectronIDv5_CFMVAReader->AddVariable("PtRatioV3",    &bdt_id_PtRatioV3);
  ElectronIDv5_CFMVAReader->AddVariable("PtRelV2",      &bdt_id_PtRelV2);
  ElectronIDv5_CFMVAReader->AddVariable("JetDiscCJ"    ,&bdt_id_JetDiscCJ);
  ElectronIDv5_CFMVAReader->AddVariable("JetDiscCJCvsB",&bdt_id_JetDiscCJCvsB);
  ElectronIDv5_CFMVAReader->AddVariable("JetDiscCJCvsL",&bdt_id_JetDiscCJCvsL);

  ElectronIDv5_CFMVAReader->AddVariable("MVA",          &bdt_id_MVA);
  ElectronIDv5_CFMVAReader->AddVariable("MVAIso",       &bdt_id_MVAIso);
  ElectronIDv5_CFMVAReader->AddVariable("dPhiIn",       &bdt_id_dPhiIn);
  ElectronIDv5_CFMVAReader->AddVariable("EoverP",       &bdt_id_EoverP);
  ElectronIDv5_CFMVAReader->AddVariable("FBrem",        &bdt_id_FBrem);
  ElectronIDv5_CFMVAReader->AddVariable("R9" ,          &bdt_id_R9);
  ElectronIDv5_CFMVAReader->AddVariable("e55",          &bdt_id_e55);
  ElectronIDv5_CFMVAReader->AddVariable("PhiWidth",     &bdt_id_PhiWidth);
  ElectronIDv5_CFMVAReader->AddVariable("EtaWidth",     &bdt_id_EtaWidth);
  ElectronIDv5_CFMVAReader->AddVariable("MissingHits",  &bdt_id_MissingHits);
  ElectronIDv5_CFMVAReader->AddVariable("PassConversionVeto",  &bdt_id_PassConversionVeto);
  ElectronIDv5_CFMVAReader->AddVariable("IsGsfCtfScPixChargeConsistent",  &bdt_id_IsGsfCtfScPixChargeConsistent);
  ElectronIDv5_CFMVAReader->AddVariable("IsGsfScPixChargeConsistent",  &bdt_id_IsGsfScPixChargeConsistent);
  ElectronIDv5_CFMVAReader->AddVariable("IsGsfCtfChargeConsistent",  &bdt_id_IsGsfCtfChargeConsistent);
  ElectronIDv5_CFMVAReader->AddVariable("InvEminusInvP",  &bdt_id_InvEminusInvP);
  ElectronIDv5_CFMVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);


  ////////// FAKE MVA READER HF vs LF

  //////// IP Reader                                                                                                                                                    
  ElectronIDv5_FakeMVAReader->AddVariable("Eta",          &bdt_id_Eta);
  ElectronIDv5_FakeMVAReader->AddVariable("MiniIsoChHad", &bdt_id_MiniIsoChHad);
  ElectronIDv5_FakeMVAReader->AddVariable("MiniIsoPhHad", &bdt_id_MiniIsoPhHad);
  ElectronIDv5_FakeMVAReader->AddVariable("MiniIsoNHad",  &bdt_id_MiniIsoNHad);
  ElectronIDv5_FakeMVAReader->AddVariable("RelMiniIsoCh", &bdt_id_RelMiniIsoCh);
  ElectronIDv5_FakeMVAReader->AddVariable("RelMiniIsoN",  &bdt_id_RelMiniIsoN);
  ElectronIDv5_FakeMVAReader->AddVariable("RelIso",       &bdt_id_RelIso);
  if(DataYear == 2016){
    ElectronIDv5_FakeMVAReader->AddVariable("Dxy",        &bdt_id_Dxy);
    ElectronIDv5_FakeMVAReader->AddVariable("Dz",         &bdt_id_Dz);
  }
  else{
    ElectronIDv5_FakeMVAReader->AddVariable("DxySig",     &bdt_id_DxySig);
    ElectronIDv5_FakeMVAReader->AddVariable("DzSig",  &bdt_id_DzSig);
  }
  ElectronIDv5_FakeMVAReader->AddVariable("IP3D",  &bdt_id_IP3D);
  ElectronIDv5_FakeMVAReader->AddVariable("PtRatioV3",    &bdt_id_PtRatioV3);
  ElectronIDv5_FakeMVAReader->AddVariable("PtRelV2",      &bdt_id_PtRelV2);
  ElectronIDv5_FakeMVAReader->AddVariable("JetDiscCJ"    ,&bdt_id_JetDiscCJ);
  ElectronIDv5_FakeMVAReader->AddVariable("JetDiscCJCvsB",&bdt_id_JetDiscCJCvsB);
  ElectronIDv5_FakeMVAReader->AddVariable("JetDiscCJCvsL",&bdt_id_JetDiscCJCvsL);
  ElectronIDv5_FakeMVAReader->AddVariable("MVA",          &bdt_id_MVA);
  ElectronIDv5_FakeMVAReader->AddVariable("MVAIso",       &bdt_id_MVAIso);
  ElectronIDv5_FakeMVAReader->AddVariable("dPhiIn",       &bdt_id_dPhiIn);
  ElectronIDv5_FakeMVAReader->AddVariable("EoverP",       &bdt_id_EoverP);
  ElectronIDv5_FakeMVAReader->AddVariable("FBrem",        &bdt_id_FBrem);
  ElectronIDv5_FakeMVAReader->AddVariable("R9" ,          &bdt_id_R9);
  ElectronIDv5_FakeMVAReader->AddVariable("CEMFracCJ",&bdt_id_CEMFracCJ);
  ElectronIDv5_FakeMVAReader->AddVariable("NEMFracCJ",&bdt_id_NEMFracCJ);
  ElectronIDv5_FakeMVAReader->AddVariable("CHFracCJ",&bdt_id_CHFracCJ);
  ElectronIDv5_FakeMVAReader->AddVariable("NHFracCJ",&bdt_id_NHFracCJ);
  ElectronIDv5_FakeMVAReader->AddVariable("e55",          &bdt_id_e55);
  ElectronIDv5_FakeMVAReader->AddVariable("PhiWidth",     &bdt_id_PhiWidth);
  ElectronIDv5_FakeMVAReader->AddVariable("EtaWidth",     &bdt_id_EtaWidth);
  ElectronIDv5_FakeMVAReader->AddVariable("MissingHits",  &bdt_id_MissingHits);
  ElectronIDv5_FakeMVAReader->AddVariable("PassConversionVeto",  &bdt_id_PassConversionVeto);
  ElectronIDv5_FakeMVAReader->AddVariable("IsGsfCtfScPixChargeConsistent",  &bdt_id_IsGsfCtfScPixChargeConsistent);
  ElectronIDv5_FakeMVAReader->AddVariable("IsGsfScPixChargeConsistent",  &bdt_id_IsGsfScPixChargeConsistent);
  ElectronIDv5_FakeMVAReader->AddVariable("IsGsfCtfChargeConsistent",  &bdt_id_IsGsfCtfChargeConsistent);
  ElectronIDv5_FakeMVAReader->AddVariable("InvEminusInvP",  &bdt_id_InvEminusInvP);
  ElectronIDv5_FakeMVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);


  TString MVAPathMulti ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIMulti/Version11/"   +GetYearString()+"/dataset/weights/";
  TString MVAPathMultiV10 ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIMulti/Version10/"   +GetYearString()+"/dataset/weights/";
  
  TString MVAPathFake ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIElectronFake/Version12/"        +GetYearString()+"/dataset/weights/";
  TString xmlpf = "_TMVAClassification_BDTG.weights.xml";


  if(DataYear == 2016){
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_LF_Fake",   MVAPathMulti+ "BDTG_version11_FakeBkg_LF_TypeI_EE_SignalFakeBkg_LF_IP_Inc_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);   
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_HFB_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_HFB_TypeI_EE_SignalFakeBkg_HFB_IP_Inc_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);   
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_HFC_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_HFC_TypeI_EE_SignalFakeBkg_HFC_IP_Inc_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);   
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_Fake",   MVAPathMulti+ "BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_Inc_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);   
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_Fake",   MVAPathMulti+ "BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);   
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_Conv",   MVAPathMulti+ "BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_Inc_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_Conv",   MVAPathMulti+ "BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_CF",     MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_Inc_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_CF",     MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_EC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_CFMVAReader->BookMVA("BDTGv5Pt_BB_CF",   MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_CFMVAReader->BookMVA("BDTGv5Pt_EC_CF",   MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);

    //----> 0.991062884939: ['0.149', '0.185', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.990798401177: ['0.181', '0.673', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_Inc_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.97059833623:  ['0.661', '0.15',  'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.97117516843:  ['0.568', '0.126', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_EC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT.root']
    //----> 0.957969456743: ['0.668', '0.106', 'BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_Inc_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT.root']
    //----> 0.931905703359: ['0.4',   '0.149', 'BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_200_BDT.root']
    //----> 0.976175150802: ['0.116', '0.164', 'BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_Inc_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.987073656244: ['0.144', '0.126', 'BDTG_version11_FakeBkg_HFB_TypeI_EE_SignalFakeBkg_HFB_IP_Inc_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.97930107624:  ['0.349', '0.133', 'BDTG_version11_FakeBkg_HFC_TypeI_EE_SignalFakeBkg_HFC_IP_Inc_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.979841755577: ['0.125', '0.128', 'BDTG_version11_FakeBkg_LF_TypeI_EE_SignalFakeBkg_LF_IP_Inc_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.959983970767: ['0.327', '0.105', 'BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']

    //// Falvour Fake
    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LFvsHF_Fake",   MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_LFvsHF_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);  
    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_HFBvsHFC_Fake",         MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_HFBvsHFC_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);  
    
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_LFvsHF_Fake", 
          MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFvsHF_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+ xmlpf);  
    
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_HFBvsHFC_Fake",
          MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_HFBvsHFC_2016_NTrees700_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);  
    

    //----> 0.820222557836: ['0.45', '0.108', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_HFBvsHFC_2016_NTrees700_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.845329887104: ['0.508', '0.114', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_HFBvsHFC_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT.root']
    ///----> 0.940261336547: ['0.108', '0.844', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_LFvsHF_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.956875330221: ['0.107', '0.468', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFvsHF_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT.root']

    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother1_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_1_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_600_BDT"+ xmlpf);
    
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother2_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_400_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF2_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother3_2016_NTrees300_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);


  }  
  
  if(DataYear == 2017){

    ElectronIDv5_MVAReader->BookMVA("BDTGv5_LF_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_LF_TypeI_EE_SignalFakeBkg_LF_IP_Inc_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_HFB_Fake", MVAPathMulti+ "BDTG_version11_FakeBkg_HFB_TypeI_EE_SignalFakeBkg_HFB_IP_Inc_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_HFC_Fake", MVAPathMulti+ "BDTG_version11_FakeBkg_HFC_TypeI_EE_SignalFakeBkg_HFC_IP_Inc_2017_NTrees300_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_Inc_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_Conv",  MVAPathMultiV10+ "BDTG_version10_ConvBkg_TypeI_EE_SignalConvBkg_IP_Inc_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_EC_2017_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_Conv",  MVAPathMulti+ "BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_EC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_CF",    MVAPathMultiV10+ "BDTG_version10_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_CF",    MVAPathMultiV10+ "BDTG_version10_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_CFMVAReader->BookMVA("BDTGv5Pt_BB_CF",   MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2017_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_CFMVAReader->BookMVA("BDTGv5Pt_EC_CF",   MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2017_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);

    //----> 0.990828228762: ['0.128', '0.387', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2017_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.978197862817: ['0.601', '0.116', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2017_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.991111776595: ['0.137', '0.305', 'BDTG_version10_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.978149831063: ['0.715', '0.139', 'BDTG_version10_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.95638347114: ['0.478', '0.107', 'BDTG_version10_ConvBkg_TypeI_EE_SignalConvBkg_IP_Inc_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.936974932769: ['0.808', '0.106', 'BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_EC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.978882992815: ['0.145', '0.188', 'BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_Inc_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.983114923384: ['0.225', '0.103', 'BDTG_version11_FakeBkg_LF_TypeI_EE_SignalFakeBkg_LF_IP_Inc_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.988083459142: ['0.222', '0.182', 'BDTG_version11_FakeBkg_HFB_TypeI_EE_SignalFakeBkg_HFB_IP_Inc_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.967189103974: ['0.566', '0.145', 'BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_EC_2017_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']

    //// Reader for LF/HF tagging
    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LFvsHF_Fake",         MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_LFvsHF_2017_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_HFBvsHFC_Fake",       MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_HFBvsHFC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_LFvsHF_Fake",      MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFvsHF_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_HFBvsHFC_Fake",    MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_HFBvsHFC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    
    //----> 0.832200821965: ['0.316', '0.162', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_HFBvsHFC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.860209742143: ['0.115', '0.542', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_HFBvsHFC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_2_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.946179015603: ['0.114', '0.693', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_LFvsHF_2017_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.954029739705: ['0.884', '0.139', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFvsHF_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']

    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother1_2017_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother2_2017_NTrees300_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_300_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF2_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother3_2017_NTrees300_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);

  }
  

  if(DataYear == 2018){
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_LF_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_LF_TypeI_EE_SignalFakeBkg_LF_IP_Inc_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_HFB_Fake", MVAPathMulti+ "BDTG_version11_FakeBkg_HFB_TypeI_EE_SignalFakeBkg_HFB_IP_Inc_2018_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_HFC_Fake", MVAPathMulti+ "BDTG_version11_FakeBkg_HFC_TypeI_EE_SignalFakeBkg_HFC_IP_Inc_2018_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_Inc_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_Conv",  MVAPathMulti+ "BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_Inc_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_Fake",  MVAPathMulti+ "BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_Conv",  MVAPathMulti+ "BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_EC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_0.1_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_BB_CF",    MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_Inc_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_MVAReader->BookMVA("BDTGv5_EC_CF",    MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_EC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_CFMVAReader->BookMVA("BDTGv5Pt_BB_CF",   MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_CFMVAReader->BookMVA("BDTGv5Pt_EC_CF",   MVAPathMulti+ "BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+ xmlpf);


    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LFvsHF_Fake",         MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_LFvsHF_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_HFBvsHFC_Fake",       MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_HFBvsHFC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_LFvsHF_Fake",      MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFvsHF_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_QCD_HFBvsHFC_Fake",    MVAPathFake+ "BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_HFBvsHFC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);


    //----> 0.828152956177: ['0.107', '0.117', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_HFBvsHFC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.862270057625: ['0.876', '0.158', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_HFBvsHFC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.946927590863: ['0.152', '0.143', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_LFvsHF_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.960522118948: ['0.183', '0.118', 'BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFvsHF_2018_NTrees700_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']



    //----> 0.983071530511: ['0.41', '0.138', 'BDTG_version11_FakeBkg_LF_TypeI_EE_SignalFakeBkg_LF_IP_Inc_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.988276867484: ['0.332', '0.196', 'BDTG_version11_FakeBkg_HFB_TypeI_EE_SignalFakeBkg_HFB_IP_Inc_2018_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_200_Shrinkage_0.1_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.982666976387: ['0.333', '0.102', 'BDTG_version11_FakeBkg_HFC_TypeI_EE_SignalFakeBkg_HFC_IP_Inc_2018_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_3_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.979829589516: ['0.873', '0.1', 'BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_Inc_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.969013599405: ['0.339', '0.118', 'BDTG_version11_FakeBkg_TypeI_EE_SignalFakeBkg_IP_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.95722221247: ['0.11', '0.101', 'BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_Inc_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_4_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT.root']
    //----> 0.938911043985: ['0.117', '0.108', 'BDTG_version11_ConvBkg_TypeI_EE_SignalConvBkg_IP_EC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_0.1_MaxDepth_3_nCuts_200_Shrinkage_0.1_BaggedFrac_0.6_Seed_100_BDT.root']
    //----> 0.991307165103: ['0.12', '0.243', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_Inc_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.978299413465: ['0.54', '0.104', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_IP_EC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.990921894748: ['0.101', '0.78', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_Inc_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']
    //----> 0.978420600256: ['0.406', '0.11', 'BDTG_version11_CFBkg_TypeI_EE_SignalCFBkg_CFPt_EC_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_0.5_MaxDepth_5_nCuts_200_Shrinkage_0.05_BaggedFrac_0.6_Seed_200_BDT.root']


    //ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother1_2018_NTrees300_NormMode_EqualNumEvents_MinNodeSize_1.0_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF1_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother2_2018_NTrees300_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);
    ElectronIDv5_FakeMVAReader->BookMVA("BDTGv5_LF2_Fake",MVAPathFake +"BDTG_version12_FakeBkg_TypeI_EE_SignalElectronFake_IP_QCD_LFMother3_2018_NTrees300_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_200_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+ xmlpf);

  }
  

  return;

}


void AnalyzerCore::SetupIDMVAReaderElectron(){
  
  if(!( iSetupLeptonBDTv4)) return;
  
  /*                     
       
       electron_v1:                                                                                                                                                                                                  
       - sets up for legacy purposes the first of the BDT ID. This was in Dec 2022. This tuned out buggy since 3 variables were set as ints/floats incorrectly.                                                      
       This also had more variables in the training.                                                                                                                                                                 
       
       electron_v2plus:                                                                                                                                                                                              
       - This is all versions after and including version2.                                                                                                                                                          
       - These all share the training variables, hense why they can be grouped together and use the same Reader class variable.                                                                                      
       -  Version 2 of the training had issues in regards to ptbinned variable being set wrong and so was scrpped                                                                                                    
       -  Version 3 had issues in miniiso and so Fakes were scrapped                                                                                                                                                 
       -  Version 4 saw poor training in LF and so the LF was retrained removiong pi+                                                                                                                                
       
  */
  

  InitializeIDTreeVars();
  
  ElectronIDv4_FakeMVAReader->AddVariable("Pt", &bdt_id_Pt);
  ElectronIDv4_FakeMVAReader->AddVariable("Eta", &bdt_id_Eta);
  ElectronIDv4_FakeMVAReader->AddVariable("MiniIsoChHad", &bdt_id_MiniIsoChHad);
  ElectronIDv4_FakeMVAReader->AddVariable("MiniIsoPhHad", &bdt_id_MiniIsoPhHad);
  ElectronIDv4_FakeMVAReader->AddVariable("MiniIsoNHad", &bdt_id_MiniIsoNHad);
  ElectronIDv4_FakeMVAReader->AddVariable("RelMiniIsoCh", &bdt_id_RelMiniIsoCh);
  ElectronIDv4_FakeMVAReader->AddVariable("RelMiniIsoN", &bdt_id_RelMiniIsoN);
  ElectronIDv4_FakeMVAReader->AddVariable("Dxy",  &bdt_id_Dxy);
  ElectronIDv4_FakeMVAReader->AddVariable("Dz",  &bdt_id_Dz);
  ElectronIDv4_FakeMVAReader->AddVariable("RelIso", &bdt_id_RelIso);
  ElectronIDv4_FakeMVAReader->AddVariable("IP3D", &bdt_id_IP3D);
  ElectronIDv4_FakeMVAReader->AddVariable("PtRatio",  &bdt_id_PtRatio);
  ElectronIDv4_FakeMVAReader->AddVariable("PtRel",  &bdt_id_PtRel);
  ElectronIDv4_FakeMVAReader->AddVariable("NEMFracCJ",&bdt_id_NEMFracCJ);
  ElectronIDv4_FakeMVAReader->AddVariable("CHFracCJ",&bdt_id_CHFracCJ);
  ElectronIDv4_FakeMVAReader->AddVariable("JetDiscCJ",&bdt_id_JetDiscCJ);
  ElectronIDv4_FakeMVAReader->AddVariable("NHFracCJ",&bdt_id_NHFracCJ);
  ElectronIDv4_FakeMVAReader->AddVariable("CEMFracCJ",&bdt_id_CEMFracCJ);
  ElectronIDv4_FakeMVAReader->AddVariable("MVA",  &bdt_id_MVA);
  ElectronIDv4_FakeMVAReader->AddVariable("MVAIso",  &bdt_id_MVAIso);
  ElectronIDv4_FakeMVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);
  
  ElectronIDv4_CFMVAReader->AddVariable("PtBinned", &bdt_id_PtBinned);
  ElectronIDv4_CFMVAReader->AddVariable("Eta", &bdt_id_Eta);
  ElectronIDv4_CFMVAReader->AddVariable("Dxy",  &bdt_id_Dxy);
  ElectronIDv4_CFMVAReader->AddVariable("DxySig",  &bdt_id_DxySig);
  ElectronIDv4_CFMVAReader->AddVariable("Dz",  &bdt_id_Dz);
  ElectronIDv4_CFMVAReader->AddVariable("DzSig",  &bdt_id_DzSig);
  ElectronIDv4_CFMVAReader->AddVariable("IP3D", &bdt_id_IP3D);
  ElectronIDv4_CFMVAReader->AddVariable("MVA",  &bdt_id_MVA);
  ElectronIDv4_CFMVAReader->AddVariable("MVAIso",  &bdt_id_MVAIso);
  ElectronIDv4_CFMVAReader->AddVariable("Full5x5_sigmaIetaIeta",  &bdt_id_Full5x5_sigmaIetaIeta);
  ElectronIDv4_CFMVAReader->AddVariable("dPhiIn",  &bdt_id_dPhiIn);
  ElectronIDv4_CFMVAReader->AddVariable("EoverP",  &bdt_id_EoverP);
  ElectronIDv4_CFMVAReader->AddVariable("FBrem",  &bdt_id_FBrem);
  ElectronIDv4_CFMVAReader->AddVariable("R9",  &bdt_id_R9);
  ElectronIDv4_CFMVAReader->AddVariable("e55",  &bdt_id_e55);
  ElectronIDv4_CFMVAReader->AddVariable("EtaWidth",  &bdt_id_EtaWidth);
  ElectronIDv4_CFMVAReader->AddVariable("PhiWidth",  &bdt_id_PhiWidth);
  ElectronIDv4_CFMVAReader->AddVariable("PassConversionVeto",  &bdt_id_PassConversionVeto);
  ElectronIDv4_CFMVAReader->AddVariable("IsGsfCtfScPixChargeConsistent",  &bdt_id_IsGsfCtfScPixChargeConsistent);
  ElectronIDv4_CFMVAReader->AddVariable("IsGsfScPixChargeConsistent",  &bdt_id_IsGsfScPixChargeConsistent);
  ElectronIDv4_CFMVAReader->AddVariable("IsGsfCtfChargeConsistent",  &bdt_id_IsGsfCtfChargeConsistent);
  ElectronIDv4_CFMVAReader->AddVariable("InvEminusInvP", &bdt_id_InvEminusInvP);
  ElectronIDv4_CFMVAReader->AddVariable("hcalPFClusterIso",  &bdt_id_hcalPFClusterIso);
  ElectronIDv4_CFMVAReader->AddVariable("ecalPFClusterIso",  &bdt_id_ecalPFClusterIso);
  ElectronIDv4_CFMVAReader->AddVariable("dr03TkSumPt",  &bdt_id_dr03TkSumPt);
  ElectronIDv4_CFMVAReader->AddVariable("HoverE",  &bdt_id_HoverE);
  ElectronIDv4_CFMVAReader->AddVariable("MissingHits",  &bdt_id_MissingHits);
  ElectronIDv4_CFMVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);
    
  ElectronIDv4_ConvMVAReader->AddVariable("Pt", &bdt_id_Pt);
  ElectronIDv4_ConvMVAReader->AddVariable("Eta", &bdt_id_Eta);
  ElectronIDv4_ConvMVAReader->AddVariable("MiniIsoChHad", &bdt_id_MiniIsoChHad);
  ElectronIDv4_ConvMVAReader->AddVariable("RelMiniIsoCh", &bdt_id_RelMiniIsoCh);
  ElectronIDv4_ConvMVAReader->AddVariable("IsoChHad", &bdt_id_IsoChHad);
  ElectronIDv4_ConvMVAReader->AddVariable("IsoNHad", &bdt_id_IsoNHad);
  ElectronIDv4_ConvMVAReader->AddVariable("IsoPhHad", &bdt_id_IsoPhHad);
  ElectronIDv4_ConvMVAReader->AddVariable("Dxy",  &bdt_id_Dxy);
  ElectronIDv4_ConvMVAReader->AddVariable("DxySig",  &bdt_id_DxySig);
  ElectronIDv4_ConvMVAReader->AddVariable("Dz",  &bdt_id_Dz);
  ElectronIDv4_ConvMVAReader->AddVariable("DzSig",  &bdt_id_DzSig);
  ElectronIDv4_ConvMVAReader->AddVariable("RelIso", &bdt_id_RelIso);
  ElectronIDv4_ConvMVAReader->AddVariable("IP3D", &bdt_id_IP3D);
  ElectronIDv4_ConvMVAReader->AddVariable("PtRatio",  &bdt_id_PtRatio);
  ElectronIDv4_ConvMVAReader->AddVariable("PtRel",  &bdt_id_PtRel);
  ElectronIDv4_ConvMVAReader->AddVariable("CEMFracCJ",&bdt_id_CEMFracCJ);
  ElectronIDv4_ConvMVAReader->AddVariable("NEMFracCJ",&bdt_id_NEMFracCJ);
  ElectronIDv4_ConvMVAReader->AddVariable("CHFracCJ",&bdt_id_CHFracCJ);
  ElectronIDv4_ConvMVAReader->AddVariable("JetDiscCJ",&bdt_id_JetDiscCJ);
  ElectronIDv4_ConvMVAReader->AddVariable("NHFracCJ",&bdt_id_NHFracCJ);
  ElectronIDv4_ConvMVAReader->AddVariable("MVA",  &bdt_id_MVA);
  ElectronIDv4_ConvMVAReader->AddVariable("MVAIso",  &bdt_id_MVAIso);
  ElectronIDv4_ConvMVAReader->AddVariable("Full5x5_sigmaIetaIeta",  &bdt_id_Full5x5_sigmaIetaIeta);
  ElectronIDv4_ConvMVAReader->AddVariable("dEtaSeed",  &bdt_id_dEtaSeed);
  ElectronIDv4_ConvMVAReader->AddVariable("dPhiIn",  &bdt_id_dPhiIn);
  ElectronIDv4_ConvMVAReader->AddVariable("EoverP",  &bdt_id_EoverP);
  ElectronIDv4_ConvMVAReader->AddVariable("FBrem",  &bdt_id_FBrem);
  ElectronIDv4_ConvMVAReader->AddVariable("R9",  &bdt_id_R9);
  ElectronIDv4_ConvMVAReader->AddVariable("TrkIso",  &bdt_id_TrkIso);
  ElectronIDv4_ConvMVAReader->AddVariable("EtaWidth",  &bdt_id_EtaWidth);
  ElectronIDv4_ConvMVAReader->AddVariable("PhiWidth",  &bdt_id_PhiWidth);
  ElectronIDv4_ConvMVAReader->AddVariable("InvEminusInvP", &bdt_id_InvEminusInvP);
  ElectronIDv4_ConvMVAReader->AddVariable("ecalPFClusterIso",  &bdt_id_ecalPFClusterIso);
  ElectronIDv4_ConvMVAReader->AddVariable("MissingHits",  &bdt_id_MissingHits);
  ElectronIDv4_ConvMVAReader->AddVariable("PassConversionVeto",  &bdt_id_PassConversionVeto);
  ElectronIDv4_ConvMVAReader->AddVariable("IsGsfCtfScPixChargeConsistent",  &bdt_id_IsGsfCtfScPixChargeConsistent);
  ElectronIDv4_ConvMVAReader->AddVariable("IsGsfScPixChargeConsistent",  &bdt_id_IsGsfScPixChargeConsistent);
  ElectronIDv4_ConvMVAReader->AddVariable("IsGsfCtfChargeConsistent",  &bdt_id_IsGsfCtfChargeConsistent);
  ElectronIDv4_ConvMVAReader->AddSpectator("w_id_tot", &bdt_id_w_id_tot);
  
  TString AnalyzerPath=std::getenv("SKFlat_WD");
  /// Version 2 paths
  TString MVAPathConvv2 ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIConv/Version3/"        +GetYearString()+"/dataset/weights/";
  TString MVAPathCFv2   ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeICF/Version3/"          +GetYearString()+"/dataset/weights/";

  /// Other 

  TString MVAPathFakev3 ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIElectronFake/Version4/"+GetYearString()+"/dataset/weights/";

  TString MVAPathFakev4 ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIElectronFake/Version5/"+GetYearString()+"/dataset/weights/";
  TString MVAPathFakev5a ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIElectronFake/Version6/"+GetYearString()+"/dataset/weights/";
  TString MVAPathFakev5b ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIElectronFakeHF/Version6/"+GetYearString()+"/dataset/weights/";

  TString MVAPathConvv3 ="/data6/Users/jalmond/BDTOutput/Run2UltraLegacy_v3/runIDBDT_HNtypeIConv/Version6/"        +GetYearString()+"/dataset/weights/";
  TString xmlpf = "_TMVAClassification_BDTG.weights.xml";
  
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  ///
  ///    VERSION 1 ELECTRON MVA (Dec 22)
  ///
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  /// Version1 : Original training with some minor bugs
  /// Version2 : new training with bug fix and some variables changed + ptbinned
  /// Version3 : is new with bug fix plus All variables checked,
  /// Version4 : fixed rho bug ub version3
  /// Version5 : is same as 4 but with pi+ fakes suppressed
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////
  ///                                                                             
  ///    VERSION 3 ELECTRON MVA (Feb 20)                                          
  ///                                                                             
  /////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////         
  
    
  cout << "Lepton BDT ID details: Training done for CF/Conversion/Fake backgrounds." << endl;

  if(GetYear() == 2016) {
    cout << "Setting up 2016 CF READER: ElectronIDv4_CFMVAReader/ElectronIDv4_CFMVAReaderPt/ElectronIDv3CFMVAReader" <<endl;
    
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_BB_CF",     MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT" +xmlpf);
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_EC_CF",     MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT" +xmlpf);
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_CF",        MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT" +xmlpf);

  }
  if(GetYear() == 2017) {
    cout << "ElectronIDv4_CFMVAReader 2017 ElectronIDv4_CFMVAReader/ElectronIDv4_CFMVAReaderPt/ElectronIDv3CFMVAReader" << endl;
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_BB_CF",     MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT" +xmlpf);
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_EC_CF",     MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT" +xmlpf);
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_CF",        MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT" +xmlpf);
    
  }
  
  if(GetYear() == 2018) {
    cout << "ElectronIDv4_CFMVAReader 2018 ElectronIDv4_CFMVAReader/ElectronIDv4_CFMVAReaderPt/ElectronIDv3CFMVAReader" << endl;
    
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_BB_CF",     MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_EC_CF",     MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT" +xmlpf);
    ElectronIDv4_CFMVAReader->BookMVA("BDTGv4_CF",        MVAPathCFv2+ "BDTG_version3_CF_TypeI_EE_SignalCFPtBinned_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT" +xmlpf);
  }
  
  /// FAKE ID BDT
  
  if(GetYear() == 2016) {
    
    ////// Version 5 training--> v4 variable                                                                                                                                                                                                                                
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Fake"     ,MVAPathFakev4+"BDTG_version5_FakeBkg_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_Fake" ,MVAPathFakev4+"BDTG_version5_FakeBkgTop_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_Fake"  ,MVAPathFakev4+"BDTG_version5_FakeBkg_LF_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf); 
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_Fake"  ,MVAPathFakev3+"BDTG_version4_FakeBkg_HF_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_Fake" ,MVAPathFakev3+"BDTG_version4_FakeBkg_HFB_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_Fake" ,MVAPathFakev3+"BDTG_version4_FakeBkg_HFC_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    // 0.971327007236: ['0.202', '0.815', 'BDTG_version5_FakeBkg_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    // 0.976957626476: ['0.372', '0.332', 'BDTG_version5_FakeBkgTop_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    // 0.975798366843: ['0.504', '0.11',  'BDTG_version5_FakeBkg_LF_TypeI_EE_SignalElectronFake_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
   
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_BB_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_BB_Fake" ,MVAPathFakev5a+"BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_BB_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_BB_Fake"  ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_BB_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_BB_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_BB_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);

    //----> 0.975949420253: ['0.446', '0.205', 'BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.979959343427: ['0.73', '0.103', 'BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.979043755334: ['0.33', '0.191', 'BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.985552179145: ['0.848', '0.159', 'BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.98730828959: ['0.751', '0.405', 'BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_BB_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.976667335878: ['0.683', '0.162', 'BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_BB_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']


    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_EC_Fake"  ,   MVAPathFakev5a+"BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_EC_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_EC_Fake" ,MVAPathFakev5a+"BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_EC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_EC_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_EC_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_EC_Fake"  ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_EC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_EC_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_EC_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_EC_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+xmlpf);
    //----> 0.95022859816: ['0.256', '0.136', 'BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_EC_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT.root']
    //----> 0.961784246695: ['0.134', '0.15', 'BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_EC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.956937361582: ['0.173', '0.303', 'BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_EC_2016_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT.root']
    // ----> 0.971113356591: ['0.705', '0.241', 'BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_EC_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    // ----> 0.974627102644: ['0.54', '0.168', 'BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.960022052178: ['0.491', '0.155', 'BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_EC_2016_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT.root']

  }
  if(GetYear() == 2017) {
    
    ////// Version 5 training--> v4 variable (HF same as v3/version4)
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Fake",    MVAPathFakev4+"BDTG_version5_FakeBkg_TypeI_EE_SignalElectronFake_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_Fake",MVAPathFakev4+"BDTG_version5_FakeBkgTop_TypeI_EE_SignalElectronFake_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_Fake", MVAPathFakev4+"BDTG_version5_FakeBkg_LF_TypeI_EE_SignalElectronFake_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_Fake", MVAPathFakev3+"BDTG_version4_FakeBkg_HF_TypeI_EE_SignalElectronFake_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+xmlpf); 
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_Fake",MVAPathFakev3+"BDTG_version4_FakeBkg_HFB_TypeI_EE_SignalElectronFake_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_Fake",MVAPathFakev3+"BDTG_version4_FakeBkg_HFC_TypeI_EE_SignalElectronFake_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_200_BDT"+xmlpf);

    //  0.975586771441: ['0.223', '0.123', 'BDTG_version5_FakeBkg_TypeI_EE_SignalElectronFake_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //  0.979394122213: ['0.577', '0.367', 'BDTG_version5_FakeBkgTop_TypeI_EE_SignalElectronFake_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //  0.980371549497: ['0.164', '0.111', 'BDTG_version5_FakeBkg_LF_TypeI_EE_SignalElectronFake_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_BB_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_BB_Fake" ,MVAPathFakev5a+"BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_BB_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_BB_Fake"  ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_BB_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_BB_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);

    //----> 0.978938844852: ['0.751', '0.729', 'BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.981902876321: ['0.629', '0.165', 'BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.981820420981: ['0.563', '0.275', 'BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.987001382377: ['0.267', '0.196', 'BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    // ----> 0.988386810659: ['0.388', '0.232', 'BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.979082329986: ['0.667', '0.227', 'BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_BB_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']

    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_EC_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_EC_Fake" ,MVAPathFakev5a+"BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_EC_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_EC_Fake"  ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_EC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_EC_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_EC_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_EC_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);

    //----> 0.962165822548: ['0.248', '0.176', 'BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.968933049054: ['0.231', '0.169', 'BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.970866620109: ['0.639', '0.264', 'BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.974173905908: ['0.476', '0.111', 'BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_EC_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.97665037703: ['0.778', '0.113', 'BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.969668869156: ['0.508', '0.231', 'BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_EC_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']


  }
  if(GetYear() == 2018) {

    ////// Version 5 training--> v4 variable                                                                                                                                                                                                                                 
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Fake",MVAPathFakev4+"BDTG_version5_FakeBkg_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_Fake",MVAPathFakev4+"BDTG_version5_FakeBkgTop_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);  
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_Fake",MVAPathFakev4+"BDTG_version5_FakeBkg_LF_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_Fake",MVAPathFakev3+"BDTG_version4_FakeBkg_HF_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_Fake",MVAPathFakev3+"BDTG_version4_FakeBkg_HFB_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_Fake",MVAPathFakev3+"BDTG_version4_FakeBkg_HFC_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);

    //  0.976461138531: ['0.125', '0.139', 'BDTG_version5_FakeBkg_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //  0.979605314702: ['0.254', '0.209', 'BDTG_version5_FakeBkgTop_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //  0.980518631458: ['0.339', '0.15',  'BDTG_version5_FakeBkg_LF_TypeI_EE_SignalElectronFake_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']


    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_BB_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_BB_Fake" ,MVAPathFakev5a+"BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_BB_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_BB_Fake"  ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_BB_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_BB_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_BB_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);

    //----> 0.979606996988: ['0.779', '0.551', 'BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.982087022796: ['0.741', '0.236', 'BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.982137700661: ['0.22', '0.444', 'BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.987488943063: ['0.597', '0.445', 'BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.988714985006: ['0.582', '0.123', 'BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_BB_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.979900644334: ['0.595', '0.163', 'BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_BB_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']

    
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_EC_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_Top_EC_Fake" ,MVAPathFakev5a+"BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_LF_EC_Fake"  ,MVAPathFakev5a+"BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HF_EC_Fake"  ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFB_EC_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_EC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
    ElectronIDv4_FakeMVAReader->BookMVA("BDTGv4_HFC_EC_Fake" ,MVAPathFakev5b+"BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_EC_2018_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT"+xmlpf);//
  
    //----> 0.963927708661: ['0.133', '0.318', 'BDTG_version6_FakeBkg_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.969841629299: ['0.136', '0.281', 'BDTG_version6_FakeBkgTop_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
    //----> 0.971429977072: ['0.137', '0.46',  'BDTG_version6_FakeBkg_LF_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.97553488121: ['0.145', '0.593', 'BDTG_version6_FakeBkg_HF_TypeI_EE_SignalElectronFake_EC_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_3_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.977575778164: ['0.132', '0.569', 'BDTG_version6_FakeBkg_HFB_TypeI_EE_SignalElectronFake_EC_2018_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT.root']
    //----> 0.969351834758: ['0.714', '0.131', 'BDTG_version6_FakeBkg_HFC_TypeI_EE_SignalElectronFake_EC_2018_NTrees1500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_200_BDT.root']

  }
  
  /// CONV ID BDT                                                                                                                                           
  if(GetYear() == 2016)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_Conv",MVAPathConvv2+"BDTG_version3_Conv_TypeI_EE_SignalConv_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
  if(GetYear() == 2017)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_Conv",MVAPathConvv2+"BDTG_version3_Conv_TypeI_EE_SignalConv_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
  if(GetYear() == 2018)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_Conv",MVAPathConvv2+"BDTG_version3_Conv_TypeI_EE_SignalConv_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
  // AUC              KS_signal   KS_bkg      File 

  // 0.964866659147: ['0.503', '0.127', 'BDTG_version3_Conv_TypeI_EE_SignalConv_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
  // 0.96530088056:  ['0.504', '0.107', 'BDTG_version3_Conv_TypeI_EE_SignalConv_2017_NTrees500_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT.root']
  // 0.966752119434: ['0.162', '0.214', 'BDTG_version3_Conv_TypeI_EE_SignalConv_2018_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']


  if(GetYear() == 2016)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_BB_Conv",MVAPathConvv3+"BDTG_version6_Conv_TypeI_EE_SignalConv_BB_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
  if(GetYear() == 2017)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_BB_Conv",MVAPathConvv3+"BDTG_version6_Conv_TypeI_EE_SignalConv_BB_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);
  if(GetYear() == 2018)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_BB_Conv",MVAPathConvv3+"BDTG_version6_Conv_TypeI_EE_SignalConv_BB_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT"+xmlpf);

  
  //0.967854971214: ['0.275', '0.15', 'BDTG_version6_Conv_TypeI_EE_SignalConv_BB_2016_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
  //0.966308164128: ['0.507', '0.14', 'BDTG_version6_Conv_TypeI_EE_SignalConv_BB_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']
  //0.961272507148: ['0.373', '0.118', 'BDTG_version6_Conv_TypeI_EE_SignalConv_BB_2018_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.5_Seed_100_BDT.root']

  if(GetYear() == 2016)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_EC_Conv",MVAPathConvv3+"BDTG_version6_Conv_TypeI_EE_SignalConv_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
  if(GetYear() == 2017)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_EC_Conv",MVAPathConvv3+"BDTG_version6_Conv_TypeI_EE_SignalConv_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);
  if(GetYear() == 2018)   ElectronIDv4_ConvMVAReader->BookMVA("BDTGv4_EC_Conv",MVAPathConvv3+"BDTG_version6_Conv_TypeI_EE_SignalConv_EC_2018_NTrees300_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT"+xmlpf);

  //0.939642286898: ['0.395', '0.119', 'BDTG_version6_Conv_TypeI_EE_SignalConv_EC_2016_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_5_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
  //0.956053445237: ['0.789', '0.117', 'BDTG_version6_Conv_TypeI_EE_SignalConv_EC_2017_NTrees2000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_4_nCuts_300_Shrinkage_0.05_BaggedFrac_0.8_Seed_100_BDT.root']
  //0.951211013251: ['0.112', '0.452', 'BDTG_version6_Conv_TypeI_EE_SignalConv_EC_2018_NTrees300_NormMode_EqualNumEvents_MinNodeSize_2.5_MaxDepth_2_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT.root']

  return;


}

void AnalyzerCore::SetBDTIDVariablesMuon(Muon mu){

  bdt_id_MVA = mu.MVA();

  if (JetLeptonPtRelLepAware(mu) >600) bdt_id_PtRel = 600;
  else bdt_id_PtRel=JetLeptonPtRelLepAware(mu);
  bdt_id_PtRatio=JetLeptonPtRatioLepAware(mu);

  if(mu.Chi2() > 100) bdt_id_Chi2=100;
  else bdt_id_Chi2 = mu.Chi2();
  bdt_id_Validhits = float(mu.ValidMuonHits());
  bdt_id_Matched_stations = float(mu.MatchedStations());
  bdt_id_Pixel_hits  = float(mu.PixelHits());
  bdt_id_Tracker_layers = float(mu.TrackerLayers());

  bdt_id_POGTight = mu.isPOGTight() ?  1.: 0.;
  bdt_id_POGMedium = mu.isPOGMedium() ?  1.: 0.;
  bdt_id_HNTightID = mu.PassID("HNTightV2") ?  1.: 0.;

  return;

}


void AnalyzerCore::SetBDTIDVariablesElectronOLD(Electron el){

  bdt_id_MVA    = el.MVANoIsoResponse();
  bdt_id_MVAIso    = el.MVAIsoResponse();
  bdt_id_RelMVA    = el.MVANoIsoResponse()/el.Pt();
  bdt_id_RelMVAIso    = el.MVAIsoResponse()/el.Pt();
  bdt_id_MissingHits = el.NMissingHits();
  bdt_id_Full5x5_sigmaIetaIeta  = el.Full5x5_sigmaIetaIeta(); 
  bdt_id_dEtaSeed  = el.dEtaSeed();
  if(bdt_id_dEtaSeed > 0.2) bdt_id_dEtaSeed = 0.2;
  if(bdt_id_dEtaSeed < -0.2) bdt_id_dEtaSeed= -0.2;

  bdt_id_dPhiIn  = el.dPhiIn();
  if(bdt_id_dPhiIn > 0.6) bdt_id_dPhiIn = 0.6;
  if(bdt_id_dPhiIn < -0.6) bdt_id_dPhiIn = -0.6;
  
  bdt_id_dEtaIn  = el.dEtaIn();   
  if(bdt_id_dEtaIn > 0.1) bdt_id_dEtaIn=  0.1;
  if(bdt_id_dEtaIn < -0.1) bdt_id_dEtaIn=  -0.1;

  bdt_id_EtaWidth = el.EtaWidth();
  bdt_id_PhiWidth = el.PhiWidth();
  bdt_id_e2x5OverE5x5 = el.e2x5OverE5x5();
  bdt_id_e1x5OverE5x5= el.e1x5OverE5x5();
  bdt_id_e15= el.e15();
  bdt_id_e25= el.e25();
  bdt_id_e55= el.e55();
  bdt_id_dr03EcalRecHitSumEt= el.dr03EcalRecHitSumEt()/el.UncorrPt();
  bdt_id_dr03HcalDepth1TowerSumEt= el.dr03HcalDepth1TowerSumEt()/el.UncorrPt();
  bdt_id_dr03HcalTowerSumEt= el.dr03HcalTowerSumEt()/el.UncorrPt();
  bdt_id_dr03TkSumPt= el.dr03TkSumPt()/el.UncorrPt();
  bdt_id_R9= el.R9();
  if(bdt_id_R9 > 5) bdt_id_R9 = 5;

  bdt_id_HoverE  = el.HoverE();
  if (el.TrkIso()/el.UncorrPt() > 10) bdt_id_TrkIso = 10;
  else bdt_id_TrkIso  = el.TrkIso()/el.UncorrPt();  
  if(isinf(el.InvEminusInvP())) bdt_id_InvEminusInvP = 0.;
  else bdt_id_InvEminusInvP = fabs(el.InvEminusInvP());                                                                         

  bdt_id_ecalPFClusterIso = el.ecalPFClusterIso()/el.UncorrPt();
  bdt_id_hcalPFClusterIso = el.hcalPFClusterIso()/el.UncorrPt();
  bdt_id_isEcalDriven = (el.isEcalDriven()) ? 1. : 0.;
  bdt_id_EoverP = log(el.EOverP());
  if(bdt_id_EoverP > 20) bdt_id_EoverP = 20;
  if(bdt_id_EoverP < -20) bdt_id_EoverP = -20;

  if (el.FBrem() < -1) bdt_id_FBrem = -1;
  else   bdt_id_FBrem = el.FBrem();

  bdt_id_PassConversionVeto  = (el.PassConversionVeto()) ? 1. : 0.;
  bdt_id_IsGsfCtfScPixChargeConsistent  = (el.IsGsfCtfScPixChargeConsistent()) ? 1. : 0.;
  bdt_id_IsGsfScPixChargeConsistent  = (el.IsGsfScPixChargeConsistent()) ? 1. : 0.;
  bdt_id_IsGsfCtfChargeConsistent  = (el.IsGsfCtfChargeConsistent()) ? 1. : 0.;

  bdt_id_PtRatio=JetLeptonPtRatioLepAware(el);

  if (JetLeptonPtRelLepAware(el) > 600)bdt_id_PtRel = 600;
  else bdt_id_PtRel=JetLeptonPtRelLepAware(el);

  bdt_id_POGTight = el.PassID("passPOGTight") ?  1.: 0.;
  bdt_id_POGMedium =el.PassID("passPOGMedium") ?  1.: 0.;
  bdt_id_HNTightID = el.PassID("HNTightV2") ?  1.: 0.;


}

void AnalyzerCore::SetBDTIDVariablesElectron(Electron el){

  //// MVA Values  Response used as POG cut uses rrsponse not value
  bdt_id_MVA       = el.MVANoIsoResponse();
  bdt_id_MVAIso    = el.MVAIsoResponse();
  
  bdt_id_MVARaw    = el.MVANoIso();
  bdt_id_MVAIsoRaw = el.MVAIso();
   
  ///// https://github.com/cms-sw/cmssw/blob/CMSSW_10_6_25/RecoEgamma/ElectronIdentification/data/ElectronMVAEstimatorRun2Variables.txt

  bdt_id_MissingHits           = el.NMissingHits();
  bdt_id_Full5x5_sigmaIetaIeta = el.Full5x5_sigmaIetaIeta(); 

  bdt_id_dEtaSeed          = fabs(el.dEtaSeed());
  if(bdt_id_dEtaSeed > 0.2) bdt_id_dEtaSeed = 0.2;

  bdt_id_dPhiIn  = fabs(el.dPhiIn());
  if(bdt_id_dPhiIn > 0.6) bdt_id_dPhiIn = 0.6;
  
  bdt_id_dEtaIn  = fabs(el.dEtaIn());   
  if(bdt_id_dEtaIn > 0.06) bdt_id_dEtaIn=  0.06;   /// 0.06

  bdt_id_EtaWidth = el.EtaWidth();
  bdt_id_PhiWidth = el.PhiWidth();
  
  bdt_id_e2x5OverE5x5 = el.e2x5OverE5x5();
  bdt_id_e1x5OverE5x5= (el.e55()) !=0. ? 1.-(el.e15()/el.e55()) : -1. ;
  if(bdt_id_e1x5OverE5x5 < -1) bdt_id_e1x5OverE5x5=-1;
  if(bdt_id_e1x5OverE5x5 > 2) bdt_id_e1x5OverE5x5=2;

  bdt_id_e55= el.e55();

  bdt_id_dr03HcalTowerSumEt= el.dr03HcalTowerSumEt()/el.UncorrPt();
  bdt_id_dr03TkSumPt= el.dr03TkSumPt()/el.UncorrPt();
  bdt_id_R9= el.R9();
  if(bdt_id_R9 > 5) bdt_id_R9 = 5;

  
  bdt_id_psEoRraw= el.psEoverEraw();
  if (bdt_id_psEoRraw > 1) bdt_id_psEoRraw=1;

  bdt_id_HoverE  = el.HoverE();

  if (el.TrkIso()/el.UncorrPt() > 10) bdt_id_TrkIso = 10;
  else bdt_id_TrkIso  = el.TrkIso()/el.UncorrPt();  

  if(isinf(el.InvEminusInvP())) bdt_id_InvEminusInvP = 0.;
  else bdt_id_InvEminusInvP = fabs(el.InvEminusInvP());                                                                         
  
  bdt_id_ecalPFClusterIso = el.ecalPFClusterIso()/el.UncorrPt();
  bdt_id_hcalPFClusterIso = el.hcalPFClusterIso()/el.UncorrPt();
 
  bdt_id_isEcalDriven = (el.isEcalDriven()) ? 1. : 0.;
  
  if(el.EOverP() > 20) bdt_id_EoverP = log(20);
  else bdt_id_EoverP = log(el.EOverP());  //// 20 then log?

  if (el.FBrem() < -1) bdt_id_FBrem = -1;
  else   bdt_id_FBrem = el.FBrem();

  bdt_id_Rho=el.Rho();
  bdt_id_PassConversionVeto  = (el.PassConversionVeto()) ? 1. : 0.;

  bdt_id_IsGsfCtfScPixChargeConsistent  = (el.IsGsfCtfScPixChargeConsistent()) ? 1. : 0.;
  bdt_id_IsGsfScPixChargeConsistent  = (el.IsGsfScPixChargeConsistent()) ? 1. : 0.;
  bdt_id_IsGsfCtfChargeConsistent  = (el.IsGsfCtfChargeConsistent()) ? 1. : 0.;

  bdt_id_PtRatio=JetLeptonPtRatioLepAware(el);
  bdt_id_PtRatioV2=JetLeptonPtRatioLepAwareV2(el);
  bdt_id_PtRatioV3 = (el.IsBB()) ?  bdt_id_PtRatio : bdt_id_PtRatioV2;

  if (JetLeptonPtRelLepAware(el) > 600)bdt_id_PtRel = 600;
  else bdt_id_PtRel=JetLeptonPtRelLepAware(el);

  if (JetLeptonPtRelLepAwareV2(el) > 600)bdt_id_PtRelV2 = 600;
  else bdt_id_PtRelV2=JetLeptonPtRelLepAwareV2(el);

  bdt_id_PtRelV3 = (el.IsBB()) ? bdt_id_PtRel : bdt_id_PtRelV2;

  bdt_id_POGTight = el.PassID("passPOGTight") ?  1.: 0.;
  bdt_id_POGMedium =el.PassID("passPOGMedium") ?  1.: 0.;
  bdt_id_HNTightID = el.PassID("HNTightV2") ?  1.: 0.;
  bdt_id_POGMVA80ID = el.passMVAID_noIso_WP80() ? 1.: 0.;
  bdt_id_POGMVA90ID = el.passMVAID_noIso_WP90() ? 1.: 0.;
  return;
}



double AnalyzerCore::GetBDTScoreMuon_EtaDependant(Muon mu ,BkgType bkg, TString BDTTag){

  TString  MVATagStr = BDTTag;
  int Version=1;
  if(MVATagStr.Contains("BDTGv4"))Version=4;
  if(MVATagStr.Contains("BDTGv5"))Version=5;

  InitializeIDTreeVars();

  if(bkg != BkgType::Fake) return 1.;

  Lepton *lep = (Lepton *)(&mu);
  if(Version < 5) {
    SetBDTIDVarOLD(lep);
    SetBDTIDVariablesMuon(mu);
    if(mu.IsBB())    MVATagStr += "_BB";
    else  MVATagStr += "_EC";

  }
  else{
    SetBDTIDVar(lep);
    SetBDTIDVariablesMuon(mu);
    
    if(mu.IsEC() )  MVATagStr += "_EC";  
  }
  
  if (bkg == BkgType::Fake)   MVATagStr += "_Fake";

  if(MVATagStr.Contains("v4"))   return  MuonIDv4_FakeMVAReader->EvaluateMVA(MVATagStr);
  if(MVATagStr.Contains("v5"))   return  MuonIDv5_FakeMVAReader->EvaluateMVA(MVATagStr);

  return -1.;
}



double AnalyzerCore::GetBDTScoreEl_EtaDependant(Electron el ,BkgType bkg, TString BDTTag){

  TString  MVATagStr = BDTTag;
  int Version=1;
  if(MVATagStr.Contains("BDTGv4"))Version=4;
  if(MVATagStr.Contains("BDTGv5"))Version=5;

  /// Version 5 now sets BB to inclusive and EC to EC 

  InitializeIDTreeVars();
  
  Lepton *lep = (Lepton *)(&el);
  if(Version < 5) {
    SetBDTIDVarOLD(lep);
    SetBDTIDVariablesElectronOLD(el);
    
    if(el.IsBB())    MVATagStr += "_BB";
    else  MVATagStr += "_EC";
  }
  if(Version == 5) {
    SetBDTIDVar(lep);
    SetBDTIDVariablesElectron(el);

    if(el.IsBB())    MVATagStr += "_BB";
    if(el.IsEC())  MVATagStr += "_EC";
  }
  
  if (bkg == BkgType::CF)     MVATagStr += "_CF";
  if (bkg == BkgType::Conv)   MVATagStr += "_Conv";
  if (bkg == BkgType::Fake)   MVATagStr += "_Fake";


  if(MVATagStr.Contains("BDTGv4")){

    if(!iSetupLeptonBDTv4) {   cout << "iSetupLeptonBDTv2 not set.." << MVATagStr<< endl;     exit(EXIT_FAILURE);}
    
    if (bkg == BkgType::CF)    return  ElectronIDv4_CFMVAReader->EvaluateMVA(MVATagStr);
    
    if (bkg == BkgType::Conv) return  ElectronIDv4_ConvMVAReader->EvaluateMVA(MVATagStr);
  
    if (bkg == BkgType::Fake){
      TString MVATagStrTMP = MVATagStr;
      return  ElectronIDv4_FakeMVAReader->EvaluateMVA(MVATagStrTMP);
    }
  }
  
  if(MVATagStr.Contains("BDTGv5")){
    if(!iSetupLeptonBDTv5) {   cout << "iSetupLeptonBDTv5 not set.." << MVATagStr<< endl;     exit(EXIT_FAILURE);}

    if(MVATagStr.Contains("BDTGv5_"))      return  ElectronIDv5_MVAReader->EvaluateMVA(MVATagStr);
    
    if (bkg == BkgType::CF) {
      if(MVATagStr.Contains("Pt"))  return  ElectronIDv5_CFMVAReader->EvaluateMVA(MVATagStr);
    }
  }

  return -1.;
}



/// Original BDT functions

double AnalyzerCore::GetBDTScoreElV1(Electron el ,BkgType bkg, TString BDTTag){

  InitializeElectronIDTreeVars();

  std::vector<Jet>   JetAllColl = GetJets("NoID", 10., 5.0);

  Lepton *lep = (Lepton *)(&el);
  SetBDTIDVarV1(lep);

  bdt_id_MVA    = el.MVANoIsoResponseV1();
  bdt_id_MVAIso    = el.MVAIsoResponseV1();
  bdt_id_MissingHits = el.NMissingHits();
  bdt_id_Full5x5_sigmaIetaIeta  = el.Full5x5_sigmaIetaIeta();  //sigmaIetaIeta  = el.sigmaIetaIeta();                                                                                                                                                
  bdt_id_dEtaSeed  = el.dEtaSeed();
  bdt_id_dPhiIn  = el.dPhiIn();//  dEtaIn  = el.dEtaIn();                                                                                                                                                                                            
  bdt_id_EtaWidth = el.EtaWidth();
  bdt_id_PhiWidth = el.PhiWidth();
  bdt_id_e2x5OverE5x5 = el.e2x5OverE5x5();
  bdt_id_e55= el.e55();

  bdt_id_dr03HcalTowerSumEt= el.dr03HcalTowerSumEt()/el.UncorrPt();
  bdt_id_dr03TkSumPt= el.dr03TkSumPt()/el.UncorrPt();
  bdt_id_R9= el.R9();
  bdt_id_HoverE  = el.HoverE();
  bdt_id_TrkIso  = el.TrkIso()/el.UncorrPt();  //InvEminusInvP = fabs(el.InvEminusInvP());                                                                                                                                                           
  bdt_id_ecalPFClusterIso = el.ecalPFClusterIso()/el.UncorrPt();
  bdt_id_hcalPFClusterIso = el.hcalPFClusterIso()/el.UncorrPt();

  bdt_id_isEcalDriven = el.isEcalDriven();

  bdt_id_EoverP = log(el.EOverP());

  bdt_id_FBrem = el.FBrem();

  bdt_id_PassConversionVeto  = (el.PassConversionVeto()) ? 1 : 0;
  bdt_id_IsGsfCtfScPixChargeConsistent  = (el.IsGsfCtfScPixChargeConsistent()) ? 1 : 0;
  bdt_id_IsGsfScPixChargeConsistent  = (el.IsGsfScPixChargeConsistent()) ? 1 : 0;
  bdt_id_IsGsfCtfChargeConsistent  = (el.IsGsfCtfChargeConsistent()) ? 1 : 0;


  //==== Vars for non-prompt lepton bkg                                                                                                                                                                                                       
  int IdxMatchJet=-1;
  float mindR1=999.;

  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){
    float dR1=lep->DeltaR(JetAllColl.at(ij));
    if(dR1>0.4) continue;
    if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
  }

  if(IdxMatchJet!=-1){
    bdt_id_PtRatio=JetLeptonPtRatioLepAware(el);
    bdt_id_PtRel=JetLeptonPtRelLepAware(el);
  }

  else{
    bdt_id_PtRatio = min(1/(1.+lep->RelIso()), 1.5);
    bdt_id_PtRel=0;
  }

  TString MVATagStr="";
  if(BDTTag.Contains("v2")){

    MVATagStr = BDTTag;
    MVATagStr=MVATagStr.ReplaceAll("v2","");
    
    if(el.IsBB())    MVATagStr += "_BB";
    else  MVATagStr += "_EC";

    if (bkg == BkgType::Fake) MVATagStr += "_Fake";
    if (bkg == BkgType::Conv) MVATagStr += "_Conv";

  }
  else{
    MVATagStr = BDTTag;
    if (bkg == BkgType::Fake) MVATagStr += "_Fake";
    if (bkg == BkgType::Conv) MVATagStr += "_Conv";
    if (bkg == BkgType::CF){
      if(el.IsBB())   MVATagStr += "_BB";
      else     MVATagStr += "_EC";
      MVATagStr += "_CF";
    }
  }


  return -999;
}



double AnalyzerCore::GetBDTScoreEl(Electron el ,BkgType bkg, TString BDTTag){

  TString  MVATagStr = BDTTag;
  int Version=1;
  if(MVATagStr.Contains("BDTGv4"))Version=4;
  if(MVATagStr.Contains("BDTGv5"))Version=5;

  InitializeIDTreeVars();

  Lepton *lep = (Lepton *)(&el);
  if(Version < 5) {
    SetBDTIDVarOLD(lep);
    SetBDTIDVariablesElectronOLD(el);
  }
  if(Version == 5) {
    SetBDTIDVar(lep);
    SetBDTIDVariablesElectron(el);
  }

  if (bkg == BkgType::Fake) MVATagStr += "_Fake";
  if (bkg == BkgType::FakeRate) MVATagStr += "_Fake";
  if (bkg == BkgType::Conv) MVATagStr += "_Conv";
  if (bkg == BkgType::CF)   MVATagStr += "_CF";

  if(MVATagStr.Contains("BDTGv4")){

    if(!iSetupLeptonBDTv4) {   cout << "iSetupLeptonBDTv2 not set.." << MVATagStr<< endl;     exit(EXIT_FAILURE);}
    
    
    if (bkg == BkgType::CF){

      if(MVATagStr.Contains("BDTGv4_"))      return  ElectronIDv4_CFMVAReader->EvaluateMVA(MVATagStr);
    }
    
    if (bkg == BkgType::Fake) {
      TString MVATagStrTMP = MVATagStr;
      //      MVATagStrTMP=MVATagStrTMP.ReplaceAll("BDTGv4_HF","BDTGv3_HF"); //// TEST
      return  ElectronIDv4_FakeMVAReader->EvaluateMVA(MVATagStrTMP);
    }
    
    if (bkg == BkgType::Conv) return  ElectronIDv4_ConvMVAReader->EvaluateMVA(MVATagStr);
  }

  
  if(MVATagStr.Contains("BDTGv5")){

    if(!iSetupLeptonBDTv5) {   cout << "iSetupLeptonBDTv5 not set.." << MVATagStr<< endl;     exit(EXIT_FAILURE);}

    if (bkg == BkgType::FakeRate) return  ElectronIDv5_FakeMVAReader->EvaluateMVA(MVATagStr);

    if(MVATagStr.Contains("BDTGv5_"))  return  ElectronIDv5_MVAReader->EvaluateMVA(MVATagStr);
    if(MVATagStr.Contains("Pt"))       return  ElectronIDv5_CFMVAReader->EvaluateMVA(MVATagStr);


    cout << "--> ElectronIDv5_FakeMVAReader " << MVATagStr << endl;
  }

  
  return -1.;
}



std::vector<Muon> AnalyzerCore::GetMuons(TString id, double ptmin, double fetamax){

  std::vector<Muon> muons =  All_Muons;
  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    if(!( muons.at(i).Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << muons.at(i).Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(muons.at(i).Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(muons.at(i).Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( muons.at(i).PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back( muons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Muon> AnalyzerCore::GetMuons(AnalyzerParameter param, bool Run_Fake){
  return GetMuons(param, param.Muon_Tight_ID, param.Muon_MinPt, param.Muon_MaxEta, Run_Fake);
}

std::vector<Muon> AnalyzerCore::GetMuons(AnalyzerParameter param, TString id, double ptmin, double fetamax, bool Run_Fake){

  std::vector<Muon> this_AllMuons =  All_Muons;
  std::vector<Muon> muons ;

  if(param.syst_ == AnalyzerParameter::MuonEnUp)    muons = ScaleMuons( this_AllMuons, +1 );
  else if(param.syst_ == AnalyzerParameter::MuonEnDown)    muons = ScaleMuons( this_AllMuons, -1 );
  else muons = this_AllMuons;

  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    if(!( muons.at(i).Pt()> ptmin )){
      //cout << "Fail Pt : pt = " << muons.at(i).Pt() << ", cut = " << ptmin << endl;                                                     
      continue;
    }
    if(!( fabs(muons.at(i).Eta())< fetamax )){
      // cout << "Fail Eta : eta = " << fabs(muons.at(i).Eta()) << ", cut = " << fetamax << endl;                                          
      continue;
    }
    if(!( muons.at(i).PassID(id) )){
      continue;
    }
 
    if(Run_Fake){

      double isocut_mu = GetIsoFromID("Muon", param.Muon_Tight_ID,muons.at(i).Eta(), muons.at(i).Pt());
      Muon this_muon = muons.at(i);
      this_muon.SetPtEtaPhiM( muons.at(i).CalcPtCone(muons.at(i).RelIso(), isocut_mu), muons.at(i).Eta(), muons.at(i).Phi(), muons.at(i).M() );
      out.push_back( this_muon);

    }
    else   out.push_back( muons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}



std::vector<Electron> AnalyzerCore::GetAllElectrons(){

  std::vector<Electron> out;
  if(!electron_Energy) return out;


  for(unsigned int i=0; i<electron_Energy->size(); i++){

    Electron el;

    el.SetEnShift(  electron_Energy_Scale_Up->at(i)/electron_Energy->at(i), electron_Energy_Scale_Down->at(i)/electron_Energy->at(i) );
    el.SetResShift( electron_Energy_Smear_Up->at(i)/electron_Energy->at(i), electron_Energy_Smear_Down->at(i)/electron_Energy->at(i) );

    el.SetPtEtaPhiE(1., electron_eta->at(i), electron_phi->at(i), electron_Energy->at(i));
    double el_theta = el.Theta();
    double el_pt = electron_Energy->at(i) * TMath::Sin( el_theta );
    el.SetPtEtaPhiE( el_pt, electron_eta->at(i), electron_phi->at(i), electron_Energy->at(i));
    el.SetUncorrectedPt(electron_EnergyUnCorr->at(i) * TMath::Sin( el_theta ));

    el.SetUncorrE(electron_EnergyUnCorr->at(i));
    el.SetSC(electron_scEta->at(i), electron_scPhi->at(i), electron_scEnergy->at(i));
    el.SetElSCEta(electron_scEta->at(i));
    el.SetCharge(electron_charge->at(i));
    el.SetdXY(electron_dxyVTX->at(i), electron_dxyerrVTX->at(i));
    el.SetdZ(electron_dzVTX->at(i), electron_dzerrVTX->at(i));
    el.SetIP3D(electron_3DIPVTX->at(i), electron_3DIPerrVTX->at(i));
    el.SetMVA(electron_MVAIso->at(i), electron_MVANoIso->at(i));
    el.SetLepMVA( electron_MVANoIso->at(i));
    el.SetPassConversionVeto(electron_passConversionVeto->at(i));
    el.SetNMissingHits(electron_mHits->at(i));
    el.SetRho(Rho);
    el.SetIsGsfCtfScPixChargeConsistent(electron_isGsfCtfScPixChargeConsistent->at(i),   electron_isGsfScPixChargeConsistent->at(i),electron_isGsfCtfChargeConsistent->at(i));

    el.SetR9(electron_r9->at(i));
    el.SetL1Et(electron_l1et->at(i));
    el.SetpsEoverEraw(electron_scPreEnergy->at(i)/electron_scRawEnergy->at(i));
    
    el.SetCutBasedIDVariables(
      electron_Full5x5_SigmaIEtaIEta->at(i),
      electron_dEtaInSeed->at(i),
      electron_dPhiIn->at(i),
      electron_HoverE->at(i),
      electron_InvEminusInvP->at(i),
      electron_e2x5OverE5x5->at(i),
      electron_e1x5OverE5x5->at(i),
      electron_trackIso->at(i),
      electron_dr03EcalRecHitSumEt->at(i),
      electron_dr03HcalDepth1TowerSumEt->at(i),
      electron_dr03HcalTowerSumEt->at(i),
      electron_dr03TkSumPt->at(i),
      electron_ecalPFClusterIso->at(i),
      electron_hcalPFClusterIso->at(i),
      electron_ecalDriven->at(i)
    );

    el.SetIDBit(electron_IDBit->at(i));
    vector<int> temp_idcutbit;
    for(unsigned int j=0; j<Electron::N_SELECTOR; j++){
      temp_idcutbit.push_back( electron_IDCutBit->at( i*Electron::N_SELECTOR + j ) );
    }
    el.SetIDCutBit(temp_idcutbit);
    el.SetRelPFIso_Rho(electron_RelPFIso_Rho->at(i));

    //==== Should be ran after SCeta is set
    el.SetMiniIso(
      electron_chMiniIso->at(i),
      electron_nhMiniIso->at(i),
      electron_phMiniIso->at(i),
      electron_puChMiniIso->at(i),
      Rho,
      el.EA()
    );
    el.SetEtaWidth(electron_etaWidth->at(i));
    el.SetPhiWidth(electron_phiWidth->at(i));
    el.SetFBrem(electron_fbrem->at(i));
    el.SetEOverP(electron_eOverP->at(i));
    el.SetDetaIn(electron_dEtaIn->at(i));
    el.SetSigmaIEtaIE(electron_sigmaIEtaIEta->at(i));
    el.SetLepIso(electron_chIso03->at(i),electron_nhIso03->at(i),electron_phIso03->at(i));
    
    el.SetE15(electron_E15->at(i));
    el.SetE25(electron_E25->at(i));
    el.SetE55(electron_E55->at(i));


    el.SetFilterBits(electron_filterbits->at(i));
    el.SetPathBits(electron_pathbits->at(i));


    /////// USE NEW Branches to set lepton variables
    if(fChain->GetBranch("electron_v2_cj_flavour")) el.SetCloseJetFlavour(electron_v2_cj_flavour->at(i));
    if(fChain->GetBranch("electron_ptrel"))      el.SetJetPtRel(electron_ptrel->at(i));
    if(fChain->GetBranch("electron_ptratio"))    el.SetJetPtRatio(electron_ptratio->at(i));
    if(fChain->GetBranch("electron_v2_cj_bjetdisc"))    el.SetCloseJetBScore(electron_v2_cj_bjetdisc->at(i));
    if(fChain->GetBranch("electron_v2_cj_cvsbjetdisc")) el.SetCloseJetCvsBScore(electron_v2_cj_cvsbjetdisc->at(i));
    if(fChain->GetBranch("electron_v2_cj_cvsljetdisc")) el.SetCloseJetCvsLScore(electron_v2_cj_cvsljetdisc->at(i));
        
    //////// MVA Branches
    ////*************** FAKE MVA
    if(fChain->GetBranch("electron_mva_fake_v4"))  el.SetHNL_FakeLepMVAV4(electron_mva_fake_v4->at(i), 
                    electron_mva_fakeHF_v4->at(i), 
                    electron_mva_fakeHFB_v4->at(i),
                    electron_mva_fakeHFC_v4->at(i), 
                    electron_mva_fakeLF_v4->at(i),
                    electron_mva_fakeTop_v4->at(i));

    if(fChain->GetBranch("electron_mva_fake_ed_v4"))  el.SetHNL_FakeLepMVA_EtaDependantV4(electron_mva_fake_ed_v4->at(i),
                        electron_mva_fakeHF_ed_v4->at(i),
                        electron_mva_fakeHFB_ed_v4->at(i),
                        electron_mva_fakeHFC_ed_v4->at(i),
                        electron_mva_fakeLF_ed_v4->at(i),
                        electron_mva_fakeTop_ed_v4->at(i));
    
    ////*************** CONV MVA
    if(fChain->GetBranch("electron_mva_conv_v2"))    el.SetHNL_ConvLepMVAV4(electron_mva_conv_v2->at(i));
    if(fChain->GetBranch("electron_mva_conv_ed_v2")) el.SetHNL_ConvLepMVA_EtaDependantV4(electron_mva_conv_ed_v2->at(i));

    ////*************** CF MVA
    if(fChain->GetBranch("electron_mva_cf_v2"))     el.SetHNL_CFLepMVAV4(electron_mva_cf_v2->at(i));

    if(fChain->GetBranch("electron_mva_cf_ed_v2"))  el.SetHNL_CFLepMVA_EtaDependantV4(electron_mva_cf_ed_v2->at(i));

    

    if(fChain->GetBranch("electron_mva_conv_ed_v5")) el.SetHNL_ConvLepMVA_EtaDependantV5(electron_mva_conv_ed_v5->at(i));
                       

    if(fChain->GetBranch("electron_mva_cf_ed_v5"))  el.SetHNL_CFLepMVA_EtaDependantV5(electron_mva_cf_ed_v5->at(i), 
                          electron_mva_cf_ed_v5pt->at(i));

    if(fChain->GetBranch("electron_mva_fake_ed_v5")) {
      
      el.SetHNL_FakeLepMVA_EtaDependantV5(electron_mva_fake_ed_v5->at(i),
            electron_mva_fakeHFB_v5->at(i),
            electron_mva_fakeHFC_v5->at(i),
            electron_mva_fakeLF_v5->at(i));
      
      el.SetHNL_FakeFlavourLepMVA_V5(electron_mva_fake_QCD_LFvsHF_v5->at(i),
             electron_mva_fake_QCD_HFBvsHFC_v5->at(i),
             electron_mva_fake_QCD_LF1_v5->at(i),
             electron_mva_fake_QCD_LF2_v5->at(i));





    }
    
    if(iSetupLeptonBDTv5){


      if(!fChain->GetBranch("electron_mva_conv_ed_v5"))      el.SetHNL_ConvLepMVA_EtaDependantV5( GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Conv,  "BDTGv5"));
      if(!fChain->GetBranch("electron_mva_fake_ed_v5"))      el.SetHNL_FakeLepMVA_EtaDependantV5(GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Fake,  "BDTGv5" ), 
                         GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv5_HFB" ) , 
                         GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv5_HFC" ),  
                         GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv5_LF" ));
      
      
      if(!fChain->GetBranch("electron_mva_cf_ed_v5"))      el.SetHNL_CFLepMVA_EtaDependantV5  (GetBDTScoreEl_EtaDependant(el,AnalyzerCore::CF, "BDTGv5" ), 
                             GetBDTScoreEl_EtaDependant(el,AnalyzerCore::CF, "BDTGv5Pt" ));
      
      if(!fChain->GetBranch("electron_mva_fake_QCD_LFvsHF_v5")) {
  
        el.SetHNL_FakeFlavourLepMVA_V5(  GetBDTScoreEl(el,AnalyzerCore::FakeRate,  "BDTGv5_QCD_LFvsHF"),
          GetBDTScoreEl(el,AnalyzerCore::FakeRate,  "BDTGv5_QCD_HFBvsHFC"),
          GetBDTScoreEl(el,AnalyzerCore::FakeRate,  "BDTGv5_LF1"),
          GetBDTScoreEl(el,AnalyzerCore::FakeRate,  "BDTGv5_LF2"));
      }
    }


    if(iSetupLeptonBDTv4){

      if(!fChain->GetBranch("electron_mva_fake_v4"))  el.SetHNL_FakeLepMVAV4(GetBDTScoreEl(el,  AnalyzerCore::Fake,  "BDTGv4" ), 
                       GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv4_HF"), 
                       GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv4_HFB"), 
                       GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv4_HFC"), 
                       GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv4_LF"),
                       GetBDTScoreEl(el,AnalyzerCore::Fake,  "BDTGv4_Top"));

      if(!fChain->GetBranch("electron_mva_fake_ed_v4"))  el.SetHNL_FakeLepMVA_EtaDependantV4(GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Fake,  "BDTGv4" ),
                           GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Fake,  "BDTGv4_HF"),
                           GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Fake,  "BDTGv4_HFB"),
                           GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Fake,  "BDTGv4_HFC"),
                           GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Fake,  "BDTGv4_LF"),
                           GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Fake,  "BDTGv4_Top"));
      



      if(!fChain->GetBranch("electron_mva_conv_v2"))    el.SetHNL_ConvLepMVAV4(GetBDTScoreEl(el,AnalyzerCore::Conv,  "BDTGv4"));
      if(!fChain->GetBranch("electron_mva_conv_ed_v2")) el.SetHNL_ConvLepMVA_EtaDependantV4(GetBDTScoreEl_EtaDependant(el,AnalyzerCore::Conv,  "BDTGv4"));
      
      
      if(!fChain->GetBranch("electron_mva_cf_v4"))     el.SetHNL_CFLepMVAV4(GetBDTScoreEl(el,AnalyzerCore::CF,  "BDTGv4"));
      if(!fChain->GetBranch("electron_mva_cf_ed_v4"))  el.SetHNL_CFLepMVA_EtaDependantV4(GetBDTScoreEl_EtaDependant(el,AnalyzerCore::CF,  "BDTGv4"));
      
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool FillCloseJetVar=!fChain->GetBranch("electron_v2_cj_cvsbjetdisc");
    if(Analyzer=="HNL_LeptonID_BDT_KinVar") FillCloseJetVar=false;
    if(FillCloseJetVar){
      
      std::vector<Jet>    AK4_JetAllColl = All_Jets;
      
      float  JetDiscCJ = -999;
      float  JetDiscCJ_CvsB = -999;
      float  JetDiscCJ_CvsL = -999;

      float mindR1=999.;
      int IdxMatchJet=-1;
      int JetHadFlavour = -999;
    
      for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
  float dR1=el.DeltaR(AK4_JetAllColl.at(ij));
  if(dR1>0.4) continue;
  if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
      }
      if(IdxMatchJet!=-1) {
  JetDiscCJ        = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
        JetDiscCJ_CvsB   = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsB);
        JetDiscCJ_CvsL    = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsL);

  JetHadFlavour  = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();
      }
      else {
        JetDiscCJ=0.1; //// These are low pt ; value chosen to match peak in prompt leptons                                                                                
        JetDiscCJ_CvsB=0.4;
        JetDiscCJ_CvsL=0.1;
  JetHadFlavour = 0;
      }

      el.SetJetPtRel(JetLeptonPtRelLepAware(el));
      el.SetJetPtRatio(JetLeptonPtRatioLepAware(el));
      el.SetCloseJetBScore(JetDiscCJ);
      el.SetCloseJetCvsBScore(JetDiscCJ_CvsB);
      el.SetCloseJetCvsLScore(JetDiscCJ_CvsL);
      el.SetCloseJetFlavour(JetHadFlavour);
      
    }
    
    if(!fChain->GetBranch("electron_lepton_type") ||  !fChain->GetBranch("electron_is_cf"))  {
      int lep_type = GetLeptonType_JH(el, All_Gens);
      el.SetLeptonType(lep_type);
      el.SetLeptonIsCF(IsCF(el, All_Gens) );
    }
    else {
      if(fChain->GetBranch("electron_lepton_type")) el.SetLeptonType(electron_lepton_type->at(i));
      if(fChain->GetBranch("electron_is_cf")) el.SetLeptonIsCF(electron_is_cf->at(i));
    }
    out.push_back(el);
  }
  
  //if(PtOrderObj) std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


std::vector<Electron> AnalyzerCore::GetElectrons(AnalyzerParameter param,  bool run_fake, bool vetoHEM){
  return GetElectrons(param, param.Electron_Tight_ID, param.Electron_MinPt, param.Electron_MaxEta, run_fake, vetoHEM);
}

std::vector<Electron> AnalyzerCore::GetElectrons(AnalyzerParameter param, TString id, double ptmin, double fetamax, bool run_fake, bool vetoHEM){
  
  std::vector<Electron> this_AllElectrons = All_Electrons;
  std::vector<Electron> electrons ;

  //cout << "GetElectrons starts;" << endl;
  //cout << "AllElectrons size : " << this_AllElectrons.size() << endl; //JH

  if(param.syst_ == AnalyzerParameter::ElectronResUp)   electrons = SmearElectrons( this_AllElectrons, +1 );
  else if(param.syst_ == AnalyzerParameter::ElectronResDown)   electrons = SmearElectrons( this_AllElectrons, -1 );
  else if(param.syst_ == AnalyzerParameter::ElectronEnUp)    electrons = ScaleElectrons( this_AllElectrons, +1 );
  else if(param.syst_ == AnalyzerParameter::ElectronEnDown)    electrons = ScaleElectrons( this_AllElectrons, -1 );
  else electrons = this_AllElectrons;
  
  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){

    if(!( electrons.at(i).Pt()> ptmin )){
      continue;
    }
    if(!( fabs(electrons.at(i).scEta())< fetamax )){
      continue;
    }
    //cout << i+1 << "th electron passed pt, eta;" << endl;
    //cout << i+1 << "th electron trying to pass " << id << " ..." <<  endl;
    if(!( electrons.at(i).PassID(id) )){
      //cout << "Fail " << id << endl;
      continue;
    }
    //cout << i+1 << "th electron passed ID;" << endl; //JH
    if(vetoHEM){
      
      if ( FindHEMElectron (electrons.at(i)) ){
        continue;
      }
    }

    if(run_fake){
     
      double isocut_el = GetIsoFromID("Electron",param.Electron_Tight_ID,electrons.at(i).Eta(), electrons.at(i).Pt());   
      Electron this_electron = electrons.at(i);
      this_electron.SetPtEtaPhiM( electrons.at(i).CalcPtCone(electrons.at(i).RelIso(), isocut_el), electrons.at(i).Eta(), electrons.at(i).Phi(), electrons.at(i).M() );
      out.push_back( this_electron);
    }
    else   out.push_back( electrons.at(i) );
  }
  
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Electron> AnalyzerCore::GetElectrons(TString id, double ptmin, double fetamax, bool vetoHEM){

  std::vector<Electron> electrons = All_Electrons;

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    if(!( electrons.at(i).Pt()>ptmin )){
      continue;
    }
    if(!( fabs(electrons.at(i).scEta())<fetamax )){
      continue;
    }
    if(!( electrons.at(i).PassID(id) )){
      continue;
    }
    if(vetoHEM){
      if ( FindHEMElectron (electrons.at(i)) ){
        continue;
      }
    }

    out.push_back( electrons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


std::vector<Tau> AnalyzerCore::GetAllTaus(){

  std::vector<Tau> out;
  if(!tau_pt) return out;
  for(unsigned int i=0; i<tau_pt->size(); i++){

    Tau tau;
    tau.SetCharge(tau_charge->at(i));
    tau.SetPtEtaPhiM(tau_pt->at(i), tau_eta->at(i), tau_phi->at(i), tau_mass->at(i));
    tau.SetDecayMode(tau_decaymode->at(i));
    tau.SetdXY(tau_dxy->at(i),0.);
    tau.SetdZ(tau_dz->at(i),0.);
    tau.SetDecayModeNewDM(tau_idDecayModeNewDMs->at(i));
    tau.SetIDBit(tau_IDBit->at(i));

    out.push_back(tau);

  }


  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


std::vector<Tau> AnalyzerCore::GetTaus(std::vector<Lepton* > leps,TString id, double ptmin, double fetamax){

  std::vector<Tau> Taus = GetTaus(id, ptmin, fetamax);
  std::vector<Tau> out;
  for(auto tau : Taus){
    for(auto lep : leps){
      if(lep->DeltaR(tau) > 0.4)  out.push_back(tau);
    }
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;
}

std::vector<Tau> AnalyzerCore::GetTaus(TString id, double ptmin, double fetamax){

  std::vector<Tau> taus = GetAllTaus();
  std::vector<Tau> out;

  for(unsigned int i=0; i<taus.size(); i++){
    if(!( taus.at(i).Pt()>ptmin )){
      continue;
    }
    if(!( fabs(taus.at(i).Eta())<fetamax )){
      continue;
    }
    if(!( taus.at(i).PassID(id) )){
      continue;
    }
    out.push_back( taus.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


double AnalyzerCore::GetIsoFromID(TString  lep_type, TString id, double eta, double pt){

  if (lep_type == "Muon") {
    
    if (id.Contains("MuOpt")){

      TString ID_sub = id;
      ID_sub = ID_sub.ReplaceAll("_"," ");
      string sID_sub = string(ID_sub);

      vector<TString> subStrings;
      istringstream ID_subs(sID_sub);
      do {
  string subs;
  ID_subs >> subs;
  subStrings.push_back(TString(subs));
      } while (ID_subs);

      TString iso_string="";
      for(unsigned int i=0; i < subStrings.size(); i++){
  if (subStrings[i].Contains("ISO")) iso_string = subStrings[i];
      }
      TString iso_cut = iso_string.ReplaceAll("ISOB","");
      iso_cut = iso_cut.ReplaceAll("ISOEC","");
      iso_cut = iso_cut.ReplaceAll("p",".");

      std::string iso_s = std::string(iso_cut);
      std::string::size_type sz;  
      
      double iso_d = std::stod (iso_s,&sz);
      return iso_d;
      
    }

    if (id == "HNTight_17028") return 0.07;
    if (id == "HNTightV1") return 0.07;
    if (id == "HNTightV2") return 0.07;

    if (id == "POGTightPFIsoVeryVeryTight") return 0.05;
    if (id == "HNTight_Iso07_dxy_02_ip_3")  return 0.07;
    if (id.Contains("TightPFIsoVeryVeryTight")) return 0.05;
    if (id.Contains("TightPFIsoVeryTight")) return 0.1;
    if (id.Contains("TightPFIsoTight")) return 0.15;
    if (id.Contains("TightWithTightIso")) return 0.15;
    if (id.Contains("TightStandardPFIsoTight")) return 0.15;
    if (id.Contains("PFIsoMedium")) return 0.2;
    if (id.Contains("PFIsoLoose")) return 0.25;
    if (id.Contains("PFIsoVeto")) return 0.4;

    if (id.Contains("Iso05")) return 0.05;
    if (id.Contains("Iso06")) return 0.06;
    if (id.Contains("Iso07")) return 0.07;
    if (id.Contains("Iso08")) return 0.08;
    if (id.Contains("Iso09")) return 0.09;
    if (id.Contains("Iso10")) return 0.1;

    

    if (id == "POGHighPtTight") return 0.1;
    if (id == "POGHighPtMixTight") return 0.1;
    if (id.Contains("HNMVA_")) return 0.1;
  }
  else if(lep_type == "Electron"){

    if (id.Contains("ElOpt")){

      TString ID_sub = id;
      ID_sub = ID_sub.ReplaceAll("_"," ");
      string sID_sub = string(ID_sub);

      vector<TString> subStrings;
      istringstream ID_subs(sID_sub);
      do {
        string subs;
        ID_subs >> subs;
        subStrings.push_back(TString(subs));
      } while (ID_subs);

      TString iso_string="";
      for(unsigned int i=0; i < subStrings.size(); i++){
        if (subStrings[i].Contains("ISO")) iso_string = subStrings[i];
      } 
      TString iso_cut = iso_string.ReplaceAll("ISOB","");
      iso_cut = iso_cut.ReplaceAll("ISOEC","");
      iso_cut = iso_cut.ReplaceAll("p",".");

      std::string iso_s = std::string(iso_cut);
      std::string::size_type sz;  
      
      double iso_d = std::stod (iso_s,&sz);
      return iso_d;

    }


    if( id == "HNTight_17028") return 0.08;

    if( id.Contains("HNTightV")) {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id == "HN2016") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.06);
    }
    if( id == "HN2017") {
      if(fabs(eta) < 1.479) return 0.085;
      else  return (0.05);
    }
    if( id == "HN2018") {
      if(fabs(eta) < 1.479) return 0.095;
      else  return (0.07);
    }
    if( id == "HNRelaxedIP2016") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.05);
    }
    if( id == "HNRelaxedIP2017") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.05);
    }
    if( id == "HNRelaxedIP2018") {
      if(fabs(eta) < 1.479) return 0.095;
      else  return (0.07);
    }
    if( id == "passTightID_nocc") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id.Contains("passPOGTight")){
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));

    }
    if( id.Contains("passPOGMedium")){
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "passTightID") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id.Contains("HNMediumV")) {
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "passMediumID") {
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "HN2016POG") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }

    if( id == "Iso1") {
      if(fabs(eta) < 1.479) return  0.08;
      else  return 0.08;
    }
    if( id == "Iso2") {
      if(fabs(eta) < 1.479) return  0.09;
      else  return 0.08;
    }
    if( id == "Iso3") {
      if(fabs(eta) < 1.479) return  0.1;
      else  return 0.08;
    }
    if( id == "Iso4") {
      if(fabs(eta) < 1.479) return  0.12;
      else  return 0.08;
    }
    if( id == "Iso5") {
      if(fabs(eta) < 1.479) return  0.09;
      else  return 0.09;
    }
    if( id == "Iso6") {
      if(fabs(eta) < 1.479) return  0.1;
      else  return 0.1;
    }
    if( id == "Iso7") {
      if(fabs(eta) < 1.479) return  0.12;
      else  return 0.12;
    }

    
    if( id.Contains("HNTight_Opt")) return 0.08;

    if( id.Contains("HN2016MVA")) return 0.08;   
    if( id.Contains("HN2016POG")) return 0.08;   
    if( id == "passMVAID_noIso_WP90V16") return 0.05;
    if( id == "passMVAID_noIso_WP80") return 0.08;
    if( id == "passMVAID_noIso_WP90") return 0.08;
    if( id == "passMVAID_Iso_WP80") return 999.0;
    if( id == "passMVAID_Iso_WP90") return 999.0;

    if (id.Contains("HNMVA_")) return 0.1;


  }
  cout << "[AnalyzerCore::GetIsoFromID ] ID not found.." << id<< endl;
  exit(EXIT_FAILURE);

  return -999999999.;

}


std::vector<Lepton *> AnalyzerCore::MakeLeptonPointerVector(const std::vector<Muon>& muons, double TightIso, bool UseMini){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<muons.size(); i++){
    Lepton *l = (Lepton *)(&muons.at(i));
    if( !(l->LeptonFlavour() == Lepton::MUON) ){
      cout << "[AnalyzerCore::MakeLeptonPointerVector(std::vector<Muon>& muons)] Not muon.." << endl;
      exit(EXIT_FAILURE);
    }
    if(TightIso>0){

      double this_RelIso = l->RelIso();
      if(UseMini) this_RelIso = l->MiniRelIso();
      double ptcone = l->CalcPtCone(this_RelIso, TightIso);
      l->SetPtCone( ptcone );

    }
    out.push_back(l);
  }
  std::sort(out.begin(),     out.end(),     PtComparingPtr);

  return out;

}
std::vector<Lepton *> AnalyzerCore::MakeLeptonPointerVector(const std::vector<Electron>& electrons, double TightIso, bool UseMini){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    Lepton *l = (Lepton *)(&electrons.at(i));
    if( !(l->LeptonFlavour() == Lepton::ELECTRON) ){
      cout << "[AnalyzerCore::MakeLeptonPointerVector(std::vector<ELECTRON>& electrons)] Not electron.." << endl;
      exit(EXIT_FAILURE);
    }
    if(TightIso>0){

      double this_RelIso = l->RelIso();
      if(UseMini) this_RelIso = l->MiniRelIso();
      double ptcone = l->CalcPtCone(this_RelIso, TightIso);
      l->SetPtCone( ptcone );

    }
    out.push_back(l);
  }

  std::sort(out.begin(),     out.end(),     PtComparingPtr);

  return out;

}

std::vector<Lepton *> AnalyzerCore::MakeLeptonPointerVector(const std::vector<Muon>& muons, const std::vector<Electron>& electrons,double TightIso, bool UseMini){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<muons.size(); i++){
    Lepton *l = (Lepton *)(&muons.at(i));
    if( !(l->LeptonFlavour() == Lepton::MUON) ){
      cout << "[AnalyzerCore::MakeLeptonPointerVector(std::vector<Muon>& muons)] Not muon.." << endl;
      exit(EXIT_FAILURE);
    }
    if(TightIso>0){

      double this_RelIso = l->RelIso();
      if(UseMini) this_RelIso = l->MiniRelIso();
      double ptcone = l->CalcPtCone(this_RelIso, TightIso);
      l->SetPtCone( ptcone );

    }
    out.push_back(l);
  }

  for(unsigned int i=0; i<electrons.size(); i++){
    Lepton *l = (Lepton *)(&electrons.at(i));
    if( !(l->LeptonFlavour() == Lepton::ELECTRON) ){
      cout << "[AnalyzerCore::MakeLeptonPointerVector(std::vector<ELECTRON>& electrons)] Not electron.." << endl;
      exit(EXIT_FAILURE);
    }
    if(TightIso>0){

      double this_RelIso = l->RelIso();
      if(UseMini) this_RelIso = l->MiniRelIso();
      double ptcone = l->CalcPtCone(this_RelIso, TightIso);
      l->SetPtCone( ptcone );

    }
    out.push_back(l);
  }

  std::sort(out.begin(),     out.end(),     PtComparingPtr);

  return out;
}


std::vector<Lepton *> AnalyzerCore::MakeLeptonPointerVector(const std::vector<Muon>& muons, const std::vector<Electron>& electrons, AnalyzerParameter param_,  double TightIso, bool UseMini){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<muons.size(); i++){
    Lepton *l = (Lepton *)(&muons.at(i));
    if( !(l->LeptonFlavour() == Lepton::MUON) ){
      cout << "[AnalyzerCore::MakeLeptonPointerVector(std::vector<Muon>& muons)] Not muon.." << endl;
      exit(EXIT_FAILURE);
    }
    if(TightIso>0){

      double this_RelIso = l->RelIso();
      if(UseMini) this_RelIso = l->MiniRelIso();
      double ptcone = l->CalcPtCone(this_RelIso, TightIso);
      l->SetPtCone( ptcone );

    }
    l->SetPassID(muons[i].PassID(param_.Muon_Tight_ID));
    //cout << "MakeLeptonPointerVector Muon " << param_.Muon_Tight_ID << " " <<  l->Pt() << endl;
    out.push_back(l);
  }

  for(unsigned int i=0; i<electrons.size(); i++){
    Lepton *l = (Lepton *)(&electrons.at(i));
    if( !(l->LeptonFlavour() == Lepton::ELECTRON) ){
      cout << "[AnalyzerCore::MakeLeptonPointerVector(std::vector<ELECTRON>& electrons)] Not electron.." << endl;
      exit(EXIT_FAILURE);
    }
    if(TightIso>0){

      double this_RelIso = l->RelIso();
      if(UseMini) this_RelIso = l->MiniRelIso();
      double ptcone = l->CalcPtCone(this_RelIso, TightIso);
      l->SetPtCone( ptcone );

    }
    l->SetPassID(electrons[i].PassID(param_.Electron_Tight_ID));

    //cout << "MakeLeptonPointerVector Electron " << param_.Electron_Tight_ID <<" " <<  l->Pt() << endl;

    out.push_back(l);
  }

  std::sort(out.begin(),     out.end(),     PtComparingPtr);

  return out;
}




std::vector<Photon> AnalyzerCore::GetAllPhotons(){

  std::vector<Photon> out;
  for(unsigned int i=0; i<photon_Energy->size(); i++){
    
    Photon pho;

    pho.SetPtEtaPhiE(1., photon_eta->at(i), photon_phi->at(i), photon_Energy->at(i));
    double pho_theta = pho.Theta();
    double pho_pt = photon_Energy->at(i) * TMath::Sin( pho_theta );
    pho.SetPtEtaPhiE( pho_pt, photon_eta->at(i), photon_phi->at(i), photon_Energy->at(i));

    pho.SetSC(photon_scEta->at(i), photon_scPhi->at(i));
    pho.SetRho(Rho);

    pho.SetCutBasedIDVariables(
      photon_Full5x5_SigmaIEtaIEta->at(i),
      photon_HoverE->at(i),
      photon_ChIsoWithEA->at(i),
      photon_NhIsoWithEA->at(i),
      photon_PhIsoWithEA->at(i)
    );
    
    std::vector<bool> ids = {
      photon_passLooseID->at(i),
      photon_passMediumID->at(i),
      photon_passTightID->at(i),
      photon_passMVAID_WP80->at(i),
      photon_passMVAID_WP90->at(i)
     };
    pho.SetPOGIDs(ids);
        
    out.push_back(pho);
    
  }
  return out;
  
}

std::vector<Photon> AnalyzerCore::GetPhotons(TString id, double ptmin, double fetamax){

  std::vector<Photon> photons = GetAllPhotons();
  std::vector<Photon> out;
  for(unsigned int i=0; i<photons.size(); i++){
    if(!( photons.at(i).Pt()>ptmin )){
      continue;
    }
    if(!( fabs(photons.at(i).scEta())<fetamax )){
      continue;
    }
    if(!( photons.at(i).PassID(id) )){
      continue;
    }
    out.push_back( photons.at(i) );
  }
  return out;
}



std::vector<Jet> AnalyzerCore::GetAllJets(bool applySmear){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jet_pt->size(); i++){
    Jet jet;
    jet.SetPtEtaPhiM(jet_pt->at(i), jet_eta->at(i), jet_phi->at(i), jet_m->at(i));

    jet.SetPxUnSmeared(jet.Px());
    jet.SetPyUnSmeared(jet.Py());

    jet.SetJEC(jet_JECL1FastJet->at(i), jet_JECFull->at(i));
    //jet.SetNTracks(jet_vtxNtracks->at(i));
    //==== Jet energy up and down are 1.xx or 0.99, not energy
    jet.SetEnShift( jet_shiftedEnUp->at(i), jet_shiftedEnDown->at(i) );
    if(!IsDATA){
      if(applySmear){
  jet *= jet_smearedRes->at(i);
  jet.SetIsSmeared(true);
      }
      jet.SetResShift( jet_smearedResUp->at(i)/jet_smearedRes->at(i), jet_smearedResDown->at(i)/jet_smearedRes->at(i) );
      jet.SetRes(jet_smearedRes->at(i));
      jet.SetGenFlavours(jet_partonFlavour->at(i), jet_hadronFlavour->at(i));
      jet.SetGenHFHadronMatcher(jet_GenHFHadronMatcher_flavour->at(i),jet_GenHFHadronMatcher_origin->at(i));
    }
    jet.SetBJetNNCorrection(jet_bJetNN_corr->at(i),jet_bJetNN_res->at(i));
    jet.SetCJetNNCorrection(jet_cJetNN_corr->at(i),jet_cJetNN_res->at(i));
    jet.SetCharge(jet_charge->at(i));

    jet.SetArea(jet_area->at(i));
    std::vector<double> tvs = {
      jet_DeepCSV->at(i),
      jet_DeepCSV_CvsL->at(i),
      jet_DeepCSV_CvsB->at(i),
      jet_DeepJet->at(i),
      jet_DeepJet_CvsL->at(i),
      jet_DeepJet_CvsB->at(i),
    };
    jet.SetTaggerResults(tvs);
    jet.SetEnergyFractions(jet_chargedHadronEnergyFraction->at(i), jet_neutralHadronEnergyFraction->at(i), jet_neutralEmEnergyFraction->at(i), jet_chargedEmEnergyFraction->at(i), jet_muonEnergyFraction->at(i));
    jet.SetMultiplicities(jet_chargedMultiplicity->at(i), jet_neutralMultiplicity->at(i));
    jet.SetPileupJetId(jet_PileupJetId->at(i));
    jet.SetTightJetID(jet_tightJetID->at(i));
    jet.SetTightLepVetoJetID(jet_tightLepVetoJetID->at(i));
    
    out.push_back(jet);
  }

  //if(PtOrderObj)  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Jet> AnalyzerCore::GetJets(TString ID, double ptmin, double fetamax){

  std::vector<Jet> jets = All_Jets;

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()>ptmin )){
      continue;
    }
    if(!( fabs(jets.at(i).Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(jets.at(i).Eta()) << ", cut = " << fetamax << endl;                                           
      continue;
    }
    if(!( jets.at(i).PassID(ID) )){
      //cout << "Fail ID" << endl;                                                                                                        
      continue;
    }
    out.push_back( jets.at(i) );
  }

  
  
  std::sort(out.begin(),       out.end(),        PtComparing);
  
  return out;
  
}
    

std::vector<Jet> AnalyzerCore::GetJets(AnalyzerParameter param){

  return GetJets(param, param.Jet_ID, param.Jet_MinPt, param.Jet_MaxEta);

}
std::vector<Jet> AnalyzerCore::GetJets(AnalyzerParameter param,TString id, double ptmin, double fetamax){

  std::vector<Jet> jets_uncorr = All_Jets;
  std::vector<Jet> jets;
  if(param.syst_ == AnalyzerParameter::JetEnUp)            jets    = ScaleJets( jets_uncorr, +1 );
  else if(param.syst_ == AnalyzerParameter::JetEnDown)     jets    = ScaleJets( jets_uncorr, -1 );
  else if(param.syst_ == AnalyzerParameter::JetResUp)      jets    = SmearJets(jets_uncorr, +1 );
  else if(param.syst_ == AnalyzerParameter::JetResDown)    jets    = SmearJets(jets_uncorr, -1 );
  else jets =jets_uncorr;

  
  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()> ptmin )){
      //cout << "Fail Pt : pt = " << jets.at(i).Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(jets.at(i).Eta() )< fetamax )){
      //cout << "Fail Eta : eta = " << fabs(jets.at(i).Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( jets.at(i).PassID( id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back( jets.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<FatJet> AnalyzerCore::GetAllFatJets(){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<fatjet_pt->size(); i++){
    FatJet jet;
    jet.SetPtEtaPhiM(fatjet_pt->at(i), fatjet_eta->at(i), fatjet_phi->at(i), fatjet_m->at(i));
    jet.SetPxUnSmeared(jet.Px());
    jet.SetPyUnSmeared(jet.Py());
    jet.SetEnShift( fatjet_shiftedEnUp->at(i), fatjet_shiftedEnDown->at(i) );
    if(!IsDATA){
      jet *= fatjet_smearedRes->at(i);
      jet.SetResShift( fatjet_smearedResUp->at(i)/fatjet_smearedRes->at(i), fatjet_smearedResDown->at(i)/fatjet_smearedRes->at(i) );
    }
    jet.SetCharge(fatjet_charge->at(i));

    jet.SetArea(fatjet_area->at(i));
    jet.SetGenFlavours(fatjet_partonFlavour->at(i), fatjet_hadronFlavour->at(i));
    std::vector<double> tvs = {
      fatjet_DeepCSV->at(i),
      fatjet_DeepCSV_CvsL->at(i),
      fatjet_DeepCSV_CvsB->at(i),
      fatjet_particleNet_TvsQCD->at(i),
      fatjet_particleNet_WvsQCD->at(i),
      fatjet_particleNet_ZvsQCD->at(i),
      fatjet_particleNet_HbbvsQCD->at(i),
      fatjet_particleNet_HccvsQCD->at(i),
      fatjet_particleNet_H4qvsQCD->at(i),
      fatjet_particleNet_QCD->at(i),
      fatjet_particleNetMD_Xbb->at(i),
      fatjet_particleNetMD_Xcc->at(i),
      fatjet_particleNetMD_Xqq->at(i),
      fatjet_particleNetMD_QCD->at(i),
    };
    jet.SetTaggerResults(tvs);
    jet.SetEnergyFractions(fatjet_chargedHadronEnergyFraction->at(i), fatjet_neutralHadronEnergyFraction->at(i), fatjet_neutralEmEnergyFraction->at(i), fatjet_chargedEmEnergyFraction->at(i), fatjet_muonEnergyFraction->at(i));
    jet.SetMultiplicities(fatjet_chargedMultiplicity->at(i), fatjet_neutralMultiplicity->at(i));
    jet.SetLSF(fatjet_LSF->at(i), fatjet_LSFlep_PID->at(i));
    jet.SetTightJetID(fatjet_tightJetID->at(i));
    jet.SetTightLepVetoJetID(fatjet_tightLepVetoJetID->at(i));
    jet.SetPuppiTaus(fatjet_puppi_tau1->at(i), fatjet_puppi_tau2->at(i), fatjet_puppi_tau3->at(i), fatjet_puppi_tau4->at(i));
    jet.SetSDMass(fatjet_softdropmass->at(i));

    out.push_back(jet);
  }

  //if(PtOrderObj)  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<FatJet> AnalyzerCore::GetFatJets(TString id, double ptmin, double fetamax){

  std::vector<FatJet> jets = All_FatJets;
  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << jets.at(i).Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(jets.at(i).Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(jets.at(i).Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( jets.at(i).PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back( jets.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);
  return out;

}

std::vector<FatJet> AnalyzerCore::GetFatJets(AnalyzerParameter param){
  
  return GetFatJets(param, param.FatJet_ID, param.FatJet_MinPt, param.FatJet_MaxEta);

}
std::vector<FatJet> AnalyzerCore::GetFatJets(AnalyzerParameter param,TString id, double ptmin, double fetamax){

  std::vector<FatJet> jets_pc = puppiCorr->Correct(All_FatJets);
  std::vector<FatJet> jets;
  if(param.syst_ == AnalyzerParameter::JetEnUp)            jets    = ScaleFatJets( jets_pc, +1 );
  else if(param.syst_ == AnalyzerParameter::JetEnDown)     jets    = ScaleFatJets( jets_pc, -1 );
  else if(param.syst_ == AnalyzerParameter::JetResUp)      jets    = SmearFatJets(jets_pc, +1 );
  else if(param.syst_ == AnalyzerParameter::JetResDown)    jets    = SmearFatJets(jets_pc, -1 );
  else if(param.syst_ == AnalyzerParameter::JetMassUp)     jets    = ScaleSDMassFatJets( jets_pc, +1 );
  else if(param.syst_ == AnalyzerParameter::JetMassDown)   jets    = ScaleSDMassFatJets( jets_pc, -1 );
  else if(param.syst_ == AnalyzerParameter::JetMassSmearUp)     jets    = SmearSDMassFatJets( jets_pc, +1 );
  else if(param.syst_ == AnalyzerParameter::JetMassSmearDown)   jets    = SmearSDMassFatJets( jets_pc, -1 );
  else jets = jets_pc;


  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()>  ptmin )){
      //cout << "Fail Pt : pt = " << jets.at(i).Pt() << ", cut = " << ptmin << endl;                                                      
      continue;
    }
    if(!( fabs(jets.at(i).Eta())< fetamax )){
      //cout << "Fail Eta : eta = " << fabs(jets.at(i).Eta()) << ", cut = " << fetamax << endl;                                           
      continue;
    }
    if(!( jets.at(i).PassID(id) )){
      //cout << "Fail ID" << endl;                                                                                                        
      continue;
    }
    out.push_back( jets.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);
  return out;

}


std::vector<Gen> AnalyzerCore::GetGens(){

  std::vector<Gen> out;
  if(IsDATA) return out;

  for(unsigned int i=0; i<gen_pt->size(); i++){

    Gen gen;

    gen.SetIsEmpty(false);
    gen.SetPtEtaPhiM(gen_pt->at(i), gen_eta->at(i), gen_phi->at(i), gen_mass->at(i));
    gen.SetCharge(gen_charge->at(i));
    gen.SetIndexPIDStatus(i, gen_PID->at(i), gen_status->at(i));
    gen.SetMother(gen_mother_index->at(i));
    gen.SetGenStatusFlag_isPrompt( gen_isPrompt->at(i) );
    gen.SetGenStatusFlag_isPromptFinalState( gen_isPromptFinalState->at(i) );
    gen.SetGenStatusFlag_isTauDecayProduct( gen_isTauDecayProduct->at(i) );
    gen.SetGenStatusFlag_isPromptTauDecayProduct( gen_isPromptTauDecayProduct->at(i) );
    gen.SetGenStatusFlag_isDirectPromptTauDecayProductFinalState( gen_isDirectPromptTauDecayProductFinalState->at(i) );
    gen.SetGenStatusFlag_isHardProcess( gen_isHardProcess->at(i) );
    gen.SetGenStatusFlag_isLastCopy( gen_isLastCopy->at(i) );
    gen.SetGenStatusFlag_isLastCopyBeforeFSR( gen_isLastCopyBeforeFSR->at(i) );
    gen.SetGenStatusFlag_isPromptDecayed( gen_isPromptDecayed->at(i) );
    gen.SetGenStatusFlag_isDecayedLeptonHadron( gen_isDecayedLeptonHadron->at(i) );
    gen.SetGenStatusFlag_fromHardProcessBeforeFSR( gen_fromHardProcessBeforeFSR->at(i) );
    gen.SetGenStatusFlag_fromHardProcessDecayed( gen_fromHardProcessDecayed->at(i) );
    gen.SetGenStatusFlag_fromHardProcessFinalState( gen_fromHardProcessFinalState->at(i) );
    gen.SetGenStatusFlag_isMostlyLikePythia6Status3( gen_isMostlyLikePythia6Status3->at(i) );

    out.push_back(gen);

  }

  return out;

}

std::vector<LHE> AnalyzerCore::GetLHEs(){

  std::vector<LHE> out;
  if(IsDATA) return out;

  for(unsigned int i=0; i<LHE_Px->size(); i++){

    LHE lhe;

    lhe.SetPxPyPzE(LHE_Px->at(i), LHE_Py->at(i), LHE_Pz->at(i), LHE_E->at(i));
    lhe.SetIndexIDStatus(i, LHE_ID->at(i), LHE_Status->at(i));

    out.push_back(lhe);

  }

  return out;

}

std::vector<Muon> AnalyzerCore::UseTunePMuon(const std::vector<Muon>& muons){

  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    //==== muons is a const vector. So in this function, we have to copy the elements like below
    Muon this_muon=muons.at(i);

    Particle this_tunep4 = this_muon.TuneP4();

    //==== Momentum scaling
    //==== 1) if tuneP Pt < 200 -> Rochester
    //==== 2) if tuneP pt >= 200 -> Generalized Endpoint

    double new_pt( this_tunep4.Pt() ), new_pt_up( this_tunep4.Pt() ), new_pt_down( this_tunep4.Pt() );
    if(this_tunep4.Pt()<200){

      //==== 19/03/24 (jskim) : For 99% of the muons, MiniAODPt and TunePPt are same
      //==== we can just use MiniAODPt * RochesterCorrection, multiplied by (TuneP Pt)/(MiniAODPt)
      double TunePOverPt = this_tunep4.Pt() / this_muon.MiniAODPt();
      new_pt      = TunePOverPt * this_muon.Pt(); // this_muon.Pt() = MiniAODPt * RochesterCorrection
      new_pt_up   = TunePOverPt * this_muon.MomentumShift(+1);
      new_pt_down = TunePOverPt * this_muon.MomentumShift(-1);

/*
      cout << "## Rochester ##" << endl;
      cout << "this_muon.MiniAODPt() = " << this_muon.MiniAODPt() << endl;
      cout << "this_muon.MiniAODTunePPt() = " << this_muon.MiniAODTunePPt() << endl;
      cout << "new_pt = " << new_pt << endl;
      cout << "new_pt_up = " << new_pt_up << endl;
      cout << "new_pt_down = " << new_pt_down << endl;
*/

    }
    else{

      //==== Unlike rochester, GE method should be only applied to MC

      if(!IsDATA){

        //==== ScaledPts defined in GeneralizedEndpointPt.h ..

        ScaledPts ptvalues;
        //==== TODO FIXME
        //==== 19/09/02 : There is no GEScaleSyst map for 2016
        if(DataYear==2016) ptvalues = muonGE->GeneralizedEndpointPt(this_tunep4.Pt(), this_tunep4.Charge(), this_tunep4.Eta(), this_tunep4.Phi()*180./M_PI, event);
        else ptvalues = muonGEScaleSyst->GEPt(DataYear, this_tunep4.Pt(), this_tunep4.Eta(), this_tunep4.Phi(), this_tunep4.Charge());

        new_pt = ptvalues.ScaledPt;
        //==== Mode == 1 : Kappa up
        //==== Mode == 2 : Kappa down
        new_pt_up = ptvalues.ScaeldPt_Up;
        new_pt_down = ptvalues.ScaeldPt_Down;

/*
        cout << "## GeneralizedEndpointPt ##" << endl;
        cout << "old_pt = " << this_tunep4.Pt() << endl;
        cout << "new_pt = " << new_pt << endl;
        cout << "new_pt_up = " << new_pt_up << endl;
        cout << "new_pt_down = " << new_pt_down << endl;
*/

      }

    }

    //==== Scale the pt
    this_muon.SetPtEtaPhiM( new_pt, this_tunep4.Eta(), this_tunep4.Phi(), this_tunep4.M() );
    this_muon.SetMomentumScaleUpDown(new_pt_up,new_pt_down);
    this_muon.SetCharge( this_tunep4.Charge() );
    this_muon.SetMiniAODPt( this_muon.MiniAODTunePPt() );


/*
    cout << "@@@@ TuneP @@@@" << endl;
    cout << "this_muon.Pt() = " << this_muon.Pt() << endl;
    cout << "this_muon.MiniAODPt() = " << this_muon.MiniAODPt() << endl;
    cout << "this_muon.MiniAODTunePPt() = " << this_muon.MiniAODTunePPt() << endl;
    cout << "this_muon.MomentumShift(0) = " << this_muon.MomentumShift(0) << endl;
    cout << "this_muon.MomentumShift(+1) = " << this_muon.MomentumShift(+1) << endl;
    cout << "this_muon.MomentumShift(-1) = " << this_muon.MomentumShift(-1) << endl;
*/

    out.push_back(this_muon);
  }

  std::sort(out.begin(),       out.end(),        PtComparing);
 
  return out;

}

std::vector<Muon> AnalyzerCore::SelectMuons(const std::vector<Muon>& muons, TString id, double ptmin, double fetamax){

  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    if(!( muons.at(i).Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << muons.at(i).Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(muons.at(i).Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(muons.at(i).Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( muons.at(i).PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back( muons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);
  
  return out;

}

std::vector<Electron> AnalyzerCore::SelectElectrons(const std::vector<Electron>& electrons, TString id, double ptmin, double fetamax, bool vetoHEM){

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    if(!( electrons.at(i).Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << electrons.at(i).Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(electrons.at(i).scEta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(electrons.at(i).scEta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( electrons.at(i).PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    if(vetoHEM){
      if ( FindHEMElectron (electrons.at(i)) ){
        continue;
      }
    }

    out.push_back(electrons.at(i));
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


std::vector<Tau> AnalyzerCore::SelectTaus(const std::vector<Tau>& taus, TString id, double ptmin, double fetamax){

  std::vector<Tau> out;
  for(unsigned int i=0; i<taus.size(); i++){
    if(!( taus.at(i).Pt()>ptmin )){

      continue;
    }
    if(!( fabs(taus.at(i).Eta())<fetamax )){

      continue;
    }
    if(!( taus.at(i).PassID(id) )){
      continue;
    }
    out.push_back( taus.at(i) );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Jet> AnalyzerCore::SelectJets(const std::vector<Jet>& jets, TString id, double ptmin, double fetamax){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << jets.at(i).Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(jets.at(i).Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(jets.at(i).Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( jets.at(i).PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back( jets.at(i) );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<FatJet> AnalyzerCore::SelectFatJets(const std::vector<FatJet>& jets, TString id, double ptmin, double fetamax){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()>ptmin )){
      //cout << "Fail Pt : pt = " << jets.at(i).Pt() << ", cut = " << ptmin << endl;
      continue;
    }
    if(!( fabs(jets.at(i).Eta())<fetamax )){
      //cout << "Fail Eta : eta = " << fabs(jets.at(i).Eta()) << ", cut = " << fetamax << endl;
      continue;
    }
    if(!( jets.at(i).PassID(id) )){
      //cout << "Fail ID" << endl;
      continue;
    }
    out.push_back( jets.at(i) );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


double AnalyzerCore::GetEventFatJetSF(vector<FatJet> fatjets, TString label, int dir){

  double FatJetTau21_SF(1);
  for (auto ifj : fatjets){
    FatJetTau21_SF*=GetFatJetSF(ifj, label,dir);
  }
  return FatJetTau21_SF;

}

double AnalyzerCore::GetFatJetSF(FatJet fatjet, TString tag,  int dir){

  if(IsDATA) return 1.;
  double fsys = -1;
  if(dir > 0) fsys =1;
  if(dir==0) fsys=0.;


  double loose_sf(1.);
  if(tag.Contains("HP")){
    if(DataYear==2016) loose_sf = 0.99  + fsys*0.11;
    if(DataYear==2017) loose_sf = 0.974 + fsys*0.029;
    if(DataYear==2018) loose_sf = 0.980 + fsys*0.019;
    return loose_sf;
  }
  else if(tag.Contains("LP")){
    
    if(DataYear==2016) loose_sf = 1.03 + fsys*0.14;

    else if(DataYear==2017) {
      if (fatjet.PuppiTau21() < 0.45) loose_sf = 0.974 + fsys*0.029;
      else if (fatjet.PuppiTau21() < 0.75)    loose_sf = 1.136 + fsys*0.162;
    }

    else if(DataYear==2018) {
      if (fatjet.PuppiTau21() <0.45) loose_sf = 0.980 + fsys*0.019;
      else if (fatjet.PuppiTau21() < 0.75)  loose_sf = 1.20 + fsys*0.194;
    }
    return loose_sf;
  }

  return 1.;

}

double  AnalyzerCore::GetBJetSF(AnalyzerParameter param,vector<Jet> jets, JetTagging::Parameters jtp){
  
  if(IsData) return 1.;
  string syst = "";
  if(param.syst_ == AnalyzerParameter::BTagSFHTagUp) syst="SystHTagUp";
  else if (param.syst_ == AnalyzerParameter::BTagSFHTagDown) syst="SystHTagDown";
  else if (param.syst_ == AnalyzerParameter::BTagSFLTagUp) syst="SystLTagUp";
  else if (param.syst_ == AnalyzerParameter::BTagSFLTagDown) syst="SystLTagDown";
  
  return mcCorr->GetBTaggingReweight_1a(jets, jtp, syst);
}

vector<Jet>   AnalyzerCore::SelectBJets(AnalyzerParameter param,vector<Jet> jetColl, JetTagging::Parameters jtp){

  vector<Jet> output_jets;

  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    if( jetColl[ijet].GetTaggerResult(jtp.j_Tagger) <= mcCorr->GetJetTaggingCutValue(jtp.j_Tagger, jtp.j_WP) ) continue;  
    output_jets.push_back( jetColl.at(ijet) );
  }
  std::sort(output_jets.begin(),       output_jets.end(),        PtComparing);
  return output_jets;

}

vector<Jet>   AnalyzerCore::SelectLJets(AnalyzerParameter param,vector<Jet> jetColl, JetTagging::Parameters jtp){

  vector<Jet> output_jets;

  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    if( jetColl[ijet].GetTaggerResult(jtp.j_Tagger) > mcCorr->GetJetTaggingCutValue(jtp.j_Tagger, jtp.j_WP) ) continue;
    output_jets.push_back( jetColl.at(ijet) );
  }
  std::sort(output_jets.begin(),       output_jets.end(),        PtComparing);
  return output_jets;

}



vector<Jet>   AnalyzerCore::SelectAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean, TString pu_tag,std::vector<Lepton *> leps_veto,  vector<FatJet> fatjets){

  vector<Jet> output_jets;
  for(unsigned int ijet =0; ijet < jets.size(); ijet++){
    bool jetok=true;

    if(fabs(jets[ijet].Eta()) > eta_cut) continue;
    if(jets[ijet].Pt() < pt_cut)continue;

    for(auto ilep : leps_veto){
      if(ilep->DeltaR(jets[ijet]) < dr_lep_clean) jetok = false;
    }

    for(unsigned int ifjet =0; ifjet < fatjets.size(); ifjet++){
      if(jets[ijet].DeltaR(fatjets[ifjet]) <dr_ak8_clean) jetok = false;
    }

    if(lepton_cleaning&&!jetok) continue;
    if(pu_tag=="")output_jets.push_back(jets[ijet]);
    else {
      if(jets[ijet].PassPileupMVA(pu_tag,GetEra())) output_jets.push_back(jets[ijet]);
    }
  }

  std::sort(output_jets.begin(),       output_jets.end(),        PtComparing);

  return output_jets;

}



vector<Jet>   AnalyzerCore::SelectAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean, TString pu_tag, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, vector<FatJet> fatjets){

  vector<Jet> output_jets;
  for(unsigned int ijet =0; ijet < jets.size(); ijet++){
    bool jetok=true;

    if(fabs(jets[ijet].Eta()) > eta_cut) continue;
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

    if(lepton_cleaning&&!jetok) continue;
    if(pu_tag=="")output_jets.push_back(jets[ijet]);
    else if(jets[ijet].PassPileupMVA(pu_tag,GetEra())) output_jets.push_back(jets[ijet]);
  }
  std::sort(output_jets.begin(),       output_jets.end(),        PtComparing);

  return output_jets;
}


double AnalyzerCore::GetJetPileupIDSF(vector<Jet> jets , TString WP, AnalyzerParameter param){

  if(IsData) return 1.;
  if(WP=="") return 1.;
  
  double JPU_W=1.;
  for(auto ij: jets){
    if(param.syst_ == AnalyzerParameter::JetPUIDUp)   JPU_W*= mcCorr->JetPileUpSF(ij, WP,1 );
    else if(param.syst_ == AnalyzerParameter::JetPUIDDown)   JPU_W*= mcCorr->JetPileUpSF(ij, WP,-1 );
    else JPU_W*= mcCorr->JetPileUpSF(ij, WP,0 );
  }

  return JPU_W;
}



vector<FatJet>  AnalyzerCore::SelectAK8Jets(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut, vector<Electron>  veto_electrons, vector<Muon>  veto_muons){

  return SelectAK8Jetsv2(fatjets,pt_cut,eta_cut, lepton_cleaning, dr_lep_clean,apply_tau21, tau21_cut,apply_masscut,sdmass_lower_cut,sdmass_upper_cut, "", veto_electrons,veto_muons);
}

vector<FatJet>  AnalyzerCore::SelectAK8Jetsv2(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut, TString  tagger,  vector<Electron>  veto_electrons, vector<Muon>  veto_muons){


  vector<FatJet> output_fatjets;
  for(unsigned int ijet =0; ijet < fatjets.size(); ijet++){

    bool jetok=true;

    for(unsigned int iel=0 ; iel < veto_electrons.size(); iel++){
      if(fatjets[ijet].DeltaR(veto_electrons[iel]) < dr_lep_clean) jetok = false;
    }

    for(unsigned int iel=0 ; iel < veto_muons.size(); iel++){
      if(fatjets[ijet].DeltaR(veto_muons[iel]) < dr_lep_clean) jetok = false;
    }

    if( tagger != ""){
      if (!fatjets[ijet].PassTagger(JetTagging::StringToTagger(string(tagger)), DataEra)) continue;
    }

    double lower_sd_mass_cut=sdmass_lower_cut;
    double upper_sd_mass_cut=sdmass_upper_cut;
    if(sdmass_lower_cut < 0.){
      lower_sd_mass_cut = 40.;
      upper_sd_mass_cut = 130.;
      if(DataYear==2017){
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
    if(tau21_cut > 0.){
      if(DataYear==2016) tau_21_cut = 0.55;
      if(DataYear==2017) tau_21_cut = 0.75;
      if(DataYear==2018) tau_21_cut = 0.75;
    }
    else{
      if(DataYear==2016) tau_21_cut = 0.35;
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


  std::sort(output_fatjets.begin(),       output_fatjets.end(),        PtComparing);

  return output_fatjets;
}



std::vector<Electron> AnalyzerCore::ScaleElectrons(const std::vector<Electron>& electrons, int sys){

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    //==== electrons is a const vector. So in this function, we have to copy the elements like below
    Electron this_electron = electrons.at(i);

    double this_sf = this_electron.EnShift(sys);
    this_electron.SetPtEtaPhiM( this_electron.Pt() * this_sf, this_electron.Eta(), this_electron.Phi(), this_electron.M() );

    out.push_back( this_electron );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}
std::vector<Electron> AnalyzerCore::SmearElectrons(const std::vector<Electron>& electrons, int sys){

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    //==== electrons is a const vector. So in this function, we have to copy the elements like below
    Electron this_electron = electrons.at(i);

    double this_sf = this_electron.ResShift(sys);
    this_electron.SetPtEtaPhiM( this_electron.Pt() * this_sf, this_electron.Eta(), this_electron.Phi(), this_electron.M() );

    out.push_back( this_electron );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


bool AnalyzerCore::IsHEMIssueRun(){

  if(!IsDATA) return false;

  int RunNumber=run;
  return RunNumber>319077;

}

bool AnalyzerCore::IsHEMIssueReg(Particle& Particle){

  bool IstheRegion=false;
  double eta=Particle.Eta(), phi=Particle.Phi();

  if(eta>-3. && eta<-1.3 && phi>-1.57 && phi<-0.87) IstheRegion=true;

  return IstheRegion;
}



bool AnalyzerCore::IsHEMCRReg(Particle& Particle, TString Option){

  bool IstheRegion=false;
  double eta=Particle.Eta(), phi=Particle.Phi();
  bool SameEta = Option.Contains("SameEta");
  bool SamePhi = Option.Contains("SamePhi");

  if     (SameEta && (eta>-3. && eta<-1.3) && !(phi>-1.57 && phi<-0.87) ) IstheRegion=true;
  else if(SamePhi && (eta< 3. && eta> 1.3) &&  (phi>-1.57 && phi<-0.87) ) IstheRegion=true;

  return IstheRegion;
}



bool AnalyzerCore::FindHEMElectron(Electron electron){

    if (DataYear != 2018) return false;

    if (electron.Eta() < -1.25){
        if((electron.Phi() < -0.82) && (electron.Phi() > -1.62)) return true;
    }

    return false;

}

std::vector<Muon> AnalyzerCore::ScaleMuons(const std::vector<Muon>& muons, int sys){

  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    //==== muons is a const vector. So in this function, we have to copy the elements like below
    Muon this_muon = muons.at(i);

    //==== Even for TuneP muons, MomentumShift() are set correctly from AnalyzerCore::UseTunePMuon()
    //==== So we can just use MomentumShift()

    this_muon.SetPtEtaPhiM( this_muon.MomentumShift(sys), this_muon.Eta(), this_muon.Phi(), this_muon.M() );

    out.push_back(this_muon);

  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Jet> AnalyzerCore::ScaleJets(const std::vector<Jet>& jets, int sys){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below
    Jet this_jet = jets.at(i);

    this_jet *= this_jet.EnShift(sys);

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


std::vector<Jet> AnalyzerCore::ScaleJetsIndividualSource(const std::vector<Jet>& jets, int sys, TString source){

  if(!std::count(JECSources.begin(),JECSources.end(), source)) {
    cout << "[AnalyzerCore::ScaleJetsIndividualSource] source " << source << " was not found" << endl;
    exit(EXIT_FAILURE);
  }

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){

    Jet this_jet = jets.at(i);

    double get_shift = GetJECUncertainty(source , "AK4PFchs",this_jet.Eta(),this_jet.Pt(), sys);

    this_jet *= get_shift;

    out.push_back( this_jet );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}



std::vector<Jet> AnalyzerCore::SmearJets(const std::vector<Jet>& jets, int sys){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below
    Jet this_jet = jets.at(i);

    this_jet *= this_jet.ResShift(sys);

    out.push_back( this_jet );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<FatJet> AnalyzerCore::ScaleFatJets(const std::vector<FatJet>& jets, int sys){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below
    FatJet this_jet = jets.at(i);

    this_jet *= this_jet.EnShift(sys);

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}
std::vector<FatJet> AnalyzerCore::SmearFatJets(const std::vector<FatJet>& jets, int sys){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below
    FatJet this_jet = jets.at(i);

    this_jet *= this_jet.ResShift(sys);

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}
//Fatjet SDMass systematics (https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging#2016%20scale%20factors%20and%20correctio)
std::vector<FatJet> AnalyzerCore::ScaleSDMassFatJets(const std::vector<FatJet>& jets, int sys){
  
  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below
    FatJet this_jet = jets.at(i);

    double current_SDMass (1.);    
    if(DataYear == 2016) current_SDMass = this_jet.SDMass() * (1. + double(sys) * 0.0094 );
    if(DataYear == 2017) current_SDMass = this_jet.SDMass() * (0.982 + double(sys) * 0.004 );
    if(DataYear == 2018) current_SDMass = this_jet.SDMass() * (0.982 + double(sys) * 0.004 ); // JH : FIXME but 2018 not updated. which WP? SF?

    this_jet.SetSDMass( current_SDMass );
    
    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;
  
}
std::vector<FatJet> AnalyzerCore::SmearSDMassFatJets(const std::vector<FatJet>& jets, int sys){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below
    FatJet this_jet = jets.at(i);
    double current_SDMass (1.);
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetWtagging#2017_scale_factors_and_correctio
    if(DataYear == 2016) current_SDMass = this_jet.SDMass() * (1. + double(sys) * 0.20 );
    if(DataYear == 2017) current_SDMass = this_jet.SDMass() * (1.09 + double(sys) * 0.05 );
    if(DataYear == 2018) current_SDMass = this_jet.SDMass() * (1.09 + double(sys) * 0.05 ); // JH : FIXME same as the above
    
    this_jet.SetSDMass( current_SDMass );

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

bool AnalyzerCore::PassMETFilter(){

  //==== https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#UL_data

  if(!Flag_goodVertices) return false;
  if(!IsFastSim){
    if(!Flag_globalSuperTightHalo2016Filter) return false;
  }
  if(!Flag_HBHENoiseFilter) return false;
  if(!Flag_HBHENoiseIsoFilter) return false;
  if(!Flag_EcalDeadCellTriggerPrimitiveFilter) return false;
  if(!Flag_BadPFMuonFilter) return false;
  if(!Flag_BadPFMuonDzFilter) return false;
  if(!Flag_eeBadScFilter) return false;
  if(DataYear>=2017){
    if(!Flag_ecalBadCalibFilter) return false;
  }

  return true;

}

void AnalyzerCore::CompareBDT(TString var1, double a, double b,  int ilep ){

  if( (a/b) != 1 ) cout << "Electron " << ilep << " " << var1 << " " <<  a << "   : " << b <<  "  a/b = " << a/b << endl;
  
  
  return;
}


///======== Weight functions                                                                                                                     



double AnalyzerCore::GetMuonSFEventWeight(std::vector<Muon> muons,AnalyzerParameter param ){

  double this_weight(1.);
  if(!IsDATA){

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;
    
    double SystDir_MuonTriggerSF(0);
    if(param.syst_ == AnalyzerParameter::MuonTriggerSFUp)SystDir_MuonTriggerSF = +1;
    else if(param.syst_ == AnalyzerParameter::MuonTriggerSFDown)SystDir_MuonTriggerSF = -1;

    double this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, param.Muon_Trigger_NameForSF, muons,SystDir_MuonTriggerSF);
    this_weight*=this_trigsf;
    for (auto mu: muons){
      double MiniAODP = sqrt( mu.MiniAODPt() * mu.MiniAODPt() + mu.Pz() * mu.Pz() );
      double this_pt  = mu.MiniAODPt();
      double this_eta = mu.Eta();

      int SystDir_MuonIDSF(0), SystDir_MuonISOSF(0), SystDir_MuonRecoSF (0);
      if(param.syst_ == AnalyzerParameter::MuonRecoSFUp)SystDir_MuonRecoSF = +1;
      else if(param.syst_ == AnalyzerParameter::MuonRecoSFDown)SystDir_MuonRecoSF = -1;
      else if(param.syst_ == AnalyzerParameter::MuonIDSFUp)  SystDir_MuonIDSF = +1;
      else if(param.syst_ == AnalyzerParameter::MuonIDSFDown)  SystDir_MuonIDSF = -1;
      else if(param.syst_ == AnalyzerParameter::MuonISOSFUp) SystDir_MuonISOSF  = +1;
      else if(param.syst_ == AnalyzerParameter::MuonISOSFDown) SystDir_MuonISOSF  = -1;
      

      double this_idsf   = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  this_eta, this_pt,SystDir_MuonIDSF);
      double this_isosf  = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, this_eta, this_pt,SystDir_MuonISOSF);

      this_weight *= this_idsf*this_isosf;
      if(param.DEBUG) cout << "GetMuonSFEventWeight this_idsf=" << this_idsf << " this_isosf=" << this_isosf << " this_trigsf=" << this_trigsf << endl;

      double reco_pt = (param.Muon_RECO_SF_Key  == "HighPtMuonRecoSF") ?  MiniAODP : this_pt;

      double this_recosf = mcCorr->MuonReco_SF(param.Muon_RECO_SF_Key, this_eta, reco_pt,SystDir_MuonRecoSF);

      this_weight *= this_recosf;          

      FillWeightHist(param.Name+"/RecoMuWeight_"+param.Name,this_recosf); 
      FillWeightHist(param.Name+"/IDMuWeight_"+param.Name,this_idsf);
      FillWeightHist(param.Name+"/ISOMuWeight_"+param.Name,this_isosf);
      FillWeightHist(param.Name+"/TrigMuWeight_"+param.Name,this_trigsf);

    }// end of muon loop  

  }// end of MC req.                                                                                                                             

  bool apply_tracking_SF = true;
  double MuonTrackineSF(1.);
  
  //https://twiki.cern.ch/twiki/bin/view/CMSPublic/TrackingPOGResultsRun2Legacy#Muon_tracking_performance_in_AN2
  // https://cds.cern.ch/record/2724492/files/DP2020_035.pdf
  if (apply_tracking_SF){
    if(DataEra=="2016preVFP"){
      for(auto im : muons) {
        if(fabs(im.Eta()) > 1.5) MuonTrackineSF*= 0.99;
  
      } 
    }
    if(DataEra=="2016postVFP"){
      for(auto im : muons) {
  if(fabs(im.Eta()) > 1.) MuonTrackineSF*= 0.995;
      }
    }
    if(DataYear==2017){
      for(auto im : muons) {
        if(im.Pt() > 60. && im.Pt() < 120) MuonTrackineSF*= 0.995;
      }
    }
    
    if(DataYear==2018){
      for(auto im : muons) {
  if(im.Pt() > 60. && im.Pt() < 120) MuonTrackineSF*= 0.997;
      }
    }
    this_weight=this_weight*MuonTrackineSF;
    
    FillWeightHist(param.Name+"/TrackerMuWeight_"+param.Name,MuonTrackineSF);
  }

  FillWeightHist(param.Name+"/FullMuWeight_"+param.Name,this_weight);


  return this_weight;

}


double AnalyzerCore::GetLeptonSFEventWeight(std::vector<Lepton *> leps, AnalyzerParameter param ){

  double this_weight(1.);
  if(!IsDATA){

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

    int SystDir_MuonIDSF(0), SystDir_MuonISOSF(0), SystDir_MuonRecoSF (0), SystDir_MuonTriggerSF(0);
    if(param.syst_ == AnalyzerParameter::MuonRecoSFUp)SystDir_MuonRecoSF = +1;
    else if(param.syst_ == AnalyzerParameter::MuonRecoSFDown)SystDir_MuonRecoSF = -1;
    else if(param.syst_ == AnalyzerParameter::MuonIDSFUp)  SystDir_MuonIDSF = +1;
    else if(param.syst_ == AnalyzerParameter::MuonIDSFDown)  SystDir_MuonIDSF = -1;
    else if(param.syst_ == AnalyzerParameter::MuonISOSFUp) SystDir_MuonISOSF  = +1;
    else if(param.syst_ == AnalyzerParameter::MuonISOSFDown) SystDir_MuonISOSF  = -1;
    else if(param.syst_ == AnalyzerParameter::MuonTriggerSFUp)SystDir_MuonTriggerSF = +1;
    else if(param.syst_ == AnalyzerParameter::MuonTriggerSFDown)SystDir_MuonTriggerSF = -1;

    int SystDir_ElectronIDSF(0),SystDir_ElectronRecoSF (0), SystDir_ElectronTriggerSF(0);
    if(param.syst_ == AnalyzerParameter::ElectronRecoSFUp)SystDir_ElectronRecoSF = +1;
    else if(param.syst_ == AnalyzerParameter::ElectronRecoSFDown)SystDir_ElectronRecoSF = -1;
    else if(param.syst_ == AnalyzerParameter::ElectronIDSFUp)  SystDir_ElectronIDSF = +1;
    else if(param.syst_ == AnalyzerParameter::ElectronIDSFDown)  SystDir_ElectronIDSF = -1;
    else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFUp)SystDir_ElectronTriggerSF = +1;
    else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFDown)SystDir_ElectronTriggerSF = -1;






    for (auto lep: leps){
      if(lep->LeptonFlavour() == Lepton::ELECTRON){
  double this_recosf  = mcCorr->ElectronReco_SF(lep->Eta(),lep->Pt(), SystDir_ElectronRecoSF);
  double this_idsf    = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, lep->Eta(), lep->Pt(), SystDir_ElectronIDSF);
  
  this_weight *= this_recosf*this_idsf;

  double this_trigsf = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, param.Electron_Trigger_NameForSF,leps, SystDir_ElectronTriggerSF);

  this_weight  *= this_trigsf;

      }
      if(lep->LeptonFlavour() == Lepton::MUON){
  
  double this_pt  = lep->Pt();
  double this_eta = lep->Eta();


  double this_idsf   = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  this_eta, this_pt,SystDir_MuonIDSF);
  double this_isosf  = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, this_eta, this_pt,SystDir_MuonISOSF);
  double this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, param.Muon_Trigger_NameForSF, leps,SystDir_MuonTriggerSF);

  this_weight *= this_idsf*this_isosf*this_trigsf;

  //double reco_pt = (param.Muon_RECO_SF_Key  == "HighPtMuonRecoSF") ?  MiniAODP : this_pt;
  double reco_pt =this_pt;
  double this_recosf = mcCorr->MuonReco_SF(param.Muon_RECO_SF_Key, this_eta, reco_pt,SystDir_MuonRecoSF);

  this_weight *= this_recosf;

      }
      
    }

  }
  return this_weight;


}
double AnalyzerCore::GetElectronSFEventWeight(std::vector<Electron> electrons, AnalyzerParameter param ){

  double this_weight(1.);
  if(!IsDATA){

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

    
   int SystDir_ElectronIDSF(0), SystDir_ElectronRecoSF (0), SystDir_ElectronTriggerSF(0);
    if(param.syst_ == AnalyzerParameter::ElectronRecoSFUp)SystDir_ElectronRecoSF = +1;
    else if(param.syst_ == AnalyzerParameter::ElectronRecoSFDown)SystDir_ElectronRecoSF = -1;
    else if(param.syst_ == AnalyzerParameter::ElectronIDSFUp)  SystDir_ElectronIDSF = +1;
    else if(param.syst_ == AnalyzerParameter::ElectronIDSFDown)  SystDir_ElectronIDSF = -1;
    else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFUp)SystDir_ElectronTriggerSF = +1;
    else if(param.syst_ == AnalyzerParameter::ElectronTriggerSFDown)SystDir_ElectronTriggerSF = -1;



    for (auto el: electrons){

      double pt = (el.Pt() < 15) ? 16 : el.Pt();
      double this_recosf  = mcCorr->ElectronReco_SF(el.scEta(),pt, SystDir_ElectronRecoSF);
      double this_idsf    = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, el.scEta(), pt, SystDir_ElectronIDSF);

      this_weight *= this_recosf*this_idsf;
      
      if(param.WriteOutVerbose == 0){
  FillWeightHist(param.Name+"/el_reco_sf_"+param.Name, this_recosf);
  FillWeightHist(param.Name+"/el_id_sf_"+param.Name, this_idsf);
      }
    }
    double this_trigsf = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, param.Electron_Trigger_NameForSF, electrons,SystDir_ElectronTriggerSF);

    this_weight  *= this_trigsf;

  }
  return this_weight;

}



double AnalyzerCore::GetFakeRateEl(double eta, double pt, AnalyzerParameter param){

  double scale=1.;

  return fakeEst->GetElectronFakeRate(param.Electron_Tight_ID,param.Electron_FR_Key,eta, pt, 0)*scale;


}
double AnalyzerCore::GetFakeRateM(double eta, double pt, AnalyzerParameter param){

  double scale=1.;

  return fakeEst->GetMuonFakeRate(param.Muon_Tight_ID, param.Muon_FR_Key ,eta, pt, 0)*scale;

}



double AnalyzerCore::GetFakeWeight(std::vector<Lepton *> leps, AnalyzerParameter _param, bool apply_pr){

  //if(!IsData) return 1.; FIXME need this to perform the closure test
  
  double this_weight = -1.;
  if(leps.size() == 1){
    TString fr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_FR_Key : _param.Muon_FR_Key;
    TString pr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_PR_Key : _param.Muon_PR_Key;

    if(!leps[0]->LepIDSet()) {
      cout << "Lepton ID not set" << endl;
      exit(EXIT_FAILURE);
    }
    
    double this_fr1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()); 
    double this_pr1 = 1.;
    //    if(apply_pr) this_pr1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID,pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) :fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) ;
    
    this_weight=  fakeEst->CalculateLepWeight(this_pr1, this_fr1, leps[0]->PassLepID() );

    //    cout << this_weight<< " L"<<endl;

    return this_weight;



    // fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID, pr_key1, fabs(electrons[0].Eta()), electrons[0].Pt());
    // fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key1, fabs(muons[0].Eta()), muons[0].Pt());
  }
  if (leps.size() == 2){

    if(!(leps[0]->LepIDSet() || leps[1]->LepIDSet())) {
      cout << "Lepton ID not set" << endl;
      exit(EXIT_FAILURE);
    }

    TString fr_key1;
    TString fr_key2;
    if(_param.Name.Contains("LFvsHF")){
      fr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_FR_Key : leps[0]->FakeFlavourString()+"_pt_eta_AwayJetPt40";
      fr_key2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_FR_Key : leps[1]->FakeFlavourString()+"_pt_eta_AwayJetPt40";
    }
    else{
      fr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_FR_Key : _param.Muon_FR_Key;
      fr_key2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_FR_Key : _param.Muon_FR_Key;
    } //FIXME tmp test for LFvsHF method
    TString  pr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_PR_Key : _param.Muon_PR_Key;
    TString  pr_key2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_PR_Key : _param.Muon_PR_Key;

    double this_fr1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()); 
    double this_fr2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, fr_key2, fabs(leps[1]->Eta()), leps[1]->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, fr_key2, fabs(leps[1]->Eta()), leps[1]->Pt()); 

    double this_pr1 = 1.;
    //if(apply_pr) this_pr1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID,pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) :fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) ;
    double this_pr2 = 1.;
    //if(apply_pr) this_pr2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID,pr_key2 , fabs(leps[1]->Eta()), leps[1]->Pt()) :fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key2 , fabs(leps[1]->Eta()), leps[1]->Pt()) ;

    TString ID1 =  (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
    TString ID2 =  (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;

    // TMP FIX TO PT 
    //if (leps[1]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->Pt() < 20) this_fr2*= 1.2;
    //if (leps[1]->LeptonFlavour() == Lepton::MUON && leps[1]->Pt() < 20) this_fr2*= 1.1; //JH : optimize this value
    
    this_weight = fakeEst->CalculateDilepWeight(this_pr1,this_fr1, this_pr2, this_fr2, leps[0]->PassLepID(),leps[1]->PassLepID(),0);

    //cout << this_weight << " 2L"<<endl;
    return this_weight;
  }


  else{
    

    double this_weight = -1.;
    vector<double> FRs;

    double this_fr = -999.;
    for(auto lep : leps){

      if( lep->PassLepID() ) continue;
      
      if(_param.Name.Contains("LFvsHF")){
        this_fr =  (lep->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, _param.Electron_FR_Key, fabs(lep->Eta()), lep->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, lep->FakeFlavourString()+"_pt_eta_AwayJetPt40", fabs(lep->Eta()), lep->Pt()); 
      }
      else{
        this_fr =  (lep->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, _param.Electron_FR_Key, fabs(lep->Eta()), lep->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, _param.Muon_FR_Key, fabs(lep->Eta()), lep->Pt()); 
      } //FIXME tmp test for LFvsHF method
      this_weight *= -1.*this_fr/(1.-this_fr);
      FRs.push_back(this_fr);
      
    }
    
    if(FRs.size()==0){
      return 0.;
    }
    else{
      //cout << this_weight<< " ML"<<endl;
      
      return this_weight;
    }
  }
  
  return -999.;
}




double AnalyzerCore::GetCFWeightElectron(std::vector<Lepton* > leps ,  AnalyzerParameter param){

  vector<double> el_pt,el_eta;
  for(auto ilep : leps){
    if(ilep->LeptonFlavour()==Lepton::ELECTRON){
      el_pt.push_back(ilep->Pt());
      el_eta.push_back(ilep->Eta());
    }
  }

  return GetCFWeightElectron(el_pt,el_eta, param);
}

double AnalyzerCore::GetCFWeightElectron(std::vector<Electron> electrons ,  AnalyzerParameter param){

  vector<double> el_pt, el_eta;
  for(auto ilep : electrons){
    el_pt.push_back(ilep.Pt());
    el_eta.push_back(ilep.scEta());
    
  }

  return GetCFWeightElectron(el_pt,el_eta, param);
}


double AnalyzerCore::GetCFWeightElectron(vector<double> el_pt, vector<double> el_eta ,  AnalyzerParameter param){
  //double CFBackgroundEstimator::GetElectronCFRate(TString ID, TString key, double eta, double pt, int sys){                                    

  cfEst->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;


  if(el_pt.size()  != 2) return 1.;

  double el1_cf_rate =   cfEst->GetElectronCFRate2D(param.Electron_Tight_ID,"central",el_eta[0], el_pt[0], 0);
  double el2_cf_rate =   cfEst->GetElectronCFRate2D(param.Electron_Tight_ID,"central",el_eta[1], el_pt[1], 0);


  if((el_eta[0]) < 1.5) el1_cf_rate *= 0.95;
  else el1_cf_rate *= 0.95;
  if((el_eta[1]) < 1.5) el2_cf_rate *= 0.95;
  else el2_cf_rate *= 0.95;

  double cf_weight = (el1_cf_rate / (1.-el1_cf_rate))  + (el2_cf_rate/(1.-el2_cf_rate));
  return cf_weight;
}

double AnalyzerCore::GetCFrates(TString id, double pt, double eta){ //JH

  eta = fabs(eta);
  double x = 1./pt;
  double a, b, c;
  double rate (0.);

  if(DataEra=="2016preVFP"){ // UL MIGRATED
    if(id == "HNTightV2"){
      if(eta < 0.8){
        if(x < 0.004){ a = -6.19041e+00; b = -7.38711e+02; rate = TMath::Exp(a + b*x); }
        else if(x>=0.004 && x<0.038){ a = 1.45276e-06; b = 7.72346e-03; c = -1.45076e-05; rate = a/(x+b)+c; }
        else{ a = 9.71303e-05; b = -0.00208995; rate = a + b*x; }
      }
      else if(eta>=0.8 && eta<1.479){
        if(x < 0.004){ a = -4.73210e+00; b = -4.98697e+02; rate = TMath::Exp(a + b*x); }
        else if(x>=0.004 && x<0.036){ a = 1.08024e-05; b = 4.03858e-03; c = -8.32696e-05; rate = a/(x+b)+c; }
        else{ a = 0.000740277; b = -0.0154686; rate = a + b*x; }
      }
      else{
        //if(x < 0.01){ a = 0.0127778; b = -0.744197; }
        //else if(x>=0.01 && x<0.0205){ a = 0.00725863; b = -0.18864; }
        //else{ a = 0.00417112; b = -0.0371866; }
        a = 3.19899e-05; b = -6.09451e-04; c = 1.50164e-03; rate = a/(x+b)+c;
      }
    }
  }

  else if(DataEra=="2016postVFP"){ // UL MIGRATED
    if(id == "HNTightV2"){
      if(eta < 0.8){
        if(x < 0.002){ a = -5.09610e+00; b = -1.29241e+03; rate = TMath::Exp(a + b*x); }
        else if(x>=0.002 && x<0.022){ a = 3.95560e-07; b = -1.14944e-03; c = 1.83440e-05; rate = a/(x+b)+c; }
        else{ a = 6.34584e-05; b = -0.00131961; rate = a + b*x; }
      }
      else if(eta>=0.8 && eta<1.479){
        if(x < 0.002){ a = -3.55492e+00; b = -1.05267e+03; rate = TMath::Exp(a + b*x); }
        else if(x>=0.002 && x<0.014){ a = 5.04842e-06; b = -2.06530e-04; c = 1.51220e-04; rate = a/(x+b)+c; }
        else{ a = 0.000642187; b = -0.0134713; rate = a + b*x; }
      }
      else{
        //if(x < 0.01){ a = 0.0127778; b = -0.744197; }
        //else if(x>=0.01 && x<0.0205){ a = 0.00725863; b = -0.18864; }
        //else{ a = 0.00417112; b = -0.0371866; }
        a = 4.98575e-05; b = 8.07806e-04; c = 6.91510e-04; rate = a/(x+b)+c;
      }
    }
  }

  else if(DataYear==2017){ //UL MIGRATED
    if(id == "HNTightV2"){
      if(eta < 0.8){ 
        if(x < 0.005){ a = -7.20188e+00; b = -5.35974e+02; rate = TMath::Exp(a + b*x); }
        else if(x>=0.005 && x<0.026){ a = 1.11758e-06; b = 1.20114e-02; c = -1.48319e-05; rate = a/(x+b)+c; }
        else{ a = 2.75774e-05; b = -0.000472906; rate = a + b*x; }
      }
      else if(eta>=0.8 && eta<1.479){ 
        if(x < 0.003){ a = -4.84051e+00; b = -7.80357e+02; rate = TMath::Exp(a + b*x); }
        else if(x>=0.003 && x<0.038){ a = 2.04166e-06; b = -2.92844e-04; c = 4.42685e-06; rate = a/(x+b)+c; }
        else{ a = 0.000126619; b = -0.00189268; rate = a + b*x; }
      }
      else{
        //if(x < 0.01){ a = 0.0127778; b = -0.744197; }
        //else if(x>=0.01 && x<0.0205){ a = 0.00725863; b = -0.18864; }
        //else{ a = 0.00417112; b = -0.0371866; }
        a = 2.12574e-05; b = 4.01600e-04; c = -7.53337e-06; rate = a/(x+b)+c;
      }
    }
  }
  else if(DataYear==2018){ // UL MIGRATED
    if(id == "HNTightV2"){
      if(eta < 0.8){
        if(x < 0.002){ a = -5.73695e+00; b = -1.29458e+03; rate = TMath::Exp(a + b*x); }
        else if(x>=0.002 && x<0.022){ a = 1.85580e-07; b = -1.29739e-03; c = 1.18060e-05; rate = a/(x+b)+c; }
        else{ a = 3.68107e-05; b = -0.000730055; rate = a + b*x; }
      }
      else if(eta>=0.8 && eta<1.479){
        if(x < 0.002){ a = -4.48813e+00; b = -1.06378e+03; rate = TMath::Exp(a + b*x); }
        else if(x>=0.002 && x<0.035){ a = 2.04543e-06; b = -5.01415e-04; c = 1.05875e-05; rate = a/(x+b)+c; }
        else{ a = 0.000119787; b = -0.00166241; rate = a + b*x; }
      }
      else{
        //if(x < 0.01){ a = 0.0127778; b = -0.744197; }
        //else if(x>=0.01 && x<0.0205){ a = 0.00725863; b = -0.18864; }
        //else{ a = 0.00417112; b = -0.0371866; }
        a = 2.06403e-05; b = 4.25782e-04; c = 4.31896e-06; rate = a/(x+b)+c;
      }
    }
  }

  if(rate < 0.) rate = 0.;
  return rate;

}

double AnalyzerCore::GetCFWeightElectron(vector<Lepton *> lepptrs, AnalyzerParameter param, bool applySF, int syst){
  if(!param.Electron_Tight_ID.Contains("HNTight")) return 0.;
  if(lepptrs.size() > 2) return 0.;

  std::vector<Electron> el;
  for(unsigned int i=0; i<lepptrs.size(); i++){
    if(lepptrs.at(i)->LeptonFlavour() == Lepton::ELECTRON){
      Electron *el_tmp = (Electron *)lepptrs.at(i);
      el.push_back(*el_tmp);
    }
  }
  if(el.size()==2 && el.at(0).Charge()*el.at(1).Charge()>0) return 0.;

  std::vector<double> CFrate, CFweight, sf;
  double cfweight = 0.;
  for(unsigned int i=0; i<el.size(); i++){
    CFrate.push_back(GetCFrates(param.Electron_Tight_ID, el.at(i).Pt(), el.at(i).scEta()));
    CFweight.push_back(CFrate.at(i)/(1.-CFrate.at(i)));

    if(applySF){
      if(DataEra=="2016preVFP"){
        if(fabs(el.at(i).scEta()) < 1.479){
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(0.9330 + syst*0.);
        }
        else{
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(1.1681 + syst*0.);
        }
      }
      if(DataEra=="2016postVFP"){
        if(fabs(el.at(i).scEta()) < 1.479){
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(0.9763 + syst*0.);
        }
        else{
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(1.1475 + syst*0.);
        }
      }
      if(DataYear==2017){
        if(fabs(el.at(i).scEta()) < 1.479){
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(1.4395 + syst*0.);
        }
        else{
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(1.3247 + syst*0.);
        }
      }
      if(DataYear==2018){
        if(fabs(el.at(i).scEta()) < 1.479){
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(1.1234 + syst*0.);
        }
        else{
          if(param.Electron_Tight_ID == "HNTightV2") sf.push_back(1.3097 + syst*0.);
        }
      }
    }
    else sf.push_back(1.);

    cfweight += sf.at(i)*CFweight.at(i);
  }

  return cfweight;

}

void AnalyzerCore::SetupLeptonBDTSKFlatV5(){
  
  std::vector<Muon>     AllmuonColl     = All_Muons;
  std::vector<Electron> AllelectronColl = All_Electrons;
  std::vector<Jet>      AK4_JetAllColl  = All_Jets;

  
  for(auto i: AllmuonColl)   {

    float  JetDiscCJ_CvsB = -999;
    float  JetDiscCJ_CvsL = -999;
    float  JetDiscCJ = -999;

    int JetFlavourCJ=-999;
    int IdxMatchJet=-1;
    float mindR1=999.;

    for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
      float dR1=i.DeltaR(AK4_JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
    }
    if(IdxMatchJet!=-1) {
      JetDiscCJ_CvsB   = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsB);
      JetDiscCJ_CvsL    = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsL);
      JetDiscCJ = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
      JetFlavourCJ = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();

    }
    else {
      JetDiscCJ_CvsB=0.4;
      JetDiscCJ_CvsL=0.1;
      JetDiscCJ=0.1;
      JetFlavourCJ=0;
    }
    vmuon_v2_cj_cvsbjetdisc->push_back(JetDiscCJ_CvsB);
    vmuon_v2_cj_cvsljetdisc->push_back(JetDiscCJ_CvsL);
    vmuon_v2_cj_bjetdisc->push_back(JetDiscCJ);
    vmuon_v2_cj_flavour->push_back(JetFlavourCJ);

    vmuon_mva_fake_QCD_LFvsHF_v5->push_back(GetBDTScoreMuon(i,AnalyzerCore::FakeRate,     "BDTGv5_QCD_LFvsHF"));
    vmuon_mva_fake_QCD_HFBvsHFC_v5->push_back(GetBDTScoreMuon(i,AnalyzerCore::FakeRate,   "BDTGv5_QCD_HFBvsHFC"));
    vmuon_mva_fake_QCD_LF1_v5->push_back(GetBDTScoreMuon(i,AnalyzerCore::FakeRate,  "BDTGv5_LF1"));
    vmuon_mva_fake_QCD_LF2_v5->push_back(GetBDTScoreMuon(i,AnalyzerCore::FakeRate,  "BDTGv5_LF2"));

  }


  
  for(auto i: AllelectronColl){

    velectron_mva_fake_ed_v5->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv5"));
    velectron_mva_fakeHFB_v5->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv5_HFB"));
    velectron_mva_fakeHFC_v5->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv5_HFC"));
    velectron_mva_fakeLF_v5->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv5_LF"));

    velectron_mva_conv_ed_v5->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Conv,  "BDTGv5"));

    velectron_mva_cf_ed_v5->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::CF,  "BDTGv5"));
    velectron_mva_cf_ed_v5pt->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::CF,  "BDTGv5Pt"));

    velectron_mva_fake_QCD_LFvsHF_v5->push_back(GetBDTScoreEl(i,AnalyzerCore::FakeRate,    "BDTGv5_QCD_LFvsHF"));
    velectron_mva_fake_QCD_HFBvsHFC_v5->push_back(GetBDTScoreEl(i,AnalyzerCore::FakeRate,  "BDTGv5_QCD_HFBvsHFC"));
    velectron_mva_fake_QCD_LF1_v5->push_back(GetBDTScoreEl(i,AnalyzerCore::FakeRate,  "BDTGv5_LF1"));
    velectron_mva_fake_QCD_LF2_v5->push_back(GetBDTScoreEl(i,AnalyzerCore::FakeRate,  "BDTGv5_LF2"));


    float  JetDiscCJ_CvsB = -999;
    float  JetDiscCJ_CvsL = -999;
    float  JetDiscCJ = -999;

    int JetFlavourCJ=-999;
    int IdxMatchJet=-1;
    float mindR1=999.;

    for(unsigned int ij=0; ij<AK4_JetAllColl.size(); ij++){
      float dR1=i.DeltaR(AK4_JetAllColl.at(ij));
      if(dR1>0.4) continue;
      if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
    }
    if(IdxMatchJet!=-1) {
      JetDiscCJ_CvsB   = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsB);
      JetDiscCJ_CvsL    = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet_CvsL);
      JetDiscCJ = AK4_JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
      JetFlavourCJ = AK4_JetAllColl.at(IdxMatchJet).hadronFlavour();

    }
    else {
      JetDiscCJ_CvsB=0.4;
      JetDiscCJ_CvsL=0.1;
      JetDiscCJ=0.1; 
      JetFlavourCJ=0;
    }
    velectron_v2_cj_cvsbjetdisc->push_back(JetDiscCJ_CvsB);
    velectron_v2_cj_cvsljetdisc->push_back(JetDiscCJ_CvsL);
    velectron_v2_cj_bjetdisc->push_back(JetDiscCJ);
    velectron_v2_cj_flavour->push_back(JetFlavourCJ);
  }
 
  return;
}

void AnalyzerCore::SetupLeptonBDTSKFlat(){

  // BUG IN PT ORDERING 
  /// Originally used GetALl* which orders pt, 
  /// Then if order changes the pushed vector has a mis match in lepton index
  
  std::vector<Muon>     AllmuonColl     = All_Muons;
  std::vector<Electron> AllelectronColl = All_Electrons;
  std::vector<Jet>      AK4_JetAllColl  = All_Jets;

  for(auto i: AllmuonColl){
    vmuon_mva_fake_v4->push_back(GetBDTScoreMuon(i,AnalyzerCore::Fake,  "BDTGv4"));
    vmuon_mva_fake_ed_v4->push_back(GetBDTScoreMuon_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv4"));

    vmuon_ptratio->push_back(JetLeptonPtRatioLepAware(i));
    vmuon_ptrel->push_back(JetLeptonPtRelLepAware(i));
    vmuon_lepton_type->push_back(GetLeptonType_JH(i, All_Gens));
    vmuon_is_cf->push_back(IsCF(i, All_Gens));

  }

  for(auto i: AllelectronColl){

    velectron_mva_fake_v4->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv4"));
    velectron_mva_fakeHF_v4->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv4_HF"));
    velectron_mva_fakeHFB_v4->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv4_HFB"));
    velectron_mva_fakeHFC_v4->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv4_HFC"));
    velectron_mva_fakeLF_v4->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv4_LF"));
    velectron_mva_fakeTop_v4->push_back(GetBDTScoreEl(i,AnalyzerCore::Fake,  "BDTGv4_Top"));

    velectron_mva_fake_ed_v4->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv4"));
    velectron_mva_fakeHF_ed_v4->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv4_HF"));
    velectron_mva_fakeHFB_ed_v4->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv4_HFB"));
    velectron_mva_fakeHFC_ed_v4->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv4_HFC"));
    velectron_mva_fakeLF_ed_v4->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv4_LF"));
    velectron_mva_fakeTop_ed_v4->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Fake,  "BDTGv4_Top"));

    velectron_mva_conv_v2->push_back(GetBDTScoreEl(i,AnalyzerCore::Conv,  "BDTGv4"));
    velectron_mva_conv_ed_v2->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::Conv,  "BDTGv4"));

    velectron_mva_cf_v2->push_back(GetBDTScoreEl(i,AnalyzerCore::CF,  "BDTGv4"));
    velectron_mva_cf_ed_v2->push_back(GetBDTScoreEl_EtaDependant(i,AnalyzerCore::CF,  "BDTGv4"));


    velectron_ptratio->push_back(JetLeptonPtRatioLepAware(i));
    velectron_ptrel->push_back(JetLeptonPtRelLepAware(i));
    velectron_lepton_type->push_back(GetLeptonType_JH(i, All_Gens));
    velectron_is_cf->push_back(IsCF(i, All_Gens));

   
  }

  return;


}

void AnalyzerCore::ResetLeptonBDTSKFlatV5(){
  
  ///// Reset V5 variables to add to BDT skim
  ///// No overlap with V4
  //cout << "V5 variables (some removed from original BDT skim code     "<< endl;

  velectron_v2_cj_bjetdisc->clear();
  velectron_v2_cj_cvsbjetdisc->clear();
  velectron_v2_cj_cvsljetdisc->clear();
  velectron_v2_cj_flavour->clear();
  
  velectron_mva_cf_ed_v5->clear();
  velectron_mva_cf_ed_v5pt->clear();
  velectron_mva_conv_ed_v5->clear();
  velectron_mva_fake_ed_v5->clear();
  velectron_mva_fakeHFB_v5->clear();
  velectron_mva_fakeHFC_v5->clear();
  velectron_mva_fakeLF_v5->clear();
  velectron_mva_fake_QCD_LFvsHF_v5->clear();
  velectron_mva_fake_QCD_HFBvsHFC_v5->clear();
  velectron_mva_fake_QCD_LF1_v5->clear();
  velectron_mva_fake_QCD_LF2_v5->clear();

  vmuon_v2_cj_bjetdisc->clear();
  vmuon_v2_cj_cvsbjetdisc->clear();
  vmuon_v2_cj_cvsljetdisc->clear();
  vmuon_v2_cj_flavour->clear();

  vmuon_mva_fake_QCD_LFvsHF_v5->clear();
  vmuon_mva_fake_QCD_HFBvsHFC_v5->clear();
  vmuon_mva_fake_QCD_LF1_v5->clear();
  vmuon_mva_fake_QCD_LF2_v5->clear();

  //cout << "END V5 variables (some removed from original BDT skim code     "<< endl;

  return;
}

void AnalyzerCore::ResetLeptonBDTSKFlat(){

  //// V4 variables (some removed from original BDT skim code
  //  cout << "V4 variables (some removed from original BDT skim code     " << endl;

  velectron_ptrel->clear();
  velectron_ptratio->clear();
  vmuon_ptrel->clear();
  vmuon_ptratio->clear();
  velectron_lepton_type->clear();
  velectron_is_cf->clear();
  vmuon_lepton_type->clear();
  vmuon_is_cf->clear();

  velectron_mva_cf_v2->clear();
  velectron_mva_cf_ed_v2->clear();
  velectron_mva_conv_v2->clear();
  velectron_mva_conv_ed_v2->clear();
  velectron_mva_fake_v4->clear();
  velectron_mva_fakeHF_v4->clear();
  velectron_mva_fakeHFB_v4->clear();
  velectron_mva_fakeHFC_v4->clear();
  velectron_mva_fakeLF_v4->clear();
  velectron_mva_fakeTop_v4->clear();
  velectron_mva_fake_ed_v4->clear();
  velectron_mva_fakeHF_ed_v4->clear();
  velectron_mva_fakeHFB_ed_v4->clear();
  velectron_mva_fakeHFC_ed_v4->clear();
  velectron_mva_fakeLF_ed_v4->clear();
  velectron_mva_fakeTop_ed_v4->clear();
  vmuon_mva_fake_ed_v4->clear();
  vmuon_mva_fake_v4->clear();
  
  //  cout << "END V4 variables (some removed from original BDT skim code     "<< endl;

  return;
}
void AnalyzerCore::InitialiseLeptonBDTSKFlat(){

  vSKWeight=0;

  velectron_ptratio = 0;
  velectron_ptrel  = 0;
  velectron_v2_cj_bjetdisc = 0;
  velectron_v2_cj_cvsbjetdisc = 0;
  velectron_v2_cj_cvsljetdisc = 0;
  velectron_v2_cj_flavour = 0;

  velectron_mva_cf_v2 = 0;
  velectron_mva_cf_ed_v2 = 0;
  velectron_mva_conv_ed_v2 = 0;
  velectron_mva_conv_v2 = 0;

  velectron_mva_fake_v4 = 0 ;
  velectron_mva_fakeHF_v4 = 0 ;
  velectron_mva_fakeHFB_v4 = 0 ;
  velectron_mva_fakeHFC_v4 = 0 ;
  velectron_mva_fakeLF_v4 = 0 ;
  velectron_mva_fakeTop_v4 = 0 ;
  velectron_mva_fake_ed_v4 = 0 ;
  velectron_mva_fakeHF_ed_v4 = 0 ;
  velectron_mva_fakeHFB_ed_v4 = 0 ;
  velectron_mva_fakeHFC_ed_v4 = 0 ;
  velectron_mva_fakeLF_ed_v4 = 0 ;
  velectron_mva_fakeTop_ed_v4 = 0 ;

  ///// BDT V5 
  /// ELECTRON
  velectron_mva_fake_ed_v5 = 0 ;
  velectron_mva_fakeHFB_v5 = 0 ;
  velectron_mva_fakeHFC_v5 = 0 ;
  velectron_mva_fakeLF_v5 = 0 ;
  velectron_mva_fake_QCD_LFvsHF_v5 = 0 ;
  velectron_mva_fake_QCD_HFBvsHFC_v5 = 0 ;
  velectron_mva_conv_ed_v5 = 0;
  velectron_mva_cf_ed_v5 = 0;
  velectron_mva_cf_ed_v5pt = 0;
  velectron_mva_fake_QCD_LF1_v5 = 0 ;
  velectron_mva_fake_QCD_LF2_v5 = 0 ;

  velectron_lepton_type=0;
  velectron_is_cf=0;

  vmuon_mva_fake_v4 = 0;
  vmuon_mva_fake_ed_v4 = 0;

  ///// BDT V5                                                                                                                                                                                                                                                                                                                
  /// MUON
  vmuon_mva_fake_QCD_LFvsHF_v5 = 0 ;
  vmuon_mva_fake_QCD_HFBvsHFC_v5 = 0 ;
  vmuon_mva_fake_QCD_LF1_v5 = 0 ;
  vmuon_mva_fake_QCD_LF2_v5 = 0 ;

  vmuon_ptratio = 0;
  vmuon_ptrel  = 0;
  vmuon_v2_cj_bjetdisc = 0;
  vmuon_v2_cj_cvsbjetdisc = 0;
  vmuon_v2_cj_cvsljetdisc = 0;
  vmuon_v2_cj_flavour = 0;
  vmuon_lepton_type=0;
  vmuon_is_cf=0;

  return;
}

void AnalyzerCore::SetupLeptonBDT( bool version4, bool version5){

  cout << "AnalyzerCore::SetupLeptonBDT version4 = " << version4 << " version5 = " << version5 << endl;

  iSetupLeptonBDTv4=version4;
  iSetupLeptonBDTv5=version5;

  TMVA::Tools::Instance();
  //ElectronIDv3ConvMVAReader = new TMVA::Reader();

  if(version4) SetupLeptonBDTv4();
  if(version5) SetupLeptonBDTv5();
  return;
}

void AnalyzerCore::SetupLeptonBDTv5(){

  ElectronIDv5_MVAReader = new TMVA::Reader();
  ElectronIDv5_CFMVAReader = new TMVA::Reader();
  ElectronIDv5_FakeMVAReader  = new TMVA::Reader();
  MuonIDv5_FakeMVAReader = new TMVA::Reader();
}

void AnalyzerCore::SetupLeptonBDTv4(){

   /// Version 2                                                                                                                                                               

  ElectronIDv4_FakeMVAReader = new TMVA::Reader();    
  ElectronIDv4_CFMVAReader   = new TMVA::Reader();
  ElectronIDv4_ConvMVAReader = new TMVA::Reader();
  MuonIDv4_FakeMVAReader = new TMVA::Reader();
  
  return;
}


void AnalyzerCore::beginEvent(){
  
  // fill jets first as they are independant
  if(Analyzer.Contains("BDT") && Analyzer.Contains("SkimTree")) PtOrderObj=false;
  else PtOrderObj=true;
  
  if(_jentry%10000==0) cout << "PtOrderObj = " << PtOrderObj << endl;

  if(!IsData) All_Gens = GetGens();  
  All_Jets      = GetAllJets();
  All_FatJets   = GetAllFatJets();
  if(_jentry%10000==0) cout << "GetAllMuons " << endl;
  All_Muons     = GetAllMuons();
  if(_jentry%10000==0) cout << "GetAllElectrons " << endl;
  All_Electrons = GetAllElectrons();

  return;
}
void AnalyzerCore::initializeAnalyzerTools(){

  //==== MCCorrection
  mcCorr->SetMCSample(MCSample);
  mcCorr->SetEra(GetEra());
  mcCorr->SetIsDATA(IsDATA);
  mcCorr->SetEventInfo(run, lumi, event);
  mcCorr->SetIsFastSim(IsFastSim);
  if(!IsDATA){
    mcCorr->ReadHistograms();
    mcCorr->SetupJetTagging();
  }

  puppiCorr->SetEra(GetEra());
  puppiCorr->ReadHistograms();

  //==== FakeBackgroundEstimator
  //if(AnalyserRunsFullBkg()){
  fakeEst->SetEra(GetEra());
  fakeEst->ReadHistograms();
  //==== CFBackgroundEstimator
  cfEst->SetEra(GetEra());
  cfEst->ReadHistograms();

  /*                                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                                              
    // In your analyser code add this line to constructor to fill map with JEC source values.                                                                                                                                                                                 
    for(auto jec_source : JECSources)   SetupJECUncertainty(jec_source, "AK4PFchs");                                                                                                                                                                                          
                                                                                                                                                                                                                                                                              
  }                                                                                                                                                                                                                                                                           
    // Then you can get vector of jets with shift calling                                                                                                                                                                                                                     
    std::vector<Jet> AnalyzerCore::ScaleJetsIndividualSource(const std::vector<Jet>& jets, int sys, TString source);                                                                                                                                                          
    vector<Jet> jets_AbsoluteStatUp = ScaleJetsIndividualSource(jets, 1, "AbsoluteStat");                                                                                                                                                                                     
  */

}


double AnalyzerCore::GetKFactor(){

  if(IsDATA) return 1.;

  double weight = 1.;

  if(MCSample.Contains("WZTo3LNu_powheg") or MCSample.Contains("WZTo2L2Q")){
    //Physics Letters B 761 (2016) 197 
    //http://dx.doi.org/10.1016/j.physletb.2016.08.017 
    weight = 1.109;
  }
  else if(MCSample.Contains("ZZTo4L_powheg") or MCSample.Contains("ZZTo2L2Nu") or MCSample.Contains("ZZTo2L2Q")){
    // Physics Letters B 735 (2014) 311-313
    // https://doi.org/10.1016/j.physletb.2014.06.056
    weight = 1.16; 
  }
  else if(MCSample.Contains("GluGluToZZto")){
    //  1.67 brings gg->ZZ from LO to NLO (http://arxiv.org/abs/1509.06734)
    return 1.67;
  }
  else if(MCSample.Contains("GluGluHToZZ")){
    return 1.67;
    //AN2016_359
  }
  else if(MCSample.Contains("ttZ") && !MCSample.Contains("To")){
    weight = 839.3/780.;
  }
  else if(MCSample.Contains("ttW") && !MCSample.Contains("To")){
    weight = 600.8/610.;
  }
  else if(MCSample.Contains("WJet") && MCSample.Contains("HT")){
    return 1.21;
  }



  return weight;

}


double AnalyzerCore::MCweight(bool usesign, bool norm_1invpb) const {

  if(IsDATA) return 1.;
  double weight=gen_weight;
  
  //MiNNLO sample has some events with unphysically large weight
  if(MCSample.Contains("DYJets")&&MCSample.Contains("MiNNLO")){
    double maxweight=2358.0700*5.;
    if(abs(weight)>maxweight){
      weight=weight>0. ? maxweight : -1.0*maxweight;
    }
  }
  //Sherpa sample needs weighted events
  if(MCSample.Contains("WJets") && MCSample.Contains("Sherpa")){
    usesign = false;
  }

  if(usesign){
    if(weight>0) weight=1.0;
    else if(weight<0) weight=-1.0;
    else weight=0.0;
  }
  if(norm_1invpb){
    if(usesign) weight*=xsec/sumSign;
    else weight*=xsec/sumW;
  }
  return weight;
}

double AnalyzerCore::GetPrefireWeight(int sys){

  if(IsDATA) return 1.;
  else{
    if(sys==0) return L1PrefireReweight_Central;
    else if(sys>0) return L1PrefireReweight_Up;
    else return L1PrefireReweight_Down;
  }

  cout << "[AnalyzerCore::GetPrefireWeight] wtf" << endl;
  exit(EXIT_FAILURE);
  return 1.;

}

double AnalyzerCore::GetPileUpWeight(int N_pileup, int syst){
  if(IsDATA) return 1.;
  else return mcCorr->GetPileUpWeight(N_pileup, syst);
}

double AnalyzerCore::GetPDFWeight(LHAPDF::PDF* pdf_){

  double pdf_1 = pdf_->xfxQ(genWeight_id1, genWeight_X1, genWeight_Q);
  double pdf_2 = pdf_->xfxQ(genWeight_id2, genWeight_X2, genWeight_Q);

  return pdf_1 * pdf_2;

}

double AnalyzerCore::GetPDFReweight(){

  return GetPDFWeight(pdfReweight->NewPDF)/GetPDFWeight(pdfReweight->ProdPDF);

}

double AnalyzerCore::GetPDFReweight(int member){

  return GetPDFWeight(pdfReweight->PDFErrorSet.at(member))/GetPDFWeight(pdfReweight->ProdPDF);

}

bool AnalyzerCore::IsOnZ(double m, double width){
  if( fabs(m-M_Z) < width ) return true;
  else return false;
}

double AnalyzerCore::MT(TLorentzVector a, TLorentzVector b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
}

double AnalyzerCore::MT2(TLorentzVector a, TLorentzVector b, Particle METv, double METgap){

  TLorentzVector METa, METb;
  METa.SetPxPyPzE( 0., 0., 0., 0.);
  double MTa, MTb;
  double tempMETa =0., tempMT2 = TMath::Max(MT(a, METv), MT(b, METv));

  while(tempMETa < METv.Pt()){

    METa.SetPxPyPzE(tempMETa*TMath::Cos(METv.Phi()), tempMETa*TMath::Sin(METv.Phi()), 0., tempMETa);
    METb = METv - METa;

    MTa = MT(METa, a);
    MTb = MT(METb, b);

    tempMT2 = TMath::Min(tempMT2, TMath::Max(MTa, MTb));

    tempMETa = tempMETa + METgap;
  }  

  return tempMT2;

}

double AnalyzerCore::projectedMET(TLorentzVector a, TLorentzVector b, Particle METv){

  if( fabs(a.DeltaPhi(METv)) < fabs(b.DeltaPhi(METv)) ){
    return (METv.Pt() * TMath::Sin(fabs(a.DeltaPhi(METv))) );
  }
  else return (METv.Pt() * TMath::Sin(fabs(b.DeltaPhi(METv))) );

}

bool AnalyzerCore::HasFlag(TString flag){

  //cout << "[AnalyzerCore::HasFlag] Userflags.size() = " << Userflags.size() << endl;
  //for(unsigned int i=0; i<Userflags.size(); i++){
  //  cout << "[AnalyzerCore::HasFlag] " << Userflags.at(i) << endl;
  //}
  
  return std::find(Userflags.begin(), Userflags.end(), flag) != Userflags.end();

}

std::vector<Muon> AnalyzerCore::MuonWithoutGap(const std::vector<Muon>& muons){

  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    double this_eta = fabs( muons.at(i).Eta() );
    if( 1.444 <= this_eta && this_eta < 1.566 ) continue;

    out.push_back( muons.at(i) );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


vector<Muon> AnalyzerCore::SkimLepColl(const vector<Muon>& MuColl,  AnalyzerParameter param, TString Option){

  bool GetPrompt=false, GetHadFake=false, GetEWtau=false, GetNHIntConv=false, GetNHExtConv=false;

  if(Option.Contains("Prompt"))          GetPrompt    =true;
  if(Option.Contains("HFake"))           GetHadFake   =true;
  if(Option.Contains("EWtau"))           GetEWtau     =true;
  if(Option.Contains("NHConv"))         {GetNHIntConv =true; GetNHExtConv=true;}
  else{ 
    if(Option.Contains("NHIntConv")) GetNHIntConv =true;
    if(Option.Contains("NHExtConv")) GetNHExtConv =true; 
  }
  if(     Option=="Fake"     )          {GetHadFake   =true; GetNHExtConv=true;}



  vector<Muon> ReturnVec;
  for(unsigned int i=0; i<MuColl.size(); i++){

    if(IsData) ReturnVec.push_back(MuColl.at(i));
    else {
      int LepType= MuColl.at(i).LeptonGenType(); 
      bool PassSel=false;
      
      if( GetPrompt    && (LepType==1 || LepType==2) ) PassSel=true;
      if( GetHadFake   && (LepType<0 && LepType>=-4) ) PassSel=true;
      if( GetEWtau     &&         LepType==3         ) PassSel=true;
      if( GetNHIntConv &&         LepType>=4         ) PassSel=true;
      if( GetNHExtConv &&         LepType<-4         ) PassSel=true;
      if( PassSel ) ReturnVec.push_back(MuColl.at(i));
    }
  }

  return ReturnVec;
}



vector<Electron> AnalyzerCore::SkimLepColl(const vector<Electron>& ElColl, AnalyzerParameter param,TString Option){

  bool GetPrompt=false, GetHadFake=false, GetEWtau=false, GetNHIntConv=false, GetNHExtConv=false, GetCF=false;
  //CFHFakeNHConv
  if(Option.Contains("Prompt"))          GetPrompt    =true;
  if(Option.Contains("CF"))              GetCF        =true;
  if(Option.Contains("HFake"))           GetHadFake   =true;
  if(Option.Contains("EWtau"))           GetEWtau     =true;
  if(Option.Contains("NHConv"))         {GetNHIntConv =true; GetNHExtConv=true;}
  else{ if(Option.Contains("NHIntConv")) GetNHIntConv =true;
    if(Option.Contains("NHExtConv")) GetNHExtConv =true; }
  if(     Option=="Fake"     )          {GetHadFake   =true; GetNHExtConv=true;}


  vector<Electron> ReturnVec;
  for(unsigned int i=0; i<ElColl.size(); i++){
    if(IsData) ReturnVec.push_back(ElColl.at(i));
    else {
      int LepType= ElColl.at(i).LeptonGenType();
      bool PassSel=false;
      
      if( GetPrompt    && (LepType==1 || LepType==2) ) PassSel=true;
      if( GetHadFake   && (LepType<0 && LepType>=-4) ) PassSel=true;
      if( GetEWtau     &&         LepType==3         ) PassSel=true;
      if( GetNHIntConv &&         LepType>=4         ) PassSel=true;
      if( GetNHExtConv &&         LepType<-4         ) PassSel=true;
      if( GetCF        &&         ElColl.at(i).LeptonIsCF() ) PassSel=true;
      if( Option.Contains("NoCF") && ElColl.at(i).LeptonIsCF()) PassSel=false;
      if( PassSel ) ReturnVec.push_back(ElColl.at(i));
    }
  }

  return ReturnVec;
}



vector<Electron> AnalyzerCore::SkimLepColl(const vector<Electron>& ElColl, TString Option){
  
  vector<Electron> ReturnColl;

  bool Barrel1=false, Barrel2=false, Endcap=false;
  if(Option.Contains("B1")) Barrel1=true;
  if(Option.Contains("B2")) Barrel2=true;
  if(Option.Contains("E"))  Endcap =true;

  for(unsigned int i=0; i<ElColl.size(); i++){
    bool PassSel=false; double fEta=fabs(ElColl.at(i).Eta());
    if( Barrel1 && fEta <0.8               ) PassSel=true;
    if( Barrel2 && fEta>=0.8 && fEta<1.479 ) PassSel=true;
    if( Endcap  && fEta>=1.479 && fEta<2.5 ) PassSel=true;
    if( PassSel ) ReturnColl.push_back(ElColl.at(i));
  }

  return ReturnColl;
}


vector<Muon> AnalyzerCore::SkimLepColl(const vector<Muon>& MuColl,  TString Option){
  
  vector<Muon> ReturnColl;
  bool Barrel=false, Overlap=false, Endcap=false;
  if(Option.Contains("MB")) Barrel =true;
  if(Option.Contains("MO")) Overlap=true;
  if(Option.Contains("ME")) Endcap =true;

  for(unsigned int i=0; i<MuColl.size(); i++){
    bool PassSel=false; double fEta=fabs(MuColl.at(i).Eta());
    if( Barrel  && fEta <0.9               ) PassSel=true;
    if( Overlap && fEta>=0.9 && fEta<1.6   ) PassSel=true;
    if( Endcap  && fEta>=1.6 && fEta<2.4   ) PassSel=true;
    if( PassSel ) ReturnColl.push_back(MuColl.at(i));
  }

  return ReturnColl;
}


vector<Jet> AnalyzerCore::SkimJetColl(const vector<Jet>& JetColl, vector<Gen>& TruthColl, AnalyzerParameter param,TString Option){

  bool GetPrLepCleanJet=false;
  TString Criteria="";
  if(Option.Contains("NoPr"))  GetPrLepCleanJet =true;
  if(Option.Contains("NoTau")) Criteria="InclTau";

  vector<Jet> ReturnVec;
  for(unsigned int i=0; i<JetColl.size(); i++){
    bool HasEWLep=HasEWLepInJet(JetColl.at(i), TruthColl, Criteria);
    if( GetPrLepCleanJet && (!HasEWLep) ) ReturnVec.push_back(JetColl.at(i));
  }

  return ReturnVec;
}


bool AnalyzerCore::HasEWLepInJet(Jet Jet, vector<Gen>& TruthColl, TString Option){

  int HasEWLep=false;
  bool InclEWTau=Option.Contains("InclTau");
  for(unsigned int i=2; i<TruthColl.size(); i++){
    if(TruthColl.at(i).MotherIndex()<0 )  continue;
    bool IsCand=false;
    int abspid=fabs(TruthColl.at(i).PID());
    int absmpid=fabs(TruthColl.at(TruthColl.at(i).MotherIndex()).PID());
    if( TruthColl.at(i).Status()==1 ){
      if( abspid==11 || abspid==13 ){
        int LepType=GetLeptonType_JH(i, TruthColl);
        if( LepType==1 || LepType==2 || LepType==3 ) IsCand=true; 
      }
    }
    if( InclEWTau && abspid==15 && TruthColl.at(i).Status()>20 && TruthColl.at(i).Status()<30 ) IsCand=true;
    else if( InclEWTau && abspid==15 && (absmpid==23 || absmpid==24) && TruthColl.at(i).Status()==2 ) IsCand=true;

    if( IsCand && TruthColl.at(i).DeltaR(Jet)<0.4) HasEWLep=true;
    if( HasEWLep ) break;
  }

  return HasEWLep;
}


std::vector<Muon> AnalyzerCore::MuonPromptOnly(const std::vector<Muon>& muons, const std::vector<Gen>& gens){

  if(IsDATA) return muons;

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){
    if(GetLeptonType(muons.at(i), gens)<=0) continue;
    out.push_back( muons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Muon> AnalyzerCore::MuonPromptOnly(const std::vector<Muon>& muons, const std::vector<Gen>& gens, AnalyzerParameter param){

  if(IsDATA) return muons;

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){
    if(param.FakeMethod == "Data"){
      if(GetLeptonType(muons.at(i), gens)<=0) continue;
    }
    out.push_back( muons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


TString AnalyzerCore::PromptStatus(Muon mu, const std::vector<Gen>& gens){

  if(GetLeptonType(mu, gens)>0) return "Prompt";
  else return "Fake";

}


std::vector<Muon> AnalyzerCore::MuonUsePtCone(const std::vector<Muon>& muons){

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){
    //==== muons is a const vector. So in this function, we have to copy the elements like below
    Muon this_muon = muons.at(i);
    this_muon.SetPtEtaPhiM( muons.at(i).PtCone(), muons.at(i).Eta(), muons.at(i).Phi(), muons.at(i).M() );
    out.push_back( this_muon );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

Muon AnalyzerCore::MuonUsePtCone(const Muon& muon){

  //==== muon is a const object. So in this function, we have to copy the object like below
  Muon this_muon = muon;
  this_muon.SetPtEtaPhiM( muon.PtCone(), muon.Eta(), muon.Phi(), muon.M() );
  return this_muon;

}

Particle AnalyzerCore::UpdateMETSyst(double met_pt, double met_phi, double met_shift_pt, double met_shift_phi, const Particle& METv){
  
  double met_px = met_pt*TMath::Cos(met_phi);
  double met_py = met_pt*TMath::Sin(met_phi);
  double met_shift_px = met_shift_pt*TMath::Cos(met_shift_phi);
  double met_shift_py = met_shift_pt*TMath::Sin(met_shift_phi);

  double met_x = METv.Px();
  double met_y = METv.Py();
  
  met_x = met_x + met_shift_px - met_px;
  met_y = met_y + met_shift_py - met_py;

  Particle METout;
  METout.SetPxPyPzE(met_x,met_y,0,sqrt(met_x*met_x+met_y*met_y));
  return METout;

  
}

Particle AnalyzerCore::UpdateMET(const Particle& METv, const std::vector<Muon>& muons){

  double met_x = METv.Px();
  double met_y = METv.Py();

  double px_orig(0.), py_orig(0.),px_corrected(0.), py_corrected(0.);
  for(unsigned int i=0; i<muons.size(); i++){

    px_orig+= muons.at(i).MiniAODPt()*TMath::Cos(muons.at(i).Phi());
    py_orig+= muons.at(i).MiniAODPt()*TMath::Sin(muons.at(i).Phi());

    px_corrected += muons.at(i).Px();
    py_corrected += muons.at(i).Py();

  }

  met_x = met_x + px_orig - px_corrected;
  met_y = met_y + py_orig - py_corrected;

  Particle METout;
  METout.SetPxPyPzE(met_x,met_y,0,sqrt(met_x*met_x+met_y*met_y));
  return METout;

}

Particle AnalyzerCore::UpdateMETSmearedJet(const Particle& METv, const std::vector<Jet>& jets){

  double met_x = METv.Px();
  double met_y = METv.Py();

  double px_orig(0.), py_orig(0.),px_corrected(0.), py_corrected(0.);
  for(auto jet : jets){
    px_orig+= jet.PxUnSmeared();
    py_orig+= jet.PyUnSmeared();

    px_corrected += jet.Px();
    py_corrected += jet.Py();
  }

  met_x = met_x + px_orig - px_corrected;
  met_y = met_y + py_orig - py_corrected;

  Particle METout;
  METout.SetPxPyPzE(met_x,met_y,0,sqrt(met_x*met_x+met_y*met_y));
  return METout;

}

Particle AnalyzerCore::UpdateMETSyst(AnalyzerParameter param, const Particle& METv, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Muon> muons, std::vector<Electron> electrons){

  double met_x = METv.Px();
  double met_y = METv.Py();

  double px_orig(0.), py_orig(0.),px_corrected(0.), py_corrected(0.);

  if(param.syst_ == AnalyzerParameter::JetResUp || 
     param.syst_ == AnalyzerParameter::JetResDown || 
     param.syst_ == AnalyzerParameter::JetEnUp || 
     param.syst_ == AnalyzerParameter::JetEnDown){
    
    for(unsigned int i=0; i<jets.size(); i++){
      px_orig += jets.at(i).PxUnSmeared();
      py_orig += jets.at(i).PyUnSmeared();
      px_corrected += jets.at(i).Px();
      py_corrected += jets.at(i).Py();
    }
  }

  if(param.syst_ == AnalyzerParameter::JetResUp ||
     param.syst_ == AnalyzerParameter::JetResDown ||
     param.syst_ == AnalyzerParameter::JetEnUp ||
     param.syst_ == AnalyzerParameter::JetEnDown ||
     param.syst_ == AnalyzerParameter::JetMassSmearUp ||
     param.syst_ == AnalyzerParameter::JetMassSmearDown){
    
    for(unsigned int i=0; i<fatjets.size(); i++){
      px_orig += fatjets.at(i).PxUnSmeared();
      py_orig += fatjets.at(i).PyUnSmeared();
      px_corrected += fatjets.at(i).Px();
      py_corrected += fatjets.at(i).Py();
    }
  }
  
  if(param.syst_ ==AnalyzerParameter::MuonEnUp ||
     param.syst_ ==AnalyzerParameter::MuonEnDown){
    for(unsigned int i=0; i<muons.size(); i++){
      px_orig += muons.at(i).UncorrectedPt() * TMath::Cos(muons.at(i).Phi());
      py_orig += muons.at(i).UncorrectedPt() * TMath::Sin(muons.at(i).Phi());
      px_corrected += muons.at(i).Px();
      py_corrected += muons.at(i).Py();
    }
  }
  if(param.syst_ ==AnalyzerParameter::ElectronEnUp ||
     param.syst_ ==AnalyzerParameter::ElectronEnDown ||
     param.syst_ ==AnalyzerParameter::ElectronResUp ||
     param.syst_ ==AnalyzerParameter::ElectronResDown){
    for(unsigned int i=0; i<electrons.size(); i++){
      px_orig += electrons.at(i).UncorrectedPt() * TMath::Cos(electrons.at(i).Phi());
      py_orig += electrons.at(i).UncorrectedPt() * TMath::Sin(electrons.at(i).Phi());
      px_corrected += electrons.at(i).Px();
      py_corrected += electrons.at(i).Py();
    }
  }

  met_x = met_x + px_orig - px_corrected;
  met_y = met_y + py_orig - py_corrected;

  Particle METout;
  METout.SetPxPyPzE(met_x,met_y,0,sqrt(met_x*met_x+met_y*met_y));
  return METout;

}


std::vector<Muon> AnalyzerCore::MuonApplyPtCut(const std::vector<Muon>& muons, double ptcut){

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){
    if(!( muons.at(i).Pt() > ptcut )) continue;
    out.push_back( muons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Electron> AnalyzerCore::ElectronPromptOnly(const std::vector<Electron>& electrons, const std::vector<Gen>& gens){

  if(IsDATA) return electrons;

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    bool pass=true;
    if(GetLeptonType(electrons.at(i), gens) <= 0)pass=false;
    if(GetLeptonType(electrons.at(i), gens)== -5)pass=true;
    if(GetLeptonType(electrons.at(i), gens)== -6)pass=true;
  
    if(pass)out.push_back( electrons.at(i) );
  }
  
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;
  
}

std::vector<Electron> AnalyzerCore::ElectronPromptOnly(const std::vector<Electron>& electrons, const std::vector<Gen>& gens, AnalyzerParameter param){

  if(IsDATA) return electrons;

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    bool pass=true;
    //if(GetLeptonType(electrons.at(i), gens)<=0) continue;

    if(param.FakeMethod == "Data"){
      if(GetLeptonType(electrons.at(i), gens) <= 0)pass=false;
    }

    if(param.FakeMethod == "DataNoConv"){
      if(GetLeptonType(electrons.at(i), gens) <= 0)pass=false;
      if(GetLeptonType(electrons.at(i), gens)== -5)pass=true;
      if(GetLeptonType(electrons.at(i), gens)== -6)pass=true;
    }
    if(param.CFMethod == "Data"){
      if(electrons.at(i).LeptonIsCF()) pass=false;
    }
    
    if(pass)out.push_back( electrons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Electron> AnalyzerCore::ElectronUsePtCone(const std::vector<Electron>& electrons){

  std::vector<Electron> out;

  for(unsigned int i=0; i<electrons.size(); i++){
    //==== electrons is a const vector. So in this function, we have to copy the elements like below
    Electron this_electron = electrons.at(i);
    this_electron.SetPtEtaPhiM( electrons.at(i).PtCone(), electrons.at(i).Eta(), electrons.at(i).Phi(), electrons.at(i).M() );
    out.push_back( this_electron );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

Electron AnalyzerCore::ElectronUsePtCone(const Electron& electron){

  //==== electron is a const object. So in this function, we have to copy the object like below
  Electron this_electron = electron;
  this_electron.SetPtEtaPhiM( electron.PtCone(), electron.Eta(), electron.Phi(), electron.M() );
  return this_electron;

}

std::vector<Electron> AnalyzerCore::ElectronApplyPtCut(const std::vector<Electron>& electrons, double ptcut){

  std::vector<Electron> out;

  for(unsigned int i=0; i<electrons.size(); i++){
    if(!( electrons.at(i).Pt() > ptcut )) continue;
    out.push_back( electrons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Jet> AnalyzerCore::JetsAwayFromFatJet(const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets, double mindr){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){

    bool Overlap = false;
    for(unsigned int j=0; j<fatjets.size(); j++){
      if( ( jets.at(i) ).DeltaR( fatjets.at(j) ) < mindr ){
        Overlap = true;
        break;
      }
    }
    if(!Overlap) out.push_back( jets.at(i) );

  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

Jet AnalyzerCore::GetCorrectedJetCloseToLepton(vector<Particle> leps, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton                                                                                           
  Particle  rawJet =  jet * jet.JECfactor();
  double l1corrFactor = jet.JEC_L1();

  Particle  rawJetMinusLep = rawJet;
  Particle Lepsp4;
  for(auto ip : leps) {
    Lepsp4 = Lepsp4+ip;
    
    rawJetMinusLep = rawJetMinusLep - ip* (1.0 / l1corrFactor);
  }
  Particle jetp4 = (rawJetMinusLep ) * (jet.Pt() / rawJet.Pt()) + Lepsp4;


  //cout << "Corr Eta : " << jetp4.Eta() << " " << jet.Eta() << endl;                                                                                        
  // cout << "Corr Phi : " << jetp4.Phi() << " " << jet.Phi() << endl;                                                                                       

  Jet jet_corr = jet;

  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}



Jet AnalyzerCore::GetCorrectedJetCloseToLepton(Muon lep, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton                                                        

  Particle  rawJet =  jet * jet.JECfactor();

  Particle  lepp4 = lep;
  double l1corrFactor = jet.JEC_L1();
  Particle jetp4 = (rawJet - lepp4 * (1.0 / l1corrFactor)) * (jet.Pt() / rawJet.Pt()) + lepp4;


  //cout << "Corr Eta : " << jetp4.Eta() << " " << jet.Eta() << endl;
  // cout << "Corr Phi : " << jetp4.Phi() << " " << jet.Phi() << endl;

  Jet jet_corr = jet;

  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}

Jet AnalyzerCore::GetCorrectedJetCloseToLepton(Electron lep, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton
  Particle  rawJet =  jet * jet.JECfactor(); 

  Particle  lepp4 = lep;
  double l1corrFactor = jet.JEC_L1();
  Particle jetp4 = (rawJet - lepp4 * (1.0 / l1corrFactor)) * (jet.Pt() / rawJet.Pt()) + lepp4;
  
  Jet jet_corr = jet;
  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}

Jet AnalyzerCore::GetCorrectedJetCloseToLepton(Lepton lep, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton                                                                                                                                                                                                      

  Particle  rawJet =  jet * jet.JECfactor();

  Particle  lepp4 = lep;
  double l1corrFactor = jet.JEC_L1();
  Particle jetp4 = (rawJet - lepp4 * (1.0 / l1corrFactor)) * (jet.Pt() / rawJet.Pt()) + lepp4;

  Jet jet_corr = jet;
  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}


double  AnalyzerCore::JetLeptonMassDropLepAware( Muon lep, bool removeLep, bool ApplyCorr){


  // ApplyCorr def is false, this is same as Mini/NanoAOD value stored                                                                                                                                              
  // if ApplyCorr is true then Jet smearing and lepton smearing is applied and values are corrected                                                                                                                 

  std::vector<Jet> jets = GetAllJets(ApplyCorr);

  double mindR=0.4;
  if(!ApplyCorr)lep.SetPtEtaPhiE(lep.MiniAODPt(), lep.Eta(), lep.Phi(), lep.E());

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }
  if(mindR==0.4) return -1;

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;


  if(removeLep)   jetp4 = jetp4 - lep;

  TLorentzVector LJ =  jetp4 + lep;
  
  return (LJ.M() - jetp4.M());

}


double  AnalyzerCore::JetLeptonMassDropLepAware( Electron lep, bool removeLep, bool ApplyCorr){
  std::vector<Jet> jets = GetAllJets(ApplyCorr);
  double mindR=0.4;
  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) closejet = GetCorrectedJetCloseToLepton(lep,jet);
  }
  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;
  if(removeLep)   jetp4 = jetp4 - lep;
  TLorentzVector LJ =  jetp4 + lep;
  return (LJ.M() - jetp4.M());
}


double  AnalyzerCore::JetLeptonPtRelLepAware( Muon lep, bool CorrLep){

  if(!CorrLep)lep.SetPtEtaPhiE(lep.MiniAODPt(), lep.Eta(), lep.Phi(), lep.E());
  return JetLeptonPtRelLepAware(Lepton(lep));
}

double  AnalyzerCore::JetLeptonPtRelLepAware( Electron lep){
  return JetLeptonPtRelLepAware(Lepton(lep));
}

double  AnalyzerCore::JetLeptonPtRelLepAware( Lepton lep, Jet jet){

  Jet closejet = GetCorrectedJetCloseToLepton(lep,jet);

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;

  double PtRel = lepp4.Perp((closejet-lepp4).Vect()); // Default       
  return PtRel;
}


double  AnalyzerCore::JetLeptonPtRelLepAware( Lepton lep){
  
  // ApplyCorr def is false, this is same as Mini/NanoAOD value stored
  // if ApplyCorr is true then Jet smearing and lepton smearing is applied and values are corrected

  std::vector<Jet> jets = GetAllJets(false);

  double mindR=0.4;

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }
  
  if(mindR==0.4) return 0.;

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;
  
  double PtRel = lepp4.Perp((closejet-lepp4).Vect()); // Default

  return PtRel;

}


double  AnalyzerCore::JetLeptonPtRelLepAwareV2( Lepton lep){

  lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());

  std::vector<Jet> jets = GetAllJets(false);

  double mindR=0.4;

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }

  if(mindR==0.4) return 0.;

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;

  double PtRel = lepp4.Perp((closejet-lepp4).Vect()); // Default                                                                                             

  return PtRel;

}


double  AnalyzerCore::JetLeptonPtRatioLepAware(Muon lep, bool CorrLep){
  if(!CorrLep)lep.SetPtEtaPhiE(lep.MiniAODPt(), lep.Eta(), lep.Phi(), lep.E());
  return JetLeptonPtRatioLepAware(Lepton(lep));
}
double  AnalyzerCore::JetLeptonPtRatioLepAware( Electron lep){
  return JetLeptonPtRatioLepAware(Lepton(lep));
}


double  AnalyzerCore::JetLeptonPtRatioLepAware(Lepton lep, Jet jet){
  Jet closejet = GetCorrectedJetCloseToLepton(lep,jet);
  Particle lepp4 = lep;
  return std::min(lepp4.Pt() / closejet.Pt(),1.5);
}

double  AnalyzerCore::JetLeptonPtRatioLepAware(Lepton lep){

  std::vector<Jet> jets = GetAllJets(false);

  double  mindR=0.4;
  
  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }

  Particle lepp4 = lep;
  
  if (mindR == 0.4)  return std::min(1./ (1. + lep.RelIso()), 1.5);
    
  return std::min(lepp4.Pt() / closejet.Pt(),1.5); 

}

double  AnalyzerCore::JetLeptonPtRatioLepAwareV2(Lepton lep){

  lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());
  std::vector<Jet> jets = GetAllJets(false);

  double  mindR=0.4;

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }

  Particle lepp4 = lep;

  if (mindR == 0.4)  return std::min(1./ (1. + lep.RelIso()), 1.5);

  return std::min(lepp4.Pt() / closejet.Pt(),1.5);

}



double  AnalyzerCore::JetLeptonPtRatioLepAwareMuon(Lepton lep, bool smearjet, bool corrMu, bool uncorrLepE){

  std::vector<Jet> jets = GetAllJets(smearjet);

  if(uncorrLepE)     lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());

  double  mindR=0.4;

  Jet closejet;
  Jet UncorJet;
  for(auto jet : jets){
    if(jet.Pt() < 10) continue;

    if (lep.DeltaR(jet) < mindR) {
      UncorJet=jet;
      mindR = lep.DeltaR(jet);
    }
  }
  
  vector<Muon> LooseMuons = All_Muons;
  vector<Particle> OverlapLeps = {lep};
  if(corrMu){
    for(auto imu : LooseMuons){
      if(UncorJet.DeltaR(imu) < 0.4){
  if (imu.PassID("POGLoose")) {
    OverlapLeps.push_back(imu);
  }
      }
    }
  }
  
  closejet = GetCorrectedJetCloseToLepton(OverlapLeps, UncorJet);

  Particle lepp4 = lep;

  if (mindR == 0.4)  return std::min(1./ (1. + lep.RelIso()), 1.5);

  return std::min(lepp4.Pt() / closejet.Pt(),1.5);

}


double  AnalyzerCore::JetLeptonPtRelLepAwareMuon( Lepton lep, bool smearjet, bool corrMu, bool uncorrLepE){

  std::vector<Jet> jets = GetAllJets(smearjet);

  if(uncorrLepE)     lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());
  
  double mindR=0.4;

  Jet closejet;
  Jet UncorJet;

  for(auto jet : jets){
    if(jet.Pt() < 10) continue;

    if (lep.DeltaR(jet) < mindR) {
      UncorJet = jet;
      mindR = lep.DeltaR(jet);
    }
  }

  if(mindR==0.4) return 0.;

  vector<Muon> LooseMuons = All_Muons;
  vector<Particle> OverlapLeps = {lep};
  if(corrMu){
    for(auto imu : LooseMuons){
      if(UncorJet.DeltaR(imu) < 0.4){
        if (imu.PassID("POGLoose")) {
          OverlapLeps.push_back(imu);
        }
      }
    }
  }
  
  
  //  if(OverlapLeps.size() > 1)
  closejet = GetCorrectedJetCloseToLepton(OverlapLeps, UncorJet);

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;
  
  
  double PtRel = lepp4.Perp((jetp4-lepp4).Vect()); // Default                                                                                               
  
  return PtRel;

}




std::vector<Jet> AnalyzerCore::JetsVetoLeptonInside(const std::vector<Jet>& jets, const std::vector<Electron>& els, const std::vector<Muon>& mus, double dR){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){

    bool HasLeptonInside = false;

    for(unsigned int j=0; j<els.size(); j++){
      if( jets.at(i).DeltaR( els.at(j) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    for(unsigned int j=0; j<mus.size(); j++){
      if( jets.at(i).DeltaR( mus.at(j) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    //==== if all fine,
    out.push_back( jets.at(i) );

  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<FatJet> AnalyzerCore::FatJetsVetoLeptonInside(const std::vector<FatJet>& jets, const std::vector<Electron>& els, const std::vector<Muon>& mus, double dR){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    FatJet this_jet = jets.at(i);

    bool HasLeptonInside = false;

    for(unsigned int j=0; j<els.size(); j++){
      if( this_jet.DeltaR( els.at(j) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    for(unsigned int j=0; j<mus.size(); j++){
      if( this_jet.DeltaR( mus.at(j) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    //==== if all fine,
    out.push_back( this_jet );

  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<Jet> AnalyzerCore::JetsAwayFromPhoton(const std::vector<Jet>& jets, const std::vector<Photon>& photons, double mindr){
  
  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    
    bool Overlap = false;
    for(unsigned int j=0; j<photons.size(); j++){
      if( ( jets.at(i) ).DeltaR( photons.at(j) ) < mindr ){
        Overlap = true;
        break;
      }
    }
    if(!Overlap) out.push_back( jets.at(i) );

  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}


Particle AnalyzerCore::AddFatJetAndLepton(const FatJet& fatjet, const Lepton& lep){

  if(fatjet.DeltaR( lep )<0.8){
    return fatjet;
  }
  else{
    return fatjet+lep;
  }

}

//=========================================================
//==== Gen Matching Tools

void AnalyzerCore::PrintGen(const std::vector<Gen>& gens){

  cout << "===========================================================" << endl;
  cout << "RunNumber:EventNumber = " << run << ":" << event << endl;
  cout << "index\tPID\tStatus\tMIdx\tMPID\tStart\tPt\tEta\tPhi\tM" << endl;
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    if(!MCSample.Contains("Sherpa")){
      vector<int> history = TrackGenSelfHistory(gen, gens);
      cout << i << "\t" << gen.PID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() << "\t" << gens.at(gen.MotherIndex()).PID()<< "\t" << history[0] << "\t";
    }
  
    printf("%.2f\t%.2f\t%.2f\t%.2f\n",gen.Pt(), gen.Eta(), gen.Phi(), gen.M());
  }

}

bool AnalyzerCore::GenTypeMatched(TString gen_string){

  vector<TString> lables =GetGenListPlot();
  if (std::find(lables.begin(), lables.end(), gen_string) == lables.end()) return false;
  
  return true;  
}


vector<TString> AnalyzerCore::GetGenListPlot(){

  vector<TString> lables = {"electron",
                            "muon",
                            "pi0",
                            "pi+",
                            "ph",
                            "tau",
                            "q",
                            "c",
                            "b",
                            "t",
                            "g",
                            "K",
                            "K*",
                            "K+",
                            "D",
                            "D0",
                            "D+",
                            "B",
                            "B0",
                            "B+",
                            "D0_K+",
                            "B0_K+",
                            "LightMeson",
                            "ccMeson",
                            "bbMeson",
                            "CMeson",
                            "BMeson",
                            "CBaryon",
                            "BBaryon",
                            "proton",
                            "conv",

                            "__"};

  return lables;

}


vector<TString> AnalyzerCore::GetGenList(){

  vector<TString> lables = {"electron",
                            "muon",
                            "pi0ph",
                            "pi0",
                            "pi+",
                            "ph",
                            "tau",
                            "q",
                            "c",
                            "b",
                            "t",
                            "g",
                            "JPsi",
                            "K",
                            "K*",
                            "K+",
                            "D",
                            "D0",
                            "D+",
                            "B",
          "B0",
                            "B+",
                            "D0_K+",
                            "B0_K+",
                            "LightMeson",
                            "CMeson",
                            "ccMeson",
                            "BMeson",
                            "bbMeson",
                            "CBaryon",
                            "BBaryon",
                            "proton",
                            "conv",
          "__"};

  return lables;

}

TString AnalyzerCore::MatchGenPID(int PID, vector<Gen> gens,  Gen gen){

  if(PID == 13 && gen.Status()==1 && (fabs(gens.at(gen.MotherIndex()).PID()) == 24)) return "muon";
  if(PID == 11 && gen.Status()==1 && (fabs(gens.at(gen.MotherIndex()).PID()) == 24)) return "electron";
  if(PID == 22 && gen.Status()==1 && (fabs(gens.at(gen.MotherIndex()).PID()) == 111)) return "pi0ph";
  if(PID == 321 && gen.Status()==1 && (fabs(gens.at(gen.MotherIndex()).PID()) == 421)) {
    if(fabs( gens.at(gens.at(gen.MotherIndex()).MotherIndex()).PID()) == 413)     return "D";
    if(fabs( gens.at(gens.at(gen.MotherIndex()).MotherIndex()).PID()) == 423)     return "D";
    return "D0";
  }
  if(PID == 321 && gen.Status()==1 && (fabs(gens.at(gen.MotherIndex()).PID()) == 511)) {
    return "B0";
  }
  int MID = fabs(gens.at(gen.MotherIndex()).PID());
  
  if(PID == 511 )  return "B0";
  if(PID  == 531 ) return "B";
  if(PID  == 533 ) return "B";
  if(PID  == 521 ) return "B+";
  if(PID > 5000  && PID < 6000 ) return "BBaryon";
  if(PID > 4000  && PID < 5000 ) return "CBaryon";
  

  if(MID == 511 ) return "B0";
  if(MID  == 531 ) return "B";
  if(MID  == 533 ) return "B";
  if(MID  == 521 ) return "B+";
  if(MID  == 431 ) return "D";
  if(MID  == 433 ) return "D";
  if(MID > 5000  && MID < 6000 ) return "BBaryon";
  if(MID > 4000  && MID < 5000 ) return "CBaryon";


  return "";
}

TString AnalyzerCore::MatchGenDef(std::vector<Gen>& gens,const Lepton& Lep, bool DEBUG){
  
  //////////////// JOHN 
  
  if(DEBUG) {cout << "Lepton type = " << Lep.GetFlavour() << endl;}
  int Idx_Closest    = GenMatchedIdx(Lep,gens);
  if(DEBUG) {cout << "Idx_Closest = " << Idx_Closest << endl;}
  if (Idx_Closest < 0){

    /// Check All particles since no matched status 1 lepton is found

    double mindR=0.4;
    int closest_pid=9999;
    int Mclosest_pid=9999;

    vector<TString> MatchedGens;
    vector<int> MatchedGensID;
    vector<int> CloseGensID;
    int closest_id=-1;
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if(gen.Pt() < 5) continue;
      if(fabs(gen.PID()) == 23) continue;
      if(fabs(gen.PID()) == 24) continue;
      if(fabs(gen.PID()) == 12) continue;
      if(fabs(gen.PID()) == 14) continue;
      if(fabs(gen.PID()) == 16) continue;
      if(fabs(gen.PID()) == 6) continue;

      if(Lep.DeltaR(gen) < 0.4)  {
        if(fabs(gen.Pt() - Lep.Pt() ) / Lep.Pt() < 0.25) {
    
    if(MatchGenPID(fabs(gen.PID()), gens, gen) != "") {
      return MatchGenPID(fabs(gen.PID()),gens,  gen);
    }
    MatchedGensID.push_back(i);
  }
  //cout << "CloseGensID.push_back " << i << endl;
  CloseGensID.push_back(i);
      }
      
      if(Lep.DeltaR(gen) < mindR ){
  mindR=Lep.DeltaR(gen) ;
  closest_pid= fabs(gen.PID());
  Mclosest_pid= fabs(gens[gen.MotherIndex()].PID());
  
  closest_id = i;
      }
    }
    
    mindR=0.4;
    TString pid_matched="NULL";
    for(auto j : MatchedGensID) {
      if(Lep.DeltaR(gens[j]) < mindR){
  mindR=Lep.DeltaR(gens[j]);
  
  if(fabs(gens[j].Pt() - Lep.Pt() ) / Lep.Pt() < 0.25) {
    pid_matched=Lep.PIDToString(fabs(gens[j].PID()));
    
    //cout << "pid_matched = " << pid_matched <<  " " <<  j <<endl;
    int mind = gens.at(j).MotherIndex();
    int pdid = j;
    //cout << "mind = " << mind << endl;
    while(std::find(CloseGensID.begin(), CloseGensID.end(), mind) != CloseGensID.end()) {
      if(fabs(gens.at(mind).PID()) <= 6)  break;
            if(fabs(gens.at(mind).PID()) == 2212)  break;

      pdid = mind;
      mind = gens.at(mind).MotherIndex();
    }
    
          if(fabs(gens[pdid].PID()) == 423) return "D";
    if(fabs(gens[pdid].PID()) == 421) return "D0";
    if(fabs(gens[pdid].PID()) == 431) return "D";
    if(fabs(gens[pdid].PID()) == 433) return "D";
    if(fabs(gens[pdid].PID()) == 413) return "D";
    if(fabs(gens[pdid].PID()) == 411) return "D+";
          if(fabs(gens[pdid].PID()) == 511) return "B0";
          if(fabs(gens[pdid].PID()) == 531) return "B";
          if(fabs(gens[pdid].PID()) == 533) return "B";
          if(fabs(gens[pdid].PID()) == 513) return "B";
          if(fabs(gens[pdid].PID()) == 521) return "B+";
          if(fabs(gens[pdid].PID()) == 523) return "B";

  }
      }
    }
    
    

    if(closest_pid == 11){
      vector<int> history = TrackGenSelfHistory(gens[closest_id], gens);
      if(fabs(gens.at(history[1]).PID()) == 23) return "electron";
      if(fabs(gens.at(history[1]).PID()) == 15) return "tau";
    }

    if(closest_pid == 13){
      vector<int> history = TrackGenSelfHistory(gens[closest_id], gens);
      if(fabs(gens.at(history[1]).PID()) == 23) return "muon";
      if(fabs(gens.at(history[1]).PID()) == 15) return "tau";
    }

    
    if(pid_matched!= "NULL")return pid_matched;
    
    if(closest_pid < 4) return "q";
    if(closest_pid == 4) return "c";
    if(closest_pid == 5) return "b";
    if(closest_pid == 11) return "electron";
    if(closest_pid == 13) return "muon";

    if(closest_pid == 22) return "ph";
    if(closest_pid == 21) return "g";
    if(closest_pid == 211)return "pi+";
    if(closest_pid == 111) return "pi0";
    if(closest_pid == 443) return "JPsi";
    if(closest_pid == 411) return "D+";
    if(closest_pid == 321) return "K+";
    if(closest_pid == 313) return "K*";
    if(closest_pid == 323) return "K*";

    if(closest_pid == 130) return "K";
    if(closest_pid == 310) return "K";
    if(closest_pid == 311) return "K";
    if(closest_pid == 10313) return "K";

    if(closest_pid == 223) return "LightMeson";
    if(closest_pid == 411) return "D+";
    if(closest_pid == 421) return "D0";
    if(closest_pid == 511) return "B0";
    if(closest_pid == 521) return "B+";
    if(closest_pid == 513) return "B";
    if(closest_pid == 523) return "B";
    if(closest_pid == 531) return "B";
    if(closest_pid == 533) return "B";
    
    int pdid2=-1;
    while(std::find(CloseGensID.begin(), CloseGensID.end(), closest_id) != CloseGensID.end()) {
      if(fabs(gens.at(closest_id).PID()) <= 6)  break;
      pdid2 = closest_id;
      closest_id = gens.at(closest_id).MotherIndex();
    }    
    
    if(pdid2 > 0 )return Lep.PIDToString(gens.at(pdid2).PID());
    //    else {
    //  PrintMatchedGen(gens,Lep);
    // }
    
    if(closest_pid != 9999 )return Lep.PIDToString(closest_pid) ;
            
            
  }
  else{
    
    if(DEBUG) {cout << "MATCHED CLOSTEST  = fabs(gens.at(gens[Idx_Closest].MotherIndex()).PID()) " << fabs(gens.at(gens[Idx_Closest].MotherIndex()).PID())  <<endl;}
    
    
    vector<int> history; //= TrackGenSelfHistory(gens[Idx_Closest], gens);                                                                   
    
    int motherindex = gens[Idx_Closest].MotherIndex();
    history.push_back(Idx_Closest);
    while(motherindex > 1){
      history.push_back(motherindex);
      motherindex = gens[motherindex].MotherIndex();
    }

    
    // for (std::vector<int>::reverse_iterator it = history.rbegin(); it != history.rend(); ++it)      {
    
    
    for(auto ig : history){
      if(fabs(gens.at(ig).PID()) == 521)return "B+"; 
      if(fabs(gens.at(ig).PID()) == 511)return "B0"; 
      if(fabs(gens.at(ig).PID()) == 421)return "D0"; 
      if(fabs(gens.at(ig).PID()) == 411)return "D+";
    }
    for(auto ig : history){
      if(Lep.LeptonFlavour()==Lepton::MUON && Lep.PIDToString(gens.at(ig).PID()) == "muon") continue;
      if(Lep.LeptonFlavour()==Lepton::ELECTRON && Lep.PIDToString(gens.at(ig).PID()) == "electron") continue;
      
      if(Lep.PIDToString(gens.at(ig).PID()) != "NULL") {
  if(Lep.PIDToString(gens.at(ig).PID()) == "D")  return Lep.PIDToString(gens.at(ig).PID());
  if(Lep.PIDToString(gens.at(ig).PID()) == "B")  return Lep.PIDToString(gens.at(ig).PID());
      }
    }

    for(auto ig : history){
      if(Lep.LeptonFlavour()==Lepton::MUON && Lep.PIDToString(gens.at(ig).PID()) == "muon") continue;
      if(Lep.LeptonFlavour()==Lepton::ELECTRON && Lep.PIDToString(gens.at(ig).PID()) == "electron") continue;
      if(Lep.PIDToString(gens.at(ig).PID()) != "NULL") return Lep.PIDToString(gens.at(ig).PID());
    }
    
    
    int mother_id = gens[Idx_Closest].MotherIndex();

    while(fabs(gens.at(mother_id).PID()) == 13)  mother_id = gens[mother_id].MotherIndex();
    while(fabs(gens.at(mother_id).PID()) == 11)  mother_id = gens[mother_id].MotherIndex();
    
    if(fabs(gens.at(mother_id).PID()) == 2212 ) {

      for(unsigned int i=2; i<gens.size(); i++){
        Gen gen = gens.at(i);
        if(Lep.DeltaR(gen) < 0.4)  {
          if(fabs(gen.PID()) == 22 && gen.Status()==1 && (fabs(gens.at(gen.MotherIndex()).PID()) == 111)) return "pi0ph";
          if((fabs(gens.at(gen.MotherIndex()).PID()) == 111)) return "pi0";
          if(fabs(gen.PID()) == 22 ) return "conv";

        }
      }
      return "proton";
    }
    
    if(fabs(gens.at(mother_id).PID()) == 2212)return "proton";
    if(fabs(gens.at(mother_id).PID()) == 22)return "ph";
    if(fabs(gens.at(mother_id).PID()) == 130)return "K0L";
    if(fabs(gens.at(mother_id).PID()) == 443)return "JPsi";
    if(fabs(gens.at(mother_id).PID()) == 15)return "tau";

    if(fabs(gens[Idx_Closest].PID()) != 11){
      if(fabs(gens.at(mother_id).PID()) == 11 )return "electron";
    }
    if(fabs(gens[Idx_Closest].PID()) !=13){
      
      if(fabs(gens.at(mother_id).PID()) == 13 )return "muon";
    }
  }
  
  return "__";
}

void AnalyzerCore::PrintMatchedGen(std::vector<Gen>& gens,const Lepton& Lep){

  cout << "===========================================================" << endl;
  cout << "RunNumber:EventNumber = " << run << ":" << event << endl;
  cout << "index\tPID\tStatus\tMIdx\tMPID\tStart\tPt\tEta\tPhi\tM" << endl;
  
  int Idx_Closest    = GenMatchedIdx(Lep,gens);
  cout << "Matched gen = " << Idx_Closest << endl;

  float minDr=1.;
  unsigned int closest_index = 0;
  for(unsigned int i=2; i<gens.size(); i++){

    Gen gen = gens.at(i);
    if(Lep.DeltaR(gen) < minDr)  {
      minDr = Lep.DeltaR(gen) ;
      closest_index = i;
    }
  }

  for(unsigned int i=2; i<gens.size(); i++){

    Gen gen = gens.at(i);
    vector<int> history = TrackGenSelfHistory(gen, gens);
    
    TString addon="";
    if(closest_index == i) addon = " ----> ";
    else addon="";
    if (Idx_Closest == i)  addon += " ----> ";
    //if(Lep.DeltaR(gen) < 0.4)     cout << "\033[91m\033[107m"<<  i << "\t" << gen.SPID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() << "\t" << gens.at(gen.MotherIndex()).SPID()<< "\t" << history[0] << "\t \033[0m";

    if(Lep.DeltaR(gen) < 0.4)     cout <<  i << "\t" << gen.SPID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() << "\t" << gens.at(gen.MotherIndex()).SPID()<< "\t" << history[0] << "\t";
    //else cout << i << "\t" << gen.SPID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() << "\t" << gens.at(gen.MotherIndex()).SPID()<< "\t" << history[0] << "\t";

    //if(Lep.DeltaR(gen) < 0.4)  printf("\033[91m\033[107m%.2f\t%.2f\t%.2f\t%.2f\033[0m\n",gen.Pt(), gen.Eta(), gen.Phi(), gen.M());
    if(Lep.DeltaR(gen) < 0.4)  printf("%.2f\t%.2f\t%.2f\t%.2f " +addon+ "\n",gen.Pt(), gen.Eta(), gen.Phi(), gen.M());
    //else  printf("%.2f\t%.2f\t%.2f\t%.2f\n",gen.Pt(), gen.Eta(), gen.Phi(), gen.M());
    
  }

}



void AnalyzerCore::PrintEvent(AnalyzerParameter param,TString selection,double w){

  // if run_timestamp is not set dont run                                                                                                        
  if (timestamp == "-999999") return;

  FillEventComparisonFile(param,selection,string(timestamp), w);

  cout <<  "selection = " << selection << " RunNumber:EventNumber = " << run << ":" << event << endl;

}

bool AnalyzerCore::IsCF(Electron el, std::vector<Gen> truthColl){

  int charge_el_reco = el.Charge();
  Lepton l = Lepton(el);
  
  int LepType = GetLeptonType_JH(el, truthColl);
  
  if(LepType<= 0)    return false;
  int Idx_Closest    = GenMatchedIdx(el,truthColl);
  int IdxType_NearEl = LepType>3? GetPrElType_InSameSCRange(Idx_Closest, truthColl, "IdxType"):Idx_Closest;
  int Idx_NearEl     = LepType>3? IdxType_NearEl/10:Idx_Closest;

  // METHOD 1
  // This does not include internal conv CF
  //bool method1=false;
  //Gen gen_el= GetGenMatchedLepton(l, truthColl);
  //int pdgid = gen_el.PID() ;
  //if( (pdgid * charge_el_reco) > 0) method1=true;
  
  //if((charge_el_reco*truthColl.at(Idx_NearEl).PID()>0) != method1)cout << (charge_el_reco*truthColl.at(Idx_NearEl).PID()>0) << "  method 1= " << method1 << endl;

  //  if((charge_el_reco*truthColl.at(Idx_NearEl).PID()>0) != method1) cout << "Lep Type = " << LepType << endl;

  if(charge_el_reco*truthColl.at(Idx_NearEl).PID()>0) return true;

  return false;
}

bool AnalyzerCore::IsCF(Muon mu, std::vector<Gen> truthColl){

  int charge_mu_reco = mu.Charge();
  Lepton l = Lepton(mu);

  int LepType = GetLeptonType_JH(mu, truthColl);

  if(LepType<= 0)    return false;
  int Idx_Closest    = GenMatchedIdx(mu,truthColl);
  int IdxType_NearEl = LepType>3? GetPrElType_InSameSCRange(Idx_Closest, truthColl, "IdxType"):Idx_Closest;
  int Idx_NearEl     = LepType>3? IdxType_NearEl/10:Idx_Closest;

  if(charge_mu_reco*truthColl.at(Idx_NearEl).PID()>0) return true;

  return false;
}



Gen AnalyzerCore::GetGenMatchedLepton(const Lepton& lep, const std::vector<Gen>& gens){

  //==== find status 1 lepton

  int reco_PID = -999;
  if(lep.LeptonFlavour()==Lepton::ELECTRON) reco_PID = 11;
  else if(lep.LeptonFlavour()==Lepton::MUON) reco_PID = 13;
  else{
    cout << "[AnalyzerCore::GetGenMatchedLepton] input lepton flavour not set" << endl;
    exit(EXIT_FAILURE);
  }

  double min_dR = 0.1;
  Gen gen_closest;
  for(unsigned int i=2; i<gens.size(); i++){

    Gen gen = gens.at(i);

    //==== Status 1
    if( gen.Status() != 1 ) continue;
    //==== PID
    if( abs( gen.PID() ) != reco_PID ) continue;
    //==== reject ISR?
    if( gen.MotherIndex() < 0 ) continue;
    //==== dR matching
    if( gen.DeltaR( lep ) < min_dR ){
      min_dR = gen.DeltaR( lep ) ;
      gen_closest = gen;
    }

  }

  return gen_closest;

}

Gen AnalyzerCore::GetGenMatchedPhoton(const Lepton& lep, const std::vector<Gen>& gens){
//Find if there is a photon candidate for a source of external conversion (similar pt, direction of lepton)

  double min_dR = 0.2;//1)
  Gen gen_closest;
  double pt_min = 10., dPtRelmax=0.5, dRmax=0.2;//1)
  for(unsigned int i=2; i<gens.size(); i++){

    Gen gen = gens.at(i);

    if( gen.MotherIndex() < 0 ) continue;
    if( !( abs(gen.PID())==22 && (gen.Status()==1 || gen.Status()==23) ) ) continue;//2)
    if( gen.Pt() < pt_min ) continue;
    if( !(lep.Pt()/gen.Pt()>(1.-dPtRelmax) && lep.Pt()/gen.Pt()<(1.+dPtRelmax)) ) continue;
    if( gen.DeltaR( lep ) > dRmax ) continue;
    if( gen.Status()==23 && !IsFinalPhotonSt23_Public(gens) ) continue;//2)

    if( gen.DeltaR( lep ) < min_dR ){
      min_dR = gen.DeltaR( lep ) ;
      gen_closest = gen;
    }

  }

  return gen_closest;

//1) Validity of cuts are checked for electron with PT>10. Note that the previous dPtRel<0.2 cut in the CatNtuple analysis was optimized for PT(e)>25.
//   External conversion rate proportionate to M^{-2}, thus muon external conversion rate is negligible.
//2) As the algorithm targets stable photon near lepton, only status-1 photon should be investigated theoretically. 
//   However, in some events, last status in photon history is 23. This is believed to be due to skimming of GEN history between pythia & MiniAOD. 
//   This case is also covered here. Note that IsFinalPhotonSt23 has to analyze whole history, i.e. computationally heavier part in this function.
//   Therefore it is put at last part of criteria for efficient functioning.
//For more details about the optimization, ask J.B..
}

vector<int> AnalyzerCore::TrackGenSelfHistory(const Gen& me, const std::vector<Gen>& gens){
//returns {index of the first history of the gen, index of the last history of the gen's mother}

  int myindex = me.Index();

  if(myindex<2){
    vector<int> out = {myindex, -1};
    return out;
  }

  int mypid = gens.at(myindex).PID();

  int currentidx = myindex;
  int motherindex = me.MotherIndex();

  while(gens.at(motherindex).PID() == mypid){

    //==== Go one generation up
    currentidx = motherindex;
    motherindex = gens.at(motherindex).MotherIndex();

    if(motherindex<0) break;
  }

  vector<int> out = {currentidx, motherindex};

  return out;

}

bool AnalyzerCore::IsFromHadron(const Gen& me, const std::vector<Gen>& gens){

  bool out = false;

  int myindex = me.Index();
  if(myindex<2) return true;

  vector<int> my_history = TrackGenSelfHistory(me, gens);
  Gen          Start = gens.at( my_history[0] );
  Gen MotherOf_Start = gens.at( my_history[1] );

  //==== Status 21~29 are from hardprocess
  //==== Means it's lepton from hardprocess
  //==== e.g., leptons from Z start their lives with status 23
  if( 20 < Start.Status() && Start.Status() < 30 ) return false;

  Gen current_me = Start; // me will always be Start
  Gen current_mother = Start; // initializing
  while( current_mother.Index() >= 2 ){

    vector<int> current_history = TrackGenSelfHistory(current_me, gens);

    //==== Go one generation up
    //==== not being used after this line
    //==== not a bug!!
    current_me = gens.at(current_history[1]);

    //==== Now look at mother of previous "me"
    current_mother = gens.at(current_history[1]);

    vector<int> current_mother_history = TrackGenSelfHistory(current_mother, gens);
    Gen StartOf_current_mother = gens.at(current_mother_history[0]);
    int current_mother_PID = current_mother.PID();

    //==== From Z,W,H,t
    if( current_mother_PID==23 || current_mother_PID==24 || current_mother_PID==25 || current_mother_PID==6 ){
      out = false;
      break;
    }
    //==== From Signal
    if( IsSignalPID(current_mother_PID) ){
      out = false;
    }
    if( (current_mother_PID==11 || current_mother_PID==13 || current_mother_PID==15 || current_mother_PID==22) && (StartOf_current_mother.Status()>20 && StartOf_current_mother.Status()<30)){
      out = false;
      break;
    }
    if( current_mother_PID>50 ){
      out=true;
      break;
    }
    if( (current_mother_PID>=1 && current_mother_PID<=5) || current_mother_PID==21 ){
      out=true; 
      break;
    }

  }

  return out;

}

std::vector<Lepton *> AnalyzerCore::LeptonUsePtParton(std::vector<Lepton *> leps){

  std::vector<Lepton *> out;

  for(unsigned int i=0; i<leps.size(); i++){
    //==== muons is a const vector. So in this function, we have to copy the elements like below
    Lepton *this_lepton = leps.at(i);
    if(this_lepton->LeptonFlavour()==Lepton::MUON) this_lepton->SetPtEtaPhiM( leps.at(i)->PtParton(0.697,0.64,0.64), leps.at(i)->Eta(), leps.at(i)->Phi(), leps.at(i)->M() ); //FIXME JH : year, sample dependent...
    else if(this_lepton->LeptonFlavour()==Lepton::ELECTRON) this_lepton->SetPtEtaPhiM( leps.at(i)->PtParton(1,0.15,0.2), leps.at(i)->Eta(), leps.at(i)->Phi(), leps.at(i)->M() ); //FIXME JH : year, sample dependent...
    out.push_back( this_lepton );
  }

  std::sort(out.begin(),       out.end(),        PtComparingPtr);

  return out;

}

bool AnalyzerCore::ConversionSplitting(std::vector<Lepton *> leps, bool RunConvMode,  int nlep){
  
  if(!RunConvMode) return true;
  if(IsData) return true;
  
  int nlep_pt15(0);

  for(auto ilep : leps){

    if(ilep->Pt() > 15.) nlep_pt15++;
  }
 
  //if(MCSample.Contains("WGTo") ||MCSample.Contains("ZGTo")){
  //  if(nlep_pt15 ==3) return true;
  //  else return false;
  //}
  //else if(MCSample.Contains("DYJet") || MCSample.Contains("WJet")) {
  //  if(nlep_pt15 !=3) return true;
  //  else return false;
  //}
  
  //if(MCSample.Contains("ZGTo")){
  //  if(nlep_pt15 ==3) return true;
  //  else return false;
  //}
  //else if(MCSample.Contains("DYJet")) {
  //  if(nlep_pt15 !=3) return true;
  //  else return false;
  //}

  return true;
 
}

bool AnalyzerCore::ConversionVeto_Backup(std::vector<Lepton *> leps,const std::vector<Gen>& gens){ //JH : back-up

  // function vetos conversion events in DY/X+G                                                                                                  
  // since Z/G and WG have cut on photon in GEN need to overlap with DY                                                                          
  // Photon Cut is 15 GeV                                                                                                                        
  //                                                                                                                                             
  
  if (IsData) return true;
  
  double ph_pt=-1;
  bool photon_found=false;
  bool GENTMatched=false;
  for(auto ilep : leps){
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if(ilep->DeltaR(gen) < 0.2) {
      if(gen.PID() == 22 && gen.isPromptFinalState()) ph_pt = gen.Pt();
        if(gen.PID() == 22 && gen.isPromptFinalState() && gen.Pt()> 15.) {
          photon_found=true;
          GENTMatched=true;
          for(unsigned int j=2; j<gens.size(); j++){
            if(!(fabs(gens.at(j).PID()) <7 || fabs(gens.at(j).PID()) == 21)) continue;
            if(gens.at(j).Status() != 23) continue;
            if(gens.at(j).DeltaR(gen) <0.05)GENTMatched=false;
          }
        }
      }
    }
    if(GENTMatched) break;
  }

  //if(!photon_found) {
  //  cout << "No Matching photon found " << endl;
  //  PrintGen(gens);
  //}
  
  //if(MCSample.Contains("WGTo") ||MCSample.Contains("ZGTo") )   {
  //  if(!photon_found) return true;
  //  if(!GENTMatched)cout << "GHENT Conv event removed in " << MCSample << endl;
  //  cout << "Matched photon pt = " << ph_pt << endl;
  //  return GENTMatched;
  //}
  //else if(MCSample.Contains("DYJet") || MCSample.Contains("WJet") ){
  //  if(!photon_found) return true;
  //  cout << "Matched photon pt = " << ph_pt << endl;
  //  return !GENTMatched;
  //}

  //if(MCSample.Contains("ZGTo") )   {
  //  if(!photon_found) return true;
  //  if(!GENTMatched)cout << "GHENT Conv event removed in " << MCSample << endl;
  //  cout << "Matched photon pt = " << ph_pt << endl;
  //  return GENTMatched;
  //}
  //else if(MCSample.Contains("DYJet")){
  //  if(!photon_found) return true;
  //  cout << "Matched photon pt = " << ph_pt << endl;
  //  return !GENTMatched;
  //}

  return true;
}

bool AnalyzerCore::ConversionVeto(std::vector<Lepton *> leps,const std::vector<Gen>& gens){

  // function vetos conversion events in DY/X+G                                                                                                  
  // since Z/G and WG have cut on photon in GEN need to overlap with DY                                                                          
  // Photon Cut is 15 GeV                                                                                                                        
  //                                                                                                                                             
  
  if (IsData) return true;

  if(leps.size()==0) return false;

  double ph_pt=-1;
  bool photon_found=false;
  bool GENTMatched=false;
  for(auto ilep : leps){
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if(ilep->DeltaR(gen) < 0.2) {
        if(gen.PID() == 22 && gen.isPromptFinalState()) ph_pt = gen.Pt();
        if(gen.PID() == 22 && gen.isPromptFinalState() && gen.Pt()> 15.) {
          photon_found=true;
          GENTMatched=true;
          for(unsigned int j=2; j<gens.size(); j++){
            if(!(fabs(gens.at(j).PID()) <7 || fabs(gens.at(j).PID()) == 21)) continue;
            if(gens.at(j).Status() != 23) continue;
            if(gens.at(j).DeltaR(gen) <0.05)GENTMatched=false;
          }
        }
      }
    }
    if(GENTMatched) break;
  }

  //if(!photon_found) {
  //  cout << "No Matching photon found " << endl;
  //  PrintGen(gens);
  //}

  //if(MCSample.Contains("WGTo") ||MCSample.Contains("ZGTo") )   {
  //  if(leps.back()->Pt()>15.) return true;
  //  else return GENTMatched;
  //}
  //else if(MCSample.Contains("DYJet") || MCSample.Contains("WJet") ){
  //  if(leps.back()->Pt()>15.) return false;
  //  else return !GENTMatched;
  //}

  //if(MCSample.Contains("ZGTo") )   {
  //  if(leps.back()->Pt()>15.) return true;
  //  else return GENTMatched;
  //}
  //else if(MCSample.Contains("DYJet")){
  //  if(leps.back()->Pt()>15.) return false;
  //  else return !GENTMatched;
  //}

  return true;
}



int AnalyzerCore::GetPrElType_InSameSCRange_Public(int TruthIdx, const std::vector<Gen>& TruthColl){
//Abbreviation: Get Prompt Electron Type In Same Supercluster Range (Public (shared) version of J.B.'s original gen-matching code)
//Prompt e>eee (int. conv.) case, collimated electrons can be merged in one SC & track is selected among them, and reconstructed as single electron. 
//In this case, still there will be a nearby prompt electron of LepType 1-3 within supercluster merging range.
//Supercluster merging range: |dphi|<0.3/0.3 (EB/EE), |deta|<0/0.07 (EB/EE) (additionally put 0.03 for marginal difference from size of crystals)
//Ref:JINST 10 (2015) P06005, arXiv:1502.02701
//Return Value: 1/2/3:LeptonType-1/2/3 electron found in same SC range
//             -1: No LeptonType-1/2/3 electron found in same SC range
//
//Note: Hadronic origins are not included in this algo. as I am not sure of effect of many nearby particles in jets on the ele-reco performance.
//      + it is not of my interest yet.

  if(TruthIdx<2) return false;
  if(abs(TruthColl.at(TruthIdx).PID())!=11) return false;
  if(TruthColl.at(TruthIdx).Status()!=1) return false;

  double dPhiMax=0.3, dEtaMax=0.1;
  int NearbyElType=-1;

  for(unsigned int it_gen=2; it_gen<TruthColl.size(); it_gen++){
    if(TruthColl.at(it_gen).Status()!=1) continue;
    if(abs(TruthColl.at(it_gen).PID())!=11) continue;
    if(fabs(TruthColl.at(TruthIdx).Eta()-TruthColl.at(it_gen).Eta())>dEtaMax) continue;
    if(TruthColl.at(TruthIdx).DeltaPhi(TruthColl.at(it_gen))>dPhiMax) continue;

    int LepType = GetLeptonType_Public(it_gen, TruthColl);
    if(!(LepType>=1 && LepType<=3)) continue;
    NearbyElType  = LepType;
  }

  return NearbyElType;
}


int AnalyzerCore::GetLeptonType_Public(int TruthIdx, const std::vector<Gen>& TruthColl){
//Type : 1:EW-Prompt / 2: BSM-Prompt / 3:EW/BSM-Prompt-Tau Daughter 
//       4:Internal Conversion from Soft QED Radiation (PS-level) / 5:Internal Conversion from Hard Process Photon (ME-level)
//      -1:Unmatched & not EW Conversion candidate (mis-reco. or external conversion within jets)
//      -2:Hadron Daughter / -3:Daughter of Tau from Hadron or Parton / -4:Internal Conversion Daughter having hadronic origin
//      -5:External Conversion from Hard process photon / -6:External conversion from t/EWV/EWlep
//      (-4:Daughter of Non-hard scattered photon & has parton or hadron ancestor OR implicit Conv from quark)
//       0:Error / >0: Non-fake: Non-hadronic origin / <0 : Fakes: Hadronic origin or external conversion
//Note: There is no physical distinction between type 4 vs. 5 and type -5 vs. -6. The distinction is only intended for straightforward debugging.


  //Only consider Status 1 lepton
  if(TruthIdx<2) return 0;
  if(TruthColl.at(TruthIdx).Status()!=1) return 0;
  if( !(fabs(TruthColl.at(TruthIdx).PID())==11 || fabs(TruthColl.at(TruthIdx).PID())==13) ) return 0;


  int LeptonType=0;
  int MPID=0, GrMPID=0;
  vector<int> my_history, mom_history, grmom_history;
  int LastSelfIdx=-1, MotherIdx=-1, LastSelfMIdx=-1, GrMotherIdx=-1, LastSelfGrMIdx=-1;
  int Status_orig=0, MStatus_orig=0, MStatus_last=0, GrMStatus_orig=0, GrMStatus_last=0;
  bool HadronicOrigin = false;

  my_history      = TrackGenSelfHistory(TruthColl.at(TruthIdx), TruthColl);
  LastSelfIdx     = my_history[0];
  MotherIdx       = my_history[1];
  Status_orig     = TruthColl.at(LastSelfIdx).Status();
  HadronicOrigin  = IsFromHadron(TruthColl.at(TruthIdx), TruthColl);
  
  if(   MotherIdx!=-1   ){ mom_history  = TrackGenSelfHistory(TruthColl.at(MotherIdx), TruthColl);
                           LastSelfMIdx = mom_history[0];
                           GrMotherIdx  = mom_history[1];
                           MPID         = TruthColl.at(MotherIdx).PID();
                           MStatus_orig = TruthColl.at(LastSelfMIdx).Status();
                           MStatus_last = TruthColl.at(MotherIdx).Status();
                         }
  if(  GrMotherIdx!=-1  ){ grmom_history  = TrackGenSelfHistory(TruthColl.at(GrMotherIdx), TruthColl);
                           LastSelfGrMIdx = grmom_history[0];
                           GrMPID         = TruthColl.at(GrMotherIdx).PID();
                           GrMStatus_orig = TruthColl.at(LastSelfGrMIdx).Status();
                           GrMStatus_last = TruthColl.at(GrMotherIdx).Status();
                         }

  if     ( TruthIdx==-1 )                                       LeptonType= 0;
  else if( fabs(MPID)==23 || fabs(MPID)==24 || fabs(MPID)==25 ) LeptonType= 1;
  else if( IsSignalPID(MPID) )                                  LeptonType= 2;
  else if( Status_orig>20 && Status_orig<30 )                   LeptonType= 1;//1)
  else if( fabs(MPID)>50 )                                      LeptonType=-2;
  else if( fabs(MPID)==15 && MStatus_last==2 ){
           if     ( fabs(GrMPID)==23 || fabs(GrMPID)==24 || fabs(GrMPID)==25 ) LeptonType= 3;
           else if( IsSignalPID(GrMPID) )                                      LeptonType= 3;
           else if( MStatus_orig>20  && MStatus_orig<30  )                     LeptonType= 3;//1)
           else if( HadronicOrigin )                                           LeptonType=-3;//2-a)
           else if( fabs(GrMPID)==22  && GrMStatus_orig>20 && GrMStatus_orig<30 )                     LeptonType= 5;//2-b)
           else if( fabs(GrMPID)==22 )                                                                LeptonType= 4;//2-c)
           else if( (fabs(GrMPID)==11 || fabs(GrMPID)==13 || fabs(GrMPID)==15) && GrMStatus_last!=2 ) LeptonType= 4;//2-d)
           else                                                                                       LeptonType= 0;
         }
  else if( fabs(MPID)==22 ){
           if( MStatus_orig>20 && MStatus_orig<30 )                            LeptonType= 5;//3-a)
           else if( HadronicOrigin )                                           LeptonType=-4;//3-b)
           else if( fabs(GrMPID)==24 || fabs(GrMPID)==23 || fabs(GrMPID)==6  ) LeptonType= 4;//3-c)
           else if( fabs(GrMPID)==11 || fabs(GrMPID)==13 || fabs(GrMPID)==15 ) LeptonType= 4;//3-d)
           else                                                                LeptonType= 0;
         }
  else if( (fabs(MPID)==11 || fabs(MPID)==13 || fabs(MPID)==15) && MStatus_last!=2 && !HadronicOrigin ) LeptonType= 4;//4-a)
  else if( ((fabs(MPID)>=1 && fabs(MPID)<=5) || fabs(MPID)==21) && MStatus_last!=2 )                    LeptonType=-4;//4-b)
  else if( fabs(MPID)==6 ) LeptonType=4;//4-c)
  else LeptonType=0;


  return LeptonType;

//**footnote
//These are based on observation in DY,ZG,TT sample(DY,ZG:amcnlo+pythia, TT:powheg+pythia) for other PS generator, convention may differ.
//1) In MG5aMC@NLO generator, output of ME level generation does not have specific guage field mother if it is not on-shell specific generation.
//   e.g. u u~ > l+ l- -> fabs(MID)=1 .
//   This line is put after checking prompt mother PID checking, in order to distinguish type 1 and 2.
//2-a) e.g. a)Had > ta+X, ta>l+2nu b) q>ta+X in jet fragmentation (ta is not from hard process (ME), as it was already checked in prev. step)
//2-b) e.g. gamma>ta(+)+ta(-)+X, ta>lnu (St=2)
//2-c) e.g. " " " " " " " " " " " " " " " " " ", but soft gamma case. this is not observed in test sample but put here just in case.
//          (hadronic origin was already checked before, so no need to worry about contamination from hadronic origins at this line)
//2-d) e.g. l>tata..+l.. , ta>l+2nu (tau pair produced from internal conversion), photon radiation is not decay: GenStatus!=2
//3-a) e.g. hard process gamma>ll
//3-b) e.g. a) Had>gamma+X, gamma>ll+X (in PS+Had stage intermediate process is omitted you see just Had>Nphoton+Mhadrons+..)
//          b) q>gamma+q, gamma>ll+X in jet fragmentation or radiations of tops.
//          c) gluon>Ngamma+Mhadrons in jet fragmentation (actually observed in samples)
//3-c) e.g. W+>W+ gamma, or t>t+gamma, gamma>ll+X, not yet observed in test sample but possible (upto radiation is observed so far)
//3-d) e.g. ta>ta+gamma, gamma>ll+X, tau not from hadron(e.g. pp>tata)
//4-a) e.g. EW lep l, l>lll... just implicit conversion. 
//4-b) e.g. q or g> Nlepton +MHadrons... in parton shower history
//4-c) e.g. t>t+ll.. implicit conversion
}



int AnalyzerCore::GetLeptonType(const Lepton& lep, const std::vector<Gen>& gens){

  //==== [Type]
  //====  1 : EWPrompt
  //====  2 : Signal Daughter
  //====  3 : EWtau daughter
  //====  4 : Internal Conversion daughter from t/EWV/EWlep(Implicit,Explicit)
  //====  5 : Internal Conversion daughter from HardScatterPhoton
  //==== -1 : Unmatched & not EW Conversion candidate
  //==== -2 : Hadron daughter
  //==== -3 : Daughter of tau from hadron or parton
  //==== -4 : Internal conversion daughter(implicit,explicit) having hadronic origin 
  //==== -5 : External conversion candidate(Hard scattered photon)
  //==== -6 : External conversion from t/EWV/EWlep
  //==== (-4: Daughter of Non-hard scattered photon & has parton or hadron ancestor OR implicit Conv from quark)
  //==== -------------------------------------------------------------
  //====  0 : Error
  //==== >0 : Non-fake; Non-hadronic origin
  //==== <0 : Fakes   ; Hadronic origin or external conversion

  Gen gen_closest = GetGenMatchedLepton(lep, gens); // if gen_closest.IsEmpty(), we should then look for a near photon
  //cout << "[AnalyzerCore::GetLeptonType] [Reco] pt = " << lep.Pt() << "\t, eta = " << lep.Eta() << endl;
  //cout << "[AnalyzerCore::GetLeptonType] [Gen] Index = " << gen_closest.Index() << endl;

/*
  //==== 1) Using Simple Flags
  //==== Prompt
  if( gen_closest.isPromptFinalState() ){
    return 1;
  }
  //=== From Tau
  else if( gen_closest.isDirectPromptTauDecayProductFinalState() ){
    return 2;
  }
  else{
    //=== Fake
    return -1;
  }
*/  

  //==== 2) Use Gen history

  int LeptonType=0, MatchedTruthIdx=-1;
  //==== No matched gen lepton
  if( gen_closest.IsEmpty() ){
    Gen gen_photon_closest = GetGenMatchedPhoton(lep, gens);
    int NearPhotonType = GetGenPhotonType(gen_photon_closest,gens);
    if     ( NearPhotonType<=0 ) LeptonType=-1;//1)
    else if( NearPhotonType==1 ) LeptonType=-5;//2)
    else if( NearPhotonType==2 ) LeptonType=-6;//3)
  }
  //==== Has macthed gen lepton
  else{
    MatchedTruthIdx = gen_closest.Index();
    LeptonType = GetLeptonType_Public(MatchedTruthIdx, gens);//4)
    if(LeptonType>=4 && LeptonType<=5){//5)
      int NearbyPrElType = GetPrElType_InSameSCRange_Public(MatchedTruthIdx, gens);
      if(NearbyPrElType>0) LeptonType = NearbyPrElType;
    }
  }

  return LeptonType;

//**footnote
//1) matched to no gen-lepton nor gen-photon -> mis-reco.
//   matched to no gen-lepton, but to photon with hadronic origin -> mis-reco. (e.g. pions->e) or external conversion from photon in jets
//2) matched to no gen-lepton, but to photon with non-hadronic origin (hard process) -> external conversion from photon with non-hadronic source (ME-level)
//3) matched to no gen-lepton, but to photon with non-hadronic origin (soft QED radiation) -> external conversion from photon with non-hadronic source (PS-level)
//4) matched to gen-lepton, categorize based on the truth categorization algo.: AnalyzerCore::GetLeptonType_Public(int TruthIdx, std::vector<Gen>& TruthColl)
//5) collimated e/gm objects are merged in SC, hence if there is prompt electron within SC-merging range, reco-electron's properties actually represent pre-QED-FSR prompt-electron,
//   rather than the closest internal conversion electron. Therefore shift the type to the prompt lepton's type.
//- Note: distinction between type 4 vs. 5 and type -5 vs. -6 is unphysical. it is only for debugging.
}

int AnalyzerCore::GetGenPhotonType(const Gen& genph, const std::vector<Gen>& gens){

  //==== [Type]
  //====  0 : Invalid input or Error or HardScatter is input when hardscatter is not final state
  //====  1 : HardScatter / 2: Else prompt daughter(l,V,t)
  //==== -1 : Reserved for unmatched(Not used now) / -2: Hadronic origin

  if(genph.IsEmpty()) return 0;

  int genph_index = genph.Index();

  if( genph_index<2 ) return 0;
  if( !(gens.at(genph_index).PID()==22 && (gens.at(genph_index).Status()==1 || gens.at(genph_index).Status()==23)) ) return 0;

  if(gens.at(genph_index).Status()==23){
    if(IsFinalPhotonSt23_Public(gens)) return 1;
    else                               return 0;
  }//From this pt, only St1 Photon is treated.

  vector<int> phhist = TrackGenSelfHistory(genph, gens);

  Gen          Start = gens.at(phhist[0]);
  Gen MotherOf_Start = gens.at(phhist[1]);
  int MotherOf_Start_PID = abs(MotherOf_Start.PID()); // |PID|

  bool fromhadron = IsFromHadron(genph, gens);

  if     ( 20 < Start.Status() && Start.Status() < 30 ) return 1;//1)
  else if( MotherOf_Start_PID==23 || MotherOf_Start_PID==25) return 1;//2)
  else if( MotherOf_Start_PID==24 || MotherOf_Start_PID==6 || IsSignalPID(MotherOf_Start_PID) ) return 2;//3)
  else if( fromhadron ) return -2;//4)
  else if( MotherOf_Start_PID==11 || MotherOf_Start_PID==13 || MotherOf_Start_PID==15 ) return 2;//5)
  else return 0;

//*footnote
//1) Hard process photon's original status is 20's, unless the history is skimmed in MiniAOD.
//2) If part of intermediate history is skimmed at MinoAOD, then last photon status is 1 without any preceding history lines.
//   In that case, intermediate mother is written in history, and this line catches the case.
//3) top and charged bosons can radiate photons.
// - Note that distinction between 1 and 2 is not physical. You should use both 1 & 2 for prompt photons.
//4) this category does not include top, but photons from hadrons and quarks. Predominantly, in most of the cases they are daughter of pi0.
//   Rarer, but other mesons as eta, B, or even some quarks can also radiate energetic photons.
//5) Photons radiated from lepton FSR. Sometimes they are quite energetic.
}


bool AnalyzerCore::IsFinalPhotonSt23_Public(const std::vector<Gen>& TruthColl){
//In Some XG proc events, it is observed that some of photons' last status is 23. Presumably due to skimming of generator history between pythia and MiniAOD.
//The function returns if this is the case.
//And this is designed only for 1 hard photon case as W+G or Z+G or TT+G

  bool IsFinalGammaStatus23 = false;
  bool HasStatus23Photon    = false;
  for(unsigned int i=2; i<TruthColl.size(); i++){
    int fpid  = fabs(TruthColl.at(i).PID());
    int GenSt = TruthColl.at(i).Status();
    int MPID_direct= TruthColl.at(TruthColl.at(i).MotherIndex()).PID();
    if( !((fpid!=22 && MPID_direct==22) || (fpid==22 && (GenSt==23||GenSt==1))) ) continue;

    vector<int> my_history, mom_history;
    my_history  = TrackGenSelfHistory(TruthColl.at(i), TruthColl);
    int LastSelfIdx     = my_history[0];
    int MotherIdx       = my_history[1];
    int LastSelfSt      = TruthColl.at(LastSelfIdx).Status();
    int LastSelfMIdx=-1, MStatus_orig=-1;
    if(MotherIdx!=-1){
      mom_history  = TrackGenSelfHistory(TruthColl.at(MotherIdx), TruthColl);
      LastSelfMIdx = mom_history[0];
      MStatus_orig = TruthColl.at(LastSelfMIdx).Status();
    }

    if(fpid==22){
      if(GenSt==23) {HasStatus23Photon=true; IsFinalGammaStatus23=true;}
      else if(GenSt==1 && LastSelfSt==23) {IsFinalGammaStatus23=false; break;}//a)
    }
    else if( MPID_direct==22 && MStatus_orig==23 ){ IsFinalGammaStatus23=false; break;}//b)
  }

  if(!HasStatus23Photon) return false;

  return IsFinalGammaStatus23;

//**footnotes
//a) Status-23 photon's last is 1. Thus status-23 photon is not the last history.
//b) Daughter particle of status-23 photon is found. Thus status-23 photon is not the last history.
}


bool AnalyzerCore::IsSignalPID(int pid){

  pid = abs(pid);

  //==== HeavyNeutrino
  if(pid>=9900000) return true;
  //==== ChargedHiggs
  if(pid==36 || pid==37) return true;

  return false;

}

//==== END Gen Matching Tools
//==============================================================

TProfile* AnalyzerCore::GetHistPf(TString histname){

  TProfile *h = NULL;
  std::map<TString, TProfile*>::iterator mapit = maphist_TProfile.find(histname);
  if(mapit != maphist_TProfile.end()) return mapit->second;

  return h;

}

TProfile2D* AnalyzerCore::GetHistPf2D(TString histname){

  TProfile2D *h = NULL;
  std::map<TString, TProfile2D*>::iterator mapit = maphist_TProfile2D.find(histname);
  if(mapit != maphist_TProfile2D.end()) return mapit->second;

  return h;

}

TProfile3D* AnalyzerCore::GetHistPf3D(TString histname){

  TProfile3D *h = NULL;
  std::map<TString, TProfile3D*>::iterator mapit = maphist_TProfile3D.find(histname);
  if(mapit != maphist_TProfile3D.end()) return mapit->second;

  return h;

}

TH1D* AnalyzerCore::GetHist1D(TString histname){

  TH1D *h = NULL;
  std::map<TString, TH1D*>::iterator mapit = maphist_TH1D.find(histname);
  if(mapit != maphist_TH1D.end()) return mapit->second;

  return h;

}

TH2D* AnalyzerCore::GetHist2D(TString histname){

  TH2D *h = NULL;
  std::map<TString, TH2D*>::iterator mapit = maphist_TH2D.find(histname);
  if(mapit != maphist_TH2D.end()) return mapit->second;

  return h;

}

TH3D* AnalyzerCore::GetHist3D(TString histname){
  
  TH3D *h = NULL;
  std::map<TString, TH3D*>::iterator mapit = maphist_TH3D.find(histname);
  if(mapit != maphist_TH3D.end()) return mapit->second;
  
  return h;
  
}

double AnalyzerCore::FillWeightHist(TString label, double _weight){
  
  if(!label.Contains("Syst_"))   FillHist( "weights/"+ label , _weight ,1., 200, -5., 5,"ev weight");
    
  return _weight;
}

void AnalyzerCore::FillProf(TString histname, double xvalue, double yvalue, double weight, int n_bin, double x_min, double x_max, bool IsAverage){ //JH : true by default

  TProfile *this_hist = GetHistPf(histname);
  if( !this_hist ){
    this_hist = new TProfile(histname, "", n_bin, x_min, x_max);
    this_hist->SetDirectory(NULL);
    if(IsAverage) this_hist->SetBit(TH1::kIsAverage);
    maphist_TProfile[histname] = this_hist;
  }

  this_hist->Fill(xvalue, yvalue, weight);

}

void AnalyzerCore::FillProf(TString histname, double xvalue, double yvalue, double zvalue, double weight,
                                              int n_binx, double x_min, double x_max,
                                              int n_biny, double y_min, double y_max){

  TProfile2D *this_hist = GetHistPf2D(histname);
  if( !this_hist ){
    this_hist = new TProfile2D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max);
    this_hist->SetDirectory(NULL);
    maphist_TProfile2D[histname] = this_hist;
  }

  this_hist->Fill(xvalue, yvalue, zvalue, weight);

}

void AnalyzerCore::FillProf(TString histname, double xvalue, double yvalue, double zvalue, double wvalue, double weight,
                                              int n_binx, double x_min, double x_max,
                                              int n_biny, double y_min, double y_max,
                                              int n_binz, double z_min, double z_max){

  TProfile3D *this_hist = GetHistPf3D(histname);
  if( !this_hist ){
    this_hist = new TProfile3D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max, n_binz, z_min, z_max);
    this_hist->SetDirectory(NULL);
    maphist_TProfile3D[histname] = this_hist;
  }

  this_hist->Fill(xvalue, yvalue, zvalue, wvalue, weight);

}

void AnalyzerCore::FillHist(TString histname, double value, double weight, int n_bin, double x_min, double x_max , TString label){

  TH1D *this_hist = GetHist1D(histname);
  if( !this_hist ){
    this_hist = new TH1D(histname, "", n_bin, x_min, x_max);
    this_hist->SetDirectory(NULL);
    this_hist->GetXaxis()->SetTitle(label);
    maphist_TH1D[histname] = this_hist;
  }

  this_hist->Fill(value, weight);

}


void AnalyzerCore::FillHist(TString histname, double value, double weight, int n_bin, double *xbins, TString label){

  TH1D *this_hist = GetHist1D(histname);
  if( !this_hist ){
    this_hist = new TH1D(histname, "", n_bin, xbins);
    this_hist->SetDirectory(NULL);
    this_hist->GetXaxis()->SetTitle(label);

    maphist_TH1D[histname] = this_hist;
  }

  this_hist->Fill(value, weight);

}

void AnalyzerCore::FillHist(TString histname, vector<TString> labels, TString label, double weight){

  TH1D *this_hist = GetHist1D(histname);
  if( !this_hist ){
    this_hist = new TH1D(histname, "", labels.size(), 0, labels.size());
    this_hist->SetDirectory(NULL);
    for(unsigned int i=0; i<labels.size(); i++) this_hist->GetXaxis()->SetBinLabel(i+1,labels[i]);

    maphist_TH1D[histname] = this_hist;
  }

  this_hist->Fill(label, weight);

}

void AnalyzerCore::FillHist(TString histname,
                double value_x, double value_y,
                double weight,
                int n_binx, double x_min, double x_max,
                int n_biny, double y_min, double y_max){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max);
    this_hist->SetDirectory(NULL);
    maphist_TH2D[histname] = this_hist;
  }

  this_hist->Fill(value_x, value_y, weight);

}

void AnalyzerCore::FillHist(TString histname,
                double value_x, double value_y,
                double weight,
                int n_binx, double *xbins,
                int n_biny, double *ybins){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D(histname, "", n_binx, xbins, n_biny, ybins);
    this_hist->SetDirectory(NULL);
    maphist_TH2D[histname] = this_hist;
    //cout << "CReating TH2D " <<  histname << endl;
  }

  this_hist->Fill(value_x, value_y, weight);

}

void AnalyzerCore::FillHist(TString histname,
          double value_x, double value_y,
          double weight,
          int n_binx, double *xbins,
          int n_biny, double y_min, double y_max){


  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D(histname, "", n_binx, xbins, n_biny, y_min, y_max);
    this_hist->SetDirectory(NULL);
    maphist_TH2D[histname] = this_hist;
    //cout << "CReating TH2D " <<  histname << endl;                                                                                                                                                                                          
  }

  this_hist->Fill(value_x, value_y, weight);

}



void AnalyzerCore::FillHist(TString histname,
          double value_x, double value_y, double value_z,
          double weight,
          int n_binx, double x_min, double x_max,
          int n_biny, double y_min, double y_max,
          int n_binz, double z_min, double z_max){
  
  TH3D *this_hist = GetHist3D(histname);
  if( !this_hist ){
    this_hist = new TH3D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max, n_binz, z_min, z_max);
    this_hist->SetDirectory(NULL);
    maphist_TH3D[histname] = this_hist;
  }
  
  this_hist->Fill(value_x, value_y, value_z, weight);
  
}

void AnalyzerCore::FillHist(TString histname,
          double value_x, double value_y, double value_z,
          double weight,
          int n_binx, const double *xbins,
          int n_biny, const double *ybins,
          int n_binz, const double *zbins){
  
  TH3D *this_hist = GetHist3D(histname);
  if( !this_hist ){
    this_hist = new TH3D(histname, "", n_binx, xbins, n_biny, ybins, n_binz, zbins);
    this_hist->SetDirectory(NULL);
    maphist_TH3D[histname] = this_hist;
  }
  
  this_hist->Fill(value_x, value_y, value_z, weight);
  
}

TH1D* AnalyzerCore::JSGetHist1D(TString suffix, TString histname){

  TH1D *h = NULL;

  std::map< TString, std::map<TString, TH1D*> >::iterator mapit = JSmaphist_TH1D.find(suffix);
  if(mapit==JSmaphist_TH1D.end()){
    return h;
  }
  else{

    std::map<TString, TH1D*> this_maphist = mapit->second;
    std::map<TString, TH1D*>::iterator mapitit = this_maphist.find(histname);
    if(mapitit != this_maphist.end()) return mapitit->second;

  }

  return h;

}

void AnalyzerCore::JSFillHist(TString suffix, TString histname, double value, double weight, int n_bin, double x_min, double x_max){

  TH1D *this_hist = JSGetHist1D(suffix, histname);
  if( !this_hist ){

    this_hist = new TH1D(histname, "", n_bin, x_min, x_max);
    (JSmaphist_TH1D[suffix])[histname] = this_hist;

  }

  this_hist->Fill(value, weight);

}

TH2D* AnalyzerCore::JSGetHist2D(TString suffix, TString histname){

  TH2D *h = NULL;

  std::map< TString, std::map<TString, TH2D*> >::iterator mapit = JSmaphist_TH2D.find(suffix);
  if(mapit==JSmaphist_TH2D.end()){
    return h;
  }
  else{

    std::map<TString, TH2D*> this_maphist = mapit->second;
    std::map<TString, TH2D*>::iterator mapitit = this_maphist.find(histname);
    if(mapitit != this_maphist.end()) return mapitit->second;

  }

  return h;

}

void AnalyzerCore::JSFillHist(TString suffix, TString histname,
                  double value_x, double value_y,
                  double weight,
                  int n_binx, double x_min, double x_max,
                  int n_biny, double y_min, double y_max){

  TH2D *this_hist = JSGetHist2D(suffix, histname);
  if( !this_hist ){

    this_hist = new TH2D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max);
    (JSmaphist_TH2D[suffix])[histname] = this_hist;

  }

  this_hist->Fill(value_x, value_y, weight);

}

void AnalyzerCore::JSFillHist(TString suffix, TString histname,
                  double value_x, double value_y,
                  double weight,
                  int n_binx, double *xbins,
                  int n_biny, double *ybins){

  TH2D *this_hist = JSGetHist2D(suffix, histname);
  if( !this_hist ){

    this_hist = new TH2D(histname, "", n_binx, xbins, n_biny, ybins);
    (JSmaphist_TH2D[suffix])[histname] = this_hist;

  }

  this_hist->Fill(value_x, value_y, weight);

}

void AnalyzerCore::WriteHist(){

  outfile->cd();
  for(std::map< TString, TProfile* >::iterator mapit = maphist_TProfile.begin(); mapit!=maphist_TProfile.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }
  for(std::map< TString, TProfile2D* >::iterator mapit = maphist_TProfile2D.begin(); mapit!=maphist_TProfile2D.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }
  for(std::map< TString, TProfile3D* >::iterator mapit = maphist_TProfile3D.begin(); mapit!=maphist_TProfile3D.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }
  for(std::map< TString, TH1D* >::iterator mapit = maphist_TH1D.begin(); mapit!=maphist_TH1D.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }
  for(std::map< TString, TH2D* >::iterator mapit = maphist_TH2D.begin(); mapit!=maphist_TH2D.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      cout << "Making outdir  " << this_suffix << endl;
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    //cout << "Writing " << this_name << endl;
    mapit->second->Write(this_name);
    outfile->cd();
  }
  for(std::map< TString, TH3D* >::iterator mapit = maphist_TH3D.begin(); mapit!=maphist_TH3D.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }

  outfile->cd();
  for(std::map< TString, std::map<TString, TH1D*> >::iterator mapit=JSmaphist_TH1D.begin(); mapit!=JSmaphist_TH1D.end(); mapit++){

    TString this_suffix = mapit->first;
    std::map< TString, TH1D* > this_maphist = mapit->second;


    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);

    for(std::map< TString, TH1D* >::iterator mapit = this_maphist.begin(); mapit!=this_maphist.end(); mapit++){
      mapit->second->Write();
    }

    outfile->cd();

  }

  for(std::map< TString, std::map<TString, TH2D*> >::iterator mapit=JSmaphist_TH2D.begin(); mapit!=JSmaphist_TH2D.end(); mapit++){

    TString this_suffix = mapit->first;
    std::map< TString, TH2D* > this_maphist = mapit->second;

    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);

    for(std::map< TString, TH2D* >::iterator mapit = this_maphist.begin(); mapit!=this_maphist.end(); mapit++){
      mapit->second->Write();
    }

    outfile->cd();

  }


  //=== hist maps                                                                                                                                                                                                                                                             
  if(TimingMap.size() > 0){
    auto itr = TimingMap.find("start");
    double start_time = itr->second;

    for(auto i : TimingMap) {
      cout << i.first << " processing time = " << (i.second- start_time) / CLOCKS_PER_SEC << endl;
    }
  }


  if(TimerMap.size() > 1){
    vector<TString> TimerLabels;
    for(auto i: TimeTagMatcher) TimerLabels.push_back(i.second);
    sort(TimerLabels.begin(), TimerLabels.end());

    TH1* timer_hist = new TH1D("TimeHist", "", TimerLabels.size(), 0, TimerLabels.size());
    for (unsigned int i=0 ; i < TimerLabels.size(); i++) timer_hist->GetXaxis()->SetBinLabel(i+1,TimerLabels[i]);
    timer_hist->SetDirectory(NULL);
    for(auto i : TimerMap) {
      if(i.first != "LATEST"){
        cout << i.first << " processing time = " << i.second << endl;
  timer_hist->Fill(i.first, i.second);
      }
    }

    TDirectory *dir = outfile->GetDirectory("Timer");
    if(!dir)  outfile->mkdir("Timer");
    outfile->cd("Timer");
    
    timer_hist->Write();
    outfile->cd();
    delete timer_hist;
  }
 
 
}


void AnalyzerCore::FillLeptonPlots(std::vector<Lepton *> leps, TString this_region, double weight){

  for(unsigned int i=0; i<leps.size(); i++){

    TString this_itoa = TString::Itoa(i,10);

    Lepton *lep = leps[i];

    FillHist(this_region+"/Lepton_"+this_itoa+"_Pt_"+this_region, lep->Pt(), weight, 1000, 0., 1000.);
    FillHist(this_region+"/Lepton_"+this_itoa+"_Eta_"+this_region, lep->Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/Lepton_"+this_itoa+"_RelIso_"+this_region, lep->RelIso(), weight, 100, 0., 1.);
    FillHist(this_region+"/Lepton_"+this_itoa+"_MiniRelIso_"+this_region, lep->MiniRelIso(), weight, 100, 0., 1.);

    FillHist(this_region+"/Lepton_"+this_itoa+"_dXY_"+this_region, fabs(lep->dXY()), weight, 500, 0., 0.05);
    FillHist(this_region+"/Lepton_"+this_itoa+"_dXYSig_"+this_region, fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
    FillHist(this_region+"/Lepton_"+this_itoa+"_dZ_"+this_region, fabs(lep->dZ()), weight, 500, 0., 0.5);
    FillHist(this_region+"/Lepton_"+this_itoa+"_dZSig_"+this_region, fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
    FillHist(this_region+"/Lepton_"+this_itoa+"_IP3D_"+this_region, fabs(lep->IP3D()), weight, 500, 0., 0.5);
    FillHist(this_region+"/Lepton_"+this_itoa+"_IP3DSig_"+this_region, fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);

    if(lep->LeptonFlavour()==Lepton::ELECTRON){
      Electron *el = (Electron *)lep;
      FillHist(this_region+"/Lepton_"+this_itoa+"_MVANoIso_"+this_region, el->MVANoIso(), weight, 200, -1., 1.);
    }
    else if(lep->LeptonFlavour()==Lepton::MUON){
      Muon *mu = (Muon *)lep;
      FillHist(this_region+"/Lepton_"+this_itoa+"_Chi2_"+this_region, mu->Chi2(), weight, 500, 0., 50.);
      FillHist(this_region+"/Lepton_"+this_itoa+"_TrkRelIso_"+this_region, mu->TrkIso()/mu->TuneP4().Pt(), weight, 100, 0., 1.);
    }
    else{
      cout << "[AnalyzerCore::FillLeptonPlots] lepton flavour wrong.." << endl;
      exit(EXIT_FAILURE);
    }


  }

}

void AnalyzerCore::FillJetPlots(std::vector<Jet> jets, std::vector<FatJet> fatjets, TString this_region, double weight){

  for(unsigned int i=0; i<jets.size(); i++){

    TString this_itoa = TString::Itoa(i,10);
    FillHist(this_region+"/Jet_"+this_itoa+"_Pt_"+this_region, jets.at(i).Pt(), weight, 1000, 0., 1000.);
    FillHist(this_region+"/Jet_"+this_itoa+"_Eta_"+this_region, jets.at(i).Eta(), weight, 60, -3., 3.);

  }

  for(unsigned int i=0; i<fatjets.size(); i++){

    TString this_itoa = TString::Itoa(i,10);
    FillHist(this_region+"/FatJet_"+this_itoa+"_Pt_"+this_region, fatjets.at(i).Pt(), weight, 1000, 0., 1000.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_Eta_"+this_region, fatjets.at(i).Eta(), weight, 60, -3., 3.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_Mass_"+this_region, fatjets.at(i).M(), weight, 3000, 0., 3000.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_SDMass_"+this_region, fatjets.at(i).SDMass(), weight, 3000, 0., 3000.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_LSF_"+this_region, fatjets.at(i).LSF(), weight, 100, 0., 1.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_PuppiTau21_"+this_region, fatjets.at(i).PuppiTau2()/fatjets.at(i).PuppiTau1(), weight, 100, 0., 1.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_PuppiTau31_"+this_region, fatjets.at(i).PuppiTau3()/fatjets.at(i).PuppiTau1(), weight, 100, 0., 1.);
    FillHist(this_region+"/FatJet_"+this_itoa+"_PuppiTau32_"+this_region, fatjets.at(i).PuppiTau3()/fatjets.at(i).PuppiTau2(), weight, 100, 0., 1.);
  }

}



int AnalyzerCore::GetFakeLepSrcType(const Lepton& Lep, vector<Jet>& JetColl){
  //Type: -1: Unmatched, 1:L, 2:C, 3:B
  int SrcType=-1;
  bool NearB=false, NearC=false, NearL=false;
  for(unsigned int ij=0; ij<JetColl.size(); ij++){
    if(Lep.DeltaR(JetColl.at(ij))<0.4){
      if     (JetColl.at(ij).hadronFlavour()==5){ NearB=true; break; }//1)
      else if(JetColl.at(ij).hadronFlavour()==4){ NearC=true; }
      else if(JetColl.at(ij).hadronFlavour()==0){ NearL=true; }
    }
  }

  if     (NearB) SrcType=3;
  else if(NearC) SrcType=2;
  else if(NearL) SrcType=1;

  return SrcType;
  //1) Higher Priority to B. if there's multiple near jets, then b-jet has higher priority

}



int AnalyzerCore::GetPartonType_JH(int TruthIdx, std::vector<Gen>& TruthColl){
  //Type : 1:W decay product (LO sample)
  //       Currently have interest only in parton from W decay
  //       0:Error/Non classified 

  //Only consider Status 1 lepton
  if(TruthIdx<2) return 0;
  if( !(fabs(TruthColl.at(TruthIdx).PID())>0 && fabs(TruthColl.at(TruthIdx).PID())<10) ) return 0;

  int PartonType=0;
  int MotherIdx       = TruthColl.at(TruthIdx).MotherIndex();

  int MPID=0;
  //  int Status_now=0;
  //    if(    TruthIdx!=-1   ){ Status_now    = TruthColl.at(TruthIdx).Status();
  //                           }                           
  if(   MotherIdx!=-1   ){ MPID         = TruthColl.at(MotherIdx).PID();
  }
 
  if     ( TruthIdx==-1 )                                       PartonType= 0;
  else if( fabs(MPID)==23 || fabs(MPID)==24 || fabs(MPID)==25 ) PartonType= 1;
  else PartonType=0;

  return PartonType;
}


int AnalyzerCore::GetPrElType_InSameSCRange(int TruthIdx, std::vector<Gen>& TruthColl, TString Option){
  //Abbreviation: Get Prompt Electron Type In Same Supercluster Range
  //Pr. e>eee (int. conv.) case, collimated electrons can be merged in one SC & track is selected among them, and reconstructed as single electron. 
  //In this case, still there will be a nearby prompt electron of LepType 1-3 within supercluster merging range.
  //Supercluster merging range: |dphi|<0.3/0.3 (EB/EE), |deta|<0/0.07 (EB/EE) (+marginal 0.03 from 2 crystal size of 5x5 clusters)
  //Ref:JINST 10 (2015) P06005, arXiv:1502.02701
  //Return Value: 1/2/3:LeptonType-1/2/3 electron found in same SC range
  //             -1: No LeptonType-1/2/3 electron found in same SC range
  //
  //Note: Hadronic origins are not included in this algo. as I am not sure of effect of many nearby particles in jets on the ele-reco performance.
  //      + it is not of my interest yet.

  if(TruthIdx<2) return 0;
  if(abs(TruthColl.at(TruthIdx).PID())!=11) return 0;
  if(TruthColl.at(TruthIdx).Status()!=1) return 0;

  double dPhiMax=0.3, dEtaMax=0.1;
  int NearbyElType=-1, NearbyPrElIdx=-1;

  for(unsigned int it_gen=2; it_gen<TruthColl.size(); it_gen++){
    if(TruthColl.at(it_gen).Status()!=1) continue;
    if(abs(TruthColl.at(it_gen).PID())!=11) continue;
    if(fabs(TruthColl.at(TruthIdx).Eta()-TruthColl.at(it_gen).Eta())>dEtaMax) continue;
    if(fabs(TruthColl.at(TruthIdx).DeltaPhi(TruthColl.at(it_gen)))>dPhiMax) continue;

    int LepType = GetLeptonType_JH(it_gen, TruthColl);
    if(!(LepType>=1 && LepType<=3)) continue;
    NearbyElType  = LepType;
    NearbyPrElIdx = it_gen;
  }

  if     (Option.Contains("IdxType")){ return (NearbyElType>0? NearbyPrElIdx*10+NearbyElType:-1); }
  else if(Option.Contains("Idx"))    { return NearbyPrElIdx; }
  
  return NearbyElType;
  
}


int AnalyzerCore::GetLeptonType_JH(int TruthIdx, std::vector<Gen>& TruthColl){
  //Type : 1:EWPrompt  /  2:Signal Daughter /  3:EW/Sig-tau daughter / 4:Internal Conversion daughter from t/EWV/EWlep(Implicit,Explicit) / 5:Internal Conversion daughter from HardScatterPhoton
  //      -1:Unmatched & not EW Conversion candidate / -2:Hadron daughter / -3:Daughter of tau from hadron or parton / -4:Internal conversion daughter(implicit,explicit) having hadronic origin / -5:External conversion candidate(Hard scattered photon) / -6:External conversion from t/EWV/EWlep
  //      (-4:Daughter of Non-hard scattered photon & has parton or hadron ancestor OR implicit Conv from quark)
  //       0:Error / >0: Non-fake: Non-hadronic origin / <0 : Fakes: Hadronic origin or external conversion


  //Only consider Status 1 lepton
  if(TruthColl.size()==0) return 0;
  if(TruthIdx<2) return 0;
  if(TruthColl.at(TruthIdx).Status()!=1) return 0;
  if( !(fabs(TruthColl.at(TruthIdx).PID())==11 || fabs(TruthColl.at(TruthIdx).PID())==13) ) return 0;

  int LeptonType=0;
  int LastSelfIdx     = LastSelfMotherIdx(TruthIdx,TruthColl);
  int MotherIdx       = FirstNonSelfMotherIdx(TruthIdx,TruthColl);
  int LastSelfMIdx    = LastSelfMotherIdx(MotherIdx,TruthColl);
  int GrMotherIdx     = FirstNonSelfMotherIdx(MotherIdx,TruthColl);
  int LastSelfGrMIdx  = LastSelfMotherIdx(GrMotherIdx,TruthColl);

  int MPID=0, GrMPID=0;
  int Status_orig=0, MStatus_orig=0, MStatus_last=0, GrMStatus_orig=0, GrMStatus_last=0;
  bool HadronicOrigin = false;
  if(    TruthIdx!=-1   ){ Status_orig    = TruthColl.at(LastSelfIdx).Status();
    HadronicOrigin = HasHadronicAncestor(TruthIdx, TruthColl);
  }                           
  if(   MotherIdx!=-1   ){ MPID         = TruthColl.at(MotherIdx).PID();
    MStatus_orig = TruthColl.at(LastSelfMIdx).Status();
    MStatus_last = TruthColl.at(MotherIdx).Status();
  }
  if(  GrMotherIdx!=-1  ){ GrMPID         = TruthColl.at(GrMotherIdx).PID();
    GrMStatus_orig = TruthColl.at(LastSelfGrMIdx).Status();
    GrMStatus_last = TruthColl.at(GrMotherIdx).Status();
  }
 
  if     ( TruthIdx==-1 )                                       LeptonType= 0;
  else if( fabs(MPID)==23 || fabs(MPID)==24 || fabs(MPID)==25 ) LeptonType= 1;
  else if( IsSignalPID(MPID) )                                  LeptonType= 2;
  else if( Status_orig>20 && Status_orig<30 )                   LeptonType= 1;//1)
  else if( fabs(MPID)>50 )                                      LeptonType=-2;
  else if( fabs(MPID)==15 && MStatus_last==2 ){
    if     ( fabs(GrMPID)==23 || fabs(GrMPID)==24 || fabs(GrMPID)==25 ) LeptonType= 3;
    else if( IsSignalPID(GrMPID) )                                      LeptonType= 3;
    else if( MStatus_orig>20  && MStatus_orig<30  )                     LeptonType= 3;//1)
    else if( HadronicOrigin )                                           LeptonType=-3;//2-a)
    else if( fabs(GrMPID)==22  && GrMStatus_orig>20 && GrMStatus_orig<30 )                     LeptonType= 5;//2-b)
    else if( fabs(GrMPID)==22 )                                                                LeptonType= 4;//2-c)
    else if( (fabs(GrMPID)==11 || fabs(GrMPID)==13 || fabs(GrMPID)==15) && GrMStatus_last!=2 ) LeptonType= 4;//2-d)
    else                                                                                       LeptonType= 0;
  }
  else if( fabs(MPID)==22 ){
    if( MStatus_orig>20 && MStatus_orig<30 )                            LeptonType= 5;//3-a)
    else if( HadronicOrigin )                                           LeptonType=-4;//3-b)
    else if( fabs(GrMPID)==24 || fabs(GrMPID)==23 || fabs(GrMPID)==6  ) LeptonType= 4;//3-c)
    else if( fabs(GrMPID)==11 || fabs(GrMPID)==13 || fabs(GrMPID)==15 ) LeptonType= 4;//3-d)
    else                                                                LeptonType= 0;
  }
  else if( (fabs(MPID)==11 || fabs(MPID)==13 || fabs(MPID)==15) && MStatus_last!=2 && !HadronicOrigin ) LeptonType= 4;//4-a)
  else if( ((fabs(MPID)>=1 && fabs(MPID)<=5) || fabs(MPID)==21) && MStatus_last!=2 )                    LeptonType=-4;//4-b)
  else if( fabs(MPID)==6 ) LeptonType=4;//4-c)
  else LeptonType=0;


  return LeptonType;

  //**footnote
  //These are based on observation in DY,ZG,TT sample(DY,ZG:amcnlo+pythia, TT:powheg+pythia) for other PS generator, convention may differ.
  //1) In amcnlo generator, output of ME level generation does not have specific guage field mother. e.g. u u~ > l+ l- -> fabs(MID)=1
  //   This perhaps due to multiple field can interplay in production, and apparently it is not possible to distinguish them in any logic.
  //   e.g. think about previous example. you cannot say whether this is from gamma or Z or H...
  //   But in PS procedure, corrections on ME proc is done sometimes. In that case it seems mother is set Z for OS ll prod. W for lnu prod.
  //   e.g. If pythia applies ISR process on input u u~, than it should affect momentum of all the consequent processes, or in case of lnu, W radiating gamma can be added.
  //   You may think lnu case is obvious, but it may not like in case of pp > lllnu(You never know which one is from W>lnu and Z>ll)
  //2-a) e.g. a)Had > ta+X, ta>l+2nu b) q>ta+X in jet fragmentation (ta is not hardscattered, since it is already considered prev. step)
  //2-b) e.g. gamma>ta(+)+ta(-)+X, ta>lnu (St=2)
  //2-c) e.g. " " " " " " " " " " " " " " " " " ", but soft gamma case. this is not observed in test sample but put here just in case.
  //          (Non hadronic origin since such case already counted before, gamma should be from non-hadronic source)
  //2-d) e.g. l>tata..+l.. , ta>l+2nu (Implicit tau conv. from non-hadronic lepton and decay) In implicit conv. GenStatus!=2
  //3-a) e.g. hard gamma>ll
  //3-b) e.g. a) Had>gamma+X, gamma>ll+X (in PS+Had stage intermediate process is omitted you see just Had>Nphoton+Mhadrons+..)
  //          b) q>gamma+q, gamms>ll+X in jet fragmentation or radiations of tops.
  //          c) gluon>Ngamma+Mhadrons in jet fragmentation (Actually observed in samples)
  //3-c) e.g. W+>W+ gamma, or t>t+gamma, gamma>ll+X, not yet observed in test sample but possible (upto radiation is observed so far)
  //3-d) e.g. ta>ta+gamma, gamma>ll+X, tau not from hadron(e.g. pp>tata)
  //4-a) e.g. EW lep l, l>lll... just implicit conversion. 
  //4-b) e.g. q or g> Nlepton +MHadrons... in parton shower history
  //4-c) e.g. t>t+ll.. implicit conversion
}


int AnalyzerCore::GetLeptonType_JH(const Lepton& Lep, std::vector<Gen>& TruthColl){
  //Type : 1:EW-Prompt / 2: BSM-Prompt / 3:EW/BSM-Prompt-Tau Daughter 
  //       4:Internal Conversion from Soft QED Radiation (PS-level) / 5:Internal Conversion from Hard Process Photon (ME-level)
  //      -1:Unmatched & not EW Conversion candidate (mis-reco. or external conversion within jets)
  //      -2:Hadron Daughter / -3:Daughter of Tau from Hadron or Parton / -4:Internal Conversion Daughter having hadronic origin
  //      -5:External Conversion Candidate (Hard scattered photon) / -6:External conversion from t/EWV/EWlep
  //      (-4:Daughter of Non-hard scattered photon & has parton or hadron ancestor OR implicit Conv from quark)
  //       0:Error / >0: Non-fake: Non-hadronic origin / <0 : Fakes: Hadronic origin or external conversion

  if(TruthColl.size()==0) return 0;

  int LeptonType=0;
  int MatchedTruthIdx = GenMatchedIdx(Lep,TruthColl);

  int NearPhotonType=0, NearPhotonIdx=-1;
  if( MatchedTruthIdx==-1 ){
    NearPhotonIdx  = GetNearPhotonIdx(Lep, TruthColl);
    NearPhotonType = GetPhotonType_JH(NearPhotonIdx, TruthColl);
    if     ( NearPhotonType<=0 ) LeptonType=-1;//1)
    else if( NearPhotonType==1 ) LeptonType=-5;//2)
    else if( NearPhotonType==2 ) LeptonType=-6;//3)
  }
  else{
    LeptonType = GetLeptonType_JH(MatchedTruthIdx, TruthColl);//4)
    if(LeptonType>=4 && LeptonType<=5){//5)
      int NearbyPrElType = GetPrElType_InSameSCRange(MatchedTruthIdx, TruthColl);
      if(NearbyPrElType>0) LeptonType = NearbyPrElType;
    }
  }


  return LeptonType;

  //**footnote
  //1) matched to no gen-lepton nor gen-photon -> mis-reco.
  //   matched to no gen-lepton, but to photon with hadronic origin -> mis-reco. (e.g. pions->e) or external conversion from photon in jets
  //2) matched to no gen-lepton, but to photon with non-hadronic origin (hard process) -> external conversion from photon with non-hadronic source (ME-level)
  //3) matched to no gen-lepton, but to photon with non-hadronic origin (soft QED radiation) -> external conversion from photon with non-hadronic source (PS-level)
  //4) matched to gen-lepton, categorize based on the truth categorization algo.: AnalyzerCore::GetLeptonType_JH(int TruthIdx, std::vector<Gen>& TruthColl)
  //5) collimated e/gm objects are merged in SC, hence if there is prompt electron within SC-merging range, reco-electron's properties actually represent pre-QED-FSR prompt-electron,
  //   rather than the closest internal conversion electron. Therefore shift the type to the prompt lepton's type.
}



int AnalyzerCore::GetPhotonType_JH(int PhotonIdx, std::vector<Gen>& TruthColl){
  //Type : 
  // 0: Invalid input or Error or Hard process photon is input when it is not final line of its history
  // 1: prompt photon (hard process (ME-level)) / 2: prompt Else prompt daughter(l,V,t)
  //-1: Reserved for unmatched(Not used now) / -2: Hadronic origin

  if( PhotonIdx<2 ) return 0;
  if( !(TruthColl.at(PhotonIdx).PID()==22 && (TruthColl.at(PhotonIdx).Status()==1 || TruthColl.at(PhotonIdx).Status()==23)) ) return 0;

  if(TruthColl.at(PhotonIdx).Status()==23){
    if(IsFinalPhotonSt23(TruthColl)) return 1;
    else                             return 0;
  }//From this pt, only St1 Photon is treated.

  int PhotonType=0;
  int LastSelfIdx    = LastSelfMotherIdx(PhotonIdx,TruthColl);
  int MotherIdx      = FirstNonSelfMotherIdx(PhotonIdx,TruthColl);
  int fMPID=0, Status_orig=0;
  bool HadronicOrigin = false;
  if( PhotonIdx!=-1 ){ Status_orig    = TruthColl.at(LastSelfIdx).Status();
    HadronicOrigin = HasHadronicAncestor(PhotonIdx, TruthColl);
  }                           
  if( MotherIdx!=-1 ){ fMPID          = fabs(TruthColl.at(MotherIdx).PID()); }


  if     (    Status_orig>20 && Status_orig<30          ) PhotonType= 1;//1)
  else if(         fMPID==23 || fMPID==25               ) PhotonType= 1;//2)
  else if( fMPID==24 || fMPID==6  || IsSignalPID(fMPID) ) PhotonType= 2;//3)
  else if(            HadronicOrigin                    ) PhotonType=-2;//4)
  else if( fMPID==11 || fMPID==13 || fMPID==15          ) PhotonType= 2;//5)
  else                                                    PhotonType= 0;
  
  return PhotonType;
  //**footnote
  //1) In case of ME-level photon, they have history; GenSt=23>...>1.
  //   this is better than checking mother because intermediate-mother is not written in some cases, where generation is not explicitly on-shell.
  //   e.g. qq>llG instead of qq>Z>llG
  //2) Sometimes, photon's final state is 1 before any history. (Presumably skimmed between pythia & MiniAOD).
  //   e.g. G;St=1, Mother=Z ; algorithm 1) cannot catch this.
  //   but PS step also treats photon radiation (same kind of history), therefore distinction between type 1 and 2 is not physical.
  //3) top and charged bosons radiate photons, and some case the photon is very energetic.
  //4) This category does not include tops. Photons from hadrons and quarks. But predominantly, in most of the cases they are daughter of pi0.
  //   But rarely other mesons as eta, B, or even some quarks can also radiate energetic photons.
  //5) Photons radiated from lepton FSR. Sometimes they are quite energetic.
}

//------------------------------------------------------------------------------------------//



//--Gen-Matching Tools----------------------------------------------------------------------//
int AnalyzerCore::GenMatchedIdx(const Lepton& Lep, std::vector<Gen>& truthColl){
  //Find Matched Index within dR01; if ambiguous closest dR one chosen (Resolution way better than dPtRel)
  //Seed from RecoLepton

  int MatchedIdx=-1, PIDLep=0;
  double dR=999., dRmax=0.1;

  if(Lep.LeptonFlavour()==Lepton::ELECTRON) PIDLep = 11;
  else if(Lep.LeptonFlavour()==Lepton::MUON) PIDLep = 13;
  else{
    cout << "[AnalyzerCore::GetGenMatchedLepton] input lepton flavour not set" << endl;
    exit(EXIT_FAILURE);
  }
  
  for(unsigned int i=2; i<truthColl.size(); i++){
    if( truthColl.at(i).MotherIndex()<0     ) continue;
    if( truthColl.at(i).Status()!=1         ) continue;
    if( fabs(truthColl.at(i).PID())!=PIDLep ) continue;
    if( truthColl.at(i).DeltaR(Lep)>dRmax   ) continue;

    if( truthColl.at(i).DeltaR(Lep)<dR ){ dR=truthColl.at(i).DeltaR(Lep); MatchedIdx=i; }
  }


  //  cout << "truthColl.size() = " << truthColl.size() << " MatchedIdx = " << MatchedIdx << endl;
  return MatchedIdx;
}


int AnalyzerCore::GetNearPhotonIdx(const Lepton& Lep, std::vector<Gen>& TruthColl){
  // Return gen-photon index, which are candidate for source of external conversion. 1)

  int NearPhotonIdx=-1;
  double PTthreshold=10., dPtRelmax=0.5;//2)
  double dRmax=0.2;//3)
  double dRmin=999.;
  for(unsigned int i=2; i<TruthColl.size(); i++){
    if( TruthColl.at(i).MotherIndex()<0   ) continue;
    if( !(TruthColl.at(i).PID()==22 && (TruthColl.at(i).Status()==1 || TruthColl.at(i).Status()==23)) ) continue;
    if( TruthColl.at(i).Pt()<PTthreshold  ) continue;
    if( !(Lep.Pt()/TruthColl.at(i).Pt()>(1.-dPtRelmax) && Lep.Pt()/TruthColl.at(i).Pt()<(1.+dPtRelmax)) ) continue;
    if( Lep.DeltaR(TruthColl.at(i))>dRmax ) continue;

    if( TruthColl.at(i).Status()==23 && !IsFinalPhotonSt23(TruthColl) ) continue;//4)
    if( Lep.DeltaR(TruthColl.at(i))<dRmin ){ dRmin=Lep.DeltaR(TruthColl.at(i)); NearPhotonIdx=i; }
  }


  return NearPhotonIdx;
  //footnote
  //1) External conversion is only meaningful for electron, as external conversion rate is theoretically ~M^{-2} in asymmetric limit (ref: arXiv:1110.1368)
  //   In the conversion study, I also observed that muon conversion rate is less than ~1/3000 of electron conversion rate (unobserved).
  //2) In 16' analyses using Cat-Ntuples, I used dPtRel<0.2 cut, which was optimized for PT>25 electrons. It is updated to 0.5 as I observe that PT(e)/PT(g) varies between 0.5 and 1.3 for 10<PT(e)<25. Thus this cut is valid for PT(e)>10 GeV.
  //3) dReg is larger for low Pt electrons, but 0.2 cut is still enough for matching for all PT(e)>10.
  //4) In some MC events, hard scattered photon(GenSt23) is the last history of the photon and do not have daughter in the history. Presumably because generator history is partly skimmed in MiniAOD.
}


int AnalyzerCore::FirstNonSelfMotherIdx(int TruthIdx, std::vector<Gen>& TruthColl){

  if(TruthIdx<2) return -1;

  int pid=TruthColl.at(TruthIdx).PID(), midx=TruthIdx;
  while(TruthColl.at(midx).PID()==pid){
    midx=TruthColl.at(midx).MotherIndex();  
    if(midx<0) break;
  }

  return midx;
}




int AnalyzerCore::LastSelfMotherIdx(int TruthIdx,std::vector<Gen>& TruthColl){

  if(TruthIdx<2) return TruthIdx;

  int pid=TruthColl.at(TruthIdx).PID(), midx=TruthIdx, currentidx=TruthIdx;
  while(TruthColl.at(midx).PID()==pid){
    currentidx=midx;
    midx=TruthColl.at(midx).MotherIndex();  
    if(midx<0) break;
  }

  return currentidx;
}


bool AnalyzerCore::HasHadronicAncestor(int TruthIdx, std::vector<Gen>& TruthColl){
  //Returns true  if 1)has hadron mother, 2)has quark mother(!top) 3)Incident protons
  //        false if 1)is hardscattered truth, 2)EW/H/BSM/t daughter, 3)not above, 4)invalid input(e.g. unmatched case)
  
  if(TruthIdx<0) return false;
  if(TruthIdx<2) return true;

  bool HasPartonHadronAncestor=false;
  int  midx=TruthIdx, fmid=fabs(TruthColl.at(midx).PID()), MSt_orig=-1;
  int  St_orig=TruthColl.at(LastSelfMotherIdx(TruthIdx, TruthColl)).Status();
  if( St_orig>20 && St_orig<30) return false;

  while( midx>=2 ){
    midx=FirstNonSelfMotherIdx(midx,TruthColl);
    MSt_orig=TruthColl.at(LastSelfMotherIdx(midx,TruthColl)).Status();
    fmid=fabs(TruthColl.at(midx).PID());
    if(  fmid==23 || fmid==24 || fmid==25 || fmid==6 || IsSignalPID(fmid) ){ HasPartonHadronAncestor=false; break; }
    if( (fmid==11 || fmid==13 || fmid==15 || fmid==22) && (MSt_orig>20 && MSt_orig<30)){ HasPartonHadronAncestor=false; break; }
    if( fmid>50 ) { HasPartonHadronAncestor=true; break; }
    if( (fmid>=1 && fmid<=5) || fmid==21 ){ HasPartonHadronAncestor=true; break; }
  }

  return HasPartonHadronAncestor;
}




bool AnalyzerCore::IsFinalPhotonSt23(std::vector<Gen>& TruthColl){
  //In Some XG proc events, it is observed that some of photons' last status is 23. Presumably due to skimming of generator history between pythia and MiniAOD.
  //The function returns if this is the case.
  //And this is designed only for 1 hard photon case as W+G or Z+G or TT+G

  bool IsFinalGammaStatus23 = false;
  bool HasStatus23Photon    = false;
  for(unsigned int i=2; i<TruthColl.size(); i++){
    int fpid  = fabs(TruthColl.at(i).PID());
    int GenSt = TruthColl.at(i).Status();
    int MPID_direct= TruthColl.at(TruthColl.at(i).MotherIndex()).PID();
    if( !((fpid!=22 && MPID_direct==22) || (fpid==22 && (GenSt==23||GenSt==1))) ) continue;

    int LastSelfIdx  = LastSelfMotherIdx(i,TruthColl);
    int LastSelfSt   = TruthColl.at(LastSelfIdx).Status();
    int MotherIdx    = FirstNonSelfMotherIdx(i,TruthColl);
    int LastSelfMIdx=-1, MStatus_orig=-1;
    if(MotherIdx!=-1){
      LastSelfMIdx = LastSelfMotherIdx(MotherIdx,TruthColl);
      MStatus_orig = TruthColl.at(LastSelfMIdx).Status();
    }

    if(fpid==22){
      if(GenSt==23) {HasStatus23Photon=true; IsFinalGammaStatus23=true;}
      else if(GenSt==1 && LastSelfSt==23) {IsFinalGammaStatus23=false; break;}//a)
    }
    else if( MPID_direct==22 && MStatus_orig==23 ){ IsFinalGammaStatus23=false; break;}//b)
  }

  if(!HasStatus23Photon) return false;
  
  return IsFinalGammaStatus23;

  //**footnotes
  //a) Status-23 photon's last is 1. Thus status-23 photon is not the last history.
  //b) Daughter particle of status-23 photon is found. Thus status-23 photon is not the last history.
}






