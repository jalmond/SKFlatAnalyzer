import os

Classifiers = ["BDTG"]

NTreeOptions = [300,500,700]
NCuts = [200]
Channels = ["MuMu"]
SignalModes =  [7]
VarModes = [0]
MaxDepths = [1,2,3,4]
EtaBin = [-1]
NormModes = ["EqualNumEvents"]
Eras = ["2016" ,"2017"]#,"2018"]
Seeds = ["400","500"]
MinNodeSizes = ["1.0","2.5","5.0"]
BoostLearningValues = ["0.1","0.5"]
BaggedFracs = ["0.5","0.8","1."]
BDTVersion = "Version12"

for Classifier in Classifiers:
    
    for NTREES in NTreeOptions:
        for NCut in NCuts:
            for SignalMode in SignalModes:
                for MaxDepth in MaxDepths:
                    for Eta in EtaBin:
                        for NormMode in NormModes:
                            
                            for BoostLearningValue in BoostLearningValues:
                                for BaggedFrac in BaggedFracs:
                                    for Era in Eras:
                                        for Channel in Channels:
                                            for Seed in Seeds:
                                                for MinNodeSize in MinNodeSizes:
                                                    for VarMode in VarModes:
                                                        nMAX = " --nmax 180"
                                                                                                                
                                                        os.system("RunIDBDT.py -a runIDBDT_HNtypeIMuonFake -m " + str(Classifier) + " -b FakeBkg -ns "+str(SignalMode)+  " -nv  "+str(VarMode)+  "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) + " -eta "+ str(Eta) +  " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed + "  -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize+ " -t " + BDTVersion)   
 


                                
