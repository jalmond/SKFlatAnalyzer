#include "HNL_LeptonCore.h"

// helper: return mass-specific bins if present and non-empty, else era default                                                              
const vector<double>& HNL_LeptonCore::choose_bins(
    const map<TString, vector<double>>& per_mass,
    TString mass)
{
    TString lookup_mass = mass;

    // ----------------------------
    // Mass remapping
    // ----------------------------
    int m = mass.Atoi();

    if (m == 1000 || m == 1100 || m == 1200 || m == 1300) {
        lookup_mass = "1000";
    }
    else if (m == 1500 || m == 1700) {
        lookup_mass = "1500";
    }
    else if (m == 2000 || m == 2500 || m == 3000) {
        lookup_mass = "2000";
    }

    auto in_list = [](int val, std::initializer_list<int> lst) {
      
      for (int x : lst) {
	
        if (val == x) return true;
	
      }
      
      return false;
      
    };
    
    if (HasFlag("SR1_Binned")) {
  
      if (in_list(m, {400, 450, 500, 600})) {
	lookup_mass = "MassBinned1";
      }
      else if (in_list(m, {700, 750, 800, 900})) {
	lookup_mass = "MassBinned2";
      }
      else {
	lookup_mass = "MassBinned3";
      }
    }
    
    // ----------------------------
    // Lookup
    // ----------------------------
    auto it = per_mass.find(lookup_mass);
    if (it != per_mass.end() && !it->second.empty())
        return it->second;

    // Fallback to 1000
    auto it_fallback = per_mass.find("1000");
    if (it_fallback != per_mass.end() && !it_fallback->second.empty())
        return it_fallback->second;

    cout << "choose_bins " << mass << " missing" << endl;
    exit(EXIT_FAILURE);
}


