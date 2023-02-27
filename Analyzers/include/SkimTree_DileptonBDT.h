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

  
  vector<TString> double_triggers;
  vector<TString> single_muon_triggers;
  vector<TString> single_electron_triggers;
  void WriteHist();


};



#endif
