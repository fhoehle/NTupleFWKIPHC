//******************************************************
// Compute CutFlowTables from Proof output 
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
#include <fstream>
#include <vector>
#include <TLegend.h>
#include <TGraphErrors.h>




void CutFlow(){


  
//   double SF_DY_mm = 1.27;
//   double SF_DY_ee = 1.39;
//   double SF_DY_em = 1.00;
//   double SF_DY_mm = 1.00;
//   double SF_DY_ee = 1.00;
//   double SF_DY_em = 1.00;
  /*double SF_DY_mm = 1.95;
  double SF_DY_ee = 2.04;
  double SF_DY_em = 1.14;*/
  
  /*
  double SF_DY_AfterJet_ee = 1.59;
  double SF_DY_AfterJet_mm = 1.51;
  double SF_DY_AfterJet_em = 1.32;
  
  double SF_DY_AfterMet_ee = 1.59;
  double SF_DY_AfterMet_mm = 1.51;
  double SF_DY_AfterMet_em = 1.32;
  
  double SF_DY_AfterBTag1_ee = 1.59;
  double SF_DY_AfterBTag1_mm = 1.51;
  double SF_DY_AfterBTag1_em = 1.32;
  
  double SF_DY_AfterBTag2_ee = 1.59;
  double SF_DY_AfterBTag2_mm = 1.51;
  double SF_DY_AfterBTag2_em = 1.32;*/
  
  double SF_DY_mm = 1.;
  double SF_DY_ee = 1.;
  double SF_DY_em = 1.;
  
  double SF_Wjets_ee = 1.;
  double SF_Wjets_mm = 1.;
  double SF_Wjets_em = 1.;
  
  double SF_QCD_ee = 1.;
  double SF_QCD_mm = 1.;
  double SF_QCD_em = 1.;
  
  double TriggError[3]      = {0.014, 0.016, 0.007};
  double SF_Lepton_error[3] = {0.004,  0.005, 0.003};
  double SF_MET_error[3]    = { 0.0107397, 0.0156524, 0.};



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
  
  
//  TFile *f_data  = new TFile("proof.root");
//  TFile *f_data  = new TFile("proof_mysample_all.root");
//  TFile *f_data  = new TFile("proof_marcosample_all.root");
//  TFile *f_data  = new TFile("proof_marcosample_all_SFDYequal1.root");
//  TFile *f_data  = new TFile("proof_marcosample_allSF_2178.root");
 // TFile *f_data  = new TFile("../backup_outputProof04-11-11_15-45-51/proof.root");
 // TFile *f_data  = new TFile("../backup_outputProof04-11-11_17-45-51/proof.root");
  //TFile *f_data  = new TFile("../backup_outputProof14-01-12_21-43-31/proof.root");
  //TFile *f_data  = new TFile("../backup_outputProof19-01-12_22-39-57_allSFbutDY//proof.root");
  //TFile *f_data  = new TFile("../backup_outputProof26-01-12_19-38-49_forcutFlow_and_Plots//proof.root");
  //TFile *f_data  = new TFile("../backup_outputProof26-01-12_17-35-58_noDYdebug_enfin/proof.root");
  //TFile *f_data  = new TFile("../backup_outputProof01-02-12_20-27-49_AllSF/proof.root");
  TFile *f_data  = new TFile("../backup_outputProof21-02-12_17-49-14//proof.root");

  f_data->cd();
  
  
  //define tables [channel][process][cut step]
  double TabFlow1[5][103][101];
  double TabFlow2[5][103][101];


  for(int k0=0; k0<5; ++k0) {
    for(int k1=0; k1<103; k1++) {
      for(int k2=0; k2<101; ++k2) {
        TabFlow1[k0][k1][k2]=0.;
        TabFlow2[k0][k1][k2]=0.;
      }
    } 
  } 
  
// mumu   
   TH1F *  CutFlow_mumu_DataMu     = (TH1F*)gROOT->FindObject("CutFlow_mumu_DataMu");
   if ( CutFlow_mumu_DataMu==NULL ) std::cout<<"WARNING "<<"CutFlow_mumu_DataMu empty"<<std::endl;
   TH1F *  CutFlow_mumu_TTbarSig   = (TH1F*)gROOT->FindObject("CutFlow_mumu_TTbarSig");
   if ( CutFlow_mumu_TTbarSig==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_TTbarSig empty"<<std::endl;
   TH1F *  CutFlow_mumu_TTbarBkg   = (TH1F*)gROOT->FindObject("CutFlow_mumu_TTbarBkg");
   if ( CutFlow_mumu_TTbarBkg==NULL) std::cout<<"WARNING CutFlow_mumu_TTbarBkg"<<" empty"<<std::endl;
   TH1F *  CutFlow_mumu_Zjets      = (TH1F*)gROOT->FindObject("CutFlow_mumu_Zjets");
   if (CutFlow_mumu_Zjets ==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_Zjets empty"<<std::endl;
   TH1F *  CutFlow_mumu_Wjets      = (TH1F*)gROOT->FindObject("CutFlow_mumu_Wjets");
   if ( CutFlow_mumu_Wjets==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_Wjets empty"<<std::endl;
   TH1F *  CutFlow_mumu_TtW        = (TH1F*)gROOT->FindObject("CutFlow_mumu_TtW");
   if ( CutFlow_mumu_TtW==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_TtW empty"<<std::endl;
   TH1F *  CutFlow_mumu_TbartW     = (TH1F*)gROOT->FindObject("CutFlow_mumu_TbartW");
   if ( CutFlow_mumu_TbartW==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_TbartW empty"<<std::endl;
   TH1F *  CutFlow_mumu_WW         = (TH1F*)gROOT->FindObject("CutFlow_mumu_WW");
   if ( CutFlow_mumu_WW==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_WW empty"<<std::endl;
   TH1F *  CutFlow_mumu_WZ         = (TH1F*)gROOT->FindObject("CutFlow_mumu_WZ");
   if ( CutFlow_mumu_WZ==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_WZ empty"<<std::endl;
   TH1F *  CutFlow_mumu_ZZ         = (TH1F*)gROOT->FindObject("CutFlow_mumu_ZZ");
   if ( CutFlow_mumu_ZZ==NULL) std::cout<<"WARNING "<<"CutFlow_mumu_ZZ empty"<<std::endl;
   TH1F *  CutFlow_mumu_DYToLL_M_10To50      = (TH1F*)gROOT->FindObject("CutFlow_mumu_DYToLL_M10-50");
   if ( CutFlow_mumu_DYToLL_M_10To50==NULL) std::cout<<"WARNING CutFlow_mumu_DYToLL_M10-50"<<" empty"<<std::endl;

   
   TH1F *  ErrCutFlow_mumu_DataMu     = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_DataMu");
   if ( ErrCutFlow_mumu_DataMu==NULL ) std::cout<<"WARNING "<<"ErrCutFlow_mumu_DataMu empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_TTbarSig   = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_TTbarSig");
   if ( ErrCutFlow_mumu_TTbarSig==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_TTbarSig empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_TTbarBkg   = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_TTbarBkg");
   if ( ErrCutFlow_mumu_TTbarBkg==NULL) std::cout<<"WARNING ErrCutFlow_mumu_TTbarBkg"<<" empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_Zjets      = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_Zjets");
   if (ErrCutFlow_mumu_Zjets ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_Zjets empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_Wjets      = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_Wjets");
   if ( ErrCutFlow_mumu_Wjets==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_Wjets empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_TtW        = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_TtW");
   if ( ErrCutFlow_mumu_TtW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_TtW empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_TbartW     = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_TbartW");
   if ( ErrCutFlow_mumu_TbartW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_TbartW empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_WW         = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_WW");
   if ( ErrCutFlow_mumu_WW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_WW empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_WZ         = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_WZ");
   if ( ErrCutFlow_mumu_WZ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_WZ empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_ZZ         = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_ZZ");
   if ( ErrCutFlow_mumu_ZZ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_mumu_ZZ empty"<<std::endl;
   TH1F *  ErrCutFlow_mumu_DYToLL_M_10To50      = (TH1F*)gROOT->FindObject("ErrCutFlow_mumu_DYToLL_M10-50");
   if ( ErrCutFlow_mumu_DYToLL_M_10To50==NULL) std::cout<<"WARNING ErrCutFlow_mumu_DYToLL_M10-50"<<" empty"<<std::endl;

   
//ee
   TH1F *  CutFlow_ee_DataEG     = (TH1F*)gROOT->FindObject("CutFlow_ee_DataEG");
   if ( CutFlow_ee_DataEG==NULL ) std::cout<<"WARNING "<<"CutFlow_ee_DataEG empty"<<std::endl;
   TH1F *  CutFlow_ee_TTbarSig   = (TH1F*)gROOT->FindObject("CutFlow_ee_TTbarSig");
   if ( CutFlow_ee_TTbarSig==NULL) std::cout<<"WARNING "<<"CutFlow_ee_TTbarSig empty"<<std::endl;
   TH1F *  CutFlow_ee_TTbarBkg   = (TH1F*)gROOT->FindObject("CutFlow_ee_TTbarBkg");
   if ( CutFlow_ee_TTbarBkg==NULL) std::cout<<"WARNING CutFlow_ee_TTbarBkg"<<" empty"<<std::endl;
   TH1F *  CutFlow_ee_Zjets      = (TH1F*)gROOT->FindObject("CutFlow_ee_Zjets");
   if (CutFlow_ee_Zjets ==NULL) std::cout<<"WARNING "<<"CutFlow_ee_Zjets empty"<<std::endl;
   TH1F *  CutFlow_ee_Wjets      = (TH1F*)gROOT->FindObject("CutFlow_ee_Wjets");
   if ( CutFlow_ee_Wjets==NULL) std::cout<<"WARNING "<<"CutFlow_ee_Wjets empty"<<std::endl;
   TH1F *  CutFlow_ee_TtW        = (TH1F*)gROOT->FindObject("CutFlow_ee_TtW");
   if ( CutFlow_ee_TtW==NULL) std::cout<<"WARNING "<<"CutFlow_ee_TtW empty"<<std::endl;
   TH1F *  CutFlow_ee_TbartW     = (TH1F*)gROOT->FindObject("CutFlow_ee_TbartW");
   if ( CutFlow_ee_TbartW==NULL) std::cout<<"WARNING "<<"CutFlow_ee_TbartW empty"<<std::endl;
   TH1F *  CutFlow_ee_WW         = (TH1F*)gROOT->FindObject("CutFlow_ee_WW");
   if ( CutFlow_ee_WW==NULL) std::cout<<"WARNING "<<"CutFlow_ee_WW empty"<<std::endl;
   TH1F *  CutFlow_ee_WZ         = (TH1F*)gROOT->FindObject("CutFlow_ee_WZ");
   if ( CutFlow_ee_WZ==NULL) std::cout<<"WARNING "<<"CutFlow_ee_WZ empty"<<std::endl;
   TH1F *  CutFlow_ee_ZZ         = (TH1F*)gROOT->FindObject("CutFlow_ee_ZZ");
   if ( CutFlow_ee_ZZ==NULL) std::cout<<"WARNING "<<"CutFlow_ee_ZZ empty"<<std::endl;
   TH1F *  CutFlow_ee_DYToLL_M_10To50      = (TH1F*)gROOT->FindObject("CutFlow_ee_DYToLL_M10-50");
   if ( CutFlow_ee_DYToLL_M_10To50==NULL) std::cout<<"WARNING CutFlow_ee_DYToLL_M10-50"<<" empty"<<std::endl;
   
   TH1F *  ErrCutFlow_ee_DataEG     = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_DataEG");
   if ( ErrCutFlow_ee_DataEG==NULL ) std::cout<<"WARNING "<<"ErrCutFlow_ee_DataEG empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_TTbarSig   = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_TTbarSig");
   if ( ErrCutFlow_ee_TTbarSig==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_TTbarSig empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_TTbarBkg   = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_TTbarBkg");
   if ( ErrCutFlow_ee_TTbarBkg==NULL) std::cout<<"WARNING ErrCutFlow_ee_TTbarBkg"<<" empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_Zjets      = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_Zjets");
   if (ErrCutFlow_ee_Zjets ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_Zjets empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_Wjets      = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_Wjets");
   if ( ErrCutFlow_ee_Wjets==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_Wjets empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_TtW        = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_TtW");
   if ( ErrCutFlow_ee_TtW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_TtW empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_TbartW     = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_TbartW");
   if ( ErrCutFlow_ee_TbartW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_TbartW empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_WW         = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_WW");
   if ( ErrCutFlow_ee_WW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_WW empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_WZ         = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_WZ");
   if ( ErrCutFlow_ee_WZ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_WZ empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_ZZ         = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_ZZ");
   if ( ErrCutFlow_ee_ZZ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_ee_ZZ empty"<<std::endl;
   TH1F *  ErrCutFlow_ee_DYToLL_M_10To50      = (TH1F*)gROOT->FindObject("ErrCutFlow_ee_DYToLL_M10-50");
   if ( ErrCutFlow_ee_DYToLL_M_10To50==NULL) std::cout<<"WARNING ErrCutFlow_ee_DYToLL_M10-50"<<" empty"<<std::endl;
   
// emu   
   TH1F *  CutFlow_emu_DataMuEG     = (TH1F*)gROOT->FindObject("CutFlow_emu_DataMuEG");
   if ( CutFlow_emu_DataMuEG==NULL ) std::cout<<"WARNING "<<"CutFlow_emu_DataMuEG empty"<<std::endl;
   TH1F *  CutFlow_emu_TTbarSig   = (TH1F*)gROOT->FindObject("CutFlow_emu_TTbarSig");
   if ( CutFlow_emu_TTbarSig==NULL) std::cout<<"WARNING "<<"CutFlow_emu_TTbarSig empty"<<std::endl;
   TH1F *  CutFlow_emu_TTbarBkg   = (TH1F*)gROOT->FindObject("CutFlow_emu_TTbarBkg");
   if ( CutFlow_emu_TTbarBkg==NULL) std::cout<<"WARNING CutFlow_emu_TTbarBkg"<<" empty"<<std::endl;
   TH1F *  CutFlow_emu_Zjets      = (TH1F*)gROOT->FindObject("CutFlow_emu_Zjets");
   if (CutFlow_emu_Zjets ==NULL) std::cout<<"WARNING "<<"CutFlow_emu_Zjets empty"<<std::endl;
   TH1F *  CutFlow_emu_Wjets      = (TH1F*)gROOT->FindObject("CutFlow_emu_Wjets");
   if ( CutFlow_emu_Wjets==NULL) std::cout<<"WARNING "<<"CutFlow_emu_Wjets empty"<<std::endl;
   TH1F *  CutFlow_emu_TtW        = (TH1F*)gROOT->FindObject("CutFlow_emu_TtW");
   if ( CutFlow_emu_TtW==NULL) std::cout<<"WARNING "<<"CutFlow_emu_TtW empty"<<std::endl;
   TH1F *  CutFlow_emu_TbartW     = (TH1F*)gROOT->FindObject("CutFlow_emu_TbartW");
   if ( CutFlow_emu_TbartW==NULL) std::cout<<"WARNING "<<"CutFlow_emu_TbartW empty"<<std::endl;
   TH1F *  CutFlow_emu_WW         = (TH1F*)gROOT->FindObject("CutFlow_emu_WW");
   if ( CutFlow_emu_WW==NULL) std::cout<<"WARNING "<<"CutFlow_emu_WW empty"<<std::endl;
   TH1F *  CutFlow_emu_WZ         = (TH1F*)gROOT->FindObject("CutFlow_emu_WZ");
   if ( CutFlow_emu_WZ==NULL) std::cout<<"WARNING "<<"CutFlow_emu_WZ empty"<<std::endl;
   TH1F *  CutFlow_emu_ZZ         = (TH1F*)gROOT->FindObject("CutFlow_emu_ZZ");
   if ( CutFlow_emu_ZZ==NULL) std::cout<<"WARNING "<<"CutFlow_emu_ZZ empty"<<std::endl;
   TH1F *  CutFlow_emu_DYToLL_M_10To50      = (TH1F*)gROOT->FindObject("CutFlow_emu_DYToLL_M10-50");
   if ( CutFlow_emu_DYToLL_M_10To50==NULL) std::cout<<"WARNING CutFlow_emu_DYToLL_M10-50"<<" empty"<<std::endl;
   
   TH1F *  ErrCutFlow_emu_DataMuEG     = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_DataMuEG");
   if ( ErrCutFlow_emu_DataMuEG==NULL ) std::cout<<"WARNING "<<"ErrCutFlow_emu_DataMuEG empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_TTbarSig   = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_TTbarSig");
   if ( ErrCutFlow_emu_TTbarSig==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_TTbarSig empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_TTbarBkg   = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_TTbarBkg");
   if ( ErrCutFlow_emu_TTbarBkg==NULL) std::cout<<"WARNING ErrCutFlow_emu_TTbarBkg"<<" empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_Zjets      = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_Zjets");
   if (ErrCutFlow_emu_Zjets ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_Zjets empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_Wjets      = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_Wjets");
   if ( ErrCutFlow_emu_Wjets==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_Wjets empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_TtW        = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_TtW");
   if ( ErrCutFlow_emu_TtW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_TtW empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_TbartW     = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_TbartW");
   if ( ErrCutFlow_emu_TbartW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_TbartW empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_WW         = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_WW");
   if ( ErrCutFlow_emu_WW==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_WW empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_WZ         = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_WZ");
   if ( ErrCutFlow_emu_WZ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_WZ empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_ZZ         = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_ZZ");
   if ( ErrCutFlow_emu_ZZ==NULL) std::cout<<"WARNING "<<"ErrCutFlow_emu_ZZ empty"<<std::endl;
   TH1F *  ErrCutFlow_emu_DYToLL_M_10To50      = (TH1F*)gROOT->FindObject("ErrCutFlow_emu_DYToLL_M10-50");
   if ( ErrCutFlow_emu_DYToLL_M_10To50==NULL) std::cout<<"WARNING ErrCutFlow_emu_DYToLL_M10-50"<<" empty"<<std::endl;
   
// SFtrigger  
   TH1F *  SFtrigger     = (TH1F*)gROOT->FindObject("SFtrigger");
   if ( SFtrigger==NULL ) std::cout<<"WARNING "<<"SFtrigger empty"<<std::endl;
  
  std::cout<<"================================="<<std::endl;
  std::cout<<"SF_DY_mm = "<<SF_DY_mm<<std::endl;
  std::cout<<"SF_DY_ee = "<<SF_DY_ee<<std::endl;
  std::cout<<"SF_DY_em = "<<SF_DY_em<<std::endl;
  std::cout<<"================================="<<std::endl;
  std::string rep = "";
  std::cout << "Inputs : SF DY corrects ? [y/n] "<<std::endl;
  cin >> rep ;
  if ( rep=="y" ){ 
 
   
  //*********************************
  // 
  //*********************************
  
  for(int i=0; i<12; ++i) {
  
// mumu channel  
     if ( CutFlow_mumu_TTbarSig!=NULL ) { 
        TabFlow1[0][0][i] = CutFlow_mumu_TTbarSig->GetBinContent(i+1);
	TabFlow2[0][0][i] = ErrCutFlow_mumu_TTbarSig->GetBinContent(i+1);
     }	
     if ( CutFlow_mumu_TTbarBkg!=NULL ) { 
        TabFlow1[0][1][i] = CutFlow_mumu_TTbarBkg->GetBinContent(i+1);
        TabFlow2[0][1][i] = ErrCutFlow_mumu_TTbarBkg->GetBinContent(i+1);
     }	
  
     if ( CutFlow_mumu_Zjets!=NULL ) {
        TabFlow1[0][2][i] = CutFlow_mumu_Zjets->GetBinContent(i+1);
	TabFlow2[0][2][i] = ErrCutFlow_mumu_Zjets->GetBinContent(i+1);
     }		 
     if ( CutFlow_mumu_DYToLL_M_10To50!=NULL ) {
         TabFlow1[0][2][i] += CutFlow_mumu_DYToLL_M_10To50->GetBinContent(i+1) ;
	 TabFlow2[0][2][i] += ErrCutFlow_mumu_DYToLL_M_10To50->GetBinContent(i+1) ; 
     }	 

 
    
     if ( CutFlow_mumu_Wjets!=NULL ) { 
        TabFlow1[0][3][i] = CutFlow_mumu_Wjets->GetBinContent(i+1);
	TabFlow2[0][3][i] = ErrCutFlow_mumu_Wjets->GetBinContent(i+1);
     }	
      	
     
     if ( CutFlow_mumu_TtW!=NULL ) { 
        TabFlow1[0][4][i] = CutFlow_mumu_TtW->GetBinContent(i+1);
	TabFlow2[0][4][i] = ErrCutFlow_mumu_TtW->GetBinContent(i+1);
     }	
     if ( CutFlow_mumu_TbartW!=NULL ) {
        TabFlow1[0][4][i] += CutFlow_mumu_TbartW->GetBinContent(i+1);
	TabFlow2[0][4][i] += ErrCutFlow_mumu_TbartW->GetBinContent(i+1);
     }	
     if ( CutFlow_mumu_WZ!=NULL ) {
        TabFlow1[0][5][i] = CutFlow_mumu_WZ->GetBinContent(i+1) ;
	TabFlow2[0][5][i] = ErrCutFlow_mumu_WZ->GetBinContent(i+1) ;
     }	
     if ( CutFlow_mumu_ZZ!=NULL ) { 
        TabFlow1[0][5][i] += CutFlow_mumu_ZZ->GetBinContent(i+1) ;
	TabFlow2[0][5][i] += ErrCutFlow_mumu_ZZ->GetBinContent(i+1) ;
     }	
     if ( CutFlow_mumu_WW!=NULL ) { 
        TabFlow1[0][5][i] += CutFlow_mumu_WW->GetBinContent(i+1) ;
	TabFlow2[0][5][i] += ErrCutFlow_mumu_WW->GetBinContent(i+1) ;
     }	
     
     if ( CutFlow_mumu_DataMu!=NULL ) {
        TabFlow1[0][100][i] = CutFlow_mumu_DataMu->GetBinContent(i+1);
	TabFlow2[0][100][i] = ErrCutFlow_mumu_DataMu->GetBinContent(i+1);
     }	
     

// eechannel
     if ( CutFlow_ee_TTbarSig!=NULL ) { 
        TabFlow1[1][0][i] = CutFlow_ee_TTbarSig->GetBinContent(i+1);
	TabFlow2[1][0][i] = ErrCutFlow_ee_TTbarSig->GetBinContent(i+1);
     }	
     if ( CutFlow_ee_TTbarBkg!=NULL ) { 
        TabFlow1[1][1][i] = CutFlow_ee_TTbarBkg->GetBinContent(i+1);
        TabFlow2[1][1][i] = ErrCutFlow_ee_TTbarBkg->GetBinContent(i+1);
     }	
  
     if ( CutFlow_ee_Zjets!=NULL ) {
        TabFlow1[1][2][i] = CutFlow_ee_Zjets->GetBinContent(i+1);
	TabFlow2[1][2][i] = ErrCutFlow_ee_Zjets->GetBinContent(i+1);
     }	
     if ( CutFlow_ee_DYToLL_M_10To50!=NULL ) {
         TabFlow1[1][2][i] += CutFlow_ee_DYToLL_M_10To50->GetBinContent(i+1) ;
	 TabFlow2[1][2][i] += ErrCutFlow_ee_DYToLL_M_10To50->GetBinContent(i+1) ; 
     } 
    
     if ( CutFlow_ee_Wjets!=NULL ) { 
        TabFlow1[1][3][i] = CutFlow_ee_Wjets->GetBinContent(i+1);
	TabFlow2[1][3][i] = ErrCutFlow_ee_Wjets->GetBinContent(i+1);
     }	
      	
     
     if ( CutFlow_ee_TtW!=NULL ) { 
        TabFlow1[1][4][i] = CutFlow_ee_TtW->GetBinContent(i+1);
	TabFlow2[1][4][i] = ErrCutFlow_ee_TtW->GetBinContent(i+1);
     }	
     if ( CutFlow_ee_TbartW!=NULL ) {
        TabFlow1[1][4][i] += CutFlow_ee_TbartW->GetBinContent(i+1);
	TabFlow2[1][4][i] += ErrCutFlow_ee_TbartW->GetBinContent(i+1);
     }	
     if ( CutFlow_ee_WZ!=NULL ) {
        TabFlow1[1][5][i] = CutFlow_ee_WZ->GetBinContent(i+1) ;
	TabFlow2[1][5][i] = ErrCutFlow_ee_WZ->GetBinContent(i+1) ;
     }	
     if ( CutFlow_ee_ZZ!=NULL ) { 
        TabFlow1[1][5][i] += CutFlow_ee_ZZ->GetBinContent(i+1) ;
	TabFlow2[1][5][i] += ErrCutFlow_ee_ZZ->GetBinContent(i+1) ;
     }	
     if ( CutFlow_ee_WW!=NULL ) { 
        TabFlow1[1][5][i] += CutFlow_ee_WW->GetBinContent(i+1) ;
	TabFlow2[1][5][i] += ErrCutFlow_ee_WW->GetBinContent(i+1) ;
     }	
     
     if ( CutFlow_ee_DataEG!=NULL ) {
        TabFlow1[1][100][i] = CutFlow_ee_DataEG->GetBinContent(i+1);
	TabFlow2[1][100][i] = ErrCutFlow_ee_DataEG->GetBinContent(i+1);
     }	
     
// emu channel  
     if ( CutFlow_emu_TTbarSig!=NULL ) { 
        TabFlow1[2][0][i] = CutFlow_emu_TTbarSig->GetBinContent(i+1);
	TabFlow2[2][0][i] = ErrCutFlow_emu_TTbarSig->GetBinContent(i+1);
     }	
     if ( CutFlow_emu_TTbarBkg!=NULL ) { 
        TabFlow1[2][1][i] = CutFlow_emu_TTbarBkg->GetBinContent(i+1);
        TabFlow2[2][1][i] = ErrCutFlow_emu_TTbarBkg->GetBinContent(i+1);
     }	
  
     if ( CutFlow_emu_Zjets!=NULL ) {
        TabFlow1[2][2][i] = CutFlow_emu_Zjets->GetBinContent(i+1);
	TabFlow2[2][2][i] = ErrCutFlow_emu_Zjets->GetBinContent(i+1);
     }	
     if ( CutFlow_emu_DYToLL_M_10To50!=NULL ) {
         TabFlow1[2][2][i] += CutFlow_emu_DYToLL_M_10To50->GetBinContent(i+1) ;
	 TabFlow2[2][2][i] += ErrCutFlow_emu_DYToLL_M_10To50->GetBinContent(i+1) ; 
     }	
     if ( CutFlow_emu_Wjets!=NULL ) { 
        TabFlow1[2][3][i] = CutFlow_emu_Wjets->GetBinContent(i+1);
	TabFlow2[2][3][i] = ErrCutFlow_emu_Wjets->GetBinContent(i+1);
     }	
      	
     
     if ( CutFlow_emu_TtW!=NULL ) { 
        TabFlow1[2][4][i] = CutFlow_emu_TtW->GetBinContent(i+1);
	TabFlow2[2][4][i] = ErrCutFlow_emu_TtW->GetBinContent(i+1);
     }	
     if ( CutFlow_emu_TbartW!=NULL ) {
        TabFlow1[2][4][i] += CutFlow_emu_TbartW->GetBinContent(i+1);
	TabFlow2[2][4][i] += ErrCutFlow_emu_TbartW->GetBinContent(i+1);
     }	
     if ( CutFlow_emu_WZ!=NULL ) {
        TabFlow1[2][5][i] = CutFlow_emu_WZ->GetBinContent(i+1) ;
	TabFlow2[2][5][i] = ErrCutFlow_emu_WZ->GetBinContent(i+1) ;
     }	
     if ( CutFlow_emu_ZZ!=NULL ) { 
        TabFlow1[2][5][i] += CutFlow_emu_ZZ->GetBinContent(i+1) ;
	TabFlow2[2][5][i] += ErrCutFlow_emu_ZZ->GetBinContent(i+1) ;
     }	
     if ( CutFlow_emu_WW!=NULL ) { 
        TabFlow1[2][5][i] += CutFlow_emu_WW->GetBinContent(i+1) ;
	TabFlow2[2][5][i] += ErrCutFlow_emu_WW->GetBinContent(i+1) ;
     }	
     
     if ( CutFlow_emu_DataMuEG!=NULL ) {
        TabFlow1[2][100][i] = CutFlow_emu_DataMuEG->GetBinContent(i+1);
	TabFlow2[2][100][i] = ErrCutFlow_emu_DataMuEG->GetBinContent(i+1);
     }	
     
	  
	  
  }
  
  
  cout<<"#########################"<<endl;
  cout<<" Fill the latex tables   "<<endl;
  cout<<"#########################"<<endl;
  
//  string ofilenametex = string("CrossSection")+string("_")+ChannelName+string(".tex");
//  string ofilenametex = string("CrossSection")+string(".tex");

  string ofilenametex = "CutFlow.tex";
  ofstream ofile(ofilenametex.c_str());
  
  ofile<<"\\documentclass[amsmath,amssymb]{revtex4}"<<endl;
  ofile<<"\\begin{document}"<<endl;
  
  ofile.setf(ios::fixed);
  ofile.precision(1);
  
  vector<string> CutName;  // for same flavour leptons
  CutName.push_back("Total");
  CutName.push_back("Trigger");
  CutName.push_back("$ll$ pair, M($ll$)$>$20 GeV$/c^2$ and VxP"); 
  CutName.push_back("M($ll$) $\\notin$ [76,106]");
  CutName.push_back("$E_T(jet1,2)$ $>$ 30 GeV");
  CutName.push_back("$\\not\\!\\!E_T$ $>$ 40 GeV");
  CutName.push_back("btag ($\\ge$1)"); 
  CutName.push_back("btag ($\\ge$2)"); 
  vector<string> CutName2;
  CutName2.push_back("Total");
  CutName2.push_back("Trigger");
  CutName2.push_back("$ll$ pair, M($ll$)$>$12 GeV$/c^2$ and VxP");
  CutName2.push_back(" - ");
  CutName2.push_back("$E_T(jet1,2)$ $>$ 30 GeV");
  CutName2.push_back(" - ");
  CutName2.push_back("btag ($\\ge$1)"); 
  CutName2.push_back("btag ($\\ge$2)"); 

  //compute error
  for(int k0=0; k0<5; ++k0) {
    for(int k1=0; k1<101; ++k1) {
      for(int k2=0; k2<101; ++k2) {
         //if data driven, don't usr square root
	if( (k1 == 2 && k2 >= 4) || k1 == 1 ) TabFlow2[k0][k1][k2]= TabFlow2[k0][k1][k2];
	else{
	  if (k1==0 || k1==2 || k1==4 || k1==5   ){       
	    TabFlow2[k0][k1][k2]= TabFlow2[k0][k1][k2] 
	  				+ pow(0.045*TabFlow1[k0][k1][k2], 2) 
	  				+ pow(TriggError[k0]*TabFlow1[k0][k1][k2], 2) 
	  				+ pow(SF_Lepton_error[k0]  *TabFlow1[k0][k1][k2], 2) ;
	   if(k2 >= 5)  TabFlow2[k0][k1][k2] +=  pow(SF_MET_error[k0], 2) ;
	   TabFlow2[k0][k1][k2]  = sqrt(TabFlow2[k0][k1][k2]);
	  }
	  else{ TabFlow2[k0][k1][k2]= sqrt(TabFlow2[k0][k1][k2] );}
	}
	//TabFlow2[k0][k1][k2]= sqrt(TabFlow2[k0][k1][k2]);
      }
    }
  }  
  
  
  //**************************
  //Compute total bckgd
  for(int k0=0; k0<5; ++k0) {
    for(int k1=1; k1<10; ++k1) {
      for(int k2=0; k2<50; ++k2) {
	TabFlow1[k0][50][k2] += TabFlow1[k0][k1][k2];
      }
    }
  } 
  
  //*********************************
  //Compute error for total bckgd
  for(int k0=0; k0<5; ++k0) {
    for(int k2=1; k2<51; ++k2) {
      for(int k1=1; k1<10; ++k1) {
	TabFlow2[k0][50][k2] += TabFlow2[k0][k1][k2]*TabFlow2[k0][k1][k2];
      }
      TabFlow2[k0][50][k2] = sqrt(TabFlow2[k0][50][k2]);
    }
  } 

  //******************************
  //compute trigger
  //******************************
  float sf_trig_sum[3];
  float sf_id_sum[3];
  for (int ijl=0; ijl<3; ijl++) {
    if ((TabFlow1[ijl][0][10]+TabFlow1[ijl][50][10])>0)
       sf_trig_sum[ijl] = (TabFlow1[ijl][0][1]+TabFlow1[ijl][50][1])/(TabFlow1[ijl][0][10]+TabFlow1[ijl][50][10]);
    else sf_trig_sum[ijl] =0;
   if ((TabFlow1[ijl][0][11]+TabFlow1[ijl][50][11])>0)
      sf_id_sum[ijl]= (TabFlow1[ijl][0][2]+TabFlow1[ijl][50][2])/(TabFlow1[ijl][0][11]+TabFlow1[ijl][50][11]);
   else sf_id_sum[ijl] =0;
  }
  //float sf_trig_sum[3];
  sf_trig_sum[0] = SFtrigger->GetBinContent(1)/SFtrigger->GetBinContent(11); // mumu
  sf_trig_sum[1] = SFtrigger->GetBinContent(2)/SFtrigger->GetBinContent(12); // ee
  sf_trig_sum[2] = SFtrigger->GetBinContent(3)/SFtrigger->GetBinContent(13); // emu
  std::cout<<"sf_trig_sum[0,1,2] "<<sf_trig_sum[0]<<" "<<sf_trig_sum[1]<<" "<<sf_trig_sum[2]<<std::endl;
  
  
  //******************************
  //compute SF_ID
  //******************************
  for(int k0=0; k0<3; ++k0) {
    for(int k2=0; k2<12; ++k2) {
      int ityp=k2;
      if (k2==10) ityp=50;
      if (k2==11) ityp=100;
      if (TabFlow1[k0][ityp][10]>0) TabFlow1[k0][ityp][10] = TabFlow1[k0][ityp][1]/TabFlow1[k0][ityp][10];
      if (TabFlow1[k0][ityp][11]>0) TabFlow1[k0][ityp][11] = TabFlow1[k0][ityp][2]/TabFlow1[k0][ityp][11];
      
      if (TabFlow1[k0][ityp][11]>0) TabFlow2[k0][ityp][11] = sqrt(TabFlow2[k0][ityp][11])/TabFlow1[k0][ityp][2];
      
    }
  } 

  for (int IChannel=0; IChannel<3; IChannel++) {
  
  // Summary tables
  ofile << "\\clearpage" << endl;
  ofile << "\\begin{landscape}" << endl;
  ofile << "\\begin{table}[p]" << endl;
  
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & DATA & Sum MC & TT signal  & Total Background & S/B \\\\" << endl;
  ofile << "\\hline" << endl;
  
  for(int ic=0; ic<CutName.size(); ++ic) {
   if (IChannel<2) {
   double SoverB = 0;
   if (TabFlow1[IChannel][50][ic]>0 ) SoverB = TabFlow1[IChannel][0][ic]/TabFlow1[IChannel][50][ic];
    ofile <<CutName[ic]<<" & "<<  TabFlow1[IChannel][100][ic] << " $\\pm$ "<<  TabFlow2[IChannel][100][ic] << " & " <<
     TabFlow1[IChannel][0][ic]+TabFlow1[IChannel][50][ic] << " $\\pm$ "<< 
     sqrt(TabFlow2[IChannel][0][ic]*TabFlow2[IChannel][0][ic]+TabFlow2[IChannel][50][ic]*TabFlow2[IChannel][50][ic]) << " & " <<
     TabFlow1[IChannel][0][ic] << " $\\pm$ "<< TabFlow2[IChannel][0][ic] << " & " <<
     TabFlow1[IChannel][50][ic] << " $\\pm$ "<< TabFlow2[IChannel][50][ic] << " & " <<
     SoverB <<  " \\\\" << endl;
   }
   else {// emu
    ofile <<CutName2[ic]<<" & "<<  TabFlow1[IChannel][100][ic] << " $\\pm$ "<<  TabFlow2[IChannel][100][ic] << " & " <<
     TabFlow1[IChannel][0][ic]+TabFlow1[IChannel][50][ic] << " $\\pm$ "<< 
     sqrt(TabFlow2[IChannel][0][ic]*TabFlow2[IChannel][0][ic]+TabFlow2[IChannel][50][ic]*TabFlow2[IChannel][50][ic]) << " & " <<
     TabFlow1[IChannel][0][ic] << " $\\pm$ "<< TabFlow2[IChannel][0][ic] << " & " <<
     TabFlow1[IChannel][50][ic] << " $\\pm$ "<< TabFlow2[IChannel][50][ic] << " & " <<
     TabFlow1[IChannel][0][ic]/TabFlow1[IChannel][50][ic] <<  " \\\\" << endl;
   }
  }
  
// summary table
  ofile.precision(3);
  ofile << "\\hline" << endl;
// ajout Caro
 // ofile << "$SF_{trig}$  & " << TabFlow1[IChannel][100][10] <<  " & " <<
 //     sf_trig_sum[IChannel] <<  " & " <<
 //     sf_trig_sum[IChannel] << " & " <<
 //     sf_trig_sum[IChannel] <<  " &  \\\\" << endl;
 // ofile << "$SF_{ID}$  & " << TabFlow1[IChannel][100][11] << " & " <<
 //     sf_id_sum[IChannel] << " & " <<
 //     TabFlow1[IChannel][0][11]   << " $\\pm $ " << TabFlow2[IChannel][0][11]  << " & " <<
 //     TabFlow1[IChannel][50][11]  << " $\\pm $ " << TabFlow2[IChannel][50][11] << " &  \\\\" << endl;
 // ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
// end ajout Caro
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  //ofile << "\\begin{tabular}{|l|c|c|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & Fake Rate Backg & SingleTop & DY & Diboson  \\\\" << endl;
  //ofile << "Cut & TopBackg & SingleTop & DY & Wjets  & Diboson & QCD  \\\\" << endl;
  ofile << "\\hline" << endl;
  
  for(int ic=0; ic<CutName.size(); ++ic) {
    double TopBkgd    = TabFlow1[IChannel][1][ic];
    double ErrTopBkgd = TabFlow2[IChannel][1][ic];
    // to be changed
    //    double DY = DY + mll<50;
    double DY         = TabFlow1[IChannel][2][ic];
    double errDY      = TabFlow2[IChannel][2][ic];
    //double errDY      = (TabFlow2[IChannel][2][ic]*TabFlow2[IChannel][2][ic]);
    //errDY             = sqrt(errDY);
    double Wj         = TabFlow1[IChannel][3][ic];
    double errWjets   = TabFlow2[IChannel][3][ic];
    double Qcd        = TabFlow1[IChannel][8][ic];
    double errQcd     = TabFlow2[IChannel][8][ic];
    double SinglTop    = TabFlow1[IChannel][4][ic];
    double errSinglTop = sqrt(TabFlow2[IChannel][4][ic]*TabFlow2[IChannel][4][ic]);
    double Dibos      =  TabFlow1[IChannel][5][ic];
    double errDibos   = TabFlow2[IChannel][5][ic];
   
  ofile.precision(1);
    if (IChannel<2) {
     ofile <<CutName[ic]<<" & "<<
     TopBkgd         << " $\\pm$ "<< ErrTopBkgd      << " & " <<
     SinglTop        << " $\\pm$ "<< errSinglTop     << " & " <<
     DY              << " $\\pm$ "<< errDY           << " & " <<
     //Wj              << " $\\pm$ "<< errWjets        << " & " <<
     //Dibos           << " $\\pm$ "<< errDibos        << " & " <<
     Dibos           << " $\\pm$ "<< errDibos        << " \\\\" << endl;
     //Qcd             << " $\\pm$ "<< errQcd <<   " \\\\" << endl;
    }
    else { // emu
     ofile <<CutName2[ic]<<" & "<<
     TopBkgd         << " $\\pm$ "<< ErrTopBkgd      << " & " <<
     SinglTop        << " $\\pm$ "<< errSinglTop     << " & " <<
     DY              << " $\\pm$ "<< errDY           << " & " <<
     //Wj              << " $\\pm$ "<< errWjets        << " & " <<
     //Dibos           << " $\\pm$ "<< errDibos        << " & " <<
     Dibos           << " $\\pm$ "<< errDibos        << " \\\\" << endl;
     //Qcd             << " $\\pm$ "<< errQcd <<   " \\\\" << endl;
   } 
  }
  
  ofile.precision(3);
// mommentane!
  ofile << "\\hline " << endl;
// ajout Caro
 /* ofile << "$SF_{trig}$  & " << TabFlow1[IChannel][1][10] <<  " & "
     << sf_trig_sum[IChannel] <<  " & " 
    // << sf_trig_sum[IChannel] <<  " & " 
    // << sf_trig_sum[IChannel] <<  " & " 
     << sf_trig_sum[IChannel] <<  " & " 
     << sf_trig_sum[IChannel] <<  " \\\\" << endl;
  ofile << "$SF_{ID}$  & " << TabFlow1[IChannel][1][11] <<  " & "
     << TabFlow1[IChannel][4][11]  << " $\\pm $ " << TabFlow2[IChannel][4][11] <<  " & " 
     //<< TabFlow1[IChannel][2][11]  << " $\\pm $ " << TabFlow2[IChannel][2][11] <<  " & " 
     //<< TabFlow1[IChannel][3][11]  << " $\\pm $ " << TabFlow2[IChannel][3][11] <<  " & " 
     << TabFlow1[IChannel][5][11]  << " $\\pm $ " << TabFlow2[IChannel][5][11] <<  " & " 
     << TabFlow1[IChannel][8][11]  << " $\\pm $ " << TabFlow2[IChannel][8][11] <<  " \\\\" << endl;
  if (IChannel==0) ofile << "$SF_{DD}$  &  &  & " << SF_DY_mm << " & " << SF_Wjets_mm << " & & " << SF_QCD_mm << " \\\\" << endl;
  else if (IChannel==1) ofile << "$SF_{DD}$  &  &  & " << SF_DY_ee << " & " << SF_Wjets_ee << " & & " << SF_QCD_ee << " \\\\" << endl;
  else if (IChannel==2) ofile << "$SF_{DD}$  &  &  & " << SF_DY_em << " & " << SF_Wjets_em << " & & " << SF_QCD_em << " \\\\" << endl;
// end ajout Caro
*/
  //ofile << "\\hline " << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  
  string ChannelName;
  if (IChannel==0) ChannelName= "mumu"; 
  else if (IChannel==1) ChannelName= "ee"; 
  else if (IChannel==2) ChannelName= "emu"; 
  
  if ( ChannelName == "mumu" )  ofile << "\\caption{Dimuon cut flow, including SF for trigger efficiency, lepton and \\met selection. For the \\ttbar, diboson and single top samples, the uncertainties account for the uncertainty on the luminosity and the various selection efficiencies. The DY backgrounds is estimated from data.}" << endl;
  if ( ChannelName == "ee" )  ofile << "\\caption{ Dielectron cut flow, including SF for trigger efficiency, lepton and \\met selection. For the \\ttbar, diboson and single top samples, the uncertainties account for the uncertainty on the luminosity and the various selection efficiencies. The DY backgrounds is estimated from data.}" << endl;
  if ( ChannelName == "emu" )  ofile << "\\caption{ ElectronMuon cut flow, including SF for trigger efficiency, lepton and \\met selection. For the \\ttbar, diboson and single top samples, the uncertainties account for the uncertainty on the luminosity and the various selection efficiencies. The DY backgrounds is estimated from data.The DY backgrounds is estimated from data.  }" << endl;
  if ( ChannelName == "mumu" ) ofile << "\\label{Table:CutFlow_mumu}" << endl;
  if ( ChannelName == "ee"   ) ofile << "\\label{Table:CutFlow_ee}" << endl;
  if ( ChannelName == "emu"  ) ofile << "\\label{Table:CutFlow_emu}" << endl;
  
  
  
  ofile << "\\end{table}" << endl;
  ofile << "\\end{landscape}" << endl;
  } // end loop IChannel
  
  
  
  ofile<<"\\end{document}"<<endl;
//  string prodpdf = string("pdflatex CrossSection")+string("_")+ChannelName+string(".tex");
//  string prodpdf = string("pdflatex CrossSection")+string(".tex");
  string prodpdf = string("pdflatex ")+ofilenametex;
  system(prodpdf.c_str());
  
  }
  
  
 }
