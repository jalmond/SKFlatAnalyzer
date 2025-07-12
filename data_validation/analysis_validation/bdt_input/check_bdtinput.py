import ROOT
import os

# Configuration
versions = {
    "V2": "HNL_SR3_BDT_KinVar_BDTV2",
    "V3": "HNL_SR3_BDT_KinVar_BDTV3"
}

eras = ["2016postVFP", "2016preVFP", "2017", "2018"]

c_list = [
    "RunConv__/HNL_SR3_BDT_KinVar_Conv.root",
    "RunCF__/HNL_SR3_BDT_KinVar_CF.root",
    "RunFake__/HNL_SR3_BDT_KinVar_MCFake_SSOS.root",
    "RunPrompt__/HNL_SR3_BDT_KinVar_Prompt.root"
]

hist_names = ["sumw_EE", "sumw_EMu", "sumw_MuMu"]

# Pretty header
header = (
    "{:<12} | {:<35} | {:<8} | {:>10} {:>20} | {:>10} {:>20} || {:>9} {:>9}"
    .format("Era", "File", "Hist", "Entries V2", "Int V2 [%]", "Entries V3", "Int V3 [%]", "%Ent", "%Int")
)
print(header)
print("=" * len(header))

def percent_change(v2, v3):
    if v2 and v2 != 0 and v3 is not None:
        return "{:+.2f}%".format((v3 - v2) / v2 * 100)
    else:
        return "   -   "

for era in eras:
    print("=" * len(header))
    print("{:<12} | {:<35} | {:<8}".format(era, "", ""))  # Era line once

    for hist_name in hist_names:
        hist_short = hist_name.replace("sumw_", "")

        # Collect all integrals/entries first
        total_integrals = {"V2": 0.0, "V3": 0.0}
        integrals_by_file = {"V2": {}, "V3": {}}
        entries_by_file = {"V2": {}, "V3": {}}

        for c in c_list:
            file_display_name = os.path.basename(c)

            for version_key, version_path in versions.items():
                if c.startswith("/"):
                    filepath = c.replace(versions["V2"], version_path).replace(versions["V3"], version_path)
                else:
                    filepath = f"/data9/Users/HNL_public/SUS-24-014/BDTInput/{version_path}/{era}/{c}"

                entries = None
                integral = 0.0
                if os.path.isfile(filepath):
                    f = ROOT.TFile.Open(filepath)
                    if f and not f.IsZombie():
                        hist = f.Get(hist_name)
                        if hist:
                            entries = int(hist.GetEntries())
                            integral = hist.Integral()
                        f.Close()

                integrals_by_file[version_key][file_display_name] = integral
                entries_by_file[version_key][file_display_name] = entries
                total_integrals[version_key] += integral

        # Print histogram block header line
        print(" " * 13 + "| {:<35} | {:<8}".format("", hist_short))

        for c in c_list:
            file_display_name = os.path.basename(c)

            ent_v2 = entries_by_file["V2"][file_display_name]
            ent_v3 = entries_by_file["V3"][file_display_name]

            int_v2_val = integrals_by_file["V2"][file_display_name]
            int_v3_val = integrals_by_file["V3"][file_display_name]

            int_v2_pct = (int_v2_val / total_integrals["V2"] * 100) if total_integrals["V2"] > 0 else 0
            int_v3_pct = (int_v3_val / total_integrals["V3"] * 100) if total_integrals["V3"] > 0 else 0

            int_v2_str = f"{int_v2_val:6.2f} [{int_v2_pct:5.2f}%]" if int_v2_val is not None else "-"
            int_v3_str = f"{int_v3_val:6.2f} [{int_v3_pct:5.2f}%]" if int_v3_val is not None else "-"

            pct_entries = percent_change(ent_v2, ent_v3)
            pct_int = percent_change(int_v2_val, int_v3_val)

            print("{:<12} | {:<35} | {:<8} | {:>10} {:>20} | {:>10} {:>20} || {:>9} {:>9}".format(
                "", file_display_name, "",
                ent_v2 if ent_v2 is not None else "-",
                int_v2_str,
                ent_v3 if ent_v3 is not None else "-",
                int_v3_str,
                pct_entries,
                pct_int
            ))

        # Separator after each histogram block
        print("-" * (len(header) - 12))  # exclude Era col indent
