#include "AnalyzerCore.h"

AnalyzerCore::AnalyzerCore(){

  outfile = NULL;

  
  mcCorr = new MCCorrection();
  puppiCorr = new PuppiSoftdropMassCorr();
  if(Analyzer == "HNL_SignalRegionPlotter"){
    fakeEst = new FakeBackgroundEstimator();
    cfEst = new CFBackgroundEstimator();
    pdfReweight = new PDFReweight();
  }
  muonGE = new GeneralizedEndpoint();
  muonGEScaleSyst = new GEScaleSyst();

  JECSources = {"AbsoluteStat","AbsoluteScale","AbsoluteFlavMap","AbsoluteMPFBias","Fragmentation","SinglePionECAL","SinglePionHCAL","FlavorQCD","TimePtEta","RelativeJEREC1","RelativeJEREC2","RelativeJERHF","RelativePtBB","RelativePtEC1","RelativePtEC2","RelativePtHF","RelativeBal","RelativeSample","RelativeFSR","RelativeStatFSR","RelativeStatEC","RelativeStatHF","PileUpDataMC","PileUpPtRef","PileUpPtBB","PileUpPtEC1","PileUpPtEC2","PileUpPtHF","FlavorZJet","FlavorPhotonJet","FlavorPureGluon","FlavorPureQuark","FlavorPureCharm","FlavorPureBottom","Total"};

  /*
    
    // In your analyser code add this line to constructor to fill map with JEC source values.
    for(auto jec_source : JECSources)   SetupJECUncertainty(jec_source, "AK4PFchs");

  }
    // Then you can get vector of jets with shift calling 
    std::vector<Jet> AnalyzerCore::ScaleJetsIndividualSource(const std::vector<Jet>& jets, int sys, TString source);
    vector<Jet> jets_AbsoluteStatUp = ScaleJetsIndividualSource(jets, 1, "AbsoluteStat");
  */
  

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
  if(Analyzer == "HNL_SignalRegionPlotter"){
    if(fakeEst) delete fakeEst;
    if(cfEst) delete cfEst;
    if(pdfReweight) delete pdfReweight;
  }
  if(muonGE) delete muonGE;
  if(muonGEScaleSyst) delete muonGEScaleSyst;

  AK4CHSJECUncMap.clear();
  AK4PUPPIJECUncMap.clear();
  AK8CHSJECUncMap.clear();
  AK8PUPPIJECUncMap.clear();

  

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

float AnalyzerCore::GetJECUncertainty(TString source, TString JetType, float eta, float pt, int sys){


  std::map<TString, std::vector<std::map<float, std::vector<float> > > >::iterator mapit;
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

  
  float bin_boundary(-999.);

  std::map<float, std::vector<float> > ptmap = mapit->second.at(0);

  std::vector<float> etabins;  
  for(std::map<float, std::vector<float> >::iterator it = ptmap.begin(); it!= ptmap.end(); it++){
    etabins.push_back(it->first);
  }

  
  for(unsigned int i=0; i < etabins.size()-1 ; i++){
    if(eta >= etabins.at(i) && eta < etabins.at(i+1)){  bin_boundary = float(etabins.at(i)) ; break;}
  }

  if(bin_boundary == -999) return 1.;
  
  std::vector<float> ptbins;
  

  for(std::map<float, std::vector<float> >::iterator pit = ptmap.begin();  pit != ptmap.end(); pit++){
    if(float(pit->first) == float(bin_boundary)) {ptbins = pit->second; }
  }
  
  int ptbin(-999);
  if(pt >= ptbins.at(ptbins.size() - 1)) ptbin = ptbins.size() - 1;
  for(unsigned int j = 0 ; j < ptbins.size()-1; j++){
    if( pt >= ptbins.at(j)  && pt < ptbins.at(j+1)) {ptbin=j; break;}
  }
  
  if(ptbin == -999) return 1.;


  std::map<float, std::vector<float> > upmap = mapit->second.at(1); 
  std::map<float, std::vector<float> > downmap = mapit->second.at(2); 
  
  std::map<float, std::vector<float> >::iterator mapit_unc;
  if(sys> 0) mapit_unc =  mapit->second.at(1).find(bin_boundary);
  else mapit_unc =  mapit->second.at(2).find(bin_boundary);
  
  float unc = (sys> 0) ?   1+ mapit_unc->second.at(ptbin) : 1 - mapit_unc->second.at(ptbin);

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

  std::map<float, std::vector<float> > etaptmap, eta_uncupmap, eta_uncdownmap;
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
    std::vector<float> ptbin, unc_up, unc_down;
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

    std::vector<float> NULLBin;
    if(ilines ==  SourceLines.size() -1) etaptmap[eta_max] = NULLBin;
        
  }
  
  jec_file.close();

  std::vector<std::map<float, std::vector<float> > > vec_unc;
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
    mu.SetMiniAODPt(muon_pt->at(i));
    mu.SetMiniAODTunePPt(muon_TuneP_pt->at(i));

    mu.SetJetPtRel(muon_jetPtRel->at(i));
    mu.SetJetPtRatio(muon_jetPtRatio->at(i));


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

    out.push_back(mu);

  }
  return out;

}



