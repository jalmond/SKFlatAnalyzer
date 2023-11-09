#include "HNL_LeptonFakeRateMC.h"

void HNL_LeptonFakeRateMC::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

  TString AnalyzerPath=getenv("SKFlat_WD"), SKFlatV = getenv("SKFlatV");
  TString FilePath="/data/"+SKFlatV+"/"+GetEra()+"/FakeRate/NvtxSF/";
  NvtxSFFile = new TFile(AnalyzerPath+FilePath+"FakeRateNVtx13TeV_"+GetEra()+".root");

  //// Flags available in this code
  /// HasFlag("GetNvtxSF")

  /*
    Method 1
    
   */
}

void HNL_LeptonFakeRateMC::executeEvent(){

  Event ev = GetEvent();

  //************************************************///
  // setup list of IDs and jobs

  vector<AnalyzerParameter> VParameters;
  
  if(HasFlag("CheckProfile")){
    //// Check MVA Profile  for PtParton
    vector<TString> LIDs = {"HNL_ULID_FO",
			    "HNL_ULID_POGM_FO" ,
			    "HNL_ULID_FOv2_"+GetYearString() , 
			    "HNL_ULID_FOv3_"+GetYearString(), 
			    "HNL_ULID_FOv4", 
			    "HNL_ULID_FOv5", 
			    "HNL_ULID_FOv6_"+GetYearString()};

    vector<TString> NIDs = {"HNL_LooseID_",
			    "HNL_LooseID_POGM_",
			    "HNL_LooseID_v2_", 
			    "HNL_LooseID_v3_", 
			    "HNL_LooseID_v4_",
			    "HNL_LooseID_v5_",
			    "HNL_LooseID_v6_"};
    

    for(unsigned int l=0 ; l < LIDs.size(); l++)   VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu, HNL_LeptonCore::NormToXsec, {"DATAProfile"},NIDs[l]+"_DATA","HNL_ULID_"+GetYearString(),LIDs[l]));  
    
    goto RunJobs;
    
  }
  if(HasFlag("CheckProfileEE")){
    /// Check MVA/Pt Profile for ptparton in Electrons
    vector<TString> LIDs = {"HNL_ULID_FO_"+GetYearString() ,
			    "HNL_ULID_FOv2_"+GetYearString() ,
			    "HNL_ULID_FOv3_"+GetYearString(), 
			    "HNL_ULID_FOv4_"+GetYearString(), 
			    "HNL_ULID_FOv5_"+GetYearString(), 
			    "HNL_ULID_FOv6_"+GetYearString()};
    vector<TString> NIDs = {"HNL_LooseID_", 
			    "HNL_LooseID_v2_", 
			    "HNL_LooseID_v3_", 
			    "HNL_LooseID_v4_",
			    "HNL_LooseID_v5_",
			    "HNL_LooseID_v6_"};
    for(unsigned int l=0 ; l < LIDs.size(); l++)  VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE, HNL_LeptonCore::NormToXsec, {"DATAProfile"},NIDs[l]+"DATA","HNL_ULID_"+GetYearString(),LIDs[l]));
    
    goto RunJobs;
  }

  if(HasFlag("MCFakes")) {
    //// Measure Fake Rates in MC
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NoNorm,  {"MCFakes"},"HNL_ID"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO"));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,  {"MCFakes"},"HNL_ID_Weighted"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO"));
    //VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,  HNL_LeptonCore::NoNorm,  {"MCFakes"},"HNL_ID"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_"+GetYearString()));
    goto RunJobs;
  }

  
 RunJobs:
  
  for(auto param : VParameters)  executeEventFromParameter(param);
  
}

