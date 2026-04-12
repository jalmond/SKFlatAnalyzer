#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from tqdm import tqdm
import ctypes
import time
import argparse

ROOT.gROOT.SetBatch(True)

#### Read config
from config import ERAS, FLAVOURS, FAKE_FLOOR, USE_FAKE_FIX, MASSES, RUN_REF,RUN_SCANS,NBINS_TO_SCAN
#### Build data
from data_format import hist_to_array,bins_to_array_with_err,build_data

from helper import  debug_data_summary,get_latest_dir

from plotter import make_mass_plot_multi,convert_results_for_plot

from ref_fom_utils import evaluate_ref_bins_no_fakecorr,evaluate_ref_bins_fakecorr,compare_fake_impact

from fom_utils import *

# =========================================================
# LOGGER
# =========================================================                                                                                                                            

class TeeLogger:
    def __init__(self, filename):
        self.file = open(filename, "w")
        self.stdout = sys.stdout

    def write(self, message):
        self.stdout.write(message)
        self.file.write(message)

    def flush(self):
        self.stdout.flush()
        self.file.flush()

from logger import print_final_summary
        

def main():

    base = "/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"

    # ----------------------------------
    # Use latest production directory
    # ----------------------------------
    base = get_latest_dir(base)

    parser = argparse.ArgumentParser(description="FOM scan runner")
    parser.add_argument("--runRef", action="store_true", default=None)
    parser.add_argument("--runDP",  action="store_true", default=None)
    args = parser.parse_args()

    run_ref = args.runRef if args.runRef is not None else RUN_REF
    run_dp  = args.runDP  if args.runDP  is not None else RUN_SCANS

    print("==============================")
    if run_ref:
        print("Plot Reference bins in ref_bins.py")
        
    if run_dp:
        print("Run Scans")
    print("==============================")
        
    # ----------------------------------
    # Setup logging
    # ----------------------------------
    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs", exist_ok=True)

    log_file = f"logs/build_data_{ts}.txt"
    sys.stdout = TeeLogger(log_file)

    print("[INFO] Log file:", log_file)
    print("[INFO] Using input directory:", base)

    # ----------------------------------
    # Mass list
    # ----------------------------------
    print("\n[INFO] Masses:", MASSES)

    # ----------------------------------
    # Build data
    # ----------------------------------
    print("\n[STEP] Building data...")
    data = build_data(base, MASSES)
    debug_data_summary(data,5)
    
    # ----------------------------------
    # Norm check
    # ----------------------------------
    print("\n[NORM VALUES]")
    for m in MASSES:
        print(f"  {m} -> {data['norm'][m]:.6f}")



    print("\n==============================")
    print(" DONE")
    print("==============================")

    # Make scans and save results for plots
    results_for_plots = []
    scan_resuls_for_plots= []
    if run_ref:
        print("==============================")
        print("Plot Reference bins in ref_bins.py")
        print("==============================")
        
        print("\n[STEP] Evaluating REF bins...")
        
        res_no_raw = evaluate_ref_bins_no_fakecorr(data)
        res_fx_raw = evaluate_ref_bins_fakecorr(data)
        
        compare_fake_impact(res_no_raw, res_fx_raw)
        
        res_no_run2  = convert_results_for_plot(res_no_raw, mode="run2")
        #res_fx_run2  = convert_results_for_plot(res_fx_raw, mode="run2")
        #res_no_quad  = convert_results_for_plot(res_no_raw, mode="quad")
        #res_fx_quad  = convert_results_for_plot(res_fx_raw, mode="quad")
        
        for flav in res_no_run2:
            print(flav, res_no_run2[flav][:3])
            
            results_for_plots.append({
                "results": res_no_run2,
                "label": "No Fake (Run2)",
            })
            
            results_for_plots.append({
                "results": convert_results_for_plot(res_fx_raw, mode="run2"),
                "label": "Fake Corr (Run2)",
            })
            
            results_for_plots.append({
            "results": convert_results_for_plot(res_no_raw, mode="quad"),
                "label": "No Fake (Quad)",
            })
            
            results_for_plots.append({
                "results": convert_results_for_plot(res_fx_raw, mode="quad"),
                "label": "Fake Corr (Quad)",
            })
            scan_resuls_for_plots.append({
                "results": convert_results_for_plot(res_fx_raw, mode="run2"),
                "label": "Fake Corr (Run2)",
            })
            
            
            for flav in res_no_run2:

                print("\n==============================")
                print(f"Plotting flavour: {flav}")
                print("==============================")
                
                make_mass_plot_multi(
                    results_list=results_for_plots,
                    flav=flav,
                    out_tag="fake_comparison"
                )


    if run_dp:


        scan_results_for_plots_perN = []
        for nb in NBINS_TO_SCAN:

            print("\n======================================")
            print(f" Running DP scan for nBins = {nb}")
            print("======================================")
            
            res_dp_raw = evaluate_dp_per_mass(data, nb)
            
            print_final_summary(res_dp_raw)

            scan_results_for_plots_perN.append({
                "results": convert_results_for_plot(res_dp_raw, mode="run2"),
                "label": f"DP (Run2, nBins={nb})"
            })

            if nb == NBINS_TO_SCAN[-1]:
                
                scan_results_for_plots.append({
                    "results": convert_results_for_plot(res_dp_raw, mode="run2"),
                    "label": f"DP (Run2, nBins={nb})"
                })
                
        
        for flav in res_dp_raw:
            
            print("\n==============================")
            print(f"Plotting flavour: {flav}")
            print("==============================")
            make_mass_plot_multi(
                results_list=scan_resuls_for_plots,
                flav=flav,
                out_tag="scan_results"
            )
            
            make_mass_plot_multi(
                results_list=scan_resuls_for_plots_perN,
                flav=flav,
                out_tag="scan_results_Nbins"
            )
            
    
        
if __name__ == "__main__":
    main()
