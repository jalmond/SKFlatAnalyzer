import os
from os import listdir
import ROOT

def GetSize(filename):

    _size=0
    _localfile = open(filename, "r")
    for f in _localfile:
        _size += os.path.getsize((f.split())[0])
        
    _localfile.close()
    return round(_size/1024.)
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
    
def GetDirName(_dir, dirlist,_ndirlist):

    for f in range(0, len(dirlist)):
        if dirlist[f] == _dir:
            return "["+ str(_ndirlist[f]) + "]"

    return "[-]"

def IsUsed(filename, prod):
    _localfile = open(filename, "r")
    _used="---"
    _isused=False
    for f in _localfile:
        if _isused:
            break

        if prod in f:    
            _used="Yes"
            _isused=True
    _localfile.close()

    return _used
        

def CheckEventCount(dirpath, alias, skim):
    
    
    if not os.path.exists(dirpath+"/CheckSkim_"+skim+"_"+alias+".root"):
        return
    if not os.path.exists(dirpath+"/CheckSkim_"+alias+".root"):
        return

    _fileskim = ROOT.TFile(dirpath+"/CheckSkim_"+skim+"_"+alias+".root")
    _file = ROOT.TFile(dirpath+"/CheckSkim_"+alias+".root")
    
    if not _fileskim:
        return
    if not _file:
        return

    hist=_file.Get("sumW")
    histskim=_fileskim.Get("sumW")

    print " hist->Integral()  " + str( hist.Integral() )
    print " histskim->Integral()  " + str( histskim.Integral() )
    values = [hist.Integral(), histskim.Integral() ]

    _fileskim.Close()
    _file.Close()
    return values

def CheckMyFile(ver, filename, prod, nfile):
    
    tag="Run2Legacy_v4"
    _localfile = open(filename, "r")
    nFile=0
    nOtherFile=0
    BadFile=[]
    for f in _localfile:
        if prod in f:
            nFile = nFile+1
            _file = ROOT.TFile((f.split())[0])
            _tree  = _file.Get("recoTree/SKFlat")
            if not _tree or _tree.IsZombie():
                BadFile.append((f.split())[0])

        if not prod in f:
            nOtherFile=nOtherFile+1

    _localfile.close()

    if ver==0:
        if nFile == 0:
            return 0
        return int(nfile) - nFile
    
    return BadFile

years =["2016"]#,"2017","2018"]
tag="Run2Legacy_v4"
skim="SkimTree_SSNonIso"
datetag= ["-"]
import ROOT


ex_datetag=["2020_06"]
other_tags=[]
samples_added=[]
test=False
_alias=[]
_samples =[]
_dir =[]
_nFiles=[]
_date_tags=[]
_year=[]
VERBOSE=False
for y in years:
    if VERBOSE:
        print "#"*30
        print "Year: " + y

    for dtag in datetag:

        skim_tmp="_"+skim
        if not skim:
            skim_tmp=skim

        for f in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/"):
            if not  "WZZ" in f:
                continue
                
            alias=""
            found=False
            for al in listdir("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/"):
                if found:
                    continue;
                commonfile = open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/"+al, "r")
                for x in commonfile:
                    if f in x:
                        sx=x.split()
                        alias=sx[0]
                        found=True
                commonfile.close()
                
                if not os.path.exists("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/ForSNU/"+skim +"_"+ alias +".txt"):
                    print "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/ForSNU/"+skim + alias +".txt not found "
                    continue

                os.system("SKFlat.py -a CheckSkim -i " + alias + " -n 50  --nmax 50  -y "+y)
                os.system("SKFlat.py -a CheckSkim -i " + alias + "  --skim " + skim + " -n 50  --nmax 50  -y "+y)
                
                vals = CheckEventCount("/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/CheckSkim/2016/", alias, skim)    
                if vals:
                    if len(vals) == 2:
                        print str(vals[0]) + " : " + str(vals[1])
            continue

            if VERBOSE:
                print "Alias : " + alias + " list of productions in tamsa:"
                os.system("ls /gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/" + f  )

            for d in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/"+f):

                dirname="/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/" + f + "/"+d

                nFiles=0
                for files in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/"+f + "/"+d):
                    if skim:
                        if ".root" in files:
                            nFiles=nFiles+1
                    else:
                        for d2 in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC/"+f+"/"+d+"/"+files):
                            if ".root" in d2:
                                nFiles=nFiles+1
                if VERBOSE:
                    print "NFiles = " + str(nFiles)
                _samples.append("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/"+f )
                _dir.append("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/" )
                _alias.append("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/" +f + alias)
                _date_tags.append("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/" +f  + d)
                _nFiles.append("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/"+f +str(nFiles))
                _year.append(y)

multi_file=[]
out_skim= "Sample list for skim " + skim
print "#"*len(out_skim)
print out_skim
print "#"*len(out_skim)
print "-"*len(out_skim)
sample_space=70
alias_space=20
prod_space= 20
nfile_space=7
nev_space=25
size_space=20
input_space=17
dirlist=[]
_ndirlist=[]
print "Labels for tamsa directory:"
for i in range(0, len(_dir)):
    if not _dir[i] in dirlist:
        dirlist.append(_dir[i])
        _ndirlist.append(len(dirlist))
        print "["+str(len(dirlist)) + "]" + _dir[i]

