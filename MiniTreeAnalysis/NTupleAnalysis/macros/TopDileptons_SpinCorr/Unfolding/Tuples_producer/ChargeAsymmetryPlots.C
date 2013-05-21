ChargeAsymmetryPlots(){

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetOptTitle(0);

  TFile * file_Estimated  = new TFile("ChargeAsymmetryResults.root");

  file_Estimated->cd();

  TCanvas * canv0 = new TCanvas("canv0", "canv0", 600, 600);
  TH1F* hGenDelta_y  = (TH1F*)gROOT->FindObject("GenDelta_y");
  hGenDelta_y->GetXaxis()->SetTitle("|y_{t}^{gen}| - |y_{#bar{t}}^{gen}|");
  hGenDelta_y->GetYaxis()->SetTitle("Number of events");
  hGenDelta_y->GetYaxis()->SetLabelSize(0.020);
  hGenDelta_y->Draw();

  TCanvas * canv1 = new TCanvas("canv1", "canv1", 600, 600);
  TH1F* hGenTTRapidity  = (TH1F*)gROOT->FindObject("GenTTRapidity");
  hGenTTRapidity->GetXaxis()->SetTitle("y_{t #bar{t}}^{gen}");
  hGenTTRapidity->GetYaxis()->SetTitle("Number of events");
  hGenTTRapidity->GetYaxis()->SetLabelSize(0.020);
  hGenTTRapidity->Draw();


  TCanvas * canv2 = new TCanvas("canv2", "canv2", 600, 600);
  TH1F* hGenTTPt  = (TH1F*)gROOT->FindObject("GenTTPt");
  hGenTTPt->GetXaxis()->SetTitle("P_{T t #bar{t}}^{gen}");
  hGenTTPt->GetYaxis()->SetTitle("Number of events");
  hGenTTPt->GetYaxis()->SetLabelSize(0.020);
  hGenTTPt->Draw();

  TCanvas * canv3 = new TCanvas("canv3", "canv3", 600, 600);
  TH1F* hGenTTMass  = (TH1F*)gROOT->FindObject("GenTTMass");
  hGenTTMass->GetXaxis()->SetTitle("m_{t #bar{t}}^{gen}");
  hGenTTMass->GetYaxis()->SetTitle("Number of events");
  hGenTTMass->GetYaxis()->SetLabelSize(0.020);
  hGenTTMass->Draw();

  TCanvas * canv4 = new TCanvas("canv4", "canv4", 600, 600);
  TH1F* hGenA_cTTRapidity  = (TH1F*)gROOT->FindObject("FineBinning_GenA_cTTRapidity");
  hGenA_cTTRapidity->GetXaxis()->SetTitle("y_{t #bar{t}}^{gen}");
  hGenA_cTTRapidity->GetYaxis()->SetTitle("A_{C}^{gen}");
  hGenA_cTTRapidity->GetYaxis()->SetLabelSize(0.020);
  hGenA_cTTRapidity->Draw();

  TCanvas * canv5 = new TCanvas("canv5", "canv5", 600, 600);
  TH1F* hGenA_cTTPt  = (TH1F*)gROOT->FindObject("FineBinning_GenA_cTTPt");
  hGenA_cTTPt->GetXaxis()->SetTitle("P_{T t #bar{t}}^{gen}");
  hGenA_cTTPt->GetYaxis()->SetTitle("A_{C}^{gen}");
  hGenA_cTTPt->GetYaxis()->SetLabelSize(0.020);
  hGenA_cTTPt->Draw();

  TCanvas * canv6 = new TCanvas("canv6", "canv6", 600, 600);
  TH1F* hGenA_cTTMass  = (TH1F*)gROOT->FindObject("FineBinning_GenA_cTTMass");
  hGenA_cTTMass->GetXaxis()->SetTitle("m_{t #bar{t}}^{gen}");
  hGenA_cTTMass->GetYaxis()->SetTitle("A_{C}^{gen}");
  hGenA_cTTMass->GetYaxis()->SetLabelSize(0.020);
  hGenA_cTTMass->Draw();

  TCanvas * canv7 = new TCanvas("canv7", "canv7", 600, 600);
  TH1F* hRecoTTRapidity  = (TH1F*)gROOT->FindObject("RecoTTRapidity");
  hRecoTTRapidity->GetXaxis()->SetTitle("y_{t #bar{t}}^{reco}");
  hRecoTTRapidity->GetYaxis()->SetTitle("Number of events");
  hRecoTTRapidity->GetYaxis()->SetLabelSize(0.020);
  hRecoTTRapidity->Draw();

  TCanvas * canv8 = new TCanvas("canv8", "canv8", 600, 600);
  TH1F* hRecoTTPt  = (TH1F*)gROOT->FindObject("RecoTTPt");
  hRecoTTPt->GetXaxis()->SetTitle("P_{T t #bar{t}}^{reco}");
  hRecoTTPt->GetYaxis()->SetTitle("Number of events");
  hRecoTTPt->GetYaxis()->SetLabelSize(0.020);
  hRecoTTPt->Draw();

  TCanvas * canv9 = new TCanvas("canv9", "canv9", 600, 600);
  TH1F* hRecoTTMass  = (TH1F*)gROOT->FindObject("RecoTTMass");
  hRecoTTMass->GetXaxis()->SetTitle("m_{t #bar{t}}^{reco}");
  hRecoTTMass->GetYaxis()->SetTitle("Number of events");
  hRecoTTMass->GetYaxis()->SetLabelSize(0.020);
  hRecoTTMass->Draw();

  TCanvas * canv10 = new TCanvas("canv10", "canv10", 600, 600);
  TH1F* hRecoDelta_y  = (TH1F*)gROOT->FindObject("RecoDelta_y");
  hRecoDelta_y->GetXaxis()->SetTitle("|y_{t}^{reco}| - |y_{#bar{t}}^{reco}|");
  hRecoDelta_y->GetYaxis()->SetTitle("Number of events");
  hRecoDelta_y->GetYaxis()->SetLabelSize(0.020);
  hRecoDelta_y->Draw();

  TCanvas * canv11 = new TCanvas("canv11", "canv11", 600, 600);
  TH1F* hDelta_yEfficiency  = (TH1F*)gROOT->FindObject("Delta_yEfficiency");
  hDelta_yEfficiency->GetXaxis()->SetTitle("|y_{t}| - |y_{#bar{t}}|");
  hDelta_yEfficiency->GetYaxis()->SetTitle("Selection efficiency");
  hDelta_yEfficiency->GetYaxis()->SetLabelSize(0.020);
  hDelta_yEfficiency->Draw();

  TCanvas * canv12 = new TCanvas("canv12", "canv12", 600, 600);
  TH1F* hRecoMinusGenDivGenTopMass1  = (TH1F*)gROOT->FindObject("RecoMinusGenDivGenTopMass1");
  hRecoMinusGenDivGenTopMass1->GetXaxis()->SetTitle("(m_{t}^{reco} - m_{t}^{gen})/m_{t}^{gen}");
  hRecoMinusGenDivGenTopMass1->GetYaxis()->SetTitle("Number of events");
  hRecoMinusGenDivGenTopMass1->GetYaxis()->SetLabelSize(0.020);
  hRecoMinusGenDivGenTopMass1->Draw();


  TCanvas * canv13 = new TCanvas("canv13", "canv13", 600, 600);
  TH1F* hRecoMinusGenDivGenTopMass2  = (TH1F*)gROOT->FindObject("RecoMinusGenDivGenTopMass2");
  hRecoMinusGenDivGenTopMass2->GetXaxis()->SetTitle("(m_{#bar{t}}^{reco} - m_{#bar{t}}^{gen})/m_{#bar{t}}^{gen}");
  hRecoMinusGenDivGenTopMass2->GetYaxis()->SetTitle("Number of events");
  hRecoMinusGenDivGenTopMass2->GetYaxis()->SetLabelSize(0.020);
  hRecoMinusGenDivGenTopMass2->Draw();

  TLegend* qw1 = 0;
  qw1 = new TLegend(0.70, 0.75, 0.95, 0.95);
  qw1->SetFillColor(kWhite);

  TCanvas * canv14 = new TCanvas("canv14", "canv14", 600, 600);
  TH1F* hRecoMinusGenDivGenTopRapidity1  = (TH1F*)gROOT->FindObject("RecoMinusGenDivGenTopRapidity1");
  TH1F* hRecoMinusGenDivGenLeptonRapidity1  = (TH1F*)gROOT->FindObject("RecoMinusGenDivGenLeptonRapidity1");
  hRecoMinusGenDivGenTopRapidity1->GetXaxis()->SetTitle("(y_{t}^{reco} - y_{t}^{gen})/y_{t}^{gen}");
  hRecoMinusGenDivGenTopRapidity1->SetLineColor(kRed);
  hRecoMinusGenDivGenLeptonRapidity1->SetLineColor(kBlue);
  hRecoMinusGenDivGenTopRapidity1->SetLineWidth(3);
  hRecoMinusGenDivGenLeptonRapidity1->SetLineWidth(3);
  hRecoMinusGenDivGenTopRapidity1->GetYaxis()->SetTitle("Number of events");
  hRecoMinusGenDivGenTopRapidity1->GetYaxis()->SetLabelSize(0.020);
  hRecoMinusGenDivGenTopRapidity1->Draw();
  hRecoMinusGenDivGenLeptonRapidity1->Draw("same");
  qw1->AddEntry(hRecoMinusGenDivGenTopRapidity1, "Reco Top");
  qw1->AddEntry(hRecoMinusGenDivGenLeptonRapidity1, "Reco Lepton");
  qw1->Draw("same");

  TLegend* qw2 = 0;
  qw2 = new TLegend(0.70, 0.75, 0.95, 0.95);
  qw2->SetFillColor(kWhite);

  TCanvas * canv15 = new TCanvas("canv15", "canv15", 600, 600);
  TH1F* hRecoMinusGenDivGenTopRapidity2  = (TH1F*)gROOT->FindObject("RecoMinusGenDivGenTopRapidity2");
  TH1F* hRecoMinusGenDivGenLeptonRapidity2  = (TH1F*)gROOT->FindObject("RecoMinusGenDivGenLeptonRapidity2");
  hRecoMinusGenDivGenTopRapidity2->GetXaxis()->SetTitle("(y_{#bar{t}}^{reco} - y_{#bar{t}}^{gen})/y_{#bar{t}}^{gen}");
  hRecoMinusGenDivGenTopRapidity2->SetLineColor(kRed);
  hRecoMinusGenDivGenLeptonRapidity2->SetLineColor(kBlue);
  hRecoMinusGenDivGenTopRapidity2->SetLineWidth(3);
  hRecoMinusGenDivGenLeptonRapidity2->SetLineWidth(3);
  hRecoMinusGenDivGenTopRapidity2->GetYaxis()->SetTitle("Number of events");
  hRecoMinusGenDivGenTopRapidity2->GetYaxis()->SetLabelSize(0.020);
  hRecoMinusGenDivGenTopRapidity2->Draw();
  hRecoMinusGenDivGenLeptonRapidity2->Draw("same");
  qw2->AddEntry(hRecoMinusGenDivGenTopRapidity1, "Reco Top");
  qw2->AddEntry(hRecoMinusGenDivGenLeptonRapidity1, "Reco Lepton");
  qw2->Draw("same");

  TCanvas * canv18 = new TCanvas("canv18", "canv18", 600, 600);
  TH2F* hRecoVsGenDelta_y  = (TH2F*)gROOT->FindObject("RecoVsGenDelta_y");
  hRecoVsGenDelta_y->GetXaxis()->SetTitle("|y_{t}^{reco}| - |y_{#bar{t}}^{reco}|");
  hRecoVsGenDelta_y->GetYaxis()->SetLabelSize(0.020);
  hRecoVsGenDelta_y->GetYaxis()->SetTitle("|y_{t}^{gen}| - |y_{#bar{t}}^{gen}|");
  hRecoVsGenDelta_y->GetZaxis()->SetLabelSize(0.020);
  double nentries = hRecoVsGenDelta_y->GetEntries();
  hRecoVsGenDelta_y->Scale(1/nentries);
  hRecoVsGenDelta_y->SetMinimum(0.);
  hRecoVsGenDelta_y->SetMaximum(0.05);
  TPaveText *myText = new TPaveText(0.2,0.9,0.3,1.0, "NDC"); //NDC sets coords relative to pad
  myText->SetTextSize(0.04);
  myText->SetFillColor(0); //white background
  myText->SetTextAlign(12);
  myTextEntry = myText->AddText("Migration Matrix (plot normalized to 1)");
  hRecoVsGenDelta_y->Draw("COLZ");
  myText->Draw("same");

  TCanvas * canv19 = new TCanvas("canv19", "canv19", 600, 600);
  TH2F* hDelta_yVsTTMassEfficiency  = (TH2F*)gROOT->FindObject("Delta_yVsTTMassEfficiency");
  hDelta_yVsTTMassEfficiency->GetXaxis()->SetTitle("|y_{t}| - |y_{#bar{t}}|");
  hDelta_yVsTTMassEfficiency->GetYaxis()->SetLabelSize(0.020);
  hDelta_yVsTTMassEfficiency->GetYaxis()->SetTitle("m_{t #bar{t}}");
  //  hDelta_yVsTTMassEfficiency->GetYaxis()->SetRangeUser(200,1000);
  hDelta_yVsTTMassEfficiency->GetZaxis()->SetLabelSize(0.020);
  hDelta_yVsTTMassEfficiency->GetZaxis()->SetTitle("Selection efficiency");
  hDelta_yVsTTMassEfficiency->GetZaxis()->SetTitleOffset(-0.35);
  hDelta_yVsTTMassEfficiency->SetMinimum(0.);
  hDelta_yVsTTMassEfficiency->SetMaximum(0.1);
  hDelta_yVsTTMassEfficiency->Draw("COLZ");

  TCanvas * canv20 = new TCanvas("canv20", "canv20", 600, 600);
  TH2F* hFineBinning_GenDelta_yVsTTMass  = (TH2F*)gROOT->FindObject("FineBinning_GenDelta_yVsTTMass");
  hFineBinning_GenDelta_yVsTTMass->GetXaxis()->SetTitle("|y_{t}^{gen}| - |y_{#bar{t}}^{gen}|");
  hFineBinning_GenDelta_yVsTTMass->GetYaxis()->SetLabelSize(0.020);
  hFineBinning_GenDelta_yVsTTMass->GetYaxis()->SetTitle("m_{t #bar{t}}^{gen}");
  //  hFineBinning_GenDelta_yVsTTMass->GetYaxis()->SetRangeUser(200,1000);
  hFineBinning_GenDelta_yVsTTMass->GetZaxis()->SetLabelSize(0.020);
  hFineBinning_GenDelta_yVsTTMass->GetZaxis()->SetTitle("Number of events");
  hFineBinning_GenDelta_yVsTTMass->GetZaxis()->SetTitleOffset(-0.35);
  hFineBinning_GenDelta_yVsTTMass->Draw("COLZ");

 
 
}
