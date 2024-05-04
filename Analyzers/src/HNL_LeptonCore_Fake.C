#include "HNL_LeptonCore.h"

double HNL_LeptonCore::GetPtPartonSF(Lepton  Lep, TString LooseID){


  // If CheckProfile flag then new IDs are being ran so function would crash 
  if(HasFlag("CheckProfile")) return 1;
  if(HasFlag("CheckProfileEE")) return 1;
  
  
  if(LooseID == "HNL_TopMVA_FO_MM") return 0.661;
  if(LooseID == "HNL_HN3L") return 0.661;

  bool IsMuon=(Lep.LeptonFlavour() == Lepton::MUON);

  /// MUON   //https://jalmond.web.cern.ch/jalmond/SNU/WebPlots/HNL/HNL_LeptonFakeRatesMC/HNL_LeptonFakeRatesMC/2017/HNL_GetProfileSFMC_Jan24_Muon/?match=v1_M

  /// DeepJet split

  if(IsMuon && (LooseID == "HNL_ULID_FO_2016a")) return GetPtPartonSF(Lep,"HNL_ULID_FO_v3_b_2016");
  if(IsMuon && (LooseID == "HNL_ULID_FO_2016b")) return GetPtPartonSF(Lep,"HNL_ULID_FO_v4_b_2016");
  if(IsMuon && (LooseID == "HNL_ULID_FO_2017") && Lep.IsBB()) return GetPtPartonSF(Lep,"HNL_ULID_FO_v9_c_2017");
  if(IsMuon && (LooseID == "HNL_ULID_FO_2017") && !Lep.IsBB()) return GetPtPartonSF(Lep,"HNL_ULID_FO_v1_a_2017");
  if(IsMuon && (LooseID == "HNL_ULID_FO_2018")) return GetPtPartonSF(Lep,"HNL_ULID_FO_v3_b_2018");

  if(!IsMuon && (LooseID == "HNL_ULID_FO_2016a"))  return GetPtPartonSF(Lep,"HNL_ULID_FO_v5_a_2016");
  if(!IsMuon && (LooseID == "HNL_ULID_FO_2016b"))  return GetPtPartonSF(Lep,"HNL_ULID_FO_v3_a_2016");
  if(!IsMuon && (LooseID == "HNL_ULID_FO_2017"))  return GetPtPartonSF(Lep,"HNL_ULID_FO_v0_2017");
  if(!IsMuon && (LooseID == "HNL_ULID_FO_2018"))  return GetPtPartonSF(Lep,"HNL_ULID_FO_v3_a_2018");

  /// Scan Numbers


  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.783613;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.708957;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.708957;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.708957;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.768181;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.768181;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.768181;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.770471;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.770471;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.770471;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.770838;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.770852;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.770838;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.771703;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.771782;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.771706;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.773904;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.774012;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.773918;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.779457;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.779563;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.779489;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.783934;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.784215;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.784074;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.783988;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.784214;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.784114;
  
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.731840;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.670988;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.670988;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.670988;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.712000;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.712025;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.712017;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.721715;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.721826;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.721743;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.727142;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.727351;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.727195;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.728656;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.729078;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.728758;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.730617;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.731246;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.730775;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.733005;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.733813;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.733244;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.734326;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.735616;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.734749;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.734106;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.735528;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.734528;

  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.746282;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.684136;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.684136;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.684136;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.725486;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.725507;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.725499;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.735893;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.735973;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.735960;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.741833;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.742044;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.741940;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.743603;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.744014;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.743810;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.745032;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.745613;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.745377;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.747030;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.747768;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.747462;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.748208;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.749440;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.748806;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.747726;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.749375;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.748375;


  /*if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v1_"+GetYearString())) return 0.709;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v2_"+GetYearString())) return 0.784;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v3_"+GetYearString())) return 0.774;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v4_"+GetYearString())) return 0.771;
  if(DataYear == 2016 && IsMuon && (LooseID == "HNL_ULID_FO_v5_"+GetYearString())) return 0.770;

  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v1_"+GetYearString())) return 0.671;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v2_"+GetYearString())) return 0.735;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v3_"+GetYearString())) return 0.730;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v4_"+GetYearString())) return 0.727;
  if(DataYear == 2017 && IsMuon && (LooseID == "HNL_ULID_FO_v5_"+GetYearString())) return 0.722;

  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v1_"+GetYearString())) return 0.684;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v2_"+GetYearString())) return 0.749;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v3_"+GetYearString())) return 0.745;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v4_"+GetYearString())) return 0.742;
  if(DataYear == 2018 && IsMuon && (LooseID == "HNL_ULID_FO_v5_"+GetYearString())) return 0.736;*/

  ///// ELECTRON
  
  /*if(DataYear == 2016 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_"+GetYearString()))   return 0.657;
  if(DataYear == 2016 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_"+GetYearString()))   return 0.791;
  if(DataYear == 2016 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_"+GetYearString()))   return 0.773;
  if(DataYear == 2016 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_"+GetYearString()))   return 0.774;
  if(DataYear == 2016 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_"+GetYearString()))   return 0.658;
  if(DataYear == 2016 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_"+GetYearString()))   return 0.701;
  if(DataYear == 2016 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_"+GetYearString()))   return 0.821;
  if(DataYear == 2016 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_"+GetYearString()))   return 0.807;
  if(DataYear == 2016 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_"+GetYearString()))   return 0.784;
  if(DataYear == 2016 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_"+GetYearString()))   return 0.701;

  if(DataYear == 2017 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_"+GetYearString()))   return 0.677;
  if(DataYear == 2017 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_"+GetYearString()))   return 0.812;
  if(DataYear == 2017 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_"+GetYearString()))   return 0.795;
  if(DataYear == 2017 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_"+GetYearString()))   return 0.767;
  if(DataYear == 2017 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_"+GetYearString()))   return 0.677;
  if(DataYear == 2017 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_"+GetYearString()))   return 0.649;
  if(DataYear == 2017 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_"+GetYearString()))   return 0.773;
  if(DataYear == 2017 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_"+GetYearString()))   return 0.757;
  if(DataYear == 2017 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_"+GetYearString()))   return 0.733;
  if(DataYear == 2017 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_"+GetYearString()))   return 0.650;

  if(DataYear == 2018 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_"+GetYearString()))   return 0.658;
  if(DataYear == 2018 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_"+GetYearString()))   return 0.813;
  if(DataYear == 2018 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_"+GetYearString()))   return 0.798;
  if(DataYear == 2018 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_"+GetYearString()))   return 0.781;
  if(DataYear == 2018 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_"+GetYearString()))   return 0.658;
  if(DataYear == 2018 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_"+GetYearString()))   return 0.681;
  if(DataYear == 2018 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_"+GetYearString()))   return 0.851;
  if(DataYear == 2018 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_"+GetYearString()))   return 0.834;
  if(DataYear == 2018 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_"+GetYearString()))   return 0.788;
  if(DataYear == 2018 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_"+GetYearString()))   return 0.682;

  /// Iteration 1
  if(DataYear == 2016 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FOv2_"+GetYearString())) return 0.6556;
  if(DataYear == 2016 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FOv3_"+GetYearString())) return 0.6577;
  if(DataYear == 2016 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FOv2_"+GetYearString())) return 0.7226;
  if(DataYear == 2016 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FOv3_"+GetYearString())) return 0.725;
  if(DataYear == 2017 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FOv2_"+GetYearString())) return 0.691;
  if(DataYear == 2017 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FOv3_"+GetYearString())) return 0.689;
  if(DataYear == 2017 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FOv2_"+GetYearString())) return 0.6336;
  if(DataYear == 2017 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FOv3_"+GetYearString())) return 0.631;
  if(DataYear == 2018 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FOv2_"+GetYearString())) return 0.678;
  if(DataYear == 2018 && !IsMuon && Lep.IsBB() && (LooseID == "HNL_ULID_FOv3_"+GetYearString())) return 0.678;
  if(DataYear == 2018 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FOv2_"+GetYearString())) return 0.717;
  if(DataYear == 2018 && !IsMuon && Lep.IsEC() && (LooseID == "HNL_ULID_FOv3_"+GetYearString())) return 0.717;*/


  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.783697;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.380025;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.380025;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.380025;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.539335;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.539335;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.539335;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.657513;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.657513;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.657513;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.744058;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.744058;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.744058;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.764903;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.765155;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.765155;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.772946;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.773188;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.773190;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.783616;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.783812;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.783895;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.790672;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.790723;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.790958;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.790225;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.790659;
  if(DataYear == 2016 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.790659;

  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.819370;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.539174;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.539174;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.539174;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.564789;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.564789;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.564789;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.701080;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.701080;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.701080;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.784767;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.784767;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.784767;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.797004;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.797004;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.797004;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.807281;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.807275;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.807281;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.817880;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.818044;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.817880;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.821755;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.821457;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.821737;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.821899;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.821457;
  if(DataYear == 2016 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.821457;

  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.813958;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.401366;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.401366;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.401366;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.539074;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.539074;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.539074;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.676694;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.676694;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.676694;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.767112;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.767112;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.767112;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.788526;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.788526;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.788526;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.795248;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.795170;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.795248;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.804573;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.804487;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.804573;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.812872;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.812443;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.812404;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.812509;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.812443;
  if(DataYear == 2017 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.812443;

  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.774139;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.469279;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.469279;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.469279;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.567023;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.567023;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.567023;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.649528;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.649528;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.649528;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.733043;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.733043;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.733043;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.741018;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.741410;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.741018;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.756848;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.757258;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.756848;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.767435;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.767873;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.767435;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.773027;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.773003;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.772559;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.773018;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.773003;
  if(DataYear == 2017 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.773003;

  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.814278;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.388897;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.388897;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.388897;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.518268;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.518268;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.518268;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.658492;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.658492;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.658492;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.780781;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.782367;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.780781;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.792093;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.793562;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.792093;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.797667;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.799083;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.797667;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.808554;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.810123;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.808716;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.812062;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.813304;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.811802;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.812682;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.813334;
  if(DataYear == 2018 && !IsMuon  && Lep.IsBB() && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.813334;

  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v0_"+GetYearString())) return 0.853143;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_a_"+GetYearString())) return 0.525277;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_b_"+GetYearString())) return 0.525277;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v1_c_"+GetYearString())) return 0.525277;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_a_"+GetYearString())) return 0.547863;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_b_"+GetYearString())) return 0.547863;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v2_c_"+GetYearString())) return 0.547863;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_a_"+GetYearString())) return 0.681626;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_b_"+GetYearString())) return 0.681626;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v3_c_"+GetYearString())) return 0.681626;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_a_"+GetYearString())) return 0.788392;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_b_"+GetYearString())) return 0.788392;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v4_c_"+GetYearString())) return 0.788392;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_a_"+GetYearString())) return 0.816768;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_b_"+GetYearString())) return 0.816768;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v5_c_"+GetYearString())) return 0.816768;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_a_"+GetYearString())) return 0.833908;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_b_"+GetYearString())) return 0.833908;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v6_c_"+GetYearString())) return 0.833908;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_a_"+GetYearString())) return 0.850559;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_b_"+GetYearString())) return 0.850866;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v7_c_"+GetYearString())) return 0.850559;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_a_"+GetYearString())) return 0.853421;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_b_"+GetYearString())) return 0.854105;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v8_c_"+GetYearString())) return 0.853834;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_a_"+GetYearString())) return 0.853001;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_b_"+GetYearString())) return 0.854105;
  if(DataYear == 2018 && !IsMuon  && Lep.IsEC() && (LooseID == "HNL_ULID_FO_v9_c_"+GetYearString())) return 0.854105;

  cout << "ERROR in GetPtPartonSF " << IsMuon << " " << LooseID << endl;
  exit(EXIT_FAILURE);

}

