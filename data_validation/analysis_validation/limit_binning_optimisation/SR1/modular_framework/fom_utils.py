# =========================================================
# EVALUATORs
# =========================================================

from default_config import ERAS, FLAVOURS, FAKE_FLOOR, MIN_SIGNAL_FRAC
from helper import fix_fake_and_bkg,compute_bin_Z

import numpy as np
import math

from tqdm import tqdm


DEBUG = False

def pass_stat(B):
    return (B >= 1.0)

def pass_stat_and_err(B, rel):
    return (B >= 1.0) or (B >= 0.5 and rel < 0.3)

#### compute_bin_Z in helper.py

def compute_bin_Z_with_unc(S, B, E,run_z_no_unc=True):

    if run_z_no_unc:
        return compute_bin_Z(S,B)
    
    if S <= 0 or B <= 0:
        return 0.0

    sigma2 = E

    if sigma2 <= 0:
        return compute_bin_Z(S, B)

    term1 = (S + B) * math.log((S + B)*(B + sigma2) / (B*B + (S + B)*sigma2))
    term2 = (B*B / sigma2) * math.log(1 + sigma2*S / (B*(B + sigma2)))

    Z2 = 2 * (term1 - term2)

    return math.sqrt(Z2) if Z2 > 0 else 0.0

#=============================================
### Scan functions rundp_*
#=============================================


def run_dp_on_arrays_with_flav_stat(
        bin_lo,
        n_bins=6,
        use_fake_corr=True,
        S_flav_era=None,
        B_flav_era=None,
        E_flav_era=None,
        F_flav_era=None,
        run_z_no_unc=True,
        
):

    nbins = len(bin_lo)

    dp = np.full((nbins + 1, n_bins + 1), -np.inf)
    prev = [[-1]*(n_bins+1) for _ in range(nbins+1)]

    dp[0][0] = 0.0

    for i in range(nbins):
        for k in range(n_bins):

            if dp[i][k] == -np.inf:
                continue

            for j in range(i+1, nbins+1):

                mask = slice(i, j)

                valid = True

                # =========================================
                # STAT CHECK (PER FLAVOUR, CORRECTED FAKE)
                # =========================================
                for f in FLAVOURS:

                    B_tot = 0.0
                    E_tot = 0.0

                    for era in ERAS:

                        B_arr = B_flav_era[f][era]
                        F_arr = F_flav_era[f][era]
                        E_arr = E_flav_era[f][era]

                        b_e = B_arr[mask].sum()
                        f_e = F_arr[mask].sum()
                        e_e = E_arr[mask].sum()

                        if use_fake_corr:
                            f_e, b_e = fix_fake_and_bkg(
                                f_e, b_e, FAKE_FLOOR,
                                flavour=f, era=era
                            )

                        B_tot += b_e
                        E_tot += e_e

                    if B_tot < 1e-6:
                        valid = False
                        break

                    rel = math.sqrt(E_tot) / B_tot

                    if not pass_stat_and_err(B_tot, rel):
                        valid = False
                        break

                if not valid:
                    continue

                # =========================================
                # Z CALCULATION (PER FLAVOUR, SUMMED)
                # =========================================
                Z_bin = 0.0

                for f in FLAVOURS:

                    S_f = 0.0
                    B_f = 0.0
                    E_f = 0.0

                    for era in ERAS:

                        S_arr = S_flav_era[f][era]
                        B_arr = B_flav_era[f][era]
                        F_arr = F_flav_era[f][era]
                        E_arr = E_flav_era[f][era]

                        s_e = S_arr[mask].sum()
                        b_e = B_arr[mask].sum()
                        f_e = F_arr[mask].sum()
                        e_e = E_arr[mask].sum()

                        if use_fake_corr:
                            f_e, b_e = fix_fake_and_bkg(
                                f_e, b_e, FAKE_FLOOR,
                                flavour=f, era=era
                            )

                        S_f += s_e
                        B_f += b_e
                        E_f += e_e

                    if S_f > 0 and B_f > 0:
                        Z_f = compute_bin_Z_with_unc(S_f, B_f, E_f,run_z_no_unc=run_z_no_unc)
                        Z_bin += Z_f*Z_f

                if dp[i][k] + Z_bin > dp[j][k+1]:
                    dp[j][k+1] = dp[i][k] + Z_bin
                    prev[j][k+1] = i

    # =========================
    # FIND BEST SOLUTION
    # =========================
    best_score = -np.inf
    best_k = -1

    for k in range(1, n_bins+1):
        if dp[nbins][k] > best_score:
            best_score = dp[nbins][k]
            best_k = k

    if best_k == -1:
        return None

    # =========================
    # BACKTRACK
    # =========================
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

    # =========================
    # BUILD EDGES
    # =========================
    bin_hi = np.append(bin_lo[1:], bin_lo[-1] + (bin_lo[1] - bin_lo[0]))

    edges = [bin_lo[0]]

    for idx in edges_idx[1:-1]:
        edges.append(bin_lo[idx])

    edges.append(bin_hi[-1])

    return edges


