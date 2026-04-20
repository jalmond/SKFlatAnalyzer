from python.config.base_config import *

Bin_NBKG_REQ = 1.0
Bin_NBKG_REQ_Tight = 0.5
Bin_BKG_RelUnc = 0.3

TAG="Full_Scan"
# Rebuild tag AFTER override


LOG_TAG = build_log_tag(
    RunGlobalSig,
    NBinScan,
    USE_FAKE_FIX,
    RUN_Z_NO_UNC,
    Bin_NBKG_REQ,
    Bin_NBKG_REQ_Tight,
    Bin_BKG_RelUnc)

    
LOG_TAG = build_log_tag()
