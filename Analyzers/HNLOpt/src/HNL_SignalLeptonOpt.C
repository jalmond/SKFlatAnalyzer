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


  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNLOpt","_UL");
    
  //cout << "########################################################" << endl;
  bool opt_IDEl=RunEE;

  std::vector<Electron>   ElectronCollV = GetElectrons(param_signal.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param_signal.Muon_Veto_ID, 5., 2.4);
  std::vector<Lepton *>   LepsVeto      = MakeLeptonPointerVector(MuonCollV,ElectronCollV,param_signal);
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

    // 2017 
    /// BB HNLOpt_UL_ElOpt_HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_CFMVABB0p2_CFMVAMethodEDv2p1_DXYv1  5.6896150203
    //     HNLOpt_UL_ElOpt_HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_CFMVABB0p93_CFMVAMethodv2_DXYv1     5.68214929691
    //     HNLOpt_UL_ElOpt_HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_CFMVABB0p76_CFMVAMethodEDv2_DXYv1   5.68022917878

    //  EC  /HNLOpt_UL_ElOpt_HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_CFMVAEC0p91_CFMVAMethodEDv2_DXYv1/LimitBins/ElectronSR 5.47130259473
    //      
    if(HasFlag("ELID_NP_CF")){

    }
    
    
    /// V1 El
    if(HasFlag("ELID_CF")){
      
      vMVACFBB.clear();
      vMVACFEC.clear();
      
      if(HasFlag("BB")){

	int nMVACF=30;
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


	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodv2_DXYv1"); 
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodv2p1_DXYv1"); 
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodv2p2_DXYv1"); 
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodEDv2_DXYv1");
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodEDv2p1_DXYv1");
	for(auto i1 : vMVACFBB)   ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"CFMVAMethodEDv2p2_DXYv1");
      }
      if(HasFlag("EC")){

	int nMVACF=30;
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
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodv2_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodv2p1_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodv2p2_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodEDv2_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodEDv2p1_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"CFMVAMethodEDv2p2_DXYv1");
      }

      if(HasFlag("FullEta")){
	
	int nMVACF=15;
        for(int imva=0 ; imva < nMVACF ; imva++){
          double mva_d=  0.2 + double(imva)*.05;
          TString mvaTS= DoubleToString(mva_d);
          vMVACFBB.push_back("CFMVABB"+mvaTS+"_");
          vMVACFEC.push_back("CFMVAEC"+mvaTS+"_");
        }
	for(auto i1 : vMVACFBB) {
	  for(auto i2 : vMVACFEC) {
            ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i1+i2+"CFMVAMethodv2_DXYv1");
            ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i1+i2+"CFMVAMethodv2p1_DXYv1");
	    ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i1+i2+"CFMVAMethodEDv2_DXYv1");
	    ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i1+i2+"CFMVAMethodEDv2p1_DXYv1");
	  }
	}
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
    ev.SetMVA("EE",100, EvaluateEventMVA("100", "300","850", EE,  LepsVeto,ev, GetvMET("PuppiT1xyCorr") ,param_signal));
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


      RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetColl,ev);
      
      param_signal.Name = param_signal.DefName;
      param_signal.SRConfig  = "";
    }
  }

  

  Event ev = GetEvent();

  bool opt_IDMu=!RunEE;
  if(opt_IDMu){
    
    vector<TString> MuonsIDs;


    if(!SameCharge(MuonCollV)) return;

    if(ElectronCollV.size() > 0) return;


    std::vector<FatJet> FatjetColl                  = GetHNLAK8Jets("HNL",param_signal);
    std::vector<FatJet> FatjetPNColl                = GetHNLAK8Jets("HNL_PNL",param_signal);
    std::vector<FatJet> FatjetNoMassColl            = GetHNLAK8Jets("HNLNoMass",param_signal);
    std::vector<Jet> AllJetColl                     = GetHNLJets("NoCut_Eta3",param_signal);
    std::vector<Jet> JetColl                        = GetHNLJets("Tight",param_signal);
    std::vector<Jet> JetCollLoose                   = GetHNLJets("Loose",param_signal);
    std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param_signal);
    std::vector<Jet> BJetColl                       = GetHNLJets("BJetM",param_signal);
    std::vector<Jet> BJetCollSR1                    = GetHNLJets("BJetT",param_signal);
    std::vector<Jet> BJetColl2                       = GetHNLJets("BJetMPtL",param_signal);
    std::vector<Jet> BJetColl2SR1                    = GetHNLJets("BJetTPtL",param_signal);
    std::vector<Jet> BJetColl3                       = GetHNLJets("BJetMPtM",param_signal);
    std::vector<Jet> BJetColl3SR1                    = GetHNLJets("BJetTPtM",param_signal);


    
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
    
    if(HasFlag("MuID_NP_HF")){

      MuonsIDs.push_back("HNLUL_POGBT_ISOB0p15_POGECT_ISOEC0p15_DXYv1");

      if(HasFlag("FullEta")){
        vector<TString> mvaHF1;
	for(unsigned int imva=0 ; imva < 21 ; imva++){
          double mva_d= 0.4 + double(imva)*0.02;
          TString mvaTS= DoubleToString(mva_d);

	  for(unsigned int imva2=0 ; imva2 < 21 ; imva2++){
	    double mva2_d= 0.6 + double(imva2)*0.01;
	    TString mva2TS= DoubleToString(mva2_d);
	    mvaHF1.push_back("MVAHFBB"+mvaTS+"_MVAHFEC"+mva2TS+"_");
	  }
	}
      
        for(auto i1 : mvaHF1){ 
          MuonsIDs.push_back("HNLUL_"+i1+"DXYv8");
	}
      }
    }



    if(HasFlag("MuID_NP")){
      
      MuonsIDs.push_back("HNLUL_POGBT_ISOB0p15_POGECT_ISOEC0p15_DXYv1");
      
      if(HasFlag("FullEta")){
	
	if(HasFlag("HF_Bin1")){
	  
	  vector<TString> mvaHF1;
	  vector<TString> mvaHF2;
	  for(unsigned int imva=0 ; imva < 5 ; imva++){
	    double mva_d= -1 + double(imva)*0.25;
            TString mvaTS= DoubleToString(mva_d);
            mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
            mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
	  }
	  
	  for(unsigned int imva=0 ; imva < 9 ; imva++){
	    double mva_d= 0.1 + double(imva)*0.1;
	    TString mvaTS= DoubleToString(mva_d);
	    mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
            mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
	  }

	  for(auto i1 : mvaHF1){
	    for(auto i2 : mvaHF2){
	      MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv1");
	    }
	  }
	}// Bin1
	if(HasFlag("HF_Bin2")){

          vector<TString> mvaHF1;
          vector<TString> mvaHF2;

          for(unsigned int imva=0 ; imva < 11 ; imva++){
            double mva_d= 0.4 + double(imva)*0.02;
            TString mvaTS= DoubleToString(mva_d);
            mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
            mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
          }
	  for(unsigned int imva=0 ; imva < 20 ; imva++){
            double mva_d= 0.61 + double(imva)*0.01;
            TString mvaTS= DoubleToString(mva_d);
            mvaHF1.push_back("MVAHFBB"+mvaTS+"_");
            mvaHF2.push_back("MVAHFEC"+mvaTS+"_");
          }

          for(auto i1 : mvaHF1){
            for(auto i2 : mvaHF2){
              MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv8");
            }
          }
        }// Bin2                                                                                                                                                            
        if(HasFlag("HF_Bin3")){

          vector<TString> mvaHF;
	  if(DataYear == 2016) mvaHF.push_back("MVAHFBB0p75_MVAHFEC0p72_");
	  if(DataYear == 2017) mvaHF.push_back("MVAHFBB0p68_MVAHFEC0p63_");
	  if(DataYear == 2018) mvaHF.push_back("MVAHFBB0p68_MVAHFEC0p64_");
          vector<TString> vdxy = {"DXY0p005_","DXY0p01_","DXY0p02_","DXY0p05_","DXY0p1_"};
          vector<TString> vdz  = {"DZ0p02_","DZ0p04_","DZ0p05_","DZ0p1_"}; 
          vector<TString> vsip ;

	  for(unsigned int ip=0 ; ip < 8 ; ip++){
            double ip_d= 3. + double(ip)*1;
            TString ipTS= DoubleToString(ip_d);
            vsip.push_back("SIP"+ipTS+"_");
          }

          for(auto i1 : mvaHF){
            for(auto i2 : vdxy){
	      for(auto i3 : vdz){
		for(auto i4 : vsip){
		  MuonsIDs.push_back("HNLUL_"+i1+i2+i3+i4);
		}
	      }
	    }
          }

        }// Bin3                                                                                                                                            
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
    
    MuonsIDs.push_back("HNL_ULID_2016");
    MuonsIDs.push_back("HNL_ULID_2017");
    MuonsIDs.push_back("HNL_ULID_2018");
    MuonsIDs.push_back("HNL_ULIDv2");
    MuonsIDs.push_back("HNL_ULIDv3");
    MuonsIDs.push_back("HNL_ULIDv4");
    MuonsIDs.push_back("HNTight_17028");
    MuonsIDs.push_back("HNTightV2");
    MuonsIDs.push_back("HNL_Peking");
    MuonsIDs.push_back("HNL_HN3L");
    
    bool SROpt=false;
    if(HasFlag("Mu_SelectionOptSR1")){
      SROpt=true;
      MuonsIDs.clear();
      MuonsIDs.push_back("HNTightV2");
    }
    if(HasFlag("Mu_SelectionOptSR2")){
      SROpt=true;
      MuonsIDs.clear();
      MuonsIDs.push_back("HNTightV2");
    }
    if(HasFlag("Mu_SelectionOptSR3")){
      SROpt=true;
      MuonsIDs.clear();
      MuonsIDs.push_back("HNTightV2");
    }



    for (unsigned int i=0; i<MuonsIDs.size(); i++) {
      for (unsigned int j=0; j<MuonsIDs.size(); j++) {
	if(i != j && MuonsIDs[i] == MuonsIDs[j]) {
	  cout << "[Muon::ID]  : " << MuonsIDs[i] << " occurs twice" << endl;

	  exit(ENODATA);

	}
      }
    }

    
    /// Caulcate BDT for MM events
    
    

    ev.SetMVA("MuMu",100, EvaluateEventMVA("100", "300","850", MuMu,  LepsVeto,ev, GetvMET("PuppiT1xyCorr"),param_signal));
    ev.SetMVA("MuMu",400, EvaluateEventMVA("400", "300","850", MuMu,  LepsVeto,ev, GetvMET("PuppiT1xyCorr"),param_signal));
    ev.SetMVA("MuMu",500, EvaluateEventMVA("500", "300","850", MuMu,  LepsVeto,ev, GetvMET("PuppiT1xyCorr"),param_signal));
    
    //cout << "Number of MuonsIDs = " << MuonsIDs.size() << endl;
    
    for(auto id : MuonsIDs){

      if(id.Contains("HNLUL")){
        param_signal.Name = param_signal.DefName  + "_MuOpt_"+ id;
        param_signal.SRConfig  = "";
        param_signal.Muon_Tight_ID="MuOpt_"+id;
        param_signal.Muon_FR_ID = "MuOptLoose_"+id;
        param_signal.Electron_Tight_ID = "HNTightV2";
        param_signal.Electron_FR_ID = "HNLooseV4";
      }
      else{
        param_signal.Name = param_signal.DefName  + id;
        param_signal.SRConfig  = "";
        param_signal.Muon_Tight_ID=id;
        param_signal.Muon_FR_ID = id;
        param_signal.Electron_Tight_ID = "HNTightV2";
        param_signal.Electron_FR_ID = "HNLooseV4";
	
      }

      if(!SROpt ){
	RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
	continue;
      }
      
      if(HasFlag("Mu_SelectionOptSR3")){
	param_signal.SRConfig  = "OptSR_SigR1_v1_SigR2_v1_SigR3_v1";
        param_signal.Name = param_signal.DefName  + id + "_SigR1_v1_SigR2_v1_SigR3_v1";
        RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);

        vector<TString> Sig1Bin = {"SigR1_v3"};
        vector<TString> Sig2Bin = {"SigR2_v2"};
        vector<TString> Sig3Bin = {"SigR3_v1","SigR3_v2","SigR3_v3","SigR3_v4","SigR3_v5", "SigR3_v6","SigR3_MD_EXO17"};                                                                                                                                    
        for (auto i : Sig1Bin){
          for (auto j : Sig2Bin){
            for (auto k : Sig3Bin){
	      param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k;
	      param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k;
	      RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
	      if(k == "SigR3_v6"){
		vector<TString> SigR3MET = {"SR3MET_20", "SR3MET_18", "SR3MET_15","SR3MET_12","SR3MET_10","SR3MET_9","SR3MET_8","SR3MET_7","SR3MET_6"};
                for(auto imet : SigR3MET) {
                  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+imet;
                  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+imet;
                  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
                }
		
		vector<TString> SigR3PT1 = {"SR3_bin1pt_60","SR3_bin1pt_70","SR3_bin1pt_80","SR3_bin1pt_90","SR3_bin1pt_100","SR3_bin1pt_120","SR3_bin1pt_140","SR3_bin1pt_160"};
		for(auto ipt : SigR3PT1){
		  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+ipt;
                  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+ipt;
                  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		}
		vector<TString> SigR3PT2 = {"SR3_bin2pt_60","SR3_bin2pt_70","SR3_bin2pt_80","SR3_bin2pt_90","SR3_bin2pt_100","SR3_bin2pt_120","SR3_bin2pt_140","SR3_bin2pt_160"};
                for(auto ipt : SigR3PT2){
                  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+ipt;
                  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+ipt;
                  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
                }

		
		param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k;
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR3BjetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR3BjetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetCollSR1,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR3B2jetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl2,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR3B2jetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl2SR1,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR3B3jetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl3,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR3B3jetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl3SR1,BJetColl,ev);



	      }
            }
          }
        }
      }

      if(HasFlag("Mu_SelectionOptSR2")){

	param_signal.SRConfig  = "OptSR_SigR1_v1_SigR2_v1_SigR3_v1";
	param_signal.Name = param_signal.DefName  + id + "_SigR1_v1_SigR2_v1_SigR3_v1";
        RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);


        vector<TString> Sig1Bin = {"SigR1_v3"};
        vector<TString> Sig2Bin = {"SigR2_v1","SigR2_v2"};
        vector<TString> Sig3Bin = {"SigR3_v6"};
        for (auto i : Sig1Bin){
          for (auto j : Sig2Bin){
            for (auto k : Sig3Bin){
              param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k;
              param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k;
              RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);


	      if(j == "SigR2_v2"){

		param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_LooseJet";
		param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_LooseJet";
		RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		
		param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_TightJet";
		param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_TightJet";
		RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		
		
		vector<TString> SigR2Jet = {"VBFLeadJet_MJJ0p45","VBFLeadJet_MJJ0p5","VBFLeadJet_MJJ0p6","VBFLeadJet_MJJ0p75",
					    "VBFLeadJet_MJJ0p8","VBFLeadJet_MJJ0p85","VBFLeadJet_MJJ0p9", "VBFLeadJet_MJJ1", 
					    "VBFAwayJet_MJJ0p45","VBFAwayJet_MJJ0p5","VBFAwayJet_MJJ0p6","VBFAwayJet_MJJ0p75",
					    "VBFAwayJet_MJJ0p8","VBFAwayJet_MJJ0p85","VBFAwayJet_MJJ0p9", "VBFAwayJet_MJJ1"};

		for(auto ijet : SigR2Jet) {
		  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+ijet;
		  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+ijet;
		  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		}
		
                vector<TString> SigR2MET = {"SR2MET_20", "SR2MET_18","SR2MET_15","SR2MET_12","SR2MET_10","SR2MET_9","SR2MET_8","SR2MET_7","SR2MET_6"};
                for(auto imet : SigR2MET) {
                  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+imet;
                  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+imet;
                  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
                }
		
		vector<TString> SigR2HTLT = {"HTLT_10", "HTLT_15","HTLT_20","HTLT_25","HTLT_30","HTLT_50"};
		for(auto iht  : SigR2HTLT) {
		  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+iht;
                  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+iht;
                  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		}
		

                param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k;
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR2BjetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR2BjetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetCollSR1,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR2B2jetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl2,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR2B2jetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl2SR1,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR2B3jetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl3,BJetColl,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR2B3jetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl3SR1,BJetColl,ev);


	      }
	    }
          }
        }
      }

    
      if(HasFlag("Mu_SelectionOptSR1")){
	
	param_signal.SRConfig  = "OptSR_SigR1_v1_SigR2_v1_SigR3_v1";
	param_signal.Name = param_signal.DefName  + id + "_SigR1_v1_SigR2_v1_SigR3_v1";
	RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);


	vector<TString> Sig1Bin = {"SigR1_v1","SigR1_v2","SigR1_v3", "SigR1_MD_v1","SigR1_MD_EXO17"};
	vector<TString> Sig2Bin = {"SigR2_v2"};
	vector<TString> Sig3Bin = {"SigR3_v6"};
	
	for (auto i : Sig1Bin){
	  for (auto j : Sig2Bin){
	    for (auto k : Sig3Bin){
	      param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k;
	      param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_NoParticleNet";
	      RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetColl,ev);
	      
	      
	      param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k;
	      RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
	      
	      if(i == "SigR1_v2"){
		vector<TString> SigR1MET = {"SR1MET_20","SR1MET_18", "SR1MET_15","SR1MET_12","SR1MET_10", "SR1MET_9", "SR1MET_8","SR1MET_7","SR1MET_6"};
		for(auto imet : SigR1MET) {
		  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+imet;
		  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+imet;
		  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		}
		
		vector<TString> SigR1MW = {"SR1MLLJJ_0p3","SR1MLLJJ_0p4","SR1MLLJJ_0p45","SR1MLLJJ_0p5","SR1MLLJJ_0p6","SR1MLLJJ_0p7",};
		for(auto im : SigR1MW) {
                  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+im;
   		  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+im;
		  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		} 
		
                param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k;
		param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR1BjetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR1BjetT";
		RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetCollSR1,ev);
		param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR1B2jetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl2,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR1B2jetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl2SR1,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR1B3jetM";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl3,ev);
                param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_SR1B3jetT";
                RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl3SR1,ev);

		
		vector<TString> SigR1LPt = {"SR1PT_80","SR1PT_90","SR1PT_100","SR1PT_110","SR1PT_120","SR1PT_140","SR1PT_160","SR1PT_200"};
		for(auto ip : SigR1LPt) {
                  param_signal.SRConfig  = "OptSR_"+i+"_"+j+"_"+k+"_"+ip;
		  param_signal.Name = param_signal.DefName  + id + "_" + i+"_"+j+"_"+k+"_"+ip;
		  RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetPNColl,BJetColl,BJetColl,ev);
		}
	      }
	    }
	  }
	}
      }

      //vector<TString> ZMassBin = {"PreSel_MZ_0_","PreSel_MZ_5_","PreSel_MZ_10_", "PreSel_MZ_15_","PreSel_MZ_20" };
      //for (auto i : ZMassBin){//
      //	param_signal.SRConfig  = i;//
      //param_signal.Name = param_signal.DefName + i;
      //	RunULAnalysis(param_signal,ElectronCollV,MuonCollV,TauColl,AllJetColl,JetCollLoose,JetColl,VBF_JetColl,FatjetColl,BJetColl,BJetColl,ev);
      //}
    }
    
    }
  
  return ;
}

