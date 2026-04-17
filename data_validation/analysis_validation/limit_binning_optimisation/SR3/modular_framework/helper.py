import math
import os,sys
from default_config import ERAS, FLAVOURS, FAKE_FLOOR
from ref_bins import  get_sr3_ref_edges

# Default fallback values (will be overridden at runtime)                                                                                                                                                                                                                                                                                                                                                                                                                               
Bin_NBKG_REQ = 1.0
Bin_NBKG_REQ_Tight = 0.5
Bin_BKG_RelUnc = 0.3

import numpy as np
import math

from tqdm import tqdm


DEBUG = False

def set_stat_config(cfg):
    global Bin_NBKG_REQ, Bin_NBKG_REQ_Tight, Bin_BKG_RelUnc

    Bin_NBKG_REQ = getattr(cfg, "Bin_NBKG_REQ", Bin_NBKG_REQ)
    Bin_NBKG_REQ_Tight = getattr(cfg, "Bin_NBKG_REQ_Tight", Bin_NBKG_REQ_Tight)
    Bin_BKG_RelUnc = getattr(cfg, "Bin_BKG_RelUnc", Bin_BKG_RelUnc)

    print("\n[STAT CONFIG]")
    print(f"  Bin_NBKG_REQ       = {Bin_NBKG_REQ}")
    print(f"  Bin_NBKG_REQ_Tight = {Bin_NBKG_REQ_Tight}")
    print(f"  Bin_BKG_RelUnc     = {Bin_BKG_RelUnc}")

def pass_stat(B):
    return (B >= Bin_NBKG_REQ)

def pass_stat_and_err(B, rel):
    return (B >= Bin_NBKG_REQ) or (B >= Bin_NBKG_REQ_Tight and rel < Bin_BKG_RelUnc)





def red(text):
    return f"\033[91m{text}\033[0m"

def green(text):
    return f"\033[92m{text}\033[0m"


def get_met_boundary(flav, era):
    # default                                                                                                                                                                                                                                
    boundary = 5

    if flav == "EE":
        if "2016" in era:
            boundary = 4
        else:
            boundary = 3

    return str(boundary)


def parse_sr3_category(cat):

    parts = cat.split("_")

    jet = parts[0]                  # LowJet / HighJet                                                                                                                                                                                       
    is_ltcut = "LTcut" in parts[3]  # True/False                                                                                                                                                                                             

    return jet, is_ltcut



def list_available_configs(config_dir="config", script_name=None):
    print("\n====================================")
    print(" AVAILABLE CONFIG FILES")
    print("====================================\n")

    if not os.path.isdir(config_dir):
        print(f"[ERROR] Config directory not found: {config_dir}")
        return

    files = sorted(os.listdir(config_dir))
    py_files = [f for f in files if f.endswith(".py") and not f.startswith("__")]

    if len(py_files) == 0:
        print("[WARNING] No config files found")
        return

    if script_name is None:
        script_name = os.path.basename(sys.argv[0])

    for f in py_files:
        mod = f.replace(".py", "")
        print(f"python {script_name} --config config.{mod}")

    print("\n====================================\n")
    
def ConvertConfPath(path):
    # remove .py if present
    if path.endswith(".py"):
        path = path[:-3]
    else:
        return path

    # replace / with .
    path = path.replace("/", ".")

    # remove leading ./ if present
    if path.startswith("."):
        path = path[1:]

    return path

def ReadConfig(cfg):

    def get(name):
        if not hasattr(cfg, name):
            raise AttributeError(f"[CONFIG ERROR] Missing '{name}' in config")
        return getattr(cfg, name)
    
    return (
        get("MASSES"),
        get("NBINS_TO_SCAN"),
        get("USE_FAKE_FIX"),
        get("RUN_Z_NO_UNC"),
        get("LOG_TAG"),
        get("TAG"),
        get("DPScan"),
    )


def get_latest_dir(base):
    dirs = [os.path.join(base, d)
            for d in os.listdir(base)
            if os.path.isdir(os.path.join(base, d))]
    return max(dirs, key=os.path.getmtime)




