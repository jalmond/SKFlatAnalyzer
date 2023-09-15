import os

Classifiers = ["BDTG"]
NTreeOptions = [300,500,700]
NCuts = [200]
Channels = ["EE"]
SignalModes =  [5]
VarModes = [0]
MaxDepths = [1,2,3,4]
EtaBin = [-1]
NormModes = ["EqualNumEvents"]
Eras = ["2016","2017","2018"]
Seeds = ["600","700"]
MinNodeSizes = ["1.0","2.5", "5.0"]
BoostLearningValues = ["0.5","0.1"]
BaggedFracs = ["0.5","0.8"]
BDTVersion = "Version12"

for Classifier in Classifiers:
    
    for NTREES in NTreeOptions:
        for NCut in NCuts:
            for SignalMode in SignalModes:
                for VarMode in VarModes:
                    for MaxDepth in MaxDepths:
                        for Eta in EtaBin:
                            for NormMode in NormModes:
                                
                                for BoostLearningValue in BoostLearningValues:
                                    for BaggedFrac in BaggedFracs:
                                        for Channel in Channels:
                                            for Seed in Seeds:
                                                for MinNodeSize in MinNodeSizes:
                                                    for Era in Eras:
                                                        
                                                        nMAX = " --nmax 150"
                                                        
                                                        os.system("RunIDBDT.py -a runIDBDT_HNtypeIElectronFake -m " + str(Classifier) + " -b FakeBkg -ns "+str(SignalMode)+  " -nv  "+str(VarMode)+  "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) + " -eta "+ str(Eta) +  " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed + "  -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize + " -t " + BDTVersion)    
