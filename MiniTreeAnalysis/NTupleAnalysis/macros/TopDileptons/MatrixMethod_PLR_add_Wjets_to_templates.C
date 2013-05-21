MatrixMethod_PLR_add_Wjets_to_templates(){


  TFile * file_IN_EE = new TFile("Feed_PLR_EE.root");

  file_IN_EE->cd();

  TH1F* ee_Count_Wjets = (TH1F*)gDirectory->Get("ee_Count_Wjets");
  TH1F* ee_Count_JES_minus_Wjets = (TH1F*)gDirectory->Get("ee_Count_JES-minus_Wjets");
  TH1F* ee_Count_JES_plus_Wjets = (TH1F*)gDirectory->Get("ee_Count_JES-plus_Wjets");

  TH1F* ee_Njets_Wjets = (TH1F*)gDirectory->Get("ee_Njets_Wjets");
  TH1F* ee_Njets_JES_minus_Wjets = (TH1F*)gDirectory->Get("ee_Njets_JES-minus_Wjets");
  TH1F* ee_Njets_JES_plus_Wjets = (TH1F*)gDirectory->Get("ee_Njets_JES-plus_Wjets");



  TFile * file_IN_MuMu = new TFile("Feed_PLR_MuMu.root");

  file_IN_MuMu->cd();

  TH1F* mumu_Count_Wjets = (TH1F*)gDirectory->Get("mumu_Count_Wjets");
  TH1F* mumu_Count_JES_minus_Wjets = (TH1F*)gDirectory->Get("mumu_Count_JES-minus_Wjets");
  TH1F* mumu_Count_JES_plus_Wjets = (TH1F*)gDirectory->Get("mumu_Count_JES-plus_Wjets");

  TH1F* mumu_Njets_Wjets = (TH1F*)gDirectory->Get("mumu_Njets_Wjets");
  TH1F* mumu_Njets_JES_minus_Wjets = (TH1F*)gDirectory->Get("mumu_Njets_JES-minus_Wjets");
  TH1F* mumu_Njets_JES_plus_Wjets = (TH1F*)gDirectory->Get("mumu_Njets_JES-plus_Wjets");



  TFile * file_IN_EMu = new TFile("Feed_PLR_EMu.root");

  file_IN_EMu->cd();

  TH1F* emu_Count_Wjets = (TH1F*)gDirectory->Get("emu_Count_Wjets");
  TH1F* emu_Count_JES_minus_Wjets = (TH1F*)gDirectory->Get("emu_Count_JES-minus_Wjets");
  TH1F* emu_Count_JES_plus_Wjets = (TH1F*)gDirectory->Get("emu_Count_JES-plus_Wjets");

  TH1F* emu_Njets_Wjets = (TH1F*)gDirectory->Get("emu_Njets_Wjets");
  TH1F* emu_Njets_JES_minus_Wjets = (TH1F*)gDirectory->Get("emu_Njets_JES-minus_Wjets");
  TH1F* emu_Njets_JES_plus_Wjets = (TH1F*)gDirectory->Get("emu_Njets_JES-plus_Wjets");





  TFile * file_OUT = new TFile("FinalTemplates_withDYPowheg.root","update");

  file_OUT->cd();

  ee_Count_Wjets->Write("ee_Count_Wjets");
  ee_Count_JES_minus_Wjets->Write("ee_Count_JES-minus_Wjets");
  ee_Count_JES_plus_Wjets->Write("ee_Count_JES-plus_Wjets");


  ee_Njets_Wjets->Write("ee_Njets_Wjets");
  ee_Njets_JES_minus_Wjets->Write("ee_Njets_JES-minus_Wjets");
  ee_Njets_JES_plus_Wjets->Write("ee_Njets_JES-plus_Wjets");



  mumu_Count_Wjets->Write("mumu_Count_Wjets");
  mumu_Count_JES_minus_Wjets->Write("mumu_Count_JES-minus_Wjets");
  mumu_Count_JES_plus_Wjets->Write("mumu_Count_JES-plus_Wjets");


  mumu_Njets_Wjets->Write("mumu_Njets_Wjets");
  mumu_Njets_JES_minus_Wjets->Write("mumu_Njets_JES-minus_Wjets");
  mumu_Njets_JES_plus_Wjets->Write("mumu_Njets_JES-plus_Wjets");




  emu_Count_Wjets->Write("emu_Count_Wjets");
  emu_Count_JES_minus_Wjets->Write("emu_Count_JES-minus_Wjets");
  emu_Count_JES_plus_Wjets->Write("emu_Count_JES-plus_Wjets");


  emu_Njets_Wjets->Write("emu_Njets_Wjets");
  emu_Njets_JES_minus_Wjets->Write("emu_Njets_JES-minus_Wjets");
  emu_Njets_JES_plus_Wjets->Write("emu_Njets_JES-plus_Wjets");


  file_OUT->Close();




 
}
