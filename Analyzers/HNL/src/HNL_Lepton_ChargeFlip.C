#include "HNL_Lepton_ChargeFlip.h"

void HNL_Lepton_ChargeFlip::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  
  /// 5 MODES
  ///

  // HasFlag("ShiftEnergy")  ---> Step 1 measure Pt response of CF vs Prompt Electron to Get Pt/Eta dep shift  
  // HasFlag("ElCFRates")    ---> Measure rates in DY/Top MC (using Pt CF response)
  // HasFlag("ClosureTest") ---> Check mc closure  (Using only DY)
  // HasFlag("ScaleFactor")  ---> Compare Data SS vs OS weighted in BB / EC (using CF Rates) 
  
  //  cfEst->IgnoreNoHist = true;

  //  if(HasFlag("ShiftEnergy"))cfEst->IgnoreNoHist = true;
  
}

void HNL_Lepton_ChargeFlip::executeEvent(){
  
  
  vector<TString> LepIDs = {"HNL_ULID_"+GetYearString() ,"HNTightV2", "POGTight", "HNL_ULID_LooseNP_"+GetYearString() ,  "HNL_ULID_LooseCF_"+GetYearString()};

  for (auto id: LepIDs){
    AnalyzerParameter param = InitialiseHNLParameter("SignalStudy");
    param.Name    =  id;
    param.DefName =  id;
    
    //// Do not include Xsec norm since looking to Add TT/DY samples in Eff
    param.Apply_Weight_Norm1Ipb = false;
    param.Apply_Weight_LumiNorm = false;
    
    if(HasFlag("ScaleFactor")){
      param.Apply_Weight_LumiNorm = true;
      param.Apply_Weight_Norm1Ipb = true;
    }
    if(HasFlag("ClosureTest")){
      param.Apply_Weight_LumiNorm = true;
      param.Apply_Weight_Norm1Ipb = true;
    }
    
    param.Apply_Weight_SumQ     = true;
    param.Apply_Weight_PileUp   = true;
    param.Apply_Weight_PreFire  = true;
    param.Apply_Weight_kFactor  = false;
    param.Apply_Weight_IDSF     = false;
    param.Apply_Weight_RECOSF     = true;
    param.Apply_Weight_TriggerSF= false;
      
    param.FakeMethod   = "DATA";
    param.CFMethod     = "DATA";
    param.ConvMethod   = "MC";
    
    param.Electron_Tight_ID = (id == "TopHN") ? "TopHNSST" :  id;
    param.Muon_Tight_ID   =    "POGTightWithTightIso";
    if(id =="POGTight"){   
      param.Electron_Tight_ID  = "passPOGTight";
      //param.k.Electron_CF  = CFMethod+"_EtaRegion_POGTight";
    }
    executeEventFromParameter(param);
  }
}

// check if fitting is useless
/*
 execute under --userflag CFfitval
1. pick up some processes with large xsec (ttbar DY)
2. choose OS ee events
3-1. use CF rate fit function to estimate SS ee from OS ee events
3-2. use CF rate bin values to estimate SS ee from OS ee events

and compare 3-1 and 3-2

*/

