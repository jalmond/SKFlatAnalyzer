#include "HNL_LeptonCore.h"

/*
  FUNCTIONS USED IN CR/SR selection

 */

pair<int,int> HNL_LeptonCore::LeptonPairBestZCand(std::vector<Lepton *> leps){

  Particle Z1Cand;
  double MIN_MZ = 1000000000;
  int Zindex_1(-1),Zindex_2(-1);
  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      if(IsOSSF(leps[iel],leps[iel2])){
	Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
	if(fabs(Z1Cand.M() - M_Z) < MIN_MZ) {
	  MIN_MZ = fabs(Z1Cand.M() - M_Z);
	  Zindex_1=iel;
	  Zindex_2=iel2;
	}	
      }
    }
  }
  if(MIN_MZ==1000000000) return std::make_pair(-1,-1);
  return std::make_pair(Zindex_1,Zindex_2);
}

double HNL_LeptonCore::LeptonMassBestZ(std::vector<Lepton *> leps, pair<int,int> Zind){
  
  if(Zind.first < 0  ) return -1;
  if(Zind.second < 0 ) return -1;

  Particle ZCand = *leps[Zind.first] + *leps[Zind.second];
  
  return ZCand.M();
}
 
double HNL_LeptonCore::LeptonMassNonZ(std::vector<Lepton *> leps, pair<int,int> Zind){

  if(Zind.first < 0  ) return -1;
  if(Zind.second < 0 ) return -1;
  Particle ZCand;
  int lsz=int(leps.size());
  for(int iZ =0; iZ < lsz; iZ++){
    if(iZ == Zind.first) continue;
    if(iZ == Zind.second) continue;
    ZCand+= *leps.at(iZ);
  }
  return ZCand.M();

}


double HNL_LeptonCore::GetMET2ST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){
  double ST = GetST(leps, jets, fatjets,met);
  double met2_st = pow(met.Pt(),2.)/ ST;
  return met2_st;
}


double HNL_LeptonCore::GetST( std::vector<Electron> electrons, std::vector<Muon> muons, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){

  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)      _st +=  jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)   _st +=  fatjets.at(i).Pt();
  for(unsigned int i=0; i<muons.size(); i++)     _st +=  muons[i].Pt();
  for(unsigned int i=0; i<electrons.size(); i++) _st +=  electrons[i].Pt();
  _st += met.Pt();
  return _st;

}

double HNL_LeptonCore::GetST( std::vector<Lepton *> leps, std::vector<Jet> jets, std::vector<FatJet> fatjets,  Particle met){

  double _st(0.);
  for(unsigned int i=0; i<jets.size(); i++)    _st += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++) _st += fatjets.at(i).Pt();
  for(auto ilep : leps) _st +=  ilep->Pt();
  _st += met.Pt();
  return _st;

}


double HNL_LeptonCore::M_T(Particle a, Particle b){
  double dphi = a.DeltaPhi(b);

  return TMath::Sqrt( 2.*a.Pt()*b.Pt()*(1.- TMath::Cos(dphi) ) );

}

double HNL_LeptonCore::GetHT(std::vector<Jet> jets, std::vector<FatJet> fatjets){
  double _ht(0.);
  for(unsigned int i=0; i<jets.size(); i++)_ht += jets.at(i).Pt();
  for(unsigned int i=0; i<fatjets.size(); i++)_ht += fatjets.at(i).Pt();
  return _ht;
}

double HNL_LeptonCore::GetLT(std::vector<Lepton *> leps){
  double lt(0.);
  for(auto ilep : leps) lt +=  ilep->Pt();
  return lt;
}

double HNL_LeptonCore::GetLLMass(std::vector<Muon> leps){

  return GetLLMass(MakeLeptonPointerVector(leps));
}

double HNL_LeptonCore::GetLLMass(std::vector<Electron> leps){

  return GetLLMass(MakeLeptonPointerVector(leps));
}

double HNL_LeptonCore::GetLLMass(std::vector<Lepton *> leps){

  if(leps.size() != 2) return -1.;

  Particle ll = (*leps[0]) + (*leps[1]);
  return ll.M();
}

