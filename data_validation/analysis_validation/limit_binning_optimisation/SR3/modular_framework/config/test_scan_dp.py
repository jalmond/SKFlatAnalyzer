from config.base_config import *

DPScan=True

MASSES = [
    "400",
    "2000"
]

NBINS_TO_SCAN=[3]

Bin_NBKG_REQ = 1.0
Bin_NBKG_REQ_Tight = 0.5
Bin_BKG_RelUnc = 0.3

TAG="TestRun_DP"

# Rebuild tag AFTER override
LOG_TAG = build_log_tag()
