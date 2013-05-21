#include <iomanip>
#include <iostream>
#include <limits.h>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
// #include "../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
// #include "../Plots/interface/DiLepAnaHistoManager.h"
// #include "../BckgdEstimation/interface/DYEstimation.h"
// #include "../BckgdEstimation/interface/MMEstimation.h"

#include "../../MassAMWT/interface/AMWT.h"

using namespace TopTree;
using namespace std;


int main( int argc, const char* argv[] ){

  // List of arguments:
  // 0: The executable itself
  // 1: The number of times JetMET is smeared (not smeared if equal to 1)
  // 2: The scale and type of JES shift (if fabs(x) < 0.5, shift by eta, else absolute

// fixme: Depracated
   float JES_JetMET = 1.0;

  string xmlFileName = string ("./MyMassAnalysis.xml");

  for (int i=1;i<argc;++i) {
    if (strncmp(argv[i],"-c",2)==0) xmlFileName = string(argv[i+1]);
    if (strncmp(argv[i],"-h",2)==0) {
      cout << " -c xm_config_file\n";
      exit(1);
    }
  }

  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;

  //////////////////////
  //Global variables
  //////////////////////
  vector < Dataset > datasets;
  DiLeptonSelection sel;
  float Luminosity = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0;
  int verbosity = -1;
  
  // Analysis variables
  int step;
  //4-vectors
  vector < TLorentzVector > nu1;
  vector < TLorentzVector > nu2;
  TLorentzVector lvTop1;
  TLorentzVector lvTop2;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );

  int nEvents;
  int firstEvent;
  int initialEvents;
  anaEL.LoadAnalysisInfo("Run", "firstEvent", firstEvent);
  anaEL.LoadAnalysisInfo("Run", "nEvents", nEvents);
  anaEL.LoadAnalysisInfo("Run", "initialEvents", initialEvents);
  string channelName;
  anaEL.LoadAnalysisInfo("MassAnalysis", "ChannelName", channelName);

  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)

  //******************************************
  //Load Scale Factors for lepton efficiencies
  //******************************************
  sel.LoadElScaleFactors();
  sel.LoadMuScaleFactors();
  sel.InitJESUnc();

  TopTree::NTEvent * event = 0;

  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<" Loop over the datasets  "<<endl;
    cout<<"#########################"<<endl;
  }

  for (unsigned int d = 0; d < datasets.size (); d++) {

    AMWT amwt(anaEL, datasets[d].isData());

    TString sample_name(datasets[d].Name());


    bool isData = datasets[d].isData ();
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);

    cout << "Sample : " << sample_name<< endl;
    cout << "Data   : " << isData<< endl;

    unsigned int nEventsSample = (int) (datasets[d].eventTree ()->GetEntries ());
    unsigned int endEventToRun;

    if (firstEvent>nEventsSample) firstEvent = nEventsSample;
    if ((nEvents==-1)||((nEvents+firstEvent)>nEventsSample)) endEventToRun = nEventsSample;
    else endEventToRun = nEvents+firstEvent;
    cout << "Events to run: number / first / last / all: " << endEventToRun-firstEvent 
         << " / " << firstEvent << " / " << endEventToRun
         << " / " << nEventsSample << endl;

// Standard parameters for doFullSelection:

    bool applyEEScale = false; float EEScaleParam = 1.; bool applyEEResol = false; float EEResolParam = 1.;
    bool applyMEScale = false; float MEScaleParam = 1.; bool applyMEResol = false; float MEResolParam = 1.;
    bool applyMETS = false; float METScale = 1.;
    bool applyJES = false; float JESParam = 1.;
    bool applyJER = false; float JERFactor = 0.; 

    //////////////////////
    //LOOP OVER THE EVENTS
    //////////////////////
    for (unsigned int ievt = firstEvent; ievt < endEventToRun; ievt++) {
//           for (unsigned int ievt = 0; ievt < 10; ievt++) {

      datasets[d].eventTree ()->GetEntry (ievt);

      if(ievt%1000 == 0) cout << "number of processed events " << ievt << endl;
      if (verbosity >= 1) cout << "Event : "<< event->runNb<<" , " << event->lumiblock <<" , " << event->eventNb<<endl;


      //Load event for the selection
      sel.LoadEvent(event);

      int finalCut = sel.doFullSelection(&(datasets[d]), channelName, false, /*print*/ verbosity,
	false, false, -1., -1., applyJES,  JESParam,
	 applyEEScale,  EEScaleParam,  applyEEResol,  EEResolParam,
	 applyMEScale,  MEScaleParam,  
	 applyJER,  JERFactor,  applyMETS,  METScale);

      if (finalCut >5) {
	TopMassVariables * tmv = amwt.findMass(sel);
	tmv->printAll();
	delete tmv;
      }
    }  // end of loop over evts
  }				// end of loop over the datasets


//  if(verbosity>0) {
    cout<<"#########################"<<endl;
    cout<<"    End of the program   "<<endl;
    cout<<"#########################"<<endl;
//  }

  return (0);
}
