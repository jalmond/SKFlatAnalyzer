Follow this order

1. Place MakeInput.py at your SKFlatOutput/HNL_SignalRegionPlotter and run it.
--> This will make basic input root files.
2. Go to your Combine directory. Copy or link MyModel.py to CombineTool/CMSSW_10_2_13/python/HiggsAnalysis/CombinedLimit then scram.
--> Now we have the proper physics model where the SSWW signal POI is the squared DYVBF signal POI.
3. Make your own Combine working directory and place card_skeleton*.txt and MakeDataCard.py in it.
Open MakeDataCard.py and input your path to MakeWorkspace.py. Then run it.
--> This will make actual datacards in another directory.
4. Go to datacards' directory and run MakeWorkspace.py. (step 3 and 4 could be merged later...)
--> Now you get the final input root files containing information of fitting and physics model.
5. Now you can run the Combine Tool by yourself.
If you want to utilize batch system, you can use create-batch.py.
Run MakeRunList.py and then feed the output into the create-batch.py.
6. ReadLimitFromTree.py will collect the outputs from the batch system and give you the list of limit values.
7. DrawLimits.C will draw the limit plots for you.
root -l;
.L DrawLimits.C;
DrawLimits("2017","MuMu");
