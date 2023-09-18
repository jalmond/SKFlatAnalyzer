#include "HNL_LeptonFakeRate.h"

void HNL_LeptonFakeRate::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

  TString AnalyzerPath=getenv("SKFlat_WD"), SKFlatV = getenv("SKFlatV");
  TString FilePath="/data/"+SKFlatV+"/"+GetEra()+"/FakeRate/NvtxSF/";
  NvtxSFFile = new TFile(AnalyzerPath+FilePath+"FakeRateNVtx13TeV_"+GetEra()+".root");

}

void HNL_LeptonFakeRate::executeEvent(){

  Event ev = GetEvent();

  AnalyzerParameter param;

  //************************************************///
  // setup list of IDs
  //************************************************///
  vector<pair<TString, TString> > MuIDs; vector<pair<TString, TString> > ElIDs;
  vector<HNL_LeptonCore::Channel> channel;
  vector<TString> paramnames;
  
  TString           era="2016";
  if(DataYear==2017)era="2017";
  if(DataYear==2018)era="2018";
  
  // Loose ID has no loose IP
  // HNTight_17028 :  PassMVA(0.9, 0.825, 0.5)) && fabs(dXY())<0.01 && fabs(dZ())<0.04 && fabs(IP3D()/IP3Derr())<4. RelIso()<0.08
  //                  PassConversionVeto() IsGsfCtfScPixChargeConsistent Pass_TriggerEmulation
  // HNLoose_17028 : Pass_HNLoose2016(0.6, 0.2, 0.1, 10.) && MVA(-0.1, 0.1, -0.1) PassConversionVeto() IsGsfCtfScPixChargeConsistent Pass_TriggerEmulation

  //ElIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_FO_"+era)); //JH : El Tight + El Loose
  //paramnames.push_back("HNL_ULID_"+era  );                                                                                                                    


  //for (auto i: ElIDs) {
  //  channel.push_back(EE);
  //  MuIDs.push_back(make_pair("HNVeto2016","HNLoose_17028")); //JH : Mu Tight (never used) + Mu Loose (to veto muon in El fake mearuement)
  //}
   
/*
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_TagHF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_TagHF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_TagLF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_TagLF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Pgt0p45")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Pgt0p45"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Plt0p45")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Plt0p45"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p01")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p01"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p01_Pgt0p45")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p01_Pgt0p45"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p01_Plt0p45")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p01_Plt0p45"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p01_TagHF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p01_TagHF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p01_TagLF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p01_TagLF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p015")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p015"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p02")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p02"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p025")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p025"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p025_Pgt0p45")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p025_Pgt0p45"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p025_Plt0p45")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p025_Plt0p45"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p025_TagHF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p025_TagHF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p025_TagLF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p025_TagLF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p025_Pgt0p45_TagHF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p025_Pgt0p45_TagHF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p025_Pgt0p45_TagLF")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p025_Pgt0p45_TagLF"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p03")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p03"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p035")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p035"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p04")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p04"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p045")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p045"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p05")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p05"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p055")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p055"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p1")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p1"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_2017_Blt0p2")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_Blt0p2"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_TopMVA_FO_MM")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_TriLep"  );
  MuIDs.push_back(make_pair("HNL_ULID_"+era, "MVALoose")); //JH : test
  paramnames.push_back("HNL_ULID_"+era+"_MVALoose"  );
*/  

  MuIDs.push_back(make_pair("HNL_ULID_"+era, "HNL_ULID_Fake")); //JH : Tight --> HF MVA, Loose --> Tight without MVA (MVALoose + SIP3D < 7)
  paramnames.push_back("HNL_ULID_"+era+"_LFvsHF");

  // MUON IDS
  /*MuIDs.push_back(make_pair("HNTightPFIsoMedium","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTightPFIsoTight","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTightPFIsoVeryTight","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTightPFIsoVeryVeryTight","HNLoosePOG"));
  MuIDs.push_back(make_pair("HNTight_17028","HNLoose_17028"));
  MuIDs.push_back(make_pair("HNTightV2","HNLooseV1"));
  MuIDs.push_back(make_pair("HNLooseMVA","HNLooseMVA"));
  
  paramnames.push_back("POGTightWithMediumIso");
  paramnames.push_back("POGTightWithTightIso");
  paramnames.push_back("POGTightWithVeryTightIso");
  paramnames.push_back("POGTightWithVeryVeryTightIso");
  paramnames.push_back("HNTight_17028");
  paramnames.push_back("HNTightV2");
  paramnames.push_back("HNMVA");*/

 
  int imu = MuIDs.size() - channel.size(); //JH : N of muon for muon fake rate measurement (not for muon veto in electron channel)
  for (int i =0; i < imu; i++){
    channel.push_back(MuMu);
    ElIDs.push_back(make_pair("HNVetoMVA","HNLoose_17028"));
  } //JH

  if( paramnames.size() != ElIDs.size()) return;


  for(unsigned int it_id=0; it_id<ElIDs.size(); it_id++){
    
    //************************************************///
    // setup leptton veto/loose/tight 
    //************************************************///

    TString MuonTightID      = MuIDs[it_id].first;
    TString MuonLooseID      = MuIDs[it_id].second;
    TString ElectronTightID  = ElIDs[it_id].first;
    TString ElectronLooseID  = ElIDs[it_id].second;

    TString FakeRateID       = (channel[it_id] == MuMu) ? MuonLooseID :  ElectronLooseID;
    TString Tight_ID         = (channel[it_id] == MuMu) ? MuonTightID :  ElectronTightID;
    
    param.Clear();
  
    param.syst_ = AnalyzerParameter::Central;
    param.Name = paramnames[it_id];
    param.MCCorrrectionIgnoreNoHist = false;

   
    //==== Muon IDparam.Muon_Loose_ID
    param.Muon_Tight_ID        = MuonTightID;
    param.Muon_Loose_ID        = MuonLooseID;

    //==== Electron ID
    param.Electron_Tight_ID       = ElectronTightID;
    param.Electron_Loose_ID       = ElectronLooseID;

    //==== Jet ID
    param.Jet_ID    = "tight";

    //    param.WriteOutVerbose = -1;  Run All IDs and make All Region plots
    //    param.WriteOutVerbose = -2;  Run All IDs and make only FR
    //    param.WriteOutVerbose = -3;  Run All IDs and make only FR + PR
    //     param.WriteOutVerbose = 1;  Run Main ID and make All Region plots
    //     param.WriteOutVerbose = 2;  Run Main ID and make  only FR 
    //     param.WriteOutVerbose = 3;  Run Main ID and make  only FR + PR
    //     param.WriteOutVerbose = 0;  makes NVertx plots
    param.WriteOutVerbose= 2; // 0 means only make FR  1 means FR+PR  2 means SR+PR + CR plots  3 means makes NVertx plots
   
    
    if(param.WriteOutVerbose >=0){
      if (channel[it_id]==MuMu && MuonTightID != "HNL_ULID_"+era) continue;
      if (channel[it_id]==EE   && ElectronTightID != "HNL_ULID_"+era) continue;
    }
    
    param.Name = GetChannelString(channel[it_id]) + "_"+ paramnames[it_id]; //JH : MuMu_HNL_ULID_2017_V1
    
    //************************************************///
    // run event
    //************************************************///
    
    executeEventFromParameter(param, Tight_ID, channel[it_id]);
  } 
}

