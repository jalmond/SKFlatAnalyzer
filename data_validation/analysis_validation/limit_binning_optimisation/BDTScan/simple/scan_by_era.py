#!/usr/bin/env python3

import os
import sys
import math
import ROOT
import time
import numpy as np
import random

ROOT.gROOT.SetBatch(True)

FAKE_FLOOR = 0.096

class Tee:
    def __init__(self, *files):
        self.files = files

    def write(self, obj):
        for f in self.files:
            f.write(obj)
            f.flush()

    def flush(self):
        for f in self.files:
            f.flush()
            

def format_edges_str(edges, ndigits=2):

    out = []

    for x in edges:
        if abs(x) < 1e-10:
            x = 0.0

        out.append(f"{x:.{ndigits}f}")

    return "[" + ", ".join(out) + "]"


# ---------------------------------------
# Fake correction
# ---------------------------------------
def fix_fake_and_bkg(F, B, FAKE_FLOOR, flavour=None, era=None, debug=False):
    if F < FAKE_FLOOR:
        delta = FAKE_FLOOR - F
        F = FAKE_FLOOR
        B += delta
    return F, B

# ---------------------------------------
# Asimov FOM
# ---------------------------------------
def calculate_fom(s, b):
    if b <= 0:
        return 0
    term = (s + b) * math.log(1 + s / b) - s
    return 2 * term if term > 0 else 0

# ---------------------------------------
# Random combinations
# ---------------------------------------
def get_random_combinations(edges, k, n_samples):
    edges = list(edges)
    seen = set()
    while len(seen) < n_samples:
        combo = tuple(sorted(random.sample(edges, k)))
        if combo not in seen:
            seen.add(combo)
            yield combo

def evaluate_combination(args):

    try:
        
        return evaluate_combination_fnct(args)

    except Exception as e:

        print("[WORKER ERROR]", e)

        return None
            
def evaluate_combination_fnct(args):
    mids, bin_cache, nbins, x_min, x_max, tail_cut = args

    edges = [x_min] + list(mids) + [x_max]

    if edges[-2] > tail_cut:
        return None
    
    total_fom = 0
    valid = True
    
    for j in range(nbins):

        x_low = edges[j]
        x_high = edges[j+1]

        run2_sig = 0
        run2_bkg = 0

        for era, (centers, bvals,fvals, svals, evals) in bin_cache.items():

            mask = (centers >= x_low) & (centers < x_high)

            if not np.any(mask):
                return None

            bkg  = bvals[mask].sum()
            fake = fvals[mask].sum()           
            sig  = svals[mask].sum()
            err2 = np.sum(evals[mask]**2)

            fake, bkg = fix_fake_and_bkg(fake, bkg, FAKE_FLOOR)

            
            if bkg < 0.15:
                return None

            run2_sig += sig
            run2_bkg += bkg


        if run2_bkg < 1:
            return None

        total_fom += calculate_fom(run2_sig, run2_bkg)
        
    return (total_fom, edges)
            
def find_tail_cut(histograms, n_consecutive=3):

    test_edges = np.arange(0.4, -0.4, -0.01)

    pass_count = 0

    for x in test_edges:

        run2_bkg = 0
        valid = True

        for era, (h, hs, hf) in histograms.items():

            bkg = 0

            for k in range(1, h.GetNbinsX()+1):

                xc = h.GetBinCenter(k)

                if not (x <= xc <= 0.4):
                    continue

                b = h.GetBinContent(k)
                f = hf.GetBinContent(k)

                f, b = fix_fake_and_bkg(f, b, FAKE_FLOOR)

                bkg += b

            if bkg < 0.15:
                valid = False
                break

            run2_bkg += bkg

        if valid and run2_bkg >= 1:
            pass_count += 1
            if pass_count >= n_consecutive:
                return x
        else:
            pass_count = 0

    return 0.4
            
# ---------------------------------------
# Scan binning (multi-era)
# ---------------------------------------
from multiprocessing import Pool, cpu_count

