import os,ROOT
import sys,math
import argparse
import datetime
import mylib
from array import array
import math

import CMS_lumi, tdrstyle, canvas_margin


from GeneralSetup import check_lxplus_connection,GetFromConfig

WORKING_DIR = os.environ['HNDILEPTONWORKSPACE_DIR']

sys.path.insert(1, WORKING_DIR+'/python')
from os import listdir
from os.path import isfile,isdir, join

import argparse

ROOT.gROOT.SetBatch(ROOT.kTRUE)

from HNType1_config import *

def getall(d, basepath="/"):

    for key in d.GetListOfKeys():
        kname = key.GetName()
        if key.IsFolder():
            for i in getall(d.Get(kname), basepath+kname+"/"):
                yield i
        else:
            yield basepath+kname, d.Get(kname)


def GetLegend(args):
    
    tlg = ROOT.TLegend(0.55, 0.46, 0.92, 0.90)

    if args.DrawRatio:
        tlg = ROOT.TLegend(0.75, 0.46, 0.92, 0.90)
    else:
        tlg = ROOT.TLegend(0.50, 0.56, 0.92, 0.90)
    tlg.SetBorderSize(0)
    tlg.SetFillStyle(0)

    tlg.SetTextSize(0.025)

    return tlg


def DrawHist(HistsName, Inputs,tag,args,SavedHistName):

    PLOT_PATH = os.environ['PLOT_PATH']
    
    outdir = PLOT_PATH+'/IndividualPlots/'
    os.system("mkdir -p " + outdir)

    ROOT.gErrorIgnoreLevel = ROOT.kFatal
    
    tdrstyle.setTDRStyle()
    ROOT.TH1.AddDirectory(False)
    ROOT.gROOT.SetBatchRebin(True)

    c1 = ROOT.TCanvas(HistsName ,HistsName, 800, 800)
    latex_CMSPriliminary = ROOT.TLatex()
    latex_Lumi = ROOT.TLatex()

    latex_CMSPriliminary.SetNDC()
    latex_Lumi.SetNDC()
    latex_CMSPriliminary.SetTextSize(0.055)
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")
    latex_Lumi.SetTextSize(0.035)
    latex_Lumi.SetTextFont(42)
        
    filename = HistsName+tag
    
    print ("Adding plot for file " + Inputs[0])

    f1 = ROOT.TFile(Inputs[0])
    
    h1 = f1.Get(HistsName)
    
    if args.Norm:
        h1.Scale(1./h1.Integral())
        
    h1.Rebin(args.Rebin)
    h1.GetYaxis().SetLabelSize(0.04)
    h1.GetYaxis().SetTitleSize(0.054)
    h1.GetYaxis().SetTitleOffset(1.30)
    h1.GetXaxis().SetLabelSize(0.03)
    h1.GetXaxis().SetTitleSize(0.05)
    
    h1.Draw("axis")
    h1.SetLineColor(ROOT.kRed)

    h1.Draw("histsame")

    lg = GetLegend(args)
    lg.AddEntry( h1, 'hist 1', 'l')                                                                              

    for x in range(1, len(Inputs)):
        print ("Adding plot for file " + Inputs[x])
        fx = ROOT.TFile(Inputs[x])
        hx =  fx.Get(HistsName)
        hx.SetLineColor(GetColorByInt(x))

        if args.Norm:
            hx.Scale(1./hx.Integral())
        hx.Rebin(args.Rebin)
            
        hx.Draw("histsame")
        lg.AddEntry( h1, 'hist '+str(x+1), 'l')
        
    
    lg.Draw()

    filename = outdir+"/"+SavedHistName
    c1.SaveAs(filename+'.pdf')
    c1.SaveAs(filename+'.png')

    lxpath = "/afs/cern.ch/user/j/jalmond/www/SNU/WebPlots/HNL/IndividualPlots/"
    os.system("ssh jalmond@lxplus.cern.ch 'mkdir -p " + lxpath+"'")
    os.system("ssh jalmond@lxplus.cern.ch 'cp  /afs/cern.ch/user/j/jalmond/www/SNU/WebPlots/HNL/SignalStudies/SignalSplit/index.php " + lxpath +"'") 

    os.system("scp " + outdir+"/"+SavedHistName+".png jalmond@lxplus.cern.ch:" + lxpath )
    os.system("scp " + outdir+"/" +SavedHistName +".pdf jalmond@lxplus.cern.ch:" + lxpath )
    print ("scp " + outdir+"/"+ SavedHistName+  ".*   jalmond@lxplus.cern.ch:" + lxpath )
    print ("Plots at  https://jalmond.web.cern.ch/jalmond/SNU/"+lxpath.replace("/afs/cern.ch/user/j/jalmond/www/SNU/",""))
    


