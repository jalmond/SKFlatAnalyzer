#include "HNL_Lepton_FakeRate.h"

void HNL_Lepton_FakeRate::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

  TString AnalyzerPath=getenv("SKFlat_WD"), SKFlatV = getenv("SKFlatV");
  TString FilePath="/data/"+SKFlatV+"/"+GetEra()+"/FakeRate/NvtxSF/";
  NvtxSFFile = new TFile(AnalyzerPath+FilePath+"FakeRateNVtx13TeV_"+GetEra()+".root");

  //// Flags available in this code
  /// HasFlag("GetNvtxSF")

  /*
    Method 1
    
   */
}

void HNL_Lepton_FakeRate::executeEvent(){

  Event ev = GetEvent();

  //************************************************///
  // setup list of IDs and jobs


  vector<TString> vLMVA;

  for(int ilmva=0 ; ilmva < 1 ; ilmva++){
    double lmva_d=  -1. + double(ilmva)*.05;
    vLMVA.push_back(DoubleToString(lmva_d));
  }

  vector<AnalyzerParameter> VParameters;
  
  if(HasFlag("CheckProfile")){
    //// Check MVA Profile  for PtParton

    vector<TString> LIDs = {"HNL_ULID_FO_v1_a_"+GetYearString(),
			    "HNL_ULID_FO_v2_a_"+GetYearString(),
			    "HNL_ULID_FO_v3_a_"+GetYearString(),
			    "HNL_ULID_FO_v4_a_"+GetYearString(),
			    "HNL_ULID_FO_v5_a_"+GetYearString(),
			    "HNL_ULID_FO_v6_a_"+GetYearString(),
			    "HNL_ULID_FO_v7_a_"+GetYearString(),
			    "HNL_ULID_FO_v8_a_"+GetYearString(),
			    "HNL_ULID_FO_v9_a_"+GetYearString(),
			    "HNL_ULID_FO_v1_b_"+GetYearString(),
                            "HNL_ULID_FO_v2_b_"+GetYearString(),
                            "HNL_ULID_FO_v3_b_"+GetYearString(),
                            "HNL_ULID_FO_v4_b_"+GetYearString(),
                            "HNL_ULID_FO_v5_b_"+GetYearString(),
                            "HNL_ULID_FO_v6_b_"+GetYearString(),
                            "HNL_ULID_FO_v7_b_"+GetYearString(),
                            "HNL_ULID_FO_v8_b_"+GetYearString(),
                            "HNL_ULID_FO_v9_b_"+GetYearString(),
			    "HNL_ULID_FO_v1_c_"+GetYearString(),
                            "HNL_ULID_FO_v2_c_"+GetYearString(),
                            "HNL_ULID_FO_v3_c_"+GetYearString(),
                            "HNL_ULID_FO_v4_c_"+GetYearString(),
                            "HNL_ULID_FO_v5_c_"+GetYearString(),
                            "HNL_ULID_FO_v6_c_"+GetYearString(),
                            "HNL_ULID_FO_v7_c_"+GetYearString(),
                            "HNL_ULID_FO_v8_c_"+GetYearString(),
                            "HNL_ULID_FO_v9_c_"+GetYearString(),
                            "HNL_ULID_FO_v0_"+GetYearString()};
			    

    vector<TString> NIDs;
    for(auto i : LIDs){
      TString aname = i;
      aname = aname.ReplaceAll("HNL_ULID_FO_","HNL_LooseID_FO");
      NIDs.push_back(i);
    }
    
			    
        
    for(unsigned int l=0 ; l < LIDs.size(); l++)  VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu, HNL_LeptonCore::NormTo1Invpb, {"DATAProfile"},NIDs[l]+"DATA","HNL_ULID_"+GetYearString(),LIDs[l]));  
    
    goto RunJobs;
    
  }
  if(HasFlag("CheckProfileEE")){
    /// Check MVA/Pt Profile for ptparton in Electrons

    vector<TString> LIDs = {"HNL_ULID_FO_v1_a_"+GetYearString(),
                            "HNL_ULID_FO_v2_a_"+GetYearString(),
                            "HNL_ULID_FO_v3_a_"+GetYearString(),
                            "HNL_ULID_FO_v4_a_"+GetYearString(),
                            "HNL_ULID_FO_v5_a_"+GetYearString(),
                            "HNL_ULID_FO_v6_a_"+GetYearString(),
                            "HNL_ULID_FO_v7_a_"+GetYearString(),
                            "HNL_ULID_FO_v8_a_"+GetYearString(),
                            "HNL_ULID_FO_v9_a_"+GetYearString(),
                            "HNL_ULID_FO_v1_b_"+GetYearString(),
                            "HNL_ULID_FO_v2_b_"+GetYearString(),
                            "HNL_ULID_FO_v3_b_"+GetYearString(),
                            "HNL_ULID_FO_v4_b_"+GetYearString(),
                            "HNL_ULID_FO_v5_b_"+GetYearString(),
                            "HNL_ULID_FO_v6_b_"+GetYearString(),
                            "HNL_ULID_FO_v7_b_"+GetYearString(),
                            "HNL_ULID_FO_v8_b_"+GetYearString(),
                            "HNL_ULID_FO_v9_b_"+GetYearString(),
                            "HNL_ULID_FO_v1_c_"+GetYearString(),
                            "HNL_ULID_FO_v2_c_"+GetYearString(),
                            "HNL_ULID_FO_v3_c_"+GetYearString(),
                            "HNL_ULID_FO_v4_c_"+GetYearString(),
                            "HNL_ULID_FO_v5_c_"+GetYearString(),
                            "HNL_ULID_FO_v6_c_"+GetYearString(),
                            "HNL_ULID_FO_v7_c_"+GetYearString(),
                            "HNL_ULID_FO_v8_c_"+GetYearString(),
                            "HNL_ULID_FO_v9_c_"+GetYearString(),
                            "HNL_ULID_FO_v0_"+GetYearString()};


    vector<TString> NIDs;
    for(auto i : LIDs){
      TString aname = i;
      aname = aname.ReplaceAll("HNL_ULID_FO_","HNL_LooseID_FO");
      NIDs.push_back(i);
    }

    for(unsigned int l=0 ; l < LIDs.size(); l++)  VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE, HNL_LeptonCore::NormTo1Invpb, {"DATAProfile"},NIDs[l]+"DATA","HNL_ULID_"+GetYearString(),LIDs[l]));
    
    goto RunJobs;
  }

  if(HasFlag("RunRatesTauAnalysis")){
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"WRTau_Resolved","POGHighPtWithLooseTrkIso","POGHighPt"));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"WRTau_Boosted","POGHighPtWithVLooseTrkIso","POGHighPt"));

  }

  if(HasFlag("RunRatesTauAnalysisEE")){
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"WRTau_Resolved", "passHEEPID"  ,"passVetoID"));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"WRTau_Boosted","CutBasedLooseNoIso"  ,"passVetoID"));

  }


  if(HasFlag("RunRates")){
    /// Measure FR in Data

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v0_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v0_"+GetYearString()));
   
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v1_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v1_a_"+GetYearString()));                                                   
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v1_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v1_b_"+GetYearString()));                                                 
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v1_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v1_c_"+GetYearString()));                                                 

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v2_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v2_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v2_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v2_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v2_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v2_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v3_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v3_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v3_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v3_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v3_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v3_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v4_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v4_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v4_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v4_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v4_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v4_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v5_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v5_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v5_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v5_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v5_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v5_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v6_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v6_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v6_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v6_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v6_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v6_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v7_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v7_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v7_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v7_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v7_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v7_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v8_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v8_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v8_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v8_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v8_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v8_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v9_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v9_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v9_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v9_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v9_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v9_c_"+GetYearString()));


    goto RunJobs;
  }
  if(HasFlag("RunRatesNonSNU")){
    /// Measure FR in Data    
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR","PR"},"Peking"        , "Peking",    "Peking_FO"));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR","PR"},"HNTightV2"     , "HNTightV2", "HNLooseV1"));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"FR","PR"},"HNL_HN3L"      , "HNL_HN3L",  "HNL_TopMVA_FO_MM"));
    goto RunJobs;
  }
  
  if(HasFlag("RunRatesEE")){
    /// Measure FR in Data 

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v0_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v0_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v1_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v1_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v1_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v1_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v1_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v1_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v2_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v2_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v2_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v2_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v2_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v2_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v3_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v3_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v3_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v3_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v3_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v3_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v4_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v4_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v4_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v4_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v4_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v4_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v5_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v5_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v5_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v5_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v5_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v5_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v6_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v6_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v6_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v6_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v6_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v6_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v7_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v7_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v7_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v7_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v7_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v7_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v8_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v8_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v8_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v8_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v8_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v8_c_"+GetYearString()));

    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v9_a_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v9_a_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v9_b_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v9_b_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR"},"HNL_ULID_FO_v9_c_"+GetYearString()  ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO_v9_c_"+GetYearString()));



    goto RunJobs;
  }
  if(HasFlag("RunRatesNonSNU_EE")){
    /// Measure FR in Data       
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR","PR"},"Peking"        , "Peking_"+GetYearString(), "Peking_FO_"+GetYearString()));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,EE,HNL_LeptonCore::NormTo1Invpb,{"FR","PR"},"HNTightV2"     , "HNTightV2", "HNLooseV4"));
    goto RunJobs;
  }

  if(HasFlag("GetNvtxSF")){
    VParameters.clear();
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"NvtxSF"},"HNL_ID"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO"));
    goto RunJobs;
  }
  if(HasFlag("MakeRegionPlots")){
    VParameters.clear();
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"MakeDiLeptonPlots"}    ,"HNL_ID_MakeDiLeptonPlots"         ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO"));
    VParameters.push_back(SetupFakeParameter(AnalyzerParameter::Central,MuMu,HNL_LeptonCore::NormTo1Invpb,{"MakeSingleLeptonPlots"},"HNL_ID_MakeSingleLeptonPlots"     ,"HNL_ULID_"+GetYearString(), "HNL_ULID_FO"));
    goto RunJobs;
  }


 RunJobs:
  
  for(auto param : VParameters)  executeEventFromParameter(param);
  
}

