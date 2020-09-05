import os
from os import listdir

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

def CheckEventCountLumi(dirpath, alias):
    if not os.path.exists(dirpath+"/GetEffLumi_"+alias+".root"):
        return -999
    _file = ROOT.TFile(dirpath+"/GetEffLumi_"+alias+".root")
    if not _file:
        return -999
    hist=_file.Get("sumW")
    values = hist.Integral()
    _file.Close()
    return values
        
def CheckEventCount(dirpath, alias, skim):


    if not os.path.exists(dirpath+"/CheckSkim_"+skim+"_"+alias+".root"):
        return [0.,0.]
    if not os.path.exists(dirpath+"/CheckSkim_"+alias+".root"):
        return [0.,0.]

    _fileskim = ROOT.TFile(dirpath+"/CheckSkim_"+skim+"_"+alias+".root")
    _file = ROOT.TFile(dirpath+"/CheckSkim_"+alias+".root")

    if not _fileskim:
        return [0.,0.]
    if not _file:
        return [0.,0.]

    hist=_file.Get("sumW")
    histskim=_fileskim.Get("sumW")

    #print " hist->Integral()  " + str( hist.Integral() )
    #print " histskim->Integral()  " + str( histskim.Integral() )
    values = [hist.Integral(), histskim.Integral() ]

    _fileskim.Close()
    _file.Close()
    return values


def CreatMCFileLumi(alias_list_,y,skim):
    dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/GetEffLumi/"+y+"/"
    listpath="MCGetEffLumi_"+y + "_"+skim+".txt"
    list_ = open (listpath,"w")
    jobs_run=False

    for x in alias_list_:
        if not y in x:
            continue
        _x = x.replace("_"+y,'')
        if not os.path.exists(dirpath+"/GetEffLumi_"+_x+".root"):
            print "Missing " +  dirpath+"/GetEffLumi_"+_x+".root will run"
            list_.write(_x+"\n")
            jobs_run=True

    list_.close()
    if jobs_run:
        return listpath
    else:
        os.system("rm " + listpath)
        return "NONE"

def RunCheckWeight(alias_list, skim,y ):

    inputlist= CreatMCFileLumi(alias_list,y,skim)
    dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/GetEffLumi/"+y+"/"
    test=False
    if not test:
        if os.path.exists(inputlist):
            os.system("SKFlat.py -a GetEffLumi -l " + inputlist + " -n 50  --nmax 100  -y "+y )
    if inputlist != "NONE":
        os.system("rm " + inputlist)
        

def CreatMCFile(alias_list_,y,skim, analyzer):
    dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/"+analyzer+"/"+y+"/"

    jobs_run=False
    listpath_all="/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/run/lists/MC_"+y + "_"+skim+".txt"
    if skim == "":
        listpath_all="/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/run/lists/MC_"+y + ".txt"

    listpath="MCCheckSkim_"+y + "_"+skim+".txt"
    list_ = open (listpath,"w")
    listall_ = open (listpath_all,"w")
    for x in alias_list_:
        if not y in x:
            continue
        _x = x.replace("_"+y,'')
        listall_.write(_x+"\n")
        if skim== "":
            if not os.path.exists(dirpath+"/CheckSkim_"+_x+".root"):
                #print "Missing " +  dirpath+"/CheckSkim_"+_x+".root will run"
                list_.write(_x+"\n")
                jobs_run=True
        else:
            if not os.path.exists(dirpath+"/CheckSkim_"+skim+"_"+_x+".root"):
                jobs_run=True
                list_.write(_x+"\n")
                #print "Missing " +  dirpath+"/CheckSkim_"+skim+"_"+_x+".root will run"
    list_.close()
    listall_.close()
    if jobs_run:
        return listpath
    else:
        os.system("rm " + listpath )
        return "NONE"



