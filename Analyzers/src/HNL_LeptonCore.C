#include "HNL_LeptonCore.h"

void HNL_LeptonCore::initializeAnalyzer(){


  
  SglMuon_Channel    =   (IsDATA && (this->DataStream == "SingleMuon"        )); //--> Needed when adding Mu50 + validation                 
  DblEG_Channel      =   (IsDATA && (this->DataStream == DoubleElectronPD()  ));
  DblMuon_Channel    =   (IsDATA && (this->DataStream == DoubleMuonPD()      ));
  SglEG_Channel      =   (IsDATA && (this->DataStream == SingleElectronPD()      ));
  MuonEG_Channel     =  (IsDATA && (this->DataStream == MuonEGPD()      ));
    
  //=== VERBOSE                                                                                                                                        
  run_Debug = HasFlag("DEBUG");
  

  //=== bkg flags                                                                                                                                      
  RunFake   = HasFlag("RunFake");
  RunCF     = HasFlag("RunCF");
  RunConv   = HasFlag("RunConv");
  RunPromptTLRemoval = HasFlag("RunPromptTLRemoval");
  run_ORTrigger = HasFlag("MultiTrig");

  /// Other flags                                                                                                                                      
  RunSyst = HasFlag("RunSyst");

  HEM1516 = HasFlag("HEM1516");


  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Loose, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Tight, JetTagging::incl, JetTagging::comb) );
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Loose, JetTagging::incl, JetTagging::mujets));
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets));
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets));


  // clear Trig vect

  TrigList_HNL_HighPtMu.clear();
  TrigList_HNL_DblMu.clear();
  TrigList_HNL_Mu.clear();
  TrigList_Full_Mu.clear();
  TrigList_POG_Mu.clear();

  // EG                                                                                                                                                                              
  TrigList_HNL_DblEG.clear();
  TrigList_HNL_EG.clear();
  TrigList_POG_EG.clear();
  TrigList_HNL_HighPtEG.clear();
  TrigList_Full_EG.clear();

  // MUG                                                                                                                                                                             
  TrigList_HNL_MuEG.clear();
  TrigList_POG_MuEG.clear();
  TrigList_Full_MuEG.clear();

  
  if(GetEraShort()=="2016a"){

    // Lumi = 19.5 fb-1
    //2016a run period 278808
    // MU
    TrigList_HNL_DblMu = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v", 
        "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v"};

    TrigList_HNL_MuEG  = {"HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"};
    TrigList_POG_MuEG  = {"HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v", "HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v"};

    // 278806 and 278807 should be removed frmo thsi dataset
    // REF https://twiki.cern.ch/twiki/bin/view/CMS/PdmVDatasetsUL2016
    // Run Range Bver2 [273150] - F [278770]
  }
  if(GetEraShort()=="2016b"){

    // Lumi=16.8 /fb.
    // MU                                                                                                                                                     
    TrigList_HNL_DblMu = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v", 
        "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"};// "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v"};
    
    TrigList_HNL_MuEG  = {"HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v", "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"};
    TrigList_POG_MuEG  = {"HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v", "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"};
    

    //https://twiki.cern.ch/twiki/bin/view/CMS/MuonHLT2016
    // Do we include HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v which is only in periodH
    // Do we include HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v in perfiod F + G
  }

  if(DataYear==2016){

    // Mu};
    
    TrigList_POG_Mu = {    "HLT_IsoMu24_v",   "HLT_IsoTkMu24_v",};
    TrigList_HNL_HighPtMu = {"HLT_Mu50_v",   "HLT_TkMu50_v"};
    TrigList_HNL_Mu = {    "HLT_IsoMu24_v",   "HLT_IsoTkMu24_v",};
    // EG                                                                                                                                                     
    TrigList_HNL_DblEG = {  "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v"};
    TrigList_HNL_EG    = {  "HLT_Ele27_WPTight_Gsf_v" ,
          "HLT_Ele25_eta2p1_WPTight_Gsf_v" ,
          "HLT_Ele115_CaloIdVT_GsfTrkIdT_v", 
          "HLT_Photon175_v",
          "HLT_Ele45_CaloIdVT_GsfTrkIdT_PFJet200_PFJet50_v",
          "HLT_Photon175_v",
          "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v",
          "HLT_Photon175_v",
          "HLT_DoubleEle33_CaloIdL_MW_v",
          "HLT_DoubleEle37_Ele27_CaloIdL_GsfTrkIdVL_v"};
    //https://twiki.cern.ch/twiki/bin/view/CMS/EgHLTRunIISummary
    TrigList_HNL_HighPtEG = {"HLT_Photon175_v","HLT_DoublePhoton60_v"};
    TrigList_POG_EG    = {  "HLT_Ele27_WPTight_Gsf_v" };

    //https://twiki.cern.ch/twiki/bin/view/CMS/EgHLTPathDetails

    
  }  // END OF 2016 Triggers                                                                                                                 

  else if(DataYear==2017){


    TrigList_HNL_DblMu = { "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v", 
         "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v",
         "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v" };
    TrigList_HNL_Mu = {  "HLT_IsoMu27_v"};
    TrigList_POG_Mu = {  "HLT_IsoMu27_v"};
    TrigList_HNL_HighPtMu = {"HLT_Mu50_v"};


    TrigList_HNL_DblEG = { "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"};
    TrigList_HNL_EG = { "HLT_Ele32_WPTight_Gsf_v", 
      "HLT_Ele32_WPTight_Gsf_L1DoubleEG_v",
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_Photon200_v",
      "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v",
      "HLT_Photon200_v",
      "HLT_DoubleEle33_CaloIdL_MW_v"};
    TrigList_POG_EG = {"HLT_Ele32_WPTight_Gsf_v", "HLT_Ele32_WPTight_Gsf_L1DoubleEG_v"};
    TrigList_HNL_HighPtEG = {"HLT_Photon200_v","HLT_DoublePhoton70_v"};

    TrigList_HNL_MuEG  = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"};

  } // end of 2017 triggers                                                                                                                  

  else if(DataYear==2018){

    TrigList_HNL_DblMu = {"HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v"};
    TrigList_HNL_Mu = {   "HLT_IsoMu24_v"};
    TrigList_HNL_HighPtMu = {"HLT_Mu50_v"};
    TrigList_POG_Mu = {   "HLT_IsoMu24_v"};
    
    // MAIN TRIGGER
    TrigList_HNL_DblEG = {"HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v"};
    
    TrigList_HNL_EG = { "HLT_Ele32_WPTight_Gsf_v",
      "HLT_DoubleEle25_CaloIdL_MW_v" ,
      "HLT_Ele115_CaloIdVT_GsfTrkIdT_v",
      "HLT_Ele50_CaloIdVT_GsfTrkIdT_PFJet165_v"};

    TrigList_POG_EG = { "HLT_Ele32_WPTight_Gsf_v"};

    TrigList_HNL_HighPtEG = { "HLT_Photon200_v", "HLT_DoublePhoton70_v"};

    TrigList_HNL_MuEG  = {"HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v", "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v"};

  }   

  //--- Method 1d using JetTagging::iterativefit needs csv file changing in histmap to run                                                   
  // jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::iterativefit, JetTagging::iterativefit) );  
  mcCorr->SetJetTaggingParameters(jtps);

  
  TrigList_Full_Mu.clear();
  TrigList_Full_EG.clear();
  TrigList_Full_MuEG.clear();
  for(auto itrig : TrigList_HNL_DblMu) TrigList_Full_Mu.push_back(itrig);
  for(auto itrig : TrigList_HNL_Mu) TrigList_Full_Mu.push_back(itrig);
  for(auto itrig : TrigList_HNL_HighPtMu) TrigList_Full_Mu.push_back(itrig);
  
  for(auto itrig : TrigList_HNL_DblEG) TrigList_Full_EG.push_back(itrig);
  for(auto itrig : TrigList_HNL_EG) TrigList_Full_EG.push_back(itrig);
  for(auto itrig : TrigList_HNL_HighPtEG) TrigList_Full_EG.push_back(itrig);

  for(auto itrig : TrigList_HNL_MuEG) TrigList_Full_MuEG.push_back(itrig);
  for(auto itrig : TrigList_Full_Mu)TrigList_Full_MuEG.push_back(itrig);
  for(auto itrig : TrigList_Full_EG) TrigList_Full_MuEG.push_back(itrig);

  for(auto itrig : TrigList_Full_Mu){
    cout << "TrigList_Full_Mu  : "<< itrig << endl;
  }
  for(auto itrig : TrigList_Full_EG){
    cout << "TrigList_Full_EG  : "<< itrig << endl;
  } 

  
  //SetupIDMVAReader(false,BkgType::Fake,"");
  
  //if (Analyzer == "HNL_SignalRegionPlotter")SetupMVAReader();
  //if (Analyzer == "HNL_SignalLeptonOpt")SetupMVAReader();

}

void HNL_LeptonCore::OutCutFlow(TString lab, double w){
  
  
  std::map<TString, double>::iterator mapit = cfmap.find(lab);
  if(mapit != cfmap.end()) {
    cfmap[mapit->first] = mapit->second+w;
  }
  else     cfmap[lab]= w;

  
  return;

}

bool HNL_LeptonCore::IsSignal(){

  if (MCSample.Contains("TypeI")) return true;
  return false;

}


double HNL_LeptonCore::MergeMultiMC(vector<TString> vec, TString Method){

  // This function can be used to merge MC bkg with multiple samples
  
  if(IsData) return 1;
  if(IsSignal()) return 1;

  if (std::count(vec.begin(), vec.end(), "WJet")) {
    /// Merge sherpa and HT binned
    if (MCSample.Contains("WJet")){
      return 0.5;
    }
  }

  if (std::count(vec.begin(), vec.end(), "DY")) {
    if (MCSample.Contains("DYJets")){
      return 0.25;
    }
  }


  if (std::count(vec.begin(), vec.end(), "DYMG")) {
    if (MCSample.Contains("DYJets_MG")){
      return 0.5;
    }
  }
  if (std::count(vec.begin(), vec.end(), "WG")) {
    
    if (MCSample.Contains("WGToLNuG")){
      
      int NearPhotonIdx=-1;
      
      for(unsigned int i=2; i<gens.size(); i++){
  Gen gen = gens[i];
  if( gens.at(i).MotherIndex()<0   ) continue;
  if( !(gens.at(i).PID()==22 && (gens.at(i).Status()==23)) ) continue;
  NearPhotonIdx=i;
      }
      if(NearPhotonIdx < 0) {
  double maxpt=0;
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens[i];
    if( gens.at(i).MotherIndex()<0   ) continue;
    if( !(gens.at(i).PID()==22 && (gens.at(i).Status()==1)) ) continue;
    if(gen.isPromptFinalState() && gen.Pt() > maxpt) {
      NearPhotonIdx=i;
      maxpt = gen.Pt();
    }
  }
      }

      if(NearPhotonIdx < 0) return 0;
      
      double phPt= gens[NearPhotonIdx].Pt();
    
      if(Method == "Split"){
  if (MCSample == "WGToLNuG_01J_PtG_130"){
    if (phPt < 300) return 1.;
  }
  else if (MCSample == "WGToLNuG_01J_PtG_300"){
    if (phPt < 500) return 1.;
  }
  else if (MCSample == "WGToLNuG_01J_PtG_500"){
    return 1.;
      }
  else {
    if (phPt <= 130)  return 1.;
    else return 0.;
  }
      }
      
      if(Method.Contains("Combine")){
  double nWG = (Method == "CombineAll") ? 2. : 1.;
  if (phPt < 130 ) return (1./nWG);
  if (phPt < 300 ) return (1./(nWG+1.));
  if (phPt < 500 ) return (1./(nWG+2.));
  return (1./(nWG+3.));
      
      }
    }
  }
  
  return 1.;
}



/*
void HNL_LeptonCore::SetupMVAReader(){

  InitializeTreeVars();

  MVAReader->AddVariable("Nj"      , &Nj      );
  MVAReader->AddVariable("Nvbfj", &Nvbfj);
  MVAReader->AddVariable("Ptl1", &Ptl1);
  MVAReader->AddVariable("Ptl2", &Ptl2);
  MVAReader->AddVariable("Ptj1", &Ptj1);
  MVAReader->AddVariable("Ptj2", &Ptj2);
  MVAReader->AddVariable("MET2ST", &MET2ST);
  MVAReader->AddVariable("HTLT1", &HTLT1);
  MVAReader->AddVariable("HTLT2", &HTLT2);
  MVAReader->AddVariable("dRll", &dRll);
  MVAReader->AddVariable("dRlj11", &dRlj11);
  MVAReader->AddVariable("dRlj12", &dRlj12);
  MVAReader->AddVariable("dRlj21", &dRlj21);
  MVAReader->AddVariable("dRlj22", &dRlj22);
  MVAReader->AddVariable("MSSSF", &MSSSF);
  MVAReader->AddVariable("Mlj11", &Mlj11);
  MVAReader->AddVariable("Mlj12", &Mlj12);
  MVAReader->AddVariable("Mlj21", &Mlj21);
  MVAReader->AddVariable("Mlj22", &Mlj22);
  MVAReader->AddVariable("Mllj1", &Mllj1);
  MVAReader->AddVariable("Mllj2", &Mllj2);
  MVAReader->AddVariable("PtWj1", &PtWj1);
  MVAReader->AddVariable("PtWj2", &PtWj2);
  MVAReader->AddVariable("dRWjj", &dRWjj);
  MVAReader->AddVariable("dRlW12", &dRlW12);
  MVAReader->AddVariable("dRlW22", &dRlW22);
  MVAReader->AddVariable("M_W2_jj", &M_W2_jj);
  MVAReader->AddVariable("M_W1_lljj", &M_W1_lljj);
  MVAReader->AddVariable("M_N1_l1jj", &M_N1_l1jj);
  MVAReader->AddVariable("M_N2_l2jj", &M_N2_l2jj);
  MVAReader->AddSpectator("w_tot", &w_tot);


  TString AnalyzerPath=std::getenv("SKFlat_WD");
  TString MVAPath="/data/Run2UltraLegacy_v3/2018/BDTClassifier/results_xml/";
  MNStrList = {"85","90", "95", "100", "125", "150", "200", "250", "300", "400","500"};
  vector <TString> Channels = {"MuMu","EE","EMu"};
  for(unsigned int im=0; im<MNStrList.size() ; im++){
    for(auto ich : Channels){
      TString FileName ="DYTypeI_"+ich+ "_M"+MNStrList.at(im)+"_Mode0_Run2_BDT.weights.xml";
      TString MVATagStr = "BDTG_M"+MNStrList.at(im)+"_"+ich;

      MVAReader->BookMVA(MVATagStr, AnalyzerPath+MVAPath+FileName);
    }
  }

  return;
}
*/

void HNL_LeptonCore::SetupMVAReader(){

  InitializeTreeVars();

  TString AnalyzerPath=std::getenv("SKFlat_WD");
  TString MVAPath = "/data/Run2UltraLegacy_v3/Run2/BDTClassifier/results_xml/";
  MNStrList = {"90", "100", "150", "200", "300", "400", "500"};
  NCutList  = {"300"};
  NTreeList = {"850"};

  MVAReaderMM->AddVariable("Nvbfj", &Nvbfj);
  MVAReaderMM->AddVariable("Ptl1", &Ptl1);
  MVAReaderMM->AddVariable("Ptl2", &Ptl2);
  MVAReaderMM->AddVariable("Ptj1", &Ptj1);
  MVAReaderMM->AddVariable("Ptj2", &Ptj2);
  MVAReaderMM->AddVariable("MET2ST", &MET2ST);
  MVAReaderMM->AddVariable("HTLT1", &HTLT1);
  MVAReaderMM->AddVariable("HTLT2", &HTLT2);
  MVAReaderMM->AddVariable("Etal1", &Etal1);
  MVAReaderMM->AddVariable("Etal2", &Etal2);
  MVAReaderMM->AddVariable("PtRatioAJl1", &PtRatioAJl1);
  MVAReaderMM->AddVariable("PtRatioAJl2", &PtRatioAJl2);
  MVAReaderMM->AddVariable("NEMFracAJl1", &NEMFracAJl1);
  MVAReaderMM->AddVariable("NEMFracAJl2", &NEMFracAJl2);
  MVAReaderMM->AddVariable("CHFracAJl1", &CHFracAJl1);
  MVAReaderMM->AddVariable("CHFracAJl2", &CHFracAJl2);
  MVAReaderMM->AddVariable("NHFracAJl1", &NHFracAJl1);
  MVAReaderMM->AddVariable("NHFracAJl2", &NHFracAJl2);
  MVAReaderMM->AddVariable("JetDiscAJl1", &JetDiscAJl1);
  MVAReaderMM->AddVariable("JetDiscAJl2", &JetDiscAJl2);
  MVAReaderMM->AddVariable("dRll", &dRll);
  MVAReaderMM->AddVariable("dRlj11", &dRlj11);
  MVAReaderMM->AddVariable("dRlj21", &dRlj21);
  MVAReaderMM->AddVariable("MSSSF", &MSSSF);
  MVAReaderMM->AddVariable("Mlj11", &Mlj11);
  MVAReaderMM->AddVariable("Mlj21", &Mlj21);
  MVAReaderMM->AddVariable("Mlj22", &Mlj22);
  MVAReaderMM->AddVariable("MTvl1", &MTvl1);
  MVAReaderMM->AddVariable("MTvl2", &MTvl2);
  MVAReaderMM->AddVariable("Mllj1", &Mllj1);
  MVAReaderMM->AddVariable("dRWjj", &dRWjj);
  MVAReaderMM->AddVariable("M_W2_jj", &M_W2_jj);
  MVAReaderMM->AddVariable("M_N2_l2jj", &M_N2_l2jj);
  MVAReaderMM->AddVariable("Nb", &Nb);
  MVAReaderMM->AddVariable("MuFracAJl1", &MuFracAJl1);
  MVAReaderMM->AddVariable("M_N1_l1jj", &M_N1_l1jj);

  MVAReaderMM->AddSpectator("w_tot", &w_tot);

  MVAReaderEE->AddVariable("Nvbfj", &Nvbfj);
  MVAReaderEE->AddVariable("Ptl1", &Ptl1);
  MVAReaderEE->AddVariable("Ptl2", &Ptl2);
  MVAReaderEE->AddVariable("Ptj1", &Ptj1);
  MVAReaderEE->AddVariable("Ptj2", &Ptj2);
  MVAReaderEE->AddVariable("MET2ST", &MET2ST);
  MVAReaderEE->AddVariable("HTLT1", &HTLT1);
  MVAReaderEE->AddVariable("HTLT2", &HTLT2);
  MVAReaderEE->AddVariable("Etal1", &Etal1);
  MVAReaderEE->AddVariable("Etal2", &Etal2);
  MVAReaderEE->AddVariable("PtRatioAJl1", &PtRatioAJl1);
  MVAReaderEE->AddVariable("PtRatioAJl2", &PtRatioAJl2);
  MVAReaderEE->AddVariable("NEMFracAJl1", &NEMFracAJl1);
  MVAReaderEE->AddVariable("NEMFracAJl2", &NEMFracAJl2);
  MVAReaderEE->AddVariable("CHFracAJl1", &CHFracAJl1);
  MVAReaderEE->AddVariable("CHFracAJl2", &CHFracAJl2);
  MVAReaderEE->AddVariable("NHFracAJl1", &NHFracAJl1);
  MVAReaderEE->AddVariable("NHFracAJl2", &NHFracAJl2);
  MVAReaderEE->AddVariable("JetDiscAJl1", &JetDiscAJl1);
  MVAReaderEE->AddVariable("JetDiscAJl2", &JetDiscAJl2);
  MVAReaderEE->AddVariable("dRll", &dRll);
  MVAReaderEE->AddVariable("dRlj11", &dRlj11);
  MVAReaderEE->AddVariable("dRlj21", &dRlj21);
  MVAReaderEE->AddVariable("MSSSF", &MSSSF);
  MVAReaderEE->AddVariable("Mlj11", &Mlj11);
  MVAReaderEE->AddVariable("Mlj21", &Mlj21);
  MVAReaderEE->AddVariable("Mlj22", &Mlj22);
  MVAReaderEE->AddVariable("MTvl1", &MTvl1);
  MVAReaderEE->AddVariable("MTvl2", &MTvl2);
  MVAReaderEE->AddVariable("Mllj1", &Mllj1);
  MVAReaderEE->AddVariable("dRWjj", &dRWjj);
  MVAReaderEE->AddVariable("M_W2_jj", &M_W2_jj);
  MVAReaderEE->AddVariable("M_N2_l2jj", &M_N2_l2jj);
  MVAReaderEE->AddVariable("dRlj12", &dRlj12);
  MVAReaderEE->AddVariable("dRlj22", &dRlj22);
  MVAReaderEE->AddVariable("Mllj2", &Mllj2);

  MVAReaderEE->AddSpectator("w_tot", &w_tot);

  MVAReaderEM->AddVariable("Nvbfj", &Nvbfj);
  MVAReaderEM->AddVariable("Ptl1", &Ptl1);
  MVAReaderEM->AddVariable("Ptl2", &Ptl2);
  MVAReaderEM->AddVariable("Ptj1", &Ptj1);
  MVAReaderEM->AddVariable("Ptj2", &Ptj2);
  MVAReaderEM->AddVariable("MET2ST", &MET2ST);
  MVAReaderEM->AddVariable("HTLT1", &HTLT1);
  MVAReaderEM->AddVariable("HTLT2", &HTLT2);
  MVAReaderEM->AddVariable("Etal1", &Etal1);
  MVAReaderEM->AddVariable("Etal2", &Etal2);
  MVAReaderEM->AddVariable("PtRatioAJl1", &PtRatioAJl1);
  MVAReaderEM->AddVariable("PtRatioAJl2", &PtRatioAJl2);
  MVAReaderEM->AddVariable("NEMFracAJl1", &NEMFracAJl1);
  MVAReaderEM->AddVariable("NEMFracAJl2", &NEMFracAJl2);
  MVAReaderEM->AddVariable("CHFracAJl1", &CHFracAJl1);
  MVAReaderEM->AddVariable("CHFracAJl2", &CHFracAJl2);
  MVAReaderEM->AddVariable("NHFracAJl1", &NHFracAJl1);
  MVAReaderEM->AddVariable("NHFracAJl2", &NHFracAJl2);
  MVAReaderEM->AddVariable("JetDiscAJl1", &JetDiscAJl1);
  MVAReaderEM->AddVariable("JetDiscAJl2", &JetDiscAJl2);
  MVAReaderEM->AddVariable("dRll", &dRll);
  MVAReaderEM->AddVariable("dRlj11", &dRlj11);
  MVAReaderEM->AddVariable("dRlj21", &dRlj21);
  MVAReaderEM->AddVariable("MSSSF", &MSSSF);
  MVAReaderEM->AddVariable("Mlj11", &Mlj11);
  MVAReaderEM->AddVariable("Mlj21", &Mlj21);
  MVAReaderEM->AddVariable("Mlj22", &Mlj22);
  MVAReaderEM->AddVariable("MTvl1", &MTvl1);
  MVAReaderEM->AddVariable("MTvl2", &MTvl2);
  MVAReaderEM->AddVariable("Mllj1", &Mllj1);
  MVAReaderEM->AddVariable("dRWjj", &dRWjj);
  MVAReaderEM->AddVariable("M_W2_jj", &M_W2_jj);
  MVAReaderEM->AddVariable("M_N2_l2jj", &M_N2_l2jj);
  MVAReaderEM->AddVariable("Nb", &Nb);
  MVAReaderEM->AddVariable("dRlj12", &dRlj12);
  MVAReaderEM->AddVariable("M_N1_l1jj", &M_N1_l1jj);

  MVAReaderEM->AddSpectator("w_tot", &w_tot);


  for(unsigned int im=0; im<MNStrList.size(); im++){

    //// This can be changed after checking Hyper paramters
    //FinalBDTHyperParamMap settings are whats used in limit for SR/CR
    FinalBDTHyperParamMap[MNStrList.at(im)] = make_pair("300","850");

    for(unsigned int ic=0; ic<NCutList.size(); ic++){
      for(unsigned int it=0; it<NTreeList.size(); it++){

        TString FileNameMM  = "output_DY_MuMu_M"+MNStrList.at(im)+"_Mode100_NTree"+NTreeList.at(it)+"_Run2_BDT.weights.xml";
        TString MVATagStrMM = "BDT_M"+MNStrList.at(im)+"_NCut"+NCutList.at(ic)+"_NTree"+NTreeList.at(it)+"_MuMu";

        MVAReaderMM->BookMVA(MVATagStrMM, AnalyzerPath+MVAPath+"NCuts"+NCutList.at(ic)+"/"+FileNameMM);

        TString FileNameEE  = "output_DY_EE_M"+MNStrList.at(im)+"_Mode100_NTree"+NTreeList.at(it)+"_Run2_BDT.weights.xml";
        TString MVATagStrEE = "BDT_M"+MNStrList.at(im)+"_NCut"+NCutList.at(ic)+"_NTree"+NTreeList.at(it)+"_EE";

        MVAReaderEE->BookMVA(MVATagStrEE, AnalyzerPath+MVAPath+"NCuts"+NCutList.at(ic)+"/"+FileNameEE);

        TString FileNameEM  = "output_DY_EMu_M"+MNStrList.at(im)+"_Mode100_NTree"+NTreeList.at(it)+"_Run2_BDT.weights.xml";
        TString MVATagStrEM = "BDT_M"+MNStrList.at(im)+"_NCut"+NCutList.at(ic)+"_NTree"+NTreeList.at(it)+"_EMu";

        MVAReaderEM->BookMVA(MVATagStrEM, AnalyzerPath+MVAPath+"NCuts"+NCutList.at(ic)+"/"+FileNameEM);

      }
    }
  }

}



double HNL_LeptonCore::GetHNLMVAMuon(Muon mu ,BkgType bkg){
  
  //  if(mu.Pt() < 20&&bkg==BkgType::Fake) return GetBDTScoreMuon(mu, bkg, "BDTG_LowPt");
  //  return GetBDTScoreMuon(mu, bkg, "BDTG");
  return -999;

}

