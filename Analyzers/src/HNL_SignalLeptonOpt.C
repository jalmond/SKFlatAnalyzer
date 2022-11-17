#include "HNL_SignalLeptonOpt.h"

void HNL_SignalLeptonOpt::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  cout << "SetupMVAReader " << endl;
  SetupMVAReader();

}


void HNL_SignalLeptonOpt::executeEvent(){
  
  
  if(!IsData)  gens = GetGens();

  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNLOpt","_UL");
  
  
  //cout << "########################################################" << endl;
  bool opt_IDEl=true;
  if(opt_IDEl){

    vector<TString> vTrig = {};//"LooseTrig_"};//,"TightTrig_"};
    vector<TString> vConv = {"ConvBConvEC_"};//,"ConvBConvEC_","ConvEC_"};
    vector<TString> vCC = {"CCBCCEC_"};//,"CCEC_",""};
    vector<TString> vMVABB;
    vector<TString> vMVAEB;
    vector<TString> vMVAEE;

    vector<TString> ElectronsIDs;
    ElectronsIDs.push_back("HNOpt");

    

    for(auto id : ElectronsIDs){

      if (!id.Contains("DXYB1EC1")){

	//if(SameCharge(GetElectrons(id, 10., 2.5)))FillAllElectronPlots("Bkg", "ElOpt_"+id  , GetElectrons(id, 10., 2.5), 1.);
	param_signal.Name = param_signal.DefName + "_"+ id;
	param_signal.SRConfig  = "";
	
	param_signal.Electron_Tight_ID=id;
	param_signal.Electron_FR_ID = id;
      }
      else{
	//	if(SameCharge(GetElectrons("ElOpt_"+id, 10., 2.5)))FillAllElectronPlots("Bkg", "ElOpt_"+id  , GetElectrons("ElOpt_"+id, 10., 2.5), 1.);

	param_signal.Name = param_signal.DefName + "_ElOpt_"+ id;
        param_signal.SRConfig  = "";
	
        param_signal.Electron_Tight_ID="ElOpt_"+id;
        param_signal.Electron_FR_ID = "ElOptLoose_"+id;

      }
      param_signal.Muon_Tight_ID = "HNTightV2";
      param_signal.Muon_FR_ID = "HNLooseV1";
      
      RunULAnalysis(param_signal);

      param_signal.Name = param_signal.DefName;
      param_signal.SRConfig  = "";
    }
  }

  

  bool opt_IDMu=false;
  if(opt_IDMu){
    vector<TString> vMVAB;
    vector<TString> vMVAEC;

    for(unsigned int imva=0 ; imva < 2 ; imva++){
      double mva_d= 0.6 + double(imva)*0.2;
      TString mvaTS= DoubleToString(mva_d);
      vMVAB.push_back("MVAB"+mvaTS+"_");
      vMVAB.push_back("MVAB2"+mvaTS+"_");
      vMVAB.push_back("MVAB3"+mvaTS+"_");
      vMVAB.push_back("MVAB4"+mvaTS+"_");
      vMVAB.push_back("MVAB5"+mvaTS+"_");
      vMVAB.push_back("MVAB6"+mvaTS+"_");

    }
    for(unsigned int imva=0 ; imva < 2; imva++){
      double mva_d= 0.6 + imva*0.2;
      TString mvaTS= DoubleToString(mva_d);
      vMVAEC.push_back("MVAEC"+mvaTS+"_");
      vMVAEC.push_back("MVAEC2"+mvaTS+"_");
      vMVAEC.push_back("MVAEC3"+mvaTS+"_");
      vMVAEC.push_back("MVAEC4"+mvaTS+"_");
      vMVAEC.push_back("MVAEC5"+mvaTS+"_");
      vMVAEC.push_back("MVAEC6"+mvaTS+"_");
    }


    
    vector<TString> vIsoB = {"ISOB0p05","ISOB0p06","ISOB0p07","ISOB0p08","ISOB0p09","ISOB0p1","ISOB0p11","ISOB0p12","ISOB0p15"};
    vector<TString> vIsoEC = {"ISOEC0p05","ISOEC0p06","ISOEC0p07","ISOEC0p08","ISOEC0p09","ISOEC0p1","ISOEC0p11","ISOEC0p12","ISOEC0p15"};


    vector<TString> MuonsIDs;
    for(auto iMVAB : vMVAB){
      for(auto iMVAEC : vMVAEC){
	MuonsIDs.push_back(iMVAB+iMVAEC+"ISOB0p15_ISOEC0p1_DXYB1EC1");
	MuonsIDs.push_back(iMVAB+iMVAEC+"ISOB0p1_ISOEC0p1_DXYB1EC1");
	MuonsIDs.push_back(iMVAB+iMVAEC+"ISOB0p08_ISOEC0p08_DXYB1EC1");
      }
    }

 

    /*for(unsigned int ib = 0 ; ib < vIsoB.size(); ib++){
      for(unsigned int ie = 0 ; ie < vIsoEC.size(); ie++){
	if(ie  > ib) continue;
	TString iISOB = vIsoB[ib];
	TString iISOEC = vIsoEC[ie];

	MuonsIDs.push_back("MVAB0p7_MVAEC0p7_"+iISOB+"_"+iISOEC+"_DXYB1EC1");
	MuonsIDs.push_back("POGT_"+iISOB+"_"+iISOEC+"_DXYB1EC1");
	MuonsIDs.push_back("POGM_"+iISOB+"_"+iISOEC+"_DXYB1EC1");
      }
      }
    */
    MuonsIDs.push_back("HNTight_17028");
    MuonsIDs.push_back("HNTightV2");

    for(auto id : MuonsIDs){
      
      param_signal.Name = param_signal.DefName  + "_MuOpt_"+ id;
      param_signal.SRConfig  = "";
      
      param_signal.Muon_Tight_ID="MuOpt_"+id;
      param_signal.Muon_FR_ID = "MuOptLoose_"+id;
      param_signal.Electron_Tight_ID = "HNTightV2";
      param_signal.Electron_FR_ID = "HNLooseV4";
      
      RunULAnalysis(param_signal);
      
      param_signal.Name = param_signal.DefName;
      param_signal.SRConfig  = "";
    }
    
  }
  
  

  if(!IsData) RunSyst=false;
  if(RunSyst){
    TString param_signal_name = param_signal.Name;
    vector<AnalyzerParameter::Syst> SystList;// = GetSystList("Initial");

    for(auto isyst : SystList){
      param_signal.syst_ = AnalyzerParameter::Syst(isyst);
      
      param_signal.Name = "Syst_"+param_signal.GetSystType()+param_signal_name;
      param_signal.DefName = "Syst_"+param_signal.GetSystType()+param_signal_name;
      RunULAnalysis(param_signal);
    }
  }    


  return ;
}

