#include "CFBackgroundEstimator.h"

CFBackgroundEstimator::CFBackgroundEstimator()
{

  MissingHists.clear();

  IgnoreNoHist = false;
    
  histDir = TDirectoryHelper::GetTempDirectory("CFBackgroundEstimator");

}

void CFBackgroundEstimator::ReadHistograms(){

  TString datapath = getenv("DATA_DIR");
  datapath = datapath+"/"+GetEra()+"/CFRate/";

  TDirectory* origDir = gDirectory;

  string elline;
  ifstream in(datapath+"/histmap_Electron.txt");
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString a,b,c,d,e;
    is >> a; // <Rate>
    is >> b; // histlabel
    is >> c; // syst
    is >> d; // <rootfilename>

    TFile *file = new TFile(datapath+"/"+d);
    TList *histlist = file->GetListOfKeys();
    for(int i=0;i<histlist->Capacity();i++){
      TString this_cfname = histlist->At(i)->GetName();
      if(!this_cfname.Contains(b)) continue;
      if(!this_cfname.Contains(c)) continue;
      histDir->cd();
      map_hist_Electron[a+"_"+b+"_"+c] = (TH1D *)file->Get(this_cfname)->Clone();
      file->Close();
      delete file;
      origDir->cd();
      cout << "[CFBackgroundEstimator::CFBackgroundEstimator] map_hist_Electron : " << a+"_"+b+"_"+c+" --> "+this_cfname << endl;
    }
  }

  string elline2;
  ifstream in2(datapath+"/histmap_Muon.txt");
  while(getline(in2,elline2)){
    std::istringstream is( elline2 );
    TString a,b,c,d,e;
    is >> a; // <ID>
    is >> b; // <rootfilename>
    TFile *file = new TFile(datapath+"/"+b);
    TList *histlist = file->GetListOfKeys();
    for(int i=0;i<histlist->Capacity();i++){
      TString this_cfname = histlist->At(i)->GetName();
      histDir->cd();
      map_hist_Muon[a+"_"+this_cfname] = (TH1D *)file->Get(this_cfname);
      file->Close();
      delete file;
      origDir->cd();
      cout << "[CFBackgroundEstimator::CFBackgroundEstimator] map_hist_Muon : " << a+"_"+this_cfname << endl;
    }
  }

}

CFBackgroundEstimator::~CFBackgroundEstimator(){

  if(MissingHists.size() > 0){
    cout << "CFBackgroundEstimator Missing Hists " << endl;
    for(auto iMissing : MissingHists) cout << "Missing : " << iMissing << endl;
  }
}

double CFBackgroundEstimator::GetElectronCFRate(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[CFBackgroundEstimator::GetElectronCFRate] ID = " << ID << ", key = " << key << endl;
  //cout << "[CFBackgroundEstimator::GetElectronCFRate] eta = " << eta << ", pt = " << pt << endl;

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);
  if(eta>=2.5) eta = 2.49;
  
  
  if(pt < 15) pt = 15;
  if(key.Contains("PtInv")){
    if(pt > 500) pt = 499;
  }
  else{
    if(pt > 200) pt = 199;
  }
  
  TString EtaRegion = "EtaRegion1";
  if(eta<0.8) EtaRegion = "EtaRegion1";
  else if(eta<1.2)   EtaRegion = "EtaRegion2";
  else if(eta<1.47) EtaRegion = "EtaRegion3";
  else if(eta<1.9) EtaRegion = "EtaRegion4";
  else if(eta<2.1) EtaRegion = "EtaRegion5";
  else if(eta<2.2) EtaRegion = "EtaRegion6";
  else if(eta<2.3) EtaRegion = "EtaRegion7";
  else if(eta<2.4) EtaRegion = "EtaRegion8";
  else EtaRegion = "EtaRegion9";

  key = key.ReplaceAll("EtaRegion",EtaRegion);
  std::map< TString, TH1D* >::const_iterator mapit;

  mapit = map_hist_Electron.find(key );

  if(mapit==map_hist_Electron.end()){
    cout << "[CFBackgroundEstimator::GetElectronCFRate] No"<< key  <<endl;
    if(IgnoreNoHist) {
      TString MapK = key;
      if (std::find(MissingHists.begin(), MissingHists.end(), MapK ) == MissingHists.end())   MissingHists.push_back(MapK);
      return 1.;
    }

    exit(ENODATA);
  }

  int this_bin = (key.Contains("PtInv")) ? (mapit->second)->FindBin(1./pt) : (mapit->second)->FindBin(pt);

  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  //  cout << "[CFBackgroundEstimator::CFBackgroundEstimator] value = " << value << endl;

  return value+double(sys)*error;

}

