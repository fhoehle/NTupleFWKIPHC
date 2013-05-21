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

using namespace std;



//void ThetaBtagTemplateProvider(int argc, char* argv[]){
int main (int argc, char *argv[])
{
  if(argc<3){
	 cerr<<"usage: ./PLRBtagTemplateProvider infile.root outfile.root"<<endl;
	 return(0);
  }
  cout<<"Execution:"<<endl;
  cout<<" argc = "<<argc<<endl;
  cout<<" - Input file: "<<argv[1]<<endl;
  cout<<" - Output file: "<<argv[2]<<endl;

// Which file to read
  string inFileName;
  if (argc>1 ) inFileName = string(argv[1]);
  else inFileName = string ("CrossSectionTable_8.root");
  TFile *file0 = new TFile (inFileName.c_str(),"READ");

//////////
  string outFileName;
  if (argc>2 ) outFileName = string(argv[2]);
  else outFileName = string ("btag.root");
  cout<<"outFileName = "<<outFileName<<endl;
  TFile *fileoutput = new TFile (outFileName.c_str(), "RECREATE");

  // List of histo to load
  //TH1F
  vector<string> HistoNamesTH1F;
  HistoNamesTH1F.push_back(string("NBjets"));
  HistoNamesTH1F.push_back(string("Njets"));
  HistoNamesTH1F.push_back(string("Njets_JES-minus"));
  HistoNamesTH1F.push_back(string("Njets_JES-plus"));
  HistoNamesTH1F.push_back(string("Njets_JER-minus"));
  HistoNamesTH1F.push_back(string("Njets_JER-plus"));
  //HistoNamesTH1F.push_back(string("Njets_MES-minus"));
  //HistoNamesTH1F.push_back(string("Njets_MES-plus"));
  //HistoNamesTH1F.push_back(string("Njets_EES-minus"));
  //HistoNamesTH1F.push_back(string("Njets_EES-plus"));
  //TH2F
  vector<string> HistoNamesTH2F;
  HistoNamesTH2F.push_back(string("NjetsNBjets"));
  HistoNamesTH2F.push_back(string("NjetsNBjets_JES-minus"));
  HistoNamesTH2F.push_back(string("NjetsNBjets_JES-plus"));
  HistoNamesTH2F.push_back(string("NjetsNBjets_JER-minus"));
  HistoNamesTH2F.push_back(string("NjetsNBjets_JER-plus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_MES-minus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_MES-plus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_EES-minus"));
  //HistoNamesTH2F.push_back(string("NjetsNBjets_EES-plus"));


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
  graphNames.push_back(string("sfb"));
  graphNames.push_back(string("sfl"));

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

  vector < pair < TString, int > > process;
  process.push_back (pair < TString, int >(TString ("TT"), 2));
  process.push_back (pair < TString, int >(TString ("StW"), 1));
  process.push_back (pair < TString, int >(TString ("Zjets"), 1));
  process.push_back (pair < TString, int >(TString ("VV"), 1));
  //process.push_back(pair<TString,int>(TString("Wjets"),2));

 
  file0->cd ();

  vector <string> DataNames;
  DataNames.push_back(string("DataMu"));
  DataNames.push_back(string("DataEG"));
 
  TH1F *GenericTH1F = 0;
  TH2F *GenericTH2F = 0;
  string nin;
  string nout;

  // For Data only
  for(unsigned int hn=0;hn<HistoNamesTH1F.size();hn++){
	 for(unsigned int dn=0;dn<DataNames.size();dn++){
           if((DataNames[dn]==string("DataEG") && channelNames[ic]==string("ee")) || (DataNames[dn]==string("DataEG") && channelNames[ic]==string("emu")) || (DataNames[dn]==string("DataMu") && channelNames[ic]==string("mumu"))){
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
      TH2F *Cadre = new TH2F ("Template", Tit, 100, 0.4, 1.6, 100, 0., 2.);
      //Cadre->GetYaxis()->SetRangeUser(0.,2.);

      //Declaration outside the loops
      TLegend *leg = 0;
      TH2F *Weight2D_aftermetcut_TTbarSig = 0;
      TH2F *Count2D_aftermetcut_TTbarSig = 0;


      for (int i = valnj; i < valnj + 1; i++) {	// # of jets
	char canvasName[200];
	sprintf (canvasName, "canvas_%s_%s_%s_nj%d", name.c_str(),sel.c_str(),proc.c_str (), valnj);
	TCanvas *c1 = new TCanvas (canvasName, "c1");
	c1->cd ();
	c1->SetGridx ();
	c1->SetGridy ();
	Cadre->Draw ();
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
	  if (proc == "StW")
	    proc = "SingleToptW";

	 //Generic histo TH1F
         for(unsigned int hn=0;hn<HistoNamesTH1F.size();hn++){
	  if (proc != "Wjets" && proc != "VV") {
	    nin = name + "_" + HistoNamesTH1F[hn] +  "_" + proc;
	    nout = nin;
	    GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
	  }
	  else if (proc == "Wjets") {
	    nout = name + "_" + HistoNamesTH1F[hn] + proc;
	    nin = name + "_" + HistoNamesTH1F[hn] + "_WjetsE";
	    GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
 	    GenericTH1F->SetName(nout.c_str());
	    nin = name + "_" + HistoNamesTH1F[hn] + "_WjetsMu";
	    GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
	    nin = name + "_" + HistoNamesTH1F[hn] + "_WjetsTau";
	    GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
	  }
	  else if (proc == "VV") {
	    nout = name + "_" + HistoNamesTH1F[hn] + proc;
	    nin = name + "_" + HistoNamesTH1F[hn] + "_WW";
	    GenericTH1F = (TH1F *) file0->Get (nin.c_str ())->Clone ();
 	    GenericTH1F->SetName(nout.c_str());
	    nin = name + "_" + HistoNamesTH1F[hn] + "_WZ";
	    GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
	    nin = name + "_" + HistoNamesTH1F[hn] + "_ZZ";
	    GenericTH1F->Add((TH1F *) file0->Get (nin.c_str ())->Clone ());
	  }
	    //file0->cd();
	    fileoutput->cd();
	    GenericTH1F->Write();
	 }
	 //Generic histo TH2F
         for(unsigned int hn=0;hn<HistoNamesTH2F.size();hn++){
	  if (proc != "Wjets" && proc != "VV") {
	    nin = name + "_" + HistoNamesTH2F[hn] +  "_" + proc;
	    nout = nin;
	    cout << "Name: " << nin << endl;
	    GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	  }
	  else if (proc == "Wjets") {
	    nout = name + "_" + HistoNamesTH2F[hn] + "_Wjets";
	    nin = name + "_" + HistoNamesTH2F[hn] + "_WjetsE";
	    GenericTH2F = (TH2F *) file0->Get (nout.c_str ())->Clone ();
	    GenericTH2F->SetName(nout.c_str());
	    nin = name + "_" + HistoNamesTH2F[hn] + "_WjetsMu";
	    GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = name + "_" + HistoNamesTH2F[hn] + "_WjetsTau";
	    GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
	  }
	  else if (proc == "VV") {
	    nin = name + "_" + HistoNamesTH2F[hn] + "_WW";
	    nout = name + "_" + HistoNamesTH2F[hn] + "_VV";
	    GenericTH2F = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	    GenericTH2F->SetName(nout.c_str());
	    nin = name + "_" + HistoNamesTH2F[hn] + "_WZ";
	    GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = name + "_" + HistoNamesTH2F[hn] + "_ZZ";
	    GenericTH2F->Add((TH2F *) file0->Get (nin.c_str ())->Clone ());
	  }
	    //file0->cd();
	    fileoutput->cd();
	    GenericTH2F->Write();
	 }
	


	  if (proc != "Wjets" && proc != "VV") {
	    nin = "Count2D_" + name + "_" + sel + nbj + "_" + proc;
	    nout = name + "_" + sel + nj + nbj + "_" + proc;
	    cout << "Name: " << nin << endl;
	    Count2D_aftermetcut_TTbarSig = ((TH2F *) file0->Get (nin.c_str ())->Clone (""));
	    Count2D_aftermetcut_TTbarSig->SetName (nout.c_str ());
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_" + proc;
	    cout << "Name: " << nin << endl;
	    Weight2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	    Weight2D_aftermetcut_TTbarSig->SetName (nout.c_str ());
	    Weight2D_aftermetcut_TTbarSig->Divide (Count2D_aftermetcut_TTbarSig);
	  }
	  else if (proc == "Wjets") {
	    nin = "Count2D_" + name + "_" + sel + nbj + "_WjetsMu";
	    Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	    nin = "Count2D_" + name + "_" + sel + nbj + "_WjetsE";
	    Count2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Count2D_" + name + "_" + sel + nbj + "_WjetsTau";
	    Count2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Count2D_" + name + "_" + sel + nbj + "_" + proc;
	    Count2D_aftermetcut_TTbarSig->SetName (nout.c_str ());
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_WjetsMu";
	    Weight2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_WjetsE";
	    Weight2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_WjetsTau";
	    Weight2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_" + proc;
	    Weight2D_aftermetcut_TTbarSig->SetName (nout.c_str ());
	    Weight2D_aftermetcut_TTbarSig->Divide (Count2D_aftermetcut_TTbarSig);
	  }
	  else if (proc == "VV") {
	    nin = "Count2D_" + name + "_" + sel + nbj + "_WW";
	    Count2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	    nin = "Count2D_" + name + "_" + sel + nbj + "_WZ";
	    Count2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Count2D_" + name + "_" + sel + nbj + "_ZZ";
	    Count2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Count2D_" + name + "_" + sel + nbj + "_" + proc;
	    Count2D_aftermetcut_TTbarSig->SetName (nout.c_str ());
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_WW";
	    Weight2D_aftermetcut_TTbarSig = (TH2F *) file0->Get (nin.c_str ())->Clone ();
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_WZ";
	    Weight2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_ZZ";
	    Weight2D_aftermetcut_TTbarSig->Add ((TH2F *) file0->Get (nin.c_str ())->Clone ());
	    nin = "Weight2D_" + name + "_" + sel + nbj + "_" + proc;
	    Weight2D_aftermetcut_TTbarSig->SetName (nout.c_str ());
	    Weight2D_aftermetcut_TTbarSig->Divide (Count2D_aftermetcut_TTbarSig);
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

	  TGraphErrors *g = new TGraphErrors ();
	  g = Tg (Weight2D_aftermetcut_TTbarSig, i, color, poly);
	  char gname[200];
	  sprintf(gname,"%s_%s_%d_%d_%s",name.c_str(),sel.c_str(),i,j,proc.c_str());
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
	    leg->AddEntry (Weight2D_aftermetcut_TTbarSig, njleg, "");
	  leg->AddEntry (g, legtxt, "pl");


	}			// j
	leg->Draw ();
        fileoutput->cd ();
        //file0->cd ();
        c1->Write ();
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



}

TGraphErrors *Tg (TH2F * h2d, int nj, int color, TString poly)
{

  Int_t nPoints = h2d->GetNbinsX () + 1;
  Double_t *x = new Double_t[nPoints], *ex = new Double_t[nPoints], *y = new Double_t[nPoints], *ey = new Double_t[nPoints];


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
  if (poly != "no") {
    g->Fit (poly, "Q");		// Here only used to stabilize the fit (root == bullshit....)
    g->Fit (poly);		// Here only used to stabilize the fit
    g->Fit (poly, "F");
    g->GetFunction (poly)->SetLineColor (color);
//   g->Fit("pol2","N"); // don't plot the fit result
  }

  return g;

}
