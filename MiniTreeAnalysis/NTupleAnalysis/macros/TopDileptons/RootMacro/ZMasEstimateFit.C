//******************************************************
// estimate the Z background in ee and mumu
// using the Fit method
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


double Continuum(double *x, double *p) {
  	Double_t value = p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]);
	return value;
}
double BreitWignerPlusFixedContinuum(double *x, double *p) {
 	double pi = TMath::Pi();
 	double value = p[9] * (p[8]/((p[6]*pi)*(1+pow((x[0]-p[7])/p[6],2)))) +
	               p[10]* (p[4] * (pow(p[5]*(x[0]), p[0]) * exp(p[1] * pow(p[5]*(x[0]), p[3])) + p[2]));
	return value;
}
double AsymBreitWignerPlusCutEffect (double *x, double *p) {
 	double pi = TMath::Pi();
	double value = 0.;
	if ( x[0]-p[1]<=0 ){
 	   value = p[7]*(TMath::Erf(x[0]-p[8])-TMath::Erf(x[0]-p[8]-p[9]))+
	           p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2)))) 
	              ;
	}
	if ( x[0]-p[1]>0 ){
 	   value = p[3] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) 
	              ;
	}
	return value;
}

// double AsymBreitWignerPlusCutEffectTest (double *x, double *p) {
//  	double pi = TMath::Pi();
// 	double value = 0.;
// 	if ( x[0]-p[1]<=0 ){
// // 	   value = p[7]*(TMath::Erf(x[0]-p[8])-TMath::Erf(x[0]-p[8]-p[9]))+
// 
// //   	   value =  p[7]*(TMath::Erf(x[0]-p[8]))+
//    	   value =  p[12] + p[7]*(1.+TMath::Erf(x[0]-p[8])) - p[7]*(1.+(1+p[12]/p[7]))*TMath::Erf(x[0]-p[8]-p[9]) +
//  	           p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2))))  * TMath::Exp(-(x[0]-p[1])*(x[0]-p[1])/(2.*p[11]*p[11]))
//  	              ;
// 
// //   	   value =  p[9]+p[7]*(1.+TMath::Erf(x[0]-p[8]));
// 
// //            if (  x[0]<=50 ) {
// //   	   value =  p[7]*(1.+(TMath::Erf(x[0]-p[8]-p[9])));
// // 	   }else{
// //  	   value = p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2))))  * TMath::Exp(-(x[0]-p[1])*(x[0]-p[1])/(2.*p[11]*p[11]))
// //  	              ;
// // 	   }
// 
// 	}
// 	if ( x[0]-p[1]>0 ){
//  	   value = p[3] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) 
// 	//   * TMath::Exp(-(
// 	//   x[0]-p[1])*(x[0]-p[1])/(2.*p[10]*p[10]))
// // 	   value = p[4] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) * TMath::Exp(-( x[0]-p[1])*(x[0]-p[1])/(2.*p[10]*p[10]))
// 	              ;
// 	}
// 	return value;
// }
double AsymBreitWignerPlusCutEffectTest (double *x, double *p) {
 	double pi = TMath::Pi();
	double value = 0.;
	if ( x[0]-p[1]<=0 ){
// 	   value = p[7]*(TMath::Erf(x[0]-p[8])-TMath::Erf(x[0]-p[8]-p[9]))+

//   	   value =  p[7]*(TMath::Erf(x[0]-p[8]))+
   	   value =  p[12] + p[7]*(1.+TMath::Erf(x[0]-p[8])) - p[7]*(1.+(1+p[12]/p[7]))*TMath::Erf(x[0]-p[8]-p[9]) +
 	           p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2))))  * TMath::Exp(-(x[0]-p[1])*(x[0]-p[1])/(2.*p[11]*p[11]))
 	              ;

//   	   value =  p[9]+p[7]*(1.+TMath::Erf(x[0]-p[8]));

//            if (  x[0]<=50 ) {
//   	   value =  p[7]*(1.+(TMath::Erf(x[0]-p[8]-p[9])));
// 	   }else{
//  	   value = p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2))))  * TMath::Exp(-(x[0]-p[1])*(x[0]-p[1])/(2.*p[11]*p[11]))
//  	              ;
// 	   }

	}
	if ( x[0]-p[1]>0 ){
 	   value = p[3] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) * TMath::Exp(-( x[0]-p[1])*(x[0]-p[1])/(2.*p[10]*p[10]))
// 	   value = p[4] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) * TMath::Exp(-( x[0]-p[1])*(x[0]-p[1])/(2.*p[10]*p[10]))
	              ;
	}
	return value;
}

double AsymBreitWignerPlusCutEffectTestWthContinuum (double *x, double *p) {
 	double pi = TMath::Pi();
	double value = 0.;
	if ( x[0]-p[1]<=0 ){
// 	   value = p[7]*(TMath::Erf(x[0]-p[8])-TMath::Erf(x[0]-p[8]-p[9]))+

//   	   value =  p[7]*(TMath::Erf(x[0]-p[8]))+
   	   value =  p[12] + p[7]*(1.+TMath::Erf(x[0]-p[8])) - p[7]*(1.+(1+p[12]/p[7]))*TMath::Erf(x[0]-p[8]-p[9]) +
 	           p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2))))  * TMath::Exp(-(x[0]-p[1])*(x[0]-p[1])/(2.*p[11]*p[11]))
 	              ;

//   	   value =  p[9]+p[7]*(1.+TMath::Erf(x[0]-p[8]));

//            if (  x[0]<=50 ) {
//   	   value =  p[7]*(1.+(TMath::Erf(x[0]-p[8]-p[9])));
// 	   }else{
//  	   value = p[3] * (p[4]+p[2]/((p[0]*pi)*(1+p[5]*pow((x[0]-p[1])/p[0],2))))  * TMath::Exp(-(x[0]-p[1])*(x[0]-p[1])/(2.*p[11]*p[11]))
//  	              ;
// 	   }

	}
	if ( x[0]-p[1]>0 ){
 	   value = p[3] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) * TMath::Exp(-( x[0]-p[1])*(x[0]-p[1])/(2.*p[10]*p[10]))
