import math
import os
from default_config import ERAS, FLAVOURS, FAKE_FLOOR,

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
    )


def get_latest_dir(base):
    dirs = [os.path.join(base, d)
            for d in os.listdir(base)
            if os.path.isdir(os.path.join(base, d))]
    return max(dirs, key=os.path.getmtime)




def debug_data_summary(data, max_bins=5):

    print("\n==============================")
    print(" DEBUG DATA SUMMARY")
    print("==============================")

    edges = data["edges"]
    print("\n[EDGES]")
    print("N bins =", len(edges) - 1)
    print("First edges:", edges[:max_bins+1])
    print("Last edges:", edges[-(max_bins+1):])

    for flav in FLAVOURS:

        print(f"\n================ {flav} =================")

        for era in ERAS:

            print(f"\n[{era}]")

            B = data["background"][flav][era]
            F = data["fake"][flav][era]

            print(f"  Bkg:  sum={B.sum():.3f}, min={B.min():.3f}, max={B.max():.3f}")
            print(f"  Fake: sum={F.sum():.3f}, min={F.min():.3f}, max={F.max():.3f}")

            # Check negative fake bins
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

        for mass in list(data["signal_combined_mass"][flav].keys())[:3]:

            S = data["signal_combined_mass"][flav][mass]

            print(f"  Mass {mass}: sum={S.sum():.4f}, min={S.min():.4e}, max={S.max():.4e}")
            
def rebin_array(arr, edges_full, edges_target):
    out = []

    for i in range(len(edges_target)-1):
        lo = edges_target[i]
        hi = edges_target[i+1]

        mask = (edges_full[:-1] >= lo) & (edges_full[:-1] < hi)
        out.append(arr[mask].sum())

    return np.array(out)

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

