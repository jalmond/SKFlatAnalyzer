import ROOT,os

def CheckBadFile(filepath):
  f = ROOT.TFile(filepath)

  IsZombie = f.IsZombie()
  IsRecovered = f.TestBit(1024)

  return (IsZombie or IsRecovered)

def CheckHasEntries(filepath):
  

  _localfile = open(filepath, "r")
  nevent_tree1=0
  nevent_tree2=0
  f = ROOT.TFile(filepath)
  _tree  = f.Get("recoTree/SKFlat")
  _tree2  = f.Get("SKFlat")
  if _tree:
    nevent_tree1 = nevent_tree1 + _tree.GetEntries()
  if _tree2:
    nevent_tree2 = nevent_tree2 + _tree2.GetEntries()
  f.Close()
  _localfile.close()


  if nevent_tree1 == 0 :
    return False
  else:
    return True
