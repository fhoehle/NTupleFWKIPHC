
#include <iostream>
#include <limits.h>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"
#include "../../Measurements/interface/PLRPlotsFurnisher.h"
#include "../../Tools/interface/PUWeighting.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>


///////////////////////////////////////////////////////////////////////////////////
//
// Utilisation: tourne sur les 3 types de channel etudies (ee/mumu/emu)
// Modifier le fichier de config ../../config/MyCutFlow.xml  (pour emu mettre METCutsEMU a 0 ou 20)
//
///////////////////////////////////////////////////////////////////////////////////



using namespace TopTree;
using namespace std;

int main (int argc, char* argv[])
{
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;
  
  //////////////////////
  //Global variables
  //////////////////////
  vector < Dataset > datasets;
  DiLeptonSelection sel; 
  float Luminosity = 0;
  float LumiError = 0;
  string PUWeightFileName;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;
  bool producePLRPlots = false;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  if(argc!=3){
	 cerr<<"usage: ./MyCutFlow.C file.xml file.root"<<endl;
	 //return(0);
  }
  string xmlFileName;
  if(argc!=3) xmlFileName = string ("../../config/MyCutFlow.xml");
  else xmlFileName = argv[1];
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  
  //Load for PU:
  sel.GeneratePUWeight(PUWeightFileName);
  
  //******************************************
  //Load Scale Factors for lepton efficiencies
  //******************************************
  sel.LoadElScaleFactors();
  sel.LoadMuScaleFactors();
  
  
  //Load Btag config:
   cout<<"Test2"<<endl;
   cout<<sel.GetSFBweight().Test()<<endl; 
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  cout<<"Test1"<<endl;
  //cout<<sel.GetSFBweight()<<endl;
  cout<<sel.GetSFBweight().GetHistoEffMCb()<<endl;
  cout<<sel.GetSFBweight().GetHistoEffMCb()->GetEntries()<<endl;

   cout<<"Test2"<<endl;
   cout<<sel.GetSFBweight().Test()<<endl; 

 
  TopTree::NTEvent * event = 0;
  //Selection table
  // 4 tables: ee - emu - mumu - allChannels
  SelectionTable selTable_allChannels(sel.GetCutList(),datasets, string("*"));
  SelectionTable selTable_ee(  sel.GetCutList(),datasets, string("ee")  );
  SelectionTable selTable_emu( sel.GetCutList(),datasets, string("emu") );
  SelectionTable selTable_mumu(sel.GetCutList(),datasets, string("mumu"));
  
  
  //CutFlow tables
  double TabFlow1[5][101][101]; //central values [channel][typemc][cuttype]
  double TabFlow2[5][101][101]; // errors
  //dataset weights
  double Dweight[101]; 
  for(int k0=0; k0<5; ++k0) {
    for(int k1=0; k1<101; ++k1) {
      Dweight[k1] = 0.;
      for(int k2=0; k2<101; ++k2) {
        TabFlow1[k0][k1][k2]=0.;
        TabFlow2[k0][k1][k2]=0.;
      }
    }
  } 
  


// Here define the studied channel (ee/mumu/emu)
//  string ChannelName  = "ee";  // "mumu", "ee", "emu"
  // on va tourner sur les 3 canaux en meme temps!!!


  bool IReweightWithVxP      = false;
  bool IReweight             = true;
  bool IReweight_Spring11    = false;
  bool IDYestimateWithMetCut = true;

// Here define Scale Factors
  //SF_trigger applied for mumu
  double SF_trig_mu = 0.969;  // for Run2011A
  double SF_trig_emu= 1.002;  // 
  double SF_trig_ee = 0.955;  // 
  
  //SF_eID applied for ee
//  double SF_e=0.965; // for 2010 Run
  double SF_e  =1.0;  // for 2011 Run
  double SF_mu =1.0; // for 2011 Run
  
  
/*
  double SF_DY_ee = 1.0;
  double SF_DY_mm = 1.0;
  double SF_DY_em = 1.0;
*/


  double SF_DY_ee = 1.95; // for Run2011A
  double SF_DY_mm = 2.04;
  double SF_DY_em = 1.14;

  
  double SF_BranchingRatio_ll = (0.108*9.)*(0.108*9.);
  double SF_BranchingRatio_lj = (0.108*9.)*(0.676*1.5);
  double SF_BranchingRatio_had = (0.676*1.5)*(0.676*1.5);
  

  double SF_Wjets_ee = 1.;
  double SF_Wjets_mm = 1.;
  double SF_Wjets_em = 1.;
  
  double SF_QCD_ee = 1.;
  double SF_QCD_mm = 1.;
  double SF_QCD_em = 1.;
  
  
  
  bool ApplyLeptonSF = true;
/*
  double SF_VxP1[100]= {0}; 
  double SF_VxP2[100]= {0}; 
  double SF_VxP3[100]= {0}; 
  if (!IReweightWithVxP ) {
     for (unsigned int k = 0; k < 100; k++) {
        SF_VxP1[k] = 1.;   
        SF_VxP2[k] = 1.;   
        SF_VxP3[k] = 1.;   
     }
  }
*/


  vector<string> CutName;  // for same flavour leptons
  CutName.push_back("Total");
  CutName.push_back("Trigger");
  CutName.push_back("$ll$ pair, M($ll$)$>$12 GeV$/c^2$ and VxP"); 
  CutName.push_back("M($ll$) $\\notin$ [76,106]");
  CutName.push_back("$E_T(jet1,2)$ $>$ 30 GeV");
  CutName.push_back("$\\not\\!\\!E_T$ $>$ 30 GeV");
  CutName.push_back("btag ($\\ge$1)"); 
  CutName.push_back("btag ($\\ge$2)"); 
  vector<string> CutName2;
  CutName2.push_back("Total");
  CutName2.push_back("Trigger");
  CutName2.push_back("$ll$ pair, M($ll$)$>$12 GeV$/c^2$ and VxP");
  CutName2.push_back(" - ");
  CutName2.push_back("$E_T(jet1,2)$ $>$ 30 GeV");
  if (sel.GetMETCut().first>0) CutName2.push_back("$\\not\\!\\!E_T$ $>$ 20 GeV");
  if (sel.GetMETCut().first<=0) CutName2.push_back(" - ");
  CutName2.push_back("btag ($\\ge$1)"); 
  CutName2.push_back("btag ($\\ge$2)"); 

  vector<string> VecChannelName;
  VecChannelName.push_back("mumu");
  VecChannelName.push_back("ee");
  VecChannelName.push_back("emu");

  int ITypeMC	  = -1;
  int ICut	  = -1;  

  //For PLR plots
  PLRPlotsFurnisher plotsFurnisher;
  plotsFurnisher.LoadSetup(anaEL,datasets,CutName,VecChannelName);
  if(plotsFurnisher.GetListOfSystematics().size()==0) producePLRPlots = false;
  if(producePLRPlots) plotsFurnisher.CreateHistos();

  //Book keeping of my histos
  HistoManager MyhistoManager;
  MyhistoManager.LoadDatasets(datasets);    
  MyhistoManager.LoadSelectionSteps(CutName);
  MyhistoManager.LoadChannels(VecChannelName);
  MyhistoManager.AddHisto("DilepMass", "DilepMass", "GeV", 80, 0., 200.);;	
  //////////////////////
  
  //modifdg
  std::vector<TH1F> MyHistos_mumu;
  std::vector<TH1F> MyHistos1_mumu;
  std::vector<TH1F> HInvM_ll_pair_mumu;
  std::vector<TH1F> HInvM_ll_pair_mumu_sup15;
  std::vector<TH1F> HInvM_ll_pair_mumu_inf15;
  std::vector<TH1F> MyHistos_ee;
  std::vector<TH1F> MyHistos1_ee;
  std::vector<TH1F> HInvM_ll_pair_ee;
  std::vector<TH1F> HInvM_ll_pair_ee_sup15;
  std::vector<TH1F> HInvM_ll_pair_ee_inf15;
  std::vector<TH1F> HInvM_emu_pair;
  std::vector<TH1F> HInvM_emu_pairzoom;
  std::vector<TH1F> HInvM_emu_pair_afterbtag1;
  std::vector<TH1F> HInvM_emu_pair_afterbtag2;
  std::vector<TH1F> HInvM_emu_pair_aftermetbtag1;
  std::vector<TH1F> HInvM_emu_pair_aftermetbtag2;
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

  //endmodifdg
  
  //Background estimation methods
  DYEstimation DYest_ee;
  DYEstimation DYest_mumu;
  //the following objects has to be internalized in the class itself
  std::vector<TH1F*> hlistemu_ee;
  std::vector<TH1F*> hlistSR_ee;
  std::vector<TH1F*> hlistCR_ee;
  std::vector<TH1F*> hlistContCR_ee;
  std::vector<TH1F*> hlistemu_mumu;
  std::vector<TH1F*> hlistSR_mumu;
  std::vector<TH1F*> hlistCR_mumu;
  std::vector<TH1F*> hlistContCR_mumu;
  
  std::vector<TH1F> pileup;

  int nbins = 200;
  float minx = 0.;
  float maxx = 350;
  
  TH1F* halllistemu_mumu     = new TH1F("halllistemu_mumu","halllistemu_mumu",nbins,minx,maxx);
  TH1F* hemudata_mumu        = new TH1F("hemudata_mumu","hemudata_mumu",nbins,minx,maxx);
  TH1F* halllistemu_ee       = new TH1F("halllistemu_ee","halllistemu_ee",nbins,minx,maxx);
  TH1F* hemudata_ee          = new TH1F("hemudata_ee","hemudata_ee",nbins,minx,maxx);

  TH1F* halllistSR_ee        = new TH1F("halllistSR_ee","halllistSR_ee",nbins,minx,maxx);
  TH1F* halllistCR_ee        = new TH1F("halllistCR_ee","halllistCR_ee",nbins,minx,maxx);
  TH1F* halllistContCR_ee    = new TH1F("halllistContCR_ee","halllistContCR_ee",nbins,minx,maxx);
  TH1F* hDataSR_ee           = new TH1F("hDataSR_ee","hDataSR_ee",nbins,minx,maxx);
  TH1F* hDataCR_ee           = new TH1F("hDataCR_ee","hDataCR_ee",nbins,minx,maxx);
  
  TH1F* hemudata_emu         = new TH1F("hemudata_emu","hemudata_emu",nbins,minx,maxx);

  TH1F* NjetsDataCR_ee       = new TH1F("NjetsDataCR_ee","NjetsDataCR_ee",5,-0.5,4.5);
  TH1F* NjetsDataSR_ee       = new TH1F("NjetsDataSR_ee","NjetsDataSR_ee",5,-0.5,4.5);
  TH1F* NjetsMCCR_ee         = new TH1F("NjetsMCCR_ee","NjetsMCCR_ee",5,-0.5,4.5);
  TH1F* NjetsMCSR_ee         = new TH1F("NjetsMCSR_ee","NjetsMCSR_ee",5,-0.5,4.5);

  TH1F* halllistSR_mumu      = new TH1F("halllistSR_mumu","halllistSR_mumu",nbins,minx,maxx);
  TH1F* halllistCR_mumu      = new TH1F("halllistCR_mumu","halllistCR_mumu",nbins,minx,maxx);
  TH1F* halllistContCR_mumu  = new TH1F("halllistContCR_mumu","halllistContCR_mumu",nbins,minx,maxx);
  TH1F* hDataSR_mumu         = new TH1F("hDataSR_mumu","hDataSR_mumu",nbins,minx,maxx);
  TH1F* hDataCR_mumu         = new TH1F("hDataCR_mumu","hDataCR_mumu",nbins,minx,maxx);
  
  TH1F* NjetsDataCR_mumu     = new TH1F("NjetsDataCR_mumu","NjetsDataCR_mumu",5,-0.5,4.5);
  TH1F* NjetsDataSR_mumu     = new TH1F("NjetsDataSR_mumu","NjetsDataSR_mumu",5,-0.5,4.5);
  TH1F* NjetsMCCR_mumu       = new TH1F("NjetsMCCR_mumu","NjetsMCCR_mumu",5,-0.5,4.5);
  TH1F* NjetsMCSR_mumu       = new TH1F("NjetsMCSR_mumu","NjetsMCSR_mumu",5,-0.5,4.5);

  TH1F* DYMassDataCR_mumu     = new TH1F("DYMassDataCR_mumu","DYMassDataCR_mumu",nbins,minx,maxx);
  TH1F* DYMassDataSR_mumu     = new TH1F("DYMassDataSR_mumu","DYMassDataSR_mumu",nbins,minx,maxx);
  TH1F* DYMassMCCR_mumu       = new TH1F("DYMassMCCR_mumu","DYMassMCCR_mumu",nbins,minx,maxx);
  TH1F* DYMassMCSR_mumu       = new TH1F("DYMassMCSR_mumu","DYMassMCSR_mumu",nbins,minx,maxx);
  
  TH1F* DYMassDataCR_ee     = new TH1F("DYMassDataCR_ee","DYMassDataCR_ee",nbins,minx,maxx);
  TH1F* DYMassDataSR_ee     = new TH1F("DYMassDataSR_ee","DYMassDataSR_ee",nbins,minx,maxx);
  TH1F* DYMassMCCR_ee       = new TH1F("DYMassMCCR_ee","DYMassMCCR_ee",nbins,minx,maxx);
  TH1F* DYMassMCSR_ee       = new TH1F("DYMassMCSR_ee","DYMassMCSR_ee",nbins,minx,maxx);
  //////////////////////
  
  cout<<"The verbosity mode is "<<verbosity <<endl;
  cout<<"The luminosity is equal to "<< Luminosity<<endl;
  cout<<"The DataType is ";
  switch(DataType){
  case 0: 
    cout<<"MC"<<endl; 
    break;
  case 1:
    cout<<"Data"<<endl; 
    break;
  case 2:
    cout<<"Data & MC"<<endl; 
    break;
  default:
    cout<<" unknown"<<endl; 
    break;
  }
  cout<<"SF for trigger mu "<< SF_trig_mu <<endl;
  cout<<"SF for trigger emu "<< SF_trig_emu <<endl;
  cout<<"SF for electronID "<< SF_e <<endl;
  //////////////////////
  
   //////////////////////
  //Load Vxp Reweighting
  //////////////////////
/*  if (IReweightWithVxP ) {

//      TFile* fin1  = new TFile("../data/VxP_Weight_MuEG.root","READ");
//      TH1F * R1 = new TH1F("R1","R1",20,-0.5,19.5);
//      R1 = (TH1F*)fin1->Get("VxP_Reweight");
//      for (unsigned int k = 1; k < R1->GetNbinsX()+1; k++) {
//        SF_VxP1[k-1] = R1->GetBinContent(k);
//        std::cout<<"#Bin,#Weight "<<k-1<<" "<<SF_VxP1[k-1]<<std::endl;
//      }
//      fin1->Close();
//      TFile* fin2  = new TFile("../data/VxP_Weight_Mu.root","READ");
//      TH1F * R2 = new TH1F("R2","R2",20,-0.5,19.5);
//      R2 = (TH1F*)fin2->Get("VxP_Reweight");
//      for (unsigned int k = 1; k < R2->GetNbinsX()+1; k++) {
//        SF_VxP2[k-1] = R2->GetBinContent(k);
//        std::cout<<"#Bin,#Weight "<<k-1<<" "<<SF_VxP2[k-1]<<std::endl;
//      }
//      fin2->Close();
//      TFile* fin3  = new TFile("../data/VxP_Weight_EG.root","READ");
//      TH1F * R3 = new TH1F("R3","R3",20,-0.5,19.5);
//      R3 = (TH1F*)fin3->Get("VxP_Reweight");
//      for (unsigned int k = 1; k < R3->GetNbinsX()+1; k++) {
//        SF_VxP3[k-1] = R3->GetBinContent(k);
//        std::cout<<"#Bin,#Weight "<<k-1<<" "<<SF_VxP3[k-1]<<std::endl;
//      }
//      fin3->Close();


     // from data 423 881.8 pb-1 & mc spring11 415 : ee+emu+mumu
 SF_VxP1[0]= 0.188314; 
 SF_VxP1[1]= 0.213954; 
 SF_VxP1[2]= 0.791285; 
 SF_VxP1[3]= 1.49953; 
 SF_VxP1[4]= 1.90534; 
 SF_VxP1[5]= 1.92912; 
 SF_VxP1[6]= 1.58496; 
 SF_VxP1[7]= 1.13643; 
 SF_VxP1[8]= 0.747046; 
 SF_VxP1[9]= 0.480223; 
 SF_VxP1[10]= 0.311164; 
 SF_VxP1[11]= 0.202998; 
 SF_VxP1[12]= 0.13784; 
 SF_VxP1[13]= 0.0934523; 
 SF_VxP1[14]= 0.066875; 
 SF_VxP1[15]= 0.0502024; 
 SF_VxP1[16]= 0.044272; 
 SF_VxP1[17]= 0.0326502; 
 SF_VxP1[18]= 0.0489623; 
 SF_VxP1[19]= 0.0523064; 


  }
*/ 
  
   
  PUWeighting  thePUReweighter;
  if (IReweight ) {
    TFile* file1  = new TFile(PUWeightFileName.c_str(),"READ");
    
    TH1D *  hPUData   =0;
    hPUData         = (TH1D*)file1->Get("pileup");
    TH1F *  hPUMC   = new TH1F("pileup_MC", "pileup_MC", hPUData->GetXaxis()->GetNbins(), hPUData->GetXaxis()->GetXmin(), hPUData->GetXaxis()->GetXmax() );
    //if non spring11 MC
    if(!IReweight_Spring11){ 
      TFile* file2  = new TFile( "../data/CrossSection_pileup.root" ,"READ");
      hPUMC           = (TH1F*)file2->Get("pileup_TTbarSig");
      // histo in data, histo in Mc, use out-of-time pu in the reweighting
      cout << "get MC histo  " << endl;
      thePUReweighter.setPUHisto( hPUData, hPUMC);
      cout << "set MC histo in thePUReweighter " << endl;
      thePUReweighter.setUseOutOfTimePU(false); // set to true to use out-of-time PU
    }
    else{
     thePUReweighter.setPUHisto( hPUData);
     thePUReweighter.setUseOutOfTimePU(false);
    }
  }
  
  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Loop over the datasets  "<<endl;
    cout<<"#########################"<<endl;
  }
  
