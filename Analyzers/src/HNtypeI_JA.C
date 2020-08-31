#include "HNtypeI_JA.h"

void HNtypeI_JA::initializeAnalyzer(){

  //================
  //==== Userflags
  //================

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  Signal = HasFlag("Signal");
  PromptLeptonOnly = HasFlag("PromptLeptonOnly");


  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  HEM1516 = HasFlag("HEM1516");


  cout << "[HNtypeI_JA::initializeAnalyzer()] RunFake = " << RunFake << endl;
  cout << "[HNtypeI_JA::initializeAnalyzer()] RunCF = " << RunCF << endl;
  cout << "[HNtypeI_JA::initializeAnalyzer()] RunSyst = " << RunSyst << endl;
  cout << "[HNtypeI_JAr::initializeAnalyzer()] PromptLeptonOnly = " << PromptLeptonOnly << endl;


  MuonVetoIDs      = {"HNVeto2016", "POGLoose"};


  MuonTriggers.clear();
  MuonTriggersH.clear();
  ElectronTriggers.clear();
  SingleElectronTriggers.clear();
  EMuTriggers.clear();
  EMuTriggersH.clear();
  
  if(DataYear==2016){                                                                   // Lumi values for trigger weight (/pb)
    //MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v");                       // 27267.591112919
    //MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v");                     // 27267.591112919
    //MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v");                    // 35918.219492947
    //MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v");                  // 35918.219492947
    //MuonTriggersH.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v");                   // 35918.219492947
    //MuonTriggersH.push_back("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v");                 // 35918.219492947


    // Electron trigger adopted from AN2019_031
    //https://twiki.cern.ch/twiki/bin/view/CMS/EgHLTPathDetails#HLT_Ele23_Ele12_CaloIdL_Trac_AN1
    ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");          // 35918.219492947
    //SingleElectronTriggers.push_back("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v");
    SingleElectronTriggers.push_back("HLT_Ele27_WPTight_Gsf_v");
    SingleElectronTriggers.push_back("HLT_Ele115_CaloIdVT_GsfTrkIdT_v");
    //EMuTriggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v");          // 27267.591112919
    //EMuTriggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v");         // 27267.591112919
    //EMuTriggersH.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");      // 8650.628380028
    //EMuTriggersH.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");     // 8650.628380028

    // These are needed for applying lepton pT cuts 
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v");
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }
  else if(DataYear==2017){
    //    MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v");
    ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
    //    EMuTriggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //    EMuTriggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    // These are needed for applying lepton pT cuts 
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }
  else if(DataYear==2018){
    //MuonTriggers.push_back("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v");
    ElectronTriggers.push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v");
    //EMuTriggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //EMuTriggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    // These are needed for applying lepton pT cuts 
    //Mu8Ele23Triggers.push_back("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v");
    //Mu23Ele12Triggers.push_back("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");

    MuonPtCut1 = 20., MuonPtCut2 = 10.;
    ElectronPtCut1 = 25., ElectronPtCut2 = 15.;
    EMuPtCut1 = 25., EMuPtCut2 = 15.;
  }

  //==== b tagging
  std::vector<JetTagging::Parameters> jtps;
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  mcCorr->SetJetTaggingParameters(jtps);


  //==== PUReweight
  if(!IsDATA){
    TString datapath = getenv("DATA_DIR");

    TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_"+TString::Itoa(DataYear,10)+".root";
    TString PUhname = "PUReweight_"+TString::Itoa(DataYear,10);
    //==== Only for 2016 FastSim, use different one..
    if(DataYear==2016 && IsFastSim){
      PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_Fast_"+TString::Itoa(DataYear,10)+".root";
    }

    cout << "[HNtypeI_JA::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] PUhname = " << PUhname << endl;
    cout << "[HNtypeI_JA::initializeAnalyzer()] Signal = " << Signal << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void HNtypeI_JA::executeEvent(){

  
  //************************************************
  //-----------------------------------------------
  //-----------------------------------------------
  // Initial checks/functions that run before analyzer 
  //----------------------------------------------- 
  //-----------------------------------------------     
  //************************************************                                                                                                                            

  
  //==========================
  //==== Gen for genmatching
  //==========================


  //PrintGen(gens);


  //************************************************///
  //-----------------------------------------------////
  //-----------------------------------------------////
  // Prefire weight
  //-----------------------------------------------////
  //-----------------------------------------------////
  //************************************************///   

  weight_Prefire      = GetPrefireWeight( 0);
  weight_Prefire_Up   = GetPrefireWeight(+1);
  weight_Prefire_Down = GetPrefireWeight(-1);

  //************************************************///
  //-----------------------------------------------////
  //-----------------------------------------------////
  // NVTX setup
  //-----------------------------------------------////
  //-----------------------------------------------////
  //************************************************///
  N_VTX = nPV;
  if(!IsDATA){
    //==== nPileUp starts from 0
    N_VTX = nPileUp+1;
    //==== but histogram is as a function of nPileUp not nVTX
    int nPileUpForBin = nPileUp;
    if(DataYear==2018) nPileUpForBin = max(5, nPileUpForBin);
    int bin_pu = hist_PUReweight->FindBin(nPileUpForBin);
    weight_PU = hist_PUReweight->GetBinContent(bin_pu);

    weight_PU_Up = hist_PUReweight_Up->GetBinContent(bin_pu);
    weight_PU_Down = hist_PUReweight_Down->GetBinContent(bin_pu);

  }


  //************************************************/// 
  // setup vector of all objects
  //************************************************///
  AllMuons = GetAllMuons();
  AllElectrons = GetAllElectrons();
  AllJets = GetAllJets();
  AllFatJets = puppiCorr->Correct(GetAllFatJets());

  
  //************************************************///
  // make signal specifc plots
  //************************************************///
  if(Signal)MakeSignalPlots();


  //========================
  //==== AnalyzerParameter
  //========================

  //==== Central

  AnalyzerParameter param;


  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<TString> ELIDs;
  ELIDs.push_back("passMediumID");
  ELIDs.push_back("passTightID");
  ELIDs.push_back("passMVAID_iso_WP90");
  ELIDs.push_back("passMVAID_iso_WP80");
  

  for(unsigned int it_id=0; it_id<ELIDs.size(); it_id++){
    
    //************************************************///
    // setup leptton veto/loose/tight 
    //************************************************///

    TString MuonVetoID  = MuonVetoIDs.at(0);
    TString ElectronTightID = ELIDs.at(it_id);
    TString ElectronLooseID =  "passLooseID";
    TString ElectronVetoID  = "passLooseID";
    TString FakeRateID =  "passLooseID";

    param.Clear();

    param.syst_ = AnalyzerParameter::Central;
    param.Name = "HNtypeI_JA_"+ElectronTightID;
    param.MCCorrrectionIgnoreNoHist = false;

    
    //==== Muon ID
    param.Muon_Veto_ID        = MuonVetoID;
    param.Muon_ID_SF_Key      = "";
    param.Muon_ISO_SF_Key     = "";
    param.Muon_Trigger_SF_Key = "";
    param.Muon_MinPt = 15.;
    param.Muon_UsePtCone      = true;

    //==== Electron ID
    param.Electron_Tight_ID       = ElectronTightID;
    param.Electron_Loose_ID       = ElectronLooseID;
    param.Electron_Veto_ID        = ElectronVetoID;
    param.Electron_FR_ID          = FakeRateID;     // ID name in histmap_Electron.txt
    param.Electron_FR_Key         = "AwayJetPt40";  // histname
    param.Electron_MinPt = 15.;
    //param.Electron_ID_SF_Key      = "passTightID";
    param.Electron_ID_SF_Key      = "";
    param.Electron_Trigger_SF_Key = "";
    param.Electron_UsePtCone      = true;

    //==== Jet ID
    param.Jet_ID    = "HNTight";
    param.FatJet_ID = "HNTight";


    //************************************************///
    //
    // have list of IP cuts to modify ID
    //
    //************************************************///
    TString charge_const="";
    vector<TString> v_dx;
    v_dx.push_back("b0.025_ec0.025");
    v_dx.push_back("b0.025_ec0.050");
    v_dx.push_back("b0.050_ec0.050");
    v_dx.push_back("b0.050_ec0.075");
    v_dx.push_back("b0.050_ec0.100");
    v_dx.push_back("b999_ec999");
    vector<TString> v_dz;
    v_dz.push_back("b0.05_ec0.10");
    v_dz.push_back("b0.10_ec0.10");
    v_dz.push_back("b0.10_ec0.15");
    v_dz.push_back("b0.10_ec0.20");
    v_dz.push_back("b999_ec999");

    
    //************************************************///
    // loop over charge cut values 0/1
    ///************************************************///
    for(int icc=0; icc < 2; icc++){
      TString s_cc = (icc) ? "chargeconst" : "" ;

      //************************************************///
      // loop over vector of IP dx values 0.025-999
      //************************************************///
      for(unsigned int idx=0; idx < v_dx.size();idx++){
	
	double  d_dx_b(-999.), d_dx_e(-999.);
	if(v_dx[idx].Contains("b0.025")) d_dx_b= 0.025;
	if(v_dx[idx].Contains("b0.05")) d_dx_b= 0.05;
	if(v_dx[idx].Contains("b999")) d_dx_b= 999.;
	if(v_dx[idx].Contains("ec0.075")) d_dx_e= 0.075;
	if(v_dx[idx].Contains("ec0.05")) d_dx_e= 0.05;
	if(v_dx[idx].Contains("ec0.1")) d_dx_e= 0.1;
	if(v_dx[idx].Contains("ec999")) d_dx_e= 999.;
	
	for(unsigned int idz=0; idz < v_dz.size();idz++){
	  //************************************************///
	  // loop over vector of IP Z values
	  //************************************************///
	  double d_dz_b(-999.), d_dz_e(-999.);
	  if(v_dz[idz].Contains("b0.05")) d_dz_b= 0.05;
	  if(v_dz[idz].Contains("b0.10")) d_dz_b= 0.1;
	  if(v_dz[idz].Contains("b999")) d_dz_b= 999.;
	  if(v_dz[idz].Contains("ec0.10")) d_dz_e= 0.10;
	  if(v_dz[idz].Contains("ec0.15")) d_dz_e= 0.15;
	  if(v_dz[idz].Contains("ec0.20")) d_dz_e= 0.2;
	  if(v_dz[idz].Contains("ec999")) d_dz_e= 999.;
	  
	  //************************************************///
	  // change param name to link to IP/CC cuts
	  //************************************************///
	  param.Name = "HNtypeI_JA_"+ElectronTightID+s_cc+v_dx[idx]+v_dz[idz];
	  
	  //************************************************///
	  // run event
	  //************************************************///
	  executeEventFromParameter(param, ElectronTightID, s_cc, d_dx_b, d_dx_e, d_dz_b, d_dz_e);
	}
      }
    }
  } 
}

void HNtypeI_JA::executeEventFromParameter(AnalyzerParameter param, TString El_ID, TString cc, double dx_b, double dx_e, double dz_b, double dz_e){
  
  //************************************************///
  // plot CR species cuts for All plots to be made
  //************************************************///
  bool plot_CR=false;

  if (cc && dx_b == 0.05 && dx_e==0.1 && dz_b == 0.1 && dz_e == 0.2) plot_CR=true;
  if (!cc && dx_b == 0.05 && dx_e==0.1 && dz_b == 0.1 && dz_e == 0.2) plot_CR=true;

  bool nom_ID = (El_ID == "passTightID") ? true : false; 
  if(!(cc && dx_b == 0.05 && dx_e==0.1 && dz_b == 0.1 && dz_e == 0.2)) nom_ID=false;

  //************************************************///
  // IsCentral true means nominal analysis settings
  //************************************************///
  bool IsCentral = !( param.Name.Contains("Syst_") );
 
  //************************************************///
  // setup event level objects
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  double weight = 1.;
  //************************************************///
  
  if(Signal) weight_norm_1invpb=1.;
  
  //************************************************///
  // MC weights initial settings
  // -- set to full trigger luminosity for data taking year
  // -- apply MC gen weights
  // -- apply prefire weight
  //************************************************///
  if(!IsDATA){

    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();

    //==== Apply Prefire
    weight *= weight_Prefire;
    if (nom_ID) FillWeightHist("prefire_"+param.Name, weight_Prefire);

  }

  //************************************************///  
  //==== No Cut cutflow entry, get nevents in sample
  //************************************************///   

  FillCutFlow(IsCentral, "CutFlow", "NoCut_"+param.Name, weight);
  FillCutFlow(IsCentral, "CutFlow", "NoCut_no_weight_"+param.Name, 1.);

  //************************************************///   
  //==== Event selecitions : MET Filters
  //************************************************///   

  if(!PassMETFilter()) return;

  FillCutFlow(IsCentral, "CutFlow", "METFilter_"+param.Name, weight);
  
  bool PassDoubleElectronTrig = ev.PassTrigger(ElectronTriggers);
  bool PassSingleElectronTrig = ev.PassTrigger(SingleElectronTriggers);
  
  if(PassDoubleElectronTrig || PassSingleElectronTrig )   FillCutFlow(IsCentral, "CutFlow", "TriggerOR_"+param.Name, weight);
  
  if(PassDoubleElectronTrig)   FillCutFlow(IsCentral, "CutFlow", "DiElTrigger_"+param.Name, weight);
  
  //double trigger_sf_DoubleElectron = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, Tight_electrons, SystDir_ElectronTriggerSF);

  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon> this_AllMuons = AllMuons;
  vector<Muon> this_AllTunePMuons = AllTunePMuons;
  vector<Jet> this_AllJets = AllJets;
  vector<FatJet> this_AllFatJets = AllFatJets;

  //************************************************///   
  // setup systematic variables
  //************************************************///   
  int SystDir_ElectronIDSF(0);
  int SystDir_ElectronRecoSF(0);

  //************************************************///   
  //==== PU reweight for MC
  //************************************************///   

  double weight_nopu=weight;
  double weight_pu_up = weight;
  double weight_pu_down = weight;
  if(!IsDATA){

    double this_pureweight = 1.;
    double this_pureweight_up = 1.;
    double this_pureweight_down = 1.;
    this_pureweight = weight_PU;
    this_pureweight = weight_PU_Down;
    this_pureweight = weight_PU_Up;

    weight *= this_pureweight;
    weight_pu_up *= this_pureweight_up;
    weight_pu_down *= this_pureweight_down;
  }
  
  //************************************************///   
  // 
  // setup analysis level lepton vectors
  //
  //************************************************///   
  
  std::vector<Electron> myelectrons = SelectElectrons(this_AllElectrons, El_ID, param.Electron_MinPt, 2.4, cc, dx_b, dx_e, dz_b, dz_e );
  std::vector<Electron> veto_electrons = SelectElectrons(this_AllElectrons, param.Electron_Veto_ID, param.Electron_MinPt, 2.4);
  std::vector<Muon> mymuons = SelectMuons(this_AllMuons,param.Muon_Veto_ID, param.Muon_MinPt, 2.4);

  //************************************************///   
  // when running data level fakes set this true
  //************************************************///   

  
  if(PromptLeptonOnly){
    myelectrons = ElectronPromptOnly(myelectrons, gens);
    mymuons = MuonPromptOnly(mymuons, gens);
  }
 
  std::sort(myelectrons.begin(), myelectrons.end(), PtComparing);
  std::sort(mymuons.begin(), mymuons.end(), PtComparing);
  
  

  //************************************************///   
  //==== Jets vectors
  // - jet ID is Tight POG https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2016
  // - fat jet ID is: https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging
  // - ------ Tight POG
  // - ------ tau21 < 0.75   /// this needs checking as values changed 
  // - ------ 40. < SDmass < 130 GeV
  //************************************************///   

  std::vector<FatJet>   fatjets         = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 2.5);
  std::sort(fatjets.begin(), fatjets.end(), PtComparing);

  std::vector<Jet>      jets         = SelectJets(this_AllJets, param.Jet_ID, 20., 2.5);
  std::sort(jets.begin(), jets.end(), PtComparing);

  //************************************************///
  // jet up b jet variable
  // --- use medium Deep CVS
  //************************************************///
  int NBJets=0;

  for(unsigned int i=0; i<jets.size(); i++){

    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium,                                                    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  
  //************************************************///
  //@@@@ Apply lepton scale factors
  //************************************************///

  if(!IsDATA){

    //==== lepton scale factors here

    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;
    for(unsigned int i=0; i<myelectrons.size(); i++){
  
      TString Electron_ID_SF_Key      =  El_ID;
      
      double this_recosf = mcCorr->ElectronReco_SF(myelectrons.at(i).scEta(),myelectrons.at(i).Pt(), SystDir_ElectronRecoSF);
      double this_idsf = mcCorr->ElectronID_SF(Electron_ID_SF_Key, myelectrons.at(i).scEta(), myelectrons.at(i).Pt(), SystDir_ElectronIDSF);
      weight *= this_recosf*this_idsf;
      if (nom_ID) FillWeightHist("el_reco_sf_"+param.Name, this_recosf);
      if (nom_ID) FillWeightHist("el_id_sf_"+param.Name, this_idsf);

    }
  }
    
  //************************************************///   
  //==== Sum Pts : ST/HT
  //************************************************///   

  double HT(0.), ST(0.);
  for(unsigned int i=0; i<jets.size(); i++){
    HT += jets.at(i).Pt();
    ST += jets.at(i).Pt();
  }
  for(unsigned int i=0; i<fatjets.size(); i++){
    HT += fatjets.at(i).Pt();
    ST += fatjets.at(i).Pt();
  }
  for(unsigned int i=0; i<myelectrons.size(); i++){
    ST += myelectrons[i].Pt();
  }
  ST += METv.Pt();

  
  //************************************************///   
  // set up strings for CR hist
  //************************************************///   
  TString reg="";
  TString charge_s="";
  TString met_label = "";
  

  //************************************************///   
  // select trigger
  //************************************************///   
  //if(!(PassDoubleElectronTrig || PassSingleElectronTrig )) return;
  if(!(PassDoubleElectronTrig)) return;


  if(plot_CR){
    FillHist( param.Name+"/nelectrons"+param.Name, myelectrons.size() ,weight, 5, 0., 5,"N_{electron}");
    FillHist( param.Name+"nmuons"+param.Name, mymuons.size() ,weight, 5, 0., 5,"N_{muon}");
    FillHist( param.Name+"nelectrons_veto"+param.Name, veto_electrons.size() ,weight, 5, 0., 5,"N_{electron}");
  }

  //************************************************///   
  // ZZ CR plots
  //************************************************///   
  
  if(myelectrons.size() ==4 && mymuons.size() == 0){
    if(myelectrons[0].Pt() > 25. && myelectrons[3].Pt()  > 15.) {
      if(NBJets ==0){
	bool m_llos_l10(false), z_cr_pass(false);
	Particle Z1Cand;
	Particle Z2Cand;

	for(unsigned int iel =0; iel < myelectrons.size()-1 ; iel++){
	  for(unsigned int iel2 =1; iel2 < myelectrons.size() ; iel2++){
	    if(iel== iel2) continue;
	    Z1Cand = myelectrons.at(iel) + myelectrons.at(iel2);
	    if(myelectrons.at(iel).Charge() != myelectrons.at(iel2).Charge()){
	      if(Z1Cand.M() < 10) m_llos_l10=true;
	    }
	    int el1(-9), el2(-9);
	    if(el1 ==0 && iel2==1){ el1=2; el2=3;    Z2Cand = myelectrons.at(2) + myelectrons.at(3);}
	    if(el1 ==0 && iel2==2){ el1=1; el2=3;    Z2Cand = myelectrons.at(1) + myelectrons.at(3);}
	    if(el1 ==0 && iel2==3){ el1=1; el2=2;    Z2Cand = myelectrons.at(1) + myelectrons.at(2);}
	    if(el1 ==1 && iel2==2){ el1=0; el2=3;    Z2Cand = myelectrons.at(0) + myelectrons.at(3);}
            if(el1 ==1 && iel2==3){ el1=0; el2=2;    Z2Cand = myelectrons.at(0) + myelectrons.at(2);}
	    if(el1 ==2 && iel2==3){ el1=0; el2=1;    Z2Cand = myelectrons.at(0) + myelectrons.at(1);}

	    if(myelectrons.at(iel).Charge() != myelectrons.at(iel2).Charge()){
	      if(myelectrons.at(el1).Charge() != myelectrons.at(el2).Charge()){
		if(fabs(Z1Cand.M() - 90.1) < 15.){
		  if(fabs(Z2Cand.M() - 90.1) < 15.){
		    z_cr_pass=true;
		  }
		}
	      }
	    }
	    
	  }
	}
	if(!m_llos_l10 && z_cr_pass)      FillRegionPlots(plot_CR,"ZZ_cr"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight);
      }
    }
  }

  //************************************************///   
  // WZ CR plots
  //************************************************///   
  if(myelectrons.size() ==3 && mymuons.size() == 0){
    if(myelectrons[0].Pt() > 25. && myelectrons[2].Pt()  > 15.) {
      if(METv.Pt() > 50.){
	if(NBJets ==0){
	  bool m_llos_l10(false);
	  Particle ZCand;
	  int iel_Z1(0), iel_Z2(0), iel_W(0);
	  float mass_diff_tmp=999999.;
	  for(unsigned int iel =0; iel < myelectrons.size()-1 ; iel++){
	    for(unsigned int iel2 =1; iel2 < myelectrons.size() ; iel2++){
	      if(iel== iel2) continue;
	      Particle llCand = myelectrons.at(iel) + myelectrons.at(iel2);
	      if(myelectrons.at(iel).Charge() != myelectrons.at(iel2).Charge()){
		if(llCand.M() < 10) m_llos_l10=true;
		if(fabs(llCand.M() - 90.1) < mass_diff_tmp){
		  mass_diff_tmp = fabs(llCand.M() - 90.1) ;
		  iel_Z1=iel;
		  iel_Z2=iel2;
		  ZCand = myelectrons.at(iel) + myelectrons.at(iel2);
		}
	      }
	    }
	  }
	  if(!m_llos_l10){
	    
	    if((iel_Z1 + iel_Z2)==3) iel_W=0;
	    if((iel_Z1 + iel_Z2)==2) iel_W=1;
	    if((iel_Z1 + iel_Z2)==1) iel_W=2;
	    
	    
	    if(mass_diff_tmp < 10.){
	      if(MT(METv, myelectrons.at(iel_W)) > 20.){
		if((myelectrons.at(0) + myelectrons.at(1) + myelectrons.at(2)).M()  > 105.){
		  FillRegionPlots(plot_CR,"WZ_cr"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight);
		}
	      }
	    }
	  }
	}
      }
    }
  }

  //************************************************///    
  //************************************************///   
  //************************************************///   
  // Dilepton event selection 
  //************************************************///   
  //************************************************///   
  //************************************************///   
  if(myelectrons.size()!=2)  return;
  if(mymuons.size() > 0)  return;

  //************************************************///   
  // charge_s is string to label ++ vs +-
  //************************************************///   
  charge_s= (myelectrons[0].Charge() ==  myelectrons[1].Charge() )  ?  charge_s = "_same_sign" : "_opposite_sign";

  //************************************************///   
  // apply trigger level pt cuts
  //************************************************///   
  if(myelectrons[0].Pt() < 25.) return;
  if(myelectrons[1].Pt() < 15.) return;


  //************************************************///   
  // create proxy for Z/W candidates
  //************************************************///   
  Particle llCand = myelectrons.at(0) + myelectrons.at(1);
  Particle llJCand = myelectrons.at(0) + myelectrons.at(1);
  Particle lljjCand = myelectrons.at(0) + myelectrons.at(1);
  
  if(fatjets.size() > 0) llJCand+=fatjets[0];

  //************************************************///   
  // select Z peak
  //************************************************///   
  if(llCand.M() < 100. && llCand.M() > 60.){
    FillRegionPlots(plot_CR,"DYCR"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight);
    // PU syst
    //FillRegionPlots(plot_CR,"DYCR_pu_up"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight_pu_up);
    //FillRegionPlots(plot_CR,"DYCR_pu_down"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight_pu_down);
    //1FillRegionPlots(plot_CR,"DYCR_nopu"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight_nopu);
  }
  else   FillRegionPlots(plot_CR,"presel"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight);

  //************************************************///   
  // SR selections
  //************************************************///   

  double met2_st = METv.Pt()*METv.Pt()/ ST;
  if(met2_st < 15 && NBJets == 0 ){
    if( jets.size() > 1 && fatjets.size() == 0) {  

      FillSigRegionPlots1("SR1_highmass_"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 999.,999.,999.,999.,999.,999.,999.,999.,999.,999.,999.);
      FillSigRegionPlots1("SR1_mn100_"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 4, 25., 3.1, 25., 15., 50.,120., 120., 50., 110., 6.);
      FillSigRegionPlots1("SR1_mn200_"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 4, 25., 3.1, 55., 40., 50.,120., 220.,160., 225.,  6.);
      FillSigRegionPlots1("SR1_mn500_"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 4, 25., 999., 125., 65., 50.,120., 560.,400., 555., 6.);
      FillSigRegionPlots1("SR1_mn1100_"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 4, 25., 999., 125., 999., 50.,120., 760.,400., 1490., 6.);
      
    }
    if(fatjets.size() > 0) {

      FillSigRegionPlots2("SR2_highmass"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight,  999.,  999.,  999.,  999.,  999.,  999.,  999.);
      FillSigRegionPlots2("SR2_highmass"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 25.,15., 40.,130., 90., 220., 15.);
      FillSigRegionPlots2("SR2_highmass"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 100., 20., 40., 130., 173., 220., 15.); 
      FillSigRegionPlots2("SR2_highmass"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 120., 35., 40., 130., 440., 565., 15.); 
      FillSigRegionPlots2("SR2_highmass"+ charge_s , met_label +param.Name, jets,  fatjets,  myelectrons, mymuons,  METv, nPV, weight, 140., 999.,40., 130., 1035., 1395., 15.); 


    }
  }
}

HNtypeI_JA::HNtypeI_JA(){

  RunFake = false;

}

HNtypeI_JA::~HNtypeI_JA(){

}

void HNtypeI_JA::MakeSignalPlots(){
  bool mupt_pass=true;
  int mother_nu(0);
  int ind_w(0);
  TString  mu_ch="";
  if(Signal){
    gens = GetGens();
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      vector<int> history = TrackGenSelfHistory(gen, gens);

      if(gens.at(gen.MotherIndex()).PID() == 9900012){
        if(gen.PID() == 13) mu_ch="minus";
        if(gen.PID() == -13) mu_ch="plus";

      }

      if(fabs(gen.PID()) == 24 && gens.at(gen.MotherIndex()).PID() == 9900012){
        ind_w= i;
      }

      if(gen.PID() == 9900012 &&  gen.Status() == 22) {
        mother_nu= gen.MotherIndex();
      }
    }
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      vector<int> history = TrackGenSelfHistory(gen, gens);


      if(gen.MotherIndex() == ind_w && gen.Status() == 23) {
        FillHist( ("SignalGen/" + mu_ch+"jet_w_pt").Data(), gens.at(i).Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"jet_w_eta", gens.at(i).Eta(),  1., 60, -3., 3,"");
        FillHist( "SignalGen/" + mu_ch+"w_dijet_mass", gens.at(gen.MotherIndex()).M(),  1., 200, 0., 200.,"");
      }
      if(gen.PID() == 9900012){
        FillHist( "SignalGen/" + mu_ch+"Neutrino_pt", gens.at(i).Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"Neutrino_mass", gens.at(i).M(), 1., 200, 0., 2000.,"");
        FillHist( "SignalGen/" + mu_ch+"Neutrino_eta", gens.at(i).Eta(), 1., 60, -3., 3,"");
      }
      if(fabs(gen.PID()) != 13) continue;
      //if(gen.PID() != 13) continue;                                                                                                                                           


      if(gens.at(gen.MotherIndex()).PID() == 9900012) {
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_Neutrino_pt", gen.Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_Neutrino_eta", gen.Eta(),  1., 60, -3., 3,"");

        for(unsigned int j=2; j<gens.size(); j++){
          Gen gen2 = gens.at(j);
          vector<int> history = TrackGenSelfHistory(gen2, gens);
          if(j==ind_w){
            FillHist( "SignalGen/" + mu_ch+"Lep_mu_Neutrino_dr_W_Neutrino", gen2.DeltaR(gen), 1., 100, 0., 10.,"");
            FillHist( "SignalGen/" + mu_ch+"Mass_mu_Neutrino__W_Neutrino", (gen2 + gen).M(), 1., 150, 0., 1500.,"");
          }
	}

        if(gen.Pt() < 50.) mupt_pass = false;
      }
      else if(gen.MotherIndex() == mother_nu){
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_W_pt", gen.Pt(), 1., 200, 0., 400.,"");
        FillHist( "SignalGen/" + mu_ch+"Lep_mu_W_eta", gen.Eta(),  1., 60, -3., 3,"");
        if(gen.Pt() < 50.) mupt_pass = false;

      }
    }

    bool elpt_pass=true;
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      vector<int> history = TrackGenSelfHistory(gen, gens);

      if(fabs(gen.PID()) != 11) continue;

      if(gens.at(gen.MotherIndex()).PID() == 9900012) {
        FillHist( "SignalGen/" + mu_ch+"Lep_el_Neutrino_pt", gen.Pt(), 1., 200, 0., 400.,"");
        if(gen.Pt() < 35.) elpt_pass = false;
      }
      else if(gen.MotherIndex() == mother_nu){

        FillHist( "SignalGen/" + mu_ch+"Lep_el_W_pt", gen.Pt(), 1., 200, 0., 400., "");

        if(gen.Pt() < 35.) elpt_pass = false;

      }
    }

    if(mupt_pass)       FillHist( "SignalGen/" + mu_ch+"Lep_muon_50gev_pt", 1. ,1., 2, 0., 2,"");
    if(elpt_pass)       FillHist( "SignalGen/" + mu_ch+"Lep_electron_35gev_pt", 1., 1., 2, 0., 2.,"");
  }



}
void HNtypeI_JA::FillSigRegionPlots1(TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7, double var8, double var9, double var10, double var11){

  if(els.size() < 2) return;
  if(jets.size() < 2) return;
  if(jets.size() >= var1) return;
  if(jets[0].Pt() < var2)return;

  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
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
  ST += els[0].Pt() + els[1].Pt() + met.Pt();
  double met2 = met.Pt()*met.Pt();
  Particle Wcand = jets[m] + jets[n];
  Particle W1cand = jets[m] + jets[n]+els[0] + els[1];
  Particle N1cand = jets[m] + jets[n]+els[0];
  Particle N2cand = jets[m] + jets[n]+els[1];
  if(Wcand.DeltaR(els[1]) > var3) return;
  if(els[0].Pt() < var4) return;
  if(els[1].Pt() < var5) return;
  if(Wcand.M() < var6) return;
  if(Wcand.M() > var7) return;
  if(W1cand.M() < var8) return;
  if(N1cand.M() < var9 && N2cand.M()  < var9) return;
  if(N1cand.M()  > var10 && N2cand.M()  > var10) return;
  if(met2/ST > var11) return;

  FillHist( label_2+ "/"+ label_1 +  "_reco_mlljj_"  + label_2,  W1cand.M(),  w, 200, 0, 2000, "Reco M_{lljj}");
  FillHist( label_2+ "/"+ label_1 +  "_reco_ml1jj_"  + label_2,  N1cand.M(),  w, 200, 0, 2000, "Reco M_{l1jj}");
  FillHist( label_2+ "/"+ label_1 +  "_reco_ml2jj_"  + label_2,  N2cand.M(),  w, 200, 0, 2000, "Reco M_{l2jj}");
  FillHist( label_2+ "/"+ label_1 +  "_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");

}

