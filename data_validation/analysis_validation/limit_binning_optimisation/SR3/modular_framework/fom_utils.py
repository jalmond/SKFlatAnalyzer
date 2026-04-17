# =========================================================                                                                                                                                                                                  
# EVALUATORs                                                                                                                                                                                                                                 
# =========================================================                                                                                                                                                                                 

from default_config import ERAS, FLAVOURS, FAKE_FLOOR

from helper import fix_fake_and_bkg,compute_bin_Z_with_unc,pass_stat_and_err

import math
import itertools
from tqdm import tqdm


from multiprocessing import Pool, cpu_count


def run_scan_single(args):
    data, flav, payload, run_scan = args

    if run_scan == "PerMassNBin":
        return scan_nbin_sr3(data, flav, payload)

    elif run_scan == "GlobalMass":
        return scan_global_sr3(data, flav, payload)  # payload = masses list

    else:
        return scan_sr3(data, flav, payload)
    

def compare_binnings_physics(data, flav, mass, met, cat, bins_a, bins_b):

    print("\n========================================")
    print(f"COMPARE PHYSICS | {cat} | MET={met}")
    print("========================================")


    def extract(bins):

        out = []

        for i in range(len(bins) - 1):

            lo = bins[i]
            hi = bins[i + 1]

            # accumulate per-era (correct physics)
            S_tot = 0.0
            B_tot = 0.0
            E_tot = 0.0

            edges_full = data[met][cat]["edges"]
            bin_lo = edges_full[:-1]
            
            for era in ERAS:

                sub = data[met][cat]

                S_arr = sub["signal"][flav][mass][era]
                B_arr = sub["background"][flav][era]
                F_arr = sub["fake"][flav][era]
                E_arr = sub["bkg_err2"][flav][era]

                if i == len(bins) - 2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                s = S_arr[mask].sum()
                b = B_arr[mask].sum()
                f = F_arr[mask].sum()
                e = E_arr[mask].sum()

                # fake fix per era (correct place)
                f, b = fix_fake_and_bkg(
                    f, b, FAKE_FLOOR,
                    flavour=flav, era=era
                )

                # accumulate AFTER fix
                S_tot += s
                B_tot += b
                E_tot += e

            # compute Z on summed (Run2-equivalent after correct fix)
            if S_tot > 0 and B_tot > 1e-12:
                z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot)
            else:
                z = 0.0

            out.append((lo, hi, S_tot, B_tot, E_tot, z))

        return out

    A = extract(list(bins_a))
    Bv = extract(list(bins_b))

    print("Bin |    Range A         |    Range B         |   dS      dB      dZ")
    print("----------------------------------------------------------------------------")

    for i in range(max(len(A), len(Bv))):

        if i >= len(A) or i >= len(Bv):
            print(f"{i:3d} | LENGTH MISMATCH")
            continue

        loA, hiA, sA, bA, eA, zA = A[i]
        loB, hiB, sB, bB, eB, zB = Bv[i]

        print(f"{i:3d} | [{loA:5.0f},{hiA:5.0f}] | [{loB:5.0f},{hiB:5.0f}] "
              f"| {sA-sB:+7.3e} {bA-bB:+7.3e} {zA-zB:+7.3e}")



