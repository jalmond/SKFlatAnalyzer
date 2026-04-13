#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from tqdm import tqdm
import ctypes
import time
import importlib
ROOT.gROOT.SetBatch(True)



#### Build data
from data_format import hist_to_array, bins_to_array_with_err, build_data

from helper import debug_data_summary, get_latest_dir,ReadConfig
from plotter import make_mass_plot_multi, convert_results_for_plot

from ref_fom_utils import evaluate_ref_bins_no_fakecorr, evaluate_ref_bins_fakecorr, compare_fake_impact
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


from logger import print_final_summary, print_scan_summary_table, print_scan_binning_table


# =========================================================
# MAIN
# =========================================================

def main():

    timer = Timer()
    timer.start("TOTAL")

    base = "/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"

    # ----------------------------------
    # Use latest production directory
    # ----------------------------------
    base = get_latest_dir(base)

    parser = argparse.ArgumentParser(description="FOM scan runner")
    parser.add_argument("--runRef", action="store_true", default=None)
    parser.add_argument("--runDP",  action="store_true", default=None)
    parser.add_argument("--config", default="config.quick_scan_config")
    
    args = parser.parse_args()

    #### Read config
    import importlib.util
    if importlib.util.find_spec(args.config) is None:
        raise ImportError(f"[ERROR] Config module '{args.config}' not found")
    
    config_module = importlib.import_module(args.config)
    MASSES, NBINS_TO_SCAN, USE_FAKE_FIX, RUN_Z_NO_UNC, LOG_TAG = ReadConfig(config_module)
    
    
    
    run_ref = args.runRef if args.runRef is not None else RUN_REF
    run_dp  = args.runDP  if args.runDP  is not None else RUN_SCANS

    # ----------------------------------
    # Setup logging
    # ----------------------------------
    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs", exist_ok=True)

    log_file = f"logs/build_{LOG_TAG}_data_{ts}.txt"
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
    timer.start("Build data")
    data = build_data(base, MASSES)
    timer.stop("Build data")

    debug_data_summary(data, 5)

    # ----------------------------------
    # Norm check
    # ----------------------------------
    print("\n[NORM VALUES]")
    for m in MASSES:
        print(f"  {m} -> {data['norm'][m]:.6f}")

    print("\n==============================")
    print(" DONE")
    print("==============================")

    # =====================================================
    # START SCANNING
    # =====================================================

    results_for_plots = []
    scan_results_for_plots = []
    scan_results_for_plots_eradep = []

    # =====================================================
    # REF
    # =====================================================
    if run_ref:

        timer.start("Reference evaluation")

        print("==============================")
        print("Plot Reference bins in ref_bins.py")
        print("==============================")

        print("\n[STEP] Evaluating REF bins...")

        res_no_raw = evaluate_ref_bins_no_fakecorr(data)
        res_fx_raw = evaluate_ref_bins_fakecorr(data)

        compare_fake_impact(res_no_raw, res_fx_raw)

        res_no_run2 = convert_results_for_plot(res_no_raw, mode="run2")

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
        timer.stop("Reference evaluation")
        timer.start("Reference plotting")

        for flav in res_no_run2:

            print("\n==============================")
            print(f"Plotting flavour: {flav}")
            print("==============================")

            make_mass_plot_multi(
                results_list=results_for_plots,
                flav=flav,
                LOG_TAG=LOG_TAG,
                out_tag="fake_comparison"
            )
        timer.stop("Reference plotting")

    # =====================================================
    # DP SCANS
    # =====================================================

    scan_results_for_plots_perNB = []
    scan_results_for_plots_globalflav_perNB = []
    scan_results_for_plots_globalsig_perNB = []
    scan_results_for_plots_pererabinning_perNB = []

    # ----------------------------------
    # Per flavour per mass
    # ----------------------------------

    for nb in NBINS_TO_SCAN:
        timer.start(f"DP per flavour per mass (nb={nb})")
        print("\n======================================")
        print(f" Running DP scan for nBins = {nb}")

        
        res_dp_raw = evaluate_dp_per_flavour_per_mass_run2(data, nb,use_fake_corr=USE_FAKE_FIX,run_z_no_unc=RUN_Z_NO_UNC)
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
            scan_results_for_plots_eradep.append({
                "results": convert_results_for_plot(res_dp_raw, mode="run2"),
                "raw": res_dp_raw,
                "label": f"Scan (Run2, N_bins={nb})"
            })
            scan_results_for_plots_eradep.append({
                "results": convert_results_for_plot(res_dp_raw, mode="quad"),
                "raw": res_dp_raw,
                "label": f"Scan (Run2, N_bins={nb} [QUAD])"
            })

        timer.stop(f"DP per flavour per mass (nb={nb})")
    
    # ----------------------------------
    # Global signal
    # ----------------------------------

    for nb in NBINS_TO_SCAN:
        timer.start(f"DP global signal (nb={nb})")
        print("\n======================================")
        print(f" Running DP scan global signal nBins = {nb}")

        
        res_dp_raw_global_sig = evaluate_dp_per_flavour_global_mass_run2(data, nb,use_fake_corr=USE_FAKE_FIX,run_z_no_unc=RUN_Z_NO_UNC)
        

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
        timer.stop(f"DP global signal (nb={nb})")

    # ----------------------------------
    # Global flavour
    # ----------------------------------

    for nb in NBINS_TO_SCAN:
        timer.start(f"DP global flavour (nb={nb})")
        print("\n======================================")
        print(f" Running DP global flavour nBins = {nb}")

        
        res_dp_raw_globalflav = evaluate_dp_flavour_per_mass_combined_stat_split_run2(data, nb,use_fake_corr=USE_FAKE_FIX,run_z_no_unc=RUN_Z_NO_UNC)
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

        timer.stop(f"DP global flavour (nb={nb})")
            
    # ----------------------------------
    # Per era binning
    # ----------------------------------

    for nb in NBINS_TO_SCAN:
        timer.start(f"DP per era (nb={nb})")
        print("\n======================================")
        print(f" Running DP per era nBins = {nb}")

        
        res_dp_raw_perera = evaluate_dp_per_mass_per_flavour_per_era(data, nb,use_fake_corr=USE_FAKE_FIX,run_z_no_unc=RUN_Z_NO_UNC)
        

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
            scan_results_for_plots_eradep.append({
                "results": convert_results_for_plot(res_dp_raw_perera, mode="run2"),
                "raw": res_dp_raw_perera,
                "label": f"Scan (Era-Dep, Run2, N_bins={nb})"
            })
            scan_results_for_plots_eradep.append({
                "results": convert_results_for_plot(res_dp_raw_perera, mode="quad"),
                "raw": res_dp_raw_perera,
                "label": f"Scan (Era-Dep, Run2, N_bins={nb} [QUAD])"
            })

        timer.stop(f"DP per era (nb={nb})")
    # =====================================================
    # PLOTTING
    # =====================================================
    timer.start("Plotting")

    for flav in FLAVOURS:

        print("\n==============================")
        print(f"Plotting flavour: {flav}")
        print("==============================")
        make_mass_plot_multi(
            results_list=scan_results_for_plots,
            flav=flav,
            LOG_TAG=LOG_TAG,
            out_tag="scan_results"
        )
        if len(scan_results_for_plots_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_perNB,
                flav=flav,
                LOG_TAG=LOG_TAG,
                out_tag="scan_results_perflav_persigmass_Nbins"
            )

        if len(scan_results_for_plots_globalsig_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_globalsig_perNB,
                flav=flav,
                LOG_TAG=LOG_TAG,
                out_tag="scan_results_perflav_globalsigmass_Nbins"
            )

        if len(scan_results_for_plots_globalflav_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_globalflav_perNB,
                flav=flav,
                LOG_TAG=LOG_TAG,
                out_tag="scan_results_globalflav_persigmass_Nbins"
            )
        if len(scan_results_for_plots_pererabinning_perNB) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_pererabinning_perNB,
                flav=flav,
                LOG_TAG=LOG_TAG,
                out_tag="scan_results_perflav_persigmass_perera_Nbins"
            )
        if len(scan_results_for_plots_eradep) > 0:
            make_mass_plot_multi(
                results_list=scan_results_for_plots_eradep,
                flav=flav,
                LOG_TAG=LOG_TAG,
                out_tag="scan_results_eradep_Nbins"
            )
    


    timer.stop("Plotting")

    # =====================================================
    # SUMMARY
    # =====================================================

    print_scan_summary_table(scan_results_for_plots)
    print_scan_summary_table(scan_results_for_plots, use_quad=True)
    print_scan_binning_table(scan_results_for_plots)

    print("\n==============================")
    print("[INFO] Log file saved to:")
    print(log_file)
    print("==============================")

    # FINAL TIMER OUTPUT
    timer.stop("TOTAL")
    timer.summary()


if __name__ == "__main__":
    main()