def DrawHistWithRatio(HistsName, Inputs,tag,args,SavedHistName):

    PLOT_PATH = os.environ['PLOT_PATH']

    outdir = PLOT_PATH+'/IndividualPlots/'
    os.system("mkdir -p " + outdir)

    ROOT.gErrorIgnoreLevel = ROOT.kFatal

    tdrstyle.setTDRStyle()
    ROOT.TH1.AddDirectory(False)
    ROOT.gROOT.SetBatch(True)

    c1 = ROOT.TCanvas('c1'+HistsName, "c1"+HistsName, 800, 800)
    c1_up = ROOT.TPad("c1_up", HistsName+"down", 0, 0.25, 1, 1)
    c1_down = ROOT.TPad("c1_down", HistsName+"up", 0, 0, 1, 0.25)
    
    c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
    c1.Draw()
    c1_up.Draw()
    c1_down.Draw()
    c1_up.cd()
    #if args.Logy>0:
    #    c1_up.SetLogy(True)

    c1.cd()

       
    latex_CMSPriliminary = ROOT.TLatex()
    latex_Lumi = ROOT.TLatex()

    latex_CMSPriliminary.SetNDC()
    latex_Lumi.SetNDC()
    latex_CMSPriliminary.SetTextSize(0.055)
    latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")
    latex_Lumi.SetTextSize(0.035)
    latex_Lumi.SetTextFont(42)

    filename = HistsName+tag

    print ("Adding plot for file " + Inputs[0])

    f1 = ROOT.TFile(Inputs[0])

    h1 = f1.Get(HistsName)

    if args.Norm:
        h1.Scale(1./h1.Integral())

    h1.Rebin(args.Rebin)
    h1.GetYaxis().SetLabelSize(0.04)
    h1.GetYaxis().SetTitleSize(0.054)
    h1.GetYaxis().SetTitleOffset(1.30)
    h1.GetXaxis().SetLabelSize(0.03)
    h1.GetXaxis().SetTitleSize(0.05)

    c1_up.cd()

    h1.Draw("axis")
    h1.SetLineColor(ROOT.kRed)

    h1.Draw("histsame")

    lg = ROOT.TLegend(0.75, 0.46, 0.92, 0.90)# GetLegend(args)
    lg.SetBorderSize(0)
    lg.SetFillStyle(0)
    lg.SetTextSize(0.025)


    lg.AddEntry( h1, 'hist 1', 'l')



    h_dummy_down = h1.Clone("Tmph1Down")
    h_dummy_down.GetYaxis().SetRangeUser(0.,2.)
    h_dummy_down.SetNdivisions(504,"Y")
    #h_dummy_down.GetXaxis().SetTitle(xtitle)
    h_dummy_down.GetYaxis().SetTitle("#frac{hist1}{hist2}")
    h_dummy_down.SetFillColor(0)
    h_dummy_down.SetMarkerSize(0)
    h_dummy_down.SetMarkerStyle(0)
    h_dummy_down.SetLineColor(ROOT.kWhite)
    
    h1, h_dummy_down = canvas_margin.hist_axis(h1, h_dummy_down)

    for x in range(1, len(Inputs)):
        print ("Adding plot for file " + Inputs[x])
        fx = ROOT.TFile(Inputs[x])
        hx =  fx.Get(HistsName)
        hx.SetLineColor(GetColorByInt(x))

        if args.Norm:
            hx.Scale(1./hx.Integral())
        hx.Rebin(args.Rebin)

        hx.Draw("histsame")

        h_dummy_down.Divide(hx)
        lg.AddEntry( hx, 'hist '+str(x+1), 'l')

    lg.Draw()

    c1_down.cd()
    print("Ratio bin values = " )
    if args.Bins:
        for x in range(1, h_dummy_down.GetXaxis().GetNbins()+1):
            if h_dummy_down.GetBinContent(x) != 0:
                print ( ("Bin [%f - %f], integral=%f") %(h_dummy_down.GetXaxis().GetBinLowEdge(x),h_dummy_down.GetXaxis().GetBinUpEdge(x),h_dummy_down.GetBinContent(x)))


    h_dummy_down.SetLineColor(ROOT.kRed)
    h_dummy_down.Draw("hist")
   

    filename = outdir+"/"+SavedHistName
    c1.SaveAs(filename+'.pdf')
    c1.SaveAs(filename+'.png')

    lxpath = "/afs/cern.ch/user/j/jalmond/www/SNU/WebPlots/HNL/IndividualPlots/"
    os.system("ssh jalmond@lxplus.cern.ch 'mkdir -p " + lxpath+"'")
    os.system("ssh jalmond@lxplus.cern.ch 'cp  /afs/cern.ch/user/j/jalmond/www/SNU/WebPlots/HNL/SignalStudies/SignalSplit/index.php " + lxpath +"'")

    os.system("scp " + outdir+"/"+SavedHistName+".png jalmond@lxplus.cern.ch:" + lxpath )
    os.system("scp " + outdir+"/" +SavedHistName +".pdf jalmond@lxplus.cern.ch:" + lxpath )
    print ("scp " + outdir+"/"+ SavedHistName+  ".*   jalmond@lxplus.cern.ch:" + lxpath )
    print ("Plots at  https://jalmond.web.cern.ch/jalmond/SNU/"+lxpath.replace("/afs/cern.ch/user/j/jalmond/www/SNU/",""))


