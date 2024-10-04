#ifndef CFBackgroundEstimator_h
#define CFBackgroundEstimator_h

#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"

#include "Lepton.h"
#include "Muon.h"
#include "Electron.h"

#include "AnalyzerParameter.h"
#include "TDirectoryHelper.h"

class CFBackgroundEstimator{

public:

  CFBackgroundEstimator();
  ~CFBackgroundEstimator();

  TDirectory *histDir;
  void ReadHistograms(int loadFormat=0);

  int DataYear;
  TString DataEra;
  void SetEra(TString era){
    DataEra=era;
    DataYear=TString(era(0,4)).Atoi();
  }
  TString GetEra() const { return DataEra; }
  int GetYear() const { return DataYear; } 

  std::map< TString, TH2D* > map_hist_Electron;
  std::map< TString, TH1D* > map_hist_Muon;

  std::map<TString, vector<double> > map_pt_bins;

  bool IgnoreNoHist;

  void SetFittedBins();
  vector<double> FindBin(TString key);

  double GetElectronCFRateFitted(TString ID, TString BinTag, TString key, double eta, double pt, int sys);
  double GetElectronCFRate(TString ID, TString key, double eta, double pt, int sys=0);
  double GetElectronCFRate2D(TString ID, TString key, double eta, double pt, int sys=0);
  double GetMuonCFRate(TString ID, TString key, double eta, double pt, int sys=0);

  double GetWeight(vector<Lepton *> lepptrs, AnalyzerParameter param, int sys=0);

  vector<TString> MissingHists;

};

#endif
