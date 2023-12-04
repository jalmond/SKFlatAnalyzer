#include "SkimTree_FakeEventSkim.h"

void SkimTree_FakeEventSkim::initializeAnalyzer(){

  AnalyzerCore::initializeAnalyzer();

  outfile->cd();
  cout << "[SkimTree_FakeEventSkim::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);
 

}

void SkimTree_FakeEventSkim::executeEvent(){


  std::vector<Muon>     muonPreColl     = GetMuons("HNLoosest", 5., 2.4);
  std::vector<Electron> electronPreColl = GetElectrons("HNLoosest", 10., 2.5);
  
  bool HasFakeLep=false;

  for(auto iel: electronPreColl){
    if(iel.IsFake()) HasFakeLep=true;
  }
  for(auto imu: muonPreColl){
    if(imu.IsFake()) HasFakeLep=true;
  }
  
  if(MCSample.Contains("Sherpa")) {
    HasFakeLep=false;
    std::vector<Lepton *> leps = MakeLeptonPointerVector(muonPreColl,electronPreColl);
    
    for(auto ilep : leps){
      bool LepPrompt=false;
      for(unsigned int i=2; i<All_Gens.size(); i++){
	Gen gen = All_Gens.at(i);
	
	if(gen.Status() != 1) continue;
	int mindex = All_Gens.at(i).MotherIndex();
	int MotherPID = fabs(All_Gens.at(mindex).PID());
	bool PromptLepMu = (MotherPID == 13)  || (MotherPID == 15);
	bool PromptLepEl = (MotherPID == 11) || (MotherPID == 15);
	
	while (MotherPID > 10 && MotherPID < 16){
	  mindex = All_Gens.at(mindex).MotherIndex();
	  MotherPID= fabs(All_Gens.at(mindex).PID());
	}
	
	bool PromptLepMuFull = PromptLepMu &&  ((MotherPID == 21)  || (MotherPID < 6));
	bool PromptLepElFull = PromptLepEl &&  ((MotherPID == 21)  || (MotherPID < 6));
	
	if(fabs(gen.PID()) == 13){
	  if(PromptLepMuFull && ilep->LeptonFlavour() == Lepton::MUON && ilep->DeltaR(gen) < 0.1) LepPrompt=true;
	}
	if(fabs(gen.PID()) == 11){
	  if(PromptLepElFull &&ilep->LeptonFlavour() != Lepton::MUON && ilep->DeltaR(gen) < 0.1) LepPrompt=true;
	}
      }
      if (!LepPrompt) HasFakeLep=true;
    }
  }

  if(!HasFakeLep) return;
  newtree->Fill();
  //if(newtree->Fill()<0) exit(EIO);

  return;
}

void SkimTree_FakeEventSkim::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_FakeEventSkim::SkimTree_FakeEventSkim(){
  newtree=NULL;
}

SkimTree_FakeEventSkim::~SkimTree_FakeEventSkim(){

}

void SkimTree_FakeEventSkim::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
