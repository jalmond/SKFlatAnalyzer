#include "HNL_LeptonCore.h"

void HNL_LeptonCore::SetupEventMVAReader(bool ee, bool mm, bool emu){

  InitializeTreeVars();

  TString AnalyzerPath=std::getenv("SKFlat_WD");
  TString MVAPath = "/data/Run2UltraLegacy_v3/Run2/BDTClassifier/results_xml/";
  MNStrList = {"90", "100", "150", "200", "300", "400","500"};
  NCutList  = {"300"};
  NTreeList = {"850"};


  if(mm){
    MVAReaderMM->AddVariable("Nvbfj", &ev_bdt_Nvbfj);
    MVAReaderMM->AddVariable("Ptl1", &ev_bdt_Ptl1);
    MVAReaderMM->AddVariable("Ptl2", &ev_bdt_Ptl2);
    MVAReaderMM->AddVariable("Ptj1", &ev_bdt_Ptj1);
    MVAReaderMM->AddVariable("Ptj2", &ev_bdt_Ptj2);
    MVAReaderMM->AddVariable("MET2ST", &ev_bdt_MET2ST);
    MVAReaderMM->AddVariable("HTLT1", &ev_bdt_HTLT1);
    MVAReaderMM->AddVariable("HTLT2", &ev_bdt_HTLT2);
    MVAReaderMM->AddVariable("Etal1", &ev_bdt_Etal1);
    MVAReaderMM->AddVariable("Etal2", &ev_bdt_Etal2);
    MVAReaderMM->AddVariable("PtRatioAJl1", &ev_bdt_PtRatioAJl1);
    MVAReaderMM->AddVariable("PtRatioAJl2", &ev_bdt_PtRatioAJl2);
    MVAReaderMM->AddVariable("NEMFracAJl1", &ev_bdt_NEMFracAJl1);
    MVAReaderMM->AddVariable("NEMFracAJl2", &ev_bdt_NEMFracAJl2);
    MVAReaderMM->AddVariable("CHFracAJl1", &ev_bdt_CHFracAJl1);
    MVAReaderMM->AddVariable("CHFracAJl2", &ev_bdt_CHFracAJl2);
    MVAReaderMM->AddVariable("NHFracAJl1", &ev_bdt_NHFracAJl1);
    MVAReaderMM->AddVariable("NHFracAJl2", &ev_bdt_NHFracAJl2);
    MVAReaderMM->AddVariable("JetDiscAJl1", &ev_bdt_JetDiscAJl1);
    MVAReaderMM->AddVariable("JetDiscAJl2", &ev_bdt_JetDiscAJl2);
    MVAReaderMM->AddVariable("dRll", &ev_bdt_dRll);
    MVAReaderMM->AddVariable("dRlj11", &ev_bdt_dRlj11);
    MVAReaderMM->AddVariable("dRlj21", &ev_bdt_dRlj21);
    MVAReaderMM->AddVariable("MSSSF", &ev_bdt_MSSSF);
    MVAReaderMM->AddVariable("Mlj11", &ev_bdt_Mlj11);
    MVAReaderMM->AddVariable("Mlj21", &ev_bdt_Mlj21);
    MVAReaderMM->AddVariable("Mlj22", &ev_bdt_Mlj22);
    MVAReaderMM->AddVariable("MTvl1", &ev_bdt_MTvl1);
    MVAReaderMM->AddVariable("MTvl2", &ev_bdt_MTvl2);
    MVAReaderMM->AddVariable("Mllj1", &ev_bdt_Mllj1);
    MVAReaderMM->AddVariable("dRWjj", &ev_bdt_dRWjj);
    MVAReaderMM->AddVariable("M_W2_jj", &ev_bdt_M_W2_jj);
    MVAReaderMM->AddVariable("M_N2_l2jj", &ev_bdt_M_N2_l2jj);
    MVAReaderMM->AddVariable("Nb", &ev_bdt_Nb);
    MVAReaderMM->AddVariable("MuFracAJl1", &ev_bdt_MuFracAJl1);
    MVAReaderMM->AddVariable("M_N1_l1jj", &ev_bdt_M_N1_l1jj);
    MVAReaderMM->AddSpectator("w_tot", &w_tot);
  }


  if(ee){
    MVAReaderEE->AddVariable("Nvbfj", &ev_bdt_Nvbfj);
    MVAReaderEE->AddVariable("Ptl1", &ev_bdt_Ptl1);
    MVAReaderEE->AddVariable("Ptl2", &ev_bdt_Ptl2);
    MVAReaderEE->AddVariable("Ptj1", &ev_bdt_Ptj1);
    MVAReaderEE->AddVariable("Ptj2", &ev_bdt_Ptj2);
    MVAReaderEE->AddVariable("MET2ST", &ev_bdt_MET2ST);
    MVAReaderEE->AddVariable("HTLT1", &ev_bdt_HTLT1);
    MVAReaderEE->AddVariable("HTLT2", &ev_bdt_HTLT2);
    MVAReaderEE->AddVariable("Etal1", &ev_bdt_Etal1);
    MVAReaderEE->AddVariable("Etal2", &ev_bdt_Etal2);
    MVAReaderEE->AddVariable("PtRatioAJl1", &ev_bdt_PtRatioAJl1);
    MVAReaderEE->AddVariable("PtRatioAJl2", &ev_bdt_PtRatioAJl2);
    MVAReaderEE->AddVariable("NEMFracAJl1", &ev_bdt_NEMFracAJl1);
    MVAReaderEE->AddVariable("NEMFracAJl2", &ev_bdt_NEMFracAJl2);
    MVAReaderEE->AddVariable("CHFracAJl1", &ev_bdt_CHFracAJl1);
    MVAReaderEE->AddVariable("CHFracAJl2", &ev_bdt_CHFracAJl2);
    MVAReaderEE->AddVariable("NHFracAJl1", &ev_bdt_NHFracAJl1);
    MVAReaderEE->AddVariable("NHFracAJl2", &ev_bdt_NHFracAJl2);
    MVAReaderEE->AddVariable("JetDiscAJl1", &ev_bdt_JetDiscAJl1);
    MVAReaderEE->AddVariable("JetDiscAJl2", &ev_bdt_JetDiscAJl2);
    MVAReaderEE->AddVariable("dRll", &ev_bdt_dRll);
    MVAReaderEE->AddVariable("dRlj11", &ev_bdt_dRlj11);
    MVAReaderEE->AddVariable("dRlj21", &ev_bdt_dRlj21);
    MVAReaderEE->AddVariable("MSSSF", &ev_bdt_MSSSF);
    MVAReaderEE->AddVariable("Mlj11", &ev_bdt_Mlj11);
    MVAReaderEE->AddVariable("Mlj21", &ev_bdt_Mlj21);
    MVAReaderEE->AddVariable("Mlj22", &ev_bdt_Mlj22);
    MVAReaderEE->AddVariable("MTvl1", &ev_bdt_MTvl1);
    MVAReaderEE->AddVariable("MTvl2", &ev_bdt_MTvl2);
    MVAReaderEE->AddVariable("Mllj1", &ev_bdt_Mllj1);
    MVAReaderEE->AddVariable("dRWjj", &ev_bdt_dRWjj);
    MVAReaderEE->AddVariable("M_W2_jj", &ev_bdt_M_W2_jj);
    MVAReaderEE->AddVariable("M_N2_l2jj", &ev_bdt_M_N2_l2jj);
    MVAReaderEE->AddVariable("dRlj12", &ev_bdt_dRlj12);
    MVAReaderEE->AddVariable("dRlj22", &ev_bdt_dRlj22);
    MVAReaderEE->AddVariable("Mllj2", &ev_bdt_Mllj2);
    MVAReaderEE->AddSpectator("w_tot", &w_tot);
  }


  if(emu){
    MVAReaderEM->AddVariable("Nvbfj", &ev_bdt_Nvbfj);
    MVAReaderEM->AddVariable("Ptl1", &ev_bdt_Ptl1);
    MVAReaderEM->AddVariable("Ptl2", &ev_bdt_Ptl2);
    MVAReaderEM->AddVariable("Ptj1", &ev_bdt_Ptj1);
    MVAReaderEM->AddVariable("Ptj2", &ev_bdt_Ptj2);
    MVAReaderEM->AddVariable("MET2ST", &ev_bdt_MET2ST);
    MVAReaderEM->AddVariable("HTLT1", &ev_bdt_HTLT1);
    MVAReaderEM->AddVariable("HTLT2", &ev_bdt_HTLT2);
    MVAReaderEM->AddVariable("Etal1", &ev_bdt_Etal1);
    MVAReaderEM->AddVariable("Etal2", &ev_bdt_Etal2);
    MVAReaderEM->AddVariable("PtRatioAJl1", &ev_bdt_PtRatioAJl1);
    MVAReaderEM->AddVariable("PtRatioAJl2", &ev_bdt_PtRatioAJl2);
    MVAReaderEM->AddVariable("NEMFracAJl1", &ev_bdt_NEMFracAJl1);
    MVAReaderEM->AddVariable("NEMFracAJl2", &ev_bdt_NEMFracAJl2);
    MVAReaderEM->AddVariable("CHFracAJl1", &ev_bdt_CHFracAJl1);
    MVAReaderEM->AddVariable("CHFracAJl2", &ev_bdt_CHFracAJl2);
    MVAReaderEM->AddVariable("NHFracAJl1", &ev_bdt_NHFracAJl1);
    MVAReaderEM->AddVariable("NHFracAJl2", &ev_bdt_NHFracAJl2);
    MVAReaderEM->AddVariable("JetDiscAJl1", &ev_bdt_JetDiscAJl1);
    MVAReaderEM->AddVariable("JetDiscAJl2", &ev_bdt_JetDiscAJl2);
    MVAReaderEM->AddVariable("dRll", &ev_bdt_dRll);
    MVAReaderEM->AddVariable("dRlj11", &ev_bdt_dRlj11);
    MVAReaderEM->AddVariable("dRlj21", &ev_bdt_dRlj21);
    MVAReaderEM->AddVariable("MSSSF", &ev_bdt_MSSSF);
    MVAReaderEM->AddVariable("Mlj11", &ev_bdt_Mlj11);
    MVAReaderEM->AddVariable("Mlj21", &ev_bdt_Mlj21);
    MVAReaderEM->AddVariable("Mlj22", &ev_bdt_Mlj22);
    MVAReaderEM->AddVariable("MTvl1", &ev_bdt_MTvl1);
    MVAReaderEM->AddVariable("MTvl2", &ev_bdt_MTvl2);
    MVAReaderEM->AddVariable("Mllj1", &ev_bdt_Mllj1);
    MVAReaderEM->AddVariable("dRWjj", &ev_bdt_dRWjj);
    MVAReaderEM->AddVariable("M_W2_jj", &ev_bdt_M_W2_jj);
    MVAReaderEM->AddVariable("M_N2_l2jj", &ev_bdt_M_N2_l2jj);
    MVAReaderEM->AddVariable("Nb", &ev_bdt_Nb);
    MVAReaderEM->AddVariable("dRlj12", &ev_bdt_dRlj12);
    MVAReaderEM->AddVariable("M_N1_l1jj", &ev_bdt_M_N1_l1jj);
    MVAReaderEM->AddSpectator("w_tot", &w_tot);
  }

  for(unsigned int im=0; im<MNStrList.size(); im++){

    //// This can be changed after checking Hyper paramters                                                                                                                                                                                                                                                                   
    //FinalBDTHyperParamMap settings are whats used in limit for SR/CR                                                                                                                                                                                                                                                        
    FinalBDTHyperParamMap[MNStrList.at(im)] = make_pair("300","850");

    for(unsigned int ic=0; ic<NCutList.size(); ic++){
      for(unsigned int it=0; it<NTreeList.size(); it++){

        TString FileNameMM  = "output_DY_MuMu_M"+MNStrList.at(im)+"_Mode100_NTree"+NTreeList.at(it)+"_Run2_BDT.weights.xml";
        TString MVATagStrMM = "BDT_M"+MNStrList.at(im)+"_NCut"+NCutList.at(ic)+"_NTree"+NTreeList.at(it)+"_MuMu";

        if(mm)MVAReaderMM->BookMVA(MVATagStrMM, AnalyzerPath+MVAPath+"NCuts"+NCutList.at(ic)+"/"+FileNameMM);

        TString FileNameEE  = "output_DY_EE_M"+MNStrList.at(im)+"_Mode100_NTree"+NTreeList.at(it)+"_Run2_BDT.weights.xml";
        TString MVATagStrEE = "BDT_M"+MNStrList.at(im)+"_NCut"+NCutList.at(ic)+"_NTree"+NTreeList.at(it)+"_EE";

        if(ee)MVAReaderEE->BookMVA(MVATagStrEE, AnalyzerPath+MVAPath+"NCuts"+NCutList.at(ic)+"/"+FileNameEE);

        TString FileNameEM  = "output_DY_EMu_M"+MNStrList.at(im)+"_Mode100_NTree"+NTreeList.at(it)+"_Run2_BDT.weights.xml";
        TString MVATagStrEM = "BDT_M"+MNStrList.at(im)+"_NCut"+NCutList.at(ic)+"_NTree"+NTreeList.at(it)+"_EMu";

        if(emu)MVAReaderEM->BookMVA(MVATagStrEM, AnalyzerPath+MVAPath+"NCuts"+NCutList.at(ic)+"/"+FileNameEM);

      }
    }
  }
  return;

}