std::vector<Muon> AnalyzerCore::GetMuons(TString id, double ptmin, double fetamax){

  std::vector<Muon> muons = GetAllMuons();
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
  return out;

}

std::vector<Muon> AnalyzerCore::GetMuons(AnalyzerParameter param, bool Run_Fake){
  return GetMuons(param, param.Muon_Tight_ID, param.Muon_MinPt, param.Muon_MaxEta, Run_Fake);
}

std::vector<Muon> AnalyzerCore::GetMuons(AnalyzerParameter param, TString id, double ptmin, double fetamax, bool Run_Fake){

  std::vector<Muon> this_AllMuons = GetAllMuons();
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
      // cout << "Fail ID" << endl;                                                                                                        
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

    el.SetUncorrE(electron_EnergyUnCorr->at(i));
    el.SetSC(electron_scEta->at(i), electron_scPhi->at(i), electron_scEnergy->at(i));
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
    el.SetDetaIn(electron_dEtaIn->at(i));
    el.SetSigmaIEtaIE(electron_sigmaIEtaIEta->at(i));
    el.SetLepIso(electron_chIso03->at(i),electron_nhIso03->at(i),electron_phIso03->at(i));
    
    el.SetE15(electron_E15->at(i));
    el.SetE25(electron_E25->at(i));
    el.SetE55(electron_E55->at(i));


    el.SetFilterBits(electron_filterbits->at(i));
    el.SetPathBits(electron_pathbits->at(i));

    out.push_back(el);

  }
  return out;

}

std::vector<Electron> AnalyzerCore::GetElectrons(AnalyzerParameter param,  bool run_fake, bool vetoHEM){
  return GetElectrons(param, param.Electron_Tight_ID, param.Electron_MinPt, param.Electron_MaxEta, run_fake, vetoHEM);
}

std::vector<Electron> AnalyzerCore::GetElectrons(AnalyzerParameter param, TString id, double ptmin, double fetamax, bool run_fake, bool vetoHEM){
  
  std::vector<Electron> this_AllElectrons = GetAllElectrons();
  std::vector<Electron> electrons ;

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
    if(!( electrons.at(i).PassID(id) )){
      continue;
    }
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
  
  return out;

}

std::vector<Electron> AnalyzerCore::GetElectrons(TString id, double ptmin, double fetamax, bool vetoHEM){

  std::vector<Electron> electrons = GetAllElectrons();

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
  cout << "[HNL_LeptonCore::GetIsoFromID ] ID not found.." << id<< endl;
  exit(EXIT_FAILURE);

  return -999999999.;

}

