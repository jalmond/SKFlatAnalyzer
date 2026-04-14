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
