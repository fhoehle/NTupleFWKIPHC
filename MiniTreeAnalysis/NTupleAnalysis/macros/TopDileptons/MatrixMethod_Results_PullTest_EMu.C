MatrixMethod_Results_PullTest_EMu(string selection, string eventType){


  float NjetsMin = -0.5;
  float NjetsMax = 5.5;

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  TFile * file_Expected   = new TFile("MatrixMethod_OutPut_EMuCase_MC_Fast_PullTest.root");
  TFile * file_Estimated  = new TFile("MatrixMethod_OutPut_EMuCase_MC_Fast_PullTest.root");

  file_Expected->cd();
  TH1F* tt_TTDilept  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TTbarSignal");
  TH1F* tl_TTDilept = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TTbarSignal");
  TH1F* lt_TTDilept  = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TTbarSignal");
  TH1F* ll_TTDilept  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TTbarSignal");

  tt_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_TTSemi_TF  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TTbarSemileptonic_TF");
  TH1F* tl_TTSemi_TF = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TTbarSemileptonic_TF");
  TH1F* lt_TTSemi_TF  = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TTbarSemileptonic_TF");
  TH1F* ll_TTSemi_TF  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TTbarSemileptonic_TF");

  tt_TTSemi_TF->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TTSemi_TF->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TTSemi_TF->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TTSemi_TF->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_TTSemi_FT  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TTbarSemileptonic_FT");
  TH1F* tl_TTSemi_FT = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TTbarSemileptonic_FT");
  TH1F* lt_TTSemi_FT  = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TTbarSemileptonic_FT");
  TH1F* ll_TTSemi_FT  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TTbarSemileptonic_FT");

  tt_TTSemi_FT->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TTSemi_FT->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TTSemi_FT->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TTSemi_FT->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_TTBckg  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TTbar");
  TH1F* tl_TTBckg = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TTbar");
  TH1F* lt_TTBckg  = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TTbar");
  TH1F* ll_TTBckg  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TTbar");

  tt_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  tt_TTDilept->SetFillColor(kRed+1);
  tl_TTDilept->SetFillColor(kRed+1);
  lt_TTDilept->SetFillColor(kRed+1);
  ll_TTDilept->SetFillColor(kRed+1);

  tt_TTSemi_TF->SetFillColor(kRed-7);
  tl_TTSemi_TF->SetFillColor(kRed-7);
  lt_TTSemi_TF->SetFillColor(kRed-7);
  ll_TTSemi_TF->SetFillColor(kRed-7);

  tt_TTSemi_FT->SetFillColor(kRed-4);
  tl_TTSemi_FT->SetFillColor(kRed-4);
  lt_TTSemi_FT->SetFillColor(kRed-4);
  ll_TTSemi_FT->SetFillColor(kRed-4);

  tt_TTBckg->SetFillColor(kRed-7);
  tl_TTBckg->SetFillColor(kRed-7);
  lt_TTBckg->SetFillColor(kRed-7);
  ll_TTBckg->SetFillColor(kRed-7);
  
  THStack* ttSignalLike = new THStack("ttSignalLike","tt signal like");
  ttSignalLike->Add(tt_TTDilept);
  THStack* tlSignalLike = new THStack("tlSignalLike","tl signal like");
  tlSignalLike->Add(tl_TTDilept);
  THStack* ltSignalLike = new THStack("ltSignalLike","lt signal like");
  ltSignalLike->Add(lt_TTDilept);
  THStack* llSignalLike = new THStack( "llSignalLike", "ll signal like");
  llSignalLike->Add(ll_TTDilept);

  THStack* ttWLike = new THStack( "ttWLike"," tt W-like" );
  ttWLike->Add(tt_TTSemi_TF);
  ttWLike->Add(tt_TTSemi_FT);
  THStack* tlWLike = new THStack("tlWLike","tl W-like");
  tlWLike->Add(tl_TTSemi_TF);
  tlWLike->Add(lt_TTSemi_FT);
  THStack* ltWLike = new THStack( "ltWLike", "lt W-like" );
  ltWLike->Add(lt_TTSemi_TF);
  ltWLike->Add(tl_TTSemi_FT);
  THStack* llWLike = new THStack( "llWLike", "ll W-like" );
  llWLike->Add(ll_TTSemi_TF);
  llWLike->Add(ll_TTSemi_FT);
  
  THStack* ttQCDLike = new THStack("ttQCDLike",  "tt QCD-like" );
  ttQCDLike->Add(tt_TTBckg);
  THStack* tlQCDLike = new THStack("tlQCDLike","tl QCD-like");
  tlQCDLike->Add(tl_TTBckg);
  THStack* ltQCDLike = new THStack("ltQCDLike","lt QCD-like");
  ltQCDLike->Add(lt_TTBckg);
  THStack* llQCDLike = new THStack("llQCDLike",  "ll QCD-like" );
  llQCDLike->Add(ll_TTBckg);

  
  file_Estimated->cd();
  
  TH1F* estimate_LL_QCD  = (TH1F*)gROOT->FindObject("MMEstimated_LLEMu_FF" );
  TH1F* estimate_TT_QCD  = (TH1F*)gROOT->FindObject("MMEstimated_TTEMu_FF" );
  TH1F* estimate_TL_QCD = (TH1F*)gROOT->FindObject("MMEstimated_TLEMu_FF");
  TH1F* estimate_LT_QCD = (TH1F*)gROOT->FindObject("MMEstimated_LTEMu_FF");

  estimate_LL_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TT_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TL_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_LT_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  // Be careful, here TF and FT estimates are added together to give WJets-like estimate
  TH1F* estimate_LL_TF  = (TH1F*)gROOT->FindObject("MMEstimated_LLEMu_TF" );
  TH1F* estimate_TT_TF  = (TH1F*)gROOT->FindObject("MMEstimated_TTEMu_TF" );
  TH1F* estimate_LT_TF = (TH1F*)gROOT->FindObject("MMEstimated_LTEMu_TF");
  TH1F* estimate_TL_TF = (TH1F*)gROOT->FindObject("MMEstimated_TLEMu_TF");

  TH1F* estimate_LL_WJets  = (TH1F*)gROOT->FindObject("MMEstimated_LLEMu_FT" );
  TH1F* estimate_TT_WJets = (TH1F*)gROOT->FindObject("MMEstimated_TTEMu_FT" );
  TH1F* estimate_LT_WJets = (TH1F*)gROOT->FindObject("MMEstimated_LTEMu_FT");
  TH1F* estimate_TL_WJets = (TH1F*)gROOT->FindObject("MMEstimated_TLEMu_FT");

  estimate_LL_WJets->Add(estimate_LL_TF, estimate_LL_WJets, 1, 1);
  estimate_TT_WJets->Add(estimate_TT_TF, estimate_TT_WJets, 1, 1);
  estimate_LT_WJets->Add(estimate_LT_TF, estimate_LT_WJets, 1, 1);
  estimate_TL_WJets->Add(estimate_TL_TF, estimate_TL_WJets, 1, 1);
  ///////

  estimate_LL_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TT_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_LT_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TL_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* estimate_LL_Signal  = (TH1F*)gROOT->FindObject("MMEstimated_LLEMu_TT" );
  TH1F* estimate_TT_Signal  = (TH1F*)gROOT->FindObject("MMEstimated_TTEMu_TT" );
  TH1F* estimate_TL_Signal = (TH1F*)gROOT->FindObject("MMEstimated_TLEMu_TT");
  TH1F* estimate_LT_Signal = (TH1F*)gROOT->FindObject("MMEstimated_LTEMu_TT");

  estimate_LL_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TT_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TL_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_LT_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  title = new TLatex(10.,10.,"CMS preliminary");
  title->SetNDC();
  title->SetTextAlign(12);
  title->SetX(0.60);
  title->SetY(0.70);
  title->SetTextFont(42);
  title->SetTextSize(0.04);
  title->SetTextSizePixels(24);
    
  TLegend* qw = 0;
  qw = new TLegend(0.70,0.75,0.95,0.95);
  qw->SetFillColor(kWhite);

  if( selection == "TT"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_TT_Signal->SetMarkerStyle(20);
      estimate_TT_Signal->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_TT_Signal->GetYaxis()->SetTitle("Number of events");
      estimate_TT_Signal->GetYaxis()->SetLabelSize(0.025);
      estimate_TT_Signal->SetTitle("Signal-like e#mu (tight-tight)");
      estimate_TT_Signal->Draw();
      ttSignalLike->Draw("hesame");
      estimate_TT_Signal->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tt_TTDilept);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tt_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(estimate_TT_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("TTSignalEMu_PullTest.pdf");
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_TT_WJets->SetMarkerStyle(20);
      estimate_TT_WJets->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_TT_WJets->GetYaxis()->SetTitle("Number of events");
      estimate_TT_WJets->GetYaxis()->SetLabelSize(0.025);
      estimate_TT_WJets->SetTitle("W+jets-like e#mu (tight-tight)");
      estimate_TT_WJets->Draw();
      ttWLike->Draw("hesame");
      estimate_TT_WJets->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tt_TTSemi_TF);
      tmp->Add(tmp, tt_TTSemi_FT);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tt_TTSemi_TF, " t#bar{t} semileptonic TF","f");
      qw->AddEntry(tt_TTSemi_FT, " t#bar{t} semileptonic FT","f");
      qw->AddEntry(estimate_TT_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("TTWlikeEMu_PullTest.pdf");
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_TT_QCD->SetMarkerStyle(20);
      estimate_TT_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_TT_QCD->GetYaxis()->SetTitle("Number of events");
      estimate_TT_QCD->GetYaxis()->SetLabelSize(0.025);
      estimate_TT_QCD->SetTitle("QCD-like e#mu (tight-tight)");
      estimate_TT_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_TT_QCD->Draw();
      ttQCDLike->Draw("hesame");
      estimate_TT_QCD->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tt_TTBckg);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      estimate_TT_QCD->Draw("same");
      thegraph->Draw("e2same");
      qw->AddEntry(tt_TTBckg, " t#bar{t} hadronic ","f");
      qw->AddEntry(estimate_TT_QCD, " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("TTQCDEMu_PullTest.pdf");      
    }
    
    
  }
  
  if( selection == "LT"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_LT_Signal->SetMarkerStyle(20);
      estimate_LT_Signal->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_LT_Signal->GetYaxis()->SetTitle("Number of events");
      estimate_LT_Signal->GetYaxis()->SetLabelSize(0.025);
      estimate_LT_Signal->SetTitle("Signal-like e#mu (loose-tight)");
      estimate_LT_Signal->Draw();
      ltSignalLike->Draw("hesame");
      estimate_LT_Signal->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, lt_TTDilept);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(lt_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(estimate_LT_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("LTSignalEMu_PullTest.pdf");
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_LT_WJets->SetMarkerStyle(20);
      estimate_LT_WJets->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_LT_WJets->GetYaxis()->SetTitle("Number of events");
      estimate_LT_WJets->GetYaxis()->SetLabelSize(0.025);
      estimate_LT_WJets->SetTitle("W+jets-like e#mu (loose-tight)");
      estimate_LT_WJets->Draw();
      ltWLike->Draw("hesame");
      estimate_LT_WJets->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tl_TTSemi_TF);
      tmp->Add(tmp, lt_TTSemi_FT);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tl_TTSemi_TF, " t#bar{t} semileptonic TF","f");
      qw->AddEntry(lt_TTSemi_FT, " t#bar{t} semileptonic FT","f");
      qw->AddEntry(estimate_LT_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("LTWlikeEMu_PullTest.pdf");      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_LT_QCD->SetMarkerStyle(20);
      estimate_LT_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_LT_QCD->GetYaxis()->SetTitle("Number of events");
      estimate_LT_QCD->GetYaxis()->SetLabelSize(0.025);
      estimate_LT_QCD->SetTitle("QCD-like e#mu (loose-tight)");
      estimate_LT_QCD->Draw();
      ltQCDLike->Draw("hesame");
      estimate_LT_QCD->Draw("same"); 
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, lt_TTBckg);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(lt_TTBckg, " t#bar{t} hadronic ","f");
      qw->AddEntry(estimate_LT_QCD, " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("LTQCDEMu_PullTest.pdf");      
    }
    
    
  }






  if( selection == "TL"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_TL_Signal->SetMarkerStyle(20);
      estimate_TL_Signal->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_TL_Signal->GetYaxis()->SetTitle("Number of events");
      estimate_TL_Signal->GetYaxis()->SetLabelSize(0.025);
      estimate_TL_Signal->SetTitle("Signal-like e#mu (tight-loose)");
      estimate_TL_Signal->Draw();
      estimate_TL_Signal->Draw();
      tlSignalLike->Draw("hesame");
      estimate_TL_Signal->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tl_TTDilept);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tl_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(estimate_TL_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("TLSignalEMu_PullTest.pdf");
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_TL_WJets->SetMarkerStyle(20);
      estimate_TL_WJets->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_TL_WJets->GetYaxis()->SetTitle("Number of events");
      estimate_TL_WJets->GetYaxis()->SetLabelSize(0.025);
      estimate_TL_WJets->SetTitle("W+jets-like e#mu (tight-loose)");
      estimate_TL_WJets->Draw();
      tlWLike->Draw("hesame");
      estimate_TL_WJets->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tl_TTSemi_TF);
      tmp->Add(tmp, lt_TTSemi_FT);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tl_TTSemi_TF, " t#bar{t} semileptonic TF","f");
      qw->AddEntry(lt_TTSemi_FT, " t#bar{t} semileptonic FT","f");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("TLWlikeEMu_PullTest.pdf");      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_TL_QCD->SetMarkerStyle(20);
      estimate_TL_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_TL_QCD->GetYaxis()->SetTitle("Number of events");
      estimate_TL_QCD->GetYaxis()->SetLabelSize(0.025);
      estimate_TL_QCD->SetTitle("QCD-like e#mu (tight-loose)");
      estimate_TL_QCD->Draw();
      tlQCDLike->Draw("hesame");
      estimate_TL_QCD->Draw("same"); 
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tl_TTBckg);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tl_TTBckg, " t#bar{t} hadronic ","f");
      qw->AddEntry(estimate_TL_QCD, " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("TLQCDEMu_PullTest.pdf");      
    }
    
    
  }









  
  
  
  
  
  if( selection == "LL"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_LL_Signal->SetMarkerStyle(20);
      estimate_LL_Signal->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_LL_Signal->GetYaxis()->SetTitle("Number of events");
      estimate_LL_Signal->GetYaxis()->SetLabelSize(0.025);
      estimate_LL_Signal->SetTitle("Signal-like e#mu (loose-loose)");
      estimate_LL_Signal->Draw();
      llSignalLike->Draw("hesame");
      estimate_LL_Signal->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, ll_TTDilept);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(ll_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(estimate_LL_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("LLSignalEMu_PullTest.pdf");      
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_LL_WJets->SetMarkerStyle(20);
      estimate_LL_WJets->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_LL_WJets->GetYaxis()->SetTitle("Number of events");
      estimate_LL_WJets->GetYaxis()->SetLabelSize(0.025);
      estimate_LL_WJets->SetTitle("W+jets-like e#mu (loose-loose)");
      estimate_LL_WJets->Draw();
      llWLike->Draw("hesame");
      estimate_LL_WJets->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, ll_TTSemi_TF);
      tmp->Add(tmp, ll_TTSemi_FT);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(ll_TTSemi_TF, " t#bar{t} semileptonic TF","f");
      qw->AddEntry(ll_TTSemi_FT, " t#bar{t} semileptonic FT","f");
      qw->AddEntry(estimate_LL_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("LLWlikeEMu_PullTest.pdf");      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canvPullTest_EMu_"+selection+eventType).c_str(), ("canvPullTest_EMu_"+selection+eventType).c_str(), 600, 800);
      estimate_LL_QCD->SetMarkerStyle(20);
      estimate_LL_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_LL_QCD->GetYaxis()->SetTitle("Number of events");
      estimate_LL_QCD->GetYaxis()->SetLabelSize(0.025);
      estimate_LL_QCD->SetTitle("QCD-like e#mu (loose-loose)");
      estimate_LL_QCD->Draw(); 
      llQCDLike->Draw("hesame");
      estimate_LL_QCD->Draw("same"); 
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, ll_TTBckg);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(ll_TTBckg, " t#bar{t} hadronic ","f");
      qw->AddEntry(estimate_LL_QCD, " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs("LLQCDEMu_PullTest.pdf");      
    }
    
    
  }
   
 
 
}
