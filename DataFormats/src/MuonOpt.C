#include "Muon.h"
#include <iostream>
#include <sstream>
#include <string>


bool Muon::Pass_MultiFunction_Opt(TString ID) const {

  if (ID.Contains("HNL2016") && !PassID("HNL_ULID_2016")) return false;
  if (ID.Contains("HNL2017") && !PassID("HNL_ULID_2017")) return false;
  if (ID.Contains("HNL2018") && !PassID("HNL_ULID_2018")) return false;


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

  //for (auto is : subStrings) cout << " --> " << is << endl;                                                                                                                                                                                                                                                              


  TString dxy_method = "";
  TString dz_method = "";
  TString sip_method = "";
  TString iso_methodB="";
  TString iso_methodEC="";
  TString pog_methodB="";
  TString pog_methodEC="";


  TString mva_methodBB="";
  TString mva_methodEC="";

  TString mva_methodLFBB="";
  TString mva_methodLFEC="";

  TString NPMVAString="";

  // If MVA ID then need Loose MVA and pog medium                                                                                                                                                                                                                                                                          
  if(ID.Contains("MVA")&& !Pass_LepMVAID()) return false;

  /// Apply pog medium as loose sel                                                                                                                                                                                                                                                                                        
  if(!isPOGMedium()) return false;

  for(unsigned int i=0; i < subStrings.size(); i++){

    if (subStrings[i].Contains("NPMM")){
      NPMVAString =subStrings[i];
      NPMVAString = NPMVAString.ReplaceAll("NPMM","");
    }

    if (subStrings[i].Contains("MVAHFBB")) mva_methodBB=subStrings[i];
    if (subStrings[i].Contains("MVAHFEC")) mva_methodEC=subStrings[i];

    if (subStrings[i].Contains("MVALFBB")) mva_methodLFBB=subStrings[i];
    if (subStrings[i].Contains("MVALFEC")) mva_methodLFEC=subStrings[i];

    if (subStrings[i].Contains("POGB")) pog_methodB=subStrings[i];
    if (subStrings[i].Contains("POGEC")) pog_methodEC=subStrings[i];

    if (subStrings[i].Contains("ISOB")) iso_methodB=subStrings[i];
    if (subStrings[i].Contains("ISOEC")) iso_methodEC=subStrings[i];

    if (subStrings[i].Contains("DXY")) dxy_method=subStrings[i];
    if (subStrings[i].Contains("DZ"))  dz_method=subStrings[i];
    if (subStrings[i].Contains("SIP")) sip_method=subStrings[i];

  }


  /*cout << "mva_methodBB = " << mva_methodBB << endl;                                                                                                                                                                                                                                                                     
    cout << "mva_methodEC = " << mva_methodEC << endl;                                                                                                                                                                                                                                                                       
    cout << "mva_methodLFBB = " << mva_methodLFBB << endl;                                                                                                                                                                                                                                                                   
    cout << "mva_methodLFEC = " << mva_methodLFEC << endl;                                                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                           
    cout << "pog_methodB " <<  pog_methodB << endl;                                                                                                                                                                                                                                                                          
    cout << "pog_methodEC " <<  pog_methodEC << endl;                                                                                                                                                                                                                                                                        
    cout << "iso_methodB " <<  iso_methodB << endl;                                                                                                                                                                                                                                                                          
    cout << "iso_methodEC " <<  iso_methodEC << endl;                                                                                                                                                                                                                                                                        
    cout << "dxy_method " <<  dxy_method << endl;                                                                                                                                                                                                                                                                            
    cout << "NPMVAString " << NPMVAString << endl;                                                                                                                                                                                                                                                                           
  */


  if(dxy_method.Contains("DXY")){
    double dxy_cut = StringToDouble(dxy_method,"DXY");
    if(fabs(dXY())  > dxy_cut)  return false;
  }
  if(dz_method.Contains("DZ")){
    double dz_cut = StringToDouble(dz_method,"DZ");
    if(fabs(dZ())  > dz_cut)  return false;
  }
  if(sip_method.Contains("SIP")){
    double sip_cut = StringToDouble(sip_method,"SIP");
    if(fabs(SIP3D())  > sip_cut)  return false;
  }

  return   PassIDOptMulti(NPMVAString,pog_methodB,pog_methodEC,mva_methodBB,mva_methodEC ,mva_methodLFBB,mva_methodLFEC,  iso_methodB,iso_methodEC);

}


