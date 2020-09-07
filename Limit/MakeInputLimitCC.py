import os

def GetHistName(channel,SR, mass,year):

    histname =""
    if SR == "Bin1" or SR == "Bin3":
        histname += "SR1"
    else:
        histname += "SR2"

    histname += "_" +channel + "_mn"+  mass + "_"
    if SR == "Bin1" or SR == "Bin2":
        histname += "same_sign"
    else:
        histname += "opposite_sign"

    if channel == "EE":
        histname+= "/"+ histname +"_nevent_HNtypeI_JA_"+channel+"_HNTight2016_"
    else:
        histname+= "/"+ histname +"_nevent_HNtypeI_JA_"+channel+"_HNTightV1_"
    return histname

def GetMassBin(mass):
    
    masses = ["100",
              "125",
              "200",
              "250",
              "300",
            "400",
              "500",
              "600",
              "700",
              "800",
            "900",
              "1000",
              "1100",
              "1200",
              "1300",
              "1400",
              "1500",
              "1700"]
    counter = 0
    for m in masses:
        counter = counter +1
        if m == mass:
            return counter
            

    return -1

def GetSignalEff(channel,SR, mass,year, VBF):
    
    histname=GetHistName(channel,SR, mass,year)

    filepaths = "/data6/Users/jalmond/HNDiLep/MergedFiles/2016/SIG/HNtypeI_JA_HN_"+VBF+"_"+channel+"_"+mass+"_nlo.root"

    total=0
    _file = ROOT.TFile(filepaths)
    if _file:
        hist=_file.Get(histname)
        if hist:
            total += hist.Integral()
    _file.Close()

    if total < 0:
        return 0.
    scale_ = 1.
    if int(mass) < 300:
        scale_ = 1.
    elif int(mass)< 700:
        scale_ = 10.
    else:
        scale_ = 100.

    return round(total*scale_,4)


def GetFake(channel, SR, mass,year):
    histname=GetHistName(channel,SR, mass,year)
    filepaths =[]
    if SR == "Bin1" or SR == "Bin2":
        filepaths.append("/data6/Users/jalmond/HNDiLep/MergedFiles/"        + year + "/HNtypeI_JA_SkimTree_SSNonIso_Fake"+channel+".root"  )
    else:
        filepaths.append("/data6/Users/jalmond/HNDiLep/MergedFiles/"        + year + "/HNtypeI_JA_SkimTree_SSNonIso_FakeOS.root"  )

    total=0
    for f in filepaths:
        _file = ROOT.TFile(f)
        if _file:
            hist=_file.Get(histname)
            if hist:
                total += hist.Integral()

        _file.Close()
    return round(total,4)


def GetCF(channel,SR, mass,year):
    histname=GetHistName(channel,SR, mass,year)
    filepaths =[]
    if SR == "Bin1" or SR == "Bin2":
        filepaths.append("/data6/Users/jalmond/HNDiLep/MergedFiles/"        + year + "/HNtypeI_JA_SkimTree_SSNonIso_CF.root"  )
    else:
        filepaths.append("/data6/Users/jalmond/HNDiLep/MergedFiles/"        + year + "/HNtypeI_JA_SkimTree_SSNonIso_OSCF.root"  )

    total=0
    for f in filepaths:
        _file = ROOT.TFile(f)
        if _file:
            hist=_file.Get(histname)
            if hist:
                total += hist.Integral()
        _file.Close()

    if total < 0:
        return 0.

    return round(total,4)
        
def GetPrompt(channel,SR, mass,year):
        
    histname=GetHistName(channel,SR, mass,year)
    filepaths =[]
    if SR == "Bin1" or SR == "Bin2":
        filepaths.append("/data6/Users/jalmond/HNDiLep/MergedFiles/" + year + "/HNtypeI_JA_SkimTree_SSNonIso_SSPrompt.root")

    else:
        filepaths.append("/data6/Users/jalmond/HNDiLep/MergedFiles/" + year + "/HNtypeI_JA_SkimTree_SSNonIso_OSPrompt.root")


    total=0
    for f in filepaths:
        _file = ROOT.TFile(f)
        if _file:
            hist=_file.Get(histname)
            if hist:
                total += hist.Integral()

        _file.Close()
    return round(total,4)

