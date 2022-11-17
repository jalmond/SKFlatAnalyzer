#include "HNL_RegionDefinitionsOpt.h"


/*


  Analysis REGION Functions 
  -----------------------------------                                                                                                                   a) RunPreslection
  b) PassVBFInitial : Use this IF using shape for SR2 
  c) PassVBF

  SR Functions
  -----------------------------------
  1- RunSignalRegionAK8 (SR1)
  2- RunSignalRegionWW  (SR2)
  3- RunSignalRegionAK4 (SR3)
  4- RunSignalRegionTrilepton  (REMNANT)


  CR Functions
  -----------------------------------
  1- RunElectronChannelCR
  2- RunMuonChannelCR

 */



void HNL_RegionDefinitionsOpt::FillSignalRegionForOpt(HNL_LeptonCore::ChargeType qq, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){
  
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};
  
  for(auto dilep_channel : channels){
    
    float weight_channel = weight_ll;
    
    if(MCSample.Contains("Type")) {
      if (!SelectChannel(dilep_channel)) continue;
    }
    
    if (param.Name.Contains("MuOpt") && dilep_channel != MuMu) continue;
    if (param.Name.Contains("ElOpt") && dilep_channel != EE) continue;
    
    
    std::vector<Lepton *> LepTColl       = MakeLeptonPointerVector(muons,electrons,param);
    std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
    
    if(!PassEventTypeFilter(LepTColl, gens)) continue;

    if(!CheckLeptonFlavourForChannel(dilep_channel, LepTColl)) continue;
    
    AnalyzerParameter param_channel = param;
    
    
    TString channel_string = GetChannelString(dilep_channel);
    param_channel.Name =  channel_string + "/" + param_channel.Name;
    

    //if(!IsDATA && dilep_channel != MuMu)  weight_channel*= GetElectronSFEventWeight(electrons, param_channel);
    //if(!IsDATA && dilep_channel != EE)    weight_channel*= GetMuonSFEventWeight(muons, param_channel);
    if(!PassMETFilter()) return;
    
    
    if(RunCF){
      if(dilep_channel == MuMu) continue;
      
      if(IsData && SameCharge(LepTColl)) continue;
      if(!IsData && !SameCharge(LepTColl)) continue;
      
      if(IsData)weight_channel = GetCFWeightElectron(LepTColl, param_channel);
      
    }
    else{
      if(!SameCharge(LepTColl)) continue;
    }
    
    if(!PassPreselection(dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, VBF_JetColl, AK8_JetColl,	B_JetColl,ev, METv ,param_channel,"", weight_channel)) continue;

    
    double ST = GetST(LepTColl, JetColl, AK8_JetColl, METv);
    double met2_st = pow(METv.Pt(),2.)/ ST;
    
    if (B_JetColl.size() > 0 ) continue;
    if (met2_st > 15.)  continue;

    if (B_JetColl.size() ==0 && met2_st < 15){


      TString SRBin = "";
      TString SRBDT100 = "";
      TString SRBDT250 = "";
      TString SRBDT500 = "";

      if(AK8_JetColl.size() > 0) {
	SRBin= RunSignalRegionAK8String (dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl,ev, METv ,param_channel,"", weight_channel) ;
	SRBDT100 =SRBin;
	SRBDT250 =SRBin;
	SRBDT500 =SRBin;
      }
      else{
	
	SRBin = RunSignalRegionWWString( dilep_channel,qq, LepTColl, leps_veto,  TauColl, JetCollLoose, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param_channel,  "", weight_channel);
	SRBDT100 =SRBin;
	SRBDT250 =SRBin;
	SRBDT500 =SRBin;

	if(SRBin == "false"){
	  
	  SRBDT100 = RunSignalRegionAK4StringBDT("100", dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, VBF_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);
	  SRBDT250 = RunSignalRegionAK4StringBDT("250", dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, VBF_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);
	  SRBDT500 = RunSignalRegionAK4StringBDT("500", dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, VBF_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);
	  
	  SRBin  = RunSignalRegionAK4String (dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);
	}
      }
      
      
      vector <TString> BDTlabels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1", "SR2_bin2",  "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8","SR3_bin9"};
      vector <TString>  labels = {"SR1_bin1","SR1_bin2","SR1_bin3","SR1_bin4","SR1_bin5","SR1_bin6","SR1_bin7","SR2_bin1", "SR2_bin2", "SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8","SR3_bin9","SR3_bin10","SR3_bin11","SR3_bin12","SR3_bin13","SR3_bin14", "SR3_bin15","SR3_bin16"};
      
      for(int ip=0 ; ip < 26 ; ip++){

	double ip_d1=  2 + double(ip)*0.2;
	TString ipTS1= DoubleToString(ip_d1);

	for(int ip2=0 ; ip2 < 26 ; ip2++){

	  TString EtaRegion = "BB";
	  double ip_d2=  2 + double(ip2)*0.2;

	  TString ipTS2= DoubleToString(ip_d2);
  
	  int nel=0;
	  for(auto iel : electrons) {
	    if (Category(iel).Contains(EtaRegion)) {
	      if (iel.PassIP(ip_d1,ip_d2)) nel++;
	    }
	    else if(iel.PassID("HNTightV3")){
	      nel++;
	    }
	  }
	  if (nel==2){
	    if(SRBin != "false")FillEventCutflowDef("IPOpt", "BB_IP"+ipTS1+"_"+ipTS2, weight_channel , labels, SRBin);
	    if(SRBDT100 != "false")FillEventCutflowDef("BDTIPOpt", "100_BB_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT100);
	    if(SRBDT250 != "false")FillEventCutflowDef("BDTIPOpt", "250_BB_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT250);
	    if(SRBDT500 != "false")FillEventCutflowDef("BDTIPOpt", "500_BB_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT500);
	  }
	}
      }



      for(int ip=0 ; ip < 26 ; ip++){
        for(int ip2=0 ; ip2 < 26 ; ip2++){

          TString EtaRegion = "EB";
          double ip_d1=  2 + double(ip)*0.25;
          double ip_d2=  2 + double(ip2)*0.25;
          TString ipTS1= DoubleToString(ip_d1);
          TString ipTS2= DoubleToString(ip_d2);


          int nel=0;
          for(auto iel : electrons) {
            if (Category(iel).Contains(EtaRegion)) {
              if (iel.PassIP(ip_d1,ip_d2)) nel++;
            }
            else if(iel.PassID("HNTightV3")){
              nel++;
            }
          }
          if (nel==2){
            if(SRBin != "false")FillEventCutflowDef("IPOpt", "EB_IP"+ipTS1+"_"+ipTS2, weight_channel , labels, SRBin);
            if(SRBDT100 != "false")FillEventCutflowDef("BDTIPOpt", "100_EB_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT100);
            if(SRBDT250 != "false")FillEventCutflowDef("BDTIPOpt", "250_EB_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT250);
            if(SRBDT500 != "false")FillEventCutflowDef("BDTIPOpt", "500_EB_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT500);
          }
        }
      }
      

      for(int ip=0 ; ip < 26 ; ip++){
        for(int ip2=0 ; ip2 < 26 ; ip2++){

          TString EtaRegion = "EE";
          double ip_d1=  2 + double(ip)*0.3;
          double ip_d2=  2 + double(ip2)*0.3;
          TString ipTS1= DoubleToString(ip_d1);
          TString ipTS2= DoubleToString(ip_d2);

          int nel=0;
          for(auto iel : electrons) {
            if (Category(iel).Contains(EtaRegion)) {
              if (iel.PassIP(ip_d1,ip_d2)) nel++;
            }
            else if(iel.PassID("HNTightV3")){
              nel++;
            }
          }
          if (nel==2){
            if(SRBin != "false")FillEventCutflowDef("IPOpt", "EE_IP"+ipTS1+"_"+ipTS2, weight_channel , labels, SRBin);
            if(SRBDT100 != "false")FillEventCutflowDef("BDTIPOpt", "100_EE_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT100);
            if(SRBDT250 != "false")FillEventCutflowDef("BDTIPOpt", "250_EE_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT250);
            if(SRBDT500 != "false")FillEventCutflowDef("BDTIPOpt", "500_EE_IP"+ipTS1+"_"+ipTS2, weight_channel , BDTlabels, SRBDT500);
          }
        }
      }

     
     
      
      continue;
      
      
	
	
      int nPtbins=50;
      double ptbins[nPtbins+1] = { 15.,16.,17.,18.,19.,
				   20.,21.,22.,23.,24.,25.,26.,27.,28.,29.,
				   30.,31.,32.,33.,34.,35.,36.,37.,38.,39.,
				   40.,41.,42.,43.,44.,45.,46.,47.,48.,49.,
				   50.,51.,52.,53.,54.,55.,56.,57.,58.,59.,
				   60.,70.,80.,90.,100.,200.};
      
      int nIsobins=100;
      //double isobins[nIsobins+1] = {0.,   0.0, 0.04, 0.05, 0.055, 0.06, 0.065,
      //				  0.07, 0.075, 0.08,  0.085,0.09,   0.095,
      //				  0.1,  0.11,  0.12,  0.13,  0.14,  0.15};
      
      double Isobins[101];
      for(int i=0; i < 101; i++){
	Isobins [i] = double(i)*0.002;
      }
      
      int nMVAbins = 162;
      double mvabins[nMVAbins+1] = {-8.1,-8.0, -7.9, - 7.8, -7.7, -7.6,-7.5,-7.4,-7.3,-7.2,-7.1,
				    -7.0, -6.9, - 6.8, -6.7, -6.6,-6.5,-6.4,-6.3,-6.2,-6.1,
				    -6.0, -5.9, - 5.8, -5.7, -5.6,-5.5,-5.4,-5.3,-5.2,-5.1,
				    -5.0, -4.9, - 4.8, -4.7, -4.6,-4.5,-4.4,-4.3,-4.2,-4.1,
				    -4.0, -3.9, - 3.8, -3.7, -3.6,-3.5,-3.4,-3.3,-3.2,-3.1,
				    -3.0, -2.9, - 2.8, -2.7, -2.6,-2.5,-2.4,-2.3,-2.2,-2.1,
				    -2.0, -1.9, - 1.8, -1.7, -1.6,-1.5,-1.4,-1.3,-1.2,-1.1,
				    -1.0, -0.9, - 0.8, -0.7, -0.6,-0.5,-0.4,-0.3,-0.2,-0.1,
				    0.0, 0.1, 0.2, 0.3, 0.4,0.5,0.6,0.7,0.8,0.9,
				    1.0, 1.1, 1.2, 1.3, 1.4,1.5,1.6,1.7,1.8,1.9,
				    2.0, 2.1, 2.2, 2.3, 2.4,2.5,2.6,2.7,2.8,2.9,
				    3.0, 3.1, 3.2, 3.3, 3.4,3.5,3.6,3.7,3.8,3.9,
				    4.0, 4.1, 4.2, 4.3, 4.4,4.5,4.6,4.7,4.8,4.9,
				    5.0, 5.1, 5.2, 5.3, 5.4,5.5,5.6,5.7,5.8,5.9,
				    6.0, 6.1, 6.2, 6.3, 6.4,6.5,6.6,6.7,6.8,6.9,
				    7.0, 7.1, 7.2, 7.3, 7.4,7.5,7.6,7.7,7.8,7.9,8.0,8.1};
      
      
      int nIPbins=100;
      double IPbins[101];
      for(int i=0; i < 101; i++){
        IPbins[i] = double(i)*0.1;
      }
      
      int nDXYbins=80;
      double DXYbins[81];
      for(int i=0; i < 81; i++){
        DXYbins [i] =double(i)*0.001;
      }

      int nDZbins=100;
      double DZbins[101];
      for(int i=0; i < 101; i++){
        DZbins[i] = double(i)*0.001;
      }


      vector<pair <double, TString> > IsoPairs = {std::make_pair(0.2,"020"),
						  std::make_pair(0.15,"015"),
						  std::make_pair(0.14,"014"),
						  std::make_pair(0.13,"013"),
						  std::make_pair(0.12,"012"),
						  std::make_pair(0.11,"011"),
						  std::make_pair(0.1,"010"),
						  std::make_pair(0.095,"0095"),
						  std::make_pair(0.09,"009"),
						  std::make_pair(0.085,"0085"),
						  std::make_pair(0.08,"008"),
						  std::make_pair(0.075,"0075"),
						  std::make_pair(0.07,"007"),
						  std::make_pair(0.065,"0065"),
						  std::make_pair(0.06,"006"),
						  std::make_pair(0.055,"0055"),
						  std::make_pair(0.05,"005") };
      
      
      
      
      if(AK8_JetColl.size() > 0) {
	
	TString SR1Bin=  RunSignalRegionAK8String (dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl,ev, METv ,param_channel,"", weight_channel) ;
	TString SR1bin = "SR1_bin0";
	if (SR1Bin == "SR1_bin1" || SR1Bin == "SR1_bin2"  || SR1Bin == "SR1_bin3" || SR1Bin == "SR1_bin4" )SR1bin = "SR1_bin1";
	if (SR1Bin == "SR1_bin5" || SR1Bin == "SR1_bin6" || SR1Bin == "SR1_bin7" )SR1bin = "SR1_bin2";
	
	

	for(auto iel  : electrons){
	  
	  if(electrons[0].Pt() < 100) continue;
	  
	  
	  if (!param.Name.Contains("HNOpt")){
	    FillHist( "NonOpt/"+SR1bin+"_"+Category(iel)+"Pt_"+param.Name , 1., weight_channel, 2., 0.,2);
	    
	    continue;
	  }
	  

	  double pt = (iel.Pt() > 200.) ? 199. : iel.Pt();
	  
	  
	  if(iel.PassID("HNTightV2")) {
	    TString lab = "_HNTightV2";
	    FillHist( "NonOpt/"+SR1bin+"_RegionPass_"+Category(iel)+"Pt_"+param.Name+lab ,  1., weight_channel, 2., 0.,2);
	    cout << param.Name << " HNTightV2 passed weight =  " << weight_channel << " " << Category(iel) << endl;
	    OutCutFlow("NonOpt/"+SR1bin+"_RegionPass_"+Category(iel)+"Pt_"+param.Name+lab , weight_channel);
	  }
	  
	  
	  if(iel.passMVAID_Iso_WP90()){
	    if(iel.IsGsfCtfScPixChargeConsistent()){
	      FillHist( "ISOOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"Pt_Iso_"+param.Name , pt , iel.RelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      FillHist( "ISOOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"Pt_MiniIso_"+param.Name , pt , iel.MiniRelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      if(iel.RelIso() < 0.15){
		FillHist( "IPOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"Pt_IP_"+param.Name , pt , fabs(iel.IP3D()/iel.IP3Derr()) , weight_channel, nPtbins,  ptbins , nIPbins,IPbins );
		FillHist( "IPOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"Pt_DXY_"+param.Name , pt , fabs(iel.dXY()) , weight_channel, nPtbins,  ptbins , nDXYbins,DXYbins );
		FillHist( "IPOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"Pt_DZ_"+param.Name , pt , fabs(iel.dZ()) , weight_channel, nPtbins,  ptbins , nDZbins,DZbins );

		FillHist( "CBOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"MissingHits_"+param.Name, iel.NMissingHits() , weight_channel, 5, 0., 5.);
		FillHist( "CBOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"sigmaIetaIeta_"+param.Name, iel.Full5x5_sigmaIetaIeta() , weight_channel, 1000, 0., 0.1);
		FillHist( "CBOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"dEtaSeed_"+param.Name, fabs(iel.dEtaSeed()) , weight_channel, 1000, 0., 0.01);
		FillHist( "CBOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"dPhiIn_"+param.Name, fabs(iel.dPhiIn()) , weight_channel, 1000, 0., 1.);
		FillHist( "CBOpt/"+SR1bin+"_MVAIso90_"+Category(iel)+"InvEminusInvP_"+param.Name, fabs(iel.InvEminusInvP()) , weight_channel, 1000, 0., 1.);

	      }
	    }
	  }
	  if(iel.passMVAID_noIso_WP90()){
	    if(iel.IsGsfCtfScPixChargeConsistent()){
	      FillHist( "ISOOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"Pt_Iso_"+param.Name , pt , iel.RelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      FillHist( "ISOOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"Pt_MiniIso_"+param.Name , pt , iel.MiniRelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      if(iel.RelIso() <0.15){
		FillHist( "IPOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"Pt_IP_"+param.Name , pt , fabs(iel.IP3D()/iel.IP3Derr()) , weight_channel, nPtbins,  ptbins , nIPbins,IPbins );
		FillHist( "IPOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"Pt_DXY_"+param.Name , pt , fabs(iel.dXY()) , weight_channel, nPtbins,  ptbins , nDXYbins,DXYbins );
		FillHist( "IPOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"Pt_DZ_"+param.Name , pt , fabs(iel.dZ()) , weight_channel, nPtbins,  ptbins , nDZbins,DZbins );

                FillHist( "CBOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"MissingHits_"+param.Name, iel.NMissingHits() , weight_channel, 5, 0., 5.);
                FillHist( "CBOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"sigmaIetaIeta_"+param.Name, iel.Full5x5_sigmaIetaIeta() , weight_channel, 1000, 0., 0.1);
                FillHist( "CBOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"dEtaSeed_"+param.Name, fabs(iel.dEtaSeed()) , weight_channel, 1000, 0., 0.01);
                FillHist( "CBOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"dPhiIn_"+param.Name, fabs(iel.dPhiIn()) , weight_channel, 1000, 0., 1.);
                FillHist( "CBOpt/"+SR1bin+"_MVANoIso90_"+Category(iel)+"InvEminusInvP_"+param.Name, fabs(iel.InvEminusInvP()) , weight_channel, 1000, 0., 1.);

	      }
	    }
	  }
	  
	  for (auto IsoPair : IsoPairs){
	    
	    if(iel.RelIso() < IsoPair.first){
	      FillHist( "MVAOpt/"+SR1bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
	      FillHist( "MVAOpt/"+SR1bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
	      if(iel.IsGsfCtfScPixChargeConsistent()){
		FillHist( "MVAOpt/CC_"+SR1bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		FillHist( "MVAOpt/CC_"+SR1bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
		cout << "MVAOpt/CC_"+SR1bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name + " passed weight =  " << weight_channel << " " << Category(iel) << endl;		
		OutCutFlow("MVAOpt/CC_"+SR1bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , weight_channel);

	      }
	    }
	    if(iel.MiniRelIso() < IsoPair.first){
	      FillHist( "MVAOpt/"+SR1bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
	      FillHist( "MVAOpt/"+SR1bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
	      if(iel.IsGsfCtfScPixChargeConsistent()){
		FillHist( "MVAOpt/CC_"+SR1bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		FillHist( "MVAOpt/CC_"+SR1bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
	      }
	    }
	  }
	}
      }
      else {
	
	TString SRWWbin = RunSignalRegionWWString( dilep_channel,qq, LepTColl, leps_veto,  TauColl, JetCollLoose, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param_channel,  "", weight_channel);	
	TString SRAK4bin  = RunSignalRegionAK4String (dilep_channel,qq, LepTColl, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);
	
	TString SR3bin = SRWWbin;
	if (SRWWbin == "false") SR3bin = SRAK4bin;
	
	
	for(auto iel  : electrons){
	  if (!param.Name.Contains("HNOpt")){
	    if(SR3bin!="false") FillHist( "NonOpt/"+SR3bin+"_"+Category(iel)+"Pt_"+param.Name , 1., weight_channel, 2., 0.,2);
	    continue;
	  }
	  
	  
	  double pt = (iel.Pt() > 200.) ? 199. : iel.Pt();
	    
	  
	  if(iel.PassID("HNTightV2")) {
	    TString lab = "_HNTightV2";
	    if(SR3bin!="false")FillHist( "NonOpt/"+SR3bin+"_RegionPass_"+Category(iel)+"Pt_"+param.Name+lab ,  1., weight_channel, 2., 0.,2);
	    OutCutFlow( "NonOpt/"+SR3bin+"_RegionPass_"+Category(iel)+"Pt_"+param.Name+lab  , weight_channel);


	  }
	  
	  
	  if(iel.passMVAID_Iso_WP90()){
	    if(iel.IsGsfCtfScPixChargeConsistent()){
	      if(SR3bin!="false")FillHist( "ISOOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"Pt_Iso_"+param.Name  , pt , iel.RelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      if(SR3bin!="false")FillHist( "ISOOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"Pt_MiniIso_"+param.Name  , pt , iel.MiniRelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      if(iel.RelIso() <0.15){
		if(SR3bin!="false")FillHist( "IPOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"Pt_IP_"+param.Name  , pt , fabs(iel.IP3D()/iel.IP3Derr()) , weight_channel, nPtbins,  ptbins , nIPbins,IPbins );
		if(SR3bin!="false")FillHist( "IPOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"Pt_DXY_"+param.Name  , pt , fabs(iel.dXY()) , weight_channel, nPtbins,  ptbins , nDXYbins,DXYbins );
		if(SR3bin!="false")FillHist( "IPOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"Pt_DZ_"+param.Name  , pt , fabs(iel.dZ()) , weight_channel, nPtbins,  ptbins , nDZbins,DZbins );

                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"MissingHits_"+param.Name, iel.NMissingHits() , weight_channel, 5, 0., 5.);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"sigmaIetaIeta_"+param.Name, iel.Full5x5_sigmaIetaIeta() , weight_channel, 1000, 0., 0.1);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"dEtaSeed_"+param.Name, fabs(iel.dEtaSeed()) , weight_channel, 1000, 0., 0.01);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"dPhiIn_"+param.Name, fabs(iel.dPhiIn()) , weight_channel, 1000, 0., 1.);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVAIso90_"+Category(iel)+"InvEminusInvP_"+param.Name, fabs(iel.InvEminusInvP()) , weight_channel, 1000, 0., 1.);

	      }
	    }
	  }
	  if(iel.passMVAID_noIso_WP90()){
	    if(iel.IsGsfCtfScPixChargeConsistent()){
	      if(SR3bin!="false")FillHist( "ISOOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"Pt_Iso_"+param.Name  , pt , iel.RelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      if(SR3bin!="false")FillHist( "ISOOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"Pt_MiniIso_"+param.Name  , pt , iel.MiniRelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
	      if(iel.RelIso() <0.15){
		if(SR3bin!="false")FillHist( "IPOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"Pt_IP_"+param.Name  , pt , fabs(iel.IP3D()/iel.IP3Derr()) , weight_channel, nPtbins,  ptbins , nIPbins,IPbins );
		if(SR3bin!="false")FillHist( "IPOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"Pt_DXY_"+param.Name  , pt , fabs(iel.dXY()) , weight_channel, nPtbins,  ptbins , nDXYbins,DXYbins );
		if(SR3bin!="false")FillHist( "IPOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"Pt_DZ_"+param.Name  , pt , fabs(iel.dZ()) , weight_channel, nPtbins,  ptbins , nDZbins,DZbins );
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"MissingHits_"+param.Name, iel.NMissingHits() , weight_channel, 5, 0., 5.);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"sigmaIetaIeta_"+param.Name, iel.Full5x5_sigmaIetaIeta() , weight_channel, 1000, 0., 0.1);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"dEtaSeed_"+param.Name, fabs(iel.dEtaSeed()) , weight_channel, 1000, 0., 0.01);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"dPhiIn_"+param.Name, fabs(iel.dPhiIn()) , weight_channel, 1000, 0., 1.);
                if(SR3bin!="false")FillHist( "CBOpt/"+SR3bin+"_MVANoIso90_"+Category(iel)+"InvEminusInvP_"+param.Name, fabs(iel.InvEminusInvP()) , weight_channel, 1000, 0., 1.);

	      }
	    }
	  }
	  
	  for (auto IsoPair : IsoPairs){
	    
	    if(iel.RelIso() < IsoPair.first){
	      
	      if(SR3bin!="false")FillHist( "MVAOpt/"+SR3bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
	      if(SR3bin!="false")FillHist( "MVAOpt/"+SR3bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
	      
	      if(iel.IsGsfCtfScPixChargeConsistent()){
		if(SR3bin!="false")FillHist( "MVAOpt/CC_"+SR3bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		if(SR3bin!="false")FillHist( "MVAOpt/CC_"+SR3bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);

		OutCutFlow( "MVAOpt/CC_"+SR3bin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , weight_channel);

	      }
	    }
	    if(iel.MiniRelIso() < IsoPair.first){
	      
	      if(SR3bin!="false")FillHist( "MVAOpt/"+SR3bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
	      if(SR3bin!="false")FillHist( "MVAOpt/"+SR3bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
	      
	      if(iel.IsGsfCtfScPixChargeConsistent()){
		if(SR3bin!="false")FillHist( "MVAOpt/CC_"+SR3bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		if(SR3bin!="false")FillHist( "MVAOpt/CC_"+SR3bin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
		
		
	      }
	    }
	  }
	}
      
      
	Particle ll =  (*LepTColl[0]) + (*LepTColl[1]);
	float Mll = GetLLMass(LepTColl);
	
	Nj      = JetColl.size();
	Nvbfj   = VBF_JetColl.size();
	Ptl1    = LepTColl[0]->Pt();
	Ptl2    = LepTColl.at(1)->Pt();
	LT      = GetLT(LepTColl);
	Ptj1    =  JetColl.size()<1? -1.: JetColl.at(0).Pt();
	Ptj2    =  JetColl.size()<2? -1.:JetColl.at(1).Pt();
	Ptj3    =  JetColl.size()<3? -1.:JetColl.at(2).Pt();
	MET     = METv.Pt();
	
	dEtall  = abs(LepTColl.at(0)->Eta()-LepTColl.at(1)->Eta());
	dRll    = LepTColl.at(0)->DeltaR(*LepTColl.at(1));
	dRjj12  =  JetColl.size()<2? -1.:JetColl.at(0).DeltaR(JetColl.at(1));
	dRjj23  =  JetColl.size()<3? -1.:JetColl.at(1).DeltaR(JetColl.at(2));
	dRjj13  =  JetColl.size()<3? -1.:JetColl.at(0).DeltaR(JetColl.at(2));
	dRlj11  =  JetColl.size()<1? -1.:LepTColl.at(0)->DeltaR(JetColl.at(0));
	dRlj12  =  JetColl.size()<2? -1.:LepTColl.at(0)->DeltaR(JetColl.at(1));
	dRlj13  =  JetColl.size()<3? -1.:LepTColl.at(0)->DeltaR(JetColl.at(2));
	dRlj21  =  JetColl.size()<1? -1.:LepTColl.at(1)->DeltaR(JetColl.at(0));
	dRlj22  =  JetColl.size()<2? -1.:LepTColl.at(1)->DeltaR(JetColl.at(1));
	dRlj23  =  JetColl.size()<3? -1.:LepTColl.at(1)->DeltaR(JetColl.at(2));
	
	
	MSSSF   = Mll;
	Mlj11   =  JetColl.size()<1? -1.:(*LepTColl.at(0)+JetColl.at(0)).M();
	Mlj12   =  JetColl.size()<2? -1.:(*LepTColl.at(0)+JetColl.at(1)).M();
	Mlj13   =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(2)).M();
	Mlj21   =  JetColl.size()<1? -1.:(*LepTColl.at(1)+JetColl.at(0)).M();
	Mlj22   =  JetColl.size()<2? -1.:(*LepTColl.at(1)+JetColl.at(1)).M();
	Mlj23   =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(2)).M();
	MTvl1   = MT(*LepTColl.at(0),METv);
	MTvl2   = MT(*LepTColl.at(1),METv);
	Mllj1   =  JetColl.size()<1? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)).M();
	Mllj2   =  JetColl.size()<2? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)).M();
	Mllj3   =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(2)).M();
	Mllj4   = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(3)).M();
	Mlljj12 =  JetColl.size()<2? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(1)).M();
	Mlljj13 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(2)).M();
	Mlljj14 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(3)).M();
	Mlljj23 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)+JetColl.at(2)).M();
	Mlljj24 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)+JetColl.at(3)).M();
	Mlljj34 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(2)+JetColl.at(3)).M();
	Mljj112 =  JetColl.size()<2? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(1)).M();
	Mljj113 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(2)).M();
	Mljj114 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(3)).M();
	//return;                                                                                                                                  
	Mljj123 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(1)+JetColl.at(2)).M();
	Mljj124 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(1)+JetColl.at(3)).M();
	Mljj134 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(2)+JetColl.at(3)).M();
	Mljj212 =  JetColl.size()<2? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(1)).M();
	Mljj213 =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(2)).M();
	Mljj214 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(3)).M();
	Mljj223 =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(1)+JetColl.at(2)).M();
	Mljj224 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(1)+JetColl.at(3)).M();
	Mljj234 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(2)+JetColl.at(3)).M();
	Mjj12   =  JetColl.size()<2? -1.:(JetColl.at(0)+JetColl.at(1)).M();
	Mjj13   =  JetColl.size()<3? -1.:(JetColl.at(0)+JetColl.at(2)).M();
	Mjj14   = JetColl.size()<4? -1.:(JetColl.at(0)+JetColl.at(3)).M();
	Mjj23   =  JetColl.size()<3? -1.:(JetColl.at(1)+JetColl.at(2)).M();
	Mjj24   = JetColl.size()<4? -1.:(JetColl.at(1)+JetColl.at(3)).M();
	Mjj34   = JetColl.size()<4? -1.:(JetColl.at(2)+JetColl.at(3)).M();
	
	//Vars requiring complex algo.                                                                                                             
	HT      = 0;
	for(unsigned int itj=0; itj<JetColl.size(); itj++){ HT+=JetColl.at(itj).Pt(); }
	
	HTLT=HT/LT;
	HTLT1=HT/LepTColl.at(0)->Pt();
	HTLT2=HT/LepTColl.at(1)->Pt();
	
	std::vector<FatJet> FatJetColl;
	
	double ST = GetST( LepTColl, JetColl, FatJetColl, METv);
	
	MET2HT  = JetColl.size()<1? -1.:pow(MET,2.)/HT;
	MET2ST  = pow(MET,2.)/ST;
	
	const float MW = 80.379;
	float dijetmass_tmp=9999.;
	float dijetmass=99990000.;
	int m=-999;
	int n=-999;
	
	for(UInt_t emme=0; emme<JetColl.size(); emme++){
	  for(UInt_t enne=emme+1; enne<JetColl.size(); enne++) {
	    
	    dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();
	    //if(emme == enne) continue;                                                                                                           
	    
	    if ( fabs(dijetmass_tmp-MW) < fabs(dijetmass-MW) ) {
	      dijetmass = dijetmass_tmp;
	      m = emme;
	      n = enne;
	    }
	  }
	}
	
	PtWj1     = JetColl.size() > 1 ? JetColl[m].Pt() : -1.;
	PtWj2     = JetColl.size() > 1 ? JetColl[n].Pt() : -1.;
	dRWjj     = JetColl.size() > 1 ? JetColl[m].DeltaR(JetColl[n]) : -1.;
	dRlW12    = JetColl.size() > 1 ? LepTColl.at(0)->DeltaR(JetColl[m] + JetColl[n]) : -1.;
	dRlW22    = JetColl.size() > 1 ? LepTColl.at(1)->DeltaR(JetColl[m] + JetColl[n]) : -1.;
	M_W2_jj   = JetColl.size() > 1 ? (JetColl[m] + JetColl[n]).M() : -1.;
	M_W1_lljj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(0) + *LepTColl.at(1)).M() : -1.;
	M_N1_l1jj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(0)).M() : -1.;
	M_N2_l2jj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(1)).M() : -1.;
	//for(unsigned int im=0; im<MNStrList.size(); im++){                                                                                
	
	
	vector<TString> MassesBDT = {"100","250","500"};
	
	for(auto mN : MassesBDT){
	  TString FileName ="DYTypeI_"+GetChannelString(dilep_channel)+  "_M"+mN+"_Mode0_Run2_BDT.weights.xml";
	  TString MVATagStr = "BDTG_M"+mN+"_"+GetChannelString(dilep_channel);
	  
	  float MVAvalue = MVAReader->EvaluateMVA(MVATagStr);
	  
	  TString MVABin = "MN"+mN+"_";
	  if(MVAvalue< 0.) MVABin = "MN"+mN+"_SR3_bin1";
	  else if(MVAvalue< 0.2) MVABin = "MN"+mN+"_SR3_bin2";
	  else MVABin = "MN"+mN+"_SR3_bin3";
	  

	  for(auto iel  : electrons){
	    double pt = (iel.Pt() > 200.) ? 199. : iel.Pt();
	    	    
	    if (!param.Name.Contains("HNOpt")){
	      FillHist( "NonOpt/"+MVABin+"_"+Category(iel)+"Pt_"+param.Name , 1., weight_channel, 2., 0.,2);
	      
	      continue;
	    }
	  		
	  
	    if(iel.PassID("HNTightV2")) {
	      TString lab = "_HNTightV2";
	      FillHist( "NonOpt/"+MVABin+"_RegionPass_"+Category(iel)+"Pt_"+param.Name+lab ,  1., weight_channel, 2., 0.,2);
	      OutCutFlow( "NonOpt/"+MVABin+"_RegionPass_"+Category(iel)+"Pt_"+param.Name+lab , weight_channel);

	      
	    }
	    
	    
	    if(iel.passMVAID_Iso_WP90()){
	      if(iel.IsGsfCtfScPixChargeConsistent()){
		FillHist( "ISOOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"Pt_Iso_"+param.Name  , pt , iel.RelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
		FillHist( "ISOOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"Pt_MiniIso_"+param.Name  , pt , iel.MiniRelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
		if(iel.RelIso() <0.15){
		  FillHist( "IPOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"Pt_IP_"+param.Name  , pt , fabs(iel.IP3D()/iel.IP3Derr()) , weight_channel, nPtbins,  ptbins , nIPbins,IPbins );
		  FillHist( "IPOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"Pt_DXY_"+param.Name  , pt , fabs(iel.dXY()) , weight_channel, nPtbins,  ptbins , nDXYbins,DXYbins );
		  FillHist( "IPOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"Pt_DZ_"+param.Name  , pt , fabs(iel.dZ()) , weight_channel, nPtbins,  ptbins , nDZbins,DZbins );
		  FillHist( "CBOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"MissingHits_"+param.Name, iel.NMissingHits() , weight_channel, 5, 0., 5.);
		  FillHist( "CBOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"sigmaIetaIeta_"+param.Name, iel.Full5x5_sigmaIetaIeta() , weight_channel, 1000, 0., 0.1);
		  FillHist( "CBOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"dEtaSeed_"+param.Name, fabs(iel.dEtaSeed()) , weight_channel, 1000, 0., 0.01);
		  FillHist( "CBOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"dPhiIn_"+param.Name, fabs(iel.dPhiIn()) , weight_channel, 1000, 0., 1.);
		  FillHist( "CBOpt/"+MVABin+"_MVAIso90_"+Category(iel)+"InvEminusInvP_"+param.Name, fabs(iel.InvEminusInvP()) , weight_channel, 1000, 0., 1.);

		}
	      }
	    }
	    if(iel.passMVAID_noIso_WP90()){
	      if(iel.IsGsfCtfScPixChargeConsistent()){
		FillHist( "ISOOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"Pt_Iso_"+param.Name  , pt , iel.RelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
		FillHist( "ISOOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"Pt_MiniIso_"+param.Name  , pt , iel.MiniRelIso(), weight_channel, nPtbins,  ptbins , nIsobins,Isobins );
		if(iel.RelIso() <0.15){
		  FillHist( "IPOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"Pt_IP_"+param.Name  , pt , fabs(iel.IP3D()/iel.IP3Derr()) , weight_channel, nPtbins,  ptbins , nIPbins,IPbins );
		  FillHist( "IPOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"Pt_DXY_"+param.Name  , pt , fabs(iel.dXY()) , weight_channel, nPtbins,  ptbins , nDXYbins,DXYbins );
		  FillHist( "IPOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"Pt_DZ_"+param.Name  , pt , fabs(iel.dZ()) , weight_channel, nPtbins,  ptbins , nDZbins,DZbins );
		  FillHist( "CBOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"MissingHits_"+param.Name, iel.NMissingHits() , weight_channel, 5, 0., 5.);
                  FillHist( "CBOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"sigmaIetaIeta_"+param.Name, iel.Full5x5_sigmaIetaIeta() , weight_channel, 1000, 0., 0.1);
                  FillHist( "CBOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"dEtaSeed_"+param.Name, fabs(iel.dEtaSeed()) , weight_channel, 1000, 0., 0.01);
                  FillHist( "CBOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"dPhiIn_"+param.Name, fabs(iel.dPhiIn()) , weight_channel, 1000, 0., 1.);
                  FillHist( "CBOpt/"+MVABin+"_MVANoIso90_"+Category(iel)+"InvEminusInvP_"+param.Name, fabs(iel.InvEminusInvP()) , weight_channel, 1000, 0., 1.);

		}
	      }
	    }
	    
	    
	    for (auto IsoPair : IsoPairs){
	      
	      if(iel.RelIso() < IsoPair.first){
		FillHist( "MVAOpt/"+MVABin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		FillHist( "MVAOpt/"+MVABin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
		if(iel.IsGsfCtfScPixChargeConsistent()){
		  FillHist( "MVAOpt/CC_"+MVABin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		  FillHist( "MVAOpt/CC_"+MVABin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
		  OutCutFlow( "MVAOpt/CC_"+MVABin+"_Iso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , weight_channel);

		}
	      }
	      if(iel.MiniRelIso() < IsoPair.first){
		FillHist( "MVAOpt/"+MVABin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		FillHist( "MVAOpt/"+MVABin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
		if(iel.IsGsfCtfScPixChargeConsistent()){
		  FillHist( "MVAOpt/CC_"+MVABin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name  , pt , iel.MVANoIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins );
		  FillHist( "MVAOpt/CC_"+MVABin+"_MiniIso"+IsoPair.second+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name  , pt , iel.MVAIsoResponse(), weight_channel, nPtbins,  ptbins , nMVAbins,mvabins);
		}
	      }
	    }
          }
        }
      }
    }
  }
  
  
  return;
}



void HNL_RegionDefinitionsOpt::RunAllSignalRegions(HNL_LeptonCore::ChargeType qq, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose,std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev,   Particle METv, AnalyzerParameter param,   float weight_ll){


  // Only opt for EE/MM
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu};

  for(auto dilep_channel : channels){
    
    if(run_Debug) cout << "HNL_RegionDefinitionsOpt::RunAllSignalRegions " << GetChannelString(dilep_channel) <<  endl;


    float weight_channel = weight_ll;

    if(MCSample.Contains("Type")) {
      if (!SelectChannel(dilep_channel)) continue;
    }

    
    if (param.Name.Contains("MuOpt") && dilep_channel != MuMu) continue;
    if (param.Name.Contains("ElOpt") && dilep_channel != EE) continue;

    if(run_Debug)cout << "ID Mu  " << param.Muon_Tight_ID << " " << param.Muon_FR_ID << endl;
    if(run_Debug)cout << "ID El  " << param.Electron_Tight_ID << " " << param.Electron_FR_ID << endl;

    if(run_Debug)cout << "ParamName = " << param.Name << endl;
    std::vector<Lepton *> leps       = MakeLeptonPointerVector(muons,electrons,param);

    if(!PassEventTypeFilter(leps, gens)) continue;

    if(!CheckLeptonFlavourForChannel(dilep_channel, leps)) continue;


    // Make channel speciific AnalyzerParameter                                                                                                              
    AnalyzerParameter param_channel = param;

    if(!IsDATA && dilep_channel != MuMu)  weight_channel*= GetElectronSFEventWeight(electrons, param_channel);
    if(!IsDATA && dilep_channel != EE)    weight_channel*= GetMuonSFEventWeight(muons, param_channel);

    FillEventCutflow(HNL_LeptonCore::ChannelDepInc, weight_channel, GetChannelString(dilep_channel) +"_NoCut", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//DEf

    if(!PassMETFilter()) return;

    if(MCSample.Contains("Type")){
      vector<Gen> SigGens = GetGenLepronsSignal();
      if (SigGens.size()==2){

	if(SigGens[0].PID() > 0 && SigGens[1].PID()  > 0 )     FillEventCutflow(HNL_LeptonCore::ChannelDepIncQ, weight_channel, GetChannelString(dilep_channel) +"_MMQ_NoCut", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);// Def
	else if(SigGens[0].PID()  < 0 && SigGens[1].PID()  < 0 )     FillEventCutflow(HNL_LeptonCore::ChannelDepIncQ, weight_channel, GetChannelString(dilep_channel) +"_PPQ_NoCut", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
	
      }
    }
    
    
    TString channel_string = GetChannelString(dilep_channel);
    param_channel.Name =  channel_string + "/" + param_channel.Name; 
    

    std::vector<Lepton *> leps_veto  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);
    
    if(leps.size() ==2)  FillEventCutflow(HNL_LeptonCore::ChannelDepDilep, weight_channel, GetChannelString(dilep_channel) +"_Dilep", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
    

    if(RunCF){
      if(dilep_channel == MuMu) continue;

      if(IsData && SameCharge(leps)) continue;
      if(!IsData && !SameCharge(leps)) continue;
      
      if(IsData)weight_channel = GetCFWeightElectron(leps, param_channel);
      if(IsData)FillWeightHist(param.Name+"/CFWeight",weight_channel);

    }
    else{
      if(!SameCharge(leps)) continue;
    }

    if(RunFake&& IsData){
      
      if(run_Debug)cout << "ID2 Mu  " << param_channel.Muon_Tight_ID << " " << param_channel.Muon_FR_ID << endl;
      if(run_Debug)cout << "ID2 El  " << param_channel.Electron_Tight_ID << " " << param_channel.Electron_FR_ID << endl;

      weight_channel = GetFakeWeight(leps, param_channel, false);

      if(run_Debug)cout << "Fake " << weight_channel << endl;
      FillWeightHist(param_channel.Name+"/FakeWeight",weight_channel);
    }
    
    // Run PRESEL + SR1-3                                                                                                                                                           
    //bool run_ORTrigger = false;

    
    HNL_LeptonCore::SearchRegion LimitRegions = HNL_LeptonCore::MuonSR;
    if (dilep_channel == EE) LimitRegions =HNL_LeptonCore::ElectronSR;
    if (dilep_channel == EMu) LimitRegions =HNL_LeptonCore::ElectronMuonSR;
    HNL_LeptonCore::SearchRegion LimitRegionsQ = HNL_LeptonCore::MuonSRQQ;
    if (dilep_channel == EE) LimitRegionsQ =HNL_LeptonCore::ElectronSRQQ;
    if (dilep_channel == EMu) LimitRegionsQ =HNL_LeptonCore::ElectronMuonSRQQ;


    
    if(!PassPreselection(dilep_channel,qq, leps, leps_veto, TauColl, JetColl, VBF_JetColl, AK8_JetColl, B_JetColl,ev, METv ,param_channel,"", weight_channel)) continue;


    //FillAllMuonPlots("Presel", "Muons"  , muons ,weight_channel);
    //FillAllElectronPlots("Presel", "Electrons"  , electrons ,weight_channel);


    TString  lep_charge =  (leps[0]->Charge() < 0)  ? "QM" :  "QP";

    for(auto iel  : electrons){

    FillHist( "MVAOpt/"+Category(iel)+"Pt_mvaresponse_"+param.Name , iel.Pt() , iel.MVANoIsoResponse(), weight_channel, 1000, 0., 1000.,160, -8., 8.);
    }

    if(AK8_JetColl.size() > 0) {
      TString SRbin= RunSignalRegionAK8String (dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl,ev, METv ,param_channel,"", weight_channel) ;
      for(auto iel  : electrons){
	if(SRbin != "false") FillHist( "MVAOpt/"+SRbin+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , iel.Pt() , iel.MVANoIsoResponse(), weight_channel, 1000, 0., 1000.,160, -8., 8.);
	if(SRbin != "false") FillHist( "MVAOpt/"+SRbin+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name , iel.Pt() , iel.MVAIsoResponse(), weight_channel, 1000, 0., 1000.,160, -8., 8.);

      }
      
      if(SRbin != "false") FillEventCutflow(LimitRegions, weight_channel, SRbin,"LimitInput/"+param.Name);
      if(SRbin != "false") FillEventCutflow(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param.Name);
    }
    else{
      
      TString SRbin = RunSignalRegionWWString( dilep_channel,qq, leps, leps_veto,  TauColl, JetCollLoose, VBF_JetColl,  AK8_JetColl, B_JetColl,ev, METv, param_channel,  "", weight_channel);
      
      
      if(SRbin != "false"){

	for(auto iel  : electrons){
	  if(SRbin != "false") FillHist( "MVAOpt/"+SRbin+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , iel.Pt() , iel.MVANoIsoResponse(), weight_channel, 1000, 0., 1000.,160, -8., 8.);
	  if(SRbin != "false") FillHist( "MVAOpt/"+SRbin+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name , iel.Pt() , iel.MVAIsoResponse(), weight_channel, 1000, 0., 1000.,160, -8., 8.);

	}


	FillEventCutflow(LimitRegions, weight_channel, SRbin,"LimitInput/"+param.Name);
	FillEventCutflow(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param.Name);
      }
      else{
	SRbin  = RunSignalRegionAK4String (dilep_channel,qq, leps, leps_veto, TauColl, JetColl, AK8_JetColl, B_JetColl, ev, METv ,param_channel,"", weight_channel);

	if(SRbin != "false"){
	  
	  for(auto iel  : electrons){
	    if(SRbin != "false") FillHist( "MVAOpt/"+SRbin+"_"+Category(iel)+"Pt_mvaresponse_"+param.Name , iel.Pt() , iel.MVANoIsoResponse(), weight_channel, 1000, 0., 1000.,160, -8., 8.);
	    if(SRbin != "false") FillHist( "MVAOpt/"+SRbin+"_"+Category(iel)+"Pt_Isomvaresponse_"+param.Name , iel.Pt() , iel.MVAIsoResponse(), weight_channel, 1000, 0., 1000.,160, -8., 8.);

	  }


	  FillEventCutflow(LimitRegions, weight_channel, SRbin,"LimitInput/"+param.Name);
	  FillEventCutflow(LimitRegionsQ, weight_channel, lep_charge+SRbin,"LimitInput/"+param.Name);

	}
	else{
	  if(JetColl.size() == 0)Fill_RegionPlots(dilep_channel, 0, param_channel.Name,"FailZeroJet" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, weight_channel,param.WriteOutVerbose);
	  if(JetColl.size() == 1)Fill_RegionPlots(dilep_channel, 0, param_channel.Name,"Fail1Jet" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, weight_channel,param.WriteOutVerbose);
	  if(JetColl.size() > 1)Fill_RegionPlots(dilep_channel, 0, param_channel.Name,"FailDiJet" ,  JetColl, AK8_JetColl, leps,  METv, nPV, weight_channel,param.WriteOutVerbose);
	  
	  
	  if(JetColl.size() == 0) FillEventCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_0j", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
	  else if(JetColl.size() == 1)FillEventCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_1j", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
	  else FillEventCutflow(HNL_LeptonCore::ChannelDepFAILSR3, weight_channel, GetChannelString(dilep_channel) +"_SR3Fail_2j", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
	  
	}
      }
    }
  }
  return;
}




bool  HNL_RegionDefinitionsOpt::PassPreselection(HNL_LeptonCore::Channel channel,HNL_LeptonCore::ChargeType qq, std::vector<Lepton *> leps,std::vector<Lepton *> leps_veto, vector<Tau> TauColl,  std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString TriggerOption ,  float w){

  // ONLY CODE SS PRESLECTION 
  
  // APPLY PT CUTS
  /*
    ee channel 25/10
    emu   25/5
    mm    20/5
    
    THIS IS APPLIED IN PassTriggerSelection
    
   */
  
  if(run_Debug) cout << "HNL_RegionDefinitionsOpt::PassPreselection " << GetChannelString(channel) <<  endl;

  // Make sure events contain 2 leps                                                                                                       
  // Make sure correct leptons are used                                                                                                                                      
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (! (  PassMultiTriggerSelection(channel, ev, leps,"Dilep", "HighPt") || PassMultiTriggerSelection(channel, ev, leps,"Dilep", "Lep"))) return false;
  
  FillEventCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_MultiTrigger", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
  
  if(param.SRConfig.Contains("Trig1OR")){
    if( !(PassMultiTriggerSelection(channel, ev, leps,"Dilep","HighPt")))  return false;
  }
  else if(param.SRConfig.Contains("Trig2OR")){
    if(!(PassMultiTriggerSelection(channel, ev, leps,"Dilep","Lep")))  return false;
  }
  else if(param.SRConfig.Contains("Trig3OR")){
    if( !((PassMultiTriggerSelection(channel, ev, leps,"Dilep","Lep")) || PassMultiTriggerSelection(channel, ev, leps,"Dilep", "HighPt"))) return false;
  }
  else      if (!PassTriggerSelection(channel, ev, leps,"Dilep")) return false;
  
  w *= HNLZvtxSF(channel);

  
  FillEventCutflow(HNL_LeptonCore::ChannelDepTrigger, w, GetChannelString(channel) +"_Trigger", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def


  if(run_Debug) cout << "HNL_RegionDefinitionsOpt::PassTriggerSelection " << GetChannelString(channel) <<  endl;

  
  int nel_hem(0);
  if (channel==EE){
    if (DataEra=="2018"){
      for(auto iel : leps){
	if (iel->Eta() < -1.25){
          if((iel->Phi() < -0.82) && (iel->Phi() > -1.62)) nel_hem++;
	}
      }
    }
  }

  if(nel_hem > 0) return false;


  // Make sure events contain 2 leps
  if (leps_veto.size() != 2) return false;

  
  // CUT ON MASS OF LL PAIR

  Particle ll =  (*leps[0]) + (*leps[1]);
  
  // VETO Z PEAK IN EE CHANNEL
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;

  if(ll.M() < 10) return false; // TO_CHECK: IS 20 BEST OPTION

  // REMOVE 0 Jet EVENTS
  int njets     = JetColl.size() + AK8_JetColl.size() + VBF_JetColl.size();
 

  Fill_RegionPlots(channel, 0, param.Name,"Preselection" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
  FillEventCutflow(HNL_LeptonCore::ChannelDepPresel, w, GetChannelString(channel) +"_Presel", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
  
  return true;
}




bool  HNL_RegionDefinitionsOpt::RunSignalRegionAK8(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  TString SR1String = RunSignalRegionAK8String(channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  
  if(SR1String == "false") return false;
  else return true;
  
}


/// Return TString for Limit bin

TString HNL_RegionDefinitionsOpt::RunSignalRegionAK8String(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq , std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  bool HasTightTau=false;
  for(auto itau: TauColl){
    bool HasTightTau_n=true;
    for (auto ilep : leps) if (ilep->DeltaR(itau) < 0.4) HasTightTau_n=false;
    for (auto ijet : AK8_JetColl) if (ijet.DeltaR(itau) < 1.)  HasTightTau_n=false;
    for (auto ijet : JetColl) if (ijet.DeltaR(itau) < 0.5)  HasTightTau_n=false;
    if(HasTightTau_n) HasTightTau=true;
  }

  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_Init",param.Name,param.WriteOutVerbose);
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";  
  if (leps_veto.size() != 2) return "false";
  FillEventCutflow(HNL_LeptonCore::SR1,w, "SR1_lep_pt",param.Name,param.WriteOutVerbose);
  if(AK8_JetColl.size() == 0) return "false";
  
  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_lep_charge",param.Name,param.WriteOutVerbose);

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return "false";
  
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_dilep_mass",param.Name,param.WriteOutVerbose);

  double ST = GetST(leps, JetColl, AK8_JetColl, METv);
  double HT = GetHT(JetColl, AK8_JetColl);
  double met2_st = pow(METv.Pt(),2.)/ ST;

  double met_cut = 7.;
  if (param.SRConfig.Contains("SR1MET_20_"))   met_cut = 20.;
  if (param.SRConfig.Contains("SR1MET_17_"))   met_cut = 17.;
  if (param.SRConfig.Contains("SR1MET_15_"))   met_cut = 15.;
  if (param.SRConfig.Contains("SR1MET_12_"))   met_cut = 12.;
  if (param.SRConfig.Contains("SR1MET_10_"))   met_cut = 10.;
    
  bool PassHMMet    = (met2_st < met_cut);
  bool PassBJetMVeto = (B_JetColl.size()==0);

  double mass_cut = 500.;

  if (param.SRConfig.Contains("SR1MLLJJ_500_"))   mass_cut = 500.;
  if (param.SRConfig.Contains("SR1MLLJJ_600_"))   mass_cut = 600.;
  if (param.SRConfig.Contains("SR1MLLJJ_700_"))   mass_cut = 700.;
  if (param.SRConfig.Contains("SR1MLLJJ_800_"))   mass_cut = 800.;
  if (param.SRConfig.Contains("SR1MLLJJ_900_"))   mass_cut = 900.;
  if (param.SRConfig.Contains("SR1MLLJJ_1000_"))   mass_cut = 1000.;
 
  double LowerMassSR1WmassCut = mass_cut;
  
  TString signal_region1 = "HNL_SR1";
  
  //double FatJetTau21_SF = GetEventFatJetSF(AK8_JetColl,"ak8_type1", 0);
  //FillWeightHist(param.Name+"/fatjet_ak8_type1_sf_"+param.Name,FatJetTau21_SF);
  //if(!IsData)w*=  FatJetTau21_SF;
  Fill_RegionPlots(channel, 0, param.Name,"InclusiveSR1" , TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
  
  double dPhil2_l = fabs(TVector2::Phi_mpi_pi( ( (AK8_JetColl[0] + *leps[1]).Phi() - (leps[0]->Phi() ))));
  
  double pt_cut = 100.;
  if (param.SRConfig.Contains("SR1PT_80_"))   pt_cut = 80.;
  if (param.SRConfig.Contains("SR1PT_90_"))   pt_cut = 90.;
  if (param.SRConfig.Contains("SR1PT_100_"))   pt_cut = 100.;
  if (param.SRConfig.Contains("SR1PT_110_"))   pt_cut = 110.;
  if (param.SRConfig.Contains("SR1PT_120_"))   pt_cut = 120.;

  
  if(leps[0]->Pt() < pt_cut) return "false";
  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_LepPt",param.Name,param.WriteOutVerbose);

  double dRl2_l2_cut = 2.;//(GetMass(signal_region1,JetColl, AK8_JetColl,  leps) > 1000) ? 2.5 : 1.5;

  if (dPhil2_l > dRl2_l2_cut){

    FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_DphiN_Wlep",param.Name,param.WriteOutVerbose);
    

    if(!HasTightTau) {
      FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_TauVeto",param.Name,param.WriteOutVerbose);
      
      if( ( HT/leps[0]->Pt()) < 4.) {
	FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_HTPt",param.Name,param.WriteOutVerbose);
	
	
	if(GetMass(signal_region1,JetColl, AK8_JetColl,  leps) > LowerMassSR1WmassCut ){
	  
	  
	  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_Wmass",param.Name,param.WriteOutVerbose); 
	  FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_MET",param.Name,param.WriteOutVerbose);
	  if(PassHMMet&&PassBJetMVeto)FillEventCutflow(HNL_LeptonCore::SR1, w, "SR1_bveto",param.Name,param.WriteOutVerbose);
	  
	  
	  if(PassHMMet&&PassBJetMVeto) {
	    
	    
	    //Fill Limit plot
	    
	    float dijetmass_tmp=999.;
	    float dijetmass=9990000.;
	    
	    int m=-999;
	    for(UInt_t emme=0; emme<AK8_JetColl.size(); emme++){
	      dijetmass_tmp = AK8_JetColl[emme].SDMass();
	      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
		dijetmass = dijetmass_tmp;
		m = emme;
	      }
	    }
	    
	    Particle N1cand = AK8_JetColl[m] + *leps[0];
	    
	    int nSRbins=8;
	    double ml1jbins[nSRbins] = { 0., 200., 400.,500., 600.,700., 1000., 2000.};
	    double Qml1jbins[13] = {-2000., -1000., -500., -300., -200., -100,  0., 100.,200.,300.,500., 1000., 2000.};
	    
	    double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();
	    
	    if(MN1 > 600 && leps[0]->Pt() < 140)   return "false";
	    
	    if(param.WriteOutVerbose == 0)FillHist( "LimitSR1/"+param.Name+"/N1Mass_Central",  MN1,  w, 7, ml1jbins, "Reco M_{l1jj}");
	    if(param.WriteOutVerbose == 0 )FillHist( "LimitSR1/"+param.Name+"/Q_N1Mass_Central",  leps[0]->Charge()*MN1,  w, 12, Qml1jbins, "Reco M_{l1jj}");
	    
	    //Fill_RegionPlots      (channel, true, hist_label+"/SR1/" , "", JetColl, AK8_JetColl, leps,  METv, nPV, w);
	    
	    FillEventCutflow(HNL_LeptonCore::ChannelDepSR1, w, GetChannelString(channel) +"_SR1", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def
	    
	    for(unsigned int ibin=1; ibin < nSRbins; ibin++){
	      if(MN1 < ml1jbins[ibin]) return "SR1_bin"+to_string(ibin);
	    }
	    
	    return "true";
	  }
	}
      }
    }
  }
  return "false";
}




TString HNL_RegionDefinitionsOpt::RunSignalRegionAK4StringBDT(TString mN, HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> LepTColl,   std::vector<Lepton *> leps_veto, std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<Jet> JetVBFColl, std::vector<Jet> B_JetColl, Event  ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){



  if(!CheckLeptonFlavourForChannel(channel, LepTColl)) return "false";
  FillEventCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_lep_pt",param.Name,param.WriteOutVerbose);

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus && LepTColl[0]->Charge() < 0) return "false";
  if(qq==Minus && LepTColl[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_lep_charge",param.Name,param.WriteOutVerbose);

  Particle ll =  (*LepTColl[0]) + (*LepTColl[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return "false";

  FillEventCutflow(HNL_LeptonCore::SR3BDT, w, "SR3_dilep_mass",param.Name,param.WriteOutVerbose);
  float Mll = GetLLMass(LepTColl);

  Nj      = JetColl.size();
  Nvbfj   = JetVBFColl.size();
  Ptl1    = LepTColl[0]->Pt();
  Ptl2    = LepTColl.at(1)->Pt();
  LT      = GetLT(LepTColl);
  Ptj1    =  JetColl.size()<1? -1.: JetColl.at(0).Pt();
  Ptj2    =  JetColl.size()<2? -1.:JetColl.at(1).Pt();
  Ptj3    =  JetColl.size()<3? -1.:JetColl.at(2).Pt();
  MET     = METv.Pt();

  dEtall  = abs(LepTColl.at(0)->Eta()-LepTColl.at(1)->Eta());
  dRll    = LepTColl.at(0)->DeltaR(*LepTColl.at(1));
  dRjj12  =  JetColl.size()<2? -1.:JetColl.at(0).DeltaR(JetColl.at(1));
  dRjj23  =  JetColl.size()<3? -1.:JetColl.at(1).DeltaR(JetColl.at(2));
  dRjj13  =  JetColl.size()<3? -1.:JetColl.at(0).DeltaR(JetColl.at(2));
  dRlj11  =  JetColl.size()<1? -1.:LepTColl.at(0)->DeltaR(JetColl.at(0));
  dRlj12  =  JetColl.size()<2? -1.:LepTColl.at(0)->DeltaR(JetColl.at(1));
  dRlj13  =  JetColl.size()<3? -1.:LepTColl.at(0)->DeltaR(JetColl.at(2));
  dRlj21  =  JetColl.size()<1? -1.:LepTColl.at(1)->DeltaR(JetColl.at(0));
  dRlj22  =  JetColl.size()<2? -1.:LepTColl.at(1)->DeltaR(JetColl.at(1));
  dRlj23  =  JetColl.size()<3? -1.:LepTColl.at(1)->DeltaR(JetColl.at(2));



  MSSSF   = Mll;
  Mlj11   =  JetColl.size()<1? -1.:(*LepTColl.at(0)+JetColl.at(0)).M();
  Mlj12   =  JetColl.size()<2? -1.:(*LepTColl.at(0)+JetColl.at(1)).M();
  Mlj13   =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(2)).M();
  Mlj21   =  JetColl.size()<1? -1.:(*LepTColl.at(1)+JetColl.at(0)).M();
  Mlj22   =  JetColl.size()<2? -1.:(*LepTColl.at(1)+JetColl.at(1)).M();
  Mlj23   =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(2)).M();
  MTvl1   = MT(*LepTColl.at(0),METv);
  MTvl2   = MT(*LepTColl.at(1),METv);
  Mllj1   =  JetColl.size()<1? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)).M();
  Mllj2   =  JetColl.size()<2? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)).M();
  Mllj3   =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(2)).M();
  Mllj4   = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(3)).M();
  Mlljj12 =  JetColl.size()<2? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(1)).M();
  Mlljj13 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(2)).M();
  Mlljj14 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(3)).M();
  Mlljj23 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)+JetColl.at(2)).M();
  Mlljj24 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)+JetColl.at(3)).M();
  Mlljj34 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(2)+JetColl.at(3)).M();
  Mljj112 =  JetColl.size()<2? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(1)).M();
  Mljj113 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(2)).M();
  Mljj114 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(3)).M();
  //return;                                                                                                                                                                                                                                                                                                                 
  Mljj123 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(1)+JetColl.at(2)).M();
  Mljj124 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(1)+JetColl.at(3)).M();
  Mljj134 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(2)+JetColl.at(3)).M();
  Mljj212 =  JetColl.size()<2? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(1)).M();
  Mljj213 =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(2)).M();
  Mljj214 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(3)).M();
  Mljj223 =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(1)+JetColl.at(2)).M();
  Mljj224 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(1)+JetColl.at(3)).M();
  Mljj234 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(2)+JetColl.at(3)).M();
  Mjj12   =  JetColl.size()<2? -1.:(JetColl.at(0)+JetColl.at(1)).M();
  Mjj13   =  JetColl.size()<3? -1.:(JetColl.at(0)+JetColl.at(2)).M();
  Mjj14   = JetColl.size()<4? -1.:(JetColl.at(0)+JetColl.at(3)).M();
  Mjj23   =  JetColl.size()<3? -1.:(JetColl.at(1)+JetColl.at(2)).M();
  Mjj24   = JetColl.size()<4? -1.:(JetColl.at(1)+JetColl.at(3)).M();
  Mjj34   = JetColl.size()<4? -1.:(JetColl.at(2)+JetColl.at(3)).M();

  //Vars requiring complex algo.                                                                                                                                                                                                                                                                                            
  HT      = 0;
  for(unsigned int itj=0; itj<JetColl.size(); itj++){ HT+=JetColl.at(itj).Pt(); }

  HTLT=HT/LT;
  HTLT1=HT/LepTColl.at(0)->Pt();
  HTLT2=HT/LepTColl.at(1)->Pt();

  std::vector<FatJet> FatJetColl;

  double ST = GetST( LepTColl, JetColl, FatJetColl, METv);

  MET2HT  = JetColl.size()<1? -1.:pow(MET,2.)/HT;
  MET2ST  = pow(MET,2.)/ST;

  const float MW = 80.379;
  float dijetmass_tmp=9999.;
  float dijetmass=99990000.;
  int m=-999;
  int n=-999;

  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=emme+1; enne<JetColl.size(); enne++) {

      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();
      //if(emme == enne) continue;                                                                                                                                                                                                                                                                                          

      if ( fabs(dijetmass_tmp-MW) < fabs(dijetmass-MW) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
        n = enne;
      }
    }
  }

  PtWj1     = JetColl.size() > 1 ? JetColl[m].Pt() : -1.;
  PtWj2     = JetColl.size() > 1 ? JetColl[n].Pt() : -1.;
  dRWjj     = JetColl.size() > 1 ? JetColl[m].DeltaR(JetColl[n]) : -1.;
  dRlW12    = JetColl.size() > 1 ? LepTColl.at(0)->DeltaR(JetColl[m] + JetColl[n]) : -1.;
  dRlW22    = JetColl.size() > 1 ? LepTColl.at(1)->DeltaR(JetColl[m] + JetColl[n]) : -1.;
  M_W2_jj   = JetColl.size() > 1 ? (JetColl[m] + JetColl[n]).M() : -1.;
  M_W1_lljj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(0) + *LepTColl.at(1)).M() : -1.;
  M_N1_l1jj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(0)).M() : -1.;
  M_N2_l2jj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(1)).M() : -1.;

  //for(unsigned int im=0; im<MNStrList.size(); im++){                                                                                                                                                                                                                                                                      

  TString FileName ="DYTypeI_"+GetChannelString(channel)+  "_M"+mN+"_Mode0_Run2_BDT.weights.xml";
  TString MVATagStr = "BDTG_M"+mN+"_"+GetChannelString(channel);

  float MVAvalue = MVAReader->EvaluateMVA(MVATagStr);

  FillHist("LimitSR3BDT/"+param.Name+"/SignalBins_M"+mN, MVAvalue, w, 40, -1., 1.);
  if(MVAvalue < 0.) return "SR3_bin1";
  else if(MVAvalue< 0.05) return "SR3_bin2";
  else if(MVAvalue< 0.10) return "SR3_bin3";
  else if(MVAvalue< 0.15) return "SR3_bin4";
  else if(MVAvalue< 0.2) return "SR3_bin5";
  else if(MVAvalue< 0.25) return "SR3_bin6";
  else if(MVAvalue< 0.3) return "SR3_bin7";
  else if(MVAvalue< 0.35) return "SR3_bin8";
  else  return "SR3_bin9";



  return "true";
}







bool  HNL_RegionDefinitionsOpt::RunSignalRegionWW(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){

  TString SR2String = RunSignalRegionWWString(channel, qq, leps, leps_veto, TauColl,JetCollLoose, JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR2String == "false") return false;
  else return true;
  
}



TString HNL_RegionDefinitionsOpt::RunSignalRegionWWString(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (leps_veto.size() != 2) return "false";


  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::SR2,w, "SR2_lep_charge",param.Name,param.WriteOutVerbose);

  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_lep_pt",param.Name,param.WriteOutVerbose);

  Fill_RegionPlots(channel, 0, param.Name,"InclusiveSR2" ,  TauColl, JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);

  if (param.SRConfig.Contains("VBFLeadJet")){

    if (param.SRConfig.Contains("MJJ450")){
      if(!PassVBF(JetColl,leps,450., true)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ500")){
      if(!PassVBF(JetColl,leps,500., true)) return "false";
    }   
    else if (param.SRConfig.Contains("MJJ600")){
      if(!PassVBF(JetColl,leps,600., true)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ750")){
      if(!PassVBF(JetColl,leps,750., true)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ800")){
      if(!PassVBF(JetColl,leps,800., true)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ850")){
      if(!PassVBF(JetColl,leps,850., true)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ900")){
      if(!PassVBF(JetColl,leps,900., true)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ1000")){
      if(!PassVBF(JetColl,leps,1000., true)) return "false";
    }
    else if(!PassVBF(JetColl,leps,750., true)) return "false";

  }
  else   if (param.SRConfig.Contains("VBFAwayJet")){

    if (param.SRConfig.Contains("MJJ450")){
      if(!PassVBF(JetColl,leps,450., false)) return "false";
    }   
    else if (param.SRConfig.Contains("MJJ500")){
      if(!PassVBF(JetColl,leps,500., false)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ600")){
      if(!PassVBF(JetColl,leps,600., false)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ750")){
      if(!PassVBF(JetColl,leps,750., false)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ800")){
      if(!PassVBF(JetColl,leps,800., false)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ850")){
      if(!PassVBF(JetColl,leps,850., false)) return "false";
    }

    else if (param.SRConfig.Contains("MJJ900")){
      if(!PassVBF(JetColl,leps,900., false)) return "false";
    }
    else if (param.SRConfig.Contains("MJJ1000")){
      if(!PassVBF(JetColl,leps,1000., false)) return "false";
    }

    else if(!PassVBF(JetColl,leps,750., false)) return "false";
    
  }

  else       if(!PassVBF(JetColl,leps,750., true)) return "false";


  FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_VBF",param.Name,param.WriteOutVerbose);

  bool PassBJetMVeto = (B_JetColl.size()==0);

  double HT(0.);
  if (param.SRConfig.Contains("LooseJet")){
    for(UInt_t emme=0; emme<JetCollLoose.size(); emme++){
      HT += JetColl[emme].Pt();
    }
  }
  else   if (param.SRConfig.Contains("tightJet")){

    for(UInt_t emme=0; emme<JetColl.size(); emme++){
      HT += JetColl[emme].Pt();
    }
  }
  else{
    for(UInt_t emme=0; emme<JetCollLoose.size(); emme++){
      HT += JetColl[emme].Pt();
    }
  }

  double htltcut = 2.;
  
  if (param.SRConfig.Contains("HTLT10_")) htltcut = 1.;
  if (param.SRConfig.Contains("HTLT11_")) htltcut = 1.1;
  if (param.SRConfig.Contains("HTLT12_")) htltcut = 1.2;
  if (param.SRConfig.Contains("HTLT13_")) htltcut = 1.3;
  if (param.SRConfig.Contains("HTLT14_")) htltcut = 1.4;
  if (param.SRConfig.Contains("HTLT15_")) htltcut = 1.5;
  if (param.SRConfig.Contains("HTLT16_")) htltcut = 1.6;
  if (param.SRConfig.Contains("HTLT17_")) htltcut = 1.7;
  if (param.SRConfig.Contains("HTLT18_")) htltcut = 1.8;
  if (param.SRConfig.Contains("HTLT19_")) htltcut = 1.9;
  if (param.SRConfig.Contains("HTLT20_")) htltcut = 2.;


  if (HT/leps[0]->Pt() < htltcut){
    
    FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_ht_lt1",param.Name,param.WriteOutVerbose);

    if(PassBJetMVeto){

      FillEventCutflow(HNL_LeptonCore::SR2, w, "SR2_bveto",param.Name,param.WriteOutVerbose);

      double HTLT[3] = { 0.,1., 2};
      double QHTLT[5] = { -2.,-1.,0.,1., 2};
      if(param.WriteOutVerbose == 0)FillHist( "LimitSR2/"+param.Name+"/HT_LT1_Central",  HT/leps[0]->Pt(),  w, 2, HTLT, "Reco HT/LT1");
      if(param.WriteOutVerbose == 0)FillHist( "LimitSR2/"+param.Name+"/Q_HT_LT1_Central",  (leps[0]->Charge()*HT)/leps[0]->Pt(),  w, 4 , QHTLT, "Reco HT/LT1 * Q");

      FillEventCutflow(HNL_LeptonCore::ChannelDepSR2, w, GetChannelString(channel) +"_SR2", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def


      if (HT/leps[0]->Pt() < 1)      return "SR2_bin1";
      return "SR2_bin2";
    }
  }

  return "false";
}



bool  HNL_RegionDefinitionsOpt::RunSignalRegionAK4(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){



  TString SR3String = RunSignalRegionAK4String(channel, qq, leps, leps_veto, TauColl,JetColl,AK8_JetColl,B_JetColl,ev, METv, param, PostLabel,w);
  if(SR3String == "false") return false;
  else return true;

}


TString HNL_RegionDefinitionsOpt::RunSignalRegionAK4String(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Tau> TauColl,std::vector<Jet> JetColl, std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){

                                        
  if(!CheckLeptonFlavourForChannel(channel, leps)) return "false";
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_pt",param.Name,param.WriteOutVerbose);

  if (leps_veto.size() != 2) return "false";

  if(qq==Plus && leps[0]->Charge() < 0) return "false";
  if(qq==Minus && leps[0]->Charge() > 0) return "false";

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_lep_charge",param.Name,param.WriteOutVerbose);

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return "false";

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_dilep_mass",param.Name,param.WriteOutVerbose);

  double ST = GetST(leps, JetColl, AK8_JetColl, METv);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 15);

  double LowerMassSR3WmassCut = 50.;   double UpperMassSR3WmassCut = 120.;
  
  int  NB_JetColl      =  B_JetColl.size();

  double pt_bin1= 80.;
  if (param.SRConfig.Contains("SR3_bin1pt_60_")) pt_bin1= 60.;
  if (param.SRConfig.Contains("SR3_bin1pt_70_")) pt_bin1= 70.;
  if (param.SRConfig.Contains("SR3_bin1pt_80_")) pt_bin1= 80.;
  if (param.SRConfig.Contains("SR3_bin1pt_90_")) pt_bin1= 90.;
  if (param.SRConfig.Contains("SR3_bin1pt_100_")) pt_bin1= 100.;
  if (param.SRConfig.Contains("SR3_bin1pt_120_")) pt_bin1= 120.;

  double pt_bin2= 80.;
  if (param.SRConfig.Contains("SR3_bin2pt_60_")) pt_bin2= 60.;
  if (param.SRConfig.Contains("SR3_bin2pt_70_")) pt_bin2= 70.;
  if (param.SRConfig.Contains("SR3_bin2pt_80_")) pt_bin2= 80.;
  if (param.SRConfig.Contains("SR3_bin2pt_90_")) pt_bin2= 90.;
  if (param.SRConfig.Contains("SR3_bin2pt_100_")) pt_bin2= 100.;
  if (param.SRConfig.Contains("SR3_bin2pt_120_")) pt_bin2= 120.;



  if(NB_JetColl==0 && PassHMMet){
    if(JetColl.size()==0)     Fill_RegionPlots(channel, 0, param.Name,"ZeroJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
    else if(JetColl.size()==1)     Fill_RegionPlots(channel, 0, param.Name,"OneJetSR3" ,  TauColl,JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);
  }
  
  if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 0 && leps[1]->Pt() > pt_bin1) {
    if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBins",   0.5, w, 16, 0, 16.,  "Signalbins");
    if(param.WriteOutVerbose == 0)    FillHist( "LimitSR3/"+param.Name+"/SignalBinsQ",   0.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    return "SR3_bin1";
  }
  if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 1 && leps[1]->Pt() > pt_bin2) {
    if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");
    if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBinsQ",   1.5*leps[0]->Charge(), w, 32, -16, 16., "Signalbins");                                       
    return "SR3_bin2";
  }
  
  //if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 0 && leps[1]->Pt() > 80.) FillHist( "LimitSR3/"+param.Name+"/SignalBinsQ",   0.5*leps[0]->Charge(), w, 28, -14, 14., "Signalbins");
  //if((NB_JetColl==0 && PassHMMet) && JetColl.size() == 1 && leps[1]->Pt() > 80.) FillHist( "LimitSR3/"+param.Name+"/SignalBinsQ",   1.5*leps[0]->Charge(), w, 28, -14, 14., "Signalbins");


  if(JetColl.size() >0)FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_jet",param.Name,param.WriteOutVerbose);

  //Fill_RegionPlots      (channel, true, param.Name+"/SR3Initial/" , "", JetColl, AK8_JetColl, leps,  METv, nPV, w);
  if(JetColl.size() < 2) return "false";


  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_dijet",param.Name,param.WriteOutVerbose);

  if(!(GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) < UpperMassSR3WmassCut && GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) > LowerMassSR3WmassCut)) {
    return "false";
  }
                                                           
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_Wmass",param.Name,param.WriteOutVerbose);

  if(JetColl[0].Pt() < 25.)  return "false";
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_J1Pt",param.Name,param.WriteOutVerbose);
   
  if(!PassHMMet) {
    return "false";
  }
  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_MET",param.Name,param.WriteOutVerbose);


  if(NB_JetColl>  0){
    return "false";
  }


  Fill_RegionPlots(channel, 0, param.Name,"DiJetSR3" ,TauColl,  JetColl, AK8_JetColl, leps,  METv, nPV, w,param.WriteOutVerbose);

  FillEventCutflow(HNL_LeptonCore::SR3, w, "SR3_bveto",param.Name,param.WriteOutVerbose);

  //Fill_RegionPlots      (channel, true, param.Name+"/SR3/" , "", JetColl, AK8_JetColl, leps,  METv, nPV, w);
  //FIll Limit 

  double bin = 0;
  
  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=1; enne<JetColl.size(); enne++) {

      if(emme == enne) continue;      
      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();

      
      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
        n = enne;
      }
    }
  }
  
  Particle N1cand = JetColl[m] + JetColl[n]+ *leps[0];
  Particle N2cand = JetColl[m] + JetColl[n]+ *leps[1];
  double dRl2JJ = leps[1]->DeltaR(JetColl[m] + JetColl[n]);

  TString sbin="";
  
  if(leps[1]->Pt() < 25) {
    if (met2_st < 9 && N2cand.M() < 100.&& dRl2JJ < 3.1) {
      bin=2.5;
      sbin="3";
    }
    else{
      bin=3.5;
      sbin="4";
    }
    sbin="3";

  }
  else if(leps[1]->Pt() < 60) {
 
    if (met2_st < 9  && dRl2JJ < 3.1){
      
      if(N2cand.M() < 100.) { 
	bin=4.5;
	sbin="5";
      }
      else if(N2cand.M() < 125.) {
	bin=5.5;
	sbin="6";
      }
      else if(N2cand.M() < 150.) {
	bin=6.5;
	sbin="7";
      }
      else if(N2cand.M() < 175.) {
	bin=7.5;
	sbin="8";
      }
      else if(N2cand.M() < 200.) {
	bin=8.5;
	sbin="9";
      }
      else {
	bin=9.5;
	sbin="10";
      }
    }
    else {
      bin=9.5;
      sbin="10";
    }

    sbin="4";
  }
  else{
    if(N1cand.M() < 200) {
      bin=10.5;
      sbin="11";
    }
    else  if(N1cand.M() < 250.) {
      bin=11.5;
      sbin="12";
    }
    else if(N1cand.M() < 300.) {
      bin=12.5;
      sbin="13";
    }
    else if(N1cand.M() < 500.) {
      bin=13.5;
      sbin="14";
      }
    else if(N1cand.M() < 1000.) {
      bin=14.5;
	sbin="15";
    }
    else {
      bin = 15.5;
      sbin="16";
    }
    if(N1cand.M() < 500.) sbin="5";
    else sbin="6";
  }
  

  if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBins",   bin, w, 16, 0, 16., "Signalbins");

  double qbin = bin*double(leps[0]->Charge());
  if(param.WriteOutVerbose == 0)FillHist( "LimitSR3/"+param.Name+"/SignalBinsQ",   qbin, w, 32, -16, 16, "Signalbins");
  
  TString signal_region = "HNL_SR3";
  
  FillEventCutflow(HNL_LeptonCore::ChannelDepSR3, w, GetChannelString(channel) +"_SR3", "ChannelCutFlow/"+param.Name,param.WriteOutVerbose);//Def

  return "SR3_bin"+sbin;
  
}



bool HNL_RegionDefinitionsOpt::RunSignalRegionTrilepton(HNL_LeptonCore::Channel channel,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto ,  std::vector<Jet> JetColl ,std::vector<FatJet>  AK8_JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param, TString PostLabel ,  float w){
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 3) return false;
  
  FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep",param.Name);
  
  // If running fake bkg now set weight to Fake weight
  //if(RunFake)   w= GetFakeWeightMuon(leps, param);
  if(RunFake)   FillWeightHist(param.Name+"/FakeWeight_"+param.Name,w);

  int n_veto_lep  = leps_veto.size();
  
  bool pass_sr1(true);

  if(n_veto_lep > 3) pass_sr1=false;

  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_veto",param.Name);

 
 
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_chargereq",param.Name);

  int  NB_JetColl      = B_JetColl.size();

  if (NB_JetColl > 0)pass_sr1=false;
  
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_bjet",param.Name);
  
  if(ZmasslllWindowCheck(leps)) pass_sr1=false;
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_Zmlll",param.Name);
  if(ZmassOSWindowCheck(leps)) pass_sr1=false;
  if(pass_sr1)FillEventCutflow(HNL_LeptonCore::SR4,w, "SR4_3lep_Zmll_os",param.Name);

  
  if(pass_sr1)  {
    // For Cut flow comparison
    //PrintEvent(param,"SR4_"+PostLabel,w);    
    // For event cut flow
    FillEventCutflow(HNL_LeptonCore::SR4, w, "SR4_lll_mu",param.Name);
    // Make histograms generically
    //    Fill_RegionPlots(channel_lll,true,"HNL_SR4_lll" , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
    // make limit inout plots
    //Fill_SigRegionPlots4(channel_lll, param.Name, JetColl,  AK8_JetColl, leps,  METv, nPV, w);

    FillEventCutflow(SR,w, "SR4Pass",param.Name);

  }

  if(!pass_sr1)     FillEventCutflow(SR,w, "SR4Fail",param.Name);

  return pass_sr1;
 
  
}



HNL_RegionDefinitionsOpt::HNL_RegionDefinitionsOpt(){
      


}
 
HNL_RegionDefinitionsOpt::~HNL_RegionDefinitionsOpt(){

}




bool HNL_RegionDefinitionsOpt::PassVBFInitial(vector<Jet>  JetColl){

  if(JetColl.size() < 2) return false;

  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < JetColl.size()-1; ij++){
    for(unsigned int ij2 = ij+1; ij2 < JetColl.size(); ij2++){

      double deta = fabs(JetColl[ij].Eta() - JetColl[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }

  if(maxDiJetDeta < 2.5) return false;
  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  if(JJ.M() < 450.) return false;

  return true;


}

bool HNL_RegionDefinitionsOpt::PassVBF(vector<Jet>  JetColl,std::vector<Lepton *> leps, double mjj_cut ,bool use_leadjets){

  if(leps.size() != 2) return false;
  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi < 2.) return false;
  if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return false;
  if(JetColl.size() < 2) return false;

  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < JetColl.size()-1; ij++){
    for(unsigned int ij2 = ij+1; ij2 < JetColl.size(); ij2++){

      double deta = fabs(JetColl[ij].Eta() - JetColl[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }
  if(use_leadjets){ijet1=0;ijet2=1;}
  
  if(maxDiJetDeta < 2.5) return false;
  Particle JJ = JetColl[ijet1] + JetColl[ijet2];
  if(JJ.M() < mjj_cut) return false;
  double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;

  if(zeppenfeld > 0.75) return false;

  return true;
}





void HNL_RegionDefinitionsOpt::RunAllControlRegions(std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, std::vector<Jet> JetColl, std::vector<Jet> VBF_JetColl,   std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param,  float weight_ll ){
  
  vector<HNL_LeptonCore::Channel> channels = {EE,MuMu, EMu};
  vector<HNL_LeptonCore::Channel> channels_lll = {EEE,MuMuMu, EMuL};
  vector<HNL_LeptonCore::Channel> channels_llll = {EEEE,MuMuMuMu, EMuLL};

  if(!PassMETFilter()) return;
 
  //cout << "----------------   RunAllControlRegions [Tight]  ----------------------------------------------------------------------------------------------------------------" << endl;
  std::vector<Lepton *> LepsT       = MakeLeptonPointerVector(muons,electrons,param);
  //cout << "----------------   RunAllControlRegions  [Veto ]----------------------------------------------------------------------------------------------------------------" << endl;
  std::vector<Lepton *> LepsV  = MakeLeptonPointerVector(muons_veto,electrons_veto,param);

  

  ///  Select events based on NConv and if running Run_Conv
  FillHist("CR_BeforeConvCut", 1. , weight_ll, 2., 0.,  2.);

  if(!IsData){
    cout << "-------------------------------------------------------------------------" << endl;
    int nConv(0);
    for(auto ilep: LepsT){

      int LepType=GetLeptonType_JH(*ilep, gens);
      cout << ilep->GetFlavour() << " GenType = " <<  LepType << endl;
      if( LepType >=4 || LepType < -4) nConv++;
    }

    if(run_Debug)cout <<  "Number of Conversions = " << nConv << endl;

    FillHist( "CR_NConv", nConv , weight_ll, 2., 0.,  2.);

    if(RunPromptTLRemoval){
      weight_ll = -1*weight_ll* GetFakeWeight(LepsT, param , false);
      cout << "RunPromptTLRemoval : weight = " << weight_ll << endl;
    }

    else {
      if(RunConv && nConv==0)  {
	cout << "Removing events that has no conv. " << endl;
	return;

      }
      if(!RunConv && nConv > 0) return;
    }
  }

  FillHist("CR_AfterConvCut", 1. , weight_ll, 2., 0.,  2.);

  if(run_Debug) {
    for(auto ilep: LepsV) cout <<  ilep->GetFlavour() << " Pt = " << ilep->Pt() << " Eta = " << ilep->Eta() << " PassTID =" << ilep->PassLepID() << endl;
  }

  for(unsigned int ic = 0; ic < channels.size(); ic++){

    if(run_Debug) cout << "----------------------------------------" << endl;
    if(run_Debug) cout << "HNL_RegionDefinitionsOpt::RunAllControlRegions [" << GetChannelString(channels[ic])<<" ]" << endl;
    

    HNL_LeptonCore::Channel dilep_channel = channels[ic];
    HNL_LeptonCore::Channel trilep_channel = channels_lll[ic];
    HNL_LeptonCore::Channel fourlep_channel = channels_llll[ic];


    // Change Name of param to include Flavour channel
    TString channel_string = GetChannelString(dilep_channel);
    param.Name = channel_string + "/" + param.DefName + "_RunAllControlRegions";
    param.Name = channel_string + "/" + param.DefName + "_RunAllControlRegions";

    float weight_channel = weight_ll;

    // Weight Lepton with SF corr
    if(!IsDATA && dilep_channel != MuMu)  weight_channel*= GetElectronSFEventWeight(electrons, param);
    if(!IsDATA && dilep_channel != EE)    weight_channel*= GetMuonSFEventWeight(muons, param);
    
    TString label    = param.Name;
    
    if(run_Debug){
      cout << "CheckLeptonFlavourForChannel(dilep_channel, LepsT) = " <<CheckLeptonFlavourForChannel(dilep_channel, LepsT) << endl;
      cout << "CheckLeptonFlavourForChannel(trilep_channel, LepsT) = " <<CheckLeptonFlavourForChannel(trilep_channel, LepsT) << endl;
      cout << "CheckLeptonFlavourForChannel(fourlep_channel, LepsT) = " <<CheckLeptonFlavourForChannel(fourlep_channel, LepsT) << endl;
      
    }
    
    if(! (CheckLeptonFlavourForChannel(dilep_channel, LepsT) || CheckLeptonFlavourForChannel(trilep_channel, LepsT) || CheckLeptonFlavourForChannel(fourlep_channel, LepsT))) continue;
    
    if(SameCharge(LepsT)){
      int LepType1=GetLeptonType_JH(*LepsT[0], gens);
      int LepType2=GetLeptonType_JH(*LepsT[1], gens);
      FillHist("ControlRegionLeptonType/"+param.Name + "/"+GetChannelString(channels[ic]) , LepType1, LepType2,  weight_channel, 14,-7., 7.,14, -7.,7.);
    }
   
    /// This removes overlap in streams
    if (!PassTriggerSelection(dilep_channel, ev, LepsT,"Dilep")) continue;
    if(run_Debug)cout << param.Name << " " << " passes trigger " << endl;

    
    weight_channel *= HNLZvtxSF(dilep_channel);
 

    if(RunFake&&IsData){
      weight_channel = GetFakeWeight(LepsT, param , false);
      FillWeightHist(param.Name+"/FakeWeight",weight_channel);
    }

    vector<TString> passed;
    Fill_RegionPlots(dilep_channel, 0,"Inclusive" , param.Name,  VBF_JetColl,  AK8_JetColl,  LepsT, METv, nPV, weight_channel);
    
    // OS CR
    if(FillZCRPlots  (dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("ZAK8_CR");
    if(FillTopCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("TopAK8_CR");
    
    if(run_Debug) cout << "FillTopCRPlots " << endl;

    if(RunCF){    
      if(LepsT.size() == 2){

	if(dilep_channel == MuMu) continue;
	
	if(IsData && SameCharge(LepsT)) continue;
	if(!IsData && !SameCharge(LepsT)) continue;
	
	if(IsData)weight_channel = GetCFWeightElectron(LepsT, param);
	if(IsData)FillWeightHist(param.Name+"/CFWeight",weight_channel);
      }
      else continue;
    }
    if(LepsT.size() == 2){
      if(!SameCharge(LepsT)) continue;
    }
    
    
    if(FillSSPreselectionPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("SSPresel");
    if(FillSSVBFPreselectionPlots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("SSVBFPresel");

    //VBF CR                                                                                                                                                                      
    if(FillWWCR1Plots  (dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CR1");
    if(FillWWCR2Plots  (dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CR2");
    if(FillWWCRNPPlots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CRNP");
    if(FillWWCRNP2Plots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WpWp_CRNP2");
    if(FillWZ2CRPlots (trilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WZ2_CR");
    if(FillWZBCRPlots (trilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WZB_CR");
    

    // LLL+                                                                                                                                 
                                                                     
    if(FillZZCRPlots(  fourlep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("ZZ_CR");
    if(FillZZ2CRPlots( fourlep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("ZZLoose_CR");
    if(FillWZCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WZ_CR");
    if(FillZNPCRPlots(trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("ZNP_CR");

    if(run_Debug)cout << "Conv split "  <<  ConversionVeto(LepsT,gens) << " " << ConversionSplitting(LepsT,gens) << endl;
    
    // Treat conversions using GENT MEthod
    if(ConversionVeto(LepsT,gens)){
      if(FillWGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("WG_CR");
      if(FillZGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("ZG_CR");
    }
    // Treat using pt cit method
    if(ConversionSplitting(LepsT,gens)){
      
      AnalyzerParameter paramTMP=param;
      paramTMP.Name=param.Name+"_ConvMethodPt";
      if(FillWGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTMP, weight_channel)) passed.push_back("WG_Method2_CR");
      if(FillZGCRPlots( trilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, paramTMP, weight_channel)) passed.push_back("ZG_Method2_CR");

    }
    
    
    // 17-028 +CR                                                                                                                                                                                                  
    // SR1 : MET Inv. || BVeto Inv.
    if(FillHighMassSR1CRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR1_CR");

    // SR2 : MET Inv. || BVeto Inv.
    if(FillHighMassSR2CRPlots(dilep_channel, LepsT, LepsV, VBF_JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR2_CR");

    // SR3 : MET Inv. || BVeto Inv.
    if(FillHighMassSR3CRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassSR3_CR");
  
    if(FillHighMass1JetCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMass1Jet_CR");

    if(FillHighMassBJetCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassBJet_CR");
    
    // 0 Jet : 2 SS lep dphi > 2.5
    if(FillHighMassNPCRPlots(dilep_channel, LepsT, LepsV, JetColl, AK8_JetColl, B_JetColl, ev, METv, param, weight_channel)) passed.push_back("HighMassNP_CR");
    
    

    vector<TString> cutlabels = { "TopAK8_CR","ZAK8_CR","WpWp_CR1","WpWp_CR2","WpWp_CRNP","WpWp_CRNP2","ZZ_CR","ZZLoose_CR","ZG_CR","WG_CR","WZ_CR","WG_Method2_CR","ZG_Method2_CR","WZ2_CR","WZB_CR","HighMassSR1_CR","HighMassSR2_CR","HighMassSR3_CR","HighMass1Jet_CR","HighMassBJet_CR","HighMassNP_CR","ZNP_CR","SSPresel","SSVBFPresel"};
    
    for(auto ip : passed) FillEventCutflow(HNL_LeptonCore::CR, weight_channel, ip, "ChannelCutflow/"+param.Name);
    
    for(unsigned int ipass =0; ipass < passed.size();ipass++){
      //void AnalyzerCore::FillTypeCutflow(TString histname, double weight, vector<TString> lables, TString label1, TString label2){                                                                                 
      for(unsigned int ipass2=ipass+1; ipass2 < passed.size(); ipass2++){
	FillTypeCutflow(channel_string+"CR_Correlation"+param.Name, weight_channel, cutlabels, passed[ipass], passed[ipass2]);
      }
    }
  }
  return;
}




bool HNL_RegionDefinitionsOpt::FillTopCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector< Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (leps_veto.size() != 2) return false;


  if(METv.Pt() < 50) return false;
  
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl == 0) return false;
  if(AK8_JetColl.size() == 0) return false;

  if(run_Debug){
    cout << "HNL_TopAK8_TwoLepton_CR " << event << endl;
    for(auto ilep: leps) cout << "HNL_TopAK8_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << endl;
    //PrintGen(gens);                                                                                                                                                                                              
  }
  if(SameCharge(leps)) Fill_RegionPlots(channel,1,"HNL_SS_TopAK8_TwoLepton_CR" , param.Name, JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  else Fill_RegionPlots(channel,1,"HNL_OS_TopAK8_TwoLepton_CR" , param.Name, JetColl  ,AK8_JetColl,  leps,  METv, nPV, w);
  return true;
}



bool HNL_RegionDefinitionsOpt::FillZNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 30.;
  double mtcut = 30.;


  if(METv.Pt() > metcut) return false;
  if(GetIndexNonBestZ(leps,15.) < 0) return false;
  if(M_T((*leps[GetIndexNonBestZ(leps,15.)]), METv) > mtcut) return false;

  if(run_Debug){
    cout << "HNL_ZNP_ThreeLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZNP_ThreeLepton_CR Type " <<  GetLeptonType(*ilep, gens) << endl;
  }

  Fill_RegionPlots(channel,1,"HNL_ZNP_ThreeLepton_CR" , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitionsOpt::FillZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if (leps_veto.size() != 2) return false;
  
  
  if(METv.Pt() > 30) return false;
  int NB_JetColl = B_JetColl.size();
  if(NB_JetColl > 0) return false;
  if(AK8_JetColl.size() == 0) return false;

  Particle  ll = (*leps[0]) + (*leps[1]);

  if(ll.Charge() != 0) return false;
  if (fabs(ll.M()-90.) > 15) return false;

  if(run_Debug){
    cout << "HNL_ZAK8_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_ZAK8_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << endl;
  }
  if(SameCharge(leps))Fill_RegionPlots(channel, true,"HNL_SS_ZAK8_TwoLepton_CR" , param.Name, JetColl , AK8_JetColl,  leps,  METv, nPV, w);
  else Fill_RegionPlots(channel, true,"HNL_OS_ZAK8_TwoLepton_CR" , param.Name, JetColl , AK8_JetColl,  leps,  METv, nPV, w);
  return true;

}


bool HNL_RegionDefinitionsOpt::FillWWCR1Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){
  
  // This CRis same as AN2021_008 Table 10
  // Same as SR but Inverse DPhi cut
  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;

  int NB_JetColl = B_JetColl.size();
  /***
   //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045  
   //https://arxiv.org/pdf/2005.01173.pdf 
   **/

  
  Particle ll =  *leps[0] + *leps[1];


  // two highest pt jets with    
  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( ll.M() < 20.) return false;
  //  if ( METv.Pt() < 30.) return false;
  if (NB_JetColl>0) return false;
  
  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max(leps[0]->Eta()  - Av_JetEta , (leps[1])->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;

  double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
  if(ll_dphi >  2.) return false;

  if(run_Debug){
    cout << "HNL_WpWp_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWp_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << endl;
  }
  Fill_RegionPlots(channel, true,"HNL_WpWp_TwoLepton_CR1" , param.Name, jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitionsOpt::FillWWCR2Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,  std::vector< Jet> B_JetColl,Event ev, Particle METv, AnalyzerParameter param, float w){

  // This CRis same as AN2021_008 Table 10                                                                                                                                                                         
  // Same as SR but High MET

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;

  int NB_JetColl = B_JetColl.size();
  /***                                                                                                                                                                                                             
   //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                         
   //https://arxiv.org/pdf/2005.01173.pdf                                                                                                                                                                          
   **/


  Particle ll =  *leps[0] + *leps[1];


  // two highest pt jets with                                                                                                                                                                                      
  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( ll.M() < 20.) return false;
  if ( METv.Pt() < 30.) return false;                                                                                                                                                                          

  if (NB_JetColl>0) return false;

  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max(leps[0]->Eta()  - Av_JetEta , (leps[1])->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;


  if(run_Debug){
    cout << "HNL_WpWp_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWp_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << endl;
  }
  Fill_RegionPlots(channel, true,"HNL_WpWp_TwoLepton_CR2" , param.Name, jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitionsOpt::FillWWCRNPPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  // This is the same as AN2020_045 Table 15 Nonprompt

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;
  


  std::vector<Jet> JetColl                  = GetAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();
  //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045
  //https://arxiv.org/pdf/2005.01173.pdf 
  
  Particle ll =  *leps[0] + *leps[1];

  
  // two highest pt jets with   
  
  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  
  if ( METv.Pt() < 30.) return false;
  if (NB_JetColl==0) return false;
  if ((j1+j2).M() < 500.) return false;
  
  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max((leps[0])->Eta()  - Av_JetEta , leps[1]->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;



  if(run_Debug){
    cout << "HNL_WpWpNP_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << endl;
  }

  Fill_RegionPlots(channel, true,"HNL_WpWpNP_TwoLepton_CR" , param.Name, jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);
  
  return true;

}

bool HNL_RegionDefinitionsOpt::FillWWCRNP2Plots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl,std::vector< Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  // This is the same as AN2020_045 Table 15 Nonprompt                                                                                                                                                             

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(jets_eta5.size() < 2) return false;



  std::vector<Jet> JetColl                  = GetAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();
  //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                          
  //https://arxiv.org/pdf/2005.01173.pdf                                                                                                                                                                           

  Particle ll =  *leps[0] + *leps[1];


  // two highest pt jets with                                                                                                                                                                                      

  Jet j1 = jets_eta5[0] ;
  Jet j2 = jets_eta5[1];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;

  if (NB_JetColl==0) return false;
  if ((j1+j2).M() < 500.) return false;

  double DiJetDeta = fabs(j1.Eta() - j2.Eta());
  if (DiJetDeta  <2.5) return false;

  double Av_JetEta= 0.5*(j1.Eta()+ j2.Eta());
  double zeppenfeld = TMath::Max((leps[0])->Eta()  - Av_JetEta , leps[1]->Eta()  - Av_JetEta ) /DiJetDeta;
  if (zeppenfeld > 0.75) return false;
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;



  if(run_Debug){
    cout << "HNL_WpWpNP_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps) cout << "HNL_WpWpNP_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << endl;
  }

  Fill_RegionPlots(channel, true,"HNL_WpWpNP2_TwoLepton_CR" , param.Name, jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitionsOpt::FillOSPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;
  if(ll.M() < 110) return false;

  //double ST = GetST(leps, jets, AK8_JetColl, ev);
  //  double met2_st = pow(METv.Pt(),2.)/ ST;

  int njet = AK8_JetColl.size();

  if(njet < 1) return false;

  Fill_RegionPlots(channel,1,"HNL_OSPresel_TwoLepton"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitionsOpt::FillSSPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;
  if(ll.M() < 20) return false;

  //double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  //double met2_st = pow(METv.Pt(),2.)/ ST;

  int njets = JetColl.size() + AK8_JetColl.size();

  if(SameCharge(leps))Fill_RegionPlots(channel,1,"HNL_SSPresel_TwoLepton"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  return true;

}



bool HNL_RegionDefinitionsOpt::FillSSVBFPreselectionPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Jet> VBF_JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;



  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;
  if(ll.M() < 20) return false;


  if(VBF_JetColl.size() < 2) return false;
  if(!SameCharge(leps)) return false;
  Fill_RegionPlots(channel,1,"HNL_SSVBFPresel_TwoLepton"  , param.Name, VBF_JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}

bool HNL_RegionDefinitionsOpt::FillHighMassSR1CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;


  int NB_JetColl = B_JetColl.size();


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;
  if(ll.M() < 20) return false;

  double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 20);


  double LowerMassSR1WmassCut = 30.;
  double UpperMassSR1WmassCut = 150.;


  if(PassHMMet && NB_JetColl==0) return false;

  if(run_Debug){
    cout << "HNL_HighMassSR1_TwoLepton_CR " << param.Name << " " << event  << endl;
    for(auto ilep: leps){
      Gen gen_closest = GetGenMatchedLepton(*ilep, gens);

      cout << "HNL_HighMassSR1_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << " " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << " matched gen = " <<  gen_closest.Index() << endl;


    }

    for(auto ilep: leps)  cout << "HNL_HighMassSR1_TwoLepton_CR Type " <<  GetLeptonType(*ilep, gens) << " " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << endl;
  }


  if(AK8_JetColl.size()==0)    return false;

  Particle N1cand = AK8_JetColl[0] + *leps[0];

  double ml1jbins[7] = { 0., 100.,200.,300.,500., 1000., 2000.};
  double Qml1jbins[13] = {-2000., -1000., -500., -300., -200., -100,  0., 100.,200.,300.,500., 1000., 2000.};

  double MN1 = (N1cand.M() > 2000.) ? 1999. : N1cand.M();
  FillHist( "ControlSR1/"+param.Name+"/N1Mass_Central",  MN1,  w, 6, ml1jbins, "Reco M_{l1jj}");
  FillHist( "ControlSR1/"+param.Name+"/Q_N1Mass_Central",  leps[0]->Charge()*MN1,  w, 12, Qml1jbins, "Reco M_{l1jj}");


  FillEventCutflow(HNL_LeptonCore::ChannelDepCR1, w, GetChannelString(channel) +"_CR1", "ChannelCutFlow/"+param.DefName);

  Fill_RegionPlots(channel,1,"HNL_HighMassSR1_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitionsOpt::FillHighMass1JetCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if (channel!=EE ) return false;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;



  Particle ll =  (*leps[0]) + (*leps[1]);
  // Kepp only Z peak events with many CF
  if (fabs(ll.M()-90.) > 10) return false;
  
  if(JetColl.size() != 1) return false;
  Fill_RegionPlots(channel,1,"HNL_HighMass1Jet_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);


  return true;

}


bool HNL_RegionDefinitionsOpt::FillHighMassBJetCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){



  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) > 10)) return false;
  if(ll.M() < 10) return false;
  int NB_JetColl = B_JetColl.size();

  if(NB_JetColl  < 2) return false;
  Fill_RegionPlots(channel,1,"HNL_HighMassBJet_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}


bool HNL_RegionDefinitionsOpt::FillHighMassNPCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  Particle ll =  (*leps[0]) + (*leps[1]);
  if(ll.M() < 10) return false;

  
  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;

  if(JetColl.size() > 0) return false;


  if (channel==EE  && (fabs(ll.M()-90.) < 10)) return false;

  if(leps[0]->DeltaPhi(*leps[1]) < 2.5) return false;

  Fill_RegionPlots(channel,1,"HNL_HighMassNP_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitionsOpt::FillHighMassSR3CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;


  int NB_JetColl = B_JetColl.size();

  Particle ll =  (*leps[0]) + (*leps[1]);
  
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;


  if(ll.M() < 20) return false;

  double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  double LowerMassSR3WmassCut = 50.;
  double UpperMassSR3WmassCut = 150.;

  if(AK8_JetColl.size() > 0) return false;


  if(JetColl.size() == 0 && leps[1]->Pt() > 80.) FillHist( "ContolSR3/"+param.Name+"/SignalBins",   0.5, w, 16, 0, 16., "Signalbins");
  if(JetColl.size() == 1 && leps[1]->Pt() > 80.) FillHist( "ContolSR3/"+param.Name+"/SignalBins",   1.5, w, 16, 0, 16., "Signalbins");
  
  if (JetColl.size() < 2 && leps[1]->Pt() > 80.) Fill_RegionPlots(channel,1,"HNL_HighMassSR3b_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  if (JetColl.size() < 2  && leps[1]->Pt() > 80.) {
    Fill_RegionPlots(channel,1,"HNL_HighMassSR3LowJet_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
    return true;
  }
  if (JetColl.size() < 2) return false;

  if(!(GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) < UpperMassSR3WmassCut && GetMass("HNL_SR3", JetColl, AK8_JetColl,leps) > LowerMassSR3WmassCut)) return false;

  Fill_RegionPlots(channel,1,"HNL_HighMassSR3_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  float dijetmass_tmp=999.;
  float dijetmass=9990000.;
  int m=-999;
  int n=-999;
  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=1; enne<JetColl.size(); enne++) {

      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();
      if(emme == enne) continue;

      if ( fabs(dijetmass_tmp-80.4) < fabs(dijetmass-80.4) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
        n = enne;
      }
    }
  }

  Particle N1cand = JetColl[m] + JetColl[n]+ *leps[0];
  Particle N2cand = JetColl[m] + JetColl[n]+ *leps[1];
  
  double bin(0.);
  if (met2_st < 9){
    if(leps[0]->Pt() < 25) {
      if(N2cand.M() < 100.) {
        bin=2.5;
      }
      else{
        bin=3.5;
      }
    }

    else if(leps[1]->Pt() < 60) {
      if(N2cand.M() < 100.) {
        bin=4.5;
      }
      else if(N2cand.M() < 125.) {
        bin=5.5;
      }
      else if(N2cand.M() < 150.) {
        bin=6.5;
      }
      else if(N2cand.M() < 175.) {
        bin=7.5;

              }
      else if(N2cand.M() < 200.) {
        bin=8.5;
      }
      else {
        bin=9.5;
      }
    }
    else{
      if(N1cand.M() < 200) {
        bin=10.5;
      }
      else  if(N1cand.M() < 250.) {
        bin=11.5;
      }
      else if(N1cand.M() < 300.) {
        bin=12.5;
      }
      else if(N1cand.M() < 500.) {
        bin=13.5;
      }
      else if(N1cand.M() < 1000.) {
        bin=14.5;
      }
      else {
        bin = 15.5;
      }
    }
  }

  FillHist( "ControlSR3/"+param.Name+"/SignalBins",   bin, w, 16, 0, 16., "Signalbins");
  FillEventCutflow(HNL_LeptonCore::ChannelDepCR3, w, GetChannelString(channel) +"_CR3", "ChannelCutFlow/"+param.DefName);

  return true;
  
}



bool HNL_RegionDefinitionsOpt::FillHighMassSR2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;
  if (leps_veto.size() != 2) return false;
  if (leps.size() != 2) return false;


  int NB_JetColl = B_JetColl.size();
  Particle ll =  (*leps[0]) + (*leps[1]);
  if (channel==EE  && (fabs(ll.M()-90.) < 15)) return false;
  if(ll.M() < 20) return false;

  double ST = GetST(leps, JetColl, AK8_JetColl, ev);
  double met2_st = pow(METv.Pt(),2.)/ ST;
  bool PassHMMet    = (met2_st < 20);

  if(PassHMMet && NB_JetColl==0) return false;

  if(PassVBF(JetColl,leps,450)) {
    
  
    double HT(0.);
    for(UInt_t emme=0; emme<JetColl.size(); emme++){
      HT += JetColl[emme].Pt();
    }
    if (HT/leps[0]->Pt() > 2)  return false;
    
    Fill_RegionPlots(channel,1,"HNL_HighMassCR2_TwoLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
    
  
    double HTLT[2] = { 0., 2};
    FillHist( "ControlSR2/"+param.Name+"/HT_LT1_Central",  HT/leps[0]->Pt(),  w, 1, HTLT, "Reco HT/LT1");
    FillEventCutflow(HNL_LeptonCore::ChannelDepCR2, w, GetChannelString(channel) +"_CR2", "ChannelCutFlow/"+param.DefName);
  
    return true;
  }
  return false;
}


bool HNL_RegionDefinitionsOpt::FillWZ2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if(jets_eta5.size() < 2) return false;

  double metcut = 30.;
  double trilep_masscut=105.;


  std::vector<Jet> JetColl                  = GetAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();

  ////file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                        
  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < jets_eta5.size(); ij++){
    for(unsigned int ij2 = ij+1; ij2 < jets_eta5.size(); ij2++){
      double deta = fabs(jets_eta5[ij].Eta() - jets_eta5[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }
  if(GetIndexNonBestZ(leps,15.) < 0) return false;
  if(GetMassMinOSSF(leps)  < 20.) return false;
  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;


  if(jets_eta5.size() < 2) return false;
  Jet j1 = jets_eta5[ijet1] ;
  Jet j2 = jets_eta5[ijet2];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( METv.Pt() < metcut) return false;
  if (NB_JetColl>0) return false;
  if ((j1+j2).M() < 500.) return false;
  if (maxDiJetDeta<2.5) return false;

  double Av_JetEta= 0.5*(jets_eta5[ijet1].Eta()+ jets_eta5[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  double zeppenfeld2 = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[2]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  if(zeppenfeld2 > zeppenfeld) zeppenfeld=zeppenfeld2;

  if (zeppenfeld > 1.00) return false;

  Fill_RegionPlots(channel, true,"HNL_WZ2_ThreeLepton_CR" , param.Name, jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);


  return true;

}

bool HNL_RegionDefinitionsOpt::FillWZBCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto   , std::vector<Jet> jets_eta5, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if(jets_eta5.size() < 2) return false;

  double metcut = 30.;
  double trilep_masscut=105.;


  std::vector<Jet> JetColl                  = GetAK4Jets(jets_eta5, 20., 2.5, true,  0.4,0.8,"",    leps_veto,AK8_JetColl);
  int NB_JetColl = B_JetColl.size();
  //file:///Users/john/Downloads/AN2019_089_v7.pdf SSWW +WZ  + AN2020_045                                                                                                                                          


  if(jets_eta5.size() < 2) return false;
  double maxDiJetDeta=0.;
  int ijet1(-1), ijet2(-1);
  for(unsigned int ij = 0; ij < jets_eta5.size(); ij++){
    for(unsigned int ij2 = ij+1; ij2 < jets_eta5.size(); ij2++){
      double deta = fabs(jets_eta5[ij].Eta() - jets_eta5[ij2].Eta());
      if(deta > maxDiJetDeta) {
        maxDiJetDeta=deta;
        ijet1=ij;
        ijet2=ij2;
      }
    }
  }

  if(GetIndexNonBestZ(leps,15.) < 0) return false;
  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;

  Jet j1 = jets_eta5[ijet1] ;
  Jet j2 = jets_eta5[ijet2];
  if(!(j1.Pt() > 30.) && (j2.Pt() > 30.)) return false;
  if ( METv.Pt() < metcut) return false;
  if (NB_JetColl==0) return false;
  if ((j1+j2).M() < 500.) return false;
  if (maxDiJetDeta<2.5) return false;

  double Av_JetEta= 0.5*(jets_eta5[ijet1].Eta()+ jets_eta5[ijet2].Eta());
  double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  double zeppenfeld2 = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[2]).Eta()  - Av_JetEta ) /maxDiJetDeta;
  if(zeppenfeld2 > zeppenfeld) zeppenfeld=zeppenfeld2;

  if (zeppenfeld > 1.00) return false;


  Fill_RegionPlots(channel, true,"HNL_WZB_ThreeLepton_CR" , param.Name, jets_eta5,  AK8_JetColl,  leps,  METv, nPV, w);


  return true;

}


bool HNL_RegionDefinitionsOpt::FillZZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  int NB_JetColl=B_JetColl.size();


  if(NB_JetColl  > 0 ) return false;
  bool m_llos_l10(false), z_cr_pass(false);

  Particle Z1Cand;
  Particle Z2Cand;


  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
      if(leps[iel]->Charge() != leps[iel2]->Charge()){
        if(Z1Cand.M() < 10) m_llos_l10=true;

        int zel1(-9), zel2(-9);
        if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = (*leps[2]) + (*leps[3]);}
        if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = (*leps[1]) + (*leps[3]);}
        if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = (*leps[1]) + (*leps[2]);}
        if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = (*leps[0]) + (*leps[3]);}
        if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = (*leps[0]) + (*leps[2]);}
        if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = (*leps[0]) + (*leps[1]);}

        if(leps[zel1]->Charge() != leps[zel2]->Charge()){
          if(fabs(Z1Cand.M() - 90.1) < 15.){
	    if(fabs(Z2Cand.M() - 90.1) < 15.){
	      z_cr_pass=true;
	    }
          }
        }
      }
    }
  }
  if(m_llos_l10 || !z_cr_pass)  return false;
  Fill_RegionPlots(channel, true,"HNL_ZZ_FourLepton_CR" , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitionsOpt::FillZZ2CRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){



  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  int NB_JetColl=B_JetColl.size();


  if(NB_JetColl  > 0 ) return false;
  bool m_llos_l10(false), z_cr_pass(false);

  Particle Z1Cand;

  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
      if(leps[iel]->Charge() != leps[iel2]->Charge()){
        if(Z1Cand.M() < 12) m_llos_l10=true;

        if(fabs(Z1Cand.M() - 90.1) < 15.){
          z_cr_pass=true;
        }
      }
    }
  }



  if(m_llos_l10 || !z_cr_pass)  return false;
  Fill_RegionPlots(channel, true,"HNL_ZZLoose_FourLepton_CR" , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitionsOpt::FillZGCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  //if(MCSample.Contains("DY")) return false;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 50.;
  int NB_JetColl=B_JetColl.size();

  FillHist( "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_1",   1,  w, 14, 0, 14., "FillZGCRPlots");

  FillHist( "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_MLLL",   ((*leps[0]) + (*leps[1])+ (*leps[2])).M(),  w, 200, 0, 4000., "FillZGCRPlots");
  
  Particle lll = (*leps[0]) + (*leps[1])+ (*leps[2]);
  bool passZmass_lll_Window = (fabs(lll.M() - 90.1) < 15.);
  if(!passZmass_lll_Window) return false;

  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_2",   1,  w, 14, 0, 14., "FillZGCRPlots");
  Particle ll1 = *leps[0] + *leps[1];
  Particle ll2 = *leps[0] + *leps[2];
  Particle ll3 = *leps[1] + *leps[2];

  bool RemoveLowOSSFMass=false;
  bool OSSFMInZPeak=false;
  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() == leps[j]->Charge() ) continue;
      Particle OSSFll = *leps[i] + *leps[j];
      FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_OSSSM",   OSSFll.M(),  w, 200, 0, 500., "FillZGCRPlots");
      if(fabs(OSSFll.M() - 90.1) < 15) OSSFMInZPeak=true;
      if(OSSFll.M() < 10) RemoveLowOSSFMass=true;

    }
  }
  
  if(RemoveLowOSSFMass) return false;
  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_3",   1,  w, 14, 0, 14., "FillZGCRPlots");

  if(OSSFMInZPeak) return false;
  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_4",   1,  w, 14, 0, 14., "FillZGCRPlots");

  if(ZmassOSSFWindowCheck(leps,15.)) return false;

  FillHist(  "HNL_ZG_ThreeLepton_CR/"+param.Name+"/FillZGCRPlots_5",   1,  w, 14, 0, 14., "FillZGCRPlots");
  if(NB_JetColl > 0) return false;

  if(METv.Pt() > metcut) return false;

      
  if(run_Debug){
    cout << "HNL_ZG_ThreeLepton_CR " << param.Name << " " << event  << endl;

    //for(auto ilep: leps){
    // Gen gen_closest = GetGenMatchedLepton(*ilep, gens);
    //  Gen gen_photon_closest = GetGenMatchedPhoton(*ilep, gens);
      //int NearPhotonType = GetGenPhotonType(gen_photon_closest,gens);

    //      FillHist("Photon_pt_HNL_ZG_ThreeLepton_CR",gen_photon_closest.Pt() , w, 100., 0.,  200.);
    // }

    //PrintGen(gens);
  }

  Fill_RegionPlots(channel,1,"HNL_ZG_ThreeLepton_CR" , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;

}



bool HNL_RegionDefinitionsOpt::FillWGCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){


  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  if(HasLowMassMeson(leps)) return false;

  double metcut = 30.;
  double mt_cut = 30.;
  int NB_JetColl=B_JetColl.size();

  Particle ll1 = (*leps[0]) + (*leps[1]);
  Particle ll2 = (*leps[0]) + (*leps[2]);
  Particle ll3 = (*leps[1]) + (*leps[2]);
  bool passlos_ll_mass=false;
  if(ll1.Charge() == 0 && (ll1.M() < 4.)) passlos_ll_mass=true;
  if(ll2.Charge() == 0 && (ll2.M() < 4.)) passlos_ll_mass=true;
  if(ll3.Charge() == 0 && (ll3.M() < 4.)) passlos_ll_mass=true;
  if(!passlos_ll_mass) return false;
  Particle lll = (*leps[0]) + (*leps[1])+ (*leps[2]);
  double MT_lll = M_T(METv,lll);

  if(MT_lll <= mt_cut) return false;
  if(NB_JetColl > 0)return false;
  if(METv.Pt() < metcut)return false;

  if(run_Debug){
    cout << "HNL_WG_ThreeLepton_CR " << param.Name << " " << event  << endl;

    PrintGen(gens);
  }
  Fill_RegionPlots(channel,1,"HNL_WG_ThreeLepton_CR"  , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);

  return true;
}


bool HNL_RegionDefinitionsOpt::FillWZCRPlots(HNL_LeptonCore::Channel channel, std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto, std::vector<Jet> JetColl, std::vector<FatJet> AK8_JetColl, std::vector<Jet> B_JetColl,  Event ev, Particle METv, AnalyzerParameter param, float w){

  //  cout << "FillWZCRPlots " << param.Name << " " << leps.size() << endl;

  if(!CheckLeptonFlavourForChannel(channel, leps)) return false;

  double metcut = 50.;
  double mtcut = 20.;
  double trilep_masscut=105.;


  int NB_JetColl=B_JetColl.size();
  

  if(METv.Pt() < metcut) return false;
  
  if(NB_JetColl > 0) return false;
  
  if(GetIndexNonBestZ(leps,15.) < 0) return false;

  if(M_T((*leps[GetIndexNonBestZ(leps,15.)]), METv)  < mtcut) return false;

  if(GetMassMinOSSF(leps)  < 10.) return false;

  if(((*leps[0])+ (*leps[1]) + (*leps[2])).M() <  trilep_masscut) return false;
  
  Fill_RegionPlots(channel,1,"HNL_WZ_ThreeLepton_CR" , param.Name, JetColl,  AK8_JetColl,  leps,  METv, nPV, w);
  
  return true;
}