def snap_to_variable_grid(e):

    if e < 300:
        step = 10
    elif e < 500:
        step = 20
    elif e < 900:
        step = 50
    else:
        step = 100

    return int(e // step * step)
        
def get_nbins_for_region(cat):

    if "LowJet" in cat and "LTcut" in cat:
        return 5
    elif "LowJet" in cat and "GTcut" in cat:
        return 3
    elif "HighJet" in cat and "LTcut" in cat:
        return 6
    elif "HighJet" in cat and "GTcut" in cat:
        return 4

    raise ValueError(f"Unknown category: {cat}")



def build_run2_arrays(data, flav, mass, met, cat):

    S_run2, B_run2, F_run2, E_run2 = None, None, None, None

    for era in ERAS:

        sub = data[met][cat]

        S = sub["signal"][flav][mass][era].copy()
        B = sub["background"][flav][era].copy()
        F = sub["fake"][flav][era].copy()
        E = sub["bkg_err2"][flav][era].copy()

        if S_run2 is None:
            S_run2 = S
            B_run2 = B
            F_run2 = F
            E_run2 = E
        else:
            S_run2 += S
            B_run2 += B
            F_run2 += F
            E_run2 += E

    return S_run2, B_run2, F_run2, E_run2



def generate_fixed_nbin_binnings(edges, nbins, min_width=20):

    out = []

    lo = edges[0]
    hi = edges[-1]

    internal_edges = edges[1:-1]

    # choose nbins-1 internal boundaries
    for combo in itertools.combinations(internal_edges, nbins - 1):

        full = (lo,) + combo + (hi,)

        valid = True

        for i in range(len(full) - 1):
            if full[i+1] - full[i] < min_width:
                valid = False
                break

        if valid:
            out.append(full)

    return out


def passes_all_bins(edges, S, B, E, bin_lo, flav=None):

    for i in range(len(edges) - 1):

        lo = edges[i]
        hi = edges[i + 1]

        if i == len(edges) - 2:
            mask = (bin_lo >= lo) & (bin_lo <= hi)
        else:
            mask = (bin_lo >= lo) & (bin_lo < hi)

        b = B[mask].sum()
        e = E[mask].sum()

        rel = math.sqrt(e) / b if b > 0 else 0

        # ENFORCE on ALL bins
        if not pass_stat_and_err(b, rel):
            return False

    return True


def compute_fom(edges, S, B,  E, bin_lo, flav=None):

    Z2 = 0.0

    for i in range(len(edges) - 1):

        lo = edges[i]
        hi = edges[i + 1]

        if i == len(edges) - 2:
            mask = (bin_lo >= lo) & (bin_lo <= hi)
        else:
            mask = (bin_lo >= lo) & (bin_lo < hi)

        s = S[mask].sum()
        b = B[mask].sum()
        e = E[mask].sum()

        if s > 0 and b > 1e-12:
            Z = compute_bin_Z_with_unc(s, b, e)
            Z2 += Z * Z

    return math.sqrt(Z2)



def scan_sr3(data, flav, mass):

    from tqdm import tqdm
    import math
    import numpy as np

    METS = ["2", "3", "4", "5"]

    CATEGORIES = [
        "LowJet_LT_MET{X}_LTcut",
        "HighJet_LT_MET{X}_LTcut",
        "LowJet_LT_MET{X}_GTcut",
        "HighJet_LT_MET{X}_GTcut",
    ]

    MIN_EDGE = 150
    MIN_WIDTH = 10

    best_global = None
    best_fom = -1

    per_met_results = {}

    print("\n==============================")
    print(" SR3 SCAN | {} | mass={}".format(flav, mass))
    print("==============================")

    # ----------------------------------------
    # MET loop
    # ----------------------------------------
    for met in METS:

        print("\n================ MET {} =================".format(met))

        total_Z2 = 0.0
        region_results = {}

        for cat_template in CATEGORIES:

            cat = cat_template.format(X=met)

            print("\n--- REGION: {} ---".format(cat))

            sub = data[met][cat]

            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]
            nbins = len(bin_lo)

            MAX_EDGE = edges_full[-1]

            # ----------------------------------------
            # Build Run2 arrays WITH fake fix
            # ----------------------------------------
            S = np.zeros(nbins)
            B = np.zeros(nbins)
            E = np.zeros(nbins)

            for era in ERAS:

                S_arr = sub["signal"][flav][mass][era]
                B_arr = sub["background"][flav][era]
                F_arr = sub["fake"][flav][era]
                E_arr = sub["bkg_err2"][flav][era]

                for i in range(nbins):

                    f = F_arr[i]
                    b = B_arr[i]

                    f, b = fix_fake_and_bkg(
                        f, b, FAKE_FLOOR,
                        flavour=flav, era=era
                    )

                    S[i] += S_arr[i]
                    B[i] += b
                    E[i] += E_arr[i]

            # ----------------------------------------
            # Build valid edges
            # ----------------------------------------
            valid_edges = [e for e in edges_full if e >= MIN_EDGE]
            valid_edges = sorted(set(snap_to_variable_grid(e) for e in valid_edges))

            if MIN_EDGE not in valid_edges:
                valid_edges = [MIN_EDGE] + valid_edges

            if MAX_EDGE not in valid_edges:
                valid_edges.append(MAX_EDGE)

            valid_edges = sorted(set(valid_edges))

            print("[SCAN RANGE] {} MET={}".format(cat, met))
            print("  Original range: [{}, {}]".format(edges_full[0], edges_full[-1]))
            print("  Scan range:     [{}, {}]".format(valid_edges[0], valid_edges[-1]))
            print("  N edges: {}".format(len(valid_edges)))

            # ----------------------------------------
            # Generate candidates
            # ----------------------------------------
            nbins_target = get_nbins_for_region(cat)

            binnings = generate_fixed_nbin_binnings(
                valid_edges,
                nbins_target,
                min_width=MIN_WIDTH
            )

            best_Z = -1
            best_bins = None

            # ----------------------------------------
            # Scan loop
            # ----------------------------------------
            for b in tqdm(binnings, desc="{} MET{}".format(cat, met), leave=False):

                if abs(b[-1] - MAX_EDGE) > 1e-6:
                    continue

                # ALL bins must pass stat+unc
                if not passes_all_bins(b, S, B, E, bin_lo, flav):
                    continue

                Z = compute_fom(b, S, B, E, bin_lo, flav)

                if Z > best_Z:
                    best_Z = Z
                    best_bins = b

            if best_bins is None:
                print("[WARN] No valid binning found")
                continue

            # ----------------------------------------
            # Build bin details
            # ----------------------------------------
            bin_details = []

            for i in range(len(best_bins) - 1):

                lo = best_bins[i]
                hi = best_bins[i + 1]

                if i == len(best_bins) - 2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                s = S[mask].sum()
                b = B[mask].sum()
                e = E[mask].sum()

                if s > 0 and b > 1e-12:
                    z = compute_bin_Z_with_unc(s, b, e)
                else:
                    z = 0.0

                bin_details.append({
                    "lo": lo,
                    "hi": hi,
                    "S": s,
                    "B": b,
                    "E": e,
                    "Z": z
                })

            region_results[cat] = {
                "bins": best_bins,
                "per_era": {
                    "Run2": bin_details
                },
                "Z": best_Z
            }

            print("[RESULT] bestZ={:.4f}".format(best_Z))

            total_Z2 += best_Z * best_Z

        if total_Z2 <= 0:
            continue

        total_Z = math.sqrt(total_Z2)

        print("\n[MET {}] TOTAL Z = {:.4f}".format(met, total_Z))

        per_met_results[met] = {
            "Z": total_Z,
            "regions": region_results
        }

        if total_Z > best_fom:
            best_fom = total_Z
            best_global = {
                "met": met,
                "regions": region_results,
                "Z": total_Z
            }

    if best_global is None:
        return None

    return {
        "best_met": best_global["met"],
        "best_Z": best_global["Z"],
        "best_regions": best_global["regions"],
        "per_met": per_met_results
    }