import ROOT

_channels = ["Schannel","Tchannel", "Combinedchannel"]
channels = ["MuMu", "EE"]
years = ["2016","2017","2018"]
SRs = ["Bin1" ,"Bin2","Bin3","Bin4"]
masses = ["100" ,"200","400", "600","700","800","900","1000","1100", "1200","1500"]
masses_t = [ "600","700","800","900","1000","1100", "1200","1500"]
for year in years:
    for _channel in _channels:
        for channel in channels:
            for SR in SRs:
                _masses = masses
                if _channel == "Tchannel":
                    _masses = masses_t
                                    
                for mass in _masses:
                    pinput = "DataCardsCutCount/"
                    if not os.path.exists(pinput):
                        os.system("mkdir " + pinput)
                    isVBF=""
                    if _channel == "Tchannel":
                        isVBF="_VBF"
                    if _channel == "Combinedchannel":
                        isVBF="_combined"

                    if not os.path.exists("DataCardsCutCount/"+year+"/"):
                        os.system("mkdir DataCardsCutCount/"+year+"/")
                    if not os.path.exists("DataCardsCutCount/"+year+"/" + channel + "_" + SR ):
                        os.system("mkdir DataCardsCutCount/"+year+"/"+ channel + "_" + SR )

                    limitfile = open("DataCardsCutCount/"+year+"/" + channel + "_" + SR + "/card_"+channel + "_" + SR+"_N" + mass + isVBF+".txt","w")
                    limitfile.write("imax 1  number of channels\n")
                    limitfile.write("jmax 3  number of backgroundss\n")
                    limitfile.write("kmax 8  number of nuisance parameters (sources of systematical uncertainties)\n")
                    limitfile.write("------------\n")
                    limitfile.write("# we have just one channel, in which we observe 0 events\n")
                    limitfile.write("bin sr1\n")
                    limitfile.write("observation -1\n")
                    limitfile.write("------------\n")
                    limitfile.write("# now we list the expected events for signal and all backgrounds in that bin\1n")
                    limitfile.write("# the second 'process' line must have a positive number for backgrounds, and 0 for signal\n")
                    limitfile.write("# then we list the independent sources of uncertainties, and give their effect (syst. error)\n")
                    limitfile.write("# on each process and bin\n")
                    limitfile.write("bin	sr1	sr1	sr1	sr1\n")
                    limitfile.write("process	prompt	fake	cf	HN"+mass+"\n")
                    limitfile.write("process	1	2	3	0\n")
                    
                    print "DataCardsCutCount/"+year+"/" + channel + "_" + SR + "/card_"+channel + "_" + SR+"_N" + mass + isVBF+".txt   "  + str(GetSignalEff(channel,SR,mass,year, _channel)) 
                    rate_line = "rate  " + str(GetPrompt(channel,SR, mass,year)) + " " + str(GetFake(channel,SR, mass,year)) + " " + str(GetCF(channel,SR,mass,year) ) + " " + str(GetSignalEff(channel,SR,mass,year, _channel))
                    rate_line += "\n"
                    limitfile.write(rate_line)
                    limitfile.write("Lumi	lnN	1.025	-	-	1.025\n")
                    limitfile.write("MCNorm	lnN	1.135	-	-	-\n")
                    limitfile.write("Fake	lnN	-	1.3	-	-\n")
                    limitfile.write("CF	lnN	-	-	1.439	-\n")
                    limitfile.write("MuonID	lnN	1.02	-	-	1.0054\n")
                    limitfile.write("ElectronE	lnN	1.0251	-	-	1.0191\n")
                    limitfile.write("JES	lnN	1.2491	-	-	1.0154\n")
                    limitfile.write("JER	lnN	1.0706	-	-	1.0385\n")
                    
                
                    limitfile.close()
