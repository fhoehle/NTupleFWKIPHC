
//////////////////////////////////////////////////////////
//
// Example of TSelector implementation to do a Monte Carlo
// generation using Pythia8.
// See tutorials/proof/runProof.C, option "pythia8", for an
// example of how to run this selector.
//
//////////////////////////////////////////////////////////

#ifndef ProofSelectorMyCutFlow_h
#define ProofSelectorMyCutFlow_h

#include <TSelector.h>
#include <TTree.h>
#include <TFile.h>
#include <TProofOutputFile.h>
#include <TRandom.h>

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

class ProofSelectorMyCutFlow : public TSelector {
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
  
  
  bool applyDYscale ;
  bool applyFakescale ;
  
  //Here define Scale Factors
  //SF_trigger applied for mumu
  double SF_trig_mu ;  
  double SF_trig_emu;  
  double SF_trig_ee ;  
  
  double SF_trig_mu_error ;  
  double SF_trig_emu_error;  
  double SF_trig_ee_error ;  
  
  double SF_e  ;  
  double SF_mu ; 
    
  double SF_BranchingRatio_ll ; 
  double SF_BranchingRatio_lj ; 
  double SF_BranchingRatio_had ; 
    
  double SF_DY_ee ;
  double SF_DY_mm ;
  double SF_DY_em ;
  
  double  SF_DY_Njets_ee ;
  double  SF_DY_Njets_mm ;
  double  SF_DY_Njets_em ;
  
  double SF_met_mumu;
  double SF_met_emu;
  double SF_met_ee;

  double SF_met_mumu_error;
  double SF_met_emu_error;
  double SF_met_ee_error;
   
  
  bool PUup;
  bool PUdown;
  
  bool doLinearity;
  
   
  std::vector<double> vSF_DY_ee ;
  std::vector<double> vSF_DY_mm ;
  std::vector<double> vSF_DY_em ;
   
  std::vector<double> vSF_DY_ee_error ;
  std::vector<double> vSF_DY_mm_error ;
  std::vector<double> vSF_DY_em_error ;
  
  std::vector<double> vSF_FakeBck_ee; 
  std::vector<double> vSF_FakeBck_mm; 
  std::vector<double> vSF_FakeBck_em; 
  
  
  std::vector<double> vSF_FakeBck_ee_error; 
  std::vector<double> vSF_FakeBck_mm_error; 
  std::vector<double> vSF_FakeBck_em_error; 
  
  double sumSFlept_ee;
  double sumSFlept_mumu;
  double sumSFlept_emu;
  
  double nEvents_ee;
  double nEvents_mumu;
  double nEvents_emu;
  
   
   
  double SF_Wjets_ee;
  double SF_Wjets_mm;
  double SF_Wjets_em;
  
  double SF_QCD_ee;
  double SF_QCD_mm;
  double SF_QCD_em;
  
  double scaleElec; // 1 to switch off
  double resolElec; // 0 to switch off
  
  
  bool ApplyLeptonSF;
  
  
  int ITypeMC ;
  int ICut    ;  
  
  
  //For PLR plots
  PLRPlotsFurnisher plotsFurnisher;
  
  
  
  
  TH1F* fHist;
  
  std::vector<TH1F> CutFlow_mumu;
  std::vector<TH1F> CutFlow_ee;
  std::vector<TH1F> CutFlow_emu;
  std::vector<TH1F> ErrCutFlow_mumu;
  std::vector<TH1F> ErrCutFlow_ee;
  std::vector<TH1F> ErrCutFlow_emu;
  
  TH1D* SFtrigger;     
  
  TH1D* SFDY;   
  
  TH1D* SFWjets;   
  
  TH1D* SFQCD;   
  
  TH1D* SFLeptons;   
  