void HNL_LeptonFakeRateMC::executeEventFromParameter(AnalyzerParameter param){
   
  //************************************************///
  // setup event level objects
  Event ev = GetEvent();

  double weight = SetupWeight(ev, param);
  if(IsData) weight = 1;

  if(!PassMETFilter()) return;
  
  std::vector<Electron> Initial_loose_electrons     = SelectElectrons( param,param.Electron_Loose_ID, 10, 2.5) ;
  std::vector<Muon>     Initial_loose_muons         = SelectMuons    ( param,param.Muon_Loose_ID,     5,  2.4);

  std::vector<Electron> Loose_Electrons;
  std::vector<Muon>     Loose_Muons;
  if(param.HasFlag("MCFakes") || param.HasFlag("MCProfile")) {
    for(auto ilep : Initial_loose_electrons) if(ilep.IsFake()) Loose_Electrons.push_back(ilep);
    for(auto ilep : Initial_loose_muons)     if(ilep.IsFake()) Loose_Muons.push_back(ilep);
  }
  else{
    for(auto ilep : Initial_loose_electrons) Loose_Electrons.push_back(ilep);
    for(auto ilep : Initial_loose_muons)   Loose_Muons.push_back(ilep);
  }

  std::vector<Jet> jets_tmp     = SelectJets   ( param, "tight", 30., 2.7);
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


void HNL_LeptonFakeRateMC::RunM(std::vector<Electron> loose_el,  std::vector<Muon> loose_mu, std::vector<Jet> jets,   AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleEG") return;
    if(this->DataStream == "SingleElectron") return;
    if(this->DataStream == "EGamma") return;
  }

  Event ev = GetEvent();

  if(loose_mu.size() == 0) return;

  // remove if muon                                                                                                                          
  if(loose_el.size() > 0) return;

  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_mu,param);
  
  std::vector<bool>    blepsT;
  for(auto ilep : loose_mu)   blepsT.push_back(ilep.PassID(param.Muon_Tight_ID));
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  if(param.HasFlag("MCFakes")) {
    
    std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param);
    std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);

    for(unsigned int i = 0 ; i < loose_mu.size() ; i++){
      
      TString lep_fake_tag = MatchGenDef(All_Gens, Lepton(loose_mu[i]));
     
      double PtCorr       =  (loose_mu[i].CalcMVACone(loose_mu[i].MVAFakeCut(param.Muon_Tight_ID,GetYearString()))  < 80) ? loose_mu[i].CalcMVACone( loose_mu[i].MVAFakeCut(param.Muon_Tight_ID,GetYearString()))  : 79;
      double lep_ptparton  =  (loose_mu[i].PtParton(0.66,loose_mu[i].MVAFakeCut(param.Muon_Tight_ID,GetYearString())));
      
      TString DirInc = "MCFakeOpt/Loose/";
      TString DirTInc = "MCFakeOpt/Tight/";

      TString PTPatBin = "Pt1";
      if(lep_ptparton < 20)  PTPatBin = "Pt1";
      else       if(lep_ptparton <40)  PTPatBin = "Pt2";
      else PTPatBin = "Pt3";


      vector<TString> lables = GetGenList();
      if (std::find(lables.begin(), lables.end(), lep_fake_tag) == lables.end()) return;
      FillCutflow(param, "Loose_Lep_Fake_type_"+loose_mu[i].sRegion()+"_"+PTPatBin, event_weight, lables,lep_fake_tag);
      if(loose_mu[i].PassID(param.Muon_Tight_ID))      FillCutflow(param, "Tight_Lep_Fake_type_"+loose_mu[i].sRegion()+"_"+PTPatBin, event_weight, lables,lep_fake_tag);


      FillHist((DirInc+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
      FillHist((DirInc+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
      FillHist((DirInc+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
      FillHist((DirInc+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
      FillHist((DirInc+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
      FillHist((DirInc+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
      FillHist((DirInc+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
      FillHist((DirInc+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );

      FillHistogram(DirInc+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
      FillHistogram(DirInc+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
      FillHistogram(DirInc+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
      FillHistogram(DirInc+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;



      if(loose_mu[i].PassID(param.Muon_Tight_ID)){

	FillHist((DirTInc+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
	FillHist((DirTInc+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
	FillHist((DirTInc+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
	FillHist((DirTInc+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
	FillHist((DirTInc+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
	FillHist((DirTInc+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
	FillHist((DirTInc+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	FillHist((DirTInc+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );

	FillHistogram(DirTInc+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	FillHistogram(DirTInc+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
	FillHistogram(DirTInc+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	FillHistogram(DirTInc+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
      }

      if(AK4_BJetColl.size() > 0){
	TString DirB = "MCFakeOptB/Loose/";
	TString DirTB = "MCFakeOptB/Tight/";

	FillHist((DirB+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
	FillHist((DirB+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
	FillHist((DirB+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
	FillHist((DirB+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
	FillHist((DirB+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
	FillHist((DirB+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
	FillHist((DirB+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	FillHist((DirB+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );

	FillHistogram(DirB+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	FillHistogram(DirB+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
	FillHistogram(DirB+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	FillHistogram(DirB+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;

	if(loose_mu[i].PassID(param.Muon_Tight_ID)){
	  FillHist((DirTB+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
	  FillHist((DirTB+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
	  FillHist((DirTB+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
	  FillHist((DirTB+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
	  FillHist((DirTB+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
	  FillHist((DirTB+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
	  FillHist((DirTB+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	  FillHist((DirTB+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );
	  FillHistogram(DirTB+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	  FillHistogram(DirTB+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
	  FillHistogram(DirTB+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	  FillHistogram(DirTB+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
	}
      }      
      if(AK4_JetColl.size()==0){
	TString Dir0J = "MCFakeOpt0J/Loose/";
        TString DirT0J = "MCFakeOpt0J/Tight/";

        FillHistogram(Dir0J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	FillHistogram(Dir0J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
        FillHistogram(Dir0J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
	FillHistogram(Dir0J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
	FillHist((Dir0J+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
        FillHist((Dir0J+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
        FillHist((Dir0J+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
        FillHist((Dir0J+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
        FillHist((Dir0J+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
        FillHist((Dir0J+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
        FillHist((Dir0J+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
        FillHist((Dir0J+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );

	if(loose_mu[i].PassID(param.Muon_Tight_ID)){
          FillHistogram(DirT0J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
          FillHistogram(DirT0J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
          FillHistogram(DirT0J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
          FillHistogram(DirT0J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;

	  FillHist((DirT0J+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
	  FillHist((DirT0J+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
	  FillHist((DirT0J+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
	  FillHist((DirT0J+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
	  FillHist((DirT0J+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
	  FillHist((DirT0J+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
	  FillHist((DirT0J+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
	  FillHist((DirT0J+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );

        }	
      }
      if(AK4_JetColl.size()>1){
	TString Dir2J = "MCFakeOpt2J/Loose/";
	TString DirT2J = "MCFakeOpt2J/Tight/";

        FillHistogram(Dir2J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
        FillHistogram(Dir2J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
        FillHistogram(Dir2J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
        FillHistogram(Dir2J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
        FillHist((Dir2J+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
        FillHist((Dir2J+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
        FillHist((Dir2J+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
        FillHist((Dir2J+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
        FillHist((Dir2J+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
        FillHist((Dir2J+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
        FillHist((Dir2J+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
        FillHist((Dir2J+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );

        if(loose_mu[i].PassID(param.Muon_Tight_ID)){
          FillHistogram(DirT2J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
          FillHistogram(DirT2J+lep_fake_tag+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;
          FillHistogram(DirT2J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtParton", lep_ptparton, event_weight,"Pt80") ;
          FillHistogram(DirT2J+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_PtCorr",   PtCorr, event_weight,"Pt80") ;

          FillHist((DirT2J+"MVA_"    +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(),loose_mu[i].HNL_MVA_Fake("HNL") ,  event_weight, 200, -1, 1 );
          FillHist((DirT2J+"QCD_LFvsHF_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_LFvsHF_v5") ,  event_weight, 200,-1, 1 );
          FillHist((DirT2J+"QCD_BvsC_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].HNL_MVA_Fake("QCD_BvsC_v5") ,  event_weight, 200, -1, 1 );
          FillHist((DirT2J+"BScore_" +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_BScore() ,  event_weight, 200, -1, 1 );
          FillHist((DirT2J+"CvsB_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsBScore() ,  event_weight, 200, -1, 1 );
          FillHist((DirT2J+"CvsL_"   +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_CvsLScore() ,  event_weight, 200, -1, 1);
          FillHist((DirT2J+"PtRatio_"+loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptratio() ,   event_weight, 200, 0, 5 );
          FillHist((DirT2J+"PtRel_"  +loose_mu[i].MotherJetFlavour()+"_"+loose_mu[i].sRegion()+"_"+PTPatBin+"_"+param.Name +"_Loose").Data(), loose_mu[i].CloseJet_Ptrel() ,     event_weight, 50, 0, 200  );

        }
      } 
    }
  }
  bool useevent40 = UseEvent(leps , jets, 40., METv, event_weight);
  
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetMuFakeRates("Pt", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetMuFakeRates("PtParton", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetMuFakeRates("MotherPt", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetMuFakeRates("PtCorr", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetMuFakeRates("PtCone", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  return;



  if(param.HasFlag("MCProfile")){

    for(auto ilep : leps){
      
      double PtPartonUncorr = ilep->PtParton(1,ilep->MVAFakeCut(param.Muon_Tight_ID,GetYearString()));
      double W = event_weight;
      
      FillHist(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_PtPartonUncorr").Data(),PtPartonUncorr, W, 100,0., 200.);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 100, -1, 1);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->etaRegionString()+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 100, -1, 1);

    }
    return;
  }
  
  return;

} /// end function                      





void HNL_LeptonFakeRateMC::RunE( std::vector<Electron> loose_el, std::vector<Muon> loose_mu, std::vector<Jet> jets, AnalyzerParameter param,  float event_weight){

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
  
  if(HasFlag("GetNvtxSF")){
    MakeNVertexDistPrescaledTrig(EE,param, ev, leps,blepsT,param.Name+param.Channel,event_weight);
    return;
  }

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
  
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetElFakeRates("Pt", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetElFakeRates("PtParton", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetElFakeRates("MotherPt", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetElFakeRates("PtCorr", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  for(unsigned int i = 0 ; i < leps.size() ; i++) GetElFakeRates("PtCone", {leps.at(i)},{blepsT.at(i)}, param, jets, "MCFake_J40" ,  event_weight);
  if(param.HasFlag("MCFakes"))  return;


  if(param.HasFlag("MCProfile")){
    
    for(auto ilep : leps){
      
      double PtPartonUncorr = ilep->PtParton(1,ilep->MVAFakeCut(param.Electron_Tight_ID,GetYearString()));
      double W = event_weight;
      
      FillHist(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_PtPartonUncorr").Data(),PtPartonUncorr, W, 100,0., 200.);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HNL"), PtPartonUncorr, W, 100, -1, 1);
      FillProf(("MCProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->etaRegionString()+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HNL"), PtPartonUncorr, W, 100, -1, 1);
    }
    return;
  }
  if(param.HasFlag("DATAProfile")){
    
    if(leps.size() != 1) return;
    
    double prescale_lep = GetPrescale(leps);
    if(prescale_lep == 0) return;
    event_weight*= prescale_lep;
    
    if(UseEvent(leps , jets, 40., METv, event_weight)){
      for(auto ilep : leps){
	double PtPartonUncorr = ilep->PtParton(1,ilep->MVAFakeCut(param.Electron_Tight_ID,GetYearString()));
	double W = event_weight;
	if(!IsData) {
	  if(!leps[0]->IsPrompt()) return ;
	  W=W* -1.;
	}
	FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA2_"+ilep->GetEtaRegion("2bin")+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("EDv5"), PtPartonUncorr, W, 200, -1, 1);
        FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_MVA_"+ilep->GetEtaRegion("2bin")+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("EDv5"), PtPartonUncorr, W, 40, -1, 1);
      }
    }
    return;
  }

  if(param.HasFlag("FR") || param.HasFlag("PR"))   GetFakeRateAndPromptRates(param, leps,blepsT,jets,param.Name+param.Channel,event_weight);   



  if(param.HasFlag("MakeDiLeptonPlots"))   MakeDiLepPlots(EE,param, ev, leps,blepsT,param.Name+param.Channel,event_weight);
  
  if(!param.HasFlag("MakeSingleLeptonPlots")) return;

  if(loose_el.size() != 1) return;

  bool has_away_jet=false;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(loose_el[0].Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      has_away_jet=true;
    }
  } 


  float prescale_trigger =  GetPrescale(leps);
  if(prescale_trigger == 0.) return;
  event_weight*=prescale_trigger;



  bool truth_match= false;
  if(!IsDATA) {
    if(loose_el.at(0).LeptonGenType() > 0 ) truth_match=true;
  }
  else truth_match=true;

  if(jets.size() > 0){
    if(jets.at(0).Pt() > 40.){
      FillRegionPlots("EE","SingleLooseElJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(has_away_jet)      FillRegionPlots("EE","SingleLooseElAwayJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(blepsT[0]){
        FillRegionPlots("EE","SingleTightElJet_"+param.Name,  jets,  loose_el,loose_mu,  METv, event_weight);
        if(has_away_jet) FillRegionPlots("EE","SingleTightElAwayJet_"+param.Name, jets,  loose_el, loose_mu,  METv, event_weight);
      }
    }
  }

  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int iel = 0; iel < loose_el.size();  iel++){
    METdphi = TVector2::Phi_mpi_pi((loose_el.at(iel).Phi()- METv.Phi()));
    MT = sqrt(2.* loose_el.at(iel).Et()*METv.Pt() * (1 - cos( METdphi)));
  }
  if((60. < MT)  &&(MT < 100.)){
    if(loose_el[0].Pt() > 25){
      if(jets.size() > 0 && blepsT[0]){
	if(jets[0].Pt() > 40){
	  FillRegionPlots("EE","SingleTightEl_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	  if(has_away_jet)	  FillRegionPlots("EE","SingleTightElAwayJet_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);

	  bool isPrompt=false;
          for(unsigned int ij=0; ij < jets.size(); ij++){
	    if(jets.at(ij).Pt() < 40.) continue;
            float dphi =fabs(TVector2::Phi_mpi_pi(loose_el[0].Phi()- jets.at(ij).Phi()));
            if(dphi > 2.5){
              if((jets.at(ij).Pt() /  loose_el[0].Pt() ) < 1.) isPrompt = true;
	    }
	  }
	  
          if (isPrompt ) FillRegionPlots("EE","SingleTightEl_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	  if (isPrompt && has_away_jet) FillRegionPlots("EE","SingleTightElAwayJet_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
	}
      }
    }
  }
  


  bool useevent40 = UseEvent(leps , jets, 40., METv,event_weight);
  if(useevent40){
    if (blepsT[0] && jets.size() >= 1)FillRegionPlots("EE","TightEl40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if (jets.size() >= 1)FillRegionPlots("EE","LooseEl40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    
    
    if(truth_match){
      if (blepsT[0] && jets.size() >= 1)FillRegionPlots("EE","TightEl40_"+param.Name , jets,  loose_el,loose_mu, METv, event_weight);
      if (jets.size() >= 1)FillRegionPlots("EE","LooseEl40_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    }
  }
  
  
  return;
  


} /// end function RunE

double HNL_LeptonFakeRateMC::ApplyNvtxReweight(int NPV, TString Key){

  if(IsData) return 1.;
  if(!NvtxSFFile->GetListOfKeys()->Contains(Key)){ printf("[Error] No %s in NvtxSF File.\n", Key.Data()); return -1.; }

  std::map<TString, TH1D*>::iterator mapit = maphist_NvtxSF.find(Key);
  if(mapit == maphist_NvtxSF.end()){ //first usage
    maphist_NvtxSF[Key] = (TH1D*) ((TH1*) NvtxSFFile->Get(Key))->Clone();
  }
  mapit = maphist_NvtxSF.find(Key);
  
  int this_bin = mapit->second->FindBin(NPV);
  double NvtxSF = mapit->second->GetBinContent(this_bin);
  if(NvtxSF>10) NvtxSF=1.;

  return NvtxSF;

}


void HNL_LeptonFakeRateMC::MakeDiLepPlots(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){

  double d_event_weight = event_weight;
  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;

  // now we have 2 Tight leptons                                                                                                                             

  TString plot_dir = GetChannelString(channel);

  // Make Z peak                                                                                                                                            
  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;
  bool PassZMass = (fabs(M_Z - Z.M()) < 10.) ? true : false;

  std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param); 
  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets(param.AK8JetColl,param);

  EvalJetWeight(AK4_JetColl, AK8_JetColl, d_event_weight, param);

  if(AK4_BJetColl.size() > 0) return;


  if(channel == MuMu){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    double NVxt_Mu3 =  ApplyNvtxReweight(nPV,triggerslist_3);
    double NVxt_Mu8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_Mu17 =  ApplyNvtxReweight(nPV,triggerslist_17);

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;


    if(pass_3){
      if(leps[1]->Pt() > 5){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3)*NVxt_Mu3;
	
        if(PassZMass) {
	  for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }
    
    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8;
	
        if(PassZMass){
	  for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_Mu17;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }
  
  if(channel == EE){

    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;
    double NVxt_El8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
    double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);


    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_El8;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_12){
      if(leps[1]->Pt() > 14){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }

    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_23){
      if(leps[1]->Pt() > 25){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }

  return;


}



void HNL_LeptonFakeRateMC::MakeNVertexDistPrescaledTrig(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){
  

  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;
  
  // now we have 2 Tight leptons
  // Make Z peak 

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;


  // remove b jet 

  std::vector<Jet> jets_tmp     = SelectJets   ( param, param.Jet_ID, 20., 5.);
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl    = SelectBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )event_weight*= sf_btag;

  if(BJetColl.size() > 0) return;

  bool PassZMass = (fabs(M_Z - Z.M()) < 10.) ? true : false;


  // Now plot Z peak for

  int nbin_npv    =35;
  double bins_npv[nbin_npv+1] = { 0.,5., 10.,12., 14., 16., 18., 20., 22., 24., 26., 28., 30., 32., 34., 36., 38., 40., 42., 44., 46., 48., 50., 52., 54., 56., 58., 60., 62., 64., 66., 68., 70., 75., 80., 100.};



  if(channel == MuMu){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;


    if(pass_3){
      if(leps[1]->Pt() > 5){
	double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3);

	if(PassZMass) {
	  FillHist(( triggerslist_3 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
	FillHist(( triggerslist_3 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_8){
      if(leps[1]->Pt() > 10){
	double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8);

	if(PassZMass){
	  FillHist(( triggerslist_8 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
        FillHist(( triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
	double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17);
	if(PassZMass){
	  FillHist(( triggerslist_17 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	} 
	FillHist(( triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }   
  }
  


  if(channel == EE){

    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;


    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8);
	if(PassZMass){
	  FillHist(( triggerslist_8 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
        FillHist(( triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_12){
      if(leps[1]->Pt() > 14){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12);
	if(PassZMass){
	  FillHist(( triggerslist_12 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
	FillHist(( triggerslist_12 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
	      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17);
	if(PassZMass){
	  FillHist(( triggerslist_17 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
	FillHist(( triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_23){
      if(leps[1]->Pt() > 25){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23);
	if(PassZMass){
	  FillHist(( triggerslist_23 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
	}
        FillHist(( triggerslist_23 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }
  
  return;
  

}
void HNL_LeptonFakeRateMC::GetFakeRateAndPromptRates(AnalyzerParameter param, std::vector<Lepton *> leps,std::vector<bool> blepsT, std::vector<Jet>    jetCollTight, TString label, float event_weight){
						      
  if (leps.size()<1) return;
  
  Event ev = GetEvent();
  Particle METv = GetvMET("PuppiT1xyULCorr",param);
  
  if(param.HasFlag("FR") && leps.size()==1)  MakeFakeRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, METv);

  if(leps.size()==2 && param.HasFlag("PR")){
    TString dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v";
    if(DataYear==2017) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";
    if(DataYear==2018) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";    
    if(IsDATA&& !(ev.IsPDForTrigger(dilep_triggerslist, this->DataStream) )) return;
    if(ev.PassTrigger(dilep_triggerslist))    MakePromptRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, METv);
  }
  return;
}


void HNL_LeptonFakeRateMC::MakePromptRatePlots(TString label, TString tag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT , std::vector<Jet> jets,  float event_weight, Particle MET){
  
  Event ev = GetEvent();

  if(leps.size() != 2) return;

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) event_weight = -1;

  if(fabs(M_Z - Z.M()) > 10.) return;

  TString L_prefix = "PrompRate/Prompt_Loose"+tag ;
  TString T_prefix = "PrompRate/Prompt_Tight"+tag;
  float weight_ptcorr=event_weight;

  if(blepsT[1]){
    // lep [1] is tag
    int ilep=0;
    float lep_pt  = leps[ilep]->UncorrectedPt();
    float lep_eta = fabs(leps[ilep]->Eta());
    if(lep_pt > 500.) lep_pt = 499.;

    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHistogram((prefix + "_pt_eta").Data(),      lep_pt,         lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
      if(fabs(M_Z - Z.M()) < 5)	FillHistogram((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
    }
  }
  if(blepsT[0]){
    // lep [0] is tag                                                                                                                                                             
    int ilep=1;
    float lep_pt = leps[ilep]->UncorrectedPt();
    float lep_eta = fabs(leps[ilep]->Eta());

    if(lep_pt > 500.) lep_pt = 499.;

    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHistogram((prefix + "_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
      if(fabs(M_Z - Z.M()) < 5) 	FillHistogram((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
    }
  }

  return;
}


float HNL_LeptonFakeRateMC::GetPrescale(std::vector<Lepton *>   leps  ){

  if(leps.size() != 1) return 0.;

  Event ev = GetEvent();
  float prescale_trigger= 0;
  if(leps[0]->LeptonFlavour() == Lepton::MUON){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;
  
    double NVxt_Mu3 =  ApplyNvtxReweight(nPV,triggerslist_3);
    double NVxt_Mu8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_Mu17 =  ApplyNvtxReweight(nPV,triggerslist_17);

    /// 10 - 20  HLT_Mu7
    /// 20 - INF  HLT_Mu17
    
    if(leps.at(0)->Pt() >= 20.){
      if(pass_17) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_Mu17;
      return prescale_trigger;
    }
    if(leps.at(0)->Pt() >= 10.){

      if(pass_8)prescale_trigger =  (IsDATA) ? 1. :   ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8 ; //// 20 + GeV bins
      return prescale_trigger;
    }
    if(leps.at(0)->Pt() >= 5.){
      if(pass_3)prescale_trigger =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3)*NVxt_Mu3 ; //// 20 + GeV bins                                                                             
      return prescale_trigger;
    }      
    
    return 0 ;
  }
  else{
    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";  
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;

    double NVxt_El8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
    double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);

 
    if(leps.at(0)->Pt() >= 25.){
      if(pass_23)prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
      return prescale_trigger;
    }
    else   if(leps.at(0)->Pt() >= 15.){
      if(pass_12) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
      return prescale_trigger;
    }
    else   if(leps.at(0)->Pt() >= 9.5){
      if(pass_8) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_El8;
      return prescale_trigger;
    }
    return 0 ;
  }
  
  return prescale_trigger;
}
 

void HNL_LeptonFakeRateMC::MakeFakeRatePlots(TString label, TString mutag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT ,  std::vector<Jet> jets,  float event_weight,  Particle MET){
				
  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT MUONS                                                                                                                                                  
  bool truth_match= true;

  if(!IsDATA && leps.size() > 0) truth_match = leps[0]->IsPrompt() ;

  if(param.HasFlag("MCFakeRates")) truth_match = leps[0]->IsFake();

  if(!truth_match) return;

  double AWJPt = -1;
  if(param.syst_ == AnalyzerParameter::Central) AWJPt = 40;
  if(param.syst_ == AnalyzerParameter::FRAJ30)  AWJPt = 30;
  if(param.syst_ == AnalyzerParameter::FRAJ50)  AWJPt = 50;
  if(param.syst_ == AnalyzerParameter::FRAJ60)  AWJPt = 60;
  
  
  bool useevent = UseEvent(leps , jets,  AWJPt,  MET, event_weight);

  label= mutag;
  if(jets.size() >= 1){
    if(useevent){
      if(param.Channel == "MuMu"){
	GetMuFakeRates("Pt",      leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	GetMuFakeRates("PtCone",  leps, blepsT, param,  jets,    label+"_"+param.GetSystType(),(event_weight));
	GetMuFakeRates("PtCorr",  leps, blepsT, param,  jets,    label+"_"+param.GetSystType(),(event_weight));
	GetMuFakeRates("PtParton",leps, blepsT, param,  jets,  label+"_"+param.GetSystType(),(event_weight));
	GetMuFakeRates("MotherPt",leps, blepsT, param,  jets,  label+"_"+param.GetSystType(),(event_weight));
      }
      else{
	GetElFakeRates("Pt",leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
        GetElFakeRates("PtCone",leps, blepsT, param,  jets,    label+"_"+param.GetSystType(),(event_weight));
        GetElFakeRates("PtCorr",leps, blepsT, param,  jets,    label+"_"+param.GetSystType(),(event_weight));
        GetElFakeRates("PtParton",leps, blepsT, param,  jets,  label+"_"+param.GetSystType(),(event_weight));
        GetElFakeRates("MotherPt",leps, blepsT, param,  jets,  label+"_"+param.GetSystType(),(event_weight));
      }
    }
  }
  return;
}


bool HNL_LeptonFakeRateMC::UseEvent(std::vector<Lepton *> leps ,  std::vector< Jet> jets, float awayjetcut, Particle MET, float wt){

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


void HNL_LeptonFakeRateMC::GetElFakeRates(TString Method, std::vector<Lepton *> leps,std::vector<bool> blepsT,  AnalyzerParameter param,  std::vector<Jet> jets,  TString tag,float event_weight){

  bool IsMuon=(leps[0]->LeptonFlavour() == Lepton::MUON);
  if(IsMuon) return;

  if(param.Name.Contains("HNL_")){
    if(Method.Contains("PtCone")) return;
  }
  else{
    if(!Method.Contains("PtCone")) return;
  }

  double   isocut  = (Method == "PtCone") ? GetIsoFromID(*leps[0], param.Electron_Tight_ID) : 0. ;
  TString  LooseID =  param.Electron_Loose_ID ;
  TString  TightID =  param.Electron_Tight_ID ;
  double MVACut = leps[0]->MVAFakeCut(TightID,GetYearString());


  Event ev = GetEvent();
  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";

  bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
  bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
  bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

  bool pass_8  = ev.PassTrigger(triggerslist_8) && El8PD;
  bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
  bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;
  double NVxt_El8  =  ApplyNvtxReweight(nPV,triggerslist_8);
  double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
  double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);

  int For_HLT_PFJet30_v=0;
  for(auto ij : jets){
    if(ij.Pt() > 40.) For_HLT_PFJet30_v++;
  }
  if(!For_HLT_PFJet30_v) return;

  float lep_pt (0);
  
  float weight_ptcorr=event_weight;
  TString MVAKey = "EDv5";
  TString ptname = "pt_eta";
  TString PtHist = "pt"; /// Used to set binning                                                                                                                                                                  

  if(Method == "Pt"){
    lep_pt      =  (leps[0]->Pt() < 80) ?  leps[0]->Pt() : 79;

    if(lep_pt > 25){
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *= (ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else  if(lep_pt > 15){
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);      
    }
    else  if(lep_pt > 10){
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);
    }
  }
  if(Method == "PtCone"){
    PtHist ="ptcone";
    lep_pt =  ( leps[0]->CalcPtCone(leps[0]->RelIso(), isocut) < 80) ?  leps[0]->Pt() : 79;
    ptname = "ptcone_eta";
    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 15) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 9.5) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);

    }
    else return ;
  }
  else if(Method == "PtParton"){
    PtHist ="ptcone";
    ptname = "ptparton_eta";
    double PTPartonSF = GetPtPartonSF(*leps[0], LooseID);

    lep_pt      =  (leps[0]->PtParton(PTPartonSF,MVACut) < 80) ?  leps[0]->PtParton(PTPartonSF, MVACut) : 79;

    FillProf((param.Name + "_FakeCR40_MVA_PtParton").Data(), leps[0]->HNL_MVA_Fake(MVAKey), lep_pt, event_weight, 100, -1, 1);

    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 10) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 9.5) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);

    }
    else return ;
    
  }
  if(Method == "PtCorr"){

    PtHist ="ptcone";
    ptname = "ptcorr_eta";
    lep_pt      = (leps[0]->CalcMVACone(MVACut)  < 80) ? leps[0]->CalcMVACone(MVACut)  : 79;
    

    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 10) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 9.5) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);

    }
    else return ;

  }
  if(Method == "MotherPt"){
    PtHist ="ptcone";
    ptname = "MJpt_eta";
    lep_pt      = leps[0]->MotherJetPt();


    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 10) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 9.5) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);

    }
    else return ;

  }

  float lep_eta     = leps[0]->fEta(); /// returns |eta| OR for el |scEta|
  float lep_mva_lfvshf  = leps[0]->HNL_MVA_Fake("QCD_LFvsHF_v5");
  float lep_mva_bvsc    = leps[0]->HNL_MVA_Fake("QCD_BvsC_v5");
  float lep_blscore     = leps[0]->CloseJet_BScore();
  float lep_cbscore     = leps[0]->CloseJet_CvsBScore();
  float lep_clscore     = leps[0]->CloseJet_CvsLScore();
  TString lepEtaRegion  = leps[0]->etaRegionString();
  TString lepRegion     = (leps[0]->IsBB()) ? "BB" : "EC";

  TString L_prefix = "Fake_"+Method+"_Loose"+tag;
  TString T_prefix = "Fake_"+Method+"_Tight"+tag;

  for(int ilep = 0 ; ilep < 2; ilep++)  {

    TString prefix = (ilep==0) ? L_prefix : T_prefix;
    if((ilep==1) && !(blepsT[0])) continue;
    
    prefix = Method + "/"+prefix;

    TString Ptlab = "p_{T} (GeV)";

    if(lep_pt >10){
      FillHistogram((prefix + "_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
      FillHistogram((prefix + "_FinerBins_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
      FillHistogram((prefix + "_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_eta").Data(),                lep_eta, weight_ptcorr ,"FR_eta","#eta");
      FillHist((prefix + "_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      FillHist((prefix + "_lep_mva_lfvshf").Data(),   lep_mva_lfvshf,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_lep_mva_bvsc").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_blscore").Data(), lep_blscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_cbscore").Data(), lep_cbscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_clscore").Data(), lep_clscore,    weight_ptcorr, 50, -1., 1.);

      if(lep_mva_lfvshf > 0.8){
	FillHistogram((prefix + "_LF_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
        FillHistogram((prefix + "_LF_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
	FillHistogram((prefix + "_LF_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	FillHistogram((prefix + "_LF_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	FillHistogram((prefix + "_LF_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	FillHistogram((prefix + "_LF_eta").Data(),                lep_eta, weight_ptcorr ,"FR_eta","#eta");
	FillHist((prefix + "_LF_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      }
      else{
	if(lep_mva_bvsc > 0.7){
	  FillHistogram((prefix + "_HF1_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_HF1_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_HF1_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	  FillHistogram((prefix + "_HF1_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	  FillHistogram((prefix + "_HF1_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	  FillHistogram((prefix + "_HF1_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
	  FillHist((prefix + "_HF1_eta_fine").Data(),    lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
	}
	else if(lep_mva_bvsc > -0.4){

          FillHistogram((prefix + "_HF2_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF2_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF2_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
          FillHistogram((prefix + "_HF2_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
          FillHistogram((prefix + "_HF2_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
          FillHistogram((prefix + "_HF2_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
          FillHist((prefix + "_HF2_eta_fine").Data(),    lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
	}
	else{
          FillHistogram((prefix + "_HF3_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF3_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_HF3_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	  FillHistogram((prefix + "_HF3_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	  FillHistogram((prefix + "_HF3_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
	  FillHistogram((prefix + "_HF3_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
	  FillHist((prefix + "_HF3_eta_fine").Data(),    lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
	}
      }
    }
  }
  return;

}

void HNL_LeptonFakeRateMC::GetMuFakeRates(TString Method, std::vector<Lepton *> leps,std::vector<bool> blepsT,  AnalyzerParameter param,  std::vector<Jet> jets,  TString tag,float event_weight){
  

  bool IsMuon=(leps[0]->LeptonFlavour() == Lepton::MUON);
  if(!IsMuon) return;

  if(param.Name.Contains("HNL_")){
    if(Method.Contains("PtCone")) return;
  }
  else{
    if(!Method.Contains("PtCone")) return;
  }
  
  double   isocut  = (Method == "PtCone") ? GetIsoFromID(*leps[0], param.Muon_Tight_ID) : 0.;
  TString  LooseID =  param.Muon_Loose_ID ;
  TString  TightID =  param.Muon_Tight_ID ;
  double MVACut = leps[0]->MVAFakeCut(TightID,GetYearString());
  
  ///// TRIGGER CONDITIONS

  Event ev = GetEvent();  

  TString triggerslist_3="HLT_Mu3_PFJet40_v";  TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";  TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";
  bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
  bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
  bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

  bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
  bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
  bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;

  int For_HLT_Mu3_PFJet40_v=0;
  for(auto ij : jets){
    if(ij.Pt() > 50.) For_HLT_Mu3_PFJet40_v++;
  }

  TString MVAKey = "HFTop";
  TString ptname = "pt_eta";
  TString PtHist = "pt"; /// Used to set binning
  
  float weight_ptcorr=event_weight;
  float lep_pt (0);
  if(Method == "Pt"){
    lep_pt      =  (leps[0]->Pt() < 80) ?  leps[0]->Pt() : 79;
    if(lep_pt > 20){
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *= (ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else  if(lep_pt > 10){
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));      
    }
    else  if(lep_pt > 5){
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));
    }
  }
  if(Method == "PtCone"){
    lep_pt      =  ( leps[0]->CalcPtCone(leps[0]->RelIso(), isocut) < 80) ?  leps[0]->Pt() : 79;
    ptname = "ptcone_eta";
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 5){
      if(leps[0]->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));

    }
    else return ;
  }
  else if(Method == "PtParton"){
    
    ptname = "ptparton_eta";
    double PTPartonSF = GetPtPartonSF(*leps[0], LooseID);
    
    lep_pt      =  (leps[0]->PtParton(PTPartonSF,MVACut) < 80) ?  leps[0]->PtParton(PTPartonSF, MVACut) : 79;
    
    FillProf((param.Name + "_FakeCR40_MVA_PtParton").Data(), leps[0]->HNL_MVA_Fake(MVAKey), lep_pt, event_weight, 100, -1, 1);
    
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 5){
      if(leps[0]->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));

    }
   else return ;
    
  }
  if(Method == "PtCorr"){

    ptname = "ptcorr_eta";
    lep_pt      = (leps[0]->CalcMVACone( MVACut)  < 80) ? leps[0]->CalcMVACone(MVACut)  : 79;
    
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 5){
      if(leps[0]->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));
    }
    else return ;
  }
  if(Method == "MotherPt"){

    ptname = "MJpt_eta";
    lep_pt      = leps[0]->MotherJetPt();
    
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 5){
      if(leps[0]->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));
    }
    else return ;
  }

  float lep_eta     = leps[0]->fEta(); /// returns |eta| OR for el |scEta|
  float lep_mva_lfvshf  = leps[0]->HNL_MVA_Fake("QCD_LFvsHF_v5");
  float lep_mva_bvsc    = leps[0]->HNL_MVA_Fake("QCD_BvsC_v5");
  float lep_blscore     = leps[0]->CloseJet_BScore();
  float lep_cbscore     = leps[0]->CloseJet_CvsBScore();
  float lep_clscore     = leps[0]->CloseJet_CvsLScore();
  TString lepEtaRegion  = leps[0]->etaRegionString();
  TString lepRegion     = (leps[0]->IsBB()) ? "BB" : "EC";

  TString L_prefix = "Fake_"+Method+"_Loose"+tag;
  TString T_prefix = "Fake_"+Method+"_Tight"+tag;

  for(int ilep = 0 ; ilep < 2; ilep++)  {

    TString prefix = (ilep==0) ? L_prefix : T_prefix;
    if((ilep==1) && !(blepsT[0])) continue;
    
    prefix = Method + "/"+prefix;

    TString Ptlab = "p_{T} (GeV)";

    if(lep_pt >10){
      FillHistogram((prefix + "_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
      FillHistogram((prefix + "_FinerBins_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
      
      FillHistogram((prefix + "_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_eta").Data(),                lep_eta, weight_ptcorr ,"FR_eta","#eta");
      FillHist((prefix + "_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      FillHist((prefix + "_lep_mva_lfvshf").Data(),   lep_mva_lfvshf,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_lep_mva_bvsc").Data(),   lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_blscore").Data(), lep_blscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_cbscore").Data(), lep_cbscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_clscore").Data(), lep_clscore,    weight_ptcorr, 50, -1., 1.);

      if(lep_mva_lfvshf > 0.8){
	FillHistogram((prefix + "_LF_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
	FillHistogram((prefix + "_LF_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
	FillHistogram((prefix + "_LF_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	FillHistogram((prefix + "_LF_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	FillHistogram((prefix + "_LF_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	FillHistogram((prefix + "_LF_eta").Data(),                lep_eta, weight_ptcorr ,"FR_eta","#eta");
	FillHist((prefix + "_LF_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      }
      else{
	if(lep_mva_lfvshf > 0.7){
	  FillHistogram((prefix + "_LF2_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_LF2_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_LF2_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_LF2_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_LF2_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_LF2_eta").Data(),                lep_eta, weight_ptcorr ,"FR_eta","#eta");
	  FillHist((prefix + "_LF2_eta_fine").Data(),           lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
	}
	if(lep_mva_bvsc > 0.7){
	  FillHistogram((prefix + "_HF1_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_HF1_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_HF1_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_HF1_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_HF1_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_HF1_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
	  FillHist((prefix + "_HF1_eta_fine").Data(),    lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
	}
	else if(lep_mva_bvsc > 0.3){

          FillHistogram((prefix + "_HF2_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF2_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF2_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF2_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF2_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF2_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
          FillHist((prefix + "_HF2_eta_fine").Data(),    lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
        }
	else if(lep_mva_bvsc > 0.1){

          FillHistogram((prefix + "_HF3_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF3_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF3_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF3_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF3_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF3_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
          FillHist((prefix + "_HF3_eta_fine").Data(),    lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
        }
	else if(lep_mva_bvsc > -0.1){

          FillHistogram((prefix + "_HF4_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF4_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF4_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF4_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF4_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF4_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
        }
	else if(lep_mva_bvsc > -0.4){

          FillHistogram((prefix + "_HF5_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF5_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF5_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF5_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF5_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
          FillHistogram((prefix + "_HF5_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
	}
	else{
          FillHistogram((prefix + "_HF6_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
          FillHistogram((prefix + "_HF6_FinerBins_"+ptname).Data(),             lep_pt, lep_eta,  weight_ptcorr, "FR_FB_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_eta", Ptlab);
	  FillHistogram((prefix + "_HF6_pt").Data(),                 lep_pt,   weight_ptcorr,        "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_HF6_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_HF6_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
	  FillHistogram((prefix + "_HF6_eta").Data(),    lep_eta,             weight_ptcorr ,"FR_eta","#eta");
	}
      }
    }
  }
  return;
}


HNL_LeptonFakeRateMC::HNL_LeptonFakeRateMC(){
  
}
 
HNL_LeptonFakeRateMC::~HNL_LeptonFakeRateMC(){

  //  delete NvtxSFFile;
  //for(std::map< TString, TH1D* >::iterator mapit = maphist_NvtxSF.begin(); mapit!=maphist_NvtxSF.end(); mapit++){
  //  delete mapit->second;
  // }
  //maphist_NvtxSF.clear();

}

void HNL_LeptonFakeRateMC::FillRegionPlots( TString plot_dir, TString region,  std::vector<Jet> jets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met,  double w){
  
  if((els.size()+ mus.size()) != 1) return;
  Lepton lep1;
  if(els.size()==1) lep1=els[0];
  else lep1 = mus[0];

  FillHistogram( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_binned_pt", lep1.Pt()  , w, "FR_pt","p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_pt", lep1.Pt()  , w, 400, 0., 2000.,"p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_eta", lep1.Eta()  , w, 30, -3., 3,"#eta");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_CloseJet_Ptrel", lep1.CloseJet_Ptrel(), w, 50., 0., 2.,"PtRel");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_CloseJet_Ptratio", lep1.CloseJet_Ptratio(), w, 50., 0., 2.,"PtRel");

  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Met", met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MetPhi", met.Phi()  , w, 100, -5, 5,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/njets" , jets.size() , w, 5, 0., 5., "N_{jets}");

  float lep_reliso  = lep1.RelIso();
  float lep_minireliso  = lep1.MiniRelIso();
  float lep_ip3d    = fabs(lep1.IP3D()/lep1.IP3Derr());
  float lep_mva     =  lep1.LepMVA();
  float lep_dxy     = fabs(lep1.dXY());

  if(els.size() > 0)   FillHist( plot_dir +  "/RegionPlots_"+ region+ "/NMissingHits", els[0].NMissingHits(), w, 5, 0., 5.);


  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Reliso", lep_reliso, w, 50, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MiniReliso", lep_minireliso, w, 50, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/dXY",    lep_dxy, w, 100, 0., 0.5);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/IP3D",   lep_ip3d, w, 50, 0., 10.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Mva",    lep_mva, w, 50, -1., 1.);



  double lep_jet_ratio= 0.;
  double jet_CEEF = 0.;  double jet_CHEF = 0.;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(lep1.Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      lep_jet_ratio = jets.at(ij).Pt() /  lep1.Pt();
      jet_CEEF = jets.at(ij).ChargedEmEnergyFraction();
      jet_CHEF = jets.at(ij).ChargedHadEnergyFraction();
    }
  }
  
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/JetLepPtRatio" , lep_jet_ratio  , w, 50, 0., 5.,"P^{jet}_{T} / P^{lepton}_{T}");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/AwayJetChargedEMEnergyFraction" , jet_CEEF  , w, 50, 0., 2.,"Jet charged EMF");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/AwayJetChargedHadEnergyFraction" , jet_CHEF  , w, 50, 0., 2.,"Jet charged EMF");
  


  double METdphi = TVector2::Phi_mpi_pi(lep1.Phi()-met.Phi());
  double MT = sqrt(2.* lep1.Et()*met.Pt() * (1 - cos( METdphi)));
  
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Mt" , MT  , w, 40, 0., 200.,"MET GeV");
  
}


void HNL_LeptonFakeRateMC::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){


  return;

}

void HNL_LeptonFakeRateMC::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNL_LeptonFakeRateMC::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

