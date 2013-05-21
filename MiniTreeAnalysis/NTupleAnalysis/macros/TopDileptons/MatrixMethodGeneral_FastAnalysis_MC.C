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
  MMestEMu.ReadMMFile("MatrixMethod_OutPut_EMuCase_MC.root");

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

  vector<struct MMEpsilons> valMMEpsilons;
  for(unsigned int bin_index = 0; bin_index < nbinsMM; bin_index++){
    struct MMEpsilons valMMEpsilonsTmp;
    valMMEpsilonsTmp.EpsilonESignal = hSignalEfficiencyNJets_EE->GetBinContent(bin_index+1);
    valMMEpsilonsTmp.EpsilonESignalErr = hSignalEfficiencyNJets_EE->GetBinError(bin_index+1);
    valMMEpsilonsTmp.EpsilonEFake = hFakeRateNJets_EE->GetBinContent(bin_index+1);
    valMMEpsilonsTmp.EpsilonEFakeErr = hFakeRateNJets_EE->GetBinError(bin_index+1);
    valMMEpsilonsTmp.EpsilonMuSignal = hSignalEfficiencyNJets_MuMu->GetBinContent(bin_index+1);
    valMMEpsilonsTmp.EpsilonMuSignalErr = hSignalEfficiencyNJets_MuMu->GetBinError(bin_index+1);
    valMMEpsilonsTmp.EpsilonMuFake = hFakeRateNJets_MuMu->GetBinContent(bin_index+1);
    valMMEpsilonsTmp.EpsilonMuFakeErr = hFakeRateNJets_MuMu->GetBinError(bin_index+1);
    valMMEpsilons.push_back(valMMEpsilonsTmp);
  }

  file_EE->Close();
  delete file_EE;
  file_MuMu->Close();
  delete file_MuMu;

  unsigned int NbIterations = 100000;
  bool doStatistical = true; bool doSystematic = true; bool doCorrections = true;
  MMestEMu.RunTheMatrixMethod(valMMEpsilons, NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesEMu = MMestEMu.GetMMEstimated(); // MM "Estimated" values
  //theMMEstimatedPlotsEMu = MMestEMu.GetMMEstimatedPlots(); // MM "Estimated" Plots...    
  //theMMExpectedPlotsEMu = MMestEMu.GetMMExpectedPlots(); // MM "Expected" Plots
  MMestEMu.PrintMMEstimated();
  MMestEMu.WriteMMFileFast("MatrixMethod_OutPut_EMuCase_MC_Fast.root");

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