void HNL_Lepton_ChargeFlip::executeEventFromParameter(AnalyzerParameter param){

  if(_jentry==0) param.PrintParameters();
  run_Debug = (_jentry%nLog==0);

  if(!PassMETFilter()) return;

 
  Event ev = GetEvent();

  double EvWeight =SetupWeight(ev,param);
  if(_jentry==4000) cout << "EvWeight  = " << EvWeight << endl;
  // Electrons 
  std::vector<Electron> ElectronColltmp;
  if(param.Electron_Tight_ID.Contains("HEEP")) ElectronColltmp = GetElectrons(param.Electron_Tight_ID,50.,2.5);
  else ElectronColltmp = GetElectrons(param.Electron_Tight_ID,15.,2.5);
  
  if(run_Debug) cout <<  "Number of All ID Electrons = " << ElectronColltmp.size() << endl;

  // Muons
  std::vector<Muon> MuonColl; std::vector<Muon> MuonCollVeto;
  if(param.Muon_Tight_ID.Contains("HEEP")) MuonColl = GetMuons(param.Muon_Tight_ID,25.,2.4);
  else MuonColl = GetMuons(param.Muon_Tight_ID,10.,2.4);
  MuonCollVeto = GetMuons(param.Muon_Veto_ID,10.,2.4);
  std::sort(MuonColl.begin(),MuonColl.end(),PtComparing);

  // Gen

  vector<Electron> ElectronCollPrompt = ElectronPromptOnly(ElectronColltmp,All_Gens,param);
  vector<Electron> ElectronColl;
  if(MCSample.Contains("WG") || MCSample.Contains("ZG"))  {
    ElectronColl = ElectronCollPrompt;
  }
  else {
    for(auto iel: ElectronCollPrompt){
      if(HasPromptConv(iel)) continue;
      if(iel.LeptonGenType() >= 4 ) continue;
      ElectronColl.push_back(iel);
    }
  }


  vector<Muon> PromptMuonColl   = MuonPromptOnly(MuonColl,All_Gens,param);
  std::sort(ElectronColl.begin(),ElectronColl.end(),PtComparing);
  std::vector<Lepton *> Leptons = MakeLeptonPointerVector(PromptMuonColl,ElectronColl,param);

  if(run_Debug)cout <<  "Number of GEN MATCHED ID Electrons = "<< ElectronColl.size() << endl;


  // Jet
  vector<Jet> JetColl = GetJets(param.Jet_ID,10.,5.);
  vector<Jet> BJetColl;
  JetTagging::Parameters jtp;
  jtp = JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::incl, JetTagging::comb);
  BJetColl = SelBJets(JetColl,jtp);



  if(!PassHEMVeto(Leptons)) return ;


  //// E shift from Z peak

  // BB 2017 0.075%
  // EC 2017 0.08%

  if(HasFlag("ElCFRates")){

    if(IsDATA) return;
    
    int nbins_invpt(15);
    int nbin = 15;
    double invptbins [nbins_invpt+1] = { 0., 
					 0.002,
					 0.003,
					 0.004,
					 0.005, 
					 0.0075,
					 0.010,
					 0.0125,
					 0.015,
					 0.0175,
					 0.020,
					 0.0225,
					 0.025, 
					 0.030,
					 0.035, 
					 0.04};
    double ptbin     [nbin+1]        = { 15.,
					 25.,
					 40.,
					 45., 
					 50.,
					 65., 
					 80., 
					 100.,
					 125.,
					 150.,
					 175.,
					 200.,
					 300.,
					 400.,
					 500.,
					 1000.};       

    int nbins_eta  = 4;
    int nbins_eta2 = 5 ;
    int nbins_eta3 = 6 ;

    double etabins   [nbins_eta+1]   = {  0.,0.8,      1.4442,1.556,      2.5};
    double etabins2  [nbins_eta2+1]  = {  0.,0.8,      1.4442,1.556, 2.2, 2.5};
    double etabins3  [nbins_eta3+1]  = {  0.,0.8, 1.1, 1.4442,1.556, 2.2, 2.5};

    for(unsigned int i=0 ; i < ElectronColl.size() ; i++){

      /// Remove Conv and Fakeelectrons
      if(ElectronColl.at(i).LeptonGenType() <= 0 || ElectronColl.at(i).LeptonGenType() >=4) continue;
      
      /////TruthIdx returns matched lepton status 1 index
      int TruthIdx  = GenMatchedIdx(ElectronColl.at(i), All_Gens);;
      if(TruthIdx < 0 || (TruthIdx > int(All_Gens.size()))) continue;

      ///// Get access to Matched status 1 gen el
      Gen truth_lep = All_Gens[TruthIdx];
      if(truth_lep.PID() ==0 ) continue;


      bool IsCF = ElectronColl.at(i).LeptonIsCF();
    
      /// CF Rate Method 2 WIth Int Conv                                                                                                                    
      
      if(HasPromptConv(ElectronColl.at(i))) continue;
      
      for(unsigned int imethod = 0; imethod < 3; imethod++){
	
	double PtShift = 1. / GetShiftCFEl(ElectronColl.at(i),param.Electron_Tight_ID);
	TString Method = "PBS";
	if(imethod==0) {
	  /// PtShift for CF -> Prompt 
	  PtShift = 1. / GetShiftCFEl(ElectronColl.at(i),param.Electron_Tight_ID);
	  Method = "PBS";
	}
	if(imethod==1) {
	  PtShift = 1. ;
	  Method = "NoS";
	}
	if(imethod==2) {
          PtShift = 1.3 ;
          Method = "CS";
	}

	if(_jentry>=4000 && _jentry<=4500) cout << "PtShift   = " << GetShiftCFEl(ElectronColl.at(i),param.Electron_Tight_ID) << " Eta = " << abs(ElectronColl.at(i).scEta()) << " pt = " << ElectronColl.at(i).Pt() << endl;
	
		
	vector<TString> FillCFrates = {"Denom"};
	if(IsCF) FillCFrates.push_back("Num");
	
	for(auto icf : FillCFrates){
	  
	  double ShiftValue = IsCF ? PtShift : 1.;
	  
	  if(abs(ElectronColl.at(i).scEta())<0.8) FillHist(param.Name+"/CFRate/"+Method+"/EtaRegion1_"+icf,ElectronColl.at(i).InvPt(ShiftValue), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	  else if(abs(ElectronColl.at(i).scEta())<1.1) FillHist(param.Name+"/CFRate/"+Method+"/EtaRegion2_"+icf,ElectronColl.at(i).InvPt(ShiftValue), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	  else if(abs(ElectronColl.at(i).scEta())<1.5) FillHist(param.Name+"/CFRate/"+Method+"/EtaRegion3_"+icf,ElectronColl.at(i).InvPt(ShiftValue), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	  else if(abs(ElectronColl.at(i).scEta())<2.2) FillHist(param.Name+"/CFRate/"+Method+"/EtaRegion4_"+icf,ElectronColl.at(i).InvPt(ShiftValue), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	  else FillHist(param.Name+"/CFRate/"+Method+"/EtaRegion5_"+icf,ElectronColl.at(i).InvPt(ShiftValue), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	  
	  //// Pt Inclusive
	  FillHist(param.Name+"/CFRateInv/"+Method + "/" +icf, ElectronColl.at(i).InvPt(ShiftValue), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	  FillHist(param.Name+"/CFRatePt/"+Method + "/"  +icf, ElectronColl.at(i).PtMaxed(2000,ShiftValue), EvWeight,nbin, ptbin,"pT");
	  
	  
	  FillHist(param.Name+"/CFRate/absEta/"+Method + "/"  +icf, fabs(ElectronColl.at(i).scEta()),  EvWeight, nbins_eta,etabins ,"#eta");
	  FillHist(param.Name+"/CFRate/Eta/"+Method + "/"  +icf,    fabs(ElectronColl.at(i).scEta()),     EvWeight, nbins_eta2,etabins2 ,"#eta");
	  FillHist(param.Name+"/CFRate/Eta2/"+Method + "/"  +icf,   fabs(ElectronColl.at(i).scEta()),    EvWeight, nbins_eta3,etabins3 ,"#eta");
	  
	  FillHist(param.Name+"/CFRate_Eta/"+Method + "/"+icf, ElectronColl.at(i).scEta(), EvWeight, 50, -2.5, 2.5,"scEta");
	  
	  FillHist(param.Name+"/CFRate_PtEta/"    +Method + "/"+icf,ElectronColl.at(i).PtMaxed(2000,ShiftValue),  ElectronColl.at(i).scEta(), EvWeight,  nbin,        ptbin,     nbins_eta,etabins);
	  FillHist(param.Name+"/CFRate_PtEta2/"   +Method + "/"+icf,ElectronColl.at(i).PtMaxed(2000,ShiftValue), ElectronColl.at(i).scEta(), EvWeight,  nbin,        ptbin,     nbins_eta2,etabins2);
	  FillHist(param.Name+"/CFRate_PtEta3/"   +Method + "/"+icf,ElectronColl.at(i).PtMaxed(2000,ShiftValue), ElectronColl.at(i).scEta(), EvWeight,  nbin,        ptbin,     nbins_eta3,etabins3);
	  FillHist(param.Name+"/CFRate_InvPtEta/" +Method + "/"+icf,ElectronColl.at(i).InvPt(ShiftValue) ,     ElectronColl.at(i).scEta(), EvWeight,  nbins_invpt, invptbins, nbins_eta,etabins);
	  FillHist(param.Name+"/CFRate_InvPtEta2/"+Method + "/"+icf,ElectronColl.at(i).InvPt(ShiftValue),     ElectronColl.at(i).scEta(), EvWeight,  nbins_invpt, invptbins, nbins_eta2,etabins2);
	  FillHist(param.Name+"/CFRate_InvPtEta3/"+Method + "/"+icf,ElectronColl.at(i).InvPt(ShiftValue),     ElectronColl.at(i).scEta(), EvWeight,  nbins_invpt, invptbins, nbins_eta3,etabins3);
	}
      }
    }
    return;
  }
  
  if(HasFlag("ShiftCFStudy")){

    for(auto iel : ElectronColl){
      Gen MatchedGen = GetGenMatchedLepton(iel, All_Gens);
      
      if(iel.LeptonIsCF()) {

	FillHist(param.Name+"/CFStudy/CF_El_Eta", iel.scEta() /MatchedGen.Eta() , EvWeight ,100, 0 , 5);
	FillHist(param.Name+"/CFStudy/CF_El_Phi", iel.Phi() /MatchedGen.Phi() , EvWeight ,100, 0 , 5);
        FillHist(param.Name+"/CFStudy/CF_El_E",   iel.E() /MatchedGen.E() , EvWeight ,100, 0 , 5);
        FillHist(param.Name+"/CFStudy/CF_El_Pt",  iel.Pt() /MatchedGen.Pt() , EvWeight ,100, 0 , 5);
      }
      else{
	FillHist(param.Name+"/CFStudy/Prompt_El_Eta", iel.scEta() /MatchedGen.Eta() , EvWeight ,100, 0 , 5);
        FillHist(param.Name+"/CFStudy/Prompt_El_Phi", iel.Phi() /MatchedGen.Phi() , EvWeight ,100, 0 , 5);
        FillHist(param.Name+"/CFStudy/Prompt_El_E",   iel.E() /MatchedGen.E() , EvWeight ,100, 0 , 5);
        FillHist(param.Name+"/CFStudy/Prompt_El_Pt",  iel.Pt() /MatchedGen.Pt() , EvWeight ,100, 0 , 5);
      }
    }   
    
    return;
  }


  
  if(HasFlag("ClosureTest")){
    
    int nbinNeg= 17;

    double pTbinNeg[nbinNeg+1] = {-200,-100,-80,-50,-45,-40,-35,-30,-15, 15.,30.,35,40.,45, 50., 80.,100.,200.};
    double ptbin     [14]      = {0.,20.,30.,35., 40.,45., 50.,65., 80.,  100., 150., 200.,500.,1000};

    if(run_Debug) cout << "ClosureTest processing " << endl;

    for(auto iel : ElectronColl){
      /// Remove Conv and Fakeelectrons                                                                                                                                                                                              
      if(iel.LeptonGenType() <= 0 || iel.LeptonGenType() >=4) continue;

      /////TruthIdx returns matched lepton status 1 index                                                                                                                                                                            
      int TruthIdx  = GenMatchedIdx(iel, All_Gens);;
      if(TruthIdx < 0 || (TruthIdx > int(All_Gens.size()))) continue;

      ///// Get access to Matched status 1 gen el                                                                                                                                                                                    
      Gen truth_lep = All_Gens[TruthIdx];
      if(truth_lep.PID() ==0 ) continue;


      bool IsCF = iel.LeptonIsCF();
      
      if(HasPromptConv(iel)) continue;
      
      double PtShift = GetShiftCFEl(iel,param.Electron_Tight_ID);
      
      TString CFKey = param.Electron_Tight_ID;
      if(CFKey =="passPOGTight") CFKey="POGTight";

      double rate_cf_eta3_pbs       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_PtEta3_PBS_" +CFKey,iel.defEta(), iel.Pt(), 0);

      double rate_cf_inv_eta_pbs       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta_PBS_" +CFKey,iel.defEta(), iel.Pt(), 0);
      double rate_cf_inv_eta2_pbs       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta2_PBS_" +CFKey,iel.defEta(), iel.Pt(), 0);

      double rate_cf_inv_eta3_cs       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta3_CS_" +CFKey,iel.defEta(), iel.Pt(), 0);
      double rate_cf_inv_eta3_nos       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta3_NoS_" +CFKey,iel.defEta(), iel.Pt(), 0);
      double rate_cf_inv_eta3_pbs       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta3_PBS_" +CFKey,iel.defEta(), iel.Pt(), 0);

      
      if(IsCF){
	FillHist(param.Name+"/Closure/CF_LepPt",   iel.PtMaxed(2000) ,       EvWeight        , 13, ptbin,"pT");
	FillHist(param.Name+"/Closure/CF_LepEta",  fabs(iel.scEta()) ,       EvWeight        , 100,0, 2.5);
      }
      else {
	vector<TString> WeightS = {"_w_pbs_pteta3", "_w_pbs_invpteta","_w_pbs_invpteta2","_w_cs_invpteta3","_w_nos_invpteta3","_w_pbs_invpteta3"};
	vector<double> WeightD = {rate_cf_eta3_pbs, rate_cf_inv_eta_pbs, rate_cf_inv_eta2_pbs, rate_cf_inv_eta3_cs,rate_cf_inv_eta3_nos,rate_cf_inv_eta3_pbs};
				  
	for(unsigned int iw = 0 ; iw < 4; iw++){
	  FillHist(param.Name+"/Closure/NonCF_LepPt_"+WeightS[iw],  iel.PtMaxed(2000)*PtShift ,  WeightD[iw]*EvWeight , 13, ptbin,"pT");
	  FillHist(param.Name+"/Closure/NonCF_LepPt_NoShift_"+WeightS[iw],  iel.PtMaxed(2000) ,  WeightD[iw]*EvWeight , 13, ptbin,"pT");
	  FillHist(param.Name+"/Closure/NonCF_LepEta_"+WeightS[iw],  fabs(iel.scEta()) ,       WeightD[iw]*EvWeight        , 100,0, 2.5);
	}
      }
    }


    if(ElectronColl.size() != 2) return;
    if(Leptons.size() != 2) return;

    //// Select SS events in MC that are 1 Prompt + 1 CF
    if(SameCharge(ElectronColl)){
      int nCF = 0;
      if(ElectronColl.at(0).LeptonIsCF()) nCF++;
      if(ElectronColl.at(1).LeptonIsCF()) nCF++;
      if(ElectronColl.at(0).IsPrompt() && ElectronColl.at(1).IsPrompt() && (nCF == 1)){

	/// Remove rare converstions  where e->eee share SC using HasPromptConv
	if(!HasPromptConv(ElectronColl.at(0)) && !HasPromptConv(ElectronColl.at(1))) {
	  
	  // To be safe remove LeptonGenType() > 4 which are conversions
	  if(ElectronColl.at(0).LeptonGenType() < 4 &&  ElectronColl.at(1).LeptonGenType() <4) {
	    
	    // Construct LL candidate
	    Particle ZCand = ElectronColl.at(0)+ElectronColl.at(1);
	    double Zmass = ZCand.M(); 
	    if(Zmass < 50) Zmass=50;
	    if(Zmass > 150) Zmass=150;

	    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB())  FillHist(param.Name+"/ZMass/SS_BB", Zmass , EvWeight ,100 , 50, 150);
	    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC())  FillHist(param.Name+"/ZMass/SS_EC", Zmass , EvWeight ,100 , 50, 150);
	    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB()) {
	      FillHist(param.Name+"/Z/SS_BB_LepPt",  ElectronColl[0].PtMaxed(200) ,         EvWeight , nbinNeg, pTbinNeg,"pT");
	      FillHist(param.Name+"/Z/SS_BB_LepPt", -1*ElectronColl[1].PtMaxed(200) ,      EvWeight , nbinNeg, pTbinNeg,"pT");
	      FillHist(param.Name+"/Z/SS_BB_LepEta",  (ElectronColl[0].scEta()+2.5) , EvWeight, 100., -5., 5., "Eta");
	      FillHist(param.Name+"/Z/SS_BB_LepEta",  -1*(ElectronColl[1].scEta()+2.5) , EvWeight, 100., -5., 5., "Eta");
	    }
	    
	    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC()) {
              FillHist(param.Name+"/Z/SS_EC_LepPt", ElectronColl[0].PtMaxed(200) ,         EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/Z/SS_EC_LepPt", -1*ElectronColl[1].PtMaxed(200) ,      EvWeight , nbinNeg, pTbinNeg,"pT");
	      FillHist(param.Name+"/Z/SS_EC_LepEta",  (ElectronColl[0].scEta()+2.5) , EvWeight, 100., -5., 5., "Eta");
	      FillHist(param.Name+"/Z/SS_EC_LepEta",  -1*(ElectronColl[1].scEta()+2.5) , EvWeight, 100., -5., 5., "Eta");

	    } // EC

	    FillHist(param.Name+"/ZMass/SS", Zmass , EvWeight ,100 , 50, 150);
	    FillHist(param.Name+"/Z/SS_LepPt", ElectronColl[0].PtMaxed(200) ,         EvWeight , nbinNeg, pTbinNeg,"pT");
	    FillHist(param.Name+"/Z/SS_LepPt", -1*ElectronColl[1].PtMaxed(200) ,      EvWeight , nbinNeg, pTbinNeg,"pT");
	    FillHist(param.Name+"/Z/SS_LepEta",  (ElectronColl[0].scEta()+2.5) , EvWeight, 100., -5., 5., "Eta");
	    FillHist(param.Name+"/Z/SS_LepEta",  -1*(ElectronColl[1].scEta()+2.5) , EvWeight, 100., -5., 5., "Eta");

            FillHist(param.Name+"/Z/SS_Lepton1Pt", ElectronColl[0].PtMaxed(200) ,      EvWeight , 40, 0, 200,"pT");
            FillHist(param.Name+"/Z/SS_Lepton2Pt", ElectronColl[1].PtMaxed(200) ,      EvWeight , 40, 0, 200,"pT");

	  } /// Non Conv
	} 
      } //// END OF Prompt + CF
    }
    else {

      //// Now plot OS * CF rate [Energy Shifted] to compare
      if(ElectronColl.at(0).IsPrompt() && ElectronColl.at(1).IsPrompt()){

	if(HasPromptConv(ElectronColl.at(0))  || HasPromptConv(ElectronColl.at(1))) return;
	if(ElectronColl.at(0).LeptonGenType() >= 4 || ElectronColl.at(1).LeptonGenType() >=4) return;
	

	//// Loop several Energy shift
	for (unsigned int imethod = 0 ; imethod < 27; imethod++){
	  
	  TString ShiftVersion = "";
	  if(imethod==0) ShiftVersion = "Paired_0p8";
	  if(imethod==1) ShiftVersion = "Paired_0p9";
	  if(imethod==2) ShiftVersion = "Paired_1p0";
	  if(imethod==3) ShiftVersion = "Paired_1p1";
	  if(imethod==4) ShiftVersion = "Paired_1p2";
	  if(imethod==5) ShiftVersion = "Paired_1p3";
	  if(imethod==6) ShiftVersion = "Paired_1p4";
	  if(imethod==7) ShiftVersion = "Paired_1p5";
	  if(imethod==8) ShiftVersion = "Paired_BinnedShift";
	  if(imethod==9) ShiftVersion = "Separate_0p8";
	  if(imethod==10) ShiftVersion = "Separate_0p9";
	  if(imethod==11) ShiftVersion = "Separate_1p0";
	  if(imethod==12) ShiftVersion = "Separate_1p1";
	  if(imethod==13) ShiftVersion = "Separate_1p2";
	  if(imethod==14) ShiftVersion = "Separate_1p3";
	  if(imethod==15) ShiftVersion = "Separate_1p4";
	  if(imethod==16) ShiftVersion = "Separate_1p5";
	  if(imethod==17) ShiftVersion = "Separate_BinnedShift";
	  if(imethod==18) ShiftVersion = "Separate_0p8";
	  if(imethod==19) ShiftVersion = "Separate_0p9";
	  if(imethod==20) ShiftVersion = "Separate_1p0";
	  if(imethod==21) ShiftVersion = "Separate_1p1";
	  if(imethod==22) ShiftVersion = "Separate_1p2";
	  if(imethod==23) ShiftVersion = "Separate_1p3";
	  if(imethod==24) ShiftVersion = "Separate_1p4";
	  if(imethod==25) ShiftVersion = "Separate_1p5";
	  if(imethod==26) ShiftVersion = "Separate_BinnedShift";

	  for (unsigned int iparam = 0 ; iparam < 7; iparam++){

            TString CFKey = param.Electron_Tight_ID;
            if(CFKey =="passPOGTight")CFKey="POGTight";

	    TString CFString = "CFRate_PtEta3_PBS_" +CFKey;

            if(iparam== 1 ) CFString = "CFRate_PtEta3_PBS_" +CFKey;
	    if(iparam== 2 ) CFString = "CFRate_InvPtEta_PBS_" +CFKey;
            if(iparam== 3 ) CFString = "CFRate_InvPtEta2_PBS_" +CFKey;
            if(iparam== 4 ) CFString = "CFRate_InvPtEta3_CS_" +CFKey;
            if(iparam== 5 ) CFString = "CFRate_InvPtEta3_NoS_" +CFKey;
            if(iparam== 6 ) CFString = "CFRate_InvPtEta3_PBS_" +CFKey;

	    
	    ShiftVersion = ShiftVersion + "/"+CFString;

	    // Uncorrected 
	    Electron unshifted_el1 = ElectronColl.at(0);
            Electron unshifted_el2 = ElectronColl.at(1);

	    vector<Electron> ShiftedEl;
	    Electron this_el1 = ElectronColl.at(0);
	    Electron this_el2 = ElectronColl.at(1);
	    
	    Particle ZCand = this_el1+this_el2;
	    double Zmass = ZCand.M();
	    
	    TString ShiftType = "minChi2";
	    if(imethod==8 || imethod==17 || imethod==26) ShiftType = "minChi2";
	    
	    	    
	    double shiftEl1 = GetShiftCFEl(this_el1,param.Electron_Tight_ID,ShiftType);
	    double shiftEl2 = GetShiftCFEl(this_el2,param.Electron_Tight_ID,ShiftType);
	    
	    /// Method 0 : Both el are shifted using 1.3% from past result
	    /// Method 1 : Both el are shifted using Pt Binned
	    /// Method 2/4 : Individual el are shifted (2 per event) using 1.3%
	    /// Method 3/5 : Individual el are shifted (2 per event) using PtBinned

	    if     (imethod  == 0) { this_el1*= 0.992;  this_el2*= 0.992;}
	    else if(imethod  == 1) { this_el1*= 0.991;  this_el2*= 0.991;}
	    else if(imethod  == 2) { this_el1*= 0.990;  this_el2*= 0.990;}
	    else if(imethod  == 3) { this_el1*= 0.989;  this_el2*= 0.989;}
            else if(imethod  == 4) { this_el1*= 0.988;  this_el2*= 0.988;}
            else if(imethod  == 5) { this_el1*= 0.987;  this_el2*= 0.987;}
            else if(imethod  == 6) { this_el1*= 0.986;  this_el2*= 0.986;}
            else if(imethod  == 7) { this_el1*= 0.985;  this_el2*= 0.985;}
	    else if(imethod  == 8) { this_el1*= shiftEl1;  this_el2*= shiftEl2;     }
	    else if     (imethod  == 9)  this_el1*= 0.992; 
	    else if     (imethod  == 10)  this_el1*= 0.991; 
	    else if     (imethod  == 11)  this_el1*= 0.990; 
	    else if     (imethod  == 12)  this_el1*= 0.989; 
	    else if     (imethod  == 13)  this_el1*= 0.988; 
	    else if     (imethod  == 14)  this_el1*= 0.987; 
	    else if     (imethod  == 15)  this_el1*= 0.986; 
	    else if     (imethod  == 16)  this_el1*= 0.985; 
	    else if     (imethod  == 17)  this_el1*= shiftEl1;
	    
	    else if     (imethod  == 18)  this_el2*= 0.992;
            else if     (imethod  == 19)  this_el2*= 0.991;
            else if     (imethod  == 20)  this_el2*= 0.990;
            else if     (imethod  == 21)  this_el2*= 0.989;
            else if     (imethod  == 22)  this_el2*= 0.988;
            else if     (imethod  == 23)  this_el2*= 0.987;
            else if     (imethod  == 24)  this_el2*= 0.986;
            else if     (imethod  == 25)  this_el2*= 0.985;
            else if     (imethod  == 26)  this_el2*= shiftEl2;

	    
	    Particle ZShiftedCand = this_el1+this_el2;
	    double ZShiftedmass = ZShiftedCand.M();
	    if(ZShiftedmass < 50) ZShiftedmass=50;
	    if(ZShiftedmass > 150) ZShiftedmass=150;
	    
	    if(Zmass < 50) Zmass = 50;
	    if(Zmass > 150) Zmass = 150;
	    
	    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB())FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_BB_EnergyShift", ZShiftedmass , EvWeight ,100 , 50, 150);
	    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC())FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_EC_EnergyShift", ZShiftedmass , EvWeight ,100 , 50, 150);
	    
	    /// Non shifted
	    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB())FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_BB", Zmass , EvWeight ,100 , 50, 150);
	    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC())FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_EC", Zmass , EvWeight ,100 , 50, 150);
	    
	    ShiftedEl.clear();
	    ShiftedEl.push_back(this_el1);
	    ShiftedEl.push_back(this_el2);
	    
	    double weight_shifted = 1;
            double weight_unshifted = 1;

	    if(imethod< 8) {
	      
	      double CFRate1   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,this_el1.defEta(), this_el1.Pt(), 0);
	      double CFRate2   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,this_el2.defEta(), this_el2.Pt(), 0);
	      
	      weight_shifted = (CFRate1/(1-CFRate1)) + (CFRate2/(1-CFRate2));

	      double CFRate1_unshifted   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,unshifted_el1.defEta(), unshifted_el1.Pt(), 0);
              double CFRate2_unshifted   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,unshifted_el2.defEta(), unshifted_el2.Pt(), 0);

              weight_unshifted = (CFRate1_unshifted/(1-CFRate1_unshifted)) + (CFRate2_unshifted/(1-CFRate2_unshifted));

	    }

	    else  if(imethod < 18) {
	      double CFRate1   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,this_el1.defEta(), this_el1.Pt(), 0);
	      weight_shifted = (CFRate1/(1-CFRate1));
	      
	      double CFRate1_unshifted   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,unshifted_el1.defEta(), unshifted_el1.Pt(), 0);
	      weight_unshifted = (CFRate1_unshifted/(1-CFRate1_unshifted));
	    }
	    else {
	      double CFRate2   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,this_el2.defEta(), this_el2.Pt(), 0);
	      weight_shifted = (CFRate2/(1-CFRate2));

	      double CFRate2_unshifted   = cfEst->GetElectronCFRate(param.Electron_Tight_ID, CFString,unshifted_el2.defEta(), unshifted_el2.Pt(), 0);
	      weight_unshifted = (CFRate2_unshifted/(1-CFRate2_unshifted));

	    }
	    
	    
	    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB()){
	      FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_BB_Weighted_EnergyShift",  ZShiftedmass             , weight_shifted*EvWeight ,100 , 50, 150);
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_BB_Weighted_EnergyShift_Pt",   this_el1.PtMaxed(200)    , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_BB_Weighted_EnergyShift_Pt",   -1*this_el2.PtMaxed(200) , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_BB_Weighted_EnergyShift_Eta",  (this_el1.scEta()+2.5)     , weight_shifted*EvWeight ,100., -5, 5, "Eta");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_BB_Weighted_EnergyShift_Eta",  -1*(this_el2.scEta()+2.5)  , weight_shifted*EvWeight ,100., -5, 5, "Eta");

	      
	      FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_BB_Weighted",              Zmass                    , weight_unshifted*EvWeight ,100 , 50, 150);
	      FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_BB_Weighted_Pt",   unshifted_el1.PtMaxed(200)    , weight_unshifted*EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_BB_Weighted_Pt",   -1*unshifted_el2.PtMaxed(200) , weight_unshifted*EvWeight , nbinNeg, pTbinNeg,"pT");	      	      
	    }
	    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC()){
	      FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_EC_Weighted_EnergyShift", ZShiftedmass            , weight_shifted*EvWeight ,100 , 50, 150);
	      FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_EC_Weighted_EnergyShift_Pt",  this_el1.PtMaxed(200)   , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_EC_Weighted_EnergyShift_Pt", -1*this_el2.PtMaxed(200) , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_EC_Weighted_EnergyShift_Eta", (this_el1.scEta()+2.5)    , weight_shifted*EvWeight ,100., -5, 5, "Eta");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_EC_Weighted_EnergyShift_Eta", -1*(this_el2.scEta()+2.5) , weight_shifted*EvWeight ,100., -5, 5, "Eta");

	      FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_EC_Weighted",              Zmass                    , weight_unshifted*EvWeight ,100 , 50, 150);
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_EC_Weighted_Pt",   unshifted_el1.PtMaxed(200)    , weight_unshifted*EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_EC_Weighted_Pt",   -1*unshifted_el2.PtMaxed(200) , weight_unshifted*EvWeight , nbinNeg, pTbinNeg,"pT");
      	      
	    }

	    FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_Weighted_EnergyShift", ZShiftedmass            , weight_shifted*EvWeight ,100 , 50, 150);
            FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_EnergyShift_Pt",  this_el1.PtMaxed(200)   , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
            FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_EnergyShift_Pt", -1*this_el2.PtMaxed(200) , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
	    FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_EnergyShift_Eta", (this_el1.scEta()+2.5)    , weight_shifted*EvWeight ,100., -5, 5, "Eta");
	    FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_EnergyShift_Eta", -1*(this_el2.scEta()+2.5) , weight_shifted*EvWeight ,100., -5, 5, "Eta");

	    FillHist(param.Name+"/"+ShiftVersion+"/ZMass/OS_Weighted",  Zmass                                , weight_unshifted*EvWeight ,100 , 50, 150);
	    FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_Pt",   unshifted_el1.PtMaxed(200)    , weight_unshifted*EvWeight , nbinNeg, pTbinNeg,"pT");
	    FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_Pt",   -1*unshifted_el2.PtMaxed(200) , weight_unshifted*EvWeight , nbinNeg, pTbinNeg,"pT");

            FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_EnergyShift_Lepton1Pt", this_el1.PtMaxed(200) ,      weight_shifted*EvWeight , 40, 0, 200,"pT");
            FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_EnergyShift_Lepton2Pt", this_el2.PtMaxed(200) ,      weight_shifted*EvWeight , 40, 0, 200,"pT");

	    FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_Lepton1Pt", unshifted_el1.PtMaxed(200) ,      weight_unshifted*EvWeight , 40, 0, 200,"pT");
            FillHist(param.Name+"/"+ShiftVersion+"/Z/OS_Weighted_Lepton2Pt", unshifted_el2.PtMaxed(200) ,      weight_unshifted*EvWeight , 40, 0, 200,"pT");

	    
	  }
	}
      }
    }/// END OS
    return;
  }


  if(HasFlag("ShiftEnergy")){

    
    for(auto iel : ElectronColl){

      
      /// Remove Conv and Fakeelectrons
      if(iel.LeptonGenType() <= 0 || iel.LeptonGenType() >=4) continue;

      /////TruthIdx returns matched lepton status 1 index
      int TruthIdx  = GenMatchedIdx(iel, All_Gens);;
      if(TruthIdx < 0 || (TruthIdx > int(All_Gens.size()))) continue;

      ///// Get access to Matched status 1 gen el
      Gen truth_lep = All_Gens[TruthIdx];
      if(truth_lep.PID() ==0 ) continue;

      //int LastSelfIdx    = LastAbsSelfMotherIdx(TruthIdx,All_Gens);
      
      ///// Get sum of status 1 photon and electrons in dR < 0.1 to matched el 
      double PtSum = 0;
      Particle X = truth_lep;
      vector<Gen> MatchedStatus1;
      int nSt1el(0);
      for(unsigned int i=2; i < All_Gens.size(); i++){
	if(TruthIdx == int(i)) continue;
	if(fabs(All_Gens.at(TruthIdx).Eta()-All_Gens.at(i).Eta())>0.1) continue;
	if(All_Gens.at(TruthIdx).DeltaPhi(All_Gens.at(i))>0.3) continue;
	if(All_Gens[TruthIdx].MotherIndex() == All_Gens[i].MotherIndex()){
	  if(All_Gens[i].Status() == 1) {
	    MatchedStatus1.push_back(All_Gens[i]);
	    PtSum+= All_Gens[i].Pt() ;
	    X=X+All_Gens[i];
	    if(fabs(All_Gens[i].PID()) == 11) nSt1el++;
	  }
	}
      }

      
      /// if nSt1el (number of status 1 electrons in dR < 0.4 from same mother , if > 1 e->eee  that passes JH Gen code
      bool bIsConv  =  HasPromptConv(iel);
      
      /// LeptonIsCF() == IsCF(iel, All_Gens, true);
      bool bIsCF     =  iel.LeptonIsCF() ;// IsCF(iel, All_Gens, true);
      
      double gen_d_pt = ((truth_lep.Pt()+PtSum) > 500) ? 500 : truth_lep.Pt() + PtSum;
      
      if(gen_d_pt > 499) continue;

      double gen_d_eta = X.Eta();
      double gen_d_phi = X.Phi();

      /// PtResponse measures how RECO pt compares to GEN pt
      double PtResponse  = (iel.Pt()  -  gen_d_pt) / gen_d_pt;
      double EtaResponse  = (iel.Eta()  -  gen_d_eta) / gen_d_eta;
      double PhiResponse  = (iel.Phi()  -  gen_d_phi) / gen_d_phi;


      TString EtaBin = (iel.IsBB()) ? "BB" : "EC";
      


      //double rate_cf= cfEst->GetElectronCFRate(param.Electron_Tight_ID, param.Electron_CF_Key, iel.defEta(), iel.Pt(), 0);
      
      if(bIsConv) continue;
      
      if(iel.IsPrompt() && !bIsCF){
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt_Prompt", PtResponse ,   EvWeight , 200, -0.5,0.5);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Eta_Prompt", EtaResponse , EvWeight , 200, -0.1,0.1);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Phi_Prompt", PhiResponse , EvWeight , 200,  -0.1,0.1);

	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_SmearedPhi1_Prompt", PhiResponse + gRandom->Gaus(0,0.005) , EvWeight , 200,  -0.1,0.1);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_SmearedPhi2_Prompt", PhiResponse + gRandom->Gaus(0,0.002) , EvWeight , 200,  -0.1,0.1);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_SmearedPhi3_Prompt", PhiResponse + gRandom->Gaus(0,0.001) , EvWeight , 200,  -0.1,0.1);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_SmearedPhi4_Prompt", PhiResponse + gRandom->Gaus(0,0.0005) , EvWeight , 200,  -0.1,0.1);
	
      }
      if(iel.IsPrompt() &&  bIsCF){
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt_CF",  PtResponse , EvWeight , 200, -1,1);
        FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Eta_CF", EtaResponse , EvWeight , 200, -0.1,0.1);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Phi_CF", PhiResponse , EvWeight , 200,  -0.1,0.1);
      }

      TString PtBinLabel =  "";
      if(iel.IsEC()){
	if(iel.Pt() < 50)       PtBinLabel = "_PtBin1";
        else  if(iel.Pt() < 200)PtBinLabel = "_PtBin2";
	else PtBinLabel = "_PtBin3";
      }
      else{
	if(iel.Pt()< 50)   PtBinLabel = "_PtBin1";
        else PtBinLabel = "_PtBin2";
      }

      
      TString PtBinLabel2  =  ""; 
      if(iel.IsBB()){
	if(iel.Pt() < 50)       PtBinLabel2 = "_Pt_BB_Bin1";
	else  if(iel.Pt() < 100)PtBinLabel2 = "_Pt_BB_Bin2";
	else  if(iel.Pt() < 200)PtBinLabel2 = "_Pt_BB_Bin3";
	else  if(iel.Pt() < 500)PtBinLabel2 = "_Pt_BB_Bin4";
	else PtBinLabel2 = "_Pt_BB_Bin5";
      }
      else{

	if(iel.Pt() < 30)       PtBinLabel2 = "_Pt_EC_Bin1";
	else if(iel.Pt() < 50)  PtBinLabel2 = "_Pt_EC_Bin2";
	else if(iel.Pt() < 75)  PtBinLabel2 = "_Pt_EC_Bin3";
	else  if(iel.Pt() < 100)PtBinLabel2 = "_Pt_EC_Bin4";
	else  if(iel.Pt() < 200)PtBinLabel2 = "_Pt_EC_Bin5";
	else  if(iel.Pt() < 500)PtBinLabel2 = "_Pt_EC_Bin6";
	else PtBinLabel2 = "_Pt_EC_Bin7";
      }



      
      if(iel.IsPrompt() &&  bIsCF) {
        FillHist(param.Name+"/Pt/"+EtaBin+"_CF", iel.Pt() , EvWeight , 200, 0,2000);

	FillHist(param.Name+"/EnergyShift/"+EtaBin+PtBinLabel+"_CF", PtResponse , EvWeight , 200, -1,1);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+PtBinLabel2+"_CF", PtResponse , EvWeight , 200, -1,1);

	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_CF", PtResponse , EvWeight , 200, -1,1);
      }

      
      /// Check Results in GetShiftCFEl

      if(iel.IsPrompt() &&  !bIsCF ) {
	
        FillHist(param.Name+"/Pt/"+EtaBin+"_Prompt", iel.Pt() , EvWeight , 200, 0,2000);

	double shiftEl = GetShiftCFEl(iel,param.Electron_Tight_ID);
	double shiftElMean = GetShiftCFEl(iel,param.Electron_Tight_ID,"Mean");
	double PtResponseShift = (shiftEl*iel.Pt() - gen_d_pt) / (gen_d_pt);
	double PtResponseShiftMean = (shiftElMean*iel.Pt() - gen_d_pt) / (gen_d_pt);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_PromptShifted_GetShiftCFEl", PtResponseShift , EvWeight , 200, -1,1);
	FillHist(param.Name+"/EnergyShift/"+EtaBin+PtBinLabel2+"_PromptShifted_GetShiftCFEl", PtResponseShift , EvWeight , 200, -1,1);
	
	FillHist(param.Name+"/EnergyShift/"+EtaBin+"_PromptShifted_GetShiftMeanCFEl", PtResponseShiftMean , EvWeight , 200, -1,1);
        FillHist(param.Name+"/EnergyShift/"+EtaBin+PtBinLabel2+"_PromptShifted_GetShiftMeanCFEl", PtResponseShiftMean , EvWeight , 200, -1,1);

	unsigned int nBins = 100;
	if(iel.Pt() < 50)  nBins = 200;
	
	for (unsigned int ishift = 0 ; ishift < nBins; ishift++){
	  double shiftEl = 1.05 - double(ishift)*0.001;
	  TString shift_string = DoubleToString(shiftEl);
	  double PtResponseShift = (shiftEl*iel.Pt() - gen_d_pt) / (gen_d_pt);
	  FillHist(param.Name+"/EnergyShift/"+EtaBin+PtBinLabel+"_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	  FillHist(param.Name+"/EnergyShift/"+EtaBin+PtBinLabel2+"_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	  FillHist(param.Name+"/EnergyShift/"+EtaBin+"_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	  
	}
      } /// Energy Shift
    }
    
    return;
  }

  
  if(HasFlag("ScaleFactor")){

    FillHist(param.Name+"/ZGSub/All",  1    , EvWeight ,2., 0, 2, "");

    int nbinNeg                = 17;
    double pTbinNeg[nbinNeg+1] = {-200,-100,-80,-50,-45,-40,-35,-30,-15, 15.,30.,35,40.,45, 50., 80.,100.,200.};

    if (!PassTriggerSelection(EE, ev,   Leptons,"Dilep")) return ;

    if(Leptons.size() != 2) return;
    if(ElectronColl.size() != 2) return;
    
    Particle ZCand  = ElectronColl.at(0)+ElectronColl.at(1);
    double MllLeft  = 50;
    double MllRight = 150;
    int NBin = 50;
    if(! (MllLeft<=ZCand.M()&&ZCand.M()<MllRight) ) return;

    if(!IsDATA) {
      
      if(MCSample.Contains("DY")){
	if(ElectronColl.at(0).LeptonGenType() <= 0 || ElectronColl.at(0).LeptonGenType() >=4) return;
	if(ElectronColl.at(1).LeptonGenType() <= 0 || ElectronColl.at(1).LeptonGenType() >=4) return;
	
	bool IsCF1 = ElectronColl.at(0).LeptonIsCF();
	bool IsCF2 = ElectronColl.at(1).LeptonIsCF();
	
	if(HasPromptConv(ElectronColl.at(0))) return;
	if(HasPromptConv(ElectronColl.at(1))) return;
	
	if(SameCharge(ElectronColl)){
	  if(!IsCF1 && !IsCF2) return;
	}
      }
      else{
	/// ZG Subtraction
        FillHist(param.Name+"/ZGSub/SS",  1    , EvWeight ,2., 0, 2, "");

	if(! (ElectronColl.at(0).IsConv() ||  ElectronColl.at(1).IsConv() )) return;
	EvWeight = -1*EvWeight;
        FillHist(param.Name+"/ZGSub/SS_noconv",  1    , EvWeight ,2., 0, 2, "");

      }
    }

    vector<Electron> ElectronColl_shifted;// = ElectronColl; // copy the vector                                                                                                              

    Electron this_el1 = ElectronColl.at(0);
    Electron this_el2 = ElectronColl.at(1);

    if(abs(ElectronColl.at(0).scEta())<1.4442&&abs(ElectronColl.at(1).scEta())<1.4442){
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()>0){
        FillHist(param.Name+"/ScaleFactor/BB_ZMass_SS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      }
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
        FillHist(param.Name+"/ScaleFactor/BB_ZMass_OS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      } //JH : if you want to check OS distribution before applying the CF weight
    }
    // BE
    if((abs(ElectronColl.at(0).scEta())<1.4442&&abs(ElectronColl.at(1).scEta())>=1.556)||(abs(ElectronColl.at(0).scEta())>=1.556&&abs(ElectronColl.at(1).scEta())<1.4442)){
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()>0){
        FillHist(param.Name+"/ScaleFactor/BE_ZMass_SS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      }
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
        FillHist(param.Name+"/ScaleFactor/BE_ZMass_OS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      }
    }
    // EE
    if(abs(ElectronColl.at(0).scEta())>=1.556&&abs(ElectronColl.at(1).scEta())>=1.556){
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()>0){
        FillHist(param.Name+"/ScaleFactor/EE_ZMass_SS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      }
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
        FillHist(param.Name+"/ScaleFactor/EE_ZMass_OS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      }
    }
    
  
    Electron unshifted_el1 = ElectronColl.at(0);
    Electron unshifted_el2 = ElectronColl.at(1);


    for(unsigned int ishift = 0 ; ishift < 9 ; ishift++){

      Electron this_El1 = ElectronColl.at(0);
      Electron this_El2 = ElectronColl.at(1);

      double shiftEl1 = GetShiftCFEl(this_El1,param.Electron_Tight_ID);
      double shiftEl2 = GetShiftCFEl(this_El2,param.Electron_Tight_ID);
      double shiftEl1p = shiftEl1 * 1.02;
      double shiftEl1m = shiftEl1 * 0.98;
      double shiftEl2p = shiftEl2 * 1.02;
      double shiftEl2m = shiftEl2 * 0.98;

      TString shiftstring = "";
      if(ishift < 2) shiftstring = "PTB";
      else       if(ishift< 4) shiftstring = "PTBmSig";
      else       if(ishift< 6) shiftstring = "PTBpSig";
      else if(ishift==6)  shiftstring = "CS_1p3";
      else if(ishift==7)  shiftstring = "CS_3p0";
      else if(ishift==8)  shiftstring = "CS_4p0";

      if(ishift==0)  this_El1*= shiftEl1;
      else if(ishift==1)  this_El2*= shiftEl2;
      else if(ishift==2)  this_El1*= shiftEl1m;
      else if(ishift==3)  this_El2*= shiftEl2m;
      else if(ishift==4)  this_El1*= shiftEl1p;
      else if(ishift==5)  this_El2*= shiftEl2p;

      else if(ishift==6) {
	this_El1*=  0.987;
	this_El2*=  0.987;
      }
      else if(ishift==7) {
	this_El1*=  0.975;
	this_El2*=  0.975;
      } 
      else if(ishift==8) {
	this_El1*=  0.96;
        this_El2*=  0.96;
      }

      if(_jentry>=5500 && _jentry<=6000) cout << "El 1 " << unshifted_el1.Pt () << " eta  = " << unshifted_el1.Eta() << " shifted pt = " << this_El1.Pt()<< endl;
      if(_jentry>=5500 && _jentry<=6000) cout << "El 2 " << unshifted_el2.Pt () << " eta  = " << unshifted_el2.Eta() <<   " shifted pt = " << this_El2.Pt()<< endl;
      ElectronColl_shifted.push_back(this_El1);
      ElectronColl_shifted.push_back(this_El2);
      
      Particle ZCand_shifted = ElectronColl_shifted.at(0) + ElectronColl_shifted.at(1);
      Particle ZCand_unshifted = unshifted_el1 + unshifted_el2;

      TString CFKey = param.Electron_Tight_ID;
      if(CFKey =="passPOGTight") CFKey="POGTight";

      //      double rate_cf_inv_eta_pbs       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta_PBS_" +CFKey,iel.defEta(), iel.Pt(), 0);

      double weight_Closure=1;
      double weight_ClosureSF=1;
      
      double rate_cf1       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta3_CS_" +CFKey,unshifted_el1.defEta(), unshifted_el1.Pt(), 0);
      double rate_cf2       = cfEst->GetElectronCFRate(param.Electron_Tight_ID, "CFRate_InvPtEta3_CS_" +CFKey,unshifted_el2.defEta(), unshifted_el2.Pt(),0);

      if(_jentry>=5500 && _jentry<=6000) cout << ishift << " rate_cf1 = " << rate_cf1 << " rate_cf2=" << rate_cf2 << endl;
      if(ishift==0 || ishift==2 || ishift==4) {
	weight_Closure= ReturnCFWeight(rate_cf1);
	weight_ClosureSF = ReturnCFWeight(rate_cf1 * GetCFSF(param, Lepton(unshifted_el1)));
      }
      else if(ishift==1 || ishift==3 || ishift==5) {
        weight_Closure= ReturnCFWeight(rate_cf2);
        weight_ClosureSF = ReturnCFWeight(rate_cf2 * GetCFSF(param, Lepton(unshifted_el2)));

      }
      else{

        weight_Closure     = ReturnCFWeight({rate_cf1,rate_cf2});
        weight_ClosureSF   = ReturnCFWeight({rate_cf1 * GetCFSF(param, Lepton(unshifted_el1)),rate_cf2 * GetCFSF(param, Lepton(unshifted_el1))});
	
      }
      weight_Closure *= EvWeight;
      weight_ClosureSF *= EvWeight;

     
      if(_jentry>=5500 && _jentry<=6000) cout << ishift << " weight_Closure = " << weight_Closure << " weight_shiftedSF = " << weight_ClosureSF << endl;


      if(HasFlag("RunFakes")){
	//weight_shifted = GetFakeWeight(EL_Leptons, param);
	weight_ClosureSF = weight_Closure;
      }
      else       if(HasFlag("RunBkg")){
        weight_Closure = EvWeight;
	weight_ClosureSF = weight_Closure;
      }
      
      if(MllLeft<=ZCand_shifted.M()&&ZCand_shifted.M()<MllRight){
	if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
	  
	  // BB
	  if(abs(ElectronColl_shifted.at(0).scEta())<1.4442&&abs(ElectronColl_shifted.at(1).scEta())<1.4442){
	    FillHist(param.Name+"/ScaleFactor/BB_ZMass_OS_CF_"+shiftstring+"weighted", ZCand_shifted.M(),           weight_Closure, NBin, MllLeft, MllRight);
	    FillHist(param.Name+"/ScaleFactor/BB_ZMass_OS_CF_"+shiftstring+"SFweighted", ZCand_shifted.M(),         weight_ClosureSF, NBin, MllLeft, MllRight);
	  }
  
	  // BE
	  if((abs(ElectronColl_shifted.at(0).scEta())<1.4442&&abs(ElectronColl_shifted.at(1).scEta())>=1.556)||(abs(ElectronColl_shifted.at(0).scEta())>=1.556&&abs(ElectronColl_shifted.at(1).scEta())<1.4442)){
	    FillHist(param.Name+"/ScaleFactor/BE_ZMass_OS_CF_"+shiftstring+"weighted", ZCand_shifted.M(),         weight_Closure, NBin, MllLeft, MllRight);
	    FillHist(param.Name+"/ScaleFactor/BE_ZMass_OS_CF_"+shiftstring+"SFweighted", ZCand_shifted.M(),       weight_ClosureSF, NBin, MllLeft, MllRight);
	  }
	  
	  // EE
	  if(abs(ElectronColl_shifted.at(0).scEta())>=1.556&&abs(ElectronColl_shifted.at(1).scEta())>=1.556){
	    FillHist(param.Name+"/ScaleFactor/EE_ZMass_OS_CF_"+shiftstring+"weighted", ZCand_shifted.M(),  weight_Closure, NBin, MllLeft, MllRight);
            FillHist(param.Name+"/ScaleFactor/EE_ZMass_OS_CF_"+shiftstring+"SFweighted", ZCand_shifted.M(), weight_ClosureSF, NBin, MllLeft, MllRight);
	    
	  }
	}
      }
      if(ishift < 2){
	
	if(MllLeft<=ZCand_unshifted.M()&&ZCand_unshifted.M()<MllRight){
	  if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
	    
	    // BB                                                                                                                                                                                                                                               
	    if(abs(ElectronColl.at(0).scEta())<1.4442&&abs(ElectronColl.at(1).scEta())<1.4442){
	      FillHist(param.Name+"/ScaleFactor/BB_ZMass_OS_CFweighted_unshifted", ZCand_unshifted.M(),         weight_Closure, NBin, MllLeft, MllRight);
	      FillHist(param.Name+"/ScaleFactor/BB_ZMass_OS_CFweighted_unshiftedSF", ZCand_unshifted.M(),       weight_ClosureSF, NBin, MllLeft, MllRight);
	    }
	    
	    // EE                                                                                                                                                                                                                                               
	    if(abs(ElectronColl.at(0).scEta())>=1.556&&abs(ElectronColl.at(1).scEta())>=1.556){
	      FillHist(param.Name+"/ScaleFactor/EE_ZMass_OS_CFweighted_unshifted", ZCand_unshifted.M(), weight_Closure, NBin, MllLeft, MllRight);
	      FillHist(param.Name+"/ScaleFactor/EE_ZMass_OS_CFweighted_unshiftedSF", ZCand_unshifted.M(), weight_ClosureSF, NBin, MllLeft, MllRight);
      
	    }
	  }
        } // OS
      }// unsifted
    }// shift
  }
}

HNL_Lepton_ChargeFlip::HNL_Lepton_ChargeFlip(){

}

HNL_Lepton_ChargeFlip::~HNL_Lepton_ChargeFlip(){

}