double HNL_LeptonCore::GetHNLMVAElectron(Electron el ,BkgType bkg){

  //if(el.Pt() < 20) return GetBDTScoreEl(el, bkg, "BDTG_Bin1");
  //return GetBDTScoreEl(el, bkg, "BDTG");
  return -999;

}




void HNL_LeptonCore::InitializeTreeVars(){

  /*Nj=-1, Nvbfj=-1;
  Ptl1=-1, Ptl2=-1, Ptj1=-1, Ptj2=-1, Ptj3=-1, MET=-1, HTLT=-1, HTLT1=-1,HTLT2=-1,LT=-1,  HT=-1, MET2HT=-1, MET2ST=-1;
  dEtall=-1, dRll=-1, dRjj12=-1, dRjj23=-1, dRjj13=-1;
  dRlj11=-1, dRlj12=-1, dRlj13=-1, dRlj21=-1, dRlj22=-1, dRlj23=-1;
  MSSSF=-1, Mlj11=-1, Mlj12=-1, Mlj13=-1, Mlj21=-1, Mlj22=-1, Mlj23=-1;
  MTvl1=-1, MTvl2=-1, Mllj1=-1, Mllj2=-1, Mllj3=-1, Mllj4=-1;
  Mlljj12=-1, Mlljj13=-1, Mlljj14=-1, Mlljj23=-1, Mlljj24=-1, Mlljj34=-1;
  Mljj112=-1, Mljj113=-1, Mljj114=-1, Mljj123=-1, Mljj124=-1, Mljj134=-1;
  Mljj212=-1, Mljj213=-1, Mljj214=-1, Mljj223=-1, Mljj224=-1, Mljj234=-1;
  Mjj12=-1, Mjj13=-1, Mjj14=-1, Mjj23=-1, Mjj24=-1, Mjj34=-1;*/

  Nvbfj=-1, Nb=-1;
  Ptl1=-1, Ptl2=-1, Ptj1=-1, Ptj2=-1, HTLT1=-1, HTLT2=-1, MET2ST=-1, Etal1=-1, Etal2=-1;
  PtRatioAJl1=-1, PtRatioAJl2=-1;
  CEMFracAJl1=-1, CEMFracAJl2=-1, NEMFracAJl1=-1, NEMFracAJl2=-1, CHFracAJl1=-1, CHFracAJl2=-1, NHFracAJl1=-1, NHFracAJl2=-1, MuFracAJl1=-1, MuFracAJl2=-1, JetDiscAJl1=-1, JetDiscAJl2=-1;
  dRll=-1, dRlj11=-1, dRlj12=-1, dRlj21=-1, dRlj22=-1;
  MSSSF=-1, Mlj11=-1, Mlj12=-1, Mlj21=-1, Mlj22=-1;
  MTvl1=-1, MTvl2=-1, Mllj1=-1, Mllj2=-1; 

  PtWj1=-1, PtWj2=-1;
  dRWjj=-1, dRlW12=-1, dRlW22=-1;
  M_W2_jj=-1, M_W1_lljj=-1, M_N1_l1jj=-1, M_N2_l2jj=-1;
  
  w_tot=-1.;
}


void HNL_LeptonCore::TriggerPrintOut(Event ev){
  
  vector<TString> PDs = {"DoubleEG","DoubleMuon","MuonEG","SingleElectron","SingleMuon", "EGamma"};

  cout << "TriggerPrintOut: MuMu" << endl;
  for(auto itrig : TrigList_HNL_DblMu){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl; 
    }
  }
  
  cout << " " << endl;

  cout << "TriggerPrintOut: EE" << endl;
  for(auto itrig : TrigList_HNL_DblEG){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl;
    }
  }

  cout << " " << endl;


  cout << "TriggerPrintOut: EMu" << endl;
  for(auto itrig : TrigList_HNL_MuEG){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl;
    }
  }

  cout << " " << endl;

  return;

}
vector<AnalyzerParameter::Syst> HNL_LeptonCore::GetSystList(TString SystType){

  vector<AnalyzerParameter::Syst> SystList = {};

  if(SystType == "Initial"){
    SystList.push_back(AnalyzerParameter::JetResUp);
    SystList.push_back(AnalyzerParameter::JetResDown);
    //SystList.push_back(AnalyzerParameter::PUUp);
    //SystList.push_back(AnalyzerParameter::PUDown);
    //SystList.push_back(AnalyzerParameter::FRUp);
    //SystList.push_back(AnalyzerParameter::FRDown);
  }
  
  if(SystType == "All"){
    SystList = {AnalyzerParameter::JetResUp,AnalyzerParameter::JetResDown,
    AnalyzerParameter::JetEnUp, AnalyzerParameter::JetEnDown,
    AnalyzerParameter::JetMassUp,AnalyzerParameter::JetMassDown,                                                                                     
    AnalyzerParameter::JetMassSmearUp,AnalyzerParameter::JetMassSmearDown,                                                                           
    AnalyzerParameter::MuonRecoSFUp,AnalyzerParameter::MuonRecoSFDown,                                                                               
    AnalyzerParameter::MuonEnUp,AnalyzerParameter::MuonEnDown,                                                                                       
    AnalyzerParameter::MuonIDSFUp,AnalyzerParameter::MuonIDSFDown,                                                                                   
    AnalyzerParameter::MuonISOSFUp,AnalyzerParameter::MuonISOSFDown,                                                                                 
    AnalyzerParameter::ElectronRecoSFUp,AnalyzerParameter::ElectronRecoSFDown,                                                                       
    AnalyzerParameter::ElectronResUp,AnalyzerParameter::ElectronResDown,                                                                             
    AnalyzerParameter::ElectronEnUp,AnalyzerParameter::ElectronEnDown,                                                                               
    AnalyzerParameter::ElectronIDSFUp,AnalyzerParameter::ElectronIDSFDown,                                                                           
    AnalyzerParameter::ElectronTriggerSFUp,AnalyzerParameter::ElectronTriggerSFDown,                                                                 
    AnalyzerParameter::BTagSFHTagUp,AnalyzerParameter::BTagSFHTagDown,                                                                               
    AnalyzerParameter::BTagSFLTagUp,AnalyzerParameter::BTagSFLTagDown,                                                                               
    AnalyzerParameter::METUnclUp,AnalyzerParameter::METUnclDown,                                                                                     
    //AnalyzerParameter::CFUp,AnalyzerParameter::CFDown,                                                                                               
    //AnalyzerParameter::FRUp,AnalyzerParameter::FRDown,                                                                                               
    AnalyzerParameter::PrefireUp,AnalyzerParameter::PrefireDown,                                                                                     
    AnalyzerParameter::PUUp,AnalyzerParameter::PUDown};
    
    
  }
  
  return SystList;

}
//====================================================/====================================================
//====================================================/====================================================
//====================================================/====================================================


bool HNL_LeptonCore::PassHEMVeto(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps){

  int nel_hem(0);
  if (channel==EE){
    if (DataEra=="2018"){
      for(auto iel : leps){
        if (iel->Eta() < -1.25){
          if((iel->Phi() < -0.82) && (iel->Phi() > -1.62)) nel_hem++;
        }
      }
    }
  }

  if(nel_hem >0) return false;
  
  return true;

}


bool HNL_LeptonCore::PassMultiTriggerSelection(HNL_LeptonCore::Channel channel,Event ev, std::vector<Lepton *> leps, TString selectionMain, TString selectionOR){

  
  bool PassTrigger  = PassTriggerSelection(channel, ev,leps, selectionMain, true);
  bool PassTrigger2  = PassTriggerSelection(channel, ev,leps, selectionOR, true);
  bool PassTriggerAllPD  = PassTriggerSelection(channel, ev,leps, selectionMain, false); 

  if(!IsData) return (PassTrigger || PassTrigger2);
  
  if(PassTrigger) return true;
  if(PassTriggerAllPD) return false;
  return PassTrigger2;
  
}

bool HNL_LeptonCore::PassTriggerSelection(HNL_LeptonCore::Channel channel,Event ev, std::vector<Lepton *> leps, TString selection, bool check_pd ){

  bool PassTrigger(false);

  bool apply_ptcut=true;
  
  if (channel == MuMu){
    
    std::vector<Lepton *> leps_muon;
    for(auto ilep : leps) {
      if(ilep->LeptonFlavour() == Lepton::MUON) leps_muon.push_back(ilep);
    }
    
    if(selection == "Dilep"){
      
      // Check It passes DiMu Trigger
      // Check if for data we are running on correct data stream
      
      return  PassTriggerAndCheckStream(apply_ptcut,leps_muon,ev,TrigList_HNL_DblMu,check_pd);

    }
    
    else     if(selection == "Lep"){
      
      return  PassTriggerAndCheckStream(apply_ptcut,leps_muon,ev,TrigList_HNL_Mu,check_pd);
      
    }
    else     if(selection == "POG"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps_muon,ev,TrigList_POG_Mu,check_pd);

    }
    
    else if(selection == "HighPt"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps_muon,ev,TrigList_HNL_HighPtMu,check_pd);


    }
    else if(selection == "Full"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps_muon,ev,TrigList_Full_Mu,check_pd);

    }
    else {
      cout << "[HNL_LeptonCore::PassTriggerSelection ] selection " << selection <<  "not found.." << endl;
      exit(EXIT_FAILURE);
    }
  }
  
  
  if (channel == EE){
    
    std::vector<Lepton *> leps_eg;
    
    for(auto ilep : leps) {
      if(ilep->LeptonFlavour() == Lepton::ELECTRON) leps_eg.push_back(ilep);
    }
    
    if(selection == "Dilep"){
      
      return  PassTriggerAndCheckStream(apply_ptcut,leps_eg,ev,TrigList_HNL_DblEG,check_pd);
    }

    else     if(selection == "POG"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps_eg,ev,TrigList_POG_EG,check_pd);
    }

    else     if(selection == "Lep"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps_eg,ev,TrigList_HNL_EG,check_pd);
    }

    else if(selection == "HighPt"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps_eg,ev,TrigList_HNL_HighPtEG,check_pd);
 
    }
    
    
    else if(selection == "Full"){
      
      return  PassTriggerAndCheckStream(apply_ptcut,leps_eg,ev,TrigList_Full_EG,check_pd);

    }
    else {
      
      cout << "[HNL_LeptonCore::PassTriggerSelection ] selection " << selection << "  not found.." << endl;
      exit(EXIT_FAILURE);
    }
  }



  if (channel == EMu){
    
    std::vector<Lepton *> leps_muon;
    for(auto ilep : leps) {
      if(ilep->LeptonFlavour() == Lepton::MUON) leps_muon.push_back(ilep);
    }
    if(selection == "Dilep"){
      
      return  PassTriggerAndCheckStream(apply_ptcut,leps,ev,TrigList_HNL_MuEG,check_pd);

    }
    else if(selection == "Lep"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps,ev,TrigList_HNL_Mu,check_pd) || PassTriggerAndCheckStream(apply_ptcut,leps,ev,TrigList_HNL_EG,check_pd);

    }

    else if(selection == "HighPt"){


      return  PassTriggerAndCheckStream(apply_ptcut,leps,ev,TrigList_HNL_HighPtMu,check_pd) || PassTriggerAndCheckStream(apply_ptcut,leps,ev,TrigList_HNL_HighPtEG,check_pd);

    }
    else     if(selection == "POG"){

      return  PassTriggerAndCheckStream(apply_ptcut,leps_muon,ev,TrigList_POG_Mu,check_pd);
    }    

    else    if(selection == "Full"){
      
      return  PassTriggerAndCheckStream(apply_ptcut,leps,ev,TrigList_Full_MuEG,check_pd);

    }
    
    else {
      cout << "[HNL_LeptonCore::PassTriggerSelection ] selection " <<  selection << " not found.." << endl;
      exit(EXIT_FAILURE);
    }
  }
  
       



  return PassTrigger;
}

AnalyzerParameter HNL_LeptonCore::InitialiseHNLParameter(TString s_setup, TString tag){
  
  /// This functions sets up AnalyzerParameter class for different types of Analyser
  
  AnalyzerParameter param  ;
  param.Clear();

  param.Name  = s_setup+tag;
  param.DefName  =s_setup+tag;


  // Default settings
  
  param.syst_ = AnalyzerParameter::Central;
  param.MCCorrrectionIgnoreNoHist = true;

  /// By default setup POG ID 
  /// By Default use MC for all Bkgs
  param.MuFakeMethod = "MC";
  param.ElFakeMethod = "MC";
  param.CFMethod   = "MC";
  param.ConvMethod = "MC";

  /// By default use tight Jets 
  param.Jet_ID                     = "tight";
  param.Jet_MinPt = 10.;
  param.Jet_MaxEta = 5.;

  param.FatJet_ID                  = "tight";
  param.FatJet_MinPt = 200.;
  param.FatJet_MaxEta = 5.;

  param.BJet_Method                = "1a";

  param.Muon_MinPt = 5.;
  param.Muon_MaxEta = 2.4;

  param.Electron_MinPt = 10.;
  param.Electron_MaxEta = 2.5;

  /// Lepton IDs
  param.Muon_Veto_ID = "HNVeto2016";
  param.Electron_Veto_ID = "HNVeto";
  param.Tau_Veto_ID = "JetVLElVLMuVL";
  
  /// Fakes
  param.Muon_FR_Key  ="pt_eta_AwayJetPt40";
  param.Muon_PR_Key  ="ptcone_eta_central";
  param.Electron_FR_Key  = "pt_eta_AwayJetPt40";
  param.Electron_PR_Key  ="ptcone_eta_central";

  /// Defaul Corrections                                                                                                                     

  param.Muon_ID_SF_Key = "NUM_TightID_DEN_TrackerMuons";
  param.Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";
  param.Muon_Tight_ID = "POGTightWithTightIso";
  param.Muon_RECO_SF_Key = "MuonRecoSF";


  param.Electron_ID_SF_Key = "passTightID";
  param.Electron_Tight_ID = "passPOGTight";

  TString trigKey=TrigList_POG_Mu[0];
  trigKey=trigKey.ReplaceAll("HLT_","");
  trigKey=trigKey.ReplaceAll("_v","");
  param.Muon_Trigger_SF_Key = "POGTight";
  param.Muon_Trigger_NameForSF = trigKey;

  param.Muon_FR_ID = "HNLooseV1";
  param.Electron_FR_ID = "HNLoosePOG";

  if (s_setup=="") return param;

  if (s_setup=="SignalStudy"){
    param.CFMethod   = "MC";
    param.Muon_Tight_ID = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    return param;
  }
  else if (s_setup=="HNL"){
    param.CFMethod   = "Data";
    param.Muon_Tight_ID = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";
    param.Electron_ID_SF_Key = "TmpHNTightV2";
    param.Muon_ID_SF_Key = "TmpHNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    return param;
  }
  else if (s_setup=="HNLOpt"){
    param.MuFakeMethod = "MC";
    param.ElFakeMethod = "MC";
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";
    param.Electron_ID_SF_Key = "TmpHNTightV2";
    param.Muon_ID_SF_Key = "TmpHNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    return param;
  }
  else if (s_setup=="HNLSROpt"){
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";
    param.Electron_ID_SF_Key = "TmpHNTightV2";
    param.Muon_ID_SF_Key = "TmpHNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    return param;
  }
  else if (s_setup=="BDT"){
    param.MuFakeMethod = "MC";
    param.ElFakeMethod = "MC";
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "MVAID";
    param.Electron_Tight_ID = "MVAID";
    param.Electron_ID_SF_Key = "TmpHNTightV2";
    param.Muon_ID_SF_Key = "TmpHNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    return param;
  }
  else if (s_setup=="MVAUL"){
    param.MuFakeMethod = "DATA";
    param.ElFakeMethod = "DATA";
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "HNL_ULID_"+GetYearString();
    param.Electron_Tight_ID = "HNL_ULID_"+GetYearString();
    param.Electron_ID_SF_Key = "TmpHNL_ULID_"+GetYearString();
    param.Muon_ID_SF_Key = "TmpHNL_ULID_"+GetYearString();
    param.Muon_FR_ID = "HNL_ULID_FO_"+GetYearString();
    param.Electron_FR_ID = "HNL_ULID_FO_"+GetYearString();
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    return param;
  }
  else if (s_setup.Contains("MVAULN")){
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "HNL_ULID_"+GetYearString();

    if (s_setup=="MVAULN1")  param.Electron_Tight_ID = "HNL_ULID_Baseline";
    if (s_setup=="MVAULN2")  param.Electron_Tight_ID = "HNL_ULID_CF";
    if (s_setup=="MVAULN3")  param.Electron_Tight_ID = "HNL_ULID_Fake";
    if (s_setup=="MVAULN4")  param.Electron_Tight_ID = "HNL_ULID_Conv";
    if (s_setup=="MVAULNCF2EDPt1")  param.Electron_Tight_ID = "HNL_ULID_CFT_ED_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF2EDPt2")  param.Electron_Tight_ID = "HNL_ULID_CFM_ED_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF2EDPt3")  param.Electron_Tight_ID = "HNL_ULID_CFL_ED_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF2EDPt4")  param.Electron_Tight_ID = "HNL_ULID_CFVL_ED_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF2ED1")  param.Electron_Tight_ID = "HNL_ULID_CFT_ED_BDTGv2_MD3";
    if (s_setup=="MVAULNCF2ED2")  param.Electron_Tight_ID = "HNL_ULID_CFM_ED_BDTGv2_MD3";
    if (s_setup=="MVAULNCF2ED3")  param.Electron_Tight_ID = "HNL_ULID_CFL_ED_BDTGv2_MD3";
    if (s_setup=="MVAULNCF2ED4")  param.Electron_Tight_ID = "HNL_ULID_CFVL_ED_BDTGv2_MD3";
    if (s_setup=="MVAULNCF3ED1")  param.Electron_Tight_ID = "HNL_ULID_CFT_ED_BDTGv3_MD3";
    if (s_setup=="MVAULNCF3ED2")  param.Electron_Tight_ID = "HNL_ULID_CFM_ED_BDTGv3_MD3";
    if (s_setup=="MVAULNCF3ED3")  param.Electron_Tight_ID = "HNL_ULID_CFL_ED_BDTGv3_MD3";
    if (s_setup=="MVAULNCF3ED4")  param.Electron_Tight_ID = "HNL_ULID_CFVL_ED_BDTGv3_MD3";
    if (s_setup=="MVAULNCF2Pt1")  param.Electron_Tight_ID = "HNL_ULID_CFT_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF2Pt2")  param.Electron_Tight_ID = "HNL_ULID_CFM_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF2Pt3")  param.Electron_Tight_ID = "HNL_ULID_CFL_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF2Pt4")  param.Electron_Tight_ID = "HNL_ULID_CFVL_BDTGv2Pt_MD3";
    if (s_setup=="MVAULNCF21")  param.Electron_Tight_ID = "HNL_ULID_CFT_BDTGv2_MD3";
    if (s_setup=="MVAULNCF22")  param.Electron_Tight_ID = "HNL_ULID_CFM_BDTGv2_MD3";
    if (s_setup=="MVAULNCF23")  param.Electron_Tight_ID = "HNL_ULID_CFL_BDTGv2_MD3";
    if (s_setup=="MVAULNCF24")  param.Electron_Tight_ID = "HNL_ULID_CFVL_BDTGv2_MD3";
    if (s_setup=="MVAULNCF31")  param.Electron_Tight_ID = "HNL_ULID_CFT_BDTGv3_MD3";
    if (s_setup=="MVAULNCF32")  param.Electron_Tight_ID = "HNL_ULID_CFM_BDTGv3_MD3";
    if (s_setup=="MVAULNCF33")  param.Electron_Tight_ID = "HNL_ULID_CFL_BDTGv3_MD3";
    if (s_setup=="MVAULNCF34")  param.Electron_Tight_ID = "HNL_ULID_CFVL_BDTGv3_MD3";

    param.Electron_ID_SF_Key = "TmpHNTightV2";
    param.Muon_ID_SF_Key = "TmpHNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    return param;
  }
  else if (s_setup=="BDTTop"){
    param.MuFakeMethod = "MC";
    param.ElFakeMethod = "MC";
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "MVAID";
    param.Electron_Tight_ID = "TopMVAID";
    param.Electron_ID_SF_Key = "TmpHNTightV2";
    param.Muon_ID_SF_Key = "TmpHNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    return param;
  }
  else if (s_setup=="EXO17028"){
    param.CFMethod   = "MC";
    param.Muon_Tight_ID = "HNTight_17028";
    param.Electron_Tight_ID = "HNTight_17028";
    param.Electron_ID_SF_Key = "TmpHNTight_17028";
    param.Muon_ID_SF_Key = "TmpHNTight_17028";
    param.Muon_FR_ID = "HNLoose_17028";
    param.Electron_FR_ID = "HNLoose_17028";
    return param;
  }
  else if (s_setup=="POG"){
    param.CFMethod   = "MC";
    param.MuFakeMethod = "MC";
    param.ElFakeMethod = "MC";
    param.Electron_ID_SF_Key = "passTightID";
    param.Electron_Tight_ID = "passPOGTight";
    param.Muon_ID_SF_Key = "NUM_TightID_DEN_TrackerMuons";
    param.Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";
    param.Muon_Tight_ID = "POGTightWithTightIso";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    TString trigKey=TrigList_POG_Mu[0];
    trigKey=trigKey.ReplaceAll("HLT_","");
    trigKey=trigKey.ReplaceAll("_v","");
    param.Muon_Trigger_SF_Key = "POGTight";
    param.Muon_Trigger_NameForSF = trigKey;
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLoosePOG";
    return param;
  }
  else if (s_setup=="POGCR"){
    param.CFMethod   = "MC";
    param.MuFakeMethod = "Data";
    param.ElFakeMethod = "Data";
    param.Electron_ID_SF_Key = "passTightID";
    param.Electron_Tight_ID = "passPOGTight";
    param.Muon_ID_SF_Key = "NUM_TightID_DEN_TrackerMuons";
    param.Muon_ISO_SF_Key = "NUM_TightRelIso_DEN_TightIDandIPCut";
    param.Muon_Tight_ID = "POGTightWithTightIso";
    param.Muon_RECO_SF_Key = "MuonRecoSF";
    param.Muon_Trigger_SF_Key = "Default";
    param.Muon_Trigger_NameForSF = "Default";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLoosePOG";
    return param;
  }

  cout << "[HNL_LeptonCore::InitialiseHNLParameters ] ID not found.." << endl;
  exit(EXIT_FAILURE);
  
  
}

double HNL_LeptonCore::HNLZvtxSF(HNL_LeptonCore::Channel ch){
  // See https://indico.cern.ch/event/697573/contributions/2887077/attachments/1596611/2530584/hltZVtxInefficiency.pdf

  if(ch == MuMu) return 1.;
  
  if(IsData) return 1.;
  if(DataYear==2016) return 1.;
  if(DataYear==2018) return 1.;
  if(DataYear==2017) return 0.991;

  return 1.;
}

AnalyzerParameter HNL_LeptonCore::InitialiseHNLParameters( TString param_name, vector<vector<TString> >  hnl_run_param){

  AnalyzerParameter param  ;
  param.Clear();

  param.syst_ = AnalyzerParameter::Central;
  param.Name  = param_name;
  param.DefName  = param_name;

  param.MCCorrrectionIgnoreNoHist = false;

  
  //==== Muon ID                                                                                                                                                                                                                                                                              
  param.Muon_Tight_ID             = hnl_run_param[1][1];
  param.Muon_Loose_ID             = hnl_run_param[1][2];
  param.Muon_Veto_ID              = hnl_run_param[1][3];
  param.Muon_FR_ID                = hnl_run_param[1][2];
  param.Muon_UsePtCone            = false;


  param.Muon_ID_SF_Key             = hnl_run_param[3][1];
  param.Muon_ISO_SF_Key            = hnl_run_param[3][2];
  param.Muon_Trigger_SF_Key        = hnl_run_param[3][3];
  param.Muon_Trigger_NameForSF     = hnl_run_param[3][4];
  param.Muon_RECO_SF_Key           = hnl_run_param[3][5];
  param.Muon_FR_Key                = hnl_run_param[3][6];
  //==== Electron ID                                                                                                                                                                                                                                                                          
  param.Electron_Tight_ID          = hnl_run_param[0][1];
  param.Electron_Loose_ID          = hnl_run_param[0][2];
  param.Electron_Veto_ID           = hnl_run_param[0][3];
  param.Electron_FR_ID             = hnl_run_param[0][2];

  param.Electron_ID_SF_Key         = hnl_run_param[2][1];
  param.Electron_Trigger_SF_Key    = hnl_run_param[2][2];
  param.Electron_Trigger_NameForSF = hnl_run_param[2][3];
  param.Electron_FR_Key            = hnl_run_param[2][4];

  param.Electron_UsePtCone         = false;

  //==== Jet ID                                                                                                                                                                                                                                                                               
  param.Jet_ID                     = hnl_run_param[4][1];
  param.FatJet_ID                  = hnl_run_param[4][2];
  param.BJet_Method                = "2a";

  return param;
}



  
void HNL_LeptonCore::PrintParam(AnalyzerParameter param){
  
  cout << "------------------------------------------------------------" << endl;
  cout << "------------------------------------------------------------" << endl;
  cout << "param name = " << param.Name << endl;
  cout << "param Default name = " << param.DefName << endl;
  cout << "param.MuFakeMethod = " << param.MuFakeMethod << endl;
  cout << "param.ElFakeMethod = " << param.ElFakeMethod << endl;
  cout << "param.CFMethod   = " << param.CFMethod << endl;
  cout << "param.ConvMethod = " << param.ConvMethod << endl;


  cout << "Electron_Tight_ID = " << param.Electron_Tight_ID << endl;
  cout << "Electron_Loose_ID = " << param.Electron_Loose_ID << endl;
  cout << "Electron_Veto_ID = " << param.Electron_Veto_ID << endl;
  cout << "Electron_ID_SF_Key = " << param.Electron_ID_SF_Key << endl;
  cout << "Electron_Trigger_SF_Key = " << param.Electron_Trigger_SF_Key << endl;
  cout << "Electron_Trigger_NameForSF = " << param.Electron_Trigger_NameForSF << endl;
  cout << "Electron_FR_ID = " << param.Electron_FR_ID << endl;
  cout << "Electron_FR_Key = " << param.Electron_FR_Key << endl;

  cout << "Muon_Tight_ID = " << param.Muon_Tight_ID << endl;
  cout << "Muon_Loose_ID = " << param.Muon_Loose_ID << endl;
  cout << "Muon_Veto_ID = " << param.Muon_Veto_ID << endl;
  cout << "Muon_RECO_SF_Key = " << param.Muon_RECO_SF_Key << endl;
  cout << "Muon_ID_SF_Key = " << param.Muon_ID_SF_Key << endl;
  cout << "Muon_ISO_SF_Key = " << param.Muon_ISO_SF_Key << endl;
  cout << "Muon_Trigger_SF_Key = " << param.Muon_Trigger_SF_Key << endl;
  cout << "Muon_Trigger_NameForSF = " << param.Muon_Trigger_NameForSF << endl;
  cout << "Muon_FR_ID = " << param.Muon_FR_ID << endl;
  cout << "Muon_FR_Key = " << param.Muon_FR_Key << endl;
  cout << "Jet_ID = " << param.Jet_ID << endl;
  cout << "FatJet_ID = " << param.FatJet_ID << endl;
  
  return;
  
}

