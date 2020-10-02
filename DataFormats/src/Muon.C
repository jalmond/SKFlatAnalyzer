#include "Muon.h"

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

void Muon::CalcPFRelIso(){
  double absiso = j_PFCH04+std::max( 0., j_PFNH04 + j_PFPH04 - 0.5*j_PU04 );
  //cout << "[Muon::CalcPFRelIso] j_PFCH04 = " << j_PFCH04 << endl;
  //cout << "[Muon::CalcPFRelIso] j_PFNH04 = " << j_PFNH04 << endl;
  //cout << "[Muon::CalcPFRelIso] j_PFPH04 = " << j_PFPH04 << endl;
  //cout << "[Muon::CalcPFRelIso] j_PU04 = " << j_PU04 << endl;
  //cout << "[Muon::CalcPFRelIso] --> absiso = " << absiso << endl;
  //this->SetRelIso(absiso/this->Pt());
  this->SetRelIso(absiso/this->MiniAODPt()); //TODO This is same as IDBit -> SAME!!
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
  //j_lowptMVA = lowptMVA;
  //j_softMVA = softMVA;
}

bool Muon::PassID(TString ID) const {
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

  if(ID=="POGHighPtMixTight") return  Pass_POGHighPtTightMixed();
  if(ID=="POGHighPtMixLoose") return  Pass_POGHighPtLooseMixed();


  if(ID=="HNVeto2016") return Pass_HNVeto2016();
  if(ID=="HNLoosest") return Pass_HNVeto2016();

  if(ID=="HNLoose2016") return Pass_HNLoose2016(0.4, 0.2, 0.1, 3.);
  if(ID=="HNTight2016") return Pass_HNTight2016();

  /// looser IP
  if(ID=="HNLooseV1") return Pass_HNLoose(0.4,  0.2, 0.5);
  if(ID=="HNTightV1") return Pass_HNTight(0.07, 0.01, 0.1);
  if(ID=="HNTightV2") return Pass_HNTight(0.07, 0.05, 0.2);




  if(ID=="POGTightPFIsoLoose") return Pass_POGTightPFIsoLoose();
  if(ID=="POGTightPFIsoMedium") return Pass_POGTightPFIsoMedium();
  if(ID=="POGTightPFIsoTight") return Pass_POGTightPFIsoTight();
  if(ID=="POGTightPFIsoVeryTight") return Pass_POGTightPFIsoVeryTight();
  if(ID=="POGTightPFIsoVeryVeryTight") return Pass_POGTightPFIsoVeryVeryTight();
  if(ID=="CutBasedTightNoIP") return Pass_CutBasedTightNoIP();

  //==== No cut
  if(ID=="NOCUT") return true;

  cout << "[Muon::PassID] No id : " << ID << endl;
  exit(EXIT_FAILURE);

  return false;

}
bool Muon::Pass_POGTightWithTightIso() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.15 ))  return false;
  return true;
}

bool Muon::Pass_POGHighPtLooseMixed() const {
  if(Pass_HNLoose2016(0.4, 0.2, 0.1, 3.)) return true;
  if(this->Pt() < 50) return false;
  if(!( isPOGHighPt() )) return false;
  if(!( TrkIso()/TuneP4().Pt()<0.3 )) return false;
  return true;
}

bool Muon::Pass_POGHighPtTightMixed() const {
  if(Pass_POGTightPFIsoVeryTight()) return true;
  
  if(this->Pt() < 60) return false;
  
  if(!( isPOGHighPt() )) return false;
  if(!( TrkIso()/TuneP4().Pt()<0.1 )) return false;


  return true;
}



bool Muon::Pass_POGHighPtLoose() const {
  if(this->Pt() < 50) return false;
  if(!( isPOGHighPt() )) return false;
  if(!( TrkIso()/TuneP4().Pt()<0.3 )) return false;
  return true;
}

bool Muon::Pass_POGHighPtTight() const {
  if(this->Pt() < 60) return false;
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

bool Muon::Pass_HNLoose(double relisoCut, double dxyCut, double dzCut) const {
  // Individual cuts of the POG cut-based ID
  if(!( isPOGLoose() )) return false;
  if(!( IsType(GlobalMuon) )) return false;
  if(!( Chi2()<10. )) return false;
  if(!( ValidMuonHits()>0 )) return false;
  if(!( MatchedStations()>1 )) return false;
  if(!( fabs(dXY())<dxyCut && fabs(dZ())<dzCut) ) return false;
  if(!( PixelHits()>0 )) return false;
  if(!( TrackerLayers()>5 )) return false;
  // RelPFIso
  if(!( RelIso()<relisoCut )) return false;
  return true;
}

bool Muon::Pass_HNTight(double relisoCut, double dxyCut, double dzCut) const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<relisoCut )) return false;
  if(!( fabs(dXY())<dxyCut && fabs(dZ())<dzCut &&fabs(IP3D()/IP3Derr())<3. )) return false;
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

bool Muon::Pass_POGTightPFIsoLoose() const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoLoose) )) return false;
  return true;
}

bool Muon::Pass_POGTightPFIsoMedium() const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoMedium) )) return false;
  return true;
}

bool Muon::Pass_POGTightPFIsoTight() const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoTight) )) return false;
  return true;
}

bool Muon::Pass_POGTightPFIsoVeryTight() const {
  if(!( isPOGTight() )) return false;
  if(!( PassSelector(PFIsoVeryTight) )) return false;
  return true;
}

bool Muon::Pass_POGTightPFIsoVeryVeryTight() const {
  if(!( isPOGTight() )) return false;
  if(!( RelIso()<0.05 )) return false;
  
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
  if(!( isPOGLoose() )) return false;
  if(!( IsType(GlobalMuon) )) return false;
  if(!( Chi2()<10. )) return false;
  if(!( ValidMuonHits()>0 )) return false;
  if(!( MatchedStations()>1 )) return false;
  if(!( PixelHits()>0 )) return false;
  if(!( TrackerLayers()>5 )) return false;
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
