
//////////////////////////////////////////////////////////
//
// Example of TSelector implementation to do a Monte Carlo
// generation using Pythia8.
// See tutorials/proof/runProof.C, option "pythia8", for an
// example of how to run this selector.
//
//////////////////////////////////////////////////////////

#ifndef ProofSelectorTTBarTagAndProbe_h
#define ProofSelectorTTBarTagAndProbe_h

#include <TSelector.h>
#include <TTree.h>
#include <TFile.h>
#include <TProofOutputFile.h>

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
#include "../../Plots/interface/HistoManager.h"



#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
//#include "../../BckgdEstimation/interface/MMEstimation.h"
#include "../../Measurements/interface/PLRPlotsFurnisher.h"
#include "../../Tools/interface/PUWeighting.h"
#include "../../Tools/interface/LumiReweightingStandAlone.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TH3.h>
#include <TCanvas.h>

class TH1F;
class TBranch;
class TTree;

class AnalysisEnvironmentLoader;
class DiLeptonSelection;

class ProofSelectorTTBarTagAndProbe : public TSelector {
 public :
  
  // Specific members
  //Access to the tree and outputs
  TTree* fChain;
  TBranch* branch;
  TopTree::NTEvent* event;
  TFile            *fFile;
  TProofOutputFile *fProofFile; // For optimized merging of the ntuple
  //Pointer on results from xml file  reading
  AnalysisEnvironmentLoader* anaEL; 
  //Minimimal info
  vector<Dataset> datasets;
  Dataset* dataset;
  vector<string> CutName;
  vector<string> ChannelName;
  vector<string> VecChannelName;
  DiLeptonSelection sel; 
  float Luminosity;
  int verbosity;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType;
  //Info analysis macro specific 
  
  
  TH1D *  hPUData ;
  TH1F *  hPUMC   ; 
  
  reweight::LumiReWeighting *LumiWeights;
  float LumiError ;
  string PUWeightFileName;
  reweight::PoissonMeanShifter PShiftUp_;
  reweight::PoissonMeanShifter PShiftDown_;

  bool producePLRPlots ;
  
  
  // Here define the studied channel (ee/mumu/emu)
  //  string ChannelName  = "ee";  // "mumu", "ee", "emu"
  // on va tourner sur les 3 canaux en meme temps!!!
  
  bool IReweight             ;
  bool IReweight_puUp        ;
  bool IReweight_puDown      ;
  bool IDYestimateWithMetCut ;
  
  //Here define Scale Factors
  //SF_trigger applied for mumu
  double SF_trig_mu ;  
  double SF_trig_emu;  
  double SF_trig_ee ;  
  
  double SF_e  ;  
  double SF_mu ; 
    
  double SF_BranchingRatio_ll ; 
  double SF_BranchingRatio_lj ; 
  double SF_BranchingRatio_had ; 
    
  double SF_DY_ee ;
  double SF_DY_mm ;
  double SF_DY_em ;
  
  double SF_Wjets_ee;
  double SF_Wjets_mm;
  double SF_Wjets_em;
  
  double SF_QCD_ee;
  double SF_QCD_mm;
  double SF_QCD_em;
  
  double scaleElec; // 1 to switch off
  double resolElec; // 0 to switch off
  
  
  
  
  int ITypeMC ;
  int ICut    ;  
  
  
  bool checkElecIso;
  
  
  TH1F* fHist;
  
  
  //------------------------------------
  //Definition of the various histograms
  //------------------------------------
  int nbins ;
  float minx;
  float maxx;
  HistoManager MyhistoManager;
  
 
  
  std::vector<TH1F> PtLepton_Probe;
  std::vector<TH1F> EtaLepton_Probe;
  std::vector<TH1F> PtLepton_Probe_passTight;
  std::vector<TH1F> EtaLepton_Probe_passTight;
  
  
  std::vector<TH1F> nVertex;
  
 
  //------------------------------------
  // for PileUP reweighting
  //------------------------------------
  PUWeighting  thePUReweighter;
  
  
  string datasetName ;
  
  
  //------------------------------------
  //definition of member functions
  //------------------------------------
  ProofSelectorTTBarTagAndProbe();
  virtual ~ProofSelectorTTBarTagAndProbe();
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  
  ClassDef(ProofSelectorTTBarTagAndProbe,0);
};

#endif
