#include "Electron.h"

ClassImp(Electron)

Electron::Electron(){

  j_En_up=1.;
  j_En_down=1.;;
  j_Res_up = 1.;
  j_Res_down = 1.;

  j_scEta = -999.;
  j_scPhi = -999.;
  j_scE = -999.;
  j_mvaiso = -999.;
  j_mvanoiso = -999.;
  j_EnergyUnCorr = -999.;
  j_passConversionVeto = false;
  j_NMissingHits = 0;
  j_Full5x5_sigmaIetaIeta = -999.;
  j_dEtaSeed = -999.;
  j_dPhiIn = -999.;
  j_HoverE  = -999.;
  j_InvEminusInvP = -999.;
  j_e2x5OverE5x5 = -999.;
  j_e1x5OverE5x5 = -999.;
  j_trkiso = -999.;
  j_dr03EcalRecHitSumEt = -999.;
  j_dr03HcalDepth1TowerSumEt = -999.;
  j_dr03HcalTowerSumEt = -999.;
  j_dr03TkSumPt = -999.;
  j_ecalPFClusterIso = -999.;
  j_hcalPFClusterIso = -999.;
  j_isEcalDriven = false;
  j_IDBit = 0;
  j_IDCutBit.clear();
  j_Rho = -999.;
  j_isGsfCtfScPixChargeConsistent = false;
  this->SetLeptonFlavour(ELECTRON);
}

Electron::~Electron(){

}

void Electron::SetEnShift(double en_up, double en_down){
  j_En_up = en_up;
  j_En_down = en_down;
}

void Electron::SetResShift(double res_up, double res_down){
  j_Res_up = res_up;
  j_Res_down = res_down;
}

void Electron::SetSC(double sceta, double scphi, double sce){
  j_scEta = sceta;
  j_scPhi = scphi;
  j_scE = sce;
}

void Electron::SetMVA(double mvaiso, double mvanoiso){
  j_mvaiso = mvaiso;
  j_mvanoiso = mvanoiso;
}

void Electron::SetUncorrE(double une){
  j_EnergyUnCorr = une;
}

void Electron::SetPassConversionVeto(bool b){
  j_passConversionVeto = b;
}

void Electron::SetNMissingHits(int n){
  j_NMissingHits = n;
}

void Electron::SetCutBasedIDVariables(
    double Full5x5_sigmaIetaIeta,
    double dEtaSeed,
    double dPhiIn,
    double HoverE,
    double InvEminusInvP,
    double e2x5OverE5x5,
    double e1x5OverE5x5,
    double trackIso,
    double dr03EcalRecHitSumEt,
    double dr03HcalDepth1TowerSumEt,
    double dr03HcalTowerSumEt,
    double dr03TkSumPt,
    double ecalPFClusterIso,
    double hcalPFClusterIso,
    int ecalDriven
  ){
  j_Full5x5_sigmaIetaIeta = Full5x5_sigmaIetaIeta;
  j_dEtaSeed = dEtaSeed;
  j_dPhiIn = dPhiIn;
  j_HoverE = HoverE;
  j_InvEminusInvP = InvEminusInvP;
  j_e2x5OverE5x5 = e2x5OverE5x5;
  j_e1x5OverE5x5 = e1x5OverE5x5;
  j_trkiso = trackIso;
  j_dr03EcalRecHitSumEt = dr03EcalRecHitSumEt;
  j_dr03HcalDepth1TowerSumEt = dr03HcalDepth1TowerSumEt;
  j_dr03HcalTowerSumEt = dr03HcalTowerSumEt;
  j_dr03TkSumPt = dr03TkSumPt;
  j_ecalPFClusterIso = ecalPFClusterIso;
  j_hcalPFClusterIso = hcalPFClusterIso;

  if(ecalDriven==0) j_isEcalDriven = false;
  else j_isEcalDriven = true;
}

void Electron::SetIDBit(unsigned int idbit){
  j_IDBit = idbit;
}

void Electron::SetIDCutBit(vector<int> idcutbit){
  j_IDCutBit = idcutbit;
}

void Electron::SetRelPFIso_Rho(double r){
  j_RelPFIso_Rho = r;
  this->SetRelIso(r);
}

double Electron::EA(){

  //==== RecoEgamma/ElectronIdentification/data/Fall17/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_94X.txt
  
  double eta = fabs(this->scEta());

  if     (eta<1.000) return 0.1440;
  else if(eta<1.479) return 0.1562;
  else if(eta<2.000) return 0.1032;
  else if(eta<2.200) return 0.0859;
  else if(eta<2.300) return 0.1116;
  else if(eta<2.400) return 0.1321;
  else if(eta<2.500) return 0.1654;
  else return 0.1654;

}

bool Electron::PassIDOpt(TString ID, bool cc, double dx_b ,double dx_e,double dz_b,double dz_e, double iso_b, double iso_e) const{

 bool pass_id= PassID(ID);

  if( fabs(scEta()) <= 1.479 ){
    if(!( fabs(dZ()) < dz_b )) return false;
    if(!( fabs(dXY()) < dx_b )) return false;
    if(!( fabs(RelIso()) < iso_b )) return false;
    if( dx_b != 999. && fabs(IP3D()/IP3Derr())>4) return false;
  }
  else{
    if(!( fabs(dZ()) < dz_e )) return false;
    if(!( fabs(dXY()) < dx_e )) return false;
    if(!( fabs(RelIso()) < iso_e )) return false;
    if( dx_e != 999. && fabs(IP3D()/IP3Derr())>4) return false;
  }


  return true;
}
   
bool Electron::PassID(TString ID) const{

  // === list of IDs for analyis
  if(ID=="NoCut") return true;


  //==== XXX veto Gap Always
  if(etaRegion()==GAP) return false;

  //==== Customized
  if(ID=="NOCUT") return true;
  if(ID=="TEST") return Pass_TESTID();

  //======== VETO IDs
  //==== POG
  if(ID=="passVetoID") return passVetoID(); // --- VETO POG

  //==== HN
  if(ID=="HNVeto2016") return Pass_HNVeto2016(); // --- vet MVA + VVLoose ISO/IP
  if(ID=="HNVeto_17028") return Pass_HNVeto2016(); // --- vet MVA + VVLoose ISO/IP
  if(ID=="HNVeto") return Pass_HNVeto(0.6); // -- VETO POG + VVLoose ISO/IP        

  if(ID=="HNLoosest") return Pass_HNLoosest(); // OR of VETO IDs

  //=== No ISO
  if(ID=="CutBasedLooseNoIso") return Pass_CutBasedLooseNoIso();
  if(ID=="CutBasedMediumNoIso") return Pass_CutBasedMediumNoIso();
  if(ID=="CutBasedTightNoIso") return Pass_CutBasedTightNoIso();
  if(ID=="MVALooseNoIso") return passMVAID_noiso_WPLoose();
  if(ID=="CutBasedVetoNoIso") return Pass_CutBasedVetoNoIso();
  
  if(PassIDLoose(ID)   >=0) return (PassIDLoose(ID)==1) ? true : false;
  if(PassIDOptLoose(ID)>=0) return (PassIDOptLoose(ID)==1) ? true : false;
  if(PassIDTight(ID)   >=0) return (PassIDTight(ID)==1)  ?  true : false;
  if(PassIDOpt(ID)     >=0) return (PassIDOpt(ID)==1)  ?  true : false;

  cout << "[Electron::PassID] No id : " << ID << endl;
  exit(ENODATA);
  
  return false;
}

