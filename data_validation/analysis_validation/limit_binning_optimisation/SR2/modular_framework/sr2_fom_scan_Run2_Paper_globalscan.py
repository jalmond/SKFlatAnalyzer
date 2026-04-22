#!/usr/bin/env python3

import ROOT, os, argparse, sys, datetime, time, importlib
from contextlib import contextmanager

ROOT.gROOT.SetBatch(True)

@contextmanager
def redirect_stdout(target):
    old_stdout = sys.stdout
    sys.stdout = target
    try:
        yield
    finally:
        sys.stdout = old_stdout


# ---- Imports ----
from python.utils.data_format import build_data_sr2

from python.utils.helper import (
    debug_data_summary, get_latest_dir, ReadConfig, ConvertConfPath,
    list_available_configs, print_sr2_bin_table,
    debug_fixed_binning_stat_failures, print_sr2_z_ref_summary,
    build_mass_weights_from_ref, build_fixed_met_results,
    recompute_per_met_results, print_bkg_per_bin,
    recompute_per_mass_with_fixed_binning
)

from python.plotter.plotter import (
    make_mass_plot_multi, convert_results_for_plot,
    build_sr2_plot_results
)

from python.config.default_config import BASE_DIR, FLAVOURS, NCORE, ERAS
import python.config.default_config

from python.config.config_utils import GetScanName

from python.utils.logger import (
    print_config_file, print_sr2_scan_table_from_results,
    debug_print_yields_integral
)

from python.scan.ref_fom_utils import *
from python.scan.fom_utils import *
from python.scan.evaluator import (
    evaluate_scan_results, find_best_met_per_flavour
)


# =========================================================
# TIMER
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
# LOGGING
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



        

# =========================================================
# MAIN
# =========================================================
def main():

    timer = Timer()
    timer.start("TOTAL")

    # ----------------------------------
    # Input directory
    # ----------------------------------
    if BASE_DIR is not None:
        base = BASE_DIR
    else:
        base = get_latest_dir(
            "/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"
        )

    # ----------------------------------
    # Args
    # ----------------------------------
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", action="store_true")
    parser.add_argument("--config", required=True)
    parser.add_argument("--tag", default=None)

    args = parser.parse_args()

    conf_path = ConvertConfPath(args.config)
    config_module = importlib.import_module(conf_path)

    MASSES, RunZ_With_Err, LOG_TAG, TAG = ReadConfig(config_module)

    import python.utils.helper
    python.utils.helper.set_stat_config(config_module)

    tag = args.tag if args.tag else TAG

    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    LOG_TAG = LOG_TAG + "_globalscan"

    os.makedirs(f"output/logs/{tag}/{LOG_TAG}", exist_ok=True)
    outtag = f"{tag}/{LOG_TAG}"

    log_file = f"output/logs/{outtag}/build_data_{ts}.txt"
    results_file = f"output/logs/{outtag}/results_{ts}.txt"

    sys.stdout = TeeLogger(log_file)
    
    results_logger = SimpleLogger(results_file)

    print("[INFO] Input:", base)

    # ----------------------------------
    # Build data
    # ----------------------------------
    timer.start("Build data")
    data = build_data_sr2(base, MASSES, sig_name="HNL")
    timer.stop("Build data")

    # ----------------------------------
    # Reference
    # ----------------------------------
    ref_results = evaluate_sr2_run2_with_boundary(data)
    print_sr2_z_ref_summary(ref_results)

    ref_plot = build_sr2_plot_results(data)

    # ----------------------------------
    # STANDARD SCAN
    # ----------------------------------
    standard_config = {
        "scan_type": "PerMassPerFlav",
        "nbin_mode": 3,
        "opt_mode": "Run2",
        "min_bin_width": 0.1
    }

    standard_config["scan_name"] = GetScanName(standard_config)
    standard_config["mass_weights"] = build_mass_weights_from_ref(ref_results)

    timer.start("Standard scan")

    scan_outputs = run_parallel_global_scans(
        data, FLAVOURS, MASSES, standard_config, n_workers=NCORE
    )

    final_scan = evaluate_scan_results(data, scan_outputs, standard_config)

    print_sr2_scan_table_from_results(final_scan, data)

    print_bkg_per_bin(data, scan_outputs, standard_config)

    timer.stop("Standard scan")


    # ----------------------------------
    # DONE
    # ----------------------------------
    timer.summary()

    print("[DONE]")
    print("\n==============================")

    print(" OUTPUT PATHS")
    
    print("==============================")
    
    print(f"[INFO] Log (stdout)   : {log_file}")
    
    print(f"[INFO] Results log   : {results_file}")

if __name__ == "__main__":
    main()