def RunCheckSumList(inputlist , inputlist_skim, skim, y):
    
    if inputlist_skim != "NONE" and os.path.exists(inputlist_skim):
        if inputlist != "NONE" and os.path.exists(inputlist):
            #print "SKFlat.py -a CheckSkim -l " + inputlist_skim + "  --skim " + skim + " -n 30  --nmax 100  -y "+y   + " &"
            os.system("SKFlat.py -a CheckSkim -l " + inputlist_skim + "  --skim " + skim + " -n 30  --nmax 100  -y "+y   + " &")

                        
        else:
            #print "SKFlat.py -a CheckSkim -l " + inputlist_skim + "  --skim " + skim + " -n 30  --nmax 100  -y "+y
            os.system("SKFlat.py -a CheckSkim -l " + inputlist_skim + "  --skim " + skim + " -n 30  --nmax 100  -y "+y  )

    if inputlist != "NONE" and os.path.exists(inputlist):
        #print "SKFlat.py -a CheckSkim -l " + inputlist + " -n 60  --nmax 100  -y "+y
        os.system("SKFlat.py -a CheckSkim -l " + inputlist + " -n 60  --nmax 100  -y "+y )
    if inputlist != "NONE":
        os.system("rm " + inputlist)
        
    if inputlist_skim != "NONE":
        os.system("rm " + inputlist_skim)
    return

def RemoveJobFile(filepath, alias, skim,y):
    dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/GetEffLumi/"+y+"/"
    if os.path.exists(dirpath+"/CheckSkim_"+skim+"_"+alias+".root"):
        os.system("rm " + dirpath+"/CheckSkim_"+skim+"_"+alias+".root")
    if not os.path.exists(dirpath+"/CheckSkim_"+alias+".root"):
        os.system("rm " + dirpath+"/CheckSkim_"+alias+".root")
    if not os.path.exists(dirpath+"/GetEffLumi_"+_x+".root"):
        os.system("rm " + dirpath+"/GetEffLumi_"+_x+".root")
    return

def RunCheckWSum(filepath, alias,y ):

    if not os.path.exists(filepath):
        return  -999
    dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/GetEffLumi/"+y+"/"

    val = CheckEventCountLumi(dirpath, alias)

    return val

def RunCheckSum(filepath, alias, skim,y ):

    if not os.path.exists(filepath):
        return [0.,0.]
    dirpath="/data6/Users/jalmond/SKFlatOutput//Run2Legacy_v4/CheckSkim/"+y+"/"
    
    test=False
    if not test:
        if not os.path.exists(dirpath+"/CheckSkim_"+skim+"_"+alias+".root"):
            os.system("SKFlat.py -a CheckSkim -i " + alias + "  --skim " + skim + " -n 30  --nmax 100  -y "+y   + " &")
        if not os.path.exists(dirpath+"/CheckSkim_"+alias+".root"):
            os.system("SKFlat.py -a CheckSkim -i " + alias + " -n 60  --nmax 100  -y "+y )

    vals = CheckEventCount(dirpath, alias, skim)
    return vals
           
     
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
            if _file:
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

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-y", "--year"  , dest="year", default="AA",help="which year to run")
parser.add_option("-s", "--sample", dest="sample" , default="AA", help="which sample")

(options, args) = parser.parse_args()
in_samples=options.sample
in_year=options.year

years = ["2016", "2017","2018"]
if not  in_year == "AA":
    years = [in_year]
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

