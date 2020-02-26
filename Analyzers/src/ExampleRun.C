#include "ExampleRun.h"

ExampleRun::ExampleRun(){

}

void ExampleRun::initializeAnalyzer(){

  //================================================================
  //====  Example 1
  //====  Dimuon Z-peak events with two muon IDs, with systematics
  //================================================================

  //==== if you use "--userflags RunSyst" with SKFlat.py, HasFlag("RunSyst") will return "true"
  RunSyst = HasFlag("RunSyst");
  cout << "[ExampleRun::initializeAnalyzer] RunSyst = " << RunSyst << endl;

  //==== Dimuon Z-peak with two muon IDs
  //==== I defined "vector<TString> MuonIDs;" in Analyzers/include/ExampleRun.h
  MuonIDs = {
    "POGMedium",
    "POGLoose"
  };
  //==== corresponding Muon ID SF Keys for mcCorr->MuonID_SF()
  MuonIDSFKeys = {
    "NUM_MediumID_DEN_genTracks",
    "NUM_MediumID_DEN_genTracks"
  };

  //==== At this point, sample informations (e.g., IsDATA, DataStream, MCSample, or DataYear) are all set
  //==== You can define sample-dependent or year-dependent variables here
  //==== (Example) Year-dependent variables
  //==== I defined "TString IsoMuTriggerName;" and "double TriggerSafePtCut;" in Analyzers/include/ExampleRun.h 
  //==== IsoMuTriggerName is a year-dependent variable, and you don't want to do "if(Dataer==~~)" for every event (let's save cpu time).
  //==== Then, do it here, which only ran once for each macro
  if(DataYear==2016){
    IsoMuTriggerName = "HLT_IsoMu24_v";
    TriggerSafePtCut = 26.;
  }
  else if(DataYear==2017){
    IsoMuTriggerName = "HLT_IsoMu27_v";
    TriggerSafePtCut = 29.;
  }

  cout << "[ExampleRun::initializeAnalyzer] IsoMuTriggerName = " << IsoMuTriggerName << endl;
  cout << "[ExampleRun::initializeAnalyzer TriggerSafePtCut = " << TriggerSafePtCut << endl;

  //==== B-Tagging
  //==== add taggers and WP that you want to use in analysis
  std::vector<JetTagging::Parameters> jtps;
  //==== If you want to use 1a or 2a method,
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb) );
  //==== set
  mcCorr->SetJetTaggingParameters(jtps);

  //================================
  //==== Example 2
  //==== Using new PDF
  //==== It consumes so much time, so only being actiavted with --userflags RunNewPDF
  //================================

  RunNewPDF = HasFlag("RunNewPDF");
  cout << "[ExampleRun::initializeAnalyzer] RunNewPDF = " << RunNewPDF << endl;
  if(RunNewPDF && !IsDATA){

    LHAPDFHandler LHAPDFHandler_Prod;
    LHAPDFHandler_Prod.CentralPDFName = "NNPDF31_nnlo_hessian_pdfas";
    LHAPDFHandler_Prod.init();

    LHAPDFHandler LHAPDFHandler_New;
    LHAPDFHandler_New.CentralPDFName = "NNPDF31_nlo_hessian_pdfas";
    LHAPDFHandler_New.ErrorSetMember_Start = 1; 
    LHAPDFHandler_New.ErrorSetMember_End = 100; 
    LHAPDFHandler_New.AlphaSMember_Down = 101; 
    LHAPDFHandler_New.AlphaSMember_Up = 102; 
    LHAPDFHandler_New.init();

    pdfReweight->SetProdPDF( LHAPDFHandler_Prod.PDFCentral );
    pdfReweight->SetNewPDF( LHAPDFHandler_New.PDFCentral );
    pdfReweight->SetNewPDFErrorSet( LHAPDFHandler_New.PDFErrorSet );
    pdfReweight->SetNewPDFAlphaS( LHAPDFHandler_New.PDFAlphaSDown, LHAPDFHandler_New.PDFAlphaSUp );

  }

  //================================================
  //==== Example 3
  //==== How to estimate xsec errors (PDF & Scale)
  //==== For example, MET
  //================================================

  RunXSecSyst = HasFlag("RunXSecSyst");
  cout << "[ExampleRun::initializeAnalyzer] RunXSecSyst = " << RunXSecSyst << endl;

}

