import os 

Eras = ["2018"]#, "2017", "2018"]
Version="Version11"
nmax="200"
Seed="200"

for Era in Eras:
    os.system('python runAllBDTG_MultiBkg_Fake_EC.py     -e '+Era+' -v '+Version+' -s '+Seed+' -m '+nmax)
    os.system('python runAllBDTG_MultiBkg_Conv_EC.py     -e '+Era+' -v '+Version+' -s '+Seed+' -m '+nmax)
    os.system('python runAllBDTG_MultiBkg_CF_EC.py       -e '+Era+' -v '+Version+' -s '+Seed+' -m '+nmax)
    os.system('python runAllBDTG_MultiBkg_CFTmp_EC.py       -e '+Era+' -v '+Version+' -s '+Seed+' -m '+nmax)
