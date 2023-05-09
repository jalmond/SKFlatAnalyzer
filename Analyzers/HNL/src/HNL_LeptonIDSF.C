#include "HNL_LeptonIDSF.h"

void HNL_LeptonIDSF::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();

  SetupIDMVAReaderDefault();

}


void HNL_LeptonIDSF::executeEvent(){
  
  if((_jentry==0)){ 
    // Print out trigger info in HNL_LeptonCore::initializeAnalyzer
    TriggerPrintOut(GetEvent());
  }
  
  if(!IsData)  gens = GetGens();
  AnalyzerParameter param =  HNL_LeptonCore::InitialiseHNLParameter("MVAUL","");
  MeasureElectronIDSF(param);
  MeasureMuonIDSF(param);
  return ;
}



void HNL_LeptonIDSF::MeasureMuonIDSF(AnalyzerParameter param){

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  HNL_LeptonCore::Channel dilep_channel = MuMu;
  std::vector<Muon>       MuonCollProbe     = GetMuons    ("passProbe",    10., 2.4);

  /// Event cuts                                                                                                                                                                                                                       

  std::vector<Electron>   ElectronCollV = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
  if(ElectronCollV.size() != 0) return;                                                                                                                                                                                                

  std::vector<Jet> BJetColl  = GetHNLJets("BJetM", param);
  Particle METv              = GetvMET("PuppiT1xyCorr"); // reyturns MET with systematic correction                                                                                                                                                                                                                                                                                                                                                                                      
  if(BJetColl.size() > 0 ) return;
  if(!PassMETFilter()) return;

  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(MuonCollProbe);

  // Chose Typ1 Phi corr MET + smear jets 
  if(!CheckLeptonFlavourForChannel(dilep_channel, LeptonColl)) return;
  if (!PassTriggerSelection(dilep_channel, ev, LeptonColl,"Dilep")) return;

  // Reuqire 2 Probe muons in Z mass
  if(MuonCollProbe.size() != 2) return;
  if(fabs(GetLLMass(MuonCollProbe)- 90.1) > 10) return;

  vector<TString> IDs = {
    "MVALoose",
    "MVALooseCut1",
    "MVALooseCut2",
    "MVALooseCut3",
    "MVALooseCut4",
    "MVAID",
    "POGLoose",
    "POGMedium",    
    "HNL_ULID_FAKET_BDTG",
    "HNL_ULID_FAKEM_BDTG",
    "HNL_ULID_FAKEL_BDTG",
    "HNL_ULID_FAKEVL_BDTG",
    "HNL_ULID_FAKEVVL_BDTG",
    "HNL_ULID_TopFAKET_BDTG",
    "HNL_ULID_TopFAKEM_BDTG",
    "HNL_ULID_TopFAKEL_BDTG",
    "HNL_ULID_TopFAKEVL_BDTG",
    "HNL_ULID_TopFAKEVVL_BDTG",    
    "HNTightV2"
  };

  /// Make cuts to remove fakes
  if(METv.Pt() > 40) return;

  if(MuonCollProbe[1].Pt() < 20) {
    if(MT(MuonCollProbe[1], METv) > 30) return;
  }
  
  for(auto id : IDs) MeasureIDSF(param,dilep_channel, MuonCollProbe, id, weight);
  
  return;
}



