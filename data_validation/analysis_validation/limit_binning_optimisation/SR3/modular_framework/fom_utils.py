# =========================================================                                                                                                                                                                                  
# EVALUATORs                                                                                                                                                                                                                                 
# =========================================================                                                                                                                                                                                 

from default_config import ERAS, FLAVOURS, FAKE_FLOOR, MIN_SIGNAL_FRAC,NO_CUMSUM,TEST_COMPARE

from helper import fix_fake_and_bkg,compute_bin_Z_with_unc,pass_stat_and_err

import math
import itertools
from tqdm import tqdm


def compare_binnings_physics(data, flav, mass, met, cat, bins_a, bins_b):

    print("\n========================================")
    print(f"COMPARE PHYSICS | {cat} | MET={met}")
    print("========================================")

    # Run2-combined arrays
    S, B, F, E = build_run2_arrays(data, flav, mass, met, cat)

    edges_full = data[met][cat]["edges"]
    bin_lo = edges_full[:-1]

    def extract(bins):

        out = []

        for i in range(len(bins)-1):

            lo = bins[i]
            hi = bins[i+1]

            if i == len(bins)-2:
                mask = (bin_lo >= lo) & (bin_lo <= hi)
            else:
                mask = (bin_lo >= lo) & (bin_lo < hi)

            s = S[mask].sum()
            b = B[mask].sum()
            f = F[mask].sum()
            e = E[mask].sum()

            # Apply fake fix AFTER summing (Run2-level)
            f, b = fix_fake_and_bkg(f, b, FAKE_FLOOR,
                                   flavour=flav, era=None)

            # Numerical safety (important)
            if s > 0 and b > 1e-12:
                z = compute_bin_Z_with_unc(s, b, e)
            else:
                z = 0.0

            out.append((lo, hi, s, b, e, z))

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
    else:
        step = 50

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

def passes_all_bins(edges, S, B, F, E, bin_lo, flav=None):

    for i in range(len(edges) - 1):

        lo = edges[i]
        hi = edges[i + 1]

        if i == len(edges) - 2:
            mask = (bin_lo >= lo) & (bin_lo <= hi)
        else:
            mask = (bin_lo >= lo) & (bin_lo < hi)

        b = B[mask].sum()
        f = F[mask].sum()
        e = E[mask].sum()

        # Apply fake fix AFTER summing (Run2-level)
        f, b = fix_fake_and_bkg(
            f, b, FAKE_FLOOR,
            flavour=flav, era=None
        )

        rel = math.sqrt(e) / b if b > 0 else 0

        # ONLY enforce stat requirement on LAST bin
        if i == len(edges) - 2:
            if not pass_stat_and_err(b, rel):
                return False

    return True


def compute_fom(edges, S, B, F, E, bin_lo, flav=None):

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
        f = F[mask].sum()
        e = E[mask].sum()

        # Apply fake fix AFTER summing (Run2-level)
        f, b = fix_fake_and_bkg(
            f, b, FAKE_FLOOR,
            flavour=flav, era=None
        )

        # Numerical safety (important)
        if s > 0 and b > 1e-12:
            Z = compute_bin_Z_with_unc(s, b, e)
            Z2 += Z * Z

    return math.sqrt(Z2)



def scan_sr3(data, flav, mass):

    from tqdm import tqdm
    import math

    METS = ["2", "3", "4", "5"]

    CATEGORIES = [
        "LowJet_LT_MET{X}_LTcut",
        "HighJet_LT_MET{X}_LTcut",
        "LowJet_LT_MET{X}_GTcut",
        "HighJet_LT_MET{X}_GTcut",
    ]

    MIN_EDGE = 150

    best_global = None
    best_fom = -1

    # NEW: store all MET results
    per_met_results = {}

    for met in METS:

        total_Z2 = 0.0
        region_results = {}

        print("\n================ MET {} =================".format(met))

        for cat_template in CATEGORIES:

            cat = cat_template.format(X=met)

            print("\n--- REGION: {} ---".format(cat))

            # Build Run2 arrays
            S, B, F, E = build_run2_arrays(data, flav, mass, met, cat)

            edges = data[met][cat]["edges"]
            bin_lo = edges[:-1]

            MAX_EDGE = edges[-1]

            # Build valid edges
            valid_edges = [e for e in edges if e >= MIN_EDGE]
            valid_edges = sorted(set(snap_to_variable_grid(e) for e in valid_edges))

            if MIN_EDGE not in valid_edges:
                valid_edges = [MIN_EDGE] + valid_edges

            if MAX_EDGE not in valid_edges:
                valid_edges.append(MAX_EDGE)

            valid_edges = sorted(set(valid_edges))

            print("[SCAN RANGE] {} MET={}".format(cat, met))
            print("  Original range: [{}, {}]".format(edges[0], edges[-1]))
            print("  Scan range:     [{}, {}]".format(valid_edges[0], valid_edges[-1]))
            print("  N edges: {}".format(len(valid_edges)))

            nbins_target = get_nbins_for_region(cat)

            binnings = generate_fixed_nbin_binnings(
                valid_edges, nbins_target, min_width=10
            )

            print("[SCAN] candidates={}".format(len(binnings)))

            best_Z = -1
            best_bins = None

            for b in tqdm(binnings, desc="{} MET{}".format(cat, met), leave=False):

                if abs(b[-1] - MAX_EDGE) > 1e-6:
                    continue

                if not passes_all_bins(b, S, B, F, E, bin_lo, flav):
                    continue

                Z = compute_fom(b, S, B, F, E, bin_lo, flav)

                if Z > best_Z:
                    best_Z = Z
                    best_bins = b

            if best_bins is None:
                print("[WARN] No valid binning found")
                continue

            # Build bin details
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
                f = F[mask].sum()
                e = E[mask].sum()

                f, b = fix_fake_and_bkg(
                    f, b, FAKE_FLOOR,
                    flavour=flav, era=None
                )

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

        # NEW: store per-MET result
        per_met_results[met] = {
            "Z": total_Z,
            "regions": region_results
        }

        # Track best MET
        if total_Z > best_fom:
            best_fom = total_Z
            best_global = {
                "met": met,
                "regions": region_results,
                "Z": total_Z
            }

    if best_global is None:
        return None

    # FINAL RETURN (UPDATED)
    return {
        "best_met": best_global["met"],
        "best_Z": best_global["Z"],
        "best_regions": best_global["regions"],
        "per_met": per_met_results
    }




