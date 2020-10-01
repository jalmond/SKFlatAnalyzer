import os
from os import listdir

years =["2016","2017","2018"]
tag="Run2Legacy_v4"
skim="SkimTree_SSNonIso"


#sept24: add mu5 trig

datetag= ["2020_10_01_044756","2020_10_01_050828","2020_10_01_072820","2020_10_01_090411","2020_10_01_102942","2020_10_01_073044","2020_10_01_072454","2020_10_01_093814"]

ex_datetag=["2020_06","2020_09_02","2020_09_03"]
other_tags=[]
samples_added=[]
test=False
for y in years:
    print "#"*30
    print "Year: " + y

    for dtag in datetag:
        #print "_"*30
        #print "Looking for samples made with " + dtag
        #print "_"*30
        for f in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/"):
            for d in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/"+f):
                alias=""
                found=False
                for al in listdir("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/"):
                    if found:
                        continue;
                    commonfile = open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/"+al, "r")
                    for x in commonfile:
                        if f in x:
                            sx=x.split()
                            alias=sx[0]
                            found=True
                    commonfile.close()

                if d == dtag:
                    if not "/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/" + f in samples_added:
                        print "/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/" + f + "/"+d + " >  /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/ForSNU/"+skim+"_"+alias+".txt" 
                        if not test:
                            os.system("ls /gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/" + f + "/"+d + "/* >  /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/ForSNU/"+skim+"_"+alias+".txt")
                        samples_added.append("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/" + f )
                    
                else:
                    if not ("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/" + f + "/"+d) in other_tags:
                        other_tags.append("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC_"+skim+"/" + f + "/"+d)


print ""
for x in other_tags:
    ignore_=False
    for y in datetag:
        if y in x:
            ignore_=True
    for z in ex_datetag:
        if z in x:
            ignore_=True


    if not ignore_:
        print "These samples are not added " + x 
        for y in samples_added:
            if y in x:
                print "Other sample included " + y