void HNL_SignalLeptonOpt::RunULAnalysis(AnalyzerParameter param){

  if(run_Debug) cout << "HNL_SignalLeptonOpt::executeEvent " << endl;

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  
  // HL ID
  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5); 
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);

  TString el_ID = (RunFake) ?  param.Electron_FR_ID : param.Electron_Tight_ID ;
  TString mu_ID = (RunFake) ?  param.Muon_FR_ID :  param.Muon_Tight_ID ;

  double Min_Muon_Pt     = (RunFake) ? 3. : 5.;
  double Min_Electron_Pt = (RunFake) ? 7. : 10.;

  std::vector<Muon>       MuonCollTInit = GetMuons    ( param,mu_ID, Min_Muon_Pt, 2.4, RunFake);
  std::vector<Electron>   ElectronCollTInit = GetElectrons( param,el_ID, Min_Electron_Pt, 2.5, RunFake)  ;

  std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( MuonCollTInit,gens,param);
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,gens,param);
  
  //cout << "---------------------- " << endl;
  //cout << "ElectronCollT " << ElectronCollT.size() << " ID = " << el_ID << endl;
  //cout << "MuonCollT " << MuonCollT.size() << " ID = " << mu_ID << endl;
  
  
  std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

  std::vector<Tau>        TauColl        = GetTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

  // Creat Lepton vector to have lepton blind codes 

  Particle METv = GetvMET("PuppiT1xyCorr",param); // returns MET with systematic correction



  std::vector<FatJet> fatjets_tmp  = GetFatJets(param, param.FatJet_ID, 200., 5.);
  std::vector<FatJet> FatjetColl   = GetAK8Jetsv2(fatjets_tmp, 200., 2.7, true,  1., true, -999, true, 40., 130.,-999, ElectronCollV, MuonCollV);
  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);

  std::vector<Jet> JetCollLoose                    = GetAK4Jets(jets_tmp,     15., 4.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, FatjetColl);
  std::vector<Jet> BJetCollNLV                    = GetAK4Jets(jets_tmp,     20., 2.4, false,  0.4,0.8, "",   ElectronCollV,MuonCollV, FatjetColl);
  std::vector<Jet> JetColl                        = GetAK4Jets(jets_tmp,     20., 2.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, FatjetColl);
  std::vector<Jet> VBF_JetColl                    = GetAK4Jets(jets_tmp,     30., 4.7, true,  0.4,0.8, "",  ElectronCollV,MuonCollV, FatjetColl);  
  

  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl    = GetBJets(param,  BJetCollNLV, param_jetsM);
  double sf_btagM_NLV               = GetBJetSF(param, BJetCollNLV, param_jetsM);
  param.WriteOutVerbose=1; // Does not Fill Cutflow OR Region Plotter  
  
  
  if(!IsData )weight = weight*sf_btagM_NLV;
  
  if (MCSample.Contains("Type1")) {
    weight*=1/xsec;
  }
    
  FillSignalRegionForOpt(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, JetCollLoose, JetColl, VBF_JetColl,FatjetColl , BJetColl, ev,METv, param,weight);
  return;
  

}



 


HNL_SignalLeptonOpt::HNL_SignalLeptonOpt(){
  cout << "HNL_SignalRegionPlotter::HNL_SignalRegionPlotter  TMVA::Tools::Instance() " << endl;
  TMVA::Tools::Instance();
  cout << "Create Reader class " << endl;
  MVAReader = new TMVA::Reader();


}
 
HNL_SignalLeptonOpt::~HNL_SignalLeptonOpt(){

  delete MVAReader;
}




