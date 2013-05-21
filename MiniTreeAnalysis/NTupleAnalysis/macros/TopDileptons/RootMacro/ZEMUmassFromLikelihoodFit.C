
//***************************************************************
//Compute  Z->tautau contribution in the emu channel
//Use a binned likelihood  fit of the dilepton emu invariant mass
//templates are from MC
//***************************************************************


#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooHistPdf.h"

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
#include <vector>
#include <TLegend.h>
using namespace RooFit ;


//using namespace RooFit; 
//using namespace RooStats; 

//***********************************
//to execute the  code :
//root -l ZmassFromLikelihoodFit.C+
//***********************************


void ZEMUmassFromLikelihoodFit(){
  double lumi = 34;

  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  
  TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2); 
  c1->SetLogy(0);
 
  
//  TFile *f_data  = new TFile("CrossSection_noWeights2.root");
  TFile *f_data  = new TFile("../backup_outputProof14-01-12_23-03-07/proof.root");
  f_data->cd();
  
  //*********************************************
  // get the histograms and prepare the sum of MC
  //*********************************************
  std::vector<TH1F*> histo_List;
  std::vector<TH1F*> histo_List_DY;
  std::vector<TString> histoName_List;
  histoName_List.push_back("TTbarSig");
  histoName_List.push_back("TTbarBkg");
  //histoName_List.push_back("Zjets");
  //histoName_List.push_back("DYToMuMu_M-20");
  //histoName_List.push_back("DYToEE_M-20");
  //histoName_List.push_back("DYToTauTau_M-20");
  //histoName_List.push_back("Wjets");
  //histoName_List.push_back("TtW");
  //histoName_List.push_back("TbartW");
  //histoName_List.push_back("WW");
  //histoName_List.push_back("ZZ");
  //histoName_List.push_back("WZ");
  
  std::vector<TString> histoName_List_DY;
  histoName_List_DY.push_back("Zjets");
  histoName_List_DY.push_back("DYToMuMu_M-20");
  histoName_List_DY.push_back("DYToEE_M-20");
  histoName_List_DY.push_back("DYToTauTau_M-20");
  
  
  
  TH1F *zMass_Data      = (TH1F*)gROOT->FindObject("InvemuMassPair_afterjetcut_DataMuEG");
  //tempHisto->Draw();
  
  //*********************************************
  // get and summ histograms for MC
  //*********************************************
  //*********************
  // Fill Histo Vector
  //*********************			
  for(unsigned int i=0; i<histoName_List.size(); i++){
    histo_List.push_back( (TH1F*)gROOT->FindObject(  ("InvemuMassPair_afterjetcut_"+histoName_List[i]).Data()  ));
    
  } 
  //*********************
  // Fill Histo Vector	
  //*********************		
  for(unsigned int i=0; i<histoName_List_DY.size(); i++){
    histo_List_DY.push_back( (TH1F*)gROOT->FindObject(  ("InvemuMassPair_afterjetcut_"+histoName_List_DY[i]).Data()) );
    
  } 
  
  
  
  //*********************
  // summ up MC templates
  //*********************
  TH1F * totMC = new TH1F("totMC", "totMC", zMass_Data->GetNbinsX() , zMass_Data->GetXaxis()->GetXmin() , zMass_Data->GetXaxis()->GetXmax());
  for(unsigned int i=0; i<histoName_List.size(); i++){
   histo_List[i]->Sumw2();
   totMC->Add( totMC, histo_List[i], 1, 1);
  }
  
  
  
  //*********************
  // summ up MC templates
  //*********************
  TH1F * totMC_DY = new TH1F("totMC_DY", "totMC_DY", zMass_Data->GetNbinsX() , zMass_Data->GetXaxis()->GetXmin() , zMass_Data->GetXaxis()->GetXmax());
  for(unsigned int i=0; i<histoName_List_DY.size(); i++){
   histo_List_DY[i]->Sumw2();
   totMC_DY->Add( totMC_DY, histo_List_DY[i], 1, 1);
  }
  
  
  
  
  zMass_Data->SetMarkerStyle(20);
  zMass_Data->Draw("ep");
  totMC->SetLineColor(2);
  totMC->SetLineWidth(1.2);
  totMC->Draw("hsame");
  totMC_DY->SetLineColor(4);
  totMC_DY->SetLineWidth(1.2);
  totMC_DY->Draw("hsame");
  zMass_Data->Draw("epsame");
  
  
  
  //*********************************************
  // create the RooFit environement
  RooWorkspace *w = new RooWorkspace("w",kTRUE) ;  

  //*********************************************
  // create a RooFit variable : 
  // the variable that will be used for the fit
  //*********************************************
  RooRealVar * LL_Zmass        = new RooRealVar("LL_Zmass", "M_{e#mu}", zMass_Data->GetXaxis()->GetXmin() , zMass_Data->GetXaxis()->GetXmax());
  
  //create RooDataHisto for the data (to be fitted)
  RooDataHist* histoLL_Zmass   = new RooDataHist("histoFit_LL", "histoFit_LL",  *LL_Zmass,  zMass_Data ); 
  //create RooDataHisto for the MC DY : will be used to create template 1
  RooDataHist* histoFit_Template_DYEM        = new RooDataHist("histoFit_Template_DYEM",       "histoFit_Template_DYEM", *LL_Zmass, totMC_DY);
  //create RooDataHisto for the MC DY : will be used to create template 2
  RooDataHist* histoFit_Template_OtherLL     = new RooDataHist("histoFit_Template_OtherLL",    "histoFit_Template_OtherLL", *LL_Zmass, totMC);        
  
  
  //convert  RooDataHisto into the template 1
  RooHistPdf*  histoFit_Template_DYEM_pdf    = new RooHistPdf("histoFit_Template_DYEM_pdf",    "histoFit_Template_DYEM_pdf",    *LL_Zmass,  *histoFit_Template_DYEM);
  
  
  //convert  RooDataHisto into the template 2
  RooHistPdf*  histoFit_Template_OtherLL_pdf    = new RooHistPdf("histoFit_Template_OtherLL_pdf",    "histoFit_Template_OtherLL_pdf",    *LL_Zmass,  *histoFit_Template_OtherLL);
  
  
  
  //define a coefficient : it is the output of the fit
  //it represents the contribution (fraction) of one of the 2 template with resepct to the data after the fit :
  // N(event ttbar) = coeff*N(event data)
  RooRealVar coeffModel("coeffModel", "coeffModel", 0.5, 0., 1.);
  //associate the templates, the data histo and the coeff.
  RooAddPdf *pdf_sum = new RooAddPdf("pdf_sum"," test pdf sum",RooArgSet(*histoFit_Template_DYEM_pdf, *histoFit_Template_OtherLL_pdf), coeffModel);
  //do the fit.
  RooFitResult* myFitResults_all = pdf_sum->fitTo(*histoLL_Zmass, Save()) ;
    
  
  
  //create a "frame" : a kind of TCanvas used to display the result of the fit
  RooPlot* frame = LL_Zmass->frame() ;
  //plot the data on the frame
  histoLL_Zmass->plotOn(frame) ;
  //plots the templates (after the fit) on the frame
  pdf_sum->plotOn(frame, Components(*histoFit_Template_DYEM_pdf), VisualizeError(*myFitResults_all), FillColor(kGreen), LineWidth(2)  );
  pdf_sum->plotOn(frame, Components(*histoFit_Template_OtherLL_pdf), VisualizeError(*myFitResults_all), FillColor(kRed),   LineWidth(2)  );
  pdf_sum->plotOn(frame, LineStyle(kDashed),                VisualizeError(*myFitResults_all), FillColor(kBlue), LineWidth(2) );
  histoLL_Zmass->plotOn(frame) ;
  //histoFit_all_loose->plotOn(frame_loose) ;
  
  //c1->SetLogy();
  
  //frame->Minimum(0.01);
  //draw the frame
  frame->Draw() ;
  //print the coeff after the fit
  coeffModel.Print() ;
  
  //calculate the various contributions
  cout << " the DY is " << coeffModel.getVal()*zMass_Data->Integral() << " +/- " << coeffModel.getError()*zMass_Data->Integral() << endl;
  cout << " the other " << (1-coeffModel.getVal())*zMass_Data->Integral() << " +/- " << coeffModel.getError()*zMass_Data->Integral() << endl;
  
  cout << " initial DY component " << totMC_DY->Integral() << " +/-" <<  totMC_DY->Integral() << endl;
  
  cout << "scale factor " << coeffModel.getVal()*zMass_Data->Integral()/totMC_DY->Integral() << "pm " <<
  coeffModel.getError()*zMass_Data->Integral()/totMC_DY->Integral()
   << endl;
  
     
 TH1F * histoFitDY    = new TH1F("histoFitDY",    "histoFitDY",    0, 1, 100);
 TH1F * histoFitOther = new TH1F("histoFitOther", "histoFitOther", 0, 1, 100);
 TH1F * histoFitTot = new TH1F("histoFitOther", "histoFitOther", 0, 1, 100);
  
  histoFitDY->SetFillColor(3);
  histoFitOther->SetFillColor(2);
  histoFitTot->SetFillColor(4);
  TLegend* qw = new TLegend(0.75,0.70,0.98,0.98);
  qw->AddEntry(zMass_Data,         "Data" ,                "p");
  qw->AddEntry(histoFitTot,    "result of the Fit" ,    "f");
  qw->AddEntry(histoFitDY,     "DY component" ,        "f");
  qw->AddEntry(histoFitOther,  "non DY component" ,    "f");
  
  
  qw->Draw();
  
  
  
  
  
}
