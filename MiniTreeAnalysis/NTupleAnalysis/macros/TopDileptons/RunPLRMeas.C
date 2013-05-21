
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
#include "../../Measurements/interface/PLRMeasurement.h"


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
  float Lumi = 0;
  float LumiError = 0.;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0;
  int verbosity = 2;
  int doPseudoExp = 0;
  int NofPseudoExp = 0;
  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  string xmlFileName;
  cout<<"argc "<<argc<<" "<<argv[0]<<endl;
  if (argc>1 ) xmlFileName = string(argv[1]);
  else xmlFileName = string ("../../config/PLRMeas.xml");

  AnalysisEnvironmentLoader anaEL (xmlFileName);
  if(verbosity>1) cout<<" - Loading datasets ..."<<endl;
  anaEL.LoadSamples (datasets);	// now the list of datasets written in the xml file is known
  if(verbosity>1) cout<<" - Loading general info ..."<<endl;
  anaEL.LoadGeneralInfo (DataType, Lumi, LumiError, verbosity);

  if(verbosity>1) cout<<" - Initializing PLR ..."<<endl;
  PLRMeasurement plr;
  cout<<"a"<<endl;
  plr.LoadDatasets(datasets);
  cout<<"a"<<endl;
  plr.SetLumi(Lumi,LumiError);
  cout<<"a"<<endl;
  //Load additionnal spectific info
  if(verbosity>1) cout<<" - Loading PLRInformation ..."<<endl;
  anaEL.LoadPLRInformation(plr,doPseudoExp, NofPseudoExp);
  if(verbosity>1) if(doPseudoExp) cout<<"Wil perform "<<NofPseudoExp<<" pseudo experiments "<<endl;
  if(verbosity>1) cout<<" - Loading bkg info ..."<<endl;
  anaEL.LoadBgkInformation(plr);
  //Do this after loading the other info (datasets, backgrounds)
  if(verbosity>1) cout<<" - Loading histograms ..."<<endl;
  plr.LoadHistos();
  plr.AddBkgDeterminationParameter();
  if(verbosity>1) cout<<" - Loading Signal Systematics ..."<<endl;
  anaEL.LoadSystematicsEffect(plr);

 
  if(verbosity>1) cout<<" - Run the PLR ..."<<endl;
  plr.RunLikelihood(doPseudoExp,NofPseudoExp,false,true,true);
  //plr.RunLikelihood(false,1,false,true,true);
  //plr.RunLikelihood(true,10000,false,true,true);
  //plr.RunLikelihood(true,1,false,true,true);
 
  plr.TimingReport();
  
  if(verbosity>1) cout<<" - Write output tex file ..."<<endl;
  string ofoutname = string(argv[2])+".tex";
  ofstream ofout(ofoutname.c_str());
  plr.MinimizationTable(ofout);
  plr.UncertaintiesTable(ofout);
  plr.NumberOfEventsTable(ofout);

  if(verbosity>1) cout<<" - Write output root file ..."<<endl;
  TFile* fout = new TFile(argv[2],"RECREATE");
  //TFile* fout = new TFile("PLR.root","RECREATE");
  plr.Write(fout);
  fout->Close();
  delete fout;

 cout << "#########################" << endl;
  cout << "  End of the program" << endl;
  cout << "#########################" << endl;

}
