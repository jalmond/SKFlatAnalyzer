#include "HNL_SignalStudies.h"

void HNL_SignalStudies::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();
  
  // Overwite JECSources in AnalyserCore
  JECSources = {"AbsoluteStat"};
  
  for(auto jec_source : JECSources){
    SetupJECUncertainty(jec_source);
  }
  
  SetupIDMVAReaderDefault();

}


void HNL_SignalStudies::executeEvent(){

  AddTimerStamp("start_ev");
  
  FillTimer("START_EV");

  //==== Gen for genmatching
  AnalyzerParameter param  = InitialiseHNLParameter("SignalStudy","UL");
  PlotBDTVariablesElectron(param);
  PlotBDTVariablesMuon(param);

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

  FillTimer("SS_EV");

  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};

  for(auto dilep_channel : channels){

    if(MCSample.Contains("Type")){
      if (!SelectChannel(dilep_channel)) continue;
    }

    TString channel = GetChannelString(dilep_channel) ;

    FillTimer("START_RUN_"+channel);
    Particle METv = GetvMET("PuppiT1xyCorr",param); // returns MET with systematic correction                                                                                                    

    
    //double ptbins[11] = { 0., 10.,15., 20., 30., 40.,50., 100.,500. ,1000., 2000.};

    std::vector<Jet>      jetsTmp     = GetJets   ( "NoID",    10., 5.);
    //for(auto ijet : jetsTmp ) ijet.PrintObject("Jet ");

    FillHist ("NoCut_"+channel, 1, weight, 2, 0., 2.,"");
    
    if(MCSample.Contains("DYType")   && MCSample.Contains("private"))  MakeType1SignalPlots(channel, false);
    if(MCSample.Contains("VBFType")  && MCSample.Contains("private"))  MakeType1VBFSignalPlots(channel, false);
    if(MCSample.Contains("SSWWType") && MCSample.Contains("private"))  MakeType1SSWWSignalPlots(channel, false);
    
    vector<Gen> gen_lep= GetGenLepronsSignal();
        
    std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
    std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);

    FillHist ("NVetoMuon_"+channel, MuonCollV.size(), weight, 4, 0., 4.,"");
    FillHist ("NVetoElectron_"+channel, ElectronCollV.size(), weight, 4, 0., 4.,"");

    std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

    std::vector<Electron>   ElectronCollAll = GetElectrons( "MVAID", 10., 2.5);
    std::vector<Muon>       MuonCollAll     = GetMuons    (  "MVAID", 10., 2.4);
 
    std::vector<Lepton *> LepsMVAID  = (dilep_channel==EE) ? MakeLeptonPointerVector(ElectronCollAll) : MakeLeptonPointerVector(MuonCollAll);
    
    vector<Jet> AllJets = GetAllJets();
    for(auto ilep : LepsMVAID){
      int nCJ(0);
      for(auto ijet : AllJets){
	if(ilep->DeltaR(ijet) < 0.4)   nCJ++;
      }	
    }

    AddTimerStamp("start_limit_Code");

    std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets("HNL",param);
    std::vector<Jet> AK4_JetAllColl                 = GetHNLJets("NoCut_Eta3",param);
    std::vector<Jet> JetColl                        = GetHNLJets("Tight",param);
    std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param);
    std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param);
    std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param);
    std::vector<Jet> BJetCollSR1                    = GetHNLJets("BJetT",param);

    std::vector<Tau>        TauColl        = GetTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

    RunAllSignalRegions(Inclusive, 
			ElectronCollAll,ElectronCollV,MuonCollAll,MuonCollV,  TauColl,
			JetCollLoose, AK4_JetAllColl, JetColl,VBF_JetColl,AK8_JetColl, BJetColl,BJetCollSR1, 
			ev,METv, param		, weight);


    FillTimer("END_Limitcode_"+channel);
    FillTimer("END_Run_"+channel);

  }
}


