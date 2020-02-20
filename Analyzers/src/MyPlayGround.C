#include "MyPlayGround.h"

void MyPlayGround::initializeAnalyzer(){

  genFinderDY = new GenFinderForDY();

}

void MyPlayGround::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

std::vector<Lepton *> MyPlayGround::TESTFunction(std::vector<Muon>& muons){

  std::vector<Lepton *> out;
  for(unsigned int i=0; i<1; i++){
    //cout << "[MyPlayGround::TESTFunction] &(muons.at(i)) = " << &(muons.at(i))<< endl;
    Lepton *l = &(muons.at(i));
/*
    Muon *muon = &(muons.at(i));
    Lepton *l = muon;
*/
    out.push_back(l);
  }
  return out;

}

void MyPlayGround::executeEventFromParameter(AnalyzerParameter param){

  vector<Gen> gens = GetGens();


  if(HasFlag("Signal")){

    int genNpid = -1;
    int SignalLeptonChannel = -1;
    for(unsigned int i=2; i<gens.size(); i++){
      int pid = abs( gens.at(i).PID() );
      if( pid==9900012 || pid==9900014 ){
        genNpid = pid;
        FillHist("GEN_N_Eta", gens.at(i).Eta(), 1., 60, -3., 3.);
        FillHist("GEN_N_Pt", gens.at(i).Pt(), 1., 800, 0., 8000.);
        break;
      }
    }
    //==== 0 = electron
    //==== 1 = muon
    if(genNpid==9900012){
      SignalLeptonChannel = 0;
    }
    else if(genNpid==9900014){
      SignalLeptonChannel = 1;
    }
    else{
    }

    if(SignalLeptonChannel!=0) return;
    FillHist("GenEEEvent", 1., 0., 1, 0., 1.);

  }
  else{

    FillHist("GenEEEvent", 1., 0., 1, 0., 1.);

  }

  vector<TString> IDs = {
"NOCUT",
"passVetoID",
"passLooseID",
"passMediumID",
"passTightID",
"passHEEPID",
  };

  for(unsigned int it_ID=0; it_ID<IDs.size(); it_ID++){

    TString ID = IDs.at(it_ID);
    vector<Electron> testels = GetElectrons(ID,53.,2.4);
    if(testels.size()==2){
      FillHist(ID+"_TwoElectrons", 1., 0., 1, 0., 1.);



    }

  }

  return;


/*
  vector<Gen> gens = GetGens();
  Particle GenZ = genFinderDY->Find(gens);
  FillHist("Unweighted_GenZ_Mass" , GenZ.M(), 1., 5000, 0., 5000.);
  FillHist("Unweighted_GenZ_Pt" , GenZ.Pt(), 1., 3000, 0., 3000.);

  Event ev = GetEvent();
  double weight = weight_norm_1invpb*ev.GetTriggerLumi("Full")*ev.MCweight();
  FillHist("Weighted_GenZ_Mass" , GenZ.M(), weight, 5000, 0., 5000.);
  FillHist("Weighted_GenZ_Pt" , GenZ.Pt(), weight, 3000, 0., 3000.);

  int MethodUsed = genFinderDY->MethodUsed;
  TString str_MethodUsed = TString::Itoa(MethodUsed,10);

  FillHist("Method_"+str_MethodUsed+"_Unweighted_GenZ_Mass" , GenZ.M(), 1., 5000, 0., 5000.);
  FillHist("Method_"+str_MethodUsed+"_Unweighted_GenZ_Pt" , GenZ.Pt(), 1., 3000, 0., 3000.);
  FillHist("Method_"+str_MethodUsed+"_Weighted_GenZ_Mass" , GenZ.M(), weight, 5000, 0., 5000.);
  FillHist("Method_"+str_MethodUsed+"_Weighted_GenZ_Pt" , GenZ.Pt(), weight, 3000, 0., 3000.);
*/

  vector<TString> triggers_Default = {
    "HLT_Mu50_v",
    "HLT_TkMu50_v",
    "HLT_Ele27_WPTight_Gsf_v",
    "HLT_Photon175_v",
    "HLT_DoublePhoton60_v",
  };
  vector<TString> triggers_DoubleLep = {
    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v",
    "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
    "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
    "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v",
    "HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v",
    "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v",
  };
  vector<TString> triggers_SingleLep = {
    "HLT_IsoMu24_v", "HLT_IsoTkMu24_v",
  };

  Event ev = GetEvent();
  bool Pass_Default = ev.PassTrigger(triggers_Default);
  bool Pass_DoubleLep = ev.PassTrigger(triggers_DoubleLep);
  bool Pass_SingleLep = ev.PassTrigger(triggers_SingleLep);

  if(Pass_Default){
    FillHist("LRSMHighPt", 0., 1., 1, 0., 1.);
  }
  if(Pass_Default||Pass_DoubleLep){
    FillHist("LRSMHighPt_PlusDouble", 0., 1., 1, 0., 1.);
  }
  if(Pass_Default||Pass_SingleLep){
    FillHist("LRSMHighPt_PlusSingle", 0., 1., 1, 0., 1.);
  }

  //==== Try Lepton Pt Skimming
  bool HasGoodLepton = false;
  double AnalysisMinLepPt = 40.;
  vector<Electron> els = GetAllElectrons();
  vector<Muon> mus = GetAllMuons();
  for(unsigned int i=0; i<els.size(); i++){
    if(els.at(i).Pt() >= AnalysisMinLepPt){
      HasGoodLepton = true;
      break;
    }
  }
  for(unsigned int i=0; i<mus.size(); i++){
    if(mus.at(i).Pt() >= AnalysisMinLepPt){
      HasGoodLepton = true;
      break;
    }
  }
  if(HasGoodLepton){
    if(Pass_Default){
      FillHist("LRSMHighPt_MinPt40", 0., 1., 1, 0., 1.);
    }
  }


}

MyPlayGround::MyPlayGround(){

}

MyPlayGround::~MyPlayGround(){

}


