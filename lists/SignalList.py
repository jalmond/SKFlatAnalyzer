import os
from os import listdir
import ROOT 

years =["2016"]
tag="Run2Legacy_v4"
Channel =["MumMum", "MupMup", "EmEm","EpEp", ]
Type=["Schannel","Tchannel"]
mass = ["100","125","200","250","300","400","600","700","800","900","1000","1100","1200","1300","1400","1500","1700","2000"]

xsec_s=[ ["100","0.155",   "0.000965"],
         ["125","0.0489",   "0.0"],
         ["150","0.","0.000491"],
         ["200","0.0068",   "0.000328"],
         ["250","0.00286",   "0."],
         ["300","0.00143",   "0.000183"],
         ["400","0.000473",   "0.000116"],
         ["500,","0.000197","0.0000781"],
         ["600","0.0000944",   "0.000055"],
         ["700","0.0000494",   "0.0000398"],
         ["800","0.0000277",   "0.0000294"],
         ["900","0.0000162",   "0.0000222"],
         ["1000","0.00000992",   "0.0000169"],
         ["1100","0.00000624",   "0.000013"],
         ["1200","0.00000403",   "0.0000102"],
         ["1300","0.00000266",   "0.00000794"],
         ["1400","0.00000178",   "0.00000627"],
         ["1500","0.00000121",   "0.000005"],
         ["1700","0.00000058",   "0.00000321"],
         ["2000","0.000000205",   "0.00000169"]]

for y in years:
    alias=[]
    allalias=[]
    for c in Channel:
        for t in Type:
            postfix=""
            nm=0
            for m in mass:
                print "#"*30
                print "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/ForSNU/HN_"+t+"_"+c+"_"+m+"_nlo.txt"
                print "#"*30
                if not os.path.exists("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/"):
                    print "Failed : " + "/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/"
                    continue
                allalias.append("HN_"+t+"_"+c+"_"+m+"_nlo \n")
                if not os.path.exists("/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/GetEffLumi/2016/GetEffLumi_HN_"+ t + "_"+c+"_"+m+"_nlo.root"):
                    alias.append("HN_"+t+"_"+c+"_"+m+"_nlo \n")
                sample_list=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/ForSNU/HN_"+t+"_"+c+"_"+m+"_nlo.txt","w")
                for f in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/SKFlat_Run2Legacy_v4/"):
                    for f2 in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/SKFlat_Run2Legacy_v4/"+f):
                        for f3 in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/SKFlat_Run2Legacy_v4/"+f+"/"+f2):
                            print "/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/SKFlat_Run2Legacy_v4/"+f+"/"+f2+"/"+f3
                            sample_list.write("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/SKFlat_Run2Legacy_v4/"+f+"/"+f2+"/"+f3 + "\n")
                
                sample_list.close()
                common_list=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/CommonSampleInfo/HN_"+t+"_"+c+"_"+m+"_nlo.txt","w")
                common_list.write("# alias PD xsec nmc sumw\n")

                xsec = -999
                for x in xsec_s:
                    if x[0] == str(m):
                        if t == "Schannel":
                            xsec= x[1]
                        else:
                            xsec=x[2]
                common_list.write("HN_"+t+"_"+c+"_"+m+"_nlo  HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO " + xsec + " 42000 42000 \n")

                common_list.close()
                nm=nm+1

    list=open("MC.txt","w")    
    toRun=False
    for x in alias:
        list.write(x.split()[0]+"\n")
        toRun=True
    list.close()
    alllist=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/lists/Sig2016.txt","w")    
    for x in allalias:
        alllist.write(x.split()[0]+"\n")

    alllist.close()

    if toRun:
            os.system("SKFlat.py -a GetEffLumi -l MC.txt -n 50  --nmax 100  -y "+y )
    for c in Channel:
        for t in Type:
            postfix=""
            nm=0
            for m in mass:
                if not os.path.exists("/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/"):
                    print "Failed : " + "/gv0/DATA/SKFlat/"+tag+"/"+y+"/PrivateMC/HNtypeI/HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO/"
                    continue
                common_list=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/CommonSampleInfo/HN_"+t+"_"+c+"_"+m+"_nlo.txt","w")
                common_list.write("# alias PD xsec nmc sumw\n")
                dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/GetEffLumi/"+y+"/"
                _file = ROOT.TFile(dirpath + "/GetEffLumi_HN_"+t+"_"+c+"_"+m+"_nlo.root")
                hist=_file.Get("sumW")
                values = hist.Integral()
                _file.Close()

                xsec = -999
                for x in xsec_s:
                    if x[0] == str(m):
                        if t == "Schannel":
                            xsec= x[1]
                        else:
                            xsec=x[2]
                common_list.write("HN_"+t+"_"+c+"_"+m+"_nlo  HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO " + xsec + " 50000 "+ str(values)+" \n")

                common_list.close()
                nm=nm+1
