#include <iomanip>
#include <iostream>
#include <time.h>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
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

using namespace TopTree;
using namespace std;



void ComputeSystematic(string Channel, Dataset& dataset, int idataset, DiLeptonSelection& sel, DiLepXsectionMeas& meas, int AnaStep, float weight, string SysName, Syst SysValues){
      int selLastStep = 0;
      if(SysName==string("JES")){
        //btag weight is computed for JES!
        vector < float >weightb;
        weightb.push_back (weight);
        weightb.push_back (0.);
        weightb.push_back (0.);
        weightb.push_back (0.);
        weightb.push_back (0.);
      	selLastStep = sel.doFullSelection (&(dataset), weightb, Channel, false, false, false, false, -1., -1., false, SysValues.mean);
     	 if (selLastStep >= AnaStep) 
		meas.Fill (idataset, SysName, 0, weightb[0]);	//0: nominal
        weightb[0]=weight; // re-init
      	selLastStep = sel.doFullSelection (&(dataset), weightb, Channel, false, false, false, false, -1., -1., true, SysValues.m1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 1, weightb[0]);	//1: minus
        weightb[0]=weight; // re-init
      	selLastStep = sel.doFullSelection (&(dataset), weightb, Channel, false, false, false, false, -1., -1., true, SysValues.p1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 2, weightb[0]);	//2: plus
     }
      if(SysName==string("JER")){
        //btag weight is computed for JER!
        vector < float >weightb;
        weightb.push_back (weight);
        weightb.push_back (0.);
        weightb.push_back (0.);
        weightb.push_back (0.);
        weightb.push_back (0.);
      	selLastStep = sel.doFullSelection (&(dataset), weightb, Channel, false, false, false, false, -1., -1., false, 1., false, 1., false, 1., true, SysValues.mean);
     	 if (selLastStep >= AnaStep) 
		meas.Fill (idataset, SysName, 0, weightb[0]);	//0: nominal
        weightb[0]=weight; // re-init
      	selLastStep = sel.doFullSelection (&(dataset), weightb, Channel, false, false, false, false, -1., -1., false, 1., false, 1., false, 1., true, SysValues.m1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 1, weightb[0]);	//1: minus
        weightb[0]=weight; // re-init
      	selLastStep = sel.doFullSelection (&(dataset), weightb, Channel, false, false, false, false, -1., -1., false, 1., false, 1., false, 1., true, SysValues.p1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 2, weightb[0]);	//2: plus
     }
     if(SysName==string("EES")){
        //btag weight is not computed here, taken from before...
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., false, SysValues.mean);
     	 if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 0, weight);	//0: nominal
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., true, SysValues.m1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 1, weight);	//1: minus
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., true, SysValues.p1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 2, weight);	//2: plus
     }
     if(SysName==string("MES")){
        //btag weight is not computed here, taken from before...
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., false, 1., false, SysValues.mean);
     	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 0, weight);	//0: nominal
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., false, 1., true, SysValues.m1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 1, weight);	//1: minus
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., false, 1., true, SysValues.p1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 2, weight);	//2: plus
     }
     if(SysName==string("METS")){
        //btag weight is not computed here, taken from before...
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., false, 1., false, 1., false, 1., false, SysValues.mean);
     	 if (selLastStep >= AnaStep) 
		meas.Fill (idataset, SysName, 0, weight);	//0: nominal
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., false, 1., false, 1., false, 1., true, SysValues.m1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 1, weight);	//1: minus
      	selLastStep = sel.doFullSelection (&(dataset), Channel, false, false, false, false, -1., -1., false, 1., false, 1., false, 1., false, 1., true, SysValues.p1sigma);
      	if (selLastStep >= AnaStep)
		meas.Fill (idataset, SysName, 2, weight);	//2: plus
     }
     if(SysName==string("btagEff")){
      vector < float >weightb;
      weightb.push_back (weight);
      weightb.push_back (0.);
      weightb.push_back (0.);
      weightb.push_back (0.);
      weightb.push_back (0.);
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 0);
      selLastStep = sel.doFullSelection (&(dataset), weightb, Channel);
      if (selLastStep >= AnaStep) 
	meas.Fill (idataset, SysName, 0, weightb[0]);	//0: nominal
      //reinint
      weightb[0] = weight;
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 4);
      selLastStep = sel.doFullSelection (&(dataset), weightb, Channel);
      if (selLastStep >= AnaStep)
	meas.Fill (idataset, SysName, 1, weightb[0]);	//1: minus
      //reinint
      weightb[0] = weight;
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 3);
      selLastStep = sel.doFullSelection (&(dataset), weightb, Channel);
      if (selLastStep >= AnaStep)
	meas.Fill (idataset, SysName, 2, weightb[0]);	//2: plus
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 0); // back to normal!
     }
     if(SysName==string("btagFake")){
      vector < float >weightb;
      weightb.push_back (weight);
      weightb.push_back (0.);
      weightb.push_back (0.);
      weightb.push_back (0.);
      weightb.push_back (0.);
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 0);
      selLastStep = sel.doFullSelection (&(dataset), weightb, Channel);
      if (selLastStep >= AnaStep) 
	meas.Fill (idataset, SysName, 0, weightb[0]);	//0: nominal
      //reinint
      weightb[0] = weight;
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 6);
      selLastStep = sel.doFullSelection (&(dataset), weightb, Channel);
      if (selLastStep >= AnaStep)
	meas.Fill (idataset, SysName, 1, weightb[0]);	//1: minus
      //reinint
      weightb[0] = weight;
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 5);
      selLastStep = sel.doFullSelection (&(dataset), weightb, Channel);
      if (selLastStep >= AnaStep)
	meas.Fill (idataset, SysName, 2, weightb[0]);	//2: plus
      sel.ResetParameters4Bweight (1, sel.GetMethodb(), 0); // back to normal!
     }
}

