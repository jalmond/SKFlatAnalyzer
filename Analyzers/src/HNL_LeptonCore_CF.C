#include "HNL_LeptonCore.h"


double HNL_LeptonCore::ReturnCFWeight(double CFRates){

  double W = CFRates/(1-CFRates);

  return W;
}


double HNL_LeptonCore::ReturnCFWeight(vector<double> CFRates){

  double W = 0.;
  for(auto i : CFRates) W += (i/(1-i));
  return W;
}


double HNL_LeptonCore::GetCFSF(AnalyzerParameter param, Lepton*  lep, bool ApplySF){
  if(lep->IsMuon()) return 1.;

  
  return GetCFSF(param,lep->GetEtaRegion(), ApplySF);

}
double HNL_LeptonCore::GetCFSF(AnalyzerParameter param, Lepton  lep, bool ApplySF){
  if(lep.IsMuon()) return 1.;
  return GetCFSF(param,lep.GetEtaRegion(), ApplySF);

}
double HNL_LeptonCore::GetCFSF(AnalyzerParameter param, TString EraReg, bool ApplySF){

  if(!ApplySF) return 1;
  /*
    Function to get DATA/MC SF for CF Bkg
    Depends on ID/Eta
   */

  double _SF=1.;

  map<TString,vector<double> > CFSFValues;
  CFSFValues["HNL_ULID_BB"]      =  {0.987,1.064,1.398,1.413};
  CFSFValues["HNL_ULID_EC"]      =  {0.93,0.934,1.332,1.241};

  CFSFValues["passPOGTight_BB"]      =  {1.097, 1.04, 1.49, 1.54};
  CFSFValues["passPOGTight_EC"]  =  {1.01,1, 1.30, 1.35};
  
  CFSFValues["HNTightV2_BB"]     =  {0.842,0.967, 1.34, 1.44};
  CFSFValues["HNTightV2_EC"]     =  {0.84,0.90, 1.37, 1.40};
  
  CFSFValues["TopHNSST_BB"]      =  {0.80,     0.79,     1.162, 1.47};
  CFSFValues["TopHNSST_EC"]      =  {0.78,     0.83,     1.31,  1.28};

  map<TString,vector<double> >::iterator CFMapIter ;

  TString CFKey = param.Electron_Tight_ID+"_"+EraReg;
  CFKey=CFKey.ReplaceAll("_"+GetYearString(),"");
  CFMapIter = CFSFValues.find(CFKey);
  if(CFMapIter == CFSFValues.end()) {  cout << "[HNL_LeptonCore::GetCFSF ] ERROR in CFSF.. " << CFKey  << endl;   exit(EXIT_FAILURE);}
  _SF= CFMapIter->second[GetEraNum()-1];
  

  if(param.syst_ == AnalyzerParameter::CFSFUp) {
    if(EraReg== "BB") _SF *= 1.09;
    else  _SF *= 1.07;
  }
  if(param.syst_ == AnalyzerParameter::CFSFDown) {
    if(EraReg== "BB") _SF *= 1/1.09;
    else  _SF *= 1/1.07;
  }


  return _SF;

}


double HNL_LeptonCore::GetCFWeightElectron(std::vector<Electron> El ,  AnalyzerParameter param, bool ApplySF){

  std::vector<Lepton *> Leptons = MakeLeptonPointerVector(El,param);
  return GetCFWeightElectron(Leptons, param, -1, ApplySF);

}