  //------------------------------------
  //Definition of the various histograms
  //------------------------------------
  int nbins ;
  float minx;
  float maxx;
  HistoManager MyhistoManager;
  
  
  std::map<TString, int> initMCevents;
  std::map<TString, int> skimMCevents;
  std::vector<TH1F> MyHistos_mumu;
  std::vector<TH1F> MyHistos1_mumu;
  std::vector<TH1F> HInvM_ll_pair_mumu;
  std::vector<TH1F> HInvM_ll_pair_mumu_sup15;
  std::vector<TH1F> HInvM_ll_pair_mumu_inf15;
  std::vector<TH1F> HInvM_ll_afterSelButInvMNJet_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMNJet_AfterMET_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMDilepton_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvM0Btag_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvM1Btag_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvM2Btag_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup0Btag_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup1Btag_mumu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup2Btag_mumu;
  std::vector<TH1F> MyHistos_ee;
  std::vector<TH1F> MyHistos1_ee;
  std::vector<TH1F> HInvM_ll_pair_ee;
  std::vector<TH1F> HInvM_ll_pair_ee_sup15;
  std::vector<TH1F> HInvM_ll_pair_ee_inf15;
  std::vector<TH1F> HInvM_ll_afterSelButInvMNJet_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvMNJet_AfterMET_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvMDilepton_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvM0Btag_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvM1Btag_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvM2Btag_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup0Btag_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup1Btag_ee;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup2Btag_ee;
  std::vector<TH1F> HInvM_emu_pair;
  std::vector<TH1F> HInvM_emu_pairzoom;
  std::vector<TH1F> HInvM_emu_pair_afterbtag1;
  std::vector<TH1F> HInvM_emu_pair_afterbtag2;
  std::vector<TH1F> HInvM_emu_pair_aftermetbtag1;
  std::vector<TH1F> HInvM_emu_pair_aftermetbtag2;
  std::vector<TH1F> HInvM_ll_afterSelButInvM_emu;  
  std::vector<TH1F> HInvM_ll_afterSelButInvMDilepton_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMNJet_AfterMET_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMNJet_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvM0Btag_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvM1Btag_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvM2Btag_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup0Btag_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup1Btag_emu;
  std::vector<TH1F> HInvM_ll_afterSelButInvMSup2Btag_emu;
  std::vector<TH1F> HNjets_emu;
  std::vector<TH1F> HMet_emu;
  std::vector<TH1F> HNBjets_emu;
  std::vector<TH1F> HPtJet1_emu;
  std::vector<TH1F> HPtJet2_emu;
  std::vector<TH1F> HPtLpt1_emu;
  std::vector<TH1F> HPtLpt2_emu;
  std::vector<TH1F> HNVxP_emu;
  std::vector<TH1F> HNVxP_emu_aftertrigger;
  std::vector<TH1F> HInvM_emu_pair_Zemu;
  
  std::vector<TH1F> HInvM_emu_pair_aftermetcut;
  std::vector<TH1F> HNjets_emu_aftermetcut;
  std::vector<TH1F> HNBjets_emu_aftermetcut;
  std::vector<TH1F> HMet_emu_aftermetcut;
  std::vector<TH1F> HPtJet1_emu_aftermetcut;
  std::vector<TH1F> HPtJet2_emu_aftermetcut;
  std::vector<TH1F> HPtLpt1_emu_aftermetcut;
  std::vector<TH1F> HPtLpt2_emu_aftermetcut;
  std::vector<TH1F> HNVxP_emu_aftermetcut; 
  
  std::vector<TH1F> HInvM_emu_pair_afterjetcut;
  std::vector<TH1F> HNjets_emu_afterjetcut;
  std::vector<TH1F> HNBjets_emu_afterjetcut;
  std::vector<TH1F> HMet_emu_afterjetcut;
  std::vector<TH1F> HPtJet1_emu_afterjetcut;
  std::vector<TH1F> HPtJet2_emu_afterjetcut;
  std::vector<TH1F> HPtLpt1_emu_afterjetcut;
  std::vector<TH1F> HPtLpt2_emu_afterjetcut;
  
  std::vector<TH1F> HDeltaPhiLMet_emu;
  std::vector<TH1F> HDeltaPhiLMet_emu_aftermetcut;
  std::vector<TH1F> HDeltaPhiLMet_emu_afterjetcut;
  
  std::vector<TH1F> HHT_emu;
  
  
  std::vector<TH1F> LeptonIso_e;
  std::vector<TH1F> LeptonIso_mu;
  
