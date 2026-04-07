#!/usr/bin/env python3

import ROOT, os, math, itertools, argparse, sys, datetime
from multiprocessing import Pool, cpu_count
from tqdm import tqdm

ROOT.gROOT.SetBatch(True)

RED   = "\033[91m"
GREEN = "\033[92m"
RESET = "\033[0m"

STAT_THRESHOLD_PERERA = 0.5
STAT_THRESHOLD_RUN2  = 1.0

# =========================================================
# CONFIG
# =========================================================
ERAS = ["2016preVFP","2016postVFP","2017","2018"]
FLAVOURS = ["MuMu","EE","EMu"]

USE_FAKE_FIX = True
FAKE_FLOOR = 0.15 * 0.645

# =========================================================
# LOGGER
# =========================================================
class TeeLogger:
    def __init__(self,f):
        self.file=open(f,"w")
        self.stdout=sys.stdout
    def write(self,m):
        self.stdout.write(m)
        self.file.write(m)
    def flush(self):
        self.stdout.flush()
        self.file.flush()

def fmt(edges):
    return "[" + ", ".join(f"{x:.2f}" for x in edges) + "]"

def check_bins(edges, bins, threshold):
    results = []
    for i in range(len(edges)-1):
        lo, hi = edges[i], edges[i+1]
        sub = [b for b in bins if lo <= b[0] < hi]

        val = sum(x[2] for x in sub)
        ok = val > threshold

        results.append((round(val,2), ok))
    return results


def print_bkg_with_stat(edges_low, edges_high, low_per_era, high_per_era, mode):

    print("\n   --- Background + Stat Check ---")

    if mode == "perera":

        threshold = STAT_THRESHOLD_PERERA

        for era in ERAS:

            print(f"   {era}")

            res_low  = check_bins(edges_low,  low_per_era[era],  threshold)
            res_high = check_bins(edges_high, high_per_era[era], threshold)

            low_str  = [f"{GREEN}{v}{RESET}" if ok else f"{RED}{v}{RESET}" for v,ok in res_low]
            high_str = [f"{GREEN}{v}{RESET}" if ok else f"{RED}{v}{RESET}" for v,ok in res_high]

            print("     LOW  =", "[" + ", ".join(low_str) + "]")
            print("     HIGH =", "[" + ", ".join(high_str) + "]")

    else:
        # RUN2 COMBINED CHECK
        threshold = STAT_THRESHOLD_RUN2

        comb_low  = []
        comb_high = []

        for era in ERAS:
            comb_low  += low_per_era[era]
            comb_high += high_per_era[era]

        res_low  = check_bins(edges_low,  comb_low,  threshold)
        res_high = check_bins(edges_high, comb_high, threshold)

        low_str  = [f"{GREEN}{v}{RESET}" if ok else f"{RED}{v}{RESET}" for v,ok in res_low]
        high_str = [f"{GREEN}{v}{RESET}" if ok else f"{RED}{v}{RESET}" for v,ok in res_high]

        print("   COMBINED")
        print("     LOW  =", "[" + ", ".join(low_str) + "]")
        print("     HIGH =", "[" + ", ".join(high_str) + "]")



def evaluate_fixed_bins(edges_low, edges_high, low, high, cache,
                        fake_low_per_era, fake_high_per_era):

    total = 0

    for m, c in cache.items():

        f = []

        for era in ERAS:

            # LOW
            for i in range(len(edges_low)-1):
                lo,hi = edges_low[i], edges_low[i+1]
                sub=[b for b in low[era] if lo<=b[0]<hi]

                bkg = correct_bkg(lo, hi, sub, fake_low_per_era[era])
                sig=sum(v for x,v in c.items() if lo<=x<hi)

                f.append(fom(sig,bkg))

            # HIGH
            for i in range(len(edges_high)-1):
                lo,hi = edges_high[i], edges_high[i+1]
                sub=[b for b in high[era] if lo<=b[0]<hi]

                bkg = correct_bkg(lo, hi, sub, fake_high_per_era[era])
                sig=sum(v for x,v in c.items() if lo<=x<hi)

                f.append(fom(sig,bkg))

        total += sum(x*x for x in f)

    return math.sqrt(total)
        
def print_bkg_summary(edges_low, edges_high, low_per_era, high_per_era):

    print("\n   --- Background per era ---")

    for era in ERAS:

        bkg_low  = get_bkg_per_bin(edges_low,  low_per_era[era])
        bkg_high = get_bkg_per_bin(edges_high, high_per_era[era])

        print(f"   {era}")
        print(f"     LOW  = {bkg_low}")
        print(f"     HIGH = {bkg_high}")
        
