#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from multiprocessing import Pool, cpu_count
from tqdm import tqdm

from loader import load_flavour
from cacher import build_sig_cache
from helper import *
from loader import load_flavour,load_fake,load_signals
from plotter import make_lastbin_zoom_plot,make_plot_v2
from ref_fom_utils import scan_predefined_verbose,scan_predefined_binnings
from fom_utils import *
from data_format import build_data

ROOT.gROOT.SetBatch(True)

RED   = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"


# =========================================================
# CONFIG
# =========================================================
from config import STAT_THRESHOLD_PERERA,STAT_THRESHOLD_RUN2,ERAS,FLAVOURS,USE_TRUE_RUN2_FOM,USE_FAKE_FIX,FAKE_FLOOR,OPT_MASSES,EVAL_MASSES

# =========================================================
# LOGGER
# =========================================================
class TeeLogger:
    def __init__(self,f):
        self.file=open(f,"w")
        self.stdout=sys.stdout
    def write(self,m):
        self.stdout.write(m)
        self.file.write(m)
    def flush(self):
        self.stdout.flush()
        self.file.flush()



# =========================================================
# MAIN
# =========================================================
def main():

    global OPT_MASSES,EVAL_MASSES

    parser=argparse.ArgumentParser()
    parser.add_argument('--opt_weinberg',action='store_true')
    parser.add_argument('--opt_ssww',action='store_true')
    parser.add_argument('--weinberg',action='store_true')
    parser.add_argument('--true-run2-fom', action='store_true')
    args=parser.parse_args()

    global USE_TRUE_RUN2_FOM
    USE_TRUE_RUN2_FOM = args.true_run2_fom
    
    base="/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"

    ts=datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs",exist_ok=True)
    sys.stdout=TeeLogger(f"logs/scan_{ts}.txt")

    data = build_data(base, OPT_MASSES)
   
       
    # =========================================================
    # GLOBAL COMBINED SCAN (ONCE)
    # =========================================================
    best_combined_run = (-1, None, None)

    combined_results_per_n = {}

    for n in n_bin_torun:

        # ----------------------------------
        #  scan for best binning 
        # ----------------------------------
        res_combined_global_scan_run = evaluate_combined_global_scan_run2(
            data,
            n,
            OPT_MASSES,
            use_fake_corr=USE_FAKE_FIX,
            run_z_no_unc=RUN_Z_NO_UNC
        )
        print_final_summary(res_combined_global_scan_run)

    for n in n_bin_torun:
              
        
        res_evaluate_per_flavour_scan_run2=evaluate_per_flavour_scan_run2( data,
                                                                           n,
                                                                           OPT_MASSES,
                                                                           use_fake_corr=USE_FAKE_FIX,
                                                                           run_z_no_unc=RUN_Z_NO_UNC)
        
        print_final_summary(res_evaluate_per_flavour_scan_run2)
        
if __name__=="__main__":
    main()
