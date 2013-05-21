#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../BckgdEstimation/interface/GeneralMMEstimation.h"
#include "../../Tools/interface/PUWeighting.h"


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
  float Luminosity = 0;
  float LumiError = 0;
  string PUWeightFileName;
  int DataType = 0; 

  string xmlFileName = string ("../../config/MatrixMethod_MC.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  vector < Dataset > datasets;
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  int verbosity = -1;

  // Matrix Method
  int MMselStepCut = 6;
  float looseIsoMM = 0.8; float tightIsoMMmu = 0.2; float tightIsoMMe = 0.17;
  unsigned int nbinsMM = 11; float lowEdgeMM = -0.5; float highEdgeMM = 10.5;
  GeneralMMEstimation MMestEMu(datasets, looseIsoMM, tightIsoMMmu, tightIsoMMe, nbinsMM, lowEdgeMM, highEdgeMM, "EMu"); 
  //struct MMEstimatedPlots theMMEstimatedPlotsEMu;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsEMu;
  //vector<struct MMEstimated> theMMEstimatedValuesEMu;

  DiLeptonSelection sel;
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  //Load for PU:
  sel.GeneratePUWeight(PUWeightFileName);

  TopTree::NTEvent * event = 0;
  //Selection table
  // Table: emu
  SelectionTable selTable_emu( sel.GetCutList(),datasets, string("emu") );

  PUWeighting  thePUReweighter;
  TFile* file1  = new TFile(PUWeightFileName.c_str(),"READ"); 
  TH1D *  hPUData = 0;
  hPUData         = (TH1D*)file1->Get("pileup");
  TH1F *  hPUMC   = new TH1F("pileup_MC", "pileup_MC", hPUData->GetXaxis()->GetNbins(), hPUData->GetXaxis()->GetXmin(), hPUData->GetXaxis()->GetXmax() );
  TFile* file2  = new TFile( "../data/CrossSection_pileup.root" ,"READ");
  hPUMC           = (TH1F*)file2->Get("pileup_TTbarSig");
  // histo in data, histo in Mc, use out-of-time pu in the reweighting
  cout << "get MC histo  " << endl;
  thePUReweighter.setPUHisto( hPUData, hPUMC);
  cout << "set MC histo in thePUReweighter " << endl;
  thePUReweighter.setUseOutOfTimePU(false); // set to true to use out-of-time PU

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
    float weight_init;
    weight_init = datasets[d].NormFactor()*Luminosity;
    cout << "weight_init = " << weight_init << endl;
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      datasets[d].eventTree ()->GetEntry (ievt);
      //Load event for the selection
      sel.LoadEvent(event);
      if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
      //Manage DY samples to avoid overlaps
      double dileptInvMass = 0;
      if( (event->zAndDecays).size() > 0){
        TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen;
        dileptInvMass = dilept.M();
      }
      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50) continue;
      if(datasets[d].Name()=="DYToMuMu_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToEE_M-20"        && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToTauTau_M-20"    && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToEE_M-10To20"    &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToTauTau_M-10To20" &&  dileptInvMass > 20) continue;
      vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
      //float weight = weight_init*sel.GetPUWeight();
      float weight = 0;
      if(thePUReweighter.getUseOutOfTimePU()){
         weight = weight_init*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);
      }else{
         weight = weight_init*thePUReweighter.weight(event->num_pileup_bc0);
      }
     //MC
     double random = randomize.Uniform(1.);
       MMestEMu.CountNSel(sel, datasets[d], selTable_emu, d, weight, "emu", MMselStepCut);
       MMestEMu.FillMMExpected(sel, datasets[d], selTable_emu, d, weight, "emu", MMselStepCut, event);
    } // end of loop over evts
  } // end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
  
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////
  // Matrix Method estimation for ee and mumu cases 
  vector<struct MMEpsilons> valMMEpsilons; 
  for(unsigned int bin_index = 0; bin_index < nbinsMM; bin_index++){
    struct MMEpsilons valMMEpsilonsTmp;
    valMMEpsilonsTmp.EpsilonESignal = 0.99;
    valMMEpsilonsTmp.EpsilonESignalErr = 0.05;
    valMMEpsilonsTmp.EpsilonEFake = 0.20;
    valMMEpsilonsTmp.EpsilonEFakeErr = 0.10;
    valMMEpsilonsTmp.EpsilonMuSignal = 0.99;
    valMMEpsilonsTmp.EpsilonMuSignalErr = 0.05;
    valMMEpsilonsTmp.EpsilonMuFake = 0.20;
    valMMEpsilonsTmp.EpsilonMuFakeErr = 0.10;
    valMMEpsilons.push_back(valMMEpsilonsTmp);
  }
  unsigned int NbIterations = 10000;
  bool doStatistical = true; bool doSystematic = true; bool doCorrections = true;
  MMestEMu.RunTheMatrixMethod(valMMEpsilons, NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesEMu = MMestEMu.GetMMEstimated(); // MM "Estimated" values
  //theMMEstimatedPlotsEMu = MMestEMu.GetMMEstimatedPlots(); // MM "Estimated" Plots... 
  //theMMExpectedPlotsEMu = MMestEMu.GetMMExpectedPlots(); // MM "Expected" Plots
  MMestEMu.PrintMMEstimated();
  MMestEMu.WriteMMFile("MatrixMethod_OutPut_EMuCase_MC.root");

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
