import os 

Eras = ["2018"]#,"2017","2018"]
Version="Version11"
nmax="125"

for Era in Eras:
    os.system('python runAllBDTG_MultiBkg_FakeFull_BB.py -e ' + Era + ' -v '+ Version +'  -s "200" -m ' +nmax )
    os.system('python runAllBDTG_MultiBkg_Fake_BB.py     -e ' + Era + ' -v '+ Version +'  -s "200" -m ' +nmax )
    os.system('python runAllBDTG_MultiBkg_Conv_BB.py     -e ' + Era + ' -v '+ Version +'  -s "200" -m ' +nmax )
    os.system('python runAllBDTG_MultiBkg_CF_BB.py       -e ' + Era + ' -v '+ Version +'  -s "200" -m ' +nmax )
    os.system('python runAllBDTG_MultiBkg_CFTmp_BB.py       -e ' + Era + ' -v '+ Version +'  -s "200" -m ' +nmax )
