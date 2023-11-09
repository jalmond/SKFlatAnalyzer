#include "HNL_SignalEfficiency.h"

void HNL_SignalEfficiency::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_SignalEfficiency::executeEvent(){

  
  //==== Gen for genmatching
  AnalyzerParameter param  = InitialiseHNLParameter("SignalStudy");
  Event ev = GetEvent();
  double weight =  (gen_weight > 0) ? 1. : -1. ; //SetupWeight(ev,param);
  
  FillHist ("NoCut", 1, weight, 2, 0., 2.,"");

  TString process="";
  if(!IsData){
    
    process = GetProcess();

    vector<TString> labels ={"Inclusive","OS_ElEl", "SS_El-El-", "SS_El+El+","OS_MuMu", "SS_Mu-Mu-" , "SS_Mu+Mu+","OS_ElMu","SS_El-Mu-" ,"SS_El+Mu+", "OS_MuEl","SS_Mu-El-","SS_Mu+El+"};

    HNL_LeptonCore::FillCutflow("SignalProcess","SplitChannel",weight, labels, process);
    HNL_LeptonCore::FillCutflow("SignalProcess","SplitChannel",weight, labels, "Inclusive");
    

    if(_jentry% 10000==0){
      cout << "process = " << process << endl;
      
      PrintGen(All_Gens);
    }

  }
  if(process.Contains("OS")) return;


  /// No Cut to leptons
  
  vector<Muon> _InputMuons = GetMuons    ( "NoCut", 10., 2.4);

  vector<Muon> InputMuons = GetSignalLeptons(_InputMuons, All_Gens);


  vector<Electron> _InputElectrons = GetElectrons    ( "NoCut", 10., 2.5);
  
  /// Require leptons to match 

  vector<Electron> InputElectrons = GetSignalLeptons(_InputElectrons, All_Gens);
  
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};//, EMu};
  

  vector<TString> ElectronIDs = {"NoCut","HNVetoMVA","HNVeto","CutBasedLooseNoIso","CutBasedMediumNoIso","CutBasedTightNoIso","MVALooseNoIso","CutBasedVetoNoIso","HNTightV2","HNTight_17028","passPOGTight","passPOGMedium","passHEEPID","passMVAID_noIso_WP80","passMVAID_noIso_WP90","passMVAID_Iso_WP80","passMVAID_Iso_WP90","HNHEEPID","SUSYTight","HN2016MVA","HN2016MVA2","HN2016MVA2CC","HN2016POG","TriggerTight","TriggerLoose","TightTriggerTight","TightTriggerLoose","HNL_ULID_2016","HNL_ULID_2017","HNL_ULID_2018","HNL_ULID_2016_Trigger","HNL_ULID_2016_CF","HNL_ULID_2016_Fake","HNL_ULID_2016_Conv","MVALoose"};
  
  vector<TString> MuonIDs = {"NoCut","POGTight","POGHighPt","POGMedium","POGLoose","POGTightWithTightIso","POGHighPtWithLooseTrkIso","POGHighPtTight","POGHighPtMixTight","HNLoosest","HNLoose_17028","HNTight_17028","HNLooseMVA","HNLooseV1","HNLoosePOG","HNTightV1","HNTightV2","HNTightPFIsoLoose", "HNTightPFIsoMedium","HNTightPFIsoTight","HNTightPFIsoVeryTight","HNTightPFIsoVeryVeryTight","HNL_TopMVA_MM","HNL_TopMVA_TT","HNL_TopMVA_TM","HNL_TopMVA_MT","HNL_Peking","HNL_ULID_2016","HNL_ULID_2018","HNL_ULID_2017","MVALoose","MVALooseCut1","MVALooseCut2","MVALooseCut3","MVALooseCut4"};  
  
  vector<pair<HNL_LeptonCore::Channel , TString > > LeptonIDMap;
  for(auto el_id : ElectronIDs) LeptonIDMap.push_back(make_pair( EE,  el_id));
  for(auto mu_id : MuonIDs)     LeptonIDMap.push_back(make_pair( MuMu,  mu_id));

  
  for(auto dilep_channel : channels){
    
    if(MCSample.Contains("Type")){
      if (!SelectChannel(dilep_channel)) continue;
    }

    TString channel = GetChannelString(dilep_channel) ;
    FillHist( "NoCut_"+ channel, 1., weight, 2,0., 2);
    
    double ptbins[11] = { 0., 10.,15., 20., 30., 40.,50., 100.,500. ,1000.,2000.};
    
    
    for(auto imap : LeptonIDMap){

      if(imap.first != dilep_channel) continue;

      vector<Electron> vecSelectedElectrons;
      vector<Muon> vecSelectedMuons;
      
      if(imap.first == MuMu) vecSelectedMuons = SelectMuons (InputMuons, imap.second, 10., 2.4);
      if(imap.first == EE)   vecSelectedElectrons =SelectElectrons    ( InputElectrons ,  imap.second, 10., 2.5);
      
      std::vector<Lepton *> leps       = MakeLeptonPointerVector(vecSelectedMuons,vecSelectedElectrons ,param);
      
      for(auto ilep:  leps)  {
	double pt = (ilep->Pt() > 2000) ? 1999 : ilep->Pt();
	
	TString lepton_label=ilep->GetFlavour();
	TString eta_label   = ilep->GetEtaRegion();
	
	if(MCSample.Contains("Type")){
	  
	  FillHist( "SignalReco"+channel+"/"+lepton_label+"_ptbinned_"+eta_label+"_"+imap.second, pt, weight, 10, ptbins);
	  FillHist( "SignalReco"+channel+"/"+lepton_label+"_ptbinned_"+imap.second, pt, weight, 10, ptbins);
	  FillHist( "SignalReco"+channel+"/"+lepton_label+"_pt_"+eta_label+"_"+imap.second, pt, weight, 400, 0., 2000.);
          FillHist( "SignalReco"+channel+"/"+lepton_label+"_pt_"+imap.second, pt, weight,  400, 0., 2000.);
	}
      }
      
      if(MCSample.Contains("Type")) continue;
      
      if(imap.first == EE)  {
	for(auto iel : vecSelectedElectrons){

	  double pt = (iel.Pt() > 2000) ? 1999 : iel.Pt();

	  if (iel.IsPrompt()) {
	    FillHist( "Prompt_"+channel+"/Electron_pt_"+iel.GetEtaRegion()+"_"+imap.second, pt, weight, 10, ptbins);
            FillHist( "Prompt_"+channel+"/Electron_pt_"+imap.second, pt, weight,  400, 0., 2000.);
	    
	  }
	  if (iel.IsFake()) {
	    FillHist( "Fake_"+channel+"/Electron_pt_"+iel.GetEtaRegion()+"_"+imap.second, pt, weight, 10, ptbins);
	    FillHist( "Fake_"+channel+"/Electron_pt_"+imap.second, pt, weight,  400, 0., 2000.);
	  }
	  if (iel.IsConv()) {
	    FillHist( "Conv_"+channel+"/Electron_pt_"+iel.GetEtaRegion()+"_"+imap.second, pt, weight, 10, ptbins);
	    FillHist( "Conv_"+channel+"/Electron_pt_"+imap.second, pt,  weight,  400, 0., 2000.);
	  }
	  if(iel.LeptonIsCF()){
	    FillHist( "CF_"+channel+"/Electron_pt_"+iel.GetEtaRegion()+"_"+imap.second, pt, weight, 10, ptbins);
            FillHist( "CF_"+channel+"/Electron_pt_"+imap.second, pt,  weight,  400, 0., 2000.);
	  }
	}
      }
      else{

        for(auto imu : vecSelectedMuons){
	  
	  double pt = (imu.Pt() > 2000) ? 1999 : imu.Pt();
	  
	  if (imu.IsPrompt()) {
	    FillHist( "Prompt_"+channel+"/Muon_pt_"+imu.GetEtaRegion()+"_"+imap.second, pt, weight, 10, ptbins);
	    FillHist( "Prompt_"+channel+"/Muon_pt_"+imap.second, pt, weight,  400, 0., 2000.);
	  }
	  if (imu.IsFake()) {
	    FillHist( "Fake_"+channel+"/Muon_pt_"+imu.GetEtaRegion()+"_"+imap.second, pt, weight, 10, ptbins);
	    FillHist( "Fake_"+channel+"/Muon_pt_"+imap.second, pt, weight,  400, 0., 2000.);
	  }
	  if (imu.IsConv()) {
	    FillHist( "Conv_"+channel+"/Muon_pt_"+imu.GetEtaRegion()+"_"+imap.second, pt, weight, 10, ptbins);
	    FillHist( "Conv_"+channel+"/Muon_pt_"+imap.second, pt,  weight,  400, 0., 2000.);
	  }
	}
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