def run_dp_on_arrays(
        S, B, E, F,
        bin_lo,
        n_bins=6,
        use_fake_corr=True,
        flav=None,
        use_low_edge_cut=True,
        run_z_no_unc=True,
):

    N = len(bin_lo)

    # ----------------------------------
    # Build bin_hi
    # ----------------------------------
    bin_hi = np.append(bin_lo[1:], bin_lo[-1] + (bin_lo[1] - bin_lo[0]))

    # ----------------------------------
    # LOW-EDGE SIGNAL FRACTION CUT
    # ----------------------------------
    S_total = sum(S[era].sum() for era in ERAS)

    min_edge_idx = 0
    if use_low_edge_cut:
        running = 0.0
        for i in range(N):
            running += sum(S[era][i] for era in ERAS)
            if S_total > 0 and (running / S_total) > MIN_SIGNAL_FRAC:
                min_edge_idx = i
                break

    # ----------------------------------
    # HIGH-EDGE STAT CUT (PER ERA FIX)
    # ----------------------------------
    max_edge_idx = N

    for i in reversed(range(N)):

        B_tail = 0.0
        E_tail = 0.0

        for era in ERAS:

            B_e = B[era][i:].sum()
            F_e = F[era][i:].sum()
            E_e = E[era][i:].sum()

            if use_fake_corr:
                F_e, B_e = fix_fake_and_bkg(
                    F_e, B_e, FAKE_FLOOR,
                    flavour=flav, era=era
                )

            B_tail += B_e
            E_tail += E_e

        if B_tail <= 1e-6:
            continue

        rel_tail = math.sqrt(E_tail) / B_tail

        if pass_stat_and_err(B_tail, rel_tail):
            max_edge_idx = i
            break

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

                # ----------------------------------
                # Compute interval (PER ERA FIX)
                # ----------------------------------
                S_int = 0.0
                B_int = 0.0
                E_int = 0.0

                for era in ERAS:

                    S_e = S[era][p:i].sum()
                    B_e = B[era][p:i].sum()
                    F_e = F[era][p:i].sum()
                    E_e = E[era][p:i].sum()

                    if use_fake_corr:
                        F_e, B_e = fix_fake_and_bkg(
                            F_e, B_e, FAKE_FLOOR,
                            flavour=flav, era=era
                        )

                    S_int += S_e
                    B_int += B_e
                    E_int += E_e

                if B_int <= 1e-6:
                    continue

                # ----------------------------------
                # Width cut
                # ----------------------------------
                if (bin_hi[i-1] - bin_lo[p]) < 25:
                    continue

                rel = math.sqrt(E_int) / B_int

                if not pass_stat_and_err(B_int, rel):
                    continue

                # ----------------------------------
                # LOW EDGE
                # ----------------------------------
                if j == 1 and i <= min_edge_idx:
                    continue

                # ----------------------------------
                # HIGH EDGE
                # ----------------------------------
                if j == n_bins and p >= max_edge_idx:
                    continue

                # ----------------------------------
                # DP validity
                # ----------------------------------
                if dp[j-1, p] <= NEG/2:
                    continue

                # ----------------------------------
                # Z calculation (NO DUPLICATION)
                # ----------------------------------
                Z = compute_bin_Z_with_unc(S_int, B_int, E_int,run_z_no_unc=run_z_no_unc)
                z2 = Z * Z

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


