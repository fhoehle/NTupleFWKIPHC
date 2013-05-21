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

  string xmlFileName = string ("../../config/MatrixMethod_DATA_E_forEfficiency.xml");
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

  float NVTX_Min = -0.5;
  float NVTX_Max = 30.5;
  int NVTX_N_bins = 31;

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

  TH1F * IsoLooseForEfficiencyNVTX = new TH1F ("IsoLooseForEfficiencyNVTX","IsoLooseForEfficiencyNVTX", NVTX_N_bins, NVTX_Min, NVTX_Max);
  TH1F * IsoTightForEfficiencyNVTX = new TH1F ("IsoTightForEfficiencyNVTX","IsoTightForEfficiencyNVTX", NVTX_N_bins, NVTX_Min, NVTX_Max);

  TH1F * SignalEfficiencyNJets = new TH1F("SignalEfficiencyNJets", "SignalEfficiencyNJets", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * SignalEfficiencyNJetsSyst = new TH1F("SignalEfficiencyNJetsSyst", "SignalEfficiencyNJetsSyst", NJets_N_bins, NJets_Min, NJets_Max);
  TH1F * SignalEfficiencyEta = new TH1F("SignalEfficiencyEta", "SignalEfficiencyEta", Eta_N_bins, Eta_Min, Eta_Max);
  TH1F * SignalEfficiencyPT = new TH1F("SignalEfficiencyPT", "SignalEfficiencyPT", PT_N_bins, PT_Min, PT_Max);
  TH1F * SignalEfficiencyNVTX = new TH1F ("SignalEfficiencyNVTX","SignalEfficiencyNVTX", NVTX_N_bins, NVTX_Min, NVTX_Max);
  TH1F * SignalEfficiencyNJets_MC = new TH1F("SignalEfficiencyNJets_MC", "SignalEfficiencyNJets_MC", NJets_N_bins, NJets_Min, NJets_Max);


  TH1F * IsoTightForFakeRateNJets[10];
  TH1F * IsoLooseForFakeRateNJets[10];

  TH1F * IsoTightForFakeRateEta[10];
  TH1F * IsoLooseForFakeRateEta[10];

  TH1F * IsoTightForFakeRatePT[10];
  TH1F * IsoLooseForFakeRatePT[10];

  TH1F * IsoTightForFakeRateNVTX[10];
  TH1F * IsoLooseForFakeRateNVTX[10];


  TH1F * IsoTightForFakeRateNJets_Corr[10];
  TH1F * IsoLooseForFakeRateNJets_Corr[10];

  TH1F * IsoTightForFakeRateEta_Corr[10];
  TH1F * IsoLooseForFakeRateEta_Corr[10];

  TH1F * IsoTightForFakeRatePT_Corr[10];
  TH1F * IsoLooseForFakeRatePT_Corr[10];

  TH1F * IsoTightForFakeRateNVTX_Corr[10];
  TH1F * IsoLooseForFakeRateNVTX_Corr[10];

  TH1F * FakeRateNJets[10];
  TH1F * FakeRateNJetsSyst[10];
  TH1F * FakeRateEta[10];
  TH1F * FakeRatePT[10];
  TH1F * FakeRateNVTX[10];
  TH1F * FakeRateNJets_MC = new TH1F("FakeRateNJets_MC", "FakeRateNJets_MC", NJets_N_bins, NJets_Min, NJets_Max);

  string threshold[10] = {"", "_10", "_20", "_30", "_40","_50","_60","_70","_80","_90"};

  for (unsigned int i = 0; i < 10; i++){
    IsoTightForFakeRateNJets[i] = new TH1F (("IsoTightForFakeRateNJets"+threshold[i]).c_str(),("IsoTightForFakeRateNJets"+threshold[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    IsoLooseForFakeRateNJets[i] = new TH1F (("IsoLooseForFakeRateNJets"+threshold[i]).c_str(),("IsoLooseForFakeRateNJets"+threshold[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);

    IsoTightForFakeRateEta[i] = new TH1F (("IsoTightForFakeRateEta"+threshold[i]).c_str(),("IsoTightForFakeRateEta"+threshold[i]).c_str(), Eta_N_bins, Eta_Min, Eta_Max);
    IsoLooseForFakeRateEta[i] = new TH1F (("IsoLooseForFakeRateEta"+threshold[i]).c_str(),("IsoLooseForFakeRateEta"+threshold[i]).c_str(), Eta_N_bins, Eta_Min, Eta_Max);

    IsoTightForFakeRatePT[i] = new TH1F (("IsoTightForFakeRatePT"+threshold[i]).c_str(),("IsoTightForFakeRatePT"+threshold[i]).c_str(), PT_N_bins, PT_Min, PT_Max);
    IsoLooseForFakeRatePT[i] = new TH1F (("IsoLooseForFakeRatePT"+threshold[i]).c_str(),("IsoLooseForFakeRatePT"+threshold[i]).c_str(), PT_N_bins, PT_Min, PT_Max);

    IsoTightForFakeRateNVTX[i] = new TH1F (("IsoTightForFakeRateNVTX"+threshold[i]).c_str(),("IsoTightForFakeRateNVTX"+threshold[i]).c_str(), NVTX_N_bins, NVTX_Min, NVTX_Max);
    IsoLooseForFakeRateNVTX[i] = new TH1F (("IsoLooseForFakeRateNVTX"+threshold[i]).c_str(),("IsoLooseForFakeRateNVTX"+threshold[i]).c_str(), NVTX_N_bins, NVTX_Min, NVTX_Max);


    IsoTightForFakeRateNJets_Corr[i] = new TH1F (("IsoTightForFakeRateNJets_Corr"+threshold[i]).c_str(),("IsoTightForFakeRateNJets_Corr"+threshold[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    IsoLooseForFakeRateNJets_Corr[i] = new TH1F (("IsoLooseForFakeRateNJets_Corr"+threshold[i]).c_str(),("IsoLooseForFakeRateNJets_Corr"+threshold[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);

    IsoTightForFakeRateEta_Corr[i] = new TH1F (("IsoTightForFakeRateEta_Corr"+threshold[i]).c_str(),("IsoTightForFakeRateEta_Corr"+threshold[i]).c_str(), Eta_N_bins, Eta_Min, Eta_Max);
    IsoLooseForFakeRateEta_Corr[i] = new TH1F (("IsoLooseForFakeRateEta_Corr"+threshold[i]).c_str(),("IsoLooseForFakeRateEta_Corr"+threshold[i]).c_str(), Eta_N_bins, Eta_Min, Eta_Max);

    IsoTightForFakeRatePT_Corr[i] = new TH1F (("IsoTightForFakeRatePT_Corr"+threshold[i]).c_str(),("IsoTightForFakeRatePT_Corr"+threshold[i]).c_str(), PT_N_bins, PT_Min, PT_Max);
    IsoLooseForFakeRatePT_Corr[i] = new TH1F (("IsoLooseForFakeRatePT_Corr"+threshold[i]).c_str(),("IsoLooseForFakeRatePT_Corr"+threshold[i]).c_str(), PT_N_bins, PT_Min, PT_Max);

    IsoTightForFakeRateNVTX_Corr[i] = new TH1F (("IsoTightForFakeRateNVTX_Corr"+threshold[i]).c_str(),("IsoTightForFakeRateNVTX_Corr"+threshold[i]).c_str(), NVTX_N_bins, NVTX_Min, NVTX_Max);
    IsoLooseForFakeRateNVTX_Corr[i] = new TH1F (("IsoLooseForFakeRateNVTX_Corr"+threshold[i]).c_str(),("IsoLooseForFakeRateNVTX_Corr"+threshold[i]).c_str(), NVTX_N_bins, NVTX_Min, NVTX_Max);

    FakeRateNJets[i] = new TH1F(("FakeRateNJets"+threshold[i]).c_str(), ("FakeRateNJets"+threshold[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    FakeRateEta[i] = new TH1F(("FakeRateEta"+threshold[i]).c_str(), ("FakeRateEta"+threshold[i]).c_str(), Eta_N_bins, Eta_Min, Eta_Max);
    FakeRatePT[i] = new TH1F(("FakeRatePT"+threshold[i]).c_str(), ("FakeRatePT"+threshold[i]).c_str(), PT_N_bins, PT_Min, PT_Max);
    FakeRateNVTX[i] = new TH1F(("FakeRateNVTX"+threshold[i]).c_str(), ("FakeRateNVTX"+threshold[i]).c_str(), NVTX_N_bins, NVTX_Min, NVTX_Max);
    FakeRateNJetsSyst[i] = new TH1F(("FakeRateNJetsSyst"+threshold[i]).c_str(), ("FakeRateNJetsSyst"+threshold[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
  }


  //  TH1F * FakeRateEta_MC = new TH1F("FakeRateEta_MC", "FakeRateEta_MC", Eta_N_bins, Eta_Min, Eta_Max);
  //  TH1F * FakeRatePT_MC = new TH1F("FakeRatePT_MC", "FakeRatePT_MC", PT_N_bins, PT_Min, PT_Max);

  TH1F * Iso_DATA_Jet = new TH1F("Iso_DATA_Jet", "Iso_DATA_Jet", 30000, -10, 20);
  TH1F * PT_Leading_Jet_DATA_Jet = new TH1F("PT_Leading_Jet_DATA_Jet", "PT_Leading_Jet_DATA_Jet", PT_N_bins, PT_Min, PT_Max);
  TH1F * PT_Inclusive_Jet_DATA_Jet = new TH1F("PT_Inclusive_Jet_DATA_Jet", "PT_Inclusive_Jet_DATA_Jet", PT_N_bins, PT_Min, PT_Max);

  DiLeptonSelection sel_Loose(sel);
  DiLeptonSelection sel_Tight(sel);

  for (unsigned int d = 0; d < datasets.size (); d++) {
   TRandom randomize;
   datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
   cout << "dataset = " << datasets[d].Name() << endl;
   unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());

    cout << "NEvents = " << nEvents << endl;

    float weight_init;
    weight_init = datasets[d].NormFactor()*Luminosity;

    //LOOP OVER THE EVENTS

    for (unsigned int ievt = 0; ievt < nEvents; ievt++){
      datasets[d].eventTree ()->GetEntry (ievt);
      //Load event for the selection
      sel.LoadEvent(event);
      sel_Loose.LoadEvent(event);
      sel_Tight.LoadEvent(event);

      if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;

      //Manage DY samples to avoid overlaps
      double dileptInvMass = 0;
      if( (event->zAndDecays).size() > 0){
        TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen; 
        dileptInvMass = dilept.M();
      }
      if(datasets[d].Name()=="Zjets" && dileptInvMass < 50) continue;
      if(datasets[d].Name()=="DYToMuMu_M-20"      && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToEE_M-20"        && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToTauTau_M-20"    && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
      if(datasets[d].Name()=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToEE_M-10To20"    &&  dileptInvMass > 20) continue;
      if(datasets[d].Name()=="DYToTauTau_M-10To20" &&  dileptInvMass > 20) continue;
   
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




       std::vector<NTElectron> electrons =  sel.GetSelectedElectronsNoIso();
       vector<NTJet> jets = sel.GetJets();
       unsigned int jets_size[10];
       for (unsigned int k = 0; k < 10; k++) {
	 jets_size[k]=0;
       }

       for (unsigned int k = 0; k < 10; k++) {
        for (unsigned int j = 0; j < jets.size(); j++) {
	  if(jets[j].p4.Perp() > 10*k) jets_size[k]++;
        }
       }

       string channel = "ee";
       vector<NTElectron> candElec_Loose;
       vector<NTMuon> candMuon_Loose;
       vector<NTElectron> candElec_Tight;
       vector<NTMuon> candMuon_Tight;

       sel_Loose.GetLeptonPairForMM(0.8, 0.8, candMuon_Loose, candElec_Loose, channel);
       sel_Tight.GetLeptonPairForMM(0.17, 0.17, candMuon_Tight, candElec_Tight, channel);
 
       vector<NTJet> jets_Loose;
       vector<NTJet> jets_Tight;
       jets_Loose = sel_Loose.GetSelectedJets(candMuon_Loose, candElec_Loose);
       jets_Tight = sel_Tight.GetSelectedJets(candMuon_Tight, candElec_Tight);

       NTMET met = sel.GetMET();


       if(datasets[d].Name() == "Jet"){
        for (unsigned int j = 0; j < electrons.size(); j++) {
           Iso_DATA_Jet->Fill(electrons[j].RelIso03PF());
        }
        for (unsigned int j = 0; j < jets.size(); j++) {
	  PT_Inclusive_Jet_DATA_Jet->Fill(jets[j].p4.Perp());
	}
	if(jets.size() >= 1) PT_Leading_Jet_DATA_Jet->Fill(jets[0].p4.Perp());
       }


       bool ainvmasscutW = true;
       if(electrons.size() == 1){
          float phi_l = electrons[0].p4.Phi();
          float pT_l = electrons[0].p4.Perp();
          float phi_met = met.p4.Phi();
          float deltaphi = phi_l - phi_met;
          if(deltaphi < -1*M_PI ) deltaphi += 2*M_PI;
          if(deltaphi > M_PI    ) deltaphi -= 2*M_PI;
          float pT_met = met.p4.Perp();
          float mT = sqrt(2*pT_l*pT_met*(1-cos(deltaphi)));
	  //          if(mT < 80 && mT > 65) ainvmasscutW = false;
          if(mT > 20) ainvmasscutW = false;
       }



       for (unsigned int i = 0; i < 10; i++) {
        if(electrons.size() == 1 && met.p4.Perp() < 20 && jets_size[i] >= 1 && ainvmasscutW && datasets[d].Name() == "Jet"){   	    

             if (electrons[0].RelIso03PF() < 0.8){ 
                  IsoLooseForFakeRateNJets[i]->Fill(jets_Loose.size());
                  IsoLooseForFakeRateEta[i]->Fill(electrons[0].p4.Eta());
                  IsoLooseForFakeRatePT[i]->Fill(electrons[0].p4.Perp());
                  IsoLooseForFakeRateNVTX[i]->Fill(selVertices.size());
             }
   	     if (electrons[0].RelIso03PF() < 0.17){ 
                  IsoTightForFakeRateNJets[i]->Fill(jets_Tight.size());
                  IsoTightForFakeRateEta[i]->Fill(electrons[0].p4.Eta());
                  IsoTightForFakeRatePT[i]->Fill(electrons[0].p4.Perp());
                  IsoTightForFakeRateNVTX[i]->Fill(selVertices.size());
             }
        }

   
        if(electrons.size() == 1 && met.p4.Perp() < 20 && ainvmasscutW && jets_size[i] >= 1 && datasets[d].Name() != "Jet" && datasets[d].Name() != "DataEG"){
	     if (electrons[0].RelIso03PF() < 0.8){ 
                  IsoLooseForFakeRateNJets_Corr[i]->Fill(jets_Loose.size(),weight);
                  IsoLooseForFakeRateEta_Corr[i]->Fill(electrons[0].p4.Eta(),weight);
                  IsoLooseForFakeRatePT_Corr[i]->Fill(electrons[0].p4.Perp(),weight);
                  IsoLooseForFakeRateNVTX_Corr[i]->Fill(selVertices.size());
             }
   	     if (electrons[0].RelIso03PF() < 0.17){ 
                  IsoTightForFakeRateNJets_Corr[i]->Fill(jets_Tight.size(),weight);
                  IsoTightForFakeRateEta_Corr[i]->Fill(electrons[0].p4.Eta(),weight);
                  IsoTightForFakeRatePT_Corr[i]->Fill(electrons[0].p4.Perp(),weight);
                  IsoTightForFakeRateNVTX_Corr[i]->Fill(selVertices.size());
             }
        }
       }


      

       if(electrons.size() == 2 && datasets[d].Name() == "DataEG"){
        TLorentzVector Dilepton;
        Dilepton = electrons[0].p4 + electrons[1].p4;
        bool ainvmasscut = (Dilepton.M() < 106 && Dilepton.M() > 76);

        if(electrons[0].Charge == -1 && electrons[0].RelIso03PF() < 0.17){
          if(electrons[1].Charge == +1 && ainvmasscut){
            if (electrons[1].RelIso03PF() < 0.8){ 
                  IsoLooseForEfficiencyNJets->Fill(jets_Loose.size());
                  IsoLooseForEfficiencyEta->Fill(electrons[1].p4.Eta());
                  IsoLooseForEfficiencyPT->Fill(electrons[1].p4.Perp());
                  IsoLooseForEfficiencyNVTX->Fill(selVertices.size());
            }
	    if (electrons[1].RelIso03PF() < 0.17){ 
                  IsoTightForEfficiencyNJets->Fill(jets_Tight.size());
                  IsoTightForEfficiencyEta->Fill(electrons[1].p4.Eta());
                  IsoTightForEfficiencyPT->Fill(electrons[1].p4.Perp());
                  IsoTightForEfficiencyNVTX->Fill(selVertices.size());
            }
          }
        }
        if(electrons[1].Charge == -1 && electrons[1].RelIso03PF() < 0.17){
          if(electrons[0].Charge == +1 && ainvmasscut){
            if (electrons[0].RelIso03PF() < 0.8){
                  IsoLooseForEfficiencyNJets->Fill(jets_Loose.size());
                  IsoLooseForEfficiencyEta->Fill(electrons[0].p4.Eta());
                  IsoLooseForEfficiencyPT->Fill(electrons[0].p4.Perp());
                  IsoLooseForEfficiencyNVTX->Fill(selVertices.size());
            }
 	    if (electrons[0].RelIso03PF() < 0.17){
                  IsoTightForEfficiencyNJets->Fill(jets_Tight.size());
                  IsoTightForEfficiencyEta->Fill(electrons[0].p4.Eta());
                  IsoTightForEfficiencyPT->Fill(electrons[0].p4.Perp());
                  IsoTightForEfficiencyNVTX->Fill(selVertices.size());
            }
          }
        }
       }


    } // end of loop over evts


  } // end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;

  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

  TFile* file_MC = new TFile("MatrixMethod_Efficiency_MC_E.root");
  file_MC->cd();
  FakeRateNJets_MC = (TH1F*)gDirectory->Get("FakeRateNJets");
  SignalEfficiencyNJets_MC = (TH1F*)gDirectory->Get("SignalEfficiencyNJets");
  //  FakeRateEta_MC = (TH1F*)gDirectory->Get("FakeRateEta");
  //  FakeRatePT_MC = (TH1F*)gDirectory->Get("FakeRatePT");

 TFile* file_MM = new TFile("MatrixMethod_Efficiency_DATA_E.root","RECREATE");
  file_MM->cd();



  for(unsigned int i=0; i<NJets_N_bins; i++){
     float num_s = (float)IsoTightForEfficiencyNJets->GetBinContent(i+1);
     float den_s = (float)IsoLooseForEfficiencyNJets->GetBinContent(i+1);
     float eff_s = 0.;
     float err_s = 0.;
     if(den_s != 0.) {
      eff_s = num_s/den_s;
      err_s = (sqrt(num_s*(1-(num_s/den_s)))/den_s);
     }
     SignalEfficiencyNJets->SetBinContent(i+1, eff_s);
     SignalEfficiencyNJets->SetBinError(i+1, err_s);
     float err_s_sys = (float)(eff_s-SignalEfficiencyNJets_MC->GetBinContent(i+1));
     if(err_s_sys < 0.) err_s_sys = (-1)*err_s_sys;
     SignalEfficiencyNJetsSyst->SetBinContent(i+1, eff_s);
     SignalEfficiencyNJetsSyst->SetBinError(i+1, err_s+(err_s_sys/2));
  }



  for(unsigned int i=0; i<Eta_N_bins; i++){
     float num_s = (float)IsoTightForEfficiencyEta->GetBinContent(i+1);
     float den_s = (float)IsoLooseForEfficiencyEta->GetBinContent(i+1);
     float eff_s = 0.;
     float err_s = 0.;
     if(den_s != 0.) {
      eff_s = num_s/den_s;
      err_s = (sqrt(num_s*(1-(num_s/den_s)))/den_s);
     }
     SignalEfficiencyEta->SetBinContent(i+1, eff_s);
     SignalEfficiencyEta->SetBinError(i+1, err_s);
  }



  for(unsigned int i=0; i<PT_N_bins; i++){
     float num_s = (float)IsoTightForEfficiencyPT->GetBinContent(i+1);
     float den_s = (float)IsoLooseForEfficiencyPT->GetBinContent(i+1);
     float eff_s = 0.;
     float err_s = 0.;
     if(den_s != 0.) {
      eff_s = num_s/den_s;
      err_s = (sqrt(num_s*(1-(num_s/den_s)))/den_s);
     }
     SignalEfficiencyPT->SetBinContent(i+1, eff_s);
     SignalEfficiencyPT->SetBinError(i+1, err_s);
  }


  for(unsigned int i=0; i<NVTX_N_bins; i++){
     float num_s = (float)IsoTightForEfficiencyNVTX->GetBinContent(i+1);
     float den_s = (float)IsoLooseForEfficiencyNVTX->GetBinContent(i+1);
     float eff_s = 0.;
     float err_s = 0.;
     if(den_s != 0.) {
      eff_s = num_s/den_s;
      err_s = (sqrt(num_s*(1-(num_s/den_s)))/den_s);
     }
     SignalEfficiencyNVTX->SetBinContent(i+1, eff_s);
     SignalEfficiencyNVTX->SetBinError(i+1, err_s);
  }





  for(unsigned int j=0; j<10; j++){

  for(unsigned int i=0; i<NJets_N_bins; i++){
     float num_f = (float)(IsoTightForFakeRateNJets[j]->GetBinContent(i+1));
     float den_f = (float)(IsoLooseForFakeRateNJets[j]->GetBinContent(i+1));
     float eff_f = 0.;
     float err_f = 0.;
     if(den_f != 0.) {
      eff_f = num_f/den_f;
      err_f = (sqrt(num_f*(1-(num_f/den_f)))/den_f);
     }
 //    float err_f_sys = (float)(eff_f-FakeRateNJets_MC->GetBinContent(i+1));
 //    if(err_f_sys < 0.) err_f_sys = (-1)*err_f_sys;
     num_f = (float)(IsoTightForFakeRateNJets[j]->GetBinContent(i+1) /*- IsoTightForFakeRateNJets_Corr[j]->GetBinContent(i+1)*/);
     den_f = (float)(IsoLooseForFakeRateNJets[j]->GetBinContent(i+1) /*- IsoLooseForFakeRateNJets_Corr[j]->GetBinContent(i+1)*/);
     FakeRateNJets[j]->SetBinContent(i+1, eff_f);
     FakeRateNJets[j]->SetBinError(i+1, err_f/*+err_f_sys*/);
     float err_f_sys = (float)(eff_f-FakeRateNJets_MC->GetBinContent(i+1));
     if(err_f_sys < 0.) err_f_sys = (-1)*err_f_sys;
     FakeRateNJetsSyst[j]->SetBinContent(i+1, eff_f);
     FakeRateNJetsSyst[j]->SetBinError(i+1, err_f+(err_f_sys/2));

  }



  for(unsigned int i=0; i<Eta_N_bins; i++){
     float num_f = (float)(IsoTightForFakeRateEta[j]->GetBinContent(i+1));
     float den_f = (float)(IsoLooseForFakeRateEta[j]->GetBinContent(i+1));
     float eff_f = 0.;
     float err_f = 0.;
     if(den_f != 0.) {
      eff_f = num_f/den_f;
      err_f = (sqrt(num_f*(1-(num_f/den_f)))/den_f);
     }
     FakeRateEta[j]->SetBinContent(i+1, eff_f);
 //    float err_f_sys = (float)(eff_f-FakeRateEta_MC->GetBinContent(i+1));
 //    if(err_f_sys < 0.) err_f_sys = (-1)*err_f_sys;
     num_f = (float)(IsoTightForFakeRateEta[j]->GetBinContent(i+1) /*- IsoTightForFakeRateEta_Corr[j]->GetBinContent(i+1)*/);
     den_f = (float)(IsoLooseForFakeRateEta[j]->GetBinContent(i+1) /*- IsoLooseForFakeRateEta_Corr[j]->GetBinContent(i+1)*/);
     FakeRateEta[j]->SetBinContent(i+1, eff_f);
     FakeRateEta[j]->SetBinError(i+1, err_f/*+err_f_sys*/);
  }

  for(unsigned int i=0; i<PT_N_bins; i++){
     float num_f = (float)(IsoTightForFakeRatePT[j]->GetBinContent(i+1));
     float den_f = (float)(IsoLooseForFakeRatePT[j]->GetBinContent(i+1));
     float eff_f = 0.;
     float err_f = 0.;
     if(den_f != 0.) {
      eff_f = num_f/den_f;
      err_f = (sqrt(num_f*(1-(num_f/den_f)))/den_f);
     }
     FakeRatePT[j]->SetBinContent(i+1, eff_f);
 //    float err_f_sys = (float)(eff_f-FakeRatePT_MC->GetBinContent(i+1));
 //    if(err_f_sys < 0.) err_f_sys = (-1)*err_f_sys;
     num_f = (float)(IsoTightForFakeRatePT[j]->GetBinContent(i+1) /*- IsoTightForFakeRatePT_Corr[j]->GetBinContent(i+1)*/);
     den_f = (float)(IsoLooseForFakeRatePT[j]->GetBinContent(i+1) /*- IsoLooseForFakeRatePT_Corr[j]->GetBinContent(i+1)*/);
     FakeRatePT[j]->SetBinContent(i+1, eff_f);
     FakeRatePT[j]->SetBinError(i+1, err_f/*+err_f_sys*/);
  }

  for(unsigned int i=0; i<NVTX_N_bins; i++){
     float num_f = (float)(IsoTightForFakeRateNVTX[j]->GetBinContent(i+1));
     float den_f = (float)(IsoLooseForFakeRateNVTX[j]->GetBinContent(i+1));
     float eff_f = 0.;
     float err_f = 0.;
     if(den_f != 0.) {
      eff_f = num_f/den_f;
      err_f = (sqrt(num_f*(1-(num_f/den_f)))/den_f);
     }
     FakeRateNVTX[j]->SetBinContent(i+1, eff_f);
 //    float err_f_sys = (float)(eff_f-FakeRateNVTX_MC->GetBinContent(i+1));
 //    if(err_f_sys < 0.) err_f_sys = (-1)*err_f_sys;
     num_f = (float)(IsoTightForFakeRateNVTX[j]->GetBinContent(i+1) /*- IsoTightForFakeRateNVTX_Corr[j]->GetBinContent(i+1)*/);
     den_f = (float)(IsoLooseForFakeRateNVTX[j]->GetBinContent(i+1) /*- IsoLooseForFakeRateNVTX_Corr[j]->GetBinContent(i+1)*/);
     FakeRateNVTX[j]->SetBinContent(i+1, eff_f);
     FakeRateNVTX[j]->SetBinError(i+1, err_f/*+err_f_sys*/);
  }




  }





/*
  file_MC->Close();
  delete file_MC;
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
  cout << "$\\epsilon_{s}$ electron case";
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
  cout << "$\\epsilon_{s}$ electron case";
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
  cout << "$\\epsilon_{s}$ electron case";
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
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<NVTX_N_bins; bin_index++){
   cout << " & NVTX = " <<   ((NVTX_Max-NVTX_Min)/NVTX_N_bins)*(bin_index+0.5);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{s}$ electron case";
  for(unsigned int bin_index=0; bin_index<NVTX_N_bins; bin_index++){
   cout << " & " << SignalEfficiencyNVTX->GetBinContent(bin_index) << "$\\pm$" << SignalEfficiencyNVTX->GetBinError(bin_index);
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
  cout << "$\\epsilon_{f}$ electron case";
  for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
   cout << " & " << FakeRateNJets[0]->GetBinContent(bin_index) << "$\\pm$" << FakeRateNJets[0]->GetBinError(bin_index);
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
  cout << "$\\epsilon_{f}$ electron case";
  for(unsigned int bin_index=0; bin_index<Eta_N_bins; bin_index++){
   cout << " & " << FakeRateEta[0]->GetBinContent(bin_index) << "$\\pm$" << FakeRateEta[0]->GetBinError(bin_index);
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
  cout << "$\\epsilon_{f}$ electron case";
  for(unsigned int bin_index=0; bin_index<PT_N_bins; bin_index++){
   cout << " & " << FakeRatePT[0]->GetBinContent(bin_index) << "$\\pm$" << FakeRatePT[0]->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;



  cout << "\\begin{table}" << std::endl;
  cout << "\\begin{center}" << std::endl;
  cout << "\\begin{tabular}{|";
  for(unsigned int bin_index=0; bin_index<NVTX_N_bins; bin_index++){
   cout << " c |";
  }
  cout << "}" << std::endl;
  cout << "\\hline" << std::endl;
  for(unsigned int bin_index=0; bin_index<NVTX_N_bins; bin_index++){
   cout << " & NVTX = " << ((NVTX_Max-NVTX_Min)/NVTX_N_bins)*(bin_index+0.5);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "$\\epsilon_{f}$ electron case";
  for(unsigned int bin_index=0; bin_index<NVTX_N_bins; bin_index++){
   cout << " & " << FakeRateNVTX[0]->GetBinContent(bin_index) << "$\\pm$" << FakeRateNVTX[0]->GetBinError(bin_index);
  }
  cout << "\\\\ \\hline" << std::endl;
  cout << "\\end{tabular}" << std::endl;
  cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
  cout << "\\end{center}" << std::endl;
  cout << "\\end{table}" << std::endl;







  Iso_DATA_Jet->Write();
  Iso_DATA_Jet = 0;
  delete Iso_DATA_Jet;

  PT_Leading_Jet_DATA_Jet->Write();
  PT_Leading_Jet_DATA_Jet = 0;
  delete PT_Leading_Jet_DATA_Jet;

  PT_Inclusive_Jet_DATA_Jet->Write();
  PT_Inclusive_Jet_DATA_Jet = 0;
  delete PT_Inclusive_Jet_DATA_Jet;


  for(unsigned int j=0; j<10; j++){

  IsoTightForFakeRateNJets[j]->Write();
  IsoTightForFakeRateNJets[j] = 0;
  delete IsoTightForFakeRateNJets[j];
  IsoTightForFakeRateEta[j]->Write();
  IsoTightForFakeRateEta[j] = 0;
  delete IsoTightForFakeRateEta[j];
  IsoTightForFakeRatePT[j]->Write();
  IsoTightForFakeRatePT[j] = 0;
  delete IsoTightForFakeRatePT[j];
  IsoTightForFakeRateNVTX[j]->Write();
  IsoTightForFakeRateNVTX[j] = 0;
  delete IsoTightForFakeRateNVTX[j];
  IsoTightForFakeRateNJets_Corr[j]->Write();
  IsoTightForFakeRateNJets_Corr[j] = 0;
  delete IsoTightForFakeRateNJets_Corr[j];
  IsoTightForFakeRateEta_Corr[j]->Write();
  IsoTightForFakeRateEta_Corr[j] = 0;
  delete IsoTightForFakeRateEta_Corr[j];
  IsoTightForFakeRatePT_Corr[j]->Write();
  IsoTightForFakeRatePT_Corr[j] = 0;
  delete IsoTightForFakeRatePT_Corr[j];
  IsoTightForFakeRateNVTX_Corr[j]->Write();
  IsoTightForFakeRateNVTX_Corr[j] = 0;
  delete IsoTightForFakeRateNVTX_Corr[j];
  IsoLooseForFakeRateNJets[j]->Write();
  IsoLooseForFakeRateNJets[j] = 0;
  delete IsoLooseForFakeRateNJets[j];
  IsoLooseForFakeRateEta[j]->Write();
  IsoLooseForFakeRateEta[j] = 0;
  delete IsoLooseForFakeRateEta[j];
  IsoLooseForFakeRatePT[j]->Write();
  IsoLooseForFakeRatePT[j] = 0;
  delete IsoLooseForFakeRatePT[j];
  IsoLooseForFakeRateNVTX[j]->Write();
  IsoLooseForFakeRateNVTX[j] = 0;
  delete IsoLooseForFakeRateNVTX[j];
  IsoLooseForFakeRateNJets_Corr[j]->Write();
  IsoLooseForFakeRateNJets_Corr[j] = 0;
  delete IsoLooseForFakeRateNJets_Corr[j];
  IsoLooseForFakeRateEta_Corr[j]->Write();
  IsoLooseForFakeRateEta_Corr[j] = 0;
  delete IsoLooseForFakeRateEta_Corr[j];
  IsoLooseForFakeRatePT_Corr[j]->Write();
  IsoLooseForFakeRatePT_Corr[j] = 0;
  delete IsoLooseForFakeRatePT_Corr[j];
  IsoLooseForFakeRateNVTX_Corr[j]->Write();
  IsoLooseForFakeRateNVTX_Corr[j] = 0;
  delete IsoLooseForFakeRateNVTX_Corr[j];
  FakeRateNJets[j]->Write();
  FakeRateNJets[j] = 0;
  delete FakeRateNJets[j];
  FakeRateNJetsSyst[j]->Write();
  FakeRateNJetsSyst[j] = 0;
  delete FakeRateNJetsSyst[j];
  FakeRateEta[j]->Write();
  FakeRateEta[j] = 0;
  delete FakeRateEta[j];
  FakeRatePT[j]->Write();
  FakeRatePT[j] = 0;
  delete FakeRatePT[j];
  FakeRateNVTX[j]->Write();
  FakeRateNVTX[j] = 0;
  delete FakeRateNVTX[j];
  }


  SignalEfficiencyNJets->Write();
  SignalEfficiencyNJets = 0;
  delete SignalEfficiencyNJets;

  SignalEfficiencyNJetsSyst->Write();
  SignalEfficiencyNJetsSyst = 0;
  delete SignalEfficiencyNJetsSyst;

  SignalEfficiencyEta->Write();
  SignalEfficiencyEta = 0;
  delete SignalEfficiencyEta;
  SignalEfficiencyPT->Write();
  SignalEfficiencyPT = 0;
  delete SignalEfficiencyPT;
  SignalEfficiencyNVTX->Write();
  SignalEfficiencyNVTX = 0;
  delete SignalEfficiencyNVTX;


  file_MM->Write();
  file_MM->Close();
  delete file_MM;

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}