_al_subalias=[]
_al_sample=[]
_al_alias=[]
_al_alias_found=[]
_al_xsec=[]
_al_sumw=[]
_AliasFound=True
for y in years:
    if VERBOSE:
        print "#"*30
        print "Year: " + y

        
    skim_tmp="_"+skim
    if not skim:
        skim_tmp=skim

    for f in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/"):
        
        if not in_samples == "AA":
            if not  in_samples in f:
                continue


        alias=""
        found=False
        for al in listdir("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/"):
            if not ".txt" in al:
                continue
            if found:
                continue;
            commonfile = open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/"+al, "r")
            for x in commonfile:
                if f in x:
                    sx=x.split()
                    alias=sx[0]
                    _al_xsec.append(sx[2])
                    _al_sumw.append(sx[4])
                    found=True
            commonfile.close()

        if alias == "":
            _al_sample.append(f)
            _al_alias_found.append(False)
            _al_alias.append("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/")
            _al_subalias.append()
            _AliasFound=False
        else:
            _al_sample.append(f)
            _al_alias_found.append(True)
            _al_alias.append("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+y+"/Sample/CommonSampleInfo/"+alias+".txt")
            _al_subalias.append(alias)

if not _AliasFound:
    years=[]
    print "File to run sample is missing..."
    for i in range(0, len(_al_sample)):
        #if _al_alias_found[i]:
         #   print _al_sample[i] + "  file path " + _al_alias[i]
        #else:

        if not _al_alias_found[i]:
            print _al_sample[i] + "  missing common file in "   + _al_alias[i] 

if len(years) > 0:
    
    _sample_space=70
    _alias_space=30
    _xsec_space=10
    _weight_space=10
    output_log= "Sample name"+" "*(_sample_space-11) +"Alias"+" "*(_alias_space-5)+" xsec   sumw \n"

    xsec_2016_name="xsec_file2016.txt"
    xsec_2017_name="xsec_file2017.txt"
    xsec_2018_name="xsec_file2018.txt"
    if not in_samples == "AA":
        xsec_2016_name="xsec/xsec_file2016_"+in_samples+".txt"
        xsec_2017_name="xsec/xsec_file2017_"+in_samples+".txt"
        xsec_2018_name="xsec/xsec_file2018_"+in_samples+".txt"
    else:
        xsec_2016_name="xsec/xsec_file2016.txt"
        xsec_2017_name="xsec/xsec_file2017.txt"
        xsec_2018_name="xsec/xsec_file2018.txt"

    output_s16 = open(xsec_2016_name,"w")
    output_s16.write(output_log)
    output_s16.close()
    output_s17 = open(xsec_2017_name,"w")
    output_s17.write(output_log)
    output_s17.close()
    output_s18 = open(xsec_2018_name,"w")
    output_s18.write(output_log)
    output_s18.close()
    
    _xsec_warnings=[]
    for i in range(0, len(_al_sample)):
        lumi=36.47*1000
        _y = "2016"
        if "2017" in _al_alias[i]:
            _y ="2017"
            lumi=41.54*1000

        if "2018" in _al_alias[i]:
            _y ="2018"  
            lumi=59.96*1000

        if _al_xsec[i] == "1." or _al_xsec[i] == "1":
            _xsec_warnings.append( "WARNING xsec not set " + _y + " " + "Check xsec: " + _al_sample[i]  + " "*(_sample_space - len(_al_sample[i])) + _al_subalias[i] + " "* (_alias_space- len(_al_subalias[i])) + str(_al_xsec[i]))

        output_s = open("xsec_file"+_y+".txt","a")
        _w = lumi * float(_al_xsec[i]) / float(_al_sumw[i])
        if _y == "2016":
            output_s16 = open(xsec_2016_name,"a")
            output_s16.write(_al_sample[i]  + " "*(_sample_space - len(_al_sample[i])) + _al_subalias[i] + " "* (_alias_space- len(_al_subalias[i])) + str(_al_xsec[i]) + "  (%.4f)" %(_w)+ "\n")      
            output_s16.close()
        if _y == "2017":
            output_s17 = open(xsec_2017_name,"a")
            output_s17.write(_al_sample[i]  + " "*(_sample_space - len(_al_sample[i])) + _al_subalias[i] + " "* (_alias_space- len(_al_subalias[i])) + str(_al_xsec[i]) + "  (%.4f)" %(_w)+ "\n")      
            output_s17.close()
        if _y == "2018":
            output_s18 = open(xsec_2018_name,"a")
            output_s18.write(_al_sample[i]  + " "*(_sample_space - len(_al_sample[i])) + _al_subalias[i] + " "* (_alias_space- len(_al_subalias[i])) + str(_al_xsec[i]) + "  (%.4f)" %(_w)+ "\n")
            output_s18.close()
        
    for x in _xsec_warnings:
        print x
    
    for _x in years:
        if not in_samples == "AA":
            print "Xsec for " + _x + " printed in xsec_file"+y+"_"+in_samples +".txt"

    if len(_xsec_warnings) > 0:
        years=[]


output_line="_"*len(output_log)
           
             

for y in years:
    if VERBOSE:
        print "#"*30
        print "Year: " + y

    for dtag in datetag:

        skim_tmp="_"+skim
        if not skim:
            skim_tmp=skim

        for f in listdir("/gv0/DATA/SKFlat/"+tag+"/"+y+"/MC"+skim_tmp+"/"):
            if not in_samples == "AA":
                if not  in_samples in f:
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

print  "CheckSkims. Setup finshed" 
multi_file=[]
out_skim=""
if len(years) > 0:
    out_skim= "Sample list for skim " + skim
print "#"*len(out_skim)
print out_skim
print "#"*len(out_skim)
print "-"*len(out_skim)
sample_space=50
alias_space=30
prod_space= 20
nfile_space=7
nev_space=25
size_space=20
input_space=17
dirlist=[]
_ndirlist=[]

print_log=[]
for i in range(0, len(_dir)):
    if not _dir[i] in dirlist:
        dirlist.append(_dir[i])
        _ndirlist.append(len(dirlist))
        print "["+str(len(dirlist)) + "]" + _dir[i]

output_log=""
if len(years) >0:
    output_log= "Sample name"+" "*(sample_space-11) +"Alias"+" "*(alias_space-5)+"Production Tag"+" "*(prod_space-14)+"nFiles"+" "*(nfile_space-6)+"Nevent    (NOSKIM)"+" "*(nev_space-19)+"skim_factor(0)%"+" "*(size_space-14) + "ForSNU/Input.txt" +" "*(input_space-16) + "#BadFiles    Valid_CheckSkim   "
    output_log2=" "*(sample_space+alias_space+prod_space+nfile_space+nev_space+size_space+input_space+13) + "DIFF/NOSKIM   SKIM "
output_line="_"*len(output_log)
print_log.append(output_line)
print_log.append(output_log)
print_log.append(output_log2)
print_log.append(output_line)
print output_line
print output_log
print output_log2
print output_line

alias_list_run=[]

_ranbatch=False
_poss_runBATCH=True
count=0
while count < len(_samples):
    print str(count) + " : " + _samples[count]
    _duplicate_productions=False

    # if no alias file exists just output basic info

    if not _poss_runBATCH:
        if not _ranbatch:
            # when looping over main loop for first time run batch jobs that need processing to create root files for later checks
            _ranbatch=True
            # creat input mc files from available lists. checking file does not exist

            for _y in years:
                path_mc=CreatMCFile(alias_list_run,_y,"","CheckSkim" )
                path_mc_skim=CreatMCFile(alias_list_run,_y,skim,"CheckSkim" )
                RunCheckWeight(alias_list_run, skim, _y)
                if not (path_mc == "NONE" and path_mc_skim == "NONE"):
                    RunCheckSumList(path_mc , path_mc_skim, skim, _y)

    skim_def=skim+"_"
    if not skim:
        skim_def=skim
    if not os.path.exists("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt"):

        # if _poss_runBATCH then only checking batch jobs and not outputting info so can continue when no alias part
        if _poss_runBATCH:
            count=count+1
            if count == len(_samples):
                # reset loop now all alias are checked
                count=0
                _poss_runBATCH=False
            
            continue
        else:
            # not running batch jobs so can just output sampe details
            
            sample_outfix = len(_samples[count].replace(_dir[count],'')) - sample_space +8
            sample_fix=""
            if len(_samples[count].replace(_dir[count],'')) > sample_space -8 :
                sample_fix="..."
                alias_fix=""
            if len(_alias[count].replace(_samples[count],'')) > alias_space -8:
                alias_fix="..."
            sampletag= GetDirName(_dir[count], dirlist,_ndirlist) + _samples[count].replace(_dir[count],'')[:- sample_outfix] + sample_fix
            alias_outfix = len(_alias[count].replace(_samples[count],'')) - alias_space +8

            aliastag= _alias[count].replace(_samples[count],'')[:- alias_outfix] +alias_fix
            print sampletag + " "*(sample_space-len(sampletag)) + aliastag + " "*  (alias_space-len(aliastag)) + _date_tags[count].replace(_samples[count],'')  + " "*(prod_space-len(_date_tags[count].replace(_samples[count],'')))   + str(_nFiles[count]).replace(_samples[count],'')  + " "*(nfile_space - len(str(_nFiles[count]).replace(_samples[count],'')))   + "--"  +" "*(10-len("--" ))   + "--" + " "*( nev_space -10 - len("--")) + "--"  + " "*( size_space - 2)   + "--"  + " "*( input_space - 2)   + "--" + "           __"
            print_log.append(sampletag + " "*(sample_space-len(sampletag)) + aliastag + " "*  (alias_space-len(aliastag)) + _date_tags[count].replace(_samples[count],'')  + " "*(prod_space-len(_date_tags[count].replace(_samples[count],'')))   + str(_nFiles[count]).replace(_samples[count],'')  + " "*(nfile_space - len(str(_nFiles[count]).replace(_samples[count],'')))   + "--"  +" "*(10-len("--" ))   + "--" + " "*( nev_space -10 - len("--")) + "--"  + " "*( size_space - 2)   + "--"  + " "*( input_space - 2)   + "--" + "           __")

            #end of no alias loop move to next sample
            count = count+1
            continue

    # multi_file used to save time, only process samples used in ForSNU/Input.txt
    if (not _samples[count] in multi_file):
        skim_def=skim+"_"
        if not skim:
            skim_def=skim
 
        # check if current sample is used in ForSNU/Input.txt
        used_sample=IsUsed("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt", _date_tags[count].replace(_samples[count],''))

        if _poss_runBATCH:
            sample_list_file=CheckMyFile(1, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt", _date_tags[count].replace(_samples[count],''),  str(_nFiles[count]).replace(_samples[count],''))

            nFileDiff = CheckMyFile(0, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt", _date_tags[count].replace(_samples[count],''),  str(_nFiles[count]).replace(_samples[count],''))
            if nFileDiff != 0 :
                print "Discrepency in number of files " + str(nFileDiff) + " " + str(_nFiles[count]) + " in " + "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+alias+".txt with those in " + _date_tags[count]
                os.system("ls " + _samples[count] + "/"+_date_tags[count].replace(_samples[count],'') + "/* >  /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")
                print "Updated file /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt"

                RemoveJobFile("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''), skim, _year[count])
                continue

            for sl in sample_list_file:
                print "Error in file " + sl
                os.system( "rm " +  sl)
                print "removing " + sl
            if len(sample_list_file) > 0:
                os.system("ls " + _samples[count] + "/"+_date_tags[count].replace(_samples[count],'') + "/* >  /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")

                print "Updated file /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt"
                RemoveJobFile("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''), skim, _year[count])
                continue


            # after chekced rot files and lists are ok add alias to main list

            if used_sample == "Yes":
                # fill alias_list_run of files that are to be processed
                alias_list_run.append(_alias[count].replace(_samples[count],'')+"_"+_year[count])
            count = count+1
            if count == len(_samples):
                # reset loop now all alias are checked                                                                                       
                count=0
                _poss_runBATCH=False
                
            continue

        # running main function now
        ev_Vals=[]
        sum_w=0
        matched_sumw=True
        if used_sample == "Yes":
            ev_Vals=RunCheckSum("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''), skim, _year[count])
	    sum_w= RunCheckWSum("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''),  _year[count])
        else:
            ev_Vals= [0.,0.]
    

        commonfile = open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+_year[count]+"/Sample/CommonSampleInfo/"+_alias[count].replace(_samples[count],'')+".txt" , "r")
        for xc in commonfile:
            if _samples[count].replace(_dir[count],'') in xc:
                sx=xc.split()
                if float(sx[4] )!= (sum_w):
                    matched_sumw=False
                break
            
        commonfile.close()

            
        nevents = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")
        nevents_orig = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+_alias[count].replace(_samples[count],'')+".txt")
        skim_size = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")
        orig_size  = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+_alias[count].replace(_samples[count],'')+".txt")

        ev_log= str(ev_Vals[0] - ev_Vals[1])
        if ev_Vals[0] != ev_Vals[1]:
            ev_log=str(ev_Vals[0]) + " :  " + str(ev_Vals[1])

        if not matched_sumw:
            sample_list_file=[ "-999"]

        neventR=float(nevents)/float(nevents_orig)

        sample_outfix = len(_samples[count].replace(_dir[count],'')) - sample_space +8
        sample_fix=""
        if len(_samples[count].replace(_dir[count],'')) > sample_space:
            sample_fix="..."
        alias_fix=""
        if len(_alias[count].replace(_samples[count],'')) > alias_space:
            alias_fix="..."
        sampletag= GetDirName(_dir[count], dirlist,_ndirlist) + _samples[count].replace(_dir[count],'')[:- sample_outfix] + sample_fix
        alias_outfix = len(_alias[count].replace(_samples[count],'')) - alias_space +8

        aliastag= _alias[count].replace(_samples[count],'')[:- alias_outfix] +alias_fix
        print sampletag + " "*(sample_space-len(sampletag)) + aliastag + " "*  (alias_space-len(aliastag)) + _date_tags[count].replace(_samples[count],'') + " "*(prod_space-len(_date_tags[count].replace(_samples[count],''))) + str(_nFiles[count]).replace(_samples[count],'') + " "*(nfile_space - len(str(_nFiles[count]).replace(_samples[count],'')))  + str(nevents) +" "*(10-len(str(nevents) )) + "(%.0f)" %(nevents_orig) + " "*( nev_space -10 - len("(%.0f)" %(nevents_orig))) + "%.4f" %(100*skim_size/orig_size) + " "*( size_space - len("%.4f" %(100*skim_size/orig_size))) + used_sample  + " "*( input_space - len(used_sample)) + str(len(sample_list_file)) + " "*12  + ev_log
        print_log.append(sampletag + " "*(sample_space-len(sampletag)) + aliastag + " "*  (alias_space-len(aliastag)) + _date_tags[count].replace(_samples[count],'') + " "*(prod_space-len(_date_tags[count].replace(_samples[count],''))) + str(_nFiles[count]).replace(_samples[count],'') + " "*(nfile_space - len(str(_nFiles[count]).replace(_samples[count],'')))  + str(nevents) +" "*(10-len(str(nevents) )) + "(%.0f)" %(nevents_orig) + " "*( nev_space -10 - len("(%.0f)" %(nevents_orig))) + "%.4f" %(100*skim_size/orig_size) + " "*( size_space - len("%.4f" %(100*skim_size/orig_size))) + used_sample  + " "*( input_space - len(used_sample)) + str(len(sample_list_file)) + " "*12  + ev_log)
        
        multi_file.append(_samples[count])
    else:
        
        # another skim sample found, process less info as it is duplicated 
        used_sample=IsUsed("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt", _date_tags[count].replace(_samples[count],''))
        
        if _poss_runBATCH:


            sample_list_file=CheckMyFile(1, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt", _date_tags[count].replace(_samples[count],''),  str(_nFiles[count]).replace(_samples[count],''))
            nFileDiff = CheckMyFile(0, "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt", _date_tags[count].replace(_samples[count],''),  str(_nFiles[count]).replace(_samples[count],''))

            if nFileDiff !=0:
                print "Discrepency in number of files in " + "/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt with those in "+ _date_tags[count]
                os.system("ls " + _samples[count] + "/"+_date_tags[count].replace(_samples[count],'') + "/* >  /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")
                print "Updated file /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt"
                RemoveJobFile("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''), skim, _year[count])
                continue

            for sl in sample_list_file:
                print "Error in file " + sl
                os.system( "rm " +  sl)
                print "removing " + sl
            if len(sample_list_file) > 0:
                os.system("ls " + _samples[count] + "/"+_date_tags[count].replace(_samples[count],'') + "/* >  /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")
                print "Updated file /data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt"
                RemoveJobFile("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''), skim, _year[count])
                continue

            if used_sample == "Yes":
                # fill alias_list_run of files that are to be processed                                                                      
                alias_list_run.append(_alias[count].replace(_samples[count],'')+"_"+_year[count])
            count = count+1
            if count == len(_samples):
                # reset loop now all alias are checked                                                                                       
                count=0
                _poss_runBATCH=False
                
            continue



        ev_Vals=[]
        sum_w=0
        matched_sumw=True

        if used_sample == "Yes":
            ev_Vals=RunCheckSum("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''), skim, _year[count])
            sum_w= RunCheckWSum("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt" , _alias[count].replace(_samples[count],''), _year[count])
        else:
            ev_Vals= [0.,0.]

        commonfile = open("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/"+tag+"/"+_year[count]+"/Sample/CommonSampleInfo/"+_alias[count].replace(_samples[count],'')+".txt" , "r")
        for xc in commonfile:
            if _samples[count].replace(_dir[count],'') in xc:
                sx=xc.split()
                if float(sx[4]) != sum_w:
                    matched_sumw=False
                break

        commonfile.close()

        if not matched_sumw:
            sample_list_file=[ "-999"]

        nevents = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")
        nevents_orig = NEvents("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+_alias[count].replace(_samples[count],'')+".txt")
        skim_size = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+skim_def+_alias[count].replace(_samples[count],'')+".txt")
        orig_size  = GetSize("/data6/Users/jalmond/2020/HL_SKFlatAnalyzer/SKFlatAnalyzer/data/Run2Legacy_v4/"+_year[count]+"/Sample/ForSNU/"+_alias[count].replace(_samples[count],'')+".txt")

        neventR=float(nevents)/float(nevents_orig)

        ev_log= str(ev_Vals[0] - ev_Vals[1])
        if ev_Vals[0] != ev_Vals[1]:
            ev_log=str(ev_Vals[0]) + " :  " + str(ev_Vals[1])

        print " "*sample_space  + " "*  (alias_space)    + _date_tags[count].replace(_samples[count],'')  + " "*(prod_space-len(_date_tags[count].replace(_samples[count],'')))    + str(_nFiles[count]).replace(_samples[count],'')    + " "*(nfile_space - len(str(_nFiles[count]).replace(_samples[count],'')))   + str(nevents)  +" "*(10-len(str(nevents) ))   + "(%.0f)" %(nevents_orig)   + " "*( nev_space -10 - len("(%.0f)" %(nevents_orig)))  + "%.4f" %(100*skim_size/orig_size)   + " "*( size_space - len("%.4f" %(100*skim_size/orig_size)))    + used_sample    + " "*( input_space - len(used_sample))   + str(len(sample_list_file))  + " "*12 + ev_log
        print_log.append(" "*sample_space + _alias[count].replace(_samples[count],'')   + " "*  (alias_space-len(_alias[count].replace(_samples[count],'')))    + _date_tags[count].replace(_samples[count],'')  + " "*(prod_space-len(_date_tags[count].replace(_samples[count],'')))    + str(_nFiles[count]).replace(_samples[count],'')    + " "*(nfile_space - len(str(_nFiles[count]).replace(_samples[count],'')))   + str(nevents)  +" "*(10-len(str(nevents) ))   + "(%.0f)" %(nevents_orig)   + " "*( nev_space -10 - len("(%.0f)" %(nevents_orig)))  + "%.4f" %(100*skim_size/orig_size)   + " "*( size_space - len("%.4f" %(skim_size/orig_size)))    + used_sample    + " "*( input_space - len(used_sample))   + str(len(sample_list_file))  + " "*12 + ev_log)
    
    count = count + 1


outname="log/output"+skim+".txt"
if not in_samples == "AA":
    outname="log/output_"+in_samples+"_"+skim+".txt"
    if not in_year == "AA":
        outname="log/output_"+in_samples+"_"+in_year+"_"+skim+".txt"
else:
    if not in_year == "AA":
        outname="log/output_"+ in_year+"_"+skim+".txt"


out_logFinal = open(outname,"w")
for x in range(0,5):
    print " "
for x in print_log:
    print x
    out_logFinal.write(x +"\n")
out_logFinal.close()

