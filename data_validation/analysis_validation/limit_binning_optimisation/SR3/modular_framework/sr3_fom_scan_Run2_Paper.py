#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from tqdm import tqdm
import ctypes
import time
import importlib
ROOT.gROOT.SetBatch(True)

from contextlib import contextmanager

@contextmanager
def redirect_stdout(target):
    old_stdout = sys.stdout
    sys.stdout = target
    try:
        yield
    finally:
        sys.stdout = old_stdout


#### Build data
from data_format import hist_to_array, bins_to_array_with_err, build_data_sr3
from helper import debug_data_summary, get_latest_dir,ReadConfig,ConvertConfPath,list_available_configs,print_sr3_bin_table
from plotter import make_mass_plot_multi, convert_results_for_plot,extract_fixed_met
from default_config import RUN_REF,RUN_SCANS,BASE_DIR,FLAVOURS,NCORE
import default_config

from ref_fom_utils import *
from fom_utils import *

# =========================================================
# TIMER HELPER
# =========================================================

class Timer:
    def __init__(self):
        self.times = {}

    def start(self, key):
        self.times[key] = {"start": time.time(), "elapsed": None}

    def stop(self, key):
        if key in self.times and self.times[key]["elapsed"] is None:
            self.times[key]["elapsed"] = time.time() - self.times[key]["start"]

    def summary(self):
        print("\n==============================")
        print(" TIMING SUMMARY")
        print("==============================")
        for k, v in self.times.items():
            if v["elapsed"] is not None:
                print(f"{k:45s} : {v['elapsed']:.2f} s")
        print("==============================\n")


# =========================================================
# LOGGER
# =========================================================
class SimpleLogger:
    def __init__(self, filename):
        self.file = open(filename, "w")

    def write(self, message):
        self.file.write(message)

    def flush(self):
        self.file.flush()
        
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


from logger import print_final_summary, print_scan_summary_table, print_scan_binning_table,print_config_file, print_scan_summary,print_scan_details 


# =========================================================
# MAIN
# =========================================================