int HNL_LeptonCore::GetFakeLepSrcType(const Lepton& Lep, vector<Jet>& JetColl){
  //Type: -1: Unmatched, 1:L, 2:C, 3:B                                                                                                                                                                                                                                                                                                                                                                                                   
  int SrcType=-1;
  bool NearB=false, NearC=false, NearL=false;
  for(unsigned int ij=0; ij<JetColl.size(); ij++){
    if(Lep.DeltaR(JetColl.at(ij))<0.4){
      if     (JetColl.at(ij).hadronFlavour()==5){ NearB=true; break; }//1)                                                                                                                                                                                                                                                                                                                                                               
      else if(JetColl.at(ij).hadronFlavour()==4){ NearC=true; }
      else if(JetColl.at(ij).hadronFlavour()==0){ NearL=true; }
    }
  }

  if     (NearB) SrcType=3;
  else if(NearC) SrcType=2;
  else if(NearL) SrcType=1;

  return SrcType;
  //1) Higher Priority to B. if there's multiple near jets, then b-jet has higher priority                                                                                                                                                                                                                                                                                                                                              

}





/*                                   
             FAKE RATE FUNCTIONS                                                                                                                                                                              
*/


double HNL_LeptonCore::GetFakeRateElectron(Electron el, AnalyzerParameter param){

  //// Access individual lepton Fake Rate                                                                                                   

  double  LepEta = el.fEta();
  double  LepPt  = el.PtMaxed(80);
  TString  fr_key = param.k.Electron_FR;
  
  return fakeEst->GetElectronFakeRate(param.Electron_Tight_ID, fr_key,param.FakeRateMethod, param.FakeRateParam, LepEta, LepPt, el.LeptonFakeTagger() );


}
double HNL_LeptonCore::GetFakeRateMuon(Muon mu, AnalyzerParameter param){

  double  LepEta = mu.fEta();
  double  LepPt  = mu.PtMaxed(80);
  TString  fr_key = param.k.Muon_FR;
  if(param.k.Muon_BB_FR != "Default") {
    if(mu.IsBB()) fr_key = param.k.Muon_BB_FR;
  }
  if(param.k.Muon_EC_FR!= "Default") {
    if(mu.IsEC()) fr_key = param.k.Muon_EC_FR;
  }

  
  //// Access individual lepton Fake Ratex                                                                                                                                                                                                                                                                                                                                                                                               
  return fakeEst->GetMuonFakeRate(param.Muon_Tight_ID, fr_key,param.FakeRateMethod,param.FakeRateParam,LepEta, LepPt, mu.LeptonFakeTagger() );

}

