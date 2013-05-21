
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



//*************************************************
//*************************************************
//compute R_outin in constrol  region enriched in Z
//*************************************************
//*************************************************

void Rout_in(TString channel, TString plot){
  
  
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  
  TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2); 
  c1->SetLogy(0);
 
  
  
  
  
//   TFile *f_data  = new TFile("CrossSection_noWeights2.root");
//   //TFile *f_data  = new TFile("CrossSection_new.root");
//  TFile *f_data  = new TFile("proof_marcosample_all.root");
  TFile *f_data  = new TFile("../backup_outputProof02-11-11_13-13-58/proof.root");
  f_data->cd();

  
  std::vector<TH2F*> histo_List;
  std::vector<TString> histoName_List;
  histoName_List.push_back("TTbarSig");
  histoName_List.push_back("TTbarBkg");
  histoName_List.push_back("Zjets");
  //histoName_List.push_back("DYToMuMu_M-20");
  histoName_List.push_back("DYToEE_M-20");
  histoName_List.push_back("DYToTauTau_M-20");
  histoName_List.push_back("Wjets");
  histoName_List.push_back("TtW");
  histoName_List.push_back("TbartW");
  histoName_List.push_back("WW");
  histoName_List.push_back("ZZ");
  histoName_List.push_back("WZ");
  
  TString theplot;
  if(plot == "NJet")    theplot= "_pair_vs_NJet_METinf10_";
  if(plot == "MET")     theplot= "_pair_vs_MET_NJet0_";
  if(plot == "METSel")  theplot= "_pair_vs_MET_sup2Jet_";
  if(plot == "PU")      theplot= "_pair_vs_PU_";
  
  TH2F *zMass_Data_vs_NJets;
  if( channel == "ee")   zMass_Data_vs_NJets = (TH2F*)gROOT->FindObject(("HInvM_"+channel+theplot+"DataEG").Data());
  if( channel == "mumu") zMass_Data_vs_NJets = (TH2F*)gROOT->FindObject(("HInvM_"+channel+theplot+"DataMu").Data());
  

  
  for(unsigned int i=0; i<histoName_List.size(); i++){
    histo_List.push_back( (TH2F*)gROOT->FindObject(  ("HInvM_"+channel+theplot+histoName_List[i]).Data()  ));
  }
 
 
   
  //*********************
  // summ up MC templates
  //*********************
  TH2F * totMC = new TH2F("totMC", "totMC", zMass_Data_vs_NJets->GetNbinsX() , zMass_Data_vs_NJets->GetXaxis()->GetXmin() , zMass_Data_vs_NJets->GetXaxis()->GetXmax(), zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax() );
  
   
  TH2F * totMC_Z = new TH2F("totMC_Z", "totMC_Z", zMass_Data_vs_NJets->GetNbinsX() , zMass_Data_vs_NJets->GetXaxis()->GetXmin() , zMass_Data_vs_NJets->GetXaxis()->GetXmax(), zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax() );
  
  //*********************
  for(unsigned int i=0; i<histoName_List.size(); i++){
   histo_List[i]->Sumw2();
   if( histoName_List[i] == "Zjets" ||histoName_List[i] == "DYToMuMu_M-20" 
   ||histoName_List[i] == "DYToEE_M-20"  ||histoName_List[i] == "DYToTauTau_M-20") totMC_Z->Add( totMC_Z, histo_List[i], 1, 1);
   totMC->Add( totMC, histo_List[i], 1, 1);
  }

  
  
  int npoint = zMass_Data_vs_NJets->GetNbinsY();
  
  
  
  
  // ************** for Data 
  // ************** for Data 
  // ************** for Data 
  // ************** for Data 
  
   
  double Routin_Data[npoint];
  double Routin_MC[npoint];
  double Routin_DataOverMC[npoint];
  double Njets[npoint];
  

  double RoutinErr_Data[npoint];
  double RoutinErr_MC[npoint];
  double RoutinErr_DataOverMC[npoint];
  double NjetsErr[npoint];
  
  
  double ratioN_Others[npoint];
  double ratioNErr_Others[npoint];
  double norm = 1.;
  if(plot == "METSel" || plot == "MET") norm = 150./30.;//
  
  
  for(unsigned int biny=1; biny<zMass_Data_vs_NJets->GetNbinsY()+1; biny++){
    
    double num   = 0;
    double denom = 0;
    
    for(unsigned int binx=1; binx < (zMass_Data_vs_NJets->GetNbinsX()+1); binx++){
       if( binx < 76  || binx >= 106 && binx > 12 )  num+= zMass_Data_vs_NJets->GetBinContent(binx, biny);
       if( binx >= 76 && binx <  106 )               denom+= zMass_Data_vs_NJets->GetBinContent(binx, biny);
      
    }
   
    Routin_Data[biny-1]    = num/denom;
    RoutinErr_Data[biny-1] = pow( pow((1/denom)*pow(num,0.5),2) +  pow((num/(denom*denom))*pow(denom, 0.5),2), 0.5);
    
    Njets[biny-1] = (biny-1)*norm;
    if(num   == 0) Routin_MC[biny-1] = -1;
    if(denom == 0) Routin_MC[biny-1] = -1;
    if(denom == 0 || num   == 0) Njets[biny-1] = -10;
    NjetsErr[biny-1] = 0;
    
  }
  
   
  TGraphErrors * theGraphData = new TGraphErrors(npoint, Njets, Routin_Data, NjetsErr,RoutinErr_Data );
  
  
  // ************** for MC 
  // ************** for MC 
  // ************** for MC 
  // ************** for MC 
  // ************** for MC 
  for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
    
    double num   = 0;
    double denom = 0;
    
    double ntot=0;
    double nZ = 0;
    
    for(unsigned int binx=1; binx < (totMC->GetNbinsX()+1); binx++){
       if( binx < 76  || binx >= 106 && binx > 12 )  num+= totMC->GetBinContent(binx, biny);
       if( binx >= 76 && binx <  106 )               denom+= totMC->GetBinContent(binx, biny);
     
       ntot += totMC->GetBinContent(binx, biny);
       nZ   += totMC_Z->GetBinContent(binx, biny);
    }
    
    Routin_MC[biny-1] = num/denom;
    RoutinErr_MC[biny-1] = pow( pow((1/denom)*pow(num,0.5),2) +  pow((num/(denom*denom))*pow(denom, 0.5),2), 0.5);
    
