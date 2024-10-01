void checkEntry2(){

  string fileline;
  ifstream in("fileList_checkEntry2.txt");

  // Line loop
  while(getline(in, fileline)){
    std::istringstream is(fileline);
    TString this_line = fileline;
    if(this_line==""){
      cout << endl;
      continue;
    }
    if(this_line.Contains("#")) continue;
    TString filename, histname;
    is >> filename;
    is >> histname;

    cout << "===============================================" << endl;
    cout << filename << ": " << histname << endl;
    TFile *f1;
    f1 = new TFile(filename);
    TH1D *h1;
    h1 = (TH1D*)f1->Get(histname);
    for(unsigned int i=1; i<=h1->GetNbinsX(); i++){
      cout << h1->GetBinContent(i) << endl;
		}
    //TH1D *h_fake, *h_cf, *h_zg, *h_conv_others, *h_wz, *h_zz, *h_ww, *h_prompt_others, *h_signalDYVBF;
    //h_fake = (TH1D*)f1->Get("fake");
    //h_cf   = (TH1D*)f1->Get("cf");
    //h_zg   = (TH1D*)f1->Get("zg");
    //h_conv_others = (TH1D*)f1->Get("conv_others");
    //h_wz = (TH1D*)f1->Get("wz");
    //h_zz = (TH1D*)f1->Get("zz");
    //h_ww = (TH1D*)f1->Get("ww");
    //h_prompt_others = (TH1D*)f1->Get("prompt_others");
    //h_signalDYVBF = (TH1D*)f1->Get("signalDYVBF");
    //cout << filename << endl;
    //cout << h_fake         ->GetBinContent(h_fake         ->GetNbinsX()-1) << endl;
    //cout << h_cf           ->GetBinContent(h_cf           ->GetNbinsX()-1) << endl;
    //cout << h_zg           ->GetBinContent(h_zg           ->GetNbinsX()-1) << endl;
    //cout << h_conv_others  ->GetBinContent(h_conv_others  ->GetNbinsX()-1) << endl;
    //cout << h_wz           ->GetBinContent(h_wz           ->GetNbinsX()-1) << endl;
    //cout << h_zz           ->GetBinContent(h_zz           ->GetNbinsX()-1) << endl;
    //cout << h_ww           ->GetBinContent(h_ww           ->GetNbinsX()-1) << endl;
    //cout << h_prompt_others->GetBinContent(h_prompt_others->GetNbinsX()-1) << endl;
    //cout << h_signalDYVBF  ->GetBinContent(h_signalDYVBF  ->GetNbinsX()-1) << endl;
  }

}
