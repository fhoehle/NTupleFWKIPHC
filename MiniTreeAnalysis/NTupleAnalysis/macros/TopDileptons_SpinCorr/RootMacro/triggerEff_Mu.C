triggerEff_Mu(string q){


  Int_t eventType = 1; // 0 MC, 1 Data
  
  bool doSFsel = 0;
  
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
  
  
  TFile *f1 = new TFile("RootFileTagAndProbeta15_rebin/TriggerPlots.root"); 
  f1->cd();
  
  TH1F* selElectron_pt_data            = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_pT_DataEl");
  TH1F* selElectron_ElTrigger_pt_data  = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_selTrig_pT_DataEl");
  								    
  TH1F* selElectron_eta_data           = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_eta_DataEl");
  TH1F* selElectron_ElTrigger_eta_data = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_selTrig_eta_DataEl");
  
  TH1F* selElectron_phi_data           = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_phi_DataEl");
  TH1F* selElectron_ElTrigger_phi_data = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_selTrig_phi_DataEl");
  
  
  TH1F* selElectron_pt_Zjets            = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_pT_ZMadGraphZ2");
  TH1F* selElectron_ElTrigger_pt_Zjets  = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_selTrig_pT_ZMadGraphZ2");
  
  TH1F* selElectron_eta_Zjets           = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_eta_ZMadGraphZ2");
  TH1F* selElectron_ElTrigger_eta_Zjets = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_selTrig_eta_ZMadGraphZ2");
  
  TH1F* selElectron_phi_Zjets           = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_phi_ZMadGraphZ2");
  TH1F* selElectron_ElTrigger_phi_Zjets = (TH1F*)gROOT->FindObject("HElTriggerEff_MuonSel_selTrig_phi_ZMadGraphZ2");
  
  /*
  selElectron_ElTrigger_pt_data->Rebin(2);
  selElectron_pt_data->Rebin(2);
  selElectron_ElTrigger_eta_data->Rebin(2);
  selElectron_eta_data->Rebin(2);
  selElectron_ElTrigger_phi_data->Rebin(2);
  selElectron_phi_data->Rebin(2);
  
  selElectron_ElTrigger_pt_Zjets->Rebin(2);
  selElectron_pt_Zjets->Rebin(2);
  selElectron_ElTrigger_eta_Zjets->Rebin(2);
  selElectron_eta_Zjets->Rebin(2);
  selElectron_ElTrigger_phi_Zjets->Rebin(2);
  selElectron_phi_Zjets->Rebin(2);*/
  
  
  selElectron_pt_data->Sumw2();
  selElectron_ElTrigger_pt_data->Sumw2();
  selElectron_pt_data->Divide(selElectron_ElTrigger_pt_data, selElectron_pt_data, 1, 1, "b");
  
  
  selElectron_eta_data->Sumw2();
  selElectron_ElTrigger_eta_data->Sumw2();
  selElectron_eta_data->Divide(selElectron_ElTrigger_eta_data, selElectron_eta_data, 1, 1, "b");
  
  
  selElectron_phi_data->Sumw2();
  selElectron_ElTrigger_phi_data->Sumw2();
  selElectron_phi_data->Divide(selElectron_ElTrigger_phi_data, selElectron_phi_data, 1, 1, "b");
  
  
  
  selElectron_pt_Zjets->Sumw2();
  selElectron_ElTrigger_pt_Zjets->Sumw2();
  selElectron_pt_Zjets->Divide(selElectron_ElTrigger_pt_Zjets, selElectron_pt_Zjets, 1, 1, "b");
  
  
  selElectron_eta_Zjets->Sumw2();
  selElectron_ElTrigger_eta_Zjets->Sumw2();
  selElectron_eta_Zjets->Divide(selElectron_ElTrigger_eta_Zjets, selElectron_eta_Zjets, 1, 1, "b");
  
  
  selElectron_phi_Zjets->Sumw2();
  selElectron_ElTrigger_phi_Zjets->Sumw2();
  selElectron_phi_Zjets->Divide(selElectron_ElTrigger_phi_Zjets, selElectron_phi_Zjets, 1, 1, "b");

  
  if(q == "pt"){
    selElectron_pt_data->GetXaxis()->SetTitle("Leading Muon  p_{T} [GeV/c]");
    selElectron_pt_data->GetYaxis()->SetTitle("Muon trigger efficiency");
    selElectron_pt_data->GetYaxis()->SetTitleOffset(1.3);
     
    selElectron_pt_data->SetMinimum(0.4);
    selElectron_pt_data->SetMaximum(1.005);
    
    
    selElectron_pt_data->SetMarkerStyle(22);
    selElectron_pt_Zjets->SetMarkerStyle(22);
    
    selElectron_pt_data->SetMarkerColor(2);
    selElectron_pt_Zjets->SetMarkerColor(4);
    
    selElectron_pt_data->SetMarkerSize(2);
    selElectron_pt_Zjets->SetMarkerSize(2);
    
    selElectron_pt_data->Draw("ep");
    selElectron_pt_Zjets->Draw("epsame");
    
     TLegend* qw = 0;
     qw = new TLegend(0.15,0.20,0.40, 0.45);
     //qw->SetHeader("CMS Preliminary");
     qw->AddEntry(selElectron_pt_data,       "Data ","P");
     qw->AddEntry(selElectron_pt_Zjets,      "Zjets MC ","P");
 
     qw->Draw();
     c1->SaveAs("mu_Trigger_Eff_vs_pt.gif");
     
     //gr  = new TGraphAsymmErrors();
     //gr->BayesDivide(selElectron_pt_data,selElectron_pt_Zjets);
     if(doSFsel){
     Electron_pt_data->Divide(selElectron_pt_data, selElectron_pt_Zjets, 1, 1, "b");
     selElectron_pt_data->GetYaxis()->SetTitle("SF");
     selElectron_pt_data->Draw();
     }
     //gr->Draw();
     c1->SaveAs("mu_Trigger_SF_vs_pt.gif");
  }
  
  
  
  if(q == "phi"){
    selElectron_phi_data->GetXaxis()->SetTitle("Leading muon  #phi");
    selElectron_phi_data->GetYaxis()->SetTitle("Muon trigger efficiency");
    selElectron_phi_data->GetYaxis()->SetTitleOffset(1.3);
     
    selElectron_phi_data->SetMinimum(0.4);
    selElectron_phi_data->SetMaximum(1.005);
    
    
    selElectron_phi_data->SetMarkerStyle(22);
    selElectron_phi_Zjets->SetMarkerStyle(22);
    
    selElectron_phi_data->SetMarkerColor(2);
    selElectron_phi_Zjets->SetMarkerColor(4);
    
    selElectron_phi_data->SetMarkerSize(2);
    selElectron_phi_Zjets->SetMarkerSize(2);
    
    selElectron_phi_data->Draw("ep");
    selElectron_phi_Zjets->Draw("epsame");
    
     TLegend* qw = 0;
     qw = new TLegend(0.15,0.20,0.40, 0.45);
     //qw->SetHeader("CMS Preliminary");
     qw->AddEntry(selElectron_phi_data,        "Data ","P");
     qw->AddEntry(selElectron_phi_Zjets,       "Z MC ","P");
 
     qw->Draw();
     c1->SaveAs("mu_Trigger_Eff_vs_phi.gif");
     
     if(doSFsel){
     selElectron_phi_data->Divide(selElectron_phi_data, selElectron_phi_Zjets, 1, 1, "b");
     selElectron_phi_data->GetYaxis()->SetTitle("SF");
     selElectron_phi_data->Draw();
     }
     //gr->Draw();
     c1->SaveAs("mu_Trigger_SF_vs_phi.gif");
  }
  

  
  if(q == "eta"){
    selElectron_eta_data->GetXaxis()->SetTitle("Leading Muon  #eta");
    selElectron_eta_data->GetYaxis()->SetTitle("Muon trigger efficiency");
    selElectron_eta_data->GetYaxis()->SetTitleOffset(1.3);
     
    selElectron_eta_data->SetMinimum(0.4);
    selElectron_eta_data->SetMaximum(1.005);
    
    
    selElectron_eta_data->SetMarkerStyle(22);
    selElectron_eta_Zjets->SetMarkerStyle(22);
    
    selElectron_eta_data->SetMarkerColor(2);
    selElectron_eta_Zjets->SetMarkerColor(4);
    
    selElectron_eta_data->SetMarkerSize(2);
    selElectron_eta_Zjets->SetMarkerSize(2);
    
    selElectron_eta_data->Draw("ep");
    selElectron_eta_Zjets->Draw("epsame");
    
     TLegend* qw = 0;
     qw = new TLegend(0.25,0.15,0.50, 0.30);
     //qw->SetHeader("CMS Preliminary");
     qw->AddEntry(selElectron_eta_data,        "Data ","P");
     qw->AddEntry(selElectron_eta_Zjets,       "Z MC ","P");
 
     qw->Draw();
     c1->SaveAs("mu_Trigger_Eff_vs_eta.gif");
     
     if(doSFsel){
     selElectron_eta_data->Divide(selElectron_eta_data, selElectron_eta_Zjets, 1, 1, "b");
     selElectron_eta_data->GetYaxis()->SetTitle("SF");
     selElectron_eta_data->Draw();
     }
     c1->SaveAs("mu_Trigger_SF_vs_eta.gif");
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
}