// Creation of histogram list

  for (unsigned int d = 0; d < datasets.size (); d++) {
    hlistemu_ee       = DYest_ee.CreateDYHisto("emu",datasets[d].Name(),nbins,minx,maxx);
    hlistSR_ee        = DYest_ee.CreateDYHisto("SR",datasets[d].Name(),nbins,minx,maxx);
    hlistCR_ee        = DYest_ee.CreateDYHisto("CR",datasets[d].Name(),nbins,minx,maxx);
    hlistContCR_ee    = DYest_ee.CreateDYHisto("ContCR",datasets[d].Name(),nbins,minx,maxx);
    hlistemu_mumu     = DYest_mumu.CreateDYHisto("emu",datasets[d].Name(),nbins,minx,maxx);
    hlistSR_mumu      = DYest_mumu.CreateDYHisto("SR",datasets[d].Name(),nbins,minx,maxx);
    hlistCR_mumu      = DYest_mumu.CreateDYHisto("CR",datasets[d].Name(),nbins,minx,maxx);
    hlistContCR_mumu  = DYest_mumu.CreateDYHisto("ContCR",datasets[d].Name(),nbins,minx,maxx);
  }
  // ee
  bool FoundWjets = false;
   for (unsigned int j=0; j < hlistSR_ee.size(); j++) {
    if ( hlistSR_ee[j]->GetName()==string("WjetsSR") ) FoundWjets = true;  
   }
   if ( !FoundWjets ) { 
      hlistSR_ee       = DYest_ee.CreateDYHisto("SR","Wjets",nbins,minx,maxx);
      hlistCR_ee       = DYest_ee.CreateDYHisto("CR","Wjets",nbins,minx,maxx);
      hlistemu_ee      = DYest_ee.CreateDYHisto("emu","Wjets",nbins,minx,maxx);
      hlistContCR_ee   = DYest_ee.CreateDYHisto("ContCR","Wjets",nbins,minx,maxx);
   }  
  bool FoundVV = false;
   for (unsigned int j=0; j < hlistSR_ee.size(); j++) {
      if ( hlistSR_ee[j]->GetName()==string("VVSR") ) FoundVV = true;  
   }
   if ( !FoundVV ) { 
      hlistSR_ee       = DYest_ee.CreateDYHisto("SR","VV",nbins,minx,maxx);
      hlistCR_ee       = DYest_ee.CreateDYHisto("CR","VV",nbins,minx,maxx);
      hlistemu_ee      = DYest_ee.CreateDYHisto("emu","VV",nbins,minx,maxx);
      hlistContCR_ee   = DYest_ee.CreateDYHisto("ContCR","VV",nbins,minx,maxx);
   }  

  bool FoundSingleToptW = false;
   for (unsigned int j=0; j < hlistSR_ee.size(); j++) {
      if ( hlistSR_ee[j]->GetName()==string("SingleToptWSR") ) FoundSingleToptW = true;  
   }
   if ( !FoundSingleToptW ) { 
      hlistSR_ee       = DYest_ee.CreateDYHisto("SR","SingleToptW",nbins,minx,maxx);
      hlistCR_ee       = DYest_ee.CreateDYHisto("CR","SingleToptW",nbins,minx,maxx);
      hlistemu_ee      = DYest_ee.CreateDYHisto("emu","SingleToptW",nbins,minx,maxx);
      hlistContCR_ee   = DYest_ee.CreateDYHisto("ContCR","SingleToptW",nbins,minx,maxx);
   }  

  // mumu
  FoundWjets = false;
   for (unsigned int j=0; j < hlistSR_mumu.size(); j++) {
    if ( hlistSR_mumu[j]->GetName()==string("WjetsSR") ) FoundWjets = true;  
   }
   if ( !FoundWjets ) { 
      hlistSR_mumu     = DYest_mumu.CreateDYHisto("SR","Wjets",nbins,minx,maxx);
      hlistCR_mumu     = DYest_mumu.CreateDYHisto("CR","Wjets",nbins,minx,maxx);
      hlistemu_mumu    = DYest_mumu.CreateDYHisto("emu","Wjets",nbins,minx,maxx);
      hlistContCR_mumu = DYest_mumu.CreateDYHisto("ContCR","Wjets",nbins,minx,maxx);
   }  
  FoundVV = false;
   for (unsigned int j=0; j < hlistSR_mumu.size(); j++) {
      if ( hlistSR_mumu[j]->GetName()==string("VVSR") ) FoundVV = true;  
   }
   if ( !FoundVV ) { 
      hlistSR_mumu     = DYest_mumu.CreateDYHisto("SR","VV",nbins,minx,maxx);
      hlistCR_mumu     = DYest_mumu.CreateDYHisto("CR","VV",nbins,minx,maxx);
      hlistemu_mumu    = DYest_mumu.CreateDYHisto("emu","VV",nbins,minx,maxx);
      hlistContCR_mumu = DYest_mumu.CreateDYHisto("ContCR","VV",nbins,minx,maxx);
   }  
  
   FoundSingleToptW = false;
   for (unsigned int j=0; j < hlistSR_mumu.size(); j++) {
      if ( hlistSR_mumu[j]->GetName()==string("SingleToptWSR") ) FoundSingleToptW = true;  
   }
   if ( !FoundSingleToptW ) { 
      hlistSR_mumu       = DYest_mumu.CreateDYHisto("SR","SingleToptW",nbins,minx,maxx);
      hlistCR_mumu       = DYest_mumu.CreateDYHisto("CR","SingleToptW",nbins,minx,maxx);
      hlistemu_mumu      = DYest_mumu.CreateDYHisto("emu","SingleToptW",nbins,minx,maxx);
      hlistContCR_mumu   = DYest_mumu.CreateDYHisto("ContCR","SingleToptW",nbins,minx,maxx);
   }  


  for (unsigned int d = 0; d < datasets.size (); d++) {
    
    
    std::map<double, double > checkDuplicate;
    
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    //unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    unsigned int nEvents = (int) (datasets[d].NofEvtsToRunOver ());
    cout << "NEvents in " << datasets[d].Name() << " = " << nEvents << endl;
    
    
    MyhistoManager.CreateHisto(MyHistos_ee,                   "InvDilMassNJinf1_ee" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(MyHistos1_ee,                  "InvDilMassNJsup1_ee" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ll_pair_ee,              "InvDilMassPair_ee" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(MyHistos_mumu,                   "InvDilMassNJinf1_mumu" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(MyHistos1_mumu,                  "InvDilMassNJsup1_mumu" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ll_pair_mumu,              "InvDilMassPair_mumu" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    
    MyhistoManager.CreateHisto(HInvM_ll_pair_ee_sup15,          "InvDilMassPair_ee_sup15" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ll_pair_ee_inf15,          "InvDilMassPair_ee_inf15" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ll_pair_mumu_inf15,        "InvDilMassPair_mumu_inf15" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ll_pair_mumu_sup15,        "InvDilMassPair_mumu_sup15" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    
    MyhistoManager.CreateHisto(HHT_ee_aftermetbtag1,   "HHT_ee_aftermetbtag1"  , datasets[d].Name(),"HT","Entries",1000,0.,1000);
    MyhistoManager.CreateHisto(HHT_mumu_aftermetbtag1, "HHT_mumu_aftermetbtag1", datasets[d].Name(),"HT","Entries",1000,0.,1000);
    MyhistoManager.CreateHisto(HHT_emu_aftermetbtag1,  "HHT_emu_aftermetbtag1" , datasets[d].Name(),"HT","Entries",1000,0.,1000);
    
    MyhistoManager.CreateHisto(HHT_ee_aftermetbtag2,   "HHT_ee_aftermetbtag2"  , datasets[d].Name(),"HT","Entries",1000,0.,1000);
    MyhistoManager.CreateHisto(HHT_mumu_aftermetbtag2, "HHT_mumu_aftermetbtag2", datasets[d].Name(),"HT","Entries",1000,0.,1000);
    MyhistoManager.CreateHisto(HHT_emu_aftermetbtag2,  "HHT_emu_aftermetbtag2" , datasets[d].Name(),"HT","Entries",1000,0.,1000);
    
    MyhistoManager.CreateHisto(HInvM_emu_pair,             "InvemuMassPair" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_emu_pairzoom,             "InvemuMassPairzoom" ,datasets[d].Name(),"Mll","Entries",30,60.,120.);
    MyhistoManager.CreateHisto(HInvM_emu_pair_afterbtag1, "InvemuMassPair_afterbtag1" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_emu_pair_afterbtag2, "InvemuMassPair_afterbtag2" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_emu_pair_aftermetbtag1, "InvemuMassPair_aftermetbtag1" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_emu_pair_aftermetbtag2, "InvemuMassPair_aftermetbtag2" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_emu,              "Njets_emu" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_emu,                "Met_emu" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_emu,             "NBjets_emu" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtLpt1_emu,		"PtLpt1_emu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtLpt2_emu,		"PtLpt2_emu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_emu,		"PtJet1_emu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_emu,		"PtJet2_emu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HNVxP_emu,		"NVxP_emu" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);
    MyhistoManager.CreateHisto(HNVxP_emu_aftertrigger,   "NVxP_emu_aftertrigger" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);
    MyhistoManager.CreateHisto(HInvM_emu_pair_Zemu, "InvemuMassPair_Zemu" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    
    MyhistoManager.CreateHisto(HInvM_emu_pair_aftermetcut, "InvemuMassPair_aftermetcut" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_emu_aftermetcut,  "Njets_emu_aftermetcut" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_emu_aftermetcut,    "Met_emu_aftermetcut" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_emu_aftermetcut, "NBjets_emu_aftermetcut" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtLpt1_emu_aftermetcut, "PtLpt1_emu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtLpt2_emu_aftermetcut, "PtLpt2_emu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_emu_aftermetcut, "PtJet1_emu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_emu_aftermetcut, "PtJet2_emu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HNVxP_emu_aftermetcut,   "NVxP_emu_aftermetcut" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);

    MyhistoManager.CreateHisto(HInvM_emu_pair_afterjetcut, "InvemuMassPair_afterjetcut" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_emu_afterjetcut,  "Njets_emu_afterjetcut" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_emu_afterjetcut,    "Met_emu_afterjetcut" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_emu_afterjetcut, "NBjets_emu_afterjetcut" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtLpt1_emu_afterjetcut, "PtLpt1_emu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtLpt2_emu_afterjetcut, "PtLpt2_emu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_emu_afterjetcut, "PtJet1_emu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_emu_afterjetcut, "PtJet2_emu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    
    MyhistoManager.CreateHisto(HDeltaPhiLMet_emu,    "DeltaPhiLMet_emu" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_emu_aftermetcut,    "DeltaPhiLMet_emu_aftermetcut" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_emu_afterjetcut,    "DeltaPhiLMet_emu_afterjetcut" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);

    MyhistoManager.CreateHisto(HInvM_mumu_pair,             "InvmumuMassPair" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_mumu_pairzoom,         "InvmumuMassPairzoom" ,datasets[d].Name(),"Mll","Entries",30,60.,120.);
    MyhistoManager.CreateHisto(HInvM_mumu_pair_afterbtag1, "InvmumuMassPair_afterbtag1" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_mumu_pair_afterbtag2, "InvmumuMassPair_afterbtag2" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_mumu_pair_aftermetbtag1, "InvmumuMassPair_aftermetbtag1" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_mumu_pair_aftermetbtag2, "InvmumuMassPair_aftermetbtag2" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_mumu,              "Njets_mumu" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_mumu,                "Met_mumu" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_mumu,             "NBjets_mumu" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtMu1_mumu,              "PtMu1_mumu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtMu2_mumu,              "PtMu2_mumu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_mumu,             "PtJet1_mumu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_mumu,             "PtJet2_mumu" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HNVxP_mumu,		"NVxP_mumu" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);
    MyhistoManager.CreateHisto(HNVxP_mumu_aftertrigger,   "NVxP_mumu_aftertrigger" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);
    
    MyhistoManager.CreateHisto(HInvM_mumu_pair_aftermetcut, "InvmumuMassPair_aftermetcut" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_mumu_aftermetcut,  "Njets_mumu_aftermetcut" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_mumu_aftermetcut,    "Met_mumu_aftermetcut" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_mumu_aftermetcut, "NBjets_mumu_aftermetcut" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtMu1_mumu_aftermetcut,  "PtMu1_mumu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtMu2_mumu_aftermetcut,  "PtMu2_mumu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_mumu_aftermetcut, "PtJet1_mumu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_mumu_aftermetcut, "PtJet2_mumu_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HNVxP_mumu_aftermetcut,   "NVxP_mumu_aftermetcut" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);

    MyhistoManager.CreateHisto(HInvM_mumu_pair_afterjetcut, "InvmumuMassPair_afterjetcut" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_mumu_afterjetcut,  "Njets_mumu_afterjetcut" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_mumu_afterjetcut,    "Met_mumu_afterjetcut" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_mumu_afterjetcut, "NBjets_mumu_afterjetcut" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtMu1_mumu_afterjetcut,  "PtMu1_mumu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtMu2_mumu_afterjetcut,  "PtMu2_mumu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_mumu_afterjetcut, "PtJet1_mumu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_mumu_afterjetcut, "PtJet2_mumu_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    
    MyhistoManager.CreateHisto(HInvM_mumu_pair_afterveto, "InvmumuMassPair_afterveto" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_mumu_afterveto,  "Njets_mumu_afterveto" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_mumu_afterveto,    "Met_mumu_afterveto" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_mumu_afterveto, "NBjets_mumu_afterveto" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtMu1_mumu_afterveto,  "PtMu1_mumu_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtMu2_mumu_afterveto,  "PtMu2_mumu_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_mumu_afterveto, "PtJet1_mumu_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_mumu_afterveto, "PtJet2_mumu_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    
    MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu,    "DeltaPhiLMet_mumu" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu_aftermetcut,    "DeltaPhiLMet_mumu_aftermetcut" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu_afterjetcut,    "DeltaPhiLMet_mumu_afterjetcut" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_mumu_afterveto,    "DeltaPhiLMet_mumu_afterveto" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    
    MyhistoManager.CreateHisto(HInvM_ee_pair,             "InveeMassPair" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ee_pairzoom,         "InveeMassPairzoom" ,datasets[d].Name(),"Mll","Entries",30,60.,120.);
    MyhistoManager.CreateHisto(HInvM_ee_pair_afterbtag1, "InveeMassPair_afterbtag1" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ee_pair_afterbtag2, "InveeMassPair_afterbtag2" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ee_pair_aftermetbtag1, "InveeMassPair_aftermetbtag1" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HInvM_ee_pair_aftermetbtag2, "InveeMassPair_aftermetbtag2" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_ee,              "Njets_ee" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_ee,                "Met_ee" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_ee,             "NBjets_ee" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtEl1_ee,              "PtEl1_ee" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtEl2_ee,              "PtEl2_ee" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_ee,             "PtJet1_ee" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_ee,             "PtJet2_ee" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HNVxP_ee,		"NVxP_ee" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);
    MyhistoManager.CreateHisto(HNVxP_ee_aftertrigger,   "NVxP_ee_aftertrigger" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);
    
    MyhistoManager.CreateHisto(HInvM_ee_pair_aftermetcut, "InveeMassPair_aftermetcut" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_ee_aftermetcut,  "Njets_ee_aftermetcut" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_ee_aftermetcut,    "Met_ee_aftermetcut" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_ee_aftermetcut, "NBjets_ee_aftermetcut" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtEl1_ee_aftermetcut,  "PtEl1_ee_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtEl2_ee_aftermetcut,  "PtEl2_ee_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_ee_aftermetcut, "PtJet1_ee_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_ee_aftermetcut, "PtJet2_ee_aftermetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HNVxP_ee_aftermetcut,   "NVxP_ee_aftermetcut" ,datasets[d].Name(),"NbVxP","Entries",20,-0.5,19.5);

    MyhistoManager.CreateHisto(HInvM_ee_pair_afterjetcut, "InveeMassPair_afterjetcut" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_ee_afterjetcut,  "Njets_ee_afterjetcut" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_ee_afterjetcut,    "Met_ee_afterjetcut" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_ee_afterjetcut, "NBjets_ee_afterjetcut" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtEl1_ee_afterjetcut,  "PtEl1_ee_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtEl2_ee_afterjetcut,  "PtEl2_ee_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_ee_afterjetcut, "PtJet1_ee_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_ee_afterjetcut, "PtJet2_ee_afterjetcut" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);

    MyhistoManager.CreateHisto(HInvM_ee_pair_afterveto, "InveeMassPair_afterveto" ,datasets[d].Name(),"Mll","Entries",50,0.,350);
    MyhistoManager.CreateHisto(HNjets_ee_afterveto,  "Njets_ee_afterveto" ,datasets[d].Name(),"Njets","Entries",5,-0.5,4.5);
    MyhistoManager.CreateHisto(HMet_ee_afterveto,    "Met_ee_afterveto" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HNBjets_ee_afterveto, "NBjets_ee_afterveto" ,datasets[d].Name(),"NBjets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HPtEl1_ee_afterveto,  "PtEl1_ee_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtEl2_ee_afterveto,  "PtEl2_ee_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,250);
    MyhistoManager.CreateHisto(HPtJet1_ee_afterveto, "PtJet1_ee_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);
    MyhistoManager.CreateHisto(HPtJet2_ee_afterveto, "PtJet2_ee_afterveto" ,datasets[d].Name(),"Pt(GeV)","Entries",50,0.,300);

    MyhistoManager.CreateHisto(HDeltaPhiLMet_ee,    "DeltaPhiLMet_ee" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_ee_aftermetcut,    "DeltaPhiLMet_ee_aftermetcut" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_ee_afterjetcut,    "DeltaPhiLMet_ee_afterjetcut" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    MyhistoManager.CreateHisto(HDeltaPhiLMet_ee_afterveto,    "DeltaPhiLMet_ee_afterveto" ,datasets[d].Name(),"Min Delta Phi(Met,lepton)","Entries",30,0,2*M_PI);
    
    MyhistoManager.CreateHisto(HMet_emu_afterbtag1,    "Met_emu_afterbtag1" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HMet_mumu_afterbtag1,    "Met_mumu_afterbtag1" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HMet_ee_afterbtag1,    "Met_ee_afterbtag1" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HMet_emu_afterbtag2,    "Met_emu_afterbtag2" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HMet_mumu_afterbtag2,    "Met_mumu_afterbtag2" ,datasets[d].Name(),"Met","Entries",30,0,150);
    MyhistoManager.CreateHisto(HMet_ee_afterbtag2,    "Met_ee_afterbtag2" ,datasets[d].Name(),"Met","Entries",30,0,150);
    
    MyhistoManager.CreateHisto(pileup,                "pileup"            ,datasets[d].Name(),"pileup","Entries",36,-0.5,35.5);

      int debugcc=1000;
      int maxdebugcc=10;
    //////////////////////
    //LOOP OVER THE EVENTS
    //////////////////////
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
      
            cout << "event number=" << event->eventNb << endl;
      //      cout << "normfactor=" << datasets[d].NormFactor() << endl;
      //      cout << "Xsection=" << datasets[d].Xsection() << endl;
      //      cout << "NofEvtsToRunOver=" << datasets[d].NofEvtsToRunOver() << endl;
      
      
      if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
      //if(ievt > 20000) break;
      //Load event for the selection
       
      cout<<"Eric "<<endl; 
      cout<<sel.GetSFBweight().GetHistoSFB()<<endl;
      cout<<sel.GetSFBweight().GetHistoSFB()->GetEntries()<<endl;
      cout<<sel.GetSFBweight().GetHistoEffMCb()<<endl;
      cout<<sel.GetSFBweight().GetHistoEffMCb()->GetEntries()<<endl;
      sel.LoadEvent(event);
  
      //Collection of selected objects
      vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
      vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
      vector<NTMuon>     selMuons     = sel.GetSelectedMuons();
      vector<NTJet>      selJets      = sel.GetSelectedJets();
      NTMET met                       = sel.GetMET(); 
      
      
      //Candidate pair of lepton
      string CandType; // ee - emu - mumum or false
      vector<NTElectron> candElec;
      vector<NTMuon> candMuon;
      
      
      
      //Manage DY samples to avoid overlaps
      double dileptInvMass = 0;
      if( (event->zAndDecays).size() > 0){
        TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
	dileptInvMass = dilept.M();
      }
      
//      std::cout<<"dileptInvMass "<<dileptInvMass<<std::endl;
      
//      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50 ) cout << "problem !!!" << endl;
       
      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50) continue;
      
      if(datasets[d].Name()=="DYToMuMu_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToEE_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToTauTau_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToEE_M-10To20"	   &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToTauTau_M-10To20" &&  dileptInvMass > 20) continue;
      
      
     
//      cout << " aa " << datasets[d].Name() << " " << Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent() << endl;
      //Dweight[ITypeMC] = Luminosity*datasets[d].Xsection()/datasets[d].NofEvtsToRunOver();
      //cout << " getNSkimmedEvent " << datasets[d].getNSkimmedEvent() << endl;
      //cout << " NofEvtsToRunOver " << datasets[d].NofEvtsToRunOver() << endl;

      double weightITypeMC_save = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
      double weightITypeMC=0;
 
      // on introduit ici la loop sur les channels!
      for (int IChannel=0; IChannel<3; IChannel++) {
//      for (int IChannel=1; IChannel<2; IChannel++) {
          string ChannelName;
          if (IChannel==0) ChannelName= "mumu"; 
          else if (IChannel==1) ChannelName= "ee"; 
          else if (IChannel==2) ChannelName= "emu"; 


          if (IChannel==0 && (datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMuEG")) continue;
          if (IChannel==1 && (datasets[d].Name()=="DataMu" || datasets[d].Name()=="DataMuEG")) continue;
          if (IChannel==2 && (datasets[d].Name()=="DataMu" || datasets[d].Name()=="DataEG")) continue;

          if ( datasets[d].Name()!="DataEG" && datasets[d].Name()!="DataMu" && 
           datasets[d].Name()!="DataMuEG" && datasets[d].Name()!="DataEGMu") {

	   if(IReweight ){
	        if(thePUReweighter.getUseOutOfTimePU()){
		 weightITypeMC = weightITypeMC_save*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);
		 //cout << "weightITypeMC_save " << weightITypeMC_save << "  runnb " << event->runNb
		 //	<< " PU1 " << thePUReweighter.weight_Spring11(event->num_pileup_bc0) 
		//	<< " PU2 " << thePUReweighter.weight(event->num_pileup_bc0) 
		 //	<< " PU3 " << thePUReweighter.weight(event->num_pileup_bc0, event->runNb)<< endl;
		 }
	        else{
		  if( IReweight_Spring11){
		     weightITypeMC = weightITypeMC_save*thePUReweighter.weight_Spring11(event->num_pileup_bc0);
		     }
		  else {
		  
		  weightITypeMC = weightITypeMC_save*thePUReweighter.weight_Summer11ITP(event->num_pileup_bc0);
		  }
		}
	    }
	    else weightITypeMC = weightITypeMC_save;
/*
           if (IChannel==2) weightITypeMC = weightITypeMC_save*SF_VxP1[selVertices.size()];   
           else if (IChannel==0) weightITypeMC = weightITypeMC_save*SF_VxP2[selVertices.size()];
           else if (IChannel==1) weightITypeMC = weightITypeMC_save*SF_VxP3[selVertices.size()];
*/
//         weightITypeMC = weightITypeMC_save*SF_VxP1[selVertices.size()];

             

           //Now replaced by:
//           weightITypeMC = weightITypeMC_save*sel.GetPUWeight();
          }
          else { // data   
            weightITypeMC = 1;
          }
      
          bool IsSignal = false;
          int LastStep = 0;
          bool IsData = false;
          double WeightForBranchingRatio = 1.;
          bool IsLJ = false;
      
          if ( datasets[d].Name()=="TTbar" ) {
            if ( IChannel==0) { // "mumu" 
	      if ( event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000 )    IsSignal = true;
	      if ( !(event->TMEME==20 || event->TMEME==11010 || event->TMEME==22000) ) IsSignal = false;
	    }      
            else if ( IChannel==1) {  // "ee" 
	      if ( event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200 )     IsSignal = true;
	      if ( !(event->TMEME==2 || event->TMEME==10101 || event->TMEME==20200) )  IsSignal = false;
	    }      
            else if ( IChannel==2) { // "emu" 
	      if ( event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110 )     IsSignal = true;
	      if ( !(event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110) )  IsSignal = false;
	    }      
            if ( !IsSignal && event->TMEME!=0 )     IsLJ = true;
            if ( IsSignal ) {
              WeightForBranchingRatio = SF_BranchingRatio_ll;
            } else {
               if ( event->TMEME==0 ){
	          WeightForBranchingRatio = SF_BranchingRatio_had;
	       } else{
	          WeightForBranchingRatio = SF_BranchingRatio_lj;
	       } 
             }    
          }
          
          
          if ( datasets[d].Name()=="TTbar" ) { 
            if (IsSignal ) { ITypeMC = 0; Dweight[ITypeMC]= weightITypeMC * WeightForBranchingRatio; 
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC]; 
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
			     } 
	    else     { ITypeMC = 1; Dweight[ITypeMC]= weightITypeMC * WeightForBranchingRatio; 
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];}
          }
          else if ( datasets[d].Name()=="Zjets"              || 
	       datasets[d].Name()=="DYToMuMu_M-20"      || 
	       datasets[d].Name()=="DYToEE_M-20"        || 
	       datasets[d].Name()=="DYToTauTau_M-20"    || 
	       datasets[d].Name()=="DYToMuMu_M-10To20"  || 
	       datasets[d].Name()=="DYToEE_M-10To20"    || 
	       datasets[d].Name()=="DYToTauTau_M-10To20"
	       ) { 
            ITypeMC = 2; IsSignal = false; Dweight[ITypeMC]= weightITypeMC;  
/*  MOVE --> AFTER MET CUT
            if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_DY_mm; }
            else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_DY_em; }
            else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_DY_ee; }
*/
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="WjetsMu" ||
               datasets[d].Name()=="WjetsE"  ||
               datasets[d].Name()=="WjetsTau" ||
	       datasets[d].Name()=="Wjets"
           ) { 
            ITypeMC = 3; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
/*  MOVE --> AFTER MET CUT
            if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_Wjets_mm; }
            else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_Wjets_em; }
            else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_Wjets_ee; }
*/
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="SingleToptW" || datasets[d].Name()=="TtW" || datasets[d].Name()=="TbartW") { 
            ITypeMC = 4; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="WZ" || datasets[d].Name()=="WW" || datasets[d].Name()=="ZZ"  || datasets[d].Name()=="VV") { 
            ITypeMC = 5; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="DYee" ) { 
            ITypeMC = 6; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
/*  MOVE --> AFTER MET CUT
            if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_DY_mm; }
            else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_DY_em; }
            else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_DY_ee; }
*/
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="DYmumu" ) { 
            ITypeMC = 7; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC; 
/*  MOVE --> AFTER MET CUT
            if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_DY_mm; }
            else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_DY_em; }
            else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_DY_ee; }
*/
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }
          else if ( datasets[d].Name()=="QCD1" ||
               datasets[d].Name()=="QCD2" ||
               datasets[d].Name()=="QCD3" ||
               datasets[d].Name()=="QCD4" ||
               datasets[d].Name()=="QCD5" ||
               datasets[d].Name()=="QCD6" ||
               datasets[d].Name()=="QCD7" ||
               datasets[d].Name()=="QCD8" ||
               datasets[d].Name()=="QCD9" ||
               datasets[d].Name()=="QCD10" ||
               datasets[d].Name()=="QCD11" ||
               datasets[d].Name()=="QCD12" ||
               datasets[d].Name()=="QCD13" 
          
            ) { 
            ITypeMC = 8; IsSignal = false;  Dweight[ITypeMC]= weightITypeMC;
/*  MOVE --> AFTER MET CUT
            if (ChannelName=="mumu"){ Dweight[ITypeMC]*=SF_QCD_mm; }
            else if (ChannelName=="emu" ){ Dweight[ITypeMC]*=SF_QCD_em; }
            else if (ChannelName=="ee"  ){ Dweight[ITypeMC]*=SF_QCD_ee; }
*/
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }

          if ( datasets[d].Name()=="DataEG" || datasets[d].Name()=="DataMu" || 
               datasets[d].Name()=="DataMuEG" || datasets[d].Name()=="DataEGMu") { 
            ITypeMC = 100; IsData = true;  Dweight[ITypeMC]= weightITypeMC; 
                             TabFlow1[IChannel][ITypeMC][0]+=Dweight[ITypeMC];
                             TabFlow2[IChannel][ITypeMC][0]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          }

          if (debugcc<maxdebugcc) {
            cout << debugcc << " IChannel " << IChannel << " ChannelName " << ChannelName << " ITypeMC " << ITypeMC 
                 << " TabFlow1 " << TabFlow1[IChannel][ITypeMC][0] << endl;
//            debugcc++; 
           }
	   
          
// Seuelemnt pour PlotsFurnisherDeMerde
       //    if ( ChannelName=="ee" ) std::cout<<"code reference before "<<Dweight[ITypeMC] <<std::endl;
	   string CandType0 = "";
	   double LeptonSF = 0.; 
	   sel.GetLeptonPair(candMuon, candElec, CandType0 ); 
	   if(ApplyLeptonSF){
             if (CandType0=="mumu" &&  !IsData && (  datasets[d].Name() != "Wjets" &&  datasets[d].Name() != "QCD"  
                && !(datasets[d].Name()  =="TTbar" &&  !IsSignal) )   ) {
//	  	|| (datasets[d].Name()  =="TTbar" &&  IsSignal)  )   ) {
                LeptonSF =sel.getLeptonScaleFactor(candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), candMuon[1].p4.Pt(), candMuon[1].p4.Eta(), "mumu");
	     }
             if (CandType0=="emu"  &&  !IsData 
	   	&& ( datasets[d].Name() != "Wjets" &&  datasets[d].Name() != "QCD"  
                && !(datasets[d].Name()  =="TTbar" &&  !IsSignal) )   ) {
//		|| (datasets[d].Name()  =="TTbar" &&  IsSignal)     ) ){
		LeptonSF =sel.getLeptonScaleFactor(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), "emu");
             }
	     if (CandType0=="ee"   &&  !IsData 
	     	&&  (datasets[d].Name() != "Wjets" &&  datasets[d].Name() != "QCD"  
                && !(datasets[d].Name()  =="TTbar" &&  !IsSignal) )   ) {
//		|| (datasets[d].Name()  =="TTbar" &&  IsSignal)     ) ) {
                LeptonSF=sel.getLeptonScaleFactor(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candElec[1].p4.Pt(), candElec[1].p4.Eta(), "ee");
             }
	   }
	  if(producePLRPlots){ 
		plotsFurnisher.LoadEvent (sel, ChannelName, IsSignal, ITypeMC, event->TMEME, Dweight[ITypeMC], datasets[d], SF_trig_mu, SF_trig_emu, SF_trig_ee, SF_e,SF_mu, SF_DY_ee, SF_DY_mm, SF_DY_em,int(selElectrons.size()),CandType0,LeptonSF);
	}  

//     Passage trigger
      // Voir DileptonSelection.cc pour conditions de trigger
         MyhistoManager.FillHisto(pileup, "pileup" , event->num_pileup_bc0,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);

         if (  sel.passTriggerSelection ( &datasets[d], ChannelName)     ) {
//         if (  sel.passTriggerSelection ( &datasets[d], ChannelName)     || !IsData ) {

          if (debugcc<maxdebugcc) {
            cout <<  " seltrigger " ;
           }
	

   	   if (ChannelName=="mumu") MyhistoManager.FillHisto(HNVxP_mumu_aftertrigger, "NVxP_mumu_aftertrigger" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	   if (ChannelName=="ee")   MyhistoManager.FillHisto(HNVxP_ee_aftertrigger, "NVxP_ee_aftertrigger" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
    	   if (ChannelName=="emu")  MyhistoManager.FillHisto(HNVxP_emu_aftertrigger, "NVxP_emu_aftertrigger" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
        

   	   TabFlow1[IChannel][ITypeMC][10]+=Dweight[ITypeMC];
	   TabFlow2[IChannel][ITypeMC][10]+=Dweight[ITypeMC]*Dweight[ITypeMC];
	
//         add SF_trigger here   
           if (ChannelName=="mumu" &&  !IsData ){
	   	 Dweight[ITypeMC]*=SF_trig_mu;
           }
           if (ChannelName=="emu"  &&  !IsData ){
	   	  Dweight[ITypeMC]*=SF_trig_emu;
           }
           if (ChannelName=="ee"   &&  !IsData ){
	   	  Dweight[ITypeMC]*=SF_trig_ee;
           }

/*
	   //cout << "try rescale mumu " << endl;
           if (ChannelName=="mumu" &&  !IsData ){
	   	 Dweight[ITypeMC]*=SF_trig_mu;
		  if(datasets[d].Name()=="Zjets" || datasets[d].Name()=="DYee" || datasets[d].Name()=="DYmumu" )
		  	Dweight[ITypeMC]*=SF_DY_mm;
			
		  if(datasets[d].Name()=="Wjets" ) 
		  	Dweight[ITypeMC]*SF_Wjets_mm;
			
		  if(datasets[d].Name()=="QCD1" || datasets[d].Name()=="QCD2" || datasets[d].Name()=="QCD3" ||
                     datasets[d].Name()=="QCD4" || datasets[d].Name()=="QCD5" || datasets[d].Name()=="QCD6" ||
                     datasets[d].Name()=="QCD7" || datasets[d].Name()=="QCD8" || datasets[d].Name()=="QCD9" ||
                     datasets[d].Name()=="QCD10"|| datasets[d].Name()=="QCD11" ||   datasets[d].Name()=="QCD12" ||
                     datasets[d].Name()=="QCD13" ) 
		  	Dweight[ITypeMC]*=SF_QCD_mm;
		 
	   }
           if (ChannelName=="emu"  &&  !IsData ){
	   	  Dweight[ITypeMC]*=SF_trig_emu;
		  if(datasets[d].Name()=="Zjets" || datasets[d].Name()=="DYee" || datasets[d].Name()=="DYmumu" ) 
		  	Dweight[ITypeMC]*=SF_DY_em;
		  if(datasets[d].Name()=="Wjets" ) 
		  	Dweight[ITypeMC]*=SF_Wjets_em;
			
		  if(datasets[d].Name()=="QCD1" || datasets[d].Name()=="QCD2" || datasets[d].Name()=="QCD3" ||
                     datasets[d].Name()=="QCD4" || datasets[d].Name()=="QCD5" || datasets[d].Name()=="QCD6" ||
                     datasets[d].Name()=="QCD7" || datasets[d].Name()=="QCD8" || datasets[d].Name()=="QCD9" ||
                     datasets[d].Name()=="QCD10"|| datasets[d].Name()=="QCD11" ||   datasets[d].Name()=="QCD12" ||
                     datasets[d].Name()=="QCD13" ) 
		  	Dweight[ITypeMC]*=SF_QCD_em;
		  
	   }
           if (ChannelName=="ee"   &&  !IsData ){
	   	  Dweight[ITypeMC]*=SF_trig_ee;
		  if(datasets[d].Name()=="Zjets" || datasets[d].Name()=="DYee" || datasets[d].Name()=="DYmumu" ) 
		  	Dweight[ITypeMC]*=SF_DY_ee;
		  if(datasets[d].Name()=="Wjets" ) 
		  	Dweight[ITypeMC]*=SF_Wjets_ee;
			
		  if(datasets[d].Name()=="QCD1" || datasets[d].Name()=="QCD2" || datasets[d].Name()=="QCD3" ||
                     datasets[d].Name()=="QCD4" || datasets[d].Name()=="QCD5" || datasets[d].Name()=="QCD6" ||
                     datasets[d].Name()=="QCD7" || datasets[d].Name()=="QCD8" || datasets[d].Name()=="QCD9" ||
                     datasets[d].Name()=="QCD10"|| datasets[d].Name()=="QCD11" || datasets[d].Name()=="QCD12" ||
                     datasets[d].Name()=="QCD13" ) 
		  	Dweight[ITypeMC]*=SF_QCD_ee;
	   }
*/

	   //cout << "after trigger " << endl;
    	   TabFlow1[IChannel][ITypeMC][1]+=Dweight[ITypeMC];
	   TabFlow2[IChannel][ITypeMC][1]+=Dweight[ITypeMC]*Dweight[ITypeMC];

	   sel.GetLeptonPair(candMuon, candElec, CandType ); 

	   TLorentzVector DiLepton_mumu;
	   for(unsigned int i=0;i<candMuon.size();i++) DiLepton_mumu+=candMuon[i].p4;
	   TLorentzVector DiLepton_ee;
	   for(unsigned int i=0;i<candElec.size();i++) DiLepton_ee+=candElec[i].p4;
	   TLorentzVector DiLepton_emu;
	   for(unsigned int i=0;i<candMuon.size();i++) DiLepton_emu+=candMuon[i].p4;
	   for(unsigned int i=0;i<candElec.size();i++) DiLepton_emu+=candElec[i].p4;

	   float InvDilMass = 0;
	   if (ChannelName=="mumu")  InvDilMass = DiLepton_mumu.M();
	   if (ChannelName=="ee")    InvDilMass = DiLepton_ee.M();
	   if (ChannelName=="emu")   InvDilMass = DiLepton_emu.M();


           // compute DeltaPhi
           float deltaPhi_met1 =1000;
           float deltaPhi_met2 =1000;
           float deltaPhi_met  =1000;

           if (ChannelName=="mumu" && CandType=="mumu")   {
             deltaPhi_met1= candMuon[0].p4.DeltaPhi(met.p4);
             deltaPhi_met2= candMuon[1].p4.DeltaPhi(met.p4);
           }
           else if (ChannelName=="ee" && CandType=="ee") {
             deltaPhi_met1= candElec[0].p4.DeltaPhi(met.p4);
             deltaPhi_met2= candElec[1].p4.DeltaPhi(met.p4);
           }
           else if (ChannelName=="emu" && CandType=="emu") {
             deltaPhi_met1= candElec[0].p4.DeltaPhi(met.p4);
             deltaPhi_met2= candMuon[0].p4.DeltaPhi(met.p4);
           }

           if (deltaPhi_met1<0) deltaPhi_met1*=-1.;
           if (deltaPhi_met2<0) deltaPhi_met2*=-1.;
           if (deltaPhi_met1<deltaPhi_met2) deltaPhi_met=deltaPhi_met1;
           else deltaPhi_met=deltaPhi_met2;

           // keep track before SF_Id application (Caro)
   	   if ( ((CandType=="mumu" && ChannelName=="mumu")   || (CandType=="ee"   && ChannelName=="ee")     || (CandType=="emu"  && ChannelName=="emu")     )
	      && selVertices.size()>0 && InvDilMass> sel.GetMinValueMassCut() ) {
	      TabFlow1[IChannel][ITypeMC][11]+=Dweight[ITypeMC];
	      TabFlow2[IChannel][ITypeMC][11]+=Dweight[ITypeMC]*Dweight[ITypeMC];
           }
            // end of keep track
	   
	   if(ApplyLeptonSF){
	   if (ChannelName=="ee" ) {
//	   std::cout<<"avant applic SF weight = " << Dweight[ITypeMC]<<std::endl;
	   } 
	   
	   
             if (CandType=="mumu" &&  !IsData && (  datasets[d].Name() != "Wjets" &&  datasets[d].Name() != "QCD"  
                && !(datasets[d].Name()  =="TTbar" &&  !IsSignal) )   ) {
//	  	|| (datasets[d].Name()  =="TTbar" &&  IsSignal)  )   ) {
                 Dweight[ITypeMC]*=sel.getLeptonScaleFactor(candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), candMuon[1].p4.Pt(), candMuon[1].p4.Eta(), "mumu");
	     }
             if (CandType=="emu"  &&  !IsData 
	   	&& ( datasets[d].Name() != "Wjets" &&  datasets[d].Name() != "QCD"  
                && !(datasets[d].Name()  =="TTbar" &&  !IsSignal) )   ) {
//		|| (datasets[d].Name()  =="TTbar" &&  IsSignal)     ) ){
		 Dweight[ITypeMC]*=sel.getLeptonScaleFactor(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candMuon[0].p4.Pt(), candMuon[0].p4.Eta(), "emu");
             }
	     if (CandType=="ee"   &&  !IsData 
	     	&&  (datasets[d].Name() != "Wjets" &&  datasets[d].Name() != "QCD"  
                && !(datasets[d].Name()  =="TTbar" &&  !IsSignal) )   ) {
//		|| (datasets[d].Name()  =="TTbar" &&  IsSignal)     ) ) {
                Dweight[ITypeMC]*=sel.getLeptonScaleFactor(candElec[0].p4.Pt(), candElec[0].p4.Eta(), candElec[1].p4.Pt(), candElec[1].p4.Eta(), "ee");
             }
	   if (ChannelName=="ee" ) {
//	   std::cout<<"apres applic SF weight = " << Dweight[ITypeMC]<<std::endl;
	   } 
	   }
	   
	   
	   

//      Passage dilepton candidat
	if ( ((CandType=="mumu" && ChannelName=="mumu")   || 
	      (CandType=="ee"   && ChannelName=="ee")     ||
	      (CandType=="emu"  && ChannelName=="emu")     )
	      && selVertices.size()>0 
              && InvDilMass> sel.GetMinValueMassCut() ) {
          if (debugcc<maxdebugcc) {
            cout << " selpair " ;
           }

//	  TabFlow1[IChannel][ITypeMC][11]+=Dweight[ITypeMC];     // DONE BEFORE sel.getLeptonScaleFactor !!!
//	  TabFlow2[IChannel][ITypeMC][11]+=Dweight[ITypeMC]*Dweight[ITypeMC];
          

          //DYEstimation
          if (ChannelName=="emu" && sel.GetSelectedJets(candMuon, candElec).size()>=2 ) {
                DYest_ee.FillDYHistos("emu",hlistemu_ee,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                DYest_mumu.FillDYHistos("emu",hlistemu_mumu,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                if ( datasets[d].Name()=="DataMuEG" && CandType=="emu") hemudata_emu->Fill(InvDilMass,Dweight[ITypeMC]);
          }
	  
	  
          if(ChannelName == "ee" &&  !IsData ){
             if ( datasets[d].Name()=="TTbar")  {
               // ee
               if (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200  )   Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
               // emu
	       if ( event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110 ) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
               // ejet
               if (event->TMEME == 1 || event->TMEME == 10100) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
              }
              if ( datasets[d].Name()=="Zjets"              || 
	         datasets[d].Name()=="DYToEE_M-20"        || 
	         datasets[d].Name()=="DYToTauTau_M-20"    || 
	         datasets[d].Name()=="DYToEE_M-10To20"    || 
	         datasets[d].Name()=="DYToTauTau_M-10To20"
	         )Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
              if ( datasets[d].Name()=="WZ" || datasets[d].Name()=="WW" || datasets[d].Name()=="ZZ"  || datasets[d].Name()=="VV" )Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
              if ( datasets[d].Name()=="WjetsE"  || datasets[d].Name()=="WjetsTau" || datasets[d].Name()=="Wjets") Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
              if ( datasets[d].Name()=="SingleToptW" || datasets[d].Name()=="TtW" || datasets[d].Name()=="TbartW") Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e*SF_e;
          }


          int nrecoel= selElectrons.size();
          if(ChannelName == "emu" &&  !IsData ){
             if ( datasets[d].Name()=="TTbar")  {
               // ee
               if (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200  ) {
                     Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
                     if (nrecoel>1) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
               }
               // emu
	       if ( event->TMEME==11 || event->TMEME==21100 || event->TMEME==11001 || event->TMEME==10110 ) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
               // ejet
               if (event->TMEME == 1 || event->TMEME == 10100) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
              }
              if ( 
                 // datasets[d].Name()=="Zjets"              --> hyp: real mu, fake e
	         datasets[d].Name()=="DYToEE_M-20"        || 
	         datasets[d].Name()=="DYToEE_M-10To20"    
                 // DYTauTau -->> hyp : fake e
	         ) {
                     Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
                     if (nrecoel>1) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
                 }
              if ( datasets[d].Name()=="WZ" || datasets[d].Name()=="WW" || datasets[d].Name()=="ZZ"  || datasets[d].Name()=="VV") {
                     Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
                     if (nrecoel>1) Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
                 }
//              if ( datasets[d].Name()=="WjetsE"  || datasets[d].Name()=="WjetsTau" ) --> hyp : real mu, fake e
              if ( datasets[d].Name()=="SingleToptW" || datasets[d].Name()=="TtW" || datasets[d].Name()=="TbartW") Dweight[ITypeMC] = Dweight[ITypeMC]*SF_e;
          }



	  TabFlow1[IChannel][ITypeMC][2]+=Dweight[ITypeMC];
	  TabFlow2[IChannel][ITypeMC][2]+=Dweight[ITypeMC]*Dweight[ITypeMC];
	  

	  vector<NTJet>      theselJets = sel.GetSelectedJets(candMuon, candElec);

	  if (ChannelName=="mumu")  {
               MyhistoManager.FillHisto(HInvM_ll_pair_mumu, "InvDilMassPair_mumu" ,InvDilMass,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if( fabs(candMuon[0].p4.Eta()) <1.5 &&  fabs(candMuon[1].p4.Eta()) <1.5 )
	        MyhistoManager.FillHisto(HInvM_ll_pair_mumu_inf15, "InvDilMassPair_mumu_inf15" ,InvDilMass,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if( fabs(candMuon[0].p4.Eta()) >1.5 &&  fabs(candMuon[1].p4.Eta()) >.5 )
	        MyhistoManager.FillHisto(HInvM_ll_pair_mumu_sup15, "InvDilMassPair_mumu_sup15" ,InvDilMass,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if ( theselJets.size()<=1 )   MyhistoManager.FillHisto(MyHistos_mumu, "InvDilMassNJinf1_mumu",InvDilMass,datasets[d].Name(),IsSignal,weight);
	       else   MyhistoManager.FillHisto(MyHistos1_mumu,"InvDilMassNJsup1_mumu",InvDilMass,datasets[d].Name(),IsSignal,weight);

          }
          else if (ChannelName=="ee") {
               MyhistoManager.FillHisto(HInvM_ll_pair_ee, "InvDilMassPair_ee" ,InvDilMass,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if( fabs(candElec[0].p4.Eta()) <1.5 &&  fabs(candElec[1].p4.Eta()) <1.5 )
	        MyhistoManager.FillHisto(HInvM_ll_pair_ee_inf15, "InvDilMassPair_ee_inf15" ,InvDilMass,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if( fabs(candElec[0].p4.Eta()) >1.5 &&  fabs(candElec[1].p4.Eta()) >.5 )
	        MyhistoManager.FillHisto(HInvM_ll_pair_ee_sup15, "InvDilMassPair_ee_sup15" ,InvDilMass,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	       if ( theselJets.size()<=1 )   MyhistoManager.FillHisto(MyHistos_ee, "InvDilMassNJinf1_ee",InvDilMass,datasets[d].Name(),IsSignal,weight);
	       else   MyhistoManager.FillHisto(MyHistos1_ee,"InvDilMassNJsup1_ee",InvDilMass,datasets[d].Name(),IsSignal,weight);
          }

	  int JetMul = theselJets.size();
	  if ( JetMul>3 ) JetMul=4;
	  float misset = met.p4.Et();
	  
	  int NBtaggedJets = 0;
	  int AlgoBtag = sel.GetbtagAlgo();
	  for(unsigned int j=0;j<theselJets.size();j++){
	    if ( AlgoBtag==0 &&  theselJets[j].TCDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	    if ( AlgoBtag==1 &&  theselJets[j].SVDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	    if ( AlgoBtag==2 &&  theselJets[j].SMDiscri>=sel.GetbtagDiscriCut() ) NBtaggedJets++;
	  }			  
// weightb
          vector < float >weightb;
          weightb.push_back (1.);
          weightb.push_back (0.);
          weightb.push_back (0.);
          weightb.push_back (0.);
          weightb.push_back (0.);
          if (sel.GetFlagb() == 1) {
             if (!IsData) {      //MC
	      cout<<"LINE 1376"<<endl; 
              vector < float >weight_temp = sel.GetSFBweight().GetWeigth4BSel (sel.GetMethodb(), sel.GetSystb(),theselJets);
              weightb[0] = weight_temp[0]; //weight of the event
              weightb[1] = weight_temp[1];  //proba 0 jet
              weightb[2] = weight_temp[2];  //proba 1 jet
              weightb[3] = weight_temp[3];  //proba 2 jets
              weightb[4] = weight_temp[4];  //proba at least 3 jets
             }
/*
             else {  //data
               if (NBtaggedJets >= sel.GetNofBtagJetsCut()) {
                  weightb[0] = 1;
               }
               else  {
                  weightb[0]=0;
               }
              for (int ibj=0; ibj<4; ibj++) {
                 if ( NBtaggedJets== ibj) weightb[ibj+1] = 1;
                 else weightb[ibj] = 0;
              }
              if ( NBtaggedJets>=3) weightb[4] = 1;
             }
*/

          }
	  
	  int NBtaggedJets3 = NBtaggedJets;
	  if (NBtaggedJets3>3) NBtaggedJets3 = 3; 
	  
	  if(CandType=="mumu" && ChannelName=="mumu" ){
	    MyhistoManager.FillHisto(HInvM_mumu_pair, "InvmumuMassPair" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HInvM_mumu_pairzoom, "InvmumuMassPairzoom" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNjets_mumu,     "Njets_mumu"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HMet_mumu,       "Met_mumu"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtMu1_mumu, "PtMu1_mumu" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtMu2_mumu, "PtMu2_mumu" ,candMuon[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNVxP_mumu, "NVxP_mumu" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HDeltaPhiLMet_mumu, "DeltaPhiLMet_mumu" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu, "PtJet1_mumu" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu, "PtJet2_mumu" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    if (sel.GetFlagb() == 0 || IsData) {
              MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
            }
            else if (sel.GetFlagb() == 1) {
              MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
              MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
              MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
              MyhistoManager.FillHisto(HNBjets_mumu,    "NBjets_mumu"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
            }
	  }
	  if(CandType=="ee" && ChannelName=="ee" ){
	    MyhistoManager.FillHisto(HInvM_ee_pair, "InveeMassPair" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HInvM_ee_pairzoom, "InveeMassPairzoom" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNjets_ee,     "Njets_ee"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HMet_ee,       "Met_ee"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtEl1_ee, "PtEl1_ee" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HPtEl2_ee, "PtEl2_ee" ,candElec[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HNVxP_ee, "NVxP_ee" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    MyhistoManager.FillHisto(HDeltaPhiLMet_ee, "DeltaPhiLMet_ee" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee, "PtJet1_ee" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	    if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee, "PtJet2_ee" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
            if (sel.GetFlagb() == 0 || IsData) {
  	      MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
            }
            else if (sel.GetFlagb() == 1) {
              MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
              MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
              MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
              MyhistoManager.FillHisto(HNBjets_ee,    "NBjets_ee"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
            }
	  }
	  if(CandType=="emu" && ChannelName=="emu" ){
	     MyhistoManager.FillHisto(HInvM_emu_pair, "InvemuMassPair" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     MyhistoManager.FillHisto(HInvM_emu_pairzoom, "InvemuMassPairzoom" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     MyhistoManager.FillHisto(HNjets_emu,     "Njets_emu"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     MyhistoManager.FillHisto(HMet_emu,       "Met_emu"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     MyhistoManager.FillHisto(HNVxP_emu, "NVxP_emu" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     MyhistoManager.FillHisto(HDeltaPhiLMet_emu, "DeltaPhiLMet_emu" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     if (candElec[0].p4.Pt () > candMuon[0].p4.Pt ()) { 
	        MyhistoManager.FillHisto(HPtLpt1_emu, "PtLpt1_emu" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	        MyhistoManager.FillHisto(HPtLpt2_emu, "PtLpt2_emu" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
             } else {
	        MyhistoManager.FillHisto(HPtLpt1_emu, "PtLpt1_emu" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	        MyhistoManager.FillHisto(HPtLpt2_emu, "PtLpt2_emu" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     }
	     if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_emu, "PtJet1_emu" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_emu, "PtJet2_emu" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	     if (sel.GetFlagb() == 0 || IsData) {
	        MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
            }
            else if (sel.GetFlagb() == 1) {
	      MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(HNBjets_emu,    "NBjets_emu"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
            }
	  }
	  
          
//DYEstimation : mumu
           if ( InvDilMass>20 && ChannelName=="mumu" && CandType == ChannelName ){
              bool isinSR = false;
              bool isinCR = false;
	      
// cut au niveau dilepton pair ou apres la coupure en met
// 
              float metcutForDY = sel.GetMETCut().second;
              if ( !IDYestimateWithMetCut ) metcutForDY = -1;
              if ( theselJets.size()>1 ) {
                    if ( met.p4.Et()>metcutForDY ) {
	               DYest_mumu.FillDYHistos("SR",hlistSR_mumu,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
		       isinSR = true;
	            }	 
	      }  
	      
// cut au niveau dilepton pair ou apres la coupure en met
              if ( theselJets.size()<=1 ) { 
                    if ( met.p4.Et()> metcutForDY) { 
	               DYest_mumu.FillDYHistos("CR",hlistCR_mumu,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
	               if (datasets[d].Name()!="Zjets" && datasets[d].Name()!="DYToMuMu_M-20" && 
		          datasets[d].Name()!="DYToEE_M-20" && datasets[d].Name()!="DYToTauTau_M-20" &&
		          datasets[d].Name()!="DYToMuMu_M-10To20" && datasets[d].Name()!="DYToEE_M-10To20" && 
		          datasets[d].Name()!="DYToTauTau_M-10To20") {
		          DYest_mumu.FillDYHistos("ContCR",hlistContCR_mumu,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                       }
		       isinCR = true;
	            }
	      }
	      
// cut au niveau dilepton pair ou apres la coupure en met
	      if ( met.p4.Et()>metcutForDY ){  
                 DYest_mumu.FillNbofEventsInSRandCR(isinCR,isinSR,sel.DiLeptonMass(candMuon,candElec),datasets[d].Name(),Dweight[ITypeMC]);
                 if ( isinCR &&  IsData ) NjetsDataCR_mumu->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinSR &&  IsData ) NjetsDataSR_mumu->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinCR &&  !IsData ) NjetsMCCR_mumu->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinSR &&  !IsData ) NjetsMCSR_mumu->Fill(theselJets.size(),Dweight[ITypeMC]);
		 
                 if ( isinCR &&  IsData ) DYMassDataCR_mumu->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                 if ( isinSR &&  IsData ) DYMassDataSR_mumu->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                 bool isZjetslike = false;
                 isZjetslike = ( datasets[d].Name()=="Zjets" || datasets[d].Name()=="DYToMuMu_M-20" || datasets[d].Name()=="DYToEE_M-20" || datasets[d].Name()=="DYToTauTau_M-20" );
                 if ( isinCR &&  !IsData && isZjetslike ) DYMassMCCR_mumu->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                 if ( isinSR &&  !IsData && isZjetslike ) DYMassMCSR_mumu->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
              }
	   }

//DYEstimation : ee
           if ( InvDilMass>20 && ChannelName=="ee"  && CandType == ChannelName ){
              bool isinSR = false;
              bool isinCR = false;
	      
// cut au niveau dilepton pair ou apres la coupure en met
              float metcutForDY = sel.GetMETCut().second;
              if ( !IDYestimateWithMetCut ) metcutForDY = -1;
              if ( theselJets.size()>1 ) {
                 if ( met.p4.Et()>metcutForDY ) {
	            DYest_ee.FillDYHistos("SR",hlistSR_ee,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
		    isinSR = true;
		 }
	      }  
	      
// cut au niveau dilepton pair ou apres la coupure en met
              if ( theselJets.size()<=1 ) { 
                 if ( met.p4.Et()>metcutForDY ) { 
	            DYest_ee.FillDYHistos("CR",hlistCR_ee,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
	            if (datasets[d].Name()!="Zjets" && datasets[d].Name()!="DYToMuMu_M-20" && 
		        datasets[d].Name()!="DYToEE_M-20" && datasets[d].Name()!="DYToTauTau_M-20" &&
		        datasets[d].Name()!="DYToMuMu_M-10To20" && datasets[d].Name()!="DYToEE_M-10To20" && 
		        datasets[d].Name()!="DYToTauTau_M-10To20") {
		        DYest_ee.FillDYHistos("ContCR",hlistContCR_ee,datasets[d].Name(),event->TMEME,sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                    }
		    isinCR = true;
	         }
	      } 
	      
// cut au niveau dilepton pair ou apres la coupure en met
	      if ( met.p4.Et()>metcutForDY ){  
                 DYest_ee.FillNbofEventsInSRandCR(isinCR,isinSR,sel.DiLeptonMass(candMuon,candElec),datasets[d].Name(),Dweight[ITypeMC]);
                 if ( isinCR &&  IsData ) NjetsDataCR_ee->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinSR &&  IsData ) NjetsDataSR_ee->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinCR &&  !IsData ) NjetsMCCR_ee->Fill(theselJets.size(),Dweight[ITypeMC]);
                 if ( isinSR &&  !IsData ) NjetsMCSR_ee->Fill(theselJets.size(),Dweight[ITypeMC]);
		 
                 if ( isinCR &&  IsData ) DYMassDataCR_ee->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                 if ( isinSR &&  IsData ) DYMassDataSR_ee->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                 bool isZjetslike = false;
                 isZjetslike = ( datasets[d].Name()=="Zjets" || datasets[d].Name()=="DYToMuMu_M-20" || datasets[d].Name()=="DYToEE_M-20" || datasets[d].Name()=="DYToTauTau_M-20" );
                 if ( isinCR &&  !IsData && isZjetslike ) DYMassMCCR_ee->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
                 if ( isinSR &&  !IsData && isZjetslike ) DYMassMCSR_ee->Fill(sel.DiLeptonMass(candMuon,candElec),Dweight[ITypeMC]);
              }
	   }

// Rough Z->tautau->emu selection
	    if ( theselJets.size()<2 )  {
	        if(CandType=="emu" && ChannelName=="emu"){
		   if (datasets[d].Name()!="Zjets" ) {
	             MyhistoManager.FillHisto(HInvM_emu_pair_Zemu, "InvemuMassPair_Zemu" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   }else {
		      if (event->TMEME==21100) MyhistoManager.FillHisto(HInvM_emu_pair_Zemu, "InvemuMassPair_Zemu" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
		   }
	        }
	    }
	    
// Passage invariante mass dilepton
//	  if (( InvDilMass<sel.GetZmassWindowCut().first || InvDilMass>sel.GetZmassWindowCut().second ) && (InvDilMass> sel.GetMinValueMassCut() ))  {
	  if ( ( (ChannelName=="mumu" || ChannelName=="ee") && 
	        sel.DiLeptonMassCut(sel.GetMinValueMassCut(),sel.GetZmassWindowCut(),candMuon,candElec,ChannelName) ) || 
		(ChannelName=="emu" && InvDilMass> sel.GetMinValueMassCut())
	       )  {

	    TabFlow1[IChannel][ITypeMC][3]+=Dweight[ITypeMC];
	    TabFlow2[IChannel][ITypeMC][3]+=Dweight[ITypeMC]*Dweight[ITypeMC];

          if (debugcc<maxdebugcc) {
            cout << " selmass " ;
           }

		if(CandType=="mumu" && ChannelName=="mumu"){ 
	         MyhistoManager.FillHisto(HInvM_mumu_pair_afterveto, "InvmumuMassPair_afterveto" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HNjets_mumu_afterveto,     "Njets_mumu_afterveto"     ,JetMul,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HMet_mumu_afterveto,       "Met_mumu_afterveto"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HPtMu1_mumu_afterveto, "PtMu1_mumu_afterveto" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HPtMu2_mumu_afterveto, "PtMu2_mumu_afterveto" ,candMuon[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HDeltaPhiLMet_mumu_afterveto, "DeltaPhiLMet_mumu_afterveto" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu_afterveto, "PtJet1_mumu_afterveto" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu_afterveto, "PtJet2_mumu_afterveto" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 if (sel.GetFlagb() == 0 || IsData) {
	            MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 }
                 else if (sel.GetFlagb() == 1) {
	            MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_afterveto,    "NBjets_mumu_afterveto"    ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }
	        if(CandType=="ee" && ChannelName=="ee"){
	          MyhistoManager.FillHisto(HInvM_ee_pair_afterveto, "InveeMassPair_afterveto" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HNjets_ee_afterveto,     "Njets_ee_afterveto"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HMet_ee_afterveto,       "Met_ee_afterveto"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HPtEl1_ee_afterveto, "PtEl1_ee_afterveto" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HPtEl2_ee_afterveto, "PtEl2_ee_afterveto" ,candElec[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HDeltaPhiLMet_ee_afterveto, "DeltaPhiLMet_ee_afterveto" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee_afterveto, "PtJet1_ee_afterveto" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee_afterveto, "PtJet2_ee_afterveto" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  if (sel.GetFlagb() == 0 || IsData) {
	             MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  }
                  else if (sel.GetFlagb() == 1) {
	             MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_afterveto,    "NBjets_ee_afterveto"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }
	    
// Passage multiplicite de jets
	    if ( theselJets.size()>=2 )  {
	      TabFlow1[IChannel][ITypeMC][4]+=Dweight[ITypeMC];
	      TabFlow2[IChannel][ITypeMC][4]+=Dweight[ITypeMC]*Dweight[ITypeMC];

          if (debugcc<maxdebugcc) {
            cout << " seljet " ;
           }


		if(CandType=="mumu" && ChannelName=="mumu"){ 
	         MyhistoManager.FillHisto(HInvM_mumu_pair_afterjetcut, "InvmumuMassPair_afterjetcut" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HNjets_mumu_afterjetcut,     "Njets_mumu_afterjetcut"     ,JetMul,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HMet_mumu_afterjetcut,       "Met_mumu_afterjetcut"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HPtMu1_mumu_afterjetcut, "PtMu1_mumu_afterjetcut" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HPtMu2_mumu_afterjetcut, "PtMu2_mumu_afterjetcut" ,candMuon[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HDeltaPhiLMet_mumu_afterjetcut, "DeltaPhiLMet_mumu_afterjetcut" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu_afterjetcut, "PtJet1_mumu_afterjetcut" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu_afterjetcut, "PtJet2_mumu_afterjetcut" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 if (sel.GetFlagb() == 0 || IsData) {
	            MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 }
                 else if (sel.GetFlagb() == 1) {
	            MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_afterjetcut,    "NBjets_mumu_afterjetcut"    ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }
	        if(CandType=="ee" && ChannelName=="ee"){
	          MyhistoManager.FillHisto(HInvM_ee_pair_afterjetcut, "InveeMassPair_afterjetcut" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HNjets_ee_afterjetcut,     "Njets_ee_afterjetcut"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HMet_ee_afterjetcut,       "Met_ee_afterjetcut"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HPtEl1_ee_afterjetcut, "PtEl1_ee_afterjetcut" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HPtEl2_ee_afterjetcut, "PtEl2_ee_afterjetcut" ,candElec[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HDeltaPhiLMet_ee_afterjetcut, "DeltaPhiLMet_ee_afterjetcut" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee_afterjetcut, "PtJet1_ee_afterjetcut" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee_afterjetcut, "PtJet2_ee_afterjetcut" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  if (sel.GetFlagb() == 0 || IsData) {
	             MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  }
                  else if (sel.GetFlagb() == 1) {
	             MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_afterjetcut,    "NBjets_ee_afterjetcut"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }
	        if(CandType=="emu" && ChannelName=="emu"){
	           MyhistoManager.FillHisto(HInvM_emu_pair_afterjetcut, "InvemuMassPair_afterjetcut" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HNjets_emu_afterjetcut,     "Njets_emu_afterjetcut"     ,JetMul,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HMet_emu_afterjetcut,       "Met_emu_afterjetcut"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HDeltaPhiLMet_emu_afterjetcut, "DeltaPhiLMet_emu_afterjetcut" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	           if (candElec[0].p4.Pt () > candMuon[0].p4.Pt ()) { 
	              MyhistoManager.FillHisto(HPtLpt1_emu_afterjetcut, "PtLpt1_emu_afterjetcut" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	              MyhistoManager.FillHisto(HPtLpt2_emu_afterjetcut, "PtLpt2_emu_afterjetcut" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                   } else {
	              MyhistoManager.FillHisto(HPtLpt1_emu_afterjetcut, "PtLpt1_emu_afterjetcut" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	              MyhistoManager.FillHisto(HPtLpt2_emu_afterjetcut, "PtLpt2_emu_afterjetcut" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	           }
	           if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_emu_afterjetcut, "PtJet1_emu_afterjetcut" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	           if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_emu_afterjetcut, "PtJet2_emu_afterjetcut" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  if (sel.GetFlagb() == 0 || IsData) {
	             MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 }
                 else if (sel.GetFlagb() == 1) {
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_afterjetcut,    "NBjets_emu_afterjetcut"    ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }

              // plot MET after btag but w/o MET cut
              if (sel.GetFlagb() == 0 || IsData) {
                if (NBtaggedJets>=1 ) {
	          if(CandType=="mumu" && ChannelName=="mumu"){
	           MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag1, "InvmumuMassPair_afterbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HMet_mumu_afterbtag1,       "Met_mumu_afterbtag1"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
                  }
	          else if(CandType=="ee" && ChannelName=="ee"){
	           MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag1, "InveeMassPair_afterbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HMet_ee_afterbtag1,         "Met_ee_afterbtag1"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
                  }
	          else if(CandType=="emu" && ChannelName=="emu"){
	           MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag1, "InvemuMassPair_afterbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HMet_emu_afterbtag1,        "Met_emu_afterbtag1"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
                  }
                }
                if (NBtaggedJets>=2 ) {
                  if(CandType=="mumu" && ChannelName=="mumu"){
	           MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag2, "InvmumuMassPair_afterbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HMet_mumu_afterbtag2,       "Met_mumu_afterbtag2"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
                  }
                  else if(CandType=="ee" && ChannelName=="ee"){
	           MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag2, "InveeMassPair_afterbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HMet_ee_afterbtag2,         "Met_ee_afterbtag2"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
                  }
                  else if(CandType=="emu" && ChannelName=="emu"){
	           MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag2, "InvemuMassPair_afterbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HMet_emu_afterbtag2,        "Met_emu_afterbtag2"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
                  }
                }
              }
              else if (sel.GetFlagb() == 1) {
                 if(CandType=="mumu" && ChannelName=="mumu"){
                   MyhistoManager.FillHisto(HMet_mumu_afterbtag1,       "Met_mumu_afterbtag1"       ,misset,datasets[d].Name()       , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HMet_mumu_afterbtag2,       "Met_mumu_afterbtag2"       ,misset,datasets[d].Name()       , IsSignal, weightb[0]*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag1, "InvmumuMassPair_afterbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HInvM_mumu_pair_afterbtag2, "InvmumuMassPair_afterbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, weightb[0]*Dweight[ITypeMC]);
                  }
                  else if(CandType=="ee" && ChannelName=="ee"){
                   MyhistoManager.FillHisto(HMet_ee_afterbtag1,         "Met_ee_afterbtag1"       ,misset,datasets[d].Name()       , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HMet_ee_afterbtag2,         "Met_ee_afterbtag2"       ,misset,datasets[d].Name()       , IsSignal, weightb[0]*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag1, "InveeMassPair_afterbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HInvM_ee_pair_afterbtag2, "InveeMassPair_afterbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, weightb[0]*Dweight[ITypeMC]);
                  }
                  else if(CandType=="emu" && ChannelName=="emu"){
                   MyhistoManager.FillHisto(HMet_emu_afterbtag1,        "Met_emu_afterbtag1"       ,misset,datasets[d].Name()       , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HMet_emu_afterbtag2,        "Met_emu_afterbtag2"       ,misset,datasets[d].Name()       , IsSignal, weightb[0]*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag1, "InvemuMassPair_afterbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
                   MyhistoManager.FillHisto(HInvM_emu_pair_afterbtag2, "InvemuMassPair_afterbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, weightb[0]*Dweight[ITypeMC]);

                  } 
              }
	      
// Passage met
	      if ( ((ChannelName=="mumu" || ChannelName=="ee") &&  met.p4.Et()>sel.GetMETCut().second) ||
	           (ChannelName=="emu"  &&  met.p4.Et()>sel.GetMETCut().first)    )  {

             // APPLY SF_DD HERE! :
                 if (ChannelName=="mumu"){ 
                    if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )   { Dweight[ITypeMC]*=SF_DY_mm; }
                    else if (ITypeMC==3) { Dweight[ITypeMC]*=SF_Wjets_mm; }
                    else if (ITypeMC==8) { Dweight[ITypeMC]*=SF_QCD_mm; }
                 }
                 else if (ChannelName=="emu" ){
                    if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { Dweight[ITypeMC]*=SF_DY_em; }
                    else if (ITypeMC==3) { Dweight[ITypeMC]*=SF_Wjets_em; }
                    else if (ITypeMC==8) { Dweight[ITypeMC]*=SF_QCD_em; }
                 }
                 else if (ChannelName=="ee"  ){
                    if (ITypeMC==2 || ITypeMC==6 || ITypeMC==7 )    { Dweight[ITypeMC]*=SF_DY_ee; }
                    else if (ITypeMC==3) { Dweight[ITypeMC]*=SF_Wjets_ee; }
                    else if (ITypeMC==8) { Dweight[ITypeMC]*=SF_QCD_ee; }
                 }


		TabFlow1[IChannel][ITypeMC][5]+=Dweight[ITypeMC];
		TabFlow2[IChannel][ITypeMC][5]+=Dweight[ITypeMC]*Dweight[ITypeMC];

          if (debugcc<maxdebugcc) {
            cout << " selmet " ;
           }
		
//make sense?
//		if(checkDuplicate[event->eventNb] == event->runNb) cout << "duplicate evt" << event->eventNb << "  run " << event->runNb << endl;
		
		
		if(CandType=="mumu" && ChannelName=="mumu"){ 
	         MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetcut, "InvmumuMassPair_aftermetcut" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HNjets_mumu_aftermetcut,     "Njets_mumu_aftermetcut"     ,JetMul,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HMet_mumu_aftermetcut,       "Met_mumu_aftermetcut"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HPtMu1_mumu_aftermetcut, "PtMu1_mumu_aftermetcut" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HPtMu2_mumu_aftermetcut, "PtMu2_mumu_aftermetcut" ,candMuon[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HNVxP_mumu_aftermetcut, "NVxP_mumu_aftermetcut" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         MyhistoManager.FillHisto(HDeltaPhiLMet_mumu_aftermetcut, "DeltaPhiLMet_mumu_aftermetcut" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_mumu_aftermetcut, "PtJet1_mumu_aftermetcut" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	         if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_mumu_aftermetcut, "PtJet2_mumu_aftermetcut" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 if (sel.GetFlagb() == 0 || IsData) {
	            MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 }
                 else if (sel.GetFlagb() == 1) {
	            MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_mumu_aftermetcut,    "NBjets_mumu_aftermetcut"    ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }
	        if(CandType=="ee" && ChannelName=="ee"){
	          MyhistoManager.FillHisto(HInvM_ee_pair_aftermetcut, "InveeMassPair_aftermetcut" ,InvDilMass,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
//                  std::cout<<"code reference after "<<Dweight[ITypeMC] <<std::endl;
	          MyhistoManager.FillHisto(HNjets_ee_aftermetcut,     "Njets_ee_aftermetcut"      ,JetMul,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HMet_ee_aftermetcut,       "Met_ee_aftermetcut"        ,misset,       datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HPtEl1_ee_aftermetcut, "PtEl1_ee_aftermetcut" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HPtEl2_ee_aftermetcut, "PtEl2_ee_aftermetcut" ,candElec[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HNVxP_ee_aftermetcut, "NVxP_ee_aftermetcut" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          MyhistoManager.FillHisto(HDeltaPhiLMet_ee_aftermetcut, "DeltaPhiLMet_ee_aftermetcut" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_ee_aftermetcut, "PtJet1_ee_aftermetcut" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	          if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_ee_aftermetcut, "PtJet2_ee_aftermetcut" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  if (sel.GetFlagb() == 0 || IsData) {
	             MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  }
                  else if (sel.GetFlagb() == 1) {
	             MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	             MyhistoManager.FillHisto(HNBjets_ee_aftermetcut,    "NBjets_ee_aftermetcut"     ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }
	        if(CandType=="emu" && ChannelName=="emu"){
	           MyhistoManager.FillHisto(HInvM_emu_pair_aftermetcut, "InvemuMassPair_aftermetcut" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HNjets_emu_aftermetcut,     "Njets_emu_aftermetcut"     ,JetMul,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HMet_emu_aftermetcut,       "Met_emu_aftermetcut"       ,misset,datasets[d].Name()       , IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HNVxP_emu_aftermetcut, "NVxP_emu_aftermetcut" ,selVertices.size(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	           MyhistoManager.FillHisto(HDeltaPhiLMet_emu_aftermetcut, "DeltaPhiLMet_emu_aftermetcut" ,deltaPhi_met,   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	           if (candElec[0].p4.Pt () > candMuon[0].p4.Pt ()) { 
	              MyhistoManager.FillHisto(HPtLpt1_emu_aftermetcut, "PtLpt1_emu_aftermetcut" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	              MyhistoManager.FillHisto(HPtLpt2_emu_aftermetcut, "PtLpt2_emu_aftermetcut" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                   } else {
	              MyhistoManager.FillHisto(HPtLpt1_emu_aftermetcut, "PtLpt1_emu_aftermetcut" ,candMuon[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	              MyhistoManager.FillHisto(HPtLpt2_emu_aftermetcut, "PtLpt2_emu_aftermetcut" ,candElec[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	           }
	           if ( theselJets.size()>0) MyhistoManager.FillHisto(HPtJet1_emu_aftermetcut, "PtJet1_emu_aftermetcut" ,theselJets[0].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	           if ( theselJets.size()>1) MyhistoManager.FillHisto(HPtJet2_emu_aftermetcut, "PtJet2_emu_aftermetcut" ,theselJets[1].p4.Pt(),   datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                  if (sel.GetFlagb() == 0 || IsData) {
	             MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,NBtaggedJets3,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                 }
                 else if (sel.GetFlagb() == 1) {
	            MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,0,datasets[d].Name(), IsSignal, weightb[1]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,1,datasets[d].Name(), IsSignal, weightb[2]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,2,datasets[d].Name(), IsSignal, weightb[3]*Dweight[ITypeMC]);
	            MyhistoManager.FillHisto(HNBjets_emu_aftermetcut,    "NBjets_emu_aftermetcut"    ,3,datasets[d].Name(), IsSignal, weightb[4]*Dweight[ITypeMC]);
                 }
	        }
		
		
// Passage Btagging

		double HT = 0;
		for(unsigned int i=0; i<candElec.size(); i++){
		  HT+=candElec[i].p4.Pt();
		}
		for(unsigned int i=0; i<candMuon.size(); i++){
		  HT+=candMuon[i].p4.Pt();
		}
		for(unsigned int i=0; i<theselJets.size(); i++){
		  HT+=theselJets[i].p4.Pt();
		}
		HT+=met.p4.Et();
		
		
                if (sel.GetFlagb() == 0 || IsData) {
		  if ( NBtaggedJets>=1 )  {
		   TabFlow1[IChannel][ITypeMC][6]+=Dweight[ITypeMC];
		   TabFlow2[IChannel][ITypeMC][6]+=Dweight[ITypeMC]*Dweight[ITypeMC];
		   if(CandType=="ee"   && ChannelName=="ee"  ) {
                      MyhistoManager.FillHisto(HHT_ee_aftermetbtag1,   "HHT_ee_aftermetbtag1",   HT,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                      MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag1, "InveeMassPair_aftermetbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   }
                   else if(CandType=="emu"  && ChannelName=="emu" )  {
                      MyhistoManager.FillHisto(HHT_emu_aftermetbtag1,  "HHT_emu_aftermetbtag1",  HT,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                      MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag1, "InvemuMassPair_aftermetbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   }
                   else if(CandType=="mumu" && ChannelName=="mumu") {
                      MyhistoManager.FillHisto(HHT_mumu_aftermetbtag1, "HHT_mumu_aftermetbtag1", HT,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
                      MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag1, "InvmumuMassPair_aftermetbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   }
                  }
		  if ( NBtaggedJets>=2 )  {
		   TabFlow1[IChannel][ITypeMC][7]+=Dweight[ITypeMC];
		   TabFlow2[IChannel][ITypeMC][7]+=Dweight[ITypeMC]*Dweight[ITypeMC];
		   if(CandType=="ee"   && ChannelName=="ee"  ) {
                       MyhistoManager.FillHisto(HHT_ee_aftermetbtag2,   "HHT_ee_aftermetbtag2",   HT,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	               MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag2, "InveeMassPair_aftermetbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   }
		   if(CandType=="emu"  && ChannelName=="emu" ) {
                       MyhistoManager.FillHisto(HHT_emu_aftermetbtag2,  "HHT_emu_aftermetbtag2",  HT,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	               MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag2, "InvemuMassPair_aftermetbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   }
		   if(CandType=="mumu" && ChannelName=="mumu") {
                       MyhistoManager.FillHisto(HHT_mumu_aftermetbtag2, "HHT_mumu_aftermetbtag2", HT,datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	               MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag2, "InvmumuMassPair_aftermetbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, Dweight[ITypeMC]);
                   }
                  }
                }
                else if (sel.GetFlagb() == 1) {
                  //n btagged jet==1
		  TabFlow1[IChannel][ITypeMC][6]+=(1-weightb[1])*Dweight[ITypeMC];
		  TabFlow2[IChannel][ITypeMC][6]+=(1-weightb[1])*Dweight[ITypeMC]*(1-weightb[1])*Dweight[ITypeMC];
                  //n btagged jet==2
                  TabFlow1[IChannel][ITypeMC][7]+=weightb[0]*Dweight[ITypeMC];
                  TabFlow2[IChannel][ITypeMC][7]+=weightb[0]*Dweight[ITypeMC]*weightb[0]*Dweight[ITypeMC];

                   if(CandType=="ee"   && ChannelName=="ee"  ) {
                      MyhistoManager.FillHisto(HHT_ee_aftermetbtag1,   "HHT_ee_aftermetbtag1",   HT,datasets[d].Name(), IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
                      MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag1, "InveeMassPair_aftermetbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);

                      MyhistoManager.FillHisto(HHT_ee_aftermetbtag2,   "HHT_ee_aftermetbtag2",   HT,datasets[d].Name(), IsSignal, weightb[0]*Dweight[ITypeMC]);
                      MyhistoManager.FillHisto(HInvM_ee_pair_aftermetbtag2, "InveeMassPair_aftermetbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, weightb[0]*Dweight[ITypeMC]);
                   }
                   else if(CandType=="emu"  && ChannelName=="emu" ) {
                      MyhistoManager.FillHisto(HHT_emu_aftermetbtag1,  "HHT_emu_aftermetbtag1",  HT,datasets[d].Name(), IsSignal,(1-weightb[1])*Dweight[ITypeMC]); 
                      MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag1, "InvemuMassPair_aftermetbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);
 
                      MyhistoManager.FillHisto(HHT_emu_aftermetbtag2,  "HHT_emu_aftermetbtag2",  HT,datasets[d].Name(), IsSignal, weightb[0]*Dweight[ITypeMC]);
                      MyhistoManager.FillHisto(HInvM_emu_pair_aftermetbtag2, "InvemuMassPair_aftermetbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, weightb[0]*Dweight[ITypeMC]);
                   }
                   else if(CandType=="mumu" && ChannelName=="mumu") {
                      MyhistoManager.FillHisto(HHT_mumu_aftermetbtag1, "HHT_mumu_aftermetbtag1", HT,datasets[d].Name(), IsSignal,(1-weightb[1])*Dweight[ITypeMC]); 
                      MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag1, "InvmumuMassPair_aftermetbtag1" ,InvDilMass,datasets[d].Name()   , IsSignal, (1-weightb[1])*Dweight[ITypeMC]);

                      MyhistoManager.FillHisto(HHT_mumu_aftermetbtag2, "HHT_mumu_aftermetbtag2", HT,datasets[d].Name(), IsSignal, weightb[0]*Dweight[ITypeMC]);
                      MyhistoManager.FillHisto(HInvM_mumu_pair_aftermetbtag2, "InvmumuMassPair_aftermetbtag2" ,InvDilMass,datasets[d].Name()   , IsSignal, weightb[0]*Dweight[ITypeMC]);
                   }

		} // btag cuts
		
	      }//met.p4.Et()>X (X==30 for ee/mumu; X==20/0 for emu)

	    }//theselJets.size()>=2
	    
	  }// (DiLepton.M()<76 ||  DiLepton.M() >106) && DiLepton.M()>12
	  
	} //CandType (Dilepton pair+VxP && DiLepton.M()>12)
	
      }//event->TriggerPassed()	 
          if (debugcc<maxdebugcc) {
            cout << endl ;
           }
      
      } // end loop over IChannel
          if (debugcc<maxdebugcc) {
            debugcc++;
           }

    }  // end of loop over evts

    cout << " LOOP OVER EVENTS IN " << datasets[d].Name() << " IS FINISHED :) " << endl;
    
    
  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
  
  
//DYEstimation 
  DYest_mumu.MakeDYHistos("emu",hlistemu_mumu,halllistemu_mumu);
  DYest_mumu.MakeDYHistos("SR",hlistSR_mumu,halllistSR_mumu);
  DYest_mumu.MakeDYHistos("CR",hlistCR_mumu,halllistCR_mumu);
  DYest_mumu.MakeDYHistos("ContCR",hlistContCR_mumu,halllistContCR_mumu);
  DYest_mumu.MakeDataHistos("SR",hlistSR_mumu,hDataSR_mumu);
  DYest_mumu.MakeDataHistos("CR",hlistCR_mumu,hDataCR_mumu);
//DYEstimation 
  DYest_ee.MakeDYHistos("emu",hlistemu_ee,halllistemu_ee);
  DYest_ee.MakeDYHistos("SR",hlistSR_ee,halllistSR_ee);
  DYest_ee.MakeDYHistos("CR",hlistCR_ee,halllistCR_ee);
  DYest_ee.MakeDYHistos("ContCR",hlistContCR_ee,halllistContCR_ee);
  DYest_ee.MakeDataHistos("SR",hlistSR_ee,hDataSR_ee);
  DYest_ee.MakeDataHistos("CR",hlistCR_ee,hDataCR_ee);
  
  
  
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

  bool IUseAsymBW = true;
  DYest_mumu.SetDYVerbose(true);
  DYest_ee.SetDYVerbose(true);

  cout << " " << endl;
  cout << " DYEstimation : mumu " << endl;
  cout << "===================" << endl;

//DYEstimation  : mumu
// Fit the emu channel to parametrize the shape of the non-DY events
  vector<double> parafit_mumu;
  parafit_mumu = DYest_mumu.MakeDYFits_emu(halllistemu_mumu);
//  parafit_mumu = DYest_mumu.MakeDYFits_emu(halllistContCR_mumu);

// Fit the mumu or ee channel with the BW shape + parametrized shape of the non-DY events in the Signal Region for MC
// Take into account a scaled fraction of non-DY events for systematics
  if ( !IUseAsymBW ) {
     std::cout<<"Appel a MakeDYFits_SR "<<std::endl;
     DYest_mumu.MakeDYFits_SR(halllistSR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"MC",1.);   // simple BW + fond
  } else {  
    std::cout<<"Appel a MakeDYFits_SR asym"<<std::endl;
    DYest_mumu.MakeDYFits_SR_asym(halllistSR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"MC",1.); // asym BW + fond
  }
  
// Fit the mumu or ee channel with the BW shape in the Control Region for MC 
  std::cout<<"Appel a MakeDYFits_CR"<<std::endl;
//  DYest_mumu.MakeDYFits_CR(halllistCR_mumu,nbins,minx,maxx,0,0,"MC"); // simple BW
  if ( !IUseAsymBW ) {
    std::cout<<"Appel a MakeDYFits_CR"<<std::endl;
    DYest_mumu.MakeDYFits_CR_WithBkgSubtraction(halllistCR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"MC",1.);  // simple BW + fond
  } else {  
    if ( IDYestimateWithMetCut ) {
       std::cout<<"Appel a MakeDYFits_CR asym"<<std::endl;
       DYest_mumu.MakeDYFits_CR_WithBkgSubtraction_asym(halllistCR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"MC",1.);  // asym BW + fond
    }else{
       std::cout<<"Appel a MakeDYFits_CR asym plus CE"<<std::endl;
       DYest_mumu.MakeDYFits_CR_asym_plusCE(halllistCR_mumu,nbins,minx,maxx,0,0,"MC");  // asym BW + cuteffect
    }
  }
  
  std::cout<<"appel a ComputeDYEstimatedEvents_MC"<<std::endl;
  DYest_mumu.ComputeDYEstimatedEvents_MC();
  std::cout<<"Number of MC DY events in Z peak (for CR) = "<<DYest_mumu.GetNDYMCinMz_CR()<<std::endl;
  std::cout<<"Number of MC DY events in Z peak (for SR) = "<<DYest_mumu.GetNDYMCinMz_SR()<<std::endl;
  std::cout<<"Number of MC DY events outside the Z peak (for CR) = "<<DYest_mumu.GetNDYMC_CR()<<std::endl;
  std::cout<<"Number of fitted MC DY events in Z peak (for CR) = "<<DYest_mumu.GetNFittedDYMCinMz_CR()<<std::endl;
  std::cout<<"Number of fitted MC DY events in Z peak (for SR) = "<<DYest_mumu.GetNFittedDYMCinMz_SR()<<std::endl;
  std::cout<<"Number of fitted MC DY events outside the Z peak (for CR) = "<<DYest_mumu.GetNFittedDYMC_CR()<<std::endl;

// MC comparison between real and expected yields
  std::cout<<"Number of MC DY events outside the Z peak (for SR) = "<<DYest_mumu.GetNDYMC_SR()<<std::endl;
  std::cout<<"Number of ESTIMATED MC DY events outside the Z peak (for SR) = "<<DYest_mumu.GetNDYEstimatedMC() <<std::endl;

// Computation for Data
// Fit the emu channel to parametrize the shape of the non-DY events
//  parafit_mumu.clear();
//  parafit_mumu = DYest_mumu.MakeDYFits_emu(hemudata_emu);
// Fit the mumu or ee channel with the BW shape + parametrized MC shape of the non-DY events in the Signal Region 

  if ( !IUseAsymBW ) {
     std::cout<<"Appel a MakeDYFits_SR pour Data"<<std::endl;
     DYest_mumu.MakeDYFits_SR(hDataSR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"Data",1.); // simple BW + fond
  } else {  
    std::cout<<"Appel a MakeDYFits_SR asym pour Data"<<std::endl;
    DYest_mumu.MakeDYFits_SR_asym(hDataSR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"Data",1.);  // asym BW + fond
  }
  
// Fit the mumu or ee channel with the BW shape in the Control Region    
  std::cout<<"Appel a MakeDYFits_CR pour Data"<<std::endl;
//  DYest_mumu.MakeDYFits_CR(hDataCR_mumu,nbins,minx,maxx,0,0,"Data"); // simple BW
  if ( !IUseAsymBW ) {
    std::cout<<"Appel a MakeDYFits_CR_WithBkgSubtraction pour Data"<<std::endl;
    DYest_mumu.MakeDYFits_CR_WithBkgSubtraction(hDataCR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"Data",1.);  // simple BW + fond
  } else {
    if ( IDYestimateWithMetCut ) {
       std::cout<<"Appel a MakeDYFits_CR_WithBkgSubtraction asym pour Data"<<std::endl;
       DYest_mumu.MakeDYFits_CR_WithBkgSubtraction_asym(hDataCR_mumu,nbins,minx,maxx,parafit_mumu,0,0,"Data",1.);  // asym BW + fond
    }else{
       std::cout<<"Appel a MakeDYFits_CR asym plus CE pour DATA "<<std::endl;
       DYest_mumu.MakeDYFits_CR_asym_plusCE(hDataCR_mumu,nbins,minx,maxx,0,0,"Data");  // asym BW + cuteffect
    }
  }  
  
// Compute the number of expected bckg according to 
// "Counting" method (simply count the number of events in each zones) or from a "Fit" method of the inv.mass distribution (effect of others process bckg for DY can be taken into account)
// "MC" or "DATA" origin for the scale factor (NofEventsintheMZpieak(inSR)/NofEventsintheMZpieak(inCR))
  std::cout<<""<<std::endl; 
  std::cout<<"==== Computation of DY_mumu estimate (Counting) ==== "<<std::endl; 
  DYest_mumu.ComputeDYEstimatedEvents_DATA("Counting","Data");
  std::cout<<"Counted Number of Data events outside the Z peak (for SR)                              = "<<DYest_mumu.GetNDATA_SR() <<std::endl;
  std::cout<<"Counted Number of Data events outside the Z peak (for CR)                              = "<<DYest_mumu.GetNDATA_CR() <<std::endl;
  std::cout<<"Counted Number of Data events outside the Z peak (for CR, with MCbkg subtraction) (1)  = "<<DYest_mumu.GetNDATA_BkgSubtracted_CR() <<std::endl;
  std::cout<<"Counted Number of Data events inside the Z peak (for SR) (2)                           = "<<DYest_mumu.GetNDATAinMz_SR() <<std::endl;
  std::cout<<"Counted Number of Continuum inside the Z peak    (for SR)(3)                           = "<<DYest_mumu.GetContinuumInZdata() <<std::endl;
  std::cout<<"Counted Number of Data events inside the Z peak (for CR) (4)                           = "<<DYest_mumu.GetNDATAinMz_CR() <<std::endl;
  std::cout<<"Counted Number of ESTIMATED DY events outside the Z peak (for SR) [(1)*((2)-(3))/(4)]  =  "<<DYest_mumu.GetNDYEstimatedDATACounting() <<std::endl;
  float sterc =  DYest_mumu.GetNDYEstimatedStatErrorDATACounting();
  std::cout<<"======== "<<std::endl; 
  std::cout<<"==== Computation of DY_mumu estimate (Fitting) ==== "<<std::endl; 
  DYest_mumu.ComputeDYEstimatedEvents_DATA("Fit","Data");
  std::cout<<"Fitted Number of DY events outside the Z peak (for CR) (1)                            = "<<DYest_mumu.GetNFittedDATA_CR() <<std::endl;
  std::cout<<"Fitted Number of DY events inside the Z peak  (for SR) (2)                            = "<<DYest_mumu.GetNFittedDATAinMz_SR() <<std::endl;
  std::cout<<"Fitted Number of DY events inside the Z peak  (for CR) (3)                            = "<<DYest_mumu.GetNFittedDATAinMz_CR() <<std::endl;
  std::cout<<"Fitting Number of ESTIMATED DY events outside the Z peak (for SR) [(1)*(2)/(3)]       = "<<DYest_mumu.GetNDYEstimatedDATA() <<std::endl;
  std::cout<<"to be compared with the number of MC DY events outside the Z peak (for SR) = "<< DYest_mumu.GetNDYMC_SR() <<std::endl;
  std::cout<<"==== Errors ==== "<<std::endl; 
  std::cout<<"statistical error (fitting) =  "<<DYest_mumu.GetNDYEstimatedStatErrorDATA() <<std::endl;
  std::cout<<"systematical error (fitting)=  "<<DYest_mumu.GetNDYEstimatedSysErrorDATA() <<std::endl;
  float ert1 = sqrt(DYest_mumu.GetNDYEstimatedStatErrorDATA()*DYest_mumu.GetNDYEstimatedStatErrorDATA()+
                    DYest_mumu.GetNDYEstimatedSysErrorDATA()*DYest_mumu.GetNDYEstimatedSysErrorDATA());
  std::cout<<"total error  (fitting)      =  "<< ert1 <<std::endl;
  std::cout<<"statistical error (counting)  =  "<<sterc <<std::endl;
  std::cout<<"systematical error (counting) =  "<<DYest_mumu.GetNDYEstimatedSysErrorDATACounting() <<std::endl;
  float ert0 = sqrt(sterc*sterc+DYest_mumu.GetNDYEstimatedSysErrorDATACounting()*DYest_mumu.GetNDYEstimatedSysErrorDATACounting());
  std::cout<<"total error  (counting)       =  "<< ert0 <<std::endl;
  std::cout<<"======== "<<std::endl; 
  std::cout<<""<<std::endl; 

  cout << " " << endl;
  cout << " DYEstimation : ee " << endl;
  cout << "===================" << endl;
//DYEstimation  : ee
// Fit the emu channel to parametrize the shape of the non-DY events
  vector<double> parafit_ee;
  parafit_ee = DYest_ee.MakeDYFits_emu(halllistemu_ee);
//  parafit_ee = DYest_ee.MakeDYFits_emu(halllistContCR_ee);

// Fit the mumu or ee channel with the BW shape + parametrized shape of the non-DY events in the Signal Region for MC
// Take into account a scaled fraction of non-DY events for systematics
  if ( !IUseAsymBW ) {
     std::cout<<"Appel a MakeDYFits_SR "<<std::endl;
     DYest_ee.MakeDYFits_SR(halllistSR_ee,nbins,minx,maxx,parafit_ee,0,0,"MC",1.);   // simple BW + fond
  } else {  
    std::cout<<"Appel a MakeDYFits_SR asym"<<std::endl;
    DYest_ee.MakeDYFits_SR_asym(halllistSR_ee,nbins,minx,maxx,parafit_ee,0,0,"MC",1.); // asym BW + fond
  }
  
// Fit the mumu or ee channel with the BW shape in the Control Region for MC 
  std::cout<<"Appel a MakeDYFits_CR"<<std::endl;
//  DYest_ee.MakeDYFits_CR(halllistCR_ee,nbins,minx,maxx,0,0,"MC"); // simple BW
  if ( !IUseAsymBW ) {
    std::cout<<"Appel a MakeDYFits_CR"<<std::endl;
    DYest_ee.MakeDYFits_CR_WithBkgSubtraction(halllistCR_ee,nbins,minx,maxx,parafit_ee,0,0,"MC",1.);  // simple BW + fond
  } else {  
    if ( IDYestimateWithMetCut ) {
       std::cout<<"Appel a MakeDYFits_CR asym"<<std::endl;
       DYest_ee.MakeDYFits_CR_WithBkgSubtraction_asym(halllistCR_ee,nbins,minx,maxx,parafit_ee,0,0,"MC",1.);  // asym BW + fond
    }else{
       std::cout<<"Appel a MakeDYFits_CR asym plus CE  "<<std::endl;
       DYest_ee.MakeDYFits_CR_asym_plusCE(halllistCR_ee,nbins,minx,maxx,0,0,"MC");  // asym BW + cuteffect
    }
  }
  
  std::cout<<"appel a ComputeDYEstimatedEvents_MC"<<std::endl;
  DYest_ee.ComputeDYEstimatedEvents_MC();
  std::cout<<"Number of MC DY events in Z peak (for CR) = "<<DYest_ee.GetNDYMCinMz_CR()<<std::endl;
  std::cout<<"Number of MC DY events in Z peak (for SR) = "<<DYest_ee.GetNDYMCinMz_SR()<<std::endl;
  std::cout<<"Number of MC DY events outside the Z peak (for CR) = "<<DYest_ee.GetNDYMC_CR()<<std::endl;
  std::cout<<"Number of fitted MC DY events in Z peak (for CR) = "<<DYest_ee.GetNFittedDYMCinMz_CR()<<std::endl;
  std::cout<<"Number of fitted MC DY events in Z peak (for SR) = "<<DYest_ee.GetNFittedDYMCinMz_SR()<<std::endl;
  std::cout<<"Number of fitted MC DY events outside the Z peak (for CR) = "<<DYest_ee.GetNFittedDYMC_CR()<<std::endl;

// MC comparison between real and expected yields
  std::cout<<"Number of MC DY events outside the Z peak (for SR) = "<<DYest_ee.GetNDYMC_SR()<<std::endl;
  std::cout<<"Number of ESTIMATED MC DY events outside the Z peak (for SR) = "<<DYest_ee.GetNDYEstimatedMC() <<std::endl;

// Computation for Data
// Fit the emu channel to parametrize the shape of the non-DY events
//  parafit_ee.clear();
//  parafit_ee = DYest_ee.MakeDYFits_emu(hemudata_emu);
// Fit the mumu or ee channel with the BW shape + parametrized MC shape of the non-DY events in the Signal Region 

  if ( !IUseAsymBW ) {
     std::cout<<"Appel a MakeDYFits_SR pour Data"<<std::endl;
     DYest_ee.MakeDYFits_SR(hDataSR_ee,nbins,minx,maxx,parafit_ee,0,0,"Data",1.); // simple BW + fond
  } else {  
    std::cout<<"Appel a MakeDYFits_SR asym pour Data"<<std::endl;
    DYest_ee.MakeDYFits_SR_asym(hDataSR_ee,nbins,minx,maxx,parafit_ee,0,0,"Data",1.);  // asym BW + fond
  }
  
// Fit the mumu or ee channel with the BW shape in the Control Region    
  std::cout<<"Appel a MakeDYFits_CR pour Data"<<std::endl;
//  DYest_ee.MakeDYFits_CR(hDataCR_ee,nbins,minx,maxx,0,0,"Data"); // simple BW
  if ( !IUseAsymBW ) {
    std::cout<<"Appel a MakeDYFits_CR_WithBkgSubtraction pour Data"<<std::endl;
    DYest_ee.MakeDYFits_CR_WithBkgSubtraction(hDataCR_ee,nbins,minx,maxx,parafit_ee,0,0,"Data",1.);  // simple BW + fond
  } else {
    if ( IDYestimateWithMetCut ) {
       std::cout<<"Appel a MakeDYFits_CR_WithBkgSubtraction asym pour Data"<<std::endl;
       DYest_ee.MakeDYFits_CR_WithBkgSubtraction_asym(hDataCR_ee,nbins,minx,maxx,parafit_ee,0,0,"Data",1.);  // asym BW + fond
    }else{
       std::cout<<"Appel a MakeDYFits_CR asym plus CE pour DATA  "<<std::endl;
       DYest_ee.MakeDYFits_CR_asym_plusCE(hDataCR_ee,nbins,minx,maxx,0,0,"Data");  // asym BW + cuteffect
    }
  }  
  
// Compute the number of expected bckg according to 
// "Counting" method (simply count the number of events in each zones) or from a "Fit" method of the inv.mass distribution (effect of others process bckg for DY can be taken into account)
// "MC" or "DATA" origin for the scale factor (NofEventsintheMZpieak(inSR)/NofEventsintheMZpieak(inCR))
  std::cout<<""<<std::endl; 
  std::cout<<"==== Computation of DY_ee estimate (Counting) ==== "<<std::endl; 
  DYest_ee.ComputeDYEstimatedEvents_DATA("Counting","Data");
  std::cout<<"Counted Number of Data events outside the Z peak (for SR)                              = "<<DYest_ee.GetNDATA_SR() <<std::endl;
  std::cout<<"Counted Number of Data events outside the Z peak (for CR)                              = "<<DYest_ee.GetNDATA_CR() <<std::endl;
  std::cout<<"Counted Number of Data events outside the Z peak (for CR, with MCbkg subtraction) (1)  = "<<DYest_ee.GetNDATA_BkgSubtracted_CR() <<std::endl;
  std::cout<<"Counted Number of Data events inside the Z peak (for SR) (2)                           = "<<DYest_ee.GetNDATAinMz_SR() <<std::endl;
  std::cout<<"Counted Number of Continuum inside the Z peak    (for SR)(3)                           = "<<DYest_ee.GetContinuumInZdata() <<std::endl;
  std::cout<<"Counted Number of Data events inside the Z peak (for CR) (4)                           = "<<DYest_ee.GetNDATAinMz_CR() <<std::endl;
  std::cout<<"Counted Number of ESTIMATED DY events outside the Z peak (for SR) [(1)*((2)-(3))/(4)]  =  "<<DYest_ee.GetNDYEstimatedDATACounting() <<std::endl;
  sterc =  DYest_ee.GetNDYEstimatedStatErrorDATACounting();
  std::cout<<"======== "<<std::endl; 
  std::cout<<"==== Computation of DY_ee estimate (Fitting) ==== "<<std::endl; 
  DYest_ee.ComputeDYEstimatedEvents_DATA("Fit","Data");
  std::cout<<"Fitted Number of DY events outside the Z peak (for CR) (1)                            = "<<DYest_ee.GetNFittedDATA_CR() <<std::endl;
  std::cout<<"Fitted Number of DY events inside the Z peak  (for SR) (2)                            = "<<DYest_ee.GetNFittedDATAinMz_SR() <<std::endl;
  std::cout<<"Fitted Number of DY events inside the Z peak  (for CR) (3)                            = "<<DYest_ee.GetNFittedDATAinMz_CR() <<std::endl;
  std::cout<<"Fitting Number of ESTIMATED DY events outside the Z peak (for SR) [(1)*(2)/(3)]       = "<<DYest_ee.GetNDYEstimatedDATA() <<std::endl;
  std::cout<<"to be compared with the number of MC DY events outside the Z peak (for SR) = "<< DYest_ee.GetNDYMC_SR() <<std::endl;
  std::cout<<"==== Errors ==== "<<std::endl; 
  std::cout<<"statistical error (fitting) =  "<<DYest_ee.GetNDYEstimatedStatErrorDATA() <<std::endl;
  std::cout<<"systematical error (fitting)=  "<<DYest_ee.GetNDYEstimatedSysErrorDATA() <<std::endl;
  ert1 = sqrt(DYest_ee.GetNDYEstimatedStatErrorDATA()*DYest_ee.GetNDYEstimatedStatErrorDATA()+
                    DYest_ee.GetNDYEstimatedSysErrorDATA()*DYest_ee.GetNDYEstimatedSysErrorDATA());
  std::cout<<"total error  (fitting)      =  "<< ert1 <<std::endl;
  std::cout<<"statistical error (counting)  =  "<<sterc <<std::endl;
  std::cout<<"systematical error (counting) =  "<<DYest_ee.GetNDYEstimatedSysErrorDATACounting() <<std::endl;
  ert0 = sqrt(sterc*sterc+DYest_ee.GetNDYEstimatedSysErrorDATACounting()*DYest_ee.GetNDYEstimatedSysErrorDATACounting());
  std::cout<<"total error  (counting)       =  "<< ert0 <<std::endl;
  std::cout<<"======== "<<std::endl; 
  std::cout<<""<<std::endl; 
  
  //modifdg
  std::vector<string> ordre; // display = from bottom to top
  ordre.push_back("TTbarBkg");
  ordre.push_back("WjetsMu");
  ordre.push_back("WjetsE");
  ordre.push_back("WjetsTau");
  ordre.push_back("Wjets");
  ordre.push_back("Zjets");
  ordre.push_back("DYToMuMu_M-20");
  ordre.push_back("DYToEE_M-20");
  ordre.push_back("DYToTauTau_M-20");
  ordre.push_back("DYToMuMu_M-10To20");
  ordre.push_back("DYToEE_M-10To20");
  ordre.push_back("DYToTauTau_M-10To20");
  ordre.push_back("TtW");
  ordre.push_back("TbartW");
  ordre.push_back("SingleToptW");
  ordre.push_back("WW");
  ordre.push_back("WZ");
  ordre.push_back("ZZ");
  ordre.push_back("VV");
  ordre.push_back("TTbarSig");
  
  THStack* hS_ee = new THStack();
  THStack* hS_HInvM_ll_pair_ee = new THStack();
  THStack* hS_mumu = new THStack();
  THStack* hS_HInvM_ll_pair_mumu = new THStack();
  
  THStack* hS_HInvM_emu_pair = new THStack();
  THStack* hS_HInvM_emu_pair_afterbtag1 = new THStack();
  THStack* hS_HInvM_emu_pair_afterbtag2 = new THStack();
  THStack* hS_HNjets_emu = new THStack();
  THStack* hS_HMet_emu = new THStack();
  THStack* hS_HNBjets_emu = new THStack();
  THStack* hS_HPtLpt1_emu = new THStack();
  THStack* hS_HPtLpt2_emu = new THStack();
  THStack* hS_HPtJet1_emu = new THStack();
  THStack* hS_HPtJet2_emu = new THStack();
  THStack* hS_HNVxP_emu = new THStack();
  THStack* hS_HInvM_emu_pair_Zemu = new THStack();

  THStack* hS_HInvM_emu_pair_aftermetcut = new THStack();
  THStack* hS_HNjets_emu_aftermetcut = new THStack();
  THStack* hS_HMet_emu_aftermetcut = new THStack();
  THStack* hS_HNBjets_emu_aftermetcut = new THStack();
  THStack* hS_HPtLpt1_emu_aftermetcut = new THStack();
  THStack* hS_HPtLpt2_emu_aftermetcut = new THStack();
  THStack* hS_HPtJet1_emu_aftermetcut = new THStack();
  THStack* hS_HPtJet2_emu_aftermetcut = new THStack();
  THStack* hS_HNVxP_emu_aftermetcut = new THStack();

  THStack* hS_HInvM_emu_pair_afterjetcut = new THStack();
  THStack* hS_HNjets_emu_afterjetcut = new THStack();
  THStack* hS_HMet_emu_afterjetcut = new THStack();
  THStack* hS_HNBjets_emu_afterjetcut = new THStack();
  THStack* hS_HPtLpt1_emu_afterjetcut = new THStack();
  THStack* hS_HPtLpt2_emu_afterjetcut = new THStack();
  THStack* hS_HPtJet1_emu_afterjetcut = new THStack();
  THStack* hS_HPtJet2_emu_afterjetcut = new THStack();
  
  
  THStack* hS_HInvM_mumu_pair = new THStack();
  THStack* hS_HInvM_mumu_pair_afterbtag1 = new THStack();
  THStack* hS_HInvM_mumu_pair_afterbtag2 = new THStack();
  THStack* hS_HNjets_mumu = new THStack();
  THStack* hS_HMet_mumu = new THStack();
  THStack* hS_HNBjets_mumu = new THStack();
  THStack* hS_HPtMu1_mumu = new THStack();
  THStack* hS_HPtMu2_mumu = new THStack();
  THStack* hS_HPtJet1_mumu = new THStack();
  THStack* hS_HPtJet2_mumu = new THStack();
  THStack* hS_HNVxP_mumu = new THStack();


  THStack* hS_HInvM_mumu_pair_aftermetcut = new THStack();
  THStack* hS_HNjets_mumu_aftermetcut = new THStack();
  THStack* hS_HMet_mumu_aftermetcut = new THStack();
  THStack* hS_HNBjets_mumu_aftermetcut = new THStack();
  THStack* hS_HPtMu1_mumu_aftermetcut = new THStack();
  THStack* hS_HPtMu2_mumu_aftermetcut = new THStack();
  THStack* hS_HPtJet1_mumu_aftermetcut = new THStack();
  THStack* hS_HPtJet2_mumu_aftermetcut = new THStack();
  THStack* hS_HNVxP_mumu_aftermetcut = new THStack();

  THStack* hS_HInvM_mumu_pair_afterjetcut = new THStack();
  THStack* hS_HNjets_mumu_afterjetcut = new THStack();
  THStack* hS_HMet_mumu_afterjetcut = new THStack();
  THStack* hS_HNBjets_mumu_afterjetcut = new THStack();
  THStack* hS_HPtMu1_mumu_afterjetcut = new THStack();
  THStack* hS_HPtMu2_mumu_afterjetcut = new THStack();
  THStack* hS_HPtJet1_mumu_afterjetcut = new THStack();
  THStack* hS_HPtJet2_mumu_afterjetcut = new THStack();
  
  THStack* hS_HInvM_mumu_pair_afterveto = new THStack();
  THStack* hS_HNjets_mumu_afterveto = new THStack();
  THStack* hS_HMet_mumu_afterveto = new THStack();
  THStack* hS_HNBjets_mumu_afterveto = new THStack();
  THStack* hS_HPtMu1_mumu_afterveto = new THStack();
  THStack* hS_HPtMu2_mumu_afterveto = new THStack();
  THStack* hS_HPtJet1_mumu_afterveto = new THStack();
  THStack* hS_HPtJet2_mumu_afterveto = new THStack();
  


  THStack* hS_HInvM_ee_pair = new THStack();
  THStack* hS_HInvM_ee_pair_afterbtag1 = new THStack();
  THStack* hS_HInvM_ee_pair_afterbtag2 = new THStack();
  THStack* hS_HNjets_ee = new THStack();
  THStack* hS_HMet_ee = new THStack();
  THStack* hS_HNBjets_ee = new THStack();
  THStack* hS_HPtEl1_ee = new THStack();
  THStack* hS_HPtEl2_ee = new THStack();
  THStack* hS_HPtJet1_ee = new THStack();
  THStack* hS_HPtJet2_ee = new THStack();
  THStack* hS_HNVxP_ee = new THStack();

  THStack* hS_HInvM_ee_pair_aftermetcut = new THStack();
  THStack* hS_HNjets_ee_aftermetcut = new THStack();
  THStack* hS_HMet_ee_aftermetcut = new THStack();
  THStack* hS_HNBjets_ee_aftermetcut = new THStack();
  THStack* hS_HPtEl1_ee_aftermetcut = new THStack();
  THStack* hS_HPtEl2_ee_aftermetcut = new THStack();
  THStack* hS_HPtJet1_ee_aftermetcut = new THStack();
  THStack* hS_HPtJet2_ee_aftermetcut = new THStack();
  THStack* hS_HNVxP_ee_aftermetcut = new THStack();
    
  THStack* hS_HInvM_ee_pair_afterjetcut = new THStack();
  THStack* hS_HNjets_ee_afterjetcut = new THStack();
  THStack* hS_HMet_ee_afterjetcut = new THStack();
  THStack* hS_HNBjets_ee_afterjetcut = new THStack();
  THStack* hS_HPtEl1_ee_afterjetcut = new THStack();
  THStack* hS_HPtEl2_ee_afterjetcut = new THStack();
  THStack* hS_HPtJet1_ee_afterjetcut = new THStack();
  THStack* hS_HPtJet2_ee_afterjetcut = new THStack();

  THStack* hS_HInvM_ee_pair_afterveto = new THStack();
  THStack* hS_HNjets_ee_afterveto = new THStack();
  THStack* hS_HMet_ee_afterveto = new THStack();
  THStack* hS_HNBjets_ee_afterveto = new THStack();
  THStack* hS_HPtEl1_ee_afterveto = new THStack();
  THStack* hS_HPtEl2_ee_afterveto = new THStack();
  THStack* hS_HPtJet1_ee_afterveto = new THStack();
  THStack* hS_HPtJet2_ee_afterveto = new THStack();

  THStack* hS_HMet_emu_afterbtag1 = new THStack();
  THStack* hS_HMet_emu_afterbtag2 = new THStack();
  THStack* hS_HMet_mumu_afterbtag1 = new THStack();
  THStack* hS_HMet_mumu_afterbtag2 = new THStack();
  THStack* hS_HMet_ee_afterbtag1 = new THStack();
  THStack* hS_HMet_ee_afterbtag2 = new THStack();

  MyhistoManager.StackHisto(hS_mumu,MyHistos_mumu,"InvDilMassNJinf1_mumu",ordre);
  MyhistoManager.StackHisto(hS_ee,MyHistos_ee,"InvDilMassNJinf1_ee",ordre);
  
  THStack* hS_HHT_ee_aftermetbtag1= new THStack();
  THStack* hS_HHT_emu_aftermetbtag1= new THStack();
  THStack* hS_HHT_mumu_aftermetbtag1= new THStack();
  
  THStack* hS_HHT_ee_aftermetbtag2= new THStack();
  THStack* hS_HHT_emu_aftermetbtag2= new THStack();
  THStack* hS_HHT_mumu_aftermetbtag2= new THStack();
  
/*
  string dytype = ""; 
  if (ChannelName=="ee" ) dytype = "DYee";
  if (ChannelName=="mumu" ) dytype = "DYmumu";
*/
  
  //  MyhistoManager.MergeHistos(MyHistos,"Zjets",dytype);
  //  MyhistoManager.MergeHistos(HInvM_ll_pair,"Zjets",dytype);
  MyhistoManager.StackHisto(hS_ee,              MyHistos_ee,     "InvDilMassNJinf1_ee",ordre);
  MyhistoManager.StackHisto(hS_HInvM_ll_pair_ee,HInvM_ll_pair_ee,"InvDilMassPair_ee"  ,ordre);
  MyhistoManager.StackHisto(hS_mumu,              MyHistos_mumu,     "InvDilMassNJinf1_mumu",ordre);
  MyhistoManager.StackHisto(hS_HInvM_ll_pair_mumu,HInvM_ll_pair_mumu,"InvDilMassPair_mumu"  ,ordre);
  
  
  MyhistoManager.StackHisto(hS_HInvM_emu_pair            ,HInvM_emu_pair            ,"InvemuMassPair",ordre);
  MyhistoManager.StackHisto(hS_HInvM_emu_pair_afterbtag1,HInvM_emu_pair_afterbtag1,"InvemuMassPair_afterbtag1",ordre);
  MyhistoManager.StackHisto(hS_HInvM_emu_pair_afterbtag2,HInvM_emu_pair_afterbtag2,"InvemuMassPair_afterbtag2",ordre);
  MyhistoManager.StackHisto(hS_HNjets_emu                ,HNjets_emu                ,"Njets_emu",ordre);
  MyhistoManager.StackHisto(hS_HMet_emu                  ,HMet_emu                  ,"Met_emu",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_emu               ,HNBjets_emu               ,"NBjets_emu",ordre);
  MyhistoManager.StackHisto(hS_HPtLpt1_emu               ,HPtLpt1_emu		   ,"PtLpt1_emu",ordre);
  MyhistoManager.StackHisto(hS_HPtLpt2_emu               ,HPtLpt2_emu		   ,"PtLpt2_emu",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_emu               ,HPtJet1_emu		   ,"PtJet1_emu",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_emu               ,HPtJet2_emu		   ,"PtJet2_emu",ordre);
  MyhistoManager.StackHisto(hS_HNVxP_emu               ,HNVxP_emu		   ,"NVxP_emu",ordre);
  MyhistoManager.StackHisto(hS_HInvM_emu_pair_Zemu,HInvM_emu_pair_Zemu,"InvemuMassPair_Zemu",ordre);
  
  MyhistoManager.StackHisto(hS_HInvM_emu_pair_aftermetcut,HInvM_emu_pair_aftermetcut,"InvemuMassPair_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNjets_emu_aftermetcut    ,HNjets_emu_aftermetcut    ,"Njets_emu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HMet_emu_aftermetcut      ,HMet_emu_aftermetcut      ,"Met_emu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_emu_aftermetcut   ,HNBjets_emu_aftermetcut   ,"NBjets_emu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtLpt1_emu_aftermetcut   ,HPtLpt1_emu_aftermetcut  ,"PtLpt1_emu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtLpt2_emu_aftermetcut   ,HPtLpt2_emu_aftermetcut  ,"PtLpt2_emu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_emu_aftermetcut   ,HPtJet1_emu_aftermetcut  ,"PtJet1_emu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_emu_aftermetcut   ,HPtJet2_emu_aftermetcut  ,"PtJet2_emu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNVxP_emu_aftermetcut   ,HNVxP_emu_aftermetcut  ,"NVxP_emu_aftermetcut",ordre);
  
  MyhistoManager.StackHisto(hS_HInvM_emu_pair_afterjetcut,HInvM_emu_pair_afterjetcut,"InvemuMassPair_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HNjets_emu_afterjetcut    ,HNjets_emu_afterjetcut    ,"Njets_emu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HMet_emu_afterjetcut      ,HMet_emu_afterjetcut      ,"Met_emu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_emu_afterjetcut   ,HNBjets_emu_afterjetcut   ,"NBjets_emu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtLpt1_emu_afterjetcut   ,HPtLpt1_emu_afterjetcut  ,"PtLpt1_emu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtLpt2_emu_afterjetcut   ,HPtLpt2_emu_afterjetcut  ,"PtLpt2_emu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_emu_afterjetcut   ,HPtJet1_emu_afterjetcut  ,"PtJet1_emu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_emu_afterjetcut   ,HPtJet2_emu_afterjetcut  ,"PtJet2_emu_afterjetcut",ordre);
  
  MyhistoManager.StackHisto(hS_HInvM_mumu_pair,HInvM_mumu_pair,"InvmumuMassPair",ordre);
  MyhistoManager.StackHisto(hS_HInvM_mumu_pair_afterbtag1,HInvM_mumu_pair_afterbtag1,"InvmumuMassPair_afterbtag1",ordre);
  MyhistoManager.StackHisto(hS_HInvM_mumu_pair_afterbtag2,HInvM_mumu_pair_afterbtag2,"InvmumuMassPair_afterbtag2",ordre);
  MyhistoManager.StackHisto(hS_HNjets_mumu,HNjets_mumu,"Njets_mumu",ordre);
  MyhistoManager.StackHisto(hS_HMet_mumu,HMet_mumu,"Met_mumu",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_mumu,HNBjets_mumu,"NBjets_mumu",ordre);
  MyhistoManager.StackHisto(hS_HPtMu1_mumu               ,HPtMu1_mumu		   ,"PtMu1_mumu",ordre);
  MyhistoManager.StackHisto(hS_HPtMu2_mumu               ,HPtMu2_mumu		   ,"PtMu2_mumu",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_mumu              ,HPtJet1_mumu  	    ,"PtJet1_mumu",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_mumu              ,HPtJet2_mumu  	    ,"PtJet2_mumu",ordre);
  MyhistoManager.StackHisto(hS_HNVxP_mumu                ,HNVxP_mumu		   ,"NVxP_mumu",ordre);

  MyhistoManager.StackHisto(hS_HInvM_mumu_pair_aftermetcut,HInvM_mumu_pair_aftermetcut,"InvmumuMassPair_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNjets_mumu_aftermetcut,HNjets_mumu_aftermetcut,"Njets_mumu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HMet_mumu_aftermetcut,HMet_mumu_aftermetcut,"Met_mumu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_mumu_aftermetcut,HNBjets_mumu_aftermetcut,"NBjets_mumu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtMu1_mumu_aftermetcut   ,HPtMu1_mumu_aftermetcut  ,"PtMu1_mumu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtMu2_mumu_aftermetcut   ,HPtMu2_mumu_aftermetcut  ,"PtMu2_mumu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_mumu_aftermetcut  ,HPtJet1_mumu_aftermetcut  ,"PtJet1_mumu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_mumu_aftermetcut  ,HPtJet2_mumu_aftermetcut  ,"PtJet2_mumu_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNVxP_mumu_aftermetcut    ,HNVxP_mumu_aftermetcut  ,"NVxP_mumu_aftermetcut",ordre);

  MyhistoManager.StackHisto(hS_HInvM_mumu_pair_afterjetcut,HInvM_mumu_pair_afterjetcut,"InvmumuMassPair_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HNjets_mumu_afterjetcut,HNjets_mumu_afterjetcut,"Njets_mumu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HMet_mumu_afterjetcut,HMet_mumu_afterjetcut,"Met_mumu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_mumu_afterjetcut,HNBjets_mumu_afterjetcut,"NBjets_mumu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtMu1_mumu_afterjetcut   ,HPtMu1_mumu_afterjetcut  ,"PtMu1_mumu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtMu2_mumu_afterjetcut   ,HPtMu2_mumu_afterjetcut  ,"PtMu2_mumu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_mumu_afterjetcut  ,HPtJet1_mumu_afterjetcut  ,"PtJet1_mumu_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_mumu_afterjetcut  ,HPtJet2_mumu_afterjetcut  ,"PtJet2_mumu_afterjetcut",ordre);
  
  MyhistoManager.StackHisto(hS_HInvM_mumu_pair_afterveto,HInvM_mumu_pair_afterveto,"InvmumuMassPair_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HNjets_mumu_afterveto,HNjets_mumu_afterveto,"Njets_mumu_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HMet_mumu_afterveto,HMet_mumu_afterveto,"Met_mumu_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_mumu_afterveto,HNBjets_mumu_afterveto,"NBjets_mumu_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtMu1_mumu_afterveto   ,HPtMu1_mumu_afterveto  ,"PtMu1_mumu_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtMu2_mumu_afterveto   ,HPtMu2_mumu_afterveto  ,"PtMu2_mumu_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_mumu_afterveto  ,HPtJet1_mumu_afterveto  ,"PtJet1_mumu_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_mumu_afterveto  ,HPtJet2_mumu_afterveto  ,"PtJet2_mumu_afterveto",ordre);
  
  MyhistoManager.StackHisto(hS_HInvM_ee_pair,HInvM_ee_pair,"InveeMassPair",ordre);
  MyhistoManager.StackHisto(hS_HInvM_ee_pair_afterbtag1,HInvM_ee_pair_afterbtag1,"InveeMassPair_afterbtag1",ordre);
  MyhistoManager.StackHisto(hS_HInvM_ee_pair_afterbtag2,HInvM_ee_pair_afterbtag2,"InveeMassPair_afterbtag2",ordre);
  MyhistoManager.StackHisto(hS_HNjets_ee,HNjets_ee,"Njets_ee",ordre);
  MyhistoManager.StackHisto(hS_HMet_ee,HMet_ee,"Met_ee",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_ee,HNBjets_ee,"NBjets_ee",ordre);
  MyhistoManager.StackHisto(hS_HPtEl1_ee               ,HPtEl1_ee	       ,"PtEl1_ee",ordre);
  MyhistoManager.StackHisto(hS_HPtEl2_ee               ,HPtEl2_ee	       ,"PtEl2_ee",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_ee              ,HPtJet1_ee		,"PtJet1_ee",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_ee              ,HPtJet2_ee		,"PtJet2_ee",ordre);
  MyhistoManager.StackHisto(hS_HNVxP_ee                ,HNVxP_ee		   ,"NVxP_ee",ordre);

  MyhistoManager.StackHisto(hS_HInvM_ee_pair_aftermetcut,HInvM_ee_pair_aftermetcut,"InveeMassPair_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNjets_ee_aftermetcut,HNjets_ee_aftermetcut,"Njets_ee_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HMet_ee_aftermetcut,HMet_ee_aftermetcut,"Met_ee_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_ee_aftermetcut,HNBjets_ee_aftermetcut,"NBjets_ee_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtEl1_ee_aftermetcut   ,HPtEl1_ee_aftermetcut  ,"PtEl1_ee_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtEl2_ee_aftermetcut   ,HPtEl2_ee_aftermetcut  ,"PtEl2_ee_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_ee_aftermetcut  ,HPtJet1_ee_aftermetcut  ,"PtJet1_ee_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_ee_aftermetcut  ,HPtJet2_ee_aftermetcut  ,"PtJet2_ee_aftermetcut",ordre);
  MyhistoManager.StackHisto(hS_HNVxP_ee_aftermetcut    ,HNVxP_ee_aftermetcut  ,"NVxP_ee_aftermetcut",ordre);

  MyhistoManager.StackHisto(hS_HInvM_ee_pair_afterjetcut,HInvM_ee_pair_afterjetcut,"InveeMassPair_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HNjets_ee_afterjetcut,HNjets_ee_afterjetcut,"Njets_ee_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HMet_ee_afterjetcut,HMet_ee_afterjetcut,"Met_ee_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_ee_afterjetcut,HNBjets_ee_afterjetcut,"NBjets_ee_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtEl1_ee_afterjetcut   ,HPtEl1_ee_afterjetcut  ,"PtEl1_ee_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtEl2_ee_afterjetcut   ,HPtEl2_ee_afterjetcut  ,"PtEl2_ee_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_ee_afterjetcut  ,HPtJet1_ee_afterjetcut  ,"PtJet1_ee_afterjetcut",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_ee_afterjetcut  ,HPtJet2_ee_afterjetcut  ,"PtJet2_ee_afterjetcut",ordre);

  MyhistoManager.StackHisto(hS_HInvM_ee_pair_afterveto,HInvM_ee_pair_afterveto,"InveeMassPair_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HNjets_ee_afterveto,HNjets_ee_afterveto,"Njets_ee_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HMet_ee_afterveto,HMet_ee_afterveto,"Met_ee_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HNBjets_ee_afterveto,HNBjets_ee_afterveto,"NBjets_ee_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtEl1_ee_afterveto   ,HPtEl1_ee_afterveto  ,"PtEl1_ee_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtEl2_ee_afterveto   ,HPtEl2_ee_afterveto  ,"PtEl2_ee_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtJet1_ee_afterveto  ,HPtJet1_ee_afterveto  ,"PtJet1_ee_afterveto",ordre);
  MyhistoManager.StackHisto(hS_HPtJet2_ee_afterveto  ,HPtJet2_ee_afterveto  ,"PtJet2_ee_afterveto",ordre);
  
  MyhistoManager.StackHisto(hS_HMet_emu_afterbtag1      ,HMet_emu_afterbtag1      ,"Met_emu_afterbtag1",ordre);
  MyhistoManager.StackHisto(hS_HMet_emu_afterbtag2      ,HMet_emu_afterbtag2      ,"Met_emu_afterbtag2",ordre);
  MyhistoManager.StackHisto(hS_HMet_mumu_afterbtag1      ,HMet_mumu_afterbtag1      ,"Met_mumu_afterbtag1",ordre);
  MyhistoManager.StackHisto(hS_HMet_mumu_afterbtag2      ,HMet_mumu_afterbtag2      ,"Met_mumu_afterbtag2",ordre);
  MyhistoManager.StackHisto(hS_HMet_ee_afterbtag1      ,HMet_ee_afterbtag1      ,"Met_ee_afterbtag1",ordre);
  MyhistoManager.StackHisto(hS_HMet_ee_afterbtag2      ,HMet_ee_afterbtag2      ,"Met_ee_afterbtag2",ordre);
  MyhistoManager.StackHisto(hS_HHT_ee_aftermetbtag1  ,HHT_ee_aftermetbtag1 ,	"HHT_ee_aftermetbtag1", ordre);
  MyhistoManager.StackHisto(hS_HHT_emu_aftermetbtag1 ,HHT_emu_aftermetbtag1 ,	"HHT_emu_aftermetbtag1", ordre);
  MyhistoManager.StackHisto(hS_HHT_mumu_aftermetbtag1,HHT_mumu_aftermetbtag1 ,  "HHT_mumu_aftermetbtag1", ordre);

  MyhistoManager.StackHisto(hS_HHT_ee_aftermetbtag2   , HHT_ee_aftermetbtag2 ,   "HHT_ee_aftermetbtag2", ordre);
  MyhistoManager.StackHisto(hS_HHT_emu_aftermetbtag2  , HHT_emu_aftermetbtag2,   "HHT_emu_aftermetbtag2", ordre);
  MyhistoManager.StackHisto(hS_HHT_mumu_aftermetbtag2 , HHT_mumu_aftermetbtag2 , "HHT_mumu_aftermetbtag2", ordre);
  //endmodifdg
  
  for(int k0=0; k0<5; ++k0) {
    for(int k1=0; k1<101; ++k1) {
      for(int k2=0; k2<101; ++k2) {
        TabFlow2[k0][k1][k2]=sqrt(TabFlow2[k0][k1][k2]);
      }
    }
  }  
  
  //Compute total bckgd
  for(int k0=0; k0<5; ++k0) {
    for(int k1=1; k1<10; ++k1) {
      for(int k2=0; k2<50; ++k2) {
	TabFlow1[k0][50][k2] += TabFlow1[k0][k1][k2];
      }
    }
  } 
  
  for(int k0=0; k0<5; ++k0) {
    for(int k2=1; k2<51; ++k2) {
      for(int k1=1; k1<10; ++k1) {
	TabFlow2[k0][50][k2] += TabFlow2[k0][k1][k2]*TabFlow2[k0][k1][k2];
      }
      TabFlow2[k0][50][k2] = sqrt(TabFlow2[k0][50][k2]);
    }
  } 
  

// compute SF effect CARO
  float sf_trig_sum[3];
  float sf_id_sum[3];
  for (int ijl=0; ijl<3; ijl++) {
   if ((TabFlow1[ijl][0][10]+TabFlow1[ijl][50][10])>0)
      sf_trig_sum[ijl] = (TabFlow1[ijl][0][1]+TabFlow1[ijl][50][1])/(TabFlow1[ijl][0][10]+TabFlow1[ijl][50][10]);
   else sf_trig_sum[ijl] =0;
   if ((TabFlow1[ijl][0][11]+TabFlow1[ijl][50][11])>0)
      sf_id_sum[ijl]= (TabFlow1[ijl][0][2]+TabFlow1[ijl][50][2])/(TabFlow1[ijl][0][11]+TabFlow1[ijl][50][11]);
   else sf_id_sum[ijl] =0;
  }

  for(int k0=0; k0<3; ++k0) {
    for(int k2=0; k2<12; ++k2) {
      int ityp=k2;
      if (k2==10) ityp=50;
      if (k2==11) ityp=100;
      if (TabFlow1[k0][ityp][10]>0) TabFlow1[k0][ityp][10] = TabFlow1[k0][ityp][1]/TabFlow1[k0][ityp][10];
      if (TabFlow1[k0][ityp][11]>0) TabFlow1[k0][ityp][11] = TabFlow1[k0][ityp][2]/TabFlow1[k0][ityp][11];
    }
  } 

//
  
  
  for(int k0=0; k0<1; ++k0) {
    for(int k1=0; k1<6; ++k1) {
      for(int k2=0; k2<6; ++k2) {
	std::cout<<"channel,type,cut "<<k0<<" "<<k1<<" "<<k2<<" "<<TabFlow1[k0][k1][k2] <<" +- "<< TabFlow2[k0][k1][k2]<<std::endl;
      }
    }
  }  
  
  for(int k0=0; k0<1; ++k0) {
    for(int k2=0; k2<6; ++k2) {
      std::cout<<"total bkgd "<<" "<<TabFlow1[k0][50][k2] <<" +- "<< sqrt(TabFlow2[k0][50][k2])<<std::endl;
    }
  }  
  
  
  
  cout<<"#########################"<<endl;
  cout<<" Fill the latex tables   "<<endl;
  cout<<"#########################"<<endl;
  
//  string ofilenametex = string("CrossSection")+string("_")+ChannelName+string(".tex");
//  string ofilenametex = string("CrossSection")+string(".tex");

  string ofilenametex;
  if(argc==3){
	 ofilenametex = argv[2];
         size_t ext_pos = ofilenametex.find_last_of( '.' );
//	 ofilenametex = argv[2]-string(".root")+string(".tex");
	 ofilenametex.replace( ext_pos + 1, ofilenametex.size() - 1, "tex" );
  }
  else ofilenametex = string("CrossSection")+string(".tex");

  ofstream ofile(ofilenametex.c_str());
  ofile<<"\\documentclass[amsmath,amssymb]{revtex4}"<<endl;
  ofile<<"\\begin{document}"<<endl;
  
  ofile.setf(ios::fixed);
  ofile.precision(3);
  
  for (int IChannel=0; IChannel<3; IChannel++) {
  // Summary tables
  ofile << "\\clearpage" << endl;
  ofile << "\\begin{table}[p]" << endl;
  
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & DATA & Sum MC & TT signal  & Total Background & S/B \\\\" << endl;
  ofile << "\\hline" << endl;
  
  for(int ic=0; ic<CutName.size(); ++ic) {
   if (IChannel<2) {
    ofile <<CutName[ic]<<" & "<<  TabFlow1[IChannel][100][ic] << " $\\pm$ "<<  TabFlow2[IChannel][100][ic] << " & " <<
     TabFlow1[IChannel][0][ic]+TabFlow1[IChannel][50][ic] << " $\\pm$ "<< 
     sqrt(TabFlow2[IChannel][0][ic]*TabFlow2[IChannel][0][ic]+TabFlow2[IChannel][50][ic]*TabFlow2[IChannel][50][ic]) << " & " <<
     TabFlow1[IChannel][0][ic] << " $\\pm$ "<< TabFlow2[IChannel][0][ic] << " & " <<
     TabFlow1[IChannel][50][ic] << " $\\pm$ "<< TabFlow2[IChannel][50][ic] << " & " <<
     TabFlow1[IChannel][0][ic]/TabFlow1[IChannel][50][ic] <<  " \\\\" << endl;
   }
   else {// emu
    ofile <<CutName2[ic]<<" & "<<  TabFlow1[IChannel][100][ic] << " $\\pm$ "<<  TabFlow2[IChannel][100][ic] << " & " <<
     TabFlow1[IChannel][0][ic]+TabFlow1[IChannel][50][ic] << " $\\pm$ "<< 
     sqrt(TabFlow2[IChannel][0][ic]*TabFlow2[IChannel][0][ic]+TabFlow2[IChannel][50][ic]*TabFlow2[IChannel][50][ic]) << " & " <<
     TabFlow1[IChannel][0][ic] << " $\\pm$ "<< TabFlow2[IChannel][0][ic] << " & " <<
     TabFlow1[IChannel][50][ic] << " $\\pm$ "<< TabFlow2[IChannel][50][ic] << " & " <<
     TabFlow1[IChannel][0][ic]/TabFlow1[IChannel][50][ic] <<  " \\\\" << endl;
   }
  }
  
  // summary table
  ofile << "\\hline" << endl;
// ajout Caro
  ofile << "$SF_{trig}$  & " << TabFlow1[IChannel][100][10] <<  " & " <<
      sf_trig_sum[IChannel] <<  " & " <<
      TabFlow1[IChannel][0][10] << " & " <<
      TabFlow1[IChannel][50][10] <<  " &  \\\\" << endl;
  ofile << "$SF_{ID}$  & " << TabFlow1[IChannel][100][11] << " & " <<
      sf_id_sum[IChannel] << " & " <<
      TabFlow1[IChannel][0][11] << " & " <<
      TabFlow1[IChannel][50][11] << " &  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
// end ajout Caro
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & TopBackg & SingleTop & DY & Wjets  & Diboson & QCD  \\\\" << endl;
  ofile << "\\hline" << endl;
  
  for(int ic=0; ic<CutName.size(); ++ic) {
    double TopBkgd    = TabFlow1[IChannel][1][ic];
    double ErrTopBkgd = TabFlow2[IChannel][1][ic];
    // to be changed
    //    double DY = DY + mll<50;
    double DY         = TabFlow1[IChannel][2][ic];
    double errDY      = (TabFlow2[IChannel][2][ic]*TabFlow2[IChannel][2][ic]);
    errDY             = sqrt(errDY);
    double Wj         = TabFlow1[IChannel][3][ic];
    double errWjets   = TabFlow2[IChannel][3][ic];
    double Qcd        = TabFlow1[IChannel][8][ic];
    double errQcd     = TabFlow2[IChannel][8][ic];
    double SinglTop    = TabFlow1[IChannel][4][ic];
    double errSinglTop = sqrt(TabFlow2[IChannel][4][ic]*TabFlow2[IChannel][4][ic]);
    double Dibos      =  TabFlow1[IChannel][5][ic];
    double errDibos   = TabFlow2[IChannel][5][ic];
   
    if (IChannel<2) {
     ofile <<CutName[ic]<<" & "<<
     TopBkgd         << " $\\pm$ "<< ErrTopBkgd      << " & " <<
     SinglTop        << " $\\pm$ "<< errSinglTop     << " & " <<
     DY              << " $\\pm$ "<< errDY           << " & " <<
     Wj              << " $\\pm$ "<< errWjets        << " & " <<
     Dibos           << " $\\pm$ "<< errDibos        << " & " <<
     Qcd             << " $\\pm$ "<< errQcd <<   " \\\\" << endl;
    }
    else { // emu
     ofile <<CutName2[ic]<<" & "<<
     TopBkgd         << " $\\pm$ "<< ErrTopBkgd      << " & " <<
     SinglTop        << " $\\pm$ "<< errSinglTop     << " & " <<
     DY              << " $\\pm$ "<< errDY           << " & " <<
     Wj              << " $\\pm$ "<< errWjets        << " & " <<
     Dibos           << " $\\pm$ "<< errDibos        << " & " <<
     Qcd             << " $\\pm$ "<< errQcd <<   " \\\\" << endl;
   } 
  }
  
  ofile << "\\hline " << endl;
// ajout Caro
  ofile << "$SF_{trig}$  & " << TabFlow1[IChannel][1][10] <<  " & "
     << TabFlow1[IChannel][4][10] <<  " & " 
     << TabFlow1[IChannel][2][10] <<  " & " 
     << TabFlow1[IChannel][3][10] <<  " & " 
     << TabFlow1[IChannel][5][10] <<  " & " 
     << TabFlow1[IChannel][8][10] <<  " \\\\" << endl;
  ofile << "$SF_{ID}$  & " << TabFlow1[IChannel][1][11] <<  " & "
     << TabFlow1[IChannel][4][11] <<  " & " 
     << TabFlow1[IChannel][2][11] <<  " & " 
     << TabFlow1[IChannel][3][11] <<  " & " 
     << TabFlow1[IChannel][5][11] <<  " & " 
     << TabFlow1[IChannel][8][11] <<  " \\\\" << endl;
  if (IChannel==0) ofile << "$SF_{DD}$  &  &  & " << SF_DY_mm << " & " << SF_Wjets_mm << " & & " << SF_QCD_mm << " \\\\" << endl;
  else if (IChannel==1) ofile << "$SF_{DD}$  &  &  & " << SF_DY_ee << " & " << SF_Wjets_ee << " & & " << SF_QCD_ee << " \\\\" << endl;
  else if (IChannel==2) ofile << "$SF_{DD}$  &  &  & " << SF_DY_em << " & " << SF_Wjets_em << " & & " << SF_QCD_em << " \\\\" << endl;
// end ajout Caro
  ofile << "\\hline " << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  
  string ChannelName;
  if (IChannel==0) ChannelName= "mumu"; 
  else if (IChannel==1) ChannelName= "ee"; 
  else if (IChannel==2) ChannelName= "emu"; 
  
  if ( ChannelName == "mumu" )  ofile << "\\caption{ Dimuon cut flow }" << endl;
  if ( ChannelName == "ee" )  ofile << "\\caption{ Dielectron cut flow }" << endl;
  if ( ChannelName == "emu" )  ofile << "\\caption{ ElectronMuon cut flow }" << endl;
  ofile << "\\label{Table:CutFlow}" << endl;
  ofile << "\\end{table}" << endl;
  } // end loop IChannel
  
  
  
  ofile<<"\\end{document}"<<endl;
//  string prodpdf = string("pdflatex CrossSection")+string("_")+ChannelName+string(".tex");
//  string prodpdf = string("pdflatex CrossSection")+string(".tex");
  string prodpdf = string("pdflatex ")+ofilenametex;
  system(prodpdf.c_str());
  
//CARO 
  cout<<"##############################"<<endl;
  cout<<" Save the info in ascii file  "<<endl;
  cout<<"##############################"<<endl;
  
  string ofilenameascii;
  if(argc==3){
	 ofilenameascii = argv[2];
         size_t ext_pos2 = ofilenameascii.find_last_of( '.' );
	 ofilenameascii.replace( ext_pos2 + 1, ofilenameascii.size() - 1, "txt" );
  }
  else ofilenameascii = string("CrossSection")+string(".txt");

  ofstream ofile2(ofilenameascii.c_str());
  ofile2.setf(ios::fixed);
  ofile2.precision(3);
  
  int jlist=-1;
  
  int index_jlist=0;
  for (int IChannel=0; IChannel<3; IChannel++) {
  // Summary tables
     if (TabFlow1[IChannel][100][0] > 0) { 
          jlist=100;
          ofile2 <<  "Data     " << IChannel <<   
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]   << endl;  // SF_trig && SF_ID
     }
     if (TabFlow2[IChannel][0][0] > 0)   { 
          jlist=0;
          ofile2 << "TTbarSig  " << IChannel << 
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]  << endl ;  // SF_trig && SF_ID
     }
     if (TabFlow2[IChannel][1][0] > 0)   { 
          jlist=1;
          ofile2 << "TopBkgd   " << IChannel <<
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]  << endl ;  // SF_trig && SF_ID
     }
     if (TabFlow2[IChannel][4][0] > 0)   { 
          jlist=4;
          ofile2 << "SingleTop " << IChannel <<
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]  << endl ;  // SF_trig && SF_ID
     }
     if (TabFlow2[IChannel][2][0] > 0)   { 
          jlist=2;
          ofile2 << "DY        " << IChannel <<
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]  << endl ;  // SF_trig && SF_ID
     }
     if (TabFlow2[IChannel][3][0] > 0)   { 
          jlist=3;
          ofile2 << "Wjet      " << IChannel <<
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]  << endl ;  // SF_trig && SF_ID
     }
     if (TabFlow2[IChannel][5][0] > 0)   { 
          jlist=5;
          ofile2 << "Diboson   " << IChannel <<
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]  << endl ;  // SF_trig && SF_ID
     }
     if (TabFlow2[IChannel][8][0] > 0)   { 
          jlist=8;
          ofile2 << "QCD       " << IChannel <<
                 "   " <<  TabFlow1[IChannel][jlist][0]  << "   " << TabFlow2[IChannel][jlist][0] << 
                 "   " <<  TabFlow1[IChannel][jlist][1]  << "   " << TabFlow2[IChannel][jlist][1] << 
                 "   " <<  TabFlow1[IChannel][jlist][2]  << "   " << TabFlow2[IChannel][jlist][2] << 
                 "   " <<  TabFlow1[IChannel][jlist][3]  << "   " << TabFlow2[IChannel][jlist][3] << 
                 "   " <<  TabFlow1[IChannel][jlist][4]  << "   " << TabFlow2[IChannel][jlist][4] << 
                 "   " <<  TabFlow1[IChannel][jlist][5]  << "   " << TabFlow2[IChannel][jlist][5] << 
                 "   " <<  TabFlow1[IChannel][jlist][6]  << "   " << TabFlow2[IChannel][jlist][6] << 
                 "   " <<  TabFlow1[IChannel][jlist][7]  << "   " << TabFlow2[IChannel][jlist][7] << 
                 "   " <<  TabFlow1[IChannel][jlist][10]    << "   " <<  TabFlow1[IChannel][jlist][11]  << endl ;  // SF_trig && SF_ID
     }

  } // end loop IChannel
 

 
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Write output root file "<<endl;
    cout<<"#########################"<<endl;
  }
//  string ofilename = string("CrossSection")+string("_")+ChannelName+string(".root");
  string ofilename;
  if(argc==3){
	 ofilename = argv[2];
  }
  else ofilename = string("CrossSection")+string(".root");
  TFile* fout  = new TFile(ofilename.c_str(),"RECREATE");

  if(producePLRPlots) plotsFurnisher.WriteHistos();
  
  
  
  MyhistoManager.WriteMyHisto(HHT_ee_aftermetbtag1,"all");
  MyhistoManager.WriteMyHisto(HHT_emu_aftermetbtag1,"all");
  MyhistoManager.WriteMyHisto(HHT_mumu_aftermetbtag1,"all");
  
  MyhistoManager.WriteMyHisto(HHT_ee_aftermetbtag2,"all");
  MyhistoManager.WriteMyHisto(HHT_emu_aftermetbtag2,"all");
  MyhistoManager.WriteMyHisto(HHT_mumu_aftermetbtag2,"all");
  
  
//  if ( ChannelName == "emu" ){
  MyhistoManager.WriteMyHisto(HInvM_ll_pair_mumu,"all");
  MyhistoManager.WriteMyHisto(HInvM_ll_pair_ee,"all");
  MyhistoManager.WriteMyHisto(HInvM_emu_pair,"all");
  MyhistoManager.WriteMyHisto(HInvM_emu_pairzoom,"all");
  MyhistoManager.WriteMyHisto(HInvM_emu_pair_afterbtag1,"all");
  MyhistoManager.WriteMyHisto(HInvM_emu_pair_afterbtag2,"all");
  MyhistoManager.WriteMyHisto(HInvM_emu_pair_aftermetbtag1,"all");
  MyhistoManager.WriteMyHisto(HInvM_emu_pair_aftermetbtag2,"all");
  MyhistoManager.WriteMyHisto(HNjets_emu,"all");
  MyhistoManager.WriteMyHisto(HMet_emu,"all");
  MyhistoManager.WriteMyHisto(HNBjets_emu,"all");
  MyhistoManager.WriteMyHisto(HPtLpt1_emu,"all");
  MyhistoManager.WriteMyHisto(HPtLpt2_emu,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_emu,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_emu,"all");
  MyhistoManager.WriteMyHisto(HNVxP_emu,"all");
  MyhistoManager.WriteMyHisto(HNVxP_emu_aftertrigger,"all");
  MyhistoManager.WriteMyHisto(HInvM_emu_pair_Zemu,"all");
  MyhistoManager.WriteMyHisto(HMet_emu_afterbtag1,"all");
  MyhistoManager.WriteMyHisto(HMet_emu_afterbtag2,"all");

  MyhistoManager.WriteMyHisto(HInvM_emu_pair_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNjets_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HMet_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNBjets_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtLpt1_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtLpt2_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNVxP_emu_aftermetcut,"all");

  MyhistoManager.WriteMyHisto(HInvM_emu_pair_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HNjets_emu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HMet_emu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HNBjets_emu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtLpt1_emu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtLpt2_emu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_emu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_emu_afterjetcut,"all");

  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_emu,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_emu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_emu_afterjetcut,"all");
//  }
  
//  if ( ChannelName == "mumu" ){
  MyhistoManager.WriteMyHisto(HInvM_mumu_pair,"all");
  MyhistoManager.WriteMyHisto(HInvM_mumu_pairzoom,"all");
  MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterbtag1,"all");
  MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterbtag2,"all");
  MyhistoManager.WriteMyHisto(HInvM_mumu_pair_aftermetbtag1,"all");
  MyhistoManager.WriteMyHisto(HInvM_mumu_pair_aftermetbtag2,"all");
  MyhistoManager.WriteMyHisto(HNjets_mumu,"all");
  MyhistoManager.WriteMyHisto(HMet_mumu,"all");
  MyhistoManager.WriteMyHisto(HNBjets_mumu,"all");
  MyhistoManager.WriteMyHisto(HPtMu1_mumu,"all");
  MyhistoManager.WriteMyHisto(HPtMu2_mumu,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_mumu,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_mumu,"all");
  MyhistoManager.WriteMyHisto(HNVxP_mumu,"all");
  MyhistoManager.WriteMyHisto(HNVxP_mumu_aftertrigger,"all");
  MyhistoManager.WriteMyHisto(HMet_mumu_afterbtag1,"all");
  MyhistoManager.WriteMyHisto(HMet_mumu_afterbtag2,"all");

  MyhistoManager.WriteMyHisto(HInvM_mumu_pair_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNjets_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HMet_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNBjets_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtMu1_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtMu2_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNVxP_mumu_aftermetcut,"all");

  MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HNjets_mumu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HMet_mumu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HNBjets_mumu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtMu1_mumu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtMu2_mumu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_mumu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_mumu_afterjetcut,"all");

  MyhistoManager.WriteMyHisto(HInvM_mumu_pair_afterveto,"all");
  MyhistoManager.WriteMyHisto(HNjets_mumu_afterveto,"all");
  MyhistoManager.WriteMyHisto(HMet_mumu_afterveto,"all");
  MyhistoManager.WriteMyHisto(HNBjets_mumu_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtMu1_mumu_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtMu2_mumu_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_mumu_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_mumu_afterveto,"all");

  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_mumu_afterveto,"all");
//  }
  
//  if ( ChannelName == "ee" ){
  MyhistoManager.WriteMyHisto(HInvM_ee_pair,"all");
  MyhistoManager.WriteMyHisto(HInvM_ee_pairzoom,"all");
  MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterbtag1,"all");
  MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterbtag2,"all");
  MyhistoManager.WriteMyHisto(HInvM_ee_pair_aftermetbtag1,"all");
  MyhistoManager.WriteMyHisto(HInvM_ee_pair_aftermetbtag2,"all");
  MyhistoManager.WriteMyHisto(HNjets_ee,"all");
  MyhistoManager.WriteMyHisto(HMet_ee,"all");
  MyhistoManager.WriteMyHisto(HNBjets_ee,"all");
  MyhistoManager.WriteMyHisto(HPtEl1_ee,"all");
  MyhistoManager.WriteMyHisto(HPtEl2_ee,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_ee,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_ee,"all");
  MyhistoManager.WriteMyHisto(HNVxP_ee,"all");
  MyhistoManager.WriteMyHisto(HNVxP_ee_aftertrigger,"all");
  MyhistoManager.WriteMyHisto(HMet_ee_afterbtag1,"all");
  MyhistoManager.WriteMyHisto(HMet_ee_afterbtag2,"all");
  MyhistoManager.WriteMyHisto(pileup,"all");

  MyhistoManager.WriteMyHisto(HInvM_ee_pair_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNjets_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HMet_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNBjets_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtEl1_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtEl2_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HNVxP_ee_aftermetcut,"all");

  MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HNjets_ee_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HMet_ee_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HNBjets_ee_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtEl1_ee_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtEl2_ee_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_ee_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_ee_afterjetcut,"all");

  MyhistoManager.WriteMyHisto(HInvM_ee_pair_afterveto,"all");
  MyhistoManager.WriteMyHisto(HNjets_ee_afterveto,"all");
  MyhistoManager.WriteMyHisto(HMet_ee_afterveto,"all");
  MyhistoManager.WriteMyHisto(HNBjets_ee_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtEl1_ee_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtEl2_ee_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtJet1_ee_afterveto,"all");
  MyhistoManager.WriteMyHisto(HPtJet2_ee_afterveto,"all");

  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee_aftermetcut,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee_afterjetcut,"all");
  MyhistoManager.WriteMyHisto(HDeltaPhiLMet_ee_afterveto,"all");
//  }
  
 
/*
  //write only if there are MC samples
  if(datasets.size()>0 && !datasets[0].isData()){ 
  hS_HInvM_ll_pair_ee->Write();
  hS_HInvM_ll_pair_mumu->Write();
//  if ( ChannelName == "emu" ){
  hS_HInvM_emu_pair->Write();
  hS_HInvM_emu_pair_afterbtag1->Write();
  hS_HInvM_emu_pair_afterbtag2->Write();
  hS_HNjets_emu->Write();
  hS_HMet_emu->Write();
  hS_HNBjets_emu->Write();
  hS_HPtLpt1_emu->Write();
  hS_HPtLpt2_emu->Write();
  hS_HPtJet1_emu->Write();
  hS_HPtJet2_emu->Write();
  hS_HNVxP_emu->Write();
  hS_HInvM_emu_pair_Zemu->Write();
  hS_HMet_emu_afterbtag1->Write();
  hS_HMet_emu_afterbtag2->Write();

  hS_HInvM_emu_pair_aftermetcut->Write();
  hS_HNjets_emu_aftermetcut->Write();
  hS_HMet_emu_aftermetcut->Write();
  hS_HNBjets_emu_aftermetcut->Write();
  hS_HPtLpt1_emu_aftermetcut->Write();
  hS_HPtLpt2_emu_aftermetcut->Write();
  hS_HPtJet1_emu_aftermetcut->Write();
  hS_HPtJet2_emu_aftermetcut->Write();
  hS_HNVxP_emu_aftermetcut->Write();

  hS_HInvM_emu_pair_afterjetcut->Write();
  hS_HNjets_emu_afterjetcut->Write();
  hS_HMet_emu_afterjetcut->Write();
  hS_HNBjets_emu_afterjetcut->Write();
  hS_HPtLpt1_emu_afterjetcut->Write();
  hS_HPtLpt2_emu_afterjetcut->Write();
  hS_HPtJet1_emu_afterjetcut->Write();
  hS_HPtJet2_emu_afterjetcut->Write();
//  }
  
//  if ( ChannelName == "mumu" ){
  hS_HInvM_mumu_pair->Write();
  hS_HInvM_mumu_pair_afterbtag1->Write();
  hS_HInvM_mumu_pair_afterbtag2->Write();
  hS_HNjets_mumu->Write();
  hS_HMet_mumu->Write();
  hS_HNBjets_mumu->Write();
  hS_HPtMu1_mumu->Write();
  hS_HPtJet1_mumu->Write();
  hS_HPtMu2_mumu_afterveto->Write();
  hS_HPtJet2_mumu->Write();
  hS_HNVxP_mumu->Write();
  hS_HMet_mumu_afterbtag1->Write();
  hS_HMet_mumu_afterbtag2->Write();

  hS_HInvM_mumu_pair_aftermetcut->Write();
  hS_HNjets_mumu_aftermetcut->Write();
  hS_HMet_mumu_aftermetcut->Write();
  hS_HNBjets_mumu_aftermetcut->Write();
  hS_HPtMu1_mumu_aftermetcut->Write();
  hS_HPtMu2_mumu_aftermetcut->Write();
  hS_HPtJet1_mumu_aftermetcut->Write();
  hS_HPtJet2_mumu_aftermetcut->Write();
  hS_HNVxP_mumu_aftermetcut->Write();

  hS_HInvM_mumu_pair_afterjetcut->Write();
  hS_HNjets_mumu_afterjetcut->Write();
  hS_HMet_mumu_afterjetcut->Write();
  hS_HNBjets_mumu_afterjetcut->Write();
  hS_HPtMu1_mumu_afterjetcut->Write();
  hS_HPtMu2_mumu_afterjetcut->Write();
  hS_HPtJet1_mumu_afterjetcut->Write();
  hS_HPtJet2_mumu_afterjetcut->Write();

  hS_HInvM_mumu_pair_afterveto->Write();
  hS_HNjets_mumu_afterveto->Write();
  hS_HMet_mumu_afterveto->Write();
  hS_HNBjets_mumu_afterveto->Write();
  hS_HPtMu1_mumu_afterveto->Write();
  hS_HPtMu2_mumu->Write();
  hS_HPtJet1_mumu_afterveto->Write();
  hS_HPtJet2_mumu_afterveto->Write();
//  }
  
  
//  if ( ChannelName == "ee" ){
  hS_HInvM_ee_pair->Write();
  hS_HInvM_ee_pair_afterbtag1->Write();
  hS_HInvM_ee_pair_afterbtag2->Write();
  hS_HNjets_ee->Write();
  hS_HMet_ee->Write();
  hS_HNBjets_ee->Write();
  hS_HPtEl1_ee->Write();
  hS_HPtEl2_ee->Write();
  hS_HPtJet1_ee->Write();
  hS_HPtJet2_ee->Write();
  hS_HNVxP_ee->Write();
  hS_HMet_ee_afterbtag1->Write();
  hS_HMet_ee_afterbtag2->Write();

  hS_HInvM_ee_pair_aftermetcut->Write();
  hS_HNjets_ee_aftermetcut->Write();
  hS_HMet_ee_aftermetcut->Write();
  hS_HNBjets_ee_aftermetcut->Write();
  hS_HPtEl1_ee_aftermetcut->Write();
  hS_HPtEl2_ee_aftermetcut->Write();
  hS_HPtJet1_ee_aftermetcut->Write();
  hS_HPtJet2_ee_aftermetcut->Write();
  hS_HNVxP_ee_aftermetcut->Write();

  hS_HInvM_ee_pair_afterjetcut->Write();
  hS_HNjets_ee_afterjetcut->Write();
  hS_HMet_ee_afterjetcut->Write();
  hS_HNBjets_ee_afterjetcut->Write();
  hS_HPtEl1_ee_afterjetcut->Write();
  hS_HPtEl2_ee_afterjetcut->Write();
  hS_HPtJet1_ee_afterjetcut->Write();
  hS_HPtJet2_ee_afterjetcut->Write();

  hS_HInvM_ee_pair_afterveto->Write();
  hS_HNjets_ee_afterveto->Write();
  hS_HMet_ee_afterveto->Write();
  hS_HNBjets_ee_afterveto->Write();
  hS_HPtEl1_ee_afterveto->Write();
  hS_HPtEl2_ee_afterveto->Write();
  hS_HPtJet1_ee_afterveto->Write();
  hS_HPtJet2_ee_afterveto->Write();
//  }
   }

*/

//DYestimate
  halllistemu_mumu->Write(); 
  halllistSR_mumu->Write();  
  halllistCR_mumu->Write();  
  halllistContCR_mumu->Write();  
  hDataSR_mumu->Write();     
  hDataCR_mumu->Write(); 
  hemudata_mumu->Write(); 
  NjetsDataCR_mumu->Write(); 
  NjetsDataSR_mumu->Write(); 
  NjetsMCCR_mumu->Write(); 
  NjetsMCSR_mumu->Write(); 
  DYMassDataCR_mumu->Write(); 
  DYMassDataSR_mumu->Write(); 
  DYMassMCCR_mumu->Write(); 
  DYMassMCSR_mumu->Write(); 
  for(unsigned int i=0; i<hlistemu_mumu.size(); i++) {
//     hlistemu_mumu[i]->Write();
     string n = hlistemu_mumu[i]->GetName();
     n += "_mumu";
     TH1F* GenericTH1F =  (TH1F *) hlistemu_mumu[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }
  for(unsigned int i=0; i<hlistContCR_mumu.size(); i++) {
//     hlistContCR_mumu[i]->Write();
     string n = hlistContCR_mumu[i]->GetName();
     n += "_mumu";
     TH1F* GenericTH1F =  (TH1F *) hlistContCR_mumu[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }
  for(unsigned int i=0; i<hlistSR_mumu.size();  i++) {
//     hlistSR_mumu[i]->Write();
     string n = hlistSR_mumu[i]->GetName();
     n += "_mumu";
     TH1F* GenericTH1F =  (TH1F *) hlistSR_mumu[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }  
  for(unsigned int i=0; i<hlistCR_mumu.size();  i++) { 
//     hlistCR_mumu[i]->Write();
     string n = hlistCR_mumu[i]->GetName();
     n += "_mumu";
     TH1F* GenericTH1F =  (TH1F *) hlistCR_mumu[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }   
  halllistemu_ee->Write(); 
  hemudata_emu->Write(); 
  halllistSR_ee->Write();  
  halllistCR_ee->Write();  
  halllistContCR_ee->Write();  
  hDataSR_ee->Write();     
  hDataCR_ee->Write(); 
  hemudata_ee->Write(); 
  NjetsDataCR_ee->Write(); 
  NjetsDataSR_ee->Write(); 
  NjetsMCCR_ee->Write(); 
  NjetsMCSR_ee->Write(); 
  DYMassDataCR_ee->Write(); 
  DYMassDataSR_ee->Write(); 
  DYMassMCCR_ee->Write(); 
  DYMassMCSR_ee->Write(); 
  for(unsigned int i=0; i<hlistemu_ee.size(); i++) {
//     hlistemu_ee[i]->Write();
     string n = hlistemu_ee[i]->GetName();
     n += "_ee";
     TH1F* GenericTH1F =  (TH1F *) hlistemu_ee[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }   
  for(unsigned int i=0; i<hlistContCR_ee.size(); i++) {
//     hlistContCR_ee[i]->Write();
     string n = hlistContCR_ee[i]->GetName();
     n += "_ee";
     TH1F* GenericTH1F =  (TH1F *) hlistContCR_ee[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }
  for(unsigned int i=0; i<hlistSR_ee.size();  i++) {
//     hlistSR_ee[i]->Write();
     string n = hlistSR_ee[i]->GetName();
     n += "_ee";
     TH1F* GenericTH1F =  (TH1F *) hlistSR_ee[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }   
  for(unsigned int i=0; i<hlistCR_ee.size();  i++) {
     hlistCR_ee[i]->Write();
     string n = hlistCR_ee[i]->GetName();
     n += "_ee";
     TH1F* GenericTH1F =  (TH1F *) hlistCR_ee[i]->Clone ();
     GenericTH1F->SetName(n.c_str());
     GenericTH1F->Write();
  }
  //endmodifdg
  //fout->Write();
  fout->Close();
  
  //Clear histos before deleting the TFile
  //  histoManager.Clear();  
  //  MyhistoManager.Clear();  
  
  delete fout;
  
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<"    End of the program   "<<endl;
    cout<<"#########################"<<endl;
  }
  
  return (0);
}
