#include "FakeBackgroundEstimator.h"

FakeBackgroundEstimator::FakeBackgroundEstimator() : 
IgnoreNoHist(false),
HasLooseLepton(false)
{

  histDir = TDirectoryHelper::GetTempDirectory("FakeBackgroundEstimator");

}

void FakeBackgroundEstimator::ReadHistograms(){

  TString datapath = getenv("DATA_DIR");
  datapath = datapath+"/"+GetEra()+"/FakeRate/";

  TDirectory* origDir = gDirectory;

  string elline;
  ifstream in(datapath+"/histmap_Electron.txt");
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString a,b,c,d,e;
    is >> a; // <Rate>
    is >> b; // <IDlabel>
    is >> c; // <var key>
    is >> d; // <syst key>
    is >> e; // <rootfilename>

    if(a.Contains("#")) continue; //JH : skip so many histograms
    
    TFile *file = new TFile(datapath+"/"+e);
    TList *histlist = file->GetListOfKeys(); //JH : call hist names consisting of b_c_d; e.g. HNTightV2_ptcone_ptfix_eta_AwayJetPt40, HNTight_17028_ptcone_ptfix_eta_AwayJetPt40, ...
    for(int i=0;i<histlist->Capacity();i++){
      TString this_frname = histlist->At(i)->GetName();

      if (!this_frname.Contains(b)) continue;
      if (!this_frname.Contains(c)) continue;
      if (!this_frname.Contains(d)) continue; //JH : filter out unmatched hist name

      histDir->cd();

      map_hist_Electron[a+"_"+b+"_"+c+"_"+d] = (TH2D *)file->Get(this_frname)->Clone(a+"_"+b+"_"+c+"_"+d);

      origDir->cd();
      cout << "[FakeBackgroundEstimator::FakeBackgroundEstimator] map_hist_Electron : " << a+"_"+b+"_"+c+"_"+d+ " --> "+this_frname << endl;

    }
    file->Close();
    delete file;
  }

  string elline2;
  ifstream in2(datapath+"/histmap_Muon.txt");
  while(getline(in2,elline2)){
   std::istringstream is( elline2 );
    TString a,b,c,d,e;
    is >> a; // <Rate>  
    is >> b; // <IDlabel> 
    is >> c; // <var key> 
    is >> d; // <syst key>
    is >> e; // <rootfilename>                                                                                                                                                                    
    if(a.Contains("#")) continue; //JH : skip so many histograms

    TFile *file = new TFile(datapath+"/"+e);
    TList *histlist = file->GetListOfKeys(); //JH : call hist names consisting of b_c_d; e.g. HNTightV2_ptcone_ptfix_eta_AwayJetPt40, HNTight_17028_ptcone_ptfix_eta_AwayJetPt40, ...
    for(int i=0;i<histlist->Capacity();i++){
      TString this_frname = histlist->At(i)->GetName();
      if (!this_frname.Contains(b)) continue;
      if (!this_frname.Contains(c)) continue;
      if (!this_frname.Contains(d)) continue; //JH : filter out unmatched hist name
      
      histDir->cd();

      map_hist_Muon[a+"_"+b+"_"+c+"_"+d] = (TH2D *)file->Get(this_frname)->Clone();

      origDir->cd();
      cout << "[FakeBackgroundEstimator::FakeBackgroundEstimator] map_hist_Muon : " << a+"_"+b+"_"+c+"_"+d+ " --> "+this_frname << endl;

    }
    file->Close();
    delete file;
  }

}

FakeBackgroundEstimator::~FakeBackgroundEstimator(){

}

double FakeBackgroundEstimator::GetElectronFakeRate(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[FakeBackgroundEstimator::GetElectronFakeRate] ID = " << ID << ", key = " << key << endl;
  //cout << "[FakeBackgroundEstimator::GetElectronFakeRate] eta = " << eta << ", pt = " << pt << endl;

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);

  if(pt>=60) pt = 59;
  if(pt < 10) pt=11;
  
  ID = ID.ReplaceAll("ElOpt_","");
  ID = ID.ReplaceAll("MuOpt_","");
  
  std::map< TString, TH2D* >::const_iterator mapit;
  //passPOGMedium_LIP_ptcone_eta_AwayJetPt60;1

  //cout << "FakeRate_"+ID+"_"+key << endl;
  mapit = map_hist_Electron.find("FakeRate_"+ID+"_"+key);

  if(mapit==map_hist_Electron.end()){

    if(IgnoreNoHist) return 1.;
    else{

      cout << "[FakeBackgroundEstimator::GetElectronFakeRate] No "<< ID+"_"+key <<endl;
      exit(ENODATA);
    }
  }

  int this_bin = (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  //cout << "[FakeBackgroundEstimator::GetElectronFakeRate] pt = " << pt << ", eta = " << eta << endl;
  //cout << "[FakeBackgroundEstimator::GetElectronFakeRate] value = " << value << endl;

  return value+double(sys)*error;

}

