#include "HNL_SignalEfficiency.h"

void HNL_SignalEfficiency::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_SignalEfficiency::executeEvent(){

  
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


  /// No Cut to leptons
  
  vector<Muon> _InputMuons = GetMuons    ( "NoCut", 10., 2.4);
  vector<Electron> _InputElectrons = GetElectrons    ( "NoCut", 10., 2.5);
  
  /// Require leptons to match 
  vector<Muon> InputMuons = GetSignalLeptons(_InputMuons, gens);
  vector<Electron> InputElectrons = GetSignalLeptons(_InputElectrons, gens);
  
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};//, EMu};
  

  vector<TString> ElectronIDs = {"NoCut","HNVeto2016","HNVeto","CutBasedLooseNoIso","CutBasedMediumNoIso","CutBasedTightNoIso","MVALooseNoIso","CutBasedVetoNoIso","HNTightV2","HNTight_17028","passPOGTight","passPOGMedium","passHEEPID","passMVAID_noIso_WP80","passMVAID_noIso_WP90","passMVAID_Iso_WP80","passMVAID_Iso_WP90","HNHEEPID","SUSYTight","HN2016MVA","HN2016MVA2","HN2016MVA2CC","HN2016POG","HNOpt","TriggerTight","TriggerLoose","TightTriggerTight","TightTriggerLoose","HNIP","HNIP2","HNIP3"};
  
  vector<TString> MuonIDs = {"NoCut","POGTight","POGHighPt","POGMedium","POGLoose","POGTightWithTightIso","POGHighPtWithLooseTrkIso","POGHighPtTight","POGHighPtMixTight","HNVeto2016","HNLoosest","HNLoose_17028","HNTight_17028","HNLooseMVA","HNLooseV1","HNLoosePOG","HNTightMVAVL","HNTightMVAM","HNTightMVAVVT","HNTightV1","HNTightV2","HNTightPFIsoLoose", "HNTightPFIsoMedium","HNTightPFIsoTight","HNTightPFIsoVeryTight","HNTightPFIsoVeryVeryTight","HNOpt"};  
  
  vector<pair<HNL_LeptonCore::Channel , TString > > LeptonIDMap;
  for(auto el_id : ElectronIDs) LeptonIDMap.push_back(make_pair( EE,  el_id));
  for(auto mu_id : MuonIDs)     LeptonIDMap.push_back(make_pair( MuMu,  mu_id));

  
  for(auto dilep_channel : channels){
    
    if(MCSample.Contains("Type")){
      if (!SelectChannel(dilep_channel)) continue;
    }
    
    TString channel = GetChannelString(dilep_channel) ;
    
    double ptbins[10] = { 0., 10.,15., 20., 30., 40.,50., 100.,500. ,1000.};
    
    
    for(auto imap : LeptonIDMap){
      vector<Electron> vecSelectedElectrons;
      vector<Muon> vecSelectedMuons;
      
      if(imap.first == MuMu) vecSelectedMuons = SelectMuons (InputMuons, imap.second, 10., 2.4);
      if(imap.first == EE)   vecSelectedElectrons =SelectElectrons    ( InputElectrons ,  imap.second, 10., 2.5);
      
      
      std::vector<Lepton *> leps       = MakeLeptonPointerVector(vecSelectedMuons,vecSelectedElectrons ,param);
      
      for(auto ilep:  leps)  {
	double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
	
	TString lepton_label=ilep->GetFlavour();
	TString eta_label   = ilep->GetEtaRegion();
	
	FillHist( "SignalReco"+channel+"/"+lepton_label+"_pt_"+eta_label+"_"+imap.second, pt, weight, 9, ptbins);
	FillHist( "SignalReco"+channel+"/"+lepton_label+"_pt_eta_"+imap.second , ilep->Pt() , ilep->Eta(), weight, 200, 0., 1000., 25, -2.5, 2.5);

      }
      
    }
  }
}



      
//      std::vector<Muon> MuonCollB = SkimLepColl(MuonColl, "MBMO");
//      std::vector<Muon> MuonCollE = SkimLepColl(MuonColl, "ME");
//      std::vector<Muon> MuonCollFakeB = SkimLepColl(MuonCollFake, "MBMO");
//      std::vector<Muon> MuonCollFakeE = SkimLepColl(MuonCollFake, "ME");
//      std::vector<Muon> MuonCollConvB = SkimLepColl(MuonCollConv, "MBMO");
//      std::vector<Muon> MuonCollConvE = SkimLepColl(MuonCollConv, "ME");


  


HNL_SignalEfficiency::HNL_SignalEfficiency(){


}
 
HNL_SignalEfficiency::~HNL_SignalEfficiency(){

}

