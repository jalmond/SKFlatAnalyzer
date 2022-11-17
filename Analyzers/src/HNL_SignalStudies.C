#include "HNL_SignalStudies.h"

void HNL_SignalStudies::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  
  // Overwite JECSources in AnalyserCore
  JECSources = {"AbsoluteStat"};
  
  for(auto jec_source : JECSources){
    SetupJECUncertainty(jec_source);
  }
  

}


void HNL_SignalStudies::executeEvent(){

  
 
  //==== Gen for genmatching
  AnalyzerParameter param  = InitialiseHNLParameter("SignalStudy","UL");
  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  FillHist ("NoCut", 1, weight, 2, 0., 2.,"");

  TString process="";
  if(!IsData){
    gens = GetGens();
    
    process = GetProcess();

    vector<TString> labels ={"Inclusive","OS_ElEl", "SS_El-El-", "SS_El+El+","OS_MuMu", "SS_Mu-Mu-" , "SS_Mu+Mu+","OS_ElMu","SS_El-Mu-" ,"SS_El+Mu+", "OS_MuEl","SS_Mu-El-","SS_Mu+El+"};

    HNL_LeptonCore::FillEventCutflowAll("SignalProcess","SplitChannel",weight, labels, process);
    HNL_LeptonCore::FillEventCutflowAll("SignalProcess","SplitChannel",weight, labels, "Inclusive");
    

    if(_jentry% 10000==0){
      cout << "process = " << process << endl;
      
      PrintGen(gens);
    }


  }
  if(process.Contains("OS")) return;


  
  /*
  vector<Muon> _InputMuons = GetMuons    ( "HNTightPFIsoMedium", 10., 2.4);
  vector<Electron> _InputElectrons = GetElectrons    ( "passPOGTight", 15., 2.4);

  vector<Electron> _TriggerElectrons = GetElectrons    ( "Trigger", 15., 2.4);
  vector<Electron> _TriggerLooseElectrons = GetElectrons    ( "TriggerLoose", 15., 2.4);
  vector<Electron> _CCElectrons = GetElectrons    ( "CC", 15., 2.4);
  vector<Electron> _POGElectrons = GetElectrons    ( "passMVAID_noIso_WP90Opt", 15., 2.4);

  vector<Muon> InputMuons = GetSignalLeptons(_InputMuons, gens);
  vector<Electron> InputElectrons = GetSignalLeptons(_InputElectrons, gens);

  vector<Electron> TriggerElectrons = GetSignalLeptons(_TriggerElectrons, gens);
  vector<Electron> TriggerLooseElectrons = GetSignalLeptons(_TriggerLooseElectrons, gens);
  vector<Electron> CCElectrons = GetSignalLeptons(_CCElectrons,gens); 
  vector<Electron> POGElectrons = GetSignalLeptons(_POGElectrons,gens); 
  */
  vector<HNL_LeptonCore::Channel> channels = {EE};//,MuMu, EMu};


  for(auto dilep_channel : channels){

    if(MCSample.Contains("Type")){
      if (!SelectChannel(dilep_channel)) continue;
    }

    TString channel = GetChannelString(dilep_channel) ;

    double ptbins[11] = { 0., 10.,15., 20., 30., 40.,50., 100.,500. ,1000., 2000.};
    
    /*
    std::vector<Lepton *> leps       = MakeLeptonPointerVector( InputMuons,InputElectrons,param);

    

    for(auto ilep:  leps)  {
      double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
      FillHist( "SignalReco"+channel+"/Lep_pt_inc", pt, weight, 10, ptbins);
    }

    if (PassTriggerSelection(dilep_channel, ev, leps,"Dilep")) {
      if(SameCharge(leps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 0, weight, 10., 0., 10);

      for(auto ilep:  leps)  {
        double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
        if (pt  < 10) cout << "Pt Trig = " << ilep->Pt() << endl;
        FillHist( "SignalReco"+channel+"/Lep_pt_DiLepTrig", pt, weight, 10, ptbins);
      }
    }
    if (PassTriggerSelection(dilep_channel, ev, leps,"Dilep")  || PassTriggerSelection(dilep_channel, ev, leps,"Lep")) {

      if(SameCharge(leps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 1, weight, 10., 0., 10);

      for(auto ilep:  leps)  {
        double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
        FillHist( "SignalReco"+channel+"/Lep_pt_DiLepSngTrig", pt, weight, 10, ptbins);
      }
    }
    if (PassTriggerSelection(dilep_channel, ev, leps,"Lep")) {

      if(SameCharge(leps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 2, weight, 10., 0., 10);

      for(auto ilep:  leps)  {
        double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
        FillHist( "SignalReco"+channel+"/Lep_pt_SngLepTrig", pt, weight, 10, ptbins);
      }
    }
    if (PassTriggerSelection(dilep_channel, ev, leps,"Dilep")  || PassTriggerSelection(dilep_channel, ev, leps,"HighPt")) {

      if(SameCharge(leps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 3, weight, 10., 0., 10);

      for(auto ilep:  leps)  {
        double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
        FillHist( "SignalReco"+channel+"/Lep_pt_DiLepHightPt", pt, weight, 10, ptbins);
      }
    }
    if (PassTriggerSelection(dilep_channel, ev, leps,"HighPt")) {
      if(SameCharge(leps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 4, weight, 10., 0., 10);

      for(auto ilep:  leps)  {
        double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
        FillHist( "SignalReco"+channel+"/Lep_pt_HightPt", pt, weight, 10, ptbins);
      }
    }

    if (PassTriggerSelection(dilep_channel, ev, leps,"Full")) {

      if(SameCharge(leps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 5, weight, 10., 0., 10);

      for(auto ilep:  InputElectrons)  {
	FillHist( "SignalReco"+channel+"/Emulation_Bins", 0, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(1)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 0, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(2)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 1, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(3)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 2, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(4)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 3, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(5)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 4, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(6)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 5, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(7)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 6, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(8)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 7, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(9)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 8, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(10)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 9, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(11)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 10, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(12)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 11, weight, 13., 0., 13);
	if(!ilep.Pass_TriggerEmulationN(13)) FillHist( "SignalReco"+channel+"/Emulation_Bins", 12, weight, 13., 0., 13);
	

      }
    }

    std::vector<Lepton *> Triggerleps       = MakeLeptonPointerVector( InputMuons, TriggerElectrons,param);
    std::vector<Lepton *> TriggerLooseleps       = MakeLeptonPointerVector( InputMuons, TriggerLooseElectrons,param);
    std::vector<Lepton *> CCleps       = MakeLeptonPointerVector( InputMuons, CCElectrons,param);
    std::vector<Lepton *> POGleps       = MakeLeptonPointerVector( InputMuons, POGElectrons,param);
    
    if (PassTriggerSelection(dilep_channel, ev, Triggerleps,"Full")) {
      
      if(SameCharge(Triggerleps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 6, weight, 10., 0., 10);
    }
    if (PassTriggerSelection(dilep_channel, ev, TriggerLooseleps,"Full")) {

      if(SameCharge(TriggerLooseleps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 7, weight, 10., 0., 10);
    }
    
    if (PassTriggerSelection(dilep_channel, ev, CCleps,"Full")) {
      
      if(SameCharge(CCleps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 8, weight, 10., 0., 10);
      
    }

    if (PassTriggerSelection(dilep_channel, ev, CCleps,"Dilep")) {

      if(SameCharge(CCleps))          FillHist( "SignalReco"+channel+"/Trigger_Bins", 9, weight, 10., 0., 10);

    }
    
    if (PassTriggerSelection(dilep_channel, ev, POGleps,"Full")) {

      for(auto ilep:  POGElectrons)  {
        FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 0, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(1)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 0, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(2)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 1, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(3)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 2, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(4)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 3, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(5)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 4, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(6)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 5, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(7)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 6, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(8)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 7, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(9)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 8, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(10)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 9, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(11)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 10, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(12)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 11, weight, 13., 0., 13);
        if(!ilep.Pass_TriggerEmulationN(13)) FillHist( "SignalReco"+channel+"POG/Emulation_Bins", 12, weight, 13., 0., 13);
	
      }
      
    }
    
    */
    
    FillHist ("NoCut_"+channel, 1, weight, 2, 0., 2.,"");
    
    if(MCSample.Contains("DYType") && MCSample.Contains("private"))  MakeType1SignalPlots(channel, false);
    
    if(MCSample.Contains("VBFType") && MCSample.Contains("private"))  MakeType1VBFSignalPlots(channel, false);
    
    if(MCSample.Contains("SSWWType") && MCSample.Contains("private"))  MakeType1SSWWSignalPlots(channel, false);
  

    vector<Gen> gen_lep= GetGenLepronsSignal();
    
    
    std::vector<Electron>   ElectronCollAll = GetElectrons( "NoCut", 5., 2.5);
    //for(auto iel: ElectronCollAll){
    //  if(iel.passMVAID_noIso_WP90())cout << "El pt = " << iel.Pt() << "   eta = " << iel.Eta() << "  mva = " << iel.MVANoIso() << "  Pass MVA = " << iel.passMVAID_noIso_WP90() << end//l;
    //    else if (iel.MVANoIso() > 0.9) cout << "FAIL El pt = " << iel.Pt() << "   eta = " << iel.Eta() << "  mva = " << iel.MVANoIso() << "  Pass MVA = " << iel.passMVAID_noIso_WP90() << e//ndl;
      
				     //  }

    std::vector<Muon>       MuonCollAll     = GetMuons    (  "NoCut", 5., 2.4);
    
    std::vector<Electron>   ElectronColl;
    std::vector<Muon>       MuonColl;
    if(MCSample.Contains("Type")){

      for(auto iel: ElectronCollAll){ 
	bool matched_lep=false;
	for(auto igen : gen_lep){
	  if(iel.DeltaR(igen) < 0.2) matched_lep=true;
	}
	if(matched_lep) ElectronColl.push_back(iel);
      }
      
      for(auto iel: MuonCollAll){
	bool matched_lep=false;
	for(auto igen :gen_lep){
	  if(iel.DeltaR(igen) <0.2) matched_lep=true;
	}
	if(matched_lep)MuonColl.push_back(iel);
      } 
      
    }
    else{
      ElectronColl = ElectronCollAll;
      MuonColl=MuonCollAll;
    }

    std::vector<Electron>   ElectronCollFake;
    std::vector<Muon>       MuonCollFake;
    std::vector<Electron>   ElectronCollExtConv;
    std::vector<Electron>   ElectronCollIntConv;
    std::vector<Electron>   ElectronCollCF;
    std::vector<Muon>       MuonCollConv;
    

    if(!MCSample.Contains("Type")){
      ElectronCollFake = SkimLepColl(ElectronColl, gens, param, "HFake");
      MuonCollFake = SkimLepColl(MuonColl, gens, param, "HFake");
      ElectronCollExtConv = SkimLepColl(ElectronColl, gens, param, "NHExtConv");
      MuonCollConv = SkimLepColl(MuonColl, gens, param, "NHConv");
      ElectronCollIntConv = SkimLepColl(ElectronColl, gens, param, "NHIntConv");
      ElectronCollCF = SkimLepColl(ElectronColl, gens, param, "CF");
      
      FillAllMuonPlots("Fake", "Muons"  , MuonCollFake , weight);
      FillAllElectronPlots("Fake", "Electrons"  , ElectronCollFake , weight);
      FillAllElectronPlots("IntConv", "Electrons"  , ElectronCollIntConv , weight);
      FillAllMuonPlots("Conv", "Muons"  , MuonCollConv , weight);
      FillAllElectronPlots("ExtConv", "Electrons"  , ElectronCollExtConv , weight);
      FillAllElectronPlots("CF", "Electrons"  , ElectronCollCF , weight);
      
      
      FillAllElectronPlots("Bkg", "ElectronsPOGMVA80"  , SelectElectrons    (ElectronColl,  "passMVAID_noIso_WP80Opt", 10., 2.5) , weight);
      FillAllElectronPlots("Bkg", "ElectronsPOGMVA90"  , SelectElectrons    (ElectronColl,  "passMVAID_noIso_WP90Opt", 10., 2.5) , weight);

      FillAllMuonPlots("Fake", "MuonsOpt"  , SelectMuons    ( MuonCollFake ,  "HNOpt", 5., 2.4) , weight);
      FillAllElectronPlots("Fake", "ElectronsOpt"  , SelectElectrons    (ElectronCollFake,  "HNOpt", 10., 2.5) , weight);
      FillAllElectronPlots("IntConv", "ElectronsOpt"  , SelectElectrons    (ElectronCollIntConv,  "HNOpt", 10., 2.5) , weight);
      FillAllMuonPlots("Conv", "MuonsOpt"  , SelectMuons    ( MuonCollConv,  "HNOpt", 5., 2.4) , weight);
      FillAllElectronPlots("ExtConv", "ElectronsOpt"  , SelectElectrons    (ElectronCollExtConv,  "HNOpt", 10., 2.5) , weight);
      FillAllElectronPlots("CF", "ElectronOpts"  , SelectElectrons    (ElectronCollCF,  "HNOpt", 10., 2.5) , weight);
    }
    else{
      FillAllMuonPlots("Signal", "Muons"  , MuonColl , weight);
      FillAllElectronPlots("Signal", "Electrons"  , ElectronColl , weight);
      FillAllMuonPlots("Signal", "MuonsOpt"  , SelectMuons    ( MuonColl ,  "HNOpt", 5., 2.4) , weight);
      FillAllElectronPlots("Signal", "ElectronsOpt"  , SelectElectrons    (ElectronColl,  "HNOpt", 10., 2.5) , weight);
      FillAllElectronPlots("Signal", "ElectronsPOGMVA80"  , SelectElectrons    (ElectronColl,  "passMVAID_noIso_WP80Opt", 10., 2.5) , weight);
      FillAllElectronPlots("Signal", "ElectronsPOGMVA90"  , SelectElectrons    (ElectronColl,  "passMVAID_noIso_WP90Opt", 10., 2.5) , weight);

    }      

    std::vector<Lepton *> Leps       = MakeLeptonPointerVector( MuonColl,ElectronColl,param);

    if(!SameCharge(Leps)) continue;
    
    if (!PassTriggerSelection(dilep_channel, ev, Leps,"Dilep")) continue;


    vector<TString> MuonIDs = {"NoCut","POGTight","POGHighPt","POGMedium","POGLoose","POGTightWithTightIso","POGHighPtWithLooseTrkIso","POGHighPtTight","POGHighPtMixTight","HNVeto2016","HNLoosest","HNLoose_17028","HNTight_17028","HNLooseMVA","HNLooseV1","HNLoosePOG","HNTightMVAVL","HNTightMVAM","HNTightMVAVVT","HNTightV1","HNTightV2","HNTightPFIsoLoose", "HNTightPFIsoMedium","HNTightPFIsoTight","HNTightPFIsoVeryTight","HNTightPFIsoVeryVeryTight","HNOpt"};

    if(dilep_channel == MuMu){
      for(auto id_mu  : MuonIDs){
	std::vector<Muon>       MuonCollID     = GetMuons    (  id_mu, 5., 2.4);
	for(auto ilep:  MuonCollID)  {
	  double pt = (ilep.Pt() > 2000) ? 1999 : ilep.Pt();
	  FillHist( "Reco"+channel+"/Lep_pt_"+id_mu, pt, weight, 10, ptbins);
	  if(SameCharge(MuonCollID))          FillHist( "Reco"+channel+"_SS/Lep_pt_"+id_mu, pt, weight, 10, ptbins);

	}
      }

      /*
	ElectronCollFake = ElectronColl;
      ElectronCollExtConv = ElectronColl;
      ElectronCollIntConv = ElectronColl;
      ElectronCollCF=ElectronColl;
      MuonCollFake = MuonColl;
      MuonCollConv= MuonColl;
      */
      
      std::vector<Muon> MuonCollB = SkimLepColl(MuonColl, "MBMO");
      std::vector<Muon> MuonCollE = SkimLepColl(MuonColl, "ME");
      std::vector<Muon> MuonCollFakeB = SkimLepColl(MuonCollFake, "MBMO");
      std::vector<Muon> MuonCollFakeE = SkimLepColl(MuonCollFake, "ME");
      std::vector<Muon> MuonCollConvB = SkimLepColl(MuonCollConv, "MBMO");
      std::vector<Muon> MuonCollConvE = SkimLepColl(MuonCollConv, "ME");


      for( int ibkg = 0 ; ibkg < 6; ibkg++){
	TString muon_type;
	std::vector<Muon>       MuonCollID ;
	std::vector<Muon>       MuonCollIDMVA ;
	if (ibkg==0) {
	  MuonCollID     = SelectMuons    ( MuonCollB,  "HNTightPFIsoMedium", 5., 2.4);
	  MuonCollIDMVA     = SelectMuons    ( MuonCollB,  "HNOpt", 5., 2.4);
	  muon_type = "SigB";
	}
	if (ibkg==3) {
          MuonCollID     = SelectMuons    ( MuonCollE,  "HNTightPFIsoMedium", 5., 2.4);
          MuonCollIDMVA     = SelectMuons    ( MuonCollE,  "HNOpt", 5., 2.4);
          muon_type = "SigEC";
        }
	if (ibkg==1) {
          MuonCollID     = SelectMuons    ( MuonCollFakeB,  "HNTightPFIsoMedium", 5., 2.4);

          MuonCollIDMVA     = SelectMuons    ( MuonCollFakeB,  "HNOpt", 5., 2.4);
          muon_type = "FakeB";
        }
	if (ibkg==4) {
          MuonCollID     = SelectMuons    ( MuonCollFakeE,  "HNTightPFIsoMedium", 5., 2.4);
          MuonCollIDMVA     = SelectMuons    ( MuonCollFakeE,  "HNOpt", 5., 2.4);
          muon_type = "FakeEC";
        }
	if (ibkg==2) {
          MuonCollID     = SelectMuons    ( MuonCollConvB,  "HNTightPFIsoMedium", 5., 2.4);
          MuonCollIDMVA     = SelectMuons    ( MuonCollConvB,  "HNOpt", 5., 2.4);
          muon_type = "ConvB";
        }
        if (ibkg==5) {
          MuonCollID     = SelectMuons    ( MuonCollConvE,  "HNTightPFIsoMedium", 5., 2.4);
          MuonCollIDMVA     = SelectMuons    ( MuonCollConvE,  "HNOpt", 5., 2.4);
          muon_type = "ConvEC";
	}
	


	for(auto imu : MuonCollID) {
	  double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();

	  FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_POGMediumnom", pt, weight, 10, ptbins);
	}
	
	// IP
	if(1){
	  
	  for( int j1=-1; j1 < 71; j1++){
	    double IP_1 = double(3.+0.1*double(j1));
	    std::string IP1= std::to_string(IP_1);
	    
	    if(j1==-1){
	      IP_1 = 999.;
	      IP1="NoCut";
	    }
	    
	    std::vector<Muon>       MuonCollOpt;
	    for (auto mu : MuonCollID){
	      if(ibkg < 3 && mu.passIDHN(2, 0.05, 0.05, 0.1,0.1, IP_1,10., 0.6, 0.6, -999, -999)) MuonCollOpt.push_back(mu);
	      if(ibkg > 2 && mu.passIDHN(2, 0.05, 0.05, 0.1,0.1, 10., IP_1, 0.6, 0.6, -999, -999)) MuonCollOpt.push_back(mu);
	    }

	    TString id_mu = "IP3D_"+IP1;

	    for(auto imu : MuonCollOpt) {
	      double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();

	      FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
	    }
	  }
	}
	
	// IP                                                                                                                                                                     
	if(1){
	  // Check DXY cut                                                                                                                                                       
	  
	  for( int j1=-1; j1 < 46; j1++){
	    double DXY_1 = double(0.005+0.001*double(j1));
	    std::string DXY1= std::to_string(DXY_1);

	    if(j1==-1){
              DXY_1 = 999.;
	      DXY1="NoCut";
            }


	    std::vector<Muon>       MuonCollOpt;
	    for (auto mu : MuonCollID){
	      if(ibkg < 3 && mu.passIDHN(2, DXY_1, 0.05, 0.1,0.1, 10.,10., 0.6, 0.6, -999, -999)) MuonCollOpt.push_back(mu);
	      if(ibkg > 2 && mu.passIDHN(2, 0.05, DXY_1, 0.1,0.1, 10., 10, 0.6, 0.6, -999, -999)) MuonCollOpt.push_back(mu);
	    }
	    TString id_mu = "DXY_"+DXY1;
	    for(auto imu : MuonCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();

              FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
	    }
	  }
	} 
	
	if(1){
	  // Check DXY cut                                                                                                                                                       

	  for( int j1=-1; j1 < 21; j1++){
	    double DZ_1 = double(0.04+0.01*double(j1));
	    std::string DZ1= std::to_string(DZ_1);

	    if(j1==-1){
              DZ_1 = 999.;
              DZ1="NoCut";
            }
	    std::vector<Muon>       MuonCollOpt;
	    for (auto mu : MuonCollID){
	      if(ibkg < 3 &&mu.passIDHN(2, 0.05, 0.05, DZ_1, 0.1, 10.,10., 0.6, 0.6, -999, -999)) MuonCollOpt.push_back(mu);
	      if(ibkg > 2 &&mu.passIDHN(2, 0.05, 0.05, 0.1,DZ_1, 10., 10, 0.6, 0.6, -999, -999)) MuonCollOpt.push_back(mu);
	    }
	    TString id_mu = "DZ_"+DZ1;
	    for(auto imu : MuonCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
	      FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }
	    
	  }
	}
	
	if(1){
	  // Check ISO cut                                                                                                                                                       
	  
	  for( int j1=-1; j1 < 61; j1++){
	    double ISO_1 = double(0.05+0.005*double(j1));
	    std::string ISO1= std::to_string(ISO_1);
	    std::vector<Muon>       MuonCollOpt;

	    if(j1==-1){
              ISO_1 = 999.;
              ISO1="NoCut";
            }

	    for (auto mu : MuonCollID){
	      if(ibkg < 3 &&mu.passIDHN(2, 0.05, 0.05, 0.1, 0.1, 10.,10., ISO_1, 0.6, -999, -999)) MuonCollOpt.push_back(mu);
	      if(ibkg > 2 &&mu.passIDHN(2, 0.05, 0.05, 0.1,0.1, 10., 10, 0.6, ISO_1, -999, -999)) MuonCollOpt.push_back(mu);
	    }
	    TString id_mu = "ISO_"+ISO1;
            for(auto imu : MuonCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
	      FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }
	  }
	}
	if(1){
	  // Check ISO cut                                                                                                                                                      
	  
	  for( int j1=-1; j1 < 61; j1++){
	    double ISO_1 = double(0.05+0.005*double(j1));
	    std::string ISO1= std::to_string(ISO_1);

	    if(j1==-1){
              ISO_1 = 999.;
              ISO1="NoCut";
            }
	    std::vector<Muon>       MuonCollOpt;
	    for (auto mu : MuonCollID){
	      if(ibkg < 3 &&mu.passIDHN(2, 0.05, 0.05, 0.1, 0.1, 10.,10., -999, -999, ISO_1, 0.6)) MuonCollOpt.push_back(mu);
	      if(ibkg > 2 &&mu.passIDHN(2, 0.05, 0.05, 0.1,0.1, 10., 10, -999, -999, 0.6, ISO_1)) MuonCollOpt.push_back(mu);
	    }
	    TString id_mu = "MiniISO_"+ISO1;
            for(auto imu : MuonCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }

	  }
	} 
	if(1){
	  // Check MVA cut                                                                                                                                                       
	  
	  for( int j1=-1; j1 < 200; j1++){
	    double MVA_1 = double(-1+0.01*double(j1));
	    std::string MVA1= std::to_string(MVA_1);

	    if(j1==-1){
              MVA_1 = -1.;
              MVA1="NoCut";
            }
	    std::vector<Muon>       MuonCollOpt;
	    for (auto mu : MuonCollIDMVA){
	      if(ibkg < 3 &&mu.PassMVA(MVA_1,MVA_1,0.8)) MuonCollOpt.push_back(mu);
	      if(ibkg > 2 &&mu.PassMVA(0.8,0.8,MVA_1)) MuonCollOpt.push_back(mu);
	    }
	    TString id_mu = "MVA_"+MVA1;
	    for(auto imu : MuonCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }

	  }
	} 
	
	if(1){
	  // Check MVA cut                                                                                                                                                                                            
	  
	  for( int j1=-1; j1 < 5; j1++){
	    double MissingHit_1 = double(0+1*double(j1));
	    std::string MissingHit1= std::to_string(MissingHit_1);

	    if(j1==-1){
              MissingHit_1 = 0;
              MissingHit1="NoCut";
            }
	    std::vector<Muon>       MuonCollOpt;
	    for (auto mu : MuonCollID){
	      bool pass=true;
	      if(mu.MatchedStations() > MissingHit_1) MuonCollOpt.push_back(mu);
	      
	    }
	    TString id_mu = "MissingHit_"+MissingHit1;
	    for(auto imu : MuonCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+muon_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }

	  }
	}
      }
    }
      
      
      
      
    if(dilep_channel == EE){
      vector<TString> ElectronIDs = {"NoCut","HNVeto2016","HNVeto","CutBasedLooseNoIso","CutBasedMediumNoIso","CutBasedTightNoIso","MVALooseNoIso","CutBasedVetoNoIso","HNTightV2","HNTight_17028","passPOGTight","passPOGMedium","passHEEPID","passMVAID_noIso_WP80","passMVAID_noIso_WP90","passMVAID_Iso_WP80","passMVAID_Iso_WP90","HNHEEPID","SUSYTight","HN2016MVA","HN2016MVA2","HN2016MVA2CC","HN2016POG","HNOpt"};
      for(auto id_mu  : ElectronIDs){
	std::vector<Electron>       ElectronCollID     = GetElectrons    (  id_mu, 5., 2.4);
	for(auto ilep:  ElectronCollID)  {
	  double pt = (ilep.Pt() > 2000) ? 1999 : ilep.Pt();
	  FillHist( "Reco"+channel+"/Lep_pt_"+id_mu, pt, weight, 10, ptbins);
          if(SameCharge(ElectronCollID))          FillHist( "Reco"+channel+"_SS/Lep_pt_"+id_mu, pt, weight, 10, ptbins);
	  
	}
      }



      /*                                                                                                                                                                                                                           
        ElectronCollFake = ElectronColl;                                                                                                                                                                                           
      ElectronCollExtConv = ElectronColl;                                                                                                                                                                                          
      ElectronCollIntConv = ElectronColl;                                                                                                                                                                                          
      ElectronCollCF=ElectronColl;                                                                                                                                                                                                 
      ElectronCollFake = ElectronColl;                                                                                                                                                                                                     
      ElectronCollConv= ElectronColl;                                                                                                                                                                                                      
      */

      std::vector<Electron> ElectronCollB = SkimLepColl(ElectronColl, "B1B2");
      std::vector<Electron> ElectronCollE = SkimLepColl(ElectronColl, "E");
      std::vector<Electron> ElectronCollFakeB = SkimLepColl(ElectronCollFake, "B1B2");
      std::vector<Electron> ElectronCollFakeE = SkimLepColl(ElectronCollFake, "E");
      std::vector<Electron> ElectronCollExtConvB = SkimLepColl(ElectronCollExtConv, "B1B2");
      std::vector<Electron> ElectronCollExtConvE = SkimLepColl(ElectronCollExtConv, "E");
      std::vector<Electron> ElectronCollIntConvB = SkimLepColl(ElectronCollIntConv, "B1B2");
      std::vector<Electron> ElectronCollIntConvE = SkimLepColl(ElectronCollIntConv, "E");
      std::vector<Electron> ElectronCollCFB = SkimLepColl(ElectronCollCF, "B1B2");
      std::vector<Electron> ElectronCollCFE = SkimLepColl(ElectronCollCF, "E");


      for( int ibkg = 0 ; ibkg < 10; ibkg++){

	TString electron_type="";

	std::vector<Electron>       ElectronCollID; 
	std::vector<Electron>       ElectronCollIDNoMVA;
	
        if (ibkg==0) {
          ElectronCollID     = SelectElectrons    ( ElectronCollB,  "passPOGTight", 10., 2.5);
	  ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollB,  "HNOpt", 10., 2.5);
          electron_type = "SigB";
        }
        if (ibkg==1) {
          ElectronCollID     = SelectElectrons    ( ElectronCollFakeB,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollFakeB,  "HNOpt", 10., 2.5);
          electron_type = "FakeB";
        }
	if (ibkg==2) {
	  ElectronCollID     = SelectElectrons    ( ElectronCollExtConvB,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollExtConvB,  "HNOpt", 10., 2.5);
          electron_type = "ExtConvB";
        }

	if (ibkg==3) {
          ElectronCollID     = SelectElectrons    ( ElectronCollIntConvB,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollIntConvB,  "HNOpt", 10., 2.5);
          electron_type = "IntConvB";
        }
	if (ibkg==4) {
          ElectronCollID     = SelectElectrons    ( ElectronCollCFB,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollCFB,  "HNOpt", 10., 2.5);
          electron_type = "CFB";
        }

        if (ibkg==5) {
          ElectronCollID     = SelectElectrons    ( ElectronCollE,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollE,  "HNOpt", 10., 2.5);
          electron_type = "SigEC";
        }
        if (ibkg==6) {
          ElectronCollID     = SelectElectrons    ( ElectronCollFakeE,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollFakeE,  "HNOpt", 10., 2.5);
          electron_type = "FakeEC";
        }
        if (ibkg==7) {
          ElectronCollID     = SelectElectrons    ( ElectronCollExtConvE,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollExtConvE,  "HNOpt", 10., 2.5);
          electron_type = "ExtConvEC";
        }

        if (ibkg==8) {
          ElectronCollID     = SelectElectrons    ( ElectronCollIntConvE,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollIntConvE,  "HNOpt", 10., 2.5);
          electron_type = "IntConvEC";
        }
        if (ibkg==9) {
          ElectronCollID     = SelectElectrons    ( ElectronCollCFE,  "passPOGTight", 10., 2.5);
          ElectronCollIDNoMVA     = SelectElectrons    ( ElectronCollCFE,  "HNOpt", 10., 2.5);
          electron_type = "CFEC";

        }
	

	for(auto imu : ElectronCollID) {
          double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
          FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_MVA90nom", pt, weight, 10, ptbins);
        }
	
	for(auto imu : ElectronCollIDNoMVA) {
          double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
          FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_MVAnom", pt, weight, 10, ptbins);
        }

	
	// IP                                                                                                                                                                                                                                                                                                                   
	if(1){
	  
	  for( int j1=-1; j1 < 71; j1++){
	    double IP_1 = double(3.+0.1*double(j1));
	    std::string IP1= std::to_string(IP_1);

	    if(j1==-1){
              IP_1 = 999.;
              IP1="NoCut";
            }
	    std::vector<Electron>       ElectronCollOpt;
	    for (auto mu : ElectronCollID){
	      if(ibkg < 5 && mu.passIDHN(3, 0.05, 0.05, 0.1,0.1, IP_1,999., 0.6, 0.6, -999, -999)) ElectronCollOpt.push_back(mu);
	      if(ibkg > 4 && mu.passIDHN(3, 0.05, 0.05, 0.1,0.1, 999., IP_1, 0.6, 0.6, -999, -999)) ElectronCollOpt.push_back(mu);
	    }
	    TString id_mu = "IP3D_"+IP1;

	    for(auto imu : ElectronCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }
	    
	  }
	}
	
	
	// IP                                                                                                                                                                                                                                   
	if(1){
	  // Check DXY cut                                                                                                                                                                                                                      
	  
	  for( int j1=-1; j1 < 46; j1++){
	    double DXY_1 = double(0.005+0.001*double(j1));
	    std::string DXY1= std::to_string(DXY_1);

	    if(j1==-1){
              DXY_1 = 999.;
              DXY1="NoCut";
            }
	    std::vector<Electron>       ElectronCollOpt;
	    for (auto mu : ElectronCollID){
	      if(ibkg < 5 && mu.passIDHN(3, DXY_1, 0.05, 0.1,0.1, 10.,10., 0.6, 0.6, -999, -999)) ElectronCollOpt.push_back(mu);
	      if(ibkg > 4 && mu.passIDHN(3, 0.05, DXY_1, 0.1,0.1, 10., 10, 0.6, 0.6, -999, -999)) ElectronCollOpt.push_back(mu);
	    }
	    TString id_mu = "DXY_"+DXY1;
            for(auto imu : ElectronCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }

	  }
	}
	
	if(1){
	  // Check DXY cut                                                                                                                                                                                                                      
	  
	  for( int j1=-1; j1 < 21; j1++){
	    double DZ_1 = double(0.04+0.01*double(j1));
	    std::string DZ1= std::to_string(DZ_1);

	    if(j1==-1){
              DZ_1 = 999.;
              DZ1="NoCut";
            }
	    std::vector<Electron>       ElectronCollOpt;
	    for (auto mu : ElectronCollID){
	      if(ibkg < 5 &&mu.passIDHN(3, 0.05, 0.05, DZ_1, 0.1, 10.,10., 0.6, 0.6, -999, -999)) ElectronCollOpt.push_back(mu);
	      if(ibkg > 4 &&mu.passIDHN(3, 0.05, 0.05, 0.1,DZ_1, 10., 10, 0.6, 0.6, -999, -999)) ElectronCollOpt.push_back(mu);
	    }
	    TString id_mu = "DZ_"+DZ1;
	    for(auto imu : ElectronCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }



	  }
	}
	
	
	if(1){
	  
	  for( int j1=-1; j1 < 61; j1++){
	    double ISO_1 = double(0.05+0.005*double(j1));
	    std::string ISO1= std::to_string(ISO_1);

	    if(j1==-1){
              ISO_1 = 999.;
              ISO1="NoCut";
            }
	    std::vector<Electron>       ElectronCollOpt;
	    for (auto mu : ElectronCollID){
	      if(ibkg < 5 &&mu.passIDHN(3, 0.05, 0.05, 0.1, 0.1, 10.,10., ISO_1, 0.6, -999, -999)) ElectronCollOpt.push_back(mu);
	      if(ibkg > 4 &&mu.passIDHN(3, 0.05, 0.05, 0.1,0.1, 10., 10, 0.6, ISO_1, -999, -999)) ElectronCollOpt.push_back(mu);
	    }
	    TString id_mu = "ISO_"+ISO1;
	    for(auto imu : ElectronCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }

	  }
	}
	if(1){
	  // Check ISO cut                                                                                                                                                                                                                      
	  
	  for( int j1=-1; j1 < 61; j1++){
	    double ISO_1 = double(0.05+0.005*double(j1));
	    std::string ISO1= std::to_string(ISO_1);

	    if(j1==-1){
              ISO_1 = 999.;
              ISO1="NoCut";
            }
	    std::vector<Electron>       ElectronCollOpt;
	    
	    for (auto mu : ElectronCollID){
	      if(ibkg < 5 &&mu.passIDHN(4, 0.05, 0.05, 0.1, 0.1, 10.,10., -999, -999, ISO_1, 0.6)) ElectronCollOpt.push_back(mu);
	      if(ibkg > 4 &&mu.passIDHN(4, 0.05, 0.05, 0.1 ,0.1, 10., 10, -999, -999, 0.6, ISO_1)) ElectronCollOpt.push_back(mu);
	    }
	    TString id_mu = "MiniISO_"+ISO1;
	    for(auto imu : ElectronCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }
	  }
	}
	

	if(1){
	  // Check MVA cut                                                                                                                                                                                    
	  
	  
	  for( int j1=-1; j1 < 200; j1++){
	    double MVA_1 = double(-1+0.01*double(j1));
	    std::string MVA1= std::to_string(MVA_1);

	    if(j1==-1){
              MVA_1 = -1.;
              MVA1="NoCut";
            }
	    std::vector<Electron>       ElectronCollOpt;
	    for (auto mu : ElectronCollIDNoMVA){
	      if(ibkg < 5 &&mu.PassMVA(MVA_1,MVA_1,0.8)) ElectronCollOpt.push_back(mu);
	      if(ibkg > 4 &&mu.PassMVA(0.8,0.8,MVA_1)) ElectronCollOpt.push_back(mu);
	    }
	    TString id_mu = "MVA_"+MVA1;
	    for(auto imu : ElectronCollOpt) {
              double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
              FillHist( "Opt"+channel+ "/"+electron_type+"/LepPt_"+id_mu, pt, weight, 10, ptbins);
            }

	  }
	}
	
	
      }
    }
    



    // HL ID
    std::vector<Electron>   ElectronCollT = GetElectrons( param.Electron_Tight_ID, 10., 2.5);
    std::vector<Muon>       MuonCollT     = GetMuons    ( param.Muon_Tight_ID, 5., 2.4);
    
    std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5); 
    std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);
    
    
    std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(MuonCollT,ElectronCollT);
    std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);
    
    
    vector<FatJet>   this_AllFatJets   =  puppiCorr->Correct(GetAllFatJets());
    std::vector<FatJet>   fatjets_tmp  = SelectFatJets(this_AllFatJets, param.FatJet_ID, 200, 5.);
    std::vector<Jet>      jets_tmp     = GetJets   ( param.Jet_ID,    15., 5.);
    std::vector<Jet>      bjets_tmp    = GetJets   ( param.Jet_ID,    20., 2.5);
    
    std::vector<FatJet> FatjetColl                  = GetAK8Jets(fatjets_tmp, 200., 5., true,  1., false, -999, false, 0., 99999., ElectronCollV, MuonCollV);
    
    std::vector<Jet> JetColl                        = GetAK4Jets(jets_tmp,     20., 2.7, true,  0.4,0.8,"",   ElectronCollV,MuonCollV, FatjetColl);
    std::vector<Jet> JetCollLoose                        = GetAK4Jets(jets_tmp,     15., 4.7, true,  0.4,0.8,"",   ElectronCollV,MuonCollV, FatjetColl);
    

    for(auto ij : JetColl) {
      cout << "Jet pt = " << ij.Pt() << " " << ij.Eta() << endl;
      for (auto isource : JECSources){
	cout << isource  << " unc = " <<  GetJECUncertainty(isource , ij.Eta(),ij.Pt(), true);
      }
    }

    continue;

    std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

    std::vector<Tau>        TauColl        = GetTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

    
    
    // select B jets
    JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
    
    std::vector<Jet> BJetColl    = GetBJets(param, bjets_tmp, param_jets);
    double sf_btag               = GetBJetSF(param, bjets_tmp, param_jets);
    if(!IsData )weight*= sf_btag;
    
    
    std::vector<Jet> VBF_JetColl                    = GetAK4Jets(jets_tmp,     30., 4.7, true,  0.4,0.8,"loose",  ElectronCollV,MuonCollV, FatjetColl); 

    TString def_paramName =param.Name;
    
    if (IsData || ( process.Contains("Mu+Mu+") or   process.Contains("Mu-Mu-") )){
      RunLeptonChannel(MuMu,LepsT, LepsV,  TauColl, JetCollLoose, JetColl, VBF_JetColl, FatjetColl, BJetColl, ev, param, weight );

    }
    
    param.Name=param.DefName;
    
    if (IsData || ( process.Contains("El+El+") or   process.Contains("El-El-") )){
      RunLeptonChannel(EE,LepsT, LepsV,  TauColl, JetCollLoose, JetColl, VBF_JetColl, FatjetColl, BJetColl, ev, param, weight );
    }
  
    param.Name=param.DefName;
    
    if (IsData || ( process.Contains("El+Mu+") or   process.Contains("El-Mu-") )){
      RunLeptonChannel(EMu,LepsT, LepsV,  TauColl, JetCollLoose, JetColl, VBF_JetColl, FatjetColl, BJetColl, ev, param, weight );
    }
    
    
  }
}

