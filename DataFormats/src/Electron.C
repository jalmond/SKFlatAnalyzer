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

bool Electron::PassID(TString ID, bool cc, double dx_b ,double dx_e,double dz_b,double dz_e, double iso_b, double iso_e) const{
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

  //==== XXX veto Gap Always
  if(etaRegion()==GAP) return false;

  if(ID=="NOCUT") return true;

  //==== POG
  if(ID=="passVetoID") return passVetoID();

  /// POG + POG IP +CC
  
  if(ID=="passLooseID")      return passLooseIDHN(); // non POG IP/ISO
  if(ID=="passMediumID")     return passMediumIDHN();
  if(ID=="passTightID")      return passTightIDHN()&&Pass_TriggerEmulation();
  if(ID=="passTightIDNoT")   return passTightIDHN();


  if(ID=="passPOGTight")                  return passTightID_NoCC();
  if(ID=="passPOGTight_IP4")              return passTightID_NoCC()&&(fabs(IP3D()/IP3Derr())<4.);
  if(ID=="passPOGTight_TC")               return passTightID_NoCC()&&IsGsfCtfScPixChargeConsistent();
  if(ID=="passPOGTight_TC_IP4")           return passTightID_NoCC()&&IsGsfCtfScPixChargeConsistent()&&(fabs(IP3D()/IP3Derr())<4.);
  if(ID=="passPOGTight_HNTC_NOPC")        return passTightID_NoCC()&&PassHNIsGsfCtfScPixChargeConsistent();
  if(ID=="passPOGTight_HNTC")             return passTightID_NoCC()&&PassHNIsGsfCtfScPixChargeConsistent()&&PassConversionVeto();
  if(ID=="passPOGTight_HNTC_IP4")         return passTightID_NoCC()&&PassHNIsGsfCtfScPixChargeConsistent()&&PassConversionVeto()&&(fabs(IP3D()/IP3Derr())<4.);
  
  if(ID=="passPOGTight_TTrig")            return passTightID_NoCC()&&Pass_TriggerEmulation();
  if(ID=="passPOGTight_TTrig_TC")            return passTightID_NoCC()&&Pass_TriggerEmulation()&&IsGsfCtfScPixChargeConsistent();
  if(ID=="passPOGTight_TTrig_IP4")        return passTightID_NoCC()&&(fabs(IP3D()/IP3Derr())<4.)&&Pass_TriggerEmulation();
  if(ID=="passPOGTight_TTrig_HNTC")       return passTightID_NoCC()&&PassHNID();
  if(ID=="passPOGTightv1_TTrig_HNTC")       return Pass_HNTight(999.0, 0.02, 0.02, 0.1,4,4);
  if(ID=="passPOGTightv2_TTrig_HNTC")       return Pass_HNTight(999.0, 0.05, 0.05, 0.1,4,4);

  if(ID=="passPOGTight_TTrig_HNTC_IP4")   return passTightID_NoCC()&&PassHNID()&&(fabs(IP3D()/IP3Derr())<4.);
  if(ID=="passPOGTight_TTrig_HNTC_IP45")  return passTightID_NoCC()&&PassHNID()&&(fabs(IP3D()/IP3Derr())<4.5);
  if(ID=="passPOGTight_TTrig_HNTC_IP5")   return passTightID_NoCC()&&PassHNID()&&(fabs(IP3D()/IP3Derr())<5.);




  if(ID=="passPOGTight_TTrig_HNTC_100_100")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(100.,100.);
  if(ID=="passPOGTight_TTrig_HNTC_100_150")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(100.,150.);
  if(ID=="passPOGTight_TTrig_HNTC_100_200")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(100.,200.);
  if(ID=="passPOGTight_TTrig_HNTC_100_250")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(100.,250.);
  if(ID=="passPOGTight_TTrig_HNTC_100_300")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(100.,300.);
  if(ID=="passPOGTight_TTrig_HNTC_100_350")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(100.,350.);
  if(ID=="passPOGTight_TTrig_HNTC_100_400")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(100.,400.);

  if(ID=="passPOGTight_TTrig_HNTC_150_200")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(150.,200.);
  if(ID=="passPOGTight_TTrig_HNTC_150_150")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(150.,150.);

  if(ID=="passPOGTight_TTrig_HNTC_150_250")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(150.,250.);
  if(ID=="passPOGTight_TTrig_HNTC_150_300")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(150.,300.);
  if(ID=="passPOGTight_TTrig_HNTC_150_350")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(150.,350.);
  if(ID=="passPOGTight_TTrig_HNTC_150_400")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(150.,400.);


  if(ID=="passPOGTight_TTrig_HNTC_200_200")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(200.,200.);
  if(ID=="passPOGTight_TTrig_HNTC_200_250")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(200.,250.);
  if(ID=="passPOGTight_TTrig_HNTC_200_300")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(200.,300.);
  if(ID=="passPOGTight_TTrig_HNTC_200_350")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(200.,350.);
  if(ID=="passPOGTight_TTrig_HNTC_200_400")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(200.,400.);


  if(ID=="passPOGTight_TTrig_HNTC_250_250")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(250.,250.);
  if(ID=="passPOGTight_TTrig_HNTC_250_300")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(250.,300.);
  if(ID=="passPOGTight_TTrig_HNTC_250_350")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(250.,350.);
  if(ID=="passPOGTight_TTrig_HNTC_250_400")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(250.,400.);

  if(ID=="passPOGTight_TTrig_HNTC_300_300")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(300.,300.);
  if(ID=="passPOGTight_TTrig_HNTC_300_350")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(300.,350.);
  if(ID=="passPOGTight_TTrig_HNTC_300_400")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(300.,400.);
  
  if(ID=="passPOGTight_TTrig_HNTC_350_350")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(350.,350.);
  if(ID=="passPOGTight_TTrig_HNTC_350_400")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(350.,400.);

  if(ID=="passPOGTight_TTrig_HNTC_400_400")       return passTightID_NoCC()&&  Pass_TriggerEmulation()&& PassConversionVeto()&&PassHNIsGsfCtfScPixChargeConsistentVar(400.,400.);



  
  if(ID=="passPOGMedium")            return passMediumID_NoCC();
  if(ID=="passPOGMedium_TC")         return passMediumID_NoCC()&&IsGsfCtfScPixChargeConsistent();
  if(ID=="passPOGMedium_TC_IP4")     return passMediumID_NoCC()&&IsGsfCtfScPixChargeConsistent()&&(fabs(IP3D()/IP3Derr())<4.);
  if(ID=="passPOGMedium_HNTC")       return passMediumID_NoCC()&&PassHNID();
  if(ID=="passPOGMedium_HNTC_IP4")   return passMediumID_NoCC()&&PassHNID()                     &&(fabs(IP3D()/IP3Derr())<4.);


     
  if(ID=="HNTight2016") return Pass_HNTight2016()&&(fabs(dXY())<0.01);  // EXO-17-028
  if(ID=="HNTight2016Update") return Pass_HNTight2016();  // EXO-17-028
  if(ID=="HNTight_dxy05_10_dz01_ip4_4") return Pass_HNTight(999.0, 0.05, 0.1, 0.1,4,4);
  if(ID=="HNTight_dxy05_10_dz01_ip4_5") return Pass_HNTight(999.0, 0.05, 0.1, 0.1,4,5);
  if(ID=="HNTight_dxy05_05_dz01_ip4_4") return Pass_HNTight(999.0, 0.05, 0.05, 0.1,4,4);
  if(ID=="HNTight_dxy05_05_dz01_ip4_5") return Pass_HNTight(999.0, 0.05, 0.05, 0.1,4,5);
  if(ID=="HNTight_dxy05_10_dz01_ip3_4") return Pass_HNTight(999.0, 0.05, 0.1, 0.1,3,4);
  if(ID=="HNTight_dxy05_10_dz01_ip3_3") return Pass_HNTight(999.0, 0.05, 0.1, 0.1,3,3);
  if(ID=="HNTight_dxy05_05_dz01_ip3_4") return Pass_HNTight(999.0, 0.05, 0.05, 0.1,3,4);
  if(ID=="HNTight_dxy05_05_dz01_ip3_3") return Pass_HNTight(999.0, 0.05, 0.05, 0.1,3,3);

  if(ID=="HNTight_dxy02_05_dz01_ip4_4") return Pass_HNTight(999.0, 0.02, 0.05, 0.1,4,4);
  if(ID=="HNTight_dxy02_05_dz01_ip4_5") return Pass_HNTight(999.0, 0.02, 0.05, 0.1,4,5);
  if(ID=="HNTight_dxy02_02_dz01_ip4_4") return Pass_HNTight(999.0, 0.02, 0.02, 0.1,4,4);
  if(ID=="HNTight_dxy02_02_dz01_ip4_5") return Pass_HNTight(999.0, 0.02, 0.02, 0.1,4,5);
  if(ID=="HNTight_dxy02_02_dz01_ip3_4") return Pass_HNTight(999.0, 0.02, 0.02, 0.1,3,4);
  if(ID=="HNTight_dxy02_02_dz01_ip3_3") return Pass_HNTight(999.0, 0.02, 0.02, 0.1,3,3);

  if(ID=="HNTight_dxy05_10_dz01_ip999") return Pass_HNTight(999.0, 0.05, 0.1, 0.1,999.,999.);
  if(ID=="HNTight_dxy05_05_dz01_ip999") return Pass_HNTight(999.0, 0.05, 0.05, 0.1,999.,999.);
  if(ID=="HNTight_dxy02_05_dz01_ip999") return Pass_HNTight(999.0, 0.02, 0.05, 0.1,999.,999.);
  if(ID=="HNTight_dxy02_02_dz01_ip999") return Pass_HNTight(999.0, 0.02, 0.02, 0.1,999.,999.);
  
    
  if(ID=="HNMedium_dxy05_10_dz01_ip4_4") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,4,4);
  if(ID=="HNMedium_dxy05_10_dz01_ip4_5") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,4,5);
  if(ID=="HNMedium_dxy05_05_dz01_ip4_4") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,4,4);
  if(ID=="HNMedium_dxy05_05_dz01_ip4_5") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,4,5);
  if(ID=="HNMedium_dxy05_10_dz01_ip3_4") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,3,4);
  if(ID=="HNMedium_dxy05_10_dz01_ip3_3") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,3,3);
  if(ID=="HNMedium_dxy05_10_dz01_ip3_4") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,3,4);
  if(ID=="HNMedium_dxy05_10_dz01_ip3_3") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,3,3);

  if(ID=="HNMedium_dxy02_05_dz01_ip4_4") return Pass_HNMedium(999.0, 0.02, 0.05, 0.1,4,4);
  if(ID=="HNMedium_dxy02_05_dz01_ip4_5") return Pass_HNMedium(999.0, 0.02, 0.05, 0.1,4,5);
  if(ID=="HNMedium_dxy02_02_dz01_ip4_4") return Pass_HNMedium(999.0, 0.02, 0.02, 0.1,4,4);
  if(ID=="HNMedium_dxy02_02_dz01_ip4_5") return Pass_HNMedium(999.0, 0.02, 0.02, 0.1,4,5);
  if(ID=="HNMedium_dxy02_02_dz01_ip3_4") return Pass_HNMedium(999.0, 0.02, 0.02, 0.1,3,4);
  if(ID=="HNMedium_dxy02_02_dz01_ip3_3") return Pass_HNMedium(999.0, 0.02, 0.02, 0.1,3,3);

  if(ID=="HNMedium_dxy05_10_dz01_ip999") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,999.,999.);
  if(ID=="HNMedium_dxy05_05_dz01_ip999") return Pass_HNMedium(999.0, 0.05, 0.1, 0.1,999.,999.);
  if(ID=="HNMedium_dxy02_05_dz01_ip999") return Pass_HNMedium(999.0, 0.02, 0.05, 0.1,999.,999.);
  if(ID=="HNMedium_dxy02_02_dz01_ip999") return Pass_HNMedium(999.0, 0.02, 0.02, 0.1,999.,999.);
  
  if(ID=="HEEPv7")  return passHEEPID();
  if(ID=="HNHEEPv7")  return passHEEPID()&&PassHNID();

  if(ID=="HNHEEP2018")  return passHEEP2018Prompt()&&PassHNID();
  if(ID=="HEEP2018")  return passHEEP2018Prompt();

  if(ID=="HEEPLoose")  return passLooseHEEPID()&&PassHNID();


  if(ID=="HNLoose")   return  Pass_HNLooseID(0.6,0.2,0.1,10); // non POG IP/ISO                                               
  if(ID=="HNLooseV2")   return  Pass_HNLooseID(0.6,0.2,0.1,9999); // non POG IP/ISO                                                
  if(ID=="HNLooseV3")   return  Pass_HNLooseID(0.6,0.02,0.1,4.); // non POG IP/ISO                                                
  if(ID=="HNLooseV4")   return  Pass_HNLooseID(0.6,0.05,0.1,4.); // non POG IP/ISO                                                

  if(ID=="passMVAID_noIso_WP80") return passMVAID_noIso_WP80HN(); // MVA 80 + iso 0.08 + POG IP + HNCC
  if(ID=="passMVAID_noIso_WP90") return passMVAID_noIso_WP90HN(); // MVA 90 + iso 0.08 + POG IP + HNCC 
  if(ID=="passMVAID_noIso_WP90V2") return passMVAID_noIso_WP90XHN(0.08,0.02,0.05,4); // MVA 90 + iso 0.08 + POG IP + HNCC 
  if(ID=="passMVAID_noIso_WP90V3") return passMVAID_noIso_WP90XHN(0.08,0.05,0.1,4); // MVA 90 + iso 0.08 + POG IP + HNCC _
  if(ID=="passMVAID_noIso_WP90V4") return passMVAID_noIso_WP90XHN(0.08,0.02,0.05,5); // MVA 90 + iso 0.08 + POG IP + HNCC         
  if(ID=="passMVAID_noIso_WP90V5") return passMVAID_noIso_WP90XHN(0.08,0.05,0.1,5); // MVA 90 + iso 0.08 + POG IP + HNCC _       
  if(ID=="passMVAID_noIso_WP90V6") return passMVAID_noIso_WP90XHN(0.08,0.02,0.05,999.); // MVA 90 + iso 0.08 + POG IP + HNCC         
  if(ID=="passMVAID_noIso_WP90V7") return passMVAID_noIso_WP90XHN(0.08,0.05,0.1,999.); // MVA 90 + iso 0.08 + POG IP + HNCC _        
  if(ID=="passMVAID_noIso_WP90V8") return passMVAID_noIso_WP90XHN(0.08,0.02,0.05,4); // MVA 90 + iso 0.08 + POG IP + HNCC                      
  if(ID=="passMVAID_noIso_WP90V9") return passMVAID_noIso_WP90XHN(0.06,0.05,0.1,4); // MVA 90 + iso 0.08 + POG IP + HNCC _                     
  if(ID=="passMVAID_noIso_WP90V10") return passMVAID_noIso_WP90XHN(0.06,0.02,0.05,5); // MVA 90 + iso 0.08 + POG IP + HNCC                      
  if(ID=="passMVAID_noIso_WP90V11") return passMVAID_noIso_WP90XHN(0.06,0.05,0.1,5); // MVA 90 + iso 0.08 + POG IP + HNCC _                     
  if(ID=="passMVAID_noIso_WP90V12") return passMVAID_noIso_WP90XHN(0.06,0.02,0.05,999.); // MVA 90 + iso 0.08 + POG IP + HNCC                   
  if(ID=="passMVAID_noIso_WP90V13") return passMVAID_noIso_WP90XHN(0.06,0.05,0.1,999.); // MVA 90 + iso 0.08 + POG IP + HNCC _                  
  if(ID=="passMVAID_noIso_WP90V14") return passMVAID_noIso_WP90XHN(0.05,0.02,0.05,4); // MVA 90 + iso 0.08 + POG IP + HNCC                      
  if(ID=="passMVAID_noIso_WP90V15") return passMVAID_noIso_WP90XHN(0.05,0.05,0.1,4); // MVA 90 + iso 0.08 + POG IP + HNCC _                     
  if(ID=="passMVAID_noIso_WP90V16") return passMVAID_noIso_WP90XHN(0.05,0.02,0.05,5); // MVA 90 + iso 0.08 + POG IP + HNCC                      
  if(ID=="passMVAID_noIso_WP90V17") return passMVAID_noIso_WP90XHN(0.05,0.05,0.1,5); // MVA 90 + iso 0.08 + POG IP + HNCC _                     
  if(ID=="passMVAID_noIso_WP90V18") return passMVAID_noIso_WP90XHN(0.05,0.02,0.05,999.); // MVA 90 + iso 0.08 + POG IP + HNCC                   
  if(ID=="passMVAID_noIso_WP90V19") return passMVAID_noIso_WP90XHN(0.05,0.05,0.1,999.); // MVA 90 + iso 0.08 + POG IP + HNCC _                  

  if(ID=="passMVAID_iso_WP80") return passMVAID_Iso_WP80HN(); // isoMVA 80 + POG IP + HNCC 
  if(ID=="passMVAID_iso_WP90") return passMVAID_Iso_WP90HN(); // isoMVA 90 + POG IP + HNCC 
  if(ID=="passPOGMVAID_noIso_WP80") return passPOGMVAID_noIso_WP80HN(); // MVA 80 + iso 0.08 + POG IP + HNCC                            
  if(ID=="passPOGMVAID_noIso_WP90") return passPOGMVAID_noIso_WP90HN(); // MVA 90 + iso 0.08 + POG IP + HNCC                            
  if(ID=="passPOGMVAID_iso_WP80") return passPOGMVAID_Iso_WP80HN(); // isoMVA 80 + POG IP + HNCC                                        
  if(ID=="passPOGMVAID_iso_WP90") return passPOGMVAID_Iso_WP90HN(); // isoMVA 90 + POG IP + HNCC          

  /// Loose+veto
  if(ID=="passLooseID_nocc") return passLooseID_NoCC();

  if(ID=="HNVeto2016") return Pass_HNVeto2016();
  if(ID=="HNLoose2016") return Pass_HNLoose2016(0.6, 0.2, 0.1, 10.);


  if(ID=="HNVeto") return Pass_HNVeto(0.6);
  //  if(ID=="HNLoose") return Pass_HNLoose(0.6, 999., 999.);
  if(ID=="HNLoosest") return Pass_HNLoosest();
  
  if(ID=="passMVAID_noIso_WP80Loose") return passMVAID_noIso_WP80HNLoose();
  if(ID=="passMVAID_noIso_WP90Loose") return passMVAID_noIso_WP90HNLoose();
  if(ID=="passMVAID_iso_WP80Loose") return passMVAID_Iso_WP80HNLoose();
  if(ID=="passMVAID_iso_WP90Loose") return passMVAID_Iso_WP90HNLoose();

  if(ID=="passMVAID_noIso_WP90LooseV2") return Pass_HNLoose2016MVANonIso(0.6,0.02,0.05,5);
  
  

  if(ID=="HNMVALoose") return Pass_HNMVALoose(0.6, 999., 999.);
  if(ID=="HNMVATight") return Pass_HNMVATight(0.08, 0.05, 0.1);

  if(ID=="CutBasedLooseNoIso") return Pass_CutBasedLooseNoIso();
  if(ID=="CutBasedMediumNoIso") return Pass_CutBasedMediumNoIso();
  if(ID=="MVALooseNoIso") return passMVAID_noiso_WPLoose();
  if(ID=="CutBasedVetoNoIso") return Pass_CutBasedVetoNoIso();
  cout << "[Electron::PassID] No id : " << ID << endl;
  exit(EXIT_FAILURE);

  return false;
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

  if(! (Pass_TriggerEmulationLoose()) ) return false;

  return true;
}

