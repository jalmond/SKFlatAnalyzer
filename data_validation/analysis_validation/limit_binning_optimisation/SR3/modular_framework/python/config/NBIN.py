def get_nbins_for_region(cat):

    if "LowJet" in cat and "LTcut" in cat:
        return 5
    elif "LowJet" in cat and "GTcut" in cat:
        return 3
    elif "HighJet" in cat and "LTcut" in cat:
        return 6
    elif "HighJet" in cat and "GTcut" in cat:
        return 4
    
    raise ValueError(f"Unknown category: {cat}")

                
