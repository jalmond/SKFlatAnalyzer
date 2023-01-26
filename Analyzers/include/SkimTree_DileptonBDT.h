#ifndef SkimTree_DileptonBDT_h
#define SkimTree_DileptonBDT_h

#include "AnalyzerCore.h"

class SkimTree_DileptonBDT : public AnalyzerCore {

 public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_DileptonBDT();
  ~SkimTree_DileptonBDT();

  TTree *newtree;

  
  float vSKWeight;
  vector<float>* velectron_ptratio;
  vector<float>* velectron_ptrel;
  vector<float>* velectron_cj_bjetdisc;
  vector<float>* velectron_cj_flavour;
  vector<float>* velectron_mva_cf;
  vector<float>* velectron_mva_conv;
  vector<float>* velectron_mva_fake;

  vector<float>* vmuon_mva_fake;
  vector<float>* vmuon_ptrel;
  vector<float>* vmuon_ptratio;
  vector<float>* vmuon_cj_bjetdisc;
  vector<float>* vmuon_cj_flavour;



  vector<TString> double_triggers;
  vector<TString> single_muon_triggers;
  vector<TString> single_electron_triggers;
  void WriteHist();


};



#endif
