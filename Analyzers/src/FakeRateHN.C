#include "FakeRateHN.h"

void FakeRateHN::initializeAnalyzer(){

  //================
  //==== Userflags
  //================
  
  isEE = HasFlag("isEE");
  isMM = HasFlag("isMM");
  isSingleMu = HasFlag("isSingleMu");


  RunSyst = HasFlag("RunSyst");
  if(IsDATA) RunSyst = false;

  HEM1516 = HasFlag("HEM1516");


  rand_ = new TRandom3(1234);


  cout << "[FakeRateHN::initializeAnalyzer()] RunSyst = " << RunSyst << endl;


  MuonTriggers.clear();
  MuonTriggersH.clear();
  ElectronTriggers.clear();
  SingleElectronTriggers.clear();
  EMuTriggers.clear();
  EMuTriggersH.clear();
  

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

    cout << "[FakeRateHN::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[FakeRateHN::initializeAnalyzer()] PUhname = " << PUhname << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void FakeRateHN::executeEvent(){
  
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

  
  AnalyzerParameter param;


  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ELIDs;
  vector<TString> channel;
  vector<TString>  loose_id;
  if(isEE||!IsDATA){
    channel.push_back("EE");
    //channel.push_back("EE");
    ELIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
    //ELIDs.push_back(make_pair("passTightID", "HNVeto2016"));
    MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    //MuIDs.push_back(make_pair("HNVeto2016","HNVeto2016"));
    //loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");
  }
  if(isSingleMu || isMM||!IsDATA){
    channel.push_back("MuMu");
    //channel.push_back("MuMu");
    //ELIDs.push_back(make_pair("HNVeto2016", "HNVeto2016"));
    ELIDs.push_back(make_pair("HNVeto2016", "HNVeto2016"));
    MuIDs.push_back(make_pair("HNTight2016", "HNVeto2016"));
    //MuIDs.push_back(make_pair("POGTightPFIsoVeryTight","HNVeto2016"));
    //loose_id.push_back("HNLoose2016");
    loose_id.push_back("HNLoose2016");

  }
  
  if(IsData){
    if(isSingleMu){
      if(DataYear!=2016){
	Event ev = GetEvent();
	if(!ev.PassTrigger("HLT_Mu3_PFJet40_v")) return;
      } 
      else return;
    }
  }

  for(unsigned int it_id=0; it_id<ELIDs.size(); it_id++){
    
    //************************************************///
    // setup leptton veto/loose/tight 
    //************************************************///

    TString MuonTightID      = MuIDs[it_id].first;
    TString MuonVetoID      = MuIDs[it_id].second;
    TString MuonLooseID  =  loose_id[it_id];
    TString ElectronTightID = ELIDs[it_id].first;
    TString ElectronLooseID = loose_id[it_id];
    TString ElectronVetoID  = ELIDs[it_id].second;

    TString FakeRateID =  ElectronLooseID;
    if(channel[it_id] == "MuMu") FakeRateID = MuonLooseID;
    param.Clear();


    TString Tight_ID = ElectronTightID;
    if(channel[it_id] == "MuMu") Tight_ID = MuonTightID;

  
    param.syst_ = AnalyzerParameter::Central;
    param.Name = "FakeRateHN_"+ElectronTightID;
    param.MCCorrrectionIgnoreNoHist = false;

    
    //==== Muon ID
    param.Muon_Tight_ID        = MuonTightID;
    param.Muon_Loose_ID        = MuonLooseID;
    param.Muon_Veto_ID        = MuonVetoID;
    param.Muon_ID_SF_Key      = "";
    param.Muon_ISO_SF_Key     = "";
    param.Muon_Trigger_SF_Key = "";
    param.Muon_MinPt = 10.;
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

    //************************************************///
    // change param name to link to IP/CC cuts
    //************************************************///
    param.Name = "FakeRateHN_"+channel[it_id] + "_" +Tight_ID;
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  } 
}

void FakeRateHN::executeEventFromParameter(AnalyzerParameter param, TString El_ID, TString channel){
  

  bool ee = true;
  if (channel == "MuMu") ee = false;

  //************************************************///
  // plot CR species cuts for All plots to be made
  //************************************************///

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
  
  //  if(Signal) weight_norm_1invpb=1.;
  
  //************************************************///
  // MC weights initial settings
  // -- set to full trigger luminosity for data taking year
  // -- apply MC gen weights
  // -- apply prefire weight
  //************************************************///
  if(!IsDATA){
    weight *= weight_norm_1invpb*ev.MCweight();
  }

  //************************************************///  
  //==== No Cut cutflow entry, get nevents in sample
  //************************************************///   

  FillEventCutflow(0,0,weight, "NoCut",param.Name);
  FillEventCutflow(0,1,weight, "NoCut",param.Name);

  //************************************************///   
  //==== Event selecitions : MET Filters
  //************************************************///   

  if(!PassMETFilter()) return;
  FillEventCutflow(0,0,weight, "METFilter",param.Name);
  FillEventCutflow(0,1,weight, "METFilter",param.Name);


  //======================
  //==== Copy AllObjects
  //======================

  vector<Electron> this_AllElectrons = AllElectrons;
  vector<Muon>     this_AllMuons = AllMuons;
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
  if(!IsDATA){

    double this_pureweight = 1.;
    this_pureweight = weight_PU;

    weight *= this_pureweight;
  }
  
  //************************************************///   
  // 
  // setup analysis level lepton vectors
  //
  //************************************************///   
  
  std::vector<Electron> tight_electrons = SelectElectrons(this_AllElectrons, param.Electron_Tight_ID, 9.5, 2.5);
  std::vector<Muon>     tight_muons         = SelectMuons    (this_AllMuons,    param.Muon_Tight_ID, 5., 2.4);


  std::vector<Electron> loose_electrons = SelectElectrons(this_AllElectrons, param.Electron_Loose_ID, 9.5, 2.5);
  std::vector<Muon>     loose_muons         = SelectMuons    (this_AllMuons,    param.Muon_Loose_ID, 5., 2.4);

 
  std::sort(tight_electrons.begin(), tight_electrons.end(), PtComparing);
  std::sort(tight_muons.begin(), tight_muons.end(), PtComparing);
  std::sort(loose_muons.begin(), loose_muons.end(), PtComparing);
  std::sort(loose_electrons.begin(),loose_electrons.end(), PtComparing);
  

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

  std::vector<Jet>      jets_tmp   = SelectJets(this_AllJets, param.Jet_ID, 20., 2.5);

  std::vector<Jet>      jets;
  for(unsigned int ijet =0; ijet < jets_tmp.size(); ijet++){
    bool jetok=true;
    
    for(unsigned int iel=0 ; iel < loose_electrons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_electrons[iel]) < 0.4) jetok = false;
    }
    for(unsigned int iel=0 ; iel < loose_muons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_muons[iel]) < 0.4) jetok = false;
    }
    if(jetok) jets.push_back(jets_tmp[ijet]);
  }
  std::sort(jets.begin(), jets.end(), PtComparing);


  //************************************************///
  // jet up b jet variable
  // --- use medium Deep CVS
  //************************************************///
  int NBJets=GetNBJets();

  
  //************************************************///
  //@@@@ Apply lepton scale factors
  //************************************************///

  
  //************************************************///   
  // select trigger
  //************************************************///   

  param.Name = param.Electron_Tight_ID;
  if(ee) RunE(tight_electrons, loose_electrons,tight_muons,loose_muons, jets, fatjets, ev,  param, weight);
  param.Name = param.Muon_Tight_ID;
  if(!ee) RunM(tight_electrons, loose_electrons,tight_muons,loose_muons,  jets, fatjets,ev, param, weight);
  
}



