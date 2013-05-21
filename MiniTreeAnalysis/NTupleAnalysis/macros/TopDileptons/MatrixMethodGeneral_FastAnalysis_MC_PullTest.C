#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../BckgdEstimation/interface/GeneralMMEstimation.h"

int main ()
{
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;
  
  //////////////////////
  // Initialisation
  //////////////////////
  vector < Dataset > datasets;
  int verbosity = -1;

  ////////////////////////////////
  // Matrix Method declaration
  ///////////////////////////////
  float looseIsoMM = 0.8; float tightIsoMMmu = 0.2; float tightIsoMMe = 0.17;
  unsigned int nbinsMM = 11; float lowEdgeMM = -0.5; float highEdgeMM = 10.5;
  GeneralMMEstimation MMestEMu(datasets, looseIsoMM, tightIsoMMmu, tightIsoMMe, nbinsMM, lowEdgeMM, highEdgeMM, "EMu");
  //struct MMEstimatedPlots theMMEstimatedPlotsEMu;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsEMu;
  //vector<struct MMEstimated> theMMEstimatedValuesEMu;
  //////////////////////

  //////////////////////////////////////////
  // Running the Matrix Method in fast way
  //////////////////////////////////////////
  MMestEMu.ReadMMFileForPullTest("MatrixMethod_OutPut_EMuCase_MC.root", 0.95, 0.2, 0.95, 0.2);
 

  vector<struct MMEpsilons> valMMEpsilons;
  for(unsigned int bin_index = 0; bin_index < nbinsMM; bin_index++){
    struct MMEpsilons valMMEpsilonsTmp;
    valMMEpsilonsTmp.EpsilonESignal = 0.95;
    valMMEpsilonsTmp.EpsilonESignalErr = 0.1;
    valMMEpsilonsTmp.EpsilonEFake = 0.2;
    valMMEpsilonsTmp.EpsilonEFakeErr = 0.1;
    valMMEpsilonsTmp.EpsilonMuSignal = 0.95;
    valMMEpsilonsTmp.EpsilonMuSignalErr = 0.1;
    valMMEpsilonsTmp.EpsilonMuFake = 0.2;
    valMMEpsilonsTmp.EpsilonMuFakeErr = 0.1;
    valMMEpsilons.push_back(valMMEpsilonsTmp);
  }

  unsigned int NbIterations = 100000;
  bool doStatistical = true; bool doSystematic = false; bool doCorrections = false;
  MMestEMu.RunTheMatrixMethod(valMMEpsilons, NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesEMu = MMestEMu.GetMMEstimated(); // MM "Estimated" values
  //theMMEstimatedPlotsEMu = MMestEMu.GetMMEstimatedPlots(); // MM "Estimated" Plots...    
  //theMMExpectedPlotsEMu = MMestEMu.GetMMExpectedPlots(); // MM "Expected" Plots
  MMestEMu.PrintMMEstimated();
  MMestEMu.WriteMMFileFastForPullTest("MatrixMethod_OutPut_EMuCase_MC.root","MatrixMethod_OutPut_EMuCase_MC_Fast_PullTest.root", 0.95, 0.2, 0.95, 0.2);
  
  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
