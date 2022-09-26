#include "Muon.h"
#include <iostream>
#include <sstream>
#include <string>

ClassImp(Muon)

Muon::Muon() : Lepton() {
  j_chi2 = 999.;
  j_PFCH04 = -999.;
  j_PFNH04 = -999.;
  j_PFPH04 = -999.;
  j_PU04 = -999.;
  j_trkiso = -999.;
  this->SetLeptonFlavour(MUON);
  j_MiniAODPt = -999.;
  j_MomentumScaleUp = -999.;
  j_MomentumScaleDown = -999.;
  j_TunePPtError = -999.;
  j_MVA = -999.;
  j_lowptMVA = -999.;
  j_softMVA = -999.;
  j_validmuonhits = 0;
  j_matchedstations = 0;
  j_pixelHits = 0;
  j_trackerLayers = 0;
}

Muon::~Muon(){
}

void Muon::SetTypeBit(unsigned int typebit){
  j_TypeBit = typebit;
}

void Muon::SetIDBit(unsigned int idbit){
  j_IDBit = idbit;
}

void Muon::SetisPOGHighPt(bool b){
  j_isPOGHighPt = b;
}


void Muon::SetPOGMediumHIP(bool ismedium_hip, bool ismedium_nohip){
  j_ismedium_hip = ismedium_hip;
  j_ismedium_nohip = ismedium_nohip;
}
void Muon::SetIso(double ch04, double nh04, double ph04, double pu04, double trkiso){
  j_PFCH04 = ch04;
  j_PFNH04 = nh04;
  j_PFPH04 = ph04;
  j_PU04 = pu04;
  j_trkiso = trkiso;
  CalcPFRelIso();
}

void Muon::SetChi2(double chi2){
  j_chi2 = chi2;
}

void Muon::CalcPFRelIso(bool use_corrected_pt){
  double absiso = j_PFCH04+std::max( 0., j_PFNH04 + j_PFPH04 - 0.5*j_PU04 );
  //cout << "[Muon::CalcPFRelIso] j_PFCH04 = " << j_PFCH04 << endl;                                                                              
  //cout << "[Muon::CalcPFRelIso] j_PFNH04 = " << j_PFNH04 << endl;                                                                              
  //cout << "[Muon::CalcPFRelIso] j_PFPH04 = " << j_PFPH04 << endl;                                                                              
  //cout << "[Muon::CalcPFRelIso] j_PU04 = " << j_PU04 << endl;                                                                                  
  //cout << "[Muon::CalcPFRelIso] --> absiso = " << absiso << endl;                                                                              

  if(use_corrected_pt)this->SetRelIso(absiso/this->Pt());
  else this->SetRelIso(absiso/this->MiniAODPt());

}


double Muon::EA(){

  double eta = fabs(this->Eta());

  if     (eta<0.8000) return 0.0566;
  else if(eta<1.3000) return 0.0562;
  else if(eta<2.0000) return 0.0363;
  else if(eta<2.2000) return 0.0119;
  else if(eta<2.4000) return 0.0064;
  else return 0.0064;

}

void Muon::SetMiniAODPt(double d){
  j_MiniAODPt = d;
}
void Muon::SetMiniAODTunePPt(double d){
  j_MiniAODTunePPt = d;
}

void Muon::SetMomentumScaleUpDown(double pt_up, double pt_down){
  j_MomentumScaleUp = pt_up;
  j_MomentumScaleDown = pt_down;
}

void Muon::SetTuneP4(double pt, double pt_err, double eta, double phi, double q){
  j_TuneP4.SetPtEtaPhiM(pt,eta,phi,M());
  j_TuneP4.SetCharge(q);
  j_TunePPtError = pt_err;
}

void Muon::SetMVA(double MVA){
  j_MVA = MVA;                                                                                                                                                                                       
}

void Muon::SetSoftMVA(double MVA){
  j_softMVA = MVA;
}

