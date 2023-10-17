#include "HNL_LeptonCore.h"

void HNL_LeptonCore::initializeAnalyzer(){

  AnalyzerCore::initializeAnalyzer();

  //=== VERBOSE                                                                                                                                        
  run_Debug = HasFlag("DEBUG");

  //=== bkg flags                                                                                                                                      
  RunPrompt = HasFlag("RunPrompt");
  RunFake   = HasFlag("RunFake");
  RunCF     = HasFlag("RunCF");
  RunConv   = HasFlag("RunConv");
  RunPromptTLRemoval = HasFlag("RunPromptTLRemoval");
  run_ORTrigger = HasFlag("MultiTrig");

  /// Other flags                                                                                                                                      
  RunSyst = HasFlag("RunSyst");
  HEM1516 = HasFlag("HEM1516");


  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Loose, JetTagging::incl, JetTagging::mujets));
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets));
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets));


  SetupTriggerLists();
  
  if(IsDYSample) SetupZptWeight();


  //--- Method 1d using JetTagging::iterativefit needs csv file changing in histmap to run                                                   
  // jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::iterativefit, JetTagging::iterativefit) );  
  mcCorr->SetJetTaggingParameters(jtps);


  SetupIDMVAReaderDefault();


}

bool HNL_LeptonCore::IsExists(TString filepath){
  ifstream fcheck(filepath);
  return fcheck.good();
}

void HNL_LeptonCore::OutCutFlow(TString lab, double w){
  
  
  std::map<TString, double>::iterator mapit = cfmap.find(lab);
  if(mapit != cfmap.end()) {
    cfmap[mapit->first] = mapit->second+w;
  }
  else     cfmap[lab]= w;

  
  return;

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
      
      for(unsigned int i=2; i<All_Gens.size(); i++){
	Gen gen = All_Gens[i];
	if( All_Gens.at(i).MotherIndex()<0   ) continue;
	if( !(All_Gens.at(i).PID()==22 && (All_Gens.at(i).Status()==23)) ) continue;
	NearPhotonIdx=i;
      }
      if(NearPhotonIdx < 0) {
	double maxpt=0;
	for(unsigned int i=2; i<All_Gens.size(); i++){
	  Gen gen = All_Gens[i];
	  if( All_Gens.at(i).MotherIndex()<0   ) continue;
	  if( !(All_Gens.at(i).PID()==22 && (All_Gens.at(i).Status()==1)) ) continue;
	  if(gen.isPromptFinalState() && gen.Pt() > maxpt) {
	    NearPhotonIdx=i;
	    maxpt = gen.Pt();
	  }
	}
      }

      if(NearPhotonIdx < 0) return 0;
      
      double phPt= All_Gens[NearPhotonIdx].Pt();
    
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




vector<AnalyzerParameter::Syst> HNL_LeptonCore::GetSystList(TString SystType){

  vector<AnalyzerParameter::Syst> SystList = {};

  if(SystType == "Initial"){
    SystList.push_back(AnalyzerParameter::JetResUp);
    SystList.push_back(AnalyzerParameter::JetResDown);
    SystList.push_back(AnalyzerParameter::PUUp);
    SystList.push_back(AnalyzerParameter::PUDown);
    SystList.push_back(AnalyzerParameter::FRUp);
    SystList.push_back(AnalyzerParameter::FRDown);
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
		AnalyzerParameter::CFUp,AnalyzerParameter::CFDown,                                                                                               
		AnalyzerParameter::FRUp,AnalyzerParameter::FRDown,                                                                                               
		AnalyzerParameter::PrefireUp,AnalyzerParameter::PrefireDown,                                                                                     
		AnalyzerParameter::PUUp,AnalyzerParameter::PUDown};
    
    
  }
  
  return SystList;

}

//====================================================/====================================================
//====================================================/====================================================
//====================================================/====================================================

AnalyzerParameter HNL_LeptonCore::InitialiseHNLParameter(TString s_setup_version, TString tag){
  
  AnalyzerParameter p = SetupHNLParameter(s_setup_version,tag);
  if(_jentry== 1 ) p.PrintParameters();
  return p;  
}


