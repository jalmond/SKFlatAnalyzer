#ifndef Lepton_h
#define Lepton_h

#include "Particle.h"
#include <map>

class Lepton: public Particle {
public:

  Lepton();
  ~Lepton();

  void  PrintObject(TString label);

  //// Function to Check Nature of lepton

  inline TString LepGenTypeString() const {
    if(j_LeptonIsCF) {
      if((j_LeptonType==1 || j_LeptonType==2)) return "IsPromptCF";
      else if ((j_LeptonType>=4 || j_LeptonType<-4 ))  return "IsConvCF";
      else return "IsFakeCF";
    }
    if((j_LeptonType==1 || j_LeptonType==2)) return "IsPrompt";
    if( (j_LeptonType<0 && j_LeptonType>=-4)) return "IsFake";
    if(j_LeptonType==3) return "IsEWtau";
    if((j_LeptonType>=4 || j_LeptonType<-4 )) return "IsConv";
    else return "NULL";
  }

  inline bool IsPrompt() const {
    if((j_LeptonType==1 || j_LeptonType==2)) return true;
    else return false;
  }

  inline bool IsFake() const {
    if( (j_LeptonType<0 && j_LeptonType>=-4)) return true;
    else return false;
  }
  inline bool IsEWtau() const {
    if(j_LeptonType==3) return true;
    else return false;
  }

  inline bool IsConv() const {
    if((j_LeptonType>=4 || j_LeptonType<-4 )) return true;
    else return false;
  }


  enum Flavour{
    NONE, ELECTRON, MUON, TAU
  };

  enum EtaRegion{
    IB, OB, GAP, EC
  };
  inline EtaRegion etaRegion() const {
    double sceta = fabs(this->Eta());
    if( sceta < 0.8 ) return IB;
    else if( sceta < 1.444 ) return OB;
    else if( sceta < 1.566 ) return GAP;
    else return EC;
  }
  
  inline TString  etaRegionString() const {
    double sceta = fabs(this->Eta());
    if( sceta < 0.8 ) return "IB";
    else if( sceta < 1.479 ) return "OB";
    else return "EC";
  }


  // Use Eta binning from EXO17028
  inline int Region() const {
    double eta = fabs(this->Eta());
    if( eta < 0.8 ) return 1;
    else if( eta < 1.479 ) return 2;
    else return 3;
  }

  inline bool IsIB() const { return (Region() == 1); }
  inline bool IsOB() const { return (Region() == 2); }
  inline bool IsEC() const { return (Region() == 3); }
  inline bool IsBB() const { return (Region() < 3); }

  //// HNL UL Funtions
  inline bool MaxPt() const { return (this->Pt() > 2000) ? 1999 : this->Pt(); }
  inline double PtConeCorrected(double Corr, bool passMVA){
    if (passMVA)  return this->Pt();
    return ( j_lep_jetptratio ) * Corr;
  }
  
  /// TEMP Variables to test MVA Top 
  inline double JetNTracksMVA() const { return j_jetntracks_mva;}
  inline double JetNTracks() const { return j_jetntracks;}
  void SetJetNTracks(double d);
  void SetJetNTracksMVA(double d);
    
  /// UncorrectedPt used for Lepton class fuctions
  inline double UncorrectedPt() const { return j_unCorrPt;}
  void SetUncorrectedPt(double d);

  /// UncorrectedPx, Py for MET propagation of energy systematics //JH
  inline double UncorrectedPx() const { return j_unCorrPx;}
  void SetUncorrectedPx(double d);
  inline double UncorrectedPy() const { return j_unCorrPy;}
  void SetUncorrectedPy(double d);

  
  /// Standard Functions

  void SetdXY(double dXY, double dXYerr);
  inline double fdXY() const {return fabs(j_dXY);}
  inline double dXY() const {return j_dXY;}
  inline double dXYerr() const {return j_dXYerr;}

  inline double LogdXY() const {return std::log(fabs(j_dXY));}
  inline double LogdZ() const {return std::log(fabs(j_dZ));}
  inline double LogdXYSig() const {return std::log(fabs(j_dXY/j_dXYerr));}
  inline double LogdZSig() const {return std::log(fabs(j_dZ/j_dZerr));}



  void SetLepMVA(double mva);
  inline double lep_mva() const {return j_lep_mva;}
  inline double LepMVA() const {return j_lep_mva;}

