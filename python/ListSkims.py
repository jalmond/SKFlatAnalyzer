import os 
from os import listdir
from os.path import isfile,isdir, join

os.system("git branch | grep '*'")

Skims = ["SkimTree_FakeEventSkimBDT","SkimTree_HNMultiLepBDT"]
Eras = ["2018"]
for Skim in Skims:
    print "-"*50
    print "List of Samples with skim " + Skim +":"
    print "-"*50

    if Skim == "SkimTree_FakeEventSkimBDT":
        print ("Input skim = SkimTree_FakeEventSkim : adds BDT variables ")

    if Skim == "SkimTree_HNMultiLepBDT":
        print ("Input skim = SkimTree_HNMultiLep : adds BDT variables ")


    for Era in Eras:
        datapath = "data/Run2UltraLegacy_v3/"+Era+"/Sample/ForSNU/"
        dirlist  = [f for f in listdir(datapath) if isfile(join(datapath,f))]
        if not os.path.exists(datapath):
            exit()
        for _file in dirlist:
            if Skim in _file:
                print (_file)