void HNtypeI_JA::FillSigRegionPlots2(TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w,  double var1,  double var2, double var3, double var4, double var5, double var6, double var7){


  if(els.size() < 2) return;
  if(fatjets.size() == 0) return;

  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  double ST(0.);
  for(UInt_t emme=0; emme<jets.size(); emme++){
    ST += jets[emme].Pt();
  }
  for(UInt_t emme=0; emme<fatjets.size(); emme++){
    ST += fatjets[emme].Pt();
    dijetmass_tmp = fatjets[emme].SDMass();
    if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
    }
  }
  ST += els[0].Pt() + els[1].Pt() + met.Pt();
  double met2 = met.Pt()*met.Pt();
  Particle Wcand = fatjets[m];
  Particle W1cand = fatjets[m] +els[0] + els[1];
  Particle N1cand = fatjets[m] +els[0];
  Particle N2cand = fatjets[m] +els[1];

  if(els[0].Pt() < var1) return;
  if(els[1].Pt() < var2) return;
  if(Wcand.M() < var3) return;
  if(Wcand.M() > var4) return;
  if(N1cand.M()  < var5 && N2cand.M()  < var5) return;
  if(N1cand.M()  > var6 && N2cand.M()  > var6) return;
  if(met2/ST > var7) return;

  FillHist( label_2+ "/"+ label_1 +  "_reco_mlljj_"  + label_2,  W1cand.M(),  w, 200, 0, 2000, "Reco M_{lljj}");
  FillHist( label_2+ "/"+ label_1 +  "_reco_ml1jj_"  + label_2,  N1cand.M(),  w, 200, 0, 2000, "Reco M_{l1jj}");
  FillHist( label_2+ "/"+ label_1 +  "_reco_ml2jj_"  + label_2,  N2cand.M(),  w, 200, 0, 2000, "Reco M_{l2jj}");
  FillHist( label_2+ "/"+ label_1 +  "_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");

}
void HNtypeI_JA::FillRegionPlots(bool plotCR, TString label_1, TString label_2,  std::vector<Jet> jets, std::vector<FatJet> fatjets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met, double nvtx,  double w){


  if(!plotCR) return;

  int NBJets=0;
  for(unsigned int i=0; i<jets.size(); i++){
    FillHist( label_2+ "/"+ label_1 + "_jet_pt_"+ label_2,  jets[i].Pt() , w, 60, 0., 300., "Jet p_{T} GeV");

    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium, 
						    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }
  
  if(els.size() < 1) return;
  Particle llCand = els.at(0) + els.at(1);
  Particle llJCand ;
  Particle lljjCand ;

  if(fatjets.size() > 0) llJCand= els.at(0) + els.at(1)+ fatjets[0];
  if(jets.size() ==1 ) lljjCand += jets[0];
  else   if(jets.size() > 1){

    float dijetmass_tmp=999.;
    float dijetmass=9990000.;
    int m=-999;
    int n=-999;

    for(UInt_t emme=0; emme<jets.size(); emme++){
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
    lljjCand = els.at(0) + els.at(1) + jets[m]+jets[n];
    
    FillHist( label_2+ "/"+ label_1 +  "_reco_mjj_"  + label_2,   (jets[m]+jets[n]).M(),  w, 250, 0, 250, "Reco M_{jj}");
    if(els.size()> 0)FillHist( label_2+ "/"+ label_1 +  "_reco_ml1jj_"  + label_2,   (els[0] + jets[m]+jets[n]).M(),  w, 250, 0, 1000, "Reco M_{l1jj}");
    if(els.size()> 1){
      FillHist( label_2+ "/"+ label_1 +  "_reco_ml2jj_"  + label_2,   (els[1] + jets[m]+jets[n]).M(),  w, 250, 0, 1000, "Reco M_{l2jj} ");
      FillHist( label_2+ "/"+ label_1 +  "_reco_mlljj_"  + label_2,  lljjCand.M(),  w, 250, 0, 1000, "Reco M_{lljj}");
    }
  }
  if(fatjets.size() > 0)FillHist( label_2+ "/"+ label_1 +  "_reco_mllJ_"  + label_2,  llJCand.M(),  w, 250, 0, 1000, "Reco M_{llJ}");
  
  FillHist( label_2+ "/"+ label_1 + "_nPV_"+ label_2,  nvtx , w, 60, 0., 60.);
  FillHist( label_2+ "/"+ label_1 +  "_njets_" + label_2, jets.size() , w, 10, 0., 10., "N_{jets}");
  
  FillHist( label_2+ "/"+ label_1 +  "_met_" + label_2, met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( label_2+ "/"+ label_1 +  "_nbjets_" + label_2, NBJets , w, 5, 0., 5., "N_{b jets}");
  FillHist( label_2+ "/"+ label_1 +  "_ll_mass_" + label_2,  llCand.M(), w, 200, 0., 400., "M_{ll} GeV");
  
  
  for(unsigned int i=0; i < fatjets.size(); i++){
    FillHist( label_2+ "/"+ label_1 + "_fatjet_pt_"+ label_2,  fatjets[i].Pt() , w, 60, 0., 300., "AK8 Jet p_{T} GeV");
    FillHist( label_2+ "/"+ label_1 + "_fatjet_pt_"+ label_2,  fatjets[i].SDMass() , w, 100, 0., 500., "Mass_{softdrop} GeV");
    FillHist( label_2+ "/"+ label_1 + "_fatjet_pt_"+ label_2,  fatjets[i].PuppiTau2()/ fatjets[i].PuppiTau1() , w, 400, 0., 2., "#tau_{21}");
    
  }
  for(unsigned int i=0; i < els.size(); i++){
    FillHist( label_2+ "/"+ label_1 + "_el_pt_" + label_2  , els.at(i).Pt() , w, 500, 0., 1000., "el p_{T} GeV");
    FillHist( label_2+ "/"+ label_1 + "_el_dxy_" + label_2  , els.at(i).dXY() , w, 500, -1., 1., "dXY");
    FillHist( label_2+ "/"+ label_1 + "_el_dz_" + label_2  , els.at(i).dZ() , w, 500, -1., 1., "dZ");
    FillHist( label_2+ "/"+ label_1 + "_el_reliso_" + label_2  , els.at(i).RelIso() , w, 50, 0., 1., "R_{ISO} GeV");
    FillHist( label_2+ "/"+ label_1 + "_el_eta_"+ label_2  , els.at(i).Eta() , w, 60, -3., 3.,"el #eta");
    FillHist( label_2+ "/"+ label_1 +  "_el_mt_" + label_2, MT(met, els[i])  , w, 200, 0., 400.,"M_{T} GeV");
    FillHist( label_2+ "/"+ label_1 +  "_el_cc_" + label_2, els[i].IsGsfCtfScPixChargeConsistent()  , w, 2, 0., 2.,"Charge Cons.");
    FillHist( label_2+ "/"+ label_1 +  "_el_convveto_" + label_2, els[i].PassConversionVeto()  , w, 2, 0., 2.,"Conv. veto.");
    FillHist( label_2+ "/"+ label_1 + "_el_mva_" + label_2  , els.at(i).MVAIso() , w, 100, -1., 1., "el mva");
    FillHist( label_2+ "/"+ label_1 + "_el_mvanoniso_" + label_2  , els.at(i).MVANoIso() , w, 100, -1., 1., "el mva nonIso");
  }
  return;
}

double HNtypeI_JA::MT(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
  
}


std::vector<Jet> HNtypeI_JA::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    Jet this_jet = jets.at(i);

    bool HasLeptonInside = false;

    for(unsigned int j=0; j<els.size(); j++){
      if( this_jet.DeltaR( *(els.at(j)) ) < dR ){
        HasLeptonInside = true;
        break;
      }
    }
    if(HasLeptonInside) continue;

    for(unsigned int j=0; j<mus.size(); j++){
      if( this_jet.DeltaR( *(mus.at(j)) ) < dR ){
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

bool HNtypeI_JA::LeptonPassID(Lepton &lepton, TString ID){

  if(lepton.LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)&lepton;
    return el->PassID(ID);
  }
  else if(lepton.LeptonFlavour()==Lepton::MUON){
    Muon *mu = (Muon *)&lepton;
    return mu->PassID(ID);
  }
  else{
    cerr << "[HNtypeI_JA::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}


void HNtypeI_JA::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNtypeI_JA::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

double HNtypeI_JA::LSFSF(int lepflav, int dir){

  //==== lepflav = 0 : electron-jet
  //==== lepflav = 1 : mu-jet

  if(lepflav==0){
    if(dir==0)     return 1.04;
    else if(dir>0) return 1.04+0.03;
    else           return 1.04-0.04;
  }
  else if(lepflav==1){
    if(dir==0)     return 1.01;
    else if(dir>0) return 1.01+0.03;
    else           return 1.01-0.03;
  }
  else{
    cerr << "[HNtypeI_JA::LSFSF] wrong lepflav : " <<  lepflav << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }

}
