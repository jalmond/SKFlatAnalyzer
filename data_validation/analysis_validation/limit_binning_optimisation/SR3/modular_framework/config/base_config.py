MASSES = [
	"400","450","500",
        "600","700","800","900",
	"1000","1500","2000"
]


#### Fake correction
USE_FAKE_FIX = True   

RUN_Z_NO_UNC=True

Bin_NBKG_REQ = 1.0
Bin_NBKG_REQ_Tight = 0.5
Bin_BKG_RelUnc = 0.3

TAG = "Default"
NBinScan=False
RunGlobalSig=True

def build_log_tag():
    parts = []

    # Z treatment
    if RUN_Z_NO_UNC:
        parts.append("runZnoUnc")
    else:
        parts.append("runZwithUnc")

    # Fake treatment
    if USE_FAKE_FIX:
        parts.append("FakeFix")
    else:
        parts.append("NoFakeFix")
    if NBinScan:
        parts.append("VaryNBin")
    else:
        parts.append("NoVaryNBin")

    if RunGlobalSig:
        parts.append("GlobalSigScan")
    else:
        parts.append("NoGlobalSigScan")


    # ----------------------------------
    # Stat config (NEW)
    # ----------------------------------
    parts.append(
        f"StatNBkg{Bin_NBKG_REQ}_Tight{Bin_NBKG_REQ_Tight}_RelErr{Bin_BKG_RelUnc}"
    )

    return "_".join(parts)

