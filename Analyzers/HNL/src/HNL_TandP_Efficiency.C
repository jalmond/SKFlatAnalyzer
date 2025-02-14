#include "HNL_TandP_Efficiency.h"

void HNL_TandP_Efficiency::initializeAnalyzer(){

  HNL_LeptonCore::initializeAnalyzer();

}


void HNL_TandP_Efficiency::executeEvent(){

  
  //==== Gen for genmatching
  AnalyzerParameter param  = InitialiseHNLParameter("");
  Event ev = GetEvent();
  double weight =SetupWeight(ev,param);

  //RunHighPt(param,weight);
  
  RunTaPMuon("HNL_ULID",param,weight);
  RunTaPMuon("POGHighPtTightWithIP",param,weight);
  RunTaPMuon("POGTightWithTightIso",param,weight);
  RunTaPMuon("Peking",param,weight);
  RunTaPMuon("TopHN",param,weight);
  //RunPeking(param,weight);

}

void HNL_TandP_Efficiency::RunHighPt(AnalyzerParameter param, double weight){

  TString ID = "POGHighPt";

  AnalyzerParameter p = HNL_LeptonCore::InitialiseHNLParameter("Basic");
  Event ev = GetEvent();
  vector<Electron> electrons= GetAllElectrons();
  if(DataYear == 2016){
    if(! (ev.PassTrigger("HLT_Ele27_WPTight_Gsf_v")))return;
  }
  if(DataYear == 2017){
    if(! (ev.PassTrigger("HLT_Ele32_WPTight_Gsf_L1DoubleEG_v"))) return;
  }
  if(DataYear == 2018){
    if(! (ev.PassTrigger("HLT_Ele32_WPTight_Gsf_v"))) return;
  }

  double EvWeight=p.w.lumiweight*p.w.PUweight*p.w.prefireweight*p.w.zptweight*p.w.z0weight*p.w.weakweight;
  
  if(electrons.size() == 2){
    
    int nPtbins=14;
    double Ptbins[nPtbins+1] = { 20.,25.,30., 40.,50., 70., 100.,  150.,  200.,350,500., 750,1000,1500,2000};

    if(1){
      Electron el_tag = electrons[0];
      Electron el_probe = electrons[1];
      if(fabs(el_tag.scEta())<1.4442 && el_tag.passHEEPID() && el_tag.PassPath("HLT_Ele32_WPTight_Gsf_L1DoubleEG_v") &&el_tag.Pt() > 35 && el_tag.IsPrompt()){
	if(el_probe.IsFake() && el_probe.Pt() > 10){
	  FillHist("Fake_QCD_All",el_probe.Pt() ,  EvWeight, nPtbins,Ptbins);
	  if(el_probe.PassID("HNL_HighPt_ULID"))           FillHist("Fake_QCD_Pass",el_probe.Pt() ,  EvWeight, nPtbins,Ptbins);
	  else   FillHist("Fake_QCD_Fail",el_probe.Pt() ,  EvWeight, nPtbins,Ptbins);
	}
      }
    }
    if(1){
      Electron el_tag = electrons[1];
      Electron el_probe = electrons[0];
      if(fabs(el_tag.scEta())<1.4442 && el_tag.passHEEPID() && el_tag.PassPath("HLT_Ele32_WPTight_Gsf_L1DoubleEG_v") &&el_tag.Pt() > 35 && el_tag.IsPrompt()){
	if(el_probe.IsFake() && el_probe.Pt() > 10){
          FillHist("Fake_QCD_All",el_probe.Pt() ,  EvWeight, nPtbins,Ptbins);
	  if(el_probe.PassID("HNL_HighPt_ULID"))           FillHist("Fake_QCD_Pass",el_probe.Pt() ,  EvWeight, nPtbins,Ptbins);
          else FillHist("Fake_QCD_Fail",el_probe.Pt() ,  EvWeight, nPtbins,Ptbins);
        }
      }
    }
  }
  return;

  if(!PassMETFilter()) return;
  
  std::vector<Muon>      Muons     = SelectMuons    ( param, "Global",    53., 2.4, weight);

  
  for(unsigned int itag=0; itag < Muons.size(); itag++){

    if(!IsData) {
      if(!Muons[itag].IsPrompt()) continue;
    }

    /// Pass Tag ID
    Muon tag = Muons[itag];
    if(!tag.PassID("POGLoose")) continue;
    if(!tag.PassID("POGHighPt")) continue;
    if((tag.TrkIso()/tag.TuneP4().Pt()) > 0.05) continue;
    if(tag.TrkIso() > 30)  continue;
    if(tag.PassPath("HLT_Mu50_v")){
      
      //// Passes Tag
      for(unsigned int iprobe=0; iprobe <Muons.size(); iprobe++){
	if(iprobe==itag) continue;

	Muon probe = Muons[iprobe];
	if(!IsData){
	  if(!probe.IsPrompt()) continue;
	}
	
	if(( probe.TrkIso()/probe.TuneP4().Pt()) > 0.05) continue;
	if(probe.TrkIso() > 30)  continue;
	
	if(PassTandP_PairCriteria(Muons, itag,iprobe)){
	//// Probe pair

	  double pr_pt = (Muons[iprobe].Pt() > 1000) ? 999 : Muons[iprobe].Pt();

          FillTandP(probe.PassID(ID), TString("HighPt"), ID ,pr_pt,probe.Eta(),weight, TString("Bin1"));
          FillTandP(probe.PassID(ID), TString("HighPt"), ID ,pr_pt,probe.Eta(), weight,TString("Bin2"));

	}
      }
    }
  }
  
}