  std::vector<TH1F> HInvM_mumu_pair;
  std::vector<TH1F> HInvM_mumu_pairzoom;
  std::vector<TH1F> HInvM_mumu_pair_afterbtag1;
  std::vector<TH1F> HInvM_mumu_pair_afterbtag2;
  std::vector<TH1F> HInvM_mumu_pair_aftermetbtag1;
  std::vector<TH1F> HInvM_mumu_pair_aftermetbtag2;
  std::vector<TH1F> HNjets_mumu;
  std::vector<TH1F> HMet_mumu;
  std::vector<TH1F> HNBjets_mumu;
  std::vector<TH1F> HPtJet1_mumu;
  std::vector<TH1F> HPtJet2_mumu;
  std::vector<TH1F> HPtMu1_mumu;
  std::vector<TH1F> HPtMu2_mumu;
  std::vector<TH1F> HNVxP_mumu;
  std::vector<TH1F> HNVxP_mumu_aftertrigger;
  
  std::vector<TH1F> HInvM_mumu_pair_aftermetcut;
  std::vector<TH1F> HNjets_mumu_aftermetcut;
  std::vector<TH1F> HMet_mumu_aftermetcut;
  std::vector<TH1F> HNBjets_mumu_aftermetcut;
  std::vector<TH1F> HPtJet1_mumu_aftermetcut;
  std::vector<TH1F> HPtJet2_mumu_aftermetcut;
  std::vector<TH1F> HPtMu1_mumu_aftermetcut;
  std::vector<TH1F> HPtMu2_mumu_aftermetcut;
  std::vector<TH1F> HNVxP_mumu_aftermetcut;
  
  std::vector<TH1F> HInvM_mumu_pair_afterjetcut;
  std::vector<TH1F> HNjets_mumu_afterjetcut;
  std::vector<TH1F> HMet_mumu_afterjetcut;
  std::vector<TH1F> HNBjets_mumu_afterjetcut;
  std::vector<TH1F> HPtJet1_mumu_afterjetcut;
  std::vector<TH1F> HPtJet2_mumu_afterjetcut;
  std::vector<TH1F> HPtMu1_mumu_afterjetcut;
  std::vector<TH1F> HPtMu2_mumu_afterjetcut;
  
  std::vector<TH1F> HInvM_mumu_pair_afterveto;
  std::vector<TH1F> HNjets_mumu_afterveto;
  std::vector<TH1F> HMet_mumu_afterveto;
  std::vector<TH1F> HNBjets_mumu_afterveto;
  std::vector<TH1F> HPtJet1_mumu_afterveto;
  std::vector<TH1F> HPtJet2_mumu_afterveto;
  std::vector<TH1F> HPtMu1_mumu_afterveto;
  std::vector<TH1F> HPtMu2_mumu_afterveto;
  
  std::vector<TH1F> HHT_mumu;
  
  std::vector<TH1F> HDeltaPhiLMet_mumu;
  std::vector<TH1F> HDeltaPhiLMet_mumu_aftermetcut;
  std::vector<TH1F> HDeltaPhiLMet_mumu_afterjetcut;
  std::vector<TH1F> HDeltaPhiLMet_mumu_afterveto;
  
  std::vector<TH1F> HInvM_ee_pair;
  std::vector<TH1F> HInvM_ee_pairzoom;
  std::vector<TH1F> HInvM_ee_pair_afterbtag1;
  std::vector<TH1F> HInvM_ee_pair_afterbtag2;
  std::vector<TH1F> HInvM_ee_pair_aftermetbtag1;
  std::vector<TH1F> HInvM_ee_pair_aftermetbtag2;
  std::vector<TH1F> HNjets_ee;
  std::vector<TH1F> HMet_ee;
  std::vector<TH1F> HNBjets_ee;
  std::vector<TH1F> HPtJet1_ee;
  std::vector<TH1F> HPtJet2_ee;
  std::vector<TH1F> HPtEl1_ee;
  std::vector<TH1F> HPtEl2_ee;
  std::vector<TH1F> HNVxP_ee;
  std::vector<TH1F> HNVxP_ee_aftertrigger;
  
