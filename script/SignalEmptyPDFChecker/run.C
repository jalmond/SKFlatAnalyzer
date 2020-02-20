#include "myclass.C"

void run(TString filepath){

  myclass m;
  m.SetTreeName("recoTree/SKFlat");
  m.AddFile(filepath);
  m.Init();
  bool isPDFEmpty = m.Loop();
  if(isPDFEmpty) cout << "EMPTYPDF\t" << filepath << endl;

}
