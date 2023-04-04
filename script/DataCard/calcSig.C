// ********************** Before running this script, make sure you put proper fake, CF path *************************** //

void calcSig2(TString channel, TString signal, TString mass, TString datacard){ //calcSig("dimu", "SSWWTypeI_NLO_SF_M1500", "M1500", "y")
  TString hist_mass;
  TString mass_number = mass(1,100);
  if(mass_number.Atof()>2000.) hist_mass = "M2500"; //JH : FIXME !!HOTFIX!! I don't have optimization M(N) > 2500 GeV
  else hist_mass = mass; //JH : FIXME !!HOTFIX!! I don't have optimization M(N) > 2500 GeV
  /////////////////////////////////////MC////////////////////////////////////////////
  string fileline;
  ifstream in("fileList_calcSig.txt");
  double MC_SR1 = 0., MC_SR1_opt = 0., MC_SR2 = 0., MC_SR2_opt = 0., MC_SR3 = 0., MC_SR3_opt = 0.;
  double signal_SR1 = 0., signal_SR1_opt = 0., signal_SR2 = 0., signal_SR2_opt = 0., signal_SR3 = 0., signal_SR3_opt = 0.;
  double fake_SR1 = 0., fake_SR1_opt = 0., fake_SR2 = 0., fake_SR2_opt = 0., fake_SR3 = 0., fake_SR3_opt = 0.;
  double CF_SR1 = 0., CF_SR1_opt = 0., CF_SR2 = 0., CF_SR2_opt = 0., CF_SR3 = 0., CF_SR3_opt = 0.;
  double bkg_SR1 = 0., bkg_SR1_opt = 0., bkg_SR2 = 0., bkg_SR2_opt = 0., bkg_SR3 = 0., bkg_SR3_opt = 0.;
  // Line loop
  while(getline(in, fileline)){
    std::istringstream is(fileline);
    TString this_line = fileline;
    if(this_line.Contains("#")||this_line=="") continue;
    TString category, process, isSkim;
    is >> category;
    is >> process;
    is >> isSkim;
    //cout << "process : " << process << endl;
    //cout << "isSkim : " << isSkim << endl;

    if(category=="signal"){
      if(process!=signal) continue;
    }

    TFile *f_MC;
    TH1D *h_MC_SR1, *h_MC_SR1_opt, *h_MC_SR2, *h_MC_SR2_opt, *h_MC_SR3, *h_MC_SR3_opt;
    if(isSkim=="skimmed") f_MC = new TFile("HNType1_SkimTree_HNMultiLep_"+process+".root");
    else f_MC = new TFile("HNType1_"+process+".root");
    //cout << "f_MC : " << f_MC << endl;
    h_MC_SR1     = (TH1D*)f_MC->Get(channel+"/SR1_Central/HToverPt1_HN");
    h_MC_SR1_opt = (TH1D*)f_MC->Get(channel+"/SR1_Central/"+hist_mass+"/HToverPt1_HN");
    h_MC_SR2     = (TH1D*)f_MC->Get(channel+"/SR2_Central/HToverPt1_HN");
    h_MC_SR2_opt = (TH1D*)f_MC->Get(channel+"/SR2_Central/"+hist_mass+"/HToverPt1_HN");
    h_MC_SR3     = (TH1D*)f_MC->Get(channel+"/SR3_Central/HToverPt1_HN");
    h_MC_SR3_opt = (TH1D*)f_MC->Get(channel+"/SR3_Central/"+hist_mass+"/HToverPt1_HN");
    int Nbin = 20; // Nbin of HToverPt1
    cout.flags(ios::left); // left align; see https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=tipsware&logNo=221115657586
    cout << "========================================================" << endl;
    cout << "!!process : " << process << "!!" << endl;
    if(h_MC_SR1){
                                cout << "SR1                                : " << h_MC_SR1->Integral(0,Nbin+1) << endl;
                                if(category=="MC")     MC_SR1     += h_MC_SR1->Integral(0,Nbin+1);
                                if(category=="signal") signal_SR1 += h_MC_SR1->Integral(0,Nbin+1);
    }
    else                        cout << "SR1                                : NULL;" << endl;
    if(h_MC_SR1_opt){
                                cout << "SR1, with "; cout.width(6); cout << mass; cout << " optimization      : " << h_MC_SR1_opt->Integral(0,Nbin+1) << endl;
                                if(category=="MC")         MC_SR1_opt += h_MC_SR1_opt->Integral(0,Nbin+1);
                                if(category=="signal") signal_SR1_opt += h_MC_SR1_opt->Integral(0,Nbin+1);
    }
    else{                        
                                cout << "SR1, with "; cout.width(6); cout << mass; cout << " optimization      : NULL;" << endl;
    }
    if(h_MC_SR2){
                                cout << "SR2                                : " << h_MC_SR2->Integral(0,Nbin+1) << endl;
                                if(category=="MC")         MC_SR2 += h_MC_SR2->Integral(0,Nbin+1);
                                if(category=="signal") signal_SR2 += h_MC_SR2->Integral(0,Nbin+1);
    }
    else                        cout << "SR2                                : NULL;" << endl;
    if(h_MC_SR2_opt){
                                cout << "SR2, with "; cout.width(6); cout << mass; cout << " optimization      : " << h_MC_SR2_opt->Integral(0,Nbin+1) << endl;
                                if(category=="MC")         MC_SR2_opt += h_MC_SR2_opt->Integral(0,Nbin+1);
                                if(category=="signal") signal_SR2_opt += h_MC_SR2_opt->Integral(0,Nbin+1);
    }
    else{                       
                                cout << "SR2, with "; cout.width(6); cout << mass; cout << " optimization      : NULL;" << endl;
    }
    if(h_MC_SR3){
                                cout << "SR3                                : " << h_MC_SR3->Integral(0,Nbin+1) << endl;
                                if(category=="MC")         MC_SR3 += h_MC_SR3->Integral(0,Nbin+1);
                                if(category=="signal") signal_SR3 += h_MC_SR3->Integral(0,Nbin+1);
    }
    else                        cout << "SR3                                : NULL;" << endl;
    if(h_MC_SR3_opt){
                                cout << "SR3, with "; cout.width(6); cout << mass; cout << " optimization      : " << h_MC_SR3_opt->Integral(0,Nbin+1) << endl;
                                if(category=="MC")         MC_SR3_opt += h_MC_SR3_opt->Integral(0,Nbin+1);
                                if(category=="signal") signal_SR3_opt += h_MC_SR3_opt->Integral(0,Nbin+1);
    }
    else{                        
                                cout << "SR3, with "; cout.width(6); cout << mass; cout << " optmization       : NULL;" << endl;
    }
  }
  cout << "===================================================================================================" << endl;
  cout << "total MC in SR1                              : " << MC_SR1 << endl;
  cout << "total MC in SR1, with "; cout.width(6); cout << mass; cout << "optimization     : " << MC_SR1_opt << endl;
  cout << "total MC in SR2                              : " << MC_SR2 << endl;
  cout << "total MC in SR2, with "; cout.width(6); cout << mass; cout << "optimization     : " << MC_SR2_opt << endl;
  cout << "total MC in SR3                              : " << MC_SR3 << endl;
  cout << "total MC in SR3, with "; cout.width(6); cout << mass; cout << "optimization     : " << MC_SR3_opt << endl;

  /////////////////////////////////////Fake////////////////////////////////////////////
  TFile *f_Fake;
  if(channel=="dimu") f_Fake = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v2/HNType1/2016preVFP/RunFake__/DATA/HNType1_SkimTree_Dilepton_DoubleMuon.root");
  if(channel=="diel") f_Fake = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v2/HNType1/2016preVFP/RunFake__/DATA/HNType1_SkimTree_Dilepton_DoubleEG.root");
  TH1D *h_Fake_SR1, *h_Fake_SR1_opt, *h_Fake_SR2, *h_Fake_SR2_opt, *h_Fake_SR3, *h_Fake_SR3_opt;
  h_Fake_SR1     = (TH1D*)f_Fake->Get(channel+"/SR1_Central/HToverPt1_HN");
  h_Fake_SR1_opt = (TH1D*)f_Fake->Get(channel+"/SR1_Central/"+hist_mass+"/HToverPt1_HN");
  h_Fake_SR2     = (TH1D*)f_Fake->Get(channel+"/SR2_Central/HToverPt1_HN");
  h_Fake_SR2_opt = (TH1D*)f_Fake->Get(channel+"/SR2_Central/"+hist_mass+"/HToverPt1_HN");
  h_Fake_SR3     = (TH1D*)f_Fake->Get(channel+"/SR3_Central/HToverPt1_HN");
  h_Fake_SR3_opt = (TH1D*)f_Fake->Get(channel+"/SR3_Central/"+hist_mass+"/HToverPt1_HN");
  int Nbin = 20; // Nbin of HToverPt1
  cout << "===================================================================================================" << endl;
  if(h_Fake_SR1){
                              cout << "fake in SR1                                  : " << h_Fake_SR1->Integral(0,Nbin+1) << endl;
                              fake_SR1 += h_Fake_SR1->Integral(0,Nbin+1);
  }
  else                        cout << "fake in SR1                                  : NULL;" << endl;
  if(h_Fake_SR1_opt){
                              cout << "fake in SR1, with "; cout.width(6); cout << mass; cout << " optimization        : " << h_Fake_SR1_opt->Integral(0,Nbin+1) << endl;
                              fake_SR1_opt += h_Fake_SR1_opt->Integral(0,Nbin+1);
  }
  else{
                              cout << "fake in SR1, with "; cout.width(6); cout << mass; cout << " optimization        : NULL;" << endl;
  }
  if(h_Fake_SR2){
                              cout << "fake in SR2                                  : " << h_Fake_SR2->Integral(0,Nbin+1) << endl;
                              fake_SR2 += h_Fake_SR2->Integral(0,Nbin+1);
  }
  else                        cout << "fake in SR2                                  : NULL;" << endl;
  if(h_Fake_SR2_opt){
                              cout << "fake in SR2, with "; cout.width(6); cout << mass; cout << " optimization        : " << h_Fake_SR2_opt->Integral(0,Nbin+1) << endl;
                              fake_SR2_opt += h_Fake_SR2_opt->Integral(0,Nbin+1);
  }
  else{
                              cout << "fake in SR2, with "; cout.width(6); cout << mass; cout << " optimization        : NULL;" << endl;
  }
  if(h_Fake_SR3){
                              cout << "fake in SR3                                  : " << h_Fake_SR3->Integral(0,Nbin+1) << endl;
                              fake_SR3 += h_Fake_SR3->Integral(0,Nbin+1);
  }
  else                        cout << "fake in SR3                                  : NULL;" << endl;
  if(h_Fake_SR3_opt){
                              cout << "fake in SR3, with "; cout.width(6); cout << mass; cout << " optimization        : " << h_Fake_SR3_opt->Integral(0,Nbin+1) << endl;
                              fake_SR3_opt += h_Fake_SR3_opt->Integral(0,Nbin+1);
  }
  else{
                              cout << "fake in SR3, with "; cout.width(6); cout << mass; cout << " optimization        : NULL;" << endl;
  }

  /////////////////////////////////////CF////////////////////////////////////////////
  if(channel=="diel"){
    TFile *f_CF = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v2/HNType1/2016preVFP/RunCF__/DATA/HNType1_SkimTree_Dilepton_DoubleEG.root");
    TH1D *h_CF_SR1, *h_CF_SR1_opt, *h_CF_SR2, *h_CF_SR2_opt, *h_CF_SR3, *h_CF_SR3_opt;
    h_CF_SR1     = (TH1D*)f_CF->Get(channel+"/SR1_Central/HToverPt1_HN");
    h_CF_SR1_opt = (TH1D*)f_CF->Get(channel+"/SR1_Central/"+hist_mass+"/HToverPt1_HN");
    h_CF_SR2     = (TH1D*)f_CF->Get(channel+"/SR2_Central/HToverPt1_HN");
    h_CF_SR2_opt = (TH1D*)f_CF->Get(channel+"/SR2_Central/"+hist_mass+"/HToverPt1_HN");
    h_CF_SR3     = (TH1D*)f_CF->Get(channel+"/SR3_Central/HToverPt1_HN");
    h_CF_SR3_opt = (TH1D*)f_CF->Get(channel+"/SR3_Central/"+hist_mass+"/HToverPt1_HN");
    int Nbin = 20; // Nbin of HToverPt1
    cout << "===================================================================================================" << endl;
    if(h_CF_SR1){
                                cout << "CF in SR1                                  : " << h_CF_SR1->Integral(0,Nbin+1) << endl;
                                CF_SR1 += h_CF_SR1->Integral(0,Nbin+1);
    }
    else                        cout << "CF in SR1                                  : NULL;" << endl;
    if(h_CF_SR1_opt){
                                cout << "CF in SR1, with "; cout.width(6); cout << mass; cout << " optimization        : " << h_CF_SR1_opt->Integral(0,Nbin+1) << endl;
                                CF_SR1_opt += h_CF_SR1_opt->Integral(0,Nbin+1);
    }
    else{
                                cout << "CF in SR1, with "; cout.width(6); cout << mass; cout << " optimization        : NULL;" << endl;
    }
    if(h_CF_SR2){
                                cout << "CF in SR2                                  : " << h_CF_SR2->Integral(0,Nbin+1) << endl;
                                CF_SR2 += h_CF_SR2->Integral(0,Nbin+1);
    }
    else                        cout << "CF in SR2                                  : NULL;" << endl;
    if(h_CF_SR2_opt){
                                cout << "CF in SR2, with "; cout.width(6); cout << mass; cout << " optimization        : " << h_CF_SR2_opt->Integral(0,Nbin+1) << endl;
                                CF_SR2_opt += h_CF_SR2_opt->Integral(0,Nbin+1);
    }
    else{
                                cout << "CF in SR2, with "; cout.width(6); cout << mass; cout << " optimization        : NULL;" << endl;
    }
    if(h_CF_SR3){
                                cout << "CF in SR3                                  : " << h_CF_SR3->Integral(0,Nbin+1) << endl;
                                CF_SR3 += h_CF_SR3->Integral(0,Nbin+1);
    }
    else                        cout << "CF in SR3                                  : NULL;" << endl;
    if(h_CF_SR3_opt){
                                cout << "CF in SR3, with "; cout.width(6); cout << mass; cout << " optimization        : " << h_CF_SR3_opt->Integral(0,Nbin+1) << endl;
                                CF_SR3_opt += h_CF_SR3_opt->Integral(0,Nbin+1);
    }
    else{
                                cout << "CF in SR3, with "; cout.width(6); cout << mass; cout << " optimization        : NULL;" << endl;
    }
  }

  bkg_SR1 = MC_SR1+fake_SR1+CF_SR1;
  bkg_SR1_opt = MC_SR1_opt+fake_SR1_opt+CF_SR1_opt;
  bkg_SR2 = MC_SR2+fake_SR2+CF_SR2;
  bkg_SR2_opt = MC_SR2_opt+fake_SR2_opt+CF_SR2_opt;
  bkg_SR3 = MC_SR3+fake_SR3+CF_SR3;
  bkg_SR3_opt = MC_SR3_opt+fake_SR3_opt+CF_SR3_opt;

  ///////////////////////////////////signal normalization//////////////////////////////////////////
  //if(signal.Contains("DY")||signal.Contains("VBF")){
  //  signal_SR1              *= 10000.;
  //  signal_SR1_opt          *= 10000.;
  //  signal_SR2              *= 10000.;
  //  signal_SR2_opt          *= 10000.;
  //  signal_SR3              *= 10000.;
  //  signal_SR3_opt          *= 10000.;
  //} //JH : we don't need this for UL. because the xsec in sample info were already normalized to V==1.

  cout << "===================================================================================================" << endl;
  cout << "signal " << signal << " in SR1         : ";
  cout.width(12); cout << signal_SR1 << "--> S/sqrt(B) = ";
  cout.width(12); cout << signal_SR1/sqrt(bkg_SR1);
  cout << ", full expansion = " << sqrt( 2.*((signal_SR1+bkg_SR1)*log(1+(signal_SR1/(bkg_SR1))) - signal_SR1) ) << endl;
  cout << "signal " << signal << " in SR1_"; cout.width(6); cout << mass; cout << "  : ";
  cout.width(12); cout << signal_SR1_opt << "--> S/sqrt(B) = ";
  cout.width(12); cout << signal_SR1_opt/sqrt(bkg_SR1_opt);
  cout << ", full expansion = " << sqrt( 2.*((signal_SR1_opt+bkg_SR1_opt)*log(1+(signal_SR1_opt/(bkg_SR1_opt))) - signal_SR1_opt) ) << endl;
  cout << "signal " << signal << " in SR2         : ";
  cout.width(12); cout << signal_SR2 << "--> S/sqrt(B) = ";
  cout.width(12); cout << signal_SR2/sqrt(bkg_SR2);
  cout << ", full expansion = " << sqrt( 2.*((signal_SR2+bkg_SR2)*log(1+(signal_SR2/(bkg_SR2))) - signal_SR2) ) << endl;
  cout << "signal " << signal << " in SR2_"; cout.width(6); cout << mass; cout << "  : ";
  cout.width(12); cout << signal_SR2_opt << "--> S/sqrt(B) = ";
  cout.width(12); cout << signal_SR2_opt/sqrt(bkg_SR2_opt);
  cout << ", full expansion = " << sqrt( 2.*((signal_SR2_opt+bkg_SR2_opt)*log(1+(signal_SR2_opt/(bkg_SR2_opt))) - signal_SR2_opt) ) << endl;
  cout << "signal " << signal << " in SR3         : ";
  cout.width(12); cout << signal_SR3 << "--> S/sqrt(B) = ";
  cout.width(12); cout << signal_SR3/sqrt(bkg_SR3);
  cout << ", full expansion = " << sqrt( 2.*((signal_SR3+bkg_SR3)*log(1+(signal_SR3/(bkg_SR3))) - signal_SR3) ) << endl;
  cout << "signal " << signal << " in SR3_"; cout.width(6); cout << mass; cout << "  : ";
  cout.width(12); cout << signal_SR3_opt << "--> S/sqrt(B) = ";
  cout.width(12); cout << signal_SR3_opt/sqrt(bkg_SR3_opt);
  cout << ", full expansion = " << sqrt( 2.*((signal_SR3_opt+bkg_SR3_opt)*log(1+(signal_SR3_opt/(bkg_SR3_opt))) - signal_SR3_opt) ) << endl;

  if(datacard=="y"){

    //signal scaling
    signal_SR1_opt *= 0.01;
    signal_SR2_opt *= 0.01;
    signal_SR3_opt *= 0.01;

    vector<double> card_items = {signal_SR1_opt, MC_SR1_opt, CF_SR1_opt, fake_SR1_opt, signal_SR2_opt, MC_SR2_opt, CF_SR2_opt, fake_SR2_opt, signal_SR3_opt, MC_SR3_opt, CF_SR3_opt, fake_SR3_opt};
    vector<TString> card_items_txt;
    for(int i=0; i<card_items.size(); i++) card_items_txt.push_back(Form("%f",card_items.at(i)));

    ofstream out("datacard/Card_2016preVFP_"+channel+"_"+mass+"_HNTightV1.txt");

    string fileline;
    ifstream in;
    if(mass=="M100") in.open("skeleton/datacard_"+channel+"_lowmass.txt");
    else in.open("skeleton/datacard_"+channel+"_highmass.txt");
    while(getline(in, fileline)){
      TString this_line = fileline;
      if(this_line.Contains("#")){
        this_line = "rate                               ";
        for(int i=0; i<card_items.size(); i++){
          this_line += card_items_txt.at(i);
          this_line += "  ";
        }
      }
      out << this_line << endl;
    }
    out.close();

  }

  //cout << "===================================================================================================" << endl;
  //cout << "put the line below into the data card;" << endl;
  //cout << signal_SR1_opt << "  " << MC_SR1_opt << "  " << CF_SR1_opt << "  " << fake_SR1_opt << "  " << signal_SR2_opt << "  " << MC_SR2_opt << "  " << CF_SR2_opt << "  " << fake_SR2_opt << "  " << signal_SR3_opt << "  " << MC_SR3_opt << "  " << CF_SR3_opt << "  " << fake_SR3_opt << endl;
}

