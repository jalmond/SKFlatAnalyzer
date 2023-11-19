void quickFOM(){
//TFile *f1 = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_TestOldMuID/LimitInputs/HNL_ULID/2017/M100_MuMu_card_input.root");
//TFile *f1 = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_BeforeTestNewOpt/LimitInputs/HNL_ULID/2017/M400_MuMu_card_input.root");
TFile *f1 = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter_BeforeTestNewOpt/LimitInputs/HNL_ULID/2017/M100_MuMu_card_input.root");
//TFile *f1 = new TFile("/data6/Users/jihkim/SKFlatOutput/Run2UltraLegacy_v3/HNL_SignalRegionPlotter/LimitInputs/HNL_ULID/2017/M100_MuMu_card_input.root");
TH1D *h_sig = (TH1D*)f1->Get("signalDYVBF");
TH1D *h_bkg = (TH1D*)f1->Get("data_obs");
double FOM = 0.;
double sig = 0.;
double bkg = 0.;
double this_sig = 0.;
double this_bkg = 0.;
double this_FOM = 0.;
for(int i=1; i<21; i++){
  this_sig = h_sig->GetBinContent(i);
  this_bkg = h_bkg->GetBinContent(i);
  if(this_bkg<=0) this_bkg = 0.001;
  this_FOM = sqrt( 2.*((this_sig+this_bkg)*log(1+(this_sig/(this_bkg))) - this_sig) );
  cout << "=================================" << endl;
  cout << "bin " << i << endl;
  cout << "sig : " << this_sig << endl;
  cout << "bkg : " << this_bkg << endl;
  cout << "S/sqrt(B) : " << this_sig/sqrt(this_bkg) << endl;
  cout << "full expansion : " << this_FOM << endl;
  FOM+=this_FOM;
  sig+=this_sig;
  bkg+=this_bkg;
};
cout << "=================================" << endl;
cout << "total signal (V^2==0.01) : " << sig << endl;
cout << "total bkg : " << bkg << endl;
cout << "total S/sqrt(B) : " << sig/sqrt(bkg) << endl;
cout << "bin by bin FOM : " << FOM << endl;



FOM = 0;
this_FOM = sqrt( 2.*((67.+18.57)*log(1+(67./(18.57))) - 67.) );
FOM+=this_FOM;
this_FOM = sqrt( 2.*((0.3+0.99)*log(1+(0.3/(0.99))) - 0.3) );
FOM+=this_FOM;
cout << "17028 FOM : " << FOM << endl;
}
