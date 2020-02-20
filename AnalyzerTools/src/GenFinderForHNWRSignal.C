#include "GenFinderForHNWRSignal.h"

GenFinderForHNWRSignal::GenFinderForHNWRSignal(){

  Debug = false;
  Clear();

}

void GenFinderForHNWRSignal::Clear(){

  N_PID = -1;
  SignalLeptonChannel = -1;
  GenAllFound = true;
  IsOffShellPrimaryWR = false;


  WR.SetIsEmpty(true);
  N.SetIsEmpty(true);
  priLep.SetIsEmpty(true);
  secLep.SetIsEmpty(true);
  jet1.SetIsEmpty(true);
  jet2.SetIsEmpty(true);

}

GenFinderForHNWRSignal::~GenFinderForHNWRSignal(){
}

void GenFinderForHNWRSignal::Run(vector<Gen> gens){

  Clear();

  //==== Primary WR
  //==== - Check PID, and mother should be quark (unless we sometimes pick the secondary WR)
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    Gen mother = gens.at(gen.MotherIndex());
    if( (abs( gen.PID() ) == 9900024 || abs( gen.PID() ) == 34) && (abs( mother.PID() ) <= 6) ){

     //=== pythia sample has strange things. first WR is nan
     double forchecknan = gen.M();
     if(forchecknan != forchecknan) continue;

       WR = gen;
       break;
    }
  }
  if(WR.IsEmpty()){
    IsOffShellPrimaryWR = true;
    if(Debug){
      cout << "Can't file WR, but most likely virtual WR" << endl;
      PrintGen(gens);
    }
  }

  //==== N
  //==== - PID
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    if(abs( gen.PID() ) == 9900012 || abs( gen.PID() ) == 9900014 || abs( gen.PID() ) == 9900016 ){
      N_PID = gen.PID();
      N = gen;
      if(abs( gen.PID() ) == 9900012) SignalLeptonChannel = 0;
      else if(abs( gen.PID() ) == 9900014) SignalLeptonChannel = 1;
      else if(abs( gen.PID() ) == 9900016 ) SignalLeptonChannel = 2;
      else SignalLeptonChannel = 999;
      break;
    }
  }
  if(N.IsEmpty()){
    if(Debug){
      cout << "Can't file N" << endl;
      PrintGen(gens);
    }
    GenAllFound = false;
  }

  //==== Primary lepton
  //==== - if onshell WR
  //====   - Mother PID is WR
  //====   - PID is lepton
  //==== - if offshell WR
  //====   - get the first lepton from the index 0..
  if(!IsOffShellPrimaryWR){
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      Gen mother = gens.at(gen.MotherIndex());
      if(abs( mother.PID() ) == 9900024 || abs( mother.PID() ) == 34 ){
        if(abs( gen.PID() ) == 11 || abs( gen.PID() ) == 13){
          priLep = gen;
          break;
        }
      }
    }
  }
  else{
    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      Gen mother = gens.at(gen.MotherIndex());
      if(abs( gen.PID() ) == 11 || abs( gen.PID() ) == 13){
        priLep = gen;
        break;
      }
    }
  }
  if(priLep.IsEmpty()){
    if(Debug){
      cout << "Can't file priLep" << endl;
      cout << "IsOffShellPrimaryWR = " << IsOffShellPrimaryWR << endl;
      PrintGen(gens);
    }
    GenAllFound = false;
  }

  //==== Secondary lepton
  //==== - Mother PID is N
  //==== - PID is lepton
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    Gen mother = gens.at(gen.MotherIndex());
    if(abs( mother.PID() ) == abs( N_PID )){
      if(abs( gen.PID() ) == 11 || abs( gen.PID() ) == 13){
        secLep = gen;
        break;
      }
    }
  }
  if(secLep.IsEmpty()){
    if(Debug){
      cout << "Can't file secLep" << endl;
      PrintGen(gens);
    }
    GenAllFound = false;
  }

  //==== Two jets
  //==== - Mother PID is N
  //==== - |PID| <= 6
  vector<Gen> tmp_jets; //jet1, jet2;
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    Gen mother = gens.at(gen.MotherIndex());
    if(abs( mother.PID() ) == abs( N_PID )){
      if(abs( gen.PID() ) <= 6){
        tmp_jets.push_back( gen );
      }
    }
  }
  //==== If second WR appeared in the history, mother is not N but WR
  //==== So the tmp_jets size is not 2
  //==== then,
  //==== - Mother PID is WR
  //==== - |PID| <= 6
  Gen secWR;
  if(tmp_jets.size()!=2){

    for(unsigned int i=2; i<gens.size(); i++){
      Gen gen = gens.at(i);
      Gen mother = gens.at(gen.MotherIndex());
      if(abs( mother.PID() ) == 9900024 || abs( mother.PID() ) == 34 ){
        if(abs( gen.PID() ) <= 6){
          tmp_jets.push_back( gen );
        }
      }
    }
    if(tmp_jets.size()!=2){
      if(Debug){
        cout << "Can't file correct jets : tmp_jets.size() = tmp_jets" << endl;
        PrintGen(gens);
      }
      GenAllFound = false;
    }
  }
  jet1 = tmp_jets.at(0);
  jet2 = tmp_jets.at(1);

}