int  Electron::PassIDTight(TString ID) const{

  // same ID used by Haneol excpet with conv                                                                                                                                                                                         
  if(ID=="HNTightV2")  return passTightID_NoCC() &&PassHNID()  &&(fabs(IP3D()/IP3Derr())<4.)? 1 : 0 ;
  if(ID=="HNTight_17028") return Pass_HNTight2016()? 1 : 0 ;  // EXO-17-028                                                                                                               
  //=== POG
  if(ID=="passPOGTight")                  return passTightID_NoCC()? 1 : 0 ;
  if(ID=="passPOGMedium")            return passMediumID_NoCC()? 1 : 0 ;

  if(ID=="passMediumID") return passMediumID()? 1 : 0 ;
  if(ID=="passTightID") return passTightID()? 1 : 0 ;
  //=== HEEP IDS                                                                                                                                                                                        
  if(ID=="passHEEPID") return passHEEPID()? 1 : 0 ;
  if(ID=="passHEEPID2018Prompt") return passHEEP2018Prompt()? 1 : 0 ;

  //=== MVA
  if(ID=="passMVAID_noIso_WP80") return passMVAID_noIso_WP80()? 1 : 0 ;
  if(ID=="passMVAID_noIso_WP90") return passMVAID_noIso_WP90()? 1 : 0 ;
  if(ID=="passMVAID_Iso_WP80") return passMVAID_Iso_WP80()? 1 : 0 ;
  if(ID=="passMVAID_Iso_WP90") return passMVAID_Iso_WP90()? 1 : 0 ;

  if(ID=="HNHEEPID")  return passHEEPID()&&PassHNID()? 1 : 0 ;
  if(ID=="HNHEEPID2018Prompt")  return passHEEP2018Prompt()&&PassHNID()? 1 : 0 ;

  //==== Customized                                                                                                                                                                                  
  if(ID=="SUSYTight") return Pass_SUSYTight()? 1 : 0 ;
  if(ID=="SUSYLoose") return Pass_SUSYLoose()? 1 : 0 ;

  // ===== Type-1                                                   


  if(ID.Contains("HNMVA_")) {
    TString mva_st = ID.ReplaceAll("HNMVA_","");
    std::string mva_s = std::string(mva_st);
    std::string::size_type sz;     // alias of size_t                                                                                                                      

    double mva_d = std::stod (mva_s,&sz);

    return ((passIDHN(3,0.04, 0.04, 0.1,0.1, 5.,5., 0.1, 0.1, -999., -999.)&&PassMVA(mva_d,mva_d,mva_d))  ? 1 : 0);
  }                                                                                                                              
  if(ID=="HN2016")                 return passIDHN(3,0.01, 0.01, 0.04,0.04, 4.,4., 0.1, 0.06, -999., -999.)&&PassMVA(0.65,0.81, 0.89) ? 1 : 0 ;
  if(ID=="HNRelaxedIP2016")        return passIDHN(3,0.05, 0.05, 0.1,0.1, 4.,4., 0.1, 0.05, -999., -999.)&&PassMVA(0.65,0.81, 0.89) ? 1 : 0 ;
  if(ID=="HN2017")                 return passIDHN(3,0.01, 0.01, 0.04,0.04, 5.,5., 0.085, 0.05, -999., -999.)&&PassMVA(0.65,0.67, 0.89) ? 1 : 0 ;
  if(ID=="HNRelaxedIP2017")        return passIDHN(3,0.05, 0.05, 0.1,0.1, 5.,5., 0.1, 0.05, -999., -999.)&&PassMVA(0.65,0.71, 0.89) ? 1 : 0 ;
  if(ID=="HN2018")                 return passIDHN(3,0.01, 0.01, 0.04,0.04, 5.,5., 0.095, 0.07, -999., -999.)&&PassMVA(0.65,0.75, 0.89) ? 1 : 0 ;
  if(ID=="HNRelaxedIP2018")        return passIDHN(3,0.05, 0.05, 0.1,0.1, 5.,5., 0.095, 0.07, -999., -999.)&&PassMVA(0.65,0.75, 0.89) ? 1 : 0 ;

  return -1;

}

int  Electron::PassIDLoose(TString ID) const{
    
  if(ID=="passProbeID") return passVetoID()&&Pass_TriggerEmulation() ? 1 : 0;  // --- VETO POG                                                                                                                                           
  //=== POG
  if(ID=="passLooseID") return passLooseID() ? 1 : 0; 
  if(ID=="HEEPLoose")   return passLooseHEEPID() ? 1 : 0; 
  if(ID=="HNHEEPLoose")  return passLooseHEEPID()&&PassHNID() ? 1 : 0; 

  //=== loose user                                                                                                                                                                                       

  if(ID=="HNLooseMVA") return ((passIDHN(3,0.2, 0.2, 0.5,0.5, 10.,10., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1))  ? 1 : 0);
;
  
  if(ID=="HNLooseV1")   return      Pass_HNLooseID(0.6,0.2,0.1,10) ? 1 : 0;  // V POG IP/ISO   17028 IP                                                                                                                   
  if(ID=="HNLooseV2")   return  Pass_HNLooseID(0.6,0.2,0.1,9999) ? 1 : 0;  // V POG IP/ISO                                                                                                                   
  if(ID=="HNLooseV3")    return  Pass_HNLooseID(0.6,0.02,0.1,4.) ? 1 : 0;  // V POG IP/ISO                                                                                                                    
  if(ID=="HNLooseV4")   {
    if( fabs(scEta())<= 1.479 ){
      return  Pass_HNLooseID(0.6,0.05,0.1,4.) ? 1 : 0;  // non POG IP/ISO                                                                         
    }
    else  return  Pass_HNLooseID(0.6,0.1,0.2,4.) ? 1 : 0; 
  }

  if(ID=="HNLooseV4_LooseIP")   {
    return  Pass_HNLooseID(0.6,0.2,0.2,10.) ? 1 : 0;  // non POG IP/ISO                                                                           
  }

  if(ID=="HNLoosePOG")   {
    if( fabs(scEta())<= 1.479 )  return  Pass_HNLooseID(0.6,0.05,0.1,999.) ? 1 : 0;  // non POG IP/ISO                                                                           
    else  return  Pass_HNLooseID(0.6,0.1,0.2,999.) ? 1 : 0; 
  }
  
  if(ID=="HNLoosePOG_LooseIP")   {
    return  Pass_HNLooseID(0.6,0.2,0.2,999.) ? 1 : 0;  // non POG IP/ISO                                                                       
  }

  // ===== Type-1                                                                                                                                                                                     
  if(ID=="HNLoose2016")           return ((passIDHN(3,0.01, 0.01, 0.04,0.04, 4.,4., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1))  ? 1 : 0); 
  if(ID=="HNRelaxedIPLoose2016")  return (passIDHN(3,0.05, 0.05, 0.1,0.1, 4.,4., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1))  ? 1 : 0; 
  if(ID=="HNLoose2016_LooseIP")   return (passIDHN(3,0.2, 0.2, 0.1,0.1, 10.,10., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1))  ? 1 : 0; 

  if(ID=="HNLoose2017")           return (passIDHN(3,0.01, 0.01, 0.04,0.04, 5.,5., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1))  ? 1 : 0; 
  if(ID=="HNRelaxedIPLoose2017")  return (passIDHN(3,0.05, 0.05, 0.1,0.1, 5.,5., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1) ) ? 1 : 0; 
  if(ID=="HNLoose2017_LooseIP")   return (passIDHN(3,0.2, 0.2, 0.1,0.1, 10.,10., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1) ) ? 1 : 0; 

  if(ID=="HNLoose2018")           return (passIDHN(3,0.01, 0.01, 0.04,0.04, 5.,5., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1))  ? 1 : 0; 
  if(ID=="HNRelaxedIPLoose2018")  return (passIDHN(3,0.05, 0.05, 0.1,0.1, 5.,5., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1) ) ? 1 : 0; 
  if(ID=="HNLoose2018_LooseIP")   return (passIDHN(3,0.2, 0.2, 0.1,0.1, 10.,10., 0.6, 0.6, -999., -999.)&&PassMVA(-0.1,0.1, -0.1) ) ? 1 : 0; 

  

  if(ID=="passLooseID")      return passLooseIDHN() ? 1 : 0;  // non POG IP/ISO Pass_HNLooseID(0.6,0.2,0.2,10.);                                          
  return -1;
}