bool Muon::PassID(TString ID) const {

  if(ID=="NoCut") return true;
  //==== POG
  if(ID=="POGTight") return isPOGTight();
  if(ID=="POGHighPt") return isPOGHighPt();
  if(ID=="POGMedium") return isPOGMedium();
  if(ID=="POGLoose") return isPOGLoose();


  if(ID=="POGTightWithTightIso") return Pass_POGTightWithTightIso();
  if(ID=="POGHighPtWithLooseTrkIso") return Pass_POGHighPtWithLooseTrkIso();
  //==== Customized
  
  if(ID=="POGHighPtTight") return  Pass_POGHighPtTight();
  if(ID=="POGHighPtLoose") return  Pass_POGHighPtLoose();

  if(ID=="POGHighPtMixTight") return  Pass_POGHighPtTightOR();
  if(ID=="POGHighPtMixLoose") return  Pass_POGHighPtLooseOR();


  if(ID=="HNVeto2016") return Pass_HNVeto2016();
  if(ID=="HNLoosest") return Pass_HNVeto2016();

  if(ID=="HNVeto_17028") return Pass_HNVeto2016();
  if(ID=="HNLoose_17028") return Pass_HNLoose2016(0.4, 0.2, 0.1, 3.);
  if(ID=="HNTight_17028") return Pass_HNTight2016();

  /// looser IP
  if(ID=="HNLooseMVA") return Pass_HNLooseMVA();
  if(ID=="HNLooseV1") return Pass_HNLoose(0.4,  0.2, 0.5,10.);
  if(ID=="HNLoosePOG") return Pass_HNLoose(0.4,  0.2, 0.5,99999.);


  if(ID.Contains("MuOpt")){
    
    TString ID_sub = ID;
    ID_sub = ID_sub.ReplaceAll("_"," ");
    string sID_sub = string(ID_sub);

    vector<TString> subStrings;
    istringstream ID_subs(sID_sub);
    do {
      string subs;
      ID_subs >> subs;
      subStrings.push_back(TString(subs));
    } while (ID_subs);


    TString dxy_method = "";
    TString iso_methodB="";
    TString iso_methodEC="";
    TString pog_methodB="";
    TString pog_methodEC="";

    for(unsigned int i=0; i < subStrings.size(); i++){
      if (subStrings[i].Contains("DXY")) dxy_method=subStrings[i];
      if (subStrings[i].Contains("MVAB")) pog_methodB=subStrings[i];
      if (subStrings[i].Contains("MVAEC")) pog_methodEC=subStrings[i];

      if (subStrings[i].Contains("POG")) pog_methodB=subStrings[i];
      if (subStrings[i].Contains("POG")) pog_methodEC=subStrings[i];

      if (subStrings[i].Contains("ISOB")) iso_methodB=subStrings[i];
      if (subStrings[i].Contains("ISOEC")) iso_methodEC=subStrings[i];
    }

    if(ID.Contains("MuOptLoose")) return PassLooseIDOpt();

    return   PassIDOptMulti(dxy_method, pog_methodB,pog_methodEC, iso_methodB,iso_methodEC);

  }




  if(ID.Contains("HNMVA_")){
    TString mva_st = ID.ReplaceAll("HNMVA_","");
    std::string mva_s = std::string(mva_st);
    std::string::size_type sz;     // alias of size_t
    
    double mva_d = std::stod (mva_s,&sz);
    return PassMVA(mva_d,mva_d,mva_d);

  }

  if(ID=="HNTightMVAVL")  return PassMVA(0.,0.,0.);
  if(ID=="HNTightMVAL")  return PassMVA(0.15,0.15,0.15);
  if(ID=="HNTightMVAM")  return PassMVA(0.45,0.45,0.45);
  if(ID=="HNTightMVAT")  return PassMVA(0.65,0.65,0.65);
  if(ID=="HNTightMVAVT")  return PassMVA(0.8,0.8,0.8);
  if(ID=="HNTightMVAVVT")  return PassMVA(0.9,0.9,0.9);

  if(ID=="HNTightV1") return Pass_HNTight(0.07, 0.02, 0.05, 3.);
  if(ID=="HNTightV2") return Pass_HNTight(0.07, 0.05, 0.1, 3.);


  if(ID=="HNOpt")                 {

    if( fabs(this->Eta())<= 1.479 ){

      double dxy_cut = 0.01 ;
      if(this->Pt() > 10 && this->Pt()  < 60.) dxy_cut -= (this->Pt() - 10.) * 0.005/ 50.;
      if(this->Pt() > 60.) dxy_cut = 0.005;
      if(fabs(dXY()) >  dxy_cut)   return false;
      if(fabs(dZ()) >  0.05)   return false;
      if(RelIso() > 0.2) return false;
      //if(fabs(IP3D()/IP3Derr())> 5.)  return false;

      //double mva_cut_B = -0.4 ;
      //if(this->Pt() > 10 && this->Pt()  < 60.) mva_cut_B -= (this->Pt() -10.) * 1.2/ 50.;
      //if(this->Pt() > 60.) mva_cut_B = 0.8;
      //if(! (MVA()> mva_cut_B) ) return false;

    }
    else{

      double dxy_cut = 0.01 ;
      if(this->Pt() > 10 && this->Pt()< 60.) dxy_cut -= (this->Pt() - 10.) * 0.005/ 50.;
      if(this->Pt() > 60.) dxy_cut = 0.005;

      if(fabs(dXY()) >  dxy_cut)   return false;
      if(fabs(dZ()) >  0.05)   return false;
      if(RelIso() > 0.2) return false;

      // if(fabs(IP3D()/IP3Derr())> 7.5)  return false;
      //double mva_cut_EC = -0.5 ;
      //if(this->Pt() > 10 && this->Pt()  < 60.) mva_cut_EC -= ( this->Pt()-10.) * 1.1/ 50.;
      //if(this->Pt() > 60.) mva_cut_EC = 0.6;

      //if(! (MVA()> mva_cut_EC) ) return false;
      

    }
    return true;
  }



  if(ID=="HNTight_Iso05_dxy_01_ip_3") return Pass_HNTight(0.05,0.01,0.05,3.);
  if(ID=="HNTight_Iso05_dxy_01_ip_4") return Pass_HNTight(0.05,0.01,0.05,4.);
  if(ID=="HNTight_Iso05_dxy_01_ip_999") return Pass_HNTight(0.05,0.01,0.05,999.);
  if(ID=="HNTight_Iso05_dxy_02_ip_3") return Pass_HNTight(0.05,0.02,0.05,3.);
  if(ID=="HNTight_Iso05_dxy_02_ip_4") return Pass_HNTight(0.05,0.02,0.05,4.);
  if(ID=="HNTight_Iso05_dxy_02_ip_999") return Pass_HNTight(0.05,0.02,0.05,999.);
  if(ID=="HNTight_Iso05_dxy_05_ip_3") return Pass_HNTight(0.05,0.05, 0.05,3.);
  if(ID=="HNTight_Iso05_dxy_05_ip_4") return Pass_HNTight(0.05,0.05, 0.05,4.);
  if(ID=="HNTight_Iso05_dxy_05_ip_999") return Pass_HNTight(0.05,0.05, 0.05,999.);


  if(ID=="HNTight_Iso06_dxy_01_ip_3") return Pass_HNTight(0.06,0.01,0.05,3.);
  if(ID=="HNTight_Iso06_dxy_01_ip_4") return Pass_HNTight(0.06,0.01,0.05,4.);
  if(ID=="HNTight_Iso06_dxy_01_ip_999") return Pass_HNTight(0.06,0.01,0.05,999.);
  if(ID=="HNTight_Iso06_dxy_02_ip_3") return Pass_HNTight(0.06,0.02,0.05,3.);
  if(ID=="HNTight_Iso06_dxy_02_ip_4") return Pass_HNTight(0.06,0.02,0.05,4.);
  if(ID=="HNTight_Iso06_dxy_02_ip_999") return Pass_HNTight(0.06,0.02,0.05,999.);
  if(ID=="HNTight_Iso06_dxy_05_ip_3") return Pass_HNTight(0.06,0.05, 0.05,3.);
  if(ID=="HNTight_Iso06_dxy_05_ip_4") return Pass_HNTight(0.06,0.05, 0.05,4.);
  if(ID=="HNTight_Iso06_dxy_05_ip_999") return Pass_HNTight(0.06,0.05, 0.05,999.);


  if(ID=="HNTight_Iso07_dxy_01_ip_3") return Pass_HNTight(0.07,0.01,0.05,3.);
  if(ID=="HNTight_Iso07_dxy_01_ip_4") return Pass_HNTight(0.07,0.01,0.05,4.);
  if(ID=="HNTight_Iso07_dxy_01_ip_999") return Pass_HNTight(0.07,0.01,0.05,999.);
  if(ID=="HNTight_Iso07_dxy_02_ip_3") return Pass_HNTight(0.07,0.02,0.05,3.);
  if(ID=="HNTight_Iso07_dxy_02_ip_4") return Pass_HNTight(0.07,0.02,0.05,4.);
  if(ID=="HNTight_Iso07_dxy_02_ip_999") return Pass_HNTight(0.07,0.02,0.05,999.);
  if(ID=="HNTight_Iso07_dxy_05_ip_3") return Pass_HNTight(0.07,0.05, 0.05,3.);
  if(ID=="HNTight_Iso07_dxy_05_ip_4") return Pass_HNTight(0.07,0.05, 0.05,4.);
  if(ID=="HNTight_Iso07_dxy_05_ip_999") return Pass_HNTight(0.07,0.05, 0.05,999.);


  if(ID=="HNTight_Iso08_dxy_01_ip_3") return Pass_HNTight(0.08,0.01,0.05,3.);
  if(ID=="HNTight_Iso08_dxy_01_ip_4") return Pass_HNTight(0.08,0.01,0.05,4.);
  if(ID=="HNTight_Iso08_dxy_01_ip_999") return Pass_HNTight(0.08,0.01,0.05,999.);
  if(ID=="HNTight_Iso08_dxy_02_ip_3") return Pass_HNTight(0.08,0.02,0.05,3.);
  if(ID=="HNTight_Iso08_dxy_02_ip_4") return Pass_HNTight(0.08,0.02,0.05,4.);
  if(ID=="HNTight_Iso08_dxy_02_ip_999") return Pass_HNTight(0.08,0.02,0.05,999.);
  if(ID=="HNTight_Iso08_dxy_05_ip_3") return Pass_HNTight(0.08,0.05, 0.05,3.);
  if(ID=="HNTight_Iso08_dxy_05_ip_4") return Pass_HNTight(0.08,0.05, 0.05,4.);
  if(ID=="HNTight_Iso08_dxy_05_ip_999") return Pass_HNTight(0.08,0.05, 0.05,999.);


  if(ID=="HNTight_Iso09_dxy_01_ip_3") return Pass_HNTight(0.09,0.01,0.05,3.);
  if(ID=="HNTight_Iso09_dxy_01_ip_4") return Pass_HNTight(0.09,0.01,0.05,4.);
  if(ID=="HNTight_Iso09_dxy_01_ip_999") return Pass_HNTight(0.09,0.01,0.05,999.);
  if(ID=="HNTight_Iso09_dxy_02_ip_3") return Pass_HNTight(0.09,0.02,0.05,3.);
  if(ID=="HNTight_Iso09_dxy_02_ip_4") return Pass_HNTight(0.09,0.02,0.05,4.);
  if(ID=="HNTight_Iso09_dxy_02_ip_999") return Pass_HNTight(0.09,0.02,0.05,999.);
  if(ID=="HNTight_Iso09_dxy_05_ip_3") return Pass_HNTight(0.09,0.05, 0.05,3.);
  if(ID=="HNTight_Iso09_dxy_05_ip_4") return Pass_HNTight(0.09,0.05, 0.05,4.);
  if(ID=="HNTight_Iso09_dxy_05_ip_999") return Pass_HNTight(0.09,0.05, 0.05,999.);


  if(ID=="HNTight_Iso10_dxy_01_ip_3") return Pass_HNTight(0.10,0.01,0.05,3.);
  if(ID=="HNTight_Iso10_dxy_01_ip_4") return Pass_HNTight(0.10,0.01,0.05,4.);
  if(ID=="HNTight_Iso10_dxy_01_ip_999") return Pass_HNTight(0.10,0.01,0.05,999.);
  if(ID=="HNTight_Iso10_dxy_02_ip_3") return Pass_HNTight(0.10,0.02,0.05,3.);
  if(ID=="HNTight_Iso10_dxy_02_ip_4") return Pass_HNTight(0.10,0.02,0.05,4.);
  if(ID=="HNTight_Iso10_dxy_02_ip_999") return Pass_HNTight(0.10,0.02,0.05,999.);
  if(ID=="HNTight_Iso10_dxy_05_ip_3") return Pass_HNTight(0.10,0.05, 0.05,3.);
  if(ID=="HNTight_Iso10_dxy_05_ip_4") return Pass_HNTight(0.10,0.05, 0.05,4.);
  if(ID=="HNTight_Iso10_dxy_05_ip_999") return Pass_HNTight(0.10,0.05, 0.05,999.);


  if(ID=="HNTightPFIsoLoose") return Pass_POGTightPFIsoLoose(true);
  if(ID=="HNTightPFIsoMedium") return Pass_POGTightPFIsoMedium(true);
  if(ID=="HNTightPFIsoTight") return Pass_POGTightPFIsoTight(true);
  if(ID=="HNTightPFIsoVeryTight") return Pass_POGTightPFIsoVeryTight(true);
  if(ID=="HNTightPFIsoVeryVeryTight") return Pass_POGTightPFIsoVeryVeryTight(true);

  if(ID=="POGTightPFIsoLoose") return Pass_POGTightPFIsoLoose(false);
  if(ID=="POGTightPFIsoMedium") return Pass_POGTightPFIsoMedium(false);
  if(ID=="POGTightPFIsoTight") return Pass_POGTightPFIsoTight(false);
  if(ID=="POGTightPFIsoVeryTight") return Pass_POGTightPFIsoVeryTight(false);
  if(ID=="POGTightPFIsoVeryVeryTight") return Pass_POGTightPFIsoVeryVeryTight(false);
  
  

  if(ID=="CutBasedTightNoIP") return Pass_CutBasedTightNoIP();

  //==== No cut
  if(ID=="NOCUT") return true;

  cout << "[Muon::PassID] No id : " << ID << endl;
  exit(ENODATA);

  return false;

}


