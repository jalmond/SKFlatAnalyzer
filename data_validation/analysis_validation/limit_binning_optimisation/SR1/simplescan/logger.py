
def format_edges(edges):
    return ", ".join(f"{e:.0f}" for e in edges)

def print_final_summary(results):

    print("\n==============================")
    print(" FINAL SUMMARY (WITH BINNING)")
    print("==============================")

    for r in results:

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

        if binning["Run2"] is not None:
            print("\n[Run2 binning]")
            print(binning["Run2"])

        if binning["per_era"]:
            print("\n[Per-era binning]")
            for era, edges in binning["per_era"].items():
                print(f"{era:10s} : {edges}")


                
def fmt_ratio(r):
    if r < 0.9:
        return f"\033[91m{r:.4f}\033[0m"   # red                                                                                                                                    
    elif r < 0.98:
        return f"\033[93m{r:.4f}\033[0m"   # yellow                                                                                                                                 
    else:
        return f"{r:.4f}"
