#include "AnalyzerCore.h"


std::vector<Jet> AnalyzerCore::GetJets(TString ID, double ptmin, double fetamax){

  std::vector<Jet> jets = All_Jets;

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()>ptmin ))          continue;
    if(!( fabs(jets.at(i).Eta())<fetamax )) continue;
    if(!( jets.at(i).PassID(ID) ))          continue;
    out.push_back( jets.at(i) );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);
  return out;
}


std::vector<FatJet> AnalyzerCore::GetFatJets(TString id, double ptmin, double fetamax){

  std::vector<FatJet> jets = All_FatJets;
  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    if(!( jets.at(i).Pt()>ptmin ))           continue;
    if(!( fabs(jets.at(i).Eta())<fetamax ))  continue;
    if(!( jets.at(i).PassID(id) ))           continue;
    out.push_back( jets.at(i) );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);
  return out;

}


std::vector<Jet> AnalyzerCore::ScaleJets(const std::vector<Jet>& jets, int sys){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below                                                                            
    
    Jet this_jet = jets.at(i);
    this_jet *= this_jet.EnShift(sys);
    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);
  return out;
}

std::vector<Jet> AnalyzerCore::ScaleJetsIndividualSource(const std::vector<Jet>& jets, int sys, TString source){

  if(!std::count(JECSources.begin(),JECSources.end(), source)) {
    cout << "[AnalyzerCore::ScaleJetsIndividualSource] source " << source << " was not found" << endl;
    exit(EXIT_FAILURE);
  }

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){

    Jet this_jet = jets.at(i);

    double get_shift = GetJECUncertainty(source , "AK4PFchs",this_jet.Eta(),this_jet.Pt(), sys);

    this_jet *= get_shift;

    out.push_back( this_jet );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}





