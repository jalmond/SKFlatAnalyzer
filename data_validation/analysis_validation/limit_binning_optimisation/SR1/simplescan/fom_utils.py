# =========================================================
# EVALUATORs
# =========================================================

from ref_bins import REF_BINS
from config import ERAS, FLAVOURS, FAKE_FLOOR, USE_FAKE_FIX,MASSES
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



def run_dp_on_arrays_with_flav_stat(
    S, B, E, F,
    bin_lo,
    n_bins=6,
    use_fake_corr=True,
    flav="combined",
    FAKE_FLOOR=0.0,
    S_flav=None,
    B_flav=None,
    E_flav=None,
    F_flav=None
):

    import numpy as np
    import math

    nbins = len(S)

    dp = np.full((nbins + 1, n_bins + 1), -np.inf)
    prev = [[-1]*(n_bins+1) for _ in range(nbins+1)]

    dp[0][0] = 0.0

    def pass_stat(s, b, e):
        if b <= 0:
            return False
        if e > b*b:
            return False
        return True

    for i in range(nbins):

        for k in range(n_bins):

            if dp[i][k] == -np.inf:
                continue

            for j in range(i+1, nbins+1):

                mask = slice(i, j)

                valid = True

                for f in FLAVOURS:

                    s_f = S_flav[f][mask].sum()
                    b_f = B_flav[f][mask].sum()
                    f_f = F_flav[f][mask].sum()
                    e_f = E_flav[f][mask].sum()

                    if use_fake_corr:
                        f_f, b_f = fix_fake_and_bkg(
                            f_f, b_f, FAKE_FLOOR,
                            flavour=f, era="Run2"
                        )

                    # protect against zero / negative background
                    if b_f <= 0:
                        valid = False
                        break
                    
                    rel_f = math.sqrt(e_f) / b_f

                    if not pass_stat_and_err( b_f, rel_f):
                        valid = False
                        break

                if not valid:
                    continue

                s = S[mask].sum()
                b = B[mask].sum()
                f_val = F[mask].sum()
                e = E[mask].sum()

                if use_fake_corr:
                    f_val, b = fix_fake_and_bkg(
                        f_val, b, FAKE_FLOOR,
                        flavour=flav, era="Run2"
                    )

                if s <= 0 or b <= 0:
                    continue

                Z = compute_bin_Z_with_unc(s, b, e)

                if dp[i][k] + Z > dp[j][k+1]:
                    dp[j][k+1] = dp[i][k] + Z
                    prev[j][k+1] = i

    best_score = -np.inf
    best_k = -1

    for k in range(1, n_bins+1):
        if dp[nbins][k] > best_score:
            best_score = dp[nbins][k]
            best_k = k

    if best_k == -1:
        return None

    edges_idx = []
    j = nbins
    k = best_k

    while k > 0:
        i = prev[j][k]
        edges_idx.append(j)
        j = i
        k -= 1

    edges_idx.append(0)
    edges_idx = sorted(edges_idx)

    # Build bin_hi same as standard DP
    bin_hi = np.append(bin_lo[1:], bin_lo[-1] + (bin_lo[1] - bin_lo[0]))
    
    edges = [bin_lo[0]]
    
    for idx in edges_idx[1:-1]:
        edges.append(bin_lo[idx])
        
    # Force last edge to match histogram upper bound
    edges.append(bin_hi[-1])
    #edges = [bin_lo[0]]
    #for idx in edges_idx[1:]:
    #    if idx < len(bin_lo):
    #        edges.append(bin_lo[idx])
    #    else:
    #        edges.append(bin_lo[-1])

    return edges

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
    # FIX: build HIGH edges properly
    # ----------------------------------
    bin_hi = np.append(bin_lo[1:], bin_lo[-1] + (bin_lo[1] - bin_lo[0]))
    
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

    upper_edge = bin_hi[-1]
    max_edge_val = bin_hi[-1] if max_edge_idx == N else bin_lo[max_edge_idx]

    print(
        f"[SCAN WINDOW] "
        f"[{bin_lo[0]:.1f}, {upper_edge:.1f}] --> "
        f"[{bin_lo[min_edge_idx]:.1f}, {max_edge_val:.1f}]"
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
                if (bin_hi[i-1] - bin_lo[p]) < 25:
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

    edges.append(bin_hi[-1])

    return edges



# =========================================================


def evaluate_dp_per_mass(data, n_bins=6, use_fake_corr=True):

    # =========================================================
    # DP BINNING (PER MASS, PER FLAVOUR)
    # =========================================================
    #
    # Strategy:
    #   - Perform binning optimisation separately for each (flavour, mass)
    #   - Each mass gets its own optimal binning
    #
    # Signal definition:
    #   - S_run2 = signal for ONE mass, summed over eras (Run2)
    #   - Signal shape changes with mass  -> drives binning differences
    #
    # Background definition:
    #   - B_run2 = background summed over all eras (Run2)
    #   - Same background used for all masses within a flavour
    #
    # Optimisation:
    #   - Run DP using (S_run2, B_run2, E_run2, F_run2)
    #   - Produces mass-dependent binning
    #
    # Evaluation:
    #   - QUAD: compute FOM per era and sum in quadrature
    #   - Run2: compute FOM using combined arrays
    #
    # Outcome:
    #   - Binning is optimised for each signal hypothesis
    #   - Maximises sensitivity per mass point
    #
    # Notes:
    #   - Background is fixed across masses; only signal changes
    #   - Fake correction applied during bin evaluation
    #   - Uncertainties stored as variance (E = sigma^2)
    #   - Final FOM uses compute_bin_Z_with_unc (includes uncertainties)
    #
    # Caveat:
    #   - Optimisation uses Z including background uncertainty (E = sum of variances)
    #     while evaluation includes uncertainties -> may shift optimal bins
    #
    # =========================================================

    print("\n==============================")
    print(" DP BINNING (PER MASS)")
    print("==============================")
    print(" - Optimisation per (flavour, mass)")
    print(" - Signal = single mass (Run2 combined)")
    print(" - Background = Run2 (same for all masses)")
    print(" - Produces mass-dependent binning")
    print("==============================\n")
    
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

            # ==================================
            # BIN STATS CONTAINERS
            # ==================================
            bin_stats_per_era = {era: [] for era in ERAS}
            bin_stats_run2 = []
            
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

                    if i == len(best_edges)-2:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)
                    
                    S = S_arr[mask].sum()
                    B = B_arr[mask].sum()
                    F = F_arr[mask].sum()
                    E = E_arr[mask].sum()
                    
                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    # Store stats (MATCH REF FORMAT)
                    bin_stats_per_era[era].append({
                        "S": S,
                        "B": B,
                        "E": E
                    })

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

                if i == len(best_edges)-2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                S = S_run2[mask].sum()
                B = B_run2[mask].sum()
                F = F_run2[mask].sum()
                E = E_run2[mask].sum()
                
                if use_fake_corr:
                    F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                            flavour=flav, era="Run2")

                
                # Store stats (MATCH REF FORMAT)
                bin_stats_run2.append({
                    "S": S,
                    "B": B,
                    "E": E
                })

                
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
                },
                "bin_stats": {
                    "Run2": bin_stats_run2,
                    "per_era": bin_stats_per_era
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


def evaluate_dp_global_per_flavour(data, n_bins=6, use_fake_corr=True):

    # =========================================================
    # GLOBAL DP BINNING (PER FLAVOUR)
    # =========================================================
    #
    # Strategy:
    #   - Perform binning optimisation once per flavour
    #   - Use Run2-combined background (all eras)
    #   - Use combined signal shape across ALL masses
    #
    # Signal definition:
    #   - S_global = sum over masses (each mass normalized)
    #   - Gives equal weight to all signal hypotheses
    #
    # Optimisation:
    #   - Run DP using (S_global, B_run2, E_run2, F_run2)
    #   - Produces ONE binning per flavour
    #
    # Evaluation:
    #   - Apply same binning to each individual mass
    #   - Compute FOM per mass (Run2 + per-era QUAD)
    #
    # Outcome:
    #   - Binning is mass-independent (stable across masses)
    #   - FOM reflects performance for each signal hypothesis
    #
    # Notes:
    #   - Background and uncertainties are summed over eras (Run2)
    #   - Fake correction applied only during bin evaluation
    #   - Errors are stored as variance (E = sigma^2)
    #
    # =========================================================

    print("\n==============================")
    print(" GLOBAL DP BINNING (PER FLAVOUR)")
    print("==============================")
    print(" - Optimisation uses combined signal across all masses")
    print(" - Background = Run2 (all eras combined)")
    print(" - One binning per flavour")
    print(" - Same binning applied to all masses")
    print("==============================\n")
    
    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]


    for flav in FLAVOURS:

        print(f"\n[GLOBAL DP] Flavour = {flav}")

        # ----------------------------------
        # Build GLOBAL Run2 arrays
        # ----------------------------------
        S_global = data["signal_sum"][flav]   # already normalized sum over masses
        B_run2 = sum(data["background"][flav][era] for era in ERAS)
        E_run2 = sum(data["bkg_err2"][flav][era] for era in ERAS)
        F_run2 = sum(data["fake"][flav][era] for era in ERAS)

        # ----------------------------------
        # Run DP ONCE (global signal)
        # ----------------------------------
        best_edges = run_dp_on_arrays(
            S_global, B_run2, E_run2, F_run2,
            bin_lo,
            n_bins=n_bins,
            use_fake_corr=use_fake_corr,
            flav=flav,
            FAKE_FLOOR=FAKE_FLOOR
        )

        if best_edges is None:
            print(f"[WARNING] No binning found for {flav}")
            continue

        print(f"[GLOBAL BINNING] {flav}: {best_edges}")

        # ==================================
        # Now evaluate per mass using SAME binning
        # ==================================
        for mass in data["signal_combined_mass"][flav]:

            print(f"\n[Mass] {mass} (using global binning)")

            # ----------------------------------
            # Build Run2 arrays per mass
            # ----------------------------------
            S_run2 = sum(data["signal"][flav][mass][era] for era in ERAS)
            B_run2 = sum(data["background"][flav][era] for era in ERAS)
            E_run2 = sum(data["bkg_err2"][flav][era] for era in ERAS)
            F_run2 = sum(data["fake"][flav][era] for era in ERAS)

            edges_run2 = best_edges
            edges_per_era = {era: best_edges for era in ERAS}

            # ==================================
            # BIN STATS
            # ==================================
            bin_stats_per_era = {era: [] for era in ERAS}
            bin_stats_run2 = []

            # ----------------------------------
            # QUAD (per era)
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

                    if i == len(best_edges)-2:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = S_arr[mask].sum()
                    B = B_arr[mask].sum()
                    F = F_arr[mask].sum()
                    E = E_arr[mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    bin_stats_per_era[era].append({
                        "S": S,
                        "B": B,
                        "E": E
                    })

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

                if i == len(best_edges)-2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                S = S_run2[mask].sum()
                B = B_run2[mask].sum()
                F = F_run2[mask].sum()
                E = E_run2[mask].sum()

                if use_fake_corr:
                    F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                            flavour=flav, era="Run2")

                bin_stats_run2.append({
                    "S": S,
                    "B": B,
                    "E": E
                })

                if S > 0 and B > 0:
                    Z = compute_bin_Z_with_unc(S, B, E)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            print(f"[SUMMARY] QUAD={quad_total:.4f} | Run2={run2_total:.4f}")

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
                },
                "bin_stats": {
                    "Run2": bin_stats_run2,
                    "per_era": bin_stats_per_era
                }
            }

            results[flav].append(result)

    return results