double HNL_LeptonCore::SetupWeight(Event ev, AnalyzerParameter param){

  //=== Apply MC weight                                                                                                                                                                                    
  if(IsDATA) return 1.;

  double prefire_weight = 1.;
  
  if(param.syst_ == AnalyzerParameter::PrefireUp) prefire_weight = GetPrefireWeight(1);
  else if(param.syst_ == AnalyzerParameter::PrefireDown)  prefire_weight = GetPrefireWeight(-1);
  else  prefire_weight = GetPrefireWeight(0);


  double pileup_weight(1.);
  if(param.syst_ == AnalyzerParameter::PUUp) pileup_weight= GetPileUpWeight(nPileUp,1);
  else if(param.syst_ == AnalyzerParameter::PUDown) pileup_weight= GetPileUpWeight(nPileUp,-1);
  else pileup_weight= GetPileUpWeight(nPileUp,0);
  
  double this_mc_weight = ev.GetTriggerLumi("Full") * MCweight(true, true) * pileup_weight * GetKFactor()*prefire_weight;
  //if(MCSample.Contains("Type") && Analyzer == "HNL_SignalLeptonOpt") this_mc_weight = MCweight(true, false) * pileup_weight * prefire_weight;


  FillWeightHist("PrefireWeight_" ,GetPrefireWeight(0));
  FillWeightHist("MCWeight_" ,MCweight(true, true));
  FillWeightHist("MCFullWeight_" , this_mc_weight);
  FillWeightHist("PileupWeight_" , GetPileUpWeight(nPileUp,0));

  double w_topptrw(1.);
  if(MCSample.Contains("TT") and MCSample.Contains("powheg")) {
    w_topptrw = mcCorr->GetTopPtReweight(GetGens()); 
    FillWeightHist("TopPtWeight_" , w_topptrw);
  }
  
  //if(MCSample.Contains("DYJets")) {
  //  double dyRW =   mcCorr->GetOfficialDYReweight(GetGens(),0);
  //  FillWeightHist("DYReWeight_" , dyRW);
  //  this_mc_weight= this_mc_weight*dyRW;
  // }
  

  return this_mc_weight;
  
}




bool HNL_LeptonCore::CheckLeptonFlavourForChannel(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps){
  
  int n_el(0);
  int n_mu(0);
  for(auto ilep : leps)  {
    if(ilep->LeptonFlavour() == Lepton::MUON) n_mu++;
    if(ilep->LeptonFlavour() == Lepton::ELECTRON) n_el++;
  }
  
  if(channel==MuMu  || channel==EE || channel== EMu  ){
    
    if (leps.size() != 2) return false;

    if (channel==EE     && !(leps[0]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->LeptonFlavour() == Lepton::ELECTRON)) return false;
    if (channel==MuMu   && !(leps[0]->LeptonFlavour() == Lepton::MUON     && leps[1]->LeptonFlavour() == Lepton::MUON))    return false;
    if (channel==EMu  &&
  !( (leps[0]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->LeptonFlavour() == Lepton::MUON) ||
     (leps[0]->LeptonFlavour() == Lepton::MUON && leps[1]->LeptonFlavour() == Lepton::ELECTRON) ))  return false;
  
    double lep1_ptcut= (channel==EE) ?   25. : 20.;
    double lep2_ptcut= (channel==EE) ?   10. : 5.;
    //
    if(!(leps[0]->Pt() > lep1_ptcut && leps[1]->Pt()  > lep2_ptcut)) return false;
    
    return true;

  }

  
  if(channel==MuMuMu  || channel==EEE || channel==EMuL ){
    
    if( leps.size() != 3) return false;
    if(channel==MuMuMu && n_mu != 3) return false;
    if(channel==EEE && n_el != 3) return false;
    if(channel==EMuL&&  (n_el == 3  || n_mu == 3)) return false;
    
    
    double lep1_ptcut= (channel==MuMuMu) ?   20.  : 25.;
    double lep2_ptcut= (channel==MuMuMu) ?   10   : 10.;
    double lep3_ptcut= (channel==MuMuMu) ?   10.  : 10.;

    if(!(leps[0]->Pt() > lep1_ptcut && leps[1]->Pt()  > lep2_ptcut && leps[2]->Pt()  > lep3_ptcut)) return false;

    return true;
  }

  if(channel == MuMuMuMu || channel == EEEE || channel == EMuLL ) {

    if( leps.size() != 4) return false;
    if( channel==MuMuMuMu && n_mu != 4) return false;
    if( channel==EEEE && n_el != 4) return false;
    if( channel == EMuLL && (n_mu == 0 || n_mu == 4)) return false;
    
    double lep1_ptcut= (channel==MuMuMuMu) ?   20. : 25.;
    double lep2_ptcut= (channel==MuMuMuMu) ?   10. : 10.;
    double lep3_ptcut= (channel==MuMuMuMu) ?   10. : 10.;
    double lep4_ptcut= (channel==MuMuMuMu) ?   10. : 10.;

    if(!(leps[0]->Pt() > lep1_ptcut && leps[1]->Pt()  > lep2_ptcut && leps[2]->Pt()  > lep3_ptcut && leps[3]->Pt()  > lep4_ptcut)) return false;

    return true;
  }

  return true;

}

double HNL_LeptonCore::GetPtCutTrigger(TString trigname, int nlep, TString flavour){

  if(flavour == "Muon"){
    if(trigname.Contains("HLT_Mu17") ||  trigname.Contains("HLT_TkMu17")) {
      if(trigname.Contains("Mu8") || trigname.Contains("TkMu8")) {
  if(nlep==0) return 20;
  if(nlep==1) return 10;
  if(nlep>1) return 5.;
      }  
    }
    else if(trigname.Contains("HLT_Mu23") ||  trigname.Contains("HLT_Mu23")) {
      if(trigname.Contains("Mu8") || trigname.Contains("TkMu8")) {
  if(nlep==0) return 20;
  if(nlep==1) return 10;
  if(nlep>1) return 5.;
      }
    }
    
    else if(trigname.Contains("HLT_Mu50") && trigname.Contains("HLT_TkMu50")) {
      if(nlep==0) return 55;
      if(nlep>0) return 5.;
    }
    
    else if(trigname.Contains("HLT_Iso") && trigname.Contains("Mu24")) {
      if(nlep==0) return 26;
      if(nlep>0) return 5.;
    }
    
    else if(trigname.Contains("HLT_Iso") && trigname.Contains("Mu27")) {
      if(nlep==0) return 30;
      if(nlep>0) return 5.;
    }

  }
  if(flavour =="Electron"){
    
    if(trigname.Contains("HLT_Ele23_Ele12")){
      if(nlep==0) return 25;
      if(nlep==1) return 15.;
      if(nlep>1) return 10.;
    }
    
    else if(trigname.Contains("HLT_Ele27_WPTight")){
      if(nlep==0) return 30;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_Ele25_eta2p1")){
      if(nlep==0) return 27;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_Ele115")){
      if(nlep==0) return 120;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_Photon175")){
      if(nlep==0) return 180;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_Ele45_CaloIdVT")){
      if(nlep==0) return 50;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_Ele50_CaloIdVT")){
      if(nlep==0) return 55;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_DoubleEle33")){
      if(nlep==0) return 35;
      if(nlep==1) return 35;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_DoubleEle37_Ele27")){
      if(nlep==0) return 40;
      if(nlep==1) return 30;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_DoublePhoton60")){
      if(nlep==0) return 65;
      if(nlep>0) return 10.;
    }
  }
  
  if(flavour =="Muon" || flavour =="Electron"){
    
    if(trigname.Contains("HLT_Mu8_TrkIsoVVL_Ele23")){
      if(flavour =="Electron" && nlep==0) return 25;
      if(flavour =="Muon"&& nlep==0) return 10;
      if(nlep>0) return 10.;
    }
    
    else if(trigname.Contains("HLT_Mu23_TrkIsoVVL_Ele8")){
      if(flavour =="Electron" && nlep==0) return 10;
      if(flavour =="Muon"&& nlep==0) return 25;
      if(nlep>0) return 10.;

    }
    else if(trigname.Contains("HLT_Mu23_TrkIsoVVL_Ele12")){
      if(flavour =="Electron" && nlep==0) return 15;
      if(flavour =="Muon"&& nlep==0) return 25;
      if(nlep>0) return 10.;
    }
    
  }

  if(flavour =="Electron"){
    
    if(trigname.Contains("HLT_Ele27_")){
      if(nlep==0) return 30;
      if(nlep>0) return 10.;
    }
    else if(trigname.Contains("HLT_Ele32_")){
      if(nlep==0) return 35;
      if(nlep>0) return 10.;
    }
    else if(trigname.Contains("HLT_Ele35")){
      if(nlep==0) return 37;
      if(nlep>0) return 10.;
    }
    else if(trigname.Contains("HLT_Photon200")){
      if(nlep==0) return 210;
      if(nlep>0) return 10.;
    }
    else if(trigname.Contains("HLT_DoublePhoton70")){
      if(nlep==0) return 80;
      if(nlep==1) return 80;
      if(nlep>1) return 10.;
    }
    
    else if(trigname.Contains("HLT_DoubleEle25")){
      if(nlep==0) return 28;
      if(nlep==1) return 28;
      if(nlep>1) return 10.;
    }
  }
  return 0.;
}
bool HNL_LeptonCore::PassPtTrigger(Event ev, vector<TString> triglist,std::vector<Lepton *> leps, bool check_pd){

  /// Code to check if event passes trigger/ Trigger is in PD running on for DAta and pt cuts are applied
  

  for(auto itrig : triglist){

    bool pass_trig_pt=true;    
    if(check_pd){
      if(IsData&&!ev.IsPDForTrigger(itrig, this->DataStream)) continue;
    }

    int nel(0);
    int nmu(0);
    for(unsigned int ilep = 0 ; ilep < leps.size(); ilep++){
      
      if(leps[ilep]->LeptonFlavour() == Lepton::ELECTRON) nel++;
      else nmu++;

      double lep_ptcut = (leps[ilep]->LeptonFlavour() == Lepton::ELECTRON) ? GetPtCutTrigger(itrig, nel-1, "Electron") : GetPtCutTrigger(itrig, nmu-1, "Muon") ;
      
      if(leps[ilep]->Pt() < lep_ptcut) pass_trig_pt=false;

    }
    if(pass_trig_pt) return true;
  }

  return false;
}

bool HNL_LeptonCore::CheckSRStream(Event ev,HNL_LeptonCore::Channel channel_ID){

  if(!IsData) return true;
  //  if(channel_ID==MuMu) return CheckStream(ev, TrigList_HNL_DblMu);
  //if(channel_ID==EE) return CheckStream(ev, TrigList_HNL_DblEG);
  //if(channel_ID==EMu) return CheckStream(ev, TrigList_HNL_MuEG);
  
  return true;
}

bool HNL_LeptonCore::PassTriggerAndCheckStream(bool apply_ptcut,vector<Lepton*> leps, Event ev, vector<TString> triglist, bool check_data_pd){

  bool trig_passed=false;
  for(auto itrig : triglist){
    
    if(!IsData) {

      if(ev.PassTrigger(itrig))  {
  trig_passed=true;

  if(apply_ptcut  && !PassPtTrigger(ev, triglist, leps)) trig_passed=false;
  if(trig_passed) return true;
      }
    }
    else if(check_data_pd){
      if(ev.PassTrigger(itrig) && ev.IsPDForTrigger(itrig, this->DataStream)) {
  trig_passed=true;
  if(apply_ptcut  && !PassPtTrigger(ev, triglist, leps,check_data_pd)) trig_passed=false;
  if(trig_passed) return true;
      }
    }
    else if(ev.PassTrigger(itrig)){
      trig_passed=true;
      if(apply_ptcut  && !PassPtTrigger(ev, triglist, leps,check_data_pd)) trig_passed=false;
      if(trig_passed) return true;
    }
  }
  
  return trig_passed;
}



TString HNL_LeptonCore::DoubleToString(double d){


  std::string str = std::to_string (d);
  str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
  str.erase ( str.find_last_not_of('.') + 1, std::string::npos );

  TString ts_str = TString(str);
  ts_str = ts_str.ReplaceAll(".","p");
  ts_str = ts_str.ReplaceAll("-","neg");
  
  return ts_str;

}

TString HNL_LeptonCore::MuonEGPD(){

  /// --- return PD name for EE channel data                                                                                                 
  return "MuonEG";
  return "";


}



TString HNL_LeptonCore::DoubleElectronPD(){

  /// --- return PD name for EE channel data                                                                                                 
  if(DataYear==2016) return "DoubleEG";
  if(DataYear==2017) return "SingleElectron";
  if(DataYear==2018) return "EGamma";

  return "";


}


TString HNL_LeptonCore::SingleElectronPD(){

  /// --- return PD name for EE channel data
  if(DataYear==2016) return "SingleElectron";
  if(DataYear==2017) return "SingleElectron";
  if(DataYear==2018) return "EGamma";

  return "";
  

}

TString HNL_LeptonCore::DoubleMuonPD(){

  /// --- return PD name for EE channel data                                                                                                                                      
  if(DataYear==2016) return "DoubleMuon";
  if(DataYear==2017) return "DoubleMuon";
  if(DataYear==2018) return "DoubleMuon";

  return "";

}

HNL_LeptonCore::HNL_LeptonCore(){
      
  rand_ = new TRandom3(1234);

  RunFake = false;
  RunCF= false;
  IsCentral=true;
  RunFullAnalysis=true;
  run_Debug=false;

}
 
HNL_LeptonCore::~HNL_LeptonCore(){
  
  cout << "Cutflow results" << endl;
  for(std::map< TString, double >::iterator mapit = cfmap.begin(); mapit!=cfmap.end(); mapit++){
    cout << "Cutflow key = " <<  mapit->first << " = " << mapit->second << endl;
  }
  cfmap.clear();

  delete rand_;
}


Particle HNL_LeptonCore::GetSignalObject(TString obj, TString Sig){
  
  if(Sig=="DY"){

    if(gens.size()==0)   gens = GetGens();

    int N_Mother_ind(-1); // Index of N                                                                                                                                                                                                                                
    int W2_ind(0); // Index of W2 : i.e W from N decay                                                                                                                

    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if(fabs(gen.PID()) == 24 && (gens.at(gen.MotherIndex()).PID() == 9900012 || gens.at(gen.MotherIndex()).PID() == 9900014)){
  W2_ind= i;
  for(unsigned int i2=2; i2<gens.size(); i2++){
    Gen gen2 = gens.at(i2);
    if(gen2.MotherIndex() == W2_ind){
      if (fabs(gen2.PID()) == 24 ) W2_ind = i2;
    }
  }
      }

      if((gen.PID() == 9900012 || gen.PID() == 9900014) &&  gen.Status() == 22) {
  N_Mother_ind= gen.MotherIndex();
      }
    }
    Gen LepFromN;
    Gen LepFromW;
    Gen N;
    Gen W2;
    Gen j1,j2;
    bool j1IsSet(false);

    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);

      if(int(gen.MotherIndex()) == W2_ind && gen.Status() == 23) {
  W2 = gens.at(gen.MotherIndex());
  if(!j1IsSet) {  j1= gen; j1IsSet=true;}
  else j2 = gen;
      }
      if(gen.PID() == 9900012 || gen.PID() == 9900014){
  N= gens.at(i);
      }
      if( ! ( ( fabs(gen.PID()) == 13)  || (fabs(gen.PID()) == 11) )) continue;

      TString LepFl = (fabs(gen.PID()) == 13) ? "Mu" : "El";

      if(gens.at(gen.MotherIndex()).PID() == 9900012|| gens.at(gen.MotherIndex()).PID() == 9900014) {
  LepFromN = gen;
      }
      else if(gen.MotherIndex() == N_Mother_ind){
  LepFromW=gen;
      }
    }

    if(obj=="W2") return W2;
    if(obj=="j1") return j1;
    if(obj=="j2") return j2;
    if(obj=="LepFromN") return LepFromN;
    if(obj=="LepFromW") return LepFromW;
    if(obj== "N") return N;
  }
  Particle NullPtc;
  return NullPtc;
}

void HNL_LeptonCore::Fill_SigRegionPlots4(HNL_LeptonCore::Channel channel, TString label, std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Lepton *> leps, Particle  met, double nvtx,  double w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return;

  double HT(0.);
  for(auto ij : jets) HT += ij.Pt();
  
  //double ST += lep1.Pt() + lep2.Pt() + met.Pt();

  int NJet = jets.size();
  int bin = 0;
  double mass_lll = (*leps[0]+ *leps[1]+ *leps[2]).M();
  double LToHT=HT/(leps[0]->Pt()+leps[1]->Pt()+leps[2]->Pt());

  if(LToHT < 10.){
    if(mass_lll < 50){
      if(NJet<=0) bin=1;
      else bin=2;
    }
    else{
      if(NJet<=0) bin=3;
      else bin=4;
    }
  }
  else {
    if(mass_lll < 50){
      if(NJet<=0) bin=5;
      else bin=6;
    }
    else{
      if(NJet<=0) bin=7;
      else bin=8;
    }

  }
  FillHist( label + "/LimitInput_HNL_SR4/signalbin_"+label+"_HNL_SR4",  bin,  w, 8, 0.,8., "SRs");


}




// https://lathomas.web.cern.ch/lathomas/METStuff/XYCorrections/XYMETCorrection_withUL17andUL18andUL16.h

