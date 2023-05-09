#include "HNL_SignalLeptonOpt.h"

void HNL_SignalLeptonOpt::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  
  //SetupIDMVAReaderDefault(); /// Not needed for BDT skim
  
  if(HasFlag("RunEE")) SetupEventMVAReader(true,false,false); 
  else SetupEventMVAReader(false,true,false);

  RunHighPt= HasFlag("RunHighPt");
  RunEE = HasFlag("RunEE");
  
}


void HNL_SignalLeptonOpt::executeEvent(){

  OutCutFlow("CutFlow_All_Events", 1);

  if(!IsData)  gens = GetGens();

  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNLOpt","_UL");
    
  //cout << "########################################################" << endl;
  bool opt_IDEl=RunEE;

  std::vector<Electron>   ElectronCollV = GetElectrons(param_signal.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param_signal.Muon_Veto_ID, 5., 2.4);
  std::vector<Lepton *>   LepsVeto     = MakeLeptonPointerVector(MuonCollV,ElectronCollV,param_signal);
  std::vector<Tau>        TauColl       = GetTaus     (LepsVeto,param_signal.Tau_Veto_ID,20., 2.3);


  if(opt_IDEl){

    if(!SameCharge(ElectronCollV)) return;
    if(MuonCollV.size() > 0) return;
   
    std::vector<FatJet> FatjetColl                  = GetHNLAK8Jets("HNL",param_signal);
    std::vector<Jet> AllJetColl                     = GetHNLJets("NoCut_Eta3",param_signal);
    std::vector<Jet> JetColl                        = GetHNLJets("Tight",param_signal);
    std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param_signal);
    std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param_signal);
    std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param_signal);
    std::vector<Jet> BJetCollSR1                    = GetHNLJets("BJetT",param_signal);

    vector<TString> ElectronsIDs;

    vector<TString> vTrig = {"LooseTrig_"};//"LooseTrig_"};//,"TightTrig_"};
    vector<TString> vConv = {"ConvBConvEC_"};//,"ConvBConvEC_","ConvEC_"};
    vector<TString> vCC = {"CCBCCEC_","CCEC_"};//,"CCEC_",""};

    vector<TString> vMVACFBB, vMVACFEC;

    vector<TString> vMVAConvBB1,vMVAConvEC1;
    vector<TString> vMVAConvBB2, vMVAConvEC2;

    
    if(HasFlag("ELID_NP_CF")){

    }
    
    
    /// V1 El
    if(HasFlag("ELID_CF")){
      
      vMVACFBB.clear();
      vMVACFEC.clear();
      
      int nMVACF=31;
      
      for(int imva=0 ; imva < nMVACF ; imva++){
	double mva_d=  -1. + double(imva)*.05;
	TString mvaTS= DoubleToString(mva_d);
	vMVACFBB.push_back("CFMVABB"+mvaTS+"_");
	vMVACFEC.push_back("CFMVAEC"+mvaTS+"_");
      }
      for(int imva=1 ; imva < 45 ; imva++){
	double mva_d=  0.5 + double(imva)*.01;
	TString mvaTS= DoubleToString(mva_d);
	vMVACFBB.push_back("CFMVABB"+mvaTS+"_");
	vMVACFEC.push_back("CFMVAEC"+mvaTS+"_");
      }
	
      vector<TString> vMVACFBB2={};
      vector<TString> vMVACFEC2={};
      for(int imva=1 ; imva < 16 ; imva++){
	double mva_d=  0.6 + double(imva)*.02;
	TString mvaTS= DoubleToString(mva_d);
	vMVACFBB2.push_back("CFMVABB"+mvaTS+"_");
	vMVACFEC2.push_back("CFMVAEC"+mvaTS+"_");
      }
      
      if(HasFlag("BB")){
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodv2_DXYv1"); 
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodv2p1_DXYv1"); 
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodv2p2_DXYv1"); 
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodEDv2_DXYv1");
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodEDv2p1_DXYv1");
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodEDv2p2_DXYv1");
      }
      if(HasFlag("EC")){
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodv2_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodv2p1_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodv2p2_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodEDv2_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodEDv2p1_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodEDv2p2_DXYv1");
      }
    }
    
    if(HasFlag("ELID_Conv")){
      vector<TString> vMVAConvBB;
      vector<TString> vMVAConvEC;

      int nMVACV=20;
      for(int imva=0 ; imva < nMVACV ; imva++){
	double mva_d=  -1. + double(imva)*.05;
	TString mvaTS= DoubleToString(mva_d);
	vMVAConvBB.push_back("CVMVABB"+mvaTS+"_"+"CVMVAEC"+mvaTS+"_");
	//vMVAConvEC.push_back("CVMVAEC"+mvaTS+"_");
	
      }
      
      
      for(auto i1 : vMVAConvBB){
	///for(auto i2 : vMVAConvEC){
	ElectronsIDs.push_back("HNLUL_HNL2016_"+i1);
	ElectronsIDs.push_back("HNLUL_HNL2018_"+i1);
	//}
      }
    }


    //HNLOpt_UL_ElOpt_HNLUL_ConvBConvEC_LooseTrig_NPMVABB1neg1_NPMVAEB1neg1_NPMVAEC1neg1_CFMVABB0p7_CFMVAEC0p85_NPMVABB20p5_NPMVAEB20p5_NPMVAEC20p5__DXYv1
    
    //    for(int i1 = 0; i1 <  vMVANPBB2.size(); i1++){
    /*
    for(auto i2 : vMVACFBB) {
      for(auto i3 : vMVACFEC) {
	ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i2+i3+"DXYv1"); 
	ElectronsIDs.push_back("HNLUL_MVAWP90_ISOB0p1_ISOEC0p1_ConvBConvEC_LooseTrig_"+i2+i3+"DXYv1"); 
	ElectronsIDs.push_back("HNLUL_MVAWP80_ISOB0p1_ISOEC0p1_ConvBConvEC_LooseTrig_"+i2+i3+"DXYv1"); 
	ElectronsIDs.push_back("HNLUL_MVAWP90_ISOB0p1_ISOEC0p1_ConvBConvEC_LooseTrig_DXYv1");
	ElectronsIDs.push_back("HNLUL_MVAWP90_ISOB0p1_ISOEC0p1_ConvBConvEC_LooseTrig");
	ElectronsIDs.push_back("HNLUL_MVAWP90_ISOB0p1_ISOEC0p1_LooseTrig");
      }
      }*/

    vector<TString> vIsoB  = {"ISOB0p05","ISOB0p06","ISOB0p07","ISOB0p08","ISOB0p09","ISOB0p1","ISOB0p11","ISOB0p12","ISOB0p15"};
    vector<TString> vIsoEC = {"ISOEC0p05","ISOEC0p06","ISOEC0p07","ISOEC0p08","ISOEC0p09","ISOEC0p1","ISOEC0p11","ISOEC0p12","ISOEC0p15"};

    if(HasFlag("ELID_Iso")){
      
      for(unsigned int i = 0; i < vIsoB.size(); i++){
	//for(auto i6 : vIsoEC){ 
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2018");
	
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016_DXYv1");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2018_DXYv1");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016_DXYv2");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2018_DXYv2");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016_DXYv3");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2018_DXYv3");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016_DXYv4");
	ElectronsIDs.push_back("HNLUL_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2018_DXYv4");
	
	ElectronsIDs.push_back("HNLUL_MVABWP80MVAECWP90_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016");
	ElectronsIDs.push_back("HNLUL_MVABWP80MVAECWP90_"+vIsoB[i] + "_"+vIsoEC[i]);
	ElectronsIDs.push_back("HNLUL_MVABWP90MVAECWP80_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016");
	ElectronsIDs.push_back("HNLUL_MVAWP80_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016");
	ElectronsIDs.push_back("HNLUL_MVAWP80_"+vIsoB[i] + "_"+vIsoEC[i]);
	ElectronsIDs.push_back("HNLUL_MVAWP90_"+vIsoB[i] + "_"+vIsoEC[i]+"_HNL2016");
	ElectronsIDs.push_back("HNLUL_MVAWP90_"+vIsoB[i] + "_"+vIsoEC[i]);
	
      }
    }

    
    for(auto iTrig : vTrig){
      for(auto iConv : vConv){
	for(auto iCC : vCC){
	  ElectronsIDs.push_back("HNLUL_"+iTrig+iConv+iCC+"POGT_DXYv1");
	  ElectronsIDs.push_back("HNLUL_"+iTrig+iConv+iCC+"POGM_DXYv1");
	  ElectronsIDs.push_back("HNLUL_"+iTrig+iConv+iCC+"MVAWP90_ISOB0p1_ISOEC0p1_DXYv1"); 
	  ElectronsIDs.push_back("HNLUL_"+iTrig+iConv+iCC+"MVAWP80_ISOB0p1_ISOEC0p1_DXYv1"); 
	  ElectronsIDs.push_back("HNLUL_"+iTrig+iConv+iCC+"POGTNoIso_ISOB0p1_ISOEC0p1_DXYv1");
	  ElectronsIDs.push_back("HNLUL_"+iTrig+iConv+iCC+"POGMNoIso_ISOB0p1_ISOEC0p1_DXYv1");
	}
      }
    }

  
    ElectronsIDs.push_back("HNTight_17028");
    ElectronsIDs.push_back("HNTightV2");
    ElectronsIDs.push_back("HNL_ULID_2016");
    ElectronsIDs.push_back("HNL_ULID_2017");
    ElectronsIDs.push_back("HNL_ULID_2018");
    ElectronsIDs.push_back("HNL_Peking");
    ElectronsIDs.push_back("HNL_Peking_2016");

    ElectronsIDs.push_back("passPOGTight");
    ElectronsIDs.push_back("passPOGMedium");
    ElectronsIDs.push_back("passMediumID");
    ElectronsIDs.push_back("passTightID");
    ElectronsIDs.push_back("passHEEPID");
    ElectronsIDs.push_back("passMVAID_noIso_WP90Opt");
    ElectronsIDs.push_back("passMVAID_noIso_WP80Opt");
    ElectronsIDs.push_back("passMVAID_noIso_WP80");
    ElectronsIDs.push_back("passMVAID_Iso_WP80");
    ElectronsIDs.push_back("passMVAID_Iso_WP90");
    ElectronsIDs.push_back("HNHEEPID");
    ElectronsIDs.push_back("SUSYTight");
    ElectronsIDs.push_back("HN2016MVA");
    ElectronsIDs.push_back("HN2016MVA2");
    ElectronsIDs.push_back("HN2016POGCC");

    Event ev = GetEvent();
    ev.SetMVA("EE",100, EvaluateEventMVA("100", "300","850", EE,  LepsVeto,ev, GetvMET("PuppiT1xyCorr",param_signal) ,param_signal));
    //ev.SetMVA("EE",400, EvaluateEventMVA("400", "300","850", EE,  LepsVeto,ev, GetvMET("PuppiT1xyCorr",param_signal) ,param_signal));


    //cout << "ElectronsIDs size = " << ElectronsIDs.size() << endl;
    for(auto id : ElectronsIDs){

      //cout << id << endl;
      if (!id.Contains("HNLUL")){

        param_signal.Name = param_signal.DefName + "_"+ id;
        param_signal.SRConfig  = "";

        param_signal.Electron_Tight_ID=id;
        param_signal.Electron_FR_ID = id;
      }
      else{

        param_signal.Name = param_signal.DefName + "_ElOpt_"+ id;
        param_signal.SRConfig  = "";

        param_signal.Electron_Tight_ID="ElOpt_"+id;
        param_signal.Electron_FR_ID = "ElOptLoose_"+id;

      }
      param_signal.Muon_Tight_ID = "HNTightV2";
      param_signal.Muon_FR_ID = "HNLooseV1";


      RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetCollSR1,ev);
      
      param_signal.Name = param_signal.DefName;
      param_signal.SRConfig  = "";
    }
  }

  

  Event ev = GetEvent();

  bool opt_IDMu=!RunEE;
  if(opt_IDMu){
    
    vector<TString> MuonsIDs;

    FillHist( "TESTHIST/"+param_signal.Name+"/NEvents",  1,  1, 10, 0,10 );

    if(!SameCharge(MuonCollV)) return;
    FillHist( "TESTHIST/"+param_signal.Name+"/NEvents",  2,  1, 10, 0,10 );

    if(ElectronCollV.size() > 0) return;


    std::vector<FatJet> FatjetColl                  = GetHNLAK8Jets("HNL",param_signal);
    std::vector<Jet> AllJetColl                     = GetHNLJets("NoCut_Eta3",param_signal);
    std::vector<Jet> JetColl                        = GetHNLJets("Tight",param_signal);
    std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param_signal);
    std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param_signal);
    std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param_signal);
    std::vector<Jet> BJetCollSR1                    = GetHNLJets("BJetT",param_signal);
    
    if(MuonCollV[0].Pt() > 15 && MuonCollV[1].Pt() > 10 ) {
      if(BJetColl.size() == 0){
	if(MuonCollV[0].PassID("MVALoose") && MuonCollV[1].PassID("MVALoose")){

	  Fill_RegionPlots(MuMu, 0, param_signal.Name,"Preselection" , TauColl, JetColl, FatjetColl, LepsVeto, GetvMET("PuppiT1xyCorr",param_signal), nPV, SetupWeight(ev,param_signal),param_signal.WriteOutVerbose);
	  if(MuonCollV[0].PassID("HNL_HN3L") && MuonCollV[1].PassID("HNL_HN3L")) Fill_RegionPlots(MuMu, 0, param_signal.Name,"Preselection2" , TauColl, JetColl, FatjetColl, LepsVeto, GetvMET("PuppiT1xyCorr",param_signal), nPV, SetupWeight(ev,param_signal),param_signal.WriteOutVerbose);

	  if(MuonCollV[0].PassID("HNTightV2") && MuonCollV[1].PassID("HNTightV2")) Fill_RegionPlots(MuMu, 0, param_signal.Name,"Preselection3" , TauColl, JetColl, FatjetColl, LepsVeto, GetvMET("PuppiT1xyCorr",param_signal), nPV, SetupWeight(ev,param_signal),param_signal.WriteOutVerbose);

	  if(MuonCollV[0].PassID("HNL_ULIDv2") && MuonCollV[1].PassID("HNL_ULIDv2")) Fill_RegionPlots(MuMu, 0, param_signal.Name,"Preselection4" , TauColl, JetColl, FatjetColl, LepsVeto, GetvMET("PuppiT1xyCorr",param_signal), nPV, SetupWeight(ev,param_signal),param_signal.WriteOutVerbose);
	  if(MuonCollV[0].PassID("HNL_ULIDv3") && MuonCollV[1].PassID("HNL_ULIDv3")) Fill_RegionPlots(MuMu, 0, param_signal.Name,"Preselection5" , TauColl, JetColl, FatjetColl, LepsVeto, GetvMET("PuppiT1xyCorr",param_signal), nPV, SetupWeight(ev,param_signal),param_signal.WriteOutVerbose);
	  if(MuonCollV[0].PassID("HNL_ULIDv4") && MuonCollV[1].PassID("HNL_ULIDv4")) Fill_RegionPlots(MuMu, 0, param_signal.Name,"Preselection6" , TauColl, JetColl, FatjetColl, LepsVeto, GetvMET("PuppiT1xyCorr",param_signal), nPV, SetupWeight(ev,param_signal),param_signal.WriteOutVerbose);
	  
	  
	}      
      }
    }
    
    if(HasFlag("MuID_DXY")){
      MuonsIDs.clear();
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv1");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv2");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv3");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv4");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv5");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv6");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv7");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv8");
      MuonsIDs.push_back("HNLUL_HNLv1" + GetYearString()+"_"+"DXYv9");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv1");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv2");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv3");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv4");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv5");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv6");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv7");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv8");
      MuonsIDs.push_back("HNLUL_HNLv2" + GetYearString()+"_"+"DXYv9");

    }
    
    if(HasFlag("MuID_NP")){
      
      MuonsIDs.push_back("HNLUL_POGBT_ISOB0p15_POGECT_ISOEC0p15_DXYv1");
      
      if(HasFlag("FullEta")){
	
	vector<TString> mvaHF1;
	vector<TString> mvaHF2;
        for(unsigned int imva=0 ; imva < 20 ; imva++){
          double mva_d= 0.4 + double(imva)*0.02;
          TString mvaTS= DoubleToString(mva_d);
          mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
          mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
        }

        vector<TString> mvaLF1;
        vector<TString> mvaLF2;
	for(unsigned int imva=0 ; imva < 5 ; imva++){
          double mva_d= -0.5 + double(imva)*0.2;
          TString mvaTS= DoubleToString(mva_d);
	  mvaLF1.push_back("MVALFBB"+mvaTS+"_");
	  mvaLF2.push_back("MVALFEC"+mvaTS+"_");
        }

	for(auto i1 : mvaHF1){
	  for(auto i2 : mvaHF2){
	    MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv1");
	    for(auto i3 : mvaLF1){
	      for(auto i4 : mvaLF2){
		MuonsIDs.push_back("HNLUL_"+i1+i2+i3+i4+"NPMMv3_DXYv1");
	      }
	    }
	  }
	}
      }

      if(HasFlag("BB")){
	
	vector<TString> mvaHF;
	mvaHF.push_back("MVAHFBBneg1_");
	mvaHF.push_back("MVAHFBBneg0p8_");
	mvaHF.push_back("MVAHFBBneg0p4_");
	mvaHF.push_back("MVAHFBB0_");
	mvaHF.push_back("MVAHFBB0p2_");
	mvaHF.push_back("MVAHFBB0p4_");
	for(unsigned int imva=0 ; imva < 20 ; imva++){
	  double mva_d= 0. + double(imva)*0.02;
	  TString mvaTS= DoubleToString(mva_d);
	  mvaHF.push_back("MVAHFBB"+mvaTS+"_");
	}

	vector<TString> mvaLF;
	for(unsigned int imva=0 ; imva < 15 ; imva++){
          double mva_d= -1. + double(imva)*0.1;
          TString mvaTS= DoubleToString(mva_d);
          mvaLF.push_back("MVALFBB"+mvaTS+"_");
        }
	
	for(auto i1 : mvaHF){
	  MuonsIDs.push_back("HNLUL_POGECT_ISOEC0p15_"+i1+"DXYv1");
	  MuonsIDs.push_back("HNLUL_MVAHFEC0p64_"+i1+"DXYv1");
	  MuonsIDs.push_back("HNLUL_MVAHFEC0p64_"+i1);
	  for(auto i2 : mvaLF){
	    MuonsIDs.push_back("HNLUL_MVAHFEC0p64_"+i2+i1+"NPMMv3_DXYv1");
	  }
	}
      }
      if(HasFlag("EC")){
	
	vector<TString> mvaHF;
	mvaHF.push_back("MVAHFECneg1_");
	mvaHF.push_back("MVAHFECneg0p8_");
	mvaHF.push_back("MVAHFECneg0p6_");
	mvaHF.push_back("MVAHFECneg0p4_");
	mvaHF.push_back("MVAHFECneg0p2_");
	for(unsigned int imva=0 ; imva < 45 ; imva++){
	  double mva_d= 0. + double(imva)*0.02;
	  TString mvaTS= DoubleToString(mva_d);
	  mvaHF.push_back("MVAHFEC"+mvaTS+"_");
	}

	vector<TString> mvaLF;
        for(unsigned int imva=0 ; imva < 30 ; imva++){
          double mva_d= -1. + double(imva)*0.05;
          TString mvaTS= DoubleToString(mva_d);
          mvaLF.push_back("MVALFEC"+mvaTS+"_");
        }

        for(auto i1 : mvaHF){
          MuonsIDs.push_back("HNLUL_POGBT_ISOB0p15_"+i1+"DXYv1");
          MuonsIDs.push_back("HNLUL_MVAHFBB0p64_"+i1+"DXYv1");
          MuonsIDs.push_back("HNLUL_MVAHFBB0p64_"+i1);
          for(auto i2 : mvaLF){
            MuonsIDs.push_back("HNLUL_MVAHFBB0p64_"+i2+i1+"NPMMv3_DXYv1");
          }
        }
      }
    }
    
    
    //cout << " MuonsIDs = " <<  MuonsIDs.size() << endl;
    vector<TString> vIsoB  = {"ISOB0p05","ISOB0p06","ISOB0p07", "ISOB0p08","ISOB0p09","ISOB0p1","ISOB0p125","ISOB0p15"};
    vector<TString> vIsoEC = {"ISOEC0p05","ISOEC0p06","ISOEC0p07","ISOEC0p08","ISOEC0p09","ISOEC0p1","ISOEC0p125","ISOEC0p15"};

    if(HasFlag("Mu_Final")){
      MuonsIDs.clear();

      MuonsIDs.push_back("HNLUL_HNL" + GetYearString());
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv1");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv1");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_"+"DXYv1");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGECT_"+"DXYv1");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGECT_");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv2");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv3");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv4");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv5");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv6");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv7");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv8");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+"DXYv9");
      
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv2");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv3");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv4");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv5");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv6");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv7");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv8");
      MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+"DXYv9");
      
      for(unsigned int ib = 0 ; ib < vIsoB.size(); ib++){
	for(unsigned int ie = 0 ; ie < vIsoEC.size(); ie++){
	  TString iISOB = vIsoB[ib];
	  TString iISOEC = vIsoEC[ie];
	  
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+iISOB+"_"+iISOEC+"_DXYv1");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+iISOB+"_"+iISOEC+"_DXYv2");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+iISOB+"_"+iISOEC+"_DXYv3");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+iISOB+"_"+iISOEC+"_DXYv4");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+iISOB+"_"+iISOEC+"_DXYv5");
	  
	  
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+iISOB+"_"+iISOEC);
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv1");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv1");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv2");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv2");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv3");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv3");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv4");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv4");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBT_POGECT_"+iISOB+"_"+iISOEC+"_DXYv5");
	  MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_POGBM_POGECM_"+iISOB+"_"+iISOEC+"_DXYv5");
	}
      }
      
      
      

      MuonsIDs.push_back("HNTight_Iso05_dxy_01_ip_3"); ;
      MuonsIDs.push_back("HNTight_Iso05_dxy_01_ip_4");
      MuonsIDs.push_back("HNTight_Iso05_dxy_01_ip_999");
      MuonsIDs.push_back("HNTight_Iso05_dxy_02_ip_3");
      MuonsIDs.push_back("HNTight_Iso05_dxy_02_ip_4");
      MuonsIDs.push_back("HNTight_Iso05_dxy_02_ip_999");
      MuonsIDs.push_back("HNTight_Iso05_dxy_05_ip_3");
      MuonsIDs.push_back("HNTight_Iso05_dxy_05_ip_4");
      MuonsIDs.push_back("HNTight_Iso05_dxy_05_ip_999");
      
      
      MuonsIDs.push_back("HNTight_Iso06_dxy_01_ip_3");
      MuonsIDs.push_back("HNTight_Iso06_dxy_01_ip_4");
      MuonsIDs.push_back("HNTight_Iso06_dxy_01_ip_999");
      MuonsIDs.push_back("HNTight_Iso06_dxy_02_ip_3");
      MuonsIDs.push_back("HNTight_Iso06_dxy_02_ip_4");
      MuonsIDs.push_back("HNTight_Iso06_dxy_02_ip_999");
      MuonsIDs.push_back("HNTight_Iso06_dxy_05_ip_3");
      MuonsIDs.push_back("HNTight_Iso06_dxy_05_ip_4");
      MuonsIDs.push_back("HNTight_Iso06_dxy_05_ip_999");
      
      MuonsIDs.push_back("HNTight_Iso07_dxy_01_ip_3");
      MuonsIDs.push_back("HNTight_Iso07_dxy_01_ip_4");
      MuonsIDs.push_back("HNTight_Iso07_dxy_01_ip_999");
      MuonsIDs.push_back("HNTight_Iso07_dxy_02_ip_3");
      MuonsIDs.push_back("HNTight_Iso07_dxy_02_ip_4");
      MuonsIDs.push_back("HNTight_Iso07_dxy_02_ip_999");
      MuonsIDs.push_back("HNTight_Iso07_dxy_05_ip_3");
      MuonsIDs.push_back("HNTight_Iso07_dxy_05_ip_4");
      MuonsIDs.push_back("HNTight_Iso07_dxy_05_ip_999");
      
      MuonsIDs.push_back("HNTight_Iso08_dxy_01_ip_3");
      MuonsIDs.push_back("HNTight_Iso08_dxy_01_ip_4");
      MuonsIDs.push_back("HNTight_Iso08_dxy_01_ip_999");
      MuonsIDs.push_back("HNTight_Iso08_dxy_02_ip_3");
      MuonsIDs.push_back("HNTight_Iso08_dxy_02_ip_4");
      MuonsIDs.push_back("HNTight_Iso08_dxy_02_ip_999");
      MuonsIDs.push_back("HNTight_Iso08_dxy_05_ip_3");
      MuonsIDs.push_back("HNTight_Iso08_dxy_05_ip_4");
      MuonsIDs.push_back("HNTight_Iso08_dxy_05_ip_999");
      
      MuonsIDs.push_back("HNTight_Iso10_dxy_01_ip_3");
      MuonsIDs.push_back("HNTight_Iso10_dxy_01_ip_4");
      MuonsIDs.push_back("HNTight_Iso10_dxy_01_ip_999");
      MuonsIDs.push_back("HNTight_Iso10_dxy_02_ip_3");
      MuonsIDs.push_back("HNTight_Iso10_dxy_02_ip_4");
      MuonsIDs.push_back("HNTight_Iso10_dxy_02_ip_999");
      MuonsIDs.push_back("HNTight_Iso10_dxy_05_ip_3");
      MuonsIDs.push_back("HNTight_Iso10_dxy_05_ip_4");
      MuonsIDs.push_back("HNTight_Iso10_dxy_05_ip_999");
      
      
      MuonsIDs.push_back("HNTightPFIsoLoose");
      MuonsIDs.push_back("HNTightPFIsoMedium");
      MuonsIDs.push_back("HNTightPFIsoTight");
      MuonsIDs.push_back("HNTightPFIsoVeryTight");
      MuonsIDs.push_back("HNTightPFIsoVeryVeryTight");
      MuonsIDs.push_back("POGTightPFIsoLoose");
      MuonsIDs.push_back("POGTightPFIsoMedium");
      MuonsIDs.push_back("POGTightPFIsoTight");
      MuonsIDs.push_back("POGTightPFIsoVeryTight");
      MuonsIDs.push_back("POGTightPFIsoVeryVeryTight");
    }
    
    MuonsIDs.push_back("HNL_ULIDv1_2016");
    MuonsIDs.push_back("HNL_ULIDv1_2017");
    MuonsIDs.push_back("HNL_ULIDv1_2018");
    MuonsIDs.push_back("HNL_ULIDv2_2016");
    MuonsIDs.push_back("HNL_ULIDv2_2017");
    MuonsIDs.push_back("HNL_ULIDv2_2018");
    MuonsIDs.push_back("HNL_ULIDv2");
    MuonsIDs.push_back("HNL_ULIDv3");
    MuonsIDs.push_back("HNL_ULIDv4");
    MuonsIDs.push_back("HNTight_17028");
    MuonsIDs.push_back("HNTightV2");
    MuonsIDs.push_back("HNL_Peking");
    MuonsIDs.push_back("HNL_HN3L");


    for (unsigned int i=0; i<MuonsIDs.size(); i++) {
      for (unsigned int j=0; j<MuonsIDs.size(); j++) {
	if(i != j && MuonsIDs[i] == MuonsIDs[j]) {
	  cout << "[Muon::ID]  : " << MuonsIDs[i] << " occurs twice" << endl;

	  exit(ENODATA);

	}
      }
    }

    
    /// Caulcate BDT for MM events
    
    

    ev.SetMVA("MuMu",100, EvaluateEventMVA("100", "300","850", MuMu,  LepsVeto,ev, GetvMET("PuppiT1xyCorr",param_signal),param_signal));
    //    ev.SetMVA("MuMu",400, EvaluateEventMVA("400", "300","850", MuMu,  LepsVeto,ev, GetvMET("PuppiT1xyCorr",param_signal),param_signal));
    
    //cout << "Number of MuonsIDs = " << MuonsIDs.size() << endl;
    
    for(auto id : MuonsIDs){
      //      cout << "------------------------------------------------------------------------" << endl;
      //cout << "------------------------------------------------------------------------" << endl;
      //cout << "------------------------------------------------------------------------" << endl;
      //cout << "------------------------------------------------------------------------" << endl;
      //cout << id << endl;
      if(id.Contains("HNLUL")){
        param_signal.Name = param_signal.DefName  + "_MuOpt_"+ id;
        param_signal.SRConfig  = "";
        param_signal.Muon_Tight_ID="MuOpt_"+id;
        param_signal.Muon_FR_ID = "MuOptLoose_"+id;
        param_signal.Electron_Tight_ID = "HNTightV2";
        param_signal.Electron_FR_ID = "HNLooseV4";
	RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetCollSR1,ev);
      }
      else{
        param_signal.Name = param_signal.DefName  + id;
        param_signal.SRConfig  = "";
        param_signal.Muon_Tight_ID=id;
        param_signal.Muon_FR_ID = id;
        param_signal.Electron_Tight_ID = "HNTightV2";
        param_signal.Electron_FR_ID = "HNLooseV4";
	RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetCollSR1,ev);
      }
    }

  }
  
  return ;
}