int  Electron::PassIDOptLoose(TString ID) const{

  if(ID=="passMVAID_noIso_WP80Loose") return passMVAID_noIso_WP80HNLoose() ? 1 : 0 ;
  if(ID=="passMVAID_noIso_WP90Loose") return passMVAID_noIso_WP90HNLoose() ? 1 : 0 ;
  if(ID=="passMVAID_Iso_WP80Loose") return passMVAID_Iso_WP80HNLoose() ? 1 : 0 ;
  if(ID=="passMVAID_Iso_WP90Loose") return passMVAID_Iso_WP90HNLoose()? 1 : 0 ;
  if(ID=="passMVAID_noIso_WP90LooseV2") return Pass_HNLoose2016MVANonIso(0.6,0.02,0.05,5)? 1 : 0 ;
  if(ID=="HNMVALoose") return Pass_HNMVALoose(0.6, 999., 999.)? 1 : 0 ;

  if(ID=="HNLoose_17028")       return Pass_HNLoose2016(0.6, 0.2, 0.1, 10.)? 1 : 0 ;
  if(ID=="HNLoose_17028_TightIP")  return Pass_HNLoose2016(0.6, 0.01, 0.04, 4.)? 1 : 0 ;
  if(ID=="HNLoose_17028_TOP")   return Pass_HNLoose2016(0.6, 0.02, 0.04, 4.)? 1 : 0 ;


  return -1;



}
int Electron::PassIDOpt(TString ID) const{

  /// POG + POG IP +CC

  // updated 2016 analysis ID with looser dxy
  if(ID=="HNTight2016Update") return passIDHN(4,0.02, 0.02, 0.05,0.04, 4.9,4.9, -999., -999., 0.089, 0.05) && PassMVA(0.8,0.8,0.775)? 1 : 0 ;  // EXO-17-028                                                  

  // POG ID with relaxed cuts
  if(ID=="HNTight_dxyB")  return passIDHN(1,999., 0.1, 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_dxyE")  return passIDHN(1,0.05, 999., 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_dzB")  return passIDHN(1,0.05, 0.1, 999.,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_dzE")  return passIDHN(1,0.05, 0.1, 0.1,999., 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_ipB")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 999.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_ipE")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,999., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_isoB")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., 999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_isoE")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., -999., 999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_miniisoB")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., 999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_miniisoE")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., -999., 999.) ? 1 : 0 ;
 
  if(ID=="HNMedium_dxyB")  return passIDHN(2,999., 0.1, 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_dxyE")  return passIDHN(2,0.05, 999., 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_dzB")  return passIDHN(2,0.05, 0.1, 999.,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_dzE")  return passIDHN(2,0.05, 0.1, 0.1,999., 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_ipB")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 999.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_ipE")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,999., -999., -999., -999., -999.) ? 1 : 0 ;

  if(ID=="HNMedium_isoB")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., 999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_isoE")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., -999., 999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_miniisoB")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., 999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_miniisoE")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., -999., 999.) ? 1 : 0 ;


  if(ID=="HN_mva")     return passIDHN(3,0.05, 0.1, 0.1,0.2, 4.,4., 0.08, 0.08, -999., -999.) ? 1 : 0 ;
  if(ID=="HNMini_mva")  return passIDHN(4,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999.,0.08, 0.08) ? 1 : 0 ;

  if(ID=="HNMVATight") return Pass_HNMVATight(0.08, 0.05, 0.1)? 1 : 0 ;

  if(ID.Contains("passMVAID_V")){
    if(ID.Contains("passMVAID_V1") && !passMVAID_noIso_XHN(0.05,0.05,0.1,4.)) return (false) ? 1 : 0 ;
    if(ID.Contains("passMVAID_V2") && !passMVAID_noIso_XHN(0.08,0.05,0.1,4.)) return (false) ? 1 : 0 ;
    if(ID.Contains("passMVAID_V3") ){
      if (!passMVAID_noIso_XHN(999.,0.05,0.1,4.)) return (false) ? 1 : 0 ;
      if(fabs(scEta()) <= 1.479 ){
	//0.0287+0.506/pT
	if(! (RelIso() < 0.0478+0.506/UncorrPt()) ) return (false) ? 1 : 0 ;
      }
      else         if(! (RelIso() < 0.0658+0.963/UncorrPt()) ) return (false) ? 1 : 0 ;
    }
    if(ID.Contains("passMVAID_V4") ){
      if (!passMVAID_noIso_XHN(999.,0.05,0.1,4.)) return (false) ? 1 : 0 ;
      if(fabs(scEta()) <= 1.479 ){
        //0.0287+0.506/pT                                                                                                                                                                                   
        if(! (RelIso() < 0.0287+0.506/UncorrPt()) ) return (false) ? 1 : 0 ;
      }
      else         if(! (RelIso() < 0.0445+0.963/UncorrPt()) ) return (false) ? 1 : 0 ;
    }

    if( fabs(scEta()) <= 0.8 ){
      double mva_1 = 0.8;
      std::string mva1= std::to_string(mva_1);

      for(unsigned int i=0; i < 11; i++){
	mva_1 = 0.8 + 0.01*double(i);
	mva1= std::to_string(mva_1);

	if(ID.Contains("_mva1_"+TString(mva1)+"_")) {
	  if(! (MVANoIso()> mva_1) ) return (false) ? 1 : 0 ;
	  else return (true) ? 1 : 0 ;
	}
      }
      cout << "FAIL "<< ID << " " << "_mva1_"+TString(mva1)+"_   " << MVANoIso()  << " " << mva_1 << endl;

    }
    else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
      double mva_1 = 0.7;
      std::string mva1= std::to_string(mva_1);
      
      for(unsigned int i=0; i < 11; i++){
        mva_1 = 0.7 + 0.02*double(i);
	mva1= std::to_string(mva_1);
	//	cout << ID << " " << "_mva1_"+TString(mva1)+"_" << endl;

	if(ID.Contains("_mva2_"+TString(mva1)+"_")) {

	  if(! (MVANoIso()> mva_1) ) return (false) ? 1 : 0 ;
	  else return (true) ? 1 : 0 ;
	}
      }
      cout << "FAIL "<< ID << " " << "_mva2_"+TString(mva1)+"_   " << MVANoIso()  << " " << mva_1 << endl;


    }
    else{
      double mva_1 = 0.5;
      std::string mva1= std::to_string(mva_1);

      for(unsigned int i=0; i < 6; i++){
        mva_1 = 0.5 + 0.05*double(i);
        mva1= std::to_string(mva_1);
	//	cout << ID << " " << "_mva1_"+TString(mva1)+"_" << endl;

	if(ID.Contains("_mva3_"+TString(mva1))) {

	  if(! (MVANoIso()> mva_1) ) return (false) ? 1 : 0 ;
	  else return (true) ? 1 : 0 ;
	}
      }
      cout << "FAIL "<< ID << " " << "_mva3_"+TString(mva1)+"_   " << MVANoIso()  << " " << mva_1 << endl;


    }
    return (true) ? 1 : 0 ;
  }
  
 

  


  if(ID=="CutBasedLooseNoIso") return Pass_CutBasedLooseNoIso()? 1 : 0 ;
  if(ID=="CutBasedMediumNoIso") return Pass_CutBasedMediumNoIso()? 1 : 0 ;
  if(ID=="CutBasedTightNoIso") return Pass_CutBasedTightNoIso()? 1 : 0 ;
  if(ID=="MVALooseNoIso") return passMVAID_noiso_WPLoose()? 1 : 0 ;
  if(ID=="CutBasedVetoNoIso") return Pass_CutBasedVetoNoIso()? 1 : 0 ;

  return -1;
}