void HNL_Lepton_FakeRate::executeEventFromParameter(AnalyzerParameter param){
   
  //************************************************///
  // setup event level objects
  Event ev = GetEvent();

  double weight = SetupWeight(ev, param);
  if(IsData) weight = 1;
  
  if(!PassMETFilter()) return;
  
  std::vector<Electron> Initial_loose_electrons     = SelectElectrons( param,param.Electron_Loose_ID, 10, 2.5) ;
  std::vector<Muon>     Initial_loose_muons         = SelectMuons    ( param,param.Muon_Loose_ID,     5,  2.4);

  std::vector<Electron> Loose_Electrons;
  std::vector<Muon>     Loose_Muons;
  for(auto ilep : Initial_loose_electrons) Loose_Electrons.push_back(ilep);
  for(auto ilep : Initial_loose_muons)   Loose_Muons.push_back(ilep);
  

  std::vector<Jet> jets_tmp     = SelectJets   ( param, "tight", 20., 2.7);
  std::vector<Jet> jets; 
  for(unsigned int ijet =0; ijet < jets_tmp.size(); ijet++){
    bool jetok=true;
    for(unsigned int iel=0 ; iel < Initial_loose_electrons.size(); iel++)   { if(jets_tmp[ijet].DeltaR(Initial_loose_electrons[iel]) < 0.4) jetok = false;}
    for(unsigned int iel=0 ; iel < Initial_loose_muons.size(); iel++)       { if(jets_tmp[ijet].DeltaR(Initial_loose_muons[iel]) < 0.4)     jetok = false;}
    if(jetok) jets.push_back(jets_tmp[ijet]);
  }
  if(param.Channel=="EE")   RunE(Loose_Electrons,Loose_Muons, jets,  param, weight);
  if(param.Channel=="MuMu") RunM(Loose_Electrons,Loose_Muons,  jets, param, weight);
  
}