std::pair<double,double> HNL_LeptonCore::METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL ,bool ispuppi){

  std::pair<double,double>  TheXYCorr_Met_MetPhi(uncormet,uncormet_phi);
  
  if(npv>100) npv=100;
  int runera =-1;
  bool usemetv2 =false;
  if(isMC && year == "2016" && !isUL) runera = y2016MC;
  else if(isMC && year == "2017" && !isUL) {runera = y2017MC; usemetv2 =true;}
  else if(isMC && year == "2018" && !isUL) runera = y2018MC;
  else if(isMC && year == "2016APV" && isUL) runera = yUL2016MCAPV;
  else if(isMC && year == "2016nonAPV" && isUL) runera = yUL2016MCnonAPV;
  else if(isMC && year == "2017" && isUL) runera = yUL2017MC;
  else if(isMC && year == "2018" && isUL) runera = yUL2018MC;
  
  
  else if(!isMC && runnb >=272007 && runnb <=275376 && !isUL) runera = y2016B;
  else if(!isMC && runnb >=275657 && runnb <=276283 && !isUL) runera = y2016C;
  else if(!isMC && runnb >=276315 && runnb <=276811 && !isUL) runera = y2016D;
  else if(!isMC && runnb >=276831 && runnb <=277420 && !isUL) runera = y2016E;
  else if(!isMC && runnb >=277772 && runnb <=278808 && !isUL) runera = y2016F;
  else if(!isMC && runnb >=278820 && runnb <=280385 && !isUL) runera = y2016G;
  else if(!isMC && runnb >=280919 && runnb <=284044 && !isUL) runera = y2016H;
  
  else if(!isMC && runnb >=297020 && runnb <=299329 && !isUL){ runera = y2017B; usemetv2 =true;}
  else if(!isMC && runnb >=299337 && runnb <=302029 && !isUL){ runera = y2017C; usemetv2 =true;}
  else if(!isMC && runnb >=302030 && runnb <=303434 && !isUL){ runera = y2017D; usemetv2 =true;}
  else if(!isMC && runnb >=303435 && runnb <=304826 && !isUL){ runera = y2017E; usemetv2 =true;}
  else if(!isMC && runnb >=304911 && runnb <=306462 && !isUL){ runera = y2017F; usemetv2 =true;}
  
  else if(!isMC && runnb >=315252 && runnb <=316995 && !isUL) runera = y2018A;
  else if(!isMC && runnb >=316998 && runnb <=319312 && !isUL) runera = y2018B;
  else if(!isMC && runnb >=319313 && runnb <=320393 && !isUL) runera = y2018C;
  else if(!isMC && runnb >=320394 && runnb <=325273 && !isUL) runera = y2018D;

  else if(!isMC && runnb >=315252 && runnb <=316995 && isUL) runera = yUL2018A;
  else if(!isMC && runnb >=316998 && runnb <=319312 && isUL) runera = yUL2018B;
  else if(!isMC && runnb >=319313 && runnb <=320393 && isUL) runera = yUL2018C;
  else if(!isMC && runnb >=320394 && runnb <=325273 && isUL) runera = yUL2018D;

  else if(!isMC && runnb >=297020 && runnb <=299329 && isUL){ runera = yUL2017B; usemetv2 =false;}
  else if(!isMC && runnb >=299337 && runnb <=302029 && isUL){ runera = yUL2017C; usemetv2 =false;}
  else if(!isMC && runnb >=302030 && runnb <=303434 && isUL){ runera = yUL2017D; usemetv2 =false;}
  else if(!isMC && runnb >=303435 && runnb <=304826 && isUL){ runera = yUL2017E; usemetv2 =false;}
  else if(!isMC && runnb >=304911 && runnb <=306462 && isUL){ runera = yUL2017F; usemetv2 =false;}

  else if(!isMC && runnb >=272007 && runnb <=275376 && isUL) runera = yUL2016B;
  else if(!isMC && runnb >=275657 && runnb <=276283 && isUL) runera = yUL2016C;
  else if(!isMC && runnb >=276315 && runnb <=276811 && isUL) runera = yUL2016D;
  else if(!isMC && runnb >=276831 && runnb <=277420 && isUL) runera = yUL2016E;
  else if(!isMC && ((runnb >=277772 && runnb <=278768) || runnb==278770) && isUL) runera = yUL2016F;
  else if(!isMC && ((runnb >=278801 && runnb <=278808) || runnb==278769) && isUL) runera = yUL2016Flate;
  else if(!isMC && runnb >=278820 && runnb <=280385 && isUL) runera = yUL2016G;
  else if(!isMC && runnb >=280919 && runnb <=284044 && isUL) runera = yUL2016H;


  else {
    //Couldn't find data/MC era => no correction applied
    return TheXYCorr_Met_MetPhi;
  }
  
  double METxcorr(0.),METycorr(0.);

  if(!usemetv2){//Current recommendation for 2016 and 2018
    if(!ispuppi){
      if(runera==y2016B) METxcorr = -(-0.0478335*npv -0.108032);
      if(runera==y2016B) METycorr = -(0.125148*npv +0.355672);
      if(runera==y2016C) METxcorr = -(-0.0916985*npv +0.393247);
      if(runera==y2016C) METycorr = -(0.151445*npv +0.114491);
      if(runera==y2016D) METxcorr = -(-0.0581169*npv +0.567316);
      if(runera==y2016D) METycorr = -(0.147549*npv +0.403088);
      if(runera==y2016E) METxcorr = -(-0.065622*npv +0.536856);
      if(runera==y2016E) METycorr = -(0.188532*npv +0.495346);
      if(runera==y2016F) METxcorr = -(-0.0313322*npv +0.39866);
      if(runera==y2016F) METycorr = -(0.16081*npv +0.960177);
      if(runera==y2016G) METxcorr = -(0.040803*npv -0.290384);
      if(runera==y2016G) METycorr = -(0.0961935*npv +0.666096);
      if(runera==y2016H) METxcorr = -(0.0330868*npv -0.209534);
      if(runera==y2016H) METycorr = -(0.141513*npv +0.816732);
      if(runera==y2017B) METxcorr = -(-0.259456*npv +1.95372);
      if(runera==y2017B) METycorr = -(0.353928*npv -2.46685);
      if(runera==y2017C) METxcorr = -(-0.232763*npv +1.08318);
      if(runera==y2017C) METycorr = -(0.257719*npv -1.1745);
      if(runera==y2017D) METxcorr = -(-0.238067*npv +1.80541);
      if(runera==y2017D) METycorr = -(0.235989*npv -1.44354);
      if(runera==y2017E) METxcorr = -(-0.212352*npv +1.851);
      if(runera==y2017E) METycorr = -(0.157759*npv -0.478139);
      if(runera==y2017F) METxcorr = -(-0.232733*npv +2.24134);
      if(runera==y2017F) METycorr = -(0.213341*npv +0.684588);
      if(runera==y2018A) METxcorr = -(0.362865*npv -1.94505);
      if(runera==y2018A) METycorr = -(0.0709085*npv -0.307365);
      if(runera==y2018B) METxcorr = -(0.492083*npv -2.93552);
      if(runera==y2018B) METycorr = -(0.17874*npv -0.786844);
      if(runera==y2018C) METxcorr = -(0.521349*npv -1.44544);
      if(runera==y2018C) METycorr = -(0.118956*npv -1.96434);
      if(runera==y2018D) METxcorr = -(0.531151*npv -1.37568);
      if(runera==y2018D) METycorr = -(0.0884639*npv -1.57089);
      if(runera==y2016MC) METxcorr = -(-0.195191*npv -0.170948);
      if(runera==y2016MC) METycorr = -(-0.0311891*npv +0.787627);
      if(runera==y2017MC) METxcorr = -(-0.217714*npv +0.493361);
      if(runera==y2017MC) METycorr = -(0.177058*npv -0.336648);
      if(runera==y2018MC) METxcorr = -(0.296713*npv -0.141506);
      if(runera==y2018MC) METycorr = -(0.115685*npv +0.0128193);
    
      //UL2017
      if(runera==yUL2017B) METxcorr = -(-0.211161*npv +0.419333);
      if(runera==yUL2017B) METycorr = -(0.251789*npv +-1.28089);
      if(runera==yUL2017C) METxcorr = -(-0.185184*npv +-0.164009);
      if(runera==yUL2017C) METycorr = -(0.200941*npv +-0.56853);
      if(runera==yUL2017D) METxcorr = -(-0.201606*npv +0.426502);
      if(runera==yUL2017D) METycorr = -(0.188208*npv +-0.58313);
      if(runera==yUL2017E) METxcorr = -(-0.162472*npv +0.176329);
      if(runera==yUL2017E) METycorr = -(0.138076*npv +-0.250239);
      if(runera==yUL2017F) METxcorr = -(-0.210639*npv +0.72934);
      if(runera==yUL2017F) METycorr = -(0.198626*npv +1.028);
      if(runera==yUL2017MC) METxcorr = -(-0.300155*npv +1.90608);
      if(runera==yUL2017MC) METycorr = -(0.300213*npv +-2.02232);

      //UL2018
      if(runera==yUL2018A) METxcorr = -(0.263733*npv +-1.91115);
      if(runera==yUL2018A) METycorr = -(0.0431304*npv +-0.112043);
      if(runera==yUL2018B) METxcorr = -(0.400466*npv +-3.05914);
      if(runera==yUL2018B) METycorr = -(0.146125*npv +-0.533233);
      if(runera==yUL2018C) METxcorr = -(0.430911*npv +-1.42865);
      if(runera==yUL2018C) METycorr = -(0.0620083*npv +-1.46021);
      if(runera==yUL2018D) METxcorr = -(0.457327*npv +-1.56856);
      if(runera==yUL2018D) METycorr = -(0.0684071*npv +-0.928372);
      if(runera==yUL2018MC) METxcorr = -(0.183518*npv +0.546754);
      if(runera==yUL2018MC) METycorr = -(0.192263*npv +-0.42121);

      //UL2016
      if(runera==yUL2016B) METxcorr = -(-0.0214894*npv +-0.188255);
      if(runera==yUL2016B) METycorr = -(0.0876624*npv +0.812885);
      if(runera==yUL2016C) METxcorr = -(-0.032209*npv +0.067288);
      if(runera==yUL2016C) METycorr = -(0.113917*npv +0.743906);
      if(runera==yUL2016D) METxcorr = -(-0.0293663*npv +0.21106);
      if(runera==yUL2016D) METycorr = -(0.11331*npv +0.815787);
      if(runera==yUL2016E) METxcorr = -(-0.0132046*npv +0.20073);
      if(runera==yUL2016E) METycorr = -(0.134809*npv +0.679068);
      if(runera==yUL2016F) METxcorr = -(-0.0543566*npv +0.816597);
      if(runera==yUL2016F) METycorr = -(0.114225*npv +1.17266);
      if(runera==yUL2016Flate) METxcorr = -(0.134616*npv +-0.89965);
      if(runera==yUL2016Flate) METycorr = -(0.0397736*npv +1.0385);
      if(runera==yUL2016G) METxcorr = -(0.121809*npv +-0.584893);
      if(runera==yUL2016G) METycorr = -(0.0558974*npv +0.891234);
      if(runera==yUL2016H) METxcorr = -(0.0868828*npv +-0.703489);
      if(runera==yUL2016H) METycorr = -(0.0888774*npv +0.902632);
      if(runera==yUL2016MCnonAPV) METxcorr = -(-0.153497*npv +-0.231751);
      if(runera==yUL2016MCnonAPV) METycorr = -(0.00731978*npv +0.243323);
      if(runera==yUL2016MCAPV) METxcorr = -(-0.188743*npv +0.136539);
      if(runera==yUL2016MCAPV) METycorr = -(0.0127927*npv +0.117747);



    }
    
    //UL2017Puppi
    if(ispuppi){
      if(runera==yUL2017B) METxcorr = -(-0.00382117*npv +-0.666228);
      if(runera==yUL2017B) METycorr = -(0.0109034*npv +0.172188);
      if(runera==yUL2017C) METxcorr = -(-0.00110699*npv +-0.747643);
      if(runera==yUL2017C) METycorr = -(-0.0012184*npv +0.303817);
      if(runera==yUL2017D) METxcorr = -(-0.00141442*npv +-0.721382);
      if(runera==yUL2017D) METycorr = -(-0.0011873*npv +0.21646);
      if(runera==yUL2017E) METxcorr = -(0.00593859*npv +-0.851999);
      if(runera==yUL2017E) METycorr = -(-0.00754254*npv +0.245956);
      if(runera==yUL2017F) METxcorr = -(0.00765682*npv +-0.945001);
      if(runera==yUL2017F) METycorr = -(-0.0154974*npv +0.804176);
      if(runera==yUL2017MC) METxcorr = -(-0.0102265*npv +-0.446416);
      if(runera==yUL2017MC) METycorr = -(0.0198663*npv +0.243182);

      //UL2018Puppi
      if(runera==yUL2018A) METxcorr = -(-0.0073377*npv +0.0250294);
      if(runera==yUL2018A) METycorr = -(-0.000406059*npv +0.0417346);
      if(runera==yUL2018B) METxcorr = -(0.00434261*npv +0.00892927);
      if(runera==yUL2018B) METycorr = -(0.00234695*npv +0.20381);
      if(runera==yUL2018C) METxcorr = -(0.00198311*npv +0.37026);
      if(runera==yUL2018C) METycorr = -(-0.016127*npv +0.402029);
      if(runera==yUL2018D) METxcorr = -(0.00220647*npv +0.378141);
      if(runera==yUL2018D) METycorr = -(-0.0160244*npv +0.471053);
      if(runera==yUL2018MC) METxcorr = -(-0.0214557*npv +0.969428);
      if(runera==yUL2018MC) METycorr = -(0.0167134*npv +0.199296);

      //UL2016Puppi
      if(runera==yUL2016B) METxcorr = -(-0.00109025*npv +-0.338093);
      if(runera==yUL2016B) METycorr = -(-0.00356058*npv +0.128407);
      if(runera==yUL2016C) METxcorr = -(-0.00271913*npv +-0.342268);
      if(runera==yUL2016C) METycorr = -(0.00187386*npv +0.104);
      if(runera==yUL2016D) METxcorr = -(-0.00254194*npv +-0.305264);
      if(runera==yUL2016D) METycorr = -(-0.00177408*npv +0.164639);
      if(runera==yUL2016E) METxcorr = -(-0.00358835*npv +-0.225435);
      if(runera==yUL2016E) METycorr = -(-0.000444268*npv +0.180479);
      if(runera==yUL2016F) METxcorr = -(0.0056759*npv +-0.454101);
      if(runera==yUL2016F) METycorr = -(-0.00962707*npv +0.35731);
      if(runera==yUL2016Flate) METxcorr = -(0.0234421*npv +-0.371298);
      if(runera==yUL2016Flate) METycorr = -(-0.00997438*npv +0.0809178);
      if(runera==yUL2016G) METxcorr = -(0.0182134*npv +-0.335786);
      if(runera==yUL2016G) METycorr = -(-0.0063338*npv +0.093349);
      if(runera==yUL2016H) METxcorr = -(0.015702*npv +-0.340832);
      if(runera==yUL2016H) METycorr = -(-0.00544957*npv +0.199093);
      if(runera==yUL2016MCnonAPV) METxcorr = -(-0.0058341*npv +-0.395049);
      if(runera==yUL2016MCnonAPV) METycorr = -(0.00971595*npv +-0.101288);
      if(runera==yUL2016MCAPV) METxcorr = -(-0.0060447*npv +-0.4183);
      if(runera==yUL2016MCAPV) METycorr = -(0.008331*npv +-0.0990046);


    }


  }
  else {//these are the corrections for v2 MET recipe (currently recommended for 2017)
    if(runera==y2016B) METxcorr = -(-0.0374977*npv +0.00488262);
    if(runera==y2016B) METycorr = -(0.107373*npv +-0.00732239);
    if(runera==y2016C) METxcorr = -(-0.0832562*npv +0.550742);
    if(runera==y2016C) METycorr = -(0.142469*npv +-0.153718);
    if(runera==y2016D) METxcorr = -(-0.0400931*npv +0.753734);
    if(runera==y2016D) METycorr = -(0.127154*npv +0.0175228);
    if(runera==y2016E) METxcorr = -(-0.0409231*npv +0.755128);
    if(runera==y2016E) METycorr = -(0.168407*npv +0.126755);
    if(runera==y2016F) METxcorr = -(-0.0161259*npv +0.516919);
    if(runera==y2016F) METycorr = -(0.141176*npv +0.544062);
    if(runera==y2016G) METxcorr = -(0.0583851*npv +-0.0987447);
    if(runera==y2016G) METycorr = -(0.0641427*npv +0.319112);
    if(runera==y2016H) METxcorr = -(0.0706267*npv +-0.13118);
    if(runera==y2016H) METycorr = -(0.127481*npv +0.370786);
    if(runera==y2017B) METxcorr = -(-0.19563*npv +1.51859);
    if(runera==y2017B) METycorr = -(0.306987*npv +-1.84713);
    if(runera==y2017C) METxcorr = -(-0.161661*npv +0.589933);
    if(runera==y2017C) METycorr = -(0.233569*npv +-0.995546);
    if(runera==y2017D) METxcorr = -(-0.180911*npv +1.23553);
    if(runera==y2017D) METycorr = -(0.240155*npv +-1.27449);
    if(runera==y2017E) METxcorr = -(-0.149494*npv +0.901305);
    if(runera==y2017E) METycorr = -(0.178212*npv +-0.535537);
    if(runera==y2017F) METxcorr = -(-0.165154*npv +1.02018);
    if(runera==y2017F) METycorr = -(0.253794*npv +0.75776);
    if(runera==y2018A) METxcorr = -(0.362642*npv +-1.55094);
    if(runera==y2018A) METycorr = -(0.0737842*npv +-0.677209);
    if(runera==y2018B) METxcorr = -(0.485614*npv +-2.45706);
    if(runera==y2018B) METycorr = -(0.181619*npv +-1.00636);
    if(runera==y2018C) METxcorr = -(0.503638*npv +-1.01281);
    if(runera==y2018C) METycorr = -(0.147811*npv +-1.48941);
    if(runera==y2018D) METxcorr = -(0.520265*npv +-1.20322);
    if(runera==y2018D) METycorr = -(0.143919*npv +-0.979328);
    if(runera==y2016MC) METxcorr = -(-0.159469*npv +-0.407022);
    if(runera==y2016MC) METycorr = -(-0.0405812*npv +0.570415);
    if(runera==y2017MC) METxcorr = -(-0.182569*npv +0.276542);
    if(runera==y2017MC) METycorr = -(0.155652*npv +-0.417633);
    if(runera==y2018MC) METxcorr = -(0.299448*npv +-0.13866);
    if(runera==y2018MC) METycorr = -(0.118785*npv +0.0889588);




    
  }

  double CorrectedMET_x = uncormet *cos( uncormet_phi)+METxcorr;
  double CorrectedMET_y = uncormet *sin( uncormet_phi)+METycorr;

  double CorrectedMET = sqrt(CorrectedMET_x*CorrectedMET_x+CorrectedMET_y*CorrectedMET_y);
  double CorrectedMETPhi;
  if(CorrectedMET_x==0 && CorrectedMET_y>0) CorrectedMETPhi = TMath::Pi();
  else if(CorrectedMET_x==0 && CorrectedMET_y<0 )CorrectedMETPhi = -TMath::Pi();
  else if(CorrectedMET_x >0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x);
  else if(CorrectedMET_x <0&& CorrectedMET_y>0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) + TMath::Pi();
  else if(CorrectedMET_x <0&& CorrectedMET_y<0) CorrectedMETPhi = TMath::ATan(CorrectedMET_y/CorrectedMET_x) - TMath::Pi();
  else CorrectedMETPhi =0;

  TheXYCorr_Met_MetPhi.first= CorrectedMET;
  TheXYCorr_Met_MetPhi.second= CorrectedMETPhi;
  return TheXYCorr_Met_MetPhi;

}




TString HNL_LeptonCore::GetChannelString(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType q){

  TString channel_string="";
  if (channel == EE) channel_string="EE";
  if (channel == MuMu) channel_string="MuMu";
  if (channel == EMu) channel_string="EMu";

  if (channel == EEE) channel_string="EEE";
  if (channel == EMuL) channel_string="EMuL";
  if (channel == MuMuMu) channel_string="MuMuMu";

  if (channel == EEEE) channel_string="EEEE";
  if (channel == MuMuMuMu) channel_string="MuMuMuMu";
  if (channel == EMuLL) channel_string="EMuLL";


  if (q == Plus) channel_string+="_+";
  else if (q == Minus) channel_string+="_-";
  else   return channel_string;

  return channel_string;
}

TString HNL_LeptonCore::QToString(HNL_LeptonCore::ChargeType q){

  if (q==Plus) return "Plus";
  if (q==Minus) return "Minus";
  else return "";

}

std::vector<FatJet> HNL_LeptonCore::GetHNLAK8Jets(TString JetType, AnalyzerParameter param){

  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID,     5.,  2.4);
  
  std::vector<FatJet>   FatjetColl  = GetFatJets(param, "tight", 200., 5.);
  if(JetType=="Loose") return FatjetColl;

  std::vector<FatJet> AK8_JetColl                  = SelectAK8Jets(FatjetColl, 200., 5., true,  1., false, -999, false, 0., 20000., ElectronCollV, MuonCollV);

  return AK8_JetColl;
}

std::vector<Jet> HNL_LeptonCore::GetHNLJets(TString JetType, AnalyzerParameter param){

  /// Lepotns for cleaninh 
  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID,     5.,  2.4);

  /// AK4 
  std::vector<Jet> AK4_Loose     = GetJets   ( param, param.Jet_ID, 10., 5.);
  std::vector<Jet> AK4_All       = GetJets   ( param, "NoID",      0.,  5.);

  // AK8
  std::vector<FatJet> AK8_JetCollLoose             = GetHNLAK8Jets("Loose", param);

  /// BJET
  std::vector<Jet> BJetCollLoose                   = SelectAK4Jets(AK4_Loose,  15., 2.4, true,  0.4,0.8, "",   ElectronCollV,MuonCollV,  AK8_JetCollLoose);
  std::vector<Jet> BJetCollNoLepClean              = SelectAK4Jets(AK4_Loose,  20., 2.4, false,  0.4,0.8, "",   ElectronCollV,MuonCollV,  AK8_JetCollLoose);
  
  if(JetType=="All")   return AK4_Loose;

  if(JetType=="Loose")    return SelectAK4Jets(AK4_Loose,     15., 4.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, AK8_JetCollLoose);
  if(JetType=="Tight")    return SelectAK4Jets(AK4_Loose,     20., 2.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, AK8_JetCollLoose);
  if(JetType=="VBFTight") return SelectAK4Jets(AK4_Loose,     20., 4.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, AK8_JetCollLoose);

  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  JetTagging::Parameters param_jetsT = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets);
  if(JetType=="BJetM")     return SelectBJets(param, BJetCollLoose, param_jets);
  if(JetType=="BJetT")     return SelectBJets(param, BJetCollLoose, param_jetsT);
  if(JetType=="BJetM_NoLC")     return SelectBJets(param, BJetCollLoose, param_jets);
  if(JetType=="BJetT_NoLC")     return SelectBJets(param, BJetCollLoose, param_jetsT);

  // Else just return Standard Jet coll for HNL
  return SelectAK4Jets(AK4_Loose,     20., 2.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, AK8_JetCollLoose);

}


vector<Gen> HNL_LeptonCore::GetGenLepronsSignal(){

  bool isDYVBF=false;
  vector<Gen> gens= GetGens();
  vector<Gen> gen_lep;
  int N_Mother(0);

  for (auto i : gens){ Gen gen = i; if(gen.PID() == 9900012 || gen.PID() == 9900014) isDYVBF=true; }

  if(isDYVBF){

    for(unsigned int i=2; i<gens.size(); i++){

      Gen gen = gens.at(i);

      if((gen.PID() == 9900012 || gen.PID() == 9900014)  && gen.Status()==22) {
        N_Mother= gen.MotherIndex();
      }
    }

    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);     TString lep_ch="";

      if((gens.at(gen.MotherIndex()).PID() == 9900012|| gens.at(gen.MotherIndex()).PID() == 9900014) && !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){

        if(fabs(gen.PID()) == 15)     gen_lep.push_back(gen);

        if(fabs(gen.PID()) == 13)     gen_lep.push_back(gen);
        if(fabs(gen.PID()) == 11)     gen_lep.push_back(gen);

      }
      else if(gen.MotherIndex() == N_Mother&& !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){


        if(fabs(gen.PID()) == 15) gen_lep.push_back(gen);

        if(fabs(gen.PID()) == 13) gen_lep.push_back(gen);
        if(fabs(gen.PID()) == 11) gen_lep.push_back(gen);
      }
    }
  }


  else{



    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if (fabs(gen.PID()) == 13 && gen.Status() == 23) gen_lep.push_back(gen);
      if (fabs(gen.PID()) == 11 && gen.Status() == 23) gen_lep.push_back(gen);
      if (fabs(gen.PID()) == 15 && gen.Status() == 23)gen_lep.push_back(gen);
    }
  }

  return gen_lep;
}

bool HNL_LeptonCore::SelectChannel(HNL_LeptonCore::Channel channel){

  TString process = GetProcess();
  if(channel == LL   && process.Contains("SS")) return true;
  if(channel == MuMu && process.Contains("SS_Mu+Mu+")) return true;
  if(channel == MuMu && process.Contains("SS_Mu-Mu-")) return true;
  if(channel == EE   && process.Contains("SS_El+El+")) return true;
  if(channel == EE   && process.Contains("SS_El-El-")) return true;
  if(channel == EMu  && process.Contains("SS_El+Mu+")) return true;
  if(channel == EMu  && process.Contains("SS_El-Mu-")) return true;
  if(channel == EMu  && process.Contains("SS_Mu+El+")) return true;
  if(channel == EMu  && process.Contains("SS_Mu-El-")) return true;


  return false;
}




TString HNL_LeptonCore::GetProcess(){

  if (IsData) return "";
  if(!MCSample.Contains("Type")) return "";

  int N_Mother(0);

  //  cout << "index\tPID\tStatus\tMIdx\tMPID\tStart\tPt\tEta\tPhi\tM" << endl;                                                                                                                                          

  bool isDYVBF=false;
  vector<Gen> gens= GetGens();

  for (auto i : gens){ Gen gen = i; if(gen.PID() == 9900012 || gen.PID() == 9900014) isDYVBF=true; }

  if(isDYVBF){

    /*bool isVBF=false;                                                                                                                                                                                                  
                                                                                                                                                                                                                         
    for (auto i : gens){                                                                                                                                                                                                 
      Gen gen = i;                                                                                                                                                                                                       
      if (gen.PID() == 22 && gen.Status() == 21) isVBF=true;                                                                                                                                                             
    }                                                                                                                                                                                                                    
    */





    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);

      if((gen.PID() == 9900012 || gen.PID() == 9900014)  && gen.Status()==22) {
        N_Mother= gen.MotherIndex();
      }
    }

    int lep_1_ch=-999;
    int lep_2_ch=-999;

    TString lep1_s="", lep2_s="";
    TString lep1_ss="", lep2_ss="";

    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);     TString lep_ch="";

      if((gens.at(gen.MotherIndex()).PID() == 9900012|| gens.at(gen.MotherIndex()).PID() == 9900014) && !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){
        //if(fabs(gen.PID())  < 16 && fabs(gen.PID())  > 10) lep_2_ch = (gen.PID() < 0) ? 1 : -1;                                                                                                                        

        if(fabs(gen.PID()) == 15) {
          lep2_s="Tau";
          if (gen.PID() <0) lep2_ss=lep2_s+"+";
          else lep2_ss=lep2_s+"-";
          lep_2_ch = (gen.PID() < 0) ? 1 : -1;
        }
        if(fabs(gen.PID()) == 13) {
          lep2_s="Mu";
          if (gen.PID() <0) lep2_ss=lep2_s+"+";
          else lep2_ss=lep2_s+"-";
          lep_2_ch = (gen.PID() < 0) ? 1 : -1;
        }
        //if(fabs(gen.PID()) == 14) {lep2_s="Mu";lep2_s+=lep_ch;}                                                                                                                                                        
        if(fabs(gen.PID()) == 11) {
          lep2_s="El";
          if (gen.PID() <0) lep2_ss=lep2_s+"+";
          else lep2_ss=lep2_s+"-";
          lep_2_ch = (gen.PID() < 0) ? 1 : -1;
        }
      }
      else if(gen.MotherIndex() == N_Mother&& !(gens.at(i).PID() == 9900012 || gens.at(i).PID() == 9900014)){

        //if(fabs(gen.PID())  < 16 && fabs(gen.PID())  > 10)  lep_1_ch = (gen.PID() < 0) ? 1 : -1;                                                                                                                       


        if(fabs(gen.PID()) == 15) {
          lep1_s="Tau";
          if (gen.PID() < 0) lep1_ss=lep1_s+"+";
          else lep1_ss=lep1_s+"-";
          lep_1_ch = (gen.PID() < 0) ? 1 : -1;
        }
        if(fabs(gen.PID()) == 13) {
          lep1_s="Mu";
          if (gen.PID() < 0) lep1_ss=lep1_s+"+";
          else lep1_ss=lep1_s+"-";
          lep_1_ch = (gen.PID() < 0) ? 1 : -1;
        }
        if(fabs(gen.PID()) == 11) {
          lep1_s="El";
          if (gen.PID() < 0) lep1_ss=lep1_s+"+";
          else lep1_ss=lep1_s+"-";
          lep_1_ch = (gen.PID() < 0) ? 1 : -1;
        }
      }
    }
    if(lep_1_ch == lep_2_ch) return ("SS_"+ lep1_ss+lep2_ss);
    else return ("OS_"+ lep1_s + lep2_s);

  }

  else{


    TString pr="SS_";
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      if (fabs(gen.PID()) == 13 && gen.Status() == 23) {
        pr=pr+"Mu";
        if (gen.PID() < 0) pr = pr+"+";
        else pr = pr+"-";
      }
      if (fabs(gen.PID()) == 11 && gen.Status() == 23) {
        pr=pr+"El";
        if (gen.PID() < 0) pr = pr+"+";
        else pr = pr+"-";
      }
      if (fabs(gen.PID()) == 15 && gen.Status() == 23){
        pr=pr+"Tau";
        if (gen.PID() < 0) pr = pr+"+";
        else pr = pr+"-";
      }
    }
    return pr;
  }
  return "TMP";

}

bool HNL_LeptonCore::HasLowMassMeson(std::vector<Lepton *> leps){

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(fabs(ll.M() - 3.10) < 0.08) return true;
      if(fabs(ll.M() - 3.69) < 0.08) return true;
      if(fabs(ll.M() - 0.78) < 0.08) return true;
      if(fabs(ll.M() - 1.02) < 0.08) return true;
      if(fabs(ll.M() - 3.10) < 0.08) return true;
      if(fabs(ll.M() - 9.46) < 0.08) return true;
      if(fabs(ll.M() - 10.02) < 0.08) return true;
      if(fabs(ll.M() - 10.36) < 0.08) return true;
    }
  }


  return false;

}

TString HNL_LeptonCore::CloseJetFlavour(std::vector<Jet> jetColl, Muon mu){
  
  int IdxMatchJet=-1;
  float mindR1=999.;

  for(unsigned int ij=0; ij<jetColl.size(); ij++){
    float dR1=mu.DeltaR(jetColl.at(ij));
    if(dR1>0.4) continue;
    if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
  }
  
  if(IdxMatchJet < 0)   return "NULL";

  int JetFlavour = jetColl.at(IdxMatchJet).hadronFlavour();
  
  if(JetFlavour == 5) return "HF";
  if(JetFlavour == 4) return "HF";
  if(JetFlavour == 0) return "LF";
  return "NULL";
}

TString HNL_LeptonCore::CloseJetFlavour(std::vector<Jet> jetColl, Electron el){

  int IdxMatchJet=-1;
  float mindR1=999.;

  for(unsigned int ij=0; ij<jetColl.size(); ij++){
    float dR1=el.DeltaR(jetColl.at(ij));
    if(dR1>0.4) continue;
    if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
  }
  if(IdxMatchJet < 0)   return "NULL";

  int JetFlavour = jetColl.at(IdxMatchJet).hadronFlavour();

  if(JetFlavour == 5) return "HF";
  if(JetFlavour == 4) return "HF";
  if(JetFlavour == 0) return "LF";
  return "NULL";
}


TString HNL_LeptonCore::CloseJetFlavour(std::vector<Jet> jetColl, Lepton* lep){

  int IdxMatchJet=-1;
  float mindR1=999.;

  for(unsigned int ij=0; ij<jetColl.size(); ij++){
    float dR1=lep->DeltaR(jetColl.at(ij));
    if(dR1>0.4) continue;
    if(dR1<mindR1){ mindR1=dR1; IdxMatchJet=ij; }
  }
  if(IdxMatchJet < 0)   return "NULL";

  int JetFlavour = jetColl.at(IdxMatchJet).hadronFlavour();

  if(JetFlavour == 5) return "HF";
  if(JetFlavour == 4) return "HF";
  if(JetFlavour == 0) return "LF";
  return "NULL";
}





