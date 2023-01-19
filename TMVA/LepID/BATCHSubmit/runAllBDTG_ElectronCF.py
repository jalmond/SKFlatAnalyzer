import os

Classifiers = ["BDTG"]
NTreeOptions = [300,500,1000,2000]
NCuts = [300]
Channels = ["EE"]
SignalModes =  [0,1]
MaxDepths = [2,3,4,5]
EtaBin = [-1]
NormModes = ["EqualNumEvents"]
Eras = ["2016","2017","2018"]
Seeds = ["100"]
MinNodeSizes = ["2.5","5.0"]
BoostLearningValues = ["0.05","0.5"]
BaggedFracs =  ["0.5","0.8"]


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

                                                    nMAX = " --nmax 25"
                                                    if SignalMode == 1:
                                                        nMAX = " --nmax 25"
                                                    if SignalMode == 0:
                                                        nMAX = " --nmax 25"

                                                    os.system("RunIDBDT.py -a runIDBDT_HNtypeICF -m " + str(Classifier) + " -b CF -ns "+str(SignalMode)+ "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) + " -eta "+ str(Eta) +  " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed + "  -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize)    
