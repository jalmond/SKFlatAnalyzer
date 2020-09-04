import os
from os import listdir

years =["2016"]
tag="Run2Legacy_v4"
Channel =["MumMum", "MupMup", "EmEm","EpEp"]
Type=["Schannel","Tchannel"]
mass = ["100","200","500","1100"]
for y in years:
    for c in Channel:
        for t in Type:
            postfix="_TuneCUETP8M1_13TeV-amcatnlo"
            if t =="Tchannel":
                 postfix="_TuneCUETP8M1_13TeV-madgraph"
            for m in mass:
                print "#"*30
                print "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/ForSNU/HN_"+t+"_"+c+"_"+m+".txt"
                print "#"*30
                if not os.path.exists("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/"):
                    print "Failed : " + "/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/"
                    continue
                
                sample_list=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/ForSNU/HN_"+t+"_"+c+"_"+m+".txt","w")
                for f in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/SKFlat_Run2Legacy_v4/"):
                    for f2 in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/SKFlat_Run2Legacy_v4/"+f):
                        for f3 in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/SKFlat_Run2Legacy_v4/"+f+"/"+f2):
                            print "/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/SKFlat_Run2Legacy_v4/"+f+"/"+f2+"/"+f3
                            sample_list.write("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/SKFlat_Run2Legacy_v4/"+f+"/"+f2+"/"+f3 + "\n")
                sample_list.close()

                    #os.system("/gv0/DATA/SKFlat/"+tag+"/"+y"/PrivateMC/HNtypeI/MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix+"/SKFlat_Run2Legacy_v4/200130_074238/0000/SKFlatNtuple_2016_MC_1.root > /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/2016/Sample/ForSNU/HN_S_MumMum_100.txt"1