AnalyzerParameter HNL_LeptonCore::SetupHNLParameter(TString s_setup_version, TString tag){
  
  /// This functions sets up AnalyzerParameter class for different types of Analyser
  
  AnalyzerParameter param  ;
  param.Clear();

  param.Name     = s_setup_version+tag;
  param.DefName  = s_setup_version+tag;
  param.hprefix  = "";

  // Default settings if NOT s_setup_version is set

  param.syst_ = AnalyzerParameter::Central;
  param.MCCorrrectionIgnoreNoHist = true;

  /// Lepton IDs
  param.k.Muon_RECO_SF = "MuonRecoSF";

  param.Muon_Veto_ID     = "HNVetoMVA";
  param.Electron_Veto_ID = "HNVetoMVA";
  param.Tau_Veto_ID      = "JetVLElVLMuVL";
  /// Fakes
  param.FakeRateMethod   = "PtCone";
  param.k.Muon_FR        = "AwayJetPt40";
  param.k.Electron_FR    = "AwayJetPt40";

  param.Jet_ID                     = "tight";
  param.FatJet_ID                  = "tight";


  //// Weights

  param.w.lumiweight= 1;
  param.w.PUweight=1;  param.w.PUweight_up=1;  param.w.PUweight_down=1;
  param.w.prefireweight=1;  param.w.prefireweight_up=1;  param.w.prefireweight_down=1;
  param.w.z0weight=1;
  param.w.zptweight=1;
  param.w.topptweight=1;
  param.w.weakweight=1;

  if(!IsDATA){
    param.w.lumiweight*= MCweight()*_Event.GetTriggerLumi("Full");
    param.w.PUweight      = GetPileUpWeight(nPileUp,0);
    param.w.PUweight_up   = GetPileUpWeight(nPileUp,1);
    param.w.PUweight_down = GetPileUpWeight(nPileUp,-1);

    param.w.prefireweight =  GetPrefireWeight(0);
    param.w.prefireweight_up= GetPrefireWeight(1);
    param.w.prefireweight_down =GetPrefireWeight(-1);

    param.w.z0weight=GetZ0Weight(vertex_Z);

    if(IsDYSample){
      if((abs(lhe_l0.ID())==11||abs(lhe_l0.ID())==13)) {
	TLorentzVector genZ=(gen_l0+gen_l1);
	param.w.zptweight =GetZptWeight(genZ.M(),genZ.Rapidity(),genZ.Pt());
	param.w.weakweight=GetDYWeakWeight(genZ.M());
      }
      else param.hprefix+="tau_";
    }
    
    if(IsTTSample)   param.w.topptweight=mcCorr->GetTopPtReweight(All_Gens);
  }

  if (s_setup_version=="") return param;
  if (s_setup_version=="Basic") return param;

  if (s_setup_version=="SignalStudy"){
    param.CFMethod          = "MC";
    param.Muon_Tight_ID     = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";
    param.Muon_FR_ID        = "HNLooseV1";
    param.Electron_FR_ID    = "HNLooseV4";
    return param;
  }
  else if (s_setup_version=="HNL"){
    param.FakeMethod        = "DATA";
    param.CFMethod          = "MC"; ///////// -> TMP
    param.Muon_Tight_ID     = "HNL_ULID_"+GetYearString();
    param.k.Muon_ID_SF      = "NUM_HNL_ULID_"+GetYearString();
    param.k.Muon_ISO_SF     = "Default";
    param.Muon_FR_ID        = "HNL_ULID_FO";
    param.Electron_Tight_ID = "HNL_ULID_"+GetYearString();
    param.k.Electron_ID_SF  = "passHNL_ULID_"+GetYearString();
    param.Electron_FR_ID    = "HNL_ULID_FO_"+GetYearString();
    return param;
  }
  else if (s_setup_version=="HNLOpt"){
    param.FakeMethod = "MC";
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";
    param.k.Electron_ID_SF = "NUM_HNTightV2";
    param.k.Muon_ID_SF = "NUM_HNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    return param;
  }
  else if (s_setup_version=="HNLSROpt"){
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "HNTightV2";
    param.Electron_Tight_ID = "HNTightV2";
    param.k.Electron_ID_SF = "NUM_HNTightV2";
    param.k.Muon_ID_SF = "NUM_HNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    return param;
  }
  else if (s_setup_version=="BDT"){
    param.FakeMethod = "MC";
    param.CFMethod   = "MC";
    param.ConvMethod = "MC";
    param.Muon_Tight_ID = "MVAID";
    param.Electron_Tight_ID = "MVAID";
    param.k.Electron_ID_SF = "NUM_HNTightV2";
    param.k.Muon_ID_SF = "NUM_HNTightV2";
    param.Muon_FR_ID = "HNLooseV1";
    param.Electron_FR_ID = "HNLooseV4";
    return param;
  }
  else if (s_setup_version=="MVAUL"){
    param.FakeMethod = "DATA";    param.CFMethod   = "MC";    param.ConvMethod = "MC";
    param.Muon_Tight_ID      = "HNL_ULID_"+GetYearString();
    param.Electron_Tight_ID  = "HNL_ULID_"+GetYearString();
    param.k.Electron_ID_SF = "TmpHNL_ULID_"+GetYearString();
    param.k.Muon_ID_SF     = "TmpHNL_ULID_"+GetYearString();
    param.Muon_FR_ID         = "HNL_ULID_FO_"+GetYearString();
    param.Electron_FR_ID     = "HNL_ULID_FO_"+GetYearString();
    return param;
  }

  else if (s_setup_version=="EXO17028"){
    param.CFMethod           = "MC";
    param.Muon_Tight_ID      = "HNTight_17028";
    param.Electron_Tight_ID  = "HNTight_17028";
    param.Muon_FR_ID         = "HNLoose_17028";
    param.Electron_FR_ID     = "HNLoose_17028";
    return param;
  }
  else if (s_setup_version=="Peking"){
    param.CFMethod     = "MC";
    param.FakeMethod   = "MC";
    param.ConvMethod   = "MC";
    param.Muon_Tight_ID = "HNL_Peking";
    param.Electron_Tight_ID = "HNL_Peking_"+GetYearString();
    return param;
  }
  else if (s_setup_version=="POG"){
    param.CFMethod   = "MC";
    param.FakeMethod = "MC";
    param.Electron_Tight_ID = "passPOGTight";
    param.Muon_Tight_ID = "POGTightWithTightIso";

    param.k.Electron_ID_SF = "passTightID";
    param.k.Muon_ID_SF     = "NUM_TightID_DEN_TrackerMuons";
    param.k.Muon_ISO_SF    = "NUM_TightRelIso_DEN_TightIDandIPCut";

    param.Muon_FR_ID     = "HNLooseV1";
    param.Electron_FR_ID = "HNLoosePOG";
    return param;
  }

  cout << "[HNL_LeptonCore::InitialiseHNLParameters ] ID not found.." << endl;
  exit(EXIT_FAILURE);
  
  
}

  

