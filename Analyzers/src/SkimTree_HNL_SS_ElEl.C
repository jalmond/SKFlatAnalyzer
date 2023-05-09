#include "SkimTree_HNL_SS_ElEl.h"

void SkimTree_HNL_SS_ElEl::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_HNL_SS_ElEl::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);


}

void SkimTree_HNL_SS_ElEl::executeEvent(){

  Event ev;

  TString process = GetProcess();
  
  if (SelectChannel(EE)){
   
    newtree->Fill();
    return;
  }

}

void SkimTree_HNL_SS_ElEl::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_HNL_SS_ElEl::SkimTree_HNL_SS_ElEl(){
  newtree=NULL;
}

SkimTree_HNL_SS_ElEl::~SkimTree_HNL_SS_ElEl(){

}

void SkimTree_HNL_SS_ElEl::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