double HNL_LeptonCore::GetCFWeightElectron(std::vector<Lepton* > leps ,  AnalyzerParameter param,int nEl,  bool ApplySF){

  if(leps.size()  != 2) return 1.;
  int nElIt = 0;

  double cf_weight = 0;
  
  int sysR  = 0;
  if(param.syst_ == AnalyzerParameter::CFRateUp) sysR = 1;
  if(param.syst_ == AnalyzerParameter::CFRateDown) sysR = -1;
  

  for(auto ilep : leps){
    if(ilep->LeptonFlavour()==Lepton::ELECTRON){
      
      if(nElIt==nEl) {
	//	double el_cf_rate =   cfEst->GetElectronCFRate(param.Electron_Tight_ID, param.k.Electron_CF,ilep->defEta(),ilep->Pt(), sysR);
	double el_cf_rate =   cfEst->GetElectronCFRateFitted(param.Electron_Tight_ID, param.k.Electron_CF,ilep->defEta(),ilep->Pt(), sysR);
	el_cf_rate *= GetCFSF(param,ilep,ApplySF);
	cf_weight += (el_cf_rate / (1.-el_cf_rate));
      }
      else if(nEl == -1){
	double el_cf_rate =   cfEst->GetElectronCFRateFitted(param.Electron_Tight_ID, param.k.Electron_CF,ilep->defEta(),ilep->Pt(), sysR);
	el_cf_rate *= GetCFSF(param,ilep,ApplySF);
        cf_weight += (el_cf_rate / (1.-el_cf_rate));
      }
    }
    nElIt++;
  }
  

  if(run_Debug) cout << "GetCFWeightElectron weight=" << cf_weight << endl;


  //// Appply Mass Dependant Corr for 2017 for now
  if(IsData && DataEra == "2017"){
    if(param.syst_ == AnalyzerParameter::CFSFUp) {
      if(GetLLMass(leps) < 90.) cf_weight *= 1.2;
    }
  }

  return cf_weight;
}




double HNL_LeptonCore::GetZMassShift(vector<Electron> Electrons) {

  if(Electrons.size() != 2) return 1;

  if(Electrons[0].Charge() == Electrons[1].Charge()) return 1;

  if(DataEra=="2016preVFP"){
    if(Electrons[0].IsBB() && Electrons[1].IsBB())  return 0.993;
    if(Electrons[0].IsEC() && Electrons[1].IsEC())  return 0.996;
    return (0.993+0.996)/2.;

  }
  else  if(DataEra=="2016postVFP"){
    if(Electrons[0].IsBB() && Electrons[1].IsBB())  return 0.992;
    if(Electrons[0].IsEC() && Electrons[1].IsEC())  return 0.995;
    return (0.992+0.995)/2.;

  }
  if(DataEra=="2017"){
    if(Electrons[0].IsBB() && Electrons[1].IsBB())  return 0.987;
    if(Electrons[0].IsEC() && Electrons[1].IsEC())  return 0.993;
    return (0.987+0.993)/2.;
  }
  else if(DataEra=="2018"){
    /// 2018/16 Need updating
    if(Electrons[0].IsBB() && Electrons[1].IsBB())  return 0.988;
    if(Electrons[0].IsEC() && Electrons[1].IsEC())  return 0.993;
    return (0.988+0.993)/2.;
  }
  
  return 1;

}

double HNL_LeptonCore::PtExtrap(double val, double x1, double x2, double y1, double y2){

  double ptdirr = val - x1;
  double corr = ((y2-y1) / (x2-x1)) *ptdirr;
  return y1 + corr;

}
double HNL_LeptonCore::GetShiftCFEl(Electron el,TString ID, bool ApplyDataCorr,TString Method) {
  
  if(Method == "Extrap" && ID.Contains("HNL")){
    if(DataEra.Contains("2016")){
      if(el.IsBB()){
	if(el.Pt() < 50) return 0.971;
	else if(el.Pt() < 75) return PtExtrap(el.Pt() , 50,75, 0.971,0.987);
	else if(el.Pt() < 150) return PtExtrap(el.Pt() , 75,150, 0.987,0.997);
	else return 0.997;
      }
      else{
	
        if(el.Pt() < 30)         return  0.951;
        else if(el.Pt() < 40)    return  PtExtrap(el.Pt() , 30,40., 0.951,0.972);
        else if(el.Pt() < 62.5)  return  PtExtrap(el.Pt() , 40,62.5, 0.972,0.983);
	else  if(el.Pt() < 87.5) return  PtExtrap(el.Pt() , 62.5,87.5, 0.983,0.989);
	else  if(el.Pt() < 150)  return  PtExtrap(el.Pt() , 87.5,150, 0.989,0.995);
        else  return 0.995;
      }
    }
    
    if(DataEra=="2017"){

      if(el.IsBB()){
	if(el.Pt() < 50) return 0.969;
	else if(el.Pt() < 75) return PtExtrap(el.Pt() , 50,75, 0.969,0.987);
	else if(el.Pt() < 150) return PtExtrap(el.Pt() , 75,150, 0.987,0.995);
	else return 0.995;
      } 
      else{
        if(el.Pt() < 30)         return  0.949;
        else if(el.Pt() < 40)    return  PtExtrap(el.Pt() , 30,40, 0.949,0.969);
        else if(el.Pt() < 62.5)  return  PtExtrap(el.Pt() , 40,62.5, 0.969,0.982);
	else  if(el.Pt() < 87.5) return  PtExtrap(el.Pt() ,62.5, 87.5, 0.982,0.989);
	else  if(el.Pt() < 150)  return  PtExtrap(el.Pt() , 87.5,150, 0.989,0.995);
        else  return 0.995;
      }
    }
    
    if(DataEra=="2018"){
      
      if(el.IsBB()){
	if(el.Pt() < 50)       return 0.971;
        else if(el.Pt() < 75)  return PtExtrap(el.Pt() , 50,75, 0.971,0.985);
        else if(el.Pt() < 150) return PtExtrap(el.Pt() , 75,150, 0.985,0.997);
        else return 0.997;
      }
      else{
        if(el.Pt() < 30)         return  0.948;
        else if(el.Pt() < 40)    return  PtExtrap(el.Pt() , 30,40, 0.948,0.971);
        else if(el.Pt() < 62.5)  return  PtExtrap(el.Pt() , 40,62.5, 0.971,0.982);
        else  if(el.Pt() < 87.5) return  PtExtrap(el.Pt() , 62.5,87.5, 0.982,0.988);
        else  if(el.Pt() < 150)  return  PtExtrap(el.Pt() , 87.5,150, 0.988,0.995);
        else  return 0.995;
      }
    }
    
  }
    
  return  GetShiftCFEl(el.Pt(),el.IsBB(), ID, ApplyDataCorr, "minChi2" );
  
}