bool Electron::passLooseHEEPID() const{

  int HEEPcutbit = IDCutBit().at(11);


  //==  H/E and EM+Had_depth1 = (4096-1) - (1<<6) - (1<<8) - (1<<7) - (1<<9) = 3135
  if(! ( (HEEPcutbit&3135)==3135 ) ) return false;
  if(! (HoverE()<0.13) ) return false;
  if(! (TrkIso()<20.) ) return false;
  if(! (dXY()<0.1) ) return false;
  if(! ( dr03EcalRecHitSumEt() + dr03HcalDepth1TowerSumEt() < 8. ) ) return false;     

  return true;
  
}
bool Electron::passHEEP2018Prompt() const {

  //==== If not endcap, use original function
  if( fabs(scEta()) < 1.566 ) return passHEEPID();

  //==== https://github.com/CMSSNU/SKFlatMaker/blob/Run2Legacy_v4/SKFlatMaker/python/SKFlatMaker_cfi.py#L37-L50
  int HEEPcutbit = IDCutBit().at(11);

  //==== https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2#Applying_Individual_Cuts_of_a_Se
  //==== We will modify H/E (bit nr=6) and EM+Had_depth1 (bit nr=8) isolation cut for EndCap for 2018
  //==== Decimal without H/E and EM+Had_depth1 = (4096-1) - (1<<6) - (1<<8) = 3775
  if(! ( (HEEPcutbit&3775)==3775 ) ) return false;

  //==== new cutd : https://indico.cern.ch/event/831669/contributions/3485543/attachments/1871797/3084930/ApprovalSlides_EE_v3.pdf, page 9

  //==== new H/E cut
//double cutValue_HoverE =                                      5 / scE() + 0.05; // original cut
  double cutValue_HoverE = ( -0.4 + 0.4 * fabs(scEta()) ) * Rho() / scE() + 0.05;
  if(! (HoverE()<cutValue_HoverE) ) return false;

  //==== new EM+Had_depth1 cut
//double cutValue_emhaddep1 = UncorrPt() > 50. ? 2.5 + 0.03 * (UncorrPt()-50.) +                        0.28 * Rho() : 2.5 +                        0.28 * Rho(); // original cut

  double cutValue_emhaddep1 = UncorrPt() > 50. ? 2.5 + 0.03 * (UncorrPt()-50.) + (0.15 + 0.07*fabs(scEta())) * Rho() : 2.5 + (0.15 + 0.07*fabs(scEta())) * Rho();
  if(! ( dr03EcalRecHitSumEt() + dr03HcalDepth1TowerSumEt() < cutValue_emhaddep1 ) ) return false;

  return true;

}

bool Electron::Pass_SUSYMVAWP(TString wp) const{

  double sceta = fabs(scEta());

    double cutA = 0.77;
    double cutB = 0.52;

  if(wp=="Tight"){
    if     (sceta<0.8)  { cutA = 0.77; cutB = 0.52; }
    else if(sceta<1.479){ cutA = 0.56; cutB = 0.11; } 
    else                { cutA = 0.48; cutB =-0.01; }
  }
  else if(wp=="Loose"){
    if     (sceta<0.8)  { cutA =-0.48; cutB =-0.85; }
    else if(sceta<1.479){ cutA =-0.67; cutB =-0.91; }
    else                { cutA =-0.49; cutB =-0.83; }
  }
  else{}

  double cutSlope = (cutB-cutA) / 10.;
  double cutFinal = std::min( cutA, std::max(cutB , cutA + cutSlope*(this->Pt()-15.) ) );

  //==== Using NoIso MVA, because we apply MiniIso later
  if(MVANoIso()>cutFinal) return true;
  else return false;

}

bool Electron::Pass_SUSYTight() const{
  if(! Pass_SUSYMVAWP("Tight") ) return false;
  if(! (MiniRelIso()<0.1) ) return false;
  if(! (fabs(dXY())<0.05 && fabs(dZ())<0.1 && fabs(IP3D()/IP3Derr())<8.) ) return false;
  if(! PassConversionVeto() ) return false;
  if(! (NMissingHits()==0) ) return false;

  return true;
}

bool Electron::Pass_SUSYLoose() const{
  if(! Pass_SUSYMVAWP("Loose") ) return false;
  if(! (MiniRelIso()<0.4) ) return false;
  if(! (fabs(dXY())<0.05 && fabs(dZ())<0.1 && fabs(IP3D()/IP3Derr())<8.) ) return false;
  if(! PassConversionVeto() ) return false;
  if(! (NMissingHits()==0) ) return false;

  return true;
}

bool Electron::Pass_CutBasedTightWithIPcut() const{
  if(! passTightID() ) return false;
  if( fabs(scEta()) <= 1.479 ){
    if(! (fabs(dXY())<0.05 && fabs(dZ())<0.1) ) return false;
  }
  else{
    if(! (fabs(dXY())<0.1 && fabs(dZ())<0.2) ) return false;
  }
  return true;
}

//===============================================
//==== 2016 ID
//===============================================

bool Electron::Pass_TriggerEmulationLoose() const{
  // Trigger emulation (See https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSLeptonSF#ID_IP_ISO_AN1)                                                           
  // Cuts (IdL, IdM) in single electron triggers                                                                                                              
  // No Iso cuts in the trigger with IdM                                                                                                                      
  if(this->Pt() < 40. && this->Pt() > 20.) return Pass_TriggerEmulation();

  if(! (ecalPFClusterIso()/UncorrPt() < 0.5) ) return false;    // < 0.5                                                                                     
  if(! (hcalPFClusterIso()/UncorrPt() < 0.3) ) return false;    // < 0.3                                                                                     
  if(! (dr03TkSumPt()/UncorrPt() < 0.2) ) return false;          // < 0.2                                                                                     

  if( fabs(scEta()) <= 1.479 ){
    if(! (Full5x5_sigmaIetaIeta() < 0.013) ) return false;       // < 0.013, 0.011                                                                            
    if(! (fabs(dEtaSeed()) < 0.01) ) return false;              // < 0.01 , 0.006                                                                            
    if(! (fabs(dPhiIn()) < 0.07) ) return false;                 // < 0.07 , 0.15                                                                             
    if(! (HoverE() < 0.13) ) return false;                       // < 0.13 , 0.12                                                                             
    if(! (fabs(InvEminusInvP()) < 9999999.) ) return false;          // < 9999., 0.05                                                                             
  }
  else{
    if(! (Full5x5_sigmaIetaIeta() < 0.035) ) return false;       // < 0.035, 0.031                                                                            
    if(! (fabs(dEtaSeed()) < 0.015) ) return false;              // < 0.015, 0.0085                                                                           
    if(! (fabs(dPhiIn()) < 0.1) ) return false;                 // < 0.1  , 0.1                                                                              
    if(! (HoverE() < 0.13) ) return false;                       // < 0.13 , 0.1                                                                              
    if(! (fabs(InvEminusInvP()) < 999999.) ) return false;          // < 9999., 0.05                                                                             
  }

  return true;
}

