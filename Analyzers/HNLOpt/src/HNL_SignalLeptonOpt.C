#include "HNL_SignalLeptonOpt.h"

void HNL_SignalLeptonOpt::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  
  //SetupIDMVAReaderDefault(); /// Not needed for BDT skim
  
  SetupEventMVAReader("V2",true,false,false); 

  RunHighPt= HasFlag("RunHighPt");

}


void HNL_SignalLeptonOpt::executeEvent(){

  OutCutFlow("CutFlow_All_Events", 1);


  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNLOpt");
    
  //cout << "########################################################" << endl;


  std::vector<Electron>   ElectronCollV = GetElectrons(param_signal.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param_signal.Muon_Veto_ID, 5., 2.4);
  std::vector<Lepton *>   LepsVeto      = MakeLeptonPointerVector(MuonCollV,ElectronCollV,param_signal);


  if(!SameCharge(LepsVeto)) return;

  std::vector<Jet> JetColl                        = GetHNLJets("Tight",param_signal);
  std::vector<FatJet> FatjetColl                  = GetHNLAK8Jets("HNL_PNL",param_signal);

  Event ev = GetEvent();
  Particle METv = GetMiniAODvMET("PuppiT1xyULCorr");


  ///// Initialise non lepton obj
  std::vector<Tau> TauColl                        = SelectTaus     (LepsVeto,param_signal.Tau_Veto_ID,20., 2.3);
  std::vector<Jet> AllJetColl                     = GetHNLJets("NoCut_Eta3",param_signal);
  std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param_signal);
  std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param_signal);
  std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param_signal);


  
  if(!SameCharge(ElectronCollV)) return;
  if(MuonCollV.size() > 0) return;
  
  //// 
  vector<TString> ElectronsIDs = {};
  
  
  vector<TString> vTrig = {"LTrig_"};//"LooseTrig_"};//,"TightTrig_"};
  vector<TString> vConv = {"ConvBConvEC_"};//,"ConvBConvEC_","ConvEC_"};
  vector<TString> vCC = {"CCBCCEC_","CCEC_"};//,"CCEC_",""};

    
  /////// SCANS FOR ID
  
  if(HasFlag("ELID_ULScan")){
    vector<TString> List1a = {"_CV1_","_CV2_","_CV3_","_CV4_","_CV5_"} ;
    vector<TString> List1b = {"_CV1_","_CV2_","_CV3_","_CV4_","_CV5_","_CV6_","_CV7_","_CV8_","_CV9_","_CV10_"};
    vector<TString> List1;
    if(GetYearString() == "2016") List1=List1a;
    else List1=List1b;

    vector<TString> List2 = {"_NP1_","_NP2_","_NP3_","_NP4_","_NP5_","_NP6_","_NP7_","_NP8_","_NP9_"};
    vector<TString> List3 = {"","_LFNP1_","_LFNP2_"};
    vector<TString> List4 = {"_CF1_","_CF2_","_CF3_","_CF4_","_CF5_"};
    for(auto id1 : List1){
      for(auto id2 : List2){
        for(auto id3 : List3){
          for(auto id4 : List4){
	    ElectronsIDs.push_back("HNL_ULID_Ver"+GetYearString()+"_"+id1+id2+id3+id4);
	  }
	}
      }
    }
  }
  if(HasFlag("ELID_FinalScan")){
    
    vector<TString> List1 = {"_CONV1_","_CONV2_","_CONV3_","_CONV4_","_CONV5_","_CONV6_","_CONV7_","_CONV8_","_CONV9_","_CONV10_","_CONV11_","_CONV12_","_CONV13_","_CONV14_"};
    vector<TString> List2 = {"_BNP1_","_BNP2_","_BNP3_"};
    vector<TString> List3 = {"_ENP1_","_ENP2_","_ENP3_","_ENP4_"};
    vector<TString> List4 = {"_BCF1_","_BCF2_","_BCF3_","_BCF4_"};
    vector<TString> List5 = {"_ECF1_","_ECF2_","_ECF3","_ECF4"};
    
    for(auto id1 : List1){
      for(auto id2 : List2){
	for(auto id3 : List3){
	  for(auto id4 : List4){
	    for(auto id5 : List5){
	      ElectronsIDs.push_back("HNL_ULID_FINAL_Opt_"+id1+id2+id3+id4+id5);
	    }
	  }
	}
      }
    }
  }

  if(HasFlag("ELID_FullScan")){
    
    vector<TString> vMVANP;
    //// First Include 1D cuts                                                                                                                                                                                                   
    for(int imva=0 ; imva < 7 ; imva++){
      double mva_d=  0.2 + double(imva)*.05;
      TString mvaTS= DoubleToString(mva_d);
      TString mvaTS2= DoubleToString(mva_d+0.05);
      TString mvaTS3= DoubleToString(mva_d+0.1);
      vMVANP.push_back("MVABNP"+mvaTS+"_"+"MVAENP"+mvaTS+"_");
      vMVANP.push_back("MVABNP"+mvaTS+"_"+"MVAENP"+mvaTS2+"_");
      vMVANP.push_back("MVABNP"+mvaTS+"_"+"MVAENP"+mvaTS3+"_");
    }
    
    vector<TString> vMVACF;
    
    for(int imva=0 ; imva < 11 ; imva++){
      double mva_d=  0.2 + double(imva)*.05;
      TString mvaTS= DoubleToString(mva_d);
      TString mvaTS2= DoubleToString(mva_d-0.1);
      vMVACF.push_back("MVABCF"+mvaTS+"_"+"MVAECF"+mvaTS+"_");
      vMVACF.push_back("MVABCF"+mvaTS+"_"+"MVAECF"+mvaTS2+"_");
    }
    for(auto id1 : vMVANP){
      for(auto id2 : vMVACF){
	ElectronsIDs.push_back("HNLUL_LTrig_ScanCFNP"+GetYearString()+"_"+id1+id2);
      }
    }
  }
  if(HasFlag("ELID_FullScan_Region")){

    vector<TString> vMVANPEC;
    //// First Include 1D cuts                                                                                                                                                     
    for(int imva=0 ; imva < 76 ; imva++){
      double mva_d=  -1 + double(imva)*.02;
      TString mvaTS= DoubleToString(mva_d);
      vMVANPEC.push_back("MVAENP"+mvaTS+"_");
    }
    
    for(auto id1 : vMVANPEC)      ElectronsIDs.push_back("HNLUL_LTrig_EndcapScanNP_"+id1);

    vector<TString> vMVANPBB;
    for(int imva=0 ; imva < 76 ; imva++){
      double mva_d=  -1 + double(imva)*.02;
      TString mvaTS= DoubleToString(mva_d);
      vMVANPBB.push_back("MVABNP"+mvaTS+"_");
    }
    
    for(auto id1 : vMVANPBB) ElectronsIDs.push_back("HNLUL_LTrig_BarrelScanNP_"+id1);

    
    vector<TString> vMVACFEC;
    //// First Include 1D cuts                                                                                                                                                                                                   
    for(int imva=0 ; imva < 91 ; imva++){
      double mva_d=  -1 + double(imva)*.02;
      TString mvaTS= DoubleToString(mva_d);
      vMVACFEC.push_back("MVAECF"+mvaTS+"_");
    }
    
    for(auto id1 : vMVACFEC)     ElectronsIDs.push_back("HNLUL_LTrig_EndcapScanCF_"+id1);

    vector<TString> vMVACFBB;
    for(int imva=0 ; imva < 91 ; imva++){
      double mva_d=  -1 + double(imva)*.02;
      TString mvaTS= DoubleToString(mva_d);
      vMVACFBB.push_back("MVABCF"+mvaTS+"_");
    }
    
    for(auto id1 : vMVACFBB)         ElectronsIDs.push_back("HNLUL_LTrig_BarrelScanCF_"+id1);
    



    vector<TString> vMVACFEC2D;
    for(int imva=0 ; imva < 15 ; imva++){
      double mva_d=  0 + double(imva)*.05;
      TString mvaTS= DoubleToString(mva_d);

      for(int imva2=0 ; imva2 < 15 ; imva2++){
	double mva_d2=  0 + double(imva2)*.05;
	TString mvaTS2= DoubleToString(mva_d2);
	
	vMVACFEC2D.push_back("MVAECF1"+mvaTS+"_"+"MVAECF2"+mvaTS2+"_");
      }
    }

    for(auto id1 : vMVACFEC2D)     ElectronsIDs.push_back("HNLUL_LTrig_EndcapScanCF_"+id1);
    

    
    vector<TString> vMVACFBB2D;
    for(int imva=0 ; imva < 15 ; imva++){
      double mva_d=  0 + double(imva)*.05;
      TString mvaTS= DoubleToString(mva_d);

      for(int imva2=0 ; imva2 < 15 ; imva2++){
        double mva_d2=  0 + double(imva2)*.05;
        TString mvaTS2= DoubleToString(mva_d2);

        vMVACFBB2D.push_back("MVABCF1"+mvaTS+"_"+"MVABCF2"+mvaTS2+"_");
      }
    }
    
    for(auto id1 : vMVACFBB2D)     ElectronsIDs.push_back("HNLUL_LTrig_BarrelScanCF_"+id1);



   
  }



  if(HasFlag("ELID_POGScan")){
    vector<TString> BarrelIDs = {"BBv1","BBv2","BBv3","BBv4","BBv5","BBv6","BBv7","BBv8","BBv9","BBv10","BBv11","BBv12","BBv13","BBv14","BBv15","BBv16","BBv17","BBv18","BBv19","BBv20","BBv21","BBv22","BBv23","BBv24","BBv25","BBv26","BBv27","BBv28","BBv29","BBv30","BBv31","BBv32","BBv33","BBv34","BBv35","BBv36","BBv37","BBv38","BBv39"};
    vector<TString> EndcapIDs = {"ECv1","ECv2","ECv3","ECv4","ECv5","ECv6","ECv7","ECv8","ECv9","ECv10","ECv11","ECv12","ECv13","ECv14","ECv15","ECv16","ECv17","ECv18","ECv19","ECv20","ECv21","ECv22","ECv23","ECv24","ECv25","ECv26","ECv27","ECv28","ECv29","ECv30","ECv31","ECv32","ECv33","ECv34","ECv35","ECv36","ECv37","ECv38","ECv39","ECv40","ECv41","ECv42","ECv43","ECv44","ECv45","ECv46","ECv47","ECv48","ECv49","ECv50","ECv51","ECv52","ECv53","ECv54"};
    
    for(auto id1 : BarrelIDs) {
      for(auto id2 : EndcapIDs) {
	ElectronsIDs.push_back("FINAL_BBID"+id1+"_ECID"+id2);
      }
    }
  }

  //////// STANDARD IDS 
  
  ElectronsIDs.push_back("passPOGMedium");
  ElectronsIDs.push_back("passPOGTight");
  ElectronsIDs.push_back("HNTightV2");
  ElectronsIDs.push_back("passHEEPID");
  ElectronsIDs.push_back("HNHEEPID");
  ElectronsIDs.push_back("passHEEPID2018Prompt");
  ElectronsIDs.push_back("HNTight_17028");
  ElectronsIDs.push_back("SUSYTight");
  ElectronsIDs.push_back("HN2016MVA");
  ElectronsIDs.push_back("HN2016MVA2");
  ElectronsIDs.push_back("HN2016MVA2CC");
  ElectronsIDs.push_back("HN2016POG");
  ElectronsIDs.push_back("HN2016POGCC");
  ElectronsIDs.push_back("HNL_ULID_OPT_POG1");
  ElectronsIDs.push_back("HNL_ULID_OPT_POG2");
  ElectronsIDs.push_back("HNL_ULID_OPT_POG3");
  ElectronsIDs.push_back("HNL_ULID_OPT_POG4");
  ElectronsIDs.push_back("HNL_ULID_OPT_POG5");
  ElectronsIDs.push_back("HNLOpt_UL_FINAL_BBIDBBv32_ECIDECv31");
  ElectronsIDs.push_back("HNL_Peking_2016");
  ElectronsIDs.push_back("HNL_Peking_2017");
  ElectronsIDs.push_back("passMVAID_noIso_WP90Opt");
  ElectronsIDs.push_back("passMVAID_noIso_WP80Opt");
  ElectronsIDs.push_back("passMVAID_noIso_WP90");
  ElectronsIDs.push_back("passMVAID_noIso_WP80");
  ElectronsIDs.push_back("passMVAID_Iso_WP90");
  ElectronsIDs.push_back("passMVAID_Iso_WP80");

  /////// ULIDs for HNL 
  ElectronsIDs.push_back("HNL_ULID_Run2");
  ElectronsIDs.push_back("HNL_ULID_2017"); 
  ElectronsIDs.push_back("HNL_ULID_2018");
  
  
  ev.SetMVA("EE",100, EvaluateEventMVA("100", "Incl", "300", "850", EE, LepsVeto, ev, METv, param_signal));
  ev.SetMVA("EE",400, EvaluateEventMVA("400", "Incl", "300", "850", EE, LepsVeto, ev, METv, param_signal));
  ev.SetMVA("EE",500, EvaluateEventMVA("500", "Incl", "300", "850", EE, LepsVeto, ev, METv, param_signal));
  

  if(_jentry%10000==0) cout << "ElectronsIDs size = " << ElectronsIDs.size() << endl;
  if(_jentry== 1000){
    cout << "ElectronsIDs size = " << ElectronsIDs.size() << endl;
    for(auto id : ElectronsIDs)   cout << id << endl; 
  }
  
  
  for(auto id : ElectronsIDs){
    
    //cout << id << endl;
    if (!id.Contains("HNLUL")){
      
      param_signal.Name = param_signal.DefName + "_"+ id;
      param_signal.SRConfig  = "";
      
      param_signal.Electron_Tight_ID=id;
      param_signal.Electron_FR_ID = id;
    }
    else{
      
      param_signal.Name = param_signal.DefName + "_ElOpt_"+ id;
      param_signal.SRConfig  = "";
      
      param_signal.Electron_Tight_ID="ElOpt_"+id;
      param_signal.Electron_FR_ID = "ElOptLoose_"+id;
      
    }
    param_signal.Muon_Tight_ID = "HNTightV2";
    param_signal.Muon_FR_ID = "HNLooseV1";
    
    RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetColl,ev);
    
    param_signal.Name = param_signal.DefName;
    param_signal.SRConfig  = "";
  }
  
  return ;
}

