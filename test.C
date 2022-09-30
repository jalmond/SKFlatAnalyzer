R__LOAD_LIBRARY(/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/lhapdf/6.2.3/lib/libLHAPDF.so)
R__LOAD_LIBRARY(/cvmfs/cms.cern.ch/slc7_amd64_gcc900/cms/cmssw/CMSSW_11_3_0/external/slc7_amd64_gcc900/lib/libTMVA.so)


void test(TString era, bool isdata, TString sample, TString flag){ //test("2017",false,"DYJets","DEBUG")

  HNL_ControlRegionPlotter m;

  m.SetTreeName("recoTree/SKFlat");

  m.LogEvery = 1000;
  m.IsDATA = isdata;
  if(isdata) m.DataStream = sample;
  else{
    m.MCSample = sample;
    m.xsec = 1;
    m.sumSign = 1;
    m.sumW = 1;
    m.IsFastSim = false;
  }
  m.SetEra(era);
  m.Userflags = {flag};
  if(era=="2017" && isdata && sample=="DoubleEG"){
    m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_3.root");
  }
  else if(era=="2017" && !isdata && sample=="DYJets"){
    m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/MC_SkimTree_HNMultiLep/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/2022_08_23_045258/SKFlatNtuple_2017_MC_6.root");
  }
  m.SetOutfilePath("hists.root");
  m.Init();
  m.initializeAnalyzer();
  m.initializeAnalyzerTools();
  m.SwitchToTempDir();
  m.Loop();

  m.WriteHist();

}
