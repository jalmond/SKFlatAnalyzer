#include "HNL_ConversionStudy.h"

void HNL_ConversionStudy::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_ConversionStudy::executeEvent(){


 
  Event ev = GetEvent();

  AnalyzerParameter param = HNL_LeptonCore::InitialiseHNLParameter("HNL","_UL");
  double weight =SetupWeight(ev,param);

  vector<TString> MCMergeList = {"DY","WG"};
  double weightSplit  = weight * MergeMultiMC(MCMergeList,"Split");
  double weightCombine  = weight * MergeMultiMC(MCMergeList,"Combine");
  if(weightSplit != weight)cout << "weight = " << weight << "  weightSplit = " << weightSplit << " weightCombine = " << weightCombine << endl;
  
  
  if(MergeMultiMC(MCMergeList,"Split")  == 0){

    // HL ID                                                                                                                                                                                                                                              
    std::vector<Electron>   ElectronCollV = GetElectrons("NoCut", 10., 2.5);
    std::vector<Muon>       MuonCollV     = GetMuons    ("NoCut", 10., 2.4);


    if(1){

      for(unsigned int i=2; i<All_Gens.size(); i++){
	Gen gen = All_Gens.at(i);

	if(gen.PID() == 22){
	  cout << gen.isPromptFinalState() << " " << gen.Status() << "  pt = " << gen.Pt() << " " << gen.Eta() << " " << gen.Phi() << endl;
	}
	if(fabs(gen.PID()) == 11 || fabs(gen.PID()) ==13){
	  cout << " Lep " << gen.PID() << " pt = " << gen.Pt() << " " << gen.Eta() << " " << gen.Phi() << endl;
	}
      }

      for(auto iel : ElectronCollV) {
	cout << "Electron " << " " << iel.Pt() << " eta = " << iel.Eta()  << endl;
      }

      for(auto iel : MuonCollV) {
	cout << "Muon " << " " << iel.Pt() << " eta = " << iel.Eta() << endl;
      }
    }



  }

  return;

  bool runDebug=false;
  if(runDebug){
    cout << "------------------------------" << endl;
    cout << "New Event " << endl;
    
    
    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);
      
      if(gen.PID() == 22){
	if(runDebug)cout << gen.isPromptFinalState() << " " << gen.Status() << "  pt = " << gen.Pt() << " " << gen.Eta() << " " << gen.Phi() << endl;
      }
      if(fabs(gen.PID()) == 11 || fabs(gen.PID()) ==13){
	if(runDebug)cout << " Lep " << gen.PID() << " pt = " << gen.Pt() << " " << gen.Eta() << " " << gen.Phi() << endl; 
      }
    }
  }
  // HL ID                                                                                                                                                   
  std::vector<Electron>   ElectronCollV = GetElectrons("NoCut", 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    ("NoCut", 10., 2.4);
  
  
  if(runDebug){
    
    for(auto iel : ElectronCollV) {
      if(runDebug)cout << "Electron " << " " << iel.Pt() << " eta = " << iel.Eta() << " Type = " << iel.LeptonGenType() << endl;
    }
    
    for(auto iel : MuonCollV) {
      if(runDebug)cout << "Muon " << " " << iel.Pt() << " eta = " << iel.Eta() << " Type = " <<  iel.LeptonGenType() << endl;
    }
  }
  
  TString PlotDir="Inclusive";
  

  std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

  for(unsigned int ilep=0; ilep < LepsV.size(); ilep++){

    TString SFlav = TString::Itoa(fabs(LepsV[ilep]->LeptonGenType()), 10);
    
    if(LepsV[ilep]->LeptonGenType() < 0) SFlav = "Minus_"+SFlav;
    else SFlav = "Plus_"+SFlav;

    double PTthreshold=5.,dPtRelmax=0.5;//2)                                                                                           
    float dRmax=0.2;//3)                                                                                                               
    float dRmin=999.;
    int NearPhotonIdx=-1;
    int NearPhoton23Idx=-1;
    
    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens[i];
      if( All_Gens.at(i).MotherIndex()<0   ) continue;
      //if( LepsV[ilep]->DeltaR(All_Gens.at(i)) < 0.2) {
      if(All_Gens.at(i).PID()==22){
	cout << event  << "\t" << gen.PID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() << "\t" << All_Gens.at(gen.MotherIndex()).PID()<< "  " <<  LepsV[ilep]->DeltaR(All_Gens.at(i)) <<  "\t";
	printf("%.2f\t%.2f\t%.2f\t%.2f\n",gen.Pt(), gen.Eta(), gen.Phi(), gen.M());
	
      }
      if( !(All_Gens.at(i).PID()==22 && (All_Gens.at(i).Status()==1 || All_Gens.at(i).Status()==23)) ) continue;
      if( All_Gens.at(i).Status()==23) NearPhoton23Idx=i;
      if( All_Gens.at(i).Pt()<PTthreshold  ) continue;
      if( !(LepsV[ilep]->Pt()/All_Gens.at(i).Pt()>(1.-dPtRelmax) && LepsV[ilep]->Pt()/All_Gens.at(i).Pt()<(1.+dPtRelmax)) ) continue;
      if( LepsV[ilep]->DeltaR(All_Gens.at(i))>dRmax ) continue;
      
      if( All_Gens.at(i).Status()==23 && !IsFinalPhotonSt23(All_Gens) ) continue;//4)                                                          
      if( LepsV[ilep]->DeltaR(All_Gens.at(i))<dRmin ){ dRmin=LepsV[ilep]->DeltaR(All_Gens.at(i)); NearPhotonIdx=i;}
    }
   
    
    if(NearPhotonIdx>0) {
      double phPt= All_Gens[NearPhotonIdx].Pt();
      double phPtGen= All_Gens[NearPhoton23Idx].Pt();
      
      FillHist( (PlotDir + SFlav+"/" + LepsV[ilep]->GetFlavour()+"_matched_FSphoton_pt").Data(),phPt, weight, 1000, 0, 1000);
      FillHist( (PlotDir + SFlav+"/" + LepsV[ilep]->GetFlavour()+"_matched_photon_pt").Data(),phPtGen, weight, 1000, 0, 1000);
      cout << "phPtGen = " << phPtGen << endl;
    }
    return;
    
    for(unsigned int i=2; i<All_Gens.size(); i++){
      Gen gen = All_Gens.at(i);
      double phPt=gen.Pt();

      if(LepsV[ilep]->DeltaR(gen) < 0.2) {
	if(gen.PID() == 22 && gen.isPromptFinalState() ) {
	  FillHist( (PlotDir + SFlav+"/" + LepsV[ilep]->GetFlavour()+"_matched2_photon_pt").Data(),phPt, weight, 1000, 0, 1000);
	}
      }
    }
  }
	  

  TString el_ID = param.Electron_Tight_ID ;
  TString mu_ID = param.Muon_Tight_ID ;

  double muon_pt =  10.;
  double electron_pt =  10.;

  // 3 Methods to run MC
  // 1) Fakes/ CF are done by data and Conv done my MC
  // - in this case if MC should remove Fake lep and CF leps
  // - this is done by Adding option 
  std::vector<Muon>       MuonCollT     = SkimLepColl    ( GetMuons    ( param,mu_ID, muon_pt,     2.4, RunFake) ,param,"NHConv");
  std::vector<Electron>   ElectronCollT = SkimLepColl    ( GetElectrons( param,el_ID, electron_pt, 2.5, RunFake) ,param,"NHConv");

  
  // Creat Lepton vector to have lepton blind codes                                                                                                          
  std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(MuonCollT,ElectronCollT);
  //std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);
  
  Particle METv = GetvMET("PuppiT1xyULCorr"); // reyturns MET with systematic correction                                              

  std::map< TString, bool > map_Region_to_Bool;
  map_Region_to_Bool.clear();
  map_Region_to_Bool["Inclusive"] = true;
  map_Region_to_Bool["ConversionVeto"] = ConversionVeto(LepsT,All_Gens);
    
  
  for(std::map< TString, bool >::iterator it = map_Region_to_Bool.begin(); it != map_Region_to_Bool.end(); it++){
    
    if(it->second){
    
      PlotDir=it->first;
      
      double ptbins[8] = { 0., 10.,15.,17., 20.,30., 40., 100.};
      
      for(unsigned int ilep=0; ilep < LepsT.size(); ilep++){
	
	double lpt = (LepsT.at(ilep)->Pt() > 100.) ? 99.: LepsT.at(ilep)->Pt();
	TString SFlav = TString::Itoa(fabs(LepsT[ilep]->LeptonGenType()), 10);
	if(LepsT[ilep]->LeptonGenType() < 0) SFlav = "Minus_"+SFlav;
	else SFlav = "Plus_"+SFlav;

	TString il = TString::Itoa(ilep, 10);
	
	FillHist( (PlotDir + "/" + LepsT.at(ilep)->GetFlavour()+"_lep"+il+"_pt").Data(),lpt, weight, 7, ptbins);
	FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_lep"+il+"_pt").Data(),lpt, weight, 7, ptbins);
	
	double PTthreshold=5.,dPtRelmax=0.5;//2)
	float dRmax=0.2;//3)
	float dRmin=999.;
	int NearPhotonIdx=-1;
	
	for(unsigned int i=2; i<All_Gens.size(); i++){
	  if( All_Gens.at(i).MotherIndex()<0   ) continue;
	  if( !(All_Gens.at(i).PID()==22 && (All_Gens.at(i).Status()==1 || All_Gens.at(i).Status()==23)) ) continue;
	  if( All_Gens.at(i).Pt()<PTthreshold  ) continue;
	  if( !(LepsT[ilep]->Pt()/All_Gens.at(i).Pt()>(1.-dPtRelmax) && LepsT[ilep]->Pt()/All_Gens.at(i).Pt()<(1.+dPtRelmax)) ) continue;
	  if( LepsT[ilep]->DeltaR(All_Gens.at(i))>dRmax ) continue;
	  if( All_Gens.at(i).Status()==23 && !IsFinalPhotonSt23(All_Gens) ) continue;//4)
	  if( LepsT[ilep]->DeltaR(All_Gens.at(i))<dRmin ){ dRmin=LepsT[ilep]->DeltaR(All_Gens.at(i)); NearPhotonIdx=i; }
	}
	
	if(NearPhotonIdx>0) {
	  double phPt= All_Gens[NearPhotonIdx].Pt() > 100. ? 99. : All_Gens[NearPhotonIdx].Pt();

	  FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_matched_photon_pt").Data(),phPt, weight, 7, ptbins);
	  FillHist( (PlotDir + "/" + LepsT[ilep]->GetFlavour()+"_matched_photon_pt").Data(),phPt, weight, 7, ptbins);
	}
	
	for(unsigned int i=2; i<All_Gens.size(); i++){
	  Gen gen = All_Gens.at(i);
	  if(LepsT[ilep]->DeltaR(gen) < 0.2) {
	    if(gen.PID() == 22 && gen.isPromptFinalState() ) {
	      double phPt= gen.Pt() > 100. ? 99. : All_Gens[NearPhotonIdx].Pt();
	      FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_matched2_photon_pt").Data(),phPt, weight, 7, ptbins);
	      FillHist( (PlotDir + "/" + LepsT[ilep]->GetFlavour()+"_matched2_photon_pt").Data(),phPt, weight, 7, ptbins);
	      
	    }
	  }
	}
	
	Gen MatchedGen = GetGenMatchedLepton(*LepsT[ilep], All_Gens);
	if(MatchedGen.MotherIndex() > 0){
	  TString MotherID = TString::Itoa(All_Gens[MatchedGen.MotherIndex()].PID(), 10);
	  double mpt = (All_Gens[MatchedGen.MotherIndex()].Pt() > 100) ? 99. : All_Gens[MatchedGen.MotherIndex()].Pt(); 
	  
	  FillHist( (PlotDir + SFlav+"/" + LepsT[ilep]->GetFlavour()+"_mother_pid"+MotherID+"_pt").Data(), mpt , weight, 7, ptbins);
	}
      }
    }
  }
  
}




HNL_ConversionStudy::HNL_ConversionStudy(){


}
 
HNL_ConversionStudy::~HNL_ConversionStudy(){

}