  //// BDT ID Functions
  void SetHNL_FakeLepMVAV1(double mvafake);
  void SetHNL_FakeLepMVAV2(double mvafake,double mvafake_hf, double mvafake_lf, double mvafake_top);
  void SetHNL_FakeLepMVAV3(double mvafake,double mvafake_hf,double mvafake_hfb,double mvafake_hfc, double mvafake_lf, double mvafake_top);
  void SetHNL_FakeLepMVAV4(double mvafake,double mvafake_hf,double mvafake_hfb,double mvafake_hfc, double mvafake_lf, double mvafake_top);
  void SetHNL_FakeLepMVA_EtaDependantV4(double mvafake,double mvafake_hf,double mvafake_hfb,double mvafake_hfc, double mvafake_lf, double mvafake_top);

  void SetHNL_FakeLepMVAMuonV1(double mvafake);
  void SetHNL_FakeLepMVAMuonV2(double mvafake);
  void SetHNL_FakeLepMVAMuonV3(double mvafake);
  void SetHNL_FakeLepMVAMuonV4(double mvafake);
  void SetHNL_FakeLepMVAMuon_EtaDependantV4(double mvafake);

  void SetHNL_ConvLepMVAV1(double mvaconv);
  void SetHNL_ConvLepMVAV2(double mvaconv);
  void SetHNL_ConvLepMVA_EtaDependantV2(double mvaconv);

  void SetHNL_CFLepMVAV1(double mvacf);
  void SetHNL_CFLepMVAV2(double mvacfv2, double mvacfv2p1,double mvacfv2p2);
  void SetHNL_CFLepMVA_EtaDependantV2( double mvacfv2, double mvacfv2p1,double mvacfv2p2);


  map<TString, double> MAPBDT() const {

    map<TString, double> _map;
    if(j_LeptonFlavour==ELECTRON){
      _map["El_Fake_v1"] = j_lep_mva_hnl_fake_v1;
      _map["El_Fake_v2"] = j_lep_mva_hnl_fake_v2;
      _map["El_Fake_v2_HF"] = j_lep_mva_hnl_fake_v2_hf;
      _map["El_Fake_v2_LF"] = j_lep_mva_hnl_fake_v2_lf;
      _map["El_Fake_v2_Top"] = j_lep_mva_hnl_fake_v2_top;
      _map["El_Fake_v3"] = j_lep_mva_hnl_fake_v3;
      _map["El_Fake_v3_HF"] = j_lep_mva_hnl_fake_v3_hf;
      _map["El_Fake_v3_HFB"] = j_lep_mva_hnl_fake_v3_hfb;
      _map["El_Fake_v3_HFC"] = j_lep_mva_hnl_fake_v3_hfc;
      _map["El_Fake_v3_LF"] = j_lep_mva_hnl_fake_v3_lf;
      _map["El_Fake_v3_Top"] = j_lep_mva_hnl_fake_v3_top;
      _map["El_Fake_v4"] = j_lep_mva_hnl_fake_v4;
      _map["El_Fake_v4_HF"] = j_lep_mva_hnl_fake_v4_hf;
      _map["El_Fake_v4_HFB"] = j_lep_mva_hnl_fake_v4_hfb;
      _map["El_Fake_v4_HFC"] = j_lep_mva_hnl_fake_v4_hfc;
      _map["El_Fake_v4_LF"] = j_lep_mva_hnl_fake_v4_lf;
      _map["El_Fake_v4_Top"] = j_lep_mva_hnl_fake_v4_top;
      _map["El_ED_Fake_v4"] = j_lep_mva_hnl_fake_ed_v4;
      _map["El_ED_Fake_v4_HF"] = j_lep_mva_hnl_fake_ed_v4_hf;
      _map["El_ED_Fake_v4_HFB"] = j_lep_mva_hnl_fake_ed_v4_hfb;
      _map["El_ED_Fake_v4_HFC"] = j_lep_mva_hnl_fake_ed_v4_hfc;
      _map["El_ED_Fake_v4_LF"] = j_lep_mva_hnl_fake_ed_v4_lf;
      _map["El_ED_Fake_v4_Top"] = j_lep_mva_hnl_fake_ed_v4_top;
      _map["El_Conv_v1"] = j_lep_mva_hnl_conv_v1;
      _map["El_Conv_v2"] = j_lep_mva_hnl_conv_v2;
      _map["El_ED_Conv_v2"] = j_lep_mva_hnl_conv_ed_v2;
      _map["El_CF_v1"] = j_lep_mva_hnl_cf_v1;
      _map["El_CF_v2"] = j_lep_mva_hnl_cf_v2;
      _map["El_CF_v2p1"] = j_lep_mva_hnl_cf_v2p1;
      _map["El_CF_v2p2"] = j_lep_mva_hnl_cf_v2p2;
      _map["El_ED_CF_v2"] = j_lep_mva_hnl_ed_cf_v2;
      _map["El_ED_CF_v2p1"] = j_lep_mva_hnl_ed_cf_v2p1;
      _map["El_ED_CF_v2p2"] = j_lep_mva_hnl_ed_cf_v2p2;
    }
    else{
      _map["Mu_LF_Fake_v1"] = j_lep_mva_hnl_fake_v1;
      _map["Mu_LF_Fake_v2"] = j_lep_mva_hnl_fake_v2;
      _map["Mu_LF_Fake_v3"] = j_lep_mva_hnl_fake_v3;
      _map["Mu_LF_Fake_v4"] = j_lep_mva_hnl_fake_v4;
      _map["Mu_ED_LF_Fake_v4"] = j_lep_mva_hnl_fake_ed_v4;
      
    }
    return _map;
  }

