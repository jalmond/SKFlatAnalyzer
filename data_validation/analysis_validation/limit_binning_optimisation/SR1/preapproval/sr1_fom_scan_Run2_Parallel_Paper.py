#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from tqdm import tqdm
import ctypes

DEBUG=False

ROOT.gROOT.SetBatch(True)
USE_PER_ERA_STAT = False
RED   = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"

STAT_THRESHOLD_PERERA = 0.5
STAT_THRESHOLD_RUN2  = 1.0

TOTAL_BINS_TO_SCAN = [2,3]

VALID_CACHE = {}

# =========================================================
# CONFIG
# =========================================================
ERAS = ["2016preVFP","2016postVFP","2017","2018"]
FLAVOURS = ["MuMu","EE","EMu"]

USE_FAKE_FIX = True
FAKE_FLOOR = 0.15 * 0.645

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

def fmt(edges):
    return "[" + ", ".join(f"{x:.2f}" for x in edges) + "]"

def format_bin_range(n_bins):
    return "{}to{}bins".format(min(n_bins), max(n_bins))

def combine_per_era_fom(per_era_dict):
    return math.sqrt(sum(v*v for v in per_era_dict.values()))

def evaluate_variable_binning_run2(edges_per_era,
                                   bins,
                                   cache,
                                   fake_bins):

    total = 0.0

    # pick ONE binning (reference)
    ref_era = ERAS[0]
    edges = edges_per_era[ref_era]

    for m, c in cache.items():

        f_bins = []

        for i in range(len(edges)-1):

            lo = edges[i]
            hi = edges[i+1]

            bkg = 0.0
            sig = 0.0

            # combine eras ONCE
            for era in ERAS:

                sub = [b for b in bins[era] if lo <= b[0] < hi]
                bkg += correct_bkg(lo, hi, sub, fake_bins[era])

                sig += sum(v for x, v in c.items() if lo <= x < hi)

            f_bins.append(fom(sig, bkg))

        total += sum(z*z for z in f_bins)

    return math.sqrt(total)




# =========================================================
# EVALUATOR
# =========================================================                                                                                     

def evaluate_run2(edges, bins_per_era, cache, fake_per_era):
    total = 0.0

    for m, c in cache.items():
        for era in ERAS:
            f_bins = []

            for i in range(len(edges)-1):
                lo, hi = edges[i], edges[i+1]

                sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_per_era[era])

                sig = sum(v for x, v in c.items() if lo <= x < hi)

                f_bins.append(fom(sig, bkg))

            total += sum(x*x for x in f_bins)

    return math.sqrt(total)


def print_best_binning_summary(edges, bins_per_era, cache, fake_bins, label=""):

    print("\n====================================")
    print(" BEST BINNING SUMMARY:", label)
    print("====================================")

    print("Edges =", fmt(edges))

    # ----------------------------------
    # PER ERA
    # ----------------------------------
    run2_B = [0.0 for _ in range(len(edges)-1)]
    run2_S = [0.0 for _ in range(len(edges)-1)]

    for era in ERAS:

        print("\n  ERA:", era)

        for i in range(len(edges)-1):

            lo = edges[i]
            hi = edges[i+1]

            # --- background
            sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
            bkg = correct_bkg(lo, hi, sub, fake_bins[era])

            # --- signal
            sig = 0.0
            for m, c in cache.items():
                for x, v in c.items():
                    if lo <= x < hi:
                        sig += v

            z = fom(sig, bkg)

            # accumulate Run2
            run2_B[i] += bkg
            run2_S[i] += sig

            # --- COLOR LOGIC (per-era threshold)
            if bkg < STAT_THRESHOLD_PERERA:
                b_str = f"{RED}{bkg:.3f}{RESET}"
            else:
                b_str = f"{bkg:.3f}"

            print(
                "    BIN [{:.1f}, {:.1f}] | B = {} | S = {:.3f} | Z = {:.3f}".format(
                    lo, hi, b_str, sig, z
                )
            )

    # ----------------------------------
    # RUN2 SUMMARY
    # ----------------------------------
    print("\n  RUN2 COMBINED")

    for i in range(len(edges)-1):

        lo = edges[i]
        hi = edges[i+1]

        B = run2_B[i]
        S = run2_S[i]
        Z = fom(S, B)

        # --- COLOR LOGIC (Run2 threshold = 0.5 as you requested)
        if B < 0.5:
            b_str = f"{RED}{B:.3f}{RESET}"
        else:
            b_str = f"{B:.3f}"

        print(
            "    BIN [{:.1f}, {:.1f}] | B = {} | S = {:.3f} | Z = {:.3f}".format(
                lo, hi, b_str, S, Z
            )
        )


import numpy as np
def debug_print_run2_per_flavour(edges, all_bins, all_fake):

    print("\n==============================")
    print(" DEBUG: Run2 per-flavour yields")
    print("==============================")

    for i_flav, flav in enumerate(FLAVOURS):

        print(f"\n[FLAVOUR] {flav}")

        bins_f = all_bins[i_flav]
        fake_f = all_fake[i_flav]

        for i in range(len(edges)-1):

            lo = edges[i]
            hi = edges[i+1]

            b_sum = 0.0

            for era in ERAS:
                sub = [b for b in bins_f[era] if lo <= b[0] < hi]
                b_sum += correct_bkg(lo, hi, sub, fake_f[era], apply_fake_fix=False)

            print(f"  BIN [{lo:.1f}, {hi:.1f}] -> Run2 Bkg = {b_sum:.4f}")

def print_mass_summary(mass, results_mass, n):

    print("\n====================================")
    print(f"Mass = {mass} (n = {n})")
    print("====================================\n")

    scenarios = ["global", "perflav", "sum_global", "sum_perflav", "era"]

    for scen in scenarios:

        print(f"{scen.upper()}")

        for flav in FLAVOURS:

            vals = results_mass[scen][flav]

            # find the entry for this n
            val = next((f for nn, f in vals if nn == n), None)

            if val is None:
                val_str = "N/A"
            else:
                val_str = f"{val:.3f}"

            print(f"  ({flav}) = {val_str}")

        print()  # spacing
            

