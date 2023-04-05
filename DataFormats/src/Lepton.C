#include "Lepton.h"

ClassImp(Lepton)

Lepton::Lepton() : Particle() {
  j_dXY = -999.;
  j_dXYerr = -999.;
  j_dZ = -999.;
  j_dZerr = -999.;
  j_IP3D = -999.;
  j_IP3Derr = -999.;
  j_unCorrPt = -999;
  j_unCorrPx = -999;
  j_unCorrPy = -999;
  j_RelIso = -999.;
  j_MiniRelIso = -999.;
  j_ptcone = -999.;
  j_lep_jetptrel=-999.;
  j_lep_jetptratio=-999.;
  j_lep_jetbscore=-999.;
  j_lep_jetflavour=-999;
  j_LeptonFlavour = NONE;
  j_MiniIso_ChHad = -999.;
  j_MiniIso_NHad = -999.;
  j_MiniIso_PhHad = -999.;
  j_Iso_ChHad = -999.;
  j_Iso_NHad = -999.;
  j_Iso_PhHad = -999.;
  j_jetntracks = -1;
  j_jetntracks_mva = -1;
  j_lep_mva=-999;
  j_LeptonType = -999;
  j_passID = false;
  j_IDSet= false;

  j_lep_mva_hnl_fake_v1=-999;
  j_lep_mva_hnl_fake_v2=-999;
  j_lep_mva_hnl_fake_v2_hf=-999;
  j_lep_mva_hnl_fake_v2_lf=-999;
  j_lep_mva_hnl_fake_v2_top=-999;
  j_lep_mva_hnl_fake_v3=-999;
  j_lep_mva_hnl_fake_v3_hf=-999;
  j_lep_mva_hnl_fake_v3_hfb=-999;
  j_lep_mva_hnl_fake_v3_hfc=-999;
  j_lep_mva_hnl_fake_v3_lf=-999;
  j_lep_mva_hnl_fake_v3_top=-999;
  j_lep_mva_hnl_fake_v4=-999;
  j_lep_mva_hnl_fake_v4_hf=-999;
  j_lep_mva_hnl_fake_v4_hfb=-999;
  j_lep_mva_hnl_fake_v4_hfc=-999;
  j_lep_mva_hnl_fake_v4_lf=-999;
  j_lep_mva_hnl_fake_v4_top=-999;

  
  j_lep_mva_hnl_conv_v1=-999;
  j_lep_mva_hnl_conv_v2=-999;

  j_lep_mva_hnl_cf_v1=-999;
  j_lep_mva_hnl_cf_v2=-999;
  j_lep_mva_hnl_cf_v2p1=-999;
  j_lep_mva_hnl_cf_v2p2=-999;

  j_lep_mva_hnl_ed_cf_v2=-999;
  j_lep_mva_hnl_ed_cf_v2p1=-999;
  j_lep_mva_hnl_ed_cf_v2p2=-999;


}

void Lepton::PrintObject(TString label){
  
  cout << "Pt = " << this->Pt() << endl;
  cout << "j_dXY =" << j_dXY << endl;
  cout << "j_dXYerr =" << j_dXYerr << endl;
  cout << "j_dZ =" << j_dZ  << endl;
  cout << "j_dZerr = " << j_dZerr  << endl;
  cout << "j_IP3D =" << j_IP3D  << endl;
  cout << "j_IP3Derr =" << j_IP3Derr  << endl;
  cout << "j_RelIso =" << j_RelIso  << endl;
  cout << "j_MiniRelIso =" << j_MiniRelIso << endl;
  cout << "j_ptcone = " << j_ptcone  << endl;
  cout << "j_lep_jetptrel=" << j_lep_jetptrel << endl;
  cout << "j_lep_jetptratio=" << j_lep_jetptratio << endl;
  cout << "j_MiniIso_ChHad =" << j_MiniIso_ChHad   << endl;
  cout << "j_MiniIso_NHad =" << j_MiniIso_NHad  << endl;
  cout << "j_MiniIso_PhHad =" << j_MiniIso_PhHad  << endl;
  cout << "j_Iso_ChHad =" << j_Iso_ChHad  << endl;
  cout << "j_Iso_NHad =" << j_Iso_NHad  << endl;
  cout << "j_Iso_PhHad =" << j_Iso_PhHad  << endl;
  cout << "j_jetntracks =" << j_jetntracks  << endl;
  cout << "j_jetntracks_mva =" << j_jetntracks_mva  << endl;
  cout << "j_lep_mva=" << j_lep_mva  << endl;
  cout << "j_LeptonType =" << j_LeptonType  << endl;

  
  return;
}

