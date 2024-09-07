analyzer=HNL_ControlRegion_Plotter
rundir=HNL_ControlRegion_Plotter
runPATH=${SKFlat_WD}/runJobs/HNL/${rundir}/
sigpath=${SKFlat_WD}/runJobs/SampleLists/Signals/
mcpath=${SKFlat_WD}/runJobs/SampleLists/Bkg/
datapath=${SKFlat_WD}/runJobs/SampleLists/Data/

njobs=30
njobs_sig=2
njobs_data=200
nmax=500
skim=' '

if [[ $1 == "" ]]; then

    
    #SKFlat.py -a $analyzer  -i DoubleEG:G      -n 200    --nmax ${nmax}   -e  2016postVFP --skim SkimTree_DileptonBDT  --userflags SSMultiLep,RunCF &                                                
    #SKFlat.py -a $analyzer  -i ZGToLLG         -n 10        --nmax ${nmax}   -e 2016postVFP --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunConv&                                            
    #SKFlat.py -a $analyzer  -i DoubleEG:G       -n 100    --nmax ${nmax}   -e 2016postVFP --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &                                                   
    #SKFlat.py -a $analyzer  -i MuonEG:G       -n 100  --nmax ${nmax}   -e 2016postVFP  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &                                          
    #SKFlat.py -a $analyzer  -i DoubleEG:G     -n 100  --nmax ${nmax}   -e 2016postVFP  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &                                         
    
    
    #SKFlat.py -a $analyzer  -l $datapath/2016postVFP_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e 2016postVFP --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &
    #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EMu.txt      -n 100  --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &#
    #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_HNMultiLepBDT   --userflags SSMultiLep &
    #SKFlat.py -a $analyzer  -l $mcpath/PromptSS.txt             -n 20        --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
    SKFlat.py -a $analyzer  -i ZZTo4L_powheg         -n 200       --nmax ${nmax}   -e 2018  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
    SKFlat.py -a $analyzer  -i ZZTo4L_powheg         -n 200       --nmax ${nmax}   -e 2017  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
    SKFlat.py -a $analyzer  -i ZZTo4L_powheg         -n 200       --nmax ${nmax}   -e 2016postVFP  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
    SKFlat.py -a $analyzer  -i WZ_EWK         -n 200       --nmax ${nmax}   -e 2018  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
    SKFlat.py -a $analyzer  -i WZTo3LNu_mllmin4p0_powheg        -n 200       --nmax ${nmax}   -e 2018  --skim SkimTree_HNMultiLepBDT --userflags  SSMultiLep,RunPrompt &
    
    #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_MuMu.txt      -n 100  --nmax ${nmax}   -e ${i}  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunFake   &
    #SKFlat.py -a $analyzer  -i DoubleEG:B     -n 100    --nmax ${nmax}   -e 2017  --skim SkimTree_HNMultiLepBDT  --userflags SSMultiLep,RunCF   &
    #SKFlat.py -a $analyzer  -l $datapath/${i}_DiLepton_EE.txt      -n 100    --nmax ${nmax}   -e ${i} --skim SkimTree_DileptonBDT  --userflags SSMultiLep,RunCF &
    
    
fi
