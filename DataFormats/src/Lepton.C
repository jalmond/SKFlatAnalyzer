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
  j_lep_mva_hnl_fake=-999;
  j_lep_mva_hnl_conv=-999;
  j_lep_mva_hnl_cf=-999;
  j_lep_mvav2_hnl_fake=-999;
  j_lep_mvav2_hnl_conv=-999;
  j_lep_mvav2_hnl_cf=-999;
  j_lep_mvav3_hnl_cf=-999;
  j_lep_mvav4_hnl_cf=-999;
  j_lep_mvav2_hnl_ed_cf=-999;
  j_lep_mvav3_hnl_ed_cf=-999;
  j_lep_mvav4_hnl_ed_cf=-999;
  
  j_lep_map_mva_hnl_cf.clear();

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

double  Lepton::GetHNL_LepMVA(TString key) const{

  map<TString, double>::const_iterator it = j_lep_map_mva_hnl_cf.find(key);
  if(it == j_lep_map_mva_hnl_cf.end()) {
    cout<<"[Lepton::GetHNL_LepMVA]  unknown key "<< key<<endl;
    exit(ENODATA);

  }
  //PrintMVAMap();
  return it->second;
}

void Lepton::SetHNL_LepMVAMap(TString key,double mvacf){
  
  //cout << j_lep_mva_hnl_cf << " Setting MVA for " << key << " " << mvacf << endl;
  
  map<TString, double>::iterator it = j_lep_map_mva_hnl_cf.find(key);
  if(it == j_lep_map_mva_hnl_cf.end()) j_lep_map_mva_hnl_cf[key] = mvacf;

}


void Lepton::SetHNL_LepMVA(double mvafake, double mvaconv,double mvacf){

  j_lep_mva_hnl_fake=mvafake;
  j_lep_mva_hnl_conv=mvaconv;
  j_lep_mva_hnl_cf=mvacf;
}

void Lepton::SetHNL_LepMVAVersion2(double mvafake, double mvaconv,double mvacf){

  j_lep_mvav2_hnl_fake=mvafake;
  j_lep_mvav2_hnl_conv=mvaconv;
  j_lep_mvav2_hnl_cf=mvacf;
}

void Lepton::SetHNL_LepMVAVersion3(double mvacf)  {
  j_lep_mvav3_hnl_cf=mvacf;
}
void Lepton::SetHNL_LepMVAVersion4(double mvacf){
  j_lep_mvav4_hnl_cf=mvacf;
}


void Lepton::SetHNL_LepMVA_EtaDependantVersion2(double mvacf) {
  j_lep_mvav2_hnl_ed_cf=mvacf;
}
void Lepton::SetHNL_LepMVA_EtaDependantVersion3(double mvacf) {
  j_lep_mvav3_hnl_ed_cf=mvacf;
}
void Lepton::SetHNL_LepMVA_EtaDependantVersion4(double mvacf) {
  j_lep_mvav4_hnl_ed_cf=mvacf;
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
