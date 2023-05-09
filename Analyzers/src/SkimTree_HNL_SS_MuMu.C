#include "SkimTree_HNL_SS_MuMu.h"

void SkimTree_HNL_SS_MuMu::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNL_SS_MuMu::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);


}

void SkimTree_HNL_SS_MuMu::executeEvent(){

  Event ev;

  TString process = GetProcess();
  
  if (SelectChannel(MuMu)){
    
    newtree->Fill();
    return;
  }

}

void SkimTree_HNL_SS_MuMu::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNL_SS_MuMu::SkimTree_HNL_SS_MuMu(){
  newtree=NULL;
}

SkimTree_HNL_SS_MuMu::~SkimTree_HNL_SS_MuMu(){

}

void SkimTree_HNL_SS_MuMu::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