# =========================================================
# FOM
# =========================================================
def fom(s,b):
    if b>0 and s>0:
        return math.sqrt(2*((s+b)*math.log(1+s/b)-s))
    return 0

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
# HIST
# =========================================================
def get_bins(h):
    out=[]
    for i in range(1,h.GetNbinsX()+1):
        lo=h.GetBinLowEdge(i)
        hi=h.GetBinLowEdge(i+1)
        val=h.GetBinContent(i)
        err=h.GetBinError(i)
        rel=err/val if val>0 else 999
        out.append((lo,hi,val,rel))
    return out

def get_latest_dir(base):
    dirs=[os.path.join(base,d) for d in os.listdir(base)
          if os.path.isdir(os.path.join(base,d))]
    return max(dirs,key=os.path.getmtime)


def load_fake(base, flav):

    sel = get_latest_dir(base)
    path = f"PassSR2/HNL_ULIDv2/{flav}/MainPlots"

    low = {}
    high = {}

    for era in ERAS:
        f = ROOT.TFile(os.path.join(sel, era, "HNL_SignalRegion_Plotter_Fake.root"))

        hL = f.Get(path + "/HT_PT1_LowDPhi")
        hH = f.Get(path + "/HT_PT1_HighDphi")

        hL = hL.Clone()
        hH = hH.Clone()

        hL.SetDirectory(0)
        hH.SetDirectory(0)

        low[era] = get_bins(hL)
        high[era] = get_bins(hH)

    return low, high

def load_signals(base, flav):

    sel = get_latest_dir(base)
    path = f"PassSR2/HNL_ULIDv2/{flav}/MainPlots"

    sig_low = {}
    sig_high = {}

    ALL_MASSES = []
    for m in OPT_MASSES + EVAL_MASSES:
        if m not in ALL_MASSES:
            ALL_MASSES.append(m)

    for m in ALL_MASSES:

        # ----------------------------------
        # DEFINE SIGNAL SAMPLE NAME
        # ----------------------------------
        if m == "0":
            sig_sample = "Weinberg"
        elif m.startswith("SSWW_"):
            sig_sample = "HNL_" + m.split("_")[1]
        else:
            sig_sample = "HNL_" + m

        sig_low_all = []
        sig_high_all = []

        # ----------------------------------
        # LOOP OVER ERAS
        # ----------------------------------
        for era in ERAS:

            fname = os.path.join(
                sel, era,
                f"HNL_SignalRegion_Plotter_{sig_sample}.root"
            )

            f = ROOT.TFile(fname)

            print("[DEBUG SIGNAL FILE]", era, fname,
                  "OK" if f and not f.IsZombie() else "MISSING")

            if not f or f.IsZombie():
                raise RuntimeError("[ERROR] Missing file: " + fname)

            hname_low  = path + "/HT_PT1_LowDPhi"
            hname_high = path + "/HT_PT1_HighDphi"

            hL_tmp = f.Get(hname_low)
            hH_tmp = f.Get(hname_high)

            print("[DEBUG SIGNAL]", era, hname_low,
                  "OK" if hL_tmp else "MISSING")
            print("[DEBUG SIGNAL]", era, hname_high,
                  "OK" if hH_tmp else "MISSING")

            if not hL_tmp:
                raise RuntimeError("[ERROR] Missing LOW hist in " + fname)
            if not hH_tmp:
                raise RuntimeError("[ERROR] Missing HIGH hist in " + fname)

            # =========================================================
            # CRITICAL: clone while file is open
            # =========================================================
            hL = hL_tmp.Clone()
            hH = hH_tmp.Clone()

            hL.SetDirectory(0)
            hH.SetDirectory(0)

            f.Close()

            sig_low_all.append(hL)
            sig_high_all.append(hH)

        # ----------------------------------
        # SAFETY CHECK
        # ----------------------------------
        if len(sig_low_all) == 0 or len(sig_high_all) == 0:
            print("[ERROR] No histograms loaded for mass:", m)
            continue

        # ----------------------------------
        # COMBINE AFTER ERA LOOP
        # ----------------------------------
        hL = sig_low_all[0].Clone()
        hH = sig_high_all[0].Clone()

        if not hL or not hH:
            print("[ERROR] Clone failed for mass:", m)
            continue

        for h in sig_low_all[1:]:
            hL.Add(h)
        for h in sig_high_all[1:]:
            hH.Add(h)

        # ----------------------------------
        # FINAL CHECK
        # ----------------------------------
        if not hasattr(hL, "GetNbinsX"):
            print("[ERROR] Invalid ROOT object after combine for mass:", m)
            continue

        # ----------------------------------
        # NORMALISE
        # ----------------------------------
        if hL.Integral() > 0:
            hL.Scale(1.0 / hL.Integral())
        if hH.Integral() > 0:
            hH.Scale(1.0 / hH.Integral())

        sig_low[m] = hL
        sig_high[m] = hH

        print(f"[INFO] Loaded signal: {m} -> {sig_sample}")

    print("\n[DEBUG] Final loaded masses:", list(sig_low.keys()))

    return sig_low, sig_high