//     if( channel == "mumu") Routin_MC[biny-1] *= 1.25;
//     if( channel == "ee"  ) Routin_MC[biny-1] *= 1.08;
//     if( channel == "mumu" && plot == "METSel")  Routin_MC[biny-1] *= 1.39;
//     if( channel == "ee"   && plot == "METSel")  Routin_MC[biny-1] *= 1.29;
//     if( channel == "mumu" && plot == "MET")  Routin_MC[biny-1] *= 0.957;
//     if( channel == "ee"   && plot == "MET")  Routin_MC[biny-1] *= 0.979;
    if( channel == "mumu") Routin_MC[biny-1] *= 1.25;
    if( channel == "ee"  ) Routin_MC[biny-1] *= 1.09;
    if( channel == "mumu" && plot == "METSel")  Routin_MC[biny-1] *= 1.00;
    if( channel == "ee"   && plot == "METSel")  Routin_MC[biny-1] *= 1.00;
    if( channel == "mumu" && plot == "MET")  Routin_MC[biny-1] *= 0.953;
    if( channel == "ee"   && plot == "MET")  Routin_MC[biny-1] *= 0.957;
    
    Njets[biny-1] = (biny-1)*norm;
   
    
    ratioN_Others[biny-1]    = nZ/ntot;
    ratioNErr_Others[biny-1] =  pow(ratioN_Others[biny-1]*(1-ratioN_Others[biny-1])/ntot,0.5);
    
    
    if(num   == 0) Routin_MC[biny-1] = -1;
    if(denom == 0) Routin_MC[biny-1] = -1;
    if(denom == 0 || num   == 0) Njets[biny-1] = -10;
    NjetsErr[biny-1] = 0;
    
    if(plot == "NJet" || plot == "MET" ||plot == "PU" ){
      cout << "Routin_MC   for Njet = " << Njets[biny-1] << " is " << Routin_MC[biny-1] << endl;
      cout << "Routin_Data for Njet = " << Njets[biny-1] << " is " << Routin_Data[biny-1] << endl;
      
      double error = pow( pow((1/Routin_MC[biny-1])*RoutinErr_Data[biny-1],2) +  pow((Routin_Data[biny-1]/(Routin_MC[biny-1]*Routin_MC[biny-1]))*RoutinErr_MC[biny-1],2), 0.5);
      cout << "Routin_Data/Routin_MC for jet " << Njets[biny-1] << " = " << Routin_Data[biny-1]/Routin_MC[biny-1] << " +/-" << error<< endl;
      Routin_DataOverMC[biny-1] = Routin_Data[biny-1]/Routin_MC[biny-1];
      RoutinErr_DataOverMC[biny-1] = error;
      
    }
    
  }
  
   
  TGraphErrors * theGraphMC    = new TGraphErrors(npoint, Njets, Routin_MC, NjetsErr,RoutinErr_MC );
  TGraphErrors * theGraphMC_SN = new TGraphErrors(npoint, Njets, ratioN_Others, NjetsErr, ratioNErr_Others);
  TGraphErrors * theGraphRoutinDataOverMC = new TGraphErrors(npoint, Njets,Routin_DataOverMC , NjetsErr, RoutinErr_DataOverMC );
  
  
  
  
  
  
  
  TH1F * histo = new TH1F ("histo", "histo",  zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax());
  histo->SetMaximum(1.1);
  
  theGraphData->SetMarkerStyle(20);
  theGraphMC->SetMarkerStyle(20);
  theGraphMC->SetMarkerColor(2);
  theGraphMC_SN->SetMarkerStyle(22);
  theGraphMC_SN->SetMarkerColor(4);
   
  histo->Draw();
  if(plot == "NJet") histo->GetXaxis()->SetTitle("Njet");
  if(plot == "MET")  histo->GetXaxis()->SetTitle("MET");
  if(plot == "NPU")  histo->GetXaxis()->SetTitle("nbr. of vertices");
   histo->GetYaxis()->SetTitle("R_{out/in}");
  theGraphData->Draw("ep");
  theGraphMC->Draw("ep"); 
  theGraphMC_SN->Draw("ep"); 
  
  
  TLegend* qw = new TLegend(0.75,0.70,0.98,0.98);
  qw->AddEntry(theGraphData,  "R_{out/in} Data" ,                "p");
  qw->AddEntry(theGraphMC,    "R_{out/in} all MC" ,    "p");
  qw->AddEntry(theGraphMC_SN,    "N_{Z}/N_{tot}  MC" ,    "p");
  
  
  qw->Draw();
  
  TH1F * histo1 = new TH1F ("histo1", "histo1",  zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax());
  histo1->SetMaximum(5.);
  
  theGraphRoutinDataOverMC->SetMarkerStyle(20);
  theGraphRoutinDataOverMC->SetMarkerColor(4);
   
  histo1->Draw();
  if(plot == "NJet") histo1->GetXaxis()->SetTitle("Njet");
  if(plot == "MET")  histo1->GetXaxis()->SetTitle("MET");
  if(plot == "NPU")  histo1->GetXaxis()->SetTitle("nbr. of vertices");
   histo1->GetYaxis()->SetTitle("R_{out/in}Data / R_{out/in}MC");
  theGraphRoutinDataOverMC->Draw("ep");
  
  
  TLegend* qw1 = new TLegend(0.75,0.70,0.98,0.98);
  qw1->AddEntry(theGraphRoutinDataOverMC,  "R_{out/in} Data / R_{out/in} MC" ,                "p");
  
  if(plot == "NJet") { 
    theGraphRoutinDataOverMC->Fit("pol3"); 
      double errorNjetSup2 = 0.;
      double meanNjetSup2 = 0;
      for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
        if ( Njets[biny-1] >=2 ) {  // Njets>=2
	   if ( Routin_DataOverMC[biny-1]>0  && RoutinErr_DataOverMC[biny-1]>0 ) {
	     errorNjetSup2 += 1./(RoutinErr_DataOverMC[biny-1]*RoutinErr_DataOverMC[biny-1]);
	   }
	}  
      }
      for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
        if ( Njets[biny-1] >=2 ) {  // Njets>=2
	   if ( Routin_DataOverMC[biny-1]>0 && RoutinErr_DataOverMC[biny-1]>0 ) {
	     meanNjetSup2 += Routin_DataOverMC[biny-1]/(RoutinErr_DataOverMC[biny-1]*RoutinErr_DataOverMC[biny-1]);
	   }
	}  
      }
      meanNjetSup2 /= errorNjetSup2;
      std::cout<<"Mean for Njets>=2 "<<meanNjetSup2<<std::endl;
    
  }
  if(plot == "MET") { 
      TF1 *f1 = new TF1("f1","pol0",0,30);
      theGraphRoutinDataOverMC->Fit("f1","R"); 
//       TF1 *f2 = new TF1("f2","pol1",30,120);
//       theGraphRoutinDataOverMC->Fit("f2","R"); 
      double errorMET30 = 0.;
      double meanMET30 = 0;
      for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
        if ( Njets[biny-1] > 30 ) {  // MET>30
	   if ( Routin_DataOverMC[biny-1]>0  && RoutinErr_DataOverMC[biny-1]>0 ) {
	     errorMET30 += 1./(RoutinErr_DataOverMC[biny-1]*RoutinErr_DataOverMC[biny-1]);
	   }
	}  
      }
      for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
        if ( Njets[biny-1] > 30 ) {  // MET>30
	   if ( Routin_DataOverMC[biny-1]>0 && RoutinErr_DataOverMC[biny-1]>0 ) {
	     meanMET30 += Routin_DataOverMC[biny-1]/(RoutinErr_DataOverMC[biny-1]*RoutinErr_DataOverMC[biny-1]);
	   }
	}  
      }
      meanMET30 /= errorMET30;
      std::cout<<"Mean for MET>30 "<<meanMET30<<std::endl;
      
  }
   if(plot == "PU") { 
//      theGraphRoutinDataOverMC->Fit("pol0"); 
      double errorPU = 0.;
      double meanPU = 0;
      for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
	   if ( Routin_DataOverMC[biny-1]>0  && RoutinErr_DataOverMC[biny-1]>0 ) {
	     errorPU += 1./(RoutinErr_DataOverMC[biny-1]*RoutinErr_DataOverMC[biny-1]);
	   }
      }
      for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
	   if ( Routin_DataOverMC[biny-1]>0 && RoutinErr_DataOverMC[biny-1]>0 ) {
	     meanPU += Routin_DataOverMC[biny-1]/(RoutinErr_DataOverMC[biny-1]*RoutinErr_DataOverMC[biny-1]);
	   }
      }
      meanPU /= errorPU;
      std::cout<<"Mean for PU "<<meanPU<<std::endl;
   } 
  
  qw1->Draw();

}