void HNL_SignalStudies::PlotBDTVariablesMuon(AnalyzerParameter param){

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  std::vector<Electron>   ElectronCollProbe = GetElectrons("passPOGTight", 10., 2.5);
  std::vector<Muon>       MuonCollProbe     = GetMuons    ("passProbe", 10., 2.4);

  HNL_LeptonCore::Channel dilep_channel = MuMu;

  for(auto imu : MuonCollProbe){

    TString channel_string = GetChannelString(dilep_channel) ;

    if(!imu.PassID("MVALoose")) continue;

    TString tag= imu.LepGenType(GetLeptonType_JH(imu, gens));

    TString etabin = (fabs(imu.Eta()) < 1.5) ? "BB_" : "EC_";
    TString ptbin = "Ptlt20_";
    if(imu.Pt() < 20) ptbin = "Ptlt20_";
    else if(imu.Pt() < 50) ptbin = "Ptlt50_";
    else  ptbin = "Ptgt50_";


    vector<TString> Tags = {tag+"/Lep_MVA_",
                            tag+imu.CloseJet_Flavour()+"/Lep_MVA_",
                            tag+etabin+"/El_MVA_",
                            tag+etabin+imu.CloseJet_Flavour()+"/El_MVA_",
                            tag+etabin+ptbin+"/El_MVA_",
                            tag+etabin+ptbin+imu.CloseJet_Flavour()+"/El_MVA_",
    };

    for(auto i  : Tags){

      if(i.Contains("Pileup")) continue;
      if(i.Contains("IsEWtau")) continue;
      if(i.Contains("LF") && !i.Contains("IsFake")) continue;
      if(i.Contains("HF") && !i.Contains("IsFake")) continue;

      map<TString, double> mapBDT = imu.MAPBDT();

      for(auto imap : mapBDT ) {
	FillHist(channel_string+i+"_"+imap.first, imap.second , weight, 200, -1., 1);
	if(imap.second > 0.)     FillHist(channel_string+i+"HFvar__LFcut_"+imap.first, imu.MVA() , weight, 200, -1., 1);
      }
      
      FillHist(channel_string+i+"Fake_"+channel_string, imu.MVA() , weight, 200, -1., 1);

      if(imu.MVA() > 0.5){
	for(auto imap : mapBDT ) FillHist(channel_string+i+"LFVar__HFcut_"+imap.first+"_mvacut", imap.second , weight, 200, -1., 1);
      }
      if(imu.MVA() > 0.5 && imu.HNL_MVA_Fake("v3") > 0.2){
	FillMuonKinematicPlots("muon_"+i, "PassMVA", imu, weight);
      }
    }
  }
}

void HNL_SignalStudies::PlotBDTVariablesElectron(AnalyzerParameter param){

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);
  
  std::vector<Electron>   ElectronCollProbe = GetElectrons("passProbeID", 10., 2.5);

  HNL_LeptonCore::Channel dilep_channel= EE;

  for(auto iel : ElectronCollProbe){

    TString channel_string = GetChannelString(dilep_channel) ;

    FillHist("ElectronIDPlots/"+channel_string+"_MiniIso",iel.MiniRelIso() , weight, 200, 0., 5);
    FillHist("ElectronIDPlots/"+channel_string+"_NMissingHits",iel.NMissingHits() , weight, 5, 0., 5);
    FillHist("ElectronIDPlots/"+channel_string+"_SIP3D",iel.SIP3D() , weight, 200, -10., 10);
    FillHist("ElectronIDPlots/"+channel_string+"_fdXY",iel.fdXY() , weight, 1000, -0.5, 0.5);
    FillHist("ElectronIDPlots/"+channel_string+"_fdZ",iel.fdZ() , weight, 1000, -0.5, 0.5);
    FillHist("ElectronIDPlots/"+channel_string+"_Pass_TriggerEmulationLoose",iel.Pass_TriggerEmulationLoose() , weight, 2, 0., 2);

    if(!iel.PassID("MVALoose")) continue;

    TString tag= iel.LepGenType(GetLeptonType_JH(iel, gens));
    if(IsCF(iel,gens)) tag = tag +"_IsCF";
    
    TString etabin = (fabs(iel.Eta()) < 1.5) ? "BB_" : "EC_";
    TString ptbin = "Ptlt20_";
    if(iel.Pt() < 20) ptbin = "Ptlt20_";
    else if(iel.Pt() < 50) ptbin = "Ptlt50_";
    else  ptbin = "Ptgt50_";
    
    vector<TString> Tags = {tag+"/El_MVA_",
			    tag+iel.CloseJet_Flavour()+"/El_MVA_",
			    tag+etabin+"/El_MVA_",
                            tag+etabin+iel.CloseJet_Flavour()+"/El_MVA_",
			    tag+etabin+ptbin+"/El_MVA_",
                            tag+etabin+ptbin+iel.CloseJet_Flavour()+"/El_MVA_",
		    
    };

    for(auto i  : Tags){

      if(i.Contains("Pileup")) continue;
      if(i.Contains("IsEWtau")) continue;
      if(i.Contains("IsCF") && !i.Contains("IsPrompt")) continue;
      if(i.Contains("LF") && !i.Contains("IsFake")) continue;
      if(i.Contains("HF") && !i.Contains("IsFake")) continue;

      map<TString, double> mapBDT = iel.MAPBDT();

      for(auto imap : mapBDT ) FillHist(channel_string+i+"_"+imap.first+"_"+channel_string, imap.second , weight, 200, -1., 1);
      
      
      if(iel.HNL_MVA_Fake("v4_LF") > 0. && iel.HNL_MVA_Fake("v4_HF") > 0. && iel.HNL_MVA_Conv("v2") > -0.7 && iel.HNL_MVA_CF("v2") > 0.5 )   FillElectronKinematicPlots("electron_"+i, "PassMVA", iel, weight);
      
      
    } 
  }
  
  return;
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
  //bool PassBJetMVeto = (BJetColl.size()==0);
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
  
  Fill_RegionPlots(channel_ID, 0, param.Name,"SSInclusive" ,  VBFJetColl, FatjetColl,BJetColl, LepsT, LepsV, METv, nPV, _weight);


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
  int W2_ind(0); // Index of W2 : i.e W from N decay                                                                                                                                                     
  
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
  int W2_ind(0); // Index of W2 : i.e W from N decay 
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
  //double Nphi1 =  N.Phi();
  //double Nphi2 =  (LepFromN + j1+j2).Phi(); 
  
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
