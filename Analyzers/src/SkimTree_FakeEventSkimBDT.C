#include "SkimTree_FakeEventSkimBDT.h"

void SkimTree_FakeEventSkimBDT::initializeAnalyzer(){

  outfile->cd();
  cout << "[SkimTree_FakeEventSkimBDT::initializeAnalyzer()] gDirectory = " << gDirectory->GetName() << endl;
  newtree = fChain->CloneTree(0);

  SetupIDMVAReaderDefault();
  InitialiseLeptonBDTSKFlat();

  ///// ELECTRON BRANCHES 
  newtree->Branch("electron_mva_cf_v1",&velectron_mva_cf_v1);
  newtree->Branch("electron_mva_cf_v2",&velectron_mva_cf_v2);
  newtree->Branch("electron_mva_cf_v2p1",&velectron_mva_cf_v2p1);
  newtree->Branch("electron_mva_cf_v2p2",&velectron_mva_cf_v2p2);
  newtree->Branch("electron_mva_cf_ed_v2",&velectron_mva_cf_ed_v2);
  newtree->Branch("electron_mva_cf_ed_v2p1",&velectron_mva_cf_ed_v2p1);
  newtree->Branch("electron_mva_cf_ed_v2p2",&velectron_mva_cf_ed_v2p2);
  newtree->Branch("electron_mva_conv_v1",&velectron_mva_conv_v1);
  newtree->Branch("electron_mva_conv_v2",&velectron_mva_conv_v2);
  newtree->Branch("electron_mva_conv_ed_v2",&velectron_mva_conv_ed_v2);

  newtree->Branch("electron_mva_fake_v1",&velectron_mva_fake_v1);
  newtree->Branch("electron_mva_fake_v2",&velectron_mva_fake_v2);
  newtree->Branch("electron_mva_fakeHF_v2",&velectron_mva_fakeHF_v2);
  newtree->Branch("electron_mva_fakeLF_v2",&velectron_mva_fakeLF_v2);
  newtree->Branch("electron_mva_fakeTop_v2",&velectron_mva_fakeTop_v2);

  newtree->Branch("electron_mva_fake_v3",&velectron_mva_fake_v3);
  newtree->Branch("electron_mva_fakeHF_v3",&velectron_mva_fakeHF_v3);
  newtree->Branch("electron_mva_fakeHFB_v3",&velectron_mva_fakeHFB_v3);
  newtree->Branch("electron_mva_fakeHFC_v3",&velectron_mva_fakeHFC_v3);
  newtree->Branch("electron_mva_fakeLF_v3",&velectron_mva_fakeLF_v3);
  newtree->Branch("electron_mva_fakeTop_v3",&velectron_mva_fakeTop_v3);


  newtree->Branch("electron_mva_fake_v4",&velectron_mva_fake_v4);
  newtree->Branch("electron_mva_fakeHF_v4",&velectron_mva_fakeHF_v4);
  newtree->Branch("electron_mva_fakeHFB_v4",&velectron_mva_fakeHFB_v4);
  newtree->Branch("electron_mva_fakeHFC_v4",&velectron_mva_fakeHFC_v4);
  newtree->Branch("electron_mva_fakeLF_v4",&velectron_mva_fakeLF_v4);
  newtree->Branch("electron_mva_fakeTop_v4",&velectron_mva_fakeTop_v4);

  newtree->Branch("electron_mva_fake_ed_v4",&velectron_mva_fake_ed_v4);
  newtree->Branch("electron_mva_fakeHF_ed_v4",&velectron_mva_fakeHF_ed_v4);
  newtree->Branch("electron_mva_fakeHFB_ed_v4",&velectron_mva_fakeHFB_ed_v4);
  newtree->Branch("electron_mva_fakeHFC_ed_v4",&velectron_mva_fakeHFC_ed_v4);
  newtree->Branch("electron_mva_fakeLF_ed_v4",&velectron_mva_fakeLF_ed_v4);
  newtree->Branch("electron_mva_fakeTop_ed_v4",&velectron_mva_fakeTop_ed_v4);

  newtree->Branch("electron_ptrel",&velectron_ptrel);
  newtree->Branch("electron_ptratio",&velectron_ptratio);
  newtree->Branch("electron_cj_bjetdisc",&velectron_cj_bjetdisc);
  newtree->Branch("electron_cj_flavour",&velectron_cj_flavour);
  newtree->Branch("electron_lepton_type",&velectron_lepton_type);
  newtree->Branch("electron_is_cf",   &velectron_is_cf);

  //// MUON BRANCH
  newtree->Branch("muon_mva_fake_v1",&vmuon_mva_fake_v1);
  newtree->Branch("muon_mva_fake_v2",&vmuon_mva_fake_v2);
  newtree->Branch("muon_mva_fake_v3",&vmuon_mva_fake_v3);
  newtree->Branch("muon_mva_fake_v4",&vmuon_mva_fake_v4);
  newtree->Branch("muon_mva_fake_ed_v4",&vmuon_mva_fake_ed_v4);
  newtree->Branch("muon_ptrel",&vmuon_ptrel);
  newtree->Branch("muon_ptratio",&vmuon_ptratio);
  newtree->Branch("muon_cj_bjetdisc",&vmuon_cj_bjetdisc);
  newtree->Branch("muon_cj_flavour",&vmuon_cj_flavour);
  newtree->Branch("muon_lepton_type",&vmuon_lepton_type);
  newtree->Branch("muon_is_cf",   &vmuon_is_cf);

  newtree->Branch("SKWeight", &vSKWeight);


}

void SkimTree_FakeEventSkimBDT::executeEvent(){


  ResetLeptonBDTSKFlat();

  vSKWeight=MCweight(true,true);

  SetupLeptonBDTSKFlat();

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
