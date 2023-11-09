#include "HNL_RegionDefinitions21003.h"



bool  HNL_RegionDefinitions21003::RunSignalRegionWW(HNL_LeptonCore::Channel channel, HNL_LeptonCore::ChargeType qq ,std::vector<Lepton *> leps, std::vector<Lepton *> leps_veto , std::vector<Tau> TauColl, std::vector<Jet> JetCollLoose, std::vector<Jet> JetColl, std::vector<Jet> B_JetColl, Event ev, Particle METv, AnalyzerParameter param,  float w){


  if(qq==Plus && leps[0]->Charge() < 0) return false;
  if(qq==Minus && leps[0]->Charge() > 0) return false;

  FillCutflow(HNL_LeptonCore::SR2,w, "SR2_lep_charge",param);

  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_lep_pt",param);

  //if(!PassVBF(JetColl,leps,750., true)) return false;
  bool use_leadjets=true;
  if(1){
    
    double ll_dphi = fabs(TVector2::Phi_mpi_pi( ( (*leps[0]).Phi() - (*leps[1]).Phi() )) );
    if(ll_dphi < 2.) return false;
    FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DPhi",param);

    if( ( (*leps[0]) + (*leps[1]) ).M() < 20.) return false;
    if(JetColl.size() < 2) return false;
    FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJet",param);

    double maxDiJetDeta=0.;
    int ijet1(-1), ijet2(-1);
    for(unsigned int ij = 0; ij < JetColl.size()-1; ij++){
      for(unsigned int ij2 = ij+1; ij2 < JetColl.size(); ij2++){

	double deta = fabs(JetColl[ij].Eta() - JetColl[ij2].Eta());
	if(deta > maxDiJetDeta) {
	  maxDiJetDeta=deta;
	  ijet1=ij;
	  ijet2=ij2;
	}
      }
    }
    if(use_leadjets){ijet1=0;ijet2=1;}

    if(maxDiJetDeta < 2.5) return false;
    FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetEta",param);

    Particle JJ = JetColl[ijet1] + JetColl[ijet2];
    FillCutflow(HNL_LeptonCore::SR2, w, "SR2_DiJetMass",param);
    if(JJ.M() < 750) return false;

    double Av_JetEta= 0.5*(JetColl[ijet1].Eta()+ JetColl[ijet2].Eta());
    double zeppenfeld = TMath::Max((*leps[0]).Eta()  - Av_JetEta , (*leps[1]).Eta()  - Av_JetEta ) /maxDiJetDeta;

    if(zeppenfeld > 0.75) return false;

  }

  
  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_VBF",param);

  bool PassBJetMVeto = (B_JetColl.size()==0);

  double HT(0.);
  for(UInt_t emme=0; emme<JetCollLoose.size(); emme++){
    HT += JetCollLoose[emme].Pt();
  }
  
  
  FillCutflow(HNL_LeptonCore::SR2, w, "SR2_ht_lt1",param);
  
  if(PassBJetMVeto){
    
    FillCutflow(HNL_LeptonCore::SR2, w, "SR2_bveto",param);
    
    double HTLT[5] = { 0.,1., 2, 5., 10};
    double ht_lt = (HT/leps[0]->Pt() > 10.) ? 9. : HT/leps[0]->Pt();
    
    FillHist( "LimitSR2/"+param.Name+"/HT_LT1_Central",  ht_lt,  w, 4, HTLT, "Reco HT/LT1");
    
    FillCutflow(HNL_LeptonCore::ChannelDepSR2, w, GetChannelString(channel) +"_SR2",param);
    
    return true;
    
  } 

  return false;
}






HNL_RegionDefinitions21003::HNL_RegionDefinitions21003(){
      


}
 
HNL_RegionDefinitions21003::~HNL_RegionDefinitions21003(){

}








