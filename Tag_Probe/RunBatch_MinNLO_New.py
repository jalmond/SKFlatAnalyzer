#!/usr/bin/env python

import os,sys,time
import argparse
import datetime
import random
import subprocess


samples = {
  '2016' : [
    "DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8",
    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "TGJets_TuneCP5_13TeV-amcatnlo-madspin-pythia8",
    "WZG_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWG_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZTo4L_TuneCP5_13TeV_powheg_pythia8",
    "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
    "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "WpWpJJ_QCDnotop_TuneCP5_13TeV-madgraph-pythia8",
    "WpWpJJ_EWKnotop_TuneCP5_13TeV-madgraph-pythia8",
    
    "SingleElectron",
  ],
  '2016preVFP' : [
    "DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8",
    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "TGJets_TuneCP5_13TeV-amcatnlo-madspin-pythia8",
    "WZG_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWG_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZTo4L_TuneCP5_13TeV_powheg_pythia8",
    "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
    "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "WpWpJJ_QCDnotop_TuneCP5_13TeV-madgraph-pythia8",
    "WpWpJJ_EWKnotop_TuneCP5_13TeV-madgraph-pythia8",
     "SingleElectron",
  ],
  '2016postVFP' : [
    "DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8",
    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "TGJets_TuneCP5_13TeV-amcatnlo-madspin-pythia8",
    "WZG_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWG_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZTo4L_TuneCP5_13TeV_powheg_pythia8",
    "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
    "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "WpWpJJ_QCDnotop_TuneCP5_13TeV-madgraph-pythia8",
    "WpWpJJ_EWKnotop_TuneCP5_13TeV-madgraph-pythia8",
    "SingleElectron",
  ],
  '2017' : [
    "DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8",
    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "TGJets_TuneCP5_13TeV-amcatnlo-madspin-pythia8",
    "WZG_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWG_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZTo4L_TuneCP5_13TeV_powheg_pythia8",
    "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
    "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "WpWpJJ_QCDnotop_TuneCP5_13TeV-madgraph-pythia8",
    "WpWpJJ_EWKnotop_TuneCP5_13TeV-madgraph-pythia8",
    "SingleElectron",
  ],
  '2018' : [
    "DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
    "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8",
    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "TGJets_TuneCP5_13TeV-amcatnlo-madspin-pythia8",
    "WZG_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWG_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZTo4L_TuneCP5_13TeV_powheg_pythia8",
    "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
    "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
    "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
    "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
    "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
    "WpWpJJ_QCDnotop_TuneCP5_13TeV-madgraph-pythia8",
    "WpWpJJ_EWKnotop_TuneCP5_13TeV-madgraph-pythia8",
    "EGamma",
  ],
}

# Use below when you share the same sample list
#samples = {}
#sample_list = [
#    "DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
#    "DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos",
#    "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
#    "TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8",
#    "ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
#    "ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8",
#    "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8",
#    "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8",
#    "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
#    "TGJets_TuneCP5_13TeV-amcatnlo-madspin-pythia8",
#    "WZG_TuneCP5_13TeV-amcatnlo-pythia8",
#    "WWG_TuneCP5_13TeV-amcatnlo-pythia8",
#    "ZZTo4L_TuneCP5_13TeV_powheg_pythia8",
#    "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8",
#    "WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8",
#    "WZZ_TuneCP5_13TeV-amcatnlo-pythia8",
#    "ZZZ_TuneCP5_13TeV-amcatnlo-pythia8",
#    "WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8",
#    "WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8",
#    "GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8",
#    "GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8",
#    "GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8",
#    "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8",
#    "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8",
#    "WpWpJJ_QCDnotop_TuneCP5_13TeV-madgraph-pythia8",
#    "WpWpJJ_EWKnotop_TuneCP5_13TeV-madgraph-pythia8",
#  ]
#for era in ['2016', '2016preVFP', '2016postVFP', '2017', '2018']:
#  this_sample_list = sample_list[:]
#  this_sample_list.append("SingleElectron") if era is not '2018' else this_sample_list.append("EGamma")
#  samples[era] = this_sample_list

