#include "HNL_LeptonCore.h"



double HNL_LeptonCore::GetPtPartonSF(Lepton  Lep, TString LooseID){
  
  if(LooseID == "HNL_TopMVA_FO_MM") return 0.661;

  bool IsMuon=(Lep.LeptonFlavour() == Lepton::MUON);
  
  if(DataYear == 2016){

    if(IsMuon){
      if(LooseID == "HNL_ULID_FO")      return 0.741;
      if(LooseID == "HNL_ULID_POGM_FO") return 0.741;
      if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.670;
      if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.682;
      if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.741;
      if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.747;
      if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.737;
      if(LooseID == "HNL_ULID_FOv7_"+GetYearString()) return 0.737; 
    }
    else{
      if(Lep.IsBB()){
	//https://jalmond.web.cern.ch/jalmond/SNU/WebPlots/HNL/HNL_LeptonFakeRates/HNL_LeptonFakeRates/2016/HNL_GetProfileSFElNov/?match=BB
	if(LooseID == "HNL_ULID_FO_"+GetYearString())   return 0.7661;
	if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.6412;
	if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.6496;
	if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.7650;
	if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.8176;
	if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.7477;
      }    
      else{
	if(LooseID == "HNL_ULID_FO_"+GetYearString())   return 0.8824;
	if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.7684;
	if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.7706;
	if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.8824;
	if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.901;
	if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.644;
      }
    }
  }
  if(DataYear == 2017){

    if(IsMuon){
      if(!IsData){
	if(Lep.IsBB()){
	  double SFPt = 1.;
	  if(LooseID == "HNL_ULID_FO") return 0.802;
	  if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.641;
	  if(LooseID == "HNL_ULID_FOv7_"+GetYearString()) return 0.806;
	}
	else{
	  double SFPt = 1.;
	  if(LooseID == "HNL_ULID_FO") return 0.777;
          if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.616;
	  if(LooseID == "HNL_ULID_FOv7_"+GetYearString()) return 0.779;

	}
      }
      
      if(LooseID == "HNL_ULID_FO") return 0.732;
      if(LooseID == "HNL_ULID_POGM_FO") return 0.728;
      if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.683;
      if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.680;
      if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.732;
      if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.740;
      if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.730;
      if(LooseID == "HNL_ULID_FOv7_"+GetYearString()) return 0.730; /// Update
    }
    else{
      if(Lep.IsBB()){
	if(LooseID == "HNL_ULID_FO_"+GetYearString())   return 0.814;
        if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.665;
        if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.663;
        if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.8136;
        if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.8470;
        if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.7824;
      }
      else{
	if(LooseID == "HNL_ULID_FO_"+GetYearString())   return 0.7658;
        if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.6320;
        if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.6294;
        if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.7658;
        if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.767;
        if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.548;

      }
    }
  }
  if(DataYear == 2018){
    if(IsMuon){
      if(LooseID == "HNL_ULID_FO") return 0.737;
      if(LooseID == "HNL_ULID_POGM_FO") return 0.735;
      if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.689;
      if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.687;
      if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.736;
      if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.744;
      if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.737;
      if(LooseID == "HNL_ULID_FOv7_"+GetYearString()) return 0.737;
    }
    else{
      if(Lep.IsBB()){
	if(LooseID == "HNL_ULID_FO_"+GetYearString())   return 0.820;
        if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.667;
        if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.668;
        if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.821;
        if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.823;
        if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.798;

      } 
      else{
        if(LooseID == "HNL_ULID_FO_"+GetYearString())   return 0.860;
        if(LooseID == "HNL_ULID_FOv2_"+GetYearString()) return 0.727;
        if(LooseID == "HNL_ULID_FOv3_"+GetYearString()) return 0.727;
        if(LooseID == "HNL_ULID_FOv4_"+GetYearString()) return 0.860;
        if(LooseID == "HNL_ULID_FOv5_"+GetYearString()) return 0.890;
        if(LooseID == "HNL_ULID_FOv6_"+GetYearString()) return 0.603;
	
      } 
    }
  }

  

  cout << "ERROR in GetPtPartonSF " << IsMuon << " " << LooseID << endl;
  exit(EXIT_FAILURE);

}

