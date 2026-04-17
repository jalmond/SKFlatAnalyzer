from config.base_config import *

MASSES = [
    "400",
    "800",
    "2000"
]


Bin_NBKG_REQ = 1.0
Bin_NBKG_REQ_Tight = 0.5
Bin_BKG_RelUnc = 0.3

TAG="TestRun_Parallel"

# Rebuild tag AFTER override
LOG_TAG = build_log_tag()
