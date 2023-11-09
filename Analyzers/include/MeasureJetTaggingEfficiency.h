#ifndef MeasureJetTaggingEfficiency_h
#define MeasureJetTaggingEfficiency_h

#include "HNL_LeptonCore.h"


class MeasureJetTaggingEfficiency : public HNL_LeptonCore {

public:

  void initializeAnalyzer();

  void executeEvent();

  MeasureJetTaggingEfficiency();
  ~MeasureJetTaggingEfficiency();

  //==== Read what to measrue from data/Run2Legacy_v4/<Year>/BTag/taggermap.txt
  vector<string> Taggers;
  vector<string> WPs;
  vector<double> CutValues;

};



#endif

