#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"
#include "../../BckgdEstimation/interface/FakeRate_LeptEff.h"
#include "../../Tools/interface/TagAndProbe.h"
#include "../../Tools/interface/LumiReweightingStandAlone.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

using namespace TopTree;
using namespace std;

int main ()
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
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;

  //////////////////////

  string PUWeightFileName;
  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/TagAndProb.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  
  
  
  
  TopTree::NTEvent * event = 0;
  //Selection table
  // 4 tables: ee - emu - mumu - allChannels
  
   
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
  vector<string> CutName;
  vector<string> VecChannelName;
  
  // Here define the studied channel (ee/mumu/emu)
  string ChannelName  = "";
  //
  
  bool IReweightWithVxP = true;
  bool IReweight        = true;

  
  //////////////////////
 
  //////////////////////
  
  
  
    
  cout << "loading datasets " << endl;
  //Book keeping of my histos
  HistoManager MyhistoManager; 
  MyhistoManager.LoadDatasets(datasets);    
  cout << "datasets loaded " << endl;
  
  
  //modifdg
  std::vector<TH1F> MyHistos;
  std::vector<TH1F> MyHistos1;
  std::vector<TH1F> HInvM_ll_pair;
  std::vector<TH1F> HInvM_emu_pair;
  std::vector<TH1F> HInvM_emu_pair_aftermetcut;
  std::vector<TH1F> HNjets_aftermetcut;
  std::vector<TH1F> HMet;
  std::vector<TH1F> HMet_aftermetcut;
  std::vector<TH1F> HTriggerEff_afterDileptSel_pT;
  std::vector<TH1F> HTriggerEff_afterDileptSel_eta;
  std::vector<TH1F> HTriggerEff_afterDileptSel_phi;
  std::vector<TH1F> HTriggerEff_afterDileptSel_selTrig_pT;
  std::vector<TH1F> HTriggerEff_afterDileptSel_selTrig_eta;
  std::vector<TH1F> HTriggerEff_afterDileptSel_selTrig_phi;
  
  std::vector<TH1F> HNjets;
  std::vector<TH1F> HNjets_Loose;
  std::vector<TH1F> HNjets_Medium;
  
  std::vector<TH1F> HNjets_afterLeptSel;
  std::vector<TH1F> HNjets_Loose_afterLeptSel;
  std::vector<TH1F> HNjets_Medium_afterLeptSel;
  
  std::vector<TH1F> deltaR_Jet_Lepton_FullSel;
  std::vector<TH1F> deltaR_Jet_MET_FullSel;
  std::vector<TH1F> deltaR_Muon_MET_FullSel;
  std::vector<TH1F> deltaR_Elec_MET_FullSel;
  std::vector<TH1F> deltaR_Muon_Electron_FullSel;
  
  std::vector<TH1F> deltaPhi_Jet_Lepton_FullSel;
  std::vector<TH1F> deltaPhi_Jet_MET_FullSel;
  std::vector<TH1F> deltaPhi_Muon_MET_FullSel;
  std::vector<TH1F> deltaPhi_Elec_MET_FullSel;
  std::vector<TH1F> deltaPhi_DiLept_MET_FullSel;
  
  
  
  std::vector<TH1F> HTriggerEff_ElecSel_pT;
  std::vector<TH1F> HTriggerEff_ElecSel_eta;
  std::vector<TH1F> HTriggerEff_ElecSel_phi;
  std::vector<TH1F> HTriggerEff_ElecSel_selTrig_pT;
  std::vector<TH1F> HTriggerEff_ElecSel_selTrig_eta;
  std::vector<TH1F> HTriggerEff_ElecSel_selTrig_phi;
  
  
  std::vector<TH1F> HTriggerEff_MuonSel_pT;
  std::vector<TH1F> HTriggerEff_MuonSel_eta;
  std::vector<TH1F> HTriggerEff_MuonSel_phi;
  std::vector<TH1F> HTriggerEff_MuonSel_selTrig_pT;
  std::vector<TH1F> HTriggerEff_MuonSel_selTrig_eta;
  std::vector<TH1F> HTriggerEff_MuonSel_selTrig_phi;
  
  
  
  std::vector<TH2D> correlationBTAGMET_mumu;
  std::vector<TH2D> correlationMET_mumu;
  
  std::vector<TH2D> correlationBTAGMET_ee;
  std::vector<TH2D> correlationMET_ee;
  
  std::vector<TH2D> correlationBTAGMET_emu;
  std::vector<TH2D> correlationMET_emu;
  
  //*************************
  //*************************
  //*************************
  // for MC closure test
  
  
  std::vector<TH1F> genEl_Tight_pt;
  std::vector<TH1F> genEl_Tight_eta;
  std::vector<TH1F> genEl_Tight_phi;
  std::vector<TH1F> genEl_Tight_njet;
  
  std::vector<TH1F> genEl_LooseID_pt;
  std::vector<TH1F> genEl_LooseID_eta;
  std::vector<TH1F> genEl_LooseID_phi;
  std::vector<TH1F> genEl_LooseID_njet;
  
  std::vector<TH1F> genEl_LooseIso_pt;
  std::vector<TH1F> genEl_LooseIso_eta;
  std::vector<TH1F> genEl_LooseIso_phi;
  std::vector<TH1F> genEl_LooseIso_njet;
  
  
  std::vector<TH1F> genMu_Tight_pt;
  std::vector<TH1F> genMu_Tight_eta;
  std::vector<TH1F> genMu_Tight_phi;
  std::vector<TH1F> genMu_Tight_njet;
  
  std::vector<TH1F> genMu_LooseID_pt;
  std::vector<TH1F> genMu_LooseID_eta;
  std::vector<TH1F> genMu_LooseID_phi;
  std::vector<TH1F> genMu_LooseID_njet;
  
  std::vector<TH1F> genMu_LooseIso_pt;
  std::vector<TH1F> genMu_LooseIso_eta;
  std::vector<TH1F> genMu_LooseIso_phi;
  std::vector<TH1F> genMu_LooseIso_njet;
  
  //endmodifdg
  
  reweight::LumiReWeighting *LumiWeights;
  
  int ITypeMC	  = -1;
  int ICut	  = -1;  
  int IChannel    = -1;  
 
  
  
  // Matrix Method
  
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
  //////////////////////


  
 
 
 
  
  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Loop over the datasets  "<<endl;
	cout<<"#########################"<<endl;
  }

  for (unsigned int d = 0; d < datasets.size (); d++) {
    

   datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    
   
    
    TagAndProbe theEffPlots;
    theEffPlots.CreateHistos("muons",     datasets[d].Name());
    theEffPlots.CreateHistos("electrons", datasets[d].Name());
    
    MyhistoManager.CreateHisto(HTriggerEff_afterDileptSel_pT, "HTriggerEff_afterDileptSel_pT" ,datasets[d].Name(),"trig eff Pt"  ,"Entries", 60, 20, 300);
    MyhistoManager.CreateHisto(HTriggerEff_afterDileptSel_eta,"HTriggerEff_afterDileptSel_eta" ,datasets[d].Name(),"trig eff eta","Entries", 21,-2.5,2.5);   
    MyhistoManager.CreateHisto(HTriggerEff_afterDileptSel_phi,"HTriggerEff_afterDileptSel_phi" ,datasets[d].Name(),"trig eff phi","Entries", 20, -3.2, 3.2);
    
    
    MyhistoManager.CreateHisto(HTriggerEff_afterDileptSel_selTrig_pT, "HTriggerEff_afterDileptSel_selTrig_pT" ,datasets[d].Name(),"trig eff Pt"  ,"Entries", 60, 20, 300);
    MyhistoManager.CreateHisto(HTriggerEff_afterDileptSel_selTrig_eta,"HTriggerEff_afterDileptSel_selTrig_eta" ,datasets[d].Name(),"trig eff eta","Entries", 21,-2.5,2.5);
    MyhistoManager.CreateHisto(HTriggerEff_afterDileptSel_selTrig_phi,"HTriggerEff_afterDileptSel_selTrig_phi" ,datasets[d].Name(),"trig eff phi","Entries", 20, -3.2, 3.2);
    
    
    MyhistoManager.CreateHisto(HNjets,        "Njets"        ,datasets[d].Name(),"Njets","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HNjets_Loose,  "Njets_Loose"  ,datasets[d].Name(),"Njets_Loose","Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HNjets_Medium, "Njets_Medium" ,datasets[d].Name(),"Njets_Medium","Entries",4,-0.5,3.5);
    
    MyhistoManager.CreateHisto(HNjets_afterLeptSel,        "Njets_afterLeptSel"        ,datasets[d].Name(),"Njets_afterLeptSel"       ,"Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HNjets_Loose_afterLeptSel,  "Njets_Loose_afterLeptSel"  ,datasets[d].Name(),"Njets_Loose_afterLeptSel" ,"Entries",4,-0.5,3.5);
    MyhistoManager.CreateHisto(HNjets_Medium_afterLeptSel, "Njets_Medium_afterLeptSel" ,datasets[d].Name(),"Njets_Medium_afterLeptSel","Entries",4,-0.5,3.5);
    
    MyhistoManager.CreateHisto(deltaR_Jet_Lepton_FullSel   , "deltaR_Jet_Lepton_FullSel"   , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );         
    MyhistoManager.CreateHisto(deltaR_Jet_MET_FullSel      , "deltaR_Jet_MET_FullSel"      , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );         
    MyhistoManager.CreateHisto(deltaR_Muon_MET_FullSel     , "deltaR_Muon_MET_FullSel"     , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );           
    MyhistoManager.CreateHisto(deltaR_Elec_MET_FullSel     , "deltaR_Elec_MET_FullSel"     , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );       
    //MyhistoManager.CreateHisto(deltaR_Muon_Electron_FullSel, "deltaR_Muon_Electron_FullSel", datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );         
    MyhistoManager.CreateHisto(deltaPhi_Jet_Lepton_FullSel   , "deltaPhi_Jet_Lepton_FullSel"   , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );         
    MyhistoManager.CreateHisto(deltaPhi_Jet_MET_FullSel      , "deltaPhi_Jet_MET_FullSel"      , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );         
    MyhistoManager.CreateHisto(deltaPhi_Muon_MET_FullSel     , "deltaPhi_Muon_MET_FullSel"     , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );           
    MyhistoManager.CreateHisto(deltaPhi_Elec_MET_FullSel     , "deltaPhi_Elec_MET_FullSel"     , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 );       
              
    MyhistoManager.CreateHisto(deltaPhi_DiLept_MET_FullSel     , "deltaPhi_DiLept_MET_FullSel"     , datasets[d].Name(), "delta R", "Entries", 20, 0, 4 ); 
    
    
    
    
    
    MyhistoManager.CreateHisto(HTriggerEff_ElecSel_pT,  "HTriggerEff_ElecSel_pT",  datasets[d].Name(),"trig eff el Pt"  ,"Entries", 60, 20, 300);
    MyhistoManager.CreateHisto(HTriggerEff_ElecSel_eta, "HTriggerEff_ElecSel_eta", datasets[d].Name(),"trig eff el eta" ,"Entries", 21,-2.5,2.5);
    MyhistoManager.CreateHisto(HTriggerEff_ElecSel_phi, "HTriggerEff_ElecSel_phi", datasets[d].Name(),"trig eff el phi" ,"Entries", 20, -3.2, 3.2);
    MyhistoManager.CreateHisto(HTriggerEff_ElecSel_selTrig_pT,  "HTriggerEff_ElecSel_selTrig_pT",  datasets[d].Name(),"trig eff sel el Pt"  ,"Entries", 60, 20, 300);
    MyhistoManager.CreateHisto(HTriggerEff_ElecSel_selTrig_eta, "HTriggerEff_ElecSel_selTrig_eta", datasets[d].Name(),"trig eff sel el eta" ,"Entries", 21,-2.5,2.5);
    MyhistoManager.CreateHisto(HTriggerEff_ElecSel_selTrig_phi, "HTriggerEff_ElecSel_selTrig_phi", datasets[d].Name(),"trig eff sel el phi" ,"Entries", 20, -3.2, 3.2);
  
    
    MyhistoManager.CreateHisto(HTriggerEff_MuonSel_pT,  "HTriggerEff_MuonSel_pT",  datasets[d].Name(),"trig eff el Pt"  ,"Entries", 60, 20, 300);
    MyhistoManager.CreateHisto(HTriggerEff_MuonSel_eta, "HTriggerEff_MuonSel_eta", datasets[d].Name(),"trig eff el eta" ,"Entries", 21,-2.5,2.5);
    MyhistoManager.CreateHisto(HTriggerEff_MuonSel_phi, "HTriggerEff_MuonSel_phi", datasets[d].Name(),"trig eff el phi" ,"Entries", 20, -3.2, 3.2);
    MyhistoManager.CreateHisto(HTriggerEff_MuonSel_selTrig_pT,  "HTriggerEff_MuonSel_selTrig_pT",  datasets[d].Name(),"trig eff sel el Pt"  ,"Entries", 60, 20, 300);
    MyhistoManager.CreateHisto(HTriggerEff_MuonSel_selTrig_eta, "HTriggerEff_MuonSel_selTrig_eta", datasets[d].Name(),"trig eff sel el eta" ,"Entries", 21,-2.5,2.5);
    MyhistoManager.CreateHisto(HTriggerEff_MuonSel_selTrig_phi, "HTriggerEff_MuonSel_selTrig_phi", datasets[d].Name(),"trig eff sel el phi" ,"Entries", 20, -3.2, 3.2);
  
    //float ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 65, 80, 300};
    float ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 100};
  
    MyhistoManager.CreateHisto(genEl_Tight_pt,   "genEl_Tight_pt",   datasets[d].Name(),"eff gen el "  ,"Entries", 10, ptRange);
    MyhistoManager.CreateHisto(genEl_Tight_eta,  "genEl_Tight_eta",  datasets[d].Name(),"eff gen el "  ,"Entries", 40, -2.5 , 2.5);
    MyhistoManager.CreateHisto(genEl_Tight_phi,  "genEl_Tight_phi",  datasets[d].Name(),"eff gen el el "  ,"Entries", 40, -3.14, 3.14);
    MyhistoManager.CreateHisto(genEl_Tight_njet, "genEl_Tight_njet", datasets[d].Name(),"eff gen "  ,"Entries", 7,  -0.5 , 6.5);
  
    MyhistoManager.CreateHisto(genEl_LooseID_pt,   "genEl_LooseID_pt",   datasets[d].Name(),"eff gen el "  ,"Entries", 10, ptRange);
    MyhistoManager.CreateHisto(genEl_LooseID_eta,  "genEl_LooseID_eta",  datasets[d].Name(),"eff gen el "  ,"Entries", 40, -2.5 , 2.5);
    MyhistoManager.CreateHisto(genEl_LooseID_phi,  "genEl_LooseID_phi",  datasets[d].Name(),"eff gen el "  ,"Entries", 40, -3.14, 3.14);
    MyhistoManager.CreateHisto(genEl_LooseID_njet, "genEl_LooseID_njet", datasets[d].Name(),"eff gen el "  ,"Entries", 7,  -0.5 , 6.5);
  
    MyhistoManager.CreateHisto(genEl_LooseIso_pt,   "genEl_LooseIso_pt",   datasets[d].Name(),"eff gen el "  ,"Entries", 10, ptRange);
    MyhistoManager.CreateHisto(genEl_LooseIso_eta,  "genEl_LooseIso_eta",  datasets[d].Name(),"eff gen el "  ,"Entries", 40, -2.5 , 2.5);
    MyhistoManager.CreateHisto(genEl_LooseIso_phi,  "genEl_LooseIso_phi",  datasets[d].Name(),"eff gen el "  ,"Entries", 40, -3.14, 3.14);
    MyhistoManager.CreateHisto(genEl_LooseIso_njet, "genEl_LooseIso_njet", datasets[d].Name(),"eff gen el "  ,"Entries", 7,  -0.5 , 6.5);
  
  
  
    MyhistoManager.CreateHisto(genMu_Tight_pt,   "genMu_Tight_pt",   datasets[d].Name(),"eff gen mu "  ,"Entries", 10, ptRange);
    MyhistoManager.CreateHisto(genMu_Tight_eta,  "genMu_Tight_eta",  datasets[d].Name(),"eff gen mu "  ,"Entries", 40, -2.5 , 2.5);
    MyhistoManager.CreateHisto(genMu_Tight_phi,  "genMu_Tight_phi",  datasets[d].Name(),"eff gen mu "  ,"Entries", 40, -3.14, 3.14);
    MyhistoManager.CreateHisto(genMu_Tight_njet, "genMu_Tight_njet", datasets[d].Name(),"eff gen mu "  ,"Entries", 7,  -0.5 , 6.5);
  
    MyhistoManager.CreateHisto(genMu_LooseID_pt,   "genMu_LooseID_pt",   datasets[d].Name(),"eff gen mu "  ,"Entries", 10, ptRange);
    MyhistoManager.CreateHisto(genMu_LooseID_eta,  "genMu_LooseID_eta",  datasets[d].Name(),"eff gen mu "  ,"Entries", 40, -2.5 , 2.5);
    MyhistoManager.CreateHisto(genMu_LooseID_phi,  "genMu_LooseID_phi",  datasets[d].Name(),"eff gen mu "  ,"Entries", 40, -3.14, 3.14);
    MyhistoManager.CreateHisto(genMu_LooseID_njet, "genMu_LooseID_njet", datasets[d].Name(),"eff gen mu "  ,"Entries", 7,  -0.5 , 6.5);
  
    MyhistoManager.CreateHisto(genMu_LooseIso_pt,   "genMu_LooseIso_pt",   datasets[d].Name(),"eff gen mu "  ,"Entries", 10, ptRange);
    MyhistoManager.CreateHisto(genMu_LooseIso_eta,  "genMu_LooseIso_eta",  datasets[d].Name(),"eff gen mu "  ,"Entries", 40, -2.5 , 2.5);
    MyhistoManager.CreateHisto(genMu_LooseIso_phi,  "genMu_LooseIso_phi",  datasets[d].Name(),"eff gen mu "  ,"Entries", 40, -3.14, 3.14);
    MyhistoManager.CreateHisto(genMu_LooseIso_njet, "genMu_LooseIso_njet", datasets[d].Name(),"eff gen mu "  ,"Entries", 7,  -0.5 , 6.5);
  
  
 
    MyhistoManager.CreateHisto2D(correlationBTAGMET_mumu, "correlationBTAGMET_mumu", datasets[d].Name(),"NTrigBTAGMET "  , 2, -0.5 , 1.5, "NTrigLept", 2, -0.5, 1.5);
    MyhistoManager.CreateHisto2D(correlationMET_mumu, "correlationMET_mumu", datasets[d].Name(),"NTrigMET "   , 2, -0.5 , 1.5, "NTrigLept", 2, -0.5, 1.5);
  
    MyhistoManager.CreateHisto2D(correlationBTAGMET_ee, "correlationBTAGMET_ee", datasets[d].Name(),"NTrigBTAGMET "  , 2, -0.5 , 1.5, "NTrigLept", 2, -0.5, 1.5);
    MyhistoManager.CreateHisto2D(correlationMET_ee, "correlationMET_ee", datasets[d].Name(),"NTrigMET "   , 2, -0.5 , 1.5, "NTrigLept", 2, -0.5, 1.5);
  
    MyhistoManager.CreateHisto2D(correlationBTAGMET_emu, "correlationBTAGMET_emu", datasets[d].Name(),"NTrigBTAGMET "  , 2, -0.5 , 1.5, "NTrigLept", 2, -0.5, 1.5);
    MyhistoManager.CreateHisto2D(correlationMET_emu, "correlationMET_emu", datasets[d].Name(),"NTrigMET "   , 2, -0.5 , 1.5, "NTrigLept", 2, -0.5, 1.5);
  
   //gSystem->Load("libNTuple.so");
   if (IReweight ) {
 
       string datafile = "/storage1/cms/jandrea/TopIPHC_2012_01_16/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/default73.5mb.root";
       string mcfile   = "/storage1/cms/jandrea/TopIPHC_2012_01_16/CMSSW_4_2_8_patch7/src/MiniTreeAnalysis/NTupleAnalysis/macros/data/PU3DMC.root";
       //cout << "loading  LumiWeights" <<  endl;
    
       LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
       LumiWeights->weight3D_init( 1. );
      

   }
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
      if (verbosity > 3)
	//cout << "event " << ievt << endl;
      if (verbosity > 3)
	//cout << "event number=" << event->eventNb << endl;
      if (verbosity > 3)
	//cout << "run number=" << event->runNb << endl;
     
     cout << "event number=" << event->eventNb << endl;
     if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
     //Load event for the selection
     //if(ievt == 3000000) break;
     
     
       
     //********************************
     //**** check trigger paths
     //********************************
     
      //Dweight[ITypeMC] = Luminosity*datasets[d].Xsection()/datasets[d].NofEvtsToRunOver();
      //Dweight[ITypeMC] = Luminosity*datasets[d].Xsection()/datasets[d].nEvents;
      //if ( datasets[d].Name()=="Data" || datasets[d].Name()=="DATA") Dweight[ITypeMC] = 1;
     
     
     
    
     //cout << " pass trigger TriggerPassed_mumu_DATA " << TriggerPassed_mumu_DATA << endl;
     //cout << " pass trigger TriggerPassed_ee_DATA " << TriggerPassed_ee_DATA << endl;
     //std::ostream & ostrigger:
     //if(ievt == 1) event->PrintTriggerList (std::cout);
     //if(ievt == 1) event->  PrintTriggerPassed(std::cout);
      
      bool IsSignal = false;
      int  LastStep = 0;
     
     
      //if(!IsData) cout  << TriggerPassed_ee_MC << "  sel.eleHLTMatch " << sel.eleHLTMatch << endl;
    
     sel.LoadEvent(event);

     //Collection of selected objects
     //cout << " get selected electron " << endl;
     vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
     //cout << " get selected muons  " << endl;
     vector<NTMuon>     selMuons = sel.GetSelectedMuons();
     //cout << " get selected jets " << endl;
     
     vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
     vector<NTJet>      selJets = sel.GetSelectedJets();
     NTMET met = sel.GetMET(); // no criteria applyied
     
     //fill histo for efficiencies
     vector<NTMuon>      selMuonsNonIso = sel.GetSelectedMuonsNoIso();
     vector<NTElectron>  selElecsNonIso = sel.GetSelectedElectronsNoIso();
     theEffPlots.SetLooseIsoMuonCollection(     selMuonsNonIso );
     theEffPlots.SetLooseIsoElectronCollection( selElecsNonIso );
     //theEffPlots.SetLooseIsolectronCollection( selElecsNonIso );
     vector<NTMuon>      selMuonsNonID  = sel.GetSelectedMuonIsoNonID();
     vector<NTElectron>  selElecsNonID  = sel.GetSelectedElectronsIsoNonID();
     
     
     
     
     //Manage DY samples to avoid overlaps
      double dileptInvMass = 0;
      if( (event->zAndDecays).size() > 0){
        TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
	dileptInvMass = dilept.M();
      }
      
//      std::cout<<"dileptInvMass "<<dileptInvMass<<std::endl;
      
//      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50 ) cout << "problem !!!" << endl;
       
      
      
     
     
     
     
      double weightITypeMC_save = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
      double weightITypeMC=0;
 
     
     
     
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
          datasets[d].Name()!="DataMuEG" && datasets[d].Name()!="DataEGMu" && 
          datasets[d].Name()!="MET1" && datasets[d].Name()!="MET2") {
          
	  
	  if(IReweight ){
	        weightITypeMC = weightITypeMC_save*LumiWeights->weight3D(event->num_pileup_bcm1 ,event->num_pileup_bc0,event->num_pileup_bcp1);
               //cout << "in if reweight " << endl;
	       //cout << "in if reweight " << weightITypeMC_save << endl;
	       //cout << "in if reweight " << LumiWeights->weight3D(event->num_pileup_bcm1
	       //,event->num_pileup_bc0,event->num_pileup_bcp1 )<< endl;
	 }
       }
       else { // data   
            weightITypeMC = 1;
       }
       
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
          }
	  
	  
	  
       bool passTrigger = sel.passTriggerSelection ( &datasets[d], ChannelName);
       theEffPlots.SetLooseIDMuonCollection(      selMuonsNonID  );
       theEffPlots.SetLooseIDElectronCollection(  selElecsNonID  );
       
       theEffPlots.SetTightMuonCollection(	 selMuons     );
       theEffPlots.SetTightElectronCollection( selElectrons );
       //cout << "weightITypeMC " << weightITypeMC << endl;
       if(passTrigger) theEffPlots.FillHistos(selJets, "muons",     datasets[d].Name(), weightITypeMC);
       if(passTrigger) theEffPlots.FillHistos(selJets, "electrons", datasets[d].Name(), weightITypeMC);
       
       if(verbosity > 2) {
	 //cout<<event->electrons.size()<<" electrons - "<<event->muons.size()<<" muons - "<<endl;
	 //cout<<selElectrons.size()<<" sel electrons - "<<selMuons.size()<<" sel muons - "<<endl;
       }
       
    
      
       //*************************************
       //get sel MC truth lept sel efficiency
       //for muons
       
       if( passTrigger) {
	 for(unsigned int imu = 0; imu<selMuons.size(); imu++){
	   if(selMuons[imu].LeptonOrigin != 10 && selMuons[imu].LeptonOrigin != 1 ) continue;
	   MyhistoManager.FillHisto(genMu_Tight_pt,   "genMu_Tight_pt",   selMuons[imu].p4.Pt() , datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genMu_Tight_eta,  "genMu_Tight_eta",  selMuons[imu].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genMu_Tight_phi,  "genMu_Tight_phi",  selMuons[imu].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   if(imu == 0) MyhistoManager.FillHisto(genMu_Tight_njet, "genMu_Tight_njet", selJets.size(), datasets[d].Name(), IsSignal, weightITypeMC);
	 }
	 for(unsigned int imu = 0; imu<selMuonsNonID.size(); imu++){
	   if(selMuonsNonID[imu].LeptonOrigin != 10 && selMuonsNonID[imu].LeptonOrigin != 1 ) continue;
	   MyhistoManager.FillHisto(genMu_LooseID_pt,   "genMu_LooseID_pt",  selMuonsNonID[imu].p4.Pt() , datasets[d].Name(), IsSignal, weightITypeMC);  
	   MyhistoManager.FillHisto(genMu_LooseID_eta,  "genMu_LooseID_eta", selMuonsNonID[imu].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genMu_LooseID_phi,  "genMu_LooseID_phi", selMuonsNonID[imu].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   if(imu == 0) MyhistoManager.FillHisto(genMu_LooseID_njet, "genMu_LooseID_njet",selJets.size(), datasets[d].Name(), IsSignal, weightITypeMC);
	 }
	 
	 for(unsigned int imu = 0; imu<selMuonsNonIso.size(); imu++){
	   if(selMuonsNonIso[imu].LeptonOrigin != 10 && selMuonsNonIso[imu].LeptonOrigin != 1 ) continue;
	   MyhistoManager.FillHisto(genMu_LooseIso_pt,   "genMu_LooseIso_pt",  selMuonsNonIso[imu].p4.Pt() , datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genMu_LooseIso_eta,  "genMu_LooseIso_eta", selMuonsNonIso[imu].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC); 
	   MyhistoManager.FillHisto(genMu_LooseIso_phi,  "genMu_LooseIso_phi", selMuonsNonIso[imu].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genMu_LooseIso_njet, "genMu_LooseIso_njet",selJets.size(), datasets[d].Name(), IsSignal, 1);
	 }
       }
       
       
       
       //*************************************
       //get sel MC truth lept sel efficiency
       //for electrons
       if( passTrigger ){
	 for(unsigned int iel = 0; iel<selElectrons.size(); iel++){
	   if(selElectrons[iel].LeptonOrigin != 10 && selElectrons[iel].LeptonOrigin != 1 ) continue;
	   MyhistoManager.FillHisto(genEl_Tight_pt,   "genEl_Tight_pt",   selElectrons[iel].p4.Pt() , datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genEl_Tight_eta,  "genEl_Tight_eta",  selElectrons[iel].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genEl_Tight_phi,  "genEl_Tight_phi",  selElectrons[iel].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   if(iel == 0) MyhistoManager.FillHisto(genEl_Tight_njet, "genEl_Tight_njet", selJets.size(), datasets[d].Name(), IsSignal, weightITypeMC);
	 }
	 for(unsigned int iel = 0; iel<selElecsNonID.size(); iel++){
	   if(selElecsNonID[iel].LeptonOrigin != 10 && selElecsNonID[iel].LeptonOrigin != 1 ) continue;
	   MyhistoManager.FillHisto(genEl_LooseID_pt,   "genEl_LooseID_pt",  selElecsNonID[iel].p4.Pt() , datasets[d].Name(), IsSignal, weightITypeMC);  
	   MyhistoManager.FillHisto(genEl_LooseID_eta,  "genEl_LooseID_eta", selElecsNonID[iel].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genEl_LooseID_phi,  "genEl_LooseID_phi", selElecsNonID[iel].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   if(iel == 0) MyhistoManager.FillHisto(genEl_LooseID_njet, "genEl_LooseID_njet",selJets.size(), datasets[d].Name(), IsSignal, weightITypeMC);
	 }
	 
	 for(unsigned int iel = 0; iel<selElecsNonIso.size(); iel++){
	   if(selElecsNonIso[iel].LeptonOrigin != 10 && selElecsNonIso[iel].LeptonOrigin != 1 ) continue;
	   MyhistoManager.FillHisto(genEl_LooseIso_pt,   "genEl_LooseIso_pt",  selElecsNonIso[iel].p4.Pt() , datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genEl_LooseIso_eta,  "genEl_LooseIso_eta", selElecsNonIso[iel].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC); 
	   MyhistoManager.FillHisto(genEl_LooseIso_phi,  "genEl_LooseIso_phi", selElecsNonIso[iel].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(genEl_LooseIso_njet, "genEl_LooseIso_njet",selJets.size(), datasets[d].Name(), IsSignal, 1);
	 }
       }
       
       //Candidate pair of lepton
       string CandType; // ee - emu - mumum or false
       vector<NTElectron> candElec;
       vector<NTMuon> candMuon;
       sel.GetLeptonPair(candMuon, candElec, CandType ); // fill the variables
       
       //if (verbosity > 3) if( CandType!=string("")) cout << "Lepton pair candidate is "<< CandType <<endl;
       
       //integer which define the last step of the selection that the event fullfill
       
       
       
       
       
       
       
       
       //**************************************************
      //check corrleations of leptons and Btag triggers 
       //**************************************************
      double passTriggerBTAGMET = 0;
      double passTriggerMET     = 0;
      
      
      std::pair<bool, bool> trigBTagMET_MET = sel.passMETTriggerSelection (&datasets[d]);
      
      if(trigBTagMET_MET.first  == true) passTriggerBTAGMET = 1;
      if(trigBTagMET_MET.second == true) passTriggerMET     = 1;
      
           
      if (ChannelName=="mumu" && CandType=="mumu"){ //mumu
        MyhistoManager.FillHisto2D (correlationBTAGMET_mumu, "correlationBTAGMET_mumu", passTriggerBTAGMET,passTrigger , datasets[d].Name (), IsSignal,  weightITypeMC);
        MyhistoManager.FillHisto2D (correlationMET_mumu,     "correlationMET_mumu",     passTriggerMET,    passTrigger , datasets[d].Name (), IsSignal,  weightITypeMC);
      }
      if (ChannelName=="ee" && CandType=="ee"){ //ee
        MyhistoManager.FillHisto2D (correlationBTAGMET_ee, "correlationBTAGMET_ee", passTriggerBTAGMET,passTrigger , datasets[d].Name (), IsSignal,  weightITypeMC);
        MyhistoManager.FillHisto2D (correlationMET_ee,     "correlationMET_ee",     passTriggerMET,    passTrigger , datasets[d].Name (), IsSignal,  weightITypeMC);
      }
      if (ChannelName=="emu" && CandType=="emu"){ //emu
        MyhistoManager.FillHisto2D (correlationBTAGMET_emu, "correlationBTAGMET_emu", passTriggerBTAGMET,passTrigger , datasets[d].Name (), IsSignal,  weightITypeMC);
        MyhistoManager.FillHisto2D (correlationMET_emu,     "correlationMET_emu",     passTriggerMET,    passTrigger , datasets[d].Name (), IsSignal,  weightITypeMC);
      }
       
       
        
       int selLastStep = 0;
       /*
	 Steps of the selection: (integer returned by doFullSelection() or FillTable(...))
	 - Step 1  	Dilepton pair choice
	 - Step 2 	Z mass veto 
	 - Step 3 	Minimal jet multiplicity 
	 - Step 4 	MET cuts
	 - Step 5 	btagging cuts
       */  
       bool isData = true;
       if ( datasets[d].Name()!="DataEG" && datasets[d].Name()!="DataMu" && 
          datasets[d].Name()!="DataMuEG" && datasets[d].Name()!="DataEGMu" && 
          datasets[d].Name()!="MET1" && datasets[d].Name()!="MET2") isData = false;
	  
       //boolean which is true if the event pass the selection
       bool isSelEvent = false;	
       
       //if(CandType != "") cout << "found a " << CandType << " candidate passing trigger " <<  passTrigger << endl;
       
       if( (ChannelName=="ee" && CandType=="ee" && isData) ||
       	   (ChannelName=="ee" && CandType=="ee" && !isData && passTriggerBTAGMET==1 && passTriggerMET==1  )   ){
         //cout << "    in ee  " << CandType << " candidate passing trigger " <<  passTrigger << endl;
	 MyhistoManager.FillHisto(HTriggerEff_ElecSel_pT,  "HTriggerEff_ElecSel_pT",  candElec[0].p4.Pt(), datasets[d].Name(), IsSignal, weightITypeMC);
	 MyhistoManager.FillHisto(HTriggerEff_ElecSel_eta, "HTriggerEff_ElecSel_eta", candElec[0].p4.Eta(),datasets[d].Name(), IsSignal, weightITypeMC);
	 MyhistoManager.FillHisto(HTriggerEff_ElecSel_phi, "HTriggerEff_ElecSel_phi", candElec[0].p4.Phi(),datasets[d].Name(), IsSignal, weightITypeMC);
	 
	 if( passTrigger ){
	   MyhistoManager.FillHisto(HTriggerEff_ElecSel_selTrig_pT,  "HTriggerEff_ElecSel_selTrig_pT",  candElec[0].p4.Pt(), datasets[d].Name(),				    IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(HTriggerEff_ElecSel_selTrig_eta, "HTriggerEff_ElecSel_selTrig_eta", candElec[0].p4.Eta(),datasets[d].Name(),				    IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(HTriggerEff_ElecSel_selTrig_phi, "HTriggerEff_ElecSel_selTrig_phi", candElec[0].p4.Phi(),datasets[d].Name(),				    IsSignal, weightITypeMC);
	 }
       }
       
       
       
       if( (ChannelName=="mumu" && CandType=="mumu" && isData) ||
       	   (ChannelName=="mumu" && CandType=="mumu" && !isData && passTriggerBTAGMET==1 && passTriggerMET==1) ){
        // cout << "    in mumu  " << CandType << " candidate passing trigger " <<  passTrigger << endl;
	 MyhistoManager.FillHisto(HTriggerEff_MuonSel_pT,  "HTriggerEff_MuonSel_pT",  candMuon[0].p4.Pt(), datasets[d].Name(), IsSignal, weightITypeMC);
	 MyhistoManager.FillHisto(HTriggerEff_MuonSel_eta, "HTriggerEff_MuonSel_eta", candMuon[0].p4.Eta(),datasets[d].Name(), IsSignal, weightITypeMC);
	 MyhistoManager.FillHisto(HTriggerEff_MuonSel_phi, "HTriggerEff_MuonSel_phi", candMuon[0].p4.Phi(),datasets[d].Name(), IsSignal, weightITypeMC);
	 
	 if(   passTrigger   ){
	   MyhistoManager.FillHisto(HTriggerEff_MuonSel_selTrig_pT,  "HTriggerEff_MuonSel_selTrig_pT",  candMuon[0].p4.Pt(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(HTriggerEff_MuonSel_selTrig_eta, "HTriggerEff_MuonSel_selTrig_eta", candMuon[0].p4.Eta(),datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(HTriggerEff_MuonSel_selTrig_phi, "HTriggerEff_MuonSel_selTrig_phi", candMuon[0].p4.Phi(),datasets[d].Name(), IsSignal, weightITypeMC);
	 }
       }
       
       
       
       if( (ChannelName=="emu" && CandType=="emu" && isData) ||
       	   (ChannelName=="emu" && CandType=="emu" && !isData && passTriggerBTAGMET==1 && passTriggerMET==1) ){
	 
          // cout << "    in emu  " << CandType << " candidate passing trigger " <<  passTrigger << endl;
	   //MyhistoManager.FillHisto(HInvM_ll_pair, "InvDilMassPair" ,InvDilMass,datasets[d].Name(), IsSignal, 1); 
	   MyhistoManager.FillHisto(HTriggerEff_afterDileptSel_pT, "HTriggerEff_afterDileptSel_pT" , candMuon[0].p4.Pt() , datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(HTriggerEff_afterDileptSel_eta,"HTriggerEff_afterDileptSel_eta" , candMuon[0].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC);
	   MyhistoManager.FillHisto(HTriggerEff_afterDileptSel_phi,"HTriggerEff_afterDileptSel_phi" , candMuon[0].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   
	   if ( passTrigger && CandType=="emu"){
	     MyhistoManager.FillHisto(HTriggerEff_afterDileptSel_selTrig_pT,  "HTriggerEff_afterDileptSel_selTrig_pT" ,  candMuon[0].p4.Pt(), datasets[d].Name(), IsSignal, weightITypeMC);
	     MyhistoManager.FillHisto(HTriggerEff_afterDileptSel_selTrig_eta, "HTriggerEff_afterDileptSel_selTrig_eta" , candMuon[0].p4.Eta(), datasets[d].Name(), IsSignal, weightITypeMC);
	     MyhistoManager.FillHisto(HTriggerEff_afterDileptSel_selTrig_phi, "HTriggerEff_afterDileptSel_selTrig_phi" , candMuon[0].p4.Phi(), datasets[d].Name(), IsSignal, weightITypeMC);
	   }
       }
     }
     
     // cout some values based on objects
     //if(ievt == 10000) break ;
    }  // end of loop over evts
    TString outputFileName = "tagAndProbeHisto_"+datasets[d].Name()+".root";
    theEffPlots.Savehistos(outputFileName);
    
    
    
  }		
		// end of loop over the datasets 
    cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  TFile* fout  = new TFile("TriggerPlots.root","RECREATE");


  MyhistoManager.WriteMyHisto(HTriggerEff_afterDileptSel_pT ,"all");
  MyhistoManager.WriteMyHisto(HTriggerEff_afterDileptSel_eta,"all");
  MyhistoManager.WriteMyHisto(HTriggerEff_afterDileptSel_phi,"all");
  MyhistoManager.WriteMyHisto(HTriggerEff_afterDileptSel_selTrig_pT ,"all");
  MyhistoManager.WriteMyHisto(HTriggerEff_afterDileptSel_selTrig_eta,"all");
  MyhistoManager.WriteMyHisto(HTriggerEff_afterDileptSel_selTrig_phi,"all");
   
  MyhistoManager.WriteMyHisto(deltaR_Jet_Lepton_FullSel,"all"  );         
  MyhistoManager.WriteMyHisto(deltaR_Jet_MET_FullSel   ,"all"  );	    
  MyhistoManager.WriteMyHisto(deltaR_Muon_MET_FullSel  ,"all"  );	      
  MyhistoManager.WriteMyHisto(deltaR_Elec_MET_FullSel  ,"all"  );	  
  
  MyhistoManager.WriteMyHisto(deltaPhi_Jet_Lepton_FullSel,"all"  );         
  MyhistoManager.WriteMyHisto(deltaPhi_Jet_MET_FullSel   ,"all"  );	    
  MyhistoManager.WriteMyHisto(deltaPhi_Muon_MET_FullSel  ,"all"  );	      
  MyhistoManager.WriteMyHisto(deltaPhi_Elec_MET_FullSel  ,"all"  );	      
  MyhistoManager.WriteMyHisto(deltaPhi_DiLept_MET_FullSel  ,"all"  );	
  
  MyhistoManager.WriteMyHisto(HTriggerEff_ElecSel_pT,  "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_ElecSel_eta, "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_ElecSel_phi, "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_ElecSel_selTrig_pT,  "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_ElecSel_selTrig_eta, "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_ElecSel_selTrig_phi, "all");
  
  MyhistoManager.WriteMyHisto(HTriggerEff_MuonSel_pT,  "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_MuonSel_eta, "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_MuonSel_phi, "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_MuonSel_selTrig_pT,  "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_MuonSel_selTrig_eta, "all");
  MyhistoManager.WriteMyHisto(HTriggerEff_MuonSel_selTrig_phi, "all");
   
  MyhistoManager.WriteMyHisto(genEl_Tight_pt,  "all");
  MyhistoManager.WriteMyHisto(genEl_Tight_eta, "all");
  MyhistoManager.WriteMyHisto(genEl_Tight_phi, "all");
  MyhistoManager.WriteMyHisto(genEl_Tight_njet,"all");
  
  MyhistoManager.WriteMyHisto(genEl_LooseID_pt,  "all");
  MyhistoManager.WriteMyHisto(genEl_LooseID_eta, "all");
  MyhistoManager.WriteMyHisto(genEl_LooseID_phi, "all");
  MyhistoManager.WriteMyHisto(genEl_LooseID_njet,"all");
  
  MyhistoManager.WriteMyHisto(genEl_LooseIso_pt,  "all");
  MyhistoManager.WriteMyHisto(genEl_LooseIso_eta, "all");
  MyhistoManager.WriteMyHisto(genEl_LooseIso_phi, "all");
  MyhistoManager.WriteMyHisto(genEl_LooseIso_njet,"all");
  
  
  MyhistoManager.WriteMyHisto(genMu_Tight_pt,  "all");
  MyhistoManager.WriteMyHisto(genMu_Tight_eta, "all");
  MyhistoManager.WriteMyHisto(genMu_Tight_phi, "all");
  MyhistoManager.WriteMyHisto(genMu_Tight_njet,"all");
  
  MyhistoManager.WriteMyHisto(genMu_LooseID_pt,  "all");
  MyhistoManager.WriteMyHisto(genMu_LooseID_eta, "all");
  MyhistoManager.WriteMyHisto(genMu_LooseID_phi, "all");
  MyhistoManager.WriteMyHisto(genMu_LooseID_njet,"all");
  
  MyhistoManager.WriteMyHisto(genMu_LooseIso_pt,  "all");
  MyhistoManager.WriteMyHisto(genMu_LooseIso_eta, "all");
  MyhistoManager.WriteMyHisto(genMu_LooseIso_phi, "all");
  MyhistoManager.WriteMyHisto(genMu_LooseIso_njet,"all");
  MyhistoManager.WriteMyHisto2D(correlationBTAGMET_mumu,"all");
  MyhistoManager.WriteMyHisto2D(correlationMET_mumu,"all");
  MyhistoManager.WriteMyHisto2D(correlationBTAGMET_ee,"all");
  MyhistoManager.WriteMyHisto2D(correlationMET_ee,"all");
  MyhistoManager.WriteMyHisto2D(correlationBTAGMET_emu,"all");
  MyhistoManager.WriteMyHisto2D(correlationMET_emu,"all");
 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////





  ///////////////


  return (0);
}
