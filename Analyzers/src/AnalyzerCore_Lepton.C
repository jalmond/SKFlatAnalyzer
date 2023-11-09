#include "AnalyzerCore.h"


std::vector<Muon> AnalyzerCore::GetMuons(TString id, double ptmin, double fetamax){

  std::vector<Muon> muons =  All_Muons;
  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    if(!( muons.at(i).Pt()>ptmin ))           continue;
    if(!( fabs(muons.at(i).Eta())<fetamax ))  continue;
    if(!( muons.at(i).PassID(id) ))           continue;
    out.push_back( muons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);
  return out;
}

std::vector<Electron> AnalyzerCore::GetElectrons(TString id, double ptmin, double fetamax, bool vetoHEM){

  std::vector<Electron> electrons = All_Electrons;

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    if(!( electrons.at(i).Pt()>ptmin ))             continue;
    if(!( fabs(electrons.at(i).scEta())<fetamax ))  continue;
    if(!( electrons.at(i).PassID(id) ))             continue;
    if(vetoHEM){
      if ( FindHEMElectron (electrons.at(i)) ){
        continue;
      }
    }
    out.push_back( electrons.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);
  return out;
}

std::vector<Tau> AnalyzerCore::GetTaus(TString id, double ptmin, double fetamax){

  std::vector<Tau> taus = GetAllTaus();
  std::vector<Tau> out;

  for(unsigned int i=0; i<taus.size(); i++){
    if(!( taus.at(i).Pt()>ptmin ))          continue;
    if(!( fabs(taus.at(i).Eta())<fetamax )) continue;
    if(!( taus.at(i).PassID(id) ))          continue;
    out.push_back( taus.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}



std::vector<Electron> AnalyzerCore::ScaleElectrons(const std::vector<Electron>& electrons, int sys){

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    //==== electrons is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                     
    Electron this_electron = electrons.at(i);

    double this_sf = this_electron.EnShift(sys);
    this_electron.SetPtEtaPhiM( this_electron.Pt() * this_sf, this_electron.Eta(), this_electron.Phi(), this_electron.M() );

    out.push_back( this_electron );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}
std::vector<Electron> AnalyzerCore::SmearElectrons(const std::vector<Electron>& electrons, int sys){

  std::vector<Electron> out;
  for(unsigned int i=0; i<electrons.size(); i++){
    //==== electrons is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                     
    Electron this_electron = electrons.at(i);

    double this_sf = this_electron.ResShift(sys);
    this_electron.SetPtEtaPhiM( this_electron.Pt() * this_sf, this_electron.Eta(), this_electron.Phi(), this_electron.M() );

    out.push_back( this_electron );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}



std::vector<Muon> AnalyzerCore::ScaleMuons(const std::vector<Muon>& muons, int sys){

  std::vector<Muon> out;
  for(unsigned int i=0; i<muons.size(); i++){
    //==== muons is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                         
    Muon this_muon = muons.at(i);

    //==== Even for TuneP muons, MomentumShift() are set correctly from AnalyzerCore::UseTunePMuon()                                                                                                                                                                                                                                                                                                                                     
    //==== So we can just use MomentumShift()                                                                                                                                                                                                                                                                                                                                                                                            

    this_muon.SetPtEtaPhiM( this_muon.MomentumShift(sys), this_muon.Eta(), this_muon.Phi(), this_muon.M() );

    out.push_back(this_muon);

  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

