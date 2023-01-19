#include "Tau.h"

ClassImp(Tau)

Tau::Tau(){

  j_IDBit = 0;
  j_decaymode=-1;
  j_idDecayModeNewDMs=false;

  this->SetLeptonFlavour(TAU);
}

void Tau::PrintObject(TString label){

}

Tau::~Tau(){

}

void Tau::SetIDBit(unsigned int idbit){
  j_IDBit = idbit;
}


void Tau::SetDecayMode(int decaymode){

  j_decaymode= decaymode;
}


void Tau::SetDecayModeNewDM(bool DecayModeNewDMs){

  j_idDecayModeNewDMs= DecayModeNewDMs;
}


   

bool Tau::PassID(TString ID) const{

  // === list of IDs for analyis
  if(ID=="NoCut") return true;
  if(ID=="HNVeto") {

    if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11) {
      
      if (DecayModeNewDM() && passTIDvJet() && passTIDvEl()  && passTIDvMu() ) return true;
    }

  }
  if(j_decaymode == 0 || j_decaymode == 1 || j_decaymode == 10 || j_decaymode ==11) {
    if (DecayModeNewDM()){
      bool passTauID=true;
      if(ID.Contains("JetVVL") && !passVVLIDvJet())passTauID=false;
      if(ID.Contains("JetVL") && !passVLIDvJet())passTauID=false;
      if(ID.Contains("JetL") && !passLIDvJet())passTauID=false;
      if(ID.Contains("JetM") && !passMIDvJet())passTauID=false;
      if(ID.Contains("JetT") && !passTIDvJet())passTauID=false;
      if(ID.Contains("JetVT") && !passVTIDvJet())passTauID=false;
      if(ID.Contains("JetVVT") && !passVVTIDvJet())passTauID=false;
      
      // El
      if(ID.Contains("ElVVL") && !passVVLIDvEl())passTauID=false;
      if(ID.Contains("ElVL") && !passVLIDvEl())passTauID=false;
      if(ID.Contains("ElL") && !passLIDvEl())passTauID=false;
      if(ID.Contains("ElM") && !passMIDvEl())passTauID=false;
      if(ID.Contains("ElT") && !passTIDvEl())passTauID=false;
      if(ID.Contains("ElVT") && !passVTIDvEl())passTauID=false;
      if(ID.Contains("ElVVT") && !passVVTIDvEl())passTauID=false;

      if(ID.Contains("MuVL") && !passVLIDvMu())passTauID=false;
      if(ID.Contains("MuL") && !passLIDvMu())passTauID=false;
      if(ID.Contains("MuM") && !passMIDvMu())passTauID=false;
      if(ID.Contains("MuT") && !passTIDvMu())passTauID=false;
      return passTauID;
    }
  }

  return false;
}