/// Event BDT Variables                                                                                                                                                                                                                                                                                                                                                                       

void HNL_LeptonCore::InitializeTreeVars(){

  /// Other var in BDT code (not used to train)                                                                                                                                                                                                                                                                                                                                               
  ev_bdt_Nj=-1;
  ev_bdt_LQ= -1;
  ev_bdt_LT= -1;
  ev_bdt_MET= -1;
  ev_bdt_dEtall  = -1;
  ev_bdt_dRjj12  = -1;
  ev_bdt_dRjj23  = -1;
  ev_bdt_dRjj13  = -1;
  ev_bdt_dRlj11  = -1;
  ev_bdt_dRlj12  = -1;
  ev_bdt_dRlj13  = -1;
  ev_bdt_dRlj21  = -1;
  ev_bdt_dRlj22  = -1;
  ev_bdt_dRlj23  = -1;
  ev_bdt_Mlj13   = -1;
  ev_bdt_Mlj23   = -1;
  ev_bdt_Mllj3   = -1;
  ev_bdt_Mllj4   = -1;
  ev_bdt_Mlljj12 = -1;
  ev_bdt_Mlljj13 = -1;
  ev_bdt_Mlljj14 = -1;
  ev_bdt_Mlljj23 = -1;
  ev_bdt_Mlljj24 = -1;
  ev_bdt_Mlljj34 = -1;
  ev_bdt_Mljj112 = -1;
  ev_bdt_Mljj113 = -1;
  ev_bdt_Mljj114 = -1;
  ev_bdt_Mljj123 = -1;
  ev_bdt_Mljj124 = -1;
  ev_bdt_Mljj134 = -1;
  ev_bdt_Mljj212 = -1;
  ev_bdt_Mljj213 = -1;
  ev_bdt_Mljj214 = -1;
  ev_bdt_Mljj223 = -1;
  ev_bdt_Mljj224 = -1;
  ev_bdt_Mljj234 = -1;
  ev_bdt_Mjj12   = -1;
  ev_bdt_Mjj13   = -1;
  ev_bdt_Mjj14   = -1;
  ev_bdt_Mjj23   = -1;
  ev_bdt_Mjj24   = -1;
  ev_bdt_Mjj34   = -1;
  ev_bdt_HT      = -1;
  ev_bdt_HTLT    = -1;
  ev_bdt_MET2HT = -1;


  /// Used in training                                                                                                                                                                                                                                                                                                                                                                        
  ev_bdt_Nvbfj=-1,  ev_bdt_Nb=-1;
  ev_bdt_Ptl1=-1,   ev_bdt_Ptl2=-1, ev_bdt_Ptj1=-1, ev_bdt_Ptj2=-1, ev_bdt_HTLT1=-1, ev_bdt_HTLT2=-1, ev_bdt_MET2ST=-1, ev_bdt_Etal1=-1, ev_bdt_Etal2=-1;
  ev_bdt_PtRatioAJl1=-1, ev_bdt_PtRatioAJl2=-1;
  ev_bdt_CEMFracAJl1=-1, ev_bdt_CEMFracAJl2=-1, ev_bdt_NEMFracAJl1=-1, ev_bdt_NEMFracAJl2=-1, ev_bdt_CHFracAJl1=-1, ev_bdt_CHFracAJl2=-1, ev_bdt_NHFracAJl1=-1, ev_bdt_NHFracAJl2=-1, ev_bdt_MuFracAJl1=-1, ev_bdt_MuFracAJl2=-1, ev_bdt_JetDiscAJl1=-1, ev_bdt_JetDiscAJl2=-1;
  ev_bdt_dRll=-1, ev_bdt_dRlj11=-1, ev_bdt_dRlj12=-1, ev_bdt_dRlj21=-1, ev_bdt_dRlj22=-1;
  ev_bdt_MSSSF=-1, ev_bdt_Mlj11=-1, ev_bdt_Mlj12=-1, ev_bdt_Mlj21=-1, ev_bdt_Mlj22=-1;
  ev_bdt_MTvl1=-1, ev_bdt_MTvl2=-1,   ev_bdt_Mllj1=-1, ev_bdt_Mllj2=-1;

  ev_bdt_PtWj1=-1, ev_bdt_PtWj2=-1;
  ev_bdt_dRWjj=-1, ev_bdt_dRlW12=-1, ev_bdt_dRlW22=-1;
  ev_bdt_M_W2_jj=-1, ev_bdt_M_W1_lljj=-1, ev_bdt_M_N1_l1jj=-1, ev_bdt_M_N2_l2jj=-1;

  w_tot=-1.;
}



