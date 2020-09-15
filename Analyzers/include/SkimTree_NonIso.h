#ifndef SkimTree_NonIso_h
#define SkimTree_NonIso_h

#include "AnalyzerCore.h"

class SkimTree_NonIso : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_NonIso();
  ~SkimTree_NonIso();

  TTree *newtree;

  bool isSingleMu;

  vector<TString> triggers;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;

};



#endif

