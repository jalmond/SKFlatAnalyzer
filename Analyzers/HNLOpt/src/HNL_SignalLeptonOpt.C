#include "HNL_SignalLeptonOpt.h"

void HNL_SignalLeptonOpt::initializeAnalyzer(){

  // All default settings like trigger/ PD/ BJet are decalred in HNL_LeptonCore::initializeAnalyzer to make them consistent for all HNL codes

  HNL_LeptonCore::initializeAnalyzer();
  cout << "SetupMVAReader " << endl;
  SetupMVAReader();


  RunHighPt= HasFlag("RunHighPt");
  RunEE = HasFlag("RunEE");
  
}


void HNL_SignalLeptonOpt::executeEvent(){
  
  
  if(!IsData)  gens = GetGens();

  AnalyzerParameter param_signal = HNL_LeptonCore::InitialiseHNLParameter("HNLOpt","_UL");
  
  
  //cout << "########################################################" << endl;
  bool opt_IDEl=RunEE;

  std::vector<Electron>   ElectronCollV = GetElectrons(param_signal.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon>       MuonCollV     = GetMuons    (param_signal.Muon_Veto_ID, 5., 2.4);
  if(opt_IDEl){

    if(!SameCharge(ElectronCollV)) return;
    if(MuonCollV.size() > 0) return;

    if(HasFlag("HighPt") && ElectronCollV[1].Pt() <20) return;

    
    vector<TString> ElectronsIDs;
 
    vector<TString> vTrig = {"LooseTrig_"};//"LooseTrig_"};//,"TightTrig_"};
    vector<TString> vConv = {"ConvBConvEC_"};//,"ConvBConvEC_","ConvEC_"};
    vector<TString> vCC = {"CCBCCEC_","CCEC_"};//,"CCEC_",""};

    vector<TString> vMVANPBB1,vMVANPEB1,vMVANPEC1;
    vector<TString> vMVANPBB2,vMVANPEB2,vMVANPEC2;

    vector<TString> vMVACFBB, vMVACFEC;

    vector<TString> vMVAConvBB1,vMVAConvEC1;
    vector<TString> vMVAConvBB2, vMVAConvEC2;

    
    if(HasFlag("ELID_NP_CF")){
      
      if(HasFlag("HighPt")){

	if(HasFlag("BB")){
	  
	  /// Keep EC fixed
	  /// Vary CF/Fake in20-60 range
	

	  vector<TString> IDEC = {"NPMVAEC0_CFMVAEC0p78_"};
	  
	  vMVACFBB.clear();
	  
          for(int imva=0 ; imva < 5 ; imva++){
            double mva_d=  0.68 + double(imva)*.04;
            TString mvaTS= DoubleToString(mva_d);
	    
	    vMVACFBB.push_back("CFMVABB"+mvaTS+"_");
	  }
	  
	  vector<TString> vMVANPBB;
	  
	  for(int imva=0 ; imva < 10 ; imva++){
	    double mva_d=  0. + double(imva)*.05;
	    TString mvaTS= DoubleToString(mva_d);
	    
	    vMVANPBB.push_back("NPMVABB"+mvaTS+"_NPMVAEB"+mvaTS+"_");
	    
	    
	    for(int imva2=0 ; imva2 < 7 ; imva2++){
	      double mva_d2=  -0.5 + double(imva2)* (mva_d+0.5)/6;
	      TString mvaTS2= DoubleToString(mva_d2);
	      TString IDnew = "NPMVABB1"+mvaTS2+"_NPMVABB2"+mvaTS+ "_NPMVAEB1"+mvaTS2+"_NPMVAEB2"+mvaTS+ "_NPMVAPt60_";
	      if (!std::count(vMVANPBB.begin(), vMVANPBB.end(), IDnew)) vMVANPBB.push_back(IDnew);
	    }
	  }
	
	  
	  for(auto i1 : vMVACFBB) {
	    for(auto i2 : vMVANPBB) {
	      for(auto i3 : IDEC) {
		ElectronsIDs.push_back("HNLUL_ConvBConvEC_LooseTrig_"+i1+i2+i3+"DXYv1" );
		
	      }
	    }
	  }
	}
	else  if(HasFlag("EC")){

          /// Keep BB fixed                                                                                                                                                                                                                                                   
          /// Vary CF/Fake in20-60 range                                                                                                                                                                                                                                      


          vector<TString> IDBB = {"NPMVABB0p15_NPMVAEB0p15_CFMVABB0p7_"};

          vMVACFEC.clear();

          for(int imva=0 ; imva < 5 ; imva++){
            double mva_d=  0.68 + double(imva)*.04;
            TString mvaTS= DoubleToString(mva_d);

            vMVACFEC.push_back("CFMVAEC"+mvaTS+"_");
          }

          vector<TString> vMVANPEC;

          for(int imva=0 ; imva < 10 ; imva++){
            double mva_d=  0. + double(imva)*.05;
            TString mvaTS= DoubleToString(mva_d);

            vMVANPEC.push_back("NPMVAEC"+mvaTS+"_");

            for(int imva2=0 ; imva2 < 7 ; imva2++){
              double mva_d2=  -0.5 + double(imva2)* (mva_d+0.5)/6;
              TString mvaTS2= DoubleToString(mva_d2);
              TString IDnew = "NPMVAEC1"+mvaTS2+"_NPMVAEC2"+mvaTS+"_NPMVAPt60_";
              if (!std::count(vMVANPEC.begin(), vMVANPEC.end(), IDnew)) vMVANPEC.push_back(IDnew);
            }
          }


          for(auto i1 : vMVACFEC) {
            for(auto i2 : vMVANPEC) {
              for(auto i3 : IDBB) {
                ElectronsIDs.push_back("HNLUL_ConvBConvEC_LooseTrig_"+i1+i2+i3+"DXYv1" );
		
              }
            }
          }
        }
      }
      
      else{

	if(1){
	  vector<TString> IDEC = {"NPMVAEC0p2_CFMVAEC0p84_"};
	  vector<TString> vMVANPBB;
	  
	  for(int imva=0 ; imva < 14 ; imva++){
	    double mva_d=  -0.5 + double(imva)*.05;
	    TString mvaTS= DoubleToString(mva_d);
	    vMVANPBB.push_back("NPMVABB1"+mvaTS+"_NPMVAEB1"+mvaTS+"_NPMVABB20p15_NPMVAEB20p15_NPMVAPt20_");
	  }
	  
	  
	  for(auto i1 : vMVANPBB) {
	    for(auto i2 : IDEC) {
	      ElectronsIDs.push_back("HNLUL_ConvBConvEC_LooseTrig_CFMVABB0p7_"+i1+i2+"DXYv1" );
	    }
	  }
	}
	if(1){
	  vector<TString> IDBB = {"NPMVABB0p15_NPMVAEB0p15_CFMVABB0p7_"};
	  vector<TString> vMVANPEC;
	  
	  for(int imva=0 ; imva < 15 ; imva++){
	    double mva_d=  -0.5 + double(imva)*.05;
	    TString mvaTS= DoubleToString(mva_d);
	    vMVANPEC.push_back("NPMVAEC1"+mvaTS+"_NPMVAEC20p2_NPMVAPt20_");
	  }
	  
	  
	  for(auto i1 : vMVANPEC) {
	    for(auto i2 : IDBB) {
	      ElectronsIDs.push_back("HNLUL_ConvBConvEC_LooseTrig_CFMVAEC0p84_"+i1+i2+"DXYv1" );
	      ElectronsIDs.push_back("HNLUL_ConvBConvEC_LooseTrig_CFMVAEC0p8_"+i1+i2+"DXYv1" );
	    }
	  }
	}
	
      }	


    }
    
  
    /// V1 El
    if(HasFlag("ELID_CF")){
      
      if(HasFlag("HighPt")){
	
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
	  for(auto i1 : vMVACFBB) {
	    ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCEC_LooseTrig_"+i1+"DXYv1"); 
	  }
	}
	if(HasFlag("EC")){
	  for(auto i1 : vMVACFEC) {
	    ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_CCBB_LooseTrig_"+i1+"DXYv1");
	  }
	}
	if(HasFlag("FullEta")){
	  for(auto i1 : vMVACFBB2) {
	    for(auto i2 : vMVACFEC2) {
	      ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i1+i2+"DXYv1");
	    }
	  }
	}
      }
      else  if(HasFlag("FullPt")){

	vMVACFBB.clear();
        vMVACFEC.clear();

	vector<TString> vMVACFPt={"30","40","50","60"};
	

	for(int imva=0 ; imva < 11 ; imva++){
          double mva_d=  0.45 + double(imva)*.04;
          TString mvaTS= DoubleToString(mva_d);
          vMVACFBB.push_back("CFMVABB1"+mvaTS+"_CFMVABB20p85_");
          vMVACFEC.push_back("CFMVAEC1"+mvaTS+"_CFMVAEC20p85_");
        }

	vector<TString> vMVACFBB2;
        vector<TString> vMVACFEC2;

        for(int imva=0 ; imva < 11 ; imva++){
          double mva_dB=  0.3 + double(imva)*.04;
          double mva_dEC=  0.4 + double(imva)*.04;
          TString mvaTSB= DoubleToString(mva_dB);
          TString mvaTSEC= DoubleToString(mva_dEC);
          vMVACFBB2.push_back("CFMVABB1"+mvaTSB+"_CFMVABB20p7_");
          vMVACFEC2.push_back("CFMVAEC1"+mvaTSEC+"_CFMVAEC20p8_");
        }

	//HNLOpt_UL_ElOpt_HNLUL_POGT_ConvBConvEC_LooseTrig_CCBB_CFMVAEC10p45_CFMVAEC20p85_CFMVAPt20_DXYv1;1
	
	for(auto i1 : vMVACFBB)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_CCEC_"+i1+"CFMVAPt20_DXYv1");
	for(auto i1 : vMVACFEC)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_CCBB_"+i1+"CFMVAPt20_DXYv1");
	for(auto i1 : vMVACFBB2)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_CCEC_"+i1+"CFMVAPt20_DXYv1");
	for(auto i1 : vMVACFEC2)  ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_CCBB_"+i1+"CFMVAPt20_DXYv1");


	for(auto i1 : vMVACFBB) {
	  
	  for(auto i2 : vMVACFEC) {
	    for(auto i3 : vMVACFPt) {
	      ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i1+"CFMVAPt"+i3+"_"+i2+"_DXYv1");
	    }
	  }
	}
	
	for(auto i1 : vMVACFBB2) {
          for(auto i2 : vMVACFEC2) {
            for(auto i3 : vMVACFPt) {
              ElectronsIDs.push_back("HNLUL_POGT_ConvBConvEC_LooseTrig_"+i1+"CFMVAPt"+i3+"_"+i2+"_DXYv1");
            }
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
    ElectronsIDs.push_back("HNOpt");
    ElectronsIDs.push_back("HNTight2016Update");
    //ElectronsIDs.push_back("HNTight_Opt_ECCC_Iso1_dxy1_dz1");
    //ElectronsIDs.push_back("");
    //ElectronsIDs.push_back("");
    ElectronsIDs.push_back("HNTight_ULInProgress");


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


      //cout << param_signal.Name << endl;
      RunULAnalysis(param_signal,ElectronCollV,MuonCollV);

      param_signal.Name = param_signal.DefName;
      param_signal.SRConfig  = "";
    }
  }



  


  bool opt_IDMu=!RunEE;
  if(opt_IDMu){

    vector<TString> MuonsIDs;

    if(!SameCharge(MuonCollV)) return;
    if(ElectronCollV.size() > 0) return;

    /// TEMP                                                                                                                               
    if(HasFlag("HighPt") && MuonCollV[1].Pt() <60) return;
  

    vector<TString> ID;
    vector<TString> IDBB;
    vector<TString> IDEC;

    if(HasFlag("MuID_NP")){

      if(HasFlag("HighPt")){
	
	if(HasFlag("BB")){
	  
	  IDEC = {"POGECT_ISOEC0p15_"};
	  TString BBID="NPMVAPt20_NPMVABB1neg1_NPMVABB2";
	  for(unsigned int imva=0 ; imva < 56 ; imva++){
	    double mva_d= 0.4 + double(imva)*0.01;
	    TString mvaTS= DoubleToString(mva_d);
	    IDBB.push_back(BBID+mvaTS+"_");
	  }

	  for(auto i1 : IDBB){
            for(auto i2 : IDEC){
              MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv1");
            }
          }

	}
	else if(HasFlag("EC")){

          IDBB = {"POGBT_ISOB0p15_"};
          TString ECID="NPMVAPt20_NPMVAEC1neg1_NPMVAEC2";
          for(unsigned int imva=0 ; imva < 56 ; imva++){
            double mva_d= 0.4 + double(imva)*0.01;
            TString mvaTS= DoubleToString(mva_d);
            IDEC.push_back(ECID+mvaTS+"_");
          }


	  for(auto i1 : IDBB){
            for(auto i2 : IDEC){
              MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv1");
            }
          }

	}
	else if(HasFlag("FullEta")){

	  /// B+EC 

	  vector<TString> mvaBB,  mvaEC;
	  for(unsigned int imva=0 ; imva < 16 ; imva++){
            double mva_d= 0.5 + double(imva)*0.02;
            TString mvaTS= DoubleToString(mva_d);
            mvaBB.push_back("NPMVAPt20_NPMVABB1neg1_NPMVABB2"+mvaTS+"_");
	    mvaEC.push_back("NPMVAPt20_NPMVAEC1neg1_NPMVAEC2"+mvaTS+"_");
	  }
	  for(auto ib : mvaBB){
	    for(auto ie : mvaEC){
	      ID.push_back(ib+ie);
	    }
	  }
	  
	  for(auto i1 : ID){
            MuonsIDs.push_back("HNLUL_"+i1+"DXYv1");
          }


	}
	else if(HasFlag("Full")){

          /// B+EC                                                                                                                                                                                               
          vector<TString> mvaBB = {"NPMVAPt20_NPMVABB10p7_NPMVABB20p7_"};
	  vector<TString>mvaEC = {"NPMVAPt20_NPMVAEC10p65_NPMVAEC20p65_"};

          for(auto ib : mvaBB){
            for(auto ie : mvaEC){
              ID.push_back(ib+ie);
            }
          }
	  
	}//FULL
      }// High Pt
      
      
      if(HasFlag("FullPt")){
	
	if(HasFlag("BB")){
	  IDEC = {"POGECT_ISOEC0p15_"};
	  
	  vector<TString> mvaBBTau;
	  for(unsigned int imva=0 ; imva < 21 ; imva++){
	    double mva_d= 2.5 + double(imva)*0.5;
	    TString mvaTS= DoubleToString(mva_d);
	    mvaBBTau.push_back("MVABBTau"+mvaTS+"_");
	  }
	  vector<TString> mvaBBA;
	  for(unsigned int imva=0 ; imva < 21 ; imva++){
	    double mva_d= 0. + double(imva)*0.5;
	    TString mvaTS= DoubleToString(mva_d);
	    mvaBBA.push_back("MVABBA"+mvaTS+"_");
	  }
	  
	  ///
	  TString cVal="";
	  if(GetYear()==2016) cVal="MVABBC0p73_";
	  if(GetYear()==2017) cVal="MVABBC0p71_";
	  if(GetYear()==2018) cVal="MVABBC0p65_";

	  for(auto it : mvaBBTau){
	    for(unsigned int it2 =0; it2 <  mvaBBA.size(); it2++){
	      if( it2 == 0 && it != "MVABBTau2p5_") continue;
	      if(it.Contains("MVABBTau4") && it2 > 16) continue;
	      if(it.Contains("MVABBTau5") && it2 > 14) continue;
	      if(it.Contains("MVABBTau6") && it2 > 10) continue;
	      if(it.Contains("MVABBTau7") && it2 > 8) continue;
	      if(it.Contains("MVABBTau8") && it2 > 7) continue;
	      if(it.Contains("MVABBTau9") && it2 > 6) continue;
	      if(it.Contains("MVABBTau10") && it2 > 6) continue;
	      if(it.Contains("MVABBTau11") && it2 > 5) continue;
	      if(it.Contains("MVABBTau12") && it2 > 5) continue;
	      IDBB.push_back(cVal+it+mvaBBA[it2]);
	    }
	  }

	  for(auto i1 : IDBB){
            for(auto i2 : IDEC){
              MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv1");
            }
          }
	}
	if(HasFlag("EC")){

	  TString cVal="";
          if(GetYear()==2016) cVal="MVAECC0p52_";
          if(GetYear()==2017) cVal="MVAECC0p54_";
          if(GetYear()==2018) cVal="MVAECC0p55_";

	  
          IDBB = {"POGBT_ISOB0p15_"};
          vector<TString> mvaECTau;
          for(unsigned int imva=0 ; imva < 21 ; imva++){
            double mva_d= 2.5 + double(imva)*0.5;
            TString mvaTS= DoubleToString(mva_d);
            mvaECTau.push_back("MVAECTau"+mvaTS+"_");
          }
          vector<TString> mvaECA;
          for(unsigned int imva=0 ; imva < 21 ; imva++){
            double mva_d= 0. + double(imva)*0.5;
            TString mvaTS= DoubleToString(mva_d);
            mvaECA.push_back("MVAECA"+mvaTS+"_");
          }

          for(auto it : mvaECTau){
            for(unsigned int it2 =0; it2 <  mvaECA.size(); it2++){
	      if( it2 == 0 && it != "MVAECTau2p5_") continue;
              if(it.Contains("MVAECTau4") && it2 > 16) continue;
              if(it.Contains("MVAECTau5") && it2 > 14) continue;
              if(it.Contains("MVAECTau6") && it2 > 10) continue;
              if(it.Contains("MVAECTau7") && it2 > 8) continue;
              if(it.Contains("MVAECTau8") && it2 > 7) continue;
              if(it.Contains("MVAECTau9") && it2 > 6) continue;
              if(it.Contains("MVAECTau10") && it2 > 6) continue;
              if(it.Contains("MVAECTau11") && it2 > 5) continue;
              if(it.Contains("MVAECTau12") && it2 > 5) continue;

              IDEC.push_back(cVal+it+mvaECA[it2]);
	    }   
          }
	  
	  for(auto i1 : IDBB){
	    for(auto i2 : IDEC){
	      MuonsIDs.push_back("HNLUL_"+i1+i2+"DXYv1");
	    }
	  }


        }

	if(HasFlag("FullEtaSlope")){

          /// B+EC                                                                                                                                                                                        
	  vector<TString> mvaPt = {"NPMVAPt20_"};
	  
          vector<TString> mvaBB,  mvaEC;
	  
	  for(unsigned int imva2=0 ; imva2 < 1 ; imva2++){
	    double mva2B_d= 0.77 + double(imva2)*0.02;
	    double mva2EC_d= 0.6 + double(imva2)*0.02;
	    for(unsigned int imva1=0 ; imva1 < 81 ; imva1++){
	      double mvaB_d= -0.83 + double(imva1)*(0.02);
	      double mvaEC_d= -1.0 + double(imva1)*(0.02);
	      
	      mvaBB.push_back("NPMVABB1"+DoubleToString(mvaB_d)+"_NPMVABB2"+DoubleToString(mva2B_d)+"_");
	      mvaEC.push_back("NPMVAEC1"+DoubleToString(mvaEC_d)+"_NPMVAEC2"+DoubleToString(mva2EC_d)+"_");
	    }
	  }
	  TString EEID = "NPMVAPt20_NPMVAEC10p6_NPMVAEC20p6_";
	  TString BBID = "NPMVAPt20_NPMVABB10p77_NPMVABB20p77_";
	  for(auto ib : mvaBB)   {
	    for(auto ipt :  mvaPt) {
	      ID.push_back(EEID+ib+ipt);
	    }
	  }
	  
	  for(auto ie : mvaEC){
	    for(auto ipt :  mvaPt) {
	      
	      ID.push_back(BBID+ie+ipt);
	    }
	  }


	  for(auto i1 : ID){
	    MuonsIDs.push_back("HNLUL_"+i1+"DXYv1");
	  }

	  
        }
	if(HasFlag("FullEtaExp")){

	  vector<TString> mvaBB,  mvaEC;

          for(unsigned int imva=0 ; imva < 5 ; imva++){
            double mva_c= 0.55 + double(imva)*0.05;
	    for(unsigned int imva2=0 ; imva2 < 21 ; imva2++){
	      double mva_tau= 2.5 + double(imva2)*0.5;
	      for(unsigned int imva3=0 ; imva3 < 11 ; imva3++){
		double mva_a= 0. + double(imva3)*0.5;
		if( imva3 == 0 && imva2 != 0) continue;
		
		double cutCategory10 =  mva_c - exp(- 10. / mva_tau) * mva_a;
		if(cutCategory10 < 0.) continue;
		
		mvaBB.push_back("MVABBC"+DoubleToString(mva_c)+"_MVABBTau"+DoubleToString(mva_tau)+"_MVABBA"+DoubleToString(mva_a)+"_");
		mvaEC.push_back("MVAECC"+DoubleToString(mva_c)+"_MVAECTau"+DoubleToString(mva_tau)+"_MVAECA"+DoubleToString(mva_a)+"_");
	      }
	    }
	  }

          for(auto ib : mvaBB)  ID.push_back(ib+"MVAECC0p65_MVAECTau0_MVAECA0_");
	  for(auto ie : mvaEC)  ID.push_back(ie+"MVABBC0p77_MVABBTau0_MVABBA0_");
	  for(auto i1 : ID)  MuonsIDs.push_back("HNLUL_"+i1+"DXYv1");
	
	}
      }
    }
    
    if(HasFlag("Mu_Conv")){
      
      vector<TString> mvaConv;
      
      for(unsigned int imva=0 ; imva < 50 ; imva++){
	
	double mva_c= -1 + double(imva)*0.04;
	mvaConv.push_back("MVAConv"+DoubleToString(mva_c)+"_");
	
      }
      
      for(auto ib : mvaConv){
	MuonsIDs.push_back("HNLUL_HNL" + GetYearString()+"_"+ ib);
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
    
    
    MuonsIDs.push_back("HNTight_17028");
    MuonsIDs.push_back("HNTightV2");
    MuonsIDs.push_back("HNTightV3");
    MuonsIDs.push_back("HNL_ULID_2016");
    MuonsIDs.push_back("HNL_ULID_2017");
    MuonsIDs.push_back("HNL_ULID_2018");
    MuonsIDs.push_back("HNL_ULID_v1_2016");
    MuonsIDs.push_back("HNL_ULID_v1_2017");
    MuonsIDs.push_back("HNL_ULID_v1_2018");
    MuonsIDs.push_back("HNL_ULID_v2_2016");
    MuonsIDs.push_back("HNL_ULID_v2_2017");
    MuonsIDs.push_back("HNL_Peking");
    MuonsIDs.push_back("HNL_HN3L");
    MuonsIDs.push_back("HNL_Top1");
    MuonsIDs.push_back("HNL_Top2");
    MuonsIDs.push_back("HNL_Top3");


    for (unsigned int i=0; i<MuonsIDs.size(); i++) {
      for (unsigned int j=0; j<MuonsIDs.size(); j++) {
	if(i != j && MuonsIDs[i] == MuonsIDs[j]) {
	  cout << "[Muon::ID]  : " << MuonsIDs[i] << " occurs twice" << endl;

	  exit(ENODATA);

	}
      }
    }


    for(auto id : MuonsIDs){
      //      cout << id << endl;
      if(id.Contains("HNLUL")){
        param_signal.Name = param_signal.DefName  + "_MuOpt_"+ id;
        param_signal.SRConfig  = "";
        param_signal.Muon_Tight_ID="MuOpt_"+id;
        param_signal.Muon_FR_ID = "MuOptLoose_"+id;
        param_signal.Electron_Tight_ID = "HNTightV2";
        param_signal.Electron_FR_ID = "HNLooseV4";

        RunULAnalysis(param_signal,ElectronCollV,MuonCollV);
      }
      else{
        param_signal.Name = param_signal.DefName  + id;
        param_signal.SRConfig  = "";

        param_signal.Muon_Tight_ID=id;
        param_signal.Muon_FR_ID = id;
        param_signal.Electron_Tight_ID = "HNTightV2";
        param_signal.Electron_FR_ID = "HNLooseV4";

        RunULAnalysis(param_signal,ElectronCollV,MuonCollV);

      }
    }

  }

  
  

  if(!IsData) RunSyst=false;
  if(RunSyst){
    TString param_signal_name = param_signal.Name;
    vector<AnalyzerParameter::Syst> SystList;// = GetSystList("Initial");

    for(auto isyst : SystList){
      param_signal.syst_ = AnalyzerParameter::Syst(isyst);
      
      param_signal.Name = "Syst_"+param_signal.GetSystType()+param_signal_name;
      param_signal.DefName = "Syst_"+param_signal.GetSystType()+param_signal_name;
      RunULAnalysis(param_signal,ElectronCollV,MuonCollV);
    }
  }    


  return ;
}

void HNL_SignalLeptonOpt::RunULAnalysis(AnalyzerParameter param, vector<Electron> ElectronCollV, vector<Muon> MuonCollV){

  if(run_Debug) cout << "HNL_SignalLeptonOpt::executeEvent " << endl;

  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);
  
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
  
  
  std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(MuonCollV,ElectronCollV);

  std::vector<Tau>        TauColl        = GetTaus     (leps_veto,param.Tau_Veto_ID,20., 2.3);

  // Creat Lepton vector to have lepton blind codes 

  Particle METv = GetvMET("PuppiT1xyCorr",param); // returns MET with systematic correction



  std::vector<FatJet> fatjets_tmp  = GetFatJets(param, param.FatJet_ID, 200., 5.);
  std::vector<FatJet> FatjetColl   = SelectAK8Jetsv2(fatjets_tmp, 200., 2.7, true,  1., true, -999, true, 40., 130.,-999, ElectronCollV, MuonCollV);
  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);

  std::vector<Jet> JetCollLoose                    = SelectAK4Jets(jets_tmp,     15., 4.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, FatjetColl);
  std::vector<Jet> BJetCollNLV                    = SelectAK4Jets(jets_tmp,     20., 2.4, false,  0.4,0.8, "",   ElectronCollV,MuonCollV, FatjetColl);
  std::vector<Jet> JetColl                        = SelectAK4Jets(jets_tmp,     20., 2.7, true,  0.4,0.8, "",   ElectronCollV,MuonCollV, FatjetColl);
  std::vector<Jet> VBF_JetColl                    = SelectAK4Jets(jets_tmp,     30., 4.7, true,  0.4,0.8, "",  ElectronCollV,MuonCollV, FatjetColl);  
  

  JetTagging::Parameters param_jetsM = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  JetTagging::Parameters param_jetsT = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Tight, JetTagging::incl, JetTagging::mujets);

  std::vector<Jet> BJetColl    = SelectBJets(param,  BJetCollNLV, param_jetsM);
  double sf_btagM_NLV               = GetBJetSF(param, BJetCollNLV, param_jetsM);
  std::vector<Jet> BJetCollSR1    = SelectBJets(param,  BJetCollNLV, param_jetsT);
  double sf_btagSR1_NLV               = GetBJetSF(param, BJetCollNLV, param_jetsT);

  
  param.WriteOutVerbose=1; // Does not Fill Cutflow OR Region Plotter  

  //if(param.Name.Contains("HNTightV2"))  param.WriteOutVerbose=0;

  
  if(!IsData )weight = weight*sf_btagM_NLV;
  if(!IsData && FatjetColl.size()==0)weight = weight*sf_btagM_NLV;
  if(!IsData && FatjetColl.size()>0)weight = weight*sf_btagSR1_NLV;


    
  /// Runs adapted SR functionality
  //FillSignalRegionForOpt(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, JetCollLoose, JetColl, VBF_JetColl,FatjetColl , BJetColl, ev,METv, param,weight);


  //if(IsData)cout << "Run data " << param.Name << endl;
  // Runs main SR functionality
  RunAllSignalRegions(Inclusive, ElectronCollT,ElectronCollV,MuonCollT,MuonCollV, TauColl, JetCollLoose, JetColl, VBF_JetColl,FatjetColl , BJetColl,BJetCollSR1, ev,METv, param,weight);


  return;
  

}



 


HNL_SignalLeptonOpt::HNL_SignalLeptonOpt(){

  TMVA::Tools::Instance();
  MVAReader = new TMVA::Reader();


}
 
HNL_SignalLeptonOpt::~HNL_SignalLeptonOpt(){

  delete MVAReader;
}