void calcSig(TString channel, TString signal, TString region, TString details){ //calcSig("dimu", "SSWWTypeI_NLO_SF_M1500", "SR3_Central", "y")
  /////////////////////////////////////MC////////////////////////////////////////////
  string fileline;
  ifstream in("fileList_calcSig.txt");
  double MC = 0., Nsignal = 0., fake = 0., CF = 0., bkg = 0.;

  if(details == "y"){
    cout << "********** "+region+" **********" << endl;
    cout << "===============================================" << endl;
  }
  // Line loop
  while(getline(in, fileline)){
    std::istringstream is(fileline);
    TString this_line = fileline;
    if(this_line.Contains("#")||this_line=="") continue;
    TString category, process, isSkim;
    is >> category;
    is >> process;
    is >> isSkim;
    //cout << "process : " << process << endl;
    //cout << "isSkim : " << isSkim << endl;

    if(category=="signal"){
      if(process!=signal) continue;
    }

    TFile *f_MC;
    TH1D *h_MC;
    if(isSkim=="skimmed") f_MC = new TFile("HNType1_SkimTree_HNMultiLep_"+process+".root");
    else f_MC = new TFile("HNType1_"+process+".root");
    //cout << "f_MC : " << f_MC << endl;
    h_MC = (TH1D*)f_MC->Get(channel+"/"+region+"/HToverPt1_HN");
    int Nbin = 20; // Nbin of HToverPt1
    cout.flags(ios::left); // left align; see https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=tipsware&logNo=221115657586
    if(h_MC){
      if(category=="MC")     MC      += h_MC->Integral(0,Nbin+1);
      if(category=="signal") Nsignal += h_MC->Integral(0,Nbin+1);
      if(details == "y"){
        cout.width(27); cout << process; cout << " : "; cout << h_MC->Integral(0,Nbin+1) << endl;
      }
    }
    else{
      if(details == "y"){
        cout.width(27); cout << process; cout << " : NULL;" << endl;
      }
    }
    delete f_MC;
  }
  if(details == "y"){
    cout << "===============================================" << endl;
    cout.width(27); cout << "total MC"; cout << " : " << MC << endl;
  }

  /////////////////////////////////////Fake////////////////////////////////////////////
  TFile *f_Fake;
  if(channel=="dimu") f_Fake = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v2/HNType1/2016preVFP/RunFake__/DATA/HNType1_SkimTree_Dilepton_DoubleMuon.root");
  if(channel=="diel") f_Fake = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v2/HNType1/2016preVFP/RunFake__/DATA/HNType1_SkimTree_Dilepton_DoubleEG.root");
  TH1D *h_Fake;
  h_Fake = (TH1D*)f_Fake->Get(channel+"/"+region+"/HToverPt1_HN");
  int Nbin = 20; // Nbin of HToverPt1
  if(h_Fake){
    fake += h_Fake->Integral(0,Nbin+1);
    if(details == "y"){
      cout.width(27); cout << "fake"; cout << " : " << h_Fake->Integral(0,Nbin+1) << endl;
    }
  }
  else{
    if(details == "y"){
      cout.width(27); cout << "fake"; cout << " : NULL;" << endl;
    }
  }
  delete f_Fake;

  /////////////////////////////////////CF////////////////////////////////////////////
  if(channel=="diel"){
    TFile *f_CF = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v2/HNType1/2016preVFP/RunCF__/DATA/HNType1_SkimTree_Dilepton_DoubleEG.root");
    TH1D *h_CF;
    h_CF = (TH1D*)f_CF->Get(channel+"/"+region+"/HToverPt1_HN");
    int Nbin = 20; // Nbin of HToverPt1
    if(h_CF){
      CF += h_CF->Integral(0,Nbin+1);
      if(details == "y"){
        cout.width(27); cout << "CF"; cout << " : " << h_CF->Integral(0,Nbin+1) << endl;
      }
    }
    else{
      if(details == "y"){
        cout.width(27); cout << "CF"; cout << " : NULL;" << endl;
      }
    }
    delete f_CF;
  }

  bkg = MC+fake+CF;
  if(details == "y"){
    cout << "===============================================" << endl;
  }

  cout.width(27); cout << "signal "+signal; cout.width(30); cout << " in "+region; cout << " : " << Nsignal << " --> S/sqrt(B) = ";
  //cout.width(12); cout << Nsignal << "--> S/sqrt(S+B) = ";
  cout.width(12); cout << Nsignal/sqrt(bkg);
  //cout.width(12); cout << Nsignal/sqrt(Nsignal+bkg);
  cout << ", full expansion = " << sqrt( 2.*((Nsignal+bkg)*log(1+(Nsignal/(bkg))) - Nsignal) ) << endl;
	
}

