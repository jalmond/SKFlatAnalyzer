from python.utils.loader import *

import numpy as np
import math

REGIONS = ["low", "high"]


def hist_to_array(h):
    if h is None:
        return None
    return np.array([h.GetBinContent(i) for i in range(1, h.GetNbinsX() + 1)])

# =========================================================
# MAIN LOADER
# =========================================================
def build_data(base, masses, sig_name="HNL"):

    data = {}

    base_dir=base


    for region in REGIONS:

        print(100 * "_")
        print(f"[INFO] Loading region: {region}")

        subdata = {}

        def get_hist_name():
            return "HT_PT1_LowDPhi" if region == "low" else "HT_PT1_HighDphi"

        bkg_all = {}
        fake_all = {}
        sig_all = {}

        # =====================================================
        # LOAD ALL
        # =====================================================
        for flav in FLAVOURS:

            bkg_all[flav] = {}
            fake_all[flav] = {}
            sig_all[flav] = {}

            for era in ERAS:

                path = f"PassSR2/HNL_ULIDv2/{flav}/MainPlots/{get_hist_name()}"

                # --- BKG
                fname = os.path.join(base_dir, era, "HNL_SignalRegion_Plotter_Bkg.root")
                h = load_histogram(fname, path)
                bkg_all[flav][era] = get_bins(h) if h else []

                # --- FAKE
                fname = os.path.join(base_dir, era, "HNL_SignalRegion_Plotter_Fake.root")
                h = load_histogram(fname, path)
                fake_all[flav][era] = get_bins(h) if h else []

            # --- SIGNAL
            for m in masses:

                sig_all[flav][m] = {}

                sig_sample = "Weinberg" if m == "0" else f"{sig_name}_{m}"

                for era in ERAS:

                    fname = os.path.join(
                        base_dir, era,
                        f"HNL_SignalRegion_Plotter_{sig_sample}.root"
                    )

                    h = load_histogram(fname, path)

                    if h and h.Integral() > 0:
                        h.Scale(1.0 / h.Integral())

                    sig_all[flav][m][era] = h

        # =====================================================
        # EDGES
        # =====================================================
        ref = None
        for flav in FLAVOURS:
            for era in ERAS:
                if len(bkg_all[flav][era]) > 0:
                    ref = bkg_all[flav][era]
                    break
            if ref is not None:
                break

        if ref is None:
            print(f"[WARNING] No valid histogram for {region}")
            continue

        print("\n[DEBUG RAW BINS]")

        for i in range(5):
        
            print(ref[i])
            
        edges = np.array([b[0] for b in ref] + [ref[-1][1]])
        subdata["edges"] = edges
        nbins = len(edges) - 1

        # =====================================================
        # BACKGROUND / FAKE
        # =====================================================
        subdata["background"] = {}
        subdata["fake"] = {}
        subdata["bkg_err2"] = {}

        for flav in FLAVOURS:

            subdata["background"][flav] = {}
            subdata["fake"][flav] = {}
            subdata["bkg_err2"][flav] = {}

            for era in ERAS:

                bins = bkg_all[flav][era]
                fake = fake_all[flav][era]

                if len(bins) == 0:
                    B_arr = np.zeros(nbins)
                    E_arr = np.zeros(nbins)
                else:
                    B_arr = np.array([b[2] for b in bins])
                    E_arr = np.array([b[3] for b in bins])

                subdata["background"][flav][era] = B_arr
                subdata["bkg_err2"][flav][era] = E_arr

                subdata["fake"][flav][era] = (
                    np.zeros(nbins) if len(fake) == 0
                    else np.array([f[2] for f in fake])
                )

        # =====================================================
        # SIGNAL ARRAYS
        # =====================================================
        subdata["signal"] = {}

        for flav in FLAVOURS:
            subdata["signal"][flav] = {}

            for m in masses:
                subdata["signal"][flav][m] = {}

                for era in ERAS:

                    h = sig_all[flav][m][era]
                    arr = hist_to_array(h)

                    subdata["signal"][flav][m][era] = arr

        # =====================================================
        # SIGNAL COMBINED
        # =====================================================
        subdata["signal_combined_mass"] = {}

        for flav in FLAVOURS:
            subdata["signal_combined_mass"][flav] = {}

            for m in masses:

                arr = sum(
                    subdata["signal"][flav][m][era]
                    for era in ERAS
                )

                if arr.sum() > 0:
                    arr = arr / arr.sum()

                subdata["signal_combined_mass"][flav][m] = arr

        data[region] = subdata

    # =====================================================
    # GLOBAL SIGNAL + NORM
    # =====================================================
    data["signal_global"] = {}
    data["norm"] = {}

    for m in masses:

        total = np.zeros_like(data["low"]["edges"][:-1])

        for region in REGIONS:
            for flav in FLAVOURS:
                for era in ERAS:
                    total += data[region]["signal"][flav][m][era]

        if total.sum() > 0:
            total = total / total.sum()

        data["signal_global"][m] = total

        B = sum(
            data[region]["background"][flav][era]
            for region in REGIONS
            for flav in FLAVOURS
            for era in ERAS
        )

        Z2 = 0.0

        for i in range(len(total)):
            s = total[i]
            b = B[i]

            if s > 0 and b > 0:
                Z2 += 2 * ((s + b) * np.log(1 + s / b) - s)

        data["norm"][m] = Z2

    return data