bool FakeRateHN::PassHNID(TString TightID, Electron el){

  
  bool pass=true;
  if(!el.PassID(TightID) ) pass=false;
  
  if (TightID == "passTightID" || TightID == "passMediumID" ||   TightID == "passMVAID_iso_WP90" ||   TightID == "passMVAID_iso_WP80" ){
    if (! (el.IsGsfCtfScPixChargeConsistent() &&   el.Pass_TriggerEmulation()))  pass=false;
	
    if( fabs(el.scEta()) <= 1.479 ){
      if( (fabs(el.dXY())<0.05 && fabs(el.dZ())<0.1) )  pass=false;
    }
    else{
      if( !(fabs(el.dXY())<0.1 && fabs(el.dZ())<0.2) )  pass=false;
    }
  }// ID

  if (TightID == "passTightID_nocc" ){
    if (! el.Pass_TriggerEmulation())  pass=false;
    
    if( fabs(el.scEta()) <= 1.479 ){
      if( (fabs(el.dXY())<0.05 && fabs(el.dZ())<0.1) )  pass=false;
    }
    else{
      if( !(fabs(el.dXY())<0.1 && fabs(el.dZ())<0.2) )  pass=false;
    }
  }// ID     
  return pass;



}

void FakeRateHN::RunM(std::vector<Electron> tight_el, std::vector<Electron> loose_el, std::vector<Muon> tight_mu, std::vector<Muon> loose_mu, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){
  
  TString label = param.Name;
  TString channel_s="_MuMu";
  Particle METv = ev.GetMETVector();

  if(loose_mu.size() != 1) return;
    
  if(isSingleMu) {
    if (IsData && DataYear!=2016){
      if(!ev.PassTrigger("HLT_Mu3_PFJet40_v")) return;
    }
  }
  
  // remove if muon                                                                                                                          
  if(loose_el.size() > 0) return;

  GetFakeRateAndPromptRates(loose_mu,label, tight_mu,w,0.07);



  return;

} /// end function                      





