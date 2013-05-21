#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../../tinyxml/tinyxml.h"

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Selection/interface/DiLeptonChannel.h"

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjString.h"

using namespace TopTree;

struct options
{
  vector<int> PtHatList;
};


vector < TString > parseFileName (TiXmlDocument doc, string name)
{
  vector < TString > inFileName;
  TiXmlHandle hdl (&doc);
  TiXmlNode *node = 0;
  TiXmlElement *elem = 0;
  bool nodeFound = false;
  node = hdl.Node ();
  for (node = node->FirstChild (); node; node = node->NextSibling ()) {
    if (node->Value () == string (name)) {
      nodeFound = true;
      elem = node->FirstChildElement ();
      if (!elem) {
	cerr << "The node doesn't exist" << endl;
	delete node;
	delete elem;
	exit (3);
      }
      while (elem) {
	inFileName.push_back ((TString) elem->Attribute ("file"));
	elem = elem->NextSiblingElement ();	// iteration 
      }
    }
  }

  if (!nodeFound) {
    cerr << "The node doesn't exist" << endl;
    delete node;
    delete elem;
    exit (2);
  }
  return inFileName;
}

options parseOptions (TiXmlDocument doc)
{
  options tempOptions;
  
  AnalysisEnvironmentLoader anaEnv;

  TiXmlHandle hdl (&doc);
  TiXmlNode *node = 0;
  TiXmlElement *elem = 0;
  bool nodeFound = false;
  node = hdl.Node ();
  for (node = node->FirstChild (); node; node = node->NextSibling ()) {
    if (node->Value () == string ("options")) {
      nodeFound = true;
      elem = node->FirstChildElement ();
      if (!elem) {
	cerr << "The node doesn't exist" << endl;
	delete node;
	delete elem;
	exit (3);
      }
      while (elem) {
	
        vector<int> pthatList;
        string spthatList = elem->Attribute("PtHatList"); 
        anaEnv.StringSeparator(spthatList,",",pthatList);
        tempOptions.PtHatList = pthatList;
	elem = elem->NextSiblingElement ();	// iteration 
      }
    }
  }

  if (!nodeFound) {
    cerr << "The node doesn't exist" << endl;
    delete node;
    delete elem;
    exit (2);
  }

  return tempOptions;
}

int main ()
{
  clock_t start = clock ();
  cout<<"=============================="<<endl;
  cout<<"	QCDNTupleMaker         "<<endl;
  cout<<"=============================="<<endl;

  // set verbosity equal to 0 (silent), 1 or 2 (debug)
  unsigned int verbosity = 0;

  // xml file
  char xmlfile[] = "../../config/qcdNorm.xml";

  TiXmlDocument doc (xmlfile);

  if (!doc.LoadFile ()) {
    cerr << "Error while loading the xml file: " << xmlfile << endl;
    cerr << " error #" << doc.ErrorId () << " : " << doc.ErrorDesc () << endl;
    return 1;
  }

  cout<<"- Loading filenames ..."<<endl; 
  vector < TString > inFileName = parseFileName (doc, "inputdatasets");
  if(inFileName.size()==0){
	cerr<<"No input files to run over !!"<<endl;
	return 1;
  }
  vector < TString > outFileName = parseFileName (doc, "outputfilename");
  if(outFileName.size()==0){
	cerr<<"No output filename, information missing"<<endl;
	return 1;
  }
  
  cout<<"- Loading options ..."<<endl; 
  options optionsToUse = parseOptions (doc);

  unsigned int NofOutFiles = outFileName.size();

  cout<<"- Creating files ..."<<endl; 
  TFile **outFiles = new TFile*[NofOutFiles];
  TTree **outEventTree = new TTree*[NofOutFiles];
  TopTree::NTEvent * inEvent = new TopTree::NTEvent ();
  //NTEvent* outEvent = new NTEvent();
  
  for(unsigned int i=0;i<inFileName.size ();i++){
	TString ofilename;
	ofilename = outFileName[i];
	outFiles[i] = new TFile (ofilename, "Recreate");
        outEventTree[i] = new TTree("Event","");
        outEventTree[i]->SetMaxTreeSize (ULONG_MAX);
        outEventTree[i]->Branch ("NTEvent", "TopTree::NTEvent", &inEvent);
  }


  for (unsigned int nFile = 0; nFile < inFileName.size (); nFile++) {
    cout << "input file[" << nFile << "] = " << inFileName[nFile] << endl;

    TFile *inFile = TFile::Open (inFileName[nFile]);

    TTree *inEventTree = (TTree *) inFile->Get ("MyModule/Event");
    TBranch *inEventBranch = (TBranch *) inEventTree->GetBranch ("NTEvent");
    inEventBranch->SetAddress (&inEvent);

    unsigned int nTempEvents = (int) inEventTree->GetEntries ();
    unsigned int nSelEvents = 0;
    
    //loop over events
    for (unsigned int ievt = 0; ievt < nTempEvents; ievt++) {
      if (verbosity > 1)
	cout << ">>> Trying to get event " << ievt << endl;

      inEventTree->GetEvent (ievt);

      //outEvent->Reset ();

      if (verbosity > 1)
	cout << ">>> Analyzing event " << ievt << endl;
      else if ((int) ievt / 10000 == (double) ievt / 10000)
	cout << ">>> Analyzing event " << ievt << endl;
      
      if (inEvent->ptHat < optionsToUse.PtHatList[nFile+1]){
      
      outEventTree[nFile]->Fill();
      nSelEvents++;
      }
   
      
    }// loop over events

    cout << "Efficiency of pthat cut to be used to normalize the xsec : eff = " << (float) nSelEvents / nTempEvents << endl;
    cout << "xsec --> xsec*eff " <<endl;
    if (verbosity > 1)
      cout << "Analyzing input file " << inFileName[nFile] << " finished!" << endl;

    if (inFile)
      inFile->Delete ();

  }// loop over input files


  cout << "Writting trees in outfiles ..." << endl;
  cout << "Produced the following files: "<<endl;
  int SumEvent = 0;
  for(unsigned int i=0;i<NofOutFiles;i++){
	outFiles[i]->cd();
	outFiles[i]->mkdir("MyModule");
	outFiles[i]->cd("MyModule");
  	outEventTree[i]->Write();
	cout<<i+1<<"- "<<outFiles[i]->GetName()<<" #events: "<<outEventTree[i]->GetEntries()<<endl;
	SumEvent+=outEventTree[i]->GetEntries(); 
        outFiles[i]->Close();
	//outFiles[i]->Delete();
	//delete outEventTree[i];
	//delete outFiles[i];
  }
  cout<<"Total: "<<SumEvent<<" events in the output files"<<endl; 


  delete outFiles;
  delete inEvent;


  if (((double) clock () - start) / CLOCKS_PER_SEC < 60)
    cout << "--> The skimming took " << ((double) clock () - start) / CLOCKS_PER_SEC << " seconds." << endl;

  else
    cout << "--> The skimming took " << (((double) clock () - start) / CLOCKS_PER_SEC) / 60 << " minutes." << endl;

  cout << "Code running was succesful!" << endl;

  return (0);
}
