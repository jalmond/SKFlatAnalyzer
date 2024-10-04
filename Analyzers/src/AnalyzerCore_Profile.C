#include "AnalyzerCore.h"


void AnalyzerCore::FillProf(TString histname, double xvalue, double yvalue, double weight, int n_bin, double x_min, double x_max, bool IsAverage){ // true by default                                         

  TProfile *this_hist = GetHistPf(histname);
  if( !this_hist ){
    this_hist = new TProfile(histname, "", n_bin, x_min, x_max);
    this_hist->SetDirectory(NULL);
    if(IsAverage) this_hist->SetBit(TH1::kIsAverage);
    maphist_TProfile[histname] = this_hist;
  }

  this_hist->Fill(xvalue, yvalue, weight);

}




void AnalyzerCore::FillProf(TString histname, double xvalue, double yvalue, double zvalue, double weight,
                            int n_binx, double x_min, double x_max,
                            int n_biny, double y_min, double y_max){

  TProfile2D *this_hist = GetHistPf2D(histname);
  if( !this_hist ){
    this_hist = new TProfile2D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max);
    this_hist->SetDirectory(NULL);
    maphist_TProfile2D[histname] = this_hist;
  }

  this_hist->Fill(xvalue, yvalue, zvalue, weight);

}




void AnalyzerCore::FillProf(TString histname, double xvalue, double yvalue, double zvalue, double wvalue, double weight,
                            int n_binx, double x_min, double x_max,
                            int n_biny, double y_min, double y_max,
                            int n_binz, double z_min, double z_max){

  TProfile3D *this_hist = GetHistPf3D(histname);
  if( !this_hist ){
    this_hist = new TProfile3D(histname, "", n_binx, x_min, x_max, n_biny, y_min, y_max, n_binz, z_min, z_max);
    this_hist->SetDirectory(NULL);
    maphist_TProfile3D[histname] = this_hist;
  }

  this_hist->Fill(xvalue, yvalue, zvalue, wvalue, weight);

}

TProfile* AnalyzerCore::GetHistPf(TString histname){

  TProfile *h = NULL;
  std::map<TString, TProfile*>::iterator mapit = maphist_TProfile.find(histname);
  if(mapit != maphist_TProfile.end()) return mapit->second;

  return h;

}

TProfile2D* AnalyzerCore::GetHistPf2D(TString histname){

  TProfile2D *h = NULL;
  std::map<TString, TProfile2D*>::iterator mapit = maphist_TProfile2D.find(histname);
  if(mapit != maphist_TProfile2D.end()) return mapit->second;

  return h;

}

TProfile3D* AnalyzerCore::GetHistPf3D(TString histname){

  TProfile3D *h = NULL;
  std::map<TString, TProfile3D*>::iterator mapit = maphist_TProfile3D.find(histname);
  if(mapit != maphist_TProfile3D.end()) return mapit->second;

  return h;

}


void AnalyzerCore::WriteProfile(){
  
  for(std::map< TString, TProfile* >::iterator mapit = maphist_TProfile.begin(); mapit!=maphist_TProfile.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }
  for(std::map< TString, TProfile2D* >::iterator mapit = maphist_TProfile2D.begin(); mapit!=maphist_TProfile2D.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }
  for(std::map< TString, TProfile3D* >::iterator mapit = maphist_TProfile3D.begin(); mapit!=maphist_TProfile3D.end(); mapit++){
    TString this_fullname=mapit->second->GetName();
    TString this_name=this_fullname(this_fullname.Last('/')+1,this_fullname.Length());
    TString this_suffix=this_fullname(0,this_fullname.Last('/'));
    TDirectory *dir = outfile->GetDirectory(this_suffix);
    if(!dir){
      outfile->mkdir(this_suffix);
    }
    outfile->cd(this_suffix);
    mapit->second->Write(this_name);
    outfile->cd();
  }
  return;
}


void AnalyzerCore::DeleteProfileMaps(){

  for(std::map< TString, TProfile* >::iterator mapit =maphist_TProfile.begin(); mapit!=maphist_TProfile.end(); mapit++)   delete mapit->second;
  maphist_TProfile.clear();
  
  for(std::map< TString, TProfile2D* >::iterator mapit =maphist_TProfile2D.begin(); mapit!=maphist_TProfile2D.end(); mapit++)  delete mapit->second;
  maphist_TProfile2D.clear();

  for(std::map< TString, TProfile3D* >::iterator mapit =maphist_TProfile3D.begin(); mapit!=maphist_TProfile3D.end(); mapit++)  delete mapit->second;
  maphist_TProfile3D.clear();

  return;
}


