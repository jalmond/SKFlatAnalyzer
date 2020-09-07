import os

years = ["2016"]

years =["2016"]
tag="Run2Legacy_v4"
Channel =[ ["MuMu", ["MumMum" , "MupMup"]],
           ["EE"  , ["EmEm","EpEp"]]]

Type=["Schannel","Tchannel"]
mass = ["100","200","300","400","600","700","800","900","1000","1100","1200","1300","1400","1500","1700","2000"]

charge= ["Signal__/","SignalOS__/"]

for y in years:
    outpath = "/data6/Users/jalmond/HNDiLep/MergedFiles/" + y + "/SIG/"


    for m in mass:
        for t in Type:
            for c in Channel:
                
                noMerge=False
                nFiles=0
                hadd = "hadd " + outpath + "HNtypeI_JA_HN_"+t+"_" + c[0] + "_" + m + "_nlo.root "
                s_cp = outpath + "HNtypeI_JA_HN_"+t+"_" + c[0] + "_" + m + "_nlo.root "
                counter=0
                sigs=[]
                for s in c[1]:
                    for ch in charge:
                        inpath = "/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/HNtypeI_JA/2016/"+ch

                        if os.path.exists(inpath +"HNtypeI_JA_HN_"+t+"_" + s + "_" + m + "_nlo.root"):
                            sigs.append(inpath +"HNtypeI_JA_HN_"+t+"_" + s + "_" + m + "_nlo.root")
                for si in sigs:
                    hadd = hadd + " " + si

                if len(sigs) == 1:
                    hadd = "cp " + sigs[0] +" " + s_cp
                if len(sigs)==0:
                    print "No Signal " + outpath + "HNtypeI_JA_HN_"+t+"_" + c[0] + "_" + m + "_nlo.root "
                    continue 
                print " "
                print hadd
                if os.path.exists(outpath + "HNtypeI_JA_HN_"+t+"_" + c[0] + "_" + m + "_nlo.root"):
                    os.system("rm " + outpath + "HNtypeI_JA_HN_"+t+"_" + c[0] + "_" + m + "_nlo.root")
                os.system(hadd)