//****************************
//****************************
//same but summing up bins 
//****************************
//****************************



void Rout_in_supTo(TString channel, TString plot){








  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  
  TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2); 
  c1->SetLogy(0);
 
  
  
  
  
  
  TFile *f_data  = new TFile("../CrossSection_noDYScale.root");
  f_data->cd();

  
  std::vector<TH2F*> histo_List;
  std::vector<TString> histoName_List;
  histoName_List.push_back("TTbarSig");
  histoName_List.push_back("TTbarBkg");
  histoName_List.push_back("Zjets");
  //histoName_List.push_back("DYToMuMu_M-20");
  histoName_List.push_back("DYToEE_M-20");
  histoName_List.push_back("DYToTauTau_M-20");
  histoName_List.push_back("Wjets");
  histoName_List.push_back("TtW");
  histoName_List.push_back("TbartW");
  histoName_List.push_back("WW");
  histoName_List.push_back("ZZ");
  histoName_List.push_back("WZ");
  
  TString theplot;
  if(plot == "NJet")    theplot= "_pair_vs_NJet_METinf10_";
  if(plot == "MET")     theplot= "_pair_vs_MET_NJet0_";
  if(plot == "METSel")  theplot= "_pair_vs_MET_sup2Jet_";
  if(plot == "NPU")     theplot= "_pair_vs_NPU_";
  
  TH2F *zMass_Data_vs_NJets;
  if( channel == "ee")   zMass_Data_vs_NJets = (TH2F*)gROOT->FindObject(("HInvM_"+channel+theplot+"DataEG").Data());
  if( channel == "mumu") zMass_Data_vs_NJets = (TH2F*)gROOT->FindObject(("HInvM_"+channel+theplot+"DataMu").Data());
  
  cout << ("HInvM_"+channel+theplot+"DataEG").Data() << endl;
  
  for(unsigned int i=0; i<histoName_List.size(); i++){
    histo_List.push_back( (TH2F*)gROOT->FindObject(  ("HInvM_"+channel+theplot+histoName_List[i]).Data()  ));
  }
 
 
   
  //*********************
  // summ up MC templates
  //*********************
  TH2F * totMC = new TH2F("totMC", "totMC", zMass_Data_vs_NJets->GetNbinsX() , zMass_Data_vs_NJets->GetXaxis()->GetXmin() , zMass_Data_vs_NJets->GetXaxis()->GetXmax(), zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax() );
  
   
  TH2F * totMC_Z = new TH2F("totMC_Z", "totMC_Z", zMass_Data_vs_NJets->GetNbinsX() , zMass_Data_vs_NJets->GetXaxis()->GetXmin() , zMass_Data_vs_NJets->GetXaxis()->GetXmax(), zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax() );
  
  //*********************
  for(unsigned int i=0; i<histoName_List.size(); i++){
   histo_List[i]->Sumw2();
   if( histoName_List[i] == "Zjets" ||histoName_List[i] == "DYToMuMu_M-20" 
   ||histoName_List[i] == "DYToEE_M-20"  ||histoName_List[i] == "DYToTauTau_M-20") totMC_Z->Add( totMC_Z, histo_List[i], 1, 1);
   totMC->Add( totMC, histo_List[i], 1, 1);
  }

  


 
  int npoint = zMass_Data_vs_NJets->GetNbinsY();
  
  
  
   
  double Routin_Data[npoint];
  double Routin_MC[npoint];
  double Njets[npoint];
  

  double RoutinErr_Data[npoint];
  double RoutinErr_MC[npoint];
  double NjetsErr[npoint];
  
  
  double ratioN_Others[npoint];
  double ratioNErr_Others[npoint];
  
  double norm =1;
  if(plot == "METSel" || plot == "MET") norm = 150./30.;//
  
  for(unsigned int biny=1; biny<zMass_Data_vs_NJets->GetNbinsY()+1; biny++){
  
    double num   = 0;
    double denom = 0;
    for(unsigned int binyscan=1; binyscan<zMass_Data_vs_NJets->GetNbinsY()+1; binyscan++){
      
      if(binyscan < biny) continue;
      for(unsigned int binx=1; binx < (zMass_Data_vs_NJets->GetNbinsX()+1); binx++){
         //cout << "binx " << binx << endl;
         if( binx < (76+1) || binx > (106+1) )  num+= zMass_Data_vs_NJets->GetBinContent(binx, binyscan);
         else  denom+= zMass_Data_vs_NJets->GetBinContent(binx, binyscan);
        
      }
    } float(zMass_Data_vs_NJets->GetNbinsY())/float(zMass_Data_vs_NJets->GetYaxis()->GetXmax());
    Routin_Data[biny-1] = num/denom;
    RoutinErr_Data[biny-1] = pow( pow((1/denom)*pow(num,0.5),2) +  pow((num/(denom*denom))*pow(denom, 0.5),2), 0.5);
    
    
    Njets[biny-1] = (biny-1)*norm;
    if(num   == 0) Routin_MC[biny-1] = -1;
    if(denom == 0) Routin_MC[biny-1] = -1;
    if(denom == 0 || num   == 0) Njets[biny-1] = -10;
    NjetsErr[biny-1] = 0;
   
  }
  
   
  TGraphErrors * theGraphData = new TGraphErrors(npoint, Njets, Routin_Data, NjetsErr,RoutinErr_Data );
  
  
  
  
  
  
  
  
  
  for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
  
    double num   = 0;
    double denom = 0;
    
    double ntot=0;
    double nZ = 0;
   
    for(unsigned int binyscan=1; binyscan<totMC->GetNbinsY()+1; binyscan++){
    
      if(binyscan < biny) continue;
      
      for(unsigned int binx=1; binx < (totMC->GetNbinsX()+1); binx++){
         if( binx < (76+1) || binx > (106+1) )  num+= totMC->GetBinContent(binx, binyscan);
         else  denom+= totMC->GetBinContent(binx, binyscan);
         ntot += totMC->GetBinContent(binx, binyscan);
         nZ   += totMC_Z->GetBinContent(binx, binyscan);
      }
    }
    Routin_MC[biny-1] = num/denom;
    RoutinErr_MC[biny-1] = pow( pow((1/denom)*pow(num,0.5),2) +  pow((num/(denom*denom))*pow(denom, 0.5),2), 0.5);
    
    if( channel == "mumu") Routin_MC[biny-1] *= 1.26;
    if( channel == "ee"  ) Routin_MC[biny-1] *= 1.09;
    if( channel == "mumu" && plot == "METSel")  Routin_MC[biny-1] *= 1.388;
    if( channel == "ee"   && plot == "METSel")  Routin_MC[biny-1] *= 1.290;
    
    Njets[biny-1] = (biny-1)*norm;
   
    
    ratioN_Others[biny-1]    = nZ/ntot;
    ratioNErr_Others[biny-1] =  pow(ratioN_Others[biny-1]*(1-ratioN_Others[biny-1])/ntot,0.5);
    
    
    if(num   == 0) Routin_MC[biny-1] = -1;
    if(denom == 0) Routin_MC[biny-1] = -1;
    if(denom == 0 || num   == 0) Njets[biny-1] = -10;
    NjetsErr[biny-1] = 0;
    
    if(plot == "NJet"){
      cout << "Routin_MC   for Njet = " << Njets[biny-1] << " is " << Routin_MC[biny-1] << endl;
      cout << "Routin_Data for Njet = " << Njets[biny-1] << " is " << Routin_Data[biny-1] << endl;
      
      double error = pow( pow((1/Routin_MC[biny-1])*RoutinErr_Data[biny-1],2) +  pow((Routin_Data[biny-1]/(Routin_MC[biny-1]*Routin_MC[biny-1]))*RoutinErr_MC[biny-1],2), 0.5);
      cout << "Routin_Data/Routin_MC  = " << Routin_Data[biny-1]/Routin_MC[biny-1] << " +/-" << error<< endl;
    }
    
  }
  
   
  TGraphErrors * theGraphMC    = new TGraphErrors(npoint, Njets, Routin_MC, NjetsErr,RoutinErr_MC );
  TGraphErrors * theGraphMC_SN = new TGraphErrors(npoint, Njets, ratioN_Others, NjetsErr, ratioNErr_Others);
  
  
  
  
  
  
  
 
  TH1F * histo = new TH1F ("histo", "histo",  zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax());
  histo->SetMaximum(3.1);
  
  histo->Draw();
  if(plot == "NJet") histo->GetXaxis()->SetTitle("Njet");
  if(plot == "MET")  histo->GetXaxis()->SetTitle("MET");
  if(plot == "NPU")  histo->GetXaxis()->SetTitle("nbr. of vertices");
   histo->GetYaxis()->SetTitle("R_{out/in}");
  theGraphData->SetMarkerStyle(20);
  theGraphMC->SetMarkerStyle(20);
  theGraphMC->SetMarkerColor(2);
  theGraphMC_SN->SetMarkerStyle(22);
  theGraphMC_SN->SetMarkerColor(4);
  
  theGraphData->Draw("ep");
  theGraphMC->Draw("ep"); 
  theGraphMC_SN->Draw("ep"); 
  
  
  TLegend* qw = new TLegend(0.75,0.70,0.98,0.98);
  qw->AddEntry(theGraphData,  "R_{out/in} Data" ,                "p");
  qw->AddEntry(theGraphMC,    "R_{out/in} all MC" ,    "p");
  qw->AddEntry(theGraphMC_SN,    "N_{Z}/N_{tot}  MC" ,    "p");
  
  
  qw->Draw();
  


}

