  std::vector<TH1F> HInvM_ee_pair_aftermetcut;
  std::vector<TH1F> HNjets_ee_aftermetcut;
  std::vector<TH1F> HMet_ee_aftermetcut;
  std::vector<TH1F> HNBjets_ee_aftermetcut;
  std::vector<TH1F> HPtJet1_ee_aftermetcut;
  std::vector<TH1F> HPtJet2_ee_aftermetcut;
  std::vector<TH1F> HPtEl1_ee_aftermetcut;
  std::vector<TH1F> HPtEl2_ee_aftermetcut;
  std::vector<TH1F> HNVxP_ee_aftermetcut;
  
  std::vector<TH1F> HInvM_ee_pair_afterjetcut;
  std::vector<TH1F> HNjets_ee_afterjetcut;
  std::vector<TH1F> HMet_ee_afterjetcut;
  std::vector<TH1F> HNBjets_ee_afterjetcut;
  std::vector<TH1F> HPtJet1_ee_afterjetcut;
  std::vector<TH1F> HPtJet2_ee_afterjetcut;
  std::vector<TH1F> HPtEl1_ee_afterjetcut;
  std::vector<TH1F> HPtEl2_ee_afterjetcut;
  
  std::vector<TH1F> HInvM_ee_pair_afterveto;
  std::vector<TH1F> HNjets_ee_afterveto;
  std::vector<TH1F> HMet_ee_afterveto;
  std::vector<TH1F> HNBjets_ee_afterveto;
  std::vector<TH1F> HPtJet1_ee_afterveto;
  std::vector<TH1F> HPtJet2_ee_afterveto;
  std::vector<TH1F> HPtEl1_ee_afterveto;
  std::vector<TH1F> HPtEl2_ee_afterveto;
  
  std::vector<TH1F> HDeltaPhiLMet_ee;
  std::vector<TH1F> HDeltaPhiLMet_ee_aftermetcut;
  std::vector<TH1F> HDeltaPhiLMet_ee_afterjetcut;
  std::vector<TH1F> HDeltaPhiLMet_ee_afterveto;

  std::vector<TH1F> HMet_emu_afterbtag1;
  std::vector<TH1F> HMet_mumu_afterbtag1;
  std::vector<TH1F> HMet_ee_afterbtag1;
  std::vector<TH1F> HMet_emu_afterbtag2;
  std::vector<TH1F> HMet_mumu_afterbtag2;
  std::vector<TH1F> HMet_ee_afterbtag2;
  
  std::vector<TH1F> HHT_ee_aftermetbtag1;
  std::vector<TH1F> HHT_emu_aftermetbtag1;
  std::vector<TH1F> HHT_mumu_aftermetbtag1;
  
  std::vector<TH1F> HHT_ee_aftermetbtag2;
  std::vector<TH1F> HHT_emu_aftermetbtag2;
  std::vector<TH1F> HHT_mumu_aftermetbtag2;
  
  std::vector<TH1F> pileup;
  
  
  std::vector<TH1F> HDeltaPhi_ee_LeptonMET;
  std::vector<TH1F> HDeltaPhi_ee_DiLeptonMET;
  std::vector<TH1F> HDeltaPhi_ee_ClosestDiLeptonJets;
  std::vector<TH1F> HDeltaPhi_ee_DiLeptonJets;
  std::vector<TH1F> HDeltaPhi_ee_JetsMET;
  std::vector<TH1F> HDeltaPhi_ee_ClosestJetsMET;
  std::vector<TH1F> HDeltaPhi_ee_PtJetsInMassP;
  std::vector<TH1F> HDeltaPhi_ee_PtJetsOutMassP;
  
  
  std::vector<TH1F> HDeltaPhi_emu_LeptonMET;
  std::vector<TH1F> HDeltaPhi_emu_DiLeptonMET;
  std::vector<TH1F> HDeltaPhi_emu_ClosestDiLeptonJets;
  std::vector<TH1F> HDeltaPhi_emu_DiLeptonJets;
  std::vector<TH1F> HDeltaPhi_emu_JetsMET;
  std::vector<TH1F> HDeltaPhi_emu_ClosestJetsMET;
  std::vector<TH1F> HDeltaPhi_emu_PtJetsInMassP;
  std::vector<TH1F> HDeltaPhi_emu_PtJetsOutMassP;
  
