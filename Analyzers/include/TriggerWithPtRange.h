#ifndef TriggerWithPtRange_h
#define TriggerWithPtRange_h

#include <iostream>
#include <vector>
#include "TString.h"

using namespace std;

class TriggerWithPtRange{

public:

  vector<double> PtValues;
  vector<TString> Triggers;
  vector<double> TriggerSafePtCuts;
  bool IsDATA;
  TString DataStream;

  void Validate();

  TString GetTriggerFromPt(double pt);

  TriggerWithPtRange();
  ~TriggerWithPtRange();

};

#endif


