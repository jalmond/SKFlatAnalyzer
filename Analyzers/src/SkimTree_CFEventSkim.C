#include "SkimTree_CFEventSkim.h"

void SkimTree_CFEventSkim::initializeAnalyzer(){

  AnalyzerCore::initializeAnalyzer();

  outfile->cd();
  cout << "[SkimTree_CFEventSkim::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);
  
}

void SkimTree_CFEventSkim::executeEvent(){

  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 10., 2.5);
  
  bool HasCFLep=false;

  vector<Gen>   gens= GetGens();  
  for(auto iel: electronPreColl){
    if(iel.LeptonIsCF()) HasCFLep=true;
  }
  
  if(!HasCFLep) return;
  newtree->Fill();
  //if(newtree->Fill()<0) exit(EIO);

  return;
}

void SkimTree_CFEventSkim::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_CFEventSkim::SkimTree_CFEventSkim(){
  newtree=NULL;
}

SkimTree_CFEventSkim::~SkimTree_CFEventSkim(){

}

void SkimTree_CFEventSkim::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
