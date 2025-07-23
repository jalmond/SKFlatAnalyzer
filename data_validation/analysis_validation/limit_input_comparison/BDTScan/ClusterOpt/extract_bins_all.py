import os
import re

# Settings
log_dir = "logs"
n_bins = 35
n_max_bins = 35
masses = ['100', '125', '150', '200', '250', '300', '400', '500']
flavours = ['EE', 'MuMu', 'EMu']
eras = ['2016preVFP', '2016postVFP', '2017', '2018']

def extract_bin_edges(filepath):
    try:
        with open(filepath, "r") as f:
            content = f.read()
    except Exception:
        return []

    match = re.search(
        r"\[RESULT\] Best total FOM:.*?using \d+ bins with strict-bin merge scan:\n((?:\s*Bin\s+\d+:.*\n?)+)",
        content,
    )
    if not match:
        return []

    bin_lines = match.group(1).strip().split("\n")
    edges = []
    for line in bin_lines:
        edge_match = re.search(r"X = \[([-\d.]+), ([-\d.]+)\)", line)
        if edge_match:
            hi = float(edge_match.group(1))
            lo = float(edge_match.group(2))
            edges.append(hi)
    # Remove edges at -1 and 1, sort and deduplicate
    edges = sorted(set(e for e in edges if e != -1.0 and e != 1.0))
    return edges

def print_bins():
    for mass in masses:
        print(f"\n========== Mass {mass} ===========")
        for mode in ["loose", "very_loose"]:
            for flav in flavours:
                print(f"//{flav} {mode.capitalize()}")
                for era in eras:
                    fname = (
                        f"scan_loose_{mass}_{flav}_{era}_{n_max_bins}_{n_bins}.out"
                        if mode == "loose"
                        else f"scan_{mass}_{flav}_{era}_{n_max_bins}_{n_bins}.out"
                    )
                    fpath = os.path.join(log_dir, fname)
                    if not os.path.exists(fpath):
                        print(f"  [WARN] Missing file: {fname}")
                        continue
                    edges = extract_bin_edges(fpath)
                    if not edges:
                        print(f"  [WARN] No valid result in: {fname}")
                        continue
                    edge_str = ",".join(f"{e:.3f}" for e in edges)
                    print(f'map_bdt_limit_bins["SR3BDT_{flav}_{era}_{mass}"] = {{{edge_str}}};')

if __name__ == "__main__":
    print_bins()