std::vector<Jet> AnalyzerCore::SmearJets(const std::vector<Jet>& jets, int sys){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                          
    Jet this_jet = jets.at(i);

    this_jet *= this_jet.ResShift(sys);

    out.push_back( this_jet );
  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

std::vector<FatJet> AnalyzerCore::ScaleFatJets(const std::vector<FatJet>& jets, int sys){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                          
    FatJet this_jet = jets.at(i);

    this_jet *= this_jet.EnShift(sys);

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}
std::vector<FatJet> AnalyzerCore::SmearFatJets(const std::vector<FatJet>& jets, int sys){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                          
    FatJet this_jet = jets.at(i);

    this_jet *= this_jet.ResShift(sys);

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}



//Fatjet SDMass systematics (https://twiki.cern.ch/twiki/bin/view/CMS/JetWtagging#2016%20scale%20factors%20and%20correctio)                                                                                                                                                                                                                                                                                                              
std::vector<FatJet> AnalyzerCore::ScaleSDMassFatJets(const std::vector<FatJet>& jets, int sys){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                          
    FatJet this_jet = jets.at(i);

    double current_SDMass (1.);
    if(DataYear == 2016) current_SDMass = this_jet.SDMass() * (1. + double(sys) * 0.0094 );
    if(DataYear == 2017) current_SDMass = this_jet.SDMass() * (0.982 + double(sys) * 0.004 );
    if(DataYear == 2018) current_SDMass = this_jet.SDMass() * (0.982 + double(sys) * 0.004 ); // FIX                                                                                                                                                                                                                                                                                                                                     

    this_jet.SetSDMass( current_SDMass );

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}
std::vector<FatJet> AnalyzerCore::SmearSDMassFatJets(const std::vector<FatJet>& jets, int sys){

  std::vector<FatJet> out;
  for(unsigned int i=0; i<jets.size(); i++){
    //==== jets is a const vector. So in this function, we have to copy the elements like below                                                                                                                                                                                                                                                                                                                                          
    FatJet this_jet = jets.at(i);
    double current_SDMass (1.);
    //https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetWtagging#2017_scale_factors_and_correctio                                                                                                                                                                                                                                                                                                                                          
    if(DataYear == 2016) current_SDMass = this_jet.SDMass() * (1. + double(sys) * 0.20 );
    if(DataYear == 2017) current_SDMass = this_jet.SDMass() * (1.09 + double(sys) * 0.05 );
    if(DataYear == 2018) current_SDMass = this_jet.SDMass() * (1.09 + double(sys) * 0.05 ); //FIX                                                                                                                                                                                                                                                                                                                                        

    this_jet.SetSDMass( current_SDMass );

    out.push_back( this_jet );
  }
  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

double AnalyzerCore::GetJECUncertainty(TString source, TString JetType, double eta, double pt, int sys){

  std::map<TString, std::vector<std::map<double, std::vector<double> > > >::iterator mapit;
  bool NotFound=false;
  if(JetType=="AK4PFchs") {
    mapit = AK4CHSJECUncMap.find(source);
    if(mapit == AK4CHSJECUncMap.end()) NotFound=true;
  }
  if(JetType=="AK4PFPuppi") {
    mapit = AK4PUPPIJECUncMap.find(source);
    if(mapit == AK4PUPPIJECUncMap.end()) NotFound=true;
  }
  if(JetType=="AK8PFchs"){
    mapit = AK8CHSJECUncMap.find(source);
    if(mapit == AK8CHSJECUncMap.end()) NotFound=true;
  }
  if(JetType=="AK8PFPuppi") {
    mapit = AK8PUPPIJECUncMap.find(source);
    if(mapit == AK8PUPPIJECUncMap.end()) NotFound=true;
  }
  if(NotFound) {cout<< "ERROR, " << source  << " not found in JEC Uncertainty MAP for " << JetType << endl; return -999.;}


  double bin_boundary(-999.);

  std::map<double, std::vector<double> > ptmap = mapit->second.at(0);

  std::vector<double> etabins;
  for(std::map<double, std::vector<double> >::iterator it = ptmap.begin(); it!= ptmap.end(); it++){
    etabins.push_back(it->first);
  }


  for(unsigned int i=0; i < etabins.size()-1 ; i++){
    if(eta >= etabins.at(i) && eta < etabins.at(i+1)){  bin_boundary = double(etabins.at(i)) ; break;}
  }

  if(bin_boundary == -999) return 1.;

  std::vector<double> ptbins;


  for(std::map<double, std::vector<double> >::iterator pit = ptmap.begin();  pit != ptmap.end(); pit++){
    if(double(pit->first) == double(bin_boundary)) {ptbins = pit->second; }
  }

  int ptbin(-999);
  if(pt >= ptbins.at(ptbins.size() - 1)) ptbin = ptbins.size() - 1;
  for(unsigned int j = 0 ; j < ptbins.size()-1; j++){
    if( pt >= ptbins.at(j)  && pt < ptbins.at(j+1)) {ptbin=j; break;}
  }

  if(ptbin == -999) return 1.;


  std::map<double, std::vector<double> > upmap = mapit->second.at(1);
  std::map<double, std::vector<double> > downmap = mapit->second.at(2);

  std::map<double, std::vector<double> >::iterator mapit_unc;
  if(sys> 0) mapit_unc =  mapit->second.at(1).find(bin_boundary);
  else mapit_unc =  mapit->second.at(2).find(bin_boundary);

  double unc = (sys> 0) ?   1+ mapit_unc->second.at(ptbin) : 1 - mapit_unc->second.at(ptbin);

  return unc;
}








void AnalyzerCore::SetupJECUncertainty(TString source , TString JetType){


  string analysisdir = getenv("DATA_DIR");

  string file = analysisdir + "/"+string(GetEra()) + "/JEC/Summer19UL16APV_V7_MC_UncertaintySources_AK4PFchs.txt";
  if(GetEra() == "2016postVFP") file = analysisdir + "/"+ string(GetEra())+ "/JEC/Summer19UL16_V7_MC_UncertaintySources_AK4PFchs.txt";
  if(GetEra() == "2017") file = analysisdir + "/"+ string(GetEra())+ "/JEC/Summer19UL17_V5_MC_UncertaintySources_AK4PFchs.txt";
  if(GetEra() == "2018") file = analysisdir + "/"+ string(GetEra())+ "/JEC/Summer19UL18_V5_MC_UncertaintySources_AK4PFchs.txt";


  string sline;
  vector<string> SourceLines;
  bool ExtractLine=false;
  int nline(0);
  ifstream jec_file(file.c_str());
  while(getline(jec_file,sline) ){
    if(ExtractLine){
      if(nline==0){nline++; continue;}
      if(sline.find("[")!=string::npos) break;
      if(nline==1)SourceLines.push_back(sline);
    }
    if(sline.find(source)!=string::npos) { ExtractLine=true;}
  }

  jec_file.close();

  cout << "Setting up JEC uncertainty vector for source ["<<source<< "]." << file << endl;

  std::map<double, std::vector<double> > etaptmap, eta_uncupmap, eta_uncdownmap;
  for(unsigned int ilines =0; ilines <  SourceLines.size(); ilines++){

    string line = SourceLines[ilines];
    std::istringstream is( line );

    std::string s_eta_min, s_eta_max, s_nBins;
    is >> s_eta_min;
    is >> s_eta_max;
    is >> s_nBins;

    double  eta_min = stod(s_eta_min);
    double  eta_max = stod(s_eta_max);
    double  nBins = stod(s_nBins);

    bool EtaBinCheck=true;
    if((ilines == SourceLines.size() -1)){
      if( eta_min != 5.0) EtaBinCheck=false;
      if( eta_max != 5.4) EtaBinCheck=false;
    }
    if(!EtaBinCheck){
      cout << "[AnalyzerCore::SetupJECUncertainty] Eta bin set incorrectly ..." << endl;
      exit(EXIT_FAILURE);
    }
    std::vector<double> ptbin, unc_up, unc_down;
    bool finalbin(false);

    for(int i=0; i < nBins; i++){
      std::string  subString;
      is >> subString;
      double value_from_string =  stod(subString);
      if((i %3) == 0) {ptbin.push_back(value_from_string); if( i == nBins-3) finalbin=true; }
      if((i %3) == 1) {unc_up.push_back(value_from_string);}
      if((i %3) == 2) {unc_down.push_back(value_from_string);}

      // Check Final bin is last bin in txt file for this eta bin                                                                                                                                                                                                                                                                                                                                                                        
      if((i %3) == 2 && finalbin) {
        string CheckFinalPt;
        is >> CheckFinalPt;
        if(!CheckFinalPt.empty()) {
          cout << "[AnalyzerCore::SetupJECUncertainty] Last pt bin of  set incorrectly ..." << endl;
          exit(EXIT_FAILURE);
        }
      }
    }
    etaptmap[eta_min] = ptbin;
    eta_uncupmap[eta_min] =  unc_up;
    eta_uncdownmap[eta_min] = unc_down;

    std::vector<double> NULLBin;
    if(ilines ==  SourceLines.size() -1) etaptmap[eta_max] = NULLBin;

  }

  jec_file.close();

  std::vector<std::map<double, std::vector<double> > > vec_unc;
  vec_unc.push_back(etaptmap);
  vec_unc.push_back(eta_uncupmap);
  vec_unc.push_back(eta_uncdownmap);

  if(JetType=="AK4PFchs") AK4CHSJECUncMap[source] = vec_unc;
  if(JetType=="AK4PFPuppi") AK4PUPPIJECUncMap[source] = vec_unc;
  if(JetType=="AK8PFchs") AK8CHSJECUncMap[source] = vec_unc;
  if(JetType=="AK8PFPuppi") AK8PUPPIJECUncMap[source] = vec_unc;

  return;

}

std::vector<Jet> AnalyzerCore::JetsAwayFromFatJet(const std::vector<Jet>& jets, const std::vector<FatJet>& fatjets, double mindr){

  std::vector<Jet> out;
  for(unsigned int i=0; i<jets.size(); i++){

    bool Overlap = false;
    for(unsigned int j=0; j<fatjets.size(); j++){
      if( ( jets.at(i) ).DeltaR( fatjets.at(j) ) < mindr ){
        Overlap = true;
        break;
      }
    }
    if(!Overlap) out.push_back( jets.at(i) );

  }

  std::sort(out.begin(),       out.end(),        PtComparing);

  return out;

}

Jet AnalyzerCore::GetCorrectedJetCloseToLepton(vector<Particle> leps, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton                                                                                                                                                                                                                                                                                                                                                                       
  Particle  rawJet =  jet * jet.JECfactor();
  double l1corrFactor = jet.JEC_L1();

  Particle  rawJetMinusLep = rawJet;
  Particle Lepsp4;
  for(auto ip : leps) {
    Lepsp4 = Lepsp4+ip;

    rawJetMinusLep = rawJetMinusLep - ip* (1.0 / l1corrFactor);
  }
  Particle jetp4 = (rawJetMinusLep ) * (jet.Pt() / rawJet.Pt()) + Lepsp4;


  //cout << "Corr Eta : " << jetp4.Eta() << " " << jet.Eta() << endl;                                                                                                                                                                                                                                                                                                                                                                    
  // cout << "Corr Phi : " << jetp4.Phi() << " " << jet.Phi() << endl;                                                                                                                                                                                                                                                                                                                                                                   

  Jet jet_corr = jet;

  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}



Jet AnalyzerCore::GetCorrectedJetCloseToLepton(Muon lep, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton                                                             
  Particle  rawJet =  jet * jet.JECfactor();

  Particle  lepp4 = lep;
  double l1corrFactor = jet.JEC_L1();
  Particle jetp4 = (rawJet - lepp4 * (1.0 / l1corrFactor)) * (jet.Pt() / rawJet.Pt()) + lepp4;

  Jet jet_corr = jet;

  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}

Jet AnalyzerCore::GetCorrectedJetCloseToLepton(Electron lep, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton                                                                                                                                                                                                                                                                                                                                                                       
  Particle  rawJet =  jet * jet.JECfactor();

  Particle  lepp4 = lep;
  double l1corrFactor = jet.JEC_L1();
  Particle jetp4 = (rawJet - lepp4 * (1.0 / l1corrFactor)) * (jet.Pt() / rawJet.Pt()) + lepp4;

  Jet jet_corr = jet;
  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}

Jet AnalyzerCore::GetCorrectedJetCloseToLepton(Lepton lep, Jet jet){

  //jet_LepAwareJECv2 = (raw_jet * L1 - lepton) * L2L3Res + lepton                                                                                                                                                                                                                                                                                                                                                                       

  Particle  rawJet =  jet * jet.JECfactor();

  Particle  lepp4 = lep;
  double l1corrFactor = jet.JEC_L1();
  Particle jetp4 = (rawJet - lepp4 * (1.0 / l1corrFactor)) * (jet.Pt() / rawJet.Pt()) + lepp4;

  Jet jet_corr = jet;
  jet_corr.SetPtEtaPhiE(jetp4.Pt(), jetp4.Eta(), jetp4.Phi(),jetp4.E());

  return jet_corr;
}



double  AnalyzerCore::JetLeptonMassDropLepAware( Muon lep, bool removeLep, bool ApplyCorr){


  // ApplyCorr def is false, this is same as Mini/NanoAOD value stored                                                                                                                                                                                                                                                                                                                                                                   
  // if ApplyCorr is true then Jet smearing and lepton smearing is applied and values are corrected                                                                                                                                                                                                                                                                                                                                      

  std::vector<Jet> jets = GetAllJets(ApplyCorr);

  double mindR=0.4;
  if(!ApplyCorr)lep.SetPtEtaPhiE(lep.MiniAODPt(), lep.Eta(), lep.Phi(), lep.E());

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }
  if(mindR==0.4) return -1;

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;


  if(removeLep)   jetp4 = jetp4 - lep;

  TLorentzVector LJ =  jetp4 + lep;

  return (LJ.M() - jetp4.M());

}


double  AnalyzerCore::JetLeptonMassDropLepAware( Electron lep, bool removeLep, bool ApplyCorr){
  std::vector<Jet> jets = GetAllJets(ApplyCorr);
  double mindR=0.4;
  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) closejet = GetCorrectedJetCloseToLepton(lep,jet);
  }
  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;
  if(removeLep)   jetp4 = jetp4 - lep;
  TLorentzVector LJ =  jetp4 + lep;
  return (LJ.M() - jetp4.M());
}


