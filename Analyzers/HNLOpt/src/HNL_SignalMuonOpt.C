#include "HNL_SignalMuonOpt.h"

void HNL_SignalMuonOpt::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  
  //SetupIDMVAReaderDefault(); /// Not needed for BDT skim
  
  SetupEventMVAReader(false,true,false);

  RunHighPt= HasFlag("RunHighPt");
  RunEE = HasFlag("RunEE");
  
}


void HNL_SignalMuonOpt::executeEvent(){

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

  std::vector<Tau> TauColl                        = SelectTaus    (LepsVeto,param_signal.Tau_Veto_ID,20., 2.3);
  
  std::vector<Jet> AllJetColl                     = GetHNLJets("NoCut_Eta3",param_signal);
  std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param_signal);
  std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param_signal);
  std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param_signal);


  vector<TString> MuonsIDs;
  
  
  if(!SameCharge(MuonCollV)) return;
  
  if(ElectronCollV.size() > 0) return;
  
  if(HasFlag("MuID_DXY")){
    MuonsIDs.clear();
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv1");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv2");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv3");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv4");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv5");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv6");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv7");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv8");
    MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv9");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv1");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv2");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv3");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv4");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv5");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv6");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv7");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv8");
    MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv9");
    
  }
  
  if(HasFlag("MuID_NP_HF")){
    
    MuonsIDs.push_back("HNLUL_POGBT_ISOB0p15_POGECT_ISOEC0p15_DXYv1");
    
    if(HasFlag("FullEta")){
      vector<TString> mvaHF1;
      for(unsigned int imva=0 ; imva < 21 ; imva++){
	double mva_d= 0.4 + double(imva)*0.02;
	TString mvaTS= DoubleToString(mva_d);
	
	for(unsigned int imva2=0 ; imva2 < 21 ; imva2++){
	  double mva2_d= 0.6 + double(imva2)*0.01;
	  TString mva2TS= DoubleToString(mva2_d);
	  mvaHF1.push_back("MVAHFBB"+mvaTS+"_MVAHFEC"+mva2TS+"_");
	}
      }
      
      for(auto i1 : mvaHF1){ 
	MuonsIDs.push_back("HNLUL_"+i1+"DXYv8");
      }
    }
  }

  
  
  if(HasFlag("MuID_NP")){
    
    MuonsIDs.push_back("HNLUL_POGBT_ISOB0p15_POGECT_ISOEC0p15_DXYv1");
    
    if(HasFlag("FullEta")){
      
      if(HasFlag("HF_Bin1")){
	
	vector<TString> mvaHF1;
	vector<TString> mvaHF2;
	for(unsigned int imva=0 ; imva < 5 ; imva++){
	  double mva_d= -1 + double(imva)*0.25;
	  TString mvaTS= DoubleToString(mva_d);
	  mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
	  mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
	}
	
	for(unsigned int imva=0 ; imva < 9 ; imva++){
	  double mva_d= 0.1 + double(imva)*0.1;
	  TString mvaTS= DoubleToString(mva_d);
	  mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
	  mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
	}
	
	for(auto i1 : mvaHF1){
	  for(auto i2 : mvaHF2){
	    MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv1");
	  }
	}
      }// Bin1
      if(HasFlag("HF_Bin2")){
	
	vector<TString> mvaHF1;
	vector<TString> mvaHF2;
	
	for(unsigned int imva=0 ; imva < 11 ; imva++){
	  double mva_d= 0.4 + double(imva)*0.02;
	  TString mvaTS= DoubleToString(mva_d);
	  mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
	  mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
	}
	for(unsigned int imva=0 ; imva < 20 ; imva++){
	  double mva_d= 0.61 + double(imva)*0.01;
	  TString mvaTS= DoubleToString(mva_d);
	  mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
	  mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
	}
	
	for(auto i1 : mvaHF1){
	  for(auto i2 : mvaHF2){
	    MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv8");
	  }
	}
      }// Bin2                                                                                                                                                            
      if(HasFlag("HF_Bin3")){
	
	vector<TString> mvaHF;
	if(DataYear == 2016) mvaHF.push_back("MVAHFBB0p75_MVAHFEC0p72_");
	if(DataYear == 2017) mvaHF.push_back("MVAHFBB0p68_MVAHFEC0p64_");
	if(DataYear == 2018) mvaHF.push_back("MVAHFBB0p68_MVAHFEC0p64_");
	//vector<TString> vdxy = {"DXY0p005_","DXY0p01_","DXY0p02_","DXY0p05_","DXY0p1_"};
	//vector<TString> vdz  = {"DZ0p02_","DZ0p04_","DZ0p05_","DZ0p1_"}; 
	vector<TString> vip ;
	if(HasFlag("SIP")){
	  for(unsigned int ip=0 ; ip < 51 ; ip++){
	    double ip_d= 3. + double(ip)*0.1;
	    TString ipTS= DoubleToString(ip_d);
	    vip.push_back("SIP"+ipTS+"_");
	  }
	}
	if(HasFlag("DXY")){
	  for(unsigned int ip=0 ; ip < 11 ; ip++){
	    double ip_d= 0.005 + double(ip)*0.005;
	    TString ipTS= DoubleToString(ip_d);
	    vip.push_back("DXY"+ipTS+"_");
	  }
	}
	if(HasFlag("DZ")){
	  for(unsigned int ip=0 ; ip < 13 ; ip++){
	    double ip_d= 0.04 + double(ip)*0.005;
	    TString ipTS= DoubleToString(ip_d);
	    vip.push_back("DZ"+ipTS+"_");
	  }
	}
	
	for(auto i1 : mvaHF){
	  for(auto i2 : vip){
	    MuonsIDs.push_back("HNLUL_"+i1+i2);
	  }
	}
	
      }// Bin3                                                                                                                                            
    }// Eta
    
    if(HasFlag("LF_Bin1")){
      
      vector<TString> mvaHF;
      if(DataYear == 2016) mvaHF.push_back("MVAHFBB0p75_MVAHFEC0p72_");
      if(DataYear == 2017) mvaHF.push_back("MVAHFBB0p68_MVAHFEC0p64_");
      if(DataYear == 2018) mvaHF.push_back("MVAHFBB0p68_MVAHFEC0p64_");
      
      
      vector<TString> mvaLF;
      
      for(unsigned int imva=0 ; imva < 91 ; imva++){
	double mva_d= -1 + double(imva)*0.02;
	TString mvaTS= DoubleToString(mva_d);
	if(HasFlag("BB")) mvaLF.push_back("MVALFBB"+mvaTS+"_");
	if(HasFlag("EC")) mvaLF.push_back("MVALFEC"+mvaTS+"_");
	
      }
      for(auto i1 : mvaLF){
	for(auto i2 : mvaHF){
	  MuonsIDs.push_back("HNLUL_"+i1+i2+"NPMMEDv4");
	}
      }
    }// Bin2   
  }
  
  
  //cout << " MuonsIDs = " <<  MuonsIDs.size() << endl;
  vector<TString> vIsoB  = {"ISOB0p05","ISOB0p06","ISOB0p07", "ISOB0p08","ISOB0p09","ISOB0p1","ISOB0p125","ISOB0p15"};
  vector<TString> vIsoEC = {"ISOEC0p05","ISOEC0p06","ISOEC0p07","ISOEC0p08","ISOEC0p09","ISOEC0p1","ISOEC0p125","ISOEC0p15"};
  
  if(HasFlag("Muon_Iso")){
    MuonsIDs.clear();
    
    for(unsigned int ib = 0 ; ib < vIsoB.size(); ib++){
      for(unsigned int ie = 0 ; ie < vIsoEC.size(); ie++){
	TString iISOB = vIsoB[ib];
	TString iISOEC = vIsoEC[ie];
	
	MuonsIDs.push_back("HNLUL_POGBT_POGECT_"+iISOB+"_"+iISOEC);
	MuonsIDs.push_back("HNLUL_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv1");
	MuonsIDs.push_back("HNLUL_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv1");
	MuonsIDs.push_back("HNLUL_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv2");
	MuonsIDs.push_back("HNLUL_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv2");
	MuonsIDs.push_back("HNLUL_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv3");
	MuonsIDs.push_back("HNLUL_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv3");
	MuonsIDs.push_back("HNLUL_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv4");
	MuonsIDs.push_back("HNLUL_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv4");
	MuonsIDs.push_back("HNLUL_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv5");
	MuonsIDs.push_back("HNLUL_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv5");
      }
    }
    
    
    
    MuonsIDs.push_back("HNTight_Iso05_dxy_01_ip_3"); ;
    MuonsIDs.push_back("HNTight_Iso05_dxy_01_ip_4");
    MuonsIDs.push_back("HNTight_Iso05_dxy_01_ip_999");
    MuonsIDs.push_back("HNTight_Iso05_dxy_02_ip_3");
    MuonsIDs.push_back("HNTight_Iso05_dxy_02_ip_4");
    MuonsIDs.push_back("HNTight_Iso05_dxy_02_ip_999");
    MuonsIDs.push_back("HNTight_Iso05_dxy_05_ip_3");
    MuonsIDs.push_back("HNTight_Iso05_dxy_05_ip_4");
    MuonsIDs.push_back("HNTight_Iso05_dxy_05_ip_999");
    
    
    MuonsIDs.push_back("HNTight_Iso06_dxy_01_ip_3");
    MuonsIDs.push_back("HNTight_Iso06_dxy_01_ip_4");
    MuonsIDs.push_back("HNTight_Iso06_dxy_01_ip_999");
    MuonsIDs.push_back("HNTight_Iso06_dxy_02_ip_3");
    MuonsIDs.push_back("HNTight_Iso06_dxy_02_ip_4");
    MuonsIDs.push_back("HNTight_Iso06_dxy_02_ip_999");
    MuonsIDs.push_back("HNTight_Iso06_dxy_05_ip_3");
    MuonsIDs.push_back("HNTight_Iso06_dxy_05_ip_4");
    MuonsIDs.push_back("HNTight_Iso06_dxy_05_ip_999");
    
    MuonsIDs.push_back("HNTight_Iso07_dxy_01_ip_3");
    MuonsIDs.push_back("HNTight_Iso07_dxy_01_ip_4");
    MuonsIDs.push_back("HNTight_Iso07_dxy_01_ip_999");
    MuonsIDs.push_back("HNTight_Iso07_dxy_02_ip_3");
    MuonsIDs.push_back("HNTight_Iso07_dxy_02_ip_4");
    MuonsIDs.push_back("HNTight_Iso07_dxy_02_ip_999");
    MuonsIDs.push_back("HNTight_Iso07_dxy_05_ip_3");
    MuonsIDs.push_back("HNTight_Iso07_dxy_05_ip_4");
    MuonsIDs.push_back("HNTight_Iso07_dxy_05_ip_999");
    
    MuonsIDs.push_back("HNTight_Iso08_dxy_01_ip_3");
    MuonsIDs.push_back("HNTight_Iso08_dxy_01_ip_4");
    MuonsIDs.push_back("HNTight_Iso08_dxy_01_ip_999");
    MuonsIDs.push_back("HNTight_Iso08_dxy_02_ip_3");
    MuonsIDs.push_back("HNTight_Iso08_dxy_02_ip_4");
    MuonsIDs.push_back("HNTight_Iso08_dxy_02_ip_999");
    MuonsIDs.push_back("HNTight_Iso08_dxy_05_ip_3");
    MuonsIDs.push_back("HNTight_Iso08_dxy_05_ip_4");
    MuonsIDs.push_back("HNTight_Iso08_dxy_05_ip_999");
    
    MuonsIDs.push_back("HNTight_Iso10_dxy_01_ip_3");
    MuonsIDs.push_back("HNTight_Iso10_dxy_01_ip_4");
    MuonsIDs.push_back("HNTight_Iso10_dxy_01_ip_999");
    MuonsIDs.push_back("HNTight_Iso10_dxy_02_ip_3");
    MuonsIDs.push_back("HNTight_Iso10_dxy_02_ip_4");
    MuonsIDs.push_back("HNTight_Iso10_dxy_02_ip_999");
    MuonsIDs.push_back("HNTight_Iso10_dxy_05_ip_3");
    MuonsIDs.push_back("HNTight_Iso10_dxy_05_ip_4");
    MuonsIDs.push_back("HNTight_Iso10_dxy_05_ip_999");
    
  } 
  if(HasFlag("Muon_POG")){
    MuonsIDs.clear();
      
    MuonsIDs.push_back("HNTightPFIsoLoose");
    MuonsIDs.push_back("HNTightPFIsoMedium");
    MuonsIDs.push_back("HNTightPFIsoTight");
    MuonsIDs.push_back("HNTightPFIsoVeryTight");
    MuonsIDs.push_back("HNTightPFIsoVeryVeryTight");
    MuonsIDs.push_back("POGTightPFIsoLoose");
    MuonsIDs.push_back("POGTightPFIsoMedium");
    MuonsIDs.push_back("POGTightPFIsoTight");
    MuonsIDs.push_back("POGTightPFIsoVeryTight");
    MuonsIDs.push_back("POGTightPFIsoVeryVeryTight");
    MuonsIDs.push_back("HNL_ULID_2016");
    MuonsIDs.push_back("HNL_ULID_2017");
    MuonsIDs.push_back("HNL_ULID_2018");
    MuonsIDs.push_back("HNL_ULIDv2");
    MuonsIDs.push_back("HNL_ULIDv3");
    MuonsIDs.push_back("HNL_ULIDv4");
    MuonsIDs.push_back("HNTight_17028");
    MuonsIDs.push_back("HNL_Peking");
    MuonsIDs.push_back("HNL_HN3L");
  }
  
  MuonsIDs.push_back("HNTightV2");
  MuonsIDs.push_back("HNL_HN3L");
  MuonsIDs.push_back("HNL_ULID_2016");
  MuonsIDs.push_back("HNL_ULID_2018");
  MuonsIDs.push_back("HNL_ULID_LF");
  MuonsIDs.push_back("HNL_ULIDv1_2016");
  MuonsIDs.push_back("HNL_ULIDv1_2017");
  MuonsIDs.push_back("HNL_ULIDv1_2018");
  MuonsIDs.push_back("HNL_LFULID_2017");
  MuonsIDs.push_back("HNL_Peking");
  
  
  for (unsigned int i=0; i<MuonsIDs.size(); i++) {
    for (unsigned int j=0; j<MuonsIDs.size(); j++) {
      if(i != j && MuonsIDs[i] == MuonsIDs[j]) {
	cout << "[Muon::ID]  : " << MuonsIDs[i] << " occurs twice" << endl;
	
	exit(ENODATA);
	
      }
    }
  }

  
  /// Caulcate BDT for MM events
  
  
  
  ev.SetMVA("MuMu",100, EvaluateEventMVA("100", "Incl", "300", "850", MuMu, LepsVeto, ev, METv, param_signal));
  ev.SetMVA("MuMu",400, EvaluateEventMVA("400", "Incl", "300", "850", MuMu, LepsVeto, ev, METv, param_signal));
  ev.SetMVA("MuMu",500, EvaluateEventMVA("500", "Incl", "300", "850", MuMu, LepsVeto, ev, METv, param_signal));
  
  //cout << "Number of MuonsIDs = " << MuonsIDs.size() << endl;
  
  for(auto id : MuonsIDs){
    
    if(id.Contains("HNLUL")){
      param_signal.Name = param_signal.DefName  + "_MuOpt_"+ id;
      param_signal.SRConfig  = "";
      param_signal.Muon_Tight_ID="MuOpt_"+id;
      param_signal.Muon_FR_ID = "MuOptLoose_"+id;
      param_signal.Electron_Tight_ID = "HNTightV2";
      param_signal.Electron_FR_ID = "HNLooseV4";
    }
    else{
      param_signal.Name = param_signal.DefName  + id;
      param_signal.SRConfig  = "";
      param_signal.Muon_Tight_ID=id;
      param_signal.Muon_FR_ID = id;
      param_signal.Electron_Tight_ID = "HNTightV2";
      param_signal.Electron_FR_ID = "HNLooseV4";
      
    }
    
    RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetColl,ev);
    
  }
  
  
  return ;
}