int HNL_LeptonCore::GetFakeLepSrcType(const Lepton& Lep, vector<Jet>& JetColl){
  //Type: -1: Unmatched, 1:L, 2:C, 3:B                                                                                                                                                                                                                                                                                                                                                                                                   
  int SrcType=-1;
  bool NearB=false, NearC=false, NearL=false;
  for(unsigned int ij=0; ij<JetColl.size(); ij++){
    if(Lep.DeltaR(JetColl.at(ij))<0.4){
      if     (JetColl.at(ij).hadronFlavour()==5){ NearB=true; break; }//1)                                                                                                                                                                                                                                                                                                                                                               
      else if(JetColl.at(ij).hadronFlavour()==4){ NearC=true; }
      else if(JetColl.at(ij).hadronFlavour()==0){ NearL=true; }
    }
  }

  if     (NearB) SrcType=3;
  else if(NearC) SrcType=2;
  else if(NearL) SrcType=1;

  return SrcType;
  //1) Higher Priority to B. if there's multiple near jets, then b-jet has higher priority                                                                                                                                                                                                                                                                                                                                              

}





/*                                   
				     FAKE RATE FUNCTIONS                                                                                                                                                                              
*/


double HNL_LeptonCore::GetFakeRateElectron(Electron el, AnalyzerParameter param){

  //// Access individual lepton Fake Rate                                                                                                   

  double  LepEta = el.fEta();
  double  LepPt  = el.PtMaxed(80);
  TString  fr_key = param.k.Electron_FR;
  
  return fakeEst->GetElectronFakeRate(param.Electron_Tight_ID, fr_key,param.FakeRateMethod, param.FakeRateParam, LepEta, LepPt, el.LeptonFakeTagger() );


}
double HNL_LeptonCore::GetFakeRateMuon(Muon mu, AnalyzerParameter param){

  double  LepEta = mu.fEta();
  double  LepPt  = mu.PtMaxed(80);
  TString  fr_key = param.k.Muon_FR;

  //// Access individual lepton Fake Ratex                                                                                                                                                                                                                                                                                                                                                                                               
  return fakeEst->GetMuonFakeRate(param.Muon_Tight_ID, fr_key,param.FakeRateMethod,param.FakeRateParam,LepEta, LepPt, mu.LeptonFakeTagger() );

}




