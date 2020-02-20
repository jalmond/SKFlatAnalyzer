#include "CalcFakeRate.h"

void CalcFakeRate::executeEvent(){

  //==============================
  //==== THIS IS FOR WR Analysis
  //==============================

  Electron_TrigWithPtRange.PtValues          = {10,                                        15,                                             25,                                     999999};
  Electron_TrigWithPtRange.Triggers          = {"HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v", "HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v"};
  Electron_TrigWithPtRange.IsDATA = IsDATA;
  Electron_TrigWithPtRange.DataStream = DataStream;
  Electron_TrigWithPtRange.TriggerSafePtCuts = {10,                                        15,                                             25};
  Electron_TrigWithPtRange.Validate();

  Muon_TrigWithPtRange.PtValues          = {10,        20,          25,            30,           55,      999999};
  Muon_TrigWithPtRange.Triggers          = {"HLT_Mu8_v", "HLT_Mu17_v", "HLT_Mu20_v", "HLT_Mu27_v", "HLT_Mu50_v"};
  Muon_TrigWithPtRange.IsDATA = IsDATA;
  Muon_TrigWithPtRange.DataStream = DataStream;
                                          // Double        Double        Single       Single         Single
  Muon_TrigWithPtRange.TriggerSafePtCuts = {10,        20,          25,            30,           55};
  Muon_TrigWithPtRange.Validate();

  //==== 2D Plot Pt Binnings
  Electron_FakeRatePtBinnings = {10,15,20,25,35,40,50,60,100,200,500,1000};
  Muon_FakeRatePtBinnings     = {10,15,20,25,30,40,55,60,100,200,500,1000};

  //==== Prefire weight

  weight_Prefire = GetPrefireWeight(0);

  //======================
  //==== AnalyzerParamer
  //======================

  AnalyzerParameter param;

  param.Name = "HNWR";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNWRTight";
  param.Electron_Tight_RelIso = 0.15;
  param.Electron_Loose_ID = "HNWRLoose";
  param.Electron_Loose_RelIso = 0.6;
  param.Electron_Veto_ID = "HNWRVeto";
  param.Electron_Veto_RelIso = 0.6;
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_ID_SF_Key = "Default";
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNWRTight";
  param.Muon_Tight_RelIso = 0.10;
  param.Muon_Loose_ID = "HNWRLoose";
  param.Muon_Loose_RelIso = 0.40;
  param.Muon_Veto_ID = "HNWRVeto";
  param.Muon_Veto_RelIso = 0.60;
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_UseTuneP = true;
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  AllElectrons = GetAllElectrons();
  AllMuons = UseTunePMuon( GetAllMuons() );

  executeEventFromParameter(param);


/*
  //=============================================
  //===== THIS IS FOR PairN Production Analysis
  //=============================================

  //==============================
  //==== Trigger Pt(cone) ranges
  //==============================

  Electron_TrigWithPtRange.PtValues          = {40,                50,            75,              110,              150,              200,               250,                300,              350,               999999};
  Electron_TrigWithPtRange.Triggers          = {"HLT_Photon25_v", "HLT_Photon33_v", "HLT_Photon50_v", "HLT_Photon75_v", "HLT_Photon90_v", "HLT_Photon120_v", "HLT_Photon150_v", "HLT_Photon175_v", "HLT_Photon200_v"};
  Electron_TrigWithPtRange.TriggerSafePtCuts = {28,                35,            55,               80,              100,              140,               170,                200,              250};
  Electron_TrigWithPtRange.IsDATA = IsDATA;
  Electron_TrigWithPtRange.DataStream = DataStream;
  Electron_TrigWithPtRange.Validate();


  Muon_TrigWithPtRange.PtValues          = {35,           45,           80,        999999};
  Muon_TrigWithPtRange.Triggers          = {"HLT_Mu20_v", "HLT_Mu27_v","HLT_Mu50_v"};
  Muon_TrigWithPtRange.IsDATA = IsDATA;
  Muon_TrigWithPtRange.DataStream = DataStream;
  Muon_TrigWithPtRange.TriggerSafePtCuts = {23,           30,           55};
  Muon_TrigWithPtRange.Validate();

  //==== 2D Plot Pt Binnings
  Electron_FakeRatePtBinnings = {40, 50, 75,     110, 150, 200, 250, 300, 350, 500, 1000, 1500, 2000};
  Muon_FakeRatePtBinnings     = {35, 45, 75, 80, 110, 150, 200, 250, 300, 350, 500, 1000, 1500, 2000};

  //======================
  //==== AnalyzerParamer
  //======================

  AnalyzerParameter param;

  param.Name = "HNPair";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNPairTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "HNPairLoose";
  param.Electron_Loose_RelIso = 0.6;
  param.Electron_Veto_ID = "HNPairVeto";
  param.Electron_Veto_RelIso = 0.6;
  param.Electron_UseMini = true;
  param.Electron_UsePtCone = true;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNPairTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "HNPairLoose";
  param.Muon_Loose_RelIso = 0.6;
  param.Muon_Veto_ID = "HNPairVeto";
  param.Muon_Veto_RelIso = 0.6;
  param.Muon_UseMini = true;
  param.Muon_UsePtCone = true;
  param.Muon_UseTuneP = false;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== HNPair ID with pt cut

  param.Name = "HNPair_PtCut";
  param.Electron_MinPt = 75.; // HLT_DoublePhoton70_v
  param.Muon_MinPt = 55.; // HLT_Mu50_v

  executeEventFromParameter(param);
*/
}

