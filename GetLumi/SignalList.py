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
                xsec=0.
                if m=="100":
                    xsec = 0.155
                if m=="200":
                    xsec = 0.0068
                if m=="500":
                    xsec = 0.000197
                if m=="1100":
                    xsec = 0.00000624
                if t =="Tchannel":
                    if m=="100":
                        xsec = 0.000965
                    if m=="200":
                        xsec = 0.000328
                    if m=="500":
                        xsec = 0.0000781
                    if m=="1100":
                        xsec = 0.000013

                sample_list=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample//CommonSampleInfo/HN_"+t+"_"+c+"_"+m+".txt","w")
                sample_list.write("HN_"+t+"_"+c+"_"+m + "  MajoranaNeutrinoTo"+c+"_"+t+"_M-"+m+""+postfix + " " + str(xsec) + " 46000 46000 ")
                sample_list.close()