def build_sig_cache(sig):

    cache = {}

    for m in sig:

        if sig[m] is None:
            print("[ERROR] None histogram for mass:", m)
            continue

        if not hasattr(sig[m], "GetNbinsX"):
            print("[ERROR] Invalid ROOT object for mass:", m)
            continue

        cache[m] = {
            sig[m].GetBinLowEdge(i): sig[m].GetBinContent(i)
            for i in range(1, sig[m].GetNbinsX()+1)
        }

    return cache


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

# =========================================================
# WORKER
# =========================================================
def worker(args):
    edges, bins, sig_cache, mode, all_low, fake_bins = args

    if mode == "perera":
        if not check_perera(edges, bins):
            return None
        
    elif mode == "run2":
        if not check_run2_per_flavour(edges, all_low):
            return None

    elif mode == "singleera":
        # only check the single era present
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

        for era in eras_to_use:
            for i in range(len(edges)-1):
                lo,hi=edges[i],edges[i+1]
                sub=[b for b in bins[era] if lo<=b[0]<hi]

                bkg = correct_bkg(lo, hi, sub, fake_bins[era])
                sig = sum(v for x,v in cache.items() if lo <= x < hi)
                
                f.append(fom(sig, bkg))

        total+=sum(x*x for x in f)

    return math.sqrt(total),edges

# =========================================================
# SCAN
# =========================================================
def build_edges(bins,n):
    edges=sorted(set([b[0] for b in bins]+[b[1] for b in bins]))[1:-1]
    combos=list(itertools.combinations(edges,n-1))
    return [[bins[0][0]]+list(c)+[bins[-1][1]] for c in combos]


def combine_flavours(all_low, all_high):

    combined_low = {}
    combined_high = {}

    for era in ERAS:

        combined_low[era] = []
        combined_high[era] = []

        n_bins = len(all_low[0][era])

        for i in range(n_bins):

            lo = all_low[0][era][i][0]
            hi = all_low[0][era][i][1]

            sum_low = sum(d[era][i][2] for d in all_low)
            sum_high = sum(d[era][i][2] for d in all_high)

            combined_low[era].append((lo, hi, sum_low, 0))
            combined_high[era].append((lo, hi, sum_high, 0))

    return combined_low, combined_high

def run_scan(name, bins, n, mode, sig_cache, fake_bins, all_low=None):

    base = list(bins.values())[0]
    edges=build_edges(base,n)

    print(f"[SCAN] {name} n{n} ({mode}) combos={len(edges)}")

    best=(-1,None)

    with Pool(cpu_count()) as p:
        args = [(e, bins, sig_cache, mode, all_low, fake_bins) for e in edges]

        for r in tqdm(p.imap(worker,args),total=len(edges)):
            if r and r[0]>best[0]:
                best=r

    return best

# =========================================================
# PREDEFINED SCAN
# =========================================================

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

                    f_run = math.sqrt(r_low[0]**2 + r_high[0]**2)

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

def evaluate_per_era(edges_low, edges_high, low, high, cache,fake_low_per_era, fake_high_per_era):

    results = {}

    for era in ERAS:

        total = 0

        for m, c in cache.items():

            f = []

            # LOW
            for i in range(len(edges_low)-1):
                lo,hi = edges_low[i], edges_low[i+1]
                sub=[b for b in low[era] if lo<=b[0]<hi]

                bkg = correct_bkg(lo, hi, sub, fake_low_per_era[era])
                sig=sum(v for x,v in c.items() if lo<=x<hi)

                f.append(fom(sig,bkg))

            # HIGH
            for i in range(len(edges_high)-1):
                lo,hi = edges_high[i], edges_high[i+1]
                sub=[b for b in high[era] if lo<=b[0]<hi]

                bkg = correct_bkg(lo, hi, sub, fake_high_per_era[era])
                sig=sum(v for x,v in c.items() if lo<=x<hi)

                f.append(fom(sig,bkg))

            total += sum(x*x for x in f)

        results[era] = math.sqrt(total)

    return results

# =========================================================
# LOAD
# =========================================================
def load_flavour(base,flav):

    sel=get_latest_dir(base)
    path=f"PassSR2/HNL_ULIDv2/{flav}/MainPlots"

    low={}
    high={}

    for era in ERAS:
        f=ROOT.TFile(os.path.join(sel,era,"HNL_SignalRegion_Plotter_Bkg.root"))

        hL=f.Get(path+"/HT_PT1_LowDPhi")
        hH=f.Get(path+"/HT_PT1_HighDphi")

        hL = hL.Clone()
        hH = hH.Clone()

        hL.SetDirectory(0)
        hH.SetDirectory(0)
        
        low[era]=get_bins(hL)
        high[era]=get_bins(hH)

    return low,high