  inline double HNL_MVA_Fake(TString vers) const {

    //    if(k_debug) cout << "HNL_MVA_Fake " << vers << endl;
    if(j_LeptonFlavour==ELECTRON){
      if(vers=="v1")  return j_lep_mva_hnl_fake_v1;
      else if(vers=="v2")  return j_lep_mva_hnl_fake_v2;
      else if(vers=="v2HF")  return j_lep_mva_hnl_fake_v2_hf;
      else if(vers=="v2LF")  return j_lep_mva_hnl_fake_v2_lf;
      else if(vers=="v2Top")  return j_lep_mva_hnl_fake_v2_top;
      else if(vers=="v3")  return j_lep_mva_hnl_fake_v3;
      else if(vers=="v3HF")  return j_lep_mva_hnl_fake_v3_hf;
      else if(vers=="v3HFB")  return j_lep_mva_hnl_fake_v3_hfb;
      else if(vers=="v3HFC")  return j_lep_mva_hnl_fake_v3_hfc;
      else if(vers=="v3LF")  return j_lep_mva_hnl_fake_v3_lf;
      else if(vers=="v3Top")  return j_lep_mva_hnl_fake_v3_top;
      else if(vers=="v4")  return j_lep_mva_hnl_fake_v4;
      else if(vers=="v4HF")  return j_lep_mva_hnl_fake_v4_hf;
      else if(vers=="v4HFB")  return j_lep_mva_hnl_fake_v4_hfb;
      else if(vers=="v4HFC")  return j_lep_mva_hnl_fake_v4_hfc;
      else if(vers=="v4LF")  return j_lep_mva_hnl_fake_v4_lf;
      else if(vers=="v4Top")  return j_lep_mva_hnl_fake_v4_top;
      else if(vers=="EDv4")  return j_lep_mva_hnl_fake_ed_v4;
      else if(vers=="EDv4HF")  return j_lep_mva_hnl_fake_ed_v4_hf;
      else if(vers=="EDv4HFB")  return j_lep_mva_hnl_fake_ed_v4_hfb;
      else if(vers=="EDv4HFC")  return j_lep_mva_hnl_fake_ed_v4_hfc;
      else if(vers=="EDv4LF")  return j_lep_mva_hnl_fake_ed_v4_lf;
      else if(vers=="EDv4Top")  return j_lep_mva_hnl_fake_ed_v4_top;

    }
    else{
      if(vers=="v1")  return j_lep_mva_hnl_fake_v1;
      else if(vers=="v2")  return j_lep_mva_hnl_fake_v2;
      else if(vers=="v3")  return j_lep_mva_hnl_fake_v3;
      else if(vers=="v4")  return j_lep_mva_hnl_fake_v4;
      else if(vers=="EDv4")  return j_lep_mva_hnl_fake_ed_v4;
    }
    cout<<"[Lepton::HNL_MVA_Fake] no version set "<< vers<< endl;
    exit(ENODATA);
    
  }