def scan_nbin_sr3(data, flav, mass):

    from tqdm import tqdm
    import math
    import numpy as np

    METS = ["2", "3", "4", "5"]

    CATEGORIES = [
        "LowJet_LT_MET{X}_LTcut",
        "HighJet_LT_MET{X}_LTcut",
        "LowJet_LT_MET{X}_GTcut",
        "HighJet_LT_MET{X}_GTcut",
    ]

    MIN_EDGE = 150
    MIN_WIDTH = 10

    best_global = None
    best_fom = -1

    per_met_results = {}

    print("\n==============================")
    print(" SR3 SCAN | {} | mass={}".format(flav, mass))
    print("==============================")

    def find_tail_threshold(B, E, edges):

        total_B = 0.0
        total_E = 0.0

        for i in reversed(range(len(B))):

            total_B += B[i]
            total_E += E[i]

            rel = math.sqrt(total_E) / total_B if total_B > 0 else 0

            if pass_stat_and_err(total_B, rel):
                return edges[i]

        return edges[-2]

    # ----------------------------------------
    # MET loop
    # ----------------------------------------
    for met in METS:

        print("\n================ MET {} =================".format(met))

        total_Z2 = 0.0
        region_results = {}

        for cat_template in CATEGORIES:

            cat = cat_template.format(X=met)

            print("\n--- REGION: {} ---".format(cat))

            sub = data[met][cat]

            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]
            nbins = len(bin_lo)

            MAX_EDGE = edges_full[-1]

            # ----------------------------------------
            # Build Run2 arrays WITH fake fix
            # ----------------------------------------
            S = np.zeros(nbins)
            B = np.zeros(nbins)
            E = np.zeros(nbins)

            for era in ERAS:

                S_arr = sub["signal"][flav][mass][era]
                B_arr = sub["background"][flav][era]
                F_arr = sub["fake"][flav][era]
                E_arr = sub["bkg_err2"][flav][era]

                for i in range(nbins):

                    f = F_arr[i]
                    b = B_arr[i]

                    f, b = fix_fake_and_bkg(
                        f, b, FAKE_FLOOR,
                        flavour=flav, era=era
                    )

                    S[i] += S_arr[i]
                    B[i] += b
                    E[i] += E_arr[i]

            # ----------------------------------------
            # Tail threshold (diagnostic only)
            # ----------------------------------------
            X_tail = find_tail_threshold(B, E, edges_full)

            # ----------------------------------------
            # Build valid edges
            # ----------------------------------------
            valid_edges = [e for e in edges_full if e >= MIN_EDGE]
            valid_edges = sorted(set(snap_to_variable_grid(e) for e in valid_edges))

            if MIN_EDGE not in valid_edges:
                valid_edges = [MIN_EDGE] + valid_edges

            if MAX_EDGE not in valid_edges:
                valid_edges.append(MAX_EDGE)

            valid_edges = sorted(set(valid_edges))

            print("[SCAN RANGE] {} MET={}".format(cat, met))
            print("  Original range: [{}, {}]".format(edges_full[0], edges_full[-1]))
            print("  Scan range:     [{}, {}]".format(valid_edges[0], valid_edges[-1]))
            print("  Tail threshold: {}".format(X_tail))
            print("  N edges: {}".format(len(valid_edges)))

            # ----------------------------------------
            # Scan nbins ranges
            # ----------------------------------------
            if "LTcut" in cat:
                nbins_list = [4, 5, 6, 7]
            else:
                nbins_list = [3, 4, 5]

            best_Z = -1
            best_bins = None
            best_nbins = None

            # ----------------------------------------
            # Loop over nbins choices
            # ----------------------------------------
            for nbins_target in nbins_list:

                binnings = generate_fixed_nbin_binnings(
                    valid_edges,
                    nbins_target,
                    min_width=MIN_WIDTH
                )

                print("[SCAN] nbins={} candidates={}".format(
                    nbins_target, len(binnings))
                )

                for b in tqdm(
                    binnings,
                    desc="{} MET{} nb{}".format(cat, met, nbins_target),
                    leave=False
                ):

                    if abs(b[-1] - MAX_EDGE) > 1e-6:
                        continue

                    # ----------------------------------------
                    # ENFORCE: ALL bins must pass stat+unc
                    # ----------------------------------------
                    if not passes_all_bins(b, S, B, E, bin_lo, flav):
                        continue

                    Z = compute_fom(b, S, B, E, bin_lo, flav)

                    if Z > best_Z:
                        best_Z = Z
                        best_bins = b
                        best_nbins = nbins_target

            if best_bins is None:
                print("[WARN] No valid binning found")
                continue

            print("[BEST] nbins={} Z={:.4f}".format(best_nbins, best_Z))

            # ----------------------------------------
            # Build bin details
            # ----------------------------------------
            bin_details = []

            for i in range(len(best_bins) - 1):

                lo = best_bins[i]
                hi = best_bins[i + 1]

                if i == len(best_bins) - 2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                s = S[mask].sum()
                b = B[mask].sum()
                e = E[mask].sum()

                if s > 0 and b > 1e-12:
                    z = compute_bin_Z_with_unc(s, b, e)
                else:
                    z = 0.0

                bin_details.append({
                    "lo": lo,
                    "hi": hi,
                    "S": s,
                    "B": b,
                    "E": e,
                    "Z": z
                })

            region_results[cat] = {
                "bins": best_bins,
                "nbins": best_nbins,
                "per_era": {
                    "Run2": bin_details
                },
                "Z": best_Z
            }

            print("[RESULT] bestZ={:.4f}".format(best_Z))

            total_Z2 += best_Z * best_Z

        if total_Z2 <= 0:
            continue

        total_Z = math.sqrt(total_Z2)

        print("\n[MET {}] TOTAL Z = {:.4f}".format(met, total_Z))

        per_met_results[met] = {
            "Z": total_Z,
            "regions": region_results
        }

        if total_Z > best_fom:
            best_fom = total_Z
            best_global = {
                "met": met,
                "regions": region_results,
                "Z": total_Z
            }

    if best_global is None:
        return None

    return {
        "best_met": best_global["met"],
        "best_Z": best_global["Z"],
        "best_regions": best_global["regions"],
        "per_met": per_met_results
    }




