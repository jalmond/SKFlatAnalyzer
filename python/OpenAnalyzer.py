pathAnaDir='/data6/Users/jalmond/2020/HL_SKFlatAnalyzer_UL_LONG/SKFlatAnalyzer/Analyzers/HNL/src/'

import os

from os import listdir
from os.path import isfile,isdir, join

macro_list  = [f for f in listdir(pathAnaDir) if isfile(join(pathAnaDir,f))]
for _macro in macro_list:
    if '.C' in _macro:
        if not 'C~' in _macro:
            print("emacs -nw  "+pathAnaDir+ _macro)

