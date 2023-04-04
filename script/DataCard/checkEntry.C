void checkEntry(TString channel, TString region){

  cout << "===============================================" << endl;
  cout << "<channel : " << channel << ">" << endl << endl;
  cout << "<region : " << region << ">" << endl << endl;
  string fileline;
  ifstream in("fileList_checkEntry.txt");

  // Line loop
  while(getline(in, fileline)){
    std::istringstream is(fileline);
    TString this_line = fileline;
    if(this_line==""){
      cout << endl;
      continue;
    }
    if(this_line.Contains("#")) continue;
    TString category, process, isSkim;
    is >> category;
    is >> process;
    is >> isSkim;

    TFile *f_MC;
    TH1D *h_MC, *h_Nevt;
    if(isSkim=="skimmed") f_MC = new TFile("HNType1_SkimTree_HNMultiLep_"+process+".root");
    else f_MC = new TFile("HNType1_"+process+".root");
    h_MC = (TH1D*)f_MC->Get(channel+"/"+region+"/HToverPt1_HN");
    h_Nevt = (TH1D*)f_MC->Get(channel+"/Pre/Nevents_HN");
    int Nbin;
    double Nevent, Ntot;
    if(h_MC){
      Nbin = h_MC->GetNbinsX();
      Nevent = h_MC->Integral(0,Nbin+1);
      Ntot = h_Nevt->GetBinContent(1);
    }
    else Nevent = 0.;
    
    if(category=="signal") cout << process << " : " << Nevent << " (" << (Nevent/Ntot)*100.*2. << " %)" << endl;
    else cout << process << " : " << Nevent << endl;
  }
  cout << "===============================================" << endl;

}