def evaluate_variable_binning(edges_low_per_era, edges_high_per_era,
                             low, high, cache,
                             fake_low_per_era, fake_high_per_era):

    total = 0

    for m, c in cache.items():

        f = []

        for era in ERAS:

            edges_low  = edges_low_per_era[era]
            edges_high = edges_high_per_era[era]

            # LOW
            for i in range(len(edges_low)-1):
                lo, hi = edges_low[i], edges_low[i+1]
                sub = [b for b in low[era] if lo <= b[0] < hi]

                bkg = correct_bkg(lo, hi, sub, fake_low_per_era[era])
                sig = sum(v for x,v in c.items() if lo <= x < hi)

                f.append(fom(sig, bkg))

            # HIGH
            for i in range(len(edges_high)-1):
                lo, hi = edges_high[i], edges_high[i+1]
                sub = [b for b in high[era] if lo <= b[0] < hi]

                bkg = correct_bkg(lo, hi, sub, fake_high_per_era[era])
                sig = sum(v for x,v in c.items() if lo <= x < hi)

                f.append(fom(sig, bkg))

        total += sum(x*x for x in f)

    return math.sqrt(total)


def format_edges(edges):
    return ",".join("{:.2f}".format(x) for x in edges[1:-1])


def make_plot_v2(results, flav,
                 best_scan_run,
                 best_combined_run,
                 best_pre_run,        # <-- ADD THIS
                 OPT_MASSES,
                 EVAL_MASSES):

    c = ROOT.TCanvas("c","",800,700)

    mg = ROOT.TMultiGraph()

    def make_graph(data, style, marker):
        g = ROOT.TGraph()
        for i,(x,y) in enumerate(data):
            g.SetPoint(i, x, y)

        g.SetLineStyle(style)
        g.SetMarkerStyle(marker)
        g.SetLineWidth(2)
        g.SetMarkerSize(1.2)

        return g

    # -----------------------------
    # GRAPHS
    # -----------------------------
    g_scan = make_graph(results["scan_run"], 1, 20)
    g_comb = make_graph(results["combined_run"], 2, 24)
    g_per = make_graph(results["scan_per"], 2, 21)
    g_era = make_graph(results["era_run"], 4, 25)
    
    mg.Add(g_scan)
    mg.Add(g_comb)
    mg.Add(g_per)
    mg.Add(g_era)
    
    mg.Draw("APL")

    # ----------------------------------
    # AUTO Y-RANGE (ALL GRAPHS)
    # ----------------------------------
    ymin = 1e9
    ymax = -1e9
    
    for key in ["scan_run", "combined_run", "scan_per", "era_run"]:
        for x, y in results.get(key, []):
            if y < ymin: ymin = y
            if y > ymax: ymax = y
            
    # protect against empty
    if ymin < ymax:
        ymin *= 0.8
        ymax *= 1.5
        
        mg.SetMinimum(ymin)
        mg.SetMaximum(ymax)
    
    xmin = mg.GetXaxis().GetXmin()
    xmax = mg.GetXaxis().GetXmax()
    mg.GetXaxis().SetTitle("N bins")
    mg.GetYaxis().SetTitle("Best FOM")
    mg.SetTitle("Best FOM vs N bins")

    # -----------------------------
    # LEGEND LABELS WITH BIN INFO
    # -----------------------------
    scan_low  = format_edges(best_scan_run[1])
    scan_high = format_edges(best_scan_run[2])

    comb_low  = format_edges(best_combined_run[1])
    comb_high = format_edges(best_combined_run[2])
    
    label_scan = "Scan (flavour, Run2) ({}) | ({})".format(scan_low, scan_high)
    label_comb = "Combined (all flav) ({}) | ({})".format(comb_low, comb_high)
    
    
    # -----------------------------
    # LEGEND
    # -----------------------------
    leg = ROOT.TLegend(0.15,0.55,0.55,0.80)
    leg.AddEntry(g_scan, label_scan, "lp")
    leg.AddEntry(g_comb, label_comb, "lp")
    leg.AddEntry(g_per, "Scan (flavour, per-era)", "lp")
    leg.AddEntry(g_era, "Scan (era-dependent)", "lp")


    # --- horizontal band (thin box)
    band = ROOT.TBox(xmin, best_pre_run*0.98, xmax, best_pre_run*1.02)
    band.SetFillColorAlpha(ROOT.kGray, 0.3)
    band.SetLineColor(ROOT.kGray)
    band.Draw("SAME")
    
    # --- central line
    line_pre = ROOT.TLine(xmin, best_pre_run, xmax, best_pre_run)
    line_pre.SetLineStyle(2)
    line_pre.SetLineWidth(2)
    line_pre.Draw("SAME")
    
    # --- marker at best N (optional)
    best_n = max(
        results["scan_run"] +
        results["combined_run"] +
        results["scan_per"] +
        results["era_run"],
        key=lambda x: x[1]
    )[0]
    
    g_pre = ROOT.TGraph(1)
    g_pre.SetPoint(0, best_n, best_pre_run)
    g_pre.SetMarkerStyle(29)
    g_pre.SetMarkerSize(2)
    g_pre.Draw("P SAME")
    
    leg.AddEntry(line_pre, "Predefined (Run2 grid)", "l")
    
    leg.Draw()

    # -----------------------------
    # TEXT BLOCK (like old code)
    # -----------------------------
    def format_mass(m):
        if m == "0":
            return "Weinberg"
        if m.startswith("SSWW_"):
            return m.split("_")[1]
        return str(m)

    opt_str  = ", ".join(format_mass(m) for m in OPT_MASSES)
    eval_str = ", ".join(format_mass(m) for m in EVAL_MASSES)

    txt = ROOT.TLatex()
    txt.SetNDC()
    txt.SetTextSize(0.03)

    txt.DrawLatex(0.6,0.83, "Flavour: {}".format(flav))
    txt.DrawLatex(0.6,0.78, "Opt: {}".format(opt_str))
    txt.DrawLatex(0.6,0.73, "Eval: {}".format(eval_str))

    c.SaveAs("plots/fom_vs_nbins_{}.pdf".format(flav))