bool AnalyzerCore::PassID(std::vector<Electron> electrons, TString ID){

  int nel(0);
  for(auto el : electrons){

    if( el.PassID(ID) ) nel++;

  }
  if (nel ==2) return true;
  return false;
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



std::vector<Jet> AnalyzerCore::GetAllJets(){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jet_pt->size(); i++){
    Jet jet;
    jet.SetPtEtaPhiM(jet_pt->at(i), jet_eta->at(i), jet_phi->at(i), jet_m->at(i));

    jet.SetPxUnSmeared(jet.Px());
    jet.SetPyUnSmeared(jet.Py());

    //jet.SetNTracks(jet_vtxNtracks->at(i));
    //==== Jet energy up and down are 1.xx or 0.99, not energy
    jet.SetEnShift( jet_shiftedEnUp->at(i), jet_shiftedEnDown->at(i) );
    if(!IsDATA){
      jet *= jet_smearedRes->at(i);
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

  return out;

}

std::vector<Jet> AnalyzerCore::GetJets(TString ID, double ptmin, double fetamax){

  std::vector<Jet> jets = GetAllJets();

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

  std::vector<Jet> jets_uncorr = GetAllJets();
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

  return out;

}

std::vector<FatJet> AnalyzerCore::GetFatJets(TString id, double ptmin, double fetamax){

  std::vector<FatJet> jets = GetAllFatJets();
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
  return out;

  std::sort(out.begin(),       out.end(),        PtComparing);

}

std::vector<FatJet> AnalyzerCore::GetFatJets(AnalyzerParameter param){
  
  return GetFatJets(param, param.FatJet_ID, param.FatJet_MinPt, param.FatJet_MaxEta);

}
std::vector<FatJet> AnalyzerCore::GetFatJets(AnalyzerParameter param,TString id, double ptmin, double fetamax){

  std::vector<FatJet> jets_pc = puppiCorr->Correct(GetAllFatJets());
  std::vector<FatJet> jets;
  if(param.syst_ == AnalyzerParameter::JetEnUp)            jets    = ScaleFatJets( jets_pc, -1 );
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
  return out;

  std::sort(out.begin(),       out.end(),        PtComparing);

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



pair<int,double> AnalyzerCore::GetNBJets(vector<Jet> jets, AnalyzerParameter param,TString WP){

  int NBJets_NoSF(0);
  JetTagging::WP             jwp = JetTagging::Medium;
  if  (WP == "Loose")        jwp = JetTagging::Loose;
  else if  (WP == "Medium")  jwp = JetTagging::Medium;
  else  if  (WP == "Tight")  jwp = JetTagging::Tight;
  else return make_pair(-1,1.);

  JetTagging::Tagger    tagger = JetTagging::DeepCSV;
  if  (WP.Contains("DeepJet")) tagger = JetTagging::DeepJet;

  JetTagging::MeasurmentType type = JetTagging::comb;
  if  (WP.Contains("DeepJet")) type =  JetTagging::mujets;



  JetTagging::Parameters jtp = JetTagging::Parameters(tagger,
							      jwp,
							      JetTagging::incl, type);


  double btagWeight_1a = mcCorr->GetBTaggingReweight_1a(jets, jtp);
  double btagWeight_1d = 1.;//mcCorr->GetBTaggingReweight_1d(jets, jtp_DeepCSV_Reshape);                                                         


  for(auto ij : jets)  {
    double this_discr = ij.GetTaggerResult(tagger);
    if( this_discr > mcCorr->GetJetTaggingCutValue(tagger, jwp) ) NBJets_NoSF++;
  }

  if(param.BJet_Method=="1a") return make_pair(NBJets_NoSF,btagWeight_1a);
  if(param.BJet_Method=="1d") return make_pair(NBJets_NoSF,btagWeight_1d);
  if(param.BJet_Method=="2a") return make_pair(GetNBJets2a(jets,param, WP),1.);

  cout << "[AnalyzerCore::GettNBjet] wtf" << endl;
  exit(EXIT_FAILURE);

  return make_pair(NBJets_NoSF,-9999.);

}

pair<int,double> AnalyzerCore::GetNBJets(vector<Jet> jets, TString WP, TString method){

  int NBJets_NoSF(0);
  JetTagging::WP jwp = JetTagging::Medium;
  if  (WP == "Loose")  jwp = JetTagging::Loose;
  else if  (WP == "Medium")  jwp = JetTagging::Medium;
  else  if  (WP == "Tight")  jwp = JetTagging::Tight;
  else return make_pair(-1,1.);

  JetTagging::Tagger    tagger = JetTagging::DeepCSV;
  if  (WP.Contains("DeepJet")) tagger = JetTagging::DeepJet;

  JetTagging::MeasurmentType type = JetTagging::comb;
  if  (WP.Contains("DeepJet")) type =  JetTagging::mujets;


  JetTagging::Parameters jtp= JetTagging::Parameters(tagger,
							      jwp,
							      JetTagging::incl, type);

  double btagWeight_1a = mcCorr->GetBTaggingReweight_1a(jets, jtp);
  double btagWeight_1d = 1.;//mcCorr->GetBTaggingReweight_1d(jets, jtp_DeepCSV_Reshape);                                                         


  for(auto ij : jets)  {
    double this_discr = ij.GetTaggerResult(tagger);
    if( this_discr > mcCorr->GetJetTaggingCutValue(tagger, jwp) ) NBJets_NoSF++;
  }

  if(method=="1a") return make_pair(NBJets_NoSF,btagWeight_1a);
  if(method=="1d") return make_pair(NBJets_NoSF,btagWeight_1d);
  if(method=="2a") return make_pair(GetNBJets2a(jets, WP),1.);


  cout << "[AnalyzerCore::GetNBJet] wtf" << endl;
  exit(EXIT_FAILURE);

  return make_pair(NBJets_NoSF,-9999.);
}


int AnalyzerCore::GetNBJets2a(TString ID, TString WP){

  vector<Jet> this_AllJets = GetAllJets();
  vector<Jet>       jets   = SelectJets(this_AllJets,ID, 20., 2.4);

  int NBJets_WithSF_2a(0);
  //  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);

  JetTagging::WP             jwp = JetTagging::Medium;
  if  (WP.Contains("Loose"))        jwp = JetTagging::Loose;
  else if  (WP.Contains("Medium"))  jwp = JetTagging::Medium;
  else  if  (WP.Contains("Tight"))  jwp = JetTagging::Tight;
  else return -1;

  JetTagging::Tagger    tagger = JetTagging::DeepCSV;
  if  (WP.Contains("DeepJet")) tagger = JetTagging::DeepJet;

  JetTagging::MeasurmentType type = JetTagging::comb;
  if  (WP.Contains("DeepJet")) type =  JetTagging::mujets;
  for( auto i  :  jets)
    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(tagger,       jwp, JetTagging::incl,type), i) ) NBJets_WithSF_2a++;


  return NBJets_WithSF_2a;
}



int AnalyzerCore::GetNBJets2a( vector<Jet> jets, AnalyzerParameter param, TString WP){

  int NBJets_WithSF_2a(0);

  JetTagging::WP             jwp = JetTagging::Medium;
  if  (WP == "Loose")        jwp = JetTagging::Loose;
  else if  (WP == "Medium")  jwp = JetTagging::Medium;
  else  if  (WP == "Tight")  jwp = JetTagging::Tight;
  else return -1;


  JetTagging::Tagger    tagger = JetTagging::DeepCSV;
  if  (WP.Contains("DeepJet")) tagger = JetTagging::DeepJet;

  JetTagging::MeasurmentType type = JetTagging::comb;
  if  (WP.Contains("DeepJet")) type =  JetTagging::mujets;

  for( auto i  :  jets){
    if(fabs(i.Eta()) >2.4) continue;
    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(tagger,       jwp, JetTagging::incl, type), i,param.SystDir_BTag)	) NBJets_WithSF_2a++;
  }
  
  return NBJets_WithSF_2a;
}


