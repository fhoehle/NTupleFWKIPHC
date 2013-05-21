#include <iomanip>
#include <iostream>
#include "../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../Tools/interface/Dataset.h"
#include "../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../EffEstimation/interface/TMVA_Tree.h"
#include "../Selection/interface/SemiLeptonicTauSelection.h"
#include "../Tools/interface/PUWeighting.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TRandom.h>

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
  SemiLeptonicTauSelection sel; 
  float Luminosity = 0;
  float LumiError = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0;
  int verbosity = -1;

  ///GV to rewieght the MC data////////
  string PUWeightFileName;
 
  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName = string ("../config/TMVA_Tree.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSemiLeptonicTauSelection (sel); // now the parameters for the selection are given to the selection
  //Load for PU:
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  ///Load for PU old version////////////////////////////////
  //sel.GeneratePUWeight(PUWeightFileName);
  //anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  
  TopTree::NTEvent * event = 0;
  
  bool IReweight        = true;
  bool IReweight_Spring11 = false;
 
  std::vector<TH1F> pileup;

 
  PUWeighting  thePUReweighter;
  if (IReweight ) {

    TFile* file1  = new TFile(PUWeightFileName.c_str(),"READ");
    
    TH1D *  hPUData   =0;
    hPUData         = (TH1D*)file1->Get("pileup");

   
    TH1D *  hPUMC   = new TH1D("pileup_MC", "pileup_MC", hPUData->GetXaxis()->GetNbins(), hPUData->GetXaxis()->GetXmin(), hPUData->GetXaxis()->GetXmax() );
    
    //if non spring11 MC
    if(!IReweight_Spring11){ 
     
      TFile* file2  = new TFile( "Crosssection.root" ,"READ");
      hPUMC           = (TH1D*)file2->Get("pileup");
      // histo in data, histo in Mc, use out-of-time pu in the reweighting
     	 
	  thePUReweighter.setPUHisto( hPUData, hPUMC);
	  thePUReweighter.setUseOutOfTimePU(false);
      
    }
    
    else{
	thePUReweighter.setPUHisto( hPUData);
	thePUReweighter.setUseOutOfTimePU(false);
      }
    }
  

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
    
    TMVA_Tree TMVA;
    if(datasets[d].isData() == true) TMVA.CreateHistos("TMVA_bkg.root");
    else                             TMVA.CreateHistos("TMVA_sig.root");
 
    TRandom random;
    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {

      float weight = 1.;
     
      double weightITypeMC_save = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
       
      float trigRange = random.Uniform(1);
    
      if(datasets[d].isData() == false ) 
      {

	if(thePUReweighter.getUseOutOfTimePU()) 
	{
	 if(trigRange<0.3837) 
	 weight =sel.Trigger_QuadJet40_IsoPFTau40()*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);///if you want include the Luminosity weight add: ( weightITypeMC_save*)	 
	 else  
	 weight =sel.Trigger_QuadJet45_IsoPFTau45()*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);///if you want include the Luminosity weight add: ( weightITypeMC_save*)	
	 }
	 
	else
	{ 
	 if(trigRange<0.3837)
	 weight = sel.Trigger_QuadJet40_IsoPFTau40()*thePUReweighter.weight(event->num_pileup_bc0); //weightITypeMC_save*}
         else
	 weight = sel.Trigger_QuadJet45_IsoPFTau45()*thePUReweighter.weight(event->num_pileup_bc0); //weightITypeMC_save*}	 
	 }
	 
	}
      
         
     datasets[d].eventTree ()->GetEntry (ievt);
     if(ievt%100000 == 0) cout << "number of processed events " << ievt << endl;
     
     //Load event for the selection
     sel.LoadEvent(event);
       
     //Collection of selected objects
     //vector<NTMuon> selMuons         = sel.GetSelectedMuons();
     //vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
     vector<NTMuon> selMuons         = sel.GetSelectedMuonsNoIso();
     vector<NTElectron> selElectrons = sel.GetSelectedElectronsNoIso();
     
     vector<NTTau>  selTausLoose     = sel.GetSelectedTaus(selMuons,selElectrons,false,1,0,true);////last false for no discri against electron and muon,  0 for Loose Iso/////
     vector<NTTau>  selTausMedium    = sel.GetSelectedTaus(selMuons,selElectrons,false,1,1,true);
     vector<NTJet>  selJets          = sel.GetSelectedJets(selMuons,selElectrons,selTausLoose);
     NTMET met = sel.GetMET();
     
    
     bool  PassTriggerQuadJet4045IsoPFTauSelection;
     if (datasets[d].isData()==true) 
     {     
       if (   sel.passTriggerQuadJet40IsoPFTauSelection()==true 
           || sel.passTriggerQuadJet45IsoPFTauSelection()==true ) PassTriggerQuadJet4045IsoPFTauSelection = true; 
       else PassTriggerQuadJet4045IsoPFTauSelection = false; 
      }
     else PassTriggerQuadJet4045IsoPFTauSelection = true;
     
     TMVA.FillHistos(selJets, met, selMuons, selElectrons, selTausLoose, selTausMedium, PassTriggerQuadJet4045IsoPFTauSelection, weight );
    }  // end of loop over evts
    
    
    if(datasets[d].isData() == true) TMVA.SaveHistos("TMVA_bkg.root");
    else                             TMVA.SaveHistos("TMVA_sig.root");
  }				
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  return (0);
}
