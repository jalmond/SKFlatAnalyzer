#include "HNL_Validation.h"

void HNL_Validation::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_Validation::executeEvent(){
  
  TString ParamSetup="HNL";
  TString TriggerConfig = "Dilep";

  bool ValPOG=true; // Validate using POG ID
  if(ValPOG)  ParamSetup="POG";
  if(ValPOG)  TriggerConfig="POG";


  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter(ParamSetup,"_UL");
  

  if((_jentry==0)){
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer                                                                                            
    TriggerPrintOut(GetEvent());
    PrintParam(param_signal);
  }
  if(run_Debug) cout << "HNL_Validation::executeEvent " << endl;
  
  Event ev = GetEvent();
  //double weight =SetupWeight(ev,param_signal);

  double weight = 1.;
  double weight_pu (1.);
  double weight_puM(1.);
  double weight_dy (1.);
  double w_topptrw(1.);
  if(!IsData){
    weight = ev.GetTriggerLumi("Full") * MCweight(true, true) *  GetKFactor() * GetPrefireWeight(0);
    weight_pu  = GetPileUpWeight(nPileUp,0);
    weight_puM = GetPileUpWeight(nPileUp-1,0);
    weight_dy = (IsData) ? 1:  mcCorr->GetOfficialDYReweight(All_Gens,0);

    if(MCSample.Contains("TT") and MCSample.Contains("powheg")) {
      w_topptrw = mcCorr->GetTopPtReweight(All_Gens);
      FillWeightHist("TopPtWeight_" , w_topptrw);
    }
  }

  // HL ID                                                                                                                                                                                                
  std::vector<Electron>   ElectronCollV = GetElectrons(param_signal.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param_signal.Muon_Veto_ID, 5., 2.4);

  TString el_ID = (RunFake) ?  param_signal.Electron_FR_ID : param_signal.Electron_Tight_ID ;
  TString mu_ID = (RunFake) ?  param_signal.Muon_FR_ID :  param_signal.Muon_Tight_ID ;

  std::vector<Muon>       MuonCollT     = MuonPromptOnly    ( GetMuons    ( param_signal,mu_ID, 5, 2.4, RunFake)      ,All_Gens,param_signal);
  std::vector<Electron>   ElectronCollT = ElectronPromptOnly( GetElectrons( param_signal,el_ID, 10, 2.5, RunFake) ,All_Gens,param_signal);
 
  std::vector<Muon>       MuonCollTM     = MuonPromptOnly( MuonCollT,All_Gens);
  std::vector<Electron>   ElectronCollTM = ElectronPromptOnly( ElectronCollT,All_Gens);
  
  std::vector<Tau>        mytaus        = GetTaus     (param_signal.Tau_Veto_ID,20., 2.3); 

  // Creat Lepton vector to have lepton blind codes 

  std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(MuonCollT,ElectronCollT,param_signal);
  std::vector<Lepton *> LepsTM      = MakeLeptonPointerVector(MuonCollTM,ElectronCollTM,param_signal);
  
  std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);
  
  // JET COLLECTION
  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets("",param_signal);
  std::vector<Jet> AK4_JetAllColl                 = GetHNLJets("NoCut_Eta3",param_signal);
  std::vector<Jet> JetColl                        = GetHNLJets("Tight",param_signal);
  std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param_signal);
  std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param_signal);
  std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param_signal);
  std::vector<Jet> BJetCollSR1                    = GetHNLJets("BJetT",param_signal);

  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  double sf_btag               = GetBJetSF(param_signal, JetColl, param_jets);
  if(!IsData) FillWeightHist(param_signal.Name+"/BJetSF", sf_btag);

  
  if(IsData) sf_btag=1;

  // Chose Typ1 Phi corr MET + smear jets 

  map<TString, Particle> MapOfMET = METMap(param_signal);

  Particle PuppiMETv = GetvMET("PuppiT1xyULCorr",param_signal,true);
  if(!PassMETFilter()) return;
  
  vector<HNL_LeptonCore::Channel> channels = {MuMu, EMu};
  
  for(auto dilep_channel : channels){
    
    if(run_Debug) cout << "HNL_RegionDefinitions::RunAllSignalRegions " << GetChannelString(dilep_channel) <<  endl;
    
    if(!CheckLeptonFlavourForChannel(dilep_channel, LepsT)) continue;
    
    if(run_Debug) cout << "HNL_RegionDefinitions::CorrectChannelStream " << endl;

    if (!PassTriggerSelection(dilep_channel, ev, LepsT, TriggerConfig)) continue;
    
    if(run_Debug) cout << "HNL_RegionDefinitions::PassTriggerSelection " << endl;

    AnalyzerParameter param_channel = param_signal;
        
    TString channel_string = GetChannelString(dilep_channel);
    param_channel.Name =  channel_string + "/" + param_channel.DefName;
    
    float weight_channel = weight;

    double weight_Lep = 1.;
    if(!IsDATA && dilep_channel != MuMu)  weight_Lep*= GetElectronSFEventWeight(ElectronCollT, param_channel);
    if(!IsDATA && dilep_channel != EE)    weight_Lep*= GetMuonSFEventWeight(MuonCollT, param_channel);

    if(SameCharge(LepsT)) continue;

    if(RunFake){
      weight_channel = GetFakeWeight(LepsT, param_channel, true);
      FillWeightHist(param_channel.Name+"/FakeWeight",weight_channel);
    }

    int nel_hem(0);
    if (dilep_channel==EE){
      if (DataEra=="2018"){
	for(auto iel : LepsT){
	  if (iel->Eta() < -1.25){
	    if((iel->Phi() < -0.82) && (iel->Phi() > -1.62)) nel_hem++;
	  }
	}
      }
    }
    if(nel_hem > 0) continue;
    if (LepsT.size() != 2 ) continue;
    if (LepsV.size() != 2) continue;
    
    
    Particle llCand = *LepsT[0] + *LepsT[1];
  
    if(llCand.M() < 50) continue;

    std::map< TString, bool > map_Region_to_Bool;

    map_Region_to_Bool.clear();
    map_Region_to_Bool["ZPeak"]     =  (BJetColl.size() == 0 && LepsT[1]->Pt() > 15 && fabs(llCand.M() - 90) < 10) && (LepsTM.size()==2);
    map_Region_to_Bool["ZPeakPt50"] =  (BJetColl.size() == 0 && LepsT[1]->Pt() > 50 && fabs(llCand.M() - 90) < 10) && (LepsTM.size()==2);
    map_Region_to_Bool["BJetCR"]    =  (BJetColl.size() >= 1 && LepsT[1]->Pt() > 20 && JetColl.size() >= 2);
    
    std::map< TString, double > map_Region_to_Weight;
    map_Region_to_Weight.clear();
    map_Region_to_Weight["LumiWeight"]  = weight_channel;
    map_Region_to_Weight["PileUpWight"] = weight_channel*weight_pu;
    map_Region_to_Weight["LeptonID"]    = weight_channel*weight_pu*weight_Lep;
    map_Region_to_Weight["DYReweight"]  = weight_channel*weight_pu*weight_Lep*weight_dy;
    map_Region_to_Weight["BTagSF"]      = weight_channel*weight_pu*weight_Lep*sf_btag;
    map_Region_to_Weight["TopRW"]       = weight_channel*weight_pu*weight_Lep*sf_btag*w_topptrw;
    map_Region_to_Weight["TopRWPUM"]    = weight_channel*weight_puM*weight_Lep*sf_btag*w_topptrw;
    

    for(std::map< TString, bool >::iterator it = map_Region_to_Bool.begin(); it != map_Region_to_Bool.end(); it++){
      
      if(it->second){
	
	for(std::map< TString, double >::iterator itw = map_Region_to_Weight.begin(); itw != map_Region_to_Weight.end(); itw++){
	  double _weight = itw->second;
	  TString region= it->first + "_"+itw->first;
	  
	  //double pt1bins[12] = { 20., 25., 30., 35., 40., 50., 70., 100.,200.,500., 1000., 2000.};
	  //double pt2bins[6] = {10., 15., 20., 40., 100., 2000.};

	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/LLMass",  llCand.M() , _weight, 40, 0., 200.);
	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/NJets",   JetColl.size() , _weight, 10, 0., 10.);
	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/NBJets",   BJetColl.size() , _weight, 5, 0., 5.);
	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/nPV",  nPV , _weight, 120, 0., 120.);
	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/nPileUp",  nPileUp , _weight, 120, 0., 120.);

	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/Lep1Pt", LepsT[0]->Pt()  , _weight, 100, 0., 500.);
	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/Lep2Pt", LepsT[1]->Pt()  , _weight, 50, 0., 250. );
	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/PuppiMETType1XY",  PuppiMETv.Pt() , _weight, 100, 0., 400.);
	  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/PuppiMETPhiType1XY",  PuppiMETv.Phi() , _weight, 10, -3.2, 3.2);
	  
	  for(auto imet : MapOfMET)  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/METPt_"+imet.first,  imet.second.Pt() , _weight, 100, 0., 400.);
	  for(auto imet : MapOfMET)  FillHist( param_channel.Name+"/RegionPlots_"+ region+ "/METPhi_"+imet.first,  imet.second.Phi() , _weight, 10, -3.2, 3.2);


	}
      }
    }
        
  }
  return;
}
 


HNL_Validation::HNL_Validation(){


}
 
HNL_Validation::~HNL_Validation(){

}