bool Electron::PassHNID() const{
  
  if(! (PassConversionVeto()) ) return false;
  if(! PassHNIsGsfCtfScPixChargeConsistent())  return false;

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

  if(! (Pass_TriggerEmulationLoose()) ) return false;

  return true;
}

bool Electron::passTightIDHN() const{
  if(! (passTightID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  //if(! (Pass_TriggerEmulationLoose()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) )return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  }
  else     if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2 && fabs(IP3D()/IP3Derr()) < 4.)) return false;


  return true;
}
bool Electron::passMediumIDHN() const{
  if(! (passMediumID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) )return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 4.)) return false;
  }
  else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2 && fabs(IP3D()/IP3Derr()) < 4.)) return false;




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
  if(! (Pass_TriggerEmulationLoose()) ) return false;
  if( fabs(scEta())<= 1.479 ){
    if( !(fabs(dXY()) < 0.05 && fabs(dZ())< 0.1)) return false;
  }
    else   if( !(fabs(dXY()) < 0.1 && fabs(dZ())< 0.2)) return false;

  
  return true;
}

bool Electron::passTightID_NoCCB() const {

  if(! (passTightID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulationLoose()) ) return false;
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
  if(! (Pass_TriggerEmulationLoose()) ) return false;
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
  return PassHNIsGsfCtfScPixChargeConsistentVar(300., 350.);
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

bool Electron::passMVAID_noIso_WP90XHN(double relisoCut, double dxyCut, double dzCut, double sipCut) const {
  if(!(passMVAID_noIso_WP90())) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()))return false;
  if(! (RelIso()<relisoCut) ) return false;
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

bool Electron::Pass_HNTight2016() const{
  if( fabs(scEta()) <= 0.8 ){
    if(! (MVANoIso()>0.9) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVANoIso()>0.825) ) return false;
  }
  else{
    if(! (MVANoIso()>0.5) ) return false;
  }
  if(! (fabs(dXY())<0.02 && fabs(dZ())<0.04 && fabs(IP3D()/IP3Derr())<4.) ) return false;
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
  if(! PassHNIsGsfCtfScPixChargeConsistent()) return false;

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
