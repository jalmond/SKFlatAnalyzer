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

  inline bool IsPrompt(int LepType) const {
    if((LepType==1 || LepType==2)) return true;
    else return false;
  }

  inline bool IsFake(int LepType) const {
    if( (LepType<0 && LepType>=-4)) return true;
    else return false;
  }
  inline bool IsEWtau(int LepType) const {
    if(LepType==3) return true;
    else return false;
  }

  inline bool IsConv(int LepType) const {
    if((LepType>=4 || LepType<-4 )) return true;
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
  double  GetHNL_LepMVA(TString key) const;

  void SetdXY(double dXY, double dXYerr);
  inline double fdXY() const {return fabs(j_dXY);}
  inline double dXY() const {return j_dXY;}
  inline double dXYerr() const {return j_dXYerr;}

  inline double LogdXY() const {return std::log(fabs(j_dXY));}
  inline double LogdZ() const {return std::log(fabs(j_dZ));}
  inline double LogdXYSig() const {return std::log(fabs(j_dXY/j_dXYerr));}
  inline double LogdZSig() const {return std::log(fabs(j_dZ/j_dZerr));}

  inline void PrintMVAMap() const {

    cout << "PrintMVAMap::BEGIN " << j_lep_map_mva_hnl_cf.size() << endl;
    
    for (auto imap : j_lep_map_mva_hnl_cf ) cout << "Key " << imap.first << " val=" << imap.second << endl;
    cout << "PrintMVAMap::END" << endl;
  }

  inline vector<TString> GetMVAList() const {

    vector<TString> list;
    for (auto imap : j_lep_map_mva_hnl_cf ) list.push_back(imap.first);
    return list;

  }

  void SetLepMVA(double mva);
  inline double lep_mva() const {return j_lep_mva;}

  //// BDT  Functions
  void SetHNL_LepMVAMap(TString key,double mvacf);
  void SetHNL_LepMVA(double mvaf, double mvacf, double mvaconv);
  void SetHNL_LepMVAVersion2(double mvaf, double mvacf, double mvaconv);
  void SetHNL_LepMVAVersion3(double mvacf);
  void SetHNL_LepMVAVersion4(double mvacf);

  void SetHNL_LepMVA_EtaDependantVersion2( double mvacf);
  void SetHNL_LepMVA_EtaDependantVersion3( double mvacf);
  void SetHNL_LepMVA_EtaDependantVersion4( double mvacf);

  inline double HNL_MVA_Fake() const {return j_lep_mva_hnl_fake;}
  inline double HNL_MVA_Conv() const {return j_lep_mva_hnl_conv;}
  inline double HNL_MVA_CF()   const {return j_lep_mva_hnl_cf;}
  
  inline double HNL_MVAv2_CF()   const {return j_lep_mvav2_hnl_cf;}
  inline double HNL_MVAv3_CF()   const {return j_lep_mvav3_hnl_cf;}
  inline double HNL_MVAv4_CF()   const {return j_lep_mvav4_hnl_cf;}

  inline double HNL_MVAv2_ED_CF()   const {return j_lep_mvav2_hnl_ed_cf;}
  inline double HNL_MVAv3_ED_CF()   const {return j_lep_mvav3_hnl_ed_cf;}
  inline double HNL_MVAv4_ED_CF()   const {return j_lep_mvav4_hnl_ed_cf;}
  


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
    if(j_lep_jetflavour == 5) return "HF";
    if(j_lep_jetflavour == 4) return "HF";
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

  std::map<TString, double> j_lep_map_mva_hnl_cf;

  double j_lep_mva_hnl_fake,j_lep_mva_hnl_conv,j_lep_mva_hnl_cf;
  double j_lep_mvav2_hnl_ed_cf,j_lep_mvav3_hnl_ed_cf,j_lep_mvav4_hnl_ed_cf,j_lep_mvav2_hnl_fake,j_lep_mvav2_hnl_conv,j_lep_mvav2_hnl_cf,j_lep_mvav3_hnl_cf,j_lep_mvav4_hnl_cf;


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

  bool j_passID;
  
  bool j_IDSet;



  ClassDef(Lepton,1)

};

#endif