def main():

    timer = Timer()
    timer.start("TOTAL")

    # ----------------------------------
    # Use latest production directory
    # ----------------------------------

    if BASE_DIR is not None:
        if not os.path.isdir(BASE_DIR):
            raise ValueError(f"[ERROR] BASE_DIR does not exist: {BASE_DIR}")

        base = BASE_DIR
        print(f"[INFO] Using BASE_DIR from config: {base}")
    else:
        base = get_latest_dir("/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter")
        print(f"[INFO] Using latest directory: {base}")

        
    parser = argparse.ArgumentParser(description="FOM scan runner")
    parser.add_argument("--runRef", action="store_true", default=None)
    parser.add_argument("--config", default=None)
    parser.add_argument("--tag", default=None, help="Tag string for this run")
    args = parser.parse_args()
    
    
    #### Read config
    if args.config is None:
        print("[ERROR] No config provided\n")
        list_available_configs("config")
        sys.exit(1)
        
    conf_path = ConvertConfPath(args.config)
    import importlib.util
    if args.config is None or importlib.util.find_spec(conf_path) is None:
        print(f"[ERROR] Config module '{conf_path}' not found or not provided")
    
        list_available_configs("config")
    
        sys.exit(1)
    
        
    config_module = importlib.import_module(conf_path)
    MASSES, USE_FAKE_FIX, RUN_Z_NO_UNC, LOG_TAG, TAG, NBinScan,RunGlobalSig = ReadConfig(config_module)

    import helper
    helper.set_stat_config(config_module)
    
    run_ref = args.runRef if args.runRef is not None else RUN_REF

    if args.tag is not None:
        tag = args.tag
    elif hasattr(config_module, "TAG"):
        tag = config_module.TAG
    else:
        tag = "Default"

    scan_type=""
    if NBinScan:
        scan_type="Nbin"

        
        
    # ----------------------------------
    # Setup logging
    # ----------------------------------
    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs", exist_ok=True)
    outtag=""

    os.makedirs(f"logs/{tag}", exist_ok=True)
    os.makedirs(f"logs/{tag}/{LOG_TAG}", exist_ok=True)
    outtag=f"{tag}/{LOG_TAG}"


    log_file = f"logs/{outtag}/build_data_{ts}.txt"
    sys.stdout = TeeLogger(log_file)
    
    print_config_file(default_config, title="DEFAULT CONFIG")
    print_config_file(config_module, title=f"USER CONFIG ({conf_path})")

    results_log_file = f"logs/{outtag}/results_{ts}.txt"
    results_logger = SimpleLogger(results_log_file)
    
    print("==============================")
    if run_ref:
        print("Plot Reference bins in ref_bins.py")

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
    timer.start("Build data")
    data = build_data_sr3(base, MASSES)
    timer.stop("Build data")

    debug_data_summary(data, 5)
       
    # ----------------------------------
    # Norm check
    # ----------------------------------
    print("\n[NORM VALUES PER REGION]")

    for met in data:
        for cat in data[met]:
            print(f"\n--- MET {met} | {cat} ---")
            for m in MASSES:
                print(f"  {m} -> {data[met][cat]['norm'][m]:.6f}")

    print_bin_summary()

    results = evaluate_sr3_run2_with_boundary(data)
    with redirect_stdout(results_logger):

        print("\n==============================")
        print(" REFERENCE RESULTS")
        print("==============================")
        
        print_sr3_fom_summary(results)

    
    results_for_plots = build_sr3_plot_results(data)
    plot_data = results_for_plots[0]["results"]
    
    for flav in plot_data:

        print_sr3_bin_table(data, flav=flav, mass="2000")
        
        print("\n==============================")
        print(f"Plotting flavour: {flav}")
        print("==============================")
        
        make_mass_plot_multi(
            results_list=results_for_plots,
            flav=flav,
            LOG_TAG=outtag,
            out_tag="sr3_ref"
        )

    RunScan=True
    if RunScan:
        # ----------------------------------
        # NORMAL SCAN MODE
        # ----------------------------------


        timer.start("Scan [parallel] GlobalMass")

        if RunGlobalSig:
            results_scan_globalsig = evaluate_sr3_scan_parallel(
                data,
                MASSES,
                run_scan="GlobalMass",
                n_workers=NCORE
            )
            for r in results_scan_globalsig:
                print(f"Global Mass {r['flav']} {r['mass']} -> MET={r['met']} Z={r['run2']:.4f}")
        timer.stop("Scan [parallel] GlobalMass")
       
        timer.start("Scan [parallel] PerMass")
        
        results_scan = evaluate_sr3_scan_parallel(
            data,
            MASSES,
            run_scan="PerMass"+scan_type,
            n_workers=NCORE
        )
        timer.stop("Scan [parallel] PerMass")
        
        for r in results_scan:
            print(f"Per Mass {r['flav']} {r['mass']} -> MET={r['met']} Z={r['run2']:.4f}")


        with redirect_stdout(results_logger):

            print("\n==============================")
            print(" FINAL SCAN SUMMARY")
            print("==============================")
            
            print_scan_summary(results_scan)
            
            for MASS in MASSES:
                for FLAV in FLAVOURS:
                    print_scan_details(results_scan, FLAV, MASS)


            if RunGlobalSig:
                
                print("\n==============================")
                print(" FINAL GLOBAL MASS SCAN SUMMARY")
                print("==============================")

                print_scan_summary(results_scan_globalsig)
                
                for MASS in MASSES:
                    for FLAV in FLAVOURS:
                        print_scan_details(results_scan_globalsig, FLAV, MASS)
                        
        
        combined_results_for_plots = []
            
        combined_results_for_plots.append({
            "results": convert_results_for_plot(results, mode="run2"),
            "label": "SR3 Ref",
        })
        
        combined_results_for_plots.append({
            "results": convert_results_for_plot(results_scan, mode="run2"),
            "label": "SR3 Scan [best MET/mass]",
        })

        if RunGlobalSig:
            
            combined_results_for_plots.append({
                "results": convert_results_for_plot(results_scan_globalsig, mode="run2"),
                "label": "SR3 Scan GlobalSig [best MET/mass]",
            })
        
        for flav in FLAVOURS:            
            print("\n==============================")
            print(f"Plotting flavour: {flav} (SCAN)")
            print("==============================")
            
            make_mass_plot_multi(
                results_list=combined_results_for_plots,
                flav=flav,
                LOG_TAG=outtag,
                out_tag="sr3_scan_permass_perflav"
            )
        combined_results_for_permet_plots = []
        # Ref
        combined_results_for_permet_plots.append({
            "results": convert_results_for_plot(results, mode="run2"),
        "label": "SR3 Ref",
        })
        # Fixed MET curves (from SAME scan)
        for met in ["2", "3", "4", "5"]:
            
            fixed = extract_fixed_met(results_scan, met)
            
            combined_results_for_permet_plots.append({
                "results": convert_results_for_plot(fixed, mode="run2"),
                "label": f"SR3 Scan (MET={met})",
            })
        

    
    timer.stop("TOTAL")
    timer.summary()


if __name__ == "__main__":
    main()