double FakeBackgroundEstimator::GetElectronPromptRate(TString ID, TString key, double eta, double pt, int sys){

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);
  if(pt>=2000) pt = 1999;
  if(pt < 15) pt=15;


  std::map< TString, TH2D* >::const_iterator mapit;
  mapit = map_hist_Electron.find("PromptRate_"+ID+"_"+key);

  if(mapit==map_hist_Electron.end()){
    if(IgnoreNoHist) return 1.;
    else{
      cout << "[FakeBackgroundEstimator::GetElectronPromptRate] No "<< ID+"_"+key <<endl;
      exit(ENODATA);
    }
  }

  int this_bin = (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  return value+double(sys)*error;

}

double FakeBackgroundEstimator::GetMuonPromptRate(TString ID, TString key, double eta, double pt, int sys){

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);
  if(pt>=200) pt = 199;
  if(pt < 10) pt=10;


  std::map< TString, TH2D* >::const_iterator mapit;
  mapit = map_hist_Muon.find("PromptRate_"+ID+"_"+key);

  if(mapit==map_hist_Muon.end()){
    if(IgnoreNoHist) return 1.;
    else{
      cout << "[FakeBackgroundEstimator::GetMuonPromptRate] No "<< ID+"_"+key <<endl;
      exit(ENODATA);
    }
  }

  int this_bin = (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  return value+double(sys)*error;

}


double FakeBackgroundEstimator::GetMuonFakeRate(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[FakeBackgroundEstimator::GetMuonFakeRate] ID = " << ID << ", key = " << key << endl;
  //cout << "[FakeBackgroundEstimator::GetMuonFakeRate] eta = " << eta << ", pt = " << pt << endl;

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);
  if(pt>=60) pt = 59;
  if(pt < 7) pt=7;

  ID = ID.ReplaceAll("ElOpt_","");
  ID = ID.ReplaceAll("MuOpt_","");
  
  std::map< TString, TH2D* >::const_iterator mapit;
  mapit = map_hist_Muon.find("FakeRate_"+ID+"_"+key);
  //  cout << "FakeRate_"+ID+"_"+key << endl;

  if(mapit==map_hist_Muon.end()){
    if(IgnoreNoHist) return 1.;
    else{
      cout << "[FakeBackgroundEstimator::GetMuonFakeRate] No "<< ID+"_"+key <<endl;
      exit(ENODATA);
    }
  }

  int this_bin = (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  //cout << "[FakeBackgroundEstimator::FakeBackgroundEstimator] value = " << value << endl;

  return value+double(sys)*error;

}

double FakeBackgroundEstimator::GetWeight(vector<Lepton *> lepptrs, AnalyzerParameter param, int sys){

  double this_weight = -1.;
  vector<double> FRs;
  for(unsigned int i=0; i<lepptrs.size(); i++){
    double this_fr = -999.;
    if(lepptrs.at(i)->LeptonFlavour()==Lepton::ELECTRON){

      Electron *el = (Electron *)( lepptrs.at(i) );
      if( el->PassID(param.Electron_Tight_ID) ) continue;

      double this_pt = el->Pt();
      //      if(param.Electron_UsePtCone) this_pt = el->PtCone();
      this_fr = GetElectronFakeRate(param.Electron_FR_ID, param.Electron_FR_Key, fabs(el->scEta()), this_pt, sys);
      this_weight *= -1.*this_fr/(1.-this_fr);
      FRs.push_back(this_fr);
    }
    else{

      Muon *mu = (Muon *)( lepptrs.at(i) );
      if( mu->PassID(param.Muon_Tight_ID) ) continue;
      
      double this_pt = mu->Pt();
      //if(param.Muon_UsePtCone) this_pt = mu->PtCone();
      this_fr = GetMuonFakeRate(param.Muon_FR_ID, param.Muon_FR_Key, fabs(mu->Eta()), this_pt, sys);
      this_weight *= -1.*this_fr/(1.-this_fr);
      FRs.push_back(this_fr);
    }
  }
  if(FRs.size()==0){
    HasLooseLepton = false;
    return 0;
  }
  else{
    HasLooseLepton = true;
    return this_weight;
  }

}



