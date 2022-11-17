#include "HNL_ConvStudy.h"

void HNL_ConvStudy::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_ConvStudy::executeEvent(){


 
  Event ev = GetEvent();
  if(!IsData)  gens = GetGens();

  AnalyzerParameter param = HNL_LeptonCore::InitialiseHNLParameter("HNL","_UL");
  double weight =SetupWeight(ev,param);

  bool runDebug=false;
  if(runDebug){
    cout << "------------------------------" << endl;
    cout << "New Event " << endl;
  }

  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);

    if(gen.PID() == 22){
      if(runDebug)cout << gen.isPromptFinalState() << " " << gen.Status() << "  pt = " << gen.Pt() << " " << gen.Eta() << " " << gen.Phi() << endl;
    }
    if(fabs(gen.PID()) == 11 || fabs(gen.PID()) ==13){
      if(runDebug)cout << " Lep " << gen.PID() << " pt = " << gen.Pt() << " " << gen.Eta() << " " << gen.Phi() << endl; 
    }
  }
  // HL ID                                                                                                                                                   
  std::vector<Electron>   ElectronCollV = GetElectrons("NoCut", 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    ("NoCut", 10., 2.4);
  
  

  for(auto iel : ElectronCollV) {
    if(runDebug)cout << "Electron " << " " << iel.Pt() << " eta = " << iel.Eta() << " Type = " << GetLeptonType_JH(iel, gens) << endl;
  }
  
  for(auto iel : MuonCollV) {
    if(runDebug)cout << "Muon " << " " << iel.Pt() << " eta = " << iel.Eta() << " Type = " << GetLeptonType_JH(iel, gens) << endl;
  }
  
  
  TString PlotDir="Inclusive";
  

  std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

  for(unsigned int ilep=0; ilep < LepsV.size(); ilep++){

    TString SFlav = TString::Itoa(fabs(GetLeptonType_JH(*LepsV[ilep], gens)), 10);

    if(GetLeptonType_JH(*LepsV[ilep], gens) < 0) SFlav = "Minus_"+SFlav;
    else SFlav = "Plus_"+SFlav;

    double PTthreshold=5.,dPtRelmax=0.5;//2)                                                                                           
    float dRmax=0.2;//3)                                                                                                               
    float dRmin=999.;
    int NearPhotonIdx=-1;
    
    for(unsigned int i=2; i<gens.size(); i++){
      if( gens.at(i).MotherIndex()<0   ) continue;
      if( !(gens.at(i).PID()==22 && (gens.at(i).Status()==1 || gens.at(i).Status()==23)) ) continue;
      if( gens.at(i).Pt()<PTthreshold  ) continue;
      if( !(LepsV[ilep]->Pt()/gens.at(i).Pt()>(1.-dPtRelmax) && LepsV[ilep]->Pt()/gens.at(i).Pt()<(1.+dPtRelmax)) ) continue;
      if( LepsV[ilep]->DeltaR(gens.at(i))>dRmax ) continue;
      if( gens.at(i).Status()==23 && !IsFinalPhotonSt23(gens) ) continue;//4)                                                          
      if( LepsV[ilep]->DeltaR(gens.at(i))<dRmin ){ dRmin=LepsV[ilep]->DeltaR(gens.at(i)); NearPhotonIdx=i; }
    }
    
    cout << "----------------" <<endl;
    cout << " Near " <<  SFlav << endl;
    
    if(NearPhotonIdx>0) {
      double phPt= gens[NearPhotonIdx].Pt();
 
      FillHist( (PlotDir + SFlav+"/" + LepsV[ilep]->GetFlavour()+"_matched_photon_pt").Data(),phPt, weight, 1000, 0, 1000);
      
      cout << " Near " <<  SFlav << " " << phPt << " lep pt =" <<  LepsV[ilep]->Pt() << endl;
    }

    
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      double phPt=gen.Pt();

      if(LepsV[ilep]->DeltaR(gen) < 0.2) {
	if(gen.PID() == 22 && gen.isPromptFinalState() ) {
	  FillHist( (PlotDir + SFlav+"/" + LepsV[ilep]->GetFlavour()+"_matched2_photon_pt").Data(),phPt, weight, 1000, 0, 1000);
	}
      }
    }
  }
	  

  return;

  TString el_ID = param.Electron_Tight_ID ;
  TString mu_ID = param.Muon_Tight_ID ;

  double muon_pt =  10.;
  double electron_pt =  10.;

  // 3 Methods to run MC
  // 1) Fakes/ CF are done by data and Conv done my MC
  // - in this case if MC should remove Fake lep and CF leps
  // - this is done by Adding option 
  std::vector<Muon>       MuonCollT     = SkimLepColl    ( GetMuons    ( param,mu_ID, muon_pt,     2.4, RunFake)  ,gens,param,"NHConv");
  std::vector<Electron>   ElectronCollT = SkimLepColl    ( GetElectrons( param,el_ID, electron_pt, 2.5, RunFake)  ,gens,param,"NHConv");

  
  // Creat Lepton vector to have lepton blind codes                                                                                                          
  std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(MuonCollT,ElectronCollT);
  //std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param); // reyturns MET with systematic correction                                              

  std::map< TString, bool > map_Region_to_Bool;
  map_Region_to_Bool.clear();
  map_Region_to_Bool["Inclusive"] = true;
  map_Region_to_Bool["ConversionVeto"] = ConversionVeto(LepsT,gens);
    
  
  for(std::map< TString, bool >::iterator it = map_Region_to_Bool.begin(); it != map_Region_to_Bool.end(); it++){
    
    if(it->second){
    
      PlotDir=it->first;
      
      double ptbins[8] = { 0., 10.,15.,17., 20.,30., 40., 100.};
      
      for(unsigned int ilep=0; ilep < LepsT.size(); ilep++){
	
	double lpt = (LepsT.at(ilep)->Pt() > 100.) ? 99.: LepsT.at(ilep)->Pt();
	TString SFlav = TString::Itoa(fabs(GetLeptonType_JH(*LepsT[ilep], gens)), 10);
	if(GetLeptonType_JH(*LepsT[ilep], gens) < 0) SFlav = "Minus_"+SFlav;
	else SFlav = "Plus_"+SFlav;

	TString il = TString::Itoa(ilep, 10);
	
	FillHist( (PlotDir + "/" + LepsT.at(ilep)->GetFlavour()+"_lep"+il+"_pt").Data(),lpt, weight, 7, ptbins);
	FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_lep"+il+"_pt").Data(),lpt, weight, 7, ptbins);
	
	double PTthreshold=5.,dPtRelmax=0.5;//2)
	float dRmax=0.2;//3)
	float dRmin=999.;
	int NearPhotonIdx=-1;
	
	for(unsigned int i=2; i<gens.size(); i++){
	  if( gens.at(i).MotherIndex()<0   ) continue;
	  if( !(gens.at(i).PID()==22 && (gens.at(i).Status()==1 || gens.at(i).Status()==23)) ) continue;
	  if( gens.at(i).Pt()<PTthreshold  ) continue;
	  if( !(LepsT[ilep]->Pt()/gens.at(i).Pt()>(1.-dPtRelmax) && LepsT[ilep]->Pt()/gens.at(i).Pt()<(1.+dPtRelmax)) ) continue;
	  if( LepsT[ilep]->DeltaR(gens.at(i))>dRmax ) continue;
	  if( gens.at(i).Status()==23 && !IsFinalPhotonSt23(gens) ) continue;//4)
	  if( LepsT[ilep]->DeltaR(gens.at(i))<dRmin ){ dRmin=LepsT[ilep]->DeltaR(gens.at(i)); NearPhotonIdx=i; }
	}
	
	if(NearPhotonIdx>0) {
	  double phPt= gens[NearPhotonIdx].Pt() > 100. ? 99. : gens[NearPhotonIdx].Pt();

	  FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_matched_photon_pt").Data(),phPt, weight, 7, ptbins);
	  FillHist( (PlotDir + "/" + LepsT[ilep]->GetFlavour()+"_matched_photon_pt").Data(),phPt, weight, 7, ptbins);
	}
	
	for(unsigned int i=2; i<gens.size(); i++){
	  Gen gen = gens.at(i);
	  if(LepsT[ilep]->DeltaR(gen) < 0.2) {
	    if(gen.PID() == 22 && gen.isPromptFinalState() ) {
	      double phPt= gen.Pt() > 100. ? 99. : gens[NearPhotonIdx].Pt();
	      FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_matched2_photon_pt").Data(),phPt, weight, 7, ptbins);
	      FillHist( (PlotDir + "/" + LepsT[ilep]->GetFlavour()+"_matched2_photon_pt").Data(),phPt, weight, 7, ptbins);
	      
	    }
	  }
	}
	
	Gen MatchedGen = GetGenMatchedLepton(*LepsT[ilep], gens);
	if(MatchedGen.MotherIndex() > 0){
	  TString MotherID = TString::Itoa(gens[MatchedGen.MotherIndex()].PID(), 10);
	  double mpt = (gens[MatchedGen.MotherIndex()].Pt() > 100) ? 99. : gens[MatchedGen.MotherIndex()].Pt(); 
	  
	  FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_mother_pid"+MotherID+"_pt").Data(), mpt , weight, 7, ptbins);
	}
      }
    }
  }
  
}




HNL_ConvStudy::HNL_ConvStudy(){


}
 
HNL_ConvStudy::~HNL_ConvStudy(){

}