bool Electron::Pass_TriggerEmulation() const{
  // Trigger emulation (See https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSLeptonSF#ID_IP_ISO_AN1)
  // Cuts (IdL, IdM) in single electron triggers
  // No Iso cuts in the trigger with IdM

  if(! (ecalPFClusterIso()/UncorrPt() < 0.45) ) return false;    // < 0.5
  if(! (hcalPFClusterIso()/UncorrPt() < 0.25) ) return false;    // < 0.3
  if(! (dr03TkSumPt()/UncorrPt() < 0.2) ) return false;          // < 0.2

  if( fabs(scEta()) <= 1.479 ){
    if(! (Full5x5_sigmaIetaIeta() < 0.011) ) return false;       // < 0.013, 0.011
    if(! (fabs(dEtaSeed()) < 0.005) ) return false;              // < 0.01 , 0.006
    if(! (fabs(dPhiIn()) < 0.04) ) return false;                 // < 0.07 , 0.15
    if(! (HoverE() < 0.08) ) return false;                       // < 0.13 , 0.12 
    if(! (fabs(InvEminusInvP()) < 0.01) ) return false;          // < 9999., 0.05
  }
  else{
    if(! (Full5x5_sigmaIetaIeta() < 0.031) ) return false;       // < 0.035, 0.031
    if(! (fabs(dEtaSeed()) < 0.007) ) return false;              // < 0.015, 0.0085
    if(! (fabs(dPhiIn()) < 0.08) ) return false;                 // < 0.1  , 0.1
    if(! (HoverE() < 0.08) ) return false;                       // < 0.13 , 0.1
    if(! (fabs(InvEminusInvP()) < 0.01) ) return false;          // < 9999., 0.05
  }

  return true;
}

bool Electron::Pass_HNVeto2016() const{

  if( fabs(scEta()) <= 0.8 ){
    if(! (MVANoIso()>-0.1) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVANoIso()>0.1) ) return false;
  }
  else{
    if(! (MVANoIso()>-0.1) ) return false;
  }
  if(! (fabs(dXY())<0.2 && fabs(dZ())<0.5) ) return false;

  if(! (RelIso()<0.6) ) return false;

  return true;
}

bool Electron::Pass_HNLoose2016MVAISO(double dxyCut, double dzCut, double sipCut) const{

  if( fabs(scEta()) <= 0.8 ){
    if(! (MVAIso()>-0.1) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVAIso()>0.1) ) return false;
  }
  else{
    if(! (MVAIso()>-0.1) ) return false;
  }
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<sipCut) ) return false;

  if(! (PassConversionVeto()) ) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent())  return false;

  if(! (Pass_TriggerEmulation()) ) return false;

  return true;
}

bool Electron::PassHNID() const{
  
  if(! (PassConversionVeto()) ) return false;
  if(! IsGsfCtfScPixChargeConsistent())  return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  return true;
}



bool Electron::Pass_HNLoose2016MVANonIso(double relisoCut, double dxyCut, double dzCut, double sipCut) const{
  if( fabs(scEta()) <= 0.8 ){
    if(! (MVANoIso()>-0.1) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVANoIso()>0.1) ) return false;
  }
  else{
    if(! (MVANoIso()>-0.1) ) return false;
  }
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<sipCut) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent())  return false;

  if(! (Pass_TriggerEmulation()) ) return false;

  return true;
}


bool Electron::Pass_HNLoose2016(double relisoCut, double dxyCut, double dzCut, double sipCut) const{
  if( fabs(scEta()) <= 0.8 ){
    if(! (MVANoIso()>-0.1) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVANoIso()>0.1) ) return false;
  }
  else{
    if(! (MVANoIso()>-0.1) ) return false;
  }
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<sipCut) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) ) return false;

  if(! (Pass_TriggerEmulation()) ) return false;

  return true;
}