/*
Lepton::Lepton(const Lepton& lep) : Particle() {
  j_dXY =  lep.dXY();
  j_dXYerr = lep.dXYerr();
  j_dZ = lep.dZ();
  j_dZerr = lep.dZerr();
  j_IP3D = lep.IP3D();
  j_IP3Derr = lep.IP3Derr();
  j_RelIso = lep.RelIso();
  j_MiniRelIso = lep.MiniRelIso();
  j_ptcone = lep.PtCone();
  j_lep_jetptrel=lep.lep_jet_ptrel();
  j_lep_jetptrelDef=lep.lep_jet_ptrelDef();
  j_lep_jetptratio=lep.lep_jet_ptratio();
  j_lep_jetptratioDef=lep.lep_jet_ptratioDef();
  j_LeptonFlavour = lep.LeptonFlavour();
  j_MiniIso_ChHad = lep.MiniIsoChHad();
  j_MiniIso_NHad = lep.MiniIsoNHad();
  j_MiniIso_PhHad = lep.MiniIsoPhHad();
  j_Iso_ChHad = lep.IsoChHad();
  j_Iso_NHad = lep.IsoNHad();
  j_Iso_PhHad = lep.IsoPhHad();
  j_jetntracks =lep.JetNTracks();
  j_jetntracks_mva = lep.JetNTracksMVA();

  j_LeptonType = lep.LeptonType();
  j_passID = lep.PassLepID();
  j_IDSet= lep.LepIDSet();

  j_lep_mva= lep.lep_mva();
}
*/

Lepton::~Lepton(){

}



bool Lepton::PassULMVA(double mva, double cut, TString s_mva) const {

  if(mva == -999){

    cout<<"[Electron::PassMVA] mva value not set "<< s_mva<<endl;
    exit(ENODATA);
  }

  if ( mva < cut)  return false;
  return true;
}


void Lepton::SetHNL_FakeLepMVAV1(double mvafake){
  j_lep_mva_hnl_fake_v1=mvafake;
}

void Lepton::SetHNL_FakeLepMVAV2(double mvafake,double mvafake_hf, double mvafake_lf, double mvafake_top){

  j_lep_mva_hnl_fake_v2=mvafake;
  j_lep_mva_hnl_fake_v2_hf=mvafake_hf;
  j_lep_mva_hnl_fake_v2_lf=mvafake_lf;
  j_lep_mva_hnl_fake_v2_top=mvafake_top;

}

void Lepton::SetHNL_FakeLepMVAV3(double mvafake,double mvafake_hf, double mvafake_hfb,double mvafake_hfc, double mvafake_lf, double mvafake_top){

  j_lep_mva_hnl_fake_v3=mvafake;
  j_lep_mva_hnl_fake_v3_hf=mvafake_hf;
  j_lep_mva_hnl_fake_v3_hfb=mvafake_hfb;
  j_lep_mva_hnl_fake_v3_hfc=mvafake_hfc;
  j_lep_mva_hnl_fake_v3_lf=mvafake_lf;
  j_lep_mva_hnl_fake_v3_top=mvafake_top;

}



void Lepton::SetHNL_FakeLepMVAV4(double mvafake,double mvafake_hf, double mvafake_hfb,double mvafake_hfc, double mvafake_lf, double mvafake_top){

  j_lep_mva_hnl_fake_v4=mvafake;
  j_lep_mva_hnl_fake_v4_hf=mvafake_hf;
  j_lep_mva_hnl_fake_v4_hfb=mvafake_hfb;
  j_lep_mva_hnl_fake_v4_hfc=mvafake_hfc;
  j_lep_mva_hnl_fake_v4_lf=mvafake_lf;
  j_lep_mva_hnl_fake_v4_top=mvafake_top;

}


void Lepton::SetHNL_FakeLepMVAMuonV1(double mvafake){
  j_lep_mva_hnl_fake_v1=mvafake;
}

void Lepton::SetHNL_FakeLepMVAMuonV2(double mvafake){
  j_lep_mva_hnl_fake_v2=mvafake;
}

