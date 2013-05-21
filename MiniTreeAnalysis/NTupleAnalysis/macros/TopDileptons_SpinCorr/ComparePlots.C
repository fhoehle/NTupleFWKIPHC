
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



void ComparePlots(TString channel, TString quantity, TString cut){

  
  //***********************
  //Get histograms
  //***********************
   
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

  //TFile *f_input1 = new TFile("../backup_outputProof25-04-12_11-13-55_MCatNLObtagChecks/proof.root");
  //TFile *f_input2 = new TFile("../backup_outputProof18-05-12_00-30-22MCatNLObtagFastSim_noSpinCorr/proof.root");

  TFile *f_input1 = new TFile("backup_outputProof23-05-12_02-22-57_MCatNLO_SpinCorr/proof.root");
  TFile *f_input2 = new TFile("backup_outputProof23-05-12_10-38-59_MCatNLO_NoSpinCorr/proof.root");
   
  
  TString plotname = quantity + "_" + channel + "_"  +cut +  "_" + "TTbarSig";
  if(cut == "") plotname = quantity + "_" + channel + "_" + "TTbarSig";
  if(quantity =="Inv")  plotname = quantity  + channel + "MassPair_" + cut +  "_" +"TTbarSig";
  if(quantity =="Inv" && cut == "" )  plotname = quantity  + channel + "MassPair_" +"TTbarSig";
  
  cout << "plotname " << plotname << endl;
  
  f_input1->cd();
  
  TH1F * histo1 = (TH1F*)gROOT->FindObject(plotname.Data());
  
  f_input2->cd();
  
  TH1F * histo2 = (TH1F*)gROOT->FindObject(plotname.Data());
  
  
  
  
  histo1->SetFillColor(5);
  histo2->SetMarkerSize(1.2);
  histo2->SetMarkerStyle(20);
  
  histo1->Sumw2();
  histo2->Sumw2();
  
  TLegend* qw = 0;
  if(quantity == "Njets" || quantity == "NBjets" || quantity == "DeltaPhiLLept"  ) qw = new TLegend(.15,.70,.35,.85);
  else qw = new TLegend(.60,.70,.85,.85);
  
  qw->SetShadowColor(0);
  qw->SetFillColor(0);
  qw->SetLineColor(0);
  //qw->AddEntry(histo2,        "ttbar MG5" ,    "ep");
  qw->AddEntry(histo2,        "no spin corr" ,    "ep");
  qw->AddEntry(histo1,        "spin corr","f" );
 
  
  
  histo1->SetMinimum(0.0001);
  histo1->SetMaximum(histo2->GetMaximum()+0.2*histo2->GetMaximum());
  histo1->Draw("h");
  histo2->Draw("epsame");
  
  qw->Draw();
  plotname = "plots_Comp/"+plotname+".pdf";
  c1->SaveAs(plotname.Data());
  

} 


void ComparePlotsCutFlow(TString channel){
 //***********************
  //Get histograms
  //***********************
   
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

  TFile *f_input1 = new TFile("backup_outputProof23-05-12_02-22-57_MCatNLO_SpinCorr//proof.root");
  TFile *f_input2 = new TFile("backup_outputProof23-05-12_10-38-59_MCatNLO_NoSpinCorr//proof.root");
   
  TString plotname = "CutFlow_" + channel+ "_TTbarSig";
  
  
  f_input1->cd();
  
  TH1F * histo1 = (TH1F*)gROOT->FindObject(plotname.Data());
  
  f_input2->cd();
  
  TH1F * histo2 = (TH1F*)gROOT->FindObject(plotname.Data());
  
  
  histo1->SetFillColor(5);
  histo2->SetMarkerSize(1.2);
  histo2->SetMarkerStyle(20);
  
  histo1->Sumw2();
  histo2->Sumw2();
  
  
  histo1->Divide(histo1, histo2, 1, 1, "b" );
  
  histo1->Draw();
  plotname = "plots_Comp/"+plotname+".pdf";
  c1->SaveAs(plotname.Data());

}


void ComparePlots(){


 ComparePlots("emu", "CSVDiscriBjets", "aftermetcut");

 ComparePlots("mumu", "PtMu1", "");
 ComparePlots("mumu", "PtMu1", "");
 ComparePlots("mumu", "PtMu2", "");
 ComparePlots("ee", "PtEl1", "");
 ComparePlots("ee", "PtEl2", "");
 ComparePlots("ee", "Inv", "");
 ComparePlots("mumu", "Inv", "");
 ComparePlots("emu", "Inv", "");
 ComparePlots("emu", "Njets", "");
 ComparePlots("ee", "Njets", "");
 ComparePlots("mumu", "Njets", "");
 ComparePlots("emu", "NBjets", "");
 ComparePlots("ee", "NBjets", "");
 ComparePlots("mumu", "NBjets", "");
 ComparePlots("mumu", "Met", "afterjetcut");
 ComparePlots("ee", "Met", "afterjetcut");
 ComparePlots("emu", "Met", "afterjetcut");
 ComparePlots("ee", "PtJet1", "");
 ComparePlots("ee", "PtJet2", "");
 ComparePlots("mumu", "PtJet1", "");
 ComparePlots("mumu", "PtJet2", "");
 ComparePlots("emu", "PtJet2", "");
 ComparePlots("emu", "PtJet1", "");
 ComparePlots("emu", "DeltaPhiLLept", "aftermetcut");
ComparePlots("emu", "CSVDiscriBjets", "aftermetcut");
ComparePlots("ee", "CSVDiscriBjets", "aftermetcut");
ComparePlots("mumu", "CSVDiscriBjets", "aftermetcut");
ComparePlots("mumu", "CSVDiscri", "aftermetcut");
ComparePlots("ee", "CSVDiscri", "aftermetcut");
ComparePlots("mumu", "CSVDiscri", "aftermetcut");


ComparePlots("ee",   "DeltaPhiLLept", "afterbtag1");
ComparePlots("mumu", "DeltaPhiLLept", "afterbtag1");
ComparePlots("emu",  "DeltaPhiLLept", "afterbtag1");

ComparePlots("ee",   "DeltaPhiLLept", "afterbtag2");
ComparePlots("mumu", "DeltaPhiLLept", "afterbtag2");
ComparePlots("emu",  "DeltaPhiLLept", "afterbtag2");


ComparePlotsCutFlow("ee");
ComparePlotsCutFlow("mumu");
ComparePlotsCutFlow("emu");


}



