from loader import *

import numpy as np
import math

REGIONS = ["low", "high"]

def build_data(base, masses):

    data = {}

    # =========================================================
    # LOAD ALL
    # =========================================================
    bkg_all  = {r: {} for r in REGIONS}
    fake_all = {r: {} for r in REGIONS}
    sig_all  = {r: {} for r in REGIONS}

    for flav in FLAVOURS:

        print("\n[LOAD]", flav)

        low_bkg, high_bkg   = load_flavour(base, flav)
        low_fake, high_fake = load_fake(base, flav)
        low_sig, high_sig   = load_signals(base, flav)

        # -----------------------------
        # Background / Fake
        # -----------------------------
        bkg_all["low"][flav]  = low_bkg
        bkg_all["high"][flav] = high_bkg

        fake_all["low"][flav]  = low_fake
        fake_all["high"][flav] = high_fake

        # -----------------------------
        # Signals
        # -----------------------------
        sig_all["low"][flav]  = {}
        sig_all["high"][flav] = {}

        for m in masses:
            sig_all["low"][flav][m]  = low_sig[m]
            sig_all["high"][flav][m] = high_sig[m]

    # =========================================================
    # EDGES (assume same binning)
    # =========================================================
    ref = bkg_all["low"][FLAVOURS[0]][ERAS[0]]
    edges = np.array([b[0] for b in ref] + [ref[-1][1]])

    data["edges"] = edges

    # =========================================================
    # BACKGROUND / FAKE ARRAYS
    # =========================================================
    data["background"] = {r: {} for r in REGIONS}
    data["fake"]       = {r: {} for r in REGIONS}
    data["bkg_err2"]   = {r: {} for r in REGIONS}

    for r in REGIONS:
        for flav in FLAVOURS:

            data["background"][r][flav] = {}
            data["fake"][r][flav]       = {}
            data["bkg_err2"][r][flav]   = {}

            for era in ERAS:

                bins = bkg_all[r][flav][era]
                fake = fake_all[r][flav][era]

                B_arr, E_arr = bins_to_array_with_err(bins)

                data["background"][r][flav][era] = B_arr
                data["bkg_err2"][r][flav][era]   = E_arr
                data["fake"][r][flav][era]       = np.array([f[2] for f in fake])

    # =========================================================
    # SIGNAL ARRAYS
    # =========================================================
    data["signal"] = {r: {} for r in REGIONS}

    for r in REGIONS:
        for flav in FLAVOURS:

            data["signal"][r][flav] = {}

            for m in masses:
                data["signal"][r][flav][m] = {}

                for era in ERAS:
                    h = sig_all[r][flav][m][era]
                    data["signal"][r][flav][m][era] = hist_to_array(h)

    # =========================================================
    # SIGNAL COMBINED PER MASS (per region)
    # =========================================================
    data["signal_combined_mass"] = {r: {} for r in REGIONS}

    for r in REGIONS:
        for flav in FLAVOURS:

            data["signal_combined_mass"][r].setdefault(flav, {})

            for m in masses:

                arr = sum(
                    data["signal"][r][flav][m][era]
                    for era in ERAS
                )

                if arr.sum() > 0:
                    arr = arr / arr.sum()

                data["signal_combined_mass"][r][flav][m] = arr

    # =========================================================
    # GLOBAL SIGNAL (LOW+HIGH merged)
    # =========================================================
    data["signal_global"] = {}

    for m in masses:

        total = 0

        for r in REGIONS:
            for flav in FLAVOURS:
                for era in ERAS:
                    total += data["signal"][r][flav][m][era]

        if isinstance(total, np.ndarray) and total.sum() > 0:
            total = total / total.sum()

        data["signal_global"][m] = total

    # =========================================================
    # NORMALISATION (GLOBAL)
    # =========================================================
    data["norm"] = {}

    for m in masses:

        S = data["signal_global"][m]

        B = sum(
            data["background"][r][flav][era]
            for r in REGIONS
            for flav in FLAVOURS
            for era in ERAS
        )

        Z2 = 0.0

        for i in range(len(S)):
            s = S[i]
            b = B[i]

            if s > 0 and b > 0:
                Z2 += 2*((s+b)*math.log(1+s/b) - s)

        data["norm"][m] = Z2

    return data