bool Electron::passIDHN(int ID, double dxy_b, double dxy_e, double dz_b,double dz_e, double sip_b, double sip_e, double iso_b,double iso_e, double miso_b, double miso_e) const{

  if( fabs(scEta())<= 1.479 ){
    if(! (fabs(dXY())< dxy_b && fabs(dZ())<dz_b && fabs(IP3D()/IP3Derr())< sip_b) ) return false;
  }
  else{
    if(! (fabs(dXY())< dxy_e && fabs(dZ())<dz_e && fabs(IP3D()/IP3Derr())< sip_e) ) return false;
  }
  //  if(! (passTightID()) ) return false;
  if( ID==1){


    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(! (passTightID()) ) return false;  
    }
    else {
      if(! (Pass_CutBasedTightNoIso()) ) return false; 
      if( miso_e > 0.){
	if(fabs(scEta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
      }
      else if( iso_e > 0.){
        if(fabs(scEta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
      }
      else if(fabs(scEta())> 1.479  &&  ! (passTightID()))  return false;

      if( miso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
      }
      else if( iso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
      }
      else if(fabs(scEta()) <= 1.479  &&! (passTightID()))  return false;
      
    }
  }
  if( ID==2){
    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(! (passMediumID()) ) return false;
    }
    else {
      if(! (Pass_CutBasedMediumNoIso()) ) return false;
      if( miso_e > 0.){
        if(fabs(scEta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
      }
      else if( iso_e > 0.){
        if(fabs(scEta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
      }
      else if(fabs(scEta())> 1.479  &&  ! (passMediumID()))  return false;
      if( miso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
      }
      else if( iso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
      }
      else if(fabs(scEta()) <= 1.479  &&! (passMediumID()))  return false;

    }
  }
  if( ID==3){
    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(fabs(scEta()) <= 1.479 && !(RelIso() < 0.287+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(RelIso() < 0.0445+0.963/UncorrPt()) ) return false;
    }
    if( iso_b == 999.  && iso_e == 999.  &&  miso_b == 999.  && miso_e == 999. ){
      if(fabs(scEta()) <= 1.479 && !(RelIso() < 0.0478+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(RelIso() < 0.0658+0.963/UncorrPt()) ) return false;

    }
    
    if(fabs(scEta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
    if(fabs(scEta())< 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
  }
  if( ID==4){
    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(fabs(scEta()) <= 1.479 && !(MiniRelIso() < 0.287+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(MiniRelIso() < 0.0445+0.963/UncorrPt()) ) return false;
      
    }
    if( iso_b == 999.  && iso_e == 999.  &&  miso_b == 999.  && miso_e == 999. ){
      if(fabs(scEta()) <= 1.479 && !(MiniRelIso() < 0.0478+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(MiniRelIso() < 0.0658+0.963/UncorrPt()) ) return false;
      
    }
    
    if(fabs(scEta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
    if(fabs(scEta())< 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
  }

  
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) )return false;

  return true;
}
bool Electron::passMediumIDHN() const{
  if(! (passMediumID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) )return false;

  return true;  
}

bool Electron::passMediumID_NoCC() const {
  if(! (passMediumID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1)) return false;
  }
  else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2)) return false;


  return true;
}

bool Electron::passTightID_NoCC() const {
  if(! (passTightID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1)) return false;
  }
    else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2)) return false;

  
  return true;
}

bool Electron::passTightID_NoCCB() const {

  if(! (passTightID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1)) return false;
  }
  else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2 )) return false;

  return true;
}
bool Electron::passLooseID_NoCC() const {

  if(! (Pass_CutBasedVetoNoIso()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if( !(fabs(dXY()) < 0.2 && fabs(dZ())< 0.2 )) return false;
  if(! (RelIso()<0.6) ) return false;

  return true;
}


bool Electron::passMVAID_noIso_WP80HN() const {
  if(!(passMVAID_noIso_WP80())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(! (RelIso()<0.08) ) return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  }
  else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;

  return true;
  
}

bool Electron::passPOGMVAID_noIso_WP80HN() const {
  if(!(passMVAID_noIso_WP80())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;
  return true;

}
bool Electron::passPOGMVAID_noIso_WP90HN() const {
  if(!(passMVAID_noIso_WP90())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;
  return true;

}

bool Electron::passPOGMVAID_Iso_WP90HN() const {
  if(!(passMVAID_Iso_WP90())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent())  return false;
  return true;
}

bool Electron::passPOGMVAID_Iso_WP80HN() const {
  if(!(passMVAID_Iso_WP80())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent())  return false;
  return true;
}


bool Electron::passMVAID_noIso_WP80HNLoose() const {
  if(!(Pass_HNLoose2016MVANonIso(0.6, 0.2, 0.1, 10.))) return false;

  return true;
}


bool Electron::passMVAID_Iso_WP90HN() const {
  if(!(passMVAID_Iso_WP90())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  }
  else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2 && fabs(IP3D()/IP3Derr()) < 4.)) return false;

  if(! PassHNIsGsfCtfScPixChargeConsistent())  return false;
  return true;
}

bool Electron::passMVAID_Iso_WP80HN() const {
  if(!(passMVAID_Iso_WP80())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  }
  else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2 && fabs(IP3D()/IP3Derr()) < 4.)) return false;

  if(! PassHNIsGsfCtfScPixChargeConsistent())  return false;
  return true;
}

bool Electron::passMVAID_Iso_WP80HNLoose() const {
  if(!(Pass_HNLoose2016MVAISO( 0.2, 0.1, 10.))) return false;
  return true;
}

bool Electron::passMVAID_Iso_WP90HNLoose() const {
  if(!(Pass_HNLoose2016MVAISO( 0.2, 0.1, 10.))) return false;
  return true;
}

bool Electron::PassHNIsGsfCtfScPixChargeConsistent() const{
  return PassHNIsGsfCtfScPixChargeConsistentVar(10000., 100000.);
}
bool Electron::PassHNIsGsfCtfScPixChargeConsistentVar(double pt1, double pt2) const{

  if(  fabs(scEta()) <= 1.479 ){
    if (this->Pt() < pt1) {
      if(! (IsGsfCtfScPixChargeConsistent()) )return false;
    }
  }
  else {
    if (this->Pt() < pt2) {
      if(! (IsGsfCtfScPixChargeConsistent()) )return false;
    }
  }
  
  return true;
}

bool Electron::passMVAID_noIso_XHN(double relisoCut, double dxyCut, double dzCut, double sipCut) const {
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(relisoCut > 0){
    if(! (RelIso()<relisoCut) ) return false;
  }
  else{
    if(! (MiniRelIso()<fabs(relisoCut)) ) return false;
  }
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < dxyCut && fabs(dZ())< dzCut && fabs(IP3D()/IP3Derr()) < sipCut)) return false;
  }
  else   if( !(fabs(dXY()) < dxyCut && fabs(dZ())< dzCut && fabs(IP3D()/IP3Derr()) < sipCut)) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;
  return true;

}
bool Electron::passMVAID_noIso_WP90XHN(double relisoCut, double dxyCut, double dzCut, double sipCut) const {
  if(!(passMVAID_noIso_WP90())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(relisoCut > 0){
    if(! (RelIso()<relisoCut) ) return false;
  }
  else{
    if(! (MiniRelIso()<fabs(relisoCut)) ) return false;
  }
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < dxyCut && fabs(dZ())< dzCut && fabs(IP3D()/IP3Derr()) < sipCut)) return false;
  }
  else   if( !(fabs(dXY()) < dxyCut && fabs(dZ())< dzCut && fabs(IP3D()/IP3Derr()) < sipCut)) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;
  return true;

}
bool Electron::passMVAID_noIso_WP90HN() const {
  if(!(passMVAID_noIso_WP90())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(! (RelIso()<0.08) ) return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  }
  else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;
  return true;
  
}
bool Electron::passMVAID_noIso_WP90HNLoose() const {
  if(!(Pass_HNLoose2016MVANonIso(0.6, 0.2, 0.1, 10.))) return false;

  return true;
}


bool Electron::passLooseIDHN() const {

  if(! (Pass_CutBasedVetoNoIso()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if( !(fabs(dXY()) < 0.2 && fabs(dZ())< 0.2 && fabs(IP3D()/IP3Derr())<10.)) return false;

  if(! (IsGsfCtfScPixChargeConsistent()) )return false;

  if(! (RelIso()<0.6) ) return false;
  
  return true;
}


bool Electron::passLooseID_NoCCB() const {
  return true;
}

bool Electron::PassMVA(double mva1, double mva2, double mva3) const {

  if( fabs(scEta()) <= 0.8 ){
    if(! (MVANoIso()>mva1) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVANoIso()>mva2) ) return false;
  }
  else{
    if(! (MVANoIso()>mva3) ) return false;
  }

  return true;
}
bool Electron::Pass_HNTight2016() const{


  if(!PassMVA(0.9, 0.825, 0.5)) return false;

  if(! (fabs(dXY())<0.01 && fabs(dZ())<0.04 && fabs(IP3D()/IP3Derr())<4.) ) return false;
  if(! (RelIso()<0.08) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) ) return false;

  if(! (Pass_TriggerEmulation()) ) return false;

  return true;
}

//===============================================
//==== Run2 ID
//===============================================

bool Electron::Pass_HNVeto(double relisoCut) const{

  if(! (Pass_CutBasedVetoNoIso()) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (fabs(dXY())<0.2 && fabs(dZ())<0.5) ) return false;
  return true;
}

bool Electron::Pass_HNLoosest() const{
  if(! (RelIso()< 0.6) ) return false;
  if(!( Pass_CutBasedVetoNoIso() || (MVANoIso()>-0.95)  )) return false;
  return true;
}
bool Electron::Pass_HNLoose(double relisoCut, double dxyCut, double dzCut) const{

  return true;
}

