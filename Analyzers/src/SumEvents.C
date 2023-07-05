#include "SumEvents.h"

void SumEvents::initializeAnalyzer(){
  fChain->SetBranchStatus("*",0);
  fChain->SetBranchStatus("IsData",1);


}

void SumEvents::executeEvent(){

  double weight = 1.;

  if(!IsDATA){
    weight = 0.1;
  }

  FillHist("NEvents", 0,       1., 1, 0., 1.);
  FillHist("sumW",    0, weight,   1, 0., 1.);

}