  inline double HNL_MVA_Conv(TString vers) const {
 
    //    if(k_debug)cout << "HNL_MVA_Conv "<< vers<< endl;

    if(j_LeptonFlavour==ELECTRON){
      if(vers=="v1")return j_lep_mva_hnl_conv_v1;
      else if(vers=="v2")return j_lep_mva_hnl_conv_v2;
      else if(vers=="EDv2")return j_lep_mva_hnl_conv_ed_v2;
    }
    
    cout<<"[Lepton::HNL_MVA_CONV]"<< endl;
    exit(ENODATA);

  }
  inline double HNL_MVA_CF(TString vers) const {

    //    if(k_debug)cout << "HNL_MVA_CF "<< vers<< endl;

    if(j_LeptonFlavour==ELECTRON){
      if(vers=="v1")return j_lep_mva_hnl_cf_v1;
      if(this->fEta() < 1.5) {
	if(vers=="v2")return j_lep_mva_hnl_cf_v2;
	else if(vers=="v2p1")return j_lep_mva_hnl_cf_v2p1;
	else if(vers=="v2p2")return j_lep_mva_hnl_cf_v2p2;
      }
      else{
	if(vers=="v2")return j_lep_mva_hnl_ed_cf_v2;
	else if(vers=="v2p1")return j_lep_mva_hnl_ed_cf_v2p1;
	else if(vers=="v2p2")return j_lep_mva_hnl_ed_cf_v2p2;
      }
    }
    cout<<"[Lepton::HNL_MVA_CF] " << vers << endl;
    exit(ENODATA);

  }
  

  //// Close jet functionality  NOT IN SKFLAT ADDED in ANALYZERCORE OR IN BDT SKIM
  void SetJetPtRel(double ptrel);
  void SetJetPtRatio(double ptr);
  void SetCloseJetBScore(double bjscore);
  void SetCloseJetFlavour(int flav);
  inline double CloseJet_Ptrel()   const {return j_lep_jetptrel;}
  inline double CloseJet_Ptratio() const {return j_lep_jetptratio;}
  inline double CloseJet_BScore()  const {return j_lep_jetbscore;}
  inline int CloseJet_FlavourInt()  const {return j_lep_jetflavour;}
  inline TString CloseJet_Flavour()  const {
    if(j_lep_jetflavour == 5) return "HF_B";
    if(j_lep_jetflavour == 4) return "HF_C";
    if(j_lep_jetflavour == 0) return "LF";
    return "Pileup";
  }


  void SetdZ(double dZ, double dZerr);
  inline double fdZ() const {return fabs(j_dZ);}
  inline double dZ() const {return j_dZ;}
  inline double dZerr() const {return j_dZerr;}

  void SetIP3D(double IP3D, double IP3Derr);
  inline double SIP3D() const {return (j_IP3Derr >0) ? fabs(j_IP3D/j_IP3Derr):0.0;};
  inline double IP3D() const {return j_IP3D;}
  inline double IP3Derr() const {return j_IP3Derr;}


  //==== AbsIso will be set in Muon/Electron,
  //==== and use SetRelIso to save calculated RelIso
  void SetRelIso(double r);
  inline double RelIso() const {return j_RelIso;}

  inline double MiniIsoChHad() const {return j_MiniIso_ChHad;}
  inline double MiniIsoNHad() const {return j_MiniIso_NHad;}
  inline double MiniIsoPhHad() const {return j_MiniIso_PhHad;}
  inline double IsoChHad() const {return j_Iso_ChHad;}
  inline double IsoNHad() const {return j_Iso_NHad;}
  inline double IsoPhHad() const {return j_Iso_PhHad;}

  

  //==== SUSY mini Iso has same formula for Muon and Electron
  void SetMiniIso(double ch, double nh, double ph, double pu, double rho, double EA);
  inline double MiniRelIso() const {return j_MiniRelIso;}

  inline double MiniRelIsoCharged() const {return j_MiniIso_ChHad/this->Pt();}
  inline double MiniRelIsoNeutral() const {return (j_MiniRelIso*this->Pt() - j_MiniIso_ChHad)/this->Pt();}
  
  void SetLepIso(double ch, double nh, double ph);

  inline double fEta() const {return fabs(this->Eta());}

  bool PassULMVA(double mva1, double cut, TString s_mva) const;


  inline bool PassLepID()  const {return j_passID;}
  inline bool LepIDSet()  const {return j_IDSet;}

  inline int LeptonGenType() const {return j_LeptonType;}
  void SetLeptonType(int t);

  inline bool LeptonIsCF() const {return j_LeptonIsCF;}
  void SetLeptonIsCF(bool t);

  inline Flavour LeptonFlavour() const {return j_LeptonFlavour;}
  void SetLeptonFlavour(Flavour f);

