#ifndef GenFinderForHNWRSignal_h
#define GenFinderForHNWRSignal_h

#include "TString.h"
#include <iostream>
#include "Gen.h"

using namespace std;

class GenFinderForHNWRSignal{

public:

  GenFinderForHNWRSignal();
  ~GenFinderForHNWRSignal();

  void Clear();
  void Run(vector<Gen> gens);
  void Print();
  void PrintGen(std::vector<Gen> gens);
  vector<int> TrackGenSelfHistory(Gen me, std::vector<Gen> gens);

  bool Debug;
  Gen WR, N, priLep, secLep, jet1, jet2;
  int N_PID, SignalLeptonChannel;
  bool GenAllFound, IsOffShellPrimaryWR;


};

#endif