//**************************************************************************
//**************************************************************************
//compur R_outin in constrol region enriched in Z, substrasting other MC
//**************************************************************************
//**************************************************************************

void Rout_in_METsub(TString channel, TString plot){
  
  
  
  gStyle->SetPadRightMargin(0.13);
  gStyle->SetPadLeftMargin(0.13);
  
  TCanvas *c1 = new TCanvas("c1", "plots",400,400,800,600);
  c1->SetFillColor(10);
  c1->SetFillStyle(4000);
  c1->SetBorderSize(2); 
  c1->SetLogy(0);
 
  
  
  
  
//  TFile *f_data  = new TFile("CrossSection_new.root");
  TFile *f_data  = new TFile("proof_marcosample_all.root");
  f_data->cd();

  
  std::vector<TH2F*> histo_List;
  std::vector<TString> histoName_List;
  histoName_List.push_back("TTbarSig");
  histoName_List.push_back("TTbarBkg");
  histoName_List.push_back("Zjets");
  //histoName_List.push_back("DYToMuMu_M-20");
  histoName_List.push_back("DYToEE_M-20");
  histoName_List.push_back("DYToTauTau_M-20");
  histoName_List.push_back("Wjets");
  histoName_List.push_back("TtW");
  histoName_List.push_back("TbartW");
  histoName_List.push_back("WW");
  histoName_List.push_back("ZZ");
  histoName_List.push_back("WZ");
  
  TString theplot;
  if(plot == "NJet")    theplot= "_pair_vs_NJet_METinf10_";
  if(plot == "MET")     theplot= "_pair_vs_MET_NJet0_";
  if(plot == "METSel")  theplot= "_pair_vs_MET_sup2Jet_";
  if(plot == "NPU")     theplot= "_pair_vs_NPU_";
  
  TH2F *zMass_Data_vs_NJets;
  if( channel == "ee")   zMass_Data_vs_NJets = (TH2F*)gROOT->FindObject(("HInvM_"+channel+theplot+"DataEG").Data());
  if( channel == "mumu") zMass_Data_vs_NJets = (TH2F*)gROOT->FindObject(("HInvM_"+channel+theplot+"DataMu").Data());
  
  cout << ("HInvM_"+channel+theplot+"DataEG").Data() << endl;
  
  for(unsigned int i=0; i<histoName_List.size(); i++){
    histo_List.push_back( (TH2F*)gROOT->FindObject(  ("HInvM_"+channel+theplot+histoName_List[i]).Data()  ));
  }
 
 
   
  //*********************
  // summ up MC templates
  //*********************
  TH2F * totMC = new TH2F("totMC", "totMC", zMass_Data_vs_NJets->GetNbinsX() , zMass_Data_vs_NJets->GetXaxis()->GetXmin() , zMass_Data_vs_NJets->GetXaxis()->GetXmax(), zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax() );
  
   
  TH2F * totMC_Z = new TH2F("totMC_Z", "totMC_Z", zMass_Data_vs_NJets->GetNbinsX() , zMass_Data_vs_NJets->GetXaxis()->GetXmin() , zMass_Data_vs_NJets->GetXaxis()->GetXmax(), zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , zMass_Data_vs_NJets->GetYaxis()->GetXmax() );
  
  //*********************
  for(unsigned int i=0; i<histoName_List.size(); i++){
   histo_List[i]->Sumw2();
   if( histoName_List[i] == "Zjets" ||histoName_List[i] == "DYToMuMu_M-20" 
   ||histoName_List[i] == "DYToEE_M-20"  ||histoName_List[i] == "DYToTauTau_M-20") totMC_Z->Add( totMC_Z, histo_List[i], 1, 1);
   totMC->Add( totMC, histo_List[i], 1, 1);
  }

  
  
  int npoint = zMass_Data_vs_NJets->GetNbinsY();
  
  
  
  
  
   
  double Routin_Data[npoint];
  double Routin_MC[npoint];
  double Njets[npoint];
  

  double RoutinErr_Data[npoint];
  double RoutinErr_MC[npoint];
  double NjetsErr[npoint];
  
  
  double ratioN_Others[npoint];
  double ratioNErr_Others[npoint];
  double norm = 1.;
  if(plot == "METSel" || plot == "MET") norm = 150./30.;//
 
  
   //*********** for MC
   
  for(unsigned int biny=1; biny<totMC->GetNbinsY()+1; biny++){
    
    double num   = 0;
    double denom = 0;
    
    double ntot=0;
    double nZ = 0;
    
    for(unsigned int binx=1; binx < (totMC->GetNbinsX()+1); binx++){
       if( binx < (76+1) || binx > (106+1) )  num+= totMC_Z->GetBinContent(binx, biny);
       else  denom+= totMC_Z->GetBinContent(binx, biny);
       ntot += totMC->GetBinContent(binx, biny);
       nZ   += totMC_Z->GetBinContent(binx, biny);
    }
    
    Routin_MC[biny-1] = num/denom;
    RoutinErr_MC[biny-1] = pow( pow((1/denom)*pow(num,0.5),2) +  pow((num/(denom*denom))*pow(denom, 0.5),2), 0.5);
    
    if( channel == "mumu") Routin_MC[biny-1] *= 1.26;
    if( channel == "ee"  ) Routin_MC[biny-1] *= 1.09;
    if( channel == "mumu" && plot == "METSel")  Routin_MC[biny-1] *= 1.388;
    if( channel == "ee"   && plot == "METSel")  Routin_MC[biny-1] *= 1.290;
    
    NjetsErr[biny-1] = 0;
    Njets[biny-1] = (biny-1)*norm;
   
    
    ratioN_Others[biny-1]    = nZ/ntot;
    ratioNErr_Others[biny-1] =  pow(ratioN_Others[biny-1]*(1-ratioN_Others[biny-1])/ntot,0.5);
    
    
    if(num   == 0) Routin_MC[biny-1] = -1;
    if(denom == 0) Routin_MC[biny-1] = -1;
    if(denom == 0 || num   == 0) Njets[biny-1] = -10;
    
    if(plot == "NJet"){
      cout << "Routin_MC   for Njet = " << Njets[biny-1] << " is " << Routin_MC[biny-1] << endl;
      cout << "Routin_Data for Njet = " << Njets[biny-1] << " is " << Routin_Data[biny-1] << endl;
      
      double error = pow( pow((1/Routin_MC[biny-1])*RoutinErr_Data[biny-1],2) +  pow((Routin_Data[biny-1]/(Routin_MC[biny-1]*Routin_MC[biny-1]))*RoutinErr_MC[biny-1],2), 0.5);
      cout << "Routin_Data/Routin_MC  = " << Routin_Data[biny-1]/Routin_MC[biny-1] << " +/-" << error<< endl;
    }
    
  }
  
   
  TGraphErrors * theGraphMC    = new TGraphErrors(npoint, Njets, Routin_MC, NjetsErr,RoutinErr_MC );
  TGraphErrors * theGraphMC_SN = new TGraphErrors(npoint, Njets, ratioN_Others, NjetsErr, ratioNErr_Others);
  
  
   
   
   //*********** for data
  
  for(unsigned int biny=1; biny<zMass_Data_vs_NJets->GetNbinsY()+1; biny++){
    
    double num   = 0;
    double denom = 0;
    
    for(unsigned int binx=1; binx < (zMass_Data_vs_NJets->GetNbinsX()+1); binx++){
       if( binx < (76+1) || binx > (106+1) ){
         num+= zMass_Data_vs_NJets->GetBinContent(binx, biny) - totMC->GetBinContent(binx, biny)+ totMC_Z->GetBinContent(binx, biny) ;
	 }
       else{
         denom+= zMass_Data_vs_NJets->GetBinContent(binx, biny)- totMC->GetBinContent(binx, biny)+ totMC_Z->GetBinContent(binx, biny) ;
	 }
      
    }
   
    Routin_Data[biny-1] = num/denom;
    RoutinErr_Data[biny-1] = pow( pow((1/denom)*pow(num,0.5),2) +  pow((num/(denom*denom))*pow(denom, 0.5),2), 0.5);
    
    Njets[biny-1] = (biny-1)*norm;
    if(num   == 0) Routin_MC[biny-1] = -1;
    if(denom == 0) Routin_MC[biny-1] = -1;
    if(denom == 0 || num   == 0) Njets[biny-1] = -10;
    NjetsErr[biny-1] = 0;
    
  }
  
   
  TGraphErrors * theGraphData = new TGraphErrors(npoint, Njets, Routin_Data, NjetsErr,RoutinErr_Data );
  
  
  
  
  
  TH1F * histo = new TH1F ("histo", "histo",  zMass_Data_vs_NJets->GetNbinsY() , zMass_Data_vs_NJets->GetYaxis()->GetXmin() , 60);
  histo->SetMaximum(1.1);
  
  theGraphData->SetMarkerStyle(20);
  theGraphMC->SetMarkerStyle(20);
  theGraphMC->SetMarkerColor(2);
  theGraphMC_SN->SetMarkerStyle(22);
  theGraphMC_SN->SetMarkerColor(4);
   
  histo->Draw();
  if(plot == "NJet") histo->GetXaxis()->SetTitle("Njet");
  if(plot == "MET")  histo->GetXaxis()->SetTitle("MET");
  if(plot == "NPU")  histo->GetXaxis()->SetTitle("nbr. of vertices");
   histo->GetYaxis()->SetTitle("R_{out/in}");
  theGraphData->Draw("ep");
  theGraphMC->Draw("ep"); 
  theGraphMC_SN->Draw("ep"); 
   
  
  TLegend* qw = new TLegend(0.75,0.70,0.98,0.98);
  qw->AddEntry(theGraphData,  "R_{out/in} Data" ,                "p");
  qw->AddEntry(theGraphMC,    "R_{out/in} all MC" ,    "p");
  qw->AddEntry(theGraphMC_SN,    "N_{Z}/N_{tot}  MC" ,    "p");
  
  
  qw->Draw();
  

}