void HNL_SignalLeptonOpt::RunULAnalysis(AnalyzerParameter param, vector<Electron> ElectronCollV, vector<Muon> MuonCollV, std::vector<Tau> TauColl,  std::vector<Jet> AllJetColl ,  std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl,std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1, Event ev){

  if(run_Debug) cout << "HNL_SignalLeptonOpt::executeEvent " << endl;

  double weight =SetupWeight(ev,param);

  if(param.Name.Contains("HNTightV2"))OutCutFlow("CutFlow_Events_HNTight", 1);
  if(param.Name.Contains("HNTightV2"))OutCutFlow("CutFlow_Events_HNTight_weighted", weight);
  
  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  JetTagging::Parameters param_jetsT = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets);

  double sf_btagM_NLV                 = GetBJetSF(param, B_JetColl,    param_jetsM);
  double sf_btagSR1_NLV               = GetBJetSF(param, B_JetCollSR1, param_jetsT);

  if(!IsData && AK8_JetColl.size()==0)weight = weight*sf_btagM_NLV;
  if(!IsData && AK8_JetColl.size()>0)weight = weight*sf_btagSR1_NLV;


  //cout << "weight = " << weight << endl;
  if(MCSample.Contains("Type")){
    if(MCSample.Contains("SS")) weight *= 0.01;
    else weight *= 0.1;
  }
  
  /// MERGE WJet samples for more stats
  if(MCSample.Contains("WJet")){
    vector<TString> vec = {"WJet"};
    double merge_weight = MergeMultiMC( vec, "" );
    weight*= merge_weight;
  }

  /// Merge DY samples for more stats
  if(MCSample.Contains("DYJets_MG")){
    vector<TString> vec = {"DYMG"};
    double merge_weight = MergeMultiMC( vec, "" );
    weight*= merge_weight;
  }
  
  // HL ID

  TString el_ID = (RunFake) ?  param.Electron_FR_ID : param.Electron_Tight_ID ;
  TString mu_ID = (RunFake) ?  param.Muon_FR_ID :  param.Muon_Tight_ID ;

  double Min_Muon_Pt     = (RunFake) ? 3. : 5.;
  double Min_Electron_Pt = (RunFake) ? 7. : 10.;


  std::vector<Muon>       MuonCollTInit; 
  std::vector<Electron>   ElectronCollTInit ;

  for(auto ilep : ElectronCollV) {
    if(ilep.Pt() < Min_Electron_Pt) continue;
    if(ilep.PassID(el_ID)) ElectronCollTInit.push_back(ilep);
  }

  for(auto ilep : MuonCollV) {
    if(ilep.Pt() < Min_Muon_Pt) continue;
    if(ilep.PassID(mu_ID)) MuonCollTInit.push_back(ilep);
  }

  std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( MuonCollTInit,gens,param);
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,gens,param);
  
  Particle METv = GetvMET("PuppiT1xyCorr",param); // returns MET with systematic correction                                                                                                                      

  FillHist( "TESTHIST/"+param.Name+"/NEvents",  5,  1, 10, 0,10 );
   

  param.WriteOutVerbose=1; // Does not Fill Cutflow OR Region Plotter  

  if(param.Name.Contains("HNTightV2"))OutCutFlow(param.Name+"_pre", weight);
  RunAllSignalRegions(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, AllJetColl,JetCollLoose, JetColl, VBF_JetColl,AK8_JetColl , B_JetColl,B_JetCollSR1, ev,METv, param,weight);

  return;
  

}



 


HNL_SignalLeptonOpt::HNL_SignalLeptonOpt(){

  TMVA::Tools::Instance();
  MVAReaderMM = new TMVA::Reader();
  MVAReaderEE = new TMVA::Reader();
  MVAReaderEM = new TMVA::Reader();

}
 
HNL_SignalLeptonOpt::~HNL_SignalLeptonOpt(){

}