  void SetPassID(bool p);
  void SetID();

  inline bool IsElectron() const {return j_LeptonFlavour==ELECTRON;}
  inline bool IsMuon() const {return j_LeptonFlavour==MUON;}

  inline TString GetFlavour() const {
    if (j_LeptonFlavour==ELECTRON) return "Electron";
    else return "Muon";
  }

  inline TString GetEtaRegion() const {
    if(fabs(this->Eta()) < 0.8) return "BB";
    if(fabs(this->Eta()) < 1.5) return "EB";
    return "EE";
  }

  inline float miniIsoDr() const {
    float mindr = 0.05;
    float maxdr = 0.2;
    float kt_scale = 10.0;
    return std::max(mindr, std::min(maxdr, float(kt_scale/this->Pt())));
  }

  void SetPtCone(double f){ j_ptcone = f; }
  inline bool IsPtConeAvailable() const {
    if(j_ptcone<0) return false;
    else return true;
  }
  inline double PtCone() const {
    if(j_ptcone<0){
      cout << "[Lepton::PtCone] ptcone not set" << endl;
      exit(EXIT_FAILURE);
    }
    return j_ptcone;
  }
  inline double CalcPtCone(double this_reliso, double Tight_reliso){
    return ( this->Pt() ) * ( 1. + max(0., (this_reliso-Tight_reliso)) );
  }

  virtual void Print();

  std::map<TString, double> j_lep_map_mva_hnl_cf;

  /////// FaKE
  /// Version 1
  double j_lep_mva_hnl_fake_v1;
  //// Version 2
  double j_lep_mva_hnl_fake_v2_hf,j_lep_mva_hnl_fake_v2_lf, j_lep_mva_hnl_fake_v2_top, j_lep_mva_hnl_fake_v2;
  //// Version 3                                                                                                                                                                                                                                              
  double j_lep_mva_hnl_fake_v3_hf,j_lep_mva_hnl_fake_v3_hfb,j_lep_mva_hnl_fake_v3_hfc,j_lep_mva_hnl_fake_v3_lf, j_lep_mva_hnl_fake_v3_top, j_lep_mva_hnl_fake_v3;

  //// Version 4                                                                                                                                                                                                                                       

  double j_lep_mva_hnl_fake_v4_hf,j_lep_mva_hnl_fake_v4_hfb,j_lep_mva_hnl_fake_v4_hfc,j_lep_mva_hnl_fake_v4_lf, j_lep_mva_hnl_fake_v4_top, j_lep_mva_hnl_fake_v4;
  double j_lep_mva_hnl_fake_ed_v4_hf,j_lep_mva_hnl_fake_ed_v4_hfb,j_lep_mva_hnl_fake_ed_v4_hfc,j_lep_mva_hnl_fake_ed_v4_lf, j_lep_mva_hnl_fake_ed_v4_top, j_lep_mva_hnl_fake_ed_v4;
  

  ///// Conv
  
  double j_lep_mva_hnl_conv_v1, j_lep_mva_hnl_conv_v2,j_lep_mva_hnl_conv_ed_v2;

  ////// CF
 
  double j_lep_mva_hnl_cf_v1,j_lep_mva_hnl_cf_v2,j_lep_mva_hnl_cf_v2p1,j_lep_mva_hnl_cf_v2p2;
  
  double j_lep_mva_hnl_ed_cf_v2,j_lep_mva_hnl_ed_cf_v2p1,j_lep_mva_hnl_ed_cf_v2p2;
  

private:
  double j_dXY, j_dXYerr;
  double j_lep_mva;
  double j_dZ, j_dZerr;
  double j_IP3D, j_IP3Derr;
  double j_unCorrPt;
  double j_unCorrPx;
  double j_unCorrPy;
  double j_jetntracks,j_jetntracks_mva;
  double j_RelIso, j_MiniRelIso;
  double j_MiniIso_ChHad,j_MiniIso_NHad,j_MiniIso_PhHad;
  double j_Iso_ChHad,j_Iso_NHad,j_Iso_PhHad;
  double j_ptcone;
  double j_lep_jetptrel,j_lep_jetptratio,j_lep_jetbscore;
  int j_lep_jetflavour;

  Flavour j_LeptonFlavour;

  int j_LeptonType;
  bool j_LeptonIsCF;

  bool j_passID;
  
  bool j_IDSet;



  ClassDef(Lepton,1)

};

#endif