ExampleRun::~ExampleRun(){

  //==== Destructor of this Analyzer

}

void ExampleRun::executeEvent(){

  //================================================================
  //====  Example 1
  //====  Dimuon Z-peak events with two muon IDs, with systematics
  //================================================================

  //==== *IMPORTANT TO SAVE CPU TIME*
  //==== Every GetMuon() funtion first collect ALL MINIAOD muons with GetAllMuons(),
  //==== and then check ID booleans.
  //==== GetAllMuons not only loops over all MINIAOD muons, but also actually CONSTRUCT muon objects for each muons.
  //==== We are now running systematics, and you don't want to do this for every systematic sources
  //==== So, I defined "vector<Muon> AllMuons;" in Analyzers/include/ExampleRun.h,
  //==== and save muons objects at the very beginning of executeEvent().
  //==== Later, do "SelectMuons(AllMuons, ID, pt, eta)" to get muons with ID cuts
  AllMuons = GetAllMuons();
  //=== Jets too
  AllJets = GetAllJets();

  //==== Get L1Prefire reweight
  //==== If data, 1.;
  //==== If MC && DataYear > 2017, 1.;
  //==== If MC && DataYear <= 2017, we have to reweight the event with this value
  //==== I defined "double weight_Prefire;" in Analyzers/include/ExampleRun.h
  weight_Prefire = GetPrefireWeight(0);

  //==== Declare AnalyzerParameter

  AnalyzerParameter param;

  //==== Loop over muon IDs

  for(unsigned int it_MuonID=0; it_MuonID<MuonIDs.size(); it_MuonID++){

    TString MuonID = MuonIDs.at(it_MuonID);
    TString MuonIDSFKey = MuonIDSFKeys.at(it_MuonID);

    //==== 1) First, let's run Central values of the systematics

    //==== clear parameter set
    param.Clear();

    //==== set which systematic sources you want to run this time
    //==== default syst_ is AnalyzerParameter::Central
    param.syst_ = AnalyzerParameter::Central;

    //==== set name of the parameter set
    //==== this will be used for the directory name of histograms
    param.Name = MuonID+"_"+"Central";

    //==== You can define lepton ID string here
    param.Muon_Tight_ID = MuonID;
    param.Muon_ID_SF_Key = MuonIDSFKey;

    //==== And, Jet ID
    param.Jet_ID = "tight";

    //==== Now, all parameters are set. Run executeEventFromParameter() with this parameter set
    executeEventFromParameter(param);

    //==== 2) Now, loop over systematic sources
    //==== without --userflag RunSyst, this will not be ran

    if(RunSyst){

      for(int it_syst=1; it_syst<AnalyzerParameter::NSyst; it_syst++){

        //==== Everything else remains same, but only change syst_ and parameter name

        param.syst_ = AnalyzerParameter::Syst(it_syst);
        param.Name = MuonID+"_"+"Syst_"+param.GetSystType();
        executeEventFromParameter(param);
      }

    }

  }

  //================================
  //==== Example 2
  //==== Using new PDF
  //================================

  if(RunNewPDF && !IsDATA){
    //cout << "[ExampleRun::executeEvent] PDF reweight = " << GetPDFReweight() << endl;
    FillHist("NewPDF_PDFReweight", GetPDFReweight(), 1., 2000, 0.90, 1.10);
    //cout << "[ExampleRun::executeEvent] PDF reweight for error set (NErrorSet = "<<pdfReweight->NErrorSet<< ") :" << endl;
    for(int i=0; i<pdfReweight->NErrorSet; i++){
      //cout << "[ExampleRun::executeEvent]   " << GetPDFReweight(i) << endl;
      JSFillHist("NewPDF_PDFErrorSet", "PDFReweight_Member_"+TString::Itoa(i,10), GetPDFReweight(i), 1., 2000, 0.90, 1.10);
    }
  }

  //================================================
  //==== Example 3
  //==== How to estimate xsec errors (PDF & Scale)
  //==== For example, MET
  //================================================

  if(RunXSecSyst && !IsDATA){

    Event ev = GetEvent();
    double MET = ev.GetMETVector().Pt();

    //==== 1) PDF Error
    //==== Obtain RMS of the distribution later
    for(unsigned int i=0; i<PDFWeights_Error->size(); i++){
      JSFillHist("XSecError", "MET_PDFError_"+TString::Itoa(i,10), MET, PDFWeights_Error->at(i), 200, 0., 200.);
    }

    //==== 2) PDF AlphaS
    //==== Look for PDF4LHC paper..
    //==== https://arxiv.org/abs/1510.03865
    if(PDFWeights_AlphaS->size()==2){
      JSFillHist("XSecError", "MET_PDFAlphaS_Down", MET, PDFWeights_AlphaS->at(0), 200, 0., 200.);
      JSFillHist("XSecError", "MET_PDFAlphaS_Up", MET, PDFWeights_AlphaS->at(1), 200, 0., 200.);
    }

    //==== 3) Scale
    //==== Obtain the envelop of the distribution later
    for(unsigned int i=0; i<PDFWeights_Scale->size(); i++){
      //==== i=5 and 7 are unphysical
      if(i==5) continue;
      if(i==7) continue;
      JSFillHist("XSecError", "MET_Scale_"+TString::Itoa(i,10), MET, PDFWeights_Scale->at(i), 200, 0., 200.);
    }

  }

}