double HNL_LeptonCore::GetMassMinOSSF(std::vector<Lepton *> leps){

  double minOS=99999999999.;
  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(!IsOSSF(leps[i],leps[j])) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(ll.M() < minOS) minOS=ll.M();
    }
  }
  return minOS;
}


double  HNL_LeptonCore::GetMassMinSSSF(std::vector<Lepton *> leps){

  double minSS=99999999999.;

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(leps[i]->LeptonFlavour() != leps[j]->LeptonFlavour()) continue;
      if(leps[i]->Charge() != leps[j]->Charge() ) continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if(ll.M() < minSS) minSS=ll.M();
    }
  }
  if(minSS < 0 ) cout << "minSS = " << minSS << " " << endl;
  return minSS;

}
bool  HNL_LeptonCore::ZmasslllWindowCheck(std::vector<Lepton *> leps, double MassCut){

  if (leps.size() == 3){
    Particle lll = *leps[0] + *leps[1]+ *leps[2];
    bool passZmass_lll_Window = (fabs(lll.M() - M_Z) < MassCut);
    return passZmass_lll_Window;
  }
  return true;
}

int HNL_LeptonCore::GetIndexNonMinOSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]);
    ll1+= (*leps[1]);
    Particle ll2 = (*leps[0]);
    ll2+= (*leps[2]);
    Particle ll3 = (*leps[1]);
    ll3+= (*leps[2]);

    double minOS=99999999999.;
    if(IsOSSF(leps[0],leps[1])){
      if(ll1.M()  < minOS) { minOS=ll1.M(); index=2;}
    }
    if(IsOSSF(leps[0],leps[2])){
      if(ll2.M()  < minOS) { minOS=ll2.M(); index=1;}
    }
    if(IsOSSF(leps[1],leps[2])){
      if(ll3.M()  < minOS) { minOS=ll3.M(); index=0;}
    }

  }

  return index;

}



int HNL_LeptonCore::GetIndexNonBestZ(std::vector<Lepton *> leps,double mass_diff){

  int     nonZ_index=-1;

  if (leps.size() == 3){
    Particle ll1 = (*leps[0]);
    ll1+= (*leps[1]);
    Particle ll2 = (*leps[0]);
    ll2+= (*leps[2]);
    Particle ll3 = (*leps[1]);
    ll3+= (*leps[2]);

    double minOSZ=99999999999.;
    if(IsOSSF(leps[0],leps[1])){
      if(fabs(ll1.M() - M_Z) < minOSZ) {minOSZ = fabs(ll1.M() - M_Z); nonZ_index=2;}
    }
    if(IsOSSF(leps[0],leps[2])){
      if(fabs(ll2.M() - M_Z) < minOSZ) {minOSZ = fabs(ll2.M() - M_Z);nonZ_index=1;}
    }
    if(IsOSSF(leps[1],leps[2])){
      if(fabs(ll3.M() - M_Z) < minOSZ) {minOSZ = fabs(ll3.M() - M_Z);nonZ_index=0;}
    }
    if(minOSZ < mass_diff) return nonZ_index;
  }
  return -1;

}

bool  HNL_LeptonCore::IsOSSF(Lepton* leps1, Lepton* leps2){
  
  if(leps1->LeptonFlavour() != leps2->LeptonFlavour()) return false;
  if(leps1->Charge() == leps2->Charge() ) return false;
  return true;

}

bool  HNL_LeptonCore::ZmassOSSFWindowCheck(std::vector<Lepton *> leps, double mass_diff){

  for(unsigned int i = 0; i < leps.size(); i++){
    for(unsigned int j = i+1;  j <leps.size(); j++){
      if(!IsOSSF(leps[i],leps[j]))  continue;
      Particle ll = (*leps[i]) + (*leps[j]);
      if((fabs(ll.M() - M_Z) < mass_diff)) return true;
    }
  }

  return false;
}

