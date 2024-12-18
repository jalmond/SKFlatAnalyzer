import os 

samples = {
  '2016' : [
     "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
     "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8",
     "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
     "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
     "WW_TuneCP5_13TeV-pythia8",
     "WZ_TuneCP5_13TeV-pythia8",
     "ZZ_TuneCP5_13TeV-pythia8",
     "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
     "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
     "SingleElectron",
  ],
  '2016preVFP' : [
     "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
     "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8",
     "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
     "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
     "WW_TuneCP5_13TeV-pythia8",
     "WZ_TuneCP5_13TeV-pythia8",
     "ZZ_TuneCP5_13TeV-pythia8",
     "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
     "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
     "SingleElectron",
  ],
  '2016postVFP' : [
    "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "WW_TuneCP5_13TeV-pythia8",
    "WZ_TuneCP5_13TeV-pythia8",
    "ZZ_TuneCP5_13TeV-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "SingleElectron",
  ],
  '2017' : [
    "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "WW_TuneCP5_13TeV-pythia8",
    "WZ_TuneCP5_13TeV-pythia8",
    "ZZ_TuneCP5_13TeV-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "SingleElectron",
  ],
  '2018' : [
    "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "WW_TuneCP5_13TeV-pythia8",
    "WZ_TuneCP5_13TeV-pythia8",
    "ZZ_TuneCP5_13TeV-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "EGamma",
  ],
}

types = {
  'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8' : 'MC',
  'WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8' : 'MC',
  'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'WW_TuneCP5_13TeV-pythia8' : 'MC',
  'WZ_TuneCP5_13TeV-pythia8' : 'MC',
  'ZZ_TuneCP5_13TeV-pythia8' : 'MC',
  'SingleElectron' : 'DATA',
  'EGamma' : 'DATA',
}

dates = {
  '2016preVFP':
    {
     'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8' : {
                                                             '' : '2024_12_03_013251',
                                                            },
     'WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8' : {
                                                             '' : '2024_12_03_013251',
                                                            },
     'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'WW_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'WZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'ZZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'SingleElectron' : {
                         'periodB_ver2' : '2024_11_26_102156',
                         'periodC'      : '2024_11_26_102156',
                         'periodD'      : '2024_11_26_102156',
         'periodE'      : '2024_11_26_102156',
         'periodF'      : '2024_11_26_102156',
                        },
    },
  '2016postVFP':
    {
     'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8' : {
                                                             '' : '2024_12_03_013251',
                                                            },
     'WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8' : {
                                                             '' : '2024_12_03_013251',
                                                            },
     'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'WW_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'WZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'ZZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_03_013251',
     },
     'SingleElectron' : {
                         'periodF' : '2024_11_26_102156',
                         'periodG' : '2024_11_26_102156',
                         'periodH' : '2024_11_26_102156',
                        },
    },
  '2017':
    {
     'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8' : {
                                                             '' : '2024_12_13_185927',
                                                            },
     'WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8' : {
                                                             '' : '2024_12_13_185927',
                                                            },
     'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'WW_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'WZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'ZZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'SingleElectron' : {
                         'periodB' : '2024_12_13_185927',
                         'periodC' : '2024_12_13_185927',
                         'periodD' : '2024_12_13_185927',
                         'periodE' : '2024_12_13_185927',
                         'periodF' : '2024_12_13_185927',
                        },
    },

 '2018':
    {
     'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8' : {
                                                             '' : '2024_12_13_185927',
                                                            },
     'WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8' : {
                                                             '' : '2024_12_13_185927',
                                                            },
     'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'WW_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'WZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'ZZ_TuneCP5_13TeV-pythia8' : {
     '' : '2024_12_13_185927',
     },
     'EGamma' : {
                 'periodA' : '2024_12_13_185927',
                 'periodB' : '2024_12_13_185927',
                 'periodC' : '2024_12_13_185927',
                 'periodD' : '2024_12_13_185927',
                },
    },
}
triggers = {
            '2016'        : 'passHltEle27WPTightGsf',
            '2016preVFP'  : 'passHltEle27WPTightGsf',
            '2016postVFP' : 'passHltEle27WPTightGsf',
            '2017'        : 'passHltEle32DoubleEGWPTightGsf',
            '2018'        : 'passHltEle32WPTightGsf',
}
nameFilter = {
  'DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8' : 'DYJets',
  'WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8' : 'WJets',
  'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8' : 'TTLL',
  'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8' : 'TTLJ',
  'ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : 'tW_top',
  'ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : 'tw_antitop',
  'WW_TuneCP5_13TeV-pythia8' : 'WW',
  'WZ_TuneCP5_13TeV-pythia8' : 'WZ',
  'ZZ_TuneCP5_13TeV-pythia8' : 'ZZ',
  'SingleElectron' : 'data',
  'EGamma' : 'data',
}