int FakeRateHN::GetNBJets(){



  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jets   = SelectJets(this_AllJets,"HNTight", 20., 2.5);

  int NBJets=0;

  for(unsigned int i=0; i<jets.size(); i++){

    if( mcCorr->IsBTagged_2a(JetTagging::Parameters(JetTagging::DeepCSV,
                                                    JetTagging::Medium,                                                    JetTagging::incl, JetTagging::comb), jets.at(i)) ) NBJets++;
  }

  return NBJets;
}

void FakeRateHN::RunE(std::vector<Electron> tight_el, std::vector<Electron> loose_el, std::vector<Muon> tight_mu, std::vector<Muon> loose_mu, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Event ev, AnalyzerParameter param,  float w){


  TString channel_s="_EE";
  int i_channel=0;
  TString label = param.Name;
  Particle METv = ev.GetMETVector();
  
  if(loose_el.size() != 1) return;
  
  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                                     
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_33="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"; /// 

  // remove if muon 
  if(loose_mu.size() > 0) return;


  GetFakeRateAndPromptRates(loose_el,label, tight_el,0.08,0.08,w,"ELECTRON_HN_TIGHT");



} /// end function RunE



void FakeRateHN::GetFakeRateAndPromptRates(std::vector<Muon> muonLooseColl, TString mutag, std::vector<Muon> muonTightColl, float w, float isocut){


  TString triggerslist_3="HLT_Mu3_PFJet40_v";
  TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
  TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";
  if(muonLooseColl.size()<1) return;
  
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  
  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jetColl   = SelectJets(this_AllJets, "HNTight", 20., 2.5);
  std::vector<Jet>    jetCollTight ;
  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    bool jetok=true;

    for(unsigned int iel=0 ; iel < muonLooseColl.size(); iel++){
      if(jetColl[ijet].DeltaR(muonLooseColl[iel]) < 0.4) jetok = false;
    }
    if(jetok) jetCollTight.push_back(jetColl[ijet]);
  }
  std::sort(jetCollTight.begin(), jetCollTight.end(), PtComparing);


  float prescale_trigger =  1.;//GetPrescale(muonLooseColl,   ev.PassTrigger(triggerslist_3),ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_17));

  if(muonLooseColl.size()==1)MakeFakeRatePlots("", mutag, muonTightColl ,muonLooseColl,  jetCollTight, jetColl,  prescale_trigger, w, isocut, METv);
  return;
}