def dp_global(edges, PB, PE, PS,
              PB_era, PE_era,
              PB_era_flav, PF_era_flav,
              n_bins, use_per_era_stat):
    
    def interval_bkg_fast(p, i):
        return PB[i] - PB[p]

    def interval_bkg(p, i):
        
        b_sum = 0.0
        
        for f in range(len(PB_era_flav)):
            for era in ERAS:
                
                b = PB_era_flav[f][era][i] - PB_era_flav[f][era][p]
                fake = PF_era_flav[f][era][i] - PF_era_flav[f][era][p]
                
                if fake < FAKE_FLOOR:
                    b += (FAKE_FLOOR - fake)

            b_sum += b

        return b_sum
    
    key = (
        tuple(edges),
        tuple(PB),
        tuple(PE),
        use_per_era_stat
    )

    
    if key in VALID_CACHE:
        print("[DEBUG] Using cached valid matrix")
    else:
        print("[DEBUG] Computing NEW valid matrix")
    if key in VALID_CACHE:
        valid = VALID_CACHE[key]
    else:
        valid = build_valid_matrix_global(
            edges, PB, PE, PB_era, PE_era,
            interval_bkg=interval_bkg_fast,
            use_per_era=use_per_era_stat
        )
        VALID_CACHE[key] = valid

        
    M = len(edges) - 1
    # rebuild per-bin bkg from prefix sums
    bkg = [PB[i+1] - PB[i] for i in range(M)]

    max_boundary = compute_last_valid_boundary(edges, PB_era_flav,use_per_era_stat)
    NEG = -1e300

    dp  = np.full((n_bins + 1, M + 1), NEG, dtype=float)
    prv = np.full((n_bins + 1, M + 1), -1, dtype=int)

    dp[0, 0] = 0.0

    if DEBUG:
        print("[DEBUG] interval_fom...")
        
    # ----------------------------------
    # FOM for interval [p, i]
    # ----------------------------------
    def interval_fom(p, i):
        B = interval_bkg(p, i)
        S = PS[i] - PS[p]

        if B <= 0 or S <= 0:
            return 0.0

        val = 2.0 * ((S + B) * math.log(1.0 + S / B) - S)

        if val <= 0:
            return 0.0

        return math.sqrt(val)

    # ----------------------------------
    # DP
    # ----------------------------------
    if DEBUG:
        print("[DEBUG] valid_transitions...")

    valid_transitions = {i: np.where(valid[:, i])[0] for i in range(M+1)}
    for j in range(1, n_bins + 1):
        for i in range(1, M + 1):

            best = NEG
            best_p = -1

            for p in valid_transitions[i]:

                if j == n_bins and i == M:
                    if edges[p] > max_boundary:
                        continue
                
                if dp[j - 1, p] <= NEG / 2:
                    continue

                if not valid[p, i]:
                    continue

                val = dp[j - 1, p] + interval_fom(p, i)

                if val > best:
                    best = val
                    best_p = p

            dp[j, i] = best
            prv[j, i] = best_p

    if DEBUG:
        print("[DEBUG] valid_transitions done...")
                
    # ----------------------------------
    # No solution
    # ----------------------------------
    if dp[n_bins, M] <= NEG / 2:
        return None, None

    # ----------------------------------
    # Backtrack
    # ----------------------------------
    idx = []
    j, i = n_bins, M

    while j > 0:
        p = prv[j, i]
        if p < 0:
            return None, None  # safety
        idx.append(p)
        i = p
        j -= 1

    idx = list(reversed(idx))

    best_edges = [edges[0]] + [edges[k] for k in idx if k != 0] + [edges[-1]]

    return dp[n_bins, M], best_edges


def run_global_dp(all_bins, all_fake, sig_cache, n):


    edges, bkg, err2, sig, bkg_per_era, err2_per_era, bkg_per_era_flav, fake_per_era_flav =    build_global_arrays(
        all_bins, all_fake, sig_cache
    )

    PF_era_flav = [
        {era: prefix_sums(fake_per_era_flav[f][era]) for era in ERAS}
	for f in range(len(fake_per_era_flav))
    ]
    
    PB = prefix_sums(bkg)
    PE = prefix_sums(err2)
    PS = prefix_sums(sig)
    PB_era = {era: prefix_sums(bkg_per_era[era]) for era in ERAS}
    PE_era = {era: prefix_sums(err2_per_era[era]) for era in ERAS}
    PB_era_flav = [
        {era: prefix_sums(bkg_per_era_flav[f][era]) for era in ERAS}
        for f in range(len(bkg_per_era_flav))
    ]
    return dp_global(
        edges,
        PB, PE, PS,
        PB_era,
        PE_era,
        PB_era_flav,
        PF_era_flav,
        n,
        USE_PER_ERA_STAT
    )


# =========================================================
# BUILD GLOBAL ARRAYS (ALL ERAS + FLAVOURS)
# =========================================================

def build_global_arrays(all_bins, all_fake, sig_cache):

    ref = all_bins[0]["2016preVFP"]

    edges = np.array([b[0] for b in ref] + [ref[-1][1]], dtype=float)
    n = len(ref)

    bkg = np.zeros(n)
    err2 = np.zeros(n)
    sig  = np.zeros(n)

    bkg_per_era_flav = [
        {era: np.zeros(n) for era in ERAS}
        for _ in range(len(all_bins))
    ]

    fake_per_era_flav = [
        {era: np.zeros(n) for era in ERAS}
        for _ in range(len(all_bins))
    ]

    err2_per_era = {era: np.zeros(n) for era in ERAS}
    bkg_per_era  = {era: np.zeros(n) for era in ERAS}

    # ----------------------------------
    # LOOP OVER FLAVOURS + ERAS
    # ----------------------------------
    for f_idx in range(len(all_bins)):

        bins_f = all_bins[f_idx]
        fake_f = all_fake[f_idx]

        for era in ERAS:

            bins = bins_f[era]
            fake = fake_f[era]

            for i in range(n):

                lo = bins[i][0]
                hi = bins[i][1]

                sub = [b for b in bins if lo <= b[0] < hi]

                b_val = correct_bkg(lo, hi, sub, fake, apply_fake_fix=False)

                bkg_per_era_flav[f_idx][era][i] += b_val
                fake_per_era_flav[f_idx][era][i] += fake[i][2]

                bkg[i] += b_val

                rel = bins[i][3]
                if rel < 999:
                    err2[i] += (b_val * rel) ** 2

                bkg_per_era[era][i] += b_val
                err2_per_era[era][i] += (b_val * rel) ** 2

    # ----------------------------------
    # SIGNAL
    # ----------------------------------
    for m, c in sig_cache.items():
        for i in range(n):
            lo = edges[i]
            hi = edges[i+1]

            for x, v in c.items():
                if lo <= x < hi:
                    sig[i] += v

    return edges, bkg, err2, sig, bkg_per_era, err2_per_era, bkg_per_era_flav, fake_per_era_flav


