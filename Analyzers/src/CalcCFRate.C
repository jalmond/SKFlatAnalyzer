#include "CalcCFRate.h"

void CalcCFRate::executeEvent(){


  //==============================
  //==== THIS IS FOR WR Analysis
  //==============================

  Electron_FakeRatePtBinnings = {40, 50, 75,     110, 150, 200, 250, 300, 350, 500, 1000, 1500, 2000};
  Muon_FakeRatePtBinnings     = {35, 45, 75, 80, 110, 150, 200, 250, 300, 350, 500, 1000, 1500, 2000};

  //======================
  //==== AnalyzerParamer
  //======================

  AnalyzerParameter param;

  param.Name = "HNWRTight";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNWRTight";
  param.Electron_Tight_RelIso = 0.15;
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNWRTight";
  param.Muon_Tight_RelIso = 0.15;
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  AllElectrons = GetAllElectrons();
  AllMuons = UseTunePMuon( GetAllMuons() );

  executeEventFromParameter(param);



  param.Name = "HNWRLoose";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNWRLoose";
  param.Electron_Tight_RelIso = 0.60;
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNWRLoose";
  param.Muon_Tight_RelIso = 0.60;
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

/*

  //=============================================
  //===== THIS IS FOR PairN Production Analysis
  //=============================================

  //==== 2D Plot Pt Binnings
  Electron_FakeRatePtBinnings = {40, 50, 75,     110, 150, 200, 250, 300, 350, 500, 1000, 1500, 2000};
  Muon_FakeRatePtBinnings     = {35, 45, 75, 80, 110, 150, 200, 250, 300, 350, 500, 1000, 1500, 2000};

  //======================
  //==== AnalyzerParamer
  //======================

  AnalyzerParameter param;

  //==== To Check Denominator distribution, remove all selections for loose lepton

  param.Name = "HNPairVeryLoose";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNPairTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_UseMini = true;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNPairTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_UseMini = true;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== HNPair Tight ID

  param.Name = "HNPairTight";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNPairTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_UseMini = true;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNPairTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_UseMini = true;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== HNPair Loose ID

  param.Name = "HNPairLoose";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNPairLoose";
  param.Electron_Tight_RelIso = 0.6;
  param.Electron_UseMini = true;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNPairLoose";
  param.Muon_Tight_RelIso = 0.6;
  param.Muon_UseMini = true;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);

  //==== HNPair ID, but change IP cut
  //==== Electron Loose : WIHTOUT IP cut
  //==== Muon Loose : WITH IP cut

  param.Name = "HNPair_LooseDifferentIP";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNPairLooseNoIP";
  param.Electron_Tight_RelIso = 0.6;
  param.Electron_UseMini = true;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "TEST";
  param.Muon_Tight_RelIso = 0.6;
  param.Muon_UseMini = true;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";

  executeEventFromParameter(param);
*/


}

void CalcCFRate::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  std::vector<Electron> Veto_electrons = GetElectrons(param.Electron_Veto_ID, 10., 2.5);
  std::vector<Muon> Veto_muons = GetMuons(param.Muon_Veto_ID, 10., 2.4);
  int n_Veto_Leptons = Veto_electrons.size()+Veto_muons.size();

  std::vector<Electron> Tight_electrons = GetElectrons(param.Electron_Tight_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon> Tight_muons = GetMuons(param.Muon_Tight_ID, param.Muon_MinPt, 2.4);

  vector<Gen> gens = GetGens();

  vector<Lepton *> lepptrs_electron = MakeLeptonPointerVector(Tight_electrons);
  vector<Lepton *> lepptrs_muon = MakeLeptonPointerVector(Tight_muons);
  vector<Lepton *> lepptrs;
  for(unsigned int i=0;i<lepptrs_electron.size();i++){
    lepptrs.push_back( lepptrs_electron.at(i) );
  }
  for(unsigned int i=0;i<lepptrs_muon.size();i++){
    lepptrs.push_back( lepptrs_muon.at(i) );
  }

  vector<Jet> jets = GetJets("HN", 20., 2.7);

  //==== Find prompt and fake leptons
  //==== Also save IsCF

  if(!IsDATA){

    for(unsigned int i=0; i<lepptrs.size(); i++){

      Lepton *lepptr = lepptrs.at(i);
      Lepton lep = *lepptr;
      bool IsElectron = (lep.LeptonFlavour()==Lepton::ELECTRON);
      double this_Tight_RelIso = param.Electron_Tight_RelIso;
      bool this_UseMini = param.Electron_UseMini;


      TString str_lepflv = "Electron";
      TString this_etaregion = "InnerBarrel";
      double this_eta = fabs(lep.Eta());
      if(IsElectron){
         Electron *elptr = (Electron *)lepptr;
        this_eta = fabs(elptr->scEta());
      }
      else{
        str_lepflv = "Muon";
        this_Tight_RelIso = param.Muon_Tight_RelIso;
        this_UseMini = param.Muon_UseMini;
      }

      if(this_eta<0.8){
        this_etaregion = "InnerBarrel";
      }
      else if(this_eta<1.479){
        this_etaregion = "OuterBarrel";
      }
      else{
        this_etaregion = "EndCap";
      }

      double ThisRelIso = lep.RelIso();
      if(this_UseMini) ThisRelIso = lep.MiniRelIso();

      double ptcone = lep.CalcPtCone(ThisRelIso, this_Tight_RelIso);
      lepptr->SetPtCone( ptcone );

      Gen genlep = GetGenMatchedLepton(lep, gens);
      int lep_tyep = GetLeptonType(lep, gens);

      //==== Only prompt
      if(lep_tyep<=0) continue;

      bool IsCF = lep.Charge()!=genlep.Charge();

      FillCFRatePlots(str_lepflv+"_"+param.Name, lepptr, IsCF, 1.);

      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+"Pt", lep.Pt(), 1., 2000, 0., 2000.);
      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+"InvPt", 1./lep.Pt(), 1., 100, 0., 0.1);
      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+"GenPt", genlep.Pt(), 1., 2000, 0., 2000.);
      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+"InvGenPt", 1./genlep.Pt(), 1., 100, 0., 0.1);
      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+this_etaregion+"_Pt", lep.Pt(), 1., 2000, 0., 2000.);
      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+this_etaregion+"_InvPt", 1./lep.Pt(), 1., 100, 0., 0.1);
      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+this_etaregion+"_GenPt", genlep.Pt(), 1., 2000, 0., 2000.);
      JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Den_"+this_etaregion+"_InvGenPt", 1./genlep.Pt(), 1., 100, 0., 0.1);
      if(IsCF){
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+"Pt", lep.Pt(), 1., 2000, 0., 2000.);
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+"InvPt", 1./lep.Pt(), 1., 100, 0., 0.1);
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+"GenPt", genlep.Pt(), 1., 2000, 0., 2000.);
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+"InvGenPt", 1./genlep.Pt(), 1., 100, 0., 0.1);
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+this_etaregion+"_Pt", lep.Pt(), 1., 2000, 0., 2000.);
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+this_etaregion+"_InvPt", 1./lep.Pt(), 1., 100, 0., 0.1);
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+this_etaregion+"_GenPt", genlep.Pt(), 1., 2000, 0., 2000.);
        JSFillHist(str_lepflv+"_"+param.Name, str_lepflv+"_"+param.Name+"_"+"Num_"+this_etaregion+"_InvGenPt", 1./genlep.Pt(), 1., 100, 0., 0.1);
      }

    }

  }

}


