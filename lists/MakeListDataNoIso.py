import os
from os import listdir

years =["2016","2017","2018"]
tag="Run2Legacy_v4"
skim="SkimTree_NonIso"
datetag="2020_09_0"
check=[]
for y in years:
    periodlist=[]
    datasets=[]
    if y == "2016":
        periodlist = ["B_ver2","C","D","E","F","G","H"]
        datasets=["DoubleEG","DoubleMuon"]
    if y == "2017":

        periodlist = ["B","C","D","E","F"]
        if skim == "SkimTree_NonIso":
            datasets=["SingleElectron","DoubleMuon"]
        else:
            datasets=["DoubleEG","DoubleMuon"]

    if y == "2018":
        periodlist = ["A","B","C","D"]
        datasets=["EGamma","DoubleMuon"]
    for d in datasets:
        for p in periodlist:
            print ""
            print "#"*30
            print d + " : " + p + " : " + skim 
            print "#"*30
            
            datastamp=[]
            for f in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/DATA_"+skim+"/"+d+"/period"+p+"/"):
                if datetag in f:
                    datastamp.append(f)
            if len(datastamp) == 1:
                os.system("ls /gv0/DATA/SKFlat/"+tag+"/"+y+"/DATA_"+skim+"/"+d+"/period"+p+"/"+datastamp[0]+"/SKFlatNtuple_* > /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+ "/"+y+"/Sample/ForSNU/"+skim+"_"+d+"_"+p+".txt")
                print "ls /gv0/DATA/SKFlat/"+tag+"/"+y+"/DATA_"+skim+"/"+d+"/period"+p+"/"+datastamp[0]+"/SKFlatNtuple_* > /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+ "/"+y+"/Sample/ForSNU/"+skim+"_"+d+"_"+p+".txt"
            else :
                for da in datastamp:
                    os.system("ls /gv0/DATA/SKFlat/"+tag+"/"+y+"/DATA_"+skim+"/"+d+"/period"+p+"/"+da+"/SKFlatNtuple_* > /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+ "/"+y+"/Sample/ForSNU/"+skim+"_"+d+"_"+p+"_"+da+".txt")
                    print "ls /gv0/DATA/SKFlat/"+tag+"/"+y+"/DATA_"+skim+"/"+d+"/period"+p+"/"+da+"/SKFlatNtuple_* > /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+ "/"+y+"/Sample/ForSNU/"+skim+"_"+d+"_"+p+"_"+da+".txt"
                    check.append("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+ "/"+y+"/Sample/ForSNU/"+skim+"_"+d+"_"+p+"_"+da+".txt")
                    
print "#"*30
print "Following samples had multiple jobs, check:"
for x in check:
    print x