output_log= "Sample name"+" "*(sample_space-11) +"Alias"+" "*(alias_space-5)+"Production Tag"+" "*(prod_space-14)+"nFiles"+" "*(nfile_space-6)+"Nevent    (NOSKIM)"+" "*(nev_space-19)+"skim_factor(0)%"+" "*(size_space-15) + "ForSNU/Input.txt" +" "*(input_space-16) + "#BadFiles"
output_line="_"*len(output_log)
print output_line
print output_log
print output_line
for i in range(0, len(_samples)):
    if (not _samples[i] in multi_file):
        skim_def=skim+"_"
        if not skim:
            skim_def=skim
            
        if not os.path.exists("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt"):
            print GetDirName(_dir[i], dirlist,_ndirlist) + _samples[i].replace(_dir[i],'')   + " "*(sample_space-len(GetDirName(_dir[i], dirlist,_ndirlist)            + _samples[i].replace(_dir[i],'')))   + _alias[i].replace(_samples[i],'')  + " "*  (alias_space-len(_alias[i].replace(_samples[i],'')))  + _date_tags[i].replace(_samples[i],'')  + " "*(prod_space-len(_date_tags[i].replace(_samples[i],'')))   + str(_nFiles[i]).replace(_samples[i],'')  + " "*(nfile_space - len(str(_nFiles[i]).replace(_samples[i],'')))   + "--"  +" "*(10-len("--" ))   + "--" + " "*( nev_space -10 - len("--")) + "--"  + " "*( size_space - 2)   + "--"  + " "*( input_space - 2)   + "--"

            continue
        sample_list_file=CheckMyFile(1, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt", _date_tags[i].replace(_samples[i],''),  str(_nFiles[i]).replace(_samples[i],''))
        
        used_sample=IsUsed("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt", _date_tags[i].replace(_samples[i],''))
        nFileDiff = CheckMyFile(0, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt", _date_tags[i].replace(_samples[i],''),  str(_nFiles[i]).replace(_samples[i],''))
        if nFileDiff != 0 :
            print "Discrepency in number of files " + str(nFileDiff) + " " + str(_nFiles[i]) + " in " + "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+alias+".txt with those in " + _date_tags[i]
        for sl in sample_list_file:
            print "Error in file " + sl

        nevents = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt")
        nevents_orig = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+_alias[i].replace(_samples[i],'')+".txt")
        skim_size = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt")
        orig_size  = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+_alias[i].replace(_samples[i],'')+".txt")

        neventR=float(nevents)/float(nevents_orig)
        print GetDirName(_dir[i], dirlist,_ndirlist) + _samples[i].replace(_dir[i],'')+ " "*(sample_space-len(GetDirName(_dir[i], dirlist,_ndirlist) + _samples[i].replace(_dir[i],''))) + _alias[i].replace(_samples[i],'') + " "*  (alias_space-len(_alias[i].replace(_samples[i],'')))+ _date_tags[i].replace(_samples[i],'') + " "*(prod_space-len(
            _date_tags[i].replace(_samples[i],''))) + str(_nFiles[i]).replace(_samples[i],'') + " "*(nfile_space - len(str(_nFiles[i]).replace(_samples[i],'')))  + str(nevents) +" "*(10-len(str(nevents) )) + "(%.0f)" %(nevents_orig) + " "*( nev_space -10 - len("(%.0f)" %(nevents_orig))) + "%.4f" %(skim_size/orig_size) + " "*( size_space - len("%.4f" %(skim_size/orig_size))) + used_sample  + " "*( input_space - len(used_sample)) + str(len(sample_list_file))
        
        multi_file.append(_samples[i])
    else:
        sample_list_file=CheckMyFile(1, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt", _date_tags[i].replace(_samples[i],''),  str(_nFiles[i]).replace(_samples[i],''))
        used_sample=IsUsed("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt", _date_tags[i].replace(_samples[i],''))

        nFileDiff = CheckMyFile(0, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt", _date_tags[i].replace(_samples[i],''),  str(_nFiles[i]).replace(_samples[i],''))

        if nFileDiff !=0:
            print "Discrepency in number of files in " + "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt with those in "+ _date_tags[i]
            
        for sl in sample_list_file:
            print "Error in file " + sl
        nevents = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt")
        nevents_orig = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+_alias[i].replace(_samples[i],'')+".txt")
        skim_size = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+skim_def+_alias[i].replace(_samples[i],'')+".txt")
        orig_size  = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[i]+"/Sample/ForSNU/"+_alias[i].replace(_samples[i],'')+".txt")

        neventR=float(nevents)/float(nevents_orig)

        
        print " "*sample_space + _alias[i].replace(_samples[i],'')   + " "*  (alias_space-len(_alias[i].replace(_samples[i],'')))    + _date_tags[i].replace(_samples[i],'')  + " "*(prod_space-len(_date_tags[i].replace(_samples[i],'')))    + str(_nFiles[i]).replace(_samples[i],'')    + " "*(nfile_space - len(str(_nFiles[i]).replace(_samples[i],'')))   + str(nevents)  +" "*(10-len(str(nevents) ))   + "(%.0f)" %(nevents_orig)   + " "*( nev_space -10 - len("(%.0f)" %(nevents_orig)))  + "%.4f" %(skim_size/orig_size)   + " "*( size_space - len("%.4f" %(skim_size/orig_size)))    + used_sample    + " "*( input_space - len(used_sample))   + str(len(sample_list_file))