def PrintHist(HistName,InDir,PrintBins):

    f = ROOT.TFile(InDir)
    hist=f.Get(HistName)
    BinValues = []
    if hist:
        print ( ("%s : Intergal %f") %(InDir, hist.Integral()))
        if PrintBins:
            for x in range(1, hist.GetXaxis().GetNbins()+1):
                BinValues.append(hist.GetBinContent(x))
                if hist.GetBinContent(x) != 0:
                    print ( ("Bin [%f - %f], integral=%f") %(hist.GetXaxis().GetBinLowEdge(x),hist.GetXaxis().GetBinUpEdge(x),hist.GetBinContent(x)))
                    
    else:
        print ( ("%s : Intergal %s") %(InDir, "-999"))

    f.Close()
    return BinValues

def PrintHistsInFile(InDir):

    f = ROOT.TFile(InDir)
        
    HistNames=[]
    
    for k, o in getall(f):
        print k
    f.Close()

def CheckIndividualInput(_s, _sstring):
    if _s == "NULL":
        print _sstring+ " not set...."
        return False
    else:
        return True

def CheckInput(sInput,sHistName,sargs):

    if len(sInput) ==0:
        print "sInput has 0 enters"
        return False

    if not CheckIndividualInput(sHistName,"HistName"):
        if os.path.exists(sInput[0]):
            PrintHistsInFile(sInput[0])
        return False
    
    if os.path.exists(sInput[0]):
        return True

    if not CheckIndividualInput(sInput[0],"Input"):
        return False


    return True

