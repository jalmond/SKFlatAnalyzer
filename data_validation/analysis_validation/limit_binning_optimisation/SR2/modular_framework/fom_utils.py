from config import STAT_THRESHOLD_PERERA,STAT_THRESHOLD_RUN2,ERAS,FLAVOURS,USE_TRUE_RUN2_FOM,USE_FAKE_FIX,FAKE_FLOOR,OPT_MASSES,EVAL_MASSES

def run_combined_scan(data, n, masses):

    # ---------------------------------------------------------
    # Split bins LOW / HIGH
    # ---------------------------------------------------------
    nL = n // 2
    nH = (n + 1) // 2

    # ---------------------------------------------------------
    # Build combined background + fake (sum over flavours)
    # ---------------------------------------------------------
    combined_low  = {}
    combined_high = {}
    combined_fake_low  = {}
    combined_fake_high = {}

    for era in ERAS:

        combined_low[era] = []
        combined_high[era] = []
        combined_fake_low[era] = []
        combined_fake_high[era] = []

        n_bins = len(data["background"]["low"][FLAVOURS[0]][era])

        for i in range(n_bins):

            lo = data["edges"][i]
            hi = data["edges"][i+1]

            b_low  = sum(data["background"]["low"][flav][era][i]  for flav in FLAVOURS)
            b_high = sum(data["background"]["high"][flav][era][i] for flav in FLAVOURS)

            f_low  = sum(data["fake"]["low"][flav][era][i]  for flav in FLAVOURS)
            f_high = sum(data["fake"]["high"][flav][era][i] for flav in FLAVOURS)

            combined_low[era].append((lo, hi, b_low, 0))
            combined_high[era].append((lo, hi, b_high, 0))

            combined_fake_low[era].append((lo, hi, f_low, 0))
            combined_fake_high[era].append((lo, hi, f_high, 0))

    # ---------------------------------------------------------
    # Build signal cache (global signal)
    # ---------------------------------------------------------
    def build_cache(arr):
        return {
            data["edges"][i]: arr[i]
            for i in range(len(arr))
        }

    combined_cache = {
        m: build_cache(data["signal_global"][m])
        for m in masses
    }

    # ---------------------------------------------------------
    # Needed for Run2 stat check
    # ---------------------------------------------------------
    all_low = []
    for flav in FLAVOURS:
        tmp = {}
        for era in ERAS:
            tmp[era] = [
                (data["edges"][i], data["edges"][i+1],
                 data["background"]["low"][flav][era][i], 0)
                for i in range(len(data["edges"])-1)
            ]
        all_low.append(tmp)

    # ---------------------------------------------------------
    # Run scan
    # ---------------------------------------------------------
    low_run  = run_scan("LOW",  combined_low,  nL, "run2",
                        combined_cache, combined_fake_low, all_low)

    high_run = run_scan("HIGH", combined_high, nH, "run2",
                        combined_cache, combined_fake_high, all_low)

    return low_run[1], high_run[1]