std::vector<Jet> HNL_LeptonCore::SelBJets(std::vector<Jet>& jetColl, JetTagging::Parameters jtp){

  std::vector<Jet> bjetColl;
  for(unsigned int i=0; i<jetColl.size(); i++){
    if( jetColl.at(i).GetTaggerResult(jtp.j_Tagger) <= mcCorr->GetJetTaggingCutValue(jtp.j_Tagger, jtp.j_WP) ) continue;
    bjetColl.push_back( jetColl.at(i) );
  }
  return bjetColl;
}



Particle HNL_LeptonCore::GetvMET(TString METType){

  bool IsType1   = METType.Contains("T1");
  bool IsxyCorr  = METType.Contains("xyCorr");
  bool UsePuppi  = METType.Contains("Puppi");
  bool IsFixxyCorr  = METType.Contains("xyULCorr");

  //METXYCorr_Met_MetPhi(double uncormet, double uncormet_phi, int runnb, TString year, bool isMC, int npv, bool isUL =false,bool ispuppi=false)r                                                                                                                                                          

  double Met_pt(0.), Met_phi(0.);

  Particle vMET;
  if(UsePuppi){
    if(IsType1){
      if(IsxyCorr) {
        Met_pt=PuppiMET_Type1_PhiCor_pt;
        Met_phi = PuppiMET_Type1_PhiCor_phi;
      }
      else  {
        Met_pt=PuppiMET_Type1_pt;
        Met_phi=PuppiMET_Type1_phi;
      }
    }// T1                                                                                                                                                                                                                                                                                                 
    else{
      Met_pt=PuppiMET_pt;
      Met_phi=PuppiMET_phi;
    }
  } // PUPPI                                                                                                                                                                                                                                                                                               

  else{
    // PRMET                                                                                                                                                                                                                                                                                               
    if(IsType1){
      if(IsxyCorr) {
        Met_pt =pfMET_Type1_PhiCor_pt;
        Met_phi=pfMET_Type1_PhiCor_phi;
      }
      else{
        Met_pt =pfMET_Type1_pt;
        Met_phi =pfMET_Type1_phi;
      }
    }
    else{
      Met_pt=pfMET_pt;
      Met_phi=pfMET_phi;
    }
  }
  if(IsFixxyCorr) {
    TString Year= "2016";
    if(DataYear==2017) Year= "2017";
    if(DataYear==2018) Year= "2018";
    std::pair<double,double> METPair = METXYCorr_Met_MetPhi(Met_pt, Met_phi, run, Year, !IsData, nPV, true,UsePuppi);
    Met_pt=METPair.first;
    Met_phi=METPair.second;
  }

  vMET.SetPtEtaPhiM(Met_pt, 0., Met_phi, 0.);
  return vMET;

}



double HNL_LeptonCore::GetFilterEffType1(TString SigProcess, int mass){

  //https://docs.google.com/spreadsheets/d/1adHrUM0I45-SUuaSzH0dh7fu9usEjJk4C_aH20bLDFA/edit#gid=0                                                                                                                                                                                                         
  if(SigProcess == "DY"){
    if(mass == 85) return 0.417;
    if(mass == 90) return 0.625;
    if(mass == 95) return 0.739;
    if(mass == 100) return 0.856;
    if(mass == 125) return 0.978;
    if(mass == 150) return 0.983;
    if(mass == 200) return 0.994;
    if(mass == 250) return 0.996;
    if(mass == 300) return 0.996;
    if(mass == 400) return 0.998;
    if(mass == 500) return 0.998;
  }

  return -1.;
}

double HNL_LeptonCore::GetXsec(TString SigProcess, int mass){



  return 0.;
}


Particle HNL_LeptonCore::GetvMET(TString METType, AnalyzerParameter param, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Muon> muons, std::vector<Electron> electrons){

  cout << "MET updating..." << endl; //JH
  bool IsType1   = METType.Contains("T1");
  bool IsxyCorr  = METType.Contains("xyCorr");
  bool UsePuppi  = METType.Contains("Puppi");

  int IdxSyst = -1;

  if(param.syst_ == AnalyzerParameter::METUnclUp)   IdxSyst = 10;
  if(param.syst_ == AnalyzerParameter::METUnclDown)   IdxSyst = 11;

  if(param.syst_ == AnalyzerParameter::JetResUp)  IdxSyst = 0;
  if(param.syst_ == AnalyzerParameter::JetResDown)  IdxSyst = 1;
  if(param.syst_ == AnalyzerParameter::JetEnUp)  IdxSyst = 2;
  if(param.syst_ == AnalyzerParameter::JetEnDown)  IdxSyst = 3;
  if(param.syst_ == AnalyzerParameter::MuonEnUp)  IdxSyst = 4;
  if(param.syst_ == AnalyzerParameter::MuonEnDown)  IdxSyst = 5;
  if(param.syst_ == AnalyzerParameter::ElectronEnUp)  IdxSyst = 6;
  if(param.syst_ == AnalyzerParameter::ElectronEnDown)  IdxSyst = 7;

  bool ApplySyst = (!IsDATA) && (param.syst_ != AnalyzerParameter::Central);
  cout << "Apply syst? " << ApplySyst << endl; //JH

  Particle vMET;

  if(UsePuppi){
    if(IsType1){
      if(IsxyCorr) vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.);
      else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt, 0., PuppiMET_Type1_phi, 0.);
    }
  }
  else{ //JH FIXME later
    if(IsType1){
      if( (!ApplySyst) or ( IdxSyst>=0 && (!isfinite(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst))) ) ){
        if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt, 0., pfMET_Type1_PhiCor_phi, 0.);
        else         vMET.SetPtEtaPhiM(pfMET_Type1_pt, 0., pfMET_Type1_phi, 0.);
      }
      else{
        if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst), 0., pfMET_Type1_PhiCor_phi_shifts->at(IdxSyst), 0.);
        else         vMET.SetPtEtaPhiM(pfMET_Type1_pt_shifts->at(IdxSyst), 0., pfMET_Type1_phi_shifts->at(IdxSyst), 0.);
      }
    }
  }

  //if (param.syst_ == AnalyzerParameter::Central) return vMET; //JH : the MET needs to be updated, even if it's Central.

  Particle vMETSyst;
  //if(ApplySyst){
    if(METType.Contains("CMSSW")){
      if(IsxyCorr) vMETSyst.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.); //JH FIXME is this just omitted or not provided?
      else         vMETSyst.SetPtEtaPhiM(PuppiMET_Type1_pt_shifts->at(IdxSyst), 0.,  PuppiMET_Type1_phi_shifts->at(IdxSyst), 0.);
    }
    else vMETSyst = UpdateMETSyst(param, vMET, jets, fatjets, muons, electrons);
  //} //JH

  return vMETSyst;
}

Particle HNL_LeptonCore::GetvMET(TString METType, AnalyzerParameter param){

  bool IsType1   = METType.Contains("T1");
  bool IsxyCorr  = METType.Contains("xyCorr");
  bool UsePuppi  = METType.Contains("Puppi");

  bool IsJetSmear  = METType.Contains("JetSmear");

  int IdxSyst = -1;

  if(param.syst_ == AnalyzerParameter::METUnclUp)   IdxSyst = 10;
  if(param.syst_ == AnalyzerParameter::METUnclDown)   IdxSyst = 11;

  // Use CMSSW MET SYSTa                                                                                                                                                                                                                                                                                   
  if(METType.Contains("CMSSW")){
    if(param.syst_ == AnalyzerParameter::JetEnUp)  IdxSyst = 2;
    if(param.syst_ == AnalyzerParameter::JetEnDown)  IdxSyst = 3;
    if(param.syst_ == AnalyzerParameter::JetResUp)  IdxSyst = 0;
    if(param.syst_ == AnalyzerParameter::JetResDown)  IdxSyst = 1;
  }
  bool ApplySyst = (!IsDATA) && IdxSyst >= 0;

  Particle vMET;

  if(UsePuppi){
    if(IsType1){
      if( (!ApplySyst) ){
        if(IsxyCorr) vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.);
        else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt, 0., PuppiMET_Type1_phi, 0.);
      }
      else{
        if(IsxyCorr)  vMET.SetPtEtaPhiM(PuppiMET_Type1_PhiCor_pt, 0., PuppiMET_Type1_PhiCor_phi, 0.);
        else         vMET.SetPtEtaPhiM(PuppiMET_Type1_pt_shifts->at(IdxSyst), 0.,  PuppiMET_Type1_phi_shifts->at(IdxSyst), 0.);
      }
    }

  }
  else{
    if(IsType1){
      if( (!ApplySyst) or ( IdxSyst>=0 && (!isfinite(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst))) ) ){
        if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt, 0., pfMET_Type1_PhiCor_phi, 0.);
        else         vMET.SetPtEtaPhiM(pfMET_Type1_pt, 0., pfMET_Type1_phi, 0.);
      }
      else{
        if(IsxyCorr) vMET.SetPtEtaPhiM(pfMET_Type1_PhiCor_pt_shifts->at(IdxSyst), 0., pfMET_Type1_PhiCor_phi_shifts->at(IdxSyst), 0.);
        else         vMET.SetPtEtaPhiM(pfMET_Type1_pt_shifts->at(IdxSyst), 0., pfMET_Type1_phi_shifts->at(IdxSyst), 0.);
      }
    }

  }

  if (!IsJetSmear && param.syst_ == AnalyzerParameter::Central) return vMET;

  Particle vMETSmeared;
  vector<Jet> _jets = GetJets(param, param.Jet_ID, 10., 5.);
  if (IsJetSmear) vMETSmeared = UpdateMETSmearedJet(vMET, _jets);

  if(param.syst_ == AnalyzerParameter::Central) return vMETSmeared;
  double MET = vMETSmeared.Pt();
  double METPhi = vMETSmeared.Pt();
  //if(param.syst_ == AnalyzerParameter::JetResUp ) CorrectedMETJER(1, GetJets(param, 10., 5.), GetFatJets(param, 200., 5.), MET,METPhi);                                                                                                                                                                  
  //if(param.syst_ == AnalyzerParameter::JetResDown) CorrectedMETJER(-11, GetJets(param, 10., 5.), GetFatJets(param, 200., 5.), MET,METPhi);                                                                                                                                                               
  // Write CorrectedMETXXhttps://github.com/jedori0228/LQanalyzer/blob/CatAnalyzer_13TeV_v8-0-7.36_HNAnalyzer/LQAnalysis/Analyzers/src/AnalyzerCore.cc#L4826                                                                                                                                               

  Particle vMETSyst;
  vMETSyst.SetPtEtaPhiM(MET, 0., METPhi, 0.);


  return vMETSyst;
}

int HNL_LeptonCore::GetIndexNonMinOSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]) + (*leps[1]);
    Particle ll2 = (*leps[0]) + (*leps[2]);
    Particle ll3 = (*leps[1]) + (*leps[2]);

    double minOS=99999999999.;
    if(ll1.Charge() == 0) {
      if(ll1.M()  < minOS) { minOS=ll1.M(); index=2;}
    }
    if(ll2.Charge() == 0) {
      if(ll2.M()  < minOS) { minOS=ll2.M(); index=1;}
    }
    if(ll3.Charge() == 0) {
      if(ll3.M()  < minOS) { minOS=ll3.M(); index=0;}
    }

  }

  return index;

}




double  HNL_LeptonCore::GetMass(TString type , std::vector<Jet> jets, std::vector<FatJet> fatjets,vector<Lepton*> leps){

  if (type=="HNL_SR3"){

    if(jets.size() < 2) return 0.;
    double dijetmass_tmp=9999.;
    double dijetmass=99990000.;
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

  if (type=="HNL_SR1"){
    if(fatjets.size() ==0 )  return 0.;
    if(leps.size() != 2) return 0.;
    double dijetmass_tmp=999.;
    double dijetmass=9990000;
    int m=-999;
    for(UInt_t emme=0; emme<fatjets.size(); emme++){
      dijetmass_tmp= fatjets[emme].SDMass();
      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
      }
    }
    Particle W = fatjets[m] + *leps[0] + *leps[1];
    return W.M();
  }

  if (type=="HNL_SR2_17028"){
    if(fatjets.size() ==0 )  return 0.;
    if(leps.size() != 2) return 0.;
    double dijetmass_tmp=999.;
    double dijetmass=9990000;
    int m=-999;
    for(UInt_t emme=0; emme<fatjets.size(); emme++){
      dijetmass_tmp= fatjets[emme].SDMass();
      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
      }
    }
    Particle W = fatjets[m];
    return W.M();
  }


  cout << "[HNL_LeptonCore::GetMass ] ID not found.." << endl;
  exit(EXIT_FAILURE);

  return -9999.;
}



vector<Muon> HNL_LeptonCore::GetLepCollByRunType(const std::vector<Muon>& MuColl, vector<Gen>& TruthColl, AnalyzerParameter param, TString Option){

  if(Option == ""){
    if(param.MuFakeMethod == "MC") Option+="HFake";

    if(RunConv){
      if(param.ConvMethod == "MC") Option+="NHConv";
    }
  }


  if(RunPromptTLRemoval) Option == "NHIntConv";

  ///cout << "AnalyzerCore::GetLepCollByRunType  Muon Option = " << Option << endl;                                                                                                                                                                                                                        

  bool GetHadFake=false,  GetNHIntConv=false, GetNHExtConv=false;

  if(Option.Contains("HFake"))           GetHadFake   =true;
  if(Option.Contains("NHConv"))         {GetNHIntConv =true; GetNHExtConv=true;}
  else{ if(Option.Contains("NHIntConv")) GetNHIntConv =true;
    if(Option.Contains("NHExtConv")) GetNHExtConv =true; }

  if(     Option=="Fake"     )          {GetHadFake   =true; GetNHExtConv=true;}


  if(IsData)  return MuColl;
  if(MCSample.Contains("Type")) return MuColl;


  vector<Muon> ReturnVec;
  for(unsigned int i=0; i<MuColl.size(); i++){
    if(Option=="NoSel")  ReturnVec.push_back(MuColl.at(i));
    else {
      int LepType=GetLeptonType_JH(MuColl.at(i), TruthColl); bool PassSel=true;
      if( LepType > 0 && LepType < 4) PassSel=true;
      if( GetHadFake    && (LepType<0 && LepType>=-4) ) PassSel=true;
      if( GetNHIntConv &&         LepType>=4         ) PassSel=true;
      if( GetNHExtConv &&         LepType<-4         ) PassSel=true;
      if( PassSel ) ReturnVec.push_back(MuColl.at(i));
    }
  }

  return ReturnVec;
}


vector<Electron> HNL_LeptonCore::GetSignalLeptons(const std::vector<Electron>& ElColl, vector<Gen>& TruthColl){

  std::vector<Electron>   ElectronColl;

  if(MCSample.Contains("Type")){

    vector<Gen> gen_lep= GetGenLepronsSignal();

    for(auto iel: ElColl){
      bool matched_lep=false;
      for(auto igen : gen_lep){
        if(fabs(igen.PID()) != 11) continue;
        if(iel.DeltaR(igen) < 0.2 &&  ((iel.Pt()/ igen.Pt()) > 0.7 ) && ((iel.Pt()/ igen.Pt()) < 1.3 )) matched_lep=true;
      }
      if(matched_lep) ElectronColl.push_back(iel);
    }
  }
  else  return ElColl;

  return ElectronColl;
}


vector<Muon> HNL_LeptonCore::GetSignalLeptons(const std::vector<Muon>& MuColl, vector<Gen>& TruthColl){

  std::vector<Muon>   MuonColl;

  if(MCSample.Contains("Type")){

    vector<Gen> gen_lep= GetGenLepronsSignal();
    //for(auto igen : gen_lep) cout << "GetGenLepronsSignal " << igen.Pt() << " " << igen.Eta() << endl;                                                                                                                                                                                                   
    for(auto imu: MuColl){
      bool matched_lep=false;
      for(auto igen : gen_lep){
        if(fabs(igen.PID()) != 13) continue;
        if(imu.DeltaR(igen) < 0.2&& ((imu.Pt()/ igen.Pt()) > 0.7 ) && ((imu.Pt()/ igen.Pt()) < 1.3 )) matched_lep=true;
      }
      if(matched_lep) MuonColl.push_back(imu);
    }
  }
  else return MuColl;


  return  MuonColl;
}




vector<Electron> HNL_LeptonCore::GetLepCollByRunType(const vector<Electron>& ElColl, vector<Gen>& TruthColl, AnalyzerParameter param, TString Option){


  if(Option == ""){
    if(param.ElFakeMethod == "MC") Option+="HFake";

    if(RunConv){
      if(param.ConvMethod == "MC") Option+="NHConv";
    }
    if(RunCF){
      if(param.CFMethod == "MC")     Option+="CF";
    }
  }
  if(RunPromptTLRemoval) Option == "CFNHIntConv";



  //cout << "AnalyzerCore::GetLepCollByRunType Electron  Option = " << Option << endl;                                                                                                                                                                                                                     
  bool GetHadFake=false,  GetNHIntConv=false, GetNHExtConv=false, GetCF=false;

  if(Option.Contains("HFake"))           GetHadFake   =true;
  if(Option.Contains("CF"))              GetCF   =true;
  if(Option.Contains("NHConv"))         {GetNHIntConv =true; GetNHExtConv=true;}
  else{ if(Option.Contains("NHIntConv")) GetNHIntConv =true;
    if(Option.Contains("NHExtConv")) GetNHExtConv =true; }

  if(     Option=="Fake"     )          {GetHadFake   =true; GetNHExtConv=true;}


  if(IsData)  return ElColl;
  if(MCSample.Contains("Type")) return ElColl;

  vector<Electron> ReturnVec;
  for(unsigned int i=0; i<ElColl.size(); i++){
    if (Option == "NoSel") ReturnVec.push_back(ElColl.at(i));
    else {
      int LepType=GetLeptonType_JH(ElColl.at(i), TruthColl); bool PassSel=true;
      if( LepType > 0 && LepType < 4) PassSel=true;
      if( GetHadFake    && (LepType<0 && LepType>=-4) ) PassSel=true;
      if( GetNHIntConv &&         LepType>=4         ) PassSel=true;
      if( GetNHExtConv &&         LepType<-4         ) PassSel=true;
      if( GetCF   && IsCF(ElColl.at(i), TruthColl) ) PassSel=true;
      if( !GetCF && IsCF(ElColl.at(i), TruthColl) ) PassSel=false;
      if( PassSel ) ReturnVec.push_back(ElColl.at(i));
    }
  }
  return ReturnVec;
}


double HNL_LeptonCore::GetLT(std::vector<Lepton *> leps){

  double lt(0.);
  for(auto ilep : leps) lt +=  ilep->Pt();

  return lt;

}

double HNL_LeptonCore::GetLLMass(std::vector<Muon> leps){

  return GetLLMass(MakeLeptonPointerVector(leps));
}

double HNL_LeptonCore::GetLLMass(std::vector<Electron> leps){

  return GetLLMass(MakeLeptonPointerVector(leps));
}

double HNL_LeptonCore::GetLLMass(std::vector<Lepton *> leps){

  if(leps.size() != 2) return -1.;

  Particle ll = (*leps[0]) + (*leps[1]);
  return ll.M();
}

double HNL_LeptonCore::GetMassMinOSSF(std::vector<Lepton *> leps){


  double minOS=99999999999.;

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(ll.M() < minOS) minOS=ll.M();
    }
  }



  return minOS;

}

double  HNL_LeptonCore::GetMassMinSSSF(std::vector<Lepton *> leps){

  double minSS=99999999999.;

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() != leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(ll.M() < minSS) minSS=ll.M();
    }
  }
  if(minSS < 0 ) cout << "minSS = " << minSS << " " << endl;
  return minSS;

}
bool  HNL_LeptonCore::ZmasslllWindowCheck(std::vector<Lepton *> leps){

  if (leps.size() == 3){

    Particle lll = *leps[0] + *leps[1]+ *leps[2];

    bool passZmass_lll_Window = (fabs(lll.M() - 90.1) < 15.);
    return passZmass_lll_Window;

  }

  return true;
}


int HNL_LeptonCore::GetIndexNonMinSSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]) + (*leps[1]);
    Particle ll2 = (*leps[0]) + (*leps[2]);
    Particle ll3 = (*leps[1]) + (*leps[2]);

    double minOS=99999999999.;
    if(fabs(ll1.Charge()) == 2) {
      if(ll1.M()  < minOS) { minOS=ll1.M(); index=2;}
    }
    if(fabs(ll2.Charge()) == 2) {
      if(ll2.M()  < minOS) { minOS=ll2.M(); index=1;}
    }
    if(fabs(ll3.Charge()) == 2) {
      if(ll3.M()  < minOS) { minOS=ll3.M(); index=0;}
    }

  }

  return index;

}

int HNL_LeptonCore::GetIndexNonBestZ(std::vector<Lepton *> leps,double mass_diff){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]) + (*leps[1]);
    Particle ll2 = (*leps[0]) + (*leps[2]);
    Particle ll3 = (*leps[1]) + (*leps[2]);

    double minOSZ=99999999999.;
    if(fabs(ll1.Charge()) == 0) {
      if(fabs(ll1.M() - 90.1) < minOSZ) {minOSZ = fabs(ll1.M() - 90.1); index=2;}
    }
    if(fabs(ll2.Charge()) == 0) {
      if(fabs(ll2.M() - 90.1) < minOSZ) {minOSZ = fabs(ll2.M() - 90.1);index=1;}

    }
    if(fabs(ll3.Charge()) == 0) {
      if(fabs(ll3.M() - 90.1) < minOSZ) {minOSZ = fabs(ll3.M() - 90.1);index=0;}
    }


    if(minOSZ < mass_diff) return index;
  }
  return -1;

}





bool  HNL_LeptonCore::ZmassOSSFWindowCheck(std::vector<Lepton *> leps, double mass_diff){

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if((fabs(ll.M() - 90.1) < mass_diff)) return true;
    }
  }

  return false;
}

double HNL_LeptonCore::GetMassBestZ(std::vector<Lepton *> leps,  bool bestZ){

  double minMZ = 99999999.;
  double massNonBestZ(-9999.), massBestZ(-9999);
  if(leps.size()==4){

    Particle Z1Cand;
    Particle Z2Cand;


    for(unsigned int iel =0; iel < leps.size() ; iel++){
      for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
        if(iel== iel2) continue;
        Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
        if(leps[iel]->Charge() != leps[iel2]->Charge()){

          int zel1(-9), zel2(-9);
          if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = (*leps[2]) + (*leps[3]);}
          if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = (*leps[1]) + (*leps[3]);}
          if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = (*leps[1]) + (*leps[2]);}
          if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = (*leps[0]) + (*leps[3]);}
          if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = (*leps[0]) + (*leps[2]);}
          if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = (*leps[0]) + (*leps[1]);}

          if(leps[zel1]->Charge() != leps[zel2]->Charge()){
            if(( fabs(Z1Cand.M()- 90.1) + fabs(Z2Cand.M()- 90.1)) < minMZ) {
              minMZ = fabs(Z1Cand.M()- 90.1) + fabs(Z2Cand.M()- 90.1);
              if(fabs(Z1Cand.M()- 90.1)< fabs(Z2Cand.M()- 90.1) ){ massBestZ=Z1Cand.M(); massNonBestZ=Z2Cand.M();}
              else{massBestZ=Z2Cand.M(); massNonBestZ=Z1Cand.M();}
            }
          }
        }
      }
    }


    if(bestZ) return massBestZ;
    return massNonBestZ;
  }
  Particle Z1Cand;

  for(unsigned int iel =0; iel < leps.size() ; iel++){

    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
      if(leps[iel]->Charge() != leps[iel2]->Charge()){
        if(fabs(Z1Cand.M()- 90.1) < minMZ) {minMZ = fabs(Z1Cand.M()- 90.1) ; massBestZ=Z1Cand.M();}
      }
    }
  }
  return massBestZ;

}

bool  HNL_LeptonCore::ZmassOSWindowCheck(std::vector<Lepton *> leps){

  bool passZmass_os_Window=false;

  if (leps.size() == 3){


    Particle ll1 = *leps[0] + *leps[1];
    Particle ll2 = *leps[0] + *leps[2];
    Particle ll3 = *leps[1] + *leps[2];

    if(ll1.Charge() == 0 && (fabs(ll1.M() - 90.1) < 15.)) passZmass_os_Window=true;
    if(ll2.Charge() == 0 && (fabs(ll2.M() - 90.1) < 15.)) passZmass_os_Window=true;
    if(ll3.Charge() == 0 && (fabs(ll3.M() - 90.1) < 15.)) passZmass_os_Window=true;

  }

  return passZmass_os_Window;
}


double HNL_LeptonCore::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){


  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}

double HNL_LeptonCore::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){

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


double HNL_LeptonCore::GetST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev){

  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(auto ilep: leps) _st +=ilep->Pt();

  Particle METUnsmearedv = ev.GetMETVector();
  Particle METv =UpdateMETSmearedJet(METUnsmearedv, jets);

  _st += METv.Pt();

  return _st;
}
double HNL_LeptonCore::GetST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){


  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(auto ilep : leps) _st +=  ilep->Pt();
  _st += met.Pt();
  return _st;

}



double HNL_LeptonCore::M_T(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);

  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );

}

double HNL_LeptonCore::GetHT(std::vector<Jet> jets, std::vector<FatJet> fatjets){

  double _ht(0.);
  for(unsigned int i=0; i<jets.size(); i++)_ht += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_ht += fatjets.at(i).Pt();
  return _ht;

}



TString HNL_LeptonCore::GetPtLabel(Muon mu){


  if (mu.Pt() < 10.) return "pt_5_10";
  if (mu.Pt() < 15.) return "pt_10_15";
  if (mu.Pt() < 20.) return "pt_15_20";
  if (mu.Pt() < 30.) return "pt_20_30";
  if (mu.Pt() < 40.) return "pt_30_40";
  if (mu.Pt() < 50.) return "pt_40_50";
  if (mu.Pt() < 100.) return "pt_50_100";
  if (mu.Pt() < 2000.) return "pt_100_2000";

  return "";


}


