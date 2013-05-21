#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"


double calculErrorSF(double a, double delta_a, double b, double delta_b){


     double error = pow(
	pow(delta_a/a, 2.) +
	pow(delta_b*a/(b*b), 2.) ,
	0.5);
	return error;
}



double calculSF(double a,  double b){


     double sfact = a/b;
     //cout << "a " << a  << "  b " << b << " SF is " << sfact << endl;
     return sfact;
}










std::vector<pair<double, double> > plotTagAndProbEff(TString lepton, TString q, TString isoid, TString etaCut, ofstream *ofile, bool computeSF, int systType){
  
  std::vector<pair<double, double> > output;
  
  Int_t eventType = 1; // 0 MC, 1 Data
  
  Int_t stati=0;
  Bool_t  fit=1;
  Bool_t logy=0;
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  gStyle->SetOptDate(0);
  gStyle->SetStatColor(0);
  gStyle->SetTitleColor(1);
  //gStyle->SetTitleW(0.4);
  //gStyle->SetTitleH(0.07);
  //gStyle->SetOptStat(stati);
  gStyle->SetOptStat(0);
  // gStyle->SetFrameFillColor(18);
  //gStyle->SetPalette(51,0);
  gStyle->SetPalette(1);
  
  
  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
   
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2);
  
  //c1.Divide(2,2);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillColor(10);
  
  
  
  TFile *f_Data_El ; 
  TFile *f_Data_Mu ; 
  TFile *f_MC_Zjets; 
  
  if(systType == 0){
    f_Data_El = new TFile("../TagAndProbe//tagAndProbeHisto_DataEG.root"); 
    f_Data_Mu = new TFile("../TagAndProbe//tagAndProbeHisto_DataMu.root"); 
    f_MC_Zjets= new TFile("../TagAndProbe//tagAndProbeHisto_Zjets.root"); 
  }
  if(systType == -1){
    f_Data_El = new TFile("../TagAndProbe//tagAndProbeHisto_DataEG.root"); 
    f_Data_Mu = new TFile("../TagAndProbe//tagAndProbeHisto_DataMu.root"); 
    f_MC_Zjets= new TFile("../TagAndProbe//tagAndProbeHisto_Zjets.root"); 
  }
  if(systType == 1){
    f_Data_El = new TFile("../TagAndProbe///tagAndProbeHisto_DataEG.root"); 
    f_Data_Mu = new TFile("../TagAndProbe///tagAndProbeHisto_DataMu.root"); 
    f_MC_Zjets= new TFile("../TagAndProbe///tagAndProbeHisto_Zjets.root"); 
  }
  
  
  
  
  TH1F* tight_histo;
  TH1F* looseID_histo;
  TH1F* looseIso_histo;
  
  TH1F* tight_histo_LS;
  TH1F* looseID_histo_LS;
  TH1F* looseIso_histo_LS;
  
  TH2F* tight_histo_CP;
  TH2F* looseID_histo_CP;
  TH2F* looseIso_histo_CP;
  
  TH1F* tight_histo_TTbar    ;
  TH1F* looseID_histo_TTbar  ;
  TH1F* looseIso_histo_TTbar ;
   
   
  TH1F* tight_histo_Zjets    ;
  TH1F* looseID_histo_Zjets  ;
  TH1F* looseIso_histo_Zjets ;
      
   
  TLegend* qw = 0;
  qw = new TLegend(0.50,0.15,0.85,0.50);
 
  TGraphAsymmErrors * grID;
  TGraphAsymmErrors * grIso;
  TGraphAsymmErrors * grID_Zjets;
  TGraphAsymmErrors * grIso_Zjets;
  
  if(lepton == "Mu"){
    
    if(q == "pt"){
      f_Data_Mu->cd();
      tight_histo    = (TH1F*)gROOT->FindObject( ("DataMumuons_Tight_pt"+etaCut).Data() );
      looseID_histo  = (TH1F*)gROOT->FindObject( ("DataMumuons_looseID_pt"+etaCut).Data() );
      looseIso_histo = (TH1F*)gROOT->FindObject( ("DataMumuons_looseIso_pt"+etaCut).Data() );
     
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject( ("Zjetsmuons_Tight_pt"+etaCut).Data() );
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject( ("Zjetsmuons_looseID_pt"+etaCut).Data() );
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject( ("Zjetsmuons_looseIso_pt"+etaCut).Data() );
      
      
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
     
     
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      
      
      
      looseID_histo->GetXaxis()->SetTitle("muon p_{T} [GeV/c]");
      looseIso_histo->GetXaxis()->SetTitle("muon p_{T} [GeV/c]");
      
      //c1->SaveAs("muon_eff_pt.gif");
      if(computeSF){
        /*tight_histo->Sumw2();
        looseID_histo->Sumw2();
        tight_histo_Zjets->Sumw2();
        looseID_histo_Zjets->Sumw2();
	if(isoid == "id"){
	  cout << "in id " << endl;
          tight_histo->Divide(tight_histo,looseID_histo, 1, 1, "b");
          tight_histo_Zjets->Divide(tight_histo_Zjets,looseID_histo_Zjets, 1, 1, "b");
          tight_histo->Divide(tight_histo,tight_histo_Zjets, 1, 1, "b");
	}
	if(isoid == "iso"){
	  cout << "in iso " << endl;
          tight_histo->Divide(tight_histo,looseIso_histo, 1, 1, "b");
          tight_histo_Zjets->Divide(tight_histo_Zjets,looseIso_histo_Zjets, 1, 1, "b");
          tight_histo->Divide(tight_histo,tight_histo_Zjets, 1, 1, "b");
	}*/
      }
    } 
    
    
    if(q == "eta"){
      
      f_Data_Mu->cd();
      tight_histo    = (TH1F*)gROOT->FindObject(("DataMumuons_Tight_eta"+etaCut).Data());
      looseID_histo  = (TH1F*)gROOT->FindObject(("DataMumuons_looseID_eta"+etaCut).Data());
      looseIso_histo = (TH1F*)gROOT->FindObject(("DataMumuons_looseIso_eta"+etaCut).Data());
      
      
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject(("Zjetsmuons_Tight_eta"+etaCut).Data());
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject(("Zjetsmuons_looseID_eta"+etaCut).Data());
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject(("Zjetsmuons_looseIso_eta"+etaCut).Data());
     
     
      
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
      
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      looseID_histo->GetXaxis()->SetTitle("muon #eta");
      looseIso_histo->GetXaxis()->SetTitle("muon #eta");
      //c1->SaveAs("muon_eff_eta.gif");
    } 
    if(q == "phi"){
      
      f_Data_Mu->cd();
      tight_histo    = (TH1F*)gROOT->FindObject(("DataMumuons_Tight_phi"+etaCut).Data());
      looseID_histo  = (TH1F*)gROOT->FindObject(("DataMumuons_looseID_phi"+etaCut).Data());
      looseIso_histo = (TH1F*)gROOT->FindObject(("DataMumuons_looseIso_phi"+etaCut).Data());
          
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject(("Zjetsmuons_Tight_phi"+etaCut).Data());
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject(("Zjetsmuons_looseID_phi"+etaCut).Data());
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject(("Zjetsmuons_looseIso_phi"+etaCut).Data());
     
      
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
      
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      
      looseID_histo->GetXaxis()->SetTitle("muon #phi");
      looseIso_histo->GetXaxis()->SetTitle("muon #phi");
    } 
    if(q == "njet"){
      
      f_Data_Mu->cd();
      tight_histo    = (TH1F*)gROOT->FindObject(("DataMumuons_Tight_njet"+etaCut).Data());
      looseID_histo  = (TH1F*)gROOT->FindObject(("DataMumuons_looseID_njet"+etaCut).Data());
      looseIso_histo = (TH1F*)gROOT->FindObject(("DataMumuons_looseIso_njet"+etaCut).Data());
      
      
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject(("Zjetsmuons_Tight_njet"+etaCut).Data());
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject(("Zjetsmuons_looseID_njet"+etaCut).Data());
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject(("Zjetsmuons_looseIso_njet"+etaCut).Data());
     
      
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
      
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      looseID_histo->GetXaxis()->SetTitle("NJet");
      looseIso_histo->GetXaxis()->SetTitle("NJet");
    } 
    if(isoid == "id" ) qw->AddEntry(looseID_histo,  "muon ID eff. data","P");
    if(isoid == "iso") qw->AddEntry(looseIso_histo, "muon Iso eff. data","P");
    if(isoid == "id" ) qw->AddEntry(looseID_histo_Zjets,  "muon ID eff.  Z jets","P");
    if(isoid == "iso") qw->AddEntry(looseIso_histo_Zjets, "muon iso eff. Z jets","P");
  }
  
  
  
  if(lepton == "El"){
    
    if(q == "pt"){
      
      f_Data_El->cd();
      tight_histo    = (TH1F*)gROOT->FindObject(("DataEGelectrons_Tight_pt"+etaCut).Data());
      looseID_histo  = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseID_pt"+etaCut).Data());
      looseIso_histo = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseIso_pt"+etaCut).Data());
      
      tight_histo_LS    = (TH1F*)gROOT->FindObject("DataEGelectrons_Tight_pt_LS");
      looseID_histo_LS  = (TH1F*)gROOT->FindObject("DataEGelectrons_looseID_pt_LS");
      looseIso_histo_LS = (TH1F*)gROOT->FindObject("DataEGelectrons_looseIso_pt_LS");
      
      
      tight_histo_CP    = (TH2F*)gROOT->FindObject("DataEGelectrons_Tight_pt_CP");
      looseID_histo_CP  = (TH2F*)gROOT->FindObject("DataEGelectrons_looseID_pt_CP");
      looseIso_histo_CP = (TH2F*)gROOT->FindObject("DataEGelectrons_looseIso_pt_CP");
      
      
      /*f_MC_Zjets->cd();
      
      
      tight_histo    = (TH1F*)gROOT->FindObject("Zjetselectrons_Tight_pt");
      looseID_histo  = (TH1F*)gROOT->FindObject("Zjetselectrons_looseID_pt");
      looseIso_histo = (TH1F*)gROOT->FindObject("Zjetselectrons_looseIso_pt");
      
      tight_histo_LS    = (TH1F*)gROOT->FindObject("Zjetselectrons_Tight_pt_LS");
      looseID_histo_LS  = (TH1F*)gROOT->FindObject("Zjetselectrons_looseID_pt_LS");
      looseIso_histo_LS = (TH1F*)gROOT->FindObject("Zjetselectrons_looseIso_pt_LS");
      
      
      tight_histo_CP    = (TH2F*)gROOT->FindObject("Zjetselectrons_Tight_pt_CP");
      looseID_histo_CP  = (TH2F*)gROOT->FindObject("Zjetselectrons_looseID_pt_CP");
      looseIso_histo_CP = (TH2F*)gROOT->FindObject("Zjetselectrons_looseIso_pt_CP");
       */
      /*for(unsigned int i=1; i<=14; i++){
      
	cout << "*******************  "  << endl;
        double qqmean_looseIso = 0;
        qqmean_looseIso        = -1*looseIso_histo_CP->GetBinContent(i,1) + looseIso_histo_CP->GetBinContent(i,3);
	cout << " looseIso_histo_CP->GetBinContent(i,1) " <<  looseIso_histo_CP->GetBinContent(i,1) << endl;
	cout << " looseIso_histo_CP->GetBinContent(i,3) " <<  looseIso_histo_CP->GetBinContent(i,3)  << endl;
	cout << " qqmean_looseIso 1 " <<  qqmean_looseIso << endl;
	qqmean_looseIso        = qqmean_looseIso/(looseIso_histo->GetBinContent(i)+looseIso_histo_LS->GetBinContent(i));
	cout << " qqmean_looseIso 2 " <<  qqmean_looseIso << endl;
	
	double qmiss          = (1- pow(fabs(qqmean_looseIso), 0.5) ) / 2;
	cout << "qmiss            " <<  qmiss << endl;
	cout << "pow(1-qmiss, 2)  " <<  pow(1-qmiss, 2) << endl;
	double the_N_S        = (looseIso_histo->GetBinContent(i) - looseIso_histo_LS->GetBinContent(i))/( pow(1-qmiss, 2)); 
	cout << "looseIso_histo->GetBinContent(i) " << looseIso_histo->GetBinContent(i) << endl;
	looseIso_histo->SetBinContent(i,the_N_S);
	cout <<" the_N_S " << the_N_S << endl;
        
      } */
      
      
      /*f_MC_TTbar->cd();
      tight_histo_TTbar    = (TH1F*)gROOT->FindObject("genEl_Tight_pt_TTbar");
      looseID_histo_TTbar  = (TH1F*)gROOT->FindObject("genEl_LooseID_pt_TTbar");
      looseIso_histo_TTbar = (TH1F*)gROOT->FindObject("genEl_LooseIso_pt_TTbar");
      */
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject(("Zjetselectrons_Tight_pt"+etaCut).Data());
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseID_pt"+etaCut).Data());
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseIso_pt"+etaCut).Data());
      
      /*
      tight_histo->Rebin(4);
      looseID_histo->Rebin(4);
      looseIso_histo->Rebin(4);
      
      tight_histo_Zjets->Rebin(4);
      looseID_histo_Zjets->Rebin(4);
      looseIso_histo_Zjets->Rebin(4);*/
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
      
      
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      
      looseID_histo->GetXaxis()->SetTitle("electron p_{T} [GeV/c]");
      looseIso_histo->GetXaxis()->SetTitle("electron p_{T} [GeV/c]");
      
      tight_histo->GetXaxis()->SetTitle("electron p_{T} [GeV/c]");
      //c1->SaveAs("electron_eff_pt.gif");
      //compute scale factors
      
      
      
      
      if(computeSF){
        /*tight_histo->Sumw2();
        looseID_histo->Sumw2();
        tight_histo_Zjets->Sumw2();
        looseID_histo_Zjets->Sumw2();
	if(isoid == "id"){
	  
          tight_histo->Divide(tight_histo,looseID_histo, 1, 1, "b");
          tight_histo_Zjets->Divide(tight_histo_Zjets,looseID_histo_Zjets, 1, 1, "b");
          tight_histo->Divide(tight_histo,tight_histo_Zjets, 1, 1, "b");
	}
	if(isoid == "iso"){
          tight_histo->Divide(tight_histo,looseIso_histo, 1, 1, "b");
          tight_histo_Zjets->Divide(tight_histo_Zjets,looseIso_histo_Zjets, 1, 1, "b");
          tight_histo->Divide(tight_histo,tight_histo_Zjets, 1, 1, "b");
	}*/
      }
      
    } 
    if(q == "eta"){
      
      f_Data_El->cd();
      tight_histo    = (TH1F*)gROOT->FindObject(("DataEGelectrons_Tight_eta"+etaCut).Data());
      looseID_histo  = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseID_eta"+etaCut).Data());
      looseIso_histo = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseIso_eta"+etaCut).Data());
      
      tight_histo_LS    = (TH1F*)gROOT->FindObject("DataEGelectrons_Tight_eta_LS");
      looseID_histo_LS  = (TH1F*)gROOT->FindObject("DataEGelectrons_looseID_eta_LS");
      looseIso_histo_LS = (TH1F*)gROOT->FindObject("DataEGelectrons_looseIso_eta_LS");
      
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject(("Zjetselectrons_Tight_eta"+etaCut).Data());
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseID_eta"+etaCut).Data());
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseIso_eta"+etaCut).Data());
      
      
      
      /*tight_histo->Rebin(4);
      looseID_histo->Rebin(4);
      looseIso_histo->Rebin(4);
      
      tight_histo_Zjets->Rebin(4);
      looseID_histo_Zjets->Rebin(4);
      looseIso_histo_Zjets->Rebin(4);*/
      
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      looseID_histo->GetXaxis()->SetTitle("electron #eta");
      looseIso_histo->GetXaxis()->SetTitle("electron #eta");
      tight_histo->GetXaxis()->SetTitle("electron #eta");
      //c1->SaveAs("electron_eff_eta.gif"); 
      /*if(computeSF){
        tight_histo->Sumw2();
        looseID_histo->Sumw2();
        tight_histo_Zjets->Sumw2();
        looseID_histo_Zjets->Sumw2();
        tight_histo->Divide(tight_histo,looseID_histo, 1, 1, "b");
        tight_histo_Zjets->Divide(tight_histo_Zjets,looseID_histo_Zjets, 1, 1, "b");
        tight_histo->Divide(tight_histo,tight_histo_Zjets, 1, 1, "b");
      }*/
    } 
    if(q == "phi"){
      
      f_Data_El->cd();
      tight_histo    = (TH1F*)gROOT->FindObject(("DataEGelectrons_Tight_phi"+etaCut).Data());
      looseID_histo  = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseID_phi"+etaCut).Data());
      looseIso_histo = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseIso_phi"+etaCut).Data());
      
      tight_histo_LS    = (TH1F*)gROOT->FindObject("DataEGelectrons_Tight_phi_LS");
      looseID_histo_LS  = (TH1F*)gROOT->FindObject("DataEGelectrons_looseID_phi_LS");
      looseIso_histo_LS = (TH1F*)gROOT->FindObject("DataEGelectrons_looseIso_phi_LS");
        
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject(("Zjetselectrons_Tight_phi"+etaCut).Data());
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseID_phi"+etaCut).Data());
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseIso_phi"+etaCut).Data());
      
      /*tight_histo->Rebin(4);
      looseID_histo->Rebin(4);
      looseIso_histo->Rebin(4);
      
      tight_histo_Zjets->Rebin(4);
      looseID_histo_Zjets->Rebin(4);
      looseIso_histo_Zjets->Rebin(4);*/
      
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
      
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      looseID_histo->GetXaxis()->SetTitle("electron #phi");
      looseIso_histo->GetXaxis()->SetTitle("electron #phi");
      tight_histo->GetXaxis()->SetTitle("electron #phi");
      //c1->SaveAs("electron_eff_phi.gif");
      /*if(computeSF){
        
	
	
        tight_histo->Sumw2();
        looseID_histo->Sumw2();
        tight_histo_Zjets->Sumw2();
        looseID_histo_Zjets->Sumw2();
        tight_histo->Divide(tight_histo,looseID_histo, 1, 1, "b");
        tight_histo_Zjets->Divide(tight_histo_Zjets,looseID_histo_Zjets, 1, 1, "b");
        tight_histo->Divide(tight_histo,tight_histo_Zjets, 1, 1, "b");
      }*/
    } 
    if(q == "njet"){
      
      f_Data_El->cd();
      tight_histo    = (TH1F*)gROOT->FindObject(("DataEGelectrons_Tight_njet"+etaCut).Data());
      looseID_histo  = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseID_njet"+etaCut).Data());
      looseIso_histo = (TH1F*)gROOT->FindObject(("DataEGelectrons_looseIso_njet"+etaCut).Data());
      
      
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject(("Zjetselectrons_Tight_njet"+etaCut).Data());
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseID_njet"+etaCut).Data());
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject(("Zjetselectrons_looseIso_njet"+etaCut).Data());
      
      
      grID  = new TGraphAsymmErrors();
      grID->BayesDivide(tight_histo,looseID_histo);
      grIso = new TGraphAsymmErrors();
      grIso->BayesDivide(tight_histo,looseIso_histo);
      
      
      grID_Zjets  = new TGraphAsymmErrors();
      grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
      grIso_Zjets = new TGraphAsymmErrors();
      grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
      
      
      
      
      looseID_histo->GetXaxis()->SetTitle("NJet");
      looseIso_histo->GetXaxis()->SetTitle("NJet");
      //c1->SaveAs("electron_eff_njet.gif");
    } 
    if(isoid == "id" ) qw->AddEntry(looseID_histo,  "electron ID eff. data","P");
    if(isoid == "iso") qw->AddEntry(looseIso_histo, "electron iso eff.data","P");
    //if(isoid == "id" ) qw->AddEntry(looseID_histo_TTbar,  "electron ID eff. in TT MC","P");
    //if(isoid == "iso") qw->AddEntry(looseIso_histo_TTbar, "electron iso eff. in TT MC","P");
    if(isoid == "id" ) qw->AddEntry(looseID_histo_Zjets,  "electron ID eff.  Z jets","P");
    if(isoid == "iso") qw->AddEntry(looseIso_histo_Zjets, "electron iso eff.  Z jets","P");
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  tight_histo->Sumw2();
  looseID_histo->Sumw2();
  looseIso_histo->Sumw2();
  
  looseID_histo->Divide( tight_histo, looseID_histo, 1, 1, "b");
  looseIso_histo->Divide(tight_histo, looseIso_histo, 1, 1, "b");
  
  looseID_histo->SetMarkerStyle(20);
  looseIso_histo->SetMarkerStyle(20);
  looseID_histo->SetMarkerColor(2);
  looseIso_histo->SetMarkerColor(4);
  
  //looseID_histo_TTbar->SetMarkerStyle(24);
  //looseIso_histo_TTbar->SetMarkerStyle(24);
  //looseID_histo_TTbar->SetMarkerColor(2);
  //looseIso_histo_TTbar->SetMarkerColor(4);
  
  looseID_histo_Zjets->SetMarkerStyle(26);
  looseIso_histo_Zjets->SetMarkerStyle(26);
  looseID_histo_Zjets->SetMarkerColor(2);
  looseIso_histo_Zjets->SetMarkerColor(4);
    
  looseID_histo->SetMaximum(1.1);
  looseID_histo->SetMinimum(0.);
  looseIso_histo->SetMaximum(1.1);
  looseIso_histo->SetMinimum(0.);
  if(isoid == "id" ) looseID_histo->Draw("p");
  if(isoid == "iso") looseIso_histo->Draw("P");
  
  grID->SetMarkerStyle(20);
  grIso->SetMarkerStyle(20);
  grID->SetMarkerColor(2);
  grIso->SetMarkerColor(4);
  
  
  //grID_TTbar->SetMarkerStyle(24);
  //grIso_TTbar->SetMarkerStyle(24);
  //grID_TTbar->SetMarkerColor(2);
  //grIso_TTbar->SetMarkerColor(4);
  
  
  grID_Zjets->SetMarkerStyle(26);
  grIso_Zjets->SetMarkerStyle(26);
  grID_Zjets->SetMarkerColor(2);
  grIso_Zjets->SetMarkerColor(4);
  
  
  if(isoid == "iso")   grIso->Draw("psame");
  if(isoid == "id" )   grID->Draw("psame");
  if(isoid == "id" )   grID_Zjets->Draw("psame");
  if(isoid == "iso")   grIso_Zjets->Draw("psame");
   
  
  qw->Draw();
  TString outputfilename =  lepton+"_"+ q+"_"+isoid+".gif";
  
  c1->SaveAs(outputfilename.Data());
  
 
  if(computeSF){
     //double ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 100};
     double ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 100};
     TH1F * thehistSF = new TH1F("thehistSF", "thehistSF", 10, ptRange);
  
     for(unsigned int i=0; i<= looseIso_histo->GetNbinsX()-1; i++){
	  std::pair<double, double> thepair;
       if(isoid == "iso" ){
         double eff_data = 0;
         double x_data   = 0;
	 grIso->GetPoint(i, x_data, eff_data);
	 
	 //cout << "iso x_data " << i << " " << x_data << " iso eff_data " << eff_data << endl;
         double eff_Z  = 0;
         double x_Z    = 0;
	 grIso_Zjets->GetPoint(i, x_Z,eff_Z );
	 //cout << "iso x_Z    " << i << " " << x_Z    << " iso eff_Z    " << eff_Z    << endl;
	 //cout << "calculSF(eff_data, eff_Z)  " << calculSF(eff_data, eff_Z) << endl;
         //*ofile <<   calculSF(eff_data, eff_Z)
         // << " $\pm$ " <<  calculErrorSF(eff_data, grIso->GetErrorY(i), eff_Z, grIso_Zjets->GetErrorY(i)) <<  " & " ;
	  thepair.first  = calculSF(eff_data, eff_Z);
	  thepair.second = calculErrorSF(eff_data, grIso->GetErrorY(i), eff_Z, grIso_Zjets->GetErrorY(i));
	  output.push_back(thepair);
	  thehistSF->SetBinContent(i+1, thepair.first);
	  thehistSF->SetBinError(i+1, thepair.second);
	}
       
       if(isoid == "id" ){
         double eff_data = 0;
         double x_data   = 0;
	 grID->GetPoint(i, x_data, eff_data);
	 //cout << "id x_data " << i << " "  << x_data << " id eff_data " << eff_data << endl;
         double eff_Z  = 0;
         double x_Z    = 0;
	 grID_Zjets->GetPoint(i, x_Z,eff_Z );
	 //cout << "id x_Z    " << i << " "  << x_Z    << " id eff_Z    " << eff_Z    << endl;
	 
         //*ofile <<   calculSF(eff_data, eff_Z)
          //<< " $\pm$ " <<  calculErrorSF(eff_data, grID->GetErrorY(i), eff_Z, grID_Zjets->GetErrorY(i)) <<  " & " ;
	  thepair.first  = calculSF(eff_data, eff_Z);
	  thepair.second = calculErrorSF(eff_data, grID->GetErrorY(i), eff_Z, grID_Zjets->GetErrorY(i));
	  output.push_back(thepair);
	  thehistSF->SetBinContent(i+1, thepair.first);
	  thehistSF->SetBinError(i+1, thepair.second);
	}
       
     } 
   thehistSF->SetMaximum(1.1);
   TString outputfilename =  lepton+"_SF_"+ q+"_"+isoid+".gif";
  
   if(isoid == "id" ){ thehistSF->GetYaxis()->SetTitle("SF_{ID}");   }
   if(isoid == "iso" ){ thehistSF->GetYaxis()->SetTitle("SF_{Iso}"); }
   thehistSF->SetMarkerStyle(20);
 
   thehistSF->SetMaximum(1.1);
   thehistSF->SetMinimum(0.);
   if(lepton == "El") thehistSF->GetXaxis()->SetTitle("electron p_{T} [GeV/c]");
   if(lepton == "Mu") thehistSF->GetXaxis()->SetTitle("muon p_{T} [GeV/c]");
   thehistSF->Draw("ep");
   c1->SaveAs(outputfilename.Data());
  
  }
     
  
  
  if(!computeSF){
  
     //double ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 100};
     for(unsigned int i=0; i<= looseIso_histo->GetNbinsX()-1; i++){
       
       if(isoid == "iso" ){
        //*ofile <<   looseIso_histo->GetBinContent(i) << " $\pm$ " <<  looseIso_histo->GetBinError(i) <<  " & " ;
         double eff_data = 0;
         double x_data   = 0;
	 grIso->GetPoint(i, x_data, eff_data);
	 //*ofile <<  eff_data  << " $\pm$ " << grIso->GetErrorY(i)  <<  " & " ;
	 std::pair<double, double> thepair;
	 thepair.first  = eff_data;
	 thepair.second = grIso->GetErrorY(i) ;
	 output.push_back(thepair);
       }
       if(isoid == "id" ){
         //*ofile <<   looseID_histo->GetBinContent(i)  << " $\pm$ " <<  looseID_histo->GetBinError(i) <<  " & " ;
         double eff_data = 0;
         double x_data   = 0;
	 grID->GetPoint(i, x_data, eff_data);
	 //*ofile <<  eff_data  << " $\pm$ " << grID->GetErrorY(i)  <<  " & " ;
	 std::pair<double, double> thepair;
	 thepair.first  = eff_data;
	 thepair.second = grID->GetErrorY(i) ;
	 output.push_back(thepair);
       }
       
     }
     
     
  }
  
  
  
  
  delete tight_histo;
  delete looseID_histo;
  delete looseIso_histo;
  delete tight_histo_LS;
  delete looseID_histo_LS;
  delete looseIso_histo_LS;
  delete tight_histo_CP;
  delete looseID_histo_CP;
  delete looseIso_histo_CP;
  delete tight_histo_Zjets    ;
  delete looseID_histo_Zjets  ;
  delete looseIso_histo_Zjets ;
  delete grID;
  delete grIso;
  delete grID_Zjets;
  delete grIso_Zjets;
  delete c1;
  delete f_Data_El ; 
  delete f_Data_Mu ; 
  delete f_MC_Zjets; 
  
  return output;
  
}