def scan_global_sr3(data, flav, masses):

    from tqdm import tqdm
    import math
    import numpy as np

    METS = ["2", "3", "4", "5"]

    CATEGORIES = [
        "LowJet_LT_MET{X}_LTcut",
        "HighJet_LT_MET{X}_LTcut",
        "LowJet_LT_MET{X}_GTcut",
        "HighJet_LT_MET{X}_GTcut",
    ]

    MIN_EDGE = 150
    MIN_WIDTH = 10

    best_global = None
    best_fom = -1

    print("\n==============================")
    print(f" GLOBAL SR3 SCAN | {flav} | masses={masses}")
    print("==============================")

    # =========================================================
    # LOOP OVER MET
    # =========================================================
    for met in METS:

        print(f"\n================ MET {met} =================")

        region_results = {}

        # store per-mass arrays per region
        S_all, B_all, E_all = {}, {}, {}
        for mass in masses:
            S_all[mass] = {}
            B_all[mass] = {}
            E_all[mass] = {}

        # ----------------------------------------
        # REGION LOOP
        # ----------------------------------------
        for cat_template in CATEGORIES:

            cat = cat_template.format(X=met)
            print(f"\n--- REGION: {cat} ---")

            sub = data[met][cat]

            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]
            nbins = len(bin_lo)
            MAX_EDGE = edges_full[-1]

            # ----------------------------------------
            # Build Run2 arrays PER MASS (WITH fake fix)
            # ----------------------------------------
            for mass in masses:

                S = np.zeros(nbins)
                B = np.zeros(nbins)
                E = np.zeros(nbins)

                for era in ERAS:

                    S_arr = sub["signal"][flav][mass][era]
                    B_arr = sub["background"][flav][era]
                    F_arr = sub["fake"][flav][era]
                    E_arr = sub["bkg_err2"][flav][era]

                    for i in range(nbins):

                        f, b = fix_fake_and_bkg(
                            F_arr[i], B_arr[i], FAKE_FLOOR,
                            flavour=flav, era=era
                        )

                        S[i] += S_arr[i]
                        B[i] += b
                        E[i] += E_arr[i]

                S_all[mass][cat] = S
                B_all[mass][cat] = B
                E_all[mass][cat] = E

            # ----------------------------------------
            # Build valid edges
            # ----------------------------------------
            valid_edges = [e for e in edges_full if e >= MIN_EDGE]
            valid_edges = sorted(set(snap_to_variable_grid(e) for e in valid_edges))

            if MIN_EDGE not in valid_edges:
                valid_edges = [MIN_EDGE] + valid_edges

            if MAX_EDGE not in valid_edges:
                valid_edges.append(MAX_EDGE)

            valid_edges = sorted(set(valid_edges))

            print(f"[SCAN RANGE] {cat} MET={met}")
            print(f"  N edges: {len(valid_edges)}")

            nbins_target = get_nbins_for_region(cat)

            binnings = generate_fixed_nbin_binnings(
                valid_edges,
                nbins_target,
                min_width=MIN_WIDTH
            )

            best_Z = -1
            best_bins = None

            # ----------------------------------------
            # SCAN LOOP
            # ----------------------------------------
            for b in tqdm(binnings, desc=f"{cat} MET{met}", leave=False):

                if abs(b[-1] - MAX_EDGE) > 1e-6:
                    continue

                # ----------------------------------------
                # REQUIRE: ALL bins pass for ALL masses
                # ----------------------------------------
                valid = True
                for mass in masses:

                    if not passes_all_bins(
                        b,
                        S_all[mass][cat],
                        B_all[mass][cat],
                        E_all[mass][cat],
                        bin_lo,
                        flav
                    ):
                        valid = False
                        break

                if not valid:
                    continue

                # ----------------------------------------
                # GLOBAL FOM (sum over masses)
                # ----------------------------------------
                Z_sum = 0.0

                for mass in masses:
                    Z_sum += compute_fom(
                        b,
                        S_all[mass][cat],
                        B_all[mass][cat],
                        E_all[mass][cat],
                        bin_lo,
                        flav
                    )

                if Z_sum > best_Z:
                    best_Z = Z_sum
                    best_bins = b

            if best_bins is None:
                print("[WARN] No valid binning found")
                continue

            print(f"[BEST] Zsum={best_Z:.4f}")

            region_results[cat] = {
                "bins": best_bins
            }

        # ----------------------------------------
        # Evaluate MET ranking (use first mass)
        # ----------------------------------------
        total_Z2 = 0.0

        for cat in region_results:

            bins = region_results[cat]["bins"]
            mass0 = masses[0]

            sub = data[met][cat]
            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]

            S = S_all[mass0][cat]
            B = B_all[mass0][cat]
            E = E_all[mass0][cat]

            Z = compute_fom(bins, S, B, E, bin_lo, flav)
            total_Z2 += Z * Z

        total_Z = math.sqrt(total_Z2)

        print(f"\n[MET {met}] TOTAL Z = {total_Z:.4f}")

        if total_Z > best_fom:
            best_fom = total_Z
            best_global = {
                "met": met,
                "regions": region_results
            }

    if best_global is None:
        return None

    # =========================================================
    # FINAL: BUILD PER-MASS OUTPUT
    # =========================================================
    results_per_mass = []

    best_met = best_global["met"]
    regions = best_global["regions"]

    for mass in masses:

        total_Z2 = 0.0
        region_results_mass = {}

        for cat in regions:

            bins = regions[cat]["bins"]

            sub = data[best_met][cat]
            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]

            S = S_all[mass][cat]
            B = B_all[mass][cat]
            E = E_all[mass][cat]

            Z = compute_fom(bins, S, B, E, bin_lo, flav)
            total_Z2 += Z * Z

            bin_details = []

            for i in range(len(bins) - 1):

                lo, hi = bins[i], bins[i + 1]

                if i == len(bins) - 2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                s = S[mask].sum()
                b = B[mask].sum()
                e = E[mask].sum()

                z = compute_bin_Z_with_unc(s, b, e) if (s > 0 and b > 1e-12) else 0.0

                bin_details.append({
                    "lo": lo,
                    "hi": hi,
                    "S": s,
                    "B": b,
                    "E": e,
                    "Z": z
                })

            region_results_mass[cat] = {
                "bins": bins,
                "per_era": {"Run2": bin_details},
                "Z": Z
            }

        results_per_mass.append({
            "flav": flav,
            "mass": mass,
            "best_met": best_met,
            "best_Z": math.sqrt(total_Z2),
            "best_regions": region_results_mass,
            "per_met": None
        })

    return results_per_mass



