#include "HNL_SignalLeptonOpt.h"

void HNL_SignalLeptonOpt::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  
  //SetupIDMVAReaderDefault(); /// Not needed for BDT skim
  
  SetupEventMVAReader(true,false,false); 

  RunHighPt= HasFlag("RunHighPt");

}


void HNL_SignalLeptonOpt::executeEvent(){

  OutCutFlow("CutFlow_All_Events", 1);


  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNLOpt","_UL");
    
  //cout << "########################################################" << endl;


  std::vector<Electron>   ElectronCollV = GetElectrons(param_signal.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param_signal.Muon_Veto_ID, 5., 2.4);
  std::vector<Lepton *>   LepsVeto      = MakeLeptonPointerVector(MuonCollV,ElectronCollV,param_signal);


  if(!SameCharge(LepsVeto)) return;

  std::vector<Jet> JetColl                        = GetHNLJets("Tight",param_signal);
  std::vector<FatJet> FatjetColl                  = GetHNLAK8Jets("HNL_PNL",param_signal);

  Event ev = GetEvent();
  Particle METv = GetvMET("PuppiT1xyULCorr");


  ///// Initialise non lepton obj
  std::vector<Tau> TauColl                        = GetTaus     (LepsVeto,param_signal.Tau_Veto_ID,20., 2.3);
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
  


  if(HasFlag("ELID_FullScan")){
    
    if(HasFlag("ScanEC")){

      vector<TString> vMVACFEC;
      vector<TString> vMVANPEC;
      vector<TString> vMVACVEC;

      //// First Include 1D cuts                                                                                                                                                     
      for(int imva=0 ; imva < 7 ; imva++){
        double mva_d=  0.5 + double(imva)*.025;
        TString mvaTS= DoubleToString(mva_d);
        vMVACFEC.push_back("MVAECF"+mvaTS+"_");
      }
      
      for(int imva=0 ; imva < 10 ; imva++){
        double mva_d=  0.2 + double(imva)*.025;
        TString mvaTS= DoubleToString(mva_d);
	vMVANPEC.push_back("MVAENP"+mvaTS+"_");
      }

      for(int imva=0 ; imva < 8 ; imva++){
	double mva_d=  -0.7 + double(imva)*.1;
	TString mvaTS= DoubleToString(mva_d);
        TString mvaTS2= DoubleToString(-0.35);
        TString mvaTS3= DoubleToString(0.0);

	
	vMVACVEC.push_back("MVAECV1"+mvaTS+"_"+"MVAECV2"+mvaTS+"_");
	if (imva < 4) 	vMVACVEC.push_back("MVAECV1"+mvaTS+"_"+"MVAECV2"+mvaTS2+"_");
	if (imva < 7)   vMVACVEC.push_back("MVAECV1"+mvaTS+"_"+"MVAECV2"+mvaTS3+"_");
      }

      for(auto id1 : vMVACFEC) {
	for(auto id2 : vMVANPEC) {
	  for(auto id3 : vMVACVEC) {
	    ElectronsIDs.push_back("HNLUL_LTrig_BarrelSel_"+id1+id2+id3);
	  }
	}
      }
    }
    if(HasFlag("ScanBB")){

      vector<TString> vMVACFBB;
      vector<TString> vMVANPBB;
      vector<TString> vMVACVBB;

      //// First Include 1D cuts                                                                                                                                                                                                            
      for(int imva=0 ; imva < 7 ; imva++){
        double mva_d=  0.5 + double(imva)*.025;
        TString mvaTS= DoubleToString(mva_d);
        vMVACFBB.push_back("MVABCF"+mvaTS+"_");
      }

      for(int imva=0 ; imva < 9 ; imva++){
        double mva_d=  0.15 + double(imva)*.025;
        TString mvaTS= DoubleToString(mva_d);
        vMVANPBB.push_back("MVABNP"+mvaTS+"_");
      }

      for(int imva=0 ; imva < 8 ; imva++){
        double mva_d=  -0.7 + double(imva)*.1;
        TString mvaTS= DoubleToString(mva_d);
        TString mvaTS2= DoubleToString(-0.35);
        TString mvaTS3= DoubleToString(0.0);


        vMVACVBB.push_back("MVABCV1"+mvaTS+"_"+"MVABCV2"+mvaTS+"_");
	if (imva < 4)   vMVACVBB.push_back("MVABCV1"+mvaTS+"_"+"MVABCV2"+mvaTS2+"_");
        if (imva < 7)   vMVACVBB.push_back("MVABCV1"+mvaTS+"_"+"MVABCV2"+mvaTS3+"_");

      }

      for(auto id1 : vMVACFBB) {
        for(auto id2 : vMVANPBB) {
          for(auto id3 : vMVACVBB) {
            ElectronsIDs.push_back("HNLUL_LTrig_EndCapSel_"+id1+id2+id3);
          }
        }
      }
    }
  }

  if(HasFlag("ELID_NPScan")){

    vector<TString> vMVANPBB;
    vector<TString> vMVANPEC;

    //// First Include 1D cuts                                                                                                               
    for(int imva=0 ; imva < 13 ; imva++){
      double mva_d=  0.2 + double(imva)*.025;
      TString mvaTS= DoubleToString(mva_d);

      vMVANPBB.push_back("MVABNP"+mvaTS+"_");
      vMVANPEC.push_back("MVAENP"+mvaTS+"_");

    }

    for(auto id1 : vMVANPBB) {
      for(auto id2 : vMVANPEC) {
        ElectronsIDs.push_back("HNLUL_LTrig_NPScanID"+GetYearString()+"_"+id1+id2);
      }
    }
  }




  if(HasFlag("ELID_ConvScan")){
    
    vector<TString> vMVAConvBB;
    vector<TString> vMVAConvEC;
    vector<TString> vMVANPBB;
    vector<TString> vMVANPEC;
    //// First Include 1D cuts                                                                                                 
    for(int imva=0 ; imva < 5 ; imva++){
      double mva_d=  -0.8 + double(imva)*.2;
      TString mvaTS= DoubleToString(mva_d);
      TString mvaTS3= DoubleToString(0.0);
      
      vMVAConvBB.push_back("MVABCV1"+mvaTS+"_"+"MVABCV2"+mvaTS+"_");
      if (imva < 4)   vMVAConvBB.push_back("MVABCV1"+mvaTS+"_"+"MVABCV2"+mvaTS3+"_");
      vMVAConvEC.push_back("MVAECV1"+mvaTS+"_"+"MVAECV2"+mvaTS+"_");
      if (imva < 4)       vMVAConvEC.push_back("MVAECV1"+mvaTS+"_"+"MVAECV2"+mvaTS3+"_");
    }
    
    for(int imva=0 ; imva < 7 ; imva++){
      double mva_d=  0.2 + double(imva)*.05;
      TString mvaTS= DoubleToString(mva_d);
      if(imva < 4)vMVANPBB.push_back("MVABNP"+mvaTS+"_");
      vMVANPEC.push_back("MVAENP"+mvaTS+"_");

    }


    for(auto id1 : vMVAConvBB) {
      for(auto id2 : vMVAConvEC) {
	for(auto id3 : vMVANPBB) {
	  for(auto id4 : vMVANPEC) {
	    ElectronsIDs.push_back("HNLUL_LTrig_ConvScanID_"+id1+id2+id3+id4);
	  }
	}
      }
    }
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
  ElectronsIDs.push_back("HNL_ULIDv2_2016");
  ElectronsIDs.push_back("HNL_ULIDv2_2017");
  ElectronsIDs.push_back("HNL_ULID_2018");
  
  
  ev.SetMVA("EE",100, EvaluateEventMVA("100", "300","850", EE,  LepsVeto,ev, METv ,param_signal));
  ev.SetMVA("EE",400, EvaluateEventMVA("400", "300","850", EE,  LepsVeto,ev, METv ,param_signal));
  ev.SetMVA("EE",500, EvaluateEventMVA("500", "300","850", EE,  LepsVeto,ev, METv ,param_signal));
  

  if(_jentry%10000==0) cout << "ElectronsIDs size = " << ElectronsIDs.size() << endl;

  
  
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
  JetTagging::Parameters param_jetsT = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets);

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

  std::vector<Muon>       MuonCollT       = GetLepCollByRunType    ( MuonCollTInit,param);
  std::vector<Electron>   ElectronCollT   = GetLepCollByRunType   ( ElectronCollTInit,param);
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param); 

  if(param.Name.Contains("HNL_ULID_"+GetYearString())) param.WriteOutVerbose=0; // Does not Fill Cutflow OR Region Plotter  
  else  if(param.Name.Contains("HNTightV2")) param.WriteOutVerbose=0;
  else  param.WriteOutVerbose=1;

  param.WriteOutVerbose=1;

  if(param.Name.Contains("HNL_ULID_"+GetYearString())){
    for(auto iel: ElectronCollT) FillElectronKinematicPlots(el_ID + "_"+iel.GetFlavour(),"Tight"+iel.GetFlavour(), iel, weight);
    for(auto imu: MuonCollT)     FillMuonKinematicPlots(mu_ID + "_"+imu.GetFlavour(),"Tight"+imu.GetFlavour(), imu, weight);
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