void HNL_LeptonFakeRate::executeEventFromParameter(AnalyzerParameter param, TString El_ID, HNL_LeptonCore::Channel channel){

  
  //cout << "executeEventFromParameter " << GetChannelString(channel) << " " << El_ID  << endl;

  //************************************************///
  // setup event level objects
  Event ev = GetEvent();

  double weight = SetupWeight(ev, param) / ev.GetTriggerLumi("Full"); // xsec * KFactor * pu reweight * L1 prefire
  double eventSign = MCweight(true, false); // use sign, but not scale by xsec
  if(IsData) weight = 1;
  
  if(!PassMETFilter()) return;

  std::vector<Electron> loose_electrons     = GetElectrons( param,param.Electron_Loose_ID, 9.5, 2.5,false) ;
  std::vector<Muon>     loose_muons         = GetMuons    ( param,param.Muon_Loose_ID, 5, 2.4, false);

  if(HasFlag("FakeRateTruth")){
    for(auto mu:All_Muons){
      FillHist(param.Name+"_AllMuon_MVA", mu.MVA() , eventSign, 200., -1, 1); //JH : to check all muon MVA
      FillHist(param.Name+"_AllMuon_LeptonType", mu.LeptonGenType() , eventSign, 13, -6, 7); //JH : to check all muon type
      FillHist(param.Name+"_AllMuon_JetFlavor", mu.CloseJet_FlavourInt()  , eventSign, 6, 0, 6);
      //if(mu.Pt()>10.)           FillHist(param.Name+"_AllMuon_MVA_pt10", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.fEta()<2.4)         FillHist(param.Name+"_AllMuon_MVA_eta2p4", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.MiniRelIso()<0.4)   FillHist(param.Name+"_AllMuon_MVA_MiniIso0p4", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.SIP3D()<8.)         FillHist(param.Name+"_AllMuon_MVA_SIP3D8", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.fdXY()<0.05)        FillHist(param.Name+"_AllMuon_MVA_dxy0p05", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.fdZ()<0.1)          FillHist(param.Name+"_AllMuon_MVA_dz0p1", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.isPOGLoose())       FillHist(param.Name+"_AllMuon_MVA_POGL", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.isPOGMedium())      FillHist(param.Name+"_AllMuon_MVA_POGM", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.Pass_LepMVAID())    FillHist(param.Name+"_AllMuon_MVA_LepMVAID", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
      //if(mu.PassID("MVALoose")) FillHist(param.Name+"_AllMuon_MVA_MVALoose", mu.MVA() , eventSign, 200., -1, 1); //JH : to check loose muon MVA
    } //JH
    
    FillHist(param.Name + "_AllMuon_NMu", All_Muons.size() , eventSign, 10., 0., 10);
    FillHist((param.Name + "_NlooseMu").Data(), loose_muons.size() , eventSign, 10., 0., 10);

    std::map<TString, double> PtPartonSFmap;
    //PtPartonSFmap[param.Muon_Loose_ID] = 0.697; //FIXME fill in the values later
    PtPartonSFmap["HNL_ULID_2017_MVALoose"] = 0.734;
    PtPartonSFmap["HNL_ULID_2017_TriLep"] = 0.706;
    PtPartonSFmap["HNL_ULID_2017_Blt0p01"] = 0.555;
    PtPartonSFmap["HNL_ULID_2017_Blt0p015"] = 0.611;
    PtPartonSFmap["HNL_ULID_2017_Blt0p02"] = 0.659;
    PtPartonSFmap["HNL_ULID_2017_Blt0p025"] = 0.687;
    PtPartonSFmap["HNL_ULID_2017_Blt0p03"] = 0.702;
    PtPartonSFmap["HNL_ULID_2017_Blt0p035"] = 0.717;
    PtPartonSFmap["HNL_ULID_2017_Pgt0p45"] = 0.769;
    PtPartonSFmap["HNL_ULID_2017_Plt0p45"] = 0.308;
    PtPartonSFmap["HNL_ULID_2017_Blt0p01_Pgt0p45"] = 0.571;
    PtPartonSFmap["HNL_ULID_2017_Blt0p01_Plt0p45"] = 0.308;
    double PtPartonQCD;
    //double PtPartonData;
    double PtPartonUncorr;
    double CloseJetPt;
    int TagHF;
    for(auto mu:loose_muons){
      PtPartonQCD = mu.PtParton(PtPartonSFmap[param.Muon_Loose_ID],0.64,0.64);
      //PtPartonData = mu.PtParton(1,0.64,0.64);
      PtPartonUncorr = mu.PtParton(1,0.64,0.64);
      CloseJetPt = mu.Pt()/mu.CloseJet_Ptratio();
      if(mu.MVA()<mu.HNL_MVA_Fake("EDv4")) TagHF = 1;
      else TagHF = 0;
      FillHist((param.Name + "_MVA").Data(), mu.MVA() , eventSign, 200., -1, 1);
      FillHist((param.Name + "_LeptonType").Data(), mu.LeptonGenType() , eventSign, 13, -6, 7);
      FillHist((param.Name + "_JetFlavor").Data(), mu.CloseJet_FlavourInt()  , eventSign, 6, 0, 6);
      FillHist((param.Name + "_LeptonType_JetFlavor").Data(), mu.LeptonGenType(), mu.CloseJet_FlavourInt(), eventSign, 13, -6, 7, 6, 0, 6);
      //if(mu.LepGenTypeString()=="IsPrompt"){
      //  FillProf((param.Name + "_prompt_MVA_PtPartonUncorr").Data(), mu.MVA(), PtPartonUncorr, eventSign, 200, -1, 1);
      //  FillProf((param.Name + "_prompt_MVA_PtPartonQCD").Data(), mu.MVA(), PtPartonQCD, eventSign, 200, -1, 1);
      //  //FillProf((param.Name + "_prompt_MVA_CloseJetPt").Data(), mu.MVA(), CloseJetPt, eventSign, 200, -1, 1);
      //  FillProf((param.Name + "_prompt_MVA_Ptratio").Data(), mu.MVA(), mu.CloseJet_Ptratio(), eventSign, 200, -1, 1);
      //  FillProf((param.Name + "_prompt_MVA_BScore").Data(), mu.MVA(), mu.CloseJet_BScore(), eventSign, 200, -1, 1);
      //  //FillProf((param.Name + "_prompt_Ptratio_CloseJetPt").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
      //  //FillProf((param.Name + "_prompt_BScore_CloseJetPt").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
      //  //FillProf((param.Name + "_prompt_BScore_Ptratio_CloseJetPt").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
      //  //FillHist((param.Name + "_prompt_Ptratio").Data(), mu.CloseJet_Ptratio(), eventSign, 30, 0, 1.5);
      //  FillHist((param.Name + "_prompt_BScore_Ptratio").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
      //  //FillHist((param.Name + "_prompt_BScore_PtPartonUncorr").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
      //  //FillHist((param.Name + "_prompt_BScore_PtPartonQCD").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
      //}
      if(mu.LepGenTypeString()=="IsFake"){
        FillProf((param.Name + "_fake_MVA_PtPartonUncorr").Data(), mu.MVA(), PtPartonUncorr, eventSign, 200, -1, 1);
        FillProf((param.Name + "_fake_MVA_PtPartonQCD").Data(), mu.MVA(), PtPartonQCD, eventSign, 200, -1, 1);
        //FillProf((param.Name + "_fake_MVA_CloseJetPt").Data(), mu.MVA(), CloseJetPt, eventSign, 200, -1, 1);
        FillProf((param.Name + "_fake_MVA_Ptratio").Data(), mu.MVA(), mu.CloseJet_Ptratio(), eventSign, 200, -1, 1);
        FillProf((param.Name + "_fake_MVA_BScore").Data(), mu.MVA(), mu.CloseJet_BScore(), eventSign, 200, -1, 1);
        //FillProf((param.Name + "_fake_Ptratio_CloseJetPt").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
        //FillProf((param.Name + "_fake_BScore_CloseJetPt").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
        //FillProf((param.Name + "_fake_BScore_Ptratio_CloseJetPt").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
        FillHist((param.Name + "_fake_BScore").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
        FillHist((param.Name + "_fake_Ptratio").Data(), mu.CloseJet_Ptratio(), eventSign, 30, 0, 1.5);
        FillHist((param.Name + "_fake_BScore_Ptratio").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
        //FillHist((param.Name + "_fake_BScore_PtPartonUncorr").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
        //FillHist((param.Name + "_fake_BScore_PtPartonQCD").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
        if(mu.MVA()>0.64){
          //FillProf((param.Name + "_fake_Ptratio_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
          //FillProf((param.Name + "_fake_BScore_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
          //FillProf((param.Name + "_fake_BScore_Ptratio_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
          FillHist((param.Name + "_fake_BScore_MVAgt0p64").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
          FillHist((param.Name + "_fake_Ptratio_MVAgt0p64").Data(), mu.CloseJet_Ptratio() , eventSign, 30, 0, 1.5);
          FillHist((param.Name + "_fake_BScore_Ptratio_MVAgt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
          //FillHist((param.Name + "_fake_BScore_PtPartonUncorr_MVAgt0p64").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
          //FillHist((param.Name + "_fake_BScore_PtPartonQCD_MVAgt0p64").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
        }
        else{
          //FillProf((param.Name + "_fake_Ptratio_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
          //FillProf((param.Name + "_fake_BScore_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
          //FillProf((param.Name + "_fake_BScore_Ptratio_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
          FillHist((param.Name + "_fake_BScore_MVAlt0p64").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
          FillHist((param.Name + "_fake_Ptratio_MVAlt0p64").Data(), mu.CloseJet_Ptratio() , eventSign, 30, 0, 1.5);
          FillHist((param.Name + "_fake_BScore_Ptratio_MVAlt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
          //FillHist((param.Name + "_fake_BScore_PtPartonUncorr_MVAlt0p64").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
          //FillHist((param.Name + "_fake_BScore_PtPartonQCD_MVAlt0p64").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
        }
        if(mu.CloseJet_Flavour().Contains("HF")){
          FillProf((param.Name + "_fakeHF_MVA_PtPartonUncorr").Data(), mu.MVA(), PtPartonUncorr, eventSign, 200, -1, 1);
          FillProf((param.Name + "_fakeHF_MVA_PtPartonQCD").Data(), mu.MVA(), PtPartonQCD, eventSign, 200, -1, 1);
          //FillProf((param.Name + "_fakeHF_MVA_CloseJetPt").Data(), mu.MVA(), CloseJetPt, eventSign, 200, -1, 1);
          FillProf((param.Name + "_fakeHF_MVA_Ptratio").Data(), mu.MVA(), mu.CloseJet_Ptratio(), eventSign, 200, -1, 1);
          FillProf((param.Name + "_fakeHF_MVA_BScore").Data(), mu.MVA(), mu.CloseJet_BScore(), eventSign, 200, -1, 1);
          //FillProf((param.Name + "_fakeHF_Ptratio_CloseJetPt").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
          //FillProf((param.Name + "_fakeHF_BScore_CloseJetPt").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
          //FillProf((param.Name + "_fakeHF_BScore_Ptratio_CloseJetPt").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
          FillHist((param.Name + "_fakeHF_BScore").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
          FillHist((param.Name + "_fakeHF_Ptratio").Data(), mu.CloseJet_Ptratio(), eventSign, 30, 0, 1.5);
          FillHist((param.Name + "_fakeHF_BScore_Ptratio").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
          //FillHist((param.Name + "_fakeHF_BScore_PtPartonUncorr").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
          //FillHist((param.Name + "_fakeHF_BScore_PtPartonQCD").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
          FillHist((param.Name + "_fakeHF_MVALight").Data(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1);
          FillHist((param.Name + "_fakeHF_MVA_MVALight").Data(), mu.MVA(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1, 200, -1, 1);
          FillHist((param.Name + "_fakeHF_TagHF").Data(), TagHF , eventSign, 2, 0, 2);
          FillHist((param.Name + "_fakeHF_TagHFnew").Data(), mu.TagHF() , eventSign, 2, 0, 2);
          if(mu.MVA()>0.64){
            //FillProf((param.Name + "_fakeHF_Ptratio_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
            //FillProf((param.Name + "_fakeHF_BScore_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
            //FillProf((param.Name + "_fakeHF_BScore_Ptratio_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
            FillHist((param.Name + "_fakeHF_BScore_MVAgt0p64").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
            FillHist((param.Name + "_fakeHF_Ptratio_MVAgt0p64").Data(), mu.CloseJet_Ptratio() , eventSign, 30, 0, 1.5);
            FillHist((param.Name + "_fakeHF_BScore_Ptratio_MVAgt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
            //FillHist((param.Name + "_fakeHF_BScore_PtPartonUncorr_MVAgt0p64").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
            //FillHist((param.Name + "_fakeHF_BScore_PtPartonQCD_MVAgt0p64").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
          }
          else{
            //FillProf((param.Name + "_fakeHF_Ptratio_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
            //FillProf((param.Name + "_fakeHF_BScore_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
            //FillProf((param.Name + "_fakeHF_BScore_Ptratio_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
            FillHist((param.Name + "_fakeHF_BScore_MVAlt0p64").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
            FillHist((param.Name + "_fakeHF_Ptratio_MVAlt0p64").Data(), mu.CloseJet_Ptratio() , eventSign, 30, 0, 1.5);
            FillHist((param.Name + "_fakeHF_BScore_Ptratio_MVAlt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
            FillHist((param.Name + "_fakeHF_MVALight_MVAlt0p64").Data(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1);
            FillHist((param.Name + "_fakeHF_MVA_MVALight_MVAlt0p64").Data(), mu.MVA(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1, 200, -1, 1);
            FillHist((param.Name + "_fakeHF_TagHF_MVAlt0p64").Data(), TagHF , eventSign, 2, 0, 2);
            FillHist((param.Name + "_fakeHF_TagHFnew_MVAlt0p64").Data(), mu.TagHF() , eventSign, 2, 0, 2);
            //FillHist((param.Name + "_fakeHF_BScore_PtPartonUncorr_MVAlt0p64").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
            //FillHist((param.Name + "_fakeHF_BScore_PtPartonQCD_MVAlt0p64").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
          }
        }
        else if(mu.CloseJet_Flavour().Contains("LF")){
          FillProf((param.Name + "_fakeLF_MVA_PtPartonUncorr").Data(), mu.MVA(), PtPartonUncorr, eventSign, 200, -1, 1);
          FillProf((param.Name + "_fakeLF_MVA_PtPartonQCD").Data(), mu.MVA(), PtPartonQCD, eventSign, 200, -1, 1);
          //FillProf((param.Name + "_fakeLF_MVA_CloseJetPt").Data(), mu.MVA(), CloseJetPt, eventSign, 200, -1, 1);
          FillProf((param.Name + "_fakeLF_MVA_Ptratio").Data(), mu.MVA(), mu.CloseJet_Ptratio(), eventSign, 200, -1, 1);
          FillProf((param.Name + "_fakeLF_MVA_BScore").Data(), mu.MVA(), mu.CloseJet_BScore(), eventSign, 200, -1, 1);
          //FillProf((param.Name + "_fakeLF_Ptratio_CloseJetPt").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
          //FillProf((param.Name + "_fakeLF_BScore_CloseJetPt").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
          //FillProf((param.Name + "_fakeLF_BScore_Ptratio_CloseJetPt").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
          FillHist((param.Name + "_fakeLF_BScore").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
          FillHist((param.Name + "_fakeLF_Ptratio").Data(), mu.CloseJet_Ptratio(), eventSign, 30, 0, 1.5);
          FillHist((param.Name + "_fakeLF_BScore_Ptratio").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
          //FillHist((param.Name + "_fakeLF_BScore_PtPartonUncorr").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100, 0, 1000);
          //FillHist((param.Name + "_fakeLF_BScore_PtPartonQCD").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100, 0, 1000);
          FillHist((param.Name + "_fakeLF_MVALight").Data(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1);
          FillHist((param.Name + "_fakeLF_MVA_MVALight").Data(), mu.MVA(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1, 200, -1, 1);
          FillHist((param.Name + "_fakeLF_TagHF").Data(), TagHF , eventSign, 2, 0, 2);
          FillHist((param.Name + "_fakeLF_TagHFnew").Data(), mu.TagHF() , eventSign, 2, 0, 2);
          if(mu.MVA()>0.64){
            //FillProf((param.Name + "_fakeLF_Ptratio_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
            //FillProf((param.Name + "_fakeLF_BScore_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
            //FillProf((param.Name + "_fakeLF_BScore_Ptratio_CloseJetPt_MVAgt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
            FillHist((param.Name + "_fakeLF_BScore_MVAgt0p64").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
            FillHist((param.Name + "_fakeLF_Ptratio_MVAgt0p64").Data(), mu.CloseJet_Ptratio() , eventSign, 30, 0, 1.5);
            FillHist((param.Name + "_fakeLF_BScore_Ptratio_MVAgt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
            //FillHist((param.Name + "_fakeLF_BScore_PtPartonUncorr_MVAgt0p64").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
            //FillHist((param.Name + "_fakeLF_BScore_PtPartonQCD_MVAgt0p64").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
          }
          else{
            //FillProf((param.Name + "_fakeLF_Ptratio_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
            //FillProf((param.Name + "_fakeLF_BScore_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
            //FillProf((param.Name + "_fakeLF_BScore_Ptratio_CloseJetPt_MVAlt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
            FillHist((param.Name + "_fakeLF_BScore_MVAlt0p64").Data(), mu.CloseJet_BScore() , eventSign, 200, 0, 1);
            FillHist((param.Name + "_fakeLF_Ptratio_MVAlt0p64").Data(), mu.CloseJet_Ptratio() , eventSign, 30, 0, 1.5);
            FillHist((param.Name + "_fakeLF_BScore_Ptratio_MVAlt0p64").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
            FillHist((param.Name + "_fakeLF_MVALight_MVAlt0p64").Data(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1);
            FillHist((param.Name + "_fakeLF_MVA_MVALight_MVAlt0p64").Data(), mu.MVA(), mu.HNL_MVA_Fake("EDv4") , eventSign, 200, -1, 1, 200, -1, 1);
            FillHist((param.Name + "_fakeLF_TagHF_MVAlt0p64").Data(), TagHF , eventSign, 2, 0, 2);
            FillHist((param.Name + "_fakeLF_TagHFnew_MVAlt0p64").Data(), mu.TagHF() , eventSign, 2, 0, 2);
            //FillHist((param.Name + "_fakeLF_BScore_PtPartonUncorr_MVAlt0p64").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
            //FillHist((param.Name + "_fakeLF_BScore_PtPartonQCD_MVAlt0p64").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
          }
        }
      }
      /*
      if(mu.LepGenTypeString()=="IsEWtau"){
        FillProf((param.Name + "_tau_MVA_PtPartonUncorr").Data(), mu.MVA(), PtPartonUncorr, eventSign, 200, -1, 1);
        FillProf((param.Name + "_tau_MVA_PtPartonQCD").Data(), mu.MVA(), PtPartonQCD, eventSign, 200, -1, 1);
        //FillProf((param.Name + "_tau_MVA_CloseJetPt").Data(), mu.MVA(), CloseJetPt, eventSign, 200, -1, 1);
        FillProf((param.Name + "_tau_MVA_Ptratio").Data(), mu.MVA(), mu.CloseJet_Ptratio(), eventSign, 200, -1, 1);
        FillProf((param.Name + "_tau_MVA_BScore").Data(), mu.MVA(), mu.CloseJet_BScore(), eventSign, 200, -1, 1);
        //FillProf((param.Name + "_tau_Ptratio_CloseJetPt").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
        //FillProf((param.Name + "_tau_BScore_CloseJetPt").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
        //FillProf((param.Name + "_tau_BScore_Ptratio_CloseJetPt").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
        FillHist((param.Name + "_tau_BScore_Ptratio").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
        //FillHist((param.Name + "_tau_BScore_PtPartonUncorr").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
        //FillHist((param.Name + "_tau_BScore_PtPartonQCD").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
      }
      if(mu.LepGenTypeString()=="IsConv"){
        FillProf((param.Name + "_conv_MVA_PtPartonUncorr").Data(), mu.MVA(), PtPartonUncorr, eventSign, 200, -1, 1);
        FillProf((param.Name + "_conv_MVA_PtPartonQCD").Data(), mu.MVA(), PtPartonQCD, eventSign, 200, -1, 1);
        //FillProf((param.Name + "_conv_MVA_CloseJetPt").Data(), mu.MVA(), CloseJetPt, eventSign, 200, -1, 1);
        FillProf((param.Name + "_conv_MVA_Ptratio").Data(), mu.MVA(), mu.CloseJet_Ptratio(), eventSign, 200, -1, 1);
        FillProf((param.Name + "_conv_MVA_BScore").Data(), mu.MVA(), mu.CloseJet_BScore(), eventSign, 200, -1, 1);
        //FillProf((param.Name + "_conv_Ptratio_CloseJetPt").Data(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 30, 0, 1.5);
        //FillProf((param.Name + "_conv_BScore_CloseJetPt").Data(), mu.CloseJet_BScore(), CloseJetPt, eventSign, 200, 0, 1);
        //FillProf((param.Name + "_conv_BScore_Ptratio_CloseJetPt").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), CloseJetPt, eventSign, 200, 0, 1, 30, 0, 1.5);
        FillHist((param.Name + "_conv_BScore_Ptratio").Data(), mu.CloseJet_BScore(), mu.CloseJet_Ptratio(), eventSign, 200, 0, 1, 30, 0, 1.5);
        //FillHist((param.Name + "_conv_BScore_PtPartonUncorr").Data(), mu.CloseJet_BScore(), PtPartonUncorr, eventSign, 200, 0, 1, 100., 0, 1000);
        //FillHist((param.Name + "_conv_BScore_PtPartonQCD").Data(), mu.CloseJet_BScore(), PtPartonQCD, eventSign, 200, 0, 1, 100., 0, 1000);
      }
      */
    }
    GetMCFakeRates(loose_muons, param, eventSign);
    return;
  }

  std::vector<Jet> jets_tmp     = GetJets   ( param, "tight", 30., 2.7);
  std::vector<Jet> jets; 
  for(unsigned int ijet =0; ijet < jets_tmp.size(); ijet++){
    bool jetok=true;
    
    for(unsigned int iel=0 ; iel < loose_electrons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_electrons[iel]) < 0.4) jetok = false;
    }
    for(unsigned int iel=0 ; iel < loose_muons.size(); iel++){
      if(jets_tmp[ijet].DeltaR(loose_muons[iel]) < 0.4) jetok = false;
    }
    if(jetok) jets.push_back(jets_tmp[ijet]);
  }

  if(channel==EE) {

    RunE(loose_electrons,loose_muons, jets,  param, weight);
    
  }
  if(channel==MuMu){
    RunM(loose_electrons,loose_muons,  jets, param, weight);
  }
}

void HNL_LeptonFakeRate::GetMCFakeRates(std::vector<Muon> loose_muons, AnalyzerParameter param,  float weight){

  // No selection MC fake rate
  if(!IsDATA){
    int nbin_pt = 11;
    double ptbins[nbin_pt+1];
    vector<double> vptbins =  {10., 15.,20.,25.,30.,35.,40.,50.,60.,100.,200.,1000.} ; 
    std::copy(vptbins.begin(), vptbins.end(), ptbins);
    TString L_prefix = "Fake_Loose_"+param.Name+"_NoSel";
    TString T_prefix = "Fake_Tight_"+param.Name+"_NoSel"; //JH : Fake_Tight_MuMu_HNL_ULID_2017_trilep
    std::map<TString, double> PtPartonSFmap;
    PtPartonSFmap["HNL_ULID_2017_MVALoose"] = 0.734;
    PtPartonSFmap["HNL_ULID_2017_TriLep"] = 0.706;
    PtPartonSFmap["HNL_ULID_2017_Blt0p01"] = 0.555;
    PtPartonSFmap["HNL_ULID_2017_Blt0p015"] = 0.611;
    PtPartonSFmap["HNL_ULID_2017_Blt0p02"] = 0.659;
    PtPartonSFmap["HNL_ULID_2017_Blt0p025"] = 0.687;
    PtPartonSFmap["HNL_ULID_2017_Blt0p03"] = 0.702;
    PtPartonSFmap["HNL_ULID_2017_Blt0p035"] = 0.717;
    PtPartonSFmap["HNL_ULID_2017_Pgt0p45"] = 0.769;
    PtPartonSFmap["HNL_ULID_2017_Plt0p45"] = 0.308;
    PtPartonSFmap["HNL_ULID_2017_Blt0p01_Pgt0p45"] = 0.571;
    PtPartonSFmap["HNL_ULID_2017_Blt0p01_Plt0p45"] = 0.308;
    for(auto mu:loose_muons){
      double PtPartonQCD = mu.PtParton(PtPartonSFmap[param.Muon_Loose_ID],0.64,0.64);
      //double PtPartonData = mu.PtParton(1,0.64,0.64);
      double PtPartonUncorr = mu.PtParton(1,0.64,0.64);
      bool passT = false;
      if(mu.PassID(param.Muon_Tight_ID)) passT = true;
      for(int ilep = 0 ; ilep < 2; ilep++){
        TString prefix = (ilep==0) ? L_prefix : T_prefix;
        if(ilep==1 && !passT) continue;
        if(mu.LepGenTypeString()=="IsPrompt"){
          //FillHist((prefix + "_prompt_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_prompt_PtPartonUncorr").Data(), PtPartonUncorr, weight, nbin_pt, ptbins);
          FillHist((prefix + "_prompt_PtPartonQCD").Data(), PtPartonQCD, weight, nbin_pt, ptbins);
        }
        if(mu.LepGenTypeString()=="IsFake"){
          //FillHist((prefix + "_fake_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_fake_PtPartonUncorr").Data(), PtPartonUncorr, weight, nbin_pt, ptbins);
          FillHist((prefix + "_fake_PtPartonQCD").Data(), PtPartonQCD, weight, nbin_pt, ptbins);
          if(mu.CloseJet_Flavour().Contains("HF")){
            //FillHist((prefix + "_fakeHF_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight, nbin_pt, ptbins, nbin_eta , etabins);
            FillHist((prefix + "_fakeHF_PtPartonUncorr").Data(), PtPartonUncorr, weight, nbin_pt, ptbins);
            FillHist((prefix + "_fakeHF_PtPartonQCD").Data(), PtPartonQCD, weight, nbin_pt, ptbins);
          }
          else if(mu.CloseJet_Flavour().Contains("LF")){
            //FillHist((prefix + "_fakeLF_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight, nbin_pt, ptbins, nbin_eta , etabins);
            FillHist((prefix + "_fakeLF_PtPartonUncorr").Data(), PtPartonUncorr, weight, nbin_pt, ptbins);
            FillHist((prefix + "_fakeLF_PtPartonQCD").Data(), PtPartonQCD, weight, nbin_pt, ptbins);
          }
        }
        if(mu.LepGenTypeString()=="IsEWtau"){
          //FillHist((prefix + "_tau_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_tau_PtPartonUncorr").Data(), PtPartonUncorr, weight, nbin_pt, ptbins);
          FillHist((prefix + "_tau_PtPartonQCD").Data(), PtPartonQCD, weight, nbin_pt, ptbins);
        }
        if(mu.LepGenTypeString()=="IsConv"){
          //FillHist((prefix + "_conv_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_conv_PtPartonUncorr").Data(), PtPartonUncorr, weight, nbin_pt, ptbins);
          FillHist((prefix + "_conv_PtPartonQCD").Data(), PtPartonQCD, weight, nbin_pt, ptbins);
        }
      }
    }
  }

}

void HNL_LeptonFakeRate::RunM(std::vector<Electron> loose_el,  std::vector<Muon> loose_mu, std::vector<Jet> jets,   AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleEG") return;
    if(this->DataStream == "SingleElectron") return;
    if(this->DataStream == "EGamma") return;
  }

  TString label    = param.Name; //JH : MuMu_HNL_ULID_2017_V1
  Event ev = GetEvent();

  Particle METv = GetvMET("PuppiT1xyULCorr");

  if(loose_mu.size() == 0) return;
  
  // remove if electron
  if(loose_el.size() > 0) return;


  //double isocut = GetIsoFromID("Muon", param.Muon_Tight_ID,loose_mu[0].Eta(), loose_mu[0].Pt());
  
  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_mu);
  std::vector<bool> blepsT;
  for(auto ilep : loose_mu) {
    blepsT.push_back(ilep.PassID(param.Muon_Tight_ID));
  }

  if(param.WriteOutVerbose == 0) {
    MakeNVertexDistPrescaledTrig(MuMu,param, ev, leps,blepsT,label,event_weight);
    return;
  }

  
  if(fabs(param.WriteOutVerbose) > 1 ){
    GetFakeRateAndPromptRates(param, leps,blepsT,jets,label,event_weight,-999.); //JH : MuMu_HNL_ULID_2017_V1
    return;
  }

  //if (param.Muon_Tight_ID != "HNTightV2") return; //JH

  MakeDiLepPlots(MuMu,param, ev, leps,blepsT,label,event_weight); //JH : MuMu_HNL_ULID_2017_V1

  if(loose_mu.size() != 1) return; //JH : require only one loose muon
  
  bool has_away_jet=false;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(loose_mu[0].Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      has_away_jet=true;
    }
  }

  /// Make plots to check control of prompt leptons and to plot Loose and Tight Samples
  /// Only run when not running MANY IDs so param.WriteOutVerbose is set to 2

  // prescale_trigger is to to weight of trigger prescalee depedning on the pt
  float prescale_trigger = GetPrescale(leps);
  if(prescale_trigger == 0.) return;
  event_weight*=prescale_trigger;


  bool truth_match= false;
  if(!IsDATA) {
    if(loose_mu.at(0).LeptonGenType() > 0 ) truth_match=true;
  }
  else truth_match=true;

  if(jets.size() > 0){
    if(jets.at(0).Pt() > 40.){
      
      // Plot for 1  lep + Jet pT > trig pt
      FillRegionPlots("MuMu", "SingleLooseMuJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(has_away_jet)       FillRegionPlots("MuMu","SingleLooseMuAwayJet_"+param.Name, jets,   loose_el,  loose_mu,  METv, event_weight);
      if(blepsT[0]){
        FillRegionPlots("MuMu","SingleTightMuJet_"+param.Name,  jets,  loose_el,loose_mu,  METv, event_weight);
        if(has_away_jet) FillRegionPlots("MuMu","SingleTightMuAwayJet_"+param.Name, jets,  loose_el, loose_mu,  METv, event_weight);
      }
    }
  }

  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int imu = 0; imu < loose_mu.size();  imu++){
    METdphi=  TVector2::Phi_mpi_pi((loose_mu.at(imu).Phi()- METv.Phi()));
    MT = sqrt(2.* loose_mu.at(imu).Et()*METv.Pt() * (1 - cos( METdphi)));
  }
  if((60. < MT)  &&(MT < 100.)){
    if(loose_mu[0].Pt() > 20){
      if(jets.size() > 0 && blepsT[0]){
  if(jets[0].Pt() > 40){
    
    FillRegionPlots("MuMu","SingleTightMu_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if(has_away_jet)FillRegionPlots("MuMu","SingleTightMuAwayJet_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);

    bool isPrompt=false;
    for(unsigned int ij=0; ij < jets.size(); ij++){
      if(jets.at(ij).Pt() < 40.) continue;
      float dphi =fabs(TVector2::Phi_mpi_pi(loose_mu[0].Phi()- jets.at(ij).Phi()));
      if(dphi > 2.5){
        if((jets.at(ij).Pt() /  loose_mu[0].Pt() ) < 1.) isPrompt = true;
      }
    }
    
    if (isPrompt) FillRegionPlots("MuMu","SingleTightMu_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if (isPrompt&&has_away_jet) FillRegionPlots("MuMu","SingleTightMuAwayJet_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
  }
  
      }
    }
  }

  
  bool useevent40 = UseEvent(leps , jets, 40., METv, event_weight);
  if(useevent40){
    if (blepsT[0] && jets.size() >= 1)FillRegionPlots("MuMu","TightMu40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if (jets.size() >= 1)FillRegionPlots("MuMu","LooseMu40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    
    if(truth_match){
      if (blepsT[0] && jets.size() >= 1)FillRegionPlots("MuMu","TightMu40_"+param.Name , jets,  loose_el,loose_mu, METv, event_weight);
      if (jets.size() >= 1)FillRegionPlots("MuMu","LooseMu40_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    }
    
  }
  
  return;

} /// end function                      





void HNL_LeptonFakeRate::RunE( std::vector<Electron> loose_el, std::vector<Muon> loose_mu, std::vector<Jet> jets, AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleMuon") return;
    if(this->DataStream == "SingleMuon") return;
  }

  TString label = param.Name;
  Event ev = GetEvent();

  Particle METv = GetvMET("PuppiT1xyULCorr");
  
  if(loose_el.size() == 0) return;
 

  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                                     
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_18="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_33="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"; /// 

  // remove if muon 
  if(loose_mu.size() > 0) return;

  //double isocut = GetIsoFromID("Electron", param.Electron_Tight_ID,loose_el[0].Eta(), loose_el[0].Pt());
  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_el);
  std::vector<bool> blepsT;
  for(auto ilep : loose_el) {
    blepsT.push_back(ilep.PassID(param.Electron_Tight_ID));
  }

  if(param.WriteOutVerbose == 0) {
    MakeNVertexDistPrescaledTrig(EE,param, ev, leps,blepsT,label,event_weight);
    return;
  }

  if(fabs(param.WriteOutVerbose) > 1) {
    GetFakeRateAndPromptRates(param, leps,blepsT,jets,label,event_weight,-999.);
    return;
  }


  //if (param.Electron_Tight_ID != "HNTightV2") return;

  MakeDiLepPlots(EE,param, ev, leps,blepsT,label,event_weight);
  
  if(loose_el.size() != 1) return;

  bool has_away_jet=false;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(loose_el[0].Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      has_away_jet=true;
    }
  } 


  float prescale_trigger =  GetPrescale(leps);
  if(prescale_trigger == 0.) return;
  event_weight*=prescale_trigger;



  bool truth_match= false;
  if(!IsDATA) {
    if(loose_el.at(0).LeptonGenType() > 0 ) truth_match=true; //JH : Note that this contains internal conversions (4, 5)
  }
  else truth_match=true;

  if(jets.size() > 0){
    if(jets.at(0).Pt() > 40.){
      FillRegionPlots("EE","SingleLooseElJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(has_away_jet)      FillRegionPlots("EE","SingleLooseElAwayJet_"+param.Name , jets,   loose_el,loose_mu,  METv, event_weight);
      if(blepsT[0]){
        FillRegionPlots("EE","SingleTightElJet_"+param.Name,  jets,  loose_el,loose_mu,  METv, event_weight);
        if(has_away_jet) FillRegionPlots("EE","SingleTightElAwayJet_"+param.Name, jets,  loose_el, loose_mu,  METv, event_weight);
      }
    }
  }

  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int iel = 0; iel < loose_el.size();  iel++){
    METdphi = TVector2::Phi_mpi_pi((loose_el.at(iel).Phi()- METv.Phi()));
    MT = sqrt(2.* loose_el.at(iel).Et()*METv.Pt() * (1 - cos( METdphi)));
  }
  if((60. < MT)  &&(MT < 100.)){
    if(loose_el[0].Pt() > 25){
      if(jets.size() > 0 && blepsT[0]){
  if(jets[0].Pt() > 40){
    FillRegionPlots("EE","SingleTightEl_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if(has_away_jet)    FillRegionPlots("EE","SingleTightElAwayJet_promptCR_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);

    bool isPrompt=false;
          for(unsigned int ij=0; ij < jets.size(); ij++){
      if(jets.at(ij).Pt() < 40.) continue;
            float dphi =fabs(TVector2::Phi_mpi_pi(loose_el[0].Phi()- jets.at(ij).Phi()));
            if(dphi > 2.5){
              if((jets.at(ij).Pt() /  loose_el[0].Pt() ) < 1.) isPrompt = true;
      }
    }
    
          if (isPrompt ) FillRegionPlots("EE","SingleTightEl_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if (isPrompt && has_away_jet) FillRegionPlots("EE","SingleTightElAwayJet_promptCR2_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
  }
      }
    }
  }
  


  bool useevent40 = UseEvent(leps , jets, 40., METv,event_weight);
  if(useevent40){
    if (blepsT[0] && jets.size() >= 1)FillRegionPlots("EE","TightEl40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    if (jets.size() >= 1)FillRegionPlots("EE","LooseEl40_notm_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    
    
    if(truth_match){
      if (blepsT[0] && jets.size() >= 1)FillRegionPlots("EE","TightEl40_"+param.Name , jets,  loose_el,loose_mu, METv, event_weight);
      if (jets.size() >= 1)FillRegionPlots("EE","LooseEl40_"+param.Name, jets,  loose_el,loose_mu, METv, event_weight);
    }
  }
  
  
  return;
  


} /// end function RunE

double HNL_LeptonFakeRate::ApplyNvtxReweight(int NPV, TString Key){

  if(IsData) return 1.;
  if(!NvtxSFFile->GetListOfKeys()->Contains(Key)){ printf("[Error] No %s in NvtxSF File.\n", Key.Data()); return -1.; }

  std::map<TString, TH1D*>::iterator mapit = maphist_NvtxSF.find(Key);
  if(mapit == maphist_NvtxSF.end()){ //first usage
    maphist_NvtxSF[Key] = (TH1D*) ((TH1*) NvtxSFFile->Get(Key))->Clone();
  }
  mapit = maphist_NvtxSF.find(Key);
  
  int this_bin = mapit->second->FindBin(NPV);
  double NvtxSF = mapit->second->GetBinContent(this_bin);
  if(NvtxSF>10) NvtxSF=1.;

  return NvtxSF;

}


void HNL_LeptonFakeRate::MakeDiLepPlots(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){

  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;

  // now we have 2 Tight leptons                                                                                                                             

  TString plot_dir = GetChannelString(channel);

  // Make Z peak                                                                                                                                            
  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;
  bool PassZMass = (fabs(90. - Z.M()) < 10.) ? true : false;

  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl    = SelectBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )event_weight*= sf_btag;

  if(BJetColl.size() > 0) return;

  int nbin_pt    =6;
  double bins_pt[nbin_pt+1] = {0.,5., 10., 20., 30., 50., 200. };

  if(channel == MuMu){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    double NVxt_Mu3 =  ApplyNvtxReweight(nPV,triggerslist_3);
    double NVxt_Mu8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_Mu17 =  ApplyNvtxReweight(nPV,triggerslist_17);

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;


    if(pass_3){
      if(leps[1]->Pt() > 5){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3)*NVxt_Mu3;
  
        if(PassZMass) {
    for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    
    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8;
  
        if(PassZMass){
    for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_Mu17;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }
  
  if(channel == EE){

    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;
    double NVxt_El8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
    double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);


    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_El8;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_12){
      if(leps[1]->Pt() > 14){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }

    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_23){
      if(leps[1]->Pt() > 25){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
        if(PassZMass){
          for(auto ilep : leps)FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LepPt").Data(), ilep->Pt() , event_weight*prescale_weight,nbin_pt, bins_pt);
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }

  return;


}



void HNL_LeptonFakeRate::MakeNVertexDistPrescaledTrig(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){

  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;
  
  // now we have 2 Tight leptons
  // Make Z peak 

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;


  // remove b jet 

  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl    = SelectBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )event_weight*= sf_btag;

  if(BJetColl.size() > 0) return;

  bool PassZMass = (fabs(90. - Z.M()) < 10.) ? true : false;


  // Now plot Z peak for

  int nbin_npv    =35;
  double bins_npv[nbin_npv+1] = { 0.,5., 10.,12., 14., 16., 18., 20., 22., 24., 26., 28., 30., 32., 34., 36., 38., 40., 42., 44., 46., 48., 50., 52., 54., 56., 58., 60., 62., 64., 66., 68., 70., 75., 80., 100.};



  if(channel == MuMu){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;

    if(pass_3){
      if(leps[1]->Pt() > 5){
  double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3);

  if(PassZMass) {
    FillHist(( triggerslist_3 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
  }
  FillHist(( triggerslist_3 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_8){
      if(leps[1]->Pt() > 10){
  double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8);

  if(PassZMass){
    FillHist(( triggerslist_8 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
  }
        FillHist(( triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
  double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17);
  if(PassZMass){
    FillHist(( triggerslist_17 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
  } 
  FillHist(( triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }   
  }
  


  if(channel == EE){

    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;


    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8);
  if(PassZMass){
    FillHist(( triggerslist_8 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
  }
        FillHist(( triggerslist_8 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_12){
      if(leps[1]->Pt() > 14){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12);
  if(PassZMass){
    FillHist(( triggerslist_12 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
  }
  FillHist(( triggerslist_12 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
        }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17);
  if(PassZMass){
    FillHist(( triggerslist_17 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
  }
  FillHist(( triggerslist_17 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_23){
      if(leps[1]->Pt() > 25){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23);
  if(PassZMass){
    FillHist(( triggerslist_23 + "_Nvtx").Data(),  nPV , event_weight*prescale_weight,nbin_npv, bins_npv);
  }
        FillHist(( triggerslist_23 + "_LLMass").Data(), Z.M() , event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }
  
  return;
  

}
void HNL_LeptonFakeRate::GetFakeRateAndPromptRates(AnalyzerParameter param, std::vector<Lepton *> leps,std::vector<bool> blepsT, std::vector<Jet>    jetCollTight, TString label, float event_weight, float isocut){
          
  if (leps.size()<1) return;
  
  Event ev = GetEvent();
  Particle METv = GetvMET("PuppiT1xyULCorr");

  
  if(leps.size()==1){
    MakeFakeRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, isocut, METv); //JH : MuMu_HNL_ULID_2017_V1
  }

  if(fabs(param.WriteOutVerbose) < 3) return;

  if(leps.size()==2){
    TString dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v";
    if(DataYear==2017) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";

    if(DataYear==2018) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";

    

    if(IsDATA&& !(ev.IsPDForTrigger(dilep_triggerslist, this->DataStream) )) return;
    if(ev.PassTrigger(dilep_triggerslist))    MakePromptRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, isocut, METv);
  }
  return;
}


void HNL_LeptonFakeRate::MakePromptRatePlots(TString label, TString tag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT , std::vector<Jet> jets,  float event_weight, float isocut, Particle MET){
  
  int nbin_ptcone=11;
  int nbin_pt    =11;
  int nbin_eta   =4;
  double ptbinscone[nbin_ptcone+1] = { 10.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};
  double ptbins    [nbin_pt    +1] = { 10.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};
  double etabins   [nbin_eta+1   ] = { 0.,0.8,  1.479, 2.,  2.5};

  Event ev = GetEvent();

  if(leps.size() != 2) return;

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;
  if(fabs(90. - Z.M()) > 10.) return;

  if(blepsT[1]){
    // lep [1] is tag
    int ilep=0;
    float lep_pt = leps[ilep]->Pt();
    float lep_pt_corr =  leps[ilep]->CalcPtCone(leps[ilep]->RelIso(), isocut);
    float lep_eta = fabs(leps[ilep]->Eta());
    if(lep_pt_corr > 2000.) lep_pt_corr = 1999.;

    float PtParton;
    if(leps[ilep]->GetFlavour()=="Muon"){
      PtParton = leps[ilep]->PtParton(1, 0.64, 0.64);
    }
    else{
      PtParton = leps[ilep]->PtParton(1, 0.15, 0.2); //FIXME only for 2017
    }
    
    float weight_ptcorr=event_weight;
    TString L_prefix = "Prompt_Loose"+tag ;
    TString T_prefix = "Prompt_Tight"+tag;

    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHist((prefix + "_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      FillHist((prefix + "_PtParton_eta").Data(), PtParton, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      if(fabs(90. - Z.M()) < 5){
  FillHist((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
  FillHist((prefix + "_TZ_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      }
    }
  }
  if(blepsT[0]){
    // lep [0] is tag                                                                                                                                                             
    int ilep=1;
    float lep_pt = leps[ilep]->Pt();
    float lep_pt_corr =  leps[ilep]->CalcPtCone(leps[ilep]->RelIso(), isocut);
    float lep_eta = fabs(leps[ilep]->Eta());
    float PtParton;
    if(leps[ilep]->GetFlavour()=="Muon"){
      PtParton = leps[ilep]->PtParton(1, 0.64, 0.64);
    }
    else{
      PtParton = leps[ilep]->PtParton(1, 0.15, 0.2); //FIXME only for 2017
    }

    if(lep_pt_corr > 2000.) lep_pt_corr = 1999.;
    float weight_ptcorr=event_weight;
    TString L_prefix = "Prompt_Loose"+tag ;
    TString T_prefix = "Prompt_Tight"+tag;

    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHist((prefix + "_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      FillHist((prefix + "_PtParton_eta").Data(), PtParton, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      if(fabs(90. - Z.M()) < 5){
  FillHist((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
        FillHist((prefix + "_TZ_ptcone_eta").Data(), lep_pt_corr, lep_eta,  weight_ptcorr, nbin_ptcone, ptbinscone, nbin_eta , etabins);
      }
    }
  }


  return;
}


float HNL_LeptonFakeRate::GetPrescale(std::vector<Lepton *>   leps  ){

  if(leps.size() != 1) return 0.;

  Event ev = GetEvent();
  float prescale_trigger= 1.;
  if(leps[0]->LeptonFlavour() == Lepton::MUON){

    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";

    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

    bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
    bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;
  
    double NVxt_Mu3 =  ApplyNvtxReweight(nPV,triggerslist_3);
    double NVxt_Mu8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_Mu17 =  ApplyNvtxReweight(nPV,triggerslist_17);

    /// 10 - 20  HLT_Mu7
    /// 20 - INF  HLT_Mu17
    
    if(leps.at(0)->Pt() >= 20.){
      if(pass_17) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_Mu17;
      else return 0;
    }
    else  if(leps.at(0)->Pt() >= 10.){

      if(pass_8)prescale_trigger =  (IsDATA) ? 1. :   ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8 ; //// 20 + GeV bins
      else return 0;
    }
    else  if(leps.at(0)->Pt() >= 5.){
      if(pass_3)prescale_trigger =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3)*NVxt_Mu3 ; //// 20 + GeV bins                                                                             
      else return 0 ;
    }      
    if(prescale_trigger == 0.) return 0.;
    if(leps.at(0)->Pt() < 5.) return 0.;
    
    return prescale_trigger;
  }
  else{
    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";  
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_17 = ev.PassTrigger(triggerslist_17) && EL17PD;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;

    double NVxt_El8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
    double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);

 
    if(leps.at(0)->Pt() >= 25.){
      if(pass_23)prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
      else return 0;
    }
    else   if(leps.at(0)->Pt() >= 20.){
      if(pass_17)prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_El12;
      else return 0;
    }
    else   if(leps.at(0)->Pt() >= 15.){
      if(pass_12) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
      else return 0;
    }
    else   if(leps.at(0)->Pt() >= 9.5){
      if(pass_8) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_El8;
      else return 0;
    }
    else prescale_trigger = 0.;
    if(prescale_trigger == 0.) return 0.;
    if(leps.at(0)->Pt() < 9.5) return 0.;
    return prescale_trigger;

  }
  
  return prescale_trigger;
}
 

void HNL_LeptonFakeRate::MakeFakeRatePlots(TString label, TString mutag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT ,  std::vector<Jet> jets,  float event_weight, float isocut, Particle MET){
        
  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT MUONS  
  //bool truth_match= false;

  //if(!IsDATA) {
  //  if(leps.size() > 0){
  //    if(leps[0]->LeptonGenType() > 0 ) truth_match=true;
  //  }
  //}
  //else truth_match=true;
  //if(!truth_match) return; //JH : to compare fake rates from heavy/light jets, I need non-prompt leptons as well

  //bool useevent20 = UseEvent(leps , jets, 20.,  MET, event_weight);
  //bool useevent30 = UseEvent(leps , jets, 30.,  MET, event_weight);
  bool useevent40 = UseEvent(leps , jets, 40.,  MET, event_weight);
  //bool useevent60 = UseEvent(leps , jets, 60.,  MET, event_weight);

  label= mutag;
  if(jets.size() >= 1){
    //if(useevent20)GetFakeRates(leps, blepsT, param, label, jets,  label+"_20",(event_weight),isocut);
    //if(useevent30)GetFakeRates(leps, blepsT,param, label, jets,   label+"_30",(event_weight),isocut);
    if(useevent40)GetFakeRates(leps, blepsT,param, label, jets,  label+"_40",(event_weight),isocut); //JH : MuMu_HNL_ULID_2017_V1_40
    //if(useevent60)GetFakeRates(leps, blepsT,param, label,jets,   label+"_60",(event_weight),isocut); //JH
  }
  return;
}


bool HNL_LeptonFakeRate::UseEvent(std::vector<Lepton *> leps ,  std::vector< Jet> jets, float awayjetcut, Particle MET, float wt){

  FillHist("UseEvent_Njet", jets.size(), wt, 10, 0, 10);
  FillHist("UseEvent_MET", MET.Pt() , wt, 200., 0, 2000);
  vector<TString> cutflows = {"Start","MET","MT","AwayJetPt","JetEMfrac","Jetdphi","PtRatio","Mu3","Mu8","Mu17"};
  FillHist("UseEvent_Cutflow", cutflows , "Start", wt);

  bool useevent = false;
  if(leps.size() != 1) return false;
  
  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int w = 0; w < leps.size();  w++){
    METdphi = TVector2::Phi_mpi_pi(leps.at(w)->Phi()-MET.Phi());
    MT = sqrt(2.* leps.at(w)->Et()*MET.Pt() * (1 - cos( METdphi)));

    if(MET.Pt() < 40) FillHist("UseEvent_Cutflow", cutflows , "MET", wt); 
    
    if(( (MET.Pt() < 40) && (MT < 25.)) ) {

      FillHist("UseEvent_Cutflow", cutflows , "MT", wt);
      vector<Jet> jetpt;
      vector<Jet> jetEMfrac;
      vector<Jet> jetdphi;
      vector<Jet> jetPtratio;
      for(unsigned int ij=0; ij < jets.size(); ij++){
        if(jets.at(ij).Pt() > awayjetcut) jetpt.push_back(jets.at(ij));
      }
      if(jetpt.size()>0){
        FillHist("UseEvent_Cutflow", cutflows , "AwayJetPt", wt);
        for(unsigned int ij=0; ij < jetpt.size(); ij++){
          if(jetpt.at(ij).ChargedEmEnergyFraction() < 0.65) jetEMfrac.push_back(jetpt.at(ij));
        }
      }
      if(jetEMfrac.size()>0){
        FillHist("UseEvent_Cutflow", cutflows , "JetEMfrac", wt);
        for(unsigned int ij=0; ij < jetEMfrac.size(); ij++){
          float dphi =fabs(TVector2::Phi_mpi_pi(leps.at(0)->Phi() - jetEMfrac.at(ij).Phi()));
          if(dphi > 2.5) jetdphi.push_back(jetEMfrac.at(ij));
        }
      }
      if(jetdphi.size()>0){
        FillHist("UseEvent_Cutflow", cutflows , "Jetdphi", wt);
        for(unsigned int ij=0; ij < jetdphi.size(); ij++){
          if(jetdphi.at(ij).Pt() / leps.at(0)->Pt() > 1.2) jetPtratio.push_back(jetdphi.at(ij));
        }
      }
      if(jetPtratio.size()>0){
        FillHist("UseEvent_Cutflow", cutflows , "PtRatio", wt);
      }
        

      for (unsigned int ielT=0; ielT < leps.size(); ielT++){
        for(unsigned int ij=0; ij < jets.size(); ij++){
          if(jets.at(ij).Pt() < awayjetcut) continue;
          float dphi =fabs(TVector2::Phi_mpi_pi(leps.at(ielT)->Phi()- jets.at(ij).Phi()));
          if( (jets.at(ij).ChargedEmEnergyFraction()) > 0.65)  continue;
          if(dphi > 2.5){
            if((jets.at(ij).Pt() /  leps.at(ielT)->Pt() ) < 1.2) continue;
            useevent = true;
          }
  }
      }
    }
  }

  return useevent;

}


void HNL_LeptonFakeRate::GetFakeRates(std::vector<Lepton *> leps,std::vector<bool> blepsT,  AnalyzerParameter param,TString tightlabel,  std::vector<Jet> jets,  TString tag,float event_weight, float isocut){
          
  bool IsMuon=(leps[0]->LeptonFlavour() == Lepton::MUON);
  int nbin_ptcone=  IsMuon ? 9 : 8;
  int nbin_pt    = IsMuon ? 11 : 11;
  int nbin_eta   = 4;
  
  double ptbinscone  [nbin_ptcone+1];
  double ptbins      [nbin_pt+1];
  double etabins   [nbin_eta+1   ] =  { 0.,0.8,  1.479, 2.,  2.5};
  if(IsMuon){
    vector<double> vptbinscone = {10., 15.,20.,25.,30.,35.,40.,50., 60.,100. } ;
    vector<double> vptbins =  {10., 15.,20.,25.,30.,35.,40.,50.,60.,100.,200.,1000.} ; 
    std::copy(vptbinscone.begin(), vptbinscone.end(), ptbinscone);
    std::copy(vptbins.begin(), vptbins.end(), ptbins);
  }
  else{
    vector<double> vptbinscone = { 10., 15.,23.,30.,35., 40.,50.,60.,100.};
    vector<double> vptbins     = { 10., 15.,20.,25.,30., 35.,40.,50.,60.,100.,200.,1000.};
    std::copy(vptbinscone.begin(), vptbinscone.end(), ptbinscone);
    std::copy(vptbins.begin(), vptbins.end(), ptbins);

  }
  Event ev = GetEvent();

  float lep_pt = leps[0]->Pt();
  //float lep_pt_corr =  leps[0]->CalcPtCone(leps[0]->RelIso(), isocut);
  float PtPartonUncorr;
  float PtPartonQCD;
  float PtPartonData;
  std::map<TString, double> PtPartonSFmap;
  PtPartonSFmap["HNL_ULID_2017_MVALoose"] = 0.734;
  PtPartonSFmap["HNL_ULID_2017_TriLep"] = 0.706;
  PtPartonSFmap["HNL_ULID_2017_Blt0p01"] = 0.555;
  PtPartonSFmap["HNL_ULID_2017_Blt0p015"] = 0.611;
  PtPartonSFmap["HNL_ULID_2017_Blt0p02"] = 0.659;
  PtPartonSFmap["HNL_ULID_2017_Blt0p025"] = 0.687;
  PtPartonSFmap["HNL_ULID_2017_Blt0p03"] = 0.702;
  PtPartonSFmap["HNL_ULID_2017_Blt0p035"] = 0.717;
  PtPartonSFmap["HNL_ULID_2017_Pgt0p45"] = 0.769;
  PtPartonSFmap["HNL_ULID_2017_Plt0p45"] = 0.308;
  PtPartonSFmap["HNL_ULID_2017_Blt0p01_Pgt0p45"] = 0.571;
  PtPartonSFmap["HNL_ULID_2017_Blt0p01_Plt0p45"] = 0.308;
  if(leps[0]->GetFlavour()=="Muon"){
    PtPartonUncorr = leps[0]->PtParton(1, 0.64, 0.64);
    PtPartonQCD = leps[0]->PtParton(PtPartonSFmap[param.Muon_Loose_ID], 0.64, 0.64);
  }
  else{
    PtPartonUncorr = leps[0]->PtParton(1, 0.15, 0.2); //FIXME only for 2017
  }
  float lep_eta    = fabs(leps[0]->Eta());
  float lep_reliso = leps[0]->RelIso();
  float lep_ip3d   = fabs(leps[0]->IP3D()/leps[0]->IP3Derr());
  float lep_mva    = leps[0]->LepMVA();
  float lep_dxy    = fabs(leps[0]->dXY());
  float lep_LFvsHF = leps[0]->HNL_MVA_Fake("QCD_LFvsHF_v5");
  
  //if(lep_pt > 60.) lep_pt = 59;
  float weight_ptcorr=event_weight;
  float weight_pt=event_weight;
  TString L_prefix = "Fake_Loose_"+tag;
  TString T_prefix = "Fake_Tight_"+tag; //JH : Fake_Tight_MuMu_HNL_ULID_2017_V1_40

  double prescale_lep (1.);

  bool fill_plot=false;


  if(IsMuon){
    TString triggerslist_3="HLT_Mu3_PFJet40_v";
    TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";
    TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";
    bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
    bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    vector<TString> cutflows = {"Start","MET","MT","AwayJetPt","JetEMfrac","Jetdphi","PtRatio","Mu3","Mu8","Mu17"};
    
    if(lep_pt < 5) return;
    else if(lep_pt <10){
      if(Mu3PD&&ev.PassTrigger(triggerslist_3)) { fill_plot=true; FillHist("UseEvent_Cutflow", cutflows , "Mu3", event_weight); if(!IsDATA)weight_pt = event_weight * ev.GetTriggerLumi(triggerslist_3);}
      else { fill_plot=false;weight_pt = 0.;  }
    }
    else  if(lep_pt < 20) {
      if(Mu8PD&&ev.PassTrigger(triggerslist_8)) { fill_plot=true; FillHist("UseEvent_Cutflow", cutflows , "Mu8", event_weight); if(!IsDATA)weight_pt= event_weight * ev.GetTriggerLumi(triggerslist_8); }
      else{ fill_plot=false;weight_pt = 0.;      }
    }
    else {
      if(Mu17PD && ev.PassTrigger(triggerslist_17)) { fill_plot=true; FillHist("UseEvent_Cutflow", cutflows , "Mu17", event_weight); if(!IsDATA) weight_pt= event_weight * ev.GetTriggerLumi(triggerslist_17);}
      else{ fill_plot=false;weight_pt = 0.;      }
    }
    
    /// Get precale weiht for normal pt bins
    prescale_lep = GetPrescale(leps); // JH : this is GetTriggerLumi(triggerlist) * Nvtx reweight
  } 
  else{
    
    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   /// -> tighter cut in lepton ID form tighter trigger emulation cut                    
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_17="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";

    bool El8PD=  (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool El12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    //    bool El17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));
    bool El23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));


    if(lep_pt < 10) return;
    else if(lep_pt < 15) {
      if(El8PD && ev.PassTrigger(triggerslist_8)) { fill_plot=true;if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_8);}
      else{fill_plot=false;weight_ptcorr = 0.;}
    }
    
    else if(lep_pt < 25) {
      
      if(El12PD&&ev.PassTrigger(triggerslist_12)) {  fill_plot=true;if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_12);}
      else{ fill_plot=false;weight_ptcorr = 0.;}
    }
    else{
      if(El23PD&&ev.PassTrigger(triggerslist_23)) {fill_plot=true;if(!IsDATA)weight_ptcorr = event_weight * ev.GetTriggerLumi(triggerslist_23);}
      else{fill_plot=false;    weight_ptcorr = 0.;}
    }// end ptbin check
    prescale_lep = GetPrescale(leps);
  }


  std::vector<Jet> jets_tmp     = GetJets   ( param, param.Jet_ID, 20., 5.);
  std::vector<FatJet> AK8_JetColl;
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
                   
  std::vector<Jet> BJetColl    = SelectBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )weight_pt*= sf_btag;

  vector<TString> LepTypeLabel;
  LepTypeLabel.push_back("IsPrompt");
  LepTypeLabel.push_back("IsFake");
  LepTypeLabel.push_back("IsEWtau");
  LepTypeLabel.push_back("IsConv");
  std::map<TString, TString> LepTypeLabelMap;
  LepTypeLabelMap["IsPrompt"] = "prompt";
  LepTypeLabelMap["IsFake"]   = "fake";
  LepTypeLabelMap["IsEWtau"]  = "tau";
  LepTypeLabelMap["IsConv"]   = "conv";

  for(int ilep = 0 ; ilep < 2; ilep++)  {

    TString prefix = (ilep==0) ? L_prefix : T_prefix;
    if(leps[0]->HNL_MVA_Fake("QCD_LFvsHF_v5") > 0) prefix = "LF_"+prefix;
    else prefix = "HF_"+prefix;
    if((ilep==1) && !(blepsT[0])) continue;
    
    if(fill_plot) {
      FillHist((prefix + "_reliso").Data(), lep_reliso, weight_pt, 50, 0., 1.);
      FillHist((prefix + "_dXY").Data(),    lep_dxy, weight_pt, 50, 0., 1.);
      FillHist((prefix + "_IP3D").Data(),   lep_ip3d, weight_pt, 50, 0., 10.);
      FillHist((prefix + "_mva").Data(),    lep_mva, weight_pt, 50, -1., 1.);
      FillHist((prefix + "_pt_eta").Data(), lep_pt, lep_eta,weight_pt, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_pt").Data(),     lep_pt, weight_pt, nbin_pt, ptbins, "p_{T} (GeV)");
      FillHist((prefix + "_eta").Data(),    lep_eta, weight_pt , nbin_eta, etabins,"#eta");
      if(BJetColl.size()==0){
        FillHist((prefix + "_0BJet_pt").Data(),     lep_pt, weight_pt, nbin_pt, ptbins, "p_{T} (GeV)");
        FillHist((prefix + "_0BJet_eta").Data(),    lep_eta, weight_pt , nbin_eta, etabins,"#eta");
      }
      else{
        FillHist((prefix + "_BJet_pt").Data(),     lep_pt, weight_pt, nbin_pt, ptbins, "p_{T} (GeV)");
        FillHist((prefix + "_BJet_eta").Data(),    lep_eta, weight_pt , nbin_eta, etabins,"#eta");
      }
      if(!IsDATA){
        for(unsigned int i=0; i<LepTypeLabel.size(); i++){
          if(leps[0]->LepGenTypeString()==LepTypeLabel[i]){
            FillHist((prefix + "_"+LepTypeLabelMap[LepTypeLabel[i]]+"_pt_eta").Data(), lep_pt, lep_eta,weight_pt, nbin_pt, ptbins, nbin_eta , etabins);
            FillHist((prefix + "_"+LepTypeLabelMap[LepTypeLabel[i]]+"_pt").Data(),     lep_pt, weight_pt, nbin_pt, ptbins, "p_{T} (GeV)");
            if(leps[0]->LepGenTypeString()=="IsFake"){
              if(leps[0]->CloseJet_Flavour().Contains("HF")){
                FillHist((prefix + "_fakeHF_pt_eta").Data(), lep_pt, lep_eta,weight_pt, nbin_pt, ptbins, nbin_eta , etabins);
                FillHist((prefix + "_fakeHF_pt").Data(),     lep_pt, weight_pt, nbin_pt, ptbins, "p_{T} (GeV)");
                FillHist((prefix + "_fakeHF_LFvsHF").Data(), lep_LFvsHF, weight_pt, 200, -1, 1);
              }
              else if(leps[0]->CloseJet_Flavour().Contains("LF")){
                FillHist((prefix + "_fakeLF_pt_eta").Data(), lep_pt, lep_eta,weight_pt, nbin_pt, ptbins, nbin_eta , etabins);
                FillHist((prefix + "_fakeLF_pt").Data(),     lep_pt, weight_pt, nbin_pt, ptbins, "p_{T} (GeV)");
                FillHist((prefix + "_fakeLF_LFvsHF").Data(), lep_LFvsHF, weight_pt, 200, -1, 1);
              }
            }
          }
        }
      }

/* TriLep method study
      FillProf((prefix + "_MVA_PtPartonUncorr").Data(), lep_mva, PtPartonUncorr, weight_ptcorr, 200, -1, 1);
      FillHist((prefix + "_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_PtPartonUncorr").Data(), PtPartonUncorr,  weight_ptcorr, nbin_pt, ptbins);
      FillProf((prefix + "_MVA_PtPartonQCD").Data(), lep_mva, PtPartonQCD, weight_ptcorr, 200, -1, 1);
      FillHist((prefix + "_PtPartonQCD_eta").Data(), PtPartonQCD, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
      FillHist((prefix + "_PtPartonQCD").Data(), PtPartonQCD,  weight_ptcorr, nbin_pt, ptbins);
      if(!IsDATA){
        if(leps[0]->LepGenTypeString()=="IsPrompt"){
          FillProf((prefix + "_prompt_MVA_PtPartonUncorr").Data(), lep_mva, PtPartonUncorr, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_prompt_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_prompt_PtPartonUncorr").Data(), PtPartonUncorr, weight_ptcorr, nbin_pt, ptbins);
          FillProf((prefix + "_prompt_MVA_PtPartonQCD").Data(), lep_mva, PtPartonQCD, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_prompt_PtPartonQCD_eta").Data(), PtPartonQCD, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_prompt_PtPartonQCD").Data(), PtPartonQCD, weight_ptcorr, nbin_pt, ptbins);
        }
        if(leps[0]->LepGenTypeString()=="IsFake"){
          FillProf((prefix + "_fake_MVA_PtPartonUncorr").Data(), lep_mva, PtPartonUncorr, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_fake_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_fake_PtPartonUncorr").Data(), PtPartonUncorr, weight_ptcorr, nbin_pt, ptbins);
          FillProf((prefix + "_fake_MVA_PtPartonQCD").Data(), lep_mva, PtPartonQCD, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_fake_PtPartonQCD_eta").Data(), PtPartonQCD, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_fake_PtPartonQCD").Data(), PtPartonQCD, weight_ptcorr, nbin_pt, ptbins);
          if(leps[0]->CloseJet_Flavour().Contains("HF")){
            FillProf((prefix + "_fakeHF_MVA_PtPartonUncorr").Data(), lep_mva, PtPartonUncorr, weight_ptcorr, 200, -1, 1);
            FillHist((prefix + "_fakeHF_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
            FillHist((prefix + "_fakeHF_PtPartonUncorr").Data(), PtPartonUncorr, weight_ptcorr, nbin_pt, ptbins);
            FillProf((prefix + "_fakeHF_MVA_PtPartonQCD").Data(), lep_mva, PtPartonQCD, weight_ptcorr, 200, -1, 1);
            FillHist((prefix + "_fakeHF_PtPartonQCD_eta").Data(), PtPartonQCD, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
            FillHist((prefix + "_fakeHF_PtPartonQCD").Data(), PtPartonQCD, weight_ptcorr, nbin_pt, ptbins);
          }
          else if(leps[0]->CloseJet_Flavour().Contains("LF")){
            FillProf((prefix + "_fakeLF_MVA_PtPartonUncorr").Data(), lep_mva, PtPartonUncorr, weight_ptcorr, 200, -1, 1);
            FillHist((prefix + "_fakeLF_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
            FillHist((prefix + "_fakeLF_PtPartonUncorr").Data(), PtPartonUncorr, weight_ptcorr, nbin_pt, ptbins);
            FillProf((prefix + "_fakeLF_MVA_PtPartonQCD").Data(), lep_mva, PtPartonQCD, weight_ptcorr, 200, -1, 1);
            FillHist((prefix + "_fakeLF_PtPartonQCD_eta").Data(), PtPartonQCD, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
            FillHist((prefix + "_fakeLF_PtPartonQCD").Data(), PtPartonQCD, weight_ptcorr, nbin_pt, ptbins);
          }
        }
        if(leps[0]->LepGenTypeString()=="IsEWtau"){
          FillProf((prefix + "_tau_MVA_PtPartonUncorr").Data(), lep_mva, PtPartonUncorr, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_tau_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_tau_PtPartonUncorr").Data(), PtPartonUncorr, weight_ptcorr, nbin_pt, ptbins);
          FillProf((prefix + "_tau_MVA_PtPartonQCD").Data(), lep_mva, PtPartonQCD, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_tau_PtPartonQCD_eta").Data(), PtPartonQCD, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_tau_PtPartonQCD").Data(), PtPartonQCD, weight_ptcorr, nbin_pt, ptbins);
        }
        if(leps[0]->LepGenTypeString()=="IsConv"){
          FillProf((prefix + "_conv_MVA_PtPartonUncorr").Data(), lep_mva, PtPartonUncorr, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_conv_PtPartonUncorr_eta").Data(), PtPartonUncorr, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_conv_PtPartonUncorr").Data(), PtPartonUncorr, weight_ptcorr, nbin_pt, ptbins);
          FillProf((prefix + "_conv_MVA_PtPartonQCD").Data(), lep_mva, PtPartonQCD, weight_ptcorr, 200, -1, 1);
          FillHist((prefix + "_conv_PtPartonQCD_eta").Data(), PtPartonQCD, lep_eta,  weight_ptcorr, nbin_pt, ptbins, nbin_eta , etabins);
          FillHist((prefix + "_conv_PtPartonQCD").Data(), PtPartonQCD, weight_ptcorr, nbin_pt, ptbins);
        }
      }
*/
    }
  }
  return;
}


HNL_LeptonFakeRate::HNL_LeptonFakeRate(){
  
}
 
HNL_LeptonFakeRate::~HNL_LeptonFakeRate(){

  //  delete NvtxSFFile;
  //for(std::map< TString, TH1D* >::iterator mapit = maphist_NvtxSF.begin(); mapit!=maphist_NvtxSF.end(); mapit++){
  //  delete mapit->second;
  // }
  //maphist_NvtxSF.clear();

}




void HNL_LeptonFakeRate::FillRegionPlots( TString plot_dir, TString region,  std::vector<Jet> jets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met,  double w){
  
  if((els.size()+ mus.size()) != 1) return;
  Lepton lep1;
  if(els.size()==1) lep1=els[0];
  else lep1 = mus[0];

  int nbin_pt    =11;
  double ptbins    [nbin_pt    +1] = { 0.,15.,20.,30.,35., 40.,50., 60., 80., 100.,200.,2000.};

  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_binned_pt", lep1.Pt()  , w, nbin_pt, ptbins,"p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_pt", lep1.Pt()  , w, 400, 0., 2000.,"p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_eta", lep1.Eta()  , w, 30, -3., 3,"#eta");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_CloseJet_Ptrel", lep1.CloseJet_Ptrel(), w, 50., 0., 2.,"PtRel");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_CloseJet_Ptratio", lep1.CloseJet_Ptratio(), w, 50., 0., 2.,"PtRel");

  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Met", met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MetPhi", met.Phi()  , w, 100, -5, 5,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/njets" , jets.size() , w, 5, 0., 5., "N_{jets}");

  float lep_reliso  = lep1.RelIso();
  float lep_minireliso  = lep1.MiniRelIso();
  float lep_ip3d    = fabs(lep1.IP3D()/lep1.IP3Derr());
  float lep_mva     =  lep1.LepMVA();
  float lep_dxy     = fabs(lep1.dXY());

  if(els.size() > 0)   FillHist( plot_dir +  "/RegionPlots_"+ region+ "/NMissingHits", els[0].NMissingHits(), w, 5, 0., 5.);


  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Reliso", lep_reliso, w, 50, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MiniReliso", lep_minireliso, w, 50, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/dXY",    lep_dxy, w, 100, 0., 0.5);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/IP3D",   lep_ip3d, w, 50, 0., 10.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Mva",    lep_mva, w, 50, -1., 1.);



  double lep_jet_ratio= 0.;
  double jet_CEEF = 0.;  double jet_CHEF = 0.;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(lep1.Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5){
      lep_jet_ratio = jets.at(ij).Pt() /  lep1.Pt();
      jet_CEEF = jets.at(ij).ChargedEmEnergyFraction();
      jet_CHEF = jets.at(ij).ChargedHadEnergyFraction();
    }
  }
  
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/JetLepPtRatio" , lep_jet_ratio  , w, 50, 0., 5.,"P^{jet}_{T} / P^{lepton}_{T}");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/AwayJetChargedEMEnergyFraction" , jet_CEEF  , w, 50, 0., 2.,"Jet charged EMF");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/AwayJetChargedHadEnergyFraction" , jet_CHEF  , w, 50, 0., 2.,"Jet charged EMF");
  


  double METdphi = TVector2::Phi_mpi_pi(lep1.Phi()-met.Phi());
  double MT = sqrt(2.* lep1.Et()*met.Pt() * (1 - cos( METdphi)));
  
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Mt" , MT  , w, 40, 0., 200.,"MET GeV");
  
}


void HNL_LeptonFakeRate::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){


  return;

}

void HNL_LeptonFakeRate::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNL_LeptonFakeRate::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