void HNL_LeptonIDSF::MeasureElectronIDSF(AnalyzerParameter param){

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  HNL_LeptonCore::Channel dilep_channel= EE;

  /// Event cuts
  std::vector<Jet> BJetColl  = GetHNLJets("BJetM", param);
  Particle METv = GetvMET("PuppiT1xyCorr"); // reyturns MET with systematic correction                                                                                                                            

  if(BJetColl.size() > 0 ) return;
  if(!PassMETFilter()) return;

  /// Obj cuts
  std::vector<Muon>       MuonCollV     = GetMuons    (param.Muon_Veto_ID, 5., 2.4);
  if(MuonCollV.size() != 0) return;

  std::vector<Electron> ProbeEl = GetElectrons( "passProbeID", 10., 2.5);
  if(ProbeEl.size() != 2) return;
  if(fabs(GetLLMass(ProbeEl)- 90.1) > 10) return;

  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(ProbeEl);
  if(!PassTriggerSelection(dilep_channel, ev, LeptonColl,"Dilep")) return;
  if(!CheckLeptonFlavourForChannel(dilep_channel, LeptonColl)) return; // Check if EE cahnnel has 2 el...                                                                                                         

  vector<TString> IDWP = {"VT","T","M","L","VL"}; 
  
  vector<TString> IDs;
  for(auto i : IDWP){
    IDs.push_back("HNL_ULIDs_CF"+i+"_ED_BDTGv2");
    IDs.push_back("HNL_ULIDs_CF"+i+"_ED_BDTGv2p1");
    IDs.push_back("HNL_ULIDs_CF"+i+"_ED_BDTGv2p2");
    IDs.push_back("HNL_ULIDs_CF"+i+"_BDTGv1");
    IDs.push_back("HNL_ULIDs_CF"+i+"_BDTGv2");
    IDs.push_back("HNL_ULIDs_CF"+i+"_BDTGv2p1");
    IDs.push_back("HNL_ULIDs_CF"+i+"_BDTGv2p2");
    IDs.push_back("HNL_ULIDs_FAKE"+i+"_BDTG_LF");
    IDs.push_back("HNL_ULIDs_FAKE"+i+"_BDTG_HF");
    IDs.push_back("HNL_ULIDs_FAKE"+i+"_BDTG_HFC");
    IDs.push_back("HNL_ULIDs_FAKE"+i+"_BDTG_HFB");
    IDs.push_back("HNL_ULIDs_FAKE"+i+"_BDTG_Top");
    IDs.push_back("HNL_ULIDs_FAKE"+i+"_BDTG");
  }

  IDs.push_back("HNL_ULIDs_CONVT_BDTG");
  IDs.push_back("HNL_ULIDs_CONVM_BDTG");
  IDs.push_back("HNL_ULIDs_CONVL_BDTG");
   
  IDs.push_back("HNTightV2");
  IDs.push_back("passProbeIDTight");
  IDs.push_back("MVALoose");

  if(METv.Pt() > 40) return;

  if(ProbeEl[1].Pt() < 20) {
    if(MT(ProbeEl[1], METv) > 30) return;
  }

  for(auto id : IDs)    MeasureIDSF(param ,dilep_channel, ProbeEl, id, weight);
  
  return;
  
}
     