void HNL_LeptonCore::SetupEventBDTVariables(std::vector<Lepton *> LepTColl,
                                            std::vector<Jet> JetAllColl,std::vector<Jet> JetColl, std::vector<Jet> JetVBFColl, std::vector<Jet> B_JetColl,
                                            Event  ev, Particle METv, AnalyzerParameter param){

  /// Ensure obj def is correct                                                                                                                                                                                                                                                                                                                                                               
  // All Jets = 10 GeV, noID and eta 3                                                                                                                                                                                                                                                                                                                                                        

  InitializeTreeVars();

  float Mll = GetLLMass(LepTColl);
  ev_bdt_Nb      = B_JetColl.size();
  ev_bdt_Nj      = JetColl.size();
  ev_bdt_Nvbfj   = JetVBFColl.size();
  ev_bdt_Ptl1    = LepTColl[0]->Pt();
  ev_bdt_Ptl2    = LepTColl.at(1)->Pt();
  ev_bdt_LT      = GetLT(LepTColl);
  ev_bdt_Ptj1    =  JetColl.size()<1? -1.: JetColl.at(0).Pt();
  ev_bdt_Ptj2    =  JetColl.size()<2? -1.:JetColl.at(1).Pt();
  ev_bdt_Ptj3    =  JetColl.size()<3? -1.:JetColl.at(2).Pt();
  ev_bdt_MET     = METv.Pt();
  ev_bdt_dEtall  = abs(LepTColl.at(0)->Eta()-LepTColl.at(1)->Eta());
  ev_bdt_dRll    = LepTColl.at(0)->DeltaR(*LepTColl.at(1));
  ev_bdt_dRjj12  =  JetColl.size()<2? -1.:JetColl.at(0).DeltaR(JetColl.at(1));
  ev_bdt_dRjj23  =  JetColl.size()<3? -1.:JetColl.at(1).DeltaR(JetColl.at(2));
  ev_bdt_dRjj13  =  JetColl.size()<3? -1.:JetColl.at(0).DeltaR(JetColl.at(2));
  ev_bdt_dRlj11  =  JetColl.size()<1? -1.:LepTColl.at(0)->DeltaR(JetColl.at(0));
  ev_bdt_dRlj12  =  JetColl.size()<2? -1.:LepTColl.at(0)->DeltaR(JetColl.at(1));
  ev_bdt_dRlj13  =  JetColl.size()<3? -1.:LepTColl.at(0)->DeltaR(JetColl.at(2));
  ev_bdt_dRlj21  =  JetColl.size()<1? -1.:LepTColl.at(1)->DeltaR(JetColl.at(0));
  ev_bdt_dRlj22  =  JetColl.size()<2? -1.:LepTColl.at(1)->DeltaR(JetColl.at(1));
  ev_bdt_dRlj23  =  JetColl.size()<3? -1.:LepTColl.at(1)->DeltaR(JetColl.at(2));
  ev_bdt_MSSSF   = Mll;
  ev_bdt_Mlj11   =  JetColl.size()<1? -1.:(*LepTColl.at(0)+JetColl.at(0)).M();
  ev_bdt_Mlj12   =  JetColl.size()<2? -1.:(*LepTColl.at(0)+JetColl.at(1)).M();
  ev_bdt_Mlj13   =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(2)).M();
  ev_bdt_Mlj21   =  JetColl.size()<1? -1.:(*LepTColl.at(1)+JetColl.at(0)).M();
  ev_bdt_Mlj22   =  JetColl.size()<2? -1.:(*LepTColl.at(1)+JetColl.at(1)).M();
  ev_bdt_Mlj23   =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(2)).M();
  ev_bdt_MTvl1   = MT(*LepTColl.at(0),METv);
  ev_bdt_MTvl2   = MT(*LepTColl.at(1),METv);
  ev_bdt_Mllj1   =  JetColl.size()<1? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)).M();
  ev_bdt_Mllj2   =  JetColl.size()<2? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)).M();
  ev_bdt_Mllj3   =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(2)).M();
  ev_bdt_Mllj4   = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(3)).M();
  ev_bdt_Mlljj12 =  JetColl.size()<2? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(1)).M();
  ev_bdt_Mlljj13 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(2)).M();
  ev_bdt_Mlljj14 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(0)+JetColl.at(3)).M();
  ev_bdt_Mlljj23 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)+JetColl.at(2)).M();
  ev_bdt_Mlljj24 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(1)+JetColl.at(3)).M();
  ev_bdt_Mlljj34 = JetColl.size()<4? -1.:(*LepTColl.at(0)+*LepTColl.at(1)+JetColl.at(2)+JetColl.at(3)).M();
  ev_bdt_Mljj112 =  JetColl.size()<2? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(1)).M();
  ev_bdt_Mljj113 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(2)).M();
  ev_bdt_Mljj114 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(0)+JetColl.at(3)).M();
  ev_bdt_Mljj123 =  JetColl.size()<3? -1.:(*LepTColl.at(0)+JetColl.at(1)+JetColl.at(2)).M();
  ev_bdt_Mljj124 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(1)+JetColl.at(3)).M();
  ev_bdt_Mljj134 = JetColl.size()<4? -1.:(*LepTColl.at(0)+JetColl.at(2)+JetColl.at(3)).M();
  ev_bdt_Mljj212 =  JetColl.size()<2? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(1)).M();
  ev_bdt_Mljj213 =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(2)).M();
  ev_bdt_Mljj214 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(0)+JetColl.at(3)).M();
  ev_bdt_Mljj223 =  JetColl.size()<3? -1.:(*LepTColl.at(1)+JetColl.at(1)+JetColl.at(2)).M();
  ev_bdt_Mljj224 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(1)+JetColl.at(3)).M();
  ev_bdt_Mljj234 = JetColl.size()<4? -1.:(*LepTColl.at(1)+JetColl.at(2)+JetColl.at(3)).M();
  ev_bdt_Mjj12   =  JetColl.size()<2? -1.:(JetColl.at(0)+JetColl.at(1)).M();
  ev_bdt_Mjj13   =  JetColl.size()<3? -1.:(JetColl.at(0)+JetColl.at(2)).M();
  ev_bdt_Mjj14   = JetColl.size()<4? -1.:(JetColl.at(0)+JetColl.at(3)).M();
  ev_bdt_Mjj23   =  JetColl.size()<3? -1.:(JetColl.at(1)+JetColl.at(2)).M();
  ev_bdt_Mjj24   = JetColl.size()<4? -1.:(JetColl.at(1)+JetColl.at(3)).M();
  ev_bdt_Mjj34   = JetColl.size()<4? -1.:(JetColl.at(2)+JetColl.at(3)).M();




  //==== Vars for non-prompt lepton bkg                                                                                                                                                                                                                                                                                                                                                       
  int IdxMatchAwayJetl1=-1, IdxMatchAwayJetl2=-1;
  float maxdPhi1=-999., maxdPhi2=-999.;

  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){

    float dPhi1 = fabs(TVector2::Phi_mpi_pi(LepTColl.at(0)->Phi() - JetAllColl.at(ij).Phi()));
    if(dPhi1>maxdPhi1){ maxdPhi1=dPhi1; IdxMatchAwayJetl1=ij; }

  }

  for(unsigned int ij=0; ij<JetAllColl.size(); ij++){

    float dPhi2 = fabs(TVector2::Phi_mpi_pi(LepTColl.at(1)->Phi() - JetAllColl.at(ij).Phi()));
    if(dPhi2>maxdPhi2){ maxdPhi2=dPhi2; IdxMatchAwayJetl2=ij; }

  }

  if(IdxMatchAwayJetl1!=-1){

    ev_bdt_PtRatioAJl1 = JetAllColl.at(IdxMatchAwayJetl1).Pt()/LepTColl.at(0)->Pt();
    ev_bdt_CEMFracAJl1 = JetAllColl.at(IdxMatchAwayJetl1).ChargedEmEnergyFraction();
    ev_bdt_NEMFracAJl1 = JetAllColl.at(IdxMatchAwayJetl1).NeutralEmEnergyFraction();
    ev_bdt_CHFracAJl1  = JetAllColl.at(IdxMatchAwayJetl1).ChargedHadEnergyFraction();
    ev_bdt_NHFracAJl1  = JetAllColl.at(IdxMatchAwayJetl1).NeutralHadEnergyFraction();
    ev_bdt_MuFracAJl1  = JetAllColl.at(IdxMatchAwayJetl1).MuonEnergyFraction();
    ev_bdt_JetDiscAJl1 = JetAllColl.at(IdxMatchAwayJetl1).GetTaggerResult(JetTagging::DeepJet);

  }
  else{

    ev_bdt_PtRatioAJl1=-1., ev_bdt_CEMFracAJl1=-1., ev_bdt_NEMFracAJl1=-1., ev_bdt_CHFracAJl1=-1., ev_bdt_NHFracAJl1=-1., ev_bdt_MuFracAJl1=-1., ev_bdt_JetDiscAJl1=-1.;

  }

  if(IdxMatchAwayJetl2!=-1){

    ev_bdt_PtRatioAJl2 = JetAllColl.at(IdxMatchAwayJetl2).Pt()/LepTColl.at(1)->Pt();
    ev_bdt_CEMFracAJl2 = JetAllColl.at(IdxMatchAwayJetl2).ChargedEmEnergyFraction();
    ev_bdt_NEMFracAJl2 = JetAllColl.at(IdxMatchAwayJetl2).NeutralEmEnergyFraction();
    ev_bdt_CHFracAJl2  = JetAllColl.at(IdxMatchAwayJetl2).ChargedHadEnergyFraction();
    ev_bdt_NHFracAJl2  = JetAllColl.at(IdxMatchAwayJetl2).NeutralHadEnergyFraction();
    ev_bdt_MuFracAJl2  = JetAllColl.at(IdxMatchAwayJetl2).MuonEnergyFraction();
    ev_bdt_JetDiscAJl2 = JetAllColl.at(IdxMatchAwayJetl2).GetTaggerResult(JetTagging::DeepJet);

  }
  else{

    ev_bdt_PtRatioAJl2=-1., ev_bdt_CEMFracAJl2=-1., ev_bdt_NEMFracAJl2=-1., ev_bdt_CHFracAJl2=-1., ev_bdt_NHFracAJl2=-1., ev_bdt_MuFracAJl2=-1., ev_bdt_JetDiscAJl2=-1.;

  }

  ev_bdt_HT      = 0;
  for(unsigned int itj=0; itj<JetColl.size(); itj++){ ev_bdt_HT+=JetColl.at(itj).Pt(); }
  ev_bdt_HTLT=ev_bdt_HT/ev_bdt_LT;
  ev_bdt_HTLT1=ev_bdt_HT/LepTColl.at(0)->Pt();
  ev_bdt_HTLT2=ev_bdt_HT/LepTColl.at(1)->Pt();

  std::vector<FatJet> FatJetColl;
  double ST = GetST( LepTColl, JetColl, FatJetColl, METv);
  ev_bdt_MET2HT  = JetColl.size()<1? -1.:pow(ev_bdt_MET,2.)/ev_bdt_HT;
  ev_bdt_MET2ST  = pow(ev_bdt_MET,2.)/ST;

  const float MW = 80.379;
  float dijetmass_tmp=9999.;
  float dijetmass=99990000.;
  int m=-999;
  int n=-999;

  for(UInt_t emme=0; emme<JetColl.size(); emme++){
    for(UInt_t enne=emme+1; enne<JetColl.size(); enne++) {

      dijetmass_tmp = (JetColl[emme]+JetColl[enne]).M();
      if ( fabs(dijetmass_tmp-MW) < fabs(dijetmass-MW) ) {
        dijetmass = dijetmass_tmp;
        m = emme;
        n = enne;
      }
    }
  }

  ev_bdt_PtWj1     = JetColl.size() > 1 ? JetColl[m].Pt() : -1.;
  ev_bdt_PtWj2     = JetColl.size() > 1 ? JetColl[n].Pt() : -1.;
  ev_bdt_dRWjj     = JetColl.size() > 1 ? JetColl[m].DeltaR(JetColl[n]) : -1.;
  ev_bdt_dRlW12    = JetColl.size() > 1 ? LepTColl.at(0)->DeltaR(JetColl[m] + JetColl[n]) : -1.;
  ev_bdt_dRlW22    = JetColl.size() > 1 ? LepTColl.at(1)->DeltaR(JetColl[m] + JetColl[n]) : -1.;
  ev_bdt_M_W2_jj   = JetColl.size() > 1 ? (JetColl[m] + JetColl[n]).M() : -1.;
  ev_bdt_M_W1_lljj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(0) + *LepTColl.at(1)).M() : -1.;
  ev_bdt_M_N1_l1jj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(0)).M() : -1.;
  ev_bdt_M_N2_l2jj = JetColl.size() > 1 ? (JetColl[m] + JetColl[n] + *LepTColl.at(1)).M() : -1.;

  return;


}


