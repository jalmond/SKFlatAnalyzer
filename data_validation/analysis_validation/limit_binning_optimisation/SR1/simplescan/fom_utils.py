# =========================================================
# EVALUATORs
# =========================================================

from ref_bins import REF_BINS
from config import ERAS, FLAVOURS, FAKE_FLOOR, USE_FAKE_FIX
from logger import fmt_ratio
from helper import fix_fake_and_bkg,compute_bin_Z
import math

import numpy as np
import math

from tqdm import tqdm


DEBUG = False

def pass_stat(B):
    return (B >= 1.0)


def pass_stat_and_err(B, rel):
    return (B >= 1.0) or (B >= 0.5 and rel < 0.3)

def compute_bin_Z_with_unc(S, B, E):

    if S <= 0 or B <= 0:
        return 0.0

    sigma2 = E

    if sigma2 <= 0:
        return compute_bin_Z(S, B)

    term1 = (S + B) * math.log((S + B)*(B + sigma2) / (B*B + (S + B)*sigma2))
    term2 = (B*B / sigma2) * math.log(1 + sigma2*S / (B*(B + sigma2)))

    Z2 = 2 * (term1 - term2)

    return math.sqrt(Z2) if Z2 > 0 else 0.0


def run_dp_on_arrays(
        S, B, E, F,
    bin_lo,
    n_bins=6,
    use_fake_corr=True,
    flav=None,
    FAKE_FLOOR=0.0,
    min_signal_frac=0.01,
    stat_threshold=1.0
):

    N = len(S)

    # ----------------------------------
    # Prefix sums
    # ----------------------------------
    PS = np.concatenate(([0.0], np.cumsum(S)))
    PB = np.concatenate(([0.0], np.cumsum(B)))
    PF = np.concatenate(([0.0], np.cumsum(F)))
    PE = np.concatenate(([0.0], np.cumsum(E)))
    # ----------------------------------
    # LOW-EDGE SIGNAL FRACTION CUT
    # ----------------------------------
    S_total = PS[-1]

    min_edge_idx = 0
    for i in range(N):
        if S_total > 0 and (PS[i+1] / S_total) > min_signal_frac:
            min_edge_idx = i
            break
    if bin_lo[min_edge_idx] > 500:
        print(f"[INFO] Capping min edge from {bin_lo[min_edge_idx]:.1f} -> 500")

        for i in range(len(bin_lo)):
            if bin_lo[i] >= 500:
                min_edge_idx = i
                break
    # ----------------------------------
    # HIGH-EDGE STAT CUT (Run2)
    # ----------------------------------
    max_edge_idx = N

    for i in reversed(range(N)):
        B_tail = PB[N] - PB[i]

        if pass_stat(B_tail):
            max_edge_idx = i
            break

    print(
        f"[SCAN WINDOW] "
        f"[{bin_lo[0]:.1f}, {bin_lo[-1]+5:.1f}] --> "
        f"[{bin_lo[min_edge_idx]:.1f}, {bin_lo[max_edge_idx]:.1f}]"
    )
    
    print(
        f"[SCAN REDUCTION] "
        f"{len(bin_lo)} bins -> {max_edge_idx - min_edge_idx} bins"
    )
    if DEBUG:
        print("\n[DEBUG WINDOW DETAILS]")
        print(f"min_edge_idx = {min_edge_idx} (edge = {bin_lo[min_edge_idx]:.1f})")
        print(f"max_edge_idx = {max_edge_idx} (edge = {bin_lo[max_edge_idx]:.1f})")
        
        print("\n[TAIL BACKGROUND CHECK]")
        for i in range(len(bin_lo)):
            B_tail = PB[N] - PB[i]
            print(f"edge {bin_lo[i]:.1f} -> tail B = {B_tail:.4f}")
            
    # ----------------------------------
    # Interval function (returns Z^2)
    # ----------------------------------
    def interval_z2(p, i):

        S_int = PS[i] - PS[p]
        B_int = PB[i] - PB[p]
        F_int = PF[i] - PF[p]
        E_int = PE[i] - PE[p]
        if use_fake_corr:
            F_int, B_int = fix_fake_and_bkg(
                F_int, B_int, FAKE_FLOOR,
                flavour=flav, era="Run2"
            )

        if S_int <= 0 or B_int <= 0:
            return 0.0

        Z = compute_bin_Z_with_unc(S_int, B_int, E_int)
        return Z*Z

    # ----------------------------------
    # DP arrays
    # ----------------------------------
    NEG = -1e300
    dp  = np.full((n_bins+1, N+1), NEG)
    prv = np.full((n_bins+1, N+1), -1)

    dp[0,0] = 0.0

    # ----------------------------------
    # DP loop
    # ----------------------------------
    for j in range(1, n_bins+1):
        for i in range(1, N+1):

            best = NEG
            best_p = -1

            for p in range(i):

                # Per-bin stat check 
                
                B_int = PB[i] - PB[p]
                E_int = PE[i] - PE[p]

                # ----------------------------------                                                                 
                # Min width (25 GeV)                                                                             
                # ----------------------------------                                                                        
                if (bin_lo[i-1] - bin_lo[p]) < 25:
                    continue

                if B_int <= 0:
                    continue
                
                rel = math.sqrt(E_int) / B_int if B_int > 0 else 999.0
                
                
                if not pass_stat_and_err(B_int,rel):
                    continue
                
                
                # ----------------------------------
                # FIRST BIN: enforce low-edge cut
                # ----------------------------------
                if j == 1 and i <= min_edge_idx:
                    continue

                # ----------------------------------
                # LAST BIN: enforce high-edge cut
                # ----------------------------------
                if j == n_bins and p >= max_edge_idx:
                    continue

                # ----------------------------------
                # DP validity
                # ----------------------------------
                if dp[j-1, p] <= NEG/2:
                    continue

                z2 = interval_z2(p, i)
                val = dp[j-1, p] + z2

                if val > best:
                    best = val
                    best_p = p

            dp[j, i] = best
            prv[j, i] = best_p

    # ----------------------------------
    # Check solution
    # ----------------------------------
    if dp[n_bins, N] <= NEG/2:
        tqdm.write("[DP WARNING] No valid binning found")
        return None

    # ----------------------------------
    # Backtrack
    # ----------------------------------
    idx = []
    j, i = n_bins, N

    while j > 0:
        p = prv[j, i]
        if p < 0:
            return None
        idx.append(p)
        i = p
        j -= 1

    idx = list(reversed(idx))

    # ----------------------------------
    # Build edges
    # ----------------------------------
    edges = [bin_lo[0]]

    for k in idx:
        if k != 0:
            edges.append(bin_lo[k])

    edges.append(bin_lo[-1])

    return edges



