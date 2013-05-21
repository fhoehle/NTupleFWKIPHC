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
  bool useDiLepChannel;
  vector<string> DiLepChannelList;
  bool useTMEME;
  vector<pair<vector<int>,string> >TMEMEList;
};


vector < TString > parseFileName (TiXmlDocument doc, string name)
{
  AnalysisEnvironmentLoader anaEl;
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
	//inFileName.push_back ((TString) elem->Attribute ("file"));
	string filename = elem->Attribute ("file");
	vector<string> filenames;
	anaEl.StringSeparator(filename,string(","),filenames);
	for(unsigned int i=0;i<filenames.size();i++) inFileName.push_back((TString) filenames[i]);
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
  //initialisation
  tempOptions.useDiLepChannel = false;
  tempOptions.useTMEME = false;
  
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
	int useDiLepChannel = -1;
	elem->QueryIntAttribute ("useDiLepChannel", &useDiLepChannel);
        if(useDiLepChannel==1){
		 tempOptions.useDiLepChannel = true;
        	string channels = elem->Attribute("ChannelList"); 
        	anaEnv.StringSeparator(channels,",",tempOptions.DiLepChannelList);
	}

	int useTMEME = -1;
	elem->QueryIntAttribute ("useTMEME", &useTMEME);
        if(useTMEME==1){
		vector<int> tmemeList;
		tempOptions.useTMEME = true;
        	string stmeme = elem->Attribute("TMEME"); 
        	anaEnv.StringSeparator(stmeme,",",tmemeList);
		string name = elem->Attribute("Name");
		pair<vector<int>, string > mypair(tmemeList,name);
		tempOptions.TMEMEList.push_back(mypair);
	}
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

int main (int argc, char *argv[])
{
  clock_t start = clock ();
  cout<<"=============================="<<endl;
  cout<<"	NTupleSplitter         "<<endl;
  cout<<"=============================="<<endl;

  // set verbosity equal to 0 (silent), 1 or 2 (debug)
  unsigned int verbosity = 0;

  // xml file
  char xmlfile[] = "../../config/split.xml";
  //char* xmlfile = argv[1];
  cout<<"The xml file is: " <<xmlfile<<endl;

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
  if(outFileName.size()>1) cerr<<"Too much outfilename: "<<outFileName[0]<<" will be used"<<endl;
  
  cout<<"- Loading options ..."<<endl; 
  options optionsToUse = parseOptions (doc);

  unsigned int NofOutFiles = optionsToUse.DiLepChannelList.size()+optionsToUse.TMEMEList.size();
  if(optionsToUse.useDiLepChannel) NofOutFiles++;
  if(optionsToUse.useTMEME) NofOutFiles++;

  cout<<"- Creating files ..."<<endl; 
  TFile **outFiles = new TFile*[NofOutFiles];
  TTree **outEventTree = new TTree*[NofOutFiles];
  TH1F *theNormHisto = 0;
  TopTree::NTEvent * inEvent = new TopTree::NTEvent ();
  //NTEvent* outEvent = new NTEvent();
  for(unsigned int i=0;i<optionsToUse.DiLepChannelList.size();i++){
	TString ofilename;
	ofilename = outFileName[0]+"_"+optionsToUse.DiLepChannelList[i]+".root";
	outFiles[i] = new TFile (ofilename, "Recreate");
        outEventTree[i] = new TTree("Event","");
        outEventTree[i]->SetMaxTreeSize (ULONG_MAX);
        outEventTree[i]->Branch ("NTEvent", "TopTree::NTEvent", &inEvent);
  }
  if(optionsToUse.useDiLepChannel){
	TString ofilename;
	ofilename = outFileName[0]+"_DiLep_other"+".root";
	outFiles[optionsToUse.DiLepChannelList.size()] = new TFile (ofilename, "Recreate");
        outEventTree[optionsToUse.DiLepChannelList.size()] = new TTree("Event","");
        outEventTree[optionsToUse.DiLepChannelList.size()]->SetMaxTreeSize (ULONG_MAX);
        outEventTree[optionsToUse.DiLepChannelList.size()]->Branch ("NTEvent", "TopTree::NTEvent", &inEvent);
  }
  for(unsigned int i=0;i<optionsToUse.TMEMEList.size();i++){
  	TString ofilename;
   	int iter = i + optionsToUse.DiLepChannelList.size();
        if(optionsToUse.useDiLepChannel) iter++;
	ofilename = outFileName[0]+"_TMEME_"+optionsToUse.TMEMEList[i].second+".root";
	outFiles[iter] = new TFile (ofilename, "Recreate");
        outEventTree[iter] = new TTree("Event","");
        outEventTree[iter]->SetMaxTreeSize (ULONG_MAX);
        outEventTree[iter]->Branch ("NTEvent", "TopTree::NTEvent", &inEvent);
  }
  if(optionsToUse.useTMEME){
  	TString ofilename;
   	int iter = optionsToUse.TMEMEList.size() + optionsToUse.DiLepChannelList.size();
        if(optionsToUse.useDiLepChannel) iter++;
	ofilename = outFileName[0]+"_TMEME_other"+".root";
	outFiles[iter] = new TFile (ofilename, "Recreate");
        outEventTree[iter] = new TTree("Event","");
        outEventTree[iter]->SetMaxTreeSize (ULONG_MAX);
        outEventTree[iter]->Branch ("NTEvent", "TopTree::NTEvent", &inEvent);
  }

  int nInEvents = 0;
 
  cout<<"It will run on "<<inFileName.size()<<" files !"<<endl;

  for (unsigned int nFile = 0; nFile < inFileName.size (); nFile++) {
    cout << "input file[" << nFile << "] = " << inFileName[nFile] << endl;

    TFile *inFile = TFile::Open (inFileName[nFile]);
    
    if(nFile == 0) theNormHisto = (TH1F*) ((TH1F*) inFile->Get("MyModule/theNormHisto"))->Clone();
    else theNormHisto->Add((TH1F*) inFile->Get("MyModule/theNormHisto")->Clone());
    cout<<theNormHisto<<endl;
    cout<<theNormHisto->GetName()<<endl;
    theNormHisto->SetDirectory(0);

    TTree *inEventTree = (TTree *) inFile->Get ("MyModule/Event");
    TBranch *inEventBranch = (TBranch *) inEventTree->GetBranch ("NTEvent");
    inEventBranch->SetAddress (&inEvent);

    unsigned int nTempEvents = (int) inEventTree->GetEntries ();
    nInEvents += nTempEvents;

    DiLeptonChannel dilepChannel;

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

      bool keepEvent = true;

      dilepChannel.LoadEvent(inEvent);
      
      if(optionsToUse.useDiLepChannel){
        bool ChannelFound = false;
      	for(unsigned int i=0;i<optionsToUse.DiLepChannelList.size();i++){
		if(dilepChannel.isSignal(optionsToUse.DiLepChannelList[i])){
			outEventTree[i]->Fill();
			ChannelFound =true;
		}
      	}
	if(!ChannelFound) outEventTree[optionsToUse.DiLepChannelList.size()]->Fill();
      }
      if(optionsToUse.useTMEME){
        bool passTMEMECriteria = false;
      	for(unsigned int i=0;i<optionsToUse.TMEMEList.size();i++){
		bool doFill = false;
  		for(unsigned int j=0;j<optionsToUse.TMEMEList[i].first.size();j++){
			if(inEvent->TMEME == optionsToUse.TMEMEList[i].first[j]){
				passTMEMECriteria = true;
				doFill = true;
				break;
			} 
		}
		if(doFill){
    	    		if(!optionsToUse.useDiLepChannel) outEventTree[i+optionsToUse.DiLepChannelList.size()]->Fill();
      	 		else outEventTree[i+optionsToUse.DiLepChannelList.size()+1]->Fill();
		}
	}	
	if(!passTMEMECriteria){
	 if(!optionsToUse.useDiLepChannel) outEventTree[optionsToUse.TMEMEList.size()+optionsToUse.DiLepChannelList.size()]->Fill();
	 else outEventTree[optionsToUse.TMEMEList.size()+optionsToUse.DiLepChannelList.size()+1]->Fill();
        }
      }
    }				// loop over events

    if (verbosity > 1)
      cout << "Analyzing input file " << inFileName[nFile] << " finished!" << endl;

    if (inFile)
      inFile->Delete ();

  }				// loop over input files


  cout << "Writting trees in outfiles ..." << endl;
  cout << "--> Ran over "<< nInEvents << " events" << endl;
  cout << "Produced the following files: "<<endl;
  int SumEvent = 0;
  for(unsigned int i=0;i<NofOutFiles;i++){
	outFiles[i]->cd();
	outFiles[i]->mkdir("MyModule");
	outFiles[i]->cd("MyModule");
  	outEventTree[i]->Write();
  	cout<<"here"<<endl;
    cout<<theNormHisto<<endl;
    cout<<theNormHisto->GetName()<<endl;
        theNormHisto->Write();
  	cout<<"here"<<endl;
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

  cout << "Code running was succesfull!" << endl;

  return (0);
}