def scan_sr3_dp(data, flav, mass):

    import math
    import numpy as np
    from tqdm import tqdm

    METS = ["2", "3", "4", "5"]

    CATEGORIES = [
        "LowJet_LT_MET{X}_LTcut",
        "HighJet_LT_MET{X}_LTcut",
        "LowJet_LT_MET{X}_GTcut",
        "HighJet_LT_MET{X}_GTcut",
    ]

    MIN_WIDTH = 10
    MIN_EDGE = 150

    best_global = None
    best_fom = -1

    per_met_results = {}

    print("\n==============================")
    print(" DP SCAN START | {} | mass={}".format(flav, mass))
    print("==============================")

    # ----------------------------------------
    # helper: tail-based stat boundary (diagnostic only)
    # ----------------------------------------
    def find_tail_threshold(B, E, edges):

        total_B = 0.0
        total_E = 0.0

        for i in reversed(range(len(B))):

            total_B += B[i]
            total_E += E[i]

            rel = math.sqrt(total_E) / total_B if total_B > 0 else 0
            print(f"{edges[i]:.0f}  B={total_B:.3e}  rel={rel:.3f}")

            if pass_stat_and_err(total_B, rel):
                print("PASS at", edges[i])
                return edges[i]

        return edges[-2]

    # ----------------------------------------
    # MET loop
    # ----------------------------------------
    for met in METS:

        print("\n[MET = {}]".format(met))

        total_Z2 = 0.0
        region_results = {}

        for cat_template in CATEGORIES:

            cat = cat_template.format(X=met)

            print("\n--- REGION: {} ---".format(cat))

            sub = data[met][cat]

            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]
            nbins = len(bin_lo)

            MAX_EDGE = edges_full[-1]

            # ----------------------------------------
            # Build Run2 arrays WITH fake fix
            # ----------------------------------------
            S = np.zeros(nbins)
            B = np.zeros(nbins)
            E = np.zeros(nbins)

            for era in ERAS:

                S_arr = sub["signal"][flav][mass][era]
                B_arr = sub["background"][flav][era]
                F_arr = sub["fake"][flav][era]
                E_arr = sub["bkg_err2"][flav][era]

                for i in range(nbins):

                    f = F_arr[i]
                    b = B_arr[i]

                    f, b = fix_fake_and_bkg(
                        f, b, FAKE_FLOOR,
                        flavour=flav, era=era
                    )

                    S[i] += S_arr[i]
                    B[i] += b
                    E[i] += E_arr[i]

            # ----------------------------------------
            # Tail threshold (diagnostic)
            # ----------------------------------------
            X_tail = find_tail_threshold(B, E, edges_full)

            # ----------------------------------------
            # Build valid edges
            # ----------------------------------------
            valid_edges = [e for e in edges_full if e >= MIN_EDGE]
            valid_edges = sorted(set(snap_to_variable_grid(e) for e in valid_edges))

            if MIN_EDGE not in valid_edges:
                valid_edges = [MIN_EDGE] + valid_edges

            if MAX_EDGE not in valid_edges:
                valid_edges.append(MAX_EDGE)

            valid_edges = sorted(set(valid_edges))

            print("[SCAN RANGE] {} MET={}".format(cat, met))
            print("  Original range: [{}, {}]".format(edges_full[0], edges_full[-1]))
            print("  Scan range:     [{}, {}]".format(valid_edges[0], valid_edges[-1]))
            print("  Tail threshold: {}".format(X_tail))
            print("  N edges: {}".format(len(valid_edges)))

            nbins_target = get_nbins_for_region(cat)

            counter = 0
            pruned = 0
            valid_final = 0

            best_Z = -1
            best_bins = None

            pbar = tqdm(desc="{} MET{}".format(cat, met), unit="nodes")

            # ----------------------------------------
            # DP recursion
            # ----------------------------------------
            def recurse(current_edges, start_idx, Z2_accum):

                nonlocal counter, pruned, valid_final
                nonlocal best_Z, best_bins

                counter += 1

                if counter % 1000 == 0:
                    pbar.update(1000)
                    pbar.set_postfix({"bestZ": "{:.3f}".format(best_Z)})

                nbins_now = len(current_edges) - 1

                if nbins_now > nbins_target:
                    return

                # ----------------------------------------
                # Final candidate
                # ----------------------------------------
                if nbins_now == nbins_target:

                    if abs(current_edges[-1] - MAX_EDGE) > 1e-6:
                        return

                    valid_final += 1

                    Z = math.sqrt(Z2_accum)

                    if Z > best_Z:
                        best_Z = Z
                        best_bins = current_edges.copy()

                    return

                # ----------------------------------------
                # Try extending bins
                # ----------------------------------------
                for i in range(start_idx, len(valid_edges)):

                    next_edge = valid_edges[i]

                    if next_edge <= current_edges[-1]:
                        continue

                    if next_edge - current_edges[-1] < MIN_WIDTH:
                        continue

                    lo = current_edges[-1]
                    hi = next_edge

                    if next_edge == MAX_EDGE:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    s = S[mask].sum()
                    b = B[mask].sum()
                    e = E[mask].sum()

                    # ----------------------------------------
                    # ENFORCE stat on ALL bins
                    # ----------------------------------------
                    rel = math.sqrt(e) / b if b > 0 else 0
                    if not pass_stat_and_err(b, rel):
                        pruned += 1
                        continue

                    # ----------------------------------------
                    # Incremental Z
                    # ----------------------------------------
                    if s > 0 and b > 1e-12:
                        Z_bin = compute_bin_Z_with_unc(s, b, e)
                        Z2_new = Z2_accum + Z_bin * Z_bin
                    else:
                        Z2_new = Z2_accum

                    recurse(current_edges + [next_edge], i + 1, Z2_new)

            recurse([MIN_EDGE], 0, 0.0)
            pbar.close()

            if best_bins is None:
                best_bins = [MIN_EDGE, MAX_EDGE]
                best_Z = 0.0

            # ----------------------------------------
            # Build bin details
            # ----------------------------------------
            bin_details = []

            for i in range(len(best_bins) - 1):

                lo = best_bins[i]
                hi = best_bins[i + 1]

                if i == len(best_bins) - 2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                s = S[mask].sum()
                b = B[mask].sum()
                e = E[mask].sum()

                if s > 0 and b > 1e-12:
                    z = compute_bin_Z_with_unc(s, b, e)
                else:
                    z = 0.0

                bin_details.append({
                    "lo": lo,
                    "hi": hi,
                    "S": s,
                    "B": b,
                    "E": e,
                    "Z": z
                })

            region_results[cat] = {
                "bins": best_bins,
                "per_era": {
                    "Run2": bin_details
                },
                "Z": best_Z
            }

            total_Z2 += best_Z * best_Z

            print("[RESULT] bestZ={:.4f}".format(best_Z))
            print("[STATS] explored={}, pruned={}, valid={}".format(
                counter, pruned, valid_final
            ))

        total_Z = math.sqrt(total_Z2)

        print("\n[MET {}] TOTAL Z = {:.4f}".format(met, total_Z))

        per_met_results[met] = {
            "Z": total_Z,
            "regions": region_results
        }

        if total_Z > best_fom:
            best_fom = total_Z
            best_global = {
                "met": met,
                "regions": region_results,
                "Z": total_Z
            }

    if best_global is None:
        return None

    return {
        "best_met": best_global["met"],
        "best_Z": best_global["Z"],
        "best_regions": best_global["regions"],
        "per_met": per_met_results
    }




