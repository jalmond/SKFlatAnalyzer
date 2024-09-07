#include "Electron.h"

int  Electron::PassHNLTop(TString ID) const{
  
  if(ID=="TopHNSST"){
    if(! passMVAID_noIso_WP90()          ) return 0;
    if(! (MiniRelIso()<0.1)              ) return 0;
    if(! (SIP3D()<4)                     ) return 0;
    if(! (fabs(dZ())<0.1)                ) return 0;
    if(! Pass_CaloIdL_TrackIdL_IsoVL()   ) return 0;
    if(! PassConversionVeto()            ) return 0;
    if(! (NMissingHits()<2)              ) return 0;
    if(! (fabs(Eta())<2.5)               ) return 0;
    if(! IsGsfCtfScPixChargeConsistent() ) return 0;
    return 1;
    
  }
  
  
  if(ID.Contains("TopHNSSLFixLMVAIsop4NoSIP_201") or ID.Contains("TopHNSSL_201")){
    if(! (MiniRelIso()<0.4)              ) return 0;
    if(! (fabs(dZ())<0.1)                ) return 0;
    if(! Pass_CaloIdL_TrackIdL_IsoVL()   ) return 0;
    if(! PassConversionVeto()            ) return 0;
    if(! (NMissingHits()<=1)             ) return 0;
    if(! (fabs(Eta())<2.5)               ) return 0;
    if(! IsGsfCtfScPixChargeConsistent() ) return 0;
    float MVACut=-1, fEta=fabs(Eta());
    if     (ID.Contains("2016a")) MVACut=fEta<0.8? 0.96:fEta<1.5? 0.93:0.85;
    else if(ID.Contains("2016b")) MVACut=fEta<0.8? 0.96:fEta<1.5? 0.93:0.85;
    else if(ID.Contains("2017") ) MVACut=fEta<0.8? 0.94:fEta<1.5? 0.79:0.5;
    else if(ID.Contains("2018") ) MVACut=fEta<0.8? 0.94:fEta<1.5? 0.79:0.5;
    
    if(! (MVANoIso()>MVACut or passMVAID_noIso_WP90()) ) return 0;
    return 1;
  }
  
  return -1;
  
}
