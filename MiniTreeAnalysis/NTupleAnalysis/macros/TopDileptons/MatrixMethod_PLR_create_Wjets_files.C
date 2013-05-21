MatrixMethod_PLR_create_Wjets_files(){


  TFile * file_IN_EE = new TFile("MatrixMethod_OutPut_EECase_DATA_Fast.root");
  TFile * file_IN_MuMu = new TFile("MatrixMethod_OutPut_MuMuCase_DATA_Fast.root");
  TFile * file_IN_EMu = new TFile("MatrixMethod_OutPut_EMuCase_DATA_Fast.root");

  file_IN_EE->cd();

  TH1F* Wjets_EE  = (TH1F*)gROOT->FindObject("MMEstimated_TightEE_WJets");

  TH1F* ee_Count_Wjets = new TH1F("ee_Count_Wjets","ee_Count_Wjets",1,0,1);
  TH1F* ee_Njets_Wjets = new TH1F("ee_Njets_Wjets","ee_Njets_Wjets",5,-0.5,4.5);

  TH1F* ee_Count_JES_plus_Wjets = new TH1F("ee_Count_JES-plus_Wjets","ee_Count_JES-plus_Wjets",1,0,1);
  TH1F* ee_Njets_JES_plus_Wjets = new TH1F("ee_Njets_JES-plus_Wjets","ee_Njets_JES-plus_Wjets",5,-0.5,4.5);

  TH1F* ee_Count_JES_minus_Wjets = new TH1F("ee_Count_JES-minus_Wjets","ee_Count_JES-minus_Wjets",1,0,1);
  TH1F* ee_Njets_JES_minus_Wjets = new TH1F("ee_Njets_JES-minus_Wjets","ee_Njets_JES-minus_Wjets",5,-0.5,4.5);

  float bin_content_Sum_EE = 0;
  float inclusive_bin_EE = 0;
  for(unsigned int i = 1; i < 8; i++){
   float bin_content = Wjets_EE->GetBinContent(i);
   if(bin_content >= 0) bin_content_Sum_EE = bin_content_Sum_EE + bin_content;
   if(i < 5){
     if(bin_content >= 0){ee_Njets_Wjets->SetBinContent(i, bin_content);
     }else{ee_Njets_Wjets->SetBinContent(i, 0);}
   }else{
     if (bin_content >= 0) inclusive_bin_EE = inclusive_bin_EE + bin_content;
   }
  }
  ee_Njets_Wjets->SetBinContent(5, inclusive_bin_EE);
  ee_Count_Wjets->SetBinContent(1, bin_content_Sum_EE);

  ee_Count_JES_plus_Wjets = ee_Count_Wjets;
  ee_Njets_JES_plus_Wjets = ee_Njets_Wjets;

  ee_Count_JES_minus_Wjets = ee_Count_Wjets;
  ee_Njets_JES_minus_Wjets = ee_Njets_Wjets;



  TFile * file_OUT_EE = new TFile("Feed_PLR_EE.root","RECREATE");
  file_OUT_EE->cd();

  ee_Count_Wjets->Write();
  ee_Njets_Wjets->Write();

  ee_Count_JES_plus_Wjets->Write("ee_Count_JES-plus_Wjets");
  ee_Njets_JES_plus_Wjets->Write("ee_Njets_JES-plus_Wjets");

  ee_Count_JES_minus_Wjets->Write("ee_Count_JES-minus_Wjets");
  ee_Njets_JES_minus_Wjets->Write("ee_Njets_JES-minus_Wjets");




  file_OUT_EE->Close();



  file_IN_MuMu->cd();

  TH1F* Wjets_MuMu  = (TH1F*)gROOT->FindObject("MMEstimated_TightMuMu_WJets");

  TH1F* mumu_Count_Wjets = new TH1F("mumu_Count_Wjets","mumu_Count_Wjets",1,0,1);
  TH1F* mumu_Njets_Wjets = new TH1F("mumu_Njets_Wjets","mumu_Njets_Wjets",5,-0.5,4.5);

  TH1F* mumu_Count_JES_plus_Wjets = new TH1F("mumu_Count_JES-plus_Wjets","mumu_Count_JES-plus_Wjets",1,0,1);
  TH1F* mumu_Njets_JES_plus_Wjets = new TH1F("mumu_Njets_JES-plus_Wjets","mumu_Njets_JES-plus_Wjets",5,-0.5,4.5);

  TH1F* mumu_Count_JES_minus_Wjets = new TH1F("mumu_Count_JES-minus_Wjets","mumu_Count_JES-minus_Wjets",1,0,1);
  TH1F* mumu_Njets_JES_minus_Wjets = new TH1F("mumu_Njets_JES-minus_Wjets","mumu_Njets_JES-minus_Wjets",5,-0.5,4.5);



  float bin_content_Sum_MuMu = 0;
  float inclusive_bin_MuMu = 0;
  for(unsigned int i = 1; i < 8; i++){
   float bin_content = Wjets_MuMu->GetBinContent(i);
   if (bin_content >= 0) bin_content_Sum_MuMu = bin_content_Sum_MuMu + bin_content;
   if(i < 5){
     if (bin_content >= 0){mumu_Njets_Wjets->SetBinContent(i, bin_content);
     }else{mumu_Njets_Wjets->SetBinContent(i, 0);}
   }else{
     if (bin_content >= 0) inclusive_bin_MuMu = inclusive_bin_MuMu + bin_content;
   }
  }
  mumu_Njets_Wjets->SetBinContent(5, inclusive_bin_MuMu);
  mumu_Count_Wjets->SetBinContent(1, bin_content_Sum_MuMu);

  mumu_Count_JES_plus_Wjets = mumu_Count_Wjets;
  mumu_Njets_JES_plus_Wjets = mumu_Njets_Wjets;

  mumu_Count_JES_minus_Wjets = mumu_Count_Wjets;
  mumu_Njets_JES_minus_Wjets = mumu_Njets_Wjets;



  TFile * file_OUT_MuMu = new TFile("Feed_PLR_MuMu.root","RECREATE");
  file_OUT_MuMu->cd();

  mumu_Count_Wjets->Write();
  mumu_Njets_Wjets->Write();

  mumu_Count_JES_plus_Wjets->Write("mumu_Count_JES-plus_Wjets");
  mumu_Njets_JES_plus_Wjets->Write("mumu_Njets_JES-plus_Wjets");

  mumu_Count_JES_minus_Wjets->Write("mumu_Count_JES-minus_Wjets");
  mumu_Njets_JES_minus_Wjets->Write("mumu_Njets_JES-minus_Wjets");




  file_OUT_MuMu->Close();





  file_IN_EMu->cd();

  TH1F* TF_EMu  = (TH1F*)gROOT->FindObject("MMEstimated_TTEMu_TF");
  TH1F* FT_EMu  = (TH1F*)gROOT->FindObject("MMEstimated_TTEMu_FT");

  TH1F* emu_Count_Wjets = new TH1F("emu_Count_Wjets","emu_Count_Wjets",1,0,1);
  TH1F* emu_Njets_Wjets = new TH1F("emu_Njets_Wjets","emu_Njets_Wjets",5,-0.5,4.5);


  TH1F* emu_Count_JES_plus_Wjets = new TH1F("emu_Count_JES-plus_Wjets","emu_Count_JES-plus_Wjets",1,0,1);
  TH1F* emu_Njets_JES_plus_Wjets = new TH1F("emu_Njets_JES-plus_Wjets","emu_Njets_JES-plus_Wjets",5,-0.5,4.5);

  TH1F* emu_Count_JES_minus_Wjets = new TH1F("emu_Count_JES-minus_Wjets","emu_Count_JES-minus_Wjets",1,0,1);
  TH1F* emu_Njets_JES_minus_Wjets = new TH1F("emu_Njets_JES-minus_Wjets","emu_Njets_JES-minus_Wjets",5,-0.5,4.5);



  float bin_content_Sum_EMu = 0;
  float inclusive_bin_EMu = 0;
  for(unsigned int i = 1; i < 8; i++){
   float bin_content_tf = TF_EMu->GetBinContent(i);
   float bin_content_ft = FT_EMu->GetBinContent(i);
   if(bin_content_tf >= 0) bin_content_Sum_EMu = bin_content_Sum_EMu + bin_content_tf;
   if(bin_content_ft >= 0) bin_content_Sum_EMu = bin_content_Sum_EMu + bin_content_ft;
   if(i < 5){
     if(bin_content_tf >=0 && bin_content_tf >= 0) emu_Njets_Wjets->SetBinContent(i, bin_content_tf+bin_content_ft);
     if(bin_content_tf >=0 && bin_content_tf < 0) emu_Njets_Wjets->SetBinContent(i, bin_content_tf);
     if(bin_content_ft >=0 && bin_content_ft < 0) emu_Njets_Wjets->SetBinContent(i, bin_content_ft);
     if(bin_content_tf < 0 && bin_content_tf < 0) emu_Njets_Wjets->SetBinContent(i, 0);
   }else{
     if (bin_content_tf >= 0) inclusive_bin_EMu = inclusive_bin_EMu + bin_content_tf;
     if (bin_content_ft >= 0)  inclusive_bin_EMu = inclusive_bin_EMu + bin_content_ft;
   }
  }
  emu_Njets_Wjets->SetBinContent(5, inclusive_bin_EMu);
  emu_Count_Wjets->SetBinContent(1, bin_content_Sum_EMu);

  emu_Count_JES_plus_Wjets = emu_Count_Wjets;
  emu_Njets_JES_plus_Wjets = emu_Njets_Wjets;

  emu_Count_JES_minus_Wjets = emu_Count_Wjets;
  emu_Njets_JES_minus_Wjets = emu_Njets_Wjets;


  TFile * file_OUT_EMu = new TFile("Feed_PLR_EMu.root","RECREATE");
  file_OUT_EMu->cd();

  emu_Count_Wjets->Write();
  emu_Njets_Wjets->Write();


  emu_Count_JES_plus_Wjets->Write("emu_Count_JES-plus_Wjets");
  emu_Njets_JES_plus_Wjets->Write("emu_Njets_JES-plus_Wjets");

  emu_Count_JES_minus_Wjets->Write("emu_Count_JES-minus_Wjets");
  emu_Njets_JES_minus_Wjets->Write("emu_Njets_JES-minus_Wjets");




  file_OUT_EMu->Close();










 
}