# =========================================================
# MAIN
# =========================================================
def main():

    global OPT_MASSES,EVAL_MASSES

    parser=argparse.ArgumentParser()
    parser.add_argument('--opt_weinberg',action='store_true')
    parser.add_argument('--opt_ssww',action='store_true')
    parser.add_argument('--weinberg',action='store_true')
    args=parser.parse_args()

    if args.opt_weinberg:
        OPT_MASSES=["0"]
    elif args.opt_ssww:
        OPT_MASSES=["SSWW_600","SSWW_1000"]
    else:
        OPT_MASSES=["0","SSWW_1000"]

    EVAL_MASSES=OPT_MASSES

    base="/data6/Users/jalmond/HNL/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter"

    ts=datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    os.makedirs("logs",exist_ok=True)
    sys.stdout=TeeLogger(f"logs/scan_{ts}.txt")

    # =========================================================
    # LOAD ALL FLAVOURS FIRST
    # =========================================================
    all_low = []
    all_high = []
    all_sig_low = []
    all_sig_high = []
    all_fake_low = []
    all_fake_high = []

    for flav in FLAVOURS:
        
        print("\n[LOAD]", flav)

        low, high = load_flavour(base, flav)
        sig_low, sig_high = load_signals(base, flav)
        fake_low, fake_high = load_fake(base, flav)
        all_fake_low.append(fake_low)
        all_fake_high.append(fake_high)
        all_low.append(low)
        all_high.append(high)
        all_sig_low.append(sig_low)
        all_sig_high.append(sig_high)

        
        
    # =========================================================
    # BUILD COMBINED (CORRECT LOCATION)
    # =========================================================
    combined_low, combined_high = combine_flavours(all_low, all_high)
    combined_fake_low, combined_fake_high = combine_flavours(all_fake_low, all_fake_high)
    
    combined_sig_low = {}
    combined_sig_high = {}

    for m in OPT_MASSES:

        hL = None
        hH = None

        for sig_low in all_sig_low:
            if m not in sig_low: continue
            if hL is None:
                hL = sig_low[m].Clone()
            else:
                hL.Add(sig_low[m])

        for sig_high in all_sig_high:
            if m not in sig_high: continue
            if hH is None:
                hH = sig_high[m].Clone()
            else:
                hH.Add(sig_high[m])

        if hL:
            hL.Scale(1.0 / hL.Integral())
        if hH:
            hH.Scale(1.0 / hH.Integral())

        combined_sig_low[m] = hL
        combined_sig_high[m] = hH

    combined_cache_low  = build_sig_cache(combined_sig_low)
    combined_cache_high = build_sig_cache(combined_sig_high)

        
    # =========================================================
    # GLOBAL COMBINED SCAN (ONCE)
    # =========================================================
    best_combined_per = (-1, None, None)
    best_combined_run = (-1, None, None)

    combined_results_per_n = {}
    
    for n in [3,4,5,6]:

        nL = n // 2
        nH = (n + 1) // 2

        low_per  = run_scan("LOW",  combined_low,  nL, "perera",
                    combined_cache_low, combined_fake_low)
        
        high_per = run_scan("HIGH", combined_high, nH, "perera",
                            combined_cache_high, combined_fake_high)
        f_per = math.sqrt(low_per[0]**2 + high_per[0]**2)

        if f_per > best_combined_per[0]:
            best_combined_per = (f_per, low_per[1], high_per[1])
            
        low_run  = run_scan("LOW",  combined_low,  nL, "run2",
                    combined_cache_low, combined_fake_low, all_low)

        high_run = run_scan("HIGH", combined_high, nH, "run2",
                            combined_cache_high, combined_fake_high, all_low)

        f_run = math.sqrt(low_run[0]**2 + high_run[0]**2)

        if f_run > best_combined_run[0]:
            best_combined_run = (f_run, low_run[1], high_run[1])

        combined_results_per_n[n] = (
            low_run[1],
            high_run[1],
            f_run
        )
        
    # =========================================================
    # LOOP OVER FLAVOURS (NOW SAFE)
    # =========================================================
    for i, flav in enumerate(FLAVOURS):

        results = {
            "scan_per": [],
	    "scan_run": [],
            "combined_run": [],
            "predefined_run": [],   # NEW
            "predefined_per": [],   # NEW
            "fixed": [],
            "era_run": [] 
        }
        
        
        print("\n=== FLAVOUR:", flav, "===")

        fake_low_per_era = all_fake_low[i]
        fake_high_per_era = all_fake_high[i]

        low  = all_low[i]
        high = all_high[i]

        sig_low  = all_sig_low[i]
        sig_high = all_sig_high[i]

        cache_low  = build_sig_cache(sig_low)
        cache_high = build_sig_cache(sig_high)

        # ----------------------------------
        # BEST TRACKERS
        # ----------------------------------
        best_scan_per = (-1, None, None)
        best_scan_run = (-1, None, None)

        # ----------------------------------
        # SCAN LOOP (FLAVOUR ONLY)
        # ----------------------------------

        edges_per_n = {}
        era_best_per_n = {}
        # ----------------------------------
        # PREDEFINED SCAN (STORE RESULTS)
	# ----------------------------------

        best_pre_run, cfg_pre_run = scan_predefined_binnings(
            low, high,
            low, high,
            cache_low,
            "run2",
            all_low,
            fake_low_per_era,
            fake_high_per_era
        )
        
        best_pre_per, cfg_pre_per = scan_predefined_verbose(
            low, high,
            cache_low,
            "perera",
            all_low,
            fake_low_per_era,
            fake_high_per_era
        )
        # predefined evaluated on THIS flavour                                                                                    
        f_pre_run = evaluate_fixed_bins(
            cfg_pre_run[0],
            cfg_pre_run[1],
            low,
            high,
            cache_low,
            fake_low_per_era,
            fake_high_per_era
        )
        
        f_pre_per = evaluate_fixed_bins(
            cfg_pre_per[0],
            cfg_pre_per[1],
            low,
            high,
            cache_low,
            fake_low_per_era,
            fake_high_per_era

        )
        
        for n in [3, 4, 5, 6]:

            results["predefined_run"].append((n, f_pre_run))
            results["predefined_per"].append((n, f_pre_per))

            nL = n // 2
            nH = (n + 1) // 2
        
            # ---- PER ERA
            low_per  = run_scan("LOW",  low,  nL, "perera",
                                cache_low, fake_low_per_era)
            
            high_per = run_scan("HIGH", high, nH, "perera",
                                cache_high, fake_high_per_era)
        
            f_per = math.sqrt(low_per[0]**2 + high_per[0]**2)
            
            if f_per > best_scan_per[0]:
                best_scan_per = (f_per, low_per[1], high_per[1])
                
            # ---- RUN2
            low_run  = run_scan("LOW",  low,  nL, "run2",
                                cache_low, fake_low_per_era, all_low)

            high_run = run_scan("HIGH", high, nH, "run2",
                                cache_high, fake_high_per_era, all_low)
            
            f_run = math.sqrt(low_run[0]**2 + high_run[0]**2)

            if f_run > best_scan_run[0]:
                best_scan_run = (f_run, low_run[1], high_run[1])
                
            results["scan_per"].append((n, f_per))
            results["scan_run"].append((n, f_run))
            edges_low_c, edges_high_c, _ = combined_results_per_n[n]
            
            f_this = evaluate_fixed_bins(
                edges_low_c,
                edges_high_c,
                combined_low,
                combined_high,
                combined_cache_low,
                combined_fake_low,
                combined_fake_high
            )

            results["combined_run"].append((n, f_this))
            era_edges_low  = {}
            era_edges_high = {}

            
            for era in ERAS:
                
                # isolate ONE era
                low_e  = {era: low[era]}
                high_e = {era: high[era]}

                # run scan ONLY on this era
                low_best  = run_scan("LOW",  low_e,  nL, "singleera", cache_low, {era: fake_low_per_era[era]})
                high_best = run_scan("HIGH", high_e, nH, "singleera", cache_high, {era: fake_high_per_era[era]})
                
                era_edges_low[era]  = low_best[1]
                era_edges_high[era] = high_best[1]

            f_era = evaluate_variable_binning(
                era_edges_low,
                era_edges_high,
                low,
                high,
                cache_low,
                fake_low_per_era,
                fake_high_per_era
            )

            results["era_run"].append((n, f_era))
            edges_per_n[n] = {
                "run2": (low_run[1], high_run[1]),
                "per":  (low_per[1], high_per[1]),
                "comb": (edges_low_c, edges_high_c),
                "era":  (era_edges_low, era_edges_high)
            }
            

        
        # ----------------------------------
        # FINAL PRINT
        # ----------------------------------
        print("\n====================================")
        print(f" FINAL BEST RESULT ({flav})")
        print("====================================")
        
        # =========================
        # SCAN PER-ERA
        # =========================
        print("\n-- Scan PER-ERA --")
        print("FOM  =", round(best_scan_per[0], 3))
        print("LOW  =", fmt(best_scan_per[1]))
        print("HIGH =", fmt(best_scan_per[2]))
        
        print_bkg_with_stat(best_scan_per[1], best_scan_per[2], low, high,"perera")

        
        # =========================
        # SCAN RUN2
        # =========================
        print("\n-- Scan RUN2 --")
        print("FOM  =", round(best_scan_run[0], 3))
        print("LOW  =", fmt(best_scan_run[1]))
        print("HIGH =", fmt(best_scan_run[2]))
        
        print_bkg_with_stat(best_scan_run[1], best_scan_run[2], low, high,"run2")
        
        # --- COMBINED RESULT
        edges_low_c, edges_high_c = best_combined_run[1], best_combined_run[2]
        
        f_this = evaluate_fixed_bins(
            edges_low_c,
            edges_high_c,
            low,
            high,
            cache_low,
            fake_low_per_era,
            fake_high_per_era
        )
        
        per_era_foms = evaluate_per_era(
            edges_low_c,
            edges_high_c,
            low,
            high,
            cache_low,
            fake_low_per_era,
            fake_high_per_era
        )
        
        print("\n-- Combined-optimised RUN2 --")
        print(f"{flav} FOM = {round(f_this,3)} "
              f"(combined = {round(best_combined_run[0],3)})")
        
        print("LOW  =", fmt(edges_low_c))
        print("HIGH =", fmt(edges_high_c))
        
        print("\n   --- Per-era FOM ---")
        for era in ERAS:
            print(f"   {era} = {round(per_era_foms[era],3)}")
            
        print_bkg_with_stat(edges_low_c, edges_high_c, low, high,"run2")
        
        # =========================
        # PREDEFINED PER-ERA
        # =========================
        print("\n-- Predefined PER-ERA --")
        print("FOM  =", round(best_pre_per, 3))
        
        edges_low_pre, edges_high_pre = cfg_pre_per
        
        print("LOW  =", fmt(edges_low_pre))
        print("HIGH =", fmt(edges_high_pre))
        
        print_bkg_with_stat(edges_low_pre, edges_high_pre, low, high,"perera")
        
        # =========================
        # PREDEFINED RUN2
        # =========================
        print("\n-- Predefined RUN2 --")
        print("FOM  =", round(best_pre_run, 3))
        
        edges_low_pre_run, edges_high_pre_run = cfg_pre_run
        
        print("LOW  =", fmt(edges_low_pre_run))
        print("HIGH =", fmt(edges_high_pre_run))
        
        print_bkg_with_stat(edges_low_pre_run, edges_high_pre_run, low, high,"run2")
        

        os.makedirs("plots", exist_ok=True)

        print("\n====================================")
        print(f" SUMMARY TABLE ({flav})")
        print("====================================")

        print("{:<5} {:<10} {:<10} {:<12} {:<10} {:<10} {:<10}".format(
            "n", "Run2", "PerEra", "Combined", "EraDep", "PreRun2", "PrePer"))

        all_strategies = []

        for i, n in enumerate([3,4,5,6]):

            all_strategies.append(("Run2", n, results["scan_run"][i][1]))
            all_strategies.append(("PerEra", n, results["scan_per"][i][1]))
            all_strategies.append(("Combined", n, results["combined_run"][i][1]))
            all_strategies.append(("EraDep", n, results["era_run"][i][1]))
            all_strategies.append(("PreRun2", n, results["predefined_run"][i][1]))
            all_strategies.append(("PrePer", n, results["predefined_per"][i][1]))
            
    
            run2 = results["scan_run"][i][1]
            per  = results["scan_per"][i][1]
            comb = results["combined_run"][i][1]
            era  = results["era_run"][i][1]
            pre_run = results["predefined_run"][i][1]
            pre_per = results["predefined_per"][i][1]

            print("{:<5} {:<10.3f} {:<10.3f} {:<12.3f} {:<10.3f} {:<10.3f} {:<10.3f}".format(
                n, run2, per, comb, era, pre_run, pre_per))

            
            
        print("\n====================================")
        print(f" BKG PER ERA (BIN-BY-BIN, n = 6, {flav})")
        print("====================================")
        
        cfg = edges_per_n[6]

        def get_bkg_bins(edges_low, edges_high, low, high, era,fake_low_per_era, fake_high_per_era):

            vals = []
            
            # LOW bins
            for i in range(len(edges_low)-1):
                lo, hi = edges_low[i], edges_low[i+1]
                sub = [b for b in low[era] if lo <= b[0] < hi]
                vals.append(round(correct_bkg(lo, hi, sub, fake_low_per_era[era]), 2))

            # HIGH bins
            for i in range(len(edges_high)-1):
                lo, hi = edges_high[i], edges_high[i+1]
                sub = [b for b in high[era] if lo <= b[0] < hi]
                vals.append(round(correct_bkg(lo, hi, sub, fake_high_per_era[era]), 2))

            return vals

        
        for era in ERAS:

            b_run = get_bkg_bins(cfg["run2"][0], cfg["run2"][1], low, high, era,
                                 fake_low_per_era, fake_high_per_era)
            b_per = get_bkg_bins(cfg["per"][0],  cfg["per"][1],  low, high, era, fake_low_per_era, fake_high_per_era)
            b_com = get_bkg_bins(
                cfg["comb"][0], cfg["comb"][1],
                low, high, era,
                fake_low_per_era, fake_high_per_era
            )
            
            b_era = get_bkg_bins(
                cfg["era"][0][era],
                cfg["era"][1][era],
                low,
                high,
                era, fake_low_per_era, fake_high_per_era
            )
            b_pre = get_bkg_bins(
                cfg_pre_run[0],
                cfg_pre_run[1],
                low,
                high,
                era, fake_low_per_era, fake_high_per_era
            )
            
            print(f"\n{era}")
            print("  Run2    :", b_run)
            print("  PerEra  :", b_per)
            print("  Combined:", b_com)
            print("  EraDep  :", b_era)
            print("  Predef  :", b_pre)
        
        print("\n====================================")
        print(f" BKG PER ERA (n = 6, {flav})")
        print("====================================")
        
        print("{:<12} {:<10} {:<10} {:<12} {:<10} {:<10}".format(
        "Era", "Run2", "PerEra", "Combined", "EraDep", "Predef"))        
        cfg = edges_per_n[6]

        def get_total_bkg(edges_low, edges_high, era, fake_low_per_era, fake_high_per_era):
            total = 0
            
            # LOW
            for i in range(len(edges_low)-1):
                lo, hi = edges_low[i], edges_low[i+1]
                sub = [b for b in low[era] if lo <= b[0] < hi]
                total += correct_bkg(lo, hi, sub, fake_low_per_era[era])
                
            # HIGH
            for i in range(len(edges_high)-1):
                lo, hi = edges_high[i], edges_high[i+1]
                sub = [b for b in high[era] if lo <= b[0] < hi]
                total += correct_bkg(lo, hi, sub, fake_high_per_era[era])

            return total


        for era in ERAS:

            # Run2
            b_run = get_total_bkg(cfg["run2"][0], cfg["run2"][1], era,fake_low_per_era, fake_high_per_era)

            # Per-era (common binning)
            b_per = get_total_bkg(cfg["per"][0], cfg["per"][1], era,fake_low_per_era, fake_high_per_era)
            
            # Combined flavour bins
            b_com = get_total_bkg(cfg["comb"][0], cfg["comb"][1], era,fake_low_per_era, fake_high_per_era)
            
            # Era-dependent
            b_era = get_total_bkg(
                cfg["era"][0][era],
                cfg["era"][1][era],
                era,fake_low_per_era, fake_high_per_era
            )
            b_pre = get_total_bkg(cfg_pre_run[0], cfg_pre_run[1], era,fake_low_per_era, fake_high_per_era)
            
            print("{:<12} {:<10.2f} {:<10.2f} {:<12.2f} {:<10.2f} {:<10.2f}".format(
                era, b_run, b_per, b_com, b_era, b_pre))

        best_global = max(all_strategies, key=lambda x: x[2])

        print("\n====================================")
        print(" BEST BINNING (ALL STRATEGIES)")
        print("====================================")
        
        print("Strategy =", best_global[0])
        print("nBins    =", best_global[1])
        print("FOM      =", round(best_global[2],3))
        
        make_plot_v2(results, flav,
                     best_scan_run,
                     best_combined_run,
                     best_pre_run, 
                     OPT_MASSES,
                     EVAL_MASSES)
        
if __name__=="__main__":
    main()