double HNL_LeptonCore::GetShiftCFEl(double el_pt, bool IsBB,TString ID, bool ApplyDataCorr,TString Method) {

  //// Get Shift for  Prompt -> CF Pt response                                                                                                                                                                   

  double PtShift = 1.;
  if(ID == "TopHNSST") return 1.;

  if(ID == "passPOGTight"){
    if(DataEra.Contains("2016")){
      if(IsBB){
	if(el_pt < 50)        return  (Method == "minChi2")  ? 0.985 : 0.978;
	else  if(el_pt < 100) return  (Method == "minChi2")  ? 0.996 : 0.992;
	else  if(el_pt < 200) return  (Method == "minChi2")  ? 0.998: 0.998 ;
	else  return  (Method == "minChi2") ?  0.998 : 0.998;
      }
      if(!IsBB){
	if(el_pt < 30)        return  (Method == "minChi2") ? 0.975 : 0.971 ;
	else if(el_pt < 50)   return  (Method == "minChi2") ? 0.989 : 0.985;
	else if(el_pt < 75)   return  (Method == "minChi2") ? 0.993 : 0.992;
	else  if(el_pt < 100) return  (Method == "minChi2") ? 0.996 : 0.996;
	else  if(el_pt < 200) return  (Method == "minChi2") ? 0.998 : 0.998;
	else  return  (Method == "minChi2") ? 0.999 : 0.999 ;
      }
    }
    
    if(DataEra=="2017"){
      if(IsBB){
	if(el_pt < 50)        return  (Method == "minChi2") ? 0.986 : 0.976;
	else  if(el_pt < 100) return  (Method == "minChi2") ? 0.996 : 0.992;
	else  if(el_pt < 200) return  (Method == "minChi2")  ? 0.999 : 0.997;
	else  return  (Method == "minChi2") ?   1 : 1;
      }
      if(!IsBB){
	if(el_pt < 30)        return  (Method == "minChi2")  ? 0.972:0.967;
	else if(el_pt < 50)   return  (Method == "minChi2")  ?  0.988 : 0.985; 
	else if(el_pt < 75)   return  (Method == "minChi2")  ?  0.991 : 0.990;
	else  if(el_pt < 100) return  (Method == "minChi2") ?  0.995 : 0.995;
	else  if(el_pt < 200) return  (Method == "minChi2") ?  0.997 : 0.997;
	else  return  (Method == "minChi2")   ? 0.998 : 0.998;
      }
    }
    
    if(DataEra=="2018"){
      if(IsBB){
	if(el_pt < 50)        return  (Method == "minChi2")  ? 0.985 : 0.976;
	else  if(el_pt < 100) return  (Method == "minChi2")  ? 0.996 : 0.992;
	else  if(el_pt < 200) return  (Method == "minChi2")  ? 0.998 : 0.997;
	else  return  (Method == "minChi2") ?  1 : 1;
      }
      if(!IsBB){
	if(el_pt < 30)        return  (Method == "minChi2") ?  0.971: 0.967;
	else if(el_pt < 50)   return  (Method == "minChi2") ?  0.987 : 0.985;
	else if(el_pt < 75)   return  (Method == "minChi2") ?  0.991 : 0.990;
	else  if(el_pt < 100) return  (Method == "minChi2") ?  0.995 : 0.995;
	else  if(el_pt < 200) return  (Method == "minChi2") ?  0.997 : 0.997;
	else  return  (Method == "minChi2") ? 0.998 : 0.998;
      }
    }
  }

  else if(ID == "HNTightV2"){
    if(DataEra.Contains("2016")){
      if(IsBB){
        if(el_pt < 50)        return  (Method == "minChi2")  ? 0.972 : 0.957;
        else  if(el_pt < 100) return  (Method == "minChi2")  ? 0.989 : 0.983;
        else  if(el_pt < 200) return  (Method == "minChi2")  ? 0.995 : 0.992;
        else  return  (Method == "minChi2") ?  0.994 : 0.999; 
      }
      if(!IsBB){
        if(el_pt < 30)        return  (Method == "minChi2") ?  0.959 : 0.946;
        else if(el_pt < 50)   return  (Method == "minChi2") ?  0.977 : 0.972;
        else if(el_pt < 75)   return  (Method == "minChi2") ?   0.986 : 0.983;
        else  if(el_pt < 100) return  (Method == "minChi2") ? 0.991 : 0.990;
        else  if(el_pt < 200) return  (Method == "minChi2") ?  0.994 : 0.993;
        else  return  (Method == "minChi2") ?   1 : 1;
      }
    }

    if(DataEra=="2017"){
      if(IsBB){
        if(el_pt < 50)        return  (Method == "minChi2")  ? 0.972 : 0.957; 
        else  if(el_pt < 100) return  (Method == "minChi2")  ? 0.988 : 0.983;
        else  if(el_pt < 200) return  (Method == "minChi2")  ? 0.993 : 0.992;
        else  return  (Method == "minChi2") ?    0.994 : 0.999;
      }
      if(!IsBB){
        if(el_pt < 30)        return  (Method == "minChi2") ?  0.953 : 0.946; 
        else if(el_pt < 50)   return  (Method == "minChi2") ?  0.975 : 0.972;
        else if(el_pt < 75)   return  (Method == "minChi2") ?  0.985 : 0.983;
        else  if(el_pt < 100) return  (Method == "minChi2") ?  0.991 : 0.990;
        else  if(el_pt < 200) return  (Method == "minChi2") ?   0.994 : 0.993;
        else  return  (Method == "minChi2") ?  1 : 1;
      }
    }

    if(DataEra=="2018"){
      if(IsBB){
        if(el_pt < 50)        return  (Method == "minChi2")  ? 0.969 : 0.955;
        else  if(el_pt < 100) return  (Method == "minChi2")  ? 0.987 : 0.985;
        else  if(el_pt < 200) return  (Method == "minChi2")  ? 0.993 : 0.992;
        else  return  (Method == "minChi2") ?  0.994 : 0.999;
      }
      if(!IsBB){
        if(el_pt < 30)        return  (Method == "minChi2") ?  0.953 : 0.946;
        else if(el_pt < 50)   return  (Method == "minChi2") ?  0.975 : 0.972;
        else if(el_pt < 75)   return  (Method == "minChi2") ?  0.985 : 0.983;
        else  if(el_pt < 100) return  (Method == "minChi2") ?  0.991 : 0.990;
        else  if(el_pt < 200) return  (Method == "minChi2") ?  0.994 : 0.993;
        else  return  (Method == "minChi2") ? 1 : 1;
      }
    }
  }
  else if(ID.Contains("HNL_ULID")){
    
    
    double DataCorr = 1.;
    
    if(ApplyDataCorr){
      if(DataEra=="2016preVFP"  && IsBB) DataCorr=0.975; 
      if(DataEra=="2016postVFP" && IsBB) DataCorr=0.99; 
      if(DataEra=="2017"        && IsBB) DataCorr=0.975; 
      if(DataEra=="2018"        && IsBB) DataCorr=0.975; 
      
      if(DataEra=="2016preVFP"  && !IsBB) DataCorr=0.995;
      if(DataEra=="2016postVFP" && !IsBB) DataCorr=1.;
      if(DataEra=="2017"        && !IsBB) DataCorr=1.;
      if(DataEra=="2018"        && !IsBB) DataCorr=0.98;
    }

    if(DataEra.Contains("2016")){
      if(IsBB){
	if(el_pt < 50)        return  (Method == "minChi2")  ? DataCorr*0.971 : DataCorr*0.956 ;
	else  if(el_pt < 100) return  (Method == "minChi2")  ? DataCorr*0.987: DataCorr*0.979 ;
	else  if(el_pt < 200) return  (Method == "minChi2")  ? DataCorr*0.997: DataCorr*0.991 ;
	else  return  (Method == "minChi2") ?  DataCorr*0.998: DataCorr*0.997 ;
      }
      if(!IsBB){
	if(el_pt < 30)        return  (Method == "minChi2") ? DataCorr*0.951 : DataCorr*0.944 ;
	else if(el_pt < 50)   return  (Method == "minChi2") ? DataCorr*0.972 : DataCorr*0.965 ;
	else if(el_pt < 75)   return  (Method == "minChi2") ? DataCorr*0.983 : DataCorr*0.979 ;
	else  if(el_pt < 100) return  (Method == "minChi2") ? DataCorr*0.989 : DataCorr*0.987 ;
	else  if(el_pt < 200) return  (Method == "minChi2") ? DataCorr*0.994 : DataCorr*0.992 ;
	else  return  (Method == "minChi2") ? DataCorr*0.998: DataCorr*0.999 ;
      }
    }
    
    if(DataEra=="2017"){
      if(IsBB){
	if(el_pt < 50)        return  (Method == "minChi2")  ? DataCorr*0.969 : DataCorr*0.951 ;
	else  if(el_pt < 100) return  (Method == "minChi2") ? DataCorr*0.987 : DataCorr*0.977;
	else  if(el_pt < 200) return  (Method == "minChi2")  ?  DataCorr*0.995 : DataCorr*0.990 ;
	else  return  (Method == "minChi2") ?  DataCorr*0.995 : DataCorr*0.9965 ;
      }
      if(!IsBB){
	if(el_pt < 30)        return  (Method == "minChi2") ?   DataCorr*0.949 : DataCorr*0.931 ;
	else if(el_pt < 50)   return  (Method == "minChi2") ?   DataCorr*0.969 : DataCorr*0.962;
	else if(el_pt < 75)   return  (Method == "minChi2") ?   DataCorr*0.982 : DataCorr*0.9785;
	else  if(el_pt < 100) return  (Method == "minChi2") ?  DataCorr*0.989 : DataCorr*0.9865;
	else  if(el_pt < 200) return  (Method == "minChi2") ?  DataCorr*0.993 : DataCorr*0.9925 ;
	else  return  (Method == "minChi2") ?  DataCorr*0.998 : DataCorr*0.9965;
      }
    }
    
    if(DataEra=="2018"){
      if(IsBB){
	if(el_pt < 50)        return  (Method == "minChi2")  ? DataCorr*0.971 : DataCorr*0.952 ;
	else  if(el_pt < 100) return  (Method == "minChi2")  ? DataCorr*0.985: DataCorr*0.977 ;
	else  if(el_pt < 200) return  (Method == "minChi2")  ? DataCorr*0.997: DataCorr*0.992 ;
	else  return  (Method == "minChi2") ?  DataCorr*0.999: 1 ;
      }
      if(!IsBB){
	if(el_pt < 30)        return  (Method == "minChi2") ? DataCorr*0.948 : DataCorr*0.937 ;
	else if(el_pt < 50)   return  (Method == "minChi2") ? DataCorr*0.971 : DataCorr*0.965 ;
	else if(el_pt < 75)   return  (Method == "minChi2") ? DataCorr*0.982 : DataCorr*0.977 ;
	else  if(el_pt < 100) return  (Method == "minChi2") ? DataCorr*0.988 : DataCorr*0.986 ;
	else  if(el_pt < 200) return  (Method == "minChi2") ? DataCorr*0.993 : DataCorr*0.991 ;
	else  return  (Method == "minChi2") ? DataCorr*0.996: DataCorr*0.995 ;
      }
    }

  }
  cout << "[HNL_LeptonCore::GetShiftCFEl ] ERROR in GetShiftCFEl.. " << ID <<  endl;
  exit(EXIT_FAILURE);

  return PtShift;
}

