#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from tqdm import tqdm
import ctypes
import time

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

from logger import print_final_summary,print_scan_summary_table,print_scan_binning_table
        

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

    # ----------------------------------
    # Setup logging
    # ----------------------------------
    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs", exist_ok=True)

    log_file = f"logs/build_data_{ts}.txt"
    sys.stdout = TeeLogger(log_file)

    print("==============================")
    if run_ref:
        print("Plot Reference bins in ref_bins.py")
    if run_dp:
        print("Run Scans")
    print("==============================")
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

    ########################################################################################
    ########################################################################################
    ########################################################################################
    ########################################################################################
    #### START SCANNING
    ########################################################################################
    ########################################################################################
    ########################################################################################

    # Make scans and save results for plots
    results_for_plots = []
    scan_results_for_plots= []
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
        
        
        results_for_plots.append({
            "results": res_no_run2,
            "raw": res_no_raw,  
            "label": "No Fake (Run2)",
        })
        
        results_for_plots.append({
            "results": convert_results_for_plot(res_fx_raw, mode="run2"),
            "raw": res_fx_raw,
            "label": "Fake Corr (Run2)",
        })
        
        results_for_plots.append({
            "results": convert_results_for_plot(res_no_raw, mode="quad"),
            "raw": res_no_raw,
            "label": "No Fake (Quad)",
        })
        
        results_for_plots.append({
            "results": convert_results_for_plot(res_fx_raw, mode="quad"),
            "raw": res_fx_raw,
            "label": "Fake Corr (Quad)",
        })

        scan_results_for_plots.append({
            "results": convert_results_for_plot(res_fx_raw, mode="run2"),
            "raw": res_fx_raw,
            "label": "ANv6 Mass Binning",
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


    scan_results_for_plots_perNB = []
    scan_results_for_plots_globalflav_perNB = []
    scan_results_for_plots_globalsig_perNB = []
    scan_results_for_plots_pererabinning_perNB= []
    for nb in NBINS_TO_SCAN:

        print("\n======================================")
        print(f" Running DP scan for nBins = {nb}")
        print("======================================")
        print(f" Running DP scan Per Flavour, Using Run2 shape, per signal mass")
        
        res_dp_raw = evaluate_dp_per_mass(data, nb)
        
        print_final_summary(res_dp_raw)
        
        scan_results_for_plots_perNB.append({
            "results": convert_results_for_plot(res_dp_raw, mode="run2"),
            "raw": res_dp_raw,
            "label": f"Scan (Run2, N_bins={nb})"
        })
        
        if nb == NBINS_TO_SCAN[-1]:
            
            scan_results_for_plots.append({
                "results": convert_results_for_plot(res_dp_raw, mode="run2"),
                "raw": res_dp_raw,
                "label": f"Scan (Run2, N_bins={nb})"
            })


    for nb in NBINS_TO_SCAN:
        print("\n======================================")
        print(f" Running DP scan for nBins = {nb}")
        print("======================================")
        print(f" Running DP scan Per Flavour, Using Run2 shape, combined signal mass")
        
        res_dp_raw_global_sig =  evaluate_dp_global_per_flavour(data, nb)
        print_final_summary(res_dp_raw_global_sig)
        scan_results_for_plots_globalsig_perNB.append({
            "results": convert_results_for_plot(res_dp_raw_global_sig, mode="run2"),
            "raw": res_dp_raw_global_sig,                
	    "label": f"Scan (GlobSig, Run2, N_bins={nb})"
	})
        if nb == NBINS_TO_SCAN[-1]:
            
            scan_results_for_plots.append({
                "results": convert_results_for_plot(res_dp_raw_global_sig, mode="run2"),
                "raw": res_dp_raw_global_sig,
                "label": f"Scan (GlobSig, Run2, N_bins={nb})"
            })
            


    for nb in NBINS_TO_SCAN:

        print("======================================")
        print(f" Running DP scan Summed Flavour, Using Run2 shape, per signal mass")
        
        res_dp_raw_globalflav = evaluate_dp_combined_flavours(data, nb)
        
        print_final_summary(res_dp_raw_globalflav)
        
        scan_results_for_plots_globalflav_perNB.append({
            "results": convert_results_for_plot(res_dp_raw_globalflav, mode="run2"),
            "raw": res_dp_raw_globalflav,   
            "label": f"Scan (GlobalFlav, Run2, N_bins={nb})"
        })
        
        if nb == NBINS_TO_SCAN[-1]:
            
            scan_results_for_plots.append({
                "results": convert_results_for_plot(res_dp_raw_globalflav, mode="run2"),
                "raw": res_dp_raw_globalflav,                    
                "label": f"Scan (GlobalFlav, Run2, N_bins={nb})"
            })

    for nb in NBINS_TO_SCAN:
        print("\n======================================")
        print(f" Running DP scan per era binning, per  Flavour, Using Run2 shape, per signal mass")
        
        res_dp_raw_perera =  evaluate_dp_per_mass_per_flavour_per_era(data, nb)
        print_final_summary(res_dp_raw_perera)
        scan_results_for_plots_pererabinning_perNB.append({
            "results": convert_results_for_plot(res_dp_raw_perera, mode="run2"),
            "raw": res_dp_raw_perera,
            "label": f"Scan (Era-Dep, Run2, N_bins={nb})"
        })
        if nb == NBINS_TO_SCAN[-1]:
            scan_results_for_plots.append({
                "results": convert_results_for_plot(res_dp_raw_perera, mode="run2"),
                "raw": res_dp_raw_perera,
                "label": f"Scan (Era-Dep, Run2, N_bins={nb})"
            })

        
                
    if len(scan_results_for_plots) == 0:
        sys.exit()
        
    for flav in FLAVOURS:
        
        print("\n==============================")
        print(f"Plotting flavour: {flav}")
        print("==============================")
        make_mass_plot_multi(
            results_list=scan_results_for_plots,
            flav=flav,
            out_tag="scan_results"
        )
        if len(scan_results_for_plots_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_perNB,
                flav=flav,
                out_tag="scan_results_perflav_persigmass_Nbins"
            )
            
        if len(scan_results_for_plots_globalsig_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_globalsig_perNB,
                flav=flav,
                out_tag="scan_results_perflav_globalsigmass_Nbins"
            )

        if len(scan_results_for_plots_globalflav_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_globalflav_perNB,
                flav=flav,
                out_tag="scan_results_globalflav_persigmass_Nbins"
            )
        if len(scan_results_for_plots_pererabinning_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_pererabinning_perNB,
                flav=flav,
                out_tag="scan_results_perflav_persigmass_perera_Nbins"
            )
            
    print_scan_summary_table(scan_results_for_plots)
    print_scan_summary_table(scan_results_for_plots, use_quad=True)  # NEW
    print_scan_binning_table(scan_results_for_plots)
            
        
if __name__ == "__main__":
    main()