double  AnalyzerCore::JetLeptonPtRelLepAware( Muon lep, bool CorrLep){

  if(!CorrLep)lep.SetPtEtaPhiE(lep.MiniAODPt(), lep.Eta(), lep.Phi(), lep.E());
  return JetLeptonPtRelLepAware(Lepton(lep));
}

double  AnalyzerCore::JetLeptonPtRelLepAware( Electron lep){
  return JetLeptonPtRelLepAware(Lepton(lep));
}

double  AnalyzerCore::JetLeptonPtRelLepAware( Lepton lep, Jet jet){

  Jet closejet = GetCorrectedJetCloseToLepton(lep,jet);

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;

  double PtRel = lepp4.Perp((closejet-lepp4).Vect()); // Default                                                                                                                                                                                                                                                                                                                                                                         
  return PtRel;
}


double  AnalyzerCore::JetLeptonPtRelLepAware( Lepton lep){

  // ApplyCorr def is false, this is same as Mini/NanoAOD value stored                                                                                                                                                                                                                                                                                                                                                                   
  // if ApplyCorr is true then Jet smearing and lepton smearing is applied and values are corrected                                                                                                                                                                                                                                                                                                                                      

  std::vector<Jet> jets = GetAllJets(false);

  double mindR=0.4;

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }

  if(mindR==0.4) return 0.;

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;

  double PtRel = lepp4.Perp((closejet-lepp4).Vect()); // Default                                                                                                                                                                                                                                                                                                                                                                         

  return PtRel;

}



