#include "SkimTree_ConvEventSkim.h"

void SkimTree_ConvEventSkim::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_ConvEventSkim::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);


}

void SkimTree_ConvEventSkim::executeEvent(){


  std::vector<Muon>     muonPreColl     = GetMuons("HNLoosest", 10., 2.4);
  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 10., 2.5);
  
  bool HasConvLep=false;

  vector<Gen>   gens= GetGens();  
  for(auto iel: electronPreColl){
    if(iel.IsConv(GetLeptonType_JH(iel, gens))) HasConvLep=true;
  }
  for(auto imu: muonPreColl){
    if(imu.IsConv(GetLeptonType_JH(imu, gens))) HasConvLep=true;
  }
  
  if(!HasConvLep) return;
  newtree->Fill();
  //if(newtree->Fill()<0) exit(EIO);

  return;
}

void SkimTree_ConvEventSkim::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_ConvEventSkim::SkimTree_ConvEventSkim(){
  newtree=NULL;
}

SkimTree_ConvEventSkim::~SkimTree_ConvEventSkim(){

}

void SkimTree_ConvEventSkim::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