bool Muon::passIDHN(int ID, double dxy_b, double dxy_e, double dz_b,double dz_e, double sip_b, double sip_e, double iso_b,double iso_e, double miso_b, double miso_e) const{

  if( fabs(this->Eta())<= 1.479 ){
    if(! (fabs(dXY())< dxy_b && fabs(dZ())<dz_b && fabs(IP3D()/IP3Derr())< sip_b) ) return false;
  }
  else{
    if(! (fabs(dXY())< dxy_e && fabs(dZ())<dz_e && fabs(IP3D()/IP3Derr())< sip_e) ) return false;
  }
  if( ID==1){
   

    if(! (isPOGTight()) ) return false;
    if( miso_e > 0.){
      if(fabs(this->Eta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
    }
    else if( iso_e > 0.){
      if(fabs(this->Eta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
    }
    else if(fabs(this->Eta())> 1.479  &&  ! (isPOGTight()))  return false;
    
    if( miso_b > 0.){
      if(fabs(this->Eta()) <= 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
    }
    else if( iso_b > 0.){
      if(fabs(this->Eta()) <= 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
    }
    else if(fabs(this->Eta()) <= 1.479  &&! (isPOGTight()))  return false;
    
  }
  else if( ID==2){

    if(! (isPOGMedium()) ) return false;
    if( miso_e > 0.){
      if(fabs(this->Eta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
    }
    else if( iso_e > 0.){
      if(fabs(this->Eta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
    }
    else if(fabs(this->Eta())> 1.479  &&  ! (isPOGMedium()))  return false;
    if( miso_b > 0.){
      if(fabs(this->Eta()) <= 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
    }
    else if( iso_b > 0.){
      if(fabs(this->Eta()) <= 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
    }
    else if(fabs(this->Eta()) <= 1.479  &&! (isPOGMedium()))  return false;
    
  }

  else if( ID==3){
   
    if(fabs(this->Eta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
    if(fabs(this->Eta())< 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;

  }
  else if( ID==4){
    if(fabs(this->Eta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
    if(fabs(this->Eta())< 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;

  }

  return true;
}


bool Muon::PassMVA(double mva1, double mva2) const {

  //https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/PatAlgos/plugins/PATMuonProducer.cc 
  // apply loose selection 

  //      if (muon.pt() > 5 and muon.isLooseMuon() and muon.passed(reco::Muon::MiniIsoLoose) and sip3D < 8.0 and
  //dB2D < 0.05 and dz < 0.1) {

  if (this->Pt() < 5) return false;
  if(!( isPOGLoose() )) return false;
  if(RelIso() > 0.1)  return false;
  if(!( fabs(dXY())< 0.05 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr())< 5.) ) return false;
  
  if( fabs(this->Eta()) <= 0.8 ){
    if(! (MVA()>mva1) ) return false;
  }
  else if( fabs(this->Eta()) > 0.8 && fabs(this->Eta()) <= 1.479 ){
    if(! (MVA()>mva1) ) return false;
  }
  else{
    if(! (MVA()>mva2) ) return false;
  }

  return true;
}

bool Muon::PassMVA(double mva1, double mva2, double mva3) const {

  //https://github.com/cms-sw/cmssw/blob/master/PhysicsTools/PatAlgos/plugins/PATMuonProducer.cc                                                                                                   
  // apply loose selection                                                                                                                                                                         

  //      if (muon.pt() > 5 and muon.isLooseMuon() and muon.passed(reco::Muon::MiniIsoLoose) and sip3D < 8.0 and                                                                                   
  //dB2D < 0.05 and dz < 0.1) {                                                                                                                                                                    

  //if (this->Pt() < 5) return false;
  //if(!( isPOGLoose() )) return false;
  //if(RelIso() > 0.1)  return false;
  //if(!( fabs(dXY())< 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr())< 5.) ) return false;

  if( fabs(this->Eta()) <= 0.8 ){
    if(! (MVA()>mva1) ) return false;
  }
  else if( fabs(this->Eta()) > 0.8 && fabs(this->Eta()) <= 1.479 ){
    if(! (MVA()>mva2) ) return false;
  }
  else{
    if(! (MVA()>mva3) ) return false;
  }

  return true;
}





bool Muon::PassSoftMVA(double mva1, double mva2, double mva3) const {

  if( fabs(this->Eta()) <= 0.8 ){
    if(! (softMVA()>mva1) ) return false;
  }
  else if( fabs(this->Eta()) > 0.8 && fabs(this->Eta()) <= 1.479 ){
    if(! (softMVA()>mva2) ) return false;
  }
  else{
    if(! (softMVA()>mva3) ) return false;
  }

  return true;
}

int  Muon::PassLooseIDOpt( ) const{

  if(!( isPOGLoose() )) return 0;
  if(fabs(dXY()) >  0.2)   return 0;
  if(fabs(dZ()) >  0.5)   return 0;
  if(! (RelIso()< 0.4) ) return 0;

  return 1;

}

int  Muon::PassIDOptMulti(TString dxy_method, TString sel_methodB,TString sel_methodEC,  TString iso_methodB,TString iso_methodEC ) const{

  bool DEBUG=false;

  if( fabs(this->Eta())<= 1.479 ){

    double dxy_cut = 0.01 ;
    if(dxy_method.Contains("B1")) {
      if(this->Pt() > 10 && this->Pt()  < 60.) dxy_cut -= (this->Pt() - 10.) * 0.005/ 50.;
      if(this->Pt() > 60.) dxy_cut = 0.005;
      if(fabs(dXY()) >  dxy_cut)   {
        if(DEBUG) cout << " DXY  FAIL" << endl;
        return 0;
      }
    }
    
    if(fabs(dZ()) >  0.05)   return 0;

    if(fabs(IP3D()/IP3Derr())> 5.)  return 0;

    if(DEBUG) cout << " sel_methodB = " << sel_methodB << endl;

     if(sel_methodB.Contains("MVA")){

      TString mva_st = sel_methodB.ReplaceAll("MVAB","");
      std::string mva_s = std::string(mva_st);
      std::string::size_type sz;     // alias of size_t                                                                        


      double mva_d = std::stod (mva_s,&sz);
      double mva_cut_B =  mva_d ;//-0.5 ;                                                                                                                                                                                                      

      if(this->Pt() > 10 && this->Pt()  < 60.) mva_cut_B += (this->Pt() - 10.) * (0.9 - mva_d)/ 50.;
      if(this->Pt()  > 60.)  mva_cut_B = 0.9;

      if(DEBUG) cout << "pt = " <<  this->Pt()  << " mva = " << MVA() << " cut =" << mva_cut_B << endl;

      if(! (MVA()> mva_cut_B) ) return 0;
    }
    else{
      if(sel_methodB == "POGT"){
        if(! (isPOGTight()) ) return 0;
      }
      if(sel_methodB == "POGM"){
        if(! (isPOGTight()) ) return 0;
      }
    }
    if(iso_methodB != ""){

      TString iso_st = iso_methodB.ReplaceAll("ISOB","");
      std::string iso_s = std::string(iso_st);
      std::string::size_type sz;     // alias of size_t                                                                                                                                                                                                                                                                                                          

      double iso_d = std::stod (iso_s,&sz);
      double iso_cut_B =  iso_d ;

      if(DEBUG) cout << "RelIso " << iso_cut_B << endl;
      if(! (RelIso()<iso_cut_B) ) return false;
    }
  }
  else{
    if(DEBUG) cout << "PassIDOpt ENDCAP " << endl;


    if(dxy_method.Contains("EC1")) {

      double dxy_cut = 0.01 ;
      if(this->Pt() > 10 && this->Pt()< 60.) dxy_cut -= (this->Pt()- 10.) * 0.005/ 50.;
      if(this->Pt()  > 60.) dxy_cut = 0.005;

      if(DEBUG) cout << "pt = " << this->Pt() << " DXY cut = " << dxy_cut << " value = " << fabs(dXY()) <<  endl;
      if(fabs(dXY()) >  dxy_cut)   {
        if(DEBUG) cout << " DXY  FAIL" << endl;
        return 0;
      }
    }
    if(fabs(dZ()) >  0.07)   return 0;
    if(fabs(IP3D()/IP3Derr())> 7.5)  return 0;

    if(iso_methodEC != ""){

      TString iso_st = iso_methodEC.ReplaceAll("ISOEC","");
      std::string iso_s = std::string(iso_st);
      std::string::size_type sz;     // alias of size_t                                                                                                                                                                                                                                                                                                          
      double iso_d = std::stod (iso_s,&sz);
      double iso_cut_EC =  iso_d ;

      if(DEBUG) cout << "RelIso " << iso_cut_EC << endl;

      if(! (RelIso()<iso_cut_EC) ) return false;
    }

    if(sel_methodEC.Contains("MVA")){

      TString mva_st = sel_methodEC.ReplaceAll("MVAEC","");
      std::string mva_s = std::string(mva_st);
      std::string::size_type sz;     // alias of size_t                                                                                                      

      double mva_d = std::stod (mva_s,&sz);
      double mva_cut_EC =  mva_d ;//-0.5 ;                                                                                                                  
      
      if(this->Pt() > 10 && this->Pt()  < 60.) mva_cut_EC += (this->Pt() - 10.) * (0.9 - mva_d)/ 50.;
      if(this->Pt()  > 60.)  mva_cut_EC = 0.9;

      if(DEBUG) cout << "pt = " <<  this->Pt() << " mva = " <<MVA() << " cut =" << mva_cut_EC << endl;

      if(! (MVA()> mva_cut_EC) ) return 0;
    }
    else{
      if(sel_methodEC == "POGT"){
        if(! (isPOGTight()) ) return false;
      }
      if(sel_methodEC == "POGM"){
        if(! (isPOGMedium()) ) return false;
      }

    }
  }
  return 1;

}




bool Muon::Pass_POGTightWithTightIso() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.15 ))  return false;
  return true;
}

bool Muon::Pass_POGHighPtLooseOR() const {
  if(Pass_HNLoose(0.4, 0.2, 0.5, 10.)) return true;
  if(Pass_POGHighPtLoose()) return true;
  return false;
}

bool Muon::Pass_POGHighPtTightOR() const {
  if(Pass_POGTightPFIsoVeryTight(true)) return true;
  if(Pass_POGHighPtTight())  return true;
  return false;
}



bool Muon::Pass_POGHighPtLoose() const {
  //if(this->Pt() < 60) return false;
  if(!( isPOGLoose() )) return false;
  if(!( IsType(GlobalMuon) )) return false;
  if(!( TrkIso()/TuneP4().Pt()<0.3 )) return false;
  return true;
}

bool Muon::Pass_POGHighPtTight() const {
  //if(this->Pt() < 60) return false;
  if(!( isPOGHighPt() )) return false;
  if(!( TrkIso()/TuneP4().Pt()<0.1 )) return false;
  return true;
}


bool Muon::Pass_POGHighPtWithLooseTrkIso() const {
  if(!( isPOGHighPt() )) return false;
  if(!( TrkIso()/TuneP4().Pt()<0.1 )) return false;
  return true;
}

bool Muon::Pass_HNVeto2016() const {
  //  if(Pass_POGHighPtTight()) return true;
  if(!( isPOGLoose() )) return false;
  if(!( fabs(dXY())<0.2 && fabs(dZ())<0.5) ) return false;
  if(!( RelIso()<0.6 ))  return false;
  if(!( Chi2()<50. )) return false;
  return true;
}

bool Muon::Pass_HNLoose2016(double relisoCut, double dxyCut, double dzCut, double sipCut) const {

  if(!( isPOGLoose() )) return false;
  if(!( fabs(dXY())<dxyCut && fabs(dZ())<dzCut && fabs(IP3D()/IP3Derr())<sipCut) ) return false;
  if(!( RelIso()<relisoCut ))  return false;
  if(!( Chi2()<50. )) return false;
  return true;
}

bool Muon::Pass_HNTight2016() const {
  if(!( isPOGTight() )) return false;
  if(!( fabs(dXY())<0.005 && fabs(dZ())<0.04 && fabs(IP3D()/IP3Derr())<3.) ) return false;
  if(!( RelIso()<0.07 ))  return false;
  if(!( Chi2()<10. )) return false;
  return true;
}

bool Muon::Pass_HNLooseMVA() const{

  if(!( isPOGLoose() )) return false;
  if(!( fabs(dXY())< 0.2 && fabs(dZ())<0.1) ) return false;
  if(!( RelIso()< 0.4 )) return false;
  //if(!(fabs(IP3D()/IP3Derr())< 3 )) return false;
  return true;


}

bool Muon::Pass_HNLoose(double relisoCut, double dxyCut, double dzCut,double sipCut) const {
  // Individual cuts of the POG cut-based ID
  if(!( isPOGLoose() )) return false;
  //if(!( IsType(GlobalMuon) )) return false;
  if(!( fabs(dXY())<dxyCut && fabs(dZ())<dzCut) ) return false;
  if(!(fabs(IP3D()/IP3Derr())<sipCut )) return false;
  if(!( RelIso()<relisoCut )) return false;
  if(!( Chi2()<50. )) return false;
  
  return true;
}

bool Muon::Pass_HNTight(double relisoCut, double dxyCut, double dzCut,double sipCut) const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<relisoCut )) return false;
  if(!( fabs(dXY())<dxyCut && fabs(dZ())<dzCut &&fabs(IP3D()/IP3Derr())<sipCut )) return false;
  return true;
}

bool Muon::Pass_ISRLoose(double relisoCut) const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<relisoCut )) return false;
  return true;
}

bool Muon::Pass_ISRTight() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.15 )) return false;
  return true;
}

//==== TEST ID

bool Muon::Pass_TESTID() const {
  return true;
}

bool Muon::Pass_POGTightRelIso25() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.25 )) return false;
  return true;
}

bool Muon::Pass_POGTightRelIso20() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.2 )) return false;
  return true;
}