void FakeRateHN::GetFakeRateAndPromptRates(std::vector<Electron> electronLooseColl, TString eltag, std::vector<Electron> electronTightColl, float isocutb, float isocute,  float w, TString tightid ){
  
  
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_33="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"; /// --> no 33 trigger
  
  TString triggerslist="HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v";
  
  float id_weight=1.;
  float reco_weight=1.;

  vector<Jet> this_AllJets = AllJets;

  std::vector<Jet>      jetColl   = SelectJets(this_AllJets, "HNTight", 20., 2.5);
  std::vector<Jet>    jetCollTight ;
  for(unsigned int ijet =0; ijet < jetColl.size(); ijet++){
    bool jetok=true;

    for(unsigned int iel=0 ; iel < electronLooseColl.size(); iel++){
      if(jetColl[ijet].DeltaR(electronLooseColl[iel]) < 0.4) jetok = false;
    }
    if(jetok) jetCollTight.push_back(jetColl[ijet]);
  }
  std::sort(jetCollTight.begin(), jetCollTight.end(), PtComparing);
  
  if(electronLooseColl.size()<1) return;
  

  float prescale_trigger =  1.;//GetPrescale(electronLooseColl,  ev.PassTrigger(triggerslist_8), ev.PassTrigger(triggerslist_12), ev.PassTrigger(triggerslist_18), ev.PassTrigger( triggerslist_23), ev.PassTrigger(triggerslist_33)); 
  
  

  MakeFakeRatePlots(tightid, eltag, electronTightColl,electronLooseColl,  jetCollTight, jetColl,  prescale_trigger, isocutb, isocute,w, METv);
  
  
  return;
  
}

float FakeRateHN::GetPrescale( std::vector<Muon> muons,bool pass3, bool pass2, bool pass1 ){

  Event ev = GetEvent();

  float prescale_trigger= 1.;
  if(muons.size() != 1) return 0.;
  if(muons.size() ==1){

    /// 10 - 20  HLT_Mu7
    /// 20 - INF  HLT_Mu17
    
    if(muons.at(0).Pt() >= 20.){
      if(pass1){
	if(IsData) return 1.;
	prescale_trigger = ev.PassTrigger("HLT_Mu17_TrkIsoVVL_v"); //// 20 + GeV bins
      }
      else {
	if(IsData) return 0;
	return 0;
      }
    }
    else  if(muons.at(0).Pt() >= 10.){

      if(pass2){
	if(IsData) return 1.;
        prescale_trigger = ev.PassTrigger("HLT_Mu8_TrkIsoVVL_v") ; //// 20 + GeV bins
      }
      else {
	if(IsData) return 0;
	return 0;
      }
    }
    else  if(muons.at(0).Pt() >= 5.){
      if(pass3){
        if(IsData) return 1.;
        prescale_trigger =  ev.PassTrigger("HLT_Mu3_PFJet40_v") ; //// 20 + GeV bins                                                                         
      }
      else {
        if(IsData) return 0;
	return 0;
      }
    }

  }
  if(prescale_trigger == 0.) return 0.;
  if(IsData) return 1.;
  if(muons.at(0).Pt() < 5.) return 0.;
  
  
  return prescale_trigger;
}


double FakeRateHN::GetPrescale( std::vector<Electron> electrons,bool pass5,  bool pass4, bool pass3, bool pass2, bool pass1 ){
  
  Event ev = GetEvent();

  float prescale_trigger= 1.;
  if(electrons.size() != 1) return 0.;
  if(electrons.size() ==1){

    /// 10 - 15  HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_*
    /// 15 - 20  HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_*
    /// 20 - 25  HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30_*
    /// 25 - 35  HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_* 
    /// 35 - inf HLT_Ele33_CaloIdL_TrackIdL_IsoVL_PFJet30_*

    
    if(electrons.at(0).Pt() >= 25.){
      //HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30

      if(pass2){
	if(IsDATA) return 1.;
        prescale_trigger =  ev.GetTriggerLumi("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v") ; //// 20 + GeV bins
      }
      else {
	if(IsDATA) return 0;
	return 0;
	//prescale_trigger =  ev.GetTriggerLumi("HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v") * 0.8; 
      }
    }
    else   if(electrons.at(0).Pt() >= 20.){
      //HLT_Ele18_CaloIdL_TrackIdL_IsoVL_PFJet30
      if(pass3){
	if(IsDATA) return 1.;
	prescale_trigger = ev.GetTriggerLumi("HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v");
      }
      else {
	if(IsDATA) return 0;
	return 0;
	//prescale_trigger = ev.GetTriggerLumi("HLT_Ele17_CaloIdL_TrackIdL_IsoVL_v")*0.8 ;
      }
    }
    else   if(electrons.at(0).Pt() >= 15.){
      //HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_
      if(pass4){
	if(IsDATA) return 1.;
        prescale_trigger = ev.GetTriggerLumi("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v") ;
      }
      else {
	if(IsDATA) return 0;
	return 0;
        //prescale_trigger = ev.GetTriggerLumi("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v")*0.8 ;
      }
    }
    else   if(electrons.at(0).Pt() >= 9.5){
      //HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_                                                                                                                               
      if(pass5){
        if(IsDATA) return 1.;
        prescale_trigger = ev.GetTriggerLumi("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v") ;
      }
      else {
        if(IsDATA) return 0;
	return 0;
        //prescale_trigger = ev.GetTriggerLumi("HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v")*0.8 ;
      }
    }
    else{
      prescale_trigger = 0.;
    }
  }
  if(prescale_trigger == 0.) return 0.;
  if(IsData) return 1.;
  if(electrons.at(0).Pt() < 9.5) return 0.;
  

  
  return prescale_trigger;
}