void HNL_SignalStudies::RunLeptonChannel(HNL_LeptonCore::Channel channel_ID, std::vector<Lepton *> LepsT,std::vector<Lepton *> LepsV, std::vector<Tau> TauColl,  std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<Jet> VBFJetColl,  std::vector<FatJet> FatjetColl,std::vector<Jet> BJetColl,   Event Ev, AnalyzerParameter param,  float _weight)   {
  

  FillHist (GetChannelString(channel_ID)+"_NoCut", 1, _weight, 2, 0., 2.,"");

  HNL_LeptonCore::SearchRegion Region1 = sigmm;
  HNL_LeptonCore::SearchRegion Region2 = sigmm_17028;

  if(channel_ID == EE) {  Region1 = sigee;   Region2 = sigee_17028;}
  if(channel_ID == EMu){ Region1 = sigem; Region2 = sigem_17028;}
  
  Particle METv =GetvMET("PuppiT1xyULCorr");

  //CheckHEMIssue(channel_ID, LepsT, LepsV,  JetColl, METv, _weight, param.Name, "");


  int njets_old =  (JetColl.size() + FatjetColl.size()) ;
  int njets_vbf = (VBFJetColl.size() + FatjetColl.size());
  int njets = ( njets_vbf > njets_old) ? njets_vbf : njets_old ;

  bool dijet = (FatjetColl.size() > 0) || (JetColl.size() > 1);
  bool PassBJetMVeto = (BJetColl.size()==0);
  int NBJetLV = 0;
  for(auto ib : BJetColl){
    bool hasCloseLep=false;
    for(auto il : LepsV) {
      if(il->DeltaR(ib) < 0.4) hasCloseLep=true;
    }
    if(!hasCloseLep) NBJetLV++;
  }
  
  bool PassBJetMVetoSR1 = (NBJetLV==0);

  

  // Update NAME for Muon channel                                                                                               

  TString region1 = GetChannelString(channel_ID)+"_"+param.Name;                                                       
  
  FillEventCutflow(Region1,_weight, "SSNoCut",region1);

  // UL ANALYSIS                                                                                                                
  
  Fill_RegionPlots(channel_ID, 0, param.Name,"SSInclusive" ,  VBFJetColl, FatjetColl, LepsT,  METv, nPV, _weight);


  // UL ANALYSIS
  
  if(MCSample.Contains("Type") || MCSample.Contains("private")){
    
    vector<Gen> gen_lep= GetGenLepronsSignal();
    std::sort(gen_lep.begin(),   gen_lep.end(),        PtComparing);

    
    if(gen_lep.size() == 2){
      
      if(fabs(gen_lep[0].PID()) == 11 && fabs(gen_lep[1].PID()) ==11 ){
	
	if( (fabs(gen_lep[0].Eta()) < 2.5 && gen_lep[0].Pt() > 25) &&
	    (fabs(gen_lep[1].Eta()) < 2.5 && gen_lep[1].Pt() > 15)){
	  FillEventCutflow(Region1,_weight, "SSGen",  region1);
	  FillEventCutflow(Region1,_weight, "SSGen2", region1);
	  
	}
	else       if( (fabs(gen_lep[0].Eta()) < 2.5 && gen_lep[0].Pt() > 30) &&
		       (fabs(gen_lep[1].Eta()) < 2.5 && gen_lep[1].Pt() > 10)){
	  FillEventCutflow(Region1,_weight, "SSGen",  region1);
	}
	
	else return;
      }
      
      else       if(fabs(gen_lep[0].PID()) == 13 && fabs(gen_lep[1].PID()) ==13 ){

        if( (fabs(gen_lep[0].Eta()) < 2.4 && gen_lep[0].Pt() > 20) &&
            (fabs(gen_lep[1].Eta()) < 2.4 && gen_lep[1].Pt() > 10)){
          FillEventCutflow(Region1,_weight, "SSGen", region1);
          FillEventCutflow(Region1,_weight, "SSGen2", region1);

        }
        else       if( (fabs(gen_lep[0].Eta()) < 2.4 && gen_lep[0].Pt() > 30) &&
                       (fabs(gen_lep[1].Eta()) < 2.4 && gen_lep[1].Pt() > 5)){
          FillEventCutflow(Region1,_weight, "SSGen", region1);
        }

        else return;
      }
      else       if(fabs(gen_lep[0].PID()) == 13 && fabs(gen_lep[1].PID()) ==11 ){

        if( (fabs(gen_lep[0].Eta()) < 2.4 && gen_lep[0].Pt() > 25) &&
            (fabs(gen_lep[1].Eta()) < 2.5 && gen_lep[1].Pt() > 15)){
          FillEventCutflow(Region1,_weight, "SSGen", region1);
          FillEventCutflow(Region1,_weight, "SSGen2", region1);

        }
        else       if( (fabs(gen_lep[0].Eta()) < 2.4 && gen_lep[0].Pt() > 30) &&
                       (fabs(gen_lep[1].Eta()) < 2.5 && gen_lep[1].Pt() > 10)){
          FillEventCutflow(Region1,_weight, "SSGen", region1);

        }

        else return;
      }

      else       if(fabs(gen_lep[0].PID()) == 11 && fabs(gen_lep[1].PID()) ==13 ){

        if( (fabs(gen_lep[0].Eta()) < 2.5 && gen_lep[0].Pt() > 25) &&
            (fabs(gen_lep[1].Eta()) < 2.4 && gen_lep[1].Pt() > 15)){
          FillEventCutflow(Region1,_weight, "SSGen", region1);
          FillEventCutflow(Region1,_weight, "SSGen2", region1);

        }
        else       if( (fabs(gen_lep[0].Eta()) < 2.5 && gen_lep[0].Pt() > 30) &&
                       (fabs(gen_lep[1].Eta()) < 2.4 && gen_lep[1].Pt() > 5)){
          FillEventCutflow(Region1,_weight, "SSGen", region1);

	}

	else return;
      }
    }      
  }
  
  
  if (!PassTriggerSelection(channel_ID, Ev, LepsT,"Full",false)) return;


  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"Trig",region1);

  if (!(PassTriggerSelection(channel_ID, Ev, LepsT,"Dilep",false) || PassTriggerSelection(channel_ID, Ev, LepsT,"HighPt",false) || PassTriggerSelection(channel_ID, Ev, LepsT,"Lep",false))) return;

  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"Trig2",region1);

  if (LepsT.size() != 2) return;

  if(!SameCharge(LepsT)) return;

  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID),region1);

  // PASS DIMUON TRIGGER                                                                                                                                             
  if (!PassTriggerSelection(channel_ID, Ev, LepsT,"Dilep",false)) return;
  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"Trig2L",region1);


  if (!CheckLeptonFlavourForChannel(channel_ID, LepsV)) return;

  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_Pt",region1);

  if (LepsV.size() != 2) return;
  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_LepVeto",region1);
  
  if(!PassHEMVeto(channel_ID, LepsT)) return;
  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_HEMVeto",region1);
  

  if (channel_ID==EE  && (fabs(GetLLMass(LepsT)-90.) < 15)) return;

  if(GetLLMass(LepsT) > 10.) {
    
    param.Name = GetChannelString(channel_ID)+"_"+param.DefName;
    
    FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_LLMass",region1);

    if(njets > 0 ){
      FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_Jet",region1);
      
      if(PassBJetMVetoSR1) {
	FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_BJet",region1);
	if(dijet)        FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_DiJet",region1);
	
	
	if(FatjetColl.size() > 0){
	  
	  if(RunSignalRegionAK8( channel_ID, Inclusive, LepsT, LepsV, TauColl,JetColl, FatjetColl, BJetColl, Ev, METv, param,  "", _weight ))  
	    {
	      FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_SR1",region1);
	      
	    }
	  
	  else FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_SR1Fail",region1);
	}
	else{
	  
	  if(PassVBFInitial(VBFJetColl)&&RunSignalRegionWW(channel_ID, Inclusive, LepsT, LepsV, TauColl,  JetCollLoose, VBFJetColl, FatjetColl, BJetColl, Ev,  METv, param,  "", _weight )){
	    FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_SR2",region1);
	    
	    
	  }
	  else{
	    
	    if(RunSignalRegionAK4(channel_ID, Inclusive, LepsT, LepsV ,TauColl, JetColl, FatjetColl, BJetColl, Ev, METv, param, "", _weight ))             FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_SR3",region1);
	    
	    
	    else if(JetColl.size() < 2 && LepsT[1]->Pt() >80.)             FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_SR4",region1);
	    
	    else  FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_SR3Fail",region1);
	    
	  } // FAIL SR2                                                                                                       
	}// Fail SR1                                                                                                          
      }// Fail Tau                                                                                                            
      }// NJET                                                                                                                  
    else if (LepsT[1]->Pt() >80.)   FillEventCutflow(Region1,_weight, "SS"+GetChannelString(channel_ID)+"_SR5",region1);
  } // LLMASS      
  
  
  return;

  
}


  


