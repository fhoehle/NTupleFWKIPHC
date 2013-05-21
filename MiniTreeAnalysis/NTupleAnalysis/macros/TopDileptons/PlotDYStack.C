dum(TString channel, TString selection, bool setlogy, float max){
std::cout<<"dum"<<std::endl;
}
PlotDYStack(TString channel, TString selection, bool setlogy, float max){
  
  
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
  c1->SetLogy(setlogy);
  
 
//  TFile * llchannel   = new TFile("results_v2/CrossSection_DYlowmasspythia.root");
  TFile * llchannel   = new TFile("results_v2/CrossSection_DYlowmasspythia_SANSMETCUTforDY.root");
  
  string app = "";
  if(channel == "ee"){ app = "_ee";}
  if(channel == "mumu"){ app = "_mumu";}
  
  TH1F * histo_Data;
  TH1F * histo_DataEG;
  TH1F * histo_DataMu;
  TH1F * histo_TTbarSig;
  TH1F * histo_TTbarBkg;
  TH1F * histo_Zjets;
  TH1F * histo_DYToMuMu_M_20;
  TH1F * histo_DYToEE_M_20;
  TH1F * histo_DYToTauTau_M_20;
  TH1F * histo_DYToMuMu_M_10To20;
  TH1F * histo_DYToEE_M_10To20;
  TH1F * histo_DYToTauTau_M_10To20;
  TH1F * histo_WjetsMu  ;
  TH1F * histo_WjetsE;
  TH1F * histo_WjetsTau;
  TH1F * histo_SingleToptW;
  TH1F * histo_WW ;
  TH1F * histo_WZ ;
  TH1F * histo_ZZ ;
  
  
  
  //###################################################
  //################# For ee channel ##################
  //###################################################
  
  
  
  if(selection == "InvMassemu"){
   
    
    TH1F * hTTbarSig_emu = (TH1F*)gROOT->FindObject(("TTbarsigemu"+app).c_str());
    hTTbarSig_emu->SetLineColor(1);
    hTTbarSig_emu->SetFillColor(kRed+1);
    TH1F * hTTbarBkg_emu = (TH1F*)gROOT->FindObject(("TTbarbkgemu"+app).c_str());
    hTTbarBkg_emu->SetLineColor(1);
    hTTbarBkg_emu->SetFillColor(kRed-7);
    TH1F * hZjets_emu    = (TH1F*)gROOT->FindObject(("Zjetsemu"+app).c_str());
    hZjets_emu->SetLineColor(1);
    hZjets_emu->SetFillColor(4);
    TH1F * hWjets_emu    = (TH1F*)gROOT->FindObject(("Wjetsemu"+app).c_str());
    hWjets_emu->SetLineColor(1);
    hWjets_emu->SetFillColor(8);

//    TH1F * hVV_emu       = (TH1F*)gROOT->FindObject(("VVemu"+app).c_str());
    TH1F * hVV_emu       = (TH1F*)gROOT->FindObject(("WWemu"+app).c_str());
    hVV_emu->SetLineColor(1);
    hVV_emu->SetFillColor(5);
    TH1F * hWZ_emu       = (TH1F*)gROOT->FindObject(("WZemu"+app).c_str());
    hWZ_emu->SetFillColor(5);
    hVV_emu->Add(hWZ_emu);
    TH1F * hZZ_emu       = (TH1F*)gROOT->FindObject(("ZZemu"+app).c_str());
    hZZ_emu->SetFillColor(5);
    hVV_emu->Add(hZZ_emu);

//    TH1F * hStW_emu      = (TH1F*)gROOT->FindObject(("SingleToptWemu"+app).c_str());
    TH1F * hStW_emu      = (TH1F*)gROOT->FindObject(("TtWemu"+app).c_str());
    hStW_emu->SetLineColor(1);
    hStW_emu->SetFillColor(kMagenta);
    TH1F * hTbartW_emu      = (TH1F*)gROOT->FindObject(("TbartWemu"+app).c_str());
    hTbartW_emu->SetFillColor(kMagenta);
    hStW_emu->Add(hTbartW_emu);
    
    TH1F * hDYEE_emu     = (TH1F*)gROOT->FindObject(("DYToEE_M-20emu"+app).c_str());
    if ( hDYEE_emu ) hDYEE_emu->SetFillColor(4);
    TH1F * hDYMM_emu     = (TH1F*)gROOT->FindObject(("DYToMuMu_M-20emu"+app).c_str());
    if ( hDYMM_emu ) hDYMM_emu->SetFillColor(4);
    TH1F * hDYTT_emu     = (TH1F*)gROOT->FindObject(("DYToTauTau_M-20emu"+app).c_str());
    if ( hDYTT_emu ) hDYTT_emu->SetFillColor(4);
      
    TH1F * hall_emu = (TH1F*)gROOT->FindObject(("halllistemu"+app).c_str());

    if ( hDYEE_emu ) hZjets_emu->Add(hDYEE_emu);
    if ( hDYMM_emu ) hZjets_emu->Add(hDYMM_emu);
    if ( hDYTT_emu ) hZjets_emu->Add(hDYTT_emu);

    THStack* hs_emu= new THStack();
    hs_emu->Add(hTTbarSig_emu);
    hs_emu->Add(hTTbarBkg_emu);
    hs_emu->Add(hZjets_emu);
    hs_emu->Add(hWjets_emu);
    hs_emu->Add(hVV_emu);
    hs_emu->Add(hStW_emu);


    hall_emu->GetXaxis()->SetTitle("Mll(GeV)");
    hall_emu->GetYaxis()->SetTitle("Events");
    if (max>0) hall_emu->SetMaximum(max);
    hall_emu->Draw();

    hs_emu->Draw("same"); 

    hall_emu->Draw("same");
      
    TLegend* qw = new TLegend(0.70,0.75,0.95,0.95);

    qw->AddEntry(hStW_emu,"tW","f");
    qw->AddEntry(hVV_emu,"VV","f");
    qw->AddEntry(hWjets_emu,"Wjets","f");
    qw->AddEntry(hZjets_emu,"Zjets","f");
    qw->AddEntry(hTTbarBkg_emu,"TTbarBkg","f");
    qw->AddEntry(hTTbarSig_emu,"TTbarSig","f");
    qw->Draw();
    
  } //InvMassemu
  
  if(selection == "InvMassContCR"){
   
    
    TH1F * hTTbarSig_ContCR = (TH1F*)gROOT->FindObject(("TTbarsigContCR"+app).c_str());
    hTTbarSig_ContCR->SetLineColor(1);
    hTTbarSig_ContCR->SetFillColor(kRed+1);
    TH1F * hTTbarBkg_ContCR = (TH1F*)gROOT->FindObject(("TTbarbkgContCR"+app).c_str());
    hTTbarBkg_ContCR->SetLineColor(1);
    hTTbarBkg_ContCR->SetFillColor(kRed-7);
    TH1F * hWjets_ContCR    = (TH1F*)gROOT->FindObject(("WjetsContCR"+app).c_str());
    hWjets_ContCR->SetLineColor(1);
    hWjets_ContCR->SetFillColor(8);
    
//    TH1F * hVV_ContCR       = (TH1F*)gROOT->FindObject(("VVContCR"+app).c_str());
    TH1F * hVV_ContCR       = (TH1F*)gROOT->FindObject(("WWContCR"+app).c_str());
    hVV_ContCR->SetLineColor(1);
    hVV_ContCR->SetFillColor(5);
    TH1F * hWZ_ContCR       = (TH1F*)gROOT->FindObject(("WZContCR"+app).c_str());
    hWZ_ContCR->SetFillColor(5);
    hVV_ContCR->Add(hWZ_ContCR);
    TH1F * hZZ_ContCR       = (TH1F*)gROOT->FindObject(("ZZContCR"+app).c_str());
    hZZ_ContCR->SetFillColor(5);
    hVV_ContCR->Add(hZZ_ContCR);
    
//    TH1F * hStW_ContCR      = (TH1F*)gROOT->FindObject(("SingleToptWContCR"+app).c_str());
    TH1F * hStW_ContCR      = (TH1F*)gROOT->FindObject(("TtWContCR"+app).c_str());
    hStW_ContCR->SetLineColor(1);
    hStW_ContCR->SetFillColor(kMagenta);
    TH1F * hTbartW_ContCR      = (TH1F*)gROOT->FindObject(("TbartWContCR"+app).c_str());
    hTbartW_ContCR->SetFillColor(kMagenta);
    hStW_ContCR->Add(hTbartW_ContCR);
      
    TH1F * hall_ContCR = (TH1F*)gROOT->FindObject(("halllistContCR"+app).c_str());


    THStack* hs_ContCR= new THStack();
    hs_ContCR->Add(hTTbarSig_ContCR);
    hs_ContCR->Add(hTTbarBkg_ContCR);
    hs_ContCR->Add(hWjets_ContCR);
    hs_ContCR->Add(hVV_ContCR);
    hs_ContCR->Add(hStW_ContCR);


    hall_ContCR->GetXaxis()->SetTitle("Mll(GeV)");
    hall_ContCR->GetYaxis()->SetTitle("Events");
    if (max>0) hall_ContCR->SetMaximum(max);
    hall_ContCR->Draw();

    hs_ContCR->Draw("same"); 

    hall_ContCR->Draw("same");
      
    TLegend* qw = new TLegend(0.70,0.75,0.95,0.95);

    qw->AddEntry(hStW_ContCR,"tW","f");
    qw->AddEntry(hVV_ContCR,"VV","f");
    qw->AddEntry(hWjets_ContCR,"Wjets","f");
    qw->AddEntry(hTTbarBkg_ContCR,"TTbarBkg","f");
    qw->AddEntry(hTTbarSig_ContCR,"TTbarSig","f");
    qw->Draw();
    
  } //InvMassContCR
  
  if(selection == "InvMassCR"){
   
    gStyle->SetOptFit(200);
    
    
    TH1F * hDataCR_CR = (TH1F*)gROOT->FindObject(("hDataCR"+app).c_str());


    hDataCR_CR->GetXaxis()->SetTitle("Mll(GeV)");
    hDataCR_CR->GetYaxis()->SetTitle("Events");
    hDataCR_CR->SetMarkerStyle(20);
    hDataCR_CR->SetMarkerSize(0.7);
    hDataCR_CR->GetXaxis()->SetRangeUser(20.,200.);
    if (max>0) hDataCR_CR->SetMaximum(max);
    hDataCR_CR->Draw("e");
//    TPaveStats *p1 = (TPaveStats*)hDataCR_CR->GetListOfFunctions()->FindObject("stats");  
//    p1->SetX1NDC(0.65); 
     
    TLegend* qw = new TLegend(0.70,0.75,0.90,0.90);

    qw->AddEntry(hDataCR_CR,"Data in CR","p");
    qw->Draw();
    
  }//InvMassCR
  
  if(selection == "InvMassSR"){
   
//     gStyle->SetOptFit(200);
//     gStyle->SetStatH(0.07);  gStyle->SetStatW(0.07);
        
    
    TH1F * hTTbarSig_SR = (TH1F*)gROOT->FindObject(("TTbarsigSR"+app).c_str());
    hTTbarSig_SR->SetLineColor(1);
    hTTbarSig_SR->SetFillColor(kRed+1);
    TH1F * hTTbarBkg_SR = (TH1F*)gROOT->FindObject(("TTbarbkgSR"+app).c_str());
    hTTbarBkg_SR->SetLineColor(1);
    hTTbarBkg_SR->SetFillColor(kRed-7);
    TH1F * hZjets_SR    = (TH1F*)gROOT->FindObject(("ZjetsSR"+app).c_str());
    hZjets_SR->SetLineColor(1);
    hZjets_SR->SetFillColor(4);
    TH1F * hWjets_SR    = (TH1F*)gROOT->FindObject(("WjetsSR"+app).c_str());
    hWjets_SR->SetLineColor(1);
    hWjets_SR->SetFillColor(8);
    
//    TH1F * hVV_SR       = (TH1F*)gROOT->FindObject(("VVSR"+app).c_str());
    TH1F * hVV_SR       = (TH1F*)gROOT->FindObject(("WWSR"+app).c_str());
    hVV_SR->SetLineColor(1);
    hVV_SR->SetFillColor(5);
    TH1F * hWZ_SR       = (TH1F*)gROOT->FindObject(("WZSR"+app).c_str());
    hWZ_SR->SetFillColor(5);
    hVV_SR->Add(hWZ_SR);
    TH1F * hZZ_SR       = (TH1F*)gROOT->FindObject(("ZZSR"+app).c_str());
    hZZ_SR->SetFillColor(5);
    hVV_SR->Add(hZZ_SR);
    
//    TH1F * hStW_SR      = (TH1F*)gROOT->FindObject(("SingleToptWSR"+app).c_str());
    TH1F * hStW_SR      = (TH1F*)gROOT->FindObject(("TtWSR"+app).c_str());
    hStW_SR->SetLineColor(1);
    hStW_SR->SetFillColor(kMagenta);
    TH1F * hTbartW_SR      = (TH1F*)gROOT->FindObject(("TbartWSR"+app).c_str());
    hTbartW_SR->SetFillColor(kMagenta);
    hStW_SR->Add(hTbartW_SR);
    
    TH1F * hDYEE_SR     = (TH1F*)gROOT->FindObject(("DYToEE_M-20SR"+app).c_str());
    if ( hDYEE_SR ) hDYEE_SR->SetFillColor(4);
    TH1F * hDYMM_SR     = (TH1F*)gROOT->FindObject(("DYToMuMu_M-20SR"+app).c_str());
    if ( hDYMM_SR ) hDYMM_SR->SetFillColor(4);
    TH1F * hDYTT_SR     = (TH1F*)gROOT->FindObject(("DYToTauTau_M-20SR"+app).c_str());
    if ( hDYTT_SR ) hDYTT_SR->SetFillColor(4);
      
    TH1F * hall_SR = (TH1F*)gROOT->FindObject(("halllistSR"+app).c_str());

    if ( hDYEE_SR ) hZjets_SR->Add(hDYEE_SR);
    if ( hDYMM_SR ) hZjets_SR->Add(hDYMM_SR);
    if ( hDYTT_SR ) hZjets_SR->Add(hDYTT_SR);

    THStack* hs_SR= new THStack();
    hs_SR->Add(hTTbarSig_SR);
    hs_SR->Add(hTTbarBkg_SR);
//    hZjets_SR->Scale(24./13.);
//    hZjets_SR->Scale(2.);
    hs_SR->Add(hZjets_SR);
    hs_SR->Add(hWjets_SR);
    hs_SR->Add(hVV_SR);
    hs_SR->Add(hStW_SR);
    
    TH1F * hDataSR_SR = (TH1F*)gROOT->FindObject(("hDataSR"+app).c_str());


    hall_SR->GetXaxis()->SetTitle("Mll(GeV)");
    hall_SR->GetYaxis()->SetTitle("Events");
    hall_SR->GetXaxis()->SetRangeUser(20.,200.);
    float M = hall_SR->GetMaximum();
    hall_SR->SetMaximum(M*1.25);
    if (max>0) hall_SR->SetMaximum(max);
    hall_SR->Draw();
    hs_SR->Draw("same");
    hall_SR->Draw("same");
    TF1 *p1 = (TF1*)hall_SR->GetListOfFunctions()->FindObject("AsymBWplusCF");  
    p1->SetLineColor(4);

    hDataSR_SR->GetXaxis()->SetTitle("Mll(GeV)");
    hDataSR_SR->GetYaxis()->SetTitle("Events");
    hDataSR_SR->SetMarkerStyle(20);
    hDataSR_SR->SetMarkerSize(0.7);
    hDataSR_SR->GetXaxis()->SetRangeUser(20.,200.);
    hDataSR_SR->Draw("samee");
    TF1 *p2 = (TF1*)hDataSR_SR->GetListOfFunctions()->FindObject("BWplusCF");  
//     TPaveStats *p3 = (TPaveStats*)hDataSR_SR->GetListOfFunctions()->FindObject("stats");  
//     p3->SetX1NDC(0.65); 
     
    TLegend* qw = new TLegend(0.70,0.55,1.0,0.8);

    qw->AddEntry(hDataSR_SR,"Data in SR","p");
    qw->AddEntry(p2,"Data fit","l");
    qw->AddEntry(p1,"MC fit","l");
    qw->AddEntry(hStW_SR,"tW","f");
    qw->AddEntry(hVV_SR,"VV","f");
    qw->AddEntry(hWjets_SR,"Wjets","f");
    qw->AddEntry(hZjets_SR,"Zjets","f");
    qw->AddEntry(hTTbarBkg_SR,"TTbarBkg","f");
    qw->AddEntry(hTTbarSig_SR,"TTbarSig","f");
    qw->Draw();
    
  }//InvMassSR
//  }
  
  
  
  
  
  
  
}