types = {
  'DYJetsToEE_M-50_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos'  : 'MC',
  'DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_massWgtFix_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos' : 'MC',
  'DYJetsToTauTau_M-50_AtLeastOneEorMuDecay_TuneCP5_13TeV-powhegMiNNLO-pythia8-photos' : 'MC',
  'TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'TTToSemiLeptonic_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8' : 'MC',
  'WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8': 'MC',
  'ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8': 'MC',
  'TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8': 'MC',
  'TGJets_TuneCP5_13TeV-amcatnlo-madspin-pythia8': 'MC',
  'WZG_TuneCP5_13TeV-amcatnlo-pythia8': 'MC',
  'WWG_TuneCP5_13TeV-amcatnlo-pythia8': 'MC',
  'ZZTo4L_TuneCP5_13TeV_powheg_pythia8': 'MC',
  'WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8': 'MC',
  'WZTo3LNu_mllmin4p0_TuneCP5_13TeV-powheg-pythia8': 'MC',
  'WZZ_TuneCP5_13TeV-amcatnlo-pythia8': 'MC',
  'ZZZ_TuneCP5_13TeV-amcatnlo-pythia8': 'MC',
  'WWZ_4F_TuneCP5_13TeV-amcatnlo-pythia8': 'MC',
  'WWW_4F_TuneCP5_13TeV-amcatnlo-pythia8': 'MC',
  'GluGluToContinToZZTo4e_TuneCP5_13TeV-mcfm701-pythia8': 'MC',
  'GluGluToContinToZZTo2e2mu_TuneCP5_13TeV-mcfm701-pythia8': 'MC',
  'GluGluToContinToZZTo2e2tau_TuneCP5_13TeV-mcfm701-pythia8': 'MC',
  'TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8': 'MC',
  'TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8': 'MC',
  'WpWpJJ_QCDnotop_TuneCP5_13TeV-madgraph-pythia8': 'MC',
  'WpWpJJ_EWKnotop_TuneCP5_13TeV-madgraph-pythia8': 'MC',
  'SingleElectron' : 'DATA',
  'EGamma' : 'DATA',
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



def CheckList(input_samples, types_sample,NJobs,Era,SkimName):
    from os import listdir
    from os.path import isfile, isdir,join
    NFiles={}

    for x in input_samples[Era]:
        path_to_files="/gv0/DATA/SKFlat/Run2UltraLegacy_v3/"+Era+"/"+types_sample[x]+"_"+SkimName+"/" + x
        print path_to_files

        if types_sample[x] == "MC":
            datedir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]
            datedir = sorted(datedir)

            print "Use the last item of:",datedir
            new_path_to_files = path_to_files + "/"+datedir[-1]
            infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]
            NFiles[x] = len(infiles)
            #if len(infiles) < NFewestFiles:
            #    NFewestFiles = len(infiles)
            #MakeInputFiles(types_sample[x],Era,x, new_path_to_files,infiles,NJobs)
        else:
            perioddir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]

            for period in perioddir:
                period_path_to_files  = path_to_files + "/"+period
                datedir = [f for f in listdir(period_path_to_files) if isdir(join(period_path_to_files, f))]
                datedir = sorted(datedir)

                print "Use the last item of:",datedir
                new_path_to_files = period_path_to_files + "/"+datedir[-1]
                infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]
                NFiles[x] = len(infiles)
                #if len(infiles) < NFewestFiles:
                #    NFewestFiles = len(infiles)
                #MakeInputFiles(types_sample[x],Era,x+"_"+period, new_path_to_files,infiles,NJobs)

    return NFiles