void HNL_SignalMuonOpt::RunULAnalysis(AnalyzerParameter param, vector<Electron> ElectronCollV, vector<Muon> MuonCollV, std::vector<Tau> TauColl,  std::vector<Jet> AllJetColl ,  std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl,std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1, Event ev){

  if(run_Debug) cout << "HNL_SignalMuonOpt::executeEvent " << endl;

  double weight =SetupWeight(ev,param);

  if(param.Name == "HNLOpt_ULHNTightV2") OutCutFlow("CutFlow_Events_HNTight", 1);
  if(param.Name == "HNLOpt_ULHNTightV2") OutCutFlow("CutFlow_Events_HNTight_weighted", weight);
  
  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  JetTagging::Parameters param_jetsT = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets);

  double sf_btagM_NLV                 = GetBJetSF(param, B_JetColl,    param_jetsM);
  double sf_btagSR1_NLV               = GetBJetSF(param, B_JetCollSR1, param_jetsT);
  
  if(AK8_JetColl.size() > 0 ) {
    if(!IsData){
      weight  = weight*sf_btagSR1_NLV;
      weight  *= AK8_JetColl[0].GetTaggerSF(JetTagging::particleNet_WvsQCD, DataEra,0); 
    }
  }
  else{
    if(!IsData)weight = weight*sf_btagM_NLV;
  }

  //cout << "weight = " << weight << endl;
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

  double Min_Muon_Pt     = 5.;
  double Min_Electron_Pt = 10.;


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

  std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( MuonCollTInit,param,weight);
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,param,weight);
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param); // returns MET with systematic correction                                                                                                                      

  if(param.Name.Contains("HNL_ULID_2017")) param.WriteOutVerbose=0; // Does not Fill Cutflow OR Region Plotter  
  else  param.WriteOutVerbose=1;

  if(param.Name== "HNLOpt_ULHNTightV2") OutCutFlow(param.Name+"_pre", weight);

  if(param.Name.Contains("HNL_ULID_2018")){
    for(auto iel: ElectronCollT) FillElectronKinematicPlots(param,el_ID + "_Tight"+iel.GetFlavour(), iel, weight);
    for(auto imu: MuonCollT)     FillMuonKinematicPlots(param,mu_ID + "_Tight"+imu.GetFlavour(), imu, weight);
  }

  RunAllSignalRegions(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, AllJetColl,JetCollLoose, JetColl, VBF_JetColl,AK8_JetColl , B_JetColl,B_JetCollSR1, ev,METv, param,weight);
  
  return;
  

}




HNL_SignalMuonOpt::HNL_SignalMuonOpt(){

  TMVA::Tools::Instance();
  MVAReaderMM = new TMVA::Reader();
  MVAReaderEE = new TMVA::Reader();
  MVAReaderEM = new TMVA::Reader();

}
 
HNL_SignalMuonOpt::~HNL_SignalMuonOpt(){

}