double CFBackgroundEstimator::GetElectronCFRate2D(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[CFBackgroundEstimator::GetElectronCFRate] ID = " << ID << ", key = " << key << endl;                                                                        
  //cout << "[CFBackgroundEstimator::GetElectronCFRate] eta = " << eta << ", pt = " << pt << endl;                                                                        

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);
  if(eta>=2.5) eta = 2.49;

  if(pt > 250.) pt = 249.;

  std::map< TString, TH1D* >::const_iterator mapit;
  mapit = map_hist_Electron.find("Rate_" + ID+"_"+key);
  
  if(mapit==map_hist_Electron.end()){
    if(IgnoreNoHist) {
      TString MapK = "Rate_" + ID+"_"+key;
      if (std::find(MissingHists.begin(), MissingHists.end(), MapK ) == MissingHists.end())   MissingHists.push_back(MapK);
      return 1.;
    }
    
    cout << "[CFBackgroundEstimator::GetElectronCFRate] No"<< ID+"_"+key+"_pteta" <<endl;
    exit(ENODATA);
  }

  if(!mapit->second) {
    if(IgnoreNoHist) {
      TString MapK = "Rate_" + ID+"_"+key;
      if (std::find(MissingHists.begin(), MissingHists.end(), MapK ) == MissingHists.end())   MissingHists.push_back(MapK);
      return 1.;
    }
  }
  int this_bin = (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  //cout << "[CFBackgroundEstimator::CFBackgroundEstimator] value = " << value << endl;                                                                                   

  return value+double(sys)*error;

}

double CFBackgroundEstimator::GetMuonCFRate(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[CFBackgroundEstimator::GetMuonCFRate] ID = " << ID << ", key = " << key << endl;
  //cout << "[CFBackgroundEstimator::GetMuonCFRate] eta = " << eta << ", pt = " << pt << endl;

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);

  if(eta>=2.5) eta = 2.49;

  TString EtaRegion = "InnerBarrel";
  if(eta<0.8) EtaRegion = "InnerBarrel";
  else if(eta<1.479) EtaRegion = "OuterBarrel";
  else EtaRegion = "EndCap";

  std::map< TString, TH1D* >::const_iterator mapit;
  mapit = map_hist_Muon.find(ID+"_"+key+"_"+EtaRegion+"_InvGenPt");

  if(mapit==map_hist_Muon.end()){
    cout << "[CFBackgroundEstimator::GetMuonCFRate] No"<< ID+"_"+key+"_"+EtaRegion+"_InvGenPt" <<endl;
    exit(ENODATA);
  }

  int this_bin = (mapit->second)->FindBin(1./pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  //cout << "[CFBackgroundEstimator::CFBackgroundEstimator] value = " << value << endl;

  return value+double(sys)*error;

}

double CFBackgroundEstimator::GetWeight(vector<Lepton *> lepptrs, AnalyzerParameter param, int sys){

  double this_weight = 0.;
  for(unsigned int i=0; i<lepptrs.size(); i++){

    double this_cf = -999.;

    if(lepptrs.at(i)->LeptonFlavour()==Lepton::ELECTRON){

      Electron *el = (Electron *)( lepptrs.at(i) );

      this_cf = GetElectronCFRate(param.Electron_CF_ID, param.k.Electron_CF, fabs(el->scEta()), el->Pt(), sys);

    }
    else{

      Muon *mu = (Muon *)( lepptrs.at(i) );

      this_cf = GetMuonCFRate(param.Muon_CF_ID, param.k.Muon_CF, fabs(mu->Eta()), mu->Pt(), sys);

    }

    this_weight += this_cf/(1.-this_cf);

  }

  return this_weight;

}




