#include "HNL_LeptonCore.h"


bool HNL_LeptonCore::PassGenMatchFilter(vector<Lepton *> leps, AnalyzerParameter param){

  /// Function to split Prompt/Conv/CF/Fake IF code uses RunPrompt/RunX flags etc....        
  /// If code does not use Run* to sepeate MC samples then function should return true  
  /// If user used Data driven method for Fake/CF then function returns for Fake/F bkf true for data and false for MC
  /// If user used Data driven method for Fake/CF then for Conv function requires at least one conv lepton is present
  
  bool DEBUG = false;
  if(DEBUG) cout << "RunFake = " << RunFake << " RunCF = " << RunCF << " RunConv = " << RunConv << endl;
  if(DEBUG) cout << "param.FakeMethod = " << param.FakeMethod << " param.CFMethod = " << param.CFMethod << " param.ConvMethod " << param.ConvMethod << endl;
  if(IsData) return true;
  if(RunFake && param.FakeMethod != "MC") return false;
  if(RunCF   && param.CFMethod   != "MC") return false;
  if(RunConv && param.ConvMethod != "MC") return false;

  if(MCSample.Contains("Type")) return true;

  if(RunPromptTLRemoval) return true;

  //// Function filters events when using MC based on if they are Fake/CF/Conv

  int nConv(0);
  int nCF=(0);
  int nFake=(0);
  unsigned int nPrompt(0);
  for(auto ilep: leps){
    //int LepType= GetLeptonType_JH(*ilep, gens);                                  
    if( ilep->IsConv())     nConv++; /// Add Prompt Conv to IsConv Fnct
    else if( ilep->LeptonIsCF()) nCF++;
    else if( ilep->IsFake())     nFake++;
    else if( ilep->IsPrompt())   nPrompt++;
    if( ilep->IsConv())          cout << "[PassGenMatchFilter] IsConv : " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << endl; // JH
    else if( ilep->LeptonIsCF()) cout << "[PassGenMatchFilter] LeptonIsCF : " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << endl; // JH
    else if( ilep->IsFake())     cout << "[PassGenMatchFilter] IsFake : " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << endl; // JH
    else if( ilep->IsPrompt())   cout << "[PassGenMatchFilter] IsPrompt : " << ilep->Pt() << " " << ilep->Eta() << " " << ilep->Phi() << endl; // JH
  }
  if(DEBUG) cout << "nConv = " << nConv << " nCF = " << nCF << " nFake = " << nFake << " nPrompt = " << nPrompt << endl;
  cout << "[PassGenMatchFilter] nConv = " << nConv << " nCF = " << nCF << " nFake = " << nFake << " nPrompt = " << nPrompt << endl; //JH
  if(RunPrompt && (nPrompt == leps.size())) return true;
  if(RunPrompt && (nPrompt != leps.size())) return false;

  if( (RunFake || RunConv || RunCF )){
    if(RunFake  && nFake > 0)  return true;
    if(!RunFake && nFake > 0)  return false;
    if(RunCF    && nCF   > 0)  return true;
    if(!RunCF   && nCF > 0)    return false;
    if(RunConv  && nConv > 0)  return true;
    if(!RunConv && nConv > 0) return false;
    return false;
  }

  return true;

}
