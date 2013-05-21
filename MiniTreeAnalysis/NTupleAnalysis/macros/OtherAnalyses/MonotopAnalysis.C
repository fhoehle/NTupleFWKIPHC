#include <iomanip>
#include <iostream>
#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/MuonJetsSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

using namespace TopTree;
using namespace std;

int main ()
{
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;
  
  //////////////////////
  //Global variables
  //////////////////////
  vector < Dataset > datasets;
  MuonJetsSelection sel; 
  float Luminosity = 0;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType = 0; 
  int verbosity = -1;

  //////////////////////

  //////////////////////
  // Initialisation
  //////////////////////
  
  string xmlFileName = string ("../../config/MonotopAnalysis.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSelection(sel);
  //anaEL.LoadMuonJetsSelection (sel); // now the parameters for the selection are given to the selection
  //anaEL.LoadDiLeptonSelection (sel); 
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  //anaEL.LoadWeight (sel);
  TopTree::NTEvent * event = 0;
  
  /*string xmlFileName = string ("../config/MyCutFlow.xml");
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadDiLeptonSelection (sel); // now the parameters for the selection are given to the selection
  anaEL.LoadGeneralInfo(DataType, Luminosity, verbosity );
  anaEL.LoadWeight (sel); // now the parameters for SFBweight are initialized (for b-tag!)
  TopTree::NTEvent * event = 0;
   */
   
  
  
     
  vector<string> CutName;
  CutName.push_back("Total");
  CutName.push_back("Trigger");
  CutName.push_back("LeptonVeto");
  CutName.push_back("met");
  CutName.push_back("3jets");
  CutName.push_back("1 b-tagged jet");
  CutName.push_back("Wmass");
  CutName.push_back("top mass");
   
  HistoManager MyhistoManager;
  MyhistoManager.LoadDatasets(datasets);
  MyhistoManager.LoadSelectionSteps(CutName);
  
  
  std::vector<TH1F> h_MET;
  std::vector<TH1F> h_JetPt;
  std::vector<TH1F> h_JetEta;
  std::vector<TH1F> h_JetPhi;
  std::vector<TH1F> h_JetPt_aftermetJetSel;
  std::vector<TH1F> h_JetEta_aftermetJetSel;
  std::vector<TH1F> h_JetPhi_aftermetJetSel;
  std::vector<TH1F> h_DeltaPhiJetMET_aftermetJetSel;
  std::vector<TH1F> h_DeltaPhiTopMET;
  
  std::vector<TH1F> h_JetBtag;
  std::vector<TH1F> h_DijetInvM;
  std::vector<TH1F> h_TrijetInvM;
  std::vector<TH1F> h_TrijetPt;
  std::vector<TH2D> h_TrijetPt_TrijetInvM;
  
  
  double Dweight[101]; 
  int ITypeMC	  = -1;
  int ICut	  = -1;  
  int IChannel    = -1;  
    
  //////////////////////
  // The output tree
  //////////////////////
  
  TFile * theOutfileTree = new TFile("outputtree.root", "recreate");
  
  int nmuon;
  double muon_pt[100000] ;
  double muon_eta[100000];
  double muon_phi[100000];
  int    muon_charge[100000];
  
  int nelec;
  double elec_pt[100000] ;
  double elec_eta[100000];
  double elec_phi[100000];
  int    elec_charge[100000];
  
  int njets;
  double jets_pt[100000] ;
  double jets_eta[100000];
  double jets_phi[100000];
  int    jets_flavor[100000];
  int    jets_BTagDiscr[100000];
  
  
  double missET;
  
  int tmeme;
  
  TTree * smalltree = new TTree("Tree","Tree");
  
  smalltree->Branch("nmuon",&nmuon,"nmuon/I");
  smalltree->Branch("muon_pt",  muon_pt ,  "muon_pt[nmuon]/D" );
  smalltree->Branch("muon_eta", muon_eta , "muon_eta[nmuon]/D");
  smalltree->Branch("muon_phi", muon_phi , "muon_phi[nmuon]/D");
  smalltree->Branch("muon_charge", muon_charge , "muon_charge[nmuon]/I");
  
  smalltree->Branch("nelec",&nelec,"nelec/I");
  smalltree->Branch("elec_pt",  elec_pt ,  "elec_pt[nelec]/D" );
  smalltree->Branch("elec_eta", elec_eta , "elec_eta[nelec]/D");
  smalltree->Branch("elec_phi", elec_phi , "elec_phi[nelec]/D");
  smalltree->Branch("elec_charge", elec_charge , "elec_charge[nelec]/I");
  
  smalltree->Branch("njets",&njets,"njets/I");
  smalltree->Branch("jets_pt",  jets_pt ,  "jets_pt[njets]/D" );
  smalltree->Branch("jets_eta", jets_eta , "jets_eta[njets]/D");
  smalltree->Branch("jets_phi", jets_phi , "jets_phi[njets]/D");
  smalltree->Branch("jets_flavor", jets_flavor, "jets_flavor[njets]/I");
  smalltree->Branch("jets_BTagDiscr", jets_BTagDiscr, "jets_BTagDiscr[njets]/I");
  smalltree->Branch("missET",&missET,"missET/D");
  smalltree->Branch("tmeme",&tmeme,"tmeme/I");
  
  
  
  //////////////////////
  //LOOP OVER THE DATASETS
  //////////////////////
  if(verbosity>0) {
  	cout<<"#########################"<<endl;
  	cout<<" Loop over the datasets  "<<endl;
	cout<<"#########################"<<endl;
  }
 
  for (unsigned int d = 0; d < datasets.size (); d++) {

     MyhistoManager.CreateHisto(h_MET,        "MET"        , datasets[d].Name(),"MET"        ,"Entries",500,0.,1000);
     MyhistoManager.CreateHisto(h_JetPt,      "JetPt"      , datasets[d].Name(),"JetPt"      ,"Entries",50,0.,500 );
     MyhistoManager.CreateHisto(h_JetEta,     "JetEta"     , datasets[d].Name(),"JetEta"     ,"Entries",50,0.,3.  );
     MyhistoManager.CreateHisto(h_JetPhi,     "JetPhi"     , datasets[d].Name(),"JetPhi"     ,"Entries",50,0.,3.  );
     MyhistoManager.CreateHisto(h_JetBtag,    "JetBtag"    , datasets[d].Name(),"JetBtag"    ,"Entries",50,0.,500);
     MyhistoManager.CreateHisto(h_DijetInvM,  "DijetInvM"  , datasets[d].Name(),"DijetInvM"  ,"Entries",50,0.,500);
     MyhistoManager.CreateHisto(h_TrijetInvM, "TrijetInvM" , datasets[d].Name(),"TrijetInvM" ,"Entries",50,0.,500);
     MyhistoManager.CreateHisto(h_TrijetPt,   "TrijetPt"   , datasets[d].Name(),"TrijetPt"   ,"Entries",50,0.,500);
     
     MyhistoManager.CreateHisto(h_JetPt_aftermetJetSel,      "JetPt_aftermetJetSel"      , datasets[d].Name(),"JetPt_aftermetJetSel"      ,"Entries",50,0.,500 );
     MyhistoManager.CreateHisto(h_JetEta_aftermetJetSel,     "JetEta_aftermetJetSel"     , datasets[d].Name(),"JetEta_aftermetJetSel"     ,"Entries",50,0.,3.  );
     MyhistoManager.CreateHisto(h_JetPhi_aftermetJetSel,     "JetPhi_aftermetJetSel"     , datasets[d].Name(),"JetPhi_aftermetJetSel"     ,"Entries",50,0.,3.  );
  
     MyhistoManager.CreateHisto(h_DeltaPhiJetMET_aftermetJetSel,     "DeltaPhiJetMET_aftermetJetSel"     ,
     datasets[d].Name(),"DeltaPhiJetMET_aftermetJetSel"     ,"Entries",200,-3.14,3.14  );
     MyhistoManager.CreateHisto(h_DeltaPhiTopMET,     "DeltaPhiTopMET", datasets[d].Name(),"DeltaPhiTopMET"     ,"Entries",50,-3.14,3.14  );

    //TBranch *evtBR = (TBranch *) (datasets[d].eventTree ()->GetBranch ("NTEvent"));
    //evtBR->SetAddress (&event);
    
    
     MyhistoManager.CreateHisto2D (h_TrijetPt_TrijetInvM, "TrijetPt_TrijetInvM", datasets[d].Name (), "TrijetPt", "TrijetInvM",50, 0., 500,  50, 0., 500);
     
     
    datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    
    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    
   
    /*datasets[d].eventTree ()->SetBranchAddress ("NTEvent",&event);
    
    unsigned int nEvents = (int) (datasets[d].eventTree ()->GetEntries ());
    cout << "NEvents = " << nEvents << endl;*/
   
  
    
    
    for (unsigned int ievt = 0; ievt < nEvents; ievt++) {
      float weight = 1.;
      
      datasets[d].eventTree ()->GetEntry (ievt);
      //datasets[d].eventTree ()->Print();
      //evtBR->GetEntry (ievt);
      
            
     //cout << "event number=" << event->eventNb << endl;
     //Load event for the selection
     sel.LoadEvent(event);
     
     
     double weightITypeMC = Luminosity*datasets[d].Xsection()/datasets[d].getNSkimmedEvent();
     
     if(ievt%10000 == 0) cout << "the number of processed event is " << ievt << endl; 
     
      //MyhistoManager.FillHisto(, "InvDilMassPair" ,InvDilMass, datasets[d].Name(), IsSignal, Dweight[ITypeMC]);

     //Collection of selected objects
     //cout << " get selected electron " << endl;
      
     vector<NTElectron> selElectrons = sel.GetSelectedElectrons();      
     vector<NTMuon>     selMuons = sel.GetSelectedMuons();
     vector<NTJet>      selJets = sel.GetSelectedJets();
     vector<NTJet>      selJetsAnalysis;
     NTMET met = sel.GetMET(); // no criteria applyied
     
     nmuon = 0;
     for(unsigned int imuon=0; imuon < selMuons.size(); imuon++){
       muon_pt[nmuon]  = selMuons[imuon].p4.Pt();
       muon_eta[nmuon] = selMuons[imuon].p4.Eta();
       muon_phi[nmuon] = selMuons[imuon].p4.Phi() ;
       muon_charge[nmuon] = selMuons[imuon].Charge;
       nmuon++;
     }
      
     nelec = 0;
     for(unsigned int ielec=0; ielec < selElectrons.size(); ielec++){
       elec_pt[nelec]  = selElectrons[ielec].p4.Pt();
       elec_eta[nelec] = selElectrons[ielec].p4.Eta();
       elec_phi[nelec] = selElectrons[ielec].p4.Phi() ;
       elec_charge[nelec] = selElectrons[ielec].Charge;
       nelec++;
     }
     
     njets = 0;
     for(unsigned int ijets=0; ijets < selJets.size(); ijets++){
       jets_pt[njets]  = selJets[ijets].p4.Pt();
       jets_eta[njets] = selJets[ijets].p4.Eta();
       jets_phi[njets] = selJets[ijets].p4.Phi() ;
       jets_flavor[njets] = selJets[ijets].partonFlavour;
       jets_BTagDiscr[njets] = selJets[ijets].SVDiscri;
       njets++;
     }
 
     missET= met.p4.Et();
     tmeme = event->TMEME;
     
     
    if ( datasets[d].Name()=="Data" ) weightITypeMC = 1; 
    bool IsSignal = false;
    int LastStep = 0;
    bool IsData = false;
    if ( datasets[d].Name()=="Data") { 
       ITypeMC = 100; IsData = true;  Dweight[ITypeMC]= weightITypeMC; 
     }
    
     
     
    MyhistoManager.FillHisto(h_MET, "MET" ,missET, datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
    bool passmetjetmatch = false;
    if(missET >  65){ 
      int nselJet    = 0;
      int nBJetMedium = 0;
      int nBJetLoose  = 0;
      for(unsigned int ijets=0; ijets < selJets.size(); ijets++){
        MyhistoManager.FillHisto(h_JetPt,  "JetPt"  ,selJets[ijets].p4.Pt() , datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
        MyhistoManager.FillHisto(h_JetEta, "JetEta" ,selJets[ijets].p4.Eta(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
        MyhistoManager.FillHisto(h_JetPhi, "JetPhi" ,selJets[ijets].p4.Phi(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
        if(selJets[ijets].p4.Pt() > 50 && fabs(selJets[ijets].p4.Eta()) < 2.5 ){
	    nselJet++;
	    selJetsAnalysis.push_back(selJets[ijets]);
	    if( selJets[ijets].SVDiscri > 1.74) nBJetMedium++;
	    if( selJets[ijets].TCDiscri > 1.7 ) nBJetLoose++;
	    
	    if(fabs(selJets[ijets].p4.Phi() - met.p4.Phi()) > 0.1 
	     ) passmetjetmatch = true;
	    
	}
	}
	
	
	if(nselJet == 3){
	  if(nBJetMedium == 1 ){
	    if(nBJetLoose == 1){
	      std::vector<TLorentzVector> bjets;
	      bool foundb = false;
	      std::vector<TLorentzVector> qjets;
	      
	      for(unsigned int iseljets = 0; iseljets < selJetsAnalysis.size(); iseljets++){
	       if( fabs(met.p4.DeltaPhi(selJetsAnalysis[iseljets].p4)) > 0.6 ){
	        if(selJetsAnalysis[iseljets].SVDiscri > 1.74){ 
		 TLorentzVector bjet;
		 bjet.SetPtEtaPhiE( 
			selJetsAnalysis[iseljets].p4.Pt(), 
			selJetsAnalysis[iseljets].p4.Eta(), 
			selJetsAnalysis[iseljets].p4.Phi(), 
			selJetsAnalysis[iseljets].p4.E()
			);
			foundb = true;
			bjets.push_back(bjet);
			}
		 else{
		   TLorentzVector qjet;
		    qjet.SetPtEtaPhiE( 
			selJetsAnalysis[iseljets].p4.Pt(), 
			selJetsAnalysis[iseljets].p4.Eta(), 
			selJetsAnalysis[iseljets].p4.Phi(), 
			selJetsAnalysis[iseljets].p4.E()
			);
		   qjets.push_back(qjet);
		 }
		 }
	      }
	      if(qjets.size() == 2 && bjets.size() == 1 ){
	      TLorentzVector bjet;
		 bjet.SetPtEtaPhiE( 
			bjets[0].Pt(), 
			bjets[0].Eta(), 
			bjets[0].Phi(), 
			bjets[0].E()
			);
	      MyhistoManager.FillHisto(h_JetPt_aftermetJetSel,  "JetPt_aftermetJetSel"  ,bjet.Pt() , datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
              MyhistoManager.FillHisto(h_JetEta_aftermetJetSel, "JetEta_aftermetJetSel" ,bjet.Eta(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
              MyhistoManager.FillHisto(h_JetPhi_aftermetJetSel, "JetPhi_aftermetJetSel" ,bjet.Phi(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
              
	       
	      MyhistoManager.FillHisto(h_JetPt_aftermetJetSel,  "JetPt_aftermetJetSel"  ,qjets[0].Pt() , datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
              MyhistoManager.FillHisto(h_JetEta_aftermetJetSel, "JetEta_aftermetJetSel" ,qjets[0].Eta(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
              MyhistoManager.FillHisto(h_JetPhi_aftermetJetSel, "JetPhi_aftermetJetSel" ,qjets[0].Phi(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
             
	       
	      MyhistoManager.FillHisto(h_JetPt_aftermetJetSel,  "JetPt_aftermetJetSel"  ,qjets[1].Pt() , datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
              MyhistoManager.FillHisto(h_JetEta_aftermetJetSel, "JetEta_aftermetJetSel" ,qjets[1].Eta(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
              MyhistoManager.FillHisto(h_JetPhi_aftermetJetSel, "JetPhi_aftermetJetSel" ,qjets[1].Phi(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
             
	      
	      
	      TLorentzVector W = qjets[0]+qjets[1];
	      MyhistoManager.FillHisto(h_DijetInvM, "DijetInvM" ,W.M(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	      TLorentzVector top = qjets[0]+qjets[1]+bjet;
	      
	      
	      MyhistoManager.FillHisto(h_DeltaPhiJetMET_aftermetJetSel, "DeltaPhiJetMET_aftermetJetSel", met.p4.DeltaPhi(bjet), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	      MyhistoManager.FillHisto(h_DeltaPhiJetMET_aftermetJetSel, "DeltaPhiJetMET_aftermetJetSel", met.p4.DeltaPhi(qjets[0]), datasets[d].Name(), IsSignal, Dweight[ITypeMC]); 
	      MyhistoManager.FillHisto(h_DeltaPhiJetMET_aftermetJetSel, "DeltaPhiJetMET_aftermetJetSel", met.p4.DeltaPhi(qjets[1]), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	      
	      if(W.M() > 60 && W.M() < 100  ) {
	        MyhistoManager.FillHisto(h_TrijetInvM, "TrijetInvM" ,top.M(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	        MyhistoManager.FillHisto(h_TrijetPt, "TrijetPt" ,top.Pt(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
		MyhistoManager.FillHisto(h_DeltaPhiTopMET, "DeltaPhiTopMET",met.p4.DeltaPhi(top), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
	        MyhistoManager.FillHisto2D(h_TrijetPt_TrijetInvM, "TrijetPt_TrijetInvM" ,top.Pt(), top.M(), datasets[d].Name(), IsSignal, Dweight[ITypeMC]);
 	      }
	      }
	    }
	  }
	
	}
	
        
      }
    
     
     
     
     
     
     
     
     smalltree->Fill();
     
     
     

      
     if(verbosity > 2) {
     	//cout<<event->electrons.size()<<" electrons - "<<event->muons.size()<<" muons - "<<endl;
     	//cout<<selElectrons.size()<<" sel electrons - "<<selMuons.size()<<" sel muons - "<<endl;
     }
     
     //integer which define the last step of the selection that the event fullfill
     int selLastStep = 0;
     //boolean which is true if the event pass the selection
     bool isSelEvent = false;	
     string theChannel = "TTbarMuJets" ;
    
      


    }				// end of loop over evts
   
  }				// end of loop over the datasets 
  cout<<"#########################"<<endl;
  cout<<" Loop over the datasets  "<<endl;
  cout<<"#########################"<<endl;
 
  smalltree->Write();
  MyhistoManager.WriteMyHisto(h_MET,	     "all" );
  MyhistoManager.WriteMyHisto(h_JetPt,      "all" );
  MyhistoManager.WriteMyHisto(h_JetEta,     "all" );
  MyhistoManager.WriteMyHisto(h_JetPhi,     "all" );
  MyhistoManager.WriteMyHisto(h_JetBtag,    "all" );
  MyhistoManager.WriteMyHisto(h_DijetInvM,  "all" );
  MyhistoManager.WriteMyHisto(h_TrijetInvM, "all" );
  MyhistoManager.WriteMyHisto(h_TrijetPt,   "all" );
  MyhistoManager.WriteMyHisto(h_DeltaPhiJetMET_aftermetJetSel,   "all" );
  MyhistoManager.WriteMyHisto(h_JetPt_aftermetJetSel,      "all" );
  MyhistoManager.WriteMyHisto(h_JetEta_aftermetJetSel,     "all" );
  MyhistoManager.WriteMyHisto(h_JetPhi_aftermetJetSel,     "all" );
  MyhistoManager.WriteMyHisto(h_DeltaPhiTopMET,     "all" );
  MyhistoManager.WriteMyHisto2D(h_TrijetPt_TrijetInvM,     "all" );
  ////////////////////////////
  //  Computation after loops
  ////////////////////////////

  return (0);
}
