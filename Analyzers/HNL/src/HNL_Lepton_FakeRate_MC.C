#include "HNL_Lepton_FakeRate_MC.h"

void HNL_Lepton_FakeRate_MC::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

  TString AnalyzerPath=getenv("SKFlat_WD"), SKFlatV = getenv("SKFlatV");

}

void HNL_Lepton_FakeRate_MC::executeEvent(){

  Event ev = GetEvent();

  //************************************************///
  // setup list of IDs and jobs

  vector<AnalyzerParameter> VParameters;
  
  if(HasFlag("MCClosure")){
    vector<TString> LIDs = {"HNL_ULID_FO", "HNL_ULID_FOv2_"+GetYearString(),  "HNL_ULID_FOv7_"+GetYearString()};
    vector<TString> NIDs = {"HNL_LooseID_","HNL_LooseID_v2_" , "HNL_LooseID_v7_" };
    for(unsigned int l=0 ; l < LIDs.size(); l++)   VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu, HNL_LeptonCore::NormToXsec, {"MCClosure"},NIDs[l]+"_MC","HNL_ULID_"+GetYearString(),LIDs[l]));

    goto RunJobs;
  }


  if(HasFlag("Standard")){
    //// RUN MISCJOBS
    vector<TString> LIDs = {"HNL_ULID_FO", "HNL_ULID_FOv3_"+GetYearString()};
    vector<TString> NIDs = {"HNL_LooseID_","HNL_LooseID_v3_" };
    for(unsigned int l=0 ; l < LIDs.size(); l++)   VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu, HNL_LeptonCore::NormToXsec, {"MCProfile"},NIDs[l]+"_MC","HNL_ULID_"+GetYearString(),LIDs[l]));


    goto RunJobs;
  }

  if(HasFlag("CheckProfile")){
    //// Check MVA Profile  for PtParton

    vector<TString> LIDs = {"HNL_ULID_FO_v1_"+GetYearString(),
			    "HNL_ULID_FO_v2_"+GetYearString(),
			    "HNL_ULID_FO_v3_"+GetYearString(),
			    "HNL_ULID_FO_v4_"+GetYearString(),
			    "HNL_ULID_FO_v5_"+GetYearString()};
			    
    vector<TString> NIDs = {"HNL_LooseID_FO_v1_"+GetYearString(),
			    "HNL_LooseID_FO_v2_"+GetYearString(),
			    "HNL_LooseID_FO_v3_"+GetYearString(),
			    "HNL_LooseID_FO_v4_"+GetYearString(),
			    "HNL_LooseID_FO_v5_"+GetYearString()};


    for(unsigned int l=0 ; l < LIDs.size(); l++)   VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu, HNL_LeptonCore::NormToXsec, {"MCProfile"},NIDs[l]+"_MC","HNL_ULID_"+GetYearString(),LIDs[l]));  
    
    goto RunJobs;
    
  }
  if(HasFlag("CheckProfileEE")){
    /// Check MVA/Pt Profile for ptparton in Electrons

    vector<TString> LIDs = {"HNL_ULID_FO_v1_"+GetYearString(),
                            "HNL_ULID_FO_v2_"+GetYearString(),
                            "HNL_ULID_FO_v3_"+GetYearString(),
                            "HNL_ULID_FO_v4_"+GetYearString(),
                            "HNL_ULID_FO_v5_"+GetYearString()};

    vector<TString> NIDs = {"HNL_LooseID_FO_v1_"+GetYearString(),
                            "HNL_LooseID_FO_v2_"+GetYearString(),
                            "HNL_LooseID_FO_v3_"+GetYearString(),
                            "HNL_LooseID_FO_v4_"+GetYearString(),
                            "HNL_LooseID_FO_v5_"+GetYearString()};

    for(unsigned int l=0 ; l < LIDs.size(); l++)  VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE, HNL_LeptonCore::NormToXsec, {"MCProfile"},NIDs[l]+"_MC","HNL_ULID_"+GetYearString(),LIDs[l]));
    
    goto RunJobs;
  }

  if(HasFlag("MCFakes")) {
    //// Measure Fake Rates in MC
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NoNorm,        {"MCFakes"},"HNL_ID"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO"));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,  {"MCFakes"},"HNL_ID_Weighted"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO"));
 
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NoNorm,        {"MCFakes"},"HNL_IDv2"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FOv2_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,  {"MCFakes"},"HNL_IDv2_Weighted"         ,"HNL_ULID_"+GetYearString(),"HNL_ULID_FOv2_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NoNorm,        {"MCFakes"},"HNL_IDv7"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FOv7_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,  {"MCFakes"},"HNL_IDv7_Weighted"         ,"HNL_ULID_"+GetYearString(),"HNL_ULID_FOv7_"+GetYearString()));

    goto RunJobs;
  }

  
 RunJobs:
  
  for(auto param : VParameters)  executeEventFromParameter(param);
  
}

void HNL_Lepton_FakeRate_MC::executeEventFromParameter(AnalyzerParameter param){
   
  //************************************************///
  // setup event level objects
  Event ev = GetEvent();

  double weight = SetupWeight(ev, param);
  if(IsData) weight = 1;

  if(!PassMETFilter()) return;

  //  std::vector<Muon>     Test_Initial_loose_muons         = SelectMuons    ( param,"NoCut",     5,  2.4);

  
  std::vector<Electron> Initial_loose_electrons     = SelectElectrons( param,param.Electron_Loose_ID, 10, 2.5) ;
  std::vector<Muon>     Initial_loose_muons         = SelectMuons    ( param,param.Muon_Loose_ID,     5,  2.4);

  std::vector<Electron> Loose_Electrons;
  std::vector<Muon>     Loose_Muons;
  if(param.HasFlag("MCFakes") || param.HasFlag("MCProfile") ){
    for(auto ilep : Initial_loose_electrons) {
      if(ilep.IsFake()) Loose_Electrons.push_back(ilep);
    }
    for(auto ilep : Initial_loose_muons) {
      if(ilep.IsFake()) Loose_Muons.push_back(ilep);
    }
  }
  else{
    for(auto ilep : Initial_loose_electrons) Loose_Electrons.push_back(ilep);
    for(auto ilep : Initial_loose_muons)     Loose_Muons.push_back(ilep);
  }

  std::vector<Jet> jets_tmp     = SelectJets   ( param, "tight", 25., 2.7);
  std::vector<Jet> jets; 
  for(unsigned int ijet =0; ijet < jets_tmp.size(); ijet++){
    bool jetok=true;
    for(unsigned int iel=0 ; iel < Initial_loose_electrons.size(); iel++)   { if(jets_tmp[ijet].DeltaR(Initial_loose_electrons[iel]) < 0.4) jetok = false;}
    for(unsigned int iel=0 ; iel < Initial_loose_muons.size(); iel++)       { if(jets_tmp[ijet].DeltaR(Initial_loose_muons[iel]) < 0.4)     jetok = false;}
    if(jetok) jets.push_back(jets_tmp[ijet]);
  }
  if(param.Channel=="EE")   RunE(Loose_Electrons,Loose_Muons, jets,  param, weight);
  if(param.Channel=="MuMu") RunM(Loose_Electrons,Loose_Muons,  jets, param, weight);
  
}


