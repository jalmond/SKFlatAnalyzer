#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from tqdm import tqdm
import ctypes
import time

import numpy as np

ROOT.gROOT.SetBatch(True)

from config import ERAS, FLAVOURS, FAKE_FLOOR, USE_FAKE_FIX

from helper import  rebin_array,compute_bin_Z,debug_data_summary
from loader import load_histogram,load_background, load_signal,load_fake

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

from logger import fmt_ratio
        
# =========================================================
# HELPERS
# =========================================================


def get_latest_dir(base):
    dirs = [os.path.join(base, d)
            for d in os.listdir(base)
            if os.path.isdir(os.path.join(base, d))]
    return max(dirs, key=os.path.getmtime)



def correct_bkg(lo, hi, sub_bins, fake_bins=None, apply_fake_fix=False):

    bkg = sum(b[2] for b in sub_bins)

    if apply_fake_fix and fake_bins is not None:
        sub_fake = [b for b in fake_bins if lo <= b[0] < hi]
        fake_sum = sum(b[2] for b in sub_fake)

        if fake_sum < FAKE_FLOOR:
            bkg += (FAKE_FLOOR - fake_sum)

    return bkg



# =========================================================
# BUILD ARRAYS
# =========================================================

def hist_to_array(h):
    return np.array([h.GetBinContent(i) for i in range(1, h.GetNbinsX()+1)])


def bins_to_array(bins, fake_bins):

    n = len(bins)
    arr = np.zeros(n)

    for i in range(n):
        lo, hi = bins[i][0], bins[i][1]
        sub = [b for b in bins if lo <= b[0] < hi]

        arr[i] = correct_bkg(lo, hi, sub, fake_bins, apply_fake_fix=False)

    return arr


# =========================================================
# BUILD MAIN DATA STRUCTURE
# =========================================================

def build_data(base, masses):

    data = {}

    # ----------------------------------
    # Load everything
    # ----------------------------------
    bkg_all = {}
    fake_all = {}
    sig_all = {}

    for flav in FLAVOURS:

        bkg_all[flav] = load_background(base, flav)
        fake_all[flav] = load_fake(base, flav)

        sig_all[flav] = {}
        for m in masses:
            sig_all[flav][m] = load_signal(base, flav, m)

    # ----------------------------------
    # Edges
    # ----------------------------------
    ref = bkg_all[FLAVOURS[0]][ERAS[0]]
    edges = np.array([b[0] for b in ref] + [ref[-1][1]])

    data["edges"] = edges

    # ----------------------------------
    # Background + Fake arrays
    # ----------------------------------
    data["background"] = {}
    data["fake"] = {}

    for flav in FLAVOURS:

        data["background"][flav] = {}
        data["fake"][flav] = {}

        for era in ERAS:

            bins = bkg_all[flav][era]
            fake = fake_all[flav][era]

            data["background"][flav][era] = bins_to_array(bins, fake)
            data["fake"][flav][era] = np.array([f[2] for f in fake])

    # ----------------------------------
    # Signal arrays
    # ----------------------------------
    data["signal"] = {}

    for flav in FLAVOURS:
        data["signal"][flav] = {}

        for m in masses:
            data["signal"][flav][m] = {}

            for era in ERAS:
                data["signal"][flav][m][era] = hist_to_array(sig_all[flav][m][era])

    # ----------------------------------
    # Combine signal per mass (sum eras)
    # ----------------------------------
    data["signal_combined_mass"] = {}

    for flav in FLAVOURS:

        data["signal_combined_mass"][flav] = {}

        for m in masses:

            arr = sum(data["signal"][flav][m][era] for era in ERAS)

            if arr.sum() > 0:
                arr = arr / arr.sum()

            data["signal_combined_mass"][flav][m] = arr

    # ----------------------------------
    # Sum over masses
    # ----------------------------------
    data["signal_sum"] = {}

    for flav in FLAVOURS:
        total = sum(data["signal_combined_mass"][flav][m] for m in masses)
        data["signal_sum"][flav] = total

    # ----------------------------------
    # Global signal (all flavours)
    # ----------------------------------
    data["signal_global"] = {}

    for m in masses:

        total = 0

        for flav in FLAVOURS:
            for era in ERAS:
                total += data["signal"][flav][m][era]

        if total.sum() > 0:
            total = total / total.sum()

        data["signal_global"][m] = total

    # ----------------------------------
    # Compute norm
    # ----------------------------------
    data["norm"] = {}

    for m in masses:

        S = data["signal_global"][m]
        B = sum(
            data["background"][flav][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        Z2 = 0.0

        for i in range(len(S)):
            s = S[i]
            b = B[i]

            if s > 0 and b > 0:
                Z2 += 2*((s+b)*math.log(1+s/b) - s)

        data["norm"][m] = Z2

    return data


def main():

    base = "/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"

    # ----------------------------------
    # Use latest production directory
    # ----------------------------------
    base = get_latest_dir(base)

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
    masses = [
        "400","450","500",
        "600","700","800","900",
        "1000","1500","2000"
    ]

    print("\n[INFO] Masses:", masses)

    # ----------------------------------
    # Build data
    # ----------------------------------
    print("\n[STEP] Building data...")
    data = build_data(base, masses)
    debug_data_summary(data,5)
    
    # ----------------------------------
    # Norm check
    # ----------------------------------
    print("\n[NORM VALUES]")
    for m in masses:
        print(f"  {m} -> {data['norm'][m]:.6f}")



    print("\n==============================")
    print(" DONE")
    print("==============================")

    # Make scans and save results for plots
    results_for_plots = []
    
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

    
    for flav in res_no_run2:

        print("\n==============================")
        print(f"Plotting flavour: {flav}")
        print("==============================")
        
        make_mass_plot_multi(
            results_list=results_for_plots,
            flav=flav,
            out_tag="fake_comparison"
        )


    print("==============================")
    print("Run Scans")
    print("==============================")

    scan_resuls_for_plots= []
    res_dp_raw = evaluate_dp_per_mass(data,6)
    print(type(res_dp_raw))
    print(res_dp_raw.keys())
    print(res_dp_raw["MuMu"][0])

    scan_resuls_for_plots.append({
        "results": convert_results_for_plot(res_dp_raw, mode="run2"),
        "label": "DP (Run2)"
    })
    
    
        
if __name__ == "__main__":
    main()
