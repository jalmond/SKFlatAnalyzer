#python RunBatch_MinNLO.py  -e 2016preVFP -n 100 -s Config/RunEff_Split_NNLO.py -v Version6_split_NNLO 
#python RunBatch_MinNLO.py  -e 2016postVFP -n 100 -s Config/RunEff_Split_NNLO.py -v Version6_split_NNLO
#python RunBatch_MinNLO.py  -e 2017 -n 100 -s Config/RunEff_Split_NNLO.py -v Version6_split_NNLO
python RunBatch_MinNLO.py  -e 2018 -n 100 -s Config/RunEff_Split_NNLO.py -v Version7_split_NNLO
python RunBatch_MinNLO.py  -e 2018 -n 100 -s Config/RunEff_Split_NNLO_corr.py -v Version7corr_split_NNLO