def MakeInputList(input_samples, types_sample, NJobsToRun,Era,SkimName):

    from os import listdir
    from os.path import isfile, isdir,join

    print "Making MakeInputList for following samples"

    for x in input_samples[Era]:
        path_to_files="/gv0/DATA/SKFlat/Run2UltraLegacy_v3/"+Era+"/"+types_sample[x]+"_"+SkimName+"/" + x
        print path_to_files

        if types_sample[x] == "MC":
            datedir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]
            datedir = sorted(datedir)

            print "Use the last item of:",datedir
            new_path_to_files = path_to_files + "/"+datedir[-1]
            infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]

            MakeInputFiles(types_sample[x],Era,x, new_path_to_files,infiles,NJobsToRun[x])
        else:
            perioddir = [f for f in listdir(path_to_files) if isdir(join(path_to_files, f))]

            for period in perioddir:
                period_path_to_files  = path_to_files + "/"+period
                datedir = [f for f in listdir(period_path_to_files) if isdir(join(period_path_to_files, f))]
                datedir = sorted(datedir)

                print "Use the last item of:",datedir
                new_path_to_files = period_path_to_files + "/"+datedir[0]
                infiles = [f for f in listdir(new_path_to_files) if isfile(join(new_path_to_files, f))]
                MakeInputFiles(types_sample[x],Era,x+"_"+period, new_path_to_files,infiles,NJobsToRun[x])




## Arguments

parser = argparse.ArgumentParser(description='SKFlat Command')
parser.add_argument('-e', dest='Era', default="2017",help="2016preVFP(2016a), 2016postVFP(2016b), 2017, 2018, Run2")
parser.add_argument('-o', dest='Outputdir', default="",help="")
parser.add_argument('-v', dest='Version', default="Version1",help="")
parser.add_argument('-s', dest='Script', default="NULL",help="pick config/*.py")
parser.add_argument('-n', dest='NJob',type=int, default=15,help="")
parser.add_argument('-max', dest='NMax',type=int, default=500,help="")
parser.add_argument('-ScaleCF', dest='ScaleCF', type=float, default=1, help='SF for CF')


args = parser.parse_args()

Version = args.Version
NJobs=args.NJob
Era = args.Era
ScaleCF=args.ScaleCF

if args.Script == "NULL":
    print ("-s command not set")
    exit()

print ("Running MakeInputList:")
print ("Njobs = " + str(args.NJob))
print ("Era = " + str(args.Era))

if not os.path.exists("batch_input"):
    os.mkdir("batch_input")
    print ("making dir... batch_input/")

#os.system("python MakeInputList.py -e " + Era +  " -n " + str(NJobs))

SkimName="SkimTree_EGammaTnP_HNLHighPt"



NJobsToRun=CheckList(samples,types,NJobs,Era,SkimName)

#print NJobsToRun
for k, v in NJobsToRun.items():
  if NJobs < NJobsToRun[k]:
    NJobsToRun[k] = NJobs
  else:
    print("Updating Njobs to run since "+k+" has fewer files: NJobs --> " + str(NJobsToRun[k]))

MakeInputList(samples,types,NJobsToRun,Era,SkimName)

## Add Abosolute path for outputdir

Outputdir = os.getcwd()+'/'+args.Outputdir

## TimeStamp

# 1) dir/file name style
JobStartTime = datetime.datetime.now()
timestamp =  JobStartTime.strftime('%Y_%m_%d_%H%M%S')


# 2) log style
string_JobStartTime =  JobStartTime.strftime('%Y-%m-%d %H:%M:%S')

## Environment Variables

SCRAM_ARCH = os.environ['SCRAM_ARCH']
cmsswrel = os.environ['cmsswrel']

USER = os.environ['USER']

TandPRunlogDir = "/data6/Users/"+USER+"/TandPRunlog/"

os.system("mkdir -p " +TandPRunlogDir)

if Version != "":
  TandPRunlogDir=TandPRunlogDir+Version+"/"
  os.system("mkdir -p " +TandPRunlogDir)

TandPRunlogDir=TandPRunlogDir+Era+"/"
os.system("mkdir -p " +TandPRunlogDir)

UID = str(os.getuid())
HOSTNAME = os.environ['HOSTNAME']
SampleHOSTNAME = HOSTNAME

## Check joblog email