double  AnalyzerCore::JetLeptonPtRelLepAwareV2( Lepton lep){

  lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());

  std::vector<Jet> jets = GetAllJets(false);

  double mindR=0.4;

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }

  if(mindR==0.4) return 0.;

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;

  double PtRel = lepp4.Perp((closejet-lepp4).Vect()); // Default                                                                                                                                                                                                                                                                                                                                                                         

  return PtRel;

}


double  AnalyzerCore::JetLeptonPtRatioLepAware(Muon lep, bool CorrLep){
  if(!CorrLep)lep.SetPtEtaPhiE(lep.MiniAODPt(), lep.Eta(), lep.Phi(), lep.E());
  return JetLeptonPtRatioLepAware(Lepton(lep));
}
double  AnalyzerCore::JetLeptonPtRatioLepAware( Electron lep){
  return JetLeptonPtRatioLepAware(Lepton(lep));
}


double  AnalyzerCore::JetLeptonPtRatioLepAware(Lepton lep, Jet jet){
  Jet closejet = GetCorrectedJetCloseToLepton(lep,jet);
  Particle lepp4 = lep;
  return std::min(lepp4.Pt() / closejet.Pt(),1.5);
}

double  AnalyzerCore::JetLeptonPtRatioLepAware(Lepton lep){

  std::vector<Jet> jets = GetAllJets(false);

  double  mindR=0.4;

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }

  Particle lepp4 = lep;

  if (mindR == 0.4)  return std::min(1./ (1. + lep.RelIso()), 1.5);

  return std::min(lepp4.Pt() / closejet.Pt(),1.5);

}

