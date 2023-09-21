#ifndef HNL_GenStudy_h
#define HNL_GenStudy_h

#include "HNL_LeptonCore.h"


class HNL_GenStudy  : public HNL_LeptonCore {

 public:
  
  void executeEvent();

  HNL_GenStudy();
  ~HNL_GenStudy();

};

#endif
