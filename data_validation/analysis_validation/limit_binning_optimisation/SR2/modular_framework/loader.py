import ROOT


def get_latest_dir(base):
    dirs=[os.path.join(base,d) for d in os.listdir(base)
          if os.path.isdir(os.path.join(base,d))]
    return max(dirs,key=os.path.getmtime)

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