def evaluate_combined_global_scan_run2(
    data,
    n_bins,
    masses,
    use_fake_corr=True,
    run_z_no_unc=True
):

    print("\n==============================")
    print(" GLOBAL COMBINED BINNING (RUN2)")
    print("==============================\n")

    results = {flav: [] for flav in FLAVOURS}

    bin_lo = data["edges"][:-1]

    # ----------------------------------
    # Get best binning
    # ----------------------------------
    edges_low, edges_high = run_combined_scan(data, n_bins, masses)

    print("[GLOBAL BINNING]")
    print("LOW :", edges_low)
    print("HIGH:", edges_high)

    # ----------------------------------
    # Loop over flavour + mass
    # ----------------------------------
    for flav in FLAVOURS:

        for mass in masses:

            bin_stats_per_era = {era: [] for era in ERAS}
            bin_stats_run2 = []

            quad_Z2 = 0.0

            # =========================
            # PER ERA (QUAD)
            # =========================
            for era in ERAS:

                # LOW
                for i in range(len(edges_low)-1):

                    lo, hi = edges_low[i], edges_low[i+1]

                    mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = data["signal"]["low"][flav][mass][era][mask].sum()
                    B = data["background"]["low"][flav][era][mask].sum()
                    F = data["fake"]["low"][flav][era][mask].sum()
                    E = data["bkg_err2"]["low"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    bin_stats_per_era[era].append({"S": S, "B": B, "E": E})

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E,
                                                   run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

                # HIGH
                for i in range(len(edges_high)-1):

                    lo, hi = edges_high[i], edges_high[i+1]

                    mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = data["signal"]["high"][flav][mass][era][mask].sum()
                    B = data["background"]["high"][flav][era][mask].sum()
                    F = data["fake"]["high"][flav][era][mask].sum()
                    E = data["bkg_err2"]["high"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    bin_stats_per_era[era].append({"S": S, "B": B, "E": E})

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E,
                                                   run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

            quad_total = math.sqrt(quad_Z2)

            # =========================
            # RUN2
            # =========================
            run2_Z2 = 0.0

            for i in range(len(edges_low)-1):

                lo, hi = edges_low[i], edges_low[i+1]

                mask = (bin_lo >= lo) & (bin_lo < hi)

                S_tot = B_tot = E_tot = 0.0

                for era in ERAS:

                    S = data["signal"]["low"][flav][mass][era][mask].sum()
                    B = data["background"]["low"][flav][era][mask].sum()
                    F = data["fake"]["low"][flav][era][mask].sum()
                    E = data["bkg_err2"]["low"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    S_tot += S
                    B_tot += B
                    E_tot += E

                bin_stats_run2.append({"S": S_tot, "B": B_tot, "E": E_tot})

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot,
                                               run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            # HIGH (Run2)
            for i in range(len(edges_high)-1):

                lo, hi = edges_high[i], edges_high[i+1]

                mask = (bin_lo >= lo) & (bin_lo < hi)

                S_tot = B_tot = E_tot = 0.0

                for era in ERAS:

                    S = data["signal"]["high"][flav][mass][era][mask].sum()
                    B = data["background"]["high"][flav][era][mask].sum()
                    F = data["fake"]["high"][flav][era][mask].sum()
                    E = data["bkg_err2"]["high"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    S_tot += S
                    B_tot += B
                    E_tot += E

                bin_stats_run2.append({"S": S_tot, "B": B_tot, "E": E_tot})

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot,
                                               run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)
            ratio = run2_total / quad_total if quad_total > 0 else 0.0

            results[flav].append({
                "flav": flav,
                "mass": mass,
                "nbins": n_bins,
                "quad": quad_total,
                "run2": run2_total,
                "ratio": ratio,
                "binning": {
                    "low": edges_low,
                    "high": edges_high
                },
                "bin_stats": {
                    "Run2": bin_stats_run2,
                    "per_era": bin_stats_per_era
                }
            })

    return results


# =========================================================                                             \
                                                                                                         
# SCAN                                                                                                  \
                                                                                                         
# =========================================================                                             \
                                                                                                         
def build_edges(bins,n):
    edges=sorted(set([b[0] for b in bins]+[b[1] for b in bins]))[1:-1]
    combos=list(itertools.combinations(edges,n-1))
    return [[bins[0][0]]+list(c)+[bins[-1][1]] for c in combos]


def run_scan(name, bins, n, mode, sig_cache, fake_bins, all_low=None):

    base = list(bins.values())[0]
    edges=build_edges(base,n)

    print(f"[SCAN] {name} n{n} ({mode}) combos={len(edges)}")

    best=(-1,None)

    with Pool(cpu_count()) as p:
        args = [(e, bins, sig_cache, mode, all_low, fake_bins) for e in edges]
        valid = 0
        for r in tqdm(p.imap(worker,args),total=len(edges)):
            if r:
                valid += 1
                if r and r[0]>best[0]:
                    best=r
        print(f"[DEBUG] valid combos = {valid}")
    return best


def worker(args):
    edges, bins, sig_cache, mode, all_low, fake_bins = args

    if mode == "perera":
        if not check_perera(edges, bins):
            return None

    elif mode == "run2":
        if not check_run2_per_flavour(edges, all_low):
            return None

    elif mode == "singleera":
        # only check the single era present                                                                                                                \
                                                                                                                                                            
        era = list(bins.keys())[0]

        for i in range(len(edges)-1):
            lo, hi = edges[i], edges[i+1]
            sub = [b for b in bins[era] if lo <= b[0] < hi]

            if sum(x[2] for x in sub) <= STAT_THRESHOLD_PERERA:
                return None

    total=0

    for m,cache in sig_cache.items():

        f=[]
        eras_to_use = bins.keys()

        if USE_TRUE_RUN2_FOM:

            # --- merge background over eras ---                                                                                                           \
                                                                                                                                                            
            for i in range(len(edges)-1):
                lo, hi = edges[i], edges[i+1]

                bkg_sum = 0
                for era in eras_to_use:
                    sub = [b for b in bins[era] if lo <= b[0] < hi]
                    bkg_sum += correct_bkg(lo, hi, sub, fake_bins[era])

                sig = sum(v for x,v in cache.items() if lo <= x < hi)

                f.append(fom(sig, bkg_sum))

        else:


            for era in eras_to_use:
                for i in range(len(edges)-1):
                    lo, hi = edges[i], edges[i+1]
                    sub = [b for b in bins[era] if lo <= b[0] < hi]

                    bkg = correct_bkg(lo, hi, sub, fake_bins[era])
                    sig = sum(v for x, v in cache.items() if lo <= x < hi)

                    f.append(fom(sig, bkg))
        total+=sum(x*x for x in f)

    return math.sqrt(total),edges


def evaluate_per_flavour_scan_run2(
    data,
    n_bins,
    masses,
    use_fake_corr=True,
    run_z_no_unc=True
):

    print("\n==============================")
    print(" PER-FLAVOUR BINNING (RUN2)")
    print("==============================\n")

    results = {}

    bin_lo = data["edges"][:-1]

    for flav in FLAVOURS:

        print(f"\n[FLAVOUR] {flav}")

        # ---------------------------------------------------------
        # Split bins
        # ---------------------------------------------------------
        nL = n_bins // 2
        nH = (n_bins + 1) // 2

        # ---------------------------------------------------------
        # Build per-flavour bins (same format as run_scan expects)
        # ---------------------------------------------------------
        bins_low  = {}
        bins_high = {}
        fake_low  = {}
        fake_high = {}

        for era in ERAS:

            bins_low[era]  = []
            bins_high[era] = []
            fake_low[era]  = []
            fake_high[era] = []

            n_b = len(data["background"]["low"][flav][era])

            for i in range(n_b):

                lo = data["edges"][i]
                hi = data["edges"][i+1]

                bL = data["background"]["low"][flav][era][i]
                bH = data["background"]["high"][flav][era][i]

                fL = data["fake"]["low"][flav][era][i]
                fH = data["fake"]["high"][flav][era][i]

                bins_low[era].append((lo, hi, bL, 0))
                bins_high[era].append((lo, hi, bH, 0))

                fake_low[era].append((lo, hi, fL, 0))
                fake_high[era].append((lo, hi, fH, 0))

        # ---------------------------------------------------------
        # Signal cache (per flavour, combined masses)
        # ---------------------------------------------------------
        def build_cache(arr):
            return {
                data["edges"][i]: arr[i]
                for i in range(len(arr))
            }

        sig_cache = {
            m: build_cache(data["signal_combined_mass"]["low"][flav][m])
            for m in masses
        }

        # ---------------------------------------------------------
        # Run scan (per flavour)
        # ---------------------------------------------------------
        low_run  = run_scan("LOW",  bins_low,  nL, "run2",
                            sig_cache, fake_low)

        high_run = run_scan("HIGH", bins_high, nH, "run2",
                            sig_cache, fake_high)

        edges_low  = low_run[1]
        edges_high = high_run[1]

        print(f"[BEST BINNING] {flav}")
        print(" LOW :", edges_low)
        print(" HIGH:", edges_high)

        # ---------------------------------------------------------
        # Evaluate
        # ---------------------------------------------------------
        flav_results = []

        for mass in masses:

            quad_Z2 = 0.0
            run2_Z2 = 0.0

            bin_stats_per_era = {era: [] for era in ERAS}
            bin_stats_run2 = []

            # -------------------------
            # PER ERA (QUAD)
            # -------------------------
            for era in ERAS:

                # LOW
                for i in range(len(edges_low)-1):

                    lo, hi = edges_low[i], edges_low[i+1]
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = data["signal"]["low"][flav][mass][era][mask].sum()
                    B = data["background"]["low"][flav][era][mask].sum()
                    F = data["fake"]["low"][flav][era][mask].sum()
                    E = data["bkg_err2"]["low"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    bin_stats_per_era[era].append({"S": S, "B": B, "E": E})

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E,
                                                   run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

                # HIGH
                for i in range(len(edges_high)-1):

                    lo, hi = edges_high[i], edges_high[i+1]
                    mask = (bin_lo >= lo) & (bin_lo < hi)

                    S = data["signal"]["high"][flav][mass][era][mask].sum()
                    B = data["background"]["high"][flav][era][mask].sum()
                    F = data["fake"]["high"][flav][era][mask].sum()
                    E = data["bkg_err2"]["high"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    bin_stats_per_era[era].append({"S": S, "B": B, "E": E})

                    if S > 0 and B > 0:
                        Z = compute_bin_Z_with_unc(S, B, E,
                                                   run_z_no_unc=run_z_no_unc)
                        quad_Z2 += Z * Z

            quad_total = math.sqrt(quad_Z2)

            # -------------------------
            # RUN2
            # -------------------------
            for i in range(len(edges_low)-1):

                lo, hi = edges_low[i], edges_low[i+1]
                mask = (bin_lo >= lo) & (bin_lo < hi)

                S_tot = B_tot = E_tot = 0.0

                for era in ERAS:

                    S = data["signal"]["low"][flav][mass][era][mask].sum()
                    B = data["background"]["low"][flav][era][mask].sum()
                    F = data["fake"]["low"][flav][era][mask].sum()
                    E = data["bkg_err2"]["low"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    S_tot += S
                    B_tot += B
                    E_tot += E

                bin_stats_run2.append({"S": S_tot, "B": B_tot, "E": E_tot})

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot,
                                               run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            for i in range(len(edges_high)-1):

                lo, hi = edges_high[i], edges_high[i+1]
                mask = (bin_lo >= lo) & (bin_lo < hi)

                S_tot = B_tot = E_tot = 0.0

                for era in ERAS:

                    S = data["signal"]["high"][flav][mass][era][mask].sum()
                    B = data["background"]["high"][flav][era][mask].sum()
                    F = data["fake"]["high"][flav][era][mask].sum()
                    E = data["bkg_err2"]["high"][flav][era][mask].sum()

                    if use_fake_corr:
                        F, B = fix_fake_and_bkg(F, B, FAKE_FLOOR,
                                                flavour=flav, era=era)

                    S_tot += S
                    B_tot += B
                    E_tot += E

                bin_stats_run2.append({"S": S_tot, "B": B_tot, "E": E_tot})

                if S_tot > 0 and B_tot > 0:
                    Z = compute_bin_Z_with_unc(S_tot, B_tot, E_tot,
                                               run_z_no_unc=run_z_no_unc)
                    run2_Z2 += Z * Z

            run2_total = math.sqrt(run2_Z2)

            flav_results.append({
                "mass": mass,
                "quad": quad_total,
                "run2": run2_total,
                "binning": {
                    "low": edges_low,
                    "high": edges_high
                },
                "bin_stats": {
                    "Run2": bin_stats_run2,
                    "per_era": bin_stats_per_era
                }
            })

        results[flav] = flav_results

    return results
