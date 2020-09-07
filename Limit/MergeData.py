import os

years = ["2016"]

datalist = [ ["2016" , [ "DoubleEG", "DoubleMuon"]], 
             ["2017" , [ "DoubleEG", "DoubleMuon"]],
             ["2018",  [ "EGamma",  "DoubleMuon"]]]

for d in datalist:
    year = d[0]
    if not os.path.exists("/data6/Users/jalmond/HNDiLep/MergedFiles/" + year + "/"):
        os.system("mkdir " + "/data6/Users/jalmond/HNDiLep/MergedFiles/" + year + "/")
    for dataset in d[1]:
        if not os.path.exists("/data6/Users/jalmond/HNDiLep/MergedFiles/" + year + "/" + dataset):
            os.system("mkdir " + "/data6/Users/jalmond/HNDiLep/MergedFiles/" + year + "/" + dataset)

                    
        outpath = "/data6/Users/jalmond/HNDiLep/MergedFiles/" + year + "/"
        if not os.path.exists(outpath):
            os.system("mkdir " + outpath)
            
        inpath = "/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/HNtypeI_JA/"+year + "/"
        if dataset == "DoubleMuon" :
            inpath += "isMM__/DATA/"
        else:
            inpath += "isEE__/DATA/"

        hadd = "hadd " + outpath + "HNtypeI_JA_SkimTree_SSNonIso_"+dataset + ".root "
        hadd = hadd + inpath +"HNtypeI_JA_SkimTree_SSNonIso_"+dataset + "* " 

        print " "
        print hadd
        if os.path.exists(outpath + "HNtypeI_JA_SkimTree_SSNonIso_"+dataset + ".root"):
            os.system("rm " + outpath + "HNtypeI_JA_SkimTree_SSNonIso_"+dataset + ".root")
        os.system(hadd)

