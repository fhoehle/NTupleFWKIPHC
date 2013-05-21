
#include <iostream>
#include <limits.h>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"
#include "../../Measurements/interface/PLRPlotsFurnisher.h"
#include "../../Tools/interface/PUWeighting.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>


///////////////////////////////////////////////////////////////////////////////////
//
// Utilisation: tourne sur les 3 types de channel etudies (ee/mumu/emu)
// Modifier le fichier de config ../../config/MyCutFlow.xml  (pour emu mettre METCutsEMU a 0 ou 20)
//
///////////////////////////////////////////////////////////////////////////////////



using namespace TopTree;
using namespace std;

int main (int argc, char* argv[])
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
  float LumiError = 0;
  string PUWeightFileName;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;
  bool producePLRPlots = false;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  if(argc!=3){
	 cerr<<"usage: ./MyCutFlow.C file.xml file.root"<<endl;
	 //return(0);
  }
  string xmlFileName;
  if(argc!=3) xmlFileName = string ("../../config/MyCutFlow.xml");
  else xmlFileName = argv[1];
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  
  //Load for PU:
  sel.GeneratePUWeight(PUWeightFileName);
  
  //******************************************
  //Load Scale Factors for lepton efficiencies
  //******************************************
  sel.LoadElScaleFactors();
  sel.LoadMuScaleFactors();
  
  
  //Load Btag config:
   cout<<"Test2"<<endl;
   cout<<sel.GetSFBweight().Test()<<endl; 
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  cout<<"Test1"<<endl;
  //cout<<sel.GetSFBweight()<<endl;
  cout<<sel.GetSFBweight().GetHistoEffMCb()<<endl;
  cout<<sel.GetSFBweight().GetHistoEffMCb()->GetEntries()<<endl;

   cout<<"Test2"<<endl;
   cout<<sel.GetSFBweight().Test()<<endl; 

 
  TopTree::NTEvent * event = 0;

  //--------------------------------
  //  Skimmer  
  //--------------------------------
  TTree* outEventTree = new TTree ("Event", "Event tree");
  outEventTree->Branch ("NTEvent", "TopTree::NTEvent", &event);

  string ofilename;
  if(argc==3){
	 ofilename = argv[2];
  }
  else ofilename = string("CrossSection")+string(".root");
  TFile* fout  = new TFile(ofilename.c_str(),"RECREATE");
  fout->mkdir("MyModule");
   

  vector<string> VecChannelName;
  VecChannelName.push_back("mumu");
  VecChannelName.push_back("ee");
  VecChannelName.push_back("emu");

  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Loop over the datasets  "<<endl;
    cout<<"#########################"<<endl;
  }
  


  for (unsigned int d = 0; d < datasets.size (); d++) {

    fout->cd();
    fout->cd("MyModule");
    if(datasets[d].theNormHisto()) datasets[d].theNormHisto()->Write();     
    
    
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    //unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    unsigned int nEvents = (int) (datasets[d].NofEvtsToRunOver ());
    cout << "NEvents in " << datasets[d].Name() << " = " << nEvents << endl;
    
    //////////////////////
    //LOOP OVER THE EVENTS
    //////////////////////
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      
      float weight = 1.;
      datasets[d].eventTree ()->GetEntry (ievt);
      
      if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;
       
      sel.LoadEvent(event);
      
  //------------------------------
  // Skimmer
  // Apply the selection
  //------------------------------
  bool selected = false;
  for(unsigned int i=0;i<VecChannelName.size();i++){
  	//if(sel.doFullSelection(&datasets[d],VecChannelName[i])>= 6 ) selected = true;
  	sel.doFullSelection(&datasets[d],VecChannelName[i]);
	int code = sel.GetSelCode();
	int nleptons = 0;
	nleptons+=sel.GetSelectedElectrons().size();
	nleptons+=sel.GetSelectedMuons().size();
	if( (code & 0x1 ) && (code & 0x8) && nleptons>=2) selected = true;
  }
  if(selected) outEventTree->Fill();
 
      
    }  // end of loop over evts

    cout << " LOOP OVER EVENTS IN " << datasets[d].Name() << " IS FINISHED :) " << endl;
    
    
  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
  
  
 
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Write output root file "<<endl;
    cout<<"#########################"<<endl;
  }

  fout->cd();
  fout->cd("MyModule");
  outEventTree->Write();

  fout->Write();
  fout->Close();
  

  outEventTree = 0;  
  delete fout;
  
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<"    End of the program   "<<endl;
    cout<<"#########################"<<endl;
  }
  
  return (0);
}