int HNL_LeptonCore::GetIndexNonMinSSSF(std::vector<Lepton *> leps){

  int     index=-1;

  if (leps.size() == 3){

    Particle ll1 = (*leps[0]);
    ll1+= (*leps[1]);
    Particle ll2 = (*leps[0]);
    ll2+= (*leps[2]);
    Particle ll3 = (*leps[1]);
    ll3+= (*leps[2]);

    double minSS=99999999999.;
    if(fabs(ll1.Charge()) == 2) {
      if(ll1.M()  < minSS) { minSS=ll1.M(); index=2;}
    }
    if(fabs(ll2.Charge()) == 2) {
      if(ll2.M()  < minSS) { minSS=ll2.M(); index=1;}
    }
    if(fabs(ll3.Charge()) == 2) {
      if(ll3.M()  < minSS) { minSS=ll3.M(); index=0;}
    }

  }

  return index;

}

double HNL_LeptonCore::GetMassBestZ(std::vector<Lepton *> leps,  bool bestZ){

  double minMZ = 99999999.;
  double massNonBestZ(-9999.), massBestZ(-9999);
  if(leps.size()==4){

    Particle Z1Cand;
    Particle Z2Cand;

    for(unsigned int iel =0; iel < leps.size() ; iel++){
      for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
        if(iel== iel2) continue;
	if(!IsOSSF(leps[iel],leps[iel2])) continue;
	Z1Cand = (*leps[iel]) + (*leps[iel2]) ;
	int zel1(-9), zel2(-9);
	if(iel ==0 && iel2==1){ zel1=2; zel2=3;    Z2Cand = (*leps[2]) + (*leps[3]);}
	if(iel ==0 && iel2==2){ zel1=1; zel2=3;    Z2Cand = (*leps[1]) + (*leps[3]);}
	if(iel ==0 && iel2==3){ zel1=1; zel2=2;    Z2Cand = (*leps[1]) + (*leps[2]);}
	if(iel ==1 && iel2==2){ zel1=0; zel2=3;    Z2Cand = (*leps[0]) + (*leps[3]);}
	if(iel ==1 && iel2==3){ zel1=0; zel2=2;    Z2Cand = (*leps[0]) + (*leps[2]);}
	if(iel ==2 && iel2==3){ zel1=0; zel2=1;    Z2Cand = (*leps[0]) + (*leps[1]);}
	
	if(IsOSSF(leps[zel1],leps[zel2])){
	  if(( fabs(Z1Cand.M()- M_Z) + fabs(Z2Cand.M()- M_Z)) < minMZ) {
	    minMZ = fabs(Z1Cand.M()- M_Z) + fabs(Z2Cand.M()- M_Z);
	    if(fabs(Z1Cand.M()- M_Z)< fabs(Z2Cand.M()- M_Z) ){ massBestZ=Z1Cand.M(); massNonBestZ=Z2Cand.M();}
	    else{massBestZ=Z2Cand.M(); massNonBestZ=Z1Cand.M();}
	  }
	}
      }
    }
    if(bestZ) return massBestZ;
    return massNonBestZ;
  }
  Particle Z1Cand;

  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      if(!IsOSSF(leps[iel],leps[iel2])) continue;
      Z1Cand = (*leps[iel]) + (*leps[iel2]) ;	
      if(fabs(Z1Cand.M()- M_Z) < minMZ) {minMZ = fabs(Z1Cand.M()- M_Z) ; massBestZ=Z1Cand.M();}
    }
  }
  return massBestZ;

}

bool HNL_LeptonCore::HasLowMassOSSF(std::vector<Lepton *> leps, double MassCut){

  for(unsigned int iel =0; iel < leps.size() ; iel++){
    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      if(!IsOSSF(leps[iel],leps[iel2])) continue;
      Particle OSSF = (*leps[iel]) + (*leps[iel2]) ;
      if(OSSF.M() < MassCut) return true;

    }
  }

  return false;
}


bool HNL_LeptonCore::HasOSSFPair(std::vector<Lepton *> leps){

  for(unsigned int iel =0; iel < leps.size() ; iel++){

    for(unsigned int iel2 =iel+1; iel2 < leps.size() ; iel2++){
      if(iel== iel2) continue;
      if(!IsOSSF(leps[iel],leps[iel2])) continue;
      return true;
    }
  }
  
  return false;  
}
