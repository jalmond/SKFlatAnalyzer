#include "HNL_Lepton_Conversion_Studies.h"

void HNL_Lepton_Conversion_Studies::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_Lepton_Conversion_Studies::executeEvent(){


 
  Event ev = GetEvent();

  AnalyzerParameter param = HNL_LeptonCore::InitialiseHNLParameter("HNL_ULID");
  double weight =SetupWeight(ev,param);


  std::vector<Electron>   ElectronCollV = GetElectrons("HNL_ULID_Baseline", 15., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    ("HNL_ULID_Baseline", 10., 2.4);
  
  TString PlotDir="Inclusive";

  std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV,param);


  std::vector<Lepton *> LepsT       = LepsV;   
  Particle METv = GetMiniAODvMET("PuppiT1xyULCorr"); // reyturns MET with systematic correction                                              
  std::map< TString, bool > map_Region_to_Bool;
  map_Region_to_Bool.clear();

  
  int nT=0;
  int nL_Conv=0;
  for(unsigned int ilep=0; ilep < ElectronCollV.size(); ilep++) {
    
    if(ElectronCollV[ilep].PassID(param.Electron_Tight_ID))nT++;
    else if( (ElectronCollV.at(ilep).MVAKey("Conv_EDv5") < 0) && (ElectronCollV.at(ilep).MVAKey("Fake_EDv5") > 0) && (ElectronCollV.at(ilep).MVAKey("CF_EDv5") > 0.6)) nL_Conv++;
    
  }
  for(unsigned int ilep=0; ilep < MuonCollV.size(); ilep++) {
    if(MuonCollV[ilep].PassID(param.Muon_Tight_ID))nT++;

  }

  TString Type = "";
  for(unsigned int ilep=0; ilep < LepsT.size(); ilep++) {
    if(LepsT[ilep]->LeptonIsCF()) Type+="CF";
    else if(LepsT[ilep]->IsFake())  Type+="Fake";
    else if(LepsT[ilep]->IsConv()) Type+="Conv";
    else if(LepsT[ilep]->IsPrompt())  Type+="Prompt";
  }

  if(!SameCharge(LepsV))  return;

  TString EvType = "";
  if(Type=="PromptPrompt") EvType = "Prompt";
  else if(Type.Contains("Fake")) EvType = "Fake";
  else if(Type.Contains("Conv")) EvType = "Conv";
  else EvType = "CF";

  if(IsData) EvType = "Data";

  map_Region_to_Bool.clear();
  map_Region_to_Bool["Inclusive"] = true;
  map_Region_to_Bool["ConvSB"] = (nT==1 && nL_Conv ==1 && ElectronCollV.size()==2) ;
  map_Region_to_Bool["ConvSBEMu"] = (nT==1 && nL_Conv ==1 && ElectronCollV.size()==1) ;
  map_Region_to_Bool["TT"] = (nT==2 && ElectronCollV.size()==2);
  map_Region_to_Bool["TTEMu"] = (nT==2 && ElectronCollV.size()==1);

  for(std::map< TString, bool >::iterator it = map_Region_to_Bool.begin(); it != map_Region_to_Bool.end(); it++){
    
    if(it->second){
    
      Particle LL = *LepsT[0] + *LepsT[1];
      
      PlotDir= it->first + "/"+EvType;
      
      FillHist( (PlotDir + "/LL/mass_ll").Data(), LL.M(),weight, 250, 0, 500);

      double ptbins[8] = { 0., 10.,15., 20.,25.,30., 40., 100.};
      
      for(unsigned int ilep=0; ilep < LepsT.size(); ilep++){
	
	double lpt = (LepsT.at(ilep)->Pt() > 100.) ? 99.: LepsT.at(ilep)->Pt();
	TString SFlav = LepsT.at(ilep)->sLepGenType();
	
	if(LepsT.at(ilep)->GetFlavour() == "Electron") FillHist( (PlotDir + "/EE_LL/lep_conv").Data(), LepsT.at(ilep)->MVAKey("Conv_EDv5"),weight, 100, -1, 1);
	if(LepsT.at(ilep)->GetFlavour() == "Electron")FillHist( (PlotDir + "/EE_LL/lep_cf").Data(), LepsT.at(ilep)->MVAKey("CF_EDv5"), weight,100,-1, 1);
	if(LepsT.at(ilep)->GetFlavour() == "Electron")FillHist( (PlotDir + "/EE_LL/lep_fake").Data(), LepsT.at(ilep)->MVAKey("Fake_EDv5"), weight,100,-1, 1);
	
	FillHist( (PlotDir + "/EE_LL/lep_pt").Data(),lpt, weight, 7, ptbins);
	if(nT>=1)         FillHist( (PlotDir + "/EE_TL/lep_pt").Data(),lpt, weight, 7, ptbins);
	if(nT==2)         FillHist( (PlotDir + "/EE_TT/lep_pt").Data(),lpt, weight, 7, ptbins);

      }
    }
  }
  
}




HNL_Lepton_Conversion_Studies::HNL_Lepton_Conversion_Studies(){


}
 
HNL_Lepton_Conversion_Studies::~HNL_Lepton_Conversion_Studies(){

}
