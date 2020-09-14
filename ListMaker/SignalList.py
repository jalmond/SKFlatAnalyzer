import os
from os import listdir
import ROOT 

def GetXsecDouble(s_mass):
    mass = int(s_mass)
    if mass==100:
        return 7.77E-02;
    if mass==125:
        return 2.45E-02;
    if mass==150:
        return 1.10E-02;
    if mass==200:
        return 3.40E-03;
    if mass==250:
        return 1.43E-03;
    if mass==300:
        return 7.13E-04;
    if mass==400 :
        return 2.37E-04;
    if mass==500:
        return 9.86E-05;
    if mass==600:
        return 4.72E-05;
    if mass==700:
        return 2.47E-05;
    if mass==800:
        return 1.38E-05;
    if mass==900:
        return 8.11E-06;
    if mass==1000:
        return 4.96E-06;
    if mass==1100:
        return 3.12E-06;
    if mass==1200:
        return 2.02E-06;
    if mass==1300:
        return 1.33E-06;
    if mass==1400:
        return 8.91E-07;
    if mass==1500:
        return 6.05E-07;
    if mass==1700:
        return 2.90E-07;
    if mass==2000:
        return 1.02E-07;

    return 0.

def GetXSec(channel,s_mass):
    
    xsec = GetXsecDouble(s_mass) * 0.45
    if "p" in channel:
        xsec = GetXsecDouble(s_mass) * 0.55

    return str(xsec)
    
def GetXsecDouble_tchan(s_mass):
    mass = int(s_mass)
    if mass==100:
        return 9.65E-04;
    if mass==150:
        return 4.91E-04;
    if mass==200:
        return 3.28E-04;
    if mass==300:
        return 1.83E-04;
    if mass==400:
        return 1.16E-04;
    if mass==500:
        return 7.81E-05;
    if mass==600:
        return 5.50E-05;
    if mass==700:
        return 3.98E-05;
    if mass==800:
        return 2.94E-05;
    if mass==900:
        return 2.22E-05;
    if mass==1000:
        return 1.69E-05;
    if mass==1100:
        return 1.30E-05;
    if mass==1200:
        return 1.02E-05;
    if mass==1300:
        return 7.94E-06;
    if mass==1400 :
        return 6.27E-06;
    if mass==1500:
        return 5.00E-06;
    if mass==1700:
        return 3.21E-06;
    if mass==2000:
        return 1.69E-06;
    
    return 0.

def GetXSecTChan(channel, s_mass):

    xsec = GetXsecDouble_tchan(s_mass)*0.45
    if "p" in channel:
        xsec = GetXsecDouble_tchan(s_mass) * 0.55

    return str(xsec)

def NEvents(filename):

    _localfile = open(filename, "r")
    nevent_tree1=0
    nevent_tree2=0
    for f in _localfile:
        _file = ROOT.TFile((f.split())[0])
        _tree  = _file.Get("recoTree/SKFlat")
        _tree2  = _file.Get("SKFlat")
        if _tree:
            nevent_tree1 = nevent_tree1 + _tree.GetEntries()
        if _tree2:
            nevent_tree2 = nevent_tree2 + _tree2.GetEntries()
        _file.Close()
    _localfile.close()

    return nevent_tree1
    print "recoTree/SKFlat  has a total event count of " + str(nevent_tree1)
    print "SKFlat metafile  has a total event count of " + str(nevent_tree2)


years =["2016"]
tag="Run2Legacy_v4"
Channel =["MumMum", "MupMup", "EmEm","EpEp", ]
Type=["Schannel","Tchannel"]
mass = ["100","125","200","250","300","400","500", "600","700","800","900","1000","1100","1200","1300","1400","1500","1700","2000"]



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
                
                xsec = 0.
                if t == "Schannel":
                    xsec= GetXSec(c,m)
                else:
                    xsec=GetXSecTChan(c,m)

                common_list.write("HN_"+t+"_"+c+"_"+m+"_nlo  HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO " + xsec + " 42000 42000 \n")

                common_list.close()
                nm=nm+1

    list=open("MC.txt","w")    
    toRun=False
    for x in alias:
        list.write(x.split()[0]+"\n")
        toRun=True
    list.close()
    alllist=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/ListMaker/Sig2016.txt","w")    
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
                print "Making " + "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/CommonSampleInfo/HN_"+t+"_"+c+"_"+m+"_nlo.txt"
                common_list=open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/CommonSampleInfo/HN_"+t+"_"+c+"_"+m+"_nlo.txt","w")
                common_list.write("# alias PD xsec nmc sumw\n")
                dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/GetEffLumi/"+y+"/"
                _file = ROOT.TFile(dirpath + "/GetEffLumi_HN_"+t+"_"+c+"_"+m+"_nlo.root")
                hist=_file.Get("sumW")
                values = hist.Integral()
                _file.Close()

                xsec = 0.
                if t == "Schannel":
                    xsec= GetXSec(c,m)
                else:
                    xsec=GetXSecTChan(c,m)

                common_list.write("HN_"+t+"_"+c+"_"+m+"_nlo  HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO " + xsec + " "+ str(NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/ForSNU/HN_"+t+"_"+c+"_"+m+"_nlo.txt")) +" "+ str(values)+" \n")

                print  "HN_"+t+"_"+c+"_"+m+"_nlo  HeavyNeutrinoTo"+c+"_"+t+"_M"+m+"_NLO " + xsec + " "+ str(NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+y+"/Sample/ForSNU/HN_"+t+"_"+c+"_"+m+"_nlo.txt")) +" "+ str(values)
                common_list.close()
                nm=nm+1
