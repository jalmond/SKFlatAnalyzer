#include "HNL_LeptonFakeStudy.h"

void HNL_LeptonFakeStudy::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();


}

void HNL_LeptonFakeStudy::executeEvent(){

  Event ev = GetEvent();

  AnalyzerParameter param;

  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ELIDs;
  vector<HNL_LeptonCore::Channel> channel;
  vector<TString> paramnames;
  
  TString           era="2016";
  if(DataYear==2017)era="2017";
  if(DataYear==2018)era="2018";
  
  
  double weight =1;
  
  if(!PassMETFilter()) return;

  
  
  std::vector<Electron> loose_electrons     = GetElectrons( param,"HNLooseV4" , 10, 2.5,false) ;
  std::vector<Muon>     loose_muons         = GetMuons    ( param,"HNLooseV1", 5, 2.4, false);
  
  std::vector<Jet> jets_tmp     = GetJets   ( param, "tight", 30., 2.7);

  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_electrons);

  Particle METv = GetvMET("PuppiT1xyULCorr");

  std::vector<Jet> jets; 
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


  bool useevent40 = UseEvent(leps , jets, 40.,  METv, weight);

  /*
  bool UnkiStudy=true;
  if(UnkiStudy){

    ////// check 1 mu / 2 mu events 
    ////// 1 muon events use SL trigger prescaled
    ////// 2 muon use 1 prescaled SL trigger
    ////// select back to back jets (b/non b) 

    if(useevent40){
      
      float prescale_trigger = GetPrescale(leps);
      if(prescale_trigger == 0.) return;
      event_weight*=prescale_trigger;

      for(int ilep = 0 ; ilep < 2; ilep++)  {

	TString L_prefix = "Fake_Loose"+tag;
	TString T_prefix = "Fake_Tight"+tag;
	TString prefix = (ilep==0) ? L_prefix : T_prefix;
	
	float lep_pt = ->Pt();

	FillHist((prefix + "_pt").Data(),     lep_pt, weight_pt*prescale_lep, nbin_pt, ptbins, "p_{T} (GeV)");
	FillHist((prefix + "_eta").Data(),    lep_eta, weight_pt*prescale_lep , nbin_eta, etabins,"#eta");

      }
    }
    return;
  }


  */

  vector<TString> lables = GetGenList();


  if(SameCharge(loose_electrons)) {
    if(loose_electrons[0].IsFake() || loose_electrons[1].IsFake()  ) {
      if(loose_electrons[0].Pt() > 25 && loose_electrons[1].Pt() > 15){
	
	for (auto el : loose_electrons) {
	  if(!el.IsFake()) continue;
	  
	  TString lep_fake_tag=MatchGenDef(All_Gens, el);
	  
	  
	  if(el.IsBB()){

	    if(el.HNL_MVA_Fake("v4LF") > el.HNL_MVA_Fake("v4HFB") && el.HNL_MVA_Fake("v4LF") > el.HNL_MVA_Fake("v4HFC")) {
	      if(el.HNL_MVA_Fake("v4HFB") > el.HNL_MVA_Fake("v4HFC")) FillHist( "SSMVA_match/Matched_Type_"+el.CloseJet_Flavour() , 0 , weight, 10, 0., 10);
	      else FillHist( "SSMVA_match/Matched_Type_"+el.CloseJet_Flavour() , 1 , weight, 10, 0., 10);
	    }
	    else if(el.HNL_MVA_Fake("v4HFB") > el.HNL_MVA_Fake("v4LF") && el.HNL_MVA_Fake("v4HFB") > el.HNL_MVA_Fake("v4HFC")) {
	      if(el.HNL_MVA_Fake("v4LF") > el.HNL_MVA_Fake("v4HFC")) FillHist( "SSMVA_match/Matched_Type_"+el.CloseJet_Flavour() , 2 , weight, 10, 0., 10);
	      else FillHist( "SSMVA_match/Matched_Type_"+el.CloseJet_Flavour() , 3 , weight, 10, 0., 10);
	    }
	    else {
	      if(el.HNL_MVA_Fake("v4LF") > el.HNL_MVA_Fake("v4HFB")) FillHist( "SSMVA_match/Matched_Type_"+el.CloseJet_Flavour() , 4 , weight, 10, 0., 10);
	      else FillHist( "SSMVA_match/Matched_Type_"+el.CloseJet_Flavour() , 5 , weight, 10, 0., 10);

	    }



	    if(el.HNL_MVA_Fake("v4LF") > el.HNL_MVA_Fake("v4HF"))           FillHist( "SSMVA_match/Type_LF_HF_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	    else  FillHist( "SSMVA_match/Type_LF_HF_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	    
	    
	    if(el.HNL_MVA_Fake("v4LF") > el.HNL_MVA_Fake("v4HFB"))           FillHist( "SSMVA_match/Type_LF_HFB_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	    else FillHist( "SSMVA_match/Type_LF_HFB_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	    
	    if(el.HNL_MVA_Fake("v4LF") > el.HNL_MVA_Fake("v4HFC"))           FillHist( "SSMVA_match/Type_LF_HFC_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	    else FillHist( "SSMVA_match/Type_LF_HFC_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	    
	    FillHist( "SSMVA_Loose/Lepton_MVA"   , el.HNL_MVA_Fake("v4") , weight, 100, -1., 1, "BDT score");
	    //FillHist( "SSMVA_Loose/Lepton_MVA_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("v4") , weight, 100, -1., 1, "BDT score");                                                                         
	    FillHist( "SSMVA_Loose/Lepton_MVA_"+el.CloseJet_Flavour() , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	  }
	  else{
	    

	    if(el.HNL_MVA_Fake("EDv4LF") > el.HNL_MVA_Fake("EDv4HFB") && el.HNL_MVA_Fake("EDv4LF") > el.HNL_MVA_Fake("EDv4HFC")) {
	      if(el.HNL_MVA_Fake("EDv4HFB") > el.HNL_MVA_Fake("EDv4HFC")) FillHist( "SSMVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 0 , weight, 10, 0., 10);
	      else FillHist( "SSMVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 1 , weight, 10, 0., 10);
	    }
	    else if(el.HNL_MVA_Fake("EDv4HFB") > el.HNL_MVA_Fake("EDv4LF") && el.HNL_MVA_Fake("EDv4HFB") > el.HNL_MVA_Fake("EDv4HFC")) {
	      if(el.HNL_MVA_Fake("EDv4LF") > el.HNL_MVA_Fake("EDv4HFC")) FillHist( "SSMVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 2 , weight, 10, 0., 10);
	      else FillHist( "SSMVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 3 , weight, 10, 0., 10);
	    }
	    else {
	      if(el.HNL_MVA_Fake("EDv4LF") > el.HNL_MVA_Fake("EDv4HFB")) FillHist( "SSMVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 4 , weight, 10, 0., 10);
	      else FillHist( "SSMVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 5 , weight, 10, 0., 10);
	    }



	    if(el.HNL_MVA_Fake("EDv4LF") > el.HNL_MVA_Fake("EDv4HF"))           FillHist( "SSMVA_match/EDType_LF_HF_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	    else FillHist( "SSMVA_match/EDType_LF_HF_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	    
	    
	    if(el.HNL_MVA_Fake("EDv4LF") > el.HNL_MVA_Fake("EDv4HFB"))           FillHist( "SSMVA_match/EDType_LF_HFB_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	    else FillHist( "SSMVA_match/EDType_LF_HFB_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	    
	    if(el.HNL_MVA_Fake("EDv4LF") > el.HNL_MVA_Fake("EDv4HFC"))           FillHist( "SSMVA_match/EDType_LF_HFC_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	    else FillHist( "SSMVA_match/EDType_LF_HFC_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	    
	    FillHist( "SSMVA_Loose/Lepton_EDMVA"   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	    //FillHist( "SSMVA_Loose/Lepton_EDMVA_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	    FillHist( "SSMVA_Loose/Lepton_EDMVA_"+el.CloseJet_Flavour()   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	  }
	  
	  /*
	    
	    if (std::find(lables.begin(), lables.end(), lep_fake_tag) == lables.end()){
	    cout <<"lep_fake_tag = " << lep_fake_tag << " missing "<< endl;
	    PrintMatchedGen(All_Gens, el);
	    
	    }
	    
	    else{
	    
	    if(el.CloseJet_FlavourInt()==0) FillEventCutflowAll("SSElectron", "LF_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron", "HFC_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron", "HFB_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron", "HF_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron", "HF_Fake_type", weight, lables,lep_fake_tag);
	    
	    if(jets.size() == 0) {
	    if(el.CloseJet_FlavourInt()==0) FillEventCutflowAll("SSElectron0j", "LF_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron0j", "HFC_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron0j", "HFB_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron0j", "HF_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron0j", "HF_Fake_type", weight, lables,lep_fake_tag);
	    }
	    else if(jets.size() == 1) {
	    if(el.CloseJet_FlavourInt()==0) FillEventCutflowAll("SSElectron1j", "LF_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron1j", "HFC_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron1j", "HFB_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron1j", "HF_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron1j", "HF_Fake_type", weight, lables,lep_fake_tag);
	  }
	  else{
	  if(el.CloseJet_FlavourInt()==0) FillEventCutflowAll("SSElectron2j", "LF_Fake_type", weight, lables,lep_fake_tag);
	  if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron2j", "HFC_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron2j", "HFB_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("SSElectron2j", "HF_Fake_type", weight, lables,lep_fake_tag);
	    if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("SSElectron2j", "HF_Fake_type", weight, lables,lep_fake_tag);
	  }
	  
	  if(el.IsBB()){
	    FillHist( "SSMVA_Loose/Lepton_MVA"   , el.HNL_MVA_Fake("v4") , weight, 100, -1., 1, "BDT score");
	    //FillHist( "SSMVA_Loose/Lepton_MVA_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("v4") , weight, 100, -1., 1, "BDT score");
	    FillHist( "SSMVA_Loose/Lepton_MVA_"+el.CloseJet_Flavour() , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	  }
	  else{
	    FillHist( "SSMVA_Loose/Lepton_EDMVA"   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	    FillHist( "SSMVA_Loose/Lepton_EDMVA_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	    FillHist( "SSMVA_Loose/Lepton_EDMVA_"+el.CloseJet_Flavour()+"_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	  }
	  }*/
	}
      }
    }
  }
  
  if(useevent40){

    std::vector<Jet> jets_tmp     = GetJets   ( param, "tight", 20., 5.);

    JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
    std::vector<Jet> BJetColl    = SelectBJets(param, jets_tmp , param_jets);
    std::vector<Jet> CJetColl;
    for(auto i : jets_tmp) {
      if( i.GetTaggerResult(JetTagging::DeepJet_CvsL) <= mcCorr->GetJetTaggingCutValue(JetTagging::DeepCSV_CvsL,JetTagging::Medium ) ) continue;
      CJetColl.push_back(i);
    }

    for (auto el : loose_electrons) {
      

      float lep_pt = el.Pt();
      if(lep_pt > 100.) lep_pt = 99;

      int nbin_pt    = 9;
      double ptbins      [nbin_pt+1];
      vector<double> vptbins     = { 10., 15.,20.,25.,30., 35.,40.,50.,60.,100.};
      std::copy(vptbins.begin(), vptbins.end(), ptbins);

      if(!el.IsFake()) continue;

      if(el.IsBB()){
	
	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HFB") && el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HFC")) {
	  if(el.HNL_MVA_Fake("v4HFB") < el.HNL_MVA_Fake("v4HFC")) FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 0 , weight, 10, 0., 10);
	  else FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 1 , weight, 10, 0., 10);
	}
	else if(el.HNL_MVA_Fake("v4HFB") < el.HNL_MVA_Fake("v4LF") && el.HNL_MVA_Fake("v4HFB") < el.HNL_MVA_Fake("v4HFC")) {
	  if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HFC")) FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 2 , weight, 10, 0., 10);
          else FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 3 , weight, 10, 0., 10);
	}
	else {
	  if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HFB")) FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 4 , weight, 10, 0., 10);
          else FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 5 , weight, 10, 0., 10);

	}

	
	FillHist("Fakerates/LooseEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	if(el.HNL_MVA_Fake("v4") > 0.2) FillHist("Fakerates/TightEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	if(BJetColl.size() == 0) {
	  FillHist("Fakerates/0b_LooseEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("v4") > 0.2) FillHist("Fakerates/0b_TightEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}
	else{
	  FillHist("Fakerates/b_LooseEl_BB_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	    if(el.HNL_MVA_Fake("v4") > 0.2) FillHist("Fakerates/b_TightEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	    
	}
	
	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HF")){
	  FillHist("Fakerates/LFTagged_LooseEl_BB_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("v4") > 0.2) FillHist("Fakerates/LFTagged_TightEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}
	else if(el.HNL_MVA_Fake("v4HFB") < el.HNL_MVA_Fake("v4HFC")){
	  FillHist("Fakerates/HFBTagged_LooseEl_BB_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("v4") > 0.2) FillHist("Fakerates/HFBTagged_TightEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}
	else {
          FillHist("Fakerates/HFCTagged_LooseEl_BB_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
          if(el.HNL_MVA_Fake("v4") > 0.2) FillHist("Fakerates/HFCTagged_TightEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}
	
	FillHist("Fakerates/"+el.CloseJet_Flavour()+"Gen_LooseEl_BB_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	if(el.HNL_MVA_Fake("v4") > 0.2) FillHist("Fakerates/"+el.CloseJet_Flavour()+"Gen_TightEl_BB_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
		
	
	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HF")) {

	  for(auto i : jets_tmp) {
	    if(i.DeltaR(el) < 0.4){
	      FillHist( "MVA_matchLF/BB_CLScore", i.GetTaggerResult(JetTagging::DeepJet_CvsL) , weight, 100, -1., 1);
	      FillHist( "MVA_matchLF/BB_CBScore", i.GetTaggerResult(JetTagging::DeepJet_CvsB) , weight, 100, -1., 1);
	      FillHist( "MVA_matchLF/BB_BScore", i.GetTaggerResult(JetTagging::DeepJet) , weight, 100, -1., 1);
	    }
	  }
	  FillHist( "MVA_matchLF/BB_NBJet",BJetColl.size(), weight, 5, 0., 5); 
	  FillHist( "MVA_matchLF/BB_NCJet",CJetColl.size(), weight, 5, 0., 5); 
	  FillHist( "MVA_matchLF/BB_HFC_MVA", el.HNL_MVA_Fake("v4HFC"), weight, 100, -1., 1);
	  FillHist( "MVA_matchLF/BB_HFB_MVA", el.HNL_MVA_Fake("v4HFB"), weight, 100, -1., 1);
	  FillHist( "MVA_matchLF/BB_LF_MVA", el.HNL_MVA_Fake("v4LF"), weight, 100, -1., 1);
	  for(auto ibj : BJetColl)           FillHist( "MVA_matchLF/BB_BJetDR_",el.DeltaR(ibj), weight, 100, 0., 2);
	  for(auto icj : CJetColl)           FillHist( "MVA_matchLF/BB_CJetDR_",el.DeltaR(icj), weight, 100, 0., 2);
	}
	else{


	  for(auto i : jets_tmp) {
            if(i.DeltaR(el) < 0.4){
              FillHist( "MVA_matchHF/BB_CLScore", i.GetTaggerResult(JetTagging::DeepJet_CvsL) , weight, 100, -1., 1);
              FillHist( "MVA_matchHF/BB_CBScore", i.GetTaggerResult(JetTagging::DeepJet_CvsB) , weight, 100, -1., 1);
              FillHist( "MVA_matchHF/BB_BScore", i.GetTaggerResult(JetTagging::DeepJet) , weight, 100, -1., 1);
            }
          }

	  FillHist( "MVA_matchHF/BB_NBJet",BJetColl.size(), weight, 5, 0., 5);
          FillHist( "MVA_matchHF/BB_NCJet",CJetColl.size(), weight, 5, 0., 5);
          FillHist( "MVA_matchHF/BB_HFC_MVA", el.HNL_MVA_Fake("v4HFC"), weight, 100, -1., 1);
          FillHist( "MVA_matchHF/BB_HFB_MVA", el.HNL_MVA_Fake("v4HFB"), weight, 100, -1., 1);
          FillHist( "MVA_matchHF/BB_HF_MVA", el.HNL_MVA_Fake("v4HF"), weight, 100, -1., 1);
          for(auto ibj : BJetColl)           FillHist( "MVA_matchHF/BB_BJetDR_",el.DeltaR(ibj), weight, 100, 0., 2);
          for(auto icj : CJetColl)           FillHist( "MVA_matchHF/BB_CJetDR_",el.DeltaR(icj), weight, 100, 0., 2);
	}

	for(auto i : jets_tmp) {
	  if(i.DeltaR(el) < 0.4){
	    FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_CLScore", i.GetTaggerResult(JetTagging::DeepJet_CvsL) , weight, 100, -1., 1);
	    FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_CBScore", i.GetTaggerResult(JetTagging::DeepJet_CvsB) , weight, 100, -1., 1);
	    FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_BScore", i.GetTaggerResult(JetTagging::DeepJet) , weight, 100, -1., 1);
	  }
	}


	FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_NBJet",BJetColl.size(), weight, 5, 0., 5);
	FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_NCJet",CJetColl.size(), weight, 5, 0., 5);
	FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_HFC_MVA", el.HNL_MVA_Fake("v4HFC"), weight, 100, -1., 1);
	FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_HFB_MVA", el.HNL_MVA_Fake("v4HFB"), weight, 100, -1., 1);
	FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_LF_MVA", el.HNL_MVA_Fake("v4LF"), weight, 100, -1., 1);
	for(auto ibj : BJetColl)           FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_BJetDR_",el.DeltaR(ibj), weight, 100, 0., 2);
	for(auto icj : CJetColl)           FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_BB_CJetDR_",el.DeltaR(icj), weight, 100, 0., 2);

	

	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HF")) {
          FillHist( "MVA_match/Type_LF_HF_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	  if(el.HNL_MVA_Fake("v4LF") > -0.7)           FillHist( "MVA_match/Type_LF_HF_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	}
	else  {
	  FillHist( "MVA_match/Type_LF_HF_"+el.CloseJet_Flavour() , 3 , weight, 5, 0., 5);
	  if(el.HNL_MVA_Fake("v4HF")  > -0.7)           FillHist( "MVA_match/Type_LF_HF_"+el.CloseJet_Flavour() , 4 , weight, 5, 0., 5);
	  
	}
	
	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HFB"))  {
	  FillHist( "MVA_match/Type_LF_HFB_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	  if(el.HNL_MVA_Fake("v4LF") > -0.7)           FillHist( "MVA_match/Type_LF_HFB_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	}
	else {
	  FillHist( "MVA_match/Type_LF_HFB_"+el.CloseJet_Flavour() , 3 , weight, 5, 0., 5);
	  if(el.HNL_MVA_Fake("v4HFB")  > -0.7)             FillHist( "MVA_match/Type_LF_HFB_"+el.CloseJet_Flavour() , 4 , weight, 5, 0., 5);
	}
	
	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HFC")) {
          FillHist( "MVA_match/Type_LF_HFC_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);
	  if(el.HNL_MVA_Fake("v4LF") > -0.7)            FillHist( "MVA_match/Type_LF_HFC_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	}
	else {
	  FillHist( "MVA_match/Type_LF_HFC_"+el.CloseJet_Flavour() , 3 , weight, 5, 0., 5);
	  if(el.HNL_MVA_Fake("v4HFC")  > -0.7)           FillHist( "MVA_match/Type_LF_HFC_"+el.CloseJet_Flavour() , 4 , weight, 5, 0., 5);
	}
      }
      else{

	FillHist("Fakerates/LooseEl_EC_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	if(el.HNL_MVA_Fake("EDv4") > 0.2) FillHist("Fakerates/TightEl_EC_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	if(BJetColl.size() == 0) {
	  FillHist("Fakerates/0b_LooseEl_EC_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("EDv4") > 0.2) FillHist("Fakerates/0b_TightEl_EC_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}
	else{
	  FillHist("Fakerates/b_LooseEl_EC_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("EDv4") > 0.2) FillHist("Fakerates/b_TightEl_EC_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  
	}

	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HF")) {

	  for(auto i : jets_tmp) {
            if(i.DeltaR(el) < 0.4){
              FillHist( "MVA_matchLF/EC_CLScore", i.GetTaggerResult(JetTagging::DeepJet_CvsL) , weight, 100, -1., 1);
              FillHist( "MVA_matchLF/EC_CBScore", i.GetTaggerResult(JetTagging::DeepJet_CvsB) , weight, 100, -1., 1);
              FillHist( "MVA_matchLF/EC_BScore", i.GetTaggerResult(JetTagging::DeepJet) , weight, 100, -1., 1);
            }
          }

          FillHist( "MVA_matchLF/EC_NBJet",BJetColl.size(), weight, 5, 0., 5);
          FillHist( "MVA_matchLF/EC_NCJet",CJetColl.size(), weight, 5, 0., 5);
          FillHist( "MVA_matchLF/EC_HFC_MVA", el.HNL_MVA_Fake("EDv4HFC"), weight, 100, -1., 1);
          FillHist( "MVA_matchLF/EC_HFB_MVA", el.HNL_MVA_Fake("EDv4HFB"), weight, 100, -1., 1);
          FillHist( "MVA_matchLF/EC_LF_MVA", el.HNL_MVA_Fake("EDv4LF"), weight, 100, -1., 1);
          for(auto ibj : BJetColl)           FillHist( "MVA_matchLF/EC_BJetDR_",el.DeltaR(ibj), weight, 100, 0., 2);
          for(auto icj : CJetColl)           FillHist( "MVA_matchLF/EC_CJetDR_",el.DeltaR(icj), weight, 100, 0., 2);
        }

	else{

          for(auto i : jets_tmp) {
            if(i.DeltaR(el) < 0.4){
              FillHist( "MVA_matchHF/EC_CLScore", i.GetTaggerResult(JetTagging::DeepJet_CvsL) , weight, 100, -1., 1);
              FillHist( "MVA_matchHF/EC_CBScore", i.GetTaggerResult(JetTagging::DeepJet_CvsB) , weight, 100, -1., 1);
              FillHist( "MVA_matchHF/EC_BScore", i.GetTaggerResult(JetTagging::DeepJet) , weight, 100, -1., 1);
            }
          }

          FillHist( "MVA_matchHF/EC_NBJet",BJetColl.size(), weight, 5, 0., 5);
          FillHist( "MVA_matchHF/EC_NCJet",CJetColl.size(), weight, 5, 0., 5);
          FillHist( "MVA_matchHF/EC_HFC_MVA", el.HNL_MVA_Fake("EDv4HFC"), weight, 100, -1., 1);
          FillHist( "MVA_matchHF/EC_HFB_MVA", el.HNL_MVA_Fake("EDv4HFB"), weight, 100, -1., 1);
          FillHist( "MVA_matchHF/EC_HF_MVA", el.HNL_MVA_Fake("EDv4HF"), weight, 100, -1., 1);
          for(auto ibj : BJetColl)           FillHist( "MVA_matchHF/EC_BJetDR_",el.DeltaR(ibj), weight, 100, 0., 2);
          for(auto icj : CJetColl)           FillHist( "MVA_matchHF/EC_CJetDR_",el.DeltaR(icj), weight, 100, 0., 2);
	}

        for(auto i : jets_tmp) {
          if(i.DeltaR(el) < 0.4){
            FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_CLScore", i.GetTaggerResult(JetTagging::DeepJet_CvsL) , weight, 100, -1., 1);
            FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_CBScore", i.GetTaggerResult(JetTagging::DeepJet_CvsB) , weight, 100, -1., 1);
            FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_BScore", i.GetTaggerResult(JetTagging::DeepJet) , weight, 100, -1., 1);
          }
        }


	FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_NBJet",BJetColl.size(), weight, 5, 0., 5);
	FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_NCJet",CJetColl.size(), weight, 5, 0., 5);
        FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_HFC_MVA", el.HNL_MVA_Fake("EDv4HFC"), weight, 100, -1., 1);
        FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_HFB_MVA", el.HNL_MVA_Fake("EDv4HFB"), weight, 100, -1., 1);
        FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_LF_MVA", el.HNL_MVA_Fake("EDv4LF"), weight, 100, -1., 1);
        for(auto ibj : BJetColl)           FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_BJetDR_",el.DeltaR(ibj), weight, 100, 0., 2);
        for(auto icj : CJetColl)           FillHist( "MVA_matchGen/"+el.CloseJet_Flavour()+"_EC_CJetDR_",el.DeltaR(icj), weight, 100, 0., 2);



	
	if(el.HNL_MVA_Fake("v4LF") < el.HNL_MVA_Fake("v4HFB") ){
	  FillHist("Fakerates/LFTagged_LooseEl_EC_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("EDv4") > 0.2) FillHist("Fakerates/LFTagged_TightEl_EC_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}
	else  if(el.HNL_MVA_Fake("v4HFB") < el.HNL_MVA_Fake("v4HFC") ){
	  FillHist("Fakerates/HFBTagged_LooseEl_EC_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("EDv4") > 0.2) FillHist("Fakerates/HFBTagged_TightEl_EC_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}
	else  if(el.HNL_MVA_Fake("v4HFC") < el.HNL_MVA_Fake("v4HFB") && el.HNL_MVA_Fake("v4HFC") < el.HNL_MVA_Fake("v4LF")){
	  FillHist("Fakerates/HFCTagged_LooseEl_EC_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	  if(el.HNL_MVA_Fake("EDv4") > 0.2) FillHist("Fakerates/HFCTagged_TightEl_EC_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	}	
	
	FillHist("Fakerates/"+el.CloseJet_Flavour()+"Gen_LooseEl_EC_pt",    lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	if(el.HNL_MVA_Fake("EDv4") > 0.2) FillHist("Fakerates/"+el.CloseJet_Flavour()+"Gen_TightEl_EC_pt",     lep_pt, 1, nbin_pt, ptbins, "p_{T} (GeV)");
	
	
	if(el.HNL_MVA_Fake("EDv4LF") < el.HNL_MVA_Fake("EDv4HFB") && el.HNL_MVA_Fake("EDv4LF") < el.HNL_MVA_Fake("EDv4HFC")) {
          if(el.HNL_MVA_Fake("EDv4HFB") > el.HNL_MVA_Fake("EDv4HFC")) FillHist( "MVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 0 , weight, 10, 0., 10);
          else FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 1 , weight, 10, 0., 10);
	}
	else if(el.HNL_MVA_Fake("EDv4HFB") < el.HNL_MVA_Fake("EDv4LF") && el.HNL_MVA_Fake("EDv4HFB") < el.HNL_MVA_Fake("EDv4HFC")) {
          if(el.HNL_MVA_Fake("EDv4LF") < el.HNL_MVA_Fake("EDv4HFC")) FillHist( "MVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 2 , weight, 10, 0., 10);
          else FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 3 , weight, 10, 0., 10);
	}
	else {
	  if(el.HNL_MVA_Fake("EDv4LF") < el.HNL_MVA_Fake("EDv4HFB")) FillHist( "MVA_match/EDMatched_Type_"+el.CloseJet_Flavour() , 4 , weight, 10, 0., 10);
          else FillHist( "MVA_match/Matched_Type_"+el.CloseJet_Flavour() , 5 , weight, 10, 0., 10);
	  
	}
	if(el.HNL_MVA_Fake("EDv4LF") < el.HNL_MVA_Fake("EDv4HF"))           FillHist( "MVA_match/EDType_LF_HF_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	else FillHist( "MVA_match/EDType_LF_HF_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);


	if(el.HNL_MVA_Fake("EDv4LF") < el.HNL_MVA_Fake("EDv4HFB"))           FillHist( "MVA_match/EDType_LF_HFB_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	else FillHist( "MVA_match/EDType_LF_HFB_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);

	if(el.HNL_MVA_Fake("EDv4LF") < el.HNL_MVA_Fake("EDv4HFC"))           FillHist( "MVA_match/EDType_LF_HFC_"+el.CloseJet_Flavour() , 2 , weight, 5, 0., 5);
	else FillHist( "MVA_match/EDType_LF_HFC_"+el.CloseJet_Flavour() , 1 , weight, 5, 0., 5);

      }



      TString lep_fake_tag=MatchGenDef(All_Gens, el);

      if (std::find(lables.begin(), lables.end(), lep_fake_tag) == lables.end()){
	PrintMatchedGen(All_Gens, el);
	
      }
      
      else{
	
	if(el.CloseJet_FlavourInt()==0) FillEventCutflowAll("QCDReg_Electron", "LF_Fake_type", weight, lables,lep_fake_tag);
	if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("QCDReg_Electron", "HFC_Fake_type", weight, lables,lep_fake_tag);
	if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("QCDReg_Electron", "HFB_Fake_type", weight, lables,lep_fake_tag);
	if(el.CloseJet_FlavourInt()==4) FillEventCutflowAll("QCDReg_Electron", "HF_Fake_type", weight, lables,lep_fake_tag);
	if(el.CloseJet_FlavourInt()==5) FillEventCutflowAll("QCDReg_Electron", "HF_Fake_type", weight, lables,lep_fake_tag);
	
	if(el.IsBB()){
	  FillHist( "MVA_Loose/Lepton_MVA"   , el.HNL_MVA_Fake("v4") , weight, 100, -1., 1, "BDT score");
	  FillHist( "MVA_Loose/Lepton_MVA_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("v4") , weight, 100, -1., 1, "BDT score");
	  FillHist( "MVA_Loose/Lepton_MVA_"+el.CloseJet_Flavour()+"_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	}
	else{
	  FillHist( "MVA_Loose/Lepton_EDMVA"   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	  FillHist( "MVA_Loose/Lepton_EDMVA_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	  FillHist( "MVA_Loose/Lepton_EDMVA_"+el.CloseJet_Flavour()+"_"+MatchGenDef(All_Gens,el)   , el.HNL_MVA_Fake("EDv4") , weight, 100, -1., 1, "BDT score");
	}
      }

    }
  }
  
}




HNL_LeptonFakeStudy::HNL_LeptonFakeStudy(){
  
}
 
HNL_LeptonFakeStudy::~HNL_LeptonFakeStudy(){


}






bool HNL_LeptonFakeStudy::UseEvent(std::vector<Lepton *> leps ,  std::vector< Jet> jets, float awayjetcut, Particle MET, float wt){


  bool useevent = false;
  if(leps.size() != 1) return false;

  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int w = 0; w < leps.size();  w++){
    METdphi = TVector2::Phi_mpi_pi(leps.at(w)->Phi()-MET.Phi());
    MT = sqrt(2.* leps.at(w)->Et()*MET.Pt() * (1 - cos( METdphi)));
    if(( (MET.Pt() < 40) && (MT < 25.)) ) {

      for (unsigned int ielT=0; ielT < leps.size(); ielT++){
        for(unsigned int ij=0; ij < jets.size(); ij++){
          if(jets.at(ij).Pt() < awayjetcut) continue;
          float dphi =fabs(TVector2::Phi_mpi_pi(leps.at(ielT)->Phi()- jets.at(ij).Phi()));
          if( (jets.at(ij).ChargedEmEnergyFraction()) > 0.65)  continue;

          if(dphi > 2.5){
            if((jets.at(ij).Pt() /  leps.at(ielT)->Pt() ) < 1.2) continue;
            useevent = true;
          }
        }
      }
    }
  }

  return useevent;

}