int AnalyzerCore::GetNBJets2a( vector<Jet> jets, TString WP){

  int NBJets_WithSF_2a(0);

  JetTagging::WP             jwp = JetTagging::Medium;
  if  (WP == "Loose")        jwp = JetTagging::Loose;
  else if  (WP == "Medium")  jwp = JetTagging::Medium;
  else  if  (WP == "Tight")  jwp = JetTagging::Tight;
  else return -1;


  JetTagging::Tagger    tagger = JetTagging::DeepCSV;
  if  (WP.Contains("DeepJet")) tagger = JetTagging::DeepJet;

  JetTagging::MeasurmentType type = JetTagging::comb;
  if  (WP.Contains("DeepJet")) type =  JetTagging::mujets;


  for(unsigned int i=0; i<jets.size(); i++)
    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(tagger,       jwp, JetTagging::incl, type), jets.at(i)) ) NBJets_WithSF_2a++;


  return NBJets_WithSF_2a;
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
  float fsys = -1;
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

vector<Jet>   AnalyzerCore::GetBJets(AnalyzerParameter param,vector<Jet> jetColl, JetTagging::Parameters jtp){

  vector<Jet> output_jets;

  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    if( jetColl[ijet].GetTaggerResult(jtp.j_Tagger) <= mcCorr->GetJetTaggingCutValue(jtp.j_Tagger, jtp.j_WP) ) continue;  
    output_jets.push_back( jetColl.at(ijet) );
  }
  std::sort(output_jets.begin(),       output_jets.end(),        PtComparing);
  return output_jets;

}