def scan_optimal_variable_binning(scan_era,histograms, nbins, max_trials, flav=None):

    x_min, x_max = -0.4, 0.4

    tail_cut = find_tail_cut(histograms)
    print("[INFO] Tail cutoff at:", tail_cut)

    # --- build bin_cache (same as before) ---
    bin_cache = {}

    for era, (h, hs, hf) in histograms.items():
        
        if scan_era != "Run2" and era != scan_era:
            continue
        
        centers = []
        bvals = []
        fvals = []
        svals = []
        evals = []

        for k in range(1, h.GetNbinsX()+1):

            x = h.GetBinCenter(k)

            if not (-0.4 <= x <= 0.4):
                continue

            b = h.GetBinContent(k)
            s = hs.GetBinContent(k)
            f = hf.GetBinContent(k)

            #f_corr, b_corr = fix_fake_and_bkg(f, b, FAKE_FLOOR, flavour=flav, era=era)

            centers.append(x)
            bvals.append(b)
            fvals.append(f)
            svals.append(s)
            evals.append(h.GetBinError(k))

        bin_cache[era] = (
            np.array(centers),
            np.array(bvals),
            np.array(fvals),
            np.array(svals),
            np.array(evals)
        )

    # --- edges ---
    edges_low  = np.arange(-0.35, 0.0, 0.05)
    
    edges_med  = np.arange(0.0, 0.2, 0.02)
    
    edges_high = np.arange(0.2, 0.4, 0.01)
    
    edges_all = np.concatenate((edges_low, edges_med, edges_high))
    
    edges_all = np.unique(np.round(edges_all, 5))
    

    combinations = get_random_combinations(edges_all, nbins - 1, max_trials)

    ncpu = 6
    
    print(f"[INFO] Using {ncpu} cores")
    
    args_iter = (
    
        (mids, bin_cache, nbins, x_min, x_max, tail_cut)
        
        for mids in combinations

    )
    
    best_fom = -1

    best_edges = None
    
    chunksize = max(1, max_trials // (ncpu * 10))
    
    with Pool(ncpu) as pool:

        for i, result in enumerate(
            
                pool.imap_unordered(evaluate_combination, args_iter, chunksize=chunksize)
                
        ):
            
            if i % 10000 == 0:
            
                print(f"[SCAN] {i}/{max_trials}, best={best_fom:.3f}")
                
            if result is None:

                continue

            fom, edges = result
        
            if fom > best_fom:
            
                best_fom = fom

                best_edges = edges

    return best_edges, best_fom
    


# ---------------------------------------
# Existing binning FOM
# ---------------------------------------
def compute_existing_limitbins_fom(histograms, mass, flavour, base_dir):

    if flavour == "MuMu":
        hname = "MuonSR3BDT"
    elif flavour == "EE":
        hname = "ElectronSR3BDT"
    else:
        hname = "ElectronMuonSR3BDT"

    # ---------------------------------------
    # Load all hists first
    # ---------------------------------------
    era_hists = {}

    for era in eras:

        print(f"[INFO] Loading {era}")

        f_bkg  = ROOT.TFile.Open(f"{base_dir}/{era}/HNL_SignalRegion_Plotter_Bkg.root")
        f_sig  = ROOT.TFile.Open(f"{base_dir}/{era}/HNL_SignalRegion_Plotter_HNL_{mass}.root")
        f_fake = ROOT.TFile.Open(f"{base_dir}/{era}/HNL_SignalRegion_Plotter_Fake.root")

        path = f"LimitExtractionBDT/HNL_ULIDv2/{flavour}_V3_Strict_15_Bin/M{mass}/LimitBins/{hname}"

        h_bkg  = f_bkg.Get(path)
        h_sig  = f_sig.Get(path)
        #h_sig.Scale(1/h_sig.Integral())
        h_fake = f_fake.Get(path)
        
        h_bkg.SetDirectory(0)
        
        h_sig.SetDirectory(0)
        
        h_fake.SetDirectory(0)
        if h_bkg is None:

            print(f"[FATAL] h_bkg missing for {era}")
            
            f_bkg.ls()
            
            continue

        if h_sig is None:
            
            print(f"[FATAL] h_sig missing for {era}")
            
            f_sig.ls()
            
            continue
        
        if h_fake is None:
            
            print(f"[FATAL] h_fake missing for {era}")
        
            f_fake.ls()
            
            continue

        # extra protection (PyROOT weirdness)

        if not hasattr(h_bkg, "GetNbinsX"):

            print(f"[FATAL] h_bkg invalid object for {era}")
            
            continue
        
        if h_bkg is None:
            print(f"[ERROR] Missing h_bkg: {era}")
        if h_sig is None:
            print(f"[ERROR] Missing h_sig: {era}")
        if h_fake is None:
            print(f"[ERROR] Missing h_fake: {era}")
        if h_bkg is None or h_sig is None or h_fake is None:
            print(f"[WARNING] Missing hist for {era}")
            continue

        era_hists[era] = (h_bkg, h_sig, h_fake)

    if not era_hists:
        print("[ERROR] No histograms loaded")
        return 0

    # ---------------------------------------
    # Loop over bins FIRST
    # ---------------------------------------
    nbins = None

    for era, (h_bkg, _, _) in era_hists.items():
        
        if h_bkg is not None:
            
            nbins = h_bkg.GetNbinsX()
            
            break

    if nbins is None:
        
        print("[ERROR] No valid histograms found")
        
        return 0

    total_fom_run2 = 0.0
    total_fom_per_era = {era: 0.0 for era in era_hists}

    print("\n=== BIN-BY-BIN FOM ===")

    for i in range(1, nbins + 1):

        run2_sig = 0.0
        run2_bkg = 0.0

        # --- loop eras ---
        for era, (h_bkg, h_sig, h_fake) in era_hists.items():

            b = h_bkg.GetBinContent(i)
            s = h_sig.GetBinContent(i)
            f = h_fake.GetBinContent(i)

            # fake correction per bin (same as existing logic)
            f, b = fix_fake_and_bkg(f, b, FAKE_FLOOR)

            # accumulate Run2
            run2_sig += s
            run2_bkg += b

            # per-era FOM
            fom_era = calculate_fom(s, b)
            total_fom_per_era[era] += fom_era

        # --- Run2 FOM ---
        if run2_bkg > 0:
            fom_run2 = calculate_fom(run2_sig, run2_bkg)
            total_fom_run2 += fom_run2
        else:
            fom_run2 = 0

        print(f"[BIN {i:3d}] Run2: S={run2_sig:.2f}, B={run2_bkg:.2f}, FOM={fom_run2:.3f}")

    # ---------------------------------------
    # Summary
    # ---------------------------------------
    print("\n=== SUMMARY ===")

    print(f"[Run2] Total FOM (sum Z^2) = {total_fom_run2:.3f}")

    for era in total_fom_per_era:
        print(f"[{era}] Total FOM (sum Z^2) = {total_fom_per_era[era]:.3f}")

    return total_fom_run2


def compute_run2_fom_from_edges(histograms, edges_per_era):

    nbins = len(next(iter(edges_per_era.values()))) - 1

    total_fom = 0.0

    print("\n=== RUN2 FOM USING SCANNED EDGES ===")

    for i in range(nbins):

        run2_sig = 0.0
        run2_bkg = 0.0

        # --- loop over eras ---
        for era, (h_bkg, h_sig, h_fake) in histograms.items():

            edges = edges_per_era[era]

            x_low  = edges[i]
            x_high = edges[i+1]

            sig = 0.0
            bkg = 0.0
            fake = 0.0

            # integrate histogram in this bin range
            for k in range(1, h_bkg.GetNbinsX()+1):

                x = h_bkg.GetBinCenter(k)

                if not (x_low <= x < x_high):
                    continue

                b = h_bkg.GetBinContent(k)
                s = h_sig.GetBinContent(k)
                f = h_fake.GetBinContent(k)

                fake += f
                bkg  += b
                sig  += s

            # apply fake correction AFTER summing bin
            fake, bkg = fix_fake_and_bkg(fake, bkg, FAKE_FLOOR)

            run2_sig += sig
            run2_bkg += bkg

        # --- Run2 FOM per bin ---
        if run2_bkg > 0:
            z2 = calculate_fom(run2_sig, run2_bkg)
            total_fom += z2
        else:
            z2 = 0

        print(f"[BIN {i:2d}] S={run2_sig:.3f}, B={run2_bkg:.3f}, Z2={z2:.4f}")

    print("\n[Run2 scanned binning] Total FOM =", total_fom)

    return total_fom


# ---------------------------------------
# MAIN
# ---------------------------------------



import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--mass', required=True)
parser.add_argument('--flavour', required=True)
parser.add_argument('--n_bins', type=int, default=15)
parser.add_argument('--n_trials', type=int, default=10000000)
args = parser.parse_args()

mass = args.mass
flavour = args.flavour



log_name = f"logs/scan_log_mass{mass}_{flavour}.txt"

log_file = open(log_name, "w")

sys.stdout = Tee(sys.stdout, log_file)

sys.stderr = Tee(sys.stderr, log_file)

print(f"[LOG] Writing output to {log_name}")




base_dir = "/data9/Users/HNL_public/SUS-24-014/SKFlatOutput/Scan/SR2/"
eras = ["2016preVFP","2016postVFP","2017","2018"]

histograms = {}

# Load histograms
def prepare_histogram(h, xmin=-0.4, xmax=0.4):

    h = h.Clone()
    h.SetDirectory(0)

    nb = h.GetNbinsX()

    # --- merge underflow into first bin ---
    uf = h.GetBinContent(0)
    uf_err = h.GetBinError(0)

    h.SetBinContent(1, h.GetBinContent(1) + uf)
    h.SetBinError(1, math.sqrt(h.GetBinError(1)**2 + uf_err**2))

    # --- merge overflow into last bin ---
    of = h.GetBinContent(nb+1)
    of_err = h.GetBinError(nb+1)

    h.SetBinContent(nb, h.GetBinContent(nb) + of)
    h.SetBinError(nb, math.sqrt(h.GetBinError(nb)**2 + of_err**2))

    # --- zero bins outside [-0.4, 0.4] ---
    for i in range(1, nb+1):
        x_low = h.GetBinLowEdge(i)
        x_high = h.GetBinLowEdge(i+1)

        if x_high <= xmin or x_low >= xmax:
            h.SetBinContent(i, 0)
            h.SetBinError(i, 0)

    return h

for era in eras:

    f_bkg = ROOT.TFile.Open(f"{base_dir}/{era}/HNL_SignalRegion_Plotter_Bkg.root")
    f_sig = ROOT.TFile.Open(f"{base_dir}/{era}/HNL_SignalRegion_Plotter_HNL_{mass}.root")
    f_fake = ROOT.TFile.Open(f"{base_dir}/{era}/HNL_SignalRegion_Plotter_Fake.root")

    path = f"LimitExtraction/HNL_ULIDv2/{flavour}/SR3BDTBDT"

    d_sig = f_sig.Get(path)

    for key in d_sig.GetListOfKeys():
        name = key.GetName()
        if f"M{mass}" in name:
            break
        
    h_bkg = prepare_histogram(f_bkg.Get(path).Get(name))

    h_sig = prepare_histogram(f_sig.Get(path).Get(name))
    #h_sig.Scale(1/h_sig.Integral())
    h_fake = prepare_histogram(f_fake.Get(path).Get(name))
    
    histograms[era] = (h_bkg, h_sig, h_fake)
# ---------------------------------------
# Run scan
# ---------------------------------------

edges_per_era = {}

for era in eras:

    edges, fom_scan = scan_optimal_variable_binning(

        era,

        histograms,

        nbins=args.n_bins,

        max_trials=args.n_trials,

        flav=flavour

    )

    edges_per_era[era] = edges


for era in eras:
        print(f"{era:12s} : {format_edges_str(edges_per_era[era])}")

    
run2_fom_scanned=compute_run2_fom_from_edges(histograms, edges_per_era)


edges_run2, fom_scan_run2 = scan_optimal_variable_binning(

    "Run2",

    histograms,

    nbins=args.n_bins,

    max_trials=args.n_trials,

    flav=flavour

)
edges_per_era["Run2"] = edges_run2
print(f"Run2 : {format_edges_str(edges_run2)}")
print(f"fom_scan_run2={fom_scan_run2}")

    
# ---------------------------------------
# Existing comparison
# ---------------------------------------

fom_existing = compute_existing_limitbins_fom(
    histograms,
    mass,
    flavour,
    base_dir
)

# ---------------------------------------
# Output
# ---------------------------------------

print("\n==============================")
print(" FINAL RESULT")
print("==============================")
print("Best FOM Era-Dep(scan):      ", round(run2_fom_scanned,3))
print("Best FOM Run2(scan):      ", round(fom_scan_run2,3))
print("Best edges:           ", edges)
print("")
print("Existing FOM:         ", round(fom_existing,3))
print("==============================")
