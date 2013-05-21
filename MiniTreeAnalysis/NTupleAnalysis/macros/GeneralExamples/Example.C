#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

using namespace TopTree;
using namespace std;

int main(){
        int verbosity                 = 0;
	//0 muet
	//1 Main Info
	//2
	//3 
	//4 Info for each event
	//5 Debug


	TFile* f = TFile::Open("../../../NTupleProducer/test/NTuple.root");
	TTree* tree = (TTree*) f->Get("MyModule/Event");
        TBranch* branch = (TBranch *) tree->GetBranch("NTEvent");
	TopTree::NTEvent* event = new TopTree::NTEvent();
	branch->SetAddress(&event);
        //cout<<"ClassName: "<<branch->GetClassName()<<endl;
  	
	unsigned int nEvents = (int)tree->GetEntries();
        cout<<"NEvents "<<nEvents<<endl;	
	for(unsigned int ievt=0; ievt<nEvents; ievt++)
	{
		tree->GetEntry(ievt);
		branch->GetEntry(ievt);
		if(verbosity>3) cout <<"event "<< ievt <<endl;
		if(verbosity>3) cout<<"event number="<<event->eventNb<<endl;
		if(verbosity>3) cout<<"run number="<<event->runNb<<endl;
	} // end of loop over evts

	
      if(verbosity>1) cout<<"End of the Macro"<<endl;
     
      return(0);
}