void HNL_LeptonCore::DefineLimitBins(){
  
  //// Define SR 1 binning to allow nevents >= 1 per bin per era

  vector<double> sr1bins_ll = {0.,  500, 750,  2000.};

  map<TString, vector<double>> sr1bins_mm_byMass;
  map<TString, vector<double>> sr1bins_ee_byMass;
  map<TString, vector<double>> sr1bins_em_byMass;

  vector<double> sr1bins_mm, sr1bins_ee, sr1bins_em, sr1bins;

  /////
  sr1bins = {0.0, 560.0, 665.0, 785.0, 965.0, 1375.0, 1830.0, 5000.0};
  sr1bins_mm = {0.0, 400.0, 570.0, 670.0, 770.0, 885.0, 1395.0, 5000.0};
  sr1bins_ee = {0.0, 390.0, 585.0, 780.0, 880.0, 1395.0, 1855.0, 5000.0};
  sr1bins_em = {0.0, 410.0, 670.0, 775.0, 965.0, 1160.0, 1865.0, 5000.0};

  // 400-600
  sr1bins_mm_byMass["MassRange1"] = {0.0, 415.0, 440.0, 490.0, 570.0, 680.0, 860.0, 5000.0};
  // 700-900
  sr1bins_mm_byMass["MassRange2"] = {0.0, 485.0, 570.0, 625.0, 770.0, 870.0, 1015.0, 5000.0};
  //1000+
  sr1bins_mm_byMass["MassRange3"] = {0.0, 420.0, 570.0, 755.0, 860.0, 965.0, 1185.0, 5000.0};

  sr1bins_ee_byMass["MassRange1"] = {0.0, 410.0, 450.0, 490.0, 560.0, 600.0, 655.0, 5000.0};
  sr1bins_ee_byMass["MassRange2"] = {0.0, 620.0, 675.0, 710.0, 780.0, 870.0, 985.0, 5000.0};
  sr1bins_ee_byMass["MassRange3"] = {0.0, 620.0, 770.0, 850.0, 925.0, 1095.0, 1425.0, 5000.0};

  sr1bins_em_byMass["MassRange1"] = {0.0, 445.0, 495.0, 565.0, 595.0, 640.0, 670.0, 5000.0};
  sr1bins_em_byMass["MassRange2"] = {0.0, 595.0, 655.0, 680.0, 760.0, 890.0, 990.0, 5000.0};
  sr1bins_em_byMass["MassRange3"] = {0.0, 670.0, 825.0, 915.0, 965.0, 1085.0, 1495.0, 5000.0};
  
  sr1bins_mm_byMass["400"] = {0.0, 365.0, 395.0, 450.0, 555.0, 720.0, 755.0, 5000.0};
  sr1bins_mm_byMass["450"] = {0.0, 375.0, 415.0, 440.0, 505.0, 555.0, 755.0, 5000.0};
  sr1bins_mm_byMass["500"] = {0.0, 420.0, 455.0, 485.0, 535.0, 720.0, 755.0, 5000.0};
  sr1bins_mm_byMass["600"] = {0.0, 520.0, 555.0, 580.0, 655.0, 720.0, 755.0, 5000.0};
  sr1bins_mm_byMass["700"] = {0.0, 555.0, 600.0, 650.0, 685.0, 795.0, 860.0, 5000.0};
  sr1bins_mm_byMass["800"] = {0.0, 520.0, 570.0, 665.0, 710.0, 760.0, 910.0, 5000.0};
  sr1bins_mm_byMass["900"] = {0.0, 520.0, 570.0, 685.0, 760.0, 860.0, 1015.0, 5000.0};
  sr1bins_mm_byMass["1000"] = {0.0, 555.0, 665.0, 755.0, 860.0, 965.0, 1100.0, 5000.0};
  sr1bins_mm_byMass["1500"] = {0.0, 520.0, 545.0, 595.0, 760.0, 970.0, 1185.0, 5000.0};
  sr1bins_mm_byMass["2000"] = {0.0, 510.0, 595.0, 740.0, 785.0, 965.0, 1185.0, 5000.0};
    
  sr1bins_ee_byMass["400"] = {0.0, 365.0, 390.0, 430.0, 465.0, 655.0, 740.0, 5000.0};
  sr1bins_ee_byMass["450"] = {0.0, 410.0, 450.0, 485.0, 520.0, 740.0, 905.0, 5000.0};
  sr1bins_ee_byMass["500"] = {0.0, 455.0, 480.0, 505.0, 540.0, 565.0, 905.0, 5000.0};
  sr1bins_ee_byMass["600"] = {0.0, 545.0, 575.0, 600.0, 655.0, 700.0, 905.0, 5000.0};
  sr1bins_ee_byMass["700"] = {0.0, 585.0, 630.0, 685.0, 755.0, 835.0, 905.0, 5000.0};
  sr1bins_ee_byMass["800"] = {0.0, 525.0, 630.0, 710.0, 750.0, 780.0, 870.0, 5000.0};
  sr1bins_ee_byMass["900"] = {0.0, 615.0, 740.0, 795.0, 850.0, 890.0, 980.0, 5000.0};
  sr1bins_ee_byMass["1000"] = {0.0, 610.0, 720.0, 770.0, 860.0, 925.0, 1070.0, 5000.0};
  sr1bins_ee_byMass["1500"] = {0.0, 505.0, 630.0, 780.0, 905.0, 1185.0, 1365.0, 5000.0};
  sr1bins_ee_byMass["2000"] = {0.0, 540.0, 665.0, 750.0, 905.0, 1210.0, 1430.0, 5000.0};

  
  sr1bins_em_byMass["400"] = {0.0, 365.0, 390.0, 440.0, 665.0, 865.0, 915.0, 5000.0};
  sr1bins_em_byMass["450"] = {0.0, 385.0, 430.0, 455.0, 485.0, 510.0, 665.0, 5000.0};
  sr1bins_em_byMass["500"] = {0.0, 445.0, 470.0, 495.0, 525.0, 565.0, 665.0, 5000.0};
  sr1bins_em_byMass["600"] = {0.0, 535.0, 560.0, 595.0, 640.0, 685.0, 915.0, 5000.0};
  sr1bins_em_byMass["700"] = {0.0, 535.0, 620.0, 665.0, 700.0, 770.0, 965.0, 5000.0};
  sr1bins_em_byMass["800"] = {0.0, 665.0, 735.0, 785.0, 865.0, 1050.0, 1075.0, 5000.0};
  sr1bins_em_byMass["900"] = {0.0, 535.0, 665.0, 735.0, 820.0, 875.0, 990.0, 5000.0};
  sr1bins_em_byMass["1000"] = {0.0, 670.0, 800.0, 870.0, 915.0, 965.0, 1095.0, 5000.0};
  sr1bins_em_byMass["1500"] = {0.0, 655.0, 735.0, 915.0, 1075.0, 1240.0, 1405.0, 5000.0};
  sr1bins_em_byMass["2000"] = {0.0, 535.0, 680.0, 810.0, 980.0, 1125.0, 1495.0, 5000.0};

  if(HasFlag("StatReqEra")){
    sr1bins_mm_byMass["400"] = {0.0, 310.0, 365.0, 395.0, 450.0, 555.0, 755.0, 5000.0};
    sr1bins_mm_byMass["450"] = {0.0, 375.0, 415.0, 440.0, 505.0, 550.0, 755.0, 5000.0};
    sr1bins_mm_byMass["500"] = {0.0, 415.0, 455.0, 485.0, 535.0, 615.0, 760.0, 5000.0};
    sr1bins_mm_byMass["600"] = {0.0, 395.0, 440.0, 480.0, 555.0, 580.0, 655.0, 5000.0};
    sr1bins_mm_byMass["700"] = {0.0, 455.0, 485.0, 570.0, 620.0, 665.0, 830.0, 5000.0};
    sr1bins_mm_byMass["800"] = {0.0, 510.0, 575.0, 615.0, 760.0, 910.0, 990.0, 5000.0};
    sr1bins_mm_byMass["900"] = {0.0, 510.0, 575.0, 615.0, 760.0, 860.0, 1015.0, 5000.0};
    sr1bins_mm_byMass["1000"] = {0.0, 555.0, 610.0, 755.0, 860.0, 965.0, 1135.0, 5000.0};
    sr1bins_mm_byMass["1500"] = {0.0, 510.0, 575.0, 600.0, 760.0, 970.0, 1185.0, 5000.0};
    sr1bins_mm_byMass["2000"] = {0.0, 510.0, 575.0, 600.0, 785.0, 965.0, 1185.0, 5000.0};
    
    sr1bins_ee_byMass["400"] = {0.0, 330.0, 390.0, 440.0, 655.0, 700.0, 740.0, 5000.0};
    sr1bins_ee_byMass["450"] = {0.0, 410.0, 450.0, 485.0, 520.0, 740.0, 905.0, 5000.0};
    sr1bins_ee_byMass["500"] = {0.0, 455.0, 490.0, 540.0, 565.0, 740.0, 905.0, 5000.0};
    sr1bins_ee_byMass["600"] = {0.0, 545.0, 575.0, 600.0, 655.0, 700.0, 905.0, 5000.0};
    sr1bins_ee_byMass["700"] = {0.0, 585.0, 630.0, 685.0, 765.0, 835.0, 905.0, 5000.0};
    sr1bins_ee_byMass["800"] = {0.0, 525.0, 630.0, 710.0, 750.0, 780.0, 870.0, 5000.0};
    sr1bins_ee_byMass["900"] = {0.0, 610.0, 720.0, 770.0, 810.0, 875.0, 990.0, 5000.0};
    sr1bins_ee_byMass["1000"] = {0.0, 610.0, 720.0, 770.0, 850.0, 925.0, 1070.0, 5000.0};
    sr1bins_ee_byMass["1500"] = {0.0, 505.0, 630.0, 780.0, 905.0, 1185.0, 1365.0, 5000.0};
    sr1bins_ee_byMass["2000"] = {0.0, 540.0, 665.0, 750.0, 905.0, 1205.0, 1430.0, 5000.0};
    
    
    sr1bins_em_byMass["400"] = {0.0, 355.0, 385.0, 440.0, 665.0, 845.0, 915.0, 5000.0};
    sr1bins_em_byMass["450"] = {0.0, 390.0, 430.0, 455.0, 485.0, 510.0, 665.0, 5000.0};
    sr1bins_em_byMass["500"] = {0.0, 445.0, 470.0, 495.0, 565.0, 665.0, 915.0, 5000.0};
    sr1bins_em_byMass["600"] = {0.0, 535.0, 570.0, 595.0, 640.0, 705.0, 915.0, 5000.0};
    sr1bins_em_byMass["700"] = {0.0, 535.0, 600.0, 655.0, 680.0, 770.0, 965.0, 5000.0};
    sr1bins_em_byMass["800"] = {0.0, 525.0, 580.0, 650.0, 715.0, 760.0, 890.0, 5000.0};
    sr1bins_em_byMass["900"] = {0.0, 505.0, 575.0, 665.0, 750.0, 875.0, 990.0, 5000.0};
    sr1bins_em_byMass["1000"] = {0.0, 575.0, 700.0, 825.0, 915.0, 965.0, 1095.0, 5000.0};
    sr1bins_em_byMass["1500"] = {0.0, 540.0, 660.0, 735.0, 915.0, 1070.0, 1405.0, 5000.0};
    sr1bins_em_byMass["2000"] = {0.0, 535.0, 670.0, 735.0, 935.0, 1080.0, 1495.0, 5000.0};
    
  }
  
  
  vector<TString> masses_to_prepare = {"400", "450","500", "600", "700", "800","900","1000","1500","2000"};
    
  // --------------------------------------------
  // SR1 Global Mass (per flavour, same for all masses)
  // --------------------------------------------
  if(HasFlag("SR1_GlobalMass")) {
    for(const auto& mass : masses_to_prepare) {
      sr1bins_mm_byMass[mass] = sr1bins_mm;
      sr1bins_ee_byMass[mass] = sr1bins_ee;
      sr1bins_em_byMass[mass] = sr1bins_em;
    }
    
  }
  
  // --------------------------------------------  
  // SR1 Fully Global (same for everything)
  // --------------------------------------------
  if(HasFlag("SR1_Global")) {
    
    for(const auto& mass : masses_to_prepare) {
      
      sr1bins_mm_byMass[mass] = sr1bins;
      sr1bins_ee_byMass[mass] = sr1bins;
      sr1bins_em_byMass[mass] = sr1bins;
      
    }
  }
  vector<TString> MuMu_SR1_BinTags,  EE_SR1_BinTags ,EMu_SR1_BinTags, CR1_BinTags;
  for(unsigned int i_d =1 ; i_d <  sr1bins_mm.size(); i_d++)  MuMu_SR1_BinTags.push_back("SR1_MNbin"+to_string(i_d));
  for(unsigned int i_d =1 ; i_d <  sr1bins_ee.size(); i_d++)  EE_SR1_BinTags.push_back("SR1_MNbin"+to_string(i_d));
  for(unsigned int i_d =1 ; i_d <  sr1bins_em.size(); i_d++)  EMu_SR1_BinTags.push_back("SR1_MNbin"+to_string(i_d));
  
  
  //// EE/EMu have most bins
  
  map_bins_labels ["SR1_MuMu"]= MuMu_SR1_BinTags;
  map_bins_labels ["SR1_EE"]  = EE_SR1_BinTags;
  map_bins_labels ["SR1_EMu"] = EMu_SR1_BinTags;


  for (const auto& m : masses_to_prepare) {
    const TString& mass_str = m;
    
    // labels: use the correct tag vectors per channel
    map_bins_labels["SR1_MuMu_" + mass_str] = MuMu_SR1_BinTags;
    map_bins_labels["SR1_EE_"   + mass_str] = EE_SR1_BinTags;
    map_bins_labels["SR1_EMu_"  + mass_str] = EMu_SR1_BinTags;
    
    // boundaries: choose mass-dependent bins if available, else era defaults
    map_bins_boundaries["SR1_MuMu_" + mass_str] = choose_bins(sr1bins_mm_byMass, mass_str);
    map_bins_boundaries["SR1_EE_"   + mass_str] = choose_bins(sr1bins_ee_byMass, mass_str);
    map_bins_boundaries["SR1_EMu_"  + mass_str] = choose_bins(sr1bins_em_byMass, mass_str);


  }
  
  map_bins_boundaries  ["SR1_MuMu"] = sr1bins_mm;
  map_bins_boundaries  ["SR1_EE"]   = sr1bins_ee;
  map_bins_boundaries  ["SR1_EMu"]  = sr1bins_em;

  
  //// CR
  map_bins_labels ["CR1"]       = CR1_BinTags;


  //// Define SR 2 binning  

  map_bins_labels ["SR2"]       = {"SR2_HTLT_Bin1",       "SR2_HTLT_Bin2",       "SR2_HTLT_Bin3",     "SR2_HTLT_Bin4","SR2_HTLT_Bin5","SR2_HTLT_Bin6","SR2_HTLT_Bin7","SR2_HTLT_Bin8"};
  map_bins_labels ["CR2"] = {"CR2_HTLT_Bin1", "CR2_HTLT_Bin2", "CR2_HTLT_Bin3"};

  //// Define SR 3 binning                                                                                                                              

  vector<TString> minBinsSR3={"SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8", "SR3_bin9","SR3_bin10","SR3_bin11","SR3_bin12","SR3_bin13","SR3_bin14","SR3_bin15","SR3_bin16","SR3_bin17","SR3_bin18","SR3_bin19","SR3_bin20"};

  std::vector<TString> MuMu_SR3 = minBinsSR3;
  std::vector<TString> EE_SR3   = minBinsSR3;
  std::vector<TString> EMu_SR3  = minBinsSR3;
  
  map_bins_labels ["SR3_MuMu"] = MuMu_SR3;
  map_bins_labels ["SR3_EE"]   = EE_SR3;
  map_bins_labels ["SR3_EMu"]  = EMu_SR3;

  std::vector<TString> CR3 = {"CR3_bin1","CR3_bin2","CR3_bin3","CR3_bin4"};
  map_bins_labels ["CR3"]    = CR3;
   

		 
  return ;
  
}

