ERAS = ["2016preVFP","2016postVFP","2017","2018"]
FLAVOURS = ["MuMu","EE","EMu"]
MASSES = [
	"400","450","500",
        "600","700","800","900",
	"1000","1500","2000"
]
NBINS_TO_SCAN=[1,2,3,4,5,6,7]
FAKE_FLOOR = 0.15 * 0.645
USE_FAKE_FIX = True   
RUN_Z_NO_UNC=True
RUN_REF=True
RUN_SCANS=True

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
        
    # Nbins
    if len(NBINS_TO_SCAN) > 1:
        parts.append(f"NBin{min(NBINS_TO_SCAN)}to{max(NBINS_TO_SCAN)}")
    else:
        parts.append(f"NBin{NBINS_TO_SCAN[0]}")

    return "_".join(parts)


LOG_TAG = build_log_tag()
