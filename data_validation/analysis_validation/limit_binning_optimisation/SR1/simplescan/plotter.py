import ROOT


def convert_results_for_plot(results, mode="run2"):

    out = {}

    # unify input format
    if isinstance(results, dict):
        iterable = []
        for flav in results:
            iterable.extend(results[flav])
    else:
        iterable = results

    for r in iterable:
        flav = r["flav"]
        mass = r["mass"]

        if flav not in out:
            out[flav] = []

        m = float(mass)
        val = r["run2"] if mode == "run2" else r["quad"]

        out[flav].append((m, val))

    for flav in out:
        out[flav].sort()

    return out

def make_mass_plot_multi(results_list, flav, out_tag="default"):

    import os
    c = ROOT.TCanvas(f"c_mass_{flav}", "", 800, 700)
    c.SetLeftMargin(0.12)
    c.SetBottomMargin(0.12)

    graphs = []
    all_y = []

    marker_styles = [20, 21, 22, 23, 24]
    line_styles   = [1, 2, 3, 4, 5]

    for idx, entry in enumerate(results_list):

        results = entry["results"]
        label   = entry["label"]

        if flav not in results:
            print(f"[DEBUG] {label} missing {flav}")
            continue

        data = results[flav]

        # handle dict OR list
        if isinstance(data, dict):
            points = sorted((float(m), f) for m, f in data.items())
        else:
            points = data

        if not points:
            print(f"[DEBUG] {label} empty")
            continue

        g = ROOT.TGraph()

        for i, (m, f) in enumerate(points):
            g.SetPoint(i, m, f)
            all_y.append(f)

        g.SetMarkerStyle(marker_styles[idx % len(marker_styles)])
        g.SetLineStyle(line_styles[idx % len(line_styles)])
        g.SetLineWidth(2)

        graphs.append((g, label))

    if not graphs:
        print("[WARNING] No graphs to draw")
        return

    # ------------------
    # Draw
    # ------------------
    first = True
    for g, _ in graphs:
        if first:
            g.Draw("APL")
            g.GetXaxis().SetTitle("Mass [GeV]")
            g.GetYaxis().SetTitle("FOM")
            g.SetMinimum(0.0)
            first = False
        else:
            g.Draw("PL SAME")

    if all_y:
        graphs[0][0].SetMaximum(max(all_y) * 1.5)

    # ------------------
    # Legend
    # ------------------
    leg = ROOT.TLegend(0.15,0.65,0.45,0.88)
    leg.SetTextSize(0.025)

    for g, label in graphs:
        leg.AddEntry(g, label, "lp")

    leg.Draw()

    txt = ROOT.TLatex()
    txt.SetNDC()
    txt.SetTextSize(0.04)
    txt.DrawLatex(0.18, 0.92, flav)

    # ------------------
    # Save
    # ------------------
    outdir = f"plots/{out_tag}"
    os.makedirs(outdir, exist_ok=True)

    c.SaveAs(f"{outdir}/fom_vs_mass_{flav}.pdf")