void FakeRateHN::MakeFakeRatePlots(TString label, TString mutag,   std::vector<Muon> muons_tight, std::vector<Muon> muons,  std::vector<Jet> jets, std::vector<Jet> alljets, float prescale_w, float w, float isocut, Particle MET){


  if(muons.size() != 1 ) return;

  if(prescale_w==0.) return;

  if(!IsData)gens = GetGens();

  bool useevent20 = UseEvent(muons , jets, 20., prescale_w, MET,  w);
  bool useevent30 = UseEvent(muons , jets, 30., prescale_w, MET,w);
  bool useevent40 = UseEvent(muons , jets, 40., prescale_w, MET,w);
  bool useevent60 = UseEvent(muons , jets, 60., prescale_w, MET,w);

  bool truth_match= false;
  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT MUONS                                                                                                    
  if(!IsData) {
    if(muons.size() > 0){
      /// ONLY INCLUDE MUONS FROM W/Z                                                                                                                     
      if(GetLeptonType(muons.at(0), gens) ==1) truth_match=true;
    }
    else truth_match = true;
  }
  else truth_match=true;

  label= mutag;
  if(truth_match){
    if(jets.size() >= 1){
      if(useevent20)GetFakeRates(muons, muons_tight,label, jets, alljets,  label+"_20",(prescale_w * w),isocut);
      if(useevent30)GetFakeRates(muons, muons_tight,label, jets, alljets,  label+"_30",(prescale_w * w),isocut);
      if(useevent40)GetFakeRates(muons, muons_tight,label, jets, alljets,  label+"_40",(prescale_w * w),isocut);
      if(useevent60)GetFakeRates(muons, muons_tight, label,jets, alljets,  label+"_60",(prescale_w * w),isocut);
    }
  }
}


void FakeRateHN::MakeFakeRatePlots(TString label, TString eltag,   std::vector<Electron> electrons_tight, std::vector<Electron> electrons,  std::vector<Jet> jets, std::vector<Jet> alljets, float prescale_w, float isocutb, float isocute ,float w, Particle MET){
  
  if(electrons.size() != 1 ) return;
  
  if(prescale_w==0.) return;
  
  if(!IsData)gens = GetGens();

  bool useevent20 = UseEvent(electrons , jets, 20., prescale_w, MET,  w); 
  bool useevent30 = UseEvent(electrons , jets, 30., prescale_w, MET,w); 
  bool useevent40 = UseEvent(electrons , jets, 40., prescale_w, MET,w); 
  bool useevent60 = UseEvent(electrons , jets, 60., prescale_w, MET,w); 

  bool truth_match= false;
  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT ELECTRONS
  if(!IsData) {
    if(electrons.size() > 0){
      /// ONLY INCLUDE ELECTRONS FROM W/Z
      if(GetLeptonType(electrons.at(0), gens) ==1) truth_match=true;
    }
    else truth_match = true;
  }
  else truth_match=true;
  
  label= eltag;
  


  if(truth_match){
    if(jets.size() >= 1){

      if(useevent20)GetFakeRates(electrons, electrons_tight,label, jets, alljets,  label+"_20", isocutb, isocute,(prescale_w * w));
      if(useevent30)GetFakeRates(electrons, electrons_tight,label, jets, alljets,  label+"_30", isocutb, isocute,(prescale_w * w));
      if(useevent40)GetFakeRates(electrons, electrons_tight,label, jets, alljets,  label+"_40", isocutb, isocute,(prescale_w * w));
      if(useevent60)GetFakeRates(electrons, electrons_tight, label,jets, alljets,  label+"_60", isocutb, isocute,(prescale_w * w));
    }
  }
}

