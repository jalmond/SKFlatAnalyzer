void runIDBDT_HNtypeIV2(TString Classifier ="BDTG" ,TString BkgType = "Fake", TString era="2017",TString channel="MuMu",  int signal_mode=0, TString NTrees="400", TString NormMode="EqualNumEvents", TString   MinNodeSize ="2.5", TString MaxDepth = "3", TString nCuts="300", TString BoostLearningRate="0.1", TString BaggedFrac="0.6", TString seed = "100",  int eta_mode= 0){
  
  int nTermWidth=50;
  
  for(int i=0; i < nTermWidth; i++)  cout << "=" ;   cout << endl;
  cout << "Running runIDBDT_HNtypeI{"+BkgType+"}: [Setup Options]" << endl;
  if(Classifier == "BDTA")cout << "** BDT ADABOOST *** " <<endl;
  else   if(Classifier == "BDTG")cout << "** BDT GRADBOOST *** " <<endl;
  else return;
  

  cout << "-- Era = " << era << endl;

  cout << "-- Channel = " << channel << endl;
  cout << "-- Signal Mode [0=LowMass, 1=All..] = " << signal_mode << endl;
  cout << "-- NTrees = " << NTrees << endl;
  cout << "-- NormMode = " << NormMode << endl;
  cout << "-- MinNodeSize = " << MinNodeSize << endl;
  cout << "-- MaxDepth = " << MaxDepth << endl;
  cout << "-- nCuts = " << nCuts << endl;
  cout << "-- Seed = " << seed << endl;
  if(Classifier == "BDTA") cout << "-- AdaBoostBeta= " << BoostLearningRate << endl;
  else {
    cout << "-- Shrinkage= " << BoostLearningRate << endl;
    cout << "-- BaggedFrac = " << BaggedFrac << endl;
  }
  
  for(int i=0; i < nTermWidth; i++)  cout << "="   ;  cout << endl;

  TString ClassTag = "_Shrinkage_"+BoostLearningRate+"_BaggedFrac_"+BaggedFrac;
  if(Classifier == "BDTA") ClassTag = "_AdaBoostBeta_"+BoostLearningRate;
  //BDTG_FakeSignalTypeI_EE_Signal_2017_NTrees1000_NormMode_EqualNumEvents_MinNodeSize_5.0_MaxDepth_4_nCuts_300_Shrinkage_0.5_BaggedFrac_0.8_Seed_100_BDT 

  TString  treeName = (channel == "MuMu")  ?  "Tree_mm" :  "Tree_ee";

  TString signal="";
  if(signal_mode == 1 )signal = "SignalHNLTop";
  else if(signal_mode == 2 )signal = "SignalHNLTopEC";//  pt bin1
  else return;

  cout << "signal File Name= " << signal << endl;
  for(int i=0; i < nTermWidth; i++)  cout << "-" ;   cout << endl;

  TString JobTag = Classifier +  "_"+ BkgType + signal+"TypeI_"+channel+"_"+signal+"_"+era+"_NTrees"+NTrees+"_NormMode_"+NormMode+"_MinNodeSize_"+MinNodeSize+"_MaxDepth_"+MaxDepth+"_nCuts_"+nCuts+ClassTag +"_Seed_"+seed+"_BDT";



  TMVA::gConfig().GetVariablePlotting().fNbins1D = 500; 

  const TString path = "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/HNDiLeptonWorskspace/InputFiles/MergedFiles/Run2UltraLegacy_v3/HNL_LepIDKinVarEtaBinned/"+era+"/";
 
  //TString signame  =  path+"HNL_LepIDKinVarEtaBinned_PromptTop.root";
  TString signame  =  path+"HNL_LepIDKinVarEtaBinned_PromptHNLTop.root";
  
  TFile* fsin = TFile::Open(signame);
  TFile* fbin = TFile::Open(path+"HNL_LepIDKinVarEtaBinned_"+BkgType+"Bkg.root");

  TTree* tree_signal = (TTree*)fsin->Get(treeName);
  TTree* tree_bkg    = (TTree*)fbin->Get(treeName);

  TString fout_name = (JobTag +".root");
  TFile* fout = TFile::Open(fout_name, "RECREATE");

  TMVA::Factory* factory = new TMVA::Factory(JobTag +"_TMVAClassification", fout,   "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");

  TMVA::DataLoader* data_loader = new TMVA::DataLoader("dataset");
  data_loader->AddVariable("Pt", "Pt", "units", 'F');

  data_loader->AddVariable("Eta", "Eta", "units", 'F');
  data_loader->AddVariable("MiniIsoChHad", "MiniIsoChHad", "units", 'F'); 
  data_loader->AddVariable("MiniIsoPhHad", "MiniIsoPhHad", "units", 'F');
  data_loader->AddVariable("MiniIsoNHad", "MiniIsoNHad", "units", 'F');  
  data_loader->AddVariable("RelMiniIsoCh", "RelMiniIsoCh", "units", 'F');
  data_loader->AddVariable("RelMiniIsoN", "RelMiniIsoN", "units", 'F');
  if(signal_mode >= 5  )data_loader->AddVariable("IsoChHad", "IsoChHad", "units", 'F'); 

  data_loader->AddVariable("Dxy",  "Dxy", "units", 'F');
  if(signal_mode >= 5  )data_loader->AddVariable("RelDxy",  "RelDxy", "units", 'F');
  if(signal_mode >= 5  )data_loader->AddVariable("DxySig",  "DxySig", "units", 'F');
  data_loader->AddVariable("Dz",  "Dz", "units", 'F');   
  if(signal_mode >= 5  )data_loader->AddVariable("RelDz",  "RelDz", "units", 'F');   
  if(signal_mode >= 5  )data_loader->AddVariable("DzSig",  "DzSig", "units", 'F');   
  data_loader->AddVariable("RelIso", "RelIso", "units", 'F'); 
  data_loader->AddVariable("IP3D", "IP3D", "units", 'F');       
  if(signal_mode >= 5  )data_loader->AddVariable("RelIP3D", "RelIP3D", "units", 'F');       
  data_loader->AddVariable("PtRatio",  "PtRatio", "units", 'F');
  data_loader->AddVariable("PtRel",  "PtRel", "units", 'F');
  data_loader->AddVariable("NEMFracCJ","NEMFracCJ", "units", 'F');
  data_loader->AddVariable("CHFracCJ","CHFracCJ", "units", 'F');
  data_loader->AddVariable("JetDiscCJ","JetDiscCJ","units", 'F');
  data_loader->AddVariable("NHFracCJ","NHFracCJ","units", 'F'); 


  if(channel == "MuMu") {
    data_loader->AddVariable("MVA",  "MVA", "units", 'F');
    data_loader->AddVariable("MuFracCJ","MuFracCJ","units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("Chi2", "Chi2", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("Validhits", "Validhits", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("Matched_stations", "Matched_stations", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("Pixel_hits",  "Pixel_hits", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("Minireliso",  "Minireliso", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("Tracker_layers",  "Tracker_layers", "units", 'I');
  }
  
  if(channel == "EE")   {
    if(signal_mode >= 5  )data_loader->AddVariable("IsoPhHad", "IsoPhHad", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("IsoNHad", "IsoNHad", "units", 'F'); 
    data_loader->AddVariable("CEMFracCJ","CEMFracCJ", "units", 'F');
    data_loader->AddVariable("MVA",  "MVA", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("RelMVA",  "RelMVA", "units", 'F');
    data_loader->AddVariable("MVAIso",  "MVAIso", "units", 'F');          
    if(signal_mode >= 5  )data_loader->AddVariable("RelMVAIso",  "RelMVAIso", "units", 'F');          
    if(signal_mode >= 5  )data_loader->AddVariable("Full5x5_sigmaIetaIeta",  "Full5x5_sigmaIetaIeta", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("dEtaSeed",  "dEtaSeed", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("dPhiIn",  "dPhiIn", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("HoverE",  "HoverE", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("EoverP",  "EoverP", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("FBrem",  "FBrem", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("R9",  "R9", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("TrkIso",  "TrkIso", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("dr03TkSumPt",  "dr03TkSumPt", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("dr03HcalTowerSumEt",  "dr03HcalTowerSumEt", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("dr03HcalDepth1TowerSumEt",  "dr03HcalDepth1TowerSumEt", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("dr03EcalRecHitSumEt",  "dr03EcalRecHitSumEt", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("e15",  "e15", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("e25",  "e25", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("e55",  "e55", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("e2x5OverE5x5",  "e2x5OverE5x5", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("e1x5OverE5x5",  "e1x5OverE5x5", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("EtaWidth",  "EtaWidth", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("PhiWidth",  "PhiWidth", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("InvEminusInvP", "InvEminusInvP", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("ecalPFClusterIso",  "ecalPFClusterIso", "units", 'F');
    if(signal_mode >= 5  )data_loader->AddVariable("hcalPFClusterIso", "hcalPFClusterIso", "units", 'F');

    //if(BkgType == "Conv" || BkgType == "CF" ){
    if(signal_mode >= 5  )data_loader->AddVariable("MissingHits",  "MissingHits", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("isEcalDriven", "isEcalDriven", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("PassConversionVeto",  "PassConversionVeto", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("IsGsfCtfScPixChargeConsistent",  "IsGsfCtfScPixChargeConsistent", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("IsGsfScPixChargeConsistent",  "IsGsfScPixChargeConsistent", "units", 'I');
    if(signal_mode >= 5  )data_loader->AddVariable("IsGsfCtfChargeConsistent",  "IsGsfCtfChargeConsistent", "units", 'I');
    //}
  }
  
  data_loader->AddSpectator("w_tot", "w_tot", "units", 'F');          

  data_loader->AddSignalTree(tree_signal, 1.0);

  data_loader->AddBackgroundTree(tree_bkg, 1.0);

  data_loader->SetWeightExpression("w_tot");  

  //==== Nj, Nb cut
  TCut cut_s = "";
  TCut cut_b = "";

  if(signal_mode == 2){
    cut_s = "Eta>1.5";
    cut_b = "Eta>1.5";
  }

  int n_train_signal = tree_signal->GetEntries(cut_s)/2 ;
  int n_train_back = tree_bkg->GetEntries(cut_b)/2 ;

  TString trainString = Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:SplitSeed="+seed+":NormMode="+NormMode+":V", n_train_signal, n_train_back, n_train_signal, n_train_back);
  data_loader->PrepareTrainingAndTestTree(cut_s, cut_b,  trainString);
  
  //==== Adaptive Boost

  TString classTag= (Classifier == "BDTA") ? "BDT" : "BDTG";
  TString s_BoostType=  (Classifier == "BDTA") ?":BoostType=AdaBoost" : ":BoostType=Grad";
  TString s_NTrees = ":NTrees="+NTrees;
  TString s_MinNodeSize = ":MinNodeSize="+MinNodeSize+"%";
  TString s_MaxDepth = ":MaxDepth="+MaxDepth;
  TString s_NCut=":nCuts="+nCuts;
  
  TString learnrate="";
  TString s_BaggedFrac = "";
     
  if(Classifier == "BDTA"){
    
    learnrate= ( BoostLearningRate != "-1") ? ":AdaBoostBeta="+BoostLearningRate : "";
    s_BaggedFrac="";
  }
  else{
    learnrate= ( BoostLearningRate != "-1") ? ":Shrinkage="+BoostLearningRate : "";
    s_BaggedFrac= (BaggedFrac != "-1") ?":UseBaggedBoost:BaggedSampleFraction="+BaggedFrac : "";

  }
  TString S_bookmethod = "!H:V:NTrees="+s_NTrees+s_MinNodeSize + s_MaxDepth + ":SeparationType=GiniIndex" + s_NCut + s_BoostType + learnrate+s_BaggedFrac;

  
  factory->BookMethod( data_loader,TMVA::Types::kBDT, classTag, S_bookmethod);

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();

  fout->Close();

  delete factory;
  delete data_loader;

  //TMVA::TMVAGui(fout_name);

  return;
}
