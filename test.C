R__LOAD_LIBRARY(/cvmfs/cms.cern.ch/slc7_amd64_gcc900/external/lhapdf/6.2.3/lib/libLHAPDF.so)
R__LOAD_LIBRARY(/cvmfs/cms.cern.ch/slc7_amd64_gcc900/cms/cmssw/CMSSW_11_3_0/external/slc7_amd64_gcc900/lib/libTMVA.so)


void test(TString era, bool isdata, TString sample, TString flag){ //test("2017",false,"DYJets","DEBUG")

  //HNL_ControlRegionPlotter m;
  HNL_SignalRegionPlotter m;

  m.SetTreeName("recoTree/SKFlat");

  m.LogEvery = 1000;
  m.IsDATA = isdata;
  if(isdata) m.DataStream = sample;
  else{
    m.MCSample = sample;
    m.xsec = 6077.22;
    m.sumSign = 132089877.0;
    m.sumW = 3.336547209017e+12;
    m.IsFastSim = false;
  }
  m.SetEra(era);
  TObjArray* flags = flag.Tokenize(",");
  for(int i=0; i<flags->GetEntries(); i++) m.Userflags.push_back(((TObjString*)flags->At(i))->String()); //JH : https://root-forum.cern.ch/t/split-tstring-by-delimeter-in-root-c/18228
  if(era=="2017" && isdata && sample=="DoubleEG"){
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_3.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_4.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_5.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_6.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_7.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_8.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_9.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_10.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_11.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_12.root")) exit(EIO);
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/DATA_SkimTree_HNMultiLep/DoubleEG/periodE/2022_08_23_004058/SKFlatNtuple_2017_DATA_13.root")) exit(EIO);
  }
  else if(era=="2017" && !isdata && sample=="DYJets"){
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/MC_SkimTree_HNMultiLep/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/2022_10_09_041952/SKFlatNtuple_2017_MC_6.root")) exit(EIO);
  }
  else if(era=="2017" && !isdata && sample=="WZ"){
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/MC/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/220614_141057/0000/SKFlatNtuple_2017_MC_1.root")) exit(EIO);
  }
  else if(era=="2017" && !isdata && sample=="ZZ"){
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/MC/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/220614_141827/0000/SKFlatNtuple_2017_MC_1.root")) exit(EIO);
  }
  else if(era=="2017" && !isdata && sample=="DYTypeI"){
    if(!m.AddFile("/gv0/DATA/SKFlat/Run2UltraLegacy_v3/2017/MC/DYTypeIHeavyN-DFmix_Dilepton_MN500_TuneCP5_13TeV-amcatnlo-pythia8/220801_130040/0000/SKFlatNtuple_2017_MC_1.root")) exit(EIO);
  }
  m.SetOutfilePath("hists_"+sample+".root");
  m.Init();
  m.initializeAnalyzer();
  m.initializeAnalyzerTools();
  cout << "initializeAnalyzerTools();" << endl;
  m.SwitchToTempDir();
  cout << "SwitchToTempDir();" << endl;
  m.Loop();

  m.WriteHist();

}