def evaluate_sr3_scan_parallel(data, masses, run_scan="PerMass", n_workers=None):

    
    import numpy as np
    import math

    if n_workers is None:
        n_workers = max(1, cpu_count() - 1)

    print("\n==============================")
    print(f" SR3 SCAN RESULTS (PARALLEL) {run_scan}")
    print("==============================")
    print("Workers:", n_workers)

    # ----------------------------------------
    # Prepare tasks
    # ----------------------------------------
    tasks = []
    for flav in FLAVOURS:
        if run_scan == "GlobalMass":
            tasks.append((data, flav, masses, run_scan))
        else:
            for mass in masses:
                tasks.append((data, flav, mass, run_scan))

    # ----------------------------------------
    # Run in parallel
    # ----------------------------------------
    with Pool(n_workers) as pool:
        raw_outputs = pool.map(run_scan_single, tasks)

    # ----------------------------------------
    # Flatten outputs (GlobalMass returns list)
    # ----------------------------------------
    outputs = []
    for out in raw_outputs:
        if isinstance(out, list):
            outputs.extend(out)
        else:
            outputs.append(out)

    results = []

    # ======================================================
    # LOOP OVER RESULTS (UNIFIED FOR ALL MODES)
    # ======================================================
    for best in outputs:

        if best is None:
            continue

        flav = best["flav"]
        mass = best["mass"]

        if mass == masses[0]:
            print("\n================ {} =================".format(flav))

        met = best["best_met"]
        regions = best["best_regions"]

        total_Z2_run2 = 0.0
        total_Z2_quad = 0.0

        for cat in regions:

            bins = regions[cat]["bins"]

            sub = data[met][cat]

            edges_full = sub["edges"]
            bin_lo = edges_full[:-1]
            nbins = len(bin_lo)

            # ----------------------------------------
            # Build Run2 arrays WITH fake fix
            # ----------------------------------------
            S = np.zeros(nbins)
            B = np.zeros(nbins)
            E = np.zeros(nbins)

            for era in ERAS:

                S_arr = sub["signal"][flav][mass][era]
                B_arr = sub["background"][flav][era]
                F_arr = sub["fake"][flav][era]
                E_arr = sub["bkg_err2"][flav][era]

                for i in range(nbins):

                    f, b = fix_fake_and_bkg(
                        F_arr[i], B_arr[i], FAKE_FLOOR,
                        flavour=flav, era=era
                    )

                    S[i] += S_arr[i]
                    B[i] += b
                    E[i] += E_arr[i]

            # ----------------------------------------
            # Run2 significance
            # ----------------------------------------
            Z_run2 = compute_fom(bins, S, B, E, bin_lo, flav)
            total_Z2_run2 += Z_run2 * Z_run2

            # ----------------------------------------
            # Quad significance (per-era)
            # ----------------------------------------
            Z2_quad_region = 0.0

            for era in ERAS:

                S_arr = sub["signal"][flav][mass][era]
                B_arr = sub["background"][flav][era]
                F_arr = sub["fake"][flav][era]
                E_arr = sub["bkg_err2"][flav][era]

                Z2_era = 0.0

                for i_bin in range(len(bins) - 1):

                    lo = bins[i_bin]
                    hi = bins[i_bin + 1]

                    if i_bin == len(bins) - 2:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    s = S_arr[mask].sum()
                    b = B_arr[mask].sum()
                    f = F_arr[mask].sum()
                    e = E_arr[mask].sum()

                    f, b = fix_fake_and_bkg(
                        f, b, FAKE_FLOOR,
                        flavour=flav, era=era
                    )

                    if s > 0 and b > 1e-12:
                        Z = compute_bin_Z_with_unc(s, b, e)
                        Z2_era += Z * Z

                Z2_quad_region += Z2_era

            total_Z2_quad += Z2_quad_region

        run2 = math.sqrt(total_Z2_run2)
        quad = math.sqrt(total_Z2_quad)

        # ----------------------------------------
        # Consistency check
        # ----------------------------------------
        scan_Z = best["best_Z"]

        if abs(scan_Z - run2) > 1e-6:
            print("[WARNING] mismatch for {} {}: scan={:.6f}, recomputed={:.6f}".format(
                flav, mass, scan_Z, run2
            ))

        print("{} {} -> MET={} Run2={:.4f} Quad={:.4f}".format(
            flav, mass, met, run2, quad
        ))

        results.append({
            "flav": flav,
            "mass": mass,
            "run2": run2,
            "quad": quad,
            "met": met,
            "regions": regions,
            "per_met": best["per_met"]
        })

    return results