double HNL_LeptonCore::EvaluateEventMVA(TString mN, TString NCut, TString NTree,HNL_LeptonCore::Channel channel ,
                                        std::vector<Lepton *> LepTColl, Event  ev, Particle METv, AnalyzerParameter param){

  std::vector<FatJet> FatjetColl                  = GetHNLAK8Jets("BDT",param);
  std::vector<Jet> All_JetColl                    = GetHNLJets("NoCut_Eta3",param);
  std::vector<Jet> B_JetColl                      = GetHNLJets("BJetM",param);
  std::vector<Jet> JetColl                        = GetHNLJets("Tight",param);
  std::vector<Jet> VBF_JetColl                    = GetHNLJets("VBFTight",param);

  SetupEventBDTVariables(LepTColl,
                         All_JetColl, JetColl,VBF_JetColl,B_JetColl,
                         ev,METv,param);

  TString MVATagStr = "BDT_M"+mN+"_NCut"+NCut+"_NTree"+NTree+"_"+GetChannelString(channel);
  float MVAvalue = -99999.;
  if(GetChannelString(channel) == "MuMu") MVAvalue = MVAReaderMM->EvaluateMVA(MVATagStr);
  if(GetChannelString(channel) == "EE")   MVAvalue = MVAReaderEE->EvaluateMVA(MVATagStr);
  if(GetChannelString(channel) == "EMu")  MVAvalue = MVAReaderEM->EvaluateMVA(MVATagStr);


  return MVAvalue;
}

void HNL_LeptonCore::TriggerPrintOut(Event ev){

  vector<TString> PDs = {"DoubleEG","DoubleMuon","MuonEG","SingleElectron","SingleMuon", "EGamma"};

  cout << "TriggerPrintOut: MuMu" << endl;
  for(auto itrig : TrigList_HNL_DblMu){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl;
    }
  }

  cout << " " << endl;

  cout << "TriggerPrintOut: EE" << endl;
  for(auto itrig : TrigList_HNL_DblEG){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl;
    }
  }

  cout << " " << endl;


  cout << "TriggerPrintOut: EMu" << endl;
  for(auto itrig : TrigList_HNL_MuEG){
    for(auto ipd : PDs){
      if(ev.IsPDForTrigger(itrig, ipd)) cout << "Trigger " << itrig << " In dataset " << ipd << endl;
    }
  }

  cout << " " << endl;

  return;

}

