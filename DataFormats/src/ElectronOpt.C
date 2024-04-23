#include "Electron.h"
#include <iostream>
#include <sstream>
#include <string>

bool Electron::Pass_MultiFunction_Opt(TString ID) const {

  bool DEBUG=false;
  /// OPTIMISATION CODES                                                                                                                                                                                                                                                                                                                                                                   

  if(DEBUG)cout << ID  << "  " << this->Pt() << " eta = " << this->Eta() << endl;

  //// Predefined CUts to reduce Hist string name                                                                                                                                                                                                                                                                                                                                          
  if (ID.Contains("ScanCFNP2016_")   && !PassID("HNL_ULID_CFNPOPT16")) return false;
  if (ID.Contains("ScanCFNP2017_")   && !PassID("HNL_ULID_CFNPOPT17")) return false;
  if (ID.Contains("ScanCFNP2018_")   && !PassID("HNL_ULID_CFNPOPT18")) return false;


  TString ID_sub = ID;
  ID_sub = ID_sub.ReplaceAll("_"," ");
  string sID_sub = string(ID_sub);

  vector<TString> subStrings;
  istringstream ID_subs(sID_sub);
  do {
    string subs;
    ID_subs >> subs;
    subStrings.push_back(TString(subs));
  } while (ID_subs);


  TString  trig(""), conv_method(""),dxy_method(""),chg_method("");
  TString iso_methodB (""), iso_methodEC("");
  TString pog_method (""), mvacf_bb_method(""), mvacf_ec_method(""),  mvafake_bb_method(""),  mvafake_ec_method(""),   mvaconv_bb_cut1_method(""),  mvaconv_bb_cut2_method("");
  TString  mvaconv_ec_cut1_method(""), mvaconv_ec_cut2_method(""),   mvacf_bb_cut1_method(""),   mvacf_bb_cut2_method(""),   mvacf_ec_cut1_method(""),   mvacf_ec_cut2_method("");

  for(unsigned int i=0; i < subStrings.size(); i++){

    if (subStrings[i].Contains("LTrig"))  {trig ="Loose";       }
    if (subStrings[i].Contains("TTrig"))  {trig ="Tight"; }
    if (subStrings[i].Contains("ConvB"))  {conv_method +="B"; }
    if (subStrings[i].Contains("ConvEC")) {conv_method +="EC"; }
    if (subStrings[i].Contains("CCB"))    {chg_method +="B"; }
    if (subStrings[i].Contains("CCEC"))   {chg_method +="EC"; }
    if (subStrings[i].Contains("DXY"))    {dxy_method=subStrings[i]; }
    if (subStrings[i].Contains("MVABCV1"))mvaconv_bb_cut1_method=subStrings[i];
    if (subStrings[i].Contains("MVABCV2"))mvaconv_bb_cut2_method=subStrings[i];
    if (subStrings[i].Contains("MVAECV1"))mvaconv_ec_cut1_method=subStrings[i];
    if (subStrings[i].Contains("MVAECV2"))mvaconv_ec_cut2_method=subStrings[i];
    if (subStrings[i].Contains("MVABCF1"))mvacf_bb_cut1_method=subStrings[i];
    if (subStrings[i].Contains("MVABCF2"))mvacf_bb_cut2_method=subStrings[i];
    if (subStrings[i].Contains("MVAECF1"))mvacf_ec_cut1_method=subStrings[i];
    if (subStrings[i].Contains("MVAECF2"))mvacf_ec_cut2_method=subStrings[i];
    if (subStrings[i].Contains("MVABCF")) mvacf_bb_method=subStrings[i];
    if (subStrings[i].Contains("MVAECF")) mvacf_ec_method=subStrings[i];
    if (subStrings[i].Contains("MVABNP")) mvafake_bb_method=subStrings[i];
    if (subStrings[i].Contains("MVAENP")) mvafake_ec_method=subStrings[i];
    if (subStrings[i].Contains("POG")) {pog_method=subStrings[i];        }
    if (subStrings[i].Contains("WP"))  {pog_method=subStrings[i];        }
    if (subStrings[i].Contains("ISOB")) {iso_methodB=subStrings[i];       }
    if (subStrings[i].Contains("ISOEC")) {iso_methodEC=subStrings[i];      }
  }
  

  if(mvaconv_bb_cut1_method != ""){
    if(IsBB()){
      if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(20,80, StringToDouble(mvaconv_bb_cut1_method,"MVABCV1"),StringToDouble(mvaconv_bb_cut2_method,"MVABCV2")),"j_lep_mva_hnl_conv_EDv5")) return 0;
    }
  }
  if(mvaconv_ec_cut1_method != ""){
    if(IsEC()){
      if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(20,80, StringToDouble(mvaconv_ec_cut1_method,"MVAECV1"),StringToDouble(mvaconv_ec_cut2_method,"MVAECV2")),"j_lep_mva_hnl_conv_EDv5")) return 0;
    }
  }

  if(mvacf_bb_cut1_method != "" || mvacf_ec_cut1_method != ""){
    if(mvacf_bb_cut1_method != ""){
      if(IsBB()){
        if(!Pass_MVA(HNL_MVA_CF("EDv5"), GetPtSlopeCut(10,80, StringToDouble(mvacf_bb_cut1_method,"MVABCF1"),StringToDouble(mvacf_bb_cut2_method,"MVABCF2")),"j_lep_mva_hnl_cf_EDv5")) return 0;
      }
    }
    if(mvacf_ec_cut1_method != ""){
      if(IsEC()){
        if(!Pass_MVA(HNL_MVA_CF("EDv5"), GetPtSlopeCut(10,80, StringToDouble(mvacf_ec_cut1_method,"MVAECF1"),StringToDouble(mvacf_ec_cut2_method,"MVAECF2")),"j_lep_mva_hnl_conv_EDv5")) return 0;
      }
    }

  }
  else{
    if(mvacf_bb_method != ""){
      if(IsBB()){
        if(!Pass_MVA(HNL_MVA_CF("EDv5"), StringToDouble(mvacf_bb_method, "MVABCF"), "j_lep_mva_hnl_cf_EDv5")) return 0;
      }
    }
    if(mvacf_ec_method != ""){
      if(IsEC()){
        if(!Pass_MVA(HNL_MVA_CF("EDv5"), StringToDouble(mvacf_ec_method, "MVAECF"), "j_lep_mva_hnl_cf_ed_EDv5")) return 0;
      }
    }
  }

  if(mvafake_bb_method != ""){
    if(IsBB()){
      if(!Pass_MVA(HNL_MVA_Fake("EDv5"), StringToDouble(mvafake_bb_method, "MVABNP"), "j_lep_mva_hnl_fake_v5")) return 0;
    }
  }
  if(mvafake_ec_method != ""){
    if(IsEC()){
      if(!Pass_MVA(HNL_MVA_Fake("EDv5"), StringToDouble(mvafake_ec_method, "MVAENP"), "j_lep_mva_hnl_fake_ed_v5")) return 0;

    }
  }
  if(dxy_method == "DXYv1" && !PassID("HNLIPv1")) return false;
  if(dxy_method == "DXYv2" && !PassID("HNLIPv2")) return false;
  if(dxy_method == "DXYv3"){
    if(!PassID("HNLIPv1")) return false;
    if(!PassID("HNLIPv3")) return false;
  }
  if(dxy_method == "DXYv4"){
    if(!PassID("HNLIPv1")) return false;
    if(!PassID("HNLIPv4")) return false;
  }

  if(trig == "Loose"){
    if(! (Pass_TriggerEmulationLoose()) ){
      return false;
    }
  }
  if(trig == "Tight"){
    if(! (Pass_TriggerEmulation()) ) {
      return false;
    }
  }

  if(!Pass_LepMVAID()) return false;

  if(pog_method == "POGT"      && !(passTightID())) return false;
  if(pog_method == "POGM"      && !(passMediumID())) return false;
  if(pog_method == "POGTNoIso" && !(Pass_CutBasedTightNoIso()) ) return false;
  if(pog_method == "POGMNoIso" && !(Pass_CutBasedMediumNoIso()) ) return false;
  if(pog_method == "MVAWP90"   && !passMVAID_noIso_WP90())  return false;
  if(pog_method == "MVAWP80"   && !passMVAID_noIso_WP80())  return false;

  if( IsBB() ){

    if(pog_method.Contains("MVABWP90") && !passMVAID_noIso_WP90())  return false;
    if(pog_method.Contains("MVABWP80") && !passMVAID_noIso_WP80())  return false;

    if(conv_method.Contains("B")) {
      if(! (PassConversionVeto()) ) {
        if(DEBUG) cout << "PassConversionVeto FAIL" << endl;
        return false;
      }
    }

    if(chg_method.Contains("B")) {
      if(! IsGsfCtfScPixChargeConsistent()) {
        if(DEBUG) cout << "IsGsfCtfScPixChargeConsistent FAIL" << endl;
        return false;
      }
    }

    if(iso_methodB != ""){

      double  iso_cut_B = StringToDouble(iso_methodB,"ISOB");

      if(DEBUG) cout << "RelIso " << iso_cut_B << endl;
      if(! (RelIso()<iso_cut_B) ) return false;
    }
  }
  else{

    if(pog_method.Contains("MVAECWP90") && !passMVAID_noIso_WP90())  return false;
    if(pog_method.Contains("MVAECWP80") && !passMVAID_noIso_WP80())  return false;


    if(conv_method.Contains("EC")) {
      if(! (PassConversionVeto()) ){
        if(DEBUG) cout << "PassConversionVeto FAIL" << endl;
        return false;
      }
    }

    if(chg_method.Contains("EC")) {
      if(! IsGsfCtfScPixChargeConsistent()) {
        if(DEBUG) cout << "IsGsfCtfScPixChargeConsistent FAIL" << endl;
        return false;
      }
    }
    if(iso_methodEC != ""){

      double  iso_cut_EC = StringToDouble(iso_methodEC,"ISOEC");
      if(DEBUG) cout << "RelIso " << iso_cut_EC << endl;

      if(! (RelIso()<iso_cut_EC) ) return false;
    }

  }


  return true;

}


