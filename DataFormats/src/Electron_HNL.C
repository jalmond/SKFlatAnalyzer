#include "Electron.h"

int  Electron::PassHNLTight(TString ID) const{

  
  /// These are Finalised Tight IDs used in POG/HNL or other analysis                                                                                     
  

  /// Loose MVA used to train                                                                                                                                         
  if(ID=="MVALoose")          return Pass_LepMVAID() ? 1 : 0 ;

  // breakdown of ID for checking Eff.                                                                                                                                
  if(ID=="HNL_ULID_Baseline") return PassMVABaseLine() ? 1 : 0 ;


  ////////////////////////////////////////////////////////////////////////////////////                                                                                                                                
  ////////////////////////////////////////////////////////////////////////////////////                                                                                                                                
  ////// HNL UltraLegacy ID (HNL_ULID)                                                                                                                                                                                
  ////////////////////////////////////////////////////////////////////////////////////                                                                                                                                
  ////////////////////////////////////////////////////////////////////////////////////                                                                                                                                

  ////////////////////////////////////////////////////////////////////////////////////                                                                                                                                
  ///  Era Scanned ID                                                                                                                                                                                                 
  ////////////////////////////////////////////////////////////////////////////////////                                                                                                                                
  TString Year = "";
  if(ID.Contains("2016")) Year = "2016";
  else if(ID.Contains("2017")) Year = "2017";
  else if(ID.Contains("2018")) Year = "2018";

  //// TC = Tight Charge                                                                                                                                                                                              
  //// *TC* ID are IDs before updating Charge cut to a MEDIUM charge cut to reduce CF at high pt                                                                                                                      
  if(ID == "HNL_TC0_ULID_"     +Year)     return (PassID("HNL_ULID_Defv1_FO_"+Year)    && PassID("HNL_ULID_Fake_"+Year));
  if(ID == "HNL_TC1_ULID_"     +Year)     return (PassID("HNL_ULID_Defv1_FO_"+Year)    && PassID("HNL_ULID_Fake_"+Year) && IsGsfCtfChargeConsistent());
  if(ID == "HNL_TC2_ULID_"     +Year)     return (PassID("HNL_ULID_Defv1_FO_"+Year)    && PassID("HNL_ULID_Fake_"+Year) && IsGsfScPixChargeConsistent());
  if(ID == "HNL_TC3_ULID_"     +Year)     return (PassID("HNL_ULID_Defv1_FO_"+Year)    && PassID("HNL_ULID_Fake_"+Year) && IsGsfCtfChargeConsistent() && IsGsfScPixChargeConsistent());

  //// HNL_ULID_Defv2_FO_ has Medium Charge cut on top of HNL_ULID_Defv1_FO_                                                                                                                                           
  if(ID == "HNL_ULID_"         +Year)     return (PassID("HNL_ULID_Defv2_FO_"+Year)    && PassID("HNL_ULID_Fake_"+Year));
  if(ID == "HNL_ULID_NoMCC_"     +Year)   return (PassID("HNL_ULID_Defv1_FO_"+Year)    && PassID("HNL_ULID_Fake_"+Year));

  ///// Check Side Band for conv inclusion                                                                                                                                                                            
  if(ID == "HNL_ULID_CONVSB_"+Year ) {
    return  PassID("HNL_ULID_CF_"+Year) &&  IsGsfCtfChargeConsistent();
  }


  // Loose Fake IDs [FO]                                                                                                                                                                                              

  // HNL_ULID_Defv1_FO* are OLD FO ie before changing Charge cut                                                                                                                                                     
  if(ID == "HNL_ULID_Defv1_FO_"  +Year)   return (PassID("HNL_ULID_Conv_"+Year)   && PassID("HNL_ULID_CF_"+Year));
  // HNL_ULID_Defv2_FO_ is new default FO ID WIth medium charge cut                                                                                                                                                     
  if(ID == "HNL_ULID_Defv2_FO_"  +Year)      return (PassID("HNL_ULID_Conv_"+Year)   && PassID("HNL_ULID_CF_"+Year) &&  IsGsfCtfChargeConsistent());

  /// FO = Fake Obj, is main obj in NP bkg estimate                                                                                                                                                                   
  if(ID == "HNL_ULID_FO_2016a")     return PassID("HNL_ULID_FO_v9_a_"+Year);
  if(ID == "HNL_ULID_FO_2016b")     return PassID("HNL_ULID_FO_v9_a_"+Year);
  if(ID == "HNL_ULID_FO_2017")      return PassID("HNL_ULID_FO_v9_a_"+Year);
  if(ID == "HNL_ULID_FO_2018")      return PassID("HNL_ULID_FO_v9_a_"+Year);

  if(ID == "HNL_ULID_FOUp_2016a")     return PassID("HNL_ULID_FO_v8_a_"+Year);
  if(ID == "HNL_ULID_FOUp_2016b")     return PassID("HNL_ULID_FO_v8_a_"+Year);
  if(ID == "HNL_ULID_FOUp_2017")      return PassID("HNL_ULID_FO_v8_a_"+Year);
  if(ID == "HNL_ULID_FOUp_2018")      return PassID("HNL_ULID_FO_v8_a_"+Year);

  if(ID == "HNL_ULID_FODown_2016a")     return PassID("HNL_ULID_FO_v0_"+Year);
  if(ID == "HNL_ULID_FODown_2016b")     return PassID("HNL_ULID_FO_v0_"+Year);
  if(ID == "HNL_ULID_FODown_2017")      return PassID("HNL_ULID_FO_v0_"+Year);
  if(ID == "HNL_ULID_FODown_2018")      return PassID("HNL_ULID_FO_v0_"+Year);


  if(ID == "HNL_ULID_v1_FO_2016a")     return PassID("HNL_ULID_v1_FO_v9_a_"+Year);
  if(ID == "HNL_ULID_v1_FO_2016b")     return PassID("HNL_ULID_v1_FO_v9_a_"+Year);
  if(ID == "HNL_ULID_v1_FO_2017")      return PassID("HNL_ULID_v1_FO_v9_a_"+Year);
  if(ID == "HNL_ULID_v1_FO_2018")      return PassID("HNL_ULID_v1_FO_v9_a_"+Year);




  /// FCO = Fake+Conv Object to check Conv bkg                                                                                                                                                                        
  if(ID == "HNL_ULID_Def_FO_CFO_"  +Year)      return (PassID("HNL_ULID_CF_"+Year) &&  IsGsfCtfChargeConsistent());

  //// Now List Fake DeepJet Scan IDs v1-9 a,b,c....                                                                                                                                                                  
  double CloseJet_PtratioCut = 0.4;
  if(Year=="2016") CloseJet_PtratioCut = 0.5;

  if(ID.Contains("HNL_ULID_FCO_")){
    if(!PassID("HNL_ULID_Def_FCO_"+Year)) return false;
    if(!PassID("HNL_ULID_Fake_"+Year)){
      if(this->HasCloseJet()){
        if(CloseJet_Ptratio() < CloseJet_PtratioCut)         return false;
      }
    }
    return true;
  }


  if(ID.Contains("HNL_ULID_FO_")){

    ///// V0 ID is BASIC loose ID with NO DEEPJET cuts                                                                                                                                                                
    double BJetDeepJetCut = 999;
    if(ID.Contains("HNL_ULID_FO_v1")) BJetDeepJetCut = 0.025;
    if(ID.Contains("HNL_ULID_FO_v2")) BJetDeepJetCut = 0.05;   //// Version 2 == TOP                                                                                                                                  
    if(ID.Contains("HNL_ULID_FO_v3")) BJetDeepJetCut = 0.1;
    if(ID.Contains("HNL_ULID_FO_v4")) BJetDeepJetCut = 0.2;
    if(ID.Contains("HNL_ULID_FO_v5")) BJetDeepJetCut = 0.3;
    if(ID.Contains("HNL_ULID_FO_v6")) BJetDeepJetCut = 0.4;
    if(ID.Contains("HNL_ULID_FO_v7")) BJetDeepJetCut = 0.5;
    if(ID.Contains("HNL_ULID_FO_v8")) BJetDeepJetCut = 0.75;
    if(ID.Contains("HNL_ULID_FO_v9")) BJetDeepJetCut = 0.9;
    bool ApplyCloseJet_CvsBScore=false;
    bool ApplyCloseJet_CvsLScore=false;
    double CBJetDeepJetCut = -999;
    double CLJetDeepJetCut = 999;

    if(ID.Contains("_b")){
      ApplyCloseJet_CvsBScore = true;
      ApplyCloseJet_CvsLScore= true;
      CBJetDeepJetCut=0.1;
      CLJetDeepJetCut=0.6;
    }
    if(ID.Contains("_c")){
      ApplyCloseJet_CvsBScore = true;
      ApplyCloseJet_CvsLScore= true;
      CBJetDeepJetCut=0.1;
      CLJetDeepJetCut=0.8;
    }

    if(!PassID("HNL_ULID_Defv2_FO_"+Year)) return false;
    if(!PassID("HNL_ULID_Fake_"+Year)){
      if(this->HasCloseJet()){
        if(CloseJet_Ptratio() < CloseJet_PtratioCut)         return false;
        if(this->CloseJet_BScore()    >  BJetDeepJetCut) return false;
        if(ApplyCloseJet_CvsBScore && this->CloseJet_CvsBScore() <  CBJetDeepJetCut ) return false;
        if(ApplyCloseJet_CvsLScore && this->CloseJet_CvsLScore() >  CLJetDeepJetCut ) return false;
      }
    }
    return true;
  }



  /// OLD FUNCTION OLNY USed now for comparing perfomance                                                                                                                                                             

  if(ID.Contains("HNL_ULID_v1_FO_")){

    ///// V0 ID is BASIC loose ID with NO DEEPJET cut                                                                                                                                                                                                                  
    double BJetDeepJetCut = 999;
    if(ID.Contains("HNL_ULID_v1_FO_v1")) BJetDeepJetCut = 0.025;
    if(ID.Contains("HNL_ULID_v1_FO_v2")) BJetDeepJetCut = 0.05; //// Version 2 == TOP                                                                                                                                
    if(ID.Contains("HNL_ULID_v1_FO_v3")) BJetDeepJetCut = 0.1;
    if(ID.Contains("HNL_ULID_v1_FO_v4")) BJetDeepJetCut = 0.2;
    if(ID.Contains("HNL_ULID_v1_FO_v5")) BJetDeepJetCut = 0.3;
    if(ID.Contains("HNL_ULID_v1_FO_v6")) BJetDeepJetCut = 0.4;
    if(ID.Contains("HNL_ULID_v1_FO_v7")) BJetDeepJetCut = 0.5;
    if(ID.Contains("HNL_ULID_v1_FO_v8")) BJetDeepJetCut = 0.75;
    if(ID.Contains("HNL_ULID_v1_FO_v9")) BJetDeepJetCut = 0.9;
    bool ApplyCloseJet_CvsBScore=false;
    bool ApplyCloseJet_CvsLScore=false;
    double CBJetDeepJetCut = -999;
    double CLJetDeepJetCut = 999;
    if(ID.Contains("_b")){
      ApplyCloseJet_CvsBScore = true;      ApplyCloseJet_CvsLScore= true;
      CBJetDeepJetCut=0.1;      CLJetDeepJetCut=0.6;
    }
    if(ID.Contains("_c")){
      ApplyCloseJet_CvsBScore = true;      ApplyCloseJet_CvsLScore= true;
      CBJetDeepJetCut=0.1;      CLJetDeepJetCut=0.8;
    }
    if(!PassID("HNL_ULID_Defv1_FO_"+Year)) return false;
    if(!PassID("HNL_ULID_Fake_"+Year)){
      if(this->HasCloseJet()){
        if(CloseJet_Ptratio() < CloseJet_PtratioCut)         return false;
        if(this->CloseJet_BScore()    >  BJetDeepJetCut) return false;
        if(ApplyCloseJet_CvsBScore && this->CloseJet_CvsBScore() <  CBJetDeepJetCut ) return false;
        if(ApplyCloseJet_CvsLScore && this->CloseJet_CvsLScore() >  CLJetDeepJetCut ) return false;
      }
    }
    return true;
  }

  ////////////// SINGLE MVA CUTS [Split for ID SF]                                                                                                                                                                    

  ////  ID cuts is Cut1*CUt2*Cut3, for each era... Here we define and apply each cut                                                                                                                                  
  /// 2016                                                                                                                                                                                                            
  if(ID == "HNL_ULID_Conv_2016" ) return (PassMVABaseLine() && Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.) , GetPtSlopeCut(20,60,-0.7,0.) ,   "Conv_v5"));
  if(ID == "HNL_ULID_CF_2016"   ) return (PassMVABaseLine() && Pass_MVA_BBEC("CF_EDv5",   el_mva_cut_cf_2016_B,   el_mva_cut_cf_2016_EC,     "CF_v5"));
  if(ID == "HNL_ULID_Fake_2016" ) return (PassMVABaseLine() && Pass_MVA_BBEC("Fake_EDv5", el_mva_cut_fake_2016_B, el_mva_cut_fake_2016_EC,   "Fake_v5"));
  /// 2017                                                                                                                                                                                                            
  if(ID == "HNL_ULID_Conv_2017" ) return (PassMVABaseLine() && Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.4,0.) , GetPtSlopeCut(20,50,-0.4,0.2) ,   "Conv_v5"));
  if(ID == "HNL_ULID_CF_2017"   ) return (PassMVABaseLine() && Pass_MVA_BBEC("CF_EDv5",   el_mva_cut_cf_2017_B,   el_mva_cut_cf_2017_EC,     "CF_v5"));
  if(ID == "HNL_ULID_Fake_2017" ) return (PassMVABaseLine() && Pass_MVA_BBEC("Fake_EDv5", el_mva_cut_fake_2017_B, el_mva_cut_fake_2017_EC,   "Fake_v5"));
  /// 2018                                                                                                                                                                                                            
  if(ID == "HNL_ULID_Conv_2018" ) return (PassMVABaseLine() && Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.4,0.2) , GetPtSlopeCut(20,50,-0.4,0.2) ,   "Conv_v5"));
  if(ID == "HNL_ULID_CF_2018"   ) return (PassMVABaseLine() && Pass_MVA_BBEC("CF_EDv5",   el_mva_cut_cf_2018_B ,  el_mva_cut_cf_2018_EC ,     "CF_v5")) ;
  if(ID == "HNL_ULID_Fake_2018" ) return (PassMVABaseLine() && Pass_MVA_BBEC("Fake_EDv5", el_mva_cut_fake_2018_B, el_mva_cut_fake_2018_EC ,   "Fake_v5"));

  return -1;
}