void HNL_Lepton_FakeRate_MC::RunM(std::vector<Electron> loose_el,  std::vector<Muon> loose_mu, std::vector<Jet> jets,   AnalyzerParameter param,  float event_weight){

  Event ev = GetEvent();

  

  if(loose_mu.size() == 0) return;

  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_mu,loose_el, param);
  
  std::vector<bool>    blepsT;
  for(auto ilep : loose_mu)   blepsT.push_back(ilep.PassID(param.Muon_Tight_ID));
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  if(param.HasFlag("MCClosure")) {

    if(leps.size()==2 && leps[0]->Pt() > 25 && leps[1]->Pt() > 5){
      
      std::vector<Jet>    AK4_BJetColl      = GetHNLJets("BJet", param);
      std::vector<Jet>    AK4_JetColl       = GetHNLJets(param.AK4JetColl,     param);

      bool Lep1Prompt=false;
      bool Lep2Prompt=false;
      
      for(unsigned int i=2; i<All_Gens.size(); i++){   
	Gen gen = All_Gens.at(i);
	
	if(gen.Status() != 1) continue;
	int mindex = All_Gens.at(i).MotherIndex();
	int MotherPID = fabs(All_Gens.at(mindex).PID());
	bool PromptLepMu = (MotherPID == 13)  || (MotherPID == 15);
	bool PromptLepEl = (MotherPID == 11) || (MotherPID == 15);
	
	while (MotherPID > 10 && MotherPID < 16){
	  mindex = All_Gens.at(mindex).MotherIndex();
	  MotherPID= fabs(All_Gens.at(mindex).PID());
	}
	
	bool PromptLepMuFull = PromptLepMu &&  ((MotherPID == 21)  || (MotherPID < 6));
	bool PromptLepElFull = PromptLepEl &&  ((MotherPID == 21)  || (MotherPID < 6));
	
	if(fabs(gen.PID()) == 13){
	  if(PromptLepMuFull && leps[0]->LeptonFlavour() == Lepton::MUON && leps[0]->DeltaR(gen) < 0.1) Lep1Prompt=true;	    
	  if(PromptLepMuFull &&leps[1]->LeptonFlavour() == Lepton::MUON && leps[1]->DeltaR(gen) < 0.1) Lep2Prompt=true;	    
	}
	if(fabs(gen.PID()) == 11){
          if(PromptLepElFull &&leps[0]->LeptonFlavour() != Lepton::MUON && leps[0]->DeltaR(gen) < 0.1) Lep1Prompt=true;
          if(PromptLepElFull &&leps[1]->LeptonFlavour() != Lepton::MUON && leps[1]->DeltaR(gen) < 0.1) Lep2Prompt=true;
        }
      }
      
      if(MCSample.Contains("Sherpa")) {
	
	if(Lep1Prompt&&Lep2Prompt){
	  cout << leps[0]->LeptonFlavour() << " " << leps[1]->LeptonFlavour() << endl;
	  cout << "leps[0]->IsPrompt() = " << leps[0]->IsPrompt()  << " leps[1]->IsPrompt() = " << leps[0]->IsPrompt()  << " Lep1Prompt = " <<  Lep1Prompt  <<  " Lep2Prompt= " << Lep2Prompt << endl;
	  cout << "leps[0] " << leps[0]->Pt() << " " << leps[0]->Eta() << "  Leps1 "  << leps[1]->Pt() << " " <<leps[1]->Eta() << endl;
	  PrintGen(All_Gens);
	}
      }

      if(!MCSample.Contains("Sherpa")) Lep1Prompt = (leps[0]->IsPrompt() && leps[0]->PassLepID() && leps[1]->LeptonFlavour() == Lepton::MUON &&  leps[1]->IsFake());
      else Lep1Prompt = (Lep1Prompt && leps[1]->LeptonFlavour() == Lepton::MUON);
      if(Lep1Prompt){
	
	double PTPartonSF    = GetPtPartonSF(*leps[1],param.Muon_Loose_ID);
	double MVACut        = leps[1]->MVAFakeCut(param.Muon_Tight_ID,GetYearString());
	double PtCorr        = (leps[1]->CalcMVACone(MVACut)  < 80 )       ? leps[1]->CalcMVACone( MVACut)  : 79;
	double lep_ptparton  = (leps[1]->PtParton(PTPartonSF,MVACut) < 80) ? leps[1]->PtParton(PTPartonSF,MVACut) : 79;
	TString FR25Key = "MC_"+param.Muon_Loose_ID+"_J25_FR_cent";	FR25Key = FR25Key.ReplaceAll("_"+GetYearString(),"");	FR25Key = FR25Key.ReplaceAll("_ULID","_ID");
	TString FR30Key = "MC_"+param.Muon_Loose_ID+"_J30_FR_cent";	FR30Key = FR30Key.ReplaceAll("_"+GetYearString(),"");	FR30Key = FR30Key.ReplaceAll("_ULID","_ID");
	TString FR40Key = "MC_"+param.Muon_Loose_ID+"_J40_FR_cent";	FR40Key = FR40Key.ReplaceAll("_"+GetYearString(),"");	FR40Key = FR40Key.ReplaceAll("_ULID","_ID");
	TString FR60Key = "MC_"+param.Muon_Loose_ID+"_J60_FR_cent";	FR60Key = FR60Key.ReplaceAll("_"+GetYearString(),"");	FR60Key = FR60Key.ReplaceAll("_ULID","_ID");
	
	TString BTaggerLabel ="";
	if(leps[1]->CloseJet_BScore() > 0.9) BTaggerLabel="BJet1";
	else  if(leps[1]->CloseJet_BScore() > 0.1) BTaggerLabel = "BJet2";
	else  BTaggerLabel = "BJet3";

	TString FR25BKey = FR25Key; FR25BKey=FR25BKey.ReplaceAll("FR_cent",BTaggerLabel+"_FR_cent");
	TString FR30BKey = FR30Key; FR30BKey=FR30BKey.ReplaceAll("FR_cent",BTaggerLabel+"_FR_cent");
	TString FR40BKey = FR40Key; FR40BKey=FR40BKey.ReplaceAll("FR_cent",BTaggerLabel+"_FR_cent");
	TString FR60BKey = FR60Key; FR60BKey=FR60BKey.ReplaceAll("FR_cent",BTaggerLabel+"_FR_cent");

	double FRWeight25a =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR25Key, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
	double FRWeight25b =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR25Key , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
	double FRWeight25c =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR25Key , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());

        double FRWeight25ba =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR25BKey, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
        double FRWeight25bb =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR25BKey , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
        double FRWeight25bc =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR25BKey , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());
	
        double FRWeight30a =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR30Key, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
        double FRWeight30b =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR30Key , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
	double FRWeight30c =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR30Key , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());
	
	double FRWeight30ba =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR30BKey, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
        double FRWeight30bb =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR30BKey , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
        double FRWeight30bc =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR30BKey , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());


        double FRWeight40a =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR40Key, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
        double FRWeight40b =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR40Key , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
	double FRWeight40c =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR40Key , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());

	double FRWeight40ba =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR40BKey, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
        double FRWeight40bb =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR40BKey , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
        double FRWeight40bc =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR40BKey , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());


        double FRWeight60a =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR60Key, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
        double FRWeight60b =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR60Key , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
	double FRWeight60c =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR60Key , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());

        double FRWeight60ba =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR60BKey, "Standard", "Pt", leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger());
        double FRWeight60bb =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR60BKey , "Standard", "PtParton", leps[1]->fEta(), lep_ptparton,leps[1]->LeptonFakeTagger());
        double FRWeight60bc =   fakeEst->GetFakeRate  (true, param.Muon_Tight_ID,  FR60BKey , "Standard", "PtCorr", leps[1]->fEta(), PtCorr,leps[1]->LeptonFakeTagger());

	
	FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W25a1Loose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight25a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W25aLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight25a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W25bLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight25b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W25bLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight25b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W25cLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight25c*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W25cLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight25c*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W30aLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight30a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W30aLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight30a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W30bLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight30b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W30bLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight30b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W30cLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight30c*event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W30cLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight30c*event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W40aLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight40a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W40aLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight40a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W40bLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight40b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W40bLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight40b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W40cLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight40c*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W40cLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight40c*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W60aLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight60a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W60aLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight60a*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W60bLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight60b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W60bLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight60b*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W60cLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight60c*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W60cLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight60c*event_weight,200, -1, 1 ) ;


        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W25a1BLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight25ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W25aBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight25ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W25bBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight25bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W25bBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight25bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W25cBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight25bc*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W25cBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight25bc*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W30aBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight30ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W30aBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight30ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W30bBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight30bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W30bBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight30bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W30cBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight30bc*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W30cBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight30bc*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W40aBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight40ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W40aBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight40ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W40bBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight40bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W40bBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight40bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W40cBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight40bc*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W40cBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight40bc*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W60aBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight60ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W60aBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight60ba*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W60bBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight60bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W60bBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight60bb*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_W60cBLoose", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), FRWeight60bc*event_weight,200, -1, 1 ) ;
        FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_W60cBLoose", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"),     FRWeight60bc*event_weight,200, -1, 1 ) ;

	FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_Loose", leps[1]->CloseJet_BScore(), event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_W40aLoose", leps[1]->CloseJet_BScore(), FRWeight40a*event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_W40bLoose", leps[1]->CloseJet_BScore(), FRWeight40b*event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_W40cLoose", leps[1]->CloseJet_BScore(), FRWeight40c*event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_W40BaLoose", leps[1]->CloseJet_BScore(), FRWeight40ba*event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_W40BbLoose", leps[1]->CloseJet_BScore(), FRWeight40bb*event_weight,200, -1, 1 ) ;
	FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_W40BcLoose", leps[1]->CloseJet_BScore(), FRWeight40bc*event_weight,200, -1, 1 ) ;

	if(leps[1]->PassLepID()) {
	  FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_LFvsHF_Tight", leps[1]->HNL_MVA_Fake("QCD_LFvsHF_v5"), event_weight,200, -1, 1 ) ;
	  FillHist("MCClosure/"+param.Name+"/TrailingPt_QCD_BvsC_Tight", leps[1]->HNL_MVA_Fake("QCD_BvsC_v5"), event_weight,200, -1, 1 ) ;
          FillHist("MCClosure/"+param.Name+"/TrailingPt_CloseJet_BScore_Tight", leps[1]->CloseJet_BScore(), event_weight,200, -1, 1 ) ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPt_Tight",  leps[1]->Pt(), event_weight,"Pt80") ;
	}
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_Loose", leps[1]->Pt(), event_weight,"Pt80") ;

	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W25aBLoose", leps[1]->Pt(),  FRWeight25ba*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W25bBLoose", leps[1]->Pt(),  FRWeight25bb*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W25cBLoose", leps[1]->Pt(),  FRWeight25bc*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W30aBLoose", leps[1]->Pt(),  FRWeight30ba*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W30bBLoose", leps[1]->Pt(),  FRWeight30bb*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W30cBLoose", leps[1]->Pt(),  FRWeight30bc*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W40aBLoose", leps[1]->Pt(),  FRWeight40ba*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W40bBLoose", leps[1]->Pt(),  FRWeight40bb*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W40cBLoose", leps[1]->Pt(),  FRWeight40bc*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W60aBLoose", leps[1]->Pt(),  FRWeight60ba*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W60bBLoose", leps[1]->Pt(),  FRWeight60bb*event_weight,"Pt80") ;
	FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W60cBLoose", leps[1]->Pt(),  FRWeight60bc*event_weight,"Pt80") ;

        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W25aLoose", leps[1]->Pt(),  FRWeight25a*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W25bLoose", leps[1]->Pt(),  FRWeight25b*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W25cLoose", leps[1]->Pt(),  FRWeight25c*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W30aLoose", leps[1]->Pt(),  FRWeight30a*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W30bLoose", leps[1]->Pt(),  FRWeight30b*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W30cLoose", leps[1]->Pt(),  FRWeight30c*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W40aLoose", leps[1]->Pt(),  FRWeight40a*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W40bLoose", leps[1]->Pt(),  FRWeight40b*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W40cLoose", leps[1]->Pt(),  FRWeight40c*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W60aLoose", leps[1]->Pt(),  FRWeight60a*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W60bLoose", leps[1]->Pt(),  FRWeight60b*event_weight,"Pt80") ;
        FillHistogram("MCClosure/"+param.Name+"/TrailingPt_W60cLoose", leps[1]->Pt(),  FRWeight60c*event_weight,"Pt80") ;


	if(lep_ptparton > 10) {
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_Loose", lep_ptparton,  event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W25aLoose", lep_ptparton,  FRWeight25a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W25bLoose", lep_ptparton,  FRWeight25b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W25cLoose", lep_ptparton,  FRWeight25c*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W30aLoose", lep_ptparton,  FRWeight30a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W30bLoose", lep_ptparton,  FRWeight30b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W30cLoose", lep_ptparton,  FRWeight30c*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W40aLoose", lep_ptparton,  FRWeight40a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W40bLoose", lep_ptparton,  FRWeight40b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W40cLoose", lep_ptparton,  FRWeight40c*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W60aLoose", lep_ptparton,  FRWeight60a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W60bLoose", lep_ptparton,  FRWeight60b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W60cLoose", lep_ptparton,  FRWeight60c*event_weight,"Pt80") ;
	  
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W25aBLoose", lep_ptparton,  FRWeight25ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W25bBLoose", lep_ptparton,  FRWeight25bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W25cBLoose", lep_ptparton,  FRWeight25bc*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W30aBLoose", lep_ptparton,  FRWeight30ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W30bBLoose", lep_ptparton,  FRWeight30bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W30cBLoose", lep_ptparton,  FRWeight30bc*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W40aBLoose", lep_ptparton,  FRWeight40ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W40bBLoose", lep_ptparton,  FRWeight40bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W40cBLoose", lep_ptparton,  FRWeight40bc*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W60aBLoose", lep_ptparton,  FRWeight60ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W60bBLoose", lep_ptparton,  FRWeight60bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_W60cBLoose", lep_ptparton,  FRWeight60bc*event_weight,"Pt80") ;


	  if(leps[1]->PassLepID()) FillHistogram("MCClosure/"+param.Name+"/TrailingPtParton_Tight", lep_ptparton, event_weight,"Pt80") ;
	}
	if(PtCorr > 10) {
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_Loose", PtCorr, event_weight,"Pt80") ;

          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W25aBLoose", PtCorr,  FRWeight25ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W25bBLoose", PtCorr,  FRWeight25bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W25cBLoose", PtCorr,  FRWeight25bc*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W30aBLoose", PtCorr,  FRWeight30ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W30bBLoose", PtCorr,  FRWeight30bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W30cBLoose", PtCorr,  FRWeight30bc*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W40aBLoose", PtCorr,  FRWeight40ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W40bBLoose", PtCorr,  FRWeight40bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W40cBLoose", PtCorr,  FRWeight40bc*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W60aBLoose", PtCorr,  FRWeight60ba*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W60bBLoose", PtCorr,  FRWeight60bb*event_weight,"Pt80") ;
          FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W60cBLoose", PtCorr,  FRWeight60bc*event_weight,"Pt80") ;


	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W25aLoose", PtCorr,  FRWeight25a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W25bLoose", PtCorr,  FRWeight25b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W25cLoose", PtCorr,  FRWeight25c*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W30aLoose", PtCorr,  FRWeight30a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W30bLoose", PtCorr,  FRWeight30b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W30cLoose", PtCorr,  FRWeight30c*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W40aLoose", PtCorr,  FRWeight40a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W40bLoose", PtCorr,  FRWeight40b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W40cLoose", PtCorr,  FRWeight40c*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W60aLoose", PtCorr,  FRWeight60a*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W60bLoose", PtCorr,  FRWeight60b*event_weight,"Pt80") ;
	  FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_W60cLoose", PtCorr,  FRWeight60c*event_weight,"Pt80") ;
	  if(leps[1]->PassLepID()) FillHistogram("MCClosure/"+param.Name+"/TrailingPtCorr_Tight",   PtCorr, event_weight,"Pt80") ;
	}

      }      

    }
    return;
  }

  // remove if muon              
  
  if(loose_el.size() > 0) return;

  if(param.HasFlag("MCFakes")) {
    
    std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param);
    std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);

    if(leps.size()==1){
      if (leps[0]->Pt() < 100){
	
	if(UseEvent(leps , jets, 25., METv, event_weight)){
          GetMuFakeRates("Pt",       leps[0], blepsT[0], param, jets, "MCFake_J25" ,  event_weight);
	  GetMuFakeRates("PtParton", leps[0], blepsT[0], param, jets, "MCFake_J25" ,  event_weight);
	  GetMuFakeRates("PtCorr",   leps[0], blepsT[0], param, jets, "MCFake_J25" ,  event_weight);
	}

	if(UseEvent(leps , jets, 30., METv, event_weight))      {
	  GetMuFakeRates("Pt",       leps[0], blepsT[0], param, jets, "MCFake_J30" ,  event_weight);
	  GetMuFakeRates("PtParton", leps[0], blepsT[0], param, jets, "MCFake_J30" ,  event_weight);
          GetMuFakeRates("PtCorr",   leps[0], blepsT[0], param, jets, "MCFake_J30" ,  event_weight);
	}
	if(UseEvent(leps , jets, 60., METv, event_weight))       {
	  GetMuFakeRates("Pt",       leps[0], blepsT[0], param, jets, "MCFake_J60" ,  event_weight);
	  GetMuFakeRates("PtParton", leps[0], blepsT[0], param, jets, "MCFake_J60" ,  event_weight);
          GetMuFakeRates("PtCorr",   leps[0], blepsT[0], param, jets, "MCFake_J60" ,  event_weight);
	}
	
	if(UseEvent(leps , jets, 40., METv, event_weight)){
	  GetMuFakeRates("Pt",       leps[0], blepsT[0], param, jets, "MCFake_J40" ,  event_weight);
	  GetMuFakeRates("PtParton", leps[0], blepsT[0], param, jets, "MCFake_J40" ,  event_weight);
	  GetMuFakeRates("PtCorr",   leps[0], blepsT[0], param, jets, "MCFake_J40" ,  event_weight);
	}
      }
    }

    for(unsigned int imu = 0; imu < leps.size(); imu++){

      if(leps[imu]->Pt() > 100) continue;
      TString lep_fake_tag = MatchGenDef(All_Gens, Lepton(loose_mu[imu]));
      double PTPartonSF    = GetPtPartonSF(*leps[imu],param.Muon_Loose_ID);
      double MVACut        = leps[imu]->MVAFakeCut(param.Muon_Tight_ID,GetYearString());
      double PtCorr        = (leps[imu]->CalcMVACone(MVACut)  < 80) ? leps[imu]->CalcMVACone( MVACut)  : 79;
      double lep_ptparton  = (leps[imu]->PtParton(PTPartonSF,MVACut));
            
      TString PTPatBin = "Pt1";
      if(lep_ptparton < 15)  PTPatBin = "Pt1";
      else       if(lep_ptparton <25)  PTPatBin = "Pt2";
      else       if(lep_ptparton <45)  PTPatBin = "Pt3";
      else PTPatBin = "Pt4";
      
      if(lep_fake_tag.Contains("pi")) lep_fake_tag = "pi";
      else if(lep_fake_tag.Contains("K")) lep_fake_tag = "K";
      else if(lep_fake_tag.Contains("D")) lep_fake_tag = "D";
      else if(lep_fake_tag.Contains("B")) lep_fake_tag = "B";
      else if(lep_fake_tag.Contains("q")) lep_fake_tag = "q";
      else if(lep_fake_tag.Contains("g")) lep_fake_tag = "g";
      else if(lep_fake_tag.Contains("elec")) lep_fake_tag = "el";
      else lep_fake_tag = "Other";
      
      vector<TString> lables = {"el","pi","K","q","g","D","B","Other"};//GetGenList();
      if (std::find(lables.begin(), lables.end(), lep_fake_tag) == lables.end()) continue;

      for(int i=0 ; i < 2 ; i++){
	TString LorT    = (i==0)?  "Loose" : "Tight";
	TString DirInc  = "MCFakeOpt/"+LorT+"/";
	if(i==1){
	  if(!loose_mu[imu].PassID(param.Muon_Tight_ID)) continue;
	}
	
      	FillCutflow(param, LorT+"_Lep_Fake_type", event_weight, lables,lep_fake_tag);
	     
	FillHist((DirInc+PTPatBin+"/MVA_"       +loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(),loose_mu[imu].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
	FillHist((DirInc+PTPatBin+"/QCD_LFvsHF_"+loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
	FillHist((DirInc+PTPatBin+"/QCD_BvsC_"  +loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
	FillHist((DirInc+PTPatBin+"/QCD_LF1_"  +loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].HNL_MVA_Fake("QCD_LF1_v5") ,  event_weight, 200, -1, 1 );
	FillHist((DirInc+PTPatBin+"/QCD_LF2_"  +loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].HNL_MVA_Fake("QCD_LF2_v5") ,  event_weight, 200, -1, 1 );
	FillHist((DirInc+PTPatBin+"/BScore_"    +loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
	FillHist((DirInc+PTPatBin+"/CvsB_"   +loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
	FillHist((DirInc+PTPatBin+"/CvsL_"   +loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
	FillHist((DirInc+PTPatBin+"/PtRatio_"+loose_mu[imu].MotherJetFlavour()+"_"+param.Name ).Data(), loose_mu[imu].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	
	FillHist((DirInc+"GENSplit_"+param.Name + "/MVA/"+PTPatBin+"/"+lep_fake_tag).Data(),       loose_mu[imu].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"GENSplit_"+param.Name + "/QCD_LFvsHF/"+PTPatBin+"/"+lep_fake_tag ).Data(),loose_mu[imu].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
        FillHist((DirInc+"GENSplit_"+param.Name + "/QCD_BvsC/"+PTPatBin+"/"+lep_fake_tag ).Data(), loose_mu[imu].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"GENSplit_"+param.Name + "/QCD_LF1/"+PTPatBin+"/"+lep_fake_tag ).Data(),loose_mu[imu].HNL_MVA_Fake("QCD_LF1_v5") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"GENSplit_"+param.Name + "/QCD_LF2/"+PTPatBin+"/"+lep_fake_tag ).Data(),loose_mu[imu].HNL_MVA_Fake("QCD_LF2_v5") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"GENSplit_"+param.Name + "/BScore/"+PTPatBin+"/"+lep_fake_tag ).Data(), loose_mu[imu].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
	FillHist((DirInc+"FlavourSplit_"+param.Name +"/MVA/"+PTPatBin+"/"+loose_mu[imu].MotherJetFlavour() ).Data(),       loose_mu[imu].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"FlavourSplit_"+param.Name +"/QCD_LFvsHF/"+PTPatBin+"/"+loose_mu[imu].MotherJetFlavour() ).Data(),loose_mu[imu].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
        FillHist((DirInc+"FlavourSplit_"+param.Name +"/QCD_BvsC/"+PTPatBin+"/"+loose_mu[imu].MotherJetFlavour() ).Data(), loose_mu[imu].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"FlavourSplit_"+param.Name +"/QCD_LF1/"+PTPatBin+"/"+loose_mu[imu].MotherJetFlavour() ).Data(),loose_mu[imu].HNL_MVA_Fake("QCD_LF1_v5") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"FlavourSplit_"+param.Name +"/QCD_LF2/"+PTPatBin+"/"+loose_mu[imu].MotherJetFlavour() ).Data(),loose_mu[imu].HNL_MVA_Fake("QCD_LF2_v5") ,  event_weight, 200, -1, 1 );
        FillHist((DirInc+"FlavourSplit_"+param.Name +"/BScore/"+PTPatBin+"/"+loose_mu[imu].MotherJetFlavour() ).Data(), loose_mu[imu].CloseJet_BScore() ,  event_weight, 200, -1, 1 );

	FillHistogram(DirInc+"Pt/"+lep_fake_tag+"/PtParton", lep_ptparton, event_weight,"Pt50") ;
	FillHistogram(DirInc+"Pt/"+lep_fake_tag+"/PtCorr",   PtCorr, event_weight,      "Pt50") ;
	FillHistogram(DirInc+"Pt/"+lep_fake_tag+"/"+loose_mu[i].MotherJetFlavour()+"/PtParton", lep_ptparton, event_weight,"Pt50") ;
	
      } 
    }
  }

  bool useevent40 = UseEvent(leps , jets, 40., METv, event_weight);
  
  if(param.HasFlag("MCProfile")){

    for(auto ilep : leps){
      
      double PtPartonUncorr = ilep->PtParton(1,ilep->MVAFakeCut(param.Muon_Tight_ID,GetYearString()));
      double W = event_weight;
      
      if(ilep->Pt() > 80) continue;
      
      FillHist(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_PtPartonUncorr").Data(),     PtPartonUncorr, W, 100,0., 200.);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake(), PtPartonUncorr, W, 200, -1, 1);
      /// Eta binning
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->sRegion()+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake(), PtPartonUncorr, W, 100, -1, 1);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->FakeTaggerStringLF()+"_"+ilep->sRegion()+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake(), PtPartonUncorr, W, 100, -1, 1);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->FakeTaggerStringHF()+"_"+ilep->sRegion()+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake(), PtPartonUncorr, W, 100, -1, 1);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->MotherJetFlavour() + "_"+ilep->sRegion()+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake(), PtPartonUncorr, W, 100, -1, 1);

    }
  }

  if(param.HasFlag("Standard")){
    
    vector<TString> GENLABELS = {"pion","Kaon","quark","gluon","B","C","Other"};
    
    vector<TString> RegionLabel = {"MCFakeBins_","SSMCFakeBins_","FakeCRMCFakeBins_"};
    
    for(auto ir : RegionLabel){
      if(ir == "FakeCRMCFakeBins_" && !useevent40) continue;
      if(ir == "SSMCFakeBins_"){
	if( ! SameCharge(leps)) continue;
	if(leps.at(0)->Pt() < 20) continue;
	if(leps.at(1)->Pt() < 10) continue;
      }
      
      for(auto ilep : leps){
	
	TString lep_fake_tag = MatchGenDef(All_Gens, *ilep);
	if(lep_fake_tag.Contains("pi")) lep_fake_tag = "pion";
	else if(lep_fake_tag.Contains("K")) lep_fake_tag = "Kaon";
	else if(lep_fake_tag == "q") lep_fake_tag = "quark";
	else if(lep_fake_tag == "g") lep_fake_tag = "gluon";
	else if(lep_fake_tag.Contains("B")) lep_fake_tag = "B";
	else if(lep_fake_tag.Contains("C")) lep_fake_tag = "C";
	else if(lep_fake_tag.Contains("D")) lep_fake_tag = "C";
	else lep_fake_tag = "Other";
	
	
	for(int i=0; i < 2; i++){
	  
	  TString isLoT = "Loose";
	  if(i==1) {
	    isLoT = "Tight";
	    if(!ilep->PassLepID()) continue;
	  }
	  
	  FillCutflow(param, ir+"_Lep_Fake_type_"+isLoT  , event_weight, GENLABELS, lep_fake_tag);
	  
	  vector<TString> lables = GetGenList();
	  if (!(std::find(lables.begin(), lables.end(), lep_fake_tag) == lables.end()) ) FillCutflow(param, isLoT+"_All_Lep_Fake_type", event_weight, lables, MatchGenDef(All_Gens, *ilep));
	  
	  for(int j=0; j < 2; j++){
	    
	    TString Flav = "InclusiveFlavour_";
	    if(j==1) Flav = ilep->MotherJetFlavour()+"_";
	    
	    TString PtBinLabel = "";
	    if(ilep->Pt() < 20)  PtBinLabel = "PtBin1";
	    else if(ilep->Pt() < 40)  PtBinLabel = "PtBin2";
	    else if(ilep->Pt() < 60)  PtBinLabel = "PtBin3";
	    else  PtBinLabel = "PtBin4";
	    
	    FillCutflow(param, ir+"_"+Flav+"_"+PtBinLabel+"_Lep_Fake_type_" + isLoT , event_weight, GENLABELS, lep_fake_tag);
	    if(jets.size()== 0)           FillCutflow(param, ir+"_"+Flav+"_"+PtBinLabel+"_0J_Lep_Fake_type_" + isLoT  , event_weight, GENLABELS, lep_fake_tag);
	    else if(jets.size()== 1)      FillCutflow(param, ir+"_"+Flav+"_"+PtBinLabel+"_1J_Lep_Fake_type_" + isLoT  , event_weight, GENLABELS, lep_fake_tag);
	    else                          FillCutflow(param, ir+"_"+Flav+"_"+PtBinLabel+"_2pJ_Lep_Fake_type_" + isLoT  , event_weight, GENLABELS, lep_fake_tag);
	    
	    
	    for(int k=0; k < 2; k++){
	      
	      TString genlabel = "InclusiveGEN_";
	      if(k==1) genlabel = lep_fake_tag+"_";
	      
	      FillHist((ir+ genlabel+  Flav + isLoT+ "_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_MVA").Data(),    ilep->HNL_MVA_Fake(), event_weight, 100,-1., 1.);
	      FillHist((ir+ genlabel+  Flav + isLoT+ "_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_LFvsHF").Data(),    ilep->HNL_MVA_Fake("QCD_LFvsHF_v5"), event_weight, 100,-1., 1.);
	      FillHist((ir+ genlabel+  Flav + isLoT+ "_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_HFBvsHFC").Data(),    ilep->HNL_MVA_Fake("QCD_BvsC_v5"), event_weight, 100,-1., 1.);
	      FillHist((ir+ genlabel+  Flav + isLoT+ "_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_PtRatio").Data(),    ilep->CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	      
	      if(jets.size()== 0) {
		FillHist((ir+ genlabel+  Flav + isLoT+ "_0J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_MVA").Data(),    ilep->HNL_MVA_Fake(), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_0J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_LFvsHF").Data(),    ilep->HNL_MVA_Fake("QCD_LFvsHF_v5"), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_0J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_HFBvsHFC").Data(),    ilep->HNL_MVA_Fake("QCD_BvsC_v5"), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_0J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_PtRatio").Data(),    ilep->CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	      }
	      else if(jets.size()== 1) {
		FillHist((ir+ genlabel+  Flav + isLoT+ "_1J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_MVA").Data(),    ilep->HNL_MVA_Fake(), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_1J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_LFvsHF").Data(),    ilep->HNL_MVA_Fake("QCD_LFvsHF_v5"), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_1J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_HFBvsHFC").Data(),    ilep->HNL_MVA_Fake("QCD_BvsC_v5"), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_1J_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_PtRatio").Data(),    ilep->CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	      }
	      else{
		FillHist((ir+ genlabel+  Flav + isLoT+ "_2pJ_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_MVA").Data(),    ilep->HNL_MVA_Fake(), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_2pJ_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_LFvsHF").Data(),    ilep->HNL_MVA_Fake("QCD_LFvsHF_v5"), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_2pJ_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_HFBvsHFC").Data(),    ilep->HNL_MVA_Fake("QCD_BvsC_v5"), event_weight, 100,-1., 1.);
		FillHist((ir+ genlabel+  Flav + isLoT+ "_2pJ_"+PtBinLabel+"/FakeCR"+param.GetSystType()+"_PtRatio").Data(),    ilep->CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	      }
	    }
	  }
	}
      }
    }
  }
  return;

} /// end function                      





void HNL_Lepton_FakeRate_MC::RunE( std::vector<Electron> loose_el, std::vector<Muon> loose_mu, std::vector<Jet> jets, AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleMuon") return;
    if(this->DataStream == "SingleMuon") return;
  }

  Event ev = GetEvent();

  if(loose_el.size() == 0) return;
 
  // remove if muon 
  if(loose_mu.size() > 0) return;

  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_el,param);

  std::vector<bool> blepsT;
  for(auto ilep : loose_el)   blepsT.push_back(ilep.PassID(param.Electron_Tight_ID));
  

  if(param.HasFlag("MCFakes")) {
    
    for(unsigned int i = 0 ; i < loose_el.size() ; i++){
      TString Dir = "MCFakeOpt/";
      TString DirInc = "MCFakeOpt/Inclusive/";
      
      FillHist((DirInc+"ISO_"    +loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].RelIso() ,  event_weight, 60, 0, 0.6 );
      FillHist((DirInc+"QCD_LFvsHF_"+loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
      FillHist((DirInc+"QCD_BvsC_"+loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
      FillHist((DirInc+"BScore_" +loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
      FillHist((DirInc+"CvsB_"   +loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
      FillHist((DirInc+"CvsL_"   +loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
      FillHist((DirInc+"PtRatio_"+loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
      FillHist((DirInc+"PtRel_"  +loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name +"_Loose").Data(), loose_el[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );
      
      for(int imva=0 ; imva < 150 ; imva++){

        double mva_d=  1 - double(imva)*.01;
        TString mvaTS= DoubleToString(mva_d);
        TString var = "QCD_BvsC_v5_";
	TString prefix=loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name;

        if(loose_el[i].HNL_MVA_Fake("QCD_BvsC_v5") > mva_d) continue;

        FillHist((Dir+var+mvaTS+"/"+prefix +"_Loose").Data(), 1,  event_weight, 2, 0 , 2);
        if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_Tight").Data(), 1,  event_weight, 2, 0 , 2);
	if(loose_el[i].Pt() < 30) {
          FillHist((Dir+var+mvaTS+"/"+prefix +"_ptlt30_Loose").Data(), 1,  event_weight, 2, 0 , 2);
          if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_ptlt30_Tight").Data(), 1,  event_weight, 2, 0 , 2);
	}
	else{
          FillHist((Dir+var+mvaTS+"/"+prefix +"_ptgt30_Loose").Data(), 1,  event_weight, 2, 0 , 2);
          if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_ptgt30_Tight").Data(), 1,  event_weight, 2, 0 , 2);
        }
      }

      for(int imva=0 ; imva < 150 ; imva++){

        double mva_d=  -1 + double(imva)*.01;
        TString mvaTS= DoubleToString(mva_d);
        TString var = "QCD_LFvsHF_v5_";
        TString prefix=loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name;

        if(loose_el[i].HNL_MVA_Fake("QCD_LFvsHF_v5") < mva_d) continue;

        FillHist((Dir+var+mvaTS+"/"+prefix +"_Loose").Data(), 1,  event_weight, 2, 0 , 2);
	if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_Tight").Data(), 1,  event_weight, 2, 0 , 2);
        if(loose_el[i].Pt() < 30) {
          FillHist((Dir+var+mvaTS+"/"+prefix +"_ptlt30_Loose").Data(), 1,  event_weight, 2, 0 , 2);
          if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_ptlt30_Tight").Data(), 1,  event_weight, 2, 0 , 2);
        }
        else{
          FillHist((Dir+var+mvaTS+"/"+prefix +"_ptgt30_Loose").Data(), 1,  event_weight, 2, 0 , 2);
          if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_ptgt30_Tight").Data(), 1,  event_weight, 2, 0 , 2);
        }
      }


      for(int imva=0 ; imva < 100 ; imva++){

        double mva_d=  1 - double(imva)*.01;
        TString mvaTS= DoubleToString(mva_d);
	TString var = "DeepJetB_";
        TString prefix=loose_el[i].MotherJetFlavour()+"_"+loose_el[i].sRegion()+"_"+param.Name;
        if(loose_el[i].CloseJet_BScore() > mva_d) continue;
	FillHist((Dir+var+mvaTS+"/"+prefix +"_Loose").Data(), 1,  event_weight, 2, 0 , 2);
	if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_Tight").Data(), 1,  event_weight, 2, 0 , 2);

	if(loose_el[i].Pt() < 30) {
          FillHist((Dir+var+mvaTS+"/"+prefix +"_ptlt30_Loose").Data(), 1,  event_weight, 2, 0 , 2);
          if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_ptlt30_Tight").Data(), 1,  event_weight, 2, 0 , 2);
	}
	else{
          FillHist((Dir+var+mvaTS+"/"+prefix +"_ptgt30_Loose").Data(), 1,  event_weight, 2, 0 , 2);
          if(loose_el[i].PassID(param.Electron_Tight_ID)) FillHist((Dir+var+mvaTS+"/"+prefix +"_ptgt30_Tight").Data(), 1,  event_weight, 2, 0 , 2);
        }
      }
    }
  }
  

  if(param.HasFlag("MCProfile")){
    
    for(auto ilep : leps){
      if(ilep->Pt() > 80) continue;

      double PtPartonUncorr = ilep->PtParton(1,ilep->MVAFakeCut(param.Electron_Tight_ID,GetYearString()));
      double W = event_weight;
      
      FillHist(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_PtPartonUncorr").Data(),PtPartonUncorr, W, 40,0., 200.);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HNL"), PtPartonUncorr, W, 40, -1, 1);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->etaRegionString()+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HNL"), PtPartonUncorr, W, 40, -1, 1);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_3bin_"+ilep->GetEtaRegion("3bin")+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HNL"), PtPartonUncorr, W, 40, -1, 1);
    }
    return;
  }

  
  return;
  


} /// end function RunE


bool HNL_Lepton_FakeRate_MC::UseEvent(std::vector<Lepton *> leps ,  std::vector< Jet> jets, float awayjetcut, Particle MET, float wt){

  bool useevent = false;
  if(leps.size() != 1) return false;
  
  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int w = 0; w < leps.size();  w++){
    METdphi = TVector2::Phi_mpi_pi(leps.at(w)->Phi()-MET.Phi());
    //    MT = sqrt(2.* leps.at(w)->Et()*MET.Pt() * (1 - cos( METdphi)));
    MT = sqrt(2.* 35*MET.Pt() * (1 - cos( METdphi)));
    if(( (MET.Pt() < 40) && (MT < 25.)) ) {

      for (unsigned int ielT=0; ielT < leps.size(); ielT++){
        for(unsigned int ij=0; ij < jets.size(); ij++){
          if(jets.at(ij).Pt() < awayjetcut) continue;
	  //       float dphi =fabs(TVector2::Phi_mpi_pi(leps.at(ielT)->Phi()- jets.at(ij).Phi()));
	  if( (jets.at(ij).ChargedEmEnergyFraction()) > 0.65)  continue;

          if(leps.at(ielT)->DeltaR(jets.at(ij))> 0.7){
            //if((jets.at(ij).Pt() /  leps.at(ielT)->Pt() ) < 1.2) continue;
            useevent = true;
          }
	}
      }
    }
  }

  return useevent;
}


void HNL_Lepton_FakeRate_MC::GetElFakeRates(TString Method, Lepton* lep,bool blepsT,  AnalyzerParameter param,  std::vector<Jet> jets,  TString tag,float event_weight){

  bool IsMuon=(lep->LeptonFlavour() == Lepton::MUON);
  if(IsMuon) return;

  if(param.Name.Contains("HNL_")){
    if(Method.Contains("PtCone")) return;
  }
  else{
    if(!Method.Contains("PtCone")) return;
  }

  double   isocut  = (Method == "PtCone") ? GetIsoFromID(*lep, param.Electron_Tight_ID) : 0. ;
  TString  LooseID =  param.Electron_Loose_ID ;
  TString  TightID =  param.Electron_Tight_ID ;
  double MVACut = lep->MVAFakeCut(TightID,GetYearString());


  Event ev = GetEvent();
  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";

  bool El8PD= true;
  bool EL12PD= true;
  bool EL23PD= true;

  bool pass_8  = ev.PassTrigger(triggerslist_8) && El8PD;
  bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
  bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;

  int For_HLT_PFJet30_v=0;
  for(auto ij : jets){
    if(ij.Pt() > 40.) For_HLT_PFJet30_v++;
  }
  if(!For_HLT_PFJet30_v) return;

  float lep_pt (0);
  
  float weight_ptcorr=event_weight;

  TString ptname = "pt_eta";
  TString PtHist = "pt"; /// Used to set binning                                                                                                                                                                  

  if(Method == "Pt"){
    lep_pt      =  (lep->Pt() < 80) ?  lep->Pt() : 79;

    if(lep_pt > 25){
      if(!pass_23) return;
    }
    else  if(lep_pt > 15){
      if(!pass_12) return;
    }
    else  if(lep_pt > 10){
      if(!pass_8) return;
    }
  }
  if(Method == "PtCone"){
    PtHist ="ptcone";
    lep_pt =  ( lep->CalcPtCone(lep->RelIso(), isocut) < 80) ?  lep->Pt() : 79;
    ptname = "ptcone_eta";
    if(lep_pt > 40){
      if(lep->Pt() < 25) return;
      if(!pass_23) return;
    }
    else if(lep_pt > 23){
      if(lep->Pt() < 15) return;
      if(!pass_12) return;
    }
    else if(lep_pt > 10){
      if(lep->Pt() < 9.5) return;
      if(!pass_8) return;

    }
    else return ;
  }
  else if(Method == "PtParton"){
    PtHist ="ptcone";
    ptname = "ptparton_eta";
    double PTPartonSF = GetPtPartonSF(*lep, LooseID);

    lep_pt      =  (lep->PtParton(PTPartonSF,MVACut) < 80) ?  lep->PtParton(PTPartonSF, MVACut) : 79;

    FillProf((param.Name + "_FakeCR40_MVA_PtParton").Data(), lep->HNL_MVA_Fake(), lep_pt, event_weight, 100, -1, 1);

    if(lep_pt > 40){
      if(lep->Pt() < 25) return;
      if(!pass_23) return;
    }
    else if(lep_pt > 23){
      if(lep->Pt() < 10) return;
      if(!pass_12) return;
    }
    else if(lep_pt > 10){
      if(lep->Pt() < 9.5) return;
      if(!pass_8) return;

    }
    else return ;
    
  }
  if(Method == "PtCorr"){

    PtHist ="ptcone";
    ptname = "ptcorr_eta";
    lep_pt      = (lep->CalcMVACone(MVACut)  < 80) ? lep->CalcMVACone(MVACut)  : 79;
    

    if(lep_pt > 40){
      if(lep->Pt() < 25) return;
      if(!pass_23) return;
    }
    else if(lep_pt > 23){
      if(lep->Pt() < 10) return;
      if(!pass_12) return;
    }
    else if(lep_pt > 10){
      if(lep->Pt() < 9.5) return;
      if(!pass_8) return;

    }
    else return ;

  }
  if(Method == "MotherPt"){
    PtHist ="ptcone";
    ptname = "MJpt_eta";
    lep_pt      = lep->MotherJetPt();


    if(lep_pt > 40){
      if(lep->Pt() < 25) return;
      if(!pass_23) return;
    }
    else if(lep_pt > 23){
      if(lep->Pt() < 10) return;
      if(!pass_12) return;
    }
    else if(lep_pt > 10){
      if(lep->Pt() < 9.5) return;
      if(!pass_8) return;

    }
    else return ;

  }

  float lep_eta     = lep->fEta(); /// returns |eta| OR for el |scEta|
  float lep_mva_lfvshf  = lep->HNL_MVA_Fake("QCD_LFvsHF_v5");
  float lep_mva_bvsc    = lep->HNL_MVA_Fake("QCD_BvsC_v5");
  float lep_blscore     = lep->CloseJet_BScore();
  float lep_cbscore     = lep->CloseJet_CvsBScore();
  float lep_clscore     = lep->CloseJet_CvsLScore();
  TString lepEtaRegion  = lep->etaRegionString();
  TString lepRegion     = (lep->IsBB()) ? "BB" : "EC";

  TString L_prefix = "Fake_"+Method+"_Loose"+tag;
  TString T_prefix = "Fake_"+Method+"_Tight"+tag;

  for(int ilep = 0 ; ilep < 2; ilep++)  {

    TString prefix = (ilep==0) ? L_prefix : T_prefix;
    if((ilep==1) && !(blepsT)) continue;
    
    prefix = Method + "/"+prefix;

    TString Ptlab = "p_{T} (GeV)";

    if(lep_pt >10){
      FillHistogram((prefix + "_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_FinerBins_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+lep->GetFlavour()+"_"+ PtHist , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_eta").Data(),                lep_eta, weight_ptcorr ,"FR_Eta","#eta");
      FillHist((prefix + "_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      FillHist((prefix + "_lep_mva_lfvshf").Data(),   lep_mva_lfvshf,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_lep_mva_bvsc").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_blscore").Data(), lep_blscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_cbscore").Data(), lep_cbscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_clscore").Data(), lep_clscore,    weight_ptcorr, 50, -1., 1.);
      
      if(lep_mva_lfvshf > 0.8)FillHist((prefix + "_lep_mva_bvsc_LF1").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      else  if(lep_mva_lfvshf > 0.6)FillHist((prefix + "_lep_mva_bvsc_LF2").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      else  if(lep_mva_lfvshf > 0.4)FillHist((prefix + "_lep_mva_bvsc_LF3").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      else  if(lep_mva_lfvshf > -0.2)FillHist((prefix + "_lep_mva_bvsc_LF4").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      else  if(lep_mva_lfvshf > -0.5)FillHist((prefix + "_lep_mva_bvsc_LF5").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      else FillHist((prefix + "_lep_mva_bvsc_LF6").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);

      TString TaggerLabel = "";
      if(lep_mva_lfvshf > 0.8) {
	if(lep_mva_bvsc > 0.7) TaggerLabel = "LF1_HF1";
	else if(lep_mva_bvsc > 0.2) TaggerLabel = "LF1_HF2";
	else if(lep_mva_bvsc > -0.3) TaggerLabel = "LF1_HF3";
	else TaggerLabel = "LF1_HF4";
      }
      else  if(lep_mva_lfvshf > 0.5) {
	if(lep_mva_bvsc > 0.7)      TaggerLabel = "LF2_HF1";
	else if(lep_mva_bvsc > 0.2) TaggerLabel = "LF2_HF2";
        else if(lep_mva_bvsc > -0.3) TaggerLabel = "LF2_HF3";
        else TaggerLabel = "LF2_HF4";
      }
      else  if(lep_mva_lfvshf > 0.1) {
	if(lep_mva_bvsc > 0.7) TaggerLabel = "LF3_HF1";
	else if(lep_mva_bvsc > 0.2) TaggerLabel = "LF3_HF2";
        else if(lep_mva_bvsc > -0.3) TaggerLabel = "LF3_HF3";
        else TaggerLabel = "LF3_HF4";
      } 
      else  if(lep_mva_lfvshf > -0.2) {
	if(lep_mva_bvsc > 0.7) TaggerLabel = "LF4_HF1";
	else if(lep_mva_bvsc > 0.2) TaggerLabel = "LF4_HF2";
        else if(lep_mva_bvsc > -0.3) TaggerLabel = "LF4_HF3";
        else TaggerLabel = "LF4_HF4";
      } 
      else{
	if(lep_mva_bvsc > 0.7) TaggerLabel = "LF5_HF1";
	else if(lep_mva_bvsc > 0.2) TaggerLabel = "LF5_HF2";
        else if(lep_mva_bvsc > -0.3) TaggerLabel = "LF5_HF3";
        else TaggerLabel = "LF5_HF4";
      }
            
      FillHistogram((prefix + "_"+TaggerLabel+"_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_"+TaggerLabel+"_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+lep->GetFlavour()+"_"+ PtHist , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_"+TaggerLabel+"_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_"+TaggerLabel+"_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_"+TaggerLabel+"_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_"+TaggerLabel+"_eta").Data(),                lep_eta, weight_ptcorr ,"FR_Eta","#eta");
      FillHist((prefix + "_"+TaggerLabel+"_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");

      FillHistogram((prefix + "_LF_"+ptname).Data(),             lep_pt, lep_mva_lfvshf,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist , "FR_BDT", Ptlab);
      FillHistogram((prefix + "_HF_"+ptname).Data(),             lep_pt, lep_mva_bvsc,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+ PtHist , "FR_BDT", Ptlab);
    }
  }
  return;

}

void HNL_Lepton_FakeRate_MC::GetMuFakeRates(TString Method, Lepton*  lep, bool blepsT,  AnalyzerParameter param,  std::vector<Jet> jets,  TString tag,float event_weight){
  

  bool IsMuon=(lep->LeptonFlavour() == Lepton::MUON);
  if(!IsMuon) return;

  if(param.Name.Contains("HNL_")){
    if(Method.Contains("PtCone")) return;
  }
  else{
    if(!Method.Contains("PtCone")) return;
  }
  
  FillHist("GetMuFakeRates",   1, event_weight, 1, 0,1);


  double   isocut  = (Method == "PtCone") ? GetIsoFromID(*lep, param.Muon_Tight_ID) : 0.;
  TString  LooseID =  param.Muon_Loose_ID ;
  TString  TightID =  param.Muon_Tight_ID ;
  double MVACut = lep->MVAFakeCut(TightID,GetYearString());
  
  ///// TRIGGER CONDITIONS

  Event ev = GetEvent();  

  TString triggerslist_3="HLT_Mu3_PFJet40_v";  TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";  TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";
  bool Mu3PD= true;
  bool Mu8PD= true;
  bool Mu17PD= true;

  bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
  bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
  bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;

  int For_HLT_Mu3_PFJet40_v=0;
  for(auto ij : jets){
    if(ij.Pt() > 50.) For_HLT_Mu3_PFJet40_v++;
  }

  TString ptname = "pt";
  TString PtHist = "pt"; /// Used to set binning
  
  float weight_ptcorr=event_weight;
  float lep_pt (0);
  if(Method == "Pt"){
    lep_pt      =  (lep->Pt() < 80) ?  lep->Pt() : 79;
    if(lep_pt > 20){
      if(!pass_17) return;
    }
    else  if(lep_pt > 10){
      if(!pass_8) return;
    }
    else  if(lep_pt > 5){
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
    }
  }
  if(Method == "PtCone"){
    lep_pt      =  ( lep->CalcPtCone(lep->RelIso(), isocut) < 80) ?  lep->Pt() : 79;
    ptname = "ptcone";
    if(lep_pt > 30){
      if(lep->Pt() < 20) return;
      if(!pass_17) return;
    }
    else if(lep_pt > 15){
      if(lep->Pt() < 10) return;
      if(!pass_8) return;
    }
    else if(lep_pt > 5){
      if(lep->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;

    }
    else return ;
  }
  else if(Method == "PtParton"){
    
    ptname = "ptparton";
    double PTPartonSF = GetPtPartonSF(*lep, LooseID);
    
    //cout << param.Name << "  PTPartonSF = " << PTPartonSF << " MVACut = " << MVACut << endl;
    
    lep_pt      =  (lep->PtParton(PTPartonSF,MVACut) < 80) ?  lep->PtParton(PTPartonSF, MVACut) : 79;
    
    FillProf((param.Name + "_Fake"+tag+"_MVA_PtParton").Data(), lep->HNL_MVA_Fake(), lep_pt, event_weight, 100, -1, 1);
    FillProf((param.Name + "_Fake"+tag+"_MVA_PtParton_"+lep->MotherJetFlavour() ).Data(), lep->HNL_MVA_Fake(), lep_pt, event_weight, 100, -1, 1);
    
    if(lep_pt > 30){
      if(lep->Pt() < 20) return;
      if(!pass_17) return;
    }
    else if(lep_pt > 15){
      if(lep->Pt() < 10) return;
      if(!pass_8) return;
    }
    else if(lep_pt > 5){
      if(lep->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;

    }
   else return ;
    
  }
  else if(Method == "PtParton20"){

    ptname = "ptparton";
    double PTPartonSF = GetPtPartonSF(*lep, LooseID);

    lep_pt      =  (lep->PtParton(PTPartonSF,MVACut,2) < 80) ?  lep->PtParton(PTPartonSF, MVACut,2) : 79;

    FillProf((param.Name + "_Fake"+tag+"_MVA_PtParton").Data(), lep->HNL_MVA_Fake(), lep_pt, event_weight, 100, -1, 1);
    FillProf((param.Name + "_Fake"+tag+"_MVA_PtParton_"+lep->MotherJetFlavour() ).Data(), lep->HNL_MVA_Fake(), lep_pt, event_weight, 100, -1, 1);

    if(lep_pt > 40){
      if(lep->Pt() < 20) return;
      if(!pass_17) return;
    }
    else if(lep_pt > 20){
      if(lep->Pt() < 10) return;
      if(!pass_8) return;
    }
    else if(lep_pt > 10){
      if(lep->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;

    }
    else return ;

  }

  if(Method == "PtCorr"){

    ptname = "ptcorr";
    lep_pt      = (lep->CalcMVACone( MVACut)  < 80) ? lep->CalcMVACone(MVACut)  : 79;
    
    if(lep_pt > 30){
      if(lep->Pt() < 20) return;
      if(!pass_17) return;
    }
    else if(lep_pt > 15){
      if(lep->Pt() < 10) return;
      if(!pass_8) return;
    }
    else if(lep_pt > 5){
      if(lep->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
    }
    else return ;
  }
  if(Method == "PtCorr20"){

    ptname = "ptcorr20";
    lep_pt      = (lep->CalcMVACone( MVACut,2)  < 80) ? lep->CalcMVACone(MVACut,2)  : 79;

    if(lep_pt > 40){
      if(lep->Pt() < 20) return;
      if(!pass_17) return;
    }
    else if(lep_pt > 20){
      if(lep->Pt() < 10) return;
      if(!pass_8) return;
    }
    else if(lep_pt > 10){
      if(lep->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
    }
    else return ;
  }


  if(Method == "MotherPt"){

    ptname = "MJpt";
    lep_pt      = lep->MotherJetPt();
    
    if(lep_pt > 30){
      if(lep->Pt() < 20) return;
      if(!pass_17) return;
    }
    else if(lep_pt > 15){
      if(lep->Pt() < 10) return;
      if(!pass_8) return;
    }
    else if(lep_pt > 5){
      if(lep->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
    }
    else return ;
  }

  float lep_eta     = lep->fEta(); /// returns |eta| OR for el |scEta|
  float lep_mva_lfvshf  = lep->HNL_MVA_Fake("QCD_LFvsHF_v5");
  float lep_mva_bvsc    = lep->HNL_MVA_Fake("QCD_BvsC_v5");
  float lep_blscore     = lep->CloseJet_BScore();
  float lep_cbscore     = lep->CloseJet_CvsBScore();
  float lep_clscore     = lep->CloseJet_CvsLScore();
  TString lepEtaRegion  = lep->etaRegionString();
  TString lepRegion     = (lep->IsBB()) ? "BB" : "EC";

  TString L_prefix = "Fake_"+Method+"_Loose"+tag;
  TString T_prefix = "Fake_"+Method+"_Tight"+tag;

  for(int ilep = 0 ; ilep < 2; ilep++)  {

    TString prefix = (ilep==0) ? L_prefix : T_prefix;

    if((ilep==1) && !(blepsT)) continue;
    
    prefix = param.Name + "/"+Method + "/"+prefix;

    TString Ptlab = "p_{T} (GeV)";

    if(lep_pt >10){
      FillHistogram((prefix + "_"+ptname+"_eta").Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_FinerBins_"+ptname+"_eta").Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+lep->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      
      FillHistogram((prefix + "_"+ptname).Data(),                 lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_"+ptname+"_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_eta").Data(),                lep_eta, weight_ptcorr ,"FR_Eta","#eta");
      FillHist((prefix + "_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      FillHist((prefix + "_lep_mva_lfvshf").Data(),   lep_mva_lfvshf,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_lep_mva_bvsc").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_blscore").Data(), lep_blscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_cbscore").Data(), lep_cbscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_clscore").Data(), lep_clscore,    weight_ptcorr, 50, -1., 1.);


      TString BTaggerLabel = "";
      if(lep_blscore > 0.9) BTaggerLabel = "BJet1";
      else  if(lep_blscore > 0.1) BTaggerLabel = "BJet2";
      else  BTaggerLabel = "BJet3";

      if(BTaggerLabel == "BJet3") FillHistogram((prefix + "_"+BTaggerLabel+"_"+ptname+"_eta").Data(),          lep_pt, lep_eta,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      else FillHistogram((prefix + "_"+BTaggerLabel+"_"+ptname+"_eta").Data(),          lep_pt, lep_eta,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+PtHist , "FR_Eta2", Ptlab);
      
      FillHistogram((prefix + "_"+BTaggerLabel+"_"+ptname).Data(),                 lep_pt,  weight_ptcorr, "FR_"+lep->GetFlavour()+"_"+PtHist, Ptlab);
      
    }
  }
  return;
}


HNL_Lepton_FakeRate_MC::HNL_Lepton_FakeRate_MC(){
  
}
 
HNL_Lepton_FakeRate_MC::~HNL_Lepton_FakeRate_MC(){


}


void HNL_Lepton_FakeRate_MC::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){


  return;

}


void HNL_Lepton_FakeRate_MC::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

