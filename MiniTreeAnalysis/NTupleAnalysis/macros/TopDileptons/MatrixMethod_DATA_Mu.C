#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"


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
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/MatrixMethod_DATA_Mu.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  vector < Dataset > datasets;
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  int verbosity = -1;
  // Matrix Method
  int MMselStepCut = 6;
  float looseIsoMM = 0.8; float tightIsoMM = 0.2;
  unsigned int nbinsMM = 11; float lowEdgeMM = -0.5; float highEdgeMM = 10.5;
  MMEstimation MMestMuMu(datasets,looseIsoMM, tightIsoMM, nbinsMM, lowEdgeMM, highEdgeMM, "MuMu"); 
  //struct MMEstimatedPlots theMMEstimatedPlotsMuMu;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsMuMu;
  //vector<struct MMEstimated> theMMEstimatedValuesMuMu;

  DiLeptonSelection sel; 
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  TopTree::NTEvent * event = 0;
  //Selection table
  // Table: mumu
  SelectionTable selTable_mumu(sel.GetCutList(),datasets, string("mumu"));

  DiLeptonSelection sel_aux(sel);
  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
  for (unsigned int d = 0; d < datasets.size (); d++) {
   TRandom randomize;
   datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
   cout << "dataset = " << datasets[d].Name() << endl;
   unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    float weight = 1;
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      datasets[d].eventTree ()->GetEntry (ievt);
      //Load event for the selection
      sel.LoadEvent(event);
      sel_aux.LoadEvent(event);
      if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
      MMestMuMu.CountNSel(sel_aux, datasets[d], selTable_mumu, d, weight, "mumu", MMselStepCut);
    } // end of loop over evts
  } // end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
  
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////
  // Matrix Method estimation for mumu case
  vector<struct MMEpsilons> valMMEpsilons;
  for(unsigned int bin_index = 0; bin_index < nbinsMM; bin_index++){
    struct MMEpsilons valMMEpsilonsTmp;
    valMMEpsilonsTmp.EpsilonSignal = 0.99;
    valMMEpsilonsTmp.EpsilonSignalErr = 0.05;
    valMMEpsilonsTmp.EpsilonFake = 0.20;
    valMMEpsilonsTmp.EpsilonFakeErr = 0.10;
    valMMEpsilons.push_back(valMMEpsilonsTmp);
  }
  unsigned int NbIterations = 10000;
  bool doStatistical = true; bool doSystematic = true; bool doCorrections = true;
  MMestMuMu.RunTheMatrixMethod(valMMEpsilons,  NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesMuMu = MMestMuMu.GetMMEstimated();
  //theMMEstimatedPlotsMuMu = MMestMuMu.GetMMEstimatedPlots();
  //theMMExpectedPlotsMuMu = MMestMuMu.GetMMExpectedPlots();
  MMestMuMu.PrintMMEstimated();
  MMestMuMu.WriteMMFile("MatrixMethod_OutPut_MuMuCase_DATA.root");

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
