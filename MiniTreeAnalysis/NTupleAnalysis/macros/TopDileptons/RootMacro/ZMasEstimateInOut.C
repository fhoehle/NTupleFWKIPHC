//******************************************************
// estimate the Z background in ee and mumu
// using the R_outin method
// R_outin is estimated from the DY MC
// and corrected from data study (done using Rout_in.C)
//******************************************************



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


//*******************************************************
//calculate the DY contamination 
//for a bjet multiplicity nbjet and the channel 'channel'
// output :  cout << "channel   &  $R_{out/in}$ & DY MC   & DY pred. & SF \\\\" << endl;
//*******************************************************


void ZMasEstimateInOut(int nbjet, TString channel){
  

  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  
  //TString channel = "EE";
  TString q = "MET";
  bool isMC = true;


  //TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  //c1->SetFillColor(10);
  //c1->SetFillStyle(4000);
  //c1->SetBorderSize(2); 
  //c1->SetLogy(1);
 
  
  
  
  //***********************
  //Get histograms
  //***********************
   
  
//  TFile *f_data = new TFile("CrossSection_noWeights2.root");
//  TFile *f_data = new TFile("proof_marcosample_all.root");
  TFile *f_data = new TFile("../backup_outputProof19-01-12_12-51-59/proof.root");
  TH1F* zMass_Data;
  
  
  if(channel == "MM" && nbjet == 1 ) zMass_Data      = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM_mumu_DataMu");
  if(channel == "EE" && nbjet == 1 ) zMass_Data      = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM_ee_DataEG");
  
  if(channel == "MM" && nbjet == 0 ) zMass_Data      = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM0Btag_mumu_DataMu");
  if(channel == "EE" && nbjet == 0 ) zMass_Data      = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM0Btag_ee_DataEG");
  
  if(channel == "MM" && nbjet == 2 ) zMass_Data      = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM2Btag_mumu_DataMu");
  if(channel == "EE" && nbjet == 2 ) zMass_Data      = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM2Btag_ee_DataEG");
  
  
  
  
  TH1F* zMass_Data_EM ;
  if(nbjet == 2)        zMass_Data_EM    = (TH1F*)gROOT->FindObject("InvemuMassPair_aftermetbtag2_DataMuEG");
  if(nbjet == 1)        zMass_Data_EM    = (TH1F*)gROOT->FindObject("InvemuMassPair_aftermetbtag1_DataMuEG");
  if(nbjet == 0)        zMass_Data_EM    = (TH1F*)gROOT->FindObject("InvemuMassPair_aftermetcut_DataMuEG");
  
  
  //TH1F* zMass_Data_EM            = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM_emu_DataMuEG");
  TH1F* zMass_Data_Loose_MM      = (TH1F*)gROOT->FindObject("InvmumuMassPair_DataMu");
  TH1F* zMass_Data_Loose_EE      = (TH1F*)gROOT->FindObject("InveeMassPair_DataEG");
  
  
  
  
 // zMass_DataCT    = (TH1F*)gROOT->FindObject();
  
  //from MC
  TH1F* zMass_Zjets;
  TH1F* zMass_DYToLL_M_10To50;
  
  if(channel == "MM" ){
    if(nbjet == 1){
      zMass_Zjets               = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM_mumu_Zjets");
      zMass_DYToLL_M_10To50       = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM_mumu_DYToLL_M10-50");
    }
    if(nbjet == 0){
      zMass_Zjets               = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM0Btag_mumu_Zjets");
      zMass_DYToLL_M_10To50       = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM0Btag_mumu_DYToLL_M10-50");
    }
    if(nbjet == 2){
      zMass_Zjets               = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM2Btag_mumu_Zjets");
      zMass_DYToLL_M_10To50       = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM2Btag_mumu_DYToLL_M10-50");
    }
  
   
  }
  
  if(channel == "EE" ){
  
  
   if(nbjet == 1){
      zMass_Zjets               = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM_ee_Zjets");
      zMass_DYToLL_M_10To50       = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM_ee_DYToLL_M10-50");
    }
    if(nbjet == 0){
      zMass_Zjets               = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM0Btag_ee_Zjets");
      zMass_DYToLL_M_10To50       = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM0Btag_ee_DYToLL_M10-50");
    }
    if(nbjet == 2){
      zMass_Zjets               = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM2Btag_ee_Zjets");
      zMass_DYToLL_M_10To50       = (TH1F*)gROOT->FindObject("HInvM_ll_afterSelButInvM2Btag_ee_DYToLL_M10-50");
    }
  
  }
  
  
  zMass_Zjets->Sumw2();
  zMass_DYToLL_M_10To50->Sumw2();		
  
  
  
  zMass_Zjets->Add( zMass_Zjets, zMass_DYToLL_M_10To50      , 1, 1);
  
  
  //***********************
  //Calculation of Rout/in
  //***********************
  
  double nevent_In_MC     = 0;
  double nevent_InErr_MC  = 0;
  double nevent_Out_MC    = 0;
  double nevent_OutErr_MC = 0;
  
  for(unsigned int i=1; i<=351; i++){
    
    double ibin = i;
    
    if(( ibin < 76 || ibin >= 106 ) && ibin > 12 ) {
    	nevent_Out_MC    += zMass_Zjets->GetBinContent(i); 
	nevent_OutErr_MC += zMass_Zjets->GetBinError(i)*zMass_Zjets->GetBinError(i) ;
    }
    if(  ibin >= 76 && ibin < 106 ){
    	nevent_In_MC     += zMass_Zjets->GetBinContent(i); 
	nevent_InErr_MC  += zMass_Zjets->GetBinError(i)*zMass_Zjets->GetBinError(i) ;
    }
    
  }
  
  nevent_OutErr_MC = pow(nevent_OutErr_MC, 0.5);
  nevent_InErr_MC  = pow(nevent_InErr_MC,  0.5);
  
  
  double Routin = nevent_Out_MC/nevent_In_MC;
  
  //***********************
  //count N_in
  //***********************
  
  double nevent_In_Data  = 0;
  
  for(unsigned int i=1; i<=351; i++){
    
    double ibin = i;
    
    if( ibin >= 76 && ibin < 106 ) nevent_In_Data  += zMass_Data->GetBinContent(i);
    
  }
  //cout << "nevent_In_Data " << nevent_In_Data << endl;
  //cout << "nevent out zpeak " <<  zMass_Data->GetEntries()-nevent_In_Data << endl;
  
  	
  //***********************
  //Count N_in e-mu
  //***********************
  
  double nevent_In_Data_EM  = 0;
  
  /*for(unsigned int i=1; i<=50; i++){
    
    double ibin = i*350/50.;
    
    if( ibin > 76 && ibin < 106 )nevent_In_Data_EM  += zMass_Data_EM->GetBinContent(i);
    
  }*/
  
  
  for(unsigned int i=1; i<=351; i++){
    
    double ibin = i;
    
    if( ibin >= 76 && ibin < 106 )nevent_In_Data_EM  += zMass_Data_EM->GetBinContent(i);
    
  }
  
  
  
  
  //cout << "nevent_In_Data_EM  " << nevent_In_Data_EM<< endl;
  //cout << "nevent_In_Data_EM all " <<  zMass_Data_EM->GetEntries() << endl;
  //***********************
  //Calculation kee, kmumu
  //***********************
  
  
  double nevent_EE_Loose = zMass_Data_Loose_EE->GetEntries();
  double nevent_MM_Loose = zMass_Data_Loose_MM->GetEntries();
  
  double kFact = 0;
  if(channel == "EE") kFact = nevent_EE_Loose/nevent_MM_Loose;
  if(channel == "MM") kFact = nevent_MM_Loose/nevent_EE_Loose;
  
  //cout << "kFact " << kFact << endl;
  //***********************
  //estimate Z events
  //Routin corrected
  //***********************
// // original values from LP
//   if(channel == "EE") Routin = Routin*1.09*1.33;
//   if(channel == "MM") Routin = Routin*1.25*1.39*0.92;
  //if(channel == "EE") Routin = Routin*1.09*1.38*0.99;
  //if(channel == "MM") Routin = Routin*1.25*1.43*0.915;
  if(channel == "EE") Routin = Routin;
  if(channel == "MM") Routin = Routin;
  double theEstimate = Routin*(nevent_In_Data - 0.5*nevent_In_Data_EM*pow(kFact, 0.5));
  //cout << "Routin  " << Routin << endl;
  //cout << "the Z estimate in the " << channel << "  channel is " <<  theEstimate << endl;
  
  
  
  //***********************
  //erreur calculation
  //***********************
  
  
  //erreur sur Routin
  double err_Routin = pow(pow( pow(nevent_Out_MC, 0.5)/nevent_In_MC, 2)  + pow( nevent_Out_MC*pow(nevent_In_MC,0.5)/(nevent_In_MC*nevent_In_MC), 2), 0.5);
  if(channel == "EE") err_Routin += 0.12;
  if(channel == "MM") err_Routin += 0.11;
  double dN_DRoutin = (nevent_In_Data - 0.5*nevent_In_Data_EM*pow(kFact, 0.5))*err_Routin;
  
  //erreur sur nevent_In_Data
  double err_nevent_In_Data = pow( nevent_In_Data, 0.5);
  double dN_Dnevent_In_Data = Routin;
  
  //erreur sur nevent_In_Data_EM
  double err_nevent_In_Data_EM = pow(nevent_In_Data_EM , 0.5);
  double dN_Dnevent_In_Data_EM  = -0.5*Routin*pow(kFact, 0.5);
  
  //erreur sur kFact
  double err_kFact = 0;
  double dN_DkFact = 0;
  
  
  //cout <<  "dN_DRoutin            " << dN_DRoutin            << "  err_Routin            " << err_Routin << endl;
  //cout <<  "dN_Dnevent_In_Data    " << dN_Dnevent_In_Data    << "  err_nevent_In_Data    " << err_nevent_In_Data << endl;
  //cout <<  "dN_Dnevent_In_Data_EM " << dN_Dnevent_In_Data_EM << "  err_nevent_In_Data_EM " << err_nevent_In_Data_EM << endl;
  //cout <<  "dN_DkFact             " << dN_DkFact             << "  err_kFact             " <<  err_kFact<< endl;
  
  double stat_error = pow( 
  			   pow(dN_DRoutin*err_Routin, 2) +
  			   pow(dN_Dnevent_In_Data*err_nevent_In_Data, 2) +
  			   pow(dN_Dnevent_In_Data_EM*err_nevent_In_Data_EM, 2) +
  			   pow(dN_DkFact*err_kFact, 2) 
			 , 0.5);
  
  

  //cout << "estimated Z contamination for channel " << channel << " is " << theEstimate << " +/- " << stat_error << " (stat) " << endl;
 // cout << "ZSF is  " << channel << " is " <<  theEstimate/ << " +/- " << stat_error << " (stat) " << endl;
  //cout << "expected Z contamination for channel " << channel << " is " << nevent_Out_MC <<  " (stat) "  << nevent_OutErr_MC << endl;
  
  double SF_error = pow( 
  		pow(stat_error/nevent_Out_MC, 2) +
		pow(nevent_OutErr_MC/(nevent_Out_MC*nevent_Out_MC), 2)
		, 0.5);
  
  
  //cout << "scale factor is " << theEstimate/nevent_Out_MC  << " err stat " << SF_error<< endl;
  //cout << "expected Z contamination for channel " << channel << " is " << nevent_In_MC <<  " (stat) " << endl;

  if(nbjet == 0 &&  channel == "EE") 
  cout << "ee, NBjets $>=$0 & " <<  Routin << "$\\pm$" << err_Routin << " & "  << nevent_Out_MC << "$\\pm$" << nevent_OutErr_MC << " & " 
       << theEstimate << "$\\pm$" << stat_error << " & " << theEstimate/nevent_Out_MC << "$\\pm$" << SF_error << " \\\\" << endl;
  if(nbjet == 1 &&  channel == "EE") 
  cout << "ee, NBjets $>=$1 & " <<  Routin << "$\\pm$" << err_Routin << " & " << nevent_Out_MC << "$\\pm$" << nevent_OutErr_MC << " & " 
       << theEstimate << "$\\pm$" << stat_error << " & " << theEstimate/nevent_Out_MC << "$\\pm$" << SF_error << " \\\\" << endl;
  if(nbjet == 2 &&  channel == "EE") 
  cout << "ee, NBjets $>=$2 & " <<  Routin << "$\\pm$" << err_Routin << " & " << nevent_Out_MC << "$\\pm$" << nevent_OutErr_MC << " & " 
       << theEstimate << "$\\pm$" << stat_error << " & " << theEstimate/nevent_Out_MC << "$\\pm$" << SF_error << " \\\\" << endl;

  if(nbjet == 0 &&  channel == "MM") 
  cout << "$\\mu\\mu$, NBjets $>=$0 & " <<  Routin << "$\\pm$" << err_Routin << " & " << nevent_Out_MC << "$\\pm$" << nevent_OutErr_MC << " & " 
       << theEstimate << "$\\pm$" << stat_error << " & " << theEstimate/nevent_Out_MC << "$\\pm$" << SF_error << " \\\\" << endl;
  if(nbjet == 1 &&  channel == "MM") 
  cout << "$\\mu\\mu$, NBjets $>=$1 & " <<  Routin << "$\\pm$" << err_Routin << " & " << nevent_Out_MC << "$\\pm$" << nevent_OutErr_MC << " & " 
       << theEstimate << "$\\pm$" << stat_error << " & " << theEstimate/nevent_Out_MC << "$\\pm$" << SF_error << " \\\\" << endl;
  if(nbjet == 2 &&  channel == "MM") 
  cout << "$\\mu\\mu$, NBjets $>=$2 & " <<  Routin << "$\\pm$" << err_Routin << " & " << nevent_Out_MC << "$\\pm$" << nevent_OutErr_MC << " & " 
       << theEstimate << "$\\pm$" << stat_error << " & " << theEstimate/nevent_Out_MC << "$\\pm$" << SF_error << " \\\\" << endl;
  
}


//*********************************************************************
// calculate the DY contamination 
// dump a tex table with the 2 channels and various b-jet multiplicity
//*********************************************************************

void dumpTable(){
  
  cout << "\\begin{table} " << endl;
  cout << "\\begin{tabular}{|l|c|c|c|c|} " << endl;
  cout << "\\hline " << endl;
  cout << "channel   &  $R_{out/in}$ & DY MC   & DY pred. & SF \\\\" << endl;
  cout << "\\hline " << endl;
  ZMasEstimateInOut(0, "EE");
  cout << "\\hline " << endl;
  ZMasEstimateInOut(1, "EE");
  cout << "\\hline " << endl;
  ZMasEstimateInOut(2, "EE");
  cout << "\\hline " << endl;
  cout << "\\hline " << endl;
  ZMasEstimateInOut(0, "MM");
  cout << "\\hline " << endl;
  ZMasEstimateInOut(1, "MM");
  cout << "\\hline " << endl;
  ZMasEstimateInOut(2, "MM");
  cout << "\\hline " << endl;
  cout << "\\end{tabular}" << endl;
  cout << "\\end{table}" << endl;
}


