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
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjString.h"

using namespace TopTree;

struct keepObjects
{
  string name;
  string type;
  float minPt;
  float maxEta;
  bool skipObjects;
  int minNObjects;
};

struct options
{
  bool skimOnHLT;
  string HLTPath1;
  string HLTPath2;
  bool HLTApplyAnd;
  bool useJSON;
  string JSONFile;
};

vector < keepObjects > parseObjects (TiXmlDocument doc, TTree * outEventTree)
{
  vector < keepObjects > toKeep;
  TiXmlHandle hdl (&doc);
  TiXmlNode *node = 0;
  TiXmlElement *elem = 0;
  bool nodeFound = false;
  node = hdl.Node ();
  for (node = node->FirstChild (); node; node = node->NextSibling ()) {
    if (node->Value () == string ("keepbranches")) {
      nodeFound = true;
      elem = node->FirstChildElement ();
      if (!elem) {
	cerr << "The node doesn't exist" << endl;
	delete node;
	delete elem;
      }
      while (elem) {
	keepObjects tempObj;
	tempObj.name = (TString) elem->Attribute ("name");
	tempObj.type = (TString) elem->Attribute ("type");

	elem->QueryFloatAttribute ("minPt", &(tempObj.minPt));
	elem->QueryFloatAttribute ("maxEta", &(tempObj.maxEta));

	int skipObj = 0, minNobj = 0;
	elem->QueryIntAttribute ("minNObjects", &(minNobj));
	tempObj.minNObjects = minNobj;
	elem->QueryIntAttribute ("skipObjects", &(skipObj));
	if (skipObj == 0)
	  tempObj.skipObjects = false;
	else if (skipObj == 1)
	  tempObj.skipObjects = true;
	else
	  cerr << "Wrong skipObjects : " << skipObj << " for " << tempObj.name << endl;

	cout << "The skim will keep " << tempObj.name << ", of type " << tempObj.type << endl;
	cout << "With minPt = " << tempObj.minPt << " and maxEta = " << tempObj.maxEta << endl;
	if (tempObj.skipObjects)
	  cout << "With skipObjects = true " << endl;
	else
	  cout << "With skipObjects = false " << endl;

	toKeep.push_back (tempObj);

	elem = elem->NextSiblingElement ();	// iteration 
      }
    }
  }

  if (!nodeFound) {
    cerr << "The node doesn't exist" << endl;
    delete node;
    delete elem;
  }

  return toKeep;
}

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
	int skimOnHLT = 0;
	elem->QueryIntAttribute ("skimOnHLT", &skimOnHLT);
	if (skimOnHLT == 1) {
	  tempOptions.skimOnHLT = true;
	  tempOptions.HLTPath1 = elem->Attribute ("HLTPath1");
	  tempOptions.HLTPath2 = elem->Attribute ("HLTPath2");
	  int HLTApplyAnd = 0;
	  elem->QueryIntAttribute ("HLTApplyAnd", &HLTApplyAnd);
	  if (HLTApplyAnd == 1)
	    tempOptions.HLTApplyAnd = true;
	  else
	    tempOptions.HLTApplyAnd = false;
	}
	else
	  tempOptions.skimOnHLT = false;

	int useJSON = 0;
	elem->QueryIntAttribute ("useJSON", &useJSON);
	if (useJSON == 1) {
	  tempOptions.useJSON = true;
	  tempOptions.JSONFile = elem->Attribute ("JSONFile");
	}
	else
	  tempOptions.useJSON = false;

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

  // set verbosity equal to 0 (silent), 1 or 2 (debug)
  unsigned int verbosity = 0;


