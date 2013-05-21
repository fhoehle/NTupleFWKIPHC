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
// NEW : for b-tag studies
#include "../../EffEstimation/interface/SFBweight.h"



#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

using namespace TopTree;
using namespace std;

// Copy of DiLeptonCrossSectionMeasurement.C
// + Modification for b-tag weight


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
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  TopTree::NTEvent * event = 0;
  //Selection table
  // 4 tables: ee - emu - mumu - allChannels
  SelectionTable selTable_allChannels(sel.GetCutList(),datasets, string("*"));
  SelectionTable selTable_ee(  sel.GetCutList(),datasets, string("ee")  );
  SelectionTable selTable_emu( sel.GetCutList(),datasets, string("emu") );
  SelectionTable selTable_mumu(sel.GetCutList(),datasets, string("mumu"));


  /// New : Information for b-tag Weight
  ///----
  int syst_b=0;
  /* syst_b
           0   weight
           1   weight + error
           2   weight - error
  */
  int method_b=2;
  /* method_b
           0 = SF
           1 = Eff_DATA
           2 = SF*Eff_MC <----- RECOMMANDED!
  */
  int btag_algo=sel.GetbtagAlgo();
  float btag_discri=sel.GetbtagDiscriCut();
  int btag_nbjet=sel.GetNofBtagJetsCut();
  SFBweight sf_btag(btag_algo,btag_discri,btag_nbjet);
  sf_btag.LoadInfo();
  sf_btag.LoadInfo2();
  ///----



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
    
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
      sel.LoadEvent(event);



     // EXAMPLE : HOW TO CALL THE B-TAG WEIGHT 

     //Candidate pair of lepton
     string CandType; // ee - emu - mumum or false
     vector<NTElectron> candElec;
     vector<NTMuon> candMuon;
     sel.GetLeptonPair(candMuon, candElec, CandType ); // fill the variables
     vector<NTJet>      selJets = sel.GetSelectedJets(candMuon, candElec);

     ///----
     vector<float> weightb;
     weightb.push_back(weight);
     weightb.push_back(0.);
     weightb.push_back(0.);
     weightb.push_back(0.);
     weightb.push_back(0.);

     if (!datasets[d].isData()) {
       // MC
       // COMPUTE THE MC WEIGHT FOR b-tag STUDIES 
       vector<float> aa = sf_btag.GetWeigth4BSel(method_b, syst_b,selJets);
       weightb[0] *= aa[0]; // weight of the event
       weightb[1] = aa[1];  // P(0jet) for method_b>=1
       weightb[2] = aa[2];  // P(1jet) for method_b>=1
       weightb[3] = aa[3];  // P(2jets) for method_b>=1
       weightb[4] = aa[4];  // P(>=3jets) for method_b>=1
       // this weight should be applied only at the b-tag cut level. 

     }
     // else : DATA --> do nothing     
     ///----

    }  // end of loop over evts


  }				// end of loop over the datasets 

 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////


  return (0);
}
