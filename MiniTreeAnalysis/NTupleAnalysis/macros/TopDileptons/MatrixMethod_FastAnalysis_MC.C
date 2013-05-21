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
  float looseIsoMM = 0.8; float tightIsoMMmu = 0.2; float tightIsoMMe = 0.17;
  unsigned int nbinsMM = 11; float lowEdgeMM = -0.5; float highEdgeMM = 10.5;
  MMEstimation MMestEE(datasets, looseIsoMM, tightIsoMMe, nbinsMM, lowEdgeMM, highEdgeMM, "EE");
  MMEstimation MMestMuMu(datasets,looseIsoMM, tightIsoMMmu, nbinsMM, lowEdgeMM, highEdgeMM, "MuMu"); 
  //struct MMEstimatedPlots theMMEstimatedPlotsEE;
  //struct MMEstimatedPlots theMMEstimatedPlotsMuMu;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsEE;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsMuMu;
  //vector<struct MMEstimated> theMMEstimatedValuesEE;
  //vector<struct MMEstimated> theMMEstimatedValuesMuMu;
  //////////////////////

  //////////////////////////////////////////
  // Running the Matrix Method in fast way
  //////////////////////////////////////////
  MMestEE.ReadMMFile("MatrixMethod_OutPut_EECase_MC.root");
  MMestMuMu.ReadMMFile("MatrixMethod_OutPut_MuMuCase_MC.root");

  TH1F * hSignalEfficiencyNJets_EE = new TH1F("hSignalEfficiencyNJets_EE", "hSignalEfficiencyNJets_EE", nbinsMM, lowEdgeMM, highEdgeMM);
  TH1F * hFakeRateNJets_EE = new TH1F("hFakeRateNJets_EE", "hFakeRateNJets_EE", nbinsMM, lowEdgeMM, highEdgeMM);
  TH1F * hSignalEfficiencyNJets_MuMu = new TH1F("hSignalEfficiencyNJets_MuMu", "hSignalEfficiencyNJets_MuMu", nbinsMM, lowEdgeMM, highEdgeMM);
  TH1F * hFakeRateNJets_MuMu = new TH1F("hFakeRateNJets_MuMu", "hFakeRateNJets_MuMu", nbinsMM, lowEdgeMM, highEdgeMM);

  TFile* file_EE = new TFile("MatrixMethod_Efficiency_MC_E.root");
  file_EE->cd();
  hSignalEfficiencyNJets_EE = (TH1F*)gDirectory->Get("SignalEfficiencyNJets");
  hFakeRateNJets_EE = (TH1F*)gDirectory->Get("FakeRateNJets");

  TFile* file_MuMu = new TFile("MatrixMethod_Efficiency_MC_Mu.root");
  file_MuMu->cd();
  hSignalEfficiencyNJets_MuMu = (TH1F*)gDirectory->Get("SignalEfficiencyNJets");
  hFakeRateNJets_MuMu = (TH1F*)gDirectory->Get("FakeRateNJets");

  vector<struct MMEpsilons> valMMEpsilons_EE;
  vector<struct MMEpsilons> valMMEpsilons_MuMu;
  for(unsigned int bin_index = 0; bin_index < nbinsMM; bin_index++){
    struct MMEpsilons valMMEpsilonsTmp_EE;
    struct MMEpsilons valMMEpsilonsTmp_MuMu;
    valMMEpsilonsTmp_EE.EpsilonSignal = hSignalEfficiencyNJets_EE->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_EE.EpsilonSignalErr = hSignalEfficiencyNJets_EE->GetBinError(bin_index+1);
    valMMEpsilonsTmp_EE.EpsilonFake = hFakeRateNJets_EE->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_EE.EpsilonFakeErr = hFakeRateNJets_EE->GetBinError(bin_index+1);
    valMMEpsilonsTmp_MuMu.EpsilonSignal = hSignalEfficiencyNJets_MuMu->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_MuMu.EpsilonSignalErr = hSignalEfficiencyNJets_MuMu->GetBinError(bin_index+1);
    valMMEpsilonsTmp_MuMu.EpsilonFake = hFakeRateNJets_MuMu->GetBinContent(bin_index+1);
    valMMEpsilonsTmp_MuMu.EpsilonFakeErr = hFakeRateNJets_MuMu->GetBinError(bin_index+1);
    valMMEpsilons_EE.push_back(valMMEpsilonsTmp_EE);
    valMMEpsilons_MuMu.push_back(valMMEpsilonsTmp_MuMu);
  }

  file_EE->Close();
  delete file_EE;
  file_MuMu->Close();
  delete file_MuMu;

  unsigned int NbIterations = 100000;
  bool doStatistical = true; bool doSystematic = true; bool doCorrections = true;
  MMestEE.RunTheMatrixMethod(valMMEpsilons_EE, NbIterations, doStatistical, doSystematic, doCorrections);
  MMestMuMu.RunTheMatrixMethod(valMMEpsilons_MuMu,  NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesEE = MMestEE.GetMMEstimated(); // MM "Estimated" values
  //theMMEstimatedValuesMuMu = MMestMuMu.GetMMEstimated();
  //theMMEstimatedPlotsEE = MMestEE.GetMMEstimatedPlots(); // MM "Estimated" Plots...    
  //theMMEstimatedPlotsMuMu = MMestMuMu.GetMMEstimatedPlots();
  //theMMExpectedPlotsEE = MMestEE.GetMMExpectedPlots(); // MM "Expected" Plots
  //theMMExpectedPlotsMuMu = MMestMuMu.GetMMExpectedPlots();
  MMestEE.PrintMMEstimated();
  MMestMuMu.PrintMMEstimated();
  MMestEE.WriteMMFileFast("MatrixMethod_OutPut_EECase_MC_Fast.root");
  MMestMuMu.WriteMMFileFast("MatrixMethod_OutPut_MuMuCase_MC_Fast.root");

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