int  Electron::PassIDStudy(TString ID) const{

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////   The following IDs are checking Eff of WPs for each MVA                                         
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////                                                 

  if(ID.Contains("HNL_ULID_ScanCF")){
    if(!PassMVABaseLine()) return 0;

    double _cut=1;
    if(ID.Contains("CFVT")) _cut=0.7;
    if(ID.Contains("CFT"))  _cut=0.6;
    if(ID.Contains("CFM"))  _cut=0.4;
    if(ID.Contains("CFL"))  _cut=0.2;
    if(ID.Contains("CFVL")) _cut=0.;
    TString IDTmp = ID;
    IDTmp=IDTmp.ReplaceAll("CFVT","CF");
    IDTmp=IDTmp.ReplaceAll("CFT","CF");
    IDTmp=IDTmp.ReplaceAll("CFM","CF");
    IDTmp=IDTmp.ReplaceAll("CFL","CF");
    IDTmp=IDTmp.ReplaceAll("CFVL","CF");

    if(IDTmp == "HNL_ULID_ScanCF_BDTG"        && !Pass_MVA(HNL_MVA_CF("EDv5"),_cut,"j_lep_mvav2_hnl_cf"))        return 0;
    if(IDTmp == "HNL_ULID_ScanCFPt_BDTG"      && !Pass_MVA(HNL_MVA_CF("EDv5Pt"),_cut,"j_lep_mvav2_hnl_cf"))        return 0;

    return 1;
  }

  if(ID.Contains("HNL_ULID_ScanFAKE")){
    if(!PassMVABaseLine()) return 0;

    double _cut=1;
    if(ID.Contains("FAKEVVT")) _cut=0.6;
    if(ID.Contains("FAKEVT"))  _cut=0.5;
    if(ID.Contains("FAKET"))   _cut=0.4;
    if(ID.Contains("FAKEM"))   _cut=0.3;
    if(ID.Contains("FAKEL"))   _cut=0.2;
    if(ID.Contains("FAKEVL"))  _cut=0.;
    if(ID.Contains("FAKEVVL")) _cut=-0.2;

    TString IDTmp = ID;
    IDTmp=IDTmp.ReplaceAll("FAKEVVT","FAKE");
    IDTmp=IDTmp.ReplaceAll("FAKEVT","FAKE");
    IDTmp=IDTmp.ReplaceAll("FAKET","FAKE");
    IDTmp=IDTmp.ReplaceAll("FAKEM","FAKE");
    IDTmp=IDTmp.ReplaceAll("FAKEL","FAKE");
    IDTmp=IDTmp.ReplaceAll("FAKEVL","FAKE");
    IDTmp=IDTmp.ReplaceAll("FAKEVVL","FAKE");

    if(IDTmp == "HNL_ULID_ScanFAKE_BDTG"     && !Pass_MVA(HNL_MVA_Fake("EDv5"),     _cut,"j_lep_mva_hnl_fake_v5"))  return 0;
    if(IDTmp == "HNL_ULID_ScanFAKE_BDTG_LF"  && !Pass_MVA(HNL_MVA_Fake("v5_LF"),  _cut,"j_lep_mva_hnl_fake_v5_lf"))  return 0;
    if(IDTmp == "HNL_ULID_ScanFAKE_BDTG_HF"  && !Pass_MVA(HNL_MVA_Fake("v5_HF"),  _cut,"j_lep_mva_hnl_fake_v5_hf"))  return 0;

    return 1;
  }

  if(ID.Contains("HNL_ULID_ScanCONV")){
    if(!PassMVABaseLine()) return 0;

    double _cut=1;
    if(ID.Contains("CONVVT"))  _cut=0.2;
    if(ID.Contains("CONVT"))   _cut=0.;
    if(ID.Contains("CONVM"))   _cut=-0.5;
    if(ID.Contains("CONVL"))   _cut=-0.7;

    TString IDTmp = ID;
    IDTmp=IDTmp.ReplaceAll("CONVVT","CONV");
    IDTmp=IDTmp.ReplaceAll("CONVT","CONV");
    IDTmp=IDTmp.ReplaceAll("CONVM","CONV");
    IDTmp=IDTmp.ReplaceAll("CONVL","CONV");
    if(IDTmp == "HNL_ULID_ScanCONV_BDTG"     && !Pass_MVA(HNL_MVA_Conv("EDv5"),     _cut,"j_lep_mva_hnl_conv_EDv5"))  return 0;

    return 1;
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////         
  /////////////////////////////////////////////////////////////////////////////////////////////////////////         
  //////   Efficiency IDs are IDs that are tested, based off scans of MVAs in BB/EC                                 
  /////////////////////////////////////////////////////////////////////////////////////////////////////////         
  /////////////////////////////////////////////////////////////////////////////////////////////////////////                                                 
  if(ID == "HNL_ULID_Run2_CF" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA_BBEC("CF_EDv5", 0.6,0.6, "j_lep_mva_hnl_cf_v5"))   return 0;
    return 1;
  }

  if(ID == "HNL_ULID_Run2_Conv" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.4,0.) , GetPtSlopeCut(20,50,-0.4,0.2) ,   "Conv_v5")) return 0;
    return 1;
  }

  if(ID == "HNL_ULID_Run2_Fake" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA_BBEC("Fake_EDv5", 0.4, 0.5 ,   "j_lep_mva_hnl_fake_v5")) return 0;
    return 1;
  }

  if(ID == "HNL_ULID_Run2_CFPt" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA_BBEC("CFPt_EDv5", 0.65,0.6 ,   "j_lep_mva_hnl_fake_v5")) return 0;
    return 1;
  }

  if(ID == "HNL_LID_Run2_CF"){
    if(!PassMVABaseLine()) return 0;
    if( IsBB()){
      if(!Pass_MVA(HNL_MVA_CF("v4"),   0.7,  "j_lep_mva_hnl_cf_v5"))   return 0;
    }
    else{
      if(!Pass_MVA(HNL_MVA_CF("EDv4"),   0.65, "j_lep_mva_hnl_cf_ed_v5"))   return 0;
    }

    return 1;
  }

  if(ID == "HNL_LID_Run2_Conv"){

    if(!PassMVABaseLine()) return 0;
    if( IsBB()){
      if(!Pass_MVA(HNL_MVA_Conv("v4"), -0.7,"j_lep_mva_hnl_conv_v5")) return 0;
    }
    else{
      if(!Pass_MVA(HNL_MVA_Conv("EDv4"), -0.7, "j_lep_mva_hnl_conv_ed_v5")) return 0;
    }
    return 1;
  }
  if(ID == "HNL_LID_Run2_Fake"){

    if(!PassMVABaseLine()) return 0;
    if( IsBB()){
      if(!Pass_MVA(HNL_MVA_Fake("v4"), 0.2,"j_lep_mva_hnl_fake_v5")) return 0;
    }
    else{
      if(!Pass_MVA(HNL_MVA_Fake("EDv4"), 0.2,  "j_lep_mva_hnl_fake_ed_v5")) return 0;
    }

    return 1;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////       
  /////////////////////////////////////////////////////////////////////////////////////////////////////////      
  //////   *OPT* IDs are IDs that are tested, based off scans of MVAs in BB/EC                                   
  /////////////////////////////////////////////////////////////////////////////////////////////////////////      
  /////////////////////////////////////////////////////////////////////////////////////////////////////////      

  if(ID == "BBv1" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.15,  "j_lep_mva_hnl_fake_v4")) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    return 1;
  }
  if(ID == "BBv2" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.15,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.45,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv3" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.15,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.2,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv4" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.15,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), 0.,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv5" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.15,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv6" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.15,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.2),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }


  if(ID == "BBv7" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    return 1;
  }
  if(ID == "BBv8" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.45,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }

  if(ID == "BBv9" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.2,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv10" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), 0.,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv11" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv12" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.2),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  /////                                                                                                                                                                                                                                                                                                                    

  if(ID == "BBv13" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v4HFB"),   0.1,     "j_lep_mva_hnl_fake_v4")) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    return 1;
  }


  if(ID == "BBv14" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v4HFB"),   0.1,     "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.45,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv15" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.1,     "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.2,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv16" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.1,     "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), 0.,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv17" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.),     "j_lep_mva_hnl_conv_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.1,     "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }
  if(ID == "BBv18" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.2,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.2),     "j_lep_mva_hnl_conv_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.1,     "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }

  if(ID == "BBv19" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.15,     "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), 0.,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv20" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.2,     "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), 0.,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv21" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.),     "j_lep_mva_hnl_conv_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.15,     "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }
  if(ID == "BBv22" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.),     "j_lep_mva_hnl_conv_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("v5HFB"),   0.2,     "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }


  if(ID == "BBv23" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    return 1;
  }
  if(ID == "BBv24" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.45,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv25" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.2,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv26" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), 0.,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv27" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv28" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.2),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }

  if(ID == "BBv29" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    return 1;
  }
  if(ID == "BBv30" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.45,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv31" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), -0.2,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv32" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), 0.,     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }

  if(ID == "BBv33" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }
  if(ID == "BBv34" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,  "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,         "j_lep_mva_hnl_fake_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"), GetPtSlopeCut(10,60,-0.45,0.2),     "j_lep_mva_hnl_conv_v2")) return 0;
    return 1;
  }

  if(ID == "BBv35" ){
    if(!PassMVABaseLine()) return 0;
    if(!passMVAID_noIso_WP90())  return 0;
    if(! (RelIso()<0.1) ) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,             "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    return 1;
  }
  if(ID == "BBv36" ){
    if(!PassMVABaseLine()) return 0;
    if(!passMVAID_noIso_WP90HN())  return 0;
    return 1;
  }

  if(ID == "BBv37" ){
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.65,             "j_lep_mva_hnl_cf_v2"))   return 0;
    if(!passMVAID_Iso_WP90())  return 0;
    if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    return 1;
  }

  if(ID == "BBv38" ){
    if(!PassMVABaseLine()) return 0;
    if(! (RelIso()<0.1) ) return 0;
    if(!IsGsfScPixChargeConsistent()) return 0;
    if(!passMVAID_noIso_WP90())  return 0;
    if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    return 1;
  }

  if(ID == "BBv39" ){
    if(!PassMVABaseLine()) return 0;
    if(! (RelIso()<0.1) ) return 0;
    if(!IsGsfCtfChargeConsistent()) return 0;
    if(!passMVAID_noIso_WP90())  return 0;
    if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    return 1;
  }



  ////////////////// ENDCAO IDS       

  if(ID == "ECv1" ){
    if(!PassMVABaseLine()) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.5,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv2" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv2"),    -0.45 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.5,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv3" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.2 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.5,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv4" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.5,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv5" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.5,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv6" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.25) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.5,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv7" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.5,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  //                                                                                                                                                                                                                                                                                                                       
  if(ID == "ECv8" ){
    if(!PassMVABaseLine()) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.4,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv9" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.45 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.4,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv10" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.2 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.4,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv11" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.4,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv12" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.4,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv13" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.25) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.4,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv14" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.4,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv15" ){
    if(!PassMVABaseLine()) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv16" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.45 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv17" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.2 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv18" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv19" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv20" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.25) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv21" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.35,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }


  if(ID == "ECv22" ){
    if(!PassMVABaseLine()) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv23" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.45 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv24" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.2 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv25" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv26" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv27" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.25) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv28" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv29" ){
    if(!PassMVABaseLine()) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv30" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.45 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv31" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.2 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv32" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }

  if(ID == "ECv33" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv34" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.25) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }
  if(ID == "ECv35" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    return 1;
  }


  if(ID == "ECv36" ){
    if(!PassMVABaseLine()) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }
  if(ID == "ECv37" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.45 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }

  if(ID == "ECv38" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.2 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }
  if(ID == "ECv39" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }

  if(ID == "ECv40" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }
  if(ID == "ECv41" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.25) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }
  if(ID == "ECv42" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.3,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }


  //////                                                                                                                                                                                                                                                                                                                   
  if(ID == "ECv43" ){
    if(!PassMVABaseLine()) return 0;
    if(!PassID("HNL_ULID_Conv_Run2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }
  if(ID == "ECv44" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.45 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }
  if(ID == "ECv45" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    -0.2 , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }
  if(ID == "ECv46" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }

  if(ID == "ECv47" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    0. , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;
    return 1;
  }
  if(ID == "ECv48" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.25) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }
  if(ID == "ECv49" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_Conv("EDv5"),    GetPtSlopeCut(10,60,-0.45,0.) , "j_lep_mva_hnl_conv_ed_v2")) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6, "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5"), 0.25,"j_lep_mva_hnl_fake_ed_v4")) return 0;
    if(!Pass_MVA(HNL_MVA_Fake("EDv5HFB"),   0.1,           "j_lep_mva_hnl_fake_v4")) return 0;

    return 1;
  }


  if(ID == "ECv50" ){
    if(!PassMVABaseLine()) return 0;
    if(! (RelIso()<0.1) ) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6 ,  "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!passMVAID_noIso_WP80())  return 0;
    if(!(fabs(dXY()) < 0.04 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    return 1;
  }
  if(ID == "ECv51" ){
    if(!PassMVABaseLine()) return 0;
    if(!passMVAID_noIso_WP80HN())  return 0;
    return 1;
  }
  if(ID == "ECv52" ){
    if(!PassMVABaseLine()) return 0;
    if(!Pass_MVA(HNL_MVA_CF("EDv5"),   0.6 ,  "j_lep_mva_hnl_cf_ed_v2"))   return 0;
    if(!passMVAID_Iso_WP80())  return 0;
    return 1;
  }
  if(ID == "ECv53" ){
    if(!IsGsfScPixChargeConsistent()) return 0;
    if(! (RelIso()<0.1) ) return 0;
    if(!passMVAID_noIso_WP80())  return 0;
    if(!(fabs(dXY()) < 0.04 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;

    return 1;
  }
  if(ID == "ECv54" ){
    if(! (RelIso()<0.1) ) return 0;
    if(!IsGsfCtfChargeConsistent()) return 0;
    if(!passMVAID_noIso_WP80())  return 0;
    if(!(fabs(dXY()) < 0.04 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    return 1;
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////    
  /////////////////////////////////////////////////////////////////////////////////////////////////////////   
  //////  *OPT*POG* IDs are a mix of POG MVA ID and CF MVA                                                   
  ///////////////////////////////////////////////////////////////////////////////////////////////////////// 
  ///////////////////////////////////////////////////////////////////////////////////////////////////////// 

  if(ID == "HNL_ULID_OPT_POG1" ){
    if(!PassMVABaseLine()) return 0;

    if(! (RelIso()<0.1) ) return 0;

    if(!Pass_MVA_BBEC("CF_EDv5", 0.7 , 0.6 ,   "CF_v5")) return 0;

    if( IsBB()){
      if(!passMVAID_noIso_WP90())  return 0;
      if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    else{
      if(!passMVAID_noIso_WP80())  return 0;
      if(!(fabs(dXY()) < 0.04 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    return 1;
  }

  if(ID == "HNL_ULID_OPT_POG2" ){
    if(!PassMVABaseLine()) return 0;

    if( IsBB()){
      if(!passMVAID_noIso_WP90HN())  return 0;
    }
    else{

      if(!passMVAID_noIso_WP80HN())  return 0;
    }
    return 1;
  }

  if(ID == "HNL_ULID_OPT_POG3" ){
    if(!PassMVABaseLine()) return 0;

    if(!Pass_MVA_BBEC("CF_EDv5", 0.7 , 0.6 ,   "CF_v5")) return 0;

    if( IsBB()){
      if(!passMVAID_Iso_WP90())  return 0;
      if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    else{
      if(!passMVAID_Iso_WP80())  return 0;
      if(!(fabs(dXY()) < 0.04 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    return 1;
  }


  if(ID == "HNL_ULID_OPT_POG4" ){
    if(!PassMVABaseLine()) return 0;

    if(! (RelIso()<0.1) ) return 0;

    if(!IsGsfScPixChargeConsistent()) return 0;
    if( IsBB()){
      if(!passMVAID_noIso_WP90())  return 0;
      if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    else{
      if(!passMVAID_noIso_WP80())  return 0;
      if(!(fabs(dXY()) < 0.04 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    return 1;
  }

  if(ID == "HNL_ULID_OPT_POG5" ){
    if(!PassMVABaseLine()) return 0;

    if(! (RelIso()<0.1) ) return 0;

    if(!IsGsfCtfChargeConsistent()) return 0;
    if( IsBB()){
      if(!passMVAID_noIso_WP90())  return 0;
      if(!(fabs(dXY()) < 0.02 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    else{
      if(!passMVAID_noIso_WP80())  return 0;
      if(!(fabs(dXY()) < 0.04 && fabs(dZ())< 0.1 && fabs(IP3D()/IP3Derr()) < 6.)) return 0;
    }
    return 1;
  }


  if(ID.Contains("HNL_ULID_FINAL_Opt" )){
    if(!PassMVABaseLine()) return 0;

    if(ID.Contains("_CONV1_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,0.2) , GetPtSlopeCut(20,80,-0.7,0.2) ,   "Conv_v5")) return 0;
    }
    if(ID.Contains("_CONV2_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,0.3) , GetPtSlopeCut(20,80,-0.7,0.3) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV3_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,0.4) , GetPtSlopeCut(20,80,-0.7,0.4) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV4_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,0.) , GetPtSlopeCut(20,80,-0.7,0.) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV5_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,-0.2) , GetPtSlopeCut(20,80,-0.7,-0.2) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV6_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", -0.7 , -0.7 ,   "Conv_v5")) return 0;
    }
    if(ID.Contains("_CONV7_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", -0.4 , -0.4 ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV8_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.) , GetPtSlopeCut(20,60,-0.7,0.) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV9_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,-0.2) , GetPtSlopeCut(20,60,-0.7,-0.2) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV10_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.2) , GetPtSlopeCut(20,60,-0.7,0.1) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV11_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.4) , GetPtSlopeCut(20,60,-0.7,0.2) ,   "Conv_v5")) return 0;
    }


    if(ID.Contains("_CONV12_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,0.5) , GetPtSlopeCut(20,80,-0.7,0.5) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV13_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,0.6) , GetPtSlopeCut(20,80,-0.7,0.6) ,   "Conv_v5")) return 0;
    }

    if(ID.Contains("_CONV14_" )){
      if(!Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,80, -0.7,0.6) , GetPtSlopeCut(20,80,-0.7,0.4) ,   "Conv_v5")) return 0;
    }
    double BNP =0;
    double BCF =0;
    double ENP =0;
    double ECF =0;
    if(ID.Contains("_BNP1_" )) BNP = 0.2;
    if(ID.Contains("_BNP2_" )) BNP = 0.3;
    if(ID.Contains("_BNP3_" )) BNP = 0.4;
    if(ID.Contains("_ENP1_" )) ENP = 0.2;
    if(ID.Contains("_ENP2_" )) ENP = 0.3;
    if(ID.Contains("_ENP3_" )) ENP = 0.4;
    if(ID.Contains("_ENP4_" )) ENP = 0.5;
    if(ID.Contains("_BCF1_" )) BCF = 0.5;
    if(ID.Contains("_BCF2_" )) BCF = 0.6;
    if(ID.Contains("_BCF3_" )) BCF = 0.7;
    if(ID.Contains("_BCF4_" )) BCF = GetPtSlopeCut(10,80, 0.4,0.6);
    if(ID.Contains("_ECF1_" )) ECF = 0.5;
    if(ID.Contains("_ECF2_" )) ECF = 0.6;
    if(ID.Contains("_ECF3_" )) ECF = 0.7;
    if(ID.Contains("_ECF4_" )) ECF = GetPtSlopeCut(10,80, 0.4,0.5);

    if(!Pass_MVA_BBEC("CF_EDv5", BCF, ECF ,   "CF_v5")) return 0;
    if(!Pass_MVA_BBEC("Fake_EDv5", BNP, ENP ,   "Fake_v5")) return 0;

    return 1;
  }

  if(ID.Contains("HNL_ULID_Ver2017_" ) || ID.Contains("HNL_ULID_Ver2018_" ) ){
    if(!PassMVABaseLine()) return 0;

    if(ID.Contains("_CV1_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.) , GetPtSlopeCut(20,60,-0.7,0.) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV2_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.6,0.) , GetPtSlopeCut(20,50,-0.5,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV3_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.5,0.) , GetPtSlopeCut(20,50,-0.5,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV4_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.4,0.) , GetPtSlopeCut(20,50,-0.4,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV5_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.6,0.2) , GetPtSlopeCut(20,50,-0.5,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV6_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.5,0.2) , GetPtSlopeCut(20,50,-0.5,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV7_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.4,0.2) , GetPtSlopeCut(20,50,-0.4,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV8_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.6,0.2) , GetPtSlopeCut(20,50,-0.5,0.4) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV9_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.5,0.2) , GetPtSlopeCut(20,50,-0.5,0.4) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV10_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,50, -0.4,0.2) , GetPtSlopeCut(20,50,-0.4,0.4) ,   "Conv_v5")) return 0;

    if(ID.Contains("_NP1_") && !Pass_MVA_BBEC("Fake_EDv5",   0.3 ,0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP2_") && !Pass_MVA_BBEC("Fake_EDv5",   0.3 ,0.45 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP3_") && !Pass_MVA_BBEC("Fake_EDv5",   0.3 ,0.5 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP4_") && !Pass_MVA_BBEC("Fake_EDv5",   0.35 , 0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP5_") && !Pass_MVA_BBEC("Fake_EDv5",   0.35 , 0.45 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP6_") && !Pass_MVA_BBEC("Fake_EDv5",   0.35 ,0.5 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP7_") && !Pass_MVA_BBEC("Fake_EDv5",   0.4 ,0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP8_") && !Pass_MVA_BBEC("Fake_EDv5",   0.4 ,0.5 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP9_") && !Pass_MVA_BBEC("Fake_EDv5",   0.4 ,0.45 ,   "Fake_v5")) return 0;

    if(ID.Contains("_LFNP1_") && !Pass_MVA_BBEC("FakeLF_EDv5", 0.6 , 0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_LFNP2_") && !Pass_MVA_BBEC("FakeLF_EDv5", 0.4 , 0.4 ,   "Fake_v5")) return 0;

    if(ID.Contains("_CF1_") &&!Pass_MVA_BBEC("CF_EDv5",     0.7 , 0.6 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF2_") &&!Pass_MVA_BBEC("CF_EDv5",     0.7 , 0.55 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF3_") &&!Pass_MVA_BBEC("CF_EDv5",     0.6 , 0.6 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF4_") &&!Pass_MVA_BBEC("CF_EDv5",     0.6 , 0.55 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF5_") &&!Pass_MVA_BBEC("CF_EDv5",     0.6 , 0.5 ,  "CF_v5"))   return 0;
    return 1;
  }

  if(ID.Contains("HNL_ULID_Ver2016_" ) ){
    if(!PassMVABaseLine()) return 0;

    if(ID.Contains("_CV1_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.) , GetPtSlopeCut(20,60,-0.7,0.) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV2_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.) , GetPtSlopeCut(20,60,-0.7,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV3_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.) , GetPtSlopeCut(20,60,-0.7,0.4) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV4_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.2) , GetPtSlopeCut(20,60,-0.7,0.2) ,   "Conv_v5")) return 0;
    if(ID.Contains("_CV5_")&& !Pass_MVA_BBEC("Conv_EDv5", GetPtSlopeCut(20,60, -0.7,0.2) , GetPtSlopeCut(20,60,-0.7,0.4) ,   "Conv_v5")) return 0;

    if(ID.Contains("_NP1_") && !Pass_MVA_BBEC("Fake_EDv5",   0.3 ,0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP2_") && !Pass_MVA_BBEC("Fake_EDv5",   0.3 ,0.45 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP3_") && !Pass_MVA_BBEC("Fake_EDv5",   0.3 ,0.5 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP4_") && !Pass_MVA_BBEC("Fake_EDv5",   0.35 , 0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP5_") && !Pass_MVA_BBEC("Fake_EDv5",   0.35 , 0.45 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP6_") && !Pass_MVA_BBEC("Fake_EDv5",   0.35 ,0.5 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP7_") && !Pass_MVA_BBEC("Fake_EDv5",   0.4 ,0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP8_") && !Pass_MVA_BBEC("Fake_EDv5",   0.4 ,0.5 ,   "Fake_v5")) return 0;
    if(ID.Contains("_NP9_") && !Pass_MVA_BBEC("Fake_EDv5",   0.4 ,0.45 ,   "Fake_v5")) return 0;

    if(ID.Contains("_LFNP1_") && !Pass_MVA_BBEC("FakeLF_EDv5", 0.6 , 0.4 ,   "Fake_v5")) return 0;
    if(ID.Contains("_LFNP2_") && !Pass_MVA_BBEC("FakeLF_EDv5", 0.4 , 0.4 ,   "Fake_v5")) return 0;

    if(ID.Contains("_CF1_") &&!Pass_MVA_BBEC("CF_EDv5",     0.7 , 0.6 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF2_") &&!Pass_MVA_BBEC("CF_EDv5",     0.7 , 0.55 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF3_") &&!Pass_MVA_BBEC("CF_EDv5",     0.6 , 0.6 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF4_") &&!Pass_MVA_BBEC("CF_EDv5",     0.6 , 0.55 ,  "CF_v5"))   return 0;
    if(ID.Contains("_CF5_") &&!Pass_MVA_BBEC("CF_EDv5",     0.6 , 0.5 ,  "CF_v5"))   return 0;
    return 1;
  }


  return -1;

}




int Electron::PassIDOpt(TString ID) const{

  if(ID=="HNOpt")    return   PassHNOpt();

  if(ID.Contains("HNLOpt_UL_FINAL_")){
    TString ID_sub = ID;
    ID_sub=ID_sub.ReplaceAll("FINAL_","");
    ID_sub = ID_sub.ReplaceAll("_"," ");
    string sID_sub = string(ID_sub);
    vector<TString> subStrings;
    istringstream ID_subs(sID_sub);
    do {
      string subs;
      ID_subs >> subs;
      subStrings.push_back(TString(subs));
    } while (ID_subs);

    TString Barrel_ID="";
    TString Endcap_ID="";
    for(unsigned int i=0; i < subStrings.size(); i++){

      if(subStrings[i].Contains("BBID")) {
	Barrel_ID=subStrings[i];
        Barrel_ID=Barrel_ID.ReplaceAll("BBID","");
      }
      if(subStrings[i].Contains("ECID")) {
        Endcap_ID=subStrings[i];
        Endcap_ID=Endcap_ID.ReplaceAll("ECID","");
      }

    }
    if(IsBB() && !PassID(Barrel_ID))  return 0;
    if(IsEC() && !PassID(Endcap_ID))  return 0;
    return 1;
  }


  if(ID=="MVAID_N1"){
    if(!passMVAID_noiso_WPLoose()) return 0;
    if(!Pass_TriggerEmulationLoose())  return 0;
    if(this->Pt() < 10) return 0;
    if(this->fEta() > 2.5) return 0;
    if(MiniRelIso() > 0.4) return 0;
    return 1;
  }

  if(ID=="MVAID_N2"){
    if(!passMVAID_noiso_WPLoose()) return 0;
    if(!Pass_TriggerEmulationLoose())  return 0;
    if(this->Pt() < 10) return 0;
    if(this->fEta() > 2.5) return 0;
    if(MiniRelIso() > 0.4) return 0;
    if(NMissingHits() > 1) return 0;
    return 1;
  }

  if(ID=="MVAID_N3"){
    if(!passMVAID_noiso_WPLoose()) return 0;
    if(!Pass_TriggerEmulationLoose())  return 0;
    if(this->Pt() < 10) return 0;
    if(this->fEta() > 2.5) return 0;
    if(MiniRelIso() > 0.4) return 0;
    if(NMissingHits() > 1) return 0;
    if(SIP3D() > 8) return 0;
    return 1;
  }

  if(ID=="MVAID_N4"){
    if(!passMVAID_noiso_WPLoose()) return 0;
    if(!Pass_TriggerEmulationLoose())  return 0;
    if(this->Pt() < 10) return 0;
    if(this->fEta() > 2.5) return 0;
    if(MiniRelIso() > 0.4) return 0;
    if(NMissingHits() > 1) return 0;
    if(SIP3D() > 8) return 0;
    if(this->fdXY() > 0.05) return 0;
    return 1;
  }




  if(ID.Contains("ElOpt")) return Pass_MultiFunction_Opt(ID) ? 1 : 0;


  if(ID=="HNLIPv1") {
    double dxy_cut  =  (IsBB()) ? 0.02 : 0.04;
    double dxy_cut2 =  (IsBB()) ? 0.01 : 0.02;
    double dz_cut   =  (IsBB()) ? 0.05 : 0.1;

    if(this->Pt()  > 15 && this->Pt()  < 60.) dxy_cut -= (this->Pt() - 15.) * dxy_cut2/ 45.;
    if(this->Pt()  > 60.) dxy_cut = 0.01;
    if(fabs(dXY()) >  dxy_cut)  return 0;
    if(fabs(dZ()) >  dz_cut)   return 0;

    return 1;

  }
  if(ID=="HNLIPv2") {
    double dxy_cut =  (IsBB()) ? 0.02 : 0.04;
    double dxy_cut2 =  (IsBB()) ? 0.02 : 0.04;
    double dz_cut =  (IsBB()) ? 0.05 : 0.1;

    if(this->Pt() > 15 && this->Pt()  < 60.) dxy_cut -= (this->Pt() - 15.) * dxy_cut2/ 45.;
    if(this->Pt()  > 60.) dxy_cut = 0.01;

    if(fabs(dXY()) >  dxy_cut)  return 0;

    if(fabs(dZ()) >  dz_cut)   return 0;

    return 1;

  }

  if(ID=="HNLIPv3") {
    if(fabs(IP3D()/IP3Derr())> 4.)  return 0;
    return 1;
  }
  if(ID=="HNLIPv4") {
    if(fabs(IP3D()/IP3Derr())> 5.)  return 0;
    return 1;
  }




  /// POG + POG IP +CC                                                           
  // updated 2016 analysis ID with looser dxy                                    
  if(ID=="HNTight2016Update") return passIDHN(4,0.02, 0.02, 0.05,0.04, 4.9,4.9, -999., -999., 0.089, 0.05) && PassMVA_2016(0.8,0.8,0.775)? 1 : 0 ;  // EXO-17-028                                                                                         


  // POG ID with relaxed cuts                                                                                        
  if(ID=="HNTight_dxyB")  return passIDHN(1,999., 0.1, 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_dxyE")  return passIDHN(1,0.05, 999., 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_dzB")   return passIDHN(1,0.05, 0.1, 999.,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_dzE")   return passIDHN(1,0.05, 0.1, 0.1,999., 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_ipB")   return passIDHN(1,0.05, 0.1, 0.1,0.2, 999.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_ipE")   return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,999., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_isoB")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., 999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_isoE")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., -999., 999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_miniisoB")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., 999., -999.) ? 1 : 0 ;
  if(ID=="HNTight_miniisoE")  return passIDHN(1,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., -999., 999.) ? 1 : 0 ;

  if(ID=="HNMedium_dxyB")  return passIDHN(2,999., 0.1, 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_dxyE")  return passIDHN(2,0.05, 999., 0.1,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_dzB")  return passIDHN(2,0.05, 0.1, 999.,0.2, 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_dzE")  return passIDHN(2,0.05, 0.1, 0.1,999., 4.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_ipB")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 999.,4., -999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_ipE")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,999., -999., -999., -999., -999.) ? 1 : 0 ;

  if(ID=="HNMedium_isoB")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., 999., -999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_isoE")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., -999., 999., -999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_miniisoB")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., 999., -999.) ? 1 : 0 ;
  if(ID=="HNMedium_miniisoE")  return passIDHN(2,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999., -999., 999.) ? 1 : 0 ;


  if(ID=="HN_mva")     return passIDHN(3,0.05, 0.1, 0.1,0.2, 4.,4., 0.08, 0.08, -999., -999.) ? 1 : 0 ;
  if(ID=="HNMini_mva")  return passIDHN(4,0.05, 0.1, 0.1,0.2, 4.,4., -999., -999.,0.08, 0.08) ? 1 : 0 ;

  if(ID=="HNMVATight") return Pass_HNMVATight(0.08, 0.05, 0.1)? 1 : 0 ;

  if(ID=="CutBasedLooseNoIso") return Pass_CutBasedLooseNoIso()? 1 : 0 ;
  if(ID=="CutBasedMediumNoIso") return Pass_CutBasedMediumNoIso()? 1 : 0 ;
  if(ID=="CutBasedTightNoIso") return Pass_CutBasedTightNoIso()? 1 : 0 ;
  if(ID=="MVALooseNoIso") return passMVAID_noiso_WPLoose()? 1 : 0 ;
  if(ID=="CutBasedVetoNoIso") return Pass_CutBasedVetoNoIso()? 1 : 0 ;


  return -1;
}


int Electron::PassHNOpt() const{

  if(! (Pass_TriggerEmulationLoose()) ) return 0;

  if(IsBB()){

    double dxy_cut = 0.02 ;
    if(this->Pt() > 15 && this->Pt()  < 60.) dxy_cut -= (this->Pt() - 15.) * 0.01/ 45.;
    if(this->Pt()  > 60.) dxy_cut = 0.01;

    if(fabs(dXY()) >  dxy_cut)   return 0;
    if(fabs(dZ()) >  0.07)   return 0;
    if(fabs(IP3D()/IP3Derr())> 7.)  return 0;

    return 1;
  }
  else{
    double dxy_cut = 0.04 ;
    if(this->Pt() > 15 && this->Pt()< 60.) dxy_cut -= (this->Pt()- 15.) * 0.02/ 45.;
    if(this->Pt()  > 60.) dxy_cut = 0.02;

    if(fabs(dXY()) >  dxy_cut)   return 0;
    if(fabs(dZ()) >  0.07)   return 0;
    if(fabs(IP3D()/IP3Derr())> 10)  return 0;
  }
  return 1;

}


bool Electron::passIDHN(int ID, double dxy_b, double dxy_e, double dz_b,double dz_e, double sip_b, double sip_e, double iso_b,double iso_e, double miso_b, double miso_e) const{

  if( fabs(scEta())<= 1.479 ){
    if(! (fabs(dXY())< dxy_b && fabs(dZ())<dz_b && fabs(IP3D()/IP3Derr())< sip_b) ) return false;
  }
  else{
    if(! (fabs(dXY())< dxy_e && fabs(dZ())<dz_e && fabs(IP3D()/IP3Derr())< sip_e) ) return false;
  }
  //  if(! (passTightID()) ) return false;                                                                                                                                                                                                                                                                                 
  if( ID==1){


    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(! (passTightID()) ) return false;
    }
    else {
      if(! (Pass_CutBasedTightNoIso()) ) return false;
      if( miso_e > 0.){
        if(fabs(scEta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
      }
      else if( iso_e > 0.){
        if(fabs(scEta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
      }
      else if(fabs(scEta())> 1.479  &&  ! (passTightID()))  return false;

      if( miso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
      }
      else if( iso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
      }
      else if(fabs(scEta()) <= 1.479  &&! (passTightID()))  return false;

    }
  }
  if( ID==2){
    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(! (passMediumID()) ) return false;
    }
    else {
      if(! (Pass_CutBasedMediumNoIso()) ) return false;
      if( miso_e > 0.){
        if(fabs(scEta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
      }
      else if( iso_e > 0.){
        if(fabs(scEta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
      }
      else if(fabs(scEta())> 1.479  &&  ! (passMediumID()))  return false;
      if( miso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
      }
      else if( iso_b > 0.){
        if(fabs(scEta()) <= 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
      }
      else if(fabs(scEta()) <= 1.479  &&! (passMediumID()))  return false;

    }
  }

  if( ID==3){
    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(fabs(scEta()) <= 1.479 && !(RelIso() < 0.287+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(RelIso() < 0.0445+0.963/UncorrPt()) ) return false;
    }
    if( iso_b == 999.  && iso_e == 999.  &&  miso_b == 999.  && miso_e == 999. ){
      if(fabs(scEta()) <= 1.479 && !(RelIso() < 0.0478+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(RelIso() < 0.0658+0.963/UncorrPt()) ) return false;

    }

    if(fabs(scEta())> 1.479  &&  (RelIso()>fabs(iso_e)) ) return false;
    if(fabs(scEta())< 1.479  &&  (RelIso()>fabs(iso_b)) ) return false;
  }
  if( ID==4){
    if( iso_b == -999.  && iso_e == -999.  &&  miso_b == -999.  && miso_e == -999. ){
      if(fabs(scEta()) <= 1.479 && !(MiniRelIso() < 0.287+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(MiniRelIso() < 0.0445+0.963/UncorrPt()) ) return false;

    }
    if( iso_b == 999.  && iso_e == 999.  &&  miso_b == 999.  && miso_e == 999. ){
      if(fabs(scEta()) <= 1.479 && !(MiniRelIso() < 0.0478+0.506/UncorrPt()) ) return false;
      if(fabs(scEta()) > 1.479  && !(MiniRelIso() < 0.0658+0.963/UncorrPt()) ) return false;

    }

    if(fabs(scEta())> 1.479  &&  (MiniRelIso()>fabs(miso_e)) ) return false;
    if(fabs(scEta())< 1.479  &&  (MiniRelIso()>fabs(miso_b)) ) return false;
  }


  return true;
}
