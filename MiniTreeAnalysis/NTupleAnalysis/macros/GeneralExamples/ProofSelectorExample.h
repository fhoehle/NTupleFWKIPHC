
//////////////////////////////////////////////////////////
//
// Example of TSelector implementation to do a Monte Carlo
// generation using Pythia8.
// See tutorials/proof/runProof.C, option "pythia8", for an
// example of how to run this selector.
//
//////////////////////////////////////////////////////////

#ifndef ProofSelectorExample_h
#define ProofSelectorExample_h

#include <TSelector.h>
#include <TTree.h>
#include <TFile.h>
#include <TProofOutputFile.h>

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"
#include "../../Plots/interface/HistoManager.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Selection/interface/DiLeptonSelection.h"

class TH1F;
class TBranch;
class TTree;

class AnalysisEnvironmentLoader;
class DiLeptonSelection;

class ProofSelectorExample : public TSelector {
public :

   // Specific members
   //Access to the tree and outputs
   TTree* fChain;
   TBranch* branch;
   TopTree::NTEvent* event;
   TFile            *fFile;
   TProofOutputFile *fProofFile; // For optimized merging of the ntuple
   //Pointer on results from xml file  reading
   AnalysisEnvironmentLoader* anaEL; 
   //Minimimal info
   vector<Dataset> datasets;
   Dataset* dataset;
   vector<string> CutName;
   vector<string> ChannelName;
   DiLeptonSelection sel; 
   float Luminosity;
   int verbosity;
   // 0: MC - 1: Data - 2 Data & MC
   int DataType;
   //Info analysis macro specific 
   HistoManager MyhistoManager;
   TH1F* fHist;

   ProofSelectorExample();
   virtual ~ProofSelectorExample();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual Bool_t  Process(Long64_t entry);
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(ProofSelectorExample,0);
};

#endif