def compute_last_valid_boundary(edges, PB_era_flav,use_per_era_stat):

    n = len(edges) - 1
    n_flav = len(PB_era_flav)

    for i in reversed(range(n)):

        ok = True

        for f in range(n_flav):
            if use_per_era_stat:
                for era in ERAS:
                    
                    tail = PB_era_flav[f][era][n] - PB_era_flav[f][era][i]
                    if use_per_era_stat:
                    
                        if tail < STAT_THRESHOLD_PERERA:
                            ok = False
                            break
            else:
                
                tail_run2 = sum(
                    PB_era_flav[f][era][n] - PB_era_flav[f][era][i]
                    for era in ERAS
                )
                if tail_run2 < STAT_THRESHOLD_RUN2:
                    ok = False
                    
            if not ok:
                break

            
            
        if ok:
            if DEBUG:
                print(f"[DEBUG] --> boundary chosen at {edges[i]}")
            return edges[i]

    if DEBUG:
        print("[DEBUG] --> fallback to first edge")
    return edges[0]



# =========================================================
# PREFIX SUM
# =========================================================
def prefix_sums(arr):
    ps = np.zeros(len(arr) + 1, dtype=float)
    ps[1:] = np.cumsum(arr)
    return ps



# =========================================================
# VALID MATRIX 
# =========================================================
def build_valid_matrix_global(edges, PB, PE, PB_era, PE_era, interval_bkg, use_per_era=False):

    M = len(edges) - 1
    valid = np.zeros((M + 1, M + 1), dtype=np.bool_)

    for p in range(M):
        for i in range(p+1, M+1):

            # width
            if (edges[i] - edges[p]) < 25:
                continue

            # --- Run2 check (keep)
            B = interval_bkg(p, i)
            E = PE[i] - PE[p]
            rel = math.sqrt(E)/B if B > 0 else 999

            if use_per_era:

                pass_per_era = True

                for era in ERAS:
                    B_e = PB_era[era][i] - PB_era[era][p]
                    E_e = PE_era[era][i] - PE_era[era][p]
                    rel_e = math.sqrt(E_e)/B_e if B_e > 0 else 999
                    
                    if not ((B_e >= STAT_THRESHOLD_PERERA) or (rel_e <= 0.3)):
                        pass_per_era = False
                    break

                if not pass_per_era:
                    continue

            else:
                B = interval_bkg(p, i)
                E = PE[i] - PE[p]
                rel = math.sqrt(E)/B if B > 0 else 999
                
                if not ((B >= STAT_THRESHOLD_RUN2) or (rel <= 0.3)):
                    continue

            valid[p, i] = True

    return valid


# =========================================================
# DP CORE
# =========================================================


def print_bkg_with_stat(edges, bins_per_era, mode):

    print("\n   --- Background + Stat Check ---")

    if mode == "perera":

        threshold = STAT_THRESHOLD_PERERA

        for era in ERAS:

            print(f"   {era}")

            res = check_bins(edges, bins_per_era[era], threshold)

            vals_str = [f"{GREEN}{v}{RESET}" if ok else f"{RED}{v}{RESET}"
                        for v, ok in res]

            print("     BINS =", "[" + ", ".join(vals_str) + "]")

    else:
        # RUN2 COMBINED CHECK
        threshold = STAT_THRESHOLD_RUN2

        comb = []
        for era in ERAS:
            comb += bins_per_era[era]

        res = check_bins(edges, comb, threshold)

        vals_str = [f"{GREEN}{v}{RESET}" if ok else f"{RED}{v}{RESET}"
                    for v, ok in res]

        print("   COMBINED")
        print("     BINS =", "[" + ", ".join(vals_str) + "]")



