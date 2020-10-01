#ifndef SkimTree_SSHN_h
#define SkimTree_SSHN_h

#include "AnalyzerCore.h"

class SkimTree_SSHN : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_SSHN();
  ~SkimTree_SSHN();

  TTree *newtree;

  vector<double>  *el_type;
  vector<double>  *mu_type;

  TBranch        *b_el_type;   //!                                                                                                                            
  TBranch        *b_mu_type;   //!                                                                                                                            

  vector<Gen> gens;

  bool isSingleMu;

  vector<TString> triggers;
  vector<TString> triggers_dimu;
  void WriteHist();

  double LeptonPtCut, AK4JetPtCut, AK8JetPtCut;

};



#endif