  std::vector<TH1F> HDeltaPhi_mumu_LeptonMET;
  std::vector<TH1F> HDeltaPhi_mumu_DiLeptonMET;
  std::vector<TH1F> HDeltaPhi_mumu_DiLeptonJets;
  std::vector<TH1F> HDeltaPhi_mumu_ClosestDiLeptonJets;
  std::vector<TH1F> HDeltaPhi_mumu_JetsMET;
  std::vector<TH1F> HDeltaPhi_mumu_ClosestJetsMET;
  std::vector<TH1F> HDeltaPhi_mumu_PtJetsInMassP;
  std::vector<TH1F> HDeltaPhi_mumu_PtJetsOutMassP;
  
  std::vector<TH2D> HDileptPt_vs_met_mumu;
  std::vector<TH2D> HDileptPt_vs_met_emu;
  std::vector<TH2D> HDileptPt_vs_met_ee;
  
  std::vector<TH1F> HDileptPt_mumu;
  std::vector<TH1F> HDileptPt_ee;
  std::vector<TH1F> HDileptPt_emu;
  
  std::vector<TH2D> HInvM_ee_pair_vs_NJet_METinf10;
  std::vector<TH2D> HInvM_ee_pair_vs_NBJet_METinf10;
  std::vector<TH2D> HInvM_ee_pair_vs_MET_NJet0;
  std::vector<TH2D> HInvM_ee_pair_vs_MET_NBJet0;
  std::vector<TH2D> HInvM_ee_pair_vs_PU;
  std::vector<TH3D> HInvM_ee_pair_vs_MET_NJet;
  
  std::vector<TH2D> HInvM_mumu_pair_vs_NJet_METinf10;
  std::vector<TH2D> HInvM_mumu_pair_vs_NBJet_METinf10;
  std::vector<TH2D> HInvM_mumu_pair_vs_MET_NJet0;
  std::vector<TH2D> HInvM_mumu_pair_vs_MET_NBJet0;
  std::vector<TH2D> HInvM_mumu_pair_vs_PU;
  std::vector<TH3D> HInvM_mumu_pair_vs_MET_NJet;
  
  
  std::vector<TH1F> HNjets_ee_aftermetcutbutjetcut;
  std::vector<TH1F> HNjets_mumu_aftermetcutbutjetcut;
  std::vector<TH1F> HNjets_emu_aftermetcutbutjetcut;
  
  
  std::vector<TH1F> HInvM_ee_pair_EB;
  std::vector<TH1F> HInvM_ee_pair_EE;
  
  //------------------------------------
  //Background estimation methods
  //------------------------------------
  
  std::vector<TH1F> hemudata_emu;
  std::vector<TH1F> hlistemu_ee;
  std::vector<TH1F> hlistSR_ee;
  std::vector<TH1F> hlistCR_ee;
  std::vector<TH1F> hlistContCR_ee;
  std::vector<TH1F> hlistemu_mumu;
  std::vector<TH1F> hlistSR_mumu;
  std::vector<TH1F> hlistCR_mumu;
  std::vector<TH1F> hlistContCR_mumu;
  std::vector<TH1F> NjetsCR_inMZ_mumu;
  std::vector<TH1F> NjetsSR_inMZ_mumu;
  std::vector<TH1F> NjetsCR_outMZ_mumu;
  std::vector<TH1F> NjetsSR_outMZ_mumu;
  std::vector<TH1F> DYMassCR_mumu;
  std::vector<TH1F> DYMassSR_mumu;
  std::vector<TH1F> NjetsCR_inMZ_ee;
  std::vector<TH1F> NjetsSR_inMZ_ee;
  std::vector<TH1F> NjetsCR_outMZ_ee;
  std::vector<TH1F> NjetsSR_outMZ_ee;
  std::vector<TH1F> DYMassCR_ee;
  std::vector<TH1F> DYMassSR_ee;
  
  
  //------------------------------------
  // for PileUP reweighting
  //------------------------------------
  PUWeighting  thePUReweighter;
  
  
  string datasetName ;
  
  
  //------------------------------------
  //definition of member functions
  //------------------------------------
  ProofSelectorMyCutFlow();
  virtual ~ProofSelectorMyCutFlow();
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
  
  ClassDef(ProofSelectorMyCutFlow,0);
};

#endif
