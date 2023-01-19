import os

Classifiers = ["BDTA"]
NTreeOptions = [400,1000]
NCuts = [50,300,500]
Channels = ["EE","MuMu"]
SignalModes =  [0,1,2]
MaxDepths = [1]#,2,3,4]
EtaBin = [0,1,2]
NormModes = ["EqualNumEvents"]
Eras = ["Run2"]
Seeds = ["100"]
MinNodeSizes = ["2.5","5.0"]

BoostLearningValues = ["-1"]
BaggedFracs =  ["-1"]


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

                                                    nMAX = " --nmax 80"
                                                    if SignalMode == 1:
                                                        nMAX = " --nmax 70"
                                                    if SignalMode == 0:
                                                        nMAX = " --nmax 75"

                                                    os.system("RunIDBDT.py -a runIDBDT_HNtypeI -m " + str(Classifier) + " -b Fake -ns "+str(SignalMode)+ "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) + " -eta "+ str(Eta) +  " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed + "  -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize)    
                                                    if Channel == "EE":
                                                        os.system("RunIDBDT.py -a runIDBDT_HNtypeI -m " + str(Classifier) + " -b CF -ns "+str(SignalMode)+ "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) +  " -eta "+ str(Eta) + " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +   " -s " + Seed + " -e "+Era+" " + nMAX  + " -MinNodeSize "+MinNodeSize)    
                                                
                                                    os.system("RunIDBDT.py -a runIDBDT_HNtypeI -m " + str(Classifier) + " -b Conv -ns "+str(SignalMode)+ "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) +   " -eta "+ str(Eta) +" -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed +  " -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize)    
                                