double  AnalyzerCore::JetLeptonPtRatioLepAwareV2(Lepton lep){

  lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());
  std::vector<Jet> jets = GetAllJets(false);

  double  mindR=0.4;

  Jet closejet;
  for(auto jet : jets){
    if (lep.DeltaR(jet) < mindR) {
      closejet = GetCorrectedJetCloseToLepton(lep,jet);
      mindR = lep.DeltaR(jet);
    }
  }

  Particle lepp4 = lep;

  if (mindR == 0.4)  return std::min(1./ (1. + lep.RelIso()), 1.5);

  return std::min(lepp4.Pt() / closejet.Pt(),1.5);

}



double  AnalyzerCore::JetLeptonPtRatioLepAwareMuon(Lepton lep, bool smearjet, bool corrMu, bool uncorrLepE){

  std::vector<Jet> jets = GetAllJets(smearjet);

  if(uncorrLepE)     lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());

  double  mindR=0.4;

  Jet closejet;
  Jet UncorJet;
  for(auto jet : jets){
    if(jet.Pt() < 10) continue;

    if (lep.DeltaR(jet) < mindR) {
      UncorJet=jet;
      mindR = lep.DeltaR(jet);
    }
  }

  vector<Muon> LooseMuons = All_Muons;
  vector<Particle> OverlapLeps = {lep};
  if(corrMu){
    for(auto imu : LooseMuons){
      if(UncorJet.DeltaR(imu) < 0.4){
        if (imu.PassID("POGLoose")) {
          OverlapLeps.push_back(imu);
        }
      }
    }
  }

  closejet = GetCorrectedJetCloseToLepton(OverlapLeps, UncorJet);

  Particle lepp4 = lep;

  if (mindR == 0.4)  return std::min(1./ (1. + lep.RelIso()), 1.5);

  return std::min(lepp4.Pt() / closejet.Pt(),1.5);

}


double  AnalyzerCore::JetLeptonPtRelLepAwareMuon( Lepton lep, bool smearjet, bool corrMu, bool uncorrLepE){

  std::vector<Jet> jets = GetAllJets(smearjet);

  if(uncorrLepE)     lep.SetPtEtaPhiE(lep.UncorrectedPt(),lep.Eta(), lep.Phi(), lep.E());

  double mindR=0.4;

  Jet closejet;
  Jet UncorJet;

  for(auto jet : jets){
    if(jet.Pt() < 10) continue;

    if (lep.DeltaR(jet) < mindR) {
      UncorJet = jet;
      mindR = lep.DeltaR(jet);
    }
  }

  if(mindR==0.4) return 0.;

  vector<Muon> LooseMuons = All_Muons;
  vector<Particle> OverlapLeps = {lep};
  if(corrMu){
    for(auto imu : LooseMuons){
      if(UncorJet.DeltaR(imu) < 0.4){
        if (imu.PassID("POGLoose")) {
          OverlapLeps.push_back(imu);
        }
      }
    }
  }


  //  if(OverlapLeps.size() > 1)                                                                                                                                                                                                                                                                                                                                                                                                         
  closejet = GetCorrectedJetCloseToLepton(OverlapLeps, UncorJet);

  TLorentzVector lepp4 = lep;
  TLorentzVector jetp4 = closejet;


  double PtRel = lepp4.Perp((jetp4-lepp4).Vect()); // Default                                                                                                                                                                                                                                                                                                                                                                            

  return PtRel;

}


