#include "CFBackgroundEstimator.h"

CFBackgroundEstimator::CFBackgroundEstimator()
{

  MissingHists.clear();

  IgnoreNoHist = false;
    
  histDir = TDirectoryHelper::GetTempDirectory("CFBackgroundEstimator");

}

void CFBackgroundEstimator::ReadHistograms(bool loadStandard){

  TString datapath = getenv("DATA_DIR");
  datapath = datapath+"/"+GetEra()+"/CFRate/";

  TDirectory* origDir = gDirectory;

  bool loadStudy=false;
  bool loadSyst=false;
  
  if(loadStandard){
    string elline;
    ifstream in(datapath+"/histmap_Electron.txt");
    while(getline(in,elline)){
      std::istringstream is( elline );
      TString a,b,c,d,e,f;
      is >> a; // <Rate>                                                                                                                                                                                                                                                      
      is >> b; // histlabel                                                                                                                                                                                                                                                   
      is >> c; // shiftlabel                                                                                                                                                                                                                                                  
      is >> d; // syst                                                                                                                                                                                                                                                        
      is >> e; // ID                                                                                                                                                                                                                                                          
      is >> f; // <rootfilename>                                                                                                                                                                                                                                              
      TFile *file = new TFile(datapath+"/"+f);
      TList *histlist = file->GetListOfKeys();

      for(int i=0;i<histlist->Capacity();i++){
        TString this_cfname = histlist->At(i)->GetName();
        //cout << this_cfname << " " << b+"_"+c+"_"+d  << " --- " << f << endl;                                                                                                                                                                                               
        if(!this_cfname.Contains(b+"_"+c+"_"+d )) continue;
        if(!this_cfname.Contains(e)) continue;
        histDir->cd();
        map_hist_Electron[a+"_"+b+"_"+c+"_"+d+"_"+e] = (TH2D *)file->Get(this_cfname)->Clone();
        cout << "[CFBackgroundEstimator::CFBackgroundEstimator] map_hist_Electron : " << a+"_"+b+"_"+c+"_"+d+"_"+e +"  --> "+this_cfname << endl;
      }

      //// Delete  File after loop                                                                                                                                                                                                                                            
      file->Close();
      delete file;
      origDir->cd();

    }
  }
  else{
    if(loadStudy){
      string elline;
      ifstream in(datapath+"/histmap_StudyElectron.txt");
      while(getline(in,elline)){
	std::istringstream is( elline );
	TString a,b,c,d,e,f;
	is >> a; // <Rate>
	is >> b; // histlabel
	is >> c; // shiftlabel
	is >> d; // syst
	is >> e; // ID
	is >> f; // <rootfilename>
	TFile *file = new TFile(datapath+"/"+f);
	TList *histlist = file->GetListOfKeys();
	
	for(int i=0;i<histlist->Capacity();i++){
	  TString this_cfname = histlist->At(i)->GetName();
	  //cout << this_cfname << " " << b+"_"+c+"_"+d  << " --- " << f << endl;
	  if(!this_cfname.Contains(b+"_"+c+"_"+d )) continue;
	  if(!this_cfname.Contains(e)) continue;
	  histDir->cd();
	  map_hist_Electron[a+"_"+b+"_"+c+"_"+d+"_"+e] = (TH2D *)file->Get(this_cfname)->Clone();
	  cout << "[CFBackgroundEstimator::CFBackgroundEstimator] map_hist_Electron : " << a+"_"+b+"_"+c+"_"+d+"_"+e +"  --> "+this_cfname << endl;
	}
	
	//// Delete  File after loop
	file->Close();
	delete file;
	origDir->cd();
	
      }
    }
    if(loadSyst){
      string elline;
      ifstream in(datapath+"/histmap_SystElectron.txt");
      while(getline(in,elline)){
	std::istringstream is( elline );
	TString a,b,c,d,e,f;
	is >> a; // <Rate>                                                                                                                                                                                                                              
	is >> b; // histlabel                                                                                                                                                                                                                           
	is >> c; // shiftlabel                                                                                                                                                                                                                          
	is >> d; // syst                                                                                                                                                                                                                                
	is >> e; // ID                                                                                                                                                                                                                                  
	is >> f; // <rootfilename>                                                                                                                                                                                                                      
	TFile *file = new TFile(datapath+"/"+f);
	TList *histlist = file->GetListOfKeys();
	
	for(int i=0;i<histlist->Capacity();i++){
	  TString this_cfname = histlist->At(i)->GetName();
	  if(!this_cfname.Contains(b+"_"+c+"_"+d )) continue;
	  if(!this_cfname.Contains(e)) continue;
	  histDir->cd();
	  map_hist_Electron[a+"_"+b+"_"+c+"_"+d+"_"+e] = (TH2D *)file->Get(this_cfname)->Clone();
	  cout << "[CFBackgroundEstimator::CFBackgroundEstimator] map_hist_Electron : " << a+"_"+b+"_"+c+"_"+d+"_"+e +"  --> "+this_cfname << endl;
	}
	
	//// Delete  File after loop                                                                                                                                                                                                                    
	file->Close();
	delete file;
	origDir->cd();
	
      }
    }
  }

  
  cout << "Ending ReadHistograms " << endl;
  for(auto i : map_hist_Electron) cout << i.first << endl;
  return;
}

CFBackgroundEstimator::~CFBackgroundEstimator(){

  if(MissingHists.size() > 0){
    cout << "CFBackgroundEstimator Missing Hists " << endl;
    for(auto iMissing : MissingHists) cout << "Missing : " << iMissing << endl;
  }
}


