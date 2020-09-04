#ifndef CheckSkim_h
#define CheckSkim_h

#include "AnalyzerCore.h"

class CheckSkim : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEvent();

  vector<TString> triggers;

};



#endif

