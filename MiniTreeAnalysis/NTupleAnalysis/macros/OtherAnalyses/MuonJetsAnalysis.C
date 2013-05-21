#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/MuonJetsSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"

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
  MuonJetsSelection sel; 
  float Luminosity = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  
  string xmlFileName = string ("../../config/MuonJetsAnalysis.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSelection(sel);
  //anaEL.LoadMuonJetsSelection (sel); // now the parameters for the selection are given to the selection
  //anaEL.LoadDiLeptonSelection (sel); 
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  //anaEL.LoadWeight (sel);
  TopTree::NTEvent * event = 0;
  
  /*string xmlFileName = string ("../config/MyCutFlow.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  TopTree::NTEvent * event = 0;
   */
  //////////////////////
  // The output tree
  //////////////////////
  
  TFile * theOutfileTree = new TFile("outputtree.root", "recreate");
  
  int nmuon;
  double muon_pt[100000] ;
  double muon_eta[100000];
  double muon_phi[100000];
  int    muon_charge[100000];
  
  int nelec;
  double elec_pt[100000] ;
  double elec_eta[100000];
  double elec_phi[100000];
  int    elec_charge[100000];
  
  int njets;
  double jets_pt[100000] ;
  double jets_eta[100000];
  double jets_phi[100000];
  int    jets_flavor[100000];
  int    jets_BTagDiscr[100000];
  
  
  double missET;
  
  int tmeme;
  
  TTree * smalltree = new TTree("Tree","Tree");
  
  smalltree->Branch("nmuon",&nmuon,"nmuon/I");
  smalltree->Branch("muon_pt",  muon_pt ,  "muon_pt[nmuon]/D" );
  smalltree->Branch("muon_eta", muon_eta , "muon_eta[nmuon]/D");
  smalltree->Branch("muon_phi", muon_phi , "muon_phi[nmuon]/D");
  smalltree->Branch("muon_charge", muon_charge , "muon_charge[nmuon]/I");
  
  smalltree->Branch("nelec",&nelec,"nelec/I");
  smalltree->Branch("elec_pt",  elec_pt ,  "elec_pt[nelec]/D" );
  smalltree->Branch("elec_eta", elec_eta , "elec_eta[nelec]/D");
  smalltree->Branch("elec_phi", elec_phi , "elec_phi[nelec]/D");
  smalltree->Branch("elec_charge", elec_charge , "elec_charge[nelec]/I");
  
  smalltree->Branch("njets",&njets,"njets/I");
  smalltree->Branch("jets_pt",  jets_pt ,  "jets_pt[njets]/D" );
  smalltree->Branch("jets_eta", jets_eta , "jets_eta[njets]/D");
  smalltree->Branch("jets_phi", jets_phi , "jets_phi[njets]/D");
  smalltree->Branch("jets_flavor", jets_flavor, "jets_flavor[njets]/I");
  smalltree->Branch("jets_BTagDiscr", jets_BTagDiscr, "jets_BTagDiscr[njets]/I");
  smalltree->Branch("missET",&missET,"missET/D");
  smalltree->Branch("tmeme",&tmeme,"tmeme/I");
  
  
  
  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Loop over the datasets  "<<endl;
	cout<<"#########################"<<endl;
  }
 
  for (unsigned int d = 0; d < datasets.size (); d++) {

    
    //TBranch *evtBR = (TBranch *) (datasets[d].eventTree ()->GetBranch ("NTEvent"));
    //evtBR->SetAddress (&event);
    
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    
    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    
   
    /*datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;*/
   
  
    //LOOP OVER THE EVENTS
    
    int cut0 = 0;
    int cut1 = 0;
    int cut2 = 0;
    int cut3 = 0;
    int cut4 = 0;
    int cut5 = 0;
    int cut6a = 0;
    int cut6b = 0;
    int cut6c = 0;
    int cut7  = 0;
    
    
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      float weight = 1.;
      
      datasets[d].eventTree ()->GetEntry (ievt);
      //datasets[d].eventTree ()->Print();
      //evtBR->GetEntry (ievt);
      
            
     //cout << "event number=" << event->eventNb << endl;
     //Load event for the selection
     sel.LoadEvent(event);
     
     if(ievt%10000 == 0) cout << "the number of processed event is " << ievt << endl; 
     
      

     //Collection of selected objects
     //cout << " get selected electron " << endl;
      
     vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
     
     //cout << " get selected muons  " << endl;
      
     vector<NTMuon>     selMuons = sel.GetSelectedMuons();
      
     //cout << " get selected jets " << endl;
      
     vector<NTJet>      selJets = sel.GetSelectedJets();
      
     NTMET met = sel.GetMET(); // no criteria applyied
     
     nmuon = 0;
     for(unsigned int imuon=0; imuon < selMuons.size(); imuon++){
       muon_pt[nmuon]  = selMuons[imuon].p4.Pt();
       muon_eta[nmuon] = selMuons[imuon].p4.Eta();
       muon_phi[nmuon] = selMuons[imuon].p4.Phi() ;
       muon_charge[nmuon] = selMuons[imuon].Charge;
       nmuon++;
     }
      
     nelec = 0;
     for(unsigned int ielec=0; ielec < selElectrons.size(); ielec++){
       elec_pt[nelec]  = selElectrons[ielec].p4.Pt();
       elec_eta[nelec] = selElectrons[ielec].p4.Eta();
       elec_phi[nelec] = selElectrons[ielec].p4.Phi() ;
       elec_charge[nelec] = selElectrons[ielec].Charge;
       nelec++;
     }
     
     njets = 0;
     for(unsigned int ijets=0; ijets < selJets.size(); ijets++){
       jets_pt[njets]  = selJets[ijets].p4.Pt();
       jets_eta[njets] = selJets[ijets].p4.Eta();
       jets_phi[njets] = selJets[ijets].p4.Phi() ;
       jets_flavor[njets] = selJets[ijets].partonFlavour;
       jets_BTagDiscr[njets] = selJets[ijets].SVDiscri;
       njets++;
     }
 
     missET= met.p4.Et();
     tmeme = event->TMEME;
     
     
     
     smalltree->Fill();
     
     
     

      
     if(verbosity > 2) {
     	//cout<<event->electrons.size()<<" electrons - "<<event->muons.size()<<" muons - "<<endl;
     	//cout<<selElectrons.size()<<" sel electrons - "<<selMuons.size()<<" sel muons - "<<endl;
     }
     
     //integer which define the last step of the selection that the event fullfill
     int selLastStep = 0;
     //boolean which is true if the event pass the selection
     bool isSelEvent = false;	
     string theChannel = "TTbarMuJets" ;
    
     selLastStep = sel.doFullSelection( &datasets[d], theChannel);
      //cout << "selLastStep " <<selLastStep << endl;
     if(selLastStep == 5) isSelEvent = true;
     if(selLastStep >=0 ) cut0++;
     if(selLastStep >=2 ) cut1++;
     if(selLastStep >=3 ) cut2++;
     if(selLastStep >=4 ) cut3++;
     if(selLastStep >=5 ) cut4++;
     if(selLastStep >=6 ) cut5++;
     if(selLastStep >=7 ) cut6a++;
     if(selLastStep >=8 ) cut6b++;
     if(selLastStep >=9 ) cut6c++;
     if(selLastStep >=10 ) cut7++;
      


    }				// end of loop over evts
  cout << " nbr of events after cut 0 is " << cut0 << endl;
  cout << " nbr of events after cut 1 is " << cut1 << endl;
  cout << " nbr of events after cut 2 is " << cut2 << endl;
  cout << " nbr of events after cut 3 is " << cut3 << endl;
  cout << " nbr of events after cut 4 is " << cut4 << endl;
  cout << " nbr of events after cut 5 is " << cut5 << endl;
  cout << " nbr of events after cut 6a is " << cut6a << endl;
  cout << " nbr of events after cut 6b is " << cut6b << endl;
  cout << " nbr of events after cut 6c is " << cut6c << endl;
  cout << " nbr of events after cut 7 is  " << cut7  << endl;
   
  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  smalltree->Write();

 
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

 

  return (0);
}
