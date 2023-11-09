#include "HNL_LeptonFakeStudy.h"

void HNL_LeptonFakeStudy::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();


}

void HNL_LeptonFakeStudy::executeEvent(){

  Event ev = GetEvent();

  AnalyzerParameter param = InitialiseHNLParameter("MCStudy");

  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ELIDs;
  vector<HNL_LeptonCore::Channel> channel;
  vector<TString> paramnames;
  
  TString           era="2016";
  if(DataYear==2017)era="2017";
  if(DataYear==2018)era="2018";
  
  param.Apply_Weight_LumiNorm=false;
  double weight =SetupWeight(ev,param);

  
  if(!PassMETFilter()) return;

  std::vector<Electron> loose_electrons     = SelectElectrons( param,"MVAID" , 10, 2.5) ;
  std::vector<Muon>     loose_muonsPOG      = SelectMuons    ( param,"HNLoosePOG", 10, 2.4);

  std::vector<Muon>     loose_muonsMVA      = SelectMuons    ( param,"MVAID",    10, 2.4);
  std::vector<Muon>     loose_muonsMVAT     = SelectMuons    ( param,"MVALoose", 10, 2.4);
  std::vector<Jet>      AK4_JetColl         = GetHNLJets(param.AK4JetColl,     param);


  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_muonsMVA);

  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  param.SplitPlot = "PtBin_Type_";

  //  FillMuonCollPlots(param, "HNL_MuonFakeStudy_LooseMVA",  loose_muonsMVA, param.Weight("Full") );

  bool useevent40 = UseEvent(leps , AK4_JetColl, 40.,  METv, weight);                                                                                                                                          
  
  for(auto imu : loose_muonsMVA){
    FillMuonPlots(useevent40, "40_Data" ,param, "FakeCR",  imu, param.Weight("Full") );
  }

  if(SameCharge(loose_muonsMVA)){
    
    TString JetLabel = "";
    if(AK4_JetColl.size() == 0)        JetLabel = "0j";
    else  if(AK4_JetColl.size() == 0)  JetLabel = "1j";
    else JetLabel = "2j";

    if(loose_muonsMVA[0].PassID(param.Muon_Tight_ID) && loose_muonsMVA[1].PassID(param.Muon_Tight_ID)) {
      FillHistogram(("Plots/"+JetLabel+"/TT/Pt"), loose_muonsMVA[1].Pt() , param.Weight("Full"), "Pt");
      goto Part1;
    }
    
    if(!loose_muonsMVA[0].PassID(param.Muon_Tight_ID) && !loose_muonsMVA[1].PassID(param.Muon_Tight_ID)) {
      FillHistogram(("Plots/"+JetLabel+"/LL/Pt"), loose_muonsMVA[1].Pt()  , param.Weight("Full"), "Pt");
      goto Part1;

    }
    if(loose_muonsMVA[0].PassID(param.Muon_Tight_ID)) FillHistogram(("Plots/"+JetLabel+"/TL/Pt"), loose_muonsMVA[1].Pt()  , param.Weight("Full"), "Pt");
    else FillHistogram(("Plots/"+JetLabel+"/LT/Pt"), loose_muonsMVA[1].Pt() , param.Weight("Full"), "Pt");


    /// Look at TL events
    for(auto imu : loose_muonsMVA){
      if(!IsData && !imu.IsFake()) continue;
      
      if(imu.PassID(param.Muon_Tight_ID))continue;
      
      FillHist("SS"+JetLabel+"_Flavour_cat/SS"+JetLabel+"Lep/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.80)FillHist("SS"+JetLabel+"_Flavour_cat/LFTagged/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      else       if( imu.HNL_MVA_Fake("QCD_BvsC_v5") > 0.) FillHist("SS"+JetLabel+"_Flavour_cat/BTagged/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      else FillHist("SS"+JetLabel+"_Flavour_cat/CTagged/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      
      TString label = imu.sLepGenType() +"_" + imu.GetPtLabel();
      if(IsData) label =  imu.GetPtLabel();
      
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/Pt", imu.Pt() , param.Weight("Full"), 40, 0., 200.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/PtRatio", imu.CloseJet_Ptratio() , param.Weight("Full"), 100, 0., 2.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/SS"+JetLabel+"Lep/" + label+ "_Muon/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);


      FillHist("Flavour_cat/SS"+JetLabel+"Lep/SS"+JetLabel+"Lep_Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");

      TString lep_fake_tag = MatchGenDef(All_Gens, Lepton(imu));
      vector<TString> lables = GetGenList();
      if (std::find(lables.begin(), lables.end(), lep_fake_tag) == lables.end()) return;
      FillCutflow(param, "Lep_Fake_type_"+JetLabel, weight, lables,lep_fake_tag);
    }
  }

 Part1:
  
  if(IsData) return;



  vector<TString> IDType = {"Loose","Tight"};
  for(int i = 0 ; i < 2; i++){
    for(auto imu : loose_muonsMVA){
      if(i==1 && !imu.PassID(param.Muon_Tight_ID)) continue;
      if(!imu.IsFake()) continue;
      
      FillMuonPlots((imu.CloseJet_FlavourInt() ==5), "HF_B" ,param, IDType[i],  imu, param.Weight("Full") );
      FillMuonPlots((imu.CloseJet_FlavourInt() ==4), "HF_C" ,param, IDType[i],  imu, param.Weight("Full") );
      FillMuonPlots((imu.CloseJet_FlavourInt() ==5), "HF" ,param, IDType[i],  imu, param.Weight("Full") );
      FillMuonPlots((imu.CloseJet_FlavourInt() ==4), "HF" ,param, IDType[i],  imu, param.Weight("Full") );
      FillMuonPlots((imu.CloseJet_FlavourInt() ==0), "LF" ,param, IDType[i],  imu, param.Weight("Full") );

      TString lep_fake_tag = MatchGenDef(All_Gens, Lepton(imu));
      vector<TString> lables = GetGenList();
      if (std::find(lables.begin(), lables.end(), lep_fake_tag) == lables.end()) return;

      vector<TString> Methods = {"_NoSelection"};

      if(imu.PassID(param.Muon_Tight_ID) || (!imu.PassID(param.Muon_Tight_ID) && imu.CloseJet_BScore() < 0.01)) Methods.push_back("_TriLepLoose10");
      if(imu.PassID(param.Muon_Tight_ID) || (!imu.PassID(param.Muon_Tight_ID) && imu.CloseJet_BScore() < 0.015)) Methods.push_back("_TriLepLoose15");
      if(imu.PassID(param.Muon_Tight_ID) || (!imu.PassID(param.Muon_Tight_ID) && imu.CloseJet_BScore() < 0.02)) Methods.push_back("_TriLepLoose20");
      if(imu.PassID(param.Muon_Tight_ID) || (!imu.PassID(param.Muon_Tight_ID) && imu.CloseJet_BScore() < 0.025)) Methods.push_back("_TriLepLoose25");
      if(imu.PassID(param.Muon_Tight_ID) || (!imu.PassID(param.Muon_Tight_ID) && imu.CloseJet_BScore() < 0.03)) Methods.push_back("_TriLepLoose30");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.95)  Methods.push_back("_MVATagged_LFReg0");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.90)  Methods.push_back("_MVATagged_LFReg1");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.85)  Methods.push_back("_MVATagged_LFReg2");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.80)  Methods.push_back("_MVATagged_LFReg3");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.75)  Methods.push_back("_MVATagged_LFReg4");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.70)  Methods.push_back("_MVATagged_LFReg5");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.65)  Methods.push_back("_MVATagged_LFReg6");
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.60)  Methods.push_back("_MVATagged_LFReg7");
      
      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") < 0.8 && imu.HNL_MVA_Fake("QCD_BvsC_v5") > 0.4) Methods.push_back("_MVATagged_HFBReg1");
      else if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") < 0.8 && imu.HNL_MVA_Fake("QCD_BvsC_v5") > -0.4) Methods.push_back("_MVATagged_HFBReg2");
      else if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") < 0.8 && imu.HNL_MVA_Fake("QCD_BvsC_v5") > -1.) Methods.push_back("_MVATagged_HFBReg3");

      for(auto im : Methods){
	FillHistogram(("LeptonFakeRates/"+IDType[i]+im+"/Inc/Pt_Inclusive").Data(), imu.Pt() , param.Weight("Full"), "Pt");
        if(AK4_JetColl.size() == 0)    FillHistogram(("LeptonFakeRates/"+IDType[i]+im+"/0j/Pt_Inclusive").Data(), imu.Pt() , param.Weight("Full"), "Pt");
        if(AK4_JetColl.size() >1)      FillHistogram(("LeptonFakeRates/"+IDType[i]+im+"/2j/Pt_Inclusive").Data(), imu.Pt() , param.Weight("Full"), "Pt");

	FillHistogram(("LeptonFakeRates/"+IDType[i]+im+"/Inc/Pt_"+imu.sLepGenType()).Data(), imu.Pt() , param.Weight("Full"), "Pt");
	if(AK4_JetColl.size() == 0)    FillHistogram(("LeptonFakeRates/"+IDType[i]+im+"/0j/Pt_"+imu.sLepGenType()).Data(), imu.Pt() , param.Weight("Full"), "Pt");
	if(AK4_JetColl.size() >1)      FillHistogram(("LeptonFakeRates/"+IDType[i]+im+"/2j/Pt_"+imu.sLepGenType()).Data(), imu.Pt() , param.Weight("Full"), "Pt");
      }

      
      if(SameCharge(loose_muonsMVA)) {
	FillHist("SS_Flavour_cat/"+IDType[i]+"/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
	if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.80)FillHist("SS_Flavour_cat/"+IDType[i]+"_LFTagged/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
	else       if( imu.HNL_MVA_Fake("QCD_BvsC_v5") > 0.) FillHist("SS_Flavour_cat/"+IDType[i]+"_BTagged/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
	else FillHist("SS_Flavour_cat/"+IDType[i]+"_CTagged/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");


	FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/Pt", imu.Pt() , param.Weight("Full"), 40, 0., 200.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/PtRatio", imu.CloseJet_Ptratio() , param.Weight("Full"), 100, 0., 2.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/SSLep_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);
        FillCutflow(param, "SSLep_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
        FillHist("Flavour_cat/"+IDType[i]+"/SSLep_Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");

      }
      continue;

      if(imu.PassID(param.Muon_Tight_ID) || (!imu.PassID(param.Muon_Tight_ID) && imu.CloseJet_BScore() < 0.025)){
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/Pt", imu.Pt() , param.Weight("Full"), 40, 0., 200.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/PtRatio", imu.CloseJet_Ptratio() , param.Weight("Full"), 100, 0., 2.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/TopReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);
        FillCutflow(param, "TopReg_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
        FillHist("Flavour_cat/"+IDType[i]+"/TopReg_Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      }

      if( imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.80){
	
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/Pt", imu.Pt() , param.Weight("Full"), 40, 0., 200.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/PtRatio", imu.CloseJet_Ptratio() , param.Weight("Full"), 100, 0., 2.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/LFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);
	FillCutflow(param, "LFTaggedReg_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
	FillHist("Flavour_cat/"+IDType[i]+"/LFTaggedReg_Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      }
      else{
	FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/Pt", imu.Pt() , param.Weight("Full"), 40, 0., 200.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/PtRatio", imu.CloseJet_Ptratio() , param.Weight("Full"), 100, 0., 2.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
        FillHist("FlavourCat/AntiLFTaggedReg_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);
        FillCutflow(param, "AntiLFTaggedReg_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
        FillHist("Flavour_cat/"+IDType[i]+"/AntiLFTaggedReg_Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
	
	if(imu.PassID(param.Muon_Tight_ID) || (!imu.PassID(param.Muon_Tight_ID) && imu.CloseJet_CvsBScore() > 0.1)){
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/Pt", imu.Pt() , param.Weight("Full"), 40, 0., 200.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/PtRatio", imu.CloseJet_Ptratio() , param.Weight("Full"), 100, 0., 2.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
	  FillHist("FlavourCat/Reg3_"+IDType[i]+"/" + imu.sLepGenType() + "_Muon/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);
	  FillCutflow(param, "Reg3_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
	  FillHist("Flavour_cat/"+IDType[i]+"/Reg3_Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
	}
      }

      FillHist("Flavour_cat/"+IDType[i]+"/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      if(SameCharge(loose_muonsMVA))   FillHist("SS_Flavour_cat/"+IDType[i]+"/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      if(loose_muonsMVA.size() ==2)       FillHist("Dilep_Flavour_cat/"+IDType[i]+"/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      if(useevent40) FillHist("FakeCR_Flavour_cat/"+IDType[i]+"/Flavour_cat", imu.CloseJet_FlavourInt() , param.Weight("Full"), 10, 0., 10., "Jet Hadron flavour");
      
      FillMuonPlots(useevent40,    "FakeCR40", param, IDType[i],  imu, param.Weight("Full") );
      FillMuonPlots((imu.CloseJet_BScore() > 0.7), "DeepJetM_Pass", param, IDType[i],  imu, param.Weight("Full") );
      FillMuonPlots((imu.CloseJet_BScore() < 0.2), "DeepJet_Fail", param, IDType[i],  imu, param.Weight("Full") );
    
      FillMuonPlots((imu.HNL_MVA_Fake("QCD_LFvsHF_v5") > 0.85), "LFvsHF_Pass", param, IDType[i],  imu, param.Weight("Full") );
      FillMuonPlots((imu.HNL_MVA_Fake("QCD_LFvsHF_v5") < 0.85), "LFvsHF_Fail", param, IDType[i],  imu, param.Weight("Full") );
      
      FillCutflow(param, imu.sLepGenType() + "_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
      if(AK4_JetColl.size() == 0)    FillCutflow(param, imu.sLepGenType() + "_0j_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
      if(AK4_JetColl.size() > 1)     FillCutflow(param, imu.sLepGenType() + "_2j_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
      if(SameCharge(loose_muonsMVA)){
	if(AK4_JetColl.size() == 0)       FillCutflow(param, "SS_"+imu.sLepGenType() + "_0j_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
	if(AK4_JetColl.size() > 1)      FillCutflow(param, "SS_"+imu.sLepGenType() + "_2j_Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
      }
      
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/Pt", imu.Pt() , param.Weight("Full"), 40, 0., 200.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);
      
    
      FillCutflow(param, imu.sLepGenType() + "_"+imu.GetPtLabel()+"Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
	
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetPtLabel()+"/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);

      FillCutflow(param, imu.sLepGenType() + "_"+imu.GetMotherPtLabel()+"Fake_type_"+IDType[i], weight, lables,lep_fake_tag);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/HFTop", imu.HNL_MVA_Fake("HFTop") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/QCD_BvsC_v5", imu.HNL_MVA_Fake("QCD_BvsC_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/QCD_LF1_v5", imu.HNL_MVA_Fake("QCD_LF1_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/QCD_LF2_v5", imu.HNL_MVA_Fake("QCD_LF2_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/QCD_LFvsHF_v5", imu.HNL_MVA_Fake("QCD_LFvsHF_v5") , param.Weight("Full"), 200, -1., 1.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/CloseJet_BScore", imu.CloseJet_BScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/CloseJet_CvsBScore", imu.CloseJet_CvsBScore() , param.Weight("Full"), 300, -1., 2.);
      FillHist("FlavourCat/"+IDType[i]+"/"+imu.sLepGenType() + "_Muon/"+imu.GetMotherPtLabel()+"/CloseJet_CvsLScore", imu.CloseJet_CvsLScore() , param.Weight("Full"), 300, -1., 2.);
    }
  }
  
  return;
  param.SplitPlot = "PtBin_Type_";

  if(SameCharge(loose_muonsMVA)){
    
    if(loose_muonsMVA[0].Pt() < 20) return;
    
    for(auto imu : loose_muonsMVA){
      
      FillMuonPlots((AK4_JetColl.size()==0), "0j", param, "SS_All_HNL_MuonFakeStudy_LooseMVA",  imu, param.Weight("Full") );
      FillMuonPlots((AK4_JetColl.size()==1), "1j", param, "SS_All_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
      FillMuonPlots((AK4_JetColl.size()>2),  "2j", param, "SS_All_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
      
      if(loose_muonsMVA[0].PassID(param.Muon_Tight_ID) && loose_muonsMVA[1].PassID(param.Muon_Tight_ID)){
	FillMuonPlots((AK4_JetColl.size()==0), "0j", param, "SS_TT_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
	FillMuonPlots((AK4_JetColl.size()==1), "1j", param, "SS_TT_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
	FillMuonPlots((AK4_JetColl.size()>2),  "2j", param, "SS_TT_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
      }
      else if(!imu.PassID(param.Muon_Tight_ID)){
	FillMuonPlots((AK4_JetColl.size()==0), "0j", param, "SS_L_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
        FillMuonPlots((AK4_JetColl.size()==1), "1j", param, "SS_L_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
        FillMuonPlots((AK4_JetColl.size()>2),  "2j", param, "SS_L_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
      }

      if(loose_muonsMVA[0].PassID(param.Muon_Tight_ID) && !loose_muonsMVA[1].PassID(param.Muon_Tight_ID)){
        FillMuonPlots((AK4_JetColl.size()==0), "0j", param, "SS_TL_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
	FillMuonPlots((AK4_JetColl.size()==1), "1j", param, "SS_TL_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
	FillMuonPlots((AK4_JetColl.size()>2),  "2j", param, "SS_TL_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
      }

      if(!loose_muonsMVA[0].PassID(param.Muon_Tight_ID) && loose_muonsMVA[1].PassID(param.Muon_Tight_ID)){
        FillMuonPlots((AK4_JetColl.size()==0), "0j", param, "SS_LT_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
	FillMuonPlots((AK4_JetColl.size()==1), "1j", param, "SS_LT_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
	FillMuonPlots((AK4_JetColl.size()>2),  "2j", param, "SS_LT_HNL_MuonFakeStudy_LooseMVA",  imu, weight );
      }

      
    }
  }

  
  //  bool useevent40 = UseEvent(leps , jets, 40.,  METv, weight);
  
  return;
  
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