vector<double> HNL_LeptonCore::GetLimitBinBoundary(const TString& SR, TString mass_ref, const TString& channel){
  
  
  TString key = (channel == "") ? SR : SR+"_"+channel+"_"+mass_ref;
  
  map<TString,vector<double> >::iterator mit = map_bins_boundaries.find(key);
  if(mit == map_bins_boundaries.end()) {
    cout << "GetLimitBinBoundary " << key << endl;
    exit(EXIT_FAILURE);
  } 
  vector<double> vcuts = mit->second;

  return vcuts;
}

vector<TString> HNL_LeptonCore::GetLimitLabels(const TString& SR, const TString& channel){
  
  TString key = (channel == "") ? SR : SR+"_"+channel;

  map<TString,vector<TString> > ::iterator mit = map_bins_labels.find(key);
  if(mit == map_bins_labels.end()) {
    cout << "GetLimitLabels  " << SR+"_"+channel << endl;
    
    exit(EXIT_FAILURE);
  }

  vector<TString> SRlabels = mit->second;

  return SRlabels;
}


TString HNL_LeptonCore::GetSR3StringBin(const TString& RegionTag, const TString& channel, bool LowJet, double met2_st, double LT, double ll_dphi){
  
  
  /// Detailed binning for High Mass SR3
  
  if (RegionTag.Contains("CR3")) {
    if (LT < 150) return RegionTag + "_bin1";
    if (LT < 200) return RegionTag + "_bin2";
    if (LT < 300) return RegionTag + "_bin3";
    return RegionTag + "_bin4";
  }
  
  double met2_st_boundary = 2.;
  
  TString binPrefix = RegionTag + "_bin";
  double binLimit1 = 0.0, binLimit2 = 0.0, binLimit3 = 0.0, binLimit4 = 0.0;
  
  if (channel == "MuMu") {
    if (LowJet) {
      // LowJet Bins
      if (met2_st < met2_st_boundary) {
	binLimit1 = 220; binLimit2 = 290; binLimit3 = 360; binLimit4 = 480;
      }
      else {
	binLimit1 = 260; binLimit2 = 360; binLimit3 = 460; binLimit4 = 650;
	
      }
    }
    else {
      // HighJet Bins
      if (met2_st < met2_st_boundary) {
	binLimit1 = 200; binLimit2 = 260; binLimit3 = 320; binLimit4 = 420;
	if(HasFlag("StatReqEra")){
	  binLimit1 = 200; binLimit2 = 260; binLimit3 = 320; binLimit4 = 380;
	}
      }
      else {
	binLimit1 = 230; binLimit2 = 290; binLimit3 = 340; binLimit4 = 480;
	
      }
    }
  }
  
  if (channel == "EE") {
    if (LowJet) {
      // LowJet Bins
      if (met2_st < met2_st_boundary) {
	binLimit1 = 340; binLimit2 = 600; binLimit3 = 800; binLimit4 = 1100;
	
      }
      else {
	binLimit1 = 270; binLimit2 = 420; binLimit3 = 600; binLimit4 = 800;
	
      }
    }
    else {
      // HighJet Bins
      if (met2_st < met2_st_boundary) {
	binLimit1 = 270; binLimit2 = 360; binLimit3 = 650; binLimit4 = 1000;
      }
      else {
	binLimit1 = 280; binLimit2 = 420; binLimit3 = 550; binLimit4 = 850;
	
      }
    }
  }
  
  if (channel == "EMu") {
    
    if (LowJet) {
      // LowJet Bins
      if (met2_st < met2_st_boundary) {
	binLimit1 = 300; binLimit2 = 460; binLimit3 = 550;  binLimit4 = 800;
      } else {
	binLimit1 = 300;  binLimit2 = 420; binLimit3 = 600;  binLimit4 = 850;
      }
    } else {
      // HighJet Bins
      if (met2_st < met2_st_boundary) {
	binLimit1 = 260; binLimit2 = 360; binLimit3 = 460; binLimit4 = 550;
      } else {
	binLimit1 = 250; binLimit2 = 360;   binLimit3 = 460;  binLimit4 = 750;
      }
    }
  }
    

  // Determine the correct bin based on the LT value
  if (LowJet) {
    if (met2_st < met2_st_boundary) {
      if (LT <= binLimit1) return binPrefix + "1";
      if (LT <= binLimit2) return binPrefix + "2";
      if (LT <= binLimit3) return binPrefix + "3";
      if (LT <= binLimit4) return binPrefix + "4";
        return binPrefix + "5";
    } else {
      if (LT <= binLimit1) return binPrefix + "6";
      if (LT <= binLimit2) return binPrefix + "7";
      if (LT <= binLimit3) return binPrefix + "8";
      if (LT <= binLimit4) return binPrefix + "9";
      return binPrefix + "10";
    }
  } 
  else {
    if (met2_st < met2_st_boundary) {
      if (LT < binLimit1) return binPrefix + "11";
      if (LT < binLimit2) return binPrefix + "12";
      if (LT < binLimit3) return binPrefix + "13";
      if (LT < binLimit4) return binPrefix + "14";
      return binPrefix + "15";
    } else {
      if (LT < binLimit1) return binPrefix + "16";
      if (LT < binLimit2) return binPrefix + "17";
      if (LT < binLimit3) return binPrefix + "18";
      if (LT < binLimit3) return binPrefix + "19";
      return binPrefix + "20";
    }
  }
  
  return "";
  
}


