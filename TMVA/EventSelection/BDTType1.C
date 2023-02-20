void BDTType1(int run_mode=0, TString era="2016")
{


  //TMVA::gConfig().GetVariablePlotting().fNbinsMVAoutput = 200;
  TMVA::gConfig().GetVariablePlotting().fNbins1D = 500; 
  
  //2018 
  const TString path = "/data6/Users/jalmond/SKFlatOutput/Run2UltraLegacy_v3/HNL_SR3KinVar/"+era+"/";
  
  TFile* fsin = TFile::Open(path+"Signal/HNL_SR3KinVar_MC_SkimTree_HNL_SS_MuMu_BDT_TRAIN_DYTypeI_NLO_DF_M100.root");
  
  TFile* fbin = TFile::Open(path+"HNL_SR3KinVar_Bkg.root");
  
  
  TTree* tree_signal = (TTree*)fsin->Get("Tree_mm");
  TTree* tree_bkg    = (TTree*)fbin->Get("Tree_mm");

  TString fout_name = ("BDT_Type1_MN100_MM.root");

  TFile* fout = TFile::Open(fout_name, "RECREATE");

  TMVA::Factory* factory = new TMVA::Factory("TMVAClassification", fout,
                                             "!V:!Silent:Color:DrawProgressBar:Transformations=I:AnalysisType=Classification");
  
  //TString name_data_loader;
  //if(!chk_pre_kin) name_data_loader = Form("%dJets", n_jet);
  //else name_data_loader = Form("Pre_Kin_%dJets", n_jet);
  
  TMVA::DataLoader* data_loader = new TMVA::DataLoader("dataset");
  //data_loader->AddVariable( "weight",       "weight",       "units", 'F');
  //data_loader->AddVariable( "n_jets",       "n_jets",       "units", 'I');

  //(usejets){
  data_loader->AddVariable("Nj", "Nj", "units", 'I'); 
  data_loader->AddVariable("Nvbfj", "Nvbfj", "units", 'I');
  
  
  data_loader->AddVariable("Ptl1", "Ptl1", "units", 'F');   
  data_loader->AddVariable("Ptl2", "Ptl2", "units", 'F');   
  /*
  data_loader->AddVariable("Ptj1", "Ptj1", "units", 'F');   
  data_loader->AddVariable("Ptj2", "Ptj2", "units", 'F');   
  data_loader->AddVariable("Ptj3", "Ptj3", "units", 'F');   
  data_loader->AddVariable("MET", "MET", "units", 'F');      
  data_loader->AddVariable("MET2ST", "MET2ST", "units", 'F');
  data_loader->AddVariable("HT", "HT", "units", 'F');            
  data_loader->AddVariable("LT", "LT", "units", 'F');            
  data_loader->AddVariable("HTLT", "HTLT", "units", 'F');      
  data_loader->AddVariable("HTLT1", "HTLT1", "units", 'F');   
  data_loader->AddVariable("HTLT2", "HTLT2", "units", 'F');   
  data_loader->AddVariable("MET2HT", "MET2HT", "units", 'F');
  data_loader->AddVariable("dEtall", "dEtall", "units", 'F');
  data_loader->AddVariable("dRll", "dRll", "units", 'F');      
  data_loader->AddVariable("dRjj12", "dRjj12", "units", 'F');
  data_loader->AddVariable("dRjj23", "dRjj23", "units", 'F');
  data_loader->AddVariable("dRjj13", "dRjj13", "units", 'F');
  data_loader->AddVariable("dRlj11", "dRlj11", "units", 'F');
  data_loader->AddVariable("dRlj12", "dRlj12", "units", 'F');
  data_loader->AddVariable("dRlj13", "dRlj13", "units", 'F');
  data_loader->AddVariable("dRlj21", "dRlj21", "units", 'F');
  data_loader->AddVariable("dRlj22", "dRlj22", "units", 'F');
  data_loader->AddVariable("dRlj23", "dRlj23", "units", 'F');
  data_loader->AddVariable("MSSSF", "MSSSF", "units", 'F');   
  data_loader->AddVariable("Mlj11", "Mlj11", "units", 'F');   
  data_loader->AddVariable("Mlj12", "Mlj12", "units", 'F');   
  data_loader->AddVariable("Mlj13", "Mlj13", "units", 'F');   
  data_loader->AddVariable("Mlj21", "Mlj21", "units", 'F');   
  data_loader->AddVariable("Mlj22", "Mlj22", "units", 'F');   
  data_loader->AddVariable("Mlj23", "Mlj23", "units", 'F');   
  data_loader->AddVariable("MTvl1", "MTvl1", "units", 'F');   
  data_loader->AddVariable("MTvl2", "MTvl2", "units", 'F');   
  data_loader->AddVariable("Mllj1", "Mllj1", "units", 'F');   
  data_loader->AddVariable("Mllj2", "Mllj2", "units", 'F');   
  data_loader->AddVariable("Mllj3", "Mllj3", "units", 'F');   
  data_loader->AddVariable("Mllj4", "Mllj4", "units", 'F');   
  data_loader->AddVariable("Mlljj12", "Mlljj12", "units", 'F'); 
  data_loader->AddVariable("Mlljj13", "Mlljj13", "units", 'F'); 
  data_loader->AddVariable("Mlljj14", "Mlljj14", "units", 'F'); 
  data_loader->AddVariable("Mlljj23", "Mlljj23", "units", 'F'); 
  data_loader->AddVariable("Mlljj24", "Mlljj24", "units", 'F'); 
  data_loader->AddVariable("Mlljj34", "Mlljj34", "units", 'F'); 
  data_loader->AddVariable("Mljj112", "Mljj112", "units", 'F'); 
  data_loader->AddVariable("Mljj113", "Mljj113", "units", 'F'); 
  data_loader->AddVariable("Mljj114", "Mljj114", "units", 'F'); 
  data_loader->AddVariable("Mljj123", "Mljj123", "units", 'F'); 
  data_loader->AddVariable("Mljj124", "Mljj124", "units", 'F'); 
  data_loader->AddVariable("Mljj134", "Mljj134", "units", 'F'); 
  data_loader->AddVariable("Mljj212", "Mljj212", "units", 'F'); 
  data_loader->AddVariable("Mljj213", "Mljj213", "units", 'F'); 
  data_loader->AddVariable("Mljj214", "Mljj214", "units", 'F'); 
  data_loader->AddVariable("Mljj223", "Mljj223", "units", 'F'); 
  data_loader->AddVariable("Mljj224", "Mljj224", "units", 'F'); 
  data_loader->AddVariable("Mljj234", "Mljj234", "units", 'F'); 
  data_loader->AddVariable("Mjj12", "Mjj12", "units", 'F');       
  data_loader->AddVariable("Mjj13", "Mjj13", "units", 'F');       
  data_loader->AddVariable("Mjj14", "Mjj14", "units", 'F');       
  data_loader->AddVariable("Mjj23", "Mjj23", "units", 'F');       
  data_loader->AddVariable("Mjj24", "Mjj24", "units", 'F');       
  data_loader->AddVariable("Mjj34", "Mjj34", "units", 'F');       
  data_loader->AddVariable("M_W2_jj", "M_W2_jj", "units", 'F'); 
  data_loader->AddVariable("M_W1_lljj", "M_W1_lljj", "units", 'F');
  data_loader->AddVariable("M_N1_l1jj", "M_N1_l1jj", "units", 'F');
  data_loader->AddVariable("M_N2_l2jj", "M_N2_l2jj", "units", 'F');
  */
  
  data_loader->AddSpectator("w_tot", "w_tot", "units", 'F');          




  data_loader->AddSignalTree(tree_signal, 1.);
  data_loader->AddBackgroundTree(tree_bkg, 1.0);
  data_loader->SetWeightExpression("w_tot");  
  
  TCut cut_s = "";
  TCut cut_b = "";
  
  if(run_mode==1 ){
    cut_s = "Njets<2";
    cut_b = "Njets<2";
  }
  if(run_mode==2 ){
    cut_s = "Njets>=2";
    cut_b = "Njets>=2";
  }

  int n_train_signal = tree_signal->GetEntries(cut_s)/2;
  int n_train_back = tree_bkg->GetEntries(cut_b)/2;




  data_loader->PrepareTrainingAndTestTree(cut_s, cut_b,
                                          Form("nTrain_Signal=%d:nTrain_Background=%d:nTest_Signal=%d:nTest_Background=%d:SplitMode=Random:NormMode=NumEvents:V", n_train_signal, n_train_back, n_train_signal, n_train_back));

  // //Adaptive Boost
  // factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDT",
  //       "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=100");

  // Boosted Decision Trees with adaptive boosting
  factory->BookMethod( data_loader,TMVA::Types::kBDT, "BDT",
		       "!H:!V:NTrees=850:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=100:PruneMethod=NoPruning" );
  
    
  //Gradient Boost
  //factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDTG",//
  //	      "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=100:MaxDepth=2" );


  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();

  fout->Close();

  delete factory;
  delete data_loader;

  //TMVA::TMVAGui(fout_name);

  return;
}
