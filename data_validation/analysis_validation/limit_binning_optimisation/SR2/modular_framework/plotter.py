def format_bin_range(n_bins):
    return "{}to{}bins".format(min(n_bins), max(n_bins))

def format_edges(edges):
    return ",".join("{:.2f}".format(x) for x in edges[1:-1])

def make_lastbin_zoom_plot(results, flav):

    c = ROOT.TCanvas("c_zoom","",1000,700)
    c.SetBottomMargin(0.25)
    g = ROOT.TGraph()

    labels = ["Run2", "PerEra", "Combined", "EraDep", "PreRun2", "PrePer"]

    values = [
        results["scan_run"][-1][1],
        results["scan_per"][-1][1],
        results["combined_run"][-1][1],
        results["era_run"][-1][1],
        results["predefined_run"][-1][1],
        results["predefined_per"][-1][1]
    ]

    # Fill graph                                                                                                                                                                                                                                                                                                                                
    for i, v in enumerate(values):
        g.SetPoint(i, i+1, v)

    g.SetMarkerStyle(20)
    g.SetMarkerSize(1.8)
    g.SetLineWidth(2)

    g.Draw("AP")

    # -----------------------------                                                                                                                                                                                                                                                                                                             
    # Axis setup                                                                                                                                                                                                                                                                                                                                
    # -----------------------------                                                                                                                                                                                                                                                                                                             
    g.GetXaxis().SetLimits(0.5, len(labels)+0.5)
    g.GetYaxis().SetTitle("FOM (n = {})".format(n_bin_torun[-1]))
    g.GetYaxis().SetTitleOffset(1.4)

    # ---- Rotate labels (KEY FIX)                                                                                                                                                                                                                                                                                                              
    g.GetXaxis().SetLabelSize(0)

    # -----------------------------                                                                                                                                                                                                                                                                                                             
    # Better Y range (KEY FIX)                                                                                                                                                                                                                                                                                                                  
    # -----------------------------                                                                                                                                                                                                                                                                                                             
    ymin = min(values)
    ymax = max(values)

    center = 0.5 * (ymin + ymax)
    half_range = 0.5 * (ymax - ymin)

    # protect against flat case                                                                                                                                                                                                                                                                                                                 
    if half_range == 0:
        half_range = 0.1

    half_range *= 1.6

    g.SetMinimum(center - half_range)
    g.SetMaximum(center + half_range)
    g.SetMarkerSize(1.2)

    c.Modified()
    c.Update()


    # -----------------------------                                                                                                                                                                                                                                                                                                             
    # Draw rotated X labels                                                                                                                                                                                                                                                                                                                     
    # -----------------------------                                                                                                                                                                                                                                                                                                             
    g.GetXaxis().SetLabelSize(0)
    txt = ROOT.TLatex()
    txt.SetTextSize(0.028)
    txt.SetTextAlign(23)
    c.cd()

    for i, lab in enumerate(labels):
        x = i+1
        y = g.GetMinimum() - 0.1*(g.GetMaximum() - g.GetMinimum())
        txt.SetTextAngle(90)
        txt.DrawLatex(x, y, lab)
    txt.SetTextAngle(0)


    # -----------------------------                                                                                                                                                                                                                                                                                                             
    # Draw values on points                                                                                                                                                                                                                                                                                                                     
    # -----------------------------                                                                                                                                                                                                                                                                                                             
    txt.SetTextAlign(22)
    txt.SetTextSize(0.03)

    for i, v in enumerate(values):
        offset = 0.03 * (g.GetMaximum() - g.GetMinimum())
        txt.DrawLatex(i+1, v + offset, "{:.3f}".format(v))

    # -----------------------------                                                                                                                                                                                                                                                                                                             
    # Add same text block as main plot                                                                                                                                                                                                                                                                                                          
    # -----------------------------                                                                                                                                                                                                                                                                                                             
    info = ROOT.TLatex()
    info.SetNDC()
    info.SetTextSize(0.035)

    info.DrawLatex(0.75, 0.87, "Flavour: {}".format(flav))
    info.DrawLatex(0.75, 0.82, "n bins = {}".format(n_bin_torun[-1]))

    bin_tag = format_bin_range(n_bin_torun)
    c.SaveAs("plots/fom_lastbin_zoom_{}_{}.pdf".format(flav, bin_tag))



