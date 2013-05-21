#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"
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
  MMEstimation MMestEE(datasets, looseIsoMM, tightIsoMMe, nbinsMM, lowEdgeMM, highEdgeMM, "EE"); 
  MMEstimation MMestMuMu(datasets,looseIsoMM, tightIsoMMmu, nbinsMM, lowEdgeMM, highEdgeMM, "MuMu"); 
  //struct MMEstimatedPlots theMMEstimatedPlotsEE;
  //struct MMEstimatedPlots theMMEstimatedPlotsMuMu;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsEE;
  //vector<struct MMExpectedPlots> theMMExpectedPlotsMuMu;
  //vector<struct MMEstimated> theMMEstimatedValuesEE;
  //vector<struct MMEstimated> theMMEstimatedValuesMuMu;

  DiLeptonSelection sel;
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  //Load for PU:
  sel.GeneratePUWeight(PUWeightFileName);

  float LuminosityEE = Luminosity;
  float LuminosityMuMu = Luminosity;

  TopTree::NTEvent * event = 0;
  //Selection table
  // Tables: ee - mumu
  SelectionTable selTable_ee(  sel.GetCutList(),datasets, string("ee")  );
  SelectionTable selTable_mumu(sel.GetCutList(),datasets, string("mumu"));

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
    float weightEE_init;
    float weightMuMu_init;
    weightEE_init = datasets[d].NormFactor()*LuminosityEE;
    weightMuMu_init = datasets[d].NormFactor()*LuminosityMuMu;
    cout << "weightEE_init = " << weightEE_init << endl;
    cout << "weightMuMu_init = " << weightMuMu_init << endl;
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      datasets[d].eventTree ()->GetEntry (ievt);
      //Load event for the selection
      sel.LoadEvent(event);
      sel_aux.LoadEvent(event);

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
      vector<NTVertex>   selVertices_aux  = sel_aux.GetSelectedVertex();


      //float weightEE = weightEE_init*sel.GetPUWeight();
      //float weightMuMu = weightMuMu_init*sel.GetPUWeight();
      float weightEE = 0;
      float weightMuMu = 0;
      if(thePUReweighter.getUseOutOfTimePU()){
         weightEE = weightEE_init*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);
         weightMuMu = weightMuMu_init*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);
       }else{
	 weightEE = weightEE_init*thePUReweighter.weight(event->num_pileup_bc0);
	 weightMuMu = weightMuMu_init*thePUReweighter.weight(event->num_pileup_bc0);
       }
     //MC
     double random = randomize.Uniform(1.);
       MMestEE.CountNSel(sel_aux, datasets[d], selTable_ee, d, weightEE, "ee", MMselStepCut);
       MMestMuMu.CountNSel(sel_aux, datasets[d], selTable_mumu, d, weightMuMu, "mumu", MMselStepCut);
       MMestEE.FillMMExpected(sel_aux, datasets[d], selTable_ee, d, weightEE, "ee", MMselStepCut, event);
       MMestMuMu.FillMMExpected(sel_aux, datasets[d], selTable_mumu, d, weightMuMu, "mumu", MMselStepCut, event);
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
    valMMEpsilonsTmp.EpsilonSignal = 0.99;
    valMMEpsilonsTmp.EpsilonSignalErr = 0.05;
    valMMEpsilonsTmp.EpsilonFake = 0.20;
    valMMEpsilonsTmp.EpsilonFakeErr = 0.10;
    valMMEpsilons.push_back(valMMEpsilonsTmp);
  }
  unsigned int NbIterations = 10000;
  bool doStatistical = true; bool doSystematic = true; bool doCorrections = true;
  MMestEE.RunTheMatrixMethod(valMMEpsilons, NbIterations, doStatistical, doSystematic, doCorrections);
  MMestMuMu.RunTheMatrixMethod(valMMEpsilons,  NbIterations, doStatistical, doSystematic, doCorrections);
  //theMMEstimatedValuesEE = MMestEE.GetMMEstimated(); // MM "Estimated" values
  //theMMEstimatedValuesMuMu = MMestMuMu.GetMMEstimated();
  //theMMEstimatedPlotsEE = MMestEE.GetMMEstimatedPlots(); // MM "Estimated" Plots... 
  //theMMEstimatedPlotsMuMu = MMestMuMu.GetMMEstimatedPlots();
  //theMMExpectedPlotsEE = MMestEE.GetMMExpectedPlots(); // MM "Expected" Plots
  //theMMExpectedPlotsMuMu = MMestMuMu.GetMMExpectedPlots();
  MMestEE.PrintMMEstimated();
  MMestMuMu.PrintMMEstimated();
  MMestEE.WriteMMFile("MatrixMethod_OutPut_EECase_MC.root");
  MMestMuMu.WriteMMFile("MatrixMethod_OutPut_MuMuCase_MC.root");

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
