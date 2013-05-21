MatrixMethod_Results_EMu(string selection, string eventType, string ntupleType, string fast, string other){

  float NjetsMin = -0.5;
  float NjetsMax = 5.5;

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TFile * file_Expected   = new TFile(("MatrixMethod_OutPut_EMuCase_MC"+other+".root").c_str());
  TFile * file_Estimated  = new TFile(("MatrixMethod_OutPut_EMuCase"+ntupleType+fast+other+".root").c_str());

  file_Expected->cd();
  TH1F* tt_TTDilept  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TTbarSignal");
  TH1F* lt_TTDilept = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TTbarSignal");
  TH1F* tl_TTDilept = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TTbarSignal");
  TH1F* ll_TTDilept  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TTbarSignal");

  tt_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_TTSemi  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TTbarSemileptonic");
  TH1F* lt_TTSemi = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TTbarSemileptonic");
  TH1F* tl_TTSemi = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TTbarSemileptonic");
  TH1F* ll_TTSemi  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TTbarSemileptonic");

  tt_TTSemi->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TTSemi->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TTSemi->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TTSemi->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_TTBckg  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TTbar");
  TH1F* lt_TTBckg = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TTbar");
  TH1F* tl_TTBckg = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TTbar");
  TH1F* ll_TTBckg  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TTbar");

  tt_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_ZJets  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_Zjets");
  TH1F* lt_ZJets = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_Zjets");
  TH1F* tl_ZJets = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_Zjets");
  TH1F* ll_ZJets  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_Zjets");

  tt_ZJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_ZJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_ZJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_ZJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_DYToTauTauHigh  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_DYToTauTau_M-20");
  TH1F* lt_DYToTauTauHigh = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_DYToTauTau_M-20");
  TH1F* tl_DYToTauTauHigh = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_DYToTauTau_M-20");
  TH1F* ll_DYToTauTauHigh  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_DYToTauTau_M-20");

  tt_DYToTauTauHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_DYToTauTauHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_DYToTauTauHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_DYToTauTauHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  
  TH1F* tt_DYToTauTauLow  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_DYToTauTau_M-10To20");
  TH1F* lt_DYToTauTauLow = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_DYToTauTau_M-10To20");
  TH1F* tl_DYToTauTauLow = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_DYToTauTau_M-10To20");
  TH1F* ll_DYToTauTauLow  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_DYToTauTau_M-10To20");

  tt_DYToTauTauLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_DYToTauTauLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_DYToTauTauLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_DYToTauTauLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  
  TH1F* tt_WJets  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_Wjets");
  TH1F* lt_WJets = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_Wjets");
  TH1F* tl_WJets = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_Wjets");
  TH1F* ll_WJets  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_Wjets");

  tt_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_TtW  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TtW");
  TH1F* lt_TtW = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TtW");
  TH1F* tl_TtW = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TtW");
  TH1F* ll_TtW  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TtW");

  tt_TtW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TtW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TtW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TtW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_TbartW  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_TbartW");
  TH1F* lt_TbartW = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_TbartW");
  TH1F* tl_TbartW = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_TbartW");
  TH1F* ll_TbartW  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_TbartW");

  tt_TbartW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_TbartW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_TbartW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_TbartW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_WW  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_WW");
  TH1F* lt_WW = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_WW");
  TH1F* tl_WW = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_WW");
  TH1F* ll_WW  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_WW");

  tt_WW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_WW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_WW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_WW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_WZ  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_WZ");
  TH1F* lt_WZ = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_WZ");
  TH1F* tl_WZ = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_WZ");
  TH1F* ll_WZ  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_WZ");

  tt_WZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_WZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_WZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_WZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tt_ZZ  = (TH1F*)gROOT->FindObject("MMExpected_TTEMu_ZZ");
  TH1F* lt_ZZ = (TH1F*)gROOT->FindObject("MMExpected_LTEMu_ZZ");
  TH1F* tl_ZZ = (TH1F*)gROOT->FindObject("MMExpected_TLEMu_ZZ");
  TH1F* ll_ZZ  = (TH1F*)gROOT->FindObject("MMExpected_LLEMu_ZZ");

  tt_ZZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  lt_ZZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  tl_ZZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  ll_ZZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  tt_TTDilept->SetFillColor(kRed+1);
  lt_TTDilept->SetFillColor(kRed+1);
  tl_TTDilept->SetFillColor(kRed+1);
  ll_TTDilept->SetFillColor(kRed+1);

  tt_TTSemi->SetFillColor(kRed-7);
  lt_TTSemi->SetFillColor(kRed-7);
  tl_TTSemi->SetFillColor(kRed-7);
  ll_TTSemi->SetFillColor(kRed-7);

  tt_TTBckg->SetFillColor(kRed-7);
  lt_TTBckg->SetFillColor(kRed-7);
  tl_TTBckg->SetFillColor(kRed-7);
  ll_TTBckg->SetFillColor(kRed-7);
  
  tt_ZJets->SetFillColor(kAzure-2);
  lt_ZJets->SetFillColor(kAzure-2);
  tl_ZJets->SetFillColor(kAzure-2);
  ll_ZJets->SetFillColor(kAzure-2);

  tt_DYToTauTauHigh->SetFillColor(kAzure-2);
  lt_DYToTauTauHigh->SetFillColor(kAzure-2);
  tl_DYToTauTauHigh->SetFillColor(kAzure-2);
  ll_DYToTauTauHigh->SetFillColor(kAzure-2);

  tt_DYToTauTauLow->SetFillColor(kAzure-2);
  lt_DYToTauTauLow->SetFillColor(kAzure-2);
  tl_DYToTauTauLow->SetFillColor(kAzure-2);
  ll_DYToTauTauLow->SetFillColor(kAzure-2);
  
  tt_WJets->SetFillColor(kGreen-3);
  lt_WJets->SetFillColor(kGreen-3);
  tl_WJets->SetFillColor(kGreen-3);
  ll_WJets->SetFillColor(kGreen-3);

  tt_TtW->SetFillColor(kMagenta-6);
  lt_TtW->SetFillColor(kMagenta-6);
  tl_TtW->SetFillColor(kMagenta-6);
  ll_TtW->SetFillColor(kMagenta-6);

  tt_TbartW->SetFillColor(kMagenta-6);
  lt_TbartW->SetFillColor(kMagenta-6);
  tl_TbartW->SetFillColor(kMagenta-6);
  ll_TbartW->SetFillColor(kMagenta-6);

  tt_WW->SetFillColor(kGray+2);
  lt_WW->SetFillColor(kGray+2);
  tl_WW->SetFillColor(kGray+2);
  ll_WW->SetFillColor(kGray+2);  

  tt_WZ->SetFillColor(kGray+2);
  lt_WZ->SetFillColor(kGray+2);
  tl_WZ->SetFillColor(kGray+2);
  ll_WZ->SetFillColor(kGray+2);  

  tt_ZZ->SetFillColor(kGray+2);
  lt_ZZ->SetFillColor(kGray+2);
  tl_ZZ->SetFillColor(kGray+2);
  ll_ZZ->SetFillColor(kGray+2);
   
  THStack* ttSignalLike = new THStack("ttSignalLike","tt signal like");
  ttSignalLike->Add(tt_TTDilept);
  ttSignalLike->Add(tt_ZJets);
  ttSignalLike->Add(tt_TtW);
  ttSignalLike->Add(tt_TbartW);
  ttSignalLike->Add(tt_DYToTauTauHigh);
  ttSignalLike->Add(tt_DYToTauTauLow);
  ttSignalLike->Add(tt_WW);
  ttSignalLike->Add(tt_WZ);
  ttSignalLike->Add(tt_ZZ);
  THStack* tlSignalLike = new THStack("tlSignalLike","tl signal like");
  tlSignalLike->Add(tl_TTDilept);
  tlSignalLike->Add(tl_ZJets);
  tlSignalLike->Add(tl_TtW);
  tlSignalLike->Add(tl_TbartW);
  tlSignalLike->Add(tl_DYToTauTauHigh);
  tlSignalLike->Add(tl_DYToTauTauLow);
  tlSignalLike->Add(tl_WW);
  tlSignalLike->Add(tl_WZ);
  tlSignalLike->Add(tl_ZZ);
  THStack* ltSignalLike = new THStack("ltSignalLike","lt signal like");
  ltSignalLike->Add(lt_TTDilept);
  ltSignalLike->Add(lt_ZJets);
  ltSignalLike->Add(lt_TtW);
  ltSignalLike->Add(lt_TbartW);
  ltSignalLike->Add(lt_DYToTauTauHigh);
  ltSignalLike->Add(lt_DYToTauTauLow);
  ltSignalLike->Add(lt_WW);
  ltSignalLike->Add(lt_WZ);
  ltSignalLike->Add(lt_ZZ);
  THStack* llSignalLike = new THStack( "llSignalLike", "ll signal like");
  llSignalLike->Add(ll_TTDilept);
  llSignalLike->Add(ll_ZJets);
  llSignalLike->Add(ll_TtW);
  llSignalLike->Add(ll_TbartW);
  llSignalLike->Add(ll_DYToTauTauHigh);
  llSignalLike->Add(ll_DYToTauTauLow);
  llSignalLike->Add(ll_WW);
  llSignalLike->Add(ll_WZ);
  llSignalLike->Add(ll_ZZ);


  THStack* ttWLike = new THStack( "ttWLike"," tt W-like" );
  ttWLike->Add(tt_TTSemi);
  ttWLike->Add(tt_WJets);
  THStack* tlWLike = new THStack("tlWLike","tl W-like");
  tlWLike->Add(tl_TTSemi);
  tlWLike->Add(tl_WJets);
  THStack* ltWLike = new THStack("ltWLike","lt W-like");
  ltWLike->Add(lt_TTSemi);
  ltWLike->Add(lt_WJets);
  THStack* llWLike = new THStack( "llWLike", "ll W-like" );
  llWLike->Add(ll_TTSemi);
  llWLike->Add(ll_WJets);
  
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
  TH1F* estimate_LT_QCD = (TH1F*)gROOT->FindObject("MMEstimated_LTEMu_FF");
  TH1F* estimate_TL_QCD = (TH1F*)gROOT->FindObject("MMEstimated_TLEMu_FF");

  estimate_LL_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TT_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_LT_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TL_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

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
  TH1F* estimate_LT_Signal = (TH1F*)gROOT->FindObject("MMEstimated_LTEMu_TT");
  TH1F* estimate_TL_Signal = (TH1F*)gROOT->FindObject("MMEstimated_TLEMu_TT");

  estimate_LL_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TT_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_LT_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_TL_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  
  TLegend* qw = 0;
  qw = new TLegend(0.70,0.75,0.95,0.95);
  qw->SetFillColor(kWhite);

  title = new TLatex(10.,10.,"CMS preliminary");
  title->SetNDC();
  title->SetTextAlign(12);
  title->SetX(0.60);
  title->SetY(0.70);
  title->SetTextFont(42);
  title->SetTextSize(0.04);
  title->SetTextSizePixels(24);


  if( selection == "TT"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, tt_ZJets);
      tmp->Add(tmp, tt_TtW);
      tmp->Add(tmp, tt_TbartW);
      tmp->Add(tmp, tt_DYToTauTauHigh);
      tmp->Add(tmp, tt_DYToTauTauLow);
      tmp->Add(tmp, tt_WW);
      tmp->Add(tmp, tt_WZ);
      tmp->Add(tmp, tt_ZZ);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tt_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(tt_ZJets,    " Z+Jets  ","f");
      qw->AddEntry(tt_TtW,       " tW  ","f");
      qw->AddEntry(tt_DYToTauTauHigh,    " DY ","f");
      qw->AddEntry(tt_WW,    " Dibosons  ","f");
      qw->AddEntry(estimate_TT_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("TTSignalEMu"+ntupleType+fast+other+".pdf").c_str());
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, tt_TTSemi);
      tmp->Add(tmp, tt_WJets);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      //ttWLike->Draw("hesame");
      thegraph->Draw("e2same");
      qw->AddEntry(tt_TTSemi, " t#bar{t} semileptonic ","f");
      qw->AddEntry(tt_WJets,       " W+jets  ","f");
      qw->AddEntry(estimate_TT_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("TTWlikeEMu"+ntupleType+fast+other+".pdf").c_str());
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      canv->SaveAs(("TTQCDEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
  }
  
  if( selection == "LT"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, lt_ZJets);
      tmp->Add(tmp, lt_TtW);
      tmp->Add(tmp, lt_TbartW);
      tmp->Add(tmp, lt_DYToTauTauHigh);
      tmp->Add(tmp, lt_DYToTauTauLow);
      tmp->Add(tmp, lt_WW);
      tmp->Add(tmp, lt_WZ);
      tmp->Add(tmp, lt_ZZ);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(lt_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(lt_ZJets,    " Z+Jets  ","f");
      qw->AddEntry(lt_TtW,       " tW  ","f");
      qw->AddEntry(lt_DYToTauTauHigh,    " DY ","f");
      qw->AddEntry(lt_WW,    " Dibosons  ","f");
      qw->AddEntry(estimate_LT_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("LTSignalEMu"+ntupleType+fast+other+".pdf").c_str());
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, lt_TTSemi);
      tmp->Add(tmp, lt_WJets);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(lt_TTSemi, " t#bar{t} semileptonic ","f");
      qw->AddEntry(lt_WJets,       " W+jets  ","f");
      qw->AddEntry(estimate_LT_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("LTWlikeEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      canv->SaveAs(("LTQCDEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
  }






  if( selection == "TL"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, tl_ZJets);
      tmp->Add(tmp, tl_TtW);
      tmp->Add(tmp, tl_TbartW);
      tmp->Add(tmp, tl_DYToTauTauHigh);
      tmp->Add(tmp, tl_DYToTauTauLow);
      tmp->Add(tmp, tl_WW);
      tmp->Add(tmp, tl_WZ);
      tmp->Add(tmp, tl_ZZ);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tl_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(tl_TtW,       " tW  ","f");
      qw->AddEntry(tl_DYToTauTauHigh,    " DY ","f");
      qw->AddEntry(tl_WW,    " Dibosons  ","f");
      qw->AddEntry(estimate_TL_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("TLSignalEMu"+ntupleType+fast+other+".pdf").c_str());
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, tl_TTSemi);
      tmp->Add(tmp, tl_WJets);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tl_TTSemi, " t#bar{t} semileptonic ","f");
      qw->AddEntry(tl_WJets,       " W+jets  ","f");
      qw->AddEntry(estimate_TL_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("TLWlikeEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      canv->SaveAs(("TLQCDEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
  }









  
  
  
  
  
  if( selection == "LL"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, ll_ZJets);
      tmp->Add(tmp, ll_TtW);
      tmp->Add(tmp, ll_TbartW);
      tmp->Add(tmp, ll_DYToTauTauHigh);
      tmp->Add(tmp, ll_DYToTauTauLow);
      tmp->Add(tmp, ll_WW);
      tmp->Add(tmp, ll_WZ);
      tmp->Add(tmp, ll_ZZ);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(ll_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(ll_TtW,       " tW  ","f");
      qw->AddEntry(ll_DYToTauTauHigh,    " DY ","f");
      qw->AddEntry(ll_WW,    " Dibosons  ","f");
      qw->AddEntry(estimate_LL_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      canv->SaveAs(("LLSignalEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      tmp->Add(tmp, ll_TTSemi);
      tmp->Add(tmp, ll_WJets);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(ll_TTSemi, " t#bar{t} semileptonic ","f");
      qw->AddEntry(ll_WJets,       " W+jets  ","f");
      qw->AddEntry(estimate_LL_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      canv->SaveAs(("LLWlikeEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canv"+selection+eventType+ntupleType+fast+other).c_str(), ("canv"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
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
      canv->SaveAs(("LLQCDEMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
  }
 
 
}
