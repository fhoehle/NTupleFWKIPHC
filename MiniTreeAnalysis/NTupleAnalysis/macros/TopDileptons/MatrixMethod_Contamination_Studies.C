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

bool highestDeltaR (float deltaR1, float deltaR2) { return (deltaR1<deltaR2); }

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
  SelectionTable selTable_ee(  sel.GetCutList(),datasets, string("ee")  );
  SelectionTable selTable_mumu(  sel.GetCutList(),datasets, string("mumu")  );
  SelectionTable selTable_emu(  sel.GetCutList(),datasets, string("emu")  );

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

  string channel[3] = {"ee", "mumu", "emu"};

  TH1F * TT_Signal[3];
  TH1F * TT_Wjets[3];
  TH1F * TT_QCD[3];

  TH1F * TF_Signal[3];
  TH1F * TF_Wjets[3];
  TH1F * TF_QCD[3];

  TH1F * FF_Signal[3];
  TH1F * FF_Wjets[3];
  TH1F * FF_QCD[3];


  for (unsigned int i = 0; i < 3; i++) {
    TT_Signal[i] = new TH1F (("TT_Signal"+channel[i]).c_str(),("TT_Signal"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    TT_Wjets[i] = new TH1F (("TT_Wjets"+channel[i]).c_str(),("TT_Wjets"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    TT_QCD[i] = new TH1F (("TT_QCD"+channel[i]).c_str(),("TT_QCD"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);

    TF_Signal[i] = new TH1F (("TF_Signal"+channel[i]).c_str(),("TF_Signal"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    TF_Wjets[i] = new TH1F (("TF_Wjets"+channel[i]).c_str(),("TF_Wjets"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    TF_QCD[i] = new TH1F (("TF_QCD"+channel[i]).c_str(),("TF_QCD"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);

    FF_Signal[i] = new TH1F (("FF_Signal"+channel[i]).c_str(),("FF_Signal"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    FF_Wjets[i] = new TH1F (("FF_Wjets"+channel[i]).c_str(),("FF_Wjets"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
    FF_QCD[i] = new TH1F (("FF_QCD"+channel[i]).c_str(),("FF_QCD"+channel[i]).c_str(), NJets_N_bins, NJets_Min, NJets_Max);
  }

  DiLeptonSelection sel_aux0(sel);
  DiLeptonSelection sel_aux1(sel);
  DiLeptonSelection sel_aux2(sel);

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
      
       if(ievt%10000 == 0) cout << "number of processed events " << ievt << endl;

       //Manage DY samples to avoid overlaps
       double dileptInvMass = 0;
       if( (event->zAndDecays).size() > 0){
         TLorentzVector dilept = (event->zAndDecays)[0].p4_Lep1_gen + (event->zAndDecays)[0].p4_Lep2_gen; 
  	 dileptInvMass = dilept.M();
       }             
       if(datasets[d].Name()=="Zjets" && dileptInvMass < 50) continue;
       if(datasets[d].Name()=="DYToMuMu_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
       if(datasets[d].Name()=="DYToEE_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
       if(datasets[d].Name()=="DYToTauTau_M-20"	   && (dileptInvMass > 50 || dileptInvMass < 20) ) continue;
       if(datasets[d].Name()=="DYToMuMu_M-10To20"   &&  dileptInvMass > 20) continue;
       if(datasets[d].Name()=="DYToEE_M-10To20"	   &&  dileptInvMass > 20) continue;
       if(datasets[d].Name()=="DYToTauTau_M-10To20" &&  dileptInvMass > 20) continue;
      
       vector<NTVertex>   selVertices  = sel.GetSelectedVertex();
       ///float weight = weight_init*sel.GetPUWeight();
       float weight = 0;
       if(thePUReweighter.getUseOutOfTimePU()){
          weight = weight_init*thePUReweighter.weight(event->num_pileup_bc0, event->runNb);
       }else{
          weight = weight_init*thePUReweighter.weight(event->num_pileup_bc0);
       }
       int selLastStep[3];
       selLastStep[0] = sel_aux0.FillTableForMM(0.17, 0.17, selTable_ee, &(datasets[d]), d, weight);
       selLastStep[1] = sel_aux1.FillTableForMM(0.2, 0.2, selTable_mumu, &(datasets[d]), d, weight);
       selLastStep[2] = sel_aux2.FillTableForMM(0.2, 0.17, selTable_emu, &(datasets[d]), d, weight);

       vector<NTElectron> candElec[3];
       vector<NTMuon> candMuon[3];

       sel_aux0.GetLeptonPairForMM(0.17, 0.17, candMuon[0], candElec[0], channel[0]);
       sel_aux1.GetLeptonPairForMM(0.2, 0.2, candMuon[1], candElec[1], channel[1]);
       sel_aux2.GetLeptonPairForMM(0.2, 0.17, candMuon[2], candElec[2], channel[2]);


       vector<NTJet> SelectedJets[3];
       SelectedJets[0] = sel_aux0.GetSelectedJets(candMuon[0], candElec[0]);
       SelectedJets[1] = sel_aux1.GetSelectedJets(candMuon[1], candElec[1]);
       SelectedJets[2] = sel_aux2.GetSelectedJets(candMuon[2], candElec[2]);




       for (unsigned int i = 0; i < 3; i++) {

        unsigned int njets = SelectedJets[i].size();
        std::vector<NTElectron> electrons = candElec[i];
        std::vector<NTMuon> muons = candMuon[i];



        if(selLastStep[i] >= 6){

           unsigned int electron_Matchings = 0;
           for (unsigned int j = 0; j < electrons.size(); j++) {
             for (unsigned int k = 0; k < (event->zAndDecays).size(); k++) {
               float Delta_Pt1 = electrons[j].p4.Perp() - (event->zAndDecays)[k].p4_Lep1_gen.Perp();
               if(Delta_Pt1 < 0.) Delta_Pt1 = - Delta_Pt1;
               float Delta_Pt2 = electrons[j].p4.Perp() - (event->zAndDecays)[k].p4_Lep2_gen.Perp();
               if(Delta_Pt2 < 0.) Delta_Pt2 = - Delta_Pt2;
	       float Delta_R1 = 9999;
	       float Delta_R2 = 9999;
	       if((event->zAndDecays)[k].p4_Lep1_gen.Perp() != 0.) Delta_R1 = electrons[j].p4.DeltaR((event->zAndDecays)[k].p4_Lep1_gen);
	       if((event->zAndDecays)[k].p4_Lep2_gen.Perp() != 0.) Delta_R2 = electrons[j].p4.DeltaR((event->zAndDecays)[k].p4_Lep2_gen);
	       if(Delta_R1 < 0.15 || Delta_Pt1 < 5) electron_Matchings++;
	       if(Delta_R2 < 0.15 || Delta_Pt2 < 5) electron_Matchings++;
             }
             for (unsigned int k = 0; k < (event->wAndDecays).size(); k++) {
               float Delta_Pt = electrons[j].p4.Perp() - (event->wAndDecays)[k].p4_Lep_gen.Perp();
               if(Delta_Pt < 0.) Delta_Pt = - Delta_Pt;
	       float Delta_R = 9999;
	       if((event->wAndDecays)[k].p4_Lep_gen.Perp() != 0.) Delta_R = electrons[j].p4.DeltaR((event->wAndDecays)[k].p4_Lep_gen);
               if(Delta_R < 0.15 || Delta_Pt < 5) electron_Matchings++;
             }
           }

           unsigned int muon_Matchings = 0;
	   //	     cout << "**************" << endl;
           for (unsigned int j = 0; j < muons.size(); j++) {
	     //	     if(muons.size() == 2)cout << "Reco muon pT: " <<  muons[j].p4.Perp() << endl;
             for (unsigned int k = 0; k < (event->zAndDecays).size(); k++) {
               float Delta_Pt1 = muons[j].p4.Perp() - (event->zAndDecays)[k].p4_Lep1_gen.Perp();
               if(Delta_Pt1 < 0.) Delta_Pt1 = - Delta_Pt1;
               float Delta_Pt2 = muons[j].p4.Perp() - (event->zAndDecays)[k].p4_Lep2_gen.Perp();
               if(Delta_Pt2 < 0.) Delta_Pt2 = - Delta_Pt2;
	       float Delta_R1 = 9999;
	       float Delta_R2 = 9999;
	       if((event->zAndDecays)[k].p4_Lep1_gen.Perp() != 0.) Delta_R1 = muons[j].p4.DeltaR((event->zAndDecays)[k].p4_Lep1_gen);
	       if((event->zAndDecays)[k].p4_Lep2_gen.Perp() != 0.) Delta_R2 = muons[j].p4.DeltaR((event->zAndDecays)[k].p4_Lep2_gen);
	       if(Delta_R1 < 0.4 || Delta_Pt1 < 10) muon_Matchings++;
	       if(Delta_R2 < 0.4 || Delta_Pt2 < 10) muon_Matchings++;
             }
             for (unsigned int k = 0; k < (event->wAndDecays).size(); k++) {
	       //	       if(muons.size() == 2)cout << "Gen muon pT: " << (event->wAndDecays)[k].p4_Lep_gen.Perp()  << endl;
               float Delta_Pt = muons[j].p4.Perp() - (event->wAndDecays)[k].p4_Lep_gen.Perp();
               if(Delta_Pt < 0.) Delta_Pt = - Delta_Pt;
	       float Delta_R = 9999;
	       if((event->wAndDecays)[k].p4_Lep_gen.Perp() != 0.) Delta_R = muons[j].p4.DeltaR((event->wAndDecays)[k].p4_Lep_gen);
	       //	       if(muons.size() == 2)cout << "DeltaR: " << Delta_R << endl;
               if(Delta_R < 0.4 || Delta_Pt < 10) muon_Matchings++;
             }
           }






           if(electron_Matchings == 2 && channel[i] == "ee" && (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200) //ee
           ){
	     TT_Signal[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 2 && channel[i] == "mumu" && (event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000) //mumu
           ){
	     TT_Signal[1]->Fill(njets,weight);
           }
           if(electron_Matchings == 1 && muon_Matchings == 1 && channel[i] == "emu" && (event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) //emu
           ){
	     TT_Signal[2]->Fill(njets,weight);
           }


           if(electron_Matchings == 2 && channel[i] == "ee" && (event->TMEME == 1 /*|| event->TMEME == 10*/ || // l(=e)jets - no l(=mu)jets
                                          event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        /*event->TMEME == 10010 ||*/ event->TMEME == 10001 || // l(=e)tau(->had) - no l(=mu)tau(->had)
					  event->TMEME == 10100 /*|| event->TMEME == 11000*/)  // tau(->e)jets - no tau(->mu)jets
	   ){
	     TT_Wjets[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 2 && channel[i] == "mumu" && (/*event->TMEME == 1 ||*/ event->TMEME == 10 || // no l(=e)jets - l(=mu)jets
                                        event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        event->TMEME == 10010 || /*event->TMEME == 10001 ||*/ // no l(=e)tau(->had) - l(=mu)tau(->had)
				      /*event->TMEME == 10100 ||*/ event->TMEME == 11000)  // no tau(->e)jets - tau(->mu)jets
	   ){
	     TT_Wjets[1]->Fill(njets,weight);
           }
           if(electron_Matchings == 1 && muon_Matchings == 1 && channel[i] == "emu" && (event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                                                            event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
		                                            event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                                                            event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                                                            event->TMEME == 10100 || event->TMEME == 11000) // tau(->e/mu)jets
	   ){
	     TT_Wjets[2]->Fill(njets,weight);
           }


           if(electron_Matchings == 2 && channel[i] == "ee" && !(event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || //ee
                                          event->TMEME == 1 /*|| event->TMEME == 10*/ || // l(=e)jets - no l(=mu)jets
                                          event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        /*event->TMEME == 10010 ||*/ event->TMEME == 10001 || // l(=e)tau(->had) - no l(=mu)tau(->had)
					  event->TMEME == 10100 /*|| event->TMEME == 11000*/)  // tau(->e)jets - no tau(->mu)jets
           ){
             TT_QCD[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 2 &&  channel[i] == "mumu" && !(event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || //mumu
                                      /*event->TMEME == 1 ||*/ event->TMEME == 10 || // no l(=e)jets - l(=mu)jets
                                        event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        event->TMEME == 10010 || /*event->TMEME == 10001 ||*/ // no l(=e)tau(->had) - l(=mu)tau(->had)
				      /*event->TMEME == 10100 ||*/ event->TMEME == 11000)  // no tau(->e)jets - tau(->mu)jets
           ){
             TT_QCD[1]->Fill(njets,weight);
           }
           if(electron_Matchings == 1 && muon_Matchings == 1 && channel[i] == "emu" && 
                                                                !(event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || //emu
                                                                  event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                                                                  event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
		                                                  event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                                                                  event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                                                                  event->TMEME == 10100 || event->TMEME == 11000) // tau(->e/mu)jets
           ){
             TT_QCD[2]->Fill(njets,weight);
           }











           if(electron_Matchings == 1 && channel[i] == "ee" && (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200) //ee
           ){
	     TF_Signal[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 1 && channel[i] == "mumu" && (event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000) //mumu
           ){
	     TF_Signal[1]->Fill(njets,weight);
           }
           if( ((electron_Matchings == 1 && muon_Matchings == 0) || (electron_Matchings == 0 && muon_Matchings == 1)) && channel[i] == "emu" &&
                (event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) //emu
           ){
	     TF_Signal[2]->Fill(njets,weight);
           }


           if(electron_Matchings == 1 && channel[i] == "ee" && (event->TMEME == 1 /*|| event->TMEME == 10*/ || // l(=e)jets - no l(=mu)jets
                                          event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        /*event->TMEME == 10010 ||*/ event->TMEME == 10001 || // l(=e)tau(->had) - no l(=mu)tau(->had)
					  event->TMEME == 10100 /*|| event->TMEME == 11000*/)  // tau(->e)jets - no tau(->mu)jets
	   ){
	     TF_Wjets[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 1 && channel[i] == "mumu" && (/*event->TMEME == 1 ||*/ event->TMEME == 10 || // no l(=e)jets - l(=mu)jets
                                        event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        event->TMEME == 10010 || /*event->TMEME == 10001 ||*/ // no l(=e)tau(->had) - l(=mu)tau(->had)
				      /*event->TMEME == 10100 ||*/ event->TMEME == 11000)  // no tau(->e)jets - tau(->mu)jets
	   ){
	     TF_Wjets[1]->Fill(njets,weight);
           }
           if(  ((electron_Matchings == 1 && muon_Matchings == 0) || (electron_Matchings == 0 && muon_Matchings == 1)) && channel[i] == "emu" &&
                                                           (event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                                                            event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
		                                            event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                                                            event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                                                            event->TMEME == 10100 || event->TMEME == 11000) // tau(->e/mu)jets
	   ){
	     TF_Wjets[2]->Fill(njets,weight);
           }


           if(electron_Matchings == 1 && channel[i] == "ee" && !(event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || //ee
                                          event->TMEME == 1 /*|| event->TMEME == 10*/ || // l(=e)jets - no l(=mu)jets
                                          event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        /*event->TMEME == 10010 ||*/ event->TMEME == 10001 || // l(=e)tau(->had) - no l(=mu)tau(->had)
					  event->TMEME == 10100 /*|| event->TMEME == 11000*/)  // tau(->e)jets - no tau(->mu)jets
           ){
             TF_QCD[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 1 && channel[i] == "mumu" && !(event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || //mumu
                                      /*event->TMEME == 1 ||*/ event->TMEME == 10 || // no l(=e)jets - l(=mu)jets
                                        event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        event->TMEME == 10010 || /*event->TMEME == 10001 ||*/ // no l(=e)tau(->had) - l(=mu)tau(->had)
				      /*event->TMEME == 10100 ||*/ event->TMEME == 11000)  // no tau(->e)jets - tau(->mu)jets
           ){
             TF_QCD[1]->Fill(njets,weight);
           }
           if( ((electron_Matchings == 1 && muon_Matchings == 0) || (electron_Matchings == 0 && muon_Matchings == 1))  && channel[i] == "emu" &&
                                                                !(event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || //emu
                                                                  event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                                                                  event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
		                                                  event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                                                                  event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                                                                  event->TMEME == 10100 || event->TMEME == 11000) // tau(->e/mu)jets
           ){
             TF_QCD[2]->Fill(njets,weight);
           }








           if(electron_Matchings == 0 && channel[i] == "ee" && (event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200) //ee
           ){
	     FF_Signal[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 0 && channel[i] == "mumu" && (event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000) //mumu
           ){
	     FF_Signal[1]->Fill(njets,weight);
           }
           if(electron_Matchings == 0 && muon_Matchings == 0 && channel[i] == "emu" &&
                                                            (event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100) //emu
           ){
	     FF_Signal[2]->Fill(njets,weight);
           }


           if(electron_Matchings == 0 && channel[i] == "ee" && (event->TMEME == 1 /*|| event->TMEME == 10*/ || // l(=e)jets - no l(=mu)jets
                                          event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        /*event->TMEME == 10010 ||*/ event->TMEME == 10001 || // l(=e)tau(->had) - no l(=mu)tau(->had)
					  event->TMEME == 10100 /*|| event->TMEME == 11000*/)  // tau(->e)jets - no tau(->mu)jets
	   ){
	     FF_Wjets[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 0 && channel[i] == "mumu" && (/*event->TMEME == 1 ||*/ event->TMEME == 10 || // no l(=e)jets - l(=mu)jets
                                        event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        event->TMEME == 10010 || /*event->TMEME == 10001 ||*/ // no l(=e)tau(->had) - l(=mu)tau(->had)
				      /*event->TMEME == 10100 ||*/ event->TMEME == 11000)  // no tau(->e)jets - tau(->mu)jets
	   ){
	     FF_Wjets[1]->Fill(njets,weight);
           }
           if(electron_Matchings == 0 && muon_Matchings == 0 && channel[i] == "emu" && (event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                                                            event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
		                                            event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                                                            event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                                                            event->TMEME == 10100 || event->TMEME == 11000) // tau(->e/mu)jets
	   ){
	     FF_Wjets[2]->Fill(njets,weight);
           }


           if(electron_Matchings == 0 && channel[i] == "ee" && !(event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || //ee
                                          event->TMEME == 1 /*|| event->TMEME == 10*/ || // l(=e)jets - no l(=mu)jets
                                          event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        /*event->TMEME == 10010 ||*/ event->TMEME == 10001 || // l(=e)tau(->had) - no l(=mu)tau(->had)
					  event->TMEME == 10100 /*|| event->TMEME == 11000*/)  // tau(->e)jets - no tau(->mu)jets
           ){
             FF_QCD[0]->Fill(njets,weight);
           }
           if(muon_Matchings == 0 && channel[i] == "mumu" && !(event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || //mumu
                                      /*event->TMEME == 1 ||*/ event->TMEME == 10 || // no l(=e)jets - l(=mu)jets
                                        event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || // emu
				        event->TMEME == 10010 || /*event->TMEME == 10001 ||*/ // no l(=e)tau(->had) - l(=mu)tau(->had)
				      /*event->TMEME == 10100 ||*/ event->TMEME == 11000)  // no tau(->e)jets - tau(->mu)jets
           ){
             FF_QCD[1]->Fill(njets,weight);
           }
           if(electron_Matchings == 0 && muon_Matchings == 0 && channel[i] == "emu" && 
                                                                 !(event->TMEME == 11 || event->TMEME == 11001 || event->TMEME == 10110 || event->TMEME == 21100 || //emu
                                                                  event->TMEME == 1 || event->TMEME == 10 || // l(=e/mu)jets
                                                                  event->TMEME == 2 || event->TMEME == 10101 || event->TMEME == 20200 || // ee
		                                                  event->TMEME == 20 || event->TMEME == 11010 || event->TMEME == 22000 || // mumu
                                                                  event->TMEME == 10010 || event->TMEME == 10001 || // l(=e/mu)tau(->had)
                                                                  event->TMEME == 10100 || event->TMEME == 11000) // tau(->e/mu)jets
           ){
             FF_QCD[2]->Fill(njets,weight);
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


  for (unsigned int i = 0; i < 3; i++) {
   for(unsigned int bin_index=0; bin_index<NJets_N_bins; bin_index++){
    cout << "\\begin{table}" << std::endl;
    cout << "\\begin{center}" << std::endl;
    cout << "\\begin{tabular}{|";
    for(unsigned int column_index=0; column_index<3; column_index++){
     cout << " c |";
    }
    cout << "}" << std::endl;
    cout << " \\hline" << std::endl;
    cout << " True True   & True False & False False \\\\ \\hline" << std::endl;
    cout << " Signal-like & " << TT_Signal[i]->GetBinContent(bin_index) << " & " << TF_Signal[i]->GetBinContent(bin_index) << " & " << FF_Signal[i]->GetBinContent(bin_index) <<  "\\\\ \\hline" << std::endl;
    cout << " W-like & " << TT_Wjets[i]->GetBinContent(bin_index) << " & " << TF_Wjets[i]->GetBinContent(bin_index) << " & " << FF_Wjets[i]->GetBinContent(bin_index) << "\\\\ \\hline" << std::endl;
    cout << " QCD-like & " << TT_QCD[i]->GetBinContent(bin_index) << " & " << TF_QCD[i]->GetBinContent(bin_index) << " & " << FF_QCD[i]->GetBinContent(bin_index) << "\\\\" << std::endl;
    cout << " \\hline" << std::endl;
    cout << "\\end{tabular}" << std::endl;
    cout << "\\caption{FIX ME} \\label{FIX ME}" << std::endl;
    cout << "\\end{center}" << std::endl;
    cout << "\\end{table}" << std::endl;
   }
  }






  TFile* file_MM = new TFile("MatrixMethod_Contamination_Studies.root","RECREATE");
  file_MM->cd();


  for (unsigned int i = 0; i < 3; i++) {
   TT_Signal[i]->Write();
   TT_Signal[i] = 0;
   delete TT_Signal[i];

   TT_Wjets[i]->Write();
   TT_Wjets[i] = 0;
   delete TT_Wjets[i];

   TT_QCD[i]->Write();
   TT_QCD[i] = 0;
   delete TT_QCD[i];


   TF_Signal[i]->Write();
   TF_Signal[i] = 0;
   delete TF_Signal[i];

   TF_Wjets[i]->Write();
   TF_Wjets[i] = 0;
   delete TF_Wjets[i];

   TF_QCD[i]->Write();
   TF_QCD[i] = 0;
   delete TF_QCD[i];


   FF_Signal[i]->Write();
   FF_Signal[i] = 0;
   delete FF_Signal[i];

   FF_Wjets[i]->Write();
   FF_Wjets[i] = 0;
   delete FF_Wjets[i];

   FF_QCD[i]->Write();
   FF_QCD[i] = 0;
   delete FF_QCD[i];
  }




  file_MM->Write();
  file_MM->Close();
  delete file_MM;

  cout<<"#########################"<<endl;
  cout<<"    End of the program   "<<endl;
  cout<<"#########################"<<endl;

  return (0);
}