void Lepton::SetHNL_FakeLepMVAMuonV3(double mvafake){
  j_lep_mva_hnl_fake_v3=mvafake;
}



void Lepton::SetHNL_ConvLepMVAV1( double mvaconv){
  j_lep_mva_hnl_conv_v1=mvaconv;
}

void Lepton::SetHNL_ConvLepMVAV2( double mvaconv){
  j_lep_mva_hnl_conv_v2=mvaconv;
}

void Lepton::SetHNL_CFLepMVAV1( double mvacf){
  j_lep_mva_hnl_cf_v1=mvacf;
}


void Lepton::SetHNL_CFLepMVAV2(double mvacfv2, double mvacfv2p1,double mvacfv2p2) {
  j_lep_mva_hnl_cf_v2=mvacfv2;
  j_lep_mva_hnl_cf_v2p1=mvacfv2p1;
  j_lep_mva_hnl_cf_v2p2=mvacfv2p2;

}

void Lepton::SetHNL_CFLepMVA_EtaDependantV2(double mvacfv2, double mvacfv2p1,double mvacfv2p2) {
  j_lep_mva_hnl_ed_cf_v2=mvacfv2;
  j_lep_mva_hnl_ed_cf_v2p1=mvacfv2p1;
  j_lep_mva_hnl_ed_cf_v2p2=mvacfv2p2;
}


void Lepton::SetJetNTracks(double d){
  j_jetntracks =d;
}

void Lepton::SetJetNTracksMVA(double d){
  j_jetntracks_mva =d;
}

void Lepton::SetJetPtRel(double ptrel){

  j_lep_jetptrel = ptrel;
}

void Lepton::SetJetPtRatio(double ptr){

  j_lep_jetptratio = ptr;
}


void Lepton::SetCloseJetBScore(double bscore){

  j_lep_jetbscore = bscore;
}

void Lepton::SetCloseJetFlavour(int flav){

  j_lep_jetflavour = flav;
}


void Lepton::SetLepMVA(double MVA){
  j_lep_mva = MVA;
}

void Lepton::SetUncorrectedPt(double uncorrPt){
  j_unCorrPt = uncorrPt;
}

void Lepton::SetUncorrectedPx(double uncorrPx){
  j_unCorrPx = uncorrPx;
}

void Lepton::SetUncorrectedPy(double uncorrPy){
  j_unCorrPy = uncorrPy;
}

void Lepton::SetdXY(double dXY, double dXYerr){
  j_dXY = dXY;
  j_dXYerr = dXYerr;
}

void Lepton::SetdZ(double dZ, double dZerr){
  j_dZ = dZ;
  j_dZerr = dZerr;
}

void Lepton::SetIP3D(double IP3D, double IP3Derr){
  j_IP3D = IP3D;
  j_IP3Derr = IP3Derr;
}

void Lepton::SetRelIso(double r){
  j_RelIso = r;
}

void Lepton::SetMiniIso(double ch, double nh, double ph, double pu, double rho, double EA){

  double r_mini = miniIsoDr();

  double correction = rho * EA * (r_mini/0.3) * (r_mini/0.3);
  double correctedIso = ch + std::max(0.0, nh + ph - correction);
  
  j_MiniRelIso = correctedIso/this->Pt();

  j_MiniIso_ChHad =ch;
  j_MiniIso_NHad =nh;
  j_MiniIso_PhHad =ph;


}

void Lepton::SetLepIso(double ch, double nh, double ph){

  j_Iso_ChHad =ch;
  j_Iso_NHad =nh;
  j_Iso_PhHad =ph;

}


void Lepton::SetLeptonFlavour(Flavour f){
  j_LeptonFlavour = f;
}

void Lepton::SetLeptonType(int t){
  j_LeptonType= t;
}



void Lepton::SetID(){
  j_IDSet = true;
}

void Lepton::SetPassID(bool p){

  j_IDSet= true;
  j_passID = p;
}



void Lepton::Print(){
  if(IsElectron()) cout << "Electron\t";
  else if(IsMuon()) cout << "Muon\t";
  else cout << "NoFlav\t";
  cout << "(Pt, Eta, Phi, M, Charge) = " << Pt() << "\t" << Eta() << "\t" << Phi() << "\t" << M() << "\t" << Charge() << endl;
}