HNL_SignalStudies::HNL_SignalStudies(){


}
 
HNL_SignalStudies::~HNL_SignalStudies(){

}


void HNL_SignalStudies::MakeType1SSWWSignalPlots(TString process, bool apply_reco_cut){

  if(apply_reco_cut) process=process+"_PtEtaReq";

  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    
    if(apply_reco_cut){
      if( (fabs(gen.PID()) == 13 )|| (fabs(gen.PID()) == 11 )){
	if(gen.Pt() < 10. || fabs(gen.Eta()) > 2.4) return;
	
      }
    }																													
  }
  

  /////////////

  Gen Lep1;
  Gen Lep2;
  Gen j1,j2;
  bool j1IsSet(false);
  bool l1IsSet(false);

  TString LepFl_l1, LepFl_l2;
  int Lep_Mother_ind(-1);

  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    if( ! ( ( fabs(gen.PID()) == 13)  || (fabs(gen.PID()) == 11) )) continue;
    if (gen.Status() == 23){
      TString LepFl = (fabs(gen.PID()) == 13) ? "Mu" : "El";

      if(!l1IsSet) {  Lep1= gen; l1IsSet=true;LepFl_l1= LepFl;}
      else { Lep2 = gen; LepFl_l2= LepFl;}
      Lep_Mother_ind = gen.MotherIndex();
    }
  }

  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);

    if (gen.MotherIndex() == Lep_Mother_ind){
      if(fabs(gen.PID()) > 6) continue;
      if(!j1IsSet) {  j1= gen; j1IsSet=true;}
      else j2 = gen;
    }
  }
  double ZeroCheck = Lep1.Pt() * Lep2.Pt()*j1.Pt() *j2.Pt() ;
  if(ZeroCheck == 0.){
    cout << "Lep1 " << Lep1.Pt() << endl;
    cout << "Lep2 " << Lep2.Pt() << endl;
    cout <<  "j1 " << j1.Pt() << endl;
    cout <<  "j2 " << j2.Pt() << endl;
    PrintGen(gens);
    exit(EXIT_FAILURE);
  }
  
  FillHist( "SignalGen"+process+"/Jet1_eta", j1.Eta(),  1.,100, -5., 5,"#eta");
  FillHist( "SignalGen"+process+"/Jet2_eta", j2.Eta(),  1.,100, -5., 5,"#eta");
  FillHist( "SignalGen"+process+"/Jet_pt",  j1.Pt(), 1., 200, 0., 1000.,"pt (GeV)");
  FillHist( "SignalGen"+process+"/Jet_pt",  j2.Pt(), 1., 200, 0., 1000.,"pt (GeV)");
  FillHist( "SignalGen"+process+"/Dijet_mass",  (j1+j2).M(),  1., 50, 0., 2000.,"m(jj) GeV");


  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_pt",  Lep1.Pt(), 1., 200, 0., 1000.,"pt (GeV)");
  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_eta", Lep1.Eta(),  1.,100, -5., 5,"#eta");
  FillHist( "SignalGen"+process+"/Lep_DeltaR_JJ", (j1+j2).DeltaR(Lep1), 1., 100, 0., 10.,"#DeltaR (jj,l1)");
  FillHist( "SignalGen"+process+"/Lep_DeltaR_JJ", (j1+j2).DeltaR(Lep2), 1., 100, 0., 10.,"#DeltaR (jj,l2)");
  FillHist( "SignalGen"+process+"/Mass_lep1_JJ", (j1+j2 + Lep1).M(), 1., 100, 0., 5000.,"M(l1jj) GeV");
  FillHist( "SignalGen"+process+"/Mass_lep2_JJ", (j1+j2 + Lep2).M(), 1., 100, 0., 5000.,"M(l2jj) GeV");
  FillHist( "SignalGen"+process+"/Mass_llJJ", (j1+j2 +Lep1+ Lep2).M(), 1., 100, 0., 5000.,"M(lljj) GeV ");

  FillHist("SignalGen"+process+"/DeltaPhi_Lep1_J1",  fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep1.Phi() )))),  1., 200, 0., 5.,"#Delta#phi (l1,j1)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep2_J1",  fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep2.Phi() )))),  1., 200, 0., 5.,"#Delta#phi (l2,j1)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep1_J2",  fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep1.Phi() )))),  1., 200, 0., 5.,"#Delta#phi (l1,j2)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep2_J2",  fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep2.Phi() )))),  1., 200, 0., 5.,"#Delta#phi (l2,j2)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep1_Lep2",  fabs(TVector2::Phi_mpi_pi( ( (Lep1.Phi() - Lep2.Phi() )))),  1., 200, 0., 5.,"#Delta#phi (l,l)");
  FillHist("SignalGen"+process+"/DeltaPhi_J1_J2",  fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - j2.Phi() )))),  1., 200, 0., 5.,"#Delta#phi (j,j)");

  double max_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep1.Phi() ))));
  if (fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep2.Phi() )))) > max_dpiphi)  max_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep2.Phi() ))));
  if (fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep1.Phi() )))) > max_dpiphi)  max_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep1.Phi() ))));
  if (fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep2.Phi() )))) > max_dpiphi)  max_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep2.Phi() ))));
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_J_Max",  max_dpiphi,  1., 200, 0., 10.,"#Delta#phi max(l,j)");

  double min_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep1.Phi() ))));
  if (fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep2.Phi() )))) < min_dpiphi)  min_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j1.Phi() - Lep2.Phi() ))));
  if (fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep1.Phi() )))) < min_dpiphi)  min_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep1.Phi() ))));
  if (fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep2.Phi() )))) < min_dpiphi)  min_dpiphi = fabs(TVector2::Phi_mpi_pi( ( (j2.Phi() - Lep2.Phi() ))));
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_J_Min",  min_dpiphi,  1., 200, 0., 5.,"#Delta#phi min(l,j)");


  FillHist("SignalGen"+process+"/DeltaR_Lep1_J1",  j1.DeltaR(Lep1), 1., 100, 0., 10.,"#DeltaR (j1,l1)");
  FillHist("SignalGen"+process+"/DeltaR_Lep2_J1",   j1.DeltaR(Lep2), 1., 100, 0., 10.,"#DeltaR (j1,l2)");
  FillHist("SignalGen"+process+"/DeltaR_Lep1_J2", j2.DeltaR(Lep1), 1., 100, 0., 10.,"#DeltaR (j2,l1)"); 
  FillHist("SignalGen"+process+"/DeltaR_Lep2_J2",  j2.DeltaR(Lep2), 1., 100, 0., 10.,"#DeltaR (j2,l2)");
  FillHist("SignalGen"+process+"/DeltaR_Lep1_Lep2",Lep2.DeltaR(Lep1), 1., 100, 0., 10.,"#DeltaR (l,l)");
  FillHist("SignalGen"+process+"/DeltaR_J1_J2",  j1.DeltaR(j2), 1., 100, 0., 10.,"#DeltaR (j1,j2)");

  double max_dpiR = j1.DeltaR(Lep1);
  if (j1.DeltaR(Lep2) > max_dpiR) max_dpiR = j1.DeltaR(Lep2);
  if (j2.DeltaR(Lep1) > max_dpiR) max_dpiR = j2.DeltaR(Lep1);
  if (j2.DeltaR(Lep2) > max_dpiR) max_dpiR = j2.DeltaR(Lep2);

  FillHist("SignalGen"+process+"/DeltaR_Lep_J_Max",  max_dpiR,  1., 200, 0., 5.,"#DeltaR max(L,J");

  double min_dpiR = j1.DeltaR(Lep1);
  if (j1.DeltaR(Lep2) < min_dpiR) min_dpiR = j1.DeltaR(Lep2);
  if (j2.DeltaR(Lep1) < min_dpiR) min_dpiR = j2.DeltaR(Lep1);
  if (j2.DeltaR(Lep2) < min_dpiR) min_dpiR = j2.DeltaR(Lep2);

  FillHist("SignalGen"+process+"/DeltaR_Lep_J_Min",  min_dpiR,  1., 200, 0., 5.,"#DeltaR min(L,J)");



  Particle JJMEta = j1+j2;

  FillHist("SignalGen"+process+"/MaxDEtaVBF_Jets_MJJ",JJMEta.M()   , 1, 100, 0., 4000., "MaxDEta MJJ");
  
  double maxDiJetDeta=fabs(j1.Eta() - j2.Eta());
  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max(Lep1.Eta()  - Av_JetEta , Lep2.Eta()  - Av_JetEta ) /maxDiJetDeta;
  FillHist( "SignalGen"+process+"/MaxDEtaVBF_Jets_zeppenfeld", zeppenfeld  , 1, 200, 0., 2., "zeppenfeld");




  
  return ;
}