bool FakeRateHN::UseEvent(std::vector<Muon> muons,  std::vector< Jet> jets, float awayjetcut, float precale_weight, Particle MET, float wt){
  bool useevent = false;
  if(muons.size() != 1) return false;
  if(precale_weight != 0.){
    /// Z and W veto                                                                                                                                          
    Double_t MT=0;
    Double_t METdphi=0;
    for(unsigned int w = 0; w < muons.size();  w++){
      METdphi = TVector2::Phi_mpi_pi(muons.at(w).Phi()-MET.Phi());
      MT = sqrt(2.* muons.at(w).Et()*MET.Pt() * (1 - cos( METdphi)));

      if(( (MET.Pt() < 20) && (MT < 25.)) ) {

        for (unsigned int ielT=0; ielT < muons.size(); ielT++){
          for(unsigned int ij=0; ij < jets.size(); ij++){
            if(jets.at(ij).Pt() < awayjetcut) continue;
            float dphi =fabs(TVector2::Phi_mpi_pi(muons.at(ielT).Phi()- jets.at(ij).Phi()));
	    if(dphi > 2.5){
	      if((jets.at(ij).Pt() /  muons.at(ielT).Pt() ) < 1.) continue;
	      useevent = true;
	    }
          }
        }
      }
    }
  }
  return useevent;

}
bool FakeRateHN::UseEvent(std::vector<Electron> electrons,  std::vector< Jet> jets, float awayjetcut, float precale_weight, Particle MET, float wt){
  
  bool useevent = false;
  if(electrons.size() != 1) return false;
  if(precale_weight != 0.){
    /// Z and W veto
    Double_t MT=0;
    Double_t METdphi=0;
    for(unsigned int w = 0; w < electrons.size();  w++){
      METdphi = TVector2::Phi_mpi_pi(electrons.at(w).Phi()-MET.Phi());
      MT = sqrt(2.* electrons.at(w).Et()*MET.Pt() * (1 - cos( METdphi)));

      if(( (MET.Pt() < 20) && (MT < 25.)) ) {
	
        for (unsigned int ielT=0; ielT < electrons.size(); ielT++){
          for(unsigned int ij=0; ij < jets.size(); ij++){
            if(jets.at(ij).Pt() < awayjetcut) continue;
            float dphi =fabs(TVector2::Phi_mpi_pi(electrons.at(ielT).Phi()- jets.at(ij).Phi()));
            if( (jets.at(ij).ChargedEmEnergyFraction()) > 0.65)  continue;
	    if(dphi > 2.5) useevent = true;
          }
        }
      }
    }
  }
  return useevent;
}


