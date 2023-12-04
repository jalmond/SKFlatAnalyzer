#include "SkimTree_FakeEventSkimBDT.h"

void SkimTree_FakeEventSkimBDT::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_FakeEventSkimBDT::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  HNL_LeptonCore::initializeAnalyzer(false,true);

  InitialiseLeptonBDTSKFlat();
  ///// ELECTRON BRANCHES                                                                                                                                                                                                                                                               
    
      
  if(!fChain->GetBranch("electron_mva_cf_v2")){
    newtree->Branch("electron_ptrel",      &velectron_ptrel);
    newtree->Branch("electron_ptratio",    &velectron_ptratio);
    newtree->Branch("electron_lepton_type",&velectron_lepton_type);
  }

  newtree->Branch("electron_mva_fake_ed_v5",&velectron_mva_fake_ed_v5);
  newtree->Branch("electron_mva_fakeHFB_v5",&velectron_mva_fakeHFB_v5);
  newtree->Branch("electron_mva_fakeHFC_v5",&velectron_mva_fakeHFC_v5);
  newtree->Branch("electron_mva_fakeLF_v5", &velectron_mva_fakeLF_v5);
  newtree->Branch("electron_mva_conv_ed_v5",&velectron_mva_conv_ed_v5);
  newtree->Branch("electron_mva_cf_ed_v5",  &velectron_mva_cf_ed_v5 );
  newtree->Branch("electron_mva_cf_ed_v5pt",&velectron_mva_cf_ed_v5pt );

  newtree->Branch("electron_mva_fake_QCD_LFvsHF_v5",  &velectron_mva_fake_QCD_LFvsHF_v5);
  newtree->Branch("electron_mva_fake_QCD_HFBvsHFC_v5",&velectron_mva_fake_QCD_HFBvsHFC_v5);
  newtree->Branch("electron_mva_fake_QCD_LF1_v5",     &velectron_mva_fake_QCD_LF1_v5);
  newtree->Branch("electron_mva_fake_QCD_LF2_v5",     &velectron_mva_fake_QCD_LF2_v5);
  
  newtree->Branch("electron_v2_cj_bjetdisc",   &velectron_v2_cj_bjetdisc);
  newtree->Branch("electron_v2_cj_cvsbjetdisc",&velectron_v2_cj_cvsbjetdisc);
  newtree->Branch("electron_v2_cj_cvsljetdisc",&velectron_v2_cj_cvsljetdisc);
  newtree->Branch("electron_v2_cj_flavour",    &velectron_v2_cj_flavour);

  //// MUON BRANCH                                                                                                                                                                                               
  
  if(!fChain->GetBranch("muon_mva_fake_ed_v4")){
    newtree->Branch("muon_ptrel",         &vmuon_ptrel);
    newtree->Branch("muon_ptratio",       &vmuon_ptratio);
    newtree->Branch("muon_lepton_type",   &vmuon_lepton_type);
    newtree->Branch("SKWeight", &vSKWeight);
  }

  newtree->Branch("muon_mva_fake_QCD_LFvsHF_v5",  &vmuon_mva_fake_QCD_LFvsHF_v5);
  newtree->Branch("muon_mva_fake_QCD_HFBvsHFC_v5",&vmuon_mva_fake_QCD_HFBvsHFC_v5);
  newtree->Branch("muon_mva_fake_QCD_LF1_v5",     &vmuon_mva_fake_QCD_LF1_v5);
  newtree->Branch("muon_mva_fake_QCD_LF2_v5",     &vmuon_mva_fake_QCD_LF2_v5);

  newtree->Branch("muon_v2_cj_bjetdisc",   &vmuon_v2_cj_bjetdisc);
  newtree->Branch("muon_v2_cj_cvsbjetdisc",&vmuon_v2_cj_cvsbjetdisc);
  newtree->Branch("muon_v2_cj_cvsljetdisc",&vmuon_v2_cj_cvsljetdisc);
  newtree->Branch("muon_v2_cj_flavour",    &vmuon_v2_cj_flavour);

}

void SkimTree_FakeEventSkimBDT::executeEvent(){

  if(!fChain->GetBranch("electron_mva_cf_v2"))  ResetLeptonBDTSKFlat();
  ResetLeptonBDTSKFlatV5();

  vSKWeight=MCweight(true,true);

  if(!fChain->GetBranch("electron_mva_cf_v2"))SetupLeptonBDTSKFlat();
  SetupLeptonBDTSKFlatV5();

  newtree->Fill();
  //if(newtree->Fill()<0) exit(EIO);

  return;
}

void SkimTree_FakeEventSkimBDT::executeEventFromParameter(AnalyzerParameter param){

}

SkimTree_FakeEventSkimBDT::SkimTree_FakeEventSkimBDT(){
  newtree=NULL;
}

SkimTree_FakeEventSkimBDT::~SkimTree_FakeEventSkimBDT(){

}

void SkimTree_FakeEventSkimBDT::WriteHist(){

  outfile->mkdir("recoTree");
  outfile->cd("recoTree");
  newtree->Write();
  outfile->cd();

}
