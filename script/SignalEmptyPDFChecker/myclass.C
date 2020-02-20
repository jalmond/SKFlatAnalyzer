#define myclass_cxx
#include "myclass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

bool myclass::Loop()
{
  Long64_t nentries = fChain->GetEntries();
  nentries = 1;
  bool isPDFEmpty = false;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    fChain->GetEntry(jentry);
    double wToCheck = PDFWeights_Scale->at(0);
    if(wToCheck!=wToCheck){
      isPDFEmpty = true;
      break;
    }
  }
  return isPDFEmpty;
}