void HNL_LeptonIDSF::MeasureIDSF(AnalyzerParameter param,HNL_LeptonCore::Channel dilep_channel, vector<Muon> MuonColl, TString ID, double weight_ll){

  TString channel_string = GetChannelString(dilep_channel) ;
       
  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(MuonColl);

  //if(!ZmassOSSFWindowCheck(LeptonColl, 10.)) return;

  if(SameCharge(MuonColl)) channel_string=channel_string+"_SS";
  else  channel_string=channel_string+"_OS";


  if(MuonColl[0].PassID("POGTightWithTightIso")) {

    double SF=1;
    if(!IsData){
      double lep1_pt  = MuonColl[0].MiniAODPt();
      double lep1_eta = MuonColl[0].Eta();
      double lep2_pt  = MuonColl[1].MiniAODPt();
      double lep2_eta = MuonColl[1].Eta();
      
      double lep1_idsf   = mcCorr->MuonID_SF ("NUM_TightID_DEN_TrackerMuons",  lep1_eta, lep1_pt,0);
      double lep1_isosf  = mcCorr->MuonISO_SF("NUM_TightRelIso_DEN_TightIDandIPCut", lep1_eta, lep1_pt,0);
      double lep1_recosf = mcCorr->MuonReco_SF("MuonRecoSF", lep1_eta, lep1_pt,0);
      double lep2_recosf = mcCorr->MuonReco_SF("MuonRecoSF", lep2_eta, lep2_pt,0);

      SF = lep1_idsf*lep1_isosf*lep1_recosf*lep2_recosf;
    }
    
    FilllHistBins(Lepton(MuonColl[1]), MuonColl[1].PassID(ID),  channel_string,ID, SF*weight_ll);
    

  }
  if(MuonColl[1].PassID("POGTightWithTightIso")) {

    double SF=1;
    if(!IsData){
      double lep1_pt  = MuonColl[1].MiniAODPt();
      double lep1_eta = MuonColl[1].Eta();
      double lep2_pt  = MuonColl[0].MiniAODPt();
      double lep2_eta = MuonColl[0].Eta();
      
      double lep1_idsf   = mcCorr->MuonID_SF ("NUM_TightID_DEN_TrackerMuons",  lep1_eta, lep1_pt,0);
      double lep1_isosf  = mcCorr->MuonISO_SF("NUM_TightRelIso_DEN_TightIDandIPCut", lep1_eta, lep1_pt,0);
      double lep1_recosf = mcCorr->MuonReco_SF("MuonRecoSF", lep1_eta, lep1_pt,0);
      double lep2_recosf = mcCorr->MuonReco_SF("MuonRecoSF", lep2_eta, lep2_pt,0);

      SF = lep1_idsf*lep1_isosf*lep1_recosf*lep2_recosf;
    }

    FilllHistBins(Lepton(MuonColl[0]), MuonColl[0].PassID(ID),  channel_string,ID, SF*weight_ll);

  }
  

  return;

}



void HNL_LeptonIDSF::MeasureIDSF(AnalyzerParameter param,HNL_LeptonCore::Channel dilep_channel, vector<Electron> ElectronColl, TString ID, double weight_ll){
  
  TString channel_string = GetChannelString(dilep_channel) ;

  std::vector<Lepton *> LeptonColl      = MakeLeptonPointerVector(ElectronColl);

  //if(!ZmassOSSFWindowCheck(LeptonColl, 10.)) return;


  if(!IsData){
    if(!SameCharge(ElectronColl)) {
      if( IsCF(ElectronColl[0],gens) || IsCF(ElectronColl[1],gens)) return;
    }
  }

  if(SameCharge(ElectronColl)) channel_string=channel_string+"_SS";
  else  channel_string=channel_string+"_OS";
  
  /*for(unsigned int i=0; i < 2; i++){
    if(ElectronColl[i].PassID("HNTightV2") && ! ElectronColl[i].PassID("MVALoose")) {
      cout << "Pass Tight and not MVALoose" << endl;
      cout << "passMVAID_noiso_WPLoose = " << ElectronColl[i].passMVAID_noiso_WPLoose() << endl;
      cout << "Pass_TriggerEmulationLoose = " << ElectronColl[i].Pass_TriggerEmulationLoose() << endl;
      cout << "this->Pt()= " << ElectronColl[i].Pt() << endl;
      cout << "this->fEta " << ElectronColl[i].fEta() << endl;
      cout << "MiniRelIso() = " << ElectronColl[i].MiniRelIso() << endl;
      cout << "NMissingHits() " << ElectronColl[i].NMissingHits() << endl;
      cout << "SIP3D() " << ElectronColl[i].SIP3D() << endl;
      cout << "this->fdXY() " << ElectronColl[i].fdXY() << endl;
      cout << "this->fdZ() = " << ElectronColl[i].fdZ() << endl;
      cout << "passTightID_NoCC() = " << ElectronColl[i].passTightID_NoCC()  << endl;
      cout << "PassHNID() " << ElectronColl[i].PassHNID() << endl;
    }
    }*/

  if(ElectronColl[0].PassID("passPOGTight")) {

    double SF=1;
    if(!IsData){
      double this_recosf  = mcCorr->ElectronReco_SF(ElectronColl[0].scEta(),ElectronColl[0].Pt(), 0);
      double this_idsf    = mcCorr->ElectronID_SF("passTightID", ElectronColl[0].scEta(), ElectronColl[0].Pt(), 0);
      
      double lep2_recosf  = mcCorr->ElectronReco_SF(ElectronColl[1].scEta(),ElectronColl[1].Pt(), 0);
      SF = this_recosf*this_idsf*lep2_recosf;
    }


    FilllHistBins(Lepton(ElectronColl[1]), ElectronColl[1].PassID(ID),  channel_string,ID, SF*weight_ll);

  }
  if(ElectronColl[1].PassID("passPOGTight")) {

    double SF=1;
    if(!IsData){
      double this_recosf  = mcCorr->ElectronReco_SF(ElectronColl[1].scEta(),ElectronColl[1].Pt(), 0);
      double this_idsf    = mcCorr->ElectronID_SF("passTightID", ElectronColl[1].scEta(), ElectronColl[1].Pt(), 0);
      
      double lep2_recosf  = mcCorr->ElectronReco_SF(ElectronColl[0].scEta(),ElectronColl[0].Pt(), 0);
      SF = this_recosf*this_idsf*lep2_recosf;
    }

    FilllHistBins(Lepton(ElectronColl[0]), ElectronColl[0].PassID(ID),  channel_string,ID, SF*weight_ll);

  }

  return;

}


