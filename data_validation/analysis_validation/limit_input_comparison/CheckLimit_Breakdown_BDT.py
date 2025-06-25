import os
import ROOT
import sys

# Function to list and select a directory (prints to screen)
def select_directory(base_path):
    print(f"Listing directories in: {base_path}")
    directories = [d for d in os.listdir(base_path) if os.path.isdir(os.path.join(base_path, d))]

    if not directories:
        print("No directories found in the specified path.")
        exit()

    for idx, directory in enumerate(directories, 1):
        print(f"{idx}. {directory}")

    while True:
        try:
            choice = int(input(f"Select a directory by entering the number (1-{len(directories)}): "))
            if 1 <= choice <= len(directories):
                selected_dir = directories[choice - 1]
                print(f"You selected: {selected_dir}")
                return os.path.join(base_path, selected_dir)
            else:
                print(f"Please enter a number between 1 and {len(directories)}.")
        except ValueError:
            print("Invalid input. Please enter a number.")

# Redirects stdout to a file temporarily
class Logger:
    def __init__(self, filepath):
        os.makedirs(os.path.dirname(filepath), exist_ok=True)
        self.terminal = sys.stdout
        self.log = open(filepath, "w")

    def write(self, message):
        self.terminal.write(message)
        self.log.write(message)

    def flush(self):
        self.terminal.flush()
        self.log.flush()

# Colored terminal output (only visible in screen, not in log file)
def print_in_color(message, is_red=False):
    if is_red:
        print(f"\033[31m{message}\033[0m")
    else:
        print(message)

# === Parameters ===
base_dir = '/data6/Users/jalmond/2020/Plotter/HNDiLeptonWorskspace/InputFiles/MergedFiles/HNL_SignalRegion_Plotter/'
eras = ['2016preVFP', '2016postVFP', '2017', '2018']
masses = ['100', '200','300','400', '500']
background_files = [
    'HNL_SignalRegion_Plotter_CF.root',
    'HNL_SignalRegion_Plotter_Conv.root',
    'HNL_SignalRegion_Plotter_Fake.root',
    'HNL_SignalRegion_Plotter_Other.root',
    'HNL_SignalRegion_Plotter_TTV.root',
    'HNL_SignalRegion_Plotter_VVV.root',
    'HNL_SignalRegion_Plotter_WW.root',
    'HNL_SignalRegion_Plotter_WZ.root',
    'HNL_SignalRegion_Plotter_ZZ.root'
]

# Select directory
dir_path = select_directory(base_dir)

# Main loop over eras and masses
for era in eras:
    for mass in masses:
        # Step 1: Collect all histograms
        all_histograms = set()
        for bkg_file in background_files:
            file_path = os.path.join(dir_path, era, bkg_file)
            if not os.path.exists(file_path):
                continue
            root_file = ROOT.TFile.Open(file_path)
            if not root_file or root_file.IsZombie():
                continue
            hist_dir = root_file.Get(f'LimitExtractionBDT/HNL_ULIDv2/LL/M{mass}/LimitBins')
            if hist_dir:
                for key in hist_dir.GetListOfKeys():
                    all_histograms.add(key.GetName())
            root_file.Close()

        # Step 2: Process each histogram separately
        for hist_name in sorted(all_histograms):
            red_lines = []
            bkg_totals = {'content': [0] * 100, 'error': [0] * 100}  # Will resize later
            bkg_contributions = {bkg_file: {} for bkg_file in background_files}

            for bkg_file in background_files:
                file_path = os.path.join(dir_path, era, bkg_file)
                if not os.path.exists(file_path):
                    continue

                root_file = ROOT.TFile.Open(file_path)
                if not root_file or root_file.IsZombie():
                    continue

                hist_dir = root_file.Get(f'LimitExtractionBDT/HNL_ULIDv2/LL/M{mass}/LimitBins')
                if not hist_dir or not hist_dir.Get(hist_name):
                    root_file.Close()
                    continue

                histogram = hist_dir.Get(hist_name)
                if not isinstance(histogram, ROOT.TH1):
                    root_file.Close()
                    continue

                # Resize arrays on first valid hist
                nbins = histogram.GetNbinsX() + 1
                if bkg_totals['content'] == [0] * 100:
                    bkg_totals['content'] = [0] * nbins
                    bkg_totals['error'] = [0] * nbins

                for bin_index in range(1, histogram.GetNbinsX() + 1):
                    bin_content = histogram.GetBinContent(bin_index)
                    bin_error = histogram.GetBinError(bin_index)

                    bkg_totals['content'][bin_index] += bin_content
                    bkg_totals['error'][bin_index] += bin_error ** 2

                    if bin_index not in bkg_contributions[bkg_file]:
                        bkg_contributions[bkg_file][bin_index] = {'content': 0, 'error': 0}
                    bkg_contributions[bkg_file][bin_index]['content'] += bin_content
                    bkg_contributions[bkg_file][bin_index]['error'] += bin_error ** 2

                root_file.Close()

            # Step 3: Logging
            log_path = f"log/{hist_name}/M{mass}_{era}.log"
            sys.stdout = Logger(log_path)

            print(f"\n==> Era: {era}, Mass: M{mass}, Histogram: {hist_name}")
            for bin_index in range(1, len(bkg_totals['content'])):
                total_bkg_content = bkg_totals['content'][bin_index]
                total_bkg_error = (bkg_totals['error'][bin_index])**0.5
                bin_percentage_error = (total_bkg_error / total_bkg_content * 100) if total_bkg_content != 0 else 0

                msg = f"Bin {bin_index}: [Bkg] Content: {total_bkg_content:.2f}, Error: {total_bkg_error:.2f}, % Error: {bin_percentage_error:.2f}%"
                print_in_color(msg, is_red=bin_percentage_error > 30)
                if bin_percentage_error > 30:
                    red_lines.append(f"{era} M{mass} {hist_name} Bin {bin_index}: {bin_percentage_error:.2f}%")

                for bkg_file in background_files:
                    if bin_index in bkg_contributions[bkg_file]:
                        content = bkg_contributions[bkg_file][bin_index]['content']
                        error = (bkg_contributions[bkg_file][bin_index]['error'])**0.5
                        label = bkg_file.replace("HNL_SignalRegion_Plotter_", "").replace(".root", "")
                        print(f"  [{label}] Content: {content:.2f}, Error: {error:.2f}")

            if red_lines:
                print("\nLines with percentage error > 30%:")
                for line in red_lines:
                    print(line)

            sys.stdout.log.close()
            sys.stdout = sys.stdout.terminal  # Restore

print("All processing completed.")