bool Muon::Pass_POGTightRelIso15() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.15 )) return false;
  return true;
}

bool Muon::Pass_POGTightRelIso10() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.1 )) return false;
  return true;
}

bool Muon::Pass_POGTightPFIsoLoose(bool applyIP) const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoLoose) )) return false;
  if(applyIP) {
    if(!( fabs(dXY())< 0.02 && fabs(dZ())<0.05)) return false;

  }
  return true;
}

bool Muon::Pass_POGTightPFIsoMedium(bool applyIP) const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoMedium) )) return false;
  if(applyIP) {
    if(!( fabs(dXY())< 0.02 && fabs(dZ())<0.05 )) return false;

  }
  return true;
}

bool Muon::Pass_POGTightPFIsoTight(bool applyIP) const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoTight) )) return false;
  if(applyIP) {
    if(!( fabs(dXY())< 0.02 && fabs(dZ())<0.05)) return false;

  }
  return true;
}

bool Muon::Pass_POGTightPFIsoVeryTight(bool applyIP) const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoVeryTight) )) return false;
  if(applyIP) {
    if(!( fabs(dXY())< 0.02 && fabs(dZ())<0.05)) return false;
  }
  return true;
}

bool Muon::Pass_POGTightPFIsoVeryVeryTight(bool applyIP) const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.05 )) return false;
  if(applyIP) {
    if(!( fabs(dXY())< 0.02 && fabs(dZ())<0.05)) return false;
  }
  return true;
}