double HNL_LeptonCore::SetupWeight(Event ev, AnalyzerParameter param){

  //=== Apply MC weight                                                                                                                                                                                    
  if(IsDATA) return 1.;

  double prefire_weight = 1.;
  if(param.Apply_Weight_PreFire){
    if(param.syst_ == AnalyzerParameter::PrefireUp) prefire_weight = GetPrefireWeight(1);
    else if(param.syst_ == AnalyzerParameter::PrefireDown)  prefire_weight = GetPrefireWeight(-1);
    else  prefire_weight = GetPrefireWeight(0);
    FillWeightHist(param.Name+"/PrefireWeight" ,prefire_weight);
  }

  double pileup_weight(1.);
  if(param.Apply_Weight_PileUp){
    if(param.syst_ == AnalyzerParameter::PUUp) pileup_weight= GetPileUpWeight(nPileUp,1);
    else if(param.syst_ == AnalyzerParameter::PUDown) pileup_weight= GetPileUpWeight(nPileUp,-1);
    else pileup_weight= GetPileUpWeight(nPileUp,0);
    FillWeightHist(param.Name+"/PileupWeight",pileup_weight);
  }

  double this_mc_weight =  MCweight(param.Apply_Weight_SumW, param.Apply_Weight_LumiNorm);
  FillWeightHist(param.Name+"/MCWeight",    this_mc_weight);
  
  if(param.Apply_Weight_LumiNorm) FillWeightHist(param.Name+"/LumiWeight",  ev.GetTriggerLumi("Full"));
  if(param.Apply_Weight_kFactor)  FillWeightHist(param.Name+"/KFactor",     GetKFactor());


  if(param.Apply_Weight_LumiNorm) this_mc_weight *= ev.GetTriggerLumi("Full");
  if(param.Apply_Weight_kFactor)  this_mc_weight *= GetKFactor();
  if(param.Apply_Weight_PileUp)   this_mc_weight *= pileup_weight;
  if(param.Apply_Weight_PreFire)  this_mc_weight *= prefire_weight;
     
     
  FillWeightHist(param.Name+"/MCFullWeight_" , this_mc_weight);

  //cout << "SetupWeight prefire_weight=" <<prefire_weight << endl;
  //cout << "SetupWeight pileup_weight=" << pileup_weight << endl;
  //cout << "SetupWeight lumniW=" <<MCweight(param.Apply_Weight_SumW, param.Apply_Weight_LumiNorm) * ev.GetTriggerLumi("Full")<< endl;
  
  if(param.Apply_Weight_Z0)      this_mc_weight *= GetZ0Weight(vertex_Z);
  if(param.Apply_Weight_TopCorr) this_mc_weight *= mcCorr->GetTopPtReweight(All_Gens);
  
  if(param.Apply_Weight_DYCorr)  this_mc_weight *= param.w.zptweight;
  if(param.Apply_Weight_DYCorr)  this_mc_weight *= param.w.weakweight;

  return this_mc_weight;
  
}