// 	   value = p[4] * (p[2]/((p[0]*pi)*(1+p[6]*pow((x[0]-p[1])/p[0],2)))) * TMath::Exp(-( x[0]-p[1])*(x[0]-p[1])/(2.*p[10]*p[10]))
	              ;
	}
	value += p[13] * (pow(p[14]*(x[0]), p[15]) * exp(p[16] * pow(p[14]*(x[0]), p[17])) + p[18]);
	return value;
}
void ZMasEstimateFit(){
  
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  

  //TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  //c1->SetFillColor(10);
  //c1->SetFillStyle(4000);
  //c1->SetBorderSize(2); 
  //c1->SetLogy(1);
 
  
  
  
  //***********************
  //Get histograms
  //***********************
  
  
  bool DYVerbose = true;
  bool DY_withMETcut = true; // with or without Met cut
  bool IncludeBkgInCR = false; // for systematics determination == true
  bool PlusOneSigmaBkgInSR = false; // for systematics determination
  bool MinusOneSigmaBkgInSR = false; // for systematics determination
//  float SFDY = 1.45; // value of Scale Factor with MET cut (for drawing)
  float SFDY = 1.0; // value of Scale Factor wo MET cut (for drawing)
   
  TFile *f_data ;
  
  if ( !DY_withMETcut ) {
// without Met cut
//     f_data = new TFile("proof_marcosample_allSF_2178_xsec164_SFbranchingRatioApplied_forDYFit.root");
     f_data = new TFile("proof_marcosample_allSF_2178_xsec164_SFbranchingRatioApplied_forDYFit_withNJ1LowMETPlots.root");
  } else {
// with Met cut
//     f_data = new TFile("proof_marcosample_allSF_2178_xsec164_SFbranchingRatioApplied_forDYFit_withMETcut.root");
     f_data = new TFile("proof_marcosample_allSF_2178_xsec164_SFbranchingRatioApplied_forDYFit_withMETcut_withNJ1LowMETPlots.root");
  }  
   
  f_data->cd();
  

  //*********************************************
  // get the histograms and prepare the sum of MC
  //*********************************************


  std::vector<TString> histoName_List_TT;
  histoName_List_TT.push_back("TTbarSig");
  histoName_List_TT.push_back("TTbarBkg");

  std::vector<TString> histoName_List_DY;
  histoName_List_DY.push_back("Zjets");
  histoName_List_DY.push_back("DYToMuMu_M-20");
  histoName_List_DY.push_back("DYToEE_M-20");
  histoName_List_DY.push_back("DYToTauTau_M-20");
  histoName_List_DY.push_back("DYToMuMu_M-10To20");
  histoName_List_DY.push_back("DYToEE_M-10To20");
  histoName_List_DY.push_back("DYToTauTau_M-10To20");


  std::vector<TString> histoName_List_VV;
  histoName_List_VV.push_back("WW");
  histoName_List_VV.push_back("WZ");
  histoName_List_VV.push_back("ZZ");


  std::vector<TString> histoName_List_SingleToptW;
  histoName_List_SingleToptW.push_back("TtW");
  histoName_List_SingleToptW.push_back("TbartW");

  std::vector<TString> histoName_List_Wjets;
  histoName_List_Wjets.push_back("Wjets");

  std::vector<TString> histoName_List_DataMu;
  histoName_List_DataMu.push_back("DataMu");

  std::vector<TString> histoName_List_DataEG;
  histoName_List_DataEG.push_back("DataEG");



  TH1F * Dum = (TH1F*) f_data->Get("DYMassCR_mumu_TTbarSig");
  TH1F * Dum1 = (TH1F*) f_data->Get("NjetsCR_inMZ_mumu_TTbarSig");
  TH1F * DYMassemu_mumu = new TH1F("DYMassemu_mumu", "DYMassemu_mumu", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  int nb = Dum->GetNbinsX();
  float minx = Dum->GetXaxis()->GetXmin();
  float maxx = Dum->GetXaxis()->GetXmax();



  //*********************************************
  // Define the histograms in the Control Region (CR)
  //*********************************************
  TH1F * DYMassCR_mumu_all = new TH1F("DYMassCR_mumu_all", "DYMassCR_mumu_all", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassCR_mumu_TT = new TH1F("DYMassCR_mumu_TT", "DYMassCR_mumu_TT", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassCR_mumu_VV = new TH1F("DYMassCR_mumu_VV", "DYMassCR_mumu_VV", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassCR_mumu_DY = new TH1F("DYMassCR_mumu_DY", "DYMassCR_mumu_DY", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassCR_mumu_SingleToptW = new TH1F("DYMassCR_mumu_SingleToptW", "DYMassCR_mumu_SingleToptW", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassCR_mumu_Wjets = new TH1F("DYMassCR_mumu_Wjets", "DYMassCR_mumu_Wjets", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());

  TH1F * DYMassCR_mumu_DataMu ;
  
  TH1F * NjetsCR_inMZ_mumu_all = new TH1F("NjetsCR_inMZ_mumu_all", "NjetsCR_inMZ_mumu_all", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_outMZ_mumu_all = new TH1F("NjetsCR_outMZ_mumu_all", "NjetsCR_outMZ_mumu_all", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_inMZ_mumu_TT = new TH1F("NjetsCR_inMZ_mumu_TT", "NjetsCR_inMZ_mumu_TT", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_outMZ_mumu_TT = new TH1F("NjetsCR_outMZ_mumu_TT", "NjetsCR_outMZ_mumu_TT", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_inMZ_mumu_VV = new TH1F("NjetsCR_inMZ_mumu_VV", "NjetsCR_inMZ_mumu_VV", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_outMZ_mumu_VV = new TH1F("NjetsCR_outMZ_mumu_VV", "NjetsCR_outMZ_mumu_VV", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_inMZ_mumu_DY = new TH1F("NjetsCR_inMZ_mumu_DY", "NjetsCR_inMZ_mumu_DY", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_outMZ_mumu_DY = new TH1F("NjetsCR_outMZ_mumu_DY", "NjetsCR_outMZ_mumu_DY", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_inMZ_mumu_SingleToptW = new TH1F("NjetsCR_inMZ_mumu_SingleToptW", "NjetsCR_inMZ_mumu_SingleToptW", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_outMZ_mumu_SingleToptW = new TH1F("NjetsCR_outMZ_mumu_SingleToptW", "NjetsCR_outMZ_mumu_SingleToptW", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_inMZ_mumu_Wjets = new TH1F("NjetsCR_inMZ_mumu_Wjets", "NjetsCR_inMZ_mumu_Wjets", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsCR_outMZ_mumu_Wjets = new TH1F("NjetsCR_outMZ_mumu_Wjets", "NjetsCR_outMZ_mumu_Wjets", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());

  TH1F * NjetsCR_inMZ_mumu_DataMu ; 
  TH1F * NjetsCR_outMZ_mumu_DataMu ;

  //*********************************************
  // Define the histograms in the Signal Region (SR)
  //*********************************************
  TH1F * DYMassSR_mumu_all = new TH1F("DYMassSR_mumu_all", "DYMassSR_mumu_all", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassSR_mumu_TT = new TH1F("DYMassSR_mumu_TT", "DYMassSR_mumu_TT", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassSR_mumu_VV = new TH1F("DYMassSR_mumu_VV", "DYMassSR_mumu_VV", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassSR_mumu_DY = new TH1F("DYMassSR_mumu_DY", "DYMassSR_mumu_DY", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassSR_mumu_SingleToptW = new TH1F("DYMassSR_mumu_SingleToptW", "DYMassSR_mumu_SingleToptW", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassSR_mumu_Wjets = new TH1F("DYMassSR_mumu_Wjets", "DYMassSR_mumu_Wjets", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassSR_mumu_NonDY = new TH1F("DYMassSR_mumu_NonDY", "DYMassSR_mumu_NonDY", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassSR_mumu_all_wDYScaled = new TH1F("DYMassSR_mumu_all_wDYScaled", "DYMassSR_mumu_all_wDYScaled", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());

  TH1F * DYMassSR_mumu_DataMu ; 

  TH1F * NjetsSR_inMZ_mumu_all = new TH1F("NjetsSR_inMZ_mumu_all", "NjetsSR_inMZ_mumu_all", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_outMZ_mumu_all = new TH1F("NjetsSR_outMZ_mumu_all", "NjetsSR_outMZ_mumu_all", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_inMZ_mumu_TT = new TH1F("NjetsSR_inMZ_mumu_TT", "NjetsSR_inMZ_mumu_TT", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_outMZ_mumu_TT = new TH1F("NjetsSR_outMZ_mumu_TT", "NjetsSR_outMZ_mumu_TT", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_inMZ_mumu_VV = new TH1F("NjetsSR_inMZ_mumu_VV", "NjetsSR_inMZ_mumu_VV", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_outMZ_mumu_VV = new TH1F("NjetsSR_outMZ_mumu_VV", "NjetsSR_outMZ_mumu_VV", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_inMZ_mumu_DY = new TH1F("NjetsSR_inMZ_mumu_DY", "NjetsSR_inMZ_mumu_DY", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_outMZ_mumu_DY = new TH1F("NjetsSR_outMZ_mumu_DY", "NjetsSR_outMZ_mumu_DY", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_inMZ_mumu_SingleToptW = new TH1F("NjetsSR_inMZ_mumu_SingleToptW", "NjetsSR_inMZ_mumu_SingleToptW", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_outMZ_mumu_SingleToptW = new TH1F("NjetsSR_outMZ_mumu_SingleToptW", "NjetsSR_outMZ_mumu_SingleToptW", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_inMZ_mumu_Wjets = new TH1F("NjetsSR_inMZ_mumu_Wjets", "NjetsSR_inMZ_mumu_Wjets", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());
  TH1F * NjetsSR_outMZ_mumu_Wjets = new TH1F("NjetsSR_outMZ_mumu_Wjets", "NjetsSR_outMZ_mumu_Wjets", Dum1->GetNbinsX() , Dum1->GetXaxis()->GetXmin() , Dum1->GetXaxis()->GetXmax());

  TH1F * NjetsSR_outMZ_mumu_DataMu ; 
  TH1F * NjetsSR_inMZ_mumu_DataMu ; 

  //*********************************************
  // Define the histograms in a Fully DY (FDY) region
  //*********************************************
  TH1F * DYMassFDY_mumu_all = new TH1F("DYMassFDY_mumu_all", "DYMassFDY_mumu_all", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassFDY_mumu_TT = new TH1F("DYMassFDY_mumu_TT", "DYMassFDY_mumu_TT", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassFDY_mumu_VV = new TH1F("DYMassFDY_mumu_VV", "DYMassFDY_mumu_VV", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassFDY_mumu_DY = new TH1F("DYMassFDY_mumu_DY", "DYMassFDY_mumu_DY", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassFDY_mumu_SingleToptW = new TH1F("DYMassFDY_mumu_SingleToptW", "DYMassFDY_mumu_SingleToptW", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassFDY_mumu_Wjets = new TH1F("DYMassFDY_mumu_Wjets", "DYMassFDY_mumu_Wjets", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  TH1F * DYMassFDY_mumu_DataMu ; 



  //*********************************************
  // Fill Histos
  //*********************************************
  for(unsigned int i=0; i<histoName_List_TT.size(); i++){
  
   DYMassCR_mumu_TT->Sumw2();
   DYMassCR_mumu_TT->Add(DYMassCR_mumu_TT ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   DYMassCR_mumu_all->Add(DYMassCR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);

   DYMassSR_mumu_TT->Sumw2();
   DYMassSR_mumu_TT->Add(DYMassSR_mumu_TT ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all->Add(DYMassSR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_NonDY->Add(DYMassSR_mumu_NonDY ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all_wDYScaled->Add(DYMassSR_mumu_all_wDYScaled ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);

   DYMassemu_mumu->Add(DYMassemu_mumu , (TH1F*)gROOT->FindObject(  ("hlistemu_mumu_"+histoName_List_TT[i]).Data()  ), 1, 1);

   NjetsCR_inMZ_mumu_all->Sumw2();
   NjetsSR_inMZ_mumu_all->Sumw2();
   NjetsCR_outMZ_mumu_all->Sumw2();
   NjetsSR_outMZ_mumu_all->Sumw2();
   NjetsCR_inMZ_mumu_TT->Sumw2();
   NjetsSR_inMZ_mumu_TT->Sumw2();
   NjetsCR_outMZ_mumu_TT->Sumw2();
   NjetsSR_outMZ_mumu_TT->Sumw2();
   NjetsCR_inMZ_mumu_TT->Add(NjetsCR_inMZ_mumu_TT ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_TT->Add(NjetsSR_inMZ_mumu_TT ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   NjetsCR_inMZ_mumu_all->Add(NjetsCR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_all->Add(NjetsSR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_TT->Add(NjetsCR_outMZ_mumu_TT ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_TT->Add(NjetsSR_outMZ_mumu_TT ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_all->Add(NjetsCR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_all->Add(NjetsSR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);

   DYMassFDY_mumu_TT->Sumw2();
   DYMassFDY_mumu_TT->Add(DYMassFDY_mumu_TT ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
   DYMassFDY_mumu_all->Add(DYMassFDY_mumu_all ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_TT[i]).Data()  ) , 1, 1);
  } 

  for(unsigned int i=0; i<histoName_List_DY.size(); i++){
  
   DYMassCR_mumu_DY->Sumw2();
   DYMassCR_mumu_DY->Add(DYMassCR_mumu_DY ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   DYMassCR_mumu_all->Add(DYMassCR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);

   DYMassSR_mumu_DY->Sumw2();
   DYMassSR_mumu_DY->Add(DYMassSR_mumu_DY ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all->Add(DYMassSR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all_wDYScaled->Add(DYMassSR_mumu_all_wDYScaled ,(TH1F*)gROOT->FindObject(("DYMassSR_mumu_"+histoName_List_DY[i]).Data()  ) , 1, SFDY);

   DYMassemu_mumu->Add(DYMassemu_mumu , (TH1F*)gROOT->FindObject(  ("hlistemu_mumu_"+histoName_List_DY[i]).Data()  ), 1, 1);

   NjetsCR_inMZ_mumu_DY->Sumw2();
   NjetsSR_inMZ_mumu_DY->Sumw2();
   NjetsCR_outMZ_mumu_DY->Sumw2();
   NjetsSR_outMZ_mumu_DY->Sumw2();
   NjetsCR_inMZ_mumu_DY->Add(NjetsCR_inMZ_mumu_DY ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_DY->Add(NjetsSR_inMZ_mumu_DY ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   NjetsCR_inMZ_mumu_all->Add(NjetsCR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_all->Add(NjetsSR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_DY->Add(NjetsCR_outMZ_mumu_DY ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_DY->Add(NjetsSR_outMZ_mumu_DY ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_all->Add(NjetsCR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_all->Add(NjetsSR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);

   DYMassFDY_mumu_DY->Sumw2();
   DYMassFDY_mumu_DY->Add(DYMassFDY_mumu_DY ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
   DYMassFDY_mumu_all->Add(DYMassFDY_mumu_all ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_DY[i]).Data()  ) , 1, 1);
  } 

  for(unsigned int i=0; i<histoName_List_VV.size(); i++){

   DYMassCR_mumu_VV->Sumw2();
   DYMassCR_mumu_VV->Add(DYMassCR_mumu_VV ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   DYMassCR_mumu_all->Add(DYMassCR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);

   DYMassSR_mumu_VV->Sumw2();
   DYMassSR_mumu_VV->Add(DYMassSR_mumu_VV ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all->Add(DYMassSR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_NonDY->Add(DYMassSR_mumu_NonDY ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all_wDYScaled->Add(DYMassSR_mumu_all_wDYScaled ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);

   DYMassemu_mumu->Add(DYMassemu_mumu , (TH1F*)gROOT->FindObject(  ("hlistemu_mumu_"+histoName_List_VV[i]).Data()  ), 1, 1);

   NjetsCR_inMZ_mumu_VV->Sumw2();
   NjetsSR_inMZ_mumu_VV->Sumw2();
   NjetsCR_outMZ_mumu_VV->Sumw2();
   NjetsSR_outMZ_mumu_VV->Sumw2();
   NjetsCR_inMZ_mumu_VV->Add(NjetsCR_inMZ_mumu_VV ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_VV->Add(NjetsSR_inMZ_mumu_VV ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   NjetsCR_inMZ_mumu_all->Add(NjetsCR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_all->Add(NjetsSR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_VV->Add(NjetsCR_outMZ_mumu_VV ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_VV->Add(NjetsSR_outMZ_mumu_VV ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_all->Add(NjetsCR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_all->Add(NjetsSR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);

   DYMassFDY_mumu_VV->Sumw2();
   DYMassFDY_mumu_VV->Add(DYMassFDY_mumu_VV ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
   DYMassFDY_mumu_all->Add(DYMassFDY_mumu_all ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_VV[i]).Data()  ) , 1, 1);
  } 

  for(unsigned int i=0; i<histoName_List_SingleToptW.size(); i++){

   DYMassCR_mumu_SingleToptW->Sumw2();
   DYMassCR_mumu_SingleToptW->Add(DYMassCR_mumu_SingleToptW ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   DYMassCR_mumu_all->Add(DYMassCR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);

   DYMassSR_mumu_SingleToptW->Sumw2();
   DYMassSR_mumu_SingleToptW->Add(DYMassSR_mumu_SingleToptW ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all->Add(DYMassSR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_NonDY->Add(DYMassSR_mumu_NonDY ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all_wDYScaled->Add(DYMassSR_mumu_all_wDYScaled ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);

   DYMassemu_mumu->Add(DYMassemu_mumu , (TH1F*)gROOT->FindObject(  ("hlistemu_mumu_"+histoName_List_SingleToptW[i]).Data()  ), 1, 1);

   NjetsCR_inMZ_mumu_SingleToptW->Sumw2();
   NjetsSR_inMZ_mumu_SingleToptW->Sumw2();
   NjetsCR_outMZ_mumu_SingleToptW->Sumw2();
   NjetsSR_outMZ_mumu_SingleToptW->Sumw2();
   NjetsCR_inMZ_mumu_SingleToptW->Add(NjetsCR_inMZ_mumu_SingleToptW ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_SingleToptW->Add(NjetsSR_inMZ_mumu_SingleToptW ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   NjetsCR_inMZ_mumu_all->Add(NjetsCR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_all->Add(NjetsSR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_SingleToptW->Add(NjetsCR_outMZ_mumu_SingleToptW ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_SingleToptW->Add(NjetsSR_outMZ_mumu_SingleToptW ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_all->Add(NjetsCR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_all->Add(NjetsSR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);

   DYMassFDY_mumu_SingleToptW->Sumw2();
   DYMassFDY_mumu_SingleToptW->Add(DYMassFDY_mumu_SingleToptW ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
   DYMassFDY_mumu_all->Add(DYMassFDY_mumu_all ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_SingleToptW[i]).Data()  ) , 1, 1);
  } 

  for(unsigned int i=0; i<histoName_List_Wjets.size(); i++){

   DYMassCR_mumu_Wjets->Sumw2();
   DYMassCR_mumu_Wjets->Add(DYMassCR_mumu_Wjets ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   DYMassCR_mumu_all->Add(DYMassCR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);

   DYMassSR_mumu_Wjets->Sumw2();
   DYMassSR_mumu_Wjets->Add(DYMassSR_mumu_Wjets ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all->Add(DYMassSR_mumu_all ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_NonDY->Add(DYMassSR_mumu_NonDY ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   DYMassSR_mumu_all_wDYScaled->Add(DYMassSR_mumu_all_wDYScaled ,(TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);

   DYMassemu_mumu->Add(DYMassemu_mumu , (TH1F*)gROOT->FindObject(  ("hlistemu_mumu_"+histoName_List_Wjets[i]).Data()  ), 1, 1);

   NjetsCR_inMZ_mumu_Wjets->Sumw2();
   NjetsSR_inMZ_mumu_Wjets->Sumw2();
   NjetsCR_outMZ_mumu_Wjets->Sumw2();
   NjetsSR_outMZ_mumu_Wjets->Sumw2();
   NjetsCR_inMZ_mumu_Wjets->Add(NjetsCR_inMZ_mumu_Wjets ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_Wjets->Add(NjetsSR_inMZ_mumu_Wjets ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   NjetsCR_inMZ_mumu_all->Add(NjetsCR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   NjetsSR_inMZ_mumu_all->Add(NjetsSR_inMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_Wjets->Add(NjetsCR_outMZ_mumu_Wjets ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_Wjets->Add(NjetsSR_outMZ_mumu_Wjets ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   NjetsCR_outMZ_mumu_all->Add(NjetsCR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   NjetsSR_outMZ_mumu_all->Add(NjetsSR_outMZ_mumu_all ,(TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);

   DYMassFDY_mumu_Wjets->Sumw2();
   DYMassFDY_mumu_Wjets->Add(DYMassFDY_mumu_Wjets ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
   DYMassFDY_mumu_all->Add(DYMassFDY_mumu_all ,(TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_Wjets[i]).Data()  ) , 1, 1);
  } 

  for(unsigned int i=0; i<histoName_List_DataMu.size(); i++){
   DYMassCR_mumu_DataMu = (TH1F*)gROOT->FindObject(  ("DYMassCR_mumu_"+histoName_List_DataMu[i]).Data()  );
   DYMassSR_mumu_DataMu = (TH1F*)gROOT->FindObject(  ("DYMassSR_mumu_"+histoName_List_DataMu[i]).Data()  );
   NjetsCR_inMZ_mumu_DataMu = (TH1F*)gROOT->FindObject(  ("NjetsCR_inMZ_mumu_"+histoName_List_DataMu[i]).Data()  ) ;
   NjetsSR_inMZ_mumu_DataMu = (TH1F*)gROOT->FindObject(  ("NjetsSR_inMZ_mumu_"+histoName_List_DataMu[i]).Data()  ) ;
   NjetsCR_outMZ_mumu_DataMu = (TH1F*)gROOT->FindObject(  ("NjetsCR_outMZ_mumu_"+histoName_List_DataMu[i]).Data()  ) ;
   NjetsSR_outMZ_mumu_DataMu = (TH1F*)gROOT->FindObject(  ("NjetsSR_outMZ_mumu_"+histoName_List_DataMu[i]).Data()  ) ;
   DYMassFDY_mumu_DataMu = (TH1F*)gROOT->FindObject(  ("hlistNJ1LowMET_mumu_"+histoName_List_DataMu[i]).Data()  );
  } 



  TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2); 
  c1->SetLogy(0);

  
// Correct use of flags
//   bool FitEmuPart = true;
//   bool FitDYMC_SR_Part = true;
//   bool FitDYMC_CR_Part = true;
//   bool FitDYDATA_SR_Part = true;
//   bool FitDYDATA_CR_Part = true;
//   bool ValidityHypothesisTest = false;
//   bool Comparison_Part = true;
//   bool FDY_study = false;
  bool FitEmuPart = true;
  bool FitDYMC_SR_Part = true;
  bool FitDYMC_CR_Part = true;
  bool FitDYDATA_SR_Part = true;
  bool FitDYDATA_CR_Part = false;
  bool ValidityHypothesisTest = false;
  bool Comparison_Part = false;
  bool FDY_study = true;
// conversion factor for the integral computation
     float convfactor = (maxx-minx)/nb;
     vector<double> pa_mumu;
     
           
if ( FitEmuPart ) {
  //*********************
  // Fit of the emu continuum   
  //*********************
     TF1* myf = new TF1("CF",Continuum, 10., 350., 6); 
     myf->SetParameters(1.7256, -4.19201, .0000207, .849781, 125., 0.005);
     myf->SetLineWidth(3);
     DYMassemu_mumu->Draw();
     DYMassemu_mumu->Fit("CF", "M","same");
     double Iemu = myf->Integral(76.,106.);
     if ( DYVerbose )   std::cout<<"Integrale = "<<Iemu <<std::endl;
     for (unsigned int j=0; j<6; ++j){
       pa_mumu.push_back(myf->GetParameter(j));
         if ( DYVerbose ) { 
            std::cout<<"parametre["<<j<<"]"<< myf->GetParameter(j)<<std::endl;
	 }   
     }
} //FitEmuPart


     float NFittedDYMCinMz_SR = 0.;
if ( FitDYMC_SR_Part ){
  //*********************
  // Fit of in the SR region (MC)  
  //*********************
  
  cout<<"***********Fit in SR MC***********"<<endl;
     vector<double> paramSR;
     for (unsigned int j=0; j<6; ++j){
       paramSR.push_back(pa_mumu[j]);
     }

     TF1* myf1 = new TF1("BWplusCF",BreitWignerPlusFixedContinuum, 10., 300., 11); 
     myf1->SetParameters(paramSR[0], paramSR[1], paramSR[2] ,paramSR[3] ,paramSR[4] , paramSR[5], 10., 90., 1000., 10., 0.1);
     for (int j=0; j<6; ++j){
       myf1->SetParLimits(j,paramSR[j],paramSR[j]);
     }	

  // Fit with shape of the continuum (normalisation free) + Symetric BreitWigner   
     myf1->SetLineWidth(3);
     DYMassSR_mumu_all->Draw();
     DYMassSR_mumu_all->Fit("BWplusCF", "M","same");
     double Imumu = myf1->Integral(76.,106.);
     Imumu = Imumu/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integral SR [76;106]= "<<Imumu <<std::endl;
        std::cout<<"Integral SR [0;350]= "<< myf1->Integral(0.,350.)/convfactor<<std::endl;
     }	

     vector<double> paSR;
     for (unsigned int j=0; j<11; ++j){
       paSR.push_back(myf1->GetParameter(j));
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR parametre["<<j<<"] "<< myf1->GetParameter(j)<<std::endl;
       }	  
     }

     myf1->SetParameter(10,0.);
     double IBW = myf1->Integral(76.,106.);
     IBW = IBW/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integral BW [76;106]= "<<IBW<<std::endl;
        std::cout<<"Integral BW [0;350]= "<< myf1->Integral(0.,350.)/convfactor<<std::endl;
     }	

     myf1->SetParameter(10,paSR[10]);
     myf1->SetParameter(9,0.);
     
     for (unsigned int j=0; j<11; ++j){
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR parametre["<<j<<"] pour calcul contimuum "<< myf1->GetParameter(j)<<std::endl;
       } 	  
     }
     
     double IContinuum = myf1->Integral(76.,106.);
     IContinuum = IContinuum/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integral continuum SR [76;106]= "<< IContinuum<<std::endl;
        std::cout<<"Integral continuum SR [0;350]= "<< myf1->Integral(0.,350.)/convfactor<<std::endl;
     }	

//      if ( datatype=="MC" ) { 
//         SetNFittedDYMCinMz_SR(IBW);
// 	NDYMCinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
// 	NDYMCinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
//      }	
//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
//         ContinuumInZdata = IContinuum;
//         SetNFittedDATAinMz_SR(IBW);
//         NFittedDATAinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
//         NFittedDATAinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
//      }

       NFittedDYMCinMz_SR = IBW;
  
  cout<<"***********End of Fit in SR MC***********"<<endl;
} //FitDYMC_SR_Part    
  
// //// NE CONVIENT PAS POUR LA SELECTION SANS MET CUT
//   //*********************
//   // Fit of in the CR region   
//   //*********************
//   cout<<"***********Fit in CR***********"<<endl;
//      vector<double> paramCR;
//      for (unsigned int j=0; j<6; ++j){
//        paramCR.push_back(pa_mumu[j]);
//      }
// 
//      TF1* myf2 = new TF1("BWplusCF",BreitWignerPlusFixedContinuum, 10., 300., 11); 
//      myf2->SetParameters(paramCR[0], paramCR[1], paramCR[2] ,paramCR[3] ,paramCR[4] , paramCR[5], 10., 90., 100., 10., 0.1);
//      for (int j=0; j<6; ++j){
//        myf2->SetParLimits(j,paramCR[j],paramCR[j]);
//      }	
// 
// 
//      myf2->SetLineWidth(3);
//      DYMassCR_mumu_all->Draw();
// //     DYMassCR_mumu_DY->Draw("same");
//      DYMassCR_mumu_all->Fit("BWplusCF", "M","same");
// //      double I = myf2->Integral(76.,106.);
// //      double Itot = myf2->Integral(0.,350.);
// //      int numbin66 = DYMassCR_mumu_all->FindBin(66.);
// //      double N66 = DYMassCR_mumu_all->Integral(0.,numbin66);
// //      I = I/convfactor; 
// //      Itot = Itot/convfactor; 
// //      if ( DYVerbose ) { 
// //         std::cout<<"Integral CR [76;106]= "<<I <<std::endl;
// //         std::cout<<"Integrale CR [0;350]= "<< myf2->Integral(0.,350.)/convfactor<<std::endl;
// //         std::cout<<"N66 CR= "<<N66 <<std::endl;
// //      } 	
// // 
// //      vector<double> paCR;
// //      for (unsigned int j=0; j<11; ++j){
// //        paCR.push_back(myf2->GetParameter(j));
// //        if ( DYVerbose ) { 
// //           std::cout<<"MakeDYFits_CR parametre["<<j<<"] "<< myf2->GetParameter(j)<<std::endl;
// //        }	  
// //      }
// // 
// //      myf2->SetParameter(10,0.);
// //      double IBWCR = myf2->Integral(76.,106.);
// //      double IBW66 = myf2->Integral(66.,350.);
// //      double ItotIBW = myf2->Integral(0.,350.);
// //      double IBW = IBW/convfactor; 
// //      ItotIBW = ItotIBW/convfactor; 
// //      IBW66 = IBW66/convfactor; 
// //      if ( DYVerbose ) { 
// //         std::cout<<"Integral CR BW [76;106]= "<<IBW<<std::endl;
// //         std::cout<<"Integral CR BW [0;350]= "<< ItotIBW <<std::endl;
// //         std::cout<<"Integral CR BW [66;350]= "<< IBW66 <<std::endl;
// //      }	
// // 
// //      myf2->SetParameter(10,paCR[10]);
// //      myf2->SetParameter(9,0.);
// //      
// //      for (unsigned int j=0; j<11; ++j){
// //        if ( DYVerbose ) { 
// //           std::cout<<"MakeDYFits_CR parametre["<<j<<"] pour calcul contimuum "<< myf2->GetParameter(j)<<std::endl;
// // 	}  
// //      }
// //      
// //      double IContinuumCR = myf2->Integral(76.,106.);
// //      double IContinuum66 = myf2->Integral(0.,66.);
// //      IContinuumCR = IContinuumCR/convfactor; 
// //      IContinuum66 = IContinuum66/convfactor; 
// //      if ( DYVerbose ) { 
// //         std::cout<<"Integral continuum CR "<< IContinuumCR<<std::endl;
// //         std::cout<<"Integral continuum CR [0;350]= "<< myf2->Integral(0.,350.)/convfactor<<std::endl;
// //         std::cout<<"Integral continuum CR [0;66]= "<< IContinuum66<<std::endl;
// //      }	
// 
// 
// //      if ( datatype=="MC" ) { 
// //         SetNFittedDYMCinMz_CR(IBW) ;
// // //        SetNFittedDYMC_CR(ItotIBW-IBW) ;
// //         SetNFittedDYMC_CR(IBW66-IBW+ N66-IContinuum66) ;
// //      }	
// //      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
// //          SetNFittedDATAinMz_CR(IBW);
// // //	 NFittedDATA_CR = ItotIBW-IBW;
// // 	 NFittedDATA_CR = IBW66-IBW+ N66-IContinuum66;
// //          if ( DYVerbose ) { 
// // 	    std::cout<<"NFittedDATA_CR = "<<NFittedDATA_CR<<std::endl;
// // 	 }   
// //      }
// 
//   cout<<"***********End of Fit in CR***********"<<endl;

     float NFittedDYMC_CR     = 0.;
     float NFittedDYMCinMz_CR = 0.;
if ( FitDYMC_CR_Part ) {
  //*********************
  // Fit of in the CR region (MC)  
  //*********************
  cout<<"***********Fit in CR MC***********"<<endl;
  
// Fit of the DY in the Control Region 
     TF1* myf3 = new TF1("BWFPlusCE",AsymBreitWignerPlusCutEffectTest, 15., 300., 13); 
     
     if ( DY_withMETcut ) {
      myf3->SetParameter(0,2.6);
      myf3->SetParameter(1,90.);
      myf3->SetParameter(2,4000.);
      myf3->SetParameter(3,3.);
      myf3->SetParameter(4,0.);
//          myf3->SetParLimits(4,0.,0.);
      myf3->SetParameter(5,1.);
      myf3->SetParameter(6,1.);
//          myf3->SetParLimits(5,1.,1.);
//          myf3->SetParLimits(6,1.,1.);
      myf3->SetParameter(7,20.);
//          myf3->SetParLimits(7,0.,0.);
//          myf3->SetParLimits(7,0.,0.);
      myf3->SetParameter(8,45.);
//          myf3->SetParLimits(8,0.,0.);
//          myf3->SetParLimits(8,0.,0.);
      myf3->SetParameter(9,10.);
//          myf3->SetParLimits(9,0.,0.);
//          myf3->SetParLimits(9,0.,0.);
      myf3->SetParameter(10,50.);
      myf3->SetParameter(11,100.);
      myf3->SetParameter(12,10.);
//          myf3->SetParLimits(12,0.,0.);
//          myf3->SetParLimits(12,0.,0.);
      } else {
      myf3->SetParameter(0,2.6);
      myf3->SetParameter(1,90.);
      myf3->SetParameter(2,30000.);
      myf3->SetParameter(3,3.);
      myf3->SetParameter(4,0.);
//          myf3->SetParLimits(4,0.,0.);
      myf3->SetParameter(5,1.);
      myf3->SetParameter(6,1.);
//          myf3->SetParLimits(5,1.,1.);
//          myf3->SetParLimits(6,1.,1.);
      myf3->SetParameter(7,50.);
      myf3->SetParameter(8,40.);
      myf3->SetParameter(9,10.);
//      myf3->SetParameter(9,10.);
      myf3->SetParameter(10,50.);
      myf3->SetParameter(11,50.);
      myf3->SetParameter(12,50.);
      }

     myf3->SetLineWidth(3);
     DYMassCR_mumu_all->Draw();
     DYMassCR_mumu_all->Fit("BWFPlusCE", "RM","same");
     double I = myf3->Integral(76.,106.);
     I = I/convfactor; 
     double Itot = myf3->Integral(0.,350.);
     Itot = Itot/convfactor; 
     
     if ( DYVerbose ) { 
        std::cout<<"Integral BreitWigner asym plus CE = "<<I <<std::endl;
        std::cout<<"Integral BreitWigner[0;350] asym plus CE = "<<Itot <<std::endl;
//        std::cout<<"Integral Error BreitWigner[0;350] asym plus CE = "<< myf3->IntegralError(0.,350.)/convfactor<<std::endl;
     }	


//      if ( datatype=="MC" )  {
//         SetNFittedDYMCinMz_CR(I) ;
// 	SetNFittedDYMC_CR(Itot-I) ;
//      }
//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") {
//         SetNFittedDATAinMz_CR(I); 
// 	NFittedDATA_CR = Itot-I;
//      }
     NFittedDYMC_CR     = Itot-I;
     NFittedDYMCinMz_CR = I;
     
  cout<<"***********End of Fit in CR MC***********"<<endl;
} //FitDYMC_CR_Part  
  

if ( FitDYMC_CR_Part) {
  //*********************
  // Computation of DY estimate from fitted MC   
  //*********************

  float NDYEstimatedMC = NFittedDYMC_CR * NFittedDYMCinMz_SR / NFittedDYMCinMz_CR;
  float NFittedDYMC_SR = NDYEstimatedMC;

  if ( DYVerbose ) { 
     std::cout<<"Computation of DY estimate from fitted MC= "<<NDYEstimatedMC<<std::endl; 
     std::cout<<"                                         = "<<NFittedDYMC_CR<<" * "<<NFittedDYMCinMz_SR<<" / "<<NFittedDYMCinMz_CR<<std::endl; 
  } 
//   float stat_uncert = sqrt(( ((NunnormDYMC_CR+NunnormDYMCinMz_CR)/NunnormDYMCinMz_CR)*((NunnormDYMC_CR+NunnormDYMCinMz_CR)/NunnormDYMCinMz_CR) )
//                                  *(1./(NunnormDYMC_CR))
//                                  + 1./(NunnormDYMCinMz_SR) );
//   if ( DYVerbose ) { 
//      std::cout<<"statistical error on the number of ESTIMATED DY "<<NDYEstimatedMC*stat_uncert<<std::endl; 
//   }

  std::cout<<"Number of MC DY events in Z peak (for CR) = "<< NjetsCR_inMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"Number of MC DY events in Z peak (for SR) = "<< NjetsSR_inMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"Number of MC DY events outside the Z peak (for CR) = "<< NjetsCR_outMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"Number of MC DY events outside the Z peak (for SR) = "<< NjetsSR_outMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"Number of fitted MC DY events in Z peak (for CR) = "<< NFittedDYMCinMz_CR <<std::endl;
  std::cout<<"Number of fitted MC DY events in Z peak (for SR) = "<< NFittedDYMCinMz_SR <<std::endl;
  std::cout<<"Number of fitted MC DY events outside the Z peak (for CR) = "<< NFittedDYMC_CR <<std::endl;
  std::cout<<"Number of fitted MC DY events outside the Z peak (for SR) = "<< NFittedDYMC_SR <<std::endl;
  std::cout<<"===="<<std::endl;
  std::cout<<"Number of MC NON-DY events in Z peak (for CR) = "<<NjetsCR_inMZ_mumu_all->Integral()- NjetsCR_inMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"Number of MC NON-DY events outside the Z peak (for CR) = "<<NjetsCR_outMZ_mumu_all->Integral()- NjetsCR_outMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"===="<<std::endl;

// MC comparison between real and expected yields
  std::cout<<"Number of MC DY events outside the Z peak (for SR) = "<< NjetsSR_outMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"Number of ESTIMATED by fitting MC DY events outside the Z peak (for SR) = "<< NFittedDYMC_SR <<std::endl;
  std::cout<<"Number of ESTIMATED by counting MC DY events outside the Z peak (for SR) = "<<NjetsCR_outMZ_mumu_DY->Integral()*NjetsSR_inMZ_mumu_DY->Integral()/NjetsCR_inMZ_mumu_DY->Integral()  <<std::endl;
} //FitDYMC_CR_Part


     double IContinuum_Data = 0.;
     float NFittedDatainMz_SR = 0.;
if ( FitDYDATA_SR_Part) {
  //*********************
  // Fit of in the SR region (DATA)  
  //*********************
  
  cout<<"***********Fit in SR DATA***********"<<endl;
     vector<double> paramSR_Data;
     for (unsigned int j=0; j<6; ++j){
       paramSR_Data.push_back(pa_mumu[j]);
     }

     TF1* myf10 = new TF1("BWplusCF",BreitWignerPlusFixedContinuum, 10., 300., 11); 
     myf10->SetParameters(paramSR_Data[0], paramSR_Data[1], paramSR_Data[2] ,paramSR_Data[3] ,paramSR_Data[4] , paramSR_Data[5], 10., 90., 1000., 10., 0.1);
     for (int j=0; j<6; ++j){
       myf10->SetParLimits(j,paramSR_Data[j],paramSR_Data[j]);
     }	

  // Fit with shape of the continuum (normalisation free) + Symetric BreitWigner   
     myf10->SetLineWidth(3);
     DYMassSR_mumu_DataMu->Draw();
     DYMassSR_mumu_DataMu->Fit("BWplusCF", "M","same");
     double Tot0 = myf10->Integral(76.,106.)/convfactor;
     if ( PlusOneSigmaBkgInSR ) {
        double Val = myf10->GetParameter(10); double ErrVal = myf10->GetParError(10);
//        myf10->SetParameter(10,Val+ErrVal); 
        myf10->SetParameter(10,Val+(0.3*Val));
     }	
     if ( MinusOneSigmaBkgInSR ) {
        double Val = myf10->GetParameter(10); double ErrVal = myf10->GetParError(10);
//        myf10->SetParameter(10,Val-ErrVal);
        myf10->SetParameter(10,Val-(0.3*Val));
     }	
     double Tot1 = myf10->Integral(76.,106.)/convfactor;
     double variation = Tot1-Tot0;
     cout<<"variation "<<variation<<endl;
     double Imumu_Data = myf10->Integral(76.,106.);
     Imumu_Data = Imumu_Data/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integral SR [76;106] in Data= "<<Imumu_Data <<std::endl;
        std::cout<<"Integral SR [0;350] in Data= "<< myf10->Integral(0.,350.)/convfactor<<std::endl;
        std::cout<<"Integral Error BW [0;350] in Data= "<< myf10->IntegralError(0.,350.)/convfactor<<std::endl;
        std::cout<<"parameter 10, error "<< myf10->GetParameter(10)<<" "<<myf10->GetParError(10) <<std::endl;
     }

     vector<double> paSR_Data;
     for (unsigned int j=0; j<11; ++j){
       paSR_Data.push_back(myf10->GetParameter(j));
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR parametre["<<j<<"] "<< myf10->GetParameter(j)<<std::endl;
       }	  
     }

     myf10->SetParameter(10,0.);
     double IBW_Data = myf10->Integral(76.,106.);
     IBW_Data = IBW_Data/convfactor; 
     cout<<"before "<<IBW_Data<<" "<<variation<<std::endl;
     if ( PlusOneSigmaBkgInSR || MinusOneSigmaBkgInSR ) IBW_Data -= variation;
     
     if ( DYVerbose ) { 
        std::cout<<"Integral BW [76;106] in Data= "<<IBW_Data<<std::endl;
        std::cout<<"Integral BW [0;350] in Data= "<< myf10->Integral(0.,350.)/convfactor<<std::endl;
     }	

     myf10->SetParameter(10,paSR_Data[10]);
     myf10->SetParameter(9,0.);
     
     for (unsigned int j=0; j<11; ++j){
       if ( DYVerbose ) { 
          std::cout<<"MakeDYFits_SR parametre["<<j<<"] pour calcul contimuum "<< myf10->GetParameter(j)<<std::endl;
       } 	  
     }
     
     IContinuum_Data = myf10->Integral(76.,106.);
     IContinuum_Data = IContinuum_Data/convfactor; 
     if ( DYVerbose ) { 
        std::cout<<"Integral continuum SR [76;106] in Data= "<< IContinuum_Data<<std::endl;
        std::cout<<"Integral continuum SR [0;350] in Data= "<< myf10->Integral(0.,350.)/convfactor<<std::endl;
     }	

//      if ( datatype=="MC" ) { 
//         SetNFittedDYMCinMz_SR(IBW);
// 	NDYMCinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
// 	NDYMCinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
//      }	
//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") { 
//         ContinuumInZdata = IContinuum;
//         SetNFittedDATAinMz_SR(IBW);
//         NFittedDATAinMzWithScaledPlusContinuum_SR = IBW + sqrt(IContinuum);
//         NFittedDATAinMzWithScaledMinusContinuum_SR = IBW - sqrt(IContinuum);
//      }

       NFittedDatainMz_SR = IBW_Data;
  
  cout<<"***********End of Fit in SR DATA***********"<<endl;
}//  FitDYDATA_SR_Part 


// /// Mauvais fit   
//   //*********************
//   // Fit of in the CR region (DATA)  
//   //*********************
//   
//   cout<<"***********Fit in CR DATA***********"<<endl;
//      vector<double> paramCR_Data;
//      for (unsigned int j=0; j<6; ++j){
//        paramCR_Data.push_back(pa_mumu[j]);
//      }
// 
//      TF1* myf11 = new TF1("BWplusCF",BreitWignerPlusFixedContinuum, 10., 300., 11); 
//      myf11->SetParameters(paramCR_Data[0], paramCR_Data[1], paramCR_Data[2] ,paramCR_Data[3] ,paramCR_Data[4] , paramCR_Data[5], 10., 90., 1000., 10., 0.1);
//      for (int j=0; j<6; ++j){
//        myf11->SetParLimits(j,paramCR_Data[j],paramCR_Data[j]);
//      }
//      	
//   // Fit with shape of the continuum (normalisation free) + Symetric BreitWigner   
//      myf11->SetLineWidth(3);
//      DYMassCR_mumu_DataMu->Draw();
//      DYMassCR_mumu_DataMu->Fit("BWplusCF", "M","same");
//      double Imumu_Data_CR = myf11->Integral(76.,106.);
//      Imumu_Data_CR = Imumu_Data_CR/convfactor; 
//      if ( DYVerbose ) { 
//         std::cout<<"Integral CR [76;106] in Data= "<<Imumu_Data_CR <<std::endl;
//         std::cout<<"Integral CR [0;350] in Data= "<< myf11->Integral(0.,350.)/convfactor<<std::endl;
//      }	
// 
// 
// 
// 
// 
//   cout<<"***********End of Fit in CR DATA***********"<<endl;
  
if ( FitDYDATA_CR_Part) {
  //*********************
  // Fit of in the CR region (DATA)  
  //*********************
  cout<<"***********Fit in CR DATA***********"<<endl;
  
// Fit of the DY in the Control Region 
     TF1* myf11;
     if ( !IncludeBkgInCR ) { 
         myf11 = new TF1("BWFPlusCE",AsymBreitWignerPlusCutEffectTest, 15., 300., 13); 
     } else{ 	 
        myf11 = new TF1("BWFPlusCE",AsymBreitWignerPlusCutEffectTestWthContinuum, 15., 300., 19); 
     } 	
     
     if ( DY_withMETcut ) {
      myf11->SetParameter(0,2.6);
      myf11->SetParameter(1,90.);
      myf11->SetParameter(2,4000.);
      myf11->SetParameter(3,3.);
      myf11->SetParameter(4,0.);
//          myf11->SetParLimits(4,0.,0.);
      myf11->SetParameter(5,1.);
      myf11->SetParameter(6,1.);
//          myf11->SetParLimits(5,1.,1.);
//          myf11->SetParLimits(6,1.,1.);
      myf11->SetParameter(7,20.);
//          myf11->SetParLimits(7,0.,0.);
//          myf11->SetParLimits(7,0.,0.);
      myf11->SetParameter(8,45.);
//          myf11->SetParLimits(8,0.,0.);
//          myf11->SetParLimits(8,0.,0.);
      myf11->SetParameter(9,10.);
//          myf11->SetParLimits(9,0.,0.);
//          myf11->SetParLimits(9,0.,0.);
      myf11->SetParameter(10,50.);
      myf11->SetParameter(11,100.);
      myf11->SetParameter(12,10.);
//          myf11->SetParLimits(12,0.,0.);
//          myf11->SetParLimits(12,0.,0.);
      if ( IncludeBkgInCR ) { 
        myf11->SetParameter(13,1.);
        myf11->SetParLimits(14,pa_mumu[5],pa_mumu[5]);
        myf11->SetParLimits(15,pa_mumu[0],pa_mumu[0]);
        myf11->SetParLimits(16,pa_mumu[1],pa_mumu[1]);
        myf11->SetParLimits(17,pa_mumu[3],pa_mumu[3]);
        myf11->SetParLimits(18,pa_mumu[2],pa_mumu[2]);
      }
     } else {
      myf11->SetParameter(0,2.6);
      myf11->SetParameter(1,90.);
      myf11->SetParameter(2,30000.);
      myf11->SetParameter(3,3.);
      myf11->SetParameter(4,0.);
//          myf11->SetParLimits(4,0.,0.);
      myf11->SetParameter(5,1.);
      myf11->SetParameter(6,1.);
//          myf11->SetParLimits(5,1.,1.);
//          myf11->SetParLimits(6,1.,1.);
      myf11->SetParameter(7,50.);
      myf11->SetParameter(8,40.);
      myf11->SetParameter(9,10.);
//      myf11->SetParameter(9,10.);
      myf11->SetParameter(10,50.);
      myf11->SetParameter(11,50.);
      myf11->SetParameter(12,50.);
     }

     myf11->SetLineWidth(3);
     DYMassCR_mumu_DataMu->Draw();
     DYMassCR_mumu_DataMu->Fit("BWFPlusCE", "RM","same");
     double I_DATA = myf11->Integral(76.,106.);
     double I_DATA_error = myf11->IntegralError(76.,106.)/convfactor;
     I_DATA = I_DATA/convfactor; 
     double Itot_DATA = myf11->Integral(10.,350.);
     Itot_DATA = Itot_DATA/convfactor; 
     double Itot_DATA_error = myf11->IntegralError(10.,350.)/convfactor;

// test for continuum effect

     double Itot_DATA_Continuum_outsideZ = 0.;
     double Itot_DATA_Continuum_insideZ = 0.;
     if ( IncludeBkgInCR ) {
       myf11->SetParameter(3,0.);
       myf11->SetParameter(7,0.0000001);
       myf11->SetParameter(12,0.);
       Itot_DATA_Continuum_outsideZ = myf11->Integral(10.,76.)+myf11->Integral(106.,350.);
       Itot_DATA_Continuum_insideZ = myf11->Integral(76.,106.);
       myf11->Draw("same");
     }
     
     if ( DYVerbose ) { 
        std::cout<<"Integral BreitWigner [76;106] asym plus CE (DATA)= "<<I_DATA <<std::endl;
        std::cout<<"Integral Error BreitWigner [76;106] asym plus CE (DATA)= "<<I_DATA_error <<std::endl;
        std::cout<<"Integral BreitWigner[10;350] asym plus CE (DATA)= "<<Itot_DATA <<std::endl;
        std::cout<<"Integral Error BreitWigner[10;350] asym plus CE (DATA)= "<<Itot_DATA_error <<std::endl;
        std::cout<<"Integral Continuum out/in Z= "<< Itot_DATA_Continuum_outsideZ <<" "<<Itot_DATA_Continuum_insideZ <<std::endl;
     }	


//      if ( datatype=="MC" )  {
//         SetNFittedDYMCinMz_CR(I) ;
// 	SetNFittedDYMC_CR(Itot-I) ;
//      }
//      if ( datatype=="Data" || datatype=="DATA" || datatype=="DataMu" || datatype=="DataEG") {
//         SetNFittedDATAinMz_CR(I); 
// 	NFittedDATA_CR = Itot-I;
//      }
     float NFittedDYDATA_CR     = Itot_DATA-I_DATA;
     float NFittedDYDATAinMz_CR = I_DATA;
     
  cout<<"***********End of Fit in CR DATA***********"<<endl;
  
if ( ValidityHypothesisTest ) {  
  double norm1 = DYMassSR_mumu_DY->Integral();
  DYMassSR_mumu_DY->Scale(1./norm1);
  DYMassSR_mumu_DY->SetLineColor(2);
  DYMassSR_mumu_DY->Draw();
  double norm2 = DYMassCR_mumu_DY->Integral();
  DYMassCR_mumu_DY->Scale(1./norm2);
  DYMassCR_mumu_DY->Draw("same");
  TH1F* DYMass_r1 = new TH1F("DYMass_r1", "DYMass_r1", Dum->GetNbinsX() , Dum->GetXaxis()->GetXmin() , Dum->GetXaxis()->GetXmax());
  DYMass_r1->Divide(DYMassSR_mumu_DY,DYMassCR_mumu_DY);
  DYMass_r1->SetMinimum(-5);
  DYMass_r1->SetMaximum(5);
  DYMass_r1->Draw();
  DYMass_r1->Fit("pol0");
  cout<<"comparison for N_in_SR/N_in_CR "<<endl;
  DYMass_r1->Fit("pol0","r","",76.,106.);
  double meanInZ = DYMass_r1->GetFunction("pol0")->GetParameter(0);
  cout<<"comparison for N_out_SR/N_out_CR "<<endl;
  DYMass_r1->Fit("pol0","r","",15.,76.);
  double meanOutZ1 = DYMass_r1->GetFunction("pol0")->GetParameter(0);
  double EmeanOutZ1 = DYMass_r1->GetFunction("pol0")->GetParError(0);
  DYMass_r1->Fit("pol0","r","",106.,350.);
  double meanOutZ2 = DYMass_r1->GetFunction("pol0")->GetParameter(0);
  double EmeanOutZ2 = DYMass_r1->GetFunction("pol0")->GetParError(0);
  double weightedmean = ((meanOutZ1)/(EmeanOutZ1*EmeanOutZ1) + (meanOutZ2)/(EmeanOutZ2*EmeanOutZ2))/(1./(EmeanOutZ1*EmeanOutZ1)+1./(EmeanOutZ2*EmeanOutZ2));
  cout<<"moyenne hors pic= "<<weightedmean<<endl;
  cout<<"difference absolue = "<<TMath::Abs(meanInZ-weightedmean)<<endl;
  
} //ValidityHypothesisTest  



// Compute the number of expected bckg according to 
// "Counting" method (simply count the number of events in each zones) or from a "Fit" method of the inv.mass distribution (effect of others process bckg for DY can be taken into account)
// "MC" or "DATA" origin for the scale factor (NofEventsintheMZpieak(inSR)/NofEventsintheMZpieak(inCR))
  std::cout<<""<<std::endl; 
  std::cout<<"==== Computation of DY_mumu estimate (Counting) ==== "<<std::endl; 
  std::cout<<"Counted Number of Data events outside the Z peak (for SR)                              = "<<NjetsSR_outMZ_mumu_DataMu->GetEntries() <<std::endl;
  std::cout<<"Counted Number of Data events outside the Z peak (for CR)                              = "<<NjetsCR_outMZ_mumu_DataMu->GetEntries() <<std::endl;
  std::cout<<"Fraction of DY events outside the Z peak (for CR) = "<<NjetsCR_outMZ_mumu_DY->GetEntries()/NjetsCR_outMZ_mumu_all->GetEntries() <<std::endl;


  std::cout<<"Counted Number of Data events outside the Z peak (for CR, with MCbkg subtraction)   = "<<NjetsCR_outMZ_mumu_DataMu->GetEntries()*NjetsCR_outMZ_mumu_DY->GetEntries()/NjetsCR_outMZ_mumu_all->GetEntries()  <<std::endl;
//  double R1 = NjetsCR_outMZ_mumu_DataMu->GetEntries()*NjetsCR_outMZ_mumu_DY->GetEntries()/NjetsCR_outMZ_mumu_all->GetEntries();
  std::cout<<"Counted Number of Data events outside the Z peak (for CR) (1)                          = "<<NjetsCR_outMZ_mumu_DataMu->GetEntries() <<std::endl;
  double R1 = NjetsCR_outMZ_mumu_DataMu->GetEntries();
  std::cout<<"Counted Number of Data events inside the Z peak (for SR) (2)                           = "<<NjetsSR_inMZ_mumu_DataMu->GetEntries() <<std::endl;
  double R2 = NjetsSR_inMZ_mumu_DataMu->GetEntries() ;
  std::cout<<"Counted Number of Continuum inside the Z peak    (for SR)(3)                           = "<<IContinuum_Data  <<std::endl;
  double R3 = IContinuum_Data;
  std::cout<<"Counted Number of Data events inside the Z peak (for CR) (4)                           = "<<NjetsCR_inMZ_mumu_DataMu->GetEntries() <<std::endl;
  double R4 = NjetsCR_inMZ_mumu_DataMu->GetEntries();
//// Stat Error for counting:
  double StatErrorDataCounting = (R1*(R2-R3)/R4) * 
                                  sqrt( (R1+R4)*(R1+R4)/(R4*R4)/R1 + 1./(R2-R3))    
  ;    
  std::cout<<"Counted Number of ESTIMATED DY events outside the Z peak (for SR) [(1)*((2)-(3))/(4)]  =  "<<R1*(R2-R3)/R4 <<" +- " <<StatErrorDataCounting<<" (stat)"<< std::endl;

  std::cout<<"======== "<<std::endl; 
  std::cout<<"==== Computation of DY_mumu estimate (Fitting) ==== "<<std::endl; 
  std::cout<<"Fitted Number of DY events outside the Z peak (for CR) (1)                            = "<<NFittedDYDATA_CR <<std::endl;
  double R1Fit = NFittedDYDATA_CR;
  std::cout<<"Fitted Number of DY events inside the Z peak  (for SR) (2)                            = "<<NFittedDatainMz_SR <<std::endl;
  double R2Fit = NFittedDatainMz_SR;
  std::cout<<"Fitted Number of DY events inside the Z peak  (for CR) (3)                            = "<<NFittedDYDATAinMz_CR <<std::endl;
  double R3Fit =  NFittedDYDATAinMz_CR;
  std::cout<<"Continuum outside the Z peak (for CR)                  (4)                            = "<<Itot_DATA_Continuum_outsideZ <<std::endl;
  double R4Fit = Itot_DATA_Continuum_outsideZ ;
  std::cout<<"Continuum inside the Z peak (for CR)                   (5)                            = "<<Itot_DATA_Continuum_insideZ <<std::endl;
  double R5Fit = Itot_DATA_Continuum_insideZ ;
  double Res = (NFittedDYDATA_CR-Itot_DATA_Continuum_outsideZ)*NFittedDatainMz_SR/(NFittedDYDATAinMz_CR-Itot_DATA_Continuum_insideZ);
  std::cout<<"Fitting Number of ESTIMATED DY events outside the Z peak (for SR) [(1-4)*(2)/(3-5)]       =  "<<(NFittedDYDATA_CR-Itot_DATA_Continuum_outsideZ)*NFittedDatainMz_SR/(NFittedDYDATAinMz_CR-Itot_DATA_Continuum_insideZ) <<std::endl;
  std::cout<<"to be compared with the number of MC DY events outside the Z peak (for SR) = "<< NjetsSR_outMZ_mumu_DY->Integral() <<std::endl;
  std::cout<<"==== Errors ==== "<<std::endl; 
/// Good approximation  
   double staterror = (R1Fit*R2Fit/R3Fit)*sqrt( ((R1Fit+R3Fit)/R3Fit)*((R1Fit+R3Fit)/R3Fit)/R1Fit +1./R2Fit );
  std::cout<<"statistical error (fitting) =  "<< staterror <<std::endl;
// Systematics:
// Presence du BdF dans CR (traite avec IncludeBkgInCR a true ): (383.4-364.1)/383.1 = 5%
// Presence du Bdf dans SR (traite avec PlusOneSigmaBkgInSR ou MinusOneSigmaBkgInSR ) : Max ( |383.4-367|,|383.4-400|)/383.4 = 4.5% 
// Validite de l'hypothese N_out_SR/N_in_SR = N_out_CR/N_in_CR  (traite avec  ValidityHypothesisTest ): 4%
// Modelisation du fit: ~20% (difference entre counting et fitting sur le lot DY MC)
   double syserror = Res * sqrt(0.05*0.05+0.045*0.045+0.04*0.04+0.20*0.20);
   std::cout<<"systematical error =  +-"<<syserror <<std::endl;
   double toterror = sqrt(staterror*staterror+syserror*syserror);
   double reltoterror = sqrt(staterror*staterror+syserror*syserror)/Res;
   std::cout<<"======== "<<std::endl; 
   std::cout<<"Scale factor  = "<< Res/NjetsSR_outMZ_mumu_DY->Integral()<<" +- "<<Res/NjetsSR_outMZ_mumu_DY->Integral()*reltoterror <<std::endl;
   std::cout<<""<<std::endl; 
// 

   if ( Comparison_Part ) {  
     float Ma = DYMassSR_mumu_DataMu->GetMaximum();
     DYMassSR_mumu_DataMu->SetMaximum(Ma*1.15);
     DYMassSR_mumu_DataMu->GetXaxis()->SetRangeUser(70.,110.);   
     DYMassSR_mumu_DataMu->SetMarkerStyle(20);   
     DYMassSR_mumu_DataMu->SetLineWidth(3);
     DYMassSR_mumu_DataMu->Draw();
     DYMassSR_mumu_all->SetLineStyle(2);
     DYMassSR_mumu_all->SetLineColor(2);
     DYMassSR_mumu_all->SetLineWidth(3);
     DYMassSR_mumu_all->Draw("same hist");
     DYMassSR_mumu_NonDY->SetLineStyle(3);
     DYMassSR_mumu_NonDY->SetLineWidth(3);
     DYMassSR_mumu_NonDY->Draw("same hist");
     DYMassSR_mumu_all_wDYScaled->SetLineWidth(3);
     DYMassSR_mumu_all_wDYScaled->Draw("same hist");

     TLegend* qw = new TLegend(0.15,0.20,0.45, 0.45);
     //qw->SetHeader("CMS Preliminary");
     qw->AddEntry(DYMassSR_mumu_DataMu,       "Data ","P");
     qw->AddEntry(DYMassSR_mumu_all,      " All summed MC ","L");
     qw->AddEntry(DYMassSR_mumu_all_wDYScaled,      " All summed MC with scaled DY ","L");
     qw->AddEntry(DYMassSR_mumu_NonDY,       "Non DY MC ","L");
     qw->SetTextSize(0.018);

     qw->Draw();
   }  
     



}//FitDYDATA_CR_Part

if ( FDY_study ) {  

     float N1 = DYMassFDY_mumu_DataMu->Integral();
     DYMassFDY_mumu_DataMu->Scale(1./N1);
     float N2 = DYMassFDY_mumu_all->Integral();
     DYMassFDY_mumu_all->Scale(1./N2);
     
     DYMassFDY_mumu_DataMu->GetXaxis()->SetRangeUser(0.,150.);;   
     DYMassFDY_mumu_DataMu->SetMarkerStyle(20);   
     DYMassFDY_mumu_DataMu->SetLineWidth(3);
     DYMassFDY_mumu_DataMu->Draw();
     DYMassFDY_mumu_all->SetLineWidth(3);
     DYMassFDY_mumu_all->SetLineColor(2);
     DYMassFDY_mumu_all->Draw("same hist");
     DYMassFDY_mumu_DataMu->Draw("same");
     TLegend* qw = new TLegend(0.15,0.20,0.45, 0.45);
     qw->AddEntry(DYMassFDY_mumu_DataMu,       "Data (njet<=1 && MET<15GeV)","P");
     qw->AddEntry(DYMassFDY_mumu_all,      " All summed MC ","L");
     qw->SetTextSize(0.018);
     qw->Draw();



//      float N1 = DYMassSR_mumu_DataMu->Integral();
//      DYMassSR_mumu_DataMu->Scale(1./N1);
//      float N2 = DYMassSR_mumu_all->Integral();
//      DYMassSR_mumu_all->Scale(1./N2);
// 
//      DYMassSR_mumu_DataMu->GetXaxis()->SetRangeUser(0.,150.);;   
//      DYMassSR_mumu_DataMu->SetMarkerStyle(20);   
//      DYMassSR_mumu_DataMu->SetLineWidth(3);
//      DYMassSR_mumu_DataMu->Draw();
//      DYMassSR_mumu_all->SetLineWidth(3);
//      DYMassSR_mumu_all->SetLineColor(2);
//      DYMassSR_mumu_all->Draw("same hist");
//      
//      TLegend* qw = new TLegend(0.15,0.20,0.45, 0.45);
//      qw->AddEntry(DYMassSR_mumu_DataMu,       "Data ","P");
//      qw->AddEntry(DYMassSR_mumu_all,      " All summed MC ","L");
//      qw->SetTextSize(0.018);
//      qw->Draw();

}



}


