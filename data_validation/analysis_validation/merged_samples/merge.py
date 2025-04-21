import os
import sys
import argparse
import subprocess
import shutil
import logging
from pathlib import Path


# A helper function to add clear breaks in logging


Plot_Version="ANv5"

def log_section_start(section_name):
    logging.info("=" * 50)  # Print 50 '=' characters as a separator                                                                                                                                                                                                                                                                                                                        
    logging.info(f"STARTING SECTION: {section_name}")
    logging.info("=" * 50)

def log_subsection_start(section_name):
    logging.info("-" * 50)  # Print 50 '=' characters as a separator                                                                                                                                                                                                                                                                                                                         
    logging.info(f"STARTING SUBSECTION: {section_name}")
    logging.info("-" * 50)
    


logging.basicConfig(
    level=logging.INFO,
    format='HaddJob:  %(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler(f'process_hadd.log'),  # Log to a file                                                                                                                                      
                                                                                                                                                                                                                   
        logging.StreamHandler()  # Log to console (screen)
    ]
       
)
eras = ["2016preVFP", "2016postVFP", "2017", "2018"]
base_path = "/data6/Users/jalmond/SKFlatOutput/Run2UltraLegacy_v3/HNL_ControlRegion_Plotter"

commands_template = [

    
    ("Conv", [
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ZGToLLG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ZGToLLG_PtG_130.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_DYJets10to50_MG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_DYJets_MG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_MG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_01J_PtG_130.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_01J_PtG_300.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_01J_PtG_500.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGJJToLNu.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_TTG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WWG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WZG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_TG.root",
    ]),

    ("ZG", [
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ZGToLLG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ZGToLLG_PtG_130.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_DYJets10to50_MG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_DYJets_MG.root",
        ]),
    ("CF", "MultiLepton__RunCF__/DATA/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_*"),
    ("Fake", "MultiLepton__RunFake__/DATA/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_*"),
    ("Data", "MultiLepton__/DATA/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_*"),

    ("WG", [
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_MG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_01J_PtG_130.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_01J_PtG_300.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGToLNuG_01J_PtG_500.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_DileptonBDT_WGJJToLNu.root",
    ]),
    ("Other_Conv", [
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_TTG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WWG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WZG.root",
        "MultiLepton__RunConv__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_TG.root",
    ]),

    ("ZZ", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto4e.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto2e2tau.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto2e2mu.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto4mu.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto2mu2tau.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluHToZZTo4L.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_VBF_HToZZTo4L.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto4tau.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ZZTo4L_powheg.root",
     ]),

    ("ggZZ", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto4e.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto2e2tau.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto2e2mu.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto4mu.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto2mu2tau.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluHToZZTo4L.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_VBF_HToZZTo4L.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_GluGluToZZto4tau.root"
    ]),
    ("qqZZ", ["MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ZZTo4L_powheg.root"]),
    ("TTV", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ttZToLLNuNu.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ttWToLNu.root"
    ]),
    ("WW", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WpWp_EWK.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WpWp_QCD.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WWTo2L2Nu_DS.root"
    ]),
    
    ("Other", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_tZq.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_TTTT.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ttHToNonbb.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_VHToNonbb.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_TTZZ.root"
    ]),
    ("Extra", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_tHq.root",
    ]),
    ("VVV", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WZZ.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_ZZZ.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WWZ.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WWW.root"
    ]),
    ("WZ", [
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WZ_EWK.root",
        "MultiLepton__RunPrompt__/HNL_ControlRegion_Plotter_SkimTree_HNMultiLepBDT_WZTo3LNu_mllmin4p0_powheg.root"
    ])
]




for era in eras:
    era_path = os.path.join(base_path, era)

    if os.path.exists(era):
        if os.path.isdir(era):  # Check if it's a directory
            logging.info(f"Removing old directory {era}")
            shutil.rmtree(era)  # Removes a directory and all its contents

    os.makedirs(era, exist_ok=True)  # Ensure output directory exists


    log_section_start(f"Checking {era}")
    for tag, input_files in commands_template:
        
        output_file = f"{era}/HNL_ControlRegion_Plotter_{tag}.root"
        log_subsection_start(f"Checking inputs to {output_file}")
        
        # If the input is a wildcard pattern (string), expand with full path                                                                                                                                        
        if not isinstance(input_files, str):
            # Check if files exist if input_files is not a wildcard                                                                                                                                                 
            input_paths = [os.path.join(era_path, f) for f in input_files]
            # Ensure all files exist                                                                                                                                                                                
            missing_files = [f for f in input_paths if not os.path.isfile(f)]
            if missing_files:
                logging.warning(f"Error: The following files do not exist: {', '.join(missing_files)}")
                sys.exit(1)

    
    log_section_start(f"Merging {era}")
   
    for tag, input_files in commands_template:
       
        output_file = f"{era}/HNL_ControlRegion_Plotter_{tag}.root"
        log_subsection_start(f"Merging -->{output_file}")
        
        # If the input is a wildcard pattern (string), expand with full path
        if isinstance(input_files, str):
            input_path = os.path.join(base_path, era, input_files)
            cmd = f"hadd {output_file} {input_path}"
            # If the tag is 'Data', copy the files as well
            if tag == "Data":
                cmd_cp = f"cp {input_path} {era}/"
                logging.info(f"Running: {cmd_cp}")
                subprocess.run(cmd_cp, shell=True, check=True)
        else:

            # Check if files exist if input_files is not a wildcard
            input_paths = [os.path.join(era_path, f) for f in input_files]

            cmd = f"hadd {output_file} " + " ".join(input_paths)
            # If the tag is 'Data', copy the files as well
            if tag == "Data":
                cmd_cp = f"cp " + " ".join(input_paths) + f" {era}/"
                logging.info(f"Running: {cmd_cp}")
                subprocess.run(cmd_cp, shell=True, check=True)

        logging.info(f"Running: {cmd}")
        subprocess.run(cmd, shell=True, check=True)


# move outfiles to plot version directory

# Define target directory
target_dir = Path(Plot_Version)

# Delete if it already exists
if target_dir.exists() and target_dir.is_dir():
    shutil.rmtree(target_dir)

# Create the directory
target_dir.mkdir(parents=True, exist_ok=True)


# Move all directories starting with "20" into the new directory
for entry in os.listdir():
    if entry.startswith("20") and os.path.isdir(entry):
        shutil.move(entry, target_dir / entry)



        