void CalcFakeRate::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  std::vector<Electron> Veto_electrons = SelectElectrons(AllElectrons, param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon> Veto_muons = SelectMuons(AllMuons, param.Muon_Veto_ID, 10., 2.4);
  int n_Veto_Leptons = Veto_electrons.size()+Veto_muons.size();

  std::vector<Electron> Loose_electrons = SelectElectrons(AllElectrons, param.Electron_Loose_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon> Loose_muons = SelectMuons(AllMuons, param.Muon_Loose_ID, param.Muon_MinPt, 2.4);
  //==== Sorting
  std::sort(Loose_electrons.begin(), Loose_electrons.end(), PtComparing);
  std::sort(Loose_muons.begin(), Loose_muons.end(), PtComparing);

  vector<Gen> gens = GetGens();

  vector<Lepton *> lepptrs_electron = MakeLeptonPointerVector(Loose_electrons);
  vector<Lepton *> lepptrs_muon = MakeLeptonPointerVector(Loose_muons);
  vector<Lepton *> lepptrs;
  for(unsigned int i=0;i<lepptrs_electron.size();i++){
    lepptrs.push_back( lepptrs_electron.at(i) );
  }
  for(unsigned int i=0;i<lepptrs_muon.size();i++){
    lepptrs.push_back( lepptrs_muon.at(i) );
  }

  vector<Jet> jets = GetJets("HN", 20., 2.4);
  std::sort(jets.begin(), jets.end(), PtComparing);
  vector<double> AwayJetMinPts = {20, 30, 40, 60, 100, 500};
  vector<TString> str_AwayJetMinPts = {"20", "30", "40", "60", "100", "500"};

  //==== Find prompt and fake leptons
  //==== Also save IsTight

  vector<Lepton *> lepptrs_prompt, lepptrs_fake;
  vector<bool>     IsTight_prompt, IsTight_fake;
  int n_TightElectron(0), n_TightMuon(0);
  double SF_Muon(1.), SF_Electron(1.);

  for(unsigned int i=0; i<lepptrs.size(); i++){

    Lepton *lepptr = lepptrs.at(i);
    Lepton lep = *lepptr;
    bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);
    double this_Tight_RelIso = param.Electron_Tight_RelIso;
    bool this_UseMini = param.Electron_UseMini;

    if(IsElectron){

    }
    else{
      this_Tight_RelIso = param.Muon_Tight_RelIso;
      this_UseMini = param.Muon_UseMini;
    }

    double ThisRelIso = lep.RelIso();
    if(this_UseMini) ThisRelIso = lep.MiniRelIso();

    double ptcone = lep.CalcPtCone(ThisRelIso, this_Tight_RelIso);
    lepptr->SetPtCone( ptcone );

    int lep_tyep=0;
    if(IsDATA){
      lep_tyep = 1;
    }
    else{
      lep_tyep = GetLeptonType(lep, gens);
    }

    bool PassTight = false;
    if(IsElectron){
      Electron *el = (Electron *)lepptr;
      PassTight = el->PassID(param.Electron_Tight_ID);
      if(PassTight){
        n_TightElectron++;
        if(!IsDATA){
          double this_recosf = mcCorr->ElectronReco_SF(el->scEta(),el->Pt());
          double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, el->scEta(), el->Pt());
          SF_Electron *= this_recosf*this_idsf;
        }
      }
    }
    else{
      Muon *mu = (Muon *)lepptr;
      PassTight = mu->PassID(param.Muon_Tight_ID);
      if(PassTight){
        n_TightMuon++;
        if(!IsDATA){
          double this_pt = mu->MiniAODPt();
          double this_eta = mu->Eta();

          if(param.Muon_UseTuneP){
            Particle this_tuneP = mu->TuneP4();
            this_pt = mu->Pt();
            this_eta = mu->Eta();
          }
          double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  this_eta, this_pt);
          double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, this_eta, this_pt);
          SF_Muon *= this_idsf*this_isosf;
        }
      }
    }

    if(lep_tyep>0){
      lepptrs_prompt.push_back( lepptr );
      IsTight_prompt.push_back( PassTight );
    }
    else if(lep_tyep<0){
      lepptrs_fake.push_back( lepptr );
      IsTight_fake.push_back( PassTight );
    }

  }

  //==== SingleLepton Trigger Norm Plots at Z-peak

  bool RunElectronSingleLeptonNormPlots = false;
  bool RunMuonSingleLeptonNormPlots = false;
  if(IsDATA){
    if(DataStream=="SingleMuon"||DataStream=="DoubleMuon")   RunMuonSingleLeptonNormPlots = true;
    if(DataStream=="SinglePhoton"||DataStream=="SingleElectron"||DataStream=="DoubleEG") RunElectronSingleLeptonNormPlots = true;
  }
  else{
    RunElectronSingleLeptonNormPlots = true;
    RunMuonSingleLeptonNormPlots = true;
  }

  if(RunElectronSingleLeptonNormPlots){

    bool OneLeptonEvent = (n_TightElectron==1) && (Loose_electrons.size()==1) && (n_Veto_Leptons==1);
    bool TwoLeptonEvent = (n_TightElectron==2) && (Loose_electrons.size()==2) && (n_Veto_Leptons==2);

    for(unsigned int i=0; i<Electron_TrigWithPtRange.Triggers.size(); i++){
      TString this_trigger = Electron_TrigWithPtRange.Triggers.at(i);

      if(PassTrigger(ev, this_trigger)){

        double weight = 1.;
        if(!IsDATA){
          weight *= weight_norm_1invpb * ev.GetTriggerLumi(this_trigger) * ev.MCweight() * SF_Electron * weight_Prefire;
        }

        if(OneLeptonEvent){
          double thisMT = MT( METv, Loose_electrons.at(0) );

          if( (METv.Pt() > 40.) && (Loose_electrons.at(0).Pt() > Electron_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_electrons.at(0).Pt() > 20.) ){
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"Lepton_0_Pt", Loose_electrons.at(0).Pt(), weight, 1000, 0., 1000.);
          }

          if( (METv.Pt() > 40.) && (thisMT > 50) && (Loose_electrons.at(0).Pt() > Electron_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_electrons.at(0).Pt() > 20.) ){
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"Lepton_0_Pt", Loose_electrons.at(0).Pt(), weight, 1000, 0., 1000.);
          }

        }
        if(TwoLeptonEvent){
          double dilepmass = (Loose_electrons.at(0)+Loose_electrons.at(1)).M();
          bool OnZEvent = IsOnZ( dilepmass, 15. );
          if( OnZEvent && (Loose_electrons.at(0).Pt() > Electron_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_electrons.at(1).Pt() > 20.) ){
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Z_Mass", dilepmass, weight, 40, 70., 110.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_0_Pt", Loose_electrons.at(0).Pt(), weight, 1000, 0., 1000.);
            JSFillHist("Electron_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_1_Pt", Loose_electrons.at(1).Pt(), weight, 1000, 0., 1000.);
          }
        }


      }

    } // END Loop Trigger

  }
  if(RunMuonSingleLeptonNormPlots){

    bool OneLeptonEvent = (n_TightMuon==1) && (Loose_muons.size()==1) && (n_Veto_Leptons==1);
    bool TwoLeptonEvent = (n_TightMuon==2) && (Loose_muons.size()==2) && (n_Veto_Leptons==2);

    for(unsigned int i=0; i<Muon_TrigWithPtRange.Triggers.size(); i++){
      TString this_trigger = Muon_TrigWithPtRange.Triggers.at(i);

      if(IsDATA){
        if(this_trigger=="HLT_Mu8_v" || this_trigger=="HLT_Mu17_v"){
          if(DataStream!="DoubleMuon") continue;
        }
        else if(this_trigger=="HLT_Mu20_v" || this_trigger=="HLT_Mu27_v" || this_trigger=="HLT_Mu50_v"){
          if(DataStream!="SingleMuon") continue;
        }
      }

      if(PassTrigger(ev, this_trigger)){

        double weight = 1.;
        if(!IsDATA){
          weight *= weight_norm_1invpb * ev.GetTriggerLumi(this_trigger) * ev.MCweight() * SF_Muon * weight_Prefire;
        }

        if(OneLeptonEvent){
          double thisMT = MT( METv, Loose_muons.at(0) );

          if( (METv.Pt() > 40.) && (Loose_muons.at(0).Pt() > Muon_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_muons.at(0).Pt() > 20.) ){
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"W_CR"+"_"+"Lepton_0_Pt", Loose_muons.at(0).Pt(), weight, 1000, 0., 1000.);
          }

          if( (METv.Pt() > 40.) && (thisMT > 50) && (Loose_muons.at(0).Pt() > Muon_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_muons.at(0).Pt() > 20.) ){
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"MET", METv.Pt(), weight, 500, 0., 500.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"MT", thisMT, weight, 500, 0., 500.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"MTcut_W_CR"+"_"+"Lepton_0_Pt", Loose_muons.at(0).Pt(), weight, 1000, 0., 1000.);
          }

        }
        if(TwoLeptonEvent){
          double dilepmass = (Loose_muons.at(0)+Loose_muons.at(1)).M();
          bool OnZEvent = IsOnZ( dilepmass, 15. );
          if( OnZEvent && (Loose_muons.at(0).Pt() > Muon_TrigWithPtRange.TriggerSafePtCuts.at(i) ) && (Loose_muons.at(1).Pt() > 20.) ){
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Z_Mass", dilepmass, weight, 40, 70., 110.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_0_Pt", Loose_muons.at(0).Pt(), weight, 1000, 0., 1000.);
            JSFillHist("Muon_"+param.Name, param.Name+"_TriggerNorm_"+this_trigger+"_"+"Z_CR"+"_"+"Lepton_1_Pt", Loose_muons.at(1).Pt(), weight, 1000, 0., 1000.);
          }
        }


      }

    } // END Loop Trigger

  }

  //==== MC Fake rate

  if(!IsDATA){

    for(unsigned int i=0; i<lepptrs_fake.size(); i++){

      Lepton *lepptr_fake = lepptrs_fake.at(i);
      Lepton lep = *lepptr_fake;
      TString str_lepflv = "Electron";
      bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);

      if(IsElectron){

      }
      else{
        str_lepflv = "Muon";
      }

      //==== MC Fake Rate
      FillFakeRatePlots(str_lepflv+"_"+param.Name, "MCFR", lepptrs_fake.at(i), IsTight_fake.at(i), 1.);

    }

  }

  //==== FR from Data
  //==== For Data, use all leptons
  //==== For MC, pick up prompt leptons 

  if(lepptrs_prompt.size()==1 && n_Veto_Leptons==1){ //TODO add veto

    Lepton *lepptr_prompt = lepptrs_prompt.at(0);
    Lepton lep = *lepptr_prompt;
    TString str_lepflv = "Electron";

    bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);
    if(!IsElectron) str_lepflv = "Muon";

    //===============================
    //==== Prepare event selections
    //===============================

    //==== 1) Trigger Pass

    bool PassTriggerByPt = false;
    TString ThisPtTrigger = "";
    double this_SF = 1.;
    if(IsElectron){

      double this_pt = lepptr_prompt->Pt();
      if(param.Electron_UsePtCone) this_pt = lepptr_prompt->PtCone();

      ThisPtTrigger = Electron_TrigWithPtRange.GetTriggerFromPt(this_pt);
      if(ThisPtTrigger!="PTFAIL"){
        PassTriggerByPt = PassTrigger(ev, ThisPtTrigger);
      }
      this_SF = SF_Electron;
    }
    else{

      double this_pt = lepptr_prompt->Pt();
      if(param.Muon_UsePtCone) this_pt = lepptr_prompt->PtCone();

      ThisPtTrigger = Muon_TrigWithPtRange.GetTriggerFromPt(this_pt);
      if(ThisPtTrigger!="PTFAIL"){
        PassTriggerByPt = PassTrigger(ev, ThisPtTrigger);
      }
      this_SF = SF_Muon;
    }
    if(ThisPtTrigger=="NULL") PassTriggerByPt = false;

    if(PassTriggerByPt){

      double weight = 1.;
      if(!IsDATA){
        weight *= weight_norm_1invpb * ev.GetTriggerLumi(ThisPtTrigger) * ev.MCweight() * this_SF * weight_Prefire;
      }

      //TODO add Scale factors

      //==== 2) Loop over AwayJet pT here

      double thisMT = MT( METv, lep );

      for(unsigned int it_jpt=0; it_jpt<AwayJetMinPts.size(); it_jpt++){
        double ThisMinJetPt = AwayJetMinPts.at(it_jpt);

        for(unsigned int it_jet=0; it_jet<jets.size(); it_jet++){

          Jet jet = jets.at(it_jet);
          if(!( jet.Pt() > ThisMinJetPt )) continue;
          double dPhi = fabs( lep.DeltaPhi(jet) );

          bool UseEvent = (dPhi > 2.5) && (jet.Pt()/lep.Pt() > 1.0);
/*
          cout << "dPhi = " << dPhi << endl;
          cout << "jet.Pt()/lep.Pt() = " << jet.Pt()/lep.Pt() << endl;
          cout << "METv.Pt() = " << METv.Pt() << endl;
          cout << "thisMT = " << thisMT << endl;
          cout << "--> UseEvent = " << UseEvent << endl;
*/
          if(UseEvent){
            FillFakeRatePlots(str_lepflv+"_"+param.Name, "DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt), lepptr_prompt, IsTight_prompt.at(0), weight);

            JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Den_"+"dPhi", dPhi, weight, 40, 0., 4.);
            JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Den_"+"JetPtOverLeptonPt", jet.Pt()/lep.Pt(), weight, 20, 0., 2.);
            JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Den_"+"MET", METv.Pt(), weight, 100, 0., 100.);
            JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Den_"+"MT", thisMT, weight, 50, 0., 50.);
            if(IsTight_prompt.at(0)){
              JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Num_"+"dPhi", dPhi, weight, 40, 0., 4.);
              JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Num_"+"JetPtOverLeptonPt", jet.Pt()/lep.Pt(), weight, 20, 0., 2.);
              JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Num_"+"MET", METv.Pt(), weight, 100, 0., 100.);
              JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"DATA_AwayJetPt"+str_AwayJetMinPts.at(it_jpt)+"_"+"Num_"+"MT", thisMT, weight, 50, 0., 50.);
            }

            break;
          }

        }

      }


    }

  }

}