void HNL_SignalLeptonOpt::RunULAnalysis(AnalyzerParameter param, vector<Electron> ElectronCollV, vector<Muon> MuonCollV, std::vector<Tau> TauColl,  std::vector<Jet> AllJetColl ,  std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl,std::vector<Jet> B_JetColl,std::vector<Jet> B_JetCollSR1, Event ev){

  if(run_Debug) cout << "HNL_SignalLeptonOpt::executeEvent " << endl;

  double weight =SetupWeight(ev,param);

  if(param.Name == "HNLOpt_ULHNTightV2") OutCutFlow("CutFlow_Events_HNTight", 1);
  if(param.Name == "HNLOpt_ULHNTightV2") OutCutFlow("CutFlow_Events_HNTight_weighted", weight);
  
  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  JetTagging::Parameters param_jetsT = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets);

  double sf_btagM_NLV                 = GetBJetSF(param, B_JetColl,    param_jetsM);
  double sf_btagSR1_NLV               = GetBJetSF(param, B_JetCollSR1, param_jetsT);
  
  if(AK8_JetColl.size() > 0 ) {
    if(!IsData){
      weight  = weight*sf_btagSR1_NLV;
      if(!param.Name.Contains("NoParticleNet"))weight*= AK8_JetColl[0].GetTaggerSF(JetTagging::particleNet_WvsQCD, DataEra,0); 
    }
  }
  else{
    if(!IsData)weight = weight*sf_btagM_NLV;
  }

  //cout << "weight = " << weight << endl;
  if(MCSample.Contains("Type")){
    if(MCSample.Contains("SS")) weight *= 0.01;
    else weight *= 0.1;
  }
  
  /// MERGE WJet samples for more stats
  if(!param.SRConfig.Contains("OptSR")){
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
  }

  // HL ID

  TString el_ID =  param.Electron_Tight_ID ;
  TString mu_ID =  param.Muon_Tight_ID ;

  double Min_Muon_Pt     = 5.;
  double Min_Electron_Pt = 10.;


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

  std::vector<Muon>       MuonCollT     = GetLepCollByRunType    ( MuonCollTInit,param);
  std::vector<Electron>   ElectronCollT  =  GetLepCollByRunType   ( ElectronCollTInit,param);
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param); // returns MET with systematic correction                                                                                                                      

  if(param.Name.Contains("HNTightV2")) param.WriteOutVerbose=0; // Does not Fill Cutflow OR Region Plotter  
  else  param.WriteOutVerbose=1;

  if(param.SRConfig.Contains("OptSR"))  param.WriteOutVerbose=1;

  if(param.Name== "HNLOpt_ULHNTightV2") OutCutFlow(param.Name+"_pre", weight);

  if(param.SRConfig.Contains("OptSR")) RunAllSignalRegionsOpt(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, AllJetColl,JetCollLoose, JetColl, VBF_JetColl, AK8_JetColl , B_JetColl,B_JetCollSR1, ev,METv, param,weight);
  else RunAllSignalRegions(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, AllJetColl,JetCollLoose, JetColl, VBF_JetColl,AK8_JetColl , B_JetColl,B_JetCollSR1, ev,METv, param,weight);
  
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