void HNL_TandP_Efficiency::RunTaPMuon(TString ID, AnalyzerParameter param, double weight){

  std::vector<Muon>      Muons     = SelectMuons    ( param, "Global",    53., 2.4, weight);

  for(unsigned int itag=0; itag < Muons.size(); itag++){

    Muon tag = Muons[itag];
    if(!IsData){
      if(!tag.IsPrompt()) continue;
    }
    if(!tag.PassID("POGTight")) continue;
    if((tag.TrkIso()/tag.TuneP4().Pt()) > 0.05) continue;
    if(tag.TrkIso() > 30)  continue;
    if(tag.RelIso() > 0.15) continue;
    
    //// Passes Tag                                                                                                                                                                                                                                                         
    for(unsigned int iprobe=0; iprobe <Muons.size(); iprobe++){
      
      if(iprobe==itag) continue;
      Muon probe = Muons[iprobe];
      
      if(!IsData){
	if(!probe.IsPrompt()) continue;
      }
      


      if((probe.TrkIso()/probe.TuneP4().Pt()) > 0.05) continue;
      if(probe.TrkIso() > 30)  continue;

      if(PassTandP_PairCriteria(Muons, itag,iprobe)){

        double pr_pt = probe.Pt();

        FillTandP(probe.PassID(ID), ID, ID ,pr_pt,probe.Eta(), 1,"Bin1");
        FillTandP(probe.PassID(ID), ID, ID ,pr_pt,probe.Eta(), 1,"Bin2");

      }/// OS   


      //// Probe pair                                                                                                                                                                                            
      if(PassTandP_PairCriteriaHighMass(Muons, itag,iprobe)){
	
	double pr_pt = probe.Pt();
	
	FillTandP(probe.PassID(ID), "HighMass_"+ID, ID ,pr_pt,probe.Eta(), 1,"Bin1");
	FillTandP(probe.PassID(ID), "HighMass_"+ID, ID ,pr_pt,probe.Eta(), 1,"Bin2");
	
      }/// OS                                                                                                                                                                                                                                                               
    }
  }

}

  


void HNL_TandP_Efficiency::RunPeking(AnalyzerParameter param, double weight){

  TString ID = "Peking";
  std::vector<Muon>      Muons     = SelectMuons    ( param, "Global",    53., 2.4, weight);

  for(unsigned int itag=0; itag < Muons.size(); itag++){

    Muon tag = Muons[itag];
    
    if(!IsData){
      if(!tag.IsPrompt()) continue;
    }

    // Pass Tight ID + ISO
    if(!tag.PassID("POGTight")) continue;
    if((tag.TrkIso()/tag.TuneP4().Pt()) > 0.05) continue;
    if(tag.TrkIso() > 30)  continue;
    if(tag.RelIso() > 0.15) continue;
    
    //// Passes Tag                                                                                                                                                                                                                                                                                                      
    for(unsigned int iprobe=0; iprobe <Muons.size(); iprobe++){
      if(iprobe==itag) continue;
      
      Muon probe = Muons[iprobe];
      
      if(!IsData){
	if(!probe.IsPrompt()) continue;
      }
      
      if((probe.TrkIso()/probe.TuneP4().Pt()) > 0.05) continue;
      if(probe.TrkIso() > 30)  continue;
      
      //// Probe pair                                                                                                                                                                                                                                                                                                    
      if(PassTandP_PairCriteria(Muons, itag,iprobe)){
	
	double pr_pt =  probe.Pt();
	
	FillTandP(probe.PassID(ID),"Peking", ID ,pr_pt,probe.Eta(), 1,"Bin1");
	FillTandP(probe.PassID(ID),"Peking", ID ,pr_pt,probe.Eta(), 1,"Bin2");
	      
      }
    }
  }

}




HNL_TandP_Efficiency::HNL_TandP_Efficiency(){


}
 
HNL_TandP_Efficiency::~HNL_TandP_Efficiency(){

}