def MakeInputFiles(_type, _era,_sample,_path,_list,NJOBS):

    if os.path.exists("batch_input/"+_era+"_"+_sample+"_1.txt"):
        os.system("rm " + "batch_input/"+_era+"_"+_sample+"_*.txt")
    nj=1
    for x in _list:
        FilePath = _path+"/"+x +"\n"
        
        fileName="batch_input/"+_era+"_"+_sample+"_"+str(nj-1)+".txt"
        file1= open(fileName, "a") 
        file1.write(FilePath)
        nj=nj+1
        if nj > NJOBS:
            nj=1

        file1.close()
    

def CheckList(input_samples, types_sample,NJobs,Era):
    from os import listdir
    from os.path import isfile, isdir,join
    NFewestFiles=1000000

    for x in input_samples[Era]:
        path_to_files="/gv0/DATA/SKFlat/Run2UltraLegacy_v3/"+Era+"/"+types[x]+"_SkimTree_EGammaTnP_HighPt/" + x
        print path_to_files

        if types[x] == "MC":
            datedir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]
            if len(datedir ) > 1:
                exit
            new_path_to_files = path_to_files + "/"+datedir[0]
            infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]
            if len(infiles) < NFewestFiles:
                NFewestFiles = len(infiles)
            MakeInputFiles(types[x],Era,x, new_path_to_files,infiles,NJobs)
        else:
            perioddir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]
            if len(perioddir ) > 1:
                exit

            for period in perioddir:
                period_path_to_files  = path_to_files + "/"+period
                datedir = [f for f in listdir(period_path_to_files) if isdir(join(period_path_to_files, f))]
                if len(datedir ) > 1:
                    exit

                new_path_to_files = period_path_to_files + "/"+datedir[0]
                infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]
                if len(infiles) < NFewestFiles:
                    NFewestFiles = len(infiles)
                MakeInputFiles(types[x],Era,x+"_"+period, new_path_to_files,infiles,NJobs)

    return NFewestFiles

    
def MakeInputList(input_samples, types_sample, NJobs,Era):

    from os import listdir
    from os.path import isfile, isdir,join

    print "Making MakeInputList for following samples"

    for x in input_samples[Era]:
        path_to_files="/gv0/DATA/SKFlat/Run2UltraLegacy_v3/"+Era+"/"+types[x]+"_SkimTree_EGammaTnP_HighPt/" + x
        print path_to_files

        if types[x] == "MC":
            datedir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]
            if len(datedir ) > 1:
                exit
            new_path_to_files = path_to_files + "/"+datedir[0]
            infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]
    
            MakeInputFiles(types[x],Era,x, new_path_to_files,infiles,NJobs)
        else:
            perioddir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]
            if len(perioddir ) > 1:
                exit

            for period in perioddir:
                period_path_to_files  = path_to_files + "/"+period
                datedir = [f for f in listdir(period_path_to_files) if isdir(join(period_path_to_files, f))]
                if len(datedir ) > 1:
                    exit

                new_path_to_files = period_path_to_files + "/"+datedir[0]
                infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]
                MakeInputFiles(types[x],Era,x+"_"+period, new_path_to_files,infiles,NJobs)

            
import argparse
parser = argparse.ArgumentParser(description='SKFlat Command')
parser.add_argument('-e', dest='Era', default="2017",help="2016preVFP(2016a), 2016postVFP(2016b), 2017, 2018, Run2")
parser.add_argument('-n', dest='NJob',type=int, default=15,help="")
        
args = parser.parse_args()

print ("Running MakeInputList:")
print ("Njobs = " + str(args.NJob))
print ("Era = " + str(args.Era))

NMaxAllowed=CheckList(samples,types,args.NJob,args.Era)
NJob=args.NJob
if args.NJob > NMaxAllowed:
    NJob=NMaxAllowed
    print("Updating Njobs since some input has fewer files: NJobs --> " + str(NJob))
    
MakeInputList(samples,types,NJob,args.Era)
#MakeInputList(samples,types,10,'2017')
#MakeInputList(samples,types,10,'2017')

    
