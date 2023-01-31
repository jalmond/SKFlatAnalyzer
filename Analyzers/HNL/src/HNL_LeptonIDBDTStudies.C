#include "HNL_LeptonIDBDTStudies.h"

void HNL_LeptonIDBDTStudies::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_LeptonIDBDTStudies::executeEvent(){
  
  if((_jentry==0)){ 
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer
    TriggerPrintOut(GetEvent());
  }
  
  if(!IsData)  gens = GetGens();

  TString ID = "MVAUL";

  AnalyzerParameter param  = HNL_LeptonCore::InitialiseHNLParameter(ID,"");
  
  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);
    
  std::vector<Electron>   ElectronCollProbe = GetElectrons("HNL_ULID_Baseline", 10., 2.5); 
  std::vector<Muon>       MuonCollProbe     = GetMuons    (param.Muon_Veto_ID, 10., 2.4);
  if(MuonCollProbe.size() > 0) return;

  std::vector<Jet> BJetColl  = GetHNLJets("BJetM", param);

    
  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(MuonCollProbe,ElectronCollProbe);

  if(BJetColl.size() > 0 ) return;
  
  if(!PassMETFilter()) return;

  HNL_LeptonCore::Channel dilep_channel= EE;

  MakeBDTPlots(param,dilep_channel, ElectronCollProbe, param.Electron_Tight_ID, weight);

  return;
  
}


void HNL_LeptonIDBDTStudies::MakeBDTPlots(AnalyzerParameter param,HNL_LeptonCore::Channel dilep_channel, vector<Electron> ElectronColl, TString ID, double weight_ll){

  TString channel_string = GetChannelString(dilep_channel) ;

  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(ElectronColl);
  
  if(!IsData){

    vector<Electron> ElectronCollFake     = SkimLepColl(ElectronColl, gens, param, "HFake");
    FillAllElectronPlots("FakeElectron","MC_Electrons",ElectronCollFake,weight_ll);

    vector<Electron> ElectronCollCF     = SkimLepColl(ElectronColl, gens, param, "CF");
    FillAllElectronPlots("CFElectron","MC_Electrons",ElectronCollCF,weight_ll);

    vector<Electron> ElectronCollConv     = SkimLepColl(ElectronColl, gens, param, "NHConv");
    FillAllElectronPlots("ConvElectron","MC_Electrons",ElectronCollConv,weight_ll);

    vector<Electron> ElectronCollPrompt   = SkimLepColl(ElectronColl, gens, param, "PromptNoCF");
    FillAllElectronPlots("PromptElectron","MC_Electrons",ElectronCollPrompt,weight_ll);
  }
  

  if(!CheckLeptonFlavourForChannel(dilep_channel, LeptonColl)) return; // Check if EE cahnnel has 2 el                            
  Event ev = GetEvent();

  if(!PassTriggerSelection(dilep_channel, ev, LeptonColl,"Dilep")) return;


  if(LeptonColl.size() != 2) return;
 
  Particle METv = GetvMET("PuppiT1xyCorr"); // reyturns MET with systematic correction                                            
  if(METv.Pt() > 40) return;

  // use Z peak                                                                                                                              
  if(fabs(GetLLMass(LeptonColl)- 90.1) < 10) {
    
    if(ElectronColl[0].PassID("passPOGTight") && ElectronColl[1].PassID("passPOGTight") ){
      AnalyzerParameter paramPOG=param;
      paramPOG.Electron_ID_SF_Key = "passTightID";
      double POGSF = GetElectronSFEventWeight(ElectronColl,paramPOG);
      if(!IsData)  POGSF*= 0.95;
      FillHist(channel_string+"/"+ID+"/llmass_"+channel_string, GetLLMass(LeptonColl), weight_ll*POGSF, 70, 50., 120);
      FillHist(channel_string+"/"+ID+"/NVTX_"+channel_string, nPileUp, weight_ll*POGSF, 100, 0., 100);
      FillHist(channel_string+"/"+ID+"/MET_"+channel_string, GetvMET("PuppiT1xyCorr").Pt(), weight_ll*POGSF, 100, 0., 100);
    }
    
    for(auto iel : ElectronColl) {
      if(iel.PassID("HNL_ULID_Baseline")){
	
	if(SameCharge(ElectronColl))FillHist( channel_string+"_SS/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");
	else FillHist( channel_string+"_OS/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");
	
	if(SameCharge(ElectronColl))FillHist( channel_string+"_SS/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");
	else FillHist( channel_string+"_OS/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");
	
	if(SameCharge(ElectronColl))FillHist( channel_string+"_SS/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");
	else FillHist( channel_string+"_OS/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");

	if(iel.PassID("HNL_ULID_Fake")){
	  if(SameCharge(ElectronColl))FillHist( channel_string+"_SS_WithMVAFake/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");
	  else FillHist( channel_string+"_OS_WithMVAFake/CF_BDT_"+channel_string  , iel.HNL_MVA_CF(), weight_ll, 100, -1., 1., "MVA");

	  if(SameCharge(ElectronColl))FillHist( channel_string+"_SS_WithMVAFake/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");
	  else FillHist( channel_string+"_OS_WithMVAFake/Conv_BDT_"+channel_string  , iel.HNL_MVA_Conv(), weight_ll, 100, -1., 1., "MVA");

	  if(SameCharge(ElectronColl))FillHist( channel_string+"_SS_WithMVAFake/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");
	  else FillHist( channel_string+"_OS_WithMVAFake/Fake_BDT_"+channel_string  , iel.HNL_MVA_Fake(), weight_ll, 100, -1., 1., "MVA");
  
	}
      }
    }
       
    if(!ZmassOSSFWindowCheck(LeptonColl, 10.)) return;

    FillAllElectronPlots("OSElectron","Z",ElectronColl,weight_ll);

    vector<Electron> ElectronCollDATATight;
    vector<Electron> ElectronCollDATAFake;
    for(auto iel : ElectronColl){
      if(iel.IsGsfCtfScPixChargeConsistent() && iel.SIP3D() < 3) ElectronCollDATATight.push_back(iel);
      if( iel.SIP3D() > 5) ElectronCollDATAFake.push_back(iel);
    }
    FillAllElectronPlots("OSElectron","ZTight",ElectronCollDATATight,weight_ll);
    FillAllElectronPlots("OSElectron","ZFake" ,ElectronCollDATAFake,weight_ll);

    
    std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
    std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);
    
    
    std::vector<FatJet>   AK8_JetColl = GetHNLAK8Jets("", param);
    std::vector<Jet>      JetColl     = GetHNLJets("Tight", param);
    std::vector<Jet> BJetColl  = GetHNLJets("BJetM", param);
    
    std::vector<Tau> TauColl;

    Fill_RegionPlots(dilep_channel, 0, param.Name,"ZPeak" ,  TauColl, JetColl, AK8_JetColl, LeptonColl,  GetvMET("PuppiT1xyCorr"), nPV, weight_ll,param.WriteOutVerbose);
  }

  return;

}



HNL_LeptonIDBDTStudies::HNL_LeptonIDBDTStudies(){


}
 
HNL_LeptonIDBDTStudies::~HNL_LeptonIDBDTStudies(){

}

