#include <iomanip>
#include <iostream>
#include "../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../Selection/interface/SelectionTable.h"
#include "../Tools/interface/Dataset.h"
#include "../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../Plots/interface/SemiLeptonicTauAnaHistoManager.h"
#include "../Selection/interface/SemiLeptonicTauSelection.h"
#include "../Tools/interface/PUWeighting.h"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TRandom.h>

#include "TMVA/Reader.h"

using namespace TopTree;
using namespace std;

int main ()
{
 
  clock_t start = clock();
  clock_t tl_tableDATA = 0;
  clock_t tl_tableMC = 0;
  clock_t time;
 
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;
  
  //////////////////////
  //Global variables
  //////////////////////
  vector < Dataset > datasets;
  SemiLeptonicTauSelection sel; 
  float Luminosity = 0.;
  float LumiError = 0.;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;
  string PUWeightFileName;

  //////////////////////
 
  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../config/SemiLeptonicTauXsectionMeasurement.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSemiLeptonicTauSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  
  //int flagOriginal=sel.GetFlagb();
  //int methodOriginal=sel.GetMethodb();
  //int systOriginal= sel.GetSystb();
  //std::cout << " For btag : flag " << flagOriginal << ", method " << methodOriginal << ", syst " << systOriginal << std::endl;
  TopTree::NTEvent * event = 0;
   
 
  //////////////////////
  // Reweighting
  //////////////////////
  
  bool IReweight = true;
  if (DataType == 1) IReweight = false;
   
  bool IReweight_Spring11 = false;
  
  std::vector<TH1F> pileup;

  PUWeighting  thePUReweighter_40;
  PUWeighting  thePUReweighter_45;
  
  if (IReweight ) {
    
    TFile* file1  = new TFile("pudist_786.root","READ");
    TH1D *  hPUData_40 = 0;
    hPUData_40 = (TH1D*)file1->Get("pileup");
    
    TFile* file3  = new TFile("pudist_1-2fb.root","READ");
    TH1D *  hPUData_45 = 0; 
    hPUData_45 = (TH1D*)file3->Get("pileup");
      
    TH1D *  hPUMC   = new TH1D("pileup_MC", "pileup_MC", hPUData_45->GetXaxis()->GetNbins(), hPUData_45->GetXaxis()->GetXmin(), hPUData_45->GetXaxis()->GetXmax() );
   
    //if non spring11 MC
    if(!IReweight_Spring11){ 
      
      TFile* file2  = new TFile( "Crosssection.root" ,"READ");
      hPUMC         = (TH1D*)file2->Get("pileup");
      
      thePUReweighter_40.setPUHisto( hPUData_40, hPUMC);
      thePUReweighter_40.setUseOutOfTimePU(false);
      
      thePUReweighter_45.setPUHisto( hPUData_45, hPUMC);
      thePUReweighter_45.setUseOutOfTimePU(false);
       
    } 
    else{
      thePUReweighter_40.setPUHisto( hPUData_40);
      thePUReweighter_40.setUseOutOfTimePU(false);
      
      thePUReweighter_45.setPUHisto( hPUData_45);
      thePUReweighter_45.setUseOutOfTimePU(false);
    }
  }
    
  sel.GetCutList();
  
  //Selection table
  SelectionTable selTable(sel.GetCutList(),datasets,string("tauhjetjet"));
  SelectionTable selDATATable(sel.GetCutList (),datasets,string ("tauhjetjet"));
  

  //Book keeping of standard histos
  SemiLeptonicTauAnaHistoManager histoManager;
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
    cout << "NEvents = " << nEvents << endl;
    cout <<" NEvents to run over, skimmed events = "<<datasets[d].NofEvtsToRunOver()<<" "<<datasets[d].getNSkimmedEvent()<<endl;
    
    
    //LOOP OVER THE EVENTS
    TRandom random;
    for (unsigned int ievt = 0; ievt < datasets[d].NofEvtsToRunOver(); ievt++) {
    
      float weight = 1.;
      if(datasets[d].isData() == false) //weight = datasets[d].NormFactor()*Luminosity; //if Data , weight = 1
      weight = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
      
      //cout<<"**weight "<<weight<<" " << Luminosity << " " << datasets[d].Xsection()<<" " << datasets[d].getNSkimmedEvent()<< endl;
     
      datasets[d].eventTree ()->GetEntry (ievt);
   
      if (verbosity > 3){
	cout << "event " << ievt <<" - event number=" << event->eventNb <<" - run number=" << event->runNb << endl;
      }
      if (ievt % 1000 == 0)
	      cout << "number of events " << ievt << endl;

     sel.LoadEvent(event);
      
     // QuadJet40_IsoPFTau40 selection 
     //if (datasets[d].isData() == true && ((sel.getRunNumber()>=165970 && sel.getRunNumber()<=166782) || sel.getRunNumber()>=171050)) continue;
    
     // QuadJet45_IsoPFTau45 selection 
     //if (datasets[d].isData() == true && (sel.getRunNumber()<165970 ||(sel.getRunNumber()>=166784 && sel.getRunNumber()<171050 ))) continue;
     
     //if (datasets[d].isData() == true) sel.QuadJet40IsoPFTau40Prescale();

     //integer which define the last step of the selection that the event fulfills
     int selLastStep = 0;
    
     //Fill the selection table
    
     float trigRange = random.Uniform(1);
    
     if (datasets[d].isData() == false) 
     {    
        if(thePUReweighter_40.getUseOutOfTimePU()) 
	{
	 if(trigRange<0.3837) 
	 weight *=sel.Trigger_QuadJet40_IsoPFTau40()*thePUReweighter_40.weight(event->num_pileup_bc0, event->runNb);///if you want include the Luminosity weight add: ( weightITypeMC_save*)      
	 else  
	 weight *=sel.Trigger_QuadJet45_IsoPFTau45()*thePUReweighter_45.weight(event->num_pileup_bc0, event->runNb);///if you want include the Luminosity weight add: ( weightITypeMC_save*)     
	 }
	 
	else
	{ 
	 if(trigRange<0.3837)
	 weight *= sel.Trigger_QuadJet40_IsoPFTau40()*thePUReweighter_40.weight(event->num_pileup_bc0); //weightITypeMC_save*}
         else
	 weight *= sel.Trigger_QuadJet45_IsoPFTau45()*thePUReweighter_45.weight(event->num_pileup_bc0); //weightITypeMC_save*}    
	}
      }
     
     sel.QuadJet45IsoPFTau45Prescale();
     
     if(datasets[d].isData()){
	 time = clock(); 
	 selLastStep = sel.FillTable (selDATATable, &(datasets[d]), d, weight); 
	 tl_tableDATA+=clock()-time;}
     else{  
         time = clock(); 
	
         selLastStep = sel.FillTable(selTable, &(datasets[d]), d, weight); 
	 tl_tableMC+=clock()-time;}
	 
     //std::cout <<"selLastStep" << selLastStep<< std::endl;
 
     if(!datasets[d].isData() && selLastStep>5)   
     histoManager.Fill(sel, selLastStep, 0, d, weight*sel.getWeightAtLeast1bjet()*sel.getWeightTau(), sel.GetNNOutputAndKinFit());
     else if(!datasets[d].isData() && selLastStep>3 && selLastStep<=5)   
     histoManager.Fill(sel, selLastStep, 0, d, weight*sel.getWeightTau(), sel.GetNNOutputAndKinFit());
     else
     histoManager.Fill(sel, selLastStep, 0, d, weight, sel.GetNNOutputAndKinFit());
    
    } // end of loop over evts
  }   // end of loop over the datasets 
  
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  histoManager.Compute();
 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Fill the latex tables   "<<endl;
	cout<<"#########################"<<endl;
  }

  ofstream ofile("SemiLeptonicTauCrossSectionTable.tex");
  ofile.precision(7);
  ofile<<"\\documentclass[8pt]{article}"<<endl;
  ofile<<"\\begin{document}"<<endl;
  //Calculations
  if(DataType==1) selDATATable.TableCalculator();
  else selTable.TableCalculator();
  //Write
  if(DataType==1) selDATATable.Write(ofile,true,3);
  else selTable.Write(ofile,true,3);
  
  ofile<<"\\end{document}"<<endl;
  system("pdflatex SemiLeptonicTauCrossSectionTable.tex");


  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Write output root file "<<endl;
	cout<<"#########################"<<endl;
  }
  
  TFile* fout  = new TFile("SemiLeptonicTauXsectionMeas.root","RECREATE");
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
