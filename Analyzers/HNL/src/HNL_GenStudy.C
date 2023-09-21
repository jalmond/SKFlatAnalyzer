#include "HNL_GenStudy.h"

void HNL_GenStudy::executeEvent(){

  double weight = MCweight(true, false); //use sign only, not xsec
  int nmu(0), nel(0);
  for(auto gen:All_Gens){
    if(gen.fromHardProcessFinalState()){
      if(abs(gen.PID())==11) nel++;
      else if(abs(gen.PID())==13) nmu++;
    }
  }

  // sanity check
  int nlep = nel+nmu;
  if(nlep!=2){
    cout << "!!ERROR!! signal doesn't have 2 leptons; rather : " << nlep << endl;
    exit(EXIT_FAILURE);
  }

  vector<TString> DYlabels, VBFlabels, SSWWlabels;
  DYlabels={
    "M85",
    "M90",
    "M95",
    "M100",
    "M125",
    "M150",
    "M200",
    "M250",
    "M300",
    "M400",
    "M500",
    "M600",
    "M700",
    "M800",
    "M900",
    "M1000",
    "M1100",
    "M1200",
    "M1300",
    "M1500",
    "M1700",
    "M2000",
    "M2500",
    "M3000",
  };
  VBFlabels={
    "M500",
    "M600",
    "M700",
    "M800",
    "M900",
    "M1000",
    "M1100",
    "M1200",
    "M1300",
    "M1500",
    "M1700",
    "M2000",
    "M2500",
    "M3000",
  };
  SSWWlabels={
    "M500",
    "M600",
    "M700",
    "M750",
    "M800",
    "M900",
    "M1000",
    "M1100",
    "M1200",
    "M1250",
    "M1300",
    "M1500",
    "M1700",
    "M1750",
    "M2000",
    "M2500",
    "M3000",
    "M5000",
    "M7500",
    "M10000",
    "M15000",
    "M20000",
  };

  bool ee=false, mm=false, em=false;
  if(nel==2&&nmu==0) ee=true;
  else if(nel==0&&nmu==2) mm=true;
  else if(nel==1&&nmu==1) em=true;
  else{
    cout << "!!ERROR!! signal isn't ee, mm, nor em" << endl;
    exit(EXIT_FAILURE);
  }

  TObjArray *tokens = MCSample.Tokenize("_"); // split TString
  TString this_mass = ((TObjString*)tokens->At(2))->GetString();
  if(MCSample.Contains("DY")){
    FillHist("DY_tot",DYlabels,this_mass,weight);
    if(ee) FillHist("DY_ee",DYlabels,this_mass,weight);
    if(mm) FillHist("DY_mm",DYlabels,this_mass,weight);
    if(em) FillHist("DY_em",DYlabels,this_mass,weight);
    if(_jentry%2==0){
      FillHist("DY_half_tot",DYlabels,this_mass,weight);
      if(ee) FillHist("DY_half_ee",DYlabels,this_mass,weight);
      if(mm) FillHist("DY_half_mm",DYlabels,this_mass,weight);
      if(em) FillHist("DY_half_em",DYlabels,this_mass,weight);
    }
  }
  if(MCSample.Contains("VBF")){
    FillHist("VBF_tot",VBFlabels,this_mass,weight);
    if(ee) FillHist("VBF_ee",VBFlabels,this_mass,weight);
    if(mm) FillHist("VBF_mm",VBFlabels,this_mass,weight);
    if(em) FillHist("VBF_em",VBFlabels,this_mass,weight);
  }
  if(MCSample.Contains("SSWW")&&MCSample.Contains("SF")){ // no need to do this with DF
    FillHist("SSWW_tot",SSWWlabels,this_mass,weight);
    if(ee) FillHist("SSWW_ee",SSWWlabels,this_mass,weight);
    if(mm) FillHist("SSWW_mm",SSWWlabels,this_mass,weight);
    if(em) FillHist("SSWW_em",SSWWlabels,this_mass,weight);
  }

}

HNL_GenStudy::HNL_GenStudy(){
  
}
 
HNL_GenStudy::~HNL_GenStudy(){

}