void dumpTagAndProbeValue(ofstream *ofile, std::vector<pair<double, double > > norm, 
std::vector<pair<double, double > > sup, std::vector<pair<double, double > > sdown , bool part1){
  
  if(part1){
   for(unsigned int ibin = 0; ibin < 5; ibin++){
    
    double systUp   = fabs(norm[ibin].first - sup[ibin].first);
    double systDown = fabs(norm[ibin].first - sdown[ibin].first);
    double syst = systUp;
    if(systUp<systDown) syst = systDown;
    *ofile << norm[ibin].first   << " $\\pm$ " << norm[ibin].second << "$\\pm$"  <<  syst ;
    if(ibin < 4 ) *ofile<< " & " ;
    
    
   }
  }else{
  
   for(unsigned int ibin = 5; ibin < norm.size(); ibin++){
    
    double systUp   = fabs(norm[ibin].first - sup[ibin].first);
    double systDown = fabs(norm[ibin].first - sdown[ibin].first);
    double syst = systUp;
    if(systUp<systDown) syst = systDown;
    *ofile << norm[ibin].first   << " $\\pm$ " << norm[ibin].second << "$\\pm$"  <<  syst ;
    if(ibin < (norm.size()-1) ) *ofile<< " & " ;
    
    
   }
  
  }
  
    *ofile << " \\\\ " << endl;


}
void dumpTagAndProbeValueRoot(
std::vector<pair<double, double > > norm_inf15,  std::vector<pair<double, double > > sup_inf15, std::vector<pair<double, double > > sdown_inf15, 
std::vector<pair<double, double > > norm_sup15,  std::vector<pair<double, double > > sup_sup15, std::vector<pair<double, double > > sdown_sup15, 
bool isMuon ){



  double ptRange[]  = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 100};
  double etaRange[] = {0., 1.5, 2.5};
  TH2F * thehistSF = new TH2F("thehistSF", "thehistSF", 10, ptRange, 2, etaRange);
  

  for(unsigned int ibin = 0; ibin < norm_inf15.size(); ibin++){
    
    double systUp   = fabs(norm_inf15[ibin].first - sup_inf15[ibin].first);
    double systDown = fabs(norm_inf15[ibin].first - sdown_inf15[ibin].first);
    double syst = systUp;
    if(systUp<systDown) syst = systDown;
    thehistSF->SetBinContent(ibin+1, 1, norm_inf15[ibin].first);
    thehistSF->SetBinError(ibin+1, 1,syst);
  }
  
  
  for(unsigned int ibin = 0; ibin < norm_sup15.size(); ibin++){
    
    double systUp   = fabs(norm_sup15[ibin].first - sup_sup15[ibin].first);
    double systDown = fabs(norm_sup15[ibin].first - sdown_sup15[ibin].first);
    double syst = systUp;
    if(systUp<systDown) syst = systDown;
    thehistSF->SetBinContent(ibin+1, 2, norm_sup15[ibin].first);
    thehistSF->SetBinError(ibin+1, 2,syst);
  }
  
  
  TFile * outputSF;
  if(isMuon) outputSF = new TFile("muonSF.root", "recreate");
  else       outputSF = new TFile("electronSF.root", "recreate");
  outputSF->cd();;
  thehistSF->Write();




}

