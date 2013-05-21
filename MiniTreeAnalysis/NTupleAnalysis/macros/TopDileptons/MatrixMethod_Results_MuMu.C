MatrixMethod_Results_MuMu(string selection, string eventType, string ntupleType, string fast, string other){


  float NjetsMin = -0.5;
  float NjetsMax = 5.5;

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TFile * file_Expected   = new TFile(("MatrixMethod_OutPut_MuMuCase_MC"+other+".root").c_str());
  TFile * file_Estimated  = new TFile(("MatrixMethod_OutPut_MuMuCase"+ntupleType+fast+other+".root").c_str());

  file_Expected->cd();
  TH1F* tight_TTDilept  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_TTbarSignal");
  TH1F* medium_TTDilept = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_TTbarSignal");
  TH1F* loose_TTDilept  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_TTbarSignal");

  tight_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_TTDilept->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_TTSemi  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_TTbarSemileptonic");
  TH1F* medium_TTSemi = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_TTbarSemileptonic");
  TH1F* loose_TTSemi  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_TTbarSemileptonic");

  tight_TTSemi->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_TTSemi->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_TTSemi->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_TTBckg  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_TTbar");
  TH1F* medium_TTBckg = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_TTbar");
  TH1F* loose_TTBckg  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_TTbar");

  tight_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_TTBckg->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_ZJets  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_Zjets");
  TH1F* medium_ZJets = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_Zjets");
  TH1F* loose_ZJets  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_Zjets");

  tight_ZJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_ZJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_ZJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_DYHigh  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_DYToMuMu_M-20");
  TH1F* medium_DYHigh = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_DYToMuMu_M-20");
  TH1F* loose_DYHigh  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_DYToMuMu_M-20");

  tight_DYHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_DYHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_DYHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  
  TH1F* tight_DYLow  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_DYToMuMu_M-10To20");
  TH1F* medium_DYLow = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_DYToMuMu_M-10To20");
  TH1F* loose_DYLow  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_DYToMuMu_M-10To20");

  tight_DYLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_DYLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_DYLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_DYTauTauHigh  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_DYToTauTau_M-20");
  TH1F* medium_DYTauTauHigh = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_DYToTauTau_M-20");
  TH1F* loose_DYTauTauHigh  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_DYToTauTau_M-20");

  tight_DYTauTauHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_DYTauTauHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_DYTauTauHigh->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  
  TH1F* tight_DYTauTauLow  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_DYToTauTau_M-10To20");
  TH1F* medium_DYTauTauLow = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_DYToTauTau_M-10To20");
  TH1F* loose_DYTauTauLow  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_DYToTauTau_M-10To20");

  tight_DYTauTauLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_DYTauTauLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_DYTauTauLow->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_WJets  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_Wjets");
  TH1F* medium_WJets = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_Wjets");
  TH1F* loose_WJets  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_Wjets");

  tight_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_TtW  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_TtW");
  TH1F* medium_TtW = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_TtW");
  TH1F* loose_TtW  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_TtW");

  tight_TtW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_TtW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_TtW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_TbartW  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_TbartW");
  TH1F* medium_TbartW = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_TbartW");
  TH1F* loose_TbartW  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_TbartW");

  tight_TbartW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_TbartW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_TbartW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_WW  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_WW");
  TH1F* medium_WW = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_WW");
  TH1F* loose_WW  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_WW");

  tight_WW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_WW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_WW->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_WZ  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_WZ");
  TH1F* medium_WZ = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_WZ");
  TH1F* loose_WZ  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_WZ");

  tight_WZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_WZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_WZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* tight_ZZ  = (TH1F*)gROOT->FindObject("MMExpected_TightMuMu_ZZ");
  TH1F* medium_ZZ = (TH1F*)gROOT->FindObject("MMExpected_MediumMuMu_ZZ");
  TH1F* loose_ZZ  = (TH1F*)gROOT->FindObject("MMExpected_LooseMuMu_ZZ");

  tight_ZZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  medium_ZZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  loose_ZZ->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);


  tight_TTDilept->SetFillColor(kRed+1);
  medium_TTDilept->SetFillColor(kRed+1);
  loose_TTDilept->SetFillColor(kRed+1);

  tight_TTSemi->SetFillColor(kRed-7);
  medium_TTSemi->SetFillColor(kRed-7);
  loose_TTSemi->SetFillColor(kRed-7);

  tight_TTBckg->SetFillColor(kRed-7);
  medium_TTBckg->SetFillColor(kRed-7);
  loose_TTBckg->SetFillColor(kRed-7);

  tight_ZJets->SetFillColor(kAzure-2);
  medium_ZJets->SetFillColor(kAzure-2);
  loose_ZJets->SetFillColor(kAzure-2);

  tight_DYHigh->SetFillColor(kAzure-2);
  medium_DYHigh->SetFillColor(kAzure-2);
  loose_DYHigh->SetFillColor(kAzure-2);

  tight_DYLow->SetFillColor(kAzure-2);
  medium_DYLow->SetFillColor(kAzure-2);
  loose_DYLow->SetFillColor(kAzure-2);

  tight_DYTauTauHigh->SetFillColor(kAzure-2);
  medium_DYTauTauHigh->SetFillColor(kAzure-2);
  loose_DYTauTauHigh->SetFillColor(kAzure-2);

  tight_DYTauTauLow->SetFillColor(kAzure-2);
  medium_DYTauTauLow->SetFillColor(kAzure-2);
  loose_DYTauTauLow->SetFillColor(kAzure-2);

  tight_WJets->SetFillColor(kGreen-3);
  medium_WJets->SetFillColor(kGreen-3);
  loose_WJets->SetFillColor(kGreen-3);

  tight_TtW->SetFillColor(kMagenta-6);
  medium_TtW->SetFillColor(kMagenta-6);
  loose_TtW->SetFillColor(kMagenta-6);

  tight_TbartW->SetFillColor(kMagenta-6);
  medium_TbartW->SetFillColor(kMagenta-6);
  loose_TbartW->SetFillColor(kMagenta-6);

  tight_WW->SetFillColor(kGray+2);
  medium_WW->SetFillColor(kGray+2);
  loose_WW->SetFillColor(kGray+2);

  tight_WZ->SetFillColor(kGray+2);
  medium_WZ->SetFillColor(kGray+2);
  loose_WZ->SetFillColor(kGray+2);

  tight_ZZ->SetFillColor(kGray+2);
  medium_ZZ->SetFillColor(kGray+2);
  loose_ZZ->SetFillColor(kGray+2);
  
  THStack* tightSignalLike = new THStack("tightSignalLike","tight signal like");
  tightSignalLike->Add(tight_TTDilept);
  tightSignalLike->Add(tight_TtW);
  tightSignalLike->Add(tight_TbartW);
  tightSignalLike->Add(tight_ZJets);
  tightSignalLike->Add(tight_DYHigh);
  tightSignalLike->Add(tight_DYLow);
  tightSignalLike->Add(tight_DYTauTauHigh);
  tightSignalLike->Add(tight_DYTauTauLow);
  tightSignalLike->Add(tight_WW);
  tightSignalLike->Add(tight_WZ);
  tightSignalLike->Add(tight_ZZ);
  THStack* mediumSignalLike = new THStack("mediumSignalLike","medium signal like");
  mediumSignalLike->Add(medium_TTDilept);
  mediumSignalLike->Add(medium_TtW);
  mediumSignalLike->Add(medium_TbartW);
  mediumSignalLike->Add(medium_ZJets);
  mediumSignalLike->Add(medium_DYHigh);
  mediumSignalLike->Add(medium_DYLow);
  mediumSignalLike->Add(medium_DYTauTauHigh);
  mediumSignalLike->Add(medium_DYTauTauLow);
  mediumSignalLike->Add(medium_WW);
  mediumSignalLike->Add(medium_WZ);
  mediumSignalLike->Add(medium_ZZ);
  THStack* looseSignalLike = new THStack( "looseSignalLike", "loose signal like");
  looseSignalLike->Add(loose_TTDilept);
  looseSignalLike->Add(loose_TtW);
  looseSignalLike->Add(loose_TbartW);
  looseSignalLike->Add(loose_ZJets);
  looseSignalLike->Add(loose_DYHigh);
  looseSignalLike->Add(loose_DYLow);
  looseSignalLike->Add(loose_DYTauTauHigh);
  looseSignalLike->Add(loose_DYTauTauLow);
  looseSignalLike->Add(loose_WW);
  looseSignalLike->Add(loose_WZ);
  looseSignalLike->Add(loose_ZZ);

  THStack* tightWLike = new THStack( "tightWLike"," tight W-like" );
  tightWLike->Add(tight_TTSemi);
  tightWLike->Add(tight_WJets);
  THStack* mediumWLike = new THStack("mediumWLike","medium W-like");
  mediumWLike->Add(medium_TTSemi);
  mediumWLike->Add(medium_WJets);
  THStack* looseWLike = new THStack( "looseWLike", "loose W-like" );
  looseWLike->Add(loose_TTSemi);
  looseWLike->Add(loose_WJets);
  
  THStack* tightQCDLike = new THStack("tightQCDLike",  "tight QCD-like" );
  tightQCDLike->Add(tight_TTBckg);
  THStack* mediumQCDLike = new THStack("mediumQCDLike","medium QCD-like");
  mediumQCDLike->Add(medium_TTBckg);
  THStack* looseQCDLike = new THStack("looseQCDLike",  "loose QCD-like" );
  looseQCDLike->Add(loose_TTBckg);

  
  file_Estimated->cd();
  
  TH1F* estimate_Loose_QCD  = (TH1F*)gROOT->FindObject("MMEstimated_LooseMuMu_QCD" );
  TH1F* estimate_Tight_QCD  = (TH1F*)gROOT->FindObject("MMEstimated_TightMuMu_QCD" );
  TH1F* estimate_Medium_QCD = (TH1F*)gROOT->FindObject("MMEstimated_MediumMuMu_QCD");

  estimate_Loose_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_Tight_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_Medium_QCD->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* estimate_Loose_WJets  = (TH1F*)gROOT->FindObject("MMEstimated_LooseMuMu_WJets" );
  TH1F* estimate_Tight_WJets  = (TH1F*)gROOT->FindObject("MMEstimated_TightMuMu_WJets" );
  TH1F* estimate_Medium_WJets = (TH1F*)gROOT->FindObject("MMEstimated_MediumMuMu_WJets");

  estimate_Loose_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_Tight_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_Medium_WJets->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  TH1F* estimate_Loose_Signal  = (TH1F*)gROOT->FindObject("MMEstimated_LooseMuMu_Signal" );
  TH1F* estimate_Tight_Signal  = (TH1F*)gROOT->FindObject("MMEstimated_TightMuMu_Signal" );
  TH1F* estimate_Medium_Signal = (TH1F*)gROOT->FindObject("MMEstimated_MediumMuMu_Signal");

  estimate_Loose_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_Tight_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
  estimate_Medium_Signal->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);

  
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

  if( selection == "Tight"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Tight_Signal->SetMarkerStyle(20);
      estimate_Tight_Signal->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Tight_Signal->GetYaxis()->SetTitle("Number of events");
      estimate_Tight_Signal->GetYaxis()->SetLabelSize(0.025);
      estimate_Tight_Signal->SetTitle("Signal-like #mu#mu (tight)");
      estimate_Tight_Signal->Draw();
      tightSignalLike->Draw("hesame");
      estimate_Tight_Signal->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tight_TTDilept);
      tmp->Add(tmp, tight_TtW);
      tmp->Add(tmp, tight_TbartW);
      tmp->Add(tmp, tight_ZJets);
      tmp->Add(tmp, tight_DYHigh);
      tmp->Add(tmp, tight_DYLow);
      tmp->Add(tmp, tight_DYTauTauHigh);
      tmp->Add(tmp, tight_DYTauTauLow);
      tmp->Add(tmp, tight_WW);
      tmp->Add(tmp, tight_WZ);
      tmp->Add(tmp, tight_ZZ);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(tight_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(tight_TtW,       " tW  ","f");
      qw->AddEntry(tight_DYHigh,    " DY ","f");
      qw->AddEntry(tight_WW,    " VV  ","f");
      qw->AddEntry(estimate_Tight_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("TightSignalMuMu"+ntupleType+fast+other+".pdf").c_str());
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Tight_WJets->SetMarkerStyle(20);
      estimate_Tight_WJets->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Tight_WJets->GetYaxis()->SetTitle("Number of events");
      estimate_Tight_WJets->GetYaxis()->SetLabelSize(0.025);
      estimate_Tight_WJets->SetTitle("W+jets-like #mu#mu (tight)");
      estimate_Tight_WJets->Draw();
      tightWLike->Draw("hesame");
      estimate_Tight_WJets->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tight_TTSemi);
      tmp->Add(tmp, tight_WJets);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      //tightWLike->Draw("hesame");
      thegraph->Draw("e2same");
      qw->AddEntry(tight_TTSemi, " t#bar{t} semileptonic ","f");
      qw->AddEntry(tight_WJets,       " W  ","f");
      qw->AddEntry(estimate_Tight_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("TightWlikeMuMu"+ntupleType+fast+other+".pdf").c_str());
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Tight_QCD->SetMarkerStyle(20);
      estimate_Tight_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Tight_QCD->GetYaxis()->SetTitle("Number of events");
      estimate_Tight_QCD->GetYaxis()->SetLabelSize(0.025);
      estimate_Tight_QCD->SetTitle("QCD-like #mu#mu (tight)");
      estimate_Tight_QCD->Draw();
      tightQCDLike->Draw("hesame");
      estimate_Tight_QCD->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, tight_TTBckg);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      estimate_Tight_QCD->Draw("same");
      thegraph->Draw("e2same");
      qw->AddEntry(tight_TTBckg, " t#bar{t} hadronic ","f");
      qw->AddEntry(estimate_Tight_QCD, " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("TightQCDMuMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
  }
  
  if( selection == "Medium"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Medium_Signal->SetMarkerStyle(20);
      estimate_Medium_Signal->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Medium_Signal->GetYaxis()->SetTitle("Number of events");
      estimate_Medium_Signal->GetYaxis()->SetLabelSize(0.025);
      estimate_Medium_Signal->SetTitle("Signal-like #mu#mu (medium)");
      estimate_Medium_Signal->Draw();
      mediumSignalLike->Draw("hesame");
      estimate_Medium_Signal->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, medium_TTDilept);
      tmp->Add(tmp, medium_TtW);
      tmp->Add(tmp, medium_TbartW);
      tmp->Add(tmp, medium_ZJets);
      tmp->Add(tmp, medium_DYHigh);
      tmp->Add(tmp, medium_DYLow);
      tmp->Add(tmp, medium_DYTauTauHigh);
      tmp->Add(tmp, medium_DYTauTauLow);
      tmp->Add(tmp, medium_WW);
      tmp->Add(tmp, medium_WZ);
      tmp->Add(tmp, medium_ZZ);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(medium_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(medium_TtW,       " tW  ","f");
      qw->AddEntry(tight_DYHigh,    " DY  ","f");
      qw->AddEntry(medium_WW,    " VV ","f");
      qw->AddEntry(estimate_Medium_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("MediumSignalMuMu"+ntupleType+fast+other+".pdf").c_str());
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Medium_WJets->SetMarkerStyle(20);
      estimate_Medium_WJets->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Medium_WJets->GetYaxis()->SetTitle("Number of events");
      estimate_Medium_WJets->GetYaxis()->SetLabelSize(0.025);
      estimate_Medium_WJets->SetTitle("W+jets-like #mu#mu (medium)");
      estimate_Medium_WJets->Draw();
      mediumWLike->Draw("hesame");
      estimate_Medium_WJets->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, medium_TTSemi);
      tmp->Add(tmp, medium_WJets);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(medium_TTSemi, " t#bar{t} semileptonic ","f");
      qw->AddEntry(medium_WJets,       " W  ","f");
      qw->AddEntry(estimate_Medium_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("MediumWlikeMuMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Medium_QCD->SetMarkerStyle(20);
      estimate_Medium_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Medium_QCD->GetYaxis()->SetTitle("Number of events");
      estimate_Medium_QCD->GetYaxis()->SetLabelSize(0.025);
      estimate_Medium_QCD->SetTitle("QCD-like #mu#mu (medium)");
      estimate_Medium_QCD->Draw();
      mediumQCDLike->Draw("hesame");
      estimate_Medium_QCD->Draw("same"); 
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, medium_TTBckg);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(medium_TTBckg, " t#bar{t} hadronic ","f");
      qw->AddEntry(estimate_Medium_QCD, " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("MediumQCDMuMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
  }
  
  
  
  
  
  if( selection == "Loose"){
    
    
    if(eventType == "Signal"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Loose_Signal->SetMarkerStyle(20);
      estimate_Loose_Signal->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Loose_Signal->GetYaxis()->SetTitle("Number of events");
      estimate_Loose_Signal->GetYaxis()->SetLabelSize(0.025);
      estimate_Loose_Signal->SetTitle("Signal-like #mu#mu (loose)");
      estimate_Loose_Signal->Draw();
      looseSignalLike->Draw("hesame");
      estimate_Loose_Signal->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, loose_TTDilept);
      tmp->Add(tmp, loose_TtW);
      tmp->Add(tmp, loose_TbartW);
      tmp->Add(tmp, loose_ZJets);
      tmp->Add(tmp, loose_DYHigh);
      tmp->Add(tmp, loose_DYLow);
      tmp->Add(tmp, loose_DYTauTauHigh);
      tmp->Add(tmp, loose_DYTauTauLow);
      tmp->Add(tmp, loose_WW);
      tmp->Add(tmp, loose_WZ);
      tmp->Add(tmp, loose_ZZ);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(loose_TTDilept, " t#bar{t} signal ","f");
      qw->AddEntry(loose_TtW,       " tW  ","f");
      qw->AddEntry(tight_DYHigh,    " DY  ","f");
      qw->AddEntry(loose_WW,    " VV  ","f");
      qw->AddEntry(estimate_Loose_Signal,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("LooseSignalMuMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
    if(eventType == "Wlike"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Loose_WJets->SetMarkerStyle(20);
      estimate_Loose_WJets->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Loose_WJets->GetYaxis()->SetTitle("Number of events");
      estimate_Loose_WJets->GetYaxis()->SetLabelSize(0.025);
      estimate_Loose_WJets->SetTitle("W+jets-like #mu#mu (loose)");
      estimate_Loose_WJets->Draw();
      looseWLike->Draw("hesame");
      estimate_Loose_WJets->Draw("same");
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, loose_TTSemi);
      tmp->Add(tmp, loose_WJets);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(loose_TTSemi, " t#bar{t} semileptonic ","f");
      qw->AddEntry(loose_WJets,       " W  ","f");
      qw->AddEntry(estimate_Loose_WJets,    " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("LooseWlikeMuMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
    if(eventType == "QCDlike"){
      TCanvas * canv = new TCanvas(("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), ("canvMuMu"+selection+eventType+ntupleType+fast+other).c_str(), 600, 800);
      estimate_Loose_QCD->SetMarkerStyle(20);
      estimate_Loose_QCD->GetXaxis()->SetTitle("Jet multiplicity");
      estimate_Loose_QCD->GetYaxis()->SetTitle("Number of events");
      estimate_Loose_QCD->GetYaxis()->SetLabelSize(0.025);
      estimate_Loose_QCD->SetTitle("QCD-like #mu#mu (loose)");
      estimate_Loose_QCD->Draw(); 
      looseQCDLike->Draw("hesame");
      estimate_Loose_QCD->Draw("same"); 
      TH1F * tmp  = new TH1F("tmp",  "tmp",  11, -0.5,  10.5);
      tmp->GetXaxis()->SetRangeUser(NjetsMin,NjetsMax);
      tmp->Add(tmp, loose_TTBckg);
      TGraphErrors *thegraph = new TGraphErrors(tmp);
      thegraph->SetFillStyle(3005);
      thegraph->SetFillColor(1);
      thegraph->Draw("e2same");
      qw->AddEntry(loose_TTBckg, " t#bar{t} hadronic ","f");
      qw->AddEntry(estimate_Loose_QCD, " Matrix Method  ","p");
      qw->Draw("same");
      title->Draw("same");
      canv->SaveAs(("LooseQCDMuMu"+ntupleType+fast+other+".pdf").c_str());      
    }
    
    
  }
 
 
 
 
 
}