bool Electron::Pass_LepMVATopID() const {

  /// ID USED IN BDT by TOP group             
                                                                                                                                                                                                                   
  if(this->Pt() < 10) return false;
  if(this->fEta() > 2.5) return false;
  if(MiniRelIso() > 0.4) return false;
  if(NMissingHits() > 1) return false;
  if(SIP3D() > 8) return false;
  if(this->fdXY() > 0.05) return false;
  if(this->fdZ() > 0.1) return false;
  return true;

}



bool Electron::Pass_LepMVAID() const {

  /// ID USED IN BDT == TopMVA LooseID + trigger emulation                                                                                                                                                            

  if(!passMVAID_noiso_WPLoose()) return false;
  if(!Pass_TriggerEmulationLoose())  return false;
  if(this->Pt() < 10) return false;
  if(this->fEta() > 2.5) return false;
  if(MiniRelIso() > 0.4) return false;
  if(NMissingHits() > 1) return false;
  if(SIP3D() > 8) return false;
  if(this->fdXY() > 0.05) return false;
  if(this->fdZ() > 0.1) return false;
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
  if(! IsGsfCtfScPixChargeConsistent()) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  return true;
}

bool Electron::Pass_HNMVALoose(double relisoCut, double dxyCut, double dzCut) const{
  if(!( passMVAID_noiso_WPLoose() )) return false;
  if(! (RelIso()<relisoCut) ) return false;
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

bool Electron::Pass_HNVetoMVA() const{

  if(!passMVAID_noiso_WPLoose()) return false;

  if(! (fabs(dXY())<0.2 && fabs(dZ())<0.5) ) return false;

  if(! (MiniRelIso() < 0.4) ) return false; /// change to mini iso to be consistent with LooseMVA cut                                                                                                                 

  return true;
}

bool Electron::Pass_HNLoose2016MVAISO(double dxyCut, double dzCut, double sipCut) const{

  if( fabs(scEta()) <= 0.8 ){
    if(! (MVAIso()>-0.99) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVAIso()>-0.99) ) return false;
  }
  else{
    if(! (MVAIso()>-0.99) ) return false;
  }
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<sipCut) ) return false;

  if(! (PassConversionVeto()) ) return false;
  if(! IsGsfCtfScPixChargeConsistent())  return false;

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
    if(! (MVANoIso()>-0.99) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVANoIso()>-0.99) ) return false;
  }
  else{
    if(! (MVANoIso()>-0.99) ) return false;
  }
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<sipCut) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! IsGsfCtfScPixChargeConsistent())  return false;

  if(! (Pass_TriggerEmulation()) ) return false;

  return true;
}