double FakeBackgroundEstimator::GetFullWeight(vector<Lepton *> lepptrs, AnalyzerParameter param, int sys){

  vector<double> FRs;
  vector<double> PRs;
  vector<bool>  isT;
  for(unsigned int i=0; i<lepptrs.size(); i++){
    double this_fr = -999.;
    double this_pr = -999.;
    if(lepptrs.at(i)->LeptonFlavour()==Lepton::ELECTRON){

      Electron *el = (Electron *)( lepptrs.at(i) );
      isT.push_back(el->PassID(param.Electron_Tight_ID));

      double this_pt = el->Pt();
      this_fr = GetElectronFakeRate(param.Electron_FR_ID, param.Electron_FR_Key, fabs(el->scEta()), this_pt, sys);
      this_pr = GetElectronPromptRate(param.Electron_FR_ID, param.Electron_FR_Key, fabs(el->scEta()), this_pt, sys);
      //this_weight *= -1.*this_fr/(1.-this_fr);
      FRs.push_back(this_fr);
      PRs.push_back(this_pr);
    }
    else{

      Muon *mu = (Muon *)( lepptrs.at(i) );
      isT.push_back(mu->PassID(param.Muon_Tight_ID) );
      double this_pt = mu->Pt();
      //if(param.Muon_UsePtCone) this_pt = mu->PtCone();                                                              
      this_fr = GetMuonFakeRate(param.Muon_FR_ID, param.Muon_FR_Key, fabs(mu->Eta()), this_pt, sys);
      this_pr = GetMuonPromptRate(param.Muon_FR_ID, param.Muon_FR_Key, fabs(mu->Eta()), this_pt, sys);
      //this_weight *= -1.*this_fr/(1.-this_fr);
      FRs.push_back(this_fr);
      PRs.push_back(this_pr);
    }
  }
  if(FRs.size()==0){
    HasLooseLepton = false;
    return 0;
  }

  if(lepptrs.size() == 2) return CalculateDilepWeight(PRs[0],FRs[0],PRs[1],FRs[1], isT[0],isT[1], 0);
  
  if(lepptrs.size() == 1) return CalculateLepWeight(PRs[0],FRs[0], isT[0]);

  return 1.;
}



float FakeBackgroundEstimator::CalculateLepWeight(float r1, float fr1, bool lep1Tight){

  //https://cds.cern.ch/record/1951336/files/ATLAS-CONF-2014-058.pdf
  double w_l =0.;
  double alpha = (fr1/(r1-fr1)) * (r1);
  if(lep1Tight) w_l = - alpha  ;
  else w_l = alpha * r1;

  
  return w_l;

}

float FakeBackgroundEstimator::CalculateDilepWeight(float r1, float fr1, float r2, float fr2, bool lep1Tight, bool lep2Tight, int eventtype){

  // Function calculates event weight given r/f of two lepons in the event
  double alpha = 1./((r1- fr1)*(r2- fr2));

  // Initialise weight
  float w_ll=-999.;

  // Terms for RF and FR events from MM
  double termTT_sf = alpha*(r1*fr2*(fr1-1.)*(1.-r2) + (fr1*r2*(r1-1.)*(1.-fr2)));
  double termTL_sf = alpha*(r1*fr2*((1-fr1)*r2) + (fr1*r2*(1.-r1)*fr2));
  double termLT_sf = alpha*(r1*fr2*(fr1*(1.-r2))+ (fr1*r2*(1.-fr2)*r1));
  double termLL_sf = -2.*alpha*(r1*fr2*(fr1*r2));

  
  // Term for FF events
  double termTT_df = alpha*fr1*fr2*((1.-r1)*(1.-r2));
  double termTL_df = alpha*fr1*fr2*r2*(r1-1.);
  double termLT_df = alpha*fr1*fr2*r1*(r2-1.);
  double termLL_df = alpha*r1*r2*(fr1*fr2);

  double termTT(0.);
  double termTL(0.);
  double termLT(0.);
  double termLL(0.);
  
  if(eventtype==0) {
    termTT = termTT_sf+ termTT_df;
    termTL = termTL_sf + termTL_df;
    termLT = termLT_sf + termLT_df;
    termLL = termLL_sf + termLL_df;    
  }
  else   if(eventtype==1) {
    termTT = termTT_sf;
    termTL = termTL_sf;
    termLT = termLT_sf;
    termLL = termLL_sf;
  }
  else if(eventtype==2) {
    termTT = termTT_df;
    termTL = termTL_df;
    termLT = termLT_df;
    termLL = termLL_df;
  }

  // decide which type of event we have so to return correct weight
  bool isLL = (!lep1Tight&&!lep2Tight);
  bool isLT = (!lep1Tight&&lep2Tight);
  bool isTL = (lep1Tight && !lep2Tight);
  bool isTT = (lep1Tight && lep2Tight);

  if(isLL)w_ll = termLL;
  if(isTL)w_ll = termTL;
  if(isLT)w_ll = termLT;
  if(isTT)w_ll=  termTT;


  return w_ll;
}