vector<int> GenFinderForHNWRSignal::TrackGenSelfHistory(Gen me, std::vector<Gen> gens){

  int myindex = me.Index();

  if(myindex<2){
    vector<int> out = {myindex, -1};
    return out;
  }

  int mypid = gens.at(myindex).PID();

  int currentidx = myindex;
  int motherindex = me.MotherIndex();

  while(gens.at(motherindex).PID() == mypid){

    //==== Go one generation up
    currentidx = motherindex;
    motherindex = gens.at(motherindex).MotherIndex();

    if(motherindex<0) break;
  }

  vector<int> out = {currentidx, motherindex};

  return out;

}

void GenFinderForHNWRSignal::Print(){

  cout << "[GenFinderForHNWRSignal::Print] GenAllFound = " << GenAllFound << endl;
  cout << "[GenFinderForHNWRSignal::Print] N_PID = " << N_PID << endl;
  cout << "[GenFinderForHNWRSignal::Print] SignalLeptonChannel (0=ee, 1=mm) = " << SignalLeptonChannel << endl;
  cout << "[GenFinderForHNWRSignal::Print] IsOffShellPrimaryWR = " << IsOffShellPrimaryWR << endl;
  cout << "[GenFinderForHNWRSignal::Print] WR = "; WR.Print();
  cout << "[GenFinderForHNWRSignal::Print] N = "; N.Print();
  cout << "[GenFinderForHNWRSignal::Print] priLep = "; priLep.Print();
  cout << "[GenFinderForHNWRSignal::Print] secLep = "; secLep.Print();
  cout << "[GenFinderForHNWRSignal::Print] jet1 = "; jet1.Print();
  cout << "[GenFinderForHNWRSignal::Print] jet2 = "; jet2.Print();

}

void GenFinderForHNWRSignal::PrintGen(std::vector<Gen> gens){

  cout << "===========================================================" << endl;
  cout << "index\tPID\tStatus\tMIdx\tMPID\tStart\tPt\tEta\tPhi\tM" << endl;
  for(unsigned int i=2; i<gens.size(); i++){
    Gen gen = gens.at(i);
    vector<int> history = TrackGenSelfHistory(gen, gens);
    cout << i << "\t" << gen.PID() << "\t" << gen.Status() << "\t" << gen.MotherIndex() << "\t" << gens.at(gen.MotherIndex()).PID()<< "\t" << history[0] << "\t";
    printf("%.2f\t%.2f\t%.2f\t%.2f\n",gen.Pt(), gen.Eta(), gen.Phi(), gen.M());
  }

}
