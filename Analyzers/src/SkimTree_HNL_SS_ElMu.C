#include "SkimTree_HNL_SS_ElMu.h"

void SkimTree_HNL_SS_ElMu::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNL_SS_ElMu::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);


}

void SkimTree_HNL_SS_ElMu::executeEvent(){

  Event ev;

  TString process = GetProcess();

  if (SelectChannel(EMu)){
      
    newtree->Fill();
    return;
  }

}

void SkimTree_HNL_SS_ElMu::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNL_SS_ElMu::SkimTree_HNL_SS_ElMu(){
  newtree=NULL;
}

SkimTree_HNL_SS_ElMu::~SkimTree_HNL_SS_ElMu(){

}

void SkimTree_HNL_SS_ElMu::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
