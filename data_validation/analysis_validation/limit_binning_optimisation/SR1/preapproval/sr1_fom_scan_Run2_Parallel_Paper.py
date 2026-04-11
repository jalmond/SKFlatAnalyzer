#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from tqdm import tqdm
import ctypes
import time


DEBUG=False

ROOT.gROOT.SetBatch(True)
USE_PER_ERA_STAT = False
RED   = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"

STAT_THRESHOLD_PERERA = 0.5
STAT_THRESHOLD_RUN2  = 1.0

TOTAL_BINS_TO_SCAN = [2,3,4,5,6,7]

VALID_CACHE = {}

REF_BINS = {

    "2016preVFP": {
        "MuMu": {
            "400": [0.0, 270.0, 340.0, 385.0, 465.0, 555.0, 730.0, 5000.0],
            "450": [0.0, 330.0, 385.0, 435.0, 515.0, 580.0, 740.0, 5000.0],
            "500": [0.0, 330.0, 385.0, 430.0, 485.0, 550.0, 730.0, 5000.0],
            "600": [0.0, 115.0, 355.0, 430.0, 485.0, 565.0, 690.0, 5000.0],
            "700": [0.0, 120.0, 320.0, 360.0, 445.0, 530.0, 620.0, 5000.0],
            "800": [0.0, 115.0, 160.0, 265.0, 435.0, 570.0, 730.0, 5000.0],
            "900": [0.0, 115.0, 150.0, 310.0, 445.0, 570.0, 740.0, 5000.0],
        },
        "EE": {
            "400": [0.0, 365.0, 445.0, 520.0, 590.0, 670.0, 750.0, 5000.0],
            "450": [0.0, 365.0, 435.0, 455.0, 490.0, 590.0, 750.0, 5000.0],
            "500": [0.0, 365.0, 455.0, 500.0, 565.0, 670.0, 750.0, 5000.0],
            "600": [0.0, 500.0, 550.0, 595.0, 655.0, 750.0, 930.0, 5000.0],
            "700": [0.0, 475.0, 550.0, 595.0, 625.0, 685.0, 780.0, 5000.0],
            "800": [0.0, 360.0, 520.0, 590.0, 635.0, 750.0, 980.0, 5000.0],
            "900": [0.0, 345.0, 475.0, 560.0, 635.0, 750.0, 980.0, 5000.0],
        },
        "EMu": {
            "400": [0.0, 335.0, 405.0, 440.0, 550.0, 595.0, 865.0, 5000.0],
            "450": [0.0, 410.0, 450.0, 490.0, 550.0, 595.0, 865.0, 5000.0],
            "500": [0.0, 410.0, 450.0, 480.0, 550.0, 595.0, 770.0, 5000.0],
            "600": [0.0, 410.0, 510.0, 580.0, 690.0, 765.0, 950.0, 5000.0],
            "700": [0.0, 410.0, 510.0, 590.0, 660.0, 805.0, 950.0, 5000.0],
            "800": [0.0, 115.0, 410.0, 510.0, 620.0, 730.0, 885.0, 5000.0],
            "900": [0.0, 415.0, 465.0, 585.0, 665.0, 830.0, 1010.0, 5000.0],
        }
    },

    "2016postVFP": {
        "MuMu": {
            "400": [0.0, 350.0, 395.0, 445.0, 505.0, 565.0, 765.0, 5000.0],
            "450": [0.0, 330.0, 375.0, 425.0, 495.0, 550.0, 685.0, 5000.0],
            "500": [0.0, 215.0, 345.0, 400.0, 460.0, 545.0, 685.0, 5000.0],
            "600": [0.0, 205.0, 355.0, 425.0, 500.0, 565.0, 765.0, 5000.0],
            "700": [0.0, 125.0, 350.0, 395.0, 460.0, 540.0, 650.0, 5000.0],
            "800": [0.0, 135.0, 195.0, 410.0, 460.0, 565.0, 730.0, 5000.0],
            "900": [0.0, 130.0, 345.0, 400.0, 460.0, 565.0, 735.0, 5000.0],
        },
        "EE": {
            "400": [0.0, 340.0, 385.0, 440.0, 555.0, 710.0, 1010.0, 5000.0],
            "450": [0.0, 330.0, 420.0, 495.0, 565.0, 825.0, 1095.0, 5000.0],
            "500": [0.0, 330.0, 435.0, 480.0, 555.0, 825.0, 1125.0, 5000.0],
            "600": [0.0, 465.0, 530.0, 585.0, 645.0, 705.0, 825.0, 5000.0],
            "700": [0.0, 420.0, 515.0, 615.0, 680.0, 765.0, 875.0, 5000.0],
            "800": [0.0, 490.0, 565.0, 600.0, 690.0, 780.0, 940.0, 5000.0],
            "900": [0.0, 485.0, 565.0, 610.0, 715.0, 825.0, 1030.0, 5000.0],
        },
        "EMu": {
            "400": [0.0, 350.0, 385.0, 450.0, 675.0, 805.0, 880.0, 5000.0],
            "450": [0.0, 370.0, 425.0, 500.0, 675.0, 805.0, 880.0, 5000.0],
            "500": [0.0, 425.0, 475.0, 510.0, 545.0, 590.0, 675.0, 5000.0],
            "600": [0.0, 515.0, 550.0, 600.0, 685.0, 805.0, 880.0, 5000.0],
            "700": [0.0, 385.0, 550.0, 605.0, 670.0, 780.0, 880.0, 5000.0],
            "800": [0.0, 405.0, 550.0, 605.0, 665.0, 765.0, 870.0, 5000.0],
            "900": [0.0, 415.0, 525.0, 605.0, 675.0, 830.0, 1005.0, 5000.0],
        }
    },

    "2017": {
        "MuMu": {
            "400": [0.0, 335.0, 380.0, 420.0, 470.0, 505.0, 540.0, 5000.0],
            "450": [0.0, 380.0, 430.0, 480.0, 505.0, 540.0, 755.0, 5000.0],
            "500": [0.0, 430.0, 485.0, 520.0, 565.0, 755.0, 930.0, 5000.0],
            "600": [0.0, 430.0, 510.0, 545.0, 595.0, 710.0, 930.0, 5000.0],
            "700": [0.0, 420.0, 500.0, 520.0, 575.0, 630.0, 745.0, 5000.0],
            "800": [0.0, 430.0, 470.0, 540.0, 610.0, 755.0, 925.0, 5000.0],
            "900": [0.0, 385.0, 435.0, 480.0, 555.0, 625.0, 825.0, 5000.0],
        },
        "EE": {
            "400": [0.0, 330.0, 390.0, 435.0, 480.0, 595.0, 885.0, 5000.0],
            "450": [0.0, 395.0, 445.0, 500.0, 615.0, 690.0, 720.0, 5000.0],
            "500": [0.0, 445.0, 490.0, 560.0, 690.0, 720.0, 885.0, 5000.0],
            "600": [0.0, 545.0, 595.0, 655.0, 700.0, 730.0, 905.0, 5000.0],
            "700": [0.0, 615.0, 670.0, 720.0, 790.0, 885.0, 1085.0, 5000.0],
            "800": [0.0, 545.0, 620.0, 710.0, 770.0, 870.0, 1085.0, 5000.0],
            "900": [0.0, 615.0, 690.0, 725.0, 795.0, 885.0, 1075.0, 5000.0],
        },
        "EMu": {
            "400": [0.0, 355.0, 385.0, 410.0, 445.0, 490.0, 530.0, 5000.0],
            "450": [0.0, 410.0, 450.0, 490.0, 515.0, 535.0, 915.0, 5000.0],
            "500": [0.0, 410.0, 450.0, 480.0, 525.0, 570.0, 915.0, 5000.0],
            "600": [0.0, 450.0, 530.0, 585.0, 650.0, 765.0, 915.0, 5000.0],
            "700": [0.0, 530.0, 640.0, 690.0, 765.0, 835.0, 970.0, 5000.0],
            "800": [0.0, 530.0, 675.0, 725.0, 800.0, 885.0, 1075.0, 5000.0],
            "900": [0.0, 440.0, 530.0, 660.0, 735.0, 855.0, 1030.0, 5000.0],
        }
    },

    "2018": {
        "MuMu": {
            "400": [0.0, 365.0, 395.0, 435.0, 460.0, 480.0, 850.0, 5000.0],
            "450": [0.0, 370.0, 420.0, 455.0, 495.0, 520.0, 850.0, 5000.0],
            "500": [0.0, 395.0, 455.0, 480.0, 525.0, 590.0, 850.0, 5000.0],
            "600": [0.0, 480.0, 540.0, 580.0, 635.0, 680.0, 850.0, 5000.0],
            "700": [0.0, 540.0, 600.0, 650.0, 710.0, 795.0, 850.0, 5000.0],
            "800": [0.0, 395.0, 480.0, 580.0, 670.0, 755.0, 915.0, 5000.0],
            "900": [0.0, 480.0, 540.0, 610.0, 670.0, 755.0, 850.0, 5000.0],
        },
        "EE": {
            "400": [0.0, 325.0, 390.0, 430.0, 465.0, 490.0, 905.0, 5000.0],
            "450": [0.0, 395.0, 450.0, 475.0, 520.0, 545.0, 905.0, 5000.0],
            "500": [0.0, 390.0, 460.0, 490.0, 530.0, 560.0, 905.0, 5000.0],
            "600": [0.0, 500.0, 570.0, 605.0, 645.0, 680.0, 905.0, 5000.0],
            "700": [0.0, 580.0, 630.0, 675.0, 710.0, 785.0, 905.0, 5000.0],
            "800": [0.0, 580.0, 710.0, 760.0, 845.0, 1015.0, 1150.0, 5000.0],
            "900": [0.0, 545.0, 670.0, 735.0, 800.0, 905.0, 1080.0, 5000.0],
        },
        "EMu": {
            "400": [0.0, 335.0, 385.0, 440.0, 535.0, 640.0, 665.0, 5000.0],
            "450": [0.0, 385.0, 445.0, 485.0, 510.0, 640.0, 665.0, 5000.0],
            "500": [0.0, 445.0, 470.0, 495.0, 555.0, 640.0, 665.0, 5000.0],
            "600": [0.0, 495.0, 540.0, 595.0, 640.0, 865.0, 960.0, 5000.0],
            "700": [0.0, 540.0, 620.0, 665.0, 710.0, 790.0, 1085.0, 5000.0],
            "800": [0.0, 535.0, 665.0, 705.0, 740.0, 895.0, 1085.0, 5000.0],
            "900": [0.0, 535.0, 665.0, 730.0, 870.0, 990.0, 1165.0, 5000.0],
        }
    }
}


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

    for m, c in cache.items():

        f_bins_all = []

        # ----------------------------------
        # LOOP OVER ERAS (each with its own binning)
        # ----------------------------------
        for era in edges_per_era:

            edges = edges_per_era[era]

            for i in range(len(edges) - 1):

                lo = edges[i]
                hi = edges[i + 1]

                # --- background (this era only)
                sub = [b for b in bins[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_bins[era])

                # --- signal (same global cache, filtered by bin)
                sig = sum(v for x, v in c.items() if lo <= x < hi)

                z = fom(sig, bkg)
                f_bins_all.append(z)

        # ----------------------------------
        # Combine all bins (across eras) in quadrature
        # ----------------------------------
        total += sum(z * z for z in f_bins_all)

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



def get_ref_mass_key(mass):
    m = int(mass)

    if m >= 1000:
        return "900"
    elif m < 400:
        return "400"   # <-- ADD THIS
    else:
        return str(m)

def evaluate_reference_single_flavour(all_bins, all_fake, all_sig, mass, i_flav):

    bins_f = all_bins[i_flav]
    fake_f = all_fake[i_flav]

    h = None
    for era in ERAS:
        h_era = all_sig[i_flav][mass][era]
        if h is None:
            h = h_era.Clone()
        else:
            h.Add(h_era)

    if h.Integral() > 0:
        h.Scale(1.0 / h.Integral())
            
    cache = build_sig_cache({mass: h})

    edges_per_era = {}
    for era in ERAS:
        mass_key = get_ref_mass_key(mass.split("_")[1])
        edges_per_era[era] = REF_BINS[era][FLAVOURS[i_flav]][mass_key]

    return evaluate(
        None,
        bins_f,
        cache,
        fake_f,
        edges_per_era=edges_per_era
    )



def print_best_binning_summary(edges, bins_per_era, cache_per_era, fake_bins,
                               label="", mass=None, n_bins=None, flavour=None):

    
    print("\n====================================")
    if flavour:
        title = f" BEST BINNING SUMMARY: {label} {flavour}"
    else:
        title = f" BEST BINNING SUMMARY: {label}"
            
    if mass is not None:
        title += f" (Mass = {mass})"
        if n_bins is not None:
            title += f" | n = {n_bins}"

    print(title)
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

            # background
            sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
            bkg = correct_bkg(lo, hi, sub, fake_bins[era])

            # signal
            sig = 0.0
            for m, c in cache_per_era[era].items():
                for x, v in c.items():
                    if lo <= x < hi:
                        sig += v

            z = fom(sig, bkg)

            # accumulate Run2
            run2_B[i] += bkg
            run2_S[i] += sig

            # color logic
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
    # RUN2 COMBINED
    # ----------------------------------
    print("\n  RUN2 COMBINED")

    Z2_sum = 0.0

    for i in range(len(edges)-1):

        lo = edges[i]
        hi = edges[i+1]

        B = run2_B[i]
        S = run2_S[i]

        Z = fom(S, B)
        Z2_sum += Z * Z

        b_str = f"{RED}{B:.3f}{RESET}" if B < 0.5 else f"{B:.3f}"

        print(
            "    BIN [{:.1f}, {:.1f}] | B = {} | S = {:.3f} | Z = {:.3f}".format(
                lo, hi, b_str, S, Z
            )
        )

    # ----------------------------------
    # ERA QUADRATURE
    # ----------------------------------
    print("\n  RUN2 Era Quad")

    Z2_total = 0.0

    for i in range(len(edges)-1):

        lo = edges[i]
        hi = edges[i+1]

        B = run2_B[i]
        S = run2_S[i]

        Z2_bin = 0.0

        for era in ERAS:

            sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
            bkg = correct_bkg(lo, hi, sub, fake_bins[era])

            sig = 0.0
            for m, c in cache_per_era[era].items():
                for x, v in c.items():
                    if lo <= x < hi:
                        sig += v

            z = fom(sig, bkg)
            Z2_bin += z * z

        Z_quad = math.sqrt(Z2_bin)
        Z2_total += Z2_bin   # NOTE: accumulate Z^2 directly

        b_str = f"{RED}{B:.3f}{RESET}" if B < 0.5 else f"{B:.3f}"

        print(
            "    BIN [{:.1f}, {:.1f}] | B = {} | S = {:.3f} | Z = {:.3f}".format(
                lo, hi, b_str, S, Z_quad
            )
        )

    print("\n  [ERA QUAD CHECK]")
    print("    sqrt(sum(Z^2)) =", round(math.sqrt(Z2_total), 6))

    # ----------------------------------
    # FINAL CHECK
    # ----------------------------------
    print("\n  [CHECK]")
    print("    sum(Z^2) =", round(Z2_sum, 6))
    print("    sqrt(sum(Z^2)) =", round(math.sqrt(Z2_sum), 6))


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

    scenarios = ["global", "perflav", "permass", "sum_global", "sum_perflav", "era_quadrature", "run2_from_era_binning"]

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

def print_scan_window(edges, min_idx, max_idx, tag=""):
    print(
        f"[{tag}][SCAN WINDOW] "
        f"[{edges[0]:.1f}, {edges[-1]:.1f}] --> "
        f"[{edges[min_idx]:.1f}, {edges[max_idx]:.1f}]"
    )


def debug_dp_failure(edges, valid, PB, PE, PB_era_flav, PS_mass):

    print("\n--- DEBUG DP FAILURE ---")

    M = len(edges) - 1

    # -----------------------------
    # 1. Check valid matrix density
    # -----------------------------
    total = (M+1)*(M+1)
    n_valid = valid.sum()

    print(f"[VALID MATRIX] {n_valid}/{total} = {n_valid/total:.6f}")

    if n_valid == 0:
        print(">>> NO VALID INTERVALS AT ALL <<<")

    # -----------------------------
    # 2. Check per-bin background
    # -----------------------------
    print("\n[PER-BIN BACKGROUND]")

    for i in range(M):
        b = PB[i+1] - PB[i]
        print(f"  bin {i}: edge [{edges[i]:.1f}, {edges[i+1]:.1f}] -> B = {b:.4f}")

    # -----------------------------
    # 3. Check tail behaviour
    # -----------------------------
    print("\n[TAIL BACKGROUND]")

    for i in range(M):
        tail = sum(
            PB_era_flav[f][era][M] - PB_era_flav[f][era][i]
            for f in range(len(PB_era_flav))
            for era in PB_era_flav[f]
        )
        print(f"  edge {edges[i]:.1f} -> tail = {tail:.4f}")

    # -----------------------------
    # 4. Check signal presence
    # -----------------------------
    print("\n[SIGNAL CHECK]")

    for m in PS_mass:
        total_s = PS_mass[m][-1]
        print(f"  mass {m} total S = {total_s:.6f}")
    

def pass_stat(B, rel):
    return (B >= 1.0) or (B >= 0.5 and rel < 0.3)


def dp_global(edges, PB, PE, PS_mass,
              PB_era, PE_era,
              PB_era_flav,PE_era_flav, PF_era_flav,
              n_bins, use_per_era_stat,no_scan_window,norm=None, tag="DP"):

    tag=f"{tag}|masses={list(PS_mass.keys())}"
    
    def interval_bkg_fast(p, i):
        return PB[i] - PB[p]

    def interval_bkg(p, i):
        
        b_sum = 0.0
        
        for f in range(len(PB_era_flav)):
            for era in PB_era_flav[f]:
                
                b = PB_era_flav[f][era][i] - PB_era_flav[f][era][p]
                fake = PF_era_flav[f][era][i] - PF_era_flav[f][era][p]
                
                if fake < FAKE_FLOOR:
                    b += (FAKE_FLOOR - fake)

                b_sum += b

        return b_sum

    print(f"[{tag}][DEBUG] active eras in DP:", list(PB_era.keys()))
    
    key = (
        tuple(edges),
        tuple(PB),
        tuple(PE),
        use_per_era_stat
    )

    if DEBUG:
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


    #    print(f"\n[{tag}][DEBUG] Valid transitions per era")

    for era in PB_era:

        count = 0
        
        for p in range(len(edges)):
            for i in range(p+1, len(edges)):
                
                B = PB_era[era][i] - PB_era[era][p]
                E = PE_era[era][i] - PE_era[era][p]
                
                rel = compute_rel_unc(B, E)

                ok = pass_stat(B, rel)
                if ok:
                    count += 1

        print(f"[{tag}] Era {era}: {count} valid intervals")
    
                    
    
    M = len(edges) - 1
    # rebuild per-bin bkg from prefix sums
    bkg = [PB[i+1] - PB[i] for i in range(M)]

    S_combined = np.zeros(len(edges))

    for m in PS_mass:
        S_combined += PS_mass[m]

    S_total = S_combined[-1]

    # ----------------------------------
    # FIND MIN EDGE INDEX
    # ----------------------------------
    min_edge_idx = 0

    for i in range(len(edges)-1):
        if S_total > 0 and (S_combined[i] / S_total) > 0.01:
            min_edge_idx = i
            break
        
    # ----------------------------------
    # HARD CAP (physics safeguard)
    # ----------------------------------
    if edges[min_edge_idx] > 500:
        print(f"[INFO][{tag}] Capping min edge from {edges[min_edge_idx]:.1f} -> 500.0")
        
        # find closest edge <= 500
        for i in range(len(edges)):
            if edges[i] >= 500:
                min_edge_idx = i
                break

    # ----------------------------------
    # DEBUG PRINT (only if still large)
    # ----------------------------------
    if edges[min_edge_idx] > 500:
        
        print(f"[WARNING][{tag}] High lower boundary detected: {edges[min_edge_idx]:.1f}")
        
        for i in range(min(10, len(edges)-1)):
            frac = S_combined[i] / S_total if S_total > 0 else 0.0
            print(f"edge {edges[i]:.1f} -> S_frac = {frac:.4f}")
        
    max_boundary = compute_last_valid_boundary(edges, PB_era_flav,PE_era_flav,use_per_era_stat)
    print(f"[{tag}][DEBUG] max_boundary = {max_boundary}")
    # ----------------------------------
    # FIND MAX EDGE INDEX
    # ----------------------------------
    max_edge_idx = len(edges) - 1
    
    for i in range(len(edges)):
        if edges[i] >= max_boundary:
            max_edge_idx = i
            break

    if no_scan_window:
        min_edge_idx = 0
        max_boundary = edges[-1]


    print_scan_window(edges, min_edge_idx, max_edge_idx,tag)
    print(
        f"[{tag}][SCAN REDUCTION] "
        f"{len(edges)-1} bins -> {max_edge_idx - min_edge_idx} bins"
    )

    
    NEG = -1e300

    dp  = np.full((n_bins + 1, M + 1), NEG, dtype=float)
    prv = np.full((n_bins + 1, M + 1), -1, dtype=int)

    dp[0, 0] = 0.0

    if DEBUG:
        print("[DEBUG] interval_fom...")
        
    # ----------------------------------
    # FOM for interval [p, i]
    # ----------------------------------
    def interval_fom_multi(p, i):
        
        B = interval_bkg(p, i)
        
        if B <= 0:
            return 0.0

        total = 0.0
        
        for m in PS_mass:
            
            S = PS_mass[m][i] - PS_mass[m][p]
    
            if S <= 0:
                continue
            
            val = 2.0 * ((S + B) * math.log(1.0 + S / B) - S)

            if val <= 0:
                continue
            
            z2 = val  # this is already Z^2

            if norm is not None:
                total += z2 / norm[m]
            else:
                total += z2

        return total   # IMPORTANT: returns Z^2, not sqrt

    # ----------------------------------
    # DP JOHN (CORRECTED)
    # ----------------------------------
    if DEBUG:
        print("[DEBUG] valid_transitions...")

    valid_transitions = {i: np.where(valid[:, i])[0] for i in range(M+1)}

    for j in range(1, n_bins + 1):
        for i in range(1, M + 1):
            
            best = NEG
            best_p = -1
            
            # ----------------------------------
            # Choose candidate p
            # ----------------------------------
            if j == 1:
                p_list = [0]   # first bin must start at 0
            else:
                p_list = valid_transitions[i]
            if j == 2 and DEBUG:
                print(f"[CHECK] i={i}, valid p:", valid_transitions[i])
            for p in p_list:

                
                if DEBUG:
                    print(f"[DP] j={j} i={i} ({edges[i]:.1f}) <- p={p} ({edges[p]:.1f})")

                # ----------------------------------
                # FIRST BIN: enforce scan window lower bound
                # ----------------------------------
                if j == 1 and i < min_edge_idx:
                    if DEBUG:
                        print("   -> rejected: first bin below min_edge_idx")
                    continue

                # ----------------------------------
                # LAST BIN: enforce scan window upper bound
                # ----------------------------------
                if j == n_bins and edges[p] > max_boundary:
                    if DEBUG:
                        print("   -> rejected: last bin starts above max_boundary")
                    continue
                
                # ----------------------------------
                # DP recursion validity
                # ----------------------------------
                if dp[j - 1, p] <= NEG / 2:
                    if DEBUG:
                        print("   -> rejected: dp previous invalid")
                    continue

                # ----------------------------------
                # VALID MATRIX (skip for first bin)
                # ----------------------------------
                if j != 1 and not valid[p, i]:
                    if DEBUG:
                        print("   -> rejected: not valid[p,i]")
                    continue

                # ----------------------------------
                # Compute value
                # ----------------------------------
                #val = dp[j - 1, p] + interval_fom(p, -i)
                z2 = interval_fom_multi(p, i)
                val = dp[j - 1, p] + z2
                
                if val > best:
                    best = val
                    best_p = p

            dp[j, i] = best
            prv[j, i] = best_p

    if DEBUG:
        print("[DEBUG] DP done...")

    # ----------------------------------
    # No solution
    # ----------------------------------
    if dp[n_bins, M] <= NEG / 2:
        print("\n[DP FAILURE]")
        print(f"  n_bins = {n_bins}")
        print(f"  M = {M}")
        debug_dp_failure(edges, valid, PB, PE, PB_era_flav, PS_mass)
        return None, None
    
    # ----------------------------------
    # Backtrack
    # ----------------------------------
    idx = []
    j, i = n_bins, M

    while j > 0:
        p = prv[j, i]
        if p < 0:
            return None, None
        idx.append(p)
        i = p
        j -= 1

    idx = list(reversed(idx))
    
    best_edges = [edges[0]] + [edges[k] for k in idx if k != 0] + [edges[-1]]
    
    return math.sqrt(dp[n_bins, M]), best_edges

def run_global_dp(all_bins, all_fake, sig_cache, n, no_scan_window, norm=None, tag=""):


    active_eras = list(all_bins[0].keys())
    
    edges, bkg, err2, sig, bkg_per_era, err2_per_era, bkg_per_era_flav, fake_per_era_flav,err2_per_era_flav =    build_global_arrays(
        all_bins, all_fake, sig_cache
    )

    PF_era_flav = [
        {era: prefix_sums(fake_per_era_flav[f][era]) for era in active_eras}
	for f in range(len(fake_per_era_flav))
    ]
    PE_era_flav = [
    {era: prefix_sums(err2_per_era_flav[f][era]) for era in active_eras}
        for f in range(len(err2_per_era_flav))
]
    # ----------------------------------
    # BUILD SIGNAL PER MASS
    # ----------------------------------
    sig_per_mass = {}
    
    n_bins_local = len(edges) - 1

    for m, c in sig_cache.items():
        
        sig_arr = np.zeros(n_bins_local)
        
        for i in range(n_bins_local):
            lo = edges[i]
            hi = edges[i+1]
            
            for x, v in c.items():
                if lo <= x < hi:
                    sig_arr[i] += v

        sig_per_mass[m] = sig_arr
    n_bins_local = len(edges) - 1
    
    PB = prefix_sums(bkg)
    PE = prefix_sums(err2)
    
    PB_era = {era: prefix_sums(bkg_per_era[era]) for era in active_eras}
    PE_era = {era: prefix_sums(err2_per_era[era]) for era in active_eras}
    PB_era_flav = [
        {era: prefix_sums(bkg_per_era_flav[f][era]) for era in active_eras}
        for f in range(len(bkg_per_era_flav))
    ]
    PS_mass = {
        m: prefix_sums(sig_per_mass[m])
        for m in sig_per_mass
    }
    for m in PS_mass:
        if len(PS_mass[m]) != len(edges):
            raise RuntimeError(f"Mismatch: PS_mass[{m}] has wrong length")
    t0_dp = time.perf_counter()

    print(f"[{tag}][DEBUG] masses in DP:", list(PS_mass.keys()))
    result = dp_global(
        edges,
        PB, PE, PS_mass,
        PB_era,
        PE_era,
        PB_era_flav,
        PE_era_flav,   # <-- ADD THIS
        PF_era_flav,
        n,
        USE_PER_ERA_STAT,
        no_scan_window,
        norm=norm,
        tag=tag
    )
    
    t_dp = time.perf_counter() - t0_dp
    print(f"[TIME][DP] n={n} -> {t_dp:.3f}s")
    
    return result


# =========================================================
# BUILD GLOBAL ARRAYS (ALL ERAS + FLAVOURS)
# =========================================================

def build_global_arrays(all_bins, all_fake, sig_cache):

    active_eras = list(all_bins[0].keys())
    ref = all_bins[0][active_eras[0]]

    edges = np.array([b[0] for b in ref] + [ref[-1][1]], dtype=float)
    n = len(ref)

    bkg = np.zeros(n)
    err2 = np.zeros(n)
    sig  = np.zeros(n)

    bkg_per_era_flav = [
        {era: np.zeros(n) for era in active_eras}
        for _ in range(len(all_bins))
    ]
    err2_per_era_flav = [
        {era: np.zeros(n) for era in active_eras}
        for _ in range(len(all_bins))
    ]
    fake_per_era_flav = [
        {era: np.zeros(n) for era in active_eras}
        for _ in range(len(all_bins))
    ]

    err2_per_era = {era: np.zeros(n) for era in active_eras}
    bkg_per_era  = {era: np.zeros(n) for era in active_eras}

    # ----------------------------------
    # LOOP OVER FLAVOURS + ERAS
    # ----------------------------------
    for f_idx in range(len(all_bins)):

        bins_f = all_bins[f_idx]
        fake_f = all_fake[f_idx]

        for era in active_eras:

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
                    e2 = (b_val * rel) ** 2
                    
                    err2[i] += e2
                    err2_per_era_flav[f_idx][era][i] += e2
                    err2_per_era[era][i] += e2
                else:
                    e2 = 0.0
                    
                bkg_per_era[era][i] += b_val

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

    return edges, bkg, err2, sig, \
        bkg_per_era, err2_per_era, \
        bkg_per_era_flav, fake_per_era_flav, \
        err2_per_era_flav



def compute_last_valid_boundary(edges, PB_era_flav, PE_era_flav, use_per_era_stat):

    n = len(edges) - 1
    n_flav = len(PB_era_flav)

    for i in reversed(range(n)):

        ok = True

        # ----------------------------------
        # LOOP OVER FLAVOURS
        # ----------------------------------
        for f in range(n_flav):

            # ----------------------------------
            # PER-ERA REQUIREMENT
            # ----------------------------------
            if use_per_era_stat:

                for era in PB_era_flav[f]:

                    B = PB_era_flav[f][era][n] - PB_era_flav[f][era][i]
                    E = PE_era_flav[f][era][n] - PE_era_flav[f][era][i]

                    rel = compute_rel_unc(B, E)

                    if not pass_stat(B, rel):

                        if DEBUG:
                            print("\n[FAIL PER-ERA STAT DEBUG]")
                            print(f"  Edge = {edges[i]:.1f}")
                            print(f"  Flavour = {FLAVOURS[f]}")
                            print(f"  Era = {era}")
                            print(f"  B = {B:.4f}, rel = {rel:.3f}")

                        ok = False
                        break

                if not ok:
                    break

            # ----------------------------------
            # RUN2 COMBINED REQUIREMENT
            # ----------------------------------
            else:

                B = sum(
                    PB_era_flav[f][era][n] - PB_era_flav[f][era][i]
                    for era in PB_era_flav[f]
                )

                E = sum(
                    PE_era_flav[f][era][n] - PE_era_flav[f][era][i]
                    for era in PB_era_flav[f]
                )

                rel = compute_rel_unc(B, E)

                if not pass_stat(B, rel):

                    if DEBUG:
                        print("\n[FAIL RUN2 STAT DEBUG]")
                        print(f"  Edge = {edges[i]:.1f}")
                        print(f"  Flavour = {FLAVOURS[f]}")
                        print(f"  B_run2 = {B:.4f}, rel = {rel:.3f}")

                    ok = False
                    break

        # ----------------------------------
        # ACCEPT EDGE
        # ----------------------------------
        if ok:
            if DEBUG:
                print(f"[DEBUG] --> boundary chosen at {edges[i]:.1f}")
            return edges[i]

    # ----------------------------------
    # FALLBACK
    # ----------------------------------
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

def build_valid_matrix_global(edges, PB, PE, PB_era, PE_era,
                             interval_bkg,
                             use_per_era=False):

    M = len(edges) - 1
    valid = np.zeros((M + 1, M + 1), dtype=np.bool_)

    for p in range(M):
        for i in range(p + 1, M + 1):

            # ----------------------------------
            # MIN WIDTH CUT
            # ----------------------------------
            if (edges[i] - edges[p]) < 25:
                continue

            # ----------------------------------
            # PER-ERA MODE
            # ----------------------------------
            if use_per_era:

                pass_per_era = True

                for era in PB_era:

                    B_e = PB_era[era][i] - PB_era[era][p]
                    E_e = PE_era[era][i] - PE_era[era][p]

                    rel_e = compute_rel_unc(B_e, E_e)

                    if not pass_stat(B_e, rel_e):
                        pass_per_era = False
                        break

                if not pass_per_era:
                    continue

            # ----------------------------------
            # RUN2 MODE
            # ----------------------------------
            else:

                B = interval_bkg(p, i)
                E = PE[i] - PE[p]

                rel = compute_rel_unc(B, E)

                if not pass_stat(B, rel):
                    continue

            # ----------------------------------
            # VALID TRANSITION
            # ----------------------------------
            valid[p, i] = True

    return valid


def compute_rel_unc(B, E):
    if B < 1e-9:
        return 999.0
    return math.sqrt(E) / B



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
        rel=err/val if val>0 else 0
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

        sig_hist[m] = {}

        # ----------------------------------
        # LOOP OVER ERAS
        # ----------------------------------
        for era in ERAS:

            fname = os.path.join(
                sel, era,
                f"HNL_SignalRegion_Plotter_{sig_sample}.root"
            )

            f = ROOT.TFile(fname)

            if not f or f.IsZombie():
                raise RuntimeError(f"[ERROR] Cannot open file: {fname}")

            h_tmp = f.Get(path)

            if not h_tmp:
                raise RuntimeError(f"[ERROR] Missing hist: {path} in {fname}")

            h = h_tmp.Clone()
            h.SetDirectory(0)
            f.Close()

            # ----------------------------------
            # NORMALISE PER ERA (shape only)
            # ----------------------------------
            if h.Integral() > 0:
                h.Scale(1.0 / h.Integral())

            sig_hist[m][era] = h

        print(f"[INFO] Loaded signal (per era): {m} -> {sig_sample}")

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

        for era in edges_per_era:

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



def find_min_signal_edge(sig_cache, edges, threshold=0.01):

    # total signal
    S_total = sum(sum(c.values()) for c in sig_cache.values())

    cumulative = 0.0

    for i in range(len(edges)-1):

        lo = edges[i]
        hi = edges[i+1]

        S_bin = 0.0
        for m, c in sig_cache.items():
            for x, v in c.items():
                if lo <= x < hi:
                    S_bin += v

        cumulative += S_bin

        if cumulative / S_total > threshold:
            return lo

    return edges[0]


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
            rel = compute_rel_unc(bkg_vals[i], 0.0)  # or better track E properly
            condition = pass_stat(bkg_vals[i], rel)
        if condition:
            max_edge = edges[i+1]
            break

    return [e for e in edges if e <= max_edge]

def get_combined_signal(all_sig, i_flav, mass):
    h = None
    for era in ERAS:
        h_era = all_sig[i_flav][mass][era]
        if h is None:
            h = h_era.Clone()
        else:
            h.Add(h_era)

    if h and h.Integral() > 0:
        h.Scale(1.0 / h.Integral())

    return h


def evaluate(edges, bins, cache, fake_bins, edges_per_era=None, force_mode=None):
    """
    force_mode:
        None       -> follow USE_PER_ERA_STAT
        "perera"   -> force per-era evaluation
        "run2"     -> force Run2 evaluation
    """

    mode = force_mode

    if mode is None:
        mode = "perera" if USE_PER_ERA_STAT else "run2"

    # -------------------------
    # PER-ERA MODE
    # -------------------------
    if mode == "perera":
        if edges_per_era is not None:
            return evaluate_variable_binning(edges_per_era, bins, cache, fake_bins)
        else:
            return math.sqrt(sum(
                v*v for v in evaluate_per_era(edges, bins, cache, fake_bins).values()
            ))

    # -------------------------
    # RUN2 MODE
    # -------------------------
    elif mode == "run2":
        if edges_per_era is not None:
            return evaluate_variable_binning_run2(edges_per_era, bins, cache, fake_bins)
        else:
            return evaluate_run2(edges, bins, cache, fake_bins)

    else:
        raise ValueError("Unknown evaluation mode")


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



def make_mass_scan_plot_per_flavour(mass_plot_data, flav, tag,all_bins, all_fake, all_sig):


    print(f"[DEBUG] ENTER make_mass_scan_plot_per_flavour: {flav}")
    
    c = ROOT.TCanvas(f"c_mass_scan_{flav}","",900,700)

    graphs = {}

    markers = {
        "global": 20,
        "perflav": 21,
        "permass": 26,
        "sum_global": 22,
        "sum_perflav": 23,
        "era_quadrature": 24,
        "run2_from_era_binning": 25
    }
    
    styles = {
        "global": 1,
        "perflav": 2,
        "permass": 7,
        "sum_global": 3,
        "sum_perflav": 4,
        "era_quadrature": 5,
        "run2_from_era_binning": 6
    }
    
    first = True
    all_y = []

    for scen in ["global", "perflav", "permass", "sum_global", "sum_perflav", "era_quadrature", "run2_from_era_binning"]:

        g = ROOT.TGraph()

        points = sorted(mass_plot_data[scen])

        for i, (m, f) in enumerate(points):
            g.SetPoint(i, m, f)
            all_y.append(f)

        g.SetMarkerStyle(markers[scen])
        g.SetLineStyle(styles[scen])
        g.SetLineWidth(2)

        if first:
            g.Draw("APL")
            g.GetXaxis().SetTitle("Mass [GeV]")
            g.GetYaxis().SetTitle("FOM")
            first = False
        else:
            g.Draw("PL SAME")

        graphs[scen] = g
    
    # -----------------------------
    # Axis control
    # -----------------------------
    if all_y:
        ymin = 0.0
        ymax_data = max(all_y)
        
        if ymax_data < 1.0:
            ymax = ymax_data * 1.5
        else:
            ymax = ymax_data * 1.3
            
        graphs["global"].SetMinimum(ymin)
        graphs["global"].SetMaximum(ymax)

        
        ROOT.gPad.Modified()
        ROOT.gPad.Update()
    xmin = min(m for scen in mass_plot_data for m, _ in mass_plot_data[scen])
    xmax = max(m for scen in mass_plot_data for m, _ in mass_plot_data[scen])

    graphs["global"].GetXaxis().SetLimits(xmin * 0.95, xmax * 1.05)

    ROOT.gPad.Modified()
    ROOT.gPad.Update()

    ref_points = []

    ref_cache = {f: {} for f in FLAVOURS}

    for mass in OPT_MASSES:
        for i_flav, flav_loop in enumerate(FLAVOURS):
            
            bins_f = all_bins[i_flav]
            fake_f = all_fake[i_flav]

            
            h = None
            for era in ERAS:
                h_era = all_sig[i_flav][mass][era]
                if h is None:
                    h = h_era.Clone()
                else:
                    h.Add(h_era)
                    
            if h.Integral() > 0:
                h.Scale(1.0 / h.Integral())
                
            cache_flav = build_sig_cache({mass: h})

            
            edges_per_era = {}
            for era in ERAS:
                mass_key = get_ref_mass_key(mass.split("_")[1])
                edges_per_era[era] = REF_BINS[era][flav_loop][mass_key]

            f_ref = evaluate(
                None,
                bins_f,
                cache_flav,
                fake_f,
                edges_per_era=edges_per_era
            )   
            print("\n[REFERENCE DEBUG]")
            print(f"Mass = {mass} | Flavour = {flav_loop}")
            print("Edges =", edges_per_era)
            
            for era in ERAS:
                print(f"[REFERENCE EDGES] {era} =", edges_per_era[era])
                
            print_best_binning_summary(
                edges_per_era[ERAS[0]],  # display only
                bins_f,
                {era: cache_flav for era in ERAS},
                fake_f,
                label="REFERENCE",
                mass=mass,
                flavour=flav 
            )
            ref_cache[flav_loop][mass] = f_ref
    
        
    for m, _ in sorted(mass_plot_data["global"]):
        mass_str = f"DYVBF_{int(m)}"
        if mass_str not in ref_cache[flav]:
            print(f"[WARNING] Missing reference for {flav}, {mass_str}")
            continue
        ref_val = ref_cache[flav][mass_str]
        ref_points.append((m, ref_val))
            
    g_ref = ROOT.TGraph()

    for i, (m, f) in enumerate(ref_points):
        g_ref.SetPoint(i, m, f)
        
    g_ref.SetLineStyle(7)
    g_ref.SetLineWidth(3)
    g_ref.Draw("PL SAME")
        
    # -----------------------------
    # Legend (top-left)
    # -----------------------------
    leg = ROOT.TLegend(0.15,0.65,0.45,0.88)
    leg.SetTextSize(0.025)

    leg.AddEntry(graphs["global"], "Global", "lp")
    leg.AddEntry(graphs["perflav"], "Per flavour", "lp")
    leg.AddEntry(graphs["sum_global"], "Sum global", "lp")
    leg.AddEntry(graphs["sum_perflav"], "Sum per flavour", "lp")
    leg.AddEntry(graphs["era_quadrature"], "Era (quadrature)", "lp")
    leg.AddEntry(graphs["run2_from_era_binning"], "Run2 (era binning)", "lp")
    leg.AddEntry(g_ref, "Reference (ANv6 Mass-Binned)", "l")
    leg.Draw()

    # -----------------------------
    # Flavour label
    # -----------------------------
    txt = ROOT.TLatex()
    txt.SetNDC()
    txt.SetTextSize(0.04)
    txt.DrawLatex(0.18, 0.92, flav)

    # -----------------------------
    # Save
    # -----------------------------
    os.makedirs("plots", exist_ok=True)
    print(f"[DEBUG] SAVING plot for {flav}")
    c.SaveAs(f"plots/fom_vs_mass_{flav}_{tag}.pdf")


def make_mass_plot(mass, results, flav, tag=""):

    c = ROOT.TCanvas(f"c_{mass}_{flav}", "", 800, 700)
    c.SetLeftMargin(0.12)
    c.SetBottomMargin(0.12)
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

    for i, (n, f) in enumerate(results["era_quadrature"][flav]):
        g_era.SetPoint(i, n, f)
    for i, (n, f) in enumerate(results["run2_from_era_binning"][flav]):
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

    g_global.SetMinimum(0.0)
    
    yvals = []
    for key in ["global", "perflav", "permass", "sum_global", "sum_perflav", "era_quadrature", "run2_from_era_binning"]:
        for _, v in results[key][flav]:
            yvals.append(v)
            
    if yvals:
        g_global.SetMaximum(max(yvals) * 1.5)
    
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
    leg = ROOT.TLegend(0.15,0.65,0.45,0.88)
    leg.SetTextSize(0.025)
    
    leg.AddEntry(g_global, "Mass-opt global", "lp")
    leg.AddEntry(g_perflav, "Mass-opt per flavour", "lp")
    leg.AddEntry(g_sum_global, "Summed global", "lp")
    leg.AddEntry(g_sum_perflav, "Summed per flavour", "lp")
    leg.AddEntry(g_era, "Era-by-era (sum2 FOM)", "lp")
    leg.AddEntry(g_era_run2, "Era-by-era (Run2 combined)", "lp")
    leg.Draw()

    txt = ROOT.TLatex()
    txt.SetNDC()
    txt.SetTextSize(0.04)
    txt.DrawLatex(0.18, 0.92, flav)
    
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
    

    
    
    
# =========================================================
# MAIN
# =========================================================



def main():

    t0_total = time.perf_counter()

    
    global OPT_MASSES, EVAL_MASSES
    global USE_PER_ERA_STAT
    global DEBUG
    VALID_CACHE.clear()
    
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", action="store_true",
                    help="Run debug printout and exit")
    parser.add_argument("--EraStatCheck", action="store_true",
                        help="Require stat threshold per era instead of Run2")
    parser.add_argument("--no-scan-window", action="store_true")
    
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
    #DYVBF_MASSES = ["400","500" ,"700","2000"]

    OPT_MASSES = ["DYVBF_" + m for m in DYVBF_MASSES]
    EVAL_MASSES = OPT_MASSES

    ba vse = "/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"

    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs", exist_ok=True)

    tag = ("perEra" if USE_PER_ERA_STAT else "run2") + ("_noScanWindow" if args.no_scan_window else "")
    
    sys.stdout = TeeLogger(f"logs/scan_{tag}_{ts}.txt")

    # ----------------------------------
    # LOAD
    # ----------------------------------
    all_bins = []
    all_sig  = []
    all_fake = []

    t0_load = time.perf_counter()
    
    for flav in FLAVOURS:
        print(f"[TIME] Load = {time.perf_counter() - t0_load:.2f} s")
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
            
            for era in ERAS:
                h_era = sig[m][era]
                
                if h is None:
                    h = h_era.Clone()
                else:
                    h.Add(h_era)

                    
        if h and h.Integral() > 0:
            h.Scale(1.0 / h.Integral())

        combined_sig[m] = h

    combined_cache = build_sig_cache(combined_sig)
    # --- NEW: per-era signal cache ---
    sig_cache_per_era = []
    
    for i_flav in range(len(all_sig)):
        per_era = {}

        for era in ERAS:
            
            per_era[era] = {}

            for m in all_sig[i_flav]:
                h = all_sig[i_flav][m][era]
                
                per_era[era][m] = {
                    h.GetBinLowEdge(i): h.GetBinContent(i)
                    for i in range(1, h.GetNbinsX()+1)
                }

        sig_cache_per_era.append(per_era)
    global_cache = combined_cache
    
    # ----------------------------------
    # Compute per-mass normalisation
    # ----------------------------------
    print("\n[INFO] Computing per-mass normalisation...")
    
    norm = {}
    
    for mass in OPT_MASSES:
        
        print(f"[NORM] Mass = {mass}")
        
        # build single-mass cache
        single_cache = build_sig_cache({
            mass: combined_sig[mass]
        })
        
        # run DP with that mass only
        best = run_global_dp(
            all_bins,
            all_fake,
            single_cache,
            max(TOTAL_BINS_TO_SCAN),
            args.no_scan_window,
            tag=f"NORM|{mass}"

        )
        
        if best[1] is None:
            print(f"[WARNING] No valid binning for {mass}")
            norm[mass] = 1.0
            continue
        
        Z_best = best[0]
        norm[mass] = Z_best * Z_best
        
        print(f"[NORM] {mass} -> Z = {Z_best:.3f}, Z^2 = {norm[mass]:.3f}")
    
    # ----------------------------------
    # GLOBAL DP PER FLAVOUR (used later)
    # ----------------------------------
    best_global_flav = {}

    t0_global_dp = time.perf_counter()
    
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
                n,
                args.no_scan_window,
                tag=f"GLOBAL_FLAV|{flav}|n={n}"
            )

            best_global_flav[flav][n] = best_edges_global
    print(f"[TIME] Global DP per flavour = {time.perf_counter() - t0_global_dp:.2f} s")

    t0_mass_loop = time.perf_counter()
    # ----------------------------------
    # MASS LOOP
    # ----------------------------------
    results_all = {}
    for mass in tqdm(OPT_MASSES, desc="Mass loop"):
        results_mass = {}

        results_mass[mass] = {
            "global": {flav: [] for flav in FLAVOURS},
            "perflav": {flav: [] for flav in FLAVOURS},
            "permass": {flav: [] for flav in FLAVOURS},
            "sum_global": {flav: [] for flav in FLAVOURS},
            "sum_perflav": {flav: [] for flav in FLAVOURS},
            "era_quadrature": {flav: [] for flav in FLAVOURS},
            "run2_from_era_binning": {flav: [] for flav in FLAVOURS},
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
            t0 = time.perf_counter()

            # =========================
            # SCENARIO 1: mass-opt global
            # =========================
            # --- GLOBAL (all masses)
            best_global = run_global_dp(
                all_bins,
                all_fake,
                combined_cache,
                n,
                args.no_scan_window,
                tag=f"GLOBAL|{mass}|n={n}"
                
            )
            
            # --- PER-MASS (single mass)
            best_permass = run_global_dp(
                all_bins,
                all_fake,
                single_cache_global,
                n,
                args.no_scan_window,
                tag=f"PERMASS|{mass}|n={n}"

            )
            t_global = time.perf_counter() - t0
            if best_global[1] is None:
                continue
            edges_global = best_global[1]
            
            combined_bins = combine_flavours(all_bins)
            combined_fake = combine_flavours(all_fake)

            for i_flav, flav in enumerate(FLAVOURS):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][mass][era]
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)

                if h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())
                    
                cache_flav = build_sig_cache({mass: h})
                
                # GLOBAL result
                edges_global = best_global[1]

                f_global = evaluate(edges_global, bins_f, cache_flav, fake_f)
                results_mass[mass]["global"][flav].append((n, f_global))

                # PER-MASS result
                edges_permass = best_permass[1]

                f_permass = evaluate(edges_permass, bins_f, cache_flav, fake_f)
                results_mass[mass]["permass"][flav].append((n, f_permass))

            # =========================
            # SCENARIO 2: mass-opt per flavour
            # =========================
            f_perflav_total = 0.0
            t0 = time.perf_counter()
            
            for i_flav, flav in enumerate(FLAVOURS):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]

                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][mass][era]
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)

                if h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())

                cache_flav = build_sig_cache({mass: h})

                best_flav = run_global_dp([bins_f], [fake_f], cache_flav, n,args.no_scan_window,    tag=f"{flav}|{mass}|n={n}")
                if best_flav[1] is None:
                    continue
                
                edges_flav = best_flav[1]
                
                f_flav = evaluate(edges_flav, bins_f, cache_flav, fake_f)
                
                results_mass[mass]["perflav"][flav].append((n, f_flav))
                
                f_perflav_total += f_flav * f_flav

            f_perflav = math.sqrt(f_perflav_total)
            t_perflav = time.perf_counter() - t0
            
            # =========================
            # SCENARIO 3: summed global
            # =========================
            t0 = time.perf_counter()
            
            res = run_global_dp(all_bins, all_fake, combined_cache, n,args.no_scan_window,norm=norm)
            t_sum_global = time.perf_counter() - t0
            if res[1] is None:
                continue

            edges_sum_global = res[1]
            
            for i_flav, flav in enumerate(FLAVOURS):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][mass][era]
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)
                        
                if h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())

                cache_flav = build_sig_cache({mass: h})
                
                f = evaluate(edges_sum_global, bins_f, cache_flav, fake_f)
                
                results_mass[mass]["sum_global"][flav].append((n, f))
            #f_sum_global = evaluate_run2(edges_sum_global, combined_bins, combined_cache, combined_fake)
            
            # =========================
            # SCENARIO 4: summed per flavour
            # =========================
            f_sum_flav_total = 0.0
            t0 = time.perf_counter()
            
            for i_flav, flav in enumerate(FLAVOURS):

                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]

                # optimisation
                cache_flav_sum = {}

                for m in OPT_MASSES:
                    h = None
                    for era in ERAS:
                        h_era = all_sig[i_flav][m][era]
                        if h is None:
                            h = h_era.Clone()
                        else:
                            h.Add(h_era)

                    if h and h.Integral() > 0:
                        h.Scale(1.0 / h.Integral())

                    cache_flav_sum[m] = h

                cache_flav_sum = build_sig_cache(cache_flav_sum)
                edges_flav_sum = run_global_dp([bins_f], [fake_f], cache_flav_sum, n,args.no_scan_window,norm=norm)[1]

                
                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][mass][era]
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)

                if h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())
                    
                cache_eval = build_sig_cache({mass: h})
                
                f_flav_sum = evaluate(edges_flav_sum, bins_f, cache_eval, fake_f)
                results_mass[mass]["sum_perflav"][flav].append((n, f_flav_sum))
                
                f_sum_flav_total += f_flav_sum * f_flav_sum

                if i_flav ==0:
                    print("[CHECK]")
                    print("Scenario: sum_perflav")
                    print("Optimisation masses:", list(cache_flav_sum.keys()))
                    print("Evaluation masses:", list(cache_eval.keys()))
                 

            f_sum_flav = math.sqrt(f_sum_flav_total)
            t_sum_perflav = time.perf_counter() - t0

            # =========================
            # SCENARIO 5: era-by-era
            # =========================

            t0 = time.perf_counter()

            edges_per_era = {}
            
            for era in ERAS:
            
                bins_era = []
                fake_era = []
            
                for f in range(len(FLAVOURS)):
                    
                    bins_tmp = {}
                    fake_tmp = {}
                    
                    for e in ERAS:
                        if e == era:
                            # keep real data for this era
                            bins_tmp[e] = all_bins[f][e]
                            fake_tmp[e] = all_fake[f][e]
                        else:
                            # zero out other eras (important: keep binning structure)
                            ref_bins = all_bins[f][era]
                            ref_fake = all_fake[f][era]
                            
                            bins_tmp[e] = [(b[0], b[1], 0.0, 0.0) for b in ref_bins]
                            fake_tmp[e] = [(b[0], b[1], 0.0, 0.0) for b in ref_fake]

                    bins_era.append(bins_tmp)
                    fake_era.append(fake_tmp)

                # ----------------------------------
                # RUN DP FOR THIS ERA
                # ----------------------------------
                res = run_global_dp(
                bins_era,
                    fake_era,
                    single_cache_global,
                    n,
                    args.no_scan_window,
                    tag=f"ERA|{era}|{mass}|n={n}"
                )

                if res[1] is None:
                    continue

                edges_per_era[era] = res[1]

            t_era = time.perf_counter() - t0
            
            # =========================
            # STORE
            # =========================
            for i_flav, flav in enumerate(FLAVOURS):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][mass][era]
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)
                        
                if h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())
                    
                cache_flav = build_sig_cache({mass: h})
                
                # --- ERA (per-era sum in quadrature)
                # ERA quadrature
                f_era = evaluate(
                    None,
                    bins_f,
                    cache_flav,
                    fake_f,
                    edges_per_era=edges_per_era,
                    force_mode="perera"
                )

                
                results_mass[mass]["era_quadrature"][flav].append((n, f_era))
                


                # --- ERA (Run2 combined)
                f_era_run2 = evaluate(
                    None,
                    bins_f,
                    cache_flav,
                    fake_f,
                    edges_per_era=edges_per_era,
                    force_mode="run2"
                )
                
                results_mass[mass]["run2_from_era_binning"][flav].append((n, f_era_run2))


            print(f"[TIME][n={n}] "
                  f"global={t_global:.3f}s | "
                  f"perflav={t_perflav:.3f}s | "
                  f"sum_global={t_sum_global:.3f}s | "
                  f"sum_perflav={t_sum_perflav:.3f}s | "
                  f"era={t_era:.3f}s")
                
        results_all[mass] = results_mass[mass]        

    print(f"[TIME] Mass loop total = {time.perf_counter() - t0_mass_loop:.2f} s")
    
    
    n_target = max(TOTAL_BINS_TO_SCAN)

    mass_plot_data_flav = {
        flav: {
            "global": [],
            "perflav": [],
            "permass": [],
            "sum_global": [],
            "sum_perflav": [],
            "era_quadrature": [],
            "run2_from_era_binning": []
        }
        for flav in FLAVOURS
    }
    
    for mass in results_all:
        
        m_val = float(mass.split("_")[1])
        
        for flav in FLAVOURS:
            
            for scen in mass_plot_data_flav[flav]:
                
                vals = results_all[mass][scen][flav]
                
                val = next((f for nn, f in vals if nn == n_target), None)
                
                if val is not None:
                    mass_plot_data_flav[flav][scen].append((m_val, val))
                
                
    # ----------------------------------
    # CLEAN SUMMARY PRINT
    # ----------------------------------
    for mass in OPT_MASSES:
        for flav in FLAVOURS:
            
            vals_pf = results_all[mass]["perflav"][flav]
            vals_pm = results_all[mass]["permass"][flav]
        
            f_pf = next(f for n,f in vals_pf if n == n_target)
            f_pm = next(f for n,f in vals_pm if n == n_target)
            print(f"{mass} | {flav} | PERFLAV = {f_pf:.3f} | PERMASS = {f_pm:.3f} | Delta = {f_pm - f_pf:.3f}")

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
        res = run_global_dp(all_bins, all_fake, combined_cache, n,args.no_scan_window, norm=norm)
        if res[1] is None:
            continue
        edges_global = res[1]

        print("\n[Scenario] GLOBAL (per flavour)")
        print("Edges =", fmt(edges_global))

        for i_flav, flav in enumerate(FLAVOURS):
            print(f"\n==================== {flav} ====================")
            print_best_binning_summary(
            edges_global,
                all_bins[i_flav],                 # <-- per flavour bins
                sig_cache_per_era[i_flav],        # <-- per flavour signal
                all_fake[i_flav],                 # <-- per flavour fake
                label=f"GLOBAL ({flav})",
                mass="Combined (all masses)",
                n_bins=n,
                flavour=flav
            )

        # =========================
        # Scenario 2: per flavour
        # =========================
        edges_perflav = {}
        
        for i_flav, flav in enumerate(FLAVOURS):
            print(f"\n==================== {flav} ====================")
            cache_flav_sum = {}

            for m in OPT_MASSES:
                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][m][era]
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)
                        
                if h and h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())

                cache_flav_sum[m] = h

            cache_flav_sum = build_sig_cache(cache_flav_sum)
            res = run_global_dp([all_bins[i_flav]], [all_fake[i_flav]], cache_flav_sum, n,args.no_scan_window)
            
            if res[1] is None:
                continue

            edges_perflav[flav] = res[1]

            print_best_binning_summary(
                edges_perflav[flav],
                all_bins[i_flav],
                sig_cache_per_era[i_flav],
                all_fake[i_flav],
                label=f"{flav} PER-FLAV",
                mass="Combined (all masses)",
                n_bins=n
            )
            
        print("\n[Scenario] PER-FLAVOUR")
        for flav in edges_perflav:
            print(f"  {flav} -> {fmt(edges_perflav[flav])}")
            
        # =========================
        # Scenario 3: summed global
        # =========================
        res_sum_global = run_global_dp(
            all_bins,
            all_fake,
            combined_cache,
            n,
            args.no_scan_window,
            norm=norm,
            tag=f"SUM_GLOBAL|n={n}"
        )
        
        if res_sum_global[1] is None:
            continue
        
        edges_sum_global = res_sum_global[1]
        
        print("\n[Scenario] SUM GLOBAL")
        print("Edges =", fmt(edges_sum_global))
        for i_flav, flav in enumerate(FLAVOURS):
            
            print_best_binning_summary(
                edges_sum_global,
                all_bins[i_flav],
                sig_cache_per_era[i_flav],
                all_fake[i_flav],
                label=f"SUM GLOBAL ({flav})",
                mass="Combined (all masses)",
                n_bins=n,
                flavour=flav
            )
        # =========================
        # Scenario 4: summed per flavour
        # =========================
        edges_sum_perflav = edges_perflav  # identical if 1 mass
        
        print("\n[Scenario] SUM PER-FLAVOUR")
        for i_flav, flav in enumerate(FLAVOURS):

            bins_f = all_bins[i_flav]
            fake_f = all_fake[i_flav]
            
            print_best_binning_summary(
                edges_sum_global,
                bins_f,
                sig_cache_per_era[i_flav],
                fake_f,
                label="SUM GLOBAL",
                mass=mass,
                n_bins=n
            )
            print_best_binning_summary(
                edges_flav_sum,
                bins_f,
                sig_cache_per_era[i_flav],
                fake_f,
                label="SUM PER-FLAV",
                mass=mass,
                n_bins=n
            )

        # =========================
        # Scenario 5: era-by-era
        # =========================
        edges_per_era = {}
        edges_per_era_global = {}
        edges_per_era_permass = {}
        for era in ERAS:

            bins_era = []
            fake_era = []
            
            for f in range(len(FLAVOURS)):

                bins_era.append({era: all_bins[f][era]})
                fake_era.append({era: all_fake[f][era]})

            # GLOBAL era
            res_global = run_global_dp(
                bins_era,
                fake_era,
                combined_cache,
                n,
                args.no_scan_window
            )
            
            # PER-MASS era
            res_permass = run_global_dp(
                bins_era,
                fake_era,
                single_cache_global,
                n,
                args.no_scan_window
            )

            if res_global[1] is None or res_permass[1] is None:
                continue

            edges_per_era_global[era] = res_global[1]
            edges_per_era_permass[era] = res_permass[1]
            
            
            
        print("\n[Scenario] ERA-BY-ERA")
        for era in edges_per_era:
            print(f"  {era} -> {fmt(edges_per_era[era])}")

        # =========================
        # EVALUATION (ALWAYS SINGLE MASS)
        # =========================

        print("\n--- Evaluation (single mass) ---")

        for mass in OPT_MASSES:
            
            print(f"\n Mass = {mass}")
            
            # ----------------------------------
            # GLOBAL SIGNAL (already combined)
            # ----------------------------------
            cache_eval_global = build_sig_cache({
                mass: combined_sig[mass]
            })
            
            # ----------------------------------
            # GLOBAL (consistent with DP)
            # ----------------------------------
            f_global = 0.0
            
            for i_flav in range(len(FLAVOURS)):

                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
            
                # build per-flavour signal (combined over eras)
                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][mass][era]
                    
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)
                        
                if h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())
                    
                cache_eval = build_sig_cache({mass: h})

                f = evaluate(
                    edges_global,
                    bins_f,
                    cache_eval,
                    fake_f
                )

                f_global += f * f

            f_global = math.sqrt(f_global)
            print("  GLOBAL =", round(f_global, 4))

            # ----------------------------------
            # PER-FLAVOUR
            # ----------------------------------
            f_pf = 0.0

            for i_flav, flav in enumerate(FLAVOURS):

                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                # signal combine over eras
                h = None
                for era in ERAS:
                    h_era = all_sig[i_flav][mass][era]
                    
                    if h is None:
                        h = h_era.Clone()
                    else:
                        h.Add(h_era)
                        
                if h.Integral() > 0:
                    h.Scale(1.0 / h.Integral())
                    
                cache_eval = build_sig_cache({mass: h})
                
                f = evaluate(
                    edges_perflav[flav],
                    bins_f,
                    cache_eval,
                    fake_f
                )
                
                f_pf += f * f

            f_pf = math.sqrt(f_pf)
            print("  PER-FLAVOUR =", round(f_pf, 4))
            
            # ----------------------------------
            # SUM GLOBAL (same as global here)
            # ----------------------------------
            print("  SUM GLOBAL =", round(f_global, 4))
            
            # ----------------------------------
            # SUM PER FLAVOUR
            # ----------------------------------
            print("  SUM PER-FLAVOUR =", round(f_pf, 4))
            
            # ----------------------------------
            # ERA (consistent: per flavour, then combine)
            # ----------------------------------
            f_era_total = 0.0
            f_era_run2_total = 0.0
            
            for i_flav in range(len(FLAVOURS)):
                
                bins_f = all_bins[i_flav]
                fake_f = all_fake[i_flav]
                
                # reuse global signal (OK because shape normalized)
                cache_eval = cache_eval_global
                
                # ERA quadrature
                f_era = evaluate(
                    None,
                    bins_f,
                    cache_eval,
                    fake_f,
                    edges_per_era=edges_per_era_global,
                    force_mode="perera"
                )
                
                f_era_run2 = evaluate(
                    None,
                    bins_f,
                    cache_eval,
                    fake_f,
                    edges_per_era=edges_per_era_global,
                    force_mode="run2"
                )
            
                f_era_total += f_era * f_era
                f_era_run2_total += f_era_run2 * f_era_run2

            print("  ERA =", round(math.sqrt(f_era_total), 4))
            print("  ERA (Run2 combined) =", round(math.sqrt(f_era_run2_total), 4))
       

            
    # ----------------------------------
    # Plot
    # ----------------------------------
    for flav in FLAVOURS:
        print("FLAVOURS =", FLAVOURS)
        make_mass_scan_plot_per_flavour(
            mass_plot_data_flav[flav],
            flav,
            tag,
            all_bins,
            all_fake,
            all_sig
        )
        
    # ----------------------------------                                                                                            
    # PLOTTING (PER FLAVOUR)                                                                                                                 
    # ----------------------------------                                                                                                        
    for mass in results_all:

        for flav in FLAVOURS:
            print("FLAVOURS =", flav)
            make_mass_plot(
                float(mass.split("_")[1]),
                results_all[mass],
                flav,
                tag
            )
    print("\n==============================")
    print(f"[TIME] TOTAL = {time.perf_counter() - t0_total:.2f} s")
    print("==============================")


if __name__=="__main__":
    main()