void CalcFakeRate::FillFakeRatePlots(TString name, TString frtype, Lepton *lep, bool IsTight, double weight){

  bool IsElectron = true;
  vector<double> vec_ptbins = Electron_FakeRatePtBinnings;
  double ThisEta = -999;
  if(lep->LeptonFlavour()==Lepton::ELECTRON){

    //==== Electron-only plots
    Electron *el = (Electron *)lep;
    JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    if(IsTight){
      JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    }

    ThisEta = el->scEta();

  }
  else{
    IsElectron = false;
    vec_ptbins = Muon_FakeRatePtBinnings;

    //==== Muon-only plots
    Muon *mu = (Muon *)lep;
    JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
    JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"TrkRelIso", mu->TrkIso()/mu->TuneP4().Pt(), weight, 100, 0., 1.0);
    if(IsTight){
      JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
      JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"TrkRelIso", mu->TrkIso()/mu->TuneP4().Pt(), weight, 100, 0., 1.0);
    }

    ThisEta = mu->Eta();

  }

  const int n_ptbins = vec_ptbins.size()-1;
  double ptbins[n_ptbins+1];
  for(int i=0;i<n_ptbins+1;i++){
    ptbins[i] = vec_ptbins.at(i);
  }
  const int n_etabins = 3;
  double etabins[n_etabins+1] = {0., 0.8, 1.479, 2.5};
  if(!IsElectron) etabins[n_etabins] = 2.4;

  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"Eta", ThisEta, weight, 60, -3, 3.);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"NEvent", 0., weight, 1, 0., 1.);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"Pt_vs_Eta", lep->Pt(), fabs(ThisEta), weight, n_ptbins, ptbins, n_etabins, etabins);
  JSFillHist(name, name+"_"+frtype+"_"+"Den_"+"PtCone_vs_Eta", lep->PtCone(), fabs(ThisEta), weight, n_ptbins, ptbins, n_etabins, etabins);

  if(IsTight){

    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"Pt", lep->Pt(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"PtCone", lep->PtCone(), weight, 2000, 0., 2000.);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"Eta", ThisEta, weight, 60, -3, 3.);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"NEvent", 0., weight, 1, 0., 1.);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"Pt_vs_Eta", lep->Pt(), fabs(ThisEta), weight, n_ptbins, ptbins, n_etabins, etabins);
    JSFillHist(name, name+"_"+frtype+"_"+"Num_"+"PtCone_vs_Eta", lep->PtCone(), fabs(ThisEta), weight, n_ptbins, ptbins, n_etabins, etabins);

  }

}


bool CalcFakeRate::PassTrigger(Event ev, TString trg){

  if(DataYear==2016 && trg=="HLT_Mu50_v"){
    return ev.PassTrigger("HLT_Mu50_v") || ev.PassTrigger("HLT_TkMu50_v");
  }
  else{
    return ev.PassTrigger(trg);
  }

}












