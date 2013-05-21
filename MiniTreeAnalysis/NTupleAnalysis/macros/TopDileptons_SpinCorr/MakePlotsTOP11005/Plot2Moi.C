#include <vector>

#include "TStyle.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLatex.h"
#include "TLegend.h"
#include <iostream>
#include "TCanvas.h"
#include "THStack.h"
#include "TPad.h"
#include "TGraphErrors.h"



using namespace std;



//void Plot(string observable = string ("NjetsNBjets_0_1.70"), string outname = string("TCHEL")){
void Plot2_main(string observable, string outname){
  
  /*
    gStyle->SetPadRightMargin(0.13);
    gStyle->SetPadLeftMargin(0.13);
    gStyle->SetPadBottomMargin(0.13);
  */
  
  
  float SF_trigger_error = 0.015;
  float SF_Lepton_error  = 0.010;
  float SF_MET_error     = 0.016;
  float SF_bag1_error     = 0.01;
  float SF_bag2_error     = 0.02;
 
  
  float systeError_VV     = 0.33;
  float systeError_stop   = 0.31;
  float systeError_ttbar  = 0.04;
  
  double frac_ee   = 0;
  double frac_mumu = 0;
  double frac_emu  = 0;
  
  
  float lumi_error=0.022;
  
  
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0); // must be kWhite but I dunno how to do that in PyROOT
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);
  
  
  // For the Pad:
  gStyle->SetPadBorderMode(0);
  // ROOT . gStyle . SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(0); // kWhite
  gStyle->SetPadGridX(0); //false
  gStyle->SetPadGridY(0); //false
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
  
  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
  
  // For the histo:
  // ROOT . gStyle . SetHistFillColor(1);
  // ROOT . gStyle . SetHistFillStyle(0);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  // ROOT . gStyle . SetLegoInnerR(Float_t rad = 0.5);
  // ROOT . gStyle . SetNumberContours(Int_t number = 20);
  
  gStyle->SetEndErrorSize(2);
  //ROOT . gStyle . SetErrorMarker(20);   /// I COMMENTED THIS OUT
  //ROOT . gStyle . SetErrorX(0.);
  
  //ROOT . gStyle . SetMarkerStyle(20);
  
  
  //For the fit/function:
  gStyle->SetOptFit(1011);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);
  
  //For the date:
  gStyle->SetOptDate(0);
  // ROOT . gStyle . SetDateX(Float_t x = 0.01);
  // ROOT . gStyle . SetDateY(Float_t y = 0.01);
  
  // For the statistics box:
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  gStyle->SetStatColor(0); // kWhite
  gStyle->SetStatFont(42);
  //ROOT . gStyle . SetStatFontSize(0.025);
  gStyle->SetStatFontSize(0.04);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.15);
  // ROOT . gStyle . SetStatStyle(Style_t style = 1001);
  // ROOT . gStyle . SetStatX(Float_t x = 0);
  // ROOT . gStyle . SetStatY(Float_t y = 0);
  
  // Margins:
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.16);
  //ROOT . gStyle . SetPadRightMargin(0.12);
  gStyle->SetPadRightMargin(0.03);
  
  // For the Global title:
  
  gStyle->SetOptTitle(0);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleFontSize(0.05);
  // ROOT . gStyle . SetTitleH(0); // Set the height of the title box
  // ROOT . gStyle . SetTitleW(0); // Set the width of the title box
  // ROOT . gStyle . SetTitleX(0); // Set the position of the title box
  // ROOT . gStyle . SetTitleY(0.985); // Set the position of the title box
  // ROOT . gStyle . SetTitleStyle(Style_t style = 1001);
  // ROOT . gStyle . SetTitleBorderSize(2);
  
  // For the axis titles:
  
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.06, "XYZ");
  // ROOT . gStyle . SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // ROOT . gStyle . SetTitleYSize(Float_t size = 0.02);
  gStyle->SetTitleXOffset(0.9);
  gStyle->SetTitleYOffset(1.25);
  // ROOT . gStyle . SetTitleOffset(1.1, "Y"); // Another way to set the Offset
  
  // For the axis labels:
  
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.05, "XYZ");
  
  // For the axis:
  
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(1); // kTRUE
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(510, "XYZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);
  
  // Change for log plots:
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);
  
  // Postscript options:
  gStyle->SetPaperSize(20.,20.);
  // ROOT . gStyle . SetLineScalePS(Float_t scale = 3);
  // ROOT . gStyle . SetLineStyleString(Int_t i, const char* text);
  // ROOT . gStyle . SetHeaderPS(const char* header);
  // ROOT . gStyle . SetTitlePS(const char* pstitle);
  
  // ROOT . gStyle . SetBarOffset(Float_t baroff = 0.5);
  // ROOT . gStyle . SetBarWidth(Float_t barwidth = 0.5);
  // ROOT . gStyle . SetPaintTextFormat(const char* format = "g");
  // ROOT . gStyle . SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // ROOT . gStyle . SetTimeOffset(Double_t toffset);
  // ROOT . gStyle . SetHistMinimumZero(kTRUE);
  






  //TFile * file = new TFile("templates_proof_with_Wjets.root","OPEN");
  TFile * file = new TFile("outfile_Template.root","OPEN");
  cout<<"toto"<<endl;
  vector<pair<string,int> > process;
  process.push_back(pair<string,int>(string("TTbarSig"),kRed+1));
  process.push_back(pair<string,int>(string("TTbarBkg"),kRed-7));
  process.push_back(pair<string,int>(string("Wjets"),kGreen-3));
  process.push_back(pair<string,int>(string("VV"),13));
  process.push_back(pair<string,int>(string("StW"),kMagenta));
  process.push_back(pair<string,int>(string("Zjets"),kAzure-2));
  process.push_back(pair<string,int>(string("Data"),1));
  
  cout<<"toto"<<endl;
  vector<string> channels;
  channels.push_back(string("ee"));
  channels.push_back(string("emu"));
  channels.push_back(string("mumu"));
  
  cout<<"toto"<<endl;
  
  //string observable ("NjetsNBjets_0_1.70");
  
  cout<<"toto"<<endl;
  /*
    TH2F* histoEmpty = new TH2F("emtpy","",10,0,10,0,500);
    histoEmpty->GetXaxis()->SetTitle("(N_{jets},N_{b-tagged jets})");
    histoEmpty->GetYaxis()->SetTitle("Events");
    histoEmpty->Draw("");
  */
  
  
  TCanvas *c1 = new TCanvas("c1","c1", 1000, 800);
  c1->SetBottomMargin(0.3);
  c1->cd();
  
  
  
  
  TH1D* histo1D_ttSig = new TH1D("histo_ttSig","",11, 0, 11);
  TH1D* histo1D_ttBkg = new TH1D("histo_ttBkg","",11, 0, 11);
  TH1D* histo1D_VV    = new TH1D("histo_VV",   "",11, 0, 11);
  TH1D* histo1D_tW    = new TH1D("histo_tW",   "",11, 0, 11);
  TH1D* histo1D_DY    = new TH1D("histo_DY",   "",11, 0, 11);
  TH1D* histo1D_Data  = new TH1D("histo_Data", "",11, 0, 11);
  
  
  
 
   
  
  //******************************
  //get histograms
  //******************************
  
  
  string name = "ee_NjetsNBjets_6_0.244_TTbarSig";
  TH2D* histo2D_ee_ttSig = (TH2D*) file->Get(name.c_str()); 
  name = "ee_NjetsNBjets_6_0.244_TTbarBkg"; 
  TH2D* histo2D_ee_ttBkg = (TH2D*) file->Get(name.c_str()); 
  name = "ee_NjetsNBjets_6_0.244_VV"; 
  TH2D* histo2D_ee_VV = (TH2D*) file->Get(name.c_str()); 
  name = "ee_NjetsNBjets_6_0.244_StW"; 
  TH2D* histo2D_ee_tW = (TH2D*) file->Get(name.c_str()); 
  name = "ee_NjetsNBjets_6_0.244_Zjets"; 
  TH2D* histo2D_ee_Zjets = (TH2D*) file->Get(name.c_str()); 
  name = "ee_NjetsNBjets_6_0.244_Data"; 
  TH2D* histo2D_ee_Data = (TH2D*) file->Get(name.c_str()); 
  
  
  name = "mumu_NjetsNBjets_6_0.244_TTbarSig";
  TH2D* histo2D_mumu_ttSig = (TH2D*) file->Get(name.c_str()); 
  name = "mumu_NjetsNBjets_6_0.244_TTbarBkg"; 
  TH2D* histo2D_mumu_ttBkg = (TH2D*) file->Get(name.c_str()); 
  name = "mumu_NjetsNBjets_6_0.244_VV"; 
  TH2D* histo2D_mumu_VV = (TH2D*) file->Get(name.c_str()); 
  name = "mumu_NjetsNBjets_6_0.244_StW"; 
  TH2D* histo2D_mumu_tW = (TH2D*) file->Get(name.c_str()); 
  name = "mumu_NjetsNBjets_6_0.244_Zjets"; 
  TH2D* histo2D_mumu_Zjets = (TH2D*) file->Get(name.c_str()); 
  name = "mumu_NjetsNBjets_6_0.244_Data"; 
  TH2D* histo2D_mumu_Data = (TH2D*) file->Get(name.c_str()); 
  
  
  name = "emu_NjetsNBjets_6_0.244_TTbarSig";
  TH2D* histo2D_emu_ttSig = (TH2D*) file->Get(name.c_str()); 
  name = "emu_NjetsNBjets_6_0.244_TTbarBkg"; 
  TH2D* histo2D_emu_ttBkg = (TH2D*) file->Get(name.c_str()); 
  name = "emu_NjetsNBjets_6_0.244_VV"; 
  TH2D* histo2D_emu_VV = (TH2D*) file->Get(name.c_str()); 
  name = "emu_NjetsNBjets_6_0.244_StW"; 
  TH2D* histo2D_emu_tW = (TH2D*) file->Get(name.c_str()); 
  name = "emu_NjetsNBjets_6_0.244_Zjets"; 
  TH2D* histo2D_emu_Zjets = (TH2D*) file->Get(name.c_str()); 
  name = "emu_NjetsNBjets_6_0.244_Data"; 
  TH2D* histo2D_emu_Data = (TH2D*) file->Get(name.c_str()); 
 
 
  
  
  histo2D_ee_Zjets->Scale(220./histo2D_ee_Zjets->Integral());
  histo2D_emu_Zjets->Scale(136./histo2D_emu_Zjets->Integral());
  histo2D_mumu_Zjets->Scale(217./histo2D_mumu_Zjets->Integral());


  histo2D_ee_ttBkg->Scale(9./histo2D_ee_ttBkg->Integral());
  histo2D_emu_ttBkg->Scale(86./histo2D_emu_ttBkg->Integral());
  histo2D_mumu_ttBkg->Scale(15./histo2D_mumu_ttBkg->Integral());
 
 
  histo2D_ee_ttSig->Scale(1.44);
  histo2D_ee_VV->Scale(1.044);
  histo2D_ee_tW->Scale(1.044);
 
  histo2D_emu_ttSig->Scale(1.044);
  histo2D_emu_VV->Scale(1.044);
  histo2D_emu_tW->Scale(1.044);
 
  histo2D_mumu_ttSig->Scale(1.044);
  histo2D_mumu_VV->Scale(1.044);
  histo2D_mumu_tW->Scale(1.044);
  
  
  frac_ee  += histo2D_ee_Zjets->Integral();
  frac_mumu+= histo2D_emu_Zjets->Integral(); 
  frac_emu += histo2D_mumu_Zjets->Integral();
  
  
  frac_ee   = frac_ee/(  frac_ee+frac_mumu+frac_emu);
  frac_mumu = frac_mumu/(frac_ee+frac_mumu+frac_emu);
  frac_emu  = frac_emu/( frac_ee+frac_mumu+frac_emu);
  
  
  
  
  histo2D_ee_ttSig->Add( histo2D_ee_ttSig, histo2D_mumu_ttSig, 1, 1);
  histo2D_ee_ttSig->Add( histo2D_ee_ttSig, histo2D_emu_ttSig, 1, 1);
  
  histo2D_ee_ttBkg->Add( histo2D_ee_ttBkg, histo2D_mumu_ttBkg, 1, 1);
  histo2D_ee_ttBkg->Add( histo2D_ee_ttBkg, histo2D_emu_ttBkg  , 1, 1);
  
  histo2D_ee_Zjets->Add( histo2D_ee_Zjets, histo2D_mumu_Zjets, 1, 1);
  histo2D_ee_Zjets->Add( histo2D_ee_Zjets, histo2D_emu_Zjets  , 1, 1);
  
  histo2D_ee_VV->Add( histo2D_ee_VV, histo2D_mumu_VV, 1, 1);
  histo2D_ee_VV->Add( histo2D_ee_VV, histo2D_emu_VV  , 1, 1);
  
  histo2D_ee_tW->Add( histo2D_ee_tW, histo2D_mumu_tW, 1, 1);
  histo2D_ee_tW->Add( histo2D_ee_tW, histo2D_emu_tW  , 1, 1);
  
  histo2D_ee_Data->Add( histo2D_ee_Data, histo2D_mumu_Data, 1, 1);
  histo2D_ee_Data->Add( histo2D_ee_Data, histo2D_emu_Data  , 1, 1);
  
  
  
  
  
  //******************************
  //create 1D histograms
  //******************************
  
  
  TH1D* histo1D = new TH1D("histo","",11,0,11);
  histo1D->GetXaxis()->SetBinLabel(1,string("(2,0)").c_str());
  histo1D->GetXaxis()->SetBinLabel(2,string("(2,1)").c_str());
  histo1D->GetXaxis()->SetBinLabel(3,string("(2,2)").c_str());
  histo1D->GetXaxis()->SetBinLabel(4,string("(3,0)").c_str());
  histo1D->GetXaxis()->SetBinLabel(5,string("(3,1)").c_str());
  histo1D->GetXaxis()->SetBinLabel(6,string("(3,2)").c_str());
  histo1D->GetXaxis()->SetBinLabel(7,string("(3,3)").c_str());
  histo1D->GetXaxis()->SetBinLabel(8,string("(#geq4,0)").c_str());
  histo1D->GetXaxis()->SetBinLabel(9,string("(#geq4,1)").c_str());
  histo1D->GetXaxis()->SetBinLabel(10,string("(#geq4,2)").c_str());
  histo1D->GetXaxis()->SetBinLabel(11,string("(#geq4,#geq3)").c_str());
  //histo1D->SetLineColor(0);
  histo1D->GetXaxis()->SetTitle("(N_{jets},N_{btagjets})");
  histo1D->GetYaxis()->SetTitle("Number of Events");
  histo1D->GetYaxis()->SetRangeUser(0.,2500.);
    
  histo1D_Data->GetXaxis()->SetBinLabel(1,string("(2,0)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(2,string("(2,1)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(3,string("(2,2)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(4,string("(3,0)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(5,string("(3,1)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(6,string("(3,2)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(7,string("(3,3)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(8,string("(#geq4,0)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(9,string("(#geq4,1)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(10,string("(#geq4,2)").c_str());
  histo1D_Data->GetXaxis()->SetBinLabel(11,string("(#geq4,#geq3)").c_str());
  //histo1D->SetLineColor(0);
  histo1D_Data->GetXaxis()->SetTitle("(N_{jets},N_{btagjets})");
  histo1D_Data->GetYaxis()->SetTitle("Number of Events");
  histo1D_Data->GetYaxis()->SetRangeUser(0.,2500.);
    
    
    
  for(int ibin = 3; ibin <6; ibin++){
	int max = 4;
	if(ibin>3) max = 5;
	for(int jbin = 1; jbin<max;jbin++){
	  double bin1D = (ibin-3)*3+jbin;
	  histo1D_ttSig->SetBinContent(bin1D, histo2D_ee_ttSig->GetBinContent(ibin,jbin));
	  histo1D_ttBkg->SetBinContent(   bin1D, histo2D_ee_ttBkg->GetBinContent(ibin,jbin)   );
	  histo1D_VV->SetBinContent(      bin1D, histo2D_ee_VV->GetBinContent(ibin,jbin)      );
	  histo1D_tW->SetBinContent(      bin1D, histo2D_ee_tW->GetBinContent(ibin,jbin)      );
	  histo1D_DY->SetBinContent(      bin1D, histo2D_ee_Zjets->GetBinContent(ibin,jbin)   );
	  histo1D_Data->SetBinContent(    bin1D, histo2D_ee_Data->GetBinContent(ibin,jbin)    );
	}
   }
  
  
  //******************************
  //add histograms
  //******************************
  
  /*
  histo1D_ee_TTbarSig->Add(histo1D_ee_TTbarSig, histo1D_mumu_TTbarSig, 1, 1);
  histo1D_ee_TTbarSig->Add(histo1D_ee_TTbarSig, histo1D_emu_TTbarSig, 1, 1);
  
  histo1D_ee_ttBk->Add(histo1D_ee_ttBkg, histo1D_mumu_ttBkg, 1, 1);
  histo1D_ee_ttBk->Add(histo1D_ee_ttBkg, histo1D_emu_ttBkg, 1, 1);
  
  histo1D_ee_VV->Add(histo1D_ee_VV, histo1D_mumu_VV, 1, 1);
  histo1D_ee_VV->Add(histo1D_ee_VV, histo1D_emu_VV, 1, 1);
  
  histo1D_ee_tW->Add(histo1D_ee_tW, histo1D_mumu_tW, 1, 1);
  histo1D_ee_tW->Add(histo1D_ee_tW, histo1D_emu_tW, 1, 1);
  
  histo1D_ee_DY->Add(histo1D_ee_DY, histo1D_mumu_DY, 1, 1);
  histo1D_ee_DY->Add(histo1D_ee_DY, histo1D_emu_DY, 1, 1);
  
  histo1D_ee_Data->Add(histo1D_ee_Data, histo1D_mumu_Data, 1, 1);
  histo1D_ee_Data->Add(histo1D_ee_Data, histo1D_emu_Data, 1, 1);
  */
  
  TH1D* histo1D_mc = (TH1D*) histo1D_ttSig->Clone();
  histo1D_mc->Add(histo1D_mc, histo1D_ttBkg ,1, 1);
  histo1D_mc->Add(histo1D_mc, histo1D_VV ,1, 1);
  histo1D_mc->Add(histo1D_mc, histo1D_tW ,1, 1);
  histo1D_mc->Add(histo1D_mc, histo1D_DY ,1, 1);
  
  TH1F * lumiband = (TH1F*) histo1D_mc->Clone();
  
  for (int ilum=0; ilum<lumiband->GetNbinsX(); ilum++) {    
	  
	  
	  
	  
	  
	  double error_all = 
       pow( histo1D_ttSig->GetBinContent(ilum+1)*lumi_error, 2)+
       //*************************
       //uncertinty on trigger eff
       pow(histo1D_ttSig->GetBinContent(ilum+1)*SF_trigger_error, 2)+
       //*************************
       //uncertinty on lepton sel
       pow(histo1D_ttSig->GetBinContent(ilum+1)*SF_Lepton_error, 2)+
       //*************************
       //uncertinty on met sel
       pow( histo1D_ttSig->GetBinContent(ilum+1)*SF_MET_error, 2);
     
       error_all += pow(histo1D_ttSig->GetBinContent(ilum+1)*systeError_ttbar, 2);   
       if(ilum > 2) error_all += pow(histo1D_ttSig->GetBinContent(ilum+1)*0.01, 2);    
       if(ilum > 6) error_all += pow(histo1D_ttSig->GetBinContent(ilum+1)*0.02, 2);   
       error_all += pow(histo1D_VV->GetBinContent(ilum+1)*systeError_VV, 2);
       error_all += pow(histo1D_tW->GetBinContent(ilum+1)*systeError_stop, 2);
     
        
       //*************************
       //uncertinty on met sel
       if(ilum < 5) pow( histo1D_ttSig->GetBinContent(ilum+1)*0.02, 2);
       if(ilum >= 5 && ilum < 8) pow( histo1D_ttSig->GetBinContent(ilum+1)*0.03, 2);
       if(ilum >= 8)             pow( histo1D_ttSig->GetBinContent(ilum+1)*0.04, 2);
	  
	
	    
	    //frac_ee   = frac_ee/  (frac_ee+frac_mumu+frac_emu);
	    //frac_mumu = frac_mumu/(frac_ee+frac_mumu+frac_emu);
	    //frac_emu  = frac_emu/ (frac_ee+frac_mumu+frac_emu);
	    
	    //cout << "frac_ee   " << frac_ee << endl;
	    //cout << "frac_mumu " << frac_mumu << endl;
	    //cout << "frac_emu  " << frac_emu << endl;
	    
	   error_all += pow(histo1D_DY->GetBinContent(ilum+1)*((0.40/1.9281)*frac_ee + (0.37/1.82219)*frac_mumu+  (0.30/1.38872)*frac_emu), 2);
	   
     lumiband->SetBinError(ilum+1,sqrt(error_all));
     
     //modifications
     histo1D_mc->SetBinError(ilum+1,sqrt(error_all));
  }
  
  
  TGraphErrors *thegraph = new TGraphErrors(lumiband);
  thegraph->SetFillStyle(3005);
  thegraph->SetFillColor(1);
  
  histo1D_ttSig->SetFillStyle(1001);
  histo1D_ttBkg->SetFillStyle(1001);
  histo1D_DY->SetFillStyle(1001);
  histo1D_VV->SetFillStyle(1001);
  histo1D_tW->SetFillStyle(1001);
    
  histo1D_ttSig->SetFillColor(kRed+1);
  histo1D_ttBkg->SetFillColor(kRed-7);
  histo1D_DY->SetFillColor(kAzure-2);
  histo1D_VV->SetFillColor(13);
  histo1D_tW->SetFillColor(kMagenta);
  
  
  histo1D_ttSig->GetYaxis()->CenterTitle();
  histo1D_ttSig->GetYaxis()->SetTitle("");
  histo1D_ttSig->GetXaxis()->SetLabelSize(0);
  histo1D_ttSig->GetXaxis()->SetTitleSize(0);
  
  histo1D_ttBkg->GetYaxis()->CenterTitle();
  histo1D_ttBkg->GetYaxis()->SetTitle("");
  histo1D_ttBkg->GetXaxis()->SetLabelSize(0);
  histo1D_ttBkg->GetXaxis()->SetTitleSize(0);
  
  histo1D_VV->GetYaxis()->CenterTitle();
  histo1D_VV->GetYaxis()->SetTitle("");
  histo1D_VV->GetXaxis()->SetLabelSize(0);
  histo1D_VV->GetXaxis()->SetTitleSize(0);
  
  histo1D_tW->GetYaxis()->CenterTitle();
  histo1D_tW->GetYaxis()->SetTitle("");
  histo1D_tW->GetXaxis()->SetLabelSize(0);
  histo1D_tW->GetXaxis()->SetTitleSize(0);
  
  histo1D_DY->GetYaxis()->CenterTitle();
  histo1D_DY->GetYaxis()->SetTitle("");
  histo1D_DY->GetXaxis()->SetLabelSize(0);
  histo1D_DY->GetXaxis()->SetTitleSize(0);
  
  
  THStack* hs= new THStack();
  hs->Add(histo1D_ttSig);
  hs->Add(histo1D_ttBkg);
  //hs->Add(histo_Wjets);
  hs->Add(histo1D_VV);
  hs->Add(histo1D_tW);
  hs->Add(histo1D_DY);
  
  for (int ibin=0; ibin<lumiband->GetNbinsX(); ibin++) {    
    //hs->GetXaxis()->SetBinLabel(ibin+1, "");
  }
  
  //hs->GetXaxis()->SetLabelSize(0.);
      
  histo1D_Data->GetXaxis()->SetTitle("");
  histo1D_Data->GetYaxis()->SetTitle("");
  histo1D_Data->GetYaxis()->CenterTitle();
  histo1D_Data->GetYaxis()->SetTitle("");
  histo1D_Data->GetXaxis()->SetLabelSize(0);
  histo1D_Data->GetXaxis()->SetTitleSize(0);
  histo1D_Data->SetMarkerStyle(20);
  
  
  histo1D->SetMaximum(2000);
  histo1D->SetMinimum(0);
  
  hs->SetMaximum(2000);
  
  
  TH1D* histo_ratio = (TH1D*) histo1D_Data->Clone();
  
  
  histo1D_Data->GetYaxis()->CenterTitle();
  histo1D_Data->GetYaxis()->SetTitle("");
  histo1D_Data->GetXaxis()->SetLabelSize(0);
  histo1D_Data->GetXaxis()->SetTitleSize(0);
  
  
  
  /*histo1D_mc- >GetYaxis()->CenterTitle();
  histo1D_mc->GetYaxis()->SetTitle("");
  histo1D_mc->GetXaxis()->SetLabelSize(0);
  histo1D_mc->GetXaxis()->SetTitleSize(0);
  */
  histo1D->GetYaxis()->CenterTitle();
  histo1D->GetYaxis()->SetTitle("");
  histo1D->GetXaxis()->SetLabelSize(0);
  histo1D->GetXaxis()->SetTitleSize(0);
  
  
  histo_ratio->GetYaxis()->SetLabelSize(0.1);histo_ratio->GetYaxis()->SetLabelSize(0.1);
  
  histo1D->Draw();
  hs->Draw();
  
  for (int ibin=0; ibin<lumiband->GetNbinsX(); ibin++) {    
    hs->GetXaxis()->SetBinLabel(ibin+1, "");
  }
  
  hs->GetXaxis()->SetLabelSize(0.0);
  hs->GetYaxis()->SetLabelSize(0.04);
  
  
  histo_ratio->GetYaxis()->SetLabelSize(0.1);
  histo1D->Draw("epsame");
  histo1D_Data->SetMarkerSize(1.2);
  histo1D_Data->Draw("epsame");
  //histo1D_Data->Draw("");
  
  thegraph->Draw("e2same");
  histo1D->Draw("same");
  
  
  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.04);
  latex->SetTextAlign(31); 
  latex->DrawLatex(0.45, 0.95, " ");
  
  
  
  TLatex *latex2 = new TLatex();
  latex2->SetNDC();
  latex2->SetTextSize(0.04);
  latex2->SetTextAlign(31); 
  latex2->DrawLatex(0.87, 0.95, "CMS 2.3 fb^{-1} at #sqrt{s} = 7 TeV");
  
  TString  info_data = "ee, #mu#mu, e#mu channels";
 
  TLatex* text2 = new TLatex(0.45,0.98, info_data);
  text2->SetNDC();
  text2->SetTextAlign(13);
  text2->SetX(0.18);
  text2->SetY(0.92);
  //text2->SetLineWidth(2);
  text2->SetTextFont(42);
  text2->SetTextSize(0.0610687);
  //    text2->SetTextSizePixels(24);// dflt=28
  text2->Draw();

  
  
  
  TLegend* qw = new TLegend(.80,.60,.95,.90);
  
  
  qw->SetShadowColor(0);
  qw->SetFillColor(0);
  qw->SetLineColor(0);
  
  
  //  qw->SetHeader("CMS Preliminary, 881.8 pb^{-1}");
  //  qw->AddEntry(histo_Data,         info_data,                "p");
  qw->AddEntry(histo1D_Data,         "Data" ,                "ep");
  qw->AddEntry(histo1D_DY,        "DY "                  ,"f");
  qw->AddEntry(histo1D_tW,  "tW "                  ,"f");
  qw->AddEntry(histo1D_VV,           "VV "                  ,"f");
  //qw->AddEntry(histo_Wjets,        "W  "                  ,"f");
  //qw->AddEntry(histo_TTbarBkg,     "t#bar{t} other  "     ,"f");
  qw->AddEntry(histo1D_ttBkg,     "non-prompt lepton  "     ,"f");
  qw->AddEntry(histo1D_ttSig,     "t#bar{t} signal "     ,"f");
  qw->SetFillColor(0);
  qw->SetTextFont(42);
  qw->Draw();

  
  
  
  
  
  TPad *canvas_2 = new TPad("canvas_2", "canvas_2", 0.0, 0.0, 1.0, 1.0);
    canvas_2->SetTopMargin(0.7);
    canvas_2->SetFillColor(0);
    canvas_2->SetFillStyle(0);
    canvas_2->SetGridy(1);
    canvas_2->Draw();
    canvas_2->cd(0);
    //gPad->SetBottomMargin(0.375);
    //gPad->SetGridy();
    
    //cout << " 721 histo_ratio->GetTitle()  " << histo_ratio->GetXaxis()->GetTitle() << endl;;
    
    //TH1D* histo_ratio = (TH1D*) histo1D_Data->Clone();
    histo_ratio->SetTitle("");
   
    histo_ratio->SetMarkerStyle(20);
    histo_ratio->SetMarkerSize(1.2);
    histo_ratio->SetMaximum( 1.5 );
    histo_ratio->SetMinimum(0.5);
    histo_ratio->GetYaxis()->SetTitle("");
    histo_ratio->GetXaxis()->SetLabelSize(0.04);
    histo_ratio->GetYaxis()->SetLabelSize(0.03);
    histo_ratio->GetYaxis()->SetNdivisions(6);
    
    histo_ratio->GetYaxis()->SetTitleSize(0.03);
    histo_ratio->SetMarkerSize(1.2);
    //histo_ratio->GetYaxis()->SetNdivisions(5);
    //ratio.Draw("e")
    
    histo_ratio->Divide(histo1D_mc);
    
    
    histo_ratio->SetMinimum(0.5);
    histo_ratio->SetMaximum(1.5);
    histo_ratio->Draw("E1X0");
    

    c1->cd();
   
  

}

void Plot2Moi(){
Plot2_main("NjetsNBjets_6_0.244", "csvhe");

}
