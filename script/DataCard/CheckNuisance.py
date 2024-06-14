# convert data cards into work space root files
# run this at /data6/Users/jihkim/CombineTool/CMSSW_10_2_13/src/DataCardsShape/HNL_SignalRegionPlotter/Workspace/myWP

import os
import commands as cmd

#ws_names = cmd.getoutput("ls *.root").replace(".root","").replace("card_","").split('\n')
#
#for i in range(len(ws_names)):
#  print cmd.getoutput("text2workspace.py -P HiggsAnalysis.CombinedLimit.MyModel:myModel "+ws_names[i]+" -o "+card_names[i]+".root")

years = ["2016","2017","2018"]
years = ["Run2"]
#years = ["2017"]
#years = ["2016preVFP","2016postVFP","2018","Run2"]
#masses = ["M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M1000","M10000"]
#masses = ["M90","M100","M150","M200","M300","M400","M500","M600","M700","M800","M900","M1000","M1100","M1200","M1300","M1500","M1700","M2000","M2500","M3000","M5000","M7500","M10000","M15000","M20000"]
#masses = ["M100","M200","M300","M400","M500"]
#masses = ["M100","M200","M500","M600","M1000"]
#channels = ["MuMu","EE","EMu"]
#tags = ["_sronly","_syst",""]

masses = ["M100","M1000","M10000"]
channels = ["MuMu","EE","EMu"]
tags = ["_syst"]

pwd = os.getcwd()

for year in years:
  for channel in channels:
    for mass in masses:
      for tag in tags:
        ws = "card_"+year+"_"+channel+"_"+mass+tag+".root"
        ws_name = ws.replace(".root","").replace("card_","")

        os.system("mkdir -p "+pwd+"/"+ws_name)
        os.chdir(pwd+"/"+ws_name)
        os.system("cp "+pwd+"/"+ws+" .")

        print "Processing",ws_name,"..."

        dict_log = {}
        dict_log["FitDiag"] = cmd.getstatusoutput("combine -M FitDiagnostics "+ws+" --rMin -10 --rMax 10 -n "+ws_name+" --plots")
        dict_log["iniFit"]  = cmd.getstatusoutput("combineTool.py -M Impacts -d "+ws+" -m "+mass.replace("M","")+" --rMin -10 --rMax 10 --robustFit 1 --doInitialFit --name "+ws_name)
        dict_log["actFit"]  = cmd.getstatusoutput("combineTool.py -M Impacts -d "+ws+" -m "+mass.replace("M","")+" --rMin -10 --rMax 10 --robustFit 1 --doFits --name "+ws_name)
        dict_log["Impjson"] = cmd.getstatusoutput("combineTool.py -M Impacts -d "+ws+" -m "+mass.replace("M","")+" --rMin -10 --rMax 10 --robustFit 1 --output "+ws_name+"_impacts.json --name "+ws_name)
        dict_log["Impplot"] = cmd.getstatusoutput("plotImpacts.py -i "+ws_name+"_impacts.json -o "+ws_name)
        for key, value in dict_log.items():
          with open(key+".log",'w') as f:
            f.write(value[1]) # output log
          if value[0] != 0:
            print "**********The status code with",key,"is not zero :",value[0]
            print "**********Please read the log carefully and fix it."

        os.chdir(pwd+"/"+ws_name)
        print "Done."
