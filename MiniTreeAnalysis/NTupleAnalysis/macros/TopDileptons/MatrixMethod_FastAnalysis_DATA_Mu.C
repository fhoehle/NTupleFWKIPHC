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
  float looseIsoMM = 0.8; float tightIsoMM = 0.2;
  unsigned int nbinsMM = 11; float lowEdgeMM = -0.5; float highEdgeMM = 10.5;
  MMEstimation MMestMuMu(datasets,looseIsoMM, tightIsoMM, nbinsMM, lowEdgeMM, highEdgeMM, "MuMu"); 
  //struct MMEstimatedPlots theMMEstimatedPlotsMuMu;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsMuMu;
  //vector<struct MMEstimated> theMMEstimatedValuesMuMu;

  //////////////////////////////////////////
  // Running the Matrix Method in fast way
  //////////////////////////////////////////
  MMestMuMu.ReadMMFile("MatrixMethod_OutPut_MuMuCase_DATA.root");

  TH1F * hSignalEfficiencyNJets_MuMu = new TH1F("hSignalEfficiencyNJets_MuMu", "hSignalEfficiencyNJets_MuMu", nbinsMM, lowEdgeMM, highEdgeMM);
  TH1F * hFakeRateNJets_MuMu = new TH1F("hFakeRateNJets_MuMu", "hFakeRateNJets_MuMu", nbinsMM, lowEdgeMM, highEdgeMM);

  TFile* file_MuMu = new TFile("MatrixMethod_Efficiency_DATA_Mu.root");
  file_MuMu->cd();
  hSignalEfficiencyNJets_MuMu = (TH1F*)gDirectory->Get("SignalEfficiencyNJetsSyst");
  hFakeRateNJets_MuMu = (TH1F*)gDirectory->Get("FakeRateNJetsSyst");

  vector<struct MMEpsilons> valMMEpsilons_MuMu;
  for(unsigned int bin_index = 0; bin_index < nbinsMM; bin_index++){                        
    struct MMEpsilons valMMEpsilonsTmp_MuMu;
    valMMEpsilonsTmp_MuMu.EpsilonSignal = hSignalEfficiencyNJets_MuMu->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_MuMu.EpsilonSignalErr = hSignalEfficiencyNJets_MuMu->GetBinError(bin_index+1);
    valMMEpsilonsTmp_MuMu.EpsilonFake = hFakeRateNJets_MuMu->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_MuMu.EpsilonFakeErr = hFakeRateNJets_MuMu->GetBinError(bin_index+1);
    valMMEpsilons_MuMu.push_back(valMMEpsilonsTmp_MuMu);
  }

  file_MuMu->Close();
  delete file_MuMu;

  unsigned int NbIterations = 100000;
  bool doStatistical = true; bool doSystematic = true; bool doCorrections = true;
  MMestMuMu.RunTheMatrixMethod(valMMEpsilons_MuMu,  NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesMuMu = MMestMuMu.GetMMEstimated();
  //theMMEstimatedPlotsMuMu = MMestMuMu.GetMMEstimatedPlots();
  //theMMExpectedPlotsMuMu = MMestMuMu.GetMMExpectedPlots();
  MMestMuMu.PrintMMEstimated();
  MMestMuMu.WriteMMFileFast("MatrixMethod_OutPut_MuMuCase_DATA_Fast.root");

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