void HNL_SignalStudies::MakeType1VBFSignalPlots(TString process, bool apply_reco_cut){

  int N_Mother_ind(-1); // Index of N                                                                                                                                                                             
  unsigned int W2_ind(0); // Index of W2 : i.e W from N decay                                                                                                                                                     
  
  TString  mu_ch="";
  
  gens = GetGens();
  
  if(apply_reco_cut) process=process+"_PtEtaReq";


  // First loop over gen collection to get index of W/N and apply pt/eta cuts if needed                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);

    if(gens.at(gen.MotherIndex()).PID() == 9900012 || gens.at(gen.MotherIndex()).PID() == 9900014){

      if(apply_reco_cut){
        if( (fabs(gen.PID()) == 13 )|| (fabs(gen.PID()) == 11 )){
          if(gen.Pt() < 10. || fabs(gen.Eta()) > 2.4) return;
        }
      }
    }


    if(fabs(gen.PID()) == 24 && (gens.at(gen.MotherIndex()).PID() == 9900012 || gens.at(gen.MotherIndex()).PID() == 9900014)){
      W2_ind= i;
      // If W decays to W in gen then get daughter                                                                                                                                                                                                                                                                                                                                                                                    
      for(unsigned int i2=2; i2<gens.size(); i2++){
        Gen gen2 = gens.at(i2);
        if(gen2.MotherIndex() == W2_ind){
          if (fabs(gen2.PID()) == 24 ) W2_ind = i2;
        }
      }
    }

    if((gen.PID() == 9900012 || gen.PID() == 9900014) &&  gen.Status() == 22) {
      N_Mother_ind= gen.MotherIndex();
    }
  }

  // Loop over again to cut on Lep from W1                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);

    if(gen.MotherIndex() == N_Mother_ind){
      if(apply_reco_cut){
        if( (fabs(gen.PID()) == 13 )|| (fabs(gen.PID()) == 11 )){
          if(gen.Pt() < 10. || fabs(gen.Eta()) > 2.4) return;
        }
      }
    }
  }


  Gen LepFromN;
  Gen LepFromW;
  Gen N;
  Gen W2;
  Gen J, j1,j2;
  bool j1IsSet(false);

  TString LepFl_l1, LepFl_l2;

  // Loop over Gen collection and Natch N/W/l/j objects                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);

    if(int(gen.MotherIndex()) == W2_ind && gen.Status() == 23) {
      W2 = gens.at(gen.MotherIndex());
      if(!j1IsSet) {  j1= gen; j1IsSet=true;}
      else j2 = gen;
    }
    if(gen.PID() == 9900012 || gen.PID() == 9900014){
      N= gens.at(i);
    }

    if( fabs(gen.PID()) < 7 && gen.Status() == 23){
      if(gen.MotherIndex() == N_Mother_ind) J=gen;
    }
    
    if( ! ( ( fabs(gen.PID()) == 13)  || (fabs(gen.PID()) == 11) )) continue;

    TString LepFl = (fabs(gen.PID()) == 13) ? "Mu" : "El";

    if(gens.at(gen.MotherIndex()).PID() == 9900012|| gens.at(gen.MotherIndex()).PID() == 9900014) {
      LepFromN = gen;
      LepFl_l1=LepFl;
    }
    else if(gen.MotherIndex() == N_Mother_ind){
      LepFromW=gen;
      LepFl_l2=LepFl;
    }
  }
  
  
  FillHist( "SignalGen"+process+"/Jet_FromW_eta", j1.Eta(),  1.,100, -5., 5,"J #eta");
  FillHist( "SignalGen"+process+"/Jet_FromW_eta", j2.Eta(),  1.,100, -5., 5,"J #eta");
  FillHist( "SignalGen"+process+"/Jet_FromW_pt",  j1.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/Jet_FromW_pt",  j2.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/W_Dijet_mass",  W2.M(),  1., 200, 0., 200.,"W(jj) Mass (GeV)");

  FillHist( "SignalGen"+process+"/InitialQ_Jet_eta", J.Eta(),  1.,100, -5., 5,"VBF_J #eta");
  FillHist( "SignalGen"+process+"/InitialQ_Jet_pt",  J.Pt(), 1., 200, 0., 400.,"VBF_J p_{T} (GeV)");

  FillHist( "SignalGen"+process+"/Neutrino_pt", N.Pt(), 1., 200, 0., 400.,"N p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/Neutrino_mass", N.M(), 1., 200, 0., 2000.,"m_N (GeV)");
  FillHist( "SignalGen"+process+"/Neutrino_eta", N.Eta(), 1.,100, -5., 5,"N #eta");

  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_pt",  LepFromN.Pt(), 1., 200, 0., 400.,"#ell_N p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_eta", LepFromN.Eta(),  1.,100, -5., 5,"#ell_N #eta");
  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_DeltaR_W_From_Neutrino", W2.DeltaR(LepFromN), 1., 100, 0., 10.,"#Delta R(ln,W2)");
  FillHist( "SignalGen"+process+"/Mass_"+LepFl_l1+"_From_Neutrino_AND_W_FromNeutrino", (W2 + LepFromN).M(), 1., 150, 0., 1500.,"M(ln+W2) GeV");

  if (LepFromN.PID() < 0){

    FillHist( "SignalGenPlusQ"+process+"/Jet_FromW_eta", j1.Eta(),  1.,100, -5., 5,"J #eta");
    FillHist( "SignalGenPlusQ"+process+"/Jet_FromW_eta", j2.Eta(),  1.,100, -5., 5,"J #eta");
    FillHist( "SignalGenPlusQ"+process+"/Jet_FromW_pt",  j1.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
    FillHist( "SignalGenPlusQ"+process+"/Jet_FromW_pt",  j2.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
    FillHist( "SignalGenPlusQ"+process+"/W_Dijet_mass",  W2.M(),  1., 200, 0., 200.,"W(jj) Mass (GeV)");

    FillHist( "SignalGenPlusQ"+process+"/InitialQ_Jet_eta", J.Eta(),  1.,100, -5., 5,"VBF_J #eta");
    FillHist( "SignalGenPlusQ"+process+"/InitialQ_Jet_pt",  J.Pt(), 1., 200, 0., 400.,"VBF_J p_{T} (GeV)");

    FillHist( "SignalGenPlusQ"+process+"/Neutrino_pt", N.Pt(), 1., 200, 0., 400.,"N p_{T} (GeV)");
    FillHist( "SignalGenPlusQ"+process+"/Neutrino_mass", N.M(), 1., 200, 0., 2000.,"m_N (GeV)");
    FillHist( "SignalGenPlusQ"+process+"/Neutrino_eta", N.Eta(), 1.,100, -5., 5,"N #eta");

    FillHist( "SignalGenPlusQ"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_pt",  LepFromN.Pt(), 1., 200, 0., 400.,"#ell_N p_{T} (GeV)");
    FillHist( "SignalGenPlusQ"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_eta", LepFromN.Eta(),  1.,100, -5., 5,"#ell_N #eta");
    FillHist( "SignalGenPlusQ"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_DelaR_W_From_Neutrino", W2.DeltaR(LepFromN), 1., 100, 0., 10.,"#Delta R(ln,W)");
    FillHist( "SignalGenPlusQ"+process+"/Mass_"+LepFl_l1+"_From_Neutrino_AND_W_FromNeutrino", (W2 + LepFromN).M(), 1., 150, 0., 1500.,"M(ln+W2) GeV");
   
  }
  else{
    FillHist( "SignalGenMinusQ"+process+"/Jet_FromW_eta", j1.Eta(),  1.,100, -5., 5,"J #eta");
    FillHist( "SignalGenMinusQ"+process+"/Jet_FromW_eta", j2.Eta(),  1.,100, -5., 5,"J #eta");
    FillHist( "SignalGenMinusQ"+process+"/Jet_FromW_pt",  j1.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
    FillHist( "SignalGenMinusQ"+process+"/Jet_FromW_pt",  j2.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
    FillHist( "SignalGenMinusQ"+process+"/W_Dijet_mass",  W2.M(),  1., 200, 0., 200.,"W(jj) Mass (GeV)");

    FillHist( "SignalGenMinusQ"+process+"/InitialQ_Jet_eta", J.Eta(),  1.,100, -5., 5,"VBF_J #eta");
    FillHist( "SignalGenMinusQ"+process+"/InitialQ_Jet_pt",  J.Pt(), 1., 200, 0., 400.,"VBF_J p_{T} (GeV)");

    FillHist( "SignalGenMinusQ"+process+"/Neutrino_pt", N.Pt(), 1., 200, 0., 400.,"N p_{T} (GeV)");
    FillHist( "SignalGenMinusQ"+process+"/Neutrino_mass", N.M(), 1., 200, 0., 2000.,"m_N (GeV)");
    FillHist( "SignalGenMinusQ"+process+"/Neutrino_eta", N.Eta(), 1.,100, -5., 5,"N #eta");

    FillHist( "SignalGenMinusQ"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_pt",  LepFromN.Pt(), 1., 200, 0., 1000.,"#ell_N p_{T} (GeV)");
    FillHist( "SignalGenMinusQ"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_eta", LepFromN.Eta(),  1.,100, -5., 5,"#ell_N #eta");
    FillHist( "SignalGenMinusQ"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_DelaR_W_From_Neutrino", W2.DeltaR(LepFromN), 1., 100, 0., 10.,"#Delta R(ln,W)");
    FillHist( "SignalGenMinusQ"+process+"/Mass_"+LepFl_l1+"_From_Neutrino_AND_W_FromNeutrino", (W2 + LepFromN).M(), 1., 150, 0., 1500.,"M(ln+W2) GeV");
  }

  FillHist( "SignalGen"+process+"/" + mu_ch+"Lep_"+LepFl_l2+"_FromW_pt", LepFromW.Pt(), 1., 200, 0., 400.,"#ell_w p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/" + mu_ch+"Lep_"+LepFl_l2+"_FromW_eta", LepFromW.Eta(),  1.,100, -5., 5,"#ell_w #eta");

  FillHist("SignalGen"+process+"/Nlep_Wlep_pt", LepFromN.Pt(), LepFromW.Pt(), 1., 500,0., 2000.,500, 0.,2000.);

  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_N",  N.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(N,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_W",  W2.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(W2,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromW_N",  N.DeltaR(LepFromW), 1., 200, 0., 5.,"#DeltaR(N,lw)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromW_W",  W2.DeltaR(LepFromW), 1., 200, 0., 5.,"#DeltaR(W2,lw)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_J1",  j1.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(j1,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_J2",  j2.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(j2,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromW_LepFromN",  LepFromW.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(lw,ln)");
  FillHist("SignalGen"+process+"/DeltaR_W_N",  W2.DeltaR(N), 1., 200, 0., 5.,"#DeltaR(N,W2)");


  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromN_N",  fabs(TVector2::Phi_mpi_pi( ( (N.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromN_W",  fabs(TVector2::Phi_mpi_pi( ( (W2.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W2,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromW_N",  fabs(TVector2::Phi_mpi_pi( ( (N.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,lw)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromW_W",  fabs(TVector2::Phi_mpi_pi( ( (W2.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W2,lw)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromW_LepFromN",  fabs(TVector2::Phi_mpi_pi( ( (LepFromN.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(lw,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_W_N",  fabs(TVector2::Phi_mpi_pi( ( (W2.Phi() - N.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,W2)");

  Particle WrongN = LepFromW + j1+j2;
  FillHist("SignalGen"+process+"/DeltaPhiWr_Lep_FromN_N",  fabs(TVector2::Phi_mpi_pi( ( (WrongN.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(wN,ln)");
  FillHist("SignalGen"+process+"/DeltaPhiWr_Lep_FromW_N",  fabs(TVector2::Phi_mpi_pi( ( (WrongN.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(wN,lw)");

  Particle W1 = (N+LepFromW);
  FillHist("SignalGen"+process+"/DeltaPhi_W1_Lep_FromN",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W1,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_W1_Lep_FromW",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W1,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_W1_N",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - N.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,W1)");
  FillHist("SignalGen"+process+"/DeltaPhi_W1_W2",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - W2.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W1,W2)");
  FillHist("SignalGen"+process+"/Mass_NpWl",  (N+LepFromW).M(), 1., 200, 0., 2000.,"M(N,Wl) GeV");


  
  vector<Particle> JetsColl = {J,j1,j2};
  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < JetsColl.size()-1; ij++){
    for(unsigned int ij2 = ij+1; ij2 < JetsColl.size(); ij2++){

      double deta = fabs(JetsColl[ij].Eta() - JetsColl[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }
  

  Particle JJMEta = JetsColl[ijet1] + JetsColl[ijet2];

  FillHist("SignalGen"+process+"/MaxDEtaVBF_Jets_MJJ",JJMEta.M()   , 1, 100, 0., 4000., "MaxDEta MJJ (GeV)");

  double Av_JetEta= 0.5*(JetsColl[ijet1].Eta()+ JetsColl[ijet2].Eta());
  double zeppenfeld = TMath::Max(LepFromW.Eta()  - Av_JetEta , LepFromN.Eta()  - Av_JetEta ) /maxDiJetDeta;
  FillHist( "SignalGen"+process+"/MaxDEtaVBF_Jets_zeppenfeld", zeppenfeld  , 1, 200, 0., 2., "zeppenfeld");


  double ZeroCheck = LepFromN.Pt()*LepFromW.Pt()*N.Pt()*W2.Pt()*J.Pt() * j1.Pt()*j1.Pt();
  if(ZeroCheck == 0.){
    cout << "LepFromN.Pt()  " << LepFromN.Pt() << endl;
    cout << "LepFromW.Pt()  " << LepFromW.Pt() << endl;
    cout << "N.Pt()  " << N.Pt() << endl;
    cout << "W2.Pt()  " << W2.Pt() << endl;
    cout << "Init Q Pt()  " << J.Pt() << endl;
    cout << "j1.Pt()  " << j1.Pt() << endl;
    cout << "j2.Pt()  " << j2.Pt() << endl;
    
    PrintGen(gens);
    exit(EXIT_FAILURE);
  }
  if(LepFromN.Pt() == 0 || LepFromW.Pt() == 0.) {
    cout << "LepFromN pt = " << LepFromN.Pt() << "  LepFromW pt = " << LepFromW.Pt() << endl;
    PrintGen(gens);
    exit(EXIT_FAILURE);
  }

  return ;
}



void HNL_SignalStudies::MakeType1SignalPlots(TString process, bool apply_reco_cut){
  
  int N_Mother_ind(-1); // Index of N
  unsigned int W2_ind(0); // Index of W2 : i.e W from N decay 
  TString  mu_ch="";

  gens = GetGens();
  
  if(apply_reco_cut) process=process+"_PtEtaReq";
  

  // First loop over gen collection to get index of W/N and apply pt/eta cuts if needed
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    
    if(gens.at(gen.MotherIndex()).PID() == 9900012 || gens.at(gen.MotherIndex()).PID() == 9900014){

      if(apply_reco_cut){
	if( (fabs(gen.PID()) == 13 )|| (fabs(gen.PID()) == 11 )){
	  if(gen.Pt() < 10. || fabs(gen.Eta()) > 2.4) return;
	}
      }
    }
   
    
    if(fabs(gen.PID()) == 24 && (gens.at(gen.MotherIndex()).PID() == 9900012 || gens.at(gen.MotherIndex()).PID() == 9900014)){
      W2_ind= i;
      // If W decays to W in gen then get daughter
      
      for(unsigned int i2=2; i2<gens.size(); i2++){
	Gen gen2 = gens.at(i2);
	if(gen2.MotherIndex() == W2_ind){
	  if (fabs(gen2.PID()) == 24 ) W2_ind = i2;
	}
      }
    }
    
    if((gen.PID() == 9900012 || gen.PID() == 9900014) &&  gen.Status() == 22) {
      N_Mother_ind= gen.MotherIndex();
    }
  }

  // Loop over again to cut on Lep from W1
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    
    if(gen.MotherIndex() == N_Mother_ind){
      if(apply_reco_cut){
        if( (fabs(gen.PID()) == 13 )|| (fabs(gen.PID()) == 11 )){
          if(gen.Pt() < 10. || fabs(gen.Eta()) > 2.4) return;
        }
      }
    }
  }

  
  Gen LepFromN;
  Gen LepFromW;
  Gen N;
  Gen W2;
  Gen j1,j2;
  bool j1IsSet(false);

  TString LepFl_l1, LepFl_l2;
  
  // Loop over Gen collection and Natch N/W/l/j objects
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    
    if(int(gen.MotherIndex()) == W2_ind && gen.Status() == 23) {
      W2 = gens.at(gen.MotherIndex());
      if(!j1IsSet) {  j1= gen; j1IsSet=true;}
      else j2 = gen;
    }
    if(gen.PID() == 9900012 || gen.PID() == 9900014){
      N= gens.at(i);
    }
    
    if( ! ( ( fabs(gen.PID()) == 13)  || (fabs(gen.PID()) == 11) )) continue;
    
    TString LepFl = (fabs(gen.PID()) == 13) ? "Mu" : "El";

    if(gens.at(gen.MotherIndex()).PID() == 9900012|| gens.at(gen.MotherIndex()).PID() == 9900014) {
      LepFromN = gen;
      LepFl_l1=LepFl;
    }
    else if(gen.MotherIndex() == N_Mother_ind){
      LepFromW=gen;
      LepFl_l2=LepFl;
    }
  }
  
  if(LepFromN.Pt() == 0 || LepFromW.Pt() == 0.) {
    cout << "LepFromN pt = " << LepFromN.Pt() << "  LepFromW pt = " << LepFromW.Pt() << endl;
    PrintGen(gens);
    exit(EXIT_FAILURE);
  }
  

  
  // PLOTS


  FillHist( "SignalGen"+process+"/Jet_FromW_eta", j1.Eta(),  1.,100, -5., 5,"J #eta");
  FillHist( "SignalGen"+process+"/Jet_FromW_eta", j2.Eta(),  1.,100, -5., 5,"J #eta");
  FillHist( "SignalGen"+process+"/Jet_FromW_pt",  j1.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/Jet_FromW_pt",  j2.Pt(), 1., 200, 0., 400.,"J p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/W_Dijet_mass",  W2.M(),  1., 200, 0., 200.,"W(jj) Mass (GeV)");


  FillHist( "SignalGen"+process+"/Neutrino_pt", N.Pt(), 1., 200, 0., 400.,"N p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/Neutrino_mass", N.M(), 1., 200, 0., 2000.,"m_N (GeV)");
  FillHist( "SignalGen"+process+"/Neutrino_eta", N.Eta(), 1.,100, -5., 5,"N #eta");

  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_pt",  LepFromN.Pt(), 1., 200, 0., 400.,"#ell_N p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_eta", LepFromN.Eta(),  1.,100, -5., 5,"#ell_N #eta");
  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l1+"_From_Neutrino_DeltaR_W_From_Neutrino", W2.DeltaR(LepFromN), 1., 100, 0., 10.,"#Delta R(ln,W2)");
  FillHist( "SignalGen"+process+"/Mass_"+LepFl_l1+"_From_Neutrino_AND_W_FromNeutrino", (W2 + LepFromN).M(), 1., 150, 0., 1500.,"M(ln+W2) GeV");

  
  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l2+"_FromW_pt", LepFromW.Pt(), 1., 200, 0., 400.,"#ell_w p_{T} (GeV)");
  FillHist( "SignalGen"+process+"/Lep_"+LepFl_l2+"_FromW_eta", LepFromW.Eta(),  1.,100, -5., 5,"#ell_w #eta ");
  
  
  FillHist("SignalGen"+process+"/Nlep_Wlep_pt", LepFromN.Pt(), LepFromW.Pt(), 1., 500,0., 2000.,500, 0.,2000.);

  if( (LepFromN.Pt() < 50) || ( LepFromW.Pt() < 50)){
    if(LepFromN.Pt() < LepFromW.Pt()) FillHist("SignalGen"+process+"/Pt_LT50_NOrderPt", 1, 1, 2, 0, 2);
    else FillHist("SignalGen"+process+"/Pt_LT50_NOrderPt",0, 1, 2, 0, 2);
  }
  if( (LepFromN.Pt() < 100) || ( LepFromW.Pt() <100)){
    if(LepFromN.Pt() < LepFromW.Pt()) FillHist("SignalGen"+process+"/Pt_LT100_NOrderPt",1, 1, 2, 0, 2);
    else FillHist("SignalGen"+process+"/Pt_LT100_NOrderPt",0, 1, 2, 0, 2);
  }
  
  if(LepFromN.Pt() < LepFromW.Pt()) FillHist("SignalGen"+process+"/Pt_NOrderPt",1, 1, 2, 0, 2);
  else FillHist("SignalGen"+process+"/Pt_NOrderPt",0, 1, 2, 0, 2);
  
  if(j1.Pt() == 0  || j2.Pt() == 0.)      {
    cout << "J1 pt = " << j1.Pt() << " J2 pt = " << j2.Pt() << endl;   
    PrintGen(gens);
    exit(EXIT_FAILURE);
  }


  //cout << "LepFromN pt = " << LepFromN.Pt() << "  LepFromW pt = " << LepFromW.Pt() << endl;

  double dphi1 = fabs(TVector2::Phi_mpi_pi( ( ((LepFromN + j1+j2).Phi() - LepFromW.Phi() ))));
  double dphi2 = fabs(TVector2::Phi_mpi_pi( ( ((LepFromW + j1+j2).Phi() - LepFromN.Phi() ))));
  double Nphi1 =  N.Phi();
  double Nphi2 =  (LepFromN + j1+j2).Phi(); 
  
  // if( Nphi1 !=  Nphi2) {
  //  
  //  cout << Nphi1 << " " << Nphi2 << endl;
  //  cout << (LepFromN + j1+j2).Phi() << " " << N.Phi() << endl;
  //  cout << (LepFromN + j1+j2).M() << " " << N.M() << endl;
  //  cout << (j1+j2).M() << " " << W2.M() << endl;
  //  PrintGen(gens);
  //  exit(EXIT_FAILURE);
  // }


  if(dphi1 > dphi2) FillHist("SignalGen"+process+"/DPhi_NOrder_",1, 1, 2, 0, 2);
  else FillHist("SignalGen"+process+"/DPhi_NOrder_",0, 1, 2, 0, 2);
  
  if(W2.DeltaR(LepFromW) > W2.DeltaR(LepFromN)) FillHist("SignalGen"+process+"/DPhiW_NOrder_",1, 1, 2, 0, 2);
  else FillHist("SignalGen"+process+"/DPhiW_NOrder_",0, 1, 2, 0, 2);

  


  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_N",  N.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(N,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_W",  W2.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(W2,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromW_N",  N.DeltaR(LepFromW), 1., 200, 0., 5.,"#DeltaR(N,lw)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromW_W",  W2.DeltaR(LepFromW), 1., 200, 0., 5.,"#DeltaR(W2,lw)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_J1",  j1.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(j1,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromN_J2",  j2.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(j2,ln)");
  FillHist("SignalGen"+process+"/DeltaR_Lep_FromW_LepFromN",  LepFromW.DeltaR(LepFromN), 1., 200, 0., 5.,"#DeltaR(lw,ln)");
  FillHist("SignalGen"+process+"/DeltaR_W_N",  W2.DeltaR(N), 1., 200, 0., 5.,"#DeltaR(N,W2)");


  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromN_N",  fabs(TVector2::Phi_mpi_pi( ( (N.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromN_W",  fabs(TVector2::Phi_mpi_pi( ( (W2.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W2,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromW_N",  fabs(TVector2::Phi_mpi_pi( ( (N.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,lw)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromW_W",  fabs(TVector2::Phi_mpi_pi( ( (W2.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W2,lw)");
  FillHist("SignalGen"+process+"/DeltaPhi_Lep_FromW_LepFromN",  fabs(TVector2::Phi_mpi_pi( ( (LepFromN.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(lw,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_W_N",  fabs(TVector2::Phi_mpi_pi( ( (W2.Phi() - N.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,W2)");

  Particle WrongN = LepFromW + j1+j2;
  FillHist("SignalGen"+process+"/DeltaPhiWr_Lep_FromN_N",  fabs(TVector2::Phi_mpi_pi( ( (WrongN.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(wN,ln)");
  FillHist("SignalGen"+process+"/DeltaPhiWr_Lep_FromW_N",  fabs(TVector2::Phi_mpi_pi( ( (WrongN.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(wN,lw)");

  Particle W1 = (N+LepFromW);
  FillHist("SignalGen"+process+"/DeltaPhi_W1_Lep_FromN",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - LepFromN.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W1,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_W1_Lep_FromW",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - LepFromW.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W1,ln)");
  FillHist("SignalGen"+process+"/DeltaPhi_W1_N",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - N.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(N,W1)");
  FillHist("SignalGen"+process+"/DeltaPhi_W1_W2",  fabs(TVector2::Phi_mpi_pi( ( (W1.Phi() - W2.Phi() )))),  1., 200, 0., 5.,"#Delta#phi(W1,W2)");
  FillHist("SignalGen"+process+"/Mass_NpWl",  (N+LepFromW).M(), 1., 200, 0., 2000.,"M(N,Wl) GeV");


  if(fabs(TVector2::Phi_mpi_pi( ( (WrongN.Phi() - LepFromN.Phi() ))))  >  N.DeltaR(LepFromN) ) FillHist("SignalGen"+process+"/DPhiWrN_NOrder_",1, 1, 2, 0, 2);
  else FillHist("SignalGen"+process+"/DPhiWrN_NOrder_",0, 1, 2, 0, 2);

  return;

}


void HNL_SignalStudies::CheckHEMIssue(HNL_LeptonCore::Channel channel,std::vector<Lepton *> LepsT,std::vector<Lepton *> LepsV,       vector<Jet>& JetColl, Particle& vMET, float weight, TString Label, TString Option)
{

  if(DataYear!=2018) return;

  if( !(LepsT.size()==2)) return;
  if( !(LepsV.size()==2)) return;
  
  /* 
     FIX
  if( DataYear==2016 or DataYear==2018 ){ if(LepsT.at(0).Pt()<26) return; }
  else{                                   if(LepsT.at(0).Pt()<29) return; }
  
  const int NEtaEdges=5, NPhiEdges=9;
  double EtaEdges_p[NEtaEdges]={0., 0.6, 1.3, 1.9, 2.5};
  double PhiEdges[NPhiEdges]={-3.15, -2.4, -1.6, -0.8, 0., 0.8, 1.6, 2.4, 3.15};
  float mu2_feta=fabs(LepsT.at(1).Eta()), mu2_phi = LepsT.at(1).Phi();
  
  if(mu2_feta>0){ FillHist(LepsT[1]->GetFlavour()+"2_fEtaPhi_p"+Label, mu2_feta, mu2_phi, weight, NEtaEdges-1, EtaEdges_p, NPhiEdges-1, PhiEdges); }
  else{           FillHist(LepsT[1]->GetFlavour()"2_fEtaPhi_m"+Label, mu2_feta, mu2_phi, weight, NEtaEdges-1, EtaEdges_p, NPhiEdges-1, PhiEdges); }
  
  if(IsDATA && IsHEMIssueRun()){
    if(mu2_feta>0){ FillHist(LepsT[1]->GetFlavour()"2_fEtaPhi_p_HEMRun"+Label, mu2_feta, mu2_phi, weight, NEtaEdges-1, EtaEdges_p, NPhiEdges-1, PhiEdges); }
    else{           FillHist(LepsT[1]->GetFlavour()"2_fEtaPhi_m_HEMRun"+Label, mu2_feta, mu2_phi, weight, NEtaEdges-1, EtaEdges_p, NPhiEdges-1, PhiEdges); }
  }
  
  */
  return;

}