def scan_sr3_dp(data, flav, mass):

    import math
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

    # NEW: store all MET results
    per_met_results = {}

    print("\n==============================")
    print(" DP SCAN START | {} | mass={}".format(flav, mass))
    print("==============================")

    for met in METS:

        print("\n[MET = {}]".format(met))

        total_Z2 = 0.0
        region_results = {}

        for cat_template in CATEGORIES:

            cat = cat_template.format(X=met)

            print("\n--- REGION: {} ---".format(cat))

            # Build Run2 arrays
            S, B, F, E = build_run2_arrays(data, flav, mass, met, cat)

            edges_full = data[met][cat]["edges"]
            bin_lo = edges_full[:-1]

            MAX_EDGE = edges_full[-1]

            # Build valid edges
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

            nbins_target = get_nbins_for_region(cat)

            counter = 0
            pruned = 0
            valid_final = 0

            best_Z = -1
            best_bins = None

            pbar = tqdm(desc="{} MET{}".format(cat, met), unit="nodes")

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

                # Final candidate
                if nbins_now == nbins_target:

                    if abs(current_edges[-1] - MAX_EDGE) > 1e-6:
                        return

                    valid_final += 1

                    Z = math.sqrt(Z2_accum)

                    if Z > best_Z:
                        best_Z = Z
                        best_bins = current_edges.copy()

                    return

                for i in range(start_idx, len(valid_edges)):

                    next_edge = valid_edges[i]

                    if next_edge <= current_edges[-1]:
                        continue

                    if next_edge - current_edges[-1] < MIN_WIDTH:
                        continue

                    lo = current_edges[-1]
                    hi = next_edge

                    if i == len(valid_edges) - 1:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    s = S[mask].sum()
                    b = B[mask].sum()
                    f = F[mask].sum()
                    e = E[mask].sum()

                    f, b = fix_fake_and_bkg(
                        f, b, FAKE_FLOOR,
                        flavour=flav, era=None
                    )

                    rel = math.sqrt(e) / b if b > 0 else 0

                    # Only enforce stat on last bin
                    is_last_edge = (i == len(valid_edges) - 1)

                    if is_last_edge:
                        if not pass_stat_and_err(b, rel):
                            pruned += 1
                            continue

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

            # Build bin details
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
                f = F[mask].sum()
                e = E[mask].sum()

                f, b = fix_fake_and_bkg(
                    f, b, FAKE_FLOOR,
                    flavour=flav, era=None
                )

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

        # NEW: store per-MET
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


def evaluate_sr3_scan(data, masses, run_dp_scan=False):

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

            # UPDATED KEYS
            met = best["best_met"]
            regions = best["best_regions"]
            per_met = best["per_met"]

            # --------------------------
            # Recompute Run2 FOM
            # --------------------------
            total_Z2_run2 = 0.0

            # --------------------------
            # QUAD per-era accumulation
            # --------------------------
            total_Z2_quad = 0.0

            for cat in regions:

                bins = regions[cat]["bins"]

                # --------------------------
                # Run2 arrays
                # --------------------------
                S, B, F, E = build_run2_arrays(data, flav, mass, met, cat)

                edges_full = data[met][cat]["edges"]
                bin_lo = edges_full[:-1]

                Z_run2 = compute_fom(bins, S, B, F, E, bin_lo, flav)
                total_Z2_run2 += Z_run2 * Z_run2

                # --------------------------
                # QUAD: sum per-era Z^2
                # --------------------------
                Z2_quad_region = 0.0

                for era in ERAS:

                    sub = data[met][cat]

                    S_arr = sub["signal"][flav][mass][era]
                    B_arr = sub["background"][flav][era]
                    F_arr = sub["fake"][flav][era]
                    E_arr = sub["bkg_err2"][flav][era]

                    Z2_era = 0.0

                    for i in range(len(bins) - 1):

                        lo = bins[i]
                        hi = bins[i+1]

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
                "per_met": per_met,   # NEW
            })

    return results
