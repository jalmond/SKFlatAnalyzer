import math



# =========================================================                                     
# FOM                                                                                           
# =========================================================                                     
def fom(s,b):
    if b>0 and s>0:
	return math.sqrt(2*((s+b)*math.log(1+s/b)-s))
    return 0


def combine_per_era_fom(per_era_dict):
    return math.sqrt(sum(v*v for v in per_era_dict.values()))




# =========================================================                                     
# FAKE FIX                                                                                      
# =========================================================                                     
def correct_bkg(lo, hi, sub_bins, fake_bins=None):

    if USE_FAKE_FIX and fake_bins is None:
        raise RuntimeError("Fake bins missing while fake fix enabled")

    bkg = sum(b[2] for b in sub_bins)

    if USE_FAKE_FIX and fake_bins is not None:

        sub_fake = [b for b in fake_bins if lo <= b[0] < hi]
        fake_sum = sum(b[2] for b in sub_fake)

        if fake_sum <= 0:
            bkg += (FAKE_FLOOR - fake_sum)

    return bkg



# =========================================================                                                       
# STAT CHECKS                                                                                                     
# =========================================================                                                       
def check(sub, threshold):
    return sum(x[2] for x in sub) > threshold


def check_perera(edges, bins):
    for era in ERAS:
        for i in range(len(edges)-1):
            lo,hi = edges[i],edges[i+1]
            sub = [b for b in bins[era] if lo<=b[0]<hi]
            if not check(sub, STAT_THRESHOLD_PERERA):
                return False
    return True


def check_run2(edges, bins):
    comb=[]
    for era in ERAS:
        comb += bins[era]

    for i in range(len(edges)-1):
        lo,hi = edges[i],edges[i+1]
        sub = [b for b in comb if lo<=b[0]<hi]
        if not check(sub, STAT_THRESHOLD_RUN2):
            return False
    return True


def check_run2_per_flavour(edges, all_low):

    for flav_bins in all_low:   # loop over MuMu, EE, EMu                                                         

        comb = []
        for era in ERAS:
            comb += flav_bins[era]

        for i in range(len(edges)-1):
            lo, hi = edges[i], edges[i+1]
            sub = [b for b in comb if lo <= b[0] < hi]

            if sum(x[2] for x in sub) <= STAT_THRESHOLD_RUN2:
                return False

    return True


def compute_run2_fom(edges, bins_per_era, sig_cache, fake_bins_per_era):

    total = 0

    for m, cache in sig_cache.items():

        f_bins = []

        for i in range(len(edges)-1):
            lo, hi = edges[i], edges[i+1]

            # --- SUM BACKGROUND OVER ERAS ---                                                                                                                                                                                       
            bkg_sum = 0

            for era in ERAS:
                sub = [b for b in bins_per_era[era] if lo <= b[0] < hi]
                bkg_sum += correct_bkg(lo, hi, sub, fake_bins_per_era[era])

            # --- SIGNAL (already Run2 combined) ---                                                                                                                                                                                 
            sig = sum(v for x, v in cache.items() if lo <= x < hi)

            f_bins.append(fom(sig, bkg_sum))

        total += sum(x*x for x in f_bins)

    return math.sqrt(total)