double HNL_LeptonCore::GetFakeWeight(std::vector<Lepton *> leps, AnalyzerParameter _param){

  //// Access event weight based on Lepton collection                                                               
  if(!IsData) return 1.;

  //fakeEst->IgnoreNoHist=true;
  fakeEst->IgnoreNoHist=false; //JH

  ///// SINGLE LEPTON EVENTS 
  double this_weight = -1.;

  if(leps.size() == 1){
    TString fr_key = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_FR : _param.k.Muon_FR;
    TString pr_key = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_PR : _param.k.Muon_PR;
    bool IsMuon    = (leps[0]->LeptonFlavour() != Lepton::ELECTRON);
    TString ID     = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
   
    if(!leps[0]->LepIDSet()) {      cout << "Lepton ID not set" << endl;      exit(EXIT_FAILURE);    }

    double this_fr = fakeEst->GetFakeRate  (IsMuon, ID,  fr_key, _param.FakeRateMethod, _param.FakeRateParam, leps[0]->fEta(), leps[0]->Pt(),leps[0]->LeptonFakeTagger());
    double this_pr = fakeEst->GetPromptRate(_param.ApplyPR, IsMuon, ID, pr_key, leps[0]->fEta(), leps[0]->UncorrectedPt());

    this_weight=  fakeEst->CalculateLepWeight(this_pr, this_fr, leps[0]->PassLepID() );
    return this_weight;
  }

  ////// DILEPTON EVENTS 
  if (leps.size() == 2){

    if(!(leps[0]->LepIDSet() || leps[1]->LepIDSet())) {
      cout << "Lepton ID not set" << endl;
      exit(EXIT_FAILURE);
    }

    TString fr_key = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_FR : _param.k.Muon_FR;
    TString pr_key = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_PR : _param.k.Muon_PR;

    if(run_Debug){
      if(leps[0]->LeptonFlavour() == Lepton::ELECTRON) cout << "_param.Electron_Tight_ID = " << _param.Electron_Tight_ID <<  " fr_key = " << fr_key << endl;
      else cout << "_param.Muon_Tight_ID  = " <<  _param.Muon_Tight_ID <<  " fr_key = " << fr_key <<endl;
    }

    TString ID1   =  (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
    TString ID2   =  (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
    bool IsMuon1  = (leps[0]->LeptonFlavour() != Lepton::ELECTRON);
    bool IsMuon2  = (leps[1]->LeptonFlavour() != Lepton::ELECTRON);

    double this_fr1 =  fakeEst->GetFakeRate(IsMuon1, ID1,  fr_key, _param.FakeRateMethod, _param.FakeRateParam, leps[0]->fEta(), leps[0]->Pt(),leps[0]->LeptonFakeTagger());
    double this_fr2 =  fakeEst->GetFakeRate(IsMuon2, ID2,  fr_key, _param.FakeRateMethod, _param.FakeRateParam, leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger() );
    double this_pr1 =  fakeEst->GetPromptRate(_param.ApplyPR, IsMuon1, ID1, pr_key, leps[0]->fEta(), leps[0]->UncorrectedPt());
    double this_pr2 =  fakeEst->GetPromptRate(_param.ApplyPR, IsMuon2, ID2, pr_key, leps[1]->fEta(), leps[1]->UncorrectedPt());
    
    
    this_weight = fakeEst->CalculateDilepWeight(this_pr1,this_fr1, this_pr2, this_fr2, leps[0]->PassLepID(),leps[1]->PassLepID(),0);
    if(run_Debug) cout << ID1 << " " << ID2 << "fr_key1 = " << fr_key <<  " w= " << this_weight << " 2L"<<endl;                                                    
    return this_weight;
  }


  else{


    double this_weight = -1.;
    vector<double> FRs;

    double this_fr = -999.;
    for(auto lep : leps){

      if( lep->PassLepID() ) continue;

      TString fr_key = (lep->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_FR : _param.k.Muon_FR;
      TString ID     = (lep->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
      bool IsMuon    = (lep->LeptonFlavour() != Lepton::ELECTRON);
      
      this_fr = fakeEst->GetFakeRate(IsMuon, ID,  fr_key, _param.FakeRateMethod, _param.FakeRateParam, lep->fEta(), lep->Pt(),lep->LeptonFakeTagger());
      this_weight *= -1.*this_fr/(1.-this_fr);
      FRs.push_back(this_fr);

    }

    if(FRs.size()==0){
      return 0.;
    }
    else{
      //cout << this_weight<< " ML"<<endl;                                                                                                                                                                                                                                                                                                                                                                                               

      return this_weight;
    }
  }

  return -999.;
}





double HNL_LeptonCore::GetCFSF(AnalyzerParameter param, std::vector<Electron> electrons){

  double _SF=1.;

  for(auto i : electrons) {
    if(i.IsBB()) {
      if(GetYearString() == "2017"){
        if(param.Electron_Tight_ID == "POGTight") _SF*= 1.217 ;
        if(param.Electron_Tight_ID == "HNTightV2") _SF*=1.030 ;
        if(param.Electron_Tight_ID == "HNL_ULID_2017") _SF*= 1.119;
        if(param.Electron_Tight_ID == "TopHNSST") _SF*=1.162 ;
      }
      else{

      } 
    }
    else{
      if(GetYearString() == "2017"){
        if(param.Electron_Tight_ID == "POGTight") _SF*= 1.026;
        if(param.Electron_Tight_ID == "HNTightV2") _SF*= 1.060;
        if(param.Electron_Tight_ID == "HNL_ULID_2017") _SF*= 1.111 ;
        if(param.Electron_Tight_ID == "TopHNSST") _SF*= 1.052;
      }
      else{

      } 


    }
  }

  return _SF;

}


double HNL_LeptonCore::GetCFWeightElectron(std::vector<Lepton* > leps ,  AnalyzerParameter param, bool ApplySF){

  vector<double> el_pt,el_eta;
  for(auto ilep : leps){
    if(ilep->LeptonFlavour()==Lepton::ELECTRON){
      el_pt.push_back(ilep->Pt());
      el_eta.push_back(ilep->defEta());
    }
  }

  return GetCFWeightElectron(el_pt,el_eta, param, ApplySF);
}

double HNL_LeptonCore::GetCFWeightElectron(std::vector<Electron> electrons ,  AnalyzerParameter param,bool ApplySF){

  vector<double> el_pt, el_eta;
  for(auto ilep : electrons){
    el_pt.push_back(ilep.Pt());
    el_eta.push_back(ilep.scEta());
  }

  return GetCFWeightElectron(el_pt,el_eta, param, ApplySF);
}

double HNL_LeptonCore::GetCFWeightElectron(vector<double> el_pt, vector<double> el_eta, AnalyzerParameter param, bool ApplySF){

  //cfEst->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;                                                                                                                                                                                                                                                                                                                                                                               

  if(el_pt.size()  != 2) return 1.;


  double el1_cf_rate =   cfEst->GetElectronCFRate(param.Electron_Tight_ID, param.k.Electron_CF,el_eta[0], el_pt[0], 0);
  double el2_cf_rate =   cfEst->GetElectronCFRate(param.Electron_Tight_ID, param.k.Electron_CF,el_eta[1], el_pt[1], 0);

  if(ApplySF){
    el1_cf_rate *= 1.;
    el2_cf_rate *= 1.; /// No SF Yet                                                                                                                                                                                                                                                                                                                                                                                                     
  }

  double cf_weight = (el1_cf_rate / (1.-el1_cf_rate))  + (el2_cf_rate/(1.-el2_cf_rate));

  if(run_Debug) cout << "GetCFWeightElectron weight=" << cf_weight << endl;
  return cf_weight;
}




double HNL_LeptonCore::GetZMassShift(vector<Electron> Electrons) {

  if(Electrons.size() != 2) return 1;

  if(Electrons[0].Charge() == Electrons[1].Charge()) return 1;
  if(DataEra=="2017"){
    if(Electrons[0].IsBB() && Electrons[1].IsBB())  return 0.988;
    if(Electrons[0].IsEC() && Electrons[1].IsEC())  return 0.993;
    return (0.988+0.993)/2.;
  }
  return 1;

}
double HNL_LeptonCore::GetShiftCFEl(Electron el) {


  //// Get Shift for  Prompt -> CF Pt response                                                                                                                                                                   

  double PtShift = 1.;
                                                                                                                                                                                                               
  if(DataEra=="2016preVFP"){
    if(el.etaRegionString() == "EB1"){
      if(el.Pt() < 30)       PtShift = 0.994;
      else if(el.Pt() < 50)  PtShift = 0.996;
      else if(el.Pt() < 100) PtShift = 0.999;
      else                   PtShift = 1;
    }
    if(el.etaRegionString() == "EB2"){
      if(el.Pt() < 30)       PtShift = 0.986;
      else if(el.Pt() < 50)  PtShift = 0.992;
      else if(el.Pt() < 100) PtShift = 0.997;
      else if(el.Pt() < 250) PtShift = 0.999;
      else                   PtShift = 0.999;
    }
    if(el.etaRegionString() == "EE1"){
      if(el.Pt() < 30)       PtShift = 0.979;
      else if(el.Pt() < 50)  PtShift = 0.989;
      else if(el.Pt() < 75)  PtShift = 0.994;
      else if(el.Pt() < 100) PtShift = 0.996;
      else if(el.Pt() < 250) PtShift = 0.998;
      else                   PtShift = 0.999;
    }
    if(el.etaRegionString() == "EE2"){
      if(el.Pt() < 30)       PtShift = 0.986;
      else if(el.Pt() < 50)  PtShift = 0.993;
      else if(el.Pt() < 75)  PtShift = 0.996;
      else if(el.Pt() < 100) PtShift = 0.997;
      else if(el.Pt() < 250) PtShift = 0.998;
      else                   PtShift = 1.;
    }
  }

  if(DataEra=="2016postVFP"){
    if(el.etaRegionString() == "EB1"){
      if(el.Pt() < 30)       PtShift = 0.995;
      else if(el.Pt() < 50)  PtShift = 0.996;
      else if(el.Pt() < 75)  PtShift = 0.998;
      else if(el.Pt() < 100) PtShift = 0.998;
      else if(el.Pt() < 250) PtShift = 1;
      else                   PtShift = 1;
    }
    if(el.etaRegionString() == "EB2"){
      if(el.Pt() < 30)       PtShift = 0.987;
      else if(el.Pt() < 50)  PtShift = 0.991;
      else if(el.Pt() <  75) PtShift = 0.996;
      else if(el.Pt() < 100) PtShift = 0.997;
      else if(el.Pt() < 250) PtShift = 0.998;
      else                   PtShift = 1;
    }
    if(el.etaRegionString() == "EE1"){
      if(el.Pt() < 30)       PtShift = 0.98;
      else if(el.Pt() < 50)  PtShift = 0.988;
      else if(el.Pt() <  75) PtShift = 0.994;
      else if(el.Pt() < 100) PtShift = 0.996;
      else if(el.Pt() < 250) PtShift = 0.998;
      else                   PtShift = 1.;
    }
    if(el.etaRegionString() == "EE2"){
      if(el.Pt() < 30)       PtShift = 0.986;
      else if(el.Pt() < 50)  PtShift = 0.993;
      else if(el.Pt() <  75) PtShift = 0.996;
      else if(el.Pt() < 100) PtShift = 0.997;
      else if(el.Pt() < 250) PtShift = 0.998;
      else                   PtShift = 0.999;
    }
  }


  if(GetYearString() == "2017"){
    if(el.etaRegionString() == "EB1"){
      if(el.Pt() < 30)       PtShift = 0.97;
      else if(el.Pt() < 50)  PtShift = 0.993;
      else if(el.Pt() < 75)  PtShift = 0.993;
      else if(el.Pt() < 100) PtShift = 0.998;
      else if(el.Pt() < 250) PtShift = 0.999;
      else                   PtShift = 1;
    }
    if(el.etaRegionString() == "EB2"){
      if(el.Pt() < 30)       PtShift = 0.985;
      else if(el.Pt() < 50)  PtShift = 0.991;
      else if(el.Pt() <  75) PtShift = 0.995;
      else if(el.Pt() < 100) PtShift = 0.997;
      else if(el.Pt() < 250) PtShift = 0.999;
      else                   PtShift = 1;
    }
    if(el.etaRegionString() == "EE1"){
      if(el.Pt() < 30)       PtShift = 0.976;
      else if(el.Pt() < 50)  PtShift = 0.985;
      else if(el.Pt() < 100) PtShift = 0.992;
      else if(el.Pt() <  75) PtShift = 0.995;
      else if(el.Pt() < 250) PtShift = 0.997;
      else                   PtShift = 0.998;
    }
    if(el.etaRegionString() == "EE2"){
      if(el.Pt() < 30)       PtShift = 0.985;
      else if(el.Pt() < 50)  PtShift = 0.991;
      else if(el.Pt() <  75) PtShift = 0.994;
      else if(el.Pt() < 100) PtShift = 0.996;
      else if(el.Pt() < 250) PtShift = 0.998;
      else                   PtShift = 0.999;
    }

  }


  if(GetYearString() == "2018"){
    if(el.etaRegionString() == "EB1"){
      if(el.Pt() < 30)       PtShift = 0.971;
      else if(el.Pt() < 50)  PtShift = 0.995;
      else if(el.Pt() <  75) PtShift = 0.999;
      else if(el.Pt() < 100) PtShift = 0.999;
      else if(el.Pt() < 250) PtShift = 1;
      else                   PtShift = 1;
    }
    if(el.etaRegionString() == "EB2"){
      if(el.Pt() < 30)       PtShift = 0.987;
      else if(el.Pt() < 50)  PtShift = 0.992;
      else if(el.Pt() <  75) PtShift = 0.995;
      else if(el.Pt() < 100) PtShift = 0.998;
      else if(el.Pt() < 250) PtShift = 0.998;
      else                   PtShift = 1;
    }
    if(el.etaRegionString() == "EE1"){
      if(el.Pt() < 30)       PtShift = 0.976;
      else if(el.Pt() < 50)  PtShift = 0.985;
      else if(el.Pt() <  75) PtShift = 0.992;
      else if(el.Pt() < 100) PtShift = 0.994;
      else if(el.Pt() < 250) PtShift = 0.997;
      else                   PtShift = 0.999;
    }
    if(el.etaRegionString() == "EE2"){
      if(el.Pt() < 50)       PtShift = 0.985;
      else if(el.Pt() < 50)  PtShift = 0.991;
      else if(el.Pt() <  75) PtShift = 0.994;
      else if(el.Pt() < 100) PtShift = 0.996;
      else if(el.Pt() < 250) PtShift = 0.997;
      else                   PtShift = 0.998;
    }

  }


  return PtShift;
}


double HNL_LeptonCore::GetIsoFromID(Lepton lep, TString id){

  double pt = lep.Pt();
  double eta = lep.Eta();

  if (lep.IsMuon()) {

    if (id == "Peking")        return 0.15;
    if (id == "TopHNT")        return 0.1;
    if (id == "TopHN")         return 0.1;
    if (id == "HNTight_17028") return 0.07;
    if (id == "HNTightV1")     return 0.07;
    if (id == "HNTightV2")     return 0.07;
    if (id == "POGTightPFIsoVeryVeryTight") return 0.05;
    if (id.Contains("TightPFIsoVeryVeryTight")) return 0.05;
    if (id.Contains("TightPFIsoVeryTight")) return 0.1;
    if (id.Contains("TightPFIsoTight")) return 0.15;
    if (id.Contains("TightWithTightIso")) return 0.15;
    if (id.Contains("TightStandardPFIsoTight")) return 0.15;
    if (id.Contains("PFIsoMedium")) return 0.2;
    if (id.Contains("PFIsoLoose")) return 0.25;
    if (id.Contains("PFIsoVeto")) return 0.4;
    if (id == "POGHighPtTight") return 0.1;
    if (id == "POGHighPtMixTight") return 0.1;
    if (id.Contains("HNMVA_")) return 0.1;
  }
  else {

    if (id == "Peking_2016")      {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if (id == "Peking_2017" || id == "Peking_2018")      {
      if(fabs(eta) < 1.479) return 0.0571;
      else  return 0.05880;
    }

    if( id == "TopHNSST" ) return 0.1;
    if( id == "HNTight_17028") return 0.08;
    if( id.Contains("HNTightV")) {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id == "HN2016") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.06);
    }
    if( id == "HN2017") {
      if(fabs(eta) < 1.479) return 0.085;
      else  return (0.05);
    }
    if( id == "HN2018") {
      if(fabs(eta) < 1.479) return 0.095;
      else  return (0.07);
    }
    if( id == "HNRelaxedIP2016") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.05);
    }
    if( id == "HNRelaxedIP2017") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.05);
    }
    if( id == "HNRelaxedIP2018") {
      if(fabs(eta) < 1.479) return 0.095;
      else  return (0.07);
    }
    if( id == "passTightID_nocc") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id.Contains("passPOGTight")){
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));

    }
    if( id.Contains("passPOGMedium")){
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "passTightID") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id.Contains("HNMediumV")) {
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "passMediumID") {
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "HN2016POG") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }

    if( id == "Iso1") {
      if(fabs(eta) < 1.479) return  0.08;
      else  return 0.08;
    }
    if( id == "Iso2") {
      if(fabs(eta) < 1.479) return  0.09;
      else  return 0.08;
    }
    if( id == "Iso3") {
      if(fabs(eta) < 1.479) return  0.1;
      else  return 0.08;
    }
    if( id == "Iso4") {
      if(fabs(eta) < 1.479) return  0.12;
      else  return 0.08;
    }
    if( id == "Iso5") {
      if(fabs(eta) < 1.479) return  0.09;
      else  return 0.09;
    }
    if( id == "Iso6") {
      if(fabs(eta) < 1.479) return  0.1;
      else  return 0.1;
    }
    if( id == "Iso7") {
      if(fabs(eta) < 1.479) return  0.12;
      else  return 0.12;
    }

    if( id.Contains("HNTight_Opt")) return 0.08;

    if( id.Contains("HN2016MVA")) return 0.08;
    if( id.Contains("HN2016POG")) return 0.08;
    if( id == "passMVAID_noIso_WP90V16") return 0.05;
    if( id == "passMVAID_noIso_WP80") return 0.08;
    if( id == "passMVAID_noIso_WP90") return 0.08;
    if( id == "passMVAID_Iso_WP80") return 999.0;
    if( id == "passMVAID_Iso_WP90") return 999.0;

    if (id.Contains("HNMVA_")) return 0.1;


  }
  cout << "[AnalyzerCore::GetIsoFromID ] ID not found.." << id<< endl;
  exit(EXIT_FAILURE);

  return -999999999.;

}