StringForHash=""
random.seed(hash(StringForHash+timestamp+args.Era))

RandomNumber = int(random.random()*1000000)
str_RandomNumber = str(RandomNumber)

webdirname = timestamp+"_"+str_RandomNumber

## Define MasterJobDir

MasterJobDir = TandPRunlogDir+'/TS_'+timestamp+'__'+str_RandomNumber+"__"
MasterJobDir += '__'+HOSTNAME+'/'
os.mkdir(MasterJobDir)

WorkDirs = [os.environ['SKFlat_WD'] + '/Tag_Probe/']

scriptname=""
for WorkDir in WorkDirs:

  scriptname = "RunEff.py"
  if args.Script != "":
    scriptname = args.Script
    
  print ("Running job for output " + MasterJobDir)
    
  os.system('cp ' + WorkDir + '/'+ scriptname+' ' + MasterJobDir)
  os.system('mv batch_input ' + MasterJobDir)
  os.system('cp -r Out_TurnOn/ ' + MasterJobDir)
  os.system('mkdir -p ' +  MasterJobDir+"/Out_Eff/")
  
  scriptname=scriptname.replace('Config/','')

  commandsfilename = 'ElectronIDSF_'+Era+"_"+Version
  run_commands = open(MasterJobDir+'/'+commandsfilename+'.sh','w')

  print>>run_commands,'''#!/bin/bash
SECTION=`printf $1`
WORKDIR=`pwd`

Trial=0

#### make sure use C locale
export LC_ALL=C

#### Don't make root history
export ROOT_HIST=0

#### use cvmfs for root ####
export CMS_PATH=/cvmfs/cms.cern.ch
source $CMS_PATH/cmsset_default.sh
export SCRAM_ARCH={2}
export cmsswrel={3}
cd /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/src
echo "@@@@ SCRAM_ARCH = "$SCRAM_ARCH
echo "@@@@ cmsswrel = "$cmsswrel
echo "@@@@ scram..."
eval `scramv1 runtime -sh`
cd -


source /cvmfs/cms.cern.ch/$SCRAM_ARCH/cms/$cmsswrel/external/$SCRAM_ARCH/bin/thisroot.sh

while [ "$Trial" -lt 3 ]; do
  echo "#### running ####"


  python {0}/{1} -nj ${{SECTION}} -e {4} -ScaleCF {5} -wd {0} 2> err.log

  EXITCODE=$?
  if [ "$EXITCODE" -eq 5 ]; then
    echo "IO error occured.. running again in 300 seconds.."
    Trial=$((Trial+=1))
    sleep 300
  else
    break
  fi
done


if [ "$EXITCODE" -ne 0 ]; then
  echo "ERROR errno=$EXITCODE" >> err.log
fi

cat err.log >&2
exit $EXITCODE
'''.format(MasterJobDir,scriptname, SCRAM_ARCH, cmsswrel, Era ,ScaleCF)
  run_commands.close()

  submit_command = open(MasterJobDir+'/submit.jds','w')
  if args.NMax:
    concurrency_limits='concurrency_limits = n'+str(args.NMax)+'.'+os.getenv("USER")
    print>>submit_command,'''executable = {1}.sh
jobbatchname = {1}
universe   = vanilla
arguments  = $(Process)
log = condor.log
getenv     = True
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
output = job_$(Process).log
error = job_$(Process).err
{2}
queue {0}
'''.format(str(NJobs), commandsfilename,concurrency_limits)
    submit_command.close()


    cwd = os.getcwd()
    os.chdir(MasterJobDir)
    condorOptions = ''
    os.system('condor_submit submit.jds '+condorOptions)
    os.chdir(cwd)


print '##################################################'
print("RunDir: " + MasterJobDir)
print 'Submission Finished'
print '- JobID = '+str_RandomNumber
print '- Run Script = ' + scriptname
print '- Njobs = ' + str(NJobs)
print '- Era = '+str(args.Era)
print '- output will be send to : '+MasterJobDir+"/Out_Eff/"

print '##################################################'
