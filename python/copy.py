#!/usr/bin/env python3

import os
import glob
import shutil
import argparse

ERAS = [
    "2016preVFP",
    "2016postVFP",
    "2017",
    "2018",
]

def copy_files(src_pattern, dst_dir, execute=False):

    files = sorted(glob.glob(src_pattern))

    if not files:
        print("[WARNING] No files found:")
        print("  {}".format(src_pattern))
        return

    os.makedirs(dst_dir, exist_ok=True)

    for src in files:

        dst = os.path.join(
            dst_dir,
            os.path.basename(src)
        )

        if execute:
            shutil.copy2(src, dst)
            print("[COPY] {} -> {}".format(src, dst))
        else:
            print("[DRYRUN] {} -> {}".format(src, dst))
            
def move_files(src_pattern, dst_dir, execute=False):

    files = sorted(glob.glob(src_pattern))

    if not files:
        print("[WARNING] No files found:")
        print("  {}".format(src_pattern))
        return

    os.makedirs(dst_dir, exist_ok=True)

    for src in files:

        dst = os.path.join(
            dst_dir,
            os.path.basename(src)
        )

        if execute:
            shutil.move(src, dst)
            print("[MOVE] {} -> {}".format(src, dst))
        else:
            print("[DRYRUN] {} -> {}".format(src, dst))


def main():

    parser = argparse.ArgumentParser(
        description="Move RunConv ROOT files."
    )

    parser.add_argument(
        "--execute",
        action="store_true",
        help="Actually move files. Default is dry-run."
    )

    args = parser.parse_args()

    for era in ERAS:

        #
        # Signal Region files
        #
        src_pattern = (
            "/data6/Users/jalmond/SKFlatOutput/"
            "Run2UltraLegacy_v3/"
            "HNL_SignalRegion_Plotter/{}/"
            "RunSyst__RegDepConv__RunConv__/*.root"
        ).format(era)

        dst_dir = (
            "/data9/Users/HNL_public/SUS-24-014/"
            "SKFlatOutput/Systematic_Run/"
            "HNL_SignalRegion_Plotter_ANv7_ConvUpdate_PR192/{}/"
            "RunSyst__RunConv__"
        ).format(era)

        print("")
        print("=" * 120)
        print("SignalRegion {}".format(era))
        print("=" * 120)

        copy_files(
            src_pattern,
            dst_dir,
            execute=args.execute
        )

        #
        # Control Region files
        #
        src_pattern = (
            "/data6/Users/jalmond/SKFlatOutput/"
            "Run2UltraLegacy_v3/"
            "HNL_ControlRegion_Plotter/{}/"
            "RunSyst__RegDepConv__MultiLepton__RunConv__/*.root"
        ).format(era)

        dst_dir = (
            "/data9/Users/HNL_public/SUS-24-014/"
            "SKFlatOutput/Systematic_Run/"
            "HNL_ControlRegion_Plotter_ANv7_ConvUpdate_PR192/{}/"
            "RunSyst__MultiLepton__RunConv__"
        ).format(era)

        print("")
        print("=" * 120)
        print("ControlRegion {}".format(era))
        print("=" * 120)

        copy_files(
            src_pattern,
            dst_dir,
            execute=args.execute
        )


if __name__ == "__main__":
    main()
