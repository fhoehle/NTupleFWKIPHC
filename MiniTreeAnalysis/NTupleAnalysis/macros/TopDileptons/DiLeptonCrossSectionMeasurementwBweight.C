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
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../../config/DiLeptonXsectionMeasurement.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  TopTree::NTEvent * event = 0;

//***    BTAG WEIGHT ****
/* init for btag weight : DONE IN XML FILE!
  int flagb =0 ; // 0 = do not apply,  1 = apply
  int methodb = 0; // 0 = SF w discri cut, 1 = Eff_data w/o discri cut, 2 = SF * Eff_MC
  int systb =0;	// 0 = weight, 1 = weight + error, 2 = weight - error
  sel.InitSFBWeight(flagb, methodb, systb);  
*/
/* Reset is possible :
  flagb=0;
  systb=1;
  sel.ResetParameters4Bweight(flagb, methodb, systb); 
*/


  //Selection table
  // 4 tables: ee - emu - mumu - allChannels
  SelectionTable selTable_allChannels(sel.GetCutList(),datasets, string("*"));
  SelectionTable selTable_ee(  sel.GetCutList(),datasets, string("ee")  );
  SelectionTable selTable_emu( sel.GetCutList(),datasets, string("emu") );
  SelectionTable selTable_mumu(sel.GetCutList(),datasets, string("mumu"));

  //Book keeping of standard histos
  DiLepAnaHistoManager histoManager;
  histoManager.LoadDatasets(datasets);    
  histoManager.LoadSelectionSteps(sel.GetCutList());
  histoManager.LoadChannels(sel.GetChannelList());
  histoManager.CreateHistos();	
  //////////////////////
 
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
    cout << "NEvents = " << datasets[d].NofEvtsToRunOver() << endl;
    
  
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < datasets[d].NofEvtsToRunOver(); ievt++) {
      float weight = 1.;
      vector<float> weightb;
      weightb.push_back(weight);  // weight of the event
      weightb.push_back(0.);      // Proba of 0 jet
      weightb.push_back(0.);      // Proba of 1 jet;
      weightb.push_back(0.);      // Proba of 2 jets;
      weightb.push_back(0.);      // Proba of at least 3 jets;
      datasets[d].eventTree ()->GetEntry (ievt);
      if (verbosity > 3)
	cout << "event " << ievt << endl;
      if (verbosity > 3)
	cout << "event number=" << event->eventNb << endl;
      if (verbosity > 3)
	cout << "run number=" << event->runNb << endl;
     
     //cout << "event number=" << event->eventNb << endl;
     //Load event for the selection
     sel.LoadEvent(event);


     //Candidate pair of lepton : for histoManager
     string CandType; // ee - emu - mumum or false
     vector<NTElectron> candElec;
     vector<NTMuon> candMuon;
     sel.GetLeptonPair(candMuon, candElec, CandType ); // fill the variables


     //integer which define the last step of the selection that the event fullfill
     int selLastStep = 0;
     //Fill the selection table
     selLastStep = sel.FillTablewBweight(selTable_ee, &(datasets[d]), d, weight, weightb);
     if (CandType=="ee") {
       // Fill the histo here, as objects ForAna are dependent on the channel, as well as weightb (because of Jets removal!)      
       histoManager.FillwBweight(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight, weightb);
     }
     selLastStep = sel.FillTablewBweight(selTable_emu, &(datasets[d]), d, weight, weightb);
     if (CandType=="emu") {
       histoManager.FillwBweight(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight, weightb);
     }
     selLastStep = sel.FillTablewBweight(selTable_mumu, &(datasets[d]), d, weight, weightb);
     if (CandType=="mumu") {
       histoManager.FillwBweight(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight, weightb);
     }

    
     
    }  // end of loop over evts


  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 


 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////


  histoManager.Compute();
 
  ///////////////

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Fill the latex tables   "<<endl;
	cout<<"#########################"<<endl;
  }

  ofstream ofile("CrossSectionTable.tex");
  ofile<<"\\documentclass[8pt]{article}"<<endl;
  ofile<<"\\begin{document}"<<endl;
  //Merge channels consistently
  vector<string> mergenames;
  mergenames.push_back("TTbarEM");
  mergenames.push_back("TTbarMM");
  mergenames.push_back("TTbarOther");
  selTable_ee.MergeDatasets(mergenames, string("TTbarOther"));
  mergenames.clear();
  mergenames.push_back("TTbarEE");
  mergenames.push_back("TTbarMM");
  mergenames.push_back("TTbarOther");
  selTable_emu.MergeDatasets(mergenames, string("TTbarOther"));
  mergenames.clear();
  mergenames.push_back("TTbarEE");
  mergenames.push_back("TTbarEM");
  mergenames.push_back("TTbarOther");
  selTable_mumu.MergeDatasets(mergenames, string("TTbarOther"));
  mergenames.clear();
  mergenames.push_back("TTbarEE");
  mergenames.push_back("TTbarEM");
  mergenames.push_back("TTbarMM");
  selTable_allChannels.MergeDatasets(mergenames, string("TTbarSignal"));
  //Define signal
  selTable_ee.DefineFirstDataset(string("TTbarEE"));
  selTable_emu.DefineFirstDataset(string("TTbarEM"));
  selTable_mumu.DefineFirstDataset(string("TTbarMM"));
  selTable_allChannels.DefineFirstDataset(string("TTbarSignal"));
  //Calculations
  selTable_ee.TableCalculator();
  selTable_emu.TableCalculator();
  selTable_mumu.TableCalculator();
  selTable_allChannels.TableCalculator();
  //Write
  selTable_ee.Write(ofile);
  selTable_emu.Write(ofile);
  selTable_mumu.Write(ofile);
  selTable_allChannels.Write(ofile);
  ofile<<"\\end{document}"<<endl;
  system("pdflatex CrossSectionTable.tex");


  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Write output root file "<<endl;
	cout<<"#########################"<<endl;
  }
  TFile* fout  = new TFile("DiLepXsectionMeas.root","RECREATE");
  histoManager.Write(fout);
  //fout->Write();
  fout->Close();
  
  //Clear histos before deleting the TFile
  histoManager.Clear();  

  delete fout;

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<"    End of the program   "<<endl;
	cout<<"#########################"<<endl;
  }

  return (0);
}
