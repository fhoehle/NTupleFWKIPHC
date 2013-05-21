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
#include <time.h>

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"
#include "../../Measurements/interface/DiLepXsectionMeas.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>


#include "../../tinyxml/tinyxml.h"

#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjString.h"
#include "TH1F.h"

using namespace TopTree;
using namespace std;

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

struct BkgNumbers{
	string name;
	float number;
	float errm;
	float errp;
	bool isASyst;
};

struct ChannelNumbers{
	string channel;
	float nData;
	float nSignal;// per pb-1
	vector<BkgNumbers> nBkg;
};

//vector < TString > ReadNumbersFromXml (TiXmlDocument doc, string channel, float nData, float nSignal, vector<BkgNumbers> nBkg)
vector < TString > ReadNumbersFromXml (TiXmlDocument doc, string channel, ChannelNumbers& chNumbers)
{
  vector < TString > inFileName;
  TiXmlHandle hdl (&doc);
  TiXmlNode *node = 0;
  TiXmlElement *elem = 0;
  bool nodeFound = false;
  node = hdl.Node ();
  for (node = node->FirstChild (); node; node = node->NextSibling ()) {
    if (node->Value () == string ("EvtNumbers")) {
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
	if(elem->Value() == string("Channel") && elem->Attribute("channel") == channel){
		chNumbers.channel = channel;
  		TiXmlElement *ChannelElem = elem->FirstChildElement();
		while(ChannelElem){
  		//for (ChannelElem = ChannelElem->FirstChild (); ChannelElem; ChannelElem = ChannelElem->NextSibling ()) {
			string name;
			float number = 0;
			float errm = 0;
			float errp = 0;
			bool isASyst = 0;
			int isASyst_i = 0;
			name = ChannelElem->Attribute("name");
			cout<<"name: "<<name<<endl;	
			if(name==string("DATA")){
				ChannelElem->QueryFloatAttribute("number", &chNumbers.nData);	
			}
			else{
				if(name.find("TTbar")<=name.size() && name.find("TTbar")>=0){
					ChannelElem->QueryFloatAttribute("number", &chNumbers.nSignal);	
				}
				else{
					ChannelElem->QueryFloatAttribute("number", &number);	
					ChannelElem->QueryFloatAttribute("errm", &errm);	
					ChannelElem->QueryFloatAttribute("errp", &errp);
					ChannelElem->QueryIntAttribute("isASyst", &isASyst_i);
					if(isASyst_i != 0) isASyst = true;
					BkgNumbers bkgnb;
					bkgnb.name = name;
					bkgnb.number = number;
					bkgnb.errm = errm;
					bkgnb.errp = errp;
					bkgnb.isASyst = isASyst;
					chNumbers.nBkg.push_back(bkgnb);	
				}
			}	

			ChannelElem = ChannelElem->NextSiblingElement ();	// iteration 
		}
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
  return inFileName;
}

void InitSysEffect(SystEffect* a, int size){
	for(int i=0;i<size;i++){
		a[i].doIt = false;
		a[i].mean = 1.;
		a[i].m1sigma = 0;
		a[i].p1sigma = 0;
		a[i].effmean = 1.;
		a[i].effm1s = 0;
		a[i].effp1s = 0;
	}
}


int main (int argc, char *argv[])
{
 
  cout << "#########################" << endl;
  cout << "Beginning of the program" << endl;
  cout << "#########################" << endl;

  //////////////////////
  //Global variables
  //////////////////////
  vector < Dataset > datasets;
  DiLeptonSelection sel;
  float Luminosity = 0;
  float LumiError = 0.;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0;
  int verbosity = -1;
  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName;
  cout<<"argc "<<argc<<" "<<argv[0]<<endl;
  if (argc>1 ) xmlFileName = string(argv[1]);
  else xmlFileName = string ("../../config/Likelihood.xml");
  TiXmlDocument doc (xmlFileName.c_str());
  if (!doc.LoadFile ()) {
    cerr << "Error while loading the xml file: " << xmlFileName << endl;
    cerr << " error #" << doc.ErrorId () << " : " << doc.ErrorDesc () << endl;
    return 1;
  }

  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets);	// now the list of datasets written in the xml file is known
  anaEL.LoadGeneralInfo (DataType, Luminosity, LumiError, verbosity);

  ChannelNumbers numbers_ee;
  ChannelNumbers numbers_emu;
  ChannelNumbers numbers_mumu;
  ReadNumbersFromXml (doc, string("ee"), numbers_ee);
  ReadNumbersFromXml (doc, string("emu"), numbers_emu);
  ReadNumbersFromXml (doc, string("mumu"), numbers_mumu);
  if(verbosity>1){
	cout<<"###########################"<<endl;
  	cout<<"For the channel ee"<<endl;
  	cout<<"Data: "<<numbers_ee.nData<<" evts"<<endl;
  	cout<<"Signal: "<<numbers_ee.nSignal<<" evts"<<endl;
  	for(unsigned int i=0;i<numbers_ee.nBkg.size();i++){
  		cout<<numbers_ee.nBkg[i].name<<": "<<numbers_ee.nBkg[i].number<<" evts"<<" errors (-/+): "<<numbers_ee.nBkg[i].errm<<"/"<<numbers_ee.nBkg[i].errp<<endl;
  	}
	cout<<"###########################"<<endl;
  	cout<<"For the channel emu"<<endl;
  	cout<<"Data: "<<numbers_emu.nData<<" evts"<<endl;
  	cout<<"Signal: "<<numbers_emu.nSignal<<" evts"<<endl;
  	for(unsigned int i=0;i<numbers_emu.nBkg.size();i++){
  		cout<<numbers_emu.nBkg[i].name<<": "<<numbers_emu.nBkg[i].number<<" evts"<<" errors (-/+): "<<numbers_emu.nBkg[i].errm<<"/"<<numbers_emu.nBkg[i].errp<<endl;
  	}
	cout<<"###########################"<<endl;
  	cout<<"For the channel mumu"<<endl;
  	cout<<"Data: "<<numbers_mumu.nData<<" evts"<<endl;
  	cout<<"Signal: "<<numbers_mumu.nSignal<<" evts"<<endl;
  	for(unsigned int i=0;i<numbers_mumu.nBkg.size();i++){
  		cout<<numbers_mumu.nBkg[i].name<<": "<<numbers_mumu.nBkg[i].number<<" evts"<<" errors (-/+): "<<numbers_mumu.nBkg[i].errm<<"/"<<numbers_mumu.nBkg[i].errp<<endl;
  	}
	cout<<"###########################"<<endl;
  }

  //////////////////////////////////
  //Computation of the Xsection
  //////////////////////////////////

  //read the xml file for XExtraction
  XExtractionChannel xec_ee;
  XExtractionChannel xec_emu;
  XExtractionChannel xec_mumu;
  anaEL.LoadXExtraction(string("ee"), xec_ee);
  anaEL.LoadXExtraction(string("emu"), xec_emu);
  anaEL.LoadXExtraction(string("mumu"), xec_mumu);


  ///////////////////////////////
  //List of systematics
  ///////////////////////////////
  if (verbosity > 2) {
    cout << " ######################################" << endl;
    cout << "	Systematics 'effect'	       " << endl;
    cout << " ######################################" << endl;
  }
  
  //0: ee - 1: emu - 2: mumu 
  //Declaration then initialisation
  SystEffect SysBtag[3]; 
  InitSysEffect(SysBtag,3);
  SystEffect SysLtag[3]; 
  InitSysEffect(SysLtag,3);
  SystEffect SysJES[3];
  InitSysEffect(SysJES,3);
  SystEffect SysJER[3];
  InitSysEffect(SysJER,3);
  SystEffect SysMETS[3];
  InitSysEffect(SysMETS,3);
  SystEffect SysEES[3];
  InitSysEffect(SysEES,3);
  SystEffect SysMES[3];
  InitSysEffect(SysMES,3);
  SystEffect SysLEF[3]; 
  InitSysEffect(SysLEF,3);
  SystEffect SysTEF[3]; 
  InitSysEffect(SysTEF,3);
  SystEffect SysBR[3]; 
  InitSysEffect(SysBR,3);
  SystEffect SysPDF[3]; 
  InitSysEffect(SysPDF,3);
  SystEffect SysQ2[3]; 
  InitSysEffect(SysQ2,3);
  SystEffect SysMEPSThr[3]; 
  InitSysEffect(SysMEPSThr,3);
  SystEffect SysMtop[3]; 
  InitSysEffect(SysMtop,3);
  SystEffect SysISRFSR[3]; 
  InitSysEffect(SysISRFSR,3);
  SystEffect SysPU[3]; 
  InitSysEffect(SysPU,3);
  ////////////////////////////////

  vector<string> channels_;
  channels_.push_back(string("ee"));
  channels_.push_back(string("emu"));
  channels_.push_back(string("mumu"));
  //Add SystematicsEffect: plug directly numbers !
  for(int i=0;i<3;i++){
  	anaEL.LoadSystematicsEffect(string("Btag"), channels_[i], SysBtag[i]);
  	anaEL.LoadSystematicsEffect(string("Ltag"), channels_[i], SysLtag[i]);
  	anaEL.LoadSystematicsEffect(string("JES"), channels_[i], SysJES[i]);
 	anaEL.LoadSystematicsEffect(string("JER"), channels_[i], SysJER[i]);
  	anaEL.LoadSystematicsEffect(string("METS"), channels_[i], SysMETS[i]);
  	anaEL.LoadSystematicsEffect(string("EES"), channels_[i], SysEES[i]);
  	anaEL.LoadSystematicsEffect(string("MES"), channels_[i], SysMES[i]);
  	anaEL.LoadSystematicsEffect(string("LEF"), channels_[i], SysLEF[i]);
  	anaEL.LoadSystematicsEffect(string("TEF"), channels_[i], SysTEF[i]);
  	anaEL.LoadSystematicsEffect(string("BR"), channels_[i], SysBR[i]);
  	anaEL.LoadSystematicsEffect(string("PDF"), channels_[i], SysPDF[i]);
  	anaEL.LoadSystematicsEffect(string("Q2"), channels_[i], SysQ2[i]);
  	anaEL.LoadSystematicsEffect(string("ME-PS-Thr"), channels_[i], SysMEPSThr[i]);
  	anaEL.LoadSystematicsEffect(string("Mtop"), channels_[i], SysMtop[i]);
  	anaEL.LoadSystematicsEffect(string("ISR/FSR"), channels_[i], SysISRFSR[i]);
  	anaEL.LoadSystematicsEffect(string("PU"), channels_[i], SysPU[i]);
  	if (verbosity > 2){
		cout << " ############ Channel "<<channels_[i]<<"       ############"<<endl;
    		cout << " - Btag: " << SysBtag[i].m1sigma << " - " << SysBtag[i].mean << " - " << SysBtag[i].p1sigma << " Eff:  " << SysBtag[i].effm1s << " - " << SysBtag[i].effmean << " - "<< SysBtag[i].effp1s << endl;
    		cout << " - Ltag: " << SysLtag[i].m1sigma << " - " << SysLtag[i].mean << " - " << SysLtag[i].p1sigma << " Eff:  " << SysLtag[i].effm1s << " - " << SysLtag[i].effmean << " - "<< SysLtag[i].effp1s << endl;
    		cout << " - JES: " << SysJES[i].m1sigma << " - " << SysJES[i].mean << " - " << SysJES[i].p1sigma << " Eff:  " << SysJES[i].effm1s << " - " << SysJES[i].effmean << " - "<< SysJES[i].effp1s << endl;
    		cout << " - JER: " << SysJER[i].m1sigma << " - " << SysJER[i].mean << " - " << SysJER[i].p1sigma << " Eff:  " << SysJER[i].effm1s << " - " << SysJER[i].effmean << " - "<< SysJER[i].effp1s << endl;
    		cout << " - METS: " << SysMETS[i].m1sigma << " - " << SysMETS[i].mean << " - " << SysMETS[i].p1sigma << " Eff:  " << SysMETS[i].effm1s << " - " << SysMETS[i].effmean << " - "<< SysMETS[i].effp1s << endl;
    		cout << " - EES: " << SysEES[i].m1sigma << " - " << SysEES[i].mean << " - " << SysEES[i].p1sigma << " Eff:  " << SysEES[i].effm1s << " - " << SysEES[i].effmean << " - "<< SysEES[i].effp1s << endl;
    		cout << " - MES: " << SysMES[i].m1sigma << " - " << SysMES[i].mean << " - " << SysMES[i].p1sigma << " Eff:  " << SysMES[i].effm1s << " - " << SysMES[i].effmean << " - "<< SysMES[i].effp1s << endl;
    		cout << " - LEF: " << SysLEF[i].m1sigma << " - " << SysLEF[i].mean << " - " << SysLEF[i].p1sigma << " Eff:  " << SysLEF[i].effm1s << " - " << SysLEF[i].effmean << " - "<< SysLEF[i].effp1s << endl;
    		cout << " - TEF: " << SysTEF[i].m1sigma << " - " << SysTEF[i].mean << " - " << SysTEF[i].p1sigma << " Eff:  " << SysTEF[i].effm1s << " - " << SysTEF[i].effmean << " - "<< SysTEF[i].effp1s << endl;
    		cout << " - BR: " << SysBR[i].m1sigma << " - " << SysBR[i].mean << " - " << SysBR[i].p1sigma << " Eff:  " << SysBR[i].effm1s << " - " << SysBR[i].effmean << " - "<< SysBR[i].effp1s << endl;
    		cout << " - PDF: " << SysPDF[i].m1sigma << " - " << SysPDF[i].mean << " - " << SysPDF[i].p1sigma << " Eff:  " << SysPDF[i].effm1s << " - " << SysPDF[i].effmean << " - "<< SysPDF[i].effp1s << endl;
    		cout << " - Q2: " << SysQ2[i].m1sigma << " - " << SysQ2[i].mean << " - " << SysQ2[i].p1sigma << " Eff:  " << SysQ2[i].effm1s << " - " << SysQ2[i].effmean << " - "<< SysQ2[i].effp1s << endl;
    		cout << " - MEPSThr: " << SysMEPSThr[i].m1sigma << " - " << SysMEPSThr[i].mean << " - " << SysMEPSThr[i].p1sigma << " Eff:  " << SysMEPSThr[i].effm1s << " - " << SysMEPSThr[i].effmean << " - "<< SysMEPSThr[i].effp1s << endl;
    		cout << " - Mtop: " << SysMtop[i].m1sigma << " - " << SysMtop[i].mean << " - " << SysMtop[i].p1sigma << " Eff:  " << SysMtop[i].effm1s << " - " << SysMtop[i].effmean << " - "<< SysMtop[i].effp1s << endl;
    		cout << " - ISRFSR: " << SysISRFSR[i].m1sigma << " - " << SysISRFSR[i].mean << " - " << SysISRFSR[i].p1sigma << " Eff:  " << SysISRFSR[i].effm1s << " - " << SysISRFSR[i].effmean << " - "<< SysISRFSR[i].effp1s << endl;
    		cout << " - PU: " << SysPU[i].m1sigma << " - " << SysPU[i].mean << " - " << SysPU[i].p1sigma << " Eff:  " << SysPU[i].effm1s << " - " << SysPU[i].effmean << " - "<< SysPU[i].effp1s << endl;
   	 	cout << " ######################################" << endl;
 	}
  }

  //////////////
  //channel ee
  //////////////
  DiLepXsectionMeas meas_ee (string("ee"), string ("TTbarEE"), xec_ee.BR);
  if(xec_ee.doIt){
  	//Luminosity has to be provided before loading the datasets !! It's important for the weight computation
  	meas_ee.SetLumi (Luminosity, LumiError);
  	meas_ee.LoadDatasets (datasets);
  	if(SysJES[0].doIt) meas_ee.AddUncertainty(string ("JES"), SysJES[0]);
  	if(SysJER[0].doIt) meas_ee.AddUncertainty(string ("JER"), SysJER[0]);
  	if(SysMETS[0].doIt) meas_ee.AddUncertainty(string ("METS"), SysMETS[0]);
  	if(SysEES[0].doIt) meas_ee.AddUncertainty(string ("EES"), SysEES[0]);
  	if(SysBtag[0].doIt) meas_ee.AddUncertainty(string("Btag"),SysBtag[0]);
  	if(SysLtag[0].doIt) meas_ee.AddUncertainty(string("Ltag"),SysLtag[0]);
  	if(SysLEF[0].doIt) meas_ee.AddUncertainty(string("LEF"),SysLEF[0]);
  	if(SysTEF[0].doIt) meas_ee.AddUncertainty(string("TEF"),SysTEF[0]);
  	if(SysBR[0].doIt) meas_ee.AddUncertainty(string("BR"),SysBR[0]);
  	if(SysPDF[0].doIt) meas_ee.AddUncertainty(string("PDF"),SysPDF[0]);
  	if(SysQ2[0].doIt) meas_ee.AddUncertainty(string("Q2"),SysQ2[0]);
  	if(SysMEPSThr[0].doIt) meas_ee.AddUncertainty(string("MEPSThr"),SysMEPSThr[0]);
  	if(SysMtop[0].doIt) meas_ee.AddUncertainty(string("Mtop"),SysMtop[0]);
  	if(SysISRFSR[0].doIt) meas_ee.AddUncertainty(string("ISRFSR"),SysISRFSR[0]);
  	if(SysPU[0].doIt) meas_ee.AddUncertainty(string("PU"),SysPU[0]);
  }

  //////////////
  //channel mumu
  //////////////
  DiLepXsectionMeas meas_mumu (string("mumu"), string ("TTbarMM"), xec_mumu.BR);
  if(xec_mumu.doIt){
  	//Luminosity has to be provided before loading the datasets !! It's important for the weight computation
  	meas_mumu.SetLumi (Luminosity, LumiError);
  	meas_mumu.LoadDatasets (datasets);
  	if(SysJES[1].doIt) meas_mumu.AddUncertainty(string ("JES"), SysJES[1]);
  	if(SysJER[1].doIt) meas_mumu.AddUncertainty(string ("JER"), SysJER[1]);
  	if(SysMETS[1].doIt) meas_mumu.AddUncertainty(string ("METS"), SysMETS[1]);
  	if(SysMES[1].doIt) meas_mumu.AddUncertainty(string ("MES"), SysMES[1]);
  	if(SysBtag[1].doIt) meas_mumu.AddUncertainty(string("Btag"),SysBtag[1]);
  	if(SysLtag[1].doIt) meas_mumu.AddUncertainty(string("Ltag"),SysLtag[1]);
  	if(SysLEF[1].doIt) meas_mumu.AddUncertainty(string("LEF"),SysLEF[1]);
  	if(SysTEF[1].doIt) meas_mumu.AddUncertainty(string("TEF"),SysTEF[1]);
  	if(SysBR[1].doIt) meas_mumu.AddUncertainty(string("BR"),SysBR[1]);
  	if(SysPDF[1].doIt) meas_mumu.AddUncertainty(string("PDF"),SysPDF[1]);
  	if(SysQ2[1].doIt) meas_mumu.AddUncertainty(string("Q2"),SysQ2[1]);
  	if(SysMEPSThr[1].doIt) meas_mumu.AddUncertainty(string("MEPSThr"),SysMEPSThr[1]);
  	if(SysMtop[1].doIt) meas_ee.AddUncertainty(string("Mtop"),SysMtop[1]);
  	if(SysISRFSR[1].doIt) meas_mumu.AddUncertainty(string("ISRFSR"),SysISRFSR[1]);
  	if(SysPU[1].doIt) meas_mumu.AddUncertainty(string("PU"),SysPU[1]);
  }

  //////////////
  //channel emu
  //////////////
  
  DiLepXsectionMeas meas_emu (string("emu"), string ("TTbarEM"), xec_emu.BR);
  if(xec_emu.doIt){
  	//Luminosity has to be provided before loading the datasets !! It's important for the weight computation
  	meas_emu.SetLumi (Luminosity, LumiError);
  	meas_emu.LoadDatasets (datasets);
  	if(SysJES[2].doIt) meas_emu.AddUncertainty(string ("JES"), SysJES[2]);
  	if(SysJER[2].doIt) meas_emu.AddUncertainty(string ("JER"), SysJER[2]);
  	if(SysMETS[2].doIt) meas_emu.AddUncertainty(string ("METS"), SysMETS[2]);
  	if(SysEES[2].doIt) meas_emu.AddUncertainty(string ("EES"), SysEES[2]);
  	if(SysMES[2].doIt) meas_emu.AddUncertainty(string ("MES"), SysMES[2]);
  	if(SysBtag[2].doIt) meas_emu.AddUncertainty(string("Btag"),SysBtag[2]);
  	if(SysLtag[2].doIt) meas_emu.AddUncertainty(string("Ltag"),SysLtag[2]);
  	if(SysLEF[2].doIt) meas_emu.AddUncertainty(string("LEF"),SysLEF[2]);
  	if(SysTEF[2].doIt) meas_emu.AddUncertainty(string("TEF"),SysTEF[2]);
  	if(SysBR[2].doIt) meas_emu.AddUncertainty(string("BR"),SysBR[2]);
  	if(SysPDF[2].doIt) meas_emu.AddUncertainty(string("PDF"),SysPDF[2]);
  	if(SysQ2[2].doIt) meas_emu.AddUncertainty(string("Q2"),SysQ2[2]);
  	if(SysMEPSThr[2].doIt) meas_emu.AddUncertainty(string("MEPSThr"),SysMEPSThr[2]);
  	if(SysMtop[2].doIt) meas_ee.AddUncertainty(string("Mtop"),SysMtop[2]);
  	if(SysISRFSR[2].doIt) meas_emu.AddUncertainty(string("ISRFSR"),SysISRFSR[2]);
  	if(SysPU[2].doIt) meas_emu.AddUncertainty(string("PU"),SysPU[2]);
  }

  ////////////////////////////
  //  Xsection extraction
  ////////////////////////////

  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Cross-section extraction" << endl;
    cout << "#########################" << endl;
  }
  bool doMCPseudoExp = false;
  int NofPseudoExp = 1;
  bool SkipOneByOne = true;
  //if(DataType == 0) doMCPseudoExp = true; 
  
  //variables for PLR plots
  int Nbins_PLR = 100;
  float PLR_min = 60;
  float PLR_max = 260;
  int verboseXEx = 1; // I don't know yet why but has to be equal to 1 !!!

  if(xec_ee.doIt){
	if(verbosity>1){
		cout<<"####  Channel ee     ####"<<endl;
	}
	//Put the selection efficiency
	meas_ee.SetSelEff(string("TTbarEE"), numbers_ee.nSignal);
  	for(unsigned int i=0;i<numbers_ee.nBkg.size();i++){
		meas_ee.BkgDataDrivenEstimation(numbers_ee.nBkg[i].name,numbers_ee.nBkg[i].number,numbers_ee.nBkg[i].errm,numbers_ee.nBkg[i].errp,numbers_ee.nBkg[i].isASyst);
	}
	meas_ee.XExtraction (numbers_ee.nData, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx, SkipOneByOne);
  }
  if(xec_emu.doIt){
	if(verbosity>1){
		cout<<"####  Channel emu     ####"<<endl;
	}
	//Put the selection efficiency
	meas_emu.SetSelEff(string("TTbarEM"), numbers_emu.nSignal);
  	for(unsigned int i=0;i<numbers_emu.nBkg.size();i++){
		meas_emu.BkgDataDrivenEstimation(numbers_emu.nBkg[i].name,numbers_emu.nBkg[i].number,numbers_emu.nBkg[i].errm,numbers_emu.nBkg[i].errp,numbers_emu.nBkg[i].isASyst);
	}
	meas_emu.XExtraction (numbers_emu.nData, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx, SkipOneByOne);
  }
  if(xec_mumu.doIt){
	if(verbosity>1){
		cout<<"####  Channel mumu     ####"<<endl;
	}
	//Put the selection efficiency
	meas_mumu.SetSelEff(string("TTbarMM"), numbers_mumu.nSignal);
  	for(unsigned int i=0;i<numbers_mumu.nBkg.size();i++){
		meas_mumu.BkgDataDrivenEstimation(numbers_mumu.nBkg[i].name,numbers_mumu.nBkg[i].number,numbers_mumu.nBkg[i].errm,numbers_mumu.nBkg[i].errp,numbers_mumu.nBkg[i].isASyst);
	}
	meas_mumu.XExtraction (numbers_mumu.nData, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx, SkipOneByOne);
  }

  string ofilename;
  if(argc>2) ofilename = argv[2];
  else ofilename = string("Likelihood.tex"); 
  ofstream ofile (ofilename.c_str());
  ofile << "\\documentclass[8pt]{article}" << endl;
  ofile << "\\begin{document}" << endl;
  //Add XSectionMeas
  if(xec_ee.doIt){
  	meas_ee.EfficienciesTable (ofile);
  	meas_ee.UncertaintiesTable (ofile, SkipOneByOne);
  	meas_ee.MinimizationTable (ofile);
  }
  if(xec_emu.doIt){
  	meas_emu.EfficienciesTable (ofile);
  	meas_emu.UncertaintiesTable (ofile, SkipOneByOne);
  	meas_emu.MinimizationTable (ofile);
  }
  if(xec_mumu.doIt){
  	meas_mumu.EfficienciesTable (ofile);
  	meas_mumu.UncertaintiesTable (ofile, SkipOneByOne);
  	meas_mumu.MinimizationTable (ofile);
  }

  ofile << "\\end{document}" << endl;
  //system ("pdflatex Likelihood.tex");


  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Write output root file " << endl;
    cout << "#########################" << endl;
  }
  string rootfilename = ofilename+string(".root");
  TFile *fout = new TFile (rootfilename.c_str(), "RECREATE");
  if(xec_ee.doIt) meas_ee.Write (fout);
  if(xec_emu.doIt) meas_emu.Write (fout);
  if(xec_mumu.doIt) meas_mumu.Write (fout);
  //fout->Write();
  fout->Close ();
  delete fout;

  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << "    End of the program   " << endl;
    cout << "#########################" << endl;
  }

  return (0);
}