HNL_LeptonCore::Channel  HNL_LeptonCore::GetTriLeptonChannel(HNL_LeptonCore::Channel channel){

  
  if(channel == EE)   return EEE;
  if(channel == MuMu) return MuMuMu;
  if(channel == EMu)  return EMuL;
  if(channel == MuE)  return MuEL;

  return channel;
}

HNL_LeptonCore::Channel HNL_LeptonCore::GetQuadLeptonChannel(HNL_LeptonCore::Channel channel){

  if(channel ==EE)   return EEEE;
  if(channel == MuMu) return MuMuMuMu;
  if(channel == EMu)  return EMuLL;
  if(channel == MuE)  return MuELL;

  return channel;
}

bool HNL_LeptonCore::CheckLeptonFlavourForChannel(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps){
  
  int n_el(0);
  int n_mu(0);
  for(auto ilep : leps)  {
    if(ilep->LeptonFlavour() == Lepton::MUON) n_mu++;
    if(ilep->LeptonFlavour() == Lepton::ELECTRON) n_el++;
  }
  
  if(channel==MuMu  || channel==EE || channel== EMu || channel== MuE ){
    
    if (leps.size() != 2) return false;

    if (channel==EE     && !(leps[0]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->LeptonFlavour() == Lepton::ELECTRON)) return false;
    if (channel==MuMu   && !(leps[0]->LeptonFlavour() == Lepton::MUON     && leps[1]->LeptonFlavour() == Lepton::MUON))    return false;
    if (channel==EMu    && !(leps[0]->LeptonFlavour() == Lepton::ELECTRON && leps[1]->LeptonFlavour() == Lepton::MUON))    return false;
    if (channel==MuE    && !(leps[1]->LeptonFlavour() == Lepton::ELECTRON && leps[0]->LeptonFlavour() == Lepton::MUON))    return false; 
  
    double lep1_ptcut= (channel==EE) ?   25. : 20.;
    double lep2_ptcut= (channel==EE) ?   10. : 10.;
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



TString HNL_LeptonCore::DoubleToString(double d){
  
  
  std::string str = std::to_string (d);
  str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
  str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
  
  TString ts_str = TString(str);
  ts_str = ts_str.ReplaceAll(".","p");
  ts_str = ts_str.ReplaceAll("-","neg");
  
  return ts_str;
  
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
  
  DeleteZptWeight();

}


Particle HNL_LeptonCore::GetSignalObject(TString obj, TString Sig){
  
  if(Sig=="DY"){


    int N_Mother_ind(-1); // Index of N                                                                                                                                                                                                                                
    int W2_ind(0); // Index of W2 : i.e W from N decay                                                                                                                

    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);
      if(fabs(gen.PID()) == 24 && (All_Gens.at(gen.MotherIndex()).PID() == 9900012 || All_Gens.at(gen.MotherIndex()).PID() == 9900014)){
	W2_ind= i;
	for(unsigned int i2=2; i2<All_Gens.size(); i2++){
	  Gen gen2 = All_Gens.at(i2);
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

    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);

      if(int(gen.MotherIndex()) == W2_ind && gen.Status() == 23) {
	W2 = All_Gens.at(gen.MotherIndex());
	if(!j1IsSet) {  j1= gen; j1IsSet=true;}
	else j2 = gen;
      }
      if(gen.PID() == 9900012 || gen.PID() == 9900014){
	N= All_Gens.at(i);
      }
      if( ! ( ( fabs(gen.PID()) == 13)  || (fabs(gen.PID()) == 11) )) continue;

      TString LepFl = (fabs(gen.PID()) == 13) ? "Mu" : "El";

      if(All_Gens.at(gen.MotherIndex()).PID() == 9900012|| All_Gens.at(gen.MotherIndex()).PID() == 9900014) {
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


TString HNL_LeptonCore::GetChannelString(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType q){

  TString channel_string="";
  if (channel == EE) channel_string="EE";
  if (channel == MuMu) channel_string="MuMu";
  if (channel == EMu) channel_string="EMu";
  if (channel == MuE) channel_string="MuE";

  if (channel == EEE) channel_string="EEE";
  if (channel == EMuL) channel_string="EMuL";
  if (channel == MuEL) channel_string="MuEL";
  if (channel == MuMuMu) channel_string="MuMuMu";

  if (channel == EEEE) channel_string="EEEE";
  if (channel == MuMuMuMu) channel_string="MuMuMuMu";
  if (channel == EMuLL) channel_string="EMuLL";
  if (channel == MuELL) channel_string="MuELL";


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



vector<Gen> HNL_LeptonCore::GetGenLepronsSignal(){

  bool isDYVBF=false;

  vector<Gen> gen_lep;
  int N_Mother(0);

  for (auto i : All_Gens){ Gen gen = i; if(gen.PID() == 9900012 || gen.PID() == 9900014) isDYVBF=true; }

  if(isDYVBF){

    for(unsigned int i=2; i<All_Gens.size(); i++){

      Gen gen = All_Gens.at(i);

      if((gen.PID() == 9900012 || gen.PID() == 9900014)  && gen.Status()==22) {
        N_Mother= gen.MotherIndex();
      }
    }

    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);     TString lep_ch="";

      if((All_Gens.at(gen.MotherIndex()).PID() == 9900012|| All_Gens.at(gen.MotherIndex()).PID() == 9900014) && !(All_Gens.at(i).PID() == 9900012 || All_Gens.at(i).PID() == 9900014)){

        if(fabs(gen.PID()) == 15)     gen_lep.push_back(gen);

        if(fabs(gen.PID()) == 13)     gen_lep.push_back(gen);
        if(fabs(gen.PID()) == 11)     gen_lep.push_back(gen);

      }
      else if(gen.MotherIndex() == N_Mother&& !(All_Gens.at(i).PID() == 9900012 || All_Gens.at(i).PID() == 9900014)){


        if(fabs(gen.PID()) == 15) gen_lep.push_back(gen);

        if(fabs(gen.PID()) == 13) gen_lep.push_back(gen);
        if(fabs(gen.PID()) == 11) gen_lep.push_back(gen);
      }
    }
  }


  else{



    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);
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
  if(channel == MuE  && process.Contains("SS_Mu+El+")) return true;
  if(channel == MuE  && process.Contains("SS_Mu-El-")) return true;


  return false;
}




TString HNL_LeptonCore::GetProcess(){

  if (IsData) return "";
  if(!MCSample.Contains("Type")) return "";

  int N_Mother(0);

  //  cout << "index\tPID\tStatus\tMIdx\tMPID\tStart\tPt\tEta\tPhi\tM" << endl;                                                                                                                                          

  bool isDYVBF=false;

  for (auto i : All_Gens){ Gen gen = i; if(gen.PID() == 9900012 || gen.PID() == 9900014) isDYVBF=true; }

  if(isDYVBF){

    /*bool isVBF=false;                                                                                                                                                                                                  
                                                                                                                                                                                                                         
    for (auto i : All_Gens){                                                                                                                                                                                                 
      Gen gen = i;                                                                                                                                                                                                       
      if (gen.PID() == 22 && gen.Status() == 21) isVBF=true;                                                                                                                                                             
    }                                                                                                                                                                                                                    
    */

    for(unsigned int i=2; i<All_Gens.size(); i++){

      Gen gen = All_Gens.at(i);

      if((gen.PID() == 9900012 || gen.PID() == 9900014)  && gen.Status()==22) {
        N_Mother= gen.MotherIndex();
      }
    }

    int lep_1_ch=-999;
    int lep_2_ch=-999;

    TString lep1_s="", lep2_s="";
    TString lep1_ss="", lep2_ss="";

    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);     TString lep_ch="";

      if((All_Gens.at(gen.MotherIndex()).PID() == 9900012|| All_Gens.at(gen.MotherIndex()).PID() == 9900014) && !(All_Gens.at(i).PID() == 9900012 || All_Gens.at(i).PID() == 9900014)){
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
      else if(gen.MotherIndex() == N_Mother&& !(All_Gens.at(i).PID() == 9900012 || All_Gens.at(i).PID() == 9900014)){

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
    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);
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



int HNL_LeptonCore::GetIndexNonMinOSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]);
    ll1+= (*leps[1]);
    Particle ll2 = (*leps[0]);
    ll2+= (*leps[2]);
    Particle ll3 = (*leps[1]);
    ll3+= (*leps[2]);

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

  if (type=="HNL_SR3_NLL"){

    if(jets.size() < 2) return 0.;
    if(leps.size() != 2) return 0.;

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
    Particle Wcand = jets[m] + jets[n]+*leps[0] + *leps[1];
    return Wcand.M();
  }
  if (type=="HNL_SR3_N1L"){

    if(jets.size() < 2) return 0.;
    if(leps.size() != 2) return 0.;

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
    Particle Ncand = jets[m] + jets[n]+*leps[0] ;
    return Ncand.M();
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

    return fatjets[m].SDMass();
  }


  cout << "[HNL_LeptonCore::GetMass ] ID not found.." << endl;
  exit(EXIT_FAILURE);

  return -9999.;
}



vector<Muon> HNL_LeptonCore::GetLepCollByRunType(const std::vector<Muon>& MuColl, AnalyzerParameter param, TString Option){


  /// Empty Option means  param is used to configure
  if(Option == ""){
    if(param.FakeMethod == "MC") Option+="HFake";
    if(param.ConvMethod == "MC") Option+="NHConv";
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
      int LepType= MuColl.at(i).LeptonGenType();

      bool PassSel=false;

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




vector<Electron> HNL_LeptonCore::GetLepCollByRunType(const vector<Electron>& ElColl, AnalyzerParameter param, TString Option){


  if(Option == ""){
    if(param.FakeMethod == "MC") Option+="HFake";
    if(param.ConvMethod == "MC") Option+="NHConv";
    if(param.CFMethod == "MC")   Option+="CF";
  }


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
      int LepType= ElColl.at(i).LeptonGenType();

      bool PassSel=false;
      if( LepType > 0 && LepType < 4) PassSel=true;
      if( GetHadFake    && (LepType<0 && LepType>=-4) ) PassSel=true;
      if( GetNHIntConv &&         LepType>=4         ) PassSel=true;
      if( GetNHExtConv &&         LepType<-4         ) PassSel=true;
      if( GetCF   && ElColl.at(i).LeptonIsCF() ) PassSel=true;
      if( !GetCF && ElColl.at(i).LeptonIsCF() ) PassSel=false;
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

    bool passZmass_lll_Window = (fabs(lll.M() - M_Z) < 15.);
    return passZmass_lll_Window;

  }

  return true;
}


int HNL_LeptonCore::GetIndexNonMinSSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){


    Particle ll1 = (*leps[0]);
    ll1+= (*leps[1]);
    Particle ll2 = (*leps[0]);
    ll2+= (*leps[2]);
    Particle ll3 = (*leps[1]);
    ll3+= (*leps[2]);

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


    Particle ll1 = (*leps[0]);
    ll1+= (*leps[1]);
    Particle ll2 = (*leps[0]);
    ll2+= (*leps[2]);
    Particle ll3 = (*leps[1]);
    ll3+= (*leps[2]);

    double minOSZ=99999999999.;
    if(fabs(ll1.Charge()) == 0) {
      if(fabs(ll1.M() - M_Z) < minOSZ) {minOSZ = fabs(ll1.M() - M_Z); index=2;}
    }
    if(fabs(ll2.Charge()) == 0) {
      if(fabs(ll2.M() - M_Z) < minOSZ) {minOSZ = fabs(ll2.M() - M_Z);index=1;}

    }
    if(fabs(ll3.Charge()) == 0) {
      if(fabs(ll3.M() - M_Z) < minOSZ) {minOSZ = fabs(ll3.M() - M_Z);index=0;}
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
      if((fabs(ll.M() - M_Z) < mass_diff)) return true;
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
            if(( fabs(Z1Cand.M()- M_Z) + fabs(Z2Cand.M()- M_Z)) < minMZ) {
              minMZ = fabs(Z1Cand.M()- M_Z) + fabs(Z2Cand.M()- M_Z);
              if(fabs(Z1Cand.M()- M_Z)< fabs(Z2Cand.M()- M_Z) ){ massBestZ=Z1Cand.M(); massNonBestZ=Z2Cand.M();}
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
        if(fabs(Z1Cand.M()- M_Z) < minMZ) {minMZ = fabs(Z1Cand.M()- M_Z) ; massBestZ=Z1Cand.M();}
      }
    }
  }
  return massBestZ;

}

bool  HNL_LeptonCore::ZmassOSWindowCheck(std::vector<Lepton *> leps){

  bool passZmass_os_Window=false;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]);
    ll1+= (*leps[1]);
    Particle ll2 = (*leps[0]);
    ll2+= (*leps[2]);
    Particle ll3 = (*leps[1]);
    ll3+= (*leps[2]);

    if(ll1.Charge() == 0 && (fabs(ll1.M() - M_Z) < 15.)) passZmass_os_Window=true;
    if(ll2.Charge() == 0 && (fabs(ll2.M() - M_Z) < 15.)) passZmass_os_Window=true;
    if(ll3.Charge() == 0 && (fabs(ll3.M() - M_Z) < 15.)) passZmass_os_Window=true;

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


bool HNL_LeptonCore::PassGenMatchFilter(vector<Lepton *> leps, AnalyzerParameter param){

  /// Function to split Prompt/Conv/CF/Fake IF code uses RunPrompt etc.... 
  /// If code does not use Run* to sepeate MC samples then function should return true

  /// If user used Data driven method for Fake/CF then function returns for Fake/F bkf true for data and false for MC 
  /// If user used Data driven method for Fake/CF then for Conv function requires at least one conv lepton is present 

  if(IsData) return true;
  if(RunFake && param.FakeMethod != "MC") return false;
  if(RunCF   && param.CFMethod   != "MC") return false;
  if(RunConv && param.ConvMethod != "MC") return false;

  if(MCSample.Contains("Type")) return true;

  //// Function filters events when using MC based on if they are Fake/CF/Conv
  
  int nConv(0);
  int nCF=(0);
  int nFake=(0);                                         
  unsigned int nPrompt(0);
  for(auto ilep: leps){
    //int LepType= GetLeptonType_JH(*ilep, gens);
    if( ilep->IsConv())     nConv++;
    if( ilep->LeptonIsCF()) nCF++;
    if( ilep->IsFake())     nFake++;
    if( ilep->IsPrompt())   nPrompt++;
  }
  
  if(RunPrompt && (nPrompt == leps.size())) return true;
  if(RunPrompt && (nPrompt != leps.size())) return false;


  if( (RunFake || RunConv || RunCF )){

    if(RunFake  && nFake > 0)  return true;
    if(!RunFake && nFake > 0)  return false;
    
    if(RunCF    && nCF   > 0)  return true;
    if(!RunCF   && nCF > 0)    return false;

    if(RunConv  && nConv > 0)  return true;
    if(!RunConv && nConv > 0) return false;
    
    return false;
  }
  
  return true;

}


TString HNL_LeptonCore::GetElTypeTString(Electron el){
  if(IsDATA) return "";
  TString tag = "";

  if(GetLeptonType(el, All_Gens) < 0) tag = "Minus";
  tag+=TString::Itoa(fabs( el.LeptonGenType()), 10);
  return tag;
}

TString HNL_LeptonCore::GetLepTypeTString(const Lepton& lep){
  if(IsDATA) return "";
  TString tag = "";
  if(GetLeptonType(lep, All_Gens) < 0) tag = "Minus";
  tag+=TString::Itoa(fabs(lep.LeptonGenType()), 10);
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





TString HNL_LeptonCore::GetPtBin(bool muon, double pt){
  TString pt_label="";
  
  if(pt< 20) pt_label = "_ptbin1"; 
  else if(pt < 50) pt_label = "_ptbin2"; 
  else if(pt < 100) pt_label = "_ptbin3";
  else pt_label = "_ptbin4";                                                               

  return pt_label;

}