def evaluate_dp_per_flavour_per_mass_run2(data, n_bins=6, use_fake_corr=None,run_z_no_unc=True):

    print("\n==============================")
    print(" DP BINNING (PER MASS)")
    print("==============================")
    print(" - Optimisation per (flavour, mass)")
    print(" - Signal = single mass (Run2 combined via per-era)")
    print(" - Background = per-era (fake fixed per era)")
    print(" - Produces mass-dependent binning")
    print("==============================\n")

    if use_fake_corr is None:
        raise ValueError("use_fake_corr must be provided from config")
    if run_z_no_unc is None:
        raise ValueError("run_z_no_unc must be provided from config")

    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]

    total_jobs = sum(len(data["signal_combined_mass"][flav]) for flav in FLAVOURS)
    pbar = tqdm(total=total_jobs, desc="DP Scan")

    for flav in FLAVOURS:

        tqdm.write(f"[DP SCAN] Flavour = {flav}")

        for mass in data["signal_combined_mass"][flav]:

            tqdm.write(f"\n[Mass] {mass}")

            # ----------------------------------
            # Build PER-ERA arrays (for DP)
            # ----------------------------------
            S_per_era = {}
            B_per_era = {}
            E_per_era = {}
            F_per_era = {}

            for era in ERAS:
                S_per_era[era] = data["signal"][flav][mass][era]
                B_per_era[era] = data["background"][flav][era]
                E_per_era[era] = data["bkg_err2"][flav][era]
                F_per_era[era] = data["fake"][flav][era]

            # ----------------------------------
            # Run DP (PER-ERA CORRECT)
            # ----------------------------------
            best_edges = run_dp_on_arrays(
                S_per_era,
                B_per_era,
                E_per_era,
                F_per_era,
                bin_lo,
                n_bins=n_bins,
                use_fake_corr=use_fake_corr,
                flav=flav,
                run_z_no_unc=run_z_no_unc
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
            # QUAD (per era evaluation)
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

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E,run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

            quad_total = math.sqrt(quad_Z2)

            # ----------------------------------
            # Run2 evaluation (sum AFTER fix)
            # ----------------------------------
            run2_Z2 = 0.0

            for i in range(len(best_edges)-1):

                lo = best_edges[i]
                hi = best_edges[i+1]

                if i == len(best_edges)-2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                S_tot = 0.0
                B_tot = 0.0
                E_tot = 0.0

                for era in ERAS:

                    S = data["signal"][flav][mass][era][mask].sum()
                    B = data["background"][flav][era][mask].sum()
                    F = data["fake"][flav][era][mask].sum()
                    E = data["bkg_err2"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(
                            F, B, FAKE_FLOOR,
                            flavour=flav, era=era
                        )

                    S_tot += S
                    B_tot += B
                    E_tot += E

                bin_stats_run2.append({
                    "S": S_tot,
                    "B": B_tot,
                    "E": E_tot
                })

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot,run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            tqdm.write(
                f"[SUMMARY] nBins={n_bins} QUAD={quad_total:.4f} | Run2={run2_total:.4f}"
            )

            results[flav].append({
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
            })

            pbar.update(1)

    pbar.close()

    return results




def evaluate_dp_per_flavour_global_mass_run2(data, n_bins=6, use_fake_corr=None,run_z_no_unc=True):

    print("\n==============================")
    print(" GLOBAL DP BINNING (PER FLAVOUR)")
    print("==============================")
    print(" - Optimisation uses combined signal across all masses")
    print(" - Background = per-era (fake fixed per era)")
    print(" - One binning per flavour")
    print(" - Same binning applied to all masses")
    print("==============================\n")

    if use_fake_corr is None:
        raise ValueError("use_fake_corr must be provided from config")
    if run_z_no_unc is None:
    	raise ValueError("run_z_no_unc must be provided from config")


    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]

    for flav in FLAVOURS:

        print(f"\n[GLOBAL DP] Flavour = {flav}")

        # ==========================================
        # Build GLOBAL per-era arrays
        # ==========================================
        S_per_era = {}
        B_per_era = {}
        E_per_era = {}
        F_per_era = {}

        for era in ERAS:
            # distribute global signal shape across eras
            # (same shape for each era, normalised behaviour preserved)
            S_per_era[era] = data["signal_sum"][flav] / len(ERAS)

            B_per_era[era] = data["background"][flav][era]
            E_per_era[era] = data["bkg_err2"][flav][era]
            F_per_era[era] = data["fake"][flav][era]

        # ----------------------------------
        # Run DP (PER-ERA CORRECT)
        # ----------------------------------
        best_edges = run_dp_on_arrays(
            S_per_era,
            B_per_era,
            E_per_era,
            F_per_era,
            bin_lo,
            n_bins=n_bins,
            use_fake_corr=use_fake_corr,
            flav=flav,
            run_z_no_unc=run_z_no_unc
        )

        if best_edges is None:
            print(f"[WARNING] No binning found for {flav}")
            continue

        print(f"[GLOBAL BINNING] {flav}: {best_edges}")

        # ==================================
        # Evaluate per mass
        # ==================================
        for mass in data["signal_combined_mass"][flav]:

            print(f"\n[Mass] {mass} (using global binning)")

            edges_run2 = best_edges
            edges_per_era = {era: best_edges for era in ERAS}

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

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E,run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

            quad_total = math.sqrt(quad_Z2)

            # ----------------------------------
            # Run2 evaluation (sum AFTER per-era fix)
            # ----------------------------------
            run2_Z2 = 0.0

            for i in range(len(best_edges)-1):

                lo = best_edges[i]
                hi = best_edges[i+1]

                if i == len(best_edges)-2:
                    mask = (bin_lo >= lo) & (bin_lo <= hi)
                else:
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                S_tot = 0.0
                B_tot = 0.0
                E_tot = 0.0

                for era in ERAS:

                    S = data["signal"][flav][mass][era][mask].sum()
                    B = data["background"][flav][era][mask].sum()
                    F = data["fake"][flav][era][mask].sum()
                    E = data["bkg_err2"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(
                            F, B, FAKE_FLOOR,
                            flavour=flav, era=era
                        )

                    S_tot += S
                    B_tot += B
                    E_tot += E

                bin_stats_run2.append({
                    "S": S_tot,
                    "B": B_tot,
                    "E": E_tot
                })

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot,run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            print(f"[SUMMARY] QUAD={quad_total:.4f} | Run2={run2_total:.4f}")

            results[flav].append({
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
            })

    return results



def evaluate_dp_flavour_per_mass_combined_stat_split_run2(data, n_bins=6, use_fake_corr=None,run_z_no_unc=True):

    print("\n==============================")
    print(" GLOBAL DP BINNING (ALL FLAVOURS)")
    print("==============================")
    print(" - Optimisation uses ALL flavours combined")
    print(" - One binning per mass")
    print(" - Same binning applied to each flavour")
    print(" - Stat check = per flavour (Run2 combined)")
    print("==============================\n")

    if use_fake_corr is None:
        raise ValueError("use_fake_corr must be provided from config")
    if run_z_no_unc is None:
    	raise ValueError("run_z_no_unc must be provided from config")

        
    results = {}

    bin_lo = data["edges"][:-1]
    masses = list(data["signal_global"].keys())

    pbar = tqdm(total=len(masses), desc="DP Global Scan")

    for mass in masses:

        tqdm.write("\n[Mass] {} (combined flavours)".format(mass))

        # ==================================================
        # BUILD PER-ERA ARRAYS (NEW)
        # ==================================================
        S_flav_era = {}
        B_flav_era = {}
        E_flav_era = {}
        F_flav_era = {}

        for flav in FLAVOURS:

            S_flav_era[flav] = {}
            B_flav_era[flav] = {}
            E_flav_era[flav] = {}
            F_flav_era[flav] = {}

            for era in ERAS:

                S_flav_era[flav][era] = data["signal"][flav][mass][era]
                B_flav_era[flav][era] = data["background"][flav][era]
                E_flav_era[flav][era] = data["bkg_err2"][flav][era]
                F_flav_era[flav][era] = data["fake"][flav][era]

        # ==================================================
        # PER-FLAVOUR Run2 ARRAYS (unchanged)
        # ==================================================
        S_flav = {}
        B_flav = {}
        E_flav = {}
        F_flav = {}

        for flav in FLAVOURS:
            S_flav[flav] = sum(data["signal"][flav][mass][era] for era in ERAS)
            B_flav[flav] = sum(data["background"][flav][era] for era in ERAS)
            E_flav[flav] = sum(data["bkg_err2"][flav][era] for era in ERAS)
            F_flav[flav] = sum(data["fake"][flav][era] for era in ERAS)

        # ==================================================
        # RUN DP (FIXED)
        # ==================================================
        best_edges = run_dp_on_arrays_with_flav_stat(
            bin_lo,
            n_bins=n_bins,
            use_fake_corr=use_fake_corr,
            S_flav_era=S_flav_era,
            B_flav_era=B_flav_era,
            E_flav_era=E_flav_era,
            F_flav_era=F_flav_era,
            run_z_no_unc=run_z_no_unc
        )

        if best_edges is None:
            pbar.update(1)
            continue

        # ==================================================
        # EVALUATION PER FLAVOUR (UNCHANGED)
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
                    Z = compute_bin_Z_with_unc(S, B, E,run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            # -------------------------
            # QUAD (unchanged)
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
                        Z = compute_bin_Z_with_unc(S, B, E,run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

                bin_stats_per_era[era] = stats

            quad_total = math.sqrt(quad_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            tqdm.write("[{}] Run2={:.4f} | QUAD={:.4f}".format(
                flav, run2_total, quad_total))

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



def evaluate_dp_per_mass_per_flavour_per_era(data, n_bins=6, use_fake_corr=None,run_z_no_unc=True):

    print("\n==============================")
    print(" DP BINNING (PER MASS, PER FLAVOUR, PER ERA)")
    print("==============================")
    print(" - Optimisation per (flavour, mass, era)")
    print(" - Each era has independent binning")
    print(" - Stat check done per era")
    print(" - Run2 FOM recomputed from combined bins")
    print("==============================\n")

    if use_fake_corr is None:
        raise ValueError("use_fake_corr must be provided from config")
    if run_z_no_unc is None:
    	raise ValueError("run_z_no_unc must be provided from config")

        
    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]

    total_jobs = sum(len(data["signal_combined_mass"][flav]) for flav in FLAVOURS)
    pbar = tqdm(total=total_jobs, desc="DP Scan (Per Era)")

    for flav in FLAVOURS:

        tqdm.write(f"[DP SCAN] Flavour = {flav}")

        for mass in data["signal_combined_mass"][flav]:

            tqdm.write(f"\n[Mass] {mass}")

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

                # ----------------------------------
                # Wrap into per-era dicts (NEW)
                # ----------------------------------
                S_dict = {era: S_arr}
                B_dict = {era: B_arr}
                E_dict = {era: E_arr}
                F_dict = {era: F_arr}

                best_edges = run_dp_on_arrays(
                    S_dict,
                    B_dict,
                    E_dict,
                    F_dict,
                    bin_lo,
                    n_bins=n_bins,
                    use_fake_corr=use_fake_corr,
                    flav=flav,
                    use_low_edge_cut=False,
                    run_z_no_unc=run_z_no_unc
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
                        Z = compute_bin_Z_with_unc(S, B, E,run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

            quad_total = math.sqrt(quad_Z2)

            # ==========================================
            # RUN2 FOM (sum AFTER per-era fix)
            # ==========================================
            run2_Z2 = 0.0

            if len(best_edges_per_era) == 0:
                pbar.update(1)
                continue

            n_bins_eff = len(next(iter(best_edges_per_era.values()))) - 1

            for i in range(n_bins_eff):

                S_tot = 0.0
                B_tot = 0.0
                E_tot = 0.0

                for era in ERAS:

                    if era not in best_edges_per_era:
                        continue

                    edges = best_edges_per_era[era]

                    lo = edges[i]
                    hi = edges[i+1]

                    if i == len(edges)-2:
                        mask = (bin_lo >= lo) & (bin_lo <= hi)
                    else:
                        mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = data["signal"][flav][mass][era][mask].sum()
                    B = data["background"][flav][era][mask].sum()
                    F = data["fake"][flav][era][mask].sum()
                    E = data["bkg_err2"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(
                            F, B, FAKE_FLOOR,
                            flavour=flav,
                            era=era
                        )

                    S_tot += S
                    B_tot += B
                    E_tot += E

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot,run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            tqdm.write(
                f"[SUMMARY] QUAD={quad_total:.4f} | Run2={run2_total:.4f}"
            )

            results[flav].append({
                "flav": flav,
                "mass": mass,
                "nbins": n_bins_eff,
                "quad": quad_total,
                "run2": run2_total,
                "ratio": ratio,
                "binning": {
                    "Run2": None,
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
