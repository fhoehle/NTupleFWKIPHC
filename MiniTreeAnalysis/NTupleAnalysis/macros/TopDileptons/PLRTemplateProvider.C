// Macro for  providing histos for Theta

#include <iomanip>
#include <fstream>
#include <iostream>
#include <math.h>
#include "TStyle.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TF1.h"


#include "PLRBtagTemplateProvider.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"

using namespace std;



//void ThetaBtagTemplateProvider(int argc, char* argv[]){
int main (int argc, char *argv[])
{
  if(argc<3){
	 cerr<<"usage: ./PLRBtagTemplateProvider infile.xml infile.root outfile.root"<<endl;
	 return(0);
  }
  cout<<"Execution:"<<endl;
  cout<<" argc = "<<argc<<endl;
  cout<<" - Input file: "<<argv[1]<<endl;
  cout<<" - Output file: "<<argv[2]<<endl;

  string xmlFileName;
  if (argc>1 ) xmlFileName = string(argv[1]);

  AnalysisEnvironmentLoader anaEL (xmlFileName);
  //Load obsservables
  vector<Observable> observables;
  anaEL.LoadObservables(observables);
  //Load systematics
  vector<ExtendedSyst> systList; 
  vector<string> recognizedSystList; 
  recognizedSystList.push_back(string("JES"));
  recognizedSystList.push_back(string("JER"));
  recognizedSystList.push_back(string("METS"));
  recognizedSystList.push_back(string("EES"));
  recognizedSystList.push_back(string("MES"));
  recognizedSystList.push_back(string("sfb"));
  recognizedSystList.push_back(string("sfl"));
  ExtendedSyst syst_;
  for(unsigned int i=0;i<recognizedSystList.size();i++){
	anaEL.LoadSystematics(recognizedSystList[i],syst_);
	if(syst_.doIt){
		systList.push_back(syst_);
	} 
  }


// Which file to read
  string inFileName;
  if (argc>1 ) inFileName = string(argv[2]);
  else inFileName = string ("CrossSectionTable_8.root");
  TFile *file0 = new TFile (inFileName.c_str(),"READ");

//////////
  string outFileName;
  if (argc>2 ) outFileName = string(argv[3]);
  else outFileName = string ("btag.root");
  cout<<"outFileName = "<<outFileName<<endl;
  TFile *fileoutput = new TFile (outFileName.c_str(), "RECREATE");

  // List of histo to load
  //TH1F
  vector<string> HistoNamesTH1F;
  //HistoNamesTH1F.push_back(string("NBjets"));
  //HistoNamesTH1F.push_back(string("Njets"));
  //HistoNamesTH1F.push_back(string("Njets_JES-minus"));
  //HistoNamesTH1F.push_back(string("Njets_JES-plus"));
  //HistoNamesTH1F.push_back(string("Njets_JER-minus"));
  //HistoNamesTH1F.push_back(string("Njets_JER-plus"));
  //HistoNamesTH1F.push_back(string("Njets_MES-minus"));
  //HistoNamesTH1F.push_back(string("Njets_MES-plus"));
  //HistoNamesTH1F.push_back(string("Njets_EES-minus"));
  //HistoNamesTH1F.push_back(string("Njets_EES-plus"));
  //TH2F
  vector<string> HistoNamesTH2F;
  //HistoNamesTH2F.push_back(string("NjetsNBjets"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_JES-minus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_JES-plus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_JER-minus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_JER-plus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_MES-minus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_MES-plus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_EES-minus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_EES-plus"));

  for(unsigned int i=0;i<observables.size();i++){
	//reference histos
	if(observables[i].dim == 1) HistoNamesTH1F.push_back(observables[i].name);
	//retrieve the plots for sfb and sflobservables[i].dim == 1) HistoNamesTH1F.push_back(observables[i].name);
	if(observables[i].dim == 2) HistoNamesTH2F.push_back(observables[i].name);
	//systematics histos
	for(unsigned int j=0;j<systList.size();j++){
		string hname = observables[i].name+"_"+systList[j].name;
		if(systList[j].discretized){
			string hnameMinus = hname+"-minus";
			string hnamePlus = hname+"-plus";
			if(observables[i].dim == 1){
				 HistoNamesTH1F.push_back(hnameMinus);
				 HistoNamesTH1F.push_back(hnamePlus);
			}
			if(observables[i].dim == 2){
				 HistoNamesTH2F.push_back(hnameMinus);
				 HistoNamesTH2F.push_back(hnamePlus);
			}
		}
	}
  }


  TRint *theApp = new TRint ("ROOT example", &argc, argv);
  //theApp->Run();

  /////////////////////////////////////////////////////////////

  gStyle->SetOptStat (000000);
  gStyle->SetCanvasColor (10);
  gStyle->SetFrameFillColor (10);
  gStyle->SetCanvasBorderMode (0);
  gStyle->SetLineWidth (2.);
  gStyle->SetTextSize (1.1);
  gStyle->SetLabelSize (0.06, "xy");
  gStyle->SetTitleSize (0.06, "xy");
  gStyle->SetTitleOffset (1.2, "x");
  gStyle->SetTitleOffset (1.0, "y");
  gStyle->SetPadTopMargin (0.1);
  gStyle->SetPadRightMargin (0.1);
  gStyle->SetPadBottomMargin (0.16);
  gStyle->SetPadLeftMargin (0.12);

  gStyle->SetHistLineWidth (2.);
  gStyle->SetPalette (1);


  /////////////////////////////////////////////////////////////

  TString his;

  //string name = "mumu";
  //string name = "ee";
  //string name = "mumu";
  //string sel = "sfl";


  vector<string> graphNames;
  for(unsigned int o=0;o<systList.size();o++){
	if(systList[o].name == "sfb")
  		graphNames.push_back(string("sfb"));
  	if(systList[o].name == "sfl")
		graphNames.push_back(string("sfl"));
  }

  vector<string> channelNames;
  channelNames.push_back(string("emu"));
  channelNames.push_back(string("ee"));
  channelNames.push_back(string("mumu"));

  //Loop to run on sfb and sfl
  

  for(unsigned int is=0;is<graphNames.size();is++){
  	string sel = graphNames[is];

  //Loop over channels

  for(unsigned int ic=0;ic<channelNames.size();ic++){
	string name = channelNames[ic];

  int valnj = 0;
  int valpol = 2;
  string proc = "TT";
  TString poly = "pol2";
  /*
     cout << "Process [TT,Zjets,Wjets,StW,VV] ? "<<std::endl;
     cin >> proc ;
     cout << "Number of jets (2<=N<=4) ? "<<std::endl;
     cin >> valnj ;
     cout << "Degree of polynomial for fitting (-1=no/1/2) ? "<<std::endl;
     cin >> valpol ;
   */

  
  ///////////////////////////////////////////////
  // List of merged samples
  ///////////////////////////////////////////////
  vector < pair < TString, int > > process;
  process.push_back (pair < TString, int >(TString ("TT"), 2));
  process.push_back (pair < TString, int >(TString ("StW"), 1));
  process.push_back (pair < TString, int >(TString ("Zjets"), 1));
  process.push_back (pair < TString, int >(TString ("VV"), 1));
  process.push_back(pair<TString,int>(TString("Wjets"),2));
  process.push_back(pair<TString,int>(TString("TTbarBkg"),2));

  ///////////////////////////////////////////
  //List of samples for merging per category
  ///////////////////////////////////////////


  vector<TString> WjetsProcessMerging;
  WjetsProcessMerging.push_back(TString("Wjets"));
  //WjetsProcessMerging.push_back(TString("WjetsE"));
  //WjetsProcessMerging.push_back(TString("WjetsMu"));
  //WjetsProcessMerging.push_back(TString("WjetsTau"));
 
  vector<TString> ZjetsProcessMerging;
  ZjetsProcessMerging.push_back(TString("Zjets"));
  ZjetsProcessMerging.push_back(TString("DYToLL_M10-50"));
  //ZjetsProcessMerging.push_back(TString("ZjetsE"));
  //ZjetsProcessMerging.push_back(TString("ZjetsMu"));
  //ZjetsProcessMerging.push_back(TString("ZjetsTau"));
  
  vector<TString> VVProcessMerging;
 // VVProcessMerging.push_back(TString("VV"));
  VVProcessMerging.push_back(TString("WW"));
  VVProcessMerging.push_back(TString("WZ"));
  VVProcessMerging.push_back(TString("ZZ"));
  
  vector<TString> SingleTopProcessMerging;
//  SingleTopProcessMerging.push_back(TString("SingleToptW"));
  SingleTopProcessMerging.push_back(TString("TtW"));
  SingleTopProcessMerging.push_back(TString("TbartW"));
  
  vector<TString> TTbarBkgProcessMerging;
  TTbarBkgProcessMerging.push_back(TString("TTbarBkg"));

  vector <string> DataNames;
  DataNames.push_back(string("DataMu"));
  DataNames.push_back(string("DataMuEG"));
  DataNames.push_back(string("DataEG"));
  
  ///////////////////////////////////////////

  file0->cd ();

 
  TH1F *GenericTH1F = 0;
  TH2F *GenericTH2F = 0;
  string nin;
  string nout;

  // For Data only
  for(unsigned int hn=0;hn<HistoNamesTH1F.size();hn++){
	 for(unsigned int dn=0;dn<DataNames.size();dn++){
           if((DataNames[dn]==string("DataEG") && channelNames[ic]==string("ee")) || (DataNames[dn]==string("DataMuEG") && channelNames[ic]==string("emu")) || (DataNames[dn]==string("DataMu") && channelNames[ic]==string("mumu"))){
	    nin = name + "_" + HistoNamesTH1F[hn] +  "_" + DataNames[dn];
	    nout = name + "_" + HistoNamesTH1F[hn] +  "_Data" ;
	    if(file0->Get (nin.c_str ())){ 
		GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
		GenericTH1F->SetName(nout.c_str());
	        //file0->cd();
	        fileoutput->cd();
	        GenericTH1F->Write();
	    }
	  }
	 }
  }
  for(unsigned int hn=0;hn<HistoNamesTH2F.size();hn++){
	 for(unsigned int dn=0;dn<DataNames.size();dn++){
           if((DataNames[dn]==string("DataEG") && channelNames[ic]==string("ee")) || (DataNames[dn]==string("DataMuEG") && channelNames[ic]==string("emu")) || (DataNames[dn]==string("DataMu") && channelNames[ic]==string("mumu"))){
	    cout<<nin<<endl;
	    nin = name + "_" + HistoNamesTH2F[hn] +  "_" + DataNames[dn];
	    nout = name + "_" + HistoNamesTH2F[hn] +  "_Data" ;
	    if(file0->Get (nin.c_str ())){ 
		cout<<"write "<<nout<<endl;
		GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
		GenericTH2F->SetName(nout.c_str());
	        //file0->cd();
	        fileoutput->cd();
	        GenericTH2F->Write();
	    }
	  }
	 }
  }
  ////////////////////////////////////////////////////////////////

  //replace by 2 loops
  for (unsigned int pr = 0; pr < process.size (); pr++) {
    proc = process[pr].first;
    valpol = process[pr].second;
    cout << "Process " << proc << endl;
    for (int pjn = 2; pjn < 5; pjn++) {
      valnj = pjn;
      cout << " ***** Njets = " << valnj << endl;


      if (valpol == -1)
	poly = "no";
      if (valpol == 1)
	poly = "pol1";
      if (valpol == 2)
	poly = "pol2";

      TString Tit = "";
      if (proc == "TT")
	Tit = "Template TTbar";
      if (proc == "Zjets")
	Tit = "Template Zjets";
      if (proc == "Wjets")
	Tit = "Template Wjets";
      if (proc == "StW")
	Tit = "Template SingleToptW";
      if (proc == "VV")
	Tit = "Template VV";
      if (proc == "TTbarBkg")
	Tit = "Template TTbarBkg";
      TH2F *Cadre = new TH2F ("Template", Tit, 100, 0.4, 1.6, 100, 0., 2.);
      //Cadre->GetYaxis()->SetRangeUser(0.,2.);

      //Declaration outside the loops
      TLegend *leg = 0;
      TH2F *Count2D_aftermetcut_TTbarSig = 0;


      for (int i = valnj; i < valnj + 1; i++) {	// # of jets
	/*
	char canvasName[200];
	sprintf (canvasName, "canvas_%s_%s_%s_nj%d", name.c_str(),sel.c_str(),proc.c_str (), valnj);
	TCanvas *c1 = new TCanvas (canvasName, "c1");
	c1->cd ();
	c1->SetGridx ();
	c1->SetGridy ();
	//Cadre->Draw ();
	*/
	leg = new TLegend (0., 0, 0.2, 0.2);

	for (int j = 0; j <= i; j++) {	// # of b jets
//     for(unsigned int j=2;j<=i;j++) {      // # of b jets

	//to be changed after bug correction !!!!
	//if(j>=3) break;

	  //string nin;
	  //string nout;
	  string nj = "";
	  if (valnj == 2)
	    nj = "_2j";
	  if (valnj == 3)
	    nj = "_3j";
	  if (valnj >= 4)
	    nj = "_4j";
	  string nbj = "";
	  if (j == 0)
	    nbj = "_0b";
	  if (j == 1)
	    nbj = "_1b";
	  if (j == 2)
	    nbj = "_2b";
	  if (j >= 3)
	    nbj = "_3b";


	  if (proc == "TT")
	    proc = "TTbarSig";
//	  if (proc == "StW")
//	    proc = "SingleToptW";

         std::cout<<"proc,nj,nbj "<<proc<<" "<<nj<<" "<<nbj<<std::endl;

	 //Generic histo TH1F
         for(unsigned int hn=0;hn<HistoNamesTH1F.size();hn++){
	  if (proc != "Wjets" && proc != "Zjets" && proc != "VV" && proc!="StW" && proc != "TTbarBkg") {
	    nin = name + "_" + HistoNamesTH1F[hn] +  "_" + proc;
		cout<<"nin = "<<nin<<endl;
	    nout = nin;
	    GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
	  }
	  else if (proc == "Wjets") {
	    nout = name + "_" + HistoNamesTH1F[hn] + proc;
	    for(unsigned int x=0;x<WjetsProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + WjetsProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH1F[hn] + "_" + proc;
	   		GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH1F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + WjetsProcessMerging[x];
			GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "Zjets") {
	    nout = name + "_" + HistoNamesTH1F[hn] + proc;
	    for(unsigned int x=0;x<ZjetsProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + ZjetsProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH1F[hn] + "_" + proc;
			cout<<"nin: "<<nin<<endl;
	   		GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH1F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + ZjetsProcessMerging[x];
			cout<<"nin: "<<nin<<endl;
			GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "VV") {
	    nout = name + "_" + HistoNamesTH1F[hn] + proc;
	    for(unsigned int x=0;x<VVProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + VVProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH1F[hn] + "_" + proc;
	   		GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH1F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + VVProcessMerging[x];
			GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "StW") {
	    nout = name + "_" + HistoNamesTH1F[hn] + proc;
	    for(unsigned int x=0;x<SingleTopProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + SingleTopProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH1F[hn] + "_" + proc;
	   		GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH1F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + SingleTopProcessMerging[x];
			GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "TTbarBkg") {
	    nout = name + "_" + HistoNamesTH1F[hn] + proc;
	    for(unsigned int x=0;x<TTbarBkgProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + TTbarBkgProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH1F[hn] + "_" + proc;
	   		GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH1F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH1F[hn] + "_" + TTbarBkgProcessMerging[x];
			GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	    //file0->cd();
	    fileoutput->cd();
	    GenericTH1F->Write();
	 }
	 //Generic histo TH2F
         for(unsigned int hn=0;hn<HistoNamesTH2F.size();hn++){
	  if (proc != "Wjets" && proc != "Zjets" && proc != "VV" && proc != "StW" && proc != "TTbarBkg" ) {
	    nin = name + "_" + HistoNamesTH2F[hn] +  "_" + proc;
		cout<<"nin2 = "<<nin<<endl;
	    nout = nin;
	    GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	  }
	  else if (proc == "Wjets") {
	    nout = name + "_" + HistoNamesTH2F[hn] + proc;
	    for(unsigned int x=0;x<WjetsProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + WjetsProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH2F[hn] + "_" + proc;
	   		GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH2F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + WjetsProcessMerging[x];
			GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "Zjets") {
	    nout = name + "_" + HistoNamesTH2F[hn] + proc;
	    for(unsigned int x=0;x<ZjetsProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + ZjetsProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH2F[hn] + "_" + proc;
	   		GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH2F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + ZjetsProcessMerging[x];
			GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "VV") {
	    nout = name + "_" + HistoNamesTH2F[hn] + proc;
	    for(unsigned int x=0;x<VVProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + VVProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH2F[hn] + "_" + proc;
	   		GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH2F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + VVProcessMerging[x];
			GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "StW") {
	    nout = name + "_" + HistoNamesTH2F[hn] + proc;
	    for(unsigned int x=0;x<SingleTopProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + SingleTopProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH2F[hn] + "_" + proc;
	   		GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH2F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + SingleTopProcessMerging[x];
			GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "TTbarBkg") {
	    nout = name + "_" + HistoNamesTH2F[hn] + proc;
	    for(unsigned int x=0;x<TTbarBkgProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + TTbarBkgProcessMerging[x];
	    		nout = name + "_" + HistoNamesTH2F[hn] + "_" + proc;
	   		GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		GenericTH2F->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + HistoNamesTH2F[hn] + "_" + TTbarBkgProcessMerging[x];
			GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	    //file0->cd();
	    fileoutput->cd();
	    GenericTH2F->Write();
	 }
	
	/////////////////////////////////////////
	//retrieve the plots for sfb and sfl
	/////////////////////////////////////////

      for(unsigned int o=0;o<observables.size();o++){
	if(observables[o].name.find(string("NjetsNBjets"))<observables[o].name.size()){ //else: do nothing

	cout<<"obs: "<<observables[o].name<<endl;
	  if (proc != "Wjets" && proc != "Zjets" && proc != "VV" && proc != "StW" && proc != "TTbarBkg") {
	    nin = name + "_" + observables[o].name + "_" + sel + nbj +  "_" + proc;
		cout<<"nin3 = "<<nin<<endl;
	    nout = nin;
	    Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	  }
	  else if (proc == "Wjets") {
	    nout = name + "_" + observables[o].name + "_" + sel + nbj + proc;
	    for(unsigned int x=0;x<WjetsProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + WjetsProcessMerging[x];
	    		nout = name + "_" + observables[o].name + "_" + sel + nbj  + "_" + proc;
	   		Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		Count2D_aftermetcut_TTbarSig->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + WjetsProcessMerging[x];
			Count2D_aftermetcut_TTbarSig->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "Zjets") {
	    nout = name + "_" + observables[o].name + "_" + sel + nbj + proc;
	    for(unsigned int x=0;x<ZjetsProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + ZjetsProcessMerging[x];
	    		nout = name + "_" + observables[o].name + "_" + sel + nbj  + "_" + proc;
	   		Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		Count2D_aftermetcut_TTbarSig->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + ZjetsProcessMerging[x];
			Count2D_aftermetcut_TTbarSig->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "VV") {
	    nout = name + "_" + observables[o].name + "_" + sel + nbj + proc;
	    for(unsigned int x=0;x<VVProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + VVProcessMerging[x];
	    		nout = name + "_" + observables[o].name + "_" + sel + nbj  + "_" + proc;
	   		Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		Count2D_aftermetcut_TTbarSig->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + VVProcessMerging[x];
			Count2D_aftermetcut_TTbarSig->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "StW") {
	    nout = name + "_" + observables[o].name + "_" + sel + nbj + proc;
	    for(unsigned int x=0;x<SingleTopProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + SingleTopProcessMerging[x];
	    		nout = name + "_" + observables[o].name + "_" + sel + nbj  + "_" + proc;
	   		Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		Count2D_aftermetcut_TTbarSig->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + SingleTopProcessMerging[x];
			Count2D_aftermetcut_TTbarSig->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }
	  else if (proc == "TTbarBkg") {
	    nout = name + "_" + observables[o].name + "_" + sel + nbj + proc;
	    for(unsigned int x=0;x<TTbarBkgProcessMerging.size();x++){
		if(x==0){
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + TTbarBkgProcessMerging[x];
	    		nout = name + "_" + observables[o].name + "_" + sel + nbj  + "_" + proc;
	   		Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
 	  		Count2D_aftermetcut_TTbarSig->SetName(nout.c_str());
		} 
		else{
	    		nin = name + "_" + observables[o].name + "_" + sel + nbj + "_" + TTbarBkgProcessMerging[x];
			Count2D_aftermetcut_TTbarSig->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
		}
	    }
	  }

	  int color = 0;
	  TString legtxt = "";
	  if (j == 0) {
	    color = 2;
	    legtxt = "==0 bjet";
	  }
	  if (j == 1) {
	    color = 3;
	    legtxt = "==1 bjet";
	  }
	  if (j == 2) {
	    color = 4;
	    legtxt = "==2 bjets";
	  }
	  if (j == 3) {
	    color = 5;
	    legtxt = "==3 bjets";
	  }
	  if (j >= 4) {
	    color = 6;
	    legtxt = ">=3 bjets";
	  }

	  std::cout << "++++++++++++++++++++++++++++++++" << std::endl;
	  std::cout << "Nombre de jets = " << i << std::endl;
	  std::cout << "Nombre de b-jets = " << j << std::endl;
	  std::cout << "++++++++++++++++++++++++++++++++" << std::endl;

	  std::cout << "Count2D_aftermetcut_TTbarSig NAME "<<Count2D_aftermetcut_TTbarSig->GetName() << std::endl;
	  cout<<"YMean of ProjY: "<<Count2D_aftermetcut_TTbarSig->ProjectionY()->GetMean()<<endl;

	  TGraphErrors *g = new TGraphErrors ();
	  //g = Tg (Weight2D_aftermetcut_TTbarSig, i, color, poly);
	  g = Tg (Count2D_aftermetcut_TTbarSig, i, color, poly);
          Double_t x,y = 0;
	  g->GetPoint(1,x,y);
          cout<<"graph: pointer: "<< Count2D_aftermetcut_TTbarSig<<" & "<<g<<" max: "<<g->GetMaximum()<<" (x,y)=("<<x<<","<<y<<")"<<endl;
	  char gname[200];
	  //sprintf(gname,"%s_%s_%d_%d_%s",name.c_str(),sel.c_str(),i,j,proc.c_str());
	  sprintf(gname,"%s_%s_%s_%d_%d_%s",name.c_str(),observables[o].name.c_str(),sel.c_str(),i,j,proc.c_str());
	  g->SetName(gname);
	  fileoutput->cd();
	  //file0->cd();
	  g->GetYaxis()->SetRange(0.,2.);
	  g->Write();
//       Tg(Weight2D_aftermetcut_TTbarSig,i,color,poly)->Draw("P");
	  g->Draw ("P");

	  TString njleg = "";
	  if (i == 1)
	    njleg = "1 jet";
	  if (i == 2)
	    njleg = "2 jets";
	  if (i == 3)
	    njleg = "3 jets";
	  if (i >= 4)
	    njleg = ">=4 jets";
	  if (j == 0)
	    //leg->AddEntry (Weight2D_aftermetcut_TTbarSig, njleg, "");
	    leg->AddEntry (Count2D_aftermetcut_TTbarSig, njleg, "");
	  leg->AddEntry (g, legtxt, "pl");
	
	}//end of if variable == NjetsNBjets*
	}//end of the loop over variables

	}			// j
	
//	c1->Update();
	leg->Draw ();
        fileoutput->cd ();
        //file0->cd ();
        //c1->Write ();
      }				// i

      leg->Draw ();
//  c1->SaveAs("dum.eps");

      fileoutput->cd ();
      //file0->cd ();
      //c1->Write ();
    }
  }

  }// loop over channels
  }// loop over graphNames (sfb,sfl)

  fileoutput->Close ();
  fileoutput->Delete ();
  //file0->Close ();
  //file0->Delete ();


  cout<<"#############################################"<<endl;
  cout<<"     THE                END             !!   "<<endl;
  cout<<"#############################################"<<endl;


}

TGraphErrors *Tg (TH2F * h2d, int nj, int color, TString poly)
{

  float xmin = 0;
  float xmax = 0;

  Int_t nPoints = h2d->GetNbinsX () + 1;
  Double_t *x = new Double_t[nPoints], *ex = new Double_t[nPoints], *y = new Double_t[nPoints], *ey = new Double_t[nPoints];

  xmin = h2d->GetBinLowEdge (1);
  xmax = h2d->GetBinLowEdge (h2d->GetNbinsX ())+h2d->GetBinWidth(h2d->GetNbinsX ());

  for (int i = 1; i < h2d->GetNbinsX () + 1; i++) { 
    x[i] = h2d->GetBinCenter (i);
    ex[i] = h2d->GetBinWidth (i) / 2.;
    for (int j = 1; j < h2d->GetNbinsY () + 1; j++) {
      if (h2d->GetYaxis ()->GetBinCenter (j) == nj)
	y[i] = h2d->GetBinContent (i, j);
      if (h2d->GetYaxis ()->GetBinCenter (j) == nj)
	ey[i] = h2d->GetBinError (i, j);
//        if (  h2d->GetYaxis()->GetBinCenter(j)==nj ) ey[i] = 0.;
    }
  }
  
  std::cout<<"passage dans Tg "<<h2d->GetName()<<std::endl;

  //just for now - compte the ration relative to SF_b=1
  int iref = 0;
  for(int i=0;i<nPoints;i++){
	if(fabs(x[i]-1)<0.001){ 
		iref = i;
		break;
	}
  } 
  double ref = y[iref];
  for(int i=0;i<nPoints;i++){
	y[i]=y[i]/ref;
	ey[i]=ey[i]/ref;
  }

  TGraphErrors *g = new TGraphErrors (nPoints, x, y, ex, ey);
  g->SetLineColor (color);
//  g->SetMarkerStyle(23);
//  g->SetMarkerSize(0.7);
  g->SetMarkerColor (color);
  g->SetLineWidth (3);
  g->SetMinimum (0.5);
  
  std::cout<<"passage dans Tg appel fit "<<std::endl; 

  if (poly != "no") {
//     g->Fit (poly, "Q","",xmin,xmax);		// Here only used to stabilize the fit (root == bullshit....)
//     g->Fit (poly,"","",xmin,xmax);		// Here only used to stabilize the fit
//    g->Fit (poly, "F","",xmin,xmax);
//    g->GetFunction (poly)->SetLineColor (color);

  
  std::cout<<"passage dans Tg  fit "<<poly<<std::endl; 

     if (poly=="pol1") {
        TF1 *f1 = new TF1("f1", "pol1",xmin,xmax);
	f1->SetParameters(1.,1.);
        g->Fit ("f1","RQ");
        g->GetFunction ("f1")->SetLineColor (color);
     }
     if (poly=="pol2") {
        TF1 *f2 = new TF1("f2", "pol2",xmin,xmax);
	f2->SetParameters(1.,1.,1.);
        g->Fit ("f2","RQ");
        g->GetFunction ("f2")->SetLineColor (color);
     }


  std::cout<<"passage dans Tg  fin fit "<<std::endl; 



//   g->Fit("pol2","N"); // don't plot the fit result
  }

  return g;

}
