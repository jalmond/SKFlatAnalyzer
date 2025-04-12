#!/bin/bash

# Configuration
analyzer="HNL_Lepton_Conversion_Studies"
rundir="HNL_Lepton_Conversion_Studies"
mcpath="${SKFlat_WD}/runJobs/HNL/${analyzer}/Bkg/"
datapath="${SKFlat_WD}/runJobs/HNL/${analyzer}/DATA/"
njobs=100
njobs_data=100
nmax=300
skim=""

# Select eras
era_list=("2017")  # For individual era testing

# Parse optional user flags
userflags=""
if [[ "$1" == "--userflags" && -n "$2" ]]; then
  userflags="--userflags $2"
fi

# Run job submission
for era in "${era_list[@]}"; do
  echo "Submitting jobs for era: $era"

  # Submit MC jobs
  SKFlat.py -a "$analyzer" -i ZGToLLG     -n "$njobs" --nmax "$nmax" -e "$era" --skim SkimTree_DileptonBDT $userflags &
  SKFlat.py -a "$analyzer" -i WGToLNuG    -n "$njobs" --nmax "$nmax" -e "$era" --skim SkimTree_DileptonBDT $userflags &
  SKFlat.py -a "$analyzer" -l runJobs/SampleLists/Bkg/Conv/ConvExt.txt -n "$njobs" --nmax "$nmax" -e "$era" --skim SkimTree_DileptonBDT $userflags &

  # Add more submissions below if needed
done

wait  # Wait for all background jobs to finish
echo "All submissions complete."