TString HNL_LeptonCore::GetEtaLabel(Muon mu){
  if(fabs(mu.Eta()) < 0.8 ) return "eta1";
  if(fabs(mu.Eta()) < 1.5 ) return "eta2";
  if(fabs(mu.Eta()) < 2.5 ) return "eta3";
  return "";
}


double HNL_LeptonCore::PassEventTypeFilter(vector<Lepton *> leps , vector<Gen> gens){

  if(IsData) return true;


  int nConv(0);
  //int nCF=(0);                                                                                                                                                                                                                                                                                           
  for(auto ilep: leps){
    int LepType=GetLeptonType_JH(*ilep, gens);
    if( LepType >=4 || LepType < -4) nConv++;
  }
  if(RunConv && nConv==0)  return false;
  if(!RunPromptTLRemoval){
    if(!RunConv && nConv > 0) return false;
  }

  return true;
}

TString HNL_LeptonCore::GetElType(Electron el, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";
  if(GetLeptonType(el, gens) > 0) tag += "NF";
  else tag += "F";

  if(fabs(GetLeptonType(el, gens))  >=4)  tag += "_Conv";
  if(IsCF(el, gens)) tag += "_CF";

  return tag;
}
TString HNL_LeptonCore::GetElTypeString(Electron el, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";
  if(GetLeptonType(el, gens) > 0) tag += "NonFake";
  else tag += "Fake";

  if(fabs(GetLeptonType(el, gens))  >=4)  tag += "_Conv";
  if(IsCF(el, gens)) tag += "_CF";

  return tag;
}

TString HNL_LeptonCore::GetElTypeTString(Electron el, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";

  if(GetLeptonType(el, gens) < 0) tag = "Minus";
  tag+=TString::Itoa(fabs(GetLeptonType(el, gens)), 10);
  return tag;
}

TString HNL_LeptonCore::GetLepTypeTString(const Lepton& lep, const std::vector<Gen>& gens){
  if(IsDATA) return "";
  TString tag = "";
  if(GetLeptonType(lep, gens) < 0) tag = "Minus";
  tag+=TString::Itoa(fabs(GetLeptonType(lep, gens)), 10);
  return tag;
}

bool HNL_LeptonCore::SameCharge(vector<Muon> mus, int ch){

  if(mus.size() != 2) return false;

  int sumQ=mus[0].Charge()+mus[1].Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }
  return false;

}

bool HNL_LeptonCore::SameCharge(vector<Electron> els, int ch){

  if(els.size() != 2) return false;

  int sumQ=els[0].Charge()+els[1].Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }
  return false;
}


bool HNL_LeptonCore::SameCharge(vector<Electron> els, vector<Muon> mus,int ch){

  if(els.size() != 1) return false;
  if(mus.size() != 1) return false;

  int sumQ=els[0].Charge()+mus[1].Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }
  return false;
}


bool HNL_LeptonCore::SameCharge(std::vector<Lepton *> leps, int ch){

  if(leps.size() != 2) return false;

  int sumQ=leps[0]->Charge()+leps[1]->Charge();
  if(ch==0){
    if(fabs(sumQ) == 2) return true;
    return false;
  }
  else if(ch==1){
    if(sumQ == 2) return true;
    return false;

  }
  else if(ch==-1){
    if(sumQ == -2) return true;
    return false;

  }

  return false;
}



TString HNL_LeptonCore::Category(Electron el){


  TString eta_label="El";
  if(fabs(el.Eta()) < 0.8) eta_label = "_BB";
  else if(fabs(el.Eta()) < 1.5) eta_label = "_EB";
  else eta_label = "_EE";

  return eta_label;


}



/*------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------
//
//
// FUNCTIONS NOW ARE FOR PLOTTING OF SR/CR/GENERAL
//
------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------*/


void HNL_LeptonCore::Fill_RegionPlots(HNL_LeptonCore::Channel channel, int plotLL, TString plot_dir, TString region,   std::vector<Jet> jets,  std::vector<FatJet> fatjets,std::vector<Lepton *> Leps, Particle  met, double nvtx,  double w, int verbose_level){

  if(verbose_level>0) return;
  std::vector<Tau>  Taus;
  Fill_RegionPlots(channel, plotLL,plot_dir ,region, Taus,jets, fatjets, Leps, met, nvtx, w , verbose_level);

}




void HNL_LeptonCore::FillAK8Plots(HNL_LeptonCore::Channel channel,  TString plot_dir, TString region,std::vector<Tau> TauColl, std::vector<Jet> jets,   std::vector<FatJet> fatjets, std::vector<Lepton *> leps , Particle  met, double nvtx,  double w){


  bool threelep = (leps.size()  == 3);
  bool fourlep  = (leps.size()  == 4);

  if(threelep && !(channel == EEE  || channel == EMuL || channel == MuMuMu )) return;
  if(fourlep  && !(channel == EEEE  || channel == MuMuMuMu || channel == EMuLL)) return;

  bool DrawAll(false), DrawSyst(false);
  if (plot_dir.Contains("Syst")){
    //DrawSyst=true; //JH
  }
  else{
    //DrawAll=true;  //JH
    //DrawSyst=true; //JH
  }



  if(DrawSyst) FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+  "N_AK8Jets", fatjets.size() , w, 10, 0., 10., "N_{AK8 jets}");

  double minDRTauAK8=9999.;
  for(unsigned int i=0; i < fatjets.size(); i++){
    
    for(auto itau : TauColl){
      if(fatjets[i].DeltaR(itau) < minDRTauAK8) minDRTauAK8 = fatjets[i].DeltaR(itau) ;
    }
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_Eta",  fatjets[i].Eta() , w, 100, -5., 5., "AK8 Jet #eta");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_Pt",  fatjets[i].Pt() , w, 100, 0., 2000., "AK8 Jet p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_SDMass",  fatjets[i].SDMass() , w, 100, 0., 500., "Mass_{softdrop} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_tau21",  fatjets[i].PuppiTau2()/ fatjets[i].PuppiTau1() , w, 200, 0., 1., "#tau_{21}");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/R_AK8Jet_MET",  fatjets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(FJ,met)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/AK8_Tagger_DeepCSV" , fatjets[i].GetTaggerResult(JetTagging::DeepCSV), w, 50, 0, 1., "JetTagging::DeepCSV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/AK8_Tagger_particleNet_WvsQCD" , fatjets[i].GetTaggerResult(JetTagging::particleNet_WvsQCD), w, 50, 0, 1., "JetTagging::particleNet_WvsQCD");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/AK8_Tagger_particleNet_TvsQCD" , fatjets[i].GetTaggerResult(JetTagging::particleNet_TvsQCD), w, 50, 0, 1., "JetTagging::particleNet_WvsQCD");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/AK8_Tagger_particleNet_ZvsQCD" , fatjets[i].GetTaggerResult(JetTagging::particleNet_ZvsQCD), w, 50, 0, 1., "JetTagging::particleNet_WvsQCD");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/AK8_Tagger_particleNet_QCD" , fatjets[i].GetTaggerResult(JetTagging::particleNet_QCD), w, 50, 0, 1., "JetTagging::particleNet_WvsQCD");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/AK8_Tagger_particleNetMD_QCD" , fatjets[i].GetTaggerResult(JetTagging::particleNetMD_QCD), w, 50, 0, 1., "JetTagging::particleNet_WvsQCD");
  }

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dRmin_Tau",   minDRTauAK8,  w, 50, 0, 5, "#DeltaR (WAK8,Tau)");


  if(leps.size() != 2) return;
  if(fatjets.size() == 0) return;
  

  Particle llJCand =  *leps[0] + *leps[1]+ fatjets[0];
  Particle l1JCand = *leps[0] +  fatjets[0];
  Particle l2JCand = *leps[1] +  fatjets[0];
  
  for(unsigned int ij =0; ij < jets.size(); ij++){
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dR_AK4J",   fatjets[0].DeltaR(jets[ij]),  w, 50, 0, 5, "#DeltaR (WAK8,j)");
    if(jets[ij].GetTaggerResult(JetTagging::DeepJet) > mcCorr->GetJetTaggingCutValue(JetTagging::DeepJet, JetTagging::Medium)) {
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dR_AK4BJ",   fatjets[0].DeltaR(jets[ij]),  w, 50, 0, 5, "#DeltaR (WAK8,Bj)");
    }
  }

  FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_Mass_l1J",  l1JCand.M(),  w, 500, 0, 2000, "Reco M_{llJ}");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_Mass_l2J",  l2JCand.M(),  w, 500, 0, 2000, "Reco M_{llJ}");
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_Mass_llJ",  llJCand.M(),  w, 500, 0, 2000, "Reco M_{llJ}");
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_Mass_llJOverl1J",  llJCand.M()/ l1JCand.M(),  w, 500, 0, 5, "Reco M_{llJ}");

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dR_W_lep1",   fatjets[0].DeltaR(*leps[0]),  w, 50, 0, 5, "#DeltaR (Wj1,lep1)");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dR_W_lep2",   fatjets[0].DeltaR(*leps[1]),  w, 50, 0, 5, "#DeltaR (Wj2,lep2)");
  
  if(fatjets[0].DeltaR(*leps[0] ) < fatjets[0].DeltaR(*leps[1] )){
    Particle lJcloseCand = *leps[0]  +  fatjets[0];
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_Mass_W_close_lep",  lJcloseCand.M(),  w, 250, 0, 2000, "Reco M_{llJ}");
  }



  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_l1_llJ",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] + *leps[0]+*leps[1]).Phi() - (leps[0]->Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_l1_llJ",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] + *leps[0]+*leps[1]).Phi() - (leps[1]->Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_N1_llJ",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] + *leps[0]+*leps[1]).Phi() - ((*leps[0]+fatjets[0]).Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_N2_llJ",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] + *leps[0]+*leps[1]).Phi() - ((*leps[1]+fatjets[0]).Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_W_llJ",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] + *leps[0]+*leps[1]).Phi() - ((fatjets[0]).Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_l1_lJ",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] + *leps[0]).Phi() - (leps[1]->Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_l2_lJ",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] + *leps[1]).Phi() - (leps[0]->Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_l1_J",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] ).Phi() - (leps[0]->Phi() )))),  w,  100, 0., 5., "");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK8Jet_dPhi_l2_J",  fabs(TVector2::Phi_mpi_pi( ( (fatjets[0] ).Phi() - (leps[1]->Phi() )))),  w,  100, 0., 5., "");

  return;
}


void HNL_LeptonCore::Fill_RegionPlots(HNL_LeptonCore::Channel channel, int fill_lep, TString plot_dir, TString region, vector<Tau> Taus,  std::vector<Jet> jets,    std::vector<FatJet> fatjets, std::vector<Lepton *> leps , Particle  met, double nvtx,  double w,int verbose_level){

  if(verbose_level > 0) return;
  Fill_RegionPlots(channel, plot_dir ,region, Taus, jets,fatjets, leps, met, nvtx, w,verbose_level);

  if(fill_lep ==0) {
    TString plot_dir_lep = plot_dir;
    plot_dir_lep = plot_dir_lep.ReplaceAll(GetChannelString(channel),"LL");
    Fill_RegionPlots(channel, plot_dir_lep ,region, Taus, jets ,fatjets, leps, met, nvtx, w,verbose_level);
  }
  return;
}

void HNL_LeptonCore::Fill_RegionPlots(HNL_LeptonCore::Channel channel, TString plot_dir, TString region, std::vector<Tau> TauColl, std::vector<Jet> jets,    std::vector<FatJet> fatjets, std::vector<Lepton *> leps , Particle  met, double nvtx,  double w, int verbose_level){
  
  if(verbose_level > 0) return;

  

  bool threelep = (leps.size()  == 3);
  bool fourlep  = (leps.size()  == 4);
  
  if(threelep && !(channel == EEE  || channel == EMuL || channel == MuMuMu )) return;
  if(fourlep  && !(channel == EEEE  || channel == MuMuMuMu || channel == EMuLL)) return;
     

  int nel(0), nmu(0);
  for(auto ilep: leps) {
    if(ilep->LeptonFlavour() == Lepton::ELECTRON) nel++;
    if(ilep->LeptonFlavour() == Lepton::MUON) nmu++;
  }

  bool DrawAll(false), DrawSyst(false);
  if (plot_dir.Contains("Syst")){
    //DrawSyst=true; //JH
  }
  else{
    //DrawAll=true;  //JH
    //DrawSyst=true; //JH
  }

  FillAK8Plots(channel, plot_dir, region, TauColl,jets , fatjets, leps, met, nvtx,w);


 /// Draw N leptons
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/N_El", nel,  w, 5, 0, 5, "El size");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/N_Mu", nmu,  w, 5, 0, 5, "Mu size");
  // Draw N jets
  if(DrawSyst) FillHist( plot_dir+"/RegionPlots_"+ region+ "/N_AK4Jets", jets.size() , w, 10, 0., 10., "N_{AK4 jets}");


  if(leps.size() < 2) return;

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/SumQ", leps[0]->Charge() + leps[1]->Charge(),  w, 10, -5, 5, "Q size");

  Particle llCand = *leps[0] + *leps[1];
  Particle lllCand = (threelep) ? *leps[0] + *leps[1] + *leps[2] :  Particle();
  Particle lljjCand;

  if(jets.size() ==1 ) {
  
    lljjCand += jets[0];

    Particle WCand  = jets[0];
    Particle N1Cand = jets[0]+ *leps[0] ;
    Particle N2Cand = jets[0]+ *leps[1] ;

    //if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_M_l1W_M_l2W",  N1Cand.M(), N2Cand.M(), w, 200, 0., 2000., 200, 0., 2000.);
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_dR_W_lep1",   WCand.DeltaR(*leps[0] ),  w, 50, 0, 5, "#DeltaR (W,lep1)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_dR_W_lep2",   WCand.DeltaR(*leps[1] ),  w, 50, 0, 5, "#DeltaR (W,lep2)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_mW",   WCand.M(),  w, 50, 0, 500, "Reco_Onejet M_{jj}");

    int nSRbins=8;
    double mljbins[nSRbins] = { 0., 200., 400.,500., 600.,700., 1000., 2000.};
    double MN1 = (N1Cand.M() > 2000.) ? 1999. : N1Cand.M();
    double MN2 = (N2Cand.M() > 2000.) ? 1999. : N2Cand.M();
    double MllW = (lljjCand.M() > 2000.) ? 1999. : lljjCand.M();

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_M_l1W", MN1,  w, 7, mljbins , "Reco_Onejet M_{l1jj}");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_M_l2W",  MN2,  w, 7, mljbins, "Reco_Onejet M_{l2jj} ");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_M_llW",  MllW,  w, 7, mljbins, "Reco_Onejet M_{lljj}");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Single_AK4Jet_dPhi_l1_l1J",  fabs(TVector2::Phi_mpi_pi( ( (*leps[0]+ jets[0]).Phi() - (leps[1]->Phi() )))),  w,  500, 0., 2000., "Reco_Onejet M_{lljj}");

    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK4Jet_1_pt", jets[0].Pt() ,  w, 2000, 0, 2000,"l_{1} p_{T} GeV"); //JH
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK4Jet_2_pt", jets[1].Pt() ,  w, 2000, 0, 2000,"1_{2} p_{T} GeV"); //JH
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK4Jet_1_eta", jets[0].Eta()  , w, 60, -3., 3,"l_{1} #eta");       //JH
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK4Jet_2_eta", jets[1].Eta()  , w, 60, -3., 3.,"l_{2} #eta");      //JH

  }
  
  if(jets.size() > 1){
    
    double dijetmass_tmp=999.;
    double dijetmass=9990000.;
    int m=-999;
    int n=-999;

    for(UInt_t emme=0; emme<jets.size(); emme++){
      for(UInt_t enne=1; enne<jets.size(); enne++) {

  if(emme == enne) continue;
        dijetmass_tmp = (jets[emme]+jets[enne]).M();
  
        if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
    dijetmass = dijetmass_tmp;
    m = emme;
          n = enne;
  }
      }
    }
    lljjCand = *leps[0]  + *leps[1]  + jets[m]+jets[n];
    Particle WCand  = jets[m]+jets[n];
    Particle N1Cand  = jets[m]+jets[n]+ *leps[0] ;
    Particle N2Cand  = jets[m]+jets[n]+ *leps[1] ;
    Particle NCand = (leps[1]->Pt() < leps[0]->Pt()) ? N2Cand : N1Cand;
    Lepton Nlep = (leps[1]->Pt() < 100.) ?  *leps[1] : *leps[0];
    Lepton Wlep = (leps[1]->Pt() > 100.) ? *leps[1] : *leps[0];

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromN_N",  NCand.DeltaR(Nlep) , 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromN_W",  WCand.DeltaR(Nlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromN_N",  NCand.DeltaR(Nlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromW_N",  NCand.DeltaR(Wlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromW_W",  WCand.DeltaR(Wlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromN_J1",  jets[m].DeltaR(Nlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromN_J2",  jets[n].DeltaR(Nlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_Lep_FromW_LepFromN",  Nlep.DeltaR(Wlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_W_N",  NCand.DeltaR(Wlep), 1., 200, 0., 5.,"");


    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_Lep_FromN_N",  fabs(TVector2::Phi_mpi_pi( ( (NCand.Phi() - Nlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_Lep_FromN_W",  fabs(TVector2::Phi_mpi_pi( ( (WCand.Phi() - Nlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_Lep_FromW_N",  fabs(TVector2::Phi_mpi_pi( ( (NCand.Phi() - Wlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_Lep_FromW_W",  fabs(TVector2::Phi_mpi_pi( ( (WCand.Phi() - Wlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_Lep_FromW_LepFromN",  fabs(TVector2::Phi_mpi_pi( ( (Nlep.Phi() - Wlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_W_N",  fabs(TVector2::Phi_mpi_pi( ( (WCand.Phi() - NCand.Phi() )))),  1., 200, 0., 5.,"");

    Particle WrongN = Wlep + WCand;

    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaRWr_Lep_FromN_N",  WrongN.DeltaR(Nlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaRWr_Lep_FromW_N",  WrongN.DeltaR(Wlep), 1., 200, 0., 5.,"");

    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhiWr_Lep_FromN_N",  fabs(TVector2::Phi_mpi_pi( ( (WrongN.Phi() - Nlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhiWr_Lep_FromW_N",  fabs(TVector2::Phi_mpi_pi( ( (WrongN.Phi() - Wlep.Phi() )))),  1., 200, 0., 5.,"");

    Particle W1 = (NCand+Wlep);

    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_W1_Lep_FromN", W1.DeltaR(Nlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_W1_Lep_FromW", W1.DeltaR(Wlep), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_W1_N", W1.DeltaR(NCand), 1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaR_W1_WCand", W1.DeltaR(WCand), 1., 200, 0., 5.,"");


    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_W1_Lep_FromN",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - Nlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_W1_Lep_FromW",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - Wlep.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_W1_N",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - NCand.Phi() )))),  1., 200, 0., 5.,"");
    if(DrawAll)FillHist(plot_dir+"/RegionPlots_"+ region+ "/DiJet_DeltaPhi_W1_WCand",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - WCand.Phi() )))),  1., 200, 0., 5.,"");



    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_l1_llJ",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] + *leps[0]+*leps[1]).Phi() - (leps[0]->Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_l1_llJ",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] + *leps[0]+*leps[1]).Phi() - (leps[1]->Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_N1_llJ",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] + *leps[0]+*leps[1]).Phi() - ((*leps[0]+jets[m]+ jets[n]).Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_N2_llJ",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] + *leps[0]+*leps[1]).Phi() - ((*leps[1]+jets[m]+ jets[n]).Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_W_llJ",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] + *leps[0]+*leps[1]).Phi() - ((jets[m]+ jets[n]).Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_l1_lJ",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] + *leps[0]).Phi() - (leps[1]->Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_l2_lJ",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] + *leps[1]).Phi() - (leps[0]->Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_l1_J",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] ).Phi() - (leps[0]->Phi() )))),  w,  100, 0., 5., "");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dPhi_l2_J",  fabs(TVector2::Phi_mpi_pi( ( (jets[m]+ jets[n] ).Phi() - (leps[1]->Phi() )))),  w,  100, 0., 5., "");



    FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_M_l1W_M_l2W",  N1Cand.M(), N2Cand.M(), w, 100, 0., 2000., 100, 0., 2000.);
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dR_W_lep1",   WCand.DeltaR(*leps[0] ),  w, 50, 0, 5, "#DeltaR (W,lep1)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dR_W_lep2",   WCand.DeltaR(*leps[1] ),  w, 50, 0, 5, "#DeltaR (W,lep2)");
    FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_M_W",   WCand.M(),  w, 100, 0, 2000, "Reco M_{jj}");
    

    int nSRbins=8;
    double mljbins[nSRbins] = { 0., 200., 400.,500., 600.,700., 1000., 2000.};
    double MN1 = (N1Cand.M() > 2000.) ? 1999. : N1Cand.M();
    double MN2 = (N2Cand.M() > 2000.) ? 1999. : N2Cand.M();
    double MllW = (lljjCand.M() > 2000.) ? 1999. : lljjCand.M();


    FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_M_l1W",   MN1,  w, 7, mljbins , "Reco M_{l1jj}");
    FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_M_l2W",   MN2,  w, 7, mljbins , "Reco M_{l2jj} ");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_M_lAv12W", (N1Cand.M()+  N2Cand.M())/2.,  w,  500, 0., 2000., "Reco M_{l1_2jj} ");
    
    if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_M_llW",  MllW ,  w, 7, mljbins , "Reco M_{lljj}");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dR_Wj1_lep2",   jets[m].DeltaR(*leps[1] ),  w, 50, 0, 5, "#DeltaR (Wj1,lep2)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/DiJet_dR_Wj2_lep2",   jets[n].DeltaR(*leps[1] ),  w, 50, 0, 5, "#DeltaR (Wj2,lep2)");
  }// end dijet loop


  double LT(0.);
  double minDRLep1Tau=9999.;
  double minDRLep2Tau=9999.;
  int nlep(0);
  for(auto i : leps){
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Leps_pt", i->Pt()  , w, 200, 0., 1000.,"1_{2} p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Leps_eta",i->Eta()  , w, 30, -3., 3,"l_{1} #eta");
    LT += i->Pt();
    
    
    for(auto itau  : TauColl){
      if(nlep==0 && i->DeltaR(itau) < minDRLep1Tau) minDRLep1Tau = i->DeltaR(itau);
      if(nlep==1 && i->DeltaR(itau) < minDRLep2Tau) minDRLep2Tau = i->DeltaR(itau);
    }
    nlep++;
  }
  if(DrawAll&&minDRLep1Tau < 999)FillHist( plot_dir+"/RegionPlots_"+ region+ "/dRMin_Lep1_Tau", minDRLep1Tau  ,w, 50, 0, 5, "#DeltaR (Tau,lep1)");
  if(DrawAll&&minDRLep2Tau < 999)FillHist( plot_dir+"/RegionPlots_"+ region+ "/dRMin_Lep2_Tau", minDRLep2Tau  ,w, 50, 0, 5, "#DeltaR (Tau,lep2)");

  int nPtbins=7;
  double Pt1bins[nPtbins+1] = { 20.,25.,30., 40.,50., 70., 100., 200.};
  double Pt2bins[nPtbins+1] = { 10.,15., 20.,30., 40.,50., 100., 200.};
  double PTLep1 = (leps[0]->Pt() > 200.) ? 199. : leps[0]->Pt();
  double PTLep2 = (leps[1]->Pt() > 200.) ? 199. : leps[1]->Pt();

  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_1_pt", leps[0]->Pt()  ,  w, 100, 0, 2000,"l_{1} p_{T} GeV"); //JH
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_2_pt", leps[1]->Pt()  ,  w, 100, 0, 2000,"1_{2} p_{T} GeV"); //JH
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_1_eta", leps[0]->Eta()  , w, 60, -3., 3,"l_{1} #eta");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_2_eta", leps[1]->Eta()  , w, 60, -3., 3.,"l_{2} #eta");

  if(threelep) {
    FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_3_pt", leps[2]->Pt()  , w, 100, 0., 1000.,"l_{3} p_{T} GeV"); //JH
    FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_3_eta", leps[2]->Eta()  , w, 60, -3., 3.,"l_{3} #eta");
  }
  if(fourlep) {
    FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_4_pt", leps[3]->Pt()  , w, 100, 0., 1000.,"l_{4} p_{T} GeV"); //JH
    FillHist( plot_dir+"/RegionPlots_"+ region+ "/Lep_4_eta", leps[3]->Eta()  , w, 60, -3., 3.,"l_{4} #eta");
  }

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Vertex_X", vertex_X  , w, 200, 0., 1,"L_{T} GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Vertex_Y", vertex_Y  , w, 200, 0., 1,"L_{T} GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Vertex_Z", vertex_Z  , w, 200, 0., 1,"L_{T} GeV");


  double HT = GetHT(jets, fatjets);
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_LT", LT  , w, 200, 0., 2000.,"L_{T} GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_HT", HT  , w, 200, 0., 2000.,"H_{T} GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/HToLepPt1", HT/ leps[0]->Pt()  , w, 100, 0., 20.,"HT/PT(1)");
  if(DrawSyst)FillHist( plot_dir+"/RegionPlots_"+ region+ "/HToLepPt2", HT/ leps[1]->Pt()  , w, 100, 0., 20.,"HT/PT(2)");
  if(threelep&&DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/HToLepPt3", HT/ leps[2]->Pt()  , w, 100, 0., 20.,"HT/PT(3)");
  if(fourlep&&DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/HToLepPt4", HT/ leps[3]->Pt()  , w, 100, 0., 20.,"HT/PT(4)");

  
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "nPV",  nvtx , w, 120, 0., 120.);
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "nPileUp",  nPileUp, w, 120, 0., 120.);
  
  double ST = GetST(leps, jets, fatjets, met);
  double met2_st = pow(met.Pt(),2.)/ ST;  

  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_ST", ST  , w, 250, 0., 5000.,"ST GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_MET2_ST", met2_st  , w, 40, 0., 20.,"MET2/ST GeV");


  Particle METv = GetvMET("T1xyCorr");
  Particle PuppiMETv = GetvMET("PuppiT1xyCorr");
  Particle METvNoPhi = GetvMET("T1");
  Particle PuppiMETvNoPhi = GetvMET("PuppiT1");
  Particle PuppiMETvULPhiCorr = GetvMET("PuppiT1xyULCorr");
  Particle METvULPhiCorr = GetvMET("T1xyULCorr");

  
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_MET", met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_pfMET_T1", METvNoPhi.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_pfMET_T1xyCorr", METv.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_PuppiMET_T1", PuppiMETvNoPhi.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_PuppiMET_T1xyCorr", PuppiMETv.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_PuppiMET_T1ULxyCorr", PuppiMETvULPhiCorr.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_pfMET_T1ULxyCorr", METvULPhiCorr.Pt()  , w, 200, 0., 400.,"MET GeV");

  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_pfMETphi_T1", METvNoPhi.Phi()  , w, 200, -5., 5.,"MET #phi");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_pfMETphi_T1xyCorr", METv.Phi()  , w, 200, -5., 5.,"MET #phi");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_PuppiMETphi_T1", PuppiMETvNoPhi.Phi()  , w, 200, -5., 5.,"MET #phi");
  FillHist( plot_dir+"/RegionPlots_"+ region+ "/Ev_PuppiMETphi_T1xyCorr", PuppiMETv.Phi()  , w, 200, -5., 5.,"MET #phi");

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Mt_lep1", MT(*leps[0] ,met)  , w, 200, 0., 400.,"MT GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Mt_lep2", MT(*leps[1] ,met)  , w, 200, 0., 400.,"MT GeV");
 
  if(threelep) {
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/M_lll"+region, lllCand.M() , w, 200, 0., 800.,"M(lll) GeV");

    if(GetIndexNonMinOSSF(leps)> 0 && DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Mt_minOSSF", MT(*leps[GetIndexNonMinOSSF(leps)], met) , w, 200, 0., 800.,"M(T) non MinOS GeV");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/M_minOSSF", GetMassMinOSSF(leps), w, 200, 0., 800.,"M non MinOS GeV");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/M_minSSSF", GetMassMinSSSF(leps), w, 200, 0., 800.,"M(T) non MinSS GeV");
    if(GetIndexNonMinSSSF(leps) > 0 && DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Mt_minSSSF", MT(*leps[GetIndexNonMinSSSF(leps)], met) , w, 200, 0., 800.,"M non MinSS GeV");

  }

  if(fourlep) {
    Particle llllCand = *leps[0] + *leps[1] + *leps[2] + *leps[3] ;
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/M_llll", llllCand.M() , w, 200, 0., 800.,"M(llll) GeV");
  }
  
  Event ev = GetEvent();
  Particle METunsmearedv = ev.GetMETVector();
  

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/MET_uncorr", METunsmearedv.Pt()  , w, 200, 0., 400.,"MET GeV");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/M_ll",  llCand.M(), w, 400, 0., 2000., "M_{ll} GeV");
  
  //  return;

  double mindRlepj1(99999.);
  double mindRlepj2(99999.);

  for(unsigned int i=0; i < jets.size(); i++){

    if(i == 0){
      FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Jet_1_pt",  jets[i].Pt() , w, 100, 0., 2000., "j_{1} p_{T} GeV");
      FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Jet_1_eta",  jets[i].Eta() , w, 100, -5., 5., "j_{1} #eta");
    }
    if(i == 1){
      FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Jet_2_pt",  jets[i].Pt() , w, 100, 0., 2000., "j_{2} p_{T} GeV");
      FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Jet_2_eta",  jets[i].Eta() , w, 100, -5., 5., "j_{2} #eta"); //JH
    }
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_pileup_mva",  jets[i].PileupJetId() , w, 100, -1., 1., "PileupJetId");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_pileup_loose", jets[i].PassPileupMVA("loose", GetEraShort()), w, 2, 0., 2., "PileupJetId");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_pileup_medium", jets[i].PassPileupMVA("medium", GetEraShort()), w, 2, 0., 2., "PileupJetId");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_pileup_tight", jets[i].PassPileupMVA("tight", GetEraShort()), w, 2, 0., 2., "PileupJetId");

    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK4DeepCSV_score", jets[i].GetTaggerResult(JetTagging::DeepCSV), w, 100, 0, 1., "DeepCSV_score");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/AK4DeepJet_score", jets[i].GetTaggerResult(JetTagging::DeepJet), w, 100, 0, 1., "DeepCSV_score");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_pt",  jets[i].Pt() , w, 400, 0., 2000., "AK4 Jet p_{T} GeV");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_eta",  jets[i].Eta() , w, 100, -5., 5., "AK4 Jet #eta ");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_CEEF", jets[i].ChargedEmEnergyFraction(), w, 50, 0., 1. ,"");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/Jet_CHEF", jets[i].ChargedHadEnergyFraction(), w, 50, 0., 1. ,"");

    if(jets[i].DeltaR(*leps[0] ) < mindRlepj1) mindRlepj1=jets[i].DeltaR(*leps[0] );
    if(jets[i].DeltaR(*leps[1] ) < mindRlepj2) mindRlepj2=jets[i].DeltaR(*leps[1] );
    
    if(i == 0){
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet1_lep1",  jets[i].DeltaR(*leps[0] ) , w, 50, 0., 5., "#Delta R(l1,j)") ;
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet1_lep2",  jets[i].DeltaR(*leps[1] ) , w, 50, 0., 5., "#Delta R(l2,j)"); 
    }
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet_lep1",  jets[i].DeltaR(*leps[0] ) , w, 50, 0., 5., "#Delta R(l1,j)") ;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dR_jet_lep2",  jets[i].DeltaR(*leps[1] ) , w, 50, 0., 5., "#Delta R(l2,j)");
    
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_jet_MET",  jets[i].DeltaR(met) ,w, 50,  0., 5,"#DeltaR(j,met)");

    for(unsigned int j=1; j < jets.size(); j++){
      if(i==j)continue;
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"Jet_dR_jj",  jets[i].DeltaR(jets[j]) ,w, 50,  0., 5,"#DeltaR(j,j)");
      if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"Jet_M_jj",  (jets[i]+jets[j]).M() ,w, 200,  0., 2000,"M(j,j)");
    }
  }



  if(jets.size() > 1){

    double maxDiJetDeta=0.;
    int ijet1(-1), ijet2(-1);
    for(unsigned int ij = 0; ij < jets.size()-1; ij++){
      for(unsigned int ij2 = ij+1; ij2 < jets.size(); ij2++){
  
  double deta = fabs(jets[ij].Eta() - jets[ij2].Eta());
  if(deta > maxDiJetDeta) {
    maxDiJetDeta=deta;
    ijet1=ij;
    ijet2=ij2;
  }
      }
    }
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/Jet_dR_jet1_lep1",  jets[ijet1].DeltaR(*leps[0]) ,w, 100,  0., 5,"#DeltaR(j,l1)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/Jet_dR_jet1_lep2",  jets[ijet1].DeltaR(*leps[1]) ,w, 100,  0., 5,"#DeltaR(j,l1)");
    
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "MaxDEta_jet1_jet2", maxDiJetDeta  , w, 200, 0., 10., "Max DEta");
    Particle JJMEta = jets[ijet1] + jets[ijet2];
    
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "MaxDEtaJets_MJJ",JJMEta.M()   , w, 200, 0., 2000., "MaxDEta MJJ");
    
    double Av_JetEta= 0.5*(jets[ijet1].Eta()+ jets[ijet2].Eta());
    double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "MaxDEta_Jets_zeppenfeld", zeppenfeld  , w, 200, 0., 2., "zeppenfeld");
  }

  if(jets.size()>1){

    Particle JJLead = jets[0] + jets[1];
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Lead_MJJ",JJLead.M()   , w, 200, 0., 2000., "Lead MJJ");
    
    double maxDiJetDeta=fabs(jets[0].Eta() - jets[1].Eta());

    double Av_JetEta= 0.5*(jets[0].Eta()+ jets[1].Eta());
    double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Lead_zeppenfeld", zeppenfeld  , w, 200, 0., 2., "zeppenfeld");    
  }


  //////

  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  double ll_deta = fabs((*leps[0]).Eta() - (*leps[1]).Eta());
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dPhi_lep1_lep2", ll_dphi  , w, 200, -5., 5., "#Delta #Phi(l1,l2)") ;
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dEta_lep1_lep2", ll_deta  , w, 200, -5., 5., "#Delta #Phi(l1,l2)") ;

  
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dRmin_jet_lep1", mindRlepj1  , w, 50, 0., 5., "#Delta R(l1,j)") ;
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "dRmin_jet_lep2", mindRlepj2  , w, 50, 0., 5., "#Delta R(l1,j)") ;

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Sum_lep_eta", (fabs(leps[0]->Eta())+  fabs(leps[1]->Eta())) , w, 50, 0., 5.);
  if(fabs(leps[0]->Eta()) > fabs(leps[1]->Eta())) {
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Max_lep_eta", fabs(leps[0]->Eta()) , w, 50, 0., 2.5);
  }
  else  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+ "/"+ "Max_lep_eta", fabs(leps[1]->Eta()) , w, 50, 0., 2.5);
    
  

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_l1_MET",  leps[0]->DeltaR(met) ,w, 100,  0., 5,"#DeltaR(l1,met)");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_l2_MET",  leps[1]->DeltaR(met) ,w, 100,  0., 5,"#DeltaR(l2,met)");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_ll", leps[0]->DeltaR(*leps[1] ) ,w, 50,  0., 5,"#DeltaR(l,l)");

  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dPhi_l1_MET",  fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (met).Phi() )) ) ,w, 100,  0., 5,"#DeltaR(l1,met)");
  if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dPhi_l2_MET",  fabs(TVector2::Phi_mpi_pi( ( (*leps[1]).Phi() - (met).Phi() )) ) ,w, 100,  0., 5,"#DeltaR(l1,met)");

  if(leps[1]->Pt() < 100){
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_lN_MET",  leps[1]->DeltaR(met) ,w, 100,  0., 5,"#DeltaR(l1,met)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dPhi_lN_MET",  fabs(TVector2::Phi_mpi_pi( ( (*leps[1]).Phi() - (met).Phi() )) ) ,w, 100,  0., 5,"#DeltaR(l1,met)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_lW_MET",  leps[0]->DeltaR(met) ,w, 100,  0., 5,"#DeltaR(l1,met)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dPhi_lW_MET",  fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (met).Phi() ))) ,w, 100,  0., 5,"#DeltaR(l1,met)");
  }
  else{
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_lN_MET",  leps[0]->DeltaR(met) ,w, 100,  0., 5,"#DeltaR(l1,met)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dPhi_lN_MET",  fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (met).Phi() ))) ,w, 100,  0., 5,"#DeltaR(l1,met)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dR_lW_MET",  leps[1]->DeltaR(met) ,w, 100,  0., 5,"#DeltaR(l1,met)");
    if(DrawAll)FillHist( plot_dir+"/RegionPlots_"+ region+"/"+"dPhi_lW_MET",  fabs(TVector2::Phi_mpi_pi( ( (*leps[1]).Phi() - (met).Phi() ))) ,w, 100,  0., 5,"#DeltaR(l1,met)");

  }


  
  
  return;
}