void CalcCFRate::FillCFRatePlots(TString name, Lepton *lep, bool IsCF, double weight){

  bool IsElectron = true;
  vector<double> vec_ptbins = Electron_FakeRatePtBinnings;
  double ThisEta = -999;
  if(lep->LeptonFlavour()==Lepton::ELECTRON){

    //==== Electron-only plots
    Electron *el = (Electron *)lep;
    JSFillHist(name, name+"_"+"Den_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    if(IsCF){
      JSFillHist(name, name+"_"+"Num_"+"MVANoIso", el->MVANoIso(), weight, 200, -1., 1.);
    }

    ThisEta = el->scEta();

  }
  else{
    IsElectron = false;
    vec_ptbins = Muon_FakeRatePtBinnings;

    //==== Muon-only plots
    Muon *mu = (Muon *)lep;

    JSFillHist(name, name+"_"+"Den_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
    JSFillHist(name, name+"_"+"Den_"+"TrkRelIso", mu->TrkIso()/mu->TuneP4().Pt(), weight, 100, 0., 1.0);
    if(IsCF){
      JSFillHist(name, name+"_"+"Num_"+"Chi2", mu->Chi2(), weight, 500, 0., 50.);
      JSFillHist(name, name+"_"+"Num_"+"TrkRelIso", mu->TrkIso()/mu->TuneP4().Pt(), weight, 100, 0., 1.0);
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

  JSFillHist(name, name+"_"+"Den_"+"Eta", ThisEta, weight, 60, -3, 3.);
  JSFillHist(name, name+"_"+"Den_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+"Den_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
  JSFillHist(name, name+"_"+"Den_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+"Den_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+"Den_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+"Den_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+"Den_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
  JSFillHist(name, name+"_"+"Den_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
  JSFillHist(name, name+"_"+"Den_"+"NEvent", 0., weight, 1, 0., 1.);

  if(IsCF){

    JSFillHist(name, name+"_"+"Num_"+"Eta", ThisEta, weight, 60, -3, 3.);
    JSFillHist(name, name+"_"+"Num_"+"RelIso", lep->RelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+"Num_"+"MiniRelIso", lep->MiniRelIso(), weight, 100, 0., 1.0);
    JSFillHist(name, name+"_"+"Num_"+"dXY", fabs(lep->dXY()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+"Num_"+"dXYSig", fabs(lep->dXY()/lep->dXYerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+"Num_"+"dZ", fabs(lep->dZ()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+"Num_"+"dZSig", fabs(lep->dZ()/lep->dZerr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+"Num_"+"IP3D", fabs(lep->IP3D()), weight, 500, 0., 0.5);
    JSFillHist(name, name+"_"+"Num_"+"IP3DSig", fabs(lep->IP3D()/lep->IP3Derr()), weight, 100, 0., 10);
    JSFillHist(name, name+"_"+"Num_"+"NEvent", 0., weight, 1, 0., 1.);

  }

}