bool Electron::Pass_HNLoose2016(double relisoCut, double dxyCut, double dzCut, double sipCut) const{
  if( fabs(scEta()) <= 0.8 ){
    if(! (MVANoIso()>-0.99) ) return false;
  }
  else if( fabs(scEta()) > 0.8 && fabs(scEta()) <= 1.479 ){
    if(! (MVANoIso()>-0.99) ) return false;
  }
  else{
    if(! (MVANoIso()>-0.99) ) return false;
  }
  if(! (fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<sipCut) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) ) return false;

  if(! (Pass_TriggerEmulation()) ) return false;

  return true;
}



bool Electron::passMediumIDHN() const{
  if(! (passMediumID()) ) return false;
  if(! (PassConversionVeto()) ) return false;
  if(! (Pass_TriggerEmulation()) ) return false;
  if(! (IsGsfCtfScPixChargeConsistent()) )return false;

  return true;
}



bool Electron::Pass_HNTight2016() const{


  if(!PassMVA_2016(0.9, 0.825, 0.5)) return false;

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

bool Electron::Pass_HNVetoCutBased(double relisoCut) const{

  if(! (Pass_CutBasedVetoNoIso()) ) return false;
  if(! (RelIso()<relisoCut) ) return false;
  if(! (fabs(dXY())<0.2 && fabs(dZ())<0.5) ) return false;
  return true;
}

bool Electron::Pass_HNLoosest() const{

  if(! (RelIso()< 0.6 || MiniRelIso() < 0.4) ) return false;
  if(!( Pass_CutBasedVetoNoIso() || passMVAID_noiso_WPLoose()))  return false;

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

  if(! IsGsfCtfScPixChargeConsistent()) return false;

  if(! (Pass_TriggerEmulation()) ) return false;
  return true;
}



