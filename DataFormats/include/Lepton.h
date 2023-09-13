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
    //// return TString based on Gen type
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
    if((j_LeptonType==1 || j_LeptonType==2 || j_LeptonType==3)) return true;
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

  inline bool LeptonIsCF() const {return j_LeptonIsCF;}


  enum Flavour{
    NONE, ELECTRON, MUON, TAU
  };

  enum EtaRegion{
    IB, OB, GAP, EC
  };

  inline double defEta() const{
    double lepeta = this->Eta();
    if(j_LeptonFlavour==ELECTRON) lepeta = j_elSCeta;
    return lepeta;
  }


  inline EtaRegion etaRegion() const {
    double sceta = fabs(defEta());
    if( sceta < 0.8 ) return IB;
    else if( sceta < 1.444 ) return OB;
    else if( sceta < 1.566 ) return GAP;
    else return EC;
  }
  
  inline TString  etaRegionString() const {
    double sceta = fabs(defEta());
    if( sceta < 0.8 ) return "EB1";
    else if( sceta < 1.479 ) return "EB2";
    else return "EE";
  }


  inline int Region() const {
    double eta = fabs(defEta());
    if( eta < 0.8 ) return 1;
    else if( eta < 1.479 ) return 2;
    else return 3;
  }
  
  inline TString sEtaRegion() const {
    if(j_LeptonFlavour==MUON){

      double eta = fabs(defEta());
      if( fabs(eta) < 0.9 )      return "EtaBin1";
      else if( fabs(eta) < 1.2)  return "EtaBin2";
      else if( fabs(eta) < 2.1 ) return "EtaBin3";
      else  if( fabs(eta) < 2.4 )return "EtaBin4";
      return "EtaX";
    }
    else{

      double eta = defEta();
      if( eta< -2 )         return "scEtaBin1";
      else if( eta< -1.56 ) return "scEtaBin2";
      else if( eta > -1.44 && eta< -0.8 ) return "scEtaBin3";
      else if( eta< 0 )     return "scEtaBin4";
      else if( eta< 0.8 )   return "scEtaBin5";
      else if( eta< 1.444 ) return "scEtaBin6";
      else if( eta< 2 && eta> 1.56) return "scEtaBin7";
      else return "scEtaBin8";
    }    
  }

  inline TString sPtRegion(TString Year) const {
    if(j_LeptonFlavour==MUON){
      if( this->Pt() > 10 && this->Pt() < 15 ) return "PtBin1";
      else  if( this->Pt()  < 20 ) return "PtBin2";
      else  if( this->Pt()  < 25 ) return "PtBin3";
      else  if( this->Pt()  < 30 ) return "PtBin4";
      else  if( this->Pt()  < 40 ) return "PtBin5";
      else  if( this->Pt()  < 50 ) return "PtBin6";
      else  if( this->Pt()  < 60 ) return "PtBin7";
      else   return "PtBin8";

    }
    else{
      if(Year=="2016"){
	if( this->Pt() > 10 && this->Pt() < 20 ) return "PtBin1";
	else  if( this->Pt()  < 35 ) return "PtBin2";
	else  if( this->Pt()  < 50 ) return "PtBin3";
	else  if( this->Pt()  < 100 ) return "PtBin4";
	else   return "PtBin5";
      }
      else{
	if( this->Pt() > 10 && this->Pt() < 20 ) return "PtBin1";
        else  if( this->Pt()  < 35 ) return "PtBin2";
        else  if( this->Pt()  < 50 ) return "PtBin3";
        else  if( this->Pt()  < 100 ) return "PtBin4";
        else  if( this->Pt()  < 200 ) return "PtBin5";
        else   return "Pt6";

      }
    }
  }


  inline bool IsIB() const { return (Region() == 1); }
  inline bool IsOB() const { return (Region() == 2); }
  inline bool IsEC() const { return (Region() == 3); }
  inline bool IsBB() const { return (Region() < 3); }

  //// HNL UL Funtions
  inline bool MaxPt() const { return (this->Pt() > 2000) ? 1999 : this->Pt(); }
  inline double PtParton(double Corr, double MVACutBB, double MVACutEC){
    if(j_LeptonFlavour==MUON){
      if (j_lep_mva > MVACutBB)  return this->Pt();
      return ( this->Pt() /j_lep_jetptratio ) * Corr;
    }
    else{
      if(IsBB()){
        if (j_lep_mva_hnl_fake_v4 > MVACutBB)  return this->Pt();
        return ( this->Pt() /j_lep_jetptratio ) * Corr;
      }
      else{
        if (j_lep_mva_hnl_fake_ed_v4 > MVACutEC)  return this->Pt();
        return ( this->Pt() /j_lep_jetptratio ) * Corr;
      }
    }
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

  inline double ElScEta() const {return j_elSCeta;};
  void SetElSCEta(double sceta);
  
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
  inline double LepMVA() const {return j_lep_mva;}

  //// BDT ID Functions
  void SetHNL_FakeLepMVAV4(double mvafake,double mvafake_hf,double mvafake_hfb,double mvafake_hfc, double mvafake_lf, double mvafake_top);
  void SetHNL_FakeLepMVA_EtaDependantV4(double mvafake,double mvafake_hf,double mvafake_hfb,double mvafake_hfc, double mvafake_lf, double mvafake_top);

  void SetHNL_FakeLepMVAMuonV4(double mvafake);
  void SetHNL_FakeLepMVAMuon_EtaDependantV4(double mvafake);


  void SetHNL_FakeLepMVA_EtaDependantV5(double mvafake, double mvafake_hfb,double mvafake_hfc, double mvafake_lf); 
  void SetHNL_FakeFlavourLepMVA_V5(double mvafake1,double mvafake2, double mvafake3,double mvafake4);

  void SetHNL_ConvLepMVAV4(double mvaconv);
  void SetHNL_ConvLepMVA_EtaDependantV4(double mvaconv);
  void SetHNL_ConvLepMVA_EtaDependantV5(double mvaconv);

  void SetHNL_CFLepMVAV4(double mvacfv4);
  void SetHNL_CFLepMVA_EtaDependantV4( double mvacfv4);
  void SetHNL_CFLepMVA_EtaDependantV5( double mvacf,double mvacfpt);

  
  map<TString, double> MAPBDT() const {

    map<TString, double> _map;
    if(j_LeptonFlavour==ELECTRON){
      _map["El_Fake_v4"]        = j_lep_mva_hnl_fake_v4;
      _map["El_Fake_v4_HF"]     = j_lep_mva_hnl_fake_v4_hf;
      _map["El_Fake_v4_HFB"]    = j_lep_mva_hnl_fake_v4_hfb;
      _map["El_Fake_v4_HFC"]    = j_lep_mva_hnl_fake_v4_hfc;
      _map["El_Fake_v4_LF"]     = j_lep_mva_hnl_fake_v4_lf;
      _map["El_Fake_v4_Top"]    = j_lep_mva_hnl_fake_v4_top;
      _map["El_ED_Fake_v4"]     = j_lep_mva_hnl_fake_ed_v4;
      _map["El_ED_Fake_v4_HF"]  = j_lep_mva_hnl_fake_ed_v4_hf;
      _map["El_ED_Fake_v4_HFB"] = j_lep_mva_hnl_fake_ed_v4_hfb;
      _map["El_ED_Fake_v4_HFC"] = j_lep_mva_hnl_fake_ed_v4_hfc;
      _map["El_ED_Fake_v4_LF"]  = j_lep_mva_hnl_fake_ed_v4_lf;
      _map["El_ED_Fake_v4_Top"] = j_lep_mva_hnl_fake_ed_v4_top;
      _map["El_Conv_v4"]        = j_lep_mva_hnl_conv_v4;
      _map["El_ED_Conv_v4"]     = j_lep_mva_hnl_conv_ed_v4;
      _map["El_CF_v4"]          = j_lep_mva_hnl_cf_v4;
      _map["El_ED_CF_v4"]       = j_lep_mva_hnl_ed_cf_v4;

      _map["El_ED_CF_v5"]          = j_lep_mva_hnl_ed_cf_v5;
      _map["El_ED_CF_v5Pt"]        = j_lep_mva_hnl_ed_cf_v5Pt;
      _map["El_ED_Conv_v5"]        = j_lep_mva_hnl_conv_ed_v5;
      _map["El_ED_Fake_v5"]        = j_lep_mva_hnl_fake_ed_v5;
      _map["El_Fake_v5_HFB"]    = j_lep_mva_hnl_fake_v5_hfb;
      _map["El_Fake_v5_HFC"]    = j_lep_mva_hnl_fake_v5_hfc;
      _map["El_Fake_v5_LF"]     = j_lep_mva_hnl_fake_v5_lf;
      _map["El_Fake_QCD_LFvsHF_v5"]= j_lep_mva_hnl_fake_QCD_LFvsHF_v5;
      _map["El_Fake_QCD_BvsC_v5"]  = j_lep_mva_hnl_fake_QCD_BvsC_v5;
      _map["El_Fake_QCD_LF1_v5"]    = j_lep_mva_hnl_fake_LF1_v5;
      _map["El_Fake_QCD_LF2_v5"]    = j_lep_mva_hnl_fake_LF2_v5;

    }
    else{
      _map["Mu_LF_Fake_v4"]         = j_lep_mva_hnl_fake_v4;
      _map["Mu_ED_LF_Fake_v4"]      = j_lep_mva_hnl_fake_ed_v4;
      _map["Mu_Fake_QCD_LFvsHF_v5"] = j_lep_mva_hnl_fake_QCD_LFvsHF_v5;
      _map["Mu_Fake_QCD_BvsC_v5"]   = j_lep_mva_hnl_fake_QCD_BvsC_v5;
      _map["Mu_Fake_QCD_LF1_v5"]    = j_lep_mva_hnl_fake_LF1_v5;
      _map["Mu_Fake_QCD_LF2_v5"]    = j_lep_mva_hnl_fake_LF2_v5;
      _map["Mu_HF_Fake_POG"]        = j_lep_mva;
    }
    return _map;
  }

  inline double HNL_MVA_Fake(TString vers) const {

    ///// v5 is July 23 MVA Trained                                                                                                                                                                                                       
    ///// v4 is Mar - May trained                                                                                                                                                                                                         
    ///// v4 has v4 {BB+EC} and v4ED which trains sep                                                                                                                                                                                       
    ///// v5 has only ED var but the BB lep are actually inclusive                                                                                                                                                                        

    if(j_LeptonFlavour==ELECTRON){
      if(vers=="v4")          return j_lep_mva_hnl_fake_v4;
      else if(vers=="v4HF")   return j_lep_mva_hnl_fake_v4_hf;
      else if(vers=="v4HFB")  return j_lep_mva_hnl_fake_v4_hfb;
      else if(vers=="v4HFC")  return j_lep_mva_hnl_fake_v4_hfc;
      else if(vers=="v4LF")   return j_lep_mva_hnl_fake_v4_lf;
      else if(vers=="v4Top")  return j_lep_mva_hnl_fake_v4_top;
      else if(vers=="EDv4")   return j_lep_mva_hnl_fake_ed_v4;
      else if(vers=="EDv4HF") return j_lep_mva_hnl_fake_ed_v4_hf;
      else if(vers=="EDv4HFB")return j_lep_mva_hnl_fake_ed_v4_hfb;
      else if(vers=="EDv4HFC")return j_lep_mva_hnl_fake_ed_v4_hfc;
      else if(vers=="EDv4LF") return j_lep_mva_hnl_fake_ed_v4_lf;
      else if(vers=="EDv4Top")return j_lep_mva_hnl_fake_ed_v4_top;

      else if(vers=="EDv5")   return j_lep_mva_hnl_fake_ed_v5;
      else if(vers=="v5HFB")  return j_lep_mva_hnl_fake_v5_hfb;
      else if(vers=="v5HFC")  return j_lep_mva_hnl_fake_v5_hfc;
      else if(vers=="v5LF")   return j_lep_mva_hnl_fake_v5_lf;
      else if(vers=="QCD_LFvsHF_v5") return j_lep_mva_hnl_fake_QCD_LFvsHF_v5;
      else if(vers=="QCD_BvsC_v5")   return j_lep_mva_hnl_fake_QCD_BvsC_v5;
      else if(vers=="QCD_LF1_v5")     return j_lep_mva_hnl_fake_LF1_v5;
      else if(vers=="QCD_LF2_v5")     return j_lep_mva_hnl_fake_LF2_v5;


    }
    else{
      if(vers=="v4")             return j_lep_mva_hnl_fake_v4;
      else if(vers=="EDv4")      return j_lep_mva_hnl_fake_ed_v4;
      else if(vers=="QCD_LFvsHF_v5")  return  j_lep_mva_hnl_fake_QCD_LFvsHF_v5;
      else if(vers=="QCD_BvsC_v5")    return  j_lep_mva_hnl_fake_QCD_BvsC_v5;
      else if(vers=="QCD_LF1_v5")     return j_lep_mva_hnl_fake_LF1_v5;
      else if(vers=="QCD_LF2_v5")     return j_lep_mva_hnl_fake_LF2_v5;
      else if(vers=="HFTop")          return  j_lep_mva;
    }
    cout<<"[Lepton::HNL_MVA_Fake] no version set "<< vers<< endl;
    exit(ENODATA);
    
  }

  inline int TagHF() const {
    if(j_LeptonFlavour==ELECTRON){
      if(HNL_MVA_Fake("EDv4HF")<HNL_MVA_Fake("EDv4LF")) return 1;
			else return 0;
		}
		else{
      if(LepMVA()<HNL_MVA_Fake("EDv4")) return 1;
			else return 0;
		}
	}

  inline double HNL_MVA_Conv(TString vers) const {
 
    ///// v5 is July 23 MVA Trained
    ///// v2 is Mar - May trained
    ///// v2 has v2 {BB+EC} and ED which trains sep
    ///// v5 has only ED var but the BB lep are actually inclusive
    ///// v2/5 was trainned with Top leptons
    ///// v5p1 was trainned with Top + HNL signals
    ///// v5IP has IP3D variable in training 

    if(j_LeptonFlavour==ELECTRON){
      if(vers=="v4")       return j_lep_mva_hnl_conv_v4;
      else if(vers=="EDv4")return j_lep_mva_hnl_conv_ed_v4;
      else if(vers=="EDv5")return j_lep_mva_hnl_conv_ed_v5;
    }
    
    cout<<"[Lepton::HNL_MVA_CONV]"<< endl;
    exit(ENODATA);

  }
  inline double HNL_MVA_CF(TString vers) const {


    if(j_LeptonFlavour==ELECTRON){

      if(vers=="v4")return j_lep_mva_hnl_cf_v4;
      if(vers=="EDv4")return j_lep_mva_hnl_ed_cf_v4;
      if(vers=="EDv5")return j_lep_mva_hnl_ed_cf_v5;
      else if(vers=="EDv5Pt")return j_lep_mva_hnl_ed_cf_v5Pt;

    }
    cout<<"[Lepton::HNL_MVA_CF] " << vers << endl;
    exit(ENODATA);

  }
  
  inline void SetBDT(TString mvaname, double mvavalue){
    j_lep_map_mva_hnl[mvaname] = mvavalue;
  }
  
  map<TString, double> MAPBDTTmp()  {
    return j_lep_map_mva_hnl;
  }
  

  inline double MVAValueTMP (TString idkey)  {

    map<TString, double>::iterator mit = j_lep_map_mva_hnl.find(idkey);
    if(mit != j_lep_map_mva_hnl.end()) return mit->second;
    
    cout<<"[Lepton::MVAValueTMP ERROR] " << idkey << endl;
    exit(ENODATA);
  }


  //// Close jet functionality  NOT IN SKFLAT ADDED in ANALYZERCORE OR IN BDT SKIM
  void SetJetPtRel(double ptrel);
  void SetJetPtRatio(double ptr);
  void SetCloseJetBScore(double bjscore);
  void SetCloseJetCvsBScore(double bjscore);
  void SetCloseJetCvsLScore(double bjscore);
  void SetCloseJetFlavour(int flav);

  inline double CloseJet_Ptrel()   const {return j_lep_jetptrel;}
  inline double CloseJet_Ptratio() const {return j_lep_jetptratio;}
  inline double CloseJet_BScore()  const {return j_lep_jetbscore;}
  inline double CloseJet_CvsBScore()  const {return j_lep_jetcvsbscore;}
  inline double CloseJet_CvsLScore()  const {return j_lep_jetcvslscore;}
  inline int CloseJet_FlavourInt()  const {return j_lep_jetflavour;}
  inline TString CloseJet_Flavour()  const {
    if(j_lep_jetflavour == 5) return "HF_B";
    if(j_lep_jetflavour == 4) return "HF_C";
    if(j_lep_jetflavour == 0) return "LF";
    return "Pileup";
  }

  inline TString MotherJetFlavour()  const {
    if(j_lep_jetflavour >= 4) return "HF";
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

  inline double fEta() const {return fabs(defEta());}

  bool PassULMVA(double mva1, double cut, TString s_mva) const;

  inline double GetPtSlopeCut(double MinPt, double MaxPt, double MinCut, double MaxCut) const{

    double cutSlope  = (MaxCut-MinCut) / (MaxPt-MinPt);
    double cutFinal = std::max( MinCut, std::min(MaxCut , MinCut + cutSlope*(this->Pt()-MinPt) ) );

    return cutFinal;
  }

  inline bool PassLepID()  const {return j_passID;}
  inline bool LepIDSet()  const {return j_IDSet;}

  inline int LeptonGenType() const {return j_LeptonType;}
  void SetLeptonType(int t);

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
    if(fabs(defEta()) < 0.8) return "BB";
    if(fabs(defEta()) < 1.5) return "EB";
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

  std::map<TString, double> j_lep_map_mva_hnl;

  /////// FAKE MVA
  /// Version 4
  double j_lep_mva_hnl_fake_v4_hf,   j_lep_mva_hnl_fake_v4_hfb,   j_lep_mva_hnl_fake_v4_hfc,   j_lep_mva_hnl_fake_v4_lf, j_lep_mva_hnl_fake_v4_top, j_lep_mva_hnl_fake_v4;
  double j_lep_mva_hnl_fake_ed_v4_hf,j_lep_mva_hnl_fake_ed_v4_hfb,j_lep_mva_hnl_fake_ed_v4_hfc,j_lep_mva_hnl_fake_ed_v4_lf, j_lep_mva_hnl_fake_ed_v4_top, j_lep_mva_hnl_fake_ed_v4;
  
  /// Version 5
  double j_lep_mva_hnl_fake_ed_v5,j_lep_mva_hnl_fake_v5_hfb,j_lep_mva_hnl_fake_v5_hfc,j_lep_mva_hnl_fake_v5_lf;
  
  double j_lep_mva_hnl_fake_QCD_LFvsHF_v5, j_lep_mva_hnl_fake_QCD_BvsC_v5,j_lep_mva_hnl_fake_LF1_v5,j_lep_mva_hnl_fake_LF2_v5;

  ///// Conv
  
  double j_lep_mva_hnl_conv_v4,j_lep_mva_hnl_conv_ed_v4,j_lep_mva_hnl_conv_ed_v5;

  ////// CF
 
  double j_lep_mva_hnl_cf_v4,j_lep_mva_hnl_ed_cf_v4;
  double j_lep_mva_hnl_ed_cf_v5,j_lep_mva_hnl_ed_cf_v5Pt;
  

private:
  double j_dXY, j_dXYerr;
  double j_lep_mva;
  double j_dZ, j_dZerr;
  double j_IP3D, j_IP3Derr;
  double j_unCorrPx;
  double j_unCorrPy;
  double j_unCorrPt,j_elSCeta;
  double j_jetntracks,j_jetntracks_mva;
  double j_RelIso, j_MiniRelIso;
  double j_MiniIso_ChHad,j_MiniIso_NHad,j_MiniIso_PhHad;
  double j_Iso_ChHad,j_Iso_NHad,j_Iso_PhHad;
  double j_ptcone;
  double j_lep_jetptrel,j_lep_jetptratio,j_lep_jetbscore,j_lep_jetcvsbscore,j_lep_jetcvslscore;
  int j_lep_jetflavour;

  Flavour j_LeptonFlavour;

  int j_LeptonType;
  bool j_LeptonIsCF;

  bool j_passID;
  
  bool j_IDSet;



  ClassDef(Lepton,1)

};

#endif