def evaluate_fixed_bins(edges, bins, cache, fake_per_era):

    total = 0.0

    for m, c in cache.items():

        f_bins = []

        for era in ERAS:

            for i in range(len(edges)-1):

                lo = edges[i]
                hi = edges[i+1]

                # --- background
                sub = [b for b in bins[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_per_era[era])

                # --- signal
                sig = 0.0
                for x, v in c.items():
                    if lo <= x < hi:
                        sig += v

                f_bins.append(fom(sig, bkg))

        total += sum(x*x for x in f_bins)

    return math.sqrt(total)

        
def print_bkg_summary(edges, bins_per_era):

    print("\n   --- Background per era ---")

    for era in ERAS:

        bkg = get_bkg_per_bin(edges, bins_per_era[era])

        print(f"   {era}")
        print(f"     BINS = {bkg}")
        
# =========================================================
# FOM
# =========================================================
def fom(s,b):
    if b>0 and s>0:
        return math.sqrt(2*((s+b)*math.log(1+s/b)-s))
    return 0

# =========================================================
# FAKE FIX
# =========================================================
def correct_bkg(lo, hi, sub_bins, fake_bins=None, apply_fake_fix=True):

    if USE_FAKE_FIX and fake_bins is None:
        raise RuntimeError("Fake bins missing while fake fix enabled")
    
    bkg = sum(b[2] for b in sub_bins)

    if USE_FAKE_FIX and apply_fake_fix and fake_bins is not None:

        sub_fake = [b for b in fake_bins if lo <= b[0] < hi]
        fake_sum = sum(b[2] for b in sub_fake)

        if fake_sum < FAKE_FLOOR:
            bkg += (FAKE_FLOOR - fake_sum)

    return bkg

# =========================================================
# HIST
# =========================================================
def get_bins(h):
    out=[]
    for i in range(1,h.GetNbinsX()+1):
        lo=h.GetBinLowEdge(i)
        hi=h.GetBinLowEdge(i+1)
        val=h.GetBinContent(i)
        err=h.GetBinError(i)
        rel=err/val if val>0 else 999
        out.append((lo,hi,val,rel))
    return out

def get_latest_dir(base):
    dirs=[os.path.join(base,d) for d in os.listdir(base)
          if os.path.isdir(os.path.join(base,d))]
    return max(dirs,key=os.path.getmtime)


def load_fake(base, flav):

    sel = get_latest_dir(base)

    path = f"PassSR1/HNL_ULIDv2/{flav}/AK8/AK8J_Unbinned_Mass/l1J"

    hist = {}

    for era in ERAS:

        fname = os.path.join(sel, era, "HNL_SignalRegion_Plotter_Fake.root")
        f = ROOT.TFile(fname)

        if not f or f.IsZombie():
            raise RuntimeError(f"[ERROR] Missing fake file: {fname}")

        h_tmp = f.Get(path)

        if not h_tmp:
            raise RuntimeError(f"[ERROR] Missing fake hist: {path} in {fname}")

        h = h_tmp.Clone()
        h.SetDirectory(0)

        f.Close()

        # IMPORTANT: convert to bin structure used everywhere
        hist[era] = get_bins(h)

    return hist


def load_signals(base, flav):

    sel = get_latest_dir(base)
    path = f"PassSR1/HNL_ULIDv2/{flav}/AK8/AK8J_Unbinned_Mass/l1J"

    sig_hist = {}

    # collect unique masses
    ALL_MASSES = []
    for m in OPT_MASSES + EVAL_MASSES:
        if m not in ALL_MASSES:
            ALL_MASSES.append(m)

    for m in ALL_MASSES:

        # ----------------------------------
        # DEFINE SIGNAL SAMPLE NAME
        # ----------------------------------
        if m.startswith("DYVBF_"):
            sig_sample = "HNL_DYVBF_" + m.split("_")[1]
        else:
            raise RuntimeError("Unknown mass format: " + m)

        sig_hist_all = []

        # ----------------------------------
        # LOOP OVER ERAS
        # ----------------------------------
        for era in ERAS:

            fname = os.path.join(
                sel, era,
                f"HNL_SignalRegion_Plotter_{sig_sample}.root"
            )

            f = ROOT.TFile(fname)

            print("[DEBUG SIGNAL FILE]", era, fname,
                  "OK" if f and not f.IsZombie() else "MISSING")

            if not f or f.IsZombie():
                raise RuntimeError("[ERROR] Missing file: " + fname)

            h_tmp = f.Get(path)

            print("[DEBUG SIGNAL]", era, path,
                  "OK" if h_tmp else "MISSING")

            if not h_tmp:
                raise RuntimeError("[ERROR] Missing hist in " + fname)

            # clone while file open
            h = h_tmp.Clone()
            h.SetDirectory(0)

            f.Close()

            sig_hist_all.append(h)

        # ----------------------------------
        # SAFETY CHECK
        # ----------------------------------
        if len(sig_hist_all) == 0:
            print("[ERROR] No histograms loaded for mass:", m)
            continue

        # ----------------------------------
        # COMBINE ERAS
        # ----------------------------------
        h_sig = sig_hist_all[0].Clone()

        for h in sig_hist_all[1:]:
            h_sig.Add(h)

        # ----------------------------------
        # FINAL CHECK
        # ----------------------------------
        if not hasattr(h_sig, "GetNbinsX"):
            print("[ERROR] Invalid ROOT object after combine for mass:", m)
            continue

        # ----------------------------------
        # NORMALISE
        # ----------------------------------
        if h_sig.Integral() > 0:
            h_sig.Scale(1.0 / h_sig.Integral())

        sig_hist[m] = h_sig

        print(f"[INFO] Loaded signal: {m} -> {sig_sample}")

    print("\n[DEBUG] Final loaded masses:", list(sig_hist.keys()))

    return sig_hist



def build_sig_cache(sig):

    cache = {}

    for m in sig:

        if sig[m] is None:
            print("[ERROR] None histogram for mass:", m)
            continue

        if not hasattr(sig[m], "GetNbinsX"):
            print("[ERROR] Invalid ROOT object for mass:", m)
            continue

        cache[m] = {
            sig[m].GetBinLowEdge(i): sig[m].GetBinContent(i)
            for i in range(1, sig[m].GetNbinsX()+1)
        }

    return cache


# =========================================================
# STAT CHECKS
# =========================================================
def check(sub, threshold):
    return sum(x[2] for x in sub) > threshold


# =========================================================
# SCAN
# =========================================================


def combine_flavours(all_bins):

    combined = {}

    for era in ERAS:

        combined[era] = []

        n_bins = len(all_bins[0][era])

        for i in range(n_bins):

            lo = all_bins[0][era][i][0]
            hi = all_bins[0][era][i][1]

            sum_bkg = sum(d[era][i][2] for d in all_bins)

            combined[era].append((lo, hi, sum_bkg, 0))

    return combined



# =========================================================
# PREDEFINED SCAN
# =========================================================


def scan_predefined_verbose(bins,
                           sig_cache,
                           mode,
                           all_bins,
                           fake_bins):

    print("\n==============================")
    print("[REFERENCE] Predefined bin scan (SR1 style)")
    print("==============================")

    # ----------------------------------
    # NEW boundaries: 300 -> 1000 step 50
    # ----------------------------------
    boundaries = list(range(300, 1050, 50))

    results = []

    # ----------------------------------
    # 6 internal boundaries
    # ----------------------------------
    for combo in itertools.combinations(boundaries, 6):

        b1, b2, b3, b4, b5, b6 = combo

        edges = [0, b1, b2, b3, b4, b5, b6, 2000]

        # -------------------------
        # Run worker (includes stat checks)
        # -------------------------
        r = worker((edges, bins, sig_cache, mode, all_bins, fake_bins))

        if not r:
            continue

        # -------------------------
        # FOM (Run2 style)
        # -------------------------
        per_era_tmp = evaluate_per_era(
            edges,
            bins,
            sig_cache,
            fake_bins
        )

        f_run = combine_per_era_fom(per_era_tmp)

        # -------------------------
        # FOM (Per-era style)
        # -------------------------
        f_per = evaluate_fixed_bins(
            edges,
            bins,
            sig_cache,
            fake_bins
        )

        # -------------------------
        # STAT CHECKS
        # -------------------------
        pass_run2 = check_run2_per_flavour(edges, all_bins)
        pass_per  = check_perera(edges, bins)

        # -------------------------
        # BKG per bin
        # -------------------------
        def get_bkg(edges, bins):
            vals = []
            for i in range(len(edges)-1):
                lo, hi = edges[i], edges[i+1]
                sub = [b for b in bins if lo <= b[0] < hi]
                vals.append(round(sum(x[2] for x in sub),2))
            return vals

        bkg = get_bkg(edges, list(bins.values())[0])

        results.append({
            "edges": edges,
            "f_run": f_run,
            "f_per": f_per,
            "pass_run2": pass_run2,
            "pass_per": pass_per,
            "bkg": bkg
        })

    # ----------------------------------
    # SORT
    # ----------------------------------
    results.sort(key=lambda x: x["f_run"], reverse=True)

    # ----------------------------------
    # PRINT
    # ----------------------------------
    for r in results[:20]:  # avoid insane printout

        status = ""
        if r["pass_run2"]:
            status = " ----> PASS STAT REQ"
        elif r["pass_per"]:
            status = " ----> PASS PER-ERA ONLY"

        print(
            "REFERENCE RESULT | "
            f"Edges {r['edges']} | "
            f"Bkg {r['bkg']} | "
            f"FOM_EraCombined {round(r['f_per'],2)} | "
            f"FOM_Run2 {round(r['f_run'],2)}"
            + status
        )

    best = results[0]

    return best["f_run"], best["edges"]




def scan_predefined_binnings(bins,
                            sig_cache,
                            mode,
                            all_bins,
                            fake_bins):

    print(f"\n[REFERENCE] Predefined ({mode})")

    # ----------------------------------
    # NEW boundaries
    # ----------------------------------
    boundaries = list(range(300, 1050, 50))

    best = (-1, None)

    # ----------------------------------
    # 6-bin scan
    # ----------------------------------
    for combo in itertools.combinations(boundaries, 6):

        b1, b2, b3, b4, b5, b6 = combo

        edges = [0, b1, b2, b3, b4, b5, b6, 2000]

        # -------------------------
        # Run worker
        # -------------------------
        r = worker((edges, bins, sig_cache, mode, all_bins, fake_bins))

        if not r:
            continue

        f = r[0]

        if f > best[0]:
            best = (f, edges)

    return best


def evaluate_per_era(edges, bins, cache, fake_bins):

    results = {}

    for era in ERAS:

        total = 0

        for m, c in cache.items():

            f = []

            for i in range(len(edges)-1):

                lo, hi = edges[i], edges[i+1]

                # background
                sub = [b for b in bins[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_bins[era])

                # signal
                sig = sum(v for x, v in c.items() if lo <= x < hi)

                f.append(fom(sig, bkg))

            total += sum(x*x for x in f)

        results[era] = math.sqrt(total)

    return results


# =========================================================
# LOAD
# =========================================================
def load_flavour(base, flav):

    sel = get_latest_dir(base)

    path = f"PassSR1/HNL_ULIDv2/{flav}/AK8/AK8J_Unbinned_Mass/l1J"

    hist = {}

    for era in ERAS:

        f = ROOT.TFile(os.path.join(sel, era, "HNL_SignalRegion_Plotter_Bkg.root"))

        h = f.Get(path)

        if not h:
            raise RuntimeError(f"[ERROR] Missing histogram: {path} in {era}")

        h = h.Clone()
        h.SetDirectory(0)

        hist[era] = get_bins(h)

        f.Close()

    return hist



def evaluate_variable_binning(edges_per_era,
                              bins,
                              cache,
                              fake_bins):

    total = 0

    for m, c in cache.items():

        f = []

        for era in ERAS:

            edges = edges_per_era[era]

            for i in range(len(edges)-1):

                lo, hi = edges[i], edges[i+1]

                # background
                sub = [b for b in bins[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_bins[era])

                # signal
                sig = sum(v for x, v in c.items() if lo <= x < hi)

                f.append(fom(sig, bkg))

        total += sum(x*x for x in f)

    return math.sqrt(total)


def format_edges(edges):
    return ",".join("{:.2f}".format(x) for x in edges[1:-1])


def evaluate_per_era_variable(edges_per_era,
                             bins,
                             cache,
                             fake_bins):

    results = {}

    for era in ERAS:

        total = 0

        edges = edges_per_era[era]

        for m, c in cache.items():

            f_bins = []

            for i in range(len(edges)-1):

                lo, hi = edges[i], edges[i+1]

                # background
                sub = [b for b in bins[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_bins[era])

                # signal
                sig = sum(v for x, v in c.items() if lo <= x < hi)

                f_bins.append(fom(sig, bkg))

            total += sum(x*x for x in f_bins)

        results[era] = math.sqrt(total)

    return results



def color_val(v):
    if v < 0.5:
        return f"{RED}{v:.2f}{RESET}"
    elif v < 5.0:
        return f"\033[93m{v:.2f}{RESET}"   # yellow
    else:
        return f"{GREEN}{v:.2f}{RESET}"

def fmt_colored(vals):
    return "[" + ", ".join(color_val(v) for v in vals) + "]"



def enforce_sr1_window(edges, bins_per_era, fake_per_era,use_per_era_stat):

    # compute combined bkg per bin using edges
    bkg_vals = []

    for i in range(len(edges)-1):
        lo, hi = edges[i], edges[i+1]

        b_sum = 0.0
        for era in ERAS:
            sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
            b_sum += correct_bkg(lo, hi, sub, fake_per_era[era], apply_fake_fix=False)

        bkg_vals.append(b_sum)

    # find last valid edge
    max_edge = edges[-1]

    bkg_vals_per_era = {era: [] for era in ERAS}

    for i in range(len(edges)-1):
        lo, hi = edges[i], edges[i+1]

        for era in ERAS:
            sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
            val = correct_bkg(lo, hi, sub, fake_per_era[era], apply_fake_fix=False)
            bkg_vals_per_era[era].append(val)
    
    for i in reversed(range(len(bkg_vals))):
        if use_per_era_stat:
            condition = all(bkg_vals_per_era[era][i] > STAT_THRESHOLD_PERERA for era in ERAS)
        else:
            condition = bkg_vals[i] > STAT_THRESHOLD_RUN2
        if condition:
            max_edge = edges[i+1]
            break

    return [e for e in edges if e <= max_edge]

def evaluate_run2_single_flavour(edges, bins_per_era, cache, fake_per_era):

    # --- enforce SAME SR1 window as DP
    edges = enforce_sr1_window(edges, bins_per_era, fake_per_era,USE_PER_ERA_STAT)

    total = 0.0

    for m, c in cache.items():

        for era in ERAS:

            f_bins = []

            for i in range(len(edges)-1):

                lo = edges[i]
                hi = edges[i+1]

                # background
                sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_per_era[era])

                # signal
                sig = sum(v for x, v in c.items() if lo <= x < hi)

                z = fom(sig, bkg)
                f_bins.append(z)

            total += sum(x*x for x in f_bins)

    return math.sqrt(total)



def make_mass_plot(mass, results, flav, tag=""):

    c = ROOT.TCanvas(f"c_{mass}_{flav}", "", 800, 700)

    g_global = ROOT.TGraph()
    g_perflav = ROOT.TGraph()
    g_sum_global = ROOT.TGraph()
    g_sum_perflav = ROOT.TGraph()
    g_era = ROOT.TGraph()
    g_era_run2 = ROOT.TGraph()
    # -----------------------------
    # Fill graphs (THIS flavour only)
    # -----------------------------
    for i, (n, f) in enumerate(results["global"][flav]):
        g_global.SetPoint(i, n, f)

    for i, (n, f) in enumerate(results["perflav"][flav]):
        g_perflav.SetPoint(i, n, f)

    for i, (n, f) in enumerate(results["sum_global"][flav]):
        g_sum_global.SetPoint(i, n, f)

    for i, (n, f) in enumerate(results["sum_perflav"][flav]):
        g_sum_perflav.SetPoint(i, n, f)

    for i, (n, f) in enumerate(results["era"][flav]):
        g_era.SetPoint(i, n, f)
    for i, (n, f) in enumerate(results["era_run2"][flav]):
        g_era_run2.SetPoint(i, n, f)

    # -----------------------------
    # Style
    # -----------------------------
    g_global.SetMarkerStyle(20)
    g_perflav.SetMarkerStyle(21)
    g_sum_global.SetMarkerStyle(22)
    g_sum_perflav.SetMarkerStyle(23)
    g_era.SetMarkerStyle(24)
    g_era_run2.SetMarkerStyle(25)
    
    g_perflav.SetLineStyle(2)
    g_sum_global.SetLineStyle(3)
    g_sum_perflav.SetLineStyle(4)
    g_era.SetLineStyle(5)
    g_era_run2.SetLineStyle(6)

    # -----------------------------
    # Draw
    # -----------------------------
    g_global.Draw("APL")
    g_perflav.Draw("PL SAME")
    g_sum_global.Draw("PL SAME")
    g_sum_perflav.Draw("PL SAME")
    g_era.Draw("PL SAME")
    g_era_run2.Draw("PL SAME")
    
    g_global.GetXaxis().SetTitle("N bins")
    g_global.GetYaxis().SetTitle("FOM")
    g_global.SetTitle(f"{mass} GeV - {flav}")

    # -----------------------------
    # Legend
    # -----------------------------
    leg = ROOT.TLegend(0.5,0.7,0.88,0.88)

    leg.AddEntry(g_global, "Mass-opt global", "lp")
    leg.AddEntry(g_perflav, "Mass-opt per flavour", "lp")
    leg.AddEntry(g_sum_global, "Summed global", "lp")
    leg.AddEntry(g_sum_perflav, "Summed per flavour", "lp")
    leg.AddEntry(g_era, "Era-by-era (sum2 FOM)", "lp")
    leg.AddEntry(g_era_run2, "Era-by-era (Run2 combined)", "lp")
    leg.Draw()

    os.makedirs("plots", exist_ok=True)
    c.SaveAs(f"plots/fom_vs_nbins_mass_{int(mass)}_{flav}_{tag}.pdf")





def make_global_dp_plot(results, tag):

    if not results:
        print("[WARNING] No valid global DP results - skipping plot")
        return
    
    c = ROOT.TCanvas("c_global","",800,700)

    g = ROOT.TGraph()

    for i, (n, fom) in enumerate(results):
        g.SetPoint(i, n, fom)

    g.SetMarkerStyle(20)
    g.SetMarkerSize(1.3)
    g.SetLineWidth(2)

    g.Draw("APL")

    g.GetXaxis().SetTitle("N bins")
    g.GetYaxis().SetTitle("Global DP FOM")
    g.SetTitle("Global SR2 DP: FOM vs N bins")

    # -----------------------------
    # Auto Y range
    # -----------------------------
    ymin = min(y for _, y in results)
    ymax = max(y for _, y in results)

    if ymin < ymax:
        g.SetMinimum(ymin * 0.9)
        g.SetMaximum(ymax * 1.2)

    # -----------------------------
    # Draw values
    # -----------------------------
    txt = ROOT.TLatex()
    txt.SetTextSize(0.03)
    txt.SetTextAlign(22)

    for n, v in results:
        txt.DrawLatex(n, v * 1.02, "{:.3f}".format(v))

    os.makedirs("plots", exist_ok=True)
    c.SaveAs(f"plots/global_dp_fom_vs_nbins_{tag}.pdf")
    

    
    
def make_mass_comparison_plot(results):

    c = ROOT.TCanvas("c_mass","",900,700)

    g_mass = ROOT.TGraph()
    g_global = ROOT.TGraph()

    for i,(m,f_mass,f_global) in enumerate(results):
        g_mass.SetPoint(i, m, f_mass)
        g_global.SetPoint(i, m, f_global)

    g_mass.SetMarkerStyle(20)
    g_mass.SetLineWidth(2)

    g_global.SetMarkerStyle(24)
    g_global.SetLineStyle(2)
    g_global.SetLineWidth(2)

    g_mass.Draw("APL")
    g_global.Draw("PL SAME")

    g_mass.GetXaxis().SetTitle("Mass [GeV]")
    g_mass.GetYaxis().SetTitle("FOM")

    leg = ROOT.TLegend(0.6,0.75,0.88,0.88)
    leg.AddEntry(g_mass,"Mass-optimised bins","lp")
    leg.AddEntry(g_global,"Global bins","lp")
    leg.Draw()

    os.makedirs("plots", exist_ok=True)
    c.SaveAs("plots/fom_vs_mass_comparison.pdf")
    
# =========================================================
# MAIN
# =========================================================



def main():

    global OPT_MASSES, EVAL_MASSES
    global USE_PER_ERA_STAT
    global DEBUG
    VALID_CACHE.clear()
    
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", action="store_true",
                    help="Run debug printout and exit")
    parser.add_argument("--EraStatCheck", action="store_true",
                        help="Require stat threshold per era instead of Run2")
    args = parser.parse_args()
    USE_PER_ERA_STAT = args.EraStatCheck

    DEBUG = args.debug
    
    print("\n[CONFIG]")
    print(" Stat mode =", "Per-era" if USE_PER_ERA_STAT else "Run2")
    print()
    
    DYVBF_MASSES = [
        "300","350","400","450","500",
        "600","700","800","900",
        "1000","1500","2000"
    ]

    # testing
    DYVBF_MASSES = ["700"]

    OPT_MASSES = ["DYVBF_" + m for m in DYVBF_MASSES]
    EVAL_MASSES = OPT_MASSES

    base = "/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"

    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs", exist_ok=True)

    tag = "perEra" if USE_PER_ERA_STAT else "run2"
    
    sys.stdout = TeeLogger(f"logs/scan_{tag}_{ts}.txt")

    # ----------------------------------
    # LOAD
    # ----------------------------------
    all_bins = []
    all_sig  = []
    all_fake = []

    
    for flav in FLAVOURS:

        print("\n[LOAD]", flav)

        bins = load_flavour(base, flav)
        sig  = load_signals(base, flav)
        fake = load_fake(base, flav)

        all_bins.append(bins)
        all_sig.append(sig)
        all_fake.append(fake)


        
    # ----------------------------------
    # COMBINE SIGNALS (global)
    # ----------------------------------
    combined_sig = {}

    for m in OPT_MASSES:

        h = None

        for sig in all_sig:
            if m not in sig:
                continue

            if h is None:
                h = sig[m].Clone()
            else:
                h.Add(sig[m])

        if h and h.Integral() > 0:
            h.Scale(1.0 / h.Integral())

        combined_sig[m] = h

    combined_cache = build_sig_cache(combined_sig)
    global_cache = combined_cache

    # ----------------------------------
    # GLOBAL DP PER FLAVOUR (used later)
    # ----------------------------------
    best_global_flav = {}

    for i_flav, flav in enumerate(FLAVOURS):

        print("\n==============================")
        print(" GLOBAL DP (FLAVOUR)", flav)
        print("==============================")

        best_global_flav[flav] = {}

        for n in tqdm(TOTAL_BINS_TO_SCAN, desc=f"{flav} DP"):

            best_edges_global = run_global_dp(
                [all_bins[i_flav]],
                [all_fake[i_flav]],
                global_cache,
                n
            )

            best_global_flav[flav][n] = best_edges_global

    # ----------------------------------
    # MASS LOOP
    # ----------------------------------
    results_all = {}
    for mass in tqdm(OPT_MASSES, desc="Mass loop"):
        results_mass = {}

        results_mass[mass] = {
            "global": {flav: [] for flav in FLAVOURS},
            "perflav": {flav: [] for flav in FLAVOURS},
            "sum_global": {flav: [] for flav in FLAVOURS},
            "sum_perflav": {flav: [] for flav in FLAVOURS},
            "era": {flav: [] for flav in FLAVOURS},
            "era_run2": {flav: [] for flav in FLAVOURS},
        }
        
        print("\n==============================")
        print(" MASS =", mass)
        print("==============================")

        single_cache_global = build_sig_cache({
            mass: combined_sig[mass]
        })

        # store per-flavour results
        fom_vs_n_mass_flav = {flav: [] for flav in FLAVOURS}
        fom_vs_n_global_flav = {flav: [] for flav in FLAVOURS}

        # ----------------------------------
        # LOOP OVER N BINS
        # ----------------------------------
        for n in tqdm(TOTAL_BINS_TO_SCAN, desc=f"{mass}", leave=False):

            # =========================
            # SCENARIO 1: mass-opt global
            # =========================
            best_global = run_global_dp(all_bins, all_fake, single_cache_global, n)
            if best_global[1] is None:
                continue
            edges_global = best_global[1]
            
            combined_bins = combine_flavours(all_bins)
            combined_fake = combine_flavours(all_fake)

            for i_flav, flav in enumerate(FLAVOURS):

                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                cache_flav = build_sig_cache({
                    mass: all_sig[i_flav][mass]
                })
                
                f = evaluate_run2(edges_global, bins_f, cache_flav, fake_f)
                
                results_mass[mass]["global"][flav].append((n, f))

            # =========================
            # SCENARIO 2: mass-opt per flavour
            # =========================
            f_perflav_total = 0.0

            for i_flav, flav in enumerate(FLAVOURS):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                cache_flav = build_sig_cache({mass: all_sig[i_flav][mass]})

                best_flav = run_global_dp([bins_f], [fake_f], cache_flav, n)
                if best_flav[1] is None:
                    continue
                
                edges_flav = best_flav[1]
                
                f_flav = evaluate_run2(edges_flav, bins_f, cache_flav, fake_f)
                
                results_mass[mass]["perflav"][flav].append((n, f_flav))
                
                f_perflav_total += f_flav * f_flav

            f_perflav = math.sqrt(f_perflav_total)

            # =========================
            # SCENARIO 3: summed global
            # =========================
            res = run_global_dp(all_bins, all_fake, combined_cache, n)

            if res[1] is None:
                continue

            edges_sum_global = res[1]
            
            for i_flav, flav in enumerate(FLAVOURS):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                cache_flav = build_sig_cache({
                    mass: all_sig[i_flav][mass]
                })
                
                f = evaluate_run2(edges_sum_global, bins_f, cache_flav, fake_f)
                
                results_mass[mass]["sum_global"][flav].append((n, f))
            #f_sum_global = evaluate_run2(edges_sum_global, combined_bins, combined_cache, combined_fake)
            
            # =========================
            # SCENARIO 4: summed per flavour
            # =========================
            f_sum_flav_total = 0.0
            
            for i_flav, flav in enumerate(FLAVOURS):

                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]

                # optimisation
                cache_flav_sum = build_sig_cache({
                    m: all_sig[i_flav][m] for m in OPT_MASSES
                })
                edges_flav_sum = run_global_dp([bins_f], [fake_f], cache_flav_sum, n)[1]

                
                # evaluation (FIX HERE)
                cache_eval = build_sig_cache({
                    mass: all_sig[i_flav][mass]
                })
                
                f_flav_sum = evaluate_run2(edges_flav_sum, bins_f, cache_eval, fake_f)
                results_mass[mass]["sum_perflav"][flav].append((n, f_flav_sum))
                
                f_sum_flav_total += f_flav_sum * f_flav_sum

            f_sum_flav = math.sqrt(f_sum_flav_total)

            # =========================
            # SCENARIO 5: era-by-era
            # =========================
            edges_per_era = {}
            
            for era in ERAS:
                res = run_global_dp(
                    [{era: all_bins[f][era] for era in ERAS} for f in range(len(FLAVOURS))],
                    [{era: all_fake[f][era] for era in ERAS} for f in range(len(FLAVOURS))],
                    single_cache_global,
                    n
                )
                
                if res[1] is None:
                    continue

                edges_per_era[era] = res[1]

            for i_flav, flav in enumerate(FLAVOURS):

                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                cache_flav = build_sig_cache({
                    mass: all_sig[i_flav][mass]
                })

                f = evaluate_variable_binning_run2(
                    edges_per_era,
                    combine_flavours(all_bins),
                    single_cache_global,
                    combine_flavours(all_fake)
                )
                results_mass[mass]["era_run2"][flav].append((n, f))

            # =========================
            # STORE
            # =========================
            for i_flav, flav in enumerate(FLAVOURS):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                cache_flav = build_sig_cache({
                    mass: all_sig[i_flav][mass]
                })
                
                f = evaluate_variable_binning(edges_per_era, bins_f, cache_flav, fake_f)
                
                results_mass[mass]["era"][flav].append((n, f))
                
        results_all[mass] = results_mass[mass]        

    
    print("[CHECK]")
    print("Scenario: sum_perflav")
    print("Optimisation masses:", list(cache_flav_sum.keys()))
    print("Evaluation masses:", list(cache_eval.keys()))

    # ----------------------------------
    # CLEAN SUMMARY PRINT
    # ----------------------------------
    print("\n\n==============================")
    print(" FINAL CLEAN SUMMARY")
    print("==============================")
    
    for mass in results_all:
        
        for n in TOTAL_BINS_TO_SCAN:
            
            print_mass_summary(
                mass,
                results_all[mass],
                n
            )
    
    # ----------------------------------
    # GLOBAL SUMMARY (ALL 5 SCENARIOS)
    # ----------------------------------
    print("\n==============================")
    print(" GLOBAL SUMMARY (ALL SCENARIOS)")
    print("==============================")
    
    for n in TOTAL_BINS_TO_SCAN:
        
        print(f"\n================ n = {n} ================")
        
        # =========================
        # Scenario 1: global (mass-opt)
        # =========================
        res = run_global_dp(all_bins, all_fake, combined_cache, n)
        if res[1] is None:
            continue
        edges_global = res[1]
        
        print("\n[Scenario] GLOBAL")
        print("Edges =", fmt(edges_global))
        
        # =========================
        # Scenario 2: per flavour
        # =========================
        edges_perflav = {}
        
        for i_flav, flav in enumerate(FLAVOURS):
            
            cache_flav_sum = build_sig_cache({
                m: all_sig[i_flav][m] for m in OPT_MASSES
            })
            
            res = run_global_dp([all_bins[i_flav]], [all_fake[i_flav]], cache_flav_sum, n)
            
            if res[1] is None:
                continue

            edges_perflav[flav] = res[1]

        print("\n[Scenario] PER-FLAVOUR")
        for flav in edges_perflav:
            print(f"  {flav} -> {fmt(edges_perflav[flav])}")
            
        # =========================
        # Scenario 3: summed global
        # =========================
        edges_sum_global = edges_global  # identical if 1 mass
        
        print("\n[Scenario] SUM GLOBAL")
        print("Edges =", fmt(edges_sum_global))
        
        # =========================
        # Scenario 4: summed per flavour
        # =========================
        edges_sum_perflav = edges_perflav  # identical if 1 mass
        
        print("\n[Scenario] SUM PER-FLAVOUR")
        for flav in edges_sum_perflav:
            print(f"  {flav} -> {fmt(edges_sum_perflav[flav])}")
            

        # =========================
        # Scenario 5: era-by-era
        # =========================
        edges_per_era = {}
    
        for era in ERAS:
            
            bins_era = []
            fake_era = []
            
            for f in range(len(FLAVOURS)):
                
                bins_tmp = {}
                fake_tmp = {}
                
                for e in ERAS:
                    if e == era:
                        bins_tmp[e] = all_bins[f][e]
                        fake_tmp[e] = all_fake[f][e]
                    else:
                        ref_bins = all_bins[f][era]
                        ref_fake = all_fake[f][era]
                        
                        bins_tmp[e] = [(b[0], b[1], 0.0, 999) for b in ref_bins]
                        fake_tmp[e] = [(b[0], b[1], 0.0, 999) for b in ref_fake]

                bins_era.append(bins_tmp)
                fake_era.append(fake_tmp)

            res = run_global_dp(bins_era, fake_era, combined_cache, n)

            if res[1] is None:
                continue

            edges_per_era[era] = res[1]

        print("\n[Scenario] ERA-BY-ERA")
        for era in edges_per_era:
            print(f"  {era} -> {fmt(edges_per_era[era])}")

        # =========================
        # EVALUATION (ALWAYS SINGLE MASS)
        # =========================
        print("\n--- Evaluation (single mass) ---")
        
        for mass in OPT_MASSES:
            
            print(f"\n Mass = {mass}")
            
            cache_eval_global = build_sig_cache({
                mass: combined_sig[mass]
            })
            
            # ---- GLOBAL
            f_global = evaluate_run2(
                edges_global,
                combine_flavours(all_bins),
                cache_eval_global,
                combine_flavours(all_fake)
            )
            print("  GLOBAL =", round(f_global,4))

            # ---- PER FLAVOUR
            f_pf = 0.0
            for i_flav, flav in enumerate(FLAVOURS):
                
                cache_eval = build_sig_cache({
                    mass: all_sig[i_flav][mass]
                })
                
                f = evaluate_run2(
                    edges_perflav[flav],
                    all_bins[i_flav],
                    cache_eval,
                    all_fake[i_flav]
                )
                
                f_pf += f*f

            print("  PER-FLAVOUR =", round(math.sqrt(f_pf),4))

            # ---- SUM GLOBAL
            f_sumg = f_global
            print("  SUM GLOBAL =", round(f_sumg,4))
            
            # ---- SUM PER FLAVOUR
            f_spf = f_pf
            print("  SUM PER-FLAVOUR =", round(math.sqrt(f_spf),4))

            # ---- ERA
            f_era = evaluate_variable_binning(
                edges_per_era,
                combine_flavours(all_bins),
                cache_eval_global,
                combine_flavours(all_fake)
            )
            print("  ERA =", round(f_era,4))
            print("  ERA (Run2 combined) =", round(f_era_run2,4))
            
    # ----------------------------------
    # Plot
    # ----------------------------------
    #make_global_dp_plot(results_global,tag)
    # ----------------------------------                                                                                            
    # PLOTTING (PER FLAVOUR)                                                                                                                 
    # ----------------------------------                                                                                                        
    for mass in results_all:

        for flav in FLAVOURS:

            make_mass_plot(
                float(mass.split("_")[1]),
                results_all[mass],
                flav,
                tag
            )

if __name__=="__main__":
    main()



