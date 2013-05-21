triggerEff2(string chan){

  TFile *f1 = new TFile("trig/TriggerPlots.root"); 
  f1->cd();
  
  TString name;
  if (chan=="mumu") {
     name="Muon";
     //HTriggerEff_MuonSel_pT
  }
  else if (chan=="ee") {
     name="Elec";
     //HTriggerEff_ElecSel_pT
  }
  else if (chan=="emu") {
     name="afterDilept";
     //HTriggerEff_afterDileptSel_pT
  }

  TString histo_Data_name_pt = "HTriggerEff_"+name+"Sel_pT_MET1";
  TH1F* selElectron_pt_data            = (TH1F*)gROOT->FindObject(histo_Data_name_pt);
  TString histo_Data_name_pttrig = "HTriggerEff_"+name+"Sel_selTrig_pT_MET1";
  TH1F* selElectron_ElTrigger_pt_data  = (TH1F*)gROOT->FindObject(histo_Data_name_pttrig);
  								    
  TString histo_Data_name_pt2 = "HTriggerEff_"+name+"Sel_pT_MET2";
  TH1F* selElectron_pt_data2            = (TH1F*)gROOT->FindObject(histo_Data_name_pt2);
  TString histo_Data_name_pttrig2 = "HTriggerEff_"+name+"Sel_selTrig_pT_MET2";
  TH1F* selElectron_ElTrigger_pt_data2  = (TH1F*)gROOT->FindObject(histo_Data_name_pttrig2);

//  TFile *f2 = new TFile("syst_m/TriggerPlots_Zjets.root"); 
//  f2->cd();


//  TString dataset="Zjets";
  TString dataset="TTbarSig";

  TString histo_Zjets_name_pt = "HTriggerEff_"+name+"Sel_pT_"+dataset;
  TH1F* selElectron_pt_Zjets            = (TH1F*)gROOT->FindObject(histo_Zjets_name_pt);
  TString histo_Zjets_name_pttrig = "HTriggerEff_"+name+"Sel_selTrig_pT_"+dataset;
  TH1F* selElectron_ElTrigger_pt_Zjets  = (TH1F*)gROOT->FindObject(histo_Zjets_name_pttrig);
  								    
    // compute the inclusive trigger efficiencies and print it
/*
    float pass_data=selElectron_ElTrigger_pt_data->Integral();
    float total_data=selElectron_pt_data->Integral();
*/
    float pass_data_1=selElectron_ElTrigger_pt_data->GetEntries();
    float total_data_1=selElectron_pt_data->GetEntries();
    float eff_data_1 = pass_data_1/total_data_1;
    float err_data_1 = sqrt(total_data_1*eff_data_1*(1.-eff_data_1))/total_data_1;
    cout << " pass_data " << pass_data_1 << " total_data " << total_data_1 << " eff " << eff_data_1 << 
            " +/- " << err_data_1 << endl; 

    float pass_data_2=selElectron_ElTrigger_pt_data2->GetEntries();
    float total_data_2=selElectron_pt_data2->GetEntries();
    float eff_data_2 = pass_data_2/total_data_2;
    float err_data_2 = sqrt(total_data_2*eff_data_2*(1.-eff_data_2))/total_data_2;
    cout << " pass_data " << pass_data_2 << " total_data " << total_data_2 << " eff " << eff_data_2 << 
            " +/- " << err_data_2 << endl; 

    float lumi1=0.2162+0.9340; //mayrereco + promptv4
    float lumi2=0.3733+0.6672; //augrereco + promptv6
    cout << " lumi " << lumi1 << " " << lumi2 << endl;
    float eff_data = (lumi1*eff_data_1 + lumi2*eff_data_2)/(lumi1+lumi2);
    float err_data = (lumi1*err_data_1 + lumi2*err_data_2)/(lumi1+lumi2);

    float pass_Zjets=selElectron_ElTrigger_pt_Zjets->Integral();
    float total_Zjets=selElectron_pt_Zjets->Integral();
    float eff_Zjets = pass_Zjets/total_Zjets;
    float tt_Zjets=selElectron_pt_Zjets->GetEntries(); // sans poids!
    float err_Zjets = sqrt(tt_Zjets*eff_Zjets*(1.-eff_Zjets))/tt_Zjets;
    
    float SF_data_Zjets= eff_data/eff_Zjets;
    float err_SF = pow( pow(err_data/eff_data, 2.) + pow(err_Zjets*eff_data/(eff_Zjets*eff_Zjets),2) , 0.5 );

    cout << endl;
    cout << " Eff Data     " << eff_data << " +/- " << err_data << endl;
    cout << " Eff " << dataset << " " <<  eff_Zjets << " +/- " << err_Zjets << endl;
    cout << " SF           " << SF_data_Zjets << " +/- " << err_SF << endl;
  
    cout << endl;
}