void HNL_Lepton_FakeRate::RunM(std::vector<Electron> loose_el,  std::vector<Muon> loose_mu, std::vector<Jet> jets,   AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleEG") return;
    if(this->DataStream == "SingleElectron") return;
    if(this->DataStream == "EGamma") return;
  }

  Event ev = GetEvent();

  if(loose_mu.size() == 0) return;

  // remove if muon                                                                                                                          
  if(loose_el.size() > 0) return;

  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_mu,param);
  std::vector<bool>    blepsT;
  for(auto ilep : loose_mu)   blepsT.push_back(ilep.PassID(param.Muon_Tight_ID));
  
  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  if(param.HasFlag("NvtxSF"))   MakeNVertexDistPrescaledTrig(MuMu,param, ev, leps,blepsT,param.Name+param.Channel,event_weight);   
  
  if(param.HasFlag("DATAProfile")){
    
    if(leps.size() != 1) return;

    for(auto ilep : leps){
      if(ilep->Pt() > 80) continue;
      
      double PtParton = ilep->PtParton(GetPtPartonSF(*ilep, param.Muon_Loose_ID),ilep->MVAFakeCut(param.Muon_Tight_ID,GetYearString()));
      double PtPartonUncorr = ilep->PtParton(1,ilep->MVAFakeCut(param.Muon_Tight_ID,GetYearString()));
      double W = event_weight;
      if(UseEvent(leps , jets, 40., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ40_MVA_PtParton").Data(), ilep->HNL_MVA_Fake("HFTop"), PtParton, W, 50, -1, 1);
      FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_Inclusive_MVA_PtParton").Data(), ilep->HNL_MVA_Fake("HFTop"), PtParton, W, 50, -1, 1);
      
      FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_Inclusive_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 50, -1, 1);
      if(UseEvent(leps , jets, 30., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ30_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 50, -1, 1);
      if(UseEvent(leps , jets, 40., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ40_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 50, -1, 1);
      if(UseEvent(leps , jets, 60., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ60_MVA_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 50, -1, 1);

      FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_Inclusive_MVA2_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 200, -1, 1);
      if(UseEvent(leps , jets, 30., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ30_MVA2_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 200, -1, 1);
      if(UseEvent(leps , jets, 40., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ40_MVA2_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 200, -1, 1);
      if(UseEvent(leps , jets, 60., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ60_MVA2_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("HFTop"), PtPartonUncorr, W, 200, -1, 1);

    }
    return;
  }


  if(param.HasFlag("FR") || param.HasFlag("PR"))   GetFakeRateAndPromptRates(param, leps,blepsT,jets,param.Name+param.Channel,event_weight);   
  
  if(param.HasFlag("MakeDiLeptonPlots"))  MakeDiLepPlots(MuMu,param, ev, leps,blepsT,param.Name+param.Channel,event_weight);
  
  if(!param.HasFlag("MakeSingleLeptonPlots")) return;

  if(loose_mu.size() != 1) return; //JH : require only one loose muon
  
  bool has_away_jet=false;
  for(unsigned int ij=0; ij < jets.size(); ij++){
    if(jets.at(ij).Pt() < 40.) continue;
    float dphi =fabs(TVector2::Phi_mpi_pi(loose_mu[0].Phi()- jets.at(ij).Phi()));
    if(dphi > 2.5)      has_away_jet=true;
  }

  /// Make plots to check control of prompt leptons and to plot Loose and Tight Samples
  /// Only run when not running MANY IDs so param.WriteOutVerbose is set to 2

  // prescale_trigger is to set weight of trigger prescale depending on the lepton pt
  float prescale_trigger = GetPrescale(leps);
  if(prescale_trigger == 0.) return;
  event_weight*=prescale_trigger;

  bool truth_match= false;
  if(!IsDATA) {
    if(loose_mu.at(0).IsPrompt()) truth_match=true;
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

  
  bool useevent40 = UseEvent(leps, jets, 40., METv, event_weight);
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





void HNL_Lepton_FakeRate::RunE( std::vector<Electron> loose_el, std::vector<Muon> loose_mu, std::vector<Jet> jets, AnalyzerParameter param,  float event_weight){

  if(IsData){
    if(this->DataStream == "DoubleMuon") return;
    if(this->DataStream == "SingleMuon") return;
  }

  Event ev = GetEvent();

  if(loose_el.size() == 0) return;
 
  // remove if muon 
  if(loose_mu.size() > 0) return;

  Particle METv = GetvMET("PuppiT1xyULCorr",param);

  std::vector<Lepton *> leps = MakeLeptonPointerVector(loose_el,param);

  std::vector<bool> blepsT;
  for(auto ilep : loose_el)   blepsT.push_back(ilep.PassID(param.Electron_Tight_ID));
  
  if(HasFlag("GetNvtxSF")){
    MakeNVertexDistPrescaledTrig(EE,param, ev, leps,blepsT,param.Name+param.Channel,event_weight);
    return;
  }

  if(param.HasFlag("DATAProfile")){
    
    if(leps.size() != 1) return;
   
    for(auto ilep : leps){

      double PtPartonUncorr = ilep->PtParton(1,ilep->MVAFakeCut(param.Electron_Tight_ID,GetYearString()));
      double PtParton = ilep->PtParton(GetPtPartonSF(*ilep, param.Electron_Loose_ID),ilep->MVAFakeCut(param.Electron_Tight_ID,GetYearString()));
      double W = event_weight;

      FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_Inclusive_MVA_"+ilep->GetEtaRegion("2bin")+"_PtParton").Data(), ilep->HNL_MVA_Fake("EDv5"), PtParton, W, 40, -1, 1);
      if(UseEvent(leps , jets, 40., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ40_MVA_PtParton").Data(), ilep->HNL_MVA_Fake("EDv5"), PtParton, W, 40, -1, 1);

      FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_Inclusive_MVA_"+ilep->GetEtaRegion("2bin")+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("EDv5"), PtPartonUncorr, W, 40, -1, 1);
      if(UseEvent(leps , jets, 30., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ30_MVA_"+ilep->GetEtaRegion("2bin")+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("EDv5"), PtPartonUncorr, W, 40, -1, 1);
      if(UseEvent(leps , jets, 40., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ40_MVA_"+ilep->GetEtaRegion("2bin")+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("EDv5"), PtPartonUncorr, W, 40, -1, 1);
      if(UseEvent(leps , jets, 60., METv, event_weight)) FillProf(("DATAProfile/"+param.Name + "/FakeCR"+param.GetSystType()+"_AJ60_MVA_"+ilep->GetEtaRegion("2bin")+"_PtPartonUncorr").Data(), ilep->HNL_MVA_Fake("EDv5"), PtPartonUncorr, W, 40, -1, 1);
      
    }
    return;
  }

  if(param.HasFlag("FR") || param.HasFlag("PR"))   GetFakeRateAndPromptRates(param, leps,blepsT,jets,param.Name+param.Channel,event_weight);   

  if(param.HasFlag("MakeDiLeptonPlots"))   MakeDiLepPlots(EE,param, ev, leps,blepsT,param.Name+param.Channel,event_weight);
  
  if(!param.HasFlag("MakeSingleLeptonPlots")) return;

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
  


  bool useevent40 = UseEvent(leps, jets, 40., METv,event_weight);
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

double HNL_Lepton_FakeRate::ApplyNvtxReweight(int NPV, TString Key){

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


void HNL_Lepton_FakeRate::MakeDiLepPlots(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){

  double d_event_weight = event_weight;
  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;

  // now we have 2 Tight leptons                                                                                                                             

  TString plot_dir = GetChannelString(channel);

  // Make Z peak                                                                                                                                            
  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;
  bool PassZMass = (fabs(M_Z - Z.M()) < 10.) ? true : false;

  std::vector<Jet>    AK4_JetColl                 = GetHNLJets(param.AK4JetColl,     param);
  std::vector<Jet>    AK4_BJetColl                = GetHNLJets("BJet", param); 
  std::vector<FatJet> AK8_JetColl                 = GetHNLAK8Jets(param.AK8JetColl,param);

  EvalJetWeight(AK4_JetColl, AK8_JetColl, d_event_weight, param);

  if(AK4_BJetColl.size() > 0) return;


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
    for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_3 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }
    
    if(pass_8){
      if(leps[1]->Pt() > 10){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8;
  
        if(PassZMass){
    for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_Mu17;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
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
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_8 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);

      }
    }
    if(pass_12){
      if(leps[1]->Pt() > 14){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_12 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }

    if(pass_17){
      if(leps[1]->Pt() > 20){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_17)*NVxt_El12;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_17 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }
    if(pass_23){
      if(leps[1]->Pt() > 25){
        double prescale_weight  =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
        if(PassZMass){
          for(auto ilep : leps)FillHistogram(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LepPt").Data(), ilep->Pt() , d_event_weight*prescale_weight,"PrescaledTriggerPt");
        }
        FillHist(( plot_dir +  "/RegionPlots_Dilep/"+triggerslist_23 + "_LLMass").Data(), Z.M() , d_event_weight*prescale_weight, 100., 0., 200);
      }
    }
  }

  return;


}



void HNL_Lepton_FakeRate::MakeNVertexDistPrescaledTrig(HNL_LeptonCore::Channel channel, AnalyzerParameter param, Event ev, std::vector<Lepton *> leps,std::vector<bool> blepsT,  TString label, float event_weight){
  
  if(leps.size() != 2) return;
  if(!blepsT[0] || !blepsT[1]) return;
  
  // now we have 2 Tight leptons
  // Make Z peak 

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) return;


  // remove b jet 

  std::vector<Jet> jets_tmp     = SelectJets   ( param, param.Jet_ID, 20., 5.);
  JetTagging::Parameters param_jets = JetTagging::Parameters(JetTagging::DeepJet, JetTagging::Medium, JetTagging::incl, JetTagging::mujets);
  std::vector<Jet> BJetColl    = SelectBJets(param, jets_tmp , param_jets);
  double sf_btag               = GetBJetSF(param, jets_tmp, param_jets);
  if(!IsData )event_weight*= sf_btag;

  if(BJetColl.size() > 0) return;

  bool PassZMass = (fabs(M_Z - Z.M()) < 10.) ? true : false;


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
void HNL_Lepton_FakeRate::GetFakeRateAndPromptRates(AnalyzerParameter param, std::vector<Lepton *> leps,std::vector<bool> blepsT, std::vector<Jet>    jetCollTight, TString label, float event_weight){
						      
  if (leps.size()<1) return;
  
  Event ev = GetEvent();
  Particle METv = GetvMET("PuppiT1xyULCorr",param);
  
  if(param.HasFlag("FR") && leps.size()==1)  MakeFakeRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, METv);

  if(leps.size()==2 && param.HasFlag("PR")){
    TString dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v";
    if(DataYear==2017) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";
    if(DataYear==2018) dilep_triggerslist = (leps[0]->LeptonFlavour() == Lepton::MUON) ?   "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass3p8_v" : "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v";    
    if(IsDATA&& !(ev.IsPDForTrigger(dilep_triggerslist, this->DataStream) )) return;
    if(ev.PassTrigger(dilep_triggerslist))    MakePromptRatePlots("", label, param,leps,blepsT,  jetCollTight,   event_weight, METv);
  }
  return;
}


void HNL_Lepton_FakeRate::MakePromptRatePlots(TString label, TString tag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT , std::vector<Jet> jets,  float event_weight, Particle MET){
  
  Event ev = GetEvent();

  if(leps.size() != 2) return;

  Particle Z = (*leps[0]) + (*leps[1]);
  if(leps[0]->Charge() == leps[1]->Charge()) event_weight = -1;

  if(fabs(M_Z - Z.M()) > 10.) return;

  TString L_prefix = "PrompRate/Prompt_Loose"+tag ;
  TString T_prefix = "PrompRate/Prompt_Tight"+tag;
  float weight_ptcorr=event_weight;

  if(blepsT[1]){
    // lep [1] is tag
    int ilep=0;
    float lep_pt  = leps[ilep]->UncorrectedPt();
    float lep_eta = fabs(leps[ilep]->Eta());
    if(lep_pt > 500.) lep_pt = 499.;

    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHistogram((prefix + "_pt_eta").Data(),      lep_pt,         lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
      if(fabs(M_Z - Z.M()) < 5)  FillHistogram((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
    }
  }
  if(blepsT[0]){
    // lep [0] is tag                                                                                                                                                             
    int ilep=1;
    float lep_pt = leps[ilep]->UncorrectedPt();
    float lep_eta = fabs(leps[ilep]->Eta());

    if(lep_pt > 500.) lep_pt = 499.;

    for(int Tlep = 0 ; Tlep < 2; Tlep++)  {
      TString prefix = (Tlep==0) ? L_prefix : T_prefix;
      if((Tlep==1) && ! (blepsT[ilep])) continue;
      FillHistogram((prefix + "_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
      if(fabs(M_Z - Z.M()) < 5)   FillHistogram((prefix + "_TZ_pt_eta").Data(), lep_pt, lep_eta,  weight_ptcorr, "PR_pt", "Eta4");
    }
  }

  return;
}


float HNL_Lepton_FakeRate::GetPrescale(std::vector<Lepton *>   leps  ){

  if(leps.size() != 1) return 0.;

  Event ev = GetEvent();
  float prescale_trigger= 0;
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
      return prescale_trigger;
    }
    if(leps.at(0)->Pt() >= 10.){

      if(pass_8)prescale_trigger =  (IsDATA) ? 1. :   ev.GetTriggerLumi(triggerslist_8)*NVxt_Mu8 ; //// 20 + GeV bins
      return prescale_trigger;
    }
    if(leps.at(0)->Pt() >= 5.){
      if(pass_3)prescale_trigger =  (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_3)*NVxt_Mu3 ; //// 20 + GeV bins                                                                             
      return prescale_trigger;
    }      
    
    return 0 ;
  }
  else{
    TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";  
    TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
    bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
    bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
    bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

    bool pass_8 = ev.PassTrigger(triggerslist_8) && El8PD;
    bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
    bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;

    double NVxt_El8 =  ApplyNvtxReweight(nPV,triggerslist_8);
    double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
    double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);

 
    if(leps.at(0)->Pt() >= 25.){
      if(pass_23)prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_23)*NVxt_El23;
      return prescale_trigger;
    }
    else   if(leps.at(0)->Pt() >= 15.){
      if(pass_12) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_12)*NVxt_El12;
      return prescale_trigger;
    }
    else   if(leps.at(0)->Pt() >= 9.5){
      if(pass_8) prescale_trigger = (IsDATA) ? 1. : ev.GetTriggerLumi(triggerslist_8)*NVxt_El8;
      return prescale_trigger;
    }
    return 0 ;
  }
  
  return prescale_trigger;
}
 

void HNL_Lepton_FakeRate::MakeFakeRatePlots(TString label, TString mutag,AnalyzerParameter param,  std::vector<Lepton *> leps , std::vector<bool> blepsT ,  std::vector<Jet> jets,  float event_weight,  Particle MET){
				
  /// FOR FAKE RATE SUBTRACTION NEED ONLY PROMPT MUONS                                                                                                                                                  
  bool truth_match= true;

  if(!IsDATA && leps.size() > 0) truth_match = leps[0]->IsPrompt() ;

  if(param.HasFlag("MCFakeRates")) truth_match = leps[0]->IsFake();

  if(!truth_match) return;

  double AWJPt = -1;
  vector<AnalyzerParameter::Syst> AJPt = {AnalyzerParameter::FRAJ25, AnalyzerParameter::FRAJ30,AnalyzerParameter::FRAJ40,AnalyzerParameter::FRAJ60};

  for(auto isys : AJPt){
    if(isys == AnalyzerParameter::FRAJ25)  AWJPt = 25;
    if(isys == AnalyzerParameter::FRAJ30)  AWJPt = 30;
    if(isys == AnalyzerParameter::FRAJ40)  AWJPt = 40;
    if(isys == AnalyzerParameter::FRAJ60)  AWJPt = 60;

    param.syst_ =  isys;

    bool useevent = UseEvent(leps , jets,  AWJPt,  MET, event_weight);
    
    label= mutag;
    if(jets.size() >= 1){
      if(useevent){
	if(param.Channel == "MuMu"){
	  GetMuFakeRates("Pt",      leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	  GetMuFakeRates("PtCone",  leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	  GetMuFakeRates("PtCorr",  leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	  GetMuFakeRates("PtParton",leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	  GetMuFakeRates("MotherPt",leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	}
	else{
	  GetElFakeRates("Pt",leps, blepsT, param,  jets,              label+"_"+param.GetSystType(),(event_weight));
	  GetElFakeRates("PtCone",leps, blepsT, param,  jets,          label+"_"+param.GetSystType(),(event_weight));
	  GetElFakeRates("PtCorr",leps, blepsT, param,  jets,          label+"_"+param.GetSystType(),(event_weight));
	  GetElFakeRates("PtParton",leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	  GetElFakeRates("MotherPt",leps, blepsT, param,  jets,        label+"_"+param.GetSystType(),(event_weight));
	}
      }
    }
  }
  return;
}


bool HNL_Lepton_FakeRate::UseEvent(std::vector<Lepton *> leps ,  std::vector< Jet> jets, float awayjetcut, Particle MET, float wt){

  bool useevent = false;
  if(leps.size() != 1) return false;
  
  Double_t MT=0;
  Double_t METdphi=0;
  for(unsigned int w = 0; w < leps.size();  w++){
    METdphi = TVector2::Phi_mpi_pi(leps.at(w)->Phi()-MET.Phi());
    MT = sqrt(2.* leps.at(w)->Et()*MET.Pt() * (1 - cos( METdphi)));
    if(( (MET.Pt() < 40) && (MT < 25.)) ) {

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


void HNL_Lepton_FakeRate::GetElFakeRates(TString Method, std::vector<Lepton *> leps,std::vector<bool> blepsT,  AnalyzerParameter param,  std::vector<Jet> jets,  TString tag,float event_weight){

  bool IsMuon=(leps[0]->LeptonFlavour() == Lepton::MUON);
  if(IsMuon) return;

  if(! ( HasFlag("RunRatesTauAnalysis") || HasFlag("RunRatesTauAnalysisEE"))){
    
    if(param.Name.Contains("HNL_")){
      if(Method.Contains("PtCone")) return;
    }
    else{
      if(!Method.Contains("PtCone")) return;
    }
  }
  else{
    if(! ( (Method == "PtCone") ||  (Method == "Pt") )) return;
  }
  double   isocut  = (Method == "PtCone") ? GetIsoFromID(*leps[0], param.Electron_Tight_ID) : 0. ;
  TString  LooseID =  param.Electron_Loose_ID ;
  TString  TightID =  param.Electron_Tight_ID ;
  double MVACut = leps[0]->MVAFakeCut(TightID,GetYearString());


  Event ev = GetEvent();
  TString triggerslist_8="HLT_Ele8_CaloIdL_TrackIdL_IsoVL_PFJet30_v";   
  TString triggerslist_12="HLT_Ele12_CaloIdL_TrackIdL_IsoVL_PFJet30_v";
  TString triggerslist_23="HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30_v";

  bool El8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
  bool EL12PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_12, this->DataStream) ));
  bool EL23PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_23, this->DataStream) ));

  bool pass_8  = ev.PassTrigger(triggerslist_8) && El8PD;
  bool pass_12 = ev.PassTrigger(triggerslist_12) && EL12PD ;
  bool pass_23 = ev.PassTrigger(triggerslist_23) && EL23PD;
  double NVxt_El8  =  ApplyNvtxReweight(nPV,triggerslist_8);
  double NVxt_El12 =  ApplyNvtxReweight(nPV,triggerslist_12);
  double NVxt_El23 =  ApplyNvtxReweight(nPV,triggerslist_23);

  int For_HLT_PFJet30_v=0;
  for(auto ij : jets){
    if(ij.Pt() > 40.) For_HLT_PFJet30_v++;
  }
  if(!For_HLT_PFJet30_v) return;

  float lep_pt (0);
  
  float weight_ptcorr=event_weight;
  TString MVAKey = "EDv5";
  TString ptname = "pt_eta";
  TString PtHist = "pt"; /// Used to set binning                                                                                                                                                                  
  double UpperPtCut = 80; //// Fill 60-80                                                                                                                                                                           
  double UpperPtCutM1 = UpperPtCut -1;


  if(Method == "Pt"){
    lep_pt      =  (leps[0]->Pt() < UpperPtCut) ?  leps[0]->Pt() : UpperPtCutM1;

    if(lep_pt > 25){
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *= (ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else  if(lep_pt > 15){
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);      
    }
    else  if(lep_pt > 10){
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);
    }
  }
  if(Method == "PtCone"){
    PtHist ="ptcone";
    lep_pt =  ( leps[0]->CalcPtCone(leps[0]->RelIso(), isocut) < UpperPtCut) ?  leps[0]->Pt() : UpperPtCutM1;
    ptname = "ptcone_eta";
    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 15) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 9.5) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);

    }
    else return ;
  }
  else if(Method == "PtParton"){
    PtHist ="ptcone";
    ptname = "ptparton_eta";
    double PTPartonSF = GetPtPartonSF(*leps[0], LooseID);

    lep_pt      =  (leps[0]->PtParton(PTPartonSF,MVACut) < UpperPtCut) ?  leps[0]->PtParton(PTPartonSF, MVACut) : UpperPtCutM1;
    double uncorr_lep_pt = (leps[0]->PtParton(1,MVACut) < UpperPtCut) ?  leps[0]->PtParton(1, MVACut) : UpperPtCutM1;

    FillProf((param.Name + "_FakeCR"+param.GetSystType()+"_MVA_PtParton").Data(), leps[0]->HNL_MVA_Fake(MVAKey), lep_pt, event_weight, 100, -1, 1);
    FillProf((param.Name + "_FakeCR"+param.GetSystType()+"_MVA_PtParton_Uncorr").Data(), leps[0]->HNL_MVA_Fake(MVAKey), uncorr_lep_pt, event_weight, 100, -1, 1);

    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 15) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 10) return;
      //if(!pass_8) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);

    }
    else return ;
    
  }

  else if(Method == "PtCorr"){

    PtHist ="ptcone";
    ptname = "ptcorr_eta";
    lep_pt      = (leps[0]->CalcMVACone(MVACut)  < UpperPtCut) ? leps[0]->CalcMVACone(MVACut)  : UpperPtCutM1;
    
    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 15) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 9.5) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);

    }
    else return ;

  }
  
  else if(Method == "MotherPt"){
    PtHist ="ptcone";
    ptname = "MJpt_eta";
    lep_pt      = leps[0]->MotherJetPt();


    if(lep_pt > 40){
      if(leps[0]->Pt() < 25) return;
      if(!pass_23) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_23)*NVxt_El23);
    }
    else if(lep_pt > 23){
      if(leps[0]->Pt() < 10) return;
      if(!pass_12) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_12)*NVxt_El12);
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 9.5) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*NVxt_El8);

    }
    else return ;

  }

  float lep_eta     = leps[0]->fEta(); /// returns |eta| OR for el |scEta|
  float lep_mva_lfvshf  = leps[0]->HNL_MVA_Fake("QCD_LFvsHF_v5");
  float lep_mva_bvsc    = leps[0]->HNL_MVA_Fake("QCD_BvsC_v5");
  float lep_blscore     = leps[0]->CloseJet_BScore();
  float lep_cbscore     = leps[0]->CloseJet_CvsBScore();
  float lep_clscore     = leps[0]->CloseJet_CvsLScore();
  TString lepEtaRegion  = leps[0]->etaRegionString();
  TString lepRegion     = (leps[0]->IsBB()) ? "BB" : "EC";

  TString L_prefix = "Fake_"+Method+"_Loose"+tag;
  TString T_prefix = "Fake_"+Method+"_Tight"+tag;

  for(int ilep = 0 ; ilep < 2; ilep++)  {

    TString prefix = (ilep==0) ? L_prefix : T_prefix;
    if((ilep==1) && !(blepsT[0])) continue;
    
    prefix = Method + "/"+prefix;

    TString Ptlab = "p_{T} (GeV)";

    if(lep_pt >10){
      FillHistogram((prefix + "_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_"+ptname+"v2").Data(),       lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Etav2", Ptlab);
      FillHistogram((prefix + "_"+ptname+"v3").Data(),       lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Etav3", Ptlab);
    

      FillHistogram((prefix + "_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+ PtHist, Ptlab);
      FillHistogram((prefix + "_eta").Data(),                lep_eta, weight_ptcorr ,"FR_Eta","#eta");
      FillHist((prefix + "_eta_fine").Data(),                lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      FillHist((prefix + "_lep_mva_lfvshf").Data(),          lep_mva_lfvshf,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_lep_mva_bvsc").Data(),            lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);



      if(lep_blscore < 0.1) FillHistogram((prefix + "_bscore0to0p1_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);

      if(lep_blscore < 0.05) FillHistogram((prefix + "_bscore1_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      else if(lep_blscore < 0.1) FillHistogram((prefix + "_bscore2_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta2Bin", Ptlab);
      else if(lep_blscore < 0.4)  FillHistogram((prefix + "_bscore3_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta1", Ptlab);
      else FillHistogram((prefix + "_bscore4_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta1", Ptlab);

      FillHist((prefix + "_blscore").Data(), lep_blscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_cbscore").Data(), lep_cbscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_clscore").Data(), lep_clscore,    weight_ptcorr, 50, -1., 1.);

      FillHistogram((prefix + "_"+leps[0]->LeptonFakeTagger()+"_"+ptname).Data(),       lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      
      
    }
  }
  return;

}

void HNL_Lepton_FakeRate::GetMuFakeRates(TString Method, std::vector<Lepton *> leps,std::vector<bool> blepsT,  AnalyzerParameter param,  std::vector<Jet> jets,  TString tag,float event_weight){
  

  bool IsMuon=(leps[0]->LeptonFlavour() == Lepton::MUON);
  if(!IsMuon) return;

  if(! ( HasFlag("RunRatesTauAnalysis") ||  HasFlag("RunRatesTauAnalysisEE"))){
    if(param.Name.Contains("HNL_")){
      if(Method.Contains("PtCone")) return;
    }
    else{
      if(!Method.Contains("PtCone")) return;
    }
  }
  else{
    if(! ( (Method == "PtCone") ||   (Method == "Pt") )) return;
  }
  double   isocut  = (Method == "PtCone") ? GetIsoFromID(*leps[0], param.Muon_Tight_ID) : 0.;
  TString  LooseID =  param.Muon_Loose_ID ;
  TString  TightID =  param.Muon_Tight_ID ;
  double MVACut = leps[0]->MVAFakeCut(TightID,GetYearString());
  
  ///// TRIGGER CONDITIONS

  Event ev = GetEvent();  

  TString triggerslist_3="HLT_Mu3_PFJet40_v";  TString triggerslist_8="HLT_Mu8_TrkIsoVVL_v";  TString triggerslist_17="HLT_Mu17_TrkIsoVVL_v";
  bool Mu3PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_3, this->DataStream) ));
  bool Mu8PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_8, this->DataStream) ));
  bool Mu17PD= (!IsDATA || (IsDATA&& ev.IsPDForTrigger(triggerslist_17, this->DataStream) ));

  bool pass_3 = ev.PassTrigger(triggerslist_3) && Mu3PD;
  bool pass_8 = ev.PassTrigger(triggerslist_8) && Mu8PD;
  bool pass_17 = ev.PassTrigger(triggerslist_17) && Mu17PD;

  int For_HLT_Mu3_PFJet40_v=0;
  for(auto ij : jets){
    if(ij.Pt() > 50.) For_HLT_Mu3_PFJet40_v++;
  }

  double UpperPtCut = 80; //// Fill 60-80
  double UpperPtCutM1 = UpperPtCut -1;

  TString MVAKey = "HFTop";
  TString ptname = "pt_eta";
  TString PtHist = "pt"; /// Used to set binning
  
  float weight_ptcorr=event_weight;
  float lep_pt (0);
  if(Method == "Pt"){
    lep_pt      =  (leps[0]->Pt() < UpperPtCut) ?  leps[0]->Pt() : UpperPtCutM1;
    if(lep_pt > 20){
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *= (ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else  if(lep_pt > 10){
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));      
    }
    else  if(lep_pt > 5){
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));
    }
  }
  if(Method == "PtCone"){
    lep_pt      =  ( leps[0]->CalcPtCone(leps[0]->RelIso(), isocut) < UpperPtCut) ?  leps[0]->Pt() : UpperPtCutM1;
    ptname = "ptcone_eta";
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 5){
      if(leps[0]->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));

    }
    else return ;
  }
  else if(Method == "PtParton"){
    
    ptname = "ptparton_eta";
    double PTPartonSF = GetPtPartonSF(*leps[0], LooseID);
    
    lep_pt      =  (leps[0]->PtParton(PTPartonSF,MVACut) < UpperPtCut) ?  leps[0]->PtParton(PTPartonSF, MVACut) : UpperPtCutM1;
    
    //if(leps[0]->Pt() < 10)      cout << "Lep pt = " << leps[0]->Pt() << " ptparton = " << lep_pt <<  << " PASS TRIG " << triggerslist_3 << " ~For_HLT_Mu3_PFJet40_v~="  <<For_HLT_Mu3_PFJet40_v <<endl;
  
    FillProf((param.Name + "_FakeCR40_MVA_PtParton").Data(), leps[0]->HNL_MVA_Fake(MVAKey), lep_pt, event_weight, 100, -1, 1);
    
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 10){
      if(leps[0]->Pt() < 5) return;
      //if(!pass_3) return;
      //if(For_HLT_Mu3_PFJet40_v==0) return;
      //if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
   else return ;
    
  }


  if(Method == "PtCorr"){

    ptname = "ptcorr_eta";
    lep_pt      = (leps[0]->CalcMVACone( MVACut)  < UpperPtCut) ? leps[0]->CalcMVACone(MVACut)  : UpperPtCutM1;
    
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 5){
      if(leps[0]->Pt() < 5) return;
      //if(!pass_3) return;
      //if(For_HLT_Mu3_PFJet40_v==0) return;
      //if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));

    }
    else return ;
  }

  if(Method == "MotherPt"){

    ptname = "MJpt_eta";
    lep_pt      = leps[0]->MotherJetPt();
    
    if(lep_pt > 30){
      if(leps[0]->Pt() < 20) return;
      if(!pass_17) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_17)*ApplyNvtxReweight(nPV,triggerslist_17));
    }
    else if(lep_pt > 15){
      if(leps[0]->Pt() < 10) return;
      if(!pass_8) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_8)*ApplyNvtxReweight(nPV,triggerslist_8));
    }
    else if(lep_pt > 5){
      if(leps[0]->Pt() < 5) return;
      if(!pass_3) return;
      if(For_HLT_Mu3_PFJet40_v==0) return;
      if(!IsData) weight_ptcorr *=(ev.GetTriggerLumi(triggerslist_3)*ApplyNvtxReweight(nPV,triggerslist_3));
    }
    else return ;
  }

  float lep_eta         = leps[0]->fEta(); /// returns |eta| OR for el |scEta|
  float lep_mva_lfvshf  = leps[0]->HNL_MVA_Fake("QCD_LFvsHF_v5");
  float lep_mva_bvsc    = leps[0]->HNL_MVA_Fake("QCD_BvsC_v5");
  float lep_blscore     = leps[0]->CloseJet_BScore();
  float lep_cbscore     = leps[0]->CloseJet_CvsBScore();
  float lep_clscore     = leps[0]->CloseJet_CvsLScore();
  TString lepEtaRegion  = leps[0]->etaRegionString();
  TString lepRegion     = (leps[0]->IsBB()) ? "BB" : "EC";

  float  lepptration = leps[0]->CloseJet_Ptratio();

  TString L_prefix = "Fake_"+Method+"_Loose"+tag;
  TString T_prefix = "Fake_"+Method+"_Tight"+tag;

  for(int ilep = 0 ; ilep < 2; ilep++)  {

    TString prefix = (ilep==0) ? L_prefix : T_prefix;
    if((ilep==1) && !(blepsT[0])) continue;
    
    prefix = Method + "/"+prefix;

    TString Ptlab = "p_{T} (GeV)";
  
    if(lep_pt >10){
      FillHistogram((prefix + "_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_"+ptname+"Binning2").Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist+"2" , "FR_Eta", Ptlab);
      FillHistogram((prefix + "_"+ptname+"v2").Data(),       lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Etav2", Ptlab);
      FillHistogram((prefix + "_"+ptname+"v3").Data(),       lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Etav3", Ptlab);
      
      FillHist((prefix + "_lepptratio_"+ptname).Data(), lepptration , weight_ptcorr , 100, 0, 5.);

      if(lep_blscore < 0.025) FillHistogram((prefix + "_bscore0to0p025_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);      
      
      if(lep_blscore < 0.01)       FillHistogram((prefix + "_bscore1_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      else if(lep_blscore < 0.025) FillHistogram((prefix + "_bscore2_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      else if(lep_blscore < 0.15) FillHistogram((prefix + "_bscore3_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta2Bin", Ptlab);
      else if(lep_blscore < 0.6)  FillHistogram((prefix + "_bscore4_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta1", Ptlab);
      else FillHistogram((prefix + "_bscore5_"+ptname).Data(),            lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta1", Ptlab);

      FillHistogram((prefix + "_pt").Data(),                 lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepEtaRegion).Data(),  lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_pt_"+ lepRegion).Data(),     lep_pt,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist, Ptlab);
      FillHistogram((prefix + "_eta").Data(),                lep_eta, weight_ptcorr ,"FR_Eta","#eta");
      FillHist((prefix + "_eta_fine").Data(),                lep_eta, weight_ptcorr , 50, 0, 2.5,"#eta");
      FillHist((prefix + "_lep_mva_lfvshf").Data(),          lep_mva_lfvshf,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_lep_mva_bvsc").Data(),            lep_mva_bvsc,  weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_blscore").Data(), lep_blscore,    weight_ptcorr, 400, -1., 1.);
      FillHist((prefix + "_cbscore").Data(), lep_cbscore,    weight_ptcorr, 50, -1., 1.);
      FillHist((prefix + "_clscore").Data(), lep_clscore,    weight_ptcorr, 50, -1., 1.);

      FillHistogram((prefix + "_"+leps[0]->LeptonFakeTagger()+"_"+ptname).Data(),       lep_pt, lep_eta,  weight_ptcorr, "FR_"+leps[0]->GetFlavour()+"_"+PtHist , "FR_Eta", Ptlab);
      
    }
  }
  return;
}


HNL_Lepton_FakeRate::HNL_Lepton_FakeRate(){
  
}
 
HNL_Lepton_FakeRate::~HNL_Lepton_FakeRate(){

  //  delete NvtxSFFile;
  //for(std::map< TString, TH1D* >::iterator mapit = maphist_NvtxSF.begin(); mapit!=maphist_NvtxSF.end(); mapit++){
  //  delete mapit->second;
  // }
  //maphist_NvtxSF.clear();

}

void HNL_Lepton_FakeRate::FillRegionPlots( TString plot_dir, TString region,  std::vector<Jet> jets,  std::vector<Electron> els, std::vector<Muon> mus, Particle  met,  double w){
  
  if((els.size()+ mus.size()) != 1) return;
  Lepton lep1;
  if(els.size()==1) lep1=els[0];
  else lep1 = mus[0];

  FillHistogram( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_binned_pt", lep1.Pt()  , w, "FR_pt","p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_pt", lep1.Pt()  , w, 400, 0., 2000.,"p_{T} GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_eta", lep1.Eta()  , w, 30, -3., 3,"#eta");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_CloseJet_Ptrel", lep1.CloseJet_Ptrel(), w, 50., 0., 2.,"PtRel");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Lep1_CloseJet_Ptratio", lep1.CloseJet_Ptratio(), w, 50., 0., 2.,"PtRel");

  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Met", met.Pt()  , w, 200, 0., 400.,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MetPhi", met.Phi()  , w, 100, -5, 5,"MET GeV");
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/njets" , jets.size() , w, 5, 0., 5., "N_{jets}");

  float lep_reliso  = lep1.RelIso();
  float lep_minireliso  = lep1.MiniRelIso();
  float lep_sip3d    = fabs(lep1.IP3D()/lep1.IP3Derr());
  float lep_mva     =  lep1.LepMVA();
  float lep_dxy     = fabs(lep1.dXY());

  if(els.size() > 0)   FillHist( plot_dir +  "/RegionPlots_"+ region+ "/NMissingHits", els[0].NMissingHits(), w, 5, 0., 5.);


  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/Reliso", lep_reliso, w, 100, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/MiniReliso", lep_minireliso, w, 100, 0., 1.);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/dXY",    lep_dxy, w, 100, 0., 0.5);
  FillHist( plot_dir +  "/RegionPlots_"+ region+ "/SIP3D",  lep_sip3d, w, 50, 0., 10.);
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


void HNL_Lepton_FakeRate::FillEventCutflow(int charge_i, int cf,float wt, TString cut,   TString label){


  return;

}

void HNL_Lepton_FakeRate::FillWeightHist(TString label, double _weight){
  FillHist( "weights/"+ label , _weight ,1., 200, 0., 5,"ev weight");
}
void HNL_Lepton_FakeRate::FillCutFlow(bool IsCentral, TString suffix, TString histname, double weight){

  if(IsCentral){

    FillHist(suffix+"/"+histname, 0., weight, 1, 0., 1.);

  }

}