def evaluate_sr3_scan(data, masses, run_dp_scan=False):

    import numpy as np
    import math

    results = []

    print("\n==============================")
    print(" SR3 SCAN RESULTS")
    print("==============================")

    for flav in FLAVOURS:

        print(f"\n================ {flav} =================")

        for mass in masses:

            # --------------------------
            # Run scan
            # --------------------------
            if run_dp_scan:
                best = scan_sr3_dp(data, flav, mass)
            else:
                best = scan_sr3(data, flav, mass)

            if best is None:
                print(f"[WARN] No valid result for {flav}, {mass}")
                continue

            met = best["best_met"]
            regions = best["best_regions"]
            per_met = best["per_met"]

            total_Z2_run2 = 0.0
            total_Z2_quad = 0.0

            for cat in regions:

                bins = regions[cat]["bins"]

                sub = data[met][cat]

                edges_full = sub["edges"]
                bin_lo = edges_full[:-1]
                nbins = len(bin_lo)

                # ======================================================
                # FIXED RUN2: build arrays WITH fake fix (like scan)
                # ======================================================
                S = np.zeros(nbins)
                B = np.zeros(nbins)
                E = np.zeros(nbins)

                for era in ERAS:

                    S_arr = sub["signal"][flav][mass][era]
                    B_arr = sub["background"][flav][era]
                    F_arr = sub["fake"][flav][era]
                    E_arr = sub["bkg_err2"][flav][era]

                    for i in range(nbins):

                        f = F_arr[i]
                        b = B_arr[i]

                        f, b = fix_fake_and_bkg(
                            f, b, FAKE_FLOOR,
                            flavour=flav, era=era
                        )

                        S[i] += S_arr[i]
                        B[i] += b
                        E[i] += E_arr[i]

                # --------------------------
                # Run2 FOM
                # --------------------------
                Z_run2 = compute_fom(bins, S, B, E, bin_lo, flav)
                total_Z2_run2 += Z_run2 * Z_run2

                # ======================================================
                # QUAD: per-era accumulation (already correct)
                # ======================================================
                Z2_quad_region = 0.0

                for era in ERAS:

                    S_arr = sub["signal"][flav][mass][era]
                    B_arr = sub["background"][flav][era]
                    F_arr = sub["fake"][flav][era]
                    E_arr = sub["bkg_err2"][flav][era]

                    Z2_era = 0.0

                    for i in range(len(bins) - 1):

                        lo = bins[i]
                        hi = bins[i + 1]

                        if i == len(bins) - 2:
                            mask = (bin_lo >= lo) & (bin_lo <= hi)
                        else:
                            mask = (bin_lo >= lo) & (bin_lo < hi)

                        s = S_arr[mask].sum()
                        b = B_arr[mask].sum()
                        f = F_arr[mask].sum()
                        e = E_arr[mask].sum()

                        # fake fix per era
                        f, b = fix_fake_and_bkg(
                            f, b, FAKE_FLOOR,
                            flavour=flav, era=era
                        )

                        if s > 0 and b > 1e-12:
                            Z = compute_bin_Z_with_unc(s, b, e)
                            Z2_era += Z * Z

                    Z2_quad_region += Z2_era

                total_Z2_quad += Z2_quad_region

            run2 = math.sqrt(total_Z2_run2)
            quad = math.sqrt(total_Z2_quad)

            # --------------------------
            # Consistency check
            # --------------------------
            scan_Z = best["best_Z"]

            if abs(scan_Z - run2) > 1e-6:
                print(f"[WARNING] mismatch for {flav} {mass}: "
                      f"scan={scan_Z:.6f}, recomputed={run2:.6f}")

            print(f"{flav} {mass}  MET={met}  Run2={run2:.4f}  Quad={quad:.4f}")

            results.append({
                "flav": flav,
                "mass": mass,
                "run2": run2,
                "quad": quad,
                "met": met,
                "regions": regions,
                "per_met": per_met,
            })

    return results