void HNL_LeptonCore::FillEventCutflowAll(TString analysis_dir_name,TString histname, double weight, vector<TString> lables, TString label){
  FillEventCutflowDef(analysis_dir_name,histname, weight,lables, label);
  
  //if(!IsData)FillEventCutflowDef(analysis_dir_name,histname+"_no_weight",1.,lables, label);

}
void HNL_LeptonCore::FillEventCutflowDef(TString analysis_dir_name,TString histname, double weight, vector<TString> lables, TString label){

  TH1D *this_hist = GetHist1D(analysis_dir_name+"/"+histname);


  if( !this_hist ){
    TString cf_name="FillEventCutflow";
    //if(histname.Contains("Syst")) cf_name= "FillEventCutflow_Syst";
    //if(histname.Contains("SR")) cf_name="FillEventCutflow_SR";
    if(histname.Contains("_massbinned")) cf_name="FillEventCutflow_MassBinned";
    
    if(!analysis_dir_name.Contains("ChannelCutFlow"))  cf_name = analysis_dir_name + "/"+cf_name;
    else cf_name = analysis_dir_name;

    this_hist = new TH1D(cf_name+"/"+histname, "", lables.size(), 0, lables.size());
    //    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    this_hist->SetDirectory(NULL);

    maphist_TH1D[analysis_dir_name+"/"+histname] = this_hist;
  }
  this_hist->Fill(label, weight);

}



void HNL_LeptonCore::FillEventCutflow(HNL_LeptonCore::SearchRegion sr, double event_weight, TString label,   TString analysis_dir_name, int verbose_level){
  

 
  if (run_Debug) cout << "FillEventCutflow " << label << " " << analysis_dir_name <<  endl;

  vector<TString> labels;
  TString EVhitname("");
  
  /// verbose_level  sets which Cutflow to fill
  /// by default it is 0 and ALL are filled
  /// If set to 1 then only limits are filled
  if (verbose_level < 0) return;
  
  if (verbose_level == 0) {
    
    if(sr==SR1 ){
      labels = {  "Presel", "AK8Jet", "SR1_Init",   "SR1_lep_charge",  "SR1_lep_pt", "SR1_dilep_mass" , "SR1_LepPt","SR1_DphiN_Wlep", "SR1_TauVeto", "SR1_HTPt", "SR1_Wmass",  "SR1_MET" , "SR1_bveto" };
      EVhitname= "SR1";
    }
    
    if(sr==SR2){
      labels = {   "Presel", "SR2_lep_charge", "SR2_lep_pt", "SR2_DPhi", "SR2_DiJet",  "SR2_DiJetEta",  "SR2_DiJetMass", "SR2_VBF",     "SR2_ht_lt1",      "SR2_bveto"};
      EVhitname= "SR2";
    }
    
    if( sr==SR3){
      labels = {  "Presel", "NoAK8Jet", "FailVBF",    "SR3_lep_charge" ,     "SR3_lep_pt",      "SR3_dilep_mass","SR3_0JetBin","SR3_1JetBin",      "SR3_jet",       "SR3_dijet",
      "SR3_Wmass", "SR3_J1Pt",     "SR3_MET",      "SR3_bveto"};
      EVhitname= "SR3";
    }
    if( sr==SR3BDT){
      labels = {  "Presel", "NoAK8Jet", "FailVBF",    "SR3_lep_charge" ,     "SR3_lep_pt",      "SR3_dilep_mass",      "SR3_jet",       "SR3_dijet",
                  "SR3_Wmass", "SR3_J1Pt",     "SR3_MET",      "SR3_bveto"};
      EVhitname= "SR3BDT";
    }



    if(sr==SR4){
      labels = {      "SR4_3lep",             "SR4_3lep_veto", "SR4_3lep_chargereq",   "SR4_3lep_bjet",      "SR4_3lep_Zmlll",       "SR4_3lep_Zmll_os",      "SR4_lll_mu"};
      EVhitname= "SR4";
    }
    if(sr==PreselSS || sr==Presel ){
      labels = {"NoCut", "METFilter", "Trigger", "Dilepton",
    "SS_Dilep" ,"SS_lep_veto", "SS_Dilep_mass", "SS_Presel"};
      EVhitname = "SS_Presel";
    }
    if(sr==PreselOS || sr==Presel ){
      labels = {"NoCut", "METFilter", "Trigger", "Dilepton",
        "OS_Dilep" ,"OS_lep_veto", "OS_Dilep_mass", "OS_Presel"};
      EVhitname ="OS_Presel";
    }
    if(sr==ChannelDepInc ){
      labels = {"MuMu_NoCut","EE_NoCut","EMu_NoCut"};
      EVhitname ="ChannelDependant_Inclusive";
    }
    if(sr==ChannelDepIncQ ){
      labels = {"MuMu_MMQ_NoCut","MuMu_PPQ_NoCut", "EE_MMQ_NoCut",  "EE_PPQ_NoCut","EMu_MMQ_NoCut", "EMu_PPQ_NoCut"};
      EVhitname ="ChannelDependantQQ_Inclusive";
    }
    if(sr==ChannelDepDilep){
      labels = {"MuMu_Dilep","EE_Dilep","EMu_Dilep"};
      EVhitname ="ChannelDependantDilep";
    }
    if(sr==ChannelDepTrigger){
      labels = {"MuMu_Trigger","EE_Trigger","EMu_Trigger", "MuMu_MultiTrigger","EE_MultiTrigger","EMu_MultiTrigger"};
      EVhitname ="ChannelDependantTrigger";
    }
    if(sr==ChannelDepPresel){
      labels = {"MuMu_Presel","EE_Presel","EMu_Presel"};
      EVhitname ="ChannelDependantPresel";
    }
    
    if(sr==ChannelDepSR1){
      labels = {"MuMu_SR1","EE_SR1","EMu_SR1"};
      EVhitname ="ChannelDependantSR1";
    }
    if(sr==ChannelDepSR2){
      labels = {"MuMu_SR2","EE_SR2","EMu_SR2"};
      EVhitname ="ChannelDependantSR2";
    }
    
    if(sr==ChannelDepSR3){
      labels = {"MuMu_SR3","EE_SR3","EMu_SR3"};
      EVhitname ="ChannelDependantSR3";
    }
    
    if(sr==ChannelDepFAILSR3){
      labels = {"MuMu_SR3Fail_0j","MuMu_SR3Fail_1j","MuMu_SR3Fail_2j", "EE_SR3Fail_0j","EE_SR3Fail_1j","EE_SR3Fail_2j","EMu_SR3Fail_0j","EMu_SR3Fail_1j","EMu_SR3Fail_2j"};
      EVhitname ="ChannelDependantSR3Fail";
    }
    
    if(sr==SR){
      labels = {"SR1Pass","SR1Fail","SR2Pass","SR2Fail","SR3Pass","SR3Fail","SR4Pass","SR4Fail"
      };
      EVhitname ="SR_Summary";
    }
    
    
    if(sr==ChannelDepCR1){
      labels = {"MuMu_CR1","EE_CR1","EMu_CR1"};
      EVhitname ="ChannelDependantCR1";
    }
    if(sr==ChannelDepCR2){
      labels = {"MuMu_CR2","EE_CR2","EMu_CR2"};
      EVhitname ="ChannelDependantCR2";
    }
    
    if(sr==ChannelDepCR3){
      labels = {"MuMu_CR3","EE_CR3","EMu_CR3"};
      EVhitname ="ChannelDependantCR3";
    }
  }
  
  if(verbose_level >= 0){
    if(sr==MuonSR){
      labels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1", "SR2_bin2", "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8"};
      EVhitname ="MuonSR";
    }
    if(sr==ElectronSR){
      labels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1","SR2_bin2", "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8"};
      EVhitname ="ElectronSR";
    }
    if(sr==ElectronMuonSR){
      labels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1","SR2_bin2", "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8"};
      EVhitname ="ElectronMuonSR";
    }
    
    
    if(sr==MuonSRQQ){
      labels = {"QMSR1_bin1","QMSR1_bin2","QMSR1_bin3","QMSR1_bin4","QMSR1_bin5","QMSR1_bin6","QMSR1_bin7","QMSR2_bin1", "QMSR2_bin2",  "QMSR3_bin1","QMSR3_bin2","QMSR3_bin3","QMSR3_bin4","QMSR3_bin5","QMSR3_bin6","QMSR3_bin7","QMSR3_bin8",  "QPSR1_bin1","QPSR1_bin2","QPSR1_bin3","QPSR1_bin4","QPSR1_bin5","QPSR1_bin6","QPSR1_bin7","QPSR2_bin1","QPSR2_bin2",  "QPSR3_bin1","QPSR3_bin2","QPSR3_bin3","QPSR3_bin4","QPSR3_bin5","QPSR3_bin6","QPSR3_bin7","QPSR3_bin8"};
      EVhitname ="MuonChargeSplitSR";
    }
    
    if(sr==ElectronSRQQ){
      labels = {"QMSR1_bin1","QMSR1_bin2","QMSR1_bin3","QMSR1_bin4","QMSR1_bin5","QMSR1_bin6","QMSR1_bin7","QMSR2_bin1","QMSR2_bin2",  "QMSR3_bin1","QMSR3_bin2","QMSR3_bin3","QMSR3_bin4","QMSR3_bin5","QMSR3_bin6","QMSR3_bin7","QMSR3_bin8", "QPSR1_bin1","QPSR1_bin2","QPSR1_bin3","QPSR1_bin4","QPSR1_bin5","QPSR1_bin6","QPSR1_bin7","QPSR2_bin1","QPSR2_bin2",  "QPSR3_bin1","QPSR3_bin2","QPSR3_bin3","QPSR3_bin4","QPSR3_bin5","QPSR3_bin6","QPSR3_bin7","QPSR3_bin8"};
      EVhitname ="ElectronChargeSplitSR";
    }
    if(sr==ElectronMuonSRQQ){
      labels = {"QMSR1_bin1","QMSR1_bin2","QMSR1_bin3","QMSR1_bin4","QMSR1_bin5","QMSR1_bin6","QMSR1_bin7","QMSR2_bin1","QMSR2_bin2",  "QMSR3_bin1","QMSR3_bin2","QMSR3_bin3","QMSR3_bin4","QMSR3_bin5","QMSR3_bin6","QMSR3_bin7","QMSR3_bin8" , "QPSR1_bin1","QPSR1_bin2","QPSR1_bin3","QPSR1_bin4","QPSR1_bin5","QPSR1_bin6","QPSR1_bin7","QPSR2_bin1","QPSR2_bin2",  "QPSR3_bin1","QPSR3_bin2","QPSR3_bin3","QPSR3_bin4","QPSR3_bin5","QPSR3_bin6","QPSR3_bin7","QPSR3_bin8"};
      EVhitname ="ElectronMuonChargeSplitSR";
    }

    if(sr==MuonSRBDT){
      labels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1", "SR2_bin2",  "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8"};
      EVhitname ="MuonSR";
    }
    if(sr==ElectronSRBDT){
      labels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1","SR2_bin2",  "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8"};
      EVhitname ="ElectronSR";
    }
    if(sr==ElectronMuonSRBDT){
      labels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1","SR2_bin2",  "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8"};
      EVhitname ="ElectronMuonSR";
    }


  }
  
  
  if(verbose_level==0){
    if(sr==CR){
      
      labels = { "TopAK8_CR","ZAK8_CR","WpWp_CR1","WpWp_CR2","WpWp_CRNP","WpWp_CRNP2","ZZ_CR","ZZLoose_CR","ZG_CR","WG_CR","WZ_CR","WG_Method2_CR","ZG_Method2_CR","WZ2_CR","WZB_CR","HighMassSR1_CR","HighMassSR2_CR","HighMassSR3_CR" ,"HighMass1Jet_CR","HighMassBJet_CR","HighMassNP_CR","ZNP_CR","SSPresel","SSVBFPresel"};
      
      
      EVhitname ="ControlRegions";
    }
    
    if(sr==SR1Tau ){
      vector<TString> JID = {"JetVVL","JetVL","JetL","JetM","JetT","JetVT","JetVVT"};
      vector<TString> ElID = {"ElVVL","ElVL","ElL","ElM","ElT","ElVT","ElVVT"};
      vector<TString> MuID = {"MuVL","MuL","MuM","MuT"};
      labels = {"SR1"};
      for(auto ij :  JID){
  for(auto iel : ElID){
    for(auto imu : MuID){
      TString TID=ij+"_"+iel+"_"+imu;
      labels.push_back(TID);
    }
  }
      }
      EVhitname= "SR1Tau";
    }
    
    if(sr==SR2Tau ){
      vector<TString> JID = {"JetVVL","JetVL","JetL","JetM","JetT","JetVT","JetVVT"};
      vector<TString> ElID = {"ElVVL","ElVL","ElL","ElM","ElT","ElVT","ElVVT"};
      vector<TString> MuID = {"MuVL","MuL","MuM","MuT"};
      labels = {"SR2"};
      for(auto ij :  JID){
  for(auto iel : ElID){
    for(auto imu : MuID){
      TString TID=ij+"_"+iel+"_"+imu;
      labels.push_back(TID);
    }
  }
      }
      EVhitname= "SR2Tau";
    }
    
    if(sr==SR3Tau ){
      vector<TString> JID = {"JetVVL","JetVL","JetL","JetM","JetT","JetVT","JetVVT"};
      vector<TString> ElID = {"ElVVL","ElVL","ElL","ElM","ElT","ElVT","ElVVT"};
      vector<TString> MuID = {"MuVL","MuL","MuM","MuT"};
      labels = {"SR3"};
      for(auto ij :  JID){
  for(auto iel : ElID){
    for(auto imu : MuID){
      TString TID=ij+"_"+iel+"_"+imu;
      labels.push_back(TID);
    }
  }
      }
      EVhitname= "SR3Tau";
    }
    
    if(sr==sigmm){
      labels = {"SSNoCut", "SSGen", "SSGen2", "SSMuMuTrig", "SSMuMuTrig2", "SSMuMuTrig2L", "SSMuMu", "SSMuMu_Pt", "SSMuMu_HEMVeto","SSMuMu_LepVeto", "SSMuMu_LLMass",  "SSMuMu_vTau","SSMuMu_Jet","SSMuMu_BJet", "SSMuMu_DiJet",  "SSMuMu_SR1","SSMuMu_SR1Fail", "SSMuMu_SR2","SSMuMu_SR3","SSMuMu_SR4","SSMuMu_SR5","SSMuMu_SR3Fail"};
      EVhitname ="SR_Summary";
    }
    
    if(sr==sigmm_17028){
      labels = {"SSNoCut", "SSGen","SSGen2", "SSMuMuTrig", "SSMuMuTrig2","SSMuMuTrig2L" , "SSMuMu","SSMuMu_Pt", "SSMuMu_HEMVeto","SSMuMu_LepVeto", "SSMuMu_LLMass", "SSMuMu_Jet", "SSMuMu_BJet", "SSMuMu_DiJet",  "SSMuMu_SR1","SSMuMu_SR1Fail","SSMuMu_SR3","SSMuMu_SR3Fail"};
      EVhitname ="SR_Summary_17028";
    }
    
    if(sr==sigee){
      labels = {"SSNoCut", "SSGen", "SSGen2", "SSEETrig", "SSEETrig2", "SSEETrig2L","SSEE", "SSEE_Pt","SSEE_LepVeto", "SSEE_HEMVeto","SSEE_LLMass",  "SSEE_vTau","SSEE_Jet","SSEE_BJet", "SSEE_DiJet",  "SSEE_SR1","SSEE_SR1Fail", "SSEE_SR2","SSEE_SR3","SSEE_SR4","SSEE_SR5","SSEE_SR3Fail"};
      EVhitname ="SR_Summary";
    }
    
    if(sr==sigee_17028){
      labels = {"SSNoCut", "SSGen","SSGen2", "SSEETrig", "SSEETrig2", "SSEETrig2L", "SSEE","SSEE_Pt", "SSEE_HEMVeto", "SSEE_LepVeto", "SSEE_LLMass", "SSEE_Jet", "SSEE_BJet", "SSEE_DiJet",  "SSEE_SR1","SSEE_SR1Fail","SSEE_SR3","SSEE_SR3Fail"};
      EVhitname ="SR_Summary_17028";
    }
    
    
    
    if(sr==sigem){
      labels = {"SSNoCut", "SSGen", "SSGen2", "SSEMuTrig", "SSEMuTrig2","SSEMuTrig2L","SSEMu", "SSEMu_Pt","SSEMu_LepVeto", "SSEMu_LLMass",  "SSEMu_vTau","SSEMu_Jet","SSEMu_BJet", "SSEMu_DiJet",  "SSEMu_SR1","SSEMu_SR1Fail", "SSEMu_SR2","SSEMu_SR3","SSEMu_SR4","SSEMu_SR5","SSEMu_SR3Fail"};
      EVhitname ="SR_Summary";
    }
    
    if(sr==sigem_17028){
      labels = {"SSNoCut", "SSGen","SSGen2", "SSEMuTrig", "SSEMuTrig2", "SSEMuTrig2L", "SSEMu","SSEMu_Pt", "SSEMu_LepVeto", "SSEMu_LLMass", "SSEMu_Jet", "SSEMu_BJet", "SSEMu_DiJet",  "SSEMu_SR1","SSEMu_SR1Fail","SSEMu_SR3","SSEMu_SR3Fail"};
      EVhitname ="SR_Summary_17028";
    }
    
  }

  if(labels.size() == 0) return;
  
  HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,EVhitname, event_weight, labels, label);

  return;
}      
 
