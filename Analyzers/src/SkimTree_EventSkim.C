#include "SkimTree_EventSkim.h"

void SkimTree_EventSkim::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_EventSkim::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);


}

void SkimTree_EventSkim::executeEvent(){


  std::vector<Muon>     muonPreColl     = GetMuons("HNLoosest", 10., 2.4);
  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 10., 2.5);
  
  bool HasFakeLep=false;

  vector<Gen>   gens= GetGens();  
  for(auto iel: electronPreColl){
    if(iel.IsFake(GetLeptonType_JH(iel, gens))) HasFakeLep=true;
  }
  for(auto imu: muonPreColl){
    if(imu.IsFake(GetLeptonType_JH(imu, gens))) HasFakeLep=true;
  }
  
  if(!HasFakeLep) return;
  newtree->Fill();
  //if(newtree->Fill()<0) exit(EIO);

  return;
}

void SkimTree_EventSkim::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_EventSkim::SkimTree_EventSkim(){
  newtree=NULL;
}

SkimTree_EventSkim::~SkimTree_EventSkim(){

}

void SkimTree_EventSkim::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