int main (int argc, char *argv[])
{
  clock_t start = clock();

  //partial time during loop over events
  clock_t tl_syst = 0;
  clock_t tl_tableDATA = 0;
  clock_t tl_tableMC = 0;
  clock_t tl_DY = 0;
  clock_t time;
 
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

  int AnaStep = 6;//which defines the cuts that the events should pass to be considered as selected

  float Nobs_ee = 0.;
  float Nobs_emu = 0.;
  float Nobs_mumu = 0.;
  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName;
  cout<<"argc "<<argc<<" "<<argv[0]<<endl;
  if (argc>1 ) xmlFileName = string(argv[1]);
  else xmlFileName = string ("../../config/DiLeptonXsectionMeasurement.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets);	// now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel);	// now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo (DataType, Luminosity, LumiError, verbosity);
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  int flagOriginal=sel.GetFlagb();
  int methodOriginal=sel.GetMethodb();
  int systOriginal= sel.GetSystb();
  std::cout << " For btag : flag " << flagOriginal << ", method " << methodOriginal << ", syst " << systOriginal << std::endl;
  TopTree::NTEvent * event = 0;
  //Selection table
  // 4 tables: ee - emu - mumu - allChannels  [FOR MC]
  SelectionTable selTable_allChannels (sel.GetCutList (), datasets, string ("*"));
  SelectionTable selTable_ee (sel.GetCutList (), datasets, string ("ee"));
  SelectionTable selTable_emu (sel.GetCutList (), datasets, string ("emu"));
  SelectionTable selTable_mumu (sel.GetCutList (), datasets, string ("mumu"));
  
  // 4 tables: ee - emu - mumu - allChannels  [FOR DATA]
  SelectionTable selDATATable_allChannels (sel.GetCutList (), datasets, string ("*"));
  SelectionTable selDATATable_ee (sel.GetCutList (), datasets, string ("ee"));
  SelectionTable selDATATable_emu (sel.GetCutList (), datasets, string ("emu"));
  SelectionTable selDATATable_mumu (sel.GetCutList (), datasets, string ("mumu"));

  //Book keeping of standard histos
  bool doHistoManager = false;
  DiLepAnaHistoManager histoManager;
  if(doHistoManager){
  	histoManager.LoadDatasets (datasets);
  	histoManager.LoadSelectionSteps (sel.GetCutList ());
  	histoManager.LoadChannels (sel.GetChannelList ());
  	histoManager.CreateHistos ();
  }
  //////////////////////


  //Background estimation methods
  DYEstimation DYest;
  //the following objects has to be internalized in the class itself
  std::vector < TH1F * >hlistemu;
  std::vector < TH1F * >hlistSR;
  std::vector < TH1F * >hlistCR;
  TH1F *halllistemu = new TH1F ("halllistemu", "halllistemu", 100, 0., 350.);
  TH1F *halllistSR = new TH1F ("halllistSR", "halllistSR", 100, 0., 350.);
  TH1F *halllistCR = new TH1F ("halllistCR", "halllistCR", 100, 0., 350.);
  //////////////////////

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
  //Jet Energy Scale JES
  Syst SysJES;
  //Jet Energy Resolution JES
  Syst SysJER;
  //Unclustered Missing Energy Scale METS
  Syst SysMETS;
  //Electron Energy Scale EES
  Syst SysEES;
  //Muon Energy Scale MES
  Syst SysMES;
  //b-tagging
  Syst SysBtag;
  SysBtag.mean = 1;
  SysBtag.p1sigma = 1.1;
  SysBtag.m1sigma = 0.9;
  if (verbosity > 2) {
    cout << " ######################################" << endl;
    cout << "	Systematics		       " << endl;
    cout << " ######################################" << endl;
  }
  anaEL.LoadSystematics (string ("JES"), SysJES);
  anaEL.LoadSystematics (string ("JER"), SysJER);
  anaEL.LoadSystematics (string ("METS"), SysMETS);
  anaEL.LoadSystematics (string ("EES"), SysEES);
  anaEL.LoadSystematics (string ("MES"), SysMES);
  if (verbosity > 2){
    cout << " - JES: " << SysJES.m1sigma << " - " << SysJES.mean << " - " << SysJES.p1sigma << endl;
    cout << " - JER: " << SysJER.m1sigma << " - " << SysJER.mean << " - " << SysJER.p1sigma << endl;
    cout << " - METS: " << SysMETS.m1sigma << " - " << SysMETS.mean << " - " << SysMETS.p1sigma << endl;
    cout << " - EES: " << SysEES.m1sigma << " - " << SysEES.mean << " - " << SysEES.p1sigma << endl;
    cout << " - MES: " << SysMES.m1sigma << " - " << SysMES.mean << " - " << SysMES.p1sigma << endl;
    cout << " ######################################" << endl;
  }
  if (verbosity > 2) {
    cout << " ######################################" << endl;
    cout << "	Systematics 'effect'	       " << endl;
    cout << " ######################################" << endl;
  }
  
  //0: ee - 1: emu - 2: mumu 
  SystEffect SysLEF[3]; 
  SystEffect SysTEF[3]; 
  SystEffect SysBR[3]; 
  SystEffect SysPDF[3]; 
  SystEffect SysQ2[3]; 
  SystEffect SysMEPSThr[3]; 
  SystEffect SysISRFSR[3]; 
  SystEffect SysPU[3]; 
  vector<string> channels_;
  channels_.push_back(string("ee"));
  channels_.push_back(string("emu"));
  channels_.push_back(string("mumu"));
  //Add SystematicsEffect: plug directly numbers !
  for(int i=0;i<3;i++){
  	anaEL.LoadSystematicsEffect(string("LEF"), channels_[i], SysLEF[i]);
  	anaEL.LoadSystematicsEffect(string("TEF"), channels_[i], SysTEF[i]);
  	anaEL.LoadSystematicsEffect(string("BR"), channels_[i], SysBR[i]);
  	anaEL.LoadSystematicsEffect(string("PDF"), channels_[i], SysPDF[i]);
  	anaEL.LoadSystematicsEffect(string("Q2"), channels_[i], SysQ2[i]);
  	anaEL.LoadSystematicsEffect(string("ME-PS-Thr"), channels_[i], SysMEPSThr[i]);
  	anaEL.LoadSystematicsEffect(string("ISR/FSR"), channels_[i], SysISRFSR[i]);
  	anaEL.LoadSystematicsEffect(string("PU"), channels_[i], SysPU[i]);
  	if (verbosity > 2){
		cout << " ############ Channel "<<channels_[i]<<"       ############"<<endl;
    		cout << " - LEF: " << SysLEF[i].m1sigma << " - " << SysLEF[i].mean << " - " << SysLEF[i].p1sigma << " Eff:  " << SysLEF[i].effm1s << " - " << SysLEF[i].effmean << " - "<< SysLEF[i].effp1s << endl;
    		cout << " - TEF: " << SysTEF[i].m1sigma << " - " << SysTEF[i].mean << " - " << SysTEF[i].p1sigma << " Eff:  " << SysTEF[i].effm1s << " - " << SysTEF[i].effmean << " - "<< SysTEF[i].effp1s << endl;
    		cout << " - BR: " << SysBR[i].m1sigma << " - " << SysBR[i].mean << " - " << SysBR[i].p1sigma << " Eff:  " << SysBR[i].effm1s << " - " << SysBR[i].effmean << " - "<< SysBR[i].effp1s << endl;
    		cout << " - PDF: " << SysPDF[i].m1sigma << " - " << SysPDF[i].mean << " - " << SysPDF[i].p1sigma << " Eff:  " << SysPDF[i].effm1s << " - " << SysPDF[i].effmean << " - "<< SysPDF[i].effp1s << endl;
    		cout << " - Q2: " << SysQ2[i].m1sigma << " - " << SysQ2[i].mean << " - " << SysQ2[i].p1sigma << " Eff:  " << SysQ2[i].effm1s << " - " << SysQ2[i].effmean << " - "<< SysQ2[i].effp1s << endl;
    		cout << " - MEPSThr: " << SysMEPSThr[i].m1sigma << " - " << SysMEPSThr[i].mean << " - " << SysMEPSThr[i].p1sigma << " Eff:  " << SysMEPSThr[i].effm1s << " - " << SysMEPSThr[i].effmean << " - "<< SysMEPSThr[i].effp1s << endl;
    		cout << " - ISRFSR: " << SysISRFSR[i].m1sigma << " - " << SysISRFSR[i].mean << " - " << SysISRFSR[i].p1sigma << " Eff:  " << SysISRFSR[i].effm1s << " - " << SysISRFSR[i].effmean << " - "<< SysISRFSR[i].effp1s << endl;
    		cout << " - PU: " << SysPU[i].m1sigma << " - " << SysPU[i].mean << " - " << SysPU[i].p1sigma << " Eff:  " << SysPU[i].effm1s << " - " << SysPU[i].effmean << " - "<< SysPU[i].effp1s << endl;
   	 	cout << " ######################################" << endl;
 	}
  }

  //////////////
  //channel ee
  //////////////
  cout<<"BR = "<<xec_ee.BR<<endl; 
  DiLepXsectionMeas meas_ee (string("ee"), string ("TTbarEE"), xec_ee.BR);
  if(xec_ee.doIt){
  	//float BRee = 0.016;
 	//float BRee = 1.; // has to be loaded from the xml file - should then be a systematic also .. (error on BR)
  	//Luminosity has to be provided before loading the datasets !! It's important for the weight computation
  	meas_ee.SetLumi (Luminosity, LumiError);
  	meas_ee.LoadDatasets (datasets);
  	if(SysJES.doIt) meas_ee.AddUncertainty (string ("JES"), SysJES);
  	if(SysJER.doIt) meas_ee.AddUncertainty (string ("JER"), SysJER);
  	if(SysMETS.doIt) meas_ee.AddUncertainty (string ("METS"), SysMETS);
 	meas_ee.AddUncertainty (string ("btagEff"), 1., 0.9, 1.1);//The first value must be one (in fact the global SF derived from the btag SF)!
 	meas_ee.AddUncertainty (string ("btagFake"), 1., 0.9, 1.1);//The first value must be one (in fact the global SF derived from the btag SF)!
  	if(SysEES.doIt) meas_ee.AddUncertainty (string ("EES"), SysEES);
  	if(SysLEF[0].doIt) meas_ee.AddUncertainty(string("LEF"),SysLEF[0]);
  	if(SysTEF[0].doIt) meas_ee.AddUncertainty(string("TEF"),SysTEF[0]);
  	if(SysBR[0].doIt) meas_ee.AddUncertainty(string("BR"),SysBR[0]);
  	if(SysPDF[0].doIt) meas_ee.AddUncertainty(string("PDF"),SysPDF[0]);
  	if(SysQ2[0].doIt) meas_ee.AddUncertainty(string("Q2"),SysQ2[0]);
  	if(SysMEPSThr[0].doIt) meas_ee.AddUncertainty(string("MEPSThr"),SysMEPSThr[0]);
  	if(SysISRFSR[0].doIt) meas_ee.AddUncertainty(string("ISRFSR"),SysISRFSR[0]);
  	if(SysPU[0].doIt) meas_ee.AddUncertainty(string("PU"),SysPU[0]);
  }

  //////////////
  //channel mumu
  //////////////
  
  DiLepXsectionMeas meas_mumu (string("mumu"), string ("TTbarMM"), xec_mumu.BR);
  if(xec_mumu.doIt){
  	//float BRee = 0.016;
 	//float BRee = 1.; // has to be loaded from the xml file - should then be a systematic also .. (error on BR)
  	//Luminosity has to be provided before loading the datasets !! It's important for the weight computation
  	meas_mumu.SetLumi (Luminosity, LumiError);
  	meas_mumu.LoadDatasets (datasets);
  	if(SysJES.doIt) meas_mumu.AddUncertainty (string ("JES"), SysJES);
  	if(SysJER.doIt) meas_mumu.AddUncertainty (string ("JER"), SysJER);
  	if(SysMETS.doIt) meas_mumu.AddUncertainty (string ("METS"), SysMETS);
 	meas_mumu.AddUncertainty (string ("btagEff"), 1., 0.9, 1.1);//The first value must be one (in fact the global SF derived from the btag SF)!
 	meas_mumu.AddUncertainty (string ("btagFake"), 1., 0.9, 1.1);//The first value must be one (in fact the global SF derived from the btag SF)!
  	if(SysMES.doIt) meas_mumu.AddUncertainty (string ("MES"), SysMES);
  	if(SysLEF[1].doIt) meas_mumu.AddUncertainty(string("LEF"),SysLEF[1]);
  	if(SysTEF[1].doIt) meas_mumu.AddUncertainty(string("TEF"),SysTEF[1]);
  	if(SysBR[1].doIt) meas_mumu.AddUncertainty(string("BR"),SysBR[1]);
  	if(SysPDF[1].doIt) meas_mumu.AddUncertainty(string("PDF"),SysPDF[1]);
  	if(SysQ2[1].doIt) meas_mumu.AddUncertainty(string("Q2"),SysQ2[1]);
  	if(SysMEPSThr[1].doIt) meas_mumu.AddUncertainty(string("MEPSThr"),SysMEPSThr[1]);
  	if(SysISRFSR[1].doIt) meas_mumu.AddUncertainty(string("ISRFSR"),SysISRFSR[1]);
  	if(SysPU[1].doIt) meas_mumu.AddUncertainty(string("PU"),SysPU[1]);
  }

  //////////////
  //channel ee
  //////////////
  
  DiLepXsectionMeas meas_emu (string("emu"), string ("TTbarEM"), xec_emu.BR);
  if(xec_ee.doIt){
  	//float BRee = 0.016;
 	//float BRee = 1.; // has to be loaded from the xml file - should then be a systematic also .. (error on BR)
  	//Luminosity has to be provided before loading the datasets !! It's important for the weight computation
  	meas_emu.SetLumi (Luminosity, LumiError);
  	meas_emu.LoadDatasets (datasets);
  	if(SysJES.doIt) meas_emu.AddUncertainty (string ("JES"), SysJES);
  	if(SysJER.doIt) meas_emu.AddUncertainty (string ("JER"), SysJER);
  	if(SysMETS.doIt) meas_emu.AddUncertainty (string ("METS"), SysMETS);
 	meas_emu.AddUncertainty (string ("btagEff"), 1., 0.9, 1.1);//The first value must be one (in fact the global SF derived from the btag SF)!
 	meas_emu.AddUncertainty (string ("btagFake"), 1., 0.9, 1.1);//The first value must be one (in fact the global SF derived from the btag SF)!
  	if(SysEES.doIt) meas_emu.AddUncertainty (string ("EES"), SysEES);
  	if(SysMES.doIt) meas_emu.AddUncertainty (string ("MES"), SysMES);
  	if(SysLEF[2].doIt) meas_emu.AddUncertainty(string("LEF"),SysLEF[2]);
  	if(SysTEF[2].doIt) meas_emu.AddUncertainty(string("TEF"),SysTEF[2]);
  	if(SysBR[2].doIt) meas_emu.AddUncertainty(string("BR"),SysBR[2]);
  	if(SysPDF[2].doIt) meas_emu.AddUncertainty(string("PDF"),SysPDF[2]);
  	if(SysQ2[2].doIt) meas_emu.AddUncertainty(string("Q2"),SysQ2[2]);
  	if(SysMEPSThr[2].doIt) meas_emu.AddUncertainty(string("MEPSThr"),SysMEPSThr[2]);
  	if(SysISRFSR[2].doIt) meas_emu.AddUncertainty(string("ISRFSR"),SysISRFSR[2]);
  	if(SysPU[2].doIt) meas_emu.AddUncertainty(string("PU"),SysPU[2]);
  }

  cout << "The verbosity mode is " << verbosity << endl;
  cout << "The luminosity is equal to " << Luminosity << endl;
  cout << "The DataType is ";
  switch (DataType) {
  case 0:
    cout << "MC" << endl;
    break;
  case 1:
    cout << "Data" << endl;
    break;
  case 2:
    cout << "Data & MC" << endl;
    break;
  default:
    cout << " unknown" << endl;
    break;
  }
  //////////////////////

  clock_t t_loop = clock();


  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Loop over the datasets  " << endl;
    cout << "#########################" << endl;
  }

  for (unsigned int d = 0; d < datasets.size (); d++) {

    if(verbosity>2) cout<<"Dataset: "<<datasets[d].Name()<<endl;
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent", &event);

    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;
    cout <<" NEvents to run over = "<<datasets[d].NofEvtsToRunOver()<<endl;

    //temporary
    hlistemu = DYest.CreateDYHisto ("emu", datasets[d].Name (), 100, 0., 350.);
    hlistSR = DYest.CreateDYHisto ("SR", datasets[d].Name (), 100, 0., 350.);
    hlistCR = DYest.CreateDYHisto ("CR", datasets[d].Name (), 100, 0., 350.);

    //LOOP OVER THE EVENTS
    for (unsigned int ievt = 0; ievt < datasets[d].NofEvtsToRunOver(); ievt++) {
      float weight = 1.;
      if(datasets[d].isData() == false) weight = datasets[d].NormFactor()*Luminosity; //if Data , weight = 1
      //cout<<"weight "<<weight<<" "<<datasets[d].isData()<<endl;
      datasets[d].eventTree ()->GetEntry (ievt);
      if (verbosity > 3){
	cout << "event " << ievt <<" - event number=" << event->eventNb <<" - run number=" << event->runNb << endl;
      }
      if (ievt % 1000 == 0)
	      cout << "number of events " << ievt << endl;

      //Load event for the selection
      sel.LoadEvent (event);

      //Collection of selected objects
      vector < NTElectron > selElectrons = sel.GetSelectedElectrons ();
      vector < NTMuon > selMuons = sel.GetSelectedMuons ();
      vector < NTJet > selJets = sel.GetSelectedJets ();
      NTMET met = sel.GetMET ();	// no criteria applyied

      //Candidate pair of lepton
      string CandType;		// ee - emu - mumum or false
      vector < NTElectron > candElec;
      vector < NTMuon > candMuon;
      sel.GetLeptonPair (candMuon, candElec, CandType);	// fill the variables

      //integer which define the last step of the selection that the event fullfill
      int selLastStep = 0;
      /*
         Steps of the selection: (integer returned by doFullSelection() or FillTable(...))
         - Step 1        Dilepton pair choice
         - Step 2        Z mass veto 
         - Step 3        Minimal jet multiplicity 
         - Step 4        MET cuts
         - Step 5        btagging cuts
       */


      //Define weight for b-tag
      vector<float> weightb;
      weightb.push_back(weight);  // weight of the event
      weightb.push_back(0.);      // Proba of 0 jet
      weightb.push_back(0.);      // Proba of 1 jet;
      weightb.push_back(0.);      // Proba of 2 jets;
      weightb.push_back(0.);      // Proba of at least 3 jets;

      vector<float> weight_keep; // save value
      weight_keep.push_back(0.);
      weight_keep.push_back(0.);
      weight_keep.push_back(0.);

      //////////////////////////////////   
      //   Fill the selection table
      //////////////////////////////////   


      //Fill the selection table
      // with correct b-tag weight! (Caro)
     
      if(datasets[d].isData()){
	 time = clock(); 
	 selLastStep = sel.FillTable (selDATATable_ee, &(datasets[d]), d, weight);
	 tl_tableDATA+=clock()-time;
      }
      else{
	 time = clock(); 
	 selLastStep = sel.FillTablewBweight(selTable_ee, &(datasets[d]), d, weight, weightb);
	 tl_tableMC+=clock()-time;
      }
      //cout<<"selLastStep = "<<selLastStep<<endl;
      if (CandType=="ee") {
       // Fill the histo here, as objects ForAna are dependent on the channel,
//       histoManager.Fill(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight);
	 weight = weight*0.908;
         if(doHistoManager) histoManager.FillwBweight(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight, weightb);
         weight_keep[0]=weightb[0];
      }
      if (selLastStep >= AnaStep && datasets[d].isData () && (DataType == 1 || DataType == 2))
	Nobs_ee+=weightb[0];
      if (selLastStep >= AnaStep && DataType == 0)
	Nobs_ee+=weightb[0];
      if (selLastStep >= AnaStep)
	if(xec_ee.doIt) meas_ee.PassTheSelection (d,weightb[0]);
      //}

      if(datasets[d].isData()){
	 time = clock(); 
	 selLastStep = sel.FillTable (selDATATable_emu, &(datasets[d]), d, weight);
	 tl_tableDATA=clock()-time;
      }
      else{
	 time = clock(); 
	 selLastStep = sel.FillTablewBweight(selTable_emu, &(datasets[d]), d, weight, weightb);
	 tl_tableMC+=clock()-time;
      }
      if (CandType=="emu") {
//       histoManager.Fill(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight);
         if(doHistoManager) histoManager.FillwBweight(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight, weightb);
         weight_keep[1]=weightb[0];
      if (selLastStep >= AnaStep && datasets[d].isData () && (DataType == 1 || DataType == 2))
	Nobs_emu+=weightb[0];
      if (selLastStep >= AnaStep && DataType == 0)
	Nobs_emu+=weightb[0];
      if (selLastStep >= AnaStep)
	if(xec_emu.doIt) meas_emu.PassTheSelection (d,weightb[0]);
      }

      if(datasets[d].isData()){
	 time = clock(); 
	 selLastStep = sel.FillTable (selDATATable_mumu, &(datasets[d]), d, weight);
	 tl_tableDATA+=clock()-time;
      }
      else{
	 time = clock(); 
	 selLastStep = sel.FillTablewBweight(selTable_mumu, &(datasets[d]), d, weight, weightb);
	 tl_tableMC+=clock()-time;
      }
      if (CandType=="mumu") {
//       histoManager.Fill(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight);
	 weight = weight*0.953;
         if(doHistoManager) histoManager.FillwBweight(sel, event, candMuon, candElec, selLastStep, sel.GetChannel(CandType), d, weight, weightb);
         weight_keep[2]=weightb[0];
      if (selLastStep >= AnaStep && datasets[d].isData () && (DataType == 1 || DataType == 2))
	Nobs_mumu+=weightb[0];
      if (selLastStep >= AnaStep && DataType == 0)
	Nobs_mumu+=weightb[0];
      if (selLastStep >= AnaStep)
	if(xec_mumu.doIt) meas_mumu.PassTheSelection (d,weightb[0]);
      }
//     selLastStep = sel.FillTable(selTable_allChannels, &(datasets[d]), d, weight);


      //////////////////////////////////   
      //	DYEstimation
      //////////////////////////////////  
      time = clock(); 
      float Dweight = Luminosity * datasets[d].Xsection () / datasets[d].NofEvtsToRunOver ();
      if (datasets[d].Name () == "Data" || datasets[d].Name () == "DATA")
	Dweight = 1;
      if (CandType == string ("emu"))
	DYest.FillDYHistos ("emu", hlistemu, datasets[d].Name (), event->TMEME, sel.DiLeptonMass (candMuon, candElec), Dweight);
      if (CandType == string ("mumu") && selJets.size () > 1)
	DYest.FillDYHistos ("SR", hlistSR, datasets[d].Name (), event->TMEME, sel.DiLeptonMass (candMuon, candElec), Dweight);
      if (CandType == string ("mumu") && selJets.size () <= 1)
	DYest.FillDYHistos ("CR", hlistCR, datasets[d].Name (), event->TMEME, sel.DiLeptonMass (candMuon, candElec), Dweight);
      bool isinSR = false;
      bool isinCR = false;
      if (CandType == string ("mumu") && selJets.size () > 1)
	isinSR = true;
      if (CandType == string ("mumu") && selJets.size () <= 1)
	isinCR = true;
      //DYest.FillNbofEventsInSRandCR(isinCR,isinSR,sel.DiLeptonMass(candMuon,candElec),datasets[d].Name());
      DYest.FillNbofEventsInSRandCR (isinCR, isinSR, sel.DiLeptonMass (candMuon, candElec), datasets[d].Name (), Dweight);
      tl_DY+=clock()-time;
      ///////////////////////////     


      ///////////////////////////////////////
      //	Fill the systematic part
      ///////////////////////////////////////

      // For ee channel

      time = clock();
      if(xec_ee.doIt){
      	// Systematics which need the computation of the btag weight :
      	// JES : impact on selected jets
      	sel.ResetParameters4Bweight (1, sel.GetMethodb(), sel.GetSystb());
      	if(SysJES.doIt) ComputeSystematic(string("ee"), datasets[d], d, sel, meas_ee, AnaStep, weight, string("JES"), SysJES);
      	// JER : impact on selected jets
      	sel.ResetParameters4Bweight (1, sel.GetMethodb(), sel.GetSystb());
      	if(SysJER.doIt) ComputeSystematic(string("ee"), datasets[d], d, sel, meas_ee, AnaStep, weight, string("JER"), SysJER);
        // METS
      	if(SysMETS.doIt) ComputeSystematic(string("ee"), datasets[d], d, sel, meas_ee, AnaStep, weight, string("METS"), SysMETS);
      	// Btag
      	ComputeSystematic(string("ee"), datasets[d], d, sel, meas_ee, AnaStep, weight, string("btagEff"), SysBtag);
      	ComputeSystematic(string("ee"), datasets[d], d, sel, meas_ee, AnaStep, weight, string("btagFake"), SysBtag);

      	// Systematics which do not depend on jets or b-tag : no computation of btag weight!
      	// EES
      	sel.ResetParameters4Bweight (0, sel.GetMethodb(), sel.GetSystb()); // switch of the computation
      	float btagw= weight;                                             // use the previous weight
      	if (CandType=="ee")        { btagw = weight_keep[0];}
      	else if (CandType=="emu")  { btagw = weight_keep[1];}
      	else if (CandType=="mumu") { btagw = weight_keep[2];}
      	if(SysEES.doIt) ComputeSystematic(string("ee"), datasets[d], d, sel, meas_ee, AnaStep, btagw, string("EES"), SysEES);

      	sel.ResetParameters4Bweight (flagOriginal, methodOriginal, systOriginal); // back to normal!
      }

      if(xec_emu.doIt){
      	// Systematics which nemud the computation of the btag weight :
      	// JES : impact on selected jets
      	sel.ResetParameters4Bweight (1, sel.GetMethodb(), sel.GetSystb());
      	if(SysJES.doIt) ComputeSystematic(string("emu"), datasets[d], d, sel, meas_emu, AnaStep, weight, string("JES"), SysJES);
      	// JER : impact on selected jets
      	sel.ResetParameters4Bweight (1, sel.GetMethodb(), sel.GetSystb());
      	if(SysJER.doIt) ComputeSystematic(string("emu"), datasets[d], d, sel, meas_emu, AnaStep, weight, string("JER"), SysJER);
        // METS
      	if(SysMETS.doIt) ComputeSystematic(string("emu"), datasets[d], d, sel, meas_emu, AnaStep, weight, string("METS"), SysMETS);
      	// Btag
      	ComputeSystematic(string("emu"), datasets[d], d, sel, meas_emu, AnaStep, weight, string("btagEff"), SysBtag);
      	ComputeSystematic(string("emu"), datasets[d], d, sel, meas_emu, AnaStep, weight, string("btagFake"), SysBtag);

      	// Systematics which do not depend on jets or b-tag : no computation of btag weight!
      	// EES & MES
      	sel.ResetParameters4Bweight (0, sel.GetMethodb(), sel.GetSystb()); // switch of the computation
      	float btagw= weight;                                             // use the previous weight
      	if (CandType=="emu")        { btagw = weight_keep[0];}
      	else if (CandType=="emu")  { btagw = weight_keep[1];}
      	else if (CandType=="mumu") { btagw = weight_keep[2];}
      	if(SysEES.doIt) ComputeSystematic(string("emu"), datasets[d], d, sel, meas_emu, AnaStep, weight, string("EES"), SysEES);
      	if(SysMES.doIt) ComputeSystematic(string("emu"), datasets[d], d, sel, meas_emu, AnaStep, weight, string("MES"), SysMES);

      	sel.ResetParameters4Bweight (flagOriginal, methodOriginal, systOriginal); // back to normal!
      }

      if(xec_mumu.doIt){
      	// Systematics which nmumud the computation of the btag weight :
      	// JES : impact on selected jets
      	sel.ResetParameters4Bweight (1, sel.GetMethodb(), sel.GetSystb());
      	if(SysJES.doIt) ComputeSystematic(string("mumu"), datasets[d], d, sel, meas_mumu, AnaStep, weight, string("JES"), SysJES);
      	// JER : impact on selected jets
      	sel.ResetParameters4Bweight (1, sel.GetMethodb(), sel.GetSystb());
      	if(SysJER.doIt) ComputeSystematic(string("mumu"), datasets[d], d, sel, meas_mumu, AnaStep, weight, string("JER"), SysJER);
        // METS
      	if(SysMETS.doIt) ComputeSystematic(string("mumu"), datasets[d], d, sel, meas_mumu, AnaStep, weight, string("METS"), SysMETS);
      	// Btag
      	ComputeSystematic(string("mumu"), datasets[d], d, sel, meas_mumu, AnaStep, weight, string("btagEff"), SysBtag);
      	ComputeSystematic(string("mumu"), datasets[d], d, sel, meas_mumu, AnaStep, weight, string("btagFake"), SysBtag);

      	// Systematics which do not depend on jets or b-tag : no computation of btag weight!
      	// MES
      	sel.ResetParameters4Bweight (0, sel.GetMethodb(), sel.GetSystb()); // switch of the computation
      	float btagw= weight;                                             // use the previous weight
      	if (CandType=="mumu")        { btagw = weight_keep[0];}
      	else if (CandType=="emu")  { btagw = weight_keep[1];}
      	else if (CandType=="mumu") { btagw = weight_keep[2];}
      	if(SysMES.doIt) ComputeSystematic(string("mumu"), datasets[d], d, sel, meas_mumu, AnaStep, weight, string("MES"), SysMES);

      	sel.ResetParameters4Bweight (flagOriginal, methodOriginal, systOriginal); // back to normal!
      }
      tl_syst+=clock()-time;

    }				// end of loop over evts


  }				// end of loop over the datasets 
  cout << "#########################" << endl;
  cout << " Loop over the datasets  " << endl;
  cout << "#########################" << endl;

  clock_t t_endloop = clock();


  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

  ////////////////////////////
  //  Xsection extraction
  ////////////////////////////

  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Cross-section extraction" << endl;
    cout << "#########################" << endl;
  }
  cout << "Nobs: " << Nobs_ee << endl;
  bool doMCPseudoExp = false;
  int NofPseudoExp = 1;
  //if(DataType == 0) doMCPseudoExp = true; 
  
  //variables for PLR plots
  int Nbins_PLR = 290;
  float PLR_min = 10;
  float PLR_max = 300;
  int verboseXEx = 1; // I don't know yet why but has to be equal to 1 !!!

  if(xec_ee.doIt){
	if(verbosity>1){
		cout<<"####  Channel ee     ####"<<endl;
	}
  	meas_ee.Compute ();
  	if(verbosity>1 ) meas_ee.PrintSelEff();
  	//Add the DDEstimation
  	if(xec_ee.NEvtsWjets>=0){
		if(verbosity>1) cout<<"DD for Wjets: "<<xec_ee.NEvtsWjets<<" - "<<xec_ee.NEvtsWjetsErrNeg<<" / + "<<xec_ee.NEvtsWjetsErrPos<<endl;
		meas_ee.BkgDataDrivenEstimation(string("Wjets"),xec_ee.NEvtsWjets,xec_ee.NEvtsWjetsErrNeg,xec_ee.NEvtsWjetsErrPos);
	}  	
	if(xec_ee.NEvtsQCD>=0){
		if(verbosity>1) cout<<"DD for QCD: "<<xec_ee.NEvtsQCD<<" - "<<xec_ee.NEvtsQCDErrNeg<<" / + "<<xec_ee.NEvtsQCDErrPos<<endl;
		 meas_ee.BkgDataDrivenEstimation(string("QCD"),xec_ee.NEvtsQCD,xec_ee.NEvtsQCDErrNeg,xec_ee.NEvtsQCDErrPos);
  	}
	if(xec_ee.NEvtsDY>=0){ 
		if(verbosity>1) cout<<"DD for DY: "<<xec_ee.NEvtsDY<<" - "<<xec_ee.NEvtsDYErrNeg<<" / + "<<xec_ee.NEvtsDYErrPos<<endl;
		meas_ee.BkgDataDrivenEstimation(string("DY"),xec_ee.NEvtsDY,xec_ee.NEvtsDYErrNeg,xec_ee.NEvtsDYErrPos);
  	}
	if(xec_ee.NEvtsData<0) meas_ee.XExtraction (Nobs_ee, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx);
	else meas_ee.XExtraction (xec_ee.NEvtsData, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx);
  }
  if(xec_emu.doIt){
	if(verbosity>1){
		cout<<"####  Channel emu     ####"<<endl;
	}
  	meas_emu.Compute ();
  	if(verbosity>1 ) meas_emu.PrintSelEff();
  	//Add the DDEstimation
  	if(xec_emu.NEvtsWjets>=0){
		if(verbosity>1) cout<<"DD for Wjets: "<<xec_emu.NEvtsWjets<<" - "<<xec_emu.NEvtsWjetsErrNeg<<" / + "<<xec_emu.NEvtsWjetsErrPos<<endl;
		meas_emu.BkgDataDrivenEstimation(string("Wjets"),xec_emu.NEvtsWjets,xec_emu.NEvtsWjetsErrNeg,xec_emu.NEvtsWjetsErrPos);
	}  	
	if(xec_emu.NEvtsQCD>=0){
		if(verbosity>1) cout<<"DD for QCD: "<<xec_emu.NEvtsQCD<<" - "<<xec_emu.NEvtsQCDErrNeg<<" / + "<<xec_emu.NEvtsQCDErrPos<<endl;
		 meas_emu.BkgDataDrivenEstimation(string("QCD"),xec_emu.NEvtsQCD,xec_emu.NEvtsQCDErrNeg,xec_emu.NEvtsQCDErrPos);
  	}
	if(xec_emu.NEvtsDY>=0){ 
		if(verbosity>1) cout<<"DD for DY: "<<xec_emu.NEvtsDY<<" - "<<xec_emu.NEvtsDYErrNeg<<" / + "<<xec_emu.NEvtsDYErrPos<<endl;
		meas_emu.BkgDataDrivenEstimation(string("DY"),xec_emu.NEvtsDY,xec_emu.NEvtsDYErrNeg,xec_emu.NEvtsDYErrPos);
  	}
	if(xec_emu.NEvtsData<0) meas_emu.XExtraction (Nobs_emu, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx);
	else meas_emu.XExtraction (xec_emu.NEvtsData, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx);
  }
  if(xec_mumu.doIt){
	if(verbosity>1){
		cout<<"####  Channel mumu     ####"<<endl;
	}
  	meas_mumu.Compute ();
  	if(verbosity>1 ) meas_mumu.PrintSelEff();
  	//Add the DDEstimation
  	if(xec_mumu.NEvtsWjets>=0){
		if(verbosity>1) cout<<"DD for Wjets: "<<xec_mumu.NEvtsWjets<<" - "<<xec_mumu.NEvtsWjetsErrNeg<<" / + "<<xec_mumu.NEvtsWjetsErrPos<<endl;
		meas_mumu.BkgDataDrivenEstimation(string("Wjets"),xec_mumu.NEvtsWjets,xec_mumu.NEvtsWjetsErrNeg,xec_mumu.NEvtsWjetsErrPos);
	}  	
	if(xec_mumu.NEvtsQCD>=0){
		if(verbosity>1) cout<<"DD for QCD: "<<xec_mumu.NEvtsQCD<<" - "<<xec_mumu.NEvtsQCDErrNeg<<" / + "<<xec_mumu.NEvtsQCDErrPos<<endl;
		 meas_mumu.BkgDataDrivenEstimation(string("QCD"),xec_mumu.NEvtsQCD,xec_mumu.NEvtsQCDErrNeg,xec_mumu.NEvtsQCDErrPos);
  	}
	if(xec_mumu.NEvtsDY>=0){ 
		if(verbosity>1) cout<<"DD for DY: "<<xec_mumu.NEvtsDY<<" - "<<xec_mumu.NEvtsDYErrNeg<<" / + "<<xec_mumu.NEvtsDYErrPos<<endl;
		meas_mumu.BkgDataDrivenEstimation(string("DY"),xec_mumu.NEvtsDY,xec_mumu.NEvtsDYErrNeg,xec_mumu.NEvtsDYErrPos);
  	}
	if(xec_mumu.NEvtsData<0) meas_mumu.XExtraction (Nobs_mumu, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx);
	else meas_mumu.XExtraction (xec_mumu.NEvtsData, doMCPseudoExp, NofPseudoExp, Nbins_PLR, PLR_min, PLR_max, verboseXEx);
  }

  clock_t t_XExtrac = clock();

  ////////////////////////////
  //  Histograms
  ////////////////////////////

  if(doHistoManager) histoManager.Compute ();

  ////////////////////////////
  //  DYEstimation
  ////////////////////////////

  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << "   DYEstimation         " << endl;
    cout << "#########################" << endl;
  }
  //DYest.MakeDYHistos("emu",hlistemu,halllistemu,hemudata);
  DYest.MakeDYHistos ("emu", hlistemu, halllistemu);
  DYest.MakeDYHistos ("SR", hlistSR, halllistSR);
  DYest.MakeDYHistos ("CR", hlistCR, halllistCR);
  //DYest.MakeDataHistos("SR",hlistSR,hDataSR);
  //DYest.MakeDataHistos("CR",hlistCR,hDataCR);

  // Fit the emu channel to parametrize the shape of the non-DY events
  vector < double >parafit;
  parafit = DYest.MakeDYFits_emu (halllistemu);

  // Fit the mumu or ee channel with the BW shape + parametrized shape of the non-DY events in the Signal Region for MC
  // Take into account a scaled fraction of non-DY events for systematics
  std::cout << "Appel a MakeDYFits_SR" << std::endl;
  DYest.MakeDYFits_SR (halllistSR, parafit, 0, 0, "MC", 1.);

  // Fit the mumu or ee channel with the BW shape in the Control Region for MC 
  std::cout << "Appel a MakeDYFits_CR" << std::endl;
  DYest.MakeDYFits_CR (halllistCR, 0, 0, "MC");

  std::cout << "appel a ComputeDYEstimatedEvents_MC" << std::endl;
  DYest.ComputeDYEstimatedEvents_MC ();
  std::cout << "Number of MC DY events in Z peak (for CR) = " << DYest.GetNDYMCinMz_CR () << std::endl;
  std::cout << "Number of MC DY events in Z peak (for CR) = " << DYest.GetNDYMCinMz_SR () << std::endl;
  std::cout << "Number of MC DY events outside the Z peak (for CR) = " << DYest.GetNDYMC_CR () << std::endl;

  // MC comparison between real and expected yields
  std::cout << "Number of MC DY events outside the Z peak (for SR) = " << DYest.GetNDYMC_SR () << std::endl;
  std::cout << "Number of ESTIMATED MC DY events outside the Z peak (for SR) = " << DYest.GetNDYEstimatedMC () << std::endl;

  // Computation for Data
  // Fit the mumu or ee channel with the BW shape + parametrized MC shape of the non-DY events in the Signal Region 
  // Take into account a scaled fraction of non-DY events for systematics
  std::cout << "Appel a MakeDYFits_SR pour Data" << std::endl;
  // DYest.MakeDYFits_SR(hDataSR,parafit,0,0,"Data",1.);

  // Fit the mumu or ee channel with the BW shape in the Control Region    
  std::cout << "Appel a MakeDYFits_CR pour Data" << std::endl;
  //DYest.MakeDYFits_CR(hDataCR,0,0,"Data");

  // Compute the number of expected bckg according to 
  // "Counting" method (simply count the number of events in each zones) or from a "Fit" method of the inv.mass distribution (effect of others process bckg for DY can be taken into account)
  // "MC" or "DATA" origin for the scale factor (NofEventsintheMZpieak(inSR)/NofEventsintheMZpieak(inCR))
  //  DYest.ComputeDYEstimatedEvents_DATA("Counting","Data");
  DYest.ComputeDYEstimatedEvents_DATA ("Fit", "Data");
  std::cout << "Number of Data events outside the Z peak (for SR) = " << DYest.GetNDATA_SR () << std::endl;
  std::cout << "Number of ESTIMATED DY events outside the Z peak (for SR) = " << DYest.GetNDYEstimatedDATA () << std::endl;
  std::cout << "statistical error =  " << DYest.GetNDYEstimatedStatErrorDATA () << std::endl;
  std::cout << "to be compared with the number of MC DY events outside the Z peak (for SR) = " << DYest.GetNDYMC_SR () << std::endl;
  ///////////////

  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Fill the latex tables   " << endl;
    cout << "#########################" << endl;
  }

  string ofilename;
  if(argc>2) ofilename = argv[2];
  else ofilename = string("CrossSectionTable.tex"); 
  ofstream ofile (ofilename.c_str());
  ofile << "\\documentclass[8pt]{article}" << endl;
  ofile << "\\begin{document}" << endl;
  //Merge channels consistently
  vector < string > mergenames;
  mergenames.push_back ("TTbarEM");
  mergenames.push_back ("TTbarMM");
  mergenames.push_back ("TTbarOther");
  selTable_ee.MergeDatasets (mergenames, string ("TTbarOther"));
  mergenames.clear ();
  mergenames.push_back ("TTbarEE");
  mergenames.push_back ("TTbarMM");
  mergenames.push_back ("TTbarOther");
  selTable_emu.MergeDatasets (mergenames, string ("TTbarOther"));
  mergenames.clear ();
  mergenames.push_back ("TTbarEE");
  mergenames.push_back ("TTbarEM");
  mergenames.push_back ("TTbarOther");
  selTable_mumu.MergeDatasets (mergenames, string ("TTbarOther"));
  mergenames.clear ();
  mergenames.push_back ("TTbarEE");
  mergenames.push_back ("TTbarEM");
  mergenames.push_back ("TTbarMM");
  selTable_allChannels.MergeDatasets (mergenames, string ("TTbarSignal"));
  //Define signal
  selTable_ee.DefineFirstDataset (string ("TTbarEE"));
  selTable_emu.DefineFirstDataset (string ("TTbarEM"));
  selTable_mumu.DefineFirstDataset (string ("TTbarMM"));
  selTable_allChannels.DefineFirstDataset (string ("TTbarSignal"));
  //Calculations
	// For MC samples
  selTable_ee.TableCalculator ();
  selTable_emu.TableCalculator ();
  selTable_mumu.TableCalculator ();
  selTable_allChannels.TableCalculator ();
  	//For DATA
  selDATATable_ee.TableCalculator ();
  selDATATable_emu.TableCalculator ();
  selDATATable_mumu.TableCalculator ();
  selDATATable_allChannels.TableCalculator ();
  //Write
	//For MC samples
  selTable_ee.Write (ofile);
  selTable_emu.Write (ofile);
  selTable_mumu.Write (ofile);
  selTable_allChannels.Write (ofile);
  //Write
	//For DATA
  selDATATable_ee.Write (ofile);
  selDATATable_emu.Write (ofile);
  selDATATable_mumu.Write (ofile);
  selDATATable_allChannels.Write (ofile);

  //Add XSectionMeas
  if(xec_ee.doIt){
  	meas_ee.EfficienciesTable (ofile);
  	meas_ee.UncertaintiesTable (ofile);
  	meas_ee.MinimizationTable (ofile);
  }
  if(xec_emu.doIt){
  	meas_emu.EfficienciesTable (ofile);
  	meas_emu.UncertaintiesTable (ofile);
  	meas_emu.MinimizationTable (ofile);
  }
  if(xec_mumu.doIt){
  	meas_mumu.EfficienciesTable (ofile);
  	meas_mumu.UncertaintiesTable (ofile);
  	meas_mumu.MinimizationTable (ofile);
  }

  ofile << "\\end{document}" << endl;
  system ("pdflatex CrossSectionTable.tex");


  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << " Write output root file " << endl;
    cout << "#########################" << endl;
  }
  TFile *fout = new TFile ("DiLepXsectionMeas.root", "RECREATE");
  if(doHistoManager) histoManager.Write (fout);
  if(xec_ee.doIt) meas_ee.Write (fout);
  if(xec_emu.doIt) meas_emu.Write (fout);
  if(xec_mumu.doIt) meas_mumu.Write (fout);
  //fout->Write();
  fout->Close ();

  //Clear histos before deleting the TFile
  if(doHistoManager) histoManager.Clear ();

  delete fout;

  clock_t t_end = clock();

  if (verbosity > 0){
	cout<<"  Time summary " <<endl;
	cout<<"   - Initialisation  = "<< (t_loop-start)/CLOCKS_PER_SEC<<" sec "<<endl;
	cout<<"   - Loops dset/evts = "<< (t_endloop-t_loop)/CLOCKS_PER_SEC<<" sec "<<endl;
	cout<<"              Intermediate times: "<<endl;
	cout<<"			- Table for Data: "<<tl_tableDATA/CLOCKS_PER_SEC<<endl; 
	cout<<"			- Table for MC: "<<tl_tableMC/CLOCKS_PER_SEC<<endl; 
	cout<<"			- DY estimation: "<<tl_DY/CLOCKS_PER_SEC<<endl; 
	cout<<"			- Systematics: "<<tl_syst/CLOCKS_PER_SEC<<endl; 
	cout<<"   - XExtraction     = "<< (t_XExtrac-t_endloop)/CLOCKS_PER_SEC<<" sec "<<endl;
	cout<<"   - Writting files  = "<< (t_end-t_XExtrac)/CLOCKS_PER_SEC<<" sec "<<endl;
  }

  if (verbosity > 0) {
    cout << "#########################" << endl;
    cout << "    End of the program   " << endl;
    cout << "#########################" << endl;
  }

  return (0);
}
