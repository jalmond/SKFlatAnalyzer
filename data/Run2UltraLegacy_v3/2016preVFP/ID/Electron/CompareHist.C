void CompareHist(){

  vector<TString> eras = {"2016a","2016b","2017","2018"};
  vector<TString> eras_full = {"2016preVFP","2016postVFP","2017","2018"};
  
  for(int i=0; i<4; i++){
    TFile *f_SF1, *f_SF2;
    TH2D *h_SF1, *h_SF2, *h_SFerr1, *h_SFerr2;

    //f_SF1 = new TFile("/data6/Users/jihkim/SKFlatAnalyzer/data/Run2UltraLegacy_v3/"+eras_full[i]+"/ID/Electron/IDEff_HNL_v1_"+eras[i]+".root");
    //f_SF1 = new TFile("/data6/Users/jihkim/SKFlatAnalyzer/data/Run2UltraLegacy_v3/"+eras_full[i]+"/ID/Electron/TrigEff_Ele23_HNL_ULID_"+eras[i]+".root");
    f_SF1 = new TFile("/data6/Users/jihkim/SKFlatAnalyzer/data/Run2UltraLegacy_v3/"+eras_full[i]+"/ID/Electron/TrigEff_Ele12_HNL_ULID_"+eras[i]+".root");
    //f_SF2 = new TFile("/data6/Users/jihkim/tnp_tamsa/results/HNL_Config_El/HNL_"+eras[i]+"/efficiency.root");
    //f_SF2 = new TFile("/data6/Users/jihkim/tnp_tamsa/results/HNL_Config_El_Trig/Ele23Leg1_HNL_ULID_"+eras[i]+"/efficiency.root");
    f_SF2 = new TFile("/data6/Users/jihkim/tnp_tamsa/results/HNL_Config_El_Trig/Ele12Leg2_HNL_ULID_"+eras[i]+"/efficiency.root");
    h_SF1 = (TH2D*)f_SF1->Get("sf");
    h_SF2 = (TH2D*)f_SF2->Get("sf");
    h_SFerr1 = (TH2D*)f_SF1->Get("sf_sys");
    h_SFerr2 = (TH2D*)f_SF2->Get("sf_sys");

    cout << "Checking " << eras_full[i] << "..." << endl;

    // check bin consistency
    cout << "SF1 GetNbinsX : " << h_SF1->GetNbinsX() << endl;
    cout << "SF2 GetNbinsX : " << h_SF2->GetNbinsX() << endl;
    cout << "SF1 GetNbinsY : " << h_SF1->GetNbinsY() << endl;
    cout << "SF2 GetNbinsY : " << h_SF2->GetNbinsY() << endl;
    //cout << "SF1 GetNbinsZ : " << h_SF1->GetNbinsZ() << endl;
    //cout << "SF2 GetNbinsZ : " << h_SF2->GetNbinsZ() << endl;
    
    for(int j=1; j<=h_SF1->GetNbinsX()*h_SF1->GetNbinsY(); j++){
      cout << "=======================================================" << endl;
      cout << j << "th bin SF1 : " << h_SF1->GetBinContent(j) << endl;
      cout << j << "th bin SF2 : " << h_SF2->GetBinContent(j) << endl;
      cout << j << "th bin SF ratio : " << h_SF1->GetBinContent(j)/h_SF2->GetBinContent(j) << endl;
      cout << j << "th bin stat1 : " << h_SF1->GetBinError(j) << endl;
      cout << j << "th bin stat2 : " << h_SF2->GetBinError(j) << endl;
      cout << j << "th bin stat ratio : " << h_SF1->GetBinError(j)/h_SF2->GetBinError(j) << endl;
      cout << j << "th bin syst1 : " << h_SFerr1->GetBinError(j) << endl;
      cout << j << "th bin syst2 : " << h_SFerr2->GetBinError(j) << endl;
      cout << j << "th bin syst ratio : " << h_SFerr1->GetBinError(j)/h_SFerr2->GetBinError(j) << endl;
    }

    delete f_SF1, f_SF2, h_SF1, h_SF2, h_SFerr1, h_SFerr2;
  }

}
