#include <TH1F.h> 
#include <TFile.h> 
#include <TF1.h> 
#include <TLegend.h> 
#include <TMath.h> 
#include <THStack.h> 
#include <TCanvas.h> 
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
#include "TLegend.h"



void MTchecks(){


 gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13); 
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  gStyle->SetStatColor(0); // kWhite

  
  TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2); 
  c1->SetLogy(0);

  TFile *f_input1 = new TFile("backup_outputProof21-02-13_12-01-12/proof.root");
  
  
  TH1F * histo1 = (TH1F*)gROOT->FindObject("ttbarTransverseMass_emu_afterbtag1_TTbarSig");
  TH1F * histo2 = (TH1F*)gROOT->FindObject("GEN_ttbarTransverseMass_mumu_afterbtag1_TTbarSig");
  
  
  histo2->SetFillColor(5);
  histo1->SetMarkerSize(1.2);
  histo1->SetMarkerStyle(20);
  
  histo1->Sumw2();
  histo2->Sumw2();

  TLegend* qw = new TLegend(.15,.70,.35,.85);
  
  qw->SetShadowColor(0);
  qw->SetFillColor(0);
  qw->SetLineColor(0);
  //qw->AddEntry(histo2,        "ttbar MG5" ,    "ep");
  qw->AddEntry(histo2,        "m_{T} Gen" ,    "f");
  qw->AddEntry(histo1,        "m_{T} Reco","ep" );
 
  histo2->GetXaxis()->SetTitle("m_{T} [GeV]");
  histo2->DrawNormalized("h");
  histo1->DrawNormalized("epsame");
 
  qw->Draw();
 

}
