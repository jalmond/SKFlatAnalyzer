#include "HNL_LeptonCore.h"
#include <set>

// helper: return mass-specific bins if present and non-empty, else era default                                                              
const vector<double>& HNL_LeptonCore::choose_bins(
    const map<TString, vector<double>>& per_mass,
    TString mass)
{
    TString lookup_mass = mass;

    
    // ----------------------------
    // Lookup
    // ----------------------------
    auto it = per_mass.find(lookup_mass);
    if (it != per_mass.end() && !it->second.empty())
        return it->second;

    cout << "choose_bins " << mass << " missing" << endl;
    exit(EXIT_FAILURE);
}


void HNL_LeptonCore::DefineLimitBins(){
  
  //// Define SR 1 binning to allow nevents >= 1 per bin per era

  vector<double> sr1bins_ll = {0.,  500, 750,  2000.};

  map<TString, vector<double>> sr1bins_mm_byMass;
  map<TString, vector<double>> sr1bins_ee_byMass;
  map<TString, vector<double>> sr1bins_em_byMass;

  vector<double> sr1bins_dummy;
  vector<double> sr1bins_mm, sr1bins_ee, sr1bins_em, sr1bins;

  ///// Global bins NOT OPTIMIAL
  sr1bins = {0.0, 560.0, 665.0, 785.0, 965.0, 1375.0, 1830.0, 5000.0};
  sr1bins_mm = {0.0, 400.0, 570.0, 670.0, 770.0, 885.0, 1395.0, 5000.0};
  sr1bins_ee = {0.0, 390.0, 585.0, 780.0, 880.0, 1395.0, 1855.0, 5000.0};
  sr1bins_em = {0.0, 410.0, 670.0, 775.0, 965.0, 1160.0, 1865.0, 5000.0};
  
  // 400-600 : 700-900 : 1000+ 7 bins
  if(HasFlag("SR1_MassGroups")){
    sr1bins_mm_byMass["MassRange1"] = {0.0, 365.0, 395.0, 440.0, 485.0, 580.0, 655.0, 5000.0};
    sr1bins_mm_byMass["MassRange2"] = {0.0, 450.0, 570.0, 625.0, 770.0, 870.0, 1015.0, 5000.0};
    sr1bins_mm_byMass["MassRange3"] = {0.0, 420.0, 570.0, 755.0, 860.0, 965.0, 1185.0, 5000.0};
    
    sr1bins_ee_byMass["MassRange1"] = {0.0, 390.0, 440.0, 490.0, 560.0, 600.0, 655.0, 5000.0};
    sr1bins_ee_byMass["MassRange2"] = {0.0, 630.0, 685.0, 755.0, 785.0, 870.0, 985.0, 5000.0};
    sr1bins_ee_byMass["MassRange3"] = {0.0, 750.0, 850.0, 925.0, 1075.0, 1185.0, 1420.0, 5000.0};
    
    sr1bins_em_byMass["MassRange1"] = {0.0, 385.0, 445.0, 490.0, 565.0, 595.0, 640.0, 5000.0};
    sr1bins_em_byMass["MassRange2"] = {0.0, 595.0, 655.0, 680.0, 760.0, 890.0, 990.0, 5000.0};
    sr1bins_em_byMass["MassRange3"] = {0.0, 665.0, 810.0, 915.0, 965.0, 1080.0, 1450.0, 5000.0};
    sr1bins_dummy = {0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0};
  }
  sr1bins_dummy = {0.0,  0.0, 0.0, 0.0, 0.0, 0.0};
  /// Per mass scan 5 bins
  sr1bins_mm_byMass["400"] = {0.0, 365.0, 395.0, 450.0, 555.0, 5000.0};
  sr1bins_mm_byMass["450"] = {0.0, 415.0, 440.0, 495.0, 755.0, 5000.0};
  sr1bins_mm_byMass["500"] = {0.0, 440.0, 485.0, 535.0, 860.0, 5000.0};
  sr1bins_mm_byMass["600"] = {0.0, 480.0, 555.0, 580.0, 655.0, 5000.0};
  sr1bins_mm_byMass["700"] = {0.0, 555.0, 620.0, 665.0, 830.0, 5000.0};
  sr1bins_mm_byMass["800"] = {0.0, 555.0, 615.0, 760.0, 910.0, 5000.0};
  sr1bins_mm_byMass["900"] = {0.0, 610.0, 760.0, 860.0, 1015.0, 5000.0};
  sr1bins_mm_byMass["1000"] = {0.0, 755.0, 860.0, 965.0, 1135.0, 5000.0};
  sr1bins_mm_byMass["1100"] = {0.0, 555.0, 755.0, 885.0, 1000.0, 5000.0};
  sr1bins_mm_byMass["1200"] = {0.0, 650.0, 850.0, 960.0, 1115.0, 5000.0};
  sr1bins_mm_byMass["1300"] = {0.0, 595.0, 850.0, 1000.0, 1185.0, 5000.0};
  sr1bins_mm_byMass["1500"] = {0.0, 590.0, 760.0, 970.0, 1185.0, 5000.0};
  sr1bins_mm_byMass["1700"] = {0.0, 555.0, 760.0, 965.0, 1185.0, 5000.0};
  sr1bins_mm_byMass["2000"] = {0.0, 595.0, 785.0, 965.0, 1185.0, 5000.0};
  sr1bins_mm_byMass["2500"] = {0.0, 595.0, 785.0, 965.0, 1185.0, 5000.0};
  sr1bins_mm_byMass["3000"] = {0.0, 615.0, 860.0, 985.0, 1185.0, 5000.0};
      
  sr1bins_ee_byMass["400"] = {0.0, 330.0, 390.0, 440.0, 740.0, 5000.0};
  sr1bins_ee_byMass["450"] = {0.0, 410.0, 450.0, 495.0, 905.0, 5000.0};
  sr1bins_ee_byMass["500"] = {0.0, 455.0, 490.0, 560.0, 905.0, 5000.0};
  sr1bins_ee_byMass["600"] = {0.0, 570.0, 600.0, 655.0, 905.0, 5000.0};
  sr1bins_ee_byMass["700"] = {0.0, 630.0, 685.0, 765.0, 905.0, 5000.0};
  sr1bins_ee_byMass["800"] = {0.0, 670.0, 735.0, 780.0, 870.0, 5000.0};
  sr1bins_ee_byMass["900"] = {0.0, 740.0, 810.0, 875.0, 990.0, 5000.0};
  sr1bins_ee_byMass["1000"] = {0.0, 750.0, 850.0, 925.0, 1070.0, 5000.0};
  sr1bins_ee_byMass["1100"] = {0.0, 770.0, 905.0, 1060.0, 1230.0, 5000.0};
  sr1bins_ee_byMass["1200"] = {0.0, 905.0, 1075.0, 1150.0, 1290.0, 5000.0};
  sr1bins_ee_byMass["1300"] = {0.0, 905.0, 1080.0, 1200.0, 1420.0, 5000.0};
  sr1bins_ee_byMass["1500"] = {0.0, 750.0, 905.0, 1185.0, 1365.0, 5000.0};
  sr1bins_ee_byMass["1700"] = {0.0, 750.0, 905.0, 1185.0, 1420.0, 5000.0};
  sr1bins_ee_byMass["2000"] = {0.0, 750.0, 905.0, 1185.0, 1420.0, 5000.0};
  sr1bins_ee_byMass["2500"] = {0.0, 750.0, 905.0, 1185.0, 1420.0, 5000.0};
  sr1bins_ee_byMass["3000"] = {0.0, 670.0, 865.0, 1190.0, 1420.0, 5000.0};

  sr1bins_em_byMass["400"] = {0.0, 355.0, 385.0, 440.0, 665.0, 5000.0};
  sr1bins_em_byMass["450"] = {0.0, 410.0, 445.0, 490.0, 665.0, 5000.0};
  sr1bins_em_byMass["500"] = {0.0, 460.0, 495.0, 565.0, 665.0, 5000.0};
  sr1bins_em_byMass["600"] = {0.0, 540.0, 595.0, 640.0, 680.0, 5000.0};
  sr1bins_em_byMass["700"] = {0.0, 595.0, 655.0, 680.0, 770.0, 5000.0};
  sr1bins_em_byMass["800"] = {0.0, 650.0, 715.0, 760.0, 890.0, 5000.0};
  sr1bins_em_byMass["900"] = {0.0, 665.0, 740.0, 870.0, 990.0, 5000.0};
  sr1bins_em_byMass["1000"] = {0.0, 810.0, 915.0, 965.0, 1095.0, 5000.0};
  sr1bins_em_byMass["1100"] = {0.0, 805.0, 960.0, 1035.0, 1250.0, 5000.0};
  sr1bins_em_byMass["1200"] = {0.0, 760.0, 965.0, 1075.0, 1425.0, 5000.0};
  sr1bins_em_byMass["1300"] = {0.0, 735.0, 905.0, 1035.0, 1250.0, 5000.0};
  sr1bins_em_byMass["1500"] = {0.0, 735.0, 915.0, 1075.0, 1405.0, 5000.0};
  sr1bins_em_byMass["1700"] = {0.0, 715.0, 965.0, 1095.0, 1500.0, 5000.0};
  sr1bins_em_byMass["2000"] = {0.0, 700.0, 935.0, 1080.0, 1500.0, 5000.0};
  sr1bins_em_byMass["2500"] = {0.0, 700.0, 935.0, 1080.0, 1500.0, 5000.0};
  sr1bins_em_byMass["3000"] = {0.0, 665.0, 850.0, 1075.0, 1500.0, 5000.0};
  
  vector<TString> masses_to_prepare = {"400", "450","500", "600", "700", "800","900","1000","1100","1200","1300","1500","1700","2000","2500","3000"};
  if(HasFlag("SR1_MassGroups"))  masses_to_prepare = {"MassRange1","MassRange2","MassRange3"};
	
  // --------------------------------------------
  // SR1 Global Mass (per flavour, same for all masses)
  // --------------------------------------------
  if(HasFlag("SR1_GlobalMass")) {
    for(const auto& mass : masses_to_prepare) {
      sr1bins_mm_byMass[mass] = sr1bins_mm;
      sr1bins_ee_byMass[mass] = sr1bins_ee;
      sr1bins_em_byMass[mass] = sr1bins_em;
    }
    sr1bins_dummy = {0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0};
    
  }
  
  // --------------------------------------------  
  // SR1 Fully Global (same for everything)
  // --------------------------------------------
  if(HasFlag("SR1_Global")) {
    sr1bins_dummy = {0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, 0.0, 0.0};
	
    for(const auto& mass : masses_to_prepare) {
      
      sr1bins_mm_byMass[mass] = sr1bins;
      sr1bins_ee_byMass[mass] = sr1bins;
      sr1bins_em_byMass[mass] = sr1bins;
      
    }
  }
  vector<TString> MuMu_SR1_BinTags,  EE_SR1_BinTags ,EMu_SR1_BinTags, CR1_BinTags;
  for(unsigned int i_d =1 ; i_d <  sr1bins_dummy.size(); i_d++) {
    MuMu_SR1_BinTags.push_back("SR1_MNbin"+to_string(i_d));
    EE_SR1_BinTags.push_back("SR1_MNbin"+to_string(i_d));
    EMu_SR1_BinTags.push_back("SR1_MNbin"+to_string(i_d));
  }
  
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

  vector<TString> minBinsSR3={"SR3_bin1","SR3_bin2","SR3_bin3","SR3_bin4","SR3_bin5","SR3_bin6","SR3_bin7","SR3_bin8", "SR3_bin9","SR3_bin10","SR3_bin11","SR3_bin12","SR3_bin13","SR3_bin14","SR3_bin15","SR3_bin16","SR3_bin17","SR3_bin18","SR3_bin19","SR3_bin20","SR3_bin21","SR3_bin22","SR3_bin23","SR3_bin24"};

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

  if(HasFlag("SR1_MassGroups")){
    
    std::set<TString> valid_masses_range1 = {"400","450","500","600"};
    std::set<TString> valid_masses_range2 = {"700","800","900","750"};
    
    if (valid_masses_range1.count(mass_ref)) {
      key = SR + "_" + channel + "_MassRange1";
    }
    
    else if (valid_masses_range2.count(mass_ref)) {
      key = SR + "_" + channel + "_MassRange2";
    }
    
    else {
      key = SR + "_" + channel + "_MassRange3";
    }
  }
  
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


TString HNL_LeptonCore::GetSR3StringBin(const TString& RegionTag, const TString& channel, bool LowJet, double met2_st, double LT, double ll_dphi, double ht_pt){
  
  
  /// Detailed binning for High Mass SR3
  
  if (RegionTag.Contains("CR3")) {
    if (LT < 150) return RegionTag + "_bin1";
    if (LT < 200) return RegionTag + "_bin2";
    if (LT < 300) return RegionTag + "_bin3";
    return RegionTag + "_bin4";
  }
  
  double met2_st_boundary = 2.;
  double ht_pt_boundary = (LowJet) ? 1. : 1.5;
  
  bool region1 = (met2_st <= met2_st_boundary) && (ht_pt <= ht_pt_boundary);
  bool region2 = (met2_st > met2_st_boundary) && (ht_pt <= ht_pt_boundary);
  
  TString binPrefix = RegionTag + "_bin";
  double binLimit1 = 0.0, binLimit2 = 0.0, binLimit3 = 0.0;

  struct Binning {
    
    int b1, b2, b3;
    
  };
  
  // key = "channel_region"

  
  std::map<TString, Binning> binMap = {
    
    // =========================
    
    // MuMu
    
    // =========================
    
    {"MuMu_LowJet_bin1", {290, 360, 480}},
    
    {"MuMu_LowJet_bin2", {270, 360, 650}},
    
    {"MuMu_LowJet_bin3", {170, 230, 280}},
    
    {"MuMu_HighJet_bin1", {240, 320, 380}},
    
    {"MuMu_HighJet_bin2", {250, 360, 480}},
    
    {"MuMu_HighJet_bin3", {190, 230, 300}},
    
    // =========================
    
    // EE
    
    // =========================
    
    {"EE_LowJet_bin1", {400, 750, 1100}},
    
    {"EE_LowJet_bin2", {340, 550, 800}},
    
    {"EE_LowJet_bin3", {200, 250, 300}},
    
    {"EE_HighJet_bin1", {360, 650, 1000}},
    
    {"EE_HighJet_bin2", {300, 440, 800}},
    
    {"EE_HighJet_bin3", {180, 250, 320}},
    
    // =========================
    
    // EMu
    
    // =========================
    
    {"EMu_LowJet_bin1", {380, 550, 800}},
    
    {"EMu_LowJet_bin2", {320, 550, 850}},
    
    {"EMu_LowJet_bin3", {190, 240, 300}},
    
    {"EMu_HighJet_bin1", {300, 460, 550}},
    
    {"EMu_HighJet_bin2", {320, 460, 750}},
    
    {"EMu_HighJet_bin3", {180, 250, 320}}
    
  };
  TString jetStr = LowJet ? "LowJet" : "HighJet";
  
  TString regionStr;
  
  if (region1) regionStr = "bin1";
  
  else if (region2) regionStr = "bin2";
  
  else regionStr = "bin3";
  
  TString key = channel + "_" + jetStr + "_" + regionStr;
  
  auto it = binMap.find(key);
  
  if (it == binMap.end()) {
    
    std::cerr << "ERROR: binning not found for key = " << key << std::endl;
    
    exit(1);
    
  }
  
  binLimit1 = it->second.b1;
  
  binLimit2 = it->second.b2;
  
  binLimit3 = it->second.b3;
  return getBinName(LT,binLimit1,binLimit2,binLimit3, LowJet,region1,region2,binPrefix);
    
}



// Helper: determine bin index (0..3) from LT and edges

int HNL_LeptonCore::getLTBinIndex(float LT, int b1, int b2, int b3) {
  
  if (LT <= b1) return 0;
  
  if (LT <= b2) return 1;
  
  if (LT <= b3) return 2;
  
  return 3;
  
}

// Main function

TString HNL_LeptonCore::getBinName(float LT,
		       		       int binLimit1,
				       int binLimit2,
				       int binLimit3,
				       bool LowJet,
				       bool region1,
				       bool region2,
				       const TString& binPrefix) {
    
  // Determine LT bin (0..3)
  
  int ltBin = getLTBinIndex(LT, binLimit1, binLimit2, binLimit3);
  
  // Determine region index (0..5)

  int regionIndex;
  
  if (LowJet) {
    
    if (region1) regionIndex = 0;

    else if (region2) regionIndex = 1;

    else regionIndex = 2;

  } else {

    if (region1) regionIndex = 3;

    else if (region2) regionIndex = 4;

    else regionIndex = 5;

  }

  // Each region has 4 bins

  int globalBin = regionIndex * 4 + ltBin + 1;

  return binPrefix + TString(std::to_string(globalBin));

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