def evaluate_dp_combined_flavours(data, n_bins=6, use_fake_corr=True):

    print("\n==============================")
    print(" GLOBAL DP BINNING (ALL FLAVOURS)")
    print("==============================")
    print(" - Optimisation uses ALL flavours combined")
    print(" - One binning per mass")
    print(" - Same binning applied to each flavour")
    print(" - Stat check = per flavour (Run2 combined)")
    print("==============================\n")

    results = {}

    bin_lo = data["edges"][:-1]
    masses = list(data["signal_global"].keys())

    pbar = tqdm(total=len(masses), desc="DP Global Scan")

    for mass in masses:

        tqdm.write(f"\n[Mass] {mass} (combined flavours)")

        # ==================================================
        # GLOBAL ARRAYS
        # ==================================================
        S_global = sum(
            data["signal"][flav][mass][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        B_global = sum(
            data["background"][flav][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        E_global = sum(
            data["bkg_err2"][flav][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        F_global = sum(
            data["fake"][flav][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        # ==================================================
        # PER-FLAVOUR Run2 ARRAYS
        # ==================================================
        S_flav, B_flav, E_flav, F_flav = {}, {}, {}, {}

        for flav in FLAVOURS:
            S_flav[flav] = sum(data["signal"][flav][mass][era] for era in ERAS)
            B_flav[flav] = sum(data["background"][flav][era] for era in ERAS)
            E_flav[flav] = sum(data["bkg_err2"][flav][era] for era in ERAS)
            F_flav[flav] = sum(data["fake"][flav][era] for era in ERAS)

        # ==================================================
        # RUN DP
        # ==================================================
        best_edges = run_dp_on_arrays_with_flav_stat(
            S_global, B_global, E_global, F_global,
            bin_lo,
            n_bins=n_bins,
            use_fake_corr=use_fake_corr,
            flav="combined",
            FAKE_FLOOR=FAKE_FLOOR,
            S_flav=S_flav,
            B_flav=B_flav,
            E_flav=E_flav,
            F_flav=F_flav
        )

        if best_edges is None:
            pbar.update(1)
            continue

        # ==================================================
        # EVALUATION PER FLAVOUR
        # ==================================================
        for flav in FLAVOURS:

            S_run2 = S_flav[flav]
            B_run2 = B_flav[flav]
            E_run2 = E_flav[flav]
            F_run2 = F_flav[flav]

            # -------------------------
            # Run2
            # -------------------------
            run2_Z2 = 0.0
            bin_stats_run2 = []

            for i in range(len(best_edges)-1):

                lo = best_edges[i]
                hi = best_edges[i+1]

                if i == len(best_edges)-2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                S = S_run2[mask].sum()
                B = B_run2[mask].sum()
                F = F_run2[mask].sum()
                E = E_run2[mask].sum()

                if use_fake_corr:
                    F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                            flavour=flav, era="Run2")

                bin_stats_run2.append({"S": S, "B": B, "E": E})

                if S > 0 and B > 0:
                    Z = compute_bin_Z_with_unc(S, B, E)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            # -------------------------
            # QUAD
            # -------------------------
            quad_Z2 = 0.0
            bin_stats_per_era = {}

            for era in ERAS:

                S_arr = data["signal"][flav][mass][era]
                B_arr = data["background"][flav][era]
                E_arr = data["bkg_err2"][flav][era]
                F_arr = data["fake"][flav][era]

                stats = []

                for i in range(len(best_edges)-1):

                    lo = best_edges[i]
                    hi = best_edges[i+1]

                    if i == len(best_edges)-2:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = S_arr[mask].sum()
                    B = B_arr[mask].sum()
                    F = F_arr[mask].sum()
                    E = E_arr[mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    stats.append({"S": S, "B": B, "E": E})

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E)
                        quad_Z2 += Z * Z

                bin_stats_per_era[era] = stats

            quad_total = math.sqrt(quad_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            tqdm.write(f"[{flav}] Run2={run2_total:.4f} | QUAD={quad_total:.4f}")

            results.setdefault(flav, []).append({
                "flav": flav,
                "mass": mass,
                "nbins": len(best_edges) - 1,
                "run2": run2_total,
                "quad": quad_total,
                "ratio": ratio,
                "binning": {
                    "Run2": best_edges,
                    "per_era": {era: best_edges for era in ERAS}
                },
                "bin_stats": {
                    "Run2": bin_stats_run2,
                    "per_era": bin_stats_per_era
                }
            })

        pbar.update(1)

    pbar.close()

    return results

def evaluate_dp_combined_flavoursOLD(data, n_bins=6, use_fake_corr=True):

    # =========================================================
    # GLOBAL DP BINNING (ALL FLAVOURS COMBINED)
    # =========================================================
    #
    # Strategy:
    #   - Combine ALL flavours into one global S/B/E/F
    #   - Run DP once per mass
    #   - Apply SAME binning to each flavour
    #
    # Outcome:
    #   - One binning per mass (shared across flavours)
    #   - Results stored per flavour (consistent format)
    #
    # =========================================================

    print("\n==============================")
    print(" GLOBAL DP BINNING (ALL FLAVOURS)")
    print("==============================")
    print(" - Optimisation uses ALL flavours combined")
    print(" - One binning per mass")
    print(" - Same binning applied to each flavour")
    print("==============================\n")

    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]

    for mass in data["signal_global"]:

        print(f"\n[Mass] {mass} (combined flavours)")

        # ----------------------------------
        # Build GLOBAL Run2 arrays (ALL flavours)
        # ----------------------------------
        S_run2 = sum(
            data["signal"][flav][mass][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        B_run2 = sum(
            data["background"][flav][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        E_run2 = sum(
            data["bkg_err2"][flav][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        F_run2 = sum(
            data["fake"][flav][era]
            for flav in FLAVOURS
            for era in ERAS
        )

        # ----------------------------------
        # Run DP (GLOBAL)
        # ----------------------------------
        best_edges = run_dp_on_arrays(
            S_run2, B_run2, E_run2, F_run2,
            bin_lo,
            n_bins=n_bins,
            use_fake_corr=use_fake_corr,
            flav="ALL",
            FAKE_FLOOR=FAKE_FLOOR
        )

        if best_edges is None:
            print(f"[WARNING] No binning found for mass {mass}")
            continue

        print(f"[GLOBAL BINNING] {best_edges}")

        # ==================================
        # Apply SAME binning to EACH flavour
        # ==================================
        for flav in FLAVOURS:

            # ----------------------------------
            # Build Run2 arrays per flavour
            # ----------------------------------
            S_run2_f = sum(data["signal"][flav][mass][era] for era in ERAS)
            B_run2_f = sum(data["background"][flav][era] for era in ERAS)
            E_run2_f = sum(data["bkg_err2"][flav][era] for era in ERAS)
            F_run2_f = sum(data["fake"][flav][era] for era in ERAS)

            edges_run2 = best_edges
            edges_per_era = {era: best_edges for era in ERAS}

            # ==================================
            # BIN STATS
            # ==================================
            bin_stats_per_era = {era: [] for era in ERAS}
            bin_stats_run2 = []

            # ----------------------------------
            # QUAD (per era)
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

                    if i == len(best_edges)-2:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = S_arr[mask].sum()
                    B = B_arr[mask].sum()
                    F = F_arr[mask].sum()
                    E = E_arr[mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(
                            F, B, FAKE_FLOOR,
                            flavour=flav, era=era
                        )

                    bin_stats_per_era[era].append({
                        "S": S,
                        "B": B,
                        "E": E
                    })

                    Z = compute_bin_Z_with_unc(S, B, E)
                    quad_Z2 += Z * Z

            quad_total = math.sqrt(quad_Z2)

            # ----------------------------------
            # Run2 evaluation
            # ----------------------------------
            run2_Z2 = 0.0

            for i in range(len(best_edges)-1):

                lo = best_edges[i]
                hi = best_edges[i+1]

                if i == len(best_edges)-2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                S = S_run2_f[mask].sum()
                B = B_run2_f[mask].sum()
                F = F_run2_f[mask].sum()
                E = E_run2_f[mask].sum()

                if use_fake_corr:
                    F, B = fix_fake_and_bkg(
                        F, B, FAKE_FLOOR,
                        flavour=flav, era="Run2"
                    )

                bin_stats_run2.append({
                    "S": S,
                    "B": B,
                    "E": E
                })

                if S > 0 and B > 0:
                    Z = compute_bin_Z_with_unc(S, B, E)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            print(f"[SUMMARY] {flav} QUAD={quad_total:.4f} | Run2={run2_total:.4f}")

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
                },
                "bin_stats": {
                    "Run2": bin_stats_run2,
                    "per_era": bin_stats_per_era
                }
            }

            results[flav].append(result)

    return results



def evaluate_dp_per_mass_per_flavour_per_era(data, n_bins=6, use_fake_corr=True):

    print("\n==============================")
    print(" DP BINNING (PER MASS, PER FLAVOUR, PER ERA)")
    print("==============================")
    print(" - Optimisation per (flavour, mass, era)")
    print(" - Each era has independent binning")
    print(" - Stat check done per era")
    print(" - Run2 FOM recomputed from combined bins")
    print("==============================\n")

    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]

    total_jobs = sum(len(data["signal_combined_mass"][flav]) for flav in FLAVOURS)
    pbar = tqdm(total=total_jobs, desc="DP Scan (Per Era)")

    for flav in FLAVOURS:

        tqdm.write(f"[DP SCAN] Flavour = {flav}")

        for mass in data["signal_combined_mass"][flav]:

            tqdm.write(f"\n[Mass] {mass}")

            # ==========================================
            # STORE PER-ERA RESULTS
            # ==========================================
            best_edges_per_era = {}
            bin_stats_per_era = {}

            quad_Z2 = 0.0

            # ==========================================
            # LOOP ERAS (OPTIMISATION)
            # ==========================================
            for era in ERAS:

                S_arr = data["signal"][flav][mass][era]
                B_arr = data["background"][flav][era]
                E_arr = data["bkg_err2"][flav][era]
                F_arr = data["fake"][flav][era]

                best_edges = run_dp_on_arrays(
                    S_arr, B_arr, E_arr, F_arr,
                    bin_lo,
                    n_bins=n_bins,
                    use_fake_corr=use_fake_corr,
                    flav=flav,
                    FAKE_FLOOR=FAKE_FLOOR
                )

                if best_edges is None:
                    continue

                best_edges_per_era[era] = best_edges
                bin_stats_per_era[era] = []

                # ----------------------------------
                # Evaluate bins (this era)
                # ----------------------------------
                for i in range(len(best_edges)-1):

                    lo = best_edges[i]
                    hi = best_edges[i+1]

                    if i == len(best_edges)-2:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = S_arr[mask].sum()
                    B = B_arr[mask].sum()
                    F = F_arr[mask].sum()
                    E = E_arr[mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(
                            F, B, FAKE_FLOOR,
                            flavour=flav,
                            era=era
                        )

                    bin_stats_per_era[era].append({
                        "S": S,
                        "B": B,
                        "E": E
                    })

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E)
                        quad_Z2 += Z * Z

            quad_total = math.sqrt(quad_Z2)

            # ==========================================
            # RUN2 FOM (COMBINE PER BIN ACROSS ERAS)
            # ==========================================
            run2_Z2 = 0.0

            # assume same number of bins per era
            n_bins_eff = len(next(iter(best_edges_per_era.values()))) - 1

            for i in range(n_bins_eff):

                S_tot = 0.0
                B_tot = 0.0
                E_tot = 0.0

                for era in ERAS:

                    if era not in bin_stats_per_era:
                        continue

                    stats = bin_stats_per_era[era][i]

                    S_tot += stats["S"]
                    B_tot += stats["B"]
                    E_tot += stats["E"]

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            tqdm.write(
                f"[SUMMARY] QUAD={quad_total:.4f} | Run2={run2_total:.4f}"
            )

            # ==========================================
            # STORE RESULT
            # ==========================================
            results[flav].append({
                "flav": flav,
                "mass": mass,
                "nbins": n_bins_eff,
                "quad": quad_total,
                "run2": run2_total,
                "ratio": ratio,
                "binning": {
                    "Run2": None,  # not unique anymore
                    "per_era": best_edges_per_era
                },
                "bin_stats": {
                    "Run2": None,
                    "per_era": bin_stats_per_era
                }
            })

            pbar.update(1)

    pbar.close()

    return results