HNL_LeptonIDSF::HNL_LeptonIDSF(){


}
 
HNL_LeptonIDSF::~HNL_LeptonIDSF(){

}



void HNL_LeptonIDSF::FilllHistBins(Lepton lep, bool passID,  TString Channel_string,TString _ID, double lep_weight){
  
  int nbin_pt    =10;
  int nbin_eta   =4;
  double ptbins    [nbin_pt    +1] = { 10.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.};
  double etabins   [nbin_eta+1   ] = { 0.,0.8,  1.479, 2.,  2.5};
  
  int lepType = (IsData) ? 1 : GetLeptonType_JH(lep, gens);
  TString TypeLable="";
  if(lepType < 0 ) TypeLable="_NonPrompt";

  TString Den_tag=TypeLable+"_denom";

  double PtLep = (lep.Pt() > 200) ? 199 : lep.Pt();

  FillHist(Channel_string+"/Pt_Eta_"+_ID+Den_tag,lep.Pt(), fabs(lep.Eta()),  lep_weight, nbin_pt, ptbins, nbin_eta , etabins);
  FillHist(Channel_string+"/Pt_"+_ID+Den_tag,  PtLep,   lep_weight, 100, 0, 200);
  FillHist(Channel_string+"/Ptbinned_"+lep.etaRegionString()+"_"+_ID+Den_tag,PtLep,   lep_weight, nbin_pt, ptbins,"");
  FillHist(Channel_string+"/Pt_"+lep.etaRegionString()+"_"+_ID+Den_tag,PtLep,         lep_weight, 100, 0, 200.);
  FillHist(Channel_string+"/Eta"+_ID+Den_tag,lep.Eta(),   lep_weight, 60., -3, 3.);
  
  if(passID){
    TString Num_tag=TypeLable+"_num";
    FillHist(Channel_string+"/Pt_Eta_"+_ID+Num_tag,PtLep, fabs(lep.Eta()),  lep_weight, nbin_pt, ptbins, nbin_eta , etabins);
    FillHist(Channel_string+"/Pt_"+    _ID+Num_tag,lep.Pt(),   lep_weight, 100, 0, 200.);
    FillHist(Channel_string+"/Pt_"+lep.etaRegionString()+"_"+_ID+Num_tag,PtLep,         lep_weight, 100,0, 200.);
    FillHist(Channel_string+"/Ptbinned_"+lep.etaRegionString()+"_"+_ID+Num_tag,PtLep,   lep_weight, nbin_pt, ptbins,"");
    FillHist(Channel_string+"/Eta"+_ID+Num_tag,lep.Eta(),   lep_weight, 60, -3, 3.);
  }

  return;
}

