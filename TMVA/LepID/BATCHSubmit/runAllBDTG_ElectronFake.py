import os

Classifiers = ["BDTG"]
#NTreeOptions = [500,1000,1500,2000,2500]
NTreeOptions = [1000]
NCuts = [300]
Channels = ["EE"]
SignalModes =  [10]
#MaxDepths = [2,3,4,5]
MaxDepths = [3]
EtaBin = [-1]
NormModes = ["EqualNumEvents"]
Eras = ["2016"]
Seeds = ["100"]
#MinNodeSizes = ["2.5","5.0"]
MinNodeSizes = ["5.0"]

#BoostLearningValues = ["0.05","0.5"]
BoostLearningValues = ["0.05"]
#BaggedFracs =  ["0.5","0.8"]
BaggedFracs =  ["0.8"]


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

                                                    nMAX = " --nmax 75"
                                                    if SignalMode == 1:
                                                        nMAX = " --nmax 40"
                                                    if SignalMode == 0:
                                                        nMAX = " --nmax 40"

                                                    os.system("RunIDBDT.py -a runIDBDT_HNtypeIElectronFake -m " + str(Classifier) + " -b FakeBkg -ns "+str(SignalMode)+ "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) + " -eta "+ str(Eta) +  " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed + "  -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize)    
                                                    os.system("RunIDBDT.py -a runIDBDT_HNtypeIElectronFake -m " + str(Classifier) + " -b FakeBkg_LF -ns "+str(SignalMode)+ "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) + " -eta "+ str(Eta) +  " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed + "  -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize)   
 
                                                    os.system("RunIDBDT.py -a runIDBDT_HNtypeIElectronFake -m " + str(Classifier) + " -b FakeBkg_HF -ns "+str(SignalMode)+ "  -nt "+ str(NTREES) + " -c " + Channel + " -MaxDepth  " + str(MaxDepth) + " -ncut " + str(NCut) + " -eta "+ str(Eta) +  " -Nrom " +NormMode+ " -BoostLearningRate  " + BoostLearningValue + " -BaggedFrac " + BaggedFrac +  " -s " + Seed + "  -e "+Era+" " + nMAX + " -MinNodeSize "+MinNodeSize)    



                                
