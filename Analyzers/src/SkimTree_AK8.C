#include "SkimTree_AK8.h"

void SkimTree_AK8::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_AK8::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);
  

  triggers_dimu.clear();

  triggers.clear();


}

void SkimTree_AK8::executeEvent(){

  std::vector<Muon>     muonPreColl     = GetMuons("HNLoosest", 7., 2.4);
  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 8., 2.5);

  std::sort(muonPreColl.begin(), muonPreColl.end(), PtComparing);
  std::sort(electronPreColl.begin(), electronPreColl.end(), PtComparing);
  
  //int NEl  = electronPreColl.size();
  //int NMu  = muonPreColl.size();
  //int NLep = NEl+NMu;
  
  vector<FatJet> allfatjets = puppiCorr->Correct( GetFatJets("tight", 180., 5.) );
  
  bool HasFatJet = (allfatjets.size() > 0);
  


  std::vector<Lepton *> leps  = MakeLeptonPointerVector(muonPreColl,electronPreColl);
  if (!SameCharge(leps)) return;
  if (!HasFatJet)  return;
  if(leps[0]->LeptonFlavour() == Lepton::ELECTRON && leps[0]->Pt() < 23.) return;
  if(leps[0]->LeptonFlavour() == Lepton::MUON     && leps[0]->Pt() < 17.) return;

  
  //==== If survived, fill tree
  //=============================

  newtree->Fill();

}

void SkimTree_AK8::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_AK8::SkimTree_AK8(){

  newtree = NULL;

}

SkimTree_AK8::~SkimTree_AK8(){

}

void SkimTree_AK8::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
