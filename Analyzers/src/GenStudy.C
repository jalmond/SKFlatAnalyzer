#include "GenStudy.h"

void GenStudy::executeEvent(){

  vector<Gen> gens = GetGens();

  //PrintGen(gens);

  std::vector<Muon> noisocut_muons = GetMuons("POGMedium", 10., 2.4);
  std::vector<Muon> muons;

  bool HasMuon = false;
  //cout << "---------- RECO -----------" << endl;
  for(unsigned int i=0; i<noisocut_muons.size(); i++){
    if(noisocut_muons.at(i).RelIso()<0.4){
      muons.push_back( noisocut_muons.at(i) );

      int leptontype = GetLeptonType(noisocut_muons.at(i), gens);
      //cout << "--> leptontype = " << leptontype << endl;
      FillHist("Muon_Type", leptontype, 1, 20, -10, 10);
      HasMuon = true;

    }
  }
  //if(HasMuon) PrintGen(gens);


  std::vector<Electron> noisocut_electrons = GetElectrons("passLooseID", 10., 2.5);
  std::vector<Electron>  electrons;
  for(unsigned int i=0; i<noisocut_electrons.size(); i++){
    if(noisocut_electrons.at(i).RelIso()<0.4){
      electrons.push_back( noisocut_electrons.at(i) );

      int leptontype = GetLeptonType(noisocut_electrons.at(i), gens);

      FillHist("Electron_Type", leptontype, 1, 20, -10, 10);


    }
  }






}

