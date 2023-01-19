#ifndef Lepton_h
#define Lepton_h

#include "Particle.h"

class Lepton: public Particle {
public:

  Lepton();
  ~Lepton();

  void  PrintObject(TString label);


  //  Lepton(const Lepton& lep) ;

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
    else if( sceta < 1.566 ) return "OB";
    else return "EE";
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

  void SetHNL_LepMVA(double mvaf, double mvacf, double mvaconv);
  inline double hnl_mva_fake() const {return j_lep_mva_hnl_fake;}
  inline double hnl_mva_conv() const {return j_lep_mva_hnl_conv;}
  inline double hnl_mva_cf() const {return j_lep_mva_hnl_cf;}



  void SetJetPtRel(double ptrel);
  inline double lep_jet_ptrel() const {return j_lep_jetptrel;}
  void SetJetPtRatio(double ptr);
  inline double lep_jet_ptratio() const {return j_lep_jetptratio;}

  void SetJetPtRelDef(double ptrel);
  inline double lep_jet_ptrelDef() const {return j_lep_jetptrelDef;}
  void SetJetPtRatioDef(double ptr);
  inline double lep_jet_ptratioDef() const {return j_lep_jetptratioDef;}


  

  void SetdZ(double dZ, double dZerr);
  inline double fdZ() const {return fabs(j_dZ);}
  inline double dZ() const {return j_dZ;}
  inline double dZerr() const {return j_dZerr;}

  void SetIP3D(double IP3D, double IP3Derr);


  inline double SIP3D() const {return j_IP3Derr>0?fabs(j_IP3D/j_IP3Derr):0.0;};
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

  inline double JetNTracksMVA() const { return j_jetntracks_mva;}
  inline double JetNTracks() const { return j_jetntracks;}

  void SetJetNTracks(double d);
  void SetJetNTracksMVA(double d);

  enum Flavour{
    NONE, ELECTRON, MUON, TAU
  };

  inline bool PassLepID()  const {return j_passID;}
  inline bool LepIDSet()  const {return j_IDSet;}


  inline int LeptonType() const {return j_LeptonType;}
  void SetLeptonType(int t);


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

  double j_lep_mva_hnl_fake,j_lep_mva_hnl_conv,j_lep_mva_hnl_cf;

private:
  double j_dXY, j_dXYerr;
  double j_lep_mva;
  double j_dZ, j_dZerr;
  double j_IP3D, j_IP3Derr;

  double j_jetntracks,j_jetntracks_mva;
  double j_RelIso, j_MiniRelIso;
  double j_MiniIso_ChHad,j_MiniIso_NHad,j_MiniIso_PhHad;
  double j_Iso_ChHad,j_Iso_NHad,j_Iso_PhHad;
  double j_ptcone;
  double j_lep_jetptrel,j_lep_jetptratio;
  double j_lep_jetptrelDef,j_lep_jetptratioDef;
  Flavour j_LeptonFlavour;

  int j_LeptonType;

  bool j_passID;
  
  bool j_IDSet;



  ClassDef(Lepton,1)

};

#endif
