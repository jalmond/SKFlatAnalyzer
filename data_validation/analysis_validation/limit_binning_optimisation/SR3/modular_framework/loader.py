# =========================================================
# LOADERS
# =========================================================
import os
import ROOT

from default_config import ERAS, FLAVOURS


def load_histogram(file_path, hist_path):
    f = ROOT.TFile(file_path)

    if not f or f.IsZombie():
        print(f"[WARNING] Cannot open {file_path}")
        return None

    h = f.Get(hist_path)

    if not h:
        print(f"[WARNING] Missing {hist_path} in {file_path}")
        f.Close()
        return None

    h = h.Clone()
    h.SetDirectory(0)
    f.Close()

    return h


# =========================================================
# Path builder (SR3 only)
# =========================================================
def build_sr3_path(flav, hist_name):
    return f"PassSR3/HNL_ULIDv2/{flav}/MainPlots/{hist_name}"


# =========================================================
# BACKGROUND
# =========================================================
def load_background(base, flav, hist_name):

    hist = {}

    path = build_sr3_path(flav, hist_name)

    for era in ERAS:
        fname = os.path.join(base, era, "HNL_SignalRegion_Plotter_Bkg.root")
        h = load_histogram(fname, path)

        if h is None:
            hist[era] = []
        else:
            hist[era] = get_bins(h)

    return hist


# =========================================================
# FAKE
# =========================================================
def load_fake(base, flav, hist_name):

    hist = {}

    path = build_sr3_path(flav, hist_name)

    for era in ERAS:
        fname = os.path.join(base, era, "HNL_SignalRegion_Plotter_Fake.root")
        h = load_histogram(fname, path)

        if h is None:
            hist[era] = []
        else:
            hist[era] = get_bins(h)

    return hist


# =========================================================
# SIGNAL
# =========================================================
def load_signal(base, flav, mass, hist_name):

    hist = {}

    path = build_sr3_path(flav, hist_name)
    sig_name = f"HNL_DYVBF_{mass}"

    for era in ERAS:
        fname = os.path.join(base, era, f"HNL_SignalRegion_Plotter_{sig_name}.root")
        h = load_histogram(fname, path)

        if h is None:
            hist[era] = None
            continue

        if h.Integral() > 0:
            h.Scale(1.0 / h.Integral())

        hist[era] = h

    return hist


# =========================================================
# BIN EXTRACTION
# =========================================================
def get_bins(h):

    out = []

    for i in range(1, h.GetNbinsX() + 1):

        lo = h.GetBinLowEdge(i)
        hi = h.GetBinLowEdge(i + 1)
        val = h.GetBinContent(i)
        err = h.GetBinError(i)

        err2 = err * err

        out.append((lo, hi, val, err2))

    return out