void compareSig(TString channel){
  calcSig(channel, "DYTypeI_NLO_SF_M100", "SR3_Central", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M100", "SR3_forward", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M100", "SR3_forward_mW", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M100", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M100", "SR3_forward_mW_HToverPt0p5", "n");
  cout << "===============================================" << endl;
  calcSig(channel, "DYTypeI_NLO_SF_M1500", "SR3_Central", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M1500", "SR3_forward", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M1500", "SR3_forward_mW", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "DYTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt0p5", "n");
  cout << "===============================================" << endl;
  calcSig(channel, "VBFTypeI_NLO_SF_M1500", "SR3_Central", "n");
  calcSig(channel, "VBFTypeI_NLO_SF_M1500", "SR3_forward", "n");
  calcSig(channel, "VBFTypeI_NLO_SF_M1500", "SR3_forward_mW", "n");
  calcSig(channel, "VBFTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "VBFTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt0p5", "n");
  cout << "===============================================" << endl;
  calcSig(channel, "SSWWTypeI_NLO_SF_M1500", "SR3_Central", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M1500", "SR3_forward", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M1500", "SR3_forward_mW", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt0p5", "n");
  cout << "===============================================" << endl;
  calcSig(channel, "HNTypeI_NLO_SF_M500", "SR3_Central", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M500", "SR3_forward", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M500", "SR3_forward_mW", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M500", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M500", "SR3_forward_mW_HToverPt0p5", "n");
  cout << "===============================================" << endl;
  calcSig(channel, "HNTypeI_NLO_SF_M1000", "SR3_Central", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1000", "SR3_forward", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1000", "SR3_forward_mW", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1000", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1000", "SR3_forward_mW_HToverPt0p5", "n");
  cout << "===============================================" << endl;
  calcSig(channel, "HNTypeI_NLO_SF_M1500", "SR3_Central", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1500", "SR3_forward", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1500", "SR3_forward_mW", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "HNTypeI_NLO_SF_M1500", "SR3_forward_mW_HToverPt0p5", "n");
  cout << "===============================================" << endl;
  calcSig(channel, "SSWWTypeI_NLO_SF_M15000", "SR3_Central", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M15000", "SR3_forward", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M15000", "SR3_forward_mW", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M15000", "SR3_forward_mW_HToverPt1", "n");
  calcSig(channel, "SSWWTypeI_NLO_SF_M15000", "SR3_forward_mW_HToverPt0p5", "n");
}
