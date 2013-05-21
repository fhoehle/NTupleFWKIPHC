#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"

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
  float looseIsoMM = 0.8; float tightIsoMM = 0.17;
  unsigned int nbinsMM = 11; float lowEdgeMM = -0.5; float highEdgeMM = 10.5;
  MMEstimation MMestEE(datasets, looseIsoMM, tightIsoMM, nbinsMM, lowEdgeMM, highEdgeMM, "EE");
  //struct MMEstimatedPlots theMMEstimatedPlotsEE;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsEE;
  //vector<struct MMEstimated> theMMEstimatedValuesEE;

  //////////////////////////////////////////
  // Running the Matrix Method in fast way
  //////////////////////////////////////////
  MMestEE.ReadMMFile("MatrixMethod_OutPut_EECase_DATA.root");

  TH1F * hSignalEfficiencyNJets_EE = new TH1F("hSignalEfficiencyNJets_EE", "hSignalEfficiencyNJets_EE", nbinsMM, lowEdgeMM, highEdgeMM);
  TH1F * hFakeRateNJets_EE = new TH1F("hFakeRateNJets_EE", "hFakeRateNJets_EE", nbinsMM, lowEdgeMM, highEdgeMM);

  TFile* file_EE = new TFile("MatrixMethod_Efficiency_DATA_E.root");
  file_EE->cd();
  hSignalEfficiencyNJets_EE = (TH1F*)gDirectory->Get("SignalEfficiencyNJetsSyst");
  hFakeRateNJets_EE = (TH1F*)gDirectory->Get("FakeRateNJetsSyst");

  vector<struct MMEpsilons> valMMEpsilons_EE;
  for(unsigned int bin_index = 0; bin_index < nbinsMM; bin_index++){                        
    struct MMEpsilons valMMEpsilonsTmp_EE;
    valMMEpsilonsTmp_EE.EpsilonSignal = hSignalEfficiencyNJets_EE->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_EE.EpsilonSignalErr = hSignalEfficiencyNJets_EE->GetBinError(bin_index+1);
    valMMEpsilonsTmp_EE.EpsilonFake = hFakeRateNJets_EE->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_EE.EpsilonFakeErr = hFakeRateNJets_EE->GetBinError(bin_index+1);
    valMMEpsilons_EE.push_back(valMMEpsilonsTmp_EE);
  }

  file_EE->Close();
  delete file_EE;

  unsigned int NbIterations = 100000;
  bool doStatistical = true; bool doSystematic = true; bool doCorrections = true;
  MMestEE.RunTheMatrixMethod(valMMEpsilons_EE, NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesEE = MMestEE.GetMMEstimated(); // MM "Estimated" values
  //theMMEstimatedPlotsEE = MMestEE.GetMMEstimatedPlots(); // MM "Estimated" Plots...    
  //theMMExpectedPlotsEE = MMestEE.GetMMExpectedPlots(); // MM "Expected" Plots
  MMestEE.PrintMMEstimated();
  MMestEE.WriteMMFileFast("MatrixMethod_OutPut_EECase_DATA_Fast.root");

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
