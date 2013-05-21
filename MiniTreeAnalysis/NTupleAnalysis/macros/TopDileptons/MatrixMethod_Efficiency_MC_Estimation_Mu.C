#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/Selection.h"
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"
#include "../../Tools/interface/PUWeighting.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>

using namespace TopTree;
using namespace std;

int main ()
{
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;
  
  //////////////////////
  // Initialisation
  //////////////////////
  float Luminosity = 0;
  float LumiError = 0;
  string PUWeightFileName;
  int DataType = 0; 

  string xmlFileName = string ("../../config/MatrixMethod_MC.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  vector < Dataset > datasets;
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  int verbosity = -1;

  DiLeptonSelection sel; 
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, LumiError, PUWeightFileName, verbosity );
  //Load for PU:
  sel.GeneratePUWeight(PUWeightFileName);

  TopTree::NTEvent * event = 0;

  PUWeighting  thePUReweighter;
  TFile* file1  = new TFile(PUWeightFileName.c_str(),"READ"); 
  TH1D *  hPUData = 0;
  hPUData         = (TH1D*)file1->Get("pileup");
  TH1F *  hPUMC   = new TH1F("pileup_MC", "pileup_MC", hPUData->GetXaxis()->GetNbins(), hPUData->GetXaxis()->GetXmin(), hPUData->GetXaxis()->GetXmax() );
  TFile* file2  = new TFile( "../data/CrossSection_pileup.root" ,"READ");
  hPUMC           = (TH1F*)file2->Get("pileup_TTbarSig");
  // histo in data, histo in Mc, use out-of-time pu in the reweighting
  cout << "get MC histo  " << endl;
  thePUReweighter.setPUHisto( hPUData, hPUMC);
  cout << "set MC histo in thePUReweighter " << endl;
  thePUReweighter.setUseOutOfTimePU(false); // set to true to use out-of-time PU

  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;

  float NJets_Min = -0.5;
  float NJets_Max = 10.5;
  int NJets_N_bins = 11;

  float Eta_Min = -3;
  float Eta_Max = 3;
  int Eta_N_bins = 20;

  float PT_Min = 0;
  float PT_Max = 1000;
  int PT_N_bins = 100;


  TH1F * IsoTightForEfficiencyNJets = new TH1F ("IsoTightForEfficiencyNJets","IsoTightForEfficiencyNJets", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * IsoLooseForEfficiencyNJets = new TH1F ("IsoLooseForEfficiencyNJets","IsoLooseForEfficiencyNJets", NJets_N_bins, NJets_Min, NJets_Max);

  TH1F * IsoTightForEfficiencyEta = new TH1F ("IsoTightForEfficiencyEta","IsoTightForEfficiencyEta", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * IsoLooseForEfficiencyEta = new TH1F ("IsoLooseForEfficiencyEta","IsoLooseForEfficiencyEta", Eta_N_bins, Eta_Min, Eta_Max);

  TH1F * IsoTightForEfficiencyPT = new TH1F ("IsoTightForEfficiencyPT","IsoTightForEfficiencyPT", PT_N_bins, PT_Min, PT_Max);
  TH1F * IsoLooseForEfficiencyPT = new TH1F ("IsoLooseForEfficiencyPT","IsoLooseForEfficiencyPT", PT_N_bins, PT_Min, PT_Max);


  TH1F * IsoTightForFakeRateNJets = new TH1F ("IsoTightForFakeRateNJets","IsoTightForFakeRateNJets", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * IsoLooseForFakeRateNJets = new TH1F ("IsoLooseForFakeRateNJets","IsoLooseForFakeRateNJets", NJets_N_bins, NJets_Min, NJets_Max);

  TH1F * IsoTightForFakeRateEta = new TH1F ("IsoTightForFakeRateEta","IsoTightForFakeRateEta", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * IsoLooseForFakeRateEta = new TH1F ("IsoLooseForFakeRateEta","IsoLooseForFakeRateEta", Eta_N_bins, Eta_Min, Eta_Max);

  TH1F * IsoTightForFakeRatePT = new TH1F ("IsoTightForFakeRatePT","IsoTightForFakeRatePT", PT_N_bins, PT_Min, PT_Max);
  TH1F * IsoLooseForFakeRatePT = new TH1F ("IsoLooseForFakeRatePT","IsoLooseForFakeRatePT", PT_N_bins, PT_Min, PT_Max);

  /*
  TH1F * IsoTightForFakeRateNJets_QCD = new TH1F ("IsoTightForFakeRateNJets_QCD","IsoTightForFakeRateNJets_QCD", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * IsoLooseForFakeRateNJets_QCD = new TH1F ("IsoLooseForFakeRateNJets_QCD","IsoLooseForFakeRateNJets_QCD", NJets_N_bins, NJets_Min, NJets_Max);

  TH1F * IsoTightForFakeRateEta_QCD = new TH1F ("IsoTightForFakeRateEta_QCD","IsoTightForFakeRateEta_QCD", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * IsoLooseForFakeRateEta_QCD = new TH1F ("IsoLooseForFakeRateEta_QCD","IsoLooseForFakeRateEta_QCD", Eta_N_bins, Eta_Min, Eta_Max);

  TH1F * IsoTightForFakeRatePT_QCD = new TH1F ("IsoTightForFakeRatePT_QCD","IsoTightForFakeRatePT_QCD", PT_N_bins, PT_Min, PT_Max);
  TH1F * IsoLooseForFakeRatePT_QCD = new TH1F ("IsoLooseForFakeRatePT_QCD","IsoLooseForFakeRatePT_QCD", PT_N_bins, PT_Min, PT_Max);
  */


  TH1F * Unweighted_IsoTightForEfficiencyNJets = new TH1F ("Unweighted_IsoTightForEfficiencyNJets","Unweighted_IsoTightForEfficiencyNJets", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * Unweighted_IsoLooseForEfficiencyNJets = new TH1F ("Unweighted_IsoLooseForEfficiencyNJets","Unweighted_IsoLooseForEfficiencyNJets", NJets_N_bins, NJets_Min, NJets_Max);

  TH1F * Unweighted_IsoTightForEfficiencyEta = new TH1F ("Unweighted_IsoTightForEfficiencyEta","Unweighted_IsoTightForEfficiencyEta", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * Unweighted_IsoLooseForEfficiencyEta = new TH1F ("Unweighted_IsoLooseForEfficiencyEta","Unweighted_IsoLooseForEfficiencyEta", Eta_N_bins, Eta_Min, Eta_Max);

  TH1F * Unweighted_IsoTightForEfficiencyPT = new TH1F ("Unweighted_IsoTightForEfficiencyPT","Unweighted_IsoTightForEfficiencyPT", PT_N_bins, PT_Min, PT_Max);
  TH1F * Unweighted_IsoLooseForEfficiencyPT = new TH1F ("Unweighted_IsoLooseForEfficiencyPT","Unweighted_IsoLooseForEfficiencyPT", PT_N_bins, PT_Min, PT_Max);


  TH1F * Unweighted_IsoTightForFakeRateNJets = new TH1F ("Unweighted_IsoTightForFakeRateNJets","Unweighted_IsoTightForFakeRateNJets", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * Unweighted_IsoLooseForFakeRateNJets = new TH1F ("Unweighted_IsoLooseForFakeRateNJets","Unweighted_IsoLooseForFakeRateNJets", NJets_N_bins, NJets_Min, NJets_Max);

  TH1F * Unweighted_IsoTightForFakeRateEta = new TH1F ("Unweighted_IsoTightForFakeRateEta","Unweighted_IsoTightForFakeRateEta", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * Unweighted_IsoLooseForFakeRateEta = new TH1F ("Unweighted_IsoLooseForFakeRateEta","Unweighted_IsoLooseForFakeRateEta", Eta_N_bins, Eta_Min, Eta_Max);

  TH1F * Unweighted_IsoTightForFakeRatePT = new TH1F ("Unweighted_IsoTightForFakeRatePT","Unweighted_IsoTightForFakeRatePT", PT_N_bins, PT_Min, PT_Max);
  TH1F * Unweighted_IsoLooseForFakeRatePT = new TH1F ("Unweighted_IsoLooseForFakeRatePT","Unweighted_IsoLooseForFakeRatePT", PT_N_bins, PT_Min, PT_Max);


  /*
  TH1F * Unweighted_IsoTightForFakeRateNJets_QCD = new TH1F ("Unweighted_IsoTightForFakeRateNJets_QCD","Unweighted_IsoTightForFakeRateNJets_QCD", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * Unweighted_IsoLooseForFakeRateNJets_QCD = new TH1F ("Unweighted_IsoLooseForFakeRateNJets_QCD","Unweighted_IsoLooseForFakeRateNJets_QCD", NJets_N_bins, NJets_Min, NJets_Max);

  TH1F * Unweighted_IsoTightForFakeRateEta_QCD = new TH1F ("Unweighted_IsoTightForFakeRateEta_QCD","Unweighted_IsoTightForFakeRateEta_QCD", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * Unweighted_IsoLooseForFakeRateEta_QCD = new TH1F ("Unweighted_IsoLooseForFakeRateEta_QCD","Unweighted_IsoLooseForFakeRateEta_QCD", Eta_N_bins, Eta_Min, Eta_Max);

  TH1F * Unweighted_IsoTightForFakeRatePT_QCD = new TH1F ("Unweighted_IsoTightForFakeRatePT_QCD","Unweighted_IsoTightForFakeRatePT_QCD", PT_N_bins, PT_Min, PT_Max);
  TH1F * Unweighted_IsoLooseForFakeRatePT_QCD = new TH1F ("Unweighted_IsoLooseForFakeRatePT_QCD","Unweighted_IsoLooseForFakeRatePT_QCD", PT_N_bins, PT_Min, PT_Max);
  */



  TH1F * SignalEfficiencyNJets = new TH1F("SignalEfficiencyNJets", "SignalEfficiencyNJets", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * SignalEfficiencyEta = new TH1F("SignalEfficiencyEta", "SignalEfficiencyEta", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * SignalEfficiencyPT = new TH1F("SignalEfficiencyPT", "SignalEfficiencyPT", PT_N_bins, PT_Min, PT_Max);

  TH1F * FakeRateNJets = new TH1F("FakeRateNJets", "FakeRateNJets", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * FakeRateEta = new TH1F("FakeRateEta", "FakeRateEta", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * FakeRatePT = new TH1F("FakeRatePT", "FakeRatePT", PT_N_bins, PT_Min, PT_Max);
  /*
  TH1F * FakeRateNJets_QCD = new TH1F("FakeRateNJets_QCD", "FakeRateNJets_QCD", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * FakeRateEta_QCD = new TH1F("FakeRateEta_QCD", "FakeRateEta_QCD", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * FakeRatePT_QCD = new TH1F("FakeRatePT_QCD", "FakeRatePT_QCD", PT_N_bins, PT_Min, PT_Max);
  */

  DiLeptonSelection sel_Loose(sel);
  DiLeptonSelection sel_Tight(sel);

  for (unsigned int d = 0; d < datasets.size (); d++) {
   TRandom randomize;
   datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
   cout << "dataset = " << datasets[d].Name() << endl;
   unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());

    cout << "NEvents = " << nEvents << endl;
    float weight_init;
    weight_init = datasets[d].NormFactor();

    //LOOP OVER THE EVENTS

    for (unsigned int ievt = 0; ievt < nEvents; ievt++){
      datasets[d].eventTree ()->GetEntry (ievt);
      //Load event for the selection
      sel.LoadEvent(event);
      sel_Loose.LoadEvent(event);
      sel_Tight.LoadEvent(event);

      if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;

      vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
      vector<NTVertex>   selVertices_Loose  = sel_Loose.GetSelectedVertex();
      vector<NTVertex>   selVertices_Tight  = sel_Tight.GetSelectedVertex();
      //float weight = weight_init*sel.GetPUWeight();
      float weight = 0;
      if(thePUReweighter.getUseOutOfTimePU()){
         weight = weight_init*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);
      }else{
         weight = weight_init*thePUReweighter.weight(event->num_pileup_bc0);
      }

       std::vector<NTMuon> muons =  sel.GetSelectedMuonsNoIso();

       string channel = "mumu";
       vector<NTElectron> candElec_Loose;
       vector<NTMuon> candMuon_Loose;
       vector<NTElectron> candElec_Tight;
       vector<NTMuon> candMuon_Tight;
       sel_Loose.GetLeptonPairForMM(0.8, 0.8, candMuon_Loose, candElec_Loose, channel);
       sel_Tight.GetLeptonPairForMM(0.2, 0.2, candMuon_Tight, candElec_Tight, channel);
       vector<NTJet> jets_Loose;
       vector<NTJet> jets_Tight;
       jets_Loose = sel_Loose.GetSelectedJets(candMuon_Loose, candElec_Loose);
       jets_Tight = sel_Tight.GetSelectedJets(candMuon_Tight, candElec_Tight);

       unsigned int loose_iso_counter_eff = 0;
       unsigned int tight_iso_counter_eff = 0;

       unsigned int loose_iso_counter_fake = 0;
       unsigned int tight_iso_counter_fake = 0;

       if(/*(datasets[d].Name() == "TTbar" && (*/event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000/*)) ||*/ //mumu
          /*datasets[d].Name() == "Zjets"*/){

       if(muons.size() == 2 && sel.passTriggerSelection ( &datasets[d], "mumu")){
        for(unsigned int i=0; i<muons.size(); i++){
	  if(muons[i].RelIso03PF() < 0.8) loose_iso_counter_eff += 1;
	  if(muons[i].RelIso03PF() < 0.2) tight_iso_counter_eff += 1;
	}
       }

       if(loose_iso_counter_eff == 2){
                  IsoLooseForEfficiencyNJets->Fill(jets_Loose.size(),weight);
                  IsoLooseForEfficiencyEta->Fill(muons[0].p4.Eta(),weight);
                  IsoLooseForEfficiencyPT->Fill(muons[0].p4.Perp(),weight);
                  Unweighted_IsoLooseForEfficiencyNJets->Fill(jets_Loose.size());
                  Unweighted_IsoLooseForEfficiencyEta->Fill(muons[0].p4.Eta());
                  Unweighted_IsoLooseForEfficiencyPT->Fill(muons[0].p4.Perp());

       }
       if(tight_iso_counter_eff == 2){
                  IsoTightForEfficiencyNJets->Fill(jets_Tight.size() , weight);
                  IsoTightForEfficiencyEta->Fill(muons[0].p4.Eta(),weight);
                  IsoTightForEfficiencyPT->Fill(muons[0].p4.Perp(),weight);
                  Unweighted_IsoTightForEfficiencyNJets->Fill(jets_Tight.size());
                  Unweighted_IsoTightForEfficiencyEta->Fill(muons[0].p4.Eta());
                  Unweighted_IsoTightForEfficiencyPT->Fill(muons[0].p4.Perp());
       }
       }


       if(/*(datasets[d].Name() == "TTbar" && (*//*event->TMEME == 1 ||*/ event->TMEME == 10 || // l(=mu)jets
                                             event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
                                             event->TMEME == 10010 || /*event->TMEME == 10001 ||*/ // l(=mu)tau(->had)
                                       	   /*event->TMEME == 10100 ||*/ event->TMEME == 11000/*)) ||*/ // tau(->e/mu)jets
	  /*datasets[d].Name() == "Wjets"*/){

       if(muons.size() == 2 && sel.passTriggerSelection ( &datasets[d], "mumu")){
        for(unsigned int i=0; i<muons.size(); i++){
	  if(muons[i].RelIso03PF() < 0.8) loose_iso_counter_fake += 1;
	  if(muons[i].RelIso03PF() < 0.2) tight_iso_counter_fake += 1;
	}
       }

       if(loose_iso_counter_fake == 2){
                  IsoLooseForFakeRateNJets->Fill(jets_Loose.size(),weight);
                  IsoLooseForFakeRateEta->Fill(muons[0].p4.Eta(),weight);
                  IsoLooseForFakeRatePT->Fill(muons[0].p4.Perp(),weight);
                  Unweighted_IsoLooseForFakeRateNJets->Fill(jets_Loose.size());
                  Unweighted_IsoLooseForFakeRateEta->Fill(muons[0].p4.Eta());
                  Unweighted_IsoLooseForFakeRatePT->Fill(muons[0].p4.Perp());
       }
       if(tight_iso_counter_fake == 2){
                  IsoTightForFakeRateNJets->Fill(jets_Tight.size(),weight);
                  IsoTightForFakeRateEta->Fill(muons[0].p4.Eta(),weight);
                  IsoTightForFakeRatePT->Fill(muons[0].p4.Perp(),weight);
                  Unweighted_IsoTightForFakeRateNJets->Fill(jets_Tight.size());
                  Unweighted_IsoTightForFakeRateEta->Fill(muons[0].p4.Eta());
                  Unweighted_IsoTightForFakeRatePT->Fill(muons[0].p4.Perp());
       }
       }



       /*
       if(datasets[d].Name() == "QCD1" || datasets[d].Name() == "QCD2" || datasets[d].Name() == "QCD3" || datasets[d].Name() == "QCD4" || datasets[d].Name() == "QCD5" ||
          datasets[d].Name() == "QCD6" || datasets[d].Name() == "QCD7" || datasets[d].Name() == "QCD8" || datasets[d].Name() == "QCD9" || datasets[d].Name() == "QCD10" ||
          datasets[d].Name() == "QCD11" || datasets[d].Name() == "QCD12" || datasets[d].Name() == "QCD13"){

       if(muons.size() == 2 && sel.passTriggerSelection ( &datasets[d], "mumu")){
        for(unsigned int i=0; i<muons.size(); i++){
	  if(muons[i].RelIso03PF() < 0.8) loose_iso_counter_fake += 1;
	  if(muons[i].RelIso03PF() < 0.2) tight_iso_counter_fake += 1;
	}
       }

       if(loose_iso_counter_fake == 2){
                  IsoLooseForFakeRateNJets_QCD->Fill(jets_Loose.size(),weight);
                  IsoLooseForFakeRateEta_QCD->Fill(muons[0].p4.Eta(),weight);
                  IsoLooseForFakeRatePT_QCD->Fill(muons[0].p4.Perp(),weight);
                  Unweighted_IsoLooseForFakeRateNJets_QCD->Fill(jets_Loose.size());
                  Unweighted_IsoLooseForFakeRateEta_QCD->Fill(muons[0].p4.Eta());
                  Unweighted_IsoLooseForFakeRatePT_QCD->Fill(muons[0].p4.Perp());
       }
       if(tight_iso_counter_fake == 2){
                  IsoTightForFakeRateNJets_QCD->Fill(jets_Tight.size(),weight);
                  IsoTightForFakeRateEta_QCD->Fill(muons[0].p4.Eta(),weight);
                  IsoTightForFakeRatePT_QCD->Fill(muons[0].p4.Perp(),weight);
                  Unweighted_IsoTightForFakeRateNJets_QCD->Fill(jets_Tight.size());
                  Unweighted_IsoTightForFakeRateEta_QCD->Fill(muons[0].p4.Eta());
                  Unweighted_IsoTightForFakeRatePT_QCD->Fill(muons[0].p4.Perp());
       }
       }
       */





    } // end of loop over evts







  } // end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;

  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

  TFile* file_MM = new TFile("MatrixMethod_Efficiency_MC_Mu.root","RECREATE");
  file_MM->cd();

  for(unsigned int i=0; i<NJets_N_bins; i++){
     float num_s_w = (float)IsoTightForEfficiencyNJets->GetBinContent(i+1);
     float den_s_w = (float)IsoLooseForEfficiencyNJets->GetBinContent(i+1);
     float num_s = (float)Unweighted_IsoTightForEfficiencyNJets->GetBinContent(i+1);
     float den_s = (float)Unweighted_IsoLooseForEfficiencyNJets->GetBinContent(i+1);
     float eff_s = 0.;
     float err_s = 0.;
     if(den_s != 0. && den_s_w !=0.) {
      eff_s = sqrt(num_s_w/den_s_w);
      err_s = (0.5)*sqrt((1-(num_s/den_s))/den_s);
     }
     SignalEfficiencyNJets->SetBinContent(i+1, eff_s);
     SignalEfficiencyNJets->SetBinError(i+1, err_s);
  }


  for(unsigned int i=0; i<NJets_N_bins; i++){
     float num_s_w = (float)IsoTightForEfficiencyNJets->GetBinContent(i+1);
     float den_s_w = (float)IsoLooseForEfficiencyNJets->GetBinContent(i+1);
     float num_f_w = (float)IsoTightForFakeRateNJets->GetBinContent(i+1);
     float den_f_w = (float)IsoLooseForFakeRateNJets->GetBinContent(i+1);
     float num_s = (float)Unweighted_IsoTightForEfficiencyNJets->GetBinContent(i+1);
     float den_s = (float)Unweighted_IsoLooseForEfficiencyNJets->GetBinContent(i+1);
     float num_f = (float)Unweighted_IsoTightForFakeRateNJets->GetBinContent(i+1);
     float den_f = (float)Unweighted_IsoLooseForFakeRateNJets->GetBinContent(i+1);
     float eff_f = 0.;
     float err_f = 0.;
     if(num_s != 0. && den_f != 0. && den_s !=0. && den_s_w != 0. && den_f_w != 0.) {
      eff_f = (num_f_w/den_f_w)/sqrt(num_s_w/den_s_w);
      err_f = (sqrt(den_s/num_s)*sqrt(num_f*(1-(num_f/den_f)))/den_f) + (((num_f*den_s)/(den_f*num_s))*0.5*sqrt((1-(num_s/den_s))/den_s));
     }
     FakeRateNJets->SetBinContent(i+1, eff_f);
     FakeRateNJets->SetBinError(i+1, err_f);
  }

  /*
  for(unsigned int i=0; i<NJets_N_bins; i++){
     float num_f_w = (float)IsoTightForFakeRateNJets_QCD->GetBinContent(i+1);
     float den_f_w = (float)IsoLooseForFakeRateNJets_QCD->GetBinContent(i+1);
     float num_f = (float)Unweighted_IsoTightForFakeRateNJets_QCD->GetBinContent(i+1);
     float den_f = (float)Unweighted_IsoLooseForFakeRateNJets_QCD->GetBinContent(i+1);
     float eff_f = 0.;
     float err_f = 0.;
     if(den_f != 0. && den_f_w !=0.) {
      eff_f = sqrt(num_f_w/den_f_w);
      err_f = (0.5)*((sqrt(num_f)+sqrt(den_f))/den_f);
     }
     FakeRateNJets_QCD->SetBinContent(i+1, eff_f);
     FakeRateNJets_QCD->SetBinError(i+1, err_f);
  }
  */
  for(unsigned int i=0; i<Eta_N_bins; i++){
     float num_s_w = (float)IsoTightForEfficiencyEta->GetBinContent(i+1);
     float den_s_w = (float)IsoLooseForEfficiencyEta->GetBinContent(i+1);
     float num_s = (float)Unweighted_IsoTightForEfficiencyEta->GetBinContent(i+1);
     float den_s = (float)Unweighted_IsoLooseForEfficiencyEta->GetBinContent(i+1);
     float eff_s = 0.;
     float err_s = 0.;
     if(den_s != 0. && den_s_w !=0.) {
      eff_s = sqrt(num_s_w/den_s_w);
      err_s = (0.5)*sqrt((1-(num_s/den_s))/den_s);
     }
     SignalEfficiencyEta->SetBinContent(i+1, eff_s);
     SignalEfficiencyEta->SetBinError(i+1, err_s);
  }


  for(unsigned int i=0; i<Eta_N_bins; i++){
     float num_s_w = (float)IsoTightForEfficiencyEta->GetBinContent(i+1);
     float den_s_w = (float)IsoLooseForEfficiencyEta->GetBinContent(i+1);
     float num_f_w = (float)IsoTightForFakeRateEta->GetBinContent(i+1);
     float den_f_w = (float)IsoLooseForFakeRateEta->GetBinContent(i+1);
     float num_s = (float)Unweighted_IsoTightForEfficiencyEta->GetBinContent(i+1);
     float den_s = (float)Unweighted_IsoLooseForEfficiencyEta->GetBinContent(i+1);
     float num_f = (float)Unweighted_IsoTightForFakeRateEta->GetBinContent(i+1);
     float den_f = (float)Unweighted_IsoLooseForFakeRateEta->GetBinContent(i+1);
     float eff_f = 0.;
     float err_f = 0.;
     if(num_s != 0. && den_f != 0. && den_s !=0. && den_s_w != 0. && den_f_w != 0.) {
      eff_f = (num_f_w/den_f_w)/sqrt(num_s_w/den_s_w);
      err_f = (sqrt(den_s/num_s)*sqrt(num_f*(1-(num_f/den_f)))/den_f) + (((num_f*den_s)/(den_f*num_s))*0.5*sqrt((1-(num_s/den_s))/den_s));
     }
     FakeRateEta->SetBinContent(i+1, eff_f);
     FakeRateEta->SetBinError(i+1, err_f);
  }
  /*
  for(unsigned int i=0; i<NJets_N_bins; i++){
     float num_f_w = (float)IsoTightForFakeRateEta_QCD->GetBinContent(i+1);
     float den_f_w = (float)IsoLooseForFakeRateEta_QCD->GetBinContent(i+1);
     float num_f = (float)Unweighted_IsoTightForFakeRateEta_QCD->GetBinContent(i+1);
     float den_f = (float)Unweighted_IsoLooseForFakeRateEta_QCD->GetBinContent(i+1);
     float eff_f = 0.;
     float err_f = 0.;
     if(den_f != 0. && den_f_w !=0.) {
      eff_f = sqrt(num_f_w/den_f_w);
      err_f = (0.5)*((sqrt(num_f)+sqrt(den_f))/den_f);
     }
     FakeRateEta_QCD->SetBinContent(i+1, eff_f);
     FakeRateEta_QCD->SetBinError(i+1, err_f);
  }
  */

  for(unsigned int i=0; i<PT_N_bins; i++){
     float num_s_w = (float)IsoTightForEfficiencyPT->GetBinContent(i+1);
     float den_s_w = (float)IsoLooseForEfficiencyPT->GetBinContent(i+1);
     float num_s = (float)Unweighted_IsoTightForEfficiencyPT->GetBinContent(i+1);
     float den_s = (float)Unweighted_IsoLooseForEfficiencyPT->GetBinContent(i+1);
     float eff_s = 0.;
     float err_s = 0.;
     if(den_s != 0. && den_s_w !=0.) {
      eff_s = sqrt(num_s_w/den_s_w);
      err_s = (0.5)*sqrt((1-(num_s/den_s))/den_s);
     }
     SignalEfficiencyPT->SetBinContent(i+1, eff_s);
     SignalEfficiencyPT->SetBinError(i+1, err_s);
  }


  for(unsigned int i=0; i<PT_N_bins; i++){
     float num_s_w = (float)IsoTightForEfficiencyPT->GetBinContent(i+1);
     float den_s_w = (float)IsoLooseForEfficiencyPT->GetBinContent(i+1);
     float num_f_w = (float)IsoTightForFakeRatePT->GetBinContent(i+1);
     float den_f_w = (float)IsoLooseForFakeRatePT->GetBinContent(i+1);
     float num_s = (float)Unweighted_IsoTightForEfficiencyPT->GetBinContent(i+1);
     float den_s = (float)Unweighted_IsoLooseForEfficiencyPT->GetBinContent(i+1);
     float num_f = (float)Unweighted_IsoTightForFakeRatePT->GetBinContent(i+1);
     float den_f = (float)Unweighted_IsoLooseForFakeRatePT->GetBinContent(i+1);
     float eff_f = 0.;
     float err_f = 0.;
     if(num_s != 0. && den_f != 0. && den_s !=0. && den_s_w != 0. && den_f_w != 0.) {
      eff_f = (num_f_w/den_f_w)/sqrt(num_s_w/den_s_w);
      err_f = (sqrt(den_s/num_s)*sqrt(num_f*(1-(num_f/den_f)))/den_f) + (((num_f*den_s)/(den_f*num_s))*0.5*sqrt((1-(num_s/den_s))/den_s));
     }
     FakeRatePT->SetBinContent(i+1, eff_f);
     FakeRatePT->SetBinError(i+1, err_f);
  }

  /*
  for(unsigned int i=0; i<NJets_N_bins; i++){
     float num_f_w = (float)IsoTightForFakeRatePT_QCD->GetBinContent(i+1);
     float den_f_w = (float)IsoLooseForFakeRatePT_QCD->GetBinContent(i+1);
     float num_f = (float)Unweighted_IsoTightForFakeRatePT_QCD->GetBinContent(i+1);
     float den_f = (float)Unweighted_IsoLooseForFakeRatePT_QCD->GetBinContent(i+1);
     float eff_f = 0.;
     float err_f = 0.;
     if(den_f != 0. && den_f_w !=0.) {
      eff_f = sqrt(num_f_w/den_f_w);
      err_f = (0.5)*((sqrt(num_f)+sqrt(den_f))/den_f);
     }
     FakeRatePT_QCD->SetBinContent(i+1, eff_f);
     FakeRatePT_QCD->SetBinError(i+1, err_f);
  }
  */






  cout << "\\begin{table}" << std::endl;
  cout << "\\begin{center}" << std::endl;
  cout << "\\begin{tabular}{|";
  for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
   cout << " & Njets = " << bin_index;
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{s}$ muon case";
  for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
   cout << " & " << SignalEfficiencyNJets->GetBinContent(bin_index) << "$\\pm$" << SignalEfficiencyNJets->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;



  cout << "\\begin{table}" << std::endl;
  cout << "\\begin{center}" << std::endl;
  cout << "\\begin{tabular}{|";
  for(unsigned int bin_index=0; bin_index<Eta_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<Eta_N_bins; bin_index++){
   cout << " & $\\eta$ = " << ((Eta_Max-Eta_Min)/Eta_N_bins)*(bin_index+0.5-Eta_N_bins*0.5);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{s}$ muon case";
  for(unsigned int bin_index=0; bin_index<Eta_N_bins; bin_index++){
   cout << " & " << SignalEfficiencyEta->GetBinContent(bin_index) << "$\\pm$" << SignalEfficiencyEta->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;



  cout << "\\begin{table}" << std::endl;
  cout << "\\begin{center}" << std::endl;
  cout << "\\begin{tabular}{|";
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " & $p_T$ = " <<   ((PT_Max-PT_Min)/PT_N_bins)*(bin_index+0.5);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{s}$ muon case";
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " & " << SignalEfficiencyPT->GetBinContent(bin_index) << "$\\pm$" << SignalEfficiencyPT->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;




  cout << "\\begin{table}" << std::endl;
  cout << "\\begin{center}" << std::endl;
  cout << "\\begin{tabular}{|";
  for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
   cout << " & Njets = " << bin_index;
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{f}$ muon case";
  for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
   cout << " & " << FakeRateNJets->GetBinContent(bin_index) << "$\\pm$" << FakeRateNJets->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;



  cout << "\\begin{table}" << std::endl;
  cout << "\\begin{center}" << std::endl;
  cout << "\\begin{tabular}{|";
  for(unsigned int bin_index=0; bin_index<Eta_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<Eta_N_bins; bin_index++){
   cout << " & $\\eta$ = " << ((Eta_Max-Eta_Min)/Eta_N_bins)*(bin_index+0.5-Eta_N_bins*0.5);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{f}$ muon case";
  for(unsigned int bin_index=0; bin_index<Eta_N_bins; bin_index++){
   cout << " & " << FakeRateEta->GetBinContent(bin_index) << "$\\pm$" << FakeRateEta->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;



  cout << "\\begin{table}" << std::endl;
  cout << "\\begin{center}" << std::endl;
  cout << "\\begin{tabular}{|";
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " & $p_T$ = " << ((PT_Max-PT_Min)/PT_N_bins)*(bin_index+0.5);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{f}$ muon case";
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " & " << FakeRatePT->GetBinContent(bin_index) << "$\\pm$" << FakeRatePT->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;








  SignalEfficiencyNJets->Write();
  SignalEfficiencyNJets = 0;
  delete SignalEfficiencyNJets;
  FakeRateNJets->Write();
  FakeRateNJets = 0;
  delete FakeRateNJets;
  //  FakeRateNJets_QCD->Write();
  //  FakeRateNJets_QCD = 0;
  //  delete FakeRateNJets_QCD;
  SignalEfficiencyEta->Write();
  SignalEfficiencyEta = 0;
  delete SignalEfficiencyEta;
  FakeRateEta->Write();
  FakeRateEta = 0;
  delete FakeRateEta;
  //  FakeRateEta_QCD->Write();
  //  FakeRateEta_QCD = 0;
  //  delete FakeRateEta_QCD;
  SignalEfficiencyPT->Write();
  SignalEfficiencyPT = 0;
  delete SignalEfficiencyPT;
  FakeRatePT->Write();
  FakeRatePT = 0;
  delete FakeRatePT;
  //  FakeRatePT_QCD->Write();
  //  FakeRatePT_QCD = 0;
  //  delete FakeRatePT_QCD;


  file_MM->Write();
  file_MM->Close();
  delete file_MM;






  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