void FakeRateHN::GetFakeRates(std::vector<Muon> loose_mu, std::vector<Muon> tight_mu, TString tightlabel,  std::vector<Jet> jets,  std::vector<Jet> lljets, TString tag,double w, float isocut){

  double ptbinscone[8] = { 10., 15.,20.,30.,40.,50.,  60., 200.};
  double ptbins[9] = { 5., 10., 15.,20.,30.,45.,60.,100., 200.};
  double etabin[2] = { 0.,  2.5};
  double etabins[4] = { 0., 0.8,1.479,  2.5};
  double etabins2[5] = { 0.,0.8,  1.479, 2.,  2.5};

  Event ev = GetEvent();
  if(loose_mu.size() == 1 && jets.size() >= 1){
    float mu_pt = loose_mu.at(0).Pt();
    //float mu_pt_corr = loose_mu.at(0).Pt()*(1+max(0.,(loose_mu.at(0).RelIso()-isocut))) ; /// will need changing for systematics 
    float mu_pt_corr =  loose_mu[0].CalcPtCone(loose_mu[0].RelIso(), 0.07);

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";


    float weight_ptcorr=w;
    float weight_pt=w;
    bool fill_plot=false;
    if (mu_pt_corr > 5  && mu_pt_corr < 15){
      if(mu_pt > 5.) {
	if(ev.PassTrigger(triggerslist_3)) {
	  fill_plot=true;
	  if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_3);
	}
	else {
	  fill_plot=false;
	  weight_ptcorr = 0.;
	}
      
	if(IsData){
	  if(DataYear!=2016){
	    if(!isSingleMu) {
	      fill_plot=false;
	    weight_ptcorr = 0.;
	    }
	  }
	}
      }
    }


    else if ( mu_pt_corr < 30){
      if(mu_pt > 10) {
	if(ev.PassTrigger(triggerslist_8)) {
	  fill_plot=true;
          if(!IsData)weight_ptcorr= w * ev.GetTriggerLumi(triggerslist_8);
	}
	else{
	  fill_plot=false;
          weight_ptcorr= 0.;
	}
     
	if(IsData){
	  if(DataYear!=2016){
	    if(isSingleMu) {
	      fill_plot=false;
	      weight_ptcorr = 0.;
	    }
	  }
	}

      }

    }

    else {
      if(mu_pt > 20) {
	if(ev.PassTrigger(triggerslist_17)) {
	  fill_plot=true;
	  if(!IsData) weight_ptcorr= w * ev.GetTriggerLumi(triggerslist_17);
	}
	else{
	  fill_plot=false;
          weight_ptcorr= 0.;
	}
	if(IsData){
          if(DataYear!=2016){
            if(isSingleMu) {
              fill_plot=false;
              weight_ptcorr = 0.;
            }
          }
        }
      }
    }
    
    
    FillHist(("LooseMu" + tag + "_pt_eta").Data(), mu_pt, fabs(loose_mu.at(0).Eta()),  w, 8, ptbins, 4 , etabins2);
    FillHist(("LooseMu" + tag + "_pt").Data(), mu_pt,   w, 8, ptbins);
    if(fill_plot){
      FillHist(("LooseMu" + tag + "_ptcorr_eta").Data(), mu_pt_corr, fabs(loose_mu.at(0).Eta()),  weight_ptcorr, 7, ptbinscone, 4 , etabins2);
      FillHist(("LooseMu" + tag + "_ptcorr").Data(), mu_pt_corr,  weight_ptcorr, 7,ptbinscone,"p_{T} cone (GeV");
    }
    
    if( tight_mu.size() == 1){
      FillHist(("TightMu" + tag + "_pt_eta").Data(), mu_pt, fabs(tight_mu.at(0).Eta()),  w, 8, ptbins, 4 , etabins2);
      FillHist(("TightMu" + tag + "_pt").Data(), mu_pt,  w, 8, ptbins);
      if(fill_plot){
	FillHist(("TightMu" + tag + "_ptcorr_eta").Data(), mu_pt_corr, fabs(tight_mu.at(0).Eta()), weight_ptcorr, 7, ptbins, 4 , etabins2);
	FillHist(("TightMu" + tag + "_ptcorr").Data(), mu_pt_corr,  weight_ptcorr, 7, ptbins,"p_{T} cone (GeV");
      }
    }
  }
  return;
}