bool Muon::Pass_POGTightCutsWithTightIso() const {  // This gives the same result with the result using POGTightWithTightIso
  if(!( isPOGLoose() )) return false;
  if(!( IsType(GlobalMuon) )) return false;
  if(!( Chi2()<10. )) return false;
  if(!( ValidMuonHits()>0 )) return false;
  if(!( MatchedStations()>1 )) return false;
  if(!( fabs(dXY())<0.2 && fabs(dZ())<0.5) ) return false;
  if(!( PixelHits()>0 )) return false;
  if(!( TrackerLayers()>5 )) return false;
  if(!( RelIso()<0.15 )) return false;
  return true;
}

//==== N-1 POG Tight
bool Muon::Pass_CutBasedTightNoIP() const {

  bool debug=false;
  if(!( isPOGLoose() )) {if(debug)cout << "Fail isPOGLoose" << endl; return false;}
  if(!( IsType(GlobalMuon) )) {if(debug)cout << "Fail GlobalMuon" << endl; return false;}
  if(!( Chi2()<10. )) {if(debug)cout << "Fail Chi2" << endl; return false;}
  if(!( ValidMuonHits()>0 )) {if(debug)cout << isPOGTight() << "  Fail ValidMuonHits" << endl; return false;}
  if(!( MatchedStations()>1 )) {if(debug)cout << "Fail MatchedStations" << endl; return false;}
  if(!( PixelHits()>0 )) {if(debug)cout << "Fail PixelHits" << endl; return false;}
  if(!( TrackerLayers()>5 )) {if(debug)cout << "Fail TrackerLayers" << endl; return false;}
  return true;
}