void ExampleRun::executeEventFromParameter(AnalyzerParameter param){

  //=============
  //==== No Cut
  //=============

  JSFillHist(param.Name, "NoCut_"+param.Name, 0., 1., 1, 0., 1.);

  //========================
  //==== MET Filter
  //========================

  if(!PassMETFilter()) return;
  
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  //==============
  //==== Trigger
  //==============
  bool unprescaled_trig= true;
  if(! (ev.PassTrigger(IsoMuTriggerName) ))unprescaled_trig =false;

  vector<TString> trigger_list_mu;
  vector<TString> trigger_list_dimu;
  trigger_list_mu.push_back("HLT_Mu20_v");
  trigger_list_mu.push_back("HLT_Mu27_v");
  trigger_list_mu.push_back("HLT_Mu50_v");
  trigger_list_mu.push_back("HLT_Mu8_v");
  trigger_list_mu.push_back("HLT_Mu17_v");
  trigger_list_mu.push_back("HLT_Mu55_v");
  trigger_list_mu.push_back("HLT_TkMu50_v");
  trigger_list_dimu.push_back("HLT_Mu17_Mu8_v");
  trigger_list_dimu.push_back("HLT_Mu17_Mu8_DZ_v");
  trigger_list_dimu.push_back("HLT_Mu20_Mu10_v");
  trigger_list_dimu.push_back("HLT_Mu20_Mu10_DZ_v");
  trigger_list_dimu.push_back("HLT_Mu17_TkMu8_DZ_v");
  trigger_list_dimu.push_back("HLT_Mu27_TkMu8_v");
  trigger_list_dimu.push_back("HLT_Mu30_TkMu11_v");



  bool single_mu=true;
  bool single_dimu=true;
  if(! (ev.PassTrigger(trigger_list_mu) )) single_mu=false;
  if(! (ev.PassTrigger(trigger_list_dimu) )) single_dimu=false;

  //======================
  //==== Copy AllObjects
  //======================

  vector<Muon> this_AllMuons = AllMuons;
  vector<Jet> this_AllJets = AllJets;

  //==== Then, for each systematic sources
  //==== 1) Smear or scale them
  //==== 2) Then apply ID selections
  //==== This order should be explicitly followed
  //==== Below are all variables for available systematic sources

  if(param.syst_ == AnalyzerParameter::Central){

  }
  else if(param.syst_ == AnalyzerParameter::JetResUp){
    this_AllJets = SmearJets( this_AllJets, +1 );
    //this_AllFatJets = SmearFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetResDown){
    this_AllJets = SmearJets( this_AllJets, -1 );
    //this_AllFatJets = SmearFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnUp){
    this_AllJets = ScaleJets( this_AllJets, +1 );
    //this_AllFatJets = ScaleFatJets( this_AllFatJets, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::JetEnDown){
    this_AllJets = ScaleJets( this_AllJets, -1 );
    //this_AllFatJets = ScaleFatJets( this_AllFatJets, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnUp){
    this_AllMuons = ScaleMuons( this_AllMuons, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::MuonEnDown){
    this_AllMuons = ScaleMuons( this_AllMuons, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronResUp){
    //this_AllElectrons = SmearElectrons( this_AllElectrons, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronResDown){
    //this_AllElectrons = SmearElectrons( this_AllElectrons, -1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronEnUp){
    //this_AllElectrons = ScaleElectrons( this_AllElectrons, +1 );
  }
  else if(param.syst_ == AnalyzerParameter::ElectronEnDown){
    //this_AllElectrons = ScaleElectrons( this_AllElectrons, -1 );
  }
  else{
    cout << "[ExampleRun::executeEventFromParameter] Wrong syst" << endl;
    exit(EXIT_FAILURE);
  }

  //==================================================
  //==== Then, apply ID selections using this_AllXXX
  //==================================================

  vector<Muon> muons = SelectMuons(this_AllMuons, param.Muon_Tight_ID, 20., 2.4);
  vector<Jet> jets = SelectJets(this_AllJets, param.Jet_ID, 30., 2.4);

  //=======================
  //==== Sort in pt-order
  //=======================

  //==== 1) leptons : after scaling/smearing, pt ordring can differ from MINIAOD
  std::sort(muons.begin(), muons.end(), PtComparing);
  //==== 2) jets : similar, but also when applying new JEC, ordering is changes. This is important if you use leading jets
  std::sort(jets.begin(), jets.end(), PtComparing);

  int NBJets_NoSF(0), NBJets_WithSF_2a(0);
  JetTagging::Parameters jtp_DeepCSV_Medium = JetTagging::Parameters(JetTagging::DeepCSV,
                                                                     JetTagging::Medium,
                                                                     JetTagging::incl, JetTagging::comb);

  //==== b tagging

  //==== method 1a)
  //==== multiply "btagWeight" to the event weight
  double btagWeight = mcCorr->GetBTaggingReweight_1a(jets, jtp_DeepCSV_Medium);

  //==== method 2a)
  for(unsigned int ij = 0 ; ij < jets.size(); ij++){

    double this_discr = jets.at(ij).GetTaggerResult(JetTagging::DeepCSV);
    //==== No SF
    if( this_discr > mcCorr->GetJetTaggingCutValue(JetTagging::DeepCSV, JetTagging::Medium) ) NBJets_NoSF++;
    //==== 2a
    if( mcCorr->IsBTagged_2a(jtp_DeepCSV_Medium, jets.at(ij)) )  NBJets_WithSF_2a++;
    
  }
 
   
  // vector<Gen> gens = GetGens();
  //
  //PrintGen(gens);     

  //=========================
  //==== Event selections..
  //=========================

  //==== dimuon
  if(muons.size() != 2) return;

  if(muons.at(0).Charge() != muons.at(1).Charge()) return;
  //==== leading muon has trigger-safe pt
  if( muons.at(0).Pt() <= 20. ) return;
  if( muons.at(1).Pt() <= 10. ) return;

  bool is_pp = (muons.at(0).Charge() >0) ? true : false;
  
  //==== ll
  Particle llCand = muons.at(0) + muons.at(1);


  //==== delta R mu Jet cuts
  Jet J;
  bool pass_drmuJ(false);
  bool is_bjet_mm(false);
  for(unsigned int ij = 0 ; ij < jets.size(); ij++){

    if(muons.at(0).DeltaR(jets[ij]) < 0.3) {
      if(muons.at(1).DeltaR(jets[ij]) < 0.3) {
	J = jets.at(ij);
	pass_drmuJ=true;
	if( mcCorr->IsBTagged_2a(jtp_DeepCSV_Medium, jets.at(ij)) )  is_bjet_mm=true;
	
      } // dr mu2 
    }// dR mu1
  }
    
  //===================
  //==== Event weight
  //===================

  double weight = 1.;
  //==== If MC
  if(!IsDATA){

    //==== weight_norm_1invpb is set to be event weight normalized to 1 pb-1
    //==== So, you have to multiply trigger luminosity
    //==== you can pass trigger names to ev.GetTriggerLumi(), but if you are using unprescaled trigger, simply pass "Full"

    weight *= weight_norm_1invpb*ev.GetTriggerLumi("Full");

    //==== MCweight is +1 or -1. Should be multiplied if you are using e.g., aMC@NLO NLO samples
    weight *= ev.MCweight();

    //==== L1Prefire reweight
    weight *= weight_Prefire;

    //==== Example of applying Muon scale factors
    for(unsigned int i=0; i<muons.size(); i++){
      
      double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  muons.at(i).Eta(), muons.at(i).MiniAODPt());

      //==== If you have iso SF, do below. Here we don't.
      //double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, muons.at(i).Eta(), muons.at(i).MiniAODPt());
      double this_isosf = 1.;

      weight *= this_idsf*this_isosf;
      
    }

  } //==== close of MC weights 


  if(unprescaled_trig)     JSFillHist(param.Name,"IsoMuNonPrescaled_ll_Cand_"+param.Name,  llCand.M(), weight, 200, 0., 200.);
  
  
  //=== Set up MET object
  
  //=== Condition for muon JJ 
  if(pass_drmuJ){
    
    TString s_bjet="J";
    if(is_bjet_mm) s_bjet = "BJ"; 
    TString s_charge = "minusminus";
    if(is_pp) s_charge = "plusplus";


    TString trig="";
    if(single_dimu) trig="Trig_DiMu";
    else  if(single_dimu) trig="Trig_SingleMu";

    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_ll_Mass_"+param.Name, llCand.M(), weight, 200, 0., 200.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_ll_Pt_"+param.Name, llCand.Pt(), weight, 50, 0., 500.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_ll_Eta_"+param.Name, llCand.Pt(), weight, 60, -3., 3.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_ll_sumPt_"+param.Name, muons[0].Pt()+muons[1].Pt(), weight, 50, 0., 500.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_njets" +param.Name, jets.size() , weight,  10, 0., 10.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_met" +param.Name, METv.Pt() , weight , 50, 0., 200.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_J_Pt" +param.Name, J.Pt() ,weight , 50, 0., 400.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_J_Eta" +param.Name, J.Eta(),weight  , 60, -3., 3.);


    //=== lepton plots
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_l1_reliso_"+param.Name,  muons[0].RelIso() , weight, 50, 0., 10.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_l2_reliso_"+param.Name,  muons[1].RelIso() , weight, 50, 0., 10.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_l1_ip3d_"+param.Name,  muons[0].IP3D() , weight, 50, 0., 10.);
    JSFillHist(param.Name, trig+"_"+s_bjet + "_ll_"+s_charge +"_Cand_l2_ip3d_"+param.Name,  muons[1].IP3D() , weight, 50, 0., 10.);

  }

}



