#ifndef HNL_GenStudy_h
#define HNL_GenStudy_h

#include "HNL_LeptonCore.h"


class HNL_GenStudy  : public HNL_LeptonCore {

 public:
  
  void executeEvent(Long64_t Nentry);

  HNL_GenStudy();
  ~HNL_GenStudy();

};

#endif