vector<TString> HNL_LeptonCore::GetBDTLimitLabels(const TString& key){

  map<TString,vector<TString> > ::iterator mit = map_BDT_bins_labels.find(key);
  if(mit == map_BDT_bins_labels.end()) {
    cout << "GetBDTLimitLabels " << key << endl;
    for(auto i : map_BDT_bins_labels) cout << i.first << endl;
    exit(EXIT_FAILURE);
  }
  vector<TString> SRlabels = mit->second;

  return SRlabels;
}


double HNL_LeptonCore::GetLimitBin(const TString& region, const std::vector<Lepton*>& leps, const std::vector<Jet>& AK4Jets,
				   const std::vector<FatJet>& AK8_JetColl, const Event& ev, double& nbins_reg){

  double Binvalue=0;
  
  if(region=="CR_SR3_WZ") {
    double LT = leps[0]->Pt() + leps[1]->Pt();
    if(LT < 100) Binvalue=0.5;
    else     if(LT < 200) Binvalue=1.5;
    else if(LT < 300) Binvalue=2.5;
    else if(LT < 400) Binvalue=3.5;
    else  Binvalue=4.5;
    nbins_reg=5;
    return Binvalue;

  }
  if(region=="CR_SR1_Inv"){

    Particle l1J =  *leps[0] +  AK8_JetColl[0];
    if(l1J.M() < 300) Binvalue=0.5;
    else if(l1J.M() < 500)  Binvalue=1.5;
    else if(l1J.M() < 750)  Binvalue= 2.5;
    else if(l1J.M() < 1000) Binvalue= 3.5;
    else Binvalue= 4.5;
    //// 3 bins                                                                                                                                                  
    nbins_reg=5;
    return Binvalue;
  }
 
  if(region=="CR_SR2_Inv"){
    double SR2BinValue = leps[0]->HTOverPt();
    if(SR2BinValue < 2) Binvalue = 0.5;
    else   if(SR2BinValue < 5) Binvalue = 1.5;
    else   Binvalue = 2.5;

    nbins_reg=3;
    return Binvalue;
    
  }


  if(region == ("CR_SR1_WZ")){
    
    Particle l1J =  *leps[0] +  AK8_JetColl[0];
    if(l1J.M() < 750) Binvalue= 0.5;
    else Binvalue= 1.5;
    //// 3 bins                                                                                                                                                             
    nbins_reg=2;
    return Binvalue;
  }
  


  if(region.Contains("CR_SR1")){

    Particle l1J =  *leps[0] +  AK8_JetColl[0];
    if(l1J.M() < 200) Binvalue=0.5;
    else if(l1J.M() < 750) Binvalue= 1.5;
    else Binvalue= 2.5;    
    //// 3 bins
    nbins_reg=3;
    return Binvalue; 
  }

  if(region == "CR_SR2_WZ"){
    double SR2BinValue = leps[0]->HTOverPt();
    if(SR2BinValue < 2) Binvalue = 0.5;
    else   if(SR2BinValue < 4) Binvalue = 1.5;
    else   Binvalue = 2.5;

    nbins_reg=3;
    return Binvalue;

  }

  if(region =="CR_SR2_WZB") {
    double SR2BinValue = leps[0]->HTOverPt();
    if(SR2BinValue < 3) Binvalue = 0.5;
    else   Binvalue = 1.5;

    nbins_reg=2;
    return Binvalue;

  }
  if(region.Contains("CR_SR2")){

    double SR2BinValue = leps[0]->HTOverPt();
    if(SR2BinValue < 2) Binvalue = 0.5;
    else   if(SR2BinValue < 5) Binvalue = 1.5;
    else   Binvalue = 2.5;

    nbins_reg=3;
    return Binvalue;

  }
  
  cout << "No Limit Region set " << region << endl;
  exit(EXIT_FAILURE);

}

