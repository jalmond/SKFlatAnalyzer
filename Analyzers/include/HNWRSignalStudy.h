#ifndef HNWRSignalStudy_h
#define HNWRSignalStudy_h

#include "AnalyzerCore.h"

class HNWRSignalStudy : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool RunNewPDF;

  HNWRSignalStudy();
  ~HNWRSignalStudy();

};



#endif

