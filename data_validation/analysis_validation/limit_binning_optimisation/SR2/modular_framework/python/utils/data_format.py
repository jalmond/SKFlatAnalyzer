from loader import *

import numpy as np
import math

REGIONS = ["low", "high"]


def hist_to_array(h):
    if h is None:
        return None
    return np.array([h.GetBinContent(i) for i in range(1, h.GetNbinsX() + 1)])


def build_data(base, masses):

    data = {}

    # =========================================================
    # LOOP REGIONS (SR3-style but no MET)
    # =========================================================
    for r in REGIONS:

        print(100 * "_")
        print(f"[INFO] Loading region: {r}")

        subdata = {}

        bkg_all = {}
        fake_all = {}
        sig_all = {}

        # ----------------------------------
        # LOAD
        # ----------------------------------
        for flav in FLAVOURS:

            print("[LOAD]", flav, r)

            low_bkg, high_bkg = load_flavour(base, flav)
            low_fake, high_fake = load_fake(base, flav)
            low_sig, high_sig = load_signals(base, flav)

            if r == "low":
                bkg_all[flav] = low_bkg
                fake_all[flav] = low_fake
                sig_src = low_sig
            else:
                bkg_all[flav] = high_bkg
                fake_all[flav] = high_fake
                sig_src = high_sig

            sig_all[flav] = {}
            for m in masses:
                sig_all[flav][m] = sig_src[m]

        # ----------------------------------
        # EDGES
        # ----------------------------------
        ref = None
        for flav in FLAVOURS:
            for era in ERAS:
                if len(bkg_all[flav][era]) > 0:
                    ref = bkg_all[flav][era]
                    break
            if ref is not None:
                break

        if ref is None:
            print(f"[WARNING] No valid histogram for region {r}")
            continue

        edges = np.array([b[0] for b in ref] + [ref[-1][1]])
        subdata["edges"] = edges

        nbins = len(edges) - 1

        # ----------------------------------
        # BACKGROUND / FAKE
        # ----------------------------------
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
                    B_arr, E_arr = bins_to_array_with_err(bins)

                subdata["background"][flav][era] = B_arr
                subdata["bkg_err2"][flav][era] = E_arr

                if len(fake) == 0:
                    subdata["fake"][flav][era] = np.zeros(nbins)
                else:
                    subdata["fake"][flav][era] = np.array([f[2] for f in fake])

        # ----------------------------------
        # SIGNAL
        # ----------------------------------
        subdata["signal"] = {}

        for flav in FLAVOURS:
            subdata["signal"][flav] = {}

            for m in masses:
                subdata["signal"][flav][m] = {}

                for era in ERAS:

                    h = sig_all[flav][m][era]

                    if h is None:
                        arr = np.zeros(nbins)
                    else:
                        arr = hist_to_array(h)

                    subdata["signal"][flav][m][era] = arr

        # ----------------------------------
        # SIGNAL COMBINED PER MASS
        # ----------------------------------
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

        # ----------------------------------
        # STORE REGION
        # ----------------------------------
        data[r] = subdata

    # =========================================================
    # GLOBAL SIGNAL + NORM (across regions)
    # =========================================================
    data["signal_global"] = {}
    data["norm"] = {}

    for m in masses:

        total = 0

        for r in REGIONS:
            for flav in FLAVOURS:
                for era in ERAS:
                    total += data[r]["signal"][flav][m][era]

        if isinstance(total, np.ndarray) and total.sum() > 0:
            total = total / total.sum()

        data["signal_global"][m] = total

        # ----------------------------------
        # NORM
        # ----------------------------------
        B = sum(
            data[r]["background"][flav][era]
            for r in REGIONS
            for flav in FLAVOURS
            for era in ERAS
        )

        Z2 = 0.0

        for i in range(len(total)):
            s = total[i]
            b = B[i]

            if s > 0 and b > 0:
                Z2 += 2 * ((s + b) * math.log(1 + s / b) - s)

        data["norm"][m] = Z2

    return data