def debug_data_summary(data, max_bins=5):

    print("\n==============================")
    print(" DEBUG DATA SUMMARY (SR3)")
    print("==============================")

    for met in data:
        for cat in data[met]:

            sub = data[met][cat]

            print("\n========================================")
            print(f" REGION: MET={met} | {cat}")
            print("========================================")

            # ----------------------------------
            # Edges
            # ----------------------------------
            edges = sub["edges"]

            print("\n[EDGES]")
            print("N bins =", len(edges) - 1)
            print("First edges:", edges[:max_bins+1])
            print("Last edges:", edges[-(max_bins+1):])

            # ----------------------------------
            # Background + Fake
            # ----------------------------------
            for flav in FLAVOURS:

                print(f"\n================ {flav} =================")

                for era in ERAS:

                    print(f"\n[{era}]")

                    B = sub["background"][flav][era]
                    F = sub["fake"][flav][era]

                    print(f"  Bkg:  sum={B.sum():.3f}, min={B.min():.3f}, max={B.max():.3f}")
                    print(f"  Fake: sum={F.sum():.3f}, min={F.min():.3f}, max={F.max():.3f}")

                    # Negative fake bins
                    neg_bins = [(i, F[i]) for i in range(len(F)) if F[i] <= 0]

                    if neg_bins:
                        print(f"  [WARN] {len(neg_bins)} fake bins <= 0 (showing first {max_bins})")
                        print("   ", neg_bins[:max_bins])
                    else:
                        print("  Fake all positive")

                    print("  First bins:")
                    for i in range(min(max_bins, len(B))):
                        print(f"    bin {i}: B={B[i]:.3f}, F={F[i]:.3f}")

                # ----------------------------------
                # Signal checks
                # ----------------------------------
                print(f"\n[{flav}] SIGNAL CHECK")

                for mass in list(sub["signal_combined_mass"][flav].keys())[:3]:

                    S = sub["signal_combined_mass"][flav][mass]

                    print(f"  Mass {mass}: sum={S.sum():.4f}, min={S.min():.4e}, max={S.max():.4e}")

            # ----------------------------------
            # Norm check (NEW - VERY IMPORTANT)
            # ----------------------------------
            print("\n[NORM CHECK]")
            for mass in list(sub["norm"].keys())[:5]:
                print(f"  Mass {mass}: norm = {sub['norm'][mass]:.6f}")

            # ----------------------------------
            # Quick total background sanity
            # ----------------------------------
            totalB = sum(
                sub["background"][flav][era].sum()
                for flav in FLAVOURS
                for era in ERAS
            )

            print(f"\n[TOTAL BKG] = {totalB:.3f}")
            

def compute_bin_Z(s, b):
    if s <= 0 or b <= 0:
        return 0.0
    return math.sqrt(2*((s+b)*math.log(1+s/b) - s))

def fix_fake_and_bkg(F, B, FAKE_FLOOR, flavour=None, era=None, debug=False):

    if F <= 0.0:
        delta = FAKE_FLOOR - F

        if debug:
            print("[fix_fake_and_bkg] era={} flavour={}, fake_before={:.6f}, fake_floor={:.6f}, delta={:.6f}".format(
                era, flavour, F, FAKE_FLOOR, delta
            ))

        if debug and era == "Run2":
            print("[DEBUG] Before:  F =", F, "B =", B)

        F = FAKE_FLOOR
        B += delta

        if debug and era == "Run2":
            print("[DEBUG] After:   F =", F, "B =", B)

    return F, B



def compute_bin_Z_with_unc(S, B, E,run_z_no_unc=True):
                
    if run_z_no_unc:
        return compute_bin_Z(S,B)
    
    if S <= 0 or B <= 0:
        return 0.0

    if B <= 1e-9:
        return 0.0

    sigma2 = E

    if sigma2 <= 0:
        return compute_bin_Z(S, B)

    term1 = (S + B) * math.log((S + B)*(B + sigma2) / (B*B + (S + B)*sigma2))
    term2 = (B*B / sigma2) * math.log(1 + sigma2*S / (B*(B + sigma2)))

    Z2 = 2 * (term1 - term2)

    return math.sqrt(Z2) if Z2 > 0 else 0.0


def print_sr3_bin_table(data, flav, mass):

    print("\n========================================================")
    print(f" BIN TABLE (SR3) | {flav} | mass={mass}")
    print("========================================================")

    header = (
        "Bin | Era       | Jet     | MET   | LT range        | "
        "   S      B      rel_unc     Z"
    )
    print(header)
    print("-" * len(header))

    for era in ERAS:

        boundary = get_met_boundary(flav, era)

        print("\n--------------------------------------------------------")
        print(f" ERA: {era}   (MET boundary = {boundary})")
        print("--------------------------------------------------------")

        bin_counter = 0

        cats = [
            f"LowJet_LT_MET{boundary}_LTcut",
            f"HighJet_LT_MET{boundary}_LTcut",
            f"LowJet_LT_MET{boundary}_GTcut",
            f"HighJet_LT_MET{boundary}_GTcut",
        ]

        for cat in cats:

            sub = data[boundary][cat]

            jet, is_ltcut = parse_sr3_category(cat)
            met_label = "<" if is_ltcut else ">="

            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]

            edges_ref = get_sr3_ref_edges(flav, era, jet, is_ltcut)

            S_arr = sub["signal"][flav][mass][era]
            B_arr = sub["background"][flav][era]
            F_arr = sub["fake"][flav][era]
            E_arr = sub["bkg_err2"][flav][era]

            for i in range(len(edges_ref) - 1):

                lo = edges_ref[i]
                hi = edges_ref[i+1]

                if i == len(edges_ref) - 2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)


                S = S_arr[mask].sum()
                B = B_arr[mask].sum()
                F = F_arr[mask].sum()
                E = E_arr[mask].sum()

                # Fake correction
                F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                        flavour=flav, era=era)

                rel_unc = math.sqrt(E)/B if B > 0 else 0.0

                # Stat check
                stat_ok = pass_stat_and_err(B, rel_unc)

                if S > 0 and B > 0:
                    Z = compute_bin_Z_with_unc(S, B, E)
                else:
                    Z = 0.0

                line = (
                    f"{bin_counter:3d} | {era:9s} | {jet:7s} | {met_label:3s}{boundary:2s} | "
                    f"[{lo:5.0f},{hi:5.0f}] | "
                    f"{S:6.3f} {B:7.2f} {rel_unc:9.3f} {Z:7.3f}"
                )

                # Color output
                if not stat_ok:
                    print(red(line))
                else:
                    print(line)

                bin_counter += 1


