#define ProofSelectorExample_cxx

//////////////////////////////////////////////////////////
//
// Example of TSelector implementation to do a Monte Carlo
// generation using Pythia8.
// See tutorials/proof/runProof.C, option "pythia8", for an
// example of how to run this selector.
//
//////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TFrame.h>
#include <TPaveText.h>
#include <TFormula.h>
#include <TF1.h>
#include <TH1F.h>
#include <TMath.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TParameter.h>
#include "TClonesArray.h"
#include "TParticle.h"
#include "TDatabasePDG.h"

#include "ProofSelectorExample.h"

//_____________________________________________________________________________
ProofSelectorExample::ProofSelectorExample()
{
   // Constructor

   fChain = 0;
   branch = 0;
   event = 0;
   dataset = 0;
   anaEL = 0;
   verbosity = 0;
   DataType = 0;
   Luminosity = 0;
   //histos
   fHist = 0;
}

//_____________________________________________________________________________
ProofSelectorExample::~ProofSelectorExample()
{
   // Destructor

   //SafeDelete(fHist);
}

//_____________________________________________________________________________
void ProofSelectorExample::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses of the tree
   // will be set. It is normaly not necessary to make changes to the
   // generated code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running with PROOF.
 
   //fChain point to the loading tree 
   fChain = tree;

   // Set branch addresses
   branch = (TBranch *) tree->GetBranch("NTEvent");
   event = new TopTree::NTEvent();
   branch->SetAddress(&event);
   //event is now retrieved and could be used in Process
}

//_____________________________________________________________________________
void ProofSelectorExample::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();


}

//_____________________________________________________________________________
void ProofSelectorExample::SlaveBegin(TTree * tree)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   //--------------------------------------//
   //       Loading the xml file
   //--------------------------------------//
   TNamed *dsname = (TNamed *) fInput->FindObject("PROOF_DATASETNAME");
   string datasetName = dsname->GetTitle();
   TNamed *xfname = (TNamed *) fInput->FindObject("PROOF_XMLFILENAME");
   string xmlFileName = xfname->GetTitle();
   //gProof->GetManager()->GetFile(xmlFileName.c_str(),"local.xml");
   //anaEL = new AnalysisEnvironmentLoader("/tmp/local.xml");
   anaEL = new AnalysisEnvironmentLoader(xmlFileName.c_str());
   anaEL->LoadSamples (datasets); // now the list of datasets written in the xml file is known
   //retrieve the current dataset according to its name
   for(unsigned int d=0;d<datasets.size();d++){
	if(datasets[d].Name()==datasetName) dataset = &datasets[d];
   }
   anaEL->LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
   anaEL->LoadGeneralInfo(DataType, Luminosity, verbosity );
   anaEL->LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)


   //--------------------------------------//
   //   Fill cuts and channels  	
   //--------------------------------------//
   CutName.push_back("Cut1");
   ChannelName.push_back("ee");

   //--------------------------------------//
   //   Managing histos  	
   //--------------------------------------//
   MyhistoManager.LoadDatasets(datasets);    
   MyhistoManager.LoadSelectionSteps(CutName);
   MyhistoManager.LoadChannels(ChannelName);
   //example
   fHist = new TH1F("fHist", "jet pt", 50, 0., 200.);

   //--------------------------------------//
   //   Output file 	
   //--------------------------------------//
   //retrieve info from the input:
   TNamed *out = (TNamed *) fInput->FindObject("PROOF_OUTPUTFILE");
   //this file will be THE file which will contains all the histograms
   fProofFile = new TProofOutputFile(out->GetTitle());
   // Open the file
   TDirectory *savedir = gDirectory;
   fFile = fProofFile->OpenFile("UPDATE");
   if (fFile && fFile->IsZombie()) SafeDelete(fFile);
   savedir->cd();


   //this fine is very important !!!
   fFile->Write();
   //It is required to add in fOutput the histos you want to feedback
   fOutput->Add(fHist);
   fOutput->Add(fFile);
}

//_____________________________________________________________________________
Bool_t ProofSelectorExample::Process(Long64_t entry)
{
   //---------------------------------------------------//
   // Main event loop: get entry
   //---------------------------------------------------//
   fChain->GetTree()->GetEntry(entry); 
   branch->GetEntry(entry);
  
   //---------------------------------------------------//
   //         Doing the analysis event by event
   //---------------------------------------------------//

   //cout<<"Entry "<<entry<<endl;
   sel.LoadEvent(event);
   
   //cout<<sel.GetSelectedJets().size()<<endl;
   for(unsigned int i=0;i<sel.GetSelectedJets().size();i++)
	fHist->Fill(sel.GetSelectedJets()[i].p4.Pt());
   
   //Collection of selected objects
   vector<TopTree::NTVertex>   selVertices  = sel.GetSelectedVertex();



   return kTRUE;
   vector<TopTree::NTElectron> selElectrons = sel.GetSelectedElectrons();
   vector<TopTree::NTMuon>     selMuons     = sel.GetSelectedMuons();
   vector<TopTree::NTJet>      selJets      = sel.GetSelectedJets();
   TopTree::NTMET met                       = sel.GetMET(); 
      
   //Candidate pair of lepton
   string CandType; // ee - emu - mumum or false
   vector<TopTree::NTElectron> candElec;
   vector<TopTree::NTMuon> candMuon;
      
   //---------------------------------------------------//
   //           Selection
   //---------------------------------------------------//
   /*
   if(sel.passTriggerSelection ( &(*dataset), ChannelName[0])     ) {
	if(sel.GetSelectedJets().size()>=2){
	}	   
   } 
   */

   return kTRUE;
}

//_____________________________________________________________________________
void ProofSelectorExample::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
 
   if(fProofFile) fProofFile->Print();
   if (fFile) {
      Bool_t cleanup = kFALSE;
      TDirectory *savedir = gDirectory;
      fFile->cd();
      fHist->Write();
      fFile->Write();
      //The following line is mandatory to copy everythin in a common RootFile
      fOutput->Add(fProofFile);
   }
}

//_____________________________________________________________________________
void ProofSelectorExample::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   //Possibility to retrieve information from the merged file and perform some calculation or plotting tasks

   //
   // Create canvas
   //
    //TList* list = fOutput->GetOutputList() ;
    /*
    TIter next_object((TList*) fOutput);
    TObject* obj ;
    cout << "-- Retrieved objects:" << endl ;
    while ((obj = next_object())) { TString objname = obj->GetName() ; cout << " " << objname << endl ; }

   if ((fi = dynamic_cast<TFile *>(fOutput->FindObject("blabla.root")))) {
	cout<<"Warning"<<endl;
	fi->Write("toto.root");
	cout<<"Warning"<<endl;
   }
   // Final update
   c1->cd();
   c1->Update();
   */
}