void HNL_LeptonCore::FillEventCutflowPerMass(TString analysis_dir_name,SearchRegion sr, double event_weight, TString label,   TString massname){

  if(sr==SR3 ){

    vector<TString> labels = {
      "SR3_default",
      "SR3_jetsize",
      "SR3_jetpt",
      "SR3_W_DR_lep2",
      "SR3_lepPt1",
      "SR3_lepPt2",
      "SR3_Wmass_Low",
      "SR3_Wmass_High",
      "SR3_lljj",
      "SR3_Nmass_Low",
      "SR3_Nmass_High",
      "SR3_MET"};

    if(run_Debug)  cout << "FillEventCutflowAll : " << analysis_dir_name+"_SR3" << " " << label << endl;
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,"Massopt_"+massname, event_weight, labels, label);
  }



  if(sr== SR1 ){

    vector<TString> labels = {
      "SR1_default",
      "SR1_lepPt1",
      "SR1_lepPt2",
      "SR1_SDmass_Low",
      "SR1_SDmass_High",
      "SR1_Nmass_Low",
      "SR1_Nmass_High",
      "SR1_MET"};

    if(run_Debug)  cout << "FillEventCutflowAll : " << analysis_dir_name+"_SR1" << " " << label << endl;
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,"Massopt_"+massname, event_weight, labels, label);
  }


  return;

}

 
void HNL_LeptonCore::FillEventCutflowSR(TString analysis_dir_name,HNL_LeptonCore::SearchRegion sr, double event_weight, TString label){                                                            

  vector<TString> masses = {"100","125","200","250","300","400","500","600","700","800","900","1000","1100","1200","1300","1400","1500","1700","2000","2500","5000","20000"};
  
  

  if(sr== SR1){
    vector<TString> labels;
    for (unsigned int x=0; x < masses.size(); x++) labels.push_back("HNL_SR1_mn"+masses[x] );
    TString histname = "SR1_massbinned";  
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,histname, event_weight, labels, label);
  }

  if(sr== SR2){
    vector<TString> labels;
    for (unsigned int x=0; x < masses.size(); x++) labels.push_back("HNL_SR2_mn"+masses[x] );
    TString histname = "SR2_massbinned";
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,histname, event_weight, labels, label);
  }
  if(sr == SR3){
    vector<TString> labels;
    for (unsigned int x=0; x < masses.size(); x++) labels.push_back("HNL_SR3_mn"+masses[x] );
    TString histname = "SR3_massbinned";
    HNL_LeptonCore::FillEventCutflowAll(analysis_dir_name,histname, event_weight, labels, label);
    
  }
  
  return;
  
}  

void HNL_LeptonCore::FillFullTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D("FillEventFullType/"+histname, "", lables.size(), 0, lables.size(),  lables.size(), 0, lables.size());
    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetYaxis()->SetBinLabel(i+1,lables[i]);

    maphist_TH2D[histname] = this_hist;
  }

  this_hist->Fill(label1, label2, fabs(weight));

}


void HNL_LeptonCore::FillTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){

  TH2D *this_hist = GetHist2D(histname);
  if( !this_hist ){
    this_hist = new TH2D("CR2DCutflow/"+histname, "", lables.size(), 0, lables.size(),  lables.size(), 0, lables.size());
    this_hist->SetDirectory(NULL);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetXaxis()->SetBinLabel(i+1,lables[i]);
    for (unsigned int i=0 ; i < lables.size(); i++)  this_hist->GetYaxis()->SetBinLabel(i+1,lables[i]);

    maphist_TH2D[histname] = this_hist;
  }

  this_hist->Fill(label1, label2, fabs(weight));

}


void HNL_LeptonCore::FillCutFlow(bool IsCentral, TString suffix, TString histname, double ev_weight){
  
  if(IsCentral){
    
    FillHist(suffix+"/"+histname, 0., ev_weight, 1, 0., 1.);
    
  }
  
}



TString HNL_LeptonCore::GetPtBin(bool muon, double pt){
  TString pt_label="";
  
  if(pt< 20) pt_label = "_ptbin1"; 
  else if(pt < 50) pt_label = "_ptbin2"; 
  else if(pt < 100) pt_label = "_ptbin3";
  else pt_label = "_ptbin4";                                                               

  return pt_label;

}

/// FillMuonPlots Fills all kinamatics
void HNL_LeptonCore::FillMuonPlots(TString label , TString cut,  std::vector<Muon> muons, double w){

  for(auto imu: muons)   FillMuonKinematicPlots("muon"+label, cut, imu, w);
  
  return;
}

/// FillAllMuonPlots Fills all kinamatics in pt/eta bins                                                                                                      

void HNL_LeptonCore::FillAllMuonPlots(TString label , TString cut,  std::vector<Muon> muons, double w){

  for(unsigned int i=0; i <  muons.size(); i++){
    TString eta_label="";
    if(fabs(muons.at(i).Eta()) < 1.5) eta_label = "_BB";
    else eta_label = "_EC";
    
    TString pt_label=GetPtBin(true,muons.at(i).Pt());
    FillMuonKinematicPlots("muon"+label+"_"+pt_label, cut, muons.at(i), w);
    FillMuonKinematicPlots("muon"+label+eta_label, cut, muons.at(i), w);
    
  }

  return;
}


void HNL_LeptonCore::FillLeptonKinematicPlots(TString label , TString cut,  Lepton lep, double w){

  vector<Jet> JetAllColl = GetAllJets();

  int IdxMatchJet=-1;
  int IdxMatchAwayJet=-1;

  double mindR(999.);
  double maxDphi=-999;
  double PtRatioAwayJet(-999.);
  double Jet_Disc(-1);
 

  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){

    double dR=lep.DeltaR(JetAllColl.at(ij));
    if(dR<mindR){       mindR=dR; IdxMatchJet=ij;     }
    double dphi =fabs(TVector2::Phi_mpi_pi(lep.Phi()- JetAllColl.at(ij).Phi()));
    if(dphi > maxDphi) {maxDphi= dphi; IdxMatchAwayJet=ij;}
  }
  if(IdxMatchJet!=-1)     Jet_Disc       = JetAllColl.at(IdxMatchJet).GetTaggerResult(JetTagging::DeepJet);
  if(IdxMatchAwayJet!=-1) PtRatioAwayJet = JetAllColl.at(IdxMatchAwayJet).Pt()/ lep.Pt();

  TString label_lep = (lep.LeptonFlavour()==Lepton::ELECTRON) ? "Electron": "Muon";
  double Pt_Lep = (lep.Pt() > 2000) ? 1999 : lep.Pt();
 
  /// Kinematics
  FillHist( cut+ "/Lepton_Pt_"+label   , lep.Pt() , w, 5000, 0., 10000., label_lep+"p_{T} GeV");
  FillHist( cut+ "/Lepton_UncorrectedPt_"+label   , lep.UncorrectedPt() , w, 5000, 0., 10000., label_lep+"p_{T} GeV");
  FillHist( cut+ "/Lepton_Eta_"+label  , lep.Eta() , w, 60, -3., 3.,label_lep+"#eta");
  FillHist( cut+ "/Lepton_Phi_"+label  , lep.Phi() , w, 60, -3., 3.,label_lep+"#phi");
  double ptbins[11] = { 0., 10.,15., 20., 30., 40.,50., 100.,500. ,1000.,2000.};
  FillHist( cut+ "/Lepton_PtBinned_"+label   , Pt_Lep, w, 10, ptbins);

  // IP
  FillHist( cut+ "/Lepton_Dxy_"+label           , lep.fdXY() , w, 250,  0, 0.2, "dXY");
  FillHist( cut+ "/Lepton_Dz_"+label            , lep.fdZ() , w, 250, 0, 0.5, "dZ");
  FillHist( cut+ "/Lepton_LogDxy_"+label        , lep.LogdXY() , w, 300, -20,  10, "dXY");

  FillHist( cut+ "/Lepton_dXYSig_"+ label       , lep.LogdXYSig(), w, 100, -10,10);
  FillHist( cut+ "/Lepton_dZSig_"+ label        , lep.LogdZSig(),w, 100,-10,10);
  FillHist( cut+ "/Lepton_SIP3D_"+ label        , lep.SIP3D(),w, 100,0,10);
  FillHist( cut+ "/Lepton_IP3D_"+ label        , lep.IP3D(),w, 100,0,1);
 

  // Lep/Jet
  FillHist( cut+ "/Lepton_PtRatio_v2_"+label    , lep.CloseJet_Ptratio(), w, 500, 0., 2., "");
  FillHist( cut+ "/Lepton_PtRel_v2_"+label      , lep.CloseJet_Ptrel(), w, 200, 0., 50., "");

  FillHist( cut+ "/Lepton_PtRel_"+label         , JetLeptonPtRelLepAware( lep), w, 500, 0., 100., "");
  FillHist( cut+ "/Lepton_PtRelCorrL_"+label         , JetLeptonPtRelLepAware( lep,true,false), w, 500, 0., 100., "");
  FillHist( cut+ "/Lepton_PtRelCorrJ_"+label         , JetLeptonPtRelLepAware( lep,false,true), w, 500, 0., 100., "");
  FillHist( cut+ "/Lepton_PtRelCorrLJ_"+label         , JetLeptonPtRelLepAware( lep,true,true), w, 500, 0., 100., "");

  FillHist( cut+ "/Lepton_PtRatio_"+label       , JetLeptonPtRatioLepAware(lep), w, 500, 0., 2., "");
  FillHist( cut+ "/Lepton_PtRatioCorrL_"+label       , JetLeptonPtRatioLepAware(lep,true,false), w, 500, 0., 2., "");
  FillHist( cut+ "/Lepton_PtRatioCorrJ_"+label       , JetLeptonPtRatioLepAware(lep,false,true), w, 500, 0., 2., "");
  FillHist( cut+ "/Lepton_PtRatioCorrLJ_"+label       , JetLeptonPtRatioLepAware(lep,true,true), w, 500, 0., 2., "");

  if(IdxMatchJet != -1 ){
    FillHist( cut+ "/Lepton_CHFracCJ_"+label      , JetAllColl.at(IdxMatchJet).ChargedHadEnergyFraction(), w, 100, 0., 1., "");
    FillHist( cut+ "/Lepton_NEMFracCJ_"+label     , JetAllColl.at(IdxMatchJet).NeutralEmEnergyFraction(), w, 100, 0., 1., "");
    FillHist( cut+ "/Lepton_CEMFracCJ_"+label     , JetAllColl.at(IdxMatchJet).ChargedEmEnergyFraction(), w, 100, 0., 1., "");
    FillHist( cut+ "/Lepton_NFracCJ_"+label       , JetAllColl.at(IdxMatchJet).NeutralHadEnergyFraction(), w, 100, 0., 1., "");
    //  return mcCorr->GetBTaggingReweight_1a(jets, jtp, syst);

    FillHist( cut+ "/Lepton_JetDiscCJ_"+label     , Jet_Disc , w, 400, -2., 2., "");

    //FillHist( cut+ "/Lepton_JetDiscCJ_Corr_"+label     , Jet_Disc , w*JetSF, 400, -2., 2., "");
    FillHist( cut+ "/Lepton_PtRatioAwayJet_"+label, PtRatioAwayJet , w, 100, 0., 5., "");
    //FillHist( cut+ "/Lepton_JetDiscCJ_WeightCorr_"+label     , JetSF , w, 100, 0., 5., "");
  }

  if(lep.LeptonFlavour()==Lepton::ELECTRON){
    FillHist( cut+ "/Lepton_Conv_Mva_"+label  , lep.HNL_MVA_Conv(), w, 100, -1., 1., "MVA");
    FillHist( cut+ "/Lepton_Fake_Mva_"+label  , lep.HNL_MVA_Fake(), w, 100, -1., 1., "MVA");
    FillHist( cut+ "/Lepton_CF_Mva_"+label  , lep.HNL_MVA_CF(), w, 100, -1., 1., "MVA");

    FillHist( cut+ "/Lepton_CFv2_Mva_"+label  , lep.HNL_MVAv2_CF(), w, 100, -1., 1., "MVA");
    FillHist( cut+ "/Lepton_CFv3_Mva_"+label  , lep.HNL_MVAv3_CF(), w, 100, -1., 1., "MVA");
    FillHist( cut+ "/Lepton_CFv4_Mva_"+label  , lep.HNL_MVAv4_CF(), w, 100, -1., 1., "MVA");

    FillHist( cut+ "/Lepton_EDCFv2_Mva_"+label  , lep.HNL_MVAv2_ED_CF(), w, 100, -1., 1., "MVA");
    FillHist( cut+ "/Lepton_EDCFv3_Mva_"+label  , lep.HNL_MVAv3_ED_CF(), w, 100, -1., 1., "MVA");
    FillHist( cut+ "/Lepton_EDCFv4_Mva_"+label  , lep.HNL_MVAv4_ED_CF(), w, 100, -1., 1., "MVA");

    vector<TString> MVAList = lep.GetMVAList();
    for(auto mvaname : MVAList)     FillHist( cut+ "/Lepton_Map_"+mvaname+"_"+label  , lep.GetHNL_LepMVA(mvaname), w, 100, -1., 1., "MVA");
    
  }

  

  
  // Isolation 

  FillHist( cut+ "/Lepton_MiniReliso_"+label , lep.MiniRelIso() , w, 50, 0., 5., "R_{ISO} GeV");
  FillHist( cut+ "/Lepton_MiniReliso_ch_"+label , lep.MiniRelIsoCharged() , w, 50, 0., 1., "R_{ISO} GeV");
  FillHist( cut+ "/Lepton_MiniReliso_neu_"+label ,lep.MiniRelIsoNeutral() , w, 50, 0., 1., "R_{ISO} GeV");
  FillHist( cut+ "/Lepton_Reliso_el_"+label , lep.RelIso() , w, 50, 0., 0.4, "R_{ISO} GeV");
  FillHist( cut+ "/Lepton_MiniIsoChHad_"+label  , lep.MiniIsoChHad(), w , 100, 0., 20.);
  FillHist( cut+ "/Lepton_MiniIsoNhHad_"+label  , lep.MiniIsoNHad(), w , 100, 0., 20.);
  FillHist( cut+ "/Lepton_MiniIsoPhHad_"+label  , lep.MiniIsoPhHad(), w , 100, 0., 20.);
  FillHist( cut+ "/Lepton_RelMiniIsoCh_"+label  , lep.MiniRelIsoCharged(), w , 100, 0., 5);
  FillHist( cut+ "/Lepton_RelMiniIsoN_"+label  , lep.MiniRelIsoNeutral(), w , 100, 0., 10);


}

void HNL_LeptonCore::FillMuonKinematicPlots(TString label , TString cut,  Muon lep, double w){

  FillLeptonKinematicPlots(label, cut, Lepton(lep),w);

  FillHist( cut+ "/Muon_Mva_"+label  , lep.MVA(), w, 600, -1., 1., "MVA");
  FillHist( cut+ "/Muon_Pt_mva_"+label , lep.Pt() , lep.MVA(), fabs(w), 200, 0., 1000., 600, -1., 2.);
  FillHist( cut+ "/Muon_Chi2_"+label  , lep.Chi2(), w, 200,0., 20., "chi2");
  FillHist( cut+ "/Muon_Validhits_"+label  , lep.ValidMuonHits(), w, 100,0., 100., "");
  FillHist( cut+ "/Muon_Matched_stations_"+label  , lep.MatchedStations(), w, 10,0., 10., "");
  FillHist( cut+ "/Muon_Pixel_hits_"+label  , lep.PixelHits(), w, 10,0., 10., "");
  FillHist( cut+ "/Muon_Tracker_layers_"+label  , lep.TrackerLayers(), w, 50,0., 50., "");

  vector<TString> IDs ={"HNTightV2"};
  for (auto ID : IDs){
    if(lep.PassID(ID)) FillHist( cut+ "/Muon_Pass_"+ID+label  , 1, w, 4, 0., 4., "Pass " + ID);
    else FillHist( cut+ "/Muon_Pass_"+ID+label  , 0, w, 4, 0., 4., "Pass " + ID);
  }
  
  return;
}



void HNL_LeptonCore::FillElectronPlots(TString label , TString cut,  std::vector<Electron> ElectronColl, double w){

  for(auto iel : ElectronColl)   FillElectronKinematicPlots("Electron_"+label, cut, iel, w);
  
  return;
}

void HNL_LeptonCore::FillAllElectronPlots(TString label , TString cut,  std::vector<Electron> ElectronColl, double w){

  if(MCSample.Contains("Type")) w=1;

  FillHist( cut+ "/nelectrons"+label , size(ElectronColl) , w, 5, 0., 5., "n_{el}");

  for(unsigned int i=0; i < ElectronColl.size(); i++){

    TString eta_label="";
    if(fabs(ElectronColl.at(i).Eta()) < 1.5) eta_label = "_BB";
    else eta_label = "_EC";

    TString pt_label=GetPtBin(false,ElectronColl.at(i).Pt());

    FillElectronKinematicPlots("Electron_"+label, cut, ElectronColl[i], w); 
    FillElectronKinematicPlots("Electron_"+eta_label+"_"+pt_label+"_"+label, cut, ElectronColl[i], w); 
    
  }
  return;
}

void HNL_LeptonCore::FillElectronKinematicPlots(TString label , TString cut,  Electron lep, double w){

  FillLeptonKinematicPlots(label, cut, Lepton(lep),w);

  FillHist( cut+ "/Electron_SCEta_"+label  , lep.scEta() , w, 60, -3., 3.,"electron #eta");
  FillHist( cut+ "/Electron_SCPhi_"+label  , lep.scPhi() , w, 70, -3.5, 3.5,"electron #phi");

  /// POG MVA
  FillHist( cut+ "/Electron_NoIsoMva_"+label  , lep.MVANoIso(), w, 600, -1., 1., "MVA");
  FillHist( cut+ "/Electron_RelNoIsoMvaResponse_"+label  , lep.MVANoIsoResponse()/lep.Pt(), w, 160, -1.,1., "MVA");
  FillHist( cut+ "/Electron_NoIsoMvaResponse_"+label  , lep.MVANoIsoResponse(), w, 160, -8.,8., "MVA");
  FillHist( cut+ "/Electron_MvaIso_"+label  , lep.MVAIso(), w, 600, -1., 1., "MVA");
  FillHist( cut+ "/Electron_RelMvaIsoResponse_"+label  , lep.MVAIsoResponse()/lep.Pt(), w, 600, -1., 1., "MVA");
  FillHist( cut+ "/Electron_MvaIsoResponse_"+label  , lep.MVAIsoResponse(), w, 600, -8., 8., "MVA");

  /// CF MVA
  FillHist( cut+ "/Electron_EoverP_"+label  , lep.EOverP() , w, 400, -10, 100);
  FillHist( cut+ "/Electron_LogEoverP_"+label  , log(lep.EOverP()) , w, 400, -1, 10);
  
  FillHist( cut+ "/Electron_FBrem_"+label  , std::max(lep.FBrem(),-2.),  w, 200, -1., 1);
  FillHist( cut+ "/Electron_R9_"+label  , lep.R9(), w, 100, 0., 1);
  FillHist( cut+ "/Electron_dr03TkSumPt_"+label  , lep.dr03TkSumPt()/lep.UncorrPt(), w, 200, 0., 0.5, "");
  FillHist( cut+ "/Electron_E15_"+label  , lep.e15(), w, 1000, 0., 1000);
  FillHist( cut+ "/Electron_E25_"+label  , lep.e25(), w, 1000, 0., 1000);
  FillHist( cut+ "/Electron_E55_"+label  , lep.e55(), w, 1000, 0., 1000);
  FillHist( cut+ "/Electron_e2x5OverE5x5_"+label  , lep.e2x5OverE5x5(), w, 100, 0., 1);
  FillHist( cut+ "/Electron_e1x5OverE5x5_"+label  , lep.e1x5OverE5x5(), w, 100, 0., 1);
  FillHist( cut+ "/Electron_EtaWidth_"+label  , lep.EtaWidth(), w, 100, 0., 0.2);
  FillHist( cut+ "/Electron_PhiWidth_"+label  , lep.PhiWidth(), w, 200, 0., 0.2);

  FillHist( cut+ "/Electron_MissingHits_"+label  , lep.NMissingHits(), w, 8, 0., 8., "Missing Hits");
  FillHist( cut+ "/Electron_Full5x5_sigmaIetaIeta_"+label  , lep.Full5x5_sigmaIetaIeta(), w, 200, 0., 0.05, "");
  FillHist( cut+ "/Electron_PassConversionVeto_"+label  , lep.PassConversionVeto(), w, 2, 0., 2., "");
  FillHist( cut+ "/Electron_IsGsfCtfScPixChargeConsistent_"+label  , lep.IsGsfCtfScPixChargeConsistent(), w, 2, 0., 2., "");
  FillHist( cut+ "/Electron_IsGsfScPixChargeConsistent_"+label  , lep.IsGsfScPixChargeConsistent(), w, 2, 0., 2., "");
  FillHist( cut+ "/Electron_IsGsfCtfChargeConsistent_"+label  , lep.IsGsfCtfChargeConsistent(), w, 2, 0., 2., "");
  FillHist( cut+ "/Electron_dEtaSeed_"+label  , lep.dEtaSeed(), w, 200, -0.1, 0.1, "");
  FillHist( cut+ "/Electron_dPhiIn_"+label  , lep.dPhiIn(), w, 400, -0.5, 0.5, "");
  FillHist( cut+ "/Electron_dEtaIn_"+label  , lep.dEtaIn(), w, 400, -0.2, 0.2, "");
  FillHist( cut+ "/Electron_HoverE_"+label  , lep.HoverE(), w, 500, 0., 1., "");
  FillHist( cut+ "/Electron_TrkIso_"+label  , lep.TrkIso(), w, 1000, 0., 100, "");
  FillHist( cut+ "/Electron_isEcalDriven_"+label  , lep.isEcalDriven(), w, 2, 0., 2., "");
  FillHist( cut+ "/Electron_InvEminusInvP_"+label  , fabs(lep.InvEminusInvP()), w, 100., 0., 0.2);


  // COnv MVA
  FillHist( cut+ "/Electron_dr03HcalTowerSumEt_"+label  , lep.dr03HcalTowerSumEt(), w, 200, 0., 10., "");
  /// Extra
  FillHist( cut+ "/Electron_dr03EcalRecHitSumEt_"+label  , lep.dr03EcalRecHitSumEt(), w, 2000, 0., 100., "");
  FillHist( cut+ "/Electron_dr03HcalDepth1TowerSumEt_"+label  , lep.dr03HcalDepth1TowerSumEt(), w, 200, 0., 10., "");
  FillHist( cut+ "/Electron_ecalPFClusterIso_"+label  , lep.ecalPFClusterIso()/lep.UncorrPt(), w, 100, 0., 1., "");
  FillHist( cut+ "/Electron_hcalPFClusterIso_"+label  , lep.hcalPFClusterIso()/lep.UncorrPt(), w, 100, 0., 1., "");
 
  vector<TString> IDs ={"HNTightV2"};
  for (auto ID : IDs){
    if(lep.PassID(ID)) FillHist( cut+ "/Electron_Pass_"+ID+label  , 1, w, 4, 0., 4., "Pass " + ID);
    else FillHist( cut+ "/Electron_Pass_"+ID+label  , 0, w, 4, 0., 4., "Pass " + ID);
  }
 
  return;


}