double CFBackgroundEstimator::GetElectronCFRateFitted(TString ID, TString EtaBinTag, TString key, double eta, double pt, int sys){

  vector<double> InputBins_BB1 = {  0.,
				   0.0015,
				   0.002,
				   0.00225,
				   0.0025,
				   0.00275,
				   0.003,
				   0.004,
				   0.005,
				   0.006,
				   0.007,
				   0.008,
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

  vector<double> InputBins_BB2 = {0.,
				  0.002,
				  0.0025,
				  0.003,
				  0.004,
				  0.005,
				  0.006,
				  0.007,
				  0.008,
				  0.010,
				  0.0125,
				  0.015,
				  0.020,
				  0.025,
				  0.030,
				  0.04,
				  0.07};


  vector<double> InputBins_EC1 =  { 0.,
				   0.0015,
				   0.002,
				   0.00225,
				   0.0025,
				   0.00275,
				   0.003,
				   0.0035,
				   0.004,
				   0.0045,
				   0.005,
				   0.0055,
				   0.006,
				   0.007,
				   0.008,
				   0.009,
				   0.010,
				   0.0111,
				   0.0125,
				   0.0135,
				   0.015,
				   0.0175,
				   0.020,
				   0.0225,
				   0.025,
				   0.030,
				   0.035,
				   0.04,
				   0.07};


  vector<double> InputBins_EC2 =  { 0.,
				   0.00225,
				   0.0025,
				   0.00275,
				   0.003,
				   0.0035,
				   0.004,
				   0.0045,
				   0.005,
				   0.0055,
				   0.006,
				   0.007,
				   0.008,
				   0.009,
				   0.010,
				   0.0111,
				   0.0125,
				   0.0135,
				   0.015,
				   0.0175,
				   0.020,
				   0.0225,
				   0.025,
				   0.030,
				   0.035,
				   0.04,
				   0.07};


  vector<double> InputBins_Pt    = { 15.,
				    25.,
				    40.,
				    45.,
				    50.,
				    55.,
				    60.,
				    65.,
				    70.,
				    80.,
				    90.,
				    100.,
				    125.,
				    150.,
				    175.,
				    200.,
				    300.,
				    400.,
				    500.,
				    1000.};



  vector<double> HistBins ;
  if(EtaBinTag == "InvPtBB1") HistBins = InputBins_BB1;
  if(EtaBinTag == "InvPtBB2") HistBins = InputBins_BB1;
  if(EtaBinTag == "InvPt2BB2") HistBins = InputBins_BB2;
  if(EtaBinTag == "InvPtEC1") HistBins = InputBins_EC1;
  if(EtaBinTag == "InvPtEC2") HistBins = InputBins_EC2;
  if(EtaBinTag == "InvPt") HistBins = InputBins_BB1;
  if(EtaBinTag == "Pt") HistBins = InputBins_Pt;
  eta = fabs(eta);
  if(eta>=2.5) eta = 2.49;
  
  //// Lowest pt value is 20;                                                                                                                                                                

  if(pt < 15) pt = 15;
  if(EtaBinTag == "InvPtBB1" || EtaBinTag == "InvPtBB2"){
    if(pt > 650) pt = 650;
  }
  else   if(EtaBinTag == "InvPt2BB2"){
    if(pt > 499.9) pt = 499;
  }
  else if(EtaBinTag == "InvPtEC1") {
    if(pt > 650) pt = 650;
  }
  else if(EtaBinTag == "InvPtEC2") {
    if(pt > 440) pt = 440;
  }
  else {
    if(pt > 900) pt = 899;

  }
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
  double error_MainBin = (mapit->second)->GetBinError(this_bin);


  double value_mBin = (mapit->second)->GetBinContent(this_bin_m1);
  double value_pBin = (mapit->second)->GetBinContent(this_bin_p1);
  double IPt = (key.Contains("Inv")) ?   (1/pt) : pt;  

  if(IPt >= HistBins.at(HistBins.size()-2))  return value_MainBin + double(sys)*error_MainBin;
  if(IPt <= HistBins.at(1)) return value_MainBin + double(sys)*error_MainBin;
  
  vector <double> InputBins = HistBins;

  vector <double> BinCentres;
  double Center_MainBin(0.), Center_mBin(0.), Center_pBin(0.);
  bool corrBinFound=false;
  for(int ib =2; ib < int(InputBins.size())-1; ib++){
    //cout << "InputBins = " << InputBins[ib] << " IPt = " << IPt << endl;
    if(!corrBinFound && (IPt < InputBins[ib]) && (IPt >=InputBins[ib-1]) ) {
      //cout << "Correct bin " << endl;
      corrBinFound=true;
      Center_MainBin = (InputBins[ib] + InputBins[ib-1]) /2.;
      Center_mBin    = (InputBins[ib-1] + InputBins[ib-2]) /2.;
      Center_pBin    = (InputBins[ib+1] + InputBins[ib]) /2.;
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

  ////Based on MC closure apply corr

  ///// Look to apply MC corr  
  double Corr =1.;
  if(GetEra() =="2018") {
    if(pt > 400) Corr  = 0.8;
  }    
  if(GetEra() =="2017") {
    if(pt > 400 && pt < 500) Corr  = 0.8;
  }
  if(GetEra() =="2016postVFP") {
    if(pt > 250) Corr = 0.8;
  }
  if(GetEra() =="2016preVFP") {
    if(pt > 500) Corr =0.78;
  }

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
    return CFRate+ double(sys)*error_MainBin;
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
    
    return CFRate+ double(sys)*error_MainBin;
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




