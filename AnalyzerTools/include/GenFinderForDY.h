#ifndef GenFinderForDY_h
#define GenFinderForDY_h

#include "TString.h"
#include <iostream>
#include "Gen.h"

using namespace std;

class GenFinderForDY{

public:

  GenFinderForDY();
  ~GenFinderForDY();

  bool FoundGenZ;
  Particle Find(vector<Gen>& gens);
  int LeptonPID;
  bool IsLepton(int pid);

};

#endif