def make_plot_v2(results, flav,
                 best_scan_run,
                 best_scan_per,      # NEW                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
                 best_combined_run,
                 cfg_pre_run,        # NEW                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
                 best_pre_run,
                 OPT_MASSES,
                 EVAL_MASSES):


    # PerEra                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    per_low  = format_edges(best_scan_per[1])
    per_high = format_edges(best_scan_per[2])


    # Predefined                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    edges_low_pre, edges_high_pre = cfg_pre_run
    pre_low  = format_edges(edges_low_pre)
    pre_high = format_edges(edges_high_pre)

    c = ROOT.TCanvas("c","",800,700)

    mg = ROOT.TM-ultiGraph()

    def make_graph(data, style, marker):
        g = ROOT.TGraph()
        for i,(x,y) in enumerate(data):
            g.SetPoint(i, x, y)

        g.SetLineStyle(style)
        g.SetMarkerStyle(marker)
        g.SetLineWidth(2)
        g.SetMarkerSize(1.2)

        return g


    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    # GRAPHS                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    g_scan = make_graph(results["scan_run"], 1, 20)
    g_comb = make_graph(results["combined_run"], 2, 24)
    g_per = make_graph(results["scan_per"], 2, 21)
    g_era = make_graph(results["era_run"], 4, 25)

    mg.Add(g_scan)
    mg.Add(g_comb)
    mg.Add(g_per)
    mg.Add(g_era)

    mg.Draw("APL")

    # ----------------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
    # AUTO Y-RANGE (ALL GRAPHS)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
    # ----------------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
    ymin = 1e9
    ymax = -1e9

    for key in ["scan_run", "combined_run", "scan_per", "era_run"]:
        for x, y in results.get(key, []):
            if y < ymin: ymin = y
            if y > ymax: ymax = y

    # protect against empty                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
    if ymin < ymax:
        ymin *= 0.9
        ymax *= 1.3

        mg.SetMinimum(ymin)
        mg.SetMaximum(ymax)

    xmin = mg.GetXaxis().GetXmin()
    xmax = mg.GetXaxis().GetXmax()
    mg.GetXaxis().SetTitle("N bins")
    mg.GetYaxis().SetTitle("Best FOM")
    mg.SetTitle("Best FOM vs N bins")

    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    # LEGEND LABELS WITH BIN INFO                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    scan_low  = format_edges(best_scan_run[1])
    scan_high = format_edges(best_scan_run[2])

    comb_low  = format_edges(best_combined_run[1])
    comb_high = format_edges(best_combined_run[2])

    label_scan = "Run2 binning (1 flav, Run2 stat) ({})|({})".format(scan_low, scan_high)
    label_comb = "Run2 binning (3 flav, Run2 stat) ({})|({})".format(comb_low, comb_high)
    label_per  = "Run2 binning (1 flav, per-era stat) ({})|({})".format(per_low, per_high)

    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    # LEGEND                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    leg = ROOT.TLegend(0.12, 0.72, 0.45, 0.88)
    leg.SetTextSize(0.02)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetEntrySeparation(0.01)
    leg.AddEntry(g_scan, label_scan, "lp")
    leg.AddEntry(g_comb, label_comb, "lp")
    leg.AddEntry(g_per, label_per, "lp")
    leg.AddEntry(g_era, "Era-dependent binning (1 flav, per-era stat)", "lp")


    # --- horizontal band (thin box)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    band = ROOT.TBox(xmin, best_pre_run*0.98, xmax, best_pre_run*1.02)
    band.SetFillColorAlpha(ROOT.kGray, 0.3)
    band.SetLineColor(ROOT.kGray)
    band.Draw("SAME")

    # --- central line                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
    line_pre = ROOT.TLine(xmin, best_pre_run, xmax, best_pre_run)
    line_pre.SetLineStyle(2)
    line_pre.SetLineWidth(2)
    line_pre.Draw("SAME")

    # --- marker at best N (optional)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
    best_n = max(
        results["scan_run"] +
        results["combined_run"] +
        results["scan_per"] +
        results["era_run"],
        key=lambda x: x[1]
    )[0]

    g_pre = ROOT.TGraph(1)
    g_pre.SetPoint(0, best_n, best_pre_run)
    g_pre.SetMarkerStyle(29)
    g_pre.SetMarkerSize(2)
    g_pre.Draw("P SAME")

    #leg.AddEntry(line_pre, "Predefined (Run2 grid)", "l")                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  

    label_pre = "Grid Scan Run2 ({}) | ({})".format(pre_low, pre_high)
    leg.AddEntry(line_pre, label_pre, "l")
    leg.Draw()

    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    # TEXT BLOCK (like old code)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    # -----------------------------                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    def format_mass(m):
        if m == "0":
            return "Weinberg"
        if m.startswith("SSWW_"):
            return m.split("_")[1]
        return str(m)

    opt_str  = ", ".join(format_mass(m) for m in OPT_MASSES)
    eval_str = ", ".join(format_mass(m) for m in EVAL_MASSES)

    txt = ROOT.TLatex()
    txt.SetNDC()
    txt.SetTextSize(0.025)

    txt.DrawLatex(0.65,0.83, "Flavour: {}".format(flav))
    txt.DrawLatex(0.65,0.78, "Opt: {}".format(opt_str))
    txt.DrawLatex(0.65,0.73, "Eval: {}".format(eval_str))

    bin_tag = format_bin_range(n_bin_torun)
    c.SaveAs("plots/fom_vs_nbins_{}_{}.pdf".format(flav, bin_tag))


    