void Muon::SetValidMuonHits(int n){
  j_validmuonhits = n;
}

void Muon::SetMatchedStations(int n){
  j_matchedstations = n;
}

void Muon::SetPixelHits(int n){
  j_pixelHits = n;
}

void Muon::SetTrackerLayers(int n){
  j_trackerLayers = n;
}

bool Muon::PassFilter(TString filter) const{
  if( filter=="hltDiMu9Ele9CaloIdLTrackIdLMuonlegL3Filtered9" ) return j_filterbits&(ULong64_t(1)<<0);
  else if( filter=="hltDiMuon178Mass3p8Filtered" ) return j_filterbits&(ULong64_t(1)<<1);
  else if( filter=="hltDiMuon178Mass8Filtered" ) return j_filterbits&(ULong64_t(1)<<2);
  else if( filter=="hltDiMuon178RelTrkIsoFiltered0p4" ) return j_filterbits&(ULong64_t(1)<<3);
  else if( filter=="hltDiMuon178RelTrkIsoFiltered0p4DzFiltered0p2" ) return j_filterbits&(ULong64_t(1)<<4);
  else if( filter=="hltDiMuonGlb17Glb8DzFiltered0p2" ) return j_filterbits&(ULong64_t(1)<<5);
  else if( filter=="hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4" ) return j_filterbits&(ULong64_t(1)<<6);
  else if( filter=="hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4DzFiltered0p2" ) return j_filterbits&(ULong64_t(1)<<7);
  else if( filter=="hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4" ) return j_filterbits&(ULong64_t(1)<<8);
  else if( filter=="hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4DzFiltered0p2" ) return j_filterbits&(ULong64_t(1)<<9);
  else if( filter=="hltDiMuonGlbFiltered17TrkFiltered8" ) return j_filterbits&(ULong64_t(1)<<10);
  else if( filter=="hltDiMuonTrk17Trk8RelTrkIsoFiltered0p4" ) return j_filterbits&(ULong64_t(1)<<11);
  else if( filter=="hltDiMuonTrk17Trk8RelTrkIsoFiltered0p4DzFiltered0p2" ) return j_filterbits&(ULong64_t(1)<<12);
  else if( filter=="hltDiTkMuonTkFiltered17TkFiltered8" ) return j_filterbits&(ULong64_t(1)<<13);
  else if( filter=="hltL1TripleMu553L2TriMuFiltered3L3TriMuFiltered10105" ) return j_filterbits&(ULong64_t(1)<<14);
  else if( filter=="hltL1TripleMu553L2TriMuFiltered3L3TriMuFiltered5" ) return j_filterbits&(ULong64_t(1)<<15);
  else if( filter=="hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09" ) return j_filterbits&(ULong64_t(1)<<16);
  else if( filter=="hltL3crIsoL1sMu22Or25L1f0L2f10QL3f27QL3trkIsoFiltered0p07" ) return j_filterbits&(ULong64_t(1)<<17);
  else if( filter=="hltL3crIsoL1sSingleMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p07" ) return j_filterbits&(ULong64_t(1)<<18);
  else if( filter=="hltL3fL1DoubleMu155fFiltered17" ) return j_filterbits&(ULong64_t(1)<<19);
  else if( filter=="hltL3fL1DoubleMu155fPreFiltered8" ) return j_filterbits&(ULong64_t(1)<<20);
  else if( filter=="hltL3fL1DoubleMu157fFiltered18" ) return j_filterbits&(ULong64_t(1)<<21);
  else if( filter=="hltL3fL1DoubleMu157fFiltered9" ) return j_filterbits&(ULong64_t(1)<<22);
  else if( filter=="hltL3fL1DoubleMu7EG7f0Filtered9" ) return j_filterbits&(ULong64_t(1)<<23);
  else if( filter=="hltL3fL1Mu6DoubleEG10f0Filtered8" ) return j_filterbits&(ULong64_t(1)<<24);
  else if( filter=="hltL3fL1TripleMu553f0Filtered10105" ) return j_filterbits&(ULong64_t(1)<<25);
  else if( filter=="hltL3fL1TripleMu553f0Filtered1055" ) return j_filterbits&(ULong64_t(1)<<26);
  else if( filter=="hltL3fL1TripleMu553f0PreFiltered555" ) return j_filterbits&(ULong64_t(1)<<27);
  else if( filter=="hltL3fL1sDoubleMu114L1f0L2f10L3Filtered17" ) return j_filterbits&(ULong64_t(1)<<28);
  else if( filter=="hltL3fL1sDoubleMu114L1f0L2f10OneMuL3Filtered17" ) return j_filterbits&(ULong64_t(1)<<29);
  else if( filter=="hltL3fL1sDoubleMu114TkFiltered17Q" ) return j_filterbits&(ULong64_t(1)<<30);
  else if( filter=="hltL3fL1sMu10lqL1f0L2f10L3Filtered17" ) return j_filterbits&(ULong64_t(1)<<31);
  else if( filter=="hltL3fL1sMu15DQlqL1f0L2f10L3Filtered17" ) return j_filterbits&(ULong64_t(1)<<32);
  else if( filter=="hltL3fL1sMu1lqL1f0L2f10L3Filtered17TkIsoFiltered0p4" ) return j_filterbits&(ULong64_t(1)<<33);
  else if( filter=="hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09" ) return j_filterbits&(ULong64_t(1)<<34);
  else if( filter=="hltL3fL1sMu5L1f0L2f5L3Filtered8" ) return j_filterbits&(ULong64_t(1)<<35);
  else if( filter=="hltL3fL1sMu5L1f0L2f5L3Filtered8TkIsoFiltered0p4" ) return j_filterbits&(ULong64_t(1)<<36);
  else if( filter=="hltL3pfL1sDoubleMu114L1f0L2pf0L3PreFiltered8" ) return j_filterbits&(ULong64_t(1)<<37);
  else if( filter=="hltL3pfL1sDoubleMu114ORDoubleMu125L1f0L2pf0L3PreFiltered8" ) return j_filterbits&(ULong64_t(1)<<38);
  else if( filter=="hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLDZFilter" ) return j_filterbits&(ULong64_t(1)<<39);
  else if( filter=="hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23" ) return j_filterbits&(ULong64_t(1)<<40);
  else if( filter=="hltMu23TrkIsoVVLEle8CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23" ) return j_filterbits&(ULong64_t(1)<<41);
  else if( filter=="hltMu8DiEle12CaloIdLTrackIdLMuonlegL3Filtered8" ) return j_filterbits&(ULong64_t(1)<<42);
  else if( filter=="hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLDZFilter" ) return j_filterbits&(ULong64_t(1)<<43);
  else if( filter=="hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8" ) return j_filterbits&(ULong64_t(1)<<44);
  else if( filter=="hltMu9Ele9DZFilter" ) return j_filterbits&(ULong64_t(1)<<45);
  else if( filter=="hltTripleTrkMuFiltered5NoVtx" ) return j_filterbits&(ULong64_t(1)<<46);
  else{
    cout<<"[Muon::PassFilter] unknown filter "<<filter<<endl;
    exit(ENODATA);
  }
  return false;
}

