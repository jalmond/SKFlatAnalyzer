#include "TriggerWithPtRange.h"

TriggerWithPtRange::TriggerWithPtRange(){
  IsDATA = true;
  DataStream = "";
}

TriggerWithPtRange::~TriggerWithPtRange(){

}

void TriggerWithPtRange::Validate(){

  if( PtValues.size() != Triggers.size()+1 ){
    cout << "[TriggerWithPtRange::Validate()] PtValues.size() != Triggers.size()+1" << endl;
    exit(EXIT_FAILURE);
  }

  double prev_pt = PtValues.at(0);
  bool PtOrderingOkay = true;
  for(unsigned int i=1; i<PtValues.size(); i++){
    if(!( prev_pt < PtValues.at(i) )){
      PtOrderingOkay = false;
      break;
    }
  }

  if(!PtOrderingOkay){
    cout << "[TriggerWithPtRange::Validate()] PtValues = {";
    for(unsigned int i=0; i<PtValues.size(); i++) cout << PtValues.at(i) << ", ";
    cout << "} --> Pt binning failed" << endl;
    exit(EXIT_FAILURE);
  }
}

TString TriggerWithPtRange::GetTriggerFromPt(double pt){
  TString out = "NULL";
  if(pt < PtValues.at(0)) return "PTFAIL"; // min pt is important
  if(pt >= PtValues.at( PtValues.size()-1 )) pt = PtValues.at( PtValues.size()-1 )-0.1; //max pt is not important..
  for(unsigned int i=0; i<Triggers.size(); i++){
    if( PtValues.at(i) < pt && pt <= PtValues.at(i+1) ){

      if(!IsDATA) return Triggers.at(i);
      else{

        if(
          Triggers.at(i)=="HLT_Mu8_v" ||
          Triggers.at(i)=="HLT_Mu17_v"
        ){
          if(DataStream=="DoubleMuon") return Triggers.at(i);
          else return "NULL";
        }

        else if(
          Triggers.at(i)=="HLT_Mu20_v" ||
          Triggers.at(i)=="HLT_Mu27_v" ||
          Triggers.at(i)=="HLT_Mu50_v"
        ){
          if(DataStream=="SingleMuon") return Triggers.at(i);
          else return "NULL";
        }

        else if(
          Triggers.at(i)=="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v" ||
          Triggers.at(i)=="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v" ||
          Triggers.at(i)=="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"
        ){
          if(DataStream=="SingleElectron") return Triggers.at(i);
          else return "NULL";
        }

        else if(
          Triggers.at(i)=="HLT_Photon25_v" ||
          Triggers.at(i)=="HLT_Photon33_v" ||
          Triggers.at(i)=="HLT_Photon50_v" ||
          Triggers.at(i)=="HLT_Photon75_v" ||
          Triggers.at(i)=="HLT_Photon90_v" ||
          Triggers.at(i)=="HLT_Photon120_v" ||
          Triggers.at(i)=="HLT_Photon150_v" ||
          Triggers.at(i)=="HLT_Photon175_v" ||
          Triggers.at(i)=="HLT_Photon200_v"
        ){
          if(DataStream=="SinglePhoton") return Triggers.at(i);
          else return "NULL";
        }

        else{

          cout << "[TriggerWithPtRange::GetTriggerFromPt] DataStream = " << DataStream << endl;
          cout << "[TriggerWithPtRange::GetTriggerFromPt] Triggers.at(i) = " << Triggers.at(i) << endl;
          exit(EXIT_FAILURE);

        }


      }

    }
  }

  cout << "[TriggerWithPtRange::GetTriggerFromPt] input pt = " << pt << endl;
  cout << "[TriggerWithPtRange::GetTriggerFromPt] not in the ptrage : " << endl;
  cout << "[TriggerWithPtRange::GetTriggerFromPt] PtValues = {";
  for(unsigned int i=0; i<PtValues.size(); i++) cout << PtValues.at(i) << ", ";
  cout << "} --> Pt binning failed" << endl;
  exit(EXIT_FAILURE);

  return out;

}