bool Electron::Pass_HNLooseID(double relisoCut, double dxyCut, double dzCut, double ipcut) const{

  if(! (Pass_CutBasedVetoNoIso()) ) return false;

  if(! (RelIso()<relisoCut) ) return false;
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<ipcut    )) return false;
  if(! IsGsfCtfScPixChargeConsistent()) return false;

  if(! (Pass_TriggerEmulation()) ) return false;
  return true;

}

bool Electron::Pass_HNMedium(double relisoCut, double dxyCut1, double dxyCut2, double dzCut, double ipcut1, double ipcut2) const{

  if(! (passMediumID()) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if( fabs(scEta()) <= 1.479 ){
    if(! (fabs(dXY())<dxyCut1 && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<ipcut1 ))   return false;
  }
  else{
    if(! (fabs(dXY())<dxyCut2 && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<ipcut2 ))   return false;

  }

  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;

  if(! (Pass_TriggerEmulation()) ) return false;
  return true;
}


bool Electron::Pass_HNTight(double relisoCut, double dxyCut1, double dxyCut2, double dzCut, double ipcut1, double ipcut2) const{
  if(! (passTightID()) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if( fabs(scEta()) <= 1.479 ){
    if(! (fabs(dXY())<dxyCut1 && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<ipcut1 ))   return false;
  }
  else{
    if(! (fabs(dXY())<dxyCut2 && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<ipcut2 ))   return false;

  }
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  return true;
}

bool Electron::Pass_ISRLoose(double relisoCut) const{
  if(! (Pass_CutBasedLooseNoIso()) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  return true;
}

bool Electron::Pass_ISRTight() const{
  if(! (passMediumID()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  return true;
}

bool Electron::Pass_HNMVALoose(double relisoCut, double dxyCut, double dzCut) const{
  if(!( passMVAID_noiso_WPLoose() )) return false;
  if(! (RelIso()<relisoCut) ) return false;
  //if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut) ) return false;
  //if(! (PassConversionVeto()) ) return false;
  //if(! (IsGsfCtfScPixChargeConsistent()) ) return false;
  return true;
}

bool Electron::Pass_HNMVATight(double relisoCut, double dxyCut, double dzCut) const{
  if(!( passMVAID_noIso_WP90() )) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) ) return false;
  return true;
}

//==== TEST ID

bool Electron::Pass_TESTID() const{
  return true;
}


bool Electron::Pass_CutBasedTightNoIso() const{

  if( fabs(scEta()) <= 1.479 ){

    if(! (Full5x5_sigmaIetaIeta() < 0.0104) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00255) ) return false;
    if(! (fabs(dPhiIn()) < 0.022) ) return false;
    if(! (HoverE() < 0.026 + 1.15/scE() + 0.0324*Rho()/scE()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.159) ) return false;
    if(! (NMissingHits() <= 1) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }
  else{

    if(! (Full5x5_sigmaIetaIeta() < 0.0353) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00501) ) return false;
    if(! (fabs(dPhiIn()) <  0.236 ) ) return false;
    if(! (HoverE() < 0.0188 + 2.06/scE() + 0.183*Rho()/scE()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.0197) ) return false;
    if(! (NMissingHits() <= 1) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }
}
bool Electron::Pass_CutBasedMediumNoIso() const{

  if( fabs(scEta()) <= 1.479 ){

    if(! (Full5x5_sigmaIetaIeta() < 0.0106) ) return false;
    if(! (fabs(dEtaSeed()) < 0.0032) ) return false;
    if(! (fabs(dPhiIn()) < 0.0547) ) return false;
    if(! (HoverE() < 0.046 + 1.16/scE() + 0.0324*Rho()/scE()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.184) ) return false;
    if(! (NMissingHits() <= 1) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }
  else{

    if(! (Full5x5_sigmaIetaIeta() < 0.0387) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00632) ) return false;
    if(! (fabs(dPhiIn()) <  0.394 ) ) return false;
    if(! (HoverE() < 0.0275 + 2.52/scE() + 0.183*Rho()/scE()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.0721) ) return false;
    if(! (NMissingHits() <= 1) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }

}

bool Electron::Pass_CutBasedLooseNoIso() const{
  bool debug=false;
  if( fabs(scEta()) <= 1.479 ){
    if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB" << endl;
    if(! (Full5x5_sigmaIetaIeta() < 0.0112) ) { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB Full5x5_sigmaIetaIeta" << Full5x5_sigmaIetaIeta()   << endl; return false;}
    if(! (fabs(dEtaSeed()) < 0.00377) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB fabs(dEtaSeed())"  << fabs(dEtaSeed()) << endl; return false;}
    if(! (fabs(dPhiIn()) < 0.0884) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB fabs(dPhiIn())  " << fabs(dPhiIn())   << endl; return false;}
    if(! (HoverE() < 0.05 + 1.16/scE() + 0.0324*Rho()/scE()) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB HoverE() " << HoverE() << " : " <<  0.05 + 1.16/scE() + 0.0324*Rho()/scE()  << endl; return false;}
    if(! (fabs(InvEminusInvP()) < 0.193) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB fabs(InvEminusInvP()) " << fabs(InvEminusInvP())    << endl; return false;}
    if(! (NMissingHits() <= 1) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB NMissingHits() " << NMissingHits()   << endl; return false;}
    if(! (PassConversionVeto()) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EB PassConversionVeto()" << PassConversionVeto()  << endl; return false;}

    return true;

  }
  else{

    if(! (Full5x5_sigmaIetaIeta() < 0.0425) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EE Full5x5_sigmaIetaIeta()" << Full5x5_sigmaIetaIeta()  << endl; return false;}
    if(! (fabs(dEtaSeed()) < 0.00674) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EE fabs(dEtaSeed())" << fabs(dEtaSeed())  << endl; return false;}
    if(! (fabs(dPhiIn()) <  0.169 ) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EE"  << endl; return false;}
    if(! (HoverE() < 0.0441 + 2.54/scE() + 0.183*Rho()/scE()) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EE HoverE() "  << HoverE()  << endl; return false;}
    if(! (fabs(InvEminusInvP()) < 0.111) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EE fabs(InvEminusInvP())"  << fabs(InvEminusInvP()) <<  endl; return false;}
    if(! (NMissingHits() <= 1) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EE NMissingHits() " << NMissingHits()   << endl; return false;}
    if(! (PassConversionVeto()) )  { if(debug) cout << "Electron::Pass_CutBasedLooseNoIso EE PassConversionVeto()" << PassConversionVeto()  << endl; return false;}

    return true;

  }

}


bool Electron::Pass_CutBasedVetoNoIso() const{
  
  if( fabs(scEta()) <= 1.479 ){
    
    if(! (Full5x5_sigmaIetaIeta() < 0.0126 ) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00463 ) ) return false;
    if(! (fabs(dPhiIn()) < 0.148) ) return false;
    if(! (HoverE() < 0.05 + 1.16/scE() + 0.0324*Rho()/scE()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.209) ) return false;
    if(! (NMissingHits() <= 2) ) return false;
    if(! (PassConversionVeto()) ) return false;
    
    return true;
  
  }
  else{
    
    if(! (Full5x5_sigmaIetaIeta() < 0.0457) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00814) ) return false;
    if(! (fabs(dPhiIn()) < 0.19) ) return false;
    if(! (HoverE() < 0.05 + 2.54/scE() + 0.183*Rho()/scE()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.132) ) return false;
    if(! (NMissingHits() <= 3) ) return false;
    if(! (PassConversionVeto()) ) return false;
    
    return true;
  
  }

}

bool Electron::Pass_CutBasedLoose() const{

  if( fabs(scEta()) <= 1.479 ){

    if(! (Full5x5_sigmaIetaIeta() < 0.0112) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00377) ) return false;
    if(! (fabs(dPhiIn()) < 0.0884) ) return false;
    if(! (HoverE() < 0.05 + 1.16/scE() + 0.0324*Rho()/scE()) ) return false;
    if(! (RelIso() < 0.112+0.506/UncorrPt()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.193) ) return false;
    if(! (NMissingHits() <= 1) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }
  else{

    if(! (Full5x5_sigmaIetaIeta() < 0.0425) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00674) ) return false;
    if(! (fabs(dPhiIn()) <  0.169 ) ) return false;
    if(! (HoverE() < 0.0441 + 2.54/scE() + 0.183*Rho()/scE()) ) return false;
    if(! (RelIso() < 0.108+0.963/UncorrPt()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.111) ) return false;
    if(! (NMissingHits() <= 1) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }

}

bool Electron::Pass_CutBasedVeto() const{

  if( fabs(scEta()) <= 1.479 ){

    if(! (Full5x5_sigmaIetaIeta() < 0.0126 ) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00463 ) ) return false;
    if(! (fabs(dPhiIn()) < 0.148) ) return false;
    if(! (HoverE() < 0.05 + 1.16/scE() + 0.0324*Rho()/scE()) ) return false;
    if(! (RelIso() < 0.198+0.506/UncorrPt()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.209) ) return false;
    if(! (NMissingHits() <= 2) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }
  else{

    if(! (Full5x5_sigmaIetaIeta() < 0.0457) ) return false;
    if(! (fabs(dEtaSeed()) < 0.00814) ) return false;
    if(! (fabs(dPhiIn()) < 0.19) ) return false;
    if(! (HoverE() < 0.05 + 2.54/scE() + 0.183*Rho()/scE()) ) return false;
    if(! (RelIso() < 0.203+0.963/UncorrPt()) ) return false;
    if(! (fabs(InvEminusInvP()) < 0.132) ) return false;
    if(! (NMissingHits() <= 3) ) return false;
    if(! (PassConversionVeto()) ) return false;

    return true;

  }

}

void Electron::SetRho(double r){
  j_Rho = r;
}

void Electron::SetIsGsfCtfScPixChargeConsistent(bool b){
  j_isGsfCtfScPixChargeConsistent = b;
}

bool Electron::PassFilter(TString filter) const{
  if( filter=="hltDiMu9Ele9CaloIdLTrackIdLElectronlegDphiFilter" ) return j_filterbits&(ULong64_t(1)<<0);
  else if( filter=="hltEGL1SingleEGOrFilter" ) return j_filterbits&(ULong64_t(1)<<1);
  else if( filter=="hltEle12CaloIdLTrackIdLIsoVLTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<2);
  else if( filter=="hltEle16Ele12Ele8CaloIdLTrackIdLDphiLeg1Filter" ) return j_filterbits&(ULong64_t(1)<<3);
  else if( filter=="hltEle16Ele12Ele8CaloIdLTrackIdLDphiLeg2Filter" ) return j_filterbits&(ULong64_t(1)<<4);
  else if( filter=="hltEle17CaloIdLTrackIdLIsoVLTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<5);
  else if( filter=="hltEle17CaloIdMGsfTrackIdMDphiFilter" ) return j_filterbits&(ULong64_t(1)<<6);
  else if( filter=="hltEle23CaloIdLTrackIdLIsoVLJet30TrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<7);
  else if( filter=="hltEle23Ele12CaloIdLTrackIdLIsoVLDZFilter" ) return j_filterbits&(ULong64_t(1)<<8);
  else if( filter=="hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg1Filter" ) return j_filterbits&(ULong64_t(1)<<9);
  else if( filter=="hltEle23Ele12CaloIdLTrackIdLIsoVLTrackIsoLeg2Filter" ) return j_filterbits&(ULong64_t(1)<<10);
  else if( filter=="hltEle27WPTightGsfTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<11);
  else if( filter=="hltEle27erWPTightGsfTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<12);
  else if( filter=="hltEle28WPTightGsfTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<13);
  else if( filter=="hltEle32L1DoubleEGWPTightGsfTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<14);
  else if( filter=="hltEle32WPTightGsfTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<15);
  else if( filter=="hltEle35noerWPTightGsfTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<16);
  else if( filter=="hltEle8CaloIdLTrackIdLIsoVLTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<17);
  else if( filter=="hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter" ) return j_filterbits&(ULong64_t(1)<<18);
  else if( filter=="hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<19);
  else if( filter=="hltMu23TrkIsoVVLEle8CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<20);
  else if( filter=="hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLDZFilter" ) return j_filterbits&(ULong64_t(1)<<21);
  else if( filter=="hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter" ) return j_filterbits&(ULong64_t(1)<<22);
  else if( filter=="hltMu9Ele9DZFilter" ) return j_filterbits&(ULong64_t(1)<<23);
  else{
    cout<<"[Electron::PassFilter] unknown filter "<<filter<<endl;
    exit(ENODATA);
  }
  return false;
}

bool Electron::PassPath(TString path) const{
  if( path=="HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<0);
  else if( path=="HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v" ) return j_pathbits&(ULong64_t(1)<<1);
  else if( path=="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v" ) return j_pathbits&(ULong64_t(1)<<2);
  else if( path=="HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v" ) return j_pathbits&(ULong64_t(1)<<3);
  else if( path=="HLT_Ele17_CaloIdL_TrackIdL_IsoVL_PFJet30_v" ) return j_pathbits&(ULong64_t(1)<<4);
  else if( path=="HLT_Ele17_CaloIdM_TrackIdM_PFJet30_v" ) return j_pathbits&(ULong64_t(1)<<5);
  else if( path=="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v" ) return j_pathbits&(ULong64_t(1)<<6);
  else if( path=="HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<7);
  else if( path=="HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v" ) return j_pathbits&(ULong64_t(1)<<8);
  else if( path=="HLT_Ele27_WPTight_Gsf_v" ) return j_pathbits&(ULong64_t(1)<<9);
  else if( path=="HLT_Ele27_eta2p1_WPTight_Gsf_v" ) return j_pathbits&(ULong64_t(1)<<10);
  else if( path=="HLT_Ele28_WPTight_Gsf_v" ) return j_pathbits&(ULong64_t(1)<<11);
  else if( path=="HLT_Ele32_WPTight_Gsf_L1DoubleEG_v" ) return j_pathbits&(ULong64_t(1)<<12);
  else if( path=="HLT_Ele32_WPTight_Gsf_v" ) return j_pathbits&(ULong64_t(1)<<13);
  else if( path=="HLT_Ele35_WPTight_Gsf_v" ) return j_pathbits&(ULong64_t(1)<<14);
  else if( path=="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v" ) return j_pathbits&(ULong64_t(1)<<15);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<16);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v" ) return j_pathbits&(ULong64_t(1)<<17);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<18);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v" ) return j_pathbits&(ULong64_t(1)<<19);
  else if( path=="HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v" ) return j_pathbits&(ULong64_t(1)<<20);
  else if( path=="HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<21);
  else if( path=="HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v" ) return j_pathbits&(ULong64_t(1)<<22);
  else{
    cout<<"[Electron::PassPath] unknown path "<<path<<endl;
    exit(ENODATA);
  }
  return false;
}