bool Muon::PassPath(TString path) const{
  if( path=="HLT_DiMu9_Ele9_CaloIdL_TrackIdL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<0);
  else if( path=="HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v" ) return j_pathbits&(ULong64_t(1)<<1);
  else if( path=="HLT_IsoMu24_v" ) return j_pathbits&(ULong64_t(1)<<2);
  else if( path=="HLT_IsoMu27_v" ) return j_pathbits&(ULong64_t(1)<<3);
  else if( path=="HLT_IsoTkMu24_v" ) return j_pathbits&(ULong64_t(1)<<4);
  else if( path=="HLT_Mu17_Mu8_SameSign_DZ_v" ) return j_pathbits&(ULong64_t(1)<<5);
  else if( path=="HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v" ) return j_pathbits&(ULong64_t(1)<<6);
  else if( path=="HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v" ) return j_pathbits&(ULong64_t(1)<<7);
  else if( path=="HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<8);
  else if( path=="HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v" ) return j_pathbits&(ULong64_t(1)<<9);
  else if( path=="HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<10);
  else if( path=="HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v" ) return j_pathbits&(ULong64_t(1)<<11);
  else if( path=="HLT_Mu17_TrkIsoVVL_v" ) return j_pathbits&(ULong64_t(1)<<12);
  else if( path=="HLT_Mu17_v" ) return j_pathbits&(ULong64_t(1)<<13);
  else if( path=="HLT_Mu18_Mu9_SameSign_v" ) return j_pathbits&(ULong64_t(1)<<14);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<15);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v" ) return j_pathbits&(ULong64_t(1)<<16);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<17);
  else if( path=="HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v" ) return j_pathbits&(ULong64_t(1)<<18);
  else if( path=="HLT_Mu50_v" ) return j_pathbits&(ULong64_t(1)<<19);
  else if( path=="HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v" ) return j_pathbits&(ULong64_t(1)<<20);
  else if( path=="HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<21);
  else if( path=="HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v" ) return j_pathbits&(ULong64_t(1)<<22);
  else if( path=="HLT_Mu8_TrkIsoVVL_v" ) return j_pathbits&(ULong64_t(1)<<23);
  else if( path=="HLT_Mu8_v" ) return j_pathbits&(ULong64_t(1)<<24);
  else if( path=="HLT_OldMu100_v" ) return j_pathbits&(ULong64_t(1)<<25);
  else if( path=="HLT_TkMu100_v" ) return j_pathbits&(ULong64_t(1)<<26);
  else if( path=="HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v" ) return j_pathbits&(ULong64_t(1)<<27);
  else if( path=="HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v" ) return j_pathbits&(ULong64_t(1)<<28);
  else if( path=="HLT_TkMu50_v" ) return j_pathbits&(ULong64_t(1)<<29);
  else if( path=="HLT_TripleMu_10_5_5_DZ_v" ) return j_pathbits&(ULong64_t(1)<<30);
  else if( path=="HLT_TripleMu_12_10_5_v" ) return j_pathbits&(ULong64_t(1)<<31);
  else if( path=="HLT_TrkMu12_DoubleTrkMu5NoFiltersNoVtx_v" ) return j_pathbits&(ULong64_t(1)<<32);
  else if( path=="HLT_TrkMu15_DoubleTrkMu5NoFiltersNoVtx_v" ) return j_pathbits&(ULong64_t(1)<<33);
  else{
    cout<<"[Muon::PassPath] unknown path "<<path<<endl;
    exit(ENODATA);
  }
  return false;
}