void HNL_SignalLeptonOpt::RunULAnalysis(AnalyzerParameter param, vector<Electron> ElectronCollV, vector<Muon> MuonCollV, std::vector<Tau> TauColl,  std::vector<Jet> AllJetColl ,  std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl,std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1, Event ev){

  if(run_Debug) cout << "HNL_SignalLeptonOpt::executeEvent " << endl;

  double weight =SetupWeight(ev,param);

  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);

  double sf_btagM_NLV                 = GetBJetSF(param, B_JetColl,    param_jetsM);
  
  if(AK8_JetColl.size() > 0 ) {
    if(!IsData){
      weight  = weight*sf_btagM_NLV;
      weight  *= AK8_JetColl[0].GetTaggerSF(JetTagging::particleNet_WvsQCD, DataEra,0); 
    }
  }
  else{
    if(!IsData)weight = weight*sf_btagM_NLV;
  }


  if(MCSample.Contains("Type")){
    if(MCSample.Contains("SS")) weight *= 0.01;
    else weight *= 0.1;
  }
  
  if(MCSample.Contains("DYJets_MG")){
    vector<TString> vec = {"DYMG"};
    double merge_weight = MergeMultiMC( vec, "" );
    weight*= merge_weight;
  }
  
  // HL ID

  TString el_ID =  param.Electron_Tight_ID ;
  TString mu_ID =  param.Muon_Tight_ID ;

  double Min_Muon_Pt     = 10.;
  double Min_Electron_Pt = 15.;

  std::vector<Muon>       MuonCollTInit; 
  std::vector<Electron>   ElectronCollTInit ;

  for(auto ilep : ElectronCollV) {
    if(ilep.Pt() < Min_Electron_Pt) continue;
    if(ilep.PassID(el_ID)) ElectronCollTInit.push_back(ilep);
  }

  for(auto ilep : MuonCollV) {
    if(ilep.Pt() < Min_Muon_Pt) continue;
    if(ilep.PassID(mu_ID)) MuonCollTInit.push_back(ilep);
  }

  std::vector<Muon>       MuonCollT       = GetLepCollByRunType    ( MuonCollTInit,param,weight);
  std::vector<Electron>   ElectronCollT   = GetLepCollByRunType   ( ElectronCollTInit,param,weight);
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param); 

  if(param.Name.Contains("HNL_ULID_"+GetYearString())) param.WriteOutVerbose=0; // Does not Fill Cutflow OR Region Plotter  
  else  if(param.Name.Contains("HNTightV2")) param.WriteOutVerbose=0;
  else  param.WriteOutVerbose=1;

  param.WriteOutVerbose=1;

  if(param.Name.Contains("HNL_ULID_"+GetYearString())){
    for(auto iel: ElectronCollT) FillElectronKinematicPlots(param,el_ID + "_Tight"+iel.GetFlavour(), iel, weight);
    for(auto imu: MuonCollT)     FillMuonKinematicPlots(param,mu_ID + "_Tight"+imu.GetFlavour(), imu, weight);
  }

  RunAllSignalRegions(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, AllJetColl,JetCollLoose, JetColl, VBF_JetColl,AK8_JetColl , B_JetColl,B_JetColl, ev,METv, param,weight);
  
  return;
  

}




HNL_SignalLeptonOpt::HNL_SignalLeptonOpt(){

  TMVA::Tools::Instance();
  MVAReaderMM = new TMVA::Reader();
  MVAReaderEE = new TMVA::Reader();
  MVAReaderEM = new TMVA::Reader();

}
 
HNL_SignalLeptonOpt::~HNL_SignalLeptonOpt(){

}