void FakeRateHN::GetFakeRates(std::vector<Electron> loose_el, std::vector<Electron> tight_el, TString tightlabel,  std::vector<Jet> jets,  std::vector<Jet> alljets, TString tag,float isocutb, float isocute, double w){
  
  double ptbins[8] = { 10., 15.,23.,30.,35., 40.,60.,200.};
  double ptbinsb[8] = { 10., 15.,20.,30.,45.,60.,100., 200.};
  double etabin[2] = { 0.,  2.5};
  double etabins[4] = { 0., 0.8,1.479,  2.5};
  double etabins2[5] = { 0.,0.8,  1.479, 2.,  2.5};

  /// for most cuts just plot pt_eta
  Event ev = GetEvent();  

  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                    
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";



  if(loose_el.size() != 1) return;
  
  
  float el_pt = loose_el.at(0).Pt();
  float isocut = isocutb;
  if(fabs(loose_el.at(0).Eta()) > 1.479)isocut = isocute;

  float el_pt_corr =  loose_el[0].CalcPtCone(loose_el[0].RelIso(), 0.08);
  if(el_pt_corr > 200.) el_pt_corr = 200.;

  
  float weight_ptcorr=w;
  float weight_pt=w;
  bool fill_plot=false;
  
  if (el_pt_corr > 10  && el_pt_corr < 23){
    if(el_pt > 9.5) {
      if(ev.PassTrigger(triggerslist_8)) {
	fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_8);
      }
      else{
	fill_plot=false;
        weight_ptcorr = 0.;
      }
    }
  }
  else if ( el_pt_corr < 35){
    if(el_pt > 15) {
      if(ev.PassTrigger(triggerslist_12)) {
	fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_12);
      }
      else{
	fill_plot=false;
        weight_ptcorr = 0.;
      }
    }
  }
    
  else if (el_pt_corr < 40){
    if(el_pt > 20) {
      if(ev.PassTrigger(triggerslist_18)) {
	fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_18);
      }

      else{
	fill_plot=false;
        weight_ptcorr = 0.;

      }
    }
  }
  else{
    if(el_pt > 25) {
      if(ev.PassTrigger(triggerslist_23)) {
        fill_plot=true;
	if(!IsData)weight_ptcorr = w * ev.GetTriggerLumi(triggerslist_23);
      } 

      else{
        fill_plot=false;
        weight_ptcorr = 0.;

      }
    }// pt 
  }// end ptbin check


  
  if(loose_el.size() == 1 && jets.size() >= 1){
    
    if(el_pt > 10.)FillHist(("LooseEl" + tag + "_pt_eta").Data(), el_pt, fabs(loose_el.at(0).Eta()),  w, 7, ptbinsb, 4 , etabins2);
    if(fill_plot)FillHist(("LooseEl" + tag + "_ptcorr_eta").Data(), el_pt_corr, fabs(loose_el.at(0).Eta()),  weight_ptcorr, 7, ptbins, 4 , etabins2);
    if(fill_plot)FillHist(("LooseEl" + tag + "_ptcorr").Data(), el_pt_corr, weight_ptcorr,7, ptbins,"p_{T} cone (GeV)");
    
    
    if( tight_el.size() == 1){
      if(el_pt > 10.)FillHist(("TightEl" + tag + "_pt_eta").Data(), el_pt, fabs(tight_el.at(0).Eta()),  w, 7, ptbinsb, 4 , etabins2);
      if(fill_plot)FillHist(("TightEl" + tag + "_ptcorr_eta").Data(), el_pt_corr, fabs(tight_el.at(0).Eta()),  weight_ptcorr, 7, ptbins, 4 , etabins2);
      if(fill_plot)FillHist(("TightEl" + tag + "_ptcorr").Data(), el_pt_corr,  weight_ptcorr, 7, ptbins, "p_{T} cone (GeV)");
    }
  }
  
  return;
  
}



FakeRateHN::FakeRateHN(){
  


}
 
 FakeRateHN::~FakeRateHN(){
   delete rand_;

}




double FakeRateHN::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){
  
  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)_st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_st += fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++) _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}


double FakeRateHN::MT(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);
  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );
  
}


std::vector<Jet> FakeRateHN::JetsVetoLeptonInside(std::vector<Jet> jets, std::vector<Electron *> els, std::vector<Muon *> mus, double dR){

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

bool FakeRateHN::LeptonPassID(Lepton &lepton, TString ID){

  if(lepton.LeptonFlavour()==Lepton::ELECTRON){
    Electron *el = (Electron *)&lepton;
    return el->PassID(ID);
  }
  else if(lepton.LeptonFlavour()==Lepton::MUON){
    Muon *mu = (Muon *)&lepton;
    return mu->PassID(ID);
  }
  else{
    cerr << "[FakeRateHN::LeptonPassID] lepton flavour wrong.." << endl;
    exit(EXIT_FAILURE);
    return false;
  }

}


void FakeRateHN::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){


  return;

}

void FakeRateHN::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void FakeRateHN::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

double FakeRateHN::LSFSF(int lepflav, int dir){

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
    cerr << "[FakeRateHN::LSFSF] wrong lepflav : " <<  lepflav << endl;
    exit(EXIT_FAILURE);
    return 1.;
  }

}