# =========================================================


def evaluate_dp_per_mass(data, n_bins=6, use_fake_corr=True):


    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]

    total_jobs = sum(len(data["signal_combined_mass"][flav]) for flav in FLAVOURS)
    pbar = tqdm(total=total_jobs, desc="DP Scan")

    for flav in FLAVOURS:

        tqdm.write(f"[DP SCAN] Flavour = {flav}")
        
        for mass in data["signal_combined_mass"][flav]:

            tqdm.write(f"\n[Mass] {mass}")

            # ----------------------------------
            # Build Run2 arrays
            # ----------------------------------
            S_run2 = sum(data["signal"][flav][mass][era] for era in ERAS)
            B_run2 = sum(data["background"][flav][era] for era in ERAS)
            E_run2 = sum(data["bkg_err2"][flav][era] for era in ERAS)
            F_run2 = sum(data["fake"][flav][era] for era in ERAS)

            # ----------------------------------
            # Run DP (Run2)
            # ----------------------------------
            best_edges = run_dp_on_arrays(
                S_run2, B_run2, E_run2,F_run2,
                bin_lo,
                n_bins=n_bins,
                use_fake_corr=use_fake_corr,
                flav=flav,
                FAKE_FLOOR=FAKE_FLOOR
            )

            if best_edges is None:
                pbar.update(1)
                continue
            
            edges_run2 = best_edges
            edges_per_era = {era: best_edges for era in ERAS}
            
            # ----------------------------------
            # QUAD (evaluate per era)
            # ----------------------------------
            quad_Z2 = 0.0

            for era in ERAS:

                S_arr = data["signal"][flav][mass][era]
                B_arr = data["background"][flav][era]
                F_arr = data["fake"][flav][era]
                E_arr = data["bkg_err2"][flav][era]
                
                for i in range(len(best_edges)-1):

                    lo = best_edges[i]
                    hi = best_edges[i+1]

                    mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = S_arr[mask].sum()
                    B = B_arr[mask].sum()
                    F = F_arr[mask].sum()
                    E = E_arr[mask].sum()
                    
                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    Z = compute_bin_Z_with_unc(S, B, E)
                    quad_Z2 += Z*Z

            quad_total = math.sqrt(quad_Z2)

            # ----------------------------------
            # Run2 evaluation
            # ----------------------------------
            run2_Z2 = 0.0

            for i in range(len(best_edges)-1):

                lo = best_edges[i]
                hi = best_edges[i+1]

                mask = (bin_lo >= lo) & (bin_lo < hi)

                S = S_run2[mask].sum()
                B = B_run2[mask].sum()
                F = F_run2[mask].sum()
                E = E_run2[mask].sum()
                
                if use_fake_corr:
                    F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                            flavour=flav, era="Run2")

                if S > 0 and B > 0:
                    Z = compute_bin_Z_with_unc(S, B, E)
                    run2_Z2 += Z * Z
                    #run2_Z2 += 2*((S+B)*math.log(1+S/B) - S)

            run2_total = math.sqrt(run2_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            
            tqdm.write(f"[SUMMARY] nBins = {n_bins} QUAD={quad_total:.4f} | Run2={run2_total:.4f}")

            result = {
                "flav": flav,
                "mass": mass,
                "nbins": len(best_edges) - 1,
                "quad": quad_total,
                "run2": run2_total,
                "ratio": ratio,
                "binning": {
                    "Run2": edges_run2,
                    "per_era": edges_per_era
                }
            }

            
            if DEBUG:
                print("\n[DEBUG BINNING]")
                print(f"{flav} mass={mass}")
                
                if result["binning"]["Run2"] is not None:
                    print("Run2:", result["binning"]["Run2"])
                    
                for era, edges in result["binning"]["per_era"].items():
                    print(f"{era}: {edges}")

            pbar.update(1)
            results[flav].append(result)
            
    pbar.close()

    return results

