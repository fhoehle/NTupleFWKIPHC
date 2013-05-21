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
#include "../../BckgdEstimation/interface/FakeRate_LeptEff.h"


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
  string xmlFileName = string ("../../config/EffEstimate.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  TopTree::NTEvent * event = 0;
  //Selection table
  // 4 tables: ee - emu - mumu - allChannels
  SelectionTable selTable_allChannels(sel.GetCutList(),datasets, string("*"));
  SelectionTable selTable_ee(  sel.GetCutList(),datasets, string("ee")  );
  SelectionTable selTable_emu( sel.GetCutList(),datasets, string("emu") );
  SelectionTable selTable_mumu(sel.GetCutList(),datasets, string("mumu"));

  //////////////////////
 
  //////////////////////

  // Matrix Method
  
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
    
   
    
    FakeRate_LeptEff theEffPlots;
    theEffPlots.CreateHistos("muons", datasets[d].Name());
    
    
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
      if (verbosity > 3)
	//cout << "event " << ievt << endl;
      if (verbosity > 3)
	//cout << "event number=" << event->eventNb << endl;
      if (verbosity > 3)
	//cout << "run number=" << event->runNb << endl;
     
     cout << "event number=" << event->eventNb << endl;
     if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
     //Load event for the selection
     sel.LoadEvent(event);

     //Collection of selected objects
     //cout << " get selected electron " << endl;
     vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
     //cout << " get selected muons  " << endl;
     vector<NTMuon>     selMuons = sel.GetSelectedMuons();
     //cout << " get selected jets " << endl;
     
     vector<NTJet>      selJets = sel.GetSelectedJets();
     NTMET met = sel.GetMET(); // no criteria applyied
     
     //fill histo for efficiencies
     
     vector<NTMuon>     selMuonsNonIso = sel.GetSelectedMuonsNoIso();
     theEffPlots.SetLooseMuonCollection( selMuonsNonIso  );
     
     theEffPlots.FillHistos(sel.GetMET(), selJets, "muons", datasets[d].Name());
     
     
     if(verbosity > 2) {
     	//cout<<event->electrons.size()<<" electrons - "<<event->muons.size()<<" muons - "<<endl;
     	//cout<<selElectrons.size()<<" sel electrons - "<<selMuons.size()<<" sel muons - "<<endl;
     }
 
     //Candidate pair of lepton
     string CandType; // ee - emu - mumum or false
     vector<NTElectron> candElec;
     vector<NTMuon> candMuon;
     sel.GetLeptonPair(candMuon, candElec, CandType ); // fill the variables
  
     //if (verbosity > 3) if( CandType!=string("")) cout << "Lepton pair candidate is "<< CandType <<endl;

     //integer which define the last step of the selection that the event fullfill
     int selLastStep = 0;
     /*
	Steps of the selection: (integer returned by doFullSelection() or FillTable(...))
	- Step 1  	Dilepton pair choice
	- Step 2 	Z mass veto 
	- Step 3 	Minimal jet multiplicity 
	- Step 4 	MET cuts
	- Step 5 	btagging cuts
     */
     //boolean which is true if the event pass the selection
     bool isSelEvent = false;	

     
     // cout some values based on objects
     //if(ievt == 10000) break ;
    }  // end of loop over evts
    cout << "173 " << endl;
    TString outputFileName = "effFRHisto_"+datasets[d].Name()+".root";
    cout << "175 " << endl;
    theEffPlots.Savehistos(outputFileName);
    cout << "177 " << endl;
    
    
    
  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 


 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////





  ///////////////


  return (0);
}