void ProducePlots(){
  
  string ofilenametex = string("Plots")+string(".tex");
  ofstream ofile(ofilenametex.c_str());
 
  std::vector<pair<double, double > > elecIsoEff_pt = plotTagAndProbEff("El", "pt", "iso","" , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIDEff_pt  = plotTagAndProbEff("El", "pt", "id",""  , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIsoSF_pt  = plotTagAndProbEff("El", "pt", "iso","" , &ofile, 1, 0);
  std::vector<pair<double, double > > elecIDSF_pt   = plotTagAndProbEff("El", "pt", "id",""  , &ofile, 1, 0);
  
  std::vector<pair<double, double > > muonIsoEff_pt = plotTagAndProbEff("Mu", "pt", "iso","" , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIDEff_pt  = plotTagAndProbEff("Mu", "pt", "id",""  , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIsoSF_pt  = plotTagAndProbEff("Mu", "pt", "iso","" , &ofile, 1, 0);
  std::vector<pair<double, double > > muonIDSF_pt   = plotTagAndProbEff("Mu", "pt", "id",""  , &ofile, 1, 0);
  
  
 
  std::vector<pair<double, double > > elecIsoEff_eta = plotTagAndProbEff("El", "eta", "iso","" , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIDEff_eta  = plotTagAndProbEff("El", "eta", "id",""  , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIsoSF_eta  = plotTagAndProbEff("El", "eta", "iso","" , &ofile, 1, 0);
  std::vector<pair<double, double > > elecIDSF_eta   = plotTagAndProbEff("El", "eta", "id",""  , &ofile, 1, 0);
  
  std::vector<pair<double, double > > muonIsoEff_eta = plotTagAndProbEff("Mu", "eta", "iso","" , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIDEff_eta  = plotTagAndProbEff("Mu", "eta", "id",""  , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIsoSF_eta  = plotTagAndProbEff("Mu", "eta", "iso","" , &ofile, 1, 0);
  std::vector<pair<double, double > > muonIDSF_eta   = plotTagAndProbEff("Mu", "eta", "id",""  , &ofile, 1, 0);
  
  
  
 
  std::vector<pair<double, double > > elecIsoEff_phi = plotTagAndProbEff("El", "phi", "iso","" , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIDEff_phi  = plotTagAndProbEff("El", "phi", "id",""  , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIsoSF_phi  = plotTagAndProbEff("El", "phi", "iso","" , &ofile, 1, 0);
  std::vector<pair<double, double > > elecIDSF_phi   = plotTagAndProbEff("El", "phi", "id",""  , &ofile, 1, 0);
  
  std::vector<pair<double, double > > muonIsoEff_phi = plotTagAndProbEff("Mu", "phi", "iso","" , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIDEff_phi  = plotTagAndProbEff("Mu", "phi", "id",""  , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIsoSF_phi  = plotTagAndProbEff("Mu", "phi", "iso","" , &ofile, 1, 0);
  std::vector<pair<double, double > > muonIDSF_phi   = plotTagAndProbEff("Mu", "phi", "id",""  , &ofile, 1, 0);

}

std::vector<pair<double, double > > multiplySF(std::vector<pair<double, double > > vect1, std::vector<pair<double, double > > vect2){
  
  std::vector<pair<double, double > > globalSF;
  for(unsigned int i=0; i< vect1.size(); i++){
    
    
	 std::pair<double, double> thepair;
	 thepair.first  = vect1[i].first*vect2[i].first;
	 thepair.second = pow(pow(vect1[i].second,2.)+ pow(vect2[i].second,2.), 0.5);
         globalSF.push_back(thepair);
  }
  return globalSF;


}

void dumpTagAndProbeTable(){
//plotTagAndProbEff(TString lepton, TString q, TString isoid, TString etaCut, ofstream ofile)
  string ofilenametex = string("TagAndProbe")+string(".tex");
  ofstream ofile(ofilenametex.c_str());
  ofile<<"\\documentclass[amsmath,amssymb]{revtex4}"<<endl;
  ofile<<"\\begin{document}"<<endl;
  
  ofile.setf(ios::fixed);
  ofile.precision(3);
  
  
  
  
   cout << "765  " << endl;
  //*********************************************************************************
  //nominal values
  std::vector<pair<double, double > > elecIsoEff_infEta15 = plotTagAndProbEff("El", "pt", "iso","_infEta15" , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIsoEff_supEta15 = plotTagAndProbEff("El", "pt", "iso","_supEta15" , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIDEff_infEta15  = plotTagAndProbEff("El", "pt", "id","_infEta15"  , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIDEff_supEta15  = plotTagAndProbEff("El", "pt", "id","_supEta15"  , &ofile, 0, 0);
  std::vector<pair<double, double > > elecIsoSF_infEta15  = plotTagAndProbEff("El", "pt", "iso","_infEta15" , &ofile, 1, 0);
  std::vector<pair<double, double > > elecIsoSF_supEta15  = plotTagAndProbEff("El", "pt", "iso","_supEta15" , &ofile, 1, 0);
  std::vector<pair<double, double > > elecIDSF_infEta15   = plotTagAndProbEff("El", "pt", "id","_infEta15"  , &ofile, 1, 0);
  std::vector<pair<double, double > > elecIDSF_supEta15   = plotTagAndProbEff("El", "pt", "id","_supEta15"  , &ofile, 1, 0);
  
     cout << "765  " << endl;
  std::vector<pair<double, double > > muonIsoEff_infEta15 = plotTagAndProbEff("Mu", "pt", "iso","_infEta15" , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIsoEff_supEta15 = plotTagAndProbEff("Mu", "pt", "iso","_supEta15" , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIDEff_infEta15  = plotTagAndProbEff("Mu", "pt", "id","_infEta15"  , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIDEff_supEta15  = plotTagAndProbEff("Mu", "pt", "id","_supEta15"  , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIsoSF_infEta15  = plotTagAndProbEff("Mu", "pt", "iso","_infEta15" , &ofile, 1, 0);
  std::vector<pair<double, double > > muonIsoSF_supEta15  = plotTagAndProbEff("Mu", "pt", "iso","_supEta15" , &ofile, 1, 0);
  std::vector<pair<double, double > > muonIDSF_infEta15   = plotTagAndProbEff("Mu", "pt", "id","_infEta15"  , &ofile, 1, 0);
  std::vector<pair<double, double > > muonIDSF_supEta15   = plotTagAndProbEff("Mu", "pt", "id","_supEta15"  , &ofile, 1, 0);
  
  
     cout << "788  " << endl;
  
  //*********************************************************************************
  //syst up
  std::vector<pair<double, double > > elecIsoEff_infEta15_sUp = plotTagAndProbEff("El", "pt", "iso","_infEta15" , &ofile, 0, 1);
  std::vector<pair<double, double > > elecIsoEff_supEta15_sUp = plotTagAndProbEff("El", "pt", "iso","_supEta15" , &ofile, 0, 1);
  std::vector<pair<double, double > > elecIDEff_infEta15_sUp  = plotTagAndProbEff("El", "pt", "id","_infEta15"  , &ofile, 0, 1);
  std::vector<pair<double, double > > elecIDEff_supEta15_sUp  = plotTagAndProbEff("El", "pt", "id","_supEta15"  , &ofile, 0, 1);
  std::vector<pair<double, double > > elecIsoSF_infEta15_sUp  = plotTagAndProbEff("El", "pt", "iso","_infEta15" , &ofile, 1, 1);
  std::vector<pair<double, double > > elecIsoSF_supEta15_sUp  = plotTagAndProbEff("El", "pt", "iso","_supEta15" , &ofile, 1, 1);
  std::vector<pair<double, double > > elecIDSF_infEta15_sUp   = plotTagAndProbEff("El", "pt", "id","_infEta15"  , &ofile, 1, 1);
  std::vector<pair<double, double > > elecIDSF_supEta15_sUp   = plotTagAndProbEff("El", "pt", "id","_supEta15"  , &ofile, 1, 1);
     cout << "800  " << endl;
  std::vector<pair<double, double > > muonIsoEff_infEta15_sUp = plotTagAndProbEff("Mu", "pt", "iso","_infEta15" , &ofile, 0, 1);
  std::vector<pair<double, double > > muonIsoEff_supEta15_sUp = plotTagAndProbEff("Mu", "pt", "iso","_supEta15" , &ofile, 0, 0);
  std::vector<pair<double, double > > muonIDEff_infEta15_sUp  = plotTagAndProbEff("Mu", "pt", "id","_infEta15"  , &ofile, 0, 1);
  std::vector<pair<double, double > > muonIDEff_supEta15_sUp  = plotTagAndProbEff("Mu", "pt", "id","_supEta15"  , &ofile, 0, 1);
  std::vector<pair<double, double > > muonIsoSF_infEta15_sUp  = plotTagAndProbEff("Mu", "pt", "iso","_infEta15" , &ofile, 1, 1);
  std::vector<pair<double, double > > muonIsoSF_supEta15_sUp  = plotTagAndProbEff("Mu", "pt", "iso","_supEta15" , &ofile, 1, 1);
  std::vector<pair<double, double > > muonIDSF_infEta15_sUp   = plotTagAndProbEff("Mu", "pt", "id","_infEta15"  , &ofile, 1, 1);
  std::vector<pair<double, double > > muonIDSF_supEta15_sUp   = plotTagAndProbEff("Mu", "pt", "id","_supEta15"  , &ofile, 1, 1);
     cout << "809 " << endl;
  
  //*********************************************************************************
  //syst down
  std::vector<pair<double, double > > elecIsoEff_infEta15_sDown = plotTagAndProbEff("El", "pt", "iso","_infEta15" , &ofile, 0, -1);
  std::vector<pair<double, double > > elecIsoEff_supEta15_sDown = plotTagAndProbEff("El", "pt", "iso","_supEta15" , &ofile, 0, -1);
  std::vector<pair<double, double > > elecIDEff_infEta15_sDown  = plotTagAndProbEff("El", "pt", "id","_infEta15"  , &ofile, 0, -1);
  std::vector<pair<double, double > > elecIDEff_supEta15_sDown  = plotTagAndProbEff("El", "pt", "id","_supEta15"  , &ofile, 0, -1);
  std::vector<pair<double, double > > elecIsoSF_infEta15_sDown  = plotTagAndProbEff("El", "pt", "iso","_infEta15" , &ofile, 1, -1);
  std::vector<pair<double, double > > elecIsoSF_supEta15_sDown  = plotTagAndProbEff("El", "pt", "iso","_supEta15" , &ofile, 1, -1);
  std::vector<pair<double, double > > elecIDSF_infEta15_sDown   = plotTagAndProbEff("El", "pt", "id","_infEta15"  , &ofile, 1, -1);
  std::vector<pair<double, double > > elecIDSF_supEta15_sDown   = plotTagAndProbEff("El", "pt", "id","_supEta15"  , &ofile, 1, -1);
       cout << "765  " << endl;
  std::vector<pair<double, double > > muonIsoEff_infEta15_sDown = plotTagAndProbEff("Mu", "pt", "iso","_infEta15" , &ofile, 0, -1);
  std::vector<pair<double, double > > muonIsoEff_supEta15_sDown = plotTagAndProbEff("Mu", "pt", "iso","_supEta15" , &ofile, 0, -1);
  std::vector<pair<double, double > > muonIDEff_infEta15_sDown  = plotTagAndProbEff("Mu", "pt", "id","_infEta15"  , &ofile, 0, -1);
  std::vector<pair<double, double > > muonIDEff_supEta15_sDown  = plotTagAndProbEff("Mu", "pt", "id","_supEta15"  , &ofile, 0, -1);
  std::vector<pair<double, double > > muonIsoSF_infEta15_sDown  = plotTagAndProbEff("Mu", "pt", "iso","_infEta15" , &ofile, 1, -1);
  std::vector<pair<double, double > > muonIsoSF_supEta15_sDown  = plotTagAndProbEff("Mu", "pt", "iso","_supEta15" , &ofile, 1, -1);
  std::vector<pair<double, double > > muonIDSF_infEta15_sDown   = plotTagAndProbEff("Mu", "pt", "id","_infEta15"  , &ofile, 1, -1);
  std::vector<pair<double, double > > muonIDSF_supEta15_sDown   = plotTagAndProbEff("Mu", "pt", "id","_supEta15"  , &ofile, 1, -1);
  
  
  
     double ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 100};
  
  //***************
  //for electrons 
  //***************
  // int systType
  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoEff_infEta15, elecIsoEff_infEta15_sUp, elecIsoEff_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoEff_supEta15, elecIsoEff_supEta15_sUp, elecIsoEff_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoEff_infEta15, elecIsoEff_infEta15_sUp, elecIsoEff_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoEff_supEta15, elecIsoEff_supEta15_sUp, elecIsoEff_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{Electron isolation efficiency as function of electrons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;

  ofile << endl;
  ofile << endl;
  ofile << endl;
  
  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDEff_infEta15, elecIDEff_infEta15_sUp, elecIDEff_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDEff_supEta15, elecIDEff_supEta15_sUp, elecIDEff_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDEff_infEta15, elecIDEff_infEta15_sUp, elecIDEff_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDEff_supEta15, elecIDEff_supEta15_sUp, elecIDEff_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{Electron identification efficiency as function of electrons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;
 



ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoEff_infEta15, muonIsoEff_infEta15_sUp, muonIsoEff_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoEff_supEta15, muonIsoEff_supEta15_sUp, muonIsoEff_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoEff_infEta15, muonIsoEff_infEta15_sUp, muonIsoEff_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoEff_supEta15, muonIsoEff_supEta15_sUp, muonIsoEff_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{muon isolation efficiency as function of muons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;

  ofile << endl;
  ofile << endl;
  ofile << endl;
  
  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDEff_infEta15, muonIDEff_infEta15_sUp, muonIDEff_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDEff_supEta15, muonIDEff_supEta15_sUp, muonIDEff_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDEff_infEta15, muonIDEff_infEta15_sUp, muonIDEff_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDEff_supEta15, muonIDEff_supEta15_sUp, muonIDEff_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{muon identification efficiency as function of muons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;
 

  ofile << endl;
  ofile << endl;
  ofile << endl;
   ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoSF_infEta15, elecIsoSF_infEta15_sUp, elecIsoSF_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoSF_supEta15, elecIsoSF_supEta15_sUp, elecIsoSF_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoSF_infEta15, elecIsoSF_infEta15_sUp, elecIsoSF_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIsoSF_supEta15, elecIsoSF_supEta15_sUp, elecIsoSF_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{Electron isolation SF as function of electrons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;

  ofile << endl;
  ofile << endl;
  ofile << endl;
  
  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDSF_infEta15, elecIDSF_infEta15_sUp, elecIDSF_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDSF_supEta15, elecIDSF_supEta15_sUp, elecIDSF_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDSF_infEta15, elecIDSF_infEta15_sUp, elecIDSF_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, elecIDSF_supEta15, elecIDSF_supEta15_sUp, elecIDSF_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{Electron identification SF as function of electrons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;


  ofile << endl;
  ofile << endl;
  ofile << endl;


  ofile << endl;
  ofile << endl;
  ofile << endl;
   ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoSF_infEta15, muonIsoSF_infEta15_sUp, muonIsoSF_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoSF_supEta15, muonIsoSF_supEta15_sUp, muonIsoSF_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoSF_infEta15, muonIsoSF_infEta15_sUp, muonIsoSF_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIsoSF_supEta15, muonIsoSF_supEta15_sUp, muonIsoSF_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{muon isolation SF as function of muons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;

  ofile << endl;
  ofile << endl;
  ofile << endl;
  
  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDSF_infEta15, muonIDSF_infEta15_sUp, muonIDSF_infEta15_sDown, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDSF_supEta15, muonIDSF_supEta15_sUp, muonIDSF_supEta15_sDown, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDSF_infEta15, muonIDSF_infEta15_sUp, muonIDSF_infEta15_sDown, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, muonIDSF_supEta15, muonIDSF_supEta15_sUp, muonIDSF_supEta15_sDown, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{muon identification SF as function of muons $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;


  cout << "compute global scale factor" << endl;
  //***************
  //Global SF
  //***************
  std::vector<pair<double, double > > mergeSFMuons_infEta15          = multiplySF(muonIDSF_infEta15,       muonIsoSF_infEta15);
  cout << "1102 " << mergeSFMuons_infEta15.size() << endl;
  std::vector<pair<double, double > > mergeSFMuonsSystUp_infEta15    = multiplySF(muonIDSF_infEta15_sUp,   muonIsoSF_infEta15_sUp);
  cout << "1104 " << mergeSFMuonsSystUp_infEta15.size() << endl;
  std::vector<pair<double, double > > mergeSFMuonsSystDown_infEta15  = multiplySF(muonIDSF_infEta15_sDown, muonIsoSF_infEta15_sDown);
  
  cout << "1107 " << mergeSFMuonsSystDown_infEta15.size()  << endl;
  std::vector<pair<double, double > > mergeSFMuons_supEta15          = multiplySF(muonIDSF_supEta15,       muonIsoSF_supEta15);
  cout << "1109 " << mergeSFMuons_supEta15.size()  << endl;
  std::vector<pair<double, double > > mergeSFMuonsSystUp_supEta15    = multiplySF(muonIDSF_supEta15_sUp,   muonIsoSF_supEta15_sUp);
  cout << "1111 " << mergeSFMuonsSystUp_supEta15.size()  << endl;
  std::vector<pair<double, double > > mergeSFMuonsSystDown_supEta15  = multiplySF(muonIDSF_supEta15_sDown, muonIsoSF_supEta15_sDown);
  cout << "1113 " << mergeSFMuonsSystDown_supEta15.size()  << endl;

  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFMuons_infEta15, mergeSFMuonsSystUp_infEta15, mergeSFMuonsSystDown_infEta15, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFMuons_supEta15, mergeSFMuonsSystUp_supEta15, mergeSFMuonsSystDown_supEta15, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFMuons_infEta15, mergeSFMuonsSystUp_infEta15, mergeSFMuonsSystDown_infEta15, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFMuons_supEta15, mergeSFMuonsSystUp_supEta15, mergeSFMuonsSystDown_supEta15, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{muon overall SF as function of muon $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;

  std::vector<pair<double, double > > mergeSFelectrons_infEta15          = multiplySF(elecIDSF_infEta15,       elecIsoSF_infEta15);
  std::vector<pair<double, double > > mergeSFelectronsSystUp_infEta15    = multiplySF(elecIDSF_infEta15_sUp,   elecIsoSF_infEta15_sUp);
  std::vector<pair<double, double > > mergeSFelectronsSystDown_infEta15  = multiplySF(elecIDSF_infEta15_sDown, elecIsoSF_infEta15_sDown);
  
  std::vector<pair<double, double > > mergeSFelectrons_supEta15          = multiplySF(elecIDSF_supEta15,       elecIsoSF_supEta15);
  std::vector<pair<double, double > > mergeSFelectronsSystUp_supEta15    = multiplySF(elecIDSF_supEta15_sUp,   elecIsoSF_supEta15_sUp);
  std::vector<pair<double, double > > mergeSFelectronsSystDown_supEta15  = multiplySF(elecIDSF_supEta15_sDown, elecIsoSF_supEta15_sDown);

  ofile << "\\begin{table}[p]" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 20-22 & 22-24 & 24-26 & 26-28 & 28-30  \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFelectrons_infEta15, mergeSFelectronsSystUp_infEta15, mergeSFelectronsSystDown_infEta15, 1);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFelectrons_supEta15, mergeSFelectronsSystUp_supEta15, mergeSFelectronsSystDown_supEta15, 1);
  ofile << "\\hline" << endl;
  ofile << " pt ranges & 30-32 & 32-34 & 34-36 & 36-40 & 40-100 \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "$|\\eta| <$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFelectrons_infEta15, mergeSFelectronsSystUp_infEta15, mergeSFelectronsSystDown_infEta15, 0);
  ofile << "$|\\eta| >$ 1.5 &" ; dumpTagAndProbeValue(&ofile, mergeSFelectrons_supEta15, mergeSFelectronsSystUp_supEta15, mergeSFelectronsSystDown_supEta15, 0);
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\caption{electron overall SF as function of electron $p_T$ [GeV/c] and $\\eta$.}" << endl;
  ofile << "\\end{table}" << endl;

  dumpTagAndProbeValueRoot(
  	mergeSFelectrons_infEta15, mergeSFelectronsSystUp_infEta15, mergeSFelectronsSystDown_infEta15,
  	mergeSFelectrons_supEta15, mergeSFelectronsSystUp_supEta15, mergeSFelectronsSystDown_supEta15, 0);
  
  
  dumpTagAndProbeValueRoot(
  	mergeSFMuons_infEta15, mergeSFMuonsSystUp_infEta15, mergeSFMuonsSystDown_infEta15,
  	mergeSFMuons_supEta15, mergeSFMuonsSystUp_supEta15, mergeSFMuonsSystDown_supEta15, 1);
  
  
  
  ofile<<"\\end{document}"<<endl;

}







// compare isolation efficiencies in MC ttbar vs Z.
//extract a correction factor out of it


void compareZ_TTbarEff(TString lepton){
   
  Int_t stati=0;
  Bool_t  fit=1;
  Bool_t logy=0;
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  gStyle->SetOptDate(0);
  gStyle->SetStatColor(0);
  gStyle->SetTitleColor(1);
  //gStyle->SetTitleW(0.4);
  //gStyle->SetTitleH(0.07);
  //gStyle->SetOptStat(stati);
  gStyle->SetOptStat(0);
  // gStyle->SetFrameFillColor(18);
  //gStyle->SetPalette(51,0);
  gStyle->SetPalette(1);
  
  
  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
   
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2);
  
  //c1.Divide(2,2);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillColor(10);
  
   
   
   //TFile *f_MC_Zjets = new TFile("../TagAndProbe//TriggerPlots.root"); 
   //TFile *f_MC_TTbar = new TFile("../TagAndProbe//TriggerPlots.root");  
 
   TFile *f_MC_Zjets = new TFile("TriggerPlots_eff2Jets.root"); 
   TFile *f_MC_TTbar = new TFile("TriggerPlots_eff2Jets.root");  
   
   TH1F* tight_histo_TTbar    ;
   TH1F* looseID_histo_TTbar  ;
   TH1F* looseIso_histo_TTbar ;
   
   
   TH1F* tight_histo_Zjets    ;
   TH1F* looseID_histo_Zjets  ;
   TH1F* looseIso_histo_Zjets ;
   
   if(lepton == "Mu"){
   
           
      f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject("genMu_Tight_pt_Zjets");
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject("genMu_LooseID_pt_Zjets");
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject("genMu_LooseIso_pt_Zjets");
      
      f_MC_TTbar->cd();
      tight_histo_TTbar    = (TH1F*)gROOT->FindObject("genMu_Tight_pt_TTbarSig");
      looseID_histo_TTbar  = (TH1F*)gROOT->FindObject("genMu_LooseID_pt_TTbarSig");
      looseIso_histo_TTbar = (TH1F*)gROOT->FindObject("genMu_LooseIso_pt_TTbarSig");
   
   }
   
   if(lepton == "El"){
   
       f_MC_Zjets->cd();
      tight_histo_Zjets    = (TH1F*)gROOT->FindObject("genEl_Tight_pt_Zjets");
      looseID_histo_Zjets  = (TH1F*)gROOT->FindObject("genEl_LooseID_pt_Zjets");
      looseIso_histo_Zjets = (TH1F*)gROOT->FindObject("genEl_LooseIso_pt_Zjets");
      
      f_MC_TTbar->cd();
      tight_histo_TTbar    = (TH1F*)gROOT->FindObject("genEl_Tight_pt_TTbarSig");
      looseID_histo_TTbar  = (TH1F*)gROOT->FindObject("genEl_LooseID_pt_TTbarSig");
      looseIso_histo_TTbar = (TH1F*)gROOT->FindObject("genEl_LooseIso_pt_TTbarSig");
   
   }
   
   
    
   
   TGraphAsymmErrors * grID_Zjets  = new TGraphAsymmErrors();
   grID_Zjets->BayesDivide(tight_histo_Zjets,looseID_histo_Zjets);
   TGraphAsymmErrors * grIso_Zjets = new TGraphAsymmErrors();
   grIso_Zjets->BayesDivide(tight_histo_Zjets,looseIso_histo_Zjets);
     
   
     
   TGraphAsymmErrors * grID_TTbar  = new TGraphAsymmErrors();
   grID_TTbar->BayesDivide(tight_histo_TTbar,looseID_histo_TTbar);
   TGraphAsymmErrors * grIso_TTbar = new TGraphAsymmErrors();
   grIso_TTbar->BayesDivide(tight_histo_TTbar,looseIso_histo_TTbar);
     
   float ptRange[] = {20, 22, 24, 26, 28, 30, 32, 34, 36, 40, 45, 50, 65, 80, 100};
  
   TH1F * thehistSF = new TH1F("thehistSF", "thehistSF", 14, ptRange);
  
   thehistSF->SetMaximum(1.1);
   thehistSF->SetMinimum(0.);
   if(lepton == "El") thehistSF->GetXaxis()->SetTitle("electron p_{T} [GeV/c]");
   if(lepton == "Mu") thehistSF->GetXaxis()->SetTitle("muon p_{T} [GeV/c]");
   
   grID_Zjets->SetMarkerColor(2);
   grID_TTbar->SetMarkerColor(4);
   grID_Zjets->SetMarkerStyle(20);
   grID_TTbar->SetMarkerStyle(20);
   
   thehistSF->Draw();
   grID_Zjets->Draw("epsame");
   grID_TTbar->Draw("epsame");
   
   c1->SaveAs(("comp_tt_Z_ID_"+lepton+"_.gif").Data());

   
   
   grIso_Zjets->SetMarkerColor(2);
   grIso_TTbar->SetMarkerColor(4);
   grIso_Zjets->SetMarkerStyle(20);
   grIso_TTbar->SetMarkerStyle(20);
   
   thehistSF->Draw();
   grIso_Zjets->Draw("epsame");
   grIso_TTbar->Draw("epsame");
   
   string ofilenametex = string("TagAndProbe")+string(".tex");
   ofstream ofile(ofilenametex.c_str());
   ofile<<"\\documentclass[amsmath,amssymb]{revtex4}"<<endl;
   ofile<<"\\begin{document}"<<endl;

   
   for(int i=0; i<14; i++){
        double x_data = 0;
        double eff_ID_Zjets= 0;
        double eff_ID_TTbar= 0;
	grID_Zjets->GetPoint(i, x_data, eff_ID_Zjets);
	grID_TTbar->GetPoint(i, x_data, eff_ID_TTbar);
	cout << "ID diff  " << eff_ID_Zjets - eff_ID_TTbar << endl;
   }
   
   
   double maxvalDiffIso = 0;
   double eff_Iso_Zjets[14];
   double eff_Iso_TTbar[14];
   for(int i=0; i<14; i++){
        double x_data = 0;
        double eff_Iso_Zjets= 0;
        double eff_Iso_TTbar= 0;
	grIso_Zjets->GetPoint(i, x_data, eff_Iso_Zjets);
	grIso_TTbar->GetPoint(i, x_data, eff_Iso_TTbar);
	cout << "Iso diff  " << eff_Iso_Zjets - eff_Iso_TTbar << endl;
	if(fabs(eff_Iso_Zjets - eff_Iso_TTbar) > maxvalDiffIso) maxvalDiffIso = fabs(eff_Iso_Zjets - eff_Iso_TTbar);
   }
   if(lepton == "El") cout << "maxIso El " << maxvalDiffIso << endl;
   if(lepton == "Mu") cout << "maxIso Mu " << maxvalDiffIso << endl;
   c1->SaveAs(("comp_tt_Z_Iso_"+lepton+"_.gif").Data());

   

}





void drawCorrelationPlot(TString channel){
Int_t stati=0;
  Bool_t  fit=1;
  Bool_t logy=0;
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  
  gStyle->SetOptDate(0);
  gStyle->SetStatColor(0);
  gStyle->SetTitleColor(1);
  //gStyle->SetTitleW(0.4);
  //gStyle->SetTitleH(0.07);
  //gStyle->SetOptStat(stati);
  gStyle->SetOptStat(0);
  // gStyle->SetFrameFillColor(18);
  //gStyle->SetPalette(51,0);
  gStyle->SetPalette(1);
  
  
  TCanvas *c1 = new TCanvas("c1", "c1",10,32,782,552);
   
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2);
  //c1->SetLogz(1);
  
  //c1.Divide(2,2);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetFrameFillColor(10);
  
  TFile *f_trigger = new TFile("RootFileTagAndProbeta12/TriggerPlots.root"); 
 
  TH2D * histo_ee = (TH2D*)gROOT->FindObject("correlationMET_ee_TTbarSig");
  TH2D * histo_em = (TH2D*)gROOT->FindObject("correlationMET_emu_TTbarSig");
  TH2D * histo_mm = (TH2D*)gROOT->FindObject("correlationMET_mumu_TTbarSig");
  
  
  
  if(channel == "ee"){
    histo_ee->GetXaxis()->SetBinLabel(1, "fail MET");
    histo_ee->GetXaxis()->SetBinLabel(2, "pass MET");
    histo_ee->GetYaxis()->SetBinLabel(1, "fail Dilept");
    histo_ee->GetYaxis()->SetBinLabel(2, "pass Dilept");
    
    histo_ee->GetXaxis()->SetTitle( "");
    histo_ee->GetYaxis()->SetTitle( "");
    histo_ee->SetTitle("correlations ee");
    
    histo_ee->Draw("colz");
    
    double n_total  = histo_ee->Integral();
    double N_dilept = histo_ee->GetBinContent(1, 2) + histo_ee->GetBinContent(2, 2);
    double N_met    = histo_ee->GetBinContent(2, 1) + histo_ee->GetBinContent(2, 2);
    double N_dilept_met   = histo_ee->GetBinContent(2, 2) ;
    
    double eff_dilept = N_dilept/n_total;
    cout << "eff dilept " << eff_dilept << endl;
    double eff_met = N_met/n_total;
    cout << "eff met " << eff_met << endl;
    double eff_dilept_met = N_dilept_met/n_total;
    cout << "eff dilept meet " <<  eff_dilept_met<< endl;
    cout << "eff_dilept*eff_met " << eff_met*eff_dilept << endl;
    cout << "corr factor is " << eff_met*eff_dilept/eff_dilept_met<< endl;
    
    
    c1->SaveAs("corree.gif");
  }
  
  
  
  if(channel == "emu"){
    histo_em->GetXaxis()->SetBinLabel(1, "fail MET");
    histo_em->GetXaxis()->SetBinLabel(2, "pass MET");
    histo_em->GetYaxis()->SetBinLabel(1, "fail Dilept");
    histo_em->GetYaxis()->SetBinLabel(2, "pass Dilept");
    
    histo_em->GetXaxis()->SetTitle( "");
    histo_em->GetYaxis()->SetTitle( "");
    histo_em->SetTitle("correlations emu");
    
    histo_em->Draw("colz");
    
    double n_total  = histo_em->Integral();
    double N_dilept = histo_em->GetBinContent(1, 2) + histo_em->GetBinContent(2, 2);
    double N_met    = histo_em->GetBinContent(2, 1) + histo_em->GetBinContent(2, 2);
    double N_dilept_met   = histo_em->GetBinContent(2, 2) ;
    
    double eff_dilept = N_dilept/n_total;
    cout << "eff dilept " << eff_dilept << endl;
    double eff_met = N_met/n_total;
    cout << "eff met " << eff_met << endl;
    double eff_dilept_met = N_dilept_met/n_total;
    cout << "eff dilept meet " <<  eff_dilept_met<< endl;
    cout << "eff_dilept*eff_met " << eff_met*eff_dilept << endl;
    cout << "corr factor is " << eff_met*eff_dilept/eff_dilept_met<< endl;
    c1->SaveAs("corremu.gif");
  
  }
  
  
  
  if(channel == "mumu"){
    histo_mm->GetXaxis()->SetBinLabel(1, "fail MET");
    histo_mm->GetXaxis()->SetBinLabel(2, "pass MET");
    histo_mm->GetYaxis()->SetBinLabel(1, "fail Dilept");
    histo_mm->GetYaxis()->SetBinLabel(2, "pass Dilept");
    
    histo_mm->GetXaxis()->SetTitle( "");
    histo_mm->GetYaxis()->SetTitle( "");
    histo_mm->SetTitle("correlations mumu");
    
    histo_mm->Draw("colz");  
    double n_total  = histo_mm->Integral();
    double N_dilept = histo_mm->GetBinContent(1, 2) + histo_mm->GetBinContent(2, 2);
    double N_met    = histo_mm->GetBinContent(2, 1) + histo_mm->GetBinContent(2, 2);
    double N_dilept_met   = histo_mm->GetBinContent(2, 2) ;
    
    double eff_dilept = N_dilept/n_total;
    cout << "eff dilept " << eff_dilept << endl;
    double eff_met = N_met/n_total;
    cout << "eff met " << eff_met << endl;
    double eff_dilept_met = N_dilept_met/n_total;
    cout << "eff dilept meet " <<  eff_dilept_met<< endl;
    cout << "eff_dilept*eff_met " << eff_met*eff_dilept << endl;
    cout << "corr factor is " << eff_met*eff_dilept/eff_dilept_met<< endl;
   
    c1->SaveAs("corrmumu.gif");
  
  }
     

}




