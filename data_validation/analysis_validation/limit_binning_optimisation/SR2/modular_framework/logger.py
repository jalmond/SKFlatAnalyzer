def print_final_summary(results):

    print("\n==============================")
    print(" FINAL SUMMARY (WITH BINNING)")
    print("==============================")

    # Handle dict or list
    if isinstance(results, dict):
        iterable = []
        for flav in results:
            iterable.extend(results[flav])
    else:
        iterable = results

    for r in iterable:

        flav = r["flav"]
        mass = r["mass"]
        nbins = r["nbins"]
        quad = r["quad"]
        run2 = r["run2"]
        ratio = r["ratio"]

        print("\n----------------------------------------")
        print(f"{flav}  Mass={mass}  Nbins={nbins}")
        print("----------------------------------------")

        print(f"QUAD  = {quad:.4f}")
        print(f"Run2  = {run2:.4f}")
        print(f"Ratio = {ratio:.4f}")

        # ------------------------
        # Binning
        # ------------------------
        binning = r["binning"]

        print("\n[LOW binning]")
        print(binning["low"])

        print("\n[HIGH binning]")
        print(binning["high"])

        # ------------------------
        # Bin stats
        # ------------------------
        if "bin_stats" in r:

            print("\n[Per-era bin stats]")

            for era, bins in r["bin_stats"]["per_era"].items():

                B_vals = [round(b["B"], 3) for b in bins]
                rel_vals = [
                    round(math.sqrt(b["E"]) / b["B"], 3) if b["B"] > 0 else 0.0
                    for b in bins
                ]

                print(f"{era:10s}")
                print(f"{'':10s}   B   = {B_vals}")
                print(f"{'':10s}   rel = {rel_vals}")

            # ------------------------
            # Run2 bin stats
            # ------------------------
            print("\n[Run2 bin stats]")

            B_vals = [round(b["B"], 3) for b in r["bin_stats"]["Run2"]]
            rel_vals = [
                round(math.sqrt(b["E"]) / b["B"], 3) if b["B"] > 0 else 0.0
                for b in r["bin_stats"]["Run2"]
            ]

            print(f"B   = {B_vals}")
            print(f"rel = {rel_vals}")
            
def check_bins(edges, bins, threshold):
    results = []
    for i in range(len(edges)-1):
        lo, hi = edges[i], edges[i+1]
	sub = [b for b in bins if lo <= b[0] < hi]

        val = sum(x[2] for x in sub)
	ok = val > threshold

	results.append((round(val,2), ok))
    return results



def fmt(edges):
    return "[" + ", ".join(f"{x:.2f}" for x in edges) + "]"

def color_val(v):
    if v < 0.5:
        return f"{RED}{v:.2f}{RESET}"
    elif v < 5.0:
        return f"\033[93m{v:.2f}{RESET}"   # yellow                                                                                                                                                                                  
    else:
        return f"{GREEN}{v:.2f}{RESET}"

def fmt_colored(vals):
    return "[" + ", ".join(color_val(v) for v in vals) + "]"


def check_bins(edges, bins, threshold):
    results = []
    for i in range(len(edges)-1):
        lo, hi = edges[i], edges[i+1]
        sub = [b for b in bins if lo <= b[0] < hi]

        val = sum(x[2] for x in sub)
        ok = val > threshold

        results.append((round(val,2), ok))
    return results



