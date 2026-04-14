
# =========================================================                                                                                                                                                                          
# PREDEFINED SCAN                                                                                                                                                                                                                    
# =========================================================

def evaluate_per_era(edges_low, edges_high, low, high, cache,
                     fake_low_per_era, fake_high_per_era):

    results = {}

    # =========================================================                                                                                                                                                                                                                       
    # TRUE RUN2 MODE: FOM(s_Run2, sum b_era)                                                                                                                                                                                                                                          
    # =========================================================                                                                                                                                                                                                                       
    if USE_TRUE_RUN2_FOM:

        total = 0

        for m, c in cache.items():

            f_bins = []

            # -------------------------                                                                                                                                                                                                                                               
            # LOW bins                                                                                                                                                                                                                                                                
            # -------------------------                                                                                                                                                                                                                                               
            for i in range(len(edges_low)-1):
                lo = edges_low[i]
                hi = edges_low[i+1]

                bkg_sum = 0
                for era2 in ERAS:
                    sub = [b for b in low[era2] if lo <= b[0] < hi]
                    bkg_sum += correct_bkg(lo, hi, sub, fake_low_per_era[era2])

                sig = sum(v for x, v in c.items() if lo <= x < hi)
                f_bins.append(fom(sig, bkg_sum))

            # -------------------------                                                                                                                                                                                                                                               
            # HIGH bins                                                                                                                                                                                                                                                               
            # -------------------------                                                                                                                                                                                                                                               
            for i in range(len(edges_high)-1):
                lo = edges_high[i]
                hi = edges_high[i+1]

                bkg_sum = 0
                for era2 in ERAS:
                    sub = [b for b in high[era2] if lo <= b[0] < hi]
                    bkg_sum += correct_bkg(lo, hi, sub, fake_high_per_era[era2])

                sig = sum(v for x, v in c.items() if lo <= x < hi)
                f_bins.append(fom(sig, bkg_sum))

            total += sum(x*x for x in f_bins)

        f_total = math.sqrt(total)

        # In Run2 mode, per-era breakdown is not meaningful                                                                                                                                                                                                                           
        # Assign same value for compatibility with existing code                                                                                                                                                                                                                      
        for era in ERAS:
            results[era] = f_total

        return results

    # =========================================================                                                                                                                                                                                                                       
    # ORIGINAL MODE: per-era FOM                                                                                                                                                                                                                                                      
    # =========================================================                                                                                                                                                                                                                       
    for era in ERAS:

        total = 0

        for m, c in cache.items():

            f_bins = []

            # -------------------------                                                                                                                                                                                                                                               
            # LOW bins                                                                                                                                                                                                                                                                
            # -------------------------                                                                                                                                                                                                                                               
            for i in range(len(edges_low)-1):
                lo = edges_low[i]
                hi = edges_low[i+1]

                sub = [b for b in low[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_low_per_era[era])

                sig = sum(v for x, v in c.items() if lo <= x < hi)
                f_bins.append(fom(sig, bkg))

            # -------------------------                                                                                                                                                                                                                                               
            # HIGH bins                                                                                                                                                                                                                                                               
            # -------------------------                                                                                                                                                                                                                                               
            for i in range(len(edges_high)-1):
                lo = edges_high[i]
                hi = edges_high[i+1]

                sub = [b for b in high[era] if lo <= b[0] < hi]
                bkg = correct_bkg(lo, hi, sub, fake_high_per_era[era])

                sig = sum(v for x, v in c.items() if lo <= x < hi)
                f_bins.append(fom(sig, bkg))

            total += sum(x*x for x in f_bins)

        results[era] = math.sqrt(total)

    return results


def evaluate_fixed_bins(edges_low, edges_high, low, high, cache,
                        fake_low_per_era, fake_high_per_era):

    total = 0

    for m, c in cache.items():

        f_bins = []

        # =========================================================                                                                                                                                        
        # TRUE RUN2 MODE                                                                                                                                                                                   
        # =========================================================                                                                                                                                        
        if USE_TRUE_RUN2_FOM:

            # -------------------------                                                                                                                                                                    
            # LOW bins                                                                                                                                                                                     
            # -------------------------                                                                                                                                                                    
            for i in range(len(edges_low)-1):
                lo = edges_low[i]
                hi = edges_low[i+1]

                bkg_sum = 0
                for era in ERAS:
                    sub = [b for b in low[era] if lo <= b[0] < hi]
                    bkg_sum += correct_bkg(lo, hi, sub, fake_low_per_era[era])

                sig = sum(v for x, v in c.items() if lo <= x < hi)
                f_bins.append(fom(sig, bkg_sum))

            # -------------------------                                                                                                                                                                    
            # HIGH bins                                                                                                                                                                                    
            # -------------------------                                                                                                                                                                    
            for i in range(len(edges_high)-1):
                lo = edges_high[i]
                hi = edges_high[i+1]

                bkg_sum = 0
                for era in ERAS:
                    sub = [b for b in high[era] if lo <= b[0] < hi]
                    bkg_sum += correct_bkg(lo, hi, sub, fake_high_per_era[era])

                sig = sum(v for x, v in c.items() if lo <= x < hi)
                f_bins.append(fom(sig, bkg_sum))

        # =========================================================                                                                                                                                        
        # ORIGINAL PER-ERA MODE                                                                                                                                                                            
        # =========================================================                                                                                                                                        
        else:

            for era in ERAS:

                # LOW                                                                                                                                                                                      
                for i in range(len(edges_low)-1):
                    lo = edges_low[i]
                    hi = edges_low[i+1]

                    sub = [b for b in low[era] if lo <= b[0] < hi]
                    bkg = correct_bkg(lo, hi, sub, fake_low_per_era[era])

                    sig = sum(v for x, v in c.items() if lo <= x < hi)
                    f_bins.append(fom(sig, bkg))

                # HIGH                                                                                                                                                                                     
                for i in range(len(edges_high)-1):
                    lo = edges_high[i]
                    hi = edges_high[i+1]

                    sub = [b for b in high[era] if lo <= b[0] < hi]
                    bkg = correct_bkg(lo, hi, sub, fake_high_per_era[era])

                    sig = sum(v for x, v in c.items() if lo <= x < hi)
                    f_bins.append(fom(sig, bkg))

        total += sum(x*x for x in f_bins)

    return math.sqrt(total)




def scan_predefined_verbose(bins_low, bins_high,
                           sig_cache, mode,
                            all_low, fake_low_per_era, fake_high_per_era):

    print("\n==============================")
    print("[REFERENCE] Predefined bin scan")
    print("==============================")

    boundaries = [1.0,1.5,2.0,2.5,3.0,3.5,4.0,5.0,7.5]

    results = []

    for b1 in boundaries:
        for b2 in boundaries:
            if b1 >= b2: continue

            for b3 in boundaries:
                for b4 in boundaries:
                    if b3 >= b4: continue

                    low  = [0, b1, b2, 10]
                    high = [0, b3, b4, 10]

                    r_low  = worker((low, bins_low, sig_cache, mode, all_low, fake_low_per_era))
                    r_high = worker((high, bins_high, sig_cache, mode, all_low, fake_high_per_era))

                    if not r_low or not r_high:
                        continue

                    per_era_tmp = evaluate_per_era(
                        low, high,
                        bins_low, bins_high,
                        sig_cache,
                        fake_low_per_era,
                        fake_high_per_era
                    )

                    f_run = combine_per_era_fom(per_era_tmp)

                    # per-era FOM                                                                                                                                                                                                    
                    f_per = evaluate_fixed_bins(
                        low, high,
                        bins_low, bins_high,
                        sig_cache,
                        fake_low_per_era,
                        fake_high_per_era
                    )

                    # --- STAT CHECKS ---                                                                                                                                                                                            
                    pass_run2 = check_run2_per_flavour(low, all_low) and \
                        check_run2_per_flavour(high, all_low)

                    pass_per  = check_perera(low, bins_low) and \
                        check_perera(high, bins_high)

                    # --- BKG ---                                                                                                                                                                                                    
                    def get_bkg(edges, bins):
                        vals = []
                        for i in range(len(edges)-1):
                            lo, hi = edges[i], edges[i+1]
                            sub = [b for b in bins if lo <= b[0] < hi]
                            vals.append(round(sum(x[2] for x in sub),2))
                        return vals

                    bkg_low  = get_bkg(low,  list(bins_low.values())[0])
                    bkg_high = get_bkg(high, list(bins_high.values())[0])

                    results.append({
                        "low": low,
                        "high": high,
                        "f_run": f_run,
                        "f_per": f_per,
                        "pass_run2": pass_run2,
                        "pass_per": pass_per,
                        "bkg_low": bkg_low,
                        "bkg_high": bkg_high
                    })

    # -------------------------                                                                                                                                                                                                      
    # SORT by Run2 FOM                                                                                                                                                                                                               
    # -------------------------                                                                                                                                                                                                      
    results.sort(key=lambda x: x["f_run"], reverse=True)

    # -------------------------                                                                                                                                                                                                      
    # PRINT                                                                                                                                                                                                                          
    # -------------------------                                                                                                                                                                                                      
    for r in results:

        status = ""
        if r["pass_run2"]:
            status = " ----> PASS STAT REQ"
        elif r["pass_per"]:
            status = " ----> PASS PER-ERA ONLY"

        print(
            "REFERENCE RESULT Predefined | "
            f"Low {r['low']} | High {r['high']} | "
            f"BkgLow {r['bkg_low']} | BkgHigh {r['bkg_high']} | "
            f"FOM_EraCombined {round(r['f_per'],2)} | "
            f"FOM_Run2 {round(r['f_run'],2)}"
            + status
        )

    best = results[0]
    return best["f_run"], (best["low"], best["high"])

def scan_predefined_binnings(bins_low, bins_high,
                            bins_low_per_era, bins_high_per_era,
                             sig_cache, mode, all_low, fake_low_per_era, fake_high_per_era):

    print(f"\n[REFERENCE] Predefined ({mode})")

    boundaries = [1.0,1.5,2.0,2.5,3.0,3.5,4.0,5.0,7.5]

    best = (-1, None)

    for b1 in boundaries:
        for b2 in boundaries:
            if b1 >= b2:
                continue

            for b3 in boundaries:
                for b4 in boundaries:
                    if b3 >= b4:
                        continue

                    # independent LOW and HIGH binning                                                                                                                                                                               
                    low  = [0, b1, b2, 10]
                    high = [0, b3, b4, 10]

                    r_low  = worker((low,  bins_low,  sig_cache, mode, all_low, fake_low_per_era))
                    r_high = worker((high, bins_high, sig_cache, mode, all_low, fake_high_per_era))

                    if not r_low or not r_high:
                        continue

                    # combine LOW + HIGH FOM                                                                                                                                                                                         
                    f = math.sqrt(r_low[0]**2 + r_high[0]**2)

                    if f > best[0]:
                        best = (f, (low, high))

    return best