vector<Jet>   AnalyzerCore::GetLJets(AnalyzerParameter param,vector<Jet> jetColl, JetTagging::Parameters jtp){

  vector<Jet> output_jets;

  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    if( jetColl[ijet].GetTaggerResult(jtp.j_Tagger) > mcCorr->GetJetTaggingCutValue(jtp.j_Tagger, jtp.j_WP) ) continue;
    output_jets.push_back( jetColl.at(ijet) );
  }
  std::sort(output_jets.begin(),       output_jets.end(),        PtComparing);
  return output_jets;

}



vector<Jet>   AnalyzerCore::GetAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean, TString pu_tag,std::vector<Lepton *> leps_veto,  vector<FatJet> fatjets){

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



vector<Jet>   AnalyzerCore::GetAK4Jets(vector<Jet> jets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean, double dr_ak8_clean, TString pu_tag, vector<Electron>  veto_electrons, vector<Muon>  veto_muons, vector<FatJet> fatjets){

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



vector<FatJet>  AnalyzerCore::GetAK8Jets(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut, vector<Electron>  veto_electrons, vector<Muon>  veto_muons){


  return GetAK8Jetsv2(fatjets,pt_cut,eta_cut, lepton_cleaning, dr_lep_clean,apply_tau21, tau21_cut,apply_masscut,sdmass_lower_cut,sdmass_upper_cut, -999., veto_electrons,veto_muons);
}

vector<FatJet>  AnalyzerCore::GetAK8Jetsv2(vector<FatJet> fatjets, double pt_cut ,  double eta_cut, bool lepton_cleaning  , double dr_lep_clean , bool apply_tau21, double tau21_cut , bool apply_masscut, double sdmass_lower_cut,  double sdmass_upper_cut, double WQCDTagger, vector<Electron>  veto_electrons, vector<Muon>  veto_muons){


  vector<FatJet> output_fatjets;
  for(unsigned int ijet =0; ijet < fatjets.size(); ijet++){

    bool jetok=true;

    for(unsigned int iel=0 ; iel < veto_electrons.size(); iel++){
      if(fatjets[ijet].DeltaR(veto_electrons[iel]) < dr_lep_clean) jetok = false;
    }

    for(unsigned int iel=0 ; iel < veto_muons.size(); iel++){
      if(fatjets[ijet].DeltaR(veto_muons[iel]) < dr_lep_clean) jetok = false;
    }

    if(WQCDTagger > 0){
      if (fatjets[ijet].GetTaggerResult(JetTagging::particleNet_WvsQCD) < WQCDTagger) continue;
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
  float eta=Particle.Eta(), phi=Particle.Phi();

  if(eta>-3. && eta<-1.3 && phi>-1.57 && phi<-0.87) IstheRegion=true;

  return IstheRegion;
}



bool AnalyzerCore::IsHEMCRReg(Particle& Particle, TString Option){

  bool IstheRegion=false;
  float eta=Particle.Eta(), phi=Particle.Phi();
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
    if(DataYear == 2018) current_SDMass = this_jet.SDMass() * (0.982 + double(sys) * 0.004 ); // FIX

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
    if(DataYear == 2018) current_SDMass = this_jet.SDMass() * (1.09 + double(sys) * 0.05 ); //FIX
    
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


///======== Weight functions                                                                                                                     



double AnalyzerCore::GetMuonSFEventWeight(std::vector<Muon> muons,AnalyzerParameter param ){

  double this_weight(1.);
  if(!IsDATA){

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

    for (auto mu: muons){
      double MiniAODP = sqrt( mu.MiniAODPt() * mu.MiniAODPt() + mu.Pz() * mu.Pz() );
      double this_pt  = mu.MiniAODPt();
      double this_eta = mu.Eta();

      int SystDir_MuonIDSF(0), SystDir_MuonISOSF(0), SystDir_MuonRecoSF (0), SystDir_MuonTriggerSF(0);
      if(param.syst_ == AnalyzerParameter::MuonRecoSFUp)SystDir_MuonRecoSF = +1;
      else if(param.syst_ == AnalyzerParameter::MuonRecoSFDown)SystDir_MuonRecoSF = -1;
      else if(param.syst_ == AnalyzerParameter::MuonIDSFUp)  SystDir_MuonIDSF = +1;
      else if(param.syst_ == AnalyzerParameter::MuonIDSFDown)  SystDir_MuonIDSF = -1;
      else if(param.syst_ == AnalyzerParameter::MuonISOSFUp) SystDir_MuonISOSF  = +1;
      else if(param.syst_ == AnalyzerParameter::MuonISOSFDown) SystDir_MuonISOSF  = -1;
      else if(param.syst_ == AnalyzerParameter::MuonTriggerSFUp)SystDir_MuonTriggerSF = +1;
      else if(param.syst_ == AnalyzerParameter::MuonTriggerSFDown)SystDir_MuonTriggerSF = -1;
      


      double this_idsf   = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  this_eta, this_pt,SystDir_MuonIDSF);
      double this_isosf  = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, this_eta, this_pt,SystDir_MuonISOSF);
      double this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, param.Muon_Trigger_NameForSF, muons,SystDir_MuonTriggerSF);

      this_weight *= this_idsf*this_isosf*this_trigsf;
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

      double this_recosf  = mcCorr->ElectronReco_SF(el.scEta(),el.Pt(), SystDir_ElectronRecoSF);
      double this_idsf    = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, el.scEta(), el.Pt(), SystDir_ElectronIDSF);

      this_weight *= this_recosf*this_idsf;
      FillWeightHist(param.Name+"/el_reco_sf_"+param.Name, this_recosf);
      FillWeightHist(param.Name+"/el_id_sf_"+param.Name, this_idsf);

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



double AnalyzerCore::GetFakeWeight(std::vector<Lepton *> leps, AnalyzerParameter _param, bool apply_r){

  if(!IsData) return 1.;
  
  double this_weight = -1.;
  if(leps.size() == 1){
    TString fr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_FR_Key : _param.Muon_FR_Key;
    TString pr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? _param.Electron_PR_Key : _param.Muon_PR_Key;

    if(!leps[0]->LepIDSet()) {
      cout << "Lepton ID not set" << endl;
      exit(EXIT_FAILURE);
    }
    
    double this_fr1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()); 
    double this_pr1 = 1.;
    if(apply_r) this_pr1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID,pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) :fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) ;
    
    this_weight=  fakeEst->CalculateLepWeight(this_pr1, this_fr1, leps[0]->PassLepID() );

    //    cout << this_weight<< " L"<<endl;

    return this_weight;



    // fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID, pr_key1, fabs(electrons[0].Eta()), electrons[0].Pt());
    // fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key1, fabs(muons[0].Eta()), muons[0].Pt());
  }
  if (leps.size() == 2){

    if(! (leps[0]->LepIDSet() || leps[1]->LepIDSet())) {
      cout << "Lepton ID not set" << endl;
      exit(EXIT_FAILURE);
    }

    TString fr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_FR_Key : _param.Muon_FR_Key;
    TString fr_key2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_FR_Key : _param.Muon_FR_Key;
    TString pr_key1 =  (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_PR_Key : _param.Muon_PR_Key;
    TString pr_key2 =  (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_PR_Key : _param.Muon_PR_Key;

    double this_fr1 =  (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, fr_key1, fabs(leps[0]->Eta()), leps[0]->Pt()); 
    double this_fr2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, fr_key2, fabs(leps[1]->Eta()), leps[1]->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, fr_key2, fabs(leps[1]->Eta()), leps[1]->Pt()); 

    double this_pr1 = 1.;
    if(apply_r) this_pr1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID,pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) :fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key1 , fabs(leps[0]->Eta()), leps[0]->Pt()) ;
    double this_pr2 = 1.;
    if(apply_r) this_pr2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronPromptRate(_param.Electron_Tight_ID,pr_key2 , fabs(leps[1]->Eta()), leps[1]->Pt()) :fakeEst->GetMuonPromptRate(_param.Muon_Tight_ID, pr_key2 , fabs(leps[1]->Eta()), leps[1]->Pt()) ;

    TString ID1 =  (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
    TString ID2 =  (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;


    // TMP FIX TO PT 
    //if (leps[1]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->Pt() < 20) this_fr2*= 1.2;
    if (leps[1]->LeptonFlavour() == Lepton::MUON && leps[1]->Pt() < 20) this_fr2*= 1.1; //JH : optimize this value
    
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
      
      this_fr =  (lep->LeptonFlavour() == Lepton::ELECTRON) ? fakeEst->GetElectronFakeRate(_param.Electron_Tight_ID, _param.Electron_FR_Key, fabs(lep->Eta()), lep->Pt()) : fakeEst->GetMuonFakeRate(_param.Muon_Tight_ID, _param.Muon_FR_Key, fabs(lep->Eta()), lep->Pt()); 
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
  if(Analyzer == "HNL_SignalRegionPlotter"){
    fakeEst->SetEra(GetEra());
    fakeEst->ReadHistograms();
  }
  //==== CFBackgroundEstimator
  if(Analyzer == "HNL_SignalRegionPlotter"){
    cfEst->SetEra(GetEra());
    cfEst->ReadHistograms();
  }
}


float AnalyzerCore::GetKFactor(){

  if(IsDATA) return 1.;

  float weight = 1.;

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
  else if(MCSample.Contains("ggZZto")){
    //  1.67 brings gg->ZZ from LO to NLO (http://arxiv.org/abs/1509.06734)
    return 1.67;
  }
  else if(MCSample.Contains("ggHtoZZ")){
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


vector<Muon> AnalyzerCore::SkimLepColl(const vector<Muon>& MuColl, vector<Gen>& TruthColl, AnalyzerParameter param, TString Option){

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
      int LepType=GetLeptonType_JH(MuColl.at(i), TruthColl); 
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



vector<Electron> AnalyzerCore::SkimLepColl(const vector<Electron>& ElColl, vector<Gen>& TruthColl, AnalyzerParameter param,TString Option){

  bool GetPrompt=false, GetHadFake=false, GetEWtau=false, GetNHIntConv=false, GetNHExtConv=false, GetCF=false;
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
      int LepType=GetLeptonType_JH(ElColl.at(i), TruthColl); bool PassSel=false;
      
      if( GetPrompt    && (LepType==1 || LepType==2) ) PassSel=true;
      if( GetHadFake   && (LepType<0 && LepType>=-4) ) PassSel=true;
      if( GetEWtau     &&         LepType==3         ) PassSel=true;
      if( GetNHIntConv &&         LepType>=4         ) PassSel=true;
      if( GetNHExtConv &&         LepType<-4         ) PassSel=true;
      if( GetCF        &&         IsCF(ElColl.at(i), TruthColl) ) PassSel=true;
      
      if( PassSel ) ReturnVec.push_back(ElColl.at(i));
    }
  }

  return ReturnVec;
}



vector<Electron> AnalyzerCore::SkimLepColl(const vector<Electron>& ElColl, TString Option){
  
  vector<Electron> ReturnColl;

  bool Barrel1=false, Barrel2=false, Endcap=false, PtCut=false;
  if(Option.Contains("B1")) Barrel1=true;
  if(Option.Contains("B2")) Barrel2=true;
  if(Option.Contains("E"))  Endcap =true;
  if(Option.Contains("Pt")) PtCut  =true;

  for(unsigned int i=0; i<ElColl.size(); i++){
    bool PassSel=false; float fEta=fabs(ElColl.at(i).Eta());
    if( Barrel1 && fEta <0.8               ) PassSel=true;
    if( Barrel2 && fEta>=0.8 && fEta<1.479 ) PassSel=true;
    if( Endcap  && fEta>=1.479 && fEta<2.5 ) PassSel=true;
    if( PassSel ) ReturnColl.push_back(ElColl.at(i));
  }

  return ReturnColl;
}


vector<Muon> AnalyzerCore::SkimLepColl(const vector<Muon>& MuColl,  TString Option){
  
  vector<Muon> ReturnColl;
  bool Barrel=false, Overlap=false, Endcap=false, PtCut=false;
  if(Option.Contains("MB")) Barrel =true;
  if(Option.Contains("MO")) Overlap=true;
  if(Option.Contains("ME")) Endcap =true;
  if(Option.Contains("Pt")) PtCut  =true;

  for(unsigned int i=0; i<MuColl.size(); i++){
    bool PassSel=false; float fEta=fabs(MuColl.at(i).Eta());
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

  return out;

}

std::vector<Muon> AnalyzerCore::MuonPromptOnly(const std::vector<Muon>& muons, const std::vector<Gen>& gens, AnalyzerParameter param){

  if(IsDATA) return muons;

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){
    if(param.MuFakeMethod == "Data"){
      if(GetLeptonType(muons.at(i), gens)<=0) continue;
    }
    out.push_back( muons.at(i) );
  }

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

Particle AnalyzerCore::UpdateMET(const Particle& METv, const std::vector<Muon>& muons){

  float met_x = METv.Px();
  float met_y = METv.Py();

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

  float met_x = METv.Px();
  float met_y = METv.Py();

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



std::vector<Muon> AnalyzerCore::MuonApplyPtCut(const std::vector<Muon>& muons, double ptcut){

  std::vector<Muon> out;

  for(unsigned int i=0; i<muons.size(); i++){
    if(!( muons.at(i).Pt() > ptcut )) continue;
    out.push_back( muons.at(i) );
  }

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
  
  return out;
  
}

std::vector<Electron> AnalyzerCore::ElectronPromptOnly(const std::vector<Electron>& electrons, const std::vector<Gen>& gens, AnalyzerParameter param){

  if(IsDATA) return electrons;

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    bool pass=true;
    //if(GetLeptonType(electrons.at(i), gens)<=0) continue;

    if(param.ElFakeMethod == "Data"){
      if(GetLeptonType(electrons.at(i), gens) <= 0)pass=false;
    }

    if(param.ElFakeMethod == "DataNoConv"){
      if(GetLeptonType(electrons.at(i), gens) <= 0)pass=false;
      if(GetLeptonType(electrons.at(i), gens)== -5)pass=true;
      if(GetLeptonType(electrons.at(i), gens)== -6)pass=true;
    }
    if(param.CFMethod == "Data"){
      if(IsCF(electrons.at(i), gens)) pass=false;
    }
    
    if(pass)out.push_back( electrons.at(i) );
  }

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

  return out;

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
    if( ! ( abs(gen.PID())==22 && (gen.Status()==1 || gen.Status()==23) ) ) continue;//2)
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


bool AnalyzerCore::ConversionSplitting(std::vector<Lepton *> leps,const std::vector<Gen>& gens){
  

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
	//	else return !GENTMatched;
  //}

  //if(MCSample.Contains("ZGTo") )   {
  //  if(leps.back()->Pt()>15.) return true;
  //  else return GENTMatched;
  //}
  //else if(MCSample.Contains("DYJet")){
  //  if(leps.back()->Pt()>15.) return false;
	//	else return !GENTMatched;
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

  float dPhiMax=0.3, dEtaMax=0.1;
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

void AnalyzerCore::FillWeightHist(TString label, double _weight){

  if(!label.Contains("Syst_"))
    FillHist( "weights/"+ label , _weight ,1., 200, -5., 5,"ev weight");

  return;
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
    cout << "Writing " << this_name << endl;
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

  float dPhiMax=0.3, dEtaMax=0.1;
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
  float dR=999., dRmax=0.1;

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

  return MatchedIdx;
}


int AnalyzerCore::GetNearPhotonIdx(const Lepton& Lep, std::vector<Gen>& TruthColl){
  // Return gen-photon index, which are candidate for source of external conversion. 1)

  int NearPhotonIdx=-1;
  float PTthreshold=10., dPtRelmax=0.5;//2)
  float dRmax=0.2;//3)
  float dRmin=999.;
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






