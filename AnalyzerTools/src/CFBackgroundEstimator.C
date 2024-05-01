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
      map_hist_Electron[a+"_"+b+"_"+c] = (TH2D *)file->Get(this_cfname)->Clone();
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


double CFBackgroundEstimator::GetElectronCFRateFitted(TString ID, TString key, double eta, double pt, int sys){

  eta = fabs(eta);
  if(eta>=2.5) eta = 2.49;
  
  //// Lowest pt value is 20;                                                                                                                                                                

  if(pt < 15) pt = 15;
  if(pt > 1000) pt = 999;

  std::map< TString, TH2D* >::const_iterator mapit;
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

  int this_bin = (key.Contains("Inv")) ?  (mapit->second)->FindBin(1/pt,eta) :  (mapit->second)->FindBin(pt,eta);
  
  int this_bin_m1 = this_bin-1;
  int this_bin_p1 = this_bin+1;

  double value_MainBin = (mapit->second)->GetBinContent(this_bin);
  double value_mBin = (mapit->second)->GetBinContent(this_bin_m1);
  double value_pBin = (mapit->second)->GetBinContent(this_bin_p1);
  double IPt = (1/pt);  
  if(IPt > 0.04)  return value_MainBin;
  if(IPt < 0.002) return value_MainBin;

  vector <double> invptbins = { 0., 0.002,
		       0.003,
		       0.004,
		       0.005,
		       0.0075,
		       0.010,
		       0.0125,
		       0.015,
		       0.0175,
		       0.020,
		       0.0225,
		       0.025,
		       0.030,
		       0.035,
		       0.04,
		       0.07};


  vector <double> BinCentres;
  double Center_MainBin(0.), Center_mBin(0.), Center_pBin(0.);
  bool corrBinFound=false;
  for(int ib =2; ib < invptbins.size()-1; ib++){
    //cout << "invptbins = " << invptbins[ib] << " IPt = " << IPt << endl;
    if(!corrBinFound && (IPt <= invptbins[ib]) && (IPt >invptbins[ib-1]) ) {
      //cout << "Correct bin " << endl;
      corrBinFound=true;
      Center_MainBin = (invptbins[ib] + invptbins[ib-1]) /2.;
      Center_mBin    = (invptbins[ib-1] + invptbins[ib-2]) /2.;
      Center_pBin    = (invptbins[ib+1] + invptbins[ib]) /2.;
      //cout << "Center_MainBin = " << Center_MainBin << " Center_mBin = " << Center_mBin << " Center_pBin = " << Center_pBin << endl;

    }
  }
  //  PrintBins((mapit->second));


  //  cout << "  "<< endl;
  //cout << "Pt = " <<  pt << " IPt = " << IPt << " this_bin = " << this_bin << " this_bin_m1 = " << this_bin_m1 << "  = " << this_bin_p1 << endl;
  //cout << "value_MainBin = " << value_MainBin << " value_mBin " << value_mBin << " value_pBin = " << value_pBin << endl;
  //cout << "Center_MainBin = " << Center_MainBin << " Center_mBin = " << Center_mBin << " Center_pBin = " << Center_pBin << endl;

  
  ////    0         0.002     0.003                      0.04     0.07
  ////    |         |         |       |    |      |     |    |
  
  if(IPt < Center_MainBin) {
    double dX = Center_MainBin - Center_mBin;
    double dY = value_mBin - value_MainBin;
    
    double CFRate = value_MainBin + (dY/dX) * (Center_MainBin-IPt);
    
    
    //cout << "Left of centre : IPt = " << IPt << " dX = " << dX << " dY = " << dY << " (Center_MainBin-IPt) = " << (Center_MainBin-IPt) << endl;
    //cout << "Left of centre : CFRate : " << value_MainBin << " --> "<<  CFRate << endl;
    if(isnan(CFRate)) {
      cout << "CF Fit returns nan. " << endl;
      
      exit(ENODATA);
    }
    return CFRate;
  }
  else{
    double dX = Center_pBin - Center_MainBin;
    double dY = value_MainBin - value_pBin;

    // cout << "Right of centre : IPt = " << IPt << " dX = "<< dX << " dY = " << dY<< " (Center_MainBin-IPt) = " << (Center_MainBin-IPt) << endl;
    double CFRate = value_MainBin + (dY/dX) * (Center_MainBin-IPt);

    //cout << "Right of centre : CFRate : "<< value_MainBin << " --> "<<  CFRate << endl;

    if(isnan(CFRate)) {
      cout << "CF Fit returns nan. " <<endl;
      
      exit(ENODATA);
    }
    
    return CFRate;
  }

  
}

double CFBackgroundEstimator::GetElectronCFRate(TString ID, TString key, double eta, double pt, int sys){

  //cout << "[CFBackgroundEstimator::GetElectronCFRate] ID = " << ID << ", key = " << key << endl;
  //cout << "[CFBackgroundEstimator::GetElectronCFRate] eta = " << eta << ", pt = " << pt << endl;

  double value = 1.;
  double error = 0.;

  eta = fabs(eta);
  if(eta>=2.5) eta = 2.49;
  
  //// Lowest pt value is 20;

  if(pt < 15) pt = 15;
  
  //if(pt < 25) {
  //// Temp fix until new Rates are added with low pt bin
  //key = key.ReplaceAll("Inv","");
  //}
  
  if(pt > 1000) pt = 999;
 
 
  std::map< TString, TH2D* >::const_iterator mapit;

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

  int this_bin = (key.Contains("Inv")) ?  (mapit->second)->FindBin(1/pt,eta) :  (mapit->second)->FindBin(pt,eta);
  value = (mapit->second)->GetBinContent(this_bin);
  error = (mapit->second)->GetBinError(this_bin);

  if(pt > 500) error = value *0.5; ///// FIX Later

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