def  GetFromConfig(slines, tag,pargs):
    
    tagged_line=""

    vLines =[]
    for line in slines:
        if tag in line:
            
            if line.split()[0] == "Input":
                vLines.append( line.split()[2])
            else:
                tagged_line = line.split()[1]
                break
    if len(vLines) > 0:
        return vLines
                
    if tagged_line == "":
        if tag == "Era":
            tagged_line = args.Era
        if tag == "Input":
            tagged_line = args.Input
        if tag == "HistName":
            tagged_line = args.HistName

        if tag == "Analyser":
            tagged_line = args.Analyser


    return tagged_line



## Arguments

parser = argparse.ArgumentParser(description='CR plot commands')
parser.add_argument('-e', dest='Era', type=str, default='NULL')
parser.add_argument('-a', dest='Analyser', type=str, default='NULL')
parser.add_argument('-hist', dest='HistName', type=str, default='NULL')
parser.add_argument('-input', dest='Input', type=str, default='NULL')
parser.add_argument('-dir', dest='Dir', type=str, default='')
parser.add_argument('-c', dest='Config', type=str, default='NULL')
parser.add_argument('--Bins', action='store_true')
parser.add_argument('-s', dest='SaveName', type=str, default='NULL')
parser.add_argument('-r', dest='Rebin', type=int, default=1)

parser.add_argument('--DrawRatio',action='store_true')
parser.add_argument('--debug',action='store_true')
parser.add_argument('--Norm',action='store_true')
parser.add_argument('--nonlxcopy', action='store_true')
parser.add_argument('--PrintHists', action='store_true')
parser.add_argument('--List', action='store_true')


args = parser.parse_args()

Analyser='HNL_Validation'


if not args.nonlxcopy:
    check_lxplus_connection()


Era=""
Input=[]
ListInput=[]
Analyzer=""
HistName=""

if args.Config != "NULL":
    
    with open(args.Config) as f:
        lines = f.readlines()
        Input = GetFromConfig(lines,"Input",args)
        Analyser = GetFromConfig(lines,"Analyser",args)
        HistName = GetFromConfig(lines,"HistName",args)
    
    print (("HistName = %s ") %(HistName))
else:
    
    
    if "," in args.Input:
        print "Split Multi Input"
        input_tmp = args.Input
        input_tmp = input_tmp.replace(","," ")
        split_input_tmp = input_tmp.split()
        for x in split_input_tmp:
            if args.Dir == "":
                Input.append(x)
            else:
                Input.append(args.Dir+"/"+x)
    elif "*" in args.Input:
        a  = os.popen('ls ' + args.Input).readlines()
        for x in a:
            if args.Dir == "":
                Input.append(x.split()[0])
            else:
                Input.append(args.Dir+"/"+x.split()[0])

    else:
        Input = [args.Input]
    Analyser = args.Analyser
    HistName = args.HistName

if args.List:
    if len(Input) > 0:
        if ".root" in Input[0]:
            PrintHistsInFile(Input[0])
        else:
            print "Dir is not root file.... list dir and update "
            in_path  = [f for f in listdir(Input[0]) if isfile(join(Input[0],f))]
            for files in in_path:
                print Input[0]+"/"+files
            
    exit()


for _Input in Input:
    print _Input


for _Input in Input:
    print _Input
    if ".root" in _Input:
        ListInput.append(_Input)

    else:
        print _Input
        in_path  = [f for f in listdir(_Input) if isfile(join(_Input,f))]
        for files in in_path:
            if ".root" in files:
                ListInput.append(_Input+"/"+files)



if not CheckInput(Input,HistName,args):
    exit()

for x in ListInput:

    if ".root" in x:
        print (("Looking up hist %s in file %s") % (HistName, x))
    else:
        print("Error in input")
        exit()

BinValuesAll=[]
for x in ListInput:
    BinValuesAll.append(PrintHist(HistName, x,args.Bins))

if len(BinValuesAll)==2:
    if len(BinValuesAll[0]) > 0:
        for x in range(1,len(BinValuesAll)):
            if BinValuesAll[0][x] != BinValuesAll[1][x]:
                print ("Bin %f, has difference " % (x))

DrawHistWithRatio(HistName,ListInput,"",args,args.SaveName+"_Ratio")