int  Muon::PassIDOptMulti(TString mva_method,TString sel_methodB,TString sel_methodEC,  TString mva_methodBB, TString mva_methodEC,TString mva_methodLFBB, TString mva_methodLFEC, TString iso_methodB,TString iso_methodEC ) const{

  bool DEBUG=false;

  //  ///NPMVABB1neg1 NPMVABB2neg1 NPMVABB3neg1 NPMVABB40p73POGECT ISOEC0p15 DXYv1/FillEventCutflow/MuonSR 3.28117668762                                                                                                                                                                                                   
  //cout << "PassIDOptMulti " << endl;                                                                                                                                                                                                                                                                                     
  if( fabs(this->Eta())<= 1.479 ){

    if(mva_method != ""){
      //cout << "LF  " << mva_methodLFBB << "  mva_method " <<  mva_method << " mva_methodLFBB = " << mva_methodLFBB << endl;                                                                                                                                                                                              
      if(mva_methodLFBB.Contains("MVALFBB") && !Pass_MVA(HNL_MVA_Fake(mva_method),StringToDouble(mva_methodLFBB,"MVALFBB"),  mva_methodLFBB )) return 0;
    }

    if(mva_methodBB.Contains("MVAHFBB")) {
      //cout << "mva_methodBB = " << mva_methodBB << endl;                                                                                                                                                                                                                                                                 
      if( !Pass_MVA(MVA(),StringToDouble(mva_methodBB,"MVAHFBB"), mva_methodBB )) return 0;
    }
    if(sel_methodB == "POGBT"){
      if(! (isPOGTight()) ) return 0;
    }
    if(sel_methodB == "POGBM"){
      if(! (isPOGMedium()) ) return 0;
    }

    if(iso_methodB != ""){

      double  iso_cut_B = StringToDouble(iso_methodB,"ISOB");

      if(DEBUG) cout << "RelIso " << iso_cut_B << " val= " <<  RelIso() << endl;
      if(! (RelIso()<iso_cut_B) ) return 0;
    }
  }
  else{
    if(DEBUG) cout << "PassIDOpt ENDCAP " << endl;

    if(mva_method != ""){
      //cout << "LF  " <<mva_methodLFEC << "  mva_method " <<  mva_method << " mva_methodLFEC = " << mva_methodLFEC << endl;                                                                                                                                                                                               

      if(mva_methodLFEC.Contains("MVALFEC") && !Pass_MVA(HNL_MVA_Fake(mva_method),StringToDouble(mva_methodLFEC,"MVALFEC"),  mva_methodLFEC )) return 0;
    }
    if(mva_methodEC.Contains("MVAHFEC")){
      //cout << "mva_methodEC = " << mva_methodEC<< endl;                                                                                                                                                                                                                                                                  
      if(!Pass_MVA(MVA(),StringToDouble(mva_methodEC,"MVAHFEC"), mva_methodEC )) return 0;
    }

    if(iso_methodEC != ""){

      double  iso_cut_EC = StringToDouble(iso_methodEC,"ISOEC");

      if(DEBUG) cout << "RelIso " << iso_cut_EC << " val = " << RelIso() <<endl;

      if(! (RelIso()<iso_cut_EC) ) return 0;
    }

    if(sel_methodEC == "POGECT"){
      if(! (isPOGTight()) ) return 0;
    }
    if(sel_methodEC == "POGECM"){
      if(! (isPOGMedium()) ) return 0;
    }
  }

  return 1;

}