//modifdg
//   vector < Dataset > datasets;
  DiLeptonSelection sel; 
  float Luminosity = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int Verbosity = 0;

  string  ChannelName = "ee";
  string xmlFileName = string ("../../config/MyCutFlow.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
//  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, Verbosity );
//endmodifdg


  // xml file
  char xmlfile[] = "../../config/MyEMUDATAskim.xml";

  TiXmlDocument doc (xmlfile);

  if (!doc.LoadFile ()) {
    cerr << "Error while loading the xml file: " << xmlfile << endl;
    cerr << " error #" << doc.ErrorId () << " : " << doc.ErrorDesc () << endl;
    return 1;
  }

  vector < TString > inFileName = parseFileName (doc, "inputdatasets");

  vector < TString > outFileName = parseFileName (doc, "outputfilename");

  options optionsToUse = parseOptions (doc);
  cout << "options:" << endl;
  cout << "HLTPath1 = " << optionsToUse.HLTPath1 << endl;
  cout << "HLTPath2 = " << optionsToUse.HLTPath2 << endl;
  cout << "HLTApplyAnd = " << optionsToUse.HLTApplyAnd << endl;
  cout << "JSONFile = " << optionsToUse.JSONFile << endl;

  cout << "output file: " << outFileName[0] << endl;

  TFile *outFile = new TFile (outFileName[0], "Recreate");
  outFile->cd ();

  TopTree::NTEvent * outEvent = new TopTree::NTEvent ();
  TTree *outEventTree = new TTree ("Event", "Event tree");
  outEventTree->SetMaxTreeSize (ULONG_MAX);
  outEventTree->Branch ("NTEvent", "TopTree::NTEvent", &outEvent);

  cout << "Parsing objectsToKeep from xml file..." << endl;

  //      parse input objects from xml
  vector < keepObjects > objectsToKeep = parseObjects (doc, outEventTree);

  if (verbosity > 0)
    cout << "objectsToKeep.size() = " << objectsToKeep.size () << endl;

  // Prepare userInfo to be added to the outEventTree
  string info = "This TopTree was skimmed with the following properties:\n";

  for (unsigned int j = 0; j != objectsToKeep.size (); j++) {
    stringstream tmpMinPt, tmpMaxEta, tmpMinNObjects;
    tmpMinPt << objectsToKeep[j].minPt;
    tmpMaxEta << objectsToKeep[j].maxEta;
    tmpMinNObjects << objectsToKeep[j].minNObjects;

    info += "name = " + objectsToKeep[j].name + "  type = " + objectsToKeep[j].type;
    info += "  minPt = " + tmpMinPt.str () + "  maxEta = " + tmpMaxEta.str ();
    if (objectsToKeep[j].skipObjects)
      info += "  skipObjects = 1";
    else
      info += "  skipObjects = 0";
    info += "  minNObjects = " + tmpMinNObjects.str () + "\n";
  }

  // Add UserInfo to outEventTree
  TObjString *userInfo = new TObjString ();
  userInfo->SetString (info.c_str ());

  cout << "userInfo that will be added to the outEventTree:\n" << userInfo->GetString () << endl;

  outEventTree->GetUserInfo ()->Add (userInfo);

  vector < vector < int > >runLumiInfo;	// To store the info from the JSON file

  if (optionsToUse.useJSON) {
    if (verbosity > 3)
      cout << "Reading in JSON file " << endl;

    string inputJSON;

    ifstream myfile ((optionsToUse.JSONFile).c_str ());
    if (myfile.is_open ()) {
      getline (myfile, inputJSON);	// Only the first line is needed
      myfile.close ();
    }

    vector < string > splittedInputJSON;
    size_t begin = 2, end = 2;

    while (end < inputJSON.size ()) {
      end = inputJSON.find ("]], \"", begin);
      string splitted = inputJSON.substr (begin, end - begin + 1);
      begin = end + 5;

      size_t tempEnd = splitted.find ("\": [[", 0);
      string runNr = splitted.substr (0, tempEnd);
      stringstream ss (runNr);
      int runNumber = 0;
      ss >> runNumber;

      string remain = splitted.substr (tempEnd + 4, splitted.size () - (tempEnd + 3));
      size_t tempEnd2 = remain.find ("]", 0);
      size_t tempBegin2 = 0;

      while (tempEnd2 < remain.size ()) {
	string lumiInfo = remain.substr (tempBegin2 + 1, tempEnd2 - tempBegin2 - 1);
	tempBegin2 = tempEnd2 + 3;
	tempEnd2 = remain.find ("]", tempBegin2);

	// parse lumiInfo string
	size_t tempBegin3 = lumiInfo.find (", ", 0);
	string minLS = lumiInfo.substr (0, tempBegin3);
	string maxLS = lumiInfo.substr (tempBegin3 + 2, lumiInfo.size ());
	int minLumiSection = 0;
	int maxLumiSection = 0;
	stringstream ssMin (minLS);
	stringstream ssMax (maxLS);
	ssMin >> minLumiSection;
	ssMax >> maxLumiSection;

	vector < int >tempInfo;
	tempInfo.push_back (runNumber);
	tempInfo.push_back (minLumiSection);
	tempInfo.push_back (maxLumiSection);
	runLumiInfo.push_back (tempInfo);
      }
    }
  }

  unsigned int nOutEvents = 0, nInEvents = 0, NHLTAccept = 0, NHLT8E29Accept = 0;
  vector < unsigned int >hltAccept, hlt8E29Accept;

// modifdg
  vector < TString > MyinFileName ;
  for (unsigned int nFile = 0; nFile < inFileName.size (); nFile++) {
     string crotte = (string) inFileName[nFile];
     int pos = (crotte).find('*');
     bool star = false;
     if(pos<(int)(crotte).size() && pos>=0){
  	string command;
	command = "ls -ltrh " + crotte + "| awk {'print $9'} > tmp.dum";
	system(command.c_str());
	ifstream ifile("tmp.dum");
	while(!ifile.eof()){
		string sread;
		ifile>>sread;
		MyinFileName.push_back(sread);
	}
	MyinFileName.erase(MyinFileName.end()-1,MyinFileName.end());
	system("rm tmp.dum");
	star = true;
   }
   if ( !star) 	MyinFileName.push_back(inFileName[0]);
  
  }
//endmodifdg

//modifdg
//   for (unsigned int nFile = 0; nFile < inFileName.size (); nFile++) {
//     cout << "input file[" << nFile << "] = " << inFileName[nFile] << endl;
// 
//     TFile *inFile = TFile::Open (inFileName[nFile]);
// 
  for (unsigned int nFile = 0; nFile < MyinFileName.size (); nFile++) {
    cout << "input file[" << nFile << "] = " << MyinFileName[nFile] << endl;

    TFile *inFile = TFile::Open (MyinFileName[nFile]);

//endmodifdg
    TTree *inEventTree = (TTree *) inFile->Get ("MyModule/Event");
    TBranch *inEventBranch = (TBranch *) inEventTree->GetBranch ("NTEvent");
    TopTree::NTEvent * inEvent = new TopTree::NTEvent ();
    inEventBranch->SetAddress (&inEvent);

    unsigned int nTempEvents = (int) inEventTree->GetEntries ();
    nInEvents += nTempEvents;

    //loop over events
    for (unsigned int ievt = 0; ievt < nTempEvents; ievt++) {
      if (verbosity > 1)
	cout << ">>> Trying to get event " << ievt << endl;

      inEventTree->GetEvent (ievt);

      outEvent->Reset ();

      if (verbosity > 1)
	cout << ">>> Analyzing event " << ievt << endl;
      else if ((int) ievt / 10000 == (double) ievt / 10000)
	cout << ">>> Analyzing event " << ievt << endl;

      bool keepEvent = true;

      // apply JSON
      if (optionsToUse.useJSON) {
	bool goodEvent = false;
	for (unsigned int k = 0; k < runLumiInfo.size (); k++) {
	  if (inEvent->runNb == runLumiInfo[k][0] && inEvent->lumiblock >= runLumiInfo[k][1] && inEvent->lumiblock <= runLumiInfo[k][2])
	    goodEvent = true;
	}
	if (goodEvent == false)
	  keepEvent = false;
      }

      if (!keepEvent)
	continue;

      // apply selection based on HLT trigger
      if (optionsToUse.skimOnHLT) {
	if (optionsToUse.HLTPath2 != "") {
	  if(!inEvent->TriggerStored(optionsToUse.HLTPath1) ){
		cerr<<"Trigger "<<optionsToUse.HLTPath1<<" not stored"<<endl;
	  	return 2;
	  }
	  if(!inEvent->TriggerStored(optionsToUse.HLTPath2) ){
		cerr<<"Trigger "<<optionsToUse.HLTPath2<<" not stored"<<endl;
	  	return 2;
	  }
	  bool passed1 = inEvent->TriggerPassed (optionsToUse.HLTPath1);
	  bool passed2 = inEvent->TriggerPassed (optionsToUse.HLTPath2);
	  if (keepEvent && optionsToUse.HLTApplyAnd)
	    keepEvent = passed1 && passed2;
	  else if (keepEvent)
	    keepEvent = passed1 || passed2;
	}
	else{
	  if(!inEvent->TriggerStored(optionsToUse.HLTPath1) ){
		cerr<<"Trigger "<<optionsToUse.HLTPath1<<" not stored"<<endl;
	  	return 2;
	  }
	  keepEvent = inEvent->TriggerPassed (optionsToUse.HLTPath1);
      	}
      }

//modifdg
  keepEvent = false;
     //Load event for the selection
     sel.LoadEvent(inEvent);


// skimming trigger  
     //Collection of selected objects
     vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
     vector<NTElectron> selElectrons = sel.GetSelectedElectrons();
     vector<NTMuon>     selMuons     = sel.GetSelectedMuons();
     vector<NTJet>      selJets      = sel.GetSelectedJets();
     NTMET met                       = sel.GetMET(); 
 
     //Candidate pair of lepton
     string CandType; // ee - emu - mumum or false
     vector<NTElectron> candElec;
     vector<NTMuon> candMuon;

     bool TriggerPassed_mumu_MC = (inEvent->TriggerPassed("HLT_Mu9"));
     bool TriggerPassed_ee_MC   = (inEvent->TriggerPassed("HLT_Ele10_LW_L1R") && (sel.eleHLTMatch>15) );
     bool TriggerPassed_mumu_DATA = false;
     bool TriggerPassed_ee_DATA   = false;
     if ( ( inEvent->runNb<138000 && inEvent->TriggerPassed("HLT_Ele10_LW_L1R") && (sel.eleHLTMatch>15) ) ||
          ( inEvent->runNb>=141900 && inEvent->runNb<144000 && inEvent->TriggerPassed("HLT_Ele15_SW_L1R") ) ||
	  ( inEvent->runNb>144000 && inEvent->runNb<=144114 && (inEvent->TriggerPassed("HLT_Ele15_SW_CaloEleId_L1R")||inEvent->TriggerPassed("HLT_Ele20_SW_L1R")||inEvent->TriggerPassed("HLT_DoubleEle10_SW_L1R") ) ) ||
	  ( inEvent->runNb>146000 && inEvent->runNb<147120 && (inEvent->TriggerPassed("HLT_DoubleEle10_SW_L1R")||inEvent->TriggerPassed("HLT_Ele17_SW_CaloEleId_L1R") ) ) ||
	  ( inEvent->runNb>147120 && inEvent->runNb<148100 && (inEvent->TriggerPassed("HLT_DoubleEle15_SW_L1R_v1")||inEvent->TriggerPassed("HLT_Ele17_SW_TightCaloEleId_SC8HE_L1R_v1")||inEvent->TriggerPassed("HLT_Ele17_SW_TightEleId_L1R") ) ) ||
	  ( inEvent->runNb>148100 && (inEvent->TriggerPassed("HLT_DoubleEle17_SW_L1R_v1")||
	                      inEvent->TriggerPassed("HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v2")|| 
			      inEvent->TriggerPassed("HLT_Ele17_SW_TightCaloEleId_Ele8HE_L1R_v1")|| 
			      inEvent->TriggerPassed("HLT_Ele22_SW_TighterEleId_L1R_v3")|| 
			      inEvent->TriggerPassed("HLT_Ele22_SW_TighterEleId_L1R_v2")|| 
			      inEvent->TriggerPassed("HLT_Ele17_SW_TighterEleIdIsol_L1R_v3")|| 
			      inEvent->TriggerPassed("HLT_Ele17_SW_TighterEleIdIsol_L1R_v2")  
			       ) ) ) TriggerPassed_ee_DATA   = true;
			       
     if ( ( inEvent->runNb<145000 && inEvent->TriggerPassed("HLT_Mu9")  ) ||  
          ( inEvent->runNb>145000 && inEvent->runNb<147120 && inEvent->TriggerPassed("HLT_Mu11")  ) || 
	  ( inEvent->runNb>147120 && inEvent->TriggerPassed("HLT_Mu15_v1")  ) ) TriggerPassed_mumu_DATA = true;

// now pass trigger_ee  && !trigger_mm requirement on EG data , then  
// pass trigger_mm && !trigger_ee requirement on MU data  
// then pass  trigger_ee && trigger_mm on the most complete dataset (EG or MU)    
      
//      if ( TriggerPassed_ee_DATA && !TriggerPassed_mumu_DATA ) keepEvent=true;
//      if ( TriggerPassed_mumu_DATA && !TriggerPassed_ee_DATA ) keepEvent=true;
      if ( TriggerPassed_ee_DATA && TriggerPassed_mumu_DATA) keepEvent=true;
       
	 
      if (!keepEvent)
	continue;

//endmodifdg

      outFile->cd ();

      //Copy non objects-vectors members;     
      outEvent->runNb = inEvent->runNb;
      outEvent->eventNb = inEvent->eventNb;
      outEvent->eventWeight = inEvent->eventWeight;
      outEvent->lumiblock = inEvent->lumiblock;
      outEvent->procId = inEvent->procId;
      outEvent->eventType = inEvent->eventType;
      outEvent->TMEME = inEvent->TMEME;
      outEvent->triggers = inEvent->triggers;
      outEvent->ptHat = inEvent->ptHat; 
      outEvent->pdgIdPartons = inEvent->pdgIdPartons;
      outEvent->match_HLT_Ele10_LW_L1R_recoEl = inEvent->match_HLT_Ele10_LW_L1R_recoEl;
      outEvent->p3BS = inEvent->p3BS;
      outEvent->p3BSErr = inEvent->p3BSErr;

    //Only selected objects are stored below

    // --- JetMet  
    std::vector < NTJetMet > jetMetVec;

    // ---- leptons
    std::vector < NTElectron > electrons;
    std::vector < NTMuon > muons;

//       for (unsigned int j = 0; j != objectsToKeep.size (); j++) {
// 	if (verbosity > 0) {
// 	  cout << "objectsToKeep[" << j << "] : " << endl;
// 	  cout << "name = " << objectsToKeep[j].name << " type = " << objectsToKeep[j].type << endl;
// 	  cout << "minPt = " << objectsToKeep[j].minPt << " maxEta = " << objectsToKeep[j].maxEta << endl;
// 	  if (objectsToKeep[j].skipObjects)
// 	    cout << "skipObjects = true" << endl;
// 	  else
// 	    cout << "skipObjects = false" << endl;
// 	}
//       }

      //if(!keepEvent) continue;
      for (unsigned int j = 0; j != objectsToKeep.size (); j++) {
	if (objectsToKeep[j].type == "Vertex") {
	  int verticesKeeped = 0;
	  for (unsigned int i = 0; i < inEvent->VertexVec.size (); i++) {
	    bool keepPrimaryVertex = true;
	    if (objectsToKeep[j].skipObjects) {
	      keepPrimaryVertex = false;
	      if (verbosity > 1)
		cout << "skip Primary Vertex" << endl;
	    }
	    else {
	      outEvent->VertexVec.push_back (inEvent->VertexVec[i]);
	      verticesKeeped++;
	    }
	  }

	  if (verticesKeeped < objectsToKeep[j].minNObjects) {
	    keepEvent = false;
	    if (verbosity > 1)
	      cout << "Too small number of selected Primary Vertices: verticesKeeped = " << verticesKeeped << endl;
	  }

	  if (verbosity > 1)
	    cout << "Processed " << objectsToKeep[j].name << endl;

	}
	else if (objectsToKeep[j].type == "Electron") {
	  int electronsKeeped = 0;
	  for (unsigned int i = 0; i < inEvent->electrons.size (); i++) {
	    bool keepElectron = true;
	    if (inEvent->electrons[i].p4.Pt () < objectsToKeep[j].minPt || fabs (inEvent->electrons[i].p4.Eta ()) > objectsToKeep[j].maxEta) {
	      if (objectsToKeep[j].skipObjects) {
		keepElectron = false;
		if (verbosity > 1)
		  cout << "skip Electron with pT = " << inEvent->electrons[i].p4.Pt () << " and eta = " << inEvent->electrons[i].p4.Eta () << endl;
	      }
	    }

	    if (keepElectron) {
	      outEvent->electrons.push_back (inEvent->electrons[i]);
	      electronsKeeped++;
	    }
	  }

	  if (electronsKeeped < objectsToKeep[j].minNObjects) {
	    keepEvent = false;
	    if (verbosity > 1)
	      cout << "Too small number of selected Electrons: electronsKeeped = " << electronsKeeped << endl;
	  }

	}

	else if (objectsToKeep[j].type == "Muon") {
	  int muonsKeeped = 0;
	  for (unsigned int i = 0; i < inEvent->muons.size (); i++) {
	    bool keepMuon = true;
	    if (inEvent->muons[i].p4.Pt () < objectsToKeep[j].minPt || fabs (inEvent->muons[i].p4.Eta ()) > objectsToKeep[j].maxEta) {
	      if (objectsToKeep[j].skipObjects) {
		keepMuon = false;
		if (verbosity > 1)
		  cout << "skip Muon with pT = " << inEvent->muons[i].p4.Pt () << " and eta = " << inEvent->muons[i].p4.Eta () << endl;
	      }
	    }

	    if (keepMuon) {
	      outEvent->muons.push_back (inEvent->muons[i]);
	      muonsKeeped++;
	    }
	  }

	  if (muonsKeeped < objectsToKeep[j].minNObjects) {
	    keepEvent = false;
	    if (verbosity > 1)
	      cout << "Too small number of selected Muons: muonsKeeped = " << muonsKeeped << endl;
	  }

	}

	
    	else if (objectsToKeep[j].type == "Tau") {
        bool doIt = false;
        int ix = 0;
        for (unsigned int x = 0; x < inEvent->tausVec.size (); x++)
	{
          if (objectsToKeep[j].name == inEvent->tausVec[x].algo) {
            doIt = true;
            ix = x;
          }
        if (doIt) {
          TopTree::NTCollection<NTTau> tauColl;
          int tausKeeped = 0;
          for (unsigned int i = 0; i < inEvent->tausVec[ix].objects.size (); i++) {
            bool keepTau = true;
            if (inEvent->tausVec[ix].objects[i].p4.Pt () < objectsToKeep[j].minPt || fabs (inEvent->tausVec[ix].objects[i].p4.Eta ()) > objectsToKeep[j].maxEta) {
              if (objectsToKeep[j].skipObjects) {
        	keepTau = false;
        	if (verbosity > 1)
        	  cout << "skip Tau with pT = " << inEvent->tausVec[ix].objects[i].p4.Pt () << " and eta = " << inEvent->tausVec[ix].objects[i].p4.Eta () << endl;
              }
            }

            if (keepTau) {
              tauColl.objects.push_back (inEvent->tausVec[ix].objects[i]);
              tausKeeped++;
            }
          }
          outEvent->NewTauColl (tauColl);


          if (tausKeeped < objectsToKeep[j].minNObjects) {
            keepEvent = false;
            if (verbosity > 1)
              cout << "Too small number of selected taus: tausKeeped = " << tausKeeped << endl;
          }
        }
       }
      }

	else if (objectsToKeep[j].type == "Track") {
	  int tracksKeeped = 0;
	  for (unsigned int i = 0; i < inEvent->tracks.size (); i++) {
	    bool keepTrack = true;
	    if (inEvent->tracks[i].p4.Pt () < objectsToKeep[j].minPt || fabs (inEvent->tracks[i].p4.Eta ()) > objectsToKeep[j].maxEta) {
	      if (objectsToKeep[j].skipObjects) {
		keepTrack = false;
		if (verbosity > 1)
		  cout << "skip Track with pT = " << inEvent->tracks[i].p4.Pt () << " and eta = " << inEvent->tracks[i].p4.Eta () << endl;
	      }
	    }

	    if (keepTrack) {
	      outEvent->tracks.push_back (inEvent->tracks[i]);
	      tracksKeeped++;
	    }
	  }

	  if (tracksKeeped < objectsToKeep[j].minNObjects) {
	    keepEvent = false;
	    if (verbosity > 1)
	      cout << "Too small number of selected Tracks: tracksKeeped = " << tracksKeeped << endl;
	  }

	}
	else if (objectsToKeep[j].type == "Jet") {
	  bool doIt = false;
	  int ix = 0;
	  for (unsigned int x = 0; x < inEvent->jetMetVec.size (); x++) {
//modifdg
//	    if (objectsToKeep[j].name == inEvent->jetMetVec[x].algo) {
	      doIt = true;
	      ix = x;
//	    }
//endmodifdg
	  if (doIt) {
	    //copy all members of JetMet expected the collection of jets
	    TopTree::NTJetMet jetmet;
	    jetmet.met = inEvent->jetMetVec[ix].met;
	    jetmet.sumEtJet = inEvent->jetMetVec[ix].sumEtJet;
	    jetmet.sumEtJetRaw = inEvent->jetMetVec[ix].sumEtJetRaw;
	    jetmet.algo = inEvent->jetMetVec[ix].algo;

	    int jetsKeeped = 0;
	    for (unsigned int i = 0; i < inEvent->jetMetVec[ix].objects.size (); i++) {
	      bool keepJet = true;
	      if (inEvent->jetMetVec[ix].objects[i].p4.Pt () < objectsToKeep[j].minPt || fabs (inEvent->jetMetVec[ix].objects[i].p4.Eta ()) > objectsToKeep[j].maxEta) {
		if (objectsToKeep[j].skipObjects) {
		  keepJet = false;
		  if (verbosity > 1)
		    cout << "skip Jet with pT = " << inEvent->jetMetVec[ix].objects[i].p4.Pt () << " and eta = " << inEvent->jetMetVec[ix].objects[i].p4.Eta () << endl;
		}
	      }

	      if (keepJet) {
		jetmet.objects.push_back (inEvent->jetMetVec[ix].objects[i]);
		jetsKeeped++;
	      }
	    }
	    outEvent->NewJetMet (jetmet);
            

	    if (jetsKeeped < objectsToKeep[j].minNObjects) {
	      keepEvent = false;
	      if (verbosity > 1)
		cout << "Too small number of selected jets: jetsKeeped = " << jetsKeeped << endl;
	    }
	  }
//modifdg
         } // x
//endmodifdg
	}
      }
      if (keepEvent) {
	nOutEvents++;		// nr of output events

	if (verbosity > 1)
	  cout << "Filling the outEventTree" << endl;

	outEventTree->Fill ();

	if (verbosity > 1)
	  cout << "outEventTree is filled" << endl;

      }

      if (verbosity > 1)
	cout << "Analyzing event is " << ievt << " finished!" << endl;

    }				// loop over events

    if (verbosity > 1)
//      cout << "Analyzing input file " << inFileName[nFile] << " finished!" << endl;

    if (inFile)
      inFile->Delete ();

  }				// loop over input files


  cout << "Filling outRunTree" << endl;

  cout << "Writing to output file" << endl;
  outFile->mkdir("MyModule");
  outFile->cd("MyModule");
  outEventTree->Write ();
//      outFile->Write();

  cout << "Closing output file" << endl;

  outFile->Close ();

  delete outFile;
  delete outEvent;

//      WARNING! Don't remove or modify the next line!!! The Automatic TopTree Producer depends on it!
  cout << "--> Skimmed " << nOutEvents << " out of a total of " << nInEvents << " events" << endl;

  if (((double) clock () - start) / CLOCKS_PER_SEC < 60)
    cout << "--> The skimming took " << ((double) clock () - start) / CLOCKS_PER_SEC << " seconds." << endl;

  else
    cout << "--> The skimming took " << (((double) clock () - start) / CLOCKS_PER_SEC) / 60 << " minutes." << endl;

  cout << "Code running was succesfull!" << endl;

  return (0);
}
