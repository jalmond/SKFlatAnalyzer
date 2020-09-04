#ifndef SkimTree_SSNonIso_h
#define SkimTree_SSNonIso_h

#include "AnalyzerCore.h"

class SkimTree_SSNonIso : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_SSNonIso();
  ~SkimTree_SSNonIso();

  TTree *newtree;

  vector<TString> triggers;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;

};



#endif

