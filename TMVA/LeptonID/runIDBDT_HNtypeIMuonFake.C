void runIDBDT_HNtypeIMuonFake(TString Classifier ="BDTG" ,TString BkgType = "FakeBkg", TString era="2016",TString channel="MuMu",  int signal_mode=10,  int var_mode=0,  TString NTrees="400", TString NormMode="EqualNumEvents", TString   MinNodeSize ="2.5", TString MaxDepth = "3", TString nCuts="300", TString BoostLearningRate="0.1", TString BaggedFrac="0.6", TString seed = "100",  int eta_mode= 0){

  
  int nTermWidth=50;
  
  for(int i=0; i < nTermWidth; i++)  cout << "=" ;   cout << endl;
  cout << "Running runIDBDT_HNtypeI{"+BkgType+"}: [Setup Options]" << endl;
  if(Classifier == "BDTA")cout << "** BDT ADABOOST *** " <<endl;
  else   if(Classifier == "BDTG")cout << "** BDT GRADBOOST *** " <<endl;
  else return;
  
  TString version="version12";

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

  TString  treeName = (channel == "MuMu")  ?  "Tree_mm" :  "Tree_ee";


  TMVA::gConfig().GetVariablePlotting().fNbins1D = 500; 



  const TString path = "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_ULv3/SKFlatAnalyzer/HNDiLeptonWorskspace/InputFiles/MergedFiles/Run2UltraLegacy_v3/HNL_LeptonID_BDT_KinVar/"+era+"/";
  TString signame  =   path+"HNL_LeptonID_BDT_KinVar_FakeBkg_LFAll.root";
  TString bkgname  =   path+"HNL_LeptonID_BDT_KinVar_FakeBkg_HF.root";

  TString SigTag="SignalMuonFake";
  if(var_mode==0) SigTag="SignalMuonFake";
  else return;

 
  TString signal=SigTag;
  if(signal_mode == 1) {
    signal=SigTag+"_LFvsHF";
  }

  if(signal_mode == 2) {
    signal=SigTag+"_HFBvsHFC";
    signame =  path+"HNL_LeptonID_BDT_KinVar_FakeBkg_HFB.root";
    bkgname =  path+"HNL_LeptonID_BDT_KinVar_FakeBkg_HFC.root";
  }

  if(signal_mode == 3) {
    signal=SigTag+"_QCD_LFvsHF";
    signame =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_LF.root";
    bkgname =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_HF.root";
  }
  if(signal_mode == 4) {
    signal=SigTag+"_QCD_HFBvsHFC";
    signame =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_HFB.root";
    bkgname =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_HFC.root";
  }

  if(signal_mode == 5) {
    signal=SigTag+"_QCD_LFMother1";
    signame =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_LF__LFMother_1.root";
    bkgname =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_LF__LFMother_not1.root";
  }
  if(signal_mode == 6) {
    signal=SigTag+"_QCD_LFMother2";
    signame =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_LF__LFMother_2.root";
    bkgname =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_LF__LFMother_not2.root";
  }
  if(signal_mode == 7) {
    signal=SigTag+"_QCD_LFMother3";
    signame =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_LF__LFMother_3.root";
    bkgname =  path+"HNL_LeptonID_BDT_KinVar_TypeFakeBkg_LF__LFMother_not3.root";
  }


  cout << "signal File Name= " << signal << endl;
  for(int i=0; i < nTermWidth; i++)  cout << "-" ;   cout << endl;

  TString JobTag = Classifier +"_"+version+"_Muon"+ BkgType +"_TypeI_"+channel+"_"+signal+"_"+era+"_NTrees"+NTrees+"_NormMode_"+NormMode+"_MinNodeSize_"+MinNodeSize+"_MaxDepth_"+MaxDepth+"_nCuts_"+nCuts+ClassTag +"_Seed_"+seed+"_BDT";



  
  TFile* fsin = TFile::Open(signame);
  TFile* fbin = TFile::Open(bkgname);

  TTree* tree_signal = (TTree*)fsin->Get(treeName);
  TTree* tree_bkg    = (TTree*)fbin->Get(treeName);

  TString fout_name = (JobTag +".root");
  TFile* fout = TFile::Open(fout_name, "RECREATE");

  TMVA::Factory* factory = new TMVA::Factory(JobTag +"_TMVAClassification", fout,   "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");

  TMVA::DataLoader* data_loader = new TMVA::DataLoader("dataset");
  // Kinematics
  //  data_loader->AddVariable("Pt", "Pt", "units", 'F');
  data_loader->AddVariable("Eta", "Eta", "units", 'F');
  data_loader->AddVariable("MiniIsoChHad", "MiniIsoChHad", "units", 'F'); 
  data_loader->AddVariable("MiniIsoPhHad", "MiniIsoPhHad", "units", 'F');
  data_loader->AddVariable("MiniIsoNHad", "MiniIsoNHad", "units", 'F');  
  data_loader->AddVariable("RelMiniIsoCh", "RelMiniIsoCh", "units", 'F');
  data_loader->AddVariable("RelMiniIsoN", "RelMiniIsoN", "units", 'F');
  data_loader->AddVariable("RelIso", "RelIso", "units", 'F');
  data_loader->AddVariable("Dxy",  "Dxy", "units", 'F');
  data_loader->AddVariable("DxySig",  "DxySig", "units", 'F');
  data_loader->AddVariable("Dz",  "Dz", "units", 'F');   
  data_loader->AddVariable("DzSig",  "DzSig", "units", 'F');   
  data_loader->AddVariable("IP3D", "IP3D", "units", 'F');       
  data_loader->AddVariable("PtRatio",  "PtRatio", "units", 'F');
  data_loader->AddVariable("PtRel",  "PtRel", "units", 'F');
  data_loader->AddVariable("JetDiscCJ","JetDiscCJ","units", 'F');
  data_loader->AddVariable("JetDiscCJCvsB","JetDiscCJCvsB","units", 'F');
  data_loader->AddVariable("JetDiscCJCvsL","JetDiscCJCvsL","units", 'F');
  data_loader->AddVariable("NEMFracCJ","NEMFracCJ", "units", 'F');
  data_loader->AddVariable("CHFracCJ","CHFracCJ", "units", 'F');
  data_loader->AddVariable("NHFracCJ","NHFracCJ","units", 'F'); 
  data_loader->AddVariable("MuFracCJ","MuFracCJ","units", 'F');
  data_loader->AddVariable("MVA",  "MVA", "units", 'F');
  data_loader->AddVariable("Chi2", "Chi2", "units", 'F');
  data_loader->AddVariable("Validhits", "Validhits", "units", 'F');
  data_loader->AddVariable("Matched_stations", "Matched_stations", "units", 'F');
  data_loader->AddSpectator("w_id_tot", "w_id_tot", "units", 'F');          

  data_loader->AddSignalTree(tree_signal, 1.0);

  data_loader->AddBackgroundTree(tree_bkg, 1.0);

  data_loader->SetWeightExpression("w_id_tot");  

  //==== Nj, Nb cut
  TCut cut_s = "";
  TCut cut_b = "";

  if(var_mode==1){
    cut_s = "Eta<1.5";
    cut_b = "Eta<1.5";
  }
  if(var_mode==2){
    cut_s = "Eta>1.5&&Eta<2.5";
    cut_b = "Eta>1.5&&Eta<2.5";
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
