#include <iomanip>
#include <iostream>
#include <limits.h>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
// #include "../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
// #include "../Plots/interface/DiLepAnaHistoManager.h"
// #include "../BckgdEstimation/interface/DYEstimation.h"
// #include "../BckgdEstimation/interface/MMEstimation.h"

#include "../../MassAMWT/interface/AMWT.h"

using namespace TopTree;
using namespace std;


int main( int argc, const char* argv[] ){

  // List of arguments:
  // 0: The executable itself
  // 1: The number of times JetMET is smeared (not smeared if equal to 1)
  // 2: The scale and type of JES shift (if fabs(x) < 0.5, shift by eta, else absolute

// fixme: Depracated
   float JES_JetMET = 1.0;

  string xmlFileName = string ("./MyMassAnalysis.xml");

  for (int i=1;i<argc;++i) {
    if (strncmp(argv[i],"-c",2)==0) xmlFileName = string(argv[i+1]);
    if (strncmp(argv[i],"-h",2)==0) {
      cout << " -c xm_config_file\n";
      exit(1);
    }
  }

  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;

  //////////////////////
  //Global variables
  //////////////////////

  // GEN
  TH1F * histoGenTopRapidity1 = new TH1F("GenTopRapidity1", "GenTopRapidity1", 100, -5, 5);
  TH1F * histoGenTopRapidity2 = new TH1F("GenTopRapidity2", "GenTopRapidity2", 100, -5, 5);
  TH1F * histoGenTopMass1 = new TH1F("GenTopMass1", "GenTopMass1", 200, 0, 200);
  TH1F * histoGenTopMass2 = new TH1F("GenTopMass2", "GenTopMass2", 200, 0, 200);
  TH2F * histoGenTopMass1VsTopMass2 = new TH2F("GenTopMass1VsTopMass2", "GenTopMass1VsTopMass2", 200, 0, 200, 200, 0, 200);
  TH1F * histoGenDelta_y = new TH1F("GenDelta_y", "GenDelta_y", 400, -2, 2);
  TH1F * histoGenTTRapidity = new TH1F("GenTTRapidity", "GenTTRapidity", 500, -2.5, 2.5);
  TH1F * histoGenTTPt = new TH1F("GenTTPt", "GenTTPt", 250, 0, 250);
  TH1F * histoGenTTMass = new TH1F("GenTTMass", "GenTTMass", 100, 0, 1000);

  TH2F * histoGenDelta_yVsTTRapidity = new TH2F("GenDelta_yVsTTRapidity", "GenDelta_yVsTTRapidity", 10, -2, 2, 5, -2.5, 2.5);
  TH2F * histoGenDelta_yVsTTPt = new TH2F("GenDelta_yVsTTPt", "GenDelta_yVsTTPt", 10, -2, 2, 5, 0, 250);
  TH2F * histoGenDelta_yVsTTMass = new TH2F("GenDelta_yVsTTMass", "GenDelta_yVsTTMass", 10, -2, 2, 5, 0, 1000);
  TH1F * histoGenN_plusTTRapidity = new TH1F("GenN_plusTTRapidity", "GenN_plusTTRapidity", 20, -2.5, 2.5);
  histoGenN_plusTTRapidity->Sumw2();
  TH1F * histoGenN_plusTTPt = new TH1F("GenN_plusTTPt", "GenN_plusTTPt", 20, 0, 250);
  histoGenN_plusTTPt->Sumw2();
  TH1F * histoGenN_plusTTMass = new TH1F("GenN_plusTTMass", "GenN_plusTTMass", 20, 0, 1000);
  histoGenN_plusTTMass->Sumw2();
  TH1F * histoGenN_minusTTRapidity = new TH1F("GenN_minusTTRapidity", "GenN_minusTTRapidity", 20, -2.5, 2.5);
  histoGenN_minusTTRapidity->Sumw2();
  TH1F * histoGenN_minusTTPt = new TH1F("GenN_minusTTPt", "GenN_minusTTPt", 20, 0, 250);
  histoGenN_minusTTPt->Sumw2();
  TH1F * histoGenN_minusTTMass = new TH1F("GenN_minusTTMass", "GenN_minusTTMass", 20, 0, 1000);
  histoGenN_minusTTMass->Sumw2();
  TH1F * histoGenN_plusMinusN_minusTTRapidity = new TH1F("GenN_plusMinusN_minusTTRapidity", "GenN_plusMinusN_minusTTRapidity", 20, -2.5, 2.5);
  histoGenN_plusMinusN_minusTTRapidity->Sumw2();
  TH1F * histoGenN_plusPlusN_minusTTRapidity = new TH1F("GenN_plusPlusN_minusTTRapidity", "GenN_plusPlusN_minusTTRapidity", 20, -2.5, 2.5);
  histoGenN_plusPlusN_minusTTRapidity->Sumw2();
  TH1F * histoGenN_plusMinusN_minusTTPt = new TH1F("GenN_plusMinusN_minusTTPt", "GenN_plusMinusN_minusTTPt", 20, 0, 250);
  histoGenN_plusMinusN_minusTTPt->Sumw2();
  TH1F * histoGenN_plusPlusN_minusTTPt = new TH1F("GenN_plusPlusN_minusTTPt", "GenN_plusPlusN_minusTTPt", 20, 0, 250);
  histoGenN_plusPlusN_minusTTPt->Sumw2();
  TH1F * histoGenN_plusMinusN_minusTTMass = new TH1F("GenN_plusMinusN_minusTTMass", "GenN_plusMinusN_minusTTMass", 20, 0, 1000);
  histoGenN_plusMinusN_minusTTMass->Sumw2();
  TH1F * histoGenN_plusPlusN_minusTTMass = new TH1F("GenN_plusPlusN_minusTTMass", "GenN_plusPlusN_minusTTMass", 20, 0, 1000);
  histoGenN_plusPlusN_minusTTMass->Sumw2();
  TH1F * histoGenA_cTTRapidity = new TH1F("GenA_cTTRapidity", "GenA_cTTRapidity", 20, -2.5, 2.5);
  histoGenA_cTTRapidity->Sumw2();
  TH1F * histoGenA_cTTPt = new TH1F("GenA_cTTPt", "GenA_cTTPt", 20, 0, 250);
  histoGenA_cTTPt->Sumw2();
  TH1F * histoGenA_cTTMass = new TH1F("GenA_cTTMass", "GenA_cTTMass", 20, 0, 1000);
  histoGenA_cTTMass->Sumw2();
  // Fine binning
  TH2F * histoFineBinning_GenDelta_yVsTTRapidity = new TH2F("FineBinning_GenDelta_yVsTTRapidity", "FineBinning_GenDelta_yVsTTRapidity", 200, -2, 2, 500, -2.5, 2.5);
  TH2F * histoFineBinning_GenDelta_yVsTTPt = new TH2F("FineBinning_GenDelta_yVsTTPt", "FineBinning_GenDelta_yVsTTPt", 200, -2, 2, 250, 0, 250);
  TH2F * histoFineBinning_GenDelta_yVsTTMass = new TH2F("FineBinning_GenDelta_yVsTTMass", "FineBinning_GenDelta_yVsTTMass", 200, -2, 2, 100, 0, 1000);
  TH1F * histoFineBinning_GenN_plusTTRapidity = new TH1F("FineBinning_GenN_plusTTRapidity", "FineBinning_GenN_plusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_GenN_plusTTRapidity->Sumw2();
  TH1F * histoFineBinning_GenN_plusTTPt = new TH1F("FineBinning_GenN_plusTTPt", "FineBinning_GenN_plusTTPt", 250, 0, 250);
  histoFineBinning_GenN_plusTTPt->Sumw2();
  TH1F * histoFineBinning_GenN_plusTTMass = new TH1F("FineBinning_GenN_plusTTMass", "FineBinning_GenN_plusTTMass", 100, 0, 1000);
  histoFineBinning_GenN_plusTTMass->Sumw2();
  TH1F * histoFineBinning_GenN_minusTTRapidity = new TH1F("FineBinning_GenN_minusTTRapidity", "FineBinning_GenN_minusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_GenN_minusTTRapidity->Sumw2();
  TH1F * histoFineBinning_GenN_minusTTPt = new TH1F("FineBinning_GenN_minusTTPt", "FineBinning_GenN_minusTTPt", 250, 0, 250);
  histoFineBinning_GenN_minusTTPt->Sumw2();
  TH1F * histoFineBinning_GenN_minusTTMass = new TH1F("FineBinning_GenN_minusTTMass", "FineBinning_GenN_minusTTMass", 100, 0, 1000);
  histoFineBinning_GenN_minusTTMass->Sumw2();
  TH1F * histoFineBinning_GenN_plusMinusN_minusTTRapidity = new TH1F("FineBinning_GenN_plusMinusN_minusTTRapidity", "FineBinning_GenN_plusMinusN_minusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_GenN_plusMinusN_minusTTRapidity->Sumw2();
  TH1F * histoFineBinning_GenN_plusPlusN_minusTTRapidity = new TH1F("FineBinning_GenN_plusPlusN_minusTTRapidity", "FineBinning_GenN_plusPlusN_minusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_GenN_plusPlusN_minusTTRapidity->Sumw2();
  TH1F * histoFineBinning_GenN_plusMinusN_minusTTPt = new TH1F("FineBinning_GenN_plusMinusN_minusTTPt", "FineBinning_GenN_plusMinusN_minusTTPt", 250, 0, 250);
  histoFineBinning_GenN_plusMinusN_minusTTPt->Sumw2();
  TH1F * histoFineBinning_GenN_plusPlusN_minusTTPt = new TH1F("FineBinning_GenN_plusPlusN_minusTTPt", "FineBinning_GenN_plusPlusN_minusTTPt", 250, 0, 250);
  histoFineBinning_GenN_plusPlusN_minusTTPt->Sumw2();
  TH1F * histoFineBinning_GenN_plusMinusN_minusTTMass = new TH1F("FineBinning_GenN_plusMinusN_minusTTMass", "FineBinning_GenN_plusMinusN_minusTTMass", 100, 0, 1000);
  histoFineBinning_GenN_plusMinusN_minusTTMass->Sumw2();
  TH1F * histoFineBinning_GenN_plusPlusN_minusTTMass = new TH1F("FineBinning_GenN_plusPlusN_minusTTMass", "FineBinning_GenN_plusPlusN_minusTTMass", 100, 0, 1000);
  histoFineBinning_GenN_plusPlusN_minusTTMass->Sumw2();
  TH1F * histoFineBinning_GenA_cTTRapidity = new TH1F("FineBinning_GenA_cTTRapidity", "FineBinning_GenA_cTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_GenA_cTTRapidity->Sumw2();
  TH1F * histoFineBinning_GenA_cTTPt = new TH1F("FineBinning_GenA_cTTPt", "FineBinning_GenA_cTTPt", 250, 0, 250);
  histoFineBinning_GenA_cTTPt->Sumw2();
  TH1F * histoFineBinning_GenA_cTTMass = new TH1F("FineBinning_GenA_cTTMass", "FineBinning_GenA_cTTMass", 100, 0, 1000);
  histoFineBinning_GenA_cTTMass->Sumw2();


  // RECO
  TH1F * histoRecoTopRapidity1 = new TH1F("RecoTopRapidity1", "RecoTopRapidity1", 100, -5, 5);
  TH1F * histoRecoTopRapidity2 = new TH1F("RecoTopRapidity2", "RecoTopRapidity2", 100, -5, 5);
  TH1F * histoRecoLeptonRapidity1 = new TH1F("RecoLeptonRapidity1", "RecoLeptonRapidity1", 100, -5, 5);
  TH1F * histoRecoLeptonRapidity2 = new TH1F("RecoLeptonRapidity2", "RecoLeptonRapidity2", 100, -5, 5);
  TH1F * histoRecoTopMass1 = new TH1F("RecoTopMass1", "RecoTopMass1", 300, 0, 300);
  TH1F * histoRecoTopMass2 = new TH1F("RecoTopMass2", "RecoTopMass2", 300, 0, 300);
  TH2F * histoRecoTopMass1VsTopMass2 = new TH2F("RecoTopMass1VsTopMass2", "RecoTopMass1VsTopMass2", 300, 0, 300, 300, 0, 300);
  TH1F * histoRecoDelta_y = new TH1F("RecoDelta_y", "RecoDelta_y", 400, -2, 2);
  TH1F * histoRecoTTRapidity = new TH1F("RecoTTRapidity", "RecoTTRapidity", 500, -2.5, 2.5);
  TH1F * histoRecoTTPt = new TH1F("RecoTTPt", "RecoTTPt", 250, 0, 250);
  TH1F * histoRecoTTMass = new TH1F("RecoTTMass", "RecoTTMass", 100, 0, 1000);

  TH2F * histoRecoDelta_yVsTTRapidity = new TH2F("RecoDelta_yVsTTRapidity", "RecoDelta_yVsTTRapidity", 10, -2, 2, 5, -2.5, 2.5);
  TH2F * histoRecoDelta_yVsTTPt = new TH2F("RecoDelta_yVsTTPt", "RecoDelta_yVsTTPt", 10, -2, 2, 5, 0, 250);
  TH2F * histoRecoDelta_yVsTTMass = new TH2F("RecoDelta_yVsTTMass", "RecoDelta_yVsTTMass", 10, -2, 2, 5, 0, 1000);
  TH1F * histoRecoN_plusTTRapidity = new TH1F("RecoN_plusTTRapidity", "RecoN_plusTTRapidity", 20, -2.5, 2.5);
  histoRecoN_plusTTRapidity->Sumw2();
  TH1F * histoRecoN_plusTTPt = new TH1F("RecoN_plusTTPt", "RecoN_plusTTPt", 20, 0, 250);
  histoRecoN_plusTTPt->Sumw2();
  TH1F * histoRecoN_plusTTMass = new TH1F("RecoN_plusTTMass", "RecoN_plusTTMass", 20, 0, 1000);
  histoRecoN_plusTTMass->Sumw2();
  TH1F * histoRecoN_minusTTRapidity = new TH1F("RecoN_minusTTRapidity", "RecoN_minusTTRapidity", 20, -2.5, 2.5);
  histoRecoN_minusTTRapidity->Sumw2();
  TH1F * histoRecoN_minusTTPt = new TH1F("RecoN_minusTTPt", "RecoN_minusTTPt", 20, 0, 250);
  histoRecoN_minusTTPt->Sumw2();
  TH1F * histoRecoN_minusTTMass = new TH1F("RecoN_minusTTMass", "RecoN_minusTTMass", 20, 0, 1000);
  histoRecoN_minusTTMass->Sumw2();
  TH1F * histoRecoN_plusMinusN_minusTTRapidity = new TH1F("RecoN_plusMinusN_minusTTRapidity", "RecoN_plusMinusN_minusTTRapidity", 20, -2.5, 2.5);
  histoRecoN_plusMinusN_minusTTRapidity->Sumw2();
  TH1F * histoRecoN_plusPlusN_minusTTRapidity = new TH1F("RecoN_plusPlusN_minusTTRapidity", "RecoN_plusPlusN_minusTTRapidity", 20, -2.5, 2.5);
  histoRecoN_plusPlusN_minusTTRapidity->Sumw2();
  TH1F * histoRecoN_plusMinusN_minusTTPt = new TH1F("RecoN_plusMinusN_minusTTPt", "RecoN_plusMinusN_minusTTPt", 20, 0, 250);
  histoRecoN_plusMinusN_minusTTPt->Sumw2();
  TH1F * histoRecoN_plusPlusN_minusTTPt = new TH1F("RecoN_plusPlusN_minusTTPt", "RecoN_plusPlusN_minusTTPt", 20, 0, 250);
  histoRecoN_plusPlusN_minusTTPt->Sumw2();
  TH1F * histoRecoN_plusMinusN_minusTTMass = new TH1F("RecoN_plusMinusN_minusTTMass", "RecoN_plusMinusN_minusTTMass", 20, 0, 1000);
  histoRecoN_plusMinusN_minusTTMass->Sumw2();
  TH1F * histoRecoN_plusPlusN_minusTTMass = new TH1F("RecoN_plusPlusN_minusTTMass", "RecoN_plusPlusN_minusTTMass", 20, 0, 1000);
  histoRecoN_plusPlusN_minusTTMass->Sumw2();
  TH1F * histoRecoA_cTTRapidity = new TH1F("RecoA_cTTRapidity", "RecoA_cTTRapidity", 20, -2.5, 2.5);
  histoRecoA_cTTRapidity->Sumw2();
  TH1F * histoRecoA_cTTPt = new TH1F("RecoA_cTTPt", "RecoA_cTTPt", 20, 0, 250);
  histoRecoA_cTTPt->Sumw2();
  TH1F * histoRecoA_cTTMass = new TH1F("RecoA_cTTMass", "RecoA_cTTMass", 20, 0, 1000);
  histoRecoA_cTTMass->Sumw2();
  // Fine binning
  TH2F * histoFineBinning_RecoDelta_yVsTTRapidity = new TH2F("FineBinning_RecoDelta_yVsTTRapidity", "FineBinning_RecoDelta_yVsTTRapidity", 200, -2, 2, 500, -2.5, 2.5);
  TH2F * histoFineBinning_RecoDelta_yVsTTPt = new TH2F("FineBinning_RecoDelta_yVsTTPt", "FineBinning_RecoDelta_yVsTTPt", 200, -2, 2, 250, 0, 250);
  TH2F * histoFineBinning_RecoDelta_yVsTTMass = new TH2F("FineBinning_RecoDelta_yVsTTMass", "FineBinning_RecoDelta_yVsTTMass", 200, -2, 2, 100, 0, 1000);
  TH1F * histoFineBinning_RecoN_plusTTRapidity = new TH1F("FineBinning_RecoN_plusTTRapidity", "FineBinning_RecoN_plusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_RecoN_plusTTRapidity->Sumw2();
  TH1F * histoFineBinning_RecoN_plusTTPt = new TH1F("FineBinning_RecoN_plusTTPt", "FineBinning_RecoN_plusTTPt", 250, 0, 250);
  histoFineBinning_RecoN_plusTTPt->Sumw2();
  TH1F * histoFineBinning_RecoN_plusTTMass = new TH1F("FineBinning_RecoN_plusTTMass", "FineBinning_RecoN_plusTTMass", 100, 0, 1000);
  histoFineBinning_RecoN_plusTTMass->Sumw2();
  TH1F * histoFineBinning_RecoN_minusTTRapidity = new TH1F("FineBinning_RecoN_minusTTRapidity", "FineBinning_RecoN_minusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_RecoN_minusTTRapidity->Sumw2();
  TH1F * histoFineBinning_RecoN_minusTTPt = new TH1F("FineBinning_RecoN_minusTTPt", "FineBinning_RecoN_minusTTPt", 250, 0, 250);
  histoFineBinning_RecoN_minusTTPt->Sumw2();
  TH1F * histoFineBinning_RecoN_minusTTMass = new TH1F("FineBinning_RecoN_minusTTMass", "FineBinning_RecoN_minusTTMass", 100, 0, 1000);
  histoFineBinning_RecoN_minusTTMass->Sumw2();
  TH1F * histoFineBinning_RecoN_plusMinusN_minusTTRapidity = new TH1F("FineBinning_RecoN_plusMinusN_minusTTRapidity", "FineBinning_RecoN_plusMinusN_minusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_RecoN_plusMinusN_minusTTRapidity->Sumw2();  
  TH1F * histoFineBinning_RecoN_plusPlusN_minusTTRapidity = new TH1F("FineBinning_RecoN_plusPlusN_minusTTRapidity", "FineBinning_RecoN_plusPlusN_minusTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_RecoN_plusPlusN_minusTTRapidity->Sumw2();
  TH1F * histoFineBinning_RecoN_plusMinusN_minusTTPt = new TH1F("FineBinning_RecoN_plusMinusN_minusTTPt", "FineBinning_RecoN_plusMinusN_minusTTPt", 250, 0, 250);
  histoFineBinning_RecoN_plusMinusN_minusTTPt->Sumw2();
  TH1F * histoFineBinning_RecoN_plusPlusN_minusTTPt = new TH1F("FineBinning_RecoN_plusPlusN_minusTTPt", "FineBinning_RecoN_plusPlusN_minusTTPt", 250, 0, 250);
  histoFineBinning_RecoN_plusPlusN_minusTTPt->Sumw2();
  TH1F * histoFineBinning_RecoN_plusMinusN_minusTTMass = new TH1F("FineBinning_RecoN_plusMinusN_minusTTMass", "FineBinning_RecoN_plusMinusN_minusTTMass", 100, 0, 1000);
  histoFineBinning_RecoN_plusMinusN_minusTTMass->Sumw2();
  TH1F * histoFineBinning_RecoN_plusPlusN_minusTTMass = new TH1F("FineBinning_RecoN_plusPlusN_minusTTMass", "FineBinning_RecoN_plusPlusN_minusTTMass", 100, 0, 1000);
  histoFineBinning_RecoN_plusPlusN_minusTTMass->Sumw2();
  TH1F * histoFineBinning_RecoA_cTTRapidity = new TH1F("FineBinning_RecoA_cTTRapidity", "FineBinning_RecoA_cTTRapidity", 500, -2.5, 2.5);
  histoFineBinning_RecoA_cTTRapidity->Sumw2();
  TH1F * histoFineBinning_RecoA_cTTPt = new TH1F("FineBinning_RecoA_cTTPt", "FineBinning_RecoA_cTTPt", 250, 0, 250);
  histoFineBinning_RecoA_cTTPt->Sumw2();
  TH1F * histoFineBinning_RecoA_cTTMass = new TH1F("FineBinning_RecoA_cTTMass", "FineBinning_RecoA_cTTMass", 100, 0, 1000);
  histoFineBinning_RecoA_cTTMass->Sumw2();




  // RECO vs GEN
  TH2F * histoRecoVsGenTopRapidity1 = new TH2F("RecoVsGenTopRapidity1", "RecoVsGenTopRapidity1", 100, -5, 5, 100, -5, 5);
  TH2F * histoRecoVsGenTopRapidity2 = new TH2F("RecoVsGenTopRapidity2", "RecoVsGenTopRapidity2", 100, -5, 5, 100, -5, 5);
  TH2F * histoRecoVsGenTopMass1 = new TH2F("RecoVsGenTopMass1", "RecoVsGenTopMass1", 300, 0, 300, 200, 0, 200);
  TH2F * histoRecoVsGenTopMass2 = new TH2F("RecoVsGenTopMass2", "RecoVsGenTopMass2", 300, 0, 300, 200, 0, 200);
  TH1F * histoRecoMinusGenDivGenTopRapidity1 = new TH1F("RecoMinusGenDivGenTopRapidity1", "RecoMinusGenDivGenTopRapidity1", 100, -3, 3);
  TH1F * histoRecoMinusGenDivGenTopRapidity2 = new TH1F("RecoMinusGenDivGenTopRapidity2", "RecoMinusGenDivGenTopRapidity2", 100, -3, 3);
  TH1F * histoRecoMinusGenDivGenLeptonRapidity1 = new TH1F("RecoMinusGenDivGenLeptonRapidity1", "RecoMinusGenDivGenLeptonRapidity1", 100, -3, 3);
  TH1F * histoRecoMinusGenDivGenLeptonRapidity2 = new TH1F("RecoMinusGenDivGenLeptonRapidity2", "RecoMinusGenDivGenLeptonRapidity2", 100, -3, 3);
  TH1F * histoRecoMinusGenDivGenTopMass1 = new TH1F("RecoMinusGenDivGenTopMass1", "RecoMinusGenDivGenTopMass1", 100, -3, 3);
  TH1F * histoRecoMinusGenDivGenTopMass2 = new TH1F("RecoMinusGenDivGenTopMass2", "RecoMinusGenDivGenTopMass2", 100, -3, 3);

  TH2F * histoFineBinning_RecoVsGenDelta_y = new TH2F("FineBinning_RecoVsGenDelta_y", "FineBinning_RecoVsGenDelta_y", 400, -2, 2, 400, -2, 2); // Migration Matrix
  TH2F * histoRecoVsGenDelta_y = new TH2F("RecoVsGenDelta_y", "RecoVsGenDelta_y", 8, -2, 2, 16, -2, 2); // Migration Matrix
  TH1F * histoRecoMinusGenDivGenDelta_y = new TH1F("RecoMinusGenDivGenDelta_y", "RecoMinusGenDivGenDelta_y", 100, -3, 3);

  TH2F * histoRecoVsGenTTRapidity = new TH2F("RecoVsGenTTRapidity", "RecoVsGenTTRapidity", 500, -2.5, 2.5, 500, -2.5, 2.5);
  TH1F * histoRecoMinusGenDivGenTTRapidity = new TH1F("RecoMinusGenDivGenTTRapidity", "RecoMinusGenDivGenTTRapidity", 100, -3, 3);
  TH2F * histoRecoVsGenTTPt = new TH2F("RecoVsGenTTPt", "RecoVsGenTTPt", 250, 0, 250, 250, 0, 250);
  TH1F * histoRecoMinusGenDivGenTTPt = new TH1F("RecoMinusGenDivGenTTPt", "RecoMinusGenDivGenTTPt", 100, -3, 3);
  TH2F * histoRecoVsGenTTMass = new TH2F("RecoVsGenTTMass", "RecoVsGenTTMass", 100, 0, 1000, 100, 0, 1000);
  TH1F * histoRecoMinusGenDivGenTTMass = new TH1F("RecoMinusGenDivGenTTMass", "RecoMinusGenDivGenTTMass", 100, -3, 3);

  TH1F * histoDelta_yEfficiencyN = new TH1F("Delta_yEfficiencyN", "Delta_yEfficiencyN", 10, -2, 2);
  TH1F * histoDelta_yEfficiencyD = new TH1F("Delta_yEfficiencyD", "Delta_yEfficiencyD", 10, -2, 2);
  TH1F * histoDelta_yEfficiency = new TH1F("Delta_yEfficiency", "Delta_yEfficiency", 10, -2, 2);

  TH2F * histoDelta_yVsTTRapidityEfficiency = new TH2F("Delta_yVsTTRapidityEfficiency", "Delta_yVsTTRapidityEfficiency", 10, -2, 2, 5, -2.5, 2.5);
  TH2F * histoDelta_yVsTTPtEfficiency = new TH2F("Delta_yVsTTPtEfficiency", "Delta_yVsTTPtEfficiency", 10, -2, 2, 5, 0, 250);
  TH2F * histoDelta_yVsTTMassEfficiency = new TH2F("Delta_yVsTTMassEfficiency", "Delta_yVsTTMassEfficiency", 10, -2, 2, 5, 0, 1000);
  // Fine binning
  TH1F * histoFineBinning_Delta_yEfficiencyN = new TH1F("FineBinning_Delta_yEfficiencyN", "FineBinning_Delta_yEfficiencyN", 200, -2, 2);
  TH1F * histoFineBinning_Delta_yEfficiencyD = new TH1F("FineBinning_Delta_yEfficiencyD", "FineBinning_Delta_yEfficiencyD", 200, -2, 2);
  TH1F * histoFineBinning_Delta_yEfficiency = new TH1F("FineBinning_Delta_yEfficiency", "FineBinning_Delta_yEfficiency", 200, -2, 2);

  TH2F * histoFineBinning_Delta_yVsTTRapidityEfficiency = new TH2F("FineBinning_Delta_yVsTTRapidityEfficiency", "FineBinning_Delta_yVsTTRapidityEfficiency", 200, -2, 2, 500, -2.5, 2.5);
  TH2F * histoFineBinning_Delta_yVsTTPtEfficiency = new TH2F("FineBinning_Delta_yVsTTPtEfficiency", "FineBinning_Delta_yVsTTPtEfficiency", 200, -2, 2, 250, 0, 250);
  TH2F * histoFineBinning_Delta_yVsTTMassEfficiency = new TH2F("FineBinning_Delta_yVsTTMassEfficiency", "FineBinning_Delta_yVsTTMassEfficiency", 200, -2, 2, 100, 0, 1000);




  vector < Dataset > datasets;
  DiLeptonSelection sel;
  float Luminosity = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0;
  int verbosity = -1;
  
  // Analysis variables
  int step;
  //4-vectors
  vector < TLorentzVector > nu1;
  vector < TLorentzVector > nu2;
  //  TLorentzVector lvTop1_aux;
  //TLorentzVector lvTop2_aux;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );

  float weight_MC; 
  int nEvents;
  int firstEvent;
  int initialEvents;
  anaEL.LoadAnalysisInfo("Run", "firstEvent", firstEvent);
  anaEL.LoadAnalysisInfo("Run", "nEvents", nEvents);
  anaEL.LoadAnalysisInfo("Run", "initialEvents", initialEvents);
  string channelName;
  anaEL.LoadAnalysisInfo("MassAnalysis", "ChannelName", channelName);

  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)

  //******************************************
  //Load Scale Factors for lepton efficiencies
  //******************************************
  sel.LoadElScaleFactors();
  sel.LoadMuScaleFactors();
  sel.InitJESUnc();

  bool nonskimmed = false;

  TopTree::NTEvent * event = 0;

  TFile * TupleFile = new TFile("TupleFileSkimmed_TTbarSignalMadgraph_JER30.root","RECREATE");
  float mttbar_rec, mttbar_gen;
  float diffysquare_rec, diffysquare_gen;
  float diffabseta_rec, diffabseta_gen;
  float weight;
  float pt_ttbar, pt_ttbar_gen;
  float pz_ttbar, pz_ttbar_gen;
  float eta_ttbar_rec, eta_ttbar_gen;
  float y_ttbar_rec, y_ttbar_gen;
  float diffabsy_rec, diffabsy_gen;

  TTree *tree = new TTree("Tuple","Tuple");
  tree->Branch("mttbar_rec",&mttbar_rec,"mttbar_rec/F");
  tree->Branch("mttbar_gen",&mttbar_gen,"mttbar_gen/F");
  tree->Branch("diffysquare_rec",&diffysquare_rec,"diffysquare_rec/F");
  tree->Branch("diffysquare_gen",&diffysquare_gen,"diffysquare_gen/F");
  tree->Branch("diffabseta_rec",&diffabseta_rec,"diffabseta_rec/F");
  tree->Branch("diffabseta_gen",&diffabseta_gen,"diffabseta_gen/F");
  tree->Branch("weight",&weight,"weight/F");
  tree->Branch("pt_ttbar",&pt_ttbar,"pt_ttbar/F");
  tree->Branch("pt_ttbar_gen",&pt_ttbar_gen,"pt_ttbar_gen/F");
  tree->Branch("pz_ttbar",&pz_ttbar,"pz_ttbar/F");
  tree->Branch("pz_ttbar_gen",&pz_ttbar_gen,"pz_ttbar_gen/F");
  tree->Branch("eta_ttbar_rec",&eta_ttbar_rec,"eta_ttbar_rec/F");
  tree->Branch("eta_ttbar_gen",&eta_ttbar_gen,"eta_ttbar_gen/F");
  tree->Branch("y_ttbar_rec",&y_ttbar_rec,"y_ttbar_rec/F");
  tree->Branch("y_ttbar_gen",&y_ttbar_gen,"y_ttbar_gen/F");
  tree->Branch("diffabsy_rec",&diffabsy_rec,"diffabsy_rec/F");
  tree->Branch("diffabsy_gen",&diffabsy_gen,"diffabsy_gen/F");

  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Loop over the datasets  "<<endl;
    cout<<"#########################"<<endl;
  }

  float num_mttbar_eff = 0.;
  float den_mttbar_eff = 0.;

  for (unsigned int d = 0; d < datasets.size (); d++) {

    weight_MC = datasets[d].NormFactor()*Luminosity;

    if(datasets[d].isData() == true) weight_MC = 1.; cout << "isDATA" << endl;

    AMWT amwt(anaEL, datasets[d].isData());

    TString sample_name(datasets[d].Name());


    bool isData = datasets[d].isData ();
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    cout << "Sample : " << sample_name<< endl;
    cout << "Data   : " << isData<< endl;

    unsigned int nEventsSample = (int) (datasets[d].eventTree ()->GetEntries ());
    unsigned int endEventToRun;

    if (firstEvent>nEventsSample) firstEvent = nEventsSample;
    if ((nEvents==-1)||((nEvents+firstEvent)>nEventsSample)) endEventToRun = nEventsSample;
    else endEventToRun = nEvents+firstEvent;
    cout << "Events to run: number / first / last / all: " << endEventToRun-firstEvent 
         << " / " << firstEvent << " / " << endEventToRun
         << " / " << nEventsSample << endl;

// Standard parameters for doFullSelection:

    bool applyEEScale = false; float EEScaleParam = 1.; bool applyEEResol = false; float EEResolParam = 1.;
    bool applyMEScale = false; float MEScaleParam = 1.; bool applyMEResol = false; float MEResolParam = 1.;
    bool applyMETS = false; float METScale = 1.;
    bool applyJES = false; float JESParam = 1.;
    bool applyJER = true; float JERFactor = 0.30; 

    //////////////////////
    //LOOP OVER THE EVENTS
    //////////////////////

    float N_plus_reco = 0;
    float N_minus_reco = 0;

    float N_plus_gen = 0;
    float N_minus_gen = 0;

    mttbar_rec = 0;
    mttbar_gen = 0;
    diffysquare_rec = 0;
    diffysquare_gen = 0;
    diffabseta_rec = 0;
    diffabseta_gen = 0;
    weight = weight_MC;
    pt_ttbar = 0;
    pt_ttbar_gen = 0;
    pz_ttbar = 0;
    pz_ttbar_gen = 0;
    eta_ttbar_rec = 0;
    eta_ttbar_gen = 0;
    y_ttbar_rec = 0;
    y_ttbar_gen = 0;
    diffabsy_rec = 0;
    diffabsy_gen = 0;

    for (unsigned int ievt = firstEvent; ievt < endEventToRun; ievt++) {
//           for (unsigned int ievt = 0; ievt < 10; ievt++) {

      datasets[d].eventTree ()->GetEntry (ievt);

      if(ievt%1000 == 0) cout << "number of processed events " << ievt << endl;
      if (verbosity >= 1) cout << "Event : "<< event->runNb<<" , " << event->lumiblock <<" , " << event->eventNb<<endl;

      if(!(event->TMEME == 2  || event->TMEME == 10101 || event->TMEME == 20200 || event->TMEME == 20 || event->TMEME == 11010 || 
event->TMEME == 22000|| event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 )) continue;

      string candType;
      vector<NTElectron> candElec;
      vector<NTMuon> candMuon;

      //Load event for the selection
      sel.LoadEvent(event);

      int finalCut = sel.doFullSelection(&(datasets[d]), channelName, false, /*print*/ verbosity,
	false, false, -1., -1., applyJES,  JESParam,
	 applyEEScale,  EEScaleParam,  applyEEResol,  EEResolParam,
	 applyMEScale,  MEScaleParam,  
	 applyJER,  JERFactor,  applyMETS,  METScale);




        TLorentzVector lvTop1_gen;
        TLorentzVector lvTop2_gen;
        TLorentzVector lvTop1plusTop2_gen;
        float yt_gen = 0.;
        float ytbar_gen = 0.;
        float Delta_y_gen = 0.;

        if(datasets[d].Name() == "TTJets_Sig"){


        lvTop1_gen = (event->topAndDecays)[0].p4_t_gen;
        lvTop2_gen = (event->topAndDecays)[1].p4_t_gen;
        lvTop1plusTop2_gen = lvTop1_gen + lvTop2_gen;

        /////////////////////////
        //// GENERATOR LEVEL ////
        /////////////////////////

        histoGenTopRapidity1->Fill(lvTop1_gen.Rapidity());
        histoGenTopRapidity2->Fill(lvTop2_gen.Rapidity());
        histoGenTopMass1->Fill(lvTop1_gen.M());
        histoGenTopMass2->Fill(lvTop2_gen.M());
        histoGenTopMass1VsTopMass2->Fill(lvTop1_gen.M(),lvTop2_gen.M());

        yt_gen = lvTop1_gen.Rapidity();
        if(yt_gen<0.) yt_gen = (-1)*yt_gen;
        ytbar_gen = lvTop2_gen.Rapidity();
        if(ytbar_gen<0.) ytbar_gen = (-1)*ytbar_gen;
        Delta_y_gen = yt_gen - ytbar_gen;
	//        cout << "Delta_y_gen:" << Delta_y_gen << endl;
        histoGenDelta_y->Fill(Delta_y_gen);
        histoDelta_yEfficiencyD->Fill(Delta_y_gen);
        histoFineBinning_Delta_yEfficiencyD->Fill(Delta_y_gen);

        if(Delta_y_gen>0.){
          N_plus_gen++;
          histoGenN_plusTTRapidity->Fill(lvTop1plusTop2_gen.Rapidity());
          histoGenN_plusTTPt->Fill(lvTop1plusTop2_gen.Perp());
          histoGenN_plusTTMass->Fill(lvTop1plusTop2_gen.M());
          histoFineBinning_GenN_plusTTRapidity->Fill(lvTop1plusTop2_gen.Rapidity());
          histoFineBinning_GenN_plusTTPt->Fill(lvTop1plusTop2_gen.Perp());
          histoFineBinning_GenN_plusTTMass->Fill(lvTop1plusTop2_gen.M());
	}

        if(Delta_y_gen<0.){
          N_minus_gen++;
          histoGenN_minusTTRapidity->Fill(lvTop1plusTop2_gen.Rapidity());
          histoGenN_minusTTPt->Fill(lvTop1plusTop2_gen.Perp());
          histoGenN_minusTTMass->Fill(lvTop1plusTop2_gen.M());
          histoFineBinning_GenN_minusTTRapidity->Fill(lvTop1plusTop2_gen.Rapidity());
          histoFineBinning_GenN_minusTTPt->Fill(lvTop1plusTop2_gen.Perp());
          histoFineBinning_GenN_minusTTMass->Fill(lvTop1plusTop2_gen.M());

	}

        histoGenTTRapidity->Fill(lvTop1plusTop2_gen.Rapidity());
        histoGenTTPt->Fill(lvTop1plusTop2_gen.Perp());
        histoGenTTMass->Fill(lvTop1plusTop2_gen.M());
        histoGenDelta_yVsTTRapidity->Fill(Delta_y_gen, lvTop1plusTop2_gen.Rapidity());
        histoGenDelta_yVsTTPt->Fill(Delta_y_gen, lvTop1plusTop2_gen.Perp());
        histoGenDelta_yVsTTMass->Fill(Delta_y_gen, lvTop1plusTop2_gen.M());

        histoFineBinning_GenDelta_yVsTTRapidity->Fill(Delta_y_gen, lvTop1plusTop2_gen.Rapidity());
        histoFineBinning_GenDelta_yVsTTPt->Fill(Delta_y_gen, lvTop1plusTop2_gen.Perp());
        histoFineBinning_GenDelta_yVsTTMass->Fill(Delta_y_gen, lvTop1plusTop2_gen.M());


        if(nonskimmed == true){
         mttbar_gen = lvTop1plusTop2_gen.M();
         diffysquare_gen = yt_gen*yt_gen - ytbar_gen*ytbar_gen;
         diffabseta_gen = fabs(lvTop1_gen.Eta()) - fabs(lvTop2_gen.Eta());
         pt_ttbar_gen = lvTop1plusTop2_gen.Perp();
         pz_ttbar_gen = lvTop1plusTop2_gen.Pz();
         eta_ttbar_gen = lvTop1plusTop2_gen.Eta();
         y_ttbar_gen = lvTop1plusTop2_gen.Rapidity();
         diffabsy_gen = fabs(lvTop1_gen.Rapidity()) - fabs(lvTop2_gen.Rapidity());
	}

	}

      if (finalCut >6) {

        if(nonskimmed == false){
         if(datasets[d].Name() == "TTJets_Sig"){
         mttbar_gen = lvTop1plusTop2_gen.M();
         diffysquare_gen = yt_gen*yt_gen - ytbar_gen*ytbar_gen;
         diffabseta_gen = fabs(lvTop1_gen.Eta()) - fabs(lvTop2_gen.Eta());
         pt_ttbar_gen = lvTop1plusTop2_gen.Perp();
         pz_ttbar_gen = lvTop1plusTop2_gen.Pz();
         eta_ttbar_gen = lvTop1plusTop2_gen.Eta();
         y_ttbar_gen = lvTop1plusTop2_gen.Rapidity();
         diffabsy_gen = fabs(lvTop1_gen.Rapidity()) - fabs(lvTop2_gen.Rapidity());
	 }
	}

        sel.GetLeptonPair(candMuon, candElec, candType);

        for(unsigned int i=0; i<candMuon.size(); i++){
          if(candMuon[i].Charge == +1) histoRecoLeptonRapidity1->Fill(candMuon[i].p4.Rapidity());
          if(candMuon[i].Charge == -1) histoRecoLeptonRapidity2->Fill(candMuon[i].p4.Rapidity());
	}

        for(unsigned int j=0; j<candElec.size(); j++){
          if(candElec[j].Charge == +1) histoRecoLeptonRapidity1->Fill(candElec[j].p4.Rapidity());
          if(candElec[j].Charge == -1) histoRecoLeptonRapidity2->Fill(candElec[j].p4.Rapidity());
	}



        TLorentzVector lvTop1_reco;
        TLorentzVector lvTop2_reco;
        TLorentzVector lvTop1plusTop2_reco;


        TopMassVariables * tmv = amwt.findMass(sel, lvTop1_reco, lvTop2_reco);
        lvTop1plusTop2_reco = lvTop1_reco + lvTop2_reco;



        ////////////////////
        /// RECO LEVEL /////
        ////////////////////

        histoRecoTopRapidity1->Fill(lvTop1_reco.Rapidity());
        histoRecoTopRapidity2->Fill(lvTop2_reco.Rapidity());
        histoRecoTopMass1->Fill(lvTop1_reco.M());
        histoRecoTopMass2->Fill(lvTop2_reco.M());
        histoRecoTopMass1VsTopMass2->Fill(lvTop1_reco.M(),lvTop2_reco.M());

        float yt_reco = lvTop1_reco.Rapidity();
        if(yt_reco<0.) yt_reco = (-1)*yt_reco;
        float ytbar_reco = lvTop2_reco.Rapidity();
        if(ytbar_reco<0.) ytbar_reco = (-1)*ytbar_reco;
        float Delta_y_reco = yt_reco - ytbar_reco;
	//        cout << "Delta_y_reco:" << Delta_y_reco << endl;
        histoRecoDelta_y->Fill(Delta_y_reco);
        histoDelta_yEfficiencyN->Fill(Delta_y_reco);
        histoFineBinning_Delta_yEfficiencyN->Fill(Delta_y_reco);

        if(Delta_y_reco>0.){
          N_plus_reco++;
          histoRecoN_plusTTRapidity->Fill(lvTop1plusTop2_reco.Rapidity());
          histoRecoN_plusTTPt->Fill(lvTop1plusTop2_reco.Perp());
          histoRecoN_plusTTMass->Fill(lvTop1plusTop2_reco.M());
          histoFineBinning_RecoN_plusTTRapidity->Fill(lvTop1plusTop2_reco.Rapidity());
          histoFineBinning_RecoN_plusTTPt->Fill(lvTop1plusTop2_reco.Perp());
          histoFineBinning_RecoN_plusTTMass->Fill(lvTop1plusTop2_reco.M());
	}

        if(Delta_y_reco<0.){
          N_minus_reco++;
          histoRecoN_minusTTRapidity->Fill(lvTop1plusTop2_reco.Rapidity());
          histoRecoN_minusTTPt->Fill(lvTop1plusTop2_reco.Perp());
          histoRecoN_minusTTMass->Fill(lvTop1plusTop2_reco.M());
          histoFineBinning_RecoN_minusTTRapidity->Fill(lvTop1plusTop2_reco.Rapidity());
          histoFineBinning_RecoN_minusTTPt->Fill(lvTop1plusTop2_reco.Perp());
          histoFineBinning_RecoN_minusTTMass->Fill(lvTop1plusTop2_reco.M());
	}

        histoRecoTTRapidity->Fill(lvTop1plusTop2_reco.Rapidity());
        histoRecoTTPt->Fill(lvTop1plusTop2_reco.Perp());
        histoRecoTTMass->Fill(lvTop1plusTop2_reco.M());
        histoRecoDelta_yVsTTRapidity->Fill(Delta_y_reco, lvTop1plusTop2_reco.Rapidity());
        histoRecoDelta_yVsTTPt->Fill(Delta_y_reco, lvTop1plusTop2_reco.Perp());
        histoRecoDelta_yVsTTMass->Fill(Delta_y_reco, lvTop1plusTop2_reco.M());

        histoFineBinning_RecoDelta_yVsTTRapidity->Fill(Delta_y_reco, lvTop1plusTop2_reco.Rapidity());
        histoFineBinning_RecoDelta_yVsTTPt->Fill(Delta_y_reco, lvTop1plusTop2_reco.Perp());
        histoFineBinning_RecoDelta_yVsTTMass->Fill(Delta_y_reco, lvTop1plusTop2_reco.M());

        if(nonskimmed == false){

         mttbar_rec = lvTop1plusTop2_reco.M();
         diffysquare_rec = yt_reco*yt_reco - ytbar_reco*ytbar_reco;
         diffabseta_rec = fabs(lvTop1_reco.Eta()) - fabs(lvTop2_reco.Eta());
         pt_ttbar = lvTop1plusTop2_reco.Perp();
         pz_ttbar = lvTop1plusTop2_reco.Pz();
         eta_ttbar_rec = lvTop1plusTop2_reco.Eta();
         y_ttbar_rec = lvTop1plusTop2_reco.Rapidity();
         diffabsy_rec = fabs(lvTop1_reco.Rapidity()) - fabs(lvTop2_reco.Rapidity());

	}

        ////////////////////////
        // RECO VS GEN LEVEL ///
        ////////////////////////
        if(datasets[d].Name() == "TTJets_Sig"){
 
        histoRecoVsGenTopRapidity1->Fill(lvTop1_reco.Rapidity(), lvTop1_gen.Rapidity());
        histoRecoVsGenTopRapidity2->Fill(lvTop2_reco.Rapidity(), lvTop2_gen.Rapidity());
        histoRecoVsGenTopMass1->Fill(lvTop1_reco.M(), lvTop1_gen.M());
        histoRecoVsGenTopMass2->Fill(lvTop2_reco.M(), lvTop2_gen.M());
	histoRecoMinusGenDivGenTopRapidity1->Fill((lvTop1_reco.Rapidity() - lvTop1_gen.Rapidity())/lvTop1_gen.Rapidity());
        histoRecoMinusGenDivGenTopRapidity2->Fill((lvTop2_reco.Rapidity() - lvTop2_gen.Rapidity())/lvTop2_gen.Rapidity());

        for(unsigned int i=0; i<candMuon.size(); i++){
          if(candMuon[i].Charge == +1) histoRecoMinusGenDivGenLeptonRapidity1->Fill((candMuon[i].p4.Rapidity() - lvTop1_gen.Rapidity())/lvTop1_gen.Rapidity());
          if(candMuon[i].Charge == -1) histoRecoMinusGenDivGenLeptonRapidity2->Fill((candMuon[i].p4.Rapidity() - lvTop2_gen.Rapidity())/lvTop2_gen.Rapidity());
	}

        for(unsigned int j=0; j<candElec.size(); j++){
          if(candElec[j].Charge == +1) histoRecoMinusGenDivGenLeptonRapidity1->Fill((candElec[j].p4.Rapidity() - lvTop1_gen.Rapidity())/lvTop1_gen.Rapidity());
          if(candElec[j].Charge == -1) histoRecoMinusGenDivGenLeptonRapidity2->Fill((candElec[j].p4.Rapidity() - lvTop2_gen.Rapidity())/lvTop2_gen.Rapidity());
	}

	histoRecoMinusGenDivGenTopMass1->Fill((lvTop1_reco.M() - lvTop1_gen.M())/lvTop1_gen.M());
        histoRecoMinusGenDivGenTopMass2->Fill((lvTop2_reco.M() - lvTop2_gen.M())/lvTop2_gen.M());

        histoRecoVsGenDelta_y->Fill(Delta_y_reco, Delta_y_gen);
        histoFineBinning_RecoVsGenDelta_y->Fill(Delta_y_reco, Delta_y_gen);
        histoRecoMinusGenDivGenDelta_y->Fill((Delta_y_reco - Delta_y_gen)/Delta_y_gen);

        histoRecoVsGenTTRapidity->Fill(lvTop1plusTop2_reco.Rapidity(),lvTop1plusTop2_gen.Rapidity());
        histoRecoMinusGenDivGenTTRapidity->Fill((lvTop1plusTop2_reco.Rapidity() - lvTop1plusTop2_gen.Rapidity())/lvTop1plusTop2_gen.Rapidity());
        histoRecoVsGenTTPt->Fill(lvTop1plusTop2_reco.Perp(),lvTop1plusTop2_gen.Perp());
        histoRecoMinusGenDivGenTTPt->Fill((lvTop1plusTop2_reco.Perp() - lvTop1plusTop2_gen.Perp())/lvTop1plusTop2_gen.Perp());
        histoRecoVsGenTTMass->Fill(lvTop1plusTop2_reco.M(),lvTop1plusTop2_gen.M());
        histoRecoMinusGenDivGenTTMass->Fill((lvTop1plusTop2_reco.M() - lvTop1plusTop2_gen.M())/lvTop1plusTop2_gen.M());

	}

	//	tmv->printAll();
	delete tmv;
       if(nonskimmed == false && mttbar_rec > 60.){ tree->Fill(); num_mttbar_eff += weight_MC;}
       if(nonskimmed == false) den_mttbar_eff += weight_MC;
      }

      if(nonskimmed == true) tree->Fill();

    }  // end of loop over evts

    float A_C_reco = (N_plus_reco - N_minus_reco)/(N_plus_reco + N_minus_reco);
    float A_C_gen = (N_plus_gen - N_minus_gen)/(N_plus_gen + N_minus_gen);

    cout << "A_C_gen:" << A_C_gen << endl;
    cout << "A_C_reco:" << A_C_reco << endl;

    histoGenN_plusMinusN_minusTTRapidity->Add(histoGenN_plusTTRapidity);
    histoGenN_plusMinusN_minusTTRapidity->Add(histoGenN_minusTTRapidity,-1);
    histoGenN_plusPlusN_minusTTRapidity->Add(histoGenN_plusTTRapidity);
    histoGenN_plusPlusN_minusTTRapidity->Add(histoGenN_minusTTRapidity);

    histoGenA_cTTRapidity->Divide(histoGenN_plusMinusN_minusTTRapidity, histoGenN_plusPlusN_minusTTRapidity);

    histoGenN_plusMinusN_minusTTPt->Add(histoGenN_plusTTPt);
    histoGenN_plusMinusN_minusTTPt->Add(histoGenN_minusTTPt,-1);
    histoGenN_plusPlusN_minusTTPt->Add(histoGenN_plusTTPt);
    histoGenN_plusPlusN_minusTTPt->Add(histoGenN_minusTTPt);

    histoGenA_cTTPt->Divide(histoGenN_plusMinusN_minusTTPt, histoGenN_plusPlusN_minusTTPt);

    histoGenN_plusMinusN_minusTTMass->Add(histoGenN_plusTTMass);
    histoGenN_plusMinusN_minusTTMass->Add(histoGenN_minusTTMass,-1);
    histoGenN_plusPlusN_minusTTMass->Add(histoGenN_plusTTMass);
    histoGenN_plusPlusN_minusTTMass->Add(histoGenN_minusTTMass);

    histoGenA_cTTMass->Divide(histoGenN_plusMinusN_minusTTMass, histoGenN_plusPlusN_minusTTMass);

    histoRecoN_plusMinusN_minusTTRapidity->Add(histoRecoN_plusTTRapidity);
    histoRecoN_plusMinusN_minusTTRapidity->Add(histoRecoN_minusTTRapidity,-1);
    histoRecoN_plusPlusN_minusTTRapidity->Add(histoRecoN_plusTTRapidity);
    histoRecoN_plusPlusN_minusTTRapidity->Add(histoRecoN_minusTTRapidity);

    histoRecoA_cTTRapidity->Divide(histoRecoN_plusMinusN_minusTTRapidity, histoRecoN_plusPlusN_minusTTRapidity);

    histoRecoN_plusMinusN_minusTTPt->Add(histoRecoN_plusTTPt);
    histoRecoN_plusMinusN_minusTTPt->Add(histoRecoN_minusTTPt,-1);
    histoRecoN_plusPlusN_minusTTPt->Add(histoRecoN_plusTTPt);
    histoRecoN_plusPlusN_minusTTPt->Add(histoRecoN_minusTTPt);

    histoRecoA_cTTPt->Divide(histoRecoN_plusMinusN_minusTTPt, histoRecoN_plusPlusN_minusTTPt);

    histoRecoN_plusMinusN_minusTTMass->Add(histoRecoN_plusTTMass);
    histoRecoN_plusMinusN_minusTTMass->Add(histoRecoN_minusTTMass,-1);
    histoRecoN_plusPlusN_minusTTMass->Add(histoRecoN_plusTTMass);
    histoRecoN_plusPlusN_minusTTMass->Add(histoRecoN_minusTTMass);

    histoRecoA_cTTMass->Divide(histoRecoN_plusMinusN_minusTTMass, histoRecoN_plusPlusN_minusTTMass);





    histoFineBinning_GenN_plusMinusN_minusTTRapidity->Add(histoFineBinning_GenN_plusTTRapidity);
    histoFineBinning_GenN_plusMinusN_minusTTRapidity->Add(histoFineBinning_GenN_minusTTRapidity,-1);
    histoFineBinning_GenN_plusPlusN_minusTTRapidity->Add(histoFineBinning_GenN_plusTTRapidity);
    histoFineBinning_GenN_plusPlusN_minusTTRapidity->Add(histoFineBinning_GenN_minusTTRapidity);

    histoFineBinning_GenA_cTTRapidity->Divide(histoFineBinning_GenN_plusMinusN_minusTTRapidity, histoFineBinning_GenN_plusPlusN_minusTTRapidity);

    histoFineBinning_GenN_plusMinusN_minusTTPt->Add(histoFineBinning_GenN_plusTTPt);
    histoFineBinning_GenN_plusMinusN_minusTTPt->Add(histoFineBinning_GenN_minusTTPt,-1);
    histoFineBinning_GenN_plusPlusN_minusTTPt->Add(histoFineBinning_GenN_plusTTPt);
    histoFineBinning_GenN_plusPlusN_minusTTPt->Add(histoFineBinning_GenN_minusTTPt);

    histoFineBinning_GenA_cTTPt->Divide(histoFineBinning_GenN_plusMinusN_minusTTPt, histoFineBinning_GenN_plusPlusN_minusTTPt);

    histoFineBinning_GenN_plusMinusN_minusTTMass->Add(histoFineBinning_GenN_plusTTMass);
    histoFineBinning_GenN_plusMinusN_minusTTMass->Add(histoFineBinning_GenN_minusTTMass,-1);
    histoFineBinning_GenN_plusPlusN_minusTTMass->Add(histoFineBinning_GenN_plusTTMass);
    histoFineBinning_GenN_plusPlusN_minusTTMass->Add(histoFineBinning_GenN_minusTTMass);

    histoFineBinning_GenA_cTTMass->Divide(histoFineBinning_GenN_plusMinusN_minusTTMass, histoFineBinning_GenN_plusPlusN_minusTTMass);

    histoFineBinning_RecoN_plusMinusN_minusTTRapidity->Add(histoFineBinning_RecoN_plusTTRapidity);
    histoFineBinning_RecoN_plusMinusN_minusTTRapidity->Add(histoFineBinning_RecoN_minusTTRapidity,-1);
    histoFineBinning_RecoN_plusPlusN_minusTTRapidity->Add(histoFineBinning_RecoN_plusTTRapidity);
    histoFineBinning_RecoN_plusPlusN_minusTTRapidity->Add(histoFineBinning_RecoN_minusTTRapidity);

    histoFineBinning_RecoA_cTTRapidity->Divide(histoFineBinning_RecoN_plusMinusN_minusTTRapidity, histoFineBinning_RecoN_plusPlusN_minusTTRapidity);

    histoFineBinning_RecoN_plusMinusN_minusTTPt->Add(histoFineBinning_RecoN_plusTTPt);
    histoFineBinning_RecoN_plusMinusN_minusTTPt->Add(histoFineBinning_RecoN_minusTTPt,-1);
    histoFineBinning_RecoN_plusPlusN_minusTTPt->Add(histoFineBinning_RecoN_plusTTPt);
    histoFineBinning_RecoN_plusPlusN_minusTTPt->Add(histoFineBinning_RecoN_minusTTPt);

    histoFineBinning_RecoA_cTTPt->Divide(histoFineBinning_RecoN_plusMinusN_minusTTPt, histoFineBinning_RecoN_plusPlusN_minusTTPt);

    histoFineBinning_RecoN_plusMinusN_minusTTMass->Add(histoFineBinning_RecoN_plusTTMass);
    histoFineBinning_RecoN_plusMinusN_minusTTMass->Add(histoFineBinning_RecoN_minusTTMass,-1);
    histoFineBinning_RecoN_plusPlusN_minusTTMass->Add(histoFineBinning_RecoN_plusTTMass);
    histoFineBinning_RecoN_plusPlusN_minusTTMass->Add(histoFineBinning_RecoN_minusTTMass);

    histoFineBinning_RecoA_cTTMass->Divide(histoFineBinning_RecoN_plusMinusN_minusTTMass, histoFineBinning_RecoN_plusPlusN_minusTTMass);



    histoDelta_yEfficiency->Divide(histoDelta_yEfficiencyN, histoDelta_yEfficiencyD);
    histoFineBinning_Delta_yEfficiency->Divide(histoFineBinning_Delta_yEfficiencyN, histoFineBinning_Delta_yEfficiencyD);


    for (unsigned int bin_index1 = 1; bin_index1 < 11; bin_index1++) {
     for (unsigned int bin_index2 = 1; bin_index2 < 6; bin_index2++) {
       float value = histoRecoDelta_yVsTTRapidity->GetBinContent(bin_index1,bin_index2)/histoGenDelta_yVsTTRapidity->GetBinContent(bin_index1,bin_index2);
       histoDelta_yVsTTRapidityEfficiency->SetBinContent(bin_index1, bin_index2, value);
       float value_fb = histoFineBinning_RecoDelta_yVsTTRapidity->GetBinContent(bin_index1,bin_index2)/histoFineBinning_GenDelta_yVsTTRapidity->GetBinContent(bin_index1,bin_index2);
       histoFineBinning_Delta_yVsTTRapidityEfficiency->SetBinContent(bin_index1, bin_index2, value_fb);

     }
    }

    for (unsigned int bin_index1 = 1; bin_index1 < 11; bin_index1++) {
     for (unsigned int bin_index2 = 1; bin_index2 < 6; bin_index2++) {
       float value = histoRecoDelta_yVsTTPt->GetBinContent(bin_index1,bin_index2)/histoGenDelta_yVsTTPt->GetBinContent(bin_index1,bin_index2);
       histoDelta_yVsTTPtEfficiency->SetBinContent(bin_index1, bin_index2, value);
       float value_fb = histoFineBinning_RecoDelta_yVsTTPt->GetBinContent(bin_index1,bin_index2)/histoFineBinning_GenDelta_yVsTTPt->GetBinContent(bin_index1,bin_index2);
       histoFineBinning_Delta_yVsTTPtEfficiency->SetBinContent(bin_index1, bin_index2, value_fb);

     }
    }


    for (unsigned int bin_index1 = 1; bin_index1 < 11; bin_index1++) {
     for (unsigned int bin_index2 = 1; bin_index2 < 6; bin_index2++) {
       float value = histoRecoDelta_yVsTTMass->GetBinContent(bin_index1,bin_index2)/histoGenDelta_yVsTTMass->GetBinContent(bin_index1,bin_index2);
       histoDelta_yVsTTMassEfficiency->SetBinContent(bin_index1, bin_index2, value);
       float value_fb = histoFineBinning_RecoDelta_yVsTTPt->GetBinContent(bin_index1,bin_index2)/histoFineBinning_GenDelta_yVsTTPt->GetBinContent(bin_index1,bin_index2);
       histoFineBinning_Delta_yVsTTPtEfficiency->SetBinContent(bin_index1, bin_index2, value_fb);

     }
    }






  }				// end of loop over the datasets


//  if(verbosity>0) {



    cout<<"#########################"<<endl;
    cout<<"    End of the program   "<<endl;
    cout<<"#########################"<<endl;
//  }

    tree->Print();

    TupleFile->Write();
    TupleFile->Close();


    TFile * fileCA = new TFile("ChargeAsymmetryResults_Skimmed_TTbarSignalMadgraph_JER30.root","RECREATE");
    //  fileCA->cd();

    // GEN
    histoGenTopRapidity1->Write();
    histoGenTopRapidity2->Write();
    histoGenTopMass1->Write();
    histoGenTopMass2->Write();
    histoGenTopMass1VsTopMass2->Write();
    histoGenDelta_y->Write();
    histoGenTTRapidity->Write();
    histoGenTTPt->Write();
    histoGenTTMass->Write();
    histoGenDelta_yVsTTRapidity->Write();
    histoGenDelta_yVsTTPt->Write();
    histoGenDelta_yVsTTMass->Write();
    histoGenN_plusTTRapidity->Write();
    histoGenN_plusTTPt->Write();
    histoGenN_plusTTMass->Write();
    histoGenN_minusTTRapidity->Write();
    histoGenN_minusTTPt->Write();
    histoGenN_minusTTMass->Write();
    histoGenN_plusMinusN_minusTTRapidity->Write();
    histoGenN_plusPlusN_minusTTRapidity->Write();
    histoGenN_plusMinusN_minusTTPt->Write();
    histoGenN_plusPlusN_minusTTPt->Write();
    histoGenN_plusMinusN_minusTTMass->Write();
    histoGenN_plusPlusN_minusTTMass->Write();
    histoGenA_cTTRapidity->Write();
    histoGenA_cTTPt->Write();
    histoGenA_cTTMass->Write();
    histoFineBinning_GenDelta_yVsTTRapidity->Write();
    histoFineBinning_GenDelta_yVsTTPt->Write();
    histoFineBinning_GenDelta_yVsTTMass->Write();
    histoFineBinning_GenN_plusTTRapidity->Write();
    histoFineBinning_GenN_plusTTPt->Write();
    histoFineBinning_GenN_plusTTMass->Write();
    histoFineBinning_GenN_minusTTRapidity->Write();
    histoFineBinning_GenN_minusTTPt->Write();
    histoFineBinning_GenN_minusTTMass->Write();
    histoFineBinning_GenN_plusMinusN_minusTTRapidity->Write();
    histoFineBinning_GenN_plusPlusN_minusTTRapidity->Write();
    histoFineBinning_GenN_plusMinusN_minusTTPt->Write();
    histoFineBinning_GenN_plusPlusN_minusTTPt->Write();
    histoFineBinning_GenN_plusMinusN_minusTTMass->Write();
    histoFineBinning_GenN_plusPlusN_minusTTMass->Write();
    histoFineBinning_GenA_cTTRapidity->Write();
    histoFineBinning_GenA_cTTPt->Write();
    histoFineBinning_GenA_cTTMass->Write(); 



    // RECO
    histoRecoTopRapidity1->Write();
    histoRecoTopRapidity2->Write();
    histoRecoLeptonRapidity1->Write();
    histoRecoLeptonRapidity2->Write();
    histoRecoTopMass1->Write();
    histoRecoTopMass2->Write();
    histoRecoTopMass1VsTopMass2->Write();
    histoRecoDelta_y->Write();
    histoRecoTTRapidity->Write();
    histoRecoTTPt->Write();
    histoRecoTTMass->Write();
    histoRecoDelta_yVsTTRapidity->Write();
    histoRecoDelta_yVsTTPt->Write();
    histoRecoDelta_yVsTTMass->Write();
    histoRecoN_plusTTRapidity->Write();
    histoRecoN_plusTTPt->Write();
    histoRecoN_plusTTMass->Write();
    histoRecoN_minusTTRapidity->Write();
    histoRecoN_minusTTPt->Write();
    histoRecoN_minusTTMass->Write();
    histoRecoN_plusMinusN_minusTTRapidity->Write();
    histoRecoN_plusPlusN_minusTTRapidity->Write();
    histoRecoN_plusMinusN_minusTTPt->Write();
    histoRecoN_plusPlusN_minusTTPt->Write();
    histoRecoN_plusMinusN_minusTTMass->Write();
    histoRecoN_plusPlusN_minusTTMass->Write();
    histoRecoA_cTTRapidity->Write();
    histoRecoA_cTTPt->Write();
    histoRecoA_cTTMass->Write();
    histoFineBinning_RecoDelta_yVsTTRapidity->Write();
    histoFineBinning_RecoDelta_yVsTTPt->Write();
    histoFineBinning_RecoDelta_yVsTTMass->Write();
    histoFineBinning_RecoN_plusTTRapidity->Write();
    histoFineBinning_RecoN_plusTTPt->Write();
    histoFineBinning_RecoN_plusTTMass->Write();
    histoFineBinning_RecoN_minusTTRapidity->Write();
    histoFineBinning_RecoN_minusTTPt->Write();
    histoFineBinning_RecoN_minusTTMass->Write();
    histoFineBinning_RecoN_plusMinusN_minusTTRapidity->Write();
    histoFineBinning_RecoN_plusPlusN_minusTTRapidity->Write();
    histoFineBinning_RecoN_plusMinusN_minusTTPt->Write();
    histoFineBinning_RecoN_plusPlusN_minusTTPt->Write();
    histoFineBinning_RecoN_plusMinusN_minusTTMass->Write();
    histoFineBinning_RecoN_plusPlusN_minusTTMass->Write();
    histoFineBinning_RecoA_cTTRapidity->Write();
    histoFineBinning_RecoA_cTTPt->Write();
    histoFineBinning_RecoA_cTTMass->Write(); 

    // RECO vs GEN
    histoRecoVsGenTopRapidity1->Write();
    histoRecoVsGenTopRapidity2->Write();
    histoRecoMinusGenDivGenLeptonRapidity1->Write();
    histoRecoMinusGenDivGenLeptonRapidity2->Write();
    histoRecoMinusGenDivGenTopRapidity1->Write();
    histoRecoMinusGenDivGenTopRapidity2->Write();
    histoRecoVsGenTopMass1->Write();
    histoRecoVsGenTopMass2->Write();
    histoRecoMinusGenDivGenTopMass1->Write();
    histoRecoMinusGenDivGenTopMass2->Write();
    histoRecoVsGenDelta_y->Write();
    histoFineBinning_RecoVsGenDelta_y->Write();
    histoRecoMinusGenDivGenDelta_y->Write();
    histoRecoVsGenTTRapidity->Write();
    histoRecoMinusGenDivGenTTRapidity->Write();
    histoRecoVsGenTTPt->Write();
    histoRecoMinusGenDivGenTTPt->Write();
    histoRecoVsGenTTMass->Write();
    histoRecoMinusGenDivGenTTMass->Write();
    histoDelta_yEfficiencyN->Write();
    histoDelta_yEfficiencyD->Write();
    histoDelta_yEfficiency->Write();
    histoDelta_yVsTTRapidityEfficiency->Write();
    histoDelta_yVsTTPtEfficiency->Write();
    histoDelta_yVsTTMassEfficiency->Write();
    histoFineBinning_Delta_yEfficiencyN->Write();
    histoFineBinning_Delta_yEfficiencyD->Write();
    histoFineBinning_Delta_yEfficiency->Write();
    histoFineBinning_Delta_yVsTTRapidityEfficiency->Write();
    histoFineBinning_Delta_yVsTTPtEfficiency->Write();
    histoFineBinning_Delta_yVsTTMassEfficiency->Write();


    //    fileCA->Write();
    fileCA->Close();
    delete fileCA;

    cout << "num_mttbar_eff = " <<  num_mttbar_eff << endl;
    cout << "den_mttbar_eff = " <<  den_mttbar_eff << endl;
    cout << "mttbar_eff = " <<  num_mttbar_eff / den_mttbar_eff << endl;

  return (0);
}