double HNL_LeptonCore::GetFakeWeight(std::vector<Lepton *> leps, AnalyzerParameter _param){

  //// Access event weight based on Lepton collection                                                               
  if(!IsData) return 1.;

  fakeEst->IgnoreNoHist=true;

  ///// SINGLE LEPTON EVENTS 
  double this_weight = -1.;

  if(leps.size() == 1){
    TString  muon_fr_key = _param.k.Muon_FR;
    if(_param.k.Muon_BB_FR!= "Default") {
      if(leps[0]->IsBB()) muon_fr_key = _param.k.Muon_BB_FR;
    }
    if(_param.k.Muon_EC_FR!= "Default") {
      if(leps[0]->IsEC()) muon_fr_key = _param.k.Muon_EC_FR;
    }
    TString fr_key = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_FR : muon_fr_key;
    TString pr_key = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_PR : _param.k.Muon_PR;
    bool IsMuon    = (leps[0]->LeptonFlavour() != Lepton::ELECTRON);
    TString ID     = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
   
    if(!leps[0]->LepIDSet()) {      cout << "Lepton ID not set" << endl;      exit(EXIT_FAILURE);    }

    double this_fr = fakeEst->GetFakeRate  (IsMuon, ID,  fr_key, _param.FakeRateMethod, _param.FakeRateParam, leps[0]->fEta(), leps[0]->Pt(),leps[0]->LeptonFakeTagger());
    double this_pr = fakeEst->GetPromptRate(_param.ApplyPR, IsMuon, ID, pr_key, leps[0]->fEta(), leps[0]->UncorrectedPt());

    this_weight=  fakeEst->CalculateLepWeight(this_pr, this_fr, leps[0]->PassLepID() );

    if(isnan(this_weight)) {
      cout << "Single Lepton Fake rate is NaN" << endl;
      cout << "IsMuon = " << IsMuon << " fr_key = " << fr_key << " ID = " << ID << " this_fr = " << this_fr << " this_pr = " << this_pr << endl;
      exit(EXIT_FAILURE);
    }
    return this_weight;
  }

  ////// DILEPTON EVENTS 
  if (leps.size() == 2){

    if(!(leps[0]->LepIDSet() || leps[1]->LepIDSet())) {
      cout << "Lepton ID not set" << endl;
      exit(EXIT_FAILURE);
    }

    TString  muon_fr_key1 = _param.k.Muon_FR;
    TString  muon_fr_key2 = _param.k.Muon_FR;
    if(_param.k.Muon_BB_FR!= "Default") {
      if(leps[0]->IsBB()) muon_fr_key1 = _param.k.Muon_BB_FR;
      else  muon_fr_key1 = _param.k.Muon_EC_FR;
      if(leps[1]->IsBB()) muon_fr_key2 = _param.k.Muon_BB_FR;
      else  muon_fr_key2 = _param.k.Muon_EC_FR;

    }
    TString fr_key1 = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_FR : muon_fr_key1;
    TString fr_key2 = (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_FR : muon_fr_key2;
    TString pr_key = (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_PR : _param.k.Muon_PR;

    if(run_Debug){
      if(leps[0]->LeptonFlavour() == Lepton::ELECTRON) cout << "_param.Electron_Tight_ID = " << _param.Electron_Tight_ID <<  " fr_key1 = " << fr_key1 << endl;
      else cout << "_param.Muon_Tight_ID  = " <<  _param.Muon_Tight_ID <<  " fr_key1 = " << fr_key1 <<endl;
    }

    
    TString ID1   =  (leps[0]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
    TString ID2   =  (leps[1]->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
    bool IsMuon1  = (leps[0]->LeptonFlavour() != Lepton::ELECTRON);
    bool IsMuon2  = (leps[1]->LeptonFlavour() != Lepton::ELECTRON);

    double this_fr1 =  fakeEst->GetFakeRate(IsMuon1, ID1,  fr_key1, _param.FakeRateMethod, _param.FakeRateParam, leps[0]->fEta(), leps[0]->Pt(),leps[0]->LeptonFakeTagger());
    double this_fr2 =  fakeEst->GetFakeRate(IsMuon2, ID2,  fr_key2, _param.FakeRateMethod, _param.FakeRateParam, leps[1]->fEta(), leps[1]->Pt(),leps[1]->LeptonFakeTagger() );
    double this_pr1 =  1;//fakeEst->GetPromptRate(_param.ApplyPR, IsMuon1, ID1, pr_key, leps[0]->fEta(), leps[0]->UncorrectedPt());
    double this_pr2 =  1;//fakeEst->GetPromptRate(_param.ApplyPR, IsMuon2, ID2, pr_key, leps[1]->fEta(), leps[1]->UncorrectedPt());
    
    //if(this_fr1 == 1){
      //cout << _param.Electron_Tight_ID << " KEY  = "  << fr_key1 << " _param.FakeRateParam = " << _param.FakeRateParam << " _param.FakeRateMethod = " << _param.FakeRateMethod << endl;
      cout << "this_fr1 = " << this_fr1 << " this_fr2 = " << this_fr2 << " this_pr1 = " << this_pr1 << " this_pr2 = " << this_pr2 << endl;
    //}
    this_weight = fakeEst->CalculateDilepWeight(this_pr1,this_fr1, this_pr2, this_fr2, leps[0]->PassLepID(),leps[1]->PassLepID(),0);

    if(isnan(this_weight)) {
      cout << "Dilepton Fake rate is NaN"<< endl;
      cout << "IsMuon1 = " << IsMuon1 << " fr_key = " << fr_key1 << " ID1 = " << ID1<< " this_fr1 = " << this_fr1 << " pt = " << leps[0]->Pt() << " eta = " << leps[0]->fEta() << endl;
      cout << "IsMuon2 = " << IsMuon2 << " fr_key = " << fr_key2 << " ID2 = " << ID2<< " this_fr2 = " << this_fr2 << " pt = " << leps[1]->Pt() << " eta = " << leps[1]->fEta() << endl;
      exit(EXIT_FAILURE);
    }

    if(run_Debug) cout << ID1 << " " << ID2 << "fr_key11 = " << fr_key1 <<  " w= " << this_weight << " 2L"<<endl;                                                    
    return this_weight;
  }


  else{


    double this_weight = -1.;
    vector<double> FRs;

    double this_fr = -999.;
    for(auto lep : leps){

      if( lep->PassLepID() ) continue;

      TString  muon_fr_key = _param.k.Muon_FR;
      if(_param.k.Muon_BB_FR!= "Default") {
        if(lep->IsBB()) muon_fr_key = _param.k.Muon_BB_FR;
        else  muon_fr_key = _param.k.Muon_EC_FR;
      }

      TString fr_key = (lep->LeptonFlavour() == Lepton::ELECTRON) ?  _param.k.Electron_FR : muon_fr_key;
      TString ID     = (lep->LeptonFlavour() == Lepton::ELECTRON) ?  _param.Electron_Tight_ID : _param.Muon_Tight_ID;
      bool IsMuon    = (lep->LeptonFlavour() != Lepton::ELECTRON);
      
      this_fr = fakeEst->GetFakeRate(IsMuon, ID,  fr_key, _param.FakeRateMethod, _param.FakeRateParam, lep->fEta(), lep->Pt(),lep->LeptonFakeTagger());
      this_weight *= -1.*this_fr/(1.-this_fr);
      FRs.push_back(this_fr);

    }

    if(FRs.size()==0){
      return 0.;
    }
    else{
      //cout << this_weight<< " ML"<<endl;                                                                                                                                                                                                                                                                                                                                                                                               

      return this_weight;
    }
  }

  return -999.;
}


double HNL_LeptonCore::GetIsoFromID(Lepton lep, TString id){

  double pt = lep.Pt();
  double eta = lep.Eta();

  if (lep.IsMuon()) {

    if (id == "Peking")        return 0.15;
    if (id == "TopHNT")        return 0.1;
    if (id == "TopHN")         return 0.1;
    if (id == "HNTight_17028") return 0.07;
    if (id == "HNTightV1")     return 0.07;
    if (id == "HNTightV2")     return 0.07;
    if (id == "POGTightPFIsoVeryVeryTight") return 0.05;
    if (id.Contains("TightPFIsoVeryVeryTight")) return 0.05;
    if (id.Contains("TightPFIsoVeryTight")) return 0.1;
    if (id.Contains("TightPFIsoTight")) return 0.15;
    if (id.Contains("TightWithTightIso")) return 0.15;
    if (id.Contains("TightStandardPFIsoTight")) return 0.15;
    if (id.Contains("PFIsoMedium")) return 0.2;
    if (id.Contains("PFIsoLoose")) return 0.25;
    if (id.Contains("PFIsoVeto")) return 0.4;
    if (id == "POGHighPtWithLooseTrkIso") return 0.1;
    if (id == "POGHighPtWithVLooseTrkIso") return 0.45;
    if (id == "POGHighPtTight") return 0.1;
    if (id == "POGHighPtMixTight") return 0.1;
    if (id.Contains("HNMVA_")) return 0.1;
  }
  else {

    if (id == "Peking_2016")      {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if (id == "Peking_2017" || id == "Peking_2018")      {
      if(fabs(eta) < 1.479) return 0.0571;
      else  return 0.05880;
    }

    if( id == "passHEEPID" )         return 0.6;
    if( id == "CutBasedLooseNoIso" ) return 0.6;

    if( id == "TopHNSST" ) return 0.1;
    if( id == "HNTight_17028") return 0.08;
    if( id.Contains("HNTightV")) {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id == "HN2016") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.06);
    }
    if( id == "HN2017") {
      if(fabs(eta) < 1.479) return 0.085;
      else  return (0.05);
    }
    if( id == "HN2018") {
      if(fabs(eta) < 1.479) return 0.095;
      else  return (0.07);
    }
    if( id == "HNRelaxedIP2016") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.05);
    }
    if( id == "HNRelaxedIP2017") {
      if(fabs(eta) < 1.479) return 0.1;
      else  return (0.05);
    }
    if( id == "HNRelaxedIP2018") {
      if(fabs(eta) < 1.479) return 0.095;
      else  return (0.07);
    }
    if( id == "passTightID_nocc") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id.Contains("passPOGTight")){
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));

    }
    if( id.Contains("passPOGMedium")){
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "passTightID") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }
    if( id.Contains("HNMediumV")) {
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "passMediumID") {
      if(fabs(eta) < 1.479) return (0.0478 + (0.506/pt));
      else  return (0.0658 + (0.963/pt));
    }
    if( id == "HN2016POG") {
      if(fabs(eta) < 1.479) return (0.0287 + (0.506/pt));
      else  return (0.0445 + (0.963/pt));
    }

    if( id == "Iso1") {
      if(fabs(eta) < 1.479) return  0.08;
      else  return 0.08;
    }
    if( id == "Iso2") {
      if(fabs(eta) < 1.479) return  0.09;
      else  return 0.08;
    }
    if( id == "Iso3") {
      if(fabs(eta) < 1.479) return  0.1;
      else  return 0.08;
    }
    if( id == "Iso4") {
      if(fabs(eta) < 1.479) return  0.12;
      else  return 0.08;
    }
    if( id == "Iso5") {
      if(fabs(eta) < 1.479) return  0.09;
      else  return 0.09;
    }
    if( id == "Iso6") {
      if(fabs(eta) < 1.479) return  0.1;
      else  return 0.1;
    }
    if( id == "Iso7") {
      if(fabs(eta) < 1.479) return  0.12;
      else  return 0.12;
    }

    if( id.Contains("HNTight_Opt")) return 0.08;

    if( id.Contains("HN2016MVA")) return 0.08;
    if( id.Contains("HN2016POG")) return 0.08;
    if( id == "passMVAID_noIso_WP90V16") return 0.05;
    if( id == "passMVAID_noIso_WP80") return 0.08;
    if( id == "passMVAID_noIso_WP90") return 0.08;
    if( id == "passMVAID_Iso_WP80") return 999.0;
    if( id == "passMVAID_Iso_WP90") return 999.0;

    if (id.Contains("HNMVA_")) return 0.1;


  }
  cout << "[AnalyzerCore::GetIsoFromID ] ID not found.." << id<< endl;
  exit(EXIT_FAILURE);

  return -999999999.;

}
