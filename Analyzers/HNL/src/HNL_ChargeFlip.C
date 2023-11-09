#include "HNL_ChargeFlip.h"

void HNL_ChargeFlip::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  
  /// 5 MODES
  ///
  // HasFlag("MuonCF") 
  // HasFlag("ShiftEnergy")  ---> Step 1 measure Pt response of CF vs Prompt Electron to Get Pt/Eta dep shift  
  // HasFlag("ElCFRates")    ---> Measure rates in DY/Top MC (using Pt CF response)
  // HasFlag("ShiftEnergyZ") ---> Check mc closure  (Using only DY)
  // HasFlag("ScaleFactor")  ---> Compare Data SS vs OS weighted in BB / EC (using CF Rates) 
  
  //  cfEst->IgnoreNoHist = true;

  //  if(HasFlag("ShiftEnergy"))cfEst->IgnoreNoHist = true;
  
}

void HNL_ChargeFlip::executeEvent(){
  
  
  vector<TString> LepIDs = {"HNL_ULID_"+GetYearString(),"HNTightV2","TopHN", "POGTight"};
  vector<TString> CFMethods ={};
  //"CFRate_PtInv","CFRate_Pt"};    
  //if(HasFlag("ShiftEnergy")) LepIDs = { "POGTight"};

  if(HasFlag("ElCFRates") || HasFlag("ShiftEnergy")){
    CFMethods.push_back("");
  }
  else{
    CFMethods ={"CFRate_PtInv","CFRate_Pt"};
  }
  for (auto id: LepIDs){
    for (auto CFMethod : CFMethods){
      AnalyzerParameter param = InitialiseHNLParameter("SignalStudy");
      param.Name    =  id+CFMethod;
      param.DefName =  id+CFMethod;
      
      //// Do not include Xsec norm since looking to Add TT/DY samples in Eff
      param.Apply_Weight_LumiNorm = false;
      if(HasFlag("ScaleFactor"))param.Apply_Weight_LumiNorm = true;
      param.Apply_Weight_SumQ     = true;
      param.Apply_Weight_PileUp   = true;
      param.Apply_Weight_PreFire  = true;
      param.Apply_Weight_kFactor  = false;
      param.Apply_Weight_IDSF     = true;
      param.Apply_Weight_RECOSF     = true;
      param.Apply_Weight_TriggerSF= true;
      
      param.FakeMethod   = "DATA";
      param.CFMethod     = "DATA";
      param.ConvMethod   = "MC";
      
      param.Electron_Tight_ID = (id == "TopHN") ? "TopHNSST" :  id;
      param.Muon_Tight_ID     = (id == "TopHN") ? "TopHNT"   :  id;
      param.k.Electron_CF  = CFMethod+"_EtaRegion_"+id;
      if(id =="POGTight"){
	param.Muon_Tight_ID   =    "POGTightWithTightIso";
	param.Electron_Tight_ID  = "passPOGTight";
	param.k.Electron_CF  = CFMethod+"_EtaRegion_POGTight";
      }
      executeEventFromParameter(param);
    }
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

void HNL_ChargeFlip::executeEventFromParameter(AnalyzerParameter param){

  if(_jentry==0) param.PrintParameters();
  run_Debug = (_jentry%nLog==0);

  if(!PassMETFilter()) return;

 
  Event ev = GetEvent();

  double EvWeight =SetupWeight(ev,param);
  
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

  vector<Electron> ElectronColl = ElectronPromptOnly(ElectronColltmp,All_Gens,param);


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
    
    if(SameCharge(ElectronColl)){
      if(ElectronColl[0].LeptonIsCF() || ElectronColl[1].LeptonIsCF()){
	double el1CF = ElectronColl[0].HNL_MVA_CF("EDv5");
	double el2CF = ElectronColl[1].HNL_MVA_CF("EDv5");
	
	if(el1CF < el2CF) {
	  if(ElectronColl[0].LeptonIsCF())FillHist(param.Name+"/CFMVATEST/CheckTagger", 1, EvWeight,5, 0, 5);
	  else  FillHist(param.Name+"/CFMVATEST/CheckTagger", 2, EvWeight,5, 0, 5);
	}
	else{
	  if(ElectronColl[1].LeptonIsCF())FillHist(param.Name+"/CFMVATEST/CheckTagger", 1, EvWeight,5, 0, 5);
          else FillHist(param.Name+"/CFMVATEST/CheckTagger", 2, EvWeight,5, 0, 5);

	}
      }
    }

    
    double invptbins[14] = { 0., 0.002,0.005, 0.0075,0.010,0.0125,0.015,0.020,0.025, 0.030,0.035, 0.040,0.05, 0.1};
    double etabins   [11] = { 0.,0.8,1.2,  1.4442,1.556, 1.9, 2.1,2.2,2.3,2.4 , 2.5};
    
    int nbins_invpt(13);

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
      
      double PtShift = 1. / GetShiftCFEl(ElectronColl.at(i));

      if(HasFlag("RateNoShift")) PtShift = 1.;
      
      if(abs(ElectronColl.at(i).scEta())<0.8){
	FillHist(param.Name+"/CFrate/EtaRegion1_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion1_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(0.8<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<1.2){
	FillHist(param.Name+"/CFrate/EtaRegion2_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion2_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(1.2<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<1.4442){
        FillHist(param.Name+"/CFrate/EtaRegion3_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
        if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion3_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(1.556<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<1.9){
	FillHist(param.Name+"/CFrate/EtaRegion4_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion4_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(1.9<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<2.1){
	FillHist(param.Name+"/CFrate/EtaRegion5_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
	if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion5_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(2.1<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<2.2){
        FillHist(param.Name+"/CFrate/EtaRegion6_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
        if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion6_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(2.2<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<2.3){
        FillHist(param.Name+"/CFrate/EtaRegion7_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
        if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion7_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(2.3<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<2.4){
        FillHist(param.Name+"/CFrate/EtaRegion8_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
        if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion8_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }
      else if(2.4<=abs(ElectronColl.at(i).scEta())&&abs(ElectronColl.at(i).scEta())<2.5){
        FillHist(param.Name+"/CFrate/EtaRegion9_Denom",ElectronColl.at(i).InvPt(), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
        if(IsCF)  FillHist(param.Name+"/CFrate/EtaRegion9_Num",ElectronColl.at(i).InvPt(PtShift), EvWeight, nbins_invpt, invptbins,"#pT^{-1}");
      }


      FillHist(param.Name+"/CFrate_Eta/Denom", ElectronColl.at(i).scEta(), EvWeight, 50, -2.5, 2.5,"scEta");
      FillHist(param.Name+"/CFrate_IP/Denom", ElectronColl.at(i).dXY(), EvWeight, 200, -0.1, 0.1,"dXY");
      FillHist(param.Name+"/CFrate_2D/Denom",ElectronColl.at(i).InvPt(), ElectronColl.at(i).scEta(), EvWeight, nbins_invpt, invptbins, 10,etabins);
      if(IsCF) {
	FillHist(param.Name+"/CFrate_Eta/Num", ElectronColl.at(i).scEta(), EvWeight, 50, -2.5, 2.5);
	FillHist(param.Name+"/CFrate_IP/Num", ElectronColl.at(i).dXY(), EvWeight, 200, -0.1, 0.1);
	FillHist(param.Name+"/CFrate_2D/Num",ElectronColl.at(i).InvPt(PtShift), ElectronColl.at(i).scEta(), EvWeight, nbins_invpt, invptbins, 10,etabins);
      }
      
      double pTbin[13] = {0.,20.,30.,35., 40.,45., 50.,65., 80.,  100., 150., 200.,500.};
      int nbin = 12;//pTbin.size()-1;
      
      if(abs(ElectronColl.at(i).scEta())<0.8){
	FillHist(param.Name+"/CFratePt/EtaRegion1_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
	if(IsCF)   FillHist(param.Name+"/CFratePt/EtaRegion1_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }
      else if(0.8<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<1.2){
	FillHist(param.Name+"/CFratePt/EtaRegion2_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
	if(IsCF)   FillHist(param.Name+"/CFratePt/EtaRegion2_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }
      else if(1.2<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<1.4442){
        FillHist(param.Name+"/CFratePt/EtaRegion3_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
        if(IsCF)   FillHist(param.Name+"/CFratePt/EtaRegion3_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }

      else if(1.556<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<1.9){
	FillHist(param.Name+"/CFratePt/EtaRegion4_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
	if(IsCF) FillHist(param.Name+"/CFratePt/EtaRegion4_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }
      else if(1.9<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<2.1){
	FillHist(param.Name+"/CFratePt/EtaRegion5_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
	if(IsCF) FillHist(param.Name+"/CFratePt/EtaRegion5_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }
      else if(2.1<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<2.2){
        FillHist(param.Name+"/CFratePt/EtaRegion6_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
        if(IsCF) FillHist(param.Name+"/CFratePt/EtaRegion6_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }
      else if(2.2<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<2.3){
        FillHist(param.Name+"/CFratePt/EtaRegion7_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
        if(IsCF) FillHist(param.Name+"/CFratePt/EtaRegion7_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }
      else if(2.3<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<2.4){
        FillHist(param.Name+"/CFratePt/EtaRegion8_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
        if(IsCF) FillHist(param.Name+"/CFratePt/EtaRegion8_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }
      else if(2.4<=abs(ElectronColl.at(i).scEta()) && abs(ElectronColl.at(i).scEta())<2.5){
        FillHist(param.Name+"/CFratePt/EtaRegion9_Denom", ElectronColl.at(i).PtMaxed(500), EvWeight,nbin, pTbin,"pT");
        if(IsCF) FillHist(param.Name+"/CFratePt/EtaRegion9_Num", ElectronColl.at(i).PtMaxed(500)*PtShift, EvWeight,nbin, pTbin,"pT");
      }

      FillHist(param.Name+"/CFrate_2DPt/Denom",ElectronColl.at(i).PtMaxed(500), ElectronColl.at(i).scEta(), EvWeight, nbin, pTbin, 10,etabins);
      if(IsCF)     FillHist(param.Name+"/CFrate_2DPt/Num",ElectronColl.at(i).PtMaxed(500)*PtShift, ElectronColl.at(i).scEta(), EvWeight, nbin, pTbin, 10,etabins);
      
    }
    return;
  }


  
  if(HasFlag("ShiftEnergyZ")){
    
    int nbinNeg= 17;
    double pTbinNeg[nbinNeg+1] = {-200,-100,-80,-50,-45,-40,-35,-30,-15, 15.,30.,35,40.,45, 50., 80.,100.,200.};

    if(run_Debug) cout << "ShiftEnergyZ processing " << endl;

    if(ElectronColl.size() != 2) return;

    //// Select SS events in MC that are 1 Prompt + 1 CF
    if(SameCharge(ElectronColl)){
      if(ElectronColl.at(0).IsPrompt() && ElectronColl.at(1).IsPrompt() && (ElectronColl.at(0).LeptonIsCF() || ElectronColl.at(1).LeptonIsCF())){

	/// Remove rare converstions  where e->eee share SC using HasPromptConv
	if(!HasPromptConv(ElectronColl.at(0)) && !HasPromptConv(ElectronColl.at(1))) {
	  
	  // To be safe remove LeptonGenType() > 4 which are conversions
	  if(ElectronColl.at(0).LeptonGenType() < 4 &&  ElectronColl.at(1).LeptonGenType() <4) {
	    
	    // Construct LL candidate
	    Particle ZCand = ElectronColl.at(0)+ElectronColl.at(1);
	    double Zmass = ZCand.M(); 
	    if(Zmass < 70) Zmass=70;
	    if(Zmass > 130) Zmass=130;

	    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB())  FillHist(param.Name+"/ZMass/SS_BB", Zmass , EvWeight ,70 , 130, 30);
	    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC())  FillHist(param.Name+"/ZMass/SS_EC", Zmass , EvWeight ,70 , 130, 30);
	    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB()) {
	      FillHist(param.Name+"/Z/SS_BB_LepPt",  ElectronColl[0].PtMaxed(200) ,         EvWeight , nbinNeg, pTbinNeg,"pT");
	      FillHist(param.Name+"/Z/SS_BB_LepPt", -1*ElectronColl[1].PtMaxed(200) ,      EvWeight , nbinNeg, pTbinNeg,"pT");
	      FillHist(param.Name+"/Z/SS_BB_LepEta",  (ElectronColl[0].Eta()+2.5) , EvWeight, 100., -5., 5., "Eta");
	      FillHist(param.Name+"/Z/SS_BB_LepEta",  -1*(ElectronColl[1].Eta()+2.5) , EvWeight, 100., -5., 5., "Eta");

	    }
	    
	    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC()) {
              FillHist(param.Name+"/Z/SS_EC_LepPt", ElectronColl[0].PtMaxed(200) ,         EvWeight , nbinNeg, pTbinNeg,"pT");
              FillHist(param.Name+"/Z/SS_EC_LepPt", -1*ElectronColl[1].PtMaxed(200) ,      EvWeight , nbinNeg, pTbinNeg,"pT");
	      FillHist(param.Name+"/Z/SS_EC_LepEta",  (ElectronColl[0].Eta()+2.5) , EvWeight, 100., -5., 5., "Eta");
	      FillHist(param.Name+"/Z/SS_EC_LepEta",  -1*(ElectronColl[1].Eta()+2.5) , EvWeight, 100., -5., 5., "Eta");

	    } // EC
	  } /// Non Conv
	} 
      } //// END OF Prompt + CF
    }
    else {

      //// Now plot OS * CF rate [Energy Shifted] to compare
      if(ElectronColl.at(0).IsPrompt() && !ElectronColl.at(0).LeptonIsCF()  && ElectronColl.at(1).IsPrompt() && !ElectronColl.at(0).LeptonIsCF() ){

	if(HasPromptConv(ElectronColl.at(0))  || HasPromptConv(ElectronColl.at(1))) return;
	if(ElectronColl.at(0).LeptonGenType() >= 4 || ElectronColl.at(1).LeptonGenType() >=4) return;
	

	//// Loop several Energy shift
	for (unsigned int ishift = 0 ; ishift < 1; ishift++){
	  
	  
	  vector<Electron> ShiftedEl;
          Electron this_el1 = ElectronColl.at(0);
          Electron this_el2 = ElectronColl.at(1);

	  double shiftEl1 = GetShiftCFEl(this_el1);
	  double shiftEl2 = GetShiftCFEl(this_el2);
	  TString shift_string = "SingleElShift";
	  
	  if(this_el1.IsBB() && !this_el2.IsBB())        this_el2*= shiftEl2;
	  else if(this_el2.IsBB() &&!this_el1.IsBB())    this_el1*= shiftEl1;
	  else if((this_el1.Pt() - this_el2.Pt() ) > 50) this_el1*= shiftEl1;
	  else if(this_el1.HNL_MVA_CF("EDv5") < this_el2.HNL_MVA_CF("EDv5")) this_el1*= shiftEl1;
	  else this_el2*= shiftEl2;
	  
	  Particle ZShiftedCand = this_el1+this_el2;
	  double ZShiftedmass = ZShiftedCand.M();
	  if(ZShiftedmass < 70) ZShiftedmass=70;
	  if(ZShiftedmass > 130) ZShiftedmass=130;

          if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB())FillHist(param.Name+"/ZMass/OS_BB_EnergyShift_"+shift_string, ZShiftedmass , EvWeight ,70 , 130, 30);
          if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC())FillHist(param.Name+"/ZMass/OS_EC_EnergyShift_"+shift_string, ZShiftedmass , EvWeight ,70 , 130, 30);
	  
	  ShiftedEl.push_back(this_el1);
	  ShiftedEl.push_back(this_el2);
	  double weight_shifted = GetCFWeightElectron(ShiftedEl, param , false);
          if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB()){
	    FillHist(param.Name+"/ZMass/OS_BB_Weighted_EnergyShift_"+shift_string,  ZShiftedmass             , weight_shifted*EvWeight ,70 , 130, 30);
	    FillHist(param.Name+"/Z/OS_BB_Weighted_EnergyShift_Pt_"+shift_string,   this_el1.PtMaxed(200)    , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
	    FillHist(param.Name+"/Z/OS_BB_Weighted_EnergyShift_Pt_"+shift_string,   -1*this_el2.PtMaxed(200) , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
            FillHist(param.Name+"/Z/OS_BB_Weighted_EnergyShift_Eta_"+shift_string,  (this_el1.Eta()+2.5)     , weight_shifted*EvWeight ,100., -5, 5, "Eta");
            FillHist(param.Name+"/Z/OS_BB_Weighted_EnergyShift_Eta_"+shift_string,  -1*(this_el2.Eta()+2.5)  , weight_shifted*EvWeight ,100., -5, 5, "Eta");

	  }
          if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC()){
	    FillHist(param.Name+"/ZMass/OS_EC_Weighted_EnergyShift_"+shift_string, ZShiftedmass            , weight_shifted*EvWeight ,70 , 130, 30);
            FillHist(param.Name+"/Z/OS_EC_Weighted_EnergyShift_Pt_"+shift_string,  this_el1.PtMaxed(200)   , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
            FillHist(param.Name+"/Z/OS_EC_Weighted_EnergyShift_Pt_"+shift_string, -1*this_el2.PtMaxed(200) , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
            FillHist(param.Name+"/Z/OS_EC_Weighted_EnergyShift_Eta_"+shift_string, (this_el1.Eta()+2.5)    , weight_shifted*EvWeight ,100., -5, 5, "Eta");
            FillHist(param.Name+"/Z/OS_EC_Weighted_EnergyShift_Eta_"+shift_string, -1*(this_el2.Eta()+2.5) , weight_shifted*EvWeight ,100., -5, 5, "Eta");
	  }
	}

	for (unsigned int ishift = 0 ; ishift < 101; ishift++){

          double shiftEl = 1.0 - double(ishift)*0.001;
          TString shift_string = DoubleToString(shiftEl);
          vector<Electron> ShiftedEl;
          Electron this_el1 = ElectronColl.at(0);
          Electron this_el2 = ElectronColl.at(1);

	  if(ishift <50){
	    this_el1*= shiftEl;
	    this_el2*= shiftEl;
	  }
	  else   if(ishift <100){
	    shiftEl = 1.0 - double(ishift-50)*0.001;
	    shift_string = DoubleToString(shiftEl)+"_sloped";

	    double ShiftSlope  = (1-(shiftEl)) / (250-15);
	    double ShiftFinal1 = std::max( shiftEl, std::min(1. , shiftEl + ShiftSlope*(this_el1.Pt()-15.)) );
	    double ShiftFinal2 = std::max( shiftEl, std::min(1. , shiftEl + ShiftSlope*(this_el2.Pt()-15.)) );

            this_el1*= ShiftFinal1;
            this_el2*= ShiftFinal2;
          }
	  else{
	    double shiftEl1 = GetShiftCFEl(this_el1);
	    double shiftEl2 = GetShiftCFEl(this_el2);
	    shiftEl = (shiftEl1+shiftEl2)/2.;
	    shift_string = "AverageSingleEl";
	    this_el1*= shiftEl;
            this_el2*= shiftEl;
	  }

	  Particle ZShiftedCand = this_el1+this_el2;
          double ZShiftedmass = ZShiftedCand.M();
          if(ZShiftedmass < 70) ZShiftedmass=70;
          if(ZShiftedmass > 130) ZShiftedmass=130;

          if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB())FillHist(param.Name+"/ZMass/OS_BB_ZEnergyShift_"+shift_string, ZShiftedmass , EvWeight ,70 , 130, 30);
          if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC())FillHist(param.Name+"/ZMass/OS_EC_ZEnergyShift_"+shift_string, ZShiftedmass , EvWeight ,70 , 130, 30);
	  
          ShiftedEl.push_back(this_el1);
          ShiftedEl.push_back(this_el2);
          double weight_shifted = GetCFWeightElectron(ShiftedEl, param , false);

          if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB()){
	    FillHist(param.Name+"/ZMass/OS_BB_Weighted_ZEnergyShift_"+shift_string, ZShiftedmass            , weight_shifted*EvWeight ,70 , 130, 30);
            FillHist(param.Name+"/Z/OS_BB_Weighted_ZEnergyShift_Pt_"+shift_string, this_el1.PtMaxed(200)    , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
            FillHist(param.Name+"/Z/OS_BB_Weighted_ZEnergyShift_Pt_"+shift_string, -1*this_el2.PtMaxed(200) , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
            FillHist(param.Name+"/Z/OS_BB_Weighted_ZEnergyShift_Eta_"+shift_string, (this_el1.Eta()+2.5)    , weight_shifted*EvWeight ,100., -5, 5, "Eta");
            FillHist(param.Name+"/Z/OS_BB_Weighted_ZEnergyShift_Eta_"+shift_string, -1*(this_el2.Eta()+2.5) , weight_shifted*EvWeight ,100., -5, 5, "Eta");

	  }
          if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC()){
	    FillHist(param.Name+"/ZMass/OS_EC_Weighted_ZEnergyShift_"+shift_string, ZShiftedmass            , weight_shifted*EvWeight ,70 , 130, 30);
            FillHist(param.Name+"/Z/OS_EC_Weighted_ZEnergyShift_Pt_"+shift_string, this_el1.PtMaxed(200)    , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
            FillHist(param.Name+"/Z/OS_EC_Weighted_ZEnergyShift_Pt_"+shift_string, -1*this_el2.PtMaxed(200) , weight_shifted*EvWeight , nbinNeg, pTbinNeg,"pT");
	    FillHist(param.Name+"/Z/OS_EC_Weighted_ZEnergyShift_Eta_"+shift_string, (this_el1.Eta()+2.5)    , weight_shifted*EvWeight ,100., -5, 5, "Eta");
            FillHist(param.Name+"/Z/OS_EC_Weighted_ZEnergyShift_Eta_"+shift_string, -1*(this_el2.Eta()+2.5) , weight_shifted*EvWeight ,100., -5, 5, "Eta");

	  }
        } /// Z shift
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
	    if(fabs(All_Gens[i].PID()) == 11) nSt1el++;
	  }
	}
      }

      /// if nSt1el (number of status 1 electrons in dR < 0.4 from same mother , if > 1 e->eee  that passes JH Gen code
      bool bIsConv  =  HasPromptConv(iel);
      
      /// LeptonIsCF() == IsCF(iel, All_Gens, true);
      bool bIsCF     =  iel.LeptonIsCF() ;// IsCF(iel, All_Gens, true);
      
      double gen_pt = ( (truth_lep.Pt()+PtSum) > 500) ? 500 : truth_lep.Pt() + PtSum;

      /// PtResponse measures how RECO pt compares to GEN pt
      double PtResponse  = (iel.Pt()  -  gen_pt) / gen_pt;


      TString EtaBin = iel.etaRegionString();//(iel.IsBB()) ? "BB" : "EC";
      
      //double rate_cf= cfEst->GetElectronCFRate(param.Electron_Tight_ID, param.Electron_CF_Key, iel.defEta(), iel.Pt(), 0);
      
      if(bIsConv) continue;
      
      if(iel.IsEC()){
	if(gen_pt < 30) {
	  if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt1_CF", PtResponse , EvWeight , 200, -1,1);
	}
	else  if(gen_pt < 50) {
	  if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt2_CF", PtResponse , EvWeight , 200, -1,1);
	}
	else  if(gen_pt< 75) {
	  if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt3_CF", PtResponse , EvWeight , 200, -1,1);
	}
	else  if(gen_pt< 100) {
	  if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt4_CF", PtResponse , EvWeight , 200, -1,1);
	} 
	else if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt5_CF", PtResponse , EvWeight , 200, -1,1);
      }
      else{
	if(gen_pt< 50) {
	  if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/BB_Pt1_CF", PtResponse , EvWeight , 200, -1,1);
        }
        else  if(gen_pt< 100) {
          if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/BB_Pt2_CF", PtResponse , EvWeight , 200, -1,1);
        }
        else if(iel.IsPrompt() &&  bIsCF) FillHist(param.Name+"/EnergyShift/BB_Pt3_CF", PtResponse , EvWeight , 200, -1,1);
      }

      for (unsigned int ishift = 0 ; ishift < 150; ishift++){
	double shiftEl = 1.05 - double(ishift)*0.001;
	TString shift_string = DoubleToString(shiftEl);
	double PtResponseShift = (shiftEl*iel.Pt() - gen_pt) / (gen_pt);
	if(iel.IsPrompt() &&  !bIsCF) {
	  if(iel.IsEC()){
	    if(gen_pt< 30)       FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt1_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	    else if(gen_pt< 50)  FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt2_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	    else if(gen_pt< 75)  FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt3_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	    else if(gen_pt< 100) FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt4_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	    else  FillHist(param.Name+"/EnergyShift/"+EtaBin+"_Pt5_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	  }
	  else{
	    if(gen_pt< 50)       FillHist(param.Name+"/EnergyShift/BB_Pt1_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
            else if(gen_pt< 100) FillHist(param.Name+"/EnergyShift/BB_Pt2_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
            else  FillHist(param.Name+"/EnergyShift/BB_Pt3_PromptShifted_"+shift_string, PtResponseShift , EvWeight , 200, -1,1);
	  }
	}
      }
    }
    return;
  }

  
  if(HasFlag("ScaleFactor")){

    double pTbinNeg[18] = {-200,-100,-80,-50,-45,-40,-35,-30,-15, 15.,30.,35,40.,45, 50., 80.,100.,200.};
    int nbinNeg         = 17;

    if (!PassTriggerSelection(EE, ev,   Leptons,"Dilep")) return ;

    if(ElectronColl.size() != 2) return;
    
    Particle ZCand = ElectronColl.at(0)+ElectronColl.at(1);
    double MllLeft = 60;
    double MllRight = 120;
    int NBin = 30;
    if(! (MllLeft<=ZCand.M()&&ZCand.M()<MllRight) ) return;

    vector<Electron> ElectronColl_shifted;// = ElectronColl; // copy the vector                                                                                                              

    Electron this_el1 = ElectronColl.at(0);
    Electron this_el2 = ElectronColl.at(1);


    if(abs(ElectronColl.at(0).scEta())<1.4442&&abs(ElectronColl.at(1).scEta())<1.4442){
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()>0){
        FillHist(param.Name+"/ScaleFactor/BB_ZMass_SS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
	FillHist(param.Name+"/ScaleFactor/BB_LepPt_SS", this_el1.PtMaxed(200) , EvWeight , nbinNeg, pTbinNeg,"pT");
	FillHist(param.Name+"/ScaleFactor/BB_LepPt_SS", -1*this_el2.PtMaxed(200) , EvWeight , nbinNeg, pTbinNeg,"pT");
	FillHist(param.Name+"/ScaleFactor/BB_LepEta_SS", (this_el1.Eta()+2.5)    , EvWeight ,100., -5, 5, "Eta");
	FillHist(param.Name+"/ScaleFactor/BB_LepEta_SS", -1*(this_el2.Eta()+2.5)    , EvWeight ,100., -5, 5, "Eta");
	
      }
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
        FillHist(param.Name+"/ScaleFactor/BB_ZMass_OS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      } //JH : if you want to check OS distribution before applying the CF weight
    }
    // BE
    if((abs(ElectronColl.at(0).scEta())<1.4442&&abs(ElectronColl.at(1).scEta())>=1.556)||(abs(ElectronColl.at(0).scEta())>=1.556&&abs(ElectronColl.at(1).scEta())<1.4442)){
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()>0){
        FillHist(param.Name+"/ScaleFactor/BE_ZMass_SS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
        FillHist(param.Name+"/ScaleFactor/BE_LepPt_SS", this_el1.PtMaxed(200) , EvWeight , nbinNeg, pTbinNeg,"pT");
	FillHist(param.Name+"/ScaleFactor/BE_LepPt_SS", -1*this_el2.PtMaxed(200) , EvWeight , nbinNeg, pTbinNeg,"pT");
	FillHist(param.Name+"/ScaleFactor/BE_LepEta_SS", (this_el1.Eta()+2.5)    , EvWeight ,100., -5, 5, "Eta");
	FillHist(param.Name+"/ScaleFactor/BE_LepEta_SS", -1*(this_el2.Eta()+2.5)    , EvWeight ,100., -5, 5, "Eta");

      }
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
        FillHist(param.Name+"/ScaleFactor/BE_ZMass_OS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      }
    }
    // EE
    if(abs(ElectronColl.at(0).scEta())>=1.556&&abs(ElectronColl.at(1).scEta())>=1.556){
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()>0){
        FillHist(param.Name+"/ScaleFactor/EE_ZMass_SS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
        FillHist(param.Name+"/ScaleFactor/EE_LepPt_SS", this_el1.PtMaxed(200) , EvWeight , nbinNeg, pTbinNeg,"pT");
	FillHist(param.Name+"/ScaleFactor/EE_LepPt_SS", -1*this_el2.PtMaxed(200) , EvWeight , nbinNeg, pTbinNeg,"pT");
	FillHist(param.Name+"/ScaleFactor/EE_LepEta_SS", (this_el1.Eta()+2.5)    , EvWeight ,100., -5, 5, "Eta");
	FillHist(param.Name+"/ScaleFactor/EE_LepEta_SS", -1*(this_el2.Eta()+2.5)    , EvWeight ,100., -5, 5, "Eta");
      }
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){
        FillHist(param.Name+"/ScaleFactor/EE_ZMass_OS", ZCand.M(), EvWeight, NBin, MllLeft, MllRight);
      }
    }
    
    
    double X = GetZMassShift(ElectronColl);;
  
    if(ElectronColl[0].IsBB() && ElectronColl[1].IsBB()){
      if(DataYear==2017) {
	
	if(HasFlag("FLATSF")) X = 0.988;
	else {
	  if(ElectronColl[0].Pt() < 50 && ElectronColl[1].Pt() < 50) X = 0.993;
	  else if(ElectronColl[0].Pt() < 100 && ElectronColl[1].Pt() < 50) X = 0.994;
	  else if( ElectronColl[1].Pt() < 100) X = 0.998;
	  else X=1;
	}
      }
    }
    if(ElectronColl[0].IsEC() && ElectronColl[1].IsEC()){
      if(DataYear==2017) {
	if(HasFlag("FLATSF"))X = 0.992;
	else{
	  if(ElectronColl[0].Pt() < 50 && ElectronColl[1].Pt() < 50) X = 0.99;
          else if(ElectronColl[0].Pt() < 100 && ElectronColl[1].Pt() < 50) X = 0.996;
          else if( ElectronColl[1].Pt() < 100) X = 0.996;
          else  if( ElectronColl[1].Pt() < 250) X = 0.997;
	  else  X=1;
	}
      }
    }

    this_el1*= X;
    this_el2*= X;
    
    TString X_string = "_Shifted";
    ElectronColl_shifted.push_back(this_el1);
    ElectronColl_shifted.push_back(this_el2);
        
    Particle ZCand_shifted = ElectronColl_shifted.at(0) + ElectronColl_shifted.at(1);
    double weight_shifted = GetCFWeightElectron(ElectronColl_shifted, param , false);
    weight_shifted *= EvWeight;
    double weight_shiftedSF = weight_shifted;
    weight_shiftedSF *= GetCFSF(param,ElectronColl);
    
    
    //Particle METv_shifted;
    //METv_shifted.SetPxPyPzE(METv.Px()+ZCand.Px()-ZCand_shifted.Px(),METv.Py()+ZCand.Py()-ZCand_shifted.Py(),0,METv.E()+ZCand.E()-ZCand_shifted.E());
    
    if(MllLeft<=ZCand_shifted.M()&&ZCand_shifted.M()<MllRight){
      if(ElectronColl.at(0).Charge()*ElectronColl.at(1).Charge()<0){

        // BB
        if(abs(ElectronColl_shifted.at(0).scEta())<1.4442&&abs(ElectronColl_shifted.at(1).scEta())<1.4442){
          FillHist(param.Name+"/ScaleFactor/BB_ZMass_OS_CFweighted_shifted_"+X_string, ZCand_shifted.M(),         weight_shifted, NBin, MllLeft, MllRight);
	  FillHist(param.Name+"/ScaleFactor/BB_LepPt_OS_CFweighted_shifted_"+X_string, this_el1.PtMaxed(200) ,    weight_shifted, nbinNeg, pTbinNeg,"pT");
	  FillHist(param.Name+"/ScaleFactor/BB_LepPt_OS_CFweighted_shifted_"+X_string, -1*this_el2.PtMaxed(200) , weight_shifted, nbinNeg, pTbinNeg,"pT");
	  FillHist(param.Name+"/ScaleFactor/BB_LepEta_OS_CFweighted_shifted_"+X_string, (this_el1.Eta()+2.5)    , weight_shifted, 100., -5, 5, "Eta");
	  FillHist(param.Name+"/ScaleFactor/BB_LepEta_OS_CFweighted_shifted_"+X_string, -1*(this_el2.Eta()+2.5) , weight_shifted, 100., -5, 5, "Eta");
	}

        // BE
        if((abs(ElectronColl_shifted.at(0).scEta())<1.4442&&abs(ElectronColl_shifted.at(1).scEta())>=1.556)||(abs(ElectronColl_shifted.at(0).scEta())>=1.556&&abs(ElectronColl_shifted.at(1).scEta())<1.4442)){
          FillHist(param.Name+"/ScaleFactor/BE_ZMass_OS_CFweighted_shifted_"+X_string, ZCand_shifted.M(),         weight_shifted, NBin, MllLeft, MllRight);
          FillHist(param.Name+"/ScaleFactor/BE_ZMass_OS_CFSFweighted_shifted_"+X_string, ZCand_shifted.M(),       weight_shiftedSF, NBin, MllLeft, MllRight);
          FillHist(param.Name+"/ScaleFactor/BE_LepPt_OS_CFweighted_shifted_"+X_string, this_el1.PtMaxed(200) ,    weight_shiftedSF, nbinNeg, pTbinNeg,"pT");
          FillHist(param.Name+"/ScaleFactor/BE_LepPt_OS_CFweighted_shifted_"+X_string, -1*this_el2.PtMaxed(200) , weight_shiftedSF, nbinNeg, pTbinNeg,"pT");
          FillHist(param.Name+"/ScaleFactor/BE_LepEta_OS_CFweighted_shifted_"+X_string, (this_el1.Eta()+2.5)    , weight_shiftedSF ,100., -5, 5, "Eta");
          FillHist(param.Name+"/ScaleFactor/BE_LepEta_OS_CFweighted_shifted_"+X_string, -1*(this_el2.Eta()+2.5) , weight_shiftedSF ,100., -5, 5, "Eta");

        }

        // EE
        if(abs(ElectronColl_shifted.at(0).scEta())>=1.556&&abs(ElectronColl_shifted.at(1).scEta())>=1.556){
          FillHist(param.Name+"/ScaleFactor/EE_ZMass_OS_CFweighted_shifted_"+X_string, ZCand_shifted.M(), weight_shifted, NBin, MllLeft, MllRight);
	  FillHist(param.Name+"/ScaleFactor/EE_LepPt_OS_CFweighted_shifted_"+X_string, this_el1.PtMaxed(200) ,    weight_shifted, nbinNeg, pTbinNeg,"pT");
          FillHist(param.Name+"/ScaleFactor/EE_LepPt_OS_CFweighted_shifted_"+X_string, -1*this_el2.PtMaxed(200) , weight_shifted, nbinNeg, pTbinNeg,"pT");
          FillHist(param.Name+"/ScaleFactor/EE_LepEta_OS_CFweighted_shifted_"+X_string, (this_el1.Eta()+2.5)    , weight_shifted ,100., -5, 5, "Eta");
          FillHist(param.Name+"/ScaleFactor/EE_LepEta_OS_CFweighted_shifted_"+X_string, -1*(this_el2.Eta()+2.5) , weight_shifted ,100., -5, 5, "Eta");

        }
      }
    }
  }
}

HNL_ChargeFlip::HNL_ChargeFlip(){

}

HNL_ChargeFlip::~HNL_ChargeFlip(){

}
