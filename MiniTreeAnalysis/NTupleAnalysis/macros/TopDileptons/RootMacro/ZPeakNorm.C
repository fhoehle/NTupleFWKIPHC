//******************************************************
// Calculate the luminosity
// Using the Z cross section from EWK-10-005
// And normalizing to the Z mass peak
//******************************************************



#include <TH1F.h> 
#include <TH2F.h> 
#include <TFile.h> 
#include <TF1.h> 
#include <TLegend.h> 
#include <TMath.h> 
#include <THStack.h> 
#include <TCanvas.h> 
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include <vector>
#include <TLegend.h>
#include <TGraphErrors.h>





void ZPeakNorm(){
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  
  //TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  //c1->SetFillColor(10);
  //c1->SetFillStyle(4000);
  //c1->SetBorderSize(2); 
  //c1->SetLogy(0);
 
  //*********************************
  //get histograms 
  //*********************************
  
  
  //TFile *f_data  = new TFile("../CrossSection.root");
  TFile *f_data  = new TFile("../backup_outputProof18-01-12_14-19-05_ref/proof.root");
  f_data->cd();
  
   TH1F *  AllZjets_ee   = (TH1F*)gROOT->FindObject("InveeMassPair_Zjets");
   TH1F *  AllZjets_mumu = (TH1F*)gROOT->FindObject("InvmumuMassPair_Zjets");
   
 
   TH1F *  InZpeak_ee   = (TH1F*)gROOT->FindObject("InveeMassPairzoom_Zjets");
   TH1F *  InZpeak_mumu = (TH1F*)gROOT->FindObject("InvmumuMassPairzoom_Zjets");
   
 
   
   TH1F *  InZpeak_ee_data   = (TH1F*)gROOT->FindObject("InveeMassPairzoom_DataEG");
   TH1F *  InZpeak_mumu_data = (TH1F*)gROOT->FindObject("InvmumuMassPairzoom_DataMu");
   
   
   //int entrie_ee   = InZpeak_ee->Integral()/AllZjets_ee->Integral();
 
  //*********************************
  // calculate the Zpeak
  //*********************************
   double entrie_mumu = InZpeak_mumu->Integral()/AllZjets_mumu->Integral();
   double Br = 1./3.;
   
   double norm_cross_section =1./(3048.*entrie_mumu*Br);
   double ewk_crosssection = 968.0;
   cout << "norm_cross_section "  <<  norm_cross_section << endl;
   
   //cout << "  " << InZpeak_mumu->Integral()*norm_cross_section*ewk_crosssection << endl;
   double N_predicted = InZpeak_mumu->Integral()*norm_cross_section*ewk_crosssection;
  
  
  
   //double N_Data =  InZpeak_mumu_data->Integral(1, InZpeak_mumu_data->GetNbinsX());
   double N_Data =  InZpeak_mumu_data->Integral();
   
  //*********************************
  // calculate error
  //*********************************
   double errorXS = InZpeak_mumu->Integral()*norm_cross_section*44.0;
   double error = pow( pow((1/N_predicted)*pow(N_Data,0.5),2) +  pow((N_Data/(N_predicted*N_predicted))*errorXS,2), 0.5);
   
   
    
   cout << "N_predicted " << N_predicted << endl;
   cout << "N_Data      " << N_Data      << endl; 
   cout << "lumi scale  " << N_Data/N_predicted  << " +/- "<<  error<< endl;
   cout << "new nlumi   " << (N_Data/N_predicted)*2.173  << " +/- "<<  (N_Data/N_predicted)*error<< endl;
  
  //qw->AddEntry(theGraphData,  "R_{out/in} Data" ,                "p");
  
  
  //qw->Draw();
 }